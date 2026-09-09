; MineClick  (mine)
;   addr: 0006:3b4e  len=3213
;   sig:  void MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks)
;   params:
;     int16_t          x              [BP+0x6]
;     int16_t          y              [BP+0x8]
;     int16_t          msg            [BP+0xa]
;     int16_t          sks            [BP+0xc]
;   locals:
;     HtMineType       ht             [BP-0x8]
;     PLANET *         lppl           [BP-0x6]
;     block 0006:3B9E  len=0x1C
;       PLANET *         lppl           [BP-0xc]
;     block 0006:3C2A  len=0x72
;       PART             part           [BP-0x10]
;     block 0006:3CA9  len=0x18C
;       int16_t          ishdef         [BP-0x250]
;       int16_t[16]      rgid           [BP-0x24e]
;       FLEET *          lpfl           [BP-0x22e]
;       int16_t          c              [BP-0x22a]
;       char *[16]       rgpsz          [BP-0x228]
;       char[32][16]     rgsz           [BP-0x208]
;     block 0006:3F41  len=0x141
;       int16_t[3]       rgMax          [BP-0x52]
;       int16_t[3]       rgCost         [BP-0x4c]
;       PLANET           pl             [BP-0x46]
;       int16_t[3]       rgMin          [BP-0xe]
;     block 0006:40A4  len=0x395
;       SCAN             scan           [BP-0x26]
;       int16_t          idNew          [BP-0x16]
;       int16_t          i              [BP-0x14]
;       FLEET *          lpfl           [BP-0x12]
;       int16_t          fOurs          [BP-0xe]
;       PLANET *         lppl           [BP-0xc]
;     block 0006:443C  len=0x130
;       char *[9]        psz            [BP-0x8a]
;       int16_t          iChecked       [BP-0x78]
;       int16_t[9]       rgi            [BP-0x76]
;       int16_t          i              [BP-0x64]
;       char[10][9]      rgsz           [BP-0x62]
;     block 0006:456F  len=0x234
;       int16_t          i              [BP-0x4e]
;       PLANET           pl             [BP-0x4c]
;       int32_t[3]       rglQuan        [BP-0x14]
;       block 0006:4608  len=0x186
;         int32_t          lVal           [BP-0x68]
;         int32_t          cMines         [BP-0x64]
;         FLEET *          lpfl           [BP-0x60]
;         int16_t          ifl            [BP-0x5c]
;         int32_t[3]       rglT           [BP-0x5a]
;
;   stats: blocks=8  labels=5
;     NoTerra: L_4010
;     CheckThing: L_40cb
;     ChangeIt: L_43d9
;     CheckPlanet: L_429f
;     CheckFleet: L_42f3

L_3b4e:                             ; mine.c:1206
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0250          
PUSH      si                  
PUSH      di                  
                                    ; mine.c:1210
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     HtMineWindow              ; HtMineType HtMineWindow(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
MOV       [bp-ht], ax               ; [bp-0x8], ax
                                    ; mine.c:1211
CMP       [bp+msg], 0x0204          ; [bp+0xa], 0x0204
JNZ       L_3b8b              

L_3b76:
CMP       [bp-ht], 0x0009           ; [bp-0x8], 0x0009
JZ        L_3b8b              

L_3b7f:
CMP       [bp-ht], 0x000b           ; [bp-0x8], 0x000b
JNZ       L_47d5              

L_3b8b:                             ; mine.c:1216
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
JMP       L_47a6              

L_3b94:                             ; mine.c:1223
CMP       [sel+0x10], 0x0001        ; [0x4966], 0x0001
JNZ       L_3bbd              

L_3b9e:                             ; mine.c:1225
PUSH      [sel+0x14]                ; [0x496a]
CALLF     LpplFromId                ; PLANET * LpplFromId(int16_t idPlanet)
ADD       sp, 0x0002          
MOV       [bp-lppl], ax             ; [bp-0xc], ax
MOV       [bp-lppl+0x2], dx         ; [bp-0xa], dx
                                    ; mine.c:1226
LES       bx, [bp-lppl]             ; bx, [bp-0xc]
MOV       ax, es:[bx+0x2]     
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
                                    ; mine.c:1228
JMP       L_3c0f              

L_3bbd:
CMP       [sel+0x10], 0x0008        ; [0x4966], 0x0008
JNZ       L_3beb              

L_3bc7:                             ; mine.c:1231
MOV       ax, 0x0012          
IMUL      [sel+0x1a]                ; [0x4970]
MOV       bx, [lpThings]            ; bx, [0x1698]
MOV       cx, [lpThings+0x2]        ; cx, [0x169a]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x000f          
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
                                    ; mine.c:1233
JMP       L_3c0f              

L_3beb:                             ; mine.c:1236
MOV       ax, [sel+0x16]            ; ax, [0x496c]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
LES       bx, es:[bx]         
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x000f          
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax

L_3c0f:                             ; mine.c:1238
MOV       [GlobalPD], 0x0002        ; [0x0b80], 0x0002
                                    ; mine.c:1239
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1240
JMP       L_47d5              

L_3c2a:                             ; mine.c:1248
MOV       ax, 0x0012          
IMUL      [sel+0x1a]                ; [0x4970]
MOV       bx, [lpThings]            ; bx, [0x1698]
MOV       cx, [lpThings+0x2]        ; cx, [0x169a]
ADD       bx, ax              
MOV       es, cx              
MOV       al, es:[bx+0xc]     
AND       ax, 0x00ff          
MOV       bx, ax              
MOV       al, [bx+0x50a]      
AND       ax, 0x00ff          
MOV       [bp-0x12], ax       
MOV       ax, [bp-0x12]       
AND       ax, 0x00ff          
MOV       cx, [bp-part+0x2]         ; cx, [bp-0xe]
AND       cx, 0xff00          
OR        cx, ax              
MOV       [bp-part+0x2], cx         ; [bp-0xe], cx
MOV       ax, cx              
                                    ; mine.c:1249
MOV       [bp-part], 0x0100         ; [bp-0x10], 0x0100
                                    ; mine.c:1250
LEA       ax, [bp-part]             ; ax, [bp-0x10]
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
                                    ; mine.c:1251
MOV       [GlobalPD], 0x0009        ; [0x0b80], 0x0009
                                    ; mine.c:1252
LEA       si, [bp-part]             ; si, [bp-0x10]
MOV       di, 0x0b82          
PUSH      ds                  
POP       es                  
MOVSW     [GlobalPD+0x2], ds:[si]   ; es:[di], ds:[si]
MOVSW     [GlobalPD+0x4], ds:[si]   ; es:[di], ds:[si]
MOVSW     [GlobalPD+0x6], ds:[si]   ; es:[di], ds:[si]
MOVSW     [GlobalPD+0x8], ds:[si]   ; es:[di], ds:[si]
MOV       ax, 0x0b82          
                                    ; mine.c:1253
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1255
JMP       L_47d5              

L_3c9f:                             ; mine.c:1259
CMP       [bp+msg], 0x0204          ; [bp+0xa], 0x0204
JNZ       L_3e38              

L_3ca9:                             ; mine.c:1262
MOV       ax, [sel+0x16]            ; ax, [0x496c]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0x22e], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x22c], dx
                                    ; mine.c:1266
