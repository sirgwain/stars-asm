; BattlePlansDlg  (battle)
;   addr: 001f:0652  len=4276
;   sig:  int16_t BattlePlansDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam)
;   params:
;     HWND             hwnd           [BP+0xe]
;     WMType           message        [BP+0xc]
;     uint16_t         wParam         [BP+0xa]
;     int32_t          lParam         [BP+0x6]
;   locals:
;     RECT             rc             [BP-0x14]
;     int16_t          fRet           [BP-0xc]
;     int16_t          i              [BP-0xa]
;     int16_t          idc            [BP-0x8]
;     int16_t() *      lpProc         [BP-0x6]
;     block 001F:113E  len=0x35A
;       int16_t          cLen           [BP-0x16]
;
;   stats: blocks=1  labels=2
;     LSelectName: L_14a5
;     LRename: L_ f9c

L_0652:                             ; battle.c:213
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0018          
PUSH      si                  
PUSH      di                  
                                    ; battle.c:220
MOV       ax, [bp+message]          ; ax, [bp+0xc]
JMP       L_16d5              

L_0661:                             ; battle.c:224
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0d96          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     StickyDlgPos              ; void StickyDlgPos(HWND hwnd, POINT *ppt, int16_t fInit)
ADD       sp, 0x0006          
                                    ; battle.c:225
MOV       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
                                    ; battle.c:226
CMP       [sel+0x4], 0x0002         ; [0x495a], 0x0002
JNZ       L_068d              

L_0684:                             ; battle.c:227
MOV       al, [sel+0x7c]            ; al, [0x49d2]
AND       ax, 0x00ff          
MOV       [iPlanSelDlg], ax         ; [0x0d9a], ax

L_068d:                             ; battle.c:229
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       si, ax              
PUSH      si                  
MOV       di, 0x51d4          
PUSH      ds                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x0012          
MOVSW.REP [btlplan], ds:[si]        ; es:[di], ds:[si]
POP       ds                  
POP       si                  
MOV       ax, 0x51d4          
                                    ; battle.c:231
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_06c5              

L_06c1:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_06c5:
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
AND       ax, 0x00ff          
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_0714              

L_06d8:                             ; battle.c:233
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0024          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
JMP       L_06c1              

L_0714:                             ; battle.c:235
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:236
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041b          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_0753              

L_074d:
MOV       ax, 0x0001          
JMP       L_0756              

L_0753:
MOV       ax, 0x0000          

L_0756:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:237
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0817          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_0779              

L_0773:
MOV       ax, 0x0001          
JMP       L_077c              

L_0779:
MOV       ax, 0x0000          

L_077c:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:238
MOV       [bp-i], 0x0198            ; [bp-0xa], 0x0198
JMP       L_07b7              

L_078a:                             ; battle.c:240
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0421          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_07b7:
CMP       [bp-i], 0x019d            ; [bp-0xa], 0x019d
JLE       L_078a              

L_07c1:                             ; battle.c:241
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0421          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       cx, 0x0008          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:243
MOV       [bp-i], 0x0190            ; [bp-0xa], 0x0190
JMP       L_0820              

L_07f3:                             ; battle.c:245
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041f          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_0820:
CMP       [bp-i], 0x0197            ; [bp-0xa], 0x0197
JLE       L_07f3              

L_082a:                             ; battle.c:246
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041f          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:248
MOV       ax, [game+0x10]           ; ax, [0x0080]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_0946              

L_0861:                             ; battle.c:250
MOV       [bp-i], 0x0078            ; [bp-0xa], 0x0078
JMP       L_0896              

L_0869:                             ; battle.c:251
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_0896:
CMP       [bp-i], 0x007b            ; [bp-0xa], 0x007b
JLE       L_0869              

L_089f:                             ; battle.c:253
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_08ab              

L_08a7:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_08ab:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_0901              

L_08b6:                             ; battle.c:254
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       [bp-i], ax                ; [bp-0xa], ax
JZ        L_08a7              

