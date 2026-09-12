; DoBombing  (battle)
;   addr: 001f:aefa  len=3993
;   sig:  void DoBombing()
;   locals:
;     int32_t          dmgPeopleSmart [BP-0x4a]
;     float            pctSuccess     [BP-0x46]
;     float            pctSmart       [BP-0x42]
;     int32_t          dmgBombPeople  [BP-0x3e]
;     int32_t          cPPE           [BP-0x3a]
;     FLEET *          lpfl           [BP-0x36]
;     int16_t          ifl            [BP-0x32]
;     PLANET *         lppl           [BP-0x30]
;     int32_t          pctTerra       [BP-0x2c]
;     int32_t          cKillFact      [BP-0x28]
;     int32_t          cKillDefenses  [BP-0x24]
;     MessageId        idmSrc         [BP-0x20]
;     int32_t          dmgBombFloor   [BP-0x1e]
;     int32_t          cKillMine      [BP-0x1a]
;     int32_t          cKillPeopleS   [BP-0x16]
;     int32_t          dmgBombBldg    [BP-0x12]
;     int32_t          cKillPeople    [BP-0xe]
;     int16_t          fMulti         [BP-0xa]
;     int32_t          modKill        [BP-0x8]
;     MessageId        idmDst         [BP-0x4]
;     block 001F:B10B  len=0x43
;       double           pctSuccessHalf [BP-0x52]
;     block 001F:B7E8  len=0x1EC
;       int16_t          i              [BP-0x50]
;       int16_t          dChg           [BP-0x4e]
;       int16_t          pctTot         [BP-0x4c]
;
;   stats: blocks=2  labels=1
;     GenericBombMsg: L_bac4

L_aefa:                             ; battle.c:4177
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x005a          
PUSH      si                  
PUSH      di                  
                                    ; battle.c:4200
MOV       [bp-ifl], 0x0000          ; [bp-0x32], 0x0000
JMP       L_af0f              

L_af0b:
ADD       [bp-ifl], 0x0001          ; [bp-0x32], 0x0001

L_af0f:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-ifl], ax              ; [bp-0x32], ax
JGE       L_be8d              

L_af1a:
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
JNZ       L_af4a              

L_af42:
CMP       dx, 0x0000          
JZ        L_be8d              

L_af4a:                             ; battle.c:4202
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x000a          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_af0b              

L_af61:
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
CMP       es:[bx+0x6], 0xffff 
JZ        L_af0b              

L_af6e:
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_af0b              

L_af88:                             ; battle.c:4205
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, 0x0038          
IMUL      es:[bx+0x6]         
MOV       cx, [lpPlanets]           ; cx, [0x00f6]
MOV       dx, [lpPlanets+0x2]       ; dx, [0x00f8]
ADD       cx, ax              
MOV       [bp-lppl], cx             ; [bp-0x30], cx
MOV       [bp-lppl+0x2], dx         ; [bp-0x2e], dx
                                    ; battle.c:4207
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx+0x2]     
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x2], ax     
JZ        L_af0b              

L_afb5:
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x2], 0xffff 
JZ        L_af0b              

L_afc2:
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
PUSH      [bp-lpfl+0x2]             ; [bp-0x34]
PUSH      [bp-lpfl]                 ; [bp-0x36]
CALLF     FAttackPlayer             ; int16_t FAttackPlayer(FLEET *lpfl, int16_t iplr)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JZ        L_af0b              

L_afdf:
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_af0b              

L_aff9:                             ; battle.c:4210
LEA       ax, [bp-fMulti]           ; ax, [bp-0xa]
PUSH      ax                  
LEA       ax, [bp-pctTerra]         ; ax, [bp-0x2c]
PUSH      ax                  
LEA       ax, [bp-dmgBombBldg]      ; ax, [bp-0x12]
PUSH      ax                  
LEA       ax, [bp-dmgPeopleSmart]   ; ax, [bp-0x4a]
PUSH      ax                  
LEA       ax, [bp-dmgBombFloor]     ; ax, [bp-0x1e]
PUSH      ax                  
LEA       ax, [bp-dmgBombPeople]    ; ax, [bp-0x3e]
PUSH      ax                  
PUSH      [bp-lpfl+0x2]             ; [bp-0x34]
PUSH      [bp-lpfl]                 ; [bp-0x36]
CALLF     FCalcFleetBombDamage      ; int16_t FCalcFleetBombDamage(FLEET *lpfl, int32_t *pdmgPeople, int32_t *pdmgPeopleMin, int32_t *pdmgPeopleSmart, int32_t *pdmgBldg, int32_t *ppctTerra, int16_t *pfMulti)
ADD       sp, 0x0010          
CMP       ax, 0x0000          
JZ        L_af0b              

L_b02a:                             ; battle.c:4213
LEA       ax, [bp-pctSmart]         ; ax, [bp-0x42]
PUSH      ax                  
LEA       ax, [bp-pctSuccess]       ; ax, [bp-0x46]
PUSH      ax                  
PUSH      [bp-lppl+0x2]             ; [bp-0x2e]
PUSH      [bp-lppl]                 ; [bp-0x30]
CALLF     CalcPctSurvive            ; void CalcPctSurvive(PLANET *lppl, float *ppct, float *ppctSmart)
ADD       sp, 0x0008          
                                    ; battle.c:4215
WAIT                          
FLD       [0x1db6]            
WAIT                          
FLD       [bp-pctSuccess]           ; [bp-0x46]
NOP                           
WAIT                          
CALLF     __aFfcompp                ; void __aFfcompp()
JNC       L_b14e              

L_b055:                             ; battle.c:4217
CMP       [bp-dmgBombPeople+0x2], 0x0000 ; [bp-0x3c], 0x0000
JL        L_b08a              

L_b05e:
JG        L_b06c              

