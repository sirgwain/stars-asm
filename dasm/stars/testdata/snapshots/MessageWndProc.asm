; MessageWndProc  (msg)
;   addr: 0007:5c92  len=5510
;   sig:  LRESULT CALLBACK MessageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
;   params:
;     HWND             hwnd           [BP+0xe]
;     UINT             message        [BP+0xc]
;     WPARAM           wParam         [BP+0xa]
;     LPARAM           lParam         [BP+0x6]
;   locals:
;     PAINTSTRUCT      ps             [BP-0x28]
;     char *           psz            [BP-0x8]
;     int16_t          i              [BP-0x6]
;     HDC              hdc            [BP-0x4]
;     block 0007:5EF1  len=0x165
;       RECT             rc             [BP-0x34]
;       int16_t          dx             [BP-0x2c]
;       int16_t          dy             [BP-0x2a]
;     block 0007:6059  len=0x2B
;       RECT             rc             [BP-0x30]
;     block 0007:6084  len=0x4B
;       HCURSOR          hcs            [BP-0x2e]
;       POINT16          pt             [BP-0x2c]
;     block 0007:60CF  len=0x30E
;       HtMsgType        ht             [BP-0x2e]
;       POINT16          pt             [BP-0x2c]
;       block 0007:6109  len=0xB7
;         MessageId        idm            [BP-0x32]
;         int16_t          fSet           [BP-0x30]
;       block 0007:62C6  len=0xAB
;         MSGPLR *         lpmpSrc        [BP-0x36]
;         MSGPLR *         lpmp           [BP-0x32]
;     block 0007:643A  len=0x63C
;       RECT             rc             [BP-0x72]
;       char[32]         szT            [BP-0x6a]
;       int16_t          iMode          [BP-0x4a]
;       int16_t          cch            [BP-0x48]
;       int16_t          dx             [BP-0x46]
;       RECT             rcActual       [BP-0x44]
;       int16_t          idm            [BP-0x3c]
;       COLORREF         crBack         [BP-0x3a]
;       int16_t          dy             [BP-0x36]
;       int16_t          dxMax          [BP-0x34]
;       COLORREF         crFore         [BP-0x32]
;       HBRUSH           hbrSav         [BP-0x2e]
;       char *           lpsz           [BP-0x2c]
;       block 0007:65EC  len=0x1A7
;         MSGPLR *         lpmsgplr       [BP-0x76]
;     block 0007:6AFF  len=0x4E
;       int16_t          i              [BP-0x2a]
;     block 0007:6DB8  len=0xB6
;       int16_t          idm            [BP-0x2a]
;     block 0007:6EA6  len=0x5F
;       THING *          lpth           [BP-0x2c]
;       block 0007:6ECA  len=0x38
;         SCAN             scan           [BP-0x3c]
;     block 0007:6FEE  len=0xC2
;       int16_t          fRet           [BP-0x2e]
;       FARPROC          lpProc         [BP-0x2c]
;       block 0007:704C  len=0x64
;         int32_t          lSerial        [BP-0x32]
;     block 0007:70FD  len=0x15
;
;   stats: blocks=15  labels=8
;     SetupNewMsg: L_6c2a
;     Default: L_718a
;     CheckBox: L_6109
;     ZoomBox: L_61cc
;     NextMsg: L_6ca9
;     PrevMsg: L_6ba0
;     ToggleMsgMode: L_6298
;     GotoMsg: L_6d8d

L_5c92:                             ; msg.c:53
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0076          
PUSH      si                  
PUSH      di                  
                                    ; msg.c:59
MOV       ax, [bp+message]          ; ax, [bp+0xc]
JMP       L_71a4              

L_5ca1:                             ; msg.c:62
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_5d3c              

L_5ca9:                             ; msg.c:64
MOV       ax, [bp+hwnd]             ; ax, [bp+0xe]
MOV       [hwndMessage], ax         ; [0x0198], ax
                                    ; msg.c:70
MOV       ax, 0x0b14          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0x6]
ADD       ax, 0x054c          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x4000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JNZ       L_5ce8              

L_5ce2:
MOV       ax, 0x0032          
JMP       L_5ceb              

L_5ce8:
MOV       ax, 0x002c          

L_5ceb:
PUSH      ax                  
MOV       ax, 0x0003          
IMUL      [dyArial8]                ; [0x23fa]
SAR       ax, 0x0001          
ADD       ax, 0xffff          
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
MOV       bx, [bp-i]                ; bx, [bp-0x6]
SHL       bx, 0x0001          
MOV       [bx+0x4b84], ax     
                                    ; msg.c:71
MOV       bx, [bp-i]                ; bx, [bp-0x6]
SHL       bx, 0x0001          
PUSH      [bx+0x4b84]         
MOV       ax, 0x0030          
PUSH      ax                  
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:72
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_5d3c:
CMP       [bp-i], 0x0004            ; [bp-0x6], 0x0004
JL        L_5ca9              

L_5d45:                             ; msg.c:75
MOV       ax, 0x0b21          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0b1b          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0003          
MOV       dx, 0x4020          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x00c8          
PUSH      ax                  
MOV       ax, 0x0050          
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
MOV       [hwndMsgDrop], ax         ; [0x51fc], ax
                                    ; msg.c:76
PUSH      [hwndMsgDrop]             ; [0x51fc]
MOV       ax, 0x0030          
PUSH      ax                  
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:80
MOV       ax, 0x0b2a          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0044          
MOV       dx, 0x4080          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x00c8          
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
MOV       [hwndMsgEdit], ax         ; [0x5924], ax
                                    ; msg.c:81
PUSH      [hwndMsgEdit]             ; [0x5924]
MOV       ax, 0x0415          
PUSH      ax                  
MOV       ax, 0x03c8          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:82
PUSH      [hwndMsgEdit]             ; [0x5924]
MOV       ax, 0x0030          
PUSH      ax                  
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:87
MOV       ax, 0x0b2f          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0844          
MOV       dx, 0x40a0          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x0064          
PUSH      ax                  
MOV       ax, 0x00c8          
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
MOV       [hwndMsgScroll], ax       ; [0x531a], ax
                                    ; msg.c:90
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:93
PUSH      [hwndMsgDrop]             ; [0x51fc]
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0549          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:94
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_5e8d              

L_5e89:
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_5e8d:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-i], ax                ; [bp-0x6], ax
JGE       L_5ed5              

L_5e98:                             ; msg.c:96
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0x6]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-psz], ax              ; [bp-0x8], ax
                                    ; msg.c:97
PUSH      [hwndMsgDrop]             ; [0x51fc]
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-psz]              ; ax, [bp-0x8]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:98
JMP       L_5e89              

L_5ed5:                             ; msg.c:99
PUSH      [hwndMsgDrop]             ; [0x51fc]
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:100
JMP       L_7207              

L_5ef1:                             ; msg.c:107
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-dx], ax               ; [bp-0x2c], ax
                                    ; msg.c:108
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
MOV       [bp-dy], ax               ; [bp-0x2a], ax
                                    ; msg.c:110
MOV       [bp-i], 0x0000            ; [bp-0x6], 0x0000
JMP       L_5f62              

L_5f19:                             ; msg.c:113
MOV       bx, [bp-i]                ; bx, [bp-0x6]
SHL       bx, 0x0001          
PUSH      [bx+0x4b84]         
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-dx]               ; ax, [bp-0x2c]
ADD       ax, 0xffd0          
PUSH      ax                  
MOV       ax, [dyArial8]            ; ax, [0x23fa]
SHL       ax, 0x0001          
MOV       [bp-0x36], ax       
MOV       ax, 0x0003          
IMUL      [dyArial8]                ; [0x23fa]
SAR       ax, 0x0001          
ADD       ax, 0x0002          
IMUL      [bp-i]                    ; [bp-0x6]
ADD       ax, 0x0003          
MOV       cx, [bp-0x36]       
ADD       ax, cx              
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0055          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_5f62:
CMP       [bp-i], 0x0003            ; [bp-0x6], 0x0003
JL        L_5f19              

