; HfontPrinterCreate  (utilgen)
;   addr: 0009:6aa6  len=202
;   sig:  HFONT HfontPrinterCreate(HDC hdc, int16_t iSize, int16_t *pdyFont)
;   params:
;     HDC              hdc            
;     int16_t          iSize          
;     int16_t *        pdyFont        
;   locals:
;     LOGFONT *        plf            [BP-0x6]
;     HFONT            hfontNew       [BP-0x4]
;     block 0009:6B28  len=0x33
;       HFONT            hfontSav       [BP-0x28]
;       TEXTMETRIC       tm             [BP-0x26]
;
;   stats: blocks=1  labels=0

L_6aa6:                             ; utilgen.c:2702
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0028          
PUSH      si                  
PUSH      di                  
                                    ; utilgen.c:2705
MOV       ax, 0x0040          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
CALLF     LocalAlloc                ; HLOCAL LocalAlloc(uint16_t arg1, uint16_t arg2)
MOV       [bp-plf], ax              ; [bp-0x6], ax
                                    ; utilgen.c:2707
MOV       ax, 0x0032          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-plf]                  ; [bp-0x6]
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; utilgen.c:2708
PUSH      [bp+0x8]            
PUSH      [bp+0x6]            
MOV       ax, 0x005a          
PUSH      ax                  
CALLF     GetDeviceCaps             ; int16_t GetDeviceCaps(HDC arg1, DeviceCapsIndex arg2)
PUSH      ax                  
MOV       ax, 0x0048          
PUSH      ax                  
CALLF     MulDiv                    ; int16_t MulDiv(int16_t arg1, int16_t arg2, int16_t arg3)
NEG       ax                  
MOV       bx, [bp-plf]              ; bx, [bp-0x6]
MOV       [bx], ax            
                                    ; utilgen.c:2709
MOV       ax, 0x06c6          
PUSH      ax                  
MOV       ax, 0x0012          
MOV       cx, [bp-plf]              ; cx, [bp-0x6]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; utilgen.c:2710
MOV       ax, [bp-plf]              ; ax, [bp-0x6]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     CreateFontIndirect        ; HFONT CreateFontIndirect(LOGFONT *arg1)
MOV       [bp-hfontNew], ax         ; [bp-0x4], ax
                                    ; utilgen.c:2712
CMP       [bp+0xa], 0x0000    
JZ        L_6b5b              

L_6b1f:
CMP       [bp-hfontNew], 0x0000     ; [bp-0x4], 0x0000
JZ        L_6b5b              

L_6b28:                             ; utilgen.c:2715
PUSH      [bp+0x6]            
PUSH      [bp-hfontNew]             ; [bp-0x4]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
MOV       [bp-hfontSav], ax         ; [bp-0x28], ax
                                    ; utilgen.c:2717
PUSH      [bp+0x6]            
LEA       ax, [bp-tm]               ; ax, [bp-0x26]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetTextMetrics            ; int16_t GetTextMetrics(HDC arg1, TEXTMETRIC *arg2)
                                    ; utilgen.c:2718
MOV       ax, [bp-tm]               ; ax, [bp-0x26]
ADD       ax, [bp-tm+0x8]           ; ax, [bp-0x1e]
MOV       bx, [bp+0xa]        
MOV       [bx], ax            
                                    ; utilgen.c:2720
PUSH      [bp+0x6]            
PUSH      [bp-hfontSav]             ; [bp-0x28]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)

L_6b5b:                             ; utilgen.c:2723
MOV       ax, [bp-plf]              ; ax, [bp-0x6]
PUSH      ax                  
CALLF     LocalFree                 ; HLOCAL LocalFree(HLOCAL arg1)
                                    ; utilgen.c:2725
MOV       ax, [bp-hfontNew]         ; ax, [bp-0x4]

L_6b6a:                             ; utilgen.c:2726
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


