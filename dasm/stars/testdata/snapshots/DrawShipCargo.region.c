void DrawShipCargo(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t dxRight;
    int32_t l2;
    int16_t yTop;
    int16_t i;
    int16_t c;
    FLEET  *pfl;
    int16_t xRight;
    RECT    rcGauge;
    int16_t xLeft;
    int32_t l;
    RECT    rc;

    pfl = obj.pfl;
    if (ptile->fFixCtls != 0x0) {
        rgrcRef[2].top = -5;
        rgrcRef[2].bottom = -6;
        rgrcRef[3].top = -5;
        rgrcRef[3].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFuelCargo)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 1;
        dxRight = dxMaxMineralQuan;
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsCargo3, szWork);
        l = GetTextExtent(hdc, szWork, c);
        c = CchGetString(idsFuel3, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        if (ptile->fMinDraw == 0x0) {
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[2] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 4);
        yTop = yTop + ((gd.fSmallTileMode == 0x0 ? 4 : 2) + dyArial8);
        if (ptile->fMinDraw == 0x0) {
            c = CchGetString(idsCargo3, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[3] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 5);
        yTop = yTop + (dyArial8 + 4);
        if (gd.fSmallTileMode == 0x0) {
            for (i = 0; i <= 2; i++) {
                if (ptile->fMinDraw == 0x0) {
                    SelectObject(hdc, rghfontArial8[1]);
                    SetTextColor(hdc, rgcrMinerals[i]);
                    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));
                }
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
                c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[i]);
                RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
                yTop = yTop + dyArial8;
            }
            if (ptile->fMinDraw == 0x0) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, 0xffffff);
                c = CchGetString(idsColonists2, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
            }
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[3]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
        }
    }
    return;
}