L_5f6b:                             ; msg.c:114
MOV       ax, 0x22b2          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, [dyArial8]            ; ax, [0x23fa]
SHL       ax, 0x0001          
ADD       ax, 0x0003          
PUSH      ax                  
MOV       ax, [bp-dx]               ; ax, [bp-0x2c]
ADD       ax, 0xffcc          
PUSH      ax                  
MOV       ax, [bp-dy]               ; ax, [bp-0x2a]
ADD       ax, 0xfffc          
PUSH      ax                  
CALLF     SetRect                   ; void SetRect(RECT *arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)
                                    ; msg.c:115
MOV       ax, 0x494a          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, [bp-dx]               ; ax, [bp-0x2c]
ADD       ax, 0xfffc          
PUSH      ax                  
MOV       ax, [dyArial8]            ; ax, [0x23fa]
SHL       ax, 0x0001          
ADD       ax, 0xfffc          
PUSH      ax                  
CALLF     SetRect                   ; void SetRect(RECT *arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)
                                    ; msg.c:117
MOV       si, 0x22b2          
LEA       di, [bp-rc]               ; di, [bp-0x34]
PUSH      ss                  
POP       es                  
MOVSW     es:[di], [rcMsgText]      ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x2]  ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x4]  ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x6]  ; es:[di], ds:[si]
LEA       ax, [bp-0x34]       
                                    ; msg.c:118
MOV       ax, 0xfffc          
PUSH      ax                  
MOV       ax, 0xfffc          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x34]
PUSH      ax                  
CALLF     ExpandRc                  ; void ExpandRc(RECT *prc, int16_t dx, int16_t dy)
ADD       sp, 0x0006          
                                    ; msg.c:121
PUSH      [hwndMsgDrop]             ; [0x51fc]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-rc]               ; ax, [bp-0x34]
ADD       ax, 0x001e          
PUSH      ax                  
PUSH      [bp-rc+0x2]               ; [bp-0x32]
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x30]
SUB       ax, [bp-rc]               ; ax, [bp-0x34]
ADD       ax, 0xffac          
PUSH      ax                  
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x2e]
SUB       ax, [bp-rc+0x2]           ; ax, [bp-0x32]
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
                                    ; msg.c:123
PUSH      [rghwndMsgBtn+0x6]        ; [0x4b8a]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x30]
ADD       ax, 0xffce          
PUSH      ax                  
PUSH      [bp-rc+0x2]               ; [bp-0x32]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0005          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
                                    ; msg.c:124
MOV       ax, [dyShipDD]            ; ax, [0x2406]
ADD       ax, 0x0003          
ADD       [bp-rc+0x2], ax           ; [bp-0x32], ax
                                    ; msg.c:127
PUSH      [hwndMsgEdit]             ; [0x5924]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-rc]                   ; [bp-0x34]
PUSH      [bp-rc+0x2]               ; [bp-0x32]
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x30]
SUB       ax, [bp-rc]               ; ax, [bp-0x34]
PUSH      ax                  
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x2e]
SUB       ax, [bp-rc+0x2]           ; ax, [bp-0x32]
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)
                                    ; msg.c:129
JMP       Default             

L_6059:                             ; msg.c:135
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0x30]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; msg.c:136
PUSH      [bp+wParam]               ; [bp+0xa]
LEA       ax, [bp-rc]               ; ax, [bp-0x30]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hbrButtonFace]           ; [0x0010]
CALLF     FillRect                  ; int16_t FillRect(HDC arg1, RECT *arg2, HBRUSH arg3)
                                    ; msg.c:137
MOV       ax, 0x0001          
MOV       dx, 0x0000          
JMP       L_7210              

L_6084:                             ; msg.c:143
MOV       [bp-hcs], 0x0000          ; [bp-0x2e], 0x0000
                                    ; msg.c:145
LEA       ax, [bp-pt]               ; ax, [bp-0x2c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetCursorPos              ; void GetCursorPos(POINT *arg1)
                                    ; msg.c:146
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-pt]               ; ax, [bp-0x2c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ScreenToClient            ; void ScreenToClient(HWND arg1, POINT *arg2)
                                    ; msg.c:147
PUSH      [bp-pt+0x2]               ; [bp-0x2a]
PUSH      [bp-pt]                   ; [bp-0x2c]
CALLF     HtMsgBox                  ; HtMsgType HtMsgBox(POINT16 pt)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        Default             

L_60ba:                             ; msg.c:149
PUSH      [hcurHand]                ; [0x525e]
CALLF     SetCursor                 ; HCURSOR SetCursor(HCURSOR arg1)
                                    ; msg.c:150
MOV       ax, 0x0001          
MOV       dx, 0x0000          
JMP       L_7210              

L_60cf:                             ; msg.c:161
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-pt], ax               ; [bp-0x2c], ax
                                    ; msg.c:162
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
MOV       [bp-pt+0x2], ax           ; [bp-0x2a], ax
                                    ; msg.c:163
PUSH      [bp-pt+0x2]               ; [bp-0x2a]
PUSH      [bp-pt]                   ; [bp-0x2c]
CALLF     HtMsgBox                  ; HtMsgType HtMsgBox(POINT16 pt)
ADD       sp, 0x0004          
MOV       [bp-ht], ax               ; [bp-0x2e], ax
                                    ; msg.c:164
CMP       [bp-ht], 0x0001           ; [bp-0x2e], 0x0001
JNZ       L_61c3              

CheckBox:                           ; msg.c:169
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JL        L_7207              

L_6116:                             ; msg.c:172
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       [bp-idm], ax              ; [bp-0x32], ax
                                    ; msg.c:174
MOV       cx, [bp-idm]              ; cx, [bp-0x32]
AND       cx, 0x0007          
MOV       ax, 0x0001          
SHL       ax, cx              
MOV       bx, [bp-idm]              ; bx, [bp-0x32]
SAR       bx, 0x0001          
SAR       bx, 0x0001          
SAR       bx, 0x0001          
MOV       cl, [bx+0x5324]     
MOV       [bp-0x34], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
MOV       cx, [bp-0x34]       
AND       ax, cx              
CMP       ax, 0x0000          
JZ        L_6158              

L_6152:
MOV       ax, 0x0001          
JMP       L_615b              

L_6158:
MOV       ax, 0x0000          

L_615b:
MOV       [bp-fSet], ax             ; [bp-0x30], ax
                                    ; msg.c:175
CMP       [bp-fSet], 0x0000         ; [bp-0x30], 0x0000
JNZ       L_616d              

L_6167:
MOV       ax, 0x0001          
JMP       L_6170              

L_616d:
MOV       ax, 0x0000          

L_6170:
PUSH      ax                  
PUSH      [bp-idm]                  ; [bp-0x32]
CALLF     SetFilteringGroups        ; void SetFilteringGroups(MessageId idm, int16_t fSet)
ADD       sp, 0x0004          
                                    ; msg.c:177
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     DirtyGame                 ; void DirtyGame(int16_t fDirty)
ADD       sp, 0x0002          
                                    ; msg.c:178
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_61a0              

L_619b:                             ; msg.c:179
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_61a0:                             ; msg.c:180
PUSH      [hwndMessage]             ; [0x0198]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; msg.c:182
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:184
JMP       L_7207              

L_61c3:
CMP       [bp-ht], 0x0002           ; [bp-0x2e], 0x0002
JNZ       L_628f              

ZoomBox:                            ; msg.c:187
CMP       [fViewFilteredMsg], 0x0000; [0x0b04], 0x0000
JNZ       L_61dc              

L_61d6:
MOV       ax, 0x0001          
JMP       L_61df              

L_61dc:
MOV       ax, 0x0000          

L_61df:
MOV       [fViewFilteredMsg], ax    ; [0x0b04], ax
                                    ; msg.c:190
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JL        L_623f              

L_61ec:
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       cx, ax              
AND       cx, 0x0007          
MOV       ax, 0x0001          
SHL       ax, cx              
MOV       [bp-0x30], ax       
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       bx, ax              
SAR       bx, 0x0001          
SAR       bx, 0x0001          
SAR       bx, 0x0001          
MOV       al, [bx+0x5324]     
AND       ax, 0x00ff          
MOV       cx, [bp-0x30]       
AND       ax, cx              
CMP       ax, 0x0000          
JZ        L_6233              

