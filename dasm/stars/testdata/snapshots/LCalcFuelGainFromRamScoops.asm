; LCalcFuelGainFromRamScoops  (util)
;   addr: 0008:56b8  len=494
;   sig:  int32_t LCalcFuelGainFromRamScoops(FLEET *lpfl, int16_t iWarp, int32_t dTravel)
;   params:
;     FLEET *          lpfl           [BP+0x6]
;     int16_t          iWarp          [BP+0xa]
;     int32_t          dTravel        [BP+0xc]
;   locals:
;     int32_t          pctShip10      [BP-0x14]
;     int32_t          pct10          [BP-0x10]
;     SHDEF *          lpshdef        [BP-0xc]
;     int16_t *        rgiFuel        [BP-0x8]
;     int16_t          i              [BP-0x4]
;
;   stats: blocks=0  labels=0

L_56b8:                             ; util.c:2258
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0014          
PUSH      si                  
PUSH      di                  
                                    ; util.c:2262
MOV       [bp-pct10], 0x0000        ; [bp-0x10], 0x0000
MOV       [bp-pct10+0x2], 0x0000    ; [bp-0xe], 0x0000
                                    ; util.c:2267
CMP       [bp+iWarp], 0x000a        ; [bp+0xa], 0x000a
JLE       L_56dd              

L_56d4:                             ; util.c:2268
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_58a0              

L_56dd:                             ; util.c:2270
MOV       [bp-i], 0x0000            ; [bp-0x4], 0x0000
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0xfe]       
MOV       dx, [bx+0x100]      
MOV       [bp-lpshdef], ax          ; [bp-0xc], ax
MOV       [bp-lpshdef+0x2], dx      ; [bp-0xa], dx
JMP       L_5712              

L_56fe:
MOV       ax, [bp-0x4]        
ADD       [bp-i], 0x0001            ; [bp-0x4], 0x0001
MOV       ax, [bp-0xc]        
MOV       es, [bp-0xa]        
ADD       [bp-lpshdef], 0x0093      ; [bp-0xc], 0x0093
MOV       dx, es              

L_5712:
CMP       [bp-i], 0x0010            ; [bp-0x4], 0x0010
JGE       L_5880              

L_571b:                             ; util.c:2271
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JZ        L_56fe              

L_5738:                             ; util.c:2276
LES       bx, [bp-lpshdef]          ; bx, [bp-0xc]
MOV       ax, es:[bx+0x3c]    
AND       ax, 0x00ff          
PUSH      ax                  
CALLF     LpengineFromId            ; ENGINE * LpengineFromId(int16_t id)
ADD       sp, 0x0002          
MOV       cx, 0x0036          
ADD       ax, cx              
MOV       [bp-rgiFuel], ax          ; [bp-0x8], ax
MOV       [bp-rgiFuel+0x2], dx      ; [bp-0x6], dx
                                    ; util.c:2277
MOV       [bp-pctShip10], 0x0000    ; [bp-0x14], 0x0000
MOV       [bp-pctShip10+0x2], 0x0000; [bp-0x12], 0x0000
                                    ; util.c:2279
CMP       [bp+iWarp], 0x0009        ; [bp+0xa], 0x0009
JG        L_56fe              

L_5766:
JMP       L_576c              

L_5769:                             ; util.c:2280
JMP       L_56fe              

L_576c:                             ; util.c:2282
MOV       ax, [bp+iWarp]            ; ax, [bp+0xa]
SHL       ax, 0x0001          
MOV       bx, [bp-rgiFuel]          ; bx, [bp-0x8]
MOV       cx, [bp-rgiFuel+0x2]      ; cx, [bp-0x6]
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JNZ       L_5852              

L_5784:                             ; util.c:2284
LES       bx, [bp-lpshdef]          ; bx, [bp-0xc]
MOV       ax, es:[bx+0x3c]    
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       dx, 0x0000          
ADD       [bp-pctShip10], ax        ; [bp-0x14], ax
ADC       [bp-pctShip10+0x2], dx    ; [bp-0x12], dx
                                    ; util.c:2286
MOV       ax, [bp+iWarp]            ; ax, [bp+0xa]
ADD       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp-rgiFuel]          ; bx, [bp-0x8]
MOV       cx, [bp-rgiFuel+0x2]      ; cx, [bp-0x6]
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JNZ       L_5852              

L_57b7:                             ; util.c:2288
LES       bx, [bp-lpshdef]          ; bx, [bp-0xc]
MOV       ax, es:[bx+0x3c]    
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
SHL       ax, 0x0001          
MOV       dx, 0x0000          
ADD       [bp-pctShip10], ax        ; [bp-0x14], ax
ADC       [bp-pctShip10+0x2], dx    ; [bp-0x12], dx
                                    ; util.c:2289
CMP       [bp+iWarp], 0x0009        ; [bp+0xa], 0x0009
JGE       L_5852              

L_57da:
MOV       ax, [bp+iWarp]            ; ax, [bp+0xa]
ADD       ax, 0x0002          
SHL       ax, 0x0001          
MOV       bx, [bp-rgiFuel]          ; bx, [bp-0x8]
MOV       cx, [bp-rgiFuel+0x2]      ; cx, [bp-0x6]
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JNZ       L_5852              

L_57f5:                             ; util.c:2291
LES       bx, [bp-lpshdef]          ; bx, [bp-0xc]
MOV       ax, es:[bx+0x3c]    
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x0003          
IMUL      cx                  
MOV       dx, 0x0000          
ADD       [bp-pctShip10], ax        ; [bp-0x14], ax
ADC       [bp-pctShip10+0x2], dx    ; [bp-0x12], dx
                                    ; util.c:2293
CMP       [bp+iWarp], 0x0008        ; [bp+0xa], 0x0008
JGE       L_5852              

L_581b:
MOV       ax, [bp+iWarp]            ; ax, [bp+0xa]
ADD       ax, 0x0003          
SHL       ax, 0x0001          
MOV       bx, [bp-rgiFuel]          ; bx, [bp-0x8]
MOV       cx, [bp-rgiFuel+0x2]      ; cx, [bp-0x6]
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JNZ       L_5852              

L_5836:                             ; util.c:2294
LES       bx, [bp-lpshdef]          ; bx, [bp-0xc]
MOV       ax, es:[bx+0x3c]    
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       dx, 0x0000          
ADD       [bp-pctShip10], ax        ; [bp-0x14], ax
ADC       [bp-pctShip10+0x2], dx    ; [bp-0x12], dx

L_5852:                             ; util.c:2298
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-pctShip10+0x2]        ; [bp-0x12]
PUSH      [bp-pctShip10]            ; [bp-0x14]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
ADD       [bp-pct10], ax            ; [bp-0x10], ax
ADC       [bp-pct10+0x2], dx        ; [bp-0xe], dx

L_587d:                             ; util.c:2301
JMP       L_56fe              

L_5880:
PUSH      [bp+dTravel+0x2]          ; [bp+0xe]
PUSH      [bp+dTravel]              ; [bp+0xc]
PUSH      [bp-pct10+0x2]            ; [bp-0xe]
PUSH      [bp-pct10]                ; [bp-0x10]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-pct10], ax            ; [bp-0x10], ax
MOV       [bp-pct10+0x2], dx        ; [bp-0xe], dx
                                    ; util.c:2303
MOV       ax, [bp-pct10]            ; ax, [bp-0x10]
MOV       dx, [bp-pct10+0x2]        ; dx, [bp-0xe]

L_58a0:                             ; util.c:2304
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