MOV       [bp-c], 0x0000            ; [bp-0x22a], 0x0000
                                    ; mine.c:1268
MOV       [bp-ishdef], 0x0000       ; [bp-0x250], 0x0000
JMP       L_3d7c              

L_3cda:                             ; mine.c:1270
MOV       ax, 0x000c          
MOV       bx, [bp-lpfl]             ; bx, [bp-0x22e]
MOV       cx, [bp-lpfl+0x2]         ; cx, [bp-0x22c]
ADD       bx, ax              
MOV       ax, [bp-ishdef]           ; ax, [bp-0x250]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JLE       L_3d77              

L_3cfa:                             ; mine.c:1272
MOV       ax, [bp-c]                ; ax, [bp-0x22a]
SHL       ax, 0x0001          
LEA       bx, [bp-0x24e]      
ADD       bx, ax              
MOV       ax, [bp-ishdef]           ; ax, [bp-0x250]
MOV       [bx], ax            
                                    ; mine.c:1273
MOV       ax, 0x0093          
IMUL      [bp-ishdef]               ; [bp-0x250]
LES       bx, [bp-lpfl]             ; bx, [bp-0x22e]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
MOV       ax, 0x0008          
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-c]                ; ax, [bp-0x22a]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       cx, [bp-rgsz]             ; cx, [bp-0x208]
ADD       cx, ax              
MOV       dx, ss              
PUSH      dx                  
PUSH      cx                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; mine.c:1274
MOV       ax, [bp-c]                ; ax, [bp-0x22a]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       cx, [bp-rgsz]             ; cx, [bp-0x208]
ADD       cx, ax              
MOV       ax, [bp-c]                ; ax, [bp-0x22a]
SHL       ax, 0x0001          
LEA       bx, [bp-0x228]      
ADD       bx, ax              
MOV       [bx], cx            
                                    ; mine.c:1275
ADD       [bp-c], 0x0001            ; [bp-0x22a], 0x0001

L_3d77:                             ; mine.c:1277
ADD       [bp-ishdef], 0x0001       ; [bp-0x250], 0x0001

L_3d7c:
CMP       [bp-ishdef], 0x0010       ; [bp-0x250], 0x0010
JL        L_3cda              

L_3d86:                             ; mine.c:1279
CMP       [bp-c], 0x0001            ; [bp-0x22a], 0x0001
JLE       L_3dcb              

L_3d90:                             ; mine.c:1281
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0xffff          
PUSH      ax                  
LEA       ax, [bp-rgpsz]            ; ax, [bp-0x228]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x22a]
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-c], ax                ; [bp-0x22a], ax
                                    ; mine.c:1282
CMP       [bp-c], 0xffff            ; [bp-0x22a], 0xffff
JZ        L_47d5              

L_3dc2:
JMP       L_3dd1              

L_3dcb:                             ; mine.c:1286
MOV       [bp-c], 0x0000            ; [bp-0x22a], 0x0000

L_3dd1:                             ; mine.c:1288
MOV       [GlobalPD], 0x000b        ; [0x0b80], 0x000b
                                    ; mine.c:1289
MOV       ax, [bp-c]                ; ax, [bp-0x22a]
SHL       ax, 0x0001          
LEA       bx, [bp-0x24e]      
ADD       bx, ax              
MOV       ax, 0x0093          
IMUL      [bx]                
LES       bx, [bp-lpfl]             ; bx, [bp-0x22e]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
MOV       [GlobalPD+0x2], cx        ; [0x0b82], cx
MOV       [GlobalPD+0x4], dx        ; [0x0b84], dx
                                    ; mine.c:1290
