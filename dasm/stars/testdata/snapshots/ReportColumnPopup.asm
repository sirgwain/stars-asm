; ReportColumnPopup  (report)
;   addr: 0022:74d4  len=1569
;   sig:  void ReportColumnPopup(POINT16 pt, int16_t icol, int16_t fRightBtn)
;   params:
;     POINT16          pt             [BP+0x6]
;     int16_t          icol           [BP+0xa]
;     int16_t          fRightBtn      [BP+0xc]
;   locals:
;     int16_t          iSortLast      [BP-0x73e]
;     int16_t          iHide          [BP-0x73c]
;     int16_t          iRet           [BP-0x73a]
;     int16_t          cch            [BP-0x738]
;     char *[32]       psz            [BP-0x736]
;     int16_t          cItems         [BP-0x6f6]
;     char[50]         szColTitle     [BP-0x6f4]
;     int16_t[32]      rgcol          [BP-0x6c2]
;     int16_t          fccolChange    [BP-0x682]
;     int16_t          ibit           [BP-0x680]
;     int16_t          i              [BP-0x67e]
;     int16_t          j              [BP-0x67c]
;     int16_t          cSubsort       [BP-0x67a]
;     int16_t          iBase          [BP-0x678]
;     char[50][32]     rgsz           [BP-0x676]
;     char[50]         szT            [BP-0x36]
;     HDC              hdc            [BP-0x4]
;
;   stats: blocks=0  labels=0

L_74d4:                             ; report.c:3119
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0740          
PUSH      si                  
PUSH      di                  
                                    ; report.c:3121
MOV       [bp-cSubsort], 0x0000     ; [bp-0x67a], 0x0000
                                    ; report.c:3131
MOV       [bp-fccolChange], 0x0000  ; [bp-0x682], 0x0000
                                    ; report.c:3139
PUSH      [hwndReportDlg]           ; [0x15c4]
CALLF     GetDC                     ; HDC GetDC(HWND arg1)
MOV       [bp-hdc], ax              ; [bp-0x4], ax
                                    ; report.c:3140
PUSH      [bp-hdc]                  ; [bp-0x4]
LEA       ax, [bp-szColTitle]       ; ax, [bp-0x6f4]
PUSH      ax                  
PUSH      [bp+icol]                 ; [bp+0xa]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
CALLF     DxReportColHdr            ; int16_t DxReportColHdr(ReportType irpt, int16_t iCol, char *psz, HDC hdc)
ADD       sp, 0x0008          
                                    ; report.c:3142
MOV       [bp-cItems], 0x0000       ; [bp-0x6f6], 0x0000
                                    ; report.c:3144
MOV       [bp-i], 0x0000            ; [bp-0x67e], 0x0000
JMP       L_76bb              

L_751e:                             ; report.c:3147
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CMP       [bp-i], 0x0000            ; [bp-0x67e], 0x0000
JNZ       L_753c              

L_7536:
MOV       ax, 0x046d          
JMP       L_753f              

L_753c:
MOV       ax, 0x046e          

L_753f:
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x738], ax
                                    ; report.c:3148
LEA       ax, [bp-szColTitle]       ; ax, [bp-0x6f4]
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
MOV       ax, [bp-cch]              ; ax, [bp-0x738]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3149
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3153
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0000    
JNZ       L_759a              

L_757f:
CMP       [bp+icol], 0x000b         ; [bp+0xa], 0x000b
JZ        L_75b0              

L_7588:
CMP       [bp+icol], 0x0009         ; [bp+0xa], 0x0009
JZ        L_75b0              

L_7591:
CMP       [bp+icol], 0x000a         ; [bp+0xa], 0x000a
JZ        L_75b0              

L_759a:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0001    
JNZ       L_76b6              

L_75a7:
CMP       [bp+icol], 0x0007         ; [bp+0xa], 0x0007
JNZ       L_76b6              

L_75b0:                             ; report.c:3155
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
ADD       ax, 0xffff          
MOV       cx, 0x0032          
IMUL      cx                  
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3156
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
ADD       ax, 0xffff          
MOV       cx, 0x0032          
IMUL      cx                  
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx], 0x0000        
                                    ; report.c:3157
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3159
MOV       [bp-j], 0x0000            ; [bp-0x67c], 0x0000
JMP       L_7601              

L_75fc:
ADD       [bp-j], 0x0001            ; [bp-0x67c], 0x0001

L_7601:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0001    
JNZ       L_7614              

