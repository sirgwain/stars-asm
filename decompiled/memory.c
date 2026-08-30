#include "common.h"

HB *LphbAlloc(uint16_t cb, HeapType ht) {
    uint16_t hmem;
    HB      *lphb;

L_0000:
    lphb = 0x0;
    cb = (cb + 0x10);
    if ((cb >= mphtcbAlloc[ht]))
        goto L_0034;
    else
        goto L_0028;

L_0028:
    cb = mphtcbAlloc[ht];

L_0034:
    /* untranslated: hmem = GlobalAlloc(0x22, words(0x0, cb)) */
    if ((hmem != 0x0))
        goto L_0082;
    else
        goto L_0051;

L_0051:
    AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
    longjmp(penvMem, 0xffff);

L_0082:
    lphb = GlobalLock(hmem);
    lphb->hmem = hmem;
    lphb->cbBlock = cb;
    lphb->cbSlop = (cb + 0xfff0);
    lphb->cbFree = (cb + 0xfff0);
    lphb->ibTop = 0x10;
    lphb->ht = LOBYTE(ht);
    lphb->lphbNext = rglphb[ht];
    rglphb[ht] = lphb;

L_0102:
    return lphb;
}

HB *LphbReAlloc(HB *lphb) {
    uint16_t hmem;
    HB      *lphbT;
    HB      *lphbNew;
    uint16_t cbCur;
    uint16_t cbGrow;
    HB      *t_merge_02d2_0001_wide;

L_0108:
    if ((LOWORD(lphb) != 0x0))
        goto L_012c;
    else
        goto L_011a;

L_011a:
    if ((HIWORD(lphb) != 0x0))
        goto L_012c;
    else
        goto L_0123;

L_0123:
    t_merge_02d2_0001_wide = 0x0;
    goto L_02d2;

L_012c:
    hmem = lphb->hmem;
    cbCur = lphb->cbBlock;
    cbGrow = mphtcbAlloc[lphb->ht];
    if ((cbCur >= 0xffdc))
        goto LReAllocOOM;
    else
        goto L_015b;

L_015b:

L_0164:
    if ((cbCur <= (0xffdc - cbGrow)))
        goto L_017b;
    else
        goto L_0172;

L_0172:
    cbGrow = (0xffdc - cbCur);

L_017b:
    GlobalUnlock(hmem);
    /* untranslated: hmem = GlobalReAlloc(hmem, words(0x0, (lphb->cbBlock + cbGrow)), 0x22) */
    if ((hmem != 0x0))
        goto L_01db;
    else
        goto LReAllocOOM;

LReAllocOOM:
    AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
    longjmp(penvMem, 0xffff);

L_01db:
    lphbNew = GlobalLock(hmem);
    lphbNew->hmem = hmem;
    if ((LOWORD(rglphb[lphbNew->ht]) != LOWORD(lphb)))
        goto L_023c;
    else
        goto L_0212;

L_0212:
    if ((HIWORD(rglphb[lphbNew->ht]) != HIWORD(lphb)))
        goto L_023c;
    else
        goto L_021b;

L_021b:
    rglphb[lphbNew->ht] = lphbNew;
    goto L_02ac;

L_023c:
    lphbT = rglphb[lphbNew->ht];

L_025a:
    if ((LOWORD(lphbT) != 0x0))
        goto L_026c;
    else
        goto L_0263;

L_0263:
    if ((HIWORD(lphbT) == 0x0))
        goto L_029b;
    else
        goto L_026c;

L_026c:
    if ((LOWORD(lphbT->lphbNext) != LOWORD(lphb)))
        goto L_0287;
    else
        goto L_027e;

L_027e:
    if ((HIWORD(lphbT->lphbNext) == HIWORD(lphb)))
        goto L_029b;
    else
        goto L_0287;

L_0287:
    lphbT = lphbT->lphbNext;
    goto L_025a;

L_029b:
    lphbT->lphbNext = lphbNew;

L_02ac:
    lphbNew->cbBlock = (lphbNew->cbBlock + cbGrow);
    lphbNew->cbFree = (lphbNew->cbFree + cbGrow);
    lphbNew->cbSlop = (lphbNew->cbSlop + cbGrow);
    t_merge_02d2_0001_wide = lphbNew;

L_02d2:
    return t_merge_02d2_0001_wide;
}

