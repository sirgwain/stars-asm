; About  (stars)
;   addr: 0004:1252  len=716
;   sig:  int16_t About(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam)
;   params:
;     HWND             hwnd           [BP+0xe]
;     WMType           message        [BP+0xc]
;     uint16_t         wParam         [BP+0xa]
;     int32_t          lParam         [BP+0x6]
;   locals:
;     RECT             rc             [BP-0xa]
;     block 0004:12D1  len=0x15B
;       HWND             hwndCtl        [BP-0x10]
;       int16_t          i              [BP-0xe]
;       HDC              hdc            [BP-0xc]
;     block 0004:14A3  len=0x3C
;       int16_t() *      lpProc         [BP-0xe]
;
;   stats: blocks=2  labels=0

L_1252:                             ; stars.c:748
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0010          
PUSH      si                  
PUSH      di                  
                                    ; stars.c:751
MOV       ax, [bp+message]          ; ax, [bp+0xc]
JMP       L_14e5              

L_1261:                             ; stars.c:754
MOV       [iAbout1st], 0xfff5       ; [0x5994], 0xfff5
                                    ; stars.c:755
MOV       [iAboutPartial], 0x0000   ; [0x51ac], 0x0000
                                    ; stars.c:757
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0401          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CALLF     SzVersion                 ; char * SzVersion()
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SetWindowText             ; void SetWindowText(HWND arg1, LPCSTR *arg2)
                                    ; stars.c:759
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x000e          
PUSH      ax                  
MOV       ax, 0x0032          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SetTimer                  ; uint16_t SetTimer(HWND arg1, uint16_t arg2, uint16_t arg3, int32_t (**arg4)())
MOV       [uTimerId], ax            ; [0x01a2], ax
                                    ; stars.c:760
MOV       ax, 0x0001          
JMP       L_1516              

L_12a9:                             ; stars.c:763
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; stars.c:764
PUSH      [bp+wParam]               ; [bp+0xa]
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hbrButtonFace]           ; [0x0010]
CALLF     FillRect                  ; int16_t FillRect(HDC arg1, RECT *arg2, HBRUSH arg3)
                                    ; stars.c:765
MOV       ax, 0x0001          
JMP       L_1516              

L_12d1:                             ; stars.c:771
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041f          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
MOV       [bp-hwndCtl], ax          ; [bp-0x10], ax
                                    ; stars.c:773
ADD       [iAboutPartial], 0x0002   ; [0x51ac], 0x0002
                                    ; stars.c:774
MOV       ax, [dyArial8]            ; ax, [0x23fa]
CMP       [iAboutPartial], ax       ; [0x51ac], ax
JL        L_130c              

L_12f1:                             ; stars.c:776
MOV       [iAboutPartial], 0x0000   ; [0x51ac], 0x0000
                                    ; stars.c:777
ADD       [iAbout1st], 0x0001       ; [0x5994], 0x0001
                                    ; stars.c:779
CMP       [iAbout1st], 0x004e       ; [0x5994], 0x004e
JLE       L_130c              

L_1306:                             ; stars.c:780
MOV       [iAbout1st], 0xfff5       ; [0x5994], 0xfff5

L_130c:                             ; stars.c:783
PUSH      [bp-hwndCtl]              ; [bp-0x10]
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; stars.c:784
PUSH      [bp-hwndCtl]              ; [bp-0x10]
CALLF     GetDC                     ; HDC GetDC(HWND arg1)
MOV       [bp-hdc], ax              ; [bp-0xc], ax
                                    ; stars.c:786
PUSH      [bp-hdc]                  ; [bp-0xc]
PUSH      [rghfontArial8+0x2]       ; [0x26b6]
CALLF     SelectObject              ; HGDIOBJ SelectObject(HDC arg1, HGDIOBJ arg2)
                                    ; stars.c:787
PUSH      [bp-hdc]                  ; [bp-0xc]
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     SetBkMode                 ; int16_t SetBkMode(HDC arg1, BkMode arg2)
                                    ; stars.c:788
PUSH      [bp-hdc]                  ; [bp-0xc]
PUSH      [crButtonFace+0x2]        ; [0x22bc]
PUSH      [crButtonFace]            ; [0x22ba]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
                                    ; stars.c:789
PUSH      [bp-hdc]                  ; [bp-0xc]
PUSH      [crButtonText+0x2]        ; [0x25f8]
PUSH      [crButtonText]            ; [0x25f6]
CALLF     SetTextColor              ; COLORREF SetTextColor(HDC arg1, COLORREF arg2)
                                    ; stars.c:791
PUSH      [bp-hdc]                  ; [bp-0xc]
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-rc+0x4]               ; [bp-0x6]
PUSH      [bp-rc+0x6]               ; [bp-0x4]
CALLF     IntersectClipRect         ; int16_t IntersectClipRect(HDC arg1, int16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5)
                                    ; stars.c:793
MOV       ax, [iAboutPartial]       ; ax, [0x51ac]
SUB       [bp-rc+0x2], ax           ; [bp-0x8], ax
                                    ; stars.c:794
MOV       ax, [bp-rc+0x2]           ; ax, [bp-0x8]
ADD       ax, [dyArial8]            ; ax, [0x23fa]
MOV       [bp-rc+0x6], ax           ; [bp-0x4], ax
                                    ; stars.c:796