L_622d:
MOV       ax, 0x0001          
JMP       L_6236              

L_6233:
MOV       ax, 0x0000          

L_6236:
CMP       ax, [fViewFilteredMsg]    ; ax, [0x0b04]
JZ        L_626c              

L_623f:                             ; msg.c:192
PUSH      [fViewFilteredMsg]        ; [0x0b04]
CALLF     IMsgNext                  ; int16_t IMsgNext(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax
                                    ; msg.c:193
CMP       [bp-i], 0xffff            ; [bp-0x6], 0xffff
JNZ       L_6266              

L_6257:                             ; msg.c:194
PUSH      [fViewFilteredMsg]        ; [0x0b04]
CALLF     IMsgPrev                  ; int16_t IMsgPrev(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax

L_6266:                             ; msg.c:195
MOV       ax, [bp-i]                ; ax, [bp-0x6]
MOV       [iMsgCur], ax             ; [0x0b00], ax

L_626c:                             ; msg.c:197
PUSH      [hwndMessage]             ; [0x0198]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; msg.c:198
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:200
JMP       L_7207              

L_628f:
CMP       [bp-ht], 0x0003           ; [bp-0x2e], 0x0003
JNZ       L_7207              

ToggleMsgMode:                      ; msg.c:203
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_62ba              

L_62ab:                             ; msg.c:205
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     FFinishPlrMsgEntry        ; int16_t FFinishPlrMsgEntry(int16_t dInc)
ADD       sp, 0x0002          
                                    ; msg.c:207
JMP       L_637a              

L_62ba:
MOV       ax, [cMsg]                ; ax, [0x0afe]
CMP       [iMsgCur], ax             ; [0x0b00], ax
JL        L_6374              

L_62c6:                             ; msg.c:212
MOV       ax, [vlpmsgplrIn]         ; ax, [0x0b06]
MOV       dx, [vlpmsgplrIn+0x2]     ; dx, [0x0b08]
MOV       [bp-lpmpSrc], ax          ; [bp-0x36], ax
MOV       [bp-lpmpSrc+0x2], dx      ; [bp-0x34], dx
                                    ; msg.c:213
MOV       ax, [iMsgCur]             ; ax, [0x0b00]
SUB       ax, [cMsg]                ; ax, [0x0afe]
MOV       [bp-i], ax                ; [bp-0x6], ax

L_62dd:                             ; msg.c:214
MOV       ax, [bp-i]                ; ax, [bp-0x6]
SUB       [bp-i], 0x0001            ; [bp-0x6], 0x0001
CMP       ax, 0x0000          
JZ        L_62ff              

L_62ec:                             ; msg.c:215
LES       bx, [bp-lpmpSrc]          ; bx, [bp-0x36]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmpSrc], ax          ; [bp-0x36], ax
MOV       [bp-lpmpSrc+0x2], dx      ; [bp-0x34], dx
JMP       L_62dd              

L_62ff:                             ; msg.c:217
MOV       ax, [vlpmsgplrOut]        ; ax, [0x0b0a]
MOV       dx, [vlpmsgplrOut+0x2]    ; dx, [0x0b0c]
MOV       [bp-lpmp], ax             ; [bp-0x32], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x30], dx
                                    ; msg.c:218
MOV       [iMsgSendCur], 0x0000     ; [0x0b02], 0x0000
                                    ; msg.c:219
JMP       L_6352              

L_6315:                             ; msg.c:222
LES       bx, [bp-lpmp]             ; bx, [bp-0x32]
MOV       ax, es:[bx+0x6]     
ADD       ax, 0xffff          
LES       bx, [bp-lpmpSrc]          ; bx, [bp-0x36]
CMP       ax, es:[bx+0x4]     
JNZ       L_633d              

L_632b:
LES       bx, [bp-lpmp]             ; bx, [bp-0x32]
MOV       ax, [iMsgCur]             ; ax, [0x0b00]
CMP       es:[bx+0x8], ax     
JZ        L_6364              

L_633d:                             ; msg.c:225
LES       bx, [bp-lpmp]             ; bx, [bp-0x32]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmp], ax             ; [bp-0x32], ax
MOV       [bp-lpmp+0x2], dx         ; [bp-0x30], dx
                                    ; msg.c:226
ADD       [iMsgSendCur], 0x0001     ; [0x0b02], 0x0001

L_6352:                             ; msg.c:227
CMP       [bp-lpmp], 0x0000         ; [bp-0x32], 0x0000
JNZ       L_6315              

L_635b:
CMP       [bp-lpmp+0x2], 0x0000     ; [bp-0x30], 0x0000
JNZ       L_6315              

L_6364:                             ; msg.c:228
LES       bx, [bp-lpmpSrc]          ; bx, [bp-0x36]
MOV       ax, es:[bx+0x4]     
ADD       ax, 0x0001          
MOV       [viInRe], ax              ; [0x0b12], ax
                                    ; msg.c:230
JMP       L_637a              

L_6374:                             ; msg.c:231
MOV       [viInRe], 0x0000          ; [0x0b12], 0x0000

L_637a:                             ; msg.c:233
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_6393              

L_638d:
MOV       ax, 0x0001          
JMP       L_6396              

L_6393:
MOV       ax, 0x0000          

L_6396:
MOV       [bp-0x30], ax       
MOV       ax, [bp-0x30]       
AND       ax, 0x0001          
MOV       cx, 0x0008          
SHL       ax, cx              
MOV       cx, [gd]                  ; cx, [0x07ca]
AND       cx, 0xfeff          
OR        cx, ax              
MOV       [gd], cx                  ; [0x07ca], cx
MOV       ax, cx              
                                    ; msg.c:234
PUSH      [hwndMessage]             ; [0x0198]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; msg.c:235
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:236
PUSH      [hwndMsgEdit]             ; [0x5924]
CALLF     SetFocus                  ; HWND SetFocus(HWND arg1)

L_63dd:                             ; msg.c:239
JMP       L_7207              

L_63e0:                             ; msg.c:242
MOV       ax, [dxWinFrame]          ; ax, [0x23ec]
SHL       ax, 0x0001          
ADD       ax, 0x00c6          
MOV       bx, [bp+lParam]           ; bx, [bp+0x6]
MOV       cx, [bp+lParam+0x2]       ; cx, [bp+0x8]
MOV       es, cx              
MOV       es:[bx+0xc], ax     
                                    ; msg.c:243
MOV       ax, 0x000d          
IMUL      [dyArial8]                ; [0x23fa]
SAR       ax, 0x0001          
ADD       ax, 0x0016          
MOV       bx, [bp+lParam]           ; bx, [bp+0x6]
MOV       cx, [bp+lParam+0x2]       ; cx, [bp+0x8]
MOV       es, cx              
MOV       es:[bx+0xe], ax     
                                    ; msg.c:244
JMP       Default             

L_640f:                             ; msg.c:247
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
CMP       ax, [hwndMsgScroll]       ; ax, [0x531a]
JNZ       Default             

L_641e:                             ; msg.c:249
PUSH      [bp+wParam]               ; [bp+0xa]
PUSH      [crButtonFace+0x2]        ; [0x22bc]
PUSH      [crButtonFace]            ; [0x22ba]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:250
MOV       ax, [hbrButtonFace]       ; ax, [0x0010]
MOV       dx, 0x0000          
JMP       L_7210              

L_643a:                             ; msg.c:269
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-ps]               ; ax, [bp-0x28]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     BeginPaint                ; HDC BeginPaint(HWND arg1, PAINTSTRUCT *arg2)
MOV       [bp-hdc], ax              ; [bp-0x4], ax
                                    ; msg.c:271
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x494a          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     _Draw3dFrame              ; void _Draw3dFrame(HDC hdc, RECT *prc, int16_t fErase)
ADD       sp, 0x0006          
                                    ; msg.c:272
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
MOV       [bp-crFore], ax           ; [bp-0x32], ax
MOV       [bp-crFore+0x2], dx       ; [bp-0x30], dx
                                    ; msg.c:273
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [crButtonFace+0x2]        ; [0x22bc]
PUSH      [crButtonFace]            ; [0x22ba]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
MOV       [bp-crBack], ax           ; [bp-0x3a], ax
MOV       [bp-crBack+0x2], dx       ; [bp-0x38], dx
                                    ; msg.c:275
