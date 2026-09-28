; EnsureAis  (mdi)
;   addr: 0005:56bc  len=477
;   sig:  void EnsureAis()
;   locals:
;     MDPLR[16]        rgmdplr        [BP-0x2e]
;     int16_t          iPlayer        [BP-0xe]
;     int16_t          fSubmitSav     [BP-0xc]
;     int16_t          fWorkDone      [BP-0xa]
;     int16_t          fOpened        [BP-0x8]
;     int16_t          fErrSav        [BP-0x6]
;     int16_t          fHostSav       [BP-0x4]
;
;   stats: blocks=0  labels=0

L_56bc:                             ; mdi.c:2771
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x002e          
PUSH      si                  
PUSH      di                  
                                    ; mdi.c:2773
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
MOV       [bp-fSubmitSav], ax       ; [bp-0xc], ax
                                    ; mdi.c:2774
MOV       [bp-fWorkDone], 0x0000    ; [bp-0xa], 0x0000
                                    ; mdi.c:2780
MOV       cx, 0x000a          
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_5893              

L_56f1:                             ; mdi.c:2783
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
MOV       [bp-fHostSav], ax         ; [bp-0x4], ax
                                    ; mdi.c:2784
MOV       ax, [gd]                  ; ax, [0x07ca]
SHR       ax, 0x0001          
SHR       ax, 0x0001          
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JNZ       L_5729              

L_5714:                             ; mdi.c:2786
CALLF     DestroyCurGame            ; void DestroyCurGame()
                                    ; mdi.c:2787
MOV       ax, 0x03f0          
PUSH      ax                  
MOV       ax, 0x56a2          
PUSH      ax                  
CALLF     FLoadGame                 ; int16_t FLoadGame(char *pszFileName, char *pszExt)
ADD       sp, 0x0004          

L_5729:                             ; mdi.c:2790
MOV       [bp-iPlayer], 0x0000      ; [bp-0xe], 0x0000
JMP       L_5735              

L_5731:
ADD       [bp-iPlayer], 0x0001      ; [bp-0xe], 0x0001

L_5735:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-iPlayer], ax          ; [bp-0xe], ax
JGE       L_575d              

L_5740:                             ; mdi.c:2791
MOV       ax, 0x00c0          
IMUL      [bp-iPlayer]              ; [bp-0xe]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, [bx+0x6]        
MOV       dx, [bp-iPlayer]          ; dx, [bp-0xe]
SHL       dx, 0x0001          
LEA       bx, [bp-0x2e]       
ADD       bx, dx              
MOV       [bx], ax            
JMP       L_5731              

L_575d:                             ; mdi.c:2793
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xffef          
OR        ax, 0x0010          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; mdi.c:2794
MOV       ax, [fFileErrSilent]      ; ax, [0x074c]
MOV       [bp-fErrSav], ax          ; [bp-0x6], ax
                                    ; mdi.c:2795
MOV       [fFileErrSilent], 0x0001  ; [0x074c], 0x0001
                                    ; mdi.c:2797
MOV       [bp-iPlayer], 0x0000      ; [bp-0xe], 0x0000
JMP       L_5781              

L_577d:
ADD       [bp-iPlayer], 0x0001      ; [bp-0xe], 0x0001

L_5781:
MOV       ax, [game+0x8]            ; ax, [0x0078]
CMP       [bp-iPlayer], ax          ; [bp-0xe], ax
JGE       L_5848              

L_578c:                             ; mdi.c:2799
MOV       ax, 0x0154          
PUSH      ax                  
MOV       ax, [bp-iPlayer]          ; ax, [bp-0xe]
ADD       ax, 0x0001          
PUSH      ax                  
PUSH      [game+0x8]                ; [0x0078]
CALLF     MulDiv                    ; int16_t MulDiv(int16_t arg1, int16_t arg2, int16_t arg3)
PUSH      ax                  
CALLF     UpdateProgressGauge       ; void UpdateProgressGauge(int16_t pctX10)
ADD       sp, 0x0002          
                                    ; mdi.c:2801
MOV       ax, [bp-iPlayer]          ; ax, [bp-0xe]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2e]       
ADD       bx, ax              
MOV       ax, [bx]            
MOV       cx, 0x0009          
SHR       ax, cx              
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_577d              

L_57c5:                             ; mdi.c:2803
MOV       [bp-fWorkDone], 0x0001    ; [bp-0xa], 0x0001
                                    ; mdi.c:2804
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xfffd          
OR        ax, 0x0002          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; mdi.c:2805
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xfff7          
OR        ax, 0x0008          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; mdi.c:2806
MOV       ax, 0x0020          
PUSH      ax                  
PUSH      [bp-iPlayer]              ; [bp-0xe]
MOV       ax, 0x0001          
PUSH      ax                  
CALLF     FOpenFile                 ; int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md)
ADD       sp, 0x0006          
MOV       [bp-fOpened], ax          ; [bp-0x8], ax
                                    ; mdi.c:2807
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xfffd          
OR        ax, 0x0000          
MOV       [gd], ax                  ; [0x07ca], ax
                                    ; mdi.c:2808
MOV       ax, [bp-fHostSav]         ; ax, [bp-0x4]
AND       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [gd]                  ; cx, [0x07ca]
AND       cx, 0xfff7          
OR        cx, ax              
MOV       [gd], cx                  ; [0x07ca], cx
                                    ; mdi.c:2810
CMP       [bp-fOpened], 0x0000      ; [bp-0x8], 0x0000
JZ        L_582e              

L_5826:                             ; mdi.c:2811
CALLF     StreamClose               ; void StreamClose()
                                    ; mdi.c:2812
JMP       L_577d              

L_582e:                             ; mdi.c:2814
MOV       ax, [bp-iPlayer]          ; ax, [bp-0xe]
SHL       ax, 0x0001          
LEA       bx, [bp-0x2e]       
ADD       bx, ax              
PUSH      [bx]                
PUSH      [bp-iPlayer]              ; [bp-0xe]
CALLF     DoAiTurn                  ; void DoAiTurn(int16_t iPlayer, uint16_t wMdPlr)
ADD       sp, 0x0004          

L_5845:                             ; mdi.c:2817
JMP       L_577d              

L_5848:                             ; mdi.c:2818
MOV       ax, [bp-fSubmitSav]       ; ax, [bp-0xc]
AND       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       cx, [gd]                  ; cx, [0x07ca]
AND       cx, 0xffef          
OR        cx, ax              
MOV       [gd], cx                  ; [0x07ca], cx
                                    ; mdi.c:2819
CMP       [bp-fWorkDone], 0x0000    ; [bp-0xa], 0x0000
JZ        L_5881              

L_586c:                             ; mdi.c:2821
CALLF     DestroyCurGame            ; void DestroyCurGame()
                                    ; mdi.c:2822
MOV       ax, 0x03f4          
PUSH      ax                  
MOV       ax, 0x56a2          
PUSH      ax                  
CALLF     FLoadGame                 ; int16_t FLoadGame(char *pszFileName, char *pszExt)
ADD       sp, 0x0004          

L_5881:                             ; mdi.c:2824
MOV       ax, [bp-fErrSav]          ; ax, [bp-0x6]
MOV       [fFileErrSilent], ax      ; [0x074c], ax
                                    ; mdi.c:2825
MOV       ax, [gd]                  ; ax, [0x07ca]
AND       ax, 0xfbff          
OR        ax, 0x0400          
MOV       [gd], ax                  ; [0x07ca], ax

L_5893:                             ; mdi.c:2826
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


