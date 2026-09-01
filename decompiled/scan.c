#include "common.h"

uint32_t rgcrScanMine[3] = {16711680, 65535, 255};
int16_t  vrgPopRad[19] = {25, 50, 100, 200, 400, 800, 1000, 1500, 2250, 3000, 4000, 5000, 6000, 7500, 9000, 11000, 14000, 18000, 25000};

int32_t ScannerWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    POINT       pt;
    PAINTSTRUCT ps;
    RECT        rc;
    int16_t     iScanNew;
    HPEN        hpenSav;
    int16_t     iRopSav;
    int16_t     i;
    uint32_t    tick;
    PLANET      plT;
    int16_t     fChgScan;
    SCAN        scan;
    int16_t     c;
    THING      *lpth;
    FLEET      *lpfl;
    int16_t     fSep;
    int32_t     rgid[100];
    int16_t     iChecked;
    int16_t     iSel;
    THING      *lpthMac;
    int16_t     id;
    int16_t     d;
    int16_t     dy;
    int16_t     dx;
    HWND        t_merge_0053_0001;
    uint32_t    t_call_0131;
    uint16_t    t_merge_04e7_0001;
    uint16_t    t_merge_081f_0001;
    uint16_t    t_merge_0b3c_0001;
    int16_t     t_merge_0cd2_0001;
    int16_t     t_merge_0d88_0001;

L_0032:
    goto L_0e87;

L_0041:
    if ((wParam == 0x0))
        goto L_0050;
    else
        goto L_004a;

L_004a:
    t_merge_0053_0001 = hwnd;
    goto L_0053;

L_0050:
    t_merge_0053_0001 = 0x0;

L_0053:
    hwndActive = t_merge_0053_0001;
    goto L_0eea;

L_0059:
    yScanTop = 1000;
    xScanTop = 1000;
    goto L_0eea;

L_0068:
    if ((wParam == 0x76))
        goto L_007a;
    else
        goto L_0071;

L_0071:
    if ((wParam != 0x56))
        goto L_0190;
    else
        goto L_007a;

L_007a:
    hdc = GetDC(hwndScanner);
    pt.x = sel.scan.pt.x;
    pt.y = sel.scan.pt.y;
    LogicalToScan(&(pt));
    iRopSav = SetROP2(hdc, 0x7);
    hpenSav = SelectObject(hdc, hpenYellow);
    i = 0;
    goto L_0162;

L_00c5:
    MoveTo(hdc, (pt.x - 300), (pt.y - 300));
    LineTo(hdc, (pt.x + 300), (pt.y + 300));
    MoveTo(hdc, (pt.x - 300), (pt.y + 300));
    LineTo(hdc, (pt.x + 300), (pt.y - 300));
    if ((i != 0))
        goto L_015e;
    else
        goto L_0126;

L_0126:
    tick = GetTickCount();

L_0131:
    t_call_0131 = GetTickCount();
    if ((HIWORD(t_call_0131) > (HIWORD(tick) + 0x0)))
        goto L_015e;
    else
        goto L_014a;

L_014a:
    if ((HIWORD(t_call_0131) < (HIWORD(tick) + 0x0)))
        goto L_0156;
    else
        goto L_014f;

L_014f:
    if ((LOWORD(t_call_0131) >= (LOWORD(tick) + 0x96)))
        goto L_015e;
    else
        goto L_0156;

L_0156:
    Yield();
    goto L_0131;

L_015e:
    i = (i + 1);

L_0162:
    if ((i < 2))
        goto L_00c5;
    else
        goto L_016b;

L_016b:
    SelectObject(hdc, hpenSav);
    SetROP2(hdc, iRopSav);
    ReleaseDC(hwndScanner, hdc);
    goto L_0eea;

L_0190:
    if ((wParam != 0x2d))
        goto L_01b3;
    else
        goto L_0199;

L_0199:
    iScanNew = (iScanZoom - 1);
    if ((iScanNew >= -4))
        goto L_01ca;
    else
        goto L_01ab;

L_01ab:
    iScanNew = -4;

L_01b3:
    iScanNew = (iScanZoom + 1);
    if ((iScanNew <= 4))
        goto L_01ca;
    else
        goto L_01c5;

L_01c5:
    iScanNew = 4;

L_01ca:
    if ((iScanNew == iScanZoom))
        goto L_0eea;
    else
        goto L_01d5;

L_01d5:
    SendMessage(hwndFrame, WM_COMMAND, (iScanNew + 3905), 0x0);

L_01f4:
    hdc = BeginPaint(hwnd, &(ps));
    if ((LOWORD(rglpfl) != 0x0))
        goto L_021a;
    else
        goto L_0210;

L_0210:
    if ((HIWORD(rglpfl) == 0x0))
        goto L_0263;
    else
        goto L_021a;

L_021a:
    if ((gd.fNoScannerDraw != 0x0))
        goto L_0263;
    else
        goto L_0228;

L_0228:
    GetClientRect(hwnd, &(rc));
    DrawScannerSBar(hdc, &(ps.rcPaint), 0x0, 0x1);
    rc.bottom = (rc.bottom - dySBar);
    DrawScanner(hdc, &(ps.rcPaint));

L_0263:
    EndPaint(hwnd, &(ps));
    goto L_0eea;

L_0275:
    GetCursorPos(&(pt));
    ScreenToClient(hwndScanner, &(pt));
    GetClientRect(hwnd, &(rc));
    if ((PtInRect(&(rc), pt) == 0))
        goto Default;
    else
        goto L_02ba;

L_02ba:
    rc.bottom = (rc.bottom - dySBar);
    if ((PtInRect(&(rc), pt) == 0))
        goto L_03c3;
    else
        goto L_02da;

L_02da:
    if ((sel.grobj != grobjFleet))
        goto L_0312;
    else
        goto L_02e4;

L_02e4:
    if (((GetAsyncKeyState(0x10) & 0xfffe) != 0x0))
        goto L_0306;
    else
        goto L_02f8;

L_02f8:
    if (((grbitScan & 0x10) == 0x0))
        goto L_0312;
    else
        goto L_0306;

L_0306:
    SetCursor(hcurScanAdd);
    goto L_03da;

L_0312:
    if ((FNearAWayPoint(pt, 0x0) == 0))
        goto L_0338;
    else
        goto L_032c;

L_032c:
    SetCursor(hcurOpenGrab);
    goto L_03da;

L_0338:
    if ((gd.fSetMassMode != 0x0))
        goto L_03ab;
    else
        goto L_034b;

L_034b:
    if ((gd.fSetRouteMode != 0x0))
        goto L_03ab;
    else
        goto L_035e;

L_035e:
    if ((sel.grobj != grobjPlanet))
        goto L_03b7;
    else
        goto L_0368;

L_0368:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_0397;
    else
        goto L_037c;

L_037c:
    if ((IWarpMAFromLppl(&(sel.pl), 0x0) > 0))
        goto L_03ab;
    else
        goto L_0397;

L_0397:
    if (((GetAsyncKeyState(0x11) & 0xfffe) == 0x0))
        goto L_03b7;
    else
        goto L_03ab;

L_03ab:
    SetCursor(hcurScanAdd);
    goto L_03da;

L_03b7:
    SetCursor(hcurScanner);

L_03c3:
    SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f00)));

L_03da:
    return 0x10000;

L_03e6:
    SetFocus(hwndFrame);
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    GetClientRect(hwnd, &(rc));
    if ((pt.y < (rc.bottom - dySBar)))
        goto L_04a8;
    else
        goto L_042d;

L_042d:
    if ((msg != WM_LBUTTONDOWN))
        goto L_0eea;
    else
        goto L_0434;

L_0434:

L_043a:
    if ((pt.y >= (rc.bottom - (dySBar >> 0x1))))
        goto L_0eea;
    else
        goto L_044c;

L_044c:
    if (((sel.scan.grobjFull & 0x3) == 0x0))
        goto L_0eea;
    else
        goto L_045a;

L_045a:
    if ((sel.scan.grobj != grobjFleet))
        goto L_048e;
    else
        goto L_0464;

L_0464:
    GlobalPD.grPopup = grPopupFleet;
    GlobalPD.lpfl = rglpfl[sel.scan.ifl];
    goto L_0494;

L_048e:
    GlobalPD.grPopup = grPopupUnknownObj;

L_0494:
    Popup(hwnd, pt.x, pt.y);

L_04a8:
    ScanToLogical(&(pt));
    if ((gd.fSetMassMode != 0x0))
        goto L_04de;
    else
        goto L_04cb;

L_04cb:
    if ((gd.fSetRouteMode == 0x0))
        goto L_04e4;
    else
        goto L_04de;

L_04de:
    t_merge_04e7_0001 = 0x1;
    goto L_04e7;

L_04e4:
    t_merge_04e7_0001 = 0xf;

L_04e7:
    FFindNearestObject(pt, t_merge_04e7_0001, &(scan));
    if ((gd.fSetMassMode != 0x0))
        goto L_053c;
    else
        goto L_0509;

L_0509:
    if ((sel.grobj != grobjPlanet))
        goto L_05d1;
    else
        goto L_0513;

L_0513:
    if (((wParam & 0x4) == 0x0))
        goto L_05d1;
    else
        goto L_0521;

L_0521:
    if ((IWarpMAFromLppl(&(sel.pl), 0x0) <= 0))
        goto L_05d1;
    else
        goto L_053c;

L_053c:
    if ((msg != WM_LBUTTONDOWN))
        goto L_05d1;
    else
        goto L_0546;

L_0546:
    DrawShipScanPath(0x0, 0x0);
    if ((scan.idpl != sel.pl.id))
        goto L_0570;
    else
        goto L_0561;

L_0561:
    sel.pl.idFling = 0x0;
    goto L_058f;

L_0570:
    sel.pl.idFling = (scan.idpl + 1);

L_058f:
    FLookupPlanet(0xffff, &(sel.pl));
    gd.fSetMassMode = 0x0;
    DrawShipScanPath(0x0, 0x1);
    DrawPlanShip(0x0, 0x4100);
    goto L_0eea;

L_05d1:
    if ((gd.fSetRouteMode != 0x0))
        goto L_05fc;
    else
        goto L_05e4;

L_05e4:
    if ((sel.grobj != grobjPlanet))
        goto L_068e;
    else
        goto L_05ee;

L_05ee:
    if (((wParam & 0x8) == 0x0))
        goto L_068e;
    else
        goto L_05fc;

L_05fc:
    if ((msg != WM_LBUTTONDOWN))
        goto L_068e;
    else
        goto L_0606;

L_0606:
    DrawShipScanPath(0x0, 0x0);
    if ((scan.idpl != sel.pl.id))
        goto L_0630;
    else
        goto L_0621;

L_0621:
    sel.pl.idRoute = 0x0;
    goto L_064f;

L_0630:
    sel.pl.wRouting = ((sel.pl.wRouting & 0xfc00) | ((scan.idpl + 0x1) & 0x3ff));

L_064f:
    FLookupPlanet(0xffff, &(sel.pl));
    gd.fSetRouteMode = 0x0;
    DrawShipScanPath(0x0, 0x1);
    DrawPlanShip(0x0, 0x4040);
    goto L_0eea;

L_068e:
    if ((msg == WM_MBUTTONDOWN))
        goto L_06b0;
    else
        goto L_0698;

L_0698:
    if ((msg != WM_RBUTTONDOWN))
        goto L_06c5;
    else
        goto L_06a2;

L_06a2:
    if (((wParam & 0x4) == 0x0))
        goto L_06c5;
    else
        goto L_06b0;

L_06b0:
    FHandleMeasuringTape(&(scan), pt);
    goto L_0eea;

L_06c5:
    if ((msg != WM_RBUTTONDOWN))
        goto L_0ae1;
    else
        goto L_06cf;

L_06cf:
    iChecked = -1;
    pt.x = scan.pt.x;
    pt.y = scan.pt.y;
    if (((scan.grobjFull & 0x1) == 0x0))
        goto L_072b;
    else
        goto L_06ef;

L_06ef:
    rgid[0] = (uint32_t)(scan.idpl);
    rgid[1] = -1;
    c = 2;
    if ((sel.grobj != grobjPlanet))
        goto L_0730;
    else
        goto L_0716;

L_0716:
    if ((sel.id != scan.idpl))
        goto L_0730;
    else
        goto L_0722;

L_0722:
    iChecked = 0;

L_072b:
    c = 0;

L_0730:
    i = 0;
    goto L_073c;

L_0738:
    i = (i + 1);

L_073c:
    if ((i >= cFleet))
        goto L_07f1;
    else
        goto L_0747;

L_0747:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_0779;
    else
        goto L_0771;

L_0771:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_07f1;
    else
        goto L_0779;

L_0779:
    /* untranslated: branch ss:[pt] != lpfl->pt.x ? L_0738 : L_0794 */

L_0794:
    /* untranslated: branch ss:[pt+0x2] != lpfl->pt.y ? L_0738 : L_079d */

L_079d:
    if ((sel.grobj != grobjFleet))
        goto L_07bd;
    else
        goto L_07a7;

L_07a7:
    if ((lpfl->id != sel.id))
        goto L_07bd;
    else
        goto L_07b6;

L_07b6:
    iChecked = c;

L_07bd:
    c = (c + 1);
    LOWORD(rgid[c]) = (lpfl->id | 0x0);
    HIWORD(rgid[c]) = (SIGNHIWORD(lpfl->id) | 0x8000);
    if ((c >= 98))
        goto L_07f1;
    else
        goto L_07e8;

L_07e8:

L_07f1:
    if ((c != 2))
        goto L_080d;
    else
        goto L_07fa;

L_07fa:
    if (((scan.grobjFull & 0x1) == 0x0))
        goto L_080d;
    else
        goto L_0808;

L_0808:
    c = 1;

L_080d:
    if ((c != 0))
        goto L_081c;
    else
        goto L_0816;

L_0816:
    t_merge_081f_0001 = 0x1;
    goto L_081f;

L_081c:
    t_merge_081f_0001 = 0x0;

L_081f:
    fSep = t_merge_081f_0001;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_08db;

L_0855:
    /* untranslated: branch ss:[pt] != lpth->pt.x ? L_08d6 : L_0870 */

L_0870:
    /* untranslated: branch ss:[pt+0x2] != lpth->pt.y ? L_08d6 : L_0879 */

L_0879:
    if ((fSep != 0))
        goto L_08a3;
    else
        goto L_0883;

L_0883:
    c = (c + 1);
    rgid[c] = -1;
    fSep = 1;

L_08a3:
    c = (c + 1);
    LOWORD(rgid[c]) = (lpth->idFull | 0x0);
    HIWORD(rgid[c]) = 0x2000;
    if ((c >= 100))
        goto L_08ec;
    else
        goto L_08d0;

L_08d0:

L_08d6:
    lpth = (lpth + 0x1);

L_08db:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_0855;
    else
        goto L_08ec;

L_08ec:
    LogicalToScan(&(pt));
    iSel = PopupMenu(hwnd, pt.x, pt.y, c, rgid, 0x0, iChecked, 0x1);
    if ((iSel < 0))
        goto L_0eea;
    else
        goto L_092b;

L_092b:
    if ((0x0 != 0x0))
        goto L_0955;
    else
        goto L_094d;

L_094d:
    if (((HIWORD(rgid[iSel]) & 0x8000) == 0x0))
        goto L_09d9;
    else
        goto L_0955;

L_0955:
    scan.grobj = grobjFleet;
    id = LOWORD(rgid[iSel]);
    i = 0;
    goto L_097d;

L_0979:
    i = (i + 1);

L_097d:
    if ((i >= cFleet))
        goto L_09d0;
    else
        goto L_0988;

L_0988:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_09ba;
    else
        goto L_09b2;

L_09b2:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_09d0;
    else
        goto L_09ba;

L_09ba:
    if ((lpfl->id == id))
        goto L_09d0;
    else
        goto L_09c7;

L_09c7:

L_09d0:
    scan.ifl = i;
    goto L_0a9d;

L_09d9:
    if ((0x0 != 0x0))
        goto L_0a03;
    else
        goto L_09fb;

L_09fb:
    if (((HIWORD(rgid[iSel]) & 0x2000) == 0x0))
        goto L_0a98;
    else
        goto L_0a03;

L_0a03:
    scan.grobj = grobjThing;
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_0a61;

L_0a3a:
    if ((lpth->idFull == LOWORD(rgid[iSel])))
        goto L_0a72;
    else
        goto L_0a56;

L_0a56:

L_0a5c:
    lpth = (lpth + 0x1);

L_0a61:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_0a3a;
    else
        goto L_0a72;

L_0a72:
    scan.ith = ((uint32_t)((LOWORD(lpth) - LOWORD(lpThings))) / 18);
    goto L_0a9d;

L_0a98:
    scan.grobj = grobjPlanet;

L_0a9d:
    ChangeScanSel(&(scan), 0x2);
    if ((scan.grobj != grobjPlanet))
        goto DblClick;
    else
        goto L_0ab6;

L_0ab6:
    if ((FLookupPlanet(scan.idpl, &(plT)) == 0))
        goto L_0eea;
    else
        goto L_0acd;

L_0acd:
    if ((plT.iPlayer != idPlayer))
        goto L_0eea;
    else
        goto L_0ad5;

L_0ad5:

L_0ae1:
    if ((sel.grobj != grobjFleet))
        goto L_0b1c;
    else
        goto L_0aeb;

L_0aeb:
    if (((wParam & 0x4) != 0x0))
        goto L_0b07;
    else
        goto L_0af9;

L_0af9:
    if (((grbitScan & 0x10) == 0x0))
        goto L_0b1c;
    else
        goto L_0b07;

L_0b07:
    FAddWayPoint(pt, &(scan));
    goto L_0eea;

L_0b1c:
    if ((scan.pt.x != sel.scan.pt.x))
        goto L_0b39;
    else
        goto L_0b2b;

L_0b2b:
    if ((scan.pt.y != sel.scan.pt.y))
        goto L_0b39;
    else
        goto L_0b33;

L_0b33:
    t_merge_0b3c_0001 = 0x1;
    goto L_0b3c;

L_0b39:
    t_merge_0b3c_0001 = 0x0;

L_0b3c:
    fChgScan = t_merge_0b3c_0001;
    ChangeScanSel(&(scan), 0x1);
    if ((FNearAWayPoint(pt, 0x1) == 0))
        goto L_0b82;
    else
        goto L_0b69;

L_0b69:
    if ((FHandleWayPointDrag(pt) != 0))
        goto L_0eea;
    else
        goto L_0b7c;

L_0b7c:

L_0b82:
    if ((fChgScan == 0))
        goto L_0eea;
    else
        goto L_0b88;

L_0b88:

L_0b8e:
    if ((scan.grobj == grobjFleet))
        goto L_0ba3;
    else
        goto L_0b97;

L_0b97:
    if ((scan.grobj != grobjPlanet))
        goto L_0eea;
    else
        goto L_0b9d;

L_0b9d:

L_0ba3:
    if ((scan.pt.x != sel.pt.x))
        goto L_0bd8;
    else
        goto L_0bb2;

L_0bb2:
    if ((scan.pt.y != sel.pt.y))
        goto L_0bd8;
    else
        goto L_0bba;

L_0bba:
    if ((FGetNextObjHere(&(scan), 0x1) == 0))
        goto L_0eea;
    else
        goto L_0bcf;

L_0bcf:

L_0bd8:
    if ((scan.grobj != grobjPlanet))
        goto DblClick;
    else
        goto L_0be1;

L_0be1:
    if ((FLookupPlanet(scan.idpl, &(plT)) == 0))
        goto L_0eea;
    else
        goto L_0bf5;

L_0bf5:

L_0bfb:
    if ((plT.iPlayer != idPlayer))
        goto L_0c29;
    else
        goto L_0c06;

L_0c06:
    if (((scan.grobjFull & 0x2) == 0x0))
        goto DblClick;
    else
        goto L_0c14;

L_0c14:
    if ((sel.grobj != grobjPlanet))
        goto DblClick;
    else
        goto L_0c1e;

L_0c1e:
    if ((scan.idpl != sel.id))
        goto DblClick;
    else
        goto L_0c29;

L_0c29:
    if (((scan.grobjFull & 0x2) == 0x0))
        goto L_0eea;
    else
        goto L_0c37;

L_0c37:
    scan.grobj = grobjFleet;
    goto DblClick;

DblClick:
    if ((scan.grobj != grobjFleet))
        goto L_0c70;
    else
        goto L_0c4b;

L_0c4b:
    if ((rglpfl[scan.ifl]->iPlayer != idPlayer))
        goto L_0eea;
    else
        goto L_0c6a;

L_0c6a:

L_0c70:
    if ((scan.grobj == grobjThing))
        goto L_0eea;
    else
        goto L_0c76;

L_0c76:

L_0c7c:
    if ((scan.grobj != grobjFleet))
        goto L_0c8a;
    else
        goto L_0c85;

L_0c85:
    scan.iwp = -1;

L_0c8a:
    ChangeScanSel(&(scan), 0x1);
    RedrawScanSel(0x0, 0x0);
    if ((scan.grobj != grobjPlanet))
        goto L_0cb9;
    else
        goto L_0cb3;

L_0cb3:
    t_merge_0cd2_0001 = scan.idpl;
    goto L_0cd2;

L_0cb9:
    t_merge_0cd2_0001 = rglpfl[scan.ifl]->id;

L_0cd2:
    ChangeMainObjSel(scan.grobj, t_merge_0cd2_0001);
    RedrawScanSel(0x0, 0x1);
    if ((scan.grobj != grobjFleet))
        goto L_0eea;
    else
        goto L_0cf7;

L_0cf7:
    if (((scan.grobjFull & 0x1) == 0x0))
        goto L_0eea;
    else
        goto L_0d05;

L_0d05:
    scan.grobj = grobjPlanet;
    ChangeScanSel(&(scan), 0x1);

L_0d1d:
    SetScanScrollBars(hwnd);
    PostMessage(hwndFrame, WM_COMMAND, (iScanZoom + 3905), 0x0);
    goto Default;

L_0d47:
    goto L_0da5;
    /* untranslated: d = (dScanInc neg 0) */
    goto L_0dc2;
    d = dScanInc;
    goto L_0dc2;
    /* untranslated: d = (dScanPage neg 0) */
    goto L_0dc2;
    d = dScanPage;
    goto L_0dc2;
    if ((msg != WM_VSCROLL))
        goto L_0d85;
    else
        goto L_0d7f;

L_0d7f:
    t_merge_0d88_0001 = yScanTop;
    goto L_0d88;

L_0d85:
    t_merge_0d88_0001 = xScanTop;

L_0d88:
    d = (LOWORD(lParam) - t_merge_0d88_0001);
    d = (d & 0xfffc);
    goto L_0dc2;

L_0da5:

L_0dc2:
    if ((d == 0))
        goto L_0eea;
    else
        goto L_0dcb;

L_0dcb:
    if ((msg != WM_VSCROLL))
        goto L_0e21;
    else
        goto L_0dd5;

L_0dd5:
    dy = yScanTop;
    SetScrollPos(hwnd, 0x1, (yScanTop + d), 0x1);
    yScanTop = GetScrollPos(hwnd, 0x1);
    ScrollScanner(0x0, PtToScan((dy - yScanTop)));
    goto L_0eea;

L_0e21:
    dx = xScanTop;
    SetScrollPos(hwnd, 0x0, (xScanTop + d), 0x1);
    xScanTop = GetScrollPos(hwnd, 0x0);
    ScrollScanner(PtToScan((dx - xScanTop)), 0x0);

Default:
    return DefWindowProc(hwnd, msg, wParam, lParam);

L_0e87:
    if ((msg == WM_CREATE))
        goto L_0059;
    else
        goto L_0e8f;

L_0e8f:
    if ((msg == WM_SIZE))
        goto L_0d1d;
    else
        goto L_0e97;

L_0e97:
    if ((msg == WM_PAINT))
        goto L_01f4;
    else
        goto L_0e9f;

L_0e9f:
    if ((msg == WM_SETCURSOR))
        goto L_0275;
    else
        goto L_0ea7;

L_0ea7:
    if ((msg == WM_CHAR))
        goto L_0068;
    else
        goto L_0eaf;

L_0eaf:
    if ((msg == WM_HSCROLL))
        goto L_0d47;
    else
        goto L_0eb7;

L_0eb7:
    if ((msg == WM_VSCROLL))
        goto L_0d47;
    else
        goto L_0ebf;

L_0ebf:
    if ((msg == WM_LBUTTONDOWN))
        goto L_03e6;
    else
        goto L_0ec7;

L_0ec7:
    if ((msg == WM_LBUTTONDBLCLK))
        goto L_03e6;
    else
        goto L_0ecf;

L_0ecf:
    if ((msg == WM_RBUTTONDOWN))
        goto L_03e6;
    else
        goto L_0ed7;

L_0ed7:
    if ((msg == WM_MBUTTONDOWN))
        goto L_03e6;
    else
        goto L_0edf;

L_0edf:
    if ((msg != WM_MDIACTIVATE))
        goto Default;
    else
        goto L_0ee4;

L_0ee4:

L_0eea:
    return 0x0;
}

int16_t PtToScan(int16_t d) {
L_0efc:
    if ((iScanZoom != 0))
        goto L_0f15;
    else
        goto L_0f0f;

L_0f0f:
    return d;

L_0f15:
    goto L_0f8f;

L_0f1b:
    goto L_0fb5;
    d = (d * 4);
    goto L_0fb5;
    d = (d * 2);
    goto L_0fb5;
    d = (d >> 0x1);
    goto L_0fb5;
    d = (d >> 0x2);
    goto L_0fb5;
    d = (((d << 0x1) + d) >> 0x3);
    goto L_0fb5;
    d = (((d << 0x1) + d) >> 0x2);
    goto L_0fb5;
    d = (((d << 0x2) + d) >> 0x2);
    goto L_0fb5;
    d = (((d << 0x1) + d) >> 0x1);
    goto L_0fb5;

L_0f8f:
    if (((iScanZoom + 4) > 0x8))
        goto L_0f1b;
    else
        goto L_0f9a;

L_0f9a:
    goto L_ffffffff;

L_0fb5:
    return d;
}

int16_t ScanToPt(int16_t d) {
L_0fc2:
    if ((iScanZoom != 0))
        goto L_0fdb;
    else
        goto L_0fd5;

L_0fd5:
    return d;

L_0fdb:
    goto L_1057;

L_0fe1:
    goto L_107d;
    d = (d >> 0x2);
    goto L_107d;
    d = (d >> 0x1);
    goto L_107d;
    d = (d * 2);
    goto L_107d;
    d = (d * 4);
    goto L_107d;
    d = ((uint32_t)((d * 0x8)) / 3);
    goto L_107d;
    d = ((uint32_t)((d * 0x4)) / 3);
    goto L_107d;
    d = ((uint32_t)((d * 0x4)) / 5);
    goto L_107d;
    d = ((uint32_t)((d * 0x2)) / 3);
    goto L_107d;

L_1057:
    if (((iScanZoom + 4) > 0x8))
        goto L_0fe1;
    else
        goto L_1062;

L_1062:
    goto L_ffffffff;

L_107d:
    return d;
}

