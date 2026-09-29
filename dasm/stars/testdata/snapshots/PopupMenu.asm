; PopupMenu  (popup)
;   addr: 0019:136c  len=1484
;   sig:  int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn)
;   params:
;     HWND             hwnd           [BP+0x6]
;     int16_t          x              [BP+0x8]
;     int16_t          y              [BP+0xa]
;     int16_t          cString        [BP+0xc]
;     int32_t *        rgids          [BP+0xe]
;     char * *         rgsz           [BP+0x10]
;     int16_t          iChecked       [BP+0x12]
;     int16_t          fRightBtn      [BP+0x14]
;   locals:
;     MSG              msg            [BP-0xa6]
;     char *           psz            [BP-0x94]
;     char *           pszT           [BP-0x92]
;     HMENU            hmenuPopup     [BP-0x90]
;     HMENU            hmenuSub       [BP-0x8e]
;     char[128]        szTemp         [BP-0x8c]
;     int16_t          i              [BP-0xc]
;     POINT16          pt             [BP-0xa]
;     int16_t          tpm            [BP-0x6]
;     char *           pszTitle       [BP-0x4]
;     block 0019:15BF  len=0x1AF
;       char *           psz            [BP-0xac]
;       int16_t          fCheckedCur    [BP-0xaa]
;       int16_t          fChecked       [BP-0xa8]
;
;   stats: blocks=1  labels=0

L_136c:                             ; popup.c:487
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x00ac          
PUSH      si                  
PUSH      di                  
                                    ; popup.c:490
MOV       [bp-hmenuSub], 0x0000     ; [bp-0x8e], 0x0000
                                    ; popup.c:499
MOV       ax, [bp+x]                ; ax, [bp+0x8]
MOV       [bp-pt], ax               ; [bp-0xa], ax
                                    ; popup.c:500
MOV       ax, [bp+y]                ; ax, [bp+0xa]
MOV       [bp-pt+0x2], ax           ; [bp-0x8], ax
                                    ; popup.c:502
PUSH      [bp+hwnd]                 ; [bp+0x6]
LEA       ax, [bp-pt]               ; ax, [bp-0xa]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     ClientToScreen            ; void ClientToScreen(HWND arg1, POINT *arg2)
                                    ; popup.c:504
CALLF     CreatePopupMenu           ; HMENU CreatePopupMenu()
MOV       [bp-hmenuPopup], ax       ; [bp-0x90], ax
                                    ; popup.c:505
MOV       [iPopMenuSel], 0xffff     ; [0x4a6a], 0xffff
                                    ; popup.c:507
MOV       [bp-i], 0x0000            ; [bp-0xc], 0x0000
JMP       L_1887              

L_13ad:                             ; popup.c:509
CMP       [bp+rgids], 0x0000        ; [bp+0xe], 0x0000
JZ        L_15ad              

L_13b6:
CMP       [bp+iChecked], 0xfffe     ; [bp+0x12], 0xfffe
JNZ       L_13c8              

L_13bf:
CMP       [bp+rgsz], 0x0000         ; [bp+0x10], 0x0000
JNZ       L_15ad              

L_13c8:                             ; popup.c:511
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
CMP       [bx], 0xffff        
JNZ       L_1401              

L_13dc:
CMP       [bx+0x2], 0xffff    
JNZ       L_1401              

L_13e5:                             ; popup.c:512
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
MOV       ax, 0x0800          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)
                                    ; popup.c:513
JMP       L_1883              

L_1401:                             ; popup.c:515
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x0000          
AND       dx, 0x1000          
CMP       ax, 0x0000          
JNZ       L_1429              

L_1421:
CMP       dx, 0x0000          
JZ        L_1432              

L_1429:                             ; popup.c:516
MOV       [bp-psz], 0x0c09          ; [bp-0x94], 0x0c09
                                    ; popup.c:517
JMP       L_152e              

L_1432:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x0000          
AND       dx, 0x4000          
CMP       ax, 0x0000          
JNZ       L_145a              

L_1452:
CMP       dx, 0x0000          
JZ        L_147b              

L_145a:                             ; popup.c:518
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
PUSH      ax                  
CALLF     PszGetCompressedString    ; char * PszGetCompressedString(StringId ids)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x94], ax
                                    ; popup.c:519
JMP       L_152e              

L_147b:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x0000          
AND       dx, 0x2000          
CMP       ax, 0x0000          
JNZ       L_14a3              

