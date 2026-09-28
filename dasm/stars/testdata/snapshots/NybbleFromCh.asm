; NybbleFromCh  (utilgen)
;   addr: 0009:4880  len=361
;   sig:  int16_t NybbleFromCh(uint8_t ch)
;   params:
;     uint8_t          ch             [BP+0x6]
;   locals:
;     char *           pch            [BP-0x4]
;
;   stats: blocks=0  labels=0

L_4880:                             ; utilgen.c:1592
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0004          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:1595
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0061          
JL        L_48b9              

L_4897:
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x007a          
JG        L_48b9              

L_48a5:                             ; utilgen.c:1596
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
MOV       bx, ax              
ADD       bx, 0xff9f          
SHL       bx, 0x0001          
MOV       ax, [bx+0x13ae]     
JMP       L_49e3              

L_48b9:                             ; utilgen.c:1597
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0020          
JNZ       L_48cd              

L_48c7:                             ; utilgen.c:1598
MOV       ax, 0x0000          
JMP       L_49e3              

L_48cd:                             ; utilgen.c:1599
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0041          
JL        L_4900              

L_48db:
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0050          
JG        L_4900              

L_48e9:                             ; utilgen.c:1600
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
ADD       ax, 0xffbf          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000b          
JMP       L_49e3              

L_4900:                             ; utilgen.c:1601
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0051          
JL        L_4933              

L_490e:
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x005a          
JG        L_4933              

L_491c:                             ; utilgen.c:1602
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
ADD       ax, 0xffaf          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000c          
JMP       L_49e3              

L_4933:                             ; utilgen.c:1603
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0030          
JL        L_4966              

L_4941:
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0035          
JG        L_4966              

L_494f:                             ; utilgen.c:1604
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
ADD       ax, 0xffda          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000c          
JMP       L_49e3              

L_4966:                             ; utilgen.c:1605
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0036          
JL        L_4999              

L_4974:
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0039          
JG        L_4999              

L_4982:                             ; utilgen.c:1606
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
ADD       ax, 0xffca          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000d          
JMP       L_49e3              

L_4999:                             ; utilgen.c:1607
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
PUSH      ax                  
MOV       ax, 0x1400          
PUSH      ax                  
CALLF     strchr                    ; char * strchr(char *s, int16_t ch)
ADD       sp, 0x0004          
MOV       [bp-pch], ax              ; [bp-0x4], ax
                                    ; utilgen.c:1608
CMP       [bp-pch], 0x0000          ; [bp-0x4], 0x0000
JZ        L_49cf              

L_49b8:                             ; utilgen.c:1609
MOV       ax, [bp-pch]              ; ax, [bp-0x4]
SUB       ax, 0x1400          
ADD       ax, 0x0004          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000e          
JMP       L_49e3              

L_49cf:                             ; utilgen.c:1611
MOV       al, [bp+ch]               ; al, [bp+0x6]
AND       ax, 0x00ff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
OR        ax, 0x000f          

L_49e3:                             ; utilgen.c:1612
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


