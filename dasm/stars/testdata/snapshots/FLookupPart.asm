; FLookupPart  (parts)
;   addr: 0002:524e  len=3695
;   sig:  int16_t FLookupPart(PART *ppart)
;   params:
;     PART *           ppart          [BP+0x6]
;   locals:
;     HS               hs             [BP-0x8]
;     int16_t          raMajor        [BP-0x4]
;
;   stats: blocks=0  labels=0

L_524e:                             ; parts.c:72
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x0008          
PUSH      si                  
PUSH      di                  
                                    ; parts.c:74
MOV       ax, 0x000e          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceStat               ; int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat)
ADD       sp, 0x0004          
MOV       [bp-raMajor], ax          ; [bp-0x4], ax
                                    ; parts.c:78
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       [bp-hs], ax               ; [bp-0x8], ax
MOV       [bp-hs+0x2], dx           ; [bp-0x6], dx
                                    ; parts.c:81
MOV       ax, [bp-hs]               ; ax, [bp-0x8]
JMP       L_6019              

L_5287:                             ; parts.c:84
MOV       ax, 0x0000          
JMP       L_60b7              

L_528d:                             ; parts.c:86
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0010          
JC        L_52a1              

L_529b:                             ; parts.c:87
MOV       ax, 0x0000          
JMP       L_60b7              

L_52a1:                             ; parts.c:88
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x004e          
IMUL      cx                  
MOV       cx, 0x0000          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:89
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_52c6:                             ; parts.c:91
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JNZ       L_52e6              

L_52d4:
CMP       [bp-raMajor], 0x0000      ; [bp-0x4], 0x0000
JZ        L_52e6              

L_52dd:                             ; parts.c:92
MOV       ax, 0xffff          
JMP       L_60b7              

L_52e6:                             ; parts.c:95
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000b          
JC        L_5302              

L_52f4:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JBE       L_5310              

L_5302:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000a          
JNZ       L_533a              

L_5310:
MOV       ax, 0x0007          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_533a              

L_5331:                             ; parts.c:96
MOV       ax, 0xffff          
JMP       L_60b7              

L_533a:                             ; parts.c:98
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JZ        L_5356              

L_5348:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JNZ       L_5380              

L_5356:
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_5380              

L_5377:                             ; parts.c:99
MOV       ax, 0xffff          
JMP       L_60b7              

L_5380:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_53b8              

L_538e:
MOV       ax, 0x0007          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_53b8              

L_53af:                             ; parts.c:101
MOV       ax, 0xffff          
JMP       L_60b7              

L_53b8:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_53cb:                             ; parts.c:103
MOV       ax, 0xffff          
JMP       L_60b7              

L_53d1:                             ; parts.c:105
JMP       L_609c              

L_53d4:                             ; parts.c:107
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000a          
JC        L_53e8              

L_53e2:                             ; parts.c:108
MOV       ax, 0x0000          
JMP       L_60b7              

L_53e8:                             ; parts.c:109
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x0ae8          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:110
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JNZ       L_5423              

L_5411:
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_5423              

L_541a:                             ; parts.c:111
MOV       ax, 0xffff          
JMP       L_60b7              

L_5423:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JNZ       L_5443              

L_5431:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_5443              

L_543a:                             ; parts.c:113
MOV       ax, 0xffff          
JMP       L_60b7              

L_5443:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5456:                             ; parts.c:115
MOV       ax, 0xffff          
JMP       L_60b7              

L_545c:                             ; parts.c:116
JMP       L_609c              

L_545f:                             ; parts.c:118
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0020          
JC        L_5473              

L_546d:                             ; parts.c:119
MOV       ax, 0x0000          
JMP       L_60b7              

L_5473:                             ; parts.c:120
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x008f          
IMUL      cx                  
MOV       cx, 0x29f0          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:121
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5498:                             ; parts.c:124
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000e          
JZ        L_54b4              

L_54a6:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x001f          
JNZ       L_54c6              

L_54b4:
CMP       [bp-raMajor], 0x0000      ; [bp-0x4], 0x0000
JZ        L_54c6              

L_54bd:                             ; parts.c:125
MOV       ax, 0xffff          
JMP       L_60b7              

L_54c6:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0019          
JZ        L_54e2              

L_54d4:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JNZ       L_54f4              

L_54e2:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_54f4              