LES       bx, [bp-lpfl]             ; bx, [bp-0x22e]
MOV       ax, es:[bx+0x2]     
CMP       [idPlayer], ax            ; [0x018c], ax
JZ        L_3e1d              

L_3e17:
MOV       ax, 0x0001          
JMP       L_3e20              

L_3e1d:
MOV       ax, 0x0000          

L_3e20:
MOV       [GlobalPD+0x8], ax        ; [0x0b88], ax
                                    ; mine.c:1291
MOV       [GlobalPD+0x6], 0x0000    ; [0x0b86], 0x0000
                                    ; mine.c:1292
MOV       [GlobalPD+0xa], 0x0000    ; [0x0b8a], 0x0000
                                    ; mine.c:1293
MOV       [GlobalPD+0xc], 0x0001    ; [0x0b8c], 0x0001
                                    ; mine.c:1295
JMP       L_3e84              

L_3e38:                             ; mine.c:1297
MOV       [GlobalPD], 0x0003        ; [0x0b80], 0x0003
                                    ; mine.c:1298
MOV       ax, [sel+0x16]            ; ax, [0x496c]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
MOV       [GlobalPD+0x4], dx        ; [0x0b84], dx
                                    ; mine.c:1299
LES       bx, [GlobalPD+0x2]        ; bx, [0x0b82]
MOV       ax, es:[bx+0x4]     
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_3e78              

L_3e72:
MOV       ax, 0x0001          
JMP       L_3e7b              

L_3e78:
MOV       ax, 0x0000          

L_3e7b:
MOV       [GlobalPD+0x6], ax        ; [0x0b86], ax
                                    ; mine.c:1300
MOV       [GlobalPD+0xa], 0x00ff    ; [0x0b8a], 0x00ff

L_3e84:                             ; mine.c:1303
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1304
JMP       L_47d5              

L_3e99:                             ; mine.c:1309
MOV       [GlobalPD], 0x000b        ; [0x0b80], 0x000b
                                    ; mine.c:1310
PUSH      [sel+0x14]                ; [0x496a]
CALLF     LpplFromId                ; PLANET * LpplFromId(int16_t idPlanet)
ADD       sp, 0x0002          
MOV       [bp-lppl], ax             ; [bp-0x6], ax
MOV       [bp-lppl+0x2], dx         ; [bp-0x4], dx
                                    ; mine.c:1311
LES       bx, [bp-lppl]             ; bx, [bp-0x6]
MOV       ax, es:[bx+0x2c]    
AND       ax, 0x000f          
MOV       cx, 0x0093          
IMUL      cx                  
LES       bx, [bp-lppl]             ; bx, [bp-0x6]
MOV       bx, es:[bx+0x2]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14c]      
MOV       dx, [bx+0x14e]      
ADD       cx, ax              
MOV       [GlobalPD+0x2], cx        ; [0x0b82], cx
MOV       [GlobalPD+0x4], dx        ; [0x0b84], dx
                                    ; mine.c:1312
LES       bx, [bp-lppl]             ; bx, [bp-0x6]
MOV       ax, es:[bx+0x2]     
CMP       [idPlayer], ax            ; [0x018c], ax
JZ        L_3ef3              

L_3eed:
MOV       ax, 0x0001          
JMP       L_3ef6              

L_3ef3:
MOV       ax, 0x0000          

L_3ef6:
MOV       [GlobalPD+0x8], ax        ; [0x0b88], ax
                                    ; mine.c:1313
MOV       [GlobalPD+0x6], 0x0001    ; [0x0b86], 0x0001
                                    ; mine.c:1314
MOV       [GlobalPD+0xa], 0x0000    ; [0x0b8a], 0x0000
                                    ; mine.c:1315
MOV       [GlobalPD+0xc], 0x0001    ; [0x0b8c], 0x0001
                                    ; mine.c:1316
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1318
JMP       L_47d5              

L_3f20:                             ; mine.c:1322
MOV       [GlobalPD], 0x0007        ; [0x0b80], 0x0007
                                    ; mine.c:1323
MOV       ax, [sel+0x14]            ; ax, [0x496a]
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
                                    ; mine.c:1324
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1325
JMP       L_47d5              

L_3f41:                             ; mine.c:1334
LEA       ax, [bp-pl]               ; ax, [bp-0x46]
PUSH      ax                  
PUSH      [sel+0x14]                ; [0x496a]
CALLF     FLookupPlanet             ; int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl)
ADD       sp, 0x0004          
                                    ; mine.c:1336
MOV       [GlobalPD], 0x0005        ; [0x0b80], 0x0005
                                    ; mine.c:1337
MOV       ax, [bp-pl]               ; ax, [bp-0x46]
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
                                    ; mine.c:1338
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xfffa          
MOV       [GlobalPD+0x4], ax        ; [0x0b84], ax
                                    ; mine.c:1339
MOV       ax, [bp-pl+0x4]           ; ax, [bp-0x42]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JC        L_3f85              

L_3f74:                             ; mine.c:1340
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
LEA       bx, [bp-0x3a]       
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
MOV       [GlobalPD+0x6], ax        ; [0x0b86], ax
                                    ; mine.c:1341
JMP       L_3f8b              

L_3f85:                             ; mine.c:1342
MOV       [GlobalPD+0x6], 0xffff    ; [0x0b86], 0xffff

L_3f8b:                             ; mine.c:1345
MOV       ax, [bp-pl+0x4]           ; ax, [bp-0x42]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JC        NoTerra             

