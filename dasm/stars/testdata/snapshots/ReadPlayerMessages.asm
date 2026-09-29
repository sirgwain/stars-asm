; ReadPlayerMessages  (msg)
;   addr: 0007:994a  len=651
;   sig:  void ReadPlayerMessages()
;   locals:
;     uint16_t         u              [BP-0x30]
;     uint8_t *        lpb            [BP-0x2e]
;     MSGPLR *         lpmp           [BP-0x2a]
;     jmp_buf          env            [BP-0x26]
;     int16_t          i              [BP-0x14]
;     uint16_t         imemMsgT       [BP-0x12]
;     MSGHDR *         lpmh           [BP-0x10]
;     jmp_buf *        penvMemSav     [BP-0xc]
;     int16_t          fOOM           [BP-0xa]
;     int16_t          iMax           [BP-0x8]
;     uint8_t *        lpbMax         [BP-0x6]
;
;   stats: blocks=0  labels=1
;     LOutOfMem: L_9bb2

L_994a:                             ; msg.c:1875
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0032          
PUSH      si                  
PUSH      di                  
                                    ; msg.c:1880
MOV       [bp-imemMsgT], 0x0000     ; [bp-0x12], 0x0000
                                    ; msg.c:1886
MOV       [bp-fOOM], 0x0000         ; [bp-0xa], 0x0000
                                    ; msg.c:1888
MOV       ax, [imemMsgCur]          ; ax, [0x0afc]
MOV       cx, [lpMsg]               ; cx, [0x0af8]
MOV       dx, [lpMsg+0x2]           ; dx, [0x0afa]
ADD       cx, ax              
MOV       [bp-lpb], cx              ; [bp-0x2e], cx
MOV       [bp-lpb+0x2], dx          ; [bp-0x2c], dx

L_9970:                             ; msg.c:1889
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x000c          
JNZ       L_99df              

L_9983:                             ; msg.c:1891
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
CMP       ax, 0x0000          
JZ        L_99d7              

L_9991:
MOV       ax, [imemMsgCur]          ; ax, [0x0afc]
ADD       ax, [bp-imemMsgT]         ; ax, [bp-0x12]
MOV       cx, [hdrCur]              ; cx, [0x2696]
AND       cx, 0x03ff          
MOV       dx, 0xffc8          
SUB       dx, cx              
CMP       ax, dx              
JNC       L_99d7              

L_99ab:                             ; msg.c:1893
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
PUSH      ax                  
MOV       ax, 0x4b98          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-imemMsgT]         ; ax, [bp-0x12]
MOV       cx, [bp-lpb]              ; cx, [bp-0x2e]
MOV       dx, [bp-lpb+0x2]          ; dx, [bp-0x2c]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; msg.c:1894
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
ADD       [bp-imemMsgT], ax         ; [bp-0x12], ax

L_99d7:                             ; msg.c:1896
CALLF     ReadRt                    ; void ReadRt()
                                    ; msg.c:1897
JMP       L_9970              

L_99df:                             ; msg.c:1899
MOV       ax, [bp-imemMsgT]         ; ax, [bp-0x12]
ADD       [imemMsgCur], ax          ; [0x0afc], ax
                                    ; msg.c:1900
MOV       ax, [bp-imemMsgT]         ; ax, [bp-0x12]
MOV       cx, [bp-lpb]              ; cx, [bp-0x2e]
MOV       dx, [bp-lpb+0x2]          ; dx, [bp-0x2c]
ADD       cx, ax              
MOV       [bp-lpbMax], cx           ; [bp-0x6], cx
MOV       [bp-lpbMax+0x2], dx       ; [bp-0x4], dx
                                    ; msg.c:1902
JMP       L_9aca              

L_99fa:                             ; msg.c:1904
MOV       ax, [bp-lpb]              ; ax, [bp-0x2e]
MOV       dx, [bp-lpb+0x2]          ; dx, [bp-0x2c]
MOV       [bp-lpmh], ax             ; [bp-0x10], ax
MOV       [bp-lpmh+0x2], dx         ; [bp-0xe], dx
                                    ; msg.c:1905
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       cx, es:[bx]         
AND       cx, 0x01ff          
AND       cx, 0x0007          
MOV       ax, 0x0001          
SHL       ax, cx              
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       cx, es:[bx]         
AND       cx, 0x01ff          
AND       cx, 0x0007          
MOV       dx, 0x0001          
SHL       dx, cx              
NOT       dx                  
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       bx, es:[bx]         
AND       bx, 0x01ff          
SHR       bx, 0x0001          
SHR       bx, 0x0001          
SHR       bx, 0x0001          
MOV       cl, [bx+0x516a]     
MOV       [bp-0x32], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
AND       ax, dx              
MOV       cx, [bp-0x32]       
OR        ax, cx              
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       bx, es:[bx]         
AND       bx, 0x01ff          
SHR       bx, 0x0001          
SHR       bx, 0x0001          
SHR       bx, 0x0001          
MOV       [bx+0x516a], al     
                                    ; msg.c:1906
