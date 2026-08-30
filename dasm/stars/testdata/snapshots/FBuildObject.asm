; FBuildObject  (turn2)
;   addr: 0018:19b2  len=5667
;   sig:  int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals)
;   params:
;     PLANET *         lppl           [BP+0x6]
;     GrobjClass       grobj          [BP+0xa]
;     int16_t          iItem          [BP+0xc]
;     int16_t          cBuilt         [BP+0xe]
;     int32_t *        rgMinerals     [BP+0x10]
;   locals:
;     int16_t          cAllowed       [BP-0x14]
;     SHDEF *          lpshdef        [BP-0x12]
;     int16_t          fTwoMAs        [BP-0xe]
;     MessageId        idm            [BP-0xc]
;     FLEET *          lpfl           [BP-0xa]
;     int16_t          i              [BP-0x6]
;     int16_t          iWarp          [BP-0x4]
;     block 0018:1D3A  len=0x462
;       uint16_t         dpShdef        [BP-0x1e]
;       int16_t          cshOrig        [BP-0x1c]
;       int16_t          cshDamaged     [BP-0x1a]
;       int32_t          dpOrig         [BP-0x18]
;     block 0018:2681  len=0x581
;       int16_t[3]       rgwt           [BP-0x2c]
;       int16_t          cSize          [BP-0x26]
;       int16_t          iWarpAsked     [BP-0x24]
;       int16_t          raMajor        [BP-0x22]
;       THING *          lpth           [BP-0x20]
;       int16_t          iDecayRate     [BP-0x1c]
;       int16_t          iWarp          [BP-0x1a]
;       THING *          lpthMac        [BP-0x18]
;       block 0018:2785  len=0x4B
;         int32_t          l              [BP-0x30]
;     block 0018:2DD5  len=0xE6
;       int16_t          iEnv           [BP-0x16]
;     block 0018:2EC1  len=0x28
;       PART             part           [BP-0x1c]
;
;   stats: blocks=5  labels=1
;     SendMsgFactMine: L_248b

L_19b2:                             ; turn2.c:519
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0030          
PUSH      si                  
PUSH      di                  
                                    ; turn2.c:530
CMP       [bp+grobj], 0x0002        ; [bp+0xa], 0x0002
JNZ       L_23c7              

L_19c4:                             ; turn2.c:532
CMP       [bp+iItem], 0x0010        ; [bp+0xc], 0x0010
JL        L_1c78              

L_19cd:                             ; turn2.c:534
SUB       [bp+iItem], 0x0010        ; [bp+0xc], 0x0010
                                    ; turn2.c:536
MOV       ax, 0x0093          
IMUL      [bp+iItem]                ; [bp+0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14c]      
MOV       dx, [bx+0x14e]      
ADD       cx, ax              
MOV       [bp-lpshdef], cx          ; [bp-0x12], cx
MOV       [bp-lpshdef+0x2], dx      ; [bp-0x10], dx
                                    ; turn2.c:538
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
MOV       ax, es:[bx+0x7b]    
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1a26              

L_1a09:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp-lpshdef+0x2]          ; [bp-0x10]
PUSH      [bp-lpshdef]              ; [bp-0x12]
CALLF     FCanBuildShdef            ; int16_t FCanBuildShdef(SHDEF *lpshdef, int16_t iplr)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JNZ       L_1a2c              

L_1a26:                             ; turn2.c:539
MOV       ax, 0x0000          
JMP       L_2fcf              

L_1a2c:                             ; turn2.c:542
MOV       [bp-idm], 0x00cd          ; [bp-0xc], 0x00cd
                                    ; turn2.c:543
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
CMP       es:[bx+0x34], 0x0000
JZ        L_1a60              

L_1a3e:                             ; turn2.c:545
ADD       [bp-idm], 0x0001          ; [bp-0xc], 0x0001
                                    ; turn2.c:546
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
MOV       ax, es:[bx+0x34]    
MOV       dx, 0x0000          
CMP       ax, 0xffff          
JNZ       L_1a60              

L_1a54:
CMP       dx, 0x0000          
JNZ       L_1a60              

L_1a5c:                             ; turn2.c:547
ADD       [bp-idm], 0x0001          ; [bp-0xc], 0x0001

L_1a60:                             ; turn2.c:551
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
PUSH      es:[bx]             
CALLF     LphuldefFromId            ; HULDEF * LphuldefFromId(int16_t id)
ADD       sp, 0x0002          
MOV       bx, ax              
MOV       es, dx              
MOV       ax, es:[bx+0x34]    
PUSH      ax                  
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
ADD       ax, 0x0010          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       dx, es:[bx+0x2]     
SHL       dx, 0x0001          
SHL       dx, 0x0001          
SHL       dx, 0x0001          
SHL       dx, 0x0001          
SHL       dx, 0x0001          
OR        dx, ax              
PUSH      dx                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
PUSH      [bp-idm]                  ; [bp-0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; turn2.c:555
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1b3a              

L_1ad6:
MOV       ax, 0x0093          
IMUL      [bp+iItem]                ; [bp+0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x2c]    
MOV       [bp-0x16], ax       
MOV       ax, cx              
AND       ax, 0x000f          
MOV       cx, 0x0093          
IMUL      cx                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       ax, [bp-0x16]       
MOV       es, cx              
CMP       es:[bx], ax         
JLE       L_1b3a              

L_1b2c:                             ; turn2.c:556
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     KillQueuedShips           ; void KillQueuedShips(PLANET *lppl)
ADD       sp, 0x0004          

L_1b3a:                             ; turn2.c:558
LEA       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
PUSH      ax                  
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     IWarpMAFromLppl           ; int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo)
ADD       sp, 0x0006          
MOV       [bp-iWarp], ax            ; [bp-0x4], ax
                                    ; turn2.c:560
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1b9b              

L_1b66:                             ; turn2.c:561
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2c]    
AND       ax, 0x000f          
MOV       cx, 0x0093          
IMUL      cx                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
SUB       es:[bx+0x83], 0x0001
SBB       es:[bx+0x85], 0x0000
                                    ; turn2.c:562
JMP       L_1baf              

L_1b9b:                             ; turn2.c:563
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
AND       ax, 0xfdff          
OR        ax, 0x0200          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x4], ax     

L_1baf:                             ; turn2.c:564
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
AND       ax, 0x000f          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x2c]    
AND       cx, 0xfff0          
OR        cx, ax              
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x2c], cx    
                                    ; turn2.c:566
