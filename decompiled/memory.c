#include "common.h"

HB *LphbAlloc(uint16_t cb, HeapType ht) {
    HGLOBAL hmem;
    HB     *lphb;

    lphb = NULL;
    cb += sizeof(HB);
    if (cb < mphtcbAlloc[ht]) {
        cb = mphtcbAlloc[ht];
    }
    hmem = GlobalAlloc(34, (uint32_t)cb);
    if (hmem == 0) {
        AlertSz(PszFormatIds(idsMemory, NULL), MB_ICONHAND);
        StarsLongJump(penvMem, -1);
    }
    lphb = (HB *)GlobalLock(hmem);
    lphb->hmem = hmem;
    lphb->cbBlock = cb;
    lphb->cbSlop = cb - sizeof(HB);
    lphb->cbFree = cb - sizeof(HB);
    lphb->ibTop = sizeof(HB);
    lphb->ht = LOBYTE(ht);
    lphb->lphbNext = rglphb[ht];
    rglphb[ht] = lphb;
    return lphb;
}

HB *LphbReAlloc(HB *lphb) {
    HGLOBAL  hmem;
    HB      *lphbT;
    HB      *lphbNew;
    uint16_t cbCur;
    uint16_t cbGrow;

    if (lphb == 0) {
        return NULL;
    }
    hmem = lphb->hmem;
    cbCur = lphb->cbBlock;
    cbGrow = mphtcbAlloc[lphb->ht];
    if (cbCur < 0xffdc) {
        if (cbCur > 0xffdc - cbGrow) {
            cbGrow = 0xffdc - cbCur;
        }
        GlobalUnlock(hmem);
        hmem = GlobalReAlloc(hmem, (uint32_t)(lphb->cbBlock + cbGrow), 34);
        if (hmem != 0)
            goto L_01db;
    }
    AlertSz(PszFormatIds(idsMemory, NULL), MB_ICONHAND);
    StarsLongJump(penvMem, -1);
L_01db:
    lphbNew = (HB *)GlobalLock(hmem);
    lphbNew->hmem = hmem;
    if (rglphb[lphbNew->ht] == lphb) {
        rglphb[lphbNew->ht] = lphbNew;
    } else {
        for (lphbT = rglphb[lphbNew->ht]; lphbT != 0 && lphbT->lphbNext != lphb; lphbT = lphbT->lphbNext) {
        }
        lphbT->lphbNext = lphbNew;
    }
    lphbNew->cbBlock += cbGrow;
    lphbNew->cbFree += cbGrow;
    lphbNew->cbSlop += cbGrow;
    return lphbNew;
}

void FreeHb(HB *lphb) {
    HGLOBAL hmem;
    HB     *lphbNext;

    if (lphb != 0) {
        for (; lphb != 0; lphb = lphbNext) {
            lphbNext = lphb->lphbNext;
            hmem = lphb->hmem;
            GlobalUnlock(hmem);
            GlobalFree(hmem);
        }
    }
    return;
}

void ResetHb(HeapType ht) {
    HB *lphb;

    for (lphb = rglphb[ht]; lphb != 0; lphb = lphb->lphbNext) {
        lphb->ibTop = sizeof(HB);
        lphb->cbSlop = lphb->cbBlock - sizeof(HB);
        lphb->cbFree = lphb->cbBlock - sizeof(HB);
    }
    return;
}

