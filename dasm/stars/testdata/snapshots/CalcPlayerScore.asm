; CalcPlayerScore  (util)
;   addr: 0008:58a6  len=1299
;   sig:  int32_t CalcPlayerScore(int16_t iPlr, SCORE *pscore)
;   params:
;     int16_t          iPlr           [BP+0x6]
;     SCORE *          pscore         [BP+0x8]
;   locals:
;     int16_t[16]      rgType         [BP-0x5c]
;     int32_t          lPower         [BP-0x3c]
;     int16_t          iTech          [BP-0x38]
;     FLEET *          lpfl           [BP-0x36]
;     int16_t          ifl            [BP-0x32]
;     int16_t          i              [BP-0x30]
;     PLANET *         lppl           [BP-0x2e]
;     PLANET *         lpplMac        [BP-0x2a]
;     SCORE            score          [BP-0x26]
;     int32_t          lTemp          [BP-0x12]
;     int32_t[3]       rgcsh          [BP-0xe]
;
;   stats: blocks=0  labels=0

L_58a6:                             ; util.c:2308
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0060          
PUSH      si                  
PUSH      di                  
                                    ; util.c:2321
MOV       ax, 0x0014          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-score]            ; ax, [bp-0x26]
PUSH      ax                  
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; util.c:2324
MOV       ax, [lpPlanets]           ; ax, [0x00f6]
MOV       dx, [lpPlanets+0x2]       ; dx, [0x00f8]
MOV       [bp-lppl], ax             ; [bp-0x2e], ax
MOV       [bp-lppl+0x2], dx         ; [bp-0x2c], dx
MOV       [bp-0x60], ax       
MOV       [bp-0x5e], dx       
MOV       ax, 0x0038          
IMUL      [cPlanet]                 ; [0x4b7c]
MOV       cx, [bp-0x60]       
MOV       dx, [bp-0x5e]       
ADD       cx, ax              
MOV       [bp-lpplMac], cx          ; [bp-0x2a], cx
MOV       [bp-lpplMac+0x2], dx      ; [bp-0x28], dx
JMP       L_59c9              

L_58ee:                             ; util.c:2326
LES       bx, [bp-lppl]             ; bx, [bp-0x2e]
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
CMP       es:[bx+0x2], ax     
JNZ       L_59c5              

L_58fa:
JMP       L_5900              

L_58fd:                             ; util.c:2327
JMP       L_59c5              

L_5900:                             ; util.c:2328
ADD       [bp-score+0x8], 0x0001    ; [bp-0x1e], 0x0001
                                    ; util.c:2330
MOV       ax, 0x03e8          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x2e]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
ADD       ax, 0x03e7          
ADC       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-lTemp], ax            ; [bp-0x12], ax
MOV       [bp-lTemp+0x2], dx        ; [bp-0x10], dx
                                    ; util.c:2331
CMP       [bp-lTemp+0x2], 0x0000    ; [bp-0x10], 0x0000
JL        L_594b              

L_5933:
JG        L_5941              

L_5938:
CMP       [bp-lTemp], 0x0006        ; [bp-0x12], 0x0006
JBE       L_594b              

L_5941:                             ; util.c:2332
MOV       [bp-lTemp], 0x0006        ; [bp-0x12], 0x0006
MOV       [bp-lTemp+0x2], 0x0000    ; [bp-0x10], 0x0000

L_594b:                             ; util.c:2333
MOV       ax, [bp-lTemp]            ; ax, [bp-0x12]
MOV       dx, [bp-lTemp+0x2]        ; dx, [bp-0x10]
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2336
LES       bx, [bp-lppl]             ; bx, [bp-0x2e]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_59ad              

L_596e:
LES       bx, [bp-lppl]             ; bx, [bp-0x2e]
MOV       ax, es:[bx+0x2c]    
AND       ax, 0x000f          
MOV       cx, 0x0093          
IMUL      cx                  
MOV       bx, [bp+iPlr]             ; bx, [bp+0x6]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
PUSH      es:[bx]             
CALLF     LphuldefFromId            ; HULDEF * LphuldefFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
CMP       es:[bx+0x34], 0x0000
JZ        L_59ad              

L_59a9:                             ; util.c:2337
ADD       [bp-score+0xa], 0x0001    ; [bp-0x1c], 0x0001

