; FOpenFile  (file)
;   addr: 000f:4ac2  len=1120
;   sig:  int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md)
;   params:
;     DtFileType       dt             [BP+0x6]
;     int16_t          iPlayer        [BP+0x8]
;     int16_t          md             [BP+0xa]
;   locals:
;     jmp_buf[9]       env            [BP-0x2e]
;     jmp_buf[9] *     penvMemSav     [BP-0x1c]
;     int16_t          fSilentSav     [BP-0x1a]
;     int16_t          fRewind        [BP-0x18]
;     int16_t          fCheckMulti    [BP-0x16]
;     StringId         ids            [BP-0x14]
;     RTBOF            rtbof          [BP-0x12]
;
;   stats: blocks=0  labels=1
;     LBadFile: L_4c36

L_4ac2:                             ; file.c:2196
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0030          
PUSH      si                  
PUSH      di                  
                                    ; file.c:2199
MOV       ax, [fFileErrSilent]      ; ax, [0x074c]
MOV       [bp-fSilentSav], ax       ; [bp-0x1a], ax
                                    ; file.c:2202
MOV       [bp-ids], 0x0004          ; [bp-0x14], 0x0004
                                    ; file.c:2206
MOV       ax, [gd+0x2]              ; ax, [0x07cc]
AND       ax, 0xff7f          
OR        ax, 0x0000          
MOV       [gd+0x2], ax              ; [0x07cc], ax
                                    ; file.c:2208
MOV       ax, [bp+dt]               ; ax, [bp+0x6]
AND       ax, 0x2000          
MOV       [bp-fCheckMulti], ax      ; [bp-0x16], ax
                                    ; file.c:2209
MOV       ax, [bp+dt]               ; ax, [bp+0x6]
AND       ax, 0x1000          
MOV       [bp-fRewind], ax          ; [bp-0x18], ax
                                    ; file.c:2210
AND       [bp+dt], 0x00ff           ; [bp+0x6], 0x00ff
                                    ; file.c:2213
PUSH      [bp+iPlayer]              ; [bp+0x8]
PUSH      [bp+dt]                   ; [bp+0x6]
CALLF     SetSzWorkFromDt           ; void SetSzWorkFromDt(DtFileType dt, int16_t iPlayer)
ADD       sp, 0x0004          
                                    ; file.c:2215
MOV       ax, [penvMem]             ; ax, [0x006e]
MOV       [bp-penvMemSav], ax       ; [bp-0x1c], ax
                                    ; file.c:2216
LEA       ax, [bp-env]              ; ax, [bp-0x2e]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; file.c:2217
LEA       ax, [bp-env]              ; ax, [bp-0x2e]
PUSH      ax                  
CALLF     setjmp                    ; int16_t setjmp(int16_t *env)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_4b49              

L_4b27:                             ; file.c:2219
MOV       ax, [bp-fSilentSav]       ; ax, [bp-0x1a]
MOV       [fFileErrSilent], ax      ; [0x074c], ax
                                    ; file.c:2220
PUSH      [bp-ids]                  ; [bp-0x14]
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2221
CALLF     StreamClose               ; void StreamClose()
                                    ; file.c:2222
MOV       ax, [bp-penvMemSav]       ; ax, [bp-0x1c]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; file.c:2223
MOV       ax, 0x0000          
JMP       L_4f1c              

L_4b49:                             ; file.c:2226
MOV       [fFileErrSilent], 0x0001  ; [0x074c], 0x0001
                                    ; file.c:2227
PUSH      [bp+md]                   ; [bp+0xa]
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     StreamOpen                ; void StreamOpen(char *szFile, int16_t mdOpen)
ADD       sp, 0x0004          
                                    ; file.c:2228
MOV       ax, [bp-fSilentSav]       ; ax, [bp-0x1a]
MOV       [fFileErrSilent], ax      ; [0x074c], ax
                                    ; file.c:2230
MOV       [bp-ids], 0x0003          ; [bp-0x14], 0x0003
                                    ; file.c:2231
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:2233
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0008          
JNZ       L_4bc4              

L_4b81:
MOV       cx, 0x000c          
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, cx              
AND       ax, 0x000f          
CMP       ax, 0x0002          
JNZ       L_4bc4              

L_4b94:
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x007f          
CMP       ax, 0x0031          
JC        L_4bc4              

L_4bac:
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x007f          
CMP       ax, 0x0054          
JC        L_4c47              

L_4bc4:                             ; file.c:2235
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0008          
JNZ       L_4c2a              

L_4bd7:                             ; file.c:2237
MOV       cx, 0x000c          
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, cx              
AND       ax, 0x000f          
CMP       ax, 0x0002          
JA        L_4c15              

