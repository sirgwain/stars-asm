; PushRandom  (utilgen)
;   addr: 0009:1440  len=98
;   sig:  void PushRandom(int32_t lNew1, int32_t lNew2)
;   params:
;     int32_t          lNew1          [BP+0x6]
;     int32_t          lNew2          [BP+0xa]
;
;   stats: blocks=0  labels=0

L_1440:                             ; utilgen.c:29
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:31
MOV       bx, [cRandStack]          ; bx, [0x1378]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [lRandSeed1]          ; ax, [0x1370]
MOV       dx, [lRandSeed1+0x2]      ; dx, [0x1372]
MOV       [bx+0x5238], ax     
MOV       [bx+0x523a], dx     
                                    ; utilgen.c:32
MOV       ax, [cRandStack]          ; ax, [0x1378]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x5238          
ADD       bx, ax              
MOV       ax, [lRandSeed2]          ; ax, [0x1374]
MOV       dx, [lRandSeed2+0x2]      ; dx, [0x1376]
MOV       [bx+0x4], ax        
MOV       [bx+0x6], dx        
                                    ; utilgen.c:33
ADD       [cRandStack], 0x0001      ; [0x1378], 0x0001
                                    ; utilgen.c:34
MOV       ax, [bp+lNew1]            ; ax, [bp+0x6]
MOV       dx, [bp+lNew1+0x2]        ; dx, [bp+0x8]
MOV       [lRandSeed1], ax          ; [0x1370], ax
MOV       [lRandSeed1+0x2], dx      ; [0x1372], dx
                                    ; utilgen.c:35
MOV       ax, [bp+lNew2]            ; ax, [bp+0xa]
MOV       dx, [bp+lNew2+0x2]        ; dx, [bp+0xc]
MOV       [lRandSeed2], ax          ; [0x1374], ax
MOV       [lRandSeed2+0x2], dx      ; [0x1376], dx
                                    ; utilgen.c:36
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


