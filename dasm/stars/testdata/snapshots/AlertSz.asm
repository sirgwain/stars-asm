; AlertSz  (utilgen)
;   addr: 0009:2160  len=189
;   sig:  int16_t AlertSz(char *sz, MessageBoxType mbType)
;   params:
;     char *           sz             [BP+0x6]
;     MessageBoxType   mbType         [BP+0x8]
;   locals:
;     char[256]        szT            [BP-0x102]
;
;   stats: blocks=0  labels=0

L_2160:                             ; utilgen.c:415
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0102          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:418
MOV       cx, 0x000e          
MOV       ax, [ini+0xa]             ; ax, [0x229a]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_21a3              

L_217c:
MOV       cx, 0x000f          
MOV       ax, [ini+0xa]             ; ax, [0x229a]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_21f7              

L_218f:
MOV       ax, [ini+0xa]             ; ax, [0x229a]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_21f7              

L_21a3:                             ; utilgen.c:420
MOV       ax, [bp+sz]               ; ax, [bp+0x6]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x137b          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szT]              ; ax, [bp-0x102]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR *lpszout, LPCSTR *lpszfmt)
ADD       sp, 0x000c          
                                    ; utilgen.c:421
LEA       ax, [bp-szT]              ; ax, [bp-0x102]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       cx, 0x000e          
MOV       ax, [ini+0xa]             ; ax, [0x229a]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_21e2              

L_21dc:
MOV       ax, 0x0007          
JMP       L_21e5              

L_21e2:
MOV       ax, 0x0006          

L_21e5:
PUSH      ax                  
CALLF     OutputSz                  ; void OutputSz(int16_t dt, char *sz)
ADD       sp, 0x0006          
                                    ; utilgen.c:422
MOV       ax, 0x0006          
JMP       L_2217              

L_21f7:                             ; utilgen.c:425
CALLF     GetFocus                  ; HWND GetFocus()
PUSH      ax                  
MOV       ax, [bp+sz]               ; ax, [bp+0x6]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x1385          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp+mbType]           ; ax, [bp+0x8]
PUSH      ax                  
CALLF     MessageBox                ; int16_t MessageBox(HWND arg1, LPCSTR *arg2, LPCSTR *arg3, MessageBoxType arg4)

L_2217:                             ; utilgen.c:426
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


