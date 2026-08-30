; InitMDIApp  (mdi)
;   addr: 0005:0000  len=907
;   sig:  int16_t InitMDIApp()
;   locals:
;     WNDCLASS         wc             [BP-0x1c]
;
;   stats: blocks=0  labels=0

L_0000:                             ; mdi.c:88
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x001c          
PUSH      si                  
PUSH      di                  
                                    ; mdi.c:92
MOV       [bp-wc], 0x000b           ; [bp-0x1c], 0x000b
                                    ; mdi.c:93
MOV       [bp-wc+0x2], 0x06d6       ; [bp-0x1a], 0x06d6
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:94
MOV       [bp-wc+0x6], 0x0000       ; [bp-0x16], 0x0000
                                    ; mdi.c:95
MOV       [bp-wc+0x8], 0x0000       ; [bp-0x14], 0x0000
                                    ; mdi.c:96
MOV       ax, [hInst]               ; ax, [0x5310]
MOV       [bp-wc+0xa], ax           ; [bp-0x12], ax
                                    ; mdi.c:97
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:98
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-wc+0xe], ax           ; [bp-0xe], ax
                                    ; mdi.c:99
MOV       [bp-wc+0x10], 0x000d      ; [bp-0xc], 0x000d
                                    ; mdi.c:100
MOV       ax, 0x0364          
MOV       dx, ds              
MOV       [bp-wc+0x12], ax          ; [bp-0xa], ax
MOV       [bp-wc+0x14], dx          ; [bp-0x8], dx
                                    ; mdi.c:101
MOV       ax, 0x01d6          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:103
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0076              

L_0070:                             ; mdi.c:104
MOV       ax, 0x0000          
JMP       L_0385              

L_0076:                             ; mdi.c:107
MOV       [bp-wc], 0x020b           ; [bp-0x1c], 0x020b
                                    ; mdi.c:108
MOV       [bp-wc+0x2], 0x5c92       ; [bp-0x1a], 0x5c92
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:109
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:110
MOV       [bp-wc+0x12], 0x0000      ; [bp-0xa], 0x0000
MOV       [bp-wc+0x14], 0x0000      ; [bp-0x8], 0x0000
                                    ; mdi.c:111
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:112
MOV       ax, 0x0202          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:114
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_00c5              

L_00bf:                             ; mdi.c:115
MOV       ax, 0x0000          
JMP       L_0385              

L_00c5:                             ; mdi.c:118
MOV       [bp-wc], 0x020b           ; [bp-0x1c], 0x020b
                                    ; mdi.c:119
MOV       [bp-wc+0x2], 0x0032       ; [bp-0x1a], 0x0032
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:120
MOV       ax, 0x0004          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:121
MOV       ax, 0x01e2          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:123
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0105              

L_00ff:                             ; mdi.c:124
MOV       ax, 0x0000          
JMP       L_0385              

L_0105:                             ; mdi.c:127
MOV       [bp-wc], 0x020b           ; [bp-0x1c], 0x020b
                                    ; mdi.c:128
MOV       [bp-wc+0x2], 0x0000       ; [bp-0x1a], 0x0000
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:129
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:130
MOV       ax, 0x01ec          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:132
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0145              

L_013f:                             ; mdi.c:133
MOV       ax, 0x0000          
JMP       L_0385              

L_0145:                             ; mdi.c:136
MOV       [bp-wc], 0x0208           ; [bp-0x1c], 0x0208
                                    ; mdi.c:137
MOV       [bp-wc+0x2], 0x001e       ; [bp-0x1a], 0x001e
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:138
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:139
MOV       ax, 0x0242          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:141
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0185              

L_017f:                             ; mdi.c:142
MOV       ax, 0x0000          
JMP       L_0385              

L_0185:                             ; mdi.c:145
MOV       [bp-wc], 0x0200           ; [bp-0x1c], 0x0200
                                    ; mdi.c:146
MOV       [bp-wc+0x2], 0x0000       ; [bp-0x1a], 0x0000
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:147
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:148
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:149
MOV       ax, 0x01f6          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:151
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_01ca              

L_01c4:                             ; mdi.c:152
MOV       ax, 0x0000          
JMP       L_0385              

L_01ca:                             ; mdi.c:155
MOV       [bp-wc], 0x0a00           ; [bp-0x1c], 0x0a00
                                    ; mdi.c:156
MOV       [bp-wc+0x2], 0x0000       ; [bp-0x1a], 0x0000
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:157
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:158
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:159
MOV       ax, 0x0210          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:161
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_020f              

L_0209:                             ; mdi.c:162
MOV       ax, 0x0000          
JMP       L_0385              

