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
    return 0x10000;

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

L_0126:
    return 0x0;
}

int16_t FIsPopupHullType(int16_t ishdef) {
    uint16_t imd;

L_0148:
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_0166 : L_015b */

L_015b:
    /* untranslated: branch part[10:2](GlobalPD) != 0xff ? L_016c : L_0166 */

L_0166:
    return 0x1;

L_016c:
    /* untranslated: imd = ((*(LphuldefFromId(rglpshdef[HIWORD(part[2:4](GlobalPD))][ishdef].hul.ihuldef)+0x7b) >> 0xa) & 0xf) */
    /* untranslated: return (mpimdgrbitBU[imd] & part[10:2](GlobalPD)) */
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
    uint16_t t_merge_03dc_0001;
    uint16_t t_merge_0b79_0001;

L_01c0:
    crBack = SetBkColor(hdc, 0xffffff);
    crFore = SetTextColor(hdc, 0x0);
    bkMode = SetBkMode(hdc, OPAQUE);
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    GetClientRect(hwnd, &(rc));
    goto L_0c14;

L_0228:
    goto L_0c44;
    /* untranslated: call CtrTextOut(hdc, (rc.right >> 0x1), 0x4, rgszMinerals[words(HIWORD(GlobalPD), part[4:2](GlobalPD))], 0x0) -> callresult(void) */
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(idsMineralConcentration);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4);
    RightTextOut(hdc, dx, (dyArial8 + 4), PszGetCompressedString(idsSurface), 0x0, 0x0);
    /* untranslated: branch part[12:2](GlobalPD) < 0x0 ? L_02fc : L_02c9 */

L_02c9:
    /* untranslated: branch part[12:2](GlobalPD) > 0x0 ? L_02d8 : L_02ce */

L_02ce:
    /* untranslated: branch part[10:2](GlobalPD) < 0x0 ? L_02fc : L_02d8 */

L_02d8:
    /* untranslated: c = _wsprintf(szWork, "%ldkT", part[10:2](GlobalPD), part[12:2](GlobalPD)) */
    goto L_0324;

L_02fc:
    strcpy(szWork, PszGetCompressedString(idsUnknown2));
    c = strlen(szWork);

L_0324:
    TextOut(hdc, dx, (dyArial8 + 4), szWork, c);
    RightTextOut(hdc, dx, ((dyArial8 * 2) + 0x4), PszGetCompressedString(idsMineralConcentration), 0x0, 0x0);
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
    /* untranslated: branch part[14:2](GlobalPD) <= 0x0 ? L_0401 : L_0385 */

L_0385:
    /* untranslated: c = _wsprintf(szWork, PCTLD, part[14:2](GlobalPD), GlobalPD.iPlrMax) */
    /* untranslated: branch part[6:2](GlobalPD) != 0x0 ? L_03ba : L_03b0 */

L_03b0:
    /* untranslated: branch part[8:2](GlobalPD) == 0x0 ? L_0423 : L_03ba */

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
    /* untranslated: branch part[14:2](GlobalPD) < 0x1e ? L_03d9 : L_03d3 */

L_03d3:
    t_merge_03dc_0001 = 0x51e;
    goto L_03dc;

L_03d9:
    t_merge_03dc_0001 = 0x51f;

L_03dc:
    c = (c + _wsprintf(&(szWork[c]), PszGetCompressedString(t_merge_03dc_0001)));

L_0401:
    c = _wsprintf(szWork, PszGetCompressedString(idsUnknown2));

L_0423:
    TextOut(hdc, dx, ((dyArial8 * 2) + 0x4), szWork, c);
    if ((HIWORD(GlobalPD.rgi[0x4]) < 0x0))
        goto L_04d9;
    else
        goto L_044b;

L_044b:
    if ((HIWORD(GlobalPD.rgi[0x4]) > 0x0))
        goto L_045a;
    else
        goto L_0450;

L_0450:
    if ((LOWORD(GlobalPD.rgi[0x4]) < 0x0))
        goto L_04d9;
    else
        goto L_045a;

L_045a:
    RightTextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 0x4), PszGetCompressedString(idsMiningRate), 0x0, 0x0);
    CchGetString(idsLdktYr, szT);
    c = _wsprintf(szWork, szT, LOWORD(GlobalPD.rgi[0x4]), HIWORD(GlobalPD.rgi[0x4]));
    TextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 0x4), szWork, c);

