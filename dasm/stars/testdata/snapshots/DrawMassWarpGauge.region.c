void DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur) {
    int32_t lMax;
    int16_t c;
    int16_t fTwoMAs;
    int16_t iMode;
    HBRUSH  hbr;
    int32_t lCur;
    int32_t l;

    fTwoMAs = iBest < 0;
    SelectObject(hdc, rghfontArial8[1]);
    if (iCur < 5) {
        iCur = 5;
    }
    if (iBest < 0) {
        iBest = -iBest;
    }
    lMax = (int16_t)(iBest - 1);
    if (iCur <= iBest + fTwoMAs) {
        hbr = hbrPurple;
    } else if (iCur < iBest + fTwoMAs + 3) {
        hbr = hbrYellow;
    } else {
        hbr = hbrRed;
    }
    lCur = (int16_t)(iCur - 4);
    l = LDrawGauge(hdc, prc, 1, &lCur, &hbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l + 4);
    l = GetTextExtent(hdc, szWork, c);
    RcCtrTextOut(hdc, prc, szWork, c);
    SetBkMode(hdc, iMode);
    return;
}
