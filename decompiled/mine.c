#include "common.h"

int32_t MineWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rc;
    POINT       pt;
    HtMineType  ht;
    COLORREF    crFore;
    uint16_t    dxMax;
    COLORREF    crBack;
    int16_t     cch;
    RECT        rc2;
    int16_t     fDetonate;
    RTLOGTHING  rtlt;
    HCURSOR     t_merge_0170_0001;

L_0000:
    goto L_03ad;

L_000f:
    hwndMineCB = CreateWindow(szButton, PszGetCompressedString(idsDetonateMineFieldYear), 0x40000003, 0x64, 0x64, 0x96, dyArial8, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndMineCB, WM_SETFONT, HIWORD(rghfontArial8), 0x0);
    SetMineralTitleBar(hwnd);
    goto L_03f8;

L_0080:
    if ((hwndPopup != 0x0))
        goto L_03f8;
    else
        goto L_008a;

L_008a:
    MineClick(LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), message, wParam);

L_00b9:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_00e4:
    SetBkColor(wParam, crButtonFace);
    /* untranslated: return words(hbrButtonFace, 0x0) */

L_00fd:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    GetClientRect(hwnd, &(rc));
    /* untranslated: branch PtInRect(&rc, words(pt.y, pt.x)) == 0x0 ? L_0393 : L_0141 */

L_0141:
    ht = HtMineWindow(hwnd, pt.x, pt.y);
    if ((ht == htMineNone))
        goto Default;
    else
        goto L_015e;

L_015e:
    if ((ht != htMineScanSel))
        goto L_016d;
    else
        goto L_0167;

L_0167:
    t_merge_0170_0001 = hcurHand;
    goto L_0170;

L_016d:
    t_merge_0170_0001 = hcurArrowHelp;

L_0170:
    SetCursor(t_merge_0170_0001);
    return 0x10000;

L_0182:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    SetRect(&(rc2), 0x4, 0x4, (rc.right + 0xfffc), ((dyArial8 * 0x2) + 0xfffc));
    _Draw3dFrame(hdc, &(rc2), 0x0);
    crFore = SetTextColor(hdc, crButtonText);
    crBack = SetBkColor(hdc, crButtonFace);
    cch = strlen(0x259c);
    dxMax = ((rc2.right - rc2.left) + 0xffe8);

L_0221:
    if ((cch <= 0x0))
        goto L_024b;
    else
        goto L_022a;

L_022a:
    if ((LOWORD(GetTextExtent(hdc, szMineralTitle, cch)) <= dxMax))
        goto L_024b;
    else
        goto L_0244;

L_0244:
    cch = (cch - 0x1);
    goto L_0221;

L_024b:
    rc2.right = (rc2.right - 0x8);
    RcCtrTextOut(hdc, &(rc2), 0x259c, cch);
    rc2.right = (rc2.right + 0x8);
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    SetRect(&(rc2), ((rc2.right - (rc2.bottom - rc2.top)) + 0x2), (rc2.top + 0x3), (rc2.right + 0xfffc), (rc2.bottom + 0xfffe));
    DrawSelectionArrow(hdc, &(rc2), FOtherStuffAtScanSel());
    rc.top = (rc.top + ((dyArial8 * 0x2) + 0xfffc));
    DrawMineSurvey(hdc, &(rc));
    EndPaint(hwnd, &(ps));
    goto L_03f8;

L_02f6:
    if ((LOWORD(lParam) != hwndMineCB))
        goto L_03f8;
    else
        goto L_0305;

L_0305:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_03f8;
    else
        goto L_0321;

L_0321:
    fDetonate = LOWORD(SendMessage(hwndMineCB, WM_USER, 0x0, 0x0));
    rtlt.idFull = LOWORD(lpThings[sel.scan.ith]);
    rtlt.fDetonate = fDetonate;
    WriteMemRt(0x2b, 0x4, rtlt);
    /* untranslated: part[13:1](lpThings[sel.scan.ith]) = lobyte(fDetonate) */

Default:
    return DefWindowProc(hwnd, message, wParam, lParam);

L_03ad:
    if ((message == WM_CREATE))
        goto L_000f;
    else
        goto L_03b5;

L_03b5:
    if ((message == WM_PAINT))
        goto L_0182;
    else
        goto L_03bd;

L_03bd:
    if ((message == WM_ERASEBKGND))
        goto L_00b9;
    else
        goto L_03c5;

L_03c5:
    if ((message == WM_CTLCOLOR))
        goto L_00e4;
    else
        goto L_03cd;

L_03cd:
    if ((message == WM_SETCURSOR))
        goto L_00fd;
    else
        goto L_03d5;

L_03d5:
    if ((message == WM_COMMAND))
        goto L_02f6;
    else
        goto L_03dd;

L_03dd:
    if ((message == WM_LBUTTONDOWN))
        goto L_0080;
    else
        goto L_03e5;

L_03e5:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_0080;
    else
        goto L_03ed;

L_03ed:
    if ((message != WM_RBUTTONDOWN))
        goto Default;
    else
        goto L_03f2;

L_03f2:

L_03f8:
    return 0x0;
}

void InvalidateMineralBars() {
    HDC     hdc;
    int16_t dyRow;
    HFONT   hfontSav;
    RECT    rcPop;
    int16_t dx;
    int16_t dxPop;
    RECT    rc;

L_040a:
    GetClientRect(hwndMine, &(rc));
    hdc = GetDC(hwndMine);
    hfontSav = SelectObject(hdc, LOWORD(rghfontArial8));
    rc.right = (rc.right - (LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN999mr), 0x5)) + 0x6));
    SelectObject(hdc, HIWORD(rghfontArial8));
    dxPop = (LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsPopulation1000000), 0x15)) + 0x6);
    dx = (LOWORD(GetTextExtent(hdc, HIWORD(rgszPlanetAttr), strlen(HIWORD(rgszPlanetAttr)))) + 0x6);
    if (((dx * 0x4) <= rc.right))
        goto L_04db;
    else
        goto L_04bf;

L_04bf:
    rc.left = (rc.left + (LOWORD(GetTextExtent(hdc, HIWORD(rgszPlanetAttr), 0x4)) + 0x6));
    goto L_04e1;

L_04db:
    rc.left = (rc.left + dx);

L_04e1:
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwndMine, hdc);
    rc.top = (rc.top + ((dyArial8 * 0x2) + 0xfffc));
    rcPop.top = (rc.top + 0x2);
    rcPop.bottom = (rcPop.top + dyArial8);
    rcPop.right = rc.right;
    rcPop.left = (rc.right - dxPop);
    InvalidateRect(hwndMine, &(rcPop), 0x1);
    dyRow = ((uint32_t)((((rc.bottom - rc.top) - (dyArial8 * 0x4)) + 0xfffe)) / 0x6);
    dyRow = ((dyRow + 0x1) & 0xfffe);
    rc.top = (rc.top + (((LOWORD((0x5 * dyArial8)) >> 0x1) + LOWORD((0x3 * dyRow))) + 0x1));
    rc.bottom = (LOWORD((0x3 * dyRow)) + rc.top);
    InvalidateRect(hwndMine, &(rc), 0x0);
    return;
}

void GetMineFieldCounts(uint16_t id, int16_t *pithm, int16_t *pcthm) {
    int16_t cthTotal;
    int16_t ithFound;
    THING  *lpth;
    THING  *lpthMac;

L_05ac:
    ithFound = 0x0;
    cthTotal = 0x0;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_0635;

L_05ea:
    if ((lpth->ith != ithMinefield))
        goto L_0631;
    else
        goto L_0600;

L_0600:
    if ((lpth->iplr != idPlayer))
        goto L_0631;
    else
        goto L_0619;

L_0619:
    cthTotal = (cthTotal + 0x1);
    if ((LOWORD(lpth) != id))
        goto L_0631;
    else
        goto L_062b;

L_062b:
    ithFound = cthTotal;

L_0631:
    lpth = (lpth + 0x1);

L_0635:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_05ea;
    else
        goto L_0643;

L_0643:
    *(pithm) = ithFound;
    *(pcthm) = cthTotal;
    return;
}

void DrawMineSurvey(HDC hdc, RECT *prc) {
    PLANET     pl;
    HBRUSH     hbrSav;
    int32_t    l2;
    COLORREF   crFore;
    int16_t    c2;
    int32_t    rgl[3];
    int16_t    c;
    int16_t    i;
    FLEET     *lpfl;
    COLORREF   crBack;
    HDC        hdcMem;
    int16_t    bkMode;
    char      *psz;
    int16_t    cch;
    RECT       rcGauge;
    char       szT[80];
    GrobjClass grobj;
    int32_t    l;
    RECT       rc;
    int32_t    cMass;
    int16_t    yTop;
    int16_t    xLeft;
    int32_t    cShip;
    int16_t    iOffset;
    ORDER     *lpord;
    char       szWP[30];
    char      *pszT;
    THING     *lpth;
    int32_t    pctDecay;
    int16_t    ibmp;
    int32_t    lDecay;
    int16_t    iplrbmp;
    THING     *lpthDest;
    int16_t    fCanTerraform;
    int16_t    rgMin[3];
    int16_t    xL;
    int16_t    dyRow;
    int16_t    iMax;
    int16_t    yBot;
    int16_t    fShortLabels;
    int16_t    cNum;
    int16_t    dy;
    PLAYER     plrSav;
    int16_t    iMin;
    int16_t    xEnd;
    int16_t    yCur;
    int16_t    dxBar;
    int16_t    dxNum;
    int16_t    xR;
    int16_t    dxRLabels;
    int16_t    iCur;
    int16_t    rgCost[3];
    int16_t    rgMax[3];
    int16_t    dxLabels;
    int16_t    dx;
    int16_t    xBeg;
    int16_t    dNum;
    HBITMAP    hbmpSav;
    COLORREF   crBkSav;
    COLORREF   crTextSav;
    int16_t    dBest;
    POINT      pt;
    int16_t    iT;
    int32_t    rglT[3];
    int16_t    ifl;
    int32_t    cMines;
    int16_t    iPass;
    uint16_t   t_merge_0d0c_0001;
    uint16_t   t_merge_0dc1_0001;
    uint16_t   t_merge_0ea0_0001;
    uint16_t   t_merge_0f67_0001;
    uint16_t   t_merge_0fc2_0001;
    uint16_t   t_merge_10ce_0001;
    uint16_t   t_merge_1120_0001;
    uint16_t   t_merge_127e_0001;
    uint16_t   t_merge_1d8c_0001;
    uint32_t   t_merge_1eae_0001_wide;
    uint16_t   t_merge_2331_0001;
    uint32_t   t_merge_23d4_0001;
    uint32_t   t_merge_2466_0001;
    uint16_t   t_merge_24d7_0001;
    uint16_t   t_merge_26f6_0001;
    uint32_t   t_merge_27c7_0001;
    uint16_t   t_merge_2a3b_0001;
    int16_t    t_2c6c;
    int16_t    t_2cca;
    uint16_t   t_merge_2d4a_0001;
    uint16_t   t_merge_2dbf_0001;
    uint16_t   t_merge_3357_0001;
    uint32_t   t_merge_33e5_0001;
    uint32_t   t_merge_33e5_0002;
    uint32_t   t_merge_3519_0001;
    uint32_t   t_merge_3519_0002;
    uint16_t   t_merge_3609_0001;
    int16_t    t_3636;
    int16_t    t_3668;
    int16_t    t_3672;

L_065a:
    hdcMem = CreateCompatibleDC(hdc);
    crBack = SetBkColor(hdc, crButtonFace);
    crFore = SetTextColor(hdc, 0xffff);
    bkMode = SetBkMode(hdc, OPAQUE);
    grobj = sel.scan.grobj;
    if ((grobj != grobjOther))
        goto L_0705;
    else
        goto L_06ba;

L_06ba:
    if (((sel.scan.grobjFull & 0x1) == 0x0))
        goto L_06d1;
    else
        goto L_06c8;

L_06c8:
    grobj = grobjPlanet;
    goto L_0705;

L_06d1:
    if (((sel.scan.grobjFull & 0x2) == 0x0))
        goto L_06e8;
    else
        goto L_06df;

L_06df:
    grobj = grobjFleet;
    goto L_0705;

L_06e8:
    if (((sel.scan.grobjFull & 0x8) == 0x0))
        goto L_06ff;
    else
        goto L_06f6;

L_06f6:
    grobj = grobjThing;
    goto L_0705;

L_06ff:
    grobj = grobjNone;

L_0705:
    if ((grobj == grobjNone))
        goto FinishUp;
    else
        goto L_070c;

L_070c:

L_0712:
    if ((grobj != grobjFleet))
        goto L_1158;
    else
        goto L_071c;

L_071c:
    cMass = 0x0;
    if ((sel.scan.ifl == 0xffff))
        goto FinishUp;
    else
        goto L_072f;

L_072f:

L_0735:
    xLeft = (prc->left + 0x6);
    yTop = (prc->top + 0x6);
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xLeft, yTop, 0x46, 0x2, PATCOPY);
    PatBlt(hdc, xLeft, (yTop + 0x2), 0x2, 0x44, PATCOPY);
    PatBlt(hdc, (xLeft + 0x10), (yTop + 0x44), 0x2, 0x26, PATCOPY);
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x44), 0xf, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x1), (yTop + 0x45), 0xf, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x34), (yTop + 0x44), 0x12, 0x2, PATCOPY);
    PatBlt(hdc, (xLeft + 0x44), (yTop + 0x2), 0x2, 0x42, PATCOPY);
    PatBlt(hdc, (xLeft + 0x45), (yTop + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x11), (yTop + 0x68), 0x25, 0x2, PATCOPY);
    PatBlt(hdc, (xLeft + 0x10), (yTop + 0x69), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x34), (yTop + 0x46), 0x2, 0x22, PATCOPY);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x2), 0x42, 0x42, BLACKNESS);
    PatBlt(hdc, (xLeft + 0x12), (yTop + 0x44), 0x22, 0x24, BLACKNESS);
    SelectObject(hdc, hbrSav);
    lpfl = rglpfl[sel.scan.ifl];
    DrawFleetBitmap(lpfl, hdc, (xLeft + 0x2), (yTop + 0x2), 0x0, 0xffff, 0x0, 0x0, 0xffff, 0x0);
    /* untranslated: iOffset = ((part[6:2](rgplr[lpfl->iplr]) >> 0x3) & 0x1f) */
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, (xLeft + 0x13), (yTop + 0x47), 0x20, 0x20, hdibRaces, ((iOffset & 0x7) * 0x20), ((0x3 - (iOffset >> 0x3)) * 0x20), 0x20, 0x20, 0xcc0020);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    cShip = 0x0;
    i = 0x0;
    goto L_0ad3;

L_0aaf:
    cShip = (cShip + (uint32_t)(lpfl->rgcsh[i]));
    i = (i + 0x1);

L_0ad3:
    if ((i < 0x10))
        goto L_0aaf;
    else
        goto L_0adc;

L_0adc:
    CchGetString(idsShipCountLd, szT);
    c = _wsprintf(szWork, &(szT), LOWORD(cShip), HIWORD(cShip));
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((lpfl->det == 0x7))
        goto L_0b5c;
    else
        goto L_0b4a;

L_0b4a:
    if ((lpfl->det != 0x4))
        goto L_0cd7;
    else
        goto L_0b5c;

