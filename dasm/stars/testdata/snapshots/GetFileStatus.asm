; GetFileStatus  (file)
;   addr: 000f:4a60  len=97
;   sig:  void GetFileStatus(int16_t dt, int16_t iPlayer)
;   params:
;     int16_t          dt             [BP+0x6]
;     int16_t          iPlayer        [BP+0x8]
;
;   stats: blocks=0  labels=0

L_4a60:                             ; file.c:2189
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0004          
PUSH      si                  
PUSH      di                  
                                    ; file.c:2190
PUSH      [bp+iPlayer]              ; [bp+0x8]
MOV       ax, [bp+dt]               ; ax, [bp+0x6]
PUSH      ax                  
CALLF     SetSzWorkFromDt           ; void SetSzWorkFromDt(DtFileType dt, int16_t iPlayer)
ADD       sp, 0x0004          
                                    ; file.c:2191
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     access                    ; int16_t access(char *path, int16_t mode)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_4a96              

L_4a90:
MOV       ax, 0x0001          
JMP       L_4a99              

L_4a96:
MOV       ax, 0x0000          

L_4a99:
MOV       [bp-0x4], ax        
MOV       ax, [bp-0x4]        
AND       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [gd+0x2]              ; cx, [0x07cc]
AND       cx, 0xffdf          
OR        cx, ax              
MOV       [gd+0x2], cx              ; [0x07cc], cx
MOV       ax, cx              
                                    ; file.c:2192
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


