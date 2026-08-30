; FCreateStuff  (init)
;   addr: 0001:0014  len=2718
;   sig:  int16_t FCreateStuff()
;   locals:
;     int16_t          dx             [BP-0xc]
;     HBITMAP          hbmp           [BP-0xa]
;     int16_t          i              [BP-0x8]
;     int16_t          dy             [BP-0x6]
;     int16_t          fFailed        [BP-0x4]
;
;   stats: blocks=0  labels=0

L_0014:                             ; init.c:29
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000c          
PUSH      si                  
PUSH      di                  
                                    ; init.c:33
MOV       [bp-fFailed], 0x0000      ; [bp-0x4], 0x0000
                                    ; init.c:36
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(SystemMetric arg1)
MOV       [bp-dx], ax               ; [bp-0xc], ax
                                    ; init.c:37
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetSystemMetrics          ; int16_t GetSystemMetrics(SystemMetric arg1)
MOV       [bp-dy], ax               ; [bp-0x6], ax
                                    ; init.c:39
CMP       [bp-dx], 0x0320           ; [bp-0xc], 0x0320
JL        L_004e              

L_0044:
CMP       [bp-dy], 0x0258           ; [bp-0x6], 0x0258
JGE       L_005d              

L_004e:                             ; init.c:40
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0x3fff          
OR        ax, 0x0000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; init.c:41
JMP       L_00af              

L_005d:
CMP       [bp-dx], 0x0400           ; [bp-0xc], 0x0400
JL        L_0071              

L_0067:
CMP       [bp-dy], 0x0300           ; [bp-0x6], 0x0300
JGE       L_0080              

L_0071:                             ; init.c:42
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0x3fff          
OR        ax, 0x4000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; init.c:43
JMP       L_00af              

L_0080:
CMP       [bp-dx], 0x0457           ; [bp-0xc], 0x0457
JL        L_0094              

L_008a:
CMP       [bp-dy], 0x0378           ; [bp-0x6], 0x0378
JGE       L_00a3              

L_0094:                             ; init.c:44
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0x3fff          
OR        ax, 0x8000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; init.c:45
JMP       L_00af              

L_00a3:                             ; init.c:46
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0x3fff          
OR        ax, 0xc000          
MOV       [gd], ax                  ; [0x07ca], ax

L_00af:                             ; init.c:48
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xff7f          
OR        ax, 0x0000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; init.c:49
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xfbff          
OR        ax, 0x0000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; init.c:50
MOV       si, 0x0da2          
MOV       di, 0x4f9a          
PUSH      ds                  
POP       es                  
MOV       cx, 0x0060          
MOVSW.REP [vplr], [vrgplrDef]       ; es:[di], ds:[si]
MOV       ax, 0x4f9a          
                                    ; init.c:52
MOV       ax, 0xfff6          
PUSH      ax                  
MOV       ax, 0xfff6          
PUSH      ax                  
MOV       ax, 0x07d0          
PUSH      ax                  
MOV       ax, 0x07d0          
PUSH      ax                  
CALLF     CreateRectRgn             ; HRGN CreateRectRgn(int16_t arg1, int16_t arg2, int16_t arg3, int16_t arg4)
MOV       [hrgnHuge], ax            ; [0x525a], ax
                                    ; init.c:53
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x000a          
PUSH      ax                  
CALLF     CreateRectRgn             ; HRGN CreateRectRgn(int16_t arg1, int16_t arg2, int16_t arg3, int16_t arg4)
MOV       [hrgnScratch], ax         ; [0x22be], ax
                                    ; init.c:55
MOV       ax, 0xff00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrShip], ax             ; [0x5318], ax
                                    ; init.c:56
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrStarbase], ax         ; [0x590c], ax
                                    ; init.c:57
MOV       ax, 0x0000          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrBBlue], ax            ; [0x599a], ax
                                    ; init.c:58
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrEnemy], ax            ; [0x5996], ax
                                    ; init.c:59
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrSelect], ax           ; [0x2430], ax
                                    ; init.c:60
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrRed], ax              ; [0x0024], ax
                                    ; init.c:61
MOV       ax, 0x0000          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrBlue], ax             ; [0x0028], ax
                                    ; init.c:62
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrGreen], ax            ; [0x0026], ax
                                    ; init.c:63
MOV       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrRadar], ax            ; [0x255a], ax
                                    ; init.c:64
MOV       ax, 0x007f          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrPurple], ax           ; [0x2692], ax
                                    ; init.c:65
MOV       ax, 0xffff          
MOV       dx, 0x009f          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrTooltip], ax          ; [0x275a], ax
                                    ; init.c:66
