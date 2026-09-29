; LpflNew  (util)
;   addr: 0008:300c  len=869
;   sig:  FLEET * LpflNew(int16_t iPlr, int16_t idPl)
;   params:
;     int16_t          iPlr           [BP+0x6]
;     int16_t          idPl           [BP+0x8]
;   locals:
;     int16_t          iflPrev        [BP-0xe]
;     FLEET *          lpfl           [BP-0xc]
;     ORDER *          lpord          [BP-0x8]
;     int16_t          i              [BP-0x4]
;
;   stats: blocks=0  labels=0

L_300c:                             ; util.c:1304
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0010          
PUSH      si                  
PUSH      di                  
                                    ; util.c:1308
MOV       [bp-iflPrev], 0xffff      ; [bp-0xe], 0xffff
                                    ; util.c:1312
MOV       [bp-i], 0x0000            ; [bp-0x4], 0x0000
JMP       L_3026              

L_3022:
ADD       [bp-i], 0x0001            ; [bp-0x4], 0x0001

L_3026:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x4], ax
JGE       L_30ad              

L_3031:
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0xc], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0xa], dx
CMP       ax, 0x0000          
JNZ       L_3061              

L_3059:
CMP       dx, 0x0000          
JZ        L_30ad              

L_3061:                             ; util.c:1314
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
CMP       es:[bx+0x2], ax     
JL        L_3022              

L_3073:                             ; util.c:1316
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
CMP       es:[bx+0x2], ax     
JG        L_30ad              

L_3085:                             ; util.c:1318
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx]         
AND       ax, 0x01ff          
MOV       cx, [bp-iflPrev]          ; cx, [bp-0xe]
ADD       cx, 0x0001          
CMP       ax, cx              
JNZ       L_30ad              

L_309e:                             ; util.c:1320
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx]         
AND       ax, 0x01ff          
MOV       [bp-iflPrev], ax          ; [bp-0xe], ax
                                    ; util.c:1321
JMP       L_3022              

L_30ad:                             ; util.c:1323
MOV       ax, 0x0006          
PUSH      ax                  
MOV       ax, [cFleet]              ; ax, [0x5356]
ADD       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
PUSH      ax                  
PUSH      [rglpfl+0x2]              ; [0x00fc]
PUSH      [rglpfl]                  ; [0x00fa]
CALLF     LpReAlloc                 ; void * LpReAlloc(void *lp, uint16_t cb, HeapType ht)
ADD       sp, 0x0008          
MOV       [rglpfl], ax              ; [0x00fa], ax
MOV       [rglpfl+0x2], dx          ; [0x00fc], dx
                                    ; util.c:1324
MOV       ax, [bp-i]                ; ax, [bp-0x4]
CMP       [cFleet], ax              ; [0x5356], ax
JZ        L_311b              

L_30df:                             ; util.c:1325
MOV       ax, [cFleet]              ; ax, [0x5356]
SUB       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x4]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [rglpfl]              ; cx, [0x00fa]
MOV       dx, [rglpfl+0x2]          ; dx, [0x00fc]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-i]                ; ax, [bp-0x4]
ADD       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [rglpfl]              ; cx, [0x00fa]
MOV       dx, [rglpfl+0x2]          ; dx, [0x00fc]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          

L_311b:                             ; util.c:1326
MOV       ax, 0x0005          
PUSH      ax                  
MOV       ax, 0x007c          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [bp-lpfl], ax             ; [bp-0xc], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0xa], dx
MOV       bx, [bp-i]                ; bx, [bp-0x4]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [rglpfl]              ; si, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       si, bx              
MOV       bx, si              
MOV       es, cx              
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; util.c:1327
ADD       [cFleet], 0x0001          ; [0x5356], 0x0001
                                    ; util.c:1328
MOV       ax, 0x00c0          
IMUL      [bp+iPlr]                 ; [bp+0x6]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, [bx+0x4]        
ADD       ax, 0x0001          
AND       ax, 0x0fff          
MOV       [bp-0x10], ax       
MOV       ax, 0x00c0          
IMUL      [bp+iPlr]                 ; [bp+0x6]
MOV       bx, 0x59a2          
ADD       bx, ax              
AND       [bx+0x4], 0xf000    
MOV       ax, [bx+0x4]        
MOV       ax, 0x00c0          
IMUL      [bp+iPlr]                 ; [bp+0x6]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, [bp-0x10]       
OR        [bx+0x4], ax        
MOV       ax, [bx+0x4]        
                                    ; util.c:1330
MOV       ax, 0x007c          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-lpfl+0x2]             ; [bp-0xa]
PUSH      [bp-lpfl]                 ; [bp-0xc]
CALLF     fmemset                   ; void * fmemset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0008          
                                    ; util.c:1331
