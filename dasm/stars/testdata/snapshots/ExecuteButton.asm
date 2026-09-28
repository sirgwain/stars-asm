; ExecuteButton  (tb)
;   addr: 000e:0db6  len=2250
;   sig:  void ExecuteButton(int16_t itb, int16_t fDown)
;   params:
;     int16_t          itb            [BP+0x6]
;     int16_t          fDown          [BP+0x8]
;   locals:
;     uint16_t         grbitNew       [BP-0x4]
;     block 000E:0E4E  len=0x228
;       int16_t          iSel           [BP-0x58]
;       int32_t[12]      rgid           [BP-0x56]
;       uint16_t         grbit          [BP-0x26]
;       int16_t          i              [BP-0x24]
;       int16_t          c              [BP-0x22]
;       char *[12]       rgszScan       [BP-0x20]
;       POINT            pt             [BP-0x8]
;     block 000E:1076  len=0x284
;       int16_t          iSel           [BP-0x8a]
;       int32_t[20]      rgid           [BP-0x88]
;       int16_t          c              [BP-0x38]
;       int16_t          i              [BP-0x36]
;       int16_t          ish            [BP-0x34]
;       char *[20]       rgszScan       [BP-0x32]
;       POINT            pt             [BP-0xa]
;       uint16_t         grbitSh        [BP-0x6]
;     block 000E:12FA  len=0x21F
;       int16_t          iSel           [BP-0x58]
;       int32_t[12]      rgid           [BP-0x56]
;       uint16_t         grbit          [BP-0x26]
;       int16_t          i              [BP-0x24]
;       int16_t          c              [BP-0x22]
;       char *[12]       rgszScan       [BP-0x20]
;       POINT            pt             [BP-0x8]
;     block 000E:1519  len=0xF3
;       int16_t          iSel           [BP-0x44]
;       int32_t[9]       rgid           [BP-0x42]
;       int16_t          i              [BP-0x1e]
;       int16_t          c              [BP-0x1c]
;       char *[9]        rgszScan       [BP-0x1a]
;       POINT            pt             [BP-0x8]
;
;   stats: blocks=4  labels=3
;     LInvalS: L_12cb
;     LBitDiddle: L_ df7
;     LInvalE: L_14ea

L_0db6:                             ; tb.c:484
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x008a          
PUSH      si                  
PUSH      di                  
                                    ; tb.c:487
MOV       ax, [gd+0x4]              ; ax, [0x07ce]
AND       ax, 0xffbf          
OR        ax, 0x0040          
MOV       [gd+0x4], ax              ; [0x07ce], ax
                                    ; tb.c:489
MOV       ax, [bp+itb]              ; ax, [bp+0x6]
JMP       L_160f              

L_0dd4:                             ; tb.c:502
CMP       [bp+fDown], 0x0000        ; [bp+0x8], 0x0000
JZ        L_167a              

L_0de0:                             ; tb.c:504
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x3ff0          
MOV       cx, [bp+itb]              ; cx, [bp+0x6]
ADD       cx, ax              
MOV       [grbitScan], cx           ; [0x0588], cx
                                    ; tb.c:505
JMP       L_1644              

L_0df2:                             ; tb.c:508
MOV       [bp-grbitNew], 0x0010     ; [bp-0x4], 0x0010

LBitDiddle:                         ; tb.c:510
CMP       [bp+fDown], 0x0000        ; [bp+0x8], 0x0000
JZ        L_0e0a              

L_0e00:                             ; tb.c:511
MOV       ax, [bp-grbitNew]         ; ax, [bp-0x4]
OR        [grbitScan], ax           ; [0x0588], ax
                                    ; tb.c:512
JMP       L_1644              

L_0e0a:                             ; tb.c:513
MOV       ax, [bp-grbitNew]         ; ax, [bp-0x4]
NOT       ax                  
AND       [grbitScan], ax           ; [0x0588], ax

L_0e13:                             ; tb.c:514
JMP       L_1644              

L_0e16:                             ; tb.c:517
MOV       [bp-grbitNew], 0x0020     ; [bp-0x4], 0x0020
                                    ; tb.c:518
JMP       LBitDiddle          

