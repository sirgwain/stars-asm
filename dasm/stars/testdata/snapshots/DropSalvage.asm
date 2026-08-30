; DropSalvage  (battle)
;   addr: 001f:24dc  len=1142
;   sig:  void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT *ppt)
;   params:
;     THING * *        plpth          [BP+0x6]
;     int32_t *        rgwtMinerals   [BP+0x8]
;     int16_t          iplr           [BP+0xc]
;     POINT *          ppt            [BP+0xe]
;   locals:
;     THING *          lpth           [BP-0x10]
;     int16_t          i              [BP-0xc]
;     int32_t          wt             [BP-0xa]
;     int32_t          wtTotal        [BP-0x6]
;
;   stats: blocks=0  labels=0

L_24dc:                             ; battle.c:775
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0012          
PUSH      si                  
PUSH      di                  
                                    ; battle.c:776
MOV       bx, [bp+plpth]            ; bx, [bp+0x6]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       [bp-lpth], ax             ; [bp-0x10], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0xe], dx
                                    ; battle.c:777
MOV       [bp-wtTotal], 0x0000      ; [bp-0x6], 0x0000
MOV       [bp-wtTotal+0x2], 0x0000  ; [bp-0x4], 0x0000
                                    ; battle.c:781
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_2509              

L_2505:
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_2509:
MOV       ax, [game+0xa]            ; ax, [0x007a]
CMP       [bp-i], ax                ; [bp-0xc], ax
JGE       L_253b              

L_2514:                             ; battle.c:782
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
MOV       dx, [bx+0x2f42]     
MOV       bx, [bp+ppt]              ; bx, [bp+0xe]
CMP       [bx], ax            
JNZ       L_2505              

L_252d:
CMP       [bx+0x2], dx        
JZ        L_294c              

L_2532:
JMP       L_2505              

L_2535:                             ; battle.c:783
JMP       L_294c              

L_2538:                             ; battle.c:785
JMP       L_2505              

L_253b:
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_2565              

L_2543:                             ; battle.c:786
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
ADD       [bp-wtTotal], ax          ; [bp-0x6], ax
ADC       [bp-wtTotal+0x2], dx      ; [bp-0x4], dx
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_2565:
CMP       [bp-i], 0x0003            ; [bp-0xc], 0x0003
JGE       L_25cb              

L_256b:
JMP       L_2543              

L_256e:                             ; battle.c:788
JMP       L_25cb              

L_2571:                             ; battle.c:790
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_25c2              

L_2579:                             ; battle.c:792
MOV       ax, 0x000a          
PUSH      ax                  
CALLF     Random                    ; int16_t Random(int16_t c)
ADD       sp, 0x0002          
CWD       dx, ax              
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgwtMinerals]     ; si, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       si, bx              
MOV       bx, si              
MOV       es, cx              
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; battle.c:793
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
ADD       [bp-wtTotal], ax          ; [bp-0x6], ax
ADC       [bp-wtTotal+0x2], dx      ; [bp-0x4], dx
                                    ; battle.c:794
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_25c2:
CMP       [bp-i], 0x0003            ; [bp-0xc], 0x0003
JL        L_2579              

L_25cb:                             ; battle.c:795
CMP       [bp-wtTotal], 0x0000      ; [bp-0x6], 0x0000
JNZ       L_25dd              

L_25d4:
CMP       [bp-wtTotal+0x2], 0x0000  ; [bp-0x4], 0x0000
JZ        L_2571              

L_25dd:                             ; battle.c:797
CMP       [bp-lpth], 0x0000         ; [bp-0x10], 0x0000
JNZ       L_2657              

L_25e6:
CMP       [bp-lpth+0x2], 0x0000     ; [bp-0xe], 0x0000
JNZ       L_2657              

