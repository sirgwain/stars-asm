; AddMinesToBlockedQueues  (aiutil)
;   addr: 0013:1792  len=1379
;   sig:  void AddMinesToBlockedQueues()
;   locals:
;     int16_t          etaBetterMines [BP-0x132]
;     PROD[64]         rgprod         [BP-0x130]
;     int32_t[4]       rgCost         [BP-0x30]
;     int16_t          ipl            [BP-0x20]
;     int32_t          cRes           [BP-0x1e]
;     int32_t          cResMine       [BP-0x1a]
;     PLANET *         lppl           [BP-0x16]
;     int16_t          etaFirst       [BP-0x12]
;     int32_t          cBuild         [BP-0x10]
;     int16_t          etaBetterAlchemy [BP-0xc]
;     int32_t          cMaxBuild      [BP-0xa]
;     PROD             prod           [BP-0x6]
;
;   stats: blocks=0  labels=0

L_1792:                             ; aiutil.c:921
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x013a          
PUSH      si                  
PUSH      di                  
                                    ; aiutil.c:935
MOV       [bp-ipl], 0x0000          ; [bp-0x20], 0x0000
JMP       L_17a7              

L_17a3:
ADD       [bp-ipl], 0x0001          ; [bp-0x20], 0x0001

L_17a7:
MOV       ax, [vclpplAi]            ; ax, [0x4948]
CMP       [bp-ipl], ax              ; [bp-0x20], ax
JGE       L_1cef              

L_17b2:
MOV       ax, [bp-ipl]              ; ax, [bp-0x20]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [vrglpplAi]           ; bx, [0x144c]
MOV       cx, [vrglpplAi+0x2]       ; cx, [0x144e]
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
MOV       [bp-lppl], ax             ; [bp-0x16], ax
MOV       [bp-lppl+0x2], dx         ; [bp-0x14], dx
CMP       ax, 0x0000          
JNZ       L_17e2              

L_17da:
CMP       dx, 0x0000          
JZ        L_1cef              

L_17e2:                             ; aiutil.c:938
LES       bx, [bp-lppl]             ; bx, [bp-0x16]
CMP       es:[bx+0x34], 0x0000
JNZ       L_17fc              

L_17ef:
CMP       es:[bx+0x36], 0x0000
JZ        L_17a3              

L_17f6:
JMP       L_17fc              

L_17f9:                             ; aiutil.c:939
JMP       L_17a3              

L_17fc:                             ; aiutil.c:941
LES       bx, [bp-lppl]             ; bx, [bp-0x16]
LES       bx, es:[bx+0x34]    
MOV       ax, es:[bx+0x4]     
MOV       dx, es:[bx+0x6]     
MOV       [bp-prod], ax             ; [bp-0x6], ax
MOV       [bp-prod+0x2], dx         ; [bp-0x4], dx
                                    ; aiutil.c:945
MOV       cx, 0x0011          
MOV       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x4]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0007          
AND       dx, 0x0000          
CMP       ax, 0x0001          
JNZ       L_18c8              

L_182d:
CMP       dx, 0x0000          
JNZ       L_18c8              

L_1835:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x4]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x0008          
JNZ       L_1859              

L_1851:
CMP       dx, 0x0000          
JZ        L_17a3              

L_1859:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x4]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x0003          
JNZ       L_187d              

L_1875:
CMP       dx, 0x0000          
JZ        L_17a3              

L_187d:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x4]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x000b          
JNZ       L_18a1              

L_1899:
CMP       dx, 0x0000          
JZ        L_17a3              

L_18a1:
MOV       cx, 0x000a          
MOV       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, [bp-prod+0x2]         ; dx, [bp-0x4]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x007f          
AND       dx, 0x0000          
CMP       ax, 0x000c          
JNZ       L_18c8              

L_18bd:
CMP       dx, 0x0000          
JZ        L_17a3              

L_18c2:
JMP       L_18c8              

L_18c5:                             ; aiutil.c:946
JMP       L_17a3              

L_18c8:                             ; aiutil.c:948
LES       bx, [bp-lppl]             ; bx, [bp-0x16]
PUSH      es:[bx]             
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     ChangeMainObjSel          ; void ChangeMainObjSel(GrobjClass grobjNew, int16_t iObjSel)
ADD       sp, 0x0004          
                                    ; aiutil.c:950
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-etaFirst]         ; ax, [bp-0x12]
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [sel+0xce]                ; [0x4a24]
PUSH      [sel+0xcc]                ; [0x4a22]
MOV       ax, 0x49ee          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     PszProductionETA          ; char * PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast)
ADD       sp, 0x000e          
                                    ; aiutil.c:951
CMP       [bp-etaFirst], 0x0001     ; [bp-0x12], 0x0001
JZ        L_17a3              