L_08c1:                             ; battle.c:255
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
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
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)

L_08fe:                             ; battle.c:257
JMP       L_08a7              

L_0901:
MOV       cx, 0x0008          
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, cx              
AND       ax, 0x001f          
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:258
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0004          
CMP       [bp-i], ax                ; [bp-0xa], ax
JL        L_0921              

L_091d:                             ; battle.c:259
SUB       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_0921:                             ; battle.c:260
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xa]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:262
JMP       L_09a8              

L_0946:                             ; battle.c:264
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x007b          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:265
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:266
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)

L_09a8:                             ; battle.c:269
MOV       [bp-i], 0x0190            ; [bp-0xa], 0x0190
JMP       L_09dd              

L_09b0:                             ; battle.c:271
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0420          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_09dd:
CMP       [bp-i], 0x0197            ; [bp-0xa], 0x0197
JLE       L_09b0              

L_09e7:                             ; battle.c:272
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0420          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:274
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041d          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0401          
PUSH      ax                  
MOV       cx, 0x000f          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:275
MOV       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000
                                    ; battle.c:276
MOV       cx, 0x000b          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_0a5c              

L_0a57:                             ; battle.c:277
CALLF     AdvanceTutor              ; void AdvanceTutor()

L_0a5c:                             ; battle.c:278
MOV       ax, 0x0001          
JMP       L_16fe              

L_0a62:                             ; battle.c:282
MOV       [bp-idc], 0x041d          ; [bp-0x8], 0x041d
JMP       L_0a8f              

L_0a6a:                             ; battle.c:283
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+0x8]        
MOV       [bp-0x16], ax       
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [bp-idc]                  ; [bp-0x8]
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
MOV       cx, [bp-0x16]       
CMP       cx, ax              
JZ        L_0a99              

L_0a8b:                             ; battle.c:286
ADD       [bp-idc], 0x0001          ; [bp-0x8], 0x0001

L_0a8f:
CMP       [bp-idc], 0x0422          ; [bp-0x8], 0x0422
JLE       L_0a6a              

L_0a99:
CMP       [bp-idc], 0x041d          ; [bp-0x8], 0x041d
JGE       L_0abf              

L_0aa3:
MOV       ax, [bp+lParam]           ; ax, [bp+0x6]
MOV       dx, [bp+lParam+0x2]       ; dx, [bp+0x8]
MOV       cx, 0x0010          
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0xffff          
AND       dx, 0x0000          
CMP       ax, 0x0006          
JNZ       L_16f8              

L_0abf:                             ; battle.c:288
PUSH      [bp+wParam]               ; [bp+0xa]
PUSH      [crButtonFace+0x2]        ; [0x22bc]
PUSH      [crButtonFace]            ; [0x22ba]
CALLF     SetBkColor                ; COLORREF SetBkColor(HDC arg1, COLORREF arg2)
                                    ; battle.c:289
MOV       ax, [hbrButtonFace]       ; ax, [0x0010]
JMP       L_16fe              

L_0ad8:                             ; battle.c:295
PUSH      [bp+hwnd]                 ; [bp+0xe]
LEA       ax, [bp-rc]               ; ax, [bp-0x14]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     GetClientRect             ; void GetClientRect(HWND arg1, RECT *arg2)
                                    ; battle.c:296
PUSH      [bp+wParam]               ; [bp+0xa]
LEA       ax, [bp-rc]               ; ax, [bp-0x14]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hbrButtonFace]           ; [0x0010]
CALLF     FillRect                  ; int16_t FillRect(HDC arg1, RECT *arg2, HBRUSH arg3)
                                    ; battle.c:297
MOV       ax, 0x0001          
JMP       L_16fe              

L_0b00:                             ; battle.c:300
CMP       [bp+wParam], 0x0001       ; [bp+0xa], 0x0001
JZ        L_0b12              

L_0b09:
CMP       [bp+wParam], 0x0002       ; [bp+0xa], 0x0002
JNZ       L_0bb8              

