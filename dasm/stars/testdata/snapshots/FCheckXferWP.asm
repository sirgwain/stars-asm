; FCheckXferWP  (tutor)
;   addr: 0020:7280  len=450
;   sig:  int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal)
;   params:
;     uint16_t         ifl            [BP+0x6]
;     int16_t          iord           [BP+0x8]
;     int16_t          id             [BP+0xa]
;     uint16_t         iWarp          [BP+0xc]
;     ITEMACTION *     lpiaGoal       [BP+0xe]
;   locals:
;     int16_t          idhSav         [BP-0x24]
;     GrobjClass       grobj          [BP-0x22]
;     int16_t          idh            [BP-0x20]
;     FLEET *          lpfl           [BP-0x1e]
;     int16_t          i              [BP-0x1a]
;     ITEMACTION *     piaCur         [BP-0x18]
;     int16_t          fRet           [BP-0x16]
;     ORDER            ord            [BP-0x14]
;
;   stats: blocks=0  labels=1
;     LReturn: L_73fa

L_7280:                             ; tutor.c:3224
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0024          
PUSH      si                  
PUSH      di                  
                                    ; tutor.c:3229
MOV       [bp-fRet], 0x0000         ; [bp-0x16], 0x0000
                                    ; tutor.c:3231
MOV       ax, [tutor+0x6]           ; ax, [0x5212]
MOV       [bp-idhSav], ax           ; [bp-0x24], ax
                                    ; tutor.c:3234
MOV       ax, [bp+id]               ; ax, [bp+0xa]
AND       ax, 0x8000          
CMP       ax, 0x0000          
JZ        L_72af              

L_72a2:                             ; tutor.c:3236
AND       [bp+id], 0x7fff           ; [bp+0xa], 0x7fff
                                    ; tutor.c:3237
MOV       [bp-grobj], 0x0002        ; [bp-0x22], 0x0002
                                    ; tutor.c:3239
JMP       L_72b4              

L_72af:                             ; tutor.c:3240
MOV       [bp-grobj], 0x0001        ; [bp-0x22], 0x0001

L_72b4:                             ; tutor.c:3242
MOV       ax, [bp+ifl]              ; ax, [bp+0x6]
PUSH      ax                  
CALLF     LpflFromId                ; FLEET * LpflFromId(int16_t idFleet)
ADD       sp, 0x0002          
MOV       [bp-lpfl], ax             ; [bp-0x1e], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x1c], dx
                                    ; tutor.c:3243
CMP       [bp-lpfl], 0x0000         ; [bp-0x1e], 0x0000
JNZ       L_72de              

L_72cf:
CMP       [bp-lpfl+0x2], 0x0000     ; [bp-0x1c], 0x0000
JNZ       L_72de              

L_72d8:                             ; tutor.c:3244
MOV       ax, 0x0000          
JMP       L_743c              

L_72de:                             ; tutor.c:3245
PUSH      [bp+iWarp]                ; [bp+0xc]
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp+id]                   ; [bp+0xa]
MOV       ax, [bp-grobj]            ; ax, [bp-0x22]
PUSH      ax                  
PUSH      [bp+iord]                 ; [bp+0x8]
PUSH      [bp+ifl]                  ; [bp+0x6]
CALLF     FCheckFleetWP             ; int16_t FCheckFleetWP(uint16_t ifl, int16_t iord, GrobjClass grobj, int16_t id, uint16_t grTask, uint16_t iWarp)
ADD       sp, 0x000c          
CMP       ax, 0x0000          
JNZ       L_7308              

L_7302:                             ; tutor.c:3246
MOV       ax, 0x0000          
JMP       L_743c              

L_7308:                             ; tutor.c:3247
LES       bx, [bp-lpfl]             ; bx, [bp-0x1e]
MOV       si, es:[bx+0x64]    
MOV       cx, es:[bx+0x66]    
MOV       ax, 0x0004          
ADD       si, ax              
MOV       ax, 0x0012          
IMUL      [bp+iord]                 ; [bp+0x8]
ADD       si, ax              
PUSH      si                  
LEA       di, [bp-ord]              ; di, [bp-0x14]
PUSH      ss                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x0009          
MOVSW.REP es:[di], ds:[si]    
POP       ds                  
POP       si                  
LEA       ax, [bp-0x14]       
                                    ; tutor.c:3248
