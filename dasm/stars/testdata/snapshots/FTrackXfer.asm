; FTrackXfer  (ship)
;   addr: 000b:5a16  len=1410
;   sig:  int16_t FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb)
;   params:
;     HWND             hwnd           [BP+0x6]
;     int16_t          x              [BP+0x8]
;     int16_t          y              [BP+0xa]
;     int16_t          fkb            [BP+0xc]
;   locals:
;     RECT             rc             [BP-0x4a]
;     int32_t          cNew           [BP-0x42]
;     BTN              btn            [BP-0x3e]
;     int16_t          iVal           [BP-0x30]
;     int16_t          iBtn           [BP-0x2e]
;     int16_t          i              [BP-0x2c]
;     int32_t          cCur           [BP-0x2a]
;     BTNT             btnt           [BP-0x26]
;     int32_t          dChg           [BP-0xe]
;     POINT            pt             [BP-0xa]
;     POINT            ptOld          [BP-0x6]
;
;   stats: blocks=0  labels=1
;     FinishUp: L_5f70

L_5a16:                             ; ship.c:2211
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x004c          
PUSH      si                  
PUSH      di                  
                                    ; ship.c:2224
PUSH      [bp+hwnd]                 ; [bp+0x6]
LEA       ax, [bp-rc]               ; ax, [bp-0x4a]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; ship.c:2225
MOV       ax, [bp+x]                ; ax, [bp+0x8]
MOV       [bp-pt], ax               ; [bp-0xa], ax
                                    ; ship.c:2226
MOV       ax, [bp+y]                ; ax, [bp+0xa]
MOV       [bp-pt+0x2], ax           ; [bp-0x8], ax
                                    ; ship.c:2228
MOV       [bp-i], 0x0000            ; [bp-0x2c], 0x0000
JMP       L_5a86              

L_5a42:                             ; ship.c:2230
MOV       ax, 0x000e          
IMUL      [bp-i]                    ; [bp-0x2c]
MOV       bx, [rgbtnXfer]           ; bx, [0x2540]
ADD       bx, ax              
MOV       ax, [bx+0x8]        
AND       ax, 0x0004          
CMP       ax, 0x0000          
JNZ       L_5a82              

L_5a5c:
MOV       ax, 0x000e          
IMUL      [bp-i]                    ; [bp-0x2c]
MOV       cx, [rgbtnXfer]           ; cx, [0x2540]
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
PUSH      [bp-pt+0x2]               ; [bp-0x8]
PUSH      [bp-pt]                   ; [bp-0xa]
CALLF     PtInRect                  ; int16_t PtInRect(RECT *arg1, POINT arg2)
CMP       ax, 0x0000          
JNZ       L_5a91              

L_5a7c:
JMP       L_5a82              

L_5a7f:                             ; ship.c:2231
JMP       L_5a91              

L_5a82:                             ; ship.c:2232
ADD       [bp-i], 0x0001            ; [bp-0x2c], 0x0001

L_5a86:
MOV       ax, [crgbtnXfer]          ; ax, [0x4b96]
CMP       [bp-i], ax                ; [bp-0x2c], ax
JL        L_5a42              

L_5a91:                             ; ship.c:2233
MOV       ax, [crgbtnXfer]          ; ax, [0x4b96]
CMP       [bp-i], ax                ; [bp-0x2c], ax
JNZ       L_5aa2              

L_5a9c:                             ; ship.c:2234
MOV       ax, 0x0000          
JMP       L_5f92              

L_5aa2:                             ; ship.c:2235
MOV       ax, [bp-i]                ; ax, [bp-0x2c]
SAR       ax, 0x0001          
MOV       [bp-iBtn], ax             ; [bp-0x2e], ax
                                    ; ship.c:2237
MOV       ax, 0x000e          
IMUL      [bp-i]                    ; [bp-0x2c]
MOV       si, [rgbtnXfer]           ; si, [0x2540]
ADD       si, ax              
PUSH      si                  
LEA       di, [bp-btn]              ; di, [bp-0x3e]
PUSH      ss                  
POP       es                  
MOV       cx, 0x0007          
MOVSW.REP es:[di], ds:[si]    
POP       si                  
LEA       ax, [bp-0x3e]       
                                    ; ship.c:2238
MOV       ax, [bp-btn+0xa]          ; ax, [bp-0x34]
AND       ax, 0x007f          
MOV       [bp-iVal], ax             ; [bp-0x30], ax
                                    ; ship.c:2240
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_5d2e              

L_5adc:                             ; ship.c:2242
CMP       [bp-iVal], 0x0004         ; [bp-0x30], 0x0004
JG        FinishUp            

L_5ae2:
JMP       L_5ae8              

