; WrapTextOut  (utilgen)
;   addr: 0009:25fe  len=525
;   sig:  void WrapTextOut(HDC hdc, int16_t *px, int16_t *py, char *psz, int16_t cLen, int16_t xLeft, int16_t dxWidth, int16_t *pxMax, int16_t fNewLine, int16_t fPrint)
;   params:
;     HDC              hdc            [BP+0x6]
;     int16_t *        px             [BP+0x8]
;     int16_t *        py             [BP+0xa]
;     char *           psz            [BP+0xc]
;     int16_t          cLen           [BP+0xe]
;     int16_t          xLeft          [BP+0x10]
;     int16_t          dxWidth        [BP+0x12]
;     int16_t *        pxMax          [BP+0x14]
;     int16_t          fNewLine       [BP+0x16]
;     int16_t          fPrint         [BP+0x18]
;   locals:
;     char *           pch            [BP-0x10]
;     char *           pchStart       [BP-0xe]
;     int16_t          dx             [BP-0xc]
;     int16_t          xRight         [BP-0xa]
;     int16_t          fItFit         [BP-0x8]
;     char *           pchEnd         [BP-0x6]
;     int16_t          dxRemain       [BP-0x4]
;
;   stats: blocks=0  labels=3
;     WrapIt: L_27c5
;     Done: L_2802
;     Top: L_2656

L_25fe:                             ; utilgen.c:572
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0010          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:576
MOV       ax, [bp+xLeft]            ; ax, [bp+0x10]
ADD       ax, [bp+dxWidth]          ; ax, [bp+0x12]
MOV       [bp-xRight], ax           ; [bp-0xa], ax
                                    ; utilgen.c:579
MOV       bx, [bp+px]               ; bx, [bp+0x8]
MOV       ax, [bx]            
SUB       ax, [bp+xLeft]            ; ax, [bp+0x10]
MOV       cx, [bp+dxWidth]          ; cx, [bp+0x12]
SUB       cx, ax              
MOV       [bp-dxRemain], cx         ; [bp-0x4], cx
                                    ; utilgen.c:581
CMP       [bp+cLen], 0x0000         ; [bp+0xe], 0x0000
JNZ       L_2637              

L_2629:                             ; utilgen.c:582
PUSH      [bp+psz]                  ; [bp+0xc]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
MOV       [bp+cLen], ax             ; [bp+0xe], ax

L_2637:                             ; utilgen.c:584
CMP       [bp+fNewLine], 0x0000     ; [bp+0x16], 0x0000
JZ        L_2650              

L_2640:                             ; utilgen.c:586
MOV       ax, [dyArial8]            ; ax, [0x23fa]
MOV       bx, [bp+py]               ; bx, [bp+0xa]
ADD       [bx], ax            
                                    ; utilgen.c:587
MOV       ax, [bp+xLeft]            ; ax, [bp+0x10]
MOV       bx, [bp+px]               ; bx, [bp+0x8]
MOV       [bx], ax            

L_2650:                             ; utilgen.c:590
MOV       ax, [bp+psz]              ; ax, [bp+0xc]
MOV       [bp-pchStart], ax         ; [bp-0xe], ax

Top:                                ; utilgen.c:593
MOV       ax, [bp-pchStart]         ; ax, [bp-0xe]
MOV       [bp-pch], ax              ; [bp-0x10], ax
                                    ; utilgen.c:594
MOV       ax, [bp+cLen]             ; ax, [bp+0xe]
MOV       cx, [bp-pchStart]         ; cx, [bp-0xe]
ADD       cx, ax              
MOV       [bp-pchEnd], cx           ; [bp-0x6], cx
                                    ; utilgen.c:595
LEA       ax, [bp-pchEnd]           ; ax, [bp-0x6]
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x10]
CALLF     ChopTrailingSpaces        ; void ChopTrailingSpaces(char *pBeg, char **ppEnd)
ADD       sp, 0x0004          
                                    ; utilgen.c:596
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, [bp-pch]              ; ax, [bp-0x10]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pch]              ; ax, [bp-0x10]
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0xc], ax
                                    ; utilgen.c:597
MOV       [bp-fItFit], 0x0001       ; [bp-0x8], 0x0001

L_2694:                             ; utilgen.c:598
MOV       ax, [bp-dxRemain]         ; ax, [bp-0x4]
CMP       [bp-dx], ax               ; [bp-0xc], ax
JLE       L_26e3              

L_269f:
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
CMP       [bp-pch], ax              ; [bp-0x10], ax
JNC       L_26e3              

L_26aa:
CMP       [bp-dx], 0x0000           ; [bp-0xc], 0x0000
JLE       L_26e3              

L_26b3:                             ; utilgen.c:600
MOV       [bp-fItFit], 0x0000       ; [bp-0x8], 0x0000
                                    ; utilgen.c:601
LEA       ax, [bp-pchEnd]           ; ax, [bp-0x6]
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x10]
CALLF     ChopLastWord              ; void ChopLastWord(char *pBeg, char **ppEnd)
ADD       sp, 0x0004          
                                    ; utilgen.c:602
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, [bp-pch]              ; ax, [bp-0x10]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pch]              ; ax, [bp-0x10]
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0xc], ax
                                    ; utilgen.c:603
JMP       L_2694              