L_020f:                             ; mdi.c:165
MOV       [bp-wc], 0x0a00           ; [bp-0x1c], 0x0a00
                                    ; mdi.c:166
MOV       [bp-wc+0x2], 0x19e4       ; [bp-0x1a], 0x19e4
MOV       [bp-wc+0x4], 0x0152       ; [bp-0x18], 0x0152
                                    ; mdi.c:167
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:168
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:169
MOV       ax, 0x024a          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:171
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0254              

L_024e:                             ; mdi.c:172
MOV       ax, 0x0000          
JMP       L_0385              

L_0254:                             ; mdi.c:175
MOV       [bp-wc], 0x0200           ; [bp-0x1c], 0x0200
                                    ; mdi.c:176
MOV       [bp-wc+0x2], 0x2876       ; [bp-0x1a], 0x2876
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:177
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:178
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:179
MOV       ax, 0x021c          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:181
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_0299              

L_0293:                             ; mdi.c:182
MOV       ax, 0x0000          
JMP       L_0385              

L_0299:                             ; mdi.c:185
MOV       [bp-wc], 0x0000           ; [bp-0x1c], 0x0000
                                    ; mdi.c:186
MOV       [bp-wc+0x2], 0x9126       ; [bp-0x1a], 0x9126
MOV       [bp-wc+0x4], 0x0016       ; [bp-0x18], 0x0016
                                    ; mdi.c:187
MOV       [bp-wc+0x6], 0x0000       ; [bp-0x16], 0x0000
                                    ; mdi.c:188
MOV       [bp-wc+0x8], 0x0000       ; [bp-0x14], 0x0000
                                    ; mdi.c:189
MOV       ax, [hInst]               ; ax, [0x5310]
MOV       [bp-wc+0xa], ax           ; [bp-0x12], ax
                                    ; mdi.c:190
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:191
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-wc+0xe], ax           ; [bp-0xe], ax
                                    ; mdi.c:192
MOV       ax, 0x0004          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:193
MOV       [bp-wc+0x12], 0x0000      ; [bp-0xa], 0x0000
MOV       [bp-wc+0x14], 0x0000      ; [bp-0x8], 0x0000
                                    ; mdi.c:194
MOV       ax, 0x022a          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:196
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_030c              

L_0306:                             ; mdi.c:197
MOV       ax, 0x0000          
JMP       L_0385              

L_030c:                             ; mdi.c:200
MOV       [bp-wc], 0x000b           ; [bp-0x1c], 0x000b
                                    ; mdi.c:201
MOV       [bp-wc+0x2], 0x0018       ; [bp-0x1a], 0x0018
MOV       [bp-wc+0x4], 0xffff       ; [bp-0x18], 0xffff
                                    ; mdi.c:202
MOV       [bp-wc+0x6], 0x0000       ; [bp-0x16], 0x0000
                                    ; mdi.c:203
MOV       [bp-wc+0x8], 0x0000       ; [bp-0x14], 0x0000
                                    ; mdi.c:204
MOV       ax, [hInst]               ; ax, [0x5310]
MOV       [bp-wc+0xa], ax           ; [bp-0x12], ax
                                    ; mdi.c:205
MOV       [bp-wc+0xc], 0x0000       ; [bp-0x10], 0x0000
                                    ; mdi.c:206
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x7f00          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     LoadCursor                ; HCURSOR LoadCursor(HINSTANCE arg1, LPCSTR *arg2)
MOV       [bp-wc+0xe], ax           ; [bp-0xe], ax
                                    ; mdi.c:207
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     GetStockObject            ; HGDIOBJ GetStockObject(StockObjectId arg1)
MOV       [bp-wc+0x10], ax          ; [bp-0xc], ax
                                    ; mdi.c:208
MOV       [bp-wc+0x12], 0x0000      ; [bp-0xa], 0x0000
MOV       [bp-wc+0x14], 0x0000      ; [bp-0x8], 0x0000
                                    ; mdi.c:209
MOV       ax, 0x0236          
MOV       dx, ds              
MOV       [bp-wc+0x16], ax          ; [bp-0x6], ax
MOV       [bp-wc+0x18], dx          ; [bp-0x4], dx
                                    ; mdi.c:211
LEA       ax, [bp-wc]               ; ax, [bp-0x1c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     RegisterClass             ; uint16_t_ATOM RegisterClass(WNDCLASS *arg1)
CMP       ax, 0x0000          
JNZ       L_037f              

L_0379:                             ; mdi.c:212
MOV       ax, 0x0000          
JMP       L_0385              

L_037f:                             ; mdi.c:214
MOV       ax, 0x0001          

L_0385:                             ; mdi.c:215
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


