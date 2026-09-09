; FReadFleet  (file)
;   addr: 000f:3a4c  len=1665
;   sig:  int16_t FReadFleet(FLEET *lpfl)
;   params:
;     FLEET *          lpfl           [BP+0x6]
;   locals:
;     uint16_t *       pus            [BP-0x16]
;     int16_t          cch            [BP-0x14]
;     uint8_t *        pb             [BP-0x12]
;     int16_t          cish           [BP-0x10]
;     int16_t          i              [BP-0xe]
;     ORDER *          lpord          [BP-0xc]
;     int16_t          fByte          [BP-0x8]
;     int16_t          cord           [BP-0x6]
;     uint16_t         us             [BP-0x4]
;     block 000F:4047  len=0x63
;       int16_t          cOut           [BP-0x3a]
;       char[33]         szT            [BP-0x38]
;
;   stats: blocks=1  labels=1
;     Corrupt: L_3d24

L_3a4c:                             ; file.c:1663
PUSH      bp                  
MOV       bp, sp              
SUB       sp, 0x003a          
PUSH      si                  
PUSH      di                  
                                    ; file.c:1672
MOV       [bp-cish], 0x0000         ; [bp-0x10], 0x0000
                                    ; file.c:1674
MOV       ax, 0x007c          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
PUSH      [bp+lpfl+0x2]             ; [bp+0x8]
PUSH      [bp+lpfl]                 ; [bp+0x6]
CALLF     fmemset                   ; void * fmemset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0008          
                                    ; file.c:1675
MOV       ax, 0x000c          
PUSH      ax                  
MOV       ax, 0x4b98          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp+lpfl+0x2]             ; [bp+0x8]
PUSH      [bp+lpfl]                 ; [bp+0x6]
CALLF     fmemmove                  ; void * fmemmove(void *dest, void *src, uint16_t count)
ADD       sp, 0x000a          
                                    ; file.c:1678
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
MOV       cx, 0x000b          
SHR       ax, cx              
AND       ax, 0x0001          
MOV       [bp-fByte], ax            ; [bp-0x8], ax
                                    ; file.c:1680
MOV       bx, 0x4ba4          
MOV       cx, ds              
MOV       es, cx              
MOV       ax, [rgbCur+0xc]          ; ax, es:[bx]
MOV       [bp-us], ax               ; [bp-0x4], ax
                                    ; file.c:1681
MOV       [bp-pb], rgbCur+0xe       ; [bp-0x12], 0x4ba6
                                    ; file.c:1683
CMP       [bp-fByte], 0x0000        ; [bp-0x8], 0x0000
JZ        L_3b2f              

L_3ab6:                             ; file.c:1685
MOV       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JMP       L_3ace              

L_3abe:
MOV       ax, [bp-0xe]        
ADD       [bp-i], 0x0001            ; [bp-0xe], 0x0001
MOV       cx, 0x0001          
SHR       [bp-us], cx               ; [bp-0x4], cx
MOV       ax, [bp-0x4]        

L_3ace:
CMP       [bp-us], 0x0000           ; [bp-0x4], 0x0000
JZ        L_3bae              

L_3ad7:                             ; file.c:1686
MOV       ax, [bp-us]               ; ax, [bp-0x4]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_3abe              

L_3ae5:                             ; file.c:1688
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
ADD       [bp-pb], 0x0001           ; [bp-0x12], 0x0001
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       cx, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xe]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         
                                    ; file.c:1689
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xe]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JZ        L_3abe              

L_3b25:                             ; file.c:1690
ADD       [bp-cish], 0x0001         ; [bp-0x10], 0x0001

L_3b29:                             ; file.c:1692
JMP       L_3abe              

L_3b2f:                             ; file.c:1695
MOV       ax, [bp-pb]               ; ax, [bp-0x12]
MOV       [bp-pus], ax              ; [bp-0x16], ax
                                    ; file.c:1696
MOV       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JMP       L_3b4d              

L_3b3d:
MOV       ax, [bp-0xe]        
ADD       [bp-i], 0x0001            ; [bp-0xe], 0x0001
MOV       cx, 0x0001          
SHR       [bp-us], cx               ; [bp-0x4], cx
MOV       ax, [bp-0x4]        