MOV       ax, 0x484c          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
MOV       [bp-cch], ax              ; [bp-0x48], ax
                                    ; msg.c:276
MOV       ax, [rcMsgTitle+0x4]      ; ax, [0x494e]
SUB       ax, [rcMsgTitle]          ; ax, [0x494a]
ADD       ax, 0xffd0          
MOV       [bp-dxMax], ax            ; [bp-0x34], ax

L_64a7:                             ; msg.c:278
CMP       [bp-cch], 0x0000          ; [bp-0x48], 0x0000
JLE       L_64d1              

L_64b0:
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x484c          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cch]                  ; [bp-0x48]
CALLF     GetTextExtent             ; uint32_t GetTextExtent(HDC arg1, LPCSTR arg2, int16_t arg3)
CMP       ax, [bp-dxMax]            ; ax, [bp-0x34]
JLE       L_64d1              

L_64ca:                             ; msg.c:279
SUB       [bp-cch], 0x0001          ; [bp-0x48], 0x0001
JMP       L_64a7              

L_64d1:                             ; msg.c:281
PUSH      [bp-cch]                  ; [bp-0x48]
MOV       ax, 0x484c          
PUSH      ax                  
MOV       ax, 0x494a          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     RcCtrTextOut              ; void RcCtrTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen)
ADD       sp, 0x0008          
                                    ; msg.c:282
MOV       ax, 0x494a          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     DecorateMsgTitleBar       ; void DecorateMsgTitleBar(HDC hdc, RECT *prc)
ADD       sp, 0x0004          
                                    ; msg.c:284
MOV       si, 0x22b2          
LEA       di, [bp-rc]               ; di, [bp-0x72]
PUSH      ss                  
POP       es                  
MOVSW     es:[di], [rcMsgText]      ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x2]  ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x4]  ; es:[di], ds:[si]
MOVSW     es:[di], [rcMsgText+0x6]  ; es:[di], ds:[si]
LEA       ax, [bp-0x72]       
                                    ; msg.c:285
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x6e]
SUB       ax, [bp-rc]               ; ax, [bp-0x72]
MOV       [bp-dx], ax               ; [bp-0x46], ax
                                    ; msg.c:286
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x6c]
SUB       ax, [bp-rc+0x2]           ; ax, [bp-0x70]
MOV       [bp-dy], ax               ; [bp-0x36], ax
                                    ; msg.c:288
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [hbrButtonShadow]         ; [0x0016]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
MOV       [bp-hbrSav], ax           ; [bp-0x2e], ax
                                    ; msg.c:289
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-rc]                   ; [bp-0x72]
PUSH      [bp-rc+0x2]               ; [bp-0x70]
PUSH      [bp-dx]                   ; [bp-0x46]
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0021          
MOV       dx, 0x00f0          
PUSH      dx                  
PUSH      ax                  
CALLF     PatBlt                    ; int16_t PatBlt(HDC arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, uint32_t arg6)
                                    ; msg.c:290
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-rc]                   ; [bp-0x72]
PUSH      [bp-rc+0x2]               ; [bp-0x70]
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-dy]                   ; [bp-0x36]
MOV       ax, 0x0021          
MOV       dx, 0x00f0          
PUSH      dx                  
PUSH      ax                  
CALLF     PatBlt                    ; int16_t PatBlt(HDC arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, uint32_t arg6)
                                    ; msg.c:292
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [hbrButtonHilite]         ; [0x0014]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; msg.c:293
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-rc]                   ; [bp-0x72]
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x6c]
ADD       ax, 0xffff          
PUSH      ax                  
PUSH      [bp-dx]                   ; [bp-0x46]
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0021          
MOV       dx, 0x00f0          
PUSH      dx                  
PUSH      ax                  
CALLF     PatBlt                    ; int16_t PatBlt(HDC arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, uint32_t arg6)
                                    ; msg.c:294
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x6e]
ADD       ax, 0xffff          
PUSH      ax                  
PUSH      [bp-rc+0x2]               ; [bp-0x70]
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-dy]                   ; [bp-0x36]
MOV       ax, 0x0021          
MOV       dx, 0x00f0          
PUSH      dx                  
PUSH      ax                  
CALLF     PatBlt                    ; int16_t PatBlt(HDC arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, uint32_t arg6)
                                    ; msg.c:296
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-hbrSav]               ; [bp-0x2e]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; msg.c:298
MOV       ax, 0xfffc          
PUSH      ax                  
MOV       ax, 0xfffc          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x72]
PUSH      ax                  
CALLF     ExpandRc                  ; void ExpandRc(RECT *prc, int16_t dx, int16_t dy)
ADD       sp, 0x0006          
                                    ; msg.c:300
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_69ee              

L_65e0:                             ; msg.c:302
MOV       ax, [cMsg]                ; ax, [0x0afe]
CMP       [iMsgCur], ax             ; [0x0b00], ax
JL        L_6796              

L_65ec:                             ; msg.c:304
MOV       ax, [vlpmsgplrIn]         ; ax, [0x0b06]
MOV       dx, [vlpmsgplrIn+0x2]     ; dx, [0x0b08]
MOV       [bp-lpmsgplr], ax         ; [bp-0x76], ax
MOV       [bp-lpmsgplr+0x2], dx     ; [bp-0x74], dx
                                    ; msg.c:305
MOV       ax, [cMsg]                ; ax, [0x0afe]
MOV       [bp-i], ax                ; [bp-0x6], ax
JMP       L_6616              

L_6602:                             ; msg.c:306
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpmsgplr], ax         ; [bp-0x76], ax
MOV       [bp-lpmsgplr+0x2], dx     ; [bp-0x74], dx
ADD       [bp-i], 0x0001            ; [bp-0x6], 0x0001

L_6616:
MOV       ax, [iMsgCur]             ; ax, [0x0b00]
CMP       [bp-i], ax                ; [bp-0x6], ax
JL        L_6602              

L_6621:                             ; msg.c:308
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
PUSH      ax                  
MOV       ax, 0x054a          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
CMP       ax, 0x0020          
JGE       L_663f              

L_6639:
MOV       ax, 0x0001          
JMP       L_6642              

L_663f:
MOV       ax, 0x0000          

L_6642:                             ; msg.c:309
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000d          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
PUSH      es:[bx+0x4]         
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [lpb2k+0x2]               ; [0x5208]
PUSH      [lpb2k]                   ; [0x5206]
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x0010          
MOV       [bp-cch], ax              ; [bp-0x48], ax
                                    ; msg.c:310
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
PUSH      ax                  
MOV       ax, 0x054b          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
CMP       ax, 0x0020          
JGE       L_66a9              

L_66a3:
MOV       ax, 0x0001          
JMP       L_66ac              

L_66a9:
MOV       ax, 0x0000          

L_66ac:                             ; msg.c:313
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000d          
PUSH      ax                  
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
CMP       es:[bx+0x6], 0x0000 
JNZ       L_66d2              

L_66c1:
MOV       ax, 0x0549          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
JMP       L_66fb              

L_66d2:
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
MOV       ax, es:[bx+0x6]     
ADD       ax, 0xffff          
PUSH      ax                  
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       dx, ds              

L_66fb:
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-cch]              ; ax, [bp-0x48]
MOV       cx, [lpb2k]               ; cx, [0x5206]
MOV       dx, [lpb2k+0x2]           ; dx, [0x5208]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x0010          
ADD       [bp-cch], ax              ; [bp-0x48], ax
                                    ; msg.c:314
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
CMP       es:[bx+0xa], 0x0000 
JL        L_6762              

L_672b:                             ; msg.c:316
MOV       [bp-i], 0x03e8            ; [bp-0x6], 0x03e8
                                    ; msg.c:317