CMP       [bp-iWarp], 0x0000        ; [bp-0x4], 0x0000
JG        L_1c55              

L_1bd1:                             ; turn2.c:568
LEA       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
PUSH      ax                  
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     IWarpMAFromLppl           ; int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo)
ADD       sp, 0x0006          
MOV       [bp-iWarp], ax            ; [bp-0x4], ax
                                    ; turn2.c:569
CMP       [bp-iWarp], 0x0000        ; [bp-0x4], 0x0000
JLE       L_1c1f              

L_1bef:                             ; turn2.c:570
MOV       ax, [bp-iWarp]            ; ax, [bp-0x4]
ADD       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
ADD       ax, 0xfffc          
MOV       [bp-0x16], ax       
MOV       ax, [bp-0x16]       
AND       ax, 0x000f          
MOV       cx, 0x000a          
SHL       ax, cx              
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x2e]    
AND       cx, 0xc3ff          
OR        cx, ax              
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x2e], cx    
MOV       ax, cx              
                                    ; turn2.c:571
JMP       L_1c55              

L_1c1f:                             ; turn2.c:573
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0xc3ff          
OR        ax, 0x0000          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x2e], ax    
                                    ; turn2.c:574
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0xfc00          
OR        ax, 0x0000          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x2e], ax    
                                    ; turn2.c:575
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     KillQueuedMassPackets     ; void KillQueuedMassPackets(PLANET *lppl)
ADD       sp, 0x0004          

L_1c55:                             ; turn2.c:579
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
ADD       es:[bx+0x7f], 0x0001
ADC       es:[bx+0x81], 0x0000
                                    ; turn2.c:580
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
ADD       es:[bx+0x83], 0x0001
ADC       es:[bx+0x85], 0x0000
                                    ; turn2.c:581
MOV       ax, 0x0001          
JMP       L_2fcf              

L_1c78:                             ; turn2.c:584
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1c98              

L_1c8f:
CMP       [bp+iItem], 0x0010        ; [bp+0xc], 0x0010
JL        L_1c9e              

L_1c98:                             ; turn2.c:587
MOV       ax, 0x0000          
JMP       L_2fcf              

L_1c9e:                             ; turn2.c:590
MOV       ax, 0x0093          
IMUL      [bp+iItem]                ; [bp+0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
MOV       [bp-lpshdef], cx          ; [bp-0x12], cx
MOV       [bp-lpshdef+0x2], dx      ; [bp-0x10], dx
                                    ; turn2.c:592
LES       bx, [bp-lpshdef]          ; bx, [bp-0x12]
MOV       ax, es:[bx+0x7b]    
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_1cf3              

L_1cd6:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp-lpshdef+0x2]          ; [bp-0x10]
PUSH      [bp-lpshdef]              ; [bp-0x12]
CALLF     FCanBuildShdef            ; int16_t FCanBuildShdef(SHDEF *lpshdef, int16_t iplr)
ADD       sp, 0x0006          
CMP       ax, 0x0000          
JNZ       L_1d1d              

L_1cf3:                             ; turn2.c:594
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
ADD       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x004f          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:595
MOV       ax, 0x0000          
JMP       L_2fcf              

L_1d1d:                             ; turn2.c:598
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, 0x00c0          
IMUL      es:[bx+0x2]         
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, [bx+0x4]        
AND       ax, 0x0fff          
CMP       ax, 0x0200          
JNZ       L_219c              

L_1d3a:                             ; turn2.c:605
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_1d46              

L_1d42:
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_1d46:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x6], ax
JGE       L_214f              

L_1d51:
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0xa], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x8], dx
CMP       ax, 0x0000          
JNZ       L_1d81              

L_1d79:
CMP       dx, 0x0000          
JZ        L_214f              

L_1d81:                             ; turn2.c:607
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
CMP       es:[bx+0x2], ax     
JG        L_214f              

L_1d91:
JMP       L_1d97              

L_1d94:                             ; turn2.c:608
JMP       L_214f              

L_1d97:                             ; turn2.c:609
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
CMP       es:[bx+0x2], ax     
JL        L_1d42              

L_1da7:
JMP       L_1dad              

L_1daa:                             ; turn2.c:610
JMP       L_1d42              

L_1dad:                             ; turn2.c:612
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx]         
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
LES       bx, es:[bx+0x64]    
CMP       es:[bx+0x4], ax     
JNZ       L_1d42              

L_1dcb:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx]         
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x2f40          
ADD       bx, ax              
MOV       ax, [bx+0x2]        
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
LES       bx, es:[bx+0x64]    
CMP       es:[bx+0x6], ax     
JNZ       L_1d42              

L_1ded:
MOV       ax, 0x7ffe          
SUB       ax, [bp+cBuilt]           ; ax, [bp+0xe]
MOV       cx, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, cx              
MOV       dx, [bp+iItem]            ; dx, [bp+0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
CMP       ax, es:[bx]         
JLE       L_1d42              

L_1e0f:                             ; turn2.c:614
MOV       ax, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JZ        L_20c5              

L_1e2c:
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0007          
SHR       ax, cx              
AND       ax, 0x01ff          
CMP       ax, 0x0000          
JZ        L_20c5              

L_1e53:                             ; turn2.c:617
MOV       ax, 0x0093          
IMUL      [bp+iItem]                ; [bp+0xc]
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x100]      
MOV       bx, [bx+0xfe]       
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x38]    
MOV       [bp-dpShdef], ax          ; [bp-0x1e], ax
                                    ; turn2.c:618
MOV       ax, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       [bp-cshOrig], ax          ; [bp-0x1c], ax
                                    ; turn2.c:619
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cshOrig]          ; ax, [bp-0x1c]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
AND       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cshDamaged], ax       ; [bp-0x1a], ax
                                    ; turn2.c:620
CMP       [bp-cshDamaged], 0x0000   ; [bp-0x1a], 0x0000
JNZ       L_1edb              

L_1ed6:                             ; turn2.c:621
MOV       [bp-cshDamaged], 0x0001   ; [bp-0x1a], 0x0001

L_1edb:                             ; turn2.c:622
MOV       ax, 0x0032          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cshDamaged]       ; ax, [bp-0x1a]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0007          
SHR       ax, cx              
AND       ax, 0x01ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-dpShdef]          ; ax, [bp-0x1e]
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-dpOrig], ax           ; [bp-0x18], ax
MOV       [bp-dpOrig+0x2], dx       ; [bp-0x16], dx
                                    ; turn2.c:623
