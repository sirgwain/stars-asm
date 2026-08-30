; FGetBestDefensePart  (planet)
;   addr: 000a:21f6  len=214
;   sig:  int16_t FGetBestDefensePart(PART *ppart)
;   params:
;     PART *           ppart          [BP+0x6]
;   locals:
;     PART             part           [BP-0xe]
;     int16_t          i              [BP-0x6]
;     int16_t          fRet           [BP-0x4]
;
;   stats: blocks=0  labels=0

L_21f6:                             ; planet.c:856
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0012          
PUSH      si                  
PUSH      di                  
                                    ; planet.c:859
MOV       [bp-fRet], 0x0001         ; [bp-0x4], 0x0001
                                    ; planet.c:861
MOV       [bp-part], 0x8000         ; [bp-0xe], 0x8000
                                    ; planet.c:862
MOV       ax, [bp-part+0x2]         ; ax, [bp-0xc]
AND       ax, 0xff00          
OR        ax, 0x0009          
MOV       [bp-part+0x2], ax         ; [bp-0xc], ax
                                    ; planet.c:863
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_2250              

L_221d:
MOV       ax, [bp-0x6]        
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001
MOV       ax, [bp-part+0x2]         ; ax, [bp-0xc]
AND       ax, 0x00ff          
MOV       [bp-0x10], ax       
MOV       ax, [bp-part+0x2]         ; ax, [bp-0xc]
ADD       ax, 0x0001          
AND       ax, 0x00ff          
MOV       [bp-0x12], ax       
AND       [bp-part+0x2], 0xff00     ; [bp-0xc], 0xff00
MOV       ax, [bp-0xc]        
MOV       ax, [bp-0x12]       
OR        [bp-part+0x2], ax         ; [bp-0xc], ax
MOV       ax, [bp-0xc]        
MOV       ax, [bp-0x12]       
MOV       ax, [bp-0x10]       

L_2250:
CMP       [bp-i], 0x0005            ; [bp-0x6], 0x0005
JGE       L_2273              

L_2259:                             ; planet.c:864
LEA       ax, [bp-part]             ; ax, [bp-0xe]
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0001          
JNZ       L_2273              

L_226a:
JMP       L_221d              

L_226d:                             ; planet.c:865
JMP       L_2273              

L_2270:                             ; planet.c:867
JMP       L_221d              

L_2273:
CMP       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JLE       L_2283              

L_227c:                             ; planet.c:868
SUB       [bp-i], 0x0001            ; [bp-0x6], 0x0001
                                    ; planet.c:869
JMP       L_2288              

L_2283:                             ; planet.c:870
MOV       [bp-fRet], 0x0000         ; [bp-0x4], 0x0000

L_2288:                             ; planet.c:872
MOV       ax, [bp-i]                ; ax, [bp-0x6]
ADD       ax, 0x0009          
MOV       [bp-0x10], ax       
MOV       ax, [bp-0x10]       
AND       ax, 0x00ff          
MOV       cx, [bp-part+0x2]         ; cx, [bp-0xc]
AND       cx, 0xff00          
OR        cx, ax              
MOV       [bp-part+0x2], cx         ; [bp-0xc], cx
MOV       ax, cx              
                                    ; planet.c:873
LEA       ax, [bp-part]             ; ax, [bp-0xe]
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
                                    ; planet.c:874
LEA       si, [bp-part]             ; si, [bp-0xe]
MOV       di, [bp+ppart]            ; di, [bp+0x6]
PUSH      ds                  
POP       es                  
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
MOV       ax, [bp+0x6]        
                                    ; planet.c:876
MOV       ax, [bp-fRet]             ; ax, [bp-0x4]

L_22c6:                             ; planet.c:877
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