L_0e1e:                             ; tb.c:521
MOV       [bp-grbitNew], 0x0080     ; [bp-0x4], 0x0080
                                    ; tb.c:522
JMP       LBitDiddle          

L_0e26:                             ; tb.c:525
MOV       [bp-grbitNew], 0x0100     ; [bp-0x4], 0x0100
                                    ; tb.c:526
JMP       LBitDiddle          

L_0e2e:                             ; tb.c:529
MOV       [bp-grbitNew], 0x0400     ; [bp-0x4], 0x0400
                                    ; tb.c:530
JMP       LBitDiddle          

L_0e36:                             ; tb.c:533
MOV       [bp-grbitNew], 0x1000     ; [bp-0x4], 0x1000
                                    ; tb.c:534
JMP       LBitDiddle          

L_0e3e:                             ; tb.c:537
MOV       [bp-grbitNew], 0x0200     ; [bp-0x4], 0x0200
                                    ; tb.c:538
JMP       LBitDiddle          

L_0e46:                             ; tb.c:541
MOV       [bp-grbitNew], 0x0800     ; [bp-0x4], 0x0800
                                    ; tb.c:542
JMP       LBitDiddle          

L_0e4e:                             ; tb.c:547
MOV       [bp-grbit], 0x0001        ; [bp-0x26], 0x0001
                                    ; tb.c:550
MOV       [bp-c], 0x0000            ; [bp-0x22], 0x0000
                                    ; tb.c:554
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0040          
CMP       ax, 0x0000          
JNZ       L_0e6c              

L_0e66:                             ; tb.c:555
MOV       [grbitScanMines], 0x0000  ; [0x4a5c], 0x0000

L_0e6c:                             ; tb.c:557
MOV       [bp-i], 0x04fe            ; [bp-0x24], 0x04fe
JMP       L_0f0d              

L_0e74:                             ; tb.c:559
CMP       [bp-i], 0x04fe            ; [bp-0x24], 0x04fe
JNZ       L_0ea7              

L_0e7e:                             ; tb.c:560
CMP       [grbitScanMines], 0x000f  ; [0x4a5c], 0x000f
JNZ       L_0e8f              

L_0e88:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_0e93              

L_0e8f:
MOV       ax, 0x0000          
CWD       dx, ax              

L_0e93:
MOV       bx, [bp-c]                ; bx, [bp-0x22]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x56]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
                                    ; tb.c:561
JMP       L_0ecd              

L_0ea7:                             ; tb.c:562
CMP       [grbitScanMines], 0x0000  ; [0x4a5c], 0x0000
JNZ       L_0eb8              

L_0eb1:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_0ebc              

L_0eb8:
MOV       ax, 0x0000          
CWD       dx, ax              

L_0ebc:
MOV       bx, [bp-c]                ; bx, [bp-0x22]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x56]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        

L_0ecd:                             ; tb.c:564
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0xfb02          
MOV       cx, 0x001e          
IMUL      cx                  
MOV       cx, 0x5844          
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp-i]                    ; [bp-0x24]
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; tb.c:565
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0xfb02          
MOV       cx, 0x001e          
IMUL      cx                  
MOV       cx, 0x5844          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:566
ADD       [bp-i], 0x0001            ; [bp-0x24], 0x0001

L_0f0d:
CMP       [bp-i], 0x04ff            ; [bp-0x24], 0x04ff
JLE       L_0e74              

L_0f17:                             ; tb.c:568
MOV       ax, [bp-c]                ; ax, [bp-0x22]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x56]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; tb.c:569
MOV       [szWork+0xfa], 0x00ff     ; [0x589e], 0x00ff
                                    ; tb.c:570
MOV       [szWork+0xfb], 0x0000     ; [0x589f], 0x0000
                                    ; tb.c:571
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], 0x589e        
                                    ; tb.c:573
MOV       [bp-i], 0x0000            ; [bp-0x24], 0x0000
JMP       L_0fba              

L_0f50:                             ; tb.c:575
MOV       cx, [bp-i]                ; cx, [bp-0x24]
MOV       ax, 0x0001          
SHL       ax, cx              
AND       ax, [grbitScanMines]      ; ax, [0x4a5c]
CMP       ax, 0x0000          
JZ        L_0f6b              

