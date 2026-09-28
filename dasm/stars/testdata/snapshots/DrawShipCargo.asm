; DrawShipCargo  (ship)
;   addr: 000b:1a54  len=1054
;   sig:  void DrawShipCargo(HDC hdc, TILE *ptile, OBJ obj)
;   params:
;     HDC              hdc            [BP+0x6]
;     TILE *           ptile          [BP+0x8]
;     OBJ              obj            [BP+0xa]
;   locals:
;     RECT             rc             [BP-0x28]
;     int32_t          l              [BP-0x20]
;     int16_t          xLeft          [BP-0x1c]
;     RECT             rcGauge        [BP-0x1a]
;     int16_t          xRight         [BP-0x12]
;     FLEET *          pfl            [BP-0x10]
;     int16_t          c              [BP-0xe]
;     int16_t          i              [BP-0xc]
;     int16_t          yTop           [BP-0xa]
;     int32_t          l2             [BP-0x8]
;     int16_t          dxRight        [BP-0x4]
;
;   stats: blocks=0  labels=0

L_1a54:                             ; ship.c:733
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0028          
PUSH      si                  
PUSH      di                  
                                    ; ship.c:734
MOV       ax, [bp+obj]              ; ax, [bp+0xa]
MOV       [bp-pfl], ax              ; [bp-0x10], ax
                                    ; ship.c:745
MOV       bx, [bp+ptile]            ; bx, [bp+0x8]
MOV       ax, [bx+0xa]        
MOV       cx, 0x000b          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1a91              

L_1a79:                             ; ship.c:747
MOV       [rgrcRef+0x12], 0xfffb    ; [0x48c2], 0xfffb
                                    ; ship.c:748
MOV       [rgrcRef+0x16], 0xfffa    ; [0x48c6], 0xfffa
                                    ; ship.c:749
MOV       [rgrcRef+0x1a], 0xfffb    ; [0x48ca], 0xfffb
                                    ; ship.c:750
MOV       [rgrcRef+0x1e], 0xfffa    ; [0x48ce], 0xfffa

L_1a91:                             ; ship.c:753
MOV       ax, 0x0374          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x28]
PUSH      ax                  
PUSH      [bp+ptile]                ; [bp+0x8]
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     FDrawTileNC               ; int16_t FDrawTileNC(HDC hdc, TILE *ptile, RECT *prc, char *pszTitle)
ADD       sp, 0x0008          
CMP       ax, 0x0000          
JZ        L_1e6c              

L_1abb:                             ; ship.c:756
MOV       ax, [bp-rc]               ; ax, [bp-0x28]
ADD       ax, 0x0004          
MOV       [bp-xLeft], ax            ; [bp-0x1c], ax
                                    ; ship.c:757
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x24]
ADD       ax, 0xfffc          
MOV       [bp-xRight], ax           ; [bp-0x12], ax
                                    ; ship.c:758
MOV       ax, [bp-rc+0x2]           ; ax, [bp-0x26]
ADD       ax, 0x0001          
MOV       [bp-yTop], ax             ; [bp-0xa], ax
                                    ; ship.c:759
MOV       ax, [dxMaxMineralQuan]    ; ax, [0x2610]
MOV       [bp-dxRight], ax          ; [bp-0x4], ax
                                    ; ship.c:761
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; ship.c:762
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x02e9          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:763
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-l], ax                ; [bp-0x20], ax
MOV       [bp-l+0x2], dx            ; [bp-0x1e], dx
                                    ; ship.c:764
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x02ea          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:765
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-l2], ax               ; [bp-0x8], ax
MOV       [bp-l2+0x2], dx           ; [bp-0x6], dx
                                    ; ship.c:766
MOV       ax, [bp-l]                ; ax, [bp-0x20]
MOV       dx, [bp-l+0x2]            ; dx, [bp-0x1e]
CMP       [bp-l2+0x2], dx           ; [bp-0x6], dx
JL        L_1b65              

L_1b4c:
JG        L_1b59              

L_1b51:
CMP       [bp-l2], ax               ; [bp-0x8], ax
JBE       L_1b65              

