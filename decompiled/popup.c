#include "common.h"

uint16_t mpimdgrbitBU[8] = {8, 8, 16, 32, 128, 64, 8, 8};

int32_t PopupWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rc;

L_0000:
    goto L_00fb;

L_0012:
    goto L_0046;

L_0018:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 1;

L_0046:
    if ((GlobalPD.grPopup == grPopupComponent))
        goto L_0018;
    else
        goto L_004e;

L_004e:
    if ((GlobalPD.grPopup == grPopupShdef))
        goto L_0018;
    else
        goto L_0056;

L_0056:
    if ((GlobalPD.grPopup != grPopupShdefSB))
        goto Default;
    else
        goto L_005b;

L_005b:
    goto L_0018;

L_0064:
    hdc = BeginPaint(hwnd, &(ps));
    DrawPopup(hwnd, hdc);
    EndPaint(hwnd, &(ps));
    goto L_0126;

L_0096:
    if ((hwndPopup == 0x0))
        goto L_0126;
    else
        goto L_00a0;

L_00a0:
    DestroyWindow(hwndPopup);
    hwndPopup = 0x0;
    GlobalPD.grPopup = 0x0;
    ReleaseCapture();
    if ((gd.fTutorial == 0x0))
        goto L_0126;
    else
        goto L_00cd;

L_00cd:
    tutor.fProgress = 0x1;
    AdvanceTutor();

L_00de:
    goto L_0126;

Default:
    return DefWindowProc(hwnd, message, wParam, lParam);

L_00fb:
    if ((message == WM_CREATE))
        goto L_0126;
    else
        goto L_0103;

L_0103:
    if ((message == WM_PAINT))
        goto L_0064;
    else
        goto L_010b;

L_010b:
    if ((message == WM_ERASEBKGND))
        goto L_0012;
    else
        goto L_0113;

L_0113:
    if ((message == WM_LBUTTONUP))
        goto L_0096;
    else
        goto L_011b;

L_011b:
    if ((message != WM_RBUTTONUP))
        goto Default;
    else
        goto L_0120;

L_0120:
    goto L_0096;

L_0126:
    return 0;
}

int16_t FIsPopupHullType(int16_t ishdef) {
    uint16_t imd;

L_0148:
    if ((GlobalPD.grbit == 0x0))
        goto L_0166;
    else
        goto L_015b;

L_015b:
    if ((GlobalPD.grbit != 0xff))
        goto L_016c;
    else
        goto L_0166;

L_0166:
    return 1;

L_016c:
    imd = LphuldefFromId(rglpshdef[GlobalPD.lpfl->iPlayer][ishdef].hul.ihuldef)->imdCategory;
    return (mpimdgrbitBU[imd] & GlobalPD.grbit);
}

void DrawPopup(HWND hwnd, HDC hdc) {
    COLORREF crBack;
    char     szT[80];
    int16_t  yCur;
    int16_t  i;
    int16_t  c;
    int16_t  bkMode;
    HFONT    hfontSav;
    char    *psz;
    int16_t  dx;
    COLORREF crFore;
    RECT     rc;
    char    *lpsz;
    int16_t  csh;
    int16_t  dpT;
    char     szTB[40];
    StringId t_merge_03dc_0001;
    uint16_t t_merge_0b79_0001;

L_01c0:
    crBack = SetBkColor(hdc, 0xffffff);
    crFore = SetTextColor(hdc, 0x0);
    bkMode = SetBkMode(hdc, OPAQUE);
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    GetClientRect(hwnd, &(rc));
    goto L_0c14;

L_022b:
    CtrTextOut(hdc, (rc.right >> 0x1), 4, rgszMinerals[GlobalPD.rgi[0]], 0);
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(idsMineralConcentration);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4);
    RightTextOut(hdc, dx, (dyArial8 + 4), PszGetCompressedString(idsSurface), 0, 0);
    if ((HIWORD(GlobalPD.rgi[2]) < 0x0))
        goto L_02fc;
    else
        goto L_02c9;

L_02c9:
    if ((HIWORD(GlobalPD.rgi[2]) > 0x0))
        goto L_02d8;
    else
        goto L_02ce;

L_02ce:
    if ((LOWORD(GlobalPD.rgi[2]) < 0x0))
        goto L_02fc;
    else
        goto L_02d8;

L_02d8:
    c = _wsprintf(szWork, "%ldkT", LOWORD(GlobalPD.rgi[2]), HIWORD(GlobalPD.rgi[2]));
    goto L_0324;

L_02fc:
    strcpy(szWork, PszGetCompressedString(idsUnknown2));
    c = strlen(szWork);

L_0324:
    TextOut(hdc, dx, (dyArial8 + 4), szWork, c);
    RightTextOut(hdc, dx, ((dyArial8 * 2) + 4), PszGetCompressedString(idsMineralConcentration), 0, 0);
    if ((GlobalPD.iPlrMax < 0))
        goto L_0401;
    else
        goto L_0376;

L_0376:
    if ((GlobalPD.iPlrMax > 0))
        goto L_0385;
    else
        goto L_037b;

L_037b:
    if ((LOWORD(GlobalPD.rgi[3]) <= 0x0))
        goto L_0401;
    else
        goto L_0385;

L_0385:
    c = _wsprintf(szWork, PCTLD, LOWORD(GlobalPD.rgi[3]), HIWORD(GlobalPD.rgi[3]));
    if ((LOWORD(GlobalPD.rgi[1]) != 0x0))
        goto L_03ba;
    else
        goto L_03b0;

L_03b0:
    if ((HIWORD(GlobalPD.rgi[1]) == 0x0))
        goto L_0423;
    else
        goto L_03ba;

L_03ba:
    if ((GlobalPD.iPlrMax < 0))
        goto L_03d9;
    else
        goto L_03c4;

L_03c4:
    if ((GlobalPD.iPlrMax > 0))
        goto L_03d3;
    else
        goto L_03c9;

L_03c9:
    if ((LOWORD(GlobalPD.rgi[3]) < 0x1e))
        goto L_03d9;
    else
        goto L_03d3;

L_03d3:
    t_merge_03dc_0001 = idsHw;
    goto L_03dc;

L_03d9:
    t_merge_03dc_0001 = idsN30;

L_03dc:
    c = (c + _wsprintf(&(szWork[c]), PszGetCompressedString(t_merge_03dc_0001)));

L_03fe:
    goto L_0423;

L_0401:
    c = _wsprintf(szWork, PszGetCompressedString(idsUnknown2));

L_0423:
    TextOut(hdc, dx, ((dyArial8 * 2) + 4), szWork, c);
    if ((HIWORD(GlobalPD.rgi[4]) < 0x0))
        goto L_0c44;
    else
        goto L_044b;

L_044b:
    if ((HIWORD(GlobalPD.rgi[4]) > 0x0))
        goto L_045a;
    else
        goto L_0450;

L_0450:
    if ((LOWORD(GlobalPD.rgi[4]) < 0x0))
        goto L_0c44;
    else
        goto L_045a;

L_045a:
    RightTextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 4), PszGetCompressedString(idsMiningRate), 0, 0);
    CchGetString(idsLdktYr, szT);
    c = _wsprintf(szWork, szT, LOWORD(GlobalPD.rgi[4]), HIWORD(GlobalPD.rgi[4]));
    TextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 4), szWork, c);

L_04d9:
    goto L_0c44;

L_04dc:
    CtrTextOut(hdc, (rc.right >> 0x1), 4, PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, 0x0), 0);
    c = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), (GlobalPD.iPlayer + 1));
    CtrTextOut(hdc, (rc.right >> 0x1), (dyArial8 + 4), szWork, c);
    goto L_0c44;

L_0561:
    if (((rc.bottom - rc.top) >= (dyArial8 * 2)))
        goto L_05b0;
    else
        goto L_0574;

L_0574:
    c = CchGetString(idsNone2, szWork);
    TextOut(hdc, 4, 4, szWork, c);
    SelectObject(hdc, rghfontArial8[0]);
    goto L_0c44;