LEA       ax, [bp-i]                ; ax, [bp-0x6]
PUSH      ax                  
MOV       ax, [bp-cch]              ; ax, [bp-0x48]
MOV       cx, [lpb2k]               ; cx, [0x5206]
MOV       dx, [lpb2k+0x2]           ; dx, [0x5208]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
LES       bx, [bp-lpmsgplr]         ; bx, [bp-0x76]
PUSH      es:[bx+0xa]         
MOV       ax, 0x000c          
MOV       cx, [bp-lpmsgplr]         ; cx, [bp-0x76]
MOV       dx, [bp-lpmsgplr+0x2]     ; dx, [bp-0x74]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     FDecompressUserString     ; int16_t FDecompressUserString(char *szIn, int16_t cIn, char *szOut, int16_t *pcOut)
ADD       sp, 0x000c          
                                    ; msg.c:319
JMP       L_6786              

L_6762:                             ; msg.c:320
MOV       ax, 0x000c          
MOV       cx, [bp-lpmsgplr]         ; cx, [bp-0x76]
MOV       dx, [bp-lpmsgplr+0x2]     ; dx, [bp-0x74]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-cch]              ; ax, [bp-0x48]
MOV       cx, [lpb2k]               ; cx, [0x5206]
MOV       dx, [lpb2k+0x2]           ; dx, [0x5208]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          

L_6786:                             ; msg.c:321
MOV       ax, [lpb2k]               ; ax, [0x5206]
MOV       dx, [lpb2k+0x2]           ; dx, [0x5208]
MOV       [bp-lpsz], ax             ; [bp-0x2c], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x2a], dx
                                    ; msg.c:323
JMP       L_683c              

L_6796:                             ; msg.c:325
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       [bp-idm], ax              ; [bp-0x3c], ax
                                    ; msg.c:326
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JGE       L_67d0              

L_67af:
CMP       [cMsg], 0x0000            ; [0x0afe], 0x0000
JLE       L_67d0              

L_67b9:                             ; msg.c:327
MOV       ax, 0x0022          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
MOV       [bp-lpsz], ax             ; [bp-0x2c], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x2a], dx
                                    ; msg.c:328
JMP       L_683c              

L_67d0:
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JL        L_6828              

L_67da:
MOV       cx, [bp-idm]              ; cx, [bp-0x3c]
AND       cx, 0x0007          
MOV       ax, 0x0001          
SHL       ax, cx              
MOV       bx, [bp-idm]              ; bx, [bp-0x3c]
SAR       bx, 0x0001          
SAR       bx, 0x0001          
SAR       bx, 0x0001          
MOV       cl, [bx+0x5324]     
MOV       [bp-0x74], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
MOV       cx, [bp-0x74]       
AND       ax, cx              
CMP       ax, 0x0000          
JZ        L_6828              

L_6807:
CMP       [fViewFilteredMsg], 0x0000; [0x0b04], 0x0000
JNZ       L_6828              

L_6811:                             ; msg.c:329
MOV       ax, 0x0021          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
MOV       [bp-lpsz], ax             ; [bp-0x2c], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x2a], dx
                                    ; msg.c:330
JMP       L_683c              

L_6828:                             ; msg.c:331
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     PszGetMessageN            ; char * PszGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       dx, ds              
MOV       [bp-lpsz], ax             ; [bp-0x2c], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x2a], dx

L_683c:                             ; msg.c:334
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0xffff          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:335
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)
MOV       [bp-iMode], ax            ; [bp-0x4a], ax
                                    ; msg.c:336
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JGE       L_686f              

L_6865:
CMP       [cMsg], 0x0000            ; [0x0afe], 0x0000
JG        L_68c6              

L_686f:
CMP       [iMsgCur], 0x0000         ; [0x0b00], 0x0000
JL        L_6903              

L_6879:
MOV       ax, [cMsg]                ; ax, [0x0afe]
CMP       [iMsgCur], ax             ; [0x0b00], ax
JGE       L_6903              

L_6885:
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       cx, ax              
AND       cx, 0x0007          
MOV       ax, 0x0001          
SHL       ax, cx              
MOV       [bp-0x74], ax       
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       bx, ax              
SAR       bx, 0x0001          
SAR       bx, 0x0001          
SAR       bx, 0x0001          
MOV       al, [bx+0x5324]     
AND       ax, 0x00ff          
MOV       cx, [bp-0x74]       
AND       ax, cx              
CMP       ax, 0x0000          
JZ        L_6903              

L_68c6:                             ; msg.c:338
MOV       ax, 0x57a4          
PUSH      ax                  
MOV       ax, 0x0548          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x48], ax
                                    ; msg.c:339
PUSH      [bp-cch]                  ; [bp-0x48]
MOV       ax, 0x57a4          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x72]
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     DiaganolTextOut           ; void DiaganolTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen)
ADD       sp, 0x0008          
                                    ; msg.c:340
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     PszGetMessageN            ; char * PszGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       dx, ds              
MOV       [bp-lpsz], ax             ; [bp-0x2c], ax
MOV       [bp-lpsz+0x2], dx         ; [bp-0x2a], dx

L_6903:                             ; msg.c:343
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:345
LEA       si, [bp-rc]               ; si, [bp-0x72]
LEA       di, [bp-rcActual]         ; di, [bp-0x44]
PUSH      ss                  
POP       es                  
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
MOVSW     es:[di], ds:[si]    
LEA       ax, [bp-0x44]       
                                    ; msg.c:347
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-lpsz+0x2]             ; [bp-0x2a]
PUSH      [bp-lpsz]                 ; [bp-0x2c]
PUSH      [bp-lpsz+0x2]             ; [bp-0x2a]
PUSH      [bp-lpsz]                 ; [bp-0x2c]
CALLF     fstrlen                   ; uint16_t fstrlen(char *s)
ADD       sp, 0x0004          
PUSH      ax                  
LEA       ax, [bp-rcActual]         ; ax, [bp-0x44]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0c10          
PUSH      ax                  
CALLF     DrawText                  ; int16_t DrawText(HDC arg1, LPCSTR arg2, int16_t arg3, RECT *arg4, uint16_t arg5)
                                    ; msg.c:349
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x6c]
CMP       [bp-rcActual+0x6], ax     ; [bp-0x3e], ax
JG        L_6998              

L_6955:
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x6e]
CMP       [bp-rcActual+0x4], ax     ; [bp-0x40], ax
JG        L_6998              

L_6960:                             ; msg.c:351
PUSH      [hwndMsgScroll]           ; [0x531a]
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     ShowWindow                ; int16_t ShowWindow(HWND arg1, int16_t arg2)
                                    ; msg.c:353
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-lpsz+0x2]             ; [bp-0x2a]
PUSH      [bp-lpsz]                 ; [bp-0x2c]
PUSH      [bp-lpsz+0x2]             ; [bp-0x2a]
PUSH      [bp-lpsz]                 ; [bp-0x2c]
CALLF     fstrlen                   ; uint16_t fstrlen(char *s)
ADD       sp, 0x0004          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x72]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0810          
PUSH      ax                  
CALLF     DrawText                  ; int16_t DrawText(HDC arg1, LPCSTR arg2, int16_t arg3, RECT *arg4, uint16_t arg5)
                                    ; msg.c:355
JMP       L_69e0              

L_6998:                             ; msg.c:357
PUSH      [hwndMsgScroll]           ; [0x531a]
PUSH      [bp-lpsz+0x2]             ; [bp-0x2a]
PUSH      [bp-lpsz]                 ; [bp-0x2c]
CALLF     SetWindowText             ; void SetWindowText(HWND arg1, LPCSTR arg2)
                                    ; msg.c:358
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, 0x0004          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0x72]
PUSH      ax                  
CALLF     ExpandRc                  ; void ExpandRc(RECT *prc, int16_t dx, int16_t dy)
ADD       sp, 0x0006          
                                    ; msg.c:361
