; CMaxMines  (planet)
;   addr: 000a:7248  len=187
;   sig:  int16_t CMaxMines(PLANET *lppl, int16_t iplr)
;   params:
;     PLANET *         lppl           [BP+0x6]
;     int16_t          iplr           [BP+0xa]
;   locals:
;     int16_t          iEff           [BP-0xc]
;     int32_t          lPopMax        [BP-0xa]
;     int32_t          cMax           [BP-0x6]
;
;   stats: blocks=0  labels=0

L_7248:                             ; planet.c:2931
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000c          
PUSH      si                  
PUSH      di                  
                                    ; planet.c:2934
PUSH      [bp+iplr]                 ; [bp+0xa]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
CALLF     CalcPlanetMaxPop          ; int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr)
ADD       sp, 0x0004          
MOV       [bp-lPopMax], ax          ; [bp-0xa], ax
MOV       [bp-lPopMax+0x2], dx      ; [bp-0x8], dx
                                    ; planet.c:2936
MOV       ax, 0x0006          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
MOV       [bp-iEff], ax             ; [bp-0xc], ax
                                    ; planet.c:2937
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-iEff]             ; ax, [bp-0xc]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-lPopMax+0x2]          ; [bp-0x8]
PUSH      [bp-lPopMax]              ; [bp-0xa]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cMax], ax             ; [bp-0x6], ax
MOV       [bp-cMax+0x2], dx         ; [bp-0x4], dx
                                    ; planet.c:2938
CMP       [bp-cMax+0x2], 0x0000     ; [bp-0x4], 0x0000
JG        L_72ca              

L_72b2:
JL        L_72c0              

L_72b7:
CMP       [bp-cMax], 0x000a         ; [bp-0x6], 0x000a
JNC       L_72ca              

L_72c0:                             ; planet.c:2939
MOV       [bp-cMax], 0x000a         ; [bp-0x6], 0x000a
MOV       [bp-cMax+0x2], 0x0000     ; [bp-0x4], 0x0000

L_72ca:                             ; planet.c:2941
MOV       ax, 0x000e          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
CMP       ax, 0x0008          
JNZ       L_72f4              

L_72ea:                             ; planet.c:2942
MOV       [bp-cMax], 0x0000         ; [bp-0x6], 0x0000
MOV       [bp-cMax+0x2], 0x0000     ; [bp-0x4], 0x0000

L_72f4:                             ; planet.c:2946
MOV       ax, [bp-cMax]             ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        

L_72fd:                             ; planet.c:2947
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