L_3f99:
MOV       ax, 0x0001          
PUSH      ax                  
LEA       ax, [bp-rgCost]           ; ax, [bp-0x4c]
PUSH      ax                  
LEA       ax, [bp-rgMax]            ; ax, [bp-0x52]
PUSH      ax                  
LEA       ax, [bp-rgMin]            ; ax, [bp-0xe]
PUSH      ax                  
LEA       ax, [bp-pl]               ; ax, [bp-0x46]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     FCanTerraformLppl         ; int16_t FCanTerraformLppl(PLANET *lppl, int16_t *rgEnvMin, int16_t *rgEnvMax, int16_t *rgEnvCost, int16_t fHelp)
ADD       sp, 0x000c          
CMP       ax, 0x0000          
JZ        NoTerra             

L_3fc0:                             ; mine.c:1347
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
SHL       ax, 0x0001          
LEA       bx, [bp-0xe]        
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [GlobalPD+0x8], ax        ; [0x0b88], ax
                                    ; mine.c:1348
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
SHL       ax, 0x0001          
LEA       bx, [bp-0x52]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [GlobalPD+0xa], ax        ; [0x0b8a], ax
                                    ; mine.c:1349
CMP       [GlobalPD+0x8], 0xffff    ; [0x0b88], 0xffff
JNZ       L_3fee              

L_3fe8:                             ; mine.c:1350
MOV       ax, [GlobalPD+0x6]        ; ax, [0x0b86]
MOV       [GlobalPD+0x8], ax        ; [0x0b88], ax

L_3fee:                             ; mine.c:1351
CMP       [GlobalPD+0xa], 0xffff    ; [0x0b8a], 0xffff
JNZ       L_3ffe              

L_3ff8:                             ; mine.c:1352
MOV       ax, [GlobalPD+0x6]        ; ax, [0x0b86]
MOV       [GlobalPD+0xa], ax        ; [0x0b8a], ax

L_3ffe:                             ; mine.c:1353
MOV       ax, [GlobalPD+0xa]        ; ax, [0x0b8a]
CMP       [GlobalPD+0x8], ax        ; [0x0b88], ax
JNZ       L_401c              

NoTerra:                            ; mine.c:1359
MOV       [GlobalPD+0x8], 0xffff    ; [0x0b88], 0xffff
                                    ; mine.c:1360
MOV       [GlobalPD+0xa], 0xffff    ; [0x0b8a], 0xffff

L_401c:                             ; mine.c:1362
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, 0x0010          
ADD       bx, ax              
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
MOV       [GlobalPD+0xc], ax        ; [0x0b8c], ax
                                    ; mine.c:1363
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, 0x0013          
ADD       bx, ax              
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
MOV       [GlobalPD+0xe], ax        ; [0x0b8e], ax
                                    ; mine.c:1364
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, 0x0016          
ADD       bx, ax              
MOV       ax, [GlobalPD+0x4]        ; ax, [0x0b84]
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
MOV       [GlobalPD+0x10], ax       ; [0x0b90], ax
                                    ; mine.c:1365
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1367
JMP       L_47d5              

L_4085:                             ; mine.c:1370
CMP       [bp+msg], 0x0204          ; [bp+0xa], 0x0204
JNZ       L_40a4              

L_408f:                             ; mine.c:1371
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     PopupMineralScanChoices   ; void PopupMineralScanChoices(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1372
JMP       L_47d5              

L_40a4:                             ; mine.c:1379
MOV       si, 0x4962          
LEA       di, [bp-scan]             ; di, [bp-0x26]
PUSH      ss                  
POP       es                  
MOV       cx, 0x0008          
MOVSW.REP es:[di], [sel+0xc]        ; es:[di], ds:[si]
LEA       ax, [bp-0x26]       
                                    ; mine.c:1381
MOV       [bp-scan+0xc], 0x0000     ; [bp-0x1a], 0x0000
                                    ; mine.c:1383
CMP       [bp-scan+0x4], 0x0008     ; [bp-0x22], 0x0008
JNZ       L_4192              

L_40c2:                             ; mine.c:1385
MOV       ax, [bp-scan+0xe]         ; ax, [bp-0x18]
ADD       ax, 0x0001          
MOV       [bp-i], ax                ; [bp-0x14], ax

CheckThing:                         ; mine.c:1387
JMP       L_4113              

L_40ce:                             ; mine.c:1388
LEA       bx, [bp-scan]             ; bx, [bp-0x26]
MOV       cx, ss              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-0x2a], ax       
MOV       [bp-0x28], dx       
MOV       ax, 0x0012          
IMUL      [bp-i]                    ; [bp-0x14]
MOV       bx, [lpThings]            ; bx, [0x1698]
MOV       cx, [lpThings+0x2]        ; cx, [0x169a]
ADD       bx, ax              
MOV       ax, [bp-0x2a]       
MOV       dx, [bp-0x28]       
MOV       es, cx              
CMP       es:[bx+0x2], ax     
JNZ       L_410f              

L_4103:
CMP       es:[bx+0x4], dx     
JZ        L_411e              

L_410f:                             ; mine.c:1390
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001

L_4113:
MOV       ax, [cThing]              ; ax, [0x169c]
CMP       [bp-i], ax                ; [bp-0x14], ax
JL        L_40ce              

L_411e:
MOV       ax, [cThing]              ; ax, [0x169c]
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_4154              

L_4129:                             ; mine.c:1392
MOV       ax, [bp-i]                ; ax, [bp-0x14]
MOV       [bp-scan+0xe], ax         ; [bp-0x18], ax
                                    ; mine.c:1393