L_1b59:                             ; ship.c:767
MOV       ax, [bp-l2]               ; ax, [bp-0x8]
MOV       dx, [bp-l2+0x2]           ; dx, [bp-0x6]
MOV       [bp-l], ax                ; [bp-0x20], ax
MOV       [bp-l+0x2], dx            ; [bp-0x1e], dx

L_1b65:                             ; ship.c:768
MOV       bx, [bp+ptile]            ; bx, [bp+0x8]
MOV       ax, [bx+0xa]        
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1b93              

L_1b7b:                             ; ship.c:769
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [bp-xLeft]                ; [bp-0x1c]
PUSH      [bp-yTop]                 ; [bp-0xa]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     TextOut                   ; int16_t TextOut(HDC arg1, int16_t arg2, int16_t arg3, LPCSTR arg4, int16_t arg5)

L_1b93:                             ; ship.c:771
LEA       ax, [bp-rcGauge]          ; ax, [bp-0x1a]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-l]                ; ax, [bp-0x20]
MOV       dx, [bp-0x1e]       
MOV       cx, [bp-xLeft]            ; cx, [bp-0x1c]
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp-yTop]                 ; [bp-0xa]
PUSH      [bp-xRight]               ; [bp-0x12]
MOV       ax, [bp-yTop]             ; ax, [bp-0xa]
ADD       ax, [dyArial8]            ; ax, [0x23fa]
PUSH      ax                  
CALLF     SetRect                   ; void SetRect(RECT *arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)
                                    ; ship.c:772
LEA       si, [bp-rcGauge]          ; si, [bp-0x1a]
MOV       di, 0x48c0          
PUSH      ds                  
POP       es                  
MOVSW     [rgrcRef+0x10], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x12], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x14], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x16], ds:[si]   ; es:[di], ds:[si]
MOV       ax, 0x48c0          
                                    ; ship.c:773
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, [bp-pfl]              ; ax, [bp-0x10]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-rcGauge]          ; ax, [bp-0x1a]
PUSH      ax                  
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     DrawFleetGauge            ; void DrawFleetGauge(HDC hdc, RECT *prc, FLEET *lpfl, int16_t grbit)
ADD       sp, 0x000a          
                                    ; ship.c:774
MOV       ax, [gd+0x2]              ; ax, [0x07cc]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1bfc              

L_1bf6:
MOV       ax, 0x0002          
JMP       L_1bff              

L_1bfc:
MOV       ax, 0x0004          

L_1bff:
ADD       ax, [dyArial8]            ; ax, [0x23fa]
ADD       [bp-yTop], ax             ; [bp-0xa], ax
                                    ; ship.c:776
MOV       bx, [bp+ptile]            ; bx, [bp+0x8]
MOV       ax, [bx+0xa]        
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1c47              

L_1c1c:                             ; ship.c:778
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x02e9          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:779
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [bp-xLeft]                ; [bp-0x1c]
PUSH      [bp-yTop]                 ; [bp-0xa]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     TextOut                   ; int16_t TextOut(HDC arg1, int16_t arg2, int16_t arg3, LPCSTR arg4, int16_t arg5)

L_1c47:                             ; ship.c:781
LEA       ax, [bp-rcGauge]          ; ax, [bp-0x1a]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-l]                ; ax, [bp-0x20]
MOV       dx, [bp-0x1e]       
MOV       cx, [bp-xLeft]            ; cx, [bp-0x1c]
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp-yTop]                 ; [bp-0xa]
PUSH      [bp-xRight]               ; [bp-0x12]
MOV       ax, [bp-yTop]             ; ax, [bp-0xa]
ADD       ax, [dyArial8]            ; ax, [0x23fa]
PUSH      ax                  
CALLF     SetRect                   ; void SetRect(RECT *arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)
                                    ; ship.c:782
LEA       si, [bp-rcGauge]          ; si, [bp-0x1a]
MOV       di, 0x48c8          
PUSH      ds                  
POP       es                  
MOVSW     [rgrcRef+0x18], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x1a], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x1c], ds:[si]   ; es:[di], ds:[si]
MOVSW     [rgrcRef+0x1e], ds:[si]   ; es:[di], ds:[si]
MOV       ax, 0x48c8          
                                    ; ship.c:783
