void WrapTextOut(HDC hdc, int16_t *px, int16_t *py, char *psz, int16_t cLen, int16_t xLeft, int16_t dxWidth, int16_t *pxMax, int16_t fNewLine, int16_t fPrint) {
    int16_t dxRemain;
    char   *pchEnd;
    int16_t fItFit;
    int16_t xRight;
    int16_t dx;
    char   *pchStart;
    char   *pch;

    xRight = xLeft + dxWidth;
    dxRemain = dxWidth - (*px - xLeft);
    if (cLen == 0) {
        cLen = strlen(psz);
    }
    if (fNewLine != 0) {
        *py += dyArial8;
        *px = xLeft;
    }
    pchStart = psz;
    while (1) {
        pch = pchStart;
        pchEnd = pchStart + cLen;
        ChopTrailingSpaces(pch, &pchEnd);
        dx = LOWORD(GetTextExtent(hdc, pch, pchEnd - pch));
        fItFit = 1;
        for (; dx > dxRemain && pch < pchEnd && dx > 0; dx = LOWORD(GetTextExtent(hdc, pch, pchEnd - pch))) {
            fItFit = 0;
            ChopLastWord(pch, &pchEnd);
        }
        if (fItFit != 0) {
            AddBackTrailingSpaces(&pchEnd, pchStart + cLen);
            dx = LOWORD(GetTextExtent(hdc, pchStart, pchEnd - pchStart));
        }
        if (pchStart == pchEnd) {
            if (*px != xLeft)
                goto WrapIt;
            pchEnd = pchStart + cLen;
            dx = LOWORD(GetTextExtent(hdc, pchStart, pchEnd - pchStart));
        }
        if (fPrint != 0) {
            TextOut(hdc, *px, *py, pchStart, pchEnd - pchStart);
        }
        *px += dx;
        if (pxMax != 0 && *px > *pxMax) {
            *pxMax = *px;
        }
        if (pchEnd == pchStart + cLen)
            break;
    WrapIt:
        AddBackTrailingSpaces(&pchEnd, pchStart + cLen);
        cLen -= pchEnd - pchStart;
        pchStart = pchEnd;
        *py += dyArial8;
        *px = xLeft;
        dxRemain = dxWidth;
    }
    return;
}
