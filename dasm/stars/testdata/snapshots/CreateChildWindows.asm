; CreateChildWindows  (mdi)
;   addr: 0005:038c  len=841
;   sig:  void CreateChildWindows()
;   locals:
;     char[15]         szGame         [BP-0x7c]
;     char *           psz            [BP-0x6c]
;     POINT            pt             [BP-0x6a]
;     char[100]        szData         [BP-0x66]
;
;   stats: blocks=0  labels=0

L_038c:                             ; mdi.c:218
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x007c          
PUSH      si                  
PUSH      di                  
                                    ; mdi.c:225
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_0480              

L_039f:                             ; mdi.c:227
MOV       ax, 0x56a2          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
MOV       cx, 0x56a2          
ADD       cx, ax              
MOV       ax, 0xffff          
ADD       cx, ax              
MOV       [bp-psz], cx              ; [bp-0x6c], cx

L_03b8:                             ; mdi.c:228
CMP       [bp-psz], 0x56a2          ; [bp-0x6c], 0x56a2
JBE       L_03e7              

L_03c2:
MOV       bx, [bp-psz]              ; bx, [bp-0x6c]
MOV       al, [bx-0x1]        
CBW       ax, al              
CMP       ax, 0x005c          
JZ        L_03e7              

L_03d1:
MOV       bx, [bp-psz]              ; bx, [bp-0x6c]
MOV       al, [bx-0x1]        
CBW       ax, al              
CMP       ax, 0x003a          
JZ        L_03e7              

L_03e0:                             ; mdi.c:229
SUB       [bp-psz], 0x0001          ; [bp-0x6c], 0x0001
JMP       L_03b8              

L_03e7:                             ; mdi.c:230
MOV       [bp-szGame+0x8], 0x0000   ; [bp-0x74], 0x0000
                                    ; mdi.c:231
MOV       ax, 0x0008          
PUSH      ax                  
PUSH      [bp-psz]                  ; [bp-0x6c]
LEA       ax, [bp-szGame]           ; ax, [bp-0x7c]
PUSH      ax                  
CALLF     strncpy                   ; char * strncpy(char *dest, char *src, uint16_t count)
ADD       sp, 0x0006          
                                    ; mdi.c:232
LEA       ax, [bp-szGame]           ; ax, [bp-0x7c]
PUSH      ax                  
CALLF     strlwr                    ; char * strlwr(char *s)
ADD       sp, 0x0002          
                                    ; mdi.c:233
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x036e          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szGame]           ; ax, [bp-0x7c]
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
LEA       cx, [bp-szGame]           ; cx, [bp-0x7c]
ADD       cx, ax              
MOV       dx, ss              
PUSH      dx                  
PUSH      cx                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
                                    ; mdi.c:235
LEA       ax, [bp-szGame]           ; ax, [bp-0x7c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [idPlayer]                ; [0x018c]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0090          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0373          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szData]           ; ax, [bp-0x66]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x0014          
                                    ; mdi.c:237
JMP       L_04ad              

L_0480:                             ; mdi.c:239
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x052b          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; mdi.c:240
MOV       ax, 0x0090          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x57a4          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szData]           ; ax, [bp-0x66]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000c          

L_04ad:                             ; mdi.c:242
PUSH      [hwndFrame]               ; [0x258c]
LEA       ax, [bp-szData]           ; ax, [bp-0x66]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     SetWindowText             ; void SetWindowText(HWND arg1, LPCSTR arg2)
                                    ; mdi.c:244
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_06cf              

L_04ca:                             ; mdi.c:249
CMP       [hwndScanner], 0x0000     ; [0x0190], 0x0000
JNZ       L_051a              

L_04d4:                             ; mdi.c:251
MOV       ax, 0x01e2          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x5000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0xff38          
PUSH      ax                  
MOV       ax, 0xff38          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndScanner], ax         ; [0x0190], ax
                                    ; mdi.c:252
JMP       L_0547              

L_051a:                             ; mdi.c:254
PUSH      [hwndScanner]             ; [0x0190]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; mdi.c:255
MOV       [yScanTop], 0x03e8        ; [0x2570], 0x03e8
MOV       ax, 0x03e8          
MOV       [xScanTop], ax            ; [0x2572], ax
                                    ; mdi.c:256
PUSH      [hwndScanner]             ; [0x0190]
CALLF     SetScanScrollBars         ; void SetScanScrollBars(HWND hwnd)
ADD       sp, 0x0002          

L_0547:                             ; mdi.c:260
CMP       [hwndMine], 0x0000        ; [0x0194], 0x0000
JNZ       L_0595              

L_0551:                             ; mdi.c:262
MOV       ax, 0x01ec          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x5000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
PUSH      [bp-pt]                   ; [bp-0x6a]
PUSH      [bp-pt+0x2]               ; [bp-0x68]
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndMine], ax            ; [0x0194], ax
                                    ; mdi.c:263
JMP       L_05aa              

L_0595:                             ; mdi.c:264
PUSH      [hwndMine]                ; [0x0194]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_05aa:                             ; mdi.c:268
CMP       [hwndPlanet], 0x0000      ; [0x0196], 0x0000
JNZ       L_05fa              

L_05b4:                             ; mdi.c:270
MOV       ax, 0x01f6          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x5000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndPlanet], ax          ; [0x0196], ax
                                    ; mdi.c:271
JMP       L_060f              

L_05fa:                             ; mdi.c:272
PUSH      [hwndPlanet]              ; [0x0196]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_060f:                             ; mdi.c:276
CMP       [hwndTb], 0x0000          ; [0x019a], 0x0000
JNZ       L_065f              

L_0619:                             ; mdi.c:278
MOV       ax, 0x0242          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x5000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndTb], ax              ; [0x019a], ax
                                    ; mdi.c:279
JMP       L_0674              

L_065f:                             ; mdi.c:280
PUSH      [hwndTb]                  ; [0x019a]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_0674:                             ; mdi.c:284
CMP       [hwndMessage], 0x0000     ; [0x0198], 0x0000
JZ        L_0687              

L_067e:                             ; mdi.c:285
PUSH      [hwndMessage]             ; [0x0198]
CALLF     DestroyWindow             ; int16_t DestroyWindow(HWND arg1)

L_0687:                             ; mdi.c:288
MOV       ax, 0x0202          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x5000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0xfe0c          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreateWindow              ; HWND CreateWindow(LPCSTR arg1, LPCSTR arg2, uint32_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, int16_t arg7, HWND arg8, HMENU arg9, HINSTANCE arg10, LPVOID arg11)
MOV       [hwndMessage], ax         ; [0x0198], ax
                                    ; mdi.c:290
CALLF     RefitFrameChildren        ; void RefitFrameChildren()

L_06cf:                             ; mdi.c:291
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


