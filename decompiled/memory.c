#include "common.h"

HB *LphbAlloc(uint16_t cb, HeapType ht) {
    HGLOBAL hmem;
    HB     *lphb;

    lphb = 0x0;
    cb = cb + sizeof(HB);
    if (cb < mphtcbAlloc[ht]) {
        cb = mphtcbAlloc[ht];
    }
    hmem = GlobalAlloc(0x22, (uint32_t)cb);
    if (hmem == 0x0) {
        AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
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

    if (lphb != 0x0) {
        hmem = lphb->hmem;
        cbCur = lphb->cbBlock;
        cbGrow = mphtcbAlloc[lphb->ht];
        if (cbCur < 0xffdc) {
            if (cbCur > 0xffdc - cbGrow) {
                cbGrow = 0xffdc - cbCur;
            }
            GlobalUnlock(hmem);
            hmem = GlobalReAlloc(hmem, (uint32_t)(lphb->cbBlock + cbGrow), 0x22);
            if (hmem != 0x0)
                goto L_01db;
        }
        AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
        StarsLongJump(penvMem, -1);
    L_01db:
        lphbNew = (HB *)GlobalLock(hmem);
        lphbNew->hmem = hmem;
        if (rglphb[lphbNew->ht] != lphb) {
            for (lphbT = rglphb[lphbNew->ht]; lphbT != 0x0 && lphbT->lphbNext != lphb; lphbT = lphbT->lphbNext) {
            }
            lphbT->lphbNext = lphbNew;
        } else {
            rglphb[lphbNew->ht] = lphbNew;
        }
        lphbNew->cbBlock = lphbNew->cbBlock + cbGrow;
        lphbNew->cbFree = lphbNew->cbFree + cbGrow;
        lphbNew->cbSlop = lphbNew->cbSlop + cbGrow;
        return lphbNew;
    }
    return 0x0;
}

void FreeHb(HB *lphb) {
    HGLOBAL hmem;
    HB     *lphbNext;

    if (lphb != 0x0) {
        for (; lphb != 0x0; lphb = lphbNext) {
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

    for (lphb = rglphb[ht]; lphb != 0x0; lphb = lphb->lphbNext) {
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
    cb = cb + 0x3 & 0xfffe;
    while (1) {
        if (lphb == 0x0 || lphb->cbFree >= cb) {
            if (lphb == 0x0) {
                lphb = LphbAlloc(cb, ht);
            }
            lpbTop = (uint8_t *)lphb + lphb->ibTop;
            if (lphb->cbSlop >= cb)
                break;
            lpb = (uint8_t *)(lphb + 1);
            while (lpb < lpbTop) {
                lpbPrev = lpb;
                fFree = RawLoad16(lpb) & 0x1;
                cbItem = RawLoad16(lpb) & 0xfffe;
                lpb = lpb + (2 + cbItem);
                if (fFree != 0) {
                    for (; lpb < lpbTop && (RawLoad16(lpb) & 0x1) != 0x0 && lpb - lpbPrev < cb; lpb = lpb + (2 + (RawLoad16(lpb) & 0xfffe))) {
                    }
                    cbItem = lpb - lpbPrev - 0x2;
                    RawStore16(lpbPrev, cbItem | 0x1);
                    if (cbItem + 0x2 >= cb)
                        goto L_0555;
                }
            }
        }
        lphb = lphb->lphbNext;
    }
    RawStore16(lpbTop, cb - 0x2);
    lphb->ibTop = lphb->ibTop + cb;
    lphb->cbFree = lphb->cbFree - cb;
    lphb->cbSlop = lphb->cbSlop - cb;
    return lpbTop + 2;
L_0555:
    RawStore16(lpbPrev, RawLoad16(lpbPrev) & 0xfffe);
    lpbPrev = lpbPrev + 2;
    lphb->cbFree = lphb->cbFree - (cbItem + 0x2);
    return lpbPrev;
}

HB *LphbFromLpHt(void *lp, HeapType ht) {
    HB *lphb;

    if (ht >= htOrd && ht < htCount) {
        for (lphb = rglphb[ht]; lphb != 0x0 && ((HB *)lp <= lphb || (uint8_t *)lp >= (uint8_t *)lphb + lphb->cbBlock); lphb = lphb->lphbNext) {
        }
        if (lphb != 0x0) {
            return lphb;
        }
        return 0x0;
    }
    return 0x0;
}

void *LpReAlloc(void *lp, uint16_t cb, HeapType ht) {
    void    *lpNew;
    HB      *lphb;
    uint16_t cbCur;
    uint16_t cbGrow;

    cbCur = RawLoad16((uint8_t *)lp - 0x2);
    cb = cb + 0x1 & 0xfffe;
    cbGrow = cb - cbCur;
    if (cb > cbCur) {
        lphb = LphbFromLpHt(lp, ht);
        for (; (uint8_t *)lphb + lphb->ibTop != (uint8_t *)lp + cbCur || lphb->cbSlop < cbGrow; lp = (uint8_t *)lphb + (sizeof(HB) + 2)) {
            if (ht != htPlanets && ht != htThings)
                goto L_0751;
            lphb = LphbReAlloc(lphb);
        }
        lphb->cbSlop = lphb->cbSlop - cbGrow;
        lphb->cbFree = lphb->cbFree - cbGrow;
        lphb->ibTop = lphb->ibTop + cbGrow;
        RawStore16((uint8_t *)lp - 0x2, cb);
        return lp;
    L_0751:
        lpNew = LpAlloc(cb, ht);
        fmemcpy(lpNew, lp, cbCur);
        FreeLp(lp, ht);
        lp = lpNew;
        return lp;
    }
    return lp;
}

void FreeLp(void *lp, HeapType ht) {
    uint16_t cbFree;
    HB      *lphb;

    if (lp != 0x0) {
        lphb = LphbFromLpHt(lp, ht);
        cbFree = RawLoad16((uint8_t *)lp - 0x2) + 0x2;
        RawStore16((uint8_t *)lp - 0x2, RawLoad16((uint8_t *)lp - 0x2) | 0x1);
        lphb->cbFree = lphb->cbFree + cbFree;
        if ((uint8_t *)lp - (uint8_t *)lphb + cbFree - 0x2 == lphb->ibTop) {
            lphb->ibTop = lphb->ibTop - cbFree;
            lphb->cbSlop = lphb->cbSlop + cbFree;
        }
    }
    return;
}

PL *LpplReAlloc(PL *lppl, uint16_t cAlloc) {
    lppl = LpReAlloc(lppl, lppl->cbItem * cAlloc + 0x4, lppl->ht);
    lppl->iMax = LOBYTE(cAlloc);
    return lppl;
}

PL *LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht) {
    PL *lppl;

    lppl = LpAlloc(cbItem * cAlloc + 0x4, ht);
    lppl->iMax = LOBYTE(cAlloc);
    lppl->iMac = 0x0;
    lppl->fMark = 0x0;
    lppl->cbItem = cbItem;
    lppl->ht = ht;
    return lppl;
}

void FreePl(PL *lppl) {
    if (lppl != 0x0) {
        FreeLp(lppl, lppl->ht);
    }
    return;
}