L_0f64:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_0f6f              

L_0f6b:
MOV       ax, 0x0000          
CWD       dx, ax              

L_0f6f:
MOV       bx, [bp-c]                ; bx, [bp-0x22]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x56]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
                                    ; tb.c:576
MOV       ax, 0x001e          
IMUL      [bp-i]                    ; [bp-0x24]
MOV       cx, 0x57a4          
ADD       cx, ax              
PUSH      cx                  
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0x0500          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; tb.c:577
MOV       ax, 0x001e          
IMUL      [bp-i]                    ; [bp-0x24]
MOV       cx, 0x57a4          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:578
ADD       [bp-i], 0x0001            ; [bp-0x24], 0x0001

L_0fba:
CMP       [bp-i], 0x0004            ; [bp-0x24], 0x0004
JL        L_0f50              

L_0fc3:                             ; tb.c:580
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCursorPos              ; void GetCursorPos(POINT *arg1)
                                    ; tb.c:581
PUSH      [hwndTb]                  ; [0x019a]
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScreenToClient            ; void ScreenToClient(HWND arg1, POINT *arg2)
                                    ; tb.c:582
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0xfffe          
PUSH      ax                  
LEA       ax, [bp-rgszScan]         ; ax, [bp-0x20]
PUSH      ax                  
LEA       ax, [bp-rgid]             ; ax, [bp-0x56]
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x22]
PUSH      [bp-pt+0x2]               ; [bp-0x6]
PUSH      [bp-pt]                   ; [bp-0x8]
PUSH      [hwndTb]                  ; [0x019a]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-iSel], ax             ; [bp-0x58], ax
                                    ; tb.c:584
CMP       [bp-iSel], 0xffff         ; [bp-0x58], 0xffff
JZ        L_167a              

L_1013:                             ; tb.c:587
CMP       [bp-iSel], 0x0003         ; [bp-0x58], 0x0003
JGE       L_1037              

L_101c:                             ; tb.c:589
CMP       [bp-iSel], 0x0000         ; [bp-0x58], 0x0000
JNZ       L_102e              

L_1025:                             ; tb.c:590
MOV       [grbitScanMines], 0x000f  ; [0x4a5c], 0x000f
                                    ; tb.c:591
JMP       L_1047              

L_102e:                             ; tb.c:592
MOV       [grbitScanMines], 0x0000  ; [0x4a5c], 0x0000

L_1034:                             ; tb.c:594
JMP       L_1047              

L_1037:                             ; tb.c:596
SUB       [bp-iSel], 0x0003         ; [bp-0x58], 0x0003
                                    ; tb.c:598
MOV       cx, [bp-iSel]             ; cx, [bp-0x58]
MOV       ax, 0x0001          
SHL       ax, cx              
XOR       [grbitScanMines], ax      ; [0x4a5c], ax

L_1047:                             ; tb.c:601
CMP       [grbitScanMines], 0x0000  ; [0x4a5c], 0x0000
JZ        L_1059              

L_1051:                             ; tb.c:602
OR        [grbitScan], 0x0040       ; [0x0588], 0x0040
                                    ; tb.c:603
JMP       L_105e              

L_1059:                             ; tb.c:604
AND       [grbitScan], 0xffbf       ; [0x0588], 0xffbf

L_105e:                             ; tb.c:606
PUSH      [hwndTb]                  ; [0x019a]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; tb.c:607
JMP       L_1644              

L_1076:                             ; tb.c:616
MOV       [bp-c], 0x0000            ; [bp-0x38], 0x0000
                                    ; tb.c:621
MOV       [bp-i], 0x04fb            ; [bp-0x36], 0x04fb
JMP       L_10d9              

L_1083:                             ; tb.c:623
MOV       ax, [bp-c]                ; ax, [bp-0x38]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x88]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; tb.c:624
MOV       ax, [bp-i]                ; ax, [bp-0x36]
ADD       ax, 0xfb05          
MOV       cx, 0x0014          
IMUL      cx                  
MOV       cx, 0x57a4          
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp-i]                    ; [bp-0x36]
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; tb.c:625
MOV       ax, [bp-i]                ; ax, [bp-0x36]
ADD       ax, 0xfb05          
MOV       cx, 0x0014          
IMUL      cx                  
MOV       cx, 0x57a4          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x38]
ADD       [bp-c], 0x0001            ; [bp-0x38], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x32]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:626
ADD       [bp-i], 0x0001            ; [bp-0x36], 0x0001