L_59ad:                             ; util.c:2338
PUSH      [bp+iPlr]                 ; [bp+0x6]
PUSH      [bp-lppl+0x2]             ; [bp-0x2c]
PUSH      [bp-lppl]                 ; [bp-0x2e]
CALLF     CResourcesAtPlanet        ; int16_t CResourcesAtPlanet(PLANET *lppl, int16_t iplr)
ADD       sp, 0x0006          
CWD       dx, ax              
ADD       [bp-score+0x4], ax        ; [bp-0x22], ax
ADC       [bp-score+0x6], dx        ; [bp-0x20], dx

L_59c5:                             ; util.c:2339
ADD       [bp-lppl], 0x0038         ; [bp-0x2e], 0x0038

L_59c9:
MOV       ax, [bp-lpplMac]          ; ax, [bp-0x2a]
MOV       dx, [bp-0x28]       
CMP       [bp-lppl], ax             ; [bp-0x2e], ax
JC        L_58ee              

L_59d7:                             ; util.c:2341
MOV       ax, 0x001e          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-score+0x6]            ; [bp-0x20]
PUSH      [bp-score+0x4]            ; [bp-0x22]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2342
MOV       ax, 0x0003          
IMUL      [bp-score+0xa]            ; [bp-0x1c]
CWD       dx, ax              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2345
MOV       ax, 0x00c0          
IMUL      [bp+iPlr]                 ; [bp+0x6]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, [bx+0x54]       
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_5aa7              

L_5a16:                             ; util.c:2347
MOV       [bp-i], 0x0000            ; [bp-0x30], 0x0000
JMP       L_5a9e              

L_5a1e:                             ; util.c:2349
MOV       ax, 0x00c0          
IMUL      [bp+iPlr]                 ; [bp+0x6]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, 0x001a          
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x30]
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
MOV       [bp-iTech], ax            ; [bp-0x38], ax
                                    ; util.c:2350
ADD       [bp-score+0x12], ax       ; [bp-0x14], ax
                                    ; util.c:2352
CMP       [bp-iTech], 0x0004        ; [bp-0x38], 0x0004
JGE       L_5a52              

L_5a45:                             ; util.c:2353
MOV       ax, [bp-iTech]            ; ax, [bp-0x38]
CWD       dx, ax              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2354
JMP       L_5a9a              

L_5a52:
CMP       [bp-iTech], 0x0007        ; [bp-0x38], 0x0007
JGE       L_5a6d              

L_5a5b:                             ; util.c:2355
MOV       ax, [bp-iTech]            ; ax, [bp-0x38]
SHL       ax, 0x0001          
ADD       ax, 0xfffd          
CWD       dx, ax              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2356
JMP       L_5a9a              

L_5a6d:
CMP       [bp-iTech], 0x000a        ; [bp-0x38], 0x000a
JGE       L_5a89              

L_5a76:                             ; util.c:2357
MOV       ax, 0x0003          
IMUL      [bp-iTech]                ; [bp-0x38]
ADD       ax, 0xfff7          
CWD       dx, ax              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2358
JMP       L_5a9a              

L_5a89:                             ; util.c:2359
MOV       ax, [bp-iTech]            ; ax, [bp-0x38]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       ax, 0xffee          
CWD       dx, ax              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx

L_5a9a:                             ; util.c:2360
ADD       [bp-i], 0x0001            ; [bp-0x30], 0x0001

L_5a9e:
CMP       [bp-i], 0x0006            ; [bp-0x30], 0x0006
JL        L_5a1e              

L_5aa7:                             ; util.c:2364
MOV       [bp-i], 0x0000            ; [bp-0x30], 0x0000
JMP       L_5b77              

L_5aaf:                             ; util.c:2366
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x30]
MOV       bx, [bp+iPlr]             ; bx, [bp+0x6]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x100]      
MOV       bx, [bx+0xfe]       
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x7b]    
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_5aed              

L_5adc:                             ; util.c:2368
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
LEA       bx, [bp-0x5c]       
ADD       bx, ax              
MOV       [bx], 0xffff        
                                    ; util.c:2369
JMP       L_5b73              

