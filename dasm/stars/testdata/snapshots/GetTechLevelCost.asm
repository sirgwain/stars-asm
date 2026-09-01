; GetTechLevelCost  (research)
;   addr: 001c:1dba  len=252
;   sig:  int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr)
;   params:
;     int16_t          iTech          [BP+0x6]
;     int16_t          iLevel         [BP+0x8]
;     int16_t          iplr           [BP+0xa]
;   locals:
;     int16_t          cTech          [BP-0xa]
;     int16_t          i              [BP-0x8]
;     int32_t          lCost          [BP-0x6]
;
;   stats: blocks=0  labels=0

L_1dba:                             ; research.c:698
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x000a          
PUSH      si                  
PUSH      di                  
                                    ; research.c:700
MOV       [bp-cTech], 0x0000        ; [bp-0xa], 0x0000
                                    ; research.c:705
MOV       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JMP       L_1def              

L_1dd0:                             ; research.c:706
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       bx, 0x59a2          
ADD       bx, ax              
MOV       ax, 0x001a          
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0x8]
ADD       bx, ax              
MOV       al, [bx]            
CBW       ax, al              
ADD       [bp-cTech], ax            ; [bp-0xa], ax
ADD       [bp-i], 0x0001            ; [bp-0x8], 0x0001

L_1def:
CMP       [bp-i], 0x0006            ; [bp-0x8], 0x0006
JL        L_1dd0              

L_1df8:                             ; research.c:708
MOV       ax, 0x000a          
IMUL      [bp-cTech]                ; [bp-0xa]
CWD       dx, ax              
MOV       bx, [bp+iLevel]           ; bx, [bp+0x8]
SHL       bx, 0x0001          
SHL       bx, 0x0001          
ADD       ax, cs:[bx+0x1d4e]  
ADC       dx, cs:[bx+0x1d50]  
MOV       [bp-lCost], ax            ; [bp-0x6], ax
MOV       [bp-lCost+0x2], dx        ; [bp-0x4], dx
                                    ; research.c:710
MOV       ax, [bp+iTech]            ; ax, [bp+0x6]
ADD       ax, 0x0008          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [bp+iplr]                 ; [bp+0xa]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
ADD       ax, 0xffff          
MOV       [bp-i], ax                ; [bp-0x8], ax
                                    ; research.c:711
CMP       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JZ        L_1e83              

L_1e40:                             ; research.c:713
CMP       [bp-i], 0x0000            ; [bp-0x8], 0x0000
JGE       L_1e6a              

L_1e49:                             ; research.c:714
MOV       cx, 0x0002          
MOV       ax, [bp-lCost]            ; ax, [bp-0x6]
MOV       dx, [bp-lCost+0x2]        ; dx, [bp-0x4]
CALLF     __aFlshr                  ; int32_t __aFlshr(int32_t val, uint16_t shift)
MOV       cx, [bp-lCost]            ; cx, [bp-0x6]
MOV       bx, [bp-lCost+0x2]        ; bx, [bp-0x4]
SUB       cx, ax              
SBB       bx, dx              
ADD       [bp-lCost], cx            ; [bp-0x6], cx
ADC       [bp-lCost+0x2], bx        ; [bp-0x4], bx
                                    ; research.c:715
JMP       L_1e83              

L_1e6a:                             ; research.c:716
MOV       ax, 0x0002          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-lCost+0x2]            ; [bp-0x4]
PUSH      [bp-lCost]                ; [bp-0x6]
CALLF     __aFldiv                  ; int32_t __aFldiv(int32_t a, int32_t b)
MOV       [bp-lCost], ax            ; [bp-0x6], ax
MOV       [bp-lCost+0x2], dx        ; [bp-0x4], dx

L_1e83:                             ; research.c:718
MOV       ax, [game+0x10]           ; ax, [0x0080]
SHR       ax, 0x0001          
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_1ea7              

L_1e93:                             ; research.c:719
MOV       cx, 0x0001          
MOV       ax, [bp-lCost]            ; ax, [bp-0x6]
MOV       dx, [bp-lCost+0x2]        ; dx, [bp-0x4]
CALLF     __aFlshl                  ; int32_t __aFlshl(int32_t val, uint16_t shift)
MOV       [bp-lCost], ax            ; [bp-0x6], ax
MOV       [bp-lCost+0x2], dx        ; [bp-0x4], dx

L_1ea7:                             ; research.c:721
MOV       ax, [bp-lCost]            ; ax, [bp-0x6]
MOV       dx, [bp-lCost+0x2]        ; dx, [bp-0x4]

L_1eb0:                             ; research.c:722
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