L_05b0:
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsShipName, szWork);
    TextOut(hdc, 4, 4, szWork, c);
    RightTextOut(hdc, ((rc.right - 4) - GlobalPD.dxDamage), 4, "#", 1, 0);
    if ((GlobalPD.dxDamage == 0))
        goto L_0673;
    else
        goto L_0619;

L_0619:
    c = CchGetString(idsDamage2, szWork);
    RightTextOut(hdc, (rc.right - 4), 4, szWork, c, 0);
    PatBlt(hdc, 4, (dyArial8 + 2), (rc.right - 8), 1, BLACKNESS);

L_0673:
    SelectObject(hdc, rghfontArial8[0]);
    yCur = (dyArial8 + 4);
    i = 0;
    goto L_08d1;

L_0690:
    if ((GlobalPD.lpfl->rgcsh[i] <= 0))
        goto L_08cd;
    else
        goto L_06af;

L_06af:
    if ((GlobalPD.grbit == 0x0))
        goto L_06cc;
    else
        goto L_06b9;

L_06b9:
    if ((FIsPopupHullType(i) == 0))
        goto L_08cd;
    else
        goto L_06cc;

L_06cc:
    if ((GlobalPD.fRedDamage == 0))
        goto L_0718;
    else
        goto L_06d6;

L_06d6:
    if ((GlobalPD.lpfl->rgdv[i].dp == 0x0))
        goto L_0708;
    else
        goto L_06f5;

L_06f5:
    SetTextColor(hdc, 0xff);
    goto L_0718;

L_0708:
    SetTextColor(hdc, 0x0);

L_0718:
    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
    lpsz = szTB;
    TextOut(hdc, 4, yCur, lpsz, fstrlen(lpsz));
    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
    RightTextOut(hdc, ((rc.right - 4) - GlobalPD.dxDamage), yCur, szWork, c, 0);
    if ((GlobalPD.fRedDamage == 0))
        goto L_08c7;
    else
        goto L_07c8;

L_07c8:
    if ((GlobalPD.lpfl->rgdv[i].dp == 0x0))
        goto L_08c7;
    else
        goto L_07e7;

L_07e7:
    csh = GlobalPD.lpfl->rgcsh[i];
    csh = LOWORD((int32_t)(((uint32_t)(((uint32_t)((GlobalPD.lpfl->rgdv[i].dp & 0x7f)) * (uint32_t)(csh))) / 0x64)));
    if ((csh > 0))
        goto L_084f;
    else
        goto L_084a;

L_084a:
    csh = 1;

L_084f:
    dpT = ((uint32_t)(((GlobalPD.lpfl->rgdv[i].dp >> 0x7) & 0x1ff)) / 0x5);
    if ((dpT != 0))
        goto L_0888;
    else
        goto L_0883;

L_0883:
    dpT = 1;

L_0888:
    c = _wsprintf(szWork, "%d@%d%%", csh, dpT);
    RightTextOut(hdc, (rc.right - 4), yCur, szWork, c, 0);

L_08c7:
    yCur = (yCur + dyArial8);

L_08cd:
    i = (i + 1);

L_08d1:
    if ((i < 16))
        goto L_0690;
    else
        goto L_08da;

L_08da:
    if ((GlobalPD.fRedDamage == 0))
        goto L_0c44;
    else
        goto L_08e4;

L_08e4:
    SetTextColor(hdc, 0x0);

L_08f4:
    goto L_0c44;

L_08f7:
    psz = PszGetCompressedString(idsPlanet);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4);
    SelectObject(hdc, rghfontArial8[1]);
    RightTextOut(hdc, dx, 4, psz, strlen(psz), 0);
    RightTextOut(hdc, dx, (dyArial8 + 4), PszGetCompressedString(idsId), 0, 0);
    RightTextOut(hdc, dx, ((dyArial8 * 2) + 4), PszGetCompressedString(idsX), 0, 0);
    RightTextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 4), PszGetCompressedString(idsY), 0, 0);
    psz = PszGetPlanetName(sel.scan.idpl);
    SelectObject(hdc, rghfontArial8[0]);
    TextOut(hdc, dx, 4, psz, strlen(psz));
    c = _wsprintf(szWork, PCTD, (sel.scan.idpl + 1));
    TextOut(hdc, dx, (dyArial8 + 4), szWork, c);
    c = _wsprintf(szWork, PCTD, sel.scan.pt.x);
    TextOut(hdc, dx, ((dyArial8 * 2) + 4), szWork, c);
    c = _wsprintf(szWork, PCTD, sel.scan.pt.y);
    TextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 4), szWork, c);
    goto L_0c44;

L_0ad0:
    PtDisplayPlanetStateInfo(hdc, 1);
    goto L_0c44;

L_0ae2:
    PtDisplayZipOrdInfo(hdc, (rc.right >> 0x1), 1);
    goto L_0c44;

L_0afa:
    PtDisplayPlanetPopInfo(hdc, 1);
    goto L_0c44;

L_0b0c:
    PtDisplayFactoryMineInfo(hdc, rc.right, 1);
    goto L_0c44;

L_0b21:
    PtDisplayResourceInfo(hdc, rc.right, 1);
    goto L_0c44;

L_0b36:
    DisplayComponentInfo(hdc, rc.right, rc.bottom, &(GlobalPD.part));
    goto L_0c44;

L_0b4e:
    PtDisplayString(hdc, rc.right, 1);
    goto L_0c44;

L_0b63:
    if ((GlobalPD.lpshdef->hul.ihuldef < ihuldefOrbitalFort))
        goto L_0b76;
    else
        goto L_0b70;

L_0b70:
    t_merge_0b79_0001 = 0x1;
    goto L_0b79;

L_0b76:
    t_merge_0b79_0001 = 0x0;

L_0b79:
    fStarbaseMode = t_merge_0b79_0001;
    DrawSlotDlg(hwnd, hdc, &(rc), -1);
    rc.top = (dyArial8 + 306);
    rc.left = (rc.left + 6);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkMode(hdc, TRANSPARENT);
    fstrcpy(szWork, GlobalPD.lpshdef->hul.szClass);
    CtrTextOut(hdc, (((rc.right - 76) >> 0x1) + 0x4c), 6, szWork, 0);
    DrawBuildSelHull(hwnd, hdc, -1, &(rc));
    goto L_0c44;

L_0c14:
    if (((GlobalPD.grPopup - 1) > 0xd))
        goto L_0c44;
    else
        goto L_0c1f;

L_0c1f:
    switch (((GlobalPD.grPopup - 1) * 0x2)) {
    case 0x0:
        goto L_022b;
    case 0x2:
        goto L_04dc;
    case 0x4:
        goto L_0561;
    case 0x6:
        goto L_08f7;
    case 0x8:
        goto L_0ad0;
    case 0xa:
        goto L_0ae2;
    case 0xc:
        goto L_0afa;
    case 0xe:
        goto L_0b0c;
    case 0x10:
        goto L_0b36;
    case 0x12:
        goto L_0b4e;
    case 0x14:
        goto L_0b63;
    case 0x16:
        goto L_0b21;
    case 0x18:
        goto L_0c44;
    case 0x1a:
        goto L_0b63;
    }

L_0c44:
    SelectObject(hdc, hfontSav);
    SetBkMode(hdc, bkMode);
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    return;
}