MOV       ax, [bp-cshOrig]          ; ax, [bp-0x1c]
ADD       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cshDamaged]       ; ax, [bp-0x1a]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-0x20], ax       
MOV       ax, [bp-0x20]       
AND       ax, 0x007f          
MOV       cx, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, cx              
MOV       dx, [bp+iItem]            ; dx, [bp+0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       cx, es:[bx]         
AND       cx, 0xff80          
OR        cx, ax              
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, si              
MOV       es:[bx], cx         
MOV       ax, cx              
                                    ; turn2.c:624
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
AND       ax, 0x007f          
CMP       ax, 0x0000          
JNZ       L_1ff4              

L_1fc0:                             ; turn2.c:625
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
AND       ax, 0xff80          
OR        ax, 0x0001          
MOV       cx, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, cx              
MOV       dx, [bp+iItem]            ; dx, [bp+0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         

L_1ff4:                             ; turn2.c:626
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cshOrig]          ; ax, [bp-0x1c]
ADD       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
AND       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cshDamaged], ax       ; [bp-0x1a], ax
                                    ; turn2.c:627
CMP       [bp-cshDamaged], 0x0000   ; [bp-0x1a], 0x0000
JNZ       L_2041              

L_203c:                             ; turn2.c:628
MOV       [bp-cshDamaged], 0x0001   ; [bp-0x1a], 0x0001

L_2041:                             ; turn2.c:629
MOV       ax, [bp-dpShdef]          ; ax, [bp-0x1e]
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cshDamaged]       ; ax, [bp-0x1a]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0005          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-dpOrig+0x2]           ; [bp-0x16]
PUSH      [bp-dpOrig]               ; [bp-0x18]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-0x20], ax       
MOV       ax, [bp-0x20]       
AND       ax, 0x01ff          
MOV       cx, 0x0007          
SHL       ax, cx              
MOV       cx, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, cx              
MOV       dx, [bp+iItem]            ; dx, [bp+0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       cx, es:[bx]         
AND       cx, 0x007f          
OR        cx, ax              
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       si, [bp-lpfl+0x2]         ; si, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, si              
MOV       es:[bx], cx         
MOV       ax, cx              
                                    ; turn2.c:631
JMP       L_20de              

L_20c5:                             ; turn2.c:632
MOV       ax, 0x002c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x8]
ADD       bx, ax              
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       es:[bx], 0x0000     

L_20de:                             ; turn2.c:634
PUSH      [bp+cBuilt]               ; [bp+0xe]
PUSH      [bp+iItem]                ; [bp+0xc]
PUSH      [bp-lpfl+0x2]             ; [bp-0x8]
PUSH      [bp-lpfl]                 ; [bp-0xa]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     CreateShip                ; void CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip)
ADD       sp, 0x000a          
                                    ; turn2.c:635
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
PUSH      [bp+cBuilt]               ; [bp+0xe]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
MOV       ax, 0x0139          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; turn2.c:636
MOV       ax, 0x0001          
JMP       L_2fcf              

L_214c:                             ; turn2.c:639
JMP       L_1d42              

L_214f:                             ; turn2.c:641
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
PUSH      [bp+cBuilt]               ; [bp+0xe]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x00ba          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; turn2.c:642
MOV       ax, 0x0000          
JMP       L_2fcf              

L_219c:                             ; turn2.c:645
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     LpflNew                   ; FLEET * LpflNew(int16_t iPlr, int16_t idPl)
ADD       sp, 0x0004          
MOV       [bp-lpfl], ax             ; [bp-0xa], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x8], dx
                                    ; turn2.c:646
PUSH      [bp+cBuilt]               ; [bp+0xe]
PUSH      [bp+iItem]                ; [bp+0xc]
PUSH      [bp-lpfl+0x2]             ; [bp-0x8]
PUSH      [bp-lpfl]                 ; [bp-0xa]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     CreateShip                ; void CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip)
ADD       sp, 0x000a          
                                    ; turn2.c:647
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-lpfl+0x2]             ; [bp-0x8]
PUSH      [bp-lpfl]                 ; [bp-0xa]
CALLF     LGetFleetStat             ; int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat)
ADD       sp, 0x0006          
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       es:[bx+0x5c], ax    
MOV       es:[bx+0x5e], dx    
                                    ; turn2.c:649
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x30]    
AND       ax, 0x03ff          
CMP       ax, 0x0000          
JZ        L_2321              

L_2201:                             ; turn2.c:651
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
PUSH      [bp-lpfl+0x2]             ; [bp-0x8]
PUSH      [bp-lpfl]                 ; [bp-0xa]
CALLF     AutoRouteFleet            ; void AutoRouteFleet(FLEET *lpfl, PLANET *lppl)
ADD       sp, 0x0008          
                                    ; turn2.c:652
CMP       [bp+cBuilt], 0x0001       ; [bp+0xe], 0x0001
JNZ       L_22a0              

L_221e:                             ; turn2.c:654
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
LES       bx, es:[bx+0x64]    
MOV       ax, es:[bx+0x1c]    
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x000f          
CMP       ax, 0x0000          
JNZ       L_2242              

L_223c:
MOV       ax, 0x0033          
JMP       L_2245              

L_2242:
MOV       ax, 0x0031          