L_b063:
CMP       [bp-dmgBombPeople], 0x0000; [bp-0x3e], 0x0000
JBE       L_b08a              

L_b06c:                             ; battle.c:4218
WAIT                          
FILD      [bp-0x3e]           
WAIT                          
FLD       [bp-0x46]           
WAIT                          
FMULP     st(1), st           
WAIT                          
FLD       [0x1dba]            
WAIT                          
FADDP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
MOV       [bp-dmgBombPeople], ax    ; [bp-0x3e], ax
MOV       [bp-dmgBombPeople+0x2], dx; [bp-0x3c], dx

L_b08a:                             ; battle.c:4219
CMP       [bp-dmgBombFloor+0x2], 0x0000 ; [bp-0x1c], 0x0000
JL        L_b0bf              

L_b093:
JG        L_b0a1              

L_b098:
CMP       [bp-dmgBombFloor], 0x0000 ; [bp-0x1e], 0x0000
JBE       L_b0bf              

L_b0a1:                             ; battle.c:4220
WAIT                          
FILD      [bp-0x1e]           
WAIT                          
FLD       [bp-0x46]           
WAIT                          
FMULP     st(1), st           
WAIT                          
FLD       [0x1dba]            
WAIT                          
FADDP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
MOV       [bp-dmgBombFloor], ax     ; [bp-0x1e], ax
MOV       [bp-dmgBombFloor+0x2], dx ; [bp-0x1c], dx

L_b0bf:                             ; battle.c:4221
CMP       [bp-dmgPeopleSmart+0x2], 0x0000 ; [bp-0x48], 0x0000
JL        L_b0f4              

L_b0c8:
JG        L_b0d6              

L_b0cd:
CMP       [bp-dmgPeopleSmart], 0x0000 ; [bp-0x4a], 0x0000
JBE       L_b0f4              

L_b0d6:                             ; battle.c:4222
WAIT                          
FILD      [bp-0x4a]           
WAIT                          
FLD       [bp-0x42]           
WAIT                          
FMULP     st(1), st           
WAIT                          
FLD       [0x1dba]            
WAIT                          
FADDP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
MOV       [bp-dmgPeopleSmart], ax   ; [bp-0x4a], ax
MOV       [bp-dmgPeopleSmart+0x2], dx ; [bp-0x48], dx

L_b0f4:                             ; battle.c:4223
CMP       [bp-dmgBombBldg+0x2], 0x0000 ; [bp-0x10], 0x0000
JL        L_b14e              

L_b0fd:
JG        L_b10b              

L_b102:
CMP       [bp-dmgBombBldg], 0x0000  ; [bp-0x12], 0x0000
JBE       L_b14e              

L_b10b:                             ; battle.c:4225
WAIT                          
FLD       [bp-pctSuccess]           ; [bp-0x46]
WAIT                          
FLD       [0x1dc2]            
WAIT                          
FXCH      st(1)               
WAIT                          
FSUBP     st(1), st           
WAIT                          
FLD       [0x1dca]            
WAIT                          
FDIVP     st(1), st           
WAIT                          
FLD       [0x1dc2]            
WAIT                          
FXCH      st(1)               
WAIT                          
FSUBP     st(1), st           
WAIT                          
FSTP      [bp-pctSuccessHalf]       ; [bp-0x52]
NOP                           
WAIT                          
                                    ; battle.c:4228
WAIT                          
FILD      [bp-0x12]           
WAIT                          
FMUL      [bp-0x52]           
WAIT                          
FLD       [0x1dba]            
WAIT                          
FADDP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
MOV       [bp-dmgBombBldg], ax      ; [bp-0x12], ax
MOV       [bp-dmgBombBldg+0x2], dx  ; [bp-0x10], dx

L_b14e:                             ; battle.c:4232
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, es:[bx+0x14]    
MOV       bx, es:[bx+0x16]    
MOV       [bp-0x4e], cx       
MOV       [bp-0x4c], bx       
MOV       cx, 0x0014          
MOV       [bp-0x52], ax       
MOV       [bp-0x50], dx       
MOV       ax, [bp-0x4e]       
MOV       dx, [bp-0x4c]       
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, es:[bx+0x14]    
MOV       bx, es:[bx+0x16]    
MOV       [bp-0x56], cx       
MOV       [bp-0x54], bx       
MOV       cx, 0x0008          
MOV       [bp-0x5a], ax       
MOV       [bp-0x58], dx       
MOV       ax, [bp-0x56]       
MOV       dx, [bp-0x54]       
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       cx, [bp-0x5a]       
MOV       bx, [bp-0x58]       
ADD       ax, cx              
ADC       dx, bx              
MOV       cx, [bp-0x52]       
MOV       bx, [bp-0x50]       
ADD       ax, cx              
ADC       dx, bx              
MOV       [bp-cPPE], ax             ; [bp-0x3a], ax
MOV       [bp-cPPE+0x2], dx         ; [bp-0x38], dx
                                    ; battle.c:4233
MOV       [bp-cKillDefenses], 0x0000; [bp-0x24], 0x0000
MOV       [bp-cKillDefenses+0x2], 0x0000 ; [bp-0x22], 0x0000
MOV       ax, 0x0000          
MOV       dx, 0x0000          
MOV       [bp-cKillPeople], ax      ; [bp-0xe], ax
MOV       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
MOV       [bp-cKillMine], ax        ; [bp-0x1a], ax
MOV       [bp-cKillMine+0x2], dx    ; [bp-0x18], dx
MOV       [bp-cKillFact], ax        ; [bp-0x28], ax
MOV       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx
                                    ; battle.c:4235
CMP       [bp-dmgBombBldg+0x2], 0x0000 ; [bp-0x10], 0x0000
JL        L_b45c              

L_b1fa:
JG        L_b208              

L_b1ff:
CMP       [bp-dmgBombBldg], 0x0000  ; [bp-0x12], 0x0000
JBE       L_b45c              

