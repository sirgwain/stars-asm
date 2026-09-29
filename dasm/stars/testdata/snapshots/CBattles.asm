; CBattles  (vcr)
;   addr: 001e:028c  len=213
;   sig:  int16_t CBattles()
;   locals:
;     int16_t          cBattles       [BP-0xc]
;     HB *             lphb           [BP-0xa]
;     BTLDATA *        lpbd           [BP-0x6]
;
;   stats: blocks=0  labels=0

L_028c:                             ; vcr.c:135
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000c          
PUSH      si                  
PUSH      di                  
                                    ; vcr.c:139
MOV       [bp-cBattles], 0x0000     ; [bp-0xc], 0x0000
                                    ; vcr.c:141
MOV       ax, [rglphb+0x2c]         ; ax, [0x0d60]
MOV       dx, [rglphb+0x2e]         ; dx, [0x0d62]
MOV       [bp-lphb], ax             ; [bp-0xa], ax
MOV       [bp-lphb+0x2], dx         ; [bp-0x8], dx
                                    ; vcr.c:142
CMP       [bp-lphb], 0x0000         ; [bp-0xa], 0x0000
JNZ       L_02bf              

L_02b0:
CMP       [bp-lphb+0x2], 0x0000     ; [bp-0x8], 0x0000
JNZ       L_02bf              

L_02b9:                             ; vcr.c:143
MOV       ax, 0x0000          
JMP       L_035b              

L_02bf:                             ; vcr.c:145
MOV       ax, 0x0012          
MOV       cx, [bp-lphb]             ; cx, [bp-0xa]
MOV       dx, [bp-lphb+0x2]         ; dx, [bp-0x8]
ADD       cx, ax              
MOV       [bp-lpbd], cx             ; [bp-0x6], cx
MOV       [bp-lpbd+0x2], dx         ; [bp-0x4], dx

L_02d3:                             ; vcr.c:149
LES       bx, [bp-lpbd]             ; bx, [bp-0x6]
CMP       es:[bx], 0xffff     
JNZ       L_0329              

L_02df:                             ; vcr.c:151
LES       bx, [bp-lphb]             ; bx, [bp-0xa]
MOV       ax, es:[bx+0x8]     
MOV       dx, es:[bx+0xa]     
MOV       [bp-lphb], ax             ; [bp-0xa], ax
MOV       [bp-lphb+0x2], dx         ; [bp-0x8], dx
                                    ; vcr.c:152
CMP       [bp-lphb], 0x0000         ; [bp-0xa], 0x0000
JNZ       L_0302              

L_02f9:
CMP       [bp-lphb+0x2], 0x0000     ; [bp-0x8], 0x0000
JZ        L_030f              

L_0302:
LES       bx, [bp-lphb]             ; bx, [bp-0xa]
CMP       es:[bx+0x6], 0x0010 
JA        L_0315              

L_030f:                             ; vcr.c:154
MOV       ax, [bp-cBattles]         ; ax, [bp-0xc]
JMP       L_035b              

L_0315:                             ; vcr.c:157
MOV       ax, 0x0012          
MOV       cx, [bp-lphb]             ; cx, [bp-0xa]
MOV       dx, [bp-lphb+0x2]         ; dx, [bp-0x8]
ADD       cx, ax              
MOV       [bp-lpbd], cx             ; [bp-0x6], cx
MOV       [bp-lpbd+0x2], dx         ; [bp-0x4], dx
                                    ; vcr.c:159
JMP       L_02d3              

L_0329:
LES       bx, [bp-lpbd]             ; bx, [bp-0x6]
CMP       es:[bx+0x6], 0x0000 
JNZ       L_033f              

L_0336:                             ; vcr.c:160
MOV       ax, [bp-cBattles]         ; ax, [bp-0xc]
JMP       L_035b              

L_033f:                             ; vcr.c:163
LES       bx, [bp-lpbd]             ; bx, [bp-0x6]
MOV       ax, es:[bx+0x6]     
MOV       cx, [bp-lpbd]             ; cx, [bp-0x6]
MOV       dx, [bp-lpbd+0x2]         ; dx, [bp-0x4]
ADD       cx, ax              
MOV       [bp-lpbd], cx             ; [bp-0x6], cx
MOV       [bp-lpbd+0x2], dx         ; [bp-0x4], dx
                                    ; vcr.c:164
ADD       [bp-cBattles], 0x0001     ; [bp-0xc], 0x0001

L_0358:                             ; vcr.c:166
JMP       L_02d3              

L_035b:                             ; vcr.c:167
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