L_3b4d:
CMP       [bp-us], 0x0000           ; [bp-0x4], 0x0000
JZ        L_3ba8              

L_3b56:                             ; file.c:1697
MOV       ax, [bp-us]               ; ax, [bp-0x4]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_3b3d              

L_3b64:                             ; file.c:1699
MOV       bx, [bp-pus]              ; bx, [bp-0x16]
ADD       [bp-pus], 0x0002          ; [bp-0x16], 0x0002
MOV       ax, [bx]            
MOV       cx, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xe]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         
                                    ; file.c:1700
MOV       ax, 0x000c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xe]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
CMP       es:[bx], 0x0000     
JZ        L_3b3d              

L_3ba1:                             ; file.c:1701
ADD       [bp-cish], 0x0001         ; [bp-0x10], 0x0001

L_3ba5:                             ; file.c:1703
JMP       L_3b3d              

L_3ba8:
MOV       ax, [bp-pus]              ; ax, [bp-0x16]
MOV       [bp-pb], ax               ; [bp-0x12], ax

L_3bae:                             ; file.c:1706
CMP       [bp-cish], 0x0000         ; [bp-0x10], 0x0000
JNZ       L_3bcb              

L_3bb7:                             ; file.c:1707
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
AND       ax, 0xfbff          
OR        ax, 0x0400          
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x4], ax     

L_3bcb:                             ; file.c:1709
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
AND       ax, 0x00ff          
CMP       ax, 0x0004          
JC        L_3cc0              

L_3bdd:                             ; file.c:1712
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       cx, ds              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       [bp-us], ax               ; [bp-0x4], ax
                                    ; file.c:1713
ADD       [bp-pb], 0x0002           ; [bp-0x12], 0x0002
                                    ; file.c:1715
MOV       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JMP       L_3c06              

L_3bf6:
MOV       ax, [bp-0xe]        
ADD       [bp-i], 0x0001            ; [bp-0xe], 0x0001
MOV       cx, 0x0002          
SHR       [bp-us], cx               ; [bp-0x4], cx
MOV       ax, [bp-0x4]        

L_3c06:
CMP       [bp-i], 0x0005            ; [bp-0xe], 0x0005
JGE       L_3cc0              

L_3c0f:                             ; file.c:1717
MOV       ax, [bp-us]               ; ax, [bp-0x4]
AND       ax, 0x0003          
JMP       L_3ca2              

L_3c18:                             ; file.c:1720
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       al, [bx]            
AND       ax, 0x00ff          
MOV       dx, 0x0000          
MOV       cx, 0x004c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       di, [bp-i]                ; di, [bp-0xe]
SHL       di, 0x0001          
SHL       di, 0x0001          
ADD       bx, di              
MOV       es, si              
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; file.c:1721
ADD       [bp-pb], 0x0001           ; [bp-0x12], 0x0001
                                    ; file.c:1722
JMP       L_3bf6              

L_3c47:                             ; file.c:1724
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       ax, [bx]            
MOV       dx, 0x0000          
MOV       cx, 0x004c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       di, [bp-i]                ; di, [bp-0xe]
SHL       di, 0x0001          
SHL       di, 0x0001          
ADD       bx, di              
MOV       es, si              
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; file.c:1725
ADD       [bp-pb], 0x0002           ; [bp-0x12], 0x0002
                                    ; file.c:1726
JMP       L_3bf6              

L_3c73:                             ; file.c:1728
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
MOV       cx, 0x004c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       di, [bp-i]                ; di, [bp-0xe]
SHL       di, 0x0001          
SHL       di, 0x0001          
ADD       bx, di              
MOV       es, si              
MOV       es:[bx], ax         
MOV       es:[bx+0x2], dx     
                                    ; file.c:1729
ADD       [bp-pb], 0x0004           ; [bp-0x12], 0x0004
                                    ; file.c:1730
JMP       L_3bf6              

L_3ca2:
CMP       ax, 0x0001          
JZ        L_3c18              

L_3caa:
CMP       ax, 0x0002          
JZ        L_3c47              

