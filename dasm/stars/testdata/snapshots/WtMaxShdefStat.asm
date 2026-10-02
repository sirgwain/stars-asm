; WtMaxShdefStat  (ship)
;   addr: 000b:41b2  len=771
;   sig:  int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat)
;   params:
;     SHDEF *          lpshdef        [BP+0x6]
;     int16_t          grStat         [BP+0xa]
;   locals:
;     HUL *            lphul          [BP-0xa]
;     int16_t          j              [BP-0x6]
;     int16_t          wt             [BP-0x4]
;
;   stats: blocks=0  labels=0

L_41b2:                             ; ship.c:1589
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000a          
PUSH      si                  
PUSH      di                  
                                    ; ship.c:1594
MOV       ax, [bp+lpshdef]          ; ax, [bp+0x6]
MOV       dx, [bp+lpshdef+0x2]      ; dx, [bp+0x8]
MOV       [bp-lphul], ax            ; [bp-0xa], ax
MOV       [bp-lphul+0x2], dx        ; [bp-0x8], dx
                                    ; ship.c:1596
MOV       ax, [bp+grStat]           ; ax, [bp+0xa]
JMP       L_4496              

L_41cd:                             ; ship.c:1599
LES       bx, [bp-lphul]            ; bx, [bp-0xa]
PUSH      es:[bx]             
CALLF     LphuldefFromId            ; HULDEF * LphuldefFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x36]    
MOV       [bp-wt], ax               ; [bp-0x4], ax
                                    ; ship.c:1600
MOV       [bp-j], 0x0000            ; [bp-0x6], 0x0000
JMP       L_41f2              

L_41ee:
ADD       [bp-j], 0x0001            ; [bp-0x6], 0x0001

L_41f2:
LES       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       al, es:[bx+0x7a]    
AND       ax, 0x00ff          
CMP       [bp-j], ax                ; [bp-0x6], ax
JGE       L_44a9              

L_4204:                             ; ship.c:1602
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x1000     
JNZ       L_42c8              

L_4224:                             ; ship.c:1604
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JNZ       L_4276              

L_4249:                             ; ship.c:1605
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x00fa          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax
                                    ; ship.c:1606
JMP       L_41ee              

L_4276:
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JNZ       L_41ee              

L_429b:                             ; ship.c:1607
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x01f4          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax

L_42c5:                             ; ship.c:1609
JMP       L_41ee              

L_42c8:
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0800     
JNZ       L_41ee              

L_42e8:                             ; ship.c:1611
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0010          
JNZ       L_41ee              

L_430d:                             ; ship.c:1612
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x00c8          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax

L_4337:                             ; ship.c:1614
JMP       L_41ee              

L_433d:                             ; ship.c:1617
LES       bx, [bp-lphul]            ; bx, [bp-0xa]
PUSH      es:[bx]             
CALLF     LphuldefFromId            ; HULDEF * LphuldefFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x34]    
MOV       [bp-wt], ax               ; [bp-0x4], ax
                                    ; ship.c:1618
MOV       [bp-j], 0x0000            ; [bp-0x6], 0x0000
JMP       L_4362              

L_435e:
ADD       [bp-j], 0x0001            ; [bp-0x6], 0x0001

L_4362:
LES       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       al, es:[bx+0x7a]    
AND       ax, 0x00ff          
CMP       [bp-j], ax                ; [bp-0x6], ax
JGE       L_44a9              

L_4374:                             ; ship.c:1620
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x1000     
JNZ       L_435e              

L_4394:                             ; ship.c:1621
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JNZ       L_43e6              

L_43b9:                             ; ship.c:1622
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x0032          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax
                                    ; ship.c:1623
JMP       L_435e              

L_43e6:
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JNZ       L_4438              

L_440b:                             ; ship.c:1624
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x0064          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax
                                    ; ship.c:1625
JMP       L_435e              

L_4438:
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JNZ       L_435e              

L_445d:                             ; ship.c:1626
MOV       ax, 0x003a          
MOV       bx, [bp-lphul]            ; bx, [bp-0xa]
MOV       cx, [bp-lphul+0x2]        ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp-j]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x00fa          
IMUL      cx                  
ADD       [bp-wt], ax               ; [bp-0x4], ax

L_4487:                             ; ship.c:1627
JMP       L_435e              

L_448d:                             ; ship.c:1631
MOV       ax, 0x0000          
JMP       L_44af              

L_4496:
CMP       ax, 0x0001          
JZ        L_41cd              

L_449e:
CMP       ax, 0x0002          
JNZ       L_448d              

L_44a3:
JMP       L_433d              

L_44a9:                             ; ship.c:1633
MOV       ax, [bp-wt]               ; ax, [bp-0x4]

L_44af:                             ; ship.c:1634
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


