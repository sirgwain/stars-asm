; Popup  (popup)
;   addr: 0019:0c7c  len=1775
;   sig:  void Popup(HWND hwnd, int16_t x, int16_t y)
;   params:
;     HWND             hwnd           [BP+0x6]
;     int16_t          x              [BP+0x8]
;     int16_t          y              [BP+0xa]
;   locals:
;     POINT16          ptT            [BP-0x18]
;     int16_t          dx             [BP-0x14]
;     char *           psz            [BP-0x12]
;     HFONT            hfontSav       [BP-0x10]
;     int16_t          c              [BP-0xe]
;     int16_t          i              [BP-0xc]
;     int16_t          dy             [BP-0xa]
;     POINT16          pt             [BP-0x8]
;     HDC              hdc            [BP-0x4]
;     block 0019:0D22  len=0x88
;       char *           psz            [BP-0x1c]
;       int16_t          dx2            [BP-0x1a]
;     block 0019:0DAA  len=0x22D
;       char *           psz            [BP-0x24]
;       int16_t          dxR            [BP-0x22]
;       char *           lpsz           [BP-0x20]
;       int16_t          dxL            [BP-0x1c]
;       int16_t          dxDamage       [BP-0x1a]
;       block 0019:0E46  len=0x131
;         char[40]         szTB           [BP-0x4c]
;     block 0019:0FD7  len=0xBB
;       int16_t          dxCoord        [BP-0x1e]
;       char *           psz            [BP-0x1c]
;       int16_t          dxName         [BP-0x1a]
;
;   stats: blocks=4  labels=1
;     SetDxDy: L_1113

L_0c7c:                             ; popup.c:310
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x004c          
PUSH      si                  
PUSH      di                  
                                    ; popup.c:320
MOV       ax, [bp+x]                ; ax, [bp+0x8]
MOV       [bp-pt], ax               ; [bp-0x8], ax
                                    ; popup.c:321
MOV       ax, [bp+y]                ; ax, [bp+0xa]
MOV       [bp-pt+0x2], ax           ; [bp-0x6], ax
                                    ; popup.c:323
PUSH      [bp+hwnd]                 ; [bp+0x6]
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ClientToScreen            ; void ClientToScreen(HWND arg1, POINT *arg2)
                                    ; popup.c:324
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     GetDC                     ; HDC GetDC(HWND arg1)
MOV       [bp-hdc], ax              ; [bp-0x4], ax
                                    ; popup.c:325
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8]           ; [0x26b4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
MOV       [bp-hfontSav], ax         ; [bp-0x10], ax
                                    ; popup.c:328
MOV       ax, [GlobalPD]            ; ax, [0x0b80]
JMP       L_11f5              

L_0cc3:                             ; popup.c:334
MOV       ax, 0x025e          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x12], ax
                                    ; popup.c:335
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x12]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x12]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
ADD       ax, 0x0008          
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:336
MOV       ax, 0x0003          
IMUL      [dyArial8]                ; [0x23fa]
ADD       ax, 0x0008          
MOV       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:337
CMP       [GlobalPD+0x14], 0x0000   ; [0x0b94], 0x0000
JL        L_1225              

L_0d0a:
JG        L_0d19              

L_0d0f:
CMP       [GlobalPD+0x12], 0x0000   ; [0x0b92], 0x0000
JC        L_1225              

L_0d19:                             ; popup.c:338
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       [bp-dy], ax               ; [bp-0xa], ax

L_0d1f:                             ; popup.c:339
JMP       L_1225              

L_0d22:                             ; popup.c:344
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:345
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [GlobalPD+0x2]            ; [0x0b82]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-psz], ax              ; [bp-0x1c], ax
                                    ; popup.c:347
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x1c]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x1c]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
ADD       ax, 0x0008          
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:348
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x0bfe          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
ADD       ax, 0x0008          
MOV       [bp-dx2], ax              ; [bp-0x1a], ax
                                    ; popup.c:349