L_54eb:                             ; parts.c:127
MOV       ax, 0xffff          
JMP       L_60b7              

L_54f4:                             ; parts.c:130
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0016          
JZ        L_552c              

L_5502:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0017          
JZ        L_552c              

L_5510:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0014          
JZ        L_552c              

L_551e:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0018          
JNZ       L_5556              

L_552c:
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_5556              

L_554d:                             ; parts.c:131
MOV       ax, 0xffff          
JMP       L_60b7              

L_5556:                             ; parts.c:133
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0014          
JZ        L_5580              

L_5564:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0016          
JZ        L_5580              

L_5572:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0018          
JNZ       L_55aa              

L_5580:
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_55aa              

L_55a1:                             ; parts.c:134
MOV       ax, 0xffff          
JMP       L_60b7              

L_55aa:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000a          
JZ        L_55c6              

L_55b8:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JNZ       L_55d8              

L_55c6:
CMP       [bp-raMajor], 0x0002      ; [bp-0x4], 0x0002
JZ        L_55d8              

L_55cf:                             ; parts.c:136
MOV       ax, 0xffff          
JMP       L_60b7              

L_55d8:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000c          
JNZ       L_55f8              

L_55e6:
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_55f8              

L_55ef:                             ; parts.c:138
MOV       ax, 0xffff          
JMP       L_60b7              

L_55f8:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0012          
JNZ       L_5618              

L_5606:
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_5618              

L_560f:                             ; parts.c:140
MOV       ax, 0xffff          
JMP       L_60b7              

L_5618:                             ; parts.c:142
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x001b          
JZ        L_5634              

L_5626:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x001c          
JNZ       L_5646              

L_5634:
CMP       [bp-raMajor], 0x0005      ; [bp-0x4], 0x0005
JZ        L_5646              

L_563d:                             ; parts.c:143
MOV       ax, 0xffff          
JMP       L_60b7              

L_5646:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5659:                             ; parts.c:145
MOV       ax, 0xffff          
JMP       L_60b7              

L_565f:                             ; parts.c:147
JMP       L_609c              

L_5662:                             ; parts.c:149
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JC        L_5676              

L_5670:                             ; parts.c:150
MOV       ax, 0x0000          
JMP       L_60b7              

L_5676:                             ; parts.c:151
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x008f          
IMUL      cx                  
MOV       cx, 0x4872          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:152
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_569b:                             ; parts.c:155
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0001          
JZ        L_56b7              

L_56a9:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JNZ       L_56de              

L_56b7:
MOV       ax, 0x0003          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_56de              

L_56d8:                             ; parts.c:156
MOV       ax, 0xffff          
JMP       L_60b7              

L_56de:                             ; parts.c:157
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JNZ       L_609c              

L_56ec:
CMP       [bp-raMajor], 0x0008      ; [bp-0x4], 0x0008
JZ        L_609c              

L_56f5:                             ; parts.c:158
MOV       ax, 0xffff          
JMP       L_60b7              

L_56fb:                             ; parts.c:160
JMP       L_609c              

L_56fe:                             ; parts.c:162
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000c          
JC        L_5712              

L_570c:                             ; parts.c:163
MOV       ax, 0x0000          
JMP       L_60b7              

L_5712:                             ; parts.c:164
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x04e0          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:165
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_574d              

L_573b:
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_574d              

L_5744:                             ; parts.c:166
MOV       ax, 0xffff          
JMP       L_60b7              

L_574d:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JNZ       L_576d              

L_575b:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_576d              

L_5764:                             ; parts.c:168
MOV       ax, 0xffff          
JMP       L_60b7              

L_576d:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5780:                             ; parts.c:170
MOV       ax, 0xffff          
JMP       L_60b7              

L_5786:                             ; parts.c:171
JMP       L_609c              

L_5789:                             ; parts.c:173
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0011          
JC        L_579d              

L_5797:                             ; parts.c:174
MOV       ax, 0x0000          
JMP       L_60b7              

L_579d:                             ; parts.c:175
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x0d04          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:176
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_57c2:                             ; parts.c:178
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_57db              

L_57d5:                             ; parts.c:179
MOV       ax, 0xffff          
JMP       L_60b7              

L_57db:                             ; parts.c:181
MOV       ax, [bp-0x6]        
AND       ax, 0x00ff          
JMP       L_5841              
                                    ; parts.c:185
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_57f3              

