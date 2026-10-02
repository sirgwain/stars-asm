; PszFormatString  (msg)
;   addr: 0007:85cc  len=2459
;   sig:  char * PszFormatString(char *pszFormat, int16_t *pParamsReal)
;   params:
;     char *           pszFormat      [BP+0x6]
;     int16_t *        pParamsReal    [BP+0x8]
;   locals:
;     char *           pch            [BP-0x1f4]
;     uint16_t         w              [BP-0x1f2]
;     char[480]        szBuf          [BP-0x1f0]
;     char *           pchT           [BP-0x10]
;     int16_t *        pParams        [BP-0xe]
;     int16_t          i              [BP-0xa]
;     int16_t          c              [BP-0x8]
;     int16_t          cOut           [BP-0x6]
;     int16_t          iMineral       [BP-0x4]
;     block 0007:8B5E  len=0x8E
;       PART             part           [BP-0x1fc]
;     block 0007:8CFD  len=0xB8
;       int32_t          l              [BP-0x1f8]
;     block 0007:8DB5  len=0x102
;       SHDEF *          lpshdef        [BP-0x1f8]
;
;   stats: blocks=3  labels=6
;     DoPlanet: L_8ad8
;     DoFleet: L_8b26
;     LThingName: L_8bec
;     DoInt: L_87c9
;     FinishString: L_8ae9
;     DoNothing: L_8b07

L_85cc:                             ; msg.c:1243
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x01fe          
PUSH      si                  
PUSH      di                  
                                    ; msg.c:1247
MOV       [bp-iMineral], 0xffff     ; [bp-0x4], 0xffff
                                    ; msg.c:1251
MOV       ax, [bp+pParamsReal]      ; ax, [bp+0x8]
MOV       dx, [bp+pParamsReal+0x2]  ; dx, [bp+0xa]
MOV       [bp-pParams], ax          ; [bp-0xe], ax
MOV       [bp-pParams+0x2], dx      ; [bp-0xc], dx
                                    ; msg.c:1254
MOV       [bp-pch], szMsgBuf        ; [bp-0x1f4], 0x535a

L_85ec:                             ; msg.c:1256
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_8f54              

L_85fa:                             ; msg.c:1258
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x005c          
JZ        L_861b              

L_8608:                             ; msg.c:1259
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
MOV       bx, [bp-pch]              ; bx, [bp-0x1f4]
ADD       [bp-pch], 0x0001          ; [bp-0x1f4], 0x0001
MOV       [bx], al            
                                    ; msg.c:1260
JMP       L_8f4d              

L_861b:                             ; msg.c:1262
ADD       [bp+pszFormat], 0x0001    ; [bp+0x6], 0x0001
                                    ; msg.c:1271
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
JMP       L_8ecd              

L_8628:                             ; msg.c:1274
MOV       ax, 0x57a4          
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1275
MOV       ax, 0x57a4          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1276
JMP       L_8f4d              

L_864b:                             ; msg.c:1283
MOV       ax, 0x56a2          
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1284
MOV       ax, 0x56a2          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1285
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
JMP       L_8727              

L_8674:                             ; msg.c:1288
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_86a2              

L_867e:                             ; msg.c:1290
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0b54          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1291
JMP       DoInt               

L_86a2:                             ; msg.c:1295
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_86d0              

L_86ac:                             ; msg.c:1297
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0b59          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1298
JMP       DoInt               

L_86d0:                             ; msg.c:1302
MOV       ax, 0x0b5e          
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1303
ADD       [bp-pch], 0x0004          ; [bp-0x1f4], 0x0004
                                    ; msg.c:1304
JMP       L_8f4d              

L_86e8:                             ; msg.c:1307
MOV       ax, [idPlayer]            ; ax, [0x018c]
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0b63          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1308
JMP       DoInt               