L_b208:
CMP       [bp-cPPE+0x2], 0x0000     ; [bp-0x38], 0x0000
JL        L_b45c              

L_b211:
JG        L_b21f              

L_b216:
CMP       [bp-cPPE], 0x0000         ; [bp-0x3a], 0x0000
JBE       L_b45c              

L_b21f:                             ; battle.c:4237
MOV       ax, [bp-dmgBombBldg]      ; ax, [bp-0x12]
MOV       dx, [bp-dmgBombBldg+0x2]  ; dx, [bp-0x10]
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-cKillFact], ax        ; [bp-0x28], ax
MOV       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx
                                    ; battle.c:4238
PUSH      [bp-cPPE+0x2]             ; [bp-0x38]
PUSH      [bp-cPPE]                 ; [bp-0x3a]
PUSH      [bp-cKillFact+0x2]        ; [bp-0x26]
PUSH      [bp-cKillFact]            ; [bp-0x28]
CALLF     __aFlrem                  ; int32_t __aFlrem(int32_t a, int32_t b)
MOV       [bp-modKill], ax          ; [bp-0x8], ax
MOV       [bp-modKill+0x2], dx      ; [bp-0x6], dx
                                    ; battle.c:4239
PUSH      [bp-cPPE+0x2]             ; [bp-0x38]
PUSH      [bp-cPPE]                 ; [bp-0x3a]
PUSH      [bp-cKillFact+0x2]        ; [bp-0x26]
PUSH      [bp-cKillFact]            ; [bp-0x28]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cKillFact], ax        ; [bp-0x28], ax
MOV       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx
                                    ; battle.c:4240
CMP       [bp-modKill+0x2], 0x0000  ; [bp-0x6], 0x0000
JL        L_b2c8              

L_b284:
JG        L_b292              

L_b289:
CMP       [bp-modKill], 0x0000      ; [bp-0x8], 0x0000
JBE       L_b2c8              

L_b292:                             ; battle.c:4241
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
CWD       dx, ax              
CMP       dx, [bp-modKill+0x2]      ; dx, [bp-0x6]
JG        L_b2be              

L_b2aa:
JL        L_b2b7              

L_b2af:
CMP       ax, [bp-modKill]          ; ax, [bp-0x8]
JNC       L_b2be              

L_b2b7:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_b2c2              

L_b2be:
MOV       ax, 0x0000          
CWD       dx, ax              

L_b2c2:
ADD       [bp-cKillFact], ax        ; [bp-0x28], ax
ADC       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx

L_b2c8:                             ; battle.c:4243
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
CMP       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx
JL        L_b315              

L_b2e9:
JG        L_b2f6              

L_b2ee:
CMP       [bp-cKillFact], ax        ; [bp-0x28], ax
JBE       L_b315              

L_b2f6:                             ; battle.c:4244
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-cKillFact], ax        ; [bp-0x28], ax
MOV       [bp-cKillFact+0x2], dx    ; [bp-0x26], dx

L_b315:                             ; battle.c:4246
MOV       ax, [bp-dmgBombBldg]      ; ax, [bp-0x12]
MOV       dx, [bp-dmgBombBldg+0x2]  ; dx, [bp-0x10]
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-cKillDefenses], ax    ; [bp-0x24], ax
MOV       [bp-cKillDefenses+0x2], dx; [bp-0x22], dx
                                    ; battle.c:4247
PUSH      [bp-cPPE+0x2]             ; [bp-0x38]
PUSH      [bp-cPPE]                 ; [bp-0x3a]
PUSH      [bp-cKillDefenses+0x2]    ; [bp-0x22]
PUSH      [bp-cKillDefenses]        ; [bp-0x24]
CALLF     __aFlrem                  ; int32_t __aFlrem(int32_t a, int32_t b)
MOV       [bp-modKill], ax          ; [bp-0x8], ax
MOV       [bp-modKill+0x2], dx      ; [bp-0x6], dx
                                    ; battle.c:4248
PUSH      [bp-cPPE+0x2]             ; [bp-0x38]
PUSH      [bp-cPPE]                 ; [bp-0x3a]
PUSH      [bp-cKillDefenses+0x2]    ; [bp-0x22]
PUSH      [bp-cKillDefenses]        ; [bp-0x24]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cKillDefenses], ax    ; [bp-0x24], ax
MOV       [bp-cKillDefenses+0x2], dx; [bp-0x22], dx
                                    ; battle.c:4249
CMP       [bp-modKill+0x2], 0x0000  ; [bp-0x6], 0x0000
JL        L_b3b6              

L_b372:
JG        L_b380              

L_b377:
CMP       [bp-modKill], 0x0000      ; [bp-0x8], 0x0000
JBE       L_b3b6              

L_b380:                             ; battle.c:4250
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
CWD       dx, ax              
CMP       dx, [bp-modKill+0x2]      ; dx, [bp-0x6]
JG        L_b3ac              

L_b398:
JL        L_b3a5              

L_b39d:
CMP       ax, [bp-modKill]          ; ax, [bp-0x8]
JNC       L_b3ac              

L_b3a5:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_b3b0              

L_b3ac:
MOV       ax, 0x0000          
CWD       dx, ax              

L_b3b0:
ADD       [bp-cKillDefenses], ax    ; [bp-0x24], ax
ADC       [bp-cKillDefenses+0x2], dx; [bp-0x22], dx

L_b3b6:                             ; battle.c:4252
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
CMP       [bp-cKillDefenses+0x2], dx; [bp-0x22], dx
JL        L_b3f3              

L_b3cf:
JG        L_b3dc              

L_b3d4:
CMP       [bp-cKillDefenses], ax    ; [bp-0x24], ax
JBE       L_b3f3              

L_b3dc:                             ; battle.c:4253
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-cKillDefenses], ax    ; [bp-0x24], ax
MOV       [bp-cKillDefenses+0x2], dx; [bp-0x22], dx