L_04d9:
    goto L_0c44;
    CtrTextOut(hdc, (rc.right >> 0x1), 0x4, PszPlayerName(HIWORD(GlobalPD), 0x1, 0x1, 0x1, 0x0, 0x0), 0x0);
    c = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), (HIWORD(GlobalPD) + 0x1));
    CtrTextOut(hdc, (rc.right >> 0x1), (dyArial8 + 4), szWork, c);
    goto L_0c44;
    if (((rc.bottom - rc.top) >= (dyArial8 * 2)))
        goto L_05b0;
    else
        goto L_0574;

L_0574:
    c = CchGetString(idsNone2, szWork);
    TextOut(hdc, 0x4, 0x4, szWork, c);
    SelectObject(hdc, rghfontArial8[0]);
    goto L_0c44;

L_05b0:
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsShipName, szWork);
    TextOut(hdc, 0x4, 0x4, szWork, c);
    /* untranslated: call RightTextOut(hdc, ((rc.right - 4) - part[8:2](GlobalPD)), 0x4, "#", 0x1, 0x0) -> callresult(void) */
    /* untranslated: branch part[8:2](GlobalPD) == 0x0 ? L_0673 : L_0619 */

L_0619:
    c = CchGetString(idsDamage2, szWork);
    RightTextOut(hdc, (rc.right - 4), 0x4, szWork, c, 0x0);
    PatBlt(hdc, 0x4, (dyArial8 + 2), (rc.right - 8), 0x1, BLACKNESS);

L_0673:
    SelectObject(hdc, rghfontArial8[0]);
    yCur = (dyArial8 + 4);
    i = 0;
    goto L_08d1;

L_0690:
    /* untranslated: branch part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0xc) + (i * 2))] <= 0x0 ? L_08cd : L_06af */

L_06af:
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_06cc : L_06b9 */

L_06b9:
    if ((FIsPopupHullType(i) == 0))
        goto L_08cd;
    else
        goto L_06cc;

L_06cc:
    /* untranslated: branch part[6:2](GlobalPD) == 0x0 ? L_0718 : L_06d6 */

L_06d6:
    /* untranslated: branch part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 2))] == 0x0 ? L_0708 : L_06f5 */

L_06f5:
    SetTextColor(hdc, 0xff);
    goto L_0718;

L_0708:
    SetTextColor(hdc, 0x0);

L_0718:
    /* untranslated: call DecorateHullName(((LOWORD(part[2:4](GlobalPD)) >> 0x9) & 0xf), i, szTB) -> callresult(void) */
    lpsz = szTB;
    TextOut(hdc, 0x4, yCur, lpsz, fstrlen(lpsz));
    /* untranslated: c = _wsprintf(szWork, PCTD, part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0xc) + (i * 0x2))]) */
    /* untranslated: call RightTextOut(hdc, ((rc.right - 4) - part[8:2](GlobalPD)), yCur, szWork, c, 0x0) -> callresult(void) */
    /* untranslated: branch part[6:2](GlobalPD) == 0x0 ? L_08c7 : L_07c8 */

L_07c8:
    /* untranslated: branch part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 2))] == 0x0 ? L_08c7 : L_07e7 */

L_07e7:
    /* untranslated: csh = part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0xc) + (i * 0x2))] */
    /* untranslated: csh = loword((int32_t)(words(hiword((uint32_t)(words(0x0, (part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 0x2))] & 0x7f)) *
     * sext16to32(csh))), loword((uint32_t)(words(0x0, (part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 0x2))] & 0x7f)) * sext16to32(csh)))) / 0x64)) */
    if ((csh > 0))
        goto L_084f;
    else
        goto L_084a;

L_084a:
    csh = 1;

L_084f:
    /* untranslated: dpT = (words(((part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 0x2))] >> 0x7) & 0x1ff), 0x0) / 5) */
    if ((dpT != 0))
        goto L_0888;
    else
        goto L_0883;

L_0883:
    dpT = 1;

L_0888:
    c = _wsprintf(szWork, "%d@%d%%", csh, dpT);
    RightTextOut(hdc, (rc.right - 4), yCur, szWork, c, 0x0);

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
    /* untranslated: branch part[6:2](GlobalPD) == 0x0 ? L_08f4 : L_08e4 */

L_08e4:
    SetTextColor(hdc, 0x0);