L_25ef:                             ; battle.c:799
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp+iplr]                 ; [bp+0xc]
CALLF     LpthNew                   ; THING * LpthNew(int16_t iplr, ThingType ith)
ADD       sp, 0x0004          
MOV       [bp-lpth], ax             ; [bp-0x10], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0xe], dx
                                    ; battle.c:800
CMP       [bp-lpth], 0x0000         ; [bp-0x10], 0x0000
JNZ       L_2619              

L_260d:
CMP       [bp-lpth+0x2], 0x0000     ; [bp-0xe], 0x0000
JZ        L_294c              

L_2613:
JMP       L_2619              

L_2616:                             ; battle.c:801
JMP       L_294c              

L_2619:                             ; battle.c:802
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xc3ff          
OR        ax, 0x0000          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x6], ax     
                                    ; battle.c:803
MOV       bx, [bp+ppt]              ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x2], ax     
MOV       es:[bx+0x4], dx     
                                    ; battle.c:804
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xfc00          
OR        ax, 0x03ff          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x6], ax     
                                    ; battle.c:806
JMP       L_26e9              

L_2657:                             ; battle.c:808
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_26cc              

L_265f:                             ; battle.c:810
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0xe]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
CWD       dx, ax              
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgwtMinerals]     ; si, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       si, bx              
MOV       bx, si              
MOV       es, cx              
ADD       es:[bx], ax         
ADC       es:[bx+0x2], dx     
                                    ; battle.c:811
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0xe]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
CWD       dx, ax              
ADD       [bp-wtTotal], ax          ; [bp-0x6], ax
ADC       [bp-wtTotal+0x2], dx      ; [bp-0x4], dx
                                    ; battle.c:812
MOV       ax, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       cx, [bp-lpth+0x2]         ; cx, [bp-0xe]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       es:[bx], 0x0000     
                                    ; battle.c:813
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_26cc:
CMP       [bp-i], 0x0003            ; [bp-0xc], 0x0003
JL        L_265f              

L_26d5:                             ; battle.c:814
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0xe]     
AND       ax, 0xc000          
OR        ax, 0x0000          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0xe], ax     

L_26e9:                             ; battle.c:817
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xbfff          
OR        ax, 0x4000          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x6], ax     
                                    ; battle.c:819
JMP       L_2927              

L_2700:                             ; battle.c:821
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_291e              

L_2708:                             ; battle.c:823
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       bx, [bp-i]                ; bx, [bp-0xc]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bp+rgwtMinerals]     ; si, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       si, bx              
MOV       bx, si              
MOV       es, cx              
ADD       ax, es:[bx]         
ADC       dx, es:[bx+0x2]     
CMP       dx, 0x0000          
JL        L_2842              

L_2746:
JG        L_2753              

L_274b:
CMP       ax, 0x7530          
JBE       L_2842              

L_2753:                             ; battle.c:825
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
MOV       cx, 0x7530          
MOV       bx, 0x0000          
SUB       cx, ax              
SBB       bx, dx              
MOV       [bp-wt], cx               ; [bp-0xa], cx
MOV       [bp-wt+0x2], bx           ; [bp-0x8], bx
                                    ; battle.c:826
MOV       ax, [bp-wt]               ; ax, [bp-0xa]
MOV       dx, [bp-wt+0x2]           ; dx, [bp-0x8]
SUB       [bp-wtTotal], ax          ; [bp-0x6], ax
SBB       [bp-wtTotal+0x2], dx      ; [bp-0x4], dx
                                    ; battle.c:827
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0xe]     
AND       ax, 0xc000          
OR        ax, 0x0bb8          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0xe], ax     
                                    ; battle.c:828
