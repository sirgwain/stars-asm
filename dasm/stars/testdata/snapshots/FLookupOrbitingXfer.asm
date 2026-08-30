; FLookupOrbitingXfer  (util)
;   addr: 0008:24fa  len=484
;   sig:  int16_t FLookupOrbitingXfer(int16_t idPlanet, int16_t iNth, XFER *pxf, int16_t idSkip)
;   params:
;     int16_t          idPlanet       [BP+0x6]
;     int16_t          iNth           [BP+0x8]
;     XFER *           pxf            [BP+0xa]
;     int16_t          idSkip         [BP+0xc]
;   locals:
;     THING *          lpthMac        [BP-0x10]
;     FLEET *          lpfl           [BP-0xc]
;     THING *          lpth           [BP-0x8]
;     int16_t          i              [BP-0x4]
;
;   stats: blocks=0  labels=0

L_24fa:                             ; util.c:963
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0014          
PUSH      si                  
PUSH      di                  
                                    ; util.c:969
CMP       [cFleet], 0x0000          ; [0x5356], 0x0000
JG        L_2513              

L_250d:                             ; util.c:970
MOV       ax, 0x0000          
JMP       L_26d8              

L_2513:                             ; util.c:972
CMP       [cFleet], 0x0000          ; [0x5356], 0x0000
JZ        L_2606              

L_251d:                             ; util.c:976
MOV       [bp-i], 0x0000            ; [bp-0x4], 0x0000
JMP       L_2529              

L_2525:
ADD       [bp-i], 0x0001            ; [bp-0x4], 0x0001

L_2529:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x4], ax
JGE       L_2606              

L_2534:
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0xc], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0xa], dx
CMP       ax, 0x0000          
JNZ       L_2564              

L_255c:
CMP       dx, 0x0000          
JZ        L_2606              

L_2564:                             ; util.c:978
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+idPlanet]         ; ax, [bp+0x6]
CMP       es:[bx+0x6], ax     
JNZ       L_2525              

L_2573:
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+idSkip]           ; ax, [bp+0xc]
CMP       es:[bx], ax         
JZ        L_2525              

L_2581:
CMP       [bp+idSkip], 0xffff       ; [bp+0xc], 0xffff
JZ        L_25ad              

L_258a:
MOV       bx, 0x4956          
MOV       cx, ds              
MOV       es, cx              
MOV       ax, [sel]                 ; ax, es:[bx]
MOV       dx, [sel+0x2]             ; dx, es:[bx+0x2]
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
CMP       es:[bx+0x8], ax     
JNZ       L_2525              

L_25a4:
CMP       es:[bx+0xa], dx     
JNZ       L_2525              

L_25ad:
MOV       ax, [bp+iNth]             ; ax, [bp+0x8]
SUB       [bp+iNth], 0x0001         ; [bp+0x8], 0x0001
CMP       ax, 0x0000          
JNZ       L_2525              

L_25bc:                             ; util.c:980
CMP       [bp+pxf], 0x0000          ; [bp+0xa], 0x0000
JZ        L_25fd              

L_25c5:                             ; util.c:982
MOV       ax, 0x0004          
MOV       di, [bp+pxf]              ; di, [bp+0xa]
ADD       di, ax              
PUSH      di                  
MOV       si, [bp-lpfl]             ; si, [bp-0xc]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0xa]
PUSH      ds                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x003e          
MOVSW.REP es:[di], ds:[si]    
POP       ds                  
POP       di                  
MOV       ax, 0x0004          
MOV       cx, [bp+0xa]        
ADD       cx, ax              
MOV       ax, cx              
                                    ; util.c:983
MOV       bx, [bp+pxf]              ; bx, [bp+0xa]
MOV       [bx+0x2], 0x0002    
                                    ; util.c:984
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx]         
MOV       bx, [bp+pxf]              ; bx, [bp+0xa]
MOV       [bx], ax            

L_25fd:                             ; util.c:986
MOV       ax, 0x0001          
JMP       L_26d8              

L_2603:                             ; util.c:988
JMP       L_2525              

L_2606:                             ; util.c:991
MOV       ax, [lpThings]            ; ax, [0x1698]
MOV       dx, [lpThings+0x2]        ; dx, [0x169a]
MOV       [bp-lpth], ax             ; [bp-0x8], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0x6], dx
MOV       [bp-0x14], ax       
MOV       [bp-0x12], dx       
MOV       ax, 0x0012          
IMUL      [cThing]                  ; [0x169c]
MOV       cx, [bp-0x14]       
MOV       dx, [bp-0x12]       
ADD       cx, ax              
MOV       [bp-lpthMac], cx          ; [bp-0x10], cx
MOV       [bp-lpthMac+0x2], dx      ; [bp-0xe], dx
JMP       L_26c4              

L_2631:                             ; util.c:993
LES       bx, [bp-lpth]             ; bx, [bp-0x8]
MOV       ax, es:[bx]         
MOV       cx, 0x000d          
SHR       ax, cx              
AND       ax, 0x0007          
CMP       ax, 0x0001          
JNZ       L_26c0              

L_2647:
MOV       bx, 0x4956          
MOV       cx, ds              
MOV       es, cx              
MOV       ax, [sel]                 ; ax, es:[bx]
MOV       dx, [sel+0x2]             ; dx, es:[bx+0x2]
LES       bx, [bp-lpth]             ; bx, [bp-0x8]
CMP       es:[bx+0x2], ax     
JNZ       L_26c0              

L_2661:
CMP       es:[bx+0x4], dx     
JNZ       L_26c0              

L_266a:
MOV       ax, [bp+iNth]             ; ax, [bp+0x8]
SUB       [bp+iNth], 0x0001         ; [bp+0x8], 0x0001
CMP       ax, 0x0000          
JNZ       L_26c0              

L_2679:                             ; util.c:995
CMP       [bp+pxf], 0x0000          ; [bp+0xa], 0x0000
JZ        L_26ba              

L_2682:                             ; util.c:997
MOV       ax, 0x0004          
MOV       di, [bp+pxf]              ; di, [bp+0xa]
ADD       di, ax              
PUSH      di                  
MOV       si, [bp-lpth]             ; si, [bp-0x8]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0x6]
PUSH      ds                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x0009          
MOVSW.REP es:[di], ds:[si]    
POP       ds                  
POP       di                  
MOV       ax, 0x0004          
MOV       cx, [bp+0xa]        
ADD       cx, ax              
MOV       ax, cx              
                                    ; util.c:998
MOV       bx, [bp+pxf]              ; bx, [bp+0xa]
MOV       [bx+0x2], 0x0008    
                                    ; util.c:999
LES       bx, [bp-lpth]             ; bx, [bp-0x8]
MOV       ax, es:[bx]         
MOV       bx, [bp+pxf]              ; bx, [bp+0xa]
MOV       [bx], ax            

L_26ba:                             ; util.c:1001
MOV       ax, 0x0001          
JMP       L_26d8              

L_26c0:                             ; util.c:1003
ADD       [bp-lpth], 0x0012         ; [bp-0x8], 0x0012

L_26c4:
MOV       ax, [bp-lpthMac]          ; ax, [bp-0x10]
MOV       dx, [bp-0xe]        
CMP       [bp-lpth], ax             ; [bp-0x8], ax
JC        L_2631              

L_26d2:                             ; util.c:1005
MOV       ax, 0x0000          

L_26d8:                             ; util.c:1006
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