int16_t DrawScanner(HDC hdc, RECT *prc) {
    int16_t  xOff;
    int16_t  dExpand;
    HPEN     hpenSav;
    FLEET   *lpflT;
    int16_t  j;
    int16_t  yTop;
    int16_t  xMax;
    POINT    pt;
    int16_t  id;
    COLORREF crFore;
    int16_t  iBkPrev;
    POINT    ptD;
    PLANET  *lpplMac;
    int16_t  yBmp;
    int16_t  dy;
    HBITMAP  hbmpXSav;
    HBITMAP  hbmpScreen;
    int16_t  id2;
    HDC      hdcScreen;
    PLANET  *lppl;
    int16_t  yMax;
    char     rgWhatsHere[999];
    HDC      hdcMem;
    THING   *lpth;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  xMin;
    HBITMAP  hbmpSav;
    int16_t  iord;
    RECT     rcClip;
    int16_t  yOff;
    RECT     rcDraw;
    int16_t  dRange;
    int16_t  idP;
    POINT    ptO;
    int16_t  fSelected;
    int16_t  fMA;
    int16_t  fStarbase;
    POINT    ptSelMain;
    THING   *lpthMac;
    int16_t  fStargate;
    uint16_t mdScanBase;
    int16_t  yMin;
    int16_t  dx;
    HBRUSH   hbrSav;
    int16_t  xLeft;
    int16_t  fDoDraw;
    POINT    ptOrigin;
    RECT     rc;
    int32_t  l;
    COLORREF crBack;
    int16_t  rgy[250];
    int16_t  fPlanetScanner;
    int16_t  rgx[250];
    int16_t  rgrad[250];
    DRAWCIR  dc;
    int16_t  dPlanRange;
    int16_t  dThingRange;
    int16_t  ropSav;
    int16_t  fDetonating;
    POINT    pt2;
    THING   *lpthDest;
    HBITMAP  hbmpTrSav;
    int16_t  fTerra;
    int16_t  dRad;
    int16_t  pctDesire;
    HBRUSH   hbr;
    int16_t  iOff;
    int16_t  xOut;
    int16_t  fConc;
    int16_t  yOut;
    int16_t  iRel;
    int32_t  lPop;
    COLORREF cr;
    uint16_t t_merge_12ec_0001;
    uint32_t t_merge_17fc_0001;
    uint32_t t_merge_1834_0001;
    int16_t  t_merge_1ca8_0001;
    int16_t  t_merge_1cd4_0001;
    int32_t  t_call_1d6a;
    uint16_t t_merge_1e3b_0001;
    uint16_t t_merge_2078_0001;
    uint32_t t_merge_20ca_0001;
    uint16_t t_merge_21cc_0001;
    THING   *t_call_270b;
    uint16_t t_merge_2916_0001;
    int16_t  t_merge_2b0c_0001;
    int16_t  t_merge_2b38_0001;
    int32_t  t_call_2bdb;
    uint16_t t_merge_2dce_0001;
    uint32_t t_merge_304c_0001;
    uint16_t t_merge_30f4_0001;
    uint16_t t_merge_32ae_0001;
    uint16_t t_merge_3314_0001;
    uint16_t t_merge_3337_0001;
    HBRUSH   t_merge_34aa_0001;
    HPEN     t_merge_34da_0001;
    HBRUSH   t_merge_359c_0001;
    HPEN     t_merge_35c8_0001;
    uint16_t t_merge_376c_0001;
    uint16_t t_merge_3783_0001;
    HBRUSH   t_merge_3b2d_0001;
    HPEN     t_merge_3b5d_0001;
    HBRUSH   t_merge_3d30_0001;
    HBRUSH   t_merge_3f8d_0001;
    int16_t  t_merge_4140_0001;
    int16_t  t_merge_416c_0001;
    int32_t  t_call_41c0;
    uint16_t t_merge_42a6_0001;
    uint16_t t_merge_42d6_0001;
    uint16_t t_merge_445f_0001;

L_108a:
    mdScanBase = (grbitScan & 0xf);
    hdcScreen = 0x0;
    hbmpScreen = 0x0;
    if ((LOWORD(rglpfl) != 0x0))
        goto L_10bb;
    else
        goto L_10b1;

L_10b1:
    if ((HIWORD(rglpfl) == 0x0))
        goto L_10d9;
    else
        goto L_10bb;

L_10bb:
    if ((gd.fNoScannerDraw != 0x0))
        goto L_10d9;
    else
        goto L_10c9;

L_10c9:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_10df;
    else
        goto L_10d9;

L_10d9:
    return 0x0;

L_10df:
    xLeft = xScanTop;
    yTop = yScanTop;
    ptOrigin.x = (PtToScan((0xfa0 - xScanTop)) & 0x7);
    ptOrigin.y = (PtToScan((0xfa0 - yScanTop)) & 0x7);
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    l = (uint32_t)((prc->right - prc->left));
    l = (uint32_t)((l * (uint32_t)((prc->bottom - prc->top))));
    if ((HIWORD(l) > 0x0))
        goto L_1287;
    else
        goto L_118a;

L_118a:
    if ((HIWORD(l) < 0x0))
        goto L_119a;
    else
        goto L_118f;

L_118f:
    if ((LOWORD(l) >= 0xbb80))
        goto L_1287;
    else
        goto L_119a;

L_119a:
    hdcMem = CreateCompatibleDC(hdc);
    if ((hdcMem == 0x0))
        goto L_11f0;
    else
        goto L_11b0;

L_11b0:
    hbmpScreen = CreateCompatibleBitmap(hdc, (prc->right - (prc->left & 0xfff8)), (prc->bottom - (prc->top & 0xfff8)));
    if ((hbmpScreen != 0x0))
        goto L_11f0;
    else
        goto L_11e7;

L_11e7:
    DeleteDC(hdcMem);

L_11f0:
    if ((hbmpScreen == 0x0))
        goto L_1287;
    else
        goto L_11f9;

L_11f9:
    hdcScreen = hdc;
    hdc = hdcMem;
    hbmpXSav = SelectObject(hdc, hbmpScreen);
    SetWindowOrg(hdc, (prc->left & 0xfff8), (prc->top & 0xfff8));
    pt.y = 0;
    pt.x = 0;
    ClientToScreen(hwndScanner, &(pt));
    ptOrigin.x = (((ptOrigin.x + 0x8) - (pt.x & 0x7)) & 0x7);
    ptOrigin.y = (((ptOrigin.y + 0x8) - (pt.y & 0x7)) & 0x7);
    rcDraw = *(prc);

L_1287:
    FillRect(hdc, prc, GetStockObject(BLACK_BRUSH));
    rcClip = *(prc);
    if ((iScanZoom < 3))
        goto L_12c5;
    else
        goto L_12bb;

L_12bb:
    if ((mdScanBase == 0x4))
        goto L_12e3;
    else
        goto L_12c5;

L_12c5:
    if ((iScanZoom < 0))
        goto L_12e9;
    else
        goto L_12cf;

L_12cf:
    if ((mdScanBase == 0x1))
        goto L_12e3;
    else
        goto L_12d9;

L_12d9:
    if ((mdScanBase != 0x2))
        goto L_12e9;
    else
        goto L_12e3;

L_12e3:
    t_merge_12ec_0001 = 0x14;
    goto L_12ec;

L_12e9:
    t_merge_12ec_0001 = 0x9;

L_12ec:
    dExpand = t_merge_12ec_0001;
    ExpandRc(prc, dExpand, dExpand);
    prc->bottom = (prc->bottom + 14);
    dx = ScanToPt((prc->right - prc->left));
    dy = ScanToPt((prc->bottom - prc->top));
    xMin = (ScanToPt(prc->left) + xLeft);
    xMax = (xMin + dx);
    yMax = ((dGalInv - yTop) - ScanToPt(prc->top));
    yMin = (yMax - dy);
    /* untranslated: xOff = (xLeft neg 0) */
    yOff = (dGalInv - yTop);
    i = 0;
    id = 1;
    goto L_13b1;

L_139f:
    i = (i + 1);
    id = (id * 2);

L_13b1:
    if ((i >= 16))
        goto L_13ea;
    else
        goto L_13bb;

L_13bb:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_139f;
    else
        goto L_13da;

L_13da:
    /* untranslated: grbitScanShip = (grbitScanShip & ((id & grbitScanShip) ~ 0x0)) */

L_13ea:
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpScanner);
    if ((sel.grobj == grobjNone))
        goto L_1423;
    else
        goto L_1411;

L_1411:
    ptSelMain.x = sel.pt.x;
    ptSelMain.y = sel.pt.y;
    goto L_1430;

L_1423:
    ptSelMain.y = -2;
    ptSelMain.x = -2;

L_1430:
    if ((gd.fFleetLinkValid != 0x0))
        goto L_1452;
    else
        goto L_1443;

L_1443:
    LinkFleets(0x0);
    goto L_14b8;

L_1452:
    i = 0;
    goto L_1460;

L_145b:
    i = (i + 1);

L_1460:
    if ((i >= cFleet))
        goto L_14b8;
    else
        goto L_146c;

L_146c:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_149f;
    else
        goto L_1497;

L_1497:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_14b8;
    else
        goto L_149f;

L_149f:
    lpfl->fDone = 0x0;
    goto L_145b;

L_14b8:
    if (((grbitScan & 0x20) == 0x0))
        goto L_1f18;
    else
        goto L_14c6;

L_14c6:
    fPlanetScanner = 0;
    dc.rgx = rgx;
    dc.rgy = rgy;
    dc.rgrad = rgrad;
    dc.cCur = 0;
    dc.cMax = 250;
    dc.hdc = hdc;
    dc.rcClip = *(prc);
    dc.fCovered = 0;
    dc.fHollowOut = 0;
    IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
    hbrSav = SelectObject(hdc, hbrRadar);
    hpenSav = SelectObject(hdc, hpenRadar);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_1677;

L_157a:
    if ((lppl->iPlayer != idPlayer))
        goto L_1673;
    else
        goto L_1586;

L_1586:

L_158c:
    dRange = GetPlanetScannerRange(lppl, &(dPlanRange));
    if ((dPlanRange <= 0))
        goto L_15b3;
    else
        goto L_15ad;

L_15ad:
    fPlanetScanner = 1;

L_15b3:
    if ((vpctRadarView >= 100))
        goto L_15d2;
    else
        goto L_15bd;

L_15bd:
    dRange = MulDiv(dRange, vpctRadarView, 0x64);

L_15d2:
    id = lppl->id;
    rc.left = PtToScan(((xOff + rgptPlan[id].x) - dRange));
    rc.top = PtToScan(((yOff - rgptPlan[id].y) - dRange));
    rc.right = PtToScan(((xOff + rgptPlan[id].x) + dRange));
    rc.bottom = PtToScan(((yOff - rgptPlan[id].y) + dRange));
    DrawRadarCircle(&(dc), &(rc));

L_1673:
    lppl = (lppl + 0x1);

L_1677:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_157a;
    else
        goto L_1685;

L_1685:
    i = 0;
    goto L_1693;

L_168e:
    i = (i + 1);

L_1693:
    if ((i >= cFleet))
        goto L_17c8;
    else
        goto L_169f;

L_169f:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_16d2;
    else
        goto L_16ca;

L_16ca:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_17c8;
    else
        goto L_16d2;

L_16d2:
    if ((lpfl->iPlayer != idPlayer))
        goto L_168e;
    else
        goto L_16df;

L_16df:

L_16e5:
    dRange = GetFleetScannerRange(lpfl, &(dPlanRange), 0x0, 0x0);
    if ((dPlanRange <= 0))
        goto L_1715;
    else
        goto L_1710;

L_1710:
    fPlanetScanner = (fPlanetScanner | 0x2);

L_1715:
    if ((dRange <= 0))
        goto L_168e;
    else
        goto L_171c;

L_171c:

L_1722:
    if ((vpctRadarView >= 100))
        goto L_1741;
    else
        goto L_172c;

L_172c:
    dRange = MulDiv(dRange, vpctRadarView, 0x64);

L_1741:
    rc.left = PtToScan(((xOff + lpfl->pt.x) - dRange));
    rc.top = PtToScan(((yOff - lpfl->pt.y) - dRange));
    rc.right = PtToScan(((xOff + lpfl->pt.x) + dRange));
    rc.bottom = PtToScan(((yOff - lpfl->pt.y) + dRange));
    DrawRadarCircle(&(dc), &(rc));
    goto L_168e;

L_17c8:
    DrawRadarCircle(&(dc), 0x0);
    if ((hbrRadarNear != 0x0))
        goto L_1809;
    else
        goto L_17e3;

L_17e3:
    if ((vcScreenColors > 8))
        goto L_17f6;
    else
        goto L_17ed;

L_17ed:
    t_merge_17fc_0001 = 0x7f7f;
    goto L_17fc;

L_17f6:
    t_merge_17fc_0001 = 0x6060;

L_17fc:
    hbrRadarNear = HbrGet(t_merge_17fc_0001);

L_1809:
    if ((hpenRadarNear != 0x0))
        goto L_183e;
    else
        goto L_1813;

L_1813:
    if ((vcScreenColors > 8))
        goto L_182e;
    else
        goto L_1825;

L_1825:
    t_merge_1834_0001 = 0x7f7f;
    goto L_1834;

L_182e:
    t_merge_1834_0001 = 0x6060;

L_1834:
    hpenRadarNear = CreatePen(0x0, 0x1, t_merge_1834_0001);

L_183e:
    SelectObject(hdc, hbrRadarNear);
    SelectObject(hdc, hpenRadarNear);
    if (((fPlanetScanner & 0x1) == 0x0))
        goto L_19a3;
    else
        goto L_1865;

L_1865:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_1995;

L_1894:
    if ((lppl->iPlayer != idPlayer))
        goto L_1991;
    else
        goto L_18a0;

L_18a0:

L_18a6:
    dRange = GetPlanetScannerRange(lppl, &(dPlanRange));
    if ((dPlanRange <= 0))
        goto L_1991;
    else
        goto L_18c4;

L_18c4:

L_18ca:
    dRange = (dRange >> 0x1);
    if ((vpctRadarView >= 100))
        goto L_18f0;
    else
        goto L_18db;

L_18db:
    dRange = MulDiv(dRange, vpctRadarView, 0x64);

L_18f0:
    id = lppl->id;
    rc.left = PtToScan(((xOff + rgptPlan[id].x) - dRange));
    rc.top = PtToScan(((yOff - rgptPlan[id].y) - dRange));
    rc.right = PtToScan(((xOff + rgptPlan[id].x) + dRange));
    rc.bottom = PtToScan(((yOff - rgptPlan[id].y) + dRange));
    DrawRadarCircle(&(dc), &(rc));

L_1991:
    lppl = (lppl + 0x1);

L_1995:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1894;
    else
        goto L_19a3;

L_19a3:
    if (((fPlanetScanner & 0x2) == 0x0))
        goto L_1ae6;
    else
        goto L_19b2;

L_19b2:
    i = 0;
    goto L_19c0;

L_19bb:
    i = (i + 1);

L_19c0:
    if ((i >= cFleet))
        goto L_1ae6;
    else
        goto L_19cc;

L_19cc:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_19ff;
    else
        goto L_19f7;

L_19f7:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_1ae6;
    else
        goto L_19ff;

L_19ff:
    if ((lpfl->iPlayer != idPlayer))
        goto L_19bb;
    else
        goto L_1a0c;

L_1a0c:

L_1a12:
    dRange = GetFleetScannerRange(lpfl, &(dPlanRange), 0x0, 0x0);
    if ((dPlanRange <= 0))
        goto L_19bb;
    else
        goto L_1a3a;

L_1a3a:

L_1a40:
    if ((vpctRadarView >= 100))
        goto L_1a5f;
    else
        goto L_1a4a;

L_1a4a:
    dPlanRange = MulDiv(dPlanRange, vpctRadarView, 0x64);

L_1a5f:
    rc.left = PtToScan(((xOff + lpfl->pt.x) - dPlanRange));
    rc.top = PtToScan(((yOff - lpfl->pt.y) - dPlanRange));
    rc.right = PtToScan(((xOff + lpfl->pt.x) + dPlanRange));
    rc.bottom = PtToScan(((yOff - lpfl->pt.y) + dPlanRange));
    DrawRadarCircle(&(dc), &(rc));
    goto L_19bb;

L_1ae6:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMassAccel))
        goto L_1c62;
    else
        goto L_1b07;

L_1b07:
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_1c51;

L_1b3a:
    if ((lpth->ith != ithMineralPacket))
        goto L_1c4c;
    else
        goto L_1b51;

L_1b51:
    if ((lpth->iplr != idPlayer))
        goto L_1c4c;
    else
        goto L_1b6b;

L_1b6b:
    if ((lpth->thp.iWarp == 0x0))
        goto L_1c4c;
    else
        goto L_1b80;

L_1b80:

L_1b86:
    dThingRange = (lpth->thp.iWarp + 4);
    dThingRange = LOWORD((dThingRange * dThingRange));
    if ((vpctRadarView >= 100))
        goto L_1bc8;
    else
        goto L_1bb3;

L_1bb3:
    dThingRange = MulDiv(dThingRange, vpctRadarView, 0x64);

L_1bc8:
    rc.left = PtToScan(((xOff + lpth->pt.x) - dThingRange));
    rc.top = PtToScan(((yOff - lpth->pt.y) - dThingRange));
    rc.right = PtToScan(((xOff + lpth->pt.x) + dThingRange));
    rc.bottom = PtToScan(((yOff - lpth->pt.y) + dThingRange));
    DrawRadarCircle(&(dc), &(rc));

L_1c4c:
    lpth = (lpth + 0x1);

L_1c51:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_1b3a;
    else
        goto L_1c62;

L_1c62:
    DrawRadarCircle(&(dc), 0x0);
    SelectObject(hdc, hbrRadar);
    SelectObject(hdc, hpenRadar);
    if ((mdScanBase == 0x5))
        goto L_1f01;
    else
        goto L_1c95;

L_1c95:
    if ((sel.grobj != grobjFleet))
        goto L_1ca5;
    else
        goto L_1c9f;

L_1c9f:
    t_merge_1ca8_0001 = sel.fl.id;
    goto L_1ca8;

L_1ca5:
    t_merge_1ca8_0001 = -1;

L_1ca8:
    id = t_merge_1ca8_0001;
    if ((sel.scan.grobj != grobjFleet))
        goto L_1cd1;
    else
        goto L_1cb5;

L_1cb5:
    t_merge_1cd4_0001 = rglpfl[sel.scan.ifl]->id;
    goto L_1cd4;

L_1cd1:
    t_merge_1cd4_0001 = -1;

L_1cd4:
    id2 = t_merge_1cd4_0001;
    SelectObject(hdcMem, hbmpScanShip);
    SetTextColor(hdc, 0x0);
    SetBkColor(hdc, 0xffffff);
    i = 0;
    goto L_1d12;

L_1d0d:
    i = (i + 1);

L_1d12:
    if ((i >= cFleet))
        goto L_1ef4;
    else
        goto L_1d1e;

L_1d1e:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_1d51;
    else
        goto L_1d49;

L_1d49:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_1ef4;
    else
        goto L_1d51;

L_1d51:
    if ((lpfl->idPlanet != -1))
        goto L_1d0d;
    else
        goto L_1d5c;

L_1d5c:

L_1d62:
    t_call_1d6a = CShipsScanVis(lpfl);
    if ((HIWORD(t_call_1d6a) > 0x0))
        goto L_1da8;
    else
        goto L_1d7a;

L_1d7a:
    if ((HIWORD(t_call_1d6a) < 0x0))
        goto L_1d87;
    else
        goto L_1d7f;

L_1d7f:
    if ((LOWORD(t_call_1d6a) > 0x0))
        goto L_1da8;
    else
        goto L_1d87;

L_1d87:
    if ((lpfl->id == id))
        goto L_1da8;
    else
        goto L_1d96;

L_1d96:
    if ((lpfl->id != id2))
        goto L_1d0d;
    else
        goto L_1da2;

L_1da2:

L_1da8:
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    if ((pt.x < xMin))
        goto L_1d0d;
    else
        goto L_1dc6;

L_1dc6:
    if ((pt.x >= xMax))
        goto L_1d0d;
    else
        goto L_1dd1;

L_1dd1:
    if ((pt.y < yMin))
        goto L_1d0d;
    else
        goto L_1ddd;

L_1ddd:
    if ((pt.y >= yMax))
        goto L_1d0d;
    else
        goto L_1de8;

L_1de8:
    pt.x = PtToScan((xOff + pt.x));
    pt.y = PtToScan((yOff - pt.y));
    /* untranslated: branch lpfl->pt.x != ss:[ptSelMain] ? L_1e38 : L_1e29 */

L_1e29:
    /* untranslated: branch lpfl->pt.y != ss:[ptSelMain+0x2] ? L_1e38 : L_1e32 */

L_1e32:
    t_merge_1e3b_0001 = 0x1;
    goto L_1e3b;

L_1e38:
    t_merge_1e3b_0001 = 0x0;

L_1e3b:
    fSelected = t_merge_1e3b_0001;
    if ((fSelected == 0))
        goto L_1e98;
    else
        goto L_1e49;

L_1e49:
    SelectObject(hdcMem, hbmpScanner);
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0xb, 0x50, SRCAND);
    SelectObject(hdcMem, hbmpScanShip);
    goto L_1d0d;

L_1e98:
    GetScanFleetOrientation(lpfl, &(ptO), &(ptD));
    BitBlt(hdc, (pt.x - ((uint32_t)(ptD.x) / 2)), (pt.y - ((uint32_t)(ptD.y) / 2)), ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCAND);

L_1ef4:
    SelectObject(hdcMem, hbmpScanner);

L_1f01:
    SelectObject(hdc, hpenSav);
    SelectObject(hdc, hbrSav);

L_1f18:
    if (((grbitScan & 0x40) == 0x0))
        goto L_25da;
    else
        goto L_1f26;

L_1f26:
    dc.rgx = rgx;
    dc.rgy = rgy;
    dc.rgrad = rgrad;
    dc.cCur = 0;
    dc.cMax = 250;
    dc.hdc = hdc;
    dc.rcClip = *(prc);
    dc.fCovered = 0;
    dc.fHollowOut = 1;
    i = 0;
    goto L_1f8b;

L_1f77:
    UnrealizeObject(rghbrPat[i]);
    i = (i + 1);

L_1f8b:
    if ((i < 3))
        goto L_1f77;
    else
        goto L_1f95;

L_1f95:
    SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
    IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
    hbrSav = SelectObject(hdc, rghbrPat[0]);
    hpenSav = SelectObject(hdc, GetStockObject(NULL_PEN));
    ropSav = GetROP2(hdc);
    SetBkColor(hdc, 0x0);
    j = 0;
    goto L_2479;

L_2006:
    if ((j != 0))
        goto L_201d;
    else
        goto L_200f;

L_200f:
    if (((grbitScanMines & 0x1) == 0x0))
        goto L_2475;
    else
        goto L_201d;

L_201d:
    if ((j != 1))
        goto L_2034;
    else
        goto L_2026;

L_2026:
    if (((grbitScanMines & 0x2) == 0x0))
        goto L_2475;
    else
        goto L_2034;

L_2034:
    if ((j != 2))
        goto L_204e;
    else
        goto L_203d;

L_203d:
    if (((grbitScanMines & 0xc) == 0x0))
        goto L_2475;
    else
        goto L_2048;

L_2048:

L_204e:
    i = 0;
    goto L_246b;

L_2057:
    fDetonating = 0;
    goto L_2065;

L_2060:
    fDetonating = (fDetonating + 1);

L_2065:
    if ((i != 0))
        goto L_2075;
    else
        goto L_206f;

L_206f:
    t_merge_2078_0001 = 0x1;
    goto L_2078;

L_2075:
    t_merge_2078_0001 = 0x0;

L_2078:
    if ((fDetonating > t_merge_2078_0001))
        goto L_2466;
    else
        goto L_2081;

L_2081:
    SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
    SelectObject(hdc, rghbrPat[i]);
    if ((fDetonating == 0))
        goto L_20b9;
    else
        goto L_20b0;

L_20b0:
    t_merge_20ca_0001 = 0xff00ff;
    goto L_20ca;

L_20b9:
    t_merge_20ca_0001 = rgcrScanMine[j];

L_20ca:
    SetTextColor(hdc, t_merge_20ca_0001);
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_2441;

L_2104:
    if ((lpth->ith != ithMinefield))
        goto L_243c;
    else
        goto L_211b;

L_211b:
    if ((lpth->thm.iType != i))
        goto L_243c;
    else
        goto L_212c;

L_212c:

L_2132:
    if ((j != 0))
        goto L_2155;
    else
        goto L_213b;

L_213b:
    if ((lpth->iplr != idPlayer))
        goto L_243c;
    else
        goto L_2155;

L_2155:
    if ((j < 1))
        goto L_21da;
    else
        goto L_215e;

L_215e:
    if ((lpth->iplr == idPlayer))
        goto L_243c;
    else
        goto L_2178;

L_2178:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lpth->iplr)]) != 0x1 ? L_21b3 : L_21ad */

L_21ad:
    goto L_21b6;

L_21b3:

L_21b6:
    if ((j != 2))
        goto L_21c9;
    else
        goto L_21c3;

L_21c3:
    t_merge_21cc_0001 = 0x1;
    goto L_21cc;

L_21c9:
    t_merge_21cc_0001 = 0x0;

L_21cc:
    /* untranslated: branch ss:[bp-0xa78] == t_merge_21cc_0001 ? L_243c : L_21d4 */

L_21d4:

L_21da:
    if ((j != 2))
        goto L_224b;
    else
        goto L_21e3;

L_21e3:
    if (((grbitScanMines & 0xc) == 0xc))
        goto L_224b;
    else
        goto L_21f1;

L_21f1:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lpth->iplr)]) != 0x0 ? L_223a : L_2226 */

L_2226:
    if (((grbitScanMines & 0x4) == 0x0))
        goto L_243c;
    else
        goto L_2231;

L_2231:

L_223a:
    if (((grbitScanMines & 0x8) == 0x0))
        goto L_243c;
    else
        goto L_2245;

L_2245:

L_224b:
    if ((lpth->thm.fDetonate != fDetonating))
        goto L_243c;
    else
        goto L_225c;

L_225c:

L_2262:
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    sqrt((double)(lpth->thm.cMines));
    dRange = LOWORD(__ftol());
    rc.left = PtToScan(((xOff + pt.x) - dRange));
    rc.top = PtToScan(((yOff - pt.y) - dRange));
    rc.right = PtToScan(((xOff + pt.x) + dRange));
    rc.bottom = PtToScan(((yOff - pt.y) + dRange));
    DrawRadarCircle(&(dc), &(rc));
    id = 0;
    goto L_231d;

L_2319:
    id = (id + 1);

L_231d:
    if ((id >= game.cPlanMax))
        goto L_234d;
    else
        goto L_2328;

L_2328:
    if ((rgptPlan[id].x != pt.x))
        goto L_2319;
    else
        goto L_233e;

L_233e:
    if ((rgptPlan[id].y == pt.y))
        goto L_234d;
    else
        goto L_2344;

L_2344:

L_234d:
    if ((id != game.cPlanMax))
        goto L_243c;
    else
        goto L_2358;

L_2358:
    pt.x = PtToScan((xOff + pt.x));
    pt.y = PtToScan((yOff - pt.y));
    dRange = PtToScan(0x1);
    if ((dRange >= 1))
        goto L_23a0;
    else
        goto L_2397;

L_2397:
    dRange = 1;
    goto L_23b0;

L_23a0:
    if ((dRange <= 3))
        goto L_23b0;
    else
        goto L_23aa;

L_23aa:
    dRange = 3;

L_23b0:
    SetRect(&(rc), (pt.x - dRange), (pt.y - dRange), ((pt.x + dRange) + 0x1), ((pt.y + dRange) + 0x1));
    SetBkColor(hdc, rgcrScanMine[j]);
    ExtTextOut(hdc, rc.left, rc.top, 0x2, &(rc), 0x0, 0x0, 0x0);
    SetBkColor(hdc, 0x0);

L_243c:
    lpth = (lpth + 0x1);

L_2441:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_2104;
    else
        goto L_2452;

L_2452:
    DrawRadarCircle(&(dc), 0x0);
    goto L_2060;

L_2466:
    i = (i + 1);

L_246b:
    if ((i < 3))
        goto L_2057;
    else
        goto L_2475;

L_2475:
    j = (j + 1);

L_2479:
    if ((j < 3))
        goto L_2006;
    else
        goto L_2482;

L_2482:
    if ((sel.scan.grobj != grobjThing))
        goto L_25b7;
    else
        goto L_248c;

L_248c:
    lpth = &(lpThings[sel.scan.ith]);
    if ((lpth->ith != ithMinefield))
        goto L_25b7;
    else
        goto L_24bc;

L_24bc:
    SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
    SelectObject(hdc, rghbrPat[lpth->thm.iType]);
    SetTextColor(hdc, 0xffff00);
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    sqrt((double)(lpth->thm.cMines));
    dRange = LOWORD(__ftol());
    rc.left = PtToScan(((xOff + pt.x) - dRange));
    rc.top = PtToScan(((yOff - pt.y) - dRange));
    rc.right = PtToScan(((xOff + pt.x) + dRange));
    rc.bottom = PtToScan(((yOff - pt.y) + dRange));
    DrawRadarCircle(&(dc), &(rc));
    DrawRadarCircle(&(dc), 0x0);

L_25b7:
    SetROP2(hdc, ropSav);
    SelectObject(hdc, hpenSav);
    SelectObject(hdc, hbrSav);

L_25da:
    if ((cThing == 0))
        goto L_2ae1;
    else
        goto L_25e4;

L_25e4:
    if ((mdScanBase == 0x5))
        goto L_2ae1;
    else
        goto L_25ee;

L_25ee:
    IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
    hbrSav = SelectObject(hdc, hbrShip);
    hpenSav = SelectObject(hdc, hpenDkPurple);
    lpth = lpThings;
    lpthMac = &(lpThings[cThing]);
    goto L_2ab9;

L_2658:
    if ((lpth->ith == ithMineralPacket))
        goto L_26a0;
    else
        goto L_266f;

L_266f:
    if ((lpth->ith == ithWormhole))
        goto L_26a0;
    else
        goto L_2686;

L_2686:
    if ((lpth->ith != ithMysteryTrader))
        goto L_2ab4;
    else
        goto L_269a;

L_269a:

L_26a0:
    pt.x = lpth->pt.x;
    pt.y = lpth->pt.y;
    LogicalToScan(&(pt));
    if ((lpth->ith != ithWormhole))
        goto L_277e;
    else
        goto L_26d5;

L_26d5:
    if ((lpth->idFull >= lpth->thw.idPartner))
        goto L_277e;
    else
        goto L_26e9;

L_26e9:
    if ((((0x1 << idPlayer) & lpth->thw.grbitPlrTrav) == 0x0))
        goto L_277e;
    else
        goto L_2702;

L_2702:
    t_call_270b = LpthFromId(lpth->thw.idPartner);
    lpthDest = t_call_270b;
    if ((FAROFF(t_call_270b) != 0x0))
        goto L_272b;
    else
        goto L_2723;

L_2723:
    if ((FARSEG(t_call_270b) == 0x0))
        goto L_277e;
    else
        goto L_272b;

L_272b:
    MoveTo(hdc, pt.x, pt.y);
    pt2.x = lpthDest->pt.x;
    pt2.y = lpthDest->pt.y;
    LogicalToScan(&(pt2));
    LineTo(hdc, pt2.x, pt2.y);
    LineTo(hdc, pt2.x, (pt2.y + 1));

L_277e:
    if ((lpth->ith != ithWormhole))
        goto L_27fc;
    else
        goto L_2795;

