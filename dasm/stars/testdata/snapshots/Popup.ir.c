void Popup(HWND hwnd, int16_t x, int16_t y) {
    HDC     hdc;
    POINT16 pt;
    int16_t dy;
    int16_t i;
    int16_t c;
    HFONT   hfontSav;
    char   *psz;
    int16_t dx;
    POINT16 ptT;
    int16_t dx2;
    int16_t dxDamage;
    int16_t dxL;
    char   *lpsz;
    int16_t dxR;
    char    szTB[40];
    int16_t dxName;
    int16_t dxCoord;
    POINT   t_pt_0c9b_1;
    int16_t t_merge_126d_0001;
    int16_t t_call_1265;
    int16_t t_merge_12a3_0001;
    int16_t t_call_129b;
    int16_t t_merge_12cc_0001;
    int16_t t_call_12c4;
    int16_t t_merge_1302_0001;
    int16_t t_call_12fa;

L_0c7c:
    pt.x = x;
    pt.y = y;
    t_pt_0c9b_1 = PointFrom16(pt);
    ClientToScreen(hwnd, &t_pt_0c9b_1);
    pt = PointTo16(t_pt_0c9b_1);
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    goto L_11f5;

L_0cc3:
    psz = PszGetCompressedString(idsMineralConcentration0000000kt);
    dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
    dy = 3 * dyArial8 + 8;
    if (GlobalPD.rgi[4] < 0)
        goto L_1225;
    else
        goto L_0d19;

L_0d19:
    dy += dyArial8;

L_0d1f:
    goto L_1225;

L_0d22:
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, NULL);
    dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
    dx2 = LOWORD(GetTextExtent(hdc, "Player #16", 10)) + 8;
    if (dx2 <= dx)
        goto L_0d9c;
    else
        goto L_0d96;

L_0d96:
    dx = dx2;

L_0d9c:
    dy = dyArial8 * 2 + 8;
    goto L_1225;

L_0daa:
    dxR = 0;
    dxDamage = 0;
    dy = dyArial8 + 8;
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszGetCompressedString(idsShipName);
    dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
    SelectObject(hdc, rghfontArial8[0]);
    i = 0;
    goto L_0f7b;

L_0e0a:
    if (GlobalPD.lpfl->rgcsh[i] <= 0)
        goto L_0f77;
    else
        goto L_0e29;

L_0e29:
    if (GlobalPD.grbit == 0)
        goto L_0e46;
    else
        goto L_0e33;

L_0e33:
    if (FIsPopupHullType(i) == 0)
        goto L_0f77;
    else
        goto L_0e46;

L_0e46:
    dy += dyArial8;
    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
    lpsz = szTB;
    dx = LOWORD(GetTextExtent(hdc, lpsz, fstrlen(lpsz)));
    dxL = dxL <= dx ? dx : dxL;
    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    dxR = dxR <= dx ? dx : dxR;
    if (GlobalPD.fRedDamage == 0)
        goto L_0f77;
    else
        goto L_0f15;

L_0f15:
    if ((GlobalPD.lpfl->rgdv[i].dp >> 7 & 0x1ff) == 0)
        goto L_0f77;
    else
        goto L_0f3e;

L_0f3e:
    if (dxDamage != 0)
        goto L_0f77;
    else
        goto L_0f47;

L_0f47:
    psz = PszGetCompressedString(idsN9999999);
    dxDamage = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;

L_0f77:
    i++;

L_0f7b:
    if (i < 16)
        goto L_0e0a;
    else
        goto L_0f84;

L_0f84:
    if (dy != dyArial8 + 8)
        goto L_0fbf;
    else
        goto L_0f92;

L_0f92:
    psz = PszGetCompressedString(idsShipName);
    dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));

L_0fbf:
    GlobalPD.dxDamage = dxDamage;
    dx = dxL + dxR + 16 + dxDamage;
    goto L_1225;

L_0fd7:
    SelectObject(hdc, rghfontArial8[1]);
    dy = dyArial8 * 4 + 8;
    psz = PszGetCompressedString(idsPlanet);
    dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
    psz = PszGetPlanetName(sel.scan.idpl);
    SelectObject(hdc, rghfontArial8[0]);
    dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
    dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 4));
    dx += dxName <= dxCoord ? dxCoord : dxName;
    goto L_1225;

L_1092:
    ptT = PtDisplayPlanetStateInfo(hdc, 0);
    goto SetDxDy;

L_10aa:
    ptT = PtDisplayZipOrdInfo(hdc, 0, 0);
    goto SetDxDy;

L_10c6:
    ptT = PtDisplayPlanetPopInfo(hdc, 0);
    goto SetDxDy;