L_10d9:
CMP       [bp-i], 0x04fd            ; [bp-0x36], 0x04fd
JLE       L_1083              

L_10e3:                             ; tb.c:628
MOV       ax, [bp-c]                ; ax, [bp-0x38]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x88]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; tb.c:629
MOV       [szWork+0xc8], 0x00ff     ; [0x586c], 0x00ff
                                    ; tb.c:630
MOV       [szWork+0xc9], 0x0000     ; [0x586d], 0x0000
                                    ; tb.c:631
MOV       ax, [bp-c]                ; ax, [bp-0x38]
ADD       [bp-c], 0x0001            ; [bp-0x38], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x32]       
ADD       bx, ax              
MOV       [bx], 0x586c        
                                    ; tb.c:633
MOV       [bp-ish], 0x0000          ; [bp-0x34], 0x0000
MOV       [bp-grbitSh], 0x0001      ; [bp-0x6], 0x0001
MOV       ax, 0x0001          
JMP       L_1135              

L_1125:
MOV       ax, [bp-0x34]       
ADD       [bp-ish], 0x0001          ; [bp-0x34], 0x0001
MOV       cx, 0x0001          
SHL       [bp-grbitSh], cx          ; [bp-0x6], cx
MOV       ax, [bp-0x6]        

L_1135:
CMP       [bp-ish], 0x0010          ; [bp-0x34], 0x0010
JGE       L_11ab              

L_113e:                             ; tb.c:634
MOV       ax, 0x0093          
IMUL      [bp-ish]                  ; [bp-0x34]
MOV       bx, 0x3f00          
ADD       bx, ax              
MOV       ax, [bx+0x7b]       
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1125              

L_115c:                             ; tb.c:636
MOV       ax, [bp-grbitSh]          ; ax, [bp-0x6]
AND       ax, [grbitScanShip]       ; ax, [0x5308]
CMP       ax, 0x0000          
JZ        L_1172              

L_116b:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_1176              

L_1172:
MOV       ax, 0x0000          
CWD       dx, ax              

L_1176:
MOV       bx, [bp-c]                ; bx, [bp-0x38]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x88]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
                                    ; tb.c:637
MOV       ax, 0x0093          
IMUL      [bp-ish]                  ; [bp-0x34]
MOV       cx, 0x3f00          
ADD       cx, ax              
MOV       ax, 0x0008          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x38]
ADD       [bp-c], 0x0001            ; [bp-0x38], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x32]       
ADD       bx, ax              
MOV       [bx], cx            

L_11a8:                             ; tb.c:640
JMP       L_1125              

L_11ab:
LEA       ax, [bp-pt]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCursorPos              ; void GetCursorPos(POINT *arg1)
                                    ; tb.c:641
PUSH      [hwndTb]                  ; [0x019a]
LEA       ax, [bp-pt]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScreenToClient            ; void ScreenToClient(HWND arg1, POINT *arg2)
                                    ; tb.c:642
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0xfffe          
PUSH      ax                  
LEA       ax, [bp-rgszScan]         ; ax, [bp-0x32]
PUSH      ax                  
LEA       ax, [bp-rgid]             ; ax, [bp-0x88]
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x38]
PUSH      [bp-pt+0x2]               ; [bp-0x8]
PUSH      [bp-pt]                   ; [bp-0xa]
PUSH      [hwndTb]                  ; [0x019a]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-iSel], ax             ; [bp-0x8a], ax
                                    ; tb.c:644
CMP       [bp-iSel], 0xffff         ; [bp-0x8a], 0xffff
JZ        L_167a              

L_11fe:                             ; tb.c:647
CMP       [bp-iSel], 0x0004         ; [bp-0x8a], 0x0004
JGE       L_1251              

L_1208:                             ; tb.c:649
CMP       [bp-iSel], 0x0000         ; [bp-0x8a], 0x0000
JNZ       L_121b              