L_2795:
    BitBlt(hdc, (pt.x - 4), (pt.y - 4), 0x9, 0x9, hdcMem, 0x9, 0x5c, SRCAND);
    BitBlt(hdc, (pt.x - 4), (pt.y - 4), 0x9, 0x9, hdcMem, 0x0, 0x5c, SRCPAINT);
    goto L_2ab4;

L_27fc:
    if ((lpth->ith != ithMysteryTrader))
        goto L_28f3;
    else
        goto L_2813;

L_2813:
    hbmpTrSav = SelectObject(hdcMem, hbmpScanShip);
    crFore = SetTextColor(hdc, 0xffff00);
    crBack = SetBkColor(hdc, 0x0);
    GetDxDyOrientation((lpth->tht.ptDest.x - lpth->pt.x), (lpth->tht.ptDest.y - lpth->pt.y), &(ptO), &(ptD));
    BitBlt(hdc, (pt.x - ((uint32_t)(ptD.x) / 2)), (pt.y - ((uint32_t)(ptD.y) / 2)), ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCPAINT);
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    SelectObject(hdcMem, hbmpTrSav);
    goto L_2ab4;

L_28f3:
    if ((iScanZoom <= 0))
        goto L_2913;
    else
        goto L_28fd;

L_28fd:
    if ((iScanZoom <= 2))
        goto L_290d;
    else
        goto L_2907;

L_2907:
    t_merge_2916_0001 = 0x5;
    goto L_2916;

L_290d:
    t_merge_2916_0001 = 0x3;

L_2913:
    t_merge_2916_0001 = 0x2;

L_2916:
    dRange = t_merge_2916_0001;
    dx = ((dRange * 2) + 1);
    if ((lpth->thp.iWarp != 0x0))
        goto L_29c8;
    else
        goto L_293f;

L_293f:
    SelectObject(hdc, hpenYellow);
    MoveTo(hdc, pt.x, ((pt.y - dRange) + 0xffff));
    LineTo(hdc, ((pt.x - dRange) + 0xffff), pt.y);
    LineTo(hdc, pt.x, ((pt.y + dRange) + 0x1));
    LineTo(hdc, ((pt.x + dRange) + 0x1), pt.y);
    LineTo(hdc, pt.x, ((pt.y - dRange) + 0xffff));
    SelectObject(hdc, hpenDkPurple);
    goto L_2ab4;

L_29c8:
    if ((lpth->iplr == idPlayer))
        goto L_29ee;
    else
        goto L_29e2;

L_29e2:
    SelectObject(hdc, hbrRed);

L_29ee:
    PatBlt(hdc, (pt.x - dRange), (pt.y - dRange), dx, 0x1, PATCOPY);
    PatBlt(hdc, (pt.x - dRange), (pt.y - dRange), 0x1, dx, PATCOPY);
    PatBlt(hdc, (pt.x - dRange), (pt.y + dRange), dx, 0x1, PATCOPY);
    PatBlt(hdc, (pt.x + dRange), (pt.y - dRange), 0x1, dx, PATCOPY);
    if ((lpth->iplr == idPlayer))
        goto L_2ab4;
    else
        goto L_2aa8;

L_2aa8:
    SelectObject(hdc, hbrShip);

L_2ab4:
    lpth = (lpth + 0x1);

L_2ab9:
    if ((LOWORD(lpth) < LOWORD(lpthMac)))
        goto L_2658;
    else
        goto L_2aca;

L_2aca:
    SelectObject(hdc, hbrSav);
    SelectObject(hdc, hpenSav);

L_2ae1:
    if (((grbitScan & 0x80) == 0x0))
        goto L_2cc0;
    else
        goto L_2aef;

L_2aef:
    if ((mdScanBase == 0x5))
        goto L_2cc0;
    else
        goto L_2af9;

L_2af9:
    if ((sel.grobj != grobjFleet))
        goto L_2b09;
    else
        goto L_2b03;

L_2b03:
    t_merge_2b0c_0001 = sel.fl.id;
    goto L_2b0c;

L_2b09:
    t_merge_2b0c_0001 = -1;

L_2b0c:
    id = t_merge_2b0c_0001;
    if ((sel.scan.grobj != grobjFleet))
        goto L_2b35;
    else
        goto L_2b19;

L_2b19:
    t_merge_2b38_0001 = rglpfl[sel.scan.ifl]->id;
    goto L_2b38;

L_2b35:
    t_merge_2b38_0001 = -1;

L_2b38:
    id2 = t_merge_2b38_0001;
    hpenSav = SelectObject(hdc, hpenStarbase);
    i = 0;
    goto L_2b58;

L_2b53:
    i = (i + 1);

L_2b58:
    if ((i >= cFleet))
        goto L_2cb5;
    else
        goto L_2b64;

L_2b64:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2b97;
    else
        goto L_2b8f;

L_2b8f:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_2cb5;
    else
        goto L_2b97;

L_2b97:
    if ((lpfl->fDead != 0x0))
        goto L_2b53;
    else
        goto L_2baf;

L_2baf:
    if ((lpfl->det < 0x7))
        goto L_2b53;
    else
        goto L_2bc2;

L_2bc2:
    if ((lpfl->cord <= 1))
        goto L_2b53;
    else
        goto L_2bcd;

L_2bcd:

L_2bd3:
    t_call_2bdb = CShipsScanVis(lpfl);
    if ((HIWORD(t_call_2bdb) > 0x0))
        goto L_2c19;
    else
        goto L_2beb;

L_2beb:
    if ((HIWORD(t_call_2bdb) < 0x0))
        goto L_2bf8;
    else
        goto L_2bf0;

L_2bf0:
    if ((LOWORD(t_call_2bdb) > 0x0))
        goto L_2c19;
    else
        goto L_2bf8;

L_2bf8:
    if ((lpfl->id == id))
        goto L_2c19;
    else
        goto L_2c07;

L_2c07:
    if ((lpfl->id != id2))
        goto L_2b53;
    else
        goto L_2c13;

L_2c13:

L_2c19:
    iord = 0;
    goto L_2c27;

L_2c22:
    iord = (iord + 1);

L_2c27:
    if ((iord >= lpfl->cord))
        goto L_2b53;
    else
        goto L_2c38;

L_2c38:
    pt.x = lpfl->lpplord->rgord[iord].pt.x;
    pt.y = lpfl->lpplord->rgord[iord].pt.y;
    pt.x = PtToScan((xOff + pt.x));
    pt.y = PtToScan((yOff - pt.y));
    if ((iord != 0))
        goto L_2ca1;
    else
        goto L_2c90;

L_2c90:
    MoveTo(hdc, pt.x, pt.y);
    goto L_2c22;

L_2ca1:
    LineTo(hdc, pt.x, pt.y);

L_2cb5:
    SelectObject(hdc, hpenSav);

L_2cc0:
    SelectClipRgn(hdc, hrgnHuge);
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    memset(rgWhatsHere, 0x0, 0x3e7);
    goto L_2d54;
    SelectObject(hdc, rghfontArial10[1]);
    goto L_2d74;
    SelectObject(hdc, rghfontArial8[1]);
    goto L_2d74;
    SelectObject(hdc, rghfontArial8[0]);
    goto L_2d74;
    SelectObject(hdc, rghfontArial6);
    goto L_2d74;

L_2d54:

L_2d74:
    crFore = SetTextColor(hdc, 0xffffff);
    crBack = SetBkColor(hdc, 0x0);
    iBkPrev = SetBkMode(hdc, TRANSPARENT);
    if ((iScanZoom < 3))
        goto L_2dcb;
    else
        goto L_2dbb;

L_2dbb:
    if ((mdScanBase != 0x4))
        goto L_2dcb;
    else
        goto L_2dc5;

L_2dc5:
    t_merge_2dce_0001 = 0xb;
    goto L_2dce;

L_2dcb:
    t_merge_2dce_0001 = 0x0;

L_2dce:
    j = t_merge_2dce_0001;
    i = 0;
    goto L_2ddf;

L_2dda:
    i = (i + 1);

L_2ddf:
    if ((i >= game.cPlanMax))
        goto L_30ae;
    else
        goto L_2deb;

L_2deb:
    if ((rgptPlan[i].x < xMin))
        goto L_3094;
    else
        goto L_2e00;

L_2e00:
    if ((rgptPlan[i].x >= xMax))
        goto L_3094;
    else
        goto L_2e14;

L_2e14:
    if ((rgptPlan[i].y < yMin))
        goto L_3094;
    else
        goto L_2e2d;

L_2e2d:
    if ((rgptPlan[i].y >= yMax))
        goto L_3094;
    else
        goto L_2e45;

L_2e45:
    fDoDraw = 1;

LBailIn:
    pt.x = PtToScan((xOff + rgptPlan[i].x));
    pt.y = PtToScan((yOff - rgptPlan[i].y));
    if ((fDoDraw == 0))
        goto L_2f55;
    else
        goto L_2e90;

L_2e90:
    if ((ptSelMain.x != rgptPlan[i].x))
        goto L_2f23;
    else
        goto L_2ea9;

L_2ea9:
    if ((ptSelMain.y != rgptPlan[i].y))
        goto L_2f23;
    else
        goto L_2eb2;

L_2eb2:
    if ((mdScanBase > 0x2))
        goto L_2f23;
    else
        goto L_2ebc;

L_2ebc:
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x0, 0x45, SRCAND);
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x0, 0x21, SRCPAINT);
    goto L_2f55;

L_2f23:
    BitBlt(hdc, (pt.x - 1), (pt.y - 1), 0x3, 0x3, hdcMem, 0xb, 0xf, SRCCOPY);

L_2f55:
    if (((grbitScan & 0x400) == 0x0))
        goto L_2dda;
    else
        goto L_2f63;

L_2f63:
    if ((iScanZoom < -1))
        goto L_2dda;
    else
        goto L_2f6d;

L_2f6d:
    if ((rgptPlan[i].x < (xMin - 50)))
        goto L_2dda;
    else
        goto L_2f85;

L_2f85:
    if ((rgptPlan[i].x >= (xMax + 50)))
        goto L_2dda;
    else
        goto L_2f9c;

L_2f9c:
    if ((rgptPlan[i].y < (yMin - 20)))
        goto L_2dda;
    else
        goto L_2fb8;

L_2fb8:
    if ((rgptPlan[i].y >= (yMax + 20)))
        goto L_2dda;
    else
        goto L_2fd3;

L_2fd3:
    PszGetPlanetName(i);
    if (((grbitScan & 0x2000) == 0x0))
        goto L_3053;
    else
        goto L_2fed;

L_2fed:
    lppl = LpplFromId(i);
    if ((LOWORD(lppl) != 0x0))
        goto L_3011;
    else
        goto L_3008;

L_3008:
    if ((HIWORD(lppl) == 0x0))
        goto L_3053;
    else
        goto L_3011;

L_3011:
    if ((lppl->iPlayer == -1))
        goto L_3053;
    else
        goto L_301e;

L_301e:
    if ((lppl->iPlayer != idPlayer))
        goto L_3039;
    else
        goto L_3030;

L_3030:
    t_merge_304c_0001 = 0xffffff;
    goto L_304c;

L_3039:
    t_merge_304c_0001 = rgcrPlrHistory[lppl->iPlayer];

L_304c:
    SetTextColor(hdc, t_merge_304c_0001);

L_3053:
    CtrTextOut(hdc, pt.x, ((pt.y + 5) + j), szWork, 0x0);
    if (((grbitScan & 0x2000) == 0x0))
        goto L_2dda;
    else
        goto L_3081;

L_3081:
    SetTextColor(hdc, 0xffffff);

L_3094:
    if (((grbitScan & 0x400) == 0x0))
        goto L_2dda;
    else
        goto L_30a2;

L_30a2:
    fDoDraw = 0;
    goto LBailIn;

L_30ae:
    SetBkMode(hdc, iBkPrev);
    SetBkColor(hdc, crBack);
    SetTextColor(hdc, crFore);
    if ((iScanZoom < 3))
        goto L_30f1;
    else
        goto L_30e1;

L_30e1:
    if ((mdScanBase != 0x4))
        goto L_30f1;
    else
        goto L_30eb;

L_30eb:
    t_merge_30f4_0001 = 0xb;
    goto L_30f4;

L_30f1:
    t_merge_30f4_0001 = 0x0;

L_30f4:
    j = t_merge_30f4_0001;
    if ((sel.grobj == grobjNone))
        goto L_31a0;
    else
        goto L_3101;

L_3101:
    if ((sel.pt.x != sel.scan.pt.x))
        goto L_31a0;
    else
        goto L_3111;

L_3111:
    if ((sel.pt.y != sel.scan.pt.y))
        goto L_31a0;
    else
        goto L_311a;

L_311a:
    pt.x = sel.pt.x;
    pt.y = sel.pt.y;
    LogicalToScan(&(pt));
    BitBlt(hdc, (pt.x - 5), ((pt.y + 11) + j), 0xb, 0xc, hdcMem, 0x0, 0x50, SRCAND);
    BitBlt(hdc, (pt.x - 5), ((pt.y + 11) + j), 0xb, 0xc, hdcMem, 0x0, 0x39, SRCPAINT);
    goto L_322d;

L_31a0:
    if ((sel.scan.grobj == grobjNone))
        goto L_322d;
    else
        goto L_31aa;

L_31aa:
    pt.x = sel.scan.pt.x;
    pt.y = sel.scan.pt.y;
    LogicalToScan(&(pt));
    BitBlt(hdc, (pt.x - 3), ((pt.y + 7) + j), 0x7, 0x8, hdcMem, 0x16, 0x50, SRCAND);
    BitBlt(hdc, (pt.x - 3), ((pt.y + 7) + j), 0x7, 0x8, hdcMem, 0x16, 0x31, SRCPAINT);

L_322d:
    if ((cPlanet == 0))
        goto L_40f8;
    else
        goto L_3237;

L_3237:
    if ((mdScanBase == 0x5))
        goto L_40f8;
    else
        goto L_3241;

L_3241:
    lppl = lpPlanets;
    i = 0;
    goto L_326c;

L_3257:
    i = (i + 1);
    lppl = (lppl + 0x1);

L_326c:
    if ((i >= cPlanet))
        goto L_40f8;
    else
        goto L_3278;

L_3278:
    id = lppl->id;
    if ((lppl->fStarbase == 0x0))
        goto L_32ab;
    else
        goto L_3298;

L_3298:
    if ((lppl->iPlayer == -1))
        goto L_32ab;
    else
        goto L_32a5;

L_32a5:
    t_merge_32ae_0001 = 0x1;
    goto L_32ae;

L_32ab:
    t_merge_32ae_0001 = 0x0;

L_32ae:
    fStarbase = t_merge_32ae_0001;
    if ((fStarbase == 0))
        goto L_333e;
    else
        goto L_32bc;

L_32bc:
    if ((rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef != ihuldefOrbitalFort))
        goto L_32f1;
    else
        goto L_32eb;

L_32eb:
    fStarbase = 2;

L_32f1:
    if ((IWarpMAFromLppl(lppl, 0x0) <= 0))
        goto L_3311;
    else
        goto L_330b;

L_330b:
    t_merge_3314_0001 = 0x1;
    goto L_3314;

L_3311:
    t_merge_3314_0001 = 0x0;

L_3314:
    fMA = t_merge_3314_0001;
    if ((IStargateFromLppl(lppl) == -1))
        goto L_3334;
    else
        goto L_332e;

L_332e:
    t_merge_3337_0001 = 0x1;
    goto L_3337;

L_3334:
    t_merge_3337_0001 = 0x0;

L_3337:
    fStargate = t_merge_3337_0001;
    goto L_334b;

L_333e:
    fMA = 0;
    fStargate = 0;

L_334b:
    if ((rgptPlan[id].x < xMin))
        goto L_3257;
    else
        goto L_335f;

L_335f:
    if ((rgptPlan[id].x >= xMax))
        goto L_3257;
    else
        goto L_3372;

L_3372:
    if ((rgptPlan[id].y < yMin))
        goto L_3257;
    else
        goto L_338a;

L_338a:
    if ((rgptPlan[id].y >= yMax))
        goto L_3257;
    else
        goto L_33a1;

L_33a1:
    pt.x = PtToScan((xOff + rgptPlan[id].x));
    pt.y = PtToScan((yOff - rgptPlan[id].y));
    if ((mdScanBase != 0x3))
        goto L_3745;
    else
        goto L_33e4;

L_33e4:
    fTerra = 0;
    if ((lppl->det < 0x3))
        goto L_3257;
    else
        goto L_33fc;

L_33fc:
    pctDesire = PctPlanetDesirability(lppl, idPlayer);
    if ((pctDesire < 0))
        goto L_343d;
    else
        goto L_341c;

L_341c:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raTerra))
        goto L_3484;
    else
        goto L_343d;

L_343d:
    pctDesire = PctPlanetOptValue(lppl, idPlayer);
    if ((pctDesire < 0))
        goto L_3484;
    else
        goto L_345d;

L_345d:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raTerra))
        goto L_3484;
    else
        goto L_347e;

L_347e:
    fTerra = 1;

L_3484:
    if ((pctDesire >= 0))
        goto L_3497;
    else
        goto L_3491;

L_3491:
    t_merge_34aa_0001 = hbrRadar;
    goto L_34aa;

L_3497:
    if ((fTerra == 0))
        goto L_34a7;
    else
        goto L_34a1;

L_34a1:
    t_merge_34aa_0001 = hbrDkYellow;
    goto L_34aa;

L_34a7:
    t_merge_34aa_0001 = hbrGreen;

L_34aa:
    hbrSav = SelectObject(hdc, t_merge_34aa_0001);
    if ((pctDesire >= 0))
        goto L_34c7;
    else
        goto L_34c1;

L_34c1:
    t_merge_34da_0001 = hpenRadar;
    goto L_34da;

L_34c7:
    if ((fTerra == 0))
        goto L_34d7;
    else
        goto L_34d1;

L_34d1:
    t_merge_34da_0001 = hpenDkYellow;
    goto L_34da;

L_34d7:
    t_merge_34da_0001 = hpenDkGreen;

L_34da:
    hpenSav = SelectObject(hdc, t_merge_34da_0001);
    if ((pctDesire < 0))
        goto L_3501;
    else
        goto L_34ed;

L_34ed:
    dRad = (((uint32_t)(pctDesire) / 11) + 2);
    goto L_3514;

L_3501:
    /* untranslated: dRad = ((sext16to32((pctDesire neg 0x0)) / 5) + 2) */

L_3514:
    if ((dRad <= 10))
        goto L_3524;
    else
        goto L_351e;

L_351e:
    dRad = 10;

L_3524:
    Ellipse(hdc, (pt.x - dRad), (pt.y - dRad), ((pt.x + dRad) + 0x1), ((pt.y + dRad) + 0x1));
    dRad = (dRad - 2);
    if ((dRad >= 3))
        goto L_3566;
    else
        goto L_3561;

L_3561:
    dRad = (dRad + 1);

L_3566:
    if ((dRad >= 1))
        goto L_3576;
    else
        goto L_3570;

L_3570:
    dRad = 1;

L_3576:
    if ((pctDesire >= 0))
        goto L_3589;
    else
        goto L_3583;

L_3583:
    t_merge_359c_0001 = hbrEnemy;
    goto L_359c;

L_3589:
    if ((fTerra == 0))
        goto L_3599;
    else
        goto L_3593;

L_3593:
    t_merge_359c_0001 = hbrYellow;
    goto L_359c;

L_3599:
    t_merge_359c_0001 = hbrShip;

L_359c:
    SelectObject(hdc, t_merge_359c_0001);
    if ((pctDesire >= 0))
        goto L_35b5;
    else
        goto L_35af;

L_35af:
    t_merge_35c8_0001 = hpenEnemy;
    goto L_35c8;

L_35b5:
    if ((fTerra == 0))
        goto L_35c5;
    else
        goto L_35bf;

L_35bf:
    t_merge_35c8_0001 = hpenYellow;
    goto L_35c8;

L_35c5:
    t_merge_35c8_0001 = hpenShip;

L_35c8:
    SelectObject(hdc, t_merge_35c8_0001);
    Ellipse(hdc, (pt.x - dRad), (pt.y - dRad), ((pt.x + dRad) + 0x1), ((pt.y + dRad) + 0x1));
    if ((lppl->iPlayer == -1))
        goto L_372b;
    else
        goto L_3609;

L_3609:
    rc.left = (pt.x - 1);
    rc.right = (rc.left + 9);
    rc.top = (pt.y - 20);
    rc.bottom = (rc.top + 8);
    FillRect(hdc, &(rc), GetStockObject(BLACK_BRUSH));
    if ((lppl->iPlayer != idPlayer))
        goto L_3666;
    else
        goto L_365c;

L_365c:
    hbr = hbrBBlue;
    goto L_36db;

L_3666:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lppl->iPlayer)]) != 0x1 ? L_369d : L_3693 */

L_3693:
    hbr = hbrStarbase;
    goto L_36db;

L_369d:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lppl->iPlayer)]) != 0x0 ? L_36d4 : L_36ca */

L_36ca:
    hbr = hbrRadar;
    goto L_36db;

L_36d4:
    hbr = hbrEnemy;

L_36db:
    SelectObject(hdc, hbr);
    PatBlt(hdc, pt.x, (pt.y - 19), 0x1, 0x15, PATCOPY);
    PatBlt(hdc, pt.x, (pt.y - 19), 0x7, 0x6, PATCOPY);

L_372b:
    SelectObject(hdc, hpenSav);
    SelectObject(hdc, hbrSav);

L_3745:
    if ((mdScanBase == 0x1))
        goto L_3759;
    else
        goto L_374f;

L_374f:
    if ((mdScanBase != 0x2))
        goto L_39ff;
    else
        goto L_3759;

L_3759:
    if ((mdScanBase != 0x2))
        goto L_3769;
    else
        goto L_3763;

L_3763:
    t_merge_376c_0001 = 0x1;
    goto L_376c;

L_3769:
    t_merge_376c_0001 = 0x0;

L_376c:
    fConc = t_merge_376c_0001;
    if ((iScanZoom >= 0))
        goto L_3780;
    else
        goto L_377a;

L_377a:
    t_merge_3783_0001 = 0x1;
    goto L_3783;

L_3780:
    t_merge_3783_0001 = 0x0;

L_3783:
    iOff = t_merge_3783_0001;
    if ((lppl->det >= 0x4))
        goto L_37b8;
    else
        goto L_3799;

L_3799:
    if ((fConc == 0))
        goto LNormalScannerMode;
    else
        goto L_37a3;

L_37a3:
    if ((lppl->det < 0x3))
        goto LNormalScannerMode;
    else
        goto L_37b2;

L_37b2:

L_37b8:
    xOut = (pt.x - vrgScanPO[iOff][0]);
    yOut = (pt.y - vrgScanPO[iOff][1]);
    hbrSav = SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, (xOut - 2), yOut, vrgScanPO[iOff][2], 0x1, PATCOPY);
    PatBlt(hdc, (xOut - 2), ((yOut - vrgScanPO[iOff][2]) + 0x1), 0x1, vrgScanPO[iOff][2], PATCOPY);
    j = 0;
    goto L_39e3;

L_386c:
    if ((fConc == 0))
        goto L_38c6;
    else
        goto L_3876;

L_3876:
    /* untranslated: l = sext16to32((words(lppl->rgMinConc[j], signhiword(lppl->rgMinConc[j])) / 0x5)) */
    if ((HIWORD(l) < 0x0))
        goto L_393e;
    else
        goto L_38a8;

L_38a8:
    if ((HIWORD(l) > 0x0))
        goto L_38b7;
    else
        goto L_38ad;

L_38ad:
    if ((LOWORD(l) <= 0x14))
        goto L_393e;
    else
        goto L_38b7;

L_38b7:
    l = 20;

L_38c6:
    l = lppl->rgwtMin[j];
    /* untranslated: l = (int32_t)(words((signhiword((sext16to32(cMinGrafMax) / 0x28)) + HIWORD(l)), ((sext16to32(cMinGrafMax) / 0x28) + LOWORD(l))) /
     * sext16to32((sext16to32(cMinGrafMax) / 0x14))) */
    if ((HIWORD(l) < 0x0))
        goto L_393e;
    else
        goto L_3923;

L_3923:
    if ((HIWORD(l) > 0x0))
        goto L_3932;
    else
        goto L_3928;

L_3928:
    if ((LOWORD(l) <= 0x14))
        goto L_393e;
    else
        goto L_3932;

L_3932:
    l = 20;

L_393e:
    if ((iOff == 0))
        goto L_3965;
    else
        goto L_3948;

L_3948:
    l = (int32_t)((l / 2));

L_3965:
    if ((HIWORD(l) < 0x0))
        goto L_39cb;
    else
        goto L_396f;

L_396f:
    if ((HIWORD(l) > 0x0))
        goto L_397e;
    else
        goto L_3974;

L_3974:
    if ((LOWORD(l) <= 0x0))
        goto L_39cb;
    else
        goto L_397e;

L_397e:
    SelectObject(hdc, rghbrMineral[j]);
    PatBlt(hdc, xOut, (yOut - LOWORD(l)), vrgScanPO[iOff][3], LOWORD(l), PATCOPY);

L_39cb:
    xOut = (xOut + vrgScanPO[iOff][4]);
    j = (j + 1);

L_39e3:
    if ((j < 3))
        goto L_386c;
    else
        goto L_39ed;

L_39ed:
    SelectObject(hdc, hbrSav);
    goto LNormalScannerMode;

L_39ff:
    if ((mdScanBase != 0x4))
        goto LNormalScannerMode;
    else
        goto L_3a09;

L_3a09:
    fTerra = 0;
    if ((lppl->det < 0x3))
        goto L_3bc5;
    else
        goto L_3a21;

L_3a21:
    if ((lppl->iPlayer == -1))
        goto L_3bc5;
    else
        goto L_3a2e;

L_3a2e:
    if ((lppl->iPlayer != idPlayer))
        goto L_3a53;
    else
        goto L_3a3d;

L_3a3d:
    lPop = lppl->rgwtMin[3];
    goto L_3a70;

L_3a53:
    /* untranslated: lPop = (int32_t)(words(lppl->uPopGuess, 0x0) * 0x4) */

L_3a70:
    dRad = 0;
    goto L_3aa4;

L_3a79:
    if ((SIGNHIWORD(vrgPopRad[dRad]) < HIWORD(lPop)))
        goto L_3a9f;
    else
        goto L_3a8e;

L_3a8e:
    if ((SIGNHIWORD(vrgPopRad[dRad]) > HIWORD(lPop)))
        goto L_3aae;
    else
        goto L_3a93;

L_3a93:
    if ((vrgPopRad[dRad] > LOWORD(lPop)))
        goto L_3aae;
    else
        goto L_3a99;

L_3a99:

L_3a9f:
    dRad = (dRad + 1);

L_3aa4:
    if ((dRad < 19))
        goto L_3a79;
    else
        goto L_3aae;

L_3aae:
    dRad = (dRad + 2);
    if ((lppl->iPlayer != idPlayer))
        goto L_3acb;
    else
        goto L_3ac2;

L_3ac2:
    iRel = 0;
    goto L_3b07;

L_3acb:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lppl->iPlayer)]) != 0x1 ? L_3b01 : L_3af8 */

L_3af8:
    iRel = 1;
    goto L_3b07;

L_3b01:
    iRel = 3;

L_3b07:
    if ((iRel != 0))
        goto L_3b1a;
    else
        goto L_3b14;

L_3b14:
    t_merge_3b2d_0001 = hbrShip;
    goto L_3b2d;

L_3b1a:
    if ((iRel != 3))
        goto L_3b2a;
    else
        goto L_3b24;

L_3b24:
    t_merge_3b2d_0001 = hbrEnemy;
    goto L_3b2d;

L_3b2a:
    t_merge_3b2d_0001 = hbrYellow;

L_3b2d:
    hbrSav = SelectObject(hdc, t_merge_3b2d_0001);
    if ((iRel != 0))
        goto L_3b4a;
    else
        goto L_3b44;

L_3b44:
    t_merge_3b5d_0001 = hpenDkGreen;
    goto L_3b5d;

L_3b4a:
    if ((iRel != 3))
        goto L_3b5a;
    else
        goto L_3b54;

L_3b54:
    t_merge_3b5d_0001 = hpenEnemy;
    goto L_3b5d;

L_3b5a:
    t_merge_3b5d_0001 = hpenDkYellow;

L_3b5d:
    hpenSav = SelectObject(hdc, t_merge_3b5d_0001);
    if ((iScanZoom >= 3))
        goto L_3b7d;
    else
        goto L_3b70;

L_3b70:
    dRad = ((dRad + 0x1) >> 0x1);

L_3b7d:
    Ellipse(hdc, (pt.x - dRad), (pt.y - dRad), ((pt.x + dRad) + 0x1), ((pt.y + dRad) + 0x1));
    SelectObject(hdc, hpenSav);
    SelectObject(hdc, hbrSav);
    goto L_3257;

L_3bc5:
    if ((lppl->iPlayer == -1))
        goto LNormalScannerMode;
    else
        goto L_3bcf;

L_3bcf:

LNormalScannerMode:
    if ((lppl->iPlayer != -1))
        goto L_3c3b;
    else
        goto L_3be5;

L_3be5:
    if ((rgptPlan[id].x != ptSelMain.x))
        goto L_3c06;
    else
        goto L_3bfd;

L_3bfd:
    if ((rgptPlan[id].y == ptSelMain.y))
        goto L_3257;
    else
        goto L_3c06;

L_3c06:
    BitBlt(hdc, (pt.x - 1), (pt.y - 1), 0x3, 0x3, hdcMem, 0xb, 0x12, SRCCOPY);

