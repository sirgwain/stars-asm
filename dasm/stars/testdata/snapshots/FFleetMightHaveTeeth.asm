; FFleetMightHaveTeeth  (aiutil)
;   addr: 0013:6152  len=139
;   sig:  int16_t FFleetMightHaveTeeth(FLEET *lpfl)
;   params:
;     FLEET *          lpfl           [BP+0x6]
;   locals:
;     int16_t          ishdef         [BP-0x8]
;     HUL *            lphul          [BP-0x6]
;
;   stats: blocks=0  labels=0

L_6152:                             ; aiutil.c:2580
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0008          
PUSH      si                  
PUSH      di                  
                                    ; aiutil.c:2584
MOV       [bp-ishdef], 0x0000       ; [bp-0x8], 0x0000
JMP       L_61c8              

L_6163:                             ; aiutil.c:2586
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-ishdef]           ; ax, [bp-0x8]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JZ        L_61c4              

L_6180:                             ; aiutil.c:2588
MOV       ax, 0x0093          
IMUL      [bp-ishdef]               ; [bp-0x8]
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       bx, es:[bx]         
MOV       cx, 0x0009          
SHR       bx, cx              
AND       bx, 0x000f          
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
MOV       [bp-lphul], cx            ; [bp-0x6], cx
MOV       [bp-lphul+0x2], dx        ; [bp-0x4], dx
                                    ; aiutil.c:2589
PUSH      [bp-lphul+0x2]            ; [bp-0x4]
PUSH      [bp-lphul]                ; [bp-0x6]
CALLF     FHullHasTeeth             ; int16_t FHullHasTeeth(HUL *lphul)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_61c4              

L_61be:                             ; aiutil.c:2590
MOV       ax, 0x0001          
JMP       L_61d7              

L_61c4:                             ; aiutil.c:2592
ADD       [bp-ishdef], 0x0001       ; [bp-0x8], 0x0001

L_61c8:
CMP       [bp-ishdef], 0x0010       ; [bp-0x8], 0x0010
JL        L_6163              

L_61d1:                             ; aiutil.c:2594
MOV       ax, 0x0000          

L_61d7:                             ; aiutil.c:2595
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