void Popup(HWND hwnd, int16_t x, int16_t y) {
    HDC      hdc;
    POINT    pt;
    int16_t  dy;
    int16_t  i;
    int16_t  c;
    HFONT    hfontSav;
    char    *psz;
    int16_t  dx;
    POINT    ptT;
    int16_t  dx2;
    int16_t  dxDamage;
    int16_t  dxL;
    char    *lpsz;
    int16_t  dxR;
    char     szTB[40];
    int16_t  dxName;
    int16_t  dxCoord;
    int16_t  t_merge_0eaa_0001;
    int16_t  t_merge_0f08_0001;
    int16_t  t_merge_108c_0001;
    POINT    t_call_1099;
    POINT    t_call_10b5;
    POINT    t_call_10cd;
    POINT    t_call_10e9;
    POINT    t_call_1105;
    uint16_t t_merge_113b_0001;
    POINT    t_call_1165;
    uint16_t t_merge_1189_0001;
    int16_t  t_merge_126d_0001;
    int16_t  t_call_1265;
    uint16_t t_merge_12a3_0001;
    int16_t  t_call_129b;
    int16_t  t_merge_12cc_0001;
    int16_t  t_call_12c4;
    uint16_t t_merge_1302_0001;
    int16_t  t_call_12fa;

L_0c7c:
    pt.x = x;
    pt.y = y;
    ClientToScreen(hwnd, &(pt));
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    goto L_11f5;

L_0cc3:
    psz = PszGetCompressedString(idsMineralConcentration0000000kt);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    dy = (LOWORD((3 * dyArial8)) + 8);
    if ((HIWORD(GlobalPD.rgi[4]) < 0x0))
        goto L_1225;
    else
        goto L_0d0a;

L_0d0a:
    if ((HIWORD(GlobalPD.rgi[4]) > 0x0))
        goto L_0d19;
    else
        goto L_0d0f;

L_0d0f:
    if ((LOWORD(GlobalPD.rgi[4]) < 0x0))
        goto L_1225;
    else
        goto L_0d19;

L_0d19:
    dy = (dy + dyArial8);

L_0d1f:
    goto L_1225;

L_0d22:
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, 0x0);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    dx2 = (LOWORD(GetTextExtent(hdc, "Player #16", 10)) + 8);
    if ((dx2 <= dx))
        goto L_0d9c;
    else
        goto L_0d96;

L_0d96:
    dx = dx2;

L_0d9c:
    dy = ((dyArial8 * 2) + 8);
    goto L_1225;

L_0daa:
    dxR = 0;
    dxDamage = 0;
    dy = (dyArial8 + 8);
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszGetCompressedString(idsShipName);
    dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
    SelectObject(hdc, rghfontArial8[0]);
    i = 0;
    goto L_0f7b;

L_0e0a:
    if ((GlobalPD.lpfl->rgcsh[i] <= 0))
        goto L_0f77;
    else
        goto L_0e29;

L_0e29:
    if ((GlobalPD.grbit == 0x0))
        goto L_0e46;
    else
        goto L_0e33;

L_0e33:
    if ((FIsPopupHullType(i) == 0))
        goto L_0f77;
    else
        goto L_0e46;

L_0e46:
    dy = (dy + dyArial8);
    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
    lpsz = szTB;
    dx = LOWORD(GetTextExtent(hdc, lpsz, fstrlen(lpsz)));
    if ((dxL <= dx))
        goto L_0ea7;
    else
        goto L_0ea1;

L_0ea1:
    t_merge_0eaa_0001 = dxL;
    goto L_0eaa;

L_0ea7:
    t_merge_0eaa_0001 = dx;

L_0eaa:
    dxL = t_merge_0eaa_0001;
    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    if ((dxR <= dx))
        goto L_0f05;
    else
        goto L_0eff;

L_0eff:
    t_merge_0f08_0001 = dxR;
    goto L_0f08;

L_0f05:
    t_merge_0f08_0001 = dx;

L_0f08:
    dxR = t_merge_0f08_0001;
    if ((GlobalPD.fRedDamage == 0))
        goto L_0f77;
    else
        goto L_0f15;

L_0f15:
    if ((((GlobalPD.lpfl->rgdv[i].dp >> 0x7) & 0x1ff) == 0x0))
        goto L_0f77;
    else
        goto L_0f3e;

L_0f3e:
    if ((dxDamage != 0))
        goto L_0f77;
    else
        goto L_0f47;

L_0f47:
    psz = PszGetCompressedString(idsN9999999);
    dxDamage = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4);

L_0f77:
    i = (i + 1);

L_0f7b:
    if ((i < 16))
        goto L_0e0a;
    else
        goto L_0f84;

L_0f84:
    if ((dy != (dyArial8 + 8)))
        goto L_0fbf;
    else
        goto L_0f92;

L_0f92:
    psz = PszGetCompressedString(idsShipName);
    dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));

L_0fbf:
    GlobalPD.dxDamage = dxDamage;
    dx = (((dxL + dxR) + 16) + dxDamage);
    goto L_1225;

L_0fd7:
    SelectObject(hdc, rghfontArial8[1]);
    dy = ((dyArial8 * 4) + 8);
    psz = PszGetCompressedString(idsPlanet);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    psz = PszGetPlanetName(sel.scan.idpl);
    SelectObject(hdc, rghfontArial8[0]);
    dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
    dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 4));
    if ((dxName <= dxCoord))
        goto L_1089;
    else
        goto L_1083;

L_1083:
    t_merge_108c_0001 = dxName;
    goto L_108c;

L_1089:
    t_merge_108c_0001 = dxCoord;

L_108c:
    dx = (dx + t_merge_108c_0001);
    goto L_1225;

L_1092:
    t_call_1099 = PtDisplayPlanetStateInfo(hdc, 0);
    ptT.x = LOWORD(t_call_1099);
    ptT.y = HIWORD(t_call_1099);
    goto SetDxDy;

L_10aa:
    t_call_10b5 = PtDisplayZipOrdInfo(hdc, 0, 0);
    ptT.x = LOWORD(t_call_10b5);
    ptT.y = HIWORD(t_call_10b5);
    goto SetDxDy;

L_10c6:
    t_call_10cd = PtDisplayPlanetPopInfo(hdc, 0);
    ptT.x = LOWORD(t_call_10cd);
    ptT.y = HIWORD(t_call_10cd);
    goto SetDxDy;

L_10de:
    t_call_10e9 = PtDisplayResourceInfo(hdc, 200, 0);
    ptT.x = LOWORD(t_call_10e9);
    ptT.y = HIWORD(t_call_10e9);
    goto SetDxDy;

L_10fa:
    t_call_1105 = PtDisplayFactoryMineInfo(hdc, 200, 0);
    ptT.x = LOWORD(t_call_1105);
    ptT.y = HIWORD(t_call_1105);

SetDxDy:
    dx = (ptT.x + 2);
    dy = (ptT.y + 2);
    goto L_1225;

L_1128:
    if ((dyArial8 <= 14))
        goto L_1138;
    else
        goto L_1132;

L_1132:
    t_merge_113b_0001 = 0x28;
    goto L_113b;

L_1138:
    t_merge_113b_0001 = 0x0;

L_113b:
    dx = (t_merge_113b_0001 + 344);
    dy = (((dyArial10 + 72) + LOWORD((12 * dyArial8))) + 6);
    goto L_1225;

L_115a:
    t_call_1165 = PtDisplayString(hdc, GlobalPD.dxOut, 0);
    ptT.x = LOWORD(t_call_1165);
    ptT.y = HIWORD(t_call_1165);
    goto SetDxDy;

L_1176:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_1186;
    else
        goto L_1180;

L_1180:
    t_merge_1189_0001 = 0x0;
    goto L_1189;

L_1186:
    t_merge_1189_0001 = 0x1;

L_1189:
    mdBuild = t_merge_1189_0001;
    lpshdefBuild = GlobalPD.lpshdef;
    UpdateSlotGlobals();
    dx = 340;
    dy = (((dyArial8 + 306) + LOWORD((6 * dyArial8))) + 8);
    if ((gd.mdScreenSize <= 0x0))
        goto L_1225;
    else
        goto L_11ce;

L_11ce:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_1225;
    else
        goto L_11d8;

L_11d8:
    dy = (dy + LOWORD((3 * dyArial8)));

L_11e2:
    goto L_1225;

L_11e5:
    dx = 120;
    dy = 80;
    goto L_1225;

L_11f5:
    if (((GlobalPD.grPopup - 1) > 0xd))
        goto L_1225;
    else
        goto L_1200;

