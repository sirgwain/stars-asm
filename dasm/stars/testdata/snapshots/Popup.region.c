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

    pt.x = x;
    pt.y = y;
    t_pt_0c9b_1 = PointFrom16(pt);
    ClientToScreen(hwnd, &t_pt_0c9b_1);
    pt = PointTo16(t_pt_0c9b_1);
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    if ((uint16_t)(GlobalPD.grPopup - 1) <= 13) {
        switch (GlobalPD.grPopup) {
        case 1:
            psz = PszGetCompressedString(idsMineralConcentration0000000kt);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dy = 3 * dyArial8 + 8;
            if (GlobalPD.rgi[4] < 0)
                break;
            dy += dyArial8;
            break;
        case 2:
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, NULL);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dx2 = LOWORD(GetTextExtent(hdc, "Player #16", 10)) + 8;
            if (dx2 > dx) {
                dx = dx2;
            }
            dy = dyArial8 * 2 + 8;
            break;
        case 3:
            dxR = 0;
            dxDamage = 0;
            dy = dyArial8 + 8;
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetCompressedString(idsShipName);
            dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            SelectObject(hdc, rghfontArial8[0]);
            for (i = 0; i < 16; i++) {
                if (GlobalPD.lpfl->rgcsh[i] > 0 && (GlobalPD.grbit == 0 || FIsPopupHullType(i) != 0)) {
                    dy += dyArial8;
                    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
                    lpsz = szTB;
                    dx = LOWORD(GetTextExtent(hdc, lpsz, fstrlen(lpsz)));
                    dxL = dxL <= dx ? dx : dxL;
                    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
                    dx = LOWORD(GetTextExtent(hdc, szWork, c));
                    dxR = dxR <= dx ? dx : dxR;
                    if (GlobalPD.fRedDamage != 0 && (GlobalPD.lpfl->rgdv[i].dp >> 7 & 0x1ff) != 0 && dxDamage == 0) {
                        psz = PszGetCompressedString(idsN9999999);
                        dxDamage = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;
                    }
                }
            }
            if (dy == dyArial8 + 8) {
                psz = PszGetCompressedString(idsShipName);
                dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            }
            GlobalPD.dxDamage = dxDamage;
            dx = dxL + dxR + 16 + dxDamage;
            break;
        case 4:
            SelectObject(hdc, rghfontArial8[1]);
            dy = dyArial8 * 4 + 8;
            psz = PszGetCompressedString(idsPlanet);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            psz = PszGetPlanetName(sel.scan.idpl);
            SelectObject(hdc, rghfontArial8[0]);
            dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 4));
            dx += dxName <= dxCoord ? dxCoord : dxName;
            break;
        case 5:
            ptT = PtDisplayPlanetStateInfo(hdc, 0);
            goto SetDxDy;
        case 6:
            ptT = PtDisplayZipOrdInfo(hdc, 0, 0);
            goto SetDxDy;
        case 7:
            ptT = PtDisplayPlanetPopInfo(hdc, 0);
            goto SetDxDy;
        case 12:
            ptT = PtDisplayResourceInfo(hdc, 200, 0);
            goto SetDxDy;
        case 8:
            ptT = PtDisplayFactoryMineInfo(hdc, 200, 0);
            goto SetDxDy;
        case 9:
            dx = (dyArial8 <= 14 ? 0 : 40) + 344;
            dy = dyArial10 + 72 + 12 * dyArial8 + 6;
            break;
        case 10:
            ptT = PtDisplayString(hdc, GlobalPD.dxOut, 0);
            goto SetDxDy;
        case 11:
        case 14:
            mdBuild = GlobalPD.grPopup == grPopupShdef ? mdBuildShdef : mdBuildHuldef;
            lpshdefBuild = GlobalPD.lpshdef;
            UpdateSlotGlobals();
            dx = 340;
            dy = dyArial8 + 306 + 6 * dyArial8 + 8;
            if (gd.mdScreenSize <= 0 || GlobalPD.grPopup != grPopupShdef)
                break;
            dy += 3 * dyArial8;
            break;
        case 13:
            dx = 120;
            dy = 80;
        }
        goto L_1225;
    SetDxDy:
        dx = ptT.x + 2;
        dy = ptT.y + 2;
    }
L_1225:
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    pt.x -= dx;
    pt.y -= dy;
    if (pt.x < GetSystemMetrics(SM_CXSCREEN) - dx) {
        t_merge_126d_0001 = pt.x;
    } else {
        t_call_1265 = GetSystemMetrics(SM_CXSCREEN);
        t_merge_126d_0001 = t_call_1265 - dx;
    }
    if (0 > t_merge_126d_0001) {
        t_merge_12a3_0001 = 0;
    } else if (pt.x < GetSystemMetrics(SM_CXSCREEN) - dx) {
        t_merge_12a3_0001 = pt.x;
    } else {
        t_call_129b = GetSystemMetrics(SM_CXSCREEN);
        t_merge_12a3_0001 = t_call_129b - dx;
    }
    pt.x = t_merge_12a3_0001;
    if (pt.y < GetSystemMetrics(SM_CYSCREEN) - dy) {
        t_merge_12cc_0001 = pt.y;
    } else {
        t_call_12c4 = GetSystemMetrics(SM_CYSCREEN);
        t_merge_12cc_0001 = t_call_12c4 - dy;
    }
    if (0 > t_merge_12cc_0001) {
        t_merge_1302_0001 = 0;
    } else if (pt.y < GetSystemMetrics(SM_CYSCREEN) - dy) {
        t_merge_1302_0001 = pt.y;
    } else {
        t_call_12fa = GetSystemMetrics(SM_CYSCREEN);
        t_merge_1302_0001 = t_call_12fa - dy;
    }
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, NULL, WS_POPUP | WS_VISIBLE | WS_BORDER, pt.x, pt.y, dx, dy, hwnd, NULL, hInst, NULL);
    SendMessage(hwndPopup, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
    SetCapture(hwndPopup);
    return;
}
