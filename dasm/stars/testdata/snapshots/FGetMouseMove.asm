; FGetMouseMove  (utilgen)
;   addr: 0009:4146  len=153
;   sig:  int16_t FGetMouseMove(POINT *ppt)
;   params:
;     POINT *          ppt            [BP+0x6]
;   locals:
;     MSG              msg            [BP-0x14]
;
;   stats: blocks=0  labels=0

L_4146:                             ; utilgen.c:1292
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0014          
PUSH      si                  
PUSH      di                  

L_4152:                             ; utilgen.c:1297
LEA       ax, [bp-msg]              ; ax, [bp-0x14]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     PeekMessage               ; int16_t PeekMessage(MSG *arg1, HWND arg2, uint16_t arg3, uint16_t arg4, uint16_t arg5)
CMP       ax, 0x0000          
JNZ       L_417c              

L_4176:                             ; utilgen.c:1298
MOV       ax, 0x0001          
JMP       L_41d9              

L_417c:                             ; utilgen.c:1299
MOV       ax, [bp-msg+0x2]          ; ax, [bp-0x12]
JMP       L_41c3              

L_4182:                             ; utilgen.c:1302
JMP       L_4152              

L_4185:                             ; utilgen.c:1305
MOV       ax, [bp-msg+0x6]          ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
MOV       bx, [bp+ppt]              ; bx, [bp+0x6]
MOV       [bx], ax            
                                    ; utilgen.c:1306
MOV       ax, [bp-msg+0x6]          ; ax, [bp-0xe]
MOV       dx, [bp-msg+0x8]          ; dx, [bp-0xc]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
MOV       bx, [bp+ppt]              ; bx, [bp+0x6]
MOV       [bx+0x2], ax        
                                    ; utilgen.c:1307
CMP       [bp-msg+0x2], 0x0202      ; [bp-0x12], 0x0202
JZ        L_41ba              

L_41b4:
MOV       ax, 0x0001          
JMP       L_41d9              

L_41ba:
MOV       ax, 0x0000          

L_41bd:
JMP       L_41d9              

L_41c3:
CMP       ax, 0x0200          
JZ        L_4185              

L_41cb:
CMP       ax, 0x0202          
JNZ       L_4152              

L_41d0:
JMP       L_4185              

L_41d3:
JMP       L_4152              

L_41d6:                             ; utilgen.c:1309
JMP       L_4152              

L_41d9:                             ; utilgen.c:1310
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