L_1200:
    switch (((GlobalPD.grPopup - 1) * 0x2)) {
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
    pt.x = (pt.x - dx);
    pt.y = (pt.y - dy);
    if ((pt.x >= (GetSystemMetrics(SM_CXSCREEN) - dx)))
        goto L_1261;
    else
        goto L_125b;

L_125b:
    t_merge_126d_0001 = pt.x;
    goto L_126d;

L_1261:
    t_call_1265 = GetSystemMetrics(SM_CXSCREEN);
    t_merge_126d_0001 = (t_call_1265 - dx);

L_126d:
    if ((0 <= t_merge_126d_0001))
        goto L_127d;
    else
        goto L_1277;

L_1277:
    t_merge_12a3_0001 = 0x0;
    goto L_12a3;

L_127d:
    if ((pt.x >= (GetSystemMetrics(SM_CXSCREEN) - dx)))
        goto L_1297;
    else
        goto L_1291;

L_1291:
    t_merge_12a3_0001 = pt.x;
    goto L_12a3;

L_1297:
    t_call_129b = GetSystemMetrics(SM_CXSCREEN);
    t_merge_12a3_0001 = (t_call_129b - dx);

L_12a3:
    pt.x = t_merge_12a3_0001;
    if ((pt.y >= (GetSystemMetrics(SM_CYSCREEN) - dy)))
        goto L_12c0;
    else
        goto L_12ba;

L_12ba:
    t_merge_12cc_0001 = pt.y;
    goto L_12cc;

L_12c0:
    t_call_12c4 = GetSystemMetrics(SM_CYSCREEN);
    t_merge_12cc_0001 = (t_call_12c4 - dy);

L_12cc:
    if ((0 <= t_merge_12cc_0001))
        goto L_12dc;
    else
        goto L_12d6;

L_12d6:
    t_merge_1302_0001 = 0x0;
    goto L_1302;

L_12dc:
    if ((pt.y >= (GetSystemMetrics(SM_CYSCREEN) - dy)))
        goto L_12f6;
    else
        goto L_12f0;

L_12f0:
    t_merge_1302_0001 = pt.y;
    goto L_1302;

L_12f6:
    t_call_12fa = GetSystemMetrics(SM_CYSCREEN);
    t_merge_1302_0001 = (t_call_12fa - dy);

L_1302:
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, 0x0, 0x90800000, pt.x, pt.y, dx, dy, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndPopup, WM_SETFONT, rghfontArial8[0], 0);
    SetCapture(hwndPopup);
    return;
}

int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char    *pszTitle;
    int16_t  tpm;
    POINT    pt;
    int16_t  i;
    char     szTemp[128];
    HMENU    hmenuSub;
    HMENU    hmenuPopup;
    char    *pszT;
    char    *psz;
    MSG      msg;
    int16_t  fChecked;
    int16_t  fCheckedCur;
    uint16_t t_merge_1592_0001;
    uint16_t t_merge_15f4_0001;
    uint16_t t_merge_163a_0001;
    uint16_t t_merge_171c_0001;
    uint16_t t_merge_175a_0001;
    int32_t  t_merge_184f_0001_wide;
    uint16_t t_merge_186e_0001;

L_136c:
    hmenuSub = 0x0;
    pt.x = x;
    pt.y = y;
    ClientToScreen(hwnd, &(pt));
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    i = 0;
    goto L_1887;

L_13ad:
    if ((rgids == 0x0))
        goto L_15ad;
    else
        goto L_13b6;

L_13b6:
    if ((iChecked != -2))
        goto L_13c8;
    else
        goto L_13bf;

L_13bf:
    if ((rgsz != 0x0))
        goto L_15ad;
    else
        goto L_13c8;

L_13c8:
    if ((LOWORD(rgids[i]) != 0xffff))
        goto L_1401;
    else
        goto L_13dc;

L_13dc:
    if ((HIWORD(rgids[i]) != 0xffff))
        goto L_1401;
    else
        goto L_13e5;

L_13e5:
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    goto L_1883;

L_1401:
    if ((0x0 != 0x0))
        goto L_1429;
    else
        goto L_1421;

L_1421:
    if (((HIWORD(rgids[i]) & 0x1000) == 0x0))
        goto L_1432;
    else
        goto L_1429;

L_1429:
    psz = 0xc09;
    goto L_152e;

L_1432:
    if ((0x0 != 0x0))
        goto L_145a;
    else
        goto L_1452;

L_1452:
    if (((HIWORD(rgids[i]) & 0x4000) == 0x0))
        goto L_147b;
    else
        goto L_145a;

L_145a:
    psz = PszGetCompressedString(LOWORD(rgids[i]));
    goto L_152e;

L_147b:
    if ((0x0 != 0x0))
        goto L_14a3;
    else
        goto L_149b;

L_149b:
    if (((HIWORD(rgids[i]) & 0x2000) == 0x0))
        goto L_14c4;
    else
        goto L_14a3;

L_14a3:
    psz = PszGetThingName(LOWORD(rgids[i]));
    goto L_152e;

L_14c4:
    if ((0x0 != 0x0))
        goto L_14ec;
    else
        goto L_14e4;

L_14e4:
    if (((HIWORD(rgids[i]) & 0x8000) == 0x0))
        goto L_1510;
    else
        goto L_14ec;

L_14ec:
    psz = PszGetFleetName((LOWORD(rgids[i]) | 0x8000));
    goto L_152e;

L_1510:
    psz = PszGetPlanetName(LOWORD(rgids[i]));

L_152e:
    pszT = szTemp;

L_1536:
    if (((uint16_t)(*(psz)) == 0))
        goto L_1573;
    else
        goto L_1545;

L_1545:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_1536;
    else
        goto L_1564;

L_1564:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_1570:
    goto L_1536;

L_1573:
    *(pszT) = 0;
    if ((i != iChecked))
        goto L_158f;
    else
        goto L_1589;

L_1589:
    t_merge_1592_0001 = 0x8;
    goto L_1592;

L_158f:
    t_merge_1592_0001 = 0x0;

L_1592:
    AppendMenu(hmenuPopup, (t_merge_1592_0001 | 0x0), (i + 15000), szTemp);

L_15aa:
    goto L_1883;

L_15ad:
    if ((rgsz[i] != 0x0))
        goto L_1771;
    else
        goto L_15bf;

L_15bf:
    pszTitle = rgsz[(i + 1)];
    if ((rgids == 0x0))
        goto L_15f1;
    else
        goto L_15da;

L_15da:
    t_merge_15f4_0001 = LOWORD(rgids[(i + 1)]);
    goto L_15f4;

L_15f1:
    t_merge_15f4_0001 = 0x0;

L_15f4:
    fChecked = t_merge_15f4_0001;
    hmenuSub = CreatePopupMenu();
    i = (i + 2);
    goto L_1738;

L_1608:
    if ((rgsz[i] == 0x0))
        goto L_1743;
    else
        goto L_161d;

L_161d:
    if ((rgids != 0x0))
        goto L_1649;
    else
        goto L_1626;

L_1626:
    if ((i != iChecked))
        goto L_1637;
    else
        goto L_1631;

L_1631:
    t_merge_163a_0001 = 0x1;
    goto L_163a;

L_1637:
    t_merge_163a_0001 = 0x0;

L_163a:
    fCheckedCur = t_merge_163a_0001;
    fChecked = (fChecked | fCheckedCur);
    goto L_165e;

L_1649:
    fCheckedCur = LOWORD(rgids[i]);

L_165e:
    if (((uint16_t)(*(rgsz[i])) != -1))
        goto L_16a9;
    else
        goto L_1675;

L_1675:
    if (((uint16_t)(rgsz[i][1]) != 0))
        goto L_16a9;
    else
        goto L_168d;

L_168d:
    AppendMenu(hmenuSub, 0x800, 0x0, 0x0);
    goto L_1734;

L_16a9:
    pszT = szTemp;
    psz = rgsz[i];