L_0b12:                             ; battle.c:302
CMP       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000
JZ        L_0b6e              

L_0b1c:                             ; battle.c:304
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:305
MOV       ax, 0x51d4          
PUSH      ax                  
CALLF     LogChangeBtlplan          ; void LogChangeBtlplan(BTLPLAN *pbtlplan)
ADD       sp, 0x0002          

L_0b6e:                             ; battle.c:307
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0d96          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     StickyDlgPos              ; void StickyDlgPos(HWND hwnd, POINT *ppt, int16_t fInit)
ADD       sp, 0x0006          
                                    ; battle.c:308
PUSH      [bp+hwnd]                 ; [bp+0xe]
PUSH      [iPlanSelDlg]             ; [0x0d9a]
CALLF     EndDialog                 ; void EndDialog(HWND arg1, int16_t arg2)
                                    ; battle.c:309
CMP       [sel+0x4], 0x0002         ; [0x495a], 0x0002
JNZ       L_0ba9              

L_0b97:                             ; battle.c:310
MOV       al, [sel+0x7c]            ; al, [0x49d2]
AND       ax, 0x00ff          
ADD       ax, 0x0001          
PUSH      ax                  
CALLF     FillBattleDD              ; void FillBattleDD(int16_t iSel)
ADD       sp, 0x0002          

L_0ba9:                             ; battle.c:311
MOV       [iPlanSelDlg], 0xffff     ; [0x0d9a], 0xffff
                                    ; battle.c:312
MOV       ax, 0x0001          
JMP       L_16fe              

L_0bb8:
CMP       [bp+wParam], 0x041d       ; [bp+0xa], 0x041d
JNZ       L_0c05              

L_0bc2:                             ; battle.c:316
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041d          
PUSH      ax                  
MOV       ax, 0x0400          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendDlgItemMessage        ; LRESULT SendDlgItemMessage(HWND arg1, int16_t arg2, uint16_t arg3, WPARAM arg4, LPARAM arg5)
MOV       [bp-0x16], ax       
MOV       ax, [bp-0x16]       
AND       ax, 0x0001          
MOV       cx, 0x000f          
SHL       ax, cx              
MOV       cx, [btlplan]             ; cx, [0x51d4]
AND       cx, 0x7fff          
OR        cx, ax              
MOV       [btlplan], cx             ; [0x51d4], cx
MOV       ax, cx              
                                    ; battle.c:317
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:319
JMP       L_16f8              

L_0c05:
CMP       [bp+wParam], 0x0817       ; [bp+0xa], 0x0817
JNZ       L_0e25              

L_0c0f:                             ; battle.c:322
CMP       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000
JZ        L_0c71              

L_0c19:                             ; battle.c:324
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:325
MOV       ax, 0x51d4          
PUSH      ax                  
CALLF     LogChangeBtlplan          ; void LogChangeBtlplan(BTLPLAN *pbtlplan)
ADD       sp, 0x0002          
                                    ; battle.c:326
MOV       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000

L_0c71:                             ; battle.c:329
MOV       ax, [btlplan]             ; ax, [0x51d4]
AND       ax, 0xbfff          
OR        ax, 0x4000          
MOV       [btlplan], ax             ; [0x51d4], ax
                                    ; battle.c:330
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:331
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
AND       ax, 0x000f          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [btlplan]             ; cx, [0x51d4]
AND       cx, 0xff0f          
OR        cx, ax              
MOV       [btlplan], cx             ; [0x51d4], cx
                                    ; battle.c:332
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [iPlanSelDlg]             ; [0x0d9a]
CALLF     FDeleteBattlePlan         ; int16_t FDeleteBattlePlan(int16_t iplan, int16_t fWarn)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_0dd0              

L_0cf7:                             ; battle.c:334
MOV       ax, 0x51d4          
PUSH      ax                  
CALLF     LogChangeBtlplan          ; void LogChangeBtlplan(BTLPLAN *pbtlplan)
ADD       sp, 0x0002          
                                    ; battle.c:335
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
ADD       ax, 0xffff          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:336
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040b          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:337
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_0d56              