L_1903:
JMP       L_1909              

L_1906:                             ; aiutil.c:952
JMP       L_17a3              

L_1909:                             ; aiutil.c:953
CMP       [bp-etaFirst], 0xffff     ; [bp-0x12], 0xffff
JNZ       L_1917              

L_1912:                             ; aiutil.c:954
MOV       [bp-etaFirst], 0x0258     ; [bp-0x12], 0x0258

L_1917:                             ; aiutil.c:956
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [idPlayer]                ; [0x018c]
LEA       ax, [bp-rgCost]           ; ax, [bp-0x30]
PUSH      ax                  
LEA       ax, [bp-prod]             ; ax, [bp-0x6]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-lppl+0x2]             ; [bp-0x14]
PUSH      [bp-lppl]                 ; [bp-0x16]
CALLF     GetProductionCosts        ; void GetProductionCosts(PLANET *lppl, PROD *lpprod, uint32_t *rgCost, int16_t iplr, int16_t fOnlyOne)
ADD       sp, 0x000e          
                                    ; aiutil.c:957
PUSH      [idPlayer]                ; [0x018c]
MOV       ax, 0x49ee          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     CResourcesAtPlanet        ; int16_t CResourcesAtPlanet(PLANET *lppl, int16_t iplr)
ADD       sp, 0x0006          
CWD       dx, ax              
MOV       [bp-cRes], ax             ; [bp-0x1e], ax
MOV       [bp-cRes+0x2], dx         ; [bp-0x1c], dx
                                    ; aiutil.c:958
MOV       cx, 0x0017          
MOV       ax, [sel+0xb0]            ; ax, [0x4a06]
MOV       dx, [sel+0xb2]            ; dx, [0x4a08]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0001          
AND       dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_19aa              

L_196f:
CMP       dx, 0x0000          
JNZ       L_19aa              

L_1977:                             ; aiutil.c:959
MOV       ax, 0x0064          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       al, [bx+0x38]       
CBW       ax, al              
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cRes+0x2]             ; [bp-0x1c]
PUSH      [bp-cRes]                 ; [bp-0x1e]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
PUSH      dx                  
PUSH      ax                  
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
SUB       [bp-cRes], ax             ; [bp-0x1e], ax
SBB       [bp-cRes+0x2], dx         ; [bp-0x1c], dx

L_19aa:                             ; aiutil.c:960
MOV       ax, [bp-etaFirst]         ; ax, [bp-0x12]
ADD       ax, 0xffff          
CWD       dx, ax              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cRes+0x2]             ; [bp-0x1c]
PUSH      [bp-cRes]                 ; [bp-0x1e]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
CMP       [bp-rgCost+0xe], dx       ; [bp-0x22], dx
JL        L_19d6              

L_19c6:
JG        L_17a3              

L_19cb:
CMP       [bp-rgCost+0xc], ax       ; [bp-0x24], ax
JA        L_17a3              

L_19d0:
JMP       L_19d6              

L_19d3:                             ; aiutil.c:961
JMP       L_17a3              

L_19d6:                             ; aiutil.c:963
MOV       cx, 0x0008          
MOV       ax, [sel+0xac]            ; ax, [0x4a02]
MOV       dx, [sel+0xae]            ; dx, [0x4a04]
CALLF     __aFulshr                 ; uint32_t __aFulshr(uint32_t val, uint16_t shift)
AND       ax, 0x0fff          
AND       dx, 0x0000          
MOV       [bp-0x136], ax      
MOV       [bp-0x134], dx      
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [idPlayer]                ; [0x018c]
MOV       ax, 0x49ee          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     CMaxOperableMines         ; int16_t CMaxOperableMines(PLANET *lppl, int16_t iplr, int16_t fNextYear)
ADD       sp, 0x0008          
CWD       dx, ax              
MOV       cx, [bp-0x136]      
MOV       bx, [bp-0x134]      
SUB       ax, cx              
SBB       dx, bx              
MOV       [bp-cMaxBuild], ax        ; [bp-0xa], ax
MOV       [bp-cMaxBuild+0x2], dx    ; [bp-0x8], dx
                                    ; aiutil.c:966
CMP       [bp-cMaxBuild+0x2], 0x0000; [bp-0x8], 0x0000
JG        L_1a3e              

L_1a26:
JL        L_1a34              

L_1a2b:
CMP       [bp-cMaxBuild], 0x0000    ; [bp-0xa], 0x0000
JNC       L_1a3e              

L_1a34:                             ; aiutil.c:967
MOV       [bp-cMaxBuild], 0x0000    ; [bp-0xa], 0x0000
MOV       [bp-cMaxBuild+0x2], 0x0000; [bp-0x8], 0x0000