L_870c:                             ; msg.c:1310
MOV       ax, 0x0b68          
PUSH      ax                  
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1311
ADD       [bp-pch], 0x0003          ; [bp-0x1f4], 0x0003
                                    ; msg.c:1312
JMP       L_8f4d              

L_8727:
CMP       ax, 0x0066          
JZ        L_8674              

L_872f:
CMP       ax, 0x0068          
JZ        L_86d0              

L_8737:
CMP       ax, 0x0072          
JZ        L_86e8              

L_873f:
CMP       ax, 0x0074          
JZ        L_86a2              

L_8747:
CMP       ax, 0x0079          
JNZ       L_8f4d              

L_874c:
JMP       L_870c              

L_8755:                             ; msg.c:1318
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       bx, es:[bx]         
SHL       bx, 0x0001          
MOV       ax, [bx+0x47e]      
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1319
JMP       FinishString        

L_8767:                             ; msg.c:1322
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x00ff          
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0008          
SHR       ax, cx              
AND       ax, 0x00ff          
AND       ax, 0x00ff          
PUSH      ax                  
CALLF     PszCalcEnvVar             ; char * PszCalcEnvVar(EnvType iEnv, int16_t iVar)
ADD       sp, 0x0004          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1323
JMP       FinishString        

L_8791:                             ; msg.c:1327
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
ADD       ax, 0x0544          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1328
JMP       FinishString        

L_87a9:                             ; msg.c:1331
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
MOV       ax, [PCTD]                ; ax, [0x01aa]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax

DoInt:                              ; msg.c:1333
MOV       ax, [bp-c]                ; ax, [bp-0x8]
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1334
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1335
JMP       L_8f4d              

L_87d7:                             ; msg.c:1342
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x00c0          
MOV       cx, 0x0006          
SAR       ax, cx              
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x0020          
CMP       ax, 0x0000          
JZ        L_8801              

L_87fb:
MOV       ax, 0x0001          
JMP       L_8804              

L_8801:
MOV       ax, 0x0000          

L_8804:
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x0010          
CMP       ax, 0x0000          
JZ        L_881c              

L_8816:
MOV       ax, 0x0001          
JMP       L_881f              

L_881c:
MOV       ax, 0x0000          

L_881f:
PUSH      ax                  
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x004c          
JNZ       L_8834              

L_882e:
MOV       ax, 0x0001          
JMP       L_8837              

L_8834:
MOV       ax, 0x0000          

L_8837:
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x000f          
PUSH      ax                  
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1343
JMP       FinishString        

L_8850:                             ; msg.c:1346
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-w], ax                ; [bp-0x1f2], ax
                                    ; msg.c:1347
CMP       [bp-w], 0x0000            ; [bp-0x1f2], 0x0000
JZ        L_8f4d              

L_8867:                             ; msg.c:1349
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
ADD       ax, 0xffff          
AND       ax, [bp-w]                ; ax, [bp-0x1f2]
CMP       ax, 0x0000          
JNZ       L_88c1              

L_887a:                             ; msg.c:1351
MOV       [bp-c], 0x0000            ; [bp-0x8], 0x0000

L_887f:                             ; msg.c:1352
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_889c              

L_888e:                             ; msg.c:1354
ADD       [bp-c], 0x0001            ; [bp-0x8], 0x0001
                                    ; msg.c:1355
MOV       cx, 0x0001          
SHR       [bp-w], cx                ; [bp-0x1f2], cx
                                    ; msg.c:1356
JMP       L_887f              

L_889c:                             ; msg.c:1357
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x8]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1358
JMP       FinishString        

L_88c1:                             ; msg.c:1361
MOV       [bp-cOut], 0x0000         ; [bp-0x6], 0x0000
                                    ; msg.c:1362
MOV       [bp-i], 0x0000            ; [bp-0xa], 0x0000
JMP       L_88e0              

