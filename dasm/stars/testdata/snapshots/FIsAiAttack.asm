; FIsAiAttack  (aiutil)
;   addr: 0013:4a72  len=296
;   sig:  int16_t FIsAiAttack(FLEET *lpfl)
;   params:
;     FLEET *          lpfl           [BP+0x6]
;   locals:
;     int16_t          i              [BP-0x6]
;     int16_t          ihul           [BP-0x4]
;
;   stats: blocks=0  labels=0

L_4a72:                             ; aiutil.c:2045
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0006          
PUSH      si                  
PUSH      di                  
                                    ; aiutil.c:2049
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_4b85              

L_4a83:                             ; aiutil.c:2051
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JLE       L_4b81              

L_4aa0:                             ; aiutil.c:2053
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x6]
MOV       bx, ax              
MOV       ax, [bx+0x3f00]     
MOV       [bp-ihul], ax             ; [bp-0x4], ax
                                    ; aiutil.c:2054
CMP       [bp-ihul], 0x0005         ; [bp-0x4], 0x0005
JLE       L_4ac7              

L_4ab8:
CMP       [bp-ihul], 0x000a         ; [bp-0x4], 0x000a
JG        L_4ac7              

L_4ac1:                             ; aiutil.c:2055
MOV       ax, 0x0001          
JMP       L_4b94              

L_4ac7:                             ; aiutil.c:2056
CMP       [bp-ihul], 0x0005         ; [bp-0x4], 0x0005
JNZ       L_4b11              

L_4ad0:                             ; aiutil.c:2057
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x6]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x100]      
MOV       bx, [bx+0xfe]       
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx+0x89], 0x0000
JL        L_4b0b              

L_4af5:
JG        L_4b05              

L_4afa:
CMP       es:[bx+0x87], 0x0000
JBE       L_4b0b              

L_4b05:
MOV       ax, 0x0001          
JMP       L_4b94              

L_4b0b:
MOV       ax, 0x0000          

L_4b0e:
JMP       L_4b94              

L_4b11:                             ; aiutil.c:2060
CMP       [bp-ihul], 0x001f         ; [bp-0x4], 0x001f
JZ        L_4b23              

L_4b1a:
CMP       [bp-ihul], 0x001d         ; [bp-0x4], 0x001d
JNZ       L_4b81              

L_4b23:
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x6]
MOV       cx, 0x3f00          
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
CALLF     WtMaxShdefStat            ; int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat)
ADD       sp, 0x0006          
CMP       ax, 0x01f4          
JGE       L_4b81              

L_4b46:
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0x6]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x100]      
MOV       bx, [bx+0xfe]       
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx+0x89], 0x0000
JL        L_4b81              

L_4b6b:
JG        L_4b7b              

L_4b70:
CMP       es:[bx+0x87], 0x0000
JBE       L_4b81              

L_4b7b:                             ; aiutil.c:2061
MOV       ax, 0x0001          
JMP       L_4b94              

L_4b81:                             ; aiutil.c:2063
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_4b85:
CMP       [bp-i], 0x0010            ; [bp-0x6], 0x0010
JL        L_4a83              

L_4b8e:                             ; aiutil.c:2064
MOV       ax, 0x0000          

L_4b94:                             ; aiutil.c:2065
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