LEA       ax, [bp-ord+0x8]          ; ax, [bp-0xc]
MOV       [bp-piaCur], ax           ; [bp-0x18], ax
                                    ; tutor.c:3249
MOV       [tutor+0x6], 0x05ef       ; [0x5212], 0x05ef
                                    ; tutor.c:3251
MOV       [bp-i], 0x0000            ; [bp-0x1a], 0x0000
JMP       L_7361              

L_7347:
MOV       ax, [bp-0x1a]       
ADD       [bp-i], 0x0001            ; [bp-0x1a], 0x0001
MOV       ax, [bp-0x18]       
ADD       [bp-piaCur], 0x0002       ; [bp-0x18], 0x0002
MOV       ax, [bp+0xe]        
MOV       es, [bp+0x10]       
ADD       [bp+lpiaGoal], 0x0002     ; [bp+0xe], 0x0002
MOV       dx, es              

L_7361:
CMP       [bp-i], 0x0005            ; [bp-0x1a], 0x0005
JGE       L_73f5              

L_736a:                             ; tutor.c:3253
MOV       bx, [bp-piaCur]           ; bx, [bp-0x18]
MOV       ax, [bx]            
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x000f          
LES       bx, [bp+lpiaGoal]         ; bx, [bp+0xe]
MOV       dx, es:[bx]         
MOV       cx, 0x000c          
SHR       dx, cx              
AND       dx, 0x000f          
CMP       ax, dx              
JZ        L_73b0              

L_738c:                             ; tutor.c:3255
MOV       bx, [bp-piaCur]           ; bx, [bp-0x18]
MOV       ax, [bx]            
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x000f          
CMP       ax, 0x0000          
JZ        LReturn             

L_73a1:                             ; tutor.c:3256
MOV       ax, 0x0268          
PUSH      ax                  
CALLF     TutorError                ; void TutorError(int16_t idsError)
ADD       sp, 0x0002          

L_73ad:                             ; tutor.c:3257
JMP       LReturn             

L_73b0:                             ; tutor.c:3259
MOV       bx, [bp-piaCur]           ; bx, [bp-0x18]
MOV       ax, [bx]            
MOV       cx, 0x000c          
SHR       ax, cx              
AND       ax, 0x000f          
JMP       L_73df              

L_73c0:                             ; tutor.c:3263
MOV       bx, [bp-piaCur]           ; bx, [bp-0x18]
MOV       ax, [bx]            
AND       ax, 0x0fff          
LES       bx, [bp+lpiaGoal]         ; bx, [bp+0xe]
MOV       cx, es:[bx]         
AND       cx, 0x0fff          
CMP       ax, cx              
JNZ       LReturn             

L_73d6:
JMP       L_7347              

L_73df:
CMP       ax, 0x0004          
JZ        L_73c0              

L_73e7:
CMP       ax, 0x0008          
JNZ       L_7347              

L_73ec:
JMP       L_73c0              

L_73f5:                             ; tutor.c:3272
MOV       [bp-fRet], 0x0001         ; [bp-0x16], 0x0001

LReturn:                            ; tutor.c:3275
MOV       ax, [tutor+0x6]           ; ax, [0x5212]
MOV       [bp-idh], ax              ; [bp-0x20], ax
                                    ; tutor.c:3276
CMP       [bp-fRet], 0x0000         ; [bp-0x16], 0x0000
JNZ       L_7427              

L_7409:
MOV       ax, [bp+ifl]              ; ax, [bp+0x6]
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     FCheckSelection           ; int16_t FCheckSelection(GrobjClass grobj, int16_t id)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_7427              

L_7421:                             ; tutor.c:3277
MOV       ax, [bp-idh]              ; ax, [bp-0x20]
MOV       [tutor+0x6], ax           ; [0x5212], ax

L_7427:                             ; tutor.c:3279
CMP       [bp-fRet], 0x0000         ; [bp-0x16], 0x0000
JZ        L_7436              

L_7430:                             ; tutor.c:3280
MOV       ax, [bp-idhSav]           ; ax, [bp-0x24]
MOV       [tutor+0x6], ax           ; [0x5212], ax

L_7436:                             ; tutor.c:3281
MOV       ax, [bp-fRet]             ; ax, [bp-0x16]

L_743c:                             ; tutor.c:3282
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


