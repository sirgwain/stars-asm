; CBuildProdItem  (turn2)
;   addr: 0018:0c92  len=3359
;   sig:  int16_t CBuildProdItem(PLANET *lppl, PROD *lpprod, PROD *pprodPartial, int32_t *rgRes, int16_t fAlchemy, int16_t *pmdStatus, int16_t fCalcOnly)
;   params:
;     PLANET *         lppl           [BP+0x6]
;     PROD *           lpprod         [BP+0xa]
;     PROD *           pprodPartial   [BP+0xe]
;     int32_t *        rgRes          [BP+0x10]
;     int16_t          fAlchemy       [BP+0x12]
;     int16_t *        pmdStatus      [BP+0x14]
;     int16_t          fCalcOnly      [BP+0x16]
;   locals:
;     int16_t          fMineralBlocked [BP-0x54]
;     int32_t[4]       rgCost         [BP-0x52]
;     int32_t          pct            [BP-0x42]
;     int32_t          pctTooBig      [BP-0x3e]
;     int32_t          pctInitial     [BP-0x3a]
;     int16_t          fResourceBlocked [BP-0x36]
;     int16_t          i              [BP-0x34]
;     int32_t[4]       rgCostPaid     [BP-0x32]
;     int16_t          cAlchemy       [BP-0x22]
;     int16_t          cBuilt         [BP-0x20]
;     int16_t          fAutoBuild     [BP-0x1e]
;     PROD             prod           [BP-0x1c]
;     int32_t          lAlchCost      [BP-0x18]
;     int32_t          lMinNeeded     [BP-0x14]
;     int32_t          cCanBuild      [BP-0x10]
;     uint32_t         iobjOther      [BP-0xc]
;     int16_t          cMax           [BP-0x8]
;     int32_t          pctT           [BP-0x6]
;     block 0018:1324  len=0x70
;       int32_t          AddCost        [BP-0x58]
;
;   stats: blocks=1  labels=1
;     LAlchemize: L_13e1

L_0c92:                             ; turn2.c:289
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0058          
PUSH      si                  
PUSH      di                  
                                    ; turn2.c:295
MOV       [bp-cAlchemy], 0x0000     ; [bp-0x22], 0x0000
                                    ; turn2.c:303
LES       bx, [bp+lpprod]           ; bx, [bp+0xa]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
MOV       [bp-pctInitial], ax       ; [bp-0x3a], ax
MOV       [bp-pctInitial+0x2], dx   ; [bp-0x38], dx
                                    ; turn2.c:307
LES       bx, [bp+lpprod]           ; bx, [bp+0xa]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-prod], ax             ; [bp-0x1c], ax
MOV       [bp-prod+0x2], dx         ; [bp-0x1a], dx
                                    ; turn2.c:309
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
LEA       ax, [bp-rgCost]           ; ax, [bp-0x52]
PUSH      ax                  
PUSH      [bp+lpprod+0x2]           ; [bp+0xc]
PUSH      [bp+lpprod]               ; [bp+0xa]
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     GetProductionCosts        ; void GetProductionCosts(PLANET *lppl, PROD *lpprod, uint32_t *rgCost, int16_t iplr, int16_t fOnlyOne)
ADD       sp, 0x000e          
                                    ; turn2.c:310
MOV       [bp-cBuilt], 0x0000       ; [bp-0x20], 0x0000
                                    ; turn2.c:312
MOV       cx, 0x0011          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0007          
AND       dx, 0x0000          
CMP       ax, 0x0001          
JNZ       L_0d49              

L_0d12:
CMP       dx, 0x0000          
JNZ       L_0d49              

L_0d1a:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       dx, 0x0000          
JA        L_0d49              

L_0d36:
JC        L_0d43              

L_0d3b:
CMP       ax, 0x0007          
JNC       L_0d49              

L_0d43:
MOV       ax, 0x0001          
JMP       L_0d4c              

L_0d49:
MOV       ax, 0x0000          

L_0d4c:
MOV       [bp-fAutoBuild], ax       ; [bp-0x1e], ax
                                    ; turn2.c:313
CMP       [bp-fAutoBuild], 0x0000   ; [bp-0x1e], 0x0000
JZ        L_102c              

L_0d58:                             ; turn2.c:315
MOV       [bp-cMax], 0x03e8         ; [bp-0x8], 0x03e8
                                    ; turn2.c:317
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
JMP       L_0f2c              

L_0d74:                             ; turn2.c:320
MOV       [bp-iobjOther], 0x0008    ; [bp-0xc], 0x0008
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:321
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0008          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x56], ax       
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxOperableMines         ; int16_t CMaxOperableMines(PLANET *lppl, int16_t iplr, int16_t fNextYear)
ADD       sp, 0x0008          
MOV       cx, [bp-0x56]       
SUB       ax, cx              
MOV       [bp-cMax], ax             ; [bp-0x8], ax
                                    ; turn2.c:322
