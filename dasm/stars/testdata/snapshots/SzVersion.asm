; SzVersion  (stars)
;   addr: 0004:1212  len=64
;   sig:  char * SzVersion()
;
;   stats: blocks=0  labels=0

L_1212:                             ; stars.c:718
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0002          
PUSH      si                  
PUSH      di                  
                                    ; stars.c:722
MOV       ax, 0x006a          
PUSH      ax                  
MOV       ax, 0x003c          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x022d          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR *lpszout, LPCSTR *lpszfmt)
ADD       sp, 0x000e          
                                    ; stars.c:738
MOV       ax, szWork                ; ax, 0x57a4

L_124c:                             ; stars.c:739
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


