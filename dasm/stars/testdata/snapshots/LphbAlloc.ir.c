HB *LphbAlloc(uint16_t cb, HeapType ht) {
    HGLOBAL hmem;
    HB     *lphb;

L_0000:
    lphb = NULL;
    cb += sizeof(HB);
    if (cb >= mphtcbAlloc[ht])
        goto L_0034;
    else
        goto L_0028;

L_0028:
    cb = mphtcbAlloc[ht];

L_0034:
    hmem = GlobalAlloc(34, (uint32_t)cb);
    if (hmem != 0)
        goto L_0082;
    else
        goto L_0051;

L_0051:
    AlertSz(PszFormatIds(idsMemory, NULL), MB_ICONHAND);
    StarsLongJump(penvMem, -1);

L_0082:
    lphb = (HB *)GlobalLock(hmem);
    lphb->hmem = hmem;
    lphb->cbBlock = cb;
    lphb->cbSlop = cb - sizeof(HB);
    lphb->cbFree = cb - sizeof(HB);
    lphb->ibTop = sizeof(HB);
    lphb->ht = ht;
    lphb->lphbNext = rglphb[ht];
    rglphb[ht] = lphb;

L_0102:
    return lphb;
}