JMP       L_0f9f              

L_0dbe:                             ; turn2.c:324
MOV       [bp-iobjOther], 0x0007    ; [bp-0xc], 0x0007
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:325
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x56], ax       
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxOperableFactories     ; int16_t CMaxOperableFactories(PLANET *lppl, int16_t iplr, int16_t fNextYear)
ADD       sp, 0x0008          
MOV       cx, [bp-0x56]       
SUB       ax, cx              
MOV       [bp-cMax], ax             ; [bp-0x8], ax
                                    ; turn2.c:326
JMP       L_0f9f              

L_0e08:                             ; turn2.c:328
MOV       [bp-iobjOther], 0x0009    ; [bp-0xc], 0x0009
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:329
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x56], ax       
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxOperableDefenses      ; int16_t CMaxOperableDefenses(PLANET *lppl, int16_t iplr, int16_t fNextYear)
ADD       sp, 0x0008          
MOV       cx, [bp-0x56]       
SUB       ax, cx              
MOV       [bp-cMax], ax             ; [bp-0x8], ax
                                    ; turn2.c:330
JMP       L_0f9f              

L_0e4a:                             ; turn2.c:332
MOV       [bp-iobjOther], 0x000b    ; [bp-0xc], 0x000b
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:333
JMP       L_0f9f              

L_0e57:                             ; turn2.c:337
MOV       [bp-iobjOther], 0x000c    ; [bp-0xc], 0x000c
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:338
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     IpctCanTerraformLppl      ; int16_t IpctCanTerraformLppl(PLANET *lppl)
ADD       sp, 0x0004          
MOV       [bp-cMax], ax             ; [bp-0x8], ax
                                    ; turn2.c:345
CMP       [bp-cMax], 0x0000         ; [bp-0x8], 0x0000
JLE       L_0f9f              

L_0e7b:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x0004          
JNZ       L_0f9f              

L_0e97:
CMP       dx, 0x0000          
JNZ       L_0f9f              

L_0e9f:
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     ChgPopFromPlanet          ; int32_t ChgPopFromPlanet(PLANET *lppl, int16_t fUpdate)
ADD       sp, 0x0006          
CMP       dx, 0x0000          
JL        L_0f9f              

L_0eb9:
JG        L_0ec6              

L_0ebe:
CMP       ax, 0x0000          
JC        L_0f9f              

L_0ec6:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     PctPlanetDesirability     ; int16_t PctPlanetDesirability(PLANET *lppl, int16_t iPlr)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JLE       L_0f9f              

L_0ee3:                             ; turn2.c:346
MOV       [bp-cMax], 0x0000         ; [bp-0x8], 0x0000

L_0ee8:                             ; turn2.c:348
JMP       L_0f9f              

L_0eeb:                             ; turn2.c:352
MOV       [bp-iobjOther], 0x0011    ; [bp-0xc], 0x0011
MOV       [bp-iobjOther+0x2], 0x0000; [bp-0xa], 0x0000
                                    ; turn2.c:353
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     IWarpMAFromLppl           ; int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JZ        L_0f21              

L_0f0f:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0x03ff          
CMP       ax, 0x0000          
JNZ       L_0f9f              

L_0f21:                             ; turn2.c:354
MOV       [bp-cMax], 0x0000         ; [bp-0x8], 0x0000

L_0f26:                             ; turn2.c:355
JMP       L_0f9f              

L_0f2c:
CMP       ax, 0x0000          
JNZ       L_0f3c              

L_0f34:
CMP       dx, 0x0000          
JZ        L_0d74              

L_0f3c:
CMP       ax, 0x0001          
JNZ       L_0f4c              

L_0f44:
CMP       dx, 0x0000          
JZ        L_0dbe              

L_0f4c:
CMP       ax, 0x0002          
JNZ       L_0f5c              

L_0f54:
CMP       dx, 0x0000          
JZ        L_0e08              

L_0f5c:
CMP       ax, 0x0003          
JNZ       L_0f6c              

L_0f64:
CMP       dx, 0x0000          
JZ        L_0e4a              

L_0f6c:
CMP       ax, 0x0004          
JNZ       L_0f7c              

L_0f74:
CMP       dx, 0x0000          
JZ        L_0e57              

L_0f7c:
CMP       ax, 0x0005          
JNZ       L_0f8c              

L_0f84:
CMP       dx, 0x0000          
JZ        L_0e57              

L_0f8c:
CMP       ax, 0x0006          
JNZ       L_0f9f              

