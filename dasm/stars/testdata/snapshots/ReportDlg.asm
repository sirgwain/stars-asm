; ReportDlg  (report)
;   addr: 0022:0018  len=2511
;   sig:  LRESULT CALLBACK ReportDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
;   params:
;     HWND             hwnd           [BP+0xe]
;     UINT             msg            [BP+0xc]
;     WPARAM           wParam         [BP+0xa]
;     LPARAM           lParam         [BP+0x6]
;   locals:
;     RECT             rc             [BP-0xe]
;     HMENU            hmenu          [BP-0x6]
;     HDC              hdc            [BP-0x4]
;     block 0022:0027  len=0x174
;       int16_t          dx             [BP-0x12]
;       int16_t          i              [BP-0x10]
;     block 0022:019B  len=0x177
;       int16_t          dx             [BP-0x14]
;       int16_t          cRow           [BP-0x12]
;       uint16_t         swp            [BP-0x10]
;     block 0022:0362  len=0x19A
;       int16_t          xCur           [BP-0x1c]
;       int16_t          iRow           [BP-0x1a]
;       int16_t          iCol           [BP-0x18]
;       int16_t          ibit           [BP-0x16]
;       int16_t          i              [BP-0x14]
;       POINT16          pt             [BP-0x12]
;     block 0022:04FC  len=0x180
;       int16_t          iNew           [BP-0x12]
;       int16_t          iCur           [BP-0x10]
;     block 0022:067C  len=0x19C
;       int16_t          iNew           [BP-0x12]
;       int16_t          iCur           [BP-0x10]
;       block 0022:0788  len=0x87
;         int16_t          ibit           [BP-0x16]
;         int16_t          i              [BP-0x14]
;     block 0022:0818  len=0x48
;       PAINTSTRUCT      ps             [BP-0x2e]
;     block 0022:0860  len=0xE5
;       MessageId        idm            [BP-0x10]
;
;   stats: blocks=8  labels=0

L_0018:                             ; report.c:117
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x002e          
PUSH      si                  
PUSH      di                  
                                    ; report.c:122
MOV       ax, [bp+msg]              ; ax, [bp+0xc]
JMP       L_0965              

L_0027:                             ; report.c:129
MOV       ax, [bp+hwnd]             ; ax, [bp+0xe]
MOV       [hwndReportDlg], ax       ; [0x15c4], ax
                                    ; report.c:131
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     GetDC                     ; HDC GetDC(HWND arg1)
MOV       [bp-hdc], ax              ; [bp-0x4], ax
                                    ; report.c:132
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; report.c:133
MOV       [bp-i], 0x0000            ; [bp-0x10], 0x0000
JMP       L_0050              

L_004c:
ADD       [bp-i], 0x0001            ; [bp-0x10], 0x0001

L_0050:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x6]        
CMP       [bp-i], ax                ; [bp-0x10], ax
JGE       L_0097              

L_005f:                             ; report.c:135
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x57a4          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0x10]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
CALLF     DxReportColHdr            ; int16_t DxReportColHdr(ReportType irpt, int16_t iCol, char *psz, HDC hdc)
ADD       sp, 0x0008          
MOV       [bp-dx], ax               ; [bp-0x12], ax
                                    ; report.c:137
MOV       cx, 0x0002          
MOV       ax, [bp-dx]               ; ax, [bp-0x12]
CWD       dx, ax              
IDIV      cx                  
MOV       cx, 0x001a          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
ADD       bx, cx              
MOV       cx, [bp-i]                ; cx, [bp-0x10]
ADD       bx, cx              
MOV       [bx], al            
                                    ; report.c:138
JMP       L_004c              

L_0097:                             ; report.c:139
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     ReleaseDC                 ; int16_t ReleaseDC(HWND arg1, HDC arg2)
                                    ; report.c:141
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0xa]            
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
CALLF     SortReportCache           ; void SortReportCache(ReportType irpt, int16_t icol)
ADD       sp, 0x0004          
                                    ; report.c:145
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x14]           
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x16]           
MOV       ax, 0x000e          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
                                    ; report.c:147
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0010          
MOV       cx, [vprptCur]            ; cx, [0x15ac]
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     StickyDlgPos              ; void StickyDlgPos(HWND hwnd, POINT16 *ppt, int16_t fInit)
ADD       sp, 0x0006          
                                    ; report.c:151