MOV       ax, [bp-dx]               ; ax, [bp-0x14]
CMP       [bp-dx2], ax              ; [bp-0x1a], ax
JLE       L_0d9c              

L_0d96:                             ; popup.c:350
MOV       ax, [bp-dx2]              ; ax, [bp-0x1a]
MOV       [bp-dx], ax               ; [bp-0x14], ax

L_0d9c:                             ; popup.c:352
MOV       ax, [dyArial8]            ; ax, [0x23fa]
SHL       ax, 0x0001          
ADD       ax, 0x0008          
MOV       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:353
JMP       L_1225              

L_0daa:                             ; popup.c:358
MOV       [bp-dxR], 0x0000          ; [bp-0x22], 0x0000
                                    ; popup.c:359
MOV       [bp-dxDamage], 0x0000     ; [bp-0x1a], 0x0000
                                    ; popup.c:363
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0008          
MOV       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:365
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:366
MOV       ax, 0x025b          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x24], ax
                                    ; popup.c:367
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x24]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x24]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dxL], ax              ; [bp-0x1c], ax
                                    ; popup.c:368
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8]           ; [0x26b4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:370
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_0f7b              

L_0e0a:                             ; popup.c:372
MOV       ax, 0x000c          
MOV       bx, [GlobalPD+0x2]        ; bx, [0x0b82]
MOV       cx, [GlobalPD+0x4]        ; cx, [0x0b84]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JLE       L_0f77              

L_0e29:
CMP       [GlobalPD+0xa], 0x0000    ; [0x0b8a], 0x0000
JZ        L_0e46              

L_0e33:
PUSH      [bp-i]                    ; [bp-0xc]
CALLF     FIsPopupHullType          ; int16_t FIsPopupHullType(int16_t ishdef)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_0f77              

L_0e46:                             ; popup.c:375
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:377
LEA       ax, [bp-szTB]             ; ax, [bp-0x4c]
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xc]
LES       bx, [GlobalPD+0x2]        ; bx, [0x0b82]
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x000f          
PUSH      ax                  
CALLF     DecorateHullName          ; void DecorateHullName(int16_t iplr, int16_t ish, char *psz)
ADD       sp, 0x0006          
                                    ; popup.c:378
LEA       ax, [bp-szTB]             ; ax, [bp-0x4c]
MOV       dx, ss              
MOV       [bp-lpsz], ax             ; [bp-0x20], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x1e], dx
                                    ; popup.c:379
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-lpsz+0x2]             ; [bp-0x1e]
PUSH      [bp-lpsz]                 ; [bp-0x20]
PUSH      [bp-lpsz+0x2]             ; [bp-0x1e]
PUSH      [bp-lpsz]                 ; [bp-0x20]
CALLF     fstrlen                   ; uint16_t fstrlen(char *s)
ADD       sp, 0x0004          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:380
MOV       ax, [bp-dx]               ; ax, [bp-0x14]
CMP       [bp-dxL], ax              ; [bp-0x1c], ax
JLE       L_0ea7              

L_0ea1:
MOV       ax, [bp-dxL]              ; ax, [bp-0x1c]
JMP       L_0eaa              

L_0ea7:
MOV       ax, [bp-dx]               ; ax, [bp-0x14]

L_0eaa:
MOV       [bp-dxL], ax              ; [bp-0x1c], ax
                                    ; popup.c:381
MOV       ax, 0x000c          
MOV       bx, [GlobalPD+0x2]        ; bx, [0x0b82]
MOV       cx, [GlobalPD+0x4]        ; cx, [0x0b84]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
PUSH      es:[bx]             
MOV       ax, [PCTD]                ; ax, [0x01aa]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; popup.c:382
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:383
MOV       ax, [bp-dx]               ; ax, [bp-0x14]
CMP       [bp-dxR], ax              ; [bp-0x22], ax
JLE       L_0f05              

L_0eff:
MOV       ax, [bp-dxR]              ; ax, [bp-0x22]
JMP       L_0f08              

L_0f05:
MOV       ax, [bp-dx]               ; ax, [bp-0x14]

