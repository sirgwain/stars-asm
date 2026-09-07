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
    /* untranslated: branch HIWORD(lpfl->lpplord):[((LOWORD(lpfl->lpplord) + 0x4) + loword(((lpfl->cord - 1) * 0x12)))] != part[0x4:2](lpfl->lpplord[iDel*0x12])
     * ? L_9ec7 : L_9eb6 */

L_9eb6:
    /* untranslated: branch scratch_bp_m16:[(scratch_bp_m18 + loword(((lpfl->cord - 1) * 0x12)))+0x2] != scratch_bp_m1a ? L_9ec7 : L_9ebf */

L_9ebf:
    fRecycle = 0;
    goto L_9ef2;

L_9ec7:
    /* untranslated: ord = part[0x4:18](lpfl->lpplord[iDel*0x12]) */

L_9ef2:
    /* untranslated: call fmemmove(words(HIWORD(lpfl->lpplord), ((LOWORD(lpfl->lpplord) + 0x4) + loword((18 * iDel)))), words(HIWORD(lpfl->lpplord),
     * ((LOWORD(lpfl->lpplord) + 0x4) + loword(((iDel + 1) * 0x12)))), loword((((lpfl->cord - iDel) + 0xffff) * 0x12))) -> callresult(void *) */
    if ((fRecycle == 0))
        goto L_9fca;
    else
        goto L_9f69;

L_9f69:
    /* untranslated: part[0x4:18](lpfl->lpplord[(load([load(dword [bp+0x6])+0x62]) + 0xffff)*0x12]) = ord */
    goto L_9fde;

L_9fca:
    lpfl->cord = (lpfl->cord - 1);
    lpfl->lpplord->iordMac = (lpfl->lpplord->iordMac - 0x1);

L_9fde:
    return;
}