MOV       ax, 0x15cc          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
MOV       dx, 0x4000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x30], ax       
                                    ; report.c:155
MOV       ax, 0x15d6          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x4000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x32], ax       
                                    ; report.c:156
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_019b              

L_0196:                             ; report.c:157
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_019b:                             ; report.c:168
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; report.c:170
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x8]
ADD       ax, 0xffdc          
MOV       cx, [dyArial8]            ; cx, [0x23fa]
ADD       cx, 0x0004          
CWD       dx, ax              
IDIV      cx                  
MOV       [bp-cRow], ax             ; [bp-0x12], ax
                                    ; report.c:171
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
CMP       [bp-cRow], ax             ; [bp-0x12], ax
JGE       L_01d2              

L_01cc:
MOV       ax, [bp-cRow]             ; ax, [bp-0x12]
JMP       L_01d9              

L_01d2:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       

L_01d9:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x2c], ax       
                                    ; report.c:173
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0x2c], ax       
JL        L_021c              

L_01f3:                             ; report.c:175
MOV       [bp-swp], 0x0084          ; [bp-0x10], 0x0084
                                    ; report.c:176
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0xe], 0x0000    
                                    ; report.c:177
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x30]           
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     SetScrollPos              ; int16_t SetScrollPos(HWND arg1, int16_t arg2, int16_t arg3, int16_t arg4)
                                    ; report.c:179
JMP       L_02b5              

L_021c:                             ; report.c:181
MOV       [bp-swp], 0x0044          ; [bp-0x10], 0x0044
                                    ; report.c:184
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0xe]        
MOV       bx, [vprptCur]            ; bx, [0x15ac]
ADD       ax, [bx+0x2c]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       ax, [bx+0x2a]       
JLE       L_0273              

L_023b:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0xe], 0x0000    
JLE       L_0273              

L_0248:                             ; report.c:186
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
SUB       ax, [bx+0x2c]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0xe], ax        
                                    ; report.c:187
MOV       bx, [vprptCur]            ; bx, [0x15ac]
CMP       [bx+0xe], 0x0000    
JGE       L_0273              

L_026a:                             ; report.c:188
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0xe], 0x0000    

L_0273:                             ; report.c:190
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x30]           
MOV       ax, 0x0002          
PUSH      ax                  
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0xe]            
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     SetScrollPos              ; int16_t SetScrollPos(HWND arg1, int16_t arg2, int16_t arg3, int16_t arg4)
                                    ; report.c:191
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x30]           
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
SUB       ax, [bx+0x2c]       
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetScrollRange            ; void SetScrollRange(HWND arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)

L_02b5:                             ; report.c:194
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
MOV       [bp-dx], ax               ; [bp-0x14], ax
                                    ; report.c:196
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x30]           
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0xa]
SUB       ax, [bp-dx]               ; ax, [bp-0x14]
PUSH      ax                  
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0006          
PUSH      ax                  
PUSH      [bp-dx]                   ; [bp-0x14]
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0004          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
IMUL      [bx+0x2c]           
ADD       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-swp]                  ; [bp-0x10]
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
                                    ; report.c:199
CALLF     SetHScrollBar             ; void SetHScrollBar()
                                    ; report.c:200
CMP       [bp+msg], 0x0001          ; [bp+0xc], 0x0001
JNZ       L_030b              

L_0304:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_09df              

L_030b:
MOV       ax, 0x0000          
CWD       dx, ax              

L_030f:
JMP       L_09df              

L_0312:                             ; report.c:204
MOV       bx, [bp+lParam]           ; bx, [bp+0x6]
MOV       cx, [bp+lParam+0x2]       ; cx, [bp+0x8]
MOV       es, cx              
MOV       es:[bx+0xc], 0x012c 
                                    ; report.c:205