MOV       [hbrRadarNear], 0x0000    ; [0x568e], 0x0000
                                    ; init.c:68
MOV       ax, 0x0000          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMineral], ax        ; [0x257c], ax
                                    ; init.c:69
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMineral+0x2], ax    ; [0x257e], ax
                                    ; init.c:70
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMineral+0x4], ax    ; [0x2580], ax
                                    ; init.c:71
MOV       ax, 0xffff          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMineral+0x6], ax    ; [0x2582], ax
                                    ; init.c:72
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMineral+0x8], ax    ; [0x2584], ax
                                    ; init.c:74
MOV       ax, 0x0000          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr], ax     ; [0x5680], ax
                                    ; init.c:75
MOV       ax, 0x0000          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr+0x2], ax ; [0x5682], ax
                                    ; init.c:76
MOV       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr+0x4], ax ; [0x5684], ax
                                    ; init.c:77
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr+0x6], ax ; [0x5686], ax
                                    ; init.c:78
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr+0x8], ax ; [0x5688], ax
                                    ; init.c:79
MOV       ax, 0xff00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrPlanetAttr+0xa], ax ; [0x568a], ax
                                    ; init.c:81
MOV       ax, 0x0000          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum], ax         ; [0x5692], ax
                                    ; init.c:82
MOV       ax, 0x0000          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0x2], ax     ; [0x5694], ax
                                    ; init.c:83
MOV       ax, 0xff00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0x4], ax     ; [0x5696], ax
                                    ; init.c:84
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0x6], ax     ; [0x5698], ax
                                    ; init.c:85
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0x8], ax     ; [0x569a], ax
                                    ; init.c:86
MOV       ax, 0x7f7f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0xa], ax     ; [0x569c], ax
                                    ; init.c:88
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0xc], ax     ; [0x569e], ax
                                    ; init.c:89
MOV       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [rghbrMinSum+0xe], ax     ; [0x56a0], ax
                                    ; init.c:91
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrYellow], ax           ; [0x22c6], ax
                                    ; init.c:92
MOV       ax, 0x7f7f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrDkYellow], ax         ; [0x519c], ax
                                    ; init.c:94
MOV       ax, 0xc0c0          
MOV       dx, 0x00c0          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrLightGray], ax        ; [0x0020], ax
                                    ; init.c:95
MOV       ax, 0x8080          
MOV       dx, 0x0080          
PUSH      dx                  
PUSH      ax                  
CALLF     HbrGet                    ; HBRUSH HbrGet(COLORREF cr)
ADD       sp, 0x0004          
MOV       [hbrGray], ax             ; [0x0022], ax
                                    ; init.c:97
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0xff00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenShip], ax            ; [0x65a2], ax
                                    ; init.c:98
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenDkGreen], ax         ; [0x5358], ax
                                    ; init.c:99
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x00ff          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenStarbase], ax        ; [0x5980], ax
                                    ; init.c:100
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x00ff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenEnemy], ax           ; [0x568c], ax
                                    ; init.c:101
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x7f7f          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenMassPath], ax        ; [0x599e], ax
                                    ; init.c:102
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x007f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenRadar], ax           ; [0x5936], ax
                                    ; init.c:103
MOV       [hpenRadarNear], 0x0000   ; [0x567e], 0x0000
                                    ; init.c:104
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenDkBlue], ax          ; [0x5690], ax
                                    ; init.c:105
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0xffff          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenYellow], ax          ; [0x26c4], ax
                                    ; init.c:106
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x7f7f          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenDkYellow], ax        ; [0x23c8], ax
                                    ; init.c:107
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x007f          
MOV       dx, 0x007f          
PUSH      dx                  
PUSH      ax                  
CALLF     CreatePen                 ; HPEN CreatePen(int16_t arg1, int16_t arg2, COLORREF arg3)
MOV       [hpenDkPurple], ax        ; [0x26c2], ax
                                    ; init.c:109
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x05e6          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-hbmp], ax             ; [bp-0xa], ax
                                    ; init.c:110
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     CreatePatternBrush        ; HBRUSH CreatePatternBrush(HBITMAP arg1)
MOV       [hbr50Screen], ax         ; [0x4b78], ax
                                    ; init.c:111
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     DeleteObject              ; int16_t DeleteObject(HGDIOBJ arg1)
                                    ; init.c:113
MOV       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JMP       L_04f1              