L_0d52:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_0d56:
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
AND       ax, 0x00ff          
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_0da5              

L_0d69:                             ; battle.c:339
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0024          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
JMP       L_0d52              

L_0da5:                             ; battle.c:340
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
ADD       ax, 0xffff          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:341
JMP       LSelectName         

L_0dd0:                             ; battle.c:345
MOV       ax, [btlplan]             ; ax, [0x51d4]
AND       ax, 0xbfff          
OR        ax, 0x0000          
MOV       [btlplan], ax             ; [0x51d4], ax
                                    ; battle.c:346
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:349
JMP       L_16f8              

L_0e25:
CMP       [bp+wParam], 0x041f       ; [bp+0xa], 0x041f
JNZ       L_0e70              

L_0e2f:                             ; battle.c:351
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0407          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:352
MOV       ax, [bp-i]                ; ax, [bp-0xa]
AND       ax, 0x000f          
MOV       cx, [btlplan+0x2]         ; cx, [0x51d6]
AND       cx, 0xfff0          
OR        cx, ax              
MOV       [btlplan+0x2], cx         ; [0x51d6], cx
                                    ; battle.c:353
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:355
JMP       L_16f8              

L_0e70:
CMP       [bp+wParam], 0x0420       ; [bp+0xa], 0x0420
JNZ       L_0ec4              

L_0e7a:                             ; battle.c:357
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0407          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:358
MOV       ax, [bp-i]                ; ax, [bp-0xa]
AND       ax, 0x000f          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [btlplan+0x2]         ; cx, [0x51d6]
AND       cx, 0xff0f          
OR        cx, ax              
MOV       [btlplan+0x2], cx         ; [0x51d6], cx
                                    ; battle.c:359
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:361
JMP       L_16f8              

L_0ec4:
CMP       [bp+wParam], 0x0422       ; [bp+0xa], 0x0422
JNZ       L_0f41              

L_0ece:                             ; battle.c:363
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0407          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:364
MOV       ax, [game+0x10]           ; ax, [0x0080]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_0f0d              

L_0f05:                             ; battle.c:365
MOV       [bp-i], 0x0003            ; [bp-0xa], 0x0003
                                    ; battle.c:366
JMP       L_0f1f              

L_0f0d:
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0004          
CMP       [bp-i], ax                ; [bp-0xa], ax
JL        L_0f1f              

L_0f1b:                             ; battle.c:367
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_0f1f:                             ; battle.c:368
MOV       ax, [bp-i]                ; ax, [bp-0xa]
AND       ax, 0x001f          
MOV       cx, 0x0008          
SHL       ax, cx              
MOV       cx, [btlplan+0x2]         ; cx, [0x51d6]
AND       cx, 0xe0ff          
OR        cx, ax              
MOV       [btlplan+0x2], cx         ; [0x51d6], cx
                                    ; battle.c:369
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:371
JMP       L_16f8              

L_0f41:
CMP       [bp+wParam], 0x0421       ; [bp+0xa], 0x0421
JNZ       L_0f92              

L_0f4b:                             ; battle.c:373
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, [bp+wParam]           ; ax, [bp+0xa]
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0407          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:374
MOV       ax, [bp-i]                ; ax, [bp-0xa]
AND       ax, 0x000f          
MOV       cx, 0x0008          
SHL       ax, cx              
MOV       cx, [btlplan]             ; cx, [0x51d4]
AND       cx, 0xf0ff          
OR        cx, ax              
MOV       [btlplan], cx             ; [0x51d4], cx
                                    ; battle.c:375
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:377
JMP       L_16f8              

L_0f92:
CMP       [bp+wParam], 0x041b       ; [bp+0xa], 0x041b
JNZ       L_1134              

LRename:                            ; battle.c:380
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0d96          
PUSH      ax                  
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     StickyDlgPos              ; void StickyDlgPos(HWND hwnd, POINT *ppt, int16_t fInit)
ADD       sp, 0x0006          
                                    ; battle.c:381