MOV       bx, [bp+lParam]           ; bx, [bp+0x6]
MOV       cx, [bp+lParam+0x2]       ; cx, [bp+0x8]
MOV       es, cx              
MOV       es:[bx+0xe], 0x00dc 
                                    ; report.c:206
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_09df              

L_0337:                             ; report.c:209
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; report.c:210
PUSH      [bp+wParam]               ; [bp+0xa]
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hbrButtonFace]           ; [0x0010]
CALLF     FillRect                  ; int16_t FillRect(HDC arg1, RECT *arg2, HBRUSH arg3)
                                    ; report.c:211
MOV       ax, 0x0001          
MOV       dx, 0x0000          
JMP       L_09df              

L_0362:                             ; report.c:222
MOV       [bp-xCur], 0x0002         ; [bp-0x1c], 0x0002
                                    ; report.c:224
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-pt], ax               ; [bp-0x12], ax
                                    ; report.c:225
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
MOV       [bp-pt+0x2], ax           ; [bp-0x10], ax
                                    ; report.c:227
CMP       [bp-pt+0x2], 0x0002       ; [bp-0x10], 0x0002
JL        L_09c8              

L_0390:
CMP       [bp-pt], 0x0002           ; [bp-0x12], 0x0002
JL        L_09c8              

L_039c:                             ; report.c:230
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0006          
CMP       [bp-pt+0x2], ax           ; [bp-0x10], ax
JGE       L_03b2              

L_03aa:                             ; report.c:231
MOV       [bp-iRow], 0xffff         ; [bp-0x1a], 0xffff
                                    ; report.c:232
JMP       L_03f1              

L_03b2:                             ; report.c:234
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0004          
MOV       cx, [bp-pt+0x2]           ; cx, [bp-0x10]
ADD       cx, 0xfffe          
MOV       [bp-0x1e], ax       
MOV       ax, cx              
MOV       cx, [bp-0x1e]       
SUB       ax, cx              
MOV       cx, [dyArial8]            ; cx, [0x23fa]
ADD       cx, 0x0004          
CWD       dx, ax              
IDIV      cx                  
MOV       [bp-iRow], ax             ; [bp-0x1a], ax
                                    ; report.c:235
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2c]       
CMP       [bp-iRow], ax             ; [bp-0x1a], ax
JGE       L_09c8              

L_03e7:                             ; report.c:238
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0xe]        
ADD       [bp-iRow], ax             ; [bp-0x1a], ax

L_03f1:                             ; report.c:241
MOV       [bp-iCol], 0xffff         ; [bp-0x18], 0xffff
                                    ; report.c:242
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
MOV       [bp-ibit], 0x0001         ; [bp-0x16], 0x0001
MOV       ax, 0x0001          
JMP       L_0416              

L_0406:
MOV       ax, [bp-0x14]       
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001
MOV       cx, 0x0001          
SHL       [bp-ibit], cx             ; [bp-0x16], cx
MOV       ax, [bp-0x16]       

L_0416:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x6]        
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_0489              

L_0425:                             ; report.c:245
MOV       ax, [bp-ibit]             ; ax, [bp-0x16]
CWD       dx, ax              
MOV       bx, [vprptCur]            ; bx, [0x15ac]
AND       ax, [bx]            
AND       dx, [bx+0x2]        
CMP       ax, 0x0000          
JNZ       L_0442              

L_043a:
CMP       dx, 0x0000          
JZ        L_0406              

L_0442:
CMP       [bp-i], 0x0000            ; [bp-0x14], 0x0000
JZ        L_045a              

L_044b:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x8]        
CMP       [bp-i], ax                ; [bp-0x14], ax
JL        L_0406              

L_045a:                             ; report.c:247
MOV       ax, 0x001a          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x14]
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
SHL       ax, 0x0001          
ADD       [bp-xCur], ax             ; [bp-0x1c], ax
                                    ; report.c:248
MOV       ax, [bp-pt]               ; ax, [bp-0x12]
CMP       [bp-xCur], ax             ; [bp-0x1c], ax
JLE       L_0406              