L_2245:
MOV       [bp-idm], ax              ; [bp-0xc], ax
                                    ; turn2.c:656
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x30]    
AND       ax, 0x03ff          
ADD       ax, 0xffff          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idm]                  ; [bp-0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; turn2.c:658
JMP       L_2fc9              

L_22a0:                             ; turn2.c:660
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
LES       bx, es:[bx+0x64]    
MOV       ax, es:[bx+0x1c]    
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x000f          
CMP       ax, 0x0000          
JNZ       L_22c4              

L_22be:
MOV       ax, 0x0034          
JMP       L_22c7              

L_22c4:
MOV       ax, 0x0032          

L_22c7:
MOV       [bp-idm], ax              ; [bp-0xc], ax
                                    ; turn2.c:662
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x30]    
AND       ax, 0x03ff          
ADD       ax, 0xffff          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
PUSH      [bp+cBuilt]               ; [bp+0xe]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
PUSH      [bp-idm]                  ; [bp-0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_231e:                             ; turn2.c:665
JMP       L_2fc9              

L_2321:                             ; turn2.c:667
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
PUSH      [bp-lpfl+0x2]             ; [bp-0x8]
PUSH      [bp-lpfl]                 ; [bp-0xa]
CALLF     AutoFleetOrder            ; void AutoFleetOrder(FLEET *lpfl, PLANET *lppl)
ADD       sp, 0x0008          
                                    ; turn2.c:668
CMP       [bp+cBuilt], 0x0001       ; [bp+0xe], 0x0001
JNZ       L_2379              

L_233e:                             ; turn2.c:671
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
MOV       ax, 0x002f          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:673
JMP       L_2fc9              

L_2379:                             ; turn2.c:676
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, [bp+iItem]            ; ax, [bp+0xc]
PUSH      ax                  
PUSH      [bp+cBuilt]               ; [bp+0xe]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp-lpfl]             ; bx, [bp-0xa]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
PUSH      ax                  
MOV       ax, 0x0030          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_23c4:                             ; turn2.c:680
JMP       L_2fc9              

L_23c7:
CMP       [bp+grobj], 0x0001        ; [bp+0xa], 0x0001
JNZ       L_2fc3              

L_23d0:                             ; turn2.c:682
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
JMP       L_2f77              

L_23d6:                             ; turn2.c:686
MOV       ax, 0x0000          
JMP       L_2fcf              
                                    ; turn2.c:691
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0014          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x16], ax       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxFactories             ; int16_t CMaxFactories(PLANET *lppl, int16_t iplr)
ADD       sp, 0x0006          
MOV       cx, [bp-0x16]       
SUB       ax, cx              
MOV       [bp-cAllowed], ax         ; [bp-0x14], ax
                                    ; turn2.c:692
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]
CMP       [bp+cBuilt], ax           ; [bp+0xe], ax
JGE       L_2426              

L_2420:
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
JMP       L_2429              

L_2426:
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]

L_2429:
MOV       [bp+cBuilt], ax           ; [bp+0xe], ax
                                    ; turn2.c:693
CMP       [bp+cBuilt], 0x0000       ; [bp+0xe], 0x0000
JLE       L_24fc              

L_2435:                             ; turn2.c:695
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
MOV       cx, 0x0014          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
ADD       ax, es:[bx+0x14]    
ADC       dx, es:[bx+0x16]    
AND       ax, 0x0000          
AND       dx, 0xfff0          
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
AND       es:[bx+0x14], 0xffff
AND       es:[bx+0x16], 0x000f
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, [bp-0x18]       
MOV       dx, [bp-0x16]       
OR        es:[bx+0x14], ax    
OR        es:[bx+0x16], dx    
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
                                    ; turn2.c:696
MOV       [bp-idm], 0x0035          ; [bp-0xc], 0x0035

SendMsgFactMine:                    ; turn2.c:701
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
PUSH      [bp-idm]                  ; [bp-0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FRemovePlayerMessage      ; int16_t FRemovePlayerMessage(int16_t iPlr, int16_t iMsg, int16_t iObj)
ADD       sp, 0x0006          
ADD       [bp+cBuilt], ax           ; [bp+0xe], ax
                                    ; turn2.c:703
CMP       [bp+cBuilt], 0x0001       ; [bp+0xe], 0x0001
JLE       L_24d7              

L_24af:                             ; turn2.c:706
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
PUSH      [bp+cBuilt]               ; [bp+0xe]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, [bp-idm]              ; ax, [bp-0xc]
ADD       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:708
JMP       L_2502              

L_24d7:                             ; turn2.c:711
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
PUSH      [bp-idm]                  ; [bp-0xc]
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          

L_24f9:                             ; turn2.c:714
JMP       L_2502              

L_24fc:                             ; turn2.c:715
MOV       ax, 0x0000          
JMP       L_2fcf              

L_2502:                             ; turn2.c:716
JMP       L_2fc0              
                                    ; turn2.c:720
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
MOV       cx, 0x0008          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x16], ax       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxMines                 ; int16_t CMaxMines(PLANET *lppl, int16_t iplr)
ADD       sp, 0x0006          
MOV       cx, [bp-0x16]       
SUB       ax, cx              
MOV       [bp-cAllowed], ax         ; [bp-0x14], ax
                                    ; turn2.c:721
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]
CMP       [bp+cBuilt], ax           ; [bp+0xe], ax
JGE       L_254f              

L_2549:
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
JMP       L_2552              

L_254f:
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]

L_2552:
MOV       [bp+cBuilt], ax           ; [bp+0xe], ax
                                    ; turn2.c:722
CMP       [bp+cBuilt], 0x0000       ; [bp+0xe], 0x0000
JLE       L_25bb              

L_255e:                             ; turn2.c:724
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
MOV       cx, 0x0008          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
ADD       ax, es:[bx+0x14]    
ADC       dx, es:[bx+0x16]    
AND       ax, 0xff00          
AND       dx, 0x000f          
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
AND       es:[bx+0x14], 0x00ff
AND       es:[bx+0x16], 0xfff0
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, [bp-0x18]       
MOV       dx, [bp-0x16]       
OR        es:[bx+0x14], ax    
OR        es:[bx+0x16], dx    
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
                                    ; turn2.c:725
MOV       [bp-idm], 0x0037          ; [bp-0xc], 0x0037
                                    ; turn2.c:726
JMP       SendMsgFactMine     

L_25bb:                             ; turn2.c:729
MOV       ax, 0x0000          
JMP       L_2fcf              
                                    ; turn2.c:735
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x16], ax       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     CMaxDefenses              ; int16_t CMaxDefenses(PLANET *lppl, int16_t iplr)
ADD       sp, 0x0006          
MOV       cx, [bp-0x16]       
SUB       ax, cx              
MOV       [bp-cAllowed], ax         ; [bp-0x14], ax
                                    ; turn2.c:736
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]
CMP       [bp+cBuilt], ax           ; [bp+0xe], ax
JGE       L_2606              

L_2600:
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
JMP       L_2609              

L_2606:
MOV       ax, [bp-cAllowed]         ; ax, [bp-0x14]

L_2609:
MOV       [bp+cBuilt], ax           ; [bp+0xe], ax
                                    ; turn2.c:737
CMP       [bp+cBuilt], 0x0000       ; [bp+0xe], 0x0000
JLE       L_2672              