L_10de:
    ptT = PtDisplayResourceInfo(hdc, 200, 0);
    goto SetDxDy;

L_10fa:
    ptT = PtDisplayFactoryMineInfo(hdc, 200, 0);

SetDxDy:
    dx = ptT.x + 2;
    dy = ptT.y + 2;
    goto L_1225;

L_1128:
    dx = (dyArial8 <= 14 ? 0 : 40) + 344;
    dy = dyArial10 + 72 + 12 * dyArial8 + 6;
    goto L_1225;

L_115a:
    ptT = PtDisplayString(hdc, GlobalPD.dxOut, 0);
    goto SetDxDy;

L_1176:
    mdBuild = GlobalPD.grPopup == grPopupShdef ? mdBuildShdef : mdBuildHuldef;
    lpshdefBuild = GlobalPD.lpshdef;
    UpdateSlotGlobals();
    dx = 340;
    dy = dyArial8 + 306 + 6 * dyArial8 + 8;
    if (gd.mdScreenSize <= 0)
        goto L_1225;
    else
        goto L_11ce;

L_11ce:
    if (GlobalPD.grPopup != grPopupShdef)
        goto L_1225;
    else
        goto L_11d8;

L_11d8:
    dy += 3 * dyArial8;

L_11e2:
    goto L_1225;

L_11e5:
    dx = 120;
    dy = 80;
    goto L_1225;

L_11f5:
    if (GlobalPD.grPopup - 1 > 13)
        goto L_1225;
    else
        goto L_1200;

L_1200:
    switch ((GlobalPD.grPopup - 1) * 2) {
    case 0x0:
        goto L_0cc3;
    case 0x2:
        goto L_0d22;
    case 0x4:
        goto L_0daa;
    case 0x6:
        goto L_0fd7;
    case 0x8:
        goto L_1092;
    case 0xa:
        goto L_10aa;
    case 0xc:
        goto L_10c6;
    case 0xe:
        goto L_10fa;
    case 0x10:
        goto L_1128;
    case 0x12:
        goto L_115a;
    case 0x14:
        goto L_1176;
    case 0x16:
        goto L_10de;
    case 0x18:
        goto L_11e5;
    case 0x1a:
        goto L_1176;
    }

L_1225:
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    pt.x -= dx;
    pt.y -= dy;
    if (pt.x >= GetSystemMetrics(SM_CXSCREEN) - dx)
        goto L_1261;
    else
        goto L_125b;

L_125b:
    t_merge_126d_0001 = pt.x;
    goto L_126d;

L_1261:
    t_call_1265 = GetSystemMetrics(SM_CXSCREEN);
    t_merge_126d_0001 = t_call_1265 - dx;

L_126d:
    if (0 <= t_merge_126d_0001)
        goto L_127d;
    else
        goto L_1277;

L_1277:
    t_merge_12a3_0001 = 0;
    goto L_12a3;

L_127d:
    if (pt.x >= GetSystemMetrics(SM_CXSCREEN) - dx)
        goto L_1297;
    else
        goto L_1291;

L_1291:
    t_merge_12a3_0001 = pt.x;
    goto L_12a3;

L_1297:
    t_call_129b = GetSystemMetrics(SM_CXSCREEN);
    t_merge_12a3_0001 = t_call_129b - dx;

L_12a3:
    pt.x = t_merge_12a3_0001;
    if (pt.y >= GetSystemMetrics(SM_CYSCREEN) - dy)
        goto L_12c0;
    else
        goto L_12ba;

L_12ba:
    t_merge_12cc_0001 = pt.y;
    goto L_12cc;

L_12c0:
    t_call_12c4 = GetSystemMetrics(SM_CYSCREEN);
    t_merge_12cc_0001 = t_call_12c4 - dy;

L_12cc:
    if (0 <= t_merge_12cc_0001)
        goto L_12dc;
    else
        goto L_12d6;

L_12d6:
    t_merge_1302_0001 = 0;
    goto L_1302;

L_12dc:
    if (pt.y >= GetSystemMetrics(SM_CYSCREEN) - dy)
        goto L_12f6;
    else
        goto L_12f0;

L_12f0:
    t_merge_1302_0001 = pt.y;
    goto L_1302;

L_12f6:
    t_call_12fa = GetSystemMetrics(SM_CYSCREEN);
    t_merge_1302_0001 = t_call_12fa - dy;

L_1302:
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, NULL, WS_POPUP | WS_VISIBLE | WS_BORDER, pt.x, pt.y, dx, dy, hwnd, NULL, hInst, NULL);
    SendMessage(hwndPopup, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
    SetCapture(hwndPopup);
    return;
}