L_08f4:
    goto L_0c44;
    psz = PszGetCompressedString(idsPlanet);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4);
    SelectObject(hdc, rghfontArial8[1]);
    RightTextOut(hdc, dx, 0x4, psz, strlen(psz), 0x0);
    RightTextOut(hdc, dx, (dyArial8 + 4), PszGetCompressedString(idsId), 0x0, 0x0);
    RightTextOut(hdc, dx, ((dyArial8 * 2) + 0x4), PszGetCompressedString(idsX), 0x0, 0x0);
    RightTextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 0x4), PszGetCompressedString(idsY), 0x0, 0x0);
    psz = PszGetPlanetName(sel.scan.idpl);
    SelectObject(hdc, rghfontArial8[0]);
    TextOut(hdc, dx, 0x4, psz, strlen(psz));
    c = _wsprintf(szWork, PCTD, (sel.scan.idpl + 1));
    TextOut(hdc, dx, (dyArial8 + 4), szWork, c);
    c = _wsprintf(szWork, PCTD, sel.scan.pt.x);
    TextOut(hdc, dx, ((dyArial8 * 2) + 0x4), szWork, c);
    c = _wsprintf(szWork, PCTD, sel.scan.pt.y);
    TextOut(hdc, dx, (LOWORD((3 * dyArial8)) + 0x4), szWork, c);
    goto L_0c44;
    PtDisplayPlanetStateInfo(hdc, 0x1);
    goto L_0c44;
    PtDisplayZipOrdInfo(hdc, (rc.right >> 0x1), 0x1);
    goto L_0c44;
    PtDisplayPlanetPopInfo(hdc, 0x1);
    goto L_0c44;
    PtDisplayFactoryMineInfo(hdc, rc.right, 0x1);
    goto L_0c44;
    PtDisplayResourceInfo(hdc, rc.right, 0x1);
    goto L_0c44;
    /* untranslated: call DisplayComponentInfo(hdc, rc.right, rc.bottom, &part[2:0](GlobalPD)) -> callresult(void) */
    goto L_0c44;
    PtDisplayString(hdc, rc.right, 0x1);
    goto L_0c44;
    /* untranslated: branch LOWORD(part[2:4](GlobalPD)) < 0x20 ? L_0b76 : L_0b70 */

L_0b70:
    t_merge_0b79_0001 = 0x1;
    goto L_0b79;

L_0b76:
    t_merge_0b79_0001 = 0x0;

L_0b79:
    fStarbaseMode = t_merge_0b79_0001;
    DrawSlotDlg(hwnd, hdc, &(rc), 0xffff);
    rc.top = (dyArial8 + 306);
    rc.left = (rc.left + 6);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkMode(hdc, TRANSPARENT);
    /* untranslated: call fstrcpy(szWork, &part[8:0](GlobalPD)) -> callresult(char *) */
    CtrTextOut(hdc, (((rc.right - 76) >> 0x1) + 0x4c), 0x6, szWork, 0x0);
    DrawBuildSelHull(hwnd, hdc, 0xffff, &(rc));
    goto L_0c44;

L_0c14:
    if (((GlobalPD.grPopup - 1) > 0xd))
        goto L_0228;
    else
        goto L_0c1f;

L_0c1f:
    goto L_ffffffff;

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
    uint16_t t_merge_113b_0001;
    uint16_t t_merge_1189_0001;
    int16_t  t_merge_126d_0001;
    uint16_t t_merge_12a3_0001;
    int16_t  t_merge_12cc_0001;
    uint16_t t_merge_1302_0001;

L_0c7c:
    pt.x = x;
    pt.y = y;
    ClientToScreen(hwnd, &(pt));
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    goto L_11f5;

L_0cc0:
    goto L_1225;
    psz = PszGetCompressedString(idsMineralConcentration0000000kt);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    dy = (LOWORD((0x3 * dyArial8)) + 8);
    if ((HIWORD(GlobalPD.rgi[0x4]) < 0x0))
        goto L_0d1f;
    else
        goto L_0d0a;

L_0d0a:
    if ((HIWORD(GlobalPD.rgi[0x4]) > 0x0))
        goto L_0d19;
    else
        goto L_0d0f;

L_0d0f:
    if ((LOWORD(GlobalPD.rgi[0x4]) < 0x0))
        goto L_0d1f;
    else
        goto L_0d19;

L_0d19:
    dy = (dy + dyArial8);

L_0d1f:
    goto L_1225;
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszPlayerName(HIWORD(GlobalPD), 0x1, 0x1, 0x1, 0x0, 0x0);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    dx2 = (LOWORD(GetTextExtent(hdc, "Player #16", 0xa)) + 8);
    if ((dx2 <= dx))
        goto L_0d9c;
    else
        goto L_0d96;

L_0d96:
    dx = dx2;