L_0b5c:
    cMass = WtFromLpfl(lpfl);
    c = CchGetString(idsFuel2, szT);
    l = GetTextExtent(hdc, &(szT), c);
    c2 = CchGetString(idsCargo, 0x57a4);
    l2 = GetTextExtent(hdc, szWork, c2);
    if ((HIWORD(l2) < HIWORD(l)))
        goto L_0bf7;
    else
        goto L_0bdc;

L_0bdc:
    if ((HIWORD(l2) > HIWORD(l)))
        goto L_0be9;
    else
        goto L_0be1;

L_0be1:
    if ((LOWORD(l2) <= LOWORD(l)))
        goto L_0bf7;
    else
        goto L_0be9;

L_0be9:
    l = l2;

L_0bf7:
    SetRect(&(rcGauge), ((prc->left + 0x5a) + LOWORD(l)), yTop, (prc->right + 0xfffc), (yTop + dyArial8));
    if ((rcGauge.left >= rcGauge.right))
        goto L_0cea;
    else
        goto L_0c38;

L_0c38:
    TextOut(hdc, (prc->left + 0x56), yTop, &(szT), c);
    yTop = (yTop + (dyArial8 + 0x2));
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c2);
    DrawFleetGauge(hdc, &(rcGauge), lpfl, 0x4);
    OffsetRc(&(rcGauge), 0x0, (dyArial8 + 0x4));
    DrawFleetGauge(hdc, &(rcGauge), lpfl, 0x5);
    yTop = (yTop + (dyArial8 + 0x2));

L_0cd7:
    LOWORD(cMass) = *(lpfl + 0x2c);
    HIWORD(cMass) = *(lpfl + 0x2e);

L_0cea:
    if ((gd.fSmallTileMode == 0x0))
        goto L_0d09;
    else
        goto L_0d03;

L_0d03:
    t_merge_0d0c_0001 = 0x276;
    goto L_0d0c;

L_0d09:
    t_merge_0d0c_0001 = 0x21d;

L_0d0c:
    c = CchGetString(t_merge_0d0c_0001, szT);
    c = _wsprintf(szWork, &(szT), LOWORD(cMass), HIWORD(cMass));
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((lpfl->det != 0x7))
        goto L_1080;
    else
        goto L_0d75;

L_0d75:
    lpord = &(lpfl->lpplord->rgord);
    if ((lpfl->cord <= 0x1))
        goto L_0d9f;
    else
        goto L_0d9a;

L_0d9a:
    lpord = (lpord + 0x1);

L_0d9f:
    if ((gd.fSmallTileMode == 0x0))
        goto L_0dbe;
    else
        goto L_0db8;

L_0db8:
    t_merge_0dc1_0001 = 0x273;
    goto L_0dc1;

L_0dbe:
    t_merge_0dc1_0001 = 0x21a;

L_0dc1:
    CchGetString(t_merge_0dc1_0001, szWP);
    if ((lpfl->cord != 0x1))
        goto L_0e05;
    else
        goto L_0dd7;

L_0dd7:
    c = _wsprintf(&(szT), &(szWP), PszGetCompressedString(idsNone), 0x25);
    goto L_0e54;

L_0e05:
    c = _wsprintf(&(szT), &(szWP), PszGetLocName(lpord->grobj, lpord->id, lpord->pt.x, lpord->pt.y), 0x25);

L_0e54:
    TextOut(hdc, (prc->left + 0x56), yTop, &(szT), c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((gd.fSmallTileMode == 0x0))
        goto L_0e9d;
    else
        goto L_0e97;

L_0e97:
    t_merge_0ea0_0001 = 0x272;
    goto L_0ea0;

L_0e9d:
    t_merge_0ea0_0001 = 0x219;

L_0ea0:
    CchGetString(t_merge_0ea0_0001, szT);
    c = _wsprintf(szWork, &(szT), PszGetCompressedString((lpord->grTask + 0x63)), 0x25);
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((lpfl->cord <= 0x1))
        goto L_0fa1;
    else
        goto L_0f14;

L_0f14:
    if ((lpord->iWarp != 0xb))
        goto L_0f45;
    else
        goto L_0f2f;

L_0f2f:
    c = CchGetString(idsUseStargate, 0x57a4);
    goto L_0fce;

L_0f45:
    if ((gd.fSmallTileMode == 0x0))
        goto L_0f64;
    else
        goto L_0f5e;

L_0f5e:
    t_merge_0f67_0001 = 0x274;
    goto L_0f67;

L_0f64:
    t_merge_0f67_0001 = 0x21b;

L_0f67:
    CchGetString(t_merge_0f67_0001, szT);
    c = _wsprintf(szWork, &(szT), lpord->iWarp);

L_0fa1:
    if ((gd.fSmallTileMode == 0x0))
        goto L_0fbf;
    else
        goto L_0fb9;

L_0fb9:
    t_merge_0fc2_0001 = 0x275;
    goto L_0fc2;

L_0fbf:
    t_merge_0fc2_0001 = 0x21c;

L_0fc2:
    c = CchGetString(t_merge_0fc2_0001, 0x57a4);

L_0fce:
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    CMineSweepFromLpfl(lpfl);
    /* untranslated: l = callresult(int32_t) */
    /* untranslated: branch hiword(callresult(int32_t)) < 0x0 ? L_3776 : L_1015 */

L_1015:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_1022 : L_101a */

L_101a:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_3776 : L_1022 */

L_1022:
    pszT = PszGetCompressedString(idsFleetCanDestroyLdMinesPerYear);
    c = _wsprintf(szWork, pszT, LOWORD(l), HIWORD(l));
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));

L_1080:
    if ((lpfl->fdirValid == 0x0))
        goto FinishUp;
    else
        goto L_109a;

L_109a:
    if ((lpfl->iwarpFlt == 0x0))
        goto L_10ff;
    else
        goto L_10ac;

L_10ac:
    if ((gd.fSmallTileMode == 0x0))
        goto L_10cb;
    else
        goto L_10c5;

L_10c5:
    t_merge_10ce_0001 = 0x274;
    goto L_10ce;

L_10cb:
    t_merge_10ce_0001 = 0x21b;

L_10ce:
    CchGetString(t_merge_10ce_0001, szT);
    c = _wsprintf(szWork, &(szT), lpfl->iwarpFlt);
    goto L_112c;

L_10ff:
    if ((gd.fSmallTileMode == 0x0))
        goto L_111d;
    else
        goto L_1117;

L_1117:
    t_merge_1120_0001 = 0x275;
    goto L_1120;

L_111d:
    t_merge_1120_0001 = 0x21c;

L_1120:
    c = CchGetString(t_merge_1120_0001, 0x57a4);

L_112c:
    TextOut(hdc, (prc->left + 0x56), yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));

L_1158:
    if ((grobj != grobjThing))
        goto L_1f96;
    else
        goto L_1162;

L_1162:
    if ((sel.scan.ith == 0xffff))
        goto FinishUp;
    else
        goto L_1169;

L_1169:

L_116f:
    lpth = &(lpThings[sel.scan.ith]);
    /* untranslated: iplrbmp = ((part[6:2](rgplr[lpth->iplr]) >> 0x3) & 0x1f) */
    xLeft = (prc->left + 0x6);
    yTop = (prc->top + 0x6);
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xLeft, yTop, 0x46, 0x2, PATCOPY);
    PatBlt(hdc, xLeft, (yTop + 0x2), 0x2, 0x44, PATCOPY);
    if ((lpth->ith != ithMinefield))
        goto L_1246;
    else
        goto L_1234;

L_1234:
    ibmp = lpth->thm.iType;
    goto L_12ae;

L_1246:
    if ((lpth->ith != ithMineralPacket))
        goto L_1288;
    else
        goto L_125d;

L_125d:
    if ((((LOWORD(lpth->thp) >> 0xa) & 0xf) == 0x0))
        goto L_127b;
    else
        goto L_1275;

L_1275:
    t_merge_127e_0001 = 0x1;
    goto L_127e;

L_127b:
    t_merge_127e_0001 = 0x0;

L_127e:
    ibmp = (t_merge_127e_0001 + 0x3);
    goto L_12ae;

L_1288:
    if ((lpth->ith != ithMysteryTrader))
        goto L_12a8;
    else
        goto L_129f;

L_129f:
    ibmp = 0x6;
    goto L_12ae;

L_12a8:
    ibmp = 0x5;

L_12ae:
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    if ((lpth->ith == ithWormhole))
        goto L_151e;
    else
        goto L_12dd;

L_12dd:
    if ((lpth->ith == ithMysteryTrader))
        goto L_151e;
    else
        goto L_12f4;

L_12f4:
    PatBlt(hdc, (xLeft + 0x10), (yTop + 0x44), 0x2, 0x26, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x44), 0xf, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x1), (yTop + 0x45), 0xf, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x34), (yTop + 0x44), 0x12, 0x2, PATCOPY);
    PatBlt(hdc, (xLeft + 0x44), (yTop + 0x2), 0x2, 0x42, PATCOPY);
    PatBlt(hdc, (xLeft + 0x45), (yTop + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x11), (yTop + 0x68), 0x25, 0x2, PATCOPY);
    PatBlt(hdc, (xLeft + 0x10), (yTop + 0x69), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x34), (yTop + 0x46), 0x2, 0x22, PATCOPY);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x2), 0x42, 0x42, BLACKNESS);
    PatBlt(hdc, (xLeft + 0x12), (yTop + 0x44), 0x22, 0x24, BLACKNESS);
    DibBlt(hdc, (xLeft + 0x13), (yTop + 0x47), 0x20, 0x20, hdibRaces, ((iplrbmp & 0x7) * 0x20), ((0x3 - (iplrbmp >> 0x3)) * 0x20), 0x20, 0x20, 0xcc0020);
    goto L_15f2;

L_151e:
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x44), 0x44, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x1), (yTop + 0x45), 0x45, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x44), (yTop + 0x2), 0x2, 0x42, PATCOPY);
    PatBlt(hdc, (xLeft + 0x45), (yTop + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (xLeft + 0x2), (yTop + 0x2), 0x42, 0x42, BLACKNESS);

L_15f2:
    SelectObject(hdc, hbrSav);
    DibBlt(hdc, (xLeft + 0x2), (yTop + 0x2), 0x40, 0x40, hdibThings, (ibmp * 0x40), 0x0, 0x40, 0x40, 0xcc0020);
    if ((lpth->ith != ithMineralPacket))
        goto L_1852;
    else
        goto L_1659;

L_1659:
    xLeft = (xLeft + 0x50);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    if ((((LOWORD(lpth->thp) >> 0xa) & 0xf) == 0x0))
        goto L_1763;
    else
        goto L_16a2;

L_16a2:
    CchGetString(idsTravelingWarpD, szT);
    c = _wsprintf(szWork, &(szT), (((LOWORD(lpth->thp) >> 0xa) & 0xf) + 0x4));
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    CchGetString(idsDestination, szT);
    psz = PszGetPlanetName((LOWORD(lpth->thp) & 0x3ff));
    strcat(szT, psz);
    TextOut(hdc, xLeft, yTop, &(szT), strlen(szT));

L_1763:
    /* untranslated: yTop = (yTop + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    /* untranslated: xLeft = (xLeft + loword(GetTextExtent(hdc, part[4:2](rgszMinerals), strlen(part[4:2](rgszMinerals))))) */
    i = 0x0;
    goto L_1843;

L_179c:
    c = _wsprintf(szWork, PszGetCompressedString(idsS2), rgszMinerals[i], 0x25);
    RightTextOut(hdc, xLeft, yTop, 0x57a4, c, 0x0);
    /* untranslated: c = _wsprintf(szWork, PCTDKT, HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    i = (i + 0x1);

L_1843:
    if ((i >= 0x3))
        goto FinishUp;
    else
        goto L_1849;

L_1849:

L_1852:
    if ((lpth->ith != ithWormhole))
        goto L_1aae;
    else
        goto L_1869;

L_1869:
    xLeft = (xLeft + 0x5e);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    cch = CchGetString(idsLocation, szT);
    xLeft = (xLeft + (LOWORD(GetTextExtent(hdc, &(szT), cch)) + 0x14));
    RightTextOut(hdc, (xLeft + 0xfffc), yTop, szT, cch, 0x0);
    cch = CchGetString(idsDD5, szT);
    c = _wsprintf(szWork, &(szT), lpth->pt.x, lpth->pt.y);
    TextOut(hdc, xLeft, yTop, szWork, c);
    /* untranslated: yTop = (yTop + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    c = CchGetString(idsDestination2, 0x57a4);
    RightTextOut(hdc, (xLeft + 0xfffc), yTop, 0x57a4, c, 0x0);
    if ((((0x1 << idPlayer) & lpth->thw.grbitPlrTrav) == 0x0))
        goto L_19f8;
    else
        goto L_19a2;

L_19a2:
    LpthFromId(lpth->thw.idPartner);
    /* untranslated: lpthDest = callresult(THING *) */
    /* untranslated: branch faroff(callresult(THING *)) != 0x0 ? L_19cb : L_19c3 */

L_19c3:
    /* untranslated: branch farseg(callresult(THING *)) == 0x0 ? L_19f8 : L_19cb */

L_19cb:
    c = _wsprintf(szWork, &(szT), lpthDest->pt.x, lpthDest->pt.y);
    goto L_1a0b;

L_19f8:
    c = CchGetString(idsUnknown2, 0x57a4);

L_1a0b:
    TextOut(hdc, xLeft, yTop, szWork, c);
    /* untranslated: yTop = (yTop + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    c = CchGetString(idsStability, 0x57a4);
    RightTextOut(hdc, (xLeft + 0xfffc), yTop, 0x57a4, c, 0x0);
    c = CchGetString((PctWormholeMoves(lpth) + 0x3c7), 0x57a4);
    TextOut(hdc, xLeft, yTop, szWork, c);
    goto FinishUp;

L_1aae:
    if ((lpth->ith != ithMysteryTrader))
        goto L_1bc0;
    else
        goto L_1ac5;

L_1ac5:
    xLeft = (xLeft + 0x50);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    SetRect(&(rc), xLeft, yTop, (prc->right + 0xfff8), (prc->bottom + 0xfff8));
    if ((((0x1 << idPlayer) & lpth->tht.grbitPlr) != 0x0))
        goto L_1b75;
    else
        goto L_1b38;

L_1b38:
    psz = PszGetCompressedString(idsTraderRequestsInterestedPartiesSendFleetLeast);
    yTop = (yTop + (DrawText(hdc, psz, strlen(psz), &(rc), 0x810) + 0x8));

L_1b75:
    cch = _wsprintf(szWork, PszGetCompressedString(idsTraderTravelingWarpD), lpth->tht.iWarp);
    TextOut(hdc, xLeft, yTop, szWork, cch);
    goto FinishUp;

L_1bc0:
    if ((lpth->ith != ithMinefield))
        goto FinishUp;
    else
        goto L_1bd4;

L_1bd4:

L_1bda:
    xLeft = (xLeft + 0x50);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    CchGetString(idsLocationDD, szT);
    c = _wsprintf(szWork, &(szT), lpth->pt.x, lpth->pt.y);
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    CchGetString(idsFieldTypeS, szT);
    c = _wsprintf(szWork, &(szT), rgszMineField[lpth->thm.iType], 0x25);
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    CchGetString(idsFieldRadiusDLYLdMines, szT);
    sqrt((double)(lpth->thm.cMines));
    c = _wsprintf(szWork, &(szT), LOWORD(__ftol()), LOWORD(lpth->thm), HIWORD(lpth->thm.cMines));
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((GetRaceStat(rgplr[lpth->iplr], rsMajorAdv) == raMines))
        goto L_1d89;
    else
        goto L_1d83;

L_1d83:
    t_merge_1d8c_0001 = 0x1;
    goto L_1d8c;

L_1d89:
    t_merge_1d8c_0001 = 0x0;

L_1d8c:
    /* untranslated: ss:[bp-0xe2] = (loword((t_merge_1d8c_0001 * 0x3)) + 0x1) */
    /* untranslated: call CPlanetsInCircle(words(lpth->pt.y, lpth->pt.x), words(HIWORD(lpth->thm.cMines), LOWORD(lpth->thm))) -> callresult(int16_t) */
    /* untranslated: LOWORD(pctDecay) = (loword((ss:[bp-0xe2] * callresult(int16_t))) + 0x2) */
    /* untranslated: HIWORD(pctDecay) = signhiword((loword((ss:[bp-0xe2] * callresult(int16_t))) + 0x2)) */
    if ((HIWORD(pctDecay) < 0x0))
        goto L_1df1;
    else
        goto L_1dd6;

L_1dd6:
    if ((HIWORD(pctDecay) > 0x0))
        goto L_1de5;
    else
        goto L_1ddb;

L_1ddb:
    if ((LOWORD(pctDecay) <= 0x32))
        goto L_1df1;
    else
        goto L_1de5;

L_1de5:
    pctDecay = 0x32;

L_1df1:
    if ((lpth->thm.fDetonate == 0x0))
        goto L_1e0e;
    else
        goto L_1e04;

L_1e04:
    pctDecay = (pctDecay + 0x19);

L_1e0e:
    /* untranslated: lDecay = (int32_t)((uint32_t)(words(HIWORD(lpth->thm.cMines), LOWORD(lpth->thm)) * pctDecay) / 0x64) */
    if ((HIWORD(lDecay) > HIWORD(pctDecay)))
        goto L_1e6d;
    else
        goto L_1e4f;

L_1e4f:
    if ((HIWORD(lDecay) < HIWORD(pctDecay)))
        goto L_1e5d;
    else
        goto L_1e54;

L_1e54:
    if ((LOWORD(lDecay) >= LOWORD(pctDecay)))
        goto L_1e6d;
    else
        goto L_1e5d;

L_1e5d:
    lDecay = pctDecay;

L_1e6d:
    if ((lpth->thm.iType == 0x2))
        goto L_1eb6;
    else
        goto L_1e80;

L_1e80:
    if ((0x0 < HIWORD(lDecay)))
        goto L_1ea6;
    else
        goto L_1e8f;

L_1e8f:
    if ((0x0 > HIWORD(lDecay)))
        goto L_1e9d;
    else
        goto L_1e94;

L_1e94:
    if ((0xa <= LOWORD(lDecay)))
        goto L_1ea6;
    else
        goto L_1e9d;

L_1e9d:
    t_merge_1eae_0001_wide = 0xa;
    goto L_1eae;

L_1ea6:
    t_merge_1eae_0001_wide = lDecay;

L_1eae:
    lDecay = t_merge_1eae_0001_wide;

L_1eb6:
    CchGetString(idsDecayRateLdYear, szT);
    c = _wsprintf(szWork, &(szT), LOWORD(lDecay), HIWORD(lDecay));
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));
    if ((lpth->iplr != idPlayer))
        goto FinishUp;
    else
        goto L_1f27;