L_16c1:
    if (((uint16_t)(*(psz)) == 0))
        goto L_16fe;
    else
        goto L_16d0;

L_16d0:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_16c1;
    else
        goto L_16ef;

L_16ef:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_16fb:
    goto L_16c1;

L_16fe:
    *(pszT) = 0;
    if ((fCheckedCur == 0))
        goto L_1719;
    else
        goto L_1713;

L_1713:
    t_merge_171c_0001 = 0x8;
    goto L_171c;

L_1719:
    t_merge_171c_0001 = 0x0;

L_171c:
    AppendMenu(hmenuSub, (t_merge_171c_0001 | 0x0), (i + 15000), szTemp);

L_1734:
    i = (i + 1);

L_1738:
    if ((i < cString))
        goto L_1608;
    else
        goto L_1743;

L_1743:
    if ((fChecked == 0))
        goto L_1757;
    else
        goto L_1751;

L_1751:
    t_merge_175a_0001 = 0x8;
    goto L_175a;

L_1757:
    t_merge_175a_0001 = 0x0;

L_175a:
    AppendMenu(hmenuPopup, (t_merge_175a_0001 | 0x10), hmenuSub, pszTitle);
    goto L_1883;

L_1771:
    if (((uint16_t)(*(rgsz[i])) != -1))
        goto L_17bc;
    else
        goto L_1788;

L_1788:
    if (((uint16_t)(rgsz[i][1]) != 0))
        goto L_17bc;
    else
        goto L_17a0;

L_17a0:
    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
    goto L_1883;

L_17bc:
    pszT = szTemp;
    psz = rgsz[i];

L_17d4:
    if (((uint16_t)(*(psz)) == 0))
        goto L_1811;
    else
        goto L_17e3;

L_17e3:
    psz = (psz + 1);
    pszT = (pszT + 1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_17d4;
    else
        goto L_1802;

L_1802:
    pszT = (pszT + 1);
    *(pszT) = 38;

L_180e:
    goto L_17d4;

L_1811:
    *(pszT) = 0;
    if ((iChecked != -2))
        goto L_1839;
    else
        goto L_1825;

L_1825:
    t_merge_184f_0001_wide = rgids[i];
    goto L_184f;

L_1839:
    if ((i != iChecked))
        goto L_184b;
    else
        goto L_1844;

L_1844:
    t_merge_184f_0001_wide = 1;
    goto L_184f;

L_184b:
    t_merge_184f_0001_wide = 0;

L_184f:
    if (((LOWORD(t_merge_184f_0001_wide) | 0x0) != 0x0))
        goto L_1865;
    else
        goto L_185d;

L_185d:
    if (((HIWORD(t_merge_184f_0001_wide) | 0x0) == 0x0))
        goto L_186b;
    else
        goto L_1865;

L_1865:
    t_merge_186e_0001 = 0x8;
    goto L_186e;

L_186b:
    t_merge_186e_0001 = 0x0;

L_186e:
    AppendMenu(hmenuPopup, t_merge_186e_0001, (i + 15000), szTemp);

L_1883:
    i = (i + 1);

L_1887:
    if ((i < cString))
        goto L_13ad;
    else
        goto L_1892;

L_1892:
    if ((fRightBtn == 0))
        goto L_18a3;
    else
        goto L_189b;

L_189b:
    tpm = 2;
    goto L_18a8;

L_18a3:
    tpm = 0;

L_18a8:
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, 0x0);
    DestroyMenu(hmenuPopup);
    if ((hmenuSub == 0x0))
        goto L_18e7;
    else
        goto L_18de;

L_18de:
    DestroyMenu(hmenuSub);

L_18e7:
    if ((PeekMessage(&(msg), hwndFrame, 0x111, 0x111, 0x2) == 0))
        goto L_192c;
    else
        goto L_190c;

L_190c:
    if ((msg.wParam < 0x3a98))
        goto L_192c;
    else
        goto L_1917;

L_1917:
    if ((msg.wParam >= 0x3afc))
        goto L_192c;
    else
        goto L_1922;

L_1922:
    iPopMenuSel = (msg.wParam - 15000);

L_192c:

L_1932:
    return iPopMenuSel;
}

POINT PtDisplayPlanetStateInfo(HDC hdc, int16_t fPrint) {
    POINT    pt;
    int16_t  y;
    int16_t  xMax;
    int16_t  cch;
    int16_t  x;
    int16_t  iNewVal;
    PLANET  *lppl;
    int16_t  pctDesireOld;
    int16_t  pctDesire;
    int16_t  iValSav;
    StringId ids;
    int16_t  dChg;
    char     szOut[90];
    int16_t  t_merge_1ef1_0001;
    int16_t  t_merge_1f18_0001;

L_1938:
    y = 4;
    xMax = 4;
    x = 4;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsCurrently), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    if ((GlobalPD.iPlanVal < 0))
        goto L_19f3;
    else
        goto L_19c6;

L_19c6:
    DxStreamTextOut(hdc, &(x), y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanVal), 0, fPrint);
    goto L_1a19;

L_19f3:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnknown2), 0, fPrint);

L_1a19:
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);
    if ((x <= xMax))
        goto L_1a53;
    else
        goto L_1a4d;

L_1a4d:
    xMax = x;

L_1a53:
    x = 4;
    y = (y + dyArial8);
    if ((GlobalPD.iPlrMin != -1))
        goto L_1b13;
    else
        goto L_1a68;

L_1a68:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsImmune), 0, fPrint);
    if ((x <= xMax))
        goto L_1a9f;
    else
        goto L_1a99;

L_1a99:
    xMax = x;

L_1a9f:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsEffects), 0, fPrint);
    DxStreamTextOut(hdc, &(x), y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);
    goto L_1c65;

L_1b13:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsPreferPlanetsWhere), 0, fPrint);
    if ((x <= xMax))
        goto L_1b4a;
    else
        goto L_1b44;

L_1b44:
    xMax = x;

L_1b4a:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsBetween), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlrMin), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsAnd), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlrMax), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);

L_1c65:
    if ((x <= xMax))
        goto L_1c76;
    else
        goto L_1c70;

L_1c70:
    xMax = x;

L_1c76:
    x = 4;
    y = (y + dyArial8);
    if ((GlobalPD.iPlanMin <= -1))
        goto L_1e9a;
    else
        goto L_1c8b;

L_1c8b:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsCurrentlyPossessTechnology), 0, fPrint);
    if ((x <= xMax))
        goto L_1cc2;
    else
        goto L_1cbc;

L_1cbc:
    xMax = x;

L_1cc2:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsModify), 0, fPrint);
    DxStreamTextOut(hdc, &(x), y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsOn2), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    if ((x <= xMax))
        goto L_1d8b;
    else
        goto L_1d85;

L_1d85:
    xMax = x;

L_1d8b:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWithinRange), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanMin), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsTo), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanMax), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);
    if ((x <= xMax))
        goto L_1e94;
    else
        goto L_1e8e;

L_1e8e:
    xMax = x;

L_1e94:
    y = (y + dyArial8);

L_1e9a:
    x = 4;
    if ((GlobalPD.iPlanMin <= -1))
        goto L_2172;
    else
        goto L_1ea9;

L_1ea9:
    if ((GlobalPD.iPlrMin == -1))
        goto L_2172;
    else
        goto L_1eb3;

L_1eb3:
    if ((GlobalPD.iPlanVal == GlobalPD.iPlrVal))
        goto L_2172;
    else
        goto L_1ebf;

L_1ebf:
    iNewVal = -1;
    if ((GlobalPD.iPlanVal >= GlobalPD.iPlrVal))
        goto L_1ef7;
    else
        goto L_1ed0;

L_1ed0:
    if ((GlobalPD.iPlanMax <= GlobalPD.iPlanVal))
        goto L_1f1b;
    else
        goto L_1edc;

L_1edc:
    if ((GlobalPD.iPlrVal >= GlobalPD.iPlanMax))
        goto L_1eee;
    else
        goto L_1ee8;