L_0d9c:
    dy = ((dyArial8 * 2) + 8);
    goto L_1225;
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
    /* untranslated: branch part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0xc) + (i * 2))] <= 0x0 ? L_0f77 : L_0e29 */

L_0e29:
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_0e46 : L_0e33 */

L_0e33:
    if ((FIsPopupHullType(i) == 0))
        goto L_0f77;
    else
        goto L_0e46;

L_0e46:
    dy = (dy + dyArial8);
    /* untranslated: call DecorateHullName(((LOWORD(part[2:4](GlobalPD)) >> 0x9) & 0xf), i, szTB) -> callresult(void) */
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
    /* untranslated: c = _wsprintf(szWork, PCTD, part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0xc) + (i * 0x2))]) */
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
    /* untranslated: branch part[6:2](GlobalPD) == 0x0 ? L_0f77 : L_0f15 */

L_0f15:
    /* untranslated: branch ((part[4:2](GlobalPD):[((HIWORD(GlobalPD) + 0x2c) + (i * 2))] >> 0x7) & 0x1ff) == 0x0 ? L_0f77 : L_0f3e */

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
    /* untranslated: part[8:2](GlobalPD) = dxDamage */
    dx = (((dxL + dxR) + 16) + dxDamage);
    goto L_1225;
    SelectObject(hdc, rghfontArial8[1]);
    dy = ((dyArial8 * 4) + 8);
    psz = PszGetCompressedString(idsPlanet);
    dx = (LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8);
    psz = PszGetPlanetName(sel.scan.idpl);
    SelectObject(hdc, rghfontArial8[0]);
    dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
    dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 0x4));
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
    PtDisplayPlanetStateInfo(hdc, 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */
    goto SetDxDy;
    PtDisplayZipOrdInfo(hdc, 0x0, 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */
    goto SetDxDy;
    PtDisplayPlanetPopInfo(hdc, 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */
    goto SetDxDy;
    PtDisplayResourceInfo(hdc, 0xc8, 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */
    goto SetDxDy;
    PtDisplayFactoryMineInfo(hdc, 0xc8, 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */

SetDxDy:
    dx = (ptT.x + 2);
    dy = (ptT.y + 2);
    goto L_1225;
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
    dy = (((dyArial10 + 72) + LOWORD((0xc * dyArial8))) + 6);
    goto L_1225;
    PtDisplayString(hdc, HIWORD(GlobalPD), 0x0);
    /* untranslated: ptT.x = loword(callresult(POINT)) */
    /* untranslated: ptT.y = hiword(callresult(POINT)) */
    goto SetDxDy;
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
    lpshdefBuild = &(GlobalPD);
    UpdateSlotGlobals();
    dx = 340;
    dy = (((dyArial8 + 306) + LOWORD((0x6 * dyArial8))) + 8);
    if ((gd.mdScreenSize <= 0x0))
        goto L_11e2;
    else
        goto L_11ce;

L_11ce:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_11e2;
    else
        goto L_11d8;

L_11d8:
    dy = (dy + LOWORD((0x3 * dyArial8)));

L_11e2:
    goto L_1225;
    dx = 120;
    dy = 80;
    goto L_1225;

L_11f5:
    if (((GlobalPD.grPopup - 1) > 0xd))
        goto L_0cc0;
    else
        goto L_1200;

L_1200:
    goto L_ffffffff;

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
    t_merge_126d_0001 = (GetSystemMetrics(SM_CXSCREEN) - dx);

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
    t_merge_12a3_0001 = (GetSystemMetrics(SM_CXSCREEN) - dx);

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
    t_merge_12cc_0001 = (GetSystemMetrics(SM_CYSCREEN) - dy);

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
    t_merge_1302_0001 = (GetSystemMetrics(SM_CYSCREEN) - dy);

L_1302:
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, 0x0, 0x90800000, pt.x, pt.y, dx, dy, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndPopup, WM_SETFONT, rghfontArial8[0], 0x0);
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
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_1536;
    else
        goto L_1564;

L_1564:
    pszT = (pszT + 0x1);
    *(pszT) = 38;

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
    t_merge_15f4_0001 = LOWORD(rgids[(i + 0x1)]);
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
        goto L_1617;

L_1617:

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
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_16c1;
    else
        goto L_16ef;

L_16ef:
    pszT = (pszT + 0x1);
    *(pszT) = 38;

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
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_17d4;
    else
        goto L_1802;

L_1802:
    pszT = (pszT + 0x1);
    *(pszT) = 38;

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
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0x0, hwndFrame, 0x0);
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
    uint16_t t_merge_1ef1_0001;
    uint16_t t_merge_1f18_0001;

L_1938:
    y = 4;
    xMax = 4;
    x = 4;
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[part[4:2](GlobalPD)], 0x0, fPrint) -> callresult(int16_t) */
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsCurrently), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: branch part[6:2](GlobalPD) < 0x0 ? L_19f3 : L_19c6 */

L_19c6:
    /* untranslated: call DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(part[4:2](GlobalPD), part[6:2](GlobalPD)), 0x0, fPrint) -> callresult(int16_t) */
    goto L_1a19;

L_19f3:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnknown2), 0x0, fPrint);

