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

L_1a54:
    pfl = obj.pfl;
    if (ptile->fFixCtls == 0)
        goto L_1a91;
    else
        goto L_1a79;

L_1a79:
    rgrcRef[2].top = -5;
    rgrcRef[2].bottom = -6;
    rgrcRef[3].top = -5;
    rgrcRef[3].bottom = -6;

L_1a91:
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFuelCargo)) == 0)
        goto L_1e6c;
    else
        goto L_1abb;

L_1abb:
    xLeft = rc.left + 4;
    xRight = rc.right - 4;
    yTop = rc.top + 1;
    dxRight = dxMaxMineralQuan;
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsCargo3, szWork);
    l = GetTextExtent(hdc, szWork, c);
    c = CchGetString(idsFuel3, szWork);
    l2 = GetTextExtent(hdc, szWork, c);
    if (l2 <= l)
        goto L_1b65;
    else
        goto L_1b59;

L_1b59:
    l = l2;

L_1b65:
    if (ptile->fMinDraw != 0)
        goto L_1b93;
    else
        goto L_1b7b;

L_1b7b:
    TextOut(hdc, xLeft, yTop, szWork, c);

L_1b93:
    SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
    rgrcRef[2] = rcGauge;
    DrawFleetGauge(hdc, &rcGauge, pfl, 4);
    yTop += (gd.fSmallTileMode == 0 ? 4 : 2) + dyArial8;
    if (ptile->fMinDraw != 0)
        goto L_1c47;
    else
        goto L_1c1c;

L_1c1c:
    c = CchGetString(idsCargo3, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);

L_1c47:
    SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
    rgrcRef[3] = rcGauge;
    DrawFleetGauge(hdc, &rcGauge, pfl, 5);
    yTop += dyArial8 + 4;
    if (gd.fSmallTileMode != 0)
        goto L_1e6c;
    else
        goto L_1cb6;

L_1cb6:
    i = 0;
    goto L_1d9e;

L_1cbe:
    if (ptile->fMinDraw != 0)
        goto L_1d25;
    else
        goto L_1cd4;

L_1cd4:
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, rgcrMinerals[i]);
    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));

L_1d25:
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crButtonText);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[i]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop += dyArial8;
    i++;

L_1d9e:
    if (i <= 2)
        goto L_1cbe;
    else
        goto L_1da7;

L_1da7:
    if (ptile->fMinDraw != 0)
        goto L_1e20;
    else
        goto L_1dbd;

L_1dbd:
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, 0xffffff);
    c = CchGetString(idsColonists2, szWork);
    TextOut(hdc, xLeft, yTop, szWork, c);
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crButtonText);

L_1e20:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[3]);
    RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    yTop += dyArial8;

L_1e6c:
    return;
}
