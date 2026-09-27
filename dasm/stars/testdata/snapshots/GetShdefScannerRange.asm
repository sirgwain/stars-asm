; GetShdefScannerRange  (util)
;   addr: 0008:50d0  len=1512
;   sig:  int16_t GetShdefScannerRange(SHDEF *lpshdef, int16_t iplr, int16_t *pdPlanRange, int16_t *ppctDetect, int16_t *piSteal)
;   params:
;     SHDEF *          lpshdef        [BP+0x6]
;     int16_t          iplr           [BP+0xa]
;     int16_t *        pdPlanRange    [BP+0xc]
;     int16_t *        ppctDetect     [BP+0xe]
;     int16_t *        piSteal        [BP+0x10]
;   locals:
;     double           lRange4        [BP-0x42]
;     double           lBIPR4         [BP-0x3a]
;     int16_t          j              [BP-0x32]
;     int16_t          iSteal         [BP-0x30]
;     double           lT             [BP-0x2e]
;     int16_t          dRange         [BP-0x26]
;     double           lPlanRange4    [BP-0x24]
;     int16_t          cDetectors     [BP-0x1c]
;     int16_t          fBuiltIn       [BP-0x1a]
;     int16_t          iScanner       [BP-0x18]
;     int16_t          fHasScanner    [BP-0x16]
;     int16_t          dRangeT        [BP-0x14]
;     double           lBIR4          [BP-0x12]
;     int16_t          dRangeT2       [BP-0xa]
;     HS *             lphs           [BP-0x8]
;     int16_t          chs            [BP-0x4]
;
;   stats: blocks=0  labels=2
;     LPlanScan: L_53ed
;     LOddBallScanners: L_5482

L_50d0:                             ; util.c:2095
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x004a          
PUSH      si                  
PUSH      di                  
                                    ; util.c:2101
MOV       ax, 0x0000          
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lRange4]              ; [bp-0x42]
NOP                           
WAIT                          
                                    ; util.c:2102
MOV       ax, 0x0000          
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lPlanRange4]          ; [bp-0x24]
NOP                           
WAIT                          
                                    ; util.c:2104
MOV       [bp-fHasScanner], 0x0000  ; [bp-0x16], 0x0000
                                    ; util.c:2105
MOV       [bp-iSteal], 0x0000       ; [bp-0x30], 0x0000
                                    ; util.c:2106
MOV       [bp-cDetectors], 0x0000   ; [bp-0x1c], 0x0000
                                    ; util.c:2108
CMP       [bp+iplr], 0xffff         ; [bp+0xa], 0xffff
JZ        L_513f              

L_5119:
MOV       ax, 0x000e          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
CMP       ax, 0x0009          
JNZ       L_513f              

L_5139:
MOV       ax, 0x0001          
JMP       L_5142              

L_513f:
MOV       ax, 0x0000          

L_5142:
MOV       [bp-fBuiltIn], ax         ; [bp-0x1a], ax
                                    ; util.c:2109