L_88ce:
MOV       ax, [bp-0xa]        
ADD       [bp-i], 0x0001            ; [bp-0xa], 0x0001
MOV       cx, 0x0001          
SHR       [bp-w], cx                ; [bp-0x1f2], cx
MOV       ax, [bp-0x1f2]      

L_88e0:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-i], ax                ; [bp-0xa], ax
JGE       DoNothing           

L_88eb:                             ; msg.c:1364
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_88ce              

L_88fd:                             ; msg.c:1367
CMP       [bp-cOut], 0x0000         ; [bp-0x6], 0x0000
JLE       L_8944              

L_8906:                             ; msg.c:1369
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
AND       ax, 0xfffe          
CMP       ax, 0x0000          
JZ        L_8930              

L_8915:                             ; msg.c:1371
MOV       bx, [bp-pch]              ; bx, [bp-0x1f4]
ADD       [bp-pch], 0x0001          ; [bp-0x1f4], 0x0001
MOV       [bx], 0x002c        
                                    ; msg.c:1372
MOV       bx, [bp-pch]              ; bx, [bp-0x1f4]
ADD       [bp-pch], 0x0001          ; [bp-0x1f4], 0x0001
MOV       [bx], 0x0020        
                                    ; msg.c:1374
JMP       L_8944              

L_8930:                             ; msg.c:1376
PUSH      [bp-pch]                  ; [bp-0x1f4]
MOV       ax, 0x0259          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax

L_8944:                             ; msg.c:1380
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-i]                    ; [bp-0xa]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1381
PUSH      [bp-pchT]                 ; [bp-0x10]
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1382
PUSH      [bp-pchT]                 ; [bp-0x10]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1384
ADD       [bp-cOut], 0x0001         ; [bp-0x6], 0x0001
                                    ; msg.c:1385
JMP       L_88ce              

L_898e:                             ; msg.c:1390
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       es:[bx], ax         
JZ        DoNothing           

L_899f:                             ; msg.c:1392
LEA       ax, [bp-szBuf]            ; ax, [bp-0x1f0]
PUSH      ax                  
MOV       ax, 0x0546          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
                                    ; msg.c:1393
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1394
PUSH      [bp-pchT]                 ; [bp-0x10]
LEA       ax, [bp-szBuf]            ; ax, [bp-0x1f0]
PUSH      ax                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1395
MOV       ax, 0x0547          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
PUSH      ax                  
LEA       ax, [bp-szBuf]            ; ax, [bp-0x1f0]
PUSH      ax                  
CALLF     strcat                    ; char * strcat(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1396
LEA       ax, [bp-szBuf]            ; ax, [bp-0x1f0]
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1397
JMP       FinishString        

L_8a09:                             ; msg.c:1400
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-iMineral], ax         ; [bp-0x4], ax
                                    ; msg.c:1402
MOV       bx, [bp-iMineral]         ; bx, [bp-0x4]
SHL       bx, 0x0001          
MOV       ax, [bx+0x4cc]      
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1403
JMP       FinishString        

L_8a21:                             ; msg.c:1407
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       bx, es:[bx]         
SHL       bx, 0x0001          
MOV       ax, [bx+0x4f2]      
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1408
JMP       FinishString        

L_8a33:                             ; msg.c:1411
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0064          
CWD       dx, ax              
IDIV      cx                  
CWD       dx, ax              
MOV       [bp-0x1fc], ax      
MOV       [bp-0x1fa], dx      
WAIT                          
FILD      [bp-0x1fc]          
WAIT                          
FLD       [0x1dae]            
WAIT                          
FXCH      st(1)               
CALLF     __aFfcompp                ; void __aFfcompp()
JC        L_8a89              

L_8a5f:                             ; msg.c:1412
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0064          
CWD       dx, ax              
IDIV      cx                  
PUSH      ax                  
MOV       ax, [PCTDPCTPCT]          ; ax, [0x01b8]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1413
JMP       L_8aca              

