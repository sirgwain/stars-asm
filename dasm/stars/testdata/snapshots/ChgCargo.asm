; ChgCargo  (ship)
;   addr: 000b:6034  len=1176
;   sig:  int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj)
;   params:
;     GrobjClass       grobj          [BP+0x6]
;     int16_t          id             [BP+0x8]
;     int16_t          iSupply        [BP+0xa]
;     int32_t          dChg           [BP+0xc]
;     void *           pobj           [BP+0x10]
;   locals:
;     PLANET *         ppl            [BP-0x8a]
;     FLEET *          pfl            [BP-0x88]
;     int16_t          i              [BP-0x86]
;     XFER             xfer           [BP-0x84]
;     THING *          pth            [BP-0x4]
;     block 000B:61DB  len=0x118
;       int32_t          wtFree         [BP-0x8e]
;
;   stats: blocks=1  labels=0

L_6034:                             ; ship.c:2346
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x008e          
PUSH      si                  
PUSH      di                  
                                    ; ship.c:2355
CMP       [bp+grobj], 0x0001        ; [bp+0x6], 0x0001
JZ        L_604f              

L_6046:
CMP       [bp+grobj], 0x0004        ; [bp+0x6], 0x0004
JNZ       L_6190              

L_604f:                             ; ship.c:2357
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JZ        L_6062              

L_6058:                             ; ship.c:2358
MOV       ax, [bp+pobj]             ; ax, [bp+0x10]
MOV       [bp-ppl], ax              ; [bp-0x8a], ax
                                    ; ship.c:2359
JMP       L_609f              

L_6062:
CMP       [bp+grobj], 0x0001        ; [bp+0x6], 0x0001
JNZ       L_6084              

L_606b:                             ; ship.c:2362
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
PUSH      ax                  
PUSH      [bp+id]                   ; [bp+0x8]
CALLF     FLookupPlanet             ; int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl)
ADD       sp, 0x0004          
                                    ; ship.c:2363
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
MOV       [bp-ppl], ax              ; [bp-0x8a], ax
                                    ; ship.c:2365
JMP       L_609f              

L_6084:                             ; ship.c:2367
MOV       ax, 0x0038          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
PUSH      ax                  
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; ship.c:2368
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
MOV       [bp-ppl], ax              ; [bp-0x8a], ax

L_609f:                             ; ship.c:2372
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JG        L_6159              

L_60a8:                             ; ship.c:2374
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JNZ       L_60ba              

L_60b1:                             ; ship.c:2375
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_64c6              

L_60ba:                             ; ship.c:2377
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_60e6              

L_60c3:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JNZ       L_60e6              

L_60cc:                             ; ship.c:2378
MOV       ax, 0x001c          
MOV       bx, [bp-ppl]              ; bx, [bp-0x8a]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
JMP       L_64c6              

L_60e6:                             ; ship.c:2380
MOV       ax, 0x001c          
MOV       bx, [bp-ppl]              ; bx, [bp-0x8a]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
ADD       ax, [bp+dChg]             ; ax, [bp+0xc]
ADC       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
CMP       dx, 0x0000          
JG        L_613c              

L_610b:
JL        L_6118              

L_6110:
CMP       ax, 0x0000          
JNC       L_613c              

L_6118:                             ; ship.c:2381
MOV       ax, 0x001c          
MOV       bx, [bp-ppl]              ; bx, [bp-0x8a]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
NEG       ax                  
ADC       dx, 0x0000          
NEG       dx                  
MOV       [bp+dChg], ax             ; [bp+0xc], ax
MOV       [bp+dChg+0x2], dx         ; [bp+0xe], dx

L_613c:                             ; ship.c:2382
MOV       ax, 0x001c          
MOV       bx, [bp-ppl]              ; bx, [bp-0x8a]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bp+dChg]             ; ax, [bp+0xc]
MOV       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
ADD       [bx], ax            
ADC       [bx+0x2], dx        

L_6159:                             ; ship.c:2385
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_616b              

L_6162:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JZ        L_64bd              

L_616b:
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JNZ       L_64bd              

L_6174:
CMP       [bp+grobj], 0x0004        ; [bp+0x6], 0x0004
JZ        L_64bd              

L_617d:                             ; ship.c:2386
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
PUSH      ax                  
MOV       ax, 0xffff          
PUSH      ax                  
CALLF     FLookupPlanet             ; int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl)
ADD       sp, 0x0004          

L_618d:                             ; ship.c:2388
JMP       L_64bd              

L_6190:
CMP       [bp+grobj], 0x0008        ; [bp+0x6], 0x0008
JNZ       L_6320              

L_6199:                             ; ship.c:2390
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JZ        L_61ab              

L_61a2:                             ; ship.c:2391
MOV       ax, [bp+pobj]             ; ax, [bp+0x10]
MOV       [bp-pth], ax              ; [bp-0x4], ax
                                    ; ship.c:2392