MOV       [bp-scan+0x4], 0x0008     ; [bp-0x22], 0x0008
                                    ; mine.c:1394
MOV       ax, 0x0012          
IMUL      [bp-i]                    ; [bp-0x14]
MOV       bx, [lpThings]            ; bx, [0x1698]
MOV       cx, [lpThings+0x2]        ; cx, [0x169a]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       [bp-idNew], ax            ; [bp-0x16], ax
                                    ; mine.c:1395
MOV       [bp-fOurs], 0x0000        ; [bp-0xe], 0x0000
                                    ; mine.c:1396
JMP       ChangeIt            

L_4154:                             ; mine.c:1398
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       CheckPlanet         

L_4168:
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0002          
CMP       ax, 0x0000          
JNZ       CheckFleet          

L_417c:
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0008          
CMP       ax, 0x0000          
JZ        L_4192              

L_418a:                             ; mine.c:1404
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
                                    ; mine.c:1405
JMP       L_4113              

L_4192:                             ; mine.c:1409
CMP       [bp-scan+0x4], 0x0002     ; [bp-0x22], 0x0002
JNZ       L_41a4              

L_419b:
MOV       ax, [bp-scan+0xa]         ; ax, [bp-0x1c]
ADD       ax, 0x0001          
JMP       L_41a7              

L_41a4:
MOV       ax, 0x0000          

L_41a7:
MOV       [bp-i], ax                ; [bp-0x14], ax
                                    ; mine.c:1410
JMP       L_41b1              

L_41ad:
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001

L_41b1:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_4215              

L_41bc:
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0x12], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x10], dx
CMP       ax, 0x0000          
JNZ       L_41ec              

L_41e4:
CMP       dx, 0x0000          
JZ        L_4215              

L_41ec:                             ; mine.c:1411
LES       bx, [bp-lpfl]             ; bx, [bp-0x12]
MOV       ax, es:[bx+0x8]     
MOV       dx, es:[bx+0xa]     
LEA       bx, [bp-scan]             ; bx, [bp-0x26]
MOV       cx, ss              
MOV       es, cx              
CMP       es:[bx], ax         
JNZ       L_41ad              

L_4206:
CMP       es:[bx+0x2], dx     
JNZ       L_41ad              

L_4215:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_4278              

L_4220:                             ; mine.c:1415
MOV       ax, [bp-i]                ; ax, [bp-0x14]
MOV       [bp-scan+0xa], ax         ; [bp-0x1c], ax
                                    ; mine.c:1416
MOV       [bp-scan+0x4], 0x0002     ; [bp-0x22], 0x0002
                                    ; mine.c:1417
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
LES       bx, es:[bx]         
MOV       ax, es:[bx]         
MOV       [bp-idNew], ax            ; [bp-0x16], ax
                                    ; mine.c:1418
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
LES       bx, es:[bx]         
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       es:[bx+0x2], ax     
JNZ       L_426f              

L_4269:
MOV       ax, 0x0001          
JMP       L_4272              

L_426f:
MOV       ax, 0x0000          

L_4272:
MOV       [bp-fOurs], ax            ; [bp-0xe], ax
                                    ; mine.c:1420
JMP       ChangeIt            

L_4278:
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0008          
CMP       ax, 0x0000          
JZ        L_4291              

L_4286:                             ; mine.c:1422
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
                                    ; mine.c:1423
JMP       L_4113              

L_4291:
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        CheckFleet          

CheckPlanet:                        ; mine.c:1428
MOV       [bp-scan+0x4], 0x0001     ; [bp-0x22], 0x0001
                                    ; mine.c:1429
MOV       ax, [bp-scan+0x8]         ; ax, [bp-0x1e]
MOV       [bp-idNew], ax            ; [bp-0x16], ax
                                    ; mine.c:1430
PUSH      [bp-idNew]                ; [bp-0x16]
CALLF     LpplFromId                ; PLANET * LpplFromId(int16_t idPlanet)
ADD       sp, 0x0002          
MOV       [bp-lppl], ax             ; [bp-0xc], ax
MOV       [bp-lppl+0x2], dx         ; [bp-0xa], dx
                                    ; mine.c:1431
CMP       [bp-lppl], 0x0000         ; [bp-0xc], 0x0000
JNZ       L_42d5              

L_42c4:
CMP       [bp-lppl+0x2], 0x0000     ; [bp-0xa], 0x0000
JNZ       L_42d5              

L_42cd:                             ; mine.c:1432
MOV       [bp-fOurs], 0x0000        ; [bp-0xe], 0x0000
                                    ; mine.c:1433
JMP       ChangeIt            

L_42d5:                             ; mine.c:1434
LES       bx, [bp-lppl]             ; bx, [bp-0xc]
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       es:[bx+0x2], ax     
JNZ       L_42ea              

L_42e4:
MOV       ax, 0x0001          
JMP       L_42ed              

L_42ea:
MOV       ax, 0x0000          

L_42ed:
MOV       [bp-fOurs], ax            ; [bp-0xe], ax

L_42f0:                             ; mine.c:1436
JMP       ChangeIt            

CheckFleet:                         ; mine.c:1439
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
JMP       L_42ff              

L_42fb:
ADD       [bp-i], 0x0001            ; [bp-0x14], 0x0001

L_42ff:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x14], ax
JGE       L_4363              

L_430a:
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0x12], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x10], dx
CMP       ax, 0x0000          
JNZ       L_433a              