L_047d:                             ; report.c:250
MOV       ax, [bp-i]                ; ax, [bp-0x14]
MOV       [bp-iCol], ax             ; [bp-0x18], ax

L_0489:                             ; report.c:256
CMP       [bp-iCol], 0xffff         ; [bp-0x18], 0xffff
JZ        L_09c8              

L_0495:                             ; report.c:259
CMP       [bp-iRow], 0xffff         ; [bp-0x1a], 0xffff
JNZ       L_04c6              

L_049e:                             ; report.c:260
CMP       [bp+msg], 0x0204          ; [bp+0xc], 0x0204
JNZ       L_04ae              

L_04a8:
MOV       ax, 0x0001          
JMP       L_04b1              

L_04ae:
MOV       ax, 0x0000          

L_04b1:
PUSH      ax                  
PUSH      [bp-iCol]                 ; [bp-0x18]
PUSH      [bp-pt+0x2]               ; [bp-0x10]
PUSH      [bp-pt]                   ; [bp-0x12]
CALLF     ReportColumnPopup         ; void ReportColumnPopup(POINT16 pt, int16_t icol, int16_t fRightBtn)
ADD       sp, 0x0008          
                                    ; report.c:261
JMP       L_04e1              

L_04c6:                             ; report.c:262
PUSH      [bp-iRow]                 ; [bp-0x1a]
PUSH      [bp-iCol]                 ; [bp-0x18]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
PUSH      [bx+0x4]            
PUSH      [bp-pt+0x2]               ; [bp-0x10]
PUSH      [bp-pt]                   ; [bp-0x12]
CALLF     ExecuteReportClick        ; void ExecuteReportClick(POINT16 pt, ReportType irpt, int16_t icol, int16_t irow)
ADD       sp, 0x000a          

L_04e1:                             ; report.c:264
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_09c8              

L_04f4:                             ; report.c:265
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_04f9:                             ; report.c:266
JMP       L_09c8              

L_04fc:                             ; report.c:271
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     GetScrollPos              ; int16_t GetScrollPos(HWND arg1, int16_t arg2)
MOV       [bp-iCur], ax             ; [bp-0x10], ax
                                    ; report.c:272
MOV       ax, [bp-iCur]             ; ax, [bp-0x10]
MOV       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:274
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
JMP       L_0576              

L_0529:                             ; report.c:277
MOV       [bp-iNew], 0x07d0         ; [bp-0x12], 0x07d0
                                    ; report.c:278
JMP       L_0597              

L_0531:                             ; report.c:280
ADD       [bp-iNew], 0x0001         ; [bp-0x12], 0x0001
                                    ; report.c:281
JMP       L_0597              

L_0538:                             ; report.c:283
SUB       [bp-iNew], 0x0001         ; [bp-0x12], 0x0001
                                    ; report.c:284
JMP       L_0597              

L_053f:                             ; report.c:286
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2c]       
ADD       ax, 0xffff          
ADD       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:287
JMP       L_0597              

L_054f:                             ; report.c:289
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2c]       
ADD       ax, 0xffff          
SUB       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:290
JMP       L_0597              

L_055f:                             ; report.c:293
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:294
JMP       L_0597              

L_056b:                             ; report.c:296
MOV       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000
                                    ; report.c:297
JMP       L_0597              

L_0576:
CMP       ax, 0x0007          
JA        L_0597              

L_057e:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x587]       

L_0597:                             ; report.c:299
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
SUB       ax, [bx+0x2c]       
CMP       [bp-iNew], ax             ; [bp-0x12], ax
JLE       L_05be              

L_05ad:                             ; report.c:300
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x2a]       
MOV       bx, [vprptCur]            ; bx, [0x15ac]
SUB       ax, [bx+0x2c]       
MOV       [bp-iNew], ax             ; [bp-0x12], ax

L_05be:                             ; report.c:301
CMP       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000
JGE       L_05cc              

L_05c7:                             ; report.c:302
MOV       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000