L_2615:                             ; turn2.c:739
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
MOV       cx, 0x0000          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
ADD       ax, es:[bx+0x18]    
ADC       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
AND       es:[bx+0x18], 0xf000
AND       es:[bx+0x1a], 0xffff
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, [bp-0x18]       
MOV       dx, [bp-0x16]       
OR        es:[bx+0x18], ax    
OR        es:[bx+0x1a], dx    
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
                                    ; turn2.c:740
MOV       [bp-idm], 0x0039          ; [bp-0xc], 0x0039
                                    ; turn2.c:741
JMP       SendMsgFactMine     

L_2672:                             ; turn2.c:744
MOV       ax, 0x0000          
JMP       L_2fcf              
                                    ; turn2.c:748
JMP       L_2fc0              
                                    ; turn2.c:753
JMP       L_2fc0              
                                    ; turn2.c:766
MOV       ax, 0x000e          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, 0x00c0          
IMUL      es:[bx+0x2]         
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
MOV       [bp-raMajor], ax          ; [bp-0x22], ax
                                    ; turn2.c:769
LEA       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
PUSH      ax                  
PUSH      [bp+lppl+0x2]             ; [bp+0x8]
PUSH      [bp+lppl]                 ; [bp+0x6]
CALLF     IWarpMAFromLppl           ; int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo)
ADD       sp, 0x0006          
MOV       [bp-iWarp], ax            ; [bp-0x1a], ax
                                    ; turn2.c:770
CMP       [bp-iWarp], 0x0000        ; [bp-0x1a], 0x0000
JNZ       L_26e7              

L_26be:                             ; turn2.c:772
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x00d1          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:773
MOV       ax, 0x0000          
JMP       L_2fcf              

L_26e7:                             ; turn2.c:775
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0x03ff          
CMP       ax, 0x0000          
JNZ       L_2722              

L_26f9:                             ; turn2.c:777
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x00d2          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:778
MOV       ax, 0x0000          
JMP       L_2fcf              

L_2722:                             ; turn2.c:781
CMP       [bp+iItem], 0x0006        ; [bp+0xc], 0x0006
JNZ       L_2730              

L_272b:                             ; turn2.c:782
MOV       [bp+iItem], 0x0011        ; [bp+0xc], 0x0011

L_2730:                             ; turn2.c:784
CMP       [bp+iItem], 0x0011        ; [bp+0xc], 0x0011
JNZ       L_2751              

L_2739:                             ; turn2.c:785
CMP       [bp-raMajor], 0x0006      ; [bp-0x22], 0x0006
JNZ       L_2748              

L_2742:
MOV       ax, 0x0019          
JMP       L_274b              

L_2748:
MOV       ax, 0x0028          

L_274b:
MOV       [bp-cSize], ax            ; [bp-0x26], ax
                                    ; turn2.c:786
JMP       L_2766              

L_2751:                             ; turn2.c:787
CMP       [bp-raMajor], 0x0006      ; [bp-0x22], 0x0006
JNZ       L_2760              

L_275a:
MOV       ax, 0x0046          
JMP       L_2763              

L_2760:
MOV       ax, 0x0064          

L_2763:
MOV       [bp-cSize], ax            ; [bp-0x26], ax

L_2766:                             ; turn2.c:789
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_27e5              

L_276e:                             ; turn2.c:790
MOV       ax, [bp+iItem]            ; ax, [bp+0xc]
ADD       ax, 0xfff2          
CMP       [bp-i], ax                ; [bp-0x6], ax
JZ        L_2785              

L_277c:
CMP       [bp+iItem], 0x0011        ; [bp+0xc], 0x0011
JNZ       L_27d3              

L_2785:                             ; turn2.c:792
MOV       ax, [bp+cBuilt]           ; ax, [bp+0xe]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cSize]            ; ax, [bp-0x26]
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       [bp-l], ax                ; [bp-0x30], ax
MOV       [bp-l+0x2], dx            ; [bp-0x2e], dx
                                    ; turn2.c:793
CMP       [bp-l+0x2], 0x0000        ; [bp-0x2e], 0x0000
JL        L_27be              

L_27a5:
JG        L_27b4              

L_27aa:
CMP       [bp-l], 0x7ff8            ; [bp-0x30], 0x7ff8
JBE       L_27be              

L_27b4:                             ; turn2.c:794
MOV       [bp-l], 0x7ff8            ; [bp-0x30], 0x7ff8
MOV       [bp-l+0x2], 0x0000        ; [bp-0x2e], 0x0000

L_27be:                             ; turn2.c:795
MOV       ax, [bp-l]                ; ax, [bp-0x30]
MOV       dx, [bp-0x2e]       
MOV       dx, [bp-i]                ; dx, [bp-0x6]
SHL       dx, 0x0001          
LEA       bx, [bp-0x2c]       
ADD       bx, dx              
MOV       [bx], ax            
                                    ; turn2.c:797
JMP       L_27e1              

L_27d3:                             ; turn2.c:798
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2c]       
ADD       bx, ax              
MOV       [bx], 0x0000        

L_27e1:
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_27e5:
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JL        L_276e              

L_27ee:                             ; turn2.c:800
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
MOV       cx, 0x000a          
SHR       ax, cx              
AND       ax, 0x000f          
ADD       ax, 0x0004          
MOV       [bp-iWarpAsked], ax       ; [bp-0x24], ax
                                    ; turn2.c:801
CMP       [bp-iWarpAsked], 0x0005   ; [bp-0x24], 0x0005
JL        L_281a              

L_280c:
MOV       ax, [bp-iWarp]            ; ax, [bp-0x1a]
ADD       ax, 0x0003          
CMP       [bp-iWarpAsked], ax       ; [bp-0x24], ax
JLE       L_2823              

L_281a:                             ; turn2.c:802
MOV       ax, [bp-iWarp]            ; ax, [bp-0x1a]
ADD       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
MOV       [bp-iWarpAsked], ax       ; [bp-0x24], ax

L_2823:                             ; turn2.c:803
MOV       ax, [bp-iWarp]            ; ax, [bp-0x1a]
ADD       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
CMP       [bp-iWarpAsked], ax       ; [bp-0x24], ax
JG        L_2839              

L_2831:                             ; turn2.c:804
MOV       [bp-iDecayRate], 0x0000   ; [bp-0x1c], 0x0000
                                    ; turn2.c:805
JMP       L_2845              