L_04b7:                             ; init.c:115
PUSH      [hInst]                   ; [0x5310]
MOV       ax, [bp-i]                ; ax, [bp-0x8]
ADD       ax, 0x01cc          
MOV       dx, 0x0000          
OR        ax, 0x0000          
OR        dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-hbmp], ax             ; [bp-0xa], ax
                                    ; init.c:116
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     CreatePatternBrush        ; HBRUSH CreatePatternBrush(HBITMAP arg1)
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
MOV       [bx+0x591e], ax     
                                    ; init.c:117
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     DeleteObject              ; int16_t DeleteObject(HGDIOBJ arg1)
                                    ; init.c:118
ADD       [bp-i], 0x0001            ; [bp-0x8], 0x0001

L_04f1:
CMP       [bp-i], 0x0003            ; [bp-0x8], 0x0003
JL        L_04b7              

L_04fa:                             ; init.c:120
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x05f2          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-hbmp], ax             ; [bp-0xa], ax
                                    ; init.c:121
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     CreatePatternBrush        ; HBRUSH CreatePatternBrush(HBITMAP arg1)
MOV       [hbrCargo], ax            ; [0x520a], ax
                                    ; init.c:122
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     DeleteObject              ; int16_t DeleteObject(HGDIOBJ arg1)
                                    ; init.c:124
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x05fb          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-hbmp], ax             ; [bp-0xa], ax
                                    ; init.c:125
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     CreatePatternBrush        ; HBRUSH CreatePatternBrush(HBITMAP arg1)
MOV       [hbrDock], ax             ; [0x5262], ax
                                    ; init.c:126
PUSH      [bp-hbmp]                 ; [bp-0xa]
CALLF     DeleteObject              ; int16_t DeleteObject(HGDIOBJ arg1)
                                    ; init.c:128
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0603          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurScanner], ax         ; [0x4a66], ax
                                    ; init.c:129
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x060e          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurScanAdd], ax         ; [0x57a2], ax
                                    ; init.c:130
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0619          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurOpenGrab], ax        ; [0x4b7a], ax
                                    ; init.c:131
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0625          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurCloseGrab], ax       ; [0x5304], ax
                                    ; init.c:132
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x007a          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurTrashCan], ax        ; [0x258a], ax
                                    ; init.c:133
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0079          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurNoWay], ax           ; [0x3ee4], ax
                                    ; init.c:134
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0102          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurResizeWE], ax        ; [0x23f0], ax
                                    ; init.c:135
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0104          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurResizeNS], ax        ; [0x26c6], ax
                                    ; init.c:136
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0107          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurResize4Way], ax      ; [0x51ce], ax
                                    ; init.c:137
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0108          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurArrowHelp], ax       ; [0x48ae], ax
                                    ; init.c:138
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0109          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hcurHand], ax            ; [0x525e], ax
                                    ; init.c:140
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0632          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpScanner], ax         ; [0x4a5a], ax
                                    ; init.c:141
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0058          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpScanShip], ax        ; [0x4b7e], ax
                                    ; init.c:142
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x063d          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpUnknownPlanet], ax   ; [0x3efa], ax
                                    ; init.c:143
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x00f9          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpNumbers], ax         ; [0x597a], ax
                                    ; init.c:145
MOV       ax, 0x0070          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibPlanets], ax         ; [0x4952], ax
                                    ; init.c:146
MOV       ax, 0x0057          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibThings], ax          ; [0x242e], ax
                                    ; init.c:147
MOV       ax, 0x00b2          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibToolbar], ax         ; [0x598c], ax
                                    ; init.c:149
CMP       [hdibPlanets], 0x0000     ; [0x4952], 0x0000
JZ        L_06b7              

L_06a3:
CMP       [hdibThings], 0x0000      ; [0x242e], 0x0000
JZ        L_06b7              

L_06ad:
CMP       [hdibToolbar], 0x0000     ; [0x598c], 0x0000
JNZ       L_06bc              

L_06b7:                             ; init.c:150
MOV       [bp-fFailed], 0x0001      ; [bp-0x4], 0x0001

L_06bc:                             ; init.c:152
MOV       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JMP       L_0720              

L_06c4:                             ; init.c:154
MOV       ax, [bp-i]                ; ax, [bp-0x8]
ADD       ax, 0x0228          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
MOV       [bx+0x4b6e], ax     
                                    ; init.c:155
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
CMP       [bx+0x4b6e], 0x0000 
JNZ       L_06f0              

L_06eb:                             ; init.c:156
MOV       [bp-fFailed], 0x0001      ; [bp-0x4], 0x0001

