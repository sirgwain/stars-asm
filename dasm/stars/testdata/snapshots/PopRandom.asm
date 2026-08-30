; PopRandom  (utilgen)
;   addr: 0009:14a2  len=72
;   sig:  void PopRandom()
;
;   stats: blocks=0  labels=0

L_14a2:                             ; utilgen.c:39
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:42
SUB       [cRandStack], 0x0001      ; [0x1378], 0x0001
                                    ; utilgen.c:43
MOV       bx, [cRandStack]          ; bx, [0x1378]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x5238]     
MOV       dx, [bx+0x523a]     
MOV       [lRandSeed1], ax          ; [0x1370], ax
MOV       [lRandSeed1+0x2], dx      ; [0x1372], dx
                                    ; utilgen.c:44
MOV       ax, [cRandStack]          ; ax, [0x1378]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x5238          
ADD       bx, ax              
MOV       ax, [bx+0x4]        
MOV       dx, [bx+0x6]        
MOV       [lRandSeed2], ax          ; [0x1374], ax
MOV       [lRandSeed2+0x2], dx      ; [0x1376], dx
                                    ; utilgen.c:45
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