L_5aed:                             ; util.c:2372
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x30]
MOV       bx, [bp+iPlr]             ; bx, [bp+0x6]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     LComputePower             ; int32_t LComputePower(SHDEF *lpshdef)
ADD       sp, 0x0004          
MOV       [bp-lPower], ax           ; [bp-0x3c], ax
MOV       [bp-lPower+0x2], dx       ; [bp-0x3a], dx
                                    ; util.c:2374
CMP       [bp-lPower+0x2], 0x0000   ; [bp-0x3a], 0x0000
JG        L_5b3c              

L_5b1d:
JL        L_5b2b              

L_5b22:
CMP       [bp-lPower], 0x0000       ; [bp-0x3c], 0x0000
JA        L_5b3c              

L_5b2b:                             ; util.c:2375
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
LEA       bx, [bp-0x5c]       
ADD       bx, ax              
MOV       [bx], 0x0000        
                                    ; util.c:2376
JMP       L_5b73              

L_5b3c:
CMP       [bp-lPower+0x2], 0x0000   ; [bp-0x3a], 0x0000
JG        L_5b65              

L_5b45:
JL        L_5b54              

L_5b4a:
CMP       [bp-lPower], 0x07d0       ; [bp-0x3c], 0x07d0
JNC       L_5b65              

L_5b54:                             ; util.c:2377
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
LEA       bx, [bp-0x5c]       
ADD       bx, ax              
MOV       [bx], 0x0001        
                                    ; util.c:2378
JMP       L_5b73              

L_5b65:                             ; util.c:2379
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
LEA       bx, [bp-0x5c]       
ADD       bx, ax              
MOV       [bx], 0x0002        

L_5b73:                             ; util.c:2380
ADD       [bp-i], 0x0001            ; [bp-0x30], 0x0001

L_5b77:
CMP       [bp-i], 0x0010            ; [bp-0x30], 0x0010
JL        L_5aaf              

L_5b80:                             ; util.c:2382
MOV       [bp-i], 0x0000            ; [bp-0x30], 0x0000
JMP       L_5ba1              

L_5b88:                             ; util.c:2383
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0xe]        
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
ADD       [bp-i], 0x0001            ; [bp-0x30], 0x0001

L_5ba1:
CMP       [bp-i], 0x0003            ; [bp-0x30], 0x0003
JL        L_5b88              

L_5baa:                             ; util.c:2385
MOV       [bp-ifl], 0x0000          ; [bp-0x32], 0x0000
JMP       L_5bb6              

L_5bb2:
ADD       [bp-ifl], 0x0001          ; [bp-0x32], 0x0001

L_5bb6:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-ifl], ax              ; [bp-0x32], ax
JGE       L_5c93              

L_5bc1:
MOV       ax, [bp-ifl]              ; ax, [bp-0x32]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0x36], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x34], dx
CMP       ax, 0x0000          
JNZ       L_5bf1              

L_5be9:
CMP       dx, 0x0000          
JZ        L_5c93              

L_5bf1:                             ; util.c:2387
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
CMP       es:[bx+0x2], ax     
JNZ       L_5bb2              

L_5c00:
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x000a          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_5bb2              

L_5c14:
JMP       L_5c1a              

L_5c17:                             ; util.c:2388
JMP       L_5bb2              

L_5c1a:                             ; util.c:2389
MOV       [bp-i], 0x0000            ; [bp-0x30], 0x0000
JMP       L_5c87              

L_5c22:                             ; util.c:2391
MOV       ax, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x34]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JLE       L_5c83              

L_5c3f:
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
LEA       bx, [bp-0x5c]       
ADD       bx, ax              
CMP       [bx], 0xffff        
JZ        L_5c83              

L_5c51:                             ; util.c:2392
MOV       ax, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x34]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
CWD       dx, ax              
MOV       bx, [bp-i]                ; bx, [bp-0x30]
SHL       bx, 0x0001          
LEA       si, [bp-0x5c]       
ADD       si, bx              
MOV       bx, [si]            
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0xe]        
ADD       si, bx              
ADD       [si], ax            
ADC       [si+0x2], dx        

L_5c83:                             ; util.c:2393
ADD       [bp-i], 0x0001            ; [bp-0x30], 0x0001

L_5c87:
CMP       [bp-i], 0x0010            ; [bp-0x30], 0x0010
JGE       L_5bb2              

L_5c8d:
JMP       L_5c22              