L_1a19:
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);
    if ((x <= xMax))
        goto L_1a53;
    else
        goto L_1a4d;

L_1a4d:
    xMax = x;

L_1a53:
    x = 4;
    y = (y + dyArial8);
    /* untranslated: branch part[14:2](GlobalPD) != 0xffff ? L_1b13 : L_1a68 */

L_1a68:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsImmune), 0x0, fPrint);
    if ((x <= xMax))
        goto L_1a9f;
    else
        goto L_1a99;

L_1a99:
    xMax = x;

L_1a9f:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsEffects), 0x0, fPrint);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[part[4:2](GlobalPD)], 0x0, fPrint) -> callresult(int16_t) */
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);
    goto L_1c65;

L_1b13:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsPreferPlanetsWhere), 0x0, fPrint);
    if ((x <= xMax))
        goto L_1b4a;
    else
        goto L_1b44;

L_1b44:
    xMax = x;

L_1b4a:
    x = 4;
    y = (y + dyArial8);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[part[4:2](GlobalPD)], 0x0, fPrint) -> callresult(int16_t) */
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsBetween), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(part[4:2](GlobalPD), part[14:2](GlobalPD)), 0x0, fPrint) -> callresult(int16_t) */
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsAnd), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(part[4:2](GlobalPD), GlobalPD.iPlrMax), 0x0, fPrint) -> callresult(int16_t) */
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);

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
    /* untranslated: branch part[8:2](GlobalPD) <= 0xffff ? L_1e9a : L_1c8b */

L_1c8b:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsCurrentlyPossessTechnology), 0x0, fPrint);
    if ((x <= xMax))
        goto L_1cc2;
    else
        goto L_1cbc;

L_1cbc:
    xMax = x;

L_1cc2:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsModify), 0x0, fPrint);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[part[4:2](GlobalPD)], 0x0, fPrint) -> callresult(int16_t) */
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsOn2), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWithinRange), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(part[4:2](GlobalPD), part[8:2](GlobalPD)), 0x0, fPrint) -> callresult(int16_t) */
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsTo), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(part[4:2](GlobalPD), part[10:2](GlobalPD)), 0x0, fPrint) -> callresult(int16_t) */
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);
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
    /* untranslated: branch part[8:2](GlobalPD) <= 0xffff ? L_2172 : L_1ea9 */

L_1ea9:
    /* untranslated: branch part[14:2](GlobalPD) == 0xffff ? L_2172 : L_1eb3 */

L_1eb3:
    /* untranslated: branch part[6:2](GlobalPD) == part[12:2](GlobalPD) ? L_2172 : L_1ebf */

L_1ebf:
    iNewVal = -1;
    /* untranslated: branch part[6:2](GlobalPD) >= part[12:2](GlobalPD) ? L_1ef7 : L_1ed0 */

L_1ed0:
    /* untranslated: branch part[10:2](GlobalPD) <= part[6:2](GlobalPD) ? L_1f1b : L_1edc */

L_1edc:
    /* untranslated: branch part[12:2](GlobalPD) >= part[10:2](GlobalPD) ? L_1eee : L_1ee8 */

L_1ee8:
    /* untranslated: t_merge_1ef1_0001 = part[12:2](GlobalPD) */
    goto L_1ef1;

L_1eee:
    /* untranslated: t_merge_1ef1_0001 = part[10:2](GlobalPD) */

L_1ef1:
    iNewVal = t_merge_1ef1_0001;

L_1ef7:
    /* untranslated: branch part[8:2](GlobalPD) >= part[6:2](GlobalPD) ? L_1f1b : L_1f03 */

L_1f03:
    /* untranslated: branch part[12:2](GlobalPD) <= part[8:2](GlobalPD) ? L_1f15 : L_1f0f */

L_1f0f:
    /* untranslated: t_merge_1f18_0001 = part[12:2](GlobalPD) */
    goto L_1f18;

