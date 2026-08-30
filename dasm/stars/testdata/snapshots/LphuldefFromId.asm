; LphuldefFromId  (parts)
;   addr: 0002:512c  len=60
;   sig:  HULDEF * LphuldefFromId(int16_t id)
;   params:
;     int16_t          id             [BP+0x6]
;
;   stats: blocks=0  labels=0

L_512c:                             ; parts.c:24
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; parts.c:26
CMP       [bp+id], 0x0020           ; [bp+0x6], 0x0020
JL        L_5150              

L_513e:                             ; parts.c:27
MOV       ax, [bp+id]               ; ax, [bp+0x6]
ADD       ax, 0xffe0          
PUSH      ax                  
CALLF     LphuldefSBFromId          ; HULDEF * LphuldefSBFromId(int16_t id)
ADD       sp, 0x0002          
JMP       L_5162              

L_5150:                             ; parts.c:28
MOV       ax, 0x008f          
IMUL      [bp+id]                   ; [bp+0x6]
MOV       cx, 0x29f0          
MOV       dx, cs              
ADD       cx, ax              
MOV       ax, cx              

L_5162:                             ; parts.c:29
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