L_3c3b:
    if ((rgptPlan[id].x != ptSelMain.x))
        goto L_3eeb;
    else
        goto L_3c53;

L_3c53:
    if ((rgptPlan[id].y != ptSelMain.y))
        goto L_3eeb;
    else
        goto L_3c5c;

L_3c5c:
    if ((lppl->iPlayer != idPlayer))
        goto L_3c73;
    else
        goto L_3c6b;

L_3c6b:
    yBmp = 0;
    goto L_3cad;

L_3c73:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lppl->iPlayer)]) != 0x1 ? L_3ca8 : L_3ca0 */

L_3ca0:
    yBmp = 22;
    goto L_3cad;

L_3ca8:
    yBmp = 11;

L_3cad:
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x0, 0x45, SRCAND);
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x0, yBmp, SRCPAINT);
    if ((fStarbase == 0))
        goto L_3db8;
    else
        goto L_3d1a;

L_3d1a:
    if ((fStarbase != 2))
        goto L_3d2d;
    else
        goto L_3d27;

L_3d27:
    t_merge_3d30_0001 = hbrBlue;
    goto L_3d30;

L_3d2d:
    t_merge_3d30_0001 = hbrYellow;

L_3d30:
    hbrSav = SelectObject(hdc, t_merge_3d30_0001);
    PatBlt(hdc, (pt.x + 4), (pt.y - 6), 0x5, 0x5, BLACKNESS);
    PatBlt(hdc, (pt.x + 5), (pt.y - 6), 0x3, 0x5, PATCOPY);
    PatBlt(hdc, (pt.x + 4), (pt.y - 5), 0x5, 0x3, PATCOPY);
    SelectObject(hdc, hbrSav);

L_3db8:
    if ((fStargate == 0))
        goto L_3e50;
    else
        goto L_3dc2;

L_3dc2:
    hbrSav = SelectObject(hdc, hbrGreen);
    PatBlt(hdc, (pt.x - 7), (pt.y - 6), 0x5, 0x5, BLACKNESS);
    PatBlt(hdc, (pt.x - 6), (pt.y - 6), 0x3, 0x5, PATCOPY);
    PatBlt(hdc, (pt.x - 7), (pt.y - 5), 0x5, 0x3, PATCOPY);
    SelectObject(hdc, hbrSav);

L_3e50:
    if ((fMA == 0))
        goto L_3257;
    else
        goto L_3e5a;

L_3e5a:
    hbrSav = SelectObject(hdc, hbrPurple);
    PatBlt(hdc, (pt.x - 2), (pt.y - 9), 0x5, 0x5, BLACKNESS);
    PatBlt(hdc, (pt.x - 1), (pt.y - 9), 0x3, 0x5, PATCOPY);
    PatBlt(hdc, (pt.x - 2), (pt.y - 8), 0x5, 0x3, PATCOPY);
    SelectObject(hdc, hbrSav);

L_3eeb:
    if ((lppl->iPlayer != idPlayer))
        goto L_3f02;
    else
        goto L_3efa;

L_3efa:
    yBmp = 0;
    goto L_3f3c;

L_3f02:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lppl->iPlayer)]) != 0x1 ? L_3f37 : L_3f2f */

L_3f2f:
    yBmp = 10;
    goto L_3f3c;

L_3f37:
    yBmp = 5;

L_3f3c:
    BitBlt(hdc, (pt.x - 2), (pt.y - 2), 0x5, 0x5, hdcMem, 0xb, yBmp, SRCCOPY);
    if ((fStarbase == 0))
        goto L_3fc9;
    else
        goto L_3f77;

L_3f77:
    if ((fStarbase != 2))
        goto L_3f8a;
    else
        goto L_3f84;

L_3f84:
    t_merge_3f8d_0001 = hbrBlue;
    goto L_3f8d;

L_3f8a:
    t_merge_3f8d_0001 = hbrYellow;

L_3f8d:
    hbrSav = SelectObject(hdc, t_merge_3f8d_0001);
    PatBlt(hdc, (pt.x + 3), (pt.y - 4), 0x3, 0x3, PATCOPY);
    SelectObject(hdc, hbrSav);

L_3fc9:
    if ((fStargate == 0))
        goto L_4061;
    else
        goto L_3fd3;

L_3fd3:
    hbrSav = SelectObject(hdc, hbrGreen);
    PatBlt(hdc, (pt.x - 5), (pt.y - 4), 0x3, 0x3, BLACKNESS);
    PatBlt(hdc, (pt.x - 4), (pt.y - 4), 0x1, 0x3, PATCOPY);
    PatBlt(hdc, (pt.x - 5), (pt.y - 3), 0x3, 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);

L_4061:
    if ((fMA == 0))
        goto L_3257;
    else
        goto L_406b;

L_406b:
    hbrSav = SelectObject(hdc, hbrPurple);
    PatBlt(hdc, (pt.x - 1), (pt.y - 6), 0x3, 0x3, BLACKNESS);
    PatBlt(hdc, pt.x, (pt.y - 6), 0x1, 0x3, PATCOPY);
    PatBlt(hdc, (pt.x - 1), (pt.y - 5), 0x3, 0x1, PATCOPY);
    SelectObject(hdc, hbrSav);

L_40f8:
    if ((cFleet == 0))
        goto L_4654;
    else
        goto L_4102;

L_4102:
    if ((mdScanBase == 0x5))
        goto L_4654;
    else
        goto L_410c;

L_410c:
    lpflT = *(rglpfl);
    hbrSav = SelectObject(hdc, hbrShip);
    if ((sel.grobj != grobjFleet))
        goto L_413d;
    else
        goto L_4137;

L_4137:
    t_merge_4140_0001 = sel.fl.id;
    goto L_4140;

L_413d:
    t_merge_4140_0001 = -1;

L_4140:
    id = t_merge_4140_0001;
    if ((sel.scan.grobj != grobjFleet))
        goto L_4169;
    else
        goto L_414d;

L_414d:
    t_merge_416c_0001 = rglpfl[sel.scan.ifl]->id;
    goto L_416c;

L_4169:
    t_merge_416c_0001 = -1;

L_416c:
    id2 = t_merge_416c_0001;
    i = 0;
    goto L_417d;

L_4178:
    i = (i + 1);

L_417d:
    if ((i >= cFleet))
        goto L_4648;
    else
        goto L_4189;

L_4189:
    lpflT = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_41ba;
    else
        goto L_41b2;

L_41b2:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_4648;
    else
        goto L_41ba;

L_41ba:
    t_call_41c0 = CShipsScanVis(lpflT);
    if ((HIWORD(t_call_41c0) > 0x0))
        goto L_41fc;
    else
        goto L_41d0;

L_41d0:
    if ((HIWORD(t_call_41c0) < 0x0))
        goto L_41dd;
    else
        goto L_41d5;

L_41d5:
    if ((LOWORD(t_call_41c0) > 0x0))
        goto L_41fc;
    else
        goto L_41dd;

L_41dd:
    if ((lpflT->id == id))
        goto L_41fc;
    else
        goto L_41eb;

L_41eb:
    if ((lpflT->id != id2))
        goto L_4178;
    else
        goto L_41f6;

L_41f6:

L_41fc:
    idP = lpflT->idPlanet;
    if ((idP == -1))
        goto L_422a;
    else
        goto L_4211;

L_4211:
    pt.x = rgptPlan[idP].x;
    pt.y = rgptPlan[idP].y;
    goto L_423b;

L_422a:
    pt.x = lpflT->pt.x;
    pt.y = lpflT->pt.y;

L_423b:
    if ((pt.x < xMin))
        goto L_4178;
    else
        goto L_4247;

L_4247:
    if ((pt.x >= xMax))
        goto L_4178;
    else
        goto L_4252;

L_4252:
    if ((pt.y < yMin))
        goto L_4178;
    else
        goto L_425e;

L_425e:
    if ((pt.y >= yMax))
        goto L_4178;
    else
        goto L_4269;

L_4269:
    pt.x = PtToScan((xOff + pt.x));
    pt.y = PtToScan((yOff - pt.y));
    if ((lpflT->iPlayer == idPlayer))
        goto L_42a3;
    else
        goto L_429d;

L_429d:
    t_merge_42a6_0001 = 0x1;
    goto L_42a6;

L_42a3:
    t_merge_42a6_0001 = 0x0;

L_42a6:
    yBmp = t_merge_42a6_0001;
    /* untranslated: branch lpflT->pt.x != ss:[ptSelMain] ? L_42d3 : L_42c4 */

L_42c4:
    /* untranslated: branch lpflT->pt.y != ss:[ptSelMain+0x2] ? L_42d3 : L_42cd */

L_42cd:
    t_merge_42d6_0001 = 0x1;
    goto L_42d6;

L_42d3:
    t_merge_42d6_0001 = 0x0;

L_42d6:
    fSelected = t_merge_42d6_0001;
    if ((idP == -1))
        goto L_447c;
    else
        goto L_42e4;

L_42e4:
    if (((uint16_t)(rgWhatsHere[idP]) == 3))
        goto L_4178;
    else
        goto L_42f9;

L_42f9:
    if (((uint16_t)(rgWhatsHere[idP]) == (yBmp + 1)))
        goto L_4178;
    else
        goto L_4313;

L_4313:
    if ((mdScanBase > 0x2))
        goto L_4178;
    else
        goto L_431a;

L_431a:

L_4320:
    rgWhatsHere[idP] = (rgWhatsHere[idP] + LOBYTE((yBmp + 0x1)));
    yBmp = ((uint16_t)(rgWhatsHere[idP]) - 1);
    if ((fSelected == 0))
        goto L_43b9;
    else
        goto L_434f;

L_434f:
    BitBlt(hdc, (pt.x - 9), (pt.y - 9), 0x13, 0x13, hdcMem, 0x1d, 0x45, SRCAND);
    BitBlt(hdc, (pt.x - 9), (pt.y - 9), 0x13, 0x13, hdcMem, 0x1d, LOWORD((19 * yBmp)), SRCPAINT);
    goto L_4420;

L_43b9:
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x10, 0x45, SRCAND);
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0x10, LOWORD((11 * yBmp)), SRCPAINT);

L_4420:
    if (((grbitScan & 0x1000) == 0x0))
        goto L_4178;
    else
        goto L_442e;

L_442e:
    if ((lpflT->fDone != 0x0))
        goto L_4178;
    else
        goto L_4445;

L_4445:
    if ((fSelected == 0))
        goto L_445c;
    else
        goto L_4456;

L_4456:
    t_merge_445f_0001 = 0x9;
    goto L_445f;

L_445c:
    t_merge_445f_0001 = 0x5;

L_445f:
    DrawScanFleetCount(lpflT, pt.x, ((pt.y - t_merge_445f_0001) + 0xfffe), hdc, hdcMem);

L_447c:
    if ((fSelected == 0))
        goto L_4505;
    else
        goto L_4486;

L_4486:
    BitBlt(hdc, (pt.x - 5), (pt.y - 5), 0xb, 0xb, hdcMem, 0xb, (LOWORD((11 * yBmp)) + 0x24), SRCPAINT);
    if (((grbitScan & 0x1000) == 0x0))
        goto L_4178;
    else
        goto L_44cc;

L_44cc:
    if ((lpflT->fDone != 0x0))
        goto L_4178;
    else
        goto L_44e3;

L_44e3:
    DrawScanFleetCount(lpflT, pt.x, (pt.y - 7), hdc, hdcMem);

L_4505:
    SelectObject(hdcMem, hbmpScanShip);
    if ((yBmp != 0))
        goto L_452a;
    else
        goto L_451b;

L_451b:
    cr = 0xff0000;
    goto L_4572;

L_452a:
    /* untranslated: branch sext8to16(byte ds:[(rgplr[idPlayer].rgmdRelation + lpflT->iPlayer)]) != 0x1 ? L_4566 : L_4557 */

L_4557:
    cr = 0xffff;
    goto L_4572;

L_4566:
    cr = 0xff;

L_4572:
    SetTextColor(hdc, cr);
    SetBkColor(hdc, 0x0);
    GetScanFleetOrientation(lpflT, &(ptO), &(ptD));
    BitBlt(hdc, (pt.x - ((uint32_t)(ptD.x) / 2)), (pt.y - ((uint32_t)(ptD.y) / 2)), ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCPAINT);
    if (((grbitScan & 0x1000) == 0x0))
        goto L_4638;
    else
        goto L_45f7;

L_45f7:
    if ((lpflT->fDone != 0x0))
        goto L_4638;
    else
        goto L_460e;

L_460e:
    DrawScanFleetCount(lpflT, pt.x, ((pt.y - ((uint32_t)(ptD.y) / 2)) + 0xfffe), hdc, hdcMem);

L_4638:
    SelectObject(hdcMem, hbmpScanner);

L_4648:
    SelectObject(hdc, hbrSav);

L_4654:
    /* untranslated: call ExpandRc(prc, (dExpand neg 0), (dExpand neg 0)) -> callresult(void) */
    prc->bottom = (prc->bottom - 14);
    crFore = SetTextColor(hdc, 0x0);
    crBack = GetBkColor(hdc);
    if ((sel.grobj == grobjFleet))
        goto L_46d3;
    else
        goto L_46a2;

L_46a2:
    if ((sel.scan.grobj == grobjFleet))
        goto L_46d3;
    else
        goto L_46ac;

L_46ac:
    if ((sel.scan.grobj == grobjThing))
        goto L_46d3;
    else
        goto L_46b6;

L_46b6:
    if ((sel.grobj != grobjPlanet))
        goto L_4713;
    else
        goto L_46c0;

L_46c0:
    if ((sel.pl.fStarbase == 0x0))
        goto L_4713;
    else
        goto L_46d3;

L_46d3:
    IntersectClipRect(hdc, prc->left, prc->top, prc->right, prc->bottom);
    fOrdersVis = 0;
    DrawShipScanPath(hdc, 0x1);
    SelectClipRgn(hdc, hrgnHuge);

L_4713:
    SetBkColor(hdc, crBack);
    SetTextColor(hdc, crFore);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    if ((hdcScreen == 0x0))
        goto L_47c5;
    else
        goto L_4750;

L_4750:
    SetWindowOrg(hdc, 0x0, 0x0);
    BitBlt(hdcScreen, prc->left, prc->top, (prc->right - prc->left), (prc->bottom - prc->top), hdc, (prc->left & 0x7), (prc->top & 0x7), SRCCOPY);
    SelectObject(hdc, hbmpXSav);
    DeleteObject(hbmpScreen);
    DeleteDC(hdc);

L_47c5:
    return 0x0;
}

void DrawScanFleetCount(FLEET *lpfl, int16_t x, int16_t y, HDC hdc, HDC hdcMem) {
    int32_t  l2;
    int16_t  f999;
    COLORREF cr;
    FLEET   *lpflWalk;
    int16_t  iPlr;
    HBITMAP  hbmpSav;
    int32_t  l;
    uint16_t t_merge_47fe_0001;

L_47d2:
    lpflWalk = lpfl;
    if (((grbitScan & 0x2000) == 0x0))
        goto L_47fb;
    else
        goto L_47f5;

L_47f5:
    t_merge_47fe_0001 = 0xffff;
    goto L_47fe;

L_47fb:
    t_merge_47fe_0001 = 0xfffe;

L_47fe:
    iPlr = t_merge_47fe_0001;
    l = 0;
    f999 = 0;

L_4810:
    l2 = CShipsScanVis(lpflWalk);
    if ((HIWORD(l2) < 0x0))
        goto L_487a;
    else
        goto L_482d;

L_482d:
    if ((HIWORD(l2) > 0x0))
        goto L_483b;
    else
        goto L_4832;

L_4832:
    if ((LOWORD(l2) <= 0x0))
        goto L_487a;
    else
        goto L_483b;

L_483b:
    l = (l + l2);
    if ((lpflWalk->iPlayer == iPlr))
        goto L_487a;
    else
        goto L_4856;

L_4856:
    if ((iPlr == -2))
        goto L_487a;
    else
        goto L_485f;

L_485f:
    if ((iPlr != -1))
        goto L_4875;
    else
        goto L_4868;

L_4868:
    iPlr = lpflWalk->iPlayer;
    goto L_487a;

L_4875:
    iPlr = -2;

L_487a:
    lpflWalk->fDone = 0x1;
    lpflWalk = lpflWalk->lpflNext;
    if ((LOWORD(lpflWalk) != LOWORD(lpfl)))
        goto L_4810;
    else
        goto L_48ad;

L_48ad:
    if ((HIWORD(lpflWalk) != HIWORD(lpfl)))
        goto L_4810;
    else
        goto L_48b5;

L_48b5:
    if ((HIWORD(l) < 0x0))
        goto L_48da;
    else
        goto L_48be;

L_48be:
    if ((HIWORD(l) > 0x0))
        goto L_48cd;
    else
        goto L_48c3;

L_48c3:
    if ((LOWORD(l) <= 0x3e7))
        goto L_48da;
    else
        goto L_48cd;

L_48cd:
    l = 999;
    goto L_48f4;

L_48da:
    if ((HIWORD(l) > 0x0))
        goto L_48f4;
    else
        goto L_48e3;

L_48e3:
    if ((HIWORD(l) < 0x0))
        goto L_4bee;
    else
        goto L_48e8;

L_48e8:
    if ((LOWORD(l) <= 0x0))
        goto L_4bee;
    else
        goto L_48ee;

L_48ee:

L_48f4:
    hbmpSav = SelectObject(hdcMem, hbmpNumbers);
    SetBkColor(hdc, 0x0);
    SetTextColor(hdc, 0xffffff);
    if ((iPlr < 0))
        goto L_494f;
    else
        goto L_492c;

L_492c:
    if ((iPlr == idPlayer))
        goto L_494f;
    else
        goto L_4937;

L_4937:
    cr = rgcrPlrHistory[iPlr];
    goto L_4959;

L_494f:
    cr = 0xffffff;

L_4959:
    x = (x - 1);
    y = (y - 7);
    if ((HIWORD(l) < 0x0))
        goto L_4a4e;
    else
        goto L_496a;

L_496a:
    if ((HIWORD(l) > 0x0))
        goto L_4978;
    else
        goto L_496f;

L_496f:
    if ((LOWORD(l) <= 0x63))
        goto L_4a4e;
    else
        goto L_4978;

L_4978:
    f999 = 1;
    x = (x - 5);
    if ((LOWORD(cr) != 0xffff))
        goto L_4994;
    else
        goto L_498a;

L_498a:
    if ((HIWORD(cr) == 0xff))
        goto L_49d8;
    else
        goto L_4994;

L_4994:
    /* untranslated: call BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, ((words(LOWORD(l), signhiword(LOWORD(l))) / 0x64) * 0x4), 0x0, 0x220326) -> callresult(int16_t) */
    SetTextColor(hdc, cr);

L_49d8:
    /* untranslated: call BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, ((words(LOWORD(l), signhiword(LOWORD(l))) / 0x64) * 0x4), 0x0, SRCPAINT) -> callresult(int16_t) */
    if ((LOWORD(cr) != 0xffff))
        goto L_4a21;
    else
        goto L_4a17;

L_4a17:
    if ((HIWORD(cr) == 0xff))
        goto L_4a31;
    else
        goto L_4a21;

L_4a21:
    SetTextColor(hdc, 0xffffff);

L_4a31:
    x = (x + 8);
    l = (int32_t)((l % 100));

L_4a4e:
    if ((HIWORD(l) > 0x0))
        goto L_4a6e;
    else
        goto L_4a57;

L_4a57:
    if ((HIWORD(l) < 0x0))
        goto L_4a65;
    else
        goto L_4a5c;

L_4a5c:
    if ((LOWORD(l) > 0x9))
        goto L_4a6e;
    else
        goto L_4a65;

L_4a65:
    if ((f999 == 0))
        goto L_4b3f;
    else
        goto L_4a6e;

L_4a6e:
    x = (x - 3);
    if ((LOWORD(cr) != 0xffff))
        goto L_4a85;
    else
        goto L_4a7b;

L_4a7b:
    if ((HIWORD(cr) == 0xff))
        goto L_4ac9;
    else
        goto L_4a85;

L_4a85:
    /* untranslated: call BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, ((words(LOWORD(l), signhiword(LOWORD(l))) / 0xa) * 0x4), 0x0, 0x220326) -> callresult(int16_t) */
    SetTextColor(hdc, cr);

L_4ac9:
    /* untranslated: call BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, ((words(LOWORD(l), signhiword(LOWORD(l))) / 0xa) * 0x4), 0x0, SRCPAINT) -> callresult(int16_t) */
    if ((LOWORD(cr) != 0xffff))
        goto L_4b12;
    else
        goto L_4b08;

L_4b08:
    if ((HIWORD(cr) == 0xff))
        goto L_4b22;
    else
        goto L_4b12;

L_4b12:
    SetTextColor(hdc, 0xffffff);

L_4b22:
    x = (x + 5);
    l = (int32_t)((l % 10));

L_4b3f:
    if ((LOWORD(cr) != 0xffff))
        goto L_4b52;
    else
        goto L_4b48;

L_4b48:
    if ((HIWORD(cr) == 0xff))
        goto L_4b90;
    else
        goto L_4b52;

L_4b52:
    BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, (LOWORD(l) * 0x4), 0x0, 0x220326);
    SetTextColor(hdc, cr);

L_4b90:
    BitBlt(hdc, x, y, 0x4, 0x7, hdcMem, (LOWORD(l) * 0x4), 0x0, SRCPAINT);
    if ((LOWORD(cr) != 0xffff))
        goto L_4bd3;
    else
        goto L_4bc9;

L_4bc9:
    if ((HIWORD(cr) == 0xff))
        goto L_4be3;
    else
        goto L_4bd3;

L_4bd3:
    SetTextColor(hdc, 0xffffff);

L_4be3:
    SelectObject(hdcMem, hbmpSav);

L_4bee:
    return;
}

int32_t CShipsScanVis(FLEET *lpfl) {
    int16_t  j;
    int32_t  csh;
    int16_t  k;
    uint16_t grbitSh;

L_4bf4:
    csh = 0;
    if (((grbitScan & 0x100) == 0x0))
        goto L_4cbc;
    else
        goto L_4c15;

L_4c15:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4c92;
    else
        goto L_4c24;

L_4c24:
    if ((lpfl->cord != 1))
        goto L_4c89;
    else
        goto L_4c31;

L_4c31:
    if ((lpfl->lpplord->rgord[0].grTask == 0x6))
        goto L_4c89;
    else
        goto L_4c47;

L_4c47:
    if ((lpfl->lpplord->rgord[0].grTask == 0x5))
        goto L_4c89;
    else
        goto L_4c5d;

L_4c5d:
    if ((lpfl->lpplord->rgord[0].grTask == 0x3))
        goto L_4c89;
    else
        goto L_4c73;

L_4c73:
    if ((lpfl->lpplord->rgord[0].grTask != 0x7))
        goto L_4c92;
    else
        goto L_4c89;

L_4c89:
    return 0x0;

L_4c92:
    if ((lpfl->iPlayer == idPlayer))
        goto L_4cbc;
    else
        goto L_4ca1;

L_4ca1:
    if ((lpfl->iwarpFlt != 0x0))
        goto L_4cbc;
    else
        goto L_4cb3;

L_4cb3:
    return 0x0;

L_4cbc:
    if (((grbitScan & 0x200) == 0x0))
        goto L_4d4f;
    else
        goto L_4cca;

L_4cca:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4d4f;
    else
        goto L_4cd9;

L_4cd9:
    grbitSh = grbitScanShip;
    j = 0;
    goto L_4cf7;

L_4ce7:
    j = (j + 1);
    grbitSh = (grbitSh >> 0x1);

L_4cf7:
    if ((grbitSh == 0x0))
        goto L_4e6c;
    else
        goto L_4d00;

L_4d00:
    if (((grbitSh & 0x1) == 0x0))
        goto L_4ce7;
    else
        goto L_4d0e;

L_4d0e:
    if ((lpfl->rgcsh[j] <= 0))
        goto L_4ce7;
    else
        goto L_4d2b;

L_4d2b:
    csh = (csh + (uint32_t)(lpfl->rgcsh[j]));

L_4d4f:
    if (((grbitScan & 0x800) == 0x0))
        goto L_4e39;
    else
        goto L_4d5d;

L_4d5d:
    if ((lpfl->iPlayer == idPlayer))
        goto L_4e39;
    else
        goto L_4d6c;

L_4d6c:
    grbitSh = grbitScanEShip;
    j = 0;
    goto L_4d8a;

L_4d7a:
    j = (j + 1);
    grbitSh = (grbitSh >> 0x1);

L_4d8a:
    if ((grbitSh == 0x0))
        goto L_4e6c;
    else
        goto L_4d93;

L_4d93:
    if (((grbitSh & 0x1) == 0x0))
        goto L_4d7a;
    else
        goto L_4da1;

L_4da1:
    k = 0;
    goto L_4e2a;

L_4da9:
    if ((lpfl->rgcsh[k] <= 0))
        goto L_4e26;
    else
        goto L_4dc6;

L_4dc6:
    if ((((*(LphuldefFromId(rglpshdef[lpfl->iPlayer][k].hul.ihuldef) + 0x7b) >> 0xa) & 0xf) != j))
        goto L_4e26;
    else
        goto L_4e08;

L_4e08:
    csh = (csh + (uint32_t)(lpfl->rgcsh[k]));

L_4e26:
    k = (k + 1);

L_4e2a:
    if ((k >= 16))
        goto L_4d7a;
    else
        goto L_4e30;

L_4e30:

L_4e39:
    j = 0;
    goto L_4e63;

L_4e41:
    csh = (csh + (uint32_t)(lpfl->rgcsh[j]));
    j = (j + 1);

L_4e63:
    if ((j < 16))
        goto L_4e41;
    else
        goto L_4e6c;

L_4e6c:
    return csh;
}

void DrawRadarCircle(DRAWCIR *pdc, RECT *prc) {
    int16_t  y2;
    int32_t  r2;
    COLORREF crSav;
    int16_t  dy;
    int16_t  y;
    int16_t  iFree;
    int16_t  i;
    int16_t  dx;
    int16_t  x2;
    int16_t  rad;
    int32_t  l;
    int16_t  x;
    RECT     rc;
    int16_t  t_merge_4f2b_0001;
    int16_t  t_merge_4f4b_0001;

L_4e7c:
    if ((prc == 0x0))
        goto L_4ec4;
    else
        goto L_4e8e;

L_4e8e:
    if ((pdc->fCovered != 0))
        goto L_5405;
    else
        goto L_4e9a;

L_4e9a:
    if ((IntersectRect(&(rc), prc, ((uint8_t *)(pdc) + 0xc)) == 0))
        goto L_5405;
    else
        goto L_4ebe;

L_4ebe:

L_4ec4:
    if ((prc == 0x0))
        goto L_52d8;
    else
        goto L_4ecd;

L_4ecd:
    rad = ((prc->right - prc->left) >> 0x1);
    x = (prc->left + rad);
    y = (prc->top + rad);
    r2 = (uint32_t)(((uint32_t)(rad) * (uint32_t)(rad)));
    i = 0;
    goto L_4fe0;

L_4f13:
    if ((i >= 2))
        goto L_4f25;
    else
        goto L_4f1c;

L_4f1c:
    t_merge_4f2b_0001 = pdc->rcClip.left;
    goto L_4f2b;

L_4f25:
    t_merge_4f2b_0001 = pdc->rcClip.right;

L_4f2b:
    x2 = t_merge_4f2b_0001;
    if (((i & 0x1) == 0x0))
        goto L_4f45;
    else
        goto L_4f3c;

L_4f3c:
    t_merge_4f4b_0001 = pdc->rcClip.top;
    goto L_4f4b;

L_4f45:
    t_merge_4f4b_0001 = pdc->rcClip.bottom;

L_4f4b:
    y2 = t_merge_4f4b_0001;
    dx = (x2 - x);
    dy = (y2 - y);
    if ((SIGNHIWORD(dx) > HIWORD(r2)))
        goto L_4fe9;
    else
        goto L_4f6c;

L_4f6c:
    if ((SIGNHIWORD(dx) < HIWORD(r2)))
        goto L_4f79;
    else
        goto L_4f71;

L_4f71:
    if ((dx > LOWORD(r2)))
        goto L_4fe9;
    else
        goto L_4f79;

L_4f79:
    if ((SIGNHIWORD(dy) > HIWORD(r2)))
        goto L_4fe9;
    else
        goto L_4f85;

L_4f85:
    if ((SIGNHIWORD(dy) < HIWORD(r2)))
        goto L_4f92;
    else
        goto L_4f8a;

L_4f8a:
    if ((dy > LOWORD(r2)))
        goto L_4fe9;
    else
        goto L_4f92;

L_4f92:
    if (((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) + HIWORD((uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))))) < HIWORD(r2)))
        goto L_4fdc;
    else
        goto L_4fcc;

L_4fcc:
    /* untranslated: branch (hiword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x2a]) > HIWORD(r2) ? L_4fe9 : L_4fd1 */

L_4fd1:
    /* untranslated: branch (loword((uint32_t)(sext16to32(dx) * sext16to32(dx))) + ss:[bp-0x2c]) > LOWORD(r2) ? L_4fe9 : L_4fd6 */

L_4fd6:

L_4fdc:
    i = (i + 1);

L_4fe0:
    if ((i < 4))
        goto L_4f13;
    else
        goto L_4fe9;

L_4fe9:
    if ((i != 4))
        goto L_509d;
    else
        goto L_4ff2;

L_4ff2:
    pdc->fCovered = 1;
    pdc->cCur = 0;