L_4332:
CMP       dx, 0x0000          
JZ        L_4363              

L_433a:                             ; mine.c:1440
LES       bx, [bp-lpfl]             ; bx, [bp-0x12]
MOV       ax, es:[bx+0x8]     
MOV       dx, es:[bx+0xa]     
LEA       bx, [bp-scan]             ; bx, [bp-0x26]
MOV       cx, ss              
MOV       es, cx              
CMP       es:[bx], ax         
JNZ       L_42fb              

L_4354:
CMP       es:[bx+0x2], dx     
JNZ       L_42fb              

L_4363:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-i], ax                ; [bp-0x14], ax
JNZ       L_4384              

L_436e:
MOV       ax, [bp-scan+0x6]         ; ax, [bp-0x20]
AND       ax, 0x0008          
CMP       ax, 0x0000          
JZ        L_4384              

L_437c:                             ; mine.c:1444
MOV       [bp-i], 0x0000            ; [bp-0x14], 0x0000
                                    ; mine.c:1445
JMP       L_4113              

L_4384:                             ; mine.c:1448
MOV       [bp-scan+0x4], 0x0002     ; [bp-0x22], 0x0002
                                    ; mine.c:1449
MOV       ax, [bp-i]                ; ax, [bp-0x14]
MOV       [bp-scan+0xa], ax         ; [bp-0x1c], ax
                                    ; mine.c:1450
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
LES       bx, es:[bx]         
MOV       ax, es:[bx]         
MOV       [bp-idNew], ax            ; [bp-0x16], ax
                                    ; mine.c:1451
MOV       ax, [bp-i]                ; ax, [bp-0x14]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
LES       bx, es:[bx]         
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       es:[bx+0x2], ax     
JNZ       L_43d3              

L_43cd:
MOV       ax, 0x0001          
JMP       L_43d6              

L_43d3:
MOV       ax, 0x0000          

L_43d6:
MOV       [bp-fOurs], ax            ; [bp-0xe], ax

ChangeIt:                           ; mine.c:1455
CMP       [bp-fOurs], 0x0000        ; [bp-0xe], 0x0000
JZ        L_43ec              

L_43e2:
CMP       [sel+0x4], 0x0002         ; [0x495a], 0x0002
JZ        L_43f2              

L_43ec:                             ; mine.c:1456
MOV       ax, [sel+0x18]            ; ax, [0x496e]
MOV       [bp-scan+0xc], ax         ; [bp-0x1a], ax

L_43f2:                             ; mine.c:1458
MOV       ax, 0x0002          
PUSH      ax                  
LEA       ax, [bp-scan]             ; ax, [bp-0x26]
PUSH      ax                  
CALLF     ChangeScanSel             ; void ChangeScanSel(SCAN *pscan, int16_t fValidScan)
ADD       sp, 0x0004          
                                    ; mine.c:1459
CMP       [bp-fOurs], 0x0000        ; [bp-0xe], 0x0000
JZ        L_47d5              

L_440b:                             ; mine.c:1461
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     RedrawScanSel             ; void RedrawScanSel(HDC hdc, int16_t fVis)
ADD       sp, 0x0004          
                                    ; mine.c:1462
PUSH      [bp-idNew]                ; [bp-0x16]
PUSH      [bp-scan+0x4]             ; [bp-0x22]
CALLF     ChangeMainObjSel          ; void ChangeMainObjSel(GrobjClass grobjNew, int16_t iObjSel)
ADD       sp, 0x0004          
                                    ; mine.c:1463
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     RedrawScanSel             ; void RedrawScanSel(HDC hdc, int16_t fVis)
ADD       sp, 0x0004          

L_4439:                             ; mine.c:1466
JMP       L_47d5              

L_443c:                             ; mine.c:1473
MOV       [bp-iChecked], 0xffff     ; [bp-0x78], 0xffff
                                    ; mine.c:1474
MOV       [bp-rgi], 0x0064          ; [bp-0x76], 0x0064
MOV       [bp-rgi+0x2], 0x01f4      ; [bp-0x74], 0x01f4
MOV       [bp-rgi+0x4], 0x03e8      ; [bp-0x72], 0x03e8
MOV       [bp-rgi+0x6], 0x09c4      ; [bp-0x70], 0x09c4
MOV       [bp-rgi+0x8], 0x1388      ; [bp-0x6e], 0x1388
MOV       [bp-rgi+0xa], 0x1d4c      ; [bp-0x6c], 0x1d4c
MOV       [bp-rgi+0xc], 0x2710      ; [bp-0x6a], 0x2710
MOV       [bp-rgi+0xe], 0x4e20      ; [bp-0x68], 0x4e20
MOV       [bp-rgi+0x10], 0x7530     ; [bp-0x66], 0x7530
                                    ; mine.c:1476
MOV       [bp-i], 0x0000            ; [bp-0x64], 0x0000
JMP       L_44d6              

L_4476:                             ; mine.c:1478
MOV       ax, [bp-i]                ; ax, [bp-0x64]
SHL       ax, 0x0001          
LEA       bx, [bp-0x76]       
ADD       bx, ax              
PUSH      [bx]                
MOV       ax, [PCTDKT]              ; ax, [0x01c0]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x000a          
IMUL      [bp-i]                    ; [bp-0x64]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x62]
ADD       cx, ax              
MOV       dx, ss              
PUSH      dx                  
PUSH      cx                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR *lpszout, LPCSTR *lpszfmt)
ADD       sp, 0x000a          
                                    ; mine.c:1479