L_1f15:
    /* untranslated: t_merge_1f18_0001 = part[8:2](GlobalPD) */

L_1f18:
    iNewVal = t_merge_1f18_0001;

L_1f1b:
    if ((iNewVal == -1))
        goto L_2172;
    else
        goto L_1f24;

L_1f24:
    lppl = LpplFromId(HIWORD(GlobalPD));
    pctDesireOld = PctPlanetDesirability(lppl, idPlayer);
    /* untranslated: iValSav = sext8to16(lppl->rgEnvVar[part[4:2](GlobalPD)]) */
    /* untranslated: lppl->rgEnvVar[part[4:2](GlobalPD)] = lobyte(iNewVal) */
    pctDesire = PctPlanetDesirability(lppl, idPlayer);
    /* untranslated: lppl->rgEnvVar[part[4:2](GlobalPD)] = lobyte(iValSav) */
    if ((pctDesireOld >= pctDesire))
        goto L_2172;
    else
        goto L_1fb6;

L_1fb6:
    cch = CchGetString(idsIfTerraform, szWork);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 0x4, (xMax - 4), 0x0, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call WrapTextOut(hdc, &x, &y, rgszPlanetAttr[part[4:2](GlobalPD)], 0x0, 0x4, (xMax - 4), 0x0, 0x0, fPrint) -> callresult(void) */
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &(x), &(y), PszGetCompressedString(idsTo), 0x0, 0x4, (xMax - 4), 0x0, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: call WrapTextOut(hdc, &x, &y, PszCalcEnvVar(part[4:2](GlobalPD), iNewVal), 0x0, 0x4, (xMax - 4), 0x0, 0x0, fPrint) -> callresult(void) */
    SelectObject(hdc, rghfontArial8[0]);
    cch = CchGetString(idsPlanetsValueWouldImprove, szWork);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 0x4, (xMax - 4), 0x0, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    cch = _wsprintf(szWork, "%d%%.  ", pctDesire);
    WrapTextOut(hdc, &(x), &(y), szWork, cch, 0x4, (xMax - 4), 0x0, 0x0, fPrint);

L_2172:
    /* untranslated: branch part[14:2](GlobalPD) == 0xffff ? L_225e : L_217c */

L_217c:
    /* untranslated: branch part[6:2](GlobalPD) == part[12:2](GlobalPD) ? L_225e : L_2188 */

L_2188:
    /* untranslated: branch part[6:2](GlobalPD) == 0xffff ? L_225e : L_2192 */

L_2192:
    /* untranslated: branch part[6:2](GlobalPD) >= part[14:2](GlobalPD) ? L_21ab : L_219e */

L_219e:
    /* untranslated: dChg = (part[14:2](GlobalPD) - part[6:2](GlobalPD)) */
    goto L_21d9;

L_21ab:
    /* untranslated: branch part[6:2](GlobalPD) <= GlobalPD.iPlrMax ? L_21c4 : L_21b7 */

L_21b7:
    /* untranslated: dChg = (part[6:2](GlobalPD) - GlobalPD.iPlrMax) */
    goto L_21d9;

L_21c4:
    /* untranslated: dChg = (abs((part[6:2](GlobalPD) - part[12:2](GlobalPD))) neg 0) */

L_21d9:
    if ((dChg >= 0))
        goto L_21f2;
    else
        goto L_21e2;

L_21e2:
    /* untranslated: dChg = (dChg neg 0) */
    ids = idsValueDAwayIdealValueRace;
    goto L_21f7;

L_21f2:
    ids = idsValueDOutsideHabitableRangeRace;

L_21f7:
    CchGetString(ids, szWork);
    cch = _wsprintf(szOut, szWork, dChg);
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &(x), &(y), szOut, cch, 0x4, (xMax - 4), 0x0, 0x0, fPrint);

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
    /* untranslated: return words(pt.x, pt.y) */
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
    FLookupPlanet(HIWORD(GlobalPD), &(pl));
    SelectObject(hdc, rghfontArial8[0]);
    if ((pl.iPlayer != idPlayer))
        goto L_23eb;
    else
        goto L_22cf;

L_22cf:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsPopulation3), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsIs), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    _wsprintf(szWork, PCTLD00, LOWORD(pl.rgwtMin[0x3]), HIWORD(pl.rgwtMin[0x3]));
    if ((LOWORD(pl.rgwtMin[0x3]) != 0x0))
        goto L_239e;
    else
        goto L_2390;

L_2390:
    if ((HIWORD(pl.rgwtMin[0x3]) != 0x0))
        goto L_239e;
    else
        goto L_2399;