L_1212:                             ; tb.c:650
MOV       [grbitScanShip], 0xffff   ; [0x5308], 0xffff
                                    ; tb.c:651
JMP       L_1233              

L_121b:
CMP       [bp-iSel], 0x0001         ; [bp-0x8a], 0x0001
JNZ       L_122d              

L_1225:                             ; tb.c:652
XOR       [grbitScanShip], 0xffff   ; [0x5308], 0xffff
                                    ; tb.c:653
JMP       L_1233              

L_122d:                             ; tb.c:654
MOV       [grbitScanShip], 0x0000   ; [0x5308], 0x0000

L_1233:                             ; tb.c:656
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0200          
CMP       ax, 0x0000          
JNZ       L_12e6              

L_1241:
CMP       [grbitScanShip], 0x0000   ; [0x5308], 0x0000
JNZ       LInvalS             

L_1248:
JMP       L_12e6              

L_1251:                             ; tb.c:661
SUB       [bp-iSel], 0x0004         ; [bp-0x8a], 0x0004
                                    ; tb.c:663
MOV       [bp-ish], 0x0000          ; [bp-0x34], 0x0000
JMP       L_1294              

L_125e:                             ; tb.c:664
MOV       ax, 0x0093          
IMUL      [bp-ish]                  ; [bp-0x34]
MOV       bx, 0x3f00          
ADD       bx, ax              
MOV       ax, [bx+0x7b]       
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1290              

L_127c:
SUB       [bp-iSel], 0x0001         ; [bp-0x8a], 0x0001
MOV       ax, [bp-iSel]             ; ax, [bp-0x8a]
CMP       ax, 0x0000          
JL        L_129d              

L_1290:                             ; tb.c:667
ADD       [bp-ish], 0x0001          ; [bp-0x34], 0x0001

L_1294:
CMP       [bp-ish], 0x0010          ; [bp-0x34], 0x0010
JL        L_125e              

L_129d:                             ; tb.c:669
MOV       cx, [bp-ish]              ; cx, [bp-0x34]
MOV       ax, 0x0001          
SHL       ax, cx              
XOR       [grbitScanShip], ax       ; [0x5308], ax
                                    ; tb.c:671
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0200          
CMP       ax, 0x0000          
JNZ       L_12e6              

L_12b7:
MOV       cx, [bp-ish]              ; cx, [bp-0x34]
MOV       ax, 0x0001          
SHL       ax, cx              
AND       ax, [grbitScanShip]       ; ax, [0x5308]
CMP       ax, 0x0000          
JZ        L_12e6              

LInvalS:                            ; tb.c:674
OR        [grbitScan], 0x0200       ; [0x0588], 0x0200
                                    ; tb.c:675
PUSH      [hwndTb]                  ; [0x019a]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_12e6:                             ; tb.c:679
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0200          
CMP       ax, 0x0000          
JZ        L_167a              

L_12f1:
JMP       L_1644              

L_12fa:                             ; tb.c:688
MOV       [bp-grbit], 0x0001        ; [bp-0x26], 0x0001
                                    ; tb.c:691
MOV       [bp-c], 0x0000            ; [bp-0x22], 0x0000
                                    ; tb.c:695
MOV       [bp-i], 0x04fb            ; [bp-0x24], 0x04fb
JMP       L_1361              

L_130c:                             ; tb.c:697
MOV       ax, [bp-c]                ; ax, [bp-0x22]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x56]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; tb.c:698
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0xfb05          
MOV       cx, 0x0019          
IMUL      cx                  
MOV       cx, 0x586c          
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp-i]                    ; [bp-0x24]
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; tb.c:699
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0xfb05          
MOV       cx, 0x0019          
IMUL      cx                  
MOV       cx, 0x586c          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:700
ADD       [bp-i], 0x0001            ; [bp-0x24], 0x0001

L_1361:
CMP       [bp-i], 0x04fd            ; [bp-0x24], 0x04fd
JLE       L_130c              

L_136b:                             ; tb.c:702
MOV       ax, [bp-c]                ; ax, [bp-0x22]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x56]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; tb.c:703
MOV       [szWork+0x12c], 0x00ff    ; [0x58d0], 0x00ff
                                    ; tb.c:704