L_4bea:
MOV       cx, 0x000c          
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, cx              
AND       ax, 0x000f          
CMP       ax, 0x0002          
JNZ       L_4c1b              

L_4bfd:
MOV       ax, [rgbCur+0x8]          ; ax, [0x4ba0]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x007f          
CMP       ax, 0x0054          
JBE       L_4c1b              

L_4c15:
MOV       ax, 0x02ca          
JMP       L_4c1e              

L_4c1b:
MOV       ax, 0x04d3          

L_4c1e:
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2238
JMP       LBadFile            

L_4c2a:                             ; file.c:2239
MOV       ax, 0x000d          
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          

LBadFile:                           ; file.c:2241
CALLF     StreamClose               ; void StreamClose()
                                    ; file.c:2242
MOV       ax, [bp-penvMemSav]       ; ax, [bp-0x1c]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; file.c:2243
MOV       ax, 0x0000          
JMP       L_4f1c              

L_4c47:                             ; file.c:2246
MOV       si, 0x4b98          
LEA       di, [bp-rtbof]            ; di, [bp-0x12]
PUSH      ss                  
POP       es                  
MOV       cx, 0x0008          
MOVSW.REP es:[di], [rgbCur]         ; es:[di], ds:[si]
LEA       ax, [bp-0x12]       
                                    ; file.c:2249
MOV       cx, 0x000b          
MOV       ax, [bp-rtbof+0xc]        ; ax, [bp-0x6]
SHL       ax, cx              
MOV       cx, 0x000b          
SAR       ax, cx              
CMP       ax, [bp+iPlayer]          ; ax, [bp+0x8]
JZ        L_4c7b              

L_4c6c:                             ; file.c:2251
MOV       ax, 0x0003          
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2252
JMP       LBadFile            

L_4c7b:                             ; file.c:2255
CMP       [game], 0x0000            ; [0x0070], 0x0000
JNZ       L_4c8f              

L_4c85:
CMP       [game+0x2], 0x0000        ; [0x0072], 0x0000
JZ        L_4ebd              

L_4c8f:                             ; file.c:2257
MOV       ax, [game]                ; ax, [0x0070]
MOV       dx, [game+0x2]            ; dx, [0x0072]
CMP       [bp-rtbof+0x4], ax        ; [bp-0xe], ax
JNZ       L_4ca6              

L_4c9e:
CMP       [bp-rtbof+0x6], dx        ; [bp-0xc], dx
JZ        L_4cb8              

L_4ca6:                             ; file.c:2259
MOV       ax, 0x001d          
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2260
JMP       LBadFile            

L_4cb8:
CMP       [bp+dt], 0x0004           ; [bp+0x6], 0x0004
JZ        L_4ea5              

L_4cc1:                             ; file.c:2264
CMP       [bp-fCheckMulti], 0x0000  ; [bp-0x16], 0x0000
JZ        L_4d4d              

L_4cca:
MOV       cx, 0x000a          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_4d4d              

L_4cdd:                             ; file.c:2266
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0xfffc          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
PUSH      [hf]                      ; [0x0a22]
CALLF     lseek                     ; int32_t lseek(int16_t handle, int32_t offset, int16_t origin)
ADD       sp, 0x0008          
                                    ; file.c:2267
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:2268
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0000          
JZ        L_4d1e              

L_4d0d:
MOV       ax, [hdrCur]              ; ax, [0x2696]
AND       ax, 0x03ff          
CMP       ax, 0x0002          
JNZ       LBadFile            

L_4d1e:                             ; file.c:2270
MOV       ax, [rgbCur]              ; ax, [0x4b98]
MOV       [bp-rtbof+0xa], ax        ; [bp-0x8], ax
                                    ; file.c:2271
MOV       cx, 0x000d          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0007          
MOV       [bp-0x30], ax       
MOV       ax, [bp-0x30]       
AND       ax, 0x0007          
MOV       cx, 0x0009          
SHL       ax, cx              
MOV       cx, [game+0x10]           ; cx, [0x0080]
AND       cx, 0xf1ff          
OR        cx, ax              
MOV       [game+0x10], cx           ; [0x0080], cx
MOV       ax, cx              

L_4d4d:                             ; file.c:2273
CMP       [game+0x12], 0x0000       ; [0x0082], 0x0000
JNZ       L_4d95              

L_4d57:
MOV       ax, [bp-rtbof+0xa]        ; ax, [bp-0x8]
CMP       [game+0x12], ax           ; [0x0082], ax
JZ        L_4d95              

L_4d63:                             ; file.c:2275
MOV       ax, [bp-rtbof+0xa]        ; ax, [bp-0x8]
MOV       [game+0x12], ax           ; [0x0082], ax
                                    ; file.c:2276