PUSH      [hwndMsgScroll]           ; [0x531a]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-rc]                   ; [bp-0x72]
PUSH      [bp-rc+0x2]               ; [bp-0x70]
MOV       ax, [bp-rc+0x4]           ; ax, [bp-0x6e]
SUB       ax, [bp-rc]               ; ax, [bp-0x72]
PUSH      ax                  
MOV       ax, [bp-rc+0x6]           ; ax, [bp-0x6c]
SUB       ax, [bp-rc+0x2]           ; ax, [bp-0x70]
PUSH      ax                  
MOV       ax, 0x0044          
PUSH      ax                  
CALLF     SetWindowPos              ; int16_t SetWindowPos(HWND arg1, HWND arg2, int16_t arg3, int16_t arg4, int16_t arg5, int16_t arg6, uint16_t arg7)

L_69e0:                             ; msg.c:364
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-iMode]                ; [bp-0x4a]
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)
                                    ; msg.c:366
JMP       L_6a4b              

L_69ee:                             ; msg.c:368
PUSH      [bp-hdc]                  ; [bp-0x4]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)
MOV       [bp-iMode], ax            ; [bp-0x4a], ax
                                    ; msg.c:369
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:370
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
PUSH      ax                  
MOV       ax, 0x0536          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-cch], ax              ; [bp-0x48], ax
                                    ; msg.c:371
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-cch]                  ; [bp-0x48]
LEA       ax, [bp-szT]              ; ax, [bp-0x6a]
PUSH      ax                  
PUSH      [bp-rc+0x2]               ; [bp-0x70]
MOV       ax, [bp-rc]               ; ax, [bp-0x72]
ADD       ax, 0x001a          
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0x4]
CALLF     RightTextOut              ; void RightTextOut(HDC hdc, int16_t x, int16_t y, char *psz, int16_t cLen, int16_t dxErase)
ADD       sp, 0x000c          
                                    ; msg.c:372
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-iMode]                ; [bp-0x4a]
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, int16_t arg2)

L_6a4b:                             ; msg.c:375
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-crFore+0x2]           ; [bp-0x30]
PUSH      [bp-crFore]               ; [bp-0x32]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:376
PUSH      [bp-hdc]                  ; [bp-0x4]
PUSH      [bp-crBack+0x2]           ; [bp-0x38]
PUSH      [bp-crBack]               ; [bp-0x3a]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
                                    ; msg.c:378
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-ps]               ; ax, [bp-0x28]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     EndPaint                  ; void EndPaint(HWND arg1, PAINTSTRUCT *arg2)
                                    ; msg.c:380
JMP       L_7207              

L_6a79:                             ; msg.c:386
CMP       [bp+wParam], 0x0028       ; [bp+0xa], 0x0028
JZ        NextMsg             

L_6a88:
CMP       [bp+wParam], 0x0026       ; [bp+0xa], 0x0026
JZ        PrevMsg             

L_6a94:                             ; msg.c:390
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       Default             

L_6aaa:                             ; msg.c:392
CMP       [bp+wParam], 0x0024       ; [bp+0xa], 0x0024
JNZ       L_6abf              

L_6ab3:                             ; msg.c:394
MOV       [iMsgCur], 0xffff         ; [0x0b00], 0xffff
                                    ; msg.c:395
JMP       NextMsg             

L_6abf:
CMP       [bp+wParam], 0x0023       ; [bp+0xa], 0x0023
JNZ       L_7207              

L_6ac8:                             ; msg.c:399
MOV       ax, [cMsg]                ; ax, [0x0afe]
ADD       ax, [vcmsgplrIn]          ; ax, [0x0b0e]
MOV       [iMsgCur], ax             ; [0x0b00], ax
                                    ; msg.c:400
JMP       PrevMsg             

L_6ad8:                             ; msg.c:407
CMP       [bp+wParam], 0x000d       ; [bp+0xa], 0x000d
JZ        GotoMsg             

L_6ae7:
CMP       [bp+wParam], 0x002b       ; [bp+0xa], 0x002b
JZ        CheckBox            

L_6af6:
CMP       [bp+wParam], 0x002d       ; [bp+0xa], 0x002d
JNZ       L_7207              

L_6aff:                             ; msg.c:414
MOV       [bp-i], 0x0000            ; [bp-0x2a], 0x0000
JMP       L_6b34              

L_6b07:                             ; msg.c:415
MOV       bx, [bp-i]                ; bx, [bp-0x2a]
MOV       al, [bx+0x5324]     
AND       ax, 0x00ff          
MOV       bx, [bp-i]                ; bx, [bp-0x2a]
MOV       cl, [bx+0x516a]     
MOV       [bp-0x2c], ax       
MOV       ax, cx              
AND       ax, 0x00ff          
MOV       cx, [bp-0x2c]       
AND       ax, cx              
CMP       ax, 0x0000          
JNZ       L_6b3f              

L_6b30:                             ; msg.c:418
ADD       [bp-i], 0x0001            ; [bp-0x2a], 0x0001

L_6b34:
MOV       ax, [bp-i]                ; ax, [bp-0x2a]
CMP       ax, 0x0031          
JC        L_6b07              

L_6b3f:
MOV       ax, [bp-i]                ; ax, [bp-0x2a]
CMP       ax, 0x0031          
JNZ       ZoomBox             

L_6b47:
JMP       L_7207              

L_6b50:                             ; msg.c:424
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_6b75              

L_6b6c:                             ; msg.c:425
PUSH      [hwndFrame]               ; [0x258c]
CALLF     SetFocus                  ; HWND SetFocus(HWND arg1)

L_6b75:                             ; msg.c:428
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
CMP       ax, [rghwndMsgBtn]        ; ax, [0x4b84]
JNZ       L_6c7e              

L_6b84:
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_6c7e              

PrevMsg:                            ; msg.c:431
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_6bc2              

L_6bb3:                             ; msg.c:433
MOV       ax, 0xffff          
PUSH      ax                  
CALLF     FFinishPlrMsgEntry        ; int16_t FFinishPlrMsgEntry(int16_t dInc)
ADD       sp, 0x0002          
                                    ; msg.c:434
JMP       SetupNewMsg         

L_6bc2:                             ; msg.c:436
MOV       ax, 0x0010          
PUSH      ax                  
CALLF     GetAsyncKeyState          ; int16_t GetAsyncKeyState(int16_t arg1)
AND       ax, 0xfffe          
CMP       ax, 0x0000          
JZ        L_6bee              