L_b3f3:                             ; battle.c:4255
MOV       ax, [bp-cKillFact]        ; ax, [bp-0x28]
MOV       dx, [bp-cKillFact+0x2]    ; dx, [bp-0x26]
ADD       ax, [bp-cKillDefenses]    ; ax, [bp-0x24]
ADC       dx, [bp-cKillDefenses+0x2]; dx, [bp-0x22]
MOV       cx, [bp-dmgBombBldg]      ; cx, [bp-0x12]
MOV       bx, [bp-dmgBombBldg+0x2]  ; bx, [bp-0x10]
SUB       cx, ax              
SBB       bx, dx              
MOV       [bp-cKillMine], cx        ; [bp-0x1a], cx
MOV       [bp-cKillMine+0x2], bx    ; [bp-0x18], bx
                                    ; battle.c:4256
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0008          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
CMP       [bp-cKillMine+0x2], dx    ; [bp-0x18], dx
JL        L_b45c              

L_b430:
JG        L_b43d              

L_b435:
CMP       [bp-cKillMine], ax        ; [bp-0x1a], ax
JBE       L_b45c              

L_b43d:                             ; battle.c:4257
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0008          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-cKillMine], ax        ; [bp-0x1a], ax
MOV       [bp-cKillMine+0x2], dx    ; [bp-0x18], dx

L_b45c:                             ; battle.c:4261
CMP       [bp-dmgBombPeople+0x2], 0x0000 ; [bp-0x3c], 0x0000
JG        L_b4a1              

L_b465:
JL        L_b473              

L_b46a:
CMP       [bp-dmgBombPeople], 0x0000; [bp-0x3e], 0x0000
JA        L_b4a1              

L_b473:
CMP       [bp-dmgBombFloor+0x2], 0x0000 ; [bp-0x1c], 0x0000
JG        L_b4a1              

L_b47c:
JL        L_b48a              

L_b481:
CMP       [bp-dmgBombFloor], 0x0000 ; [bp-0x1e], 0x0000
JA        L_b4a1              

L_b48a:
CMP       [bp-dmgPeopleSmart+0x2], 0x0000 ; [bp-0x48], 0x0000
JL        L_b65b              

L_b493:
JG        L_b4a1              

L_b498:
CMP       [bp-dmgPeopleSmart], 0x0000 ; [bp-0x4a], 0x0000
JBE       L_b65b              

L_b4a1:
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x2a], 0x0000
JL        L_b65b              

L_b4ae:
JG        L_b4bd              

L_b4b3:
CMP       es:[bx+0x28], 0x0000
JBE       L_b65b              

L_b4bd:                             ; battle.c:4264
MOV       ax, 0x03e8          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-dmgPeopleSmart+0x2]   ; [bp-0x48]
PUSH      [bp-dmgPeopleSmart]       ; [bp-0x4a]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2a]        
PUSH      es:[bx+0x28]        
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cKillPeopleS], ax     ; [bp-0x16], ax
MOV       [bp-cKillPeopleS+0x2], dx ; [bp-0x14], dx
                                    ; battle.c:4265
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
CMP       [bp-cKillPeopleS+0x2], dx ; [bp-0x14], dx
JL        L_b51f              

L_b4fb:
JG        L_b508              

L_b500:
CMP       [bp-cKillPeopleS], ax     ; [bp-0x16], ax
JC        L_b51f              

L_b508:                             ; battle.c:4266
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
ADD       ax, 0xffff          
ADC       dx, 0xffff          
MOV       [bp-cKillPeopleS], ax     ; [bp-0x16], ax
MOV       [bp-cKillPeopleS+0x2], dx ; [bp-0x14], dx

L_b51f:                             ; battle.c:4269
PUSH      [bp-dmgBombPeople+0x2]    ; [bp-0x3c]
PUSH      [bp-dmgBombPeople]        ; [bp-0x3e]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
SUB       ax, [bp-cKillPeopleS]     ; ax, [bp-0x16]
SBB       dx, [bp-cKillPeopleS+0x2] ; dx, [bp-0x14]
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-cKillPeople], ax      ; [bp-0xe], ax
MOV       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
                                    ; battle.c:4270
MOV       ax, 0x03e8          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cKillPeople+0x2]      ; [bp-0xc]
PUSH      [bp-cKillPeople]          ; [bp-0xe]
CALLF     __aFlrem                  ; int32_t __aFlrem(int32_t a, int32_t b)
MOV       [bp-modKill], ax          ; [bp-0x8], ax
MOV       [bp-modKill+0x2], dx      ; [bp-0x6], dx
                                    ; battle.c:4271
MOV       ax, 0x03e8          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cKillPeople+0x2]      ; [bp-0xc]
PUSH      [bp-cKillPeople]          ; [bp-0xe]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cKillPeople], ax      ; [bp-0xe], ax
MOV       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
                                    ; battle.c:4273
CMP       [bp-modKill+0x2], 0x0000  ; [bp-0x6], 0x0000
JL        L_b5bf              

L_b57e:
JG        L_b58c              

L_b583:
CMP       [bp-modKill], 0x0000      ; [bp-0x8], 0x0000
JBE       L_b5bf              

L_b58c:                             ; battle.c:4274
MOV       ax, 0x03e8          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
CWD       dx, ax              
CMP       dx, [bp-modKill+0x2]      ; dx, [bp-0x6]
JG        L_b5b5              

L_b5a1:
JL        L_b5ae              

L_b5a6:
CMP       ax, [bp-modKill]          ; ax, [bp-0x8]
JA        L_b5b5              

L_b5ae:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_b5b9              

L_b5b5:
MOV       ax, 0x0000          
CWD       dx, ax              

L_b5b9:
ADD       [bp-cKillPeople], ax      ; [bp-0xe], ax
ADC       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx

L_b5bf:                             ; battle.c:4276
MOV       ax, [bp-cKillPeopleS]     ; ax, [bp-0x16]
MOV       dx, [bp-cKillPeopleS+0x2] ; dx, [bp-0x14]
ADD       [bp-cKillPeople], ax      ; [bp-0xe], ax
ADC       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
                                    ; battle.c:4278
CMP       [bp-dmgBombPeople+0x2], 0x0000 ; [bp-0x3c], 0x0000
JL        L_b603              

L_b5d4:
JG        L_b5e2              

L_b5d9:
CMP       [bp-dmgBombPeople], 0x0000; [bp-0x3e], 0x0000
JBE       L_b603              

L_b5e2:
CMP       [bp-cKillPeople+0x2], 0x0000 ; [bp-0xc], 0x0000
JG        L_b603              

L_b5eb:
JL        L_b5f9              

L_b5f0:
CMP       [bp-cKillPeople], 0x0000  ; [bp-0xe], 0x0000
JA        L_b603              

L_b5f9:                             ; battle.c:4279
MOV       [bp-cKillPeople], 0x0001  ; [bp-0xe], 0x0001
MOV       [bp-cKillPeople+0x2], 0x0000 ; [bp-0xc], 0x0000

L_b603:                             ; battle.c:4281
MOV       ax, [bp-dmgBombFloor]     ; ax, [bp-0x1e]
MOV       dx, [bp-dmgBombFloor+0x2] ; dx, [bp-0x1c]
CMP       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
JG        L_b62a              

L_b611:
JL        L_b61e              

L_b616:
CMP       [bp-cKillPeople], ax      ; [bp-0xe], ax
JNC       L_b62a              

L_b61e:                             ; battle.c:4282
MOV       ax, [bp-dmgBombFloor]     ; ax, [bp-0x1e]
MOV       dx, [bp-dmgBombFloor+0x2] ; dx, [bp-0x1c]
MOV       [bp-cKillPeople], ax      ; [bp-0xe], ax
MOV       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx

L_b62a:                             ; battle.c:4284
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
CMP       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx
JL        L_b65b              

L_b63d:
JG        L_b64a              

L_b642:
CMP       [bp-cKillPeople], ax      ; [bp-0xe], ax
JBE       L_b65b              

L_b64a:                             ; battle.c:4285
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, es:[bx+0x28]    
MOV       dx, es:[bx+0x2a]    
MOV       [bp-cKillPeople], ax      ; [bp-0xe], ax
MOV       [bp-cKillPeople+0x2], dx  ; [bp-0xc], dx

L_b65b:                             ; battle.c:4288
CMP       [bp-cKillPeople+0x2], 0x0000 ; [bp-0xc], 0x0000
JL        L_b683              

L_b664:
JG        L_b672              

L_b669:
CMP       [bp-cKillPeople], 0x0000  ; [bp-0xe], 0x0000
JBE       L_b683              

L_b672:                             ; battle.c:4289
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-cKillPeople+0x2]  ; dx, [bp-0xc]
SUB       es:[bx+0x28], ax    
SBB       es:[bx+0x2a], dx    

L_b683:                             ; battle.c:4290
CMP       [bp-cKillFact+0x2], 0x0000; [bp-0x26], 0x0000
JL        L_b6f1              

L_b68c:
JG        L_b69a              

L_b691:
CMP       [bp-cKillFact], 0x0000    ; [bp-0x28], 0x0000
JBE       L_b6f1              

L_b69a:                             ; battle.c:4291
MOV       cx, 0x0014          
MOV       ax, [bp-cKillFact]        ; ax, [bp-0x28]
MOV       dx, [bp-cKillFact+0x2]    ; dx, [bp-0x26]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, es:[bx+0x14]    
MOV       bx, es:[bx+0x16]    
SUB       cx, ax              
SBB       bx, dx              
AND       cx, 0x0000          
AND       bx, 0xfff0          
MOV       [bp-0x4e], cx       
MOV       [bp-0x4c], bx       
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
AND       es:[bx+0x14], 0xffff
AND       es:[bx+0x16], 0x000f
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, [bp-0x4e]       
MOV       dx, [bp-0x4c]       
OR        es:[bx+0x14], ax    
OR        es:[bx+0x16], dx    
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    

L_b6f1:                             ; battle.c:4292
CMP       [bp-cKillMine+0x2], 0x0000; [bp-0x18], 0x0000
JL        L_b761              

L_b6fa:
JG        L_b708              

L_b6ff:
CMP       [bp-cKillMine], 0x0000    ; [bp-0x1a], 0x0000
JBE       L_b761              

L_b708:                             ; battle.c:4293
MOV       cx, 0x0008          
MOV       ax, [bp-cKillMine]        ; ax, [bp-0x1a]
MOV       dx, [bp-cKillMine+0x2]    ; dx, [bp-0x18]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, es:[bx+0x14]    
MOV       bx, es:[bx+0x16]    
SUB       cx, ax              
SBB       bx, dx              
AND       cx, 0xff00          
AND       bx, 0x000f          
MOV       [bp-0x4e], cx       
MOV       [bp-0x4c], bx       
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
AND       es:[bx+0x14], 0x00ff
AND       es:[bx+0x16], 0xfff0
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, [bp-0x4e]       
MOV       dx, [bp-0x4c]       
OR        es:[bx+0x14], ax    
OR        es:[bx+0x16], dx    
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    

L_b761:                             ; battle.c:4294
CMP       [bp-cKillDefenses+0x2], 0x0000 ; [bp-0x22], 0x0000
JL        L_b7d1              

L_b76a:
JG        L_b778              

L_b76f:
CMP       [bp-cKillDefenses], 0x0000; [bp-0x24], 0x0000
JBE       L_b7d1              