void FreeHb(HB *lphb) {
    uint16_t hmem;
    HB      *lphbNext;

L_02d8:
    if ((LOWORD(lphb) != 0x0))
        goto L_0330;
    else
        goto L_02ea;

L_02ea:
    if ((HIWORD(lphb) == 0x0))
        goto L_0342;
    else
        goto L_02f0;

L_02f0:

L_02f9:
    lphbNext = lphb->lphbNext;
    hmem = lphb->hmem;
    GlobalUnlock(hmem);
    GlobalFree(hmem);
    lphb = lphbNext;

L_0330:
    if ((LOWORD(lphb) != 0x0))
        goto L_02f9;
    else
        goto L_0339;

L_0339:
    if ((HIWORD(lphb) != 0x0))
        goto L_02f9;
    else
        goto L_0342;

L_0342:
    return;
}

void ResetHb(HeapType ht) {
    HB *lphb;

L_0348:
    lphb = rglphb[ht];
    goto L_039a;

L_0369:
    lphb->ibTop = 0x10;
    lphb->cbSlop = (lphb->cbBlock + 0xfff0);
    lphb->cbFree = (lphb->cbBlock + 0xfff0);
    lphb = lphb->lphbNext;

L_039a:
    if ((LOWORD(lphb) != 0x0))
        goto L_0369;
    else
        goto L_03a3;

L_03a3:
    if ((HIWORD(lphb) != 0x0))
        goto L_0369;
    else
        goto L_03ac;

L_03ac:
    return;
}

void *LpAlloc(uint16_t cb, HeapType ht) {
    int16_t  fFree;
    uint16_t cbItem;
    uint8_t *lpbPrev;
    uint8_t *lpbTop;
    HB      *lphb;
    uint8_t *lpb;
    uint16_t t_merge_0586_0001;
    uint16_t t_merge_0586_0002;

L_03b2:
    lphb = rglphb[ht];
    cb = ((cb + 0x3) & 0xfffe);

L_03dc:
    if ((LOWORD(lphb) != 0x0))
        goto L_03ee;
    else
        goto L_03e5;

L_03e5:
    if ((HIWORD(lphb) == 0x0))
        goto L_0410;
    else
        goto L_03ee;

L_03ee:
    if ((lphb->cbFree >= cb))
        goto L_0410;
    else
        goto LTryNextBlock;

LTryNextBlock:
    lphb = lphb->lphbNext;
    goto L_03dc;

L_0410:
    if ((LOWORD(lphb) != 0x0))
        goto L_0436;
    else
        goto L_0419;

L_0419:
    if ((HIWORD(lphb) != 0x0))
        goto L_0436;
    else
        goto L_0422;

L_0422:
    lphb = LphbAlloc(cb, ht);

L_0436:
    lpbTop = ((uint8_t *)(lphb) + lphb->ibTop);
    if ((lphb->cbSlop < cb))
        goto L_0493;
    else
        goto L_045a;

L_045a:
    LOWORD(lpbTop) = (cb + 0xfffe);
    lphb->ibTop = (lphb->ibTop + cb);
    lphb->cbFree = (lphb->cbFree - cb);
    lphb->cbSlop = (lphb->cbSlop - cb);
    t_merge_0586_0001 = (LOWORD(lpbTop) + 0x2);
    t_merge_0586_0002 = HIWORD(lpbTop);
    goto L_0586;

L_0493:
    lpb = &(lphb[0x1]);
    goto L_0575;

L_04a7:
    lpbPrev = lpb;
    fFree = (LOWORD(lpb) & 0x1);
    cbItem = (LOWORD(lpb) & 0xfffe);
    lpb = ((uint8_t *)(lpb) + (cbItem + 0x2));
    if ((fFree == 0x0))
        goto L_0575;
    else
        goto L_04dd;

L_04dd:
    if ((LOWORD(lpb) >= LOWORD(lpbTop)))
        goto L_0524;
    else
        goto L_04eb;

L_04eb:
    if (((LOWORD(lpb) & 0x1) == 0x0))
        goto L_0524;
    else
        goto L_04fc;

L_04fc:
    if (((LOWORD(lpb) - LOWORD(lpbPrev)) >= cb))
        goto L_0524;
    else
        goto L_0512;

L_0512:
    lpb = ((uint8_t *)(lpb) + ((LOWORD(lpb) & 0xfffe) + 0x2));
    goto L_04dd;

L_0524:
    cbItem = ((LOWORD(lpb) - LOWORD(lpbPrev)) + 0xfffe);
    LOWORD(lpbPrev) = (cbItem | 0x1);
    if (((cbItem + 0x2) < cb))
        goto L_0575;
    else
        goto L_054f;

L_054f:

L_0555:
    LOWORD(lpbPrev) = (LOWORD(lpbPrev) & 0xfffe);
    lpbPrev = (lpbPrev + 0x2);
    lphb->cbFree = (lphb->cbFree - (cbItem + 0x2));
    t_merge_0586_0001 = LOWORD(lpbPrev);
    t_merge_0586_0002 = HIWORD(lpbPrev);
    goto L_0586;

L_0575:
    if ((LOWORD(lpb) >= LOWORD(lpbTop)))
        goto LTryNextBlock;
    else
        goto L_0580;

L_0580:

L_0586:
    return farptr(t_merge_0586_0002, t_merge_0586_0001);
}