L_149b:
CMP       dx, 0x0000          
JZ        L_14c4              

L_14a3:                             ; popup.c:520
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
PUSH      ax                  
CALLF     PszGetThingName           ; char * PszGetThingName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x94], ax
                                    ; popup.c:521
JMP       L_152e              

L_14c4:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
AND       ax, 0x0000          
AND       dx, 0x8000          
CMP       ax, 0x0000          
JNZ       L_14ec              

L_14e4:
CMP       dx, 0x0000          
JZ        L_1510              

L_14ec:                             ; popup.c:522
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
OR        ax, 0x8000          
PUSH      ax                  
CALLF     PszGetFleetName           ; char * PszGetFleetName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x94], ax
                                    ; popup.c:523
JMP       L_152e              

L_1510:                             ; popup.c:524
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
PUSH      ax                  
CALLF     PszGetPlanetName          ; char * PszGetPlanetName(int16_t id)
ADD       sp, 0x0002          
MOV       [bp-psz], ax              ; [bp-0x94], ax

L_152e:                             ; popup.c:526
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       [bp-pszT], ax             ; [bp-0x92], ax

L_1536:                             ; popup.c:527
MOV       bx, [bp-psz]              ; bx, [bp-0x94]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_1573              

L_1545:                             ; popup.c:529
MOV       bx, [bp-psz]              ; bx, [bp-0x94]
ADD       [bp-psz], 0x0001          ; [bp-0x94], 0x0001
MOV       al, [bx]            
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], al            
CBW       ax, al              
CMP       ax, 0x0026          
JNZ       L_1536              

L_1564:                             ; popup.c:530
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], 0x0026        

L_1570:                             ; popup.c:531
JMP       L_1536              

L_1573:                             ; popup.c:532
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
MOV       [bx], 0x0000        
                                    ; popup.c:536
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
MOV       ax, [bp+iChecked]         ; ax, [bp+0x12]
CMP       [bp-i], ax                ; [bp-0xc], ax
JNZ       L_158f              

L_1589:
MOV       ax, 0x0008          
JMP       L_1592              

L_158f:
MOV       ax, 0x0000          

L_1592:
OR        ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xc]
ADD       ax, 0x3a98          
PUSH      ax                  
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)

L_15aa:                             ; popup.c:539
JMP       L_1883              

L_15ad:                             ; popup.c:542
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
CMP       [bx], 0x0000        
JNZ       L_1771              

L_15bf:                             ; popup.c:547
MOV       ax, [bp-i]                ; ax, [bp-0xc]
ADD       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [bp-pszTitle], ax         ; [bp-0x4], ax
                                    ; popup.c:548
CMP       [bp+rgids], 0x0000        ; [bp+0xe], 0x0000
JZ        L_15f1              

L_15da:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
ADD       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
JMP       L_15f4              

L_15f1:
MOV       ax, 0x0000          

L_15f4:
MOV       [bp-fChecked], ax         ; [bp-0xa8], ax
                                    ; popup.c:550
CALLF     CreatePopupMenu           ; HMENU CreatePopupMenu()
MOV       [bp-hmenuSub], ax         ; [bp-0x8e], ax
                                    ; popup.c:551
ADD       [bp-i], 0x0002            ; [bp-0xc], 0x0002
                                    ; popup.c:553
JMP       L_1738              

L_1608:                             ; popup.c:555
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
CMP       [bx], 0x0000        
JZ        L_1743              

L_161d:                             ; popup.c:557
CMP       [bp+rgids], 0x0000        ; [bp+0xe], 0x0000
JNZ       L_1649              

L_1626:                             ; popup.c:559
MOV       ax, [bp+iChecked]         ; ax, [bp+0x12]
CMP       [bp-i], ax                ; [bp-0xc], ax
JNZ       L_1637              

L_1631:
MOV       ax, 0x0001          
JMP       L_163a              

L_1637:
MOV       ax, 0x0000          

L_163a:
MOV       [bp-fCheckedCur], ax      ; [bp-0xaa], ax
                                    ; popup.c:560
MOV       ax, [bp-fCheckedCur]      ; ax, [bp-0xaa]
OR        [bp-fChecked], ax         ; [bp-0xa8], ax
                                    ; popup.c:562
JMP       L_165e              

L_1649:                             ; popup.c:563
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       [bp-fCheckedCur], ax      ; [bp-0xaa], ax