L_05cc:                             ; report.c:303
MOV       ax, [bp-iCur]             ; ax, [bp-0x10]
CMP       [bp-iNew], ax             ; [bp-0x12], ax
JZ        L_0673              

L_05d7:                             ; report.c:305
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bp-iNew]             ; ax, [bp-0x12]
MOV       [bx+0xe], ax        
                                    ; report.c:307
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; report.c:308
MOV       [bp-rc], 0x0002           ; [bp-0xe], 0x0002
                                    ; report.c:309
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(int16_t arg1)
SUB       [bp-rc+0x4], ax           ; [bp-0xa], ax
                                    ; report.c:310
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0006          
MOV       [bp-rc+0x2], ax           ; [bp-0xc], ax
                                    ; report.c:311
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0004          
MOV       bx, [vprptCur]            ; bx, [0x15ac]
IMUL      [bx+0x2c]           
ADD       ax, [bp-rc+0x2]           ; ax, [bp-0xc]
MOV       [bp-rc+0x6], ax           ; [bp-0x8], ax
                                    ; report.c:313
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [dyArial8]            ; ax, [0x23fa]
ADD       ax, 0x0004          
MOV       cx, [bp-iCur]             ; cx, [bp-0x10]
SUB       cx, [bp-iNew]             ; cx, [bp-0x12]
IMUL      cx                  
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScrollWindow              ; void ScrollWindow(HWND arg1, int16_t arg2, int16_t arg3, RECT *arg4, RECT *arg5)
                                    ; report.c:314
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
PUSH      [bp-iNew]                 ; [bp-0x12]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetScrollPos              ; int16_t SetScrollPos(HWND arg1, int16_t arg2, int16_t arg3, int16_t arg4)
                                    ; report.c:315
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     UpdateWindow              ; void UpdateWindow(HWND arg1)

L_0673:                             ; report.c:318
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_09df              

L_067c:                             ; report.c:323
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     GetScrollPos              ; int16_t GetScrollPos(HWND arg1, int16_t arg2)
MOV       [bp-iCur], ax             ; [bp-0x10], ax
                                    ; report.c:324
MOV       ax, [bp-iCur]             ; ax, [bp-0x10]
MOV       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:326
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
JMP       L_06e4              

L_06a9:                             ; report.c:329
MOV       [bp-iNew], 0x07d0         ; [bp-0x12], 0x07d0
                                    ; report.c:330
JMP       L_0705              

L_06b1:                             ; report.c:332
ADD       [bp-iNew], 0x0001         ; [bp-0x12], 0x0001
                                    ; report.c:333
JMP       L_0705              

L_06b8:                             ; report.c:335
SUB       [bp-iNew], 0x0001         ; [bp-0x12], 0x0001
                                    ; report.c:336
JMP       L_0705              

L_06bf:                             ; report.c:338
ADD       [bp-iNew], 0x0003         ; [bp-0x12], 0x0003
                                    ; report.c:339
JMP       L_0705              

L_06c6:                             ; report.c:341
SUB       [bp-iNew], 0x0003         ; [bp-0x12], 0x0003
                                    ; report.c:342
JMP       L_0705              

L_06cd:                             ; report.c:345
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:346
JMP       L_0705              

L_06d9:                             ; report.c:348
MOV       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000
                                    ; report.c:349
JMP       L_0705              

L_06e4:
CMP       ax, 0x0007          
JA        L_0705              

L_06ec:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x6f5]       

L_0705:                             ; report.c:351
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x34]       
CMP       [bp-iNew], ax             ; [bp-0x12], ax
JLE       L_071e              

L_0714:                             ; report.c:352
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x34]       
MOV       [bp-iNew], ax             ; [bp-0x12], ax

L_071e:                             ; report.c:353
CMP       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000
JGE       L_072c              

L_0727:                             ; report.c:354
MOV       [bp-iNew], 0x0000         ; [bp-0x12], 0x0000

L_072c:                             ; report.c:355
MOV       ax, [bp-iCur]             ; ax, [bp-0x10]
CMP       [bp-iNew], ax             ; [bp-0x12], ax
JZ        L_080f              