L_57ed:                             ; parts.c:186
MOV       ax, 0xffff          
JMP       L_60b7              

L_57f3:                             ; parts.c:187
JMP       L_5874              
                                    ; parts.c:189
CMP       [bp-raMajor], 0x0005      ; [bp-0x4], 0x0005
JZ        L_5805              

L_57ff:                             ; parts.c:190
MOV       ax, 0xffff          
JMP       L_60b7              

L_5805:                             ; parts.c:191
JMP       L_5874              
                                    ; parts.c:193
CMP       [bp-raMajor], 0x0007      ; [bp-0x4], 0x0007
JZ        L_5817              

L_5811:                             ; parts.c:194
MOV       ax, 0xffff          
JMP       L_60b7              

L_5817:                             ; parts.c:195
JMP       L_5874              
                                    ; parts.c:197
CMP       [bp-raMajor], 0x0000      ; [bp-0x4], 0x0000
JZ        L_5829              

L_5823:                             ; parts.c:198
MOV       ax, 0xffff          
JMP       L_60b7              

L_5829:                             ; parts.c:199
JMP       L_5874              
                                    ; parts.c:203
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_609c              

L_5835:                             ; parts.c:204
MOV       ax, 0xffff          
JMP       L_60b7              

L_583b:                             ; parts.c:205
JMP       L_609c              

L_5841:
CMP       ax, 0x0010          
JA        L_609c              

L_5849:
SHL       ax, 0x0001          
MOV       bx, ax              
JMP       L_ffffffff          
DW        0x57e4                    ; L_57e4
DW        0x5874                    ; L_5874
DW        0x5874                    ; L_5874
DW        0x57e4                    ; L_57e4
DW        0x5874                    ; L_5874
DW        0x5874                    ; L_5874
DW        0x5874                    ; L_5874
DW        0x5874                    ; L_5874
DW        0x582c                    ; L_582c
DW        0x5874                    ; L_5874
DW        0x5874                    ; L_5874
DW        0x582c                    ; L_582c
DW        0x5874                    ; L_5874
DW        0x581a                    ; L_581a
DW        0x57f6                    ; L_57f6
DW        0x582c                    ; L_582c
DW        0x5808                    ; L_5808

L_5874:                             ; parts.c:208
JMP       L_609c              

L_5877:                             ; parts.c:211
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000b          
JC        L_588b              

L_5885:                             ; parts.c:212
MOV       ax, 0x0000          
JMP       L_60b7              

L_588b:                             ; parts.c:213
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x109a          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:214
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_58b0:                             ; parts.c:216
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_58c9              

L_58c3:                             ; parts.c:217
MOV       ax, 0xffff          
JMP       L_60b7              

L_58c9:                             ; parts.c:219
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JNZ       L_58e9              

L_58d7:
CMP       [bp-raMajor], 0x0008      ; [bp-0x4], 0x0008
JNZ       L_58e9              

L_58e0:                             ; parts.c:220
MOV       ax, 0xffff          
JMP       L_60b7              

L_58e9:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0001          
JNZ       L_609c              

L_58f7:
CMP       [bp-raMajor], 0x0008      ; [bp-0x4], 0x0008
JZ        L_609c              

L_5900:                             ; parts.c:222
MOV       ax, 0xffff          
JMP       L_60b7              

L_5906:                             ; parts.c:224
JMP       L_609c              

L_5909:                             ; parts.c:226
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0010          
JC        L_591d              

L_5917:                             ; parts.c:227
MOV       ax, 0x0000          
JMP       L_60b7              

L_591d:                             ; parts.c:228
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0038          
IMUL      cx                  
MOV       cx, 0x4d8a          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:229
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5942:                             ; parts.c:231
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JC        L_598c              

L_5950:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JA        L_598c              

L_595e:                             ; parts.c:234
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JZ        L_609c              

L_596c:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000c          
JZ        L_609c              

L_597a:
CMP       [bp-raMajor], 0x0006      ; [bp-0x4], 0x0006
JZ        L_609c              

L_5983:                             ; parts.c:235
MOV       ax, 0xffff          
JMP       L_60b7              

L_5989:                             ; parts.c:237
JMP       L_609c              

L_598c:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JC        L_609c              

L_599a:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JA        L_609c              

L_59a8:                             ; parts.c:239
CMP       [bp-raMajor], 0x0007      ; [bp-0x4], 0x0007
JZ        L_59d3              