L_165e:                             ; popup.c:565
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       bx, [bx]            
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0xffff          
JNZ       L_16a9              

L_1675:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       bx, [bx]            
MOV       al, [bx+0x1]        
CBW       ax, al              
CMP       ax, 0x0000          
JNZ       L_16a9              

L_168d:                             ; popup.c:566
PUSH      [bp-hmenuSub]             ; [bp-0x8e]
MOV       ax, 0x0800          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)
                                    ; popup.c:567
JMP       L_1734              

L_16a9:                             ; popup.c:569
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       [bp-pszT], ax             ; [bp-0x92], ax
                                    ; popup.c:570
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [bp-psz], ax              ; [bp-0xac], ax

L_16c1:                             ; popup.c:572
MOV       bx, [bp-psz]              ; bx, [bp-0xac]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_16fe              

L_16d0:                             ; popup.c:574
MOV       bx, [bp-psz]              ; bx, [bp-0xac]
ADD       [bp-psz], 0x0001          ; [bp-0xac], 0x0001
MOV       al, [bx]            
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], al            
CBW       ax, al              
CMP       ax, 0x0026          
JNZ       L_16c1              

L_16ef:                             ; popup.c:575
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], 0x0026        

L_16fb:                             ; popup.c:576
JMP       L_16c1              

L_16fe:                             ; popup.c:577
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
MOV       [bx], 0x0000        
                                    ; popup.c:581
PUSH      [bp-hmenuSub]             ; [bp-0x8e]
CMP       [bp-fCheckedCur], 0x0000  ; [bp-0xaa], 0x0000
JZ        L_1719              

L_1713:
MOV       ax, 0x0008          
JMP       L_171c              

L_1719:
MOV       ax, 0x0000          

L_171c:
OR        ax, 0x0000          
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xc]
ADD       ax, 0x3a98          
PUSH      ax                  
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)

L_1734:                             ; popup.c:583
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_1738:
MOV       ax, [bp+cString]          ; ax, [bp+0xc]
CMP       [bp-i], ax                ; [bp-0xc], ax
JL        L_1608              

L_1743:                             ; popup.c:586
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
CMP       [bp-fChecked], 0x0000     ; [bp-0xa8], 0x0000
JZ        L_1757              

L_1751:
MOV       ax, 0x0008          
JMP       L_175a              

L_1757:
MOV       ax, 0x0000          

L_175a:
OR        ax, 0x0010          
PUSH      ax                  
PUSH      [bp-hmenuSub]             ; [bp-0x8e]
MOV       ax, [bp-pszTitle]         ; ax, [bp-0x4]
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)
                                    ; popup.c:588
JMP       L_1883              

L_1771:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       bx, [bx]            
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0xffff          
JNZ       L_17bc              

L_1788:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       bx, [bx]            
MOV       al, [bx+0x1]        
CBW       ax, al              
CMP       ax, 0x0000          
JNZ       L_17bc              

L_17a0:                             ; popup.c:589
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
MOV       ax, 0x0800          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)
                                    ; popup.c:590
JMP       L_1883              

L_17bc:                             ; popup.c:592
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       [bp-pszT], ax             ; [bp-0x92], ax
                                    ; popup.c:593
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
MOV       bx, [bp+rgsz]             ; bx, [bp+0x10]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       [bp-psz], ax              ; [bp-0x94], ax

L_17d4:                             ; popup.c:595
MOV       bx, [bp-psz]              ; bx, [bp-0x94]
MOV       al, [bx]            
CBW       ax, al              
CMP       ax, 0x0000          
JZ        L_1811              

L_17e3:                             ; popup.c:597
MOV       bx, [bp-psz]              ; bx, [bp-0x94]
ADD       [bp-psz], 0x0001          ; [bp-0x94], 0x0001
MOV       al, [bx]            
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], al            
CBW       ax, al              
CMP       ax, 0x0026          
JNZ       L_17d4              

L_1802:                             ; popup.c:598
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
ADD       [bp-pszT], 0x0001         ; [bp-0x92], 0x0001
MOV       [bx], 0x0026        

L_180e:                             ; popup.c:599
JMP       L_17d4              

L_1811:                             ; popup.c:600
MOV       bx, [bp-pszT]             ; bx, [bp-0x92]
MOV       [bx], 0x0000        
                                    ; popup.c:605
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
CMP       [bp+iChecked], 0xfffe     ; [bp+0x12], 0xfffe
JNZ       L_1839              

