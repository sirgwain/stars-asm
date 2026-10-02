; WriteRtShDef  (save)
;   addr: 000f:574e  len=534
;   sig:  void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore)
;   params:
;     SHDEF *          lpshdef        [BP+0x6]
;     uint8_t * *      ppbStore       [BP+0xa]
;   locals:
;     int16_t          cOut           [BP-0xba]
;     uint8_t *        pb             [BP-0xb8]
;     char[32]         szHulName      [BP-0xb6]
;     uint8_t[147]     rgb            [BP-0x96]
;
;   stats: blocks=0  labels=0

L_574e:                             ; save.c:96
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x00ba          
PUSH      si                  
PUSH      di                  
                                    ; save.c:106
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx]         
MOV       [bp-rgb+0x2], al          ; [bp-0x94], al
                                    ; save.c:107
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x7b]    
MOV       [bp-rgb], ax              ; [bp-0x96], ax
                                    ; save.c:108
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       al, es:[bx+0x7a]    
MOV       [bp-rgb+0x6], al          ; [bp-0x90], al
                                    ; save.c:109
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x32]    
MOV       [bp-rgb+0x3], al          ; [bp-0x93], al
                                    ; save.c:110
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x7b]    
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_5816              

L_5794:                             ; save.c:112
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x38]    
MOV       [bp-rgb+0x4], ax          ; [bp-0x92], ax
                                    ; save.c:113
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x7d]    
MOV       [bp-rgb+0x7], ax          ; [bp-0x8f], ax
                                    ; save.c:114
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x7f]    
MOV       dx, es:[bx+0x81]    
MOV       [bp-rgb+0x9], ax          ; [bp-0x8d], ax
MOV       [bp-rgb+0xb], dx          ; [bp-0x8b], dx
                                    ; save.c:115
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x83]    
MOV       dx, es:[bx+0x85]    
MOV       [bp-rgb+0xd], ax          ; [bp-0x89], ax
MOV       [bp-rgb+0xf], dx          ; [bp-0x87], dx
                                    ; save.c:116
LEA       ax, [bp-rgb+0x11]         ; ax, [bp-0x85]
MOV       [bp-pb], ax               ; [bp-0xb8], ax
                                    ; save.c:117
MOV       al, [bp-rgb+0x6]          ; al, [bp-0x90]
AND       ax, 0x00ff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x003a          
MOV       cx, [bp+lpshdef]          ; cx, [bp+0x6]
MOV       dx, [bp+lpshdef+0x2]      ; dx, [bp+0x8]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-pb]               ; ax, [bp-0xb8]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; save.c:118
MOV       al, [bp-rgb+0x6]          ; al, [bp-0x90]
AND       ax, 0x00ff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
ADD       [bp-pb], ax               ; [bp-0xb8], ax
                                    ; save.c:120
JMP       L_5829              

L_5816:                             ; save.c:123
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x28]    
MOV       [bp-rgb+0x4], ax          ; [bp-0x92], ax
                                    ; save.c:124
LEA       ax, [bp-rgb+0x6]          ; ax, [bp-0x90]
MOV       [bp-pb], ax               ; [bp-0xb8], ax

L_5829:                             ; save.c:127
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
MOV       ax, es:[bx+0x7b]    
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_585b              

L_583b:                             ; save.c:128
MOV       ax, 0x0008          
MOV       cx, [bp+lpshdef]          ; cx, [bp+0x6]
MOV       dx, [bp+lpshdef+0x2]      ; dx, [bp+0x8]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
LEA       ax, [bp-szHulName]        ; ax, [bp-0xb6]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; save.c:129
JMP       L_5880              

L_585b:                             ; save.c:132
LES       bx, [bp+lpshdef]          ; bx, [bp+0x6]
PUSH      es:[bx]             
CALLF     LphuldefFromId            ; HULDEF * LphuldefFromId(int16_t id)
ADD       sp, 0x0002          
MOV       cx, 0x0008          
ADD       ax, cx              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szHulName]        ; ax, [bp-0xb6]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          

L_5880:                             ; save.c:135
MOV       [bp-cOut], 0x001f         ; [bp-0xba], 0x001f
                                    ; save.c:136
MOV       al, [bp-szHulName]        ; al, [bp-0xb6]
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_58d5              

L_5893:
LEA       ax, [bp-cOut]             ; ax, [bp-0xba]
PUSH      ax                  
MOV       ax, 0x0001          
MOV       cx, [bp-pb]               ; cx, [bp-0xb8]
ADD       cx, ax              
MOV       dx, ds              
PUSH      dx                  
PUSH      cx                  
LEA       ax, [bp-szHulName]        ; ax, [bp-0xb6]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     FCompressUserString       ; int16_t FCompressUserString(char *szIn, char *szOut, int16_t *pcOut)
ADD       sp, 0x000a          
CMP       ax, 0x0000          
JZ        L_58d5              

L_58bd:                             ; save.c:138
MOV       ax, [bp-cOut]             ; ax, [bp-0xba]
MOV       bx, [bp-pb]               ; bx, [bp-0xb8]
MOV       [bx], al            
                                    ; save.c:139
MOV       ax, [bp-cOut]             ; ax, [bp-0xba]
ADD       ax, 0x0001          
ADD       [bp-pb], ax               ; [bp-0xb8], ax
                                    ; save.c:141
JMP       L_5907              

L_58d5:                             ; save.c:143
LEA       ax, [bp-szHulName]        ; ax, [bp-0xb6]
PUSH      ax                  
MOV       ax, 0x0001          
MOV       cx, [bp-pb]               ; cx, [bp-0xb8]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; save.c:144
MOV       bx, [bp-pb]               ; bx, [bp-0xb8]
MOV       [bx], 0x0000        
                                    ; save.c:145
LEA       ax, [bp-szHulName]        ; ax, [bp-0xb6]
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       ax, 0x0002          
ADD       [bp-pb], ax               ; [bp-0xb8], ax

L_5907:                             ; save.c:148
CMP       [bp+ppbStore], 0x0000     ; [bp+0xa], 0x0000
JZ        L_593f              

L_5910:                             ; save.c:150
LEA       ax, [bp-rgb]              ; ax, [bp-0x96]
MOV       cx, [bp-pb]               ; cx, [bp-0xb8]
SUB       cx, ax              
PUSH      cx                  
LEA       ax, [bp-rgb]              ; ax, [bp-0x96]
PUSH      ax                  
MOV       bx, [bp+ppbStore]         ; bx, [bp+0xa]
PUSH      [bx]                
CALLF     memmove                   ; void * memmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x0006          
                                    ; save.c:151
LEA       ax, [bp-rgb]              ; ax, [bp-0x96]
MOV       cx, [bp-pb]               ; cx, [bp-0xb8]
SUB       cx, ax              
MOV       bx, [bp+ppbStore]         ; bx, [bp+0xa]
ADD       [bx], cx            
                                    ; save.c:153
JMP       L_595e              

L_593f:                             ; save.c:154
LEA       ax, [bp-rgb]              ; ax, [bp-0x96]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-rgb]              ; ax, [bp-0x96]
MOV       cx, [bp-pb]               ; cx, [bp-0xb8]
SUB       cx, ax              
PUSH      cx                  
MOV       ax, 0x001a          
PUSH      ax                  
CALLF     WriteRt                   ; void WriteRt(RecordType rt, int16_t cb, void *rg)
ADD       sp, 0x0008          

L_595e:                             ; save.c:155
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