L_6bd6:                             ; msg.c:438
MOV       [iMsgCur], 0xffff         ; [0x0b00], 0xffff
                                    ; msg.c:439
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     IMsgNext                  ; int16_t IMsgNext(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax
                                    ; msg.c:441
JMP       L_6bfd              

L_6bee:                             ; msg.c:442
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     IMsgPrev                  ; int16_t IMsgPrev(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax

L_6bfd:                             ; msg.c:444
CMP       [bp-i], 0xffff            ; [bp-0x6], 0xffff
JZ        L_6c0f              

L_6c06:                             ; msg.c:445
MOV       ax, [bp-i]                ; ax, [bp-0x6]
MOV       [iMsgCur], ax             ; [0x0b00], ax
                                    ; msg.c:446
JMP       SetupNewMsg         

L_6c0f:
MOV       ax, [cMsg]                ; ax, [0x0afe]
ADD       ax, [vcmsgplrIn]          ; ax, [0x0b0e]
CMP       [iMsgCur], ax             ; [0x0b00], ax
JNZ       L_7207              

L_6c1f:                             ; msg.c:447
SUB       [iMsgCur], 0x0001         ; [0x0b00], 0x0001

SetupNewMsg:                        ; msg.c:451
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xefff          
OR        ax, 0x0000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; msg.c:452
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:453
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x22b2          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; msg.c:454
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_7207              

L_6c67:                             ; msg.c:456
MOV       ax, [tutor]               ; ax, [0x520c]
AND       ax, 0xfffb          
OR        ax, 0x0004          
MOV       [tutor], ax               ; [0x520c], ax
                                    ; msg.c:457
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_6c78:                             ; msg.c:459
JMP       L_7207              

L_6c7e:                             ; msg.c:462
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
CMP       ax, [rghwndMsgBtn+0x4]    ; ax, [0x4b88]
JNZ       L_6d25              

L_6c8d:
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_6d25              

NextMsg:                            ; msg.c:465
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_6ccb              

L_6cbc:                             ; msg.c:467
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     FFinishPlrMsgEntry        ; int16_t FFinishPlrMsgEntry(int16_t dInc)
ADD       sp, 0x0002          
                                    ; msg.c:468
JMP       SetupNewMsg         

L_6ccb:                             ; msg.c:470
MOV       ax, 0x0010          
PUSH      ax                  
CALLF     GetAsyncKeyState          ; int16_t GetAsyncKeyState(int16_t arg1)
AND       ax, 0xfffe          
CMP       ax, 0x0000          
JZ        L_6cfb              

L_6cdf:                             ; msg.c:472
MOV       ax, [cMsg]                ; ax, [0x0afe]
ADD       ax, [vcmsgplrIn]          ; ax, [0x0b0e]
MOV       [iMsgCur], ax             ; [0x0b00], ax
                                    ; msg.c:473
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     IMsgPrev                  ; int16_t IMsgPrev(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax
                                    ; msg.c:475
JMP       L_6d0a              

L_6cfb:                             ; msg.c:476
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     IMsgNext                  ; int16_t IMsgNext(int16_t fFilteredOnly)
ADD       sp, 0x0002          
MOV       [bp-i], ax                ; [bp-0x6], ax

L_6d0a:                             ; msg.c:478
CMP       [bp-i], 0xffff            ; [bp-0x6], 0xffff
JZ        L_7207              

L_6d13:                             ; msg.c:479
MOV       ax, [bp-i]                ; ax, [bp-0x6]
MOV       [iMsgCur], ax             ; [0x0b00], ax
                                    ; msg.c:480
JMP       SetupNewMsg         

L_6d25:                             ; msg.c:485
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
CMP       ax, [rghwndMsgBtn+0x6]    ; ax, [0x4b8a]
JNZ       L_6d62              

L_6d34:
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_6d62              

L_6d50:                             ; msg.c:487
MOV       ax, 0x03e8          
PUSH      ax                  
CALLF     FFinishPlrMsgEntry        ; int16_t FFinishPlrMsgEntry(int16_t dInc)
ADD       sp, 0x0002          
                                    ; msg.c:488
JMP       SetupNewMsg         

L_6d62:                             ; msg.c:491
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
CMP       ax, [rghwndMsgBtn+0x2]    ; ax, [0x4b86]
JNZ       Default             

L_6d71:
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       Default             

GotoMsg:                            ; msg.c:494
MOV       cx, 0x0008          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       ToggleMsgMode       

L_6da0:
MOV       ax, [cMsg]                ; ax, [0x0afe]
CMP       [iMsgCur], ax             ; [0x0b00], ax
JGE       ToggleMsgMode       

L_6daf:                             ; msg.c:497
MOV       ax, [mdMsgObj]            ; ax, [0x3ee2]
JMP       L_713a              

L_6db8:                             ; msg.c:507
PUSH      [idMsgObj]                ; [0x3ee6]
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     SelectAdjPlanet           ; void SelectAdjPlanet(int16_t dInc, int16_t idPlanet)
ADD       sp, 0x0004          
                                    ; msg.c:508
PUSH      [hwndScanner]             ; [0x0190]
CALLF     UpdateWindow              ; void UpdateWindow(HWND arg1)
                                    ; msg.c:509
PUSH      [hwndScanner]             ; [0x0190]
MOV       ax, 0x0102          
PUSH      ax                  
MOV       ax, 0x0076          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:510
PUSH      [iMsgCur]                 ; [0x0b00]
CALLF     IdmGetMessageN            ; int16_t IdmGetMessageN(int16_t iMsg)
ADD       sp, 0x0002          
MOV       [bp-idm], ax              ; [bp-0x2a], ax
                                    ; msg.c:512
CMP       [bp-idm], 0x003e          ; [bp-0x2a], 0x003e
JZ        L_6e1f              

L_6e02:
CMP       [bp-idm], 0x003f          ; [bp-0x2a], 0x003f
JZ        L_6e1f              

L_6e0b:
CMP       [bp-idm], 0x00af          ; [bp-0x2a], 0x00af
JL        L_7163              

L_6e15:
CMP       [bp-idm], 0x00b4          ; [bp-0x2a], 0x00b4
JG        L_7163              

L_6e1f:                             ; msg.c:514
MOV       cx, 0x000c          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_6e4c              

L_6e32:                             ; msg.c:516
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xefff          
OR        ax, 0x1000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; msg.c:517
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          
                                    ; msg.c:519
JMP       L_7163              

L_6e4c:
CMP       [sel+0x4], 0x0001         ; [0x495a], 0x0001
JNZ       L_7163              

L_6e56:
MOV       ax, [idMsgObj]            ; ax, [0x3ee6]
CMP       [sel+0x8], ax             ; [0x495e], ax
JNZ       L_7163              

L_6e62:                             ; msg.c:520
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     ChangeProduction          ; int16_t ChangeProduction(int16_t fClear)
ADD       sp, 0x0002          

L_6e6e:                             ; msg.c:524
JMP       L_7163              

L_6e71:                             ; msg.c:526
PUSH      [idMsgObj]                ; [0x3ee6]
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     SelectAdjFleet            ; void SelectAdjFleet(int16_t dInc, int16_t idFleet)
ADD       sp, 0x0004          
                                    ; msg.c:527
PUSH      [hwndScanner]             ; [0x0190]
CALLF     UpdateWindow              ; void UpdateWindow(HWND arg1)
                                    ; msg.c:528
PUSH      [hwndScanner]             ; [0x0190]
MOV       ax, 0x0102          
PUSH      ax                  
MOV       ax, 0x0076          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:529
JMP       L_7163              

L_6ea6:                             ; msg.c:532
PUSH      [vptMsg]                  ; [0x3edc]
CALLF     LpthFromId                ; THING * LpthFromId(int16_t idth)
ADD       sp, 0x0002          
MOV       [bp-lpth], ax             ; [bp-0x2c], ax
MOV       [bp-lpth+0x2], dx         ; [bp-0x2a], dx
                                    ; msg.c:534
CMP       [bp-lpth], 0x0000         ; [bp-0x2c], 0x0000
JNZ       L_6eca              

L_6ec1:
CMP       [bp-lpth+0x2], 0x0000     ; [bp-0x2a], 0x0000
JZ        L_7163              

L_6eca:                             ; msg.c:538
LES       bx, [bp-lpth]             ; bx, [bp-0x2c]
MOV       ax, es:[bx+0x2]     
MOV       dx, es:[bx+0x4]     
MOV       [bp-scan], ax             ; [bp-0x3c], ax
MOV       [bp-scan+0x2], dx         ; [bp-0x3a], dx
                                    ; msg.c:539
MOV       [bp-scan+0x4], 0x0008     ; [bp-0x38], 0x0008
                                    ; msg.c:541
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-scan]             ; ax, [bp-0x3c]
PUSH      ax                  
CALLF     ChangeScanSel             ; void ChangeScanSel(SCAN *pscan, int16_t fValidScan)
ADD       sp, 0x0004          
                                    ; msg.c:542
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-scan+0x2]             ; [bp-0x3a]
PUSH      [bp-scan]                 ; [bp-0x3c]
CALLF     CtrPointScan              ; void CtrPointScan(POINT16 pt, int16_t fScroll)
ADD       sp, 0x0006          

L_6f02:                             ; msg.c:544
JMP       L_7163              

L_6f05:                             ; msg.c:547
MOV       ax, 0x3edc          
PUSH      ax                  
CALLF     SelectOursAtObject        ; void SelectOursAtObject(POINT16 *ppt)
ADD       sp, 0x0002          
                                    ; msg.c:548
MOV       cx, 0x000c          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_6f33              

L_6f24:                             ; msg.c:549
PUSH      [idMsgObj]                ; [0x3ee6]
CALLF     BattleVCR                 ; void BattleVCR(int16_t iBattle)
ADD       sp, 0x0002          
                                    ; msg.c:550
JMP       L_7163              

L_6f33:                             ; msg.c:552
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xefff          
OR        ax, 0x1000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; msg.c:553
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetMsgTitle               ; void SetMsgTitle(HWND hwnd)
ADD       sp, 0x0002          

L_6f4a:                             ; msg.c:555
JMP       L_7163              

L_6f4d:                             ; msg.c:557
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x007e          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:558
JMP       L_7163              

L_6f69:                             ; msg.c:560
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x005f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:561
JMP       L_7163              

L_6f85:                             ; msg.c:563
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x007d          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:564
JMP       L_7163              

L_6fa1:                             ; msg.c:566
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x07de          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
                                    ; msg.c:567
JMP       L_7163              

L_6fbd:                             ; msg.c:569
CMP       [hwndReportDlg], 0x0000   ; [0x15c4], 0x0000
JZ        L_6fd2              

L_6fc7:
CMP       [vprptCur], vrptBattle    ; [0x15ac], 0x1536
JZ        L_7163              

L_6fd2:                             ; msg.c:570
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x0901          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)