L_59b1:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0001          
JZ        L_59cd              

L_59bf:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JC        L_59d3              

L_59cd:                             ; parts.c:240
MOV       ax, 0xffff          
JMP       L_60b7              

L_59d3:                             ; parts.c:241
CMP       [bp-raMajor], 0x0000      ; [bp-0x4], 0x0000
JNZ       L_609c              

L_59dc:                             ; parts.c:242
MOV       ax, 0xffff          
JMP       L_60b7              

L_59e2:                             ; parts.c:245
JMP       L_609c              

L_59e5:                             ; parts.c:247
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000a          
JC        L_59f9              

L_59f3:                             ; parts.c:248
MOV       ax, 0x0000          
JMP       L_60b7              

L_59f9:                             ; parts.c:249
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x12ec          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:250
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5a1e:                             ; parts.c:259
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JZ        L_5a8e              

L_5a2c:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JZ        L_5a8e              

L_5a3a:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JZ        L_5a8e              

L_5a48:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JZ        L_5a8e              

L_5a56:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JZ        L_5a8e              

L_5a64:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JZ        L_5a8e              

L_5a72:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JZ        L_5a8e              

L_5a80:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JNZ       L_5aa0              

L_5a8e:
CMP       [bp-raMajor], 0x0005      ; [bp-0x4], 0x0005
JZ        L_5aa0              

L_5a97:                             ; parts.c:260
MOV       ax, 0xffff          
JMP       L_60b7              

L_5aa0:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_5ac9              

L_5aae:
CMP       [bp-raMajor], 0x0005      ; [bp-0x4], 0x0005
JZ        L_5ac9              

L_5ab7:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_5ac9              

L_5ac0:                             ; parts.c:262
MOV       ax, 0xffff          
JMP       L_60b7              

L_5ac9:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0001          
JNZ       L_609c              

L_5ad7:
CMP       [bp-raMajor], 0x0002      ; [bp-0x4], 0x0002
JNZ       L_609c              

L_5ae0:                             ; parts.c:264
MOV       ax, 0xffff          
JMP       L_60b7              

L_5ae6:                             ; parts.c:266
JMP       L_609c              

L_5ae9:                             ; parts.c:268
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JC        L_5afd              

L_5af7:                             ; parts.c:269
MOV       ax, 0x0000          
JMP       L_60b7              

L_5afd:                             ; parts.c:270
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x1508          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:271
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5b22:                             ; parts.c:276
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JZ        L_5b68              

L_5b30:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0003          
JZ        L_5b68              

L_5b3e:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JZ        L_5b68              

L_5b4c:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JZ        L_5b68              

L_5b5a:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JNZ       L_5b92              

L_5b68:
MOV       ax, 0x0009          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_5b92              

L_5b89:                             ; parts.c:277
MOV       ax, 0xffff          
JMP       L_60b7              

L_5b92:                             ; parts.c:279
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JZ        L_5bae              

L_5ba0:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JNZ       L_5bd8              

L_5bae:
MOV       ax, 0x0002          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_5bd8              

L_5bcf:                             ; parts.c:280
MOV       ax, 0xffff          
JMP       L_60b7              

L_5bd8:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNZ       L_5bf8              

L_5be6:
CMP       [bp-raMajor], 0x0003      ; [bp-0x4], 0x0003
JZ        L_5bf8              

L_5bef:                             ; parts.c:282
MOV       ax, 0xffff          
JMP       L_60b7              

L_5bf8:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5c0b:                             ; parts.c:284
MOV       ax, 0xffff          
JMP       L_60b7              

L_5c11:                             ; parts.c:286
JMP       L_609c              

L_5c14:                             ; parts.c:288
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0010          
JC        L_5c28              

L_5c22:                             ; parts.c:289
MOV       ax, 0x0000          
JMP       L_60b7              

L_5c28:                             ; parts.c:290
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0038          
IMUL      cx                  
MOV       cx, 0x0768          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:291
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5c4d:                             ; parts.c:294
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JZ        L_5c77              

L_5c5b:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JZ        L_5c77              

L_5c69:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000c          
JNZ       L_5ca1              

L_5c77:
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_5ca1              

L_5c98:                             ; parts.c:295
MOV       ax, 0xffff          
JMP       L_60b7              

L_5ca1:                             ; parts.c:297
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0006          
JZ        L_5ccb              