L_3cb2:
CMP       ax, 0x0003          
JNZ       L_3bf6              

L_3cb7:
JMP       L_3c73              

L_3cc0:                             ; file.c:1735
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x4]     
AND       ax, 0x00ff          
CMP       ax, 0x0007          
JNC       L_3d11              

L_3cd2:                             ; file.c:1737
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       cx, ds              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       dx, es:[bx+0x2]     
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x74], ax    
MOV       es:[bx+0x76], dx    
                                    ; file.c:1738
ADD       [bp-pb], 0x0004           ; [bp-0x12], 0x0004
                                    ; file.c:1739
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       ax, [bx]            
MOV       dx, [bx+0x2]        
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x2c], ax    
MOV       es:[bx+0x2e], dx    
                                    ; file.c:1740
ADD       [bp-pb], 0x0004           ; [bp-0x12], 0x0004
                                    ; file.c:1742
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:1743
MOV       ax, 0x0001          
JMP       L_40c7              

L_3d11:                             ; file.c:1746
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0010          
JZ        L_3d4b              

Corrupt:                            ; file.c:1749
MOV       ax, 0x0010          
PUSH      ax                  
MOV       ax, 0x0000          
MOV       dx, 0x0000          
PUSH      dx                  
PUSH      ax                  
MOV       ax, 0x0003          
PUSH      ax                  
CALLF     PszFormatIds              ; char * PszFormatIds(StringId ids, int16_t *pParams)
ADD       sp, 0x0006          
PUSH      ax                  
CALLF     AlertSz                   ; int16_t AlertSz(char *sz, MessageBoxType mbType)
ADD       sp, 0x0004          
                                    ; file.c:1750
MOV       ax, 0x0000          
JMP       L_40c7              

L_3d4b:                             ; file.c:1754
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
MOV       cx, ds              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       [bp-us], ax               ; [bp-0x4], ax
                                    ; file.c:1755
ADD       [bp-pb], 0x0002           ; [bp-0x12], 0x0002
                                    ; file.c:1757
MOV       ax, [bp-pb]               ; ax, [bp-0x12]
MOV       [bp-pus], ax              ; [bp-0x16], ax
                                    ; file.c:1758
MOV       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JMP       L_3d7a              

L_3d6a:
MOV       ax, [bp-0xe]        
ADD       [bp-i], 0x0001            ; [bp-0xe], 0x0001
MOV       cx, 0x0001          
SHR       [bp-us], cx               ; [bp-0x4], cx
MOV       ax, [bp-0x4]        

L_3d7a:
CMP       [bp-us], 0x0000           ; [bp-0x4], 0x0000
JZ        L_3e0f              

L_3d83:                             ; file.c:1759
MOV       ax, [bp-us]               ; ax, [bp-0x4]
AND       ax, 0x0001          
CMP       ax, 0x0000          
JZ        L_3d6a              

L_3d91:                             ; file.c:1761
MOV       bx, [bp-pus]              ; bx, [bp-0x16]
ADD       [bp-pus], 0x0002          ; [bp-0x16], 0x0002
MOV       ax, [bx]            
MOV       cx, 0x002c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xe]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         
                                    ; file.c:1762
MOV       ax, 0x002c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xe]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
MOV       cx, 0x0007          
SHR       ax, cx              
AND       ax, 0x01ff          
CMP       ax, 0x01f4          
JC        L_3d6a              

L_3dd8:                             ; file.c:1763
MOV       ax, 0x002c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       cx, [bp+lpfl+0x2]         ; cx, [bp+0x8]
ADD       bx, ax              
MOV       ax, [bp-i]                ; ax, [bp-0xe]
SHL       ax, 0x0001          
ADD       bx, ax              
MOV       es, cx              
MOV       ax, es:[bx]         
AND       ax, 0x007f          
OR        ax, 0xf980          
MOV       cx, 0x002c          
MOV       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       si, [bp+lpfl+0x2]         ; si, [bp+0x8]
ADD       bx, cx              
MOV       dx, [bp-i]                ; dx, [bp-0xe]
SHL       dx, 0x0001          
ADD       bx, dx              
MOV       es, si              
MOV       es:[bx], ax         