DrawEllipse:
    if ((pdc->fHollowOut == 0))
        goto L_5078;
    else
        goto L_500e;

L_500e:
    SetROP2(pdc->hdc, 0x9);
    crSav = SetBkColor(pdc->hdc, 0xffffff);
    Ellipse(pdc->hdc, prc->left, prc->top, prc->right, prc->bottom);
    SetBkColor(pdc->hdc, crSav);
    SetROP2(pdc->hdc, 0xf);

L_5078:
    Ellipse(pdc->hdc, prc->left, prc->top, prc->right, prc->bottom);
    goto L_5405;

L_509d:
    if ((pdc->cCur == pdc->cMax))
        goto DrawEllipse;
    else
        goto L_50ab;

L_50ab:

L_50b1:
    iFree = pdc->cCur;
    i = 0;
    goto L_50c6;

L_50c2:
    i = (i + 1);

L_50c6:
    if ((i >= pdc->cCur))
        goto L_528b;
    else
        goto L_50d4;

L_50d4:
    if ((pdc->rgrad[i] > 0))
        goto L_50f2;
    else
        goto L_50e9;

L_50e9:
    iFree = i;
    goto L_50c2;

L_50f2:
    x2 = pdc->rgx[i];
    y2 = pdc->rgy[i];
    dx = (x - x2);
    dy = (y - y2);
    l = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((pdc->rgrad[i] >= rad))
        goto L_51f1;
    else
        goto L_5176;

L_5176:
    if (((sqrt((double)(l)) + (double)((int32_t)(pdc->rgrad[i]))) > (double)((int32_t)(rad))))
        goto L_50c2;
    else
        goto L_51d7;

L_51d7:
    pdc->rgrad[i] = 0;
    iFree = i;

L_51f1:
    if ((pdc->rgrad[i] <= rad))
        goto L_526f;
    else
        goto L_5208;

L_5208:
    if (((sqrt((double)(l)) + (double)((int32_t)(rad))) <= (double)((int32_t)(pdc->rgrad[i]))))
        goto L_5405;
    else
        goto L_5266;

L_5266:

L_526f:
    if ((x2 != x))
        goto L_50c2;
    else
        goto L_527a;

L_527a:
    if ((y2 == y))
        goto L_5405;
    else
        goto L_5282;

L_5282:

L_528b:
    pdc->rgx[iFree] = x;
    pdc->rgy[iFree] = y;
    pdc->rgrad[iFree] = rad;
    if ((iFree != pdc->cCur))
        goto L_5405;
    else
        goto L_52ce;

L_52ce:
    pdc->cCur = (pdc->cCur + 1);

L_52d8:
    if ((pdc->fHollowOut == 0))
        goto L2ndEl;
    else
        goto L_52e4;

L_52e4:
    SetROP2(pdc->hdc, 0x9);
    crSav = SetBkColor(pdc->hdc, 0xffffff);
    l = 1;

L2ndEl:
    i = 0;
    goto L_5322;

L_531e:
    i = (i + 1);

L_5322:
    if ((i >= pdc->cCur))
        goto L_53aa;
    else
        goto L_5330;

L_5330:
    if ((pdc->rgrad[i] <= 0))
        goto L_531e;
    else
        goto L_5345;

L_5345:
    x = pdc->rgx[i];
    y = pdc->rgy[i];
    rad = pdc->rgrad[i];
    Ellipse(pdc->hdc, (x - rad), (y - rad), ((x + rad) + 0x1), ((y + rad) + 0x1));

L_53aa:
    if ((pdc->fHollowOut == 0))
        goto L_53f5;
    else
        goto L_53b6;

L_53b6:
    if ((LOWORD(l) != 0x1))
        goto L_53f5;
    else
        goto L_53bf;

L_53bf:
    if ((HIWORD(l) != 0x0))
        goto L_53f5;
    else
        goto L_53c8;

L_53c8:
    l = 0;
    SetBkColor(pdc->hdc, crSav);
    SetROP2(pdc->hdc, 0xf);
    goto L2ndEl;

L_53f5:
    pdc->fCovered = 0;
    pdc->cCur = 0;

L_5405:
    return;
}

void DrawShipScanPath(HDC hdc, int16_t fShow) {
    ORDER   *lpord2;
    int16_t  rgDup[87];
    int16_t  j;
    HPEN     hpenSav;
    POINT    pt2;
    POINT    pt;
    int16_t  iRopSav;
    int16_t  dy;
    ORDER   *lpord1;
    POINT    ptCur;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  fHdc;
    int32_t  lWarp2;
    int16_t  dRad;
    int16_t  dx;
    RECT     rc;
    THING   *lpth;
    double   dAngle;
    POINT    rgptArrow[2];
    int16_t  dx5;
    POINT    ptTick;
    int16_t  dy5;
    double   m;
    int16_t  id;
    int16_t  fDoneRoute;
    uint16_t t_merge_577b_0001;
    uint16_t t_merge_58d6_0001;
    uint16_t t_merge_5977_0001;
    uint16_t t_merge_60fb_0001;
    uint16_t t_merge_61b1_0001;
    HPEN     t_merge_624f_0001;

L_540c:
    fHdc = 0;
    if ((sel.grobj == grobjFleet))
        goto L_5475;
    else
        goto L_5425;

L_5425:
    if ((sel.scan.grobj == grobjFleet))
        goto L_5475;
    else
        goto L_542f;

L_542f:
    if ((sel.scan.grobj == grobjThing))
        goto L_5475;
    else
        goto L_5439;

L_5439:
    if ((sel.grobj != grobjPlanet))
        goto L_62d2;
    else
        goto L_5443;

L_5443:
    if ((sel.pl.fStarbase == 0x0))
        goto L_5464;
    else
        goto L_5456;

L_5456:
    if ((sel.pl.idFling != 0x0))
        goto L_5475;
    else
        goto L_5464;

L_5464:
    if ((sel.pl.idRoute == 0x0))
        goto L_62d2;
    else
        goto L_546f;

L_546f:

L_5475:
    if ((fShow == fOrdersVis))
        goto L_62d2;
    else
        goto L_5480;

L_5480:
    if ((gd.fNoScannerDraw != 0x0))
        goto L_62d2;
    else
        goto L_548b;

L_548b:

L_5491:
    fOrdersVis = fShow;
    if ((hdc != 0x0))
        goto L_54b2;
    else
        goto L_54a0;

L_54a0:
    hdc = GetDC(hwndScanner);
    fHdc = 1;

L_54b2:
    if ((sel.scan.grobj != grobjThing))
        goto LNextCheck;
    else
        goto L_54bc;

L_54bc:
    if ((sel.scan.ith == -1))
        goto LNextCheck;
    else
        goto L_54c6;

L_54c6:
    lpth = &(lpThings[sel.scan.ith]);
    ptCur.x = lpth->pt.x;
    ptCur.y = lpth->pt.y;
    if ((lpth->ith != ithMysteryTrader))
        goto L_5543;
    else
        goto L_550a;

L_550a:
    dx = (lpth->tht.ptDest.x - ptCur.x);
    dy = (lpth->tht.ptDest.y - ptCur.y);
    LOWORD(lWarp2) = lpth->tht.iWarp;
    HIWORD(lWarp2) = 0x0;
    goto L_55d1;

L_5543:
    if ((lpth->ith != ithMineralPacket))
        goto LNextCheck;
    else
        goto L_555a;

L_555a:
    if ((lpth->thp.iWarp == 0x0))
        goto LNextCheck;
    else
        goto L_5572;

L_5572:
    dx = (rgptPlan[lpth->thp.idPlanet].x - ptCur.x);
    dy = (rgptPlan[lpth->thp.idPlanet].y - ptCur.y);
    LOWORD(lWarp2) = (lpth->thp.iWarp + 0x4);
    HIWORD(lWarp2) = 0x0;
    goto L_55d1;

L_55d1:
    if ((dx != 0))
        goto L_55e8;
    else
        goto L_55db;

L_55db:
    if ((dy == 0))
        goto LNextCheck;
    else
        goto L_55e2;

L_55e2:

L_55e8:
    lWarp2 = (uint32_t)((lWarp2 * (uint32_t)((lWarp2 * 5))));
    lpfl = 0x0;
    goto LCommonLineCode;

LNextCheck:
    if ((sel.scan.grobj != grobjFleet))
        goto LNoObjPath;
    else
        goto L_562d;

L_562d:
    lpfl = rglpfl[sel.scan.ifl];
    ptCur.x = lpfl->pt.x;
    ptCur.y = lpfl->pt.y;
    if ((lpfl->fdirValid == 0x0))
        goto LNoObjPath;
    else
        goto L_567e;

L_567e:
    if ((lpfl->iwarpFlt <= 0x0))
        goto LNoObjPath;
    else
        goto L_5691;

L_5691:
    dx = (lpfl->dirFltX - 127);
    dy = (lpfl->dirFltY - 127);
    if ((dx != 0))
        goto L_56d1;
    else
        goto L_56c4;

L_56c4:
    if ((dy == 0))
        goto LNoObjPath;
    else
        goto L_56cb;

L_56cb:

L_56d1:
    LOWORD(lWarp2) = LOWORD((LOWORD((lpfl->iwarpFlt * lpfl->iwarpFlt)) * 0x5));
    HIWORD(lWarp2) = 0x0;

LCommonLineCode:
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, rc.left, (rc.bottom - dySBar), rc.right, rc.bottom);
    hpenSav = SelectObject(hdc, hpenStarbase);
    iRopSav = SetROP2(hdc, 0x7);
    if ((dx != 0))
        goto L_5782;
    else
        goto L_5751;

L_5751:
    dx5 = 0;
    if ((dy >= 0))
        goto L_5773;
    else
        goto L_5761;

L_5761:
    /* untranslated: t_merge_577b_0001 = (LOWORD(lWarp2) neg 0x0) */
    goto L_577b;

L_5773:
    t_merge_577b_0001 = LOWORD(lWarp2);

L_577b:
    dy5 = t_merge_577b_0001;
    goto L_5850;

L_5782:
    lWarp2 = (uint32_t)((lWarp2 * lWarp2));
    m = ((double)((int32_t)(dy)) / (double)((int32_t)(dx)));
    sqrt(((double)(lWarp2) / ((m * m) + (double)(1))));
    dx5 = LOWORD(__ftol());
    if ((dx >= 0))
        goto L_582b;
    else
        goto L_5821;

L_5821:
    /* untranslated: dx5 = (dx5 neg 0) */

L_582b:
    dy5 = LOWORD((int32_t)(((uint32_t)(((uint32_t)(dx5) * (uint32_t)(dy))) / (uint32_t)(dx))));

L_5850:
    LogicalToScan(&(ptCur));
    dx5 = PtToScan(dx5);
    /* untranslated: dy5 = (PtToScan(dy5) neg 0) */
    MoveTo(hdc, (ptCur.x - dx5), (ptCur.y - dy5));
    LineTo(hdc, (ptCur.x + dx5), (ptCur.y + dy5));
    if ((dy != 0))
        goto L_58dd;
    else
        goto L_58bd;

L_58bd:
    ptTick.x = 0;
    if ((dx >= 0))
        goto L_58d3;
    else
        goto L_58cd;

L_58cd:
    t_merge_58d6_0001 = 0xfffc;
    goto L_58d6;

L_58d3:
    t_merge_58d6_0001 = 0x4;

L_58d6:
    ptTick.y = t_merge_58d6_0001;
    goto L_59b7;

L_58dd:
    /* untranslated: m = ((double)sext16to32((dx neg 0x0)) / (double)sext16to32(dy)) */
    sqrt((24 / ((m * m) + (double)(1))));
    ptTick.x = LOWORD(__ftol());
    if ((ptTick.x != 0))
        goto L_597e;
    else
        goto L_5964;

L_5964:
    if ((dy >= 0))
        goto L_5974;
    else
        goto L_596e;

L_596e:
    t_merge_5977_0001 = 0xfffc;
    goto L_5977;

L_5974:
    t_merge_5977_0001 = 0x4;

L_5977:
    ptTick.y = t_merge_5977_0001;
    goto L_59b7;

L_597e:
    if ((dx <= 0))
        goto L_5992;
    else
        goto L_5988;

L_5988:
    /* untranslated: ptTick.x = (ptTick.x neg 0) */

L_5992:
    ptTick.y = LOWORD((int32_t)(((uint32_t)(((uint32_t)(ptTick.x) * (uint32_t)(dx))) / (uint32_t)(dy))));

L_59b7:
    /* untranslated: dAngle = (atan2((double)sext16to32((dy neg 0x0)), (double)sext16to32((dx neg 0x0))) - 0.7853982) */
    i = 0;
    goto L_5ae8;

L_5a1c:
    /* untranslated: 10-byte ss:[bp-0x116] = (double)5 */
    /* untranslated: call cos(words(part[6:2](dAngle), part[4:2](dAngle), HIWORD(dAngle), LOWORD(dAngle))) -> callresult(double) */
    rgptArrow[i].x = LOWORD(__ftol());
    /* untranslated: 10-byte ss:[bp-0x116] = (double)5 */
    /* untranslated: call sin(words(part[6:2](dAngle), part[4:2](dAngle), HIWORD(dAngle), LOWORD(dAngle))) -> callresult(double) */
    rgptArrow[i].y = LOWORD(__ftol());
    dAngle = (dAngle + 1.5707964);
    i = (i + 1);

L_5ae8:
    if ((i < 2))
        goto L_5a1c;
    else
        goto L_5af2;

L_5af2:
    j = -5;
    i = -5;
    goto L_5c68;

L_5b01:
    if ((i != 0))
        goto L_5b14;
    else
        goto L_5b0b;

L_5b0b:
    j = 5;
    goto L_5c63;

L_5b14:
    /* untranslated: pt.x = (loword((int32_t)(words((hiword((int32_t)((uint32_t)(sext16to32(dx5) * sext16to32(i)) * 0x2)) + signhiword(j)),
     * (loword((int32_t)((uint32_t)(sext16to32(dx5) * sext16to32(i)) * 0x2)) + j)) / 0xa)) + ptCur.x) */
    /* untranslated: pt.y = (loword((int32_t)(words((hiword((int32_t)((uint32_t)(sext16to32(dy5) * sext16to32(i)) * 0x2)) + signhiword(j)),
     * (loword((int32_t)((uint32_t)(sext16to32(dy5) * sext16to32(i)) * 0x2)) + j)) / 0xa)) + ptCur.y) */
    if ((i <= 0))
        goto L_5c12;
    else
        goto L_5bb4;

L_5bb4:
    j = 0;
    goto L_5c05;

L_5bbd:
    MoveTo(hdc, (pt.x + rgptArrow[j].x), (pt.y - rgptArrow[j].y));
    LineTo(hdc, pt.x, pt.y);
    j = (j + 1);

L_5c05:
    if ((j >= 2))
        goto L_5c63;
    else
        goto L_5c0c;

L_5c0c:

L_5c12:
    MoveTo(hdc, (pt.x + ptTick.x), (pt.y + ptTick.y));
    LineTo(hdc, (pt.x - ptTick.x), (pt.y - ptTick.y));
    LineTo(hdc, (pt.x - ptTick.x), ((pt.y - ptTick.y) + 0xffff));

L_5c63:
    i = (i + 1);

L_5c68:
    if ((i <= 5))
        goto L_5b01;
    else
        goto L_5c72;

L_5c72:
    SetROP2(hdc, iRopSav);
    SelectObject(hdc, hpenSav);

LNoObjPath:
    if ((sel.grobj != grobjPlanet))
        goto L_5df0;
    else
        goto L_5c94;

L_5c94:
    fDoneRoute = 0;
    if ((sel.pl.fStarbase == 0x0))
        goto L_5daf;
    else
        goto L_5cad;

L_5cad:
    if ((sel.pl.idFling == 0x0))
        goto L_5daf;
    else
        goto L_5cbb;

L_5cbb:
    hpenSav = SelectObject(hdc, hpenDkPurple);
    id = (sel.pl.idFling - 1);

LDrawPath:
    iRopSav = SetROP2(hdc, 0x7);
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, rc.left, (rc.bottom - dySBar), rc.right, rc.bottom);
    pt.x = rgptPlan[sel.pl.id].x;
    pt.y = rgptPlan[sel.pl.id].y;
    LogicalToScan(&(pt));
    MoveTo(hdc, pt.x, pt.y);
    pt.x = rgptPlan[id].x;
    pt.y = rgptPlan[id].y;
    LogicalToScan(&(pt));
    LineTo(hdc, pt.x, pt.y);
    SetROP2(hdc, iRopSav);
    SelectObject(hdc, hpenSav);

L_5daf:
    if ((sel.pl.idRoute == 0x0))
        goto LFinishUp;
    else
        goto L_5dbd;

L_5dbd:
    if ((fDoneRoute != 0))
        goto LFinishUp;
    else
        goto L_5dc7;

L_5dc7:
    fDoneRoute = 1;
    id = (sel.pl.idRoute - 1);
    hpenSav = SelectObject(hdc, hpenDkGreen);
    goto LDrawPath;

L_5df0:
    if ((sel.grobj != grobjFleet))
        goto LFinishUp;
    else
        goto L_5dfa;

L_5dfa:
    if ((sel.fl.cord <= 1))
        goto LFinishUp;
    else
        goto L_5e01;

L_5e01:

L_5e07:
    memset(rgDup, 0x0, (sel.fl.cord * 2));
    lpord1 = sel.fl.lpplord->rgord;
    pt.x = lpord1->pt.x;
    pt.y = lpord1->pt.y;
    i = 1;
    goto L_5e67;

L_5e4f:
    i = (i + 1);
    lpord1 = (lpord1 + 0x1);

L_5e67:
    if ((i >= sel.fl.cord))
        goto L_5fa2;
    else
        goto L_5e73;

L_5e73:
    lpord2 = &(lpord1[0x1]);
    pt2.x = lpord2->pt.x;
    pt2.y = lpord2->pt.y;
    if ((rgDup[i] != 0))
        goto DoNext;
    else
        goto L_5ea9;

L_5ea9:

L_5eaf:
    j = (i + 1);
    goto L_5ed2;

L_5ebd:
    j = (j + 1);
    lpord2 = (lpord2 + 0x1);

L_5ed2:
    if ((j >= sel.fl.cord))
        goto DoNext;
    else
        goto L_5ede;

L_5ede:
    /* untranslated: branch ss:[pt] != lpord2->pt.x ? L_5f25 : L_5ef8 */

L_5ef8:
    /* untranslated: branch ss:[pt+0x2] != lpord2->pt.y ? L_5f25 : L_5f01 */

L_5f01:
    /* untranslated: branch ss:[pt2] != lpord2[1].pt.x ? L_5f25 : L_5f1c */

L_5f1c:
    /* untranslated: branch ss:[pt2+0x2] == *(lpord2+0x14) ? L_5f6c : L_5f25 */

L_5f25:
    /* untranslated: branch ss:[pt2] != lpord2->pt.x ? L_5ebd : L_5f3f */

L_5f3f:
    /* untranslated: branch ss:[pt2+0x2] != lpord2->pt.y ? L_5ebd : L_5f48 */

L_5f48:
    /* untranslated: branch ss:[pt] != lpord2[1].pt.x ? L_5ebd : L_5f63 */

L_5f63:
    /* untranslated: branch ss:[pt+0x2] != *(lpord2+0x14) ? L_5ebd : L_5f6c */

L_5f6c:
    rgDup[i] = 1;
    rgDup[j] = 2;

DoNext:
    pt.x = pt2.x;
    pt.y = pt2.y;
    goto L_5e4f;

L_5fa2:
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, rc.left, (rc.bottom - dySBar), rc.right, rc.bottom);
    if ((fShow == 0))
        goto L_60a5;
    else
        goto L_5fd9;

L_5fd9:
    if (((grbitScan & 0x80) == 0x0))
        goto L_60a5;
    else
        goto L_5fe7;

L_5fe7:
    hpenSav = SelectObject(hdc, hpenStarbase);
    pt.x = sel.fl.lpplord->rgord[0x0].pt.x;
    pt.y = sel.fl.lpplord->rgord[0x0].pt.y;
    LogicalToScan(&(pt));
    MoveTo(hdc, pt.x, pt.y);
    i = 1;
    goto L_6036;

L_6031:
    i = (i + 1);

L_6036:
    if ((i >= sel.fl.cord))
        goto L_6099;
    else
        goto L_6042;

L_6042:
    pt2.x = sel.fl.lpplord->rgord[i].pt.x;
    pt2.y = sel.fl.lpplord->rgord[i].pt.y;
    LogicalToScan(&(pt2));
    LineTo(hdc, pt2.x, pt2.y);
    pt.x = pt2.x;
    pt.y = pt2.y;
    goto L_6031;

L_6099:
    SelectObject(hdc, hpenSav);

L_60a5:
    hpenSav = SelectObject(hdc, hpenShip);
    iRopSav = SetROP2(hdc, 0x7);
    pt.x = sel.fl.lpplord->rgord[0x0].pt.x;
    pt.y = sel.fl.lpplord->rgord[0x0].pt.y;
    if ((pt.x != sel.pt.x))
        goto L_60f8;
    else
        goto L_60e9;

L_60e9:
    if ((pt.y != sel.pt.y))
        goto L_60f8;
    else
        goto L_60f2;

L_60f2:
    t_merge_60fb_0001 = 0x5;
    goto L_60fb;

L_60f8:
    t_merge_60fb_0001 = 0x5;

L_60fb:
    dRad = t_merge_60fb_0001;
    LogicalToScan(&(pt));
    ExcludeClipRect(hdc, (pt.x - dRad), (pt.y - dRad), ((pt.x + dRad) + 0x1), ((pt.y + dRad) + 0x1));
    MoveTo(hdc, pt.x, pt.y);
    i = 1;
    goto L_615c;

L_6157:
    i = (i + 1);

L_615c:
    if ((i >= sel.fl.cord))
        goto L_6298;
    else
        goto L_6168;

L_6168:
    pt2.x = sel.fl.lpplord->rgord[i].pt.x;
    pt2.y = sel.fl.lpplord->rgord[i].pt.y;
    if ((pt2.x != sel.pt.x))
        goto L_61ae;
    else
        goto L_619f;

L_619f:
    if ((pt2.y != sel.pt.y))
        goto L_61ae;
    else
        goto L_61a8;

L_61a8:
    t_merge_61b1_0001 = 0x5;
    goto L_61b1;

L_61ae:
    t_merge_61b1_0001 = 0x5;

L_61b1:
    dRad = t_merge_61b1_0001;
    LogicalToScan(&(pt2));
    ExcludeClipRect(hdc, (pt2.x - dRad), (pt2.y - dRad), ((pt2.x + dRad) + 0x1), ((pt2.y + dRad) + 0x1));
    if ((rgDup[i] != 2))
        goto L_621b;
    else
        goto L_6208;

L_6208:
    MoveTo(hdc, pt2.x, pt2.y);
    goto L_6285;

L_621b:
    if ((rgDup[i] != 1))
        goto L_6255;
    else
        goto L_622f;

L_622f:
    if (((grbitScan & 0x80) == 0x0))
        goto L_6246;
    else
        goto L_6240;

L_6240:
    t_merge_624f_0001 = hpenYellow;
    goto L_624f;

L_6246:
    t_merge_624f_0001 = GetStockObject(WHITE_PEN);

L_624f:
    SelectObject(hdc, t_merge_624f_0001);

L_6255:
    LineTo(hdc, pt2.x, pt2.y);
    if ((rgDup[i] != 1))
        goto L_6285;
    else
        goto L_6279;

L_6279:
    SelectObject(hdc, hpenShip);

L_6285:
    pt.x = pt2.x;
    pt.y = pt2.y;
    goto L_6157;

L_6298:
    SetROP2(hdc, iRopSav);
    SelectObject(hdc, hpenSav);
    SelectClipRgn(hdc, hrgnHuge);

LFinishUp:
    if ((fHdc == 0))
        goto L_62d2;
    else
        goto L_62c6;

L_62c6:
    ReleaseDC(hwndScanner, hdc);

L_62d2:
    return;
}