L_5caf:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0005          
JZ        L_5ccb              

L_5cbd:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000e          
JNZ       L_609c              

L_5ccb:
CMP       [bp-raMajor], 0x0001      ; [bp-0x4], 0x0001
JZ        L_609c              

L_5cd4:                             ; parts.c:298
MOV       ax, 0xffff          
JMP       L_60b7              

L_5cda:                             ; parts.c:300
JMP       L_609c              

L_5cdd:                             ; parts.c:302
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0018          
JC        L_5cf1              

L_5ceb:                             ; parts.c:303
MOV       ax, 0x0000          
JMP       L_60b7              

L_5cf1:                             ; parts.c:304
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x003c          
IMUL      cx                  
MOV       cx, 0x2450          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:305
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0002          
JNZ       L_5d29              

L_5d1a:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JZ        L_5d29              

L_5d23:                             ; parts.c:306
MOV       ax, 0xffff          
JMP       L_60b7              

L_5d29:                             ; parts.c:307
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0010          
JZ        L_5d45              

L_5d37:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000e          
JNZ       L_5d57              

L_5d45:
CMP       [bp-raMajor], 0x0002      ; [bp-0x4], 0x0002
JZ        L_5d57              

L_5d4e:                             ; parts.c:308
MOV       ax, 0xffff          
JMP       L_60b7              

L_5d57:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5d6a:                             ; parts.c:310
MOV       ax, 0xffff          
JMP       L_60b7              

L_5d70:                             ; parts.c:311
JMP       L_609c              

L_5d73:                             ; parts.c:313
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000c          
JC        L_5d87              

L_5d81:                             ; parts.c:314
MOV       ax, 0x0000          
JMP       L_60b7              

L_5d87:                             ; parts.c:315
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x003c          
IMUL      cx                  
MOV       cx, 0x2180          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:316
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5dac:                             ; parts.c:318
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5dbf:                             ; parts.c:319
MOV       ax, 0xffff          
JMP       L_60b7              

L_5dc5:                             ; parts.c:321
JMP       L_609c              

L_5dc8:                             ; parts.c:323
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JC        L_5ddc              

L_5dd6:                             ; parts.c:324
MOV       ax, 0x0000          
JMP       L_60b7              

L_5ddc:                             ; parts.c:325
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x003a          
IMUL      cx                  
MOV       cx, 0x1e1a          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:326
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5e01:                             ; parts.c:329
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000a          
JC        L_5e2f              

L_5e0f:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000e          
JA        L_5e2f              

L_5e1d:
CMP       [bp-raMajor], 0x0004      ; [bp-0x4], 0x0004
JNZ       L_5e2f              

L_5e26:                             ; parts.c:330
MOV       ax, 0xffff          
JMP       L_60b7              

L_5e2f:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JNZ       L_5e4f              

L_5e3d:
CMP       [bp-raMajor], 0x0003      ; [bp-0x4], 0x0003
JZ        L_5e4f              

L_5e46:                             ; parts.c:332
MOV       ax, 0xffff          
JMP       L_60b7              

L_5e4f:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5e62:                             ; parts.c:334
MOV       ax, 0xffff          
JMP       L_60b7              

L_5e68:                             ; parts.c:336
JMP       L_609c              

L_5e6b:                             ; parts.c:338
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000f          
JC        L_5e7f              

L_5e79:                             ; parts.c:339
MOV       ax, 0x0000          
JMP       L_60b7              

L_5e7f:                             ; parts.c:340
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x16b8          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:341
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5ea4:                             ; parts.c:344
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JC        L_5ef7              

L_5eb2:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JA        L_5ef7              

L_5ec0:
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
LES       bx, [bx+0x4]        
CMP       es:[bx+0x34], 0x0000
JGE       L_5ef7              

L_5ed0:
MOV       ax, 0x000a          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JZ        L_5ef7              

L_5ef1:                             ; parts.c:345
MOV       ax, 0xffff          
JMP       L_60b7              

L_5ef7:                             ; parts.c:347
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JC        L_5f22              

L_5f05:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0008          
JA        L_5f22              

L_5f13:
CMP       [bp-raMajor], 0x0008      ; [bp-0x4], 0x0008
JNZ       L_5f22              

L_5f1c:                             ; parts.c:348
MOV       ax, 0xffff          
JMP       L_60b7              

