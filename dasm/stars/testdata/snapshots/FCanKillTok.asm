; FCanKillTok  (battle)
;   addr: 001f:391e  len=264
;   sig:  int16_t FCanKillTok(TOK *ptok1, TOK *ptok2)
;   params:
;     TOK *            ptok1          [BP+0x6]
;     TOK *            ptok2          [BP+0xa]
;   locals:
;     int32_t          lp2            [BP-0xa]
;     int32_t          lp1            [BP-0x6]
;
;   stats: blocks=0  labels=0

L_391e:                             ; battle.c:1283
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000a          
PUSH      si                  
PUSH      di                  
                                    ; battle.c:1288
PUSH      [bp+ptok1+0x2]            ; [bp+0x8]
PUSH      [bp+ptok1]                ; [bp+0x6]
CALLF     LpshdefFromTok            ; SHDEF * LpshdefFromTok(TOK *ptok)
ADD       sp, 0x0004          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x87]    
MOV       dx, es:[bx+0x89]    
MOV       [bp-lp1], ax              ; [bp-0x6], ax
MOV       [bp-lp1+0x2], dx          ; [bp-0x4], dx
                                    ; battle.c:1289
PUSH      [bp+ptok2+0x2]            ; [bp+0xc]
PUSH      [bp+ptok2]                ; [bp+0xa]
CALLF     LpshdefFromTok            ; SHDEF * LpshdefFromTok(TOK *ptok)
ADD       sp, 0x0004          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x87]    
MOV       dx, es:[bx+0x89]    
MOV       [bp-lp2], ax              ; [bp-0xa], ax
MOV       [bp-lp2+0x2], dx          ; [bp-0x8], dx
                                    ; battle.c:1291
MOV       ax, [bp-lp1]              ; ax, [bp-0x6]
MOV       dx, [bp-lp1+0x2]          ; dx, [bp-0x4]
CMP       [bp-lp2+0x2], dx          ; [bp-0x8], dx
JL        L_398f              

L_3979:
JG        L_3986              

L_397e:
CMP       [bp-lp2], ax              ; [bp-0xa], ax
JBE       L_398f              

L_3986:                             ; battle.c:1292
MOV       ax, 0x0000          
JMP       L_3a20              

L_398f:
MOV       ax, [bp-lp2]              ; ax, [bp-0xa]
MOV       dx, [bp-lp2+0x2]          ; dx, [bp-0x8]
AND       ax, 0xf000          
AND       dx, 0x7fff          
MOV       cx, [bp-lp1]              ; cx, [bp-0x6]
MOV       bx, [bp-lp1+0x2]          ; bx, [bp-0x4]
AND       cx, 0xf000          
AND       bx, 0x7fff          
CMP       dx, bx              
JG        L_39c6              

L_39b1:
JL        L_39bd              

L_39b6:
CMP       ax, cx              
JNC       L_39c6              

L_39bd:                             ; battle.c:1294
MOV       ax, 0x0001          
JMP       L_3a20              

L_39c6:
MOV       ax, [bp-lp2]              ; ax, [bp-0xa]
MOV       dx, [bp-lp2+0x2]          ; dx, [bp-0x8]
AND       ax, 0xff00          
AND       dx, 0x7fff          
MOV       cx, [bp-lp1]              ; cx, [bp-0x6]
MOV       bx, [bp-lp1+0x2]          ; bx, [bp-0x4]
AND       cx, 0xff00          
AND       bx, 0x7fff          
CMP       ax, cx              
JNZ       L_3a1a              

L_39e8:
CMP       dx, bx              
JNZ       L_3a1a              

L_39ef:
LES       bx, [bp+ptok1]            ; bx, [bp+0x6]
MOV       ax, es:[bx+0x19]    
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x000f          
LES       bx, [bp+ptok2]            ; bx, [bp+0xa]
MOV       dx, es:[bx+0x19]    
MOV       cx, 0x0008          
SHR       dx, cx              
AND       dx, 0x000f          
CMP       ax, dx              
JC        L_3a1a              

L_3a14:                             ; battle.c:1296
MOV       ax, 0x0001          
JMP       L_3a20              

L_3a1a:                             ; battle.c:1297
MOV       ax, 0x0000          

L_3a20:                             ; battle.c:1298
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