L_06f0:                             ; init.c:158
MOV       ax, [bp-i]                ; ax, [bp-0x8]
ADD       ax, 0x022d          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
MOV       [bx+0x5160], ax     
                                    ; init.c:159
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
CMP       [bx+0x5160], 0x0000 
JNZ       L_071c              

L_0717:                             ; init.c:160
MOV       [bp-fFailed], 0x0001      ; [bp-0x4], 0x0001

L_071c:                             ; init.c:161
ADD       [bp-i], 0x0001            ; [bp-0x8], 0x0001

L_0720:
CMP       [bp-i], 0x0005            ; [bp-0x8], 0x0005
JL        L_06c4              

L_0729:                             ; init.c:162
MOV       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JMP       L_0761              

L_0731:                             ; init.c:164
MOV       ax, [bp-i]                ; ax, [bp-0x8]
ADD       ax, 0x01f4          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
MOV       [bx+0x258e], ax     
                                    ; init.c:165
MOV       bx, [bp-i]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
CMP       [bx+0x258e], 0x0000 
JNZ       L_075d              

L_0758:                             ; init.c:166
MOV       [bp-fFailed], 0x0001      ; [bp-0x4], 0x0001

L_075d:                             ; init.c:167
ADD       [bp-i], 0x0001            ; [bp-0x8], 0x0001

L_0761:
CMP       [bp-i], 0x0007            ; [bp-0x8], 0x0007
JL        L_0731              

L_076a:                             ; init.c:168
PUSH      [rghdibShips+0x6]         ; [0x4b74]
CALLF     HpalFromDib               ; HPALETTE HpalFromDib(HGLOBAL hdib)
ADD       sp, 0x0002          
MOV       [vhpal], ax               ; [0x5314], ax
                                    ; init.c:170
MOV       ax, 0x0085          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibRaces], ax           ; [0x5476], ax
                                    ; init.c:171
MOV       ax, 0x0050          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibRacesT], ax          ; [0x2606], ax
                                    ; init.c:172
MOV       ax, 0x004f          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibRacesX], ax          ; [0x2604], ax
                                    ; init.c:174
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0077          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpBackBld], ax         ; [0x24e2], ax
                                    ; init.c:175
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0086          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpMsg], ax             ; [0x260a], ax
                                    ; init.c:176
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x00c7          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadBitmap                ; HBITMAP LoadBitmap(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hbmpMono], ax            ; [0x5992], ax
                                    ; init.c:178
MOV       ax, 0x0437          
PUSH      ax                  
CALLF     HdibLoadBigResource       ; HGLOBAL HdibLoadBigResource(int16_t idb)
ADD       sp, 0x0002          
MOV       [hdibPlaque], ax          ; [0x0c82], ax
                                    ; init.c:180
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x064e          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hiconStars], ax          ; [0x0356], ax
                                    ; init.c:181
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0657          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hiconHost], ax           ; [0x0358], ax
                                    ; init.c:182
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x065f          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [hiconWait], ax           ; [0x035a], ax
                                    ; init.c:183
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0667          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR], ax          ; [0x24e6], ax
                                    ; init.c:184
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0670          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0x2], ax      ; [0x24e8], ax
                                    ; init.c:185
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0679          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0x4], ax      ; [0x24ea], ax
                                    ; init.c:186
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0682          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0x6], ax      ; [0x24ec], ax
                                    ; init.c:187
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x068b          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0x8], ax      ; [0x24ee], ax
                                    ; init.c:188
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0694          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0xa], ax      ; [0x24f0], ax
                                    ; init.c:189
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x069d          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     LoadIcon                  ; HICON LoadIcon(HINSTANCE arg1, LPCSTR *arg2)
MOV       [rghiconVCR+0xc], ax      ; [0x24f2], ax
                                    ; init.c:191
MOV       ax, 0x0004          
PUSH      ax                  
MOV       ax, 0x7d00          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [lpLog], ax               ; [0x09a0], ax
MOV       [lpLog+0x2], dx           ; [0x09a2], dx
                                    ; init.c:192
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0xffc8          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [lpMsg], ax               ; [0x0af8], ax
MOV       [lpMsg+0x2], dx           ; [0x0afa], dx
                                    ; init.c:194
MOV       ax, 0x1d72          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnFakeComboProc], ax   ; [0x2544], ax
MOV       [lpfnFakeComboProc+0x2], dx ; [0x2546], dx
                                    ; init.c:195
MOV       ax, 0x1df0          
MOV       dx, 0x08e1          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnFakeCEProc], ax      ; [0x2748], ax
MOV       [lpfnFakeCEProc+0x2], dx  ; [0x274a], dx
                                    ; init.c:196