L_3e0c:                             ; file.c:1765
JMP       L_3d6a              

L_3e0f:
MOV       ax, [bp-pus]              ; ax, [bp-0x16]
MOV       [bp-pb], ax               ; [bp-0x12], ax
                                    ; file.c:1767
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
ADD       [bp-pb], 0x0001           ; [bp-0x12], 0x0001
MOV       al, [bx]            
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x60], al    
                                    ; file.c:1768
MOV       bx, [bp-pb]               ; bx, [bp-0x12]
ADD       [bp-pb], 0x0001           ; [bp-0x12], 0x0001
MOV       al, [bx]            
AND       ax, 0x00ff          
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x62], ax    
                                    ; file.c:1770
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
ADD       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x0012          
PUSH      ax                  
CALLF     LpplAlloc                 ; PL * LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht)
ADD       sp, 0x0006          
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x64], ax    
MOV       es:[bx+0x66], dx    
                                    ; file.c:1771
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
ADD       ax, 0x0001          
MOV       cx, 0x0012          
IMUL      cx                  
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
PUSH      dx                  
PUSH      ax                  
CALLF     fmemset                   ; void * fmemset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0008          
                                    ; file.c:1772
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
MOV       [bp-cord], ax             ; [bp-0x6], ax
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x64]    
MOV       dx, es:[bx+0x66]    
MOV       cx, 0x0004          
ADD       ax, cx              
MOV       [bp-lpord], ax            ; [bp-0xc], ax
MOV       [bp-lpord+0x2], dx        ; [bp-0xa], dx
JMP       L_3ec0              

L_3eaf:
MOV       ax, [bp-0xc]        
MOV       es, [bp-0xa]        
ADD       [bp-lpord], 0x0012        ; [bp-0xc], 0x0012
MOV       ax, [bp-0x6]        
SUB       [bp-cord], 0x0001         ; [bp-0x6], 0x0001

L_3ec0:
CMP       [bp-cord], 0x0000         ; [bp-0x6], 0x0000
JZ        L_3f33              

L_3ec9:                             ; file.c:1774
MOV       ax, 0x0012          
PUSH      ax                  
MOV       ax, 0x0000          
PUSH      ax                  
MOV       ax, 0x4b98          
PUSH      ax                  
CALLF     memset                    ; void * memset(void *dest, int16_t value, uint16_t count)
ADD       sp, 0x0006          
                                    ; file.c:1775
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:1776
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0013          
JZ        L_3f0b              

L_3ef5:
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0014          
JNZ       Corrupt             

L_3f0b:                             ; file.c:1778
MOV       si, 0x4b98          
LES       di, [bp-lpord]            ; di, [bp-0xc]
MOV       cx, 0x0009          
MOVSW.REP es:[di], [rgbCur]         ; es:[di], ds:[si]
MOV       ax, [bp-0xc]        
MOV       dx, [bp-0xa]        
                                    ; file.c:1779
LES       bx, [bp-lpord]            ; bx, [bp-0xc]
MOV       ax, es:[bx+0x6]     
AND       ax, 0xdfff          
OR        ax, 0x0000          
LES       bx, [bp-lpord]            ; bx, [bp-0xc]
MOV       es:[bx+0x6], ax     
                                    ; file.c:1780
JMP       L_3eaf              

L_3f33:                             ; file.c:1782
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x62]    
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
LES       bx, es:[bx+0x64]    
MOV       es:[bx+0x3], al     
                                    ; file.c:1788
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
CMP       es:[bx+0x6], 0xffff 
JZ        L_3fdb              

L_3f52:                             ; file.c:1790
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, [game+0xa]            ; ax, [0x007a]
CMP       es:[bx+0x6], ax     
JLE       L_3f6a              

L_3f61:                             ; file.c:1793
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x6], 0xffff 

L_3f6a:                             ; file.c:1796
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x6]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
CMP       es:[bx+0x8], ax     
JNZ       L_3fa4              