L_1ee8:
    t_merge_1ef1_0001 = GlobalPD.iPlrVal;
    goto L_1ef1;

L_1eee:
    t_merge_1ef1_0001 = GlobalPD.iPlanMax;

L_1ef1:
    iNewVal = t_merge_1ef1_0001;

L_1ef4:
    goto L_1f1b;

L_1ef7:
    if ((GlobalPD.iPlanMin >= GlobalPD.iPlanVal))
        goto L_1f1b;
    else
        goto L_1f03;

L_1f03:
    if ((GlobalPD.iPlrVal <= GlobalPD.iPlanMin))
        goto L_1f15;
    else
        goto L_1f0f;

L_1f0f:
    t_merge_1f18_0001 = GlobalPD.iPlrVal;
    goto L_1f18;

L_1f15:
    t_merge_1f18_0001 = GlobalPD.iPlanMin;

L_1f18:
    iNewVal = t_merge_1f18_0001;

L_1f1b:
    if ((iNewVal == -1))
        goto L_2172;
    else
        goto L_1f24;

L_1f24:
    lppl = LpplFromId(GlobalPD.idPlanet);
    pctDesireOld = PctPlanetDesirability(lppl, idPlayer);
    iValSav = (uint16_t)(lppl->rgEnvVar[GlobalPD.iPlanetVar]);
    lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iNewVal);
    pctDesire = PctPlanetDesirability(lppl, idPlayer);
    lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iValSav);
    if ((pctDesireOld >= pctDesire))
        goto L_2172;
    else
        goto L_1fb6;

L_1fb6:
    cch = CchGetString(idsIfTerraform, szWork);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 4, (xMax - 4), 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    WrapTextOut(hdc, &(x), &(y), rgszPlanetAttr[GlobalPD.iPlanetVar], 0, 4, (xMax - 4), 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &(x), &(y), PszGetCompressedString(idsTo), 0, 4, (xMax - 4), 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    WrapTextOut(hdc, &(x), &(y), PszCalcEnvVar(GlobalPD.iPlanetVar, iNewVal), 0, 4, (xMax - 4), 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    cch = CchGetString(idsPlanetsValueWouldImprove, szWork);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 4, (xMax - 4), 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    cch = _wsprintf(szWork, "%d%%.  ", pctDesire);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 4, (xMax - 4), 0x0, 0, fPrint);

L_2172:
    if ((GlobalPD.iPlrMin == -1))
        goto L_225e;
    else
        goto L_217c;

L_217c:
    if ((GlobalPD.iPlanVal == GlobalPD.iPlrVal))
        goto L_225e;
    else
        goto L_2188;

L_2188:
    if ((GlobalPD.iPlanVal == -1))
        goto L_225e;
    else
        goto L_2192;

L_2192:
    if ((GlobalPD.iPlanVal >= GlobalPD.iPlrMin))
        goto L_21ab;
    else
        goto L_219e;

L_219e:
    dChg = (GlobalPD.iPlrMin - GlobalPD.iPlanVal);
    goto L_21d9;

L_21ab:
    if ((GlobalPD.iPlanVal <= GlobalPD.iPlrMax))
        goto L_21c4;
    else
        goto L_21b7;

L_21b7:
    dChg = (GlobalPD.iPlanVal - GlobalPD.iPlrMax);
    goto L_21d9;

L_21c4:
    dChg = (-abs((GlobalPD.iPlanVal - GlobalPD.iPlrVal)));

L_21d9:
    if ((dChg >= 0))
        goto L_21f2;
    else
        goto L_21e2;

L_21e2:
    dChg = (-dChg);
    ids = idsValueDAwayIdealValueRace;
    goto L_21f7;

L_21f2:
    ids = idsValueDOutsideHabitableRangeRace;

L_21f7:
    CchGetString(ids, szWork);
    cch = _wsprintf(szOut, szWork, dChg);
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &(x), &(y), szOut, cch, 4, (xMax - 4), 0x0, 0, fPrint);

L_225e:
    if ((x <= 4))
        goto L_226d;
    else
        goto L_2267;

L_2267:
    y = (y + dyArial8);

L_226d:
    pt.x = (xMax + 4);
    pt.y = (y + 4);

L_2288:
    return pt;
}

POINT PtDisplayPlanetPopInfo(HDC hdc, int16_t fPrint) {
    PLANET  pl;
    char    szT[150];
    POINT   pt;
    int16_t y;
    int16_t xMax;
    int16_t c;
    char   *psz;
    int32_t lMax;
    int16_t pctDesire;
    int16_t x;
    int32_t lPopChg;

L_228e:
    y = 4;
    xMax = 4;
    x = 4;
    FLookupPlanet(GlobalPD.idPlanet, &(pl));
    SelectObject(hdc, rghfontArial8[0]);
    if ((pl.iPlayer != idPlayer))
        goto L_23eb;
    else
        goto L_22cf;

L_22cf:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsPopulation3), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsIs), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    _wsprintf(szWork, PCTLD00, LOWORD(pl.rgwtMin[3]), HIWORD(pl.rgwtMin[3]));
    if ((LOWORD(pl.rgwtMin[3]) != 0x0))
        goto L_239e;
    else
        goto L_2390;

L_2390:
    if ((HIWORD(pl.rgwtMin[3]) != 0x0))
        goto L_239e;
    else
        goto L_2399;

L_2399:
    szWork[1] = 0;

L_239e:
    DxStreamTextOut(hdc, &(x), y, szWork, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);
    goto L_25b2;

L_23eb:
    if ((pl.iPlayer == -1))
        goto L_254a;
    else
        goto L_23f4;

L_23f4:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsEnemyPopulation), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    if ((pl.det < 0x3))
        goto L_2500;
    else
        goto L_246a;

L_246a:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsApproximately), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    _wsprintf(szWork, "%d00", (pl.uPopGuess * 0x4));
    if ((pl.uPopGuess != 0x0))
        goto L_24d2;
    else
        goto L_24cd;

L_24cd:
    szWork[1] = 0;

L_24d2:
    DxStreamTextOut(hdc, &(x), y, szWork, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    goto L_2528;

L_2500:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnknown), 0, fPrint);

L_2528:
    DxStreamTextOut(hdc, &(x), y, ".", 1, fPrint);
    goto L_25b2;

L_254a:
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUninhabited2), 0, fPrint);

L_25b2:
    if ((x <= xMax))
        goto L_25c7;
    else
        goto L_25bf;

L_25bf:
    xMax = x;

L_25c7:
    x = 4;
    y = (y + dyArial8);
    if ((pl.det < 0x3))
        goto AfterDesire;
    else
        goto L_25e5;

L_25e5:
    lMax = CalcPlanetMaxPop(GlobalPD.idPlanet, idPlayer);
    pctDesire = PctPlanetDesirability(&(pl), idPlayer);
    if ((pctDesire >= 0))
        goto L_279c;
    else
        goto L_261e;

L_261e:
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWillKillOffApproximately), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    c = _wsprintf(szWork, PCTDXPCTDPCTPCT, ((uint32_t)((-pctDesire)) / 0xa), ((uint32_t)((-pctDesire)) % 0xa));
    DxStreamTextOut(hdc, &(x), y, szWork, c, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsOf), 0, fPrint);
    if ((x <= xMax))
        goto L_272e;
    else
        goto L_2726;

L_2726:
    xMax = x;

L_272e:
    x = 4;
    y = (y + dyArial8);
    if ((pl.iPlayer != idPlayer))
        goto L_2771;
    else
        goto L_2746;

L_2746:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsEachTurn), 0, fPrint);
    goto L_2a58;

L_2771:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsSettleEveryTurn), 0, fPrint);

L_2799:
    goto L_2a58;

L_279c:
    if ((pl.iPlayer != idPlayer))
        goto L_28ce;
    else
        goto L_27a7;

L_27a7:
    if ((HIWORD(lMax) < 0x0))
        goto L_28ce;
    else
        goto L_27b1;