L_6feb:                             ; msg.c:571
JMP       L_7163              

L_6fee:                             ; msg.c:577
MOV       [szWork+0xc8], 0x0002     ; [0x586c], 0x0002
                                    ; msg.c:579
MOV       ax, 0x8f68          
MOV       dx, 0x6f45          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; FARPROC MakeProcInstance(FARPROC arg1, HINSTANCE arg2)
MOV       [bp-lpProc], ax           ; [bp-0x2c], ax
MOV       [bp-lpProc+0x2], dx       ; [bp-0x2a], dx
                                    ; msg.c:581
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0056          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CMP       [hwndTitle], 0x0000       ; [0x0352], 0x0000
JZ        L_7026              

L_7020:
MOV       ax, [hwndTitle]           ; ax, [0x0352]
JMP       L_7029              

L_7026:
MOV       ax, [hwndFrame]           ; ax, [0x258c]

L_7029:
PUSH      ax                  
PUSH      [bp-lpProc+0x2]           ; [bp-0x2a]
PUSH      [bp-lpProc]               ; [bp-0x2c]
CALLF     DialogBox                 ; int16_t DialogBox(HINSTANCE arg1, LPCSTR arg2, HWND arg3, DLGPROC arg4)
MOV       [bp-fRet], ax             ; [bp-0x2e], ax
                                    ; msg.c:582
PUSH      [bp-lpProc+0x2]           ; [bp-0x2a]
PUSH      [bp-lpProc]               ; [bp-0x2c]
CALLF     FreeProcInstance          ; void FreeProcInstance(FARPROC arg1)
                                    ; msg.c:583
CMP       [bp-fRet], 0x0000         ; [bp-0x2e], 0x0000
JZ        L_7163              

L_704c:                             ; msg.c:587
LEA       ax, [bp-lSerial]          ; ax, [bp-0x32]
PUSH      ax                  
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     FValidSerialNo            ; int16_t FValidSerialNo(char *psz, int32_t *plSerial)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_7088              

L_7064:                             ; msg.c:589
MOV       ax, [bp-lSerial]          ; ax, [bp-0x32]
MOV       dx, [bp-lSerial+0x2]      ; dx, [bp-0x30]
MOV       [vSerialNumber], ax       ; [0x08ac], ax
MOV       [vSerialNumber+0x2], dx   ; [0x08ae], dx
                                    ; msg.c:590
MOV       ax, 0x000b          
PUSH      ax                  
MOV       ax, 0x5468          
PUSH      ax                  
MOV       ax, 0x519e          
PUSH      ax                  
CALLF     memcpy                    ; void * memcpy(void *dest, void *src, uint16_t count)
ADD       sp, 0x0006          
                                    ; msg.c:592
JMP       L_7163              

L_7088:
CMP       [vSerialNumber], 0x0000   ; [0x08ac], 0x0000
JNZ       L_7163              

L_7092:
CMP       [vSerialNumber+0x2], 0x0000 ; [0x08ae], 0x0000
JNZ       L_7163              

L_709c:                             ; msg.c:593
MOV       ax, 0x000b          
PUSH      ax                  
MOV       ax, 0x5468          
PUSH      ax                  
MOV       ax, 0x519e          
PUSH      ax                  
CALLF     memcpy                    ; void * memcpy(void *dest, void *src, uint16_t count)
ADD       sp, 0x0006          

L_70b0:                             ; msg.c:596
JMP       L_7163              

L_70b3:                             ; msg.c:598
MOV       cx, 0x0008          
MOV       ax, [idMsgObj]            ; ax, [0x3ee6]
SAR       ax, cx              
MOV       cx, ax              
AND       cx, 0x000f          
MOV       ax, 0x0001          
SHL       ax, cx              
MOV       [vpartBrowser], ax        ; [0x22aa], ax
                                    ; msg.c:599
MOV       ax, [idMsgObj]            ; ax, [0x3ee6]
AND       ax, 0x00ff          
MOV       [bp-0x2a], ax       
MOV       ax, [bp-0x2a]       
AND       ax, 0x00ff          
MOV       cx, [vpartBrowser+0x2]    ; cx, [0x22ac]
AND       cx, 0xff00          
OR        cx, ax              
MOV       [vpartBrowser+0x2], cx    ; [0x22ac], cx
MOV       ax, cx              
                                    ; msg.c:600
MOV       ax, 0x22aa          
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
                                    ; msg.c:601
CMP       [hwndBrowser], 0x0000     ; [0x018e], 0x0000
JZ        L_7115              

L_70fd:                             ; msg.c:604
PUSH      [hwndBrowserChild]        ; [0x5474]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; msg.c:606
JMP       L_7163              

L_7115:                             ; msg.c:608
MOV       [fBrowserValid], 0x0001   ; [0x0d22], 0x0001
                                    ; msg.c:609
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x0100          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     PostMessage               ; int16_t PostMessage(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)

L_7134:                             ; msg.c:611
JMP       L_7163              

L_713a:
CMP       ax, 0x000b          
JA        L_7163              

L_7142:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x714b]      

L_7163:                             ; msg.c:613
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_7207              

L_7176:                             ; msg.c:615
MOV       ax, [tutor]               ; ax, [0x520c]
AND       ax, 0xfffb          
OR        ax, 0x0004          
MOV       [tutor], ax               ; [0x520c], ax
                                    ; msg.c:616
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_7187:                             ; msg.c:618
JMP       L_7207              

Default:                            ; msg.c:624
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp+message]              ; [bp+0xc]
PUSH      [bp+wParam]               ; [bp+0xa]
PUSH      [bp+lParam+0x2]           ; [bp+0x8]
PUSH      [bp+lParam]               ; [bp+0x6]
CALLF     DefWindowProc             ; LRESULT DefWindowProc(HWND arg1, uint16_t arg2, WPARAM arg3, LPARAM arg4)
JMP       L_7210              

L_71a4:
CMP       ax, 0x0001          
JZ        L_5ca1              

L_71ac:
CMP       ax, 0x0005          
JZ        L_5ef1              

L_71b4:
CMP       ax, 0x000f          
JZ        L_643a              

L_71bc:
CMP       ax, 0x0014          
JZ        L_6059              

L_71c4:
CMP       ax, 0x0019          
JZ        L_640f              

L_71cc:
CMP       ax, 0x0020          
JZ        L_6084              

L_71d4:
CMP       ax, 0x0024          
JZ        L_63e0              

L_71dc:
CMP       ax, 0x0100          
JZ        L_6a79              

L_71e4:
CMP       ax, 0x0102          
JZ        L_6ad8              

L_71ec:
CMP       ax, 0x0111          
JZ        L_6b50              

L_71f4:
CMP       ax, 0x0201          
JZ        L_60cf              

L_71fc:
CMP       ax, 0x0203          
JNZ       Default             

L_7201:
JMP       L_60cf              

L_7207:                             ; msg.c:626
MOV       ax, 0x0000          
MOV       dx, 0x0000          

L_7210:                             ; msg.c:627
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF      0x000a              