L_760e:
MOV       ax, 0x0001          
JMP       L_7617              

L_7614:
MOV       ax, 0x0000          

L_7617:
ADD       ax, 0x0003          
CMP       [bp-j], ax                ; [bp-0x67c], ax
JGE       L_764b              

L_7623:                             ; report.c:3161
MOV       bx, [bp-j]                ; bx, [bp-0x67c]
SHL       bx, 0x0001          
PUSH      [bx+0x4cc]          
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3162
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3163
JMP       L_75fc              

L_764b:                             ; report.c:3165
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx], 0x00ff        
                                    ; report.c:3166
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx+0x1], 0x0000    
                                    ; report.c:3167
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3169
MOV       ax, 0x055f          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3170
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3172
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
MOV       cx, 0x0032          
IMUL      cx                  
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx], 0x0000        
                                    ; report.c:3173
MOV       [bp-cSubsort], 0x0005     ; [bp-0x67a], 0x0005

L_76b6:                             ; report.c:3175
ADD       [bp-i], 0x0001            ; [bp-0x67e], 0x0001

L_76bb:
CMP       [bp-i], 0x0002            ; [bp-0x67e], 0x0002
JL        L_751e              

L_76c5:                             ; report.c:3177
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx], 0x00ff        
                                    ; report.c:3178
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx+0x1], 0x0000    
                                    ; report.c:3179
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3181
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
MOV       ax, 0x046f          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x738], ax
                                    ; report.c:3182
LEA       ax, [bp-szColTitle]       ; ax, [bp-0x6f4]
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
MOV       ax, [bp-cch]              ; ax, [bp-0x738]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3183
LEA       ax, [bp-szT]              ; ax, [bp-0x36]
PUSH      ax                  
MOV       ax, 0x0470          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x738], ax
                                    ; report.c:3184
LEA       ax, [bp-szT]              ; ax, [bp-0x36]
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3185
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
MOV       [bp-iHide], ax            ; [bp-0x73c], ax
MOV       [bp-iSortLast], ax        ; [bp-0x73e], ax
                                    ; report.c:3186
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3188
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx], 0x00ff        
                                    ; report.c:3189
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       [bx+0x1], 0x0000    
                                    ; report.c:3190
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001
                                    ; report.c:3192
CMP       [bp+icol], 0x0000         ; [bp+0xa], 0x0000
JNZ       L_77a3              

L_7798:                             ; report.c:3194
SUB       [bp-cItems], 0x0002       ; [bp-0x6f6], 0x0002
                                    ; report.c:3195
MOV       [bp-iHide], 0xffff        ; [bp-0x73c], 0xffff

L_77a3:                             ; report.c:3198
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
MOV       [bp-iBase], ax            ; [bp-0x678], ax
                                    ; report.c:3199
MOV       [bp-i], 0x0000            ; [bp-0x67e], 0x0000
MOV       [bp-ibit], 0x0001         ; [bp-0x680], 0x0001
MOV       ax, 0x0001          
JMP       L_77d1              

L_77bd:
MOV       ax, [bp-0x67e]      
ADD       [bp-i], 0x0001            ; [bp-0x67e], 0x0001
MOV       cx, 0x0001          
SHL       [bp-ibit], cx             ; [bp-0x680], cx
MOV       ax, [bp-0x680]      

L_77d1:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x6]        
CMP       [bp-i], ax                ; [bp-0x67e], ax
JGE       L_78a1              

L_77e1:                             ; report.c:3201
MOV       ax, [bp-ibit]             ; ax, [bp-0x680]
CWD       dx, ax              
MOV       bx, [vprptCur]            ; bx, [0x15ac]
AND       ax, [bx]            
AND       dx, [bx+0x2]        
CMP       ax, 0x0000          
JNZ       L_77bd              

L_77f7:
CMP       dx, 0x0000          
JNZ       L_77bd              

L_77ff:                             ; report.c:3203
PUSH      [bp-hdc]                  ; [bp-0x4]
LEA       ax, [bp-szColTitle]       ; ax, [bp-0x6f4]
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0x67e]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
CALLF     DxReportColHdr            ; int16_t DxReportColHdr(ReportType irpt, int16_t iCol, char *psz, HDC hdc)
ADD       sp, 0x0008          
                                    ; report.c:3205
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
MOV       ax, 0x0471          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x738], ax
                                    ; report.c:3206
