; WritePlayerMessages  (msg)
;   addr: 0007:9702  len=468
;   sig:  void WritePlayerMessages(int16_t iPlayer)
;   params:
;     int16_t          iPlayer        [BP+0x6]
;   locals:
;     uint8_t *        lpb            [BP-0x410]
;     MSGPLR *         lpmp           [BP-0x40c]
;     int16_t          cbMsg          [BP-0x408]
;     uint8_t[1024]    rgb            [BP-0x406]
;     uint8_t *        lpbMax         [BP-0x6]
;
;   stats: blocks=0  labels=0

L_9702:                             ; msg.c:1816
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0410          
PUSH      si                  
PUSH      di                  
                                    ; msg.c:1820
MOV       [bp-cbMsg], 0x0000        ; [bp-0x408], 0x0000
                                    ; msg.c:1823
CMP       [bp+iPlayer], 0xffff      ; [bp+0x6], 0xffff
JZ        L_98d0              

L_971d:                             ; msg.c:1826
MOV       ax, [lpMsg]               ; ax, [0x0af8]
MOV       dx, [lpMsg+0x2]           ; dx, [0x0afa]
MOV       [bp-lpb], ax              ; [bp-0x410], ax
MOV       [bp-lpb+0x2], dx          ; [bp-0x40e], dx
                                    ; msg.c:1827
MOV       ax, [imemMsgCur]          ; ax, [0x0afc]
MOV       cx, [bp-lpb]              ; cx, [bp-0x410]
MOV       dx, [bp-lpb+0x2]          ; dx, [bp-0x40e]
ADD       cx, ax              
MOV       [bp-lpbMax], cx           ; [bp-0x6], cx
MOV       [bp-lpbMax+0x2], dx       ; [bp-0x4], dx
                                    ; msg.c:1829
JMP       L_980d              

L_9742:                             ; msg.c:1831
MOV       ax, [bp-cbMsg]            ; ax, [bp-0x408]
ADD       ax, 0x0014          
CMP       ax, 0x0400          
JL        L_976f              

L_9751:                             ; msg.c:1833
LEA       ax, [bp-rgb]              ; ax, [bp-0x406]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cbMsg]                ; [bp-0x408]
MOV       ax, 0x000c          
PUSH      ax                  
CALLF     WriteRt                   ; void WriteRt(RecordType rt, int16_t cb, void *rg)
ADD       sp, 0x0008          
                                    ; msg.c:1834
MOV       [bp-cbMsg], 0x0000        ; [bp-0x408], 0x0000

L_976f:                             ; msg.c:1838
LES       bx, [bp-lpb]              ; bx, [bp-0x410]
MOV       al, es:[bx]         
AND       ax, 0x00ff          
AND       ax, 0x000f          
CMP       ax, [bp+iPlayer]          ; ax, [bp+0x6]
JNZ       L_97f1              

L_9784:
LES       bx, [bp-lpb]              ; bx, [bp-0x410]
MOV       ax, es:[bx+0x1]     
AND       ax, 0x01ff          
CMP       ax, 0x01ff          
JZ        L_97f1              

L_9797:                             ; msg.c:1840
LES       bx, [bp-lpb]              ; bx, [bp-0x410]
MOV       al, es:[bx]         
AND       ax, 0x00ff          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
AND       ax, 0x000f          
ADD       ax, 0x0004          
PUSH      ax                  
MOV       ax, 0x0001          
MOV       cx, [bp-lpb]              ; cx, [bp-0x410]
MOV       dx, [bp-lpb+0x2]          ; dx, [bp-0x40e]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-cbMsg]            ; ax, [bp-0x408]
LEA       cx, [bp-rgb]              ; cx, [bp-0x406]
ADD       cx, ax              
MOV       dx, ss              
PUSH      dx                  
PUSH      cx                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; msg.c:1841
LES       bx, [bp-lpb]              ; bx, [bp-0x410]
MOV       al, es:[bx]         
AND       ax, 0x00ff          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
AND       ax, 0x000f          
ADD       ax, 0x0004          
ADD       [bp-cbMsg], ax            ; [bp-0x408], ax

L_97f1:                             ; msg.c:1844
LES       bx, [bp-lpb]              ; bx, [bp-0x410]
MOV       al, es:[bx]         
AND       ax, 0x00ff          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
AND       ax, 0x000f          
ADD       ax, 0x0005          
ADD       [bp-lpb], ax              ; [bp-0x410], ax

L_980d:                             ; msg.c:1845
MOV       ax, [bp-lpbMax]           ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        
CMP       [bp-lpb], ax              ; [bp-0x410], ax
JC        L_9742              

L_981c:                             ; msg.c:1847
CMP       [bp-cbMsg], 0x0000        ; [bp-0x408], 0x0000
JZ        L_983e              

L_9826:                             ; msg.c:1848
LEA       ax, [bp-rgb]              ; ax, [bp-0x406]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cbMsg]                ; [bp-0x408]
MOV       ax, 0x000c          
PUSH      ax                  
CALLF     WriteRt                   ; void WriteRt(RecordType rt, int16_t cb, void *rg)
ADD       sp, 0x0008          

L_983e:                             ; msg.c:1850
MOV       ax, [vlpmsgplrOut]        ; ax, [0x0b0a]
MOV       dx, [vlpmsgplrOut+0x2]    ; dx, [0x0b0c]
MOV       [bp-lpmp], ax             ; [bp-0x40c], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x40a], dx
                                    ; msg.c:1851
JMP       L_98bc              

L_9850:                             ; msg.c:1854
LES       bx, [bp-lpmp]             ; bx, [bp-0x40c]
CMP       es:[bx+0x6], 0x0000 
JNZ       L_986e              

L_985e:
LES       bx, [bp-lpmp]             ; bx, [bp-0x40c]
MOV       ax, [bp+iPlayer]          ; ax, [bp+0x6]
CMP       es:[bx+0x4], ax     
JNZ       L_9881              

L_986e:
LES       bx, [bp-lpmp]             ; bx, [bp-0x40c]
MOV       ax, es:[bx+0x6]     
ADD       ax, 0xffff          
CMP       ax, [bp+iPlayer]          ; ax, [bp+0x6]
JNZ       L_98a9              

L_9881:                             ; msg.c:1855
PUSH      [bp-lpmp+0x2]             ; [bp-0x40a]
PUSH      [bp-lpmp]                 ; [bp-0x40c]
LES       bx, [bp-lpmp]             ; bx, [bp-0x40c]
PUSH      es:[bx+0xa]         
CALLF     abs                       ; int16_t abs(int16_t x)
ADD       sp, 0x0002          
ADD       ax, 0x000c          
PUSH      ax                  
MOV       ax, 0x0028          
PUSH      ax                  
CALLF     WriteRt                   ; void WriteRt(RecordType rt, int16_t cb, void *rg)
ADD       sp, 0x0008          

L_98a9:                             ; msg.c:1856
LES       bx, [bp-lpmp]             ; bx, [bp-0x40c]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmp], ax             ; [bp-0x40c], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x40a], dx

L_98bc:                             ; msg.c:1857
CMP       [bp-lpmp], 0x0000         ; [bp-0x40c], 0x0000
JNZ       L_9850              

L_98c6:
CMP       [bp-lpmp+0x2], 0x0000     ; [bp-0x40a], 0x0000
JNZ       L_9850              

L_98d0:                             ; msg.c:1858
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