L_b778:                             ; battle.c:4295
MOV       cx, 0x0000          
MOV       ax, [bp-cKillDefenses]    ; ax, [bp-0x24]
MOV       dx, [bp-cKillDefenses+0x2]; dx, [bp-0x22]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, es:[bx+0x18]    
MOV       bx, es:[bx+0x1a]    
SUB       cx, ax              
SBB       bx, dx              
AND       cx, 0x0fff          
AND       bx, 0x0000          
MOV       [bp-0x4e], cx       
MOV       [bp-0x4c], bx       
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
AND       es:[bx+0x18], 0xf000
AND       es:[bx+0x1a], 0xffff
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       ax, [bp-0x4e]       
MOV       dx, [bp-0x4c]       
OR        es:[bx+0x18], ax    
OR        es:[bx+0x1a], dx    
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    

L_b7d1:                             ; battle.c:4297
CMP       [bp-pctTerra+0x2], 0x0000 ; [bp-0x2a], 0x0000
JL        L_b9d4              

L_b7da:
JG        L_b7e8              

L_b7df:
CMP       [bp-pctTerra], 0x0000     ; [bp-0x2c], 0x0000
JBE       L_b9d4              

L_b7e8:                             ; battle.c:4299
MOV       [bp-pctTot], 0x0000       ; [bp-0x4c], 0x0000
                                    ; battle.c:4303
WAIT                          
FLD       [bp-0x46]           
WAIT                          
FLD       [0x1dc2]            
WAIT                          
FXCH      st(1)               
WAIT                          
FSUBP     st(1), st           
WAIT                          
FILD      [bp-0x2c]           
WAIT                          
FMULP     st(1), st           
MOV       ax, 0x0002          
CWD       dx, ax              
MOV       [bp-0x58], ax       
MOV       [bp-0x56], dx       
WAIT                          
FILD      [bp-0x58]           
WAIT                          
FDIVP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
MOV       cx, [bp-pctTerra]         ; cx, [bp-0x2c]
MOV       bx, [bp-pctTerra+0x2]     ; bx, [bp-0x2a]
SUB       cx, ax              
SBB       bx, dx              
MOV       [bp-pctTerra], cx         ; [bp-0x2c], cx
MOV       [bp-pctTerra+0x2], bx     ; [bp-0x2a], bx
                                    ; battle.c:4305
CMP       [bp-pctTerra+0x2], 0x0000 ; [bp-0x2a], 0x0000
JL        L_b84b              

L_b832:
JG        L_b841              

L_b837:
CMP       [bp-pctTerra], 0x01f4     ; [bp-0x2c], 0x01f4
JBE       L_b84b              

L_b841:                             ; battle.c:4306
MOV       [bp-pctTerra], 0x01f4     ; [bp-0x2c], 0x01f4
MOV       [bp-pctTerra+0x2], 0x0000 ; [bp-0x2a], 0x0000

L_b84b:                             ; battle.c:4308
MOV       [bp-i], 0x0000            ; [bp-0x50], 0x0000
JMP       L_b930              

L_b853:                             ; battle.c:4310
MOV       ax, 0x000f          
MOV       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       cx, [bp-lppl+0x2]         ; cx, [bp-0x2e]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x50]
ADD       bx, ax              
MOV       es, cx              
MOV       al, es:[bx]         
CBW       ax, al              
MOV       cx, 0x000c          
MOV       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       si, [bp-lppl+0x2]         ; si, [bp-0x2e]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x50]
ADD       bx, cx              
MOV       es, si              
MOV       cl, es:[bx]         
MOV       [bp-0x52], ax       
MOV       ax, cx              
CBW       ax, al              
MOV       cx, [bp-0x52]       
SUB       ax, cx              
MOV       [bp-dChg], ax             ; [bp-0x4e], ax
                                    ; battle.c:4311
CMP       [bp-dChg], 0x0000         ; [bp-0x4e], 0x0000
JLE       L_b8d8              

L_b895:                             ; battle.c:4313
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
CWD       dx, ax              
CMP       dx, [bp-pctTerra+0x2]     ; dx, [bp-0x2a]
JL        L_b8b7              

L_b8a1:
JG        L_b8ae              

L_b8a6:
CMP       ax, [bp-pctTerra]         ; ax, [bp-0x2c]
JC        L_b8b7              

L_b8ae:                             ; battle.c:4314
MOV       ax, [bp-pctTerra]         ; ax, [bp-0x2c]
MOV       dx, [bp-0x2a]       
MOV       [bp-dChg], ax             ; [bp-0x4e], ax

L_b8b7:                             ; battle.c:4316
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
MOV       cx, 0x000c          
MOV       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       si, [bp-lppl+0x2]         ; si, [bp-0x2e]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x50]
ADD       bx, cx              
MOV       es, si              
SUB       es:[bx], al         
                                    ; battle.c:4317
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
ADD       [bp-pctTot], ax           ; [bp-0x4c], ax
                                    ; battle.c:4319
JMP       L_b92c              

L_b8d8:
CMP       [bp-dChg], 0x0000         ; [bp-0x4e], 0x0000
JGE       L_b92c              

L_b8e1:                             ; battle.c:4321
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
NEG       ax                  
CWD       dx, ax              
CMP       dx, [bp-pctTerra+0x2]     ; dx, [bp-0x2a]
JL        L_b90c              

L_b8ef:
JG        L_b8fc              

L_b8f4:
CMP       ax, [bp-pctTerra]         ; ax, [bp-0x2c]
JC        L_b90c              

L_b8fc:                             ; battle.c:4322
MOV       ax, [bp-pctTerra]         ; ax, [bp-0x2c]
MOV       dx, [bp-0x2a]       
NEG       ax                  
ADC       dx, 0x0000          
NEG       dx                  
MOV       [bp-dChg], ax             ; [bp-0x4e], ax