L_0f94:
CMP       dx, 0x0000          
JZ        L_0eeb              

L_0f9f:                             ; turn2.c:358
CMP       [bp-cMax], 0x0000         ; [bp-0x8], 0x0000
JGE       L_0fad              

L_0fa8:                             ; turn2.c:359
MOV       [bp-cMax], 0x0000         ; [bp-0x8], 0x0000

L_0fad:                             ; turn2.c:361
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-0x1a]       
AND       ax, 0x03ff          
AND       dx, 0x0000          
MOV       [bp-0x58], ax       
MOV       [bp-0x56], dx       
MOV       ax, [bp-cMax]             ; ax, [bp-0x8]
CWD       dx, ax              
MOV       cx, [bp-0x58]       
MOV       bx, [bp-0x56]       
CMP       bx, dx              
JA        L_1000              

L_0fd0:
JC        L_0fdc              

L_0fd5:
CMP       cx, ax              
JA        L_1000              

L_0fdc:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x0003          
JNZ       L_102c              

L_0ff8:
CMP       dx, 0x0000          
JNZ       L_102c              

L_1000:                             ; turn2.c:362
MOV       ax, [bp-cMax]             ; ax, [bp-0x8]
CWD       dx, ax              
AND       ax, 0x03ff          
AND       dx, 0x0000          
MOV       cx, 0x0000          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       cx, [bp-prod]             ; cx, [bp-0x1c]
MOV       bx, [bp-prod+0x2]         ; bx, [bp-0x1a]
AND       cx, 0xfc00          
AND       bx, 0xffff          
OR        cx, ax              
OR        bx, dx              
MOV       [bp-prod], cx             ; [bp-0x1c], cx
MOV       [bp-prod+0x2], bx         ; [bp-0x1a], bx

L_102c:                             ; turn2.c:371
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_1086              

L_1034:                             ; turn2.c:372
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       cx, 0x0014          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFuldiv                 ; uint32_t __aFuldiv(uint32_t a, uint32_t b)
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_1086:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JL        L_1034              

L_108f:                             ; turn2.c:374
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-0x1a]       
AND       ax, 0x03ff          
AND       dx, 0x0000          
CMP       dx, 0x0000          
JC        L_1712              

L_10a3:
JA        L_10b0              

L_10a8:
CMP       ax, 0x0000          
JBE       L_1712              

L_10b0:                             ; turn2.c:376
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_1101              

L_10b8:                             ; turn2.c:377
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
SUB       ax, [si]            
SBB       dx, [si+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
CMP       dx, [si+0x2]        
JL        L_10fd              

L_10ee:
JG        L_110a              

L_10f3:
CMP       ax, [si]            
JA        L_110a              

L_10f7:
JMP       L_10fd              

L_10fa:                             ; turn2.c:378
JMP       L_110a              

L_10fd:                             ; turn2.c:380
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_1101:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JL        L_10b8              

L_110a:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JGE       L_165c              

L_1113:                             ; turn2.c:382
MOV       [bp-fMineralBlocked], 0x0000 ; [bp-0x54], 0x0000
MOV       ax, 0x0000          
MOV       [bp-fResourceBlocked], ax ; [bp-0x36], ax
                                    ; turn2.c:383
MOV       [bp-pct], 0x0064          ; [bp-0x42], 0x0064
MOV       [bp-pct+0x2], 0x0000      ; [bp-0x40], 0x0000
                                    ; turn2.c:384
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_12ea              

L_1130:                             ; turn2.c:386
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
CMP       [bx+0x2], 0x0000    
JL        L_12e6              

L_1145:
JG        L_1152              

L_114a:
CMP       [bx], 0x0000        
JBE       L_12e6              

L_1152:                             ; turn2.c:388
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
CMP       [si+0x2], dx        
JL        L_1190              

L_1177:
JG        L_1183              

L_117c:
CMP       [si], ax            
JC        L_1190              

L_1183:                             ; turn2.c:389
MOV       [bp-pctT], 0x0064         ; [bp-0x6], 0x0064
MOV       [bp-pctT+0x2], 0x0000     ; [bp-0x4], 0x0000
                                    ; turn2.c:390
JMP       L_1270              

L_1190:                             ; turn2.c:393
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
PUSH      [bx+0x2]            
PUSH      [bx]                
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
ADD       ax, [si]            
ADC       dx, [si+0x2]        
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-pctT], ax             ; [bp-0x6], ax
MOV       [bp-pctT+0x2], dx         ; [bp-0x4], dx
                                    ; turn2.c:395
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
PUSH      [bx+0x2]            
PUSH      [bx]                
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
ADD       ax, [si]            
ADC       dx, [si+0x2]        
ADD       ax, 0x0001          
ADC       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-pctTooBig], ax        ; [bp-0x3e], ax
MOV       [bp-pctTooBig+0x2], dx    ; [bp-0x3c], dx
                                    ; turn2.c:396