L_8a89:                             ; msg.c:1414
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0064          
CWD       dx, ax              
IDIV      cx                  
MOV       cx, 0x0064          
IMUL      cx                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       cx, es:[bx]         
SUB       cx, ax              
PUSH      cx                  
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0064          
CWD       dx, ax              
IDIV      cx                  
PUSH      ax                  
MOV       ax, [PCTDXPCTDPCTPCT]     ; ax, [0x01d0]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000c          
MOV       [bp-c], ax                ; [bp-0x8], ax

L_8aca:                             ; msg.c:1415
MOV       ax, [bp-c]                ; ax, [bp-0x8]
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1416
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1417
JMP       L_8f4d              

DoPlanet:                           ; msg.c:1421
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
CALLF     PszGetPlanetName          ; char * PszGetPlanetName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax

FinishString:                       ; msg.c:1423
PUSH      [bp-pchT]                 ; [bp-0x10]
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1424
PUSH      [bp-pchT]                 ; [bp-0x10]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax

DoNothing:                          ; msg.c:1426
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1427
JMP       L_8f4d              

L_8b11:                             ; msg.c:1433
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
PUSH      ax                  
CALLF     PszFleetNameFromWord      ; char * PszFleetNameFromWord(uint16_t w)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1434
JMP       FinishString        

DoFleet:                            ; msg.c:1438
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
OR        ax, 0x8000          
MOV       [bp-w], ax                ; [bp-0x1f2], ax
                                    ; msg.c:1439
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
PUSH      ax                  
CALLF     PszGetFleetName           ; char * PszGetFleetName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1440
JMP       FinishString        

L_8b46:                             ; msg.c:1444
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
ADD       ax, 0x0054          
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1445
JMP       FinishString        

L_8b5e:                             ; msg.c:1450
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-part], ax             ; [bp-0x1fc], ax
                                    ; msg.c:1451
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1452
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-0x1fe], ax      
MOV       ax, [bp-0x1fe]      
AND       ax, 0x00ff          
MOV       cx, [bp-part+0x2]         ; cx, [bp-0x1fa]
AND       cx, 0xff00          
OR        cx, ax              
MOV       [bp-part+0x2], cx         ; [bp-0x1fa], cx
MOV       ax, cx              
                                    ; msg.c:1453
LEA       ax, [bp-part]             ; ax, [bp-0x1fc]
PUSH      ax                  
CALLF     FLookupPart               ; int16_t FLookupPart(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JLE       L_8ba8              

L_8ba2:
MOV       ax, 0x0001          
JMP       L_8bab              

L_8ba8:
MOV       ax, 0x0000          

L_8bab:                             ; msg.c:1454
MOV       ax, 0x0008          
MOV       cx, [bp-part+0x4]         ; cx, [bp-0x1f8]
MOV       dx, [bp-part+0x6]         ; dx, [bp-0x1f6]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; msg.c:1455
MOV       ax, 0x0008          
MOV       cx, [bp-part+0x4]         ; cx, [bp-0x1f8]
MOV       dx, [bp-part+0x6]         ; dx, [bp-0x1f6]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fstrlen                   ; uint16_t fstrlen(char *s)
ADD       sp, 0x0004          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1456
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1457
JMP       L_8f4d              

LThingName:                         ; msg.c:1462
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
CALLF     PszGetThingName           ; char * PszGetThingName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1463
JMP       FinishString        

L_8c00:                             ; msg.c:1466
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-w], ax                ; [bp-0x1f2], ax
                                    ; msg.c:1468
PUSH      [bp-pch]                  ; [bp-0x1f4]
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
ADD       ax, 0x04e2          
PUSH      ax                  
CALLF     CchGetString              ; int16_t CchGetString(StringId ids, char *psz)
ADD       sp, 0x0004          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1469
MOV       ax, [bp-c]                ; ax, [bp-0x8]
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1470
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1471
JMP       L_8f4d              