void DrawScannerSBar(HDC hdc, RECT *prc, SBAR *psbar, int16_t fFullRedraw) {
    int16_t    fhdc;
    COLORREF   crText;
    POINT      pt2;
    int16_t    id;
    POINT      pt;
    int16_t    grReal;
    int16_t    iBkPrev;
    int16_t    c;
    COLORREF   crBk;
    int16_t    dxHole;
    HFONT      hfontSav;
    RECT       rcClip;
    char      *psz;
    HBRUSH     hbrSav;
    int16_t    fDoName;
    int32_t    l;
    GrobjClass grobj;
    RECT       rcT;
    RECT       rc;
    char       szBuf[100];
    GrobjClass t_merge_6730_0001;
    int16_t    t_merge_6780_0001;
    int16_t    t_merge_67c5_0001;
    int16_t    t_merge_6807_0001;
    uint16_t   t_merge_6a00_0001;

L_62d8:
    fhdc = 0;
    fDoName = 1;
    GetClientRect(hwndScanner, &(rc));
    rc.top = (rc.bottom - dySBar);
    if ((prc == 0x0))
        goto L_6333;
    else
        goto L_630e;

L_630e:
    if ((IntersectRect(&(rcT), prc, &(rc)) == 0))
        goto L_6af9;
    else
        goto L_632d;

L_632d:

L_6333:
    if ((hdc != 0x0))
        goto L_634d;
    else
        goto L_633c;

L_633c:
    fhdc = 1;
    hdc = GetDC(hwndScanner);

L_634d:
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    if ((psbar == 0x0))
        goto L_6370;
    else
        goto L_6365;

L_6365:
    grobj = psbar->grbit;
    goto L_6376;

L_6370:
    grobj = sel.scan.grobj;

L_6376:
    iBkPrev = SetBkMode(hdc, TRANSPARENT);
    crBk = SetBkColor(hdc, crButtonFace);
    crText = SetTextColor(hdc, crButtonText);
    hbrSav = SelectObject(hdc, hbrButtonFace);
    if ((fFullRedraw == 0))
        goto L_6435;
    else
        goto L_63c9;

L_63c9:
    PatBlt(hdc, rc.left, rc.top, (rc.right - rc.left), dySBar, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rc.left, rc.top, 0x1, dySBar, PATCOPY);
    PatBlt(hdc, rc.left, rc.top, (rc.right - rc.left), 0x1, PATCOPY);

L_6435:
    rc.bottom = (rc.bottom - (dySBar >> 0x1));
    rcT = rc;
    rcT.top = (rcT.top + 4);
    rcT.bottom = (rcT.bottom - 4);
    rcT.left = (rcT.left + 4);
    if ((rc.right < 360))
        goto DrawTheName;
    else
        goto L_645f;

L_645f:

L_6465:
    l = GetTextExtent(hdc, "ID #000", 0x7);
    dxHole = (LOWORD(l) + 6);
    rcT.right = ((LOWORD(l) + 6) + rcT.left);
    DrawLockLight(hdc, &(rcT), fFullRedraw);
    if (((grobj & 0x1) == 0x0))
        goto L_6520;
    else
        goto L_64ad;

L_64ad:
    if ((psbar == 0x0))
        goto L_64c2;
    else
        goto L_64b6;

L_64b6:
    id = psbar->id;
    goto L_64c8;

L_64c2:
    id = sel.scan.idpl;

L_64c8:
    if ((id == -1))
        goto L_6580;
    else
        goto L_64d1;

L_64d1:
    c = _wsprintf(szWork, "ID #%d", (id + 1));
    SetBkMode(hdc, OPAQUE);
    TextOut(hdc, (rcT.left + 3), (rcT.top + 2), szWork, c);

L_6520:
    if ((grobj != grobjOther))
        goto L_6580;
    else
        goto L_6529;

L_6529:
    if ((psbar == 0x0))
        goto L_653e;
    else
        goto L_6532;

L_6532:
    id = psbar->id;
    goto L_6544;

L_653e:
    id = sel.scan.iwp;

L_6544:
    c = _wsprintf(szWork, "WP #%d", id);
    TextOut(hdc, (rcT.left + 3), (rcT.top + 2), szWork, c);

L_6580:
    l = GetTextExtent(hdc, "X: 8888", 0x7);
    rcT.left = (rcT.left + (dxHole + 4));
    dxHole = (LOWORD(l) + 6);
    rcT.right = ((LOWORD(l) + 6) + rcT.left);
    DrawLockLight(hdc, &(rcT), fFullRedraw);
    if ((psbar == 0x0))
        goto L_65de;
    else
        goto L_65cc;

L_65cc:
    pt.x = psbar->pt.x;
    pt.y = psbar->pt.y;
    goto GotCoords;

L_65de:
    if ((grobj == grobjNone))
        goto L_65f7;
    else
        goto L_65e7;

L_65e7:
    pt.x = sel.scan.pt.x;
    pt.y = sel.scan.pt.y;
    goto GotCoords;

L_65f7:
    pt.y = 0;
    pt.x = 0;

GotCoords:
    if ((pt.x <= 0))
        goto L_6653;
    else
        goto L_660b;

L_660b:
    c = _wsprintf(szWork, "X: %d", pt.x);
    SetBkMode(hdc, OPAQUE);
    TextOut(hdc, (rcT.left + 3), (rcT.top + 2), szWork, c);

L_6653:
    rcT.left = (rcT.left + (dxHole + 4));
    dxHole = (LOWORD(l) + 6);
    rcT.right = ((LOWORD(l) + 6) + rcT.left);
    DrawLockLight(hdc, &(rcT), fFullRedraw);
    if ((pt.y <= 0))
        goto L_66ce;
    else
        goto L_6686;

L_6686:
    c = _wsprintf(szWork, "Y: %d", pt.y);
    SetBkMode(hdc, OPAQUE);
    TextOut(hdc, (rcT.left + 3), (rcT.top + 2), szWork, c);

L_66ce:
    rcT.left = (rcT.left + (dxHole + 4));

DrawTheName:
    dxHole = ((rc.right - rcT.left) - 4);
    rcT.right = (((rc.right - rcT.left) - 4) + rcT.left);
    DrawLockLight(hdc, &(rcT), fFullRedraw);
    rcClip = rcT;
    ExpandRc(&(rcClip), 0xfffe, 0xfffe);
    if ((grobj != grobjOther))
        goto L_672d;
    else
        goto L_6727;

L_6727:
    t_merge_6730_0001 = sel.scan.grobjFull;
    goto L_6730;

L_672d:
    t_merge_6730_0001 = grobj;

L_6730:
    grReal = t_merge_6730_0001;
    if ((psbar == 0x0))
        goto L_675d;
    else
        goto L_673c;

L_673c:
    if ((psbar->psz == 0x0))
        goto L_675d;
    else
        goto L_6748;

L_6748:
    strcpy(szWork, psbar->psz);
    goto L_683d;

L_675d:
    if (((grReal & 0x1) == 0x0))
        goto L_678c;
    else
        goto L_676b;

L_676b:
    if ((psbar == 0x0))
        goto L_677d;
    else
        goto L_6774;

L_6774:
    t_merge_6780_0001 = psbar->id;
    goto L_6780;

L_677d:
    t_merge_6780_0001 = sel.scan.idpl;

L_6780:
    PszGetPlanetName(t_merge_6780_0001);
    goto L_683d;

L_678c:
    if (((grReal & 0x2) == 0x0))
        goto L_67d1;
    else
        goto L_679a;

L_679a:
    if ((psbar == 0x0))
        goto L_67ac;
    else
        goto L_67a3;

L_67a3:
    t_merge_67c5_0001 = psbar->id;
    goto L_67c5;

L_67ac:
    t_merge_67c5_0001 = rglpfl[sel.scan.ifl]->id;

L_67c5:
    PszGetFleetName(t_merge_67c5_0001);
    goto L_683d;

L_67d1:
    if (((grReal & 0x8) == 0x0))
        goto L_6813;
    else
        goto L_67df;

L_67df:
    if ((psbar == 0x0))
        goto L_67f1;
    else
        goto L_67e8;

L_67e8:
    t_merge_6807_0001 = psbar->id;
    goto L_6807;

L_67f1:
    t_merge_6807_0001 = lpThings[sel.scan.ith].idFull;

L_6807:
    PszGetThingName(t_merge_6807_0001);
    goto L_683d;

L_6813:
    if ((grReal != 0))
        goto L_6838;
    else
        goto L_681c;

L_681c:
    if ((grobj != grobjOther))
        goto L_6838;
    else
        goto L_6825;

L_6825:
    CchGetString(idsDeepSpaceWaypoint, szWork);
    goto L_683d;

L_6838:
    fDoName = 0;

L_683d:
    if ((fDoName == 0))
        goto L_6883;
    else
        goto L_6846;

L_6846:
    ExtTextOut(hdc, (rcT.left + 3), (rcT.top + 2), 0x4, &(rcClip), szWork, lstrlen(szWork), 0x0);

L_6883:
    OffsetRc(&(rc), 0x0, (dySBar >> 0x1));
    rcT = rc;
    rcT.top = (rcT.top + 4);
    rcT.bottom = (rcT.bottom - 4);
    rcT.left = (rcT.left + 4);
    rcT.right = (rcT.right - 4);
    DrawLockLight(hdc, &(rcT), fFullRedraw);
    rcClip = rcT;
    ExpandRc(&(rcClip), 0xfffe, 0xfffe);
    if ((psbar == 0x0))
        goto L_6908;
    else
        goto L_68f6;

L_68f6:
    pt.x = psbar->pt.x;
    pt.y = psbar->pt.y;
    goto L_692c;

L_6908:
    if ((grobj == grobjNone))
        goto L_6921;
    else
        goto L_6911;

L_6911:
    pt.x = sel.scan.pt.x;
    pt.y = sel.scan.pt.y;
    goto L_692c;

L_6921:
    pt.y = 0;
    pt.x = 0;

L_692c:
    if ((psbar == 0x0))
        goto L_6955;
    else
        goto L_6935;

L_6935:
    if ((psbar->pscan == 0x0))
        goto L_6955;
    else
        goto L_6941;

L_6941:
    pt2.x = psbar->pscan->pt.x;
    pt2.y = psbar->pscan->pt.y;
    goto L_697e;

L_6955:
    if ((sel.grobj == grobjFleet))
        goto L_6969;
    else
        goto L_695f;

L_695f:
    if ((sel.grobj != grobjPlanet))
        goto L_6979;
    else
        goto L_6969;

L_6969:
    pt2.x = sel.pt.x;
    pt2.y = sel.pt.y;
    goto L_697e;

L_6979:
    pt2.x = -1;

L_697e:
    if ((pt.x == -1))
        goto L_6aa5;
    else
        goto L_6987;

L_6987:
    if ((pt2.x == -1))
        goto L_6aa5;
    else
        goto L_6990;

L_6990:
    if ((pt.x != pt2.x))
        goto L_69a6;
    else
        goto L_699e;

L_699e:
    if ((pt.y == pt2.y))
        goto L_6aa5;
    else
        goto L_69a6;

L_69a6:
    strcpy(szBuf, PszGetDistance(pt.x, pt.y, pt2.x, pt2.y));
    psz = szBuf;

L_69cf:
    if (((uint16_t)(*(psz)) == 32))
        goto L_69e4;
    else
        goto L_69dd;

L_69dd:
    psz = (psz + 0x1);
    goto L_69cf;

L_69e4:
    if ((rc.right < 350))
        goto L_69fd;
    else
        goto L_69f7;

L_69f7:
    t_merge_6a00_0001 = 0x1;
    goto L_6a00;

L_69fd:
    t_merge_6a00_0001 = 0x0;

L_6a00:
    CchGetString((t_merge_6a00_0001 + 0x556), &(psz[0x1]));
    if ((psbar == 0x0))
        goto L_6a21;
    else
        goto L_6a15;

L_6a15:
    if ((psbar->pscan != 0x0))
        goto L_6a66;
    else
        goto L_6a21;

L_6a21:
    CchGetString(idsFrom, &(psz[strlen(psz)]));
    strcat(&(psz[0x8]), PszGetLocName(sel.grobj, sel.id, pt2.x, pt2.y));

L_6a66:
    ExtTextOut(hdc, (rcT.left + 3), (rcT.top + 2), 0x4, &(rcClip), szBuf, strlen(szBuf), 0x0);

L_6aa5:
    SelectObject(hdc, hbrSav);
    SetBkMode(hdc, iBkPrev);
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crBk);
    SelectObject(hdc, hfontSav);
    if ((fhdc == 0))
        goto L_6af9;
    else
        goto L_6aed;

L_6aed:
    ReleaseDC(hwndScanner, hdc);

L_6af9:
    return;
}

void DrawLockLight(HDC hdc, RECT *prc, int16_t fFullRedraw) {
    int16_t dy;
    int16_t dx;
    RECT    rc;

L_6b00:
    dx = (prc->right - prc->left);
    dy = (prc->bottom - prc->top);
    if ((fFullRedraw == 0))
        goto L_6bd7;
    else
        goto L_6b2f;

L_6b2f:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, prc->left, prc->top, 0x1, dy, PATCOPY);
    PatBlt(hdc, prc->left, prc->top, dx, 0x1, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, prc->right, prc->top, 0x1, (dy + 1), PATCOPY);
    PatBlt(hdc, prc->left, prc->bottom, dx, 0x1, PATCOPY);
    goto L_6c0d;

L_6bd7:
    rc = *(prc);
    ExpandRc(&(rc), 0xfffe, 0xfffe);
    FillRect(hdc, &(rc), hbrButtonFace);

L_6c0d:
    return;
}

void SetScanScrollBars(HWND hwnd) {
    int16_t  xMax;
    int16_t  dy;
    int16_t  yMax;
    int16_t  dx;
    RECT     rc;
    uint16_t t_merge_6c75_0001;
    uint16_t t_merge_6ca0_0001;
    int16_t  t_merge_6d05_0001;
    int16_t  t_merge_6d05_0002;

L_6c14:
    fInScrollSet = 1;
    GetClientRect(hwnd, &(rc));
    dx = ScanToPt(rc.right);
    dy = ScanToPt((rc.bottom - dySBar));
    if ((0x3e8 <= ((dGalInv - 1000) - dx)))
        goto L_6c6c;
    else
        goto L_6c66;

L_6c66:
    t_merge_6c75_0001 = 0x3e8;
    goto L_6c75;

L_6c6c:
    t_merge_6c75_0001 = ((dGalInv - 1000) - dx);

L_6c75:
    xMax = ((t_merge_6c75_0001 + 0x3) & 0xfffc);
    if ((0x3e8 <= ((dGalInv - 1000) - dy)))
        goto L_6c97;
    else
        goto L_6c91;

L_6c91:
    t_merge_6ca0_0001 = 0x3e8;
    goto L_6ca0;

L_6c97:
    t_merge_6ca0_0001 = ((dGalInv - 1000) - dy);

L_6ca0:
    yMax = ((t_merge_6ca0_0001 + 0x3) & 0xfffc);
    SetScrollRange(hwnd, 0x0, 0x3e8, xMax, 0x1);
    if ((fInScrollSet == 0))
        goto L_6d29;
    else
        goto L_6cc7;

L_6cc7:

L_6ccd:
    SetScrollRange(hwnd, 0x1, 0x3e8, yMax, 0x1);
    if ((fInScrollSet == 0))
        goto L_6d29;
    else
        goto L_6ceb;

L_6ceb:

L_6cf1:
    if ((dx >= dy))
        goto L_6d02;
    else
        goto L_6cfc;

L_6cfc:
    t_merge_6d05_0001 = dx;
    t_merge_6d05_0002 = dx;
    goto L_6d05;

L_6d02:
    t_merge_6d05_0001 = dy;
    t_merge_6d05_0002 = dy;

L_6d05:
    /* untranslated: dScanPage = ((words(t_merge_6d05_0001, signhiword(t_merge_6d05_0002)) / 0x3) & 0xfffc) */
    dScanInc = ((((uint32_t)(dScanPage) / 0x8) + 0x2) & 0xfffc);
    fInScrollSet = 0;

L_6d29:
    return;
}

void ScrollScanner(int16_t dx, int16_t dy) {
    HDC  hdc;
    RECT rcUpd;
    RECT rcUpd2;
    RECT rc;

L_6d30:
    if ((dx != 0))
        goto L_6d4b;
    else
        goto L_6d42;

L_6d42:
    if ((dy == 0))
        goto L_6f2a;
    else
        goto L_6d4b;

L_6d4b:
    if ((IsWindowVisible(hwndScanner) == 0))
        goto L_6f2a;
    else
        goto L_6d5c;

L_6d5c:
    if ((gd.fNoScannerDraw != 0x0))
        goto L_6f2a;
    else
        goto L_6d67;

L_6d67:

L_6d6d:
    hdc = GetDC(hwndScanner);
    GetClientRect(hwndScanner, &(rc));
    if ((abs(dx) > (rc.right >> 0x1)))
        goto L_6dcb;
    else
        goto L_6da0;

L_6da0:
    if ((abs(dy) > (rc.bottom >> 0x1)))
        goto L_6dcb;
    else
        goto L_6db7;

L_6db7:
    if ((fDlgUp != 0))
        goto L_6dcb;
    else
        goto L_6dc1;

L_6dc1:
    if ((hwndBrowser == 0x0))
        goto L_6ddd;
    else
        goto L_6dcb;

L_6dcb:
    DrawScanner(hdc, &(rc));
    goto RelDC;

L_6ddd:
    rc.bottom = (rc.bottom - dySBar);
    UpdateWindow(hwndScanner);
    ScrollWindow(hwndScanner, dx, dy, &(rc), &(rc));
    if ((dy <= 0))
        goto L_6e35;
    else
        goto L_6e12;

L_6e12:
    SetRect(&(rcUpd2), 0x0, 0x0, rc.right, dy);
    rc.top = (rc.top + dy);
    goto L_6e61;

L_6e35:
    if ((dy >= 0))
        goto L_6e61;
    else
        goto L_6e3e;

L_6e3e:
    SetRect(&(rcUpd2), 0x0, (rc.bottom + dy), rc.right, rc.bottom);
    rc.bottom = (rc.bottom + dy);

L_6e61:
    if ((dx <= 0))
        goto L_6e8c;
    else
        goto L_6e6a;

L_6e6a:
    SetRect(&(rcUpd), 0x0, rc.top, dx, rc.bottom);
    rc.right = (rc.right - dx);
    goto L_6eb3;

L_6e8c:
    if ((dx >= 0))
        goto L_6eb3;
    else
        goto L_6e95;

L_6e95:
    rcUpd = rc;
    rcUpd.left = (rc.right + dx);
    rc.left = (rc.left - dx);

L_6eb3:
    if ((dx == 0))
        goto L_6ecc;
    else
        goto L_6ebc;

L_6ebc:
    ValidateRect(hwndScanner, &(rcUpd));

L_6ecc:
    if ((dy == 0))
        goto L_6ee5;
    else
        goto L_6ed5;

L_6ed5:
    ValidateRect(hwndScanner, &(rcUpd2));

L_6ee5:
    if ((dx == 0))
        goto L_6efd;
    else
        goto L_6eee;

L_6eee:
    DrawScanner(hdc, &(rcUpd));

L_6efd:
    if ((dy == 0))
        goto L_6f15;
    else
        goto L_6f06;

L_6f06:
    DrawScanner(hdc, &(rcUpd2));

L_6f15:
    UpdateWindow(hwndScanner);

RelDC:
    ReleaseDC(hwndScanner, hdc);

L_6f2a:
    return;
}

void RedrawScanSel(HDC hdc, int16_t fVis) {
    int16_t  sel_grobj;
    int16_t  fhdc;
    int16_t  dOff;
    POINT    pt;
    int16_t  sel_id;
    int16_t  fNoSelRedraw;
    SCAN     sel_scan;
    RECT     rc;
    int16_t  sel_grobjFull;
    uint16_t t_merge_7029_0001;

L_6f30:
    fhdc = 0;
    sel_id = sel.id;
    sel_grobj = sel.grobj;
    sel_grobjFull = sel.grobjFull;
    sel_scan = sel.scan;
    if ((hwndScanner == 0x0))
        goto L_7155;
    else
        goto L_6f6a;

L_6f6a:
    if ((IsWindowVisible(hwndScanner) == 0))
        goto L_7155;
    else
        goto L_6f78;

L_6f78:

L_6f7e:
    if ((fVis != -1))
        goto L_6f94;
    else
        goto L_6f87;

L_6f87:
    fVis = 0;
    fNoSelRedraw = 1;
    goto L_6f99;

L_6f94:
    fNoSelRedraw = 0;

L_6f99:
    if ((hdc != 0x0))
        goto L_6fb3;
    else
        goto L_6fa2;

L_6fa2:
    fhdc = 1;
    hdc = GetDC(hwndScanner);

L_6fb3:
    DrawShipScanPath(hdc, fVis);
    if ((fVis != 0))
        goto L_6ff4;
    else
        goto L_6fca;

L_6fca:
    sel.scan.iwp = -1;
    sel.scan.ifl = -1;
    sel.scan.idpl = -1;
    sel.id = -1;
    sel.grobjFull = grobjNone;
    sel.grobj = grobjNone;
    sel.scan.grobjFull = grobjNone;
    sel.scan.grobj = grobjNone;

L_6ff4:
    if ((iScanZoom < 3))
        goto L_7012;
    else
        goto L_6ffe;

L_6ffe:
    if (((grbitScan & 0xf) != 0x4))
        goto L_7012;
    else
        goto L_700c;

L_700c:
    t_merge_7029_0001 = 0xb;
    goto L_7029;

L_7012:
    if (((grbitScan & 0x1000) == 0x0))
        goto L_7026;
    else
        goto L_7020;

L_7020:
    t_merge_7029_0001 = 0x7;
    goto L_7029;

L_7026:
    t_merge_7029_0001 = 0x0;

L_7029:
    dOff = t_merge_7029_0001;
    if ((sel_grobj == 0))
        goto L_709a;
    else
        goto L_7035;

L_7035:
    if ((fNoSelRedraw != 0))
        goto L_709a;
    else
        goto L_703e;

L_703e:
    pt.x = sel.pt.x;
    pt.y = sel.pt.y;
    LogicalToScan(&(pt));
    SetRect(&(rc), ((pt.x - 11) - dOff), ((pt.y - 11) - dOff), ((pt.x + 12) + dOff), ((pt.y + 23) + dOff));
    DrawScanner(hdc, &(rc));

L_709a:
    if ((sel_scan.grobj == grobjNone))
        goto L_7115;
    else
        goto L_70a3;

L_70a3:
    if ((sel_scan.pt.x != sel.pt.x))
        goto L_70ba;
    else
        goto L_70b2;

L_70b2:
    if ((sel_scan.pt.y == sel.pt.y))
        goto L_7115;
    else
        goto L_70ba;

L_70ba:
    pt.x = sel_scan.pt.x;
    pt.y = sel_scan.pt.y;
    LogicalToScan(&(pt));
    SetRect(&(rc), ((pt.x - 6) - dOff), ((pt.y - 6) - dOff), ((pt.x + 7) + dOff), ((pt.y + 15) + dOff));
    DrawScanner(hdc, &(rc));

L_7115:
    if ((fVis != 0))
        goto L_7140;
    else
        goto L_711e;

L_711e:
    sel.id = sel_id;
    sel.grobj = sel_grobj;
    sel.grobjFull = sel_grobjFull;
    sel.scan = sel_scan;

L_7140:
    if ((fhdc == 0))
        goto L_7155;
    else
        goto L_7149;

L_7149:
    ReleaseDC(hwndScanner, hdc);

L_7155:
    return;
}

int16_t FEnsurePointOnScreen(POINT pt, int16_t fScroll) {
    int16_t cy;
    int16_t fFix;
    int16_t cx;
    POINT   ptCtr;
    RECT    rc;

L_715c:
    fFix = 0;
    GetClientRect(hwndScanner, &(rc));
    rc.bottom = (rc.bottom - dySBar);
    cx = ScanToPt(rc.right);
    cy = ScanToPt(rc.bottom);
    rc.left = (xScanTop + 10);
    rc.right = ((xScanTop + cx) - 20);
    rc.bottom = ((dGalInv - yScanTop) - 10);
    rc.top = ((((dGalInv - yScanTop) - 10) - cy) + 20);
    if ((PtInRect(&(rc), pt) == 0))
        goto L_71e7;
    else
        goto L_71e1;

L_71e1:
    return 0x1;

L_71e7:
    ptCtr.x = ((cx >> 0x1) + xScanTop);
    ptCtr.y = ((dGalInv - yScanTop) - (cy >> 0x1));
    if ((pt.x >= rc.left))
        goto L_721b;
    else
        goto L_720f;

L_720f:
    ptCtr.x = (ptCtr.x - (rc.left - pt.x));
    goto L_722f;

L_721b:
    if ((pt.x <= rc.right))
        goto L_722f;
    else
        goto L_7226;

L_7226:
    ptCtr.x = (ptCtr.x + (pt.x - rc.right));

L_722f:
    if ((pt.y >= rc.top))
        goto L_7246;
    else
        goto L_723a;

L_723a:
    ptCtr.y = (ptCtr.y - (rc.top - pt.y));
    goto L_725a;

L_7246:
    if ((pt.y <= rc.bottom))
        goto L_725a;
    else
        goto L_7251;

L_7251:
    ptCtr.y = (ptCtr.y + (pt.y - rc.bottom));

L_725a:
    CtrPointScan(ptCtr, fScroll);
    return 0x0;
}

void CtrPointScan(POINT pt, int16_t fScroll) {
    int16_t  dxCur;
    int16_t  cy;
    int16_t  y;
    int16_t  cx;
    int16_t  dyCur;
    int16_t  x;
    RECT     rc;
    uint16_t t_merge_72f1_0001;
    uint16_t t_merge_731c_0001;
    int16_t  t_merge_733f_0001;
    int16_t  t_merge_735c_0001;
    int16_t  t_merge_7378_0001;
    int16_t  t_merge_738e_0001;

L_7278:
    x = pt.x;
    y = pt.y;
    if ((hwndScanner == 0x0))
        goto L_7447;
    else
        goto L_7294;

L_7294:

L_729a:
    GetClientRect(hwndScanner, &(rc));
    rc.bottom = (rc.bottom - dySBar);
    cx = ScanToPt(rc.right);
    cy = ScanToPt(rc.bottom);
    if (((x - (cx >> 0x1)) <= 0x3e8))
        goto L_72ee;
    else
        goto L_72df;

L_72df:
    t_merge_72f1_0001 = (x - (cx >> 0x1));
    goto L_72f1;

L_72ee:
    t_merge_72f1_0001 = 0x3e8;

L_72f1:
    x = t_merge_72f1_0001;
    if ((((cy >> 0x1) + y) >= (dGalInv - 1000)))
        goto L_7316;
    else
        goto L_730b;

L_730b:
    t_merge_731c_0001 = ((cy >> 0x1) + y);
    goto L_731c;

L_7316:
    t_merge_731c_0001 = (dGalInv - 1000);

L_731c:
    y = t_merge_731c_0001;
    if ((x >= ((dGalInv - 1000) - cx)))
        goto L_7336;
    else
        goto L_7330;

L_7330:
    t_merge_733f_0001 = x;
    goto L_733f;

L_7336:
    t_merge_733f_0001 = ((dGalInv - 1000) - cx);

L_733f:
    x = t_merge_733f_0001;
    if ((y <= (cy + 1000)))
        goto L_7356;
    else
        goto L_7350;

L_7350:
    t_merge_735c_0001 = y;
    goto L_735c;

L_7356:
    t_merge_735c_0001 = (cy + 1000);

L_735c:
    y = (dGalInv - t_merge_735c_0001);
    if ((x <= 1000))
        goto L_7375;
    else
        goto L_736f;

L_736f:
    t_merge_7378_0001 = x;
    goto L_7378;

L_7375:
    t_merge_7378_0001 = 1000;

L_7378:
    x = t_merge_7378_0001;
    if ((y <= 1000))
        goto L_738b;
    else
        goto L_7385;

L_7385:
    t_merge_738e_0001 = y;
    goto L_738e;

L_738b:
    t_merge_738e_0001 = 1000;

L_738e:
    y = t_merge_738e_0001;
    dxCur = xScanTop;
    dyCur = yScanTop;
    x = ((x + 0x2) & 0xfffc);
    y = ((y + 0x2) & 0xfffc);
    if ((dxCur != x))
        goto L_73ce;
    else
        goto L_73c0;

L_73c0:
    if ((dyCur == y))
        goto L_7447;
    else
        goto L_73c8;

L_73c8:

L_73ce:
    xScanTop = x;
    SetScrollPos(hwndScanner, 0x0, x, 0x1);
    yScanTop = y;
    SetScrollPos(hwndScanner, 0x1, y, 0x1);
    if ((fScroll == 0))
        goto L_7432;
    else
        goto L_7407;

L_7407:
    ScrollScanner(PtToScan((dxCur - x)), PtToScan((dyCur - y)));
    goto L_7447;

L_7432:
    InvalidateRect(hwndScanner, 0x0, 0x0);

L_7447:
    return;
}

void LogicalToScan(POINT *ppt) {
L_744e:
    ppt->x = PtToScan((ppt->x - xScanTop));
    ppt->y = PtToScan(((dGalInv - ppt->y) - yScanTop));
    return;
}

void ScanToLogical(POINT *ppt) {
L_7490:
    ppt->x = (ScanToPt(ppt->x) + xScanTop);
    ppt->y = (dGalInv - (ScanToPt(ppt->y) + yScanTop));
    if ((ppt->x <= (dGal + 1000)))
        goto L_74e8;
    else
        goto L_74dd;

L_74dd:
    ppt->x = (dGal + 1000);

L_74e8:
    if ((ppt->y >= 1000))
        goto L_74fd;
    else
        goto L_74f5;

L_74f5:
    ppt->y = 1000;

L_74fd:
    return;
}

int16_t FAddWayPoint(POINT ptIn, SCAN *pscan) {
    HDC     hdc;
    int16_t id;
    int16_t dy;
    ORDER  *lpord;
    int16_t lDist;
    POINT   rgpt[3];
    int16_t dx;
    int16_t cpt;
    int16_t ipt;
    RECT    rc;

L_7504:
    if ((sel.fl.cord != 87))
        goto L_7559;
    else
        goto L_7517;

L_7517:
    MessageBeep(0x40);
    _wsprintf(szWork, PszGetCompressedString(idsCantHaveDWaypoints), 0x56);
    AlertSz(szWork, MB_ICONHAND);
    return 0x0;

L_7559:
    if (((grbitScan & 0x80) == 0x0))
        goto L_75c6;
    else
        goto L_7567;

L_7567:
    rgpt[0].x = ptIn.x;
    rgpt[0].y = ptIn.y;
    rgpt[1].x = pscan->pt.x;
    rgpt[1].y = pscan->pt.y;
    if ((sel.iwpAct >= (sel.fl.cord - 1)))
        goto L_75c1;
    else
        goto L_7590;

L_7590:
    cpt = 3;
    rgpt[2].x = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.x;
    rgpt[2].y = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.y;
    goto L_75c6;

L_75c1:
    cpt = 2;

L_75c6:
    dx = (ptIn.x - pscan->pt.x);
    dy = (ptIn.y - pscan->pt.y);
    lDist = ScanToPt(0x14);
    if (((HIWORD((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx)))) + HIWORD((uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))))) < SIGNHIWORD(LOWORD((lDist * lDist)))))
        goto L_7680;
    else
        goto L_7638;

L_7638:
    /* untranslated: branch ss:[bp-0x24] > signhiword(loword((lDist * lDist))) ? L_7644 : L_763d */

L_763d:
    /* untranslated: branch ss:[bp-0x26] <= loword((lDist * lDist)) ? L_7680 : L_7644 */

L_7644:
    pscan->grobjFull = grobjOther;
    pscan->grobj = grobjOther;
    pscan->idpl = -1;
    pscan->ifl = -1;
    pscan->iwp = (sel.iwpAct + 1);
    pscan->pt.x = ptIn.x;
    pscan->pt.y = ptIn.y;

L_7680:
    lpord = &(sel.fl.lpplord->rgord[sel.iwpAct]);
    if ((pscan->pt.x != lpord->pt.x))
        goto L_76d0;
    else
        goto L_76c1;

L_76c1:
    if ((pscan->pt.y != lpord->pt.y))
        goto L_76d0;
    else
        goto L_76ca;

L_76ca:
    return 0x0;

L_76d0:
    if ((sel.iwpAct >= (sel.fl.cord - 1)))
        goto L_7708;
    else
        goto L_76df;

L_76df:
    if ((pscan->pt.x != lpord[1].pt.x))
        goto L_7708;
    else
        goto L_76f9;

L_76f9:
    if ((pscan->pt.y != *(lpord + 0x14)))
        goto L_7708;
    else
        goto L_7702;

L_7702:
    return 0x0;

L_7708:
    hdc = GetDC(hwndScanner);
    DrawShipScanPath(hdc, 0x0);
    if ((sel.fl.lpplord->iordMax != sel.fl.cord))
        goto L_7784;
    else
        goto L_7737;

L_7737:
    sel.fl.lpplord = LpplReAlloc(sel.fl.lpplord, (sel.fl.cord + 3));
    lpord = &(sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)]);
    goto L_7788;

L_7784:
    lpord = (lpord + 0x1);

L_7788:
    if ((sel.iwpAct == (sel.fl.cord - 1)))
        goto L_77c2;
    else
        goto L_7797;

L_7797:
    fmemmove(&(lpord[0x1]), &(lpord), LOWORD((((sel.fl.cord - sel.iwpAct) + 0xffff) * 0x12)));

L_77c2:
    *(lpord) = *(lpord + 0xffee);
    lpord->pt.x = pscan->pt.x;
    lpord->pt.y = pscan->pt.y;
    goto L_785a;

L_77ff:
    id = pscan->idpl;
    goto L_787d;

L_780b:
    id = pscan->iwp;
    goto L_787d;

L_7817:
    id = rglpfl[pscan->ifl]->id;
    goto L_787d;

L_7839:
    id = lpThings[pscan->ith].idFull;
    goto L_787d;

L_785a:
    if ((pscan->grobj == grobjPlanet))
        goto L_77ff;
    else
        goto L_7862;

L_7862:
    if ((pscan->grobj == grobjFleet))
        goto L_7817;
    else
        goto L_786a;

L_786a:
    if ((pscan->grobj == grobjOther))
        goto L_780b;
    else
        goto L_7872;

L_7872:
    if ((pscan->grobj == grobjThing))
        goto L_7839;
    else
        goto L_787d;

L_787d:
    lpord->id = id;
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xf0ff) | ((pscan->grobj & 0xf) << 0x8));
    sel.fl.cord = (sel.fl.cord + 1);
    sel.fl.lpplord->iordMac = (sel.fl.lpplord->iordMac + 0x1);
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((IWarpBestForWaypoint(&(sel.fl), lpord) & 0xf) << 0x4));
    pscan->grobj = grobjOther;
    pscan->grobjFull = (pscan->grobjFull | 0x4);
    pscan->iwp = (sel.iwpAct + 1);
    RedrawScanSel(0x0, 0x0);
    FLookupFleet(0xffff, &(sel.fl));
    if (((*(lpord - 0xc) & 0xf) != 0x6))
        goto L_7990;
    else
        goto L_7942;