L_1f27:
    GetMineFieldCounts(LOWORD(lpth), &(i), &(c2));
    CchGetString(idsFieldDD, szT);
    c = _wsprintf(szWork, &(szT), i, c2);
    TextOut(hdc, xLeft, yTop, szWork, c);
    yTop = (yTop + (dyArial8 + 0x2));

L_1f96:
    if ((sel.scan.idpl == 0xffff))
        goto L_36ef;
    else
        goto L_1fa0;

L_1fa0:
    if ((FLookupPlanet(sel.scan.idpl, &(pl)) == 0x0))
        goto L_36ef;
    else
        goto L_1fb8;

L_1fb8:
    fShortLabels = 0x0;
    plrSav = rgplr[idPlayer];
    rc = *(prc);
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raTerra))
        goto L_2106;
    else
        goto L_200d;

L_200d:
    if ((pl.iPlayer == 0xffff))
        goto L_2106;
    else
        goto L_2016;

L_2016:
    if ((idPlayer == pl.iPlayer))
        goto L_2043;
    else
        goto L_2022;

L_2022:
    if (((uint16_t)(rgplr[idPlayer].rgmdRelation[pl.iPlayer]) != 0x1))
        goto L_2106;
    else
        goto L_2043;

L_2043:
    c = 0x0;
    goto L_20fc;

L_204c:
    /* untranslated: rgplr[idPlayer].rgEnvVar[c] = lobyte(setlobyte(c, rgplr[pl.iPlayer].rgEnvVar[c])) */
    /* untranslated: rgplr[idPlayer].rgEnvVarMin[c] = lobyte(setlobyte(c, rgplr[pl.iPlayer].rgEnvVarMin[c])) */
    /* untranslated: rgplr[idPlayer].rgEnvVarMax[c] = lobyte(setlobyte(c, rgplr[pl.iPlayer].rgEnvVarMax[c])) */
    c = (c + 0x1);

L_20fc:
    if ((c < 0x3))
        goto L_204c;
    else
        goto L_2106;

L_2106:
    SelectObject(hdc, LOWORD(rghfontArial8));
    dxRLabels = (LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN999mr), 0x5)) + 0x6);
    SelectObject(hdc, HIWORD(rghfontArial8));
    dxLabels = (LOWORD(GetTextExtent(hdc, HIWORD(rgszPlanetAttr), strlen(HIWORD(rgszPlanetAttr)))) + 0x6);
    if (((dxLabels * 0x4) <= rc.right))
        goto L_2195;
    else
        goto L_2175;

L_2175:
    fShortLabels = 0x1;
    dxLabels = (LOWORD(GetTextExtent(hdc, HIWORD(rgszPlanetAttr), 0x4)) + 0x6);

L_2195:
    xL = (rc.left + dxLabels);
    xR = (rc.right - dxRLabels);
    dyRow = ((uint32_t)((((rc.bottom - rc.top) - (dyArial8 * 0x4)) + 0xfffe)) / 0x6);
    dyRow = ((dyRow + 0x1) & 0xfffe);
    yCur = (rc.top + 0x2);
    if ((pl.fStarbase == 0x0))
        goto L_230c;
    else
        goto L_2201;

L_2201:
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpMono);
    crTextSav = SetTextColor(hdc, 0x0);
    crBkSav = SetBkColor(hdc, 0xffffff);
    BitBlt(hdc, (rc.right + 0xffea), (yCur + 0x2), 0xd, 0x10, hdcMem, 0x0, 0xc, SRCAND);
    SetTextColor(hdc, 0xffff);
    SetBkColor(hdc, 0x0);
    BitBlt(hdc, (rc.right + 0xffea), (yCur + 0x2), 0xd, 0x10, hdcMem, 0x0, 0xc, SRCPAINT);
    SetTextColor(hdc, crTextSav);
    SetBkColor(hdc, crBkSav);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);

L_230c:
    if ((pl.det < 0x3))
        goto L_24ab;
    else
        goto L_231a;

L_231a:
    if ((fShortLabels == 0x0))
        goto L_232e;
    else
        goto L_2328;

L_2328:
    t_merge_2331_0001 = 0x0;
    goto L_2331;

L_232e:
    t_merge_2331_0001 = 0x1;

L_2331:
    /* untranslated: c = CchGetString(part[8:0](szBrowser[t_merge_2331_0001]), 0x57a4) */
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    SetTextColor(hdc, crButtonText);
    TextOut(hdc, xL, yCur, szWork, c);
    dNum = PctPlanetDesirability(&(pl), idPlayer);
    c = _wsprintf(szWork, PCTDPCTPCT, dNum);
    if ((dNum >= 0x0))
        goto L_23ce;
    else
        goto L_23c5;

L_23c5:
    t_merge_23d4_0001 = 0xff;
    goto L_23d4;

L_23ce:
    t_merge_23d4_0001 = 0x7f00;

L_23d4:
    SetTextColor(hdc, t_merge_23d4_0001);
    TextOut(hdc, (xL + dx), yCur, szWork, c);
    if ((fShortLabels != 0x0))
        goto L_24ab;
    else
        goto L_2405;

L_2405:
    dx = (dx + LOWORD(GetTextExtent(hdc, szWork, c)));
    dBest = PctPlanetOptValue(&(pl), idPlayer);
    if ((dBest <= dNum))
        goto L_24ab;
    else
        goto L_2440;

L_2440:
    if ((dBest <= 0x0))
        goto L_246d;
    else
        goto L_244a;

L_244a:
    if ((dBest > 0xa))
        goto L_2460;
    else
        goto L_2457;

L_2457:
    t_merge_2466_0001 = 0x7f7f;
    goto L_2466;

L_2460:
    t_merge_2466_0001 = 0x7f00;

L_2466:
    SetTextColor(hdc, t_merge_2466_0001);

L_246d:
    c = _wsprintf(szWork, " (%d%%)", dBest);
    TextOut(hdc, (xL + dx), yCur, szWork, c);

L_24ab:
    SetTextColor(hdc, crButtonText);
    if ((pl.iPlayer == 0xffff))
        goto L_24f1;
    else
        goto L_24c4;

L_24c4:
    if ((fShortLabels == 0x0))
        goto L_24d4;
    else
        goto L_24ce;

L_24ce:
    t_merge_24d7_0001 = 0x0;
    goto L_24d7;

L_24d4:
    t_merge_24d7_0001 = 0x1;

L_24d7:
    /* untranslated: call strcpy(szT, PszGetCompressedString(part[6:0](szBrowser[t_merge_24d7_0001]))) -> callresult(char *) */

L_24f1:
    if ((pl.det != 0x7))
        goto L_255f;
    else
        goto L_24ff;

L_24ff:
    c = strlen(szT);
    /* untranslated: c = (c + CommaFormatLong(szT[c], (uint32_t)(words(part[42:2](pl), part[40:2](pl)) * 0x64))) */
    RightTextOut(hdc, xR, yCur, szT, c, 0x0);
    goto L_26c2;

L_255f:
    if ((pl.iPlayer != 0xffff))
        goto L_25a0;
    else
        goto L_2568;

L_2568:
    c = CchGetString(idsUninhabited, szT);
    RightTextOut(hdc, xR, yCur, szT, c, 0x0);
    goto L_26c2;

L_25a0:
    if ((pl.det < 0x3))
        goto L_266f;
    else
        goto L_25ae;

L_25ae:
    /* untranslated: l = (int32_t)(words(pl.uPopGuess, 0x0) * 0x4) */
    strcpy(0x57a4, szT);
    c = strlen(szT);
    if ((HIWORD(l) < 0x0))
        goto L_2638;
    else
        goto L_25f3;

L_25f3:
    if ((HIWORD(l) > 0x0))
        goto L_2602;
    else
        goto L_25f8;

L_25f8:
    if ((LOWORD(l) <= 0x0))
        goto L_2638;
    else
        goto L_2602;

L_2602:
    c = (c + _wsprintf(&(szWork[c]), PszGetCompressedString(idsCLd00), 0xb1, LOWORD(l), HIWORD(l)));
    goto L_2650;

L_2638:
    c = (c + CchGetString(idsMsg1264, szWork[c]));

L_2650:
    RightTextOut(hdc, xR, yCur, 0x57a4, c, 0x0);

L_266f:
    SetTextColor(hdc, 0xff);
    RightTextOut(hdc, xR, ((yCur + dyArial8) + 0xfffe), PszPlayerName(pl.iPlayer, 0x0, 0x1, 0x0, 0x0, 0x0), 0x0, 0x0);

L_26c2:
    yCur = (yCur + (dyArial8 + 0xfffe));
    dNum = (game.turn - pl.turn);
    if ((dNum != 0x0))
        goto L_2719;
    else
        goto L_26e3;

L_26e3:
    if ((fShortLabels == 0x0))
        goto L_26f3;
    else
        goto L_26ed;

L_26ed:
    t_merge_26f6_0001 = 0x1bc;
    goto L_26f6;

L_26f3:
    t_merge_26f6_0001 = 0x22b;

L_26f6:
    c = _wsprintf(szWork, PszGetCompressedString(t_merge_26f6_0001));
    goto L_27ab;

L_2719:
    if ((fShortLabels == 0x0))
        goto L_274d;
    else
        goto L_2723;

L_2723:
    c = _wsprintf(szWork, PszGetCompressedString(idsOld2), dNum);
    goto L_27ab;

L_274d:
    c = _wsprintf(szWork, PszGetCompressedString(idsReportDYear), dNum);
    if ((dNum <= 0x1))
        goto L_2793;
    else
        goto L_277e;

L_277e:
    strcat(0x57a4, 0x502);
    c = (c + 0x1);

L_2793:
    c = (c + CchGetString(idsOld, szWork[c]));

L_27ab:
    if ((dNum > 0x5))
        goto L_27c1;
    else
        goto L_27b8;

L_27b8:
    t_merge_27c7_0001 = 0x0;
    goto L_27c7;

L_27c1:
    t_merge_27c7_0001 = 0xff;

L_27c7:
    SetTextColor(hdc, t_merge_27c7_0001);
    TextOut(hdc, xL, yCur, szWork, c);
    yCur = (yCur + dyArial8);
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xL, yCur, (xR - xL), 0x1, PATCOPY);
    PatBlt(hdc, (xL + 0x1), (yCur + 0x1), ((xR - xL) + 0xfffe), (LOWORD((0x3 * dyRow)) + 0xffff), BLACKNESS);
    PatBlt(hdc, xL, yCur, 0x1, LOWORD((0x3 * dyRow)), PATCOPY);
    PatBlt(hdc, (xL + 0x2), (yCur + dyRow), ((xR - xL) + 0xfffc), 0x1, PATCOPY);
    PatBlt(hdc, (xL + 0x2), ((dyRow * 0x2) + yCur), ((xR - xL) + 0xfffc), 0x1, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xR + 0xffff), (yCur + 0x1), 0x1, LOWORD((0x3 * dyRow)), PATCOPY);
    PatBlt(hdc, xL, (LOWORD((0x3 * dyRow)) + yCur), (xR - xL), 0x1, PATCOPY);
    SetTextColor(hdc, crButtonText);
    SetBkMode(hdc, TRANSPARENT);
    fCanTerraform = FCanTerraformLppl(&(pl), rgMin, rgMax, rgCost, 0x1);
    dx = ((xR - xL) + 0xfffc);
    dy = ((dyRow - dyArial8) >> 0x1);
    i = 0x0;
    goto L_29c2;

L_29af:
    i = (i + 0x1);
    yCur = (yCur + dyRow);

L_29c2:
    if ((i >= 0x3))
        goto L_2e1a;
    else
        goto L_29cb;

L_29cb:
    iMin = (uint16_t)(rgplr[idPlayer].rgEnvVarMin[i]);
    iMax = (uint16_t)(rgplr[idPlayer].rgEnvVarMax[i]);
    iCur = (uint16_t)(pl.rgEnvVar[i]);
    if ((fShortLabels == 0x0))
        goto L_2a32;
    else
        goto L_2a26;