L_0737:                             ; report.c:357
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
PUSH      [bp-iNew]                 ; [bp-0x12]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetScrollPos              ; int16_t SetScrollPos(HWND arg1, int16_t arg2, int16_t arg3, int16_t arg4)
                                    ; report.c:358
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     GetScrollPos              ; int16_t GetScrollPos(HWND arg1, int16_t arg2)
MOV       [bp-iNew], ax             ; [bp-0x12], ax
                                    ; report.c:360
MOV       ax, [bp-iCur]             ; ax, [bp-0x10]
CMP       [bp-iNew], ax             ; [bp-0x12], ax
JZ        L_080f              

L_0788:                             ; report.c:365
MOV       [bp-i], 0x0001            ; [bp-0x14], 0x0001
MOV       [bp-ibit], 0x0002         ; [bp-0x16], 0x0002
MOV       ax, 0x0002          
JMP       L_07a8              

L_0798:
MOV       ax, [bp-0x14]       
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001
MOV       cx, 0x0001          
SHL       [bp-ibit], cx             ; [bp-0x16], cx
MOV       ax, [bp-0x16]       

L_07a8:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x6]        
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_07e9              

L_07b7:                             ; report.c:366
MOV       ax, [bp-ibit]             ; ax, [bp-0x16]
CWD       dx, ax              
MOV       bx, [vprptCur]            ; bx, [0x15ac]
AND       ax, [bx]            
AND       dx, [bx+0x2]        
CMP       ax, 0x0000          
JNZ       L_07d4              

L_07cc:
CMP       dx, 0x0000          
JZ        L_0798              

L_07d4:
MOV       ax, [bp-iNew]             ; ax, [bp-0x12]
SUB       [bp-iNew], 0x0001         ; [bp-0x12], 0x0001
CMP       ax, 0x0000          
JG        L_0798              

L_07e9:
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bp-i]                ; ax, [bp-0x14]
MOV       [bx+0x8], ax        
                                    ; report.c:371
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; report.c:372
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     UpdateWindow              ; void UpdateWindow(HWND arg1)

L_080f:                             ; report.c:376
MOV       ax, 0x0000          
MOV       dx, 0x0000          
JMP       L_09df              

L_0818:                             ; report.c:383
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-ps]               ; ax, [bp-0x2e]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     BeginPaint                ; HDC BeginPaint(HWND arg1, PAINTSTRUCT *arg2)
MOV       [bp-hdc], ax              ; [bp-0x4], ax
                                    ; report.c:384
LEA       ax, [bp-ps+0x4]           ; ax, [bp-0x2a]
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     DrawReport                ; void DrawReport(HWND hwnd, HDC hdc, RECT *prc)
ADD       sp, 0x0006          
                                    ; report.c:385
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-ps]               ; ax, [bp-0x2e]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     EndPaint                  ; void EndPaint(HWND arg1, PAINTSTRUCT *arg2)
                                    ; report.c:386
MOV       ax, [gd+0x2]              ; ax, [0x07cc]
AND       ax, 0xfdff          
OR        ax, 0x0000          
MOV       [gd+0x2], ax              ; [0x07cc], ax
                                    ; report.c:387
MOV       ax, 0x0001          
MOV       dx, 0x0000          
JMP       L_09df              

L_0860:                             ; report.c:394
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0010          
MOV       cx, [vprptCur]            ; cx, [0x15ac]
ADD       cx, ax              
PUSH      cx                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     StickyDlgPos              ; void StickyDlgPos(HWND hwnd, POINT16 *ppt, int16_t fInit)
ADD       sp, 0x0006          
                                    ; report.c:396
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetWindowRect             ; void GetWindowRect(HWND arg1, RECT *arg2)
                                    ; report.c:397
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0xa]
SUB       ax, [bp-rc]               ; ax, [bp-0xe]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x14], ax       
                                    ; report.c:398
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x8]
SUB       ax, [bp-rc+0x2]           ; ax, [bp-0xc]
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       [bx+0x16], ax       
                                    ; report.c:400