L_5ae5:                             ; ship.c:2243
JMP       FinishUp            

L_5ae8:                             ; ship.c:2245
MOV       bx, [pxfer]               ; bx, [0x4a68]
CMP       [bx+0x82], 0x0008   
JNZ       L_5b0e              

L_5af6:                             ; ship.c:2247
CMP       [bp-iVal], 0x0004         ; [bp-0x30], 0x0004
JZ        FinishUp            

L_5aff:
CMP       [bp-iVal], 0x0003         ; [bp-0x30], 0x0003
JZ        FinishUp            

L_5b05:
JMP       L_5b31              

L_5b08:                             ; ship.c:2248
JMP       FinishUp            

L_5b0b:                             ; ship.c:2250
JMP       L_5b31              

L_5b0e:                             ; ship.c:2253
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       [bx+0x6], ax        
JNZ       FinishUp            

L_5b2b:
JMP       L_5b31              

L_5b2e:                             ; ship.c:2254
JMP       FinishUp            

L_5b31:                             ; ship.c:2257
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     SetCapture                ; HWND SetCapture(HWND arg1)
                                    ; ship.c:2258
MOV       [bp-ptOld+0x2], 0xffff    ; [bp-0x4], 0xffff
MOV       ax, 0xffff          
MOV       [bp-ptOld], ax            ; [bp-0x6], ax

L_5b44:                             ; ship.c:2259
LEA       ax, [bp-pt]               ; ax, [bp-0xa]
PUSH      ax                  
CALLF     FGetMouseMove             ; int16_t FGetMouseMove(POINT *ppt)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_5d26              

L_5b58:                             ; ship.c:2261
MOV       ax, [bp-ptOld]            ; ax, [bp-0x6]
CMP       [bp-pt], ax               ; [bp-0xa], ax
JNZ       L_5b6e              

L_5b63:
MOV       ax, [bp-ptOld+0x2]        ; ax, [bp-0x4]
CMP       [bp-pt+0x2], ax           ; [bp-0x8], ax
JZ        L_5b44              

L_5b6e:                             ; ship.c:2263
MOV       ax, [bp-pt]               ; ax, [bp-0xa]
MOV       dx, [bp-pt+0x2]           ; dx, [bp-0x8]
MOV       [bp-ptOld], ax            ; [bp-0x6], ax
MOV       [bp-ptOld+0x2], dx        ; [bp-0x4], dx
                                    ; ship.c:2265
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0001          
JNZ       L_5bc0              

L_5b8c:
MOV       bx, [pxfer]               ; bx, [0x4a68]
CMP       [bx+0x82], 0x0008   
JNZ       L_5bc0              

L_5b9a:                             ; ship.c:2266
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       bx, [pxfer]               ; bx, [0x4a68]
MOV       ax, [bx+0x92]       
AND       ax, 0x3fff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-cNew], ax             ; [bp-0x42], ax
MOV       [bp-cNew+0x2], dx         ; [bp-0x40], dx
                                    ; ship.c:2267
JMP       L_5c26              

L_5bc0:                             ; ship.c:2270
CMP       [bp-iVal], 0x0004         ; [bp-0x30], 0x0004
JNZ       L_5bf6              

L_5bc9:
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       cx, [pxfer]               ; cx, [0x4a68]
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
CALLF     LGetFleetStat             ; int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat)
ADD       sp, 0x0006          
JMP       L_5c20              

L_5bf6:
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       cx, [pxfer]               ; cx, [0x4a68]
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
CALLF     LGetFleetStat             ; int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat)
ADD       sp, 0x0006          

L_5c20:
MOV       [bp-cNew], ax             ; [bp-0x42], ax
MOV       [bp-cNew+0x2], dx         ; [bp-0x40], dx

L_5c26:                             ; ship.c:2272
MOV       ax, [bp-btn+0x4]          ; ax, [bp-0x3a]
SUB       ax, [bp-btn]              ; ax, [bp-0x3e]
ADD       ax, 0xfffe          
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cNew+0x2]             ; [bp-0x40]
PUSH      [bp-cNew]                 ; [bp-0x42]
MOV       ax, [bp-pt]               ; ax, [bp-0xa]
SUB       ax, [bp-btn]              ; ax, [bp-0x3e]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cNew], ax             ; [bp-0x42], ax
MOV       [bp-cNew+0x2], dx         ; [bp-0x40], dx
                                    ; ship.c:2275
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       cx, [pxfer]               ; cx, [0x4a68]
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
PUSH      cx                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iVal]                 ; [bp-0x30]
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
PUSH      [bx]                
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
PUSH      [bx+0x2]            
CALLF     ChgCargo                  ; int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj)
ADD       sp, 0x000c          
MOV       [bp-cCur], ax             ; [bp-0x2a], ax
MOV       [bp-cCur+0x2], dx         ; [bp-0x28], dx
                                    ; ship.c:2276