void *LpAlloc(uint16_t cb, HeapType ht) {
    int16_t  fFree;
    uint16_t cbItem;
    uint8_t *lpbPrev;
    uint8_t *lpbTop;
    HB      *lphb;
    uint8_t *lpb;

    lphb = rglphb[ht];
    cb = cb + 3 & 0xfffe;
    while (1) {
        if (lphb == 0 || lphb->cbFree >= cb) {
            if (lphb == 0) {
                lphb = LphbAlloc(cb, ht);
            }
            lpbTop = (uint8_t *)lphb + lphb->ibTop;
            if (lphb->cbSlop >= cb)
                break;
            lpb = (uint8_t *)(lphb + 1);
            while (lpb < lpbTop) {
                lpbPrev = lpb;
                fFree = RawLoad16(lpb) & 1;
                cbItem = RawLoad16(lpb) & 0xfffe;
                lpb += 2 + cbItem;
                if (fFree != 0) {
                    for (; lpb < lpbTop && (RawLoad16(lpb) & 1) != 0 && lpb - lpbPrev < cb; lpb += 2 + (RawLoad16(lpb) & 0xfffe)) {
                    }
                    cbItem = lpb - lpbPrev - 2;
                    RawStore16(lpbPrev, cbItem | 1);
                    if (cbItem + 2 >= cb) {
                        RawStore16(lpbPrev, RawLoad16(lpbPrev) & 0xfffe);
                        lpbPrev += 2;
                        lphb->cbFree -= cbItem + 2;
                        return lpbPrev;
                    }
                }
            }
        }
        lphb = lphb->lphbNext;
    }
    RawStore16(lpbTop, cb - 2);
    lphb->ibTop += cb;
    lphb->cbFree -= cb;
    lphb->cbSlop -= cb;
    return lpbTop + 2;
}

HB *LphbFromLpHt(void *lp, HeapType ht) {
    HB *lphb;

    if (ht < htOrd || ht >= htCount) {
        return NULL;
    }
    for (lphb = rglphb[ht]; lphb != 0 && ((HB *)lp <= lphb || (uint8_t *)lp >= (uint8_t *)lphb + lphb->cbBlock); lphb = lphb->lphbNext) {
    }
    if (lphb == 0) {
        return NULL;
    }
    return lphb;
}

void *LpReAlloc(void *lp, uint16_t cb, HeapType ht) {
    void    *lpNew;
    HB      *lphb;
    uint16_t cbCur;
    uint16_t cbGrow;

    cbCur = RawLoad16((uint8_t *)lp - 0x2);
    cb = cb + 1 & 0xfffe;
    cbGrow = cb - cbCur;
    if (cb <= cbCur) {
        return lp;
    }
    lphb = LphbFromLpHt(lp, ht);
    for (; (uint8_t *)lphb + lphb->ibTop != (uint8_t *)lp + cbCur || lphb->cbSlop < cbGrow; lp = (uint8_t *)lphb + (sizeof(HB) + 2)) {
        if (ht != htPlanets && ht != htThings)
            goto L_0751;
        lphb = LphbReAlloc(lphb);
    }
    lphb->cbSlop -= cbGrow;
    lphb->cbFree -= cbGrow;
    lphb->ibTop += cbGrow;
    RawStore16((uint8_t *)lp - 0x2, cb);
    return lp;
L_0751:
    lpNew = LpAlloc(cb, ht);
    fmemcpy(lpNew, lp, cbCur);
    FreeLp(lp, ht);
    lp = lpNew;
    return lp;
}

void FreeLp(void *lp, HeapType ht) {
    uint16_t cbFree;
    HB      *lphb;

    if (lp != 0) {
        lphb = LphbFromLpHt(lp, ht);
        cbFree = RawLoad16((uint8_t *)lp - 0x2) + 2;
        RawStore16((uint8_t *)lp - 0x2, RawLoad16((uint8_t *)lp - 0x2) | 1);
        lphb->cbFree += cbFree;
        if ((uint8_t *)lp - (uint8_t *)lphb + cbFree - 2 == lphb->ibTop) {
            lphb->ibTop -= cbFree;
            lphb->cbSlop += cbFree;
        }
    }
    return;
}

PL *LpplReAlloc(PL *lppl, uint16_t cAlloc) {
    lppl = LpReAlloc(lppl, lppl->cbItem * cAlloc + 4, lppl->ht);
    lppl->iMax = LOBYTE(cAlloc);
    return lppl;
}

PL *LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht) {
    PL *lppl;

    lppl = LpAlloc(cbItem * cAlloc + 4, ht);
    lppl->iMax = LOBYTE(cAlloc);
    lppl->iMac = 0;
    lppl->fMark = 0;
    lppl->cbItem = cbItem;
    lppl->ht = ht;
    return lppl;
}

void FreePl(PL *lppl) {
    if (lppl != 0) {
        FreeLp(lppl, lppl->ht);
    }
    return;
}