MOV       [hwndReportDlg], 0x0000   ; [0x15c4], 0x0000
                                    ; report.c:401
MOV       [fBrowserValid], 0x0000   ; [0x0d22], 0x0000
                                    ; report.c:402
MOV       ax, 0x0004          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
CALLF     GetASubMenu               ; HMENU GetASubMenu(HWND hwnd, MainMenu iMenu)
ADD       sp, 0x0004          
MOV       [bp-hmenu], ax            ; [bp-0x6], ax
                                    ; report.c:404
MOV       bx, [vprptCur]            ; bx, [0x15ac]
MOV       ax, [bx+0x4]        
JMP       L_08f1              

L_08ce:                             ; report.c:410
MOV       [bp-idm], 0x08ff          ; [bp-0x10], 0x08ff
                                    ; report.c:411
JMP       L_0914              

L_08d6:                             ; report.c:413
MOV       [bp-idm], 0x0900          ; [bp-0x10], 0x0900
                                    ; report.c:414
JMP       L_0914              

L_08de:                             ; report.c:416
MOV       [bp-idm], 0x08fd          ; [bp-0x10], 0x08fd
                                    ; report.c:417
JMP       L_0914              

L_08e6:                             ; report.c:419
MOV       [bp-idm], 0x0901          ; [bp-0x10], 0x0901
                                    ; report.c:420
JMP       L_0914              

L_08f1:
CMP       ax, 0x0000          
JZ        L_08de              

L_08f9:
CMP       ax, 0x0001          
JZ        L_08ce              

L_0901:
CMP       ax, 0x0002          
JZ        L_08d6              

L_0909:
CMP       ax, 0x0003          
JZ        L_08e6              

L_0914:                             ; report.c:423
PUSH      [bp-hmenu]                ; [bp-0x6]
MOV       ax, [bp-idm]              ; ax, [bp-0x10]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     CheckMenuItem             ; int16_t CheckMenuItem(HMENU arg1, ControlId arg2, uint16_t arg3)
                                    ; report.c:424
MOV       [vprptCur], 0x0000        ; [0x15ac], 0x0000
                                    ; report.c:425
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_09c8              

L_093d:                             ; report.c:426
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_0942:                             ; report.c:428
JMP       L_09c8              

L_0945:                             ; report.c:432
CMP       [bp+wParam], 0x0002       ; [bp+0xa], 0x0002
JNZ       L_09c8              

L_094e:                             ; report.c:434
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     DestroyWindow             ; int16_t DestroyWindow(HWND arg1)
                                    ; report.c:435
MOV       ax, 0x0001          
MOV       dx, 0x0000          
JMP       L_09df              

L_0965:
CMP       ax, 0x0001          
JZ        L_0027              

L_096d:
CMP       ax, 0x0002          
JZ        L_0860              

L_0975:
CMP       ax, 0x0005          
JZ        L_019b              

L_097d:
CMP       ax, 0x000f          
JZ        L_0818              

L_0985:
CMP       ax, 0x0014          
JZ        L_0337              

L_098d:
CMP       ax, 0x0024          
JZ        L_0312              

L_0995:
CMP       ax, 0x0111          
JZ        L_0945              

L_099d:
CMP       ax, 0x0114          
JZ        L_067c              

L_09a5:
CMP       ax, 0x0115          
JZ        L_04fc              

L_09ad:
CMP       ax, 0x0201          
JZ        L_0362              

L_09b5:
CMP       ax, 0x0203          
JZ        L_0362              

L_09bd:
CMP       ax, 0x0204          
JZ        L_0362              

L_09c8:                             ; report.c:439
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp+msg]                  ; [bp+0xc]
PUSH      [bp+wParam]               ; [bp+0xa]
PUSH      [bp+lParam+0x2]           ; [bp+0x8]
PUSH      [bp+lParam]               ; [bp+0x6]
CALLF     DefWindowProc             ; LRESULT DefWindowProc(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)

L_09df:                             ; report.c:440
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF      0x000a              