L_8c2f:                             ; msg.c:1474
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
CMP       es:[bx], 0xfffe     
JNZ       L_8c45              

L_8c3b:                             ; msg.c:1476
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1477
JMP       LThingName          

L_8c45:
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
CMP       es:[bx], 0xffff     
JZ        L_8c78              

L_8c51:                             ; msg.c:1481
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx+0x2]         
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
MOV       ax, 0xffff          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
CALLF     PszGetLocName             ; char * PszGetLocName(GrobjClass grobj, int16_t id, int16_t x, int16_t y)
ADD       sp, 0x0008          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1482
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1483
JMP       FinishString        

L_8c78:                             ; msg.c:1485
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002

L_8c7c:                             ; msg.c:1490
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x8000          
CMP       ax, 0x0000          
JNZ       DoFleet             

L_8c8a:
JMP       DoPlanet            

L_8c96:                             ; msg.c:1496
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x000f          
MOV       [bp-w], ax                ; [bp-0x1f2], ax
                                    ; msg.c:1498
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
PUSH      ax                  
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1499
JMP       FinishString        

L_8ccf:                             ; msg.c:1502
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
PUSH      es:[bx]             
MOV       ax, 0x0b6c          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000a          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1503
MOV       ax, [bp-c]                ; ax, [bp-0x8]
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1504
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1505
JMP       L_8f4d              

L_8cfd:                             ; msg.c:1514
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       dx, 0x0000          
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       cx, es:[bx+0x2]     
MOV       [bp-0x1fc], ax      
MOV       [bp-0x1fa], dx      
MOV       ax, cx              
MOV       dx, 0x0000          
MOV       cx, 0x0010          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       cx, [bp-0x1fc]      
MOV       bx, [bp-0x1fa]      
OR        ax, cx              
OR        dx, bx              
MOV       [bp-l], ax                ; [bp-0x1f8], ax
MOV       [bp-l+0x2], dx            ; [bp-0x1f6], dx
                                    ; msg.c:1515
ADD       [bp-pParams], 0x0004      ; [bp-0xe], 0x0004
                                    ; msg.c:1516
PUSH      [bp-l+0x2]                ; [bp-0x1f6]
PUSH      [bp-l]                    ; [bp-0x1f8]
MOV       ax, [PCTLD]               ; ax, [0x01b0]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x000c          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1517
MOV       ax, [bp-c]                ; ax, [bp-0x8]
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1518
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0076          
JZ        L_8f4d              

L_8d71:                             ; msg.c:1520
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0056          
JNZ       L_8d88              

L_8d7f:                             ; msg.c:1521
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
MOV       [bp-iMineral], ax         ; [bp-0x4], ax

L_8d88:                             ; msg.c:1524
MOV       bx, [bp-iMineral]         ; bx, [bp-0x4]
SHL       bx, 0x0001          
MOV       ax, [bx+0xb48]      
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1525
PUSH      [bp-pchT]                 ; [bp-0x10]
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strcpy                    ; char * strcpy(char *dest, char *src)
ADD       sp, 0x0004          
                                    ; msg.c:1526
PUSH      [bp-pchT]                 ; [bp-0x10]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax

L_8db2:                             ; msg.c:1528
JMP       L_8f4d              

L_8db5:                             ; msg.c:1535
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
SAR       ax, 0x0001          
MOV       [bp-c], ax                ; [bp-0x8], ax
                                    ; msg.c:1536
LES       bx, [bp-pParams]          ; bx, [bp-0xe]
MOV       ax, es:[bx]         
AND       ax, 0x001f          
MOV       [bp-w], ax                ; [bp-0x1f2], ax
                                    ; msg.c:1537
CMP       [bp-w], 0x0010            ; [bp-0x1f2], 0x0010
JC        L_8e07              

