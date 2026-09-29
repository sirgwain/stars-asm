; LphbAlloc  (memory)
;   addr: 000d:0000  len=264
;   sig:  HB * LphbAlloc(uint16_t cb, HeapType ht)
;   params:
;     uint16_t         cb             [BP+0x6]
;     HeapType         ht             [BP+0x8]
;   locals:
;     HB *             lphb           [BP-0x8]
;     HGLOBAL          hmem           [BP-0x4]
;
;   stats: blocks=0  labels=0

L_0000:                             ; memory.c:24
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0008          
PUSH      si                  
PUSH      di                  
                                    ; memory.c:26
MOV       [bp-lphb], 0x0000         ; [bp-0x8], 0x0000
MOV       [bp-lphb+0x2], 0x0000     ; [bp-0x6], 0x0000
                                    ; memory.c:28
ADD       [bp+cb], 0x0010           ; [bp+0x6], 0x0010
                                    ; memory.c:31
MOV       bx, [bp+ht]               ; bx, [bp+0x8]
SHL       bx, 0x0001          
MOV       ax, [bx+0xd64]      
CMP       [bp+cb], ax               ; [bp+0x6], ax
JNC       L_0034              

L_0028:                             ; memory.c:32
MOV       bx, [bp+ht]               ; bx, [bp+0x8]
SHL       bx, 0x0001          
MOV       ax, [bx+0xd64]      
MOV       [bp+cb], ax               ; [bp+0x6], ax

L_0034:                             ; memory.c:34
MOV       ax, 0x0022          
PUSH      ax                  
MOV       ax, [bp+cb]               ; ax, [bp+0x6]
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     GlobalAlloc               ; HGLOBAL GlobalAlloc(uint16_t arg1, uint32_t arg2)
MOV       [bp-hmem], ax             ; [bp-0x4], ax
                                    ; memory.c:35
CMP       [bp-hmem], 0x0000         ; [bp-0x4], 0x0000
JNZ       L_0082              

L_0051:                             ; memory.c:38
MOV       ax, 0x0010          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x001a          
PUSH      ax                  
CALLF     PszFormatIds              ; char * PszFormatIds(StringId ids, int16_t *pParams)
ADD       sp, 0x0006          
PUSH      ax                  
CALLF     AlertSz                   ; int16_t AlertSz(char *sz, int16_t mbType)
ADD       sp, 0x0004          
                                    ; memory.c:39
MOV       ax, 0xffff          
PUSH      ax                  
PUSH      [penvMem]                 ; [0x006e]
CALLF     StarsLongJump             ; void StarsLongJump(jmp_buf *env, int16_t value)
ADD       sp, 0x0004          

L_0082:                             ; memory.c:42
PUSH      [bp-hmem]                 ; [bp-0x4]
CALLF     GlobalLock                ; LPSTR GlobalLock(HGLOBAL arg1)
MOV       [bp-lphb], ax             ; [bp-0x8], ax
MOV       [bp-lphb+0x2], dx         ; [bp-0x6], dx
                                    ; memory.c:43
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       ax, [bp-hmem]             ; ax, [bp-0x4]
MOV       es:[bx+0xc], ax     
                                    ; memory.c:44
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       ax, [bp+cb]               ; ax, [bp+0x6]
MOV       es:[bx+0x2], ax     
                                    ; memory.c:45
MOV       ax, [bp+cb]               ; ax, [bp+0x6]
ADD       ax, 0xfff0          
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       es:[bx+0x4], ax     
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       es:[bx], ax         
                                    ; memory.c:46
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       es:[bx+0x6], 0x0010 
                                    ; memory.c:47
MOV       ax, [bp+ht]               ; ax, [bp+0x8]
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       es:[bx+0xe], al     
                                    ; memory.c:48
MOV       bx, [bp+ht]               ; bx, [bp+0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0xd34]      
MOV       dx, [bx+0xd36]      
LES       bx, [bp-lphb]             ; bx, [bp-0x8]
MOV       es:[bx+0x8], ax     
MOV       es:[bx+0xa], dx     
                                    ; memory.c:49
MOV       bx, [bp+ht]               ; bx, [bp+0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bp-lphb]             ; ax, [bp-0x8]
MOV       dx, [bp-lphb+0x2]         ; dx, [bp-0x6]
MOV       [bx+0xd34], ax      
MOV       [bx+0xd36], dx      
                                    ; memory.c:51
MOV       ax, [bp-lphb]             ; ax, [bp-0x8]
MOV       dx, [bp-lphb+0x2]         ; dx, [bp-0x6]

L_0102:                             ; memory.c:52
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