MOV       ax, [bp-pctTooBig]        ; ax, [bp-0x3e]
MOV       dx, [bp-pctTooBig+0x2]    ; dx, [bp-0x3c]
ADD       ax, 0xffff          
ADC       dx, 0xffff          
CMP       [bp-pctT+0x2], dx         ; [bp-0x4], dx
JL        L_125e              

L_1248:
JG        L_1255              

L_124d:
CMP       [bp-pctT], ax             ; [bp-0x6], ax
JBE       L_125e              

L_1255:
MOV       ax, [bp-pctT]             ; ax, [bp-0x6]
MOV       dx, [bp-pctT+0x2]         ; dx, [bp-0x4]
JMP       L_126a              

L_125e:
MOV       ax, [bp-pctTooBig]        ; ax, [bp-0x3e]
MOV       dx, [bp-pctTooBig+0x2]    ; dx, [bp-0x3c]
ADD       ax, 0xffff          
ADC       dx, 0xffff          

L_126a:
MOV       [bp-pctT], ax             ; [bp-0x6], ax
MOV       [bp-pctT+0x2], dx         ; [bp-0x4], dx

L_1270:                             ; turn2.c:398
MOV       ax, [bp-pct]              ; ax, [bp-0x42]
MOV       dx, [bp-pct+0x2]          ; dx, [bp-0x40]
CMP       [bp-pctT+0x2], dx         ; [bp-0x4], dx
JG        L_12e6              

L_127e:
JL        L_128b              

L_1283:
CMP       [bp-pctT], ax             ; [bp-0x6], ax
JNC       L_12e6              

L_128b:                             ; turn2.c:400
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
SUB       ax, [si]            
SBB       dx, [si+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
SUB       ax, [si]            
SBB       dx, [si+0x2]        
MOV       [bp-lMinNeeded], ax       ; [bp-0x14], ax
MOV       [bp-lMinNeeded+0x2], dx   ; [bp-0x12], dx
                                    ; turn2.c:402
MOV       ax, [bp-pctT]             ; ax, [bp-0x6]
MOV       dx, [bp-pctT+0x2]         ; dx, [bp-0x4]
MOV       [bp-pct], ax              ; [bp-0x42], ax
MOV       [bp-pct+0x2], dx          ; [bp-0x40], dx
                                    ; turn2.c:403
CMP       [bp-i], 0x0003            ; [bp-0x34], 0x0003
JNZ       L_12e1              

L_12d9:                             ; turn2.c:404
MOV       [bp-fResourceBlocked], 0x0001 ; [bp-0x36], 0x0001
                                    ; turn2.c:405
JMP       L_12e6              

L_12e1:                             ; turn2.c:406
MOV       [bp-fMineralBlocked], 0x0001 ; [bp-0x54], 0x0001

L_12e6:                             ; turn2.c:409
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_12ea:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JL        L_1130              

L_12f3:                             ; turn2.c:412
CMP       [bp-fMineralBlocked], 0x0000 ; [bp-0x54], 0x0000
JZ        L_131c              

L_12fc:
CMP       [bp-fAutoBuild], 0x0000   ; [bp-0x1e], 0x0000
JZ        L_131c              

L_1305:                             ; turn2.c:413
CMP       [bp+fAlchemy], 0x0000     ; [bp+0x12], 0x0000
JNZ       LAlchemize          

L_130b:
JMP       L_1314              

L_130e:                             ; turn2.c:414
JMP       LAlchemize          

L_1314:                             ; turn2.c:417
MOV       [bp-fAutoBuild], 0x0002   ; [bp-0x1e], 0x0002
                                    ; turn2.c:418
JMP       L_1712              

L_131c:                             ; turn2.c:421
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_1398              

L_1324:                             ; turn2.c:423
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-pct+0x2]              ; [bp-0x40]
PUSH      [bp-pct]                  ; [bp-0x42]
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
PUSH      [bx+0x2]            
PUSH      [bx]                
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
SUB       ax, [si]            
SBB       dx, [si+0x2]        
MOV       [bp-AddCost], ax          ; [bp-0x58], ax
MOV       [bp-AddCost+0x2], dx      ; [bp-0x56], dx
                                    ; turn2.c:424
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bp-AddCost]          ; ax, [bp-0x58]
MOV       dx, [bp-AddCost+0x2]      ; dx, [bp-0x56]
SUB       [bx], ax            
SBB       [bx+0x2], dx        
                                    ; turn2.c:425
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x32]       
ADD       bx, ax              
MOV       ax, [bp-AddCost]          ; ax, [bp-0x58]
MOV       dx, [bp-AddCost+0x2]      ; dx, [bp-0x56]
ADD       [bx], ax            
ADC       [bx+0x2], dx        
                                    ; turn2.c:427
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_1398:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JL        L_1324              