MOV       ax, 0xa700          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnFakeEditProc], ax    ; [0x2538], ax
MOV       [lpfnFakeEditProc+0x2], dx; [0x253a], dx
                                    ; init.c:197
MOV       ax, 0x6758          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnFakeListProc], ax    ; [0x2402], ax
MOV       [lpfnFakeListProc+0x2], dx; [0x2404], dx
                                    ; init.c:198
MOV       ax, 0x7716          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnHostTimerProc], ax   ; [0x51fe], ax
MOV       [lpfnHostTimerProc+0x2], dx ; [0x5200], dx
                                    ; init.c:199
MOV       ax, 0x1ed8          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnBrowserDlgProc], ax  ; [0x51d0], ax
MOV       [lpfnBrowserDlgProc+0x2], dx ; [0x51d2], dx
                                    ; init.c:200
MOV       ax, 0x0018          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnReportDlgProc], ax   ; [0x3efc], ax
MOV       [lpfnReportDlgProc+0x2], dx ; [0x3efe], dx
                                    ; init.c:201
MOV       ax, 0x647e          
MOV       dx, 0x07e9          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [lpfnGaugeDlgProc], ax    ; [0x2408], ax
MOV       [lpfnGaugeDlgProc+0x2], dx; [0x240a], dx
                                    ; init.c:203
CALLF     GetDiskSerialNumber       ; uint32_t GetDiskSerialNumber()
                                    ; init.c:205
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x0800          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [lpb2k], ax               ; [0x5206], ax
MOV       [lpb2k+0x2], dx           ; [0x5208], dx
                                    ; init.c:206
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x0800          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [vlprgidMisc], ax         ; [0x15b4], ax
MOV       [vlprgidMisc+0x2], dx     ; [0x15b6], dx
                                    ; init.c:207
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x0800          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [vlprgidPlanet], ax       ; [0x15b8], ax
MOV       [vlprgidPlanet+0x2], dx   ; [0x15ba], dx
                                    ; init.c:208
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x0800          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
MOV       [vlprgidFleet], ax        ; [0x15bc], ax
MOV       [vlprgidFleet+0x2], dx    ; [0x15be], dx
                                    ; init.c:212
CMP       [bp-fFailed], 0x0000      ; [bp-0x4], 0x0000
JNZ       L_0a7f              

L_0a07:
CMP       [hbmpScanner], 0x0000     ; [0x4a5a], 0x0000
JZ        L_0a7f              

L_0a11:
CMP       [hbmpUnknownPlanet], 0x0000 ; [0x3efa], 0x0000
JZ        L_0a7f              

L_0a1b:
CMP       [hbmpBackBld], 0x0000     ; [0x24e2], 0x0000
JZ        L_0a7f              

L_0a25:
CMP       [hdibRaces], 0x0000       ; [0x5476], 0x0000
JZ        L_0a7f              

L_0a2f:
CMP       [hdibRacesT], 0x0000      ; [0x2606], 0x0000
JZ        L_0a7f              

L_0a39:
CMP       [hdibRacesX], 0x0000      ; [0x2604], 0x0000
JZ        L_0a7f              

L_0a43:
CMP       [hbmpMono], 0x0000        ; [0x5992], 0x0000
JZ        L_0a7f              

L_0a4d:
CMP       [hbmpScanShip], 0x0000    ; [0x4b7e], 0x0000
JZ        L_0a7f              

L_0a57:
CMP       [hbmpMsg], 0x0000         ; [0x260a], 0x0000
JZ        L_0a7f              

L_0a61:
CMP       [hiconHost], 0x0000       ; [0x0358], 0x0000
JZ        L_0a7f              

L_0a6b:
CMP       [hiconStars], 0x0000      ; [0x0356], 0x0000
JZ        L_0a7f              

L_0a75:
CMP       [hiconWait], 0x0000       ; [0x035a], 0x0000
JNZ       L_0aa6              

L_0a7f:                             ; init.c:214
MOV       ax, 0x0010          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0017          
PUSH      ax                  
CALLF     PszFormatIds              ; char * PszFormatIds(StringId ids, int16_t *pParams)
ADD       sp, 0x0006          
PUSH      ax                  
CALLF     AlertSz                   ; int16_t AlertSz(char *sz, MessageBoxType mbType)
ADD       sp, 0x0004          
                                    ; init.c:215
MOV       ax, 0x0000          
JMP       L_0aac              

L_0aa6:                             ; init.c:219
MOV       ax, 0x0001          

L_0aac:                             ; init.c:220
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


