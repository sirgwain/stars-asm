void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER    ord;
    uint16_t scratch_bp_m1a;

L_9e28:
    if ((fRecycle == 0))
        goto L_9ef2;
    else
        goto L_9e3a;

L_9e3a:
    if ((iDel == 86))
        goto L_9ebf;
    else
        goto L_9e43;

L_9e43:
    if ((lpfl->cord == 2))
        goto L_9ebf;
    else
        goto L_9e50;

L_9e50:
    /* untranslated: branch HIWORD(lpfl->lpplord):[((LOWORD(lpfl->lpplord) + 0x4) + loword(((lpfl->cord - 1) * 0x12)))] != lpfl->lpplord->rgord[iDel].pt.x ?
     * L_9ec7 : L_9eb6 */

L_9eb6:
    /* untranslated: branch scratch_bp_m16:[(scratch_bp_m18 + loword(((lpfl->cord - 1) * 0x12)))+0x2] != scratch_bp_m1a ? L_9ec7 : L_9ebf */

L_9ebf:
    fRecycle = 0;
    goto L_9ef2;

L_9ec7:
    ord = lpfl->lpplord->rgord[iDel];

L_9ef2:
    fmemmove(((lpfl->lpplord + 0x4) + LOWORD((18 * iDel))), ((lpfl->lpplord + 0x4) + LOWORD(((iDel + 1) * 0x12))),
             LOWORD((((lpfl->cord - iDel) + 0xffff) * 0x12)));
    if ((fRecycle == 0))
        goto L_9fca;
    else
        goto L_9f69;

L_9f69:
    lpfl->lpplord->rgord[(lpfl->cord - 1)] = ord;
    goto L_9fde;

L_9fca:
    lpfl->cord = (lpfl->cord - 1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac - 0x1);

L_9fde:
    return;
}