L_2399:
    szWork[1] = 0;

L_239e:
    DxStreamTextOut(hdc, &(x), y, szWork, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);
    goto L_25b2;

L_23eb:
    if ((pl.iPlayer == -1))
        goto L_254a;
    else
        goto L_23f4;

L_23f4:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsEnemyPopulation), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    if ((pl.det < 0x3))
        goto L_2500;
    else
        goto L_246a;

L_246a:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsApproximately), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    _wsprintf(szWork, "%d00", (pl.uPopGuess * 0x4));
    if ((pl.uPopGuess != 0x0))
        goto L_24d2;
    else
        goto L_24cd;

L_24cd:
    szWork[1] = 0;

L_24d2:
    DxStreamTextOut(hdc, &(x), y, szWork, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    goto L_2528;

L_2500:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnknown), 0x0, fPrint);

L_2528:
    DxStreamTextOut(hdc, &(x), y, ".", 0x1, fPrint);
    goto L_25b2;

L_254a:
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUninhabited2), 0x0, fPrint);

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
        goto L_25df;

L_25df:

L_25e5:
    lMax = CalcPlanetMaxPop(HIWORD(GlobalPD), idPlayer);
    pctDesire = PctPlanetDesirability(&(pl), idPlayer);
    if ((pctDesire >= 0))
        goto L_279c;
    else
        goto L_261e;

L_261e:
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWillKillOffApproximately), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    /* untranslated: c = _wsprintf(szWork, PCTDXPCTDPCTPCT, (sext16to32((pctDesire neg 0x0)) / 0xa), (sext16to32((pctDesire neg 0x0)) % 0xa)) */
    DxStreamTextOut(hdc, &(x), y, szWork, c, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsOf), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsEachTurn), 0x0, fPrint);
    goto L_2a58;

L_2771:
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonistsSettleEveryTurn), 0x0, fPrint);

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
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWillSupportPopulation), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonists3), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsIfColonize), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &(x), y, PszGetPlanetName(HIWORD(GlobalPD)), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWould), 0x0, fPrint);
    if ((x <= xMax))
        goto L_2998;
    else
        goto L_2990;

L_2990:
    xMax = x;

L_2998:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsSupport), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    c = _wsprintf(szWork, PCTLD00, LOWORD(lMax), HIWORD(lMax));
    DxStreamTextOut(hdc, &(x), y, szWork, c, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsColonists3), 0x0, fPrint);
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
    if ((HIWORD(pl.rgwtMin[0x3]) > HIWORD(lMax)))
        goto L_2c24;
    else
        goto L_2a9f;

L_2a9f:
    if ((HIWORD(pl.rgwtMin[0x3]) < HIWORD(lMax)))
        goto L_2aac;
    else
        goto L_2aa4;

L_2aa4:
    if ((LOWORD(pl.rgwtMin[0x3]) >= LOWORD(lMax)))
        goto L_2c24;
    else
        goto L_2aac;

L_2aac:
    c = CchGetString(idsPopulation, szT);
    WrapTextOut(hdc, &(x), &(y), szT, c, 0x4, xMax, 0x0, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    psz = PszGetPlanetName(pl.id);
    WrapTextOut(hdc, &(x), &(y), psz, 0x0, 0x4, xMax, 0x0, 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    lPopChg = ChgPopFromPlanet(&(pl), 0x0);
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
    c = _wsprintf(szT, psz, LOWORD(lPopChg), HIWORD(lPopChg), (LOWORD(pl.rgwtMin[0x3]) + LOWORD(lPopChg)), (HIWORD(pl.rgwtMin[0x3]) + HIWORD(lPopChg)));

L_2be3:
    WrapTextOut(hdc, &(x), &(y), szT, c, 0x4, xMax, 0x0, 0x0, fPrint);
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
    WrapTextOut(hdc, &(x), &(y), psz, 0x0, 0x4, xMax, 0x0, 0x0, fPrint);
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
    WrapTextOut(hdc, &(x), &(y), psz, c, 0x4, xMax, 0x0, 0x0, fPrint);
    x = 4;
    y = (y + dyArial8);

AfterDesire:
    pt.x = (xMax + 4);
    pt.y = (y + 4);

L_2d5f:
    /* untranslated: return words(pt.x, pt.y) */
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsZipordProvidesAbilityQuicklySetFleets), 0x0, fPrint);
    if ((x <= xMax))
        goto L_2e0a;
    else
        goto L_2e04;

L_2e04:
    xMax = x;

