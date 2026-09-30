void FreeHb(HB *lphb) {
    HGLOBAL hmem;
    HB     *lphbNext;

L_02d8:
    if (lphb != 0)
        goto L_0330;
    else
        goto L_0342;

L_02f0:
    goto L_0330;

L_02f9:
    lphbNext = lphb->lphbNext;
    hmem = lphb->hmem;
    GlobalUnlock(hmem);
    GlobalFree(hmem);
    lphb = lphbNext;

L_0330:
    if (lphb != 0)
        goto L_02f9;
    else
        goto L_0342;

L_0342:
    return;
}