LEA       ax, [bp-szColTitle]       ; ax, [bp-0x6f4]
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
MOV       ax, [bp-cch]              ; ax, [bp-0x738]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3208
LEA       ax, [bp-szT]              ; ax, [bp-0x36]
PUSH      ax                  
MOV       ax, 0x0470          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x738], ax
                                    ; report.c:3209
LEA       ax, [bp-szT]              ; ax, [bp-0x36]
PUSH      ax                  
MOV       ax, 0x0032          
IMUL      [bp-cItems]               ; [bp-0x6f6]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
PUSH      cx                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; report.c:3211
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
SHL       ax, 0x0001          
LEA       bx, [bp-0x6c2]      
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x67e]
MOV       [bx], ax            
                                    ; report.c:3213
ADD       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001

L_789e:                             ; report.c:3215
JMP       L_77bd              

L_78a1:                             ; report.c:3217
MOV       ax, [bp-iBase]            ; ax, [bp-0x678]
CMP       [bp-cItems], ax           ; [bp-0x6f6], ax
JNZ       L_78b3              

L_78ae:                             ; report.c:3218
SUB       [bp-cItems], 0x0001       ; [bp-0x6f6], 0x0001

L_78b3:                             ; report.c:3220
PUSH      [hwndReportDlg]           ; [0x15c4]
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     ReleaseDC                 ; int16_t ReleaseDC(HWND arg1, HDC arg2)
                                    ; report.c:3222
MOV       [bp-i], 0x0000            ; [bp-0x67e], 0x0000
JMP       L_7913              

L_78c8:                             ; report.c:3224
MOV       ax, 0x0032          
IMUL      [bp-i]                    ; [bp-0x67e]
LEA       bx, [bp-0x676]      
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_78fe              

L_78e0:                             ; report.c:3225
MOV       ax, 0x0032          
IMUL      [bp-i]                    ; [bp-0x67e]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x676]
ADD       cx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x67e]
SHL       ax, 0x0001          
LEA       bx, [bp-0x736]      
ADD       bx, ax              
MOV       [bx], cx            
                                    ; report.c:3226
JMP       L_790e              

L_78fe:                             ; report.c:3227
MOV       ax, [bp-i]                ; ax, [bp-0x67e]
SHL       ax, 0x0001          
LEA       bx, [bp-0x736]      
ADD       bx, ax              
MOV       [bx], 0x0000        

L_790e:                             ; report.c:3228
ADD       [bp-i], 0x0001            ; [bp-0x67e], 0x0001

L_7913:
MOV       ax, [bp-cItems]           ; ax, [bp-0x6f6]
CMP       [bp-i], ax                ; [bp-0x67e], ax
JL        L_78c8              

L_7920:                             ; report.c:3230
PUSH      [bp+fRightBtn]            ; [bp+0xc]
MOV       ax, 0xffff          
PUSH      ax                  
LEA       ax, [bp-psz]              ; ax, [bp-0x736]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-cItems]               ; [bp-0x6f6]
PUSH      [bp+pt+0x2]               ; [bp+0x8]
PUSH      [bp+pt]                   ; [bp+0x6]
PUSH      [hwndReportDlg]           ; [0x15c4]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-iRet], ax             ; [bp-0x73a], ax
                                    ; report.c:3232
CMP       [bp-iRet], 0x0000         ; [bp-0x73a], 0x0000
JL        L_7aef              

L_7957:                             ; report.c:3235
MOV       ax, [gd+0x4]              ; ax, [0x07ce]
AND       ax, 0xff7f          
OR        ax, 0x0080          
MOV       [gd+0x4], ax              ; [0x07ce], ax
                                    ; report.c:3237
MOV       ax, [bp-iSortLast]        ; ax, [bp-0x73e]
CMP       [bp-iRet], ax             ; [bp-0x73a], ax
JGE       L_7a71              

L_7970:                             ; report.c:3239
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0xa]        
MOV       [vicolSortPrev], ax       ; [0x162c], ax
                                    ; report.c:3240
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2e]       
MOV       [viSubsortPrev], ax       ; [0x162e], ax
                                    ; report.c:3241
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0xc]        
MOV       [vfAscendingPrev], ax     ; [0x4b8e], ax
                                    ; report.c:3243
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bp+icol]             ; ax, [bp+0xa]
MOV       [bx+0xa], ax        
                                    ; report.c:3245
CMP       [bp-cSubsort], 0x0000     ; [bp-0x67a], 0x0000
JNZ       L_79bf              

