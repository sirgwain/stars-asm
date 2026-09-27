; DpShieldOfShdef  (util)
;   addr: 0008:0f24  len=469
;   sig:  int32_t DpShieldOfShdef(SHDEF *lpshdef, int16_t iplr)
;   params:
;     SHDEF *          lpshdef        [BP+0x6]
;     int16_t          iplr           [BP+0xa]
;   locals:
;     PART             part           [BP-0x1a]
;     HUL *            lphul          [BP-0x12]
;     int32_t          dpShdef        [BP-0xe]
;     int16_t          ihs            [BP-0xa]
;     HS *             lphs           [BP-0x8]
;     int16_t          chs            [BP-0x4]
;
;   stats: blocks=0  labels=0

L_0f24:                             ; util.c:418
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x001a          
PUSH      si                  
PUSH      di                  
                                    ; util.c:419
MOV       [bp-dpShdef], 0x0000      ; [bp-0xe], 0x0000
MOV       [bp-dpShdef+0x2], 0x0000  ; [bp-0xc], 0x0000
                                    ; util.c:426
MOV       ax, [bp+lpshdef]          ; ax, [bp+0x6]
MOV       dx, [bp+lpshdef+0x2]      ; dx, [bp+0x8]
MOV       [bp-lphul], ax            ; [bp-0x12], ax
MOV       [bp-lphul+0x2], dx        ; [bp-0x10], dx
                                    ; util.c:427
MOV       ax, 0x003a          
MOV       cx, [bp-lphul]            ; cx, [bp-0x12]
MOV       dx, [bp-lphul+0x2]        ; dx, [bp-0x10]
ADD       cx, ax              
MOV       [bp-lphs], cx             ; [bp-0x8], cx
MOV       [bp-lphs+0x2], dx         ; [bp-0x6], dx
                                    ; util.c:428
LES       bx, [bp-lphul]            ; bx, [bp-0x12]
MOV       al, es:[bx+0x7a]    
AND       ax, 0x00ff          
MOV       [bp-chs], ax              ; [bp-0x4], ax
                                    ; util.c:430
MOV       [bp-ihs], 0x0000          ; [bp-0xa], 0x0000
JMP       L_0f7c              

L_0f69:
MOV       ax, [bp-0xa]        
ADD       [bp-ihs], 0x0001          ; [bp-0xa], 0x0001
MOV       ax, [bp-0x8]        
MOV       es, [bp-0x6]        
ADD       [bp-lphs], 0x0004         ; [bp-0x8], 0x0004
MOV       dx, es              

L_0f7c:
MOV       ax, [bp-chs]              ; ax, [bp-0x4]
CMP       [bp-ihs], ax              ; [bp-0xa], ax
JGE       L_107e              

L_0f87:                             ; util.c:431
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0004     
JNZ       L_0feb              

L_0f93:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JBE       L_0feb              

L_0faa:                             ; util.c:433
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-part], ax             ; [bp-0x1a], ax
MOV       [bp-part+0x2], dx         ; [bp-0x18], dx
                                    ; util.c:434
LEA       ax, [bp-part]             ; ax, [bp-0x1a]
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
                                    ; util.c:435
LES       bx, [bp-part+0x4]         ; bx, [bp-0x16]
MOV       ax, es:[bx+0x34]    
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       dx, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       dx, cx              
AND       dx, 0x00ff          
IMUL      dx                  
MOV       dx, 0x0000          
ADD       [bp-dpShdef], ax          ; [bp-0xe], ax
ADC       [bp-dpShdef+0x2], dx      ; [bp-0xc], dx
                                    ; util.c:437
JMP       L_0f69              

L_0feb:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0008     
JNZ       L_1040              

L_0ff7:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JBE       L_1040              

L_100e:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JNZ       L_1040              

L_1020:                             ; util.c:438
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x0032          
IMUL      cx                  
MOV       dx, 0x0000          
ADD       [bp-dpShdef], ax          ; [bp-0xe], ax
ADC       [bp-dpShdef+0x2], dx      ; [bp-0xc], dx
                                    ; util.c:439
JMP       L_0f69              

L_1040:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
CMP       es:[bx], 0x0008     
JNZ       L_0f69              

L_104c:
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JNZ       L_0f69              

L_105e:                             ; util.c:440
LES       bx, [bp-lphs]             ; bx, [bp-0x8]
MOV       ax, es:[bx+0x2]     
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
MOV       cx, 0x0064          
IMUL      cx                  
MOV       dx, 0x0000          
ADD       [bp-dpShdef], ax          ; [bp-0xe], ax
ADC       [bp-dpShdef+0x2], dx      ; [bp-0xc], dx

L_107b:                             ; util.c:442
JMP       L_0f69              

L_107e:
MOV       ax, 0x000d          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_10c1              

L_109e:                             ; util.c:443
MOV       ax, 0x0005          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       cx, 0x0001          
MOV       ax, [bp-dpShdef]          ; ax, [bp-0xe]
MOV       dx, [bp-dpShdef+0x2]      ; dx, [bp-0xc]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
ADD       [bp-dpShdef], ax          ; [bp-0xe], ax
ADC       [bp-dpShdef+0x2], dx      ; [bp-0xc], dx

L_10c1:                             ; util.c:445
MOV       ax, [bp-0xe]        
MOV       dx, [bp-dpShdef+0x2]      ; dx, [bp-0xc]
AND       ax, 0x0000          
AND       dx, 0xffff          
CMP       ax, 0x0000          
JNZ       L_10dd              

L_10d5:
CMP       dx, 0x0000          
JZ        L_10e7              

L_10dd:                             ; util.c:446
MOV       [bp-dpShdef], 0xffff      ; [bp-0xe], 0xffff
MOV       [bp-dpShdef+0x2], 0x0000  ; [bp-0xc], 0x0000

L_10e7:                             ; util.c:448
MOV       ax, [bp-dpShdef]          ; ax, [bp-0xe]
MOV       dx, [bp-0xc]        
MOV       dx, 0x0000          

L_10f3:                             ; util.c:449
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