MOV       ax, 0x04ce          
MOV       dx, 0x0cea          
PUSH      dx                  
PUSH      ax                  
PUSH      [hInst]                   ; [0x5310]
CALLF     MakeProcInstance          ; int32_t() * MakeProcInstance(int32_t (**arg1)(), HINSTANCE arg2)
MOV       [bp-lpProc], ax           ; [bp-0x6], ax
MOV       [bp-lpProc+0x2], dx       ; [bp-0x4], dx
                                    ; battle.c:382
PUSH      [hInst]                   ; [0x5310]
MOV       ax, 0x07e3          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
PUSH      [bp-lpProc+0x2]           ; [bp-0x4]
PUSH      [bp-lpProc]               ; [bp-0x6]
CALLF     DialogBox                 ; int16_t DialogBox(HINSTANCE arg1, DialogId arg2, HWND arg3, BOOL (**arg4)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))
MOV       [bp-fRet], ax             ; [bp-0xc], ax
                                    ; battle.c:383
PUSH      [bp-lpProc+0x2]           ; [bp-0x4]
PUSH      [bp-lpProc]               ; [bp-0x6]
CALLF     FreeProcInstance          ; void FreeProcInstance(int32_t (**arg1)())
                                    ; battle.c:384
PUSH      [bp+hwnd]                 ; [bp+0xe]
CALLF     SetFocus                  ; HWND SetFocus(HWND arg1)
                                    ; battle.c:385
CMP       [bp-fRet], 0x0000         ; [bp-0xc], 0x0000
JZ        L_10e5              

L_1000:                             ; battle.c:388
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:389
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040b          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:390
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_1074              

L_1070:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_1074:
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
AND       ax, 0x00ff          
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_10c3              

L_1087:                             ; battle.c:392
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0024          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
JMP       L_1070              

L_10c3:                             ; battle.c:393
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)

L_10e5:                             ; battle.c:395
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041b          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_1102              

L_10fc:
MOV       ax, 0x0001          
JMP       L_1105              

L_1102:
MOV       ax, 0x0000          

L_1105:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:396
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0817          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_1128              

L_1122:
MOV       ax, 0x0001          
JMP       L_112b              

L_1128:
MOV       ax, 0x0000          

L_112b:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:398
JMP       L_16f8              

L_1134:
CMP       [bp+wParam], 0x041c       ; [bp+0xa], 0x041c
JNZ       L_149b              

L_113e:                             ; battle.c:402
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JNZ       L_1157              

L_1151:                             ; battle.c:403
MOV       ax, 0x0000          
JMP       L_16fe              

L_1157:                             ; battle.c:405
CMP       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000
JZ        L_11b9              

L_1161:                             ; battle.c:407
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x18], ax       
MOV       ax, cx              
MOV       cx, [bp-0x18]       
ADD       ax, cx              
                                    ; battle.c:408
MOV       ax, 0x51d4          
PUSH      ax                  
CALLF     LogChangeBtlplan          ; void LogChangeBtlplan(BTLPLAN *pbtlplan)
ADD       sp, 0x0002          
                                    ; battle.c:409
MOV       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000

L_11b9:                             ; battle.c:412
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
ADD       [bx+0x4830], 0x0001 
AND       ax, 0x00ff          
MOV       [iPlanSelDlg], ax         ; [0x0d9a], ax
                                    ; battle.c:413
MOV       ax, 0x51d8          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
MOV       [bp-cLen], ax             ; [bp-0x16], ax
                                    ; battle.c:415
CMP       [bp-cLen], 0x001b         ; [bp-0x16], 0x001b
JG        L_128b              

L_11e7:                             ; battle.c:421
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xffff          
MOV       bx, 0x51d8          
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
CMP       ax, 0x0029          
JNZ       L_123c              

L_11ff:
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xfffe          
MOV       bx, 0x51d8          
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       bx, 0x175f          
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
AND       ax, 0x0004          
CMP       ax, 0x0000          
JZ        L_123c              