L_26e3:                             ; utilgen.c:605
CMP       [bp-fItFit], 0x0000       ; [bp-0x8], 0x0000
JZ        L_271a              

L_26ec:                             ; utilgen.c:607
MOV       ax, [bp+cLen]             ; ax, [bp+0xe]
MOV       cx, [bp-pchStart]         ; cx, [bp-0xe]
ADD       cx, ax              
PUSH      cx                  
LEA       ax, [bp-pchEnd]           ; ax, [bp-0x6]
PUSH      ax                  
CALLF     AddBackTrailingSpaces     ; void AddBackTrailingSpaces(char **ppch, char *pchEnd)
ADD       sp, 0x0004          
                                    ; utilgen.c:608
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, [bp-pchStart]         ; ax, [bp-0xe]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pchStart]         ; ax, [bp-0xe]
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0xc], ax

L_271a:                             ; utilgen.c:611
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
CMP       [bp-pchStart], ax         ; [bp-0xe], ax
JNZ       L_275f              

L_2728:
MOV       bx, [bp+px]               ; bx, [bp+0x8]
MOV       ax, [bp+xLeft]            ; ax, [bp+0x10]
CMP       [bx], ax            
JNZ       WrapIt              

L_2735:                             ; utilgen.c:616
MOV       ax, [bp+cLen]             ; ax, [bp+0xe]
MOV       cx, [bp-pchStart]         ; cx, [bp-0xe]
ADD       cx, ax              
MOV       [bp-pchEnd], cx           ; [bp-0x6], cx
                                    ; utilgen.c:617
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       ax, [bp-pchStart]         ; ax, [bp-0xe]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pchStart]         ; ax, [bp-0xe]
PUSH      ax                  
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
MOV       [bp-dx], ax               ; [bp-0xc], ax

L_275f:                             ; utilgen.c:622
CMP       [bp+fPrint], 0x0000       ; [bp+0x18], 0x0000
JZ        L_2788              

L_2768:                             ; utilgen.c:623
PUSH      [bp+hdc]                  ; [bp+0x6]
MOV       bx, [bp+px]               ; bx, [bp+0x8]
PUSH      [bx]                
MOV       bx, [bp+py]               ; bx, [bp+0xa]
PUSH      [bx]                
MOV       ax, [bp-pchStart]         ; ax, [bp-0xe]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pchStart]         ; ax, [bp-0xe]
PUSH      ax                  
CALLF     TextOut                   ; int16_t TextOut(HDC arg1, int16_t arg2, int16_t arg3, LPCSTR arg4, int16_t arg5)

L_2788:                             ; utilgen.c:624
MOV       ax, [bp-dx]               ; ax, [bp-0xc]
MOV       bx, [bp+px]               ; bx, [bp+0x8]
ADD       [bx], ax            
                                    ; utilgen.c:625
CMP       [bp+pxMax], 0x0000        ; [bp+0x14], 0x0000
JZ        L_27b2              

L_2799:
MOV       bx, [bp+pxMax]            ; bx, [bp+0x14]
MOV       ax, [bx]            
MOV       bx, [bp+px]               ; bx, [bp+0x8]
CMP       [bx], ax            
JLE       L_27b2              

L_27a8:                             ; utilgen.c:626
MOV       bx, [bp+px]               ; bx, [bp+0x8]
MOV       ax, [bx]            
MOV       bx, [bp+pxMax]            ; bx, [bp+0x14]
MOV       [bx], ax            

L_27b2:                             ; utilgen.c:628
MOV       ax, [bp+cLen]             ; ax, [bp+0xe]
MOV       cx, [bp-pchStart]         ; cx, [bp-0xe]
ADD       cx, ax              
CMP       [bp-pchEnd], cx           ; [bp-0x6], cx
JZ        L_2805              

WrapIt:                             ; utilgen.c:632
MOV       ax, [bp+cLen]             ; ax, [bp+0xe]
MOV       cx, [bp-pchStart]         ; cx, [bp-0xe]
ADD       cx, ax              
PUSH      cx                  
LEA       ax, [bp-pchEnd]           ; ax, [bp-0x6]
PUSH      ax                  
CALLF     AddBackTrailingSpaces     ; void AddBackTrailingSpaces(char **ppch, char *pchEnd)
ADD       sp, 0x0004          
                                    ; utilgen.c:633
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
SUB       ax, [bp-pchStart]         ; ax, [bp-0xe]
SUB       [bp+cLen], ax             ; [bp+0xe], ax
                                    ; utilgen.c:634
MOV       ax, [bp-pchEnd]           ; ax, [bp-0x6]
MOV       [bp-pchStart], ax         ; [bp-0xe], ax
                                    ; utilgen.c:635
MOV       ax, [dyArial8]            ; ax, [0x23fa]
MOV       bx, [bp+py]               ; bx, [bp+0xa]
ADD       [bx], ax            
                                    ; utilgen.c:636
MOV       ax, [bp+xLeft]            ; ax, [bp+0x10]
MOV       bx, [bp+px]               ; bx, [bp+0x8]
MOV       [bx], ax            
                                    ; utilgen.c:637
MOV       ax, [bp+dxWidth]          ; ax, [bp+0x12]
MOV       [bp-dxRemain], ax         ; [bp-0x4], ax
                                    ; utilgen.c:638
JMP       Top                 

L_2805:                             ; utilgen.c:642
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


