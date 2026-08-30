#ifndef STARS_DECOMPILED_MEMORY_H
#define STARS_DECOMPILED_MEMORY_H

#include <stdint.h>
#include <windows.h>

HB   *LphbAlloc(uint16_t cb, HeapType ht);
HB   *LphbReAlloc(HB *lphb);
void  FreeHb(HB *lphb);
void  ResetHb(HeapType ht);
void *LpAlloc(uint16_t cb, HeapType ht);
HB   *LphbFromLpHt(void *lp, HeapType ht);
void *LpReAlloc(void *lp, uint16_t cb, HeapType ht);
void  FreeLp(void *lp, HeapType ht);
PL   *LpplReAlloc(PL *lppl, uint16_t cAlloc);
PL   *LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht);
void  FreePl(PL *lppl);

#endif