L_0f08:
MOV       [bp-dxR], ax              ; [bp-0x22], ax
                                    ; popup.c:384
CMP       [GlobalPD+0x6], 0x0000    ; [0x0b86], 0x0000
JZ        L_0f77              

L_0f15:
MOV       ax, 0x002c          
MOV       bx, [GlobalPD+0x2]        ; bx, [0x0b82]
MOV       cx, [GlobalPD+0x4]        ; cx, [0x0b84]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0007          
SHR       ax, cx              
AND       ax, 0x01ff          
CMP       ax, 0x0000          
JZ        L_0f77              

L_0f3e:
CMP       [bp-dxDamage], 0x0000     ; [bp-0x1a], 0x0000
JNZ       L_0f77              

L_0f47:                             ; popup.c:386
MOV       ax, 0x050b          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x24], ax
                                    ; popup.c:387
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x24]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x24]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
ADD       ax, 0x0004          
MOV       [bp-dxDamage], ax         ; [bp-0x1a], ax

L_0f77:                             ; popup.c:390
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_0f7b:
CMP       [bp-i], 0x0010            ; [bp-0xc], 0x0010
JL        L_0e0a              

L_0f84:                             ; popup.c:391
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0008          
CMP       [bp-dy], ax               ; [bp-0xa], ax
JNZ       L_0fbf              

L_0f92:                             ; popup.c:393
MOV       ax, 0x025b          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x24], ax
                                    ; popup.c:394
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x24]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x24]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dxL], ax              ; [bp-0x1c], ax

L_0fbf:                             ; popup.c:397
MOV       ax, [bp-dxDamage]         ; ax, [bp-0x1a]
MOV       [GlobalPD+0x8], ax        ; [0x0b88], ax
                                    ; popup.c:398
MOV       ax, [bp-dxL]              ; ax, [bp-0x1c]
ADD       ax, [bp-dxR]              ; ax, [bp-0x22]
ADD       ax, 0x0010          
ADD       ax, [bp-dxDamage]         ; ax, [bp-0x1a]
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:399
JMP       L_1225              

L_0fd7:                             ; popup.c:407
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:408
MOV       ax, [dyArial8]            ; ax, [0x23fa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       ax, 0x0008          
MOV       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:410
MOV       ax, 0x025c          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x1c], ax
                                    ; popup.c:411
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x1c]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x1c]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
ADD       ax, 0x0008          
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:412
PUSH      [sel+0x14]                ; [0x496a]
CALLF     PszGetPlanetName          ; char * PszGetPlanetName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x1c], ax
                                    ; popup.c:413
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8]           ; [0x26b4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:414
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-psz]              ; ax, [bp-0x1c]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x1c]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dxName], ax           ; [bp-0x1a], ax
                                    ; popup.c:415
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x025d          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dxCoord], ax          ; [bp-0x1e], ax
                                    ; popup.c:416
MOV       ax, [bp-dxCoord]          ; ax, [bp-0x1e]
CMP       [bp-dxName], ax           ; [bp-0x1a], ax
JLE       L_1089              

L_1083:
MOV       ax, [bp-dxName]           ; ax, [bp-0x1a]
JMP       L_108c              

L_1089:
MOV       ax, [bp-dxCoord]          ; ax, [bp-0x1e]

L_108c:
ADD       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:417
JMP       L_1225              

L_1092:                             ; popup.c:420
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayPlanetStateInfo  ; POINT16 PtDisplayPlanetStateInfo(HDC hdc, int16_t fPrint)
ADD       sp, 0x0004          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx
                                    ; popup.c:421
JMP       SetDxDy             

L_10aa:                             ; popup.c:424
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayZipOrdInfo       ; POINT16 PtDisplayZipOrdInfo(HDC hdc, int16_t xCtr, int16_t fPrint)
ADD       sp, 0x0006          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx
                                    ; popup.c:425
JMP       SetDxDy             

L_10c6:                             ; popup.c:428
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayPlanetPopInfo    ; POINT16 PtDisplayPlanetPopInfo(HDC hdc, int16_t fPrint)
ADD       sp, 0x0004          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx
                                    ; popup.c:429
