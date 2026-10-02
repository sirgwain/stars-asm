; EnsureMacintiStarbaseDesigns  (aiutil)
;   addr: 0013:76e4  len=1253
;   sig:  void EnsureMacintiStarbaseDesigns(uint8_t *rgSB)
;   params:
;     uint8_t *        rgSB           [BP+0x6]
;   locals:
;     int16_t          iNew           [BP-0xe]
;     int16_t          j              [BP-0xc]
;     int16_t          i              [BP-0xa]
;     int16_t          cAge           [BP-0x8]
;     int16_t          iOld           [BP-0x6]
;     int16_t          k              [BP-0x4]
;
;   stats: blocks=0  labels=0

L_76e4:                             ; aiutil.c:3127
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0010          
PUSH      si                  
PUSH      di                  
                                    ; aiutil.c:3136
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
MOV       [bx], 0x0000        
                                    ; aiutil.c:3139
MOV       [bp-i], 0x0001            ; [bp-0xa], 0x0001
JMP       L_7858              

L_76fb:                             ; aiutil.c:3142
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
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
JNZ       L_7759              

L_7729:
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx+0x83], 0x0000
JNZ       L_77ad              

L_774e:
CMP       es:[bx+0x85], 0x0000
JNZ       L_77ad              

L_7759:                             ; aiutil.c:3145
PUSH      [bp-i]                    ; [bp-0xa]
MOV       bx, [bp-i]                ; bx, [bp-0xa]
ADD       bx, 0xffff          
MOV       al, cs:[bx+0x76de]  
AND       ax, 0x00ff          
PUSH      ax                  
CMP       [bp-i], 0x0003            ; [bp-0xa], 0x0003
JGE       L_777a              

L_7774:
MOV       ax, 0x0001          
JMP       L_777d              

L_777a:
MOV       ax, 0x0002          

L_777d:
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     FCreateAiStarbase         ; int16_t FCreateAiStarbase(int16_t ishdef, int16_t iLevel, int16_t aisb, isbhull isb)
ADD       sp, 0x0008          
CMP       ax, 0x0000          
JZ        L_779f              

L_7791:                             ; aiutil.c:3146
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0000        
                                    ; aiutil.c:3147
JMP       L_7854              

L_779f:                             ; aiutil.c:3148
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0001        

L_77aa:                             ; aiutil.c:3150
JMP       L_7854              

L_77ad:                             ; aiutil.c:3152
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       ax, [game+0x12]           ; ax, [0x0082]
MOV       es, cx              
SUB       ax, es:[bx+0x7d]    
MOV       [bp-cAge], ax             ; [bp-0x8], ax
                                    ; aiutil.c:3153
CMP       [bp-cAge], 0x0023         ; [bp-0x8], 0x0023
JGE       L_7832              

L_77da:                             ; aiutil.c:3156
CMP       [game+0x12], 0x0019       ; [0x0082], 0x0019
JBE       L_7824              

L_77e4:
CMP       [bp-i], 0x0001            ; [bp-0xa], 0x0001
JNZ       L_7824              

L_77ed:
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       al, es:[bx+0x7a]    
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JZ        L_7824              

L_7816:                             ; aiutil.c:3157
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0003        
                                    ; aiutil.c:3158
JMP       L_7854              

L_7824:                             ; aiutil.c:3159
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0000        

L_782f:                             ; aiutil.c:3161
JMP       L_7854              

L_7832:
CMP       [bp-cAge], 0x0032         ; [bp-0x8], 0x0032
JGE       L_7849              

L_783b:                             ; aiutil.c:3162
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0002        
                                    ; aiutil.c:3163
JMP       L_7854              

L_7849:                             ; aiutil.c:3164
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0003        

L_7854:                             ; aiutil.c:3166
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_7858:
CMP       [bp-i], 0x0003            ; [bp-0xa], 0x0003
JLE       L_76fb              

L_7861:                             ; aiutil.c:3169
MOV       [bp-iOld], 0xffff         ; [bp-0x6], 0xffff
                                    ; aiutil.c:3170
MOV       [bp-i], 0x0001            ; [bp-0xa], 0x0001
JMP       L_792f              

L_786e:                             ; aiutil.c:3171
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JL        L_792b              

L_7883:                             ; aiutil.c:3176
CMP       [bp-iOld], 0xffff         ; [bp-0x6], 0xffff
JZ        L_7925              