MOV       ax, 0x0005          
PUSH      ax                  
MOV       ax, [bp-pfl]              ; ax, [bp-0x10]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-rcGauge]          ; ax, [bp-0x1a]
PUSH      ax                  
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     DrawFleetGauge            ; void DrawFleetGauge(HDC hdc, RECT *prc, FLEET *lpfl, int16_t grbit)
ADD       sp, 0x000a          
                                    ; ship.c:784
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0004          
ADD       [bp-yTop], ax             ; [bp-0xa], ax
                                    ; ship.c:786
MOV       ax, [gd+0x2]              ; ax, [0x07cc]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1e6c              

L_1cb6:                             ; ship.c:789
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_1d9e              

L_1cbe:                             ; ship.c:791
MOV       bx, [bp+ptile]            ; bx, [bp+0x8]
MOV       ax, [bx+0xa]        
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1d25              

L_1cd4:                             ; ship.c:793
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; ship.c:794
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
PUSH      [bx+0x44a]          
PUSH      [bx+0x448]          
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; ship.c:795
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [bp-xLeft]                ; [bp-0x1c]
PUSH      [bp-yTop]                 ; [bp-0xa]
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
MOV       ax, [bx+0x4cc]      
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
MOV       ax, [bx+0x4cc]      
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     lstrlen                   ; int16_t lstrlen(LPCSTR arg1)
PUSH      ax                  
CALLF     TextOut                   ; int16_t TextOut(HDC arg1, int16_t arg2, int16_t arg3, LPCSTR arg4, int16_t arg5)

L_1d25:                             ; ship.c:798
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8]           ; [0x26b4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; ship.c:799
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; ship.c:800
MOV       ax, 0x004c          
MOV       bx, [bp-pfl]              ; bx, [bp-0x10]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
PUSH      [bx+0x2]            
PUSH      [bx]                
MOV       ax, 0x037c          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000c          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:801
PUSH      [bp-dxRight]              ; [bp-0x4]
PUSH      [bp-c]                    ; [bp-0xe]
MOV       ax, 0x57a4          
PUSH      ax                  
PUSH      [bp-yTop]                 ; [bp-0xa]
PUSH      [bp-xRight]               ; [bp-0x12]
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     RightTextOut              ; void RightTextOut(HDC hdc, int16_t x, int16_t y, char *psz, int16_t cLen, int16_t dxErase)
ADD       sp, 0x000c          
                                    ; ship.c:802
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       [bp-yTop], ax             ; [bp-0xa], ax
                                    ; ship.c:803
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_1d9e:
CMP       [bp-i], 0x0002            ; [bp-0xc], 0x0002
JLE       L_1cbe              

L_1da7:                             ; ship.c:805
MOV       bx, [bp+ptile]            ; bx, [bp+0x8]
MOV       ax, [bx+0xa]        
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1e20              

L_1dbd:                             ; ship.c:807
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; ship.c:808
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, 0xffff          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; ship.c:809
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x01b1          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:810
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [bp-xLeft]                ; [bp-0x1c]
PUSH      [bp-yTop]                 ; [bp-0xa]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0xe]
CALLF     TextOut                   ; int16_t TextOut(HDC arg1, int16_t arg2, int16_t arg3, LPCSTR arg4, int16_t arg5)
                                    ; ship.c:811
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8]           ; [0x26b4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; ship.c:812
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)

L_1e20:                             ; ship.c:814
MOV       bx, [bp-pfl]              ; bx, [bp-0x10]
PUSH      [bx+0x5a]           
PUSH      [bx+0x58]           
MOV       ax, 0x037c          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000c          
MOV       [bp-c], ax                ; [bp-0xe], ax
                                    ; ship.c:815
PUSH      [bp-dxRight]              ; [bp-0x4]
PUSH      [bp-c]                    ; [bp-0xe]
MOV       ax, 0x57a4          
PUSH      ax                  
PUSH      [bp-yTop]                 ; [bp-0xa]
PUSH      [bp-xRight]               ; [bp-0x12]
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     RightTextOut              ; void RightTextOut(HDC hdc, int16_t x, int16_t y, char *psz, int16_t cLen, int16_t dxErase)
ADD       sp, 0x000c          
                                    ; ship.c:816
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       [bp-yTop], ax             ; [bp-0xa], ax

L_1e6c:                             ; ship.c:817
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