L_3f85:
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       ax, es:[bx+0x6]     
SHL       ax, 0x0001          
SHL       ax, 0x0001          
MOV       bx, 0x2f40          
ADD       bx, ax              
MOV       ax, [bx+0x2]        
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
CMP       es:[bx+0xa], ax     
JZ        L_3fdb              

L_3fa4:                             ; file.c:1798
CMP       [bp-i], 0x0000            ; [bp-0xe], 0x0000
JNZ       Corrupt             

L_3fad:
CMP       [game+0x12], 0x0000       ; [0x0082], 0x0000
JNZ       Corrupt             

L_3fb7:                             ; file.c:1799
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       bx, es:[bx+0x6]     
SHL       bx, 0x0001          
SHL       bx, 0x0001          
MOV       ax, [bx+0x2f40]     
MOV       dx, [bx+0x2f42]     
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x8], ax     
MOV       es:[bx+0xa], dx     

L_3fdb:                             ; file.c:1805
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:1807
MOV       cx, 0x000a          
MOV       ax, [hdrCur]              ; ax, [0x2696]
SHR       ax, cx              
AND       ax, 0x003f          
CMP       ax, 0x0015          
JNZ       L_40b2              

L_3ff3:                             ; file.c:1809
MOV       al, [rgbCur]              ; al, [0x4b98]
CBW       ax, al              
MOV       [bp-cch], ax              ; [bp-0x14], ax
                                    ; file.c:1811
CMP       [bp-cch], 0x0000          ; [bp-0x14], 0x0000
JNZ       L_4047              

L_4003:                             ; file.c:1813
MOV       ax, 0x0001          
PUSH      ax                  
MOV       ax, 0x4b99          
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       ax, 0x0001          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x78], ax    
MOV       es:[bx+0x7a], dx    
                                    ; file.c:1814
MOV       ax, 0x4b99          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x7a]        
PUSH      es:[bx+0x78]        
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          
                                    ; file.c:1816
JMP       L_40aa              

L_4047:                             ; file.c:1818
MOV       [bp-cOut], 0x0020         ; [bp-0x3a], 0x0020
                                    ; file.c:1821
LEA       ax, [bp-cOut]             ; ax, [bp-0x3a]
PUSH      ax                  
LEA       ax, [bp-szT]              ; ax, [bp-0x38]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
PUSH      [bp-cch]                  ; [bp-0x14]
MOV       ax, 0x4b99          
MOV       dx, ds              
PUSH      dx                  
PUSH      ax                  
CALLF     FDecompressUserString     ; int16_t FDecompressUserString(char *szIn, int16_t cIn, char *szOut, int16_t *pcOut)
ADD       sp, 0x000c          
                                    ; file.c:1822
MOV       ax, 0x0001          
PUSH      ax                  
LEA       ax, [bp-szT]              ; ax, [bp-0x38]
PUSH      ax                  
CALLF     strlen                    ; uint16_t strlen(char *s)
ADD       sp, 0x0002          
ADD       ax, 0x0001          
PUSH      ax                  
CALLF     LpAlloc                   ; void * LpAlloc(uint16_t cb, HeapType ht)
ADD       sp, 0x0004          
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x78], ax    
MOV       es:[bx+0x7a], dx    
                                    ; file.c:1823
LEA       ax, [bp-szT]              ; ax, [bp-0x38]
MOV       dx, ss              
PUSH      dx                  
PUSH      ax                  
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
PUSH      es:[bx+0x7a]        
PUSH      es:[bx+0x78]        
CALLF     fstrcpy                   ; char * fstrcpy(char *dest, char *src)
ADD       sp, 0x0008          

L_40aa:                             ; file.c:1825
CALLF     ReadRt                    ; void ReadRt()
                                    ; file.c:1827
JMP       L_40c1              

L_40b2:                             ; file.c:1828
LES       bx, [bp+lpfl]             ; bx, [bp+0x6]
MOV       es:[bx+0x78], 0x0000
MOV       es:[bx+0x7a], 0x0000

L_40c1:                             ; file.c:1830
MOV       ax, 0x0001          

L_40c7:                             ; file.c:1831
POP       di                  
POP       si                  
MOV       sp, bp              
POP       bp                  
RETF                          