MOV       ax, [bp-cNew]             ; ax, [bp-0x42]
MOV       dx, [bp-cNew+0x2]         ; dx, [bp-0x40]
SUB       ax, [bp-cCur]             ; ax, [bp-0x2a]
SBB       dx, [bp-cCur+0x2]         ; dx, [bp-0x28]
MOV       [bp-dChg], ax             ; [bp-0xe], ax
MOV       [bp-dChg+0x2], dx         ; [bp-0xc], dx
                                    ; ship.c:2277
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0000          
JNZ       L_5ce3              

L_5cda:
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-dChg+0x2]         ; dx, [bp-0xc]
JMP       L_5cf0              

L_5ce3:
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-dChg+0x2]         ; dx, [bp-0xc]
NEG       ax                  
ADC       dx, 0x0000          
NEG       dx                  

L_5cf0:
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iVal]                 ; [bp-0x30]
CALLF     XferSupply                ; int32_t XferSupply(int16_t iSupply, int32_t cQuan)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JNZ       L_5d0d              

L_5d05:
CMP       dx, 0x0000          
JZ        L_5b44              

L_5d0d:                             ; ship.c:2278
PUSH      [bp-iVal]                 ; [bp-0x30]
LEA       ax, [bp-rc]               ; ax, [bp-0x4a]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     DrawXferDlg               ; void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply)
ADD       sp, 0x0008          

L_5d23:                             ; ship.c:2280
JMP       L_5b44              

L_5d26:                             ; ship.c:2281
CALLF     ReleaseCapture            ; void ReleaseCapture()
                                    ; ship.c:2282
JMP       FinishUp            

L_5d2e:                             ; ship.c:2284
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0050          
PUSH      ax                  
PUSH      [bp-btn+0x8]              ; [bp-0x36]
LEA       ax, [bp-btn]              ; ax, [bp-0x3e]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0x6]
LEA       ax, [bp-btnt]             ; ax, [bp-0x26]
PUSH      ax                  
CALLF     InitBtnTrack              ; void InitBtnTrack(BTNT *pbtnt, HWND hwnd, HDC hdc, RECT *prc, int16_t btf, int16_t dTimer, int16_t fInitDown, int16_t fNoEndRedraw, char *szText)
ADD       sp, 0x0012          
                                    ; ship.c:2285
MOV       ax, [bp+fkb]              ; ax, [bp+0xc]
AND       ax, 0x0008          
CMP       ax, 0x0000          
JZ        L_5d88              

L_5d66:                             ; ship.c:2286
MOV       ax, [bp+fkb]              ; ax, [bp+0xc]
AND       ax, 0x0004          
CMP       ax, 0x0000          
JZ        L_5d7b              

L_5d74:
MOV       ax, 0x03e8          
CWD       dx, ax              
JMP       L_5d7f              

L_5d7b:
MOV       ax, 0x0064          
CWD       dx, ax              

L_5d7f:
MOV       [bp-dChg], ax             ; [bp-0xe], ax
MOV       [bp-dChg+0x2], dx         ; [bp-0xc], dx
                                    ; ship.c:2287
JMP       L_5dad              

L_5d88:
MOV       ax, [bp+fkb]              ; ax, [bp+0xc]
AND       ax, 0x0004          
CMP       ax, 0x0000          
JZ        L_5da3              

L_5d96:                             ; ship.c:2288
MOV       [bp-dChg], 0x000a         ; [bp-0xe], 0x000a
MOV       [bp-dChg+0x2], 0x0000     ; [bp-0xc], 0x0000
                                    ; ship.c:2289
JMP       L_5dad              

L_5da3:                             ; ship.c:2290
MOV       [bp-dChg], 0x0001         ; [bp-0xe], 0x0001
MOV       [bp-dChg+0x2], 0x0000     ; [bp-0xc], 0x0000

L_5dad:                             ; ship.c:2292
LEA       ax, [bp-btnt]             ; ax, [bp-0x26]
PUSH      ax                  
CALLF     FTrackBtn                 ; int16_t FTrackBtn(BTNT *pbtnt)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        FinishUp            

L_5dc1:                             ; ship.c:2294
CMP       [mdXferDlg], 0x0001       ; [0x0984], 0x0001
JNZ       L_5f01              

L_5dcb:                             ; ship.c:2296
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
MOV       [bp-0x4c], ax       
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0000          
JNZ       L_5dec              

L_5de6:
MOV       ax, 0x0001          
JMP       L_5def              

L_5dec:
MOV       ax, 0x0000          