L_b90c:                             ; battle.c:4324
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
MOV       cx, 0x000c          
MOV       bx, [bp-lppl]             ; bx, [bp-0x30]
MOV       si, [bp-lppl+0x2]         ; si, [bp-0x2e]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x50]
ADD       bx, cx              
MOV       es, si              
SUB       es:[bx], al         
                                    ; battle.c:4325
MOV       ax, [bp-dChg]             ; ax, [bp-0x4e]
NEG       ax                  
ADD       [bp-pctTot], ax           ; [bp-0x4c], ax

L_b92c:                             ; battle.c:4327
ADD       [bp-i], 0x0001            ; [bp-0x50], 0x0001

L_b930:
CMP       [bp-i], 0x0003            ; [bp-0x50], 0x0003
JL        L_b853              

L_b939:                             ; battle.c:4329
CMP       [bp-pctTot], 0x0000       ; [bp-0x4c], 0x0000
JLE       L_b9d4              

L_b942:                             ; battle.c:4332
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-pctTot]               ; [bp-0x4c]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_b97a              

L_b974:
MOV       ax, 0x017a          
JMP       L_b97d              

L_b97a:
MOV       ax, 0x012e          

L_b97d:
PUSH      ax                  
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4334
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-pctTot]               ; [bp-0x4c]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_b9c1              

L_b9bb:
MOV       ax, 0x017b          
JMP       L_b9c4              

L_b9c1:
MOV       ax, 0x012e          

L_b9c4:
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_b9d4:                             ; battle.c:4339
MOV       ax, [bp-cKillMine]        ; ax, [bp-0x1a]
MOV       dx, [bp-cKillMine+0x2]    ; dx, [bp-0x18]
ADD       ax, [bp-cKillFact]        ; ax, [bp-0x28]
ADC       dx, [bp-cKillFact+0x2]    ; dx, [bp-0x26]
ADD       ax, [bp-cKillDefenses]    ; ax, [bp-0x24]
ADC       dx, [bp-cKillDefenses+0x2]; dx, [bp-0x22]
MOV       [bp-cPPE], ax             ; [bp-0x3a], ax
MOV       [bp-cPPE+0x2], dx         ; [bp-0x38], dx
                                    ; battle.c:4340
CMP       [bp-cPPE+0x2], 0x0000     ; [bp-0x38], 0x0000
JL        L_bd61              

L_b9f5:
JG        L_ba03              

L_b9fa:
CMP       [bp-cPPE], 0x0000         ; [bp-0x3a], 0x0000
JBE       L_bd61              

L_ba03:                             ; battle.c:4342
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x2a], 0x0000
JL        L_ba6b              

L_ba10:
JG        L_ba1f              

L_ba15:
CMP       es:[bx+0x28], 0x0000
JBE       L_ba6b              

L_ba1f:                             ; battle.c:4344
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_ba2e              

L_ba28:
MOV       ax, 0x0169          
JMP       L_ba31              

L_ba2e:
MOV       ax, 0x0063          

L_ba31:
MOV       [bp-idmSrc], ax           ; [bp-0x20], ax
                                    ; battle.c:4345
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_ba43              

L_ba3d:
MOV       ax, 0x0173          
JMP       L_ba46              

L_ba43:
MOV       ax, 0x006d          

L_ba46:
MOV       [bp-idmDst], ax           ; [bp-0x4], ax
                                    ; battle.c:4346
CMP       [bp-cPPE+0x2], 0x0000     ; [bp-0x38], 0x0000
JL        L_ba98              

L_ba52:
JG        L_ba60              

L_ba57:
CMP       [bp-cPPE], 0x0001         ; [bp-0x3a], 0x0001
JBE       L_ba98              

L_ba60:                             ; battle.c:4348
ADD       [bp-idmSrc], 0x0001       ; [bp-0x20], 0x0001
                                    ; battle.c:4349
ADD       [bp-idmDst], 0x0001       ; [bp-0x4], 0x0001

L_ba68:                             ; battle.c:4352
JMP       L_ba98              

L_ba6b:                             ; battle.c:4354
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_ba7a              

L_ba74:
MOV       ax, 0x017c          
JMP       L_ba7d              

L_ba7a:
MOV       ax, 0x008f          

L_ba7d:
MOV       [bp-idmSrc], ax           ; [bp-0x20], ax
                                    ; battle.c:4355
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_ba8f              

L_ba89:
MOV       ax, 0x017d          
JMP       L_ba92              

L_ba8f:
MOV       ax, 0x0090          

L_ba92:
MOV       [bp-idmDst], ax           ; [bp-0x4], ax
                                    ; battle.c:4356
JMP       GenericBombMsg      

L_ba98:                             ; battle.c:4359
CMP       [bp-cKillPeople+0x2], 0x0000 ; [bp-0xc], 0x0000
JL        L_bc0a              

L_baa1:
JG        L_baaf              

L_baa6:
CMP       [bp-cKillPeople], 0x0000  ; [bp-0xe], 0x0000
JBE       L_bc0a              

L_baaf:                             ; battle.c:4361
WAIT                          
FLD       [0x1db6]            
WAIT                          
FLD       [bp-pctSuccess]           ; [bp-0x46]
NOP                           
WAIT                          
CALLF     __aFfcompp                ; void __aFfcompp()
JNZ       L_bb47              

GenericBombMsg:                     ; battle.c:4365
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idmSrc]               ; [bp-0x20]
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4367
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
PUSH      [bp-idmDst]               ; [bp-0x4]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4369
JMP       L_be65              

L_bb47:                             ; battle.c:4371
ADD       [bp-idmSrc], 0x0005       ; [bp-0x20], 0x0005
                                    ; battle.c:4372