HB *LphbFromLpHt(void *lp, HeapType ht) {
    HB *lphb;
    HB *t_merge_0659_0001_wide;

L_058c:
    if ((ht < htOrd))
        goto L_05a7;
    else
        goto L_059e;

L_059e:
    if ((ht < htCount))
        goto L_05b0;
    else
        goto L_05a7;

L_05a7:
    t_merge_0659_0001_wide = 0x0;
    goto L_0659;

L_05b0:
    lphb = rglphb[ht];
    goto L_0623;

L_05c8:
    if ((HIWORD(lp) < HIWORD(lphb)))
        goto L_0612;
    else
        goto L_05db;

L_05db:
    if ((HIWORD(lp) > HIWORD(lphb)))
        goto L_05e7;
    else
        goto L_05e0;

L_05e0:
    if ((LOWORD(lp) <= LOWORD(lphb)))
        goto L_0612;
    else
        goto L_05e7;

L_05e7:
    if ((HIWORD(lp) > HIWORD(lphb)))
        goto L_0612;
    else
        goto L_0603;

L_0603:
    if ((HIWORD(lp) < HIWORD(lphb)))
        goto L_0635;
    else
        goto L_0608;

L_0608:
    if ((LOWORD(lp) < (LOWORD(lphb) + lphb->cbBlock)))
        goto L_0635;
    else
        goto L_060c;

L_060c:

L_0612:
    lphb = lphb->lphbNext;

L_0623:
    if ((LOWORD(lphb) != 0x0))
        goto L_05c8;
    else
        goto L_062c;

L_062c:
    if ((HIWORD(lphb) != 0x0))
        goto L_05c8;
    else
        goto L_0635;

L_0635:
    if ((LOWORD(lphb) != 0x0))
        goto L_0650;
    else
        goto L_063e;

L_063e:
    if ((HIWORD(lphb) != 0x0))
        goto L_0650;
    else
        goto L_0647;

L_0647:
    t_merge_0659_0001_wide = 0x0;
    goto L_0659;

L_0650:
    t_merge_0659_0001_wide = lphb;

L_0659:
    return t_merge_0659_0001_wide;
}

