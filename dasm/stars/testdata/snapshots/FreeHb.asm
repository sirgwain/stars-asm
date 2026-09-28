; FreeHb  (memory)
;   addr: 000d:02d8  len=112
;   sig:  void FreeHb(HB *lphb)
;   params:
;     HB *             lphb           [BP+0x6]
;   locals:
;     HB *             lphbNext       [BP-0x8]
;     HGLOBAL          hmem           [BP-0x4]
;
;   stats: blocks=0  labels=0

L_02d8:                             ; memory.c:114
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0008          
PUSH      si                  
PUSH      di                  
                                    ; memory.c:118
CMP       [bp+lphb], 0x0000         ; [bp+0x6], 0x0000
JNZ       L_0330              

L_02ea:
CMP       [bp+lphb+0x2], 0x0000     ; [bp+0x8], 0x0000
JZ        L_0342              

L_02f0:
JMP       L_0330              

L_02f9:                             ; memory.c:123
LES       bx, [bp+lphb]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x8]     
MOV       dx, es:[bx+0xa]     
MOV       [bp-lphbNext], ax         ; [bp-0x8], ax
MOV       [bp-lphbNext+0x2], dx     ; [bp-0x6], dx
                                    ; memory.c:125
LES       bx, [bp+lphb]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0xc]     
MOV       [bp-hmem], ax             ; [bp-0x4], ax
                                    ; memory.c:126
PUSH      [bp-hmem]                 ; [bp-0x4]
CALLF     GlobalUnlock              ; int16_t GlobalUnlock(HGLOBAL arg1)
                                    ; memory.c:127
PUSH      [bp-hmem]                 ; [bp-0x4]
CALLF     GlobalFree                ; HGLOBAL GlobalFree(HGLOBAL arg1)
                                    ; memory.c:129
MOV       ax, [bp-lphbNext]         ; ax, [bp-0x8]
MOV       dx, [bp-lphbNext+0x2]     ; dx, [bp-0x6]
MOV       [bp+lphb], ax             ; [bp+0x6], ax
MOV       [bp+lphb+0x2], dx         ; [bp+0x8], dx

L_0330:                             ; memory.c:130
CMP       [bp+lphb], 0x0000         ; [bp+0x6], 0x0000
JNZ       L_02f9              

L_0339:
CMP       [bp+lphb+0x2], 0x0000     ; [bp+0x8], 0x0000
JNZ       L_02f9              

L_0342:                             ; memory.c:131
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