JMP       L_61c0              

L_61ab:                             ; ship.c:2394
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
PUSH      ax                  
PUSH      [bp+id]                   ; [bp+0x8]
CALLF     FLookupThing              ; int16_t FLookupThing(int16_t idth, THING *pth)
ADD       sp, 0x0004          
                                    ; ship.c:2395
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
MOV       [bp-pth], ax              ; [bp-0x4], ax

L_61c0:                             ; ship.c:2399
CMP       [bp+iSupply], 0x0003      ; [bp+0xa], 0x0003
JL        L_61d2              

L_61c9:                             ; ship.c:2400
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_64c6              

L_61d2:                             ; ship.c:2401
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JG        L_62f3              

L_61db:                             ; ship.c:2405
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_6202              

L_61e4:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JNZ       L_6202              

L_61ed:                             ; ship.c:2406
MOV       ax, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
CWD       dx, ax              
JMP       L_64c6              

L_6202:                             ; ship.c:2407
MOV       ax, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
CWD       dx, ax              
ADD       ax, [bp+dChg]             ; ax, [bp+0xc]
ADC       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
CMP       dx, 0x0000          
JG        L_6249              

L_6222:
JL        L_622f              

L_6227:
CMP       ax, 0x0000          
JNC       L_6249              

L_622f:                             ; ship.c:2408
MOV       ax, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
NEG       ax                  
CWD       dx, ax              
MOV       [bp+dChg], ax             ; [bp+0xc], ax
MOV       [bp+dChg+0x2], dx         ; [bp+0xe], dx

L_6249:                             ; ship.c:2410
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
MOV       ax, [bx+0xe]        
AND       ax, 0x3fff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-wtFree], ax           ; [bp-0x8e], ax
MOV       [bp-wtFree+0x2], dx       ; [bp-0x8c], dx
                                    ; ship.c:2411
MOV       [bp-i], 0x0000            ; [bp-0x86], 0x0000
JMP       L_6295              

L_6275:                             ; ship.c:2412
MOV       ax, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x86]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
CWD       dx, ax              
SUB       [bp-wtFree], ax           ; [bp-0x8e], ax
SBB       [bp-wtFree+0x2], dx       ; [bp-0x8c], dx
ADD       [bp-i], 0x0001            ; [bp-0x86], 0x0001

L_6295:
CMP       [bp-i], 0x0003            ; [bp-0x86], 0x0003
JL        L_6275              

L_629f:                             ; ship.c:2414
MOV       ax, [bp-wtFree]           ; ax, [bp-0x8e]
MOV       dx, [bp-wtFree+0x2]       ; dx, [bp-0x8c]
CMP       [bp+dChg+0x2], dx         ; [bp+0xe], dx
JL        L_62ca              

L_62af:
JG        L_62bc              

L_62b4:
CMP       [bp+dChg], ax             ; [bp+0xc], ax
JBE       L_62ca              

L_62bc:                             ; ship.c:2415
MOV       ax, [bp-wtFree]           ; ax, [bp-0x8e]
MOV       dx, [bp-wtFree+0x2]       ; dx, [bp-0x8c]
MOV       [bp+dChg], ax             ; [bp+0xc], ax
MOV       [bp+dChg+0x2], dx         ; [bp+0xe], dx

L_62ca:                             ; ship.c:2417
MOV       ax, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
CWD       dx, ax              
ADD       ax, [bp+dChg]             ; ax, [bp+0xc]
ADC       dx, [bp+0xe]        
MOV       cx, 0x0008          
MOV       bx, [bp-pth]              ; bx, [bp-0x4]
ADD       bx, cx              
MOV       dx, [bp+iSupply]          ; dx, [bp+0xa]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       [bx], ax            

L_62f3:                             ; ship.c:2420
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_6305              

L_62fc:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JZ        L_64bd              

L_6305:
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JNZ       L_64bd              

L_630e:                             ; ship.c:2421
PUSH      [bp-pth]                  ; [bp-0x4]
MOV       ax, 0xffff          
PUSH      ax                  
CALLF     FLookupThing              ; int16_t FLookupThing(int16_t idth, THING *pth)
ADD       sp, 0x0004          

L_631d:                             ; ship.c:2423
JMP       L_64bd              

L_6320:                             ; ship.c:2426
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JZ        L_6333              

L_6329:                             ; ship.c:2427
MOV       ax, [bp+pobj]             ; ax, [bp+0x10]
MOV       [bp-pfl], ax              ; [bp-0x88], ax
                                    ; ship.c:2428
JMP       L_6349              

L_6333:                             ; ship.c:2430
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
PUSH      ax                  
PUSH      [bp+id]                   ; [bp+0x8]
CALLF     FLookupFleet              ; int16_t FLookupFleet(int16_t idFleet, FLEET *pfl)
ADD       sp, 0x0004          
                                    ; ship.c:2431
