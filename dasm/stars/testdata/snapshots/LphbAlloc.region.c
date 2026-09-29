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