L_13a1:                             ; turn2.c:428
MOV       ax, [bp-pct]              ; ax, [bp-0x42]
MOV       dx, [bp-0x40]       
AND       ax, 0x007f          
AND       dx, 0x0000          
MOV       cx, 0x0014          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       cx, [bp-prod]             ; cx, [bp-0x1c]
MOV       bx, [bp-prod+0x2]         ; bx, [bp-0x1a]
AND       cx, 0xffff          
AND       bx, 0xf80f          
OR        cx, ax              
OR        bx, dx              
MOV       [bp-prod], cx             ; [bp-0x1c], cx
MOV       [bp-prod+0x2], bx         ; [bp-0x1a], bx
                                    ; turn2.c:430
CMP       [bp+fAlchemy], 0x0000     ; [bp+0x12], 0x0000
JZ        L_1712              

L_13d5:
CMP       [bp-fResourceBlocked], 0x0000 ; [bp-0x36], 0x0000
JNZ       L_1712              

L_13db:
JMP       LAlchemize          

L_13de:                             ; turn2.c:431
JMP       L_1712              

LAlchemize:                         ; turn2.c:433
MOV       ax, 0x0006          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, 0x00c0          
IMUL      es:[bx+0x2]         
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_140c              

L_1405:
MOV       ax, 0x0019          
CWD       dx, ax              
JMP       L_1410              

L_140c:
MOV       ax, 0x0064          
CWD       dx, ax              

L_1410:
MOV       [bp-lAlchCost], ax        ; [bp-0x18], ax
MOV       [bp-lAlchCost+0x2], dx    ; [bp-0x16], dx
                                    ; turn2.c:434
PUSH      [bp-lAlchCost+0x2]        ; [bp-0x16]
PUSH      [bp-lAlchCost]            ; [bp-0x18]
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
PUSH      [bx+0xe]            
PUSH      [bx+0xc]            
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cCanBuild], ax        ; [bp-0x10], ax
MOV       [bp-cCanBuild+0x2], dx    ; [bp-0xe], dx
                                    ; turn2.c:435
MOV       ax, [bp-lMinNeeded]       ; ax, [bp-0x14]
MOV       dx, [bp-lMinNeeded+0x2]   ; dx, [bp-0x12]
CMP       [bp-cCanBuild+0x2], dx    ; [bp-0xe], dx
JL        L_1457              

L_143e:
JG        L_144b              

L_1443:
CMP       [bp-cCanBuild], ax        ; [bp-0x10], ax
JBE       L_1457              

L_144b:                             ; turn2.c:436
MOV       ax, [bp-lMinNeeded]       ; ax, [bp-0x14]
MOV       dx, [bp-lMinNeeded+0x2]   ; dx, [bp-0x12]
MOV       [bp-cCanBuild], ax        ; [bp-0x10], ax
MOV       [bp-cCanBuild+0x2], dx    ; [bp-0xe], dx

L_1457:                             ; turn2.c:438
CMP       [bp-cCanBuild+0x2], 0x0000; [bp-0xe], 0x0000
JL        L_14c5              

L_1460:
JG        L_146e              

L_1465:
CMP       [bp-cCanBuild], 0x0000    ; [bp-0x10], 0x0000
JBE       L_14c5              

L_146e:                             ; turn2.c:440
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_1491              

L_1476:                             ; turn2.c:441
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bp-cCanBuild]        ; ax, [bp-0x10]
MOV       dx, [bp-cCanBuild+0x2]    ; dx, [bp-0xe]
ADD       [bx], ax            
ADC       [bx+0x2], dx        
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_1491:
CMP       [bp-i], 0x0003            ; [bp-0x34], 0x0003
JL        L_1476              

L_149a:                             ; turn2.c:442
PUSH      [bp-cCanBuild+0x2]        ; [bp-0xe]
PUSH      [bp-cCanBuild]            ; [bp-0x10]
PUSH      [bp-lAlchCost+0x2]        ; [bp-0x16]
PUSH      [bp-lAlchCost]            ; [bp-0x18]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
SUB       [si], ax            
SBB       [si+0x2], dx        
                                    ; turn2.c:443
MOV       ax, [bp-cCanBuild]        ; ax, [bp-0x10]
MOV       dx, [bp-0xe]        
ADD       [bp-cAlchemy], ax         ; [bp-0x22], ax