L_7942:
    if ((*(lpord - 0xa) != 0x5))
        goto L_7990;
    else
        goto L_794f;

L_794f:
    fmemset(&(*(lpord - 0xa)), 0x0, 0xa);
    *(lpord - 0xc) = ((*(lpord - 0xc) & 0xfff0) | 0x0);
    FLookupFleet(0xffff, &(sel.fl));

L_7990:
    ChangeScanSel(pscan, 0x1);
    ReleaseDC(hwndScanner, hdc);
    if (((grbitScan & 0x80) == 0x0))
        goto L_7a0c;
    else
        goto L_79b9;

L_79b9:
    ipt = 0;
    goto L_79da;

L_79c1:
    LogicalToScan(rgpt[ipt]);
    ipt = (ipt + 1);

L_79da:
    if ((ipt < cpt))
        goto L_79c1;
    else
        goto L_79e5;

L_79e5:
    BoundPoints(&(rc), rgpt, cpt);
    InvalidateRect(hwndScanner, &(rc), 0x0);

L_7a0c:
    return 0x1;
}

int16_t IWarpBestForWaypoint(FLEET *lpfl, ORDER *lpord) {
    int32_t lFuel;
    int16_t iWarp;
    int16_t cTravel;
    int16_t iwp;
    int16_t lDist;
    int16_t cSpeed;
    int16_t fGoFlatOutAi;
    int16_t fGoFlatOut;
    int16_t iWarpAi;
    int16_t iWarpSav;
    int16_t j;
    int16_t i;
    PLANET *lppl;
    int16_t iWarpOld;
    SCAN    scan;
    int32_t t_call_7d4b;
    int32_t t_call_7d8d;

L_7a18:
    iWarpSav = lpord->iWarp;
    iWarp = IFindIdealWarp(0x0, 0x0);
    if ((fAi == 0))
        goto L_7a6e;
    else
        goto L_7a57;

L_7a57:
    iWarpAi = IFindIdealWarp(0x0, 0x1);

L_7a6e:
    iwp = (lpfl->cord - 1);
    goto L_7ab9;

L_7a7e:
    if ((LOWORD(lpord) != ((LOWORD(lpfl->lpplord) + 0x4) + LOWORD((18 * iwp)))))
        goto L_7ab5;
    else
        goto L_7aaa;

L_7aaa:
    /* untranslated: branch HIWORD(lpord) == ss:[bp-0x1a] ? L_7ac2 : L_7aaf */

L_7aaf:

L_7ab5:
    iwp = (iwp - 1);

L_7ab9:
    if ((iwp >= 0))
        goto L_7a7e;
    else
        goto L_7ac2;

L_7ac2:
    if ((iwp > 0))
        goto L_7ad1;
    else
        goto L_7acb;

L_7acb:
    return iWarp;

L_7ad1:
    if ((lpord->grTask == grTaskColonize))
        goto L_7af5;
    else
        goto L_7ae3;

L_7ae3:
    if ((lpord->grTask != grTaskScrap))
        goto L_7afd;
    else
        goto L_7af5;

L_7af5:
    fGoFlatOut = 1;
    goto L_7c20;

L_7afd:
    fGoFlatOut = 0;
    i = 0;
    goto L_7c17;

L_7b0a:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_7c13;
    else
        goto L_7b27;

L_7b27:
    j = 0;
    goto L_7b33;

L_7b2f:
    j = (j + 1);

L_7b33:
    if ((j >= rglpshdef[lpfl->iPlayer][i].hul.chs))
        goto L_7c13;
    else
        goto L_7b5f;

L_7b5f:
    if ((rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst != hstSpecialM))
        goto L_7b2f;
    else
        goto L_7b94;

L_7b94:
    if ((rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem == 0x0))
        goto L_7c08;
    else
        goto L_7bce;

L_7bce:
    if ((rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem != 0x1))
        goto L_7b2f;
    else
        goto L_7c08;

L_7c08:
    fGoFlatOut = 1;
    goto L_7c13;

L_7c13:
    i = (i + 1);

L_7c17:
    if ((i < 16))
        goto L_7b0a;
    else
        goto L_7c20;

L_7c20:
    if ((fGoFlatOut != 0))
        goto L_7c41;
    else
        goto L_7c29;

L_7c29:
    if ((fAi == 0))
        goto L_7c41;
    else
        goto L_7c33;

L_7c33:
    fGoFlatOutAi = 1;
    fGoFlatOut = 1;
    goto L_7c46;

L_7c41:
    fGoFlatOutAi = 0;

L_7c46:
    if ((iWarp >= 9))
        goto L_7f69;
    else
        goto L_7c4f;

L_7c4f:
    iWarpOld = iWarp;
    if ((FFindNearestObject(lpord->pt, 0x81, &(scan)) == 0))
        goto L_7c8b;
    else
        goto L_7c77;

L_7c77:
    lppl = LpplFromId(scan.idpl);
    goto L_7c95;

L_7c8b:
    lppl = 0x0;

L_7c95:
    if ((fGoFlatOut != 0))
        goto L_7dc7;
    else
        goto L_7c9e;

L_7c9e:
    if ((LOWORD(lppl) != 0x0))
        goto L_7cb0;
    else
        goto L_7ca7;

L_7ca7:
    if ((HIWORD(lppl) == 0x0))
        goto L_7ccc;
    else
        goto L_7cb0;

L_7cb0:
    if ((lppl->iPlayer == -1))
        goto L_7dc7;
    else
        goto L_7cbd;

L_7cbd:
    if ((lppl->iPlayer == idPlayer))
        goto L_7dc7;
    else
        goto L_7ccc;

L_7ccc:
    if ((iwp <= 1))
        goto L_7d1e;
    else
        goto L_7cd5;

L_7cd5:
    if ((((*(lpord - 0xc) >> 0x4) & 0xf) <= iWarp))
        goto L_7d1e;
    else
        goto L_7cef;

L_7cef:
    if ((((*(lpord - 0xc) >> 0x4) & 0xf) > 0xa))
        goto L_7d1e;
    else
        goto L_7d09;

L_7d09:
    iWarp = ((*(lpord - 0xc) >> 0x4) & 0xf);

L_7d1e:
    LFuelUseToWaypoint(lpfl, iwp, 0x1);
    /* untranslated: ss:[bp-0x32] = loword(callresult(int32_t)) */
    /* untranslated: ss:[bp-0x30] = hiword(callresult(int32_t)) */
    t_call_7d4b = LGetFleetStat(lpfl, 0x1);
    /* untranslated: branch ss:[bp-0x30] > hiword((int32_t)(t_call_7d4b / 10)) ? L_7f83 : L_7d67 */

L_7d67:
    /* untranslated: branch ss:[bp-0x30] < hiword((int32_t)(t_call_7d4b / 10)) ? L_7d73 : L_7d6c */

L_7d6c:
    /* untranslated: branch ss:[bp-0x32] >= loword((int32_t)(t_call_7d4b / 10)) ? L_7f83 : L_7d73 */

L_7d73:
    t_call_7d8d = LGetFleetStat(lpfl, 0x1);
    if ((HIWORD(lpfl->rgwtMin[0x4]) < HIWORD((int32_t)(((uint32_t)((t_call_7d8d * 7)) / 0xa)))))
        goto LOptimizeSpeed;
    else
        goto L_7daf;

L_7daf:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > HIWORD((int32_t)(((uint32_t)((t_call_7d8d * 7)) / 0xa)))))
        goto L_7dbd;
    else
        goto L_7db4;

L_7db4:
    if ((LOWORD(lpfl->rgwtMin[0x4]) < LOWORD((int32_t)(((uint32_t)((t_call_7d8d * 7)) / 0xa)))))
        goto LOptimizeSpeed;
    else
        goto L_7dbd;

L_7dbd:
    iWarp = (iWarp + 1);
    goto L_7f3d;

L_7dc7:
    if ((fGoFlatOutAi == 0))
        goto L_7e22;
    else
        goto L_7dd0;

L_7dd0:
    if ((LOWORD(lppl) != 0x0))
        goto L_7de2;
    else
        goto L_7dd9;

L_7dd9:
    if ((HIWORD(lppl) == 0x0))
        goto L_7e22;
    else
        goto L_7de2;

L_7de2:
    if ((lppl->iPlayer != idPlayer))
        goto L_7e22;
    else
        goto L_7df1;

L_7df1:
    if ((rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef == ihuldefOrbitalFort))
        goto L_7e22;
    else
        goto L_7e1d;

L_7e1d:
    fGoFlatOutAi = 0;

L_7e22:
    iWarp = 9;

L_7e2a:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((iWarp & 0xf) << 0x4));
    lFuel = LFuelUseToWaypoint(lpfl, iwp, 0x1);
    if ((HIWORD(lFuel) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_7e89;
    else
        goto L_7e79;

L_7e79:
    if ((HIWORD(lFuel) > HIWORD(lpfl->rgwtMin[0x4])))
        goto LDecWarp;
    else
        goto L_7e7e;

L_7e7e:
    if ((LOWORD(lFuel) > LOWORD(lpfl->rgwtMin[0x4])))
        goto LDecWarp;
    else
        goto L_7e83;

L_7e83:

L_7e89:
    if ((LOWORD(lppl) != 0x0))
        goto L_7e9b;
    else
        goto L_7e92;

L_7e92:
    if ((HIWORD(lppl) == 0x0))
        goto L_7f00;
    else
        goto L_7e9b;

L_7e9b:
    if ((lppl->fStarbase == 0x0))
        goto L_7f00;
    else
        goto L_7eb2;

L_7eb2:
    if ((lppl->iPlayer != idPlayer))
        goto L_7f00;
    else
        goto L_7ec1;

L_7ec1:
    if ((LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0x0))
        goto L_7f48;
    else
        goto L_7efa;

L_7efa:

L_7f00:
    if ((HIWORD(lFuel) < HIWORD((int32_t)((lpfl->rgwtMin[4] / 2)))))
        goto L_7f48;
    else
        goto L_7f20;

L_7f20:
    if ((HIWORD(lFuel) > HIWORD((int32_t)((lpfl->rgwtMin[4] / 2)))))
        goto L_7f2d;
    else
        goto L_7f25;

L_7f25:
    if ((LOWORD(lFuel) <= LOWORD((int32_t)((lpfl->rgwtMin[4] / 2)))))
        goto L_7f48;
    else
        goto L_7f2d;

L_7f2d:
    if ((fGoFlatOut != 0))
        goto L_7f48;
    else
        goto L_7f33;

L_7f33:

LDecWarp:
    iWarp = (iWarp - 1);

L_7f3d:
    if ((iWarp > iWarpOld))
        goto L_7e2a;
    else
        goto L_7f48;

L_7f48:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((iWarpSav & 0xf) << 0x4));

L_7f69:
    if ((fGoFlatOutAi == 0))
        goto LOptimizeSpeed;
    else
        goto L_7f72;

L_7f72:
    if ((iWarp <= iWarpAi))
        goto LOptimizeSpeed;
    else
        goto L_7f7d;

L_7f7d:
    iWarp = iWarpAi;

LOptimizeSpeed:
    if ((iWarp <= 1))
        goto L_8025;
    else
        goto L_7f8c;

L_7f8c:
    if ((lpord->grobj == grobjFleet))
        goto L_8025;
    else
        goto L_7fa3;

L_7fa3:
    DGetDistance(lpord->pt.x, lpord->pt.y, *(lpord - 0x12), *(lpord - 0x10));
    lDist = LOWORD(__ftol());
    cSpeed = LOWORD((iWarp * iWarp));
    cTravel = ((uint32_t)(((LOWORD((iWarp * iWarp)) + lDist) + 0xffff)) / cSpeed);

L_7feb:
    iWarp = (iWarp - 1);
    if ((iWarp <= 1))
        goto L_801e;
    else
        goto L_7ffa;

L_7ffa:
    cSpeed = LOWORD((iWarp * iWarp));
    if ((cTravel != ((uint32_t)(((lDist + cSpeed) + 0xffff)) / cSpeed)))
        goto L_801e;
    else
        goto L_8015;

L_8015:

L_801e:
    iWarp = (iWarp + 1);
    goto L_802a;

L_8025:
    cTravel = 2;

L_802a:
    /* untranslated: branch FCanFleetUseStargates(lpfl, words(*(lpord-0x10), *(lpord-0x12)), lpord->pt) != 1 ? L_805a : L_8055 */

L_8055:
    iWarp = 11;

L_805a:
    if ((iWarp <= 11))
        goto L_8068;
    else
        goto L_8063;

L_8063:
    iWarp = 9;

L_8068:
    return iWarp;
}

int16_t FNearAWayPoint(POINT pt, int16_t fLogical) {
    ORDER  *lpord;
    int16_t i;
    SCAN    scan;

L_8074:
    if ((sel.grobj == grobjFleet))
        goto L_808d;
    else
        goto L_8087;

L_8087:
    return 0x0;

L_808d:
    if ((fLogical != 0))
        goto L_80a2;
    else
        goto L_8096;

L_8096:
    ScanToLogical(&(pt));

L_80a2:
    if ((FFindNearestObject(pt, 0x4f, &(scan)) != 0))
        goto L_80c6;
    else
        goto L_80c0;

L_80c0:
    return 0x0;

L_80c6:
    if (((scan.grobjFull & 0x4) == 0x0))
        goto L_8169;
    else
        goto L_80d4;

L_80d4:
    if ((scan.pt.x != sel.pt.x))
        goto L_8163;
    else
        goto L_80e3;

L_80e3:
    if ((scan.pt.y != sel.pt.y))
        goto L_8163;
    else
        goto L_80eb;

L_80eb:
    lpord = &(sel.fl.lpplord->rgord[0x1]);
    i = 1;
    goto L_8119;

L_8106:
    i = (i + 1);
    lpord = (lpord + 0x1);

L_8119:
    if ((i >= sel.fl.cord))
        goto L_814c;
    else
        goto L_8124;

L_8124:
    /* untranslated: branch lpord->pt.x != ss:[scan] ? L_8106 : L_813d */

L_813d:
    /* untranslated: branch lpord->pt.y == ss:[scan+0x2] ? L_814c : L_8143 */

L_8143:

L_814c:
    if ((i == sel.fl.cord))
        goto L_815d;
    else
        goto L_8157;

L_8157:
    return 0x1;

L_815d:
    return 0x0;

L_8163:
    return 0x1;

L_8169:
    return 0x0;
}

int16_t FHandleWayPointDrag(POINT pt) {
    int16_t  fChg;
    HDC      hdc;
    HPEN     hpenSav;
    SBAR     sbar;
    int16_t  fMarker;
    char     szDeepSpace[40];
    int16_t  fDup;
    int16_t  grTypeIn;
    HCURSOR  hcurSav;
    ORDER   *lpord;
    int16_t  i;
    POINT    ptLogical;
    POINT    ptNext;
    int16_t  fDel;
    POINT    rgpt[4];
    POINT    ptNew;
    int16_t  cpt;
    POINT    ptPrev;
    SCAN     scan;
    int16_t  fFirst;
    RECT     rc;
    int16_t  t_merge_835d_0001;
    uint16_t t_merge_8383_0001;
    int16_t  t_merge_839c_0001;
    uint16_t t_merge_83c2_0001;
    uint16_t t_merge_843b_0001;
    uint16_t t_merge_8587_0001;
    uint16_t t_merge_85d2_0001;
    uint16_t t_merge_8631_0001;
    uint16_t t_merge_8682_0001;
    uint16_t t_merge_87c0_0001;

L_8176:
    fFirst = 1;
    fMarker = 0;
    LogicalToScan(&(pt));
    if ((sel.iwpAct != 0))
        goto L_820b;
    else
        goto L_819f;

L_819f:
    lpord = &(sel.fl.lpplord->rgord[0x1]);
    i = 1;
    goto L_81cd;

L_81ba:
    i = (i + 1);
    lpord = (lpord + 0x1);

L_81cd:
    if ((i >= sel.fl.cord))
        goto L_8200;
    else
        goto L_81d8;

L_81d8:
    if ((sel.fl.pt.x != lpord->pt.x))
        goto L_81ba;
    else
        goto L_81f1;

L_81f1:
    if ((sel.fl.pt.y == lpord->pt.y))
        goto L_8200;
    else
        goto L_81f7;

L_81f7:

L_8200:
    SetScanWp(i);

L_820b:
    rgpt[2].x = sel.fl.lpplord->rgord[(sel.iwpAct - 0x1)].pt.x;
    rgpt[2].y = sel.fl.lpplord->rgord[(sel.iwpAct - 0x1)].pt.y;
    ptPrev.x = rgpt[0x2].x;
    ptPrev.y = rgpt[0x2].y;
    if ((sel.iwpAct != (sel.fl.cord - 1)))
        goto L_8257;
    else
        goto L_824f;

L_824f:
    cpt = 3;
    goto L_8291;

L_8257:
    cpt = 4;
    rgpt[3].x = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.x;
    rgpt[3].y = sel.fl.lpplord->rgord[(sel.iwpAct + 0x1)].pt.y;
    ptNext.x = rgpt[0x3].x;
    ptNext.y = rgpt[0x3].y;

L_8291:
    rgpt[0].x = sel.fl.lpplord->rgord[sel.iwpAct].pt.x;
    rgpt[0].y = sel.fl.lpplord->rgord[sel.iwpAct].pt.y;
    rgpt[1].x = rgpt[0x0].x;
    rgpt[1].y = rgpt[0x0].y;
    i = 0;
    goto L_82e3;

L_82ca:
    LogicalToScan(rgpt[i]);
    i = (i + 1);

L_82e3:
    if ((i < cpt))
        goto L_82ca;
    else
        goto L_82ee;

L_82ee:
    GetClientRect(hwndScanner, &(rc));
    rc.bottom = (rc.bottom - dySBar);
    hdc = GetDC(hwndScanner);
    hcurSav = SetCursor(hcurCloseGrab);
    SetCapture(hwndScanner);
    ptNew.x = pt.x;
    ptNew.y = pt.y;

L_8333:
    if ((FGetMouseMove(&(ptNew)) == 0))
        goto L_8568;
    else
        goto L_8347;

L_8347:
    if ((rc.right >= ptNew.x))
        goto L_835a;
    else
        goto L_8353;

L_8353:
    t_merge_835d_0001 = rc.right;
    goto L_835d;

L_835a:
    t_merge_835d_0001 = ptNew.x;

L_835d:
    if ((0 <= t_merge_835d_0001))
        goto L_836d;
    else
        goto L_8367;

L_8367:
    t_merge_8383_0001 = 0x0;
    goto L_8383;

L_836d:
    if ((rc.right >= ptNew.x))
        goto L_8380;
    else
        goto L_8379;

L_8379:
    t_merge_8383_0001 = rc.right;
    goto L_8383;

L_8380:
    t_merge_8383_0001 = ptNew.x;

L_8383:
    ptNew.x = t_merge_8383_0001;
    if ((rc.bottom >= ptNew.y))
        goto L_8399;
    else
        goto L_8392;

L_8392:
    t_merge_839c_0001 = rc.bottom;
    goto L_839c;

L_8399:
    t_merge_839c_0001 = ptNew.y;

L_839c:
    if ((0 <= t_merge_839c_0001))
        goto L_83ac;
    else
        goto L_83a6;

L_83a6:
    t_merge_83c2_0001 = 0x0;
    goto L_83c2;

L_83ac:
    if ((rc.bottom >= ptNew.y))
        goto L_83bf;
    else
        goto L_83b8;

L_83b8:
    t_merge_83c2_0001 = rc.bottom;
    goto L_83c2;

L_83bf:
    t_merge_83c2_0001 = ptNew.y;

L_83c2:
    ptNew.y = t_merge_83c2_0001;
    if ((pt.x != ptNew.x))
        goto L_83db;
    else
        goto L_83d3;

L_83d3:
    if ((pt.y == ptNew.y))
        goto L_8333;
    else
        goto L_83db;

L_83db:
    if ((fFirst == 0))
        goto L_8401;
    else
        goto L_83e4;

L_83e4:
    if ((FNearAWayPoint(ptNew, 0x0) != 0))
        goto DoNext;
    else
        goto L_83fb;

L_83fb:

L_8401:
    fFirst = 0;
    ptLogical.x = ptNew.x;
    ptLogical.y = ptNew.y;
    ScanToLogical(&(ptLogical));
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_8438;
    else
        goto L_8432;

L_8432:
    t_merge_843b_0001 = 0x8f;
    goto L_843b;

L_8438:
    t_merge_843b_0001 = 0x4f;

L_843b:
    grTypeIn = t_merge_843b_0001;
    FFindNearestObject(ptLogical, grTypeIn, &(scan));
    DrawScanXorLines(hdc, rgpt, cpt);
    if ((scan.grobj != grobjNone))
        goto L_8498;
    else
        goto L_846e;

L_846e:
    rgpt[0].x = ptLogical.x;
    rgpt[0].y = ptLogical.y;
    sbar.id = -1;
    CchGetString(idsDeepSpace, szDeepSpace);
    sbar.psz = szDeepSpace;
    goto L_850d;

L_8498:
    rgpt[0].x = scan.pt.x;
    rgpt[0].y = scan.pt.y;
    if ((scan.grobj != grobjPlanet))
        goto L_84b6;
    else
        goto L_84ad;

L_84ad:
    sbar.id = scan.idpl;
    goto L_8508;

L_84b6:
    if ((scan.grobj != grobjFleet))
        goto L_84de;
    else
        goto L_84bf;

L_84bf:
    sbar.id = rglpfl[scan.ifl]->id;
    goto L_8508;

L_84de:
    if ((scan.grobj != grobjThing))
        goto L_8502;
    else
        goto L_84e7;

L_84e7:
    sbar.id = lpThings[scan.ith].idFull;
    goto L_8508;

L_8502:
    sbar.id = scan.iwp;

L_8508:
    sbar.psz = 0x0;

L_850d:
    sbar.pt.x = rgpt[0x0].x;
    sbar.pt.y = rgpt[0x0].y;
    sbar.grbit = scan.grobj;
    LogicalToScan(rgpt);
    DrawScanXorLines(hdc, rgpt, cpt);
    sbar.pscan = 0x0;
    DrawScannerSBar(hdc, 0x0, &(sbar), 0x0);

DoNext:
    pt.x = ptNew.x;
    pt.y = ptNew.y;

L_8568:
    if ((rgpt[0].x != rgpt[1].x))
        goto L_857e;
    else
        goto L_8576;

L_8576:
    if ((rgpt[0].y == rgpt[1].y))
        goto L_8584;
    else
        goto L_857e;

L_857e:
    t_merge_8587_0001 = 0x1;
    goto L_8587;

L_8584:
    t_merge_8587_0001 = 0x0;

L_8587:
    fChg = t_merge_8587_0001;
    if ((fChg == 0))
        goto L_8a37;
    else
        goto L_8593;

L_8593:
    if ((scan.grobj != grobjNone))
        goto L_85b3;
    else
        goto L_859c;

L_859c:
    scan.pt.x = ptLogical.x;
    scan.pt.y = ptLogical.y;
    scan.grobj = grobjOther;
    scan.iwp = sel.iwpAct;

L_85b3:
    if ((scan.pt.x != ptPrev.x))
        goto L_85cf;
    else
        goto L_85c1;

L_85c1:
    if ((scan.pt.y != ptPrev.y))
        goto L_85cf;
    else
        goto L_85c9;

L_85c9:
    t_merge_85d2_0001 = 0x1;
    goto L_85d2;

L_85cf:
    t_merge_85d2_0001 = 0x0;

L_85d2:
    fDup = t_merge_85d2_0001;
    if ((fDup != 0))
        goto L_8636;
    else
        goto L_85de;

L_85de:
    if ((sel.iwpAct >= (sel.fl.cord - 1)))
        goto L_8636;
    else
        goto L_85ed;

L_85ed:
    /* untranslated: branch ss:[scan] != sel.fl.lpplord->rgord[(sel.iwpAct + 1)].pt.x ? L_862e : L_861f */

L_861f:
    /* untranslated: branch ss:[scan+0x2] != sel.fl.lpplord->rgord[(sel.iwpAct + 1)].pt.y ? L_862e : L_8628 */

L_8628:
    t_merge_8631_0001 = 0x1;
    goto L_8631;

L_862e:
    t_merge_8631_0001 = 0x0;

L_8631:
    fDup = (t_merge_8631_0001 * 2);

L_8636:
    GetClientRect(hwndScanner, &(rc));
    if ((fDup == 0))
        goto L_87cc;
    else
        goto L_8650;

L_8650:
    if ((AlertSz(PszFormatIds(idsSureWantDeleteCurrentWaypoint, 0x0), 0x2024) != 6))
        goto L_867f;
    else
        goto L_8679;

L_8679:
    t_merge_8682_0001 = 0x1;
    goto L_8682;

L_867f:
    t_merge_8682_0001 = 0x0;

L_8682:
    fDel = t_merge_8682_0001;
    if (((grbitScan & 0x80) == 0x0))
        goto L_86fd;
    else
        goto L_8693;

L_8693:
    hpenSav = SelectObject(hdc, hpenStarbase);
    MoveTo(hdc, rgpt[2].x, rgpt[2].y);
    LineTo(hdc, rgpt[0].x, rgpt[0].y);
    if ((cpt <= 3))
        goto L_86f2;
    else
        goto L_86c7;

L_86c7:
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    LineTo(hdc, rgpt[3].x, rgpt[3].y);

L_86f2:
    SelectObject(hdc, hpenSav);

L_86fd:
    DrawScanXorLines(hdc, rgpt, cpt);
    rgpt[0].x = rgpt[0x1].x;
    rgpt[0].y = rgpt[0x1].y;
    if (((grbitScan & 0x80) == 0x0))
        goto L_8793;
    else
        goto L_8729;

L_8729:
    hpenSav = SelectObject(hdc, hpenStarbase);
    MoveTo(hdc, rgpt[2].x, rgpt[2].y);
    LineTo(hdc, rgpt[0].x, rgpt[0].y);
    if ((cpt <= 3))
        goto L_8788;
    else
        goto L_875d;

L_875d:
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    LineTo(hdc, rgpt[3].x, rgpt[3].y);

L_8788:
    SelectObject(hdc, hpenSav);

L_8793:
    DrawScanXorLines(hdc, rgpt, cpt);
    if ((fDel == 0))
        goto Done;
    else
        goto L_87ae;

L_87ae:
    if ((fDup != 1))
        goto L_87bd;
    else
        goto L_87b7;

L_87b7:
    t_merge_87c0_0001 = 0x1;
    goto L_87c0;

L_87bd:
    t_merge_87c0_0001 = 0x0;

L_87c0:
    DeleteCurWayPoint(t_merge_87c0_0001);

L_87cc:
    if (((grbitScan & 0x80) == 0x0))
        goto L_8844;
    else
        goto L_87da;

L_87da:
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    hpenSav = SelectObject(hdc, hpenStarbase);
    MoveTo(hdc, rgpt[2].x, rgpt[2].y);
    LineTo(hdc, rgpt[0].x, rgpt[0].y);
    if ((cpt <= 3))
        goto L_8839;
    else
        goto L_882b;

L_882b:
    LineTo(hdc, rgpt[3].x, rgpt[3].y);

L_8839:
    SelectObject(hdc, hpenSav);

L_8844:
    DrawScanXorLines(hdc, rgpt, cpt);
    rgpt[0].x = rgpt[0x1].x;
    rgpt[0].y = rgpt[0x1].y;
    if (((grbitScan & 0x80) == 0x0))
        goto L_88da;
    else
        goto L_8870;

L_8870:
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    hpenSav = SelectObject(hdc, hpenStarbase);
    MoveTo(hdc, rgpt[2].x, rgpt[2].y);
    LineTo(hdc, rgpt[0].x, rgpt[0].y);
    if ((cpt <= 3))
        goto L_88cf;
    else
        goto L_88c1;

L_88c1:
    LineTo(hdc, rgpt[3].x, rgpt[3].y);

L_88cf:
    SelectObject(hdc, hpenSav);

L_88da:
    DrawScanXorLines(hdc, rgpt, cpt);
    RedrawScanSel(0x0, 0x0);
    if ((scan.grobj != grobjPlanet))
        goto L_890e;
    else
        goto L_8905;

L_8905:
    i = scan.idpl;
    goto L_8960;

L_890e:
    if ((scan.grobj != grobjFleet))
        goto L_8936;
    else
        goto L_8917;

L_8917:
    i = rglpfl[scan.ifl]->id;
    goto L_8960;

L_8936:
    if ((scan.grobj != grobjThing))
        goto L_895a;
    else
        goto L_893f;

L_893f:
    i = lpThings[scan.ith].idFull;
    goto L_8960;

L_895a:
    i = scan.iwp;

L_8960:
    lpord = &(sel.fl.lpplord->rgord[sel.iwpAct]);
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xf0ff) | ((scan.grobj & 0xf) << 0x8));
    lpord->id = i;
    lpord->pt.x = scan.pt.x;
    lpord->pt.y = scan.pt.y;
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((IWarpBestForWaypoint(&(sel.fl), lpord) & 0xf) << 0x4));
    FLookupFleet(0xffff, &(sel.fl));
    scan.iwp = sel.iwpAct;
    scan.grobjFull = (scan.grobjFull | 0x4);
    sel.iwpAct = -2;
    ChangeScanSel(&(scan), 0x1);

