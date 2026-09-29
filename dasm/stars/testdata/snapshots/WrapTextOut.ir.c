void WrapTextOut(HDC hdc, int16_t *px, int16_t *py, char *psz, int16_t cLen, int16_t xLeft, int16_t dxWidth, int16_t *pxMax, int16_t fNewLine, int16_t fPrint) {
    int16_t dxRemain;
    char   *pchEnd;
    int16_t fItFit;
    int16_t xRight;
    int16_t dx;
    char   *pchStart;
    char   *pch;

L_25fe:
    xRight = xLeft + dxWidth;
    dxRemain = dxWidth - (*px - xLeft);
    if (cLen != 0)
        goto L_2637;
    else
        goto L_2629;

L_2629:
    cLen = strlen(psz);

L_2637:
    if (fNewLine == 0)
        goto L_2650;
    else
        goto L_2640;

L_2640:
    *py = *py + dyArial8;
    *px = xLeft;

L_2650:
    pchStart = psz;

Top:
    pch = pchStart;
    pchEnd = pchStart + cLen;
    ChopTrailingSpaces(pch, &pchEnd);
    dx = LOWORD(GetTextExtent(hdc, pch, pchEnd - pch));
    fItFit = 1;

L_2694:
    if (dx <= dxRemain)
        goto L_26e3;
    else
        goto L_269f;

L_269f:
    if (pch >= pchEnd)
        goto L_26e3;
    else
        goto L_26aa;

L_26aa:
    if (dx <= 0)
        goto L_26e3;
    else
        goto L_26b3;

L_26b3:
    fItFit = 0;
    ChopLastWord(pch, &pchEnd);
    dx = LOWORD(GetTextExtent(hdc, pch, pchEnd - pch));
    goto L_2694;

L_26e3:
    if (fItFit == 0)
        goto L_271a;
    else
        goto L_26ec;

L_26ec:
    AddBackTrailingSpaces(&pchEnd, pchStart + cLen);
    dx = LOWORD(GetTextExtent(hdc, pchStart, pchEnd - pchStart));

L_271a:
    if (pchStart != pchEnd)
        goto L_275f;
    else
        goto L_2728;

L_2728:
    if (*px != xLeft)
        goto WrapIt;
    else
        goto L_2735;

L_2735:
    pchEnd = pchStart + cLen;
    dx = LOWORD(GetTextExtent(hdc, pchStart, pchEnd - pchStart));

L_275f:
    if (fPrint == 0)
        goto L_2788;
    else
        goto L_2768;

L_2768:
    TextOut(hdc, *px, *py, pchStart, pchEnd - pchStart);

L_2788:
    *px = *px + dx;
    if (pxMax == 0x0)
        goto L_27b2;
    else
        goto L_2799;

L_2799:
    if (*px <= *pxMax)
        goto L_27b2;
    else
        goto L_27a8;

L_27a8:
    *pxMax = *px;

L_27b2:
    if (pchEnd == pchStart + cLen)
        goto L_2805;
    else
        goto WrapIt;

WrapIt:
    AddBackTrailingSpaces(&pchEnd, pchStart + cLen);
    cLen = cLen - (pchEnd - pchStart);
    pchStart = pchEnd;
    *py = *py + dyArial8;
    *px = xLeft;
    dxRemain = dxWidth;
    goto Top;

L_2805:
    return;
}