L_5def:
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
MOV       ax, 0x0010          
ADD       bx, ax              
MOV       ax, [bp-iVal]             ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bp-0x4c]       
CMP       ax, [bx]            
JGE       L_5e19              

L_5e10:
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
JMP       L_5e4d              

L_5e19:
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0000          
JNZ       L_5e31              

L_5e2b:
MOV       ax, 0x0001          
JMP       L_5e34              

L_5e31:
MOV       ax, 0x0000          

L_5e34:
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
MOV       ax, 0x0010          
ADD       bx, ax              
MOV       ax, [bp-iVal]             ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            

L_5e4d:
MOV       [bp-i], ax                ; [bp-0x2c], ax
                                    ; ship.c:2297
CMP       [bp-i], 0x0000            ; [bp-0x2c], 0x0000
JZ        L_5dad              

L_5e59:                             ; ship.c:2299
MOV       ax, 0x7ffe          
SUB       ax, [bp-i]                ; ax, [bp-0x2c]
MOV       dx, [bp-btn+0xc]          ; dx, [bp-0x32]
SHR       dx, 0x0001          
SHR       dx, 0x0001          
AND       dx, 0x0003          
MOV       cx, 0x0007          
SHL       dx, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, dx              
MOV       cx, 0x0010          
ADD       bx, cx              
MOV       dx, [bp-iVal]             ; dx, [bp-0x30]
SHL       dx, 0x0001          
ADD       bx, dx              
CMP       [bx], ax            
JL        L_5e8c              

L_5e87:                             ; ship.c:2300
MOV       [bp-i], 0x0001            ; [bp-0x2c], 0x0001

L_5e8c:                             ; ship.c:2301
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
MOV       ax, 0x0010          
ADD       bx, ax              
MOV       ax, [bp-iVal]             ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x2c]
ADD       [bx], ax            
                                    ; ship.c:2302
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0000          
JNZ       L_5eca              

L_5ec4:
MOV       ax, 0x0001          
JMP       L_5ecd              

L_5eca:
MOV       ax, 0x0000          

L_5ecd:
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       bx, [pxfer]               ; bx, [0x4a68]
ADD       bx, ax              
MOV       ax, 0x0010          
ADD       bx, ax              
MOV       ax, [bp-iVal]             ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x2c]
SUB       [bx], ax            
                                    ; ship.c:2303
PUSH      [bp-iBtn]                 ; [bp-0x2e]
LEA       ax, [bp-rc]               ; ax, [bp-0x4a]
PUSH      ax                  
PUSH      [bp-btnt+0x2]             ; [bp-0x24]
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     DrawXferDlg               ; void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply)
ADD       sp, 0x0008          

L_5efe:                             ; ship.c:2306
JMP       L_5dad              

L_5f01:
CMP       [bp-iVal], 0x0000         ; [bp-0x30], 0x0000
JL        L_5dad              

L_5f0a:
CMP       [bp-iVal], 0x0004         ; [bp-0x30], 0x0004
JG        L_5dad              

L_5f13:                             ; ship.c:2308
MOV       ax, [bp-btn+0xc]          ; ax, [bp-0x32]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0003          
CMP       ax, 0x0000          
JNZ       L_5f2e              

L_5f25:
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-dChg+0x2]         ; dx, [bp-0xc]
JMP       L_5f3b              

L_5f2e:
MOV       ax, [bp-dChg]             ; ax, [bp-0xe]
MOV       dx, [bp-dChg+0x2]         ; dx, [bp-0xc]
NEG       ax                  
ADC       dx, 0x0000          
NEG       dx                  

L_5f3b:
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iVal]                 ; [bp-0x30]
CALLF     XferSupply                ; int32_t XferSupply(int16_t iSupply, int32_t cQuan)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JNZ       L_5f58              

L_5f50:
CMP       dx, 0x0000          
JZ        L_5dad              

L_5f58:                             ; ship.c:2309
PUSH      [bp-iVal]                 ; [bp-0x30]
LEA       ax, [bp-rc]               ; ax, [bp-0x4a]
PUSH      ax                  
PUSH      [bp-btnt+0x2]             ; [bp-0x24]
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     DrawXferDlg               ; void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply)
ADD       sp, 0x0008          

L_5f6d:                             ; ship.c:2311
JMP       L_5dad              

FinishUp:                           ; ship.c:2315
CALLF     UpdateXferBtns            ; void UpdateXferBtns()
                                    ; ship.c:2316
MOV       ax, 0xfffe          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x4a]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0x6]
CALLF     DrawXferDlg               ; void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply)
ADD       sp, 0x0008          
                                    ; ship.c:2317
MOV       ax, 0x0001          

L_5f92:                             ; ship.c:2318
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