L_8a37:
    DrawScannerSBar(hdc, 0x0, 0x0, 0x0);
    ReleaseCapture();
    SetCursor(hcurSav);
    InvalidateRect(hwndMine, 0x0, 0x1);
    SetMineralTitleBar(hwndMine);

Done:
    ReleaseDC(hwndScanner, hdc);
    if ((fChg == 0))
        goto L_8aea;
    else
        goto L_8a91;

L_8a91:
    if (((grbitScan & 0x80) == 0x0))
        goto L_8aea;
    else
        goto L_8a9f;

L_8a9f:
    rgpt[1].x = ptNew.x;
    rgpt[1].y = ptNew.y;
    BoundPoints(&(rc), rgpt, cpt);
    hdc = GetDC(hwndScanner);
    DrawScanner(hdc, &(rc));
    ReleaseDC(hwndScanner, hdc);

L_8aea:

L_8af0:
    return fChg;
}

void DrawScanXorLines(HDC hdc, POINT *rgpt, int16_t cpt) {
    HPEN    hpenSav;
    int16_t iRopSav;
    int16_t i;
    RECT    rc;

L_8af6:
    GetClientRect(hwndScanner, &(rc));
    ExcludeClipRect(hdc, 0x0, (rc.bottom - dySBar), rc.right, rc.bottom);
    if ((cpt != 4))
        goto L_8b61;
    else
        goto L_8b32;

L_8b32:
    if ((rgpt[2].x != rgpt[3].x))
        goto L_8b61;
    else
        goto L_8b46;

L_8b46:
    if ((*(rgpt + 0xa) != *(rgpt + 0xe)))
        goto L_8b61;
    else
        goto L_8b4e;

L_8b4e:
    cpt = (cpt - 1);
    hpenSav = GetStockObject(WHITE_PEN);
    goto L_8b67;

L_8b61:
    hpenSav = hpenShip;

L_8b67:
    i = 1;
    goto L_8bc5;

L_8b6f:
    ExcludeClipRect(hdc, (rgpt[i].x - 5), (rgpt[i].y - 5), (rgpt[i].x + 6), (rgpt[i].y + 6));
    i = (i + 1);

L_8bc5:
    if ((i < cpt))
        goto L_8b6f;
    else
        goto L_8bd0;

L_8bd0:
    hpenSav = SelectObject(hdc, hpenSav);
    iRopSav = SetROP2(hdc, 0x7);
    MoveTo(hdc, rgpt[2].x, *(rgpt + 0xa));
    LineTo(hdc, rgpt->x, rgpt->y);
    if ((cpt <= 3))
        goto L_8c31;
    else
        goto L_8c1d;

L_8c1d:
    LineTo(hdc, rgpt[3].x, *(rgpt + 0xe));

L_8c31:
    SetROP2(hdc, iRopSav);
    SelectObject(hdc, hpenSav);
    SelectClipRgn(hdc, hrgnHuge);
    return;
}

int16_t SetScanWp(int16_t iNew) {
    SCAN scan;

L_8c5a:
    if ((iNew != sel.iwpAct))
        goto L_8c74;
    else
        goto L_8c6e;

L_8c6e:
    return iNew;

L_8c74:
    FFindNearestObject(sel.fl.lpplord->rgord[iNew].pt, grobjOther, &(scan));
    scan.iwp = iNew;
    ChangeScanSel(&(scan), 0x1);
    return iNew;
}

void ChangeScanSel(SCAN *pscan, int16_t fValidScan) {
    int16_t  fMineFieldSel;
    RECT     rcMine;
    int16_t  fChgWp;
    int16_t  iRad;
    HDC      hdc;
    uint16_t t_merge_8d30_0001;
    uint16_t t_merge_8d6c_0001;
    uint16_t t_merge_8f71_0001;

L_8cc4:
    if ((fValidScan != 0))
        goto L_8cef;
    else
        goto L_8cd6;

L_8cd6:
    FFindNearestObject(pscan->pt, pscan->grobj, pscan);

L_8cef:
    if ((memcmp(pscan, &(sel.scan), 0x10) == 0))
        goto L_9095;
    else
        goto L_8d07;

L_8d07:

L_8d0d:
    if ((pscan->iwp == -1))
        goto L_8d2d;
    else
        goto L_8d19;

L_8d19:
    if ((pscan->iwp == sel.iwpAct))
        goto L_8d2d;
    else
        goto L_8d27;

L_8d27:
    t_merge_8d30_0001 = 0x1;
    goto L_8d30;

L_8d2d:
    t_merge_8d30_0001 = 0x0;

L_8d30:
    fChgWp = t_merge_8d30_0001;
    if ((sel.scan.grobj != grobjThing))
        goto L_8d69;
    else
        goto L_8d3d;

L_8d3d:
    if ((((lpThings[sel.scan.ith].idFull >> 0xd) & 0x7) != 0x0))
        goto L_8d69;
    else
        goto L_8d63;

L_8d63:
    t_merge_8d6c_0001 = 0x1;
    goto L_8d6c;

L_8d69:
    t_merge_8d6c_0001 = 0x0;

L_8d6c:
    fMineFieldSel = t_merge_8d6c_0001;
    if ((fMineFieldSel == 0))
        goto L_8e24;
    else
        goto L_8d78;

L_8d78:
    /* untranslated: call sqrt((double)part[6:4](lpThings[sel.scan.ith])) -> callresult(double) */
    iRad = LOWORD(__ftol());
    rcMine.left = lpThings[sel.scan.ith].pt.x;
    rcMine.top = lpThings[sel.scan.ith].pt.y;
    rcMine.right = (rcMine.left + iRad);
    rcMine.bottom = (rcMine.top - iRad);
    rcMine.left = (rcMine.left - iRad);
    rcMine.top = (rcMine.top + iRad);
    LogicalToScan(rcMine);
    LogicalToScan(rcMine.right);
    InflateRect(&(rcMine), 0x1, 0x1);

L_8e24:
    RedrawScanSel(0x0, 0xffff);
    sel.scan = *(pscan);
    if (((sel.scan.grobjFull & 0x1) == 0x0))
        goto L_8e61;
    else
        goto L_8e52;

L_8e52:
    if ((fValidScan == 2))
        goto L_8e61;
    else
        goto L_8e5b;

L_8e5b:
    sel.scan.grobj = grobjPlanet;

L_8e61:
    if ((fChgWp == 0))
        goto L_8ea2;
    else
        goto L_8e6a;

L_8e6a:
    sel.iwpAct = pscan->iwp;
    FillOrdersLB();
    SetOrdersLbSel(pscan->iwp);
    UpdateOrdersDDs(0x0);
    DrawPlanShip(0x0, 0x122);

L_8ea2:
    RedrawScanSel(0x0, 0x1);
    if ((fChgWp == 0))
        goto L_8ecf;
    else
        goto L_8ebb;

L_8ebb:
    FEnsurePointOnScreen(pscan->pt, 0x1);

L_8ecf:
    DrawScannerSBar(0x0, 0x0, 0x0, 0x0);
    InvalidateRect(hwndMine, 0x0, 0x1);
    SetMineralTitleBar(hwndMine);
    if ((fMineFieldSel == 0))
        goto L_8f38;
    else
        goto L_8f11;

L_8f11:
    hdc = GetDC(hwndScanner);
    DrawScanner(hdc, &(rcMine));
    ReleaseDC(hwndScanner, hdc);

L_8f38:
    if ((sel.scan.grobj != grobjThing))
        goto L_8f6e;
    else
        goto L_8f42;

L_8f42:
    if ((((lpThings[sel.scan.ith].idFull >> 0xd) & 0x7) != 0x0))
        goto L_8f6e;
    else
        goto L_8f68;

L_8f68:
    t_merge_8f71_0001 = 0x1;
    goto L_8f71;

L_8f6e:
    t_merge_8f71_0001 = 0x0;

L_8f71:
    fMineFieldSel = t_merge_8f71_0001;
    if ((fMineFieldSel == 0))
        goto L_9029;
    else
        goto L_8f7d;

L_8f7d:
    /* untranslated: call sqrt((double)part[6:4](lpThings[sel.scan.ith])) -> callresult(double) */
    iRad = LOWORD(__ftol());
    rcMine.left = lpThings[sel.scan.ith].pt.x;
    rcMine.top = lpThings[sel.scan.ith].pt.y;
    rcMine.right = (rcMine.left + iRad);
    rcMine.bottom = (rcMine.top - iRad);
    rcMine.left = (rcMine.left - iRad);
    rcMine.top = (rcMine.top + iRad);
    LogicalToScan(rcMine);
    LogicalToScan(rcMine.right);
    InflateRect(&(rcMine), 0x1, 0x1);

L_9029:
    if ((fMineFieldSel == 0))
        goto L_9059;
    else
        goto L_9032;

L_9032:
    hdc = GetDC(hwndScanner);
    DrawScanner(hdc, &(rcMine));
    ReleaseDC(hwndScanner, hdc);

L_9059:
    if ((sel.pl.id == -1))
        goto L_9073;
    else
        goto L_9063;

L_9063:
    DrawPlanShip(0x0, 0x4002);

L_9073:
    if ((gd.fTutorial == 0x0))
        goto L_9095;
    else
        goto L_9086;

L_9086:
    if ((idPlayer != 0))
        goto L_9095;
    else
        goto L_9090;

L_9090:
    AdvanceTutor();

L_9095:
    return;
}

int16_t FGetNextObjHere(SCAN *pscan, int16_t fOnlyOurs) {
    FLEET   *lpfl;
    int16_t  i;
    int16_t  fFound;
    uint16_t t_merge_90b8_0001;

L_909c:
    if ((sel.grobj == grobjFleet))
        goto L_90b5;
    else
        goto L_90af;

L_90af:
    t_merge_90b8_0001 = 0x1;
    goto L_90b8;

L_90b5:
    t_merge_90b8_0001 = 0x0;

L_90b8:
    fFound = t_merge_90b8_0001;
    i = 0;
    goto L_90c7;

L_90c3:
    i = (i + 1);

L_90c7:
    if ((i >= cFleet))
        goto L_9162;
    else
        goto L_90d2;

L_90d2:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_9102;
    else
        goto L_90fa;

L_90fa:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_9162;
    else
        goto L_9102;

L_9102:
    if ((fFound != 0))
        goto L_9121;
    else
        goto L_910b;

L_910b:
    if ((lpfl->id != sel.id))
        goto L_90c3;
    else
        goto L_9119;

L_9119:
    fFound = 1;

L_9121:
    if ((sel.pt.x != lpfl->pt.x))
        goto L_90c3;
    else
        goto L_913b;

L_913b:
    if ((sel.pt.y != lpfl->pt.y))
        goto L_90c3;
    else
        goto L_9144;

L_9144:
    if ((fOnlyOurs == 0))
        goto L_9162;
    else
        goto L_914d;

L_914d:
    if ((lpfl->iPlayer == idPlayer))
        goto L_9162;
    else
        goto L_9159;

L_9159:

L_9162:
    if ((fFound != 0))
        goto L_9171;
    else
        goto L_916b;

L_916b:
    return 0x0;

L_9171:
    if ((i >= cFleet))
        goto L_9190;
    else
        goto L_917c;

L_917c:
    pscan->ifl = i;
    pscan->grobj = grobjFleet;
    goto L_9279;

L_9190:
    if (((sel.grobjFull & 0x1) == 0x0))
        goto L_91be;
    else
        goto L_919e;

L_919e:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_91be;
    else
        goto L_91aa;

L_91aa:
    pscan->grobj = grobjPlanet;
    pscan->idpl = sel.pl.id;
    goto L_9279;

L_91be:
    i = 0;
    goto L_91ca;

L_91c6:
    i = (i + 1);

L_91ca:
    if ((i >= cFleet))
        goto L_9246;
    else
        goto L_91d5;

L_91d5:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_9205;
    else
        goto L_91fd;

L_91fd:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_9246;
    else
        goto L_9205;

L_9205:
    if ((pscan->pt.x != lpfl->pt.x))
        goto L_91c6;
    else
        goto L_921f;

L_921f:
    if ((pscan->pt.y != lpfl->pt.y))
        goto L_91c6;
    else
        goto L_9228;

L_9228:
    if ((fOnlyOurs == 0))
        goto L_9246;
    else
        goto L_9231;

L_9231:
    if ((lpfl->iPlayer == idPlayer))
        goto L_9246;
    else
        goto L_923d;

L_923d:

L_9246:
    if ((i >= cFleet))
        goto L_9273;
    else
        goto L_9251;

L_9251:
    if ((lpfl->id == sel.id))
        goto L_9273;
    else
        goto L_925f;

L_925f:
    pscan->ifl = i;
    pscan->grobj = grobjFleet;
    goto L_9279;

L_9273:
    return 0x0;

L_9279:
    return 0x1;
}

int16_t FindDlg(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    char     szName[40];
    RECT     rc;
    uint16_t t_merge_93ea_0001;

L_9286:
    goto L_9429;

L_9295:
    StickyDlgPos(hwnd, &(ptStickyFindDlg), 0x1);
    SendDlgItemMessage(hwnd, 0x10c, 0x415, 0x27, 0x0);
    return 0x1;

L_92ca:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_92f2:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_944c;
    else
        goto L_930e;

L_930e:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_9327:
    if ((wParam == 0x1))
        goto L_9339;
    else
        goto L_9330;

L_9330:
    if ((wParam != 0x2))
        goto L_93f9;
    else
        goto L_9339;

L_9339:
    if ((wParam != 0x1))
        goto L_93c2;
    else
        goto L_9342;

L_9342:
    GetDlgItemText(hwnd, IDC_EDIT1, szName, 0x28);
    if ((FSelectSz(szName) != 0))
        goto L_93c2;
    else
        goto L_936d;

L_936d:
    AlertSz(PszFormatIds(idsSorryCantFindPlanetFleetName, 0x0), MB_ICONHAND);
    SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
    SendDlgItemMessage(hwnd, 0x10c, 0x401, 0x0, 0xffff0000);
    return 0x0;

L_93c2:
    StickyDlgPos(hwnd, &(ptStickyFindDlg), 0x0);
    if ((wParam != 0x1))
        goto L_93e7;
    else
        goto L_93e1;

L_93e1:
    t_merge_93ea_0001 = 0x1;
    goto L_93ea;

L_93e7:
    t_merge_93ea_0001 = 0x0;

L_93ea:
    EndDialog(hwnd, t_merge_93ea_0001);
    return 0x1;

L_93f9:
    if ((wParam != 0x76))
        goto L_944c;
    else
        goto L_9402;

L_9402:
    WinHelp(hwnd, szHelpFile, 0x1, 0x43d);
    return 0x1;

L_9429:
    if ((msg == WM_ERASEBKGND))
        goto L_92ca;
    else
        goto L_9431;

L_9431:
    if ((msg == WM_CTLCOLOR))
        goto L_92f2;
    else
        goto L_9439;

L_9439:
    if ((msg == WM_INITDIALOG))
        goto L_9295;
    else
        goto L_9441;

L_9441:
    if ((msg == WM_COMMAND))
        goto L_9327;
    else
        goto L_944c;

L_944c:
    return 0x0;
}

int16_t FSelectSz(char *szName) {
    char   *pch;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t ipl;
    int16_t cch;
    char    szT[20];
    int16_t iplPartial;
    SCAN    scan;

L_945a:
    iplPartial = -1;
    scan.iwp = -1;
    ipl = 0;
    goto L_9479;

L_9475:
    ipl = (ipl + 1);

L_9479:
    if ((ipl >= game.cPlanMax))
        goto GoWithPartial;
    else
        goto L_9484;

L_9484:
    if ((strcmpi(PszGetCompressedPlanet(rgidPlan[ipl]), szName) == 0))
        goto GoWithPartial;
    else
        goto L_94a6;

L_94a6:

L_94ac:
    if ((iplPartial != -1))
        goto L_9475;
    else
        goto L_94b5;

L_94b5:
    if ((strnicmp(PszGetCompressedPlanet(rgidPlan[ipl]), szName, strlen(szName)) != 0))
        goto L_9475;
    else
        goto L_94e6;

L_94e6:
    iplPartial = ipl;

GoWithPartial:
    if ((ipl == game.cPlanMax))
        goto L_9563;
    else
        goto L_94fa;

L_94fa:
    FFindNearestObject(rgptPlan[ipl], grobjPlanet, &(scan));
    ChangeScanSel(&(scan), 0x1);
    FEnsurePointOnScreen(scan.pt, 0x1);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 0x76, 0x0);
    return 0x1;

L_9563:
    cch = CchGetString(idsFleet, szT);
    if ((strnicmp(szName, szT, cch) != 0))
        goto L_959e;
    else
        goto L_9590;

L_9590:
    pch = (szName + 0x6);
    goto L_95a4;

L_959e:
    pch = szName;

L_95a4:
    if (((uint16_t)(*(pch)) != 32))
        goto L_95b9;
    else
        goto L_95b2;

L_95b2:
    pch = (pch + 0x1);
    goto L_95a4;

L_95b9:
    if (((uint16_t)(*(pch)) != 35))
        goto L_95cb;
    else
        goto L_95c7;

L_95c7:
    pch = (pch + 0x1);

L_95cb:
    if (((uint16_t)(*(pch)) != 32))
        goto L_95e0;
    else
        goto L_95d9;

L_95d9:
    pch = (pch + 0x1);
    goto L_95cb;

L_95e0:
    if (((uint16_t)(*(pch)) < 49))
        goto LNotAFleetId;
    else
        goto L_95ee;

L_95ee:
    if (((uint16_t)(*(pch)) > 57))
        goto LNotAFleetId;
    else
        goto L_95fc;

L_95fc:
    ifl = ((uint16_t)(*(pch)) - 48);
    pch = (pch + 0x1);

L_960c:
    /* untranslated: branch (byte ds:[(95 + sext8to16(*pch))] & 0x4) == 0x0 ? L_9657 : L_9627 */

L_9627:
    ifl = ((LOWORD((0xa * ifl)) + (uint16_t)(*(pch))) - 48);
    pch = (pch + 0x1);
    if ((ifl > 512))
        goto LNotAFleetId;
    else
        goto L_964e;

L_964e:

L_9657:
    ifl = (ifl - 1);
    ifl = (ifl | (idPlayer << 0x9));
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto LFoundFleetId;
    else
        goto L_9680;

L_9680:
    if ((HIWORD(lpfl) == 0x0))
        goto LNotAFleetId;
    else
        goto LFoundFleetId;

LFoundFleetId:
    FFindNearestObject(lpfl->pt, grobjFleet, &(scan));
    scan.ifl = IflFromLpfl(lpfl);
    ChangeScanSel(&(scan), 0x2);
    FEnsurePointOnScreen(scan.pt, 0x1);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 0x76, 0x0);
    return 0x1;

LNotAFleetId:
    ifl = 0;
    goto L_970b;

L_9707:
    ifl = (ifl + 1);

L_970b:
    if ((ifl >= cFleet))
        goto L_976e;
    else
        goto L_9716;

L_9716:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_9746;
    else
        goto L_973e;

L_973e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_976e;
    else
        goto L_9746;

L_9746:
    if ((strcmpi(PszGetFleetName(lpfl->id), szName) == 0))
        goto LFoundFleetId;
    else
        goto L_9765;

L_9765:

L_976e:
    if ((iplPartial == -1))
        goto L_9780;
    else
        goto L_9777;

L_9777:
    ipl = iplPartial;
    goto GoWithPartial;

L_9780:
    return 0x0;
}

void GetScanFleetOrientation(FLEET *lpfl, POINT *ppt, POINT *pptD) {
    int16_t dy;
    int16_t dx;

L_978c:
    if ((lpfl->iPlayer != idPlayer))
        goto L_980a;
    else
        goto L_97a4;

L_97a4:
    if ((lpfl->cord <= 1))
        goto NoInfo;
    else
        goto L_97b1;

L_97b1:
    if ((lpfl->lpplord->rgord[1].iWarp != 0x0))
        goto L_97dd;
    else
        goto NoInfo;

NoInfo:
    dy = 0;
    dx = 0;
    goto L_985e;

L_97dd:
    dx = (lpfl->lpplord->rgord[0x1].pt.x - lpfl->pt.x);
    dy = (lpfl->lpplord->rgord[0x1].pt.y - lpfl->pt.y);

L_980a:
    if ((lpfl->fdirValid == 0x0))
        goto NoInfo;
    else
        goto L_9824;

L_9824:
    if ((lpfl->iwarpFlt == 0x0))
        goto NoInfo;
    else
        goto L_9833;

L_9833:

L_9839:
    dx = (lpfl->dirFltX - 127);
    dy = (lpfl->dirFltY - 127);

L_985e:
    GetDxDyOrientation(dx, dy, ppt, pptD);

L_9875:
    return;
}

void GetDxDyOrientation(int16_t dx, int16_t dy, POINT *ppt, POINT *pptD) {
    double   dbl;
    int16_t  iBmp;
    uint32_t t_merge_9938_0001_wide;

L_987c:
    iBmp = 0;
    if ((dx != 0))
        goto L_989f;
    else
        goto L_9893;

L_9893:
    if ((dy == 0))
        goto LFinishUp;
    else
        goto L_9899;

L_9899:

L_989f:
    dbl = ((((atan2((double)((int32_t)(dy)), (double)((int32_t)(dx))) + 3.1415927) * 4) / 3.141592654) + 0.5);
    __ftol();
    /* untranslated: iBmp = (0x8 - (loword(callresult(int32_t)) & 0x7)) */
    /* untranslated: iBmp = (((0x8 - (loword(callresult(int32_t)) & 0x7)) + 0x1) & 0x7) */

LFinishUp:
    if ((iScanZoom < 0))
        goto L_9935;
    else
        goto L_992f;

L_992f:
    t_merge_9938_0001_wide = 0x90009;
    goto L_9938;

L_9935:
    t_merge_9938_0001_wide = 0x70007;

L_9938:
    pptD->y = LOWORD(t_merge_9938_0001_wide);
    pptD->x = HIWORD(t_merge_9938_0001_wide);
    if ((iScanZoom < 0))
        goto L_9957;
    else
        goto L_994d;

L_994d:
    ppt->x = 7;
    goto L_995e;

L_9957:
    ppt->x = 0;

L_995e:
    ppt->y = LOWORD((iBmp * pptD->y));
    return;
}

int16_t FHandleMeasuringTape(SCAN *pscan, POINT pt) {
    HDC      hdc;
    HPEN     hpenSav;
    SBAR     sbar;
    POINT    ptLogLast;
    int16_t  grTypeIn;
    POINT    ptLogical;
    POINT    ptBase;
    int16_t  iropSav;
    POINT    ptNew;
    char     szT[20];
    int16_t  fVirgin;
    SCAN     scan;
    RECT     rc;
    int16_t  t_merge_9a39_0001;
    uint16_t t_merge_9a5d_0001;
    int16_t  t_merge_9a74_0001;
    uint16_t t_merge_9a98_0001;
    uint16_t t_merge_9ad0_0001;

L_9974:
    fVirgin = 1;
    ptLogLast.x = pscan->pt.x;
    ptLogLast.y = pscan->pt.y;
    ptBase.x = pscan->pt.x;
    ptBase.y = pscan->pt.y;
    LogicalToScan(&(ptBase));
    GetClientRect(hwndScanner, &(rc));
    rc.bottom = (rc.bottom - dySBar);
    hdc = GetDC(hwndScanner);
    SetCapture(hwndScanner);
    sbar.pscan = pscan;
    hpenSav = SelectObject(hdc, hpenShip);
    iropSav = SetROP2(hdc, 0x7);
    ptNew.x = pt.x;
    ptNew.y = pt.y;
    LogicalToScan(&(ptNew));

L_9a11:
    if ((FGetRMouseMove(&(ptNew)) == 0))
        goto L_9c7b;
    else
        goto L_9a25;

L_9a25:
    if ((rc.right >= ptNew.x))
        goto L_9a36;
    else
        goto L_9a30;

L_9a30:
    t_merge_9a39_0001 = rc.right;
    goto L_9a39;

L_9a36:
    t_merge_9a39_0001 = ptNew.x;

L_9a39:
    if ((0 <= t_merge_9a39_0001))
        goto L_9a49;
    else
        goto L_9a43;

L_9a43:
    t_merge_9a5d_0001 = 0x0;
    goto L_9a5d;

L_9a49:
    if ((rc.right >= ptNew.x))
        goto L_9a5a;
    else
        goto L_9a54;

L_9a54:
    t_merge_9a5d_0001 = rc.right;
    goto L_9a5d;

L_9a5a:
    t_merge_9a5d_0001 = ptNew.x;

L_9a5d:
    ptNew.x = t_merge_9a5d_0001;
    if ((rc.bottom >= ptNew.y))
        goto L_9a71;
    else
        goto L_9a6b;

L_9a6b:
    t_merge_9a74_0001 = rc.bottom;
    goto L_9a74;

L_9a71:
    t_merge_9a74_0001 = ptNew.y;

L_9a74:
    if ((0 <= t_merge_9a74_0001))
        goto L_9a84;
    else
        goto L_9a7e;

L_9a7e:
    t_merge_9a98_0001 = 0x0;
    goto L_9a98;

L_9a84:
    if ((rc.bottom >= ptNew.y))
        goto L_9a95;
    else
        goto L_9a8f;

L_9a8f:
    t_merge_9a98_0001 = rc.bottom;
    goto L_9a98;

L_9a95:
    t_merge_9a98_0001 = ptNew.y;

L_9a98:
    ptNew.y = t_merge_9a98_0001;
    ptLogical.x = ptNew.x;
    ptLogical.y = ptNew.y;
    ScanToLogical(&(ptLogical));
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_9acd;
    else
        goto L_9ac7;

L_9ac7:
    t_merge_9ad0_0001 = 0x8f;
    goto L_9ad0;

L_9acd:
    t_merge_9ad0_0001 = 0x4f;

L_9ad0:
    grTypeIn = t_merge_9ad0_0001;
    if ((FFindNearestObject(ptLogical, grTypeIn, &(scan)) == 0))
        goto L_9afc;
    else
        goto L_9af0;

L_9af0:
    ptLogical.x = scan.pt.x;
    ptLogical.y = scan.pt.y;

L_9afc:
    if ((ptLogLast.x != ptLogical.x))
        goto L_9b15;
    else
        goto L_9b0a;

L_9b0a:
    if ((ptLogLast.y == ptLogical.y))
        goto L_9a11;
    else
        goto L_9b0f;

L_9b0f:

L_9b15:
    ptNew.x = ptLogical.x;
    ptNew.y = ptLogical.y;
    LogicalToScan(&(ptNew));
    if ((fVirgin == 0))
        goto L_9b6f;
    else
        goto L_9b36;

L_9b36:
    if ((abs((ptLogical.x - ptLogLast.x)) >= 3))
        goto L_9b67;
    else
        goto L_9b4d;

L_9b4d:
    if ((abs((ptLogical.y - ptLogLast.y)) < 3))
        goto L_9a11;
    else
        goto L_9b61;

L_9b61:

L_9b67:
    fVirgin = 0;
    goto L_9b8b;

L_9b6f:
    MoveTo(hdc, ptBase.x, ptBase.y);
    LineTo(hdc, pt.x, pt.y);

L_9b8b:
    MoveTo(hdc, ptBase.x, ptBase.y);
    LineTo(hdc, ptNew.x, ptNew.y);
    if ((scan.grobj != grobjNone))
        goto L_9bce;
    else
        goto L_9bb0;

L_9bb0:
    sbar.id = -1;
    CchGetString(idsDeepSpace, szT);
    sbar.psz = szT;
    goto L_9c37;

L_9bce:
    if ((scan.grobj != grobjPlanet))
        goto L_9be0;
    else
        goto L_9bd7;

L_9bd7:
    sbar.id = scan.idpl;
    goto L_9c32;

L_9be0:
    if ((scan.grobj != grobjFleet))
        goto L_9c08;
    else
        goto L_9be9;

L_9be9:
    sbar.id = rglpfl[scan.ifl]->id;
    goto L_9c32;

L_9c08:
    if ((scan.grobj != grobjThing))
        goto L_9c2c;
    else
        goto L_9c11;

L_9c11:
    sbar.id = lpThings[scan.ith].idFull;
    goto L_9c32;

L_9c2c:
    sbar.id = scan.iwp;

L_9c32:
    sbar.psz = 0x0;

L_9c37:
    sbar.pt.x = ptLogical.x;
    sbar.pt.y = ptLogical.y;
    sbar.grbit = scan.grobj;
    DrawScannerSBar(hdc, 0x0, &(sbar), 0x0);
    pt.x = ptNew.x;
    pt.y = ptNew.y;
    ptLogLast.x = ptLogical.x;
    ptLogLast.y = ptLogical.y;
    goto L_9a11;

L_9c7b:
    if ((fVirgin != 0))
        goto L_9ca0;
    else
        goto L_9c84;

L_9c84:
    MoveTo(hdc, ptBase.x, ptBase.y);
    LineTo(hdc, pt.x, pt.y);

L_9ca0:
    SetROP2(hdc, iropSav);
    SelectObject(hdc, hpenSav);
    DrawScannerSBar(hdc, 0x0, 0x0, 0x0);
    ReleaseCapture();
    ReleaseDC(hwndScanner, hdc);
    if ((fVirgin != 0))
        goto L_9ced;
    else
        goto L_9ce7;

L_9ce7:
    return 0x1;

L_9ced:
    return 0x0;
}
