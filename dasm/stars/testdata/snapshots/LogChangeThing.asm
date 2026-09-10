; LogChangeThing  (log)
;   addr: 000a:9908  len=238
;   sig:  void LogChangeThing(THING *lpth, THING *pthNew)
;   params:
;     THING *          lpth           [BP+0x6]
;     THING *          pthNew         [BP+0xa]
;   locals:
;     LOGXFER          lxNew          [BP-0x1e]
;     int16_t          fChg           [BP-0x6]
;     int16_t          i              [BP-0x4]
;
;   stats: blocks=0  labels=0

L_9908:                             ; log.c:463
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x001e          
PUSH      si                  
PUSH      di                  
                                    ; log.c:466
MOV       [bp-fChg], 0x0000         ; [bp-0x6], 0x0000
                                    ; log.c:468
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_99f0              

L_9929:                             ; log.c:474
MOV       ax, 0x0018          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-lxNew]            ; ax, [bp-0x1e]
PUSH      ax                  
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; log.c:475
MOV       bx, [bp+pthNew]           ; bx, [bp+0xa]
MOV       ax, [bx]            
MOV       [bp-lxNew], ax            ; [bp-0x1e], ax
                                    ; log.c:476
MOV       [bp-lxNew+0x2], 0x0008    ; [bp-0x1c], 0x0008
                                    ; log.c:478
MOV       [bp-i], 0x0000            ; [bp-0x4], 0x0000
JMP       L_99a5              

L_9952:                             ; log.c:480
MOV       ax, 0x0008          
MOV       bx, [bp+pthNew]           ; bx, [bp+0xa]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       ax, [bx]            
MOV       cx, 0x0008          
MOV       bx, [bp+lpth]             ; bx, [bp+0x6]
MOV       si, [bp+lpth+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0x4]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
SUB       ax, es:[bx]         
CWD       dx, ax              
MOV       bx, [bp-i]                ; bx, [bp-0x4]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LEA       si, [bp-0x1a]       
ADD       si, bx              
MOV       [si], ax            
MOV       [si+0x2], dx        
CMP       ax, 0x0000          
JNZ       L_999c              

L_9994:
CMP       dx, 0x0000          
JZ        L_99a1              

L_999c:                             ; log.c:481
MOV       [bp-fChg], 0x0001         ; [bp-0x6], 0x0001

L_99a1:                             ; log.c:482
ADD       [bp-i], 0x0001            ; [bp-0x4], 0x0001

L_99a5:
CMP       [bp-i], 0x0003            ; [bp-0x4], 0x0003
JL        L_9952              

L_99ae:                             ; log.c:483
CMP       [bp-fChg], 0x0000         ; [bp-0x6], 0x0000
JZ        L_99f0              

L_99b7:                             ; log.c:485
CMP       [fValidLx], 0x0000        ; [0x0998], 0x0000
JZ        L_99da              

L_99c1:                             ; log.c:487
LEA       ax, [bp-lxNew]            ; ax, [bp-0x1e]
PUSH      ax                  
MOV       ax, 0x24ca          
PUSH      ax                  
CALLF     LogMakeValidXfer          ; void LogMakeValidXfer(LOGXFER *plx1, LOGXFER *plx2)
ADD       sp, 0x0004          
                                    ; log.c:488
MOV       [fValidLx], 0x0000        ; [0x0998], 0x0000
                                    ; log.c:490
JMP       L_99f0              

L_99da:                             ; log.c:492
LEA       si, [bp-lxNew]            ; si, [bp-0x1e]
MOV       di, 0x24ca          
PUSH      ds                  
POP       es                  
MOV       cx, 0x000c          
MOVSW.REP [lx], ds:[si]             ; es:[di], ds:[si]
MOV       ax, 0x24ca          
                                    ; log.c:493
MOV       [fValidLx], 0x0001        ; [0x0998], 0x0001

L_99f0:                             ; log.c:496
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