L_8ddf:                             ; msg.c:1538
MOV       ax, [bp-w]                ; ax, [bp-0x1f2]
ADD       ax, 0xfff0          
MOV       cx, 0x0093          
IMUL      cx                  
MOV       bx, [bp-c]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0x14c]      
MOV       dx, [bx+0x14e]      
ADD       cx, ax              
MOV       [bp-lpshdef], cx          ; [bp-0x1f8], cx
MOV       [bp-lpshdef+0x2], dx      ; [bp-0x1f6], dx
                                    ; msg.c:1539
JMP       L_8e27              

L_8e07:                             ; msg.c:1540
MOV       ax, 0x0093          
IMUL      [bp-w]                    ; [bp-0x1f2]
MOV       bx, [bp-c]                ; bx, [bp-0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       cx, [bx+0xfe]       
MOV       dx, [bx+0x100]      
ADD       cx, ax              
MOV       [bp-lpshdef], cx          ; [bp-0x1f8], cx
MOV       [bp-lpshdef+0x2], dx      ; [bp-0x1f6], dx

L_8e27:                             ; msg.c:1542
MOV       ax, [idPlayer]            ; ax, [0x018c]
CMP       [bp-c], ax                ; [bp-0x8], ax
JZ        L_8e84              

L_8e32:                             ; msg.c:1544
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp-c]                    ; [bp-0x8]
CALLF     PszPlayerName             ; char * PszPlayerName(int16_t iPlayer, int16_t fCapital, int16_t fPlural, int16_t fThe, int16_t grWord, PLAYER *pplr)
ADD       sp, 0x000c          
MOV       [bp-pchT], ax             ; [bp-0x10], ax
                                    ; msg.c:1547
MOV       ax, 0x0008          
MOV       cx, [bp-lpshdef]          ; cx, [bp-0x1f8]
MOV       dx, [bp-lpshdef+0x2]      ; dx, [bp-0x1f6]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-pchT]             ; ax, [bp-0x10]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0b6f          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     _wsprintf                 ; int16_t _wsprintf(LPSTR lpszout, LPCSTR lpszfmt)
ADD       sp, 0x0010          
                                    ; msg.c:1552
JMP       L_8ea3              

L_8e84:                             ; msg.c:1553
MOV       ax, 0x0008          
MOV       cx, [bp-lpshdef]          ; cx, [bp-0x1f8]
MOV       dx, [bp-lpshdef+0x2]      ; dx, [bp-0x1f6]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, [bp-pch]              ; ax, [bp-0x1f4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          

L_8ea3:                             ; msg.c:1555
PUSH      [bp-pch]                  ; [bp-0x1f4]
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       [bp-pch], ax              ; [bp-0x1f4], ax
                                    ; msg.c:1556
ADD       [bp-pParams], 0x0002      ; [bp-0xe], 0x0002
                                    ; msg.c:1558
JMP       L_8f4d              

L_8eba:                             ; msg.c:1562
MOV       bx, [bp+pszFormat]        ; bx, [bp+0x6]
MOV       al, [bx]            
MOV       bx, [bp-pch]              ; bx, [bp-0x1f4]
ADD       [bp-pch], 0x0001          ; [bp-0x1f4], 0x0001
MOV       [bx], al            
                                    ; msg.c:1563
JMP       L_8f4d              

L_8ecd:
SUB       ax, 0x0045          
CMP       ax, 0x0035          
JA        L_8eba              

L_8ed8:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       cs:[bx-0x711f]      

L_8f4d:                             ; msg.c:1566
ADD       [bp+pszFormat], 0x0001    ; [bp+0x6], 0x0001
                                    ; msg.c:1567
JMP       L_85ec              

L_8f54:                             ; msg.c:1568
MOV       bx, [bp-pch]              ; bx, [bp-0x1f4]
MOV       [bx], 0x0000        
                                    ; msg.c:1569
MOV       ax, szMsgBuf              ; ax, 0x535a

L_8f61:                             ; msg.c:1570
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


