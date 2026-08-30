; LpscannerFromId  (parts)
;   addr: 0002:518a  len=33
;   sig:  SCANNER * LpscannerFromId(int16_t id)
;   params:
;     int16_t          id             [BP+0x6]
;
;   stats: blocks=0  labels=0

L_518a:                             ; parts.c:38
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; parts.c:40
MOV       ax, 0x0038          
IMUL      [bp+id]                   ; [bp+0x6]
MOV       cx, 0x0768          
MOV       dx, cs              
ADD       cx, ax              
MOV       ax, cx              

L_51a5:                             ; parts.c:41
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