MOV       ax, [bp-iflPrev]          ; ax, [bp-0xe]
ADD       ax, 0x0001          
MOV       [bp-0x10], ax       
MOV       ax, [bp-0x10]       
AND       ax, 0x01ff          
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       cx, es:[bx]         
AND       cx, 0xfe00          
OR        cx, ax              
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx], cx         
MOV       ax, cx              
                                    ; util.c:1332
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
MOV       es:[bx+0x2], ax     
                                    ; util.c:1333
MOV       ax, [bp+iPlr]             ; ax, [bp+0x6]
AND       ax, 0x000f          
MOV       cx, 0x0009          
SHL       ax, cx              
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       cx, es:[bx]         
AND       cx, 0xe1ff          
OR        cx, ax              
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx], cx         
                                    ; util.c:1334
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x4]     
AND       ax, 0xff00          
OR        ax, 0x0007          
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x4], ax     
                                    ; util.c:1335
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, [bp+idPl]             ; ax, [bp+0x8]
MOV       es:[bx+0x6], ax     
                                    ; util.c:1336
CMP       [bp+idPl], 0xffff         ; [bp+0x8], 0xffff
JZ        L_3231              

L_3217:                             ; util.c:1337
MOV       bx, [bp+idPl]             ; bx, [bp+0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
MOV       dx, [bx+0x2f42]     
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x8], ax     
MOV       es:[bx+0xa], dx     

L_3231:                             ; util.c:1338
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x62], 0x0001
                                    ; util.c:1339
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x4]     
AND       ax, 0xfdff          
OR        ax, 0x0000          
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x4], ax     
                                    ; util.c:1340
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0003          
PUSH      ax                  
MOV       ax, 0x0012          
PUSH      ax                  
CALLF     LpplAlloc                 ; PL * LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht)
ADD       sp, 0x0006          
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x64], ax    
MOV       es:[bx+0x66], dx    
                                    ; util.c:1341
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
LES       bx, es:[bx+0x64]    
MOV       es:[bx+0x3], 0x0001 
                                    ; util.c:1342
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x76]    
AND       ax, 0xffef          
OR        ax, 0x0000          
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       es:[bx+0x76], ax    
                                    ; util.c:1344
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
MOV       [bp-lpord], ax            ; [bp-0x8], ax
MOV       [bp-lpord+0x2], dx        ; [bp-0x6], dx
                                    ; util.c:1345
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x8]     
MOV       dx, es:[bx+0xa]     
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; util.c:1346
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x6]     
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx+0x4], ax     
                                    ; util.c:1347
LES       bx, [bp-lpfl]             ; bx, [bp-0xc]
CMP       es:[bx+0x6], 0xffff 
JZ        L_32d9              

L_32d3:
MOV       ax, 0x0001          
JMP       L_32dc              

L_32d9:
MOV       ax, 0x0004          

L_32dc:
MOV       [bp-0x10], ax       
MOV       ax, [bp-0x10]       
AND       ax, 0x000f          
MOV       cx, 0x0008          
SHL       ax, cx              
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       cx, es:[bx+0x6]     
AND       cx, 0xf0ff          
OR        cx, ax              
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx+0x6], cx     
MOV       ax, cx              
                                    ; util.c:1348
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xff0f          
OR        ax, 0x0000          
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx+0x6], ax     
                                    ; util.c:1349
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xefff          
OR        ax, 0x1000          
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx+0x6], ax     
                                    ; util.c:1350
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xfff0          
OR        ax, 0x0000          
LES       bx, [bp-lpord]            ; bx, [bp-0x8]
MOV       es:[bx+0x6], ax     
                                    ; util.c:1351
CMP       [sel+0x16], 0xffff        ; [0x496c], 0xffff
JZ        L_3356              

L_3346:
MOV       ax, [sel+0x16]            ; ax, [0x496c]
CMP       [bp-i], ax                ; [bp-0x4], ax
JG        L_3356              

L_3351:                             ; util.c:1352
ADD       [sel+0x16], 0x0001        ; [0x496c], 0x0001

L_3356:                             ; util.c:1354
MOV       ax, [gd+0x4]              ; ax, [0x07ce]
AND       ax, 0xfbff          
OR        ax, 0x0000          
MOV       [gd+0x4], ax              ; [0x07ce], ax
                                    ; util.c:1356
MOV       ax, [bp-lpfl]             ; ax, [bp-0xc]
MOV       dx, [bp-lpfl+0x2]         ; dx, [bp-0xa]

L_336b:                             ; util.c:1357
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