L_2a26:
    t_merge_2a3b_0001 = rgszPlanetAttrAbbr[i];
    goto L_2a3b;

L_2a32:
    t_merge_2a3b_0001 = rgszPlanetAttr[i];

L_2a3b:
    RightTextOut(hdc, (xL + 0xfffe), (yCur + dy), t_merge_2a3b_0001, 0x0, 0x0);
    if ((pl.det < 0x3))
        goto L_2ab7;
    else
        goto L_2a66;

L_2a66:
    psz = PszCalcEnvVar(i, iCur);
    c = strlen(psz);
    SelectObject(hdc, LOWORD(rghfontArial8));
    TextOut(hdc, (xR + 0x2), (yCur + dy), psz, c);

L_2ab7:
    SelectObject(hdc, HIWORD(rghfontArial8));
    SelectObject(hdc, rghbrPlanetAttr[i][0x0]);
    PatBlt(hdc, ((xL + 0x2) + MulDiv(iMin, dx, 0x64)), (yCur + 0x2), MulDiv((iMax - iMin), dx, 0x64), (dyRow + 0xfffd), PATCOPY);
    if ((pl.det < 0x3))
        goto L_29af;
    else
        goto L_2b36;

L_2b36:
    SelectObject(hdc, rghbrPlanetAttr[i][0x1]);
    pt.x = ((xL + 0x2) + MulDiv((uint16_t)(pl.rgEnvVarOrig[i]), dx, 0x64));
    pt.y = (((uint32_t)(dyRow) / 0x2) + yCur);
    c = (((uint32_t)(dyRow) / 0x4) + 0xffff);
    if ((c >= 0x2))
        goto L_2ba6;
    else
        goto L_2ba0;

L_2ba0:
    c = 0x2;

L_2ba6:
    /* untranslated: call PatBlt(hdc, (pt.x - c), pt.y, part[1:0](mpicolgrbitBU[c]), 0x1, PATCOPY) -> callresult(int16_t) */
    /* untranslated: call PatBlt(hdc, pt.x, (pt.y - c), 0x1, part[1:0](mpicolgrbitBU[c]), PATCOPY) -> callresult(int16_t) */
    yTop = (yCur + 0x3);
    yBot = ((yCur + dyRow) + 0xfffd);
    c = 0x1;
    xBeg = ((xL + 0x2) + MulDiv(iCur, dx, 0x64));

L_2c3d:
    PatBlt(hdc, xBeg, yTop, 0x1, 0x1, PATCOPY);
    t_2c6c = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, ((xBeg + c) + 0xffff), t_2c6c, 0x1, 0x1, PATCOPY);
    if ((yTop > yBot))
        goto L_2cf6;
    else
        goto L_2c95;

L_2c95:

L_2c9b:
    PatBlt(hdc, xBeg, yBot, 0x1, 0x1, PATCOPY);
    t_2cca = yBot;
    yBot = (yBot - 0x1);
    PatBlt(hdc, ((xBeg + c) + 0xffff), t_2cca, 0x1, 0x1, PATCOPY);
    xBeg = (xBeg - 0x1);
    c = (c + 0x2);

L_2cf6:
    if ((fCanTerraform == 0x0))
        goto L_29af;
    else
        goto L_2d00;

L_2d00:
    yTop = (yTop - 0x1);
    if ((rgMin[i] == 0xffff))
        goto L_2d51;
    else
        goto L_2d18;

L_2d18:
    if ((0x0 <= (iCur - rgMin[i])))
        goto L_2d39;
    else
        goto L_2d33;

L_2d33:
    t_merge_2d4a_0001 = 0x0;
    goto L_2d4a;

L_2d39:
    t_merge_2d4a_0001 = (iCur - rgMin[i]);

L_2d4a:
    dxBar = t_merge_2d4a_0001;
    goto L_2d57;

L_2d51:
    dxBar = 0x0;

L_2d57:
    xBeg = ((xL + 0x2) + MulDiv((iCur - dxBar), dx, 0x64));
    if ((rgMax[i] == 0xffff))
        goto L_2dc6;
    else
        goto L_2d8d;

L_2d8d:
    if ((0x0 <= (rgMax[i] - iCur)))
        goto L_2dae;
    else
        goto L_2da8;

L_2da8:
    t_merge_2dbf_0001 = 0x0;
    goto L_2dbf;

L_2dae:
    t_merge_2dbf_0001 = (rgMax[i] - iCur);

L_2dbf:
    dxBar = t_merge_2dbf_0001;
    goto L_2dcc;

L_2dc6:
    dxBar = 0x0;

L_2dcc:
    xEnd = ((xL + 0x2) + MulDiv((iCur + dxBar), dx, 0x64));
    PatBlt(hdc, xBeg, yTop, ((xEnd - xBeg) + 0x1), 0x1, PATCOPY);

L_2e1a:
    yCur = (yCur + (dyArial8 >> 0x1));
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xL, yCur, (xR - xL), 0x1, PATCOPY);
    PatBlt(hdc, (xL + 0x1), (yCur + 0x1), ((xR - xL) + 0xfffe), (LOWORD((0x3 * dyRow)) + 0x1), BLACKNESS);
    PatBlt(hdc, xL, yCur, 0x1, (LOWORD((0x3 * dyRow)) + 0x2), PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (xR + 0xffff), (yCur + 0x1), 0x1, (LOWORD((0x3 * dyRow)) + 0x2), PATCOPY);
    PatBlt(hdc, xL, ((LOWORD((0x3 * dyRow)) + yCur) + 0x2), (xR - xL), 0x1, PATCOPY);
    SelectObject(hdc, rghfontArial7);
    c = _wsprintf(szWork, PCTD, cMinGrafMax);
    dxNum = LOWORD(GetTextExtent(hdc, szWork, c));
    dxBar = ((xR - xL) + 0xfffd);
    cNum = ((uint32_t)(dxBar) / ((dxNum >> 0x1) + dxNum));
    dNum = ((uint32_t)(cMinGrafMax) / cNum);
    if ((cMinGrafMax >= 0x1f4))
        goto L_2fb4;
    else
        goto L_2f9b;

L_2f9b:
    dNum = LOWORD((((uint32_t)((dNum + 0x31)) / 0x32) * 0xa));
    goto L_305a;

L_2fb4:
    if ((cMinGrafMax >= 0x3e8))
        goto L_2fd8;
    else
        goto L_2fbf;

L_2fbf:
    dNum = LOWORD((((uint32_t)((dNum + 0x31)) / 0x32) * 0x32));
    goto L_305a;

L_2fd8:
    if ((cMinGrafMax >= 0x9c4))
        goto L_2ffc;
    else
        goto L_2fe3;

L_2fe3:
    dNum = LOWORD((((uint32_t)((dNum + 0x63)) / 0x64) * 0x64));
    goto L_305a;

L_2ffc:
    if ((cMinGrafMax >= 0x1d4c))
        goto L_3020;
    else
        goto L_3007;

L_3007:
    dNum = LOWORD((((uint32_t)((dNum + 0xf9)) / 0xfa) * 0xfa));
    goto L_305a;

L_3020:
    if ((cMinGrafMax >= 0x3a98))
        goto L_3044;
    else
        goto L_302b;

L_302b:
    /* untranslated: dNum = loword(((words(part[7:0](szMine[dNum]), signhiword(part[7:0](szMine[dNum]))) / 0x1f4) * 0x1f4)) */
    goto L_305a;

L_3044:
    dNum = LOWORD((((uint32_t)((dNum + 0x3e7)) / 0x3e8) * 0x3e8));

L_305a:
    cNum = ((uint32_t)(cMinGrafMax) / dNum);
    SetTextColor(hdc, crButtonText);
    dy = ((LOWORD((0x3 * dyRow)) + yCur) + 0x4);
    i = 0x0;
    goto L_3131;

L_3090:
    xBeg = (LOWORD((int32_t)(((uint32_t)(((uint32_t)(((uint32_t)(i) * (uint32_t)(dNum))) * (uint32_t)(dxBar))) / (uint32_t)(cMinGrafMax)))) + xL);
    PatBlt(hdc, xBeg, (yCur + 0x2), 0x1, (LOWORD((0x3 * dyRow)) + 0xffff), PATCOPY);
    c = _wsprintf(szWork, PCTD, LOWORD((i * dNum)));
    CtrTextOut(hdc, xBeg, dy, 0x57a4, c);
    i = (i + 0x1);

L_3131:
    if ((i <= cNum))
        goto L_3090;
    else
        goto L_313d;

L_313d:
    RightTextOut(hdc, (xL + 0xfffc), dy, 0x504, 0x2, 0x0);
    if ((pl.iPlayer != idPlayer))
        goto L_318d;
    else
        goto L_316b;

L_316b:
    EstMineralsMined(&(pl), rgl, 0xffffffff, 0x0);
    goto L_32da;

L_318d:
    /* untranslated: part[8:2](rgl) = 0x0 */
    /* untranslated: part[10:2](rgl) = 0x0 */
    /* untranslated: part[4:2](rgl) = 0x0 */
    /* untranslated: part[6:2](rgl) = 0x0 */
    LOWORD(rgl) = 0x0;
    HIWORD(rgl) = 0x0;
    if ((pl.iPlayer != 0xffff))
        goto L_32da;
    else
        goto L_31b2;

L_31b2:
    ifl = 0x0;
    goto L_31c0;

L_31bb:
    ifl = (ifl + 0x1);

L_31c0:
    if ((ifl >= cFleet))
        goto L_32da;
    else
        goto L_31cc;

L_31cc:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_31fd;
    else
        goto L_31f5;

L_31f5:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_32da;
    else
        goto L_31fd;

L_31fd:
    if ((lpfl->idPlanet != pl.id))
        goto L_31bb;
    else
        goto L_320c;

L_320c:
    if ((lpfl->iPlayer != idPlayer))
        goto L_31bb;
    else
        goto L_321b;

L_321b:
    if ((lpfl->fDead != 0x0))
        goto L_31bb;
    else
        goto L_3232;

L_3232:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x3 ? L_31bb : L_3245 */

L_3245:

L_324b:
    cMines = CMineFromLpfl(lpfl);
    if ((HIWORD(cMines) < 0x0))
        goto L_31bb;
    else
        goto L_326b;

L_326b:
    if ((HIWORD(cMines) > 0x0))
        goto L_327a;
    else
        goto L_3270;

L_3270:
    if ((LOWORD(cMines) <= 0x0))
        goto L_31bb;
    else
        goto L_327a;

L_327a:
    EstMineralsMined(&(pl), rglT, cMines, 0x0);
    iT = 0x0;
    goto L_32cd;

L_32a3:
    rgl[iT] = (rgl[iT] + rglT[iT]);
    iT = (iT + 0x1);

L_32cd:
    if ((iT >= 0x3))
        goto L_31bb;
    else
        goto L_32d4;

L_32d4:

L_32da:
    SelectObject(hdc, HIWORD(rghfontArial8));
    dy = (((dyRow - dyArial8) >> 0x1) + 0x2);
    i = 0x0;
    goto L_3312;

L_32ff:
    i = (i + 0x1);
    yCur = (yCur + dyRow);

L_3312:
    if ((i > 0x2))
        goto L_36ab;
    else
        goto L_331b;

L_331b:
    SetTextColor(hdc, rgcrMinerals[i]);
    if ((fShortLabels == 0x0))
        goto L_3346;
    else
        goto L_3340;

L_3340:
    t_merge_3357_0001 = 0x4;
    goto L_3357;

L_3346:
    t_merge_3357_0001 = strlen(rgszMinerals[i]);

L_3357:
    RightTextOut(hdc, (xL + 0xfffe), (yCur + dy), rgszMinerals[i], t_merge_3357_0001, 0x0);
    if ((SIGNHIWORD(cMinGrafMax) > (HIWORD(rgl[i]) + HIWORD(pl.rgwtMin[i]))))
        goto L_33c3;
    else
        goto L_33b0;

L_33b0:
    if ((SIGNHIWORD(cMinGrafMax) < (HIWORD(rgl[i]) + HIWORD(pl.rgwtMin[i]))))
        goto L_33bc;
    else
        goto L_33b5;

L_33b5:
    if ((cMinGrafMax >= (LOWORD(rgl[i]) + LOWORD(pl.rgwtMin[i]))))
        goto L_33c3;
    else
        goto L_33bc;

L_33bc:
    t_merge_33e5_0001 = (uint32_t)(cMinGrafMax);
    t_merge_33e5_0002 = (uint32_t)(cMinGrafMax);
    goto L_33e5;

L_33c3:
    /* untranslated: t_merge_33e5_0001 = words((HIWORD(rgl[i]) + HIWORD(pl.rgwtMin[i])), (LOWORD(rgl[i]) + LOWORD(pl.rgwtMin[i]))) */
    /* untranslated: t_merge_33e5_0002 = words((HIWORD(rgl[i]) + HIWORD(pl.rgwtMin[i])), (LOWORD(rgl[i]) + LOWORD(pl.rgwtMin[i]))) */

L_33e5:
    /* untranslated: dx = loword((int32_t)(words(hiword((uint32_t)(sext16to32(dxBar) * t_merge_33e5_0001)), loword((uint32_t)(sext16to32(dxBar) *
     * t_merge_33e5_0002))) / sext16to32(cMinGrafMax))) */
    SetRect(&(rcGauge), (xL + 0x1), (yCur + 0x4), ((xL + dx) + 0x1), ((yCur + dyRow) + 0xffff));
    if ((dx == 0x0))
        goto L_345a;
    else
        goto L_343c;

L_343c:
    FillRect(hdc, &(rcGauge), rghbrMinSum[i][0x1]);

L_345a:
    if (((HIWORD(rgl[i]) + HIWORD(pl.rgwtMin[i])) < SIGNHIWORD(cMinGrafMax)))
        goto L_34d3;
    else
        goto L_3497;

L_3497:
    /* untranslated: branch ss:[bp-0x1cc] > signhiword(cMinGrafMax) ? L_34a3 : L_349c */

L_349c:
    /* untranslated: branch ss:[bp-0x1ce] <= cMinGrafMax ? L_34d3 : L_34a3 */

L_34a3:
    SetTextColor(hdc, crButtonText);
    TextOut(hdc, xR, (yCur + dy), "+", 0x1);

L_34d3:
    if ((SIGNHIWORD(cMinGrafMax) > HIWORD(pl.rgwtMin[i])))
        goto L_3508;
    else
        goto L_34f5;

L_34f5:
    if ((SIGNHIWORD(cMinGrafMax) < HIWORD(pl.rgwtMin[i])))
        goto L_3501;
    else
        goto L_34fa;

L_34fa:
    if ((cMinGrafMax >= LOWORD(pl.rgwtMin[i])))
        goto L_3508;
    else
        goto L_3501;

L_3501:
    t_merge_3519_0001 = (uint32_t)(cMinGrafMax);
    t_merge_3519_0002 = (uint32_t)(cMinGrafMax);
    goto L_3519;

L_3508:
    t_merge_3519_0001 = pl.rgwtMin[i];
    t_merge_3519_0002 = pl.rgwtMin[i];