L_27b1:
    if ((HIWORD(lMax) > 0x0))
        goto L_27c0;
    else
        goto L_27b6;

L_27b6:
    if ((LOWORD(lMax) <= 0x0))
        goto L_28ce;
    else
        goto L_27c0;

L_27c0:
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWillSupportPopulation), 0, fPrint);
    if ((x <= xMax))
        goto L_283d;
    else
        goto L_2835;

L_2835:
    xMax = x;

L_283d:
    x = 4;
    y = (y + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    c = _wsprintf(szWork, PCTLD00, LOWORD(lMax), HIWORD(lMax));
    DxStreamTextOut(hdc, &(x), y, szWork, c, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonists3), 0, fPrint);
    goto L_2a58;

L_28ce:
    if ((HIWORD(lMax) < 0x0))
        goto L_2a51;
    else
        goto L_28d8;

L_28d8:
    if ((HIWORD(lMax) > 0x0))
        goto L_28e7;
    else
        goto L_28dd;

L_28dd:
    if ((LOWORD(lMax) <= 0x0))
        goto L_2a51;
    else
        goto L_28e7;

L_28e7:
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsIfColonize), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWould), 0, fPrint);
    if ((x <= xMax))
        goto L_2998;
    else
        goto L_2990;

L_2990:
    xMax = x;

L_2998:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsSupport), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    c = _wsprintf(szWork, PCTLD00, LOWORD(lMax), HIWORD(lMax));
    DxStreamTextOut(hdc, &(x), y, szWork, c, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonists3), 0, fPrint);
    goto L_2a58;

L_2a51:
    y = (y - dyArial8);

L_2a58:
    if ((x <= xMax))
        goto L_2a6d;
    else
        goto L_2a65;

L_2a65:
    xMax = x;

L_2a6d:
    x = 4;
    y = (y + dyArial8);
    if ((pl.iPlayer != idPlayer))
        goto L_2c24;
    else
        goto L_2a85;

L_2a85:
    if ((pctDesire < 0))
        goto L_2c24;
    else
        goto L_2a8f;

L_2a8f:
    if ((HIWORD(pl.rgwtMin[3]) > HIWORD(lMax)))
        goto L_2c24;
    else
        goto L_2a9f;

L_2a9f:
    if ((HIWORD(pl.rgwtMin[3]) < HIWORD(lMax)))
        goto L_2aac;
    else
        goto L_2aa4;

L_2aa4:
    if ((LOWORD(pl.rgwtMin[3]) >= LOWORD(lMax)))
        goto L_2c24;
    else
        goto L_2aac;

L_2aac:
    c = CchGetString(idsPopulation, szT);
    WrapTextOut(hdc, &(x), &(y), szT, c, 4, xMax, 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszGetPlanetName(pl.id);
    WrapTextOut(hdc, &(x), &(y), psz, 0, 4, xMax, 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    lPopChg = ChgPopFromPlanet(&(pl), 0);
    if ((pctDesire == 0))
        goto L_2b87;
    else
        goto L_2b6e;

L_2b6e:
    if ((HIWORD(lPopChg) > 0x0))
        goto L_2b9f;
    else
        goto L_2b78;

L_2b78:
    if ((HIWORD(lPopChg) < 0x0))
        goto L_2b87;
    else
        goto L_2b7d;

L_2b7d:
    if ((LOWORD(lPopChg) > 0x0))
        goto L_2b9f;
    else
        goto L_2b87;

L_2b87:
    c = CchGetString(idsWillGrowYear, szT);
    goto L_2be3;

L_2b9f:
    psz = PszGetCompressedString(idsWillGrowLd00Ld00Year);
    c = _wsprintf(szT, psz, LOWORD(lPopChg), HIWORD(lPopChg), (LOWORD(pl.rgwtMin[3]) + LOWORD(lPopChg)), (HIWORD(pl.rgwtMin[3]) + HIWORD(lPopChg)));

L_2be3:
    WrapTextOut(hdc, &(x), &(y), szT, c, 4, xMax, 0x0, 0, fPrint);
    x = 4;
    y = (y + dyArial8);
    goto AfterDesire;

L_2c24:
    if ((pl.iPlayer == idPlayer))
        goto AfterDesire;
    else
        goto L_2c2f;

L_2c2f:
    if ((pl.iPlayer == -1))
        goto AfterDesire;
    else
        goto L_2c38;

L_2c38:
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszGetPlanetName(pl.id);
    WrapTextOut(hdc, &(x), &(y), psz, 0, 4, xMax, 0x0, 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    if ((pl.uDefGuess != 0x0))
        goto L_2cb9;
    else
        goto L_2ca2;

L_2ca2:
    c = CchGetString(idsAppearsHavePlanetaryDefenses, szWork);
    goto L_2d01;

L_2cb9:
    psz = PszGetCompressedString(idsHasPlanetaryDefensesApproximatelyDCoverage);
    c = _wsprintf(szT, psz, (LOWORD((pl.uDefGuess * 0x6)) + 0x3));
    psz = szT;

L_2d01:
    WrapTextOut(hdc, &(x), &(y), psz, c, 4, xMax, 0x0, 0, fPrint);
    x = 4;
    y = (y + dyArial8);

AfterDesire:
    pt.x = (xMax + 4);
    pt.y = (y + 4);

L_2d5f:
    return pt;
}

POINT PtDisplayZipOrdInfo(HDC hdc, int16_t xCtr, int16_t fPrint) {
    POINT   pt;
    int16_t y;
    int16_t xMax;
    char   *psz;
    int16_t x;

L_2d66:
    y = 4;
    xMax = 4;
    x = 4;
    SelectObject(hdc, rghfontArial8[1]);
    if ((fPrint == 0))
        goto L_2dc1;
    else
        goto L_2d92;

L_2d92:
    psz = PszGetCompressedString(idsZipord);
    CtrTextOut(hdc, xCtr, y, psz, strlen(psz));

L_2dc1:
    y = (y + dyArial8);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsZipordProvidesAbilityQuicklySetFleets), 0, fPrint);
    if ((x <= xMax))
        goto L_2e0a;
    else
        goto L_2e04;

L_2e04:
    xMax = x;

L_2e0a:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsTransportOrdersOne3CommonSetsSelect), 0, fPrint);
    if ((x <= xMax))
        goto L_2e4c;
    else
        goto L_2e46;

L_2e46:
    xMax = x;

L_2e4c:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsZipordClickDiamondRightMouse), 0, fPrint);
    if ((x <= xMax))
        goto L_2e8e;
    else
        goto L_2e88;

L_2e88:
    xMax = x;

L_2e8e:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsButtonClickOrderChoice), 0, fPrint);
    if ((x <= xMax))
        goto L_2ed0;
    else
        goto L_2eca;

L_2eca:
    xMax = x;

L_2ed0:
    x = 4;
    y = (y + (dyArial8 * 2));
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsQuikload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsLoadMineralsAvailable), 0, fPrint);
    if ((x <= xMax))
        goto L_2f52;
    else
        goto L_2f4c;

L_2f4c:
    xMax = x;

L_2f52:
    x = 4;
    y = (y + ((dyArial8 >> 0x1) + dyArial8));
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsQuikdrop), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnloadEverythingFleetCarrying), 0, fPrint);
    if ((x <= xMax))
        goto L_2fd8;
    else
        goto L_2fd2;

L_2fd2:
    xMax = x;

L_2fd8:
    x = 4;
    y = (y + ((dyArial8 >> 0x1) + dyArial8));
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWaitload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWaitFullLoadMinerals), 0, fPrint);
    if ((x <= xMax))
        goto L_305e;
    else
        goto L_3058;

L_3058:
    xMax = x;

L_305e:
    x = 4;
    y = (y + ((dyArial8 >> 0x1) + dyArial8));
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsClear2), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsRemoveTransportOrders), 0, fPrint);
    if ((x <= xMax))
        goto L_30e4;
    else
        goto L_30de;