L_2839:                             ; turn2.c:806
MOV       ax, [bp-iWarpAsked]       ; ax, [bp-0x24]
SUB       ax, [bp-iWarp]            ; ax, [bp-0x1a]
SUB       ax, [bp-fTwoMAs]          ; ax, [bp-0xe]
MOV       [bp-iDecayRate], ax       ; [bp-0x1c], ax

L_2845:                             ; turn2.c:808
CMP       [bp-raMajor], 0x0007      ; [bp-0x22], 0x0007
JNZ       L_285b              

L_284e:
CMP       [bp-iDecayRate], 0x0003   ; [bp-0x1c], 0x0003
JGE       L_285b              

L_2857:                             ; turn2.c:809
ADD       [bp-iDecayRate], 0x0001   ; [bp-0x1c], 0x0001

L_285b:                             ; turn2.c:811
MOV       ax, [bp-iWarpAsked]       ; ax, [bp-0x24]
ADD       ax, 0xfffc          
MOV       [bp-iWarp], ax            ; [bp-0x1a], ax
                                    ; turn2.c:813
MOV       ax, [lpThings]            ; ax, [0x1698]
MOV       dx, [lpThings+0x2]        ; dx, [0x169a]
MOV       [bp-lpth], ax             ; [bp-0x20], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0x1e], dx
MOV       [bp-0x30], ax       
MOV       [bp-0x2e], dx       
MOV       ax, 0x0012          
IMUL      [cThing]                  ; [0x169c]
MOV       cx, [bp-0x30]       
MOV       dx, [bp-0x2e]       
ADD       cx, ax              
MOV       [bp-lpthMac], cx          ; [bp-0x18], cx
MOV       [bp-lpthMac+0x2], dx      ; [bp-0x16], dx
JMP       L_295a              

L_288f:                             ; turn2.c:821
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x000f          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x2]     
CMP       ax, cx              
JNZ       L_2956              

L_28ab:
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx]         
MOV       cx, 0x000d          
SHR       ax, cx              
AND       ax, 0x0007          
CMP       ax, 0x0001          
JNZ       L_2956              

L_28c1:
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx]         
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x2f40          
ADD       bx, ax              
MOV       cx, ds              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
CMP       es:[bx+0x2], ax     
JNZ       L_2956              

L_28e7:
CMP       es:[bx+0x4], dx     
JNZ       L_2956              

L_28f0:
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx+0x6]     
MOV       cx, 0x000a          
SHR       ax, cx              
AND       ax, 0x000f          
MOV       cx, [bp-iWarp]            ; cx, [bp-0x1a]
CMP       ax, cx              
JNZ       L_2956              

L_2909:
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx+0x6]     
AND       ax, 0x03ff          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x2e]    
AND       cx, 0x03ff          
ADD       cx, 0xffff          
CMP       ax, cx              
JNZ       L_2956              

L_2928:
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx+0xe]     
MOV       cx, 0x000e          
SHR       ax, cx              
AND       ax, 0x0003          
MOV       cx, [bp-iDecayRate]       ; cx, [bp-0x1c]
CMP       ax, cx              
JNZ       L_2956              

L_2941:
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
CMP       ax, 0x065e          
JC        L_2968              

L_2950:
JMP       L_2956              

L_2953:                             ; turn2.c:822
JMP       L_2968              

L_2956:                             ; turn2.c:823
ADD       [bp-lpth], 0x0012         ; [bp-0x20], 0x0012

L_295a:
MOV       ax, [bp-lpthMac]          ; ax, [bp-0x18]
MOV       dx, [bp-0x16]       
CMP       [bp-lpth], ax             ; [bp-0x20], ax
JC        L_288f              

L_2968:                             ; turn2.c:825
MOV       ax, [bp-lpthMac]          ; ax, [bp-0x18]
MOV       dx, [bp-lpthMac+0x2]      ; dx, [bp-0x16]
CMP       [bp-lpth], ax             ; [bp-0x20], ax
JNZ       L_297e              

L_2976:
CMP       [bp-lpth+0x2], dx         ; [bp-0x1e], dx
JZ        L_2a78              

L_297e:                             ; turn2.c:827
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, es:[bx+0xe]     
AND       ax, 0xc000          
OR        ax, 0x0000          
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       es:[bx+0xe], ax     
                                    ; turn2.c:828
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_2a3f              

L_299a:                             ; turn2.c:830
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2c]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       cx, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       si, [bp-lpth+0x2]         ; si, [bp-0x1e]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0x6]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
ADD       es:[bx], ax         
                                    ; turn2.c:831
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0x1e]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JGE       L_29f3              

L_29da:                             ; turn2.c:832
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0x1e]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       es:[bx], 0x7ff8     

L_29f3:                             ; turn2.c:833
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0x1e]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
ADD       ax, 0x0009          
MOV       cx, 0x000a          
CWD       dx, ax              
IDIV      cx                  
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
ADD       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
MOV       [bp-0x2e], ax       
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
AND       es:[bx+0xe], 0xc000 
MOV       ax, es:[bx+0xe]     
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, [bp-0x2e]       
OR        es:[bx+0xe], ax     
MOV       ax, es:[bx+0xe]     
                                    ; turn2.c:834
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_2a3f:
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JL        L_299a              

L_2a48:                             ; turn2.c:836
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0x03ff          
ADD       ax, 0xffff          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x00d4          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:837
JMP       L_2fc9              

L_2a78:                             ; turn2.c:841
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     LpthNew                   ; THING * LpthNew(int16_t iplr, ThingType ith)
ADD       sp, 0x0004          
MOV       [bp-lpth], ax             ; [bp-0x20], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0x1e], dx
                                    ; turn2.c:842
CMP       [bp-lpth], 0x0000         ; [bp-0x20], 0x0000
JNZ       L_2ac9              

L_2a9a:
CMP       [bp-lpth+0x2], 0x0000     ; [bp-0x1e], 0x0000
JNZ       L_2ac9              

L_2aa3:                             ; turn2.c:844
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x0129          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:845
JMP       L_2fc9              

L_2ac9:                             ; turn2.c:848
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_2b35              