L_14c5:                             ; turn2.c:445
MOV       ax, [bp-lMinNeeded]       ; ax, [bp-0x14]
MOV       dx, [bp-lMinNeeded+0x2]   ; dx, [bp-0x12]
CMP       [bp-cCanBuild], ax        ; [bp-0x10], ax
JNZ       L_14de              

L_14d3:
CMP       [bp-cCanBuild+0x2], dx    ; [bp-0xe], dx
JZ        L_108f              

L_14d8:
JMP       L_14de              

L_14db:                             ; turn2.c:446
JMP       L_108f              

L_14de:                             ; turn2.c:448
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
CMP       [bx+0xe], 0x0000    
JL        L_1712              

L_14ea:
JG        L_14f8              

L_14ef:
CMP       [bx+0xc], 0x0000    
JBE       L_1712              

L_14f8:
CMP       [bp+pprodPartial], 0x0000 ; [bp+0xe], 0x0000
JZ        L_1712              

L_1501:                             ; turn2.c:450
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+pprodPartial]         ; [bp+0xe]
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; turn2.c:451
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0xffff          
AND       dx, 0xfff1          
OR        ax, 0x0000          
OR        dx, 0x0002          
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       [bx], ax            
MOV       [bx+0x2], dx        
                                    ; turn2.c:452
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x03ff          
AND       dx, 0xfffe          
OR        ax, 0x2c00          
OR        dx, 0x0000          
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       [bx], ax            
MOV       [bx+0x2], dx        
                                    ; turn2.c:453
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0xfc00          
AND       dx, 0xffff          
OR        ax, 0x0001          
OR        dx, 0x0000          
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       [bx], ax            
MOV       [bx+0x2], dx        
                                    ; turn2.c:455
PUSH      [bp-lAlchCost+0x2]        ; [bp-0x16]
PUSH      [bp-lAlchCost]            ; [bp-0x18]
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
PUSH      [bx+0xe]            
PUSH      [bx+0xc]            
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-pctT], ax             ; [bp-0x6], ax
MOV       [bp-pctT+0x2], dx         ; [bp-0x4], dx
                                    ; turn2.c:456
PUSH      [bp-lAlchCost+0x2]        ; [bp-0x16]
PUSH      [bp-lAlchCost]            ; [bp-0x18]
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
MOV       ax, [bx+0xc]        
MOV       dx, [bx+0xe]        
ADD       ax, 0x0001          
ADC       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-pctTooBig], ax        ; [bp-0x3e], ax
MOV       [bp-pctTooBig+0x2], dx    ; [bp-0x3c], dx
                                    ; turn2.c:457
MOV       ax, [bp-pctTooBig]        ; ax, [bp-0x3e]
MOV       dx, [bp-pctTooBig+0x2]    ; dx, [bp-0x3c]
ADD       ax, 0xffff          
ADC       dx, 0xffff          
CMP       [bp-pctT+0x2], dx         ; [bp-0x4], dx
JL        L_15ec              

L_15d6:
JG        L_15e3              

L_15db:
CMP       [bp-pctT], ax             ; [bp-0x6], ax
JBE       L_15ec              

L_15e3:
MOV       ax, [bp-pctT]             ; ax, [bp-0x6]
MOV       dx, [bp-pctT+0x2]         ; dx, [bp-0x4]
JMP       L_15f8              

L_15ec:
MOV       ax, [bp-pctTooBig]        ; ax, [bp-0x3e]
MOV       dx, [bp-pctTooBig+0x2]    ; dx, [bp-0x3c]
ADD       ax, 0xffff          
ADC       dx, 0xffff          

L_15f8:
MOV       [bp-pctT], ax             ; [bp-0x6], ax
MOV       [bp-pctT+0x2], dx         ; [bp-0x4], dx
                                    ; turn2.c:459
MOV       ax, [bp-pctT]             ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        
AND       ax, 0x007f          
AND       dx, 0x0000          
MOV       cx, 0x0014          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       cx, [bx]            
MOV       bx, [bx+0x2]        
AND       cx, 0xffff          
AND       bx, 0xf80f          
OR        cx, ax              
OR        bx, dx              
MOV       si, [bp+pprodPartial]     ; si, [bp+0xe]
MOV       [si], cx            
MOV       [si+0x2], bx        
                                    ; turn2.c:461
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-lAlchCost+0x2]        ; [bp-0x16]
PUSH      [bp-lAlchCost]            ; [bp-0x18]
PUSH      [bp-pctT+0x2]             ; [bp-0x4]
PUSH      [bp-pctT]                 ; [bp-0x6]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       bx, [bp+rgRes]            ; bx, [bp+0x10]
SUB       [bx+0xc], ax        
SBB       [bx+0xe], dx        

L_1656:                             ; turn2.c:463
JMP       L_1712              