L_5c90:                             ; util.c:2394
JMP       L_5bb2              

L_5c93:                             ; util.c:2396
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              
CMP       [bp-rgcsh+0x6], dx        ; [bp-0x8], dx
JG        L_5cb5              

L_5c9f:
JL        L_5cac              

L_5ca4:
CMP       [bp-rgcsh+0x4], ax        ; [bp-0xa], ax
JNC       L_5cb5              

L_5cac:
MOV       ax, [bp-rgcsh+0x4]        ; ax, [bp-0xa]
MOV       dx, [bp-rgcsh+0x6]        ; dx, [bp-0x8]
JMP       L_5cb9              

L_5cb5:
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              

L_5cb9:
MOV       cx, 0x0001          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       cx, 0x0002          
MOV       bx, 0x0000          
PUSH      bx                  
PUSH      cx                  
MOV       [bp-0x60], ax       
MOV       [bp-0x5e], dx       
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              
CMP       [bp-rgcsh+0x2], dx        ; [bp-0xc], dx
JG        L_5cf1              

L_5cdb:
JL        L_5ce8              

L_5ce0:
CMP       [bp-rgcsh], ax            ; [bp-0xe], ax
JNC       L_5cf1              

L_5ce8:
MOV       ax, [bp-rgcsh]            ; ax, [bp-0xe]
MOV       dx, [bp-rgcsh+0x2]        ; dx, [bp-0xc]
JMP       L_5cf5              

L_5cf1:
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              

L_5cf5:
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       cx, [bp-0x60]       
MOV       bx, [bp-0x5e]       
ADD       ax, cx              
ADC       dx, bx              
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx
                                    ; util.c:2398
CMP       [bp-rgcsh+0xa], 0x0000    ; [bp-0x4], 0x0000
JL        L_5d57              

L_5d15:
JG        L_5d23              

L_5d1a:
CMP       [bp-rgcsh+0x8], 0x0000    ; [bp-0x6], 0x0000
JBE       L_5d57              

L_5d23:                             ; util.c:2400
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              
ADD       ax, [bp-rgcsh+0x8]        ; ax, [bp-0x6]
ADC       dx, [bp-rgcsh+0xa]        ; dx, [bp-0x4]
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-score+0x8]        ; ax, [bp-0x1e]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       cx, 0x0003          
MOV       ax, [bp-rgcsh+0x8]        ; ax, [bp-0x6]
MOV       dx, [bp-rgcsh+0xa]        ; dx, [bp-0x4]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
ADD       [bp-score], ax            ; [bp-0x26], ax
ADC       [bp-score+0x2], dx        ; [bp-0x24], dx

L_5d57:                             ; util.c:2403
MOV       [bp-i], 0x0000            ; [bp-0x30], 0x0000
JMP       L_5d88              

L_5d5f:                             ; util.c:2404
MOV       ax, [bp-i]                ; ax, [bp-0x30]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0xe]        
ADD       bx, ax              
PUSH      [bx+0x2]            
PUSH      [bx]                
CALLF     WPackLong                 ; uint16_t WPackLong(int32_t l)
ADD       sp, 0x0004          
MOV       dx, [bp-i]                ; dx, [bp-0x30]
SHL       dx, 0x0001          
LEA       bx, [bp-0x1a]       
ADD       bx, dx              
MOV       [bx], ax            
ADD       [bp-i], 0x0001            ; [bp-0x30], 0x0001

L_5d88:
CMP       [bp-i], 0x0003            ; [bp-0x30], 0x0003
JL        L_5d5f              

L_5d91:                             ; util.c:2406
CMP       [bp+pscore], 0x0000       ; [bp+0x8], 0x0000
JZ        L_5daa              

L_5d9a:                             ; util.c:2407
LEA       si, [bp-score]            ; si, [bp-0x26]
MOV       di, [bp+pscore]           ; di, [bp+0x8]
PUSH      ds                  
POP       es                  
MOV       cx, 0x000a          
MOVSW.REP es:[di], ds:[si]    
MOV       ax, [bp+0x8]        

L_5daa:                             ; util.c:2408
MOV       ax, [bp-score]            ; ax, [bp-0x26]
MOV       dx, [bp-score+0x2]        ; dx, [bp-0x24]

L_5db3:                             ; util.c:2409
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