L_3519:
    /* untranslated: dx = loword((int32_t)(words(hiword((uint32_t)(sext16to32(dxBar) * t_merge_3519_0001)), loword((uint32_t)(sext16to32(dxBar) *
     * t_merge_3519_0002))) / sext16to32(cMinGrafMax))) */
    SetRect(&(rcGauge), (xL + 0x1), (yCur + 0x4), ((xL + dx) + 0x1), ((yCur + dyRow) + 0xffff));
    if ((dx == 0x0))
        goto L_358a;
    else
        goto L_3570;

L_3570:
    FillRect(hdc, &(rcGauge), rghbrMinSum[i][0x0]);

L_358a:
    if ((pl.det < 0x3))
        goto L_32ff;
    else
        goto L_3598;

L_3598:
    iPass = 0x0;
    goto L_369e;

L_35a1:
    SelectObject(hdc, rghbrMinSum[i][iPass]);
    yTop = (yCur + 0x4);
    yBot = ((yCur + dyRow) + 0xfffe);
    c = 0x1;
    if ((0x64 >= pl.rgMinConc[i]))
        goto L_35fc;
    else
        goto L_35f6;

L_35f6:
    t_merge_3609_0001 = 0x64;
    goto L_3609;

L_35fc:
    t_merge_3609_0001 = pl.rgMinConc[i];

L_3609:
    xBeg = (((xL + 0x1) + iPass) + MulDiv(t_merge_3609_0001, (dxBar + 0xfff8), 0x64));

L_362f:
    t_3636 = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, xBeg, t_3636, c, 0x1, PATCOPY);
    if ((yTop > yBot))
        goto L_3699;
    else
        goto L_365f;

L_365f:

L_3665:
    t_3668 = xBeg;
    xBeg = (xBeg - 0x1);
    t_3672 = yBot;
    yBot = (yBot - 0x1);
    PatBlt(hdc, t_3668, t_3672, c, 0x1, PATCOPY);
    c = (c + 0x2);

L_3699:
    iPass = (iPass + 0x1);

L_369e:
    if ((iPass >= 0x2))
        goto L_32ff;
    else
        goto L_36a5;

L_36a5:

L_36ab:
    SetBkMode(hdc, OPAQUE);
    SelectObject(hdc, hbrSav);
    rgplr[idPlayer] = plrSav;
    goto FinishUp;

L_36ef:
    if ((sel.scan.idpl == 0xffff))
        goto FinishUp;
    else
        goto L_36f9;

L_36f9:
    hbmpSav = SelectObject(hdcMem, hbmpUnknownPlanet);
    SetTextColor(hdc, 0x0);
    BitBlt(hdc, ((((prc->right - prc->left) + 0xffc0) >> 0x1) + prc->left), ((((prc->bottom - prc->top) + 0xffc0) >> 0x1) + prc->top), 0x40, 0x40, hdcMem, 0x0,
           0x0, SRCCOPY);
    SelectObject(hdcMem, hbmpSav);

FinishUp:
    SetBkMode(hdc, bkMode);
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    DeleteDC(hdcMem);
    return;
}

HtMineType HtMineWindow(HWND hwnd, int16_t x, int16_t y) {
    PLANET     pl;
    int16_t    dyRow;
    int16_t    yCur;
    GrobjClass grobj;
    RECT       rc;

L_37ac:
    grobj = sel.scan.grobj;
    if ((grobj != grobjOther))
        goto L_37f5;
    else
        goto L_37c4;

L_37c4:
    if (((sel.scan.grobjFull & 0x1) == 0x0))
        goto L_37da;
    else
        goto L_37d2;

L_37d2:
    grobj = grobjPlanet;
    goto L_37f5;

L_37da:
    if (((sel.scan.grobjFull & 0x2) == 0x0))
        goto L_37f0;
    else
        goto L_37e8;

L_37e8:
    grobj = grobjFleet;
    goto L_37f5;

L_37f0:
    grobj = grobjNone;

L_37f5:
    GetClientRect(hwnd, &(rc));
    rc.top = (rc.top + (dyArial8 * 0x2));
    if ((y >= (rc.top + 0xfffb)))
        goto L_3852;
    else
        goto L_381a;

L_381a:
    if ((y <= 0x4))
        goto L_3852;
    else
        goto L_3823;

L_3823:
    if ((x >= (rc.right + 0xfffc)))
        goto L_3852;
    else
        goto L_3831;

L_3831:
    if ((x <= (rc.right - rc.top)))
        goto L_3852;
    else
        goto L_383f;

L_383f:
    if ((FOtherStuffAtScanSel() == 0x0))
        goto L_3852;
    else
        goto L_384c;

L_384c:
    return 0x9;

L_3852:
    if ((grobj == grobjFleet))
        goto L_38b0;
    else
        goto L_385b;

L_385b:
    if ((grobj != grobjThing))
        goto L_39e2;
    else
        goto L_3864;

L_3864:
    if ((((LOWORD(lpThings[sel.scan.ith]) >> 0xd) & 0x7) == 0x2))
        goto L_39e2;
    else
        goto L_388a;

L_388a:
    if ((((LOWORD(lpThings[sel.scan.ith]) >> 0xd) & 0x7) == 0x3))
        goto L_39e2;
    else
        goto L_38b0;

L_38b0:
    if ((x < (rc.left + 0x19)))
        goto L_38ee;
    else
        goto L_38be;

L_38be:
    if ((x >= (rc.left + 0x39)))
        goto L_38ee;
    else
        goto L_38cc;

L_38cc:
    if ((y < (rc.top + 0x47)))
        goto L_38ee;
    else
        goto L_38da;

L_38da:
    if ((y >= (rc.top + 0x67)))
        goto L_38ee;
    else
        goto L_38e8;

L_38e8:
    return 0xa;

L_38ee:
    if ((grobj != grobjThing))
        goto L_395b;
    else
        goto L_38f7;

L_38f7:
    if ((((LOWORD(lpThings[sel.scan.ith]) >> 0xd) & 0x7) != 0x0))
        goto L_395b;
    else
        goto L_391d;

L_391d:
    if ((x < (rc.left + 0x9)))
        goto L_395b;
    else
        goto L_392b;

L_392b:
    if ((x >= (rc.left + 0x49)))
        goto L_395b;
    else
        goto L_3939;

L_3939:
    if ((y < (rc.top + 0x9)))
        goto L_395b;
    else
        goto L_3947;

L_3947:
    if ((y >= (rc.top + 0x49)))
        goto L_395b;
    else
        goto L_3955;

L_3955:
    return 0xe;

L_395b:
    if ((grobj != grobjThing))
        goto L_396a;
    else
        goto L_3964;

L_3964:
    return 0x0;

L_396a:
    if ((x < (rc.left + 0x9)))
        goto L_39a8;
    else
        goto L_3978;

L_3978:
    if ((x >= (rc.left + 0x49)))
        goto L_39a8;
    else
        goto L_3986;

L_3986:
    if ((y < (rc.top + 0x9)))
        goto L_39a8;
    else
        goto L_3994;

L_3994:
    if ((y >= (rc.top + 0x49)))
        goto L_39a8;
    else
        goto L_39a2;

L_39a2:
    return 0xb;

L_39a8:
    if ((x < (rc.left + 0x5c)))
        goto L_39dc;
    else
        goto L_39b6;

L_39b6:
    if ((y < (rc.top + 0x4)))
        goto L_39dc;
    else
        goto L_39c4;

L_39c4:
    if ((y >= ((rc.top + 0x7) + dyArial8)))
        goto L_39dc;
    else
        goto L_39d6;

L_39d6:
    return 0xb;

L_39dc:
    return 0x0;

L_39e2:
    if ((sel.scan.idpl == 0xffff))
        goto L_3a04;
    else
        goto L_39ec;

L_39ec:
    if ((FLookupPlanet(sel.scan.idpl, &(pl)) != 0x0))
        goto L_3a0a;
    else
        goto L_3a04;

L_3a04:
    return 0x0;

L_3a0a:
    rc.top = (rc.top - 0x4);
    yCur = (rc.top + 0x2);
    dyRow = ((uint32_t)((((rc.bottom - rc.top) - (dyArial8 * 0x4)) + 0xfffe)) / 0x6);
    dyRow = ((dyRow + 0x1) & 0xfffe);
    yCur = (yCur + ((dyArial8 * 0x2) + 0xfffe));
    if ((y >= yCur))
        goto L_3ae6;
    else
        goto L_3a5c;

L_3a5c:
    if ((y <= rc.top))
        goto L_3ae0;
    else
        goto L_3a67;

L_3a67:
    if ((x >= (rc.right + 0xffe8)))
        goto L_3abe;
    else
        goto L_3a75;

L_3a75:
    if ((pl.iPlayer == idPlayer))
        goto L_3ab5;
    else
        goto L_3a80;

L_3a80:
    /* untranslated: branch x <= (words(loword((0x3 * rc.right)), signhiword(loword((0x3 * rc.right)))) / 0x5) ? L_3ab5 : L_3a94 */

L_3a94:
    if ((y < (yCur - dyArial8)))
        goto L_3ab5;
    else
        goto L_3aa3;

L_3aa3:
    if ((pl.iPlayer == 0xffff))
        goto L_3ab5;
    else
        goto L_3aac;

L_3aac:
    return 0xa;

L_3ab5:
    return 0xc;

L_3abe:
    if ((pl.fStarbase == 0x0))
        goto L_3ad7;
    else
        goto L_3ad1;

L_3ad1:
    return 0xd;

L_3ad7:
    return 0x0;

L_3ae0:
    return 0x0;

L_3ae6:
    if ((y >= (LOWORD((0x3 * dyRow)) + yCur)))
        goto L_3b07;
    else
        goto L_3af7;

L_3af7:
    return (((uint32_t)((y - yCur)) / dyRow) + 0x6);

L_3b07:
    yCur = (yCur + ((LOWORD((0x3 * dyRow)) + (dyArial8 >> 0x1)) + 0x1));
    if ((y >= (LOWORD((0x3 * dyRow)) + yCur)))
        goto L_3b41;
    else
        goto L_3b31;

L_3b31:
    return (((uint32_t)((y - yCur)) / dyRow) + 0x1);

L_3b41:
    return 0x5;
}

void MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks) {
    PLANET    *lppl;
    HtMineType ht;
    PART       part;
    char       rgsz[16][32];
    char      *rgpsz[16];
    int16_t    c;
    FLEET     *lpfl;
    int16_t    rgid[16];
    int16_t    ishdef;
    int16_t    rgMin[3];
    PLANET     pl;
    int16_t    rgCost[3];
    int16_t    rgMax[3];
    int16_t    fOurs;
    int16_t    i;
    int16_t    idNew;
    SCAN       scan;
    int16_t    rgi[9];
    int16_t    iChecked;
    char      *psz[9];
    int32_t    rglQuan[3];
    int32_t    rglT[3];
    int16_t    ifl;
    int32_t    cMines;
    int32_t    lVal;
    uint16_t   t_merge_3e20_0001;
    uint16_t   t_merge_3e7b_0001;
    uint16_t   t_merge_3ef6_0001;
    uint16_t   t_merge_41a7_0001;
    uint16_t   t_merge_4272_0001;
    uint16_t   t_merge_42ed_0001;
    uint16_t   t_merge_43d6_0001;

L_3b4e:
    ht = HtMineWindow(hwndMine, x, y);
    if ((msg != 0x204))
        goto L_3b8b;
    else
        goto L_3b76;

L_3b76:
    if ((ht == htMineScanSel))
        goto L_3b8b;
    else
        goto L_3b7f;

L_3b7f:
    if ((ht != htMineShipOrFleet))
        goto L_47d5;
    else
        goto L_3b85;

L_3b85:

L_3b8b:
    goto L_47a6;

L_3b91:
    goto L_47d5;
    if ((sel.scan.grobj != grobjPlanet))
        goto L_3bbd;
    else
        goto L_3b9e;

L_3b9e:
    lppl = LpplFromId(sel.scan.idpl);
    HIWORD(GlobalPD) = lppl->iPlayer;
    goto L_3c0f;

L_3bbd:
    if ((sel.scan.grobj != grobjThing))
        goto L_3beb;
    else
        goto L_3bc7;

L_3bc7:
    HIWORD(GlobalPD) = ((LOWORD(lpThings[sel.scan.ith]) >> 0x9) & 0xf);
    goto L_3c0f;

L_3beb:
    HIWORD(GlobalPD) = ((LOWORD(rglpfl[sel.scan.ifl]) >> 0x9) & 0xf);

L_3c0f:
    GlobalPD.grPopup = grPopupPlayer;
    Popup(hwndMine, x, y);
    goto L_47d5;
    /* untranslated: HIWORD(part) = ((HIWORD(part) & 0xff00) | (part[1290:1](part[12:1](lpThings[sel.scan.ith])) & 0xff)) */
    part.hs.grhst = hstMines;
    FLookupPart(&(part));
    GlobalPD.grPopup = grPopupComponent;
    GlobalPD.part = part;
    Popup(hwndMine, x, y);
    goto L_47d5;
    if ((msg != 0x204))
        goto L_3e38;
    else
        goto L_3ca9;

L_3ca9:
    lpfl = rglpfl[sel.scan.ifl];
    c = 0x0;
    ishdef = 0x0;
    goto L_3d7c;

L_3cda:
    if ((lpfl->rgcsh[ishdef] <= 0x0))
        goto L_3d77;
    else
        goto L_3cfa;

L_3cfa:
    rgid[c] = ishdef;
    /* untranslated: call fstrcpy(&rgsz[c], &part[8:0](rglpshdef[lpfl->iPlayer][ishdef].hul)) -> callresult(char *) */
    rgpsz[c] = rgsz[c];
    c = (c + 0x1);

L_3d77:
    ishdef = (ishdef + 0x1);

L_3d7c:
    if ((ishdef < 0x10))
        goto L_3cda;
    else
        goto L_3d86;

L_3d86:
    if ((c <= 0x1))
        goto L_3dcb;
    else
        goto L_3d90;

L_3d90:
    c = PopupMenu(hwndMine, x, y, c, 0x0, rgpsz, 0xffff, 0x1);
    if ((c == 0xffff))
        goto L_47d5;
    else
        goto L_3dc2;

L_3dc2:

L_3dcb:
    c = 0x0;

L_3dd1:
    GlobalPD.grPopup = grPopupShdef;
    HIWORD(GlobalPD) = (LOWORD(rglpshdef[lpfl->iPlayer]) + LOWORD((0x93 * rgid[c])));
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rglpshdef[lpfl->iPlayer]) */
    if ((idPlayer == lpfl->iPlayer))
        goto L_3e1d;
    else
        goto L_3e17;

L_3e17:
    t_merge_3e20_0001 = 0x1;
    goto L_3e20;

L_3e1d:
    t_merge_3e20_0001 = 0x0;

L_3e20:
    /* untranslated: part[8:2](GlobalPD) = t_merge_3e20_0001 */
    /* untranslated: part[6:2](GlobalPD) = 0x0 */
    /* untranslated: part[10:2](GlobalPD) = 0x0 */
    /* untranslated: part[12:2](GlobalPD) = 0x1 */
    goto L_3e84;

L_3e38:
    GlobalPD.grPopup = grPopupFleet;
    HIWORD(GlobalPD) = LOWORD(rglpfl[sel.scan.ifl]);
    /* untranslated: part[4:2](GlobalPD) = *(rglpfl[sel.scan.ifl]+0x2) */
    /* untranslated: branch (part[4:2](part[2:4](GlobalPD)) & 0xff) != 0x7 ? L_3e78 : L_3e72 */