L_30de:
    xMax = x;

L_30e4:
    x = 4;
    y = (y + dyArial8);
    pt.x = (xMax + 4);
    pt.y = (y + 4);

L_310a:
    return pt;
}

POINT PtDisplayFactoryMineInfo(HDC hdc, int16_t dx, int16_t fPrint) {
    char    *pszTypes;
    char     szT[40];
    POINT    pt;
    StringId ids;
    char    *pszType;
    int16_t  y;
    int16_t  xMax;
    int16_t  i;
    int16_t  c;
    char    *psz;
    int16_t  cnt;
    int16_t  x;
    StringId t_merge_3201_0001;
    char    *t_merge_3293_0001;

L_3110:
    ids = idsHave;
    dx = (dx - 8);
    xMax = 4;
    x = 4;
    y = 2;
    if ((GlobalPD.grbit == 0x0))
        goto L_314e;
    else
        goto L_3141;

L_3141:
    pszType = 0xc2d;
    pszTypes = 0xc35;
    goto L_3158;

L_314e:
    pszType = 0xc3f;
    pszTypes = 0xc44;

L_3158:
    SelectObject(hdc, rghfontArial8[1]);
    if ((fPrint == 0))
        goto L_31b8;
    else
        goto L_316d;

L_316d:
    CchGetString(idsSInfo, szT);
    c = _wsprintf(szWork, szT, pszType);
    CtrTextOut(hdc, (dx >> 0x1), y, szWork, c);

L_31b8:
    y = (y + (dyArial8 + 4));
    if ((GetRaceStat(&(rgplr[idPlayer]), rsMajorAdv) != raMacintosh))
        goto L_323c;
    else
        goto L_31e2;

L_31e2:
    SelectObject(hdc, rghfontArial8[0]);
    if ((GlobalPD.grbit == 0x0))
        goto L_31fe;
    else
        goto L_31f8;

L_31f8:
    t_merge_3201_0001 = idsRaceIncapableBuildingFactories;
    goto L_3201;

L_31fe:
    t_merge_3201_0001 = idsRaceIncapableBuildingMinesHoweverColonistsHave;

L_3201:
    psz = PszGetCompressedString(t_merge_3201_0001);
    WrapTextOut(hdc, &(x), &(y), psz, 0, 4, dx, &(xMax), 0, fPrint);
    goto LDone;

L_323c:
    i = 0;
    goto L_3349;

L_3244:
    SelectObject(hdc, rghfontArial8[1]);
    goto L_32f6;

L_3256:
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(ids);
    ids = (ids + 1);
    goto L_3319;

L_3277:
    cnt = GlobalPD.cCur;

SetQuan:
    if ((cnt != 1))
        goto L_328e;
    else
        goto L_3286;

L_3286:
    t_merge_3293_0001 = pszType;
    goto L_3293;

L_328e:
    t_merge_3293_0001 = pszTypes;

L_3293:
    _wsprintf(szWork, "%d %s", cnt, &(*(t_merge_3293_0001)));
    psz = szWork;
    goto L_3319;

L_32b6:
    psz = PszGetPlanetName(GlobalPD.idPlan);
    goto L_3319;

L_32c8:
    cnt = GlobalPD.cMax;
    goto SetQuan;

L_32d1:
    _wsprintf(szWork, PCTD, GlobalPD.cOperate);
    psz = szWork;
    goto L_3319;

L_32f6:
    if ((i > 8))
        goto L_3319;
    else
        goto L_32fe;

L_32fe:
    switch ((i * 2)) {
    case 0x0:
        goto L_3256;
    case 0x2:
        goto L_3277;
    case 0x4:
        goto L_3256;
    case 0x6:
        goto L_32b6;
    case 0x8:
        goto L_3256;
    case 0xa:
        goto L_32c8;
    case 0xc:
        goto L_3256;
    case 0xe:
        goto L_32d1;
    case 0x10:
        goto L_3256;
    }

L_3319:
    WrapTextOut(hdc, &(x), &(y), psz, 0, 4, dx, &(xMax), 0, fPrint);
    i = (i + 1);

L_3349:
    if ((i <= 8))
        goto L_3244;
    else
        goto LDone;

LDone:
    pt.x = (xMax + 4);
    pt.y = ((y + dyArial8) + 2);

L_3371:
    return pt;
}

POINT PtDisplayResourceInfo(HDC hdc, int16_t dx, int16_t fPrint) {
    int16_t  iMax;
    POINT    pt;
    StringId ids;
    int16_t  y;
    int16_t  xMax;
    int16_t  i;
    char    *psz;
    int16_t  cnt;
    int16_t  x;
    uint16_t t_merge_33ab_0001;

L_3378:
    if ((GetRaceStat(&(rgplr[idPlayer]), rsMajorAdv) != raMacintosh))
        goto L_33a8;
    else
        goto L_33a2;

L_33a2:
    t_merge_33ab_0001 = 0x1;
    goto L_33ab;

L_33a8:
    t_merge_33ab_0001 = 0x0;

L_33ab:
    iMax = (t_merge_33ab_0001 + 8);
    ids = idsGenerates;
    dx = (dx - 8);
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[1]);
    if ((fPrint == 0))
        goto L_3416;
    else
        goto L_33e4;

L_33e4:
    psz = PszGetCompressedString(idsResourceInfo);
    CtrTextOut(hdc, (dx >> 0x1), y, psz, strlen(psz));

L_3416:
    y = (y + (dyArial8 + 4));
    i = 0;
    goto L_352b;

L_3427:
    SelectObject(hdc, rghfontArial8[1]);
    goto L_34d6;

L_3439:
    if ((GlobalPD.iPlanVal == GlobalPD.iPlanetVar))
        goto OutOfFor;
    else
        goto L_3448;

L_3448:
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(ids);
    ids = (ids + 1);
    goto L_34fb;

L_3469:
    cnt = GlobalPD.iPlanetVar;

SetQuan:
    _wsprintf(szWork, PCTD, cnt);
    psz = szWork;
    goto L_34fb;

L_3490:
    psz = PszGetPlanetName(GlobalPD.idPlanet);
    goto L_34fb;

L_34a2:
    cnt = (GlobalPD.iPlanetVar - GlobalPD.iPlanVal);
    if ((cnt > 0))
        goto SetQuan;
    else
        goto L_34b8;

L_34b8:
    psz = PszGetCompressedString(idsNone2);
    goto L_34fb;

L_34ca:
    cnt = GlobalPD.iPlanVal;
    goto SetQuan;

L_34d6:
    if ((i > 9))
        goto L_34fb;
    else
        goto L_34de;

L_34de:
    switch ((i * 2)) {
    case 0x0:
        goto L_3490;
    case 0x2:
        goto L_3448;
    case 0x4:
        goto L_3469;
    case 0x6:
        goto L_3448;
    case 0x8:
        goto L_34a2;
    case 0xa:
        goto L_3448;
    case 0xc:
        goto L_3439;
    case 0xe:
        goto L_34ca;
    case 0x10:
        goto L_3448;
    case 0x12:
        goto L_3448;
    }

L_34fb:
    WrapTextOut(hdc, &(x), &(y), psz, 0, 4, dx, &(xMax), 0, fPrint);
    i = (i + 1);

L_352b:
    if ((i <= iMax))
        goto L_3427;
    else
        goto OutOfFor;

OutOfFor:
    pt.x = (xMax + 4);
    pt.y = ((y + dyArial8) + 2);

L_3555:
    return pt;
}

POINT PtDisplayString(HDC hdc, int16_t dx, int16_t fPrint) {
    POINT   pt;
    int16_t y;
    int16_t xMax;
    int16_t x;

L_355c:
    dx = (dx - 8);
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &(x), &(y), GlobalPD.psz, 0, 4, dx, &(xMax), 0, fPrint);
    pt.x = (xMax + 4);
    pt.y = ((y + dyArial8) + 2);

L_35d6:
    return pt;
}