L_2ad1:                             ; turn2.c:850
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2c]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       cx, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       si, [bp-lpth+0x2]         ; si, [bp-0x1e]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0x6]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         
                                    ; turn2.c:851
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2c]       
ADD       bx, ax              
MOV       ax, [bx]            
ADD       ax, 0x0009          
MOV       cx, 0x000a          
CWD       dx, ax              
IDIV      cx                  
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
ADD       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
MOV       [bp-0x2e], ax       
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
AND       es:[bx+0xe], 0xc000 
MOV       ax, es:[bx+0xe]     
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       ax, [bp-0x2e]       
OR        es:[bx+0xe], ax     
MOV       ax, es:[bx+0xe]     
                                    ; turn2.c:852
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_2b35:
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JL        L_2ad1              

L_2b3e:                             ; turn2.c:853
MOV       ax, [bp-iWarp]            ; ax, [bp-0x1a]
MOV       [bp-0x2e], ax       
MOV       ax, [bp-0x2e]       
AND       ax, 0x000f          
MOV       cx, 0x000a          
SHL       ax, cx              
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, es:[bx+0x6]     
AND       cx, 0xc3ff          
OR        cx, ax              
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       es:[bx+0x6], cx     
MOV       ax, cx              
                                    ; turn2.c:854
MOV       ax, [bp-iDecayRate]       ; ax, [bp-0x1c]
MOV       [bp-0x2e], ax       
MOV       ax, [bp-0x2e]       
AND       ax, 0x0003          
MOV       cx, 0x000e          
SHL       ax, cx              
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, es:[bx+0xe]     
AND       cx, 0x3fff          
OR        cx, ax              
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       es:[bx+0xe], cx     
MOV       ax, cx              
                                    ; turn2.c:855
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0x03ff          
ADD       ax, 0xffff          
MOV       [bp-0x2e], ax       
MOV       ax, [bp-0x2e]       
AND       ax, 0x03ff          
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       cx, es:[bx+0x6]     
AND       cx, 0xfc00          
OR        cx, ax              
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       es:[bx+0x6], cx     
MOV       ax, cx              
                                    ; turn2.c:856
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       bx, es:[bx]         
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
MOV       dx, [bx+0x2f42]     
LES       bx, [bp-lpth]             ; bx, [bp-0x20]
MOV       es:[bx+0x2], ax     
MOV       es:[bx+0x4], dx     
                                    ; turn2.c:859
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x2e]    
AND       ax, 0x03ff          
ADD       ax, 0xffff          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x00d3          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
                                    ; turn2.c:861
JMP       L_2fc0              
                                    ; turn2.c:864
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_2c11              

L_2c0d:
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_2c11:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-i], ax                ; [bp-0x6], ax
JGE       L_2c3e              

L_2c1c:                             ; turn2.c:865
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
PUSH      es:[bx]             
MOV       ax, 0x011b          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0x6]
CALLF     FSendPlrMsg2              ; int16_t FSendPlrMsg2(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2)
ADD       sp, 0x000a          
JMP       L_2c0d              

L_2c3e:                             ; turn2.c:867
MOV       ax, 0x000e          
PUSH      ax                  
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, 0x00c0          
IMUL      es:[bx+0x2]         
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
CMP       ax, 0x0008          
JZ        L_2cea              

L_2c62:                             ; turn2.c:869
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
AND       ax, 0xffff          
AND       dx, 0x000f          
OR        ax, 0x0000          
OR        dx, 0x0000          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x14], ax    
MOV       es:[bx+0x16], dx    
                                    ; turn2.c:870
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x14]    
MOV       dx, es:[bx+0x16]    
AND       ax, 0x00ff          
AND       dx, 0xfff0          
OR        ax, 0x0000          
OR        dx, 0x0000          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x14], ax    
MOV       es:[bx+0x16], dx    
                                    ; turn2.c:871
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0xf000          
AND       dx, 0xffff          
OR        ax, 0x0000          
OR        dx, 0x0000          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x18], ax    
MOV       es:[bx+0x1a], dx    
                                    ; turn2.c:872
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x18]    
MOV       dx, es:[bx+0x1a]    
AND       ax, 0x0fff          
AND       dx, 0xfffe          
OR        ax, 0xf000          
OR        dx, 0x0001          
LES       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       es:[bx+0x18], ax    
MOV       es:[bx+0x1a], dx    

L_2cea:                             ; turn2.c:875
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_2d9c              

L_2cf2:                             ; turn2.c:877
MOV       ax, 0x001c          
MOV       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       cx, [bp+lppl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       es:[bx], 0x0000     
MOV       es:[bx+0x2], 0x0000 
                                    ; turn2.c:878
MOV       ax, 0x0032          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
MOV       [bp-0x16], ax       
MOV       ax, 0x0032          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
ADD       ax, 0x0001          
MOV       cx, [bp-0x16]       
ADD       ax, cx              
MOV       cx, 0x000f          
MOV       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       si, [bp+lppl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x6]
ADD       bx, cx              
MOV       es, si              
MOV       es:[bx], al         
MOV       cx, 0x000c          
MOV       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       si, [bp+lppl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x6]
ADD       bx, cx              
MOV       es, si              
MOV       es:[bx], al         
                                    ; turn2.c:879
MOV       ax, 0x0028          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
MOV       [bp-0x16], ax       
MOV       ax, 0x0028          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
ADD       ax, 0x0019          
MOV       cx, [bp-0x16]       
ADD       ax, cx              
MOV       cx, 0x0009          
MOV       bx, [bp+lppl]             ; bx, [bp+0x6]
MOV       si, [bp+lppl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x6]
ADD       bx, cx              
MOV       es, si              
MOV       es:[bx], al         
                                    ; turn2.c:880
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_2d9c:
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JGE       L_2fc9              

L_2da2:
JMP       L_2cf2              

L_2da5:                             ; turn2.c:881
JMP       L_2fc9              

L_2da8:                             ; turn2.c:886
MOV       ax, [bp+0xe]        
SUB       [bp+0xe], 0x0001    
CMP       ax, 0x0000          
JZ        L_2ebe              

L_2db7:                             ; turn2.c:888
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp+0x8]            
PUSH      [bp+0x6]            
CALLF     IBestTerraform            ; int16_t IBestTerraform(PLANET *lppl, int16_t fHelp)
ADD       sp, 0x0006          
MOV       [bp-0x6], ax        
                                    ; turn2.c:889
CMP       [bp-0x6], 0x0000    
JZ        L_2da8              

L_2dd5:                             ; turn2.c:891
PUSH      [bp-0x6]            
CALLF     abs                       ; int16_t abs(int16_t x)
ADD       sp, 0x0002          
ADD       ax, 0xffff          
MOV       [bp-0x16], ax       
                                    ; turn2.c:893