L_79a2:                             ; report.c:3247
CMP       [bp-iRet], 0x0000         ; [bp-0x73a], 0x0000
JNZ       L_79b2              

L_79ac:
MOV       ax, 0x0001          
JMP       L_79b5              

L_79b2:
MOV       ax, 0x0000          

L_79b5:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0xc], ax        
                                    ; report.c:3249
JMP       L_7a5c              

L_79bf:                             ; report.c:3251
MOV       ax, [bp-iRet]             ; ax, [bp-0x73a]
ADD       ax, 0xfffe          
MOV       [bp-0x740], ax      
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0001    
JNZ       L_79dd              

L_79d7:
MOV       ax, 0x0001          
JMP       L_79e0              

L_79dd:
MOV       ax, 0x0000          

L_79e0:
MOV       cx, [bp-cSubsort]         ; cx, [bp-0x67a]
ADD       cx, 0x0003          
ADD       cx, ax              
MOV       ax, [bp-0x740]      
CWD       dx, ax              
IDIV      cx                  
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x2e], dx       
                                    ; report.c:3252
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0001    
JNZ       L_7a0a              

L_7a04:
MOV       ax, 0x0001          
JMP       L_7a0d              

L_7a0a:
MOV       ax, 0x0000          

L_7a0d:
ADD       ax, 0x0003          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x2e], ax       
JLE       L_7a3c              

L_7a1c:                             ; report.c:3253
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x4], 0x0001    
JNZ       L_7a2f              

L_7a29:
MOV       ax, 0x0001          
JMP       L_7a32              

L_7a2f:
MOV       ax, 0x0000          

L_7a32:
ADD       ax, 0x0003          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x2e], ax       

L_7a3c:                             ; report.c:3254
MOV       ax, [bp-cSubsort]         ; ax, [bp-0x67a]
ADD       ax, 0x0002          
CMP       [bp-iRet], ax             ; [bp-0x73a], ax
JGE       L_7a52              

L_7a4c:
MOV       ax, 0x0001          
JMP       L_7a55              

L_7a52:
MOV       ax, 0x0000          

L_7a55:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0xc], ax        

L_7a5c:                             ; report.c:3257
PUSH      [bp+icol]                 ; [bp+0xa]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
CALLF     SortReportCache           ; void SortReportCache(ReportType irpt, int16_t icol)
ADD       sp, 0x0004          
                                    ; report.c:3259
JMP       L_7acb              

L_7a71:
MOV       ax, [bp-iHide]            ; ax, [bp-0x73c]
CMP       [bp-iRet], ax             ; [bp-0x73a], ax
JNZ       L_7a9b              

L_7a7e:                             ; report.c:3261
MOV       [bp-fccolChange], 0x0001  ; [bp-0x682], 0x0001
                                    ; report.c:3262
MOV       cx, [bp+icol]             ; cx, [bp+0xa]
MOV       ax, 0x0001          
SHL       ax, cx              
NOT       ax                  
CWD       dx, ax              
MOV       bx, [vprptCur]            ; bx, [0x15ac]
AND       [bx], ax            
AND       [bx+0x2], dx        
                                    ; report.c:3264
JMP       L_7acb              

L_7a9b:
MOV       ax, [bp-iBase]            ; ax, [bp-0x678]
CMP       [bp-iRet], ax             ; [bp-0x73a], ax
JL        L_7acb              

L_7aa8:                             ; report.c:3266
MOV       [bp-fccolChange], 0x0001  ; [bp-0x682], 0x0001
                                    ; report.c:3267
MOV       ax, [bp-iRet]             ; ax, [bp-0x73a]
SHL       ax, 0x0001          
LEA       bx, [bp-0x6c2]      
ADD       bx, ax              
MOV       cx, [bx]            
MOV       ax, 0x0001          
SHL       ax, cx              
CWD       dx, ax              
MOV       bx, [vprptCur]            ; bx, [0x15ac]
OR        [bx], ax            
OR        [bx+0x2], dx        

L_7acb:                             ; report.c:3270
CMP       [bp-fccolChange], 0x0000  ; [bp-0x682], 0x0000
JZ        L_7ada              

L_7ad5:                             ; report.c:3271
CALLF     SetHScrollBar             ; void SetHScrollBar()

L_7ada:                             ; report.c:3273
PUSH      [hwndReportDlg]           ; [0x15c4]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_7aef:                             ; report.c:3274
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