L_3e72:
    t_merge_3e7b_0001 = 0x1;
    goto L_3e7b;

L_3e78:
    t_merge_3e7b_0001 = 0x0;

L_3e7b:
    /* untranslated: part[6:2](GlobalPD) = t_merge_3e7b_0001 */
    /* untranslated: part[10:2](GlobalPD) = 0xff */

L_3e84:
    Popup(hwndMine, x, y);
    goto L_47d5;
    GlobalPD.grPopup = grPopupShdef;
    lppl = LpplFromId(sel.scan.idpl);
    HIWORD(GlobalPD) = (LOWORD(rglpshdefSB[lppl->iPlayer]) + LOWORD((lppl->isb * 0x93)));
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rglpshdefSB[lppl->iPlayer]) */
    if ((idPlayer == lppl->iPlayer))
        goto L_3ef3;
    else
        goto L_3eed;

L_3eed:
    t_merge_3ef6_0001 = 0x1;
    goto L_3ef6;

L_3ef3:
    t_merge_3ef6_0001 = 0x0;

L_3ef6:
    /* untranslated: part[8:2](GlobalPD) = t_merge_3ef6_0001 */
    /* untranslated: part[6:2](GlobalPD) = 0x1 */
    /* untranslated: part[10:2](GlobalPD) = 0x0 */
    /* untranslated: part[12:2](GlobalPD) = 0x1 */
    Popup(hwndMine, x, y);
    goto L_47d5;
    GlobalPD.grPopup = grPopupPlanet;
    HIWORD(GlobalPD) = sel.scan.idpl;
    Popup(hwndMine, x, y);
    goto L_47d5;
    FLookupPlanet(sel.scan.idpl, &(pl));
    GlobalPD.grPopup = grPopupPlanetEnv;
    HIWORD(GlobalPD) = pl.id;
    /* untranslated: part[4:2](GlobalPD) = (ht + 0xfffa) */
    if ((pl.det < 0x3))
        goto L_3f85;
    else
        goto L_3f74;

L_3f74:
    /* untranslated: part[6:2](GlobalPD) = sext8to16(pl.rgEnvVar[part[4:2](GlobalPD)]) */
    goto L_3f8b;

L_3f85:
    /* untranslated: part[6:2](GlobalPD) = 0xffff */

L_3f8b:
    if ((pl.det < 0x3))
        goto NoTerra;
    else
        goto L_3f99;

L_3f99:
    if ((FCanTerraformLppl(&(pl), rgMin, rgMax, rgCost, 0x1) == 0x0))
        goto NoTerra;
    else
        goto L_3fc0;

L_3fc0:
    /* untranslated: part[8:2](GlobalPD) = rgMin[part[4:2](GlobalPD)] */
    /* untranslated: part[10:2](GlobalPD) = rgMax[part[4:2](GlobalPD)] */
    /* untranslated: branch part[8:2](GlobalPD) != 0xffff ? L_3fee : L_3fe8 */

L_3fe8:
    /* untranslated: part[8:2](GlobalPD) = part[6:2](GlobalPD) */

L_3fee:
    /* untranslated: branch part[10:2](GlobalPD) != 0xffff ? L_3ffe : L_3ff8 */

L_3ff8:
    /* untranslated: part[10:2](GlobalPD) = part[6:2](GlobalPD) */

L_3ffe:
    /* untranslated: branch part[8:2](GlobalPD) == part[10:2](GlobalPD) ? L_4010 : L_4007 */

L_4007:

NoTerra:
    /* untranslated: part[8:2](GlobalPD) = 0xffff */
    /* untranslated: part[10:2](GlobalPD) = 0xffff */

L_401c:
    /* untranslated: part[12:2](GlobalPD) = sext8to16(rgplr[idPlayer].rgEnvVar[part[4:2](GlobalPD)]) */
    /* untranslated: part[14:2](GlobalPD) = sext8to16(rgplr[idPlayer].rgEnvVarMin[part[4:2](GlobalPD)]) */
    /* untranslated: GlobalPD.iPlrMax = sext8to16(rgplr[idPlayer].rgEnvVarMax[part[4:2](GlobalPD)]) */
    Popup(hwndMine, x, y);
    goto L_47d5;
    if ((msg != 0x204))
        goto L_40a4;
    else
        goto L_408f;

L_408f:
    PopupMineralScanChoices(hwndMine, x, y);
    goto L_4439;

L_40a4:
    scan = sel.scan;
    scan.iwp = 0x0;
    if ((scan.grobj != grobjThing))
        goto L_4192;
    else
        goto L_40c2;

L_40c2:
    i = (scan.ith + 0x1);

L_40ce:
    /* untranslated: branch lpThings[i].pt.x != ss:[scan] ? L_410f : L_4103 */

L_4103:
    /* untranslated: branch lpThings[i].pt.y == ss:[bp-0x28] ? L_411e : L_4109 */

L_4109:

L_410f:
    i = (i + 0x1);

L_4113:
    if ((i < cThing))
        goto L_40ce;
    else
        goto L_411e;

L_411e:
    if ((i >= cThing))
        goto L_4154;
    else
        goto L_4129;

L_4129:
    scan.ith = i;
    scan.grobj = grobjThing;
    idNew = LOWORD(lpThings[i]);
    fOurs = 0x0;
    goto ChangeIt;

L_4154:
    if (((scan.grobjFull & 0x1) != 0x0))
        goto CheckPlanet;
    else
        goto L_415f;

L_415f:

L_4168:
    if (((scan.grobjFull & 0x2) != 0x0))
        goto CheckFleet;
    else
        goto L_4173;

L_4173:

L_417c:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4192;
    else
        goto L_418a;

L_418a:
    i = 0x0;
    goto L_4113;

L_4192:
    if ((scan.grobj != grobjFleet))
        goto L_41a4;
    else
        goto L_419b;

L_419b:
    t_merge_41a7_0001 = (scan.ifl + 0x1);
    goto L_41a7;

L_41a4:
    t_merge_41a7_0001 = 0x0;

L_41a7:
    i = t_merge_41a7_0001;
    goto L_41b1;

L_41ad:
    i = (i + 0x1);

L_41b1:
    if ((i >= cFleet))
        goto L_4215;
    else
        goto L_41bc;

L_41bc:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_41ec;
    else
        goto L_41e4;

L_41e4:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_4215;
    else
        goto L_41ec;

L_41ec:
    /* untranslated: branch ss:[scan] != lpfl->pt.x ? L_41ad : L_4206 */

L_4206:
    /* untranslated: branch ss:[scan+0x2] == lpfl->pt.y ? L_4215 : L_420c */

L_420c:

L_4215:
    if ((i >= cFleet))
        goto L_4278;
    else
        goto L_4220;

L_4220:
    scan.ifl = i;
    scan.grobj = grobjFleet;
    idNew = LOWORD(rglpfl[i]);
    if ((rglpfl[i]->iPlayer != idPlayer))
        goto L_426f;
    else
        goto L_4269;

L_4269:
    t_merge_4272_0001 = 0x1;
    goto L_4272;

L_426f:
    t_merge_4272_0001 = 0x0;

L_4272:
    fOurs = t_merge_4272_0001;
    goto ChangeIt;

L_4278:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4291;
    else
        goto L_4286;

L_4286:
    i = 0x0;
    goto L_4113;

L_4291:
    if (((scan.grobjFull & 0x1) == 0x0))
        goto CheckFleet;
    else
        goto CheckPlanet;

CheckPlanet:
    scan.grobj = grobjPlanet;
    idNew = scan.idpl;
    lppl = LpplFromId(idNew);
    if ((LOWORD(lppl) != 0x0))
        goto L_42d5;
    else
        goto L_42c4;

L_42c4:
    if ((HIWORD(lppl) != 0x0))
        goto L_42d5;
    else
        goto L_42cd;

L_42cd:
    fOurs = 0x0;
    goto ChangeIt;

L_42d5:
    if ((lppl->iPlayer != idPlayer))
        goto L_42ea;
    else
        goto L_42e4;

L_42e4:
    t_merge_42ed_0001 = 0x1;
    goto L_42ed;

L_42ea:
    t_merge_42ed_0001 = 0x0;

L_42ed:
    fOurs = t_merge_42ed_0001;

CheckFleet:
    i = 0x0;
    goto L_42ff;

L_42fb:
    i = (i + 0x1);

L_42ff:
    if ((i >= cFleet))
        goto L_4363;
    else
        goto L_430a;

L_430a:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_433a;
    else
        goto L_4332;

L_4332:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_4363;
    else
        goto L_433a;

L_433a:
    /* untranslated: branch ss:[scan] != lpfl->pt.x ? L_42fb : L_4354 */

L_4354:
    /* untranslated: branch ss:[scan+0x2] == lpfl->pt.y ? L_4363 : L_435a */

L_435a:

L_4363:
    if ((i != cFleet))
        goto L_4384;
    else
        goto L_436e;

L_436e:
    if (((scan.grobjFull & 0x8) == 0x0))
        goto L_4384;
    else
        goto L_437c;

L_437c:
    i = 0x0;
    goto L_4113;

L_4384:
    scan.grobj = grobjFleet;
    scan.ifl = i;
    idNew = LOWORD(rglpfl[i]);
    if ((rglpfl[i]->iPlayer != idPlayer))
        goto L_43d3;
    else
        goto L_43cd;

L_43cd:
    t_merge_43d6_0001 = 0x1;
    goto L_43d6;

L_43d3:
    t_merge_43d6_0001 = 0x0;

L_43d6:
    fOurs = t_merge_43d6_0001;

ChangeIt:
    if ((fOurs == 0x0))
        goto L_43ec;
    else
        goto L_43e2;

L_43e2:
    if ((sel.grobj == grobjFleet))
        goto L_43f2;
    else
        goto L_43ec;

L_43ec:
    scan.iwp = sel.scan.iwp;

L_43f2:
    ChangeScanSel(&(scan), 0x2);
    if ((fOurs == 0x0))
        goto L_4439;
    else
        goto L_440b;

L_440b:
    RedrawScanSel(0x0, 0x0);
    ChangeMainObjSel(scan.grobj, idNew);
    RedrawScanSel(0x0, 0x1);

L_4439:
    goto L_47d5;
    iChecked = 0xffff;
    LOWORD(rgi) = 0x64;
    HIWORD(rgi) = 0x1f4;
    /* untranslated: part[4:2](rgi) = 0x3e8 */
    /* untranslated: part[6:2](rgi) = 0x9c4 */
    /* untranslated: part[8:2](rgi) = 0x1388 */
    /* untranslated: part[10:2](rgi) = 0x1d4c */
    /* untranslated: part[12:2](rgi) = 0x2710 */
    /* untranslated: part[14:2](rgi) = 0x4e20 */
    /* untranslated: part[16:2](rgi) = 0x7530 */
    i = 0x0;
    goto L_44d6;

L_4476:
    _wsprintf(&(rgsz[i]), PCTDKT, rgi[i]);
    psz[i] = rgsz[i];
    if ((rgi[i] != cMinGrafMax))
        goto L_44d2;
    else
        goto L_44cc;

L_44cc:
    iChecked = i;

L_44d2:
    i = (i + 0x1);

L_44d6:
    if ((i < 0x9))
        goto L_4476;
    else
        goto L_44df;

L_44df:
    i = PopupMenu(hwndMine, x, y, 0x9, 0x0, psz, iChecked, 0x1);
    if ((i == 0xffff))
        goto L_456c;
    else
        goto L_4511;

L_4511:
    if ((rgi[i] == cMinGrafMax))
        goto L_456c;
    else
        goto L_4525;

L_4525:
    cMinGrafMax = rgi[i];
    InvalidateRect(hwndMine, 0x0, 0x1);
    if (((grbitScan & 0xf) != 0x1))
        goto L_456c;
    else
        goto L_4557;

L_4557:
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_456c:
    goto L_47d5;
    FLookupPlanet(sel.scan.idpl, &(pl));
    GlobalPD.grPopup = grPopupMineral;
    HIWORD(GlobalPD) = (ht + 0xffff);
    /* untranslated: part[4:2](GlobalPD) = signhiword((ht + 0xffff)) */
    i = 0x1;
    goto L_45b4;

L_459b:
    /* untranslated: 0x25:[(0xb82 + (i * 0x4))] = 0xffff */
    /* untranslated: 0x25:[(0xb82 + (i * 0x4))+0x2] = 0xffff */
    i = (i + 0x1);

L_45b4:
    if ((i <= 0x4))
        goto L_459b;
    else
        goto L_45bd;

L_45bd:
    if ((pl.det < 0x3))
        goto L_478e;
    else
        goto L_45cb;

L_45cb:
    /* untranslated: part[14:2](GlobalPD) = part[2:1](pl.rgpctMinLevel[ht]) */
    GlobalPD.iPlrMax = 0x0;
    /* untranslated: part[6:2](GlobalPD) = pl.fHomeworld */
    /* untranslated: part[8:2](GlobalPD) = 0x0 */
    if ((pl.det <= 0x3))
        goto L_478e;
    else
        goto L_4608;

L_4608:
    lVal = 0x0;
    /* untranslated: part[10:2](GlobalPD) = LOWORD(pl.rgwtMin[(ht - 0x1)]) */
    /* untranslated: part[12:2](GlobalPD) = HIWORD(pl.rgwtMin[(ht - 0x1)]) */
    EstMineralsMined(&(pl), rglQuan, 0xffffffff, 0x0);
    /* untranslated: part[18:2](GlobalPD) = LOWORD(rglQuan[(ht - 0x1)]) */
    /* untranslated: part[20:2](GlobalPD) = HIWORD(rglQuan[(ht - 0x1)]) */
    if ((pl.iPlayer != 0xffff))
        goto L_478e;
    else
        goto L_4670;

L_4670:
    ifl = 0x0;
    goto L_467c;

L_4678:
    ifl = (ifl + 0x1);

L_467c:
    if ((ifl >= cFleet))
        goto L_476a;
    else
        goto L_4687;

L_4687:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_46b7;
    else
        goto L_46af;

L_46af:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_476a;
    else
        goto L_46b7;

L_46b7:
    if ((lpfl->idPlanet != pl.id))
        goto L_4678;
    else
        goto L_46c6;

L_46c6:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4678;
    else
        goto L_46d5;

L_46d5:
    if ((lpfl->fDead != 0x0))
        goto L_4678;
    else
        goto L_46ec;

L_46ec:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x3 ? L_4678 : L_46ff */

L_46ff:

L_4705:
    cMines = CMineFromLpfl(lpfl);
    if ((HIWORD(cMines) < 0x0))
        goto L_4678;
    else
        goto L_4722;

L_4722:
    if ((HIWORD(cMines) > 0x0))
        goto L_4730;
    else
        goto L_4727;

L_4727:
    if ((LOWORD(cMines) <= 0x0))
        goto L_4678;
    else
        goto L_4730;

L_4730:
    EstMineralsMined(&(pl), rglT, cMines, 0x0);
    lVal = (lVal + rglT[(ht - 0x1)]);

L_476a:
    if ((HIWORD(lVal) < 0x0))
        goto L_478e;
    else
        goto L_4773;

L_4773:
    if ((HIWORD(lVal) > 0x0))
        goto L_4781;
    else
        goto L_4778;