L_165c:                             ; turn2.c:467
ADD       [bp-cBuilt], 0x0001       ; [bp-0x20], 0x0001
                                    ; turn2.c:468
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-0x1a]       
ADD       ax, 0xffff          
ADC       dx, 0xffff          
AND       ax, 0x03ff          
AND       dx, 0x0000          
MOV       [bp-0x58], ax       
MOV       [bp-0x56], dx       
AND       [bp-prod], 0xfc00         ; [bp-0x1c], 0xfc00
AND       [bp-prod+0x2], 0xffff     ; [bp-0x1a], 0xffff
MOV       ax, [bp-0x1c]       
MOV       dx, [bp-0x1a]       
MOV       ax, [bp-0x58]       
MOV       dx, [bp-0x56]       
OR        [bp-prod], ax             ; [bp-0x1c], ax
OR        [bp-prod+0x2], dx         ; [bp-0x1a], dx
MOV       ax, [bp-0x1c]       
MOV       dx, [bp-0x1a]       
                                    ; turn2.c:469
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
AND       ax, 0xffff          
AND       dx, 0xf80f          
OR        ax, 0x0000          
OR        dx, 0x0000          
MOV       [bp-prod], ax             ; [bp-0x1c], ax
MOV       [bp-prod+0x2], dx         ; [bp-0x1a], dx
                                    ; turn2.c:470
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_1706              

L_16ba:                             ; turn2.c:472
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x32]       
ADD       si, bx              
SUB       ax, [si]            
SBB       dx, [si+0x2]        
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
SUB       [si], ax            
SBB       [si+0x2], dx        
                                    ; turn2.c:473
MOV       ax, [bp-i]                ; ax, [bp-0x34]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x32]       
ADD       bx, ax              
MOV       [bx], 0x0000        
MOV       [bx+0x2], 0x0000    
                                    ; turn2.c:474
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_1706:
CMP       [bp-i], 0x0004            ; [bp-0x34], 0x0004
JGE       L_108f              

L_170c:
JMP       L_16ba              

L_170f:                             ; turn2.c:476
JMP       L_108f              

L_1712:                             ; turn2.c:479
CMP       [bp-cBuilt], 0x0000       ; [bp-0x20], 0x0000
JLE       L_17b7              

L_171b:
MOV       cx, 0x0011          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0007          
AND       dx, 0x0000          
CMP       ax, 0x0001          
JNZ       L_17b7              

L_1737:
CMP       dx, 0x0000          
JNZ       L_17b7              

L_173f:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x000b          
JNZ       L_1763              

L_175b:
CMP       dx, 0x0000          
JZ        L_1787              

L_1763:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x0003          
JNZ       L_17b7              

L_177f:
CMP       dx, 0x0000          
JNZ       L_17b7              

L_1787:                             ; turn2.c:481
MOV       ax, [bp-cBuilt]           ; ax, [bp-0x20]
ADD       [bp-cAlchemy], ax         ; [bp-0x22], ax
                                    ; turn2.c:482
MOV       [bp-i], 0x0000            ; [bp-0x34], 0x0000
JMP       L_17ae              

L_1795:                             ; turn2.c:483
MOV       ax, [bp-cBuilt]           ; ax, [bp-0x20]
CWD       dx, ax              
MOV       bx, [bp-i]                ; bx, [bp-0x34]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgRes]            ; si, [bp+0x10]
ADD       si, bx              
ADD       [si], ax            
ADC       [si+0x2], dx        
ADD       [bp-i], 0x0001            ; [bp-0x34], 0x0001

L_17ae:
CMP       [bp-i], 0x0003            ; [bp-0x34], 0x0003
JL        L_1795              

L_17b7:                             ; turn2.c:486
CMP       [bp-cAlchemy], 0x0000     ; [bp-0x22], 0x0000
JZ        L_17fb              

L_17c0:
CMP       [bp+fCalcOnly], 0x0000    ; [bp+0x16], 0x0000
JNZ       L_17fb              

L_17c9:
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_17fb              

L_17d9:                             ; turn2.c:487
PUSH      [bp-cAlchemy]             ; [bp-0x22]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x008c          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          

L_17fb:                             ; turn2.c:489
CMP       [bp+pmdStatus], 0x0000    ; [bp+0x14], 0x0000
JZ        L_18d1              

L_1804:                             ; turn2.c:491
CMP       [bp-fAutoBuild], 0x0002   ; [bp-0x1e], 0x0002
JNZ       L_1827              

L_180d:                             ; turn2.c:492
CMP       [bp-cBuilt], 0x0000       ; [bp-0x20], 0x0000
JLE       L_181c              

L_1816:
MOV       ax, 0x0003          
JMP       L_181f              