L_1224:
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xfffd          
MOV       bx, 0x51d8          
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
CMP       ax, 0x0028          
JZ        L_1254              

L_123c:                             ; battle.c:422
MOV       ax, 0x0d9c          
PUSH      ax                  
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
MOV       cx, 0x51d8          
ADD       cx, ax              
PUSH      cx                  
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; battle.c:423
JMP       L_128b              

L_1254:
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xfffe          
MOV       bx, 0x51d8          
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
CMP       ax, 0x0039          
JNZ       L_127d              

L_126c:                             ; battle.c:424
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xfffe          
MOV       bx, 0x51d8          
ADD       bx, ax              
MOV       [bx], 0x0030        
                                    ; battle.c:425
JMP       L_128b              

L_127d:                             ; battle.c:426
MOV       ax, [bp-cLen]             ; ax, [bp-0x16]
ADD       ax, 0xfffe          
MOV       bx, 0x51d8          
ADD       bx, ax              
ADD       [bx], 0x0001        

L_128b:                             ; battle.c:428
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
AND       ax, 0x000f          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [btlplan]             ; cx, [0x51d4]
AND       cx, 0xff0f          
OR        cx, ax              
MOV       [btlplan], cx             ; [0x51d4], cx
                                    ; battle.c:429
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x18], ax       
MOV       ax, cx              
MOV       cx, [bp-0x18]       
ADD       ax, cx              
                                    ; battle.c:431
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0421          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       cx, 0x0008          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:432
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040b          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:433
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_1345              

L_1341:
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_1345:
MOV       bx, [idPlayer]            ; bx, [0x018c]
MOV       al, [bx+0x4830]     
AND       ax, 0x00ff          
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       L_1394              

L_1358:                             ; battle.c:435
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0403          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0024          
IMUL      [bp-i]                    ; [bp-0xa]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
ADD       cx, ax              
MOV       ax, 0x0004          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
JMP       L_1341              

L_1394:                             ; battle.c:436
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:437
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041f          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:438
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0420          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:439
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041d          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0401          
PUSH      ax                  
MOV       cx, 0x000f          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:440
MOV       cx, 0x0008          
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, cx              
AND       ax, 0x001f          
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:441
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0004          
CMP       [bp-i], ax                ; [bp-0xa], ax
JL        L_1452              

L_144e:                             ; battle.c:442
SUB       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_1452:                             ; battle.c:443
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xa]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:445
MOV       [fDirtyPlan], 0x0001      ; [0x26b2], 0x0001
                                    ; battle.c:446
MOV       [bp+wParam], 0x041f       ; [bp+0xa], 0x041f
                                    ; battle.c:447
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041b          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:448
JMP       LRename             

L_149b:
CMP       [bp+wParam], 0x041e       ; [bp+0xa], 0x041e
JNZ       L_16a5              

LSelectName:                        ; battle.c:453
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041e          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0407          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:454
MOV       ax, [iPlanSelDlg]         ; ax, [0x0d9a]
CMP       [bp-i], ax                ; [bp-0xa], ax
JZ        L_16f8              

L_14d5:                             ; battle.c:456
CMP       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000
JZ        L_1537              

L_14df:                             ; battle.c:458
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       di, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       di, ax              
MOV       es, cx              
PUSH      di                  
MOV       si, 0x51d4          
MOV       cx, 0x0012          
MOVSW.REP es:[di], [btlplan]        ; es:[di], ds:[si]
POP       di                  
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [0x018c]        
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x5938]     
MOV       dx, [bx+0x593a]     
MOV       [bp-0x16], ax       
MOV       ax, cx              
MOV       cx, [bp-0x16]       
ADD       ax, cx              
                                    ; battle.c:459
MOV       ax, 0x51d4          
PUSH      ax                  
CALLF     LogChangeBtlplan          ; void LogChangeBtlplan(BTLPLAN *pbtlplan)
ADD       sp, 0x0002          
                                    ; battle.c:460