void *LpReAlloc(void *lp, uint16_t cb, HeapType ht) {
    void    *lpNew;
    HB      *lphb;
    uint16_t cbCur;
    uint16_t cbGrow;
    void    *t_merge_07a2_0001_wide;

L_0660:
    cbCur = *(lp - 0x2);
    cb = ((cb + 0x1) & 0xfffe);
    cbGrow = (cb - cbCur);
    if ((cb > cbCur))
        goto L_069c;
    else
        goto L_0693;

L_0693:
    t_merge_07a2_0001_wide = lp;
    goto L_07a2;

L_069c:
    lphb = LphbFromLpHt(lp, ht);

LGrewHeap:
    if (((LOWORD(lphb) + lphb->ibTop) != (LOWORD(lp) + cbCur)))
        goto L_0714;
    else
        goto L_06d4;

L_06d4:
    if ((HIWORD(lphb) != HIWORD(lp)))
        goto L_0714;
    else
        goto L_06db;

L_06db:
    if ((lphb->cbSlop < cbGrow))
        goto L_0714;
    else
        goto L_06ea;

L_06ea:
    lphb->cbSlop = (lphb->cbSlop - cbGrow);
    lphb->cbFree = (lphb->cbFree - cbGrow);
    lphb->ibTop = (lphb->ibTop + cbGrow);
    *(lp - 0x2) = cb;
    goto L_0799;

L_0714:
    if ((ht == htPlanets))
        goto L_0726;
    else
        goto L_071d;

L_071d:
    if ((ht != htThings))
        goto L_0751;
    else
        goto L_0726;

L_0726:
    lphb = LphbReAlloc(lphb);
    lp = &(*(lphb + 0x12));
    goto LGrewHeap;

L_0751:
    lpNew = LpAlloc(cb, ht);
    fmemcpy(lpNew, lp, cbCur);
    FreeLp(lp, ht);
    lp = lpNew;

L_0799:
    t_merge_07a2_0001_wide = lp;

L_07a2:
    return t_merge_07a2_0001_wide;
}

void FreeLp(void *lp, HeapType ht) {
    uint16_t cbFree;
    HB      *lphb;

L_07a8:
    if ((LOWORD(lp) != 0x0))
        goto L_07c6;
    else
        goto L_07ba;

L_07ba:
    if ((HIWORD(lp) == 0x0))
        goto L_082f;
    else
        goto L_07c0;

L_07c0:

L_07c6:
    lphb = LphbFromLpHt(lp, ht);
    cbFree = (*(lp - 0x2) + 0x2);
    *(lp - 0x2) = (*(lp - 0x2) | 0x1);
    lphb->cbFree = (lphb->cbFree + cbFree);
    if (((((LOWORD(lp) - LOWORD(lphb)) + cbFree) + 0xfffe) != lphb->ibTop))
        goto L_082f;
    else
        goto L_081b;

L_081b:
    lphb->ibTop = (lphb->ibTop - cbFree);
    lphb->cbSlop = (lphb->cbSlop + cbFree);

L_082f:
    return;
}

PL *LpplReAlloc(PL *lppl, uint16_t cAlloc) {
L_0836:
    lppl = LpReAlloc(&(lppl), (LOWORD((lppl->cbItem * cAlloc)) + 0x4), lppl->ht);
    lppl->iMax = LOBYTE(cAlloc);

L_0885:
    return lppl;
}

PL *LpplAlloc(uint16_t cbItem, uint16_t cAlloc, HeapType ht) {
    PL *lppl;

L_088c:
    lppl = LpAlloc((LOWORD((cbItem * cAlloc)) + 0x4), ht);
    lppl->iMax = LOBYTE(cAlloc);
    lppl->iMac = 0x0;
    lppl->fMark = 0x0;
    LOWORD(lppl) = ((LOWORD(lppl) & 0xff00) | (cbItem & 0xff));
    LOWORD(lppl) = ((LOWORD(lppl) & 0xf1ff) | ((ht & 0x7) << 0x9));

L_0912:
    return lppl;
}

void FreePl(PL *lppl) {
L_0918:
    if ((LOWORD(lppl) != 0x0))
        goto L_0936;
    else
        goto L_092a;

L_092a:
    if ((HIWORD(lppl) == 0x0))
        goto L_0953;
    else
        goto L_0930;

L_0930:

L_0936:
    FreeLp(&(lppl), lppl->ht);

L_0953:
    return;
}