MOV       ax, 0x000a          
IMUL      [bp-i]                    ; [bp-0x64]
LEA       cx, [bp-rgsz]             ; cx, [bp-0x62]
ADD       cx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x64]
SHL       ax, 0x0001          
LEA       bx, [bp-0x8a]       
ADD       bx, ax              
MOV       [bx], cx            
                                    ; mine.c:1480
MOV       ax, [bp-i]                ; ax, [bp-0x64]
SHL       ax, 0x0001          
LEA       bx, [bp-0x76]       
ADD       bx, ax              
MOV       ax, [cMinGrafMax]         ; ax, [0x04f8]
CMP       [bx], ax            
JNZ       L_44d2              

L_44cc:                             ; mine.c:1481
MOV       ax, [bp-i]                ; ax, [bp-0x64]
MOV       [bp-iChecked], ax         ; [bp-0x78], ax

L_44d2:                             ; mine.c:1482
ADD       [bp-i], 0x0001            ; [bp-0x64], 0x0001

L_44d6:
CMP       [bp-i], 0x0009            ; [bp-0x64], 0x0009
JL        L_4476              

L_44df:                             ; mine.c:1484
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [bp-iChecked]             ; [bp-0x78]
LEA       ax, [bp-psz]              ; ax, [bp-0x8a]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0009          
PUSH      ax                  
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     PopupMenu                 ; int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
ADD       sp, 0x0010          
MOV       [bp-i], ax                ; [bp-0x64], ax
                                    ; mine.c:1485
CMP       [bp-i], 0xffff            ; [bp-0x64], 0xffff
JZ        L_47d5              

L_4511:
MOV       ax, [bp-i]                ; ax, [bp-0x64]
SHL       ax, 0x0001          
LEA       bx, [bp-0x76]       
ADD       bx, ax              
MOV       ax, [cMinGrafMax]         ; ax, [0x04f8]
CMP       [bx], ax            
JZ        L_47d5              

L_4525:                             ; mine.c:1487
MOV       ax, [bp-i]                ; ax, [bp-0x64]
SHL       ax, 0x0001          
LEA       bx, [bp-0x76]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [cMinGrafMax], ax         ; [0x04f8], ax
                                    ; mine.c:1488
PUSH      [hwndMine]                ; [0x0194]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)
                                    ; mine.c:1489
MOV       ax, [grbitScan]           ; ax, [0x0588]
AND       ax, 0x000f          
CMP       ax, 0x0001          
JNZ       L_47d5              

L_4557:                             ; mine.c:1490
PUSH      [hwndScanner]             ; [0x0190]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     InvalidateRect            ; void InvalidateRect(HWND arg1, RECT *arg2, int16_t arg3)

L_456c:                             ; mine.c:1493
JMP       L_47d5              

L_456f:                             ; mine.c:1503
LEA       ax, [bp-pl]               ; ax, [bp-0x4c]
PUSH      ax                  
PUSH      [sel+0x14]                ; [0x496a]
CALLF     FLookupPlanet             ; int16_t FLookupPlanet(int16_t iPlanet, PLANET *ppl)
ADD       sp, 0x0004          
                                    ; mine.c:1505
MOV       [GlobalPD], 0x0001        ; [0x0b80], 0x0001
                                    ; mine.c:1506
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xffff          
CWD       dx, ax              
MOV       [GlobalPD+0x2], ax        ; [0x0b82], ax
MOV       [GlobalPD+0x4], dx        ; [0x0b84], dx
                                    ; mine.c:1508
MOV       [bp-i], 0x0001            ; [bp-0x4e], 0x0001
JMP       L_45b4              

L_459b:                             ; mine.c:1509
MOV       ax, [bp-i]                ; ax, [bp-0x4e]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x0b82          
ADD       bx, ax              
MOV       [bx], 0xffff        
MOV       [bx+0x2], 0xffff    
ADD       [bp-i], 0x0001            ; [bp-0x4e], 0x0001

L_45b4:
CMP       [bp-i], 0x0004            ; [bp-0x4e], 0x0004
JLE       L_459b              

L_45bd:                             ; mine.c:1511
MOV       ax, [bp-pl+0x4]           ; ax, [bp-0x48]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JC        L_478e              

L_45cb:                             ; mine.c:1513
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xffff          
LEA       bx, [bp-0x43]       
ADD       bx, ax              
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       dx, 0x0000          
MOV       [GlobalPD+0xe], ax        ; [0x0b8e], ax
MOV       [GlobalPD+0x10], dx       ; [0x0b90], dx
                                    ; mine.c:1514
MOV       cx, 0x000a          
MOV       ax, [bp-pl+0x4]           ; ax, [bp-0x48]
SHR       ax, cx              
AND       ax, 0x0001          
MOV       dx, 0x0000          
MOV       [GlobalPD+0x6], ax        ; [0x0b86], ax
MOV       [GlobalPD+0x8], dx        ; [0x0b88], dx
                                    ; mine.c:1516
MOV       ax, [bp-pl+0x4]           ; ax, [bp-0x48]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JBE       L_478e              

