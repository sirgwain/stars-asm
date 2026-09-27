; PszNameProdItem  (produce)
;   addr: 001b:3c92  len=653
;   sig:  char * PszNameProdItem(PROD *lpprod)
;   params:
;     PROD *           lpprod         [BP+0x6]
;   locals:
;     uint32_t         iItem          [BP-0x6]
;     block 001B:3D94  len=0x8A
;       int16_t          iDelta         [BP-0x8]
;
;   stats: blocks=1  labels=1
;     LBogus: L_3d3b

L_3c92:                             ; produce.c:1045
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000a          
PUSH      si                  
PUSH      di                  
                                    ; produce.c:1048
LES       bx, [bp+lpprod]           ; bx, [bp+0x6]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       cx, 0x000a          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
MOV       [bp-iItem], ax            ; [bp-0x6], ax
MOV       [bp-iItem+0x2], dx        ; [bp-0x4], dx
                                    ; produce.c:1050
LES       bx, [bp+lpprod]           ; bx, [bp+0x6]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       cx, 0x0011          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0007          
AND       dx, 0x0000          
CMP       ax, 0x0002          
JNZ       L_3e7c              

L_3cd9:
CMP       dx, 0x0000          
JNZ       L_3e7c              

L_3ce1:                             ; produce.c:1052
CMP       [bp-iItem+0x2], 0x0000    ; [bp-0x4], 0x0000
JC        L_3e21              

L_3cea:
JA        L_3cf8              

L_3cef:
CMP       [bp-iItem], 0x0010        ; [bp-0x6], 0x0010
JC        L_3e21              

L_3cf8:                             ; produce.c:1054
SUB       [bp-iItem], 0x0010        ; [bp-0x6], 0x0010
SBB       [bp-iItem+0x2], 0x0000    ; [bp-0x4], 0x0000
                                    ; produce.c:1057
MOV       ax, 0x0093          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iItem+0x2]            ; [bp-0x4]
PUSH      [bp-iItem]                ; [bp-0x6]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x7b]    
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_3d46              

LBogus:                             ; produce.c:1060
MOV       [szWork], 0x0000          ; [0x57a4], 0x0000
                                    ; produce.c:1061
MOV       ax, 0x57a4          
JMP       L_3f19              

L_3d46:                             ; produce.c:1064
MOV       ax, 0x0093          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iItem+0x2]            ; [bp-0x4]
PUSH      [bp-iItem]                ; [bp-0x6]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14c]      
MOV       dx, [bx+0x14e]      
ADD       cx, ax              
MOV       ax, 0x0008          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; produce.c:1065
MOV       cx, 0x0009          
MOV       ax, [sel+0x9c]            ; ax, [0x49f2]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_3f13              

L_3d94:                             ; produce.c:1068
MOV       ax, [sel+0xc4]            ; ax, [0x4a1a]
AND       ax, 0x000f          
MOV       cx, 0x0093          
IMUL      cx                  
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0093          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      cx                  
PUSH      [bp-iItem+0x2]            ; [bp-0x4]
PUSH      [bp-iItem]                ; [bp-0x6]
MOV       [bp-0xa], ax        
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       ax, [bp-0xa]        
MOV       es, cx              
SUB       ax, es:[bx]         
MOV       [bp-iDelta], ax           ; [bp-0x8], ax
                                    ; produce.c:1070
CMP       [bp-iDelta], 0x0000       ; [bp-0x8], 0x0000
JLE       L_3e05              

L_3df2:                             ; produce.c:1071
MOV       ax, 0x0ce8          
PUSH      ax                  
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; produce.c:1072
JMP       L_3f13              

L_3e05:
CMP       [bp-iDelta], 0x0000       ; [bp-0x8], 0x0000
JGE       L_3f13              

L_3e0e:                             ; produce.c:1073
MOV       ax, 0x0cf5          
PUSH      ax                  
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          

L_3e1e:                             ; produce.c:1076
JMP       L_3f13              

L_3e21:                             ; produce.c:1078
MOV       ax, 0x0093          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iItem+0x2]            ; [bp-0x4]
PUSH      [bp-iItem]                ; [bp-0x6]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, 0x3f00          
ADD       bx, ax              
MOV       ax, [bx+0x7b]       
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       LBogus              

L_3e4f:                             ; produce.c:1081
MOV       ax, 0x0093          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-iItem+0x2]            ; [bp-0x4]
PUSH      [bp-iItem]                ; [bp-0x6]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       cx, 0x3f00          
ADD       cx, ax              
MOV       ax, 0x0008          
ADD       cx, ax              
PUSH      cx                  
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          

L_3e79:                             ; produce.c:1084
JMP       L_3f13              

L_3e7c:                             ; produce.c:1090
CMP       [bp-iItem+0x2], 0x0000    ; [bp-0x4], 0x0000
JC        L_3ed8              

L_3e85:
JA        L_3e93              

L_3e8a:
CMP       [bp-iItem], 0x0012        ; [bp-0x6], 0x0012
JC        L_3ed8              

L_3e93:
CMP       [bp-iItem+0x2], 0x0000    ; [bp-0x4], 0x0000
JA        L_3ed8              

L_3e9c:
JC        L_3eaa              

L_3ea1:
CMP       [bp-iItem], 0x001a        ; [bp-0x6], 0x001a
JA        L_3ed8              

L_3eaa:                             ; produce.c:1091
MOV       ax, [bp-iItem]            ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        
ADD       ax, 0xffee          
ADC       dx, 0xffff          
PUSH      ax                  
CALLF     LpplanetaryFromId         ; PLANETARY * LpplanetaryFromId(int16_t id)
ADD       sp, 0x0002          
MOV       cx, 0x0008          
ADD       ax, cx              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; produce.c:1092
JMP       L_3f13              

L_3ed8:
CMP       [bp-iItem], 0x001b        ; [bp-0x6], 0x001b
JNZ       L_3efd              

L_3ee1:
CMP       [bp-iItem+0x2], 0x0000    ; [bp-0x4], 0x0000
JNZ       L_3efd              

L_3eea:                             ; produce.c:1093
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x051b          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; produce.c:1094
JMP       L_3f13              

L_3efd:                             ; produce.c:1096
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, [bp-iItem]            ; ax, [bp-0x6]
MOV       dx, [bp-0x4]        
ADD       ax, 0x007e          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          

L_3f13:                             ; produce.c:1099
MOV       ax, 0x57a4          

L_3f19:                             ; produce.c:1100
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


