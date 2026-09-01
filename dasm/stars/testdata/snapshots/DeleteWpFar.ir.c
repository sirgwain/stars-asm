void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

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
    if ((lpfl->lpplord->rgord[(lpfl->cord - 1)].pt.x != lpfl->lpplord->rgord[iDel].pt.x))
        goto L_9ec7;
    else
        goto L_9eb6;

L_9eb6:
    /* untranslated: branch ss:[bp-0x16]:[(ss:[bp-0x18] + loword(((lpfl->cord - 1) * 0x12)))+0x2] != ss:[bp-0x1a] ? L_9ec7 : L_9ebf */

L_9ebf:
    fRecycle = 0;
    goto L_9ef2;

L_9ec7:
    ord = lpfl->lpplord->rgord[iDel];

L_9ef2:
    fmemmove(&(lpfl->lpplord->rgord[iDel]), &(lpfl->lpplord->rgord[(iDel + 1)]), LOWORD((((lpfl->cord - iDel) + 0xffff) * 0x12)));
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