MOV       cx, 0x000d          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0007          
MOV       [bp-0x30], ax       
MOV       ax, [bp-0x30]       
AND       ax, 0x0007          
MOV       cx, 0x0009          
SHL       ax, cx              
MOV       cx, [game+0x10]           ; cx, [0x0080]
AND       cx, 0xf1ff          
OR        cx, ax              
MOV       [game+0x10], cx           ; [0x0080], cx
MOV       ax, cx              
                                    ; file.c:2278
JMP       L_4ebd              

L_4d95:
MOV       ax, [game+0x12]           ; ax, [0x0082]
CMP       [bp-rtbof+0xa], ax        ; [bp-0x8], ax
JZ        L_4db2              

L_4da0:                             ; file.c:2280
MOV       ax, 0x001c          
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2281
JMP       LBadFile            

L_4db2:
CMP       [bp+dt], 0x0002           ; [bp+0x6], 0x0002
JNZ       L_4e11              

L_4dbb:
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_4e11              

L_4dcf:
MOV       cx, 0x0009          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_4e11              

L_4de2:                             ; file.c:2285
MOV       ax, 0x2024          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0015          
PUSH      ax                  
CALLF     PszFormatIds              ; char * PszFormatIds(StringId ids, int16_t *pParams)
ADD       sp, 0x0006          
PUSH      ax                  
CALLF     AlertSz                   ; int16_t AlertSz(char *sz, MessageBoxType mbType)
ADD       sp, 0x0004          
CMP       ax, 0x0006          
JNZ       LBadFile            

L_4e08:
JMP       L_4ebd              

L_4e11:
MOV       cx, 0x0008          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_4e58              

L_4e24:
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_4e58              

L_4e34:
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_4e58              

L_4e46:                             ; file.c:2290
MOV       ax, [gd+0x2]              ; ax, [0x07cc]
AND       ax, 0xff7f          
OR        ax, 0x0080          
MOV       [gd+0x2], ax              ; [0x07cc], ax
                                    ; file.c:2291
JMP       LBadFile            

L_4e58:
CMP       [bp+dt], 0x0001           ; [bp+0x6], 0x0001
JNZ       L_4ebd              

L_4e61:
MOV       ax, [game+0x10]           ; ax, [0x0080]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_4ebd              

L_4e75:
MOV       cx, 0x000d          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0007          
MOV       cx, 0x0009          
MOV       dx, [game+0x10]           ; dx, [0x0080]
SHR       dx, cx              
AND       dx, 0x0007          
CMP       ax, dx              
JZ        L_4ebd              

L_4e93:                             ; file.c:2295
MOV       ax, 0x001d          
PUSH      ax                  
CALLF     FileError                 ; void FileError(MessageId ids)
ADD       sp, 0x0002          
                                    ; file.c:2296
JMP       LBadFile            

L_4ea5:
MOV       cx, 0x000b          
MOV       ax, [bp-rtbof+0xc]        ; ax, [bp-0x6]
SHL       ax, cx              
MOV       cx, 0x000b          
SAR       ax, cx              
CMP       ax, [bp+iPlayer]          ; ax, [bp+0x8]
JNZ       LBadFile            

L_4ebd:                             ; file.c:2303
CMP       [bp-fRewind], 0x0000      ; [bp-0x18], 0x0000
JZ        L_4ee3              

L_4ec6:                             ; file.c:2305
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [hf]                      ; [0x0a22]
CALLF     lseek                     ; int32_t lseek(int16_t handle, int32_t offset, int16_t origin)
ADD       sp, 0x0008          
                                    ; file.c:2306
CALLF     ReadRt                    ; void ReadRt()

L_4ee3:                             ; file.c:2309
MOV       ax, [bp-penvMemSav]       ; ax, [bp-0x1c]
MOV       [penvMem], ax             ; [0x006e], ax
                                    ; file.c:2310
MOV       ax, [bp-rtbof+0x8]        ; ax, [bp-0xa]
MOV       [wVersFile], ax           ; [0x0750], ax
                                    ; file.c:2311
MOV       cx, 0x000c          
MOV       ax, [bp-rtbof+0xe]        ; ax, [bp-0x4]
SHR       ax, cx              
AND       ax, 0x0001          
MOV       [bp-0x30], ax       
MOV       ax, [bp-0x30]       
AND       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [gd+0x2]              ; cx, [0x07cc]
AND       cx, 0xfffb          
OR        cx, ax              
MOV       [gd+0x2], cx              ; [0x07cc], cx
MOV       ax, cx              
                                    ; file.c:2313
MOV       ax, 0x0001          

L_4f1c:                             ; file.c:2314
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