L_2e0a:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsTransportOrdersOne3CommonSetsSelect), 0x0, fPrint);
    if ((x <= xMax))
        goto L_2e4c;
    else
        goto L_2e46;

L_2e46:
    xMax = x;

L_2e4c:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsZipordClickDiamondRightMouse), 0x0, fPrint);
    if ((x <= xMax))
        goto L_2e8e;
    else
        goto L_2e88;

L_2e88:
    xMax = x;

L_2e8e:
    x = 4;
    y = (y + dyArial8);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsButtonClickOrderChoice), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsQuikload), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsLoadMineralsAvailable), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsQuikdrop), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsUnloadEverythingFleetCarrying), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWaitload), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsWaitFullLoadMinerals), 0x0, fPrint);
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
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsClear2), 0x0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &(x), y, PszGetCompressedString(idsRemoveTransportOrders), 0x0, fPrint);
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
    /* untranslated: return words(pt.x, pt.y) */
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
    uint16_t t_merge_3201_0001;
    char    *t_merge_3293_0001;

L_3110:
    ids = idsHave;
    dx = (dx - 8);
    xMax = 4;
    x = 4;
    y = 2;
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_314e : L_3141 */

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
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_323c;
    else
        goto L_31e2;

L_31e2:
    SelectObject(hdc, rghfontArial8[0]);
    /* untranslated: branch part[10:2](GlobalPD) == 0x0 ? L_31fe : L_31f8 */

L_31f8:
    t_merge_3201_0001 = 0x4eb;
    goto L_3201;

L_31fe:
    t_merge_3201_0001 = 0x4ec;

L_3201:
    psz = PszGetCompressedString(t_merge_3201_0001);
    WrapTextOut(hdc, &(x), &(y), psz, 0x0, 0x4, dx, &(xMax), 0x0, fPrint);
    goto LDone;

L_323c:
    i = 0;
    goto L_3349;

L_3244:
    SelectObject(hdc, rghfontArial8[1]);
    goto L_32f6;
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(ids);
    ids = (ids + 1);
    goto L_3319;
    /* untranslated: cnt = part[6:2](GlobalPD) */

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
    _wsprintf(szWork, "%d %s", cnt, t_merge_3293_0001);
    psz = szWork;
    goto L_3319;
    psz = PszGetPlanetName(HIWORD(GlobalPD));
    goto L_3319;
    /* untranslated: cnt = part[4:2](GlobalPD) */
    goto SetQuan;
    /* untranslated: call _wsprintf(szWork, PCTD, part[8:2](GlobalPD)) -> callresult(int16_t) */
    psz = szWork;
    goto L_3319;

L_32f6:

L_3319:
    WrapTextOut(hdc, &(x), &(y), psz, 0x0, 0x4, dx, &(xMax), 0x0, fPrint);
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
    /* untranslated: return words(pt.x, pt.y) */
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
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
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
    /* untranslated: branch part[6:2](GlobalPD) == part[4:2](GlobalPD) ? L_3536 : L_3442 */

L_3442:

L_3448:
    SelectObject(hdc, rghfontArial8[0]);
    psz = PszGetCompressedString(ids);
    ids = (ids + 1);
    goto L_34fb;
    /* untranslated: cnt = part[4:2](GlobalPD) */

SetQuan:
    _wsprintf(szWork, PCTD, cnt);
    psz = szWork;
    goto L_34fb;
    psz = PszGetPlanetName(HIWORD(GlobalPD));
    goto L_34fb;
    /* untranslated: cnt = (part[4:2](GlobalPD) - part[6:2](GlobalPD)) */
    if ((cnt > 0))
        goto SetQuan;
    else
        goto L_34b2;

L_34b2:

L_34b8:
    psz = PszGetCompressedString(idsNone2);
    goto L_34fb;
    /* untranslated: cnt = part[6:2](GlobalPD) */
    goto SetQuan;

L_34d6:

L_34fb:

L_352b:
    if ((i <= iMax))
        goto L_3427;
    else
        goto OutOfFor;

OutOfFor:
    pt.x = (xMax + 4);
    pt.y = ((y + dyArial8) + 2);

L_3555:
    /* untranslated: return words(pt.x, pt.y) */
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
    /* untranslated: call WrapTextOut(hdc, &x, &y, part[4:2](GlobalPD), 0x0, 0x4, dx, &xMax, 0x0, fPrint) -> callresult(void) */
    pt.x = (xMax + 4);
    pt.y = ((y + dyArial8) + 2);

L_35d6:
    /* untranslated: return words(pt.x, pt.y) */
}