WAIT                          
FLD       [0x1cca]            
WAIT                          
FSTP      [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          
                                    ; util.c:2110
WAIT                          
FLD       [0x1cca]            
WAIT                          
FSTP      [bp-lBIPR4]               ; [bp-0x3a]
NOP                           
WAIT                          
                                    ; util.c:2114
CMP       [bp+ppctDetect], 0x0000   ; [bp+0xe], 0x0000
JZ        L_516b              

L_5164:                             ; util.c:2115
MOV       bx, [bp+ppctDetect]       ; bx, [bp+0xe]
MOV       [bx], 0x0064        

L_516b:                             ; util.c:2120
CMP       [bp-fBuiltIn], 0x0000     ; [bp-0x1a], 0x0000
JZ        L_5272              

L_5174:
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
CMP       es:[bx], 0x0004     
JZ        L_5198              

L_5180:
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
CMP       es:[bx], 0x0006     
JZ        L_5198              

L_518c:
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
CMP       es:[bx], 0x0005     
JNZ       L_5272              

L_5198:                             ; util.c:2122
MOV       ax, 0x0000          
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          
CALLF     __aFfcompp                ; void __aFfcompp()
JNC       L_525e              

L_51b6:                             ; util.c:2124
MOV       ax, [game+0x10]           ; ax, [0x0080]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_51e3              

L_51ca:                             ; util.c:2126
WAIT                          
FLD       [0x1cd2]            
WAIT                          
FSTP      [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          
                                    ; util.c:2127
WAIT                          
FLD       [0x1cda]            
WAIT                          
FSTP      [bp-lBIPR4]               ; [bp-0x3a]
NOP                           
WAIT                          
                                    ; util.c:2129
JMP       L_525e              

L_51e3:                             ; util.c:2131
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       al, [bx+0x1e]       
CBW       ax, al              
MOV       cx, 0x000a          
IMUL      cx                  
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lBIPR4]               ; [bp-0x3a]
NOP                           
WAIT                          
                                    ; util.c:2132
MOV       ax, 0x0002          
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FXCH      st(1)               
WAIT                          
FMULP     st(1), st           
WAIT                          
FSTP      [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          
                                    ; util.c:2134
WAIT                          
FLD       [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FMUL      [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FSTP      [bp-lBIPR4]               ; [bp-0x3a]
NOP                           
WAIT                          
                                    ; util.c:2135
WAIT                          
FLD       [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FMUL      [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FSTP      [bp-lBIPR4]               ; [bp-0x3a]
NOP                           
WAIT                          
                                    ; util.c:2137
WAIT                          
FLD       [bp-lBIR4]                ; [bp-0x12]
WAIT                          
FMUL      [bp-lBIR4]                ; [bp-0x12]
WAIT                          
FSTP      [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          
                                    ; util.c:2138
WAIT                          
FLD       [bp-lBIR4]                ; [bp-0x12]
WAIT                          
FMUL      [bp-lBIR4]                ; [bp-0x12]
WAIT                          
FSTP      [bp-lBIR4]                ; [bp-0x12]
NOP                           
WAIT                          

L_525e:                             ; util.c:2142
WAIT                          
FLD       [bp-lBIR4]                ; [bp-0x12]
WAIT                          
FSTP      [bp-lRange4]              ; [bp-0x42]
NOP                           
WAIT                          
                                    ; util.c:2143
WAIT                          
FLD       [bp-lBIPR4]               ; [bp-0x3a]
WAIT                          
FSTP      [bp-lPlanRange4]          ; [bp-0x24]
NOP                           
WAIT                          

L_5272:                             ; util.c:2146
MOV       ax, 0x003a          
MOV       cx, [bp+lpshdef]          ; cx, [bp+0x6]
MOV       dx, [bp+lpshdef+0x2]      ; dx, [bp+0x8]
ADD       cx, ax              
MOV       [bp-lphs], cx             ; [bp-0x8], cx
MOV       [bp-lphs+0x2], dx         ; [bp-0x6], dx
                                    ; util.c:2147
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       al, es:[bx+0x7a]    
AND       ax, 0x00ff          
MOV       [bp-chs], ax              ; [bp-0x4], ax
                                    ; util.c:2149
MOV       [bp-j], 0x0000            ; [bp-0x32], 0x0000
JMP       L_52ab              

L_5298:
MOV       ax, [bp-0x32]       
ADD       [bp-j], 0x0001            ; [bp-0x32], 0x0001
MOV       ax, [bp-0x8]        
MOV       es, [bp-0x6]        
ADD       [bp-lphs], 0x0004         ; [bp-0x8], 0x0004
MOV       dx, es              

L_52ab:
MOV       ax, [bp-chs]              ; ax, [bp-0x4]
CMP       [bp-j], ax                ; [bp-0x32], ax
JGE       L_5591              

L_52b6:                             ; util.c:2150
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JZ        L_5298              

L_52cd:                             ; util.c:2152
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0002     
JNZ       L_545a              

L_52d9:                             ; util.c:2154
MOV       [bp-fHasScanner], 0x0001  ; [bp-0x16], 0x0001
                                    ; util.c:2156
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
MOV       [bp-iScanner], ax         ; [bp-0x18], ax
                                    ; util.c:2157
PUSH      ax                  
CALLF     LpscannerFromId           ; SCANNER * LpscannerFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x34]    
MOV       [bp-dRangeT], ax          ; [bp-0x14], ax
                                    ; util.c:2159
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2160
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2161
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2162
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       dx, 0x0000          
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FXCH      st(1)               
WAIT                          
FMULP     st(1), st           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2164
WAIT                          
FLD       [bp-lRange4]              ; [bp-0x42]
WAIT                          
FADD      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lRange4]              ; [bp-0x42]
NOP                           
WAIT                          
                                    ; util.c:2166
PUSH      [bp-iScanner]             ; [bp-0x18]
CALLF     LpscannerFromId           ; SCANNER * LpscannerFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x36]    
MOV       [bp-dRangeT], ax          ; [bp-0x14], ax
                                    ; util.c:2167
CMP       [bp-iScanner], 0x0006     ; [bp-0x18], 0x0006
JNZ       L_5390              

L_5385:                             ; util.c:2169
MOV       [bp-dRangeT], 0x002d      ; [bp-0x14], 0x002d
                                    ; util.c:2170
JMP       LPlanScan           

L_5390:
CMP       [bp-iScanner], 0x0005     ; [bp-0x18], 0x0005
JNZ       L_53a8              

L_5399:                             ; util.c:2174
MOV       [bp-dRangeT], 0x0000      ; [bp-0x14], 0x0000
                                    ; util.c:2175
OR        [bp-iSteal], 0x0001       ; [bp-0x30], 0x0001
                                    ; util.c:2176
JMP       LPlanScan           

L_53a8:
CMP       [bp-iScanner], 0x000e     ; [bp-0x18], 0x000e
JNZ       L_53c0              

L_53b1:                             ; util.c:2180
MOV       [bp-dRangeT], 0x0078      ; [bp-0x14], 0x0078
                                    ; util.c:2181
OR        [bp-iSteal], 0x0003       ; [bp-0x30], 0x0003
                                    ; util.c:2182
JMP       LPlanScan           

L_53c0:
CMP       [bp-dRangeT], 0x0000      ; [bp-0x14], 0x0000
JLE       L_5298              

L_53c9:                             ; util.c:2187
CMP       [bp-dRangeT], 0x0001      ; [bp-0x14], 0x0001
JNZ       L_53d8              

L_53d2:
MOV       ax, 0x0032          
JMP       L_53ea              

L_53d8:
CMP       [bp-dRangeT], 0x0002      ; [bp-0x14], 0x0002
JNZ       L_53e7              

L_53e1:
MOV       ax, 0x0064          
JMP       L_53ea              

L_53e7:
MOV       ax, 0x00c8          

L_53ea:
MOV       [bp-dRangeT], ax          ; [bp-0x14], ax

LPlanScan:                          ; util.c:2189
MOV       ax, [bp-dRangeT]          ; ax, [bp-0x14]
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2190
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2191
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2192
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       dx, 0x0000          
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FXCH      st(1)               
WAIT                          
FMULP     st(1), st           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2194
WAIT                          
FLD       [bp-lPlanRange4]          ; [bp-0x24]
WAIT                          
FADD      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lPlanRange4]          ; [bp-0x24]
NOP                           
WAIT                          

L_5457:                             ; util.c:2197
JMP       L_5298              

L_545a:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0008     
JNZ       L_54f8              

L_5466:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JNZ       L_54f8              

L_5478:                             ; util.c:2199
MOV       [bp-dRangeT], 0x0050      ; [bp-0x14], 0x0050
                                    ; util.c:2200
MOV       [bp-dRangeT2], 0x0028     ; [bp-0xa], 0x0028

LOddBallScanners:                   ; util.c:2202
MOV       ax, [bp-dRangeT]          ; ax, [bp-0x14]
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2203
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2204
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FMUL      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2205
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       dx, 0x0000          
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lT]                   ; [bp-0x2e]
WAIT                          
FXCH      st(1)               
WAIT                          
FMULP     st(1), st           
WAIT                          
FSTP      [bp-lT]                   ; [bp-0x2e]
NOP                           
WAIT                          
                                    ; util.c:2207
WAIT                          
FLD       [bp-lRange4]              ; [bp-0x42]
WAIT                          
FADD      [bp-lT]                   ; [bp-0x2e]
WAIT                          
FSTP      [bp-lRange4]              ; [bp-0x42]
NOP                           
WAIT                          
                                    ; util.c:2209
MOV       ax, [bp-dRangeT2]         ; ax, [bp-0xa]
MOV       [bp-dRangeT], ax          ; [bp-0x14], ax
                                    ; util.c:2210
JMP       LPlanScan           

L_54f8:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0010     
JNZ       L_5526              

L_5504:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0012          
JNZ       L_5526              

L_5516:                             ; util.c:2214
MOV       [bp-dRangeT], 0x0096      ; [bp-0x14], 0x0096
                                    ; util.c:2215
MOV       [bp-dRangeT2], 0x004b     ; [bp-0xa], 0x004b
                                    ; util.c:2216
JMP       LOddBallScanners    

L_5526:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0004     
JNZ       L_5554              

L_5532:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JNZ       L_5554              

L_5544:                             ; util.c:2220
MOV       [bp-dRangeT], 0x0032      ; [bp-0x14], 0x0032
                                    ; util.c:2221
MOV       [bp-dRangeT2], 0x0019     ; [bp-0xa], 0x0019
                                    ; util.c:2222
JMP       LOddBallScanners    

L_5554:
CMP       [bp+ppctDetect], 0x0000   ; [bp+0xe], 0x0000
JZ        L_5298              

L_555d:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0800     
JNZ       L_5298              

L_556a:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JNZ       L_5298              

L_557c:                             ; util.c:2225
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
ADD       [bp-cDetectors], ax       ; [bp-0x1c], ax

L_558e:                             ; util.c:2228
JMP       L_5298              

L_5591:
MOV       ax, 0x0000          
CWD       dx, ax              
MOV       [bp-0x4a], ax       
MOV       [bp-0x48], dx       
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-lRange4]              ; [bp-0x42]
NOP                           
WAIT                          
CALLF     __aFfcompp                ; void __aFfcompp()
JA        L_55b8              

L_55af:
CMP       [bp-fHasScanner], 0x0000  ; [bp-0x16], 0x0000
JZ        L_5628              

L_55b8:                             ; util.c:2230
SUB       sp, 0x0008          
PUSH      [bp-lRange4+0x6]          ; [bp-0x3c]
PUSH      [bp-lRange4+0x4]          ; [bp-0x3e]
PUSH      [bp-lRange4+0x2]          ; [bp-0x40]
PUSH      [bp-lRange4]              ; [bp-0x42]
CALLF     sqrt                      ; double sqrt(double x)
ADD       sp, 0x0008          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
NOP                           
WAIT                          
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
CALLF     sqrt                      ; double sqrt(double x)
ADD       sp, 0x0008          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
NOP                           
WAIT                          
CALLF     __ftol                    ; int32_t __ftol()
MOV       [bp-dRange], ax           ; [bp-0x26], ax
                                    ; util.c:2232
CMP       [bp+iplr], 0xffff         ; [bp+0xa], 0xffff
JZ        L_562d              

L_55fd:
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_562d              

L_561d:                             ; util.c:2233
MOV       ax, [bp-dRange]           ; ax, [bp-0x26]
SHL       ax, 0x0001          
MOV       [bp-dRange], ax           ; [bp-0x26], ax

L_5625:                             ; util.c:2235
JMP       L_562d              

L_5628:                             ; util.c:2236
MOV       [bp-dRange], 0xffff       ; [bp-0x26], 0xffff

L_562d:                             ; util.c:2238
CMP       [bp+pdPlanRange], 0x0000  ; [bp+0xc], 0x0000
JZ        L_5674              

L_5636:                             ; util.c:2239
SUB       sp, 0x0008          
PUSH      [bp-lPlanRange4+0x6]      ; [bp-0x1e]
PUSH      [bp-lPlanRange4+0x4]      ; [bp-0x20]
PUSH      [bp-lPlanRange4+0x2]      ; [bp-0x22]
PUSH      [bp-lPlanRange4]          ; [bp-0x24]
CALLF     sqrt                      ; double sqrt(double x)
ADD       sp, 0x0008          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
NOP                           
WAIT                          
MOV       bx, sp              
WAIT                          
FSTP      [bx]                
NOP                           
WAIT                          
CALLF     sqrt                      ; double sqrt(double x)
ADD       sp, 0x0008          
MOV       bx, ax              
WAIT                          
FLD       [bx]                
NOP                           
WAIT                          
CALLF     __ftol                    ; int32_t __ftol()
MOV       bx, [bp+pdPlanRange]      ; bx, [bp+0xc]
MOV       [bx], ax            

L_5674:                             ; util.c:2241
CMP       [bp+piSteal], 0x0000      ; [bp+0x10], 0x0000
JZ        L_5685              

L_567d:                             ; util.c:2242
MOV       ax, [bp-iSteal]           ; ax, [bp-0x30]
MOV       bx, [bp+piSteal]          ; bx, [bp+0x10]
MOV       [bx], ax            

L_5685:                             ; util.c:2244
CMP       [bp+ppctDetect], 0x0000   ; [bp+0xe], 0x0000
JZ        L_56ac              

L_568e:                             ; util.c:2246
CMP       [bp-cDetectors], 0x0012   ; [bp-0x1c], 0x0012
JL        L_569c              

L_5697:                             ; util.c:2247
MOV       [bp-cDetectors], 0x0011   ; [bp-0x1c], 0x0011

L_569c:                             ; util.c:2248
MOV       bx, [bp-cDetectors]       ; bx, [bp-0x1c]
MOV       al, cs:[bx+0x50be]  
AND       ax, 0x00ff          
MOV       bx, [bp+ppctDetect]       ; bx, [bp+0xe]
MOV       [bx], ax            

L_56ac:                             ; util.c:2251
MOV       ax, [bp-dRange]           ; ax, [bp-0x26]

L_56b2:                             ; util.c:2252
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


