; DrawMassWarpGauge  (planet)
;   addr: 000a:2afa  len=317
;   sig:  void DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur)
;   params:
;     HDC              hdc            [BP+0x6]
;     RECT *           prc            [BP+0x8]
;     int16_t          iBest          [BP+0xa]
;     int16_t          iCur           [BP+0xc]
;   locals:
;     int32_t          l              [BP-0x16]
;     int32_t          lCur           [BP-0x12]
;     HBRUSH           hbr            [BP-0xe]
;     int16_t          iMode          [BP-0xc]
;     int16_t          fTwoMAs        [BP-0xa]
;     int16_t          c              [BP-0x8]
;     int32_t          lMax           [BP-0x6]
;
;   stats: blocks=0  labels=0

L_2afa:                             ; planet.c:1064
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0016          
PUSH      si                  
PUSH      di                  
                                    ; planet.c:1071
CMP       [bp+iBest], 0x0000        ; [bp+0xa], 0x0000
JGE       L_2b12              

L_2b0c:
MOV       ax, 0x0001          
JMP       L_2b15              

L_2b12:
MOV       ax, 0x0000          

L_2b15:
MOV       [bp-fTwoMAs], ax          ; [bp-0xa], ax
                                    ; planet.c:1075
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; planet.c:1077
CMP       [bp+iCur], 0x0005         ; [bp+0xc], 0x0005
JGE       L_2b32              

L_2b2d:                             ; planet.c:1078
MOV       [bp+iCur], 0x0005         ; [bp+0xc], 0x0005

L_2b32:                             ; planet.c:1080
CMP       [bp+iBest], 0x0000        ; [bp+0xa], 0x0000
JGE       L_2b43              

L_2b3b:                             ; planet.c:1081
MOV       ax, [bp+iBest]            ; ax, [bp+0xa]
NEG       ax                  
MOV       [bp+iBest], ax            ; [bp+0xa], ax

L_2b43:                             ; planet.c:1083
MOV       ax, [bp+iBest]            ; ax, [bp+0xa]
ADD       ax, 0xffff          
CWD       dx, ax              
MOV       [bp-lMax], ax             ; [bp-0x6], ax
MOV       [bp-lMax+0x2], dx         ; [bp-0x4], dx
                                    ; planet.c:1085
MOV       ax, [bp+iBest]            ; ax, [bp+0xa]
ADD       ax, [bp-fTwoMAs]          ; ax, [bp-0xa]
CMP       [bp+iCur], ax             ; [bp+0xc], ax
JG        L_2b67              

L_2b5e:                             ; planet.c:1086
MOV       ax, [hbrPurple]           ; ax, [0x2692]
MOV       [bp-hbr], ax              ; [bp-0xe], ax
                                    ; planet.c:1087
JMP       L_2b87              

L_2b67:
MOV       ax, [bp+iBest]            ; ax, [bp+0xa]
ADD       ax, [bp-fTwoMAs]          ; ax, [bp-0xa]
ADD       ax, 0x0003          
CMP       [bp+iCur], ax             ; [bp+0xc], ax
JGE       L_2b81              

L_2b78:                             ; planet.c:1088
MOV       ax, [hbrYellow]           ; ax, [0x22c6]
MOV       [bp-hbr], ax              ; [bp-0xe], ax
                                    ; planet.c:1089
JMP       L_2b87              

L_2b81:                             ; planet.c:1090
MOV       ax, [hbrRed]              ; ax, [0x0024]
MOV       [bp-hbr], ax              ; [bp-0xe], ax

L_2b87:                             ; planet.c:1092
MOV       ax, [bp+iCur]             ; ax, [bp+0xc]
ADD       ax, 0xfffc          
CWD       dx, ax              
MOV       [bp-lCur], ax             ; [bp-0x12], ax
MOV       [bp-lCur+0x2], dx         ; [bp-0x10], dx
                                    ; planet.c:1094
PUSH      [bp-lMax+0x2]             ; [bp-0x4]
PUSH      [bp-lMax]                 ; [bp-0x6]
LEA       ax, [bp-hbr]              ; ax, [bp-0xe]
PUSH      ax                  
LEA       ax, [bp-lCur]             ; ax, [bp-0x12]
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp+prc]                  ; [bp+0x8]
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     LDrawGauge                ; int32_t LDrawGauge(HDC hdc, RECT *prc, int16_t cSegs, int32_t *rgSize, HBRUSH *rghbr, int32_t cTot)
ADD       sp, 0x000e          
MOV       [bp-l], ax                ; [bp-0x16], ax
MOV       [bp-l+0x2], dx            ; [bp-0x14], dx
                                    ; planet.c:1095
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)
MOV       [bp-iMode], ax            ; [bp-0xc], ax
                                    ; planet.c:1097
MOV       ax, [bp-l]                ; ax, [bp-0x16]
MOV       dx, [bp-l+0x2]            ; dx, [bp-0x14]
ADD       ax, 0x0004          
ADC       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0369          
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
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; planet.c:1098
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x8]
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-l], ax                ; [bp-0x16], ax
MOV       [bp-l+0x2], dx            ; [bp-0x14], dx
                                    ; planet.c:1099
PUSH      [bp-c]                    ; [bp-0x8]
MOV       ax, 0x57a4          
PUSH      ax                  
PUSH      [bp+prc]                  ; [bp+0x8]
PUSH      [bp+hdc]                  ; [bp+0x6]
CALLF     RcCtrTextOut              ; void RcCtrTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen)
ADD       sp, 0x0008          
                                    ; planet.c:1100
PUSH      [bp+hdc]                  ; [bp+0x6]
PUSH      [bp-iMode]                ; [bp-0xc]
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)
                                    ; planet.c:1101
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