L_5f22:                             ; parts.c:350
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0009          
JC        L_5f4d              

L_5f30:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000d          
JA        L_5f4d              

L_5f3e:
CMP       [bp-raMajor], 0x0008      ; [bp-0x4], 0x0008
JNZ       L_5f4d              

L_5f47:                             ; parts.c:351
MOV       ax, 0xffff          
JMP       L_60b7              

L_5f4d:                             ; parts.c:353
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000b          
JC        L_5f7b              

L_5f5b:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x000d          
JA        L_5f7b              

L_5f69:
CMP       [bp-raMajor], 0x0002      ; [bp-0x4], 0x0002
JNZ       L_5f7b              

L_5f72:                             ; parts.c:354
MOV       ax, 0xffff          
JMP       L_60b7              

L_5f7b:
PUSH      [bp+ppart]                ; [bp+0x6]
CALLF     FShouldPartBeHidden       ; int16_t FShouldPartBeHidden(PART *ppart)
ADD       sp, 0x0002          
CMP       ax, 0x0000          
JZ        L_609c              

L_5f8e:                             ; parts.c:356
MOV       ax, 0xffff          
JMP       L_60b7              

L_5f94:                             ; parts.c:358
JMP       L_609c              

L_5f97:                             ; parts.c:360
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0014          
JC        L_5fab              

L_5fa5:                             ; parts.c:361
MOV       ax, 0x0000          
JMP       L_60b7              

L_5fab:                             ; parts.c:362
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
MOV       cx, 0x0036          
IMUL      cx                  
MOV       cx, 0x19e2          
MOV       dx, cs              
ADD       cx, ax              
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       [bx+0x4], cx        
MOV       [bx+0x6], dx        
                                    ; parts.c:363
CMP       [idPlayer], 0xffff        ; [0x018c], 0xffff
JZ        L_609c              

L_5fd0:                             ; parts.c:366
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0000          
JC        L_609c              

L_5fde:
MOV       ax, [bp-hs+0x2]           ; ax, [bp-0x6]
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JA        L_609c              

L_5fec:
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x00c0          
IMUL      [idPlayer]                ; [0x018c]
MOV       cx, 0x59a2          
ADD       cx, ax              
PUSH      cx                  
CALLF     GetRaceGrbit              ; int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit)
ADD       sp, 0x0004          
CMP       ax, 0x0000          
JNZ       L_609c              

L_600d:                             ; parts.c:367
MOV       ax, 0xffff          
JMP       L_60b7              

L_6013:                             ; parts.c:369
JMP       L_609c              

L_6019:
CMP       ax, 0x0001          
JZ        L_528d              

L_6021:
CMP       ax, 0x0002          
JZ        L_5c14              

L_6029:
CMP       ax, 0x0004          
JZ        L_53d4              

L_6031:
CMP       ax, 0x0008          
JZ        L_56fe              

L_6039:
CMP       ax, 0x0010          
JZ        L_5cdd              

L_6041:
CMP       ax, 0x0020          
JZ        L_5d73              

L_6049:
CMP       ax, 0x0040          
JZ        L_5dc8              

L_6051:
CMP       ax, 0x0080          
JZ        L_5ae9              

L_6059:
CMP       ax, 0x0100          
JZ        L_59e5              

L_6061:
CMP       ax, 0x0200          
JZ        L_5909              

L_6069:
CMP       ax, 0x0400          
JZ        L_5662              

L_6071:
CMP       ax, 0x0800          
JZ        L_5789              

L_6079:
CMP       ax, 0x1000          
JZ        L_5877              

L_6081:
CMP       ax, 0x2000          
JZ        L_5f97              

L_6089:
CMP       ax, 0x4000          
JZ        L_545f              

L_6091:
CMP       ax, 0x8000          
JNZ       L_5287              

L_6096:
JMP       L_5e6b              

L_6099:
JMP       L_5287              

L_609c:                             ; parts.c:373
MOV       bx, [bp+ppart]            ; bx, [bp+0x6]
MOV       ax, [bx+0x4]        
MOV       dx, [bx+0x6]        
MOV       cx, 0x0002          
ADD       ax, cx              
PUSH      dx                  
PUSH      ax                  
CALLF     TechStatus                ; int16_t TechStatus(char *rgTech)
ADD       sp, 0x0004          

L_60b7:                             ; parts.c:374
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