JMP       SetDxDy             

L_10de:                             ; popup.c:432
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x00c8          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayResourceInfo     ; POINT16 PtDisplayResourceInfo(HDC hdc, int16_t dx, int16_t fPrint)
ADD       sp, 0x0006          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx
                                    ; popup.c:433
JMP       SetDxDy             

L_10fa:                             ; popup.c:436
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x00c8          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayFactoryMineInfo  ; POINT16 PtDisplayFactoryMineInfo(HDC hdc, int16_t dx, int16_t fPrint)
ADD       sp, 0x0006          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx

SetDxDy:                            ; popup.c:438
MOV       ax, [bp-ptT]              ; ax, [bp-0x18]
ADD       ax, 0x0002          
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:439
MOV       ax, [bp-ptT+0x2]          ; ax, [bp-0x16]
ADD       ax, 0x0002          
MOV       [bp-dy], ax               ; [bp-0xa], ax
                                    ; popup.c:440
JMP       L_1225              

L_1128:                             ; popup.c:443
CMP       [dyArial8], 0x000e        ; [0x23fa], 0x000e
JLE       L_1138              

L_1132:
MOV       ax, 0x0028          
JMP       L_113b              

L_1138:
MOV       ax, 0x0000          

L_113b:
ADD       ax, 0x0158          
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; popup.c:444
MOV       ax, 0x000c          
IMUL      [dyArial8]                ; [0x23fa]
MOV       cx, [dyArial10]           ; cx, [0x530a]
ADD       cx, 0x0048          
ADD       cx, ax              
ADD       cx, 0x0006          
MOV       [bp-dy], cx               ; [bp-0xa], cx
                                    ; popup.c:445
JMP       L_1225              

L_115a:                             ; popup.c:448
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [GlobalPD+0x2]            ; [0x0b82]
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     PtDisplayString           ; POINT16 PtDisplayString(HDC hdc, int16_t dx, int16_t fPrint)
ADD       sp, 0x0006          
MOV       [bp-ptT], ax              ; [bp-0x18], ax
MOV       [bp-ptT+0x2], dx          ; [bp-0x16], dx
                                    ; popup.c:449
JMP       SetDxDy             

L_1176:                             ; popup.c:453
CMP       [GlobalPD], 0x000b        ; [0x0b80], 0x000b
JNZ       L_1186              

L_1180:
MOV       ax, 0x0000          
JMP       L_1189              

L_1186:
MOV       ax, 0x0001          

L_1189:
MOV       [mdBuild], ax             ; [0x5466], ax
                                    ; popup.c:454
MOV       ax, [GlobalPD+0x2]        ; ax, [0x0b82]
MOV       dx, [GlobalPD+0x4]        ; dx, [0x0b84]
MOV       [lpshdefBuild], ax        ; [0x5202], ax
MOV       [lpshdefBuild+0x2], dx    ; [0x5204], dx
                                    ; popup.c:455
CALLF     UpdateSlotGlobals         ; void UpdateSlotGlobals()
                                    ; popup.c:456
MOV       [bp-dx], 0x0154           ; [bp-0x14], 0x0154
                                    ; popup.c:457
MOV       ax, 0x0006          
IMUL      [dyArial8]                ; [0x23fa]
MOV       cx, [dyArial8]            ; cx, [0x23fa]
ADD       cx, 0x0132          
ADD       cx, ax              
ADD       cx, 0x0008          
MOV       [bp-dy], cx               ; [bp-0xa], cx
                                    ; popup.c:458
MOV       cx, 0x000e          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0003          
CMP       ax, 0x0000          
JBE       L_1225              

L_11ce:
CMP       [GlobalPD], 0x000b        ; [0x0b80], 0x000b
JNZ       L_1225              

L_11d8:                             ; popup.c:459
MOV       ax, 0x0003          
IMUL      [dyArial8]                ; [0x23fa]
ADD       [bp-dy], ax               ; [bp-0xa], ax