MOV       [szWork+0x12d], 0x0000    ; [0x58d1], 0x0000
                                    ; tb.c:705
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], 0x58d0        
                                    ; tb.c:707
MOV       [bp-i], 0x0000            ; [bp-0x24], 0x0000
JMP       L_140e              

L_13a4:                             ; tb.c:709
MOV       cx, [bp-i]                ; cx, [bp-0x24]
MOV       ax, 0x0001          
SHL       ax, cx              
AND       ax, [grbitScanEShip]      ; ax, [0x51aa]
CMP       ax, 0x0000          
JZ        L_13bf              

L_13b8:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_13c3              

L_13bf:
MOV       ax, 0x0000          
CWD       dx, ax              

L_13c3:
MOV       bx, [bp-c]                ; bx, [bp-0x22]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x56]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
                                    ; tb.c:710
MOV       ax, 0x0019          
IMUL      [bp-i]                    ; [bp-0x24]
MOV       cx, 0x57a4          
ADD       cx, ax              
PUSH      cx                  
MOV       ax, [bp-i]                ; ax, [bp-0x24]
ADD       ax, 0x017d          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; tb.c:711
MOV       ax, 0x0019          
IMUL      [bp-i]                    ; [bp-0x24]
MOV       cx, 0x57a4          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x22]
ADD       [bp-c], 0x0001            ; [bp-0x22], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x20]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:712
ADD       [bp-i], 0x0001            ; [bp-0x24], 0x0001

L_140e:
CMP       [bp-i], 0x0008            ; [bp-0x24], 0x0008
JL        L_13a4              

L_1417:                             ; tb.c:714
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCursorPos              ; void GetCursorPos(POINT *arg1)
                                    ; tb.c:715
PUSH      [hwndTb]                  ; [0x019a]
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScreenToClient            ; void ScreenToClient(HWND arg1, POINT *arg2)
                                    ; tb.c:716
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0xfffe          
PUSH      ax                  
LEA       ax, [bp-rgszScan]         ; ax, [bp-0x20]
PUSH      ax                  
LEA       ax, [bp-rgid]             ; ax, [bp-0x56]
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x22]
PUSH      [bp-pt+0x2]               ; [bp-0x6]
PUSH      [bp-pt]                   ; [bp-0x8]
PUSH      [hwndTb]                  ; [0x019a]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-iSel], ax             ; [bp-0x58], ax
                                    ; tb.c:718
CMP       [bp-iSel], 0xffff         ; [bp-0x58], 0xffff
JZ        L_167a              

L_1467:                             ; tb.c:721
CMP       [bp-iSel], 0x0004         ; [bp-0x58], 0x0004
JGE       L_14b8              

L_1470:                             ; tb.c:723
CMP       [bp-iSel], 0x0000         ; [bp-0x58], 0x0000
JNZ       L_1482              

L_1479:                             ; tb.c:724
MOV       [grbitScanEShip], 0x00ff  ; [0x51aa], 0x00ff
                                    ; tb.c:725
JMP       L_149a              

L_1482:
CMP       [bp-iSel], 0x0001         ; [bp-0x58], 0x0001
JNZ       L_1494              

L_148b:                             ; tb.c:726
XOR       [grbitScanEShip], 0x00ff  ; [0x51aa], 0x00ff
                                    ; tb.c:727
JMP       L_149a              

L_1494:                             ; tb.c:728
MOV       [grbitScanEShip], 0x0000  ; [0x51aa], 0x0000

L_149a:                             ; tb.c:730
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0800          
CMP       ax, 0x0000          
JNZ       L_1505              

L_14a8:
CMP       [grbitScanEShip], 0x0000  ; [0x51aa], 0x0000
JNZ       LInvalE             

L_14af:
JMP       L_1505              

L_14b8:                             ; tb.c:735
SUB       [bp-iSel], 0x0004         ; [bp-0x58], 0x0004
                                    ; tb.c:737
MOV       cx, [bp-iSel]             ; cx, [bp-0x58]
MOV       ax, 0x0001          
SHL       ax, cx              
XOR       [grbitScanEShip], ax      ; [0x51aa], ax
                                    ; tb.c:740
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0800          
CMP       ax, 0x0000          
JNZ       L_1505              