MOV       [fDirtyPlan], 0x0000      ; [0x26b2], 0x0000

L_1537:                             ; battle.c:463
MOV       ax, [bp-i]                ; ax, [bp-0xa]
MOV       [iPlanSelDlg], ax         ; [0x0d9a], ax
                                    ; battle.c:464
MOV       ax, 0x0024          
IMUL      [iPlanSelDlg]             ; [0x0d9a]
MOV       bx, [idPlayer]            ; bx, [0x018c]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       si, [bx+0x5938]     
MOV       cx, [bx+0x593a]     
ADD       si, ax              
PUSH      si                  
MOV       di, 0x51d4          
PUSH      ds                  
POP       es                  
PUSH      ds                  
MOV       ds, cx              
MOV       cx, 0x0012          
MOVSW.REP [btlplan], ds:[si]        ; es:[di], ds:[si]
POP       ds                  
POP       si                  
MOV       ax, 0x51d4          
                                    ; battle.c:465
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041f          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:466
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0420          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:467
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041d          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x0401          
PUSH      ax                  
MOV       cx, 0x000f          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:468
MOV       [bp+wParam], 0x041f       ; [bp+0xa], 0x041f
                                    ; battle.c:469
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x041b          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_1607              

L_1601:
MOV       ax, 0x0001          
JMP       L_160a              

L_1607:
MOV       ax, 0x0000          

L_160a:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:470
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0817          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
CMP       [iPlanSelDlg], 0x0000     ; [0x0d9a], 0x0000
JLE       L_162d              

L_1627:
MOV       ax, 0x0001          
JMP       L_1630              

L_162d:
MOV       ax, 0x0000          

L_1630:
PUSH      ax                  
CALLF     EnableWindow              ; int16_t EnableWindow(HWND arg1, int16_t arg2)
                                    ; battle.c:471
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0421          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       cx, 0x0008          
MOV       ax, [btlplan]             ; ax, [0x51d4]
SHR       ax, cx              
AND       ax, 0x000f          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)
                                    ; battle.c:472
MOV       cx, 0x0008          
MOV       ax, [btlplan+0x2]         ; ax, [0x51d6]
SHR       ax, cx              
AND       ax, 0x001f          
MOV       [bp-i], ax                ; [bp-0xa], ax
                                    ; battle.c:473
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0004          
CMP       [bp-i], ax                ; [bp-0xa], ax
JL        L_1680              

L_167c:                             ; battle.c:474
SUB       [bp-i], 0x0001            ; [bp-0xa], 0x0001

L_1680:                             ; battle.c:475
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, 0x0422          
PUSH      ax                  
CALLF     GetDlgItem                ; HWND GetDlgItem(HWND arg1, ControlId arg2)
PUSH      ax                  
MOV       ax, 0x040e          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xa]
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     SendMessage               ; LRESULT SendMessage(HWND arg1, WMType arg2, WPARAM arg3, LPARAM arg4)

L_16a2:                             ; battle.c:478
JMP       L_16f8              

L_16a5:
CMP       [bp+wParam], 0x0076       ; [bp+0xa], 0x0076
JNZ       L_16f8              

L_16ae:                             ; battle.c:480
PUSH      [bp+hwnd]                 ; [bp+0xe]
MOV       ax, [szHelpFile]          ; ax, [0x014a]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0439          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     WinHelp                   ; int16_t WinHelp(HWND hwndmain, LPCSTR *lpszhelp, uint16_t uscommand, uint32_t uldata)
                                    ; battle.c:481
MOV       ax, 0x0001          
JMP       L_16fe              

L_16d5:
CMP       ax, 0x0014          
JZ        L_0ad8              

L_16dd:
CMP       ax, 0x0019          
JZ        L_0a62              

L_16e5:
CMP       ax, 0x0110          
JZ        L_0661              

L_16ed:
CMP       ax, 0x0111          
JZ        L_0b00              

L_16f8:                             ; battle.c:486
MOV       ax, 0x0000          

L_16fe:                             ; battle.c:487
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF      0x000a              


