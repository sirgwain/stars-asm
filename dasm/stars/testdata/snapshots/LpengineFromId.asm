; LpengineFromId  (parts)
;   addr: 0002:5168  len=33
;   sig:  ENGINE * LpengineFromId(int16_t id)
;   params:
;     int16_t          id             [BP+0x6]
;
;   stats: blocks=0  labels=0

L_5168:                             ; parts.c:32
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; parts.c:34
MOV       ax, 0x004e          
IMUL      [bp+id]                   ; [bp+0x6]
MOV       cx, 0x0000          
MOV       dx, cs              
ADD       cx, ax              
MOV       ax, cx              

L_5183:                             ; parts.c:35
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