L_788c:
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       cx, [bp-iOld]             ; cx, [bp-0x6]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, cx              
MOV       cl, [bx]            
MOV       [bp-0x10], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
MOV       cx, [bp-0x10]       
CMP       cx, ax              
JG        L_7925              

L_78b5:
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       cx, [bp-iOld]             ; cx, [bp-0x6]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, cx              
MOV       cl, [bx]            
MOV       [bp-0x10], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
MOV       cx, [bp-0x10]       
CMP       cx, ax              
JNZ       L_792b              

L_78de:
MOV       ax, 0x0093          
IMUL      [bp-iOld]                 ; [bp-0x6]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx+0x7d]    
MOV       [bp-0x10], ax       
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       ax, [bp-0x10]       
MOV       es, cx              
CMP       es:[bx+0x7d], ax    
JNC       L_792b              

L_7925:                             ; aiutil.c:3177
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       [bp-iOld], ax             ; [bp-0x6], ax

L_792b:                             ; aiutil.c:3180
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_792f:
CMP       [bp-i], 0x0003            ; [bp-0xa], 0x0003
JLE       L_786e              

L_7938:
MOV       [bp-i], 0x0001            ; [bp-0xa], 0x0001
JMP       L_796f              

L_7940:                             ; aiutil.c:3181
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JL        L_796b              

L_7955:
MOV       ax, [bp-iOld]             ; ax, [bp-0x6]
CMP       [bp-i], ax                ; [bp-0xa], ax
JZ        L_796b              

L_7960:                             ; aiutil.c:3182
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, ax              
MOV       [bx], 0x0000        

L_796b:                             ; aiutil.c:3185
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_796f:
CMP       [bp-i], 0x0003            ; [bp-0xa], 0x0003
JLE       L_7940              

L_7978:
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_7a82              

L_7980:                             ; aiutil.c:3187
MOV       [bp-j], 0x0000            ; [bp-0xc], 0x0000
JMP       L_7a08              

L_7988:                             ; aiutil.c:3189
MOV       ax, 0x0003          
IMUL      [bp-i]                    ; [bp-0xa]
ADD       ax, 0x0004          
ADD       ax, [bp-j]                ; ax, [bp-0xc]
MOV       cx, 0x0093          
IMUL      cx                  
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
JNZ       L_7a04              

L_79c1:
MOV       ax, 0x0003          
IMUL      [bp-i]                    ; [bp-0xa]
ADD       ax, 0x0004          
ADD       ax, [bp-j]                ; ax, [bp-0xc]
MOV       cx, 0x0093          
IMUL      cx                  
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx+0x85], 0x0000
JC        L_7a04              

L_79f1:
JA        L_7a11              

L_79f6:
CMP       es:[bx+0x83], 0x0000
JA        L_7a11              

L_7a04:                             ; aiutil.c:3192
ADD       [bp-j], 0x0001            ; [bp-0xc], 0x0001

L_7a08:
CMP       [bp-j], 0x0003            ; [bp-0xc], 0x0003
JL        L_7988              

L_7a11:
CMP       [bp-j], 0x0003            ; [bp-0xc], 0x0003
JNZ       L_7a7e              

L_7a1a:                             ; aiutil.c:3194
MOV       [bp-j], 0x0000            ; [bp-0xc], 0x0000
JMP       L_7a75              

L_7a22:                             ; aiutil.c:3196
MOV       [bp-k], 0x0005            ; [bp-0x4], 0x0005
JMP       L_7a68              

L_7a2a:                             ; aiutil.c:3198
MOV       ax, [bp-k]                ; ax, [bp-0x4]
ADD       ax, 0xffff          
PUSH      ax                  
MOV       bx, [bp-k]                ; bx, [bp-0x4]
MOV       al, cs:[bx+0x76de]  
AND       ax, 0x00ff          
PUSH      ax                  
MOV       ax, [bp-j]                ; ax, [bp-0xc]
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0003          
IMUL      [bp-i]                    ; [bp-0xa]
ADD       ax, 0x0004          
ADD       ax, [bp-j]                ; ax, [bp-0xc]
PUSH      ax                  
CALLF     FCreateAiStarbase         ; int16_t FCreateAiStarbase(int16_t ishdef, int16_t iLevel, int16_t aisb, isbhull isb)
ADD       sp, 0x0008          
CMP       ax, 0x0000          
JNZ       L_7a71              

