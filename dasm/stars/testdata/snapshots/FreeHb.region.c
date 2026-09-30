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