L_1825:
MOV       ax, [bp-i]                ; ax, [bp-0xc]
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, [bp+rgids]            ; bx, [bp+0xe]
ADD       bx, ax              
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
JMP       L_184f              

L_1839:
MOV       ax, [bp+iChecked]         ; ax, [bp+0x12]
CMP       [bp-i], ax                ; [bp-0xc], ax
JNZ       L_184b              

L_1844:
MOV       ax, 0x0001          
CWD       dx, ax              
JMP       L_184f              

L_184b:
MOV       ax, 0x0000          
CWD       dx, ax              

L_184f:
OR        ax, 0x0000          
OR        dx, 0x0000          
CMP       ax, 0x0000          
JNZ       L_1865              

L_185d:
CMP       dx, 0x0000          
JZ        L_186b              

L_1865:
MOV       ax, 0x0008          
JMP       L_186e              

L_186b:
MOV       ax, 0x0000          

L_186e:
PUSH      ax                  
MOV       ax, [bp-i]                ; ax, [bp-0xc]
ADD       ax, 0x3a98          
PUSH      ax                  
LEA       ax, [bp-szTemp]           ; ax, [bp-0x8c]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
CALLF     AppendMenu                ; int16_t AppendMenu(HMENU arg1, uint16_t arg2, UINT_PTR arg3, LPCSTR arg4)

L_1883:                             ; popup.c:608
ADD       [bp-i], 0x0001            ; [bp-0xc], 0x0001

L_1887:
MOV       ax, [bp+cString]          ; ax, [bp+0xc]
CMP       [bp-i], ax                ; [bp-0xc], ax
JL        L_13ad              

L_1892:                             ; popup.c:610
CMP       [bp+fRightBtn], 0x0000    ; [bp+0x14], 0x0000
JZ        L_18a3              

L_189b:                             ; popup.c:611
MOV       [bp-tpm], 0x0002          ; [bp-0x6], 0x0002
                                    ; popup.c:612
JMP       L_18a8              

L_18a3:                             ; popup.c:613
MOV       [bp-tpm], 0x0000          ; [bp-0x6], 0x0000

L_18a8:                             ; popup.c:616
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
MOV       ax, [bp-tpm]              ; ax, [bp-0x6]
PUSH      ax                  
PUSH      [bp-pt]                   ; [bp-0xa]
PUSH      [bp-pt+0x2]               ; [bp-0x8]
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
CALLF     TrackPopupMenu            ; int16_t TrackPopupMenu(HMENU arg1, uint16_t arg2, int16_t arg3, int16_t arg4, int16_t arg5, HWND arg6, RECT *arg7)
                                    ; popup.c:617
PUSH      [bp-hmenuPopup]           ; [bp-0x90]
CALLF     DestroyMenu               ; int16_t DestroyMenu(HMENU arg1)
                                    ; popup.c:618
CMP       [bp-hmenuSub], 0x0000     ; [bp-0x8e], 0x0000
JZ        L_18e7              

L_18de:                             ; popup.c:619
PUSH      [bp-hmenuSub]             ; [bp-0x8e]
CALLF     DestroyMenu               ; int16_t DestroyMenu(HMENU arg1)

L_18e7:                             ; popup.c:622
LEA       ax, [bp-msg]              ; ax, [bp-0xa6]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [hwndFrame]               ; [0x258c]
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x0111          
PUSH      ax                  
MOV       ax, 0x0002          
PUSH      ax                  
CALLF     PeekMessage               ; int16_t PeekMessage(MSG *arg1, HWND arg2, uint16_t arg3, uint16_t arg4, uint16_t arg5)
CMP       ax, 0x0000          
JZ        L_192c              

L_190c:                             ; popup.c:624
CMP       [bp-msg+0x4], 0x3a98      ; [bp-0xa2], 0x3a98
JC        L_192c              

L_1917:
CMP       [bp-msg+0x4], 0x3afc      ; [bp-0xa2], 0x3afc
JNC       L_192c              

L_1922:                             ; popup.c:625
MOV       ax, [bp-msg+0x4]          ; ax, [bp-0xa2]
ADD       ax, 0xc568          
MOV       [iPopMenuSel], ax         ; [0x4a6a], ax

L_192c:                             ; popup.c:627
MOV       ax, [iPopMenuSel]         ; ax, [0x4a6a]

L_1932:                             ; popup.c:628
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