L_11e2:                             ; popup.c:460
JMP       L_1225              

L_11e5:                             ; popup.c:463
MOV       [bp-dx], 0x0078           ; [bp-0x14], 0x0078
                                    ; popup.c:464
MOV       [bp-dy], 0x0050           ; [bp-0xa], 0x0050
                                    ; popup.c:465
JMP       L_1225              

L_11f5:
SUB       ax, 0x0001          
CMP       ax, 0x000d          
JA        L_1225              

L_1200:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x1209]      

L_1225:                             ; popup.c:468
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-hfontSav]             ; [bp-0x10]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; popup.c:469
PUSH      [bp+hwnd]                 ; [bp+0x6]
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     ReleaseDC                 ; int16_t ReleaseDC(HWND arg1, HDC arg2)
                                    ; popup.c:471
MOV       ax, [bp-dx]               ; ax, [bp-0x14]
SUB       [bp-pt], ax               ; [bp-0x8], ax
                                    ; popup.c:472
MOV       ax, [bp-dy]               ; ax, [bp-0xa]
SUB       [bp-pt+0x2], ax           ; [bp-0x6], ax
                                    ; popup.c:474
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dx]               ; ax, [bp-0x14]
CMP       [bp-pt], ax               ; [bp-0x8], ax
JGE       L_1261              

L_125b:
MOV       ax, [bp-pt]               ; ax, [bp-0x8]
JMP       L_126d              

L_1261:
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dx]               ; ax, [bp-0x14]

L_126d:
MOV       cx, 0x0000          
CMP       cx, ax              
JLE       L_127d              

L_1277:
MOV       ax, 0x0000          
JMP       L_12a3              

L_127d:
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dx]               ; ax, [bp-0x14]
CMP       [bp-pt], ax               ; [bp-0x8], ax
JGE       L_1297              

L_1291:
MOV       ax, [bp-pt]               ; ax, [bp-0x8]
JMP       L_12a3              

L_1297:
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dx]               ; ax, [bp-0x14]

L_12a3:
MOV       [bp-pt], ax               ; [bp-0x8], ax
                                    ; popup.c:475
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dy]               ; ax, [bp-0xa]
CMP       [bp-pt+0x2], ax           ; [bp-0x6], ax
JGE       L_12c0              

L_12ba:
MOV       ax, [bp-pt+0x2]           ; ax, [bp-0x6]
JMP       L_12cc              

L_12c0:
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dy]               ; ax, [bp-0xa]

L_12cc:
MOV       cx, 0x0000          
CMP       cx, ax              
JLE       L_12dc              

L_12d6:
MOV       ax, 0x0000          
JMP       L_1302              

L_12dc:
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dy]               ; ax, [bp-0xa]
CMP       [bp-pt+0x2], ax           ; [bp-0x6], ax
JGE       L_12f6              

L_12f0:
MOV       ax, [bp-pt+0x2]           ; ax, [bp-0x6]
JMP       L_1302              

L_12f6:
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       ax, [bp-dy]               ; ax, [bp-0xa]

L_1302:
MOV       [bp-pt+0x2], ax           ; [bp-0x6], ax
                                    ; popup.c:479
MOV       ax, 0x0210          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x9080          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-pt]                   ; [bp-0x8]
PUSH      [bp-pt+0x2]               ; [bp-0x6]
PUSH      [bp-dx]                   ; [bp-0x14]
PUSH      [bp-dy]                   ; [bp-0xa]
PUSH      [bp+hwnd]                 ; [bp+0x6]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndPopup], ax           ; [0x0b7e], ax
                                    ; popup.c:480
PUSH      [hwndPopup]               ; [0x0b7e]
MOV       ax, 0x0030          
PUSH      ax                  
PUSH      [rghfontArial8]           ; [0x26b4]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; popup.c:482
PUSH      [hwndPopup]               ; [0x0b7e]
CALLF     SetCapture                ; HWND SetCapture(HWND arg1)
                                    ; popup.c:483
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


