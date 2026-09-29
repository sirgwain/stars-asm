void DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur) {
    int32_t lMax;
    int16_t c;
    int16_t fTwoMAs;
    int16_t iMode;
    HBRUSH  hbr;
    int32_t lCur;
    int32_t l;

L_2afa:
    fTwoMAs = iBest >= 0 ? 0 : 1;
    SelectObject(hdc, rghfontArial8[1]);
    if (iCur >= 5)
        goto L_2b32;
    else
        goto L_2b2d;

L_2b2d:
    iCur = 5;

L_2b32:
    if (iBest >= 0)
        goto L_2b43;
    else
        goto L_2b3b;

L_2b3b:
    iBest = -iBest;

L_2b43:
    lMax = (int32_t)(iBest - 1);
    if (iCur > iBest + fTwoMAs)
        goto L_2b67;
    else
        goto L_2b5e;

L_2b5e:
    hbr = hbrPurple;
    goto L_2b87;

L_2b67:
    if (iCur >= iBest + fTwoMAs + 3)
        goto L_2b81;
    else
        goto L_2b78;

L_2b78:
    hbr = hbrYellow;
    goto L_2b87;

L_2b81:
    hbr = hbrRed;

L_2b87:
    lCur = (int32_t)(iCur - 4);
    l = LDrawGauge(hdc, prc, 1, &lCur, &hbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l + 4);
    l = GetTextExtent(hdc, szWork, c);
    RcCtrTextOut(hdc, prc, szWork, c);
    SetBkMode(hdc, iMode);
    return;
}