L_14d6:
MOV       cx, [bp-iSel]             ; cx, [bp-0x58]
MOV       ax, 0x0001          
SHL       ax, cx              
AND       ax, [grbitScanEShip]      ; ax, [0x51aa]
CMP       ax, 0x0000          
JZ        L_1505              

LInvalE:                            ; tb.c:743
OR        [grbitScan], 0x0800       ; [0x0588], 0x0800
                                    ; tb.c:744
PUSH      [hwndTb]                  ; [0x019a]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_1505:                             ; tb.c:748
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x0800          
CMP       ax, 0x0000          
JZ        L_167a              

L_1510:
JMP       L_1644              

L_1519:                             ; tb.c:758
MOV       [bp-c], 0x0000            ; [bp-0x1c], 0x0000
                                    ; tb.c:762
MOV       [bp-i], 0x0000            ; [bp-0x1e], 0x0000
JMP       L_159d              

L_1526:                             ; tb.c:764
MOV       ax, [iScanZoom]           ; ax, [0x05a0]
ADD       ax, 0x0004          
CMP       ax, [bp-i]                ; ax, [bp-0x1e]
JNZ       L_153b              

L_1534:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_153f              

L_153b:
MOV       ax, 0x0000          
CWD       dx, ax              

L_153f:
MOV       bx, [bp-c]                ; bx, [bp-0x1c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x42]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
                                    ; tb.c:765
MOV       bx, [bp-i]                ; bx, [bp-0x1e]
SHL       bx, 0x0001          
PUSH      cs:[bx+0xda4]       
MOV       ax, [PCTDPCTPCT]          ; ax, [0x01b8]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x1e]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, 0x57a4          
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
                                    ; tb.c:766
MOV       ax, [bp-i]                ; ax, [bp-0x1e]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, 0x57a4          
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x1c]
ADD       [bp-c], 0x0001            ; [bp-0x1c], 0x0001
SHL       ax, 0x0001          
LEA       bx, [bp-0x1a]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; tb.c:767
ADD       [bp-i], 0x0001            ; [bp-0x1e], 0x0001

L_159d:
CMP       [bp-i], 0x0009            ; [bp-0x1e], 0x0009
JL        L_1526              

L_15a6:                             ; tb.c:769
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCursorPos              ; void GetCursorPos(POINT *arg1)
                                    ; tb.c:770
PUSH      [hwndTb]                  ; [0x019a]
LEA       ax, [bp-pt]               ; ax, [bp-0x8]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScreenToClient            ; void ScreenToClient(HWND arg1, POINT *arg2)
                                    ; tb.c:771
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0xfffe          
PUSH      ax                  
LEA       ax, [bp-rgszScan]         ; ax, [bp-0x1a]
PUSH      ax                  
LEA       ax, [bp-rgid]             ; ax, [bp-0x42]
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x1c]
PUSH      [bp-pt+0x2]               ; [bp-0x6]
PUSH      [bp-pt]                   ; [bp-0x8]
PUSH      [hwndTb]                  ; [0x019a]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-iSel], ax             ; [bp-0x44], ax
                                    ; tb.c:773
CMP       [bp-iSel], 0xffff         ; [bp-0x44], 0xffff
JZ        L_167a              

L_15f6:                             ; tb.c:776
MOV       ax, [bp-iSel]             ; ax, [bp-0x44]
ADD       ax, 0x0f3d          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
CALLF     CommandHandler            ; void CommandHandler(HWND hwnd, WPARAM wParam)
ADD       sp, 0x0004          
                                    ; tb.c:777
JMP       L_167a              

L_160f:
CMP       ax, 0x0011          
JA        L_167a              

L_1617:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x1620]      

L_1644:                             ; tb.c:782
CMP       [bp+itb], 0x0006          ; [bp+0x6], 0x0006
JZ        L_1662              

L_164d:                             ; tb.c:783
PUSH      [hwndScanner]             ; [0x0190]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_1662:                             ; tb.c:785
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_167a              

L_1675:                             ; tb.c:786
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_167a:                             ; tb.c:787
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