L_1a3e:                             ; aiutil.c:969
MOV       ax, 0x0005          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
CWD       dx, ax              
MOV       [bp-cResMine], ax         ; [bp-0x1a], ax
MOV       [bp-cResMine+0x2], dx     ; [bp-0x18], dx
                                    ; aiutil.c:970
PUSH      [bp-cMaxBuild+0x2]        ; [bp-0x8]
PUSH      [bp-cMaxBuild]            ; [bp-0xa]
PUSH      [bp-cResMine+0x2]         ; [bp-0x18]
PUSH      [bp-cResMine]             ; [bp-0x1a]
CALLF     __aFulmul                 ; uint32_t __aFulmul(uint32_t a, uint32_t b)
CMP       dx, [bp-cRes+0x2]         ; dx, [bp-0x1c]
JG        L_1a93              

L_1a77:
JL        L_1a84              

L_1a7c:
CMP       ax, [bp-cRes]             ; ax, [bp-0x1e]
JA        L_1a93              

L_1a84:                             ; aiutil.c:971
MOV       ax, [bp-cMaxBuild]        ; ax, [bp-0xa]
MOV       dx, [bp-cMaxBuild+0x2]    ; dx, [bp-0x8]
MOV       [bp-cBuild], ax           ; [bp-0x10], ax
MOV       [bp-cBuild+0x2], dx       ; [bp-0xe], dx
                                    ; aiutil.c:972
JMP       L_1aaa              

L_1a93:                             ; aiutil.c:973
PUSH      [bp-cResMine+0x2]         ; [bp-0x18]
PUSH      [bp-cResMine]             ; [bp-0x1a]
PUSH      [bp-cRes+0x2]             ; [bp-0x1c]
PUSH      [bp-cRes]                 ; [bp-0x1e]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-cBuild], ax           ; [bp-0x10], ax
MOV       [bp-cBuild+0x2], dx       ; [bp-0xe], dx

L_1aaa:                             ; aiutil.c:975
LEA       ax, [bp-rgprod]           ; ax, [bp-0x130]
PUSH      ax                  
CALLF     InitProduction            ; void InitProduction(PROD *rgprod)
ADD       sp, 0x0002          
                                    ; aiutil.c:977
CMP       [bp-cBuild+0x2], 0x0000   ; [bp-0xe], 0x0000
JL        L_1b74              

L_1ac0:
JG        L_1ace              

L_1ac5:
CMP       [bp-cBuild], 0x0000       ; [bp-0x10], 0x0000
JBE       L_1b74              

L_1ace:                             ; aiutil.c:979
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, [bp-cBuild]           ; ax, [bp-0x10]
MOV       dx, [bp-0xe]        
PUSH      ax                  
MOV       ax, 0x0008          
PUSH      ax                  
CALLF     AddItemToQueue            ; void AddItemToQueue(uint16_t iItem, uint16_t cItem, GrobjClass grobj, int16_t mdAddItem)
ADD       sp, 0x0008          
                                    ; aiutil.c:980
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     FinishProduction          ; void FinishProduction(int16_t fWrite)
ADD       sp, 0x0002          
                                    ; aiutil.c:981
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-etaBetterMines]   ; ax, [bp-0x132]
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [sel+0xce]                ; [0x4a24]
PUSH      [sel+0xcc]                ; [0x4a22]
MOV       ax, 0x49ee          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     PszProductionETA          ; char * PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast)
ADD       sp, 0x000e          
                                    ; aiutil.c:982
CMP       [bp-etaBetterMines], 0xffff ; [bp-0x132], 0xffff
JNZ       L_1b29              

L_1b23:                             ; aiutil.c:983
MOV       [bp-etaBetterMines], 0x02bc ; [bp-0x132], 0x02bc

L_1b29:                             ; aiutil.c:986
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       ax, es:[bx+0x4]     
MOV       dx, es:[bx+0x6]     
AND       ax, 0xfc00          
AND       dx, 0xffff          
OR        ax, 0x0001          
OR        dx, 0x0000          
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       es:[bx+0x4], ax     
MOV       es:[bx+0x6], dx     
                                    ; aiutil.c:987
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       ax, es:[bx+0x4]     
MOV       dx, es:[bx+0x6]     
AND       ax, 0x03ff          
AND       dx, 0xfffe          
OR        ax, 0x0c00          
OR        dx, 0x0000          
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       es:[bx+0x4], ax     
MOV       es:[bx+0x6], dx     
                                    ; aiutil.c:989
JMP       L_1b9e              

L_1b74:                             ; aiutil.c:991
MOV       [bp-etaBetterMines], 0x02bc ; [bp-0x132], 0x02bc
                                    ; aiutil.c:994
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0003          
PUSH      ax                  
CALLF     AddItemToQueue            ; void AddItemToQueue(uint16_t iItem, uint16_t cItem, GrobjClass grobj, int16_t mdAddItem)
ADD       sp, 0x0008          
                                    ; aiutil.c:995
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     FinishProduction          ; void FinishProduction(int16_t fWrite)
ADD       sp, 0x0002          