ADD       [bp-idmDst], 0x0005       ; [bp-0x4], 0x0005
                                    ; battle.c:4376
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
WAIT                          
FLD       [0x1db6]            
WAIT                          
FSUB      [bp-0x46]           
MOV       ax, 0x2710          
CWD       dx, ax              
MOV       [bp-0x52], ax       
MOV       [bp-0x50], dx       
WAIT                          
FILD      [bp-0x52]           
WAIT                          
FMULP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idmSrc]               ; [bp-0x20]
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4379
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
WAIT                          
FLD       [0x1db6]            
WAIT                          
FSUB      [bp-0x46]           
MOV       ax, 0x2710          
CWD       dx, ax              
MOV       [bp-0x52], ax       
MOV       [bp-0x50], dx       
WAIT                          
FILD      [bp-0x52]           
WAIT                          
FMULP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
PUSH      [bp-idmDst]               ; [bp-0x4]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_bc07:                             ; battle.c:4382
JMP       L_be65              

L_bc0a:                             ; battle.c:4384
SUB       [bp-idmSrc], 0x0002       ; [bp-0x20], 0x0002
                                    ; battle.c:4385
SUB       [bp-idmDst], 0x0002       ; [bp-0x4], 0x0002
                                    ; battle.c:4387
WAIT                          
FLD       [0x1db6]            
WAIT                          
FLD       [bp-pctSuccess]           ; [bp-0x46]
NOP                           
WAIT                          
CALLF     __aFfcompp                ; void __aFfcompp()
JNZ       L_bca4              

L_bc27:                             ; battle.c:4390
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idmSrc]               ; [bp-0x20]
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4392
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
PUSH      [bp-idmDst]               ; [bp-0x4]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4394
JMP       L_be65              

L_bca4:                             ; battle.c:4396
ADD       [bp-idmSrc], 0x0005       ; [bp-0x20], 0x0005
                                    ; battle.c:4397
ADD       [bp-idmDst], 0x0005       ; [bp-0x4], 0x0005
                                    ; battle.c:4400
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
WAIT                          
FLD       [0x1db6]            
WAIT                          
FSUB      [bp-0x46]           
MOV       ax, 0x2710          
CWD       dx, ax              
MOV       [bp-0x52], ax       
MOV       [bp-0x50], dx       
WAIT                          
FILD      [bp-0x52]           
WAIT                          
FMULP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idmSrc]               ; [bp-0x20]
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4403
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
WAIT                          
FLD       [0x1db6]            
WAIT                          
FSUB      [bp-0x46]           
MOV       ax, 0x2710          
CWD       dx, ax              
MOV       [bp-0x52], ax       
MOV       [bp-0x50], dx       
WAIT                          
FILD      [bp-0x52]           
WAIT                          
FMULP     st(1), st           
CALLF     __ftol                    ; int32_t __ftol()
PUSH      ax                  
MOV       ax, [bp-cPPE]             ; ax, [bp-0x3a]
MOV       dx, [bp-0x38]       
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
PUSH      [bp-idmDst]               ; [bp-0x4]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_bd5e:                             ; battle.c:4407
JMP       L_be65              

L_bd61:
CMP       [bp-cKillPeople+0x2], 0x0000 ; [bp-0xc], 0x0000
JL        L_be65              

L_bd6a:
JG        L_bd78              

L_bd6f:
CMP       [bp-cKillPeople], 0x0000  ; [bp-0xe], 0x0000
JBE       L_be65              

L_bd78:                             ; battle.c:4409
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x2a], 0x0000
JL        L_bdc1              

L_bd85:
JG        L_bd94              

L_bd8a:
CMP       es:[bx+0x28], 0x0000
JBE       L_bdc1              

L_bd94:                             ; battle.c:4411
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_bda3              

L_bd9d:
MOV       ax, 0x0166          
JMP       L_bda6              

L_bda3:
MOV       ax, 0x0060          

L_bda6:
MOV       [bp-idmSrc], ax           ; [bp-0x20], ax
                                    ; battle.c:4412
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_bdb8              

L_bdb2:
MOV       ax, 0x0170          
JMP       L_bdbb              

L_bdb8:
MOV       ax, 0x006a          

L_bdbb:
MOV       [bp-idmDst], ax           ; [bp-0x4], ax
                                    ; battle.c:4414
JMP       L_bdeb              

L_bdc1:                             ; battle.c:4416
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_bdd0              

L_bdca:
MOV       ax, 0x017c          
JMP       L_bdd3              

L_bdd0:
MOV       ax, 0x008f          

L_bdd3:
MOV       [bp-idmSrc], ax           ; [bp-0x20], ax
                                    ; battle.c:4417
CMP       [bp-fMulti], 0x0000       ; [bp-0xa], 0x0000
JZ        L_bde5              

L_bddf:
MOV       ax, 0x017d          
JMP       L_bde8              

L_bde5:
MOV       ax, 0x0090          

L_bde8:
MOV       [bp-idmDst], ax           ; [bp-0x4], ax

L_bdeb:                             ; battle.c:4421
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idmSrc]               ; [bp-0x20]
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; battle.c:4423
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-cKillPeople]      ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
PUSH      ax                  
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0x36]
PUSH      es:[bx]             
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx]             
PUSH      [bp-idmDst]               ; [bp-0x4]
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_be65:                             ; battle.c:4426
LES       bx, [bp-lppl]             ; bx, [bp-0x30]
CMP       es:[bx+0x28], 0x0000
JNZ       L_af0b              

L_be72:
CMP       es:[bx+0x2a], 0x0000
JNZ       L_af0b              

L_be7c:                             ; battle.c:4427
PUSH      [bp-lppl+0x2]             ; [bp-0x2e]
PUSH      [bp-lppl]                 ; [bp-0x30]
CALLF     UninhabitPlanet           ; void UninhabitPlanet(PLANET *lppl)
ADD       sp, 0x0004          

L_be8a:                             ; battle.c:4428
JMP       L_af0b              

L_be8d:                             ; battle.c:4429
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