MOV       ax, [bp-wt]               ; ax, [bp-0xa]
MOV       dx, [bp-0x8]        
MOV       cx, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       si, [bp-lpth+0x2]         ; si, [bp-0xe]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
ADD       es:[bx], ax         
                                    ; battle.c:829
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, [bp-wt]               ; ax, [bp-0xa]
MOV       dx, [bp-wt+0x2]           ; dx, [bp-0x8]
SUB       es:[bx], ax         
SBB       es:[bx+0x2], dx     
                                    ; battle.c:832
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp+iplr]                 ; [bp+0xc]
CALLF     LpthNew                   ; THING * LpthNew(int16_t iplr, ThingType ith)
ADD       sp, 0x0004          
MOV       [bp-lpth], ax             ; [bp-0x10], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0xe], dx
                                    ; battle.c:833
CMP       [bp-lpth], 0x0000         ; [bp-0x10], 0x0000
JNZ       L_2804              

L_27f8:
CMP       [bp-lpth+0x2], 0x0000     ; [bp-0xe], 0x0000
JZ        L_294c              

L_27fe:
JMP       L_2804              

L_2801:                             ; battle.c:834
JMP       L_294c              

L_2804:                             ; battle.c:835
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xc3ff          
OR        ax, 0x0000          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x6], ax     
                                    ; battle.c:836
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xfc00          
OR        ax, 0x03ff          
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x6], ax     
                                    ; battle.c:837
MOV       bx, [bp+ppt]              ; bx, [bp+0xe]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       es:[bx+0x2], ax     
MOV       es:[bx+0x4], dx     
                                    ; battle.c:839
JMP       L_2900              

L_2842:                             ; battle.c:841
MOV       ax, 0x000a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
ADD       ax, 0x0009          
ADC       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
ADD       ax, es:[bx+0xe]     
AND       ax, 0x3fff          
MOV       [bp-0x12], ax       
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
AND       es:[bx+0xe], 0xc000 
MOV       ax, es:[bx+0xe]     
LES       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       ax, [bp-0x12]       
OR        es:[bx+0xe], ax     
MOV       ax, es:[bx+0xe]     
                                    ; battle.c:842
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       cx, 0x0008          
MOV       bx, [bp-lpth]             ; bx, [bp-0x10]
MOV       si, [bp-lpth+0x2]         ; si, [bp-0xe]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xc]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
ADD       es:[bx], ax         
                                    ; battle.c:843
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
SUB       [bp-wtTotal], ax          ; [bp-0x6], ax
SBB       [bp-wtTotal+0x2], dx      ; [bp-0x4], dx
                                    ; battle.c:844
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgwtMinerals]     ; bx, [bp+0x8]
MOV       cx, [bp+rgwtMinerals+0x2] ; cx, [bp+0xa]
ADD       bx, ax              
MOV       es, cx              
MOV       es:[bx], 0x0000     
MOV       es:[bx+0x2], 0x0000 

L_2900:                             ; battle.c:846
CMP       [bp-wtTotal+0x2], 0x0000  ; [bp-0x4], 0x0000
JG        L_291a              

L_2909:
JL        L_2927              

L_290e:
CMP       [bp-wtTotal], 0x0000      ; [bp-0x6], 0x0000
JBE       L_2927              

L_2914:
JMP       L_291a              

L_2917:                             ; battle.c:847
JMP       L_2927              

L_291a:                             ; battle.c:848
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_291e:
CMP       [bp-i], 0x0003            ; [bp-0xc], 0x0003
JL        L_2708              

L_2927:                             ; battle.c:849
CMP       [bp-wtTotal+0x2], 0x0000  ; [bp-0x4], 0x0000
JG        L_2700              

L_2930:
JL        L_293e              

L_2935:
CMP       [bp-wtTotal], 0x0000      ; [bp-0x6], 0x0000
JA        L_2700              

L_293e:                             ; battle.c:850
MOV       ax, [bp-lpth]             ; ax, [bp-0x10]
MOV       dx, [bp-lpth+0x2]         ; dx, [bp-0xe]
MOV       bx, [bp+plpth]            ; bx, [bp+0x6]
MOV       [bx], ax            
MOV       [bx+0x2], dx        

L_294c:                             ; battle.c:851
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