LEA       ax, [bp-xfer+0x4]         ; ax, [bp-0x80]
MOV       [bp-pfl], ax              ; [bp-0x88], ax

L_6349:                             ; ship.c:2434
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JG        L_6492              

L_6352:                             ; ship.c:2436
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_637e              

L_635b:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JNZ       L_637e              

L_6364:                             ; ship.c:2437
MOV       ax, 0x004c          
MOV       bx, [bp-pfl]              ; bx, [bp-0x88]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
JMP       L_64c6              

L_637e:                             ; ship.c:2438
MOV       ax, 0x004c          
MOV       bx, [bp-pfl]              ; bx, [bp-0x88]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
ADD       ax, [bp+dChg]             ; ax, [bp+0xc]
ADC       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
CMP       dx, 0x0000          
JG        L_63d4              

L_63a3:
JL        L_63b0              

L_63a8:
CMP       ax, 0x0000          
JNC       L_63d4              

L_63b0:                             ; ship.c:2439
MOV       ax, 0x004c          
MOV       bx, [bp-pfl]              ; bx, [bp-0x88]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
NEG       ax                  
ADC       dx, 0x0000          
NEG       dx                  
MOV       [bp+dChg], ax             ; [bp+0xc], ax
MOV       [bp+dChg+0x2], dx         ; [bp+0xe], dx

L_63d4:                             ; ship.c:2440
CMP       [bp+iSupply], 0x0003      ; [bp+0xa], 0x0003
JNZ       L_63f9              

L_63dd:
MOV       bx, [bp-pfl]              ; bx, [bp-0x88]
MOV       ax, [bx+0x4]        
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JZ        L_63f9              

L_63ef:                             ; ship.c:2441
MOV       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
MOV       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000

L_63f9:                             ; ship.c:2442
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JNZ       L_6415              

L_6402:
MOV       ax, [bp-pfl]              ; ax, [bp-0x88]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     GetFuelFree               ; int32_t GetFuelFree(FLEET *lpfl)
ADD       sp, 0x0004          
JMP       L_6425              

L_6415:
MOV       ax, [bp-pfl]              ; ax, [bp-0x88]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCargoFree              ; int32_t GetCargoFree(FLEET *lpfl)
ADD       sp, 0x0004          

L_6425:
CMP       [bp+dChg+0x2], dx         ; [bp+0xe], dx
JG        L_6443              

L_642d:
JL        L_643a              

L_6432:
CMP       [bp+dChg], ax             ; [bp+0xc], ax
JNC       L_6443              

L_643a:
MOV       ax, [bp+dChg]             ; ax, [bp+0xc]
MOV       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
JMP       L_646f              

L_6443:
CMP       [bp+iSupply], 0x0004      ; [bp+0xa], 0x0004
JNZ       L_645f              

L_644c:
MOV       ax, [bp-pfl]              ; ax, [bp-0x88]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     GetFuelFree               ; int32_t GetFuelFree(FLEET *lpfl)
ADD       sp, 0x0004          
JMP       L_646f              

L_645f:
MOV       ax, [bp-pfl]              ; ax, [bp-0x88]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCargoFree              ; int32_t GetCargoFree(FLEET *lpfl)
ADD       sp, 0x0004          

L_646f:
MOV       [bp+dChg], ax             ; [bp+0xc], ax
MOV       [bp+dChg+0x2], dx         ; [bp+0xe], dx
                                    ; ship.c:2444
MOV       ax, 0x004c          
MOV       bx, [bp-pfl]              ; bx, [bp-0x88]
ADD       bx, ax              
MOV       ax, [bp+iSupply]          ; ax, [bp+0xa]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bp+dChg]             ; ax, [bp+0xc]
MOV       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]
ADD       [bx], ax            
ADC       [bx+0x2], dx        

L_6492:                             ; ship.c:2447
CMP       [bp+dChg], 0x0000         ; [bp+0xc], 0x0000
JNZ       L_64a4              

L_649b:
CMP       [bp+dChg+0x2], 0x0000     ; [bp+0xe], 0x0000
JZ        L_64bd              

L_64a4:
CMP       [bp+pobj], 0x0000         ; [bp+0x10], 0x0000
JNZ       L_64bd              

L_64ad:                             ; ship.c:2448
PUSH      [bp-pfl]                  ; [bp-0x88]
MOV       ax, 0xffff          
PUSH      ax                  
CALLF     FLookupFleet              ; int16_t FLookupFleet(int16_t idFleet, FLEET *pfl)
ADD       sp, 0x0004          

L_64bd:                             ; ship.c:2450
MOV       ax, [bp+dChg]             ; ax, [bp+0xc]
MOV       dx, [bp+dChg+0x2]         ; dx, [bp+0xe]

L_64c6:                             ; ship.c:2451
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