L_7a64:                             ; aiutil.c:3200
SUB       [bp-k], 0x0001            ; [bp-0x4], 0x0001

L_7a68:
CMP       [bp-k], 0x0003            ; [bp-0x4], 0x0003
JGE       L_7a2a              

L_7a71:
ADD       [bp-j], 0x0001            ; [bp-0xc], 0x0001

L_7a75:
CMP       [bp-j], 0x0003            ; [bp-0xc], 0x0003
JL        L_7a22              

L_7a7e:                             ; aiutil.c:3202
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_7a82:
CMP       [bp-i], 0x0002            ; [bp-0xa], 0x0002
JL        L_7980              

L_7a8b:                             ; aiutil.c:3204
MOV       [bp-i], 0x0004            ; [bp-0xa], 0x0004
JMP       L_7ad8              

L_7a93:                             ; aiutil.c:3205
MOV       ax, 0x0093          
IMUL      [bp-i]                    ; [bp-0xa]
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
JZ        L_7ac7              

L_7ac1:
MOV       ax, 0x0001          
JMP       L_7aca              

L_7ac7:
MOV       ax, 0x0000          

L_7aca:
MOV       cx, [bp-i]                ; cx, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, cx              
MOV       [bx], al            
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_7ad8:
CMP       [bp-i], 0x000a            ; [bp-0xa], 0x000a
JL        L_7a93              

L_7ae1:                             ; aiutil.c:3207
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LES       bx, [bx+0x14c]      
MOV       ax, es:[bx+0x482]   
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
LES       bx, [bx+0x14c]      
CMP       es:[bx+0x2c9], ax   
JC        L_7b15              

L_7b08:                             ; aiutil.c:3209
MOV       [bp-iNew], 0x0004         ; [bp-0xe], 0x0004
                                    ; aiutil.c:3210
MOV       [bp-iOld], 0x0007         ; [bp-0x6], 0x0007
                                    ; aiutil.c:3212
JMP       L_7b1f              

L_7b15:                             ; aiutil.c:3214
MOV       [bp-iNew], 0x0007         ; [bp-0xe], 0x0007
                                    ; aiutil.c:3215
MOV       [bp-iOld], 0x0004         ; [bp-0x6], 0x0004

L_7b1f:                             ; aiutil.c:3218
MOV       ax, 0x0093          
IMUL      [bp-iOld]                 ; [bp-0x6]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       ax, [game+0x12]           ; ax, [0x0082]
MOV       es, cx              
SUB       ax, es:[bx+0x7d]    
CMP       ax, 0x001e          
JNC       L_7b50              

L_7b48:                             ; aiutil.c:3219
MOV       [bp-j], 0x0002            ; [bp-0xc], 0x0002
                                    ; aiutil.c:3220
JMP       L_7b55              

L_7b50:                             ; aiutil.c:3221
MOV       [bp-j], 0x0003            ; [bp-0xc], 0x0003

L_7b55:                             ; aiutil.c:3223
MOV       ax, [bp-iOld]             ; ax, [bp-0x6]
MOV       [bp-i], ax                ; [bp-0xa], ax
JMP       L_7b62              

L_7b5e:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_7b62:
MOV       ax, [bp-iOld]             ; ax, [bp-0x6]
ADD       ax, 0x0003          
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_7b80              

L_7b70:                             ; aiutil.c:3224
MOV       ax, [bp-j]                ; ax, [bp-0xc]
MOV       cx, [bp-i]                ; cx, [bp-0xa]
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
ADD       bx, cx              
MOV       [bx], al            
JMP       L_7b5e              

L_7b80:                             ; aiutil.c:3226
MOV       ax, 0x0093          
IMUL      [bp-iNew]                 ; [bp-0xe]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14e]      
MOV       bx, [bx+0x14c]      
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
ADD       ax, 0xffe0          
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; aiutil.c:3227
CMP       [bp-i], 0x0004            ; [bp-0xa], 0x0004
JGE       L_7bc3              

L_7bac:                             ; aiutil.c:3229
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
MOV       [bx+0x3], 0x0002    
                                    ; aiutil.c:3231
CMP       [bp-i], 0x0003            ; [bp-0xa], 0x0003
JGE       L_7bc3              

L_7bbc:                             ; aiutil.c:3232
MOV       bx, [bp+rgSB]             ; bx, [bp+0x6]
MOV       [bx+0x2], 0x0002    

L_7bc3:                             ; aiutil.c:3234
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


