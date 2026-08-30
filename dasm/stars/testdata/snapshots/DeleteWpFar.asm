; DeleteWpFar  (ship)
;   addr: 000b:9e28  len=444
;   sig:  void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle)
;   params:
;     FLEET *          lpfl           [BP+0x6]
;     int16_t          iDel           [BP+0xa]
;     int16_t          fRecycle       [BP+0xc]
;   locals:
;     ORDER            ord            [BP-0x14]
;
;   stats: blocks=0  labels=0

L_9e28:                             ; ship.c:3812
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x001c          
PUSH      si                  
PUSH      di                  
                                    ; ship.c:3818
CMP       [bp+fRecycle], 0x0000     ; [bp+0xc], 0x0000
JZ        L_9ef2              

L_9e3a:                             ; ship.c:3821
CMP       [bp+iDel], 0x0056         ; [bp+0xa], 0x0056
JZ        L_9ebf              

L_9e43:
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
CMP       es:[bx+0x62], 0x0002
JZ        L_9ebf              

L_9e50:
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x66]    
MOV       bx, es:[bx+0x64]    
MOV       ax, 0x0004          
ADD       bx, ax              
MOV       ax, 0x0012          
IMUL      [bp+iDel]                 ; [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x66]    
MOV       bx, es:[bx+0x64]    
MOV       si, 0x0004          
ADD       bx, si              
MOV       [bp-0x18], bx       
MOV       [bp-0x16], cx       
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, es:[bx+0x62]    
MOV       [bp-0x1c], ax       
MOV       [bp-0x1a], dx       
MOV       ax, cx              
ADD       ax, 0xffff          
MOV       cx, 0x0012          
IMUL      cx                  
MOV       bx, [bp-0x18]       
MOV       cx, [bp-0x16]       
ADD       bx, ax              
MOV       ax, [bp-0x1c]       
MOV       dx, [bp-0x1a]       
MOV       es, cx              
CMP       es:[bx], ax         
JNZ       L_9ec7              

L_9eb6:
CMP       es:[bx+0x2], dx     
JNZ       L_9ec7              

L_9ebf:                             ; ship.c:3822
MOV       [bp+fRecycle], 0x0000     ; [bp+0xc], 0x0000
                                    ; ship.c:3823
JMP       L_9ef2              

L_9ec7:                             ; ship.c:3824
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, es:[bx+0x64]    
MOV       cx, es:[bx+0x66]    
MOV       ax, 0x0004          
ADD       si, ax              
MOV       ax, 0x0012          
IMUL      [bp+iDel]                 ; [bp+0xa]
ADD       si, ax              
PUSH      si                  
LEA       di, [bp-ord]              ; di, [bp-0x14]
PUSH      ss                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x0009          
MOVSW.REP es:[di], ds:[si]    
POP       ds                  
POP       si                  
LEA       ax, [bp-0x14]       

L_9ef2:                             ; ship.c:3827
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
SUB       ax, [bp+iDel]             ; ax, [bp+0xa]
ADD       ax, 0xffff          
MOV       cx, 0x0012          
IMUL      cx                  
PUSH      ax                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
MOV       cx, [bp+iDel]             ; cx, [bp+0xa]
ADD       cx, 0x0001          
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
MOV       ax, cx              
MOV       cx, 0x0012          
IMUL      cx                  
MOV       cx, [bp-0x18]       
MOV       dx, [bp-0x16]       
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
MOV       [bp-0x1c], ax       
MOV       [bp-0x1a], dx       
MOV       ax, 0x0012          
IMUL      [bp+iDel]                 ; [bp+0xa]
MOV       cx, [bp-0x1c]       
MOV       dx, [bp-0x1a]       
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; ship.c:3828
CMP       [bp+fRecycle], 0x0000     ; [bp+0xc], 0x0000
JZ        L_9fca              

L_9f69:                             ; ship.c:3829
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       di, es:[bx+0x64]    
MOV       cx, es:[bx+0x66]    
MOV       ax, 0x0004          
ADD       di, ax              
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
ADD       ax, 0xffff          
MOV       dx, 0x0012          
IMUL      dx                  
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
LEA       si, [bp-ord]              ; si, [bp-0x14]
MOV       cx, 0x0009          
MOVSW.REP es:[di], ds:[si]    
POP       di                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
LES       bx, [bp+0x6]        
MOV       cx, es:[bx+0x62]    
MOV       [bp-0x18], ax       
MOV       [bp-0x16], dx       
MOV       ax, cx              
ADD       ax, 0xffff          
MOV       cx, 0x0012          
IMUL      cx                  
MOV       cx, [bp-0x18]       
MOV       dx, [bp-0x16]       
ADD       cx, ax              
MOV       ax, cx              
                                    ; ship.c:3830
JMP       L_9fde              

L_9fca:                             ; ship.c:3832
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
SUB       es:[bx+0x62], 0x0001
                                    ; ship.c:3833
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
LES       bx, es:[bx+0x64]    
SUB       es:[bx+0x3], 0x0001 

L_9fde:                             ; ship.c:3835
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