CMP       [bp-0x6], 0x0000    
JLE       L_2df5              

L_2def:
MOV       ax, 0x0001          
JMP       L_2df8              

L_2df5:
MOV       ax, 0xffff          

L_2df8:
MOV       cx, 0x000c          
MOV       bx, [bp+0x6]        
MOV       si, [bp+0x8]        
ADD       bx, cx              
MOV       cx, [bp-0x16]       
ADD       bx, cx              
MOV       es, si              
MOV       cl, es:[bx]         
MOV       [bp-0x18], ax       
MOV       ax, cx              
CBW       ax, al              
MOV       cx, [bp-0x18]       
ADD       ax, cx              
MOV       [bp-0x14], ax       
                                    ; turn2.c:894
MOV       ax, 0x0063          
CMP       ax, [bp-0x14]       
JGE       L_2e2c              

L_2e26:
MOV       ax, 0x0063          
JMP       L_2e2f              

L_2e2c:
MOV       ax, [bp-0x14]       

L_2e2f:
MOV       cx, 0x0001          
CMP       cx, ax              
JLE       L_2e3f              

L_2e39:
MOV       ax, 0x0001          
JMP       L_2e53              

L_2e3f:
MOV       ax, 0x0063          
CMP       ax, [bp-0x14]       
JGE       L_2e50              

L_2e4a:
MOV       ax, 0x0063          
JMP       L_2e53              

L_2e50:
MOV       ax, [bp-0x14]       

L_2e53:
MOV       [bp-0x14], ax       
                                    ; turn2.c:895
MOV       ax, [bp-0x14]       
MOV       cx, 0x000c          
MOV       bx, [bp+0x6]        
MOV       si, [bp+0x8]        
ADD       bx, cx              
MOV       cx, [bp-0x16]       
ADD       bx, cx              
MOV       es, si              
MOV       es:[bx], al         
                                    ; turn2.c:898
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       cx, 0x0008          
MOV       ax, [bp-0x16]       
SHL       ax, cx              
ADD       ax, [bp-0x14]       
PUSH      ax                  
PUSH      [bp-0x16]           
CMP       [bp-0x6], 0x0000    
JLE       L_2e98              

L_2e92:
MOV       ax, 0x0001          
JMP       L_2e9b              

L_2e98:
MOV       ax, 0x0000          

L_2e9b:
PUSH      ax                  
LES       bx, [bp+0x6]        
PUSH      es:[bx]             
LES       bx, [bp+0x6]        
PUSH      es:[bx]             
MOV       ax, 0x007b          
PUSH      ax                  
LES       bx, [bp+0x6]        
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          

L_2ebb:                             ; turn2.c:900
JMP       L_2da8              

L_2ebe:                             ; turn2.c:901
JMP       L_2fc0              
                                    ; turn2.c:908
LES       bx, [bp+0x6]        
MOV       ax, es:[bx+0x2]     
MOV       [0x018c], ax        
                                    ; turn2.c:909
LEA       ax, [bp-0x1c]       
PUSH      ax                  
CALLF     LookupBestPlanetaryScanner; void LookupBestPlanetaryScanner(PART *ppart)
ADD       sp, 0x0002          
                                    ; turn2.c:910
MOV       [0x018c], 0xffff    
                                    ; turn2.c:912
MOV       ax, [bp-0x1a]       
AND       ax, 0x00ff          
ADD       ax, 0x0012          
MOV       [bp+0xc], ax        
                                    ; turn2.c:926
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp+0xc]        
ADD       ax, 0xffee          
PUSH      ax                  
MOV       ax, 0x8000          
PUSH      ax                  
LES       bx, [bp+0x6]        
PUSH      es:[bx]             
LES       bx, [bp+0x6]        
PUSH      es:[bx]             
MOV       ax, 0x007c          
PUSH      ax                  
LES       bx, [bp+0x6]        
PUSH      es:[bx+0x2]         
CALLF     FSendPlrMsg               ; int16_t FSendPlrMsg(int16_t iPlr, int16_t iMsg, int16_t iObj, int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7)
ADD       sp, 0x0014          
                                    ; turn2.c:927
MOV       ax, [bp+0xc]        
ADD       ax, 0xffee          
CWD       dx, ax              
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
MOV       ax, [bp-0x18]       
MOV       dx, [bp-0x16]       
AND       ax, 0x001f          
AND       dx, 0x0000          
MOV       cx, 0x000c          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [bp+0x6]        
MOV       cx, es:[bx+0x18]    
MOV       bx, es:[bx+0x1a]    
AND       cx, 0x0fff          
AND       bx, 0xfffe          
OR        cx, ax              
OR        bx, dx              
MOV       [bp-0x1c], cx       
MOV       [bp-0x1a], bx       
LES       bx, [bp+0x6]        
MOV       ax, [bp-0x1c]       
MOV       dx, [bp-0x1a]       
MOV       es:[bx+0x18], ax    
MOV       es:[bx+0x1a], dx    
                                    ; turn2.c:928
JMP       L_2fc9              

L_2f77:
CMP       ax, 0x001b          
JA        L_23d6              

L_2f7f:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       L_ffffffff          
DW        0x2505                    ; L_2505
DW        0x23dc                    ; L_23dc
DW        0x25c4                    ; L_25c4
DW        0x267e                    ; L_267e
DW        0x2da8                    ; L_2da8
DW        0x2da8                    ; L_2da8
DW        0x2681                    ; L_2681
DW        0x23dc                    ; L_23dc
DW        0x2505                    ; L_2505
DW        0x25c4                    ; L_25c4
DW        0x267b                    ; L_267b
DW        0x267e                    ; L_267e
DW        0x2da8                    ; L_2da8
DW        0x2c05                    ; L_2c05
DW        0x2681                    ; L_2681
DW        0x2681                    ; L_2681
DW        0x2681                    ; L_2681
DW        0x2681                    ; L_2681
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ee9                    ; L_2ee9
DW        0x2ec1                    ; L_2ec1

L_2fc0:                             ; turn2.c:931
JMP       L_2fc9              

L_2fc3:                             ; turn2.c:934
MOV       ax, 0x0000          
JMP       L_2fcf              

L_2fc9:                             ; turn2.c:937
MOV       ax, 0x0001          

L_2fcf:                             ; turn2.c:938
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