MOV       ax, [iAbout1st]           ; ax, [0x5994]
MOV       [bp-i], ax                ; [bp-0xe], ax
JMP       L_1391              

L_138d:
ADD       [bp-i], 0x0001            ; [bp-0xe], 0x0001

L_1391:
MOV       ax, [iAbout1st]           ; ax, [0x5994]
ADD       ax, 0x000a          
CMP       [bp-i], ax                ; [bp-0xe], ax
JGE       L_13fa              

L_139f:                             ; stars.c:798
CMP       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JL        L_13d7              

L_13a8:
CMP       [bp-i], 0x004d            ; [bp-0xe], 0x004d
JGE       L_13d7              

L_13b1:                             ; stars.c:800
MOV       ax, 0xffff          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xe]
ADD       ax, 0x0277          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
PUSH      ax                  
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
PUSH      ax                  
PUSH      [bp-hdc]                  ; [bp-0xc]
CALLF     RcCtrTextOut              ; void RcCtrTextOut(HDC hdc, RECT *prc, char *psz, int16_t cLen)
ADD       sp, 0x0008          
                                    ; stars.c:802
JMP       L_13e3              

L_13d7:
CMP       [bp-i], 0x004d            ; [bp-0xe], 0x004d
JGE       L_13fa              

L_13dd:
JMP       L_13e3              

L_13e0:                             ; stars.c:803
JMP       L_13fa              

L_13e3:                             ; stars.c:805
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [dyArial8]                ; [0x23fa]
CALLF     OffsetRect                ; void OffsetRect(RECT *arg1, int16_t arg2, int16_t arg3)
                                    ; stars.c:806
JMP       L_138d              

L_13fa:                             ; stars.c:808
MOV       [bp-rc+0x6], 0x03e8       ; [bp-0x4], 0x03e8
                                    ; stars.c:809
PUSH      [bp-hdc]                  ; [bp-0xc]
LEA       ax, [bp-rc]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hbrButtonFace]           ; [0x0010]
CALLF     FillRect                  ; int16_t FillRect(HDC arg1, RECT *arg2, HBRUSH arg3)
                                    ; stars.c:811
PUSH      [bp-hdc]                  ; [bp-0xc]
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     SelectClipRgn             ; int16_t SelectClipRgn(HDC arg1, HRGN arg2)
                                    ; stars.c:812
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp-hdc]                  ; [bp-0xc]
CALLF     ReleaseDC                 ; int16_t ReleaseDC(HWND arg1, HDC arg2)
                                    ; stars.c:813
JMP       L_1510              

L_142c:                             ; stars.c:818
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0006          
JNZ       L_1510              

L_1448:                             ; stars.c:820
PUSH      [bp+wParam]               ; [bp+0xa]
PUSH      [crButtonFace+0x2]        ; [0x22bc]
PUSH      [crButtonFace]            ; [0x22ba]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
                                    ; stars.c:821
MOV       ax, [hbrButtonFace]       ; ax, [0x0010]
JMP       L_1516              

L_145e:                             ; stars.c:823
JMP       L_1510              

L_1461:                             ; stars.c:827
CMP       [bp+wParam], 0x0001       ; [bp+0xa], 0x0001
JZ        L_1473              

L_146a:
CMP       [bp+wParam], 0x0002       ; [bp+0xa], 0x0002
JNZ       L_149a              

L_1473:                             ; stars.c:829
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [uTimerId]                ; [0x01a2]
CALLF     KillTimer                 ; int16_t KillTimer(HWND arg1, uint16_t arg2)
                                    ; stars.c:830
MOV       [uTimerId], 0x0000        ; [0x01a2], 0x0000
                                    ; stars.c:832
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     EndDialog                 ; void EndDialog(HWND arg1, int16_t arg2)
                                    ; stars.c:833
MOV       ax, 0x0001          
JMP       L_1516              

L_149a:
CMP       [bp+wParam], 0x0076       ; [bp+0xa], 0x0076
JNZ       L_1510              

L_14a3:                             ; stars.c:839
MOV       ax, 0x151e          
MOV       dx, 0x127d          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [bp-lpProc], ax           ; [bp-0xe], ax
MOV       [bp-lpProc+0x2], dx       ; [bp-0xc], dx
                                    ; stars.c:840
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x0061          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp-lpProc+0x2]           ; [bp-0xc]
PUSH      [bp-lpProc]               ; [bp-0xe]
CALLF     DialogBox                 ; int16_t DialogBox(HINSTANCE arg1, DialogId arg2, HWND arg3, BOOL (**arg4)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))
                                    ; stars.c:841
PUSH      [bp-lpProc+0x2]           ; [bp-0xc]
PUSH      [bp-lpProc]               ; [bp-0xe]
CALLF     FreeProcInstance          ; void FreeProcInstance(int32_t (**arg1)())

L_14df:                             ; stars.c:843
JMP       L_1510              

L_14e5:
CMP       ax, 0x0014          
JZ        L_12a9              

L_14ed:
CMP       ax, 0x0019          
JZ        L_142c              

L_14f5:
CMP       ax, 0x0110          
JZ        L_1261              

L_14fd:
CMP       ax, 0x0111          
JZ        L_1461              

L_1505:
CMP       ax, 0x0113          
JZ        L_12d1              

L_1510:                             ; stars.c:845
MOV       ax, 0x0000          

L_1516:                             ; stars.c:846
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF      0x000a              