L_4608:                             ; mine.c:1522
MOV       [bp-lVal], 0x0000         ; [bp-0x68], 0x0000
MOV       [bp-lVal+0x2], 0x0000     ; [bp-0x66], 0x0000
                                    ; mine.c:1524
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xffff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x30]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       [GlobalPD+0xa], ax        ; [0x0b8a], ax
MOV       [GlobalPD+0xc], dx        ; [0x0b8c], dx
                                    ; mine.c:1526
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0xffff          
MOV       dx, 0xffff          
PUSH      dx                  
PUSH      ax                  
LEA       ax, [bp-rglQuan]          ; ax, [bp-0x14]
PUSH      ax                  
LEA       ax, [bp-pl]               ; ax, [bp-0x4c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     EstMineralsMined          ; void EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply)
ADD       sp, 0x000c          
                                    ; mine.c:1527
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xffff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x14]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       [GlobalPD+0x12], ax       ; [0x0b92], ax
MOV       [GlobalPD+0x14], dx       ; [0x0b94], dx
                                    ; mine.c:1529
CMP       [bp-pl+0x2], 0xffff       ; [bp-0x4a], 0xffff
JNZ       L_478e              

L_4670:                             ; mine.c:1531
MOV       [bp-ifl], 0x0000          ; [bp-0x5c], 0x0000
JMP       L_467c              

L_4678:
ADD       [bp-ifl], 0x0001          ; [bp-0x5c], 0x0001

L_467c:
MOV       ax, [cFleet]              ; ax, [0x5356]
CMP       [bp-ifl], ax              ; [bp-0x5c], ax
JGE       L_476a              

L_4687:
MOV       ax, [bp-ifl]              ; ax, [bp-0x5c]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [rglpfl]              ; bx, [0x00fa]
MOV       cx, [rglpfl+0x2]          ; cx, [0x00fc]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lpfl], ax             ; [bp-0x60], ax
MOV       [bp-lpfl+0x2], dx         ; [bp-0x5e], dx
CMP       ax, 0x0000          
JNZ       L_46b7              

L_46af:
CMP       dx, 0x0000          
JZ        L_476a              

L_46b7:                             ; mine.c:1535
LES       bx, [bp-lpfl]             ; bx, [bp-0x60]
MOV       ax, [bp-pl]               ; ax, [bp-0x4c]
CMP       es:[bx+0x6], ax     
JNZ       L_4678              

L_46c6:
LES       bx, [bp-lpfl]             ; bx, [bp-0x60]
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       es:[bx+0x2], ax     
JNZ       L_4678              

L_46d5:
LES       bx, [bp-lpfl]             ; bx, [bp-0x60]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x000a          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_4678              

L_46ec:
LES       bx, [bp-lpfl]             ; bx, [bp-0x60]
LES       bx, es:[bx+0x64]    
MOV       ax, es:[bx+0xa]     
AND       ax, 0x000f          
CMP       ax, 0x0003          
JNZ       L_4678              

L_4705:                             ; mine.c:1538
PUSH      [bp-lpfl+0x2]             ; [bp-0x5e]
PUSH      [bp-lpfl]                 ; [bp-0x60]
CALLF     CMineFromLpfl             ; int32_t CMineFromLpfl(FLEET *lpfl)
ADD       sp, 0x0004          
MOV       [bp-cMines], ax           ; [bp-0x64], ax
MOV       [bp-cMines+0x2], dx       ; [bp-0x62], dx
                                    ; mine.c:1539
CMP       [bp-cMines+0x2], 0x0000   ; [bp-0x62], 0x0000
JL        L_4678              

L_4722:
JG        L_4730              

L_4727:
CMP       [bp-cMines], 0x0000       ; [bp-0x64], 0x0000
JBE       L_4678              

L_4730:                             ; mine.c:1541
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-cMines+0x2]           ; [bp-0x62]
PUSH      [bp-cMines]               ; [bp-0x64]
LEA       ax, [bp-rglT]             ; ax, [bp-0x5a]
PUSH      ax                  
LEA       ax, [bp-pl]               ; ax, [bp-0x4c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     EstMineralsMined          ; void EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply)
ADD       sp, 0x000c          
                                    ; mine.c:1542
MOV       ax, [bp-ht]               ; ax, [bp-0x8]
ADD       ax, 0xffff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
LEA       bx, [bp-0x5a]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
ADD       [bp-lVal], ax             ; [bp-0x68], ax
ADC       [bp-lVal+0x2], dx         ; [bp-0x66], dx

L_4767:                             ; mine.c:1544
JMP       L_4678              

L_476a:                             ; mine.c:1545
CMP       [bp-lVal+0x2], 0x0000     ; [bp-0x66], 0x0000
JL        L_478e              

L_4773:
JG        L_4781              

L_4778:
CMP       [bp-lVal], 0x0000         ; [bp-0x68], 0x0000
JBE       L_478e              

L_4781:                             ; mine.c:1546
MOV       ax, [bp-lVal]             ; ax, [bp-0x68]
MOV       dx, [bp-lVal+0x2]         ; dx, [bp-0x66]
MOV       [GlobalPD+0x12], ax       ; [0x0b92], ax
MOV       [GlobalPD+0x14], dx       ; [0x0b94], dx

L_478e:                             ; mine.c:1551
PUSH      [bp+y]                    ; [bp+0x8]
PUSH      [bp+x]                    ; [bp+0x6]
PUSH      [hwndMine]                ; [0x0194]
CALLF     Popup                     ; void Popup(HWND hwnd, int16_t x, int16_t y)
ADD       sp, 0x0006          
                                    ; mine.c:1552
JMP       L_47d5              

L_47a6:
CMP       ax, 0x000e          
JA        L_47d5              

L_47ae:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx+0x47b7]      

L_47d5:                             ; mine.c:1556
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