L_181c:
MOV       ax, 0x0004          

L_181f:
MOV       bx, [bp+pmdStatus]        ; bx, [bp+0x14]
MOV       [bx], ax            
                                    ; turn2.c:493
JMP       L_18d1              

L_1827:
CMP       [bp-fAutoBuild], 0x0000   ; [bp-0x1e], 0x0000
JZ        L_1866              

L_1830:
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-0x1a]       
AND       ax, 0x03ff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_1866              

L_1844:
CMP       dx, 0x0000          
JNZ       L_1866              

L_184c:                             ; turn2.c:494
CMP       [bp-cBuilt], 0x0000       ; [bp-0x20], 0x0000
JLE       L_185b              

L_1855:
MOV       ax, 0x0001          
JMP       L_185e              

L_185b:
MOV       ax, 0x0002          

L_185e:
MOV       bx, [bp+pmdStatus]        ; bx, [bp+0x14]
MOV       [bx], ax            
                                    ; turn2.c:495
JMP       L_18d1              

L_1866:
CMP       [bp-cBuilt], 0x0000       ; [bp-0x20], 0x0000
JNZ       L_18a4              

L_186f:                             ; turn2.c:496
MOV       cx, 0x0014          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       [bp-pctInitial], ax       ; [bp-0x3a], ax
JNZ       L_1899              

L_188b:
CMP       [bp-pctInitial+0x2], dx   ; [bp-0x38], dx
JNZ       L_1899              

L_1893:
MOV       ax, 0x0007          
JMP       L_189c              

L_1899:
MOV       ax, 0x0006          

L_189c:
MOV       bx, [bp+pmdStatus]        ; bx, [bp+0x14]
MOV       [bx], ax            
                                    ; turn2.c:497
JMP       L_18d1              

L_18a4:
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-0x1a]       
AND       ax, 0x03ff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_18ca              

L_18b8:
CMP       dx, 0x0000          
JNZ       L_18ca              

L_18c0:                             ; turn2.c:498
MOV       bx, [bp+pmdStatus]        ; bx, [bp+0x14]
MOV       [bx], 0x0000        
                                    ; turn2.c:499
JMP       L_18d1              

L_18ca:                             ; turn2.c:500
MOV       bx, [bp+pmdStatus]        ; bx, [bp+0x14]
MOV       [bx], 0x0005        

L_18d1:                             ; turn2.c:503
CMP       [bp+fCalcOnly], 0x0000    ; [bp+0x16], 0x0000
JNZ       L_18f3              

L_18da:
CMP       [bp-fAutoBuild], 0x0000   ; [bp-0x1e], 0x0000
JNZ       L_18f3              

L_18e3:                             ; turn2.c:504
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
LES       bx, [bp+lpprod]           ; bx, [bp+0xa]
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     

L_18f3:                             ; turn2.c:507
CMP       [bp-fAutoBuild], 0x0000   ; [bp-0x1e], 0x0000
JZ        L_19a5              

L_18fc:
CMP       [bp+pprodPartial], 0x0000 ; [bp+0xe], 0x0000
JZ        L_19a5              

L_1905:
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x03ff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_19a5              

L_191b:
CMP       dx, 0x0000          
JNZ       L_19a5              

L_1923:
MOV       cx, 0x0014          
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       dx, 0x0000          
JC        L_19a5              

L_193f:
JA        L_194c              

L_1944:
CMP       ax, 0x0000          
JBE       L_19a5              

L_194c:                             ; turn2.c:509
MOV       ax, [bp-prod]             ; ax, [bp-0x1c]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x1a]
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       [bx], ax            
MOV       [bx+0x2], dx        
                                    ; turn2.c:510
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0xfc00          
AND       dx, 0xffff          
OR        ax, 0x0001          
OR        dx, 0x0000          
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       [bx], ax            
MOV       [bx+0x2], dx        
                                    ; turn2.c:511
MOV       ax, [bp-iobjOther]        ; ax, [bp-0xc]
MOV       dx, [bp-0xa]        
AND       ax, 0x007f          
AND       dx, 0x0000          
MOV       cx, 0x000a          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       bx, [bp+pprodPartial]     ; bx, [bp+0xe]
MOV       cx, [bx]            
MOV       bx, [bx+0x2]        
AND       cx, 0x03ff          
AND       bx, 0xfffe          
OR        cx, ax              
OR        bx, dx              
MOV       si, [bp+pprodPartial]     ; si, [bp+0xe]
MOV       [si], cx            
MOV       [si+0x2], bx        

L_19a5:                             ; turn2.c:514
MOV       ax, [bp-cBuilt]           ; ax, [bp-0x20]

L_19ab:                             ; turn2.c:515
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