ADD       [cMsg], 0x0001            ; [0x0afe], 0x0001
                                    ; msg.c:1907
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x007f          
MOV       [bp-u], ax                ; [bp-0x30], ax
                                    ; msg.c:1908
ADD       [bp-lpb], 0x0004          ; [bp-0x2e], 0x0004
                                    ; msg.c:1910
LES       bx, [bp-lpmh]             ; bx, [bp-0x10]
MOV       bx, es:[bx]         
AND       bx, 0x01ff          
MOV       al, cs:[bx+0x5b0e]  
CBW       ax, al              
MOV       [bp-iMax], ax             ; [bp-0x8], ax
                                    ; msg.c:1911
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
JMP       L_9abf              

L_9a98:                             ; msg.c:1913
MOV       ax, [bp-u]                ; ax, [bp-0x30]
AND       ax, 0x0001          
CMP       ax, 0x0001          
JNZ       L_9aac              

L_9aa6:
MOV       ax, 0x0001          
JMP       L_9aaf              

L_9aac:
MOV       ax, 0x0000          

L_9aaf:
ADD       ax, 0x0001          
ADD       [bp-lpb], ax              ; [bp-0x2e], ax
                                    ; msg.c:1914
MOV       cx, 0x0001          
SHR       [bp-u], cx                ; [bp-0x30], cx
                                    ; msg.c:1915
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001

L_9abf:
MOV       ax, [bp-iMax]             ; ax, [bp-0x8]
CMP       [bp-i], ax                ; [bp-0x14], ax
JL        L_9a98              

L_9aca:                             ; msg.c:1916
MOV       ax, [bp-lpbMax]           ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        
CMP       [bp-lpb], ax              ; [bp-0x2e], ax
JC        L_99fa              

L_9ad8:                             ; msg.c:1920
MOV       ax, 0x0b06          
MOV       dx, ds              
MOV       [bp-lpmp], ax             ; [bp-0x2a], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x28], dx

L_9ae3:                             ; msg.c:1921
LES       bx, [bp-lpmp]             ; bx, [bp-0x2a]
CMP       es:[bx], 0x0000     
JNZ       L_9af9              

L_9aef:
CMP       es:[bx+0x2], 0x0000 
JZ        L_9b0c              

L_9af9:                             ; msg.c:1922
LES       bx, [bp-lpmp]             ; bx, [bp-0x2a]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmp], ax             ; [bp-0x2a], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x28], dx
JMP       L_9ae3              

L_9b0c:                             ; msg.c:1924
MOV       ax, [penvMem]             ; ax, [0x006e]
MOV       [bp-penvMemSav], ax       ; [bp-0xc], ax
                                    ; msg.c:1925
LEA       ax, [bp-env]              ; ax, [bp-0x26]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; msg.c:1926
LEA       ax, [bp-env]              ; ax, [bp-0x26]
PUSH      ax                  
CALLF     setjmp                    ; int16_t setjmp(int16_t *env)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_9b3a              

L_9b2c:                             ; msg.c:1928
MOV       ax, [bp-penvMemSav]       ; ax, [bp-0xc]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; msg.c:1929
MOV       [bp-fOOM], 0x0001         ; [bp-0xa], 0x0001
                                    ; msg.c:1930
JMP       LOutOfMem           

L_9b3a:                             ; msg.c:1933
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0028          
JNZ       L_9bba              

L_9b4d:                             ; msg.c:1935
CMP       [bp-fOOM], 0x0000         ; [bp-0xa], 0x0000
JNZ       LOutOfMem           

L_9b56:                             ; msg.c:1937
MOV       ax, 0x0008          
PUSH      ax                  
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
LES       bx, [bp-lpmp]             ; bx, [bp-0x2a]
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; msg.c:1938
LES       bx, [bp-lpmp]             ; bx, [bp-0x2a]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmp], ax             ; [bp-0x2a], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x28], dx
                                    ; msg.c:1939
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
PUSH      ax                  
MOV       ax, 0x4b98          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-lpmp+0x2]             ; [bp-0x28]
PUSH      [bp-lpmp]                 ; [bp-0x2a]
CALLF     fmemcpy                   ; void * fmemcpy(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; msg.c:1940
LES       bx, [bp-lpmp]             ; bx, [bp-0x2a]
MOV       es:[bx], 0x0000     
MOV       es:[bx+0x2], 0x0000 
                                    ; msg.c:1941
ADD       [vcmsgplrIn], 0x0001      ; [0x0b0e], 0x0001

LOutOfMem:                          ; msg.c:1945
CALLF     ReadRt                    ; void ReadRt()
                                    ; msg.c:1946
JMP       L_9b3a              

L_9bba:                             ; msg.c:1948
MOV       [iMsgCur], 0xffff         ; [0x0b00], 0xffff
                                    ; msg.c:1949
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     IMsgNext                  ; int16_t IMsgNext(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [iMsgCur], ax             ; [0x0b00], ax
                                    ; msg.c:1950
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


