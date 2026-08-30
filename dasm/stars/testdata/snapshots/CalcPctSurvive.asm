; CalcPctSurvive  (util)
;   addr: 0008:02f6  len=432
;   sig:  void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart)
;   params:
;     PLANET *         lppl           [BP+0x6]
;     float *          ppct           [BP+0xa]
;     float *          ppctSmart      [BP+0xc]
;   locals:
;     PART             part           [BP-0x14]
;     float            pct            [BP-0xc]
;     int32_t          cDefenses      [BP-0x8]
;     int16_t          iPlrSav        [BP-0x4]
;     block 0008:0369  len=0x107
;       int16_t          cMax           [BP-0x16]
;
;   stats: blocks=1  labels=0

L_02f6:                             ; util.c:113
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x001e          
PUSH      si                  
PUSH      di                  
                                    ; util.c:119
CMP       [bp+ppctSmart], 0x0000    ; [bp+0xc], 0x0000
JZ        L_0317              

L_0308:                             ; util.c:120
WAIT                          
FLD       [0x1c9e]            
NOP                           
WAIT                          
MOV       bx, [bp+ppctSmart]        ; bx, [bp+0xc]
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          

L_0317:                             ; util.c:122
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
CMP       es:[bx+0x2], 0xffff 
JZ        L_0487              

L_0324:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_0345              

L_033d:
CMP       dx, 0x0000          
JZ        L_0487              

L_0345:                             ; util.c:124
MOV       ax, [idPlayer]            ; ax, [0x018c]
MOV       [bp-iPlrSav], ax          ; [bp-0x4], ax
                                    ; util.c:125
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
MOV       [idPlayer], ax            ; [0x018c], ax
                                    ; util.c:126
LEA       ax, [bp-part]             ; ax, [bp-0x14]
PUSH      ax                  
CALLF     FGetBestDefensePart       ; int16_t FGetBestDefensePart(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_0473              

L_0369:                             ; util.c:130
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-cDefenses], ax        ; [bp-0x8], ax
MOV       [bp-cDefenses+0x2], dx    ; [bp-0x6], dx
                                    ; util.c:131
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxOperableDefenses      ; int16_t CMaxOperableDefenses(PLANET *lppl, int16_t iplr, int16_t fNextYear)
ADD       sp, 0x0008          
MOV       [bp-cMax], ax             ; [bp-0x16], ax
                                    ; util.c:132
MOV       ax, [bp-cMax]             ; ax, [bp-0x16]
CWD       dx, ax              
CMP       dx, [bp-cDefenses+0x2]    ; dx, [bp-0x6]
JG        L_03bf              

L_03a8:
JL        L_03b5              

L_03ad:
CMP       ax, [bp-cDefenses]        ; ax, [bp-0x8]
JNC       L_03bf              

L_03b5:                             ; util.c:133
MOV       ax, [bp-cMax]             ; ax, [bp-0x16]
CWD       dx, ax              
MOV       [bp-cDefenses], ax        ; [bp-0x8], ax
MOV       [bp-cDefenses+0x2], dx    ; [bp-0x6], dx

L_03bf:                             ; util.c:135
SUB       sp, 0x0008          
WAIT                          
FILD      [bp-cDefenses]            ; [bp-0x8]
NOP                           
WAIT                          
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
SUB       sp, 0x0008          
LES       bx, [bp-part+0x4]         ; bx, [bp-0x10]
MOV       ax, es:[bx+0x34]    
CWD       dx, ax              
MOV       [bp-0x1e], ax       
MOV       [bp-0x1c], dx       
WAIT                          
FILD      [bp-0x1e]           
WAIT                          
FLD       [0x1ca2]            
WAIT                          
FDIVP     st(1), st           
WAIT                          
FLD       [0x1caa]            
WAIT                          
FXCH      st(1)               
WAIT                          
FSUBP     st(1), st           
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
CALLF     pow                       ; double pow(double x, double y)
ADD       sp, 0x0010          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
WAIT                          
FSTP      [bp-pct]                  ; [bp-0xc]
NOP                           
WAIT                          
                                    ; util.c:136
CMP       [bp+ppctSmart], 0x0000    ; [bp+0xc], 0x0000
JZ        L_047e              

L_041a:                             ; util.c:137
SUB       sp, 0x0008          
WAIT                          
FILD      [bp-cDefenses]            ; [bp-0x8]
NOP                           
WAIT                          
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
SUB       sp, 0x0008          
LES       bx, [bp-part+0x4]         ; bx, [bp-0x10]
MOV       ax, es:[bx+0x34]    
CWD       dx, ax              
MOV       [bp-0x1e], ax       
MOV       [bp-0x1c], dx       
WAIT                          
FILD      [bp-0x1e]           
WAIT                          
FLD       [0x1cb2]            
WAIT                          
FDIVP     st(1), st           
WAIT                          
FLD       [0x1caa]            
WAIT                          
FXCH      st(1)               
WAIT                          
FSUBP     st(1), st           
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
CALLF     pow                       ; double pow(double x, double y)
ADD       sp, 0x0010          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
NOP                           
WAIT                          
MOV       bx, [bp+ppctSmart]        ; bx, [bp+0xc]
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          

L_0470:                             ; util.c:139
JMP       L_047e              

L_0473:                             ; util.c:140
WAIT                          
FLD       [0x1c9e]            
WAIT                          
FSTP      [bp-pct]                  ; [bp-0xc]
NOP                           
WAIT                          

L_047e:                             ; util.c:142
MOV       ax, [bp-iPlrSav]          ; ax, [bp-0x4]
MOV       [idPlayer], ax            ; [0x018c], ax
                                    ; util.c:144
JMP       L_0492              

L_0487:                             ; util.c:145
WAIT                          
FLD       [0x1c9e]            
WAIT                          
FSTP      [bp-pct]                  ; [bp-0xc]
NOP                           
WAIT                          

L_0492:                             ; util.c:147
WAIT                          
FLD       [bp-pct]                  ; [bp-0xc]
NOP                           
WAIT                          
MOV       bx, [bp+ppct]             ; bx, [bp+0xa]
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
                                    ; util.c:148
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