L_4778:
    if ((LOWORD(lVal) <= 0x0))
        goto L_478e;
    else
        goto L_4781;

L_4781:
    /* untranslated: part[18:2](GlobalPD) = LOWORD(lVal) */
    /* untranslated: part[20:2](GlobalPD) = HIWORD(lVal) */

L_478e:
    Popup(hwndMine, x, y);
    goto L_47d5;

L_47a6:
    if ((ht > htMineMinefieldType))
        goto L_3b91;
    else
        goto L_47ae;

L_47ae:
    goto L_ffffffff;

L_47d5:
    return;
}

void SetMineralTitleBar(HWND hwnd) {
    char       szDeepSpace[40];
    char       szSummary[40];
    int16_t    fVisCB;
    char      *psz;
    GrobjClass grobj;
    RECT       rc;
    GrobjClass t_merge_481d_0001;
    uint16_t   t_merge_49c5_0001;
    uint16_t   t_merge_4a04_0001;
    uint16_t   t_merge_4a58_0001;

L_47dc:
    fVisCB = 0x0;
    CchGetString(idsDeepSpace, szDeepSpace);
    CchGetString(idsSummary, szSummary);
    if ((sel.scan.grobj != grobjOther))
        goto L_481a;
    else
        goto L_4814;

L_4814:
    t_merge_481d_0001 = sel.scan.grobjFull;
    goto L_481d;

L_481a:
    t_merge_481d_0001 = sel.scan.grobj;

L_481d:
    grobj = t_merge_481d_0001;
    if (((grobj & 0x1) == 0x0))
        goto L_4859;
    else
        goto L_482e;

L_482e:
    if ((sel.scan.idpl == 0xffff))
        goto L_48fe;
    else
        goto L_4838;

L_4838:
    psz = PszGetPlanetName(sel.scan.idpl);
    strcat(psz, szSummary);

L_4859:
    if (((grobj & 0x2) == 0x0))
        goto L_48a7;
    else
        goto L_4867;

L_4867:
    if ((sel.scan.ifl == 0xffff))
        goto L_48fe;
    else
        goto L_4871;

L_4871:
    psz = PszGetFleetName(LOWORD(rglpfl[sel.scan.ifl]));
    strcat(psz, szSummary);

L_48a7:
    if (((grobj & 0x8) == 0x0))
        goto L_48f8;
    else
        goto L_48b5;

L_48b5:
    if ((sel.scan.ith == 0xffff))
        goto L_48fe;
    else
        goto L_48bf;

L_48bf:
    fVisCB = 0x1;
    psz = PszGetThingName(LOWORD(lpThings[sel.scan.ith]));
    strcat(psz, szSummary);

L_48f8:
    psz = szDeepSpace;

L_48fe:
    strcpy(0x259c, psz);
    InvalidateRect(hwnd, 0x0, 0x1);
    if ((fVisCB == 0x0))
        goto L_49c8;
    else
        goto L_492a;

L_492a:
    if ((((LOWORD(lpThings[sel.scan.ith]) >> 0xd) & 0x7) != 0x0))
        goto L_49c2;
    else
        goto L_4950;

L_4950:
    /* untranslated: branch part[12:1](lpThings[sel.scan.ith]) != 0x0 ? L_49c2 : L_4972 */

L_4972:
    if ((((LOWORD(lpThings[sel.scan.ith]) >> 0x9) & 0xf) != idPlayer))
        goto L_49c2;
    else
        goto L_499b;

L_499b:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMines))
        goto L_49c2;
    else
        goto L_49bc;

L_49bc:
    t_merge_49c5_0001 = 0x1;
    goto L_49c5;

L_49c2:
    t_merge_49c5_0001 = 0x0;

L_49c5:
    fVisCB = t_merge_49c5_0001;

L_49c8:
    if ((fVisCB == 0x0))
        goto L_4a12;
    else
        goto L_49d1;

L_49d1:
    /* untranslated: branch part[13:1](lpThings[sel.scan.ith]) == 0x0 ? L_4a01 : L_49fb */

L_49fb:
    t_merge_4a04_0001 = 0x1;
    goto L_4a04;

L_4a01:
    t_merge_4a04_0001 = 0x0;

L_4a04:
    SendMessage(hwndMineCB, CB_LIMITTEXT, t_merge_4a04_0001, 0x0);

L_4a12:
    GetClientRect(hwnd, &(rc));
    if ((fVisCB == 0x0))
        goto L_4a55;
    else
        goto L_4a4f;

L_4a4f:
    t_merge_4a58_0001 = 0x40;
    goto L_4a58;

L_4a55:
    t_merge_4a58_0001 = 0x80;

L_4a58:
    SetWindowPos(hwndMineCB, 0x0, 0x50, (rc.bottom - (dyArial8 * 0x2)), (rc.right + 0xffa8), (dyArial8 + 0x4), (t_merge_4a58_0001 | 0x4));
    return;
}

void DrawSelectionArrow(HDC hdc, RECT *prc, int16_t fEnabled) {
    HBITMAP  hbmpSav;
    HDC      hdcMem;
    int16_t  xCtr;
    uint16_t t_merge_4b25_0001;
    uint16_t t_merge_4b38_0001;

L_4a68:
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpScanner);
    xCtr = (((uint32_t)((prc->right - prc->left)) / 0x2) + prc->left);
    BitBlt(hdc, (xCtr + 0xfffb), ((((prc->bottom - prc->top) >> 0x1) + prc->top) + 0xfffb), 0xb, 0xc, hdcMem, 0x23, 0x39, SRCAND);
    if ((fEnabled == 0x0))
        goto L_4b22;
    else
        goto L_4b1c;

L_4b1c:
    t_merge_4b25_0001 = 0x18;
    goto L_4b25;

L_4b22:
    t_merge_4b25_0001 = 0x0;

L_4b25:
    if ((fEnabled == 0x0))
        goto L_4b35;
    else
        goto L_4b2f;

L_4b2f:
    t_merge_4b38_0001 = 0x39;
    goto L_4b38;

L_4b35:
    t_merge_4b38_0001 = 0x2d;

L_4b38:
    BitBlt(hdc, (xCtr + 0xfffb), ((((prc->bottom - prc->top) >> 0x1) + prc->top) + 0xfffb), 0xb, 0xc, hdcMem, t_merge_4b25_0001, t_merge_4b38_0001, SRCPAINT);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    return;
}

void DrawDiamond(HDC hdc, RECT *prc, HBRUSH hbr) {
    HBRUSH  hbrSav;
    int16_t yTop;
    int16_t yBot;
    int16_t xCtr;
    int16_t dx;
    int16_t xCur;
    int16_t t_4bbb;
    int16_t t_4bdb;
    int16_t t_4be3;
    int16_t t_4c84;
    int16_t t_4ca4;
    int16_t t_4cac;
    int16_t t_4d0b;
    int16_t t_4d2a;
    int16_t t_4d32;

L_4b60:
    xCtr = (((uint32_t)((prc->right - prc->left)) / 0x2) + prc->left);
    yTop = (prc->top + 0x1);
    yBot = (prc->bottom + 0xfffe);
    xCur = (xCtr + 0xffff);
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    goto L_4c00;

L_4bb5:
    t_4bbb = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, xCur, t_4bbb, 0x2, 0x1, PATCOPY);
    t_4bdb = xCur;
    xCur = (xCur - 0x1);
    t_4be3 = yBot;
    yBot = (yBot - 0x1);
    PatBlt(hdc, t_4bdb, t_4be3, 0x2, 0x1, PATCOPY);

L_4c00:
    if ((yTop <= yBot))
        goto L_4bb5;
    else
        goto L_4c0b;

L_4c0b:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xCtr, prc->top, 0x1, 0x1, PATCOPY);
    PatBlt(hdc, xCtr, (prc->bottom + 0xffff), 0x1, 0x1, PATCOPY);
    yTop = (prc->top + 0x1);
    yBot = (prc->bottom + 0xfffe);
    xCur = xCtr;
    goto L_4cc9;

L_4c7e:
    t_4c84 = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, xCur, t_4c84, 0x2, 0x1, PATCOPY);
    t_4ca4 = xCur;
    xCur = (xCur + 0x1);
    t_4cac = yBot;
    yBot = (yBot - 0x1);
    PatBlt(hdc, t_4ca4, t_4cac, 0x2, 0x1, PATCOPY);

L_4cc9:
    if ((yTop <= yBot))
        goto L_4c7e;
    else
        goto L_4cd4;

L_4cd4:
    yTop = (prc->top + 0x4);
    yBot = (prc->bottom + 0xfffb);
    xCur = xCtr;
    dx = 0x1;
    SelectObject(hdc, hbr);
    goto L_4d52;

L_4d05:
    t_4d0b = yTop;
    yTop = (yTop + 0x1);
    PatBlt(hdc, xCur, t_4d0b, dx, 0x1, PATCOPY);
    t_4d2a = xCur;
    xCur = (xCur - 0x1);
    t_4d32 = yBot;
    yBot = (yBot - 0x1);
    PatBlt(hdc, t_4d2a, t_4d32, dx, 0x1, PATCOPY);
    dx = (dx + 0x2);

L_4d52:
    if ((yTop <= yBot))
        goto L_4d05;
    else
        goto L_4d5d;

L_4d5d:
    SelectObject(hdc, hbrSav);
    return;
}

int16_t FOtherStuffAtScanSel() {
    int16_t c;
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;
    int16_t t_4e24;
    int16_t t_4e99;

L_4d6e:
    if ((sel.scan.idpl == 0xffff))
        goto L_4d9b;
    else
        goto L_4d81;

L_4d81:
    if ((sel.scan.ifl != 0xffff))
        goto L_4d95;
    else
        goto L_4d8b;

L_4d8b:
    if ((sel.scan.ith == 0xffff))
        goto L_4d9b;
    else
        goto L_4d95;

L_4d95:
    return 0x1;

L_4d9b:
    if ((sel.scan.ifl == 0xffff))
        goto L_4e3c;
    else
        goto L_4da5;

L_4da5:
    if ((sel.scan.ith == 0xffff))
        goto L_4db5;
    else
        goto L_4daf;

L_4daf:
    return 0x1;

L_4db5:
    c = 0x1;
    i = 0x0;
    goto L_4dc6;

L_4dc2:
    i = (i + 0x1);

L_4dc6:
    if ((i >= cFleet))
        goto L_4e3c;
    else
        goto L_4dd1;

L_4dd1:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_4e01;
    else
        goto L_4df9;

L_4df9:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_4e3c;
    else
        goto L_4e01;

L_4e01:
    if ((lpfl->pt.x != sel.scan.pt.x))
        goto L_4dc2;
    else
        goto L_4e1b;

L_4e1b:
    if ((lpfl->pt.y != sel.scan.pt.y))
        goto L_4dc2;
    else
        goto L_4e24;

L_4e24:
    t_4e24 = c;
    c = (c - 0x1);
    if ((t_4e24 != 0x0))
        goto L_4dc2;
    else
        goto L_4e33;

L_4e33:
    return 0x1;

L_4e3c:
    if ((sel.scan.ith == 0xffff))
        goto L_4ec0;
    else
        goto L_4e46;

L_4e46:
    c = 0x1;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_4eb2;

L_4e76:
    if ((lpth->pt.x != sel.scan.pt.x))
        goto L_4eae;
    else
        goto L_4e90;

L_4e90:
    if ((lpth->pt.y != sel.scan.pt.y))
        goto L_4eae;
    else
        goto L_4e99;

L_4e99:
    t_4e99 = c;
    c = (c - 0x1);
    if ((t_4e99 != 0x0))
        goto L_4eae;
    else
        goto L_4ea8;

L_4ea8:
    return 0x1;

L_4eae:
    lpth = (lpth + 0x1);

L_4eb2:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_4e76;
    else
        goto L_4ec0;

L_4ec0:
    return 0x0;
}