L_1b9e:                             ; aiutil.c:998
MOV       ax, 0x0000          
PUSH      ax                  
LEA       ax, [bp-etaBetterAlchemy] ; ax, [bp-0xc]
PUSH      ax                  
MOV       ax, 0x0001          
PUSH      ax                  
PUSH      [sel+0xce]                ; [0x4a24]
PUSH      [sel+0xcc]                ; [0x4a22]
MOV       ax, 0x49ee          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     PszProductionETA          ; char * PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast)
ADD       sp, 0x000e          
                                    ; aiutil.c:999
CMP       [bp-etaBetterAlchemy], 0xffff ; [bp-0xc], 0xffff
JNZ       L_1bcf              

L_1bca:                             ; aiutil.c:1000
MOV       [bp-etaBetterAlchemy], 0x02bc ; [bp-0xc], 0x02bc

L_1bcf:                             ; aiutil.c:1003
MOV       ax, [bp-etaFirst]         ; ax, [bp-0x12]
CMP       [bp-etaBetterAlchemy], ax ; [bp-0xc], ax
JGE       L_1be6              

L_1bda:
MOV       ax, [bp-etaBetterMines]   ; ax, [bp-0x132]
CMP       [bp-etaBetterAlchemy], ax ; [bp-0xc], ax
JL        L_17a3              

L_1be6:
CMP       [bp-cBuild+0x2], 0x0000   ; [bp-0xe], 0x0000
JG        L_1c03              

L_1bef:
JL        L_17a3              

L_1bf4:
CMP       [bp-cBuild], 0x0001       ; [bp-0x10], 0x0001
JC        L_17a3              

L_1bfa:
JMP       L_1c03              

L_1bfd:                             ; aiutil.c:1006
JMP       L_17a3              

L_1c03:                             ; aiutil.c:1010
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       ax, es:[bx+0x4]     
MOV       dx, es:[bx+0x6]     
AND       ax, 0x03ff          
AND       dx, 0xfffe          
OR        ax, 0x2000          
OR        dx, 0x0000          
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       es:[bx+0x4], ax     
MOV       es:[bx+0x6], dx     
                                    ; aiutil.c:1011
MOV       ax, [bp-etaBetterMines]   ; ax, [bp-0x132]
CMP       [bp-etaFirst], ax         ; [bp-0x12], ax
JL        L_1c4a              

L_1c33:
CMP       [bp-cBuild+0x2], 0x0000   ; [bp-0xe], 0x0000
JG        L_1c8f              

L_1c3c:
JL        L_1c4a              

L_1c41:
CMP       [bp-cBuild], 0x0000       ; [bp-0x10], 0x0000
JA        L_1c8f              

L_1c4a:                             ; aiutil.c:1014
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
SUB       es:[bx+0x3], 0x0001 
                                    ; aiutil.c:1016
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       al, es:[bx+0x3]     
AND       ax, 0x00ff          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0008          
MOV       cx, [sel+0xcc]            ; cx, [0x4a22]
MOV       dx, [sel+0xce]            ; dx, [0x4a24]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
MOV       ax, 0x0004          
MOV       cx, [sel+0xcc]            ; cx, [0x4a22]
MOV       dx, [sel+0xce]            ; dx, [0x4a24]
ADD       cx, ax              
PUSH      dx                  
PUSH      cx                  
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; aiutil.c:1017
JMP       L_17a3              

L_1c8f:                             ; aiutil.c:1022
MOV       ax, [bp-cBuild]           ; ax, [bp-0x10]
MOV       dx, [bp-0xe]        
MOV       dx, 0x0000          
MOV       [bp-0x136], ax      
MOV       [bp-0x134], dx      
MOV       ax, [bp-0x136]      
MOV       dx, [bp-0x134]      
AND       ax, 0x03ff          
AND       dx, 0x0000          
MOV       cx, 0x0000          
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       cx, es:[bx+0x4]     
MOV       bx, es:[bx+0x6]     
AND       cx, 0xfc00          
AND       bx, 0xffff          
OR        cx, ax              
OR        bx, dx              
MOV       [bp-0x13a], cx      
MOV       [bp-0x138], bx      
LES       bx, [sel+0xcc]            ; bx, [0x4a22]
MOV       ax, [bp-0x13a]      
MOV       dx, [bp-0x138]      
MOV       es:[bx+0x4], ax     
MOV       es:[bx+0x6], dx     
                                    ; aiutil.c:1023
JMP       L_17a3              

L_1cef:                             ; aiutil.c:1027
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