void PopupMineralScanChoices(HWND hwnd, int16_t x, int16_t y) {
    int16_t  fSep;
    int16_t  id;
    int16_t  fOurs;
    PLANET  *lppl;
    int16_t  i;
    int16_t  c;
    THING   *lpth;
    FLEET   *lpfl;
    THING   *lpthMac;
    int32_t  rgid[100];
    int16_t  idNew;
    int16_t  iChecked;
    SCAN     scan;
    uint16_t t_merge_5017_0001;
    uint16_t t_merge_520d_0001;
    uint16_t t_merge_530f_0001;

L_4ecc:
    iChecked = 0xffff;
    if ((sel.scan.idpl == 0xffff))
        goto L_4f15;
    else
        goto L_4ee5;

L_4ee5:
    LOWORD(rgid) = sel.scan.idpl;
    HIWORD(rgid) = SIGNHIWORD(sel.scan.idpl);
    /* untranslated: part[4:2](rgid) = 0xffff */
    /* untranslated: part[6:2](rgid) = 0xffff */
    c = 0x2;
    if ((sel.scan.grobj != grobjPlanet))
        goto L_4f1a;
    else
        goto L_4f0c;

L_4f0c:
    iChecked = 0x0;

L_4f15:
    c = 0x0;

L_4f1a:
    i = 0x0;
    goto L_4f26;

L_4f22:
    i = (i + 0x1);

L_4f26:
    if ((i >= cFleet))
        goto L_4fed;
    else
        goto L_4f31;

L_4f31:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_4f61;
    else
        goto L_4f59;

L_4f59:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_4fed;
    else
        goto L_4f61;

L_4f61:
    if ((sel.scan.pt.x != lpfl->pt.x))
        goto L_4f22;
    else
        goto L_4f7b;

L_4f7b:
    if ((sel.scan.pt.y != lpfl->pt.y))
        goto L_4f22;
    else
        goto L_4f84;

L_4f84:
    if ((sel.scan.grobj != grobjFleet))
        goto L_4fba;
    else
        goto L_4f8e;

L_4f8e:
    if ((LOWORD(rglpfl[sel.scan.ifl]) != LOWORD(lpfl)))
        goto L_4fba;
    else
        goto L_4fb3;

L_4fb3:
    iChecked = c;

L_4fba:
    c = (c + 0x1);
    LOWORD(rgid[c]) = (LOWORD(lpfl) | 0x0);
    HIWORD(rgid[c]) = (SIGNHIWORD(LOWORD(lpfl)) | 0x8000);
    if ((c >= 0x64))
        goto L_4fed;
    else
        goto L_4fe4;

L_4fe4:

L_4fed:
    if ((c != 0x2))
        goto L_5005;
    else
        goto L_4ff6;

L_4ff6:
    if ((sel.scan.idpl == 0xffff))
        goto L_5005;
    else
        goto L_5000;

L_5000:
    c = 0x1;

L_5005:
    if ((c != 0x0))
        goto L_5014;
    else
        goto L_500e;

L_500e:
    t_merge_5017_0001 = 0x1;
    goto L_5017;

L_5014:
    t_merge_5017_0001 = 0x0;

L_5017:
    fSep = t_merge_5017_0001;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_50f7;

L_5049:
    if ((sel.scan.pt.x != lpth->pt.x))
        goto L_50f3;
    else
        goto L_5063;

L_5063:
    if ((sel.scan.pt.y != lpth->pt.y))
        goto L_50f3;
    else
        goto L_506c;

L_506c:
    if ((fSep != 0x0))
        goto L_5094;
    else
        goto L_5075;

L_5075:
    c = (c + 0x1);
    rgid[c] = 0xffffffff;
    fSep = 0x1;

L_5094:
    if ((sel.scan.grobj != grobjThing))
        goto L_50cd;
    else
        goto L_509e;

L_509e:
    if ((((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 0x12) != sel.scan.ith))
        goto L_50cd;
    else
        goto L_50c6;

L_50c6:
    iChecked = c;

L_50cd:
    c = (c + 0x1);
    LOWORD(rgid[c]) = (LOWORD(lpth) | 0x0);
    HIWORD(rgid[c]) = 0x2000;

L_50f3:
    lpth = (lpth + 0x1);

L_50f7:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_5049;
    else
        goto L_5105;

L_5105:
    i = PopupMenu(hwnd, x, y, c, rgid, 0x0, iChecked, 0x1);
    if ((i < 0x0))
        goto L_535c;
    else
        goto L_5136;

L_5136:
    scan = sel.scan;
    if (((LOWORD(rgid[i]) & 0x0) != 0x0))
        goto L_5171;
    else
        goto L_5169;

L_5169:
    if (((HIWORD(rgid[i]) & 0x8000) == 0x0))
        goto L_5213;
    else
        goto L_5171;

L_5171:
    scan.grobj = grobjFleet;
    id = LOWORD(rgid[i]);
    i = 0x0;
    goto L_5198;

L_5194:
    i = (i + 0x1);

L_5198:
    if ((i >= cFleet))
        goto L_51e7;
    else
        goto L_51a3;

L_51a3:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_51d3;
    else
        goto L_51cb;

L_51cb:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_51e7;
    else
        goto L_51d3;

L_51d3:
    if ((LOWORD(lpfl) == id))
        goto L_51e7;
    else
        goto L_51de;

L_51de:

L_51e7:
    scan.ifl = i;
    idNew = id;
    if ((lpfl->iPlayer != idPlayer))
        goto L_520a;
    else
        goto L_5204;

L_5204:
    t_merge_520d_0001 = 0x1;
    goto L_520d;

L_520a:
    t_merge_520d_0001 = 0x0;

L_520d:
    fOurs = t_merge_520d_0001;
    goto L_5312;

L_5213:
    if (((LOWORD(rgid[i]) & 0x0) != 0x0))
        goto L_523c;
    else
        goto L_5234;

L_5234:
    if (((HIWORD(rgid[i]) & 0x2000) == 0x0))
        goto L_52d8;
    else
        goto L_523c;

L_523c:
    scan.grobj = grobjThing;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_5295;

L_5271:
    if ((LOWORD(lpth) == LOWORD(rgid[i])))
        goto L_52a3;
    else
        goto L_528b;

L_528b:

L_5291:
    lpth = (lpth + 0x1);

L_5295:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_5271;
    else
        goto L_52a3;

L_52a3:
    scan.ith = ((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 0x12);
    idNew = LOWORD(lpth);
    fOurs = 0x0;
    goto L_5312;

L_52d8:
    scan.grobj = grobjPlanet;
    idNew = sel.scan.idpl;
    lppl = LpplFromId(idNew);
    if ((lppl->iPlayer != idPlayer))
        goto L_530c;
    else
        goto L_5306;

L_5306:
    t_merge_530f_0001 = 0x1;
    goto L_530f;

L_530c:
    t_merge_530f_0001 = 0x0;

L_530f:
    fOurs = t_merge_530f_0001;

L_5312:
    ChangeScanSel(&(scan), 0x2);
    if ((fOurs == 0x0))
        goto L_535c;
    else
        goto L_532c;

L_532c:
    RedrawScanSel(0x0, 0x0);
    ChangeMainObjSel(scan.grobj, idNew);
    RedrawScanSel(0x0, 0x1);

L_535c:
    return;
}

void EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply) {
    int32_t  lQuanRem;
    int32_t  lQuanAct;
    int16_t  i;
    int32_t  lQuan;
    int16_t  fMacintosh;
    int16_t  fRemote;
    int32_t  lMine;
    int32_t  lMineEff;
    int32_t  lConc;
    int32_t  lLeft;
    int32_t  lLevel;
    int32_t  lLength;
    int32_t  rglQuan[3];
    int16_t  ifl;
    FLEET   *lpfl;
    uint16_t t_merge_5386_0001;
    uint16_t t_merge_53c3_0001;

L_5362:
    if ((LOWORD(cMines) != 0xffff))
        goto L_537d;
    else
        goto L_5374;

L_5374:
    if ((HIWORD(cMines) == 0xffff))
        goto L_5383;
    else
        goto L_537d;

L_537d:
    t_merge_5386_0001 = 0x1;
    goto L_5386;

L_5383:
    t_merge_5386_0001 = 0x0;

L_5386:
    fRemote = t_merge_5386_0001;
    if ((lppl->iPlayer == 0xffff))
        goto L_53c0;
    else
        goto L_5396;

L_5396:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_53c0;
    else
        goto L_53ba;

L_53ba:
    t_merge_53c3_0001 = 0x1;
    goto L_53c3;

L_53c0:
    t_merge_53c3_0001 = 0x0;

L_53c3:
    fMacintosh = t_merge_53c3_0001;
    if ((LOWORD(cMines) != 0xffff))
        goto L_547a;
    else
        goto L_53cf;

L_53cf:
    if ((HIWORD(cMines) != 0xffff))
        goto L_547a;
    else
        goto L_53d8;

L_53d8:
    if ((lppl->iPlayer == 0xffff))
        goto L_53fc;
    else
        goto L_53e5;

L_53e5:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_5429;
    else
        goto L_53f2;

L_53f2:
    if ((HIWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_5429;
    else
        goto L_53fc;

L_53fc:
    i = 0x0;
    goto L_541d;

L_5404:
    plQuan[i] = 0xffffffff;
    i = (i + 0x1);

L_541d:
    if ((i >= 0x3))
        goto L_59f4;
    else
        goto L_5423;

L_5423:

L_5429:
    lMine = (uint32_t)(CMinesOperating(lppl));
    if ((fMacintosh == 0x0))
        goto L_5454;
    else
        goto L_5447;

L_5447:
    lMineEff = 0xa;
    goto L_5490;

L_5454:
    lMineEff = (uint32_t)(GetRaceStat(rgplr[lppl->iPlayer], rsMineProd));

L_547a:
    lMine = cMines;
    lMineEff = 0xa;

L_5490:
    i = 0x0;
    goto L_5893;

L_5498:
    LOWORD(lConc) = lppl->rgMinConc[i];
    HIWORD(lConc) = 0x0;
    if ((HIWORD(lConc) > 0x0))
        goto L_5503;
    else
        goto L_54c2;

L_54c2:
    if ((HIWORD(lConc) < 0x0))
        goto L_54d0;
    else
        goto L_54c7;

L_54c7:
    if ((LOWORD(lConc) >= 0x1e))
        goto L_5503;
    else
        goto L_54d0;

L_54d0:
    if ((lppl->fHomeworld == 0x0))
        goto L_5503;
    else
        goto L_54e7;

L_54e7:
    if ((fRemote == 0x0))
        goto L_54f9;
    else
        goto L_54f0;

L_54f0:
    if ((fMacintosh == 0x0))
        goto L_5503;
    else
        goto L_54f9;

L_54f9:
    lConc = 0x1e;

L_5503:
    lQuanAct = (uint32_t)((lMine * lConc));
    if ((fRemote != 0x0))
        goto L_554c;
    else
        goto L_5523;

L_5523:
    lQuan = (int32_t)(((uint32_t)((lQuanAct * lMineEff)) / 0xa));
    goto L_5558;

L_554c:
    lQuan = lQuanAct;

L_5558:
    lQuanRem = (int32_t)((lQuan % 0x64));
    lQuan = (int32_t)((lQuan / 0x64));
    if ((LOWORD(lQuanRem) != 0x0))
        goto L_559c;
    else
        goto L_5593;

L_5593:
    if ((HIWORD(lQuanRem) == 0x0))
        goto L_55d3;
    else
        goto L_559c;

L_559c:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_55d3;
    else
        goto L_55ac;

L_55ac:
    if ((Random(0x64) >= LOWORD(lQuanRem)))
        goto L_55d3;
    else
        goto L_55cb;

L_55cb:
    lQuan = (lQuan + 0x1);

L_55d3:
    plQuan[i] = lQuan;
    if ((fApply == 0x0))
        goto L_588f;
    else
        goto L_55f3;

L_55f3:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] + lQuan);
    lQuanAct = (int32_t)((lQuanAct / 0x64));

L_562f:
    if ((HIWORD(lQuanAct) < 0x0))
        goto L_588f;
    else
        goto L_5638;

L_5638:
    if ((HIWORD(lQuanAct) > 0x0))
        goto L_5646;
    else
        goto L_563d;

L_563d:
    if ((LOWORD(lQuanAct) <= 0x0))
        goto L_588f;
    else
        goto L_5646;

L_5646:
    if ((lppl->rgMinConc[i] <= 0x1))
        goto L_588f;
    else
        goto L_5666;

L_5666:
    LOWORD(lLevel) = lppl->rgpctMinLevel[i];
    HIWORD(lLevel) = 0x0;
    LOWORD(lConc) = lppl->rgMinConc[i];
    HIWORD(lConc) = 0x0;
    if ((LOWORD(lLevel) != 0x0))
        goto L_56c4;
    else
        goto L_56b1;

L_56b1:
    if ((HIWORD(lLevel) != 0x0))
        goto L_56c4;
    else
        goto L_56ba;

L_56ba:
    lLevel = 0x100;

L_56c4:
    if ((HIWORD(lConc) < 0x0))
        goto L_56e8;
    else
        goto L_56cd;

L_56cd:
    if ((HIWORD(lConc) > 0x0))
        goto L_56db;
    else
        goto L_56d2;

L_56d2:
    if ((LOWORD(lConc) <= 0x64))
        goto L_56e8;
    else
        goto L_56db;

L_56db:
    lConc = 0x64;
    goto L_572d;

L_56e8:
    if ((HIWORD(lConc) > 0x0))
        goto L_570c;
    else
        goto L_56f1;

L_56f1:
    if ((HIWORD(lConc) < 0x0))
        goto L_56ff;
    else
        goto L_56f6;

L_56f6:
    if ((LOWORD(lConc) >= 0x5))
        goto L_570c;
    else
        goto L_56ff;

L_56ff:
    lConc = 0xa;
    goto L_572d;

L_570c:
    if ((HIWORD(lConc) > 0x0))
        goto L_572d;
    else
        goto L_5715;

L_5715:
    if ((HIWORD(lConc) < 0x0))
        goto L_5723;
    else
        goto L_571a;

L_571a:
    if ((LOWORD(lConc) >= 0x19))
        goto L_572d;
    else
        goto L_5723;

L_5723:
    lConc = 0x19;

L_572d:
    lLeft = (int32_t)(((int32_t)(((uint32_t)((lLevel * 0x30d4)) / 0x100)) / lConc));
    if ((HIWORD(lLeft) > HIWORD(lQuanAct)))
        goto L_57b8;
    else
        goto L_5770;

L_5770:
    if ((HIWORD(lLeft) < HIWORD(lQuanAct)))
        goto L_577d;
    else
        goto L_5775;

L_5775:
    if ((LOWORD(lLeft) > LOWORD(lQuanAct)))
        goto L_57b8;
    else
        goto L_577d;

L_577d:
    lQuanAct = (lQuanAct - lLeft);
    lppl->rgMinConc[i] = (lppl->rgMinConc[i] - 0x1);
    lppl->rgpctMinLevel[i] = 0x0;
    goto L_562f;

L_57b8:
    lLength = (int32_t)((0x30d4 / lConc));
    /* untranslated: lLeft = (int32_t)((int32_t)(words((LOWORD(lLeft) - LOWORD(lQuanAct)), (HIWORD(lLeft) - HIWORD(lQuanAct))) * 0x100) / lLength) */
    if ((HIWORD(lLeft) > 0x0))
        goto L_5819;
    else
        goto L_5801;

L_5801:
    if ((HIWORD(lLeft) < 0x0))
        goto L_580f;
    else
        goto L_5806;

L_5806:
    if ((LOWORD(lLeft) >= 0x1))
        goto L_5819;
    else
        goto L_580f;

L_580f:
    lLeft = 0x1;

L_5819:
    if ((HIWORD(lLeft) < HIWORD(lLevel)))
        goto L_5846;
    else
        goto L_5827;

L_5827:
    if ((HIWORD(lLeft) > HIWORD(lLevel)))
        goto L_5834;
    else
        goto L_582c;

L_582c:
    if ((LOWORD(lLeft) < LOWORD(lLevel)))
        goto L_5846;
    else
        goto L_5834;

L_5834:
    lLeft = (lLevel + 0xffffffff);

L_5846:
    lppl->rgpctMinLevel[i] = LOBYTE(LOWORD(lLeft));
    if ((LOWORD(lLeft) != 0x0))
        goto L_588f;
    else
        goto L_586a;

L_586a:
    if ((HIWORD(lLeft) != 0x0))
        goto L_588f;
    else
        goto L_5873;

L_5873:
    lppl->rgMinConc[i] = (lppl->rgMinConc[i] - 0x1);

L_588f:
    i = (i + 0x1);

L_5893:
    if ((i <= 0x2))
        goto L_5498;
    else
        goto L_589c;

L_589c:
    if ((fMacintosh == 0x0))
        goto L_59f4;
    else
        goto L_58a5;

L_58a5:
    if ((fRemote != 0x0))
        goto L_59f4;
    else
        goto L_58ae;

L_58ae:
    ifl = 0x0;
    goto L_58ba;

L_58b6:
    ifl = (ifl + 0x1);

L_58ba:
    if ((ifl >= cFleet))
        goto L_59f4;
    else
        goto L_58c5;

L_58c5:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_58f5;
    else
        goto L_58ed;

L_58ed:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_59f4;
    else
        goto L_58f5;

L_58f5:
    if ((lpfl->idPlanet != lppl->id))
        goto L_58b6;
    else
        goto L_5907;

L_5907:
    if ((lpfl->iPlayer != lppl->iPlayer))
        goto L_58b6;
    else
        goto L_591a;

L_591a:
    if ((lpfl->fDead != 0x0))
        goto L_58b6;
    else
        goto L_5931;

L_5931:
    if ((lpfl->cord > 0x1))
        goto L_58b6;
    else
        goto L_593e;

L_593e:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x3 ? L_58b6 : L_5951 */

L_5951:

L_5957:
    cMines = CMineFromLpfl(lpfl);
    if ((HIWORD(cMines) < 0x0))
        goto L_58b6;
    else
        goto L_5974;

L_5974:
    if ((HIWORD(cMines) > 0x0))
        goto L_5982;
    else
        goto L_5979;

L_5979:
    if ((LOWORD(cMines) <= 0x0))
        goto L_58b6;
    else
        goto L_5982;

L_5982:
    EstMineralsMined(lppl, rglQuan, cMines, fApply);
    i = 0x0;
    goto L_59cb;

L_59a5:
    plQuan[i] = (plQuan[i] + rglQuan[i]);
    i = (i + 0x1);

L_59cb:
    if ((i < 0x3))
        goto L_59a5;
    else
        goto L_59d4;

L_59d4:
    if ((fApply == 0x0))
        goto L_58b6;
    else
        goto L_59dd;

L_59dd:
    lpfl->fHereAllTurn = 0x0;

L_59f4:
    return;
}
