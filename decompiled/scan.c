#include "common.h"

uint32_t rgcrScanMine[3] = {16711680, 65535, 255};
int16_t  vrgPopRad[19] = {25, 50, 100, 200, 400, 800, 1000, 1500, 2250, 3000, 4000, 5000, 6000, 7500, 9000, 11000, 14000, 18000, 25000};

LRESULT CALLBACK ScannerWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    POINT16     pt;
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
    POINT       t_pt_027c;
    POINT       t_pt_028c_1;
    GrobjClass  t_merge_04e7_0001;
    int16_t     t_08b4;
    int32_t    *t_assign_1;
    int32_t    *t_assign_2;

    switch (msg) {
    case WM_MDIACTIVATE:
        hwndActive = wParam == 0 ? NULL : hwnd;
        break;
    case WM_CREATE:
        yScanTop = 1000;
        xScanTop = 1000;
        break;
    case WM_CHAR:
        switch (wParam) {
        case 118:
        case 86:
            hdc = GetDC(hwndScanner);
            pt = sel.scan.pt;
            LogicalToScan(&pt);
            iRopSav = SetROP2(hdc, 7);
            hpenSav = SelectObject(hdc, hpenYellow);
            for (i = 0; i < 2; i++) {
                MoveTo(hdc, pt.x - 300, pt.y - 300);
                LineTo(hdc, pt.x + 300, pt.y + 300);
                MoveTo(hdc, pt.x - 300, pt.y + 300);
                LineTo(hdc, pt.x + 300, pt.y - 300);
                if (i == 0) {
                    tick = GetTickCount();
                    while (GetTickCount() < tick + 150) {
                        Yield();
                    }
                }
            }
            SelectObject(hdc, hpenSav);
            SetROP2(hdc, iRopSav);
            ReleaseDC(hwndScanner, hdc);
            break;
        case 45:
            iScanNew = iScanZoom - 1;
            if (iScanNew >= -4)
                goto L_01ca;
            iScanNew = -4;
            goto L_01ca;
        default:
            iScanNew = iScanZoom + 1;
            if (iScanNew <= 4)
                goto L_01ca;
            iScanNew = 4;
            goto L_01ca;
        }
        break;
    L_01ca:
        if (iScanNew == iScanZoom)
            break;
        SendMessage(hwndFrame, WM_COMMAND, iScanNew + 3905, 0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (rglpfl != 0 && gd.fNoScannerDraw == 0) {
            GetClientRect(hwnd, &rc);
            DrawScannerSBar(hdc, &ps.rcPaint, NULL, 1);
            rc.bottom -= dySBar;
            DrawScanner(hdc, &ps.rcPaint);
        }
        EndPaint(hwnd, &ps);
        break;
    case WM_SETCURSOR:
        GetCursorPos(&t_pt_027c);
        pt = PointTo16(t_pt_027c);
        t_pt_028c_1 = PointFrom16(pt);
        ScreenToClient(hwndScanner, &t_pt_028c_1);
        pt = PointTo16(t_pt_028c_1);
        GetClientRect(hwnd, &rc);
        if (PtInRect(&rc, PointFrom16(pt)) == 0)
            goto Default;
        rc.bottom -= dySBar;
        if (PtInRect(&rc, PointFrom16(pt)) == 0) {
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(0x7f00)));
        } else if (sel.grobj == grobjFleet && ((GetAsyncKeyState(16) & 0xfffe) != 0 || (grbitScan & 0x10) != 0)) {
            SetCursor(hcurScanAdd);
        } else if (FNearAWayPoint(pt, 0) != 0) {
            SetCursor(hcurOpenGrab);
        } else if (gd.fSetMassMode != 0 || gd.fSetRouteMode != 0 ||
                   (sel.grobj == grobjPlanet &&
                    (((GetAsyncKeyState(16) & 0xfffe) != 0 && IWarpMAFromLppl(&sel.pl, NULL) > 0) || (GetAsyncKeyState(17) & 0xfffe) != 0))) {
            SetCursor(hcurScanAdd);
        } else {
            SetCursor(hcurScanner);
        }
        return 1;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        SetFocus(hwndFrame);
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        GetClientRect(hwnd, &rc);
        if (pt.y >= rc.bottom - dySBar) {
            if (msg != WM_LBUTTONDOWN || pt.y >= rc.bottom - (dySBar >> 1) || (sel.scan.grobjFull & 3) == 0)
                break;
            if (sel.scan.grobj == grobjFleet) {
                GlobalPD.grPopup = grPopupFleet;
                GlobalPD.lpfl = rglpfl[sel.scan.ifl];
            } else {
                GlobalPD.grPopup = grPopupUnknownObj;
            }
            Popup(hwnd, pt.x, pt.y);
            break;
        }
        ScanToLogical(&pt);
        t_merge_04e7_0001 = gd.fSetMassMode != 0 || gd.fSetRouteMode != 0 ? grobjPlanet : grobjPlanet | grobjFleet | grobjOther | grobjThing;
        FFindNearestObject(pt, t_merge_04e7_0001, &scan);
        if ((gd.fSetMassMode != 0 || (sel.grobj == grobjPlanet && (wParam & 4) != 0 && IWarpMAFromLppl(&sel.pl, NULL) > 0)) && msg == WM_LBUTTONDOWN) {
            DrawShipScanPath(NULL, 0);
            if (scan.idpl == sel.pl.id) {
                sel.pl.idFling = 0;
            } else {
                sel.pl.idFling = scan.idpl + 1;
            }
            FLookupPlanet(-1, &sel.pl);
            gd.fSetMassMode = 0;
            DrawShipScanPath(NULL, 1);
            DrawPlanShip(NULL, 16640);
            break;
        }
        if ((gd.fSetRouteMode != 0 || (sel.grobj == grobjPlanet && (wParam & 8) != 0)) && msg == WM_LBUTTONDOWN) {
            DrawShipScanPath(NULL, 0);
            if (scan.idpl == sel.pl.id) {
                sel.pl.idRoute = 0;
            } else {
                sel.pl.idRoute = scan.idpl + 1;
            }
            FLookupPlanet(-1, &sel.pl);
            gd.fSetRouteMode = 0;
            DrawShipScanPath(NULL, 1);
            DrawPlanShip(NULL, 16448);
            break;
        }
        if (msg == WM_MBUTTONDOWN || (msg == WM_RBUTTONDOWN && (wParam & 4) != 0)) {
            FHandleMeasuringTape(&scan, pt);
            break;
        }
        if (msg == WM_RBUTTONDOWN) {
            iChecked = -1;
            pt = scan.pt;
            if ((scan.grobjFull & 1) != 0) {
                rgid[0] = scan.idpl;
                rgid[1] = -1;
                c = 2;
                if (sel.grobj == grobjPlanet && sel.id == scan.idpl) {
                    iChecked = 0;
                }
            } else {
                c = 0;
            }
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0)
                    break;
                if (pt.x == lpfl->pt.x && pt.y == lpfl->pt.y) {
                    if (sel.grobj == grobjFleet && lpfl->id == sel.id) {
                        iChecked = c;
                    }
                    rgid[c++] = lpfl->id | 0x80000000;
                    if (c >= 98)
                        break;
                }
            }
            if (c == 2 && (scan.grobjFull & 1) != 0) {
                c = 1;
            }
            fSep = c == 0 ? 1 : 0;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (pt.x == lpth->pt.x && pt.y == lpth->pt.y) {
                    if (fSep == 0) {
                        rgid[c++] = -1;
                        fSep = 1;
                    }
                    t_08b4 = c;
                    c++;
                    t_assign_1 = &rgid[t_08b4];
                    *t_assign_1 = (int32_t)(((uint32_t)*t_assign_1 & 0xffff0000) | ((uint32_t)lpth->idFull & 0xffff));
                    t_assign_2 = &rgid[t_08b4];
                    *t_assign_2 = (int32_t)(((uint32_t)*t_assign_2 & 0xffff) | ((uint32_t)0x2000 & 0xffff) << 0x10);
                    if (c >= 100)
                        break;
                }
            }
            LogicalToScan(&pt);
            iSel = PopupMenu(hwnd, pt.x, pt.y, c, rgid, NULL, iChecked, 1);
            if (iSel < 0)
                break;
            if ((rgid[iSel] & 0x80000000) != 0) {
                scan.grobj = grobjFleet;
                id = LOWORD(rgid[iSel]);
                for (i = 0; i < cFleet; i++) {
                    lpfl = rglpfl[i];
                    if (rglpfl[i] == 0 || lpfl->id == id)
                        break;
                }
                scan.ifl = i;
            } else if ((rgid[iSel] & 0x20000000) != 0) {
                scan.grobj = grobjThing;
                lpth = lpThings;
                lpthMac = lpThings + cThing;
                for (; lpth < lpthMac && lpth->idFull != LOWORD(rgid[iSel]); lpth++) {
                }
                scan.ith = (int16_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18;
            } else {
                scan.grobj = grobjPlanet;
            }
            ChangeScanSel(&scan, 2);
            if (scan.grobj == grobjPlanet && (FLookupPlanet(scan.idpl, &plT) == 0 || plT.iPlayer != idPlayer))
                break;
        } else {
            if (sel.grobj == grobjFleet && ((wParam & 4) != 0 || (grbitScan & 0x10) != 0)) {
                FAddWayPoint(pt, &scan);
                break;
            }
            fChgScan = scan.pt.x == sel.scan.pt.x && scan.pt.y == sel.scan.pt.y;
            ChangeScanSel(&scan, 1);
            if ((FNearAWayPoint(pt, 1) != 0 && FHandleWayPointDrag(pt) != 0) || fChgScan == 0 || (scan.grobj != grobjFleet && scan.grobj != grobjPlanet))
                break;
            if (scan.pt.x == sel.pt.x && scan.pt.y == sel.pt.y) {
                if (FGetNextObjHere(&scan, 1) == 0)
                    break;
            } else if (scan.grobj == grobjPlanet) {
                if (FLookupPlanet(scan.idpl, &plT) == 0)
                    break;
                if (plT.iPlayer != idPlayer || ((scan.grobjFull & 2) != 0 && sel.grobj == grobjPlanet && scan.idpl == sel.id)) {
                    if ((scan.grobjFull & 2) == 0)
                        break;
                    scan.grobj = grobjFleet;
                }
            }
        }
        if ((scan.grobj == grobjFleet && rglpfl[scan.ifl]->iPlayer != idPlayer) || scan.grobj == grobjThing)
            break;
        if (scan.grobj == grobjFleet) {
            scan.iwp = -1;
        }
        ChangeScanSel(&scan, 1);
        RedrawScanSel(NULL, 0);
        ChangeMainObjSel(scan.grobj, scan.grobj == grobjPlanet ? scan.idpl : rglpfl[scan.ifl]->id);
        RedrawScanSel(NULL, 1);
        if (scan.grobj != grobjFleet || (scan.grobjFull & 1) == 0)
            break;
        scan.grobj = grobjPlanet;
        ChangeScanSel(&scan, 1);
        break;
    case WM_SIZE:
        SetScanScrollBars(hwnd);
        PostMessage(hwndFrame, WM_COMMAND, iScanZoom + 3905, 0);
        goto Default;
    case WM_HSCROLL:
    case WM_VSCROLL:
        if (wParam <= 5) {
            switch (wParam) {
            case 0:
                d = -dScanInc;
                break;
            case 1:
                d = dScanInc;
                break;
            case 2:
                d = -dScanPage;
                break;
            case 3:
                d = dScanPage;
                break;
            case 4:
            case 5:
                d = LOWORD(lParam) - (msg == WM_VSCROLL ? yScanTop : xScanTop);
                d &= 0xfffc;
            }
        } else {
            d = 0;
        }
        if (d == 0)
            break;
        if (msg == WM_VSCROLL) {
            dy = yScanTop;
            SetScrollPos(hwnd, 1, yScanTop + d, 1);
            yScanTop = GetScrollPos(hwnd, 1);
            ScrollScanner(0, PtToScan(dy - yScanTop));
            break;
        }
        dx = xScanTop;
        SetScrollPos(hwnd, 0, xScanTop + d, 1);
        xScanTop = GetScrollPos(hwnd, 0);
        ScrollScanner(PtToScan(dx - xScanTop), 0);
        break;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int16_t PtToScan(int16_t d) {
    if (iScanZoom == 0) {
        return d;
    }
    if ((uint16_t)(iScanZoom + 4) <= 8) {
        switch (iScanZoom) {
        case 4:
            d *= 4;
            break;
        case 3:
            d *= 2;
            break;
        case -2:
            d >>= 1;
            break;
        case -4:
            d >>= 2;
            break;
        case -3:
            d = ((d << 1) + d) >> 3;
            break;
        case -1:
            d = ((d << 1) + d) >> 2;
            break;
        case 1:
            d = ((d << 2) + d) >> 2;
            break;
        case 2:
            d = ((d << 1) + d) >> 1;
        }
    }
    return d;
}

int16_t ScanToPt(int16_t d) {
    if (iScanZoom == 0) {
        return d;
    }
    if ((uint16_t)(iScanZoom + 4) <= 8) {
        switch (iScanZoom) {
        case 4:
            d >>= 2;
            break;
        case 3:
            d >>= 1;
            break;
        case -2:
            d *= 2;
            break;
        case -4:
            d *= 4;
            break;
        case -3:
            d = (int16_t)(d * 8) / 3;
            break;
        case -1:
            d = (int16_t)(d * 4) / 3;
            break;
        case 1:
            d = (int16_t)(d * 4) / 5;
            break;
        case 2:
            d = (int16_t)(d * 2) / 3;
        }
    }
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
    POINT16  pt;
    int16_t  id;
    COLORREF crFore;
    int16_t  iBkPrev;
    POINT16  ptD;
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
    POINT16  ptO;
    int16_t  fSelected;
    int16_t  fMA;
    int16_t  fStarbase;
    POINT16  ptSelMain;
    THING   *lpthMac;
    int16_t  fStargate;
    uint16_t mdScanBase;
    int16_t  yMin;
    int16_t  dx;
    HBRUSH   hbrSav;
    int16_t  xLeft;
    int16_t  fDoDraw;
    POINT16  ptOrigin;
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
    POINT16  pt2;
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
    POINT    t_pt_1245_1;
    HGDIOBJ  t_merge_34aa_0001;
    HGDIOBJ  t_merge_34da_0001;
    HGDIOBJ  t_merge_359c_0001;
    HGDIOBJ  t_merge_35c8_0001;
    HGDIOBJ  t_merge_3b2d_0001;
    HGDIOBJ  t_merge_3b5d_0001;

    mdScanBase = grbitScan & 0xf;
    hdcScreen = 0;
    hbmpScreen = 0;
    if (rglpfl == 0 || gd.fNoScannerDraw != 0 || gd.fGeneratingTurn != 0) {
        return 0;
    }
    xLeft = xScanTop;
    yTop = yScanTop;
    ptOrigin.x = PtToScan(0xfa0 - xScanTop) & 7;
    ptOrigin.y = PtToScan(0xfa0 - yScanTop) & 7;
    GetClientRect(hwndScanner, &rc);
    ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
    l = (int16_t)(prc->right - prc->left);
    l = (uint32_t)(l * (int16_t)(prc->bottom - prc->top));
    if (l < 48000) {
        hdcMem = CreateCompatibleDC(hdc);
        if (hdcMem != 0) {
            hbmpScreen = CreateCompatibleBitmap(hdc, prc->right - (prc->left & 0xfff8), prc->bottom - (prc->top & 0xfff8));
            if (hbmpScreen == 0) {
                DeleteDC(hdcMem);
            }
        }
        if (hbmpScreen != 0) {
            hdcScreen = hdc;
            hdc = hdcMem;
            hbmpXSav = SelectObject(hdc, hbmpScreen);
            SetWindowOrg(hdc, prc->left & 0xfff8, prc->top & 0xfff8);
            pt.y = 0;
            pt.x = 0;
            t_pt_1245_1 = PointFrom16(pt);
            ClientToScreen(hwndScanner, &t_pt_1245_1);
            pt = PointTo16(t_pt_1245_1);
            ptOrigin.x = ptOrigin.x + 8 - (pt.x & 7) & 7;
            ptOrigin.y = ptOrigin.y + 8 - (pt.y & 7) & 7;
            rcDraw = *prc;
        }
    }
    FillRect(hdc, prc, GetStockObject(BLACK_BRUSH));
    rcClip = *prc;
    dExpand = (iScanZoom >= 3 && mdScanBase == 4) || (iScanZoom >= 0 && (mdScanBase == 1 || mdScanBase == 2)) ? 20 : 9;
    ExpandRc(prc, dExpand, dExpand);
    prc->bottom += 14;
    dx = ScanToPt(prc->right - prc->left);
    dy = ScanToPt(prc->bottom - prc->top);
    xMin = ScanToPt(prc->left) + xLeft;
    xMax = xMin + dx;
    yMax = dGalInv - yTop - ScanToPt(prc->top);
    yMin = yMax - dy;
    xOff = -xLeft;
    yOff = dGalInv - yTop;
    i = 0;
    id = 1;
    while (i < 16) {
        if (rgshdef[i].fFree != 0) {
            grbitScanShip &= ~(id & grbitScanShip);
        }
        i++;
        id *= 2;
    }
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpScanner);
    if (sel.grobj != grobjNone) {
        ptSelMain = sel.pt;
    } else {
        ptSelMain.y = -2;
        ptSelMain.x = -2;
    }
    if (gd.fFleetLinkValid == 0) {
        LinkFleets(0);
    } else {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            lpfl->fDone = 0;
        }
    }
    if ((grbitScan & 0x20) != 0) {
        fPlanetScanner = 0;
        dc.rgx = rgx;
        dc.rgy = rgy;
        dc.rgrad = rgrad;
        dc.cCur = 0;
        dc.cMax = 250;
        dc.hdc = hdc;
        dc.rcClip = *prc;
        dc.fCovered = 0;
        dc.fHollowOut = 0;
        IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
        hbrSav = SelectObject(hdc, hbrRadar);
        hpenSav = SelectObject(hdc, hpenRadar);
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            if (lppl->iPlayer == idPlayer) {
                dRange = GetPlanetScannerRange(lppl, &dPlanRange);
                if (dPlanRange > 0) {
                    fPlanetScanner = 1;
                }
                if (vpctRadarView < 100) {
                    dRange = MulDiv(dRange, vpctRadarView, 100);
                }
                id = lppl->id;
                rc.left = PtToScan(xOff + rgptPlan[id].x - dRange);
                rc.top = PtToScan(yOff - rgptPlan[id].y - dRange);
                rc.right = PtToScan(xOff + rgptPlan[id].x + dRange);
                rc.bottom = PtToScan(yOff - rgptPlan[id].y + dRange);
                DrawRadarCircle(&dc, &rc);
            }
        }
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpfl->iPlayer == idPlayer) {
                dRange = GetFleetScannerRange(lpfl, &dPlanRange, NULL, NULL);
                if (dPlanRange > 0) {
                    fPlanetScanner |= 2;
                }
                if (dRange > 0) {
                    if (vpctRadarView < 100) {
                        dRange = MulDiv(dRange, vpctRadarView, 100);
                    }
                    rc.left = PtToScan(xOff + lpfl->pt.x - dRange);
                    rc.top = PtToScan(yOff - lpfl->pt.y - dRange);
                    rc.right = PtToScan(xOff + lpfl->pt.x + dRange);
                    rc.bottom = PtToScan(yOff - lpfl->pt.y + dRange);
                    DrawRadarCircle(&dc, &rc);
                }
            }
        }
        DrawRadarCircle(&dc, NULL);
        if (hbrRadarNear == 0) {
            hbrRadarNear = HbrGet(vcScreenColors > 8 ? 24672 : 32639);
        }
        if (hpenRadarNear == 0) {
            hpenRadarNear = CreatePen(0, 1, vcScreenColors > 8 ? 24672 : 32639);
        }
        SelectObject(hdc, hbrRadarNear);
        SelectObject(hdc, hpenRadarNear);
        if ((fPlanetScanner & 1) != 0) {
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                if (lppl->iPlayer == idPlayer) {
                    dRange = GetPlanetScannerRange(lppl, &dPlanRange);
                    if (dPlanRange > 0) {
                        dRange >>= 1;
                        if (vpctRadarView < 100) {
                            dRange = MulDiv(dRange, vpctRadarView, 100);
                        }
                        id = lppl->id;
                        rc.left = PtToScan(xOff + rgptPlan[id].x - dRange);
                        rc.top = PtToScan(yOff - rgptPlan[id].y - dRange);
                        rc.right = PtToScan(xOff + rgptPlan[id].x + dRange);
                        rc.bottom = PtToScan(yOff - rgptPlan[id].y + dRange);
                        DrawRadarCircle(&dc, &rc);
                    }
                }
            }
        }
        if ((fPlanetScanner & 2) != 0) {
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0)
                    break;
                if (lpfl->iPlayer == idPlayer) {
                    dRange = GetFleetScannerRange(lpfl, &dPlanRange, NULL, NULL);
                    if (dPlanRange > 0) {
                        if (vpctRadarView < 100) {
                            dPlanRange = MulDiv(dPlanRange, vpctRadarView, 100);
                        }
                        rc.left = PtToScan(xOff + lpfl->pt.x - dPlanRange);
                        rc.top = PtToScan(yOff - lpfl->pt.y - dPlanRange);
                        rc.right = PtToScan(xOff + lpfl->pt.x + dPlanRange);
                        rc.bottom = PtToScan(yOff - lpfl->pt.y + dPlanRange);
                        DrawRadarCircle(&dc, &rc);
                    }
                }
            }
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMassAccel) {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (lpth->ith == ithMineralPacket && lpth->iplr == idPlayer && lpth->thp.iWarp != 0) {
                    dThingRange = lpth->thp.iWarp + 4;
                    dThingRange *= dThingRange;
                    if (vpctRadarView < 100) {
                        dThingRange = MulDiv(dThingRange, vpctRadarView, 100);
                    }
                    rc.left = PtToScan(xOff + lpth->pt.x - dThingRange);
                    rc.top = PtToScan(yOff - lpth->pt.y - dThingRange);
                    rc.right = PtToScan(xOff + lpth->pt.x + dThingRange);
                    rc.bottom = PtToScan(yOff - lpth->pt.y + dThingRange);
                    DrawRadarCircle(&dc, &rc);
                }
            }
        }
        DrawRadarCircle(&dc, NULL);
        SelectObject(hdc, hbrRadar);
        SelectObject(hdc, hpenRadar);
        if (mdScanBase != 5) {
            id = sel.grobj == grobjFleet ? sel.fl.id : -1;
            id2 = sel.scan.grobj == grobjFleet ? rglpfl[sel.scan.ifl]->id : -1;
            SelectObject(hdcMem, hbmpScanShip);
            SetTextColor(hdc, 0);
            SetBkColor(hdc, 0xffffff);
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0)
                    break;
                if (lpfl->idPlanet == -1 && (CShipsScanVis(lpfl) > 0 || lpfl->id == id || lpfl->id == id2)) {
                    pt = lpfl->pt;
                    if (pt.x >= xMin && pt.x < xMax && pt.y >= yMin && pt.y < yMax) {
                        pt.x = PtToScan(xOff + pt.x);
                        pt.y = PtToScan(yOff - pt.y);
                        fSelected = lpfl->pt.x == ptSelMain.x && lpfl->pt.y == ptSelMain.y;
                        if (fSelected != 0) {
                            SelectObject(hdcMem, hbmpScanner);
                            BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 11, 80, SRCAND);
                            SelectObject(hdcMem, hbmpScanShip);
                        } else {
                            GetScanFleetOrientation(lpfl, &ptO, &ptD);
                            BitBlt(hdc, pt.x - ptD.x / 2, pt.y - ptD.y / 2, ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCAND);
                        }
                    }
                }
            }
            SelectObject(hdcMem, hbmpScanner);
        }
        SelectObject(hdc, hpenSav);
        SelectObject(hdc, hbrSav);
    }
    if ((grbitScan & 0x40) != 0) {
        dc.rgx = rgx;
        dc.rgy = rgy;
        dc.rgrad = rgrad;
        dc.cCur = 0;
        dc.cMax = 250;
        dc.hdc = hdc;
        dc.rcClip = *prc;
        dc.fCovered = 0;
        dc.fHollowOut = 1;
        for (i = 0; i < 3; i++) {
            UnrealizeObject(rghbrPat[i]);
        }
        SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
        IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
        hbrSav = SelectObject(hdc, rghbrPat[0]);
        hpenSav = SelectObject(hdc, GetStockObject(NULL_PEN));
        ropSav = GetROP2(hdc);
        SetBkColor(hdc, 0);
        for (j = 0; j < 3; j++) {
            if ((j != 0 || (grbitScanMines & 1) != 0) && (j != 1 || (grbitScanMines & 2) != 0) && (j != 2 || (grbitScanMines & 0xc) != 0)) {
                for (i = 0; i < 3; i++) {
                    for (fDetonating = 0; fDetonating <= (i == 0 ? 1 : 0); fDetonating++) {
                        SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
                        SelectObject(hdc, rghbrPat[i]);
                        SetTextColor(hdc, fDetonating == 0 ? rgcrScanMine[j] : 16711935);
                        lpth = lpThings;
                        lpthMac = lpThings + cThing;
                        for (; lpth < lpthMac; lpth++) {
                            if (lpth->ith == ithMinefield && lpth->thm.iType == i && (j != 0 || lpth->iplr == idPlayer) &&
                                (j < 1 || (lpth->iplr != idPlayer && (rgplr[idPlayer].rgmdRelation[lpth->iplr] == 1 ? 1 : 0) != (j == 2 ? 1 : 0)))) {
                                if (j == 2 && (grbitScanMines & 0xc) != 0xc) {
                                    if (rgplr[idPlayer].rgmdRelation[lpth->iplr] == 0) {
                                        if ((grbitScanMines & 4) == 0)
                                            continue;
                                    } else if ((grbitScanMines & 8) == 0) {
                                        continue;
                                    }
                                }
                                if (lpth->thm.fDetonate == fDetonating) {
                                    pt = lpth->pt;
                                    dRange = LOWORD((int32_t)sqrt((double)lpth->thm.cMines));
                                    rc.left = PtToScan(xOff + pt.x - dRange);
                                    rc.top = PtToScan(yOff - pt.y - dRange);
                                    rc.right = PtToScan(xOff + pt.x + dRange);
                                    rc.bottom = PtToScan(yOff - pt.y + dRange);
                                    DrawRadarCircle(&dc, &rc);
                                    for (id = 0; id < game.cPlanMax && (rgptPlan[id].x != pt.x || rgptPlan[id].y != pt.y); id++) {
                                    }
                                    if (id == game.cPlanMax) {
                                        pt.x = PtToScan(xOff + pt.x);
                                        pt.y = PtToScan(yOff - pt.y);
                                        dRange = PtToScan(1);
                                        if (dRange < 1) {
                                            dRange = 1;
                                        } else if (dRange > 3) {
                                            dRange = 3;
                                        }
                                        SetRect(&rc, pt.x - dRange, pt.y - dRange, pt.x + dRange + 1, pt.y + dRange + 1);
                                        SetBkColor(hdc, rgcrScanMine[j]);
                                        ExtTextOut(hdc, rc.left, rc.top, 2, &rc, NULL, 0, NULL);
                                        SetBkColor(hdc, 0);
                                    }
                                }
                            }
                        }
                        DrawRadarCircle(&dc, NULL);
                    }
                }
            }
        }
        if (sel.scan.grobj == grobjThing) {
            lpth = lpThings + sel.scan.ith;
            if (lpth->ith == ithMinefield) {
                SetBrushOrg(hdc, ptOrigin.x, ptOrigin.y);
                SelectObject(hdc, rghbrPat[lpth->thm.iType]);
                SetTextColor(hdc, 16776960);
                pt = lpth->pt;
                dRange = LOWORD((int32_t)sqrt((double)lpth->thm.cMines));
                rc.left = PtToScan(xOff + pt.x - dRange);
                rc.top = PtToScan(yOff - pt.y - dRange);
                rc.right = PtToScan(xOff + pt.x + dRange);
                rc.bottom = PtToScan(yOff - pt.y + dRange);
                DrawRadarCircle(&dc, &rc);
                DrawRadarCircle(&dc, NULL);
            }
        }
        SetROP2(hdc, ropSav);
        SelectObject(hdc, hpenSav);
        SelectObject(hdc, hbrSav);
    }
    if (cThing != 0 && mdScanBase != 5) {
        IntersectClipRect(hdc, rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
        hbrSav = SelectObject(hdc, hbrShip);
        hpenSav = SelectObject(hdc, hpenDkPurple);
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            switch (lpth->ith) {
            case ithMineralPacket:
            case ithWormhole:
            case ithMysteryTrader:
                pt = lpth->pt;
                LogicalToScan(&pt);
                if (lpth->ith == ithWormhole && lpth->idFull < lpth->thw.idPartner && (1 << idPlayer & lpth->thw.grbitPlrTrav) != 0) {
                    lpthDest = LpthFromId(lpth->thw.idPartner);
                    if (lpthDest != 0) {
                        MoveTo(hdc, pt.x, pt.y);
                        pt2 = lpthDest->pt;
                        LogicalToScan(&pt2);
                        LineTo(hdc, pt2.x, pt2.y);
                        LineTo(hdc, pt2.x, pt2.y + 1);
                    }
                }
                if (lpth->ith == ithWormhole) {
                    BitBlt(hdc, pt.x - 4, pt.y - 4, 9, 9, hdcMem, 9, 92, SRCAND);
                    BitBlt(hdc, pt.x - 4, pt.y - 4, 9, 9, hdcMem, 0, 92, SRCPAINT);
                } else if (lpth->ith == ithMysteryTrader) {
                    hbmpTrSav = SelectObject(hdcMem, hbmpScanShip);
                    crFore = SetTextColor(hdc, 16776960);
                    crBack = SetBkColor(hdc, 0);
                    GetDxDyOrientation(lpth->tht.ptDest.x - lpth->pt.x, lpth->tht.ptDest.y - lpth->pt.y, &ptO, &ptD);
                    BitBlt(hdc, pt.x - ptD.x / 2, pt.y - ptD.y / 2, ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCPAINT);
                    SetTextColor(hdc, crFore);
                    SetBkColor(hdc, crBack);
                    SelectObject(hdcMem, hbmpTrSav);
                } else {
                    dRange = iScanZoom <= 0 ? 2 : iScanZoom > 2 ? 5 : 3;
                    dx = dRange * 2 + 1;
                    if (lpth->thp.iWarp == 0) {
                        SelectObject(hdc, hpenYellow);
                        MoveTo(hdc, pt.x, pt.y - dRange - 1);
                        LineTo(hdc, pt.x - dRange - 1, pt.y);
                        LineTo(hdc, pt.x, pt.y + dRange + 1);
                        LineTo(hdc, pt.x + dRange + 1, pt.y);
                        LineTo(hdc, pt.x, pt.y - dRange - 1);
                        SelectObject(hdc, hpenDkPurple);
                    } else {
                        if (lpth->iplr != idPlayer) {
                            SelectObject(hdc, hbrRed);
                        }
                        PatBlt(hdc, pt.x - dRange, pt.y - dRange, dx, 1, PATCOPY);
                        PatBlt(hdc, pt.x - dRange, pt.y - dRange, 1, dx, PATCOPY);
                        PatBlt(hdc, pt.x - dRange, pt.y + dRange, dx, 1, PATCOPY);
                        PatBlt(hdc, pt.x + dRange, pt.y - dRange, 1, dx, PATCOPY);
                        if (lpth->iplr != idPlayer) {
                            SelectObject(hdc, hbrShip);
                        }
                    }
                }
            }
        }
        SelectObject(hdc, hbrSav);
        SelectObject(hdc, hpenSav);
    }
    if ((grbitScan & 0x80) != 0 && mdScanBase != 5) {
        id = sel.grobj == grobjFleet ? sel.fl.id : -1;
        id2 = sel.scan.grobj == grobjFleet ? rglpfl[sel.scan.ifl]->id : -1;
        hpenSav = SelectObject(hdc, hpenStarbase);
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpfl->fDead == 0 && lpfl->det >= 7 && lpfl->cord > 1 && (CShipsScanVis(lpfl) > 0 || lpfl->id == id || lpfl->id == id2)) {
                for (iord = 0; iord < lpfl->cord; iord++) {
                    pt = lpfl->lpplord->rgord[iord].pt;
                    pt.x = PtToScan(xOff + pt.x);
                    pt.y = PtToScan(yOff - pt.y);
                    if (iord == 0) {
                        MoveTo(hdc, pt.x, pt.y);
                    } else {
                        LineTo(hdc, pt.x, pt.y);
                    }
                }
            }
        }
        SelectObject(hdc, hpenSav);
    }
    SelectClipRgn(hdc, hrgnHuge);
    GetClientRect(hwndScanner, &rc);
    ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
    memset(rgWhatsHere, 0, 999);
    if ((uint16_t)(iScanZoom + 1) <= 5) {
        switch (iScanZoom) {
        case 4:
            SelectObject(hdc, rghfontArial10[1]);
            break;
        case 3:
            SelectObject(hdc, rghfontArial8[1]);
            break;
        case 0:
        case 1:
        case 2:
            SelectObject(hdc, rghfontArial8[0]);
            break;
        case -1:
            SelectObject(hdc, rghfontArial6[0]);
        }
    }
    crFore = SetTextColor(hdc, 0xffffff);
    crBack = SetBkColor(hdc, 0);
    iBkPrev = SetBkMode(hdc, TRANSPARENT);
    j = iScanZoom >= 3 && mdScanBase == 4 ? 11 : 0;
    for (i = 0; i < game.cPlanMax; i++) {
        if (rgptPlan[i].x >= xMin && rgptPlan[i].x < xMax && rgptPlan[i].y >= yMin && rgptPlan[i].y < yMax) {
            fDoDraw = 1;
        } else {
            if ((grbitScan & 0x400) == 0)
                continue;
            fDoDraw = 0;
        }
        pt.x = PtToScan(xOff + rgptPlan[i].x);
        pt.y = PtToScan(yOff - rgptPlan[i].y);
        if (fDoDraw != 0) {
            if (ptSelMain.x == rgptPlan[i].x && ptSelMain.y == rgptPlan[i].y && mdScanBase <= 2) {
                BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 0, 69, SRCAND);
                BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 0, 33, SRCPAINT);
            } else {
                BitBlt(hdc, pt.x - 1, pt.y - 1, 3, 3, hdcMem, 11, 15, SRCCOPY);
            }
        }
        if ((grbitScan & 0x400) != 0 && iScanZoom >= -1 && rgptPlan[i].x >= xMin - 50 && rgptPlan[i].x < xMax + 50 && rgptPlan[i].y >= yMin - 20 &&
            rgptPlan[i].y < yMax + 20) {
            PszGetPlanetName(i);
            if ((grbitScan & 0x2000) != 0) {
                lppl = LpplFromId(i);
                if (lppl != 0 && lppl->iPlayer != -1) {
                    SetTextColor(hdc, lppl->iPlayer == idPlayer ? 0xffffff : rgcrPlrHistory[lppl->iPlayer]);
                }
            }
            CtrTextOut(hdc, pt.x, pt.y + 5 + j, szWork, 0);
            if ((grbitScan & 0x2000) != 0) {
                SetTextColor(hdc, 0xffffff);
            }
        }
    }
    SetBkMode(hdc, iBkPrev);
    SetBkColor(hdc, crBack);
    SetTextColor(hdc, crFore);
    j = iScanZoom >= 3 && mdScanBase == 4 ? 11 : 0;
    if (sel.grobj != grobjNone && sel.pt.x == sel.scan.pt.x && sel.pt.y == sel.scan.pt.y) {
        pt = sel.pt;
        LogicalToScan(&pt);
        BitBlt(hdc, pt.x - 5, pt.y + 11 + j, 11, 12, hdcMem, 0, 80, SRCAND);
        BitBlt(hdc, pt.x - 5, pt.y + 11 + j, 11, 12, hdcMem, 0, 57, SRCPAINT);
    } else if (sel.scan.grobj != grobjNone) {
        pt = sel.scan.pt;
        LogicalToScan(&pt);
        BitBlt(hdc, pt.x - 3, pt.y + 7 + j, 7, 8, hdcMem, 22, 80, SRCAND);
        BitBlt(hdc, pt.x - 3, pt.y + 7 + j, 7, 8, hdcMem, 22, 49, SRCPAINT);
    }
    if (cPlanet != 0 && mdScanBase != 5) {
        lppl = lpPlanets;
        i = 0;
        while (i < cPlanet) {
            id = lppl->id;
            fStarbase = lppl->fStarbase != 0 && lppl->iPlayer != -1;
            if (fStarbase != 0) {
                if (rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef == ihuldefOrbitalFort) {
                    fStarbase = 2;
                }
                fMA = IWarpMAFromLppl(lppl, NULL) <= 0 ? 0 : 1;
                fStargate = IStargateFromLppl(lppl) == -1 ? 0 : 1;
            } else {
                fMA = 0;
                fStargate = 0;
            }
            if (rgptPlan[id].x >= xMin && rgptPlan[id].x < xMax && rgptPlan[id].y >= yMin && rgptPlan[id].y < yMax) {
                pt.x = PtToScan(xOff + rgptPlan[id].x);
                pt.y = PtToScan(yOff - rgptPlan[id].y);
                switch (mdScanBase) {
                case 3:
                    fTerra = 0;
                    if (lppl->det < 3)
                        break;
                    pctDesire = PctPlanetDesirability(lppl, idPlayer);
                    if (pctDesire < 0 || GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raTerra) {
                        pctDesire = PctPlanetOptValue(lppl, idPlayer);
                        if (pctDesire >= 0 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raTerra) {
                            fTerra = 1;
                        }
                    }
                    t_merge_34aa_0001 = pctDesire < 0 ? hbrRadar : fTerra != 0 ? hbrDkYellow : hbrGreen;
                    hbrSav = SelectObject(hdc, t_merge_34aa_0001);
                    t_merge_34da_0001 = pctDesire < 0 ? hpenRadar : fTerra != 0 ? hpenDkYellow : hpenDkGreen;
                    hpenSav = SelectObject(hdc, t_merge_34da_0001);
                    if (pctDesire >= 0) {
                        dRad = pctDesire / 11 + 2;
                    } else {
                        dRad = (int16_t)-pctDesire / 5 + 2;
                    }
                    if (dRad > 10) {
                        dRad = 10;
                    }
                    Ellipse(hdc, pt.x - dRad, pt.y - dRad, pt.x + dRad + 1, pt.y + dRad + 1);
                    dRad -= 2;
                    if (dRad < 3) {
                        dRad++;
                    }
                    if (dRad < 1) {
                        dRad = 1;
                    }
                    t_merge_359c_0001 = pctDesire < 0 ? hbrEnemy : fTerra != 0 ? hbrYellow : hbrShip;
                    SelectObject(hdc, t_merge_359c_0001);
                    t_merge_35c8_0001 = pctDesire < 0 ? hpenEnemy : fTerra != 0 ? hpenYellow : hpenShip;
                    SelectObject(hdc, t_merge_35c8_0001);
                    Ellipse(hdc, pt.x - dRad, pt.y - dRad, pt.x + dRad + 1, pt.y + dRad + 1);
                    if (lppl->iPlayer != -1) {
                        rc.left = pt.x - 1;
                        rc.right = rc.left + 9;
                        rc.top = pt.y - 20;
                        rc.bottom = rc.top + 8;
                        FillRect(hdc, &rc, GetStockObject(BLACK_BRUSH));
                        if (lppl->iPlayer == idPlayer) {
                            hbr = hbrBBlue;
                        } else if (rgplr[idPlayer].rgmdRelation[lppl->iPlayer] == 1) {
                            hbr = hbrStarbase;
                        } else if (rgplr[idPlayer].rgmdRelation[lppl->iPlayer] == 0) {
                            hbr = hbrRadar;
                        } else {
                            hbr = hbrEnemy;
                        }
                        SelectObject(hdc, hbr);
                        PatBlt(hdc, pt.x, pt.y - 19, 1, 21, PATCOPY);
                        PatBlt(hdc, pt.x, pt.y - 19, 7, 6, PATCOPY);
                    }
                    SelectObject(hdc, hpenSav);
                    SelectObject(hdc, hbrSav);
                    break;
                case 1:
                case 2:
                    fConc = mdScanBase == 2 ? 1 : 0;
                    iOff = iScanZoom >= 0 ? 0 : 1;
                    if (lppl->det < 4 && (fConc == 0 || lppl->det < 3))
                        goto LNormalScannerMode;
                    xOut = pt.x - vrgScanPO[iOff][0];
                    yOut = pt.y - vrgScanPO[iOff][1];
                    hbrSav = SelectObject(hdc, hbrButtonFace);
                    PatBlt(hdc, xOut - 2, yOut, vrgScanPO[iOff][2], 1, PATCOPY);
                    PatBlt(hdc, xOut - 2, yOut - vrgScanPO[iOff][2] + 1, 1, vrgScanPO[iOff][2], PATCOPY);
                    for (j = 0; j < 3; j++) {
                        if (fConc != 0) {
                            l = (int16_t)((int16_t)lppl->rgMinConc[j] / 5);
                            if (l > 20) {
                                l = 20;
                            }
                        } else {
                            l = lppl->rgwtMin[j];
                            l = (int32_t)(((int16_t)(cMinGrafMax / 40) + l) / (int16_t)(cMinGrafMax / 20));
                            if (l > 20) {
                                l = 20;
                            }
                        }
                        if (iOff != 0) {
                            l = (int32_t)(l / 2);
                        }
                        if (l > 0) {
                            SelectObject(hdc, rghbrMineral[j]);
                            PatBlt(hdc, xOut, yOut - LOWORD(l), vrgScanPO[iOff][3], LOWORD(l), PATCOPY);
                        }
                        xOut += vrgScanPO[iOff][4];
                    }
                    SelectObject(hdc, hbrSav);
                    goto LNormalScannerMode;
                case 4:
                    fTerra = 0;
                    if (lppl->det >= 3 && lppl->iPlayer != -1) {
                        if (lppl->iPlayer == idPlayer) {
                            lPop = lppl->rgwtMin[3];
                        } else {
                            lPop = (int32_t)(lppl->uPopGuess * 4);
                        }
                        for (dRad = 0; dRad < 19 && vrgPopRad[dRad] <= lPop; dRad++) {
                        }
                        dRad += 2;
                        if (lppl->iPlayer == idPlayer) {
                            iRel = 0;
                        } else if (rgplr[idPlayer].rgmdRelation[lppl->iPlayer] == 1) {
                            iRel = 1;
                        } else {
                            iRel = 3;
                        }
                        t_merge_3b2d_0001 = iRel == 0 ? hbrShip : iRel == 3 ? hbrEnemy : hbrYellow;
                        hbrSav = SelectObject(hdc, t_merge_3b2d_0001);
                        t_merge_3b5d_0001 = iRel == 0 ? hpenDkGreen : iRel == 3 ? hpenEnemy : hpenDkYellow;
                        hpenSav = SelectObject(hdc, t_merge_3b5d_0001);
                        if (iScanZoom < 3) {
                            dRad = (dRad + 1) >> 1;
                        }
                        Ellipse(hdc, pt.x - dRad, pt.y - dRad, pt.x + dRad + 1, pt.y + dRad + 1);
                        SelectObject(hdc, hpenSav);
                        SelectObject(hdc, hbrSav);
                        break;
                    }
                    if (lppl->iPlayer != -1)
                        break;
                default:
                LNormalScannerMode:
                    if (lppl->iPlayer == -1) {
                        if (rgptPlan[id].x != ptSelMain.x || rgptPlan[id].y != ptSelMain.y) {
                            BitBlt(hdc, pt.x - 1, pt.y - 1, 3, 3, hdcMem, 11, 18, SRCCOPY);
                        }
                    } else if (rgptPlan[id].x == ptSelMain.x && rgptPlan[id].y == ptSelMain.y) {
                        if (lppl->iPlayer == idPlayer) {
                            yBmp = 0;
                        } else if (rgplr[idPlayer].rgmdRelation[lppl->iPlayer] == 1) {
                            yBmp = 22;
                        } else {
                            yBmp = 11;
                        }
                        BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 0, 69, SRCAND);
                        BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 0, yBmp, SRCPAINT);
                        if (fStarbase != 0) {
                            hbrSav = SelectObject(hdc, fStarbase == 2 ? hbrBlue : hbrYellow);
                            PatBlt(hdc, pt.x + 4, pt.y - 6, 5, 5, BLACKNESS);
                            PatBlt(hdc, pt.x + 5, pt.y - 6, 3, 5, PATCOPY);
                            PatBlt(hdc, pt.x + 4, pt.y - 5, 5, 3, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                        if (fStargate != 0) {
                            hbrSav = SelectObject(hdc, hbrGreen);
                            PatBlt(hdc, pt.x - 7, pt.y - 6, 5, 5, BLACKNESS);
                            PatBlt(hdc, pt.x - 6, pt.y - 6, 3, 5, PATCOPY);
                            PatBlt(hdc, pt.x - 7, pt.y - 5, 5, 3, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                        if (fMA != 0) {
                            hbrSav = SelectObject(hdc, hbrPurple);
                            PatBlt(hdc, pt.x - 2, pt.y - 9, 5, 5, BLACKNESS);
                            PatBlt(hdc, pt.x - 1, pt.y - 9, 3, 5, PATCOPY);
                            PatBlt(hdc, pt.x - 2, pt.y - 8, 5, 3, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                    } else {
                        if (lppl->iPlayer == idPlayer) {
                            yBmp = 0;
                        } else if (rgplr[idPlayer].rgmdRelation[lppl->iPlayer] == 1) {
                            yBmp = 10;
                        } else {
                            yBmp = 5;
                        }
                        BitBlt(hdc, pt.x - 2, pt.y - 2, 5, 5, hdcMem, 11, yBmp, SRCCOPY);
                        if (fStarbase != 0) {
                            hbrSav = SelectObject(hdc, fStarbase == 2 ? hbrBlue : hbrYellow);
                            PatBlt(hdc, pt.x + 3, pt.y - 4, 3, 3, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                        if (fStargate != 0) {
                            hbrSav = SelectObject(hdc, hbrGreen);
                            PatBlt(hdc, pt.x - 5, pt.y - 4, 3, 3, BLACKNESS);
                            PatBlt(hdc, pt.x - 4, pt.y - 4, 1, 3, PATCOPY);
                            PatBlt(hdc, pt.x - 5, pt.y - 3, 3, 1, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                        if (fMA != 0) {
                            hbrSav = SelectObject(hdc, hbrPurple);
                            PatBlt(hdc, pt.x - 1, pt.y - 6, 3, 3, BLACKNESS);
                            PatBlt(hdc, pt.x, pt.y - 6, 1, 3, PATCOPY);
                            PatBlt(hdc, pt.x - 1, pt.y - 5, 3, 1, PATCOPY);
                            SelectObject(hdc, hbrSav);
                        }
                    }
                }
            }
            i++;
            lppl++;
        }
    }
    if (cFleet != 0 && mdScanBase != 5) {
        lpflT = *rglpfl;
        hbrSav = SelectObject(hdc, hbrShip);
        id = sel.grobj == grobjFleet ? sel.fl.id : -1;
        id2 = sel.scan.grobj == grobjFleet ? rglpfl[sel.scan.ifl]->id : -1;
        for (i = 0; i < cFleet; i++) {
            lpflT = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (CShipsScanVis(lpflT) > 0 || lpflT->id == id || lpflT->id == id2) {
                idP = lpflT->idPlanet;
                if (idP != -1) {
                    pt = rgptPlan[idP];
                } else {
                    pt = lpflT->pt;
                }
                if (pt.x >= xMin && pt.x < xMax && pt.y >= yMin && pt.y < yMax) {
                    pt.x = PtToScan(xOff + pt.x);
                    pt.y = PtToScan(yOff - pt.y);
                    yBmp = lpflT->iPlayer == idPlayer ? 0 : 1;
                    fSelected = lpflT->pt.x == ptSelMain.x && lpflT->pt.y == ptSelMain.y;
                    if (idP != -1) {
                        if ((int16_t)(int8_t)rgWhatsHere[idP] != 3 && (int16_t)(int8_t)rgWhatsHere[idP] != yBmp + 1 && mdScanBase <= 2) {
                            rgWhatsHere[idP] += LOBYTE(yBmp + 1);
                            yBmp = (int16_t)(int8_t)rgWhatsHere[idP] - 1;
                            if (fSelected != 0) {
                                BitBlt(hdc, pt.x - 9, pt.y - 9, 19, 19, hdcMem, 29, 69, SRCAND);
                                BitBlt(hdc, pt.x - 9, pt.y - 9, 19, 19, hdcMem, 29, 19 * yBmp, SRCPAINT);
                            } else {
                                BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 16, 69, SRCAND);
                                BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 16, 11 * yBmp, SRCPAINT);
                            }
                            if ((grbitScan & 0x1000) != 0 && lpflT->fDone == 0) {
                                DrawScanFleetCount(lpflT, pt.x, pt.y - (fSelected == 0 ? 5 : 9) - 2, hdc, hdcMem);
                            }
                        }
                    } else if (fSelected != 0) {
                        BitBlt(hdc, pt.x - 5, pt.y - 5, 11, 11, hdcMem, 11, 11 * yBmp + 36, SRCPAINT);
                        if ((grbitScan & 0x1000) != 0 && lpflT->fDone == 0) {
                            DrawScanFleetCount(lpflT, pt.x, pt.y - 7, hdc, hdcMem);
                        }
                    } else {
                        SelectObject(hdcMem, hbmpScanShip);
                        if (yBmp == 0) {
                            cr = 16711680;
                        } else if (rgplr[idPlayer].rgmdRelation[lpflT->iPlayer] == 1) {
                            cr = 0xffff;
                        } else {
                            cr = 0xff;
                        }
                        SetTextColor(hdc, cr);
                        SetBkColor(hdc, 0);
                        GetScanFleetOrientation(lpflT, &ptO, &ptD);
                        BitBlt(hdc, pt.x - ptD.x / 2, pt.y - ptD.y / 2, ptD.x, ptD.y, hdcMem, ptO.x, ptO.y, SRCPAINT);
                        if ((grbitScan & 0x1000) != 0 && lpflT->fDone == 0) {
                            DrawScanFleetCount(lpflT, pt.x, pt.y - ptD.y / 2 - 2, hdc, hdcMem);
                        }
                        SelectObject(hdcMem, hbmpScanner);
                    }
                }
            }
        }
        SelectObject(hdc, hbrSav);
    }
    ExpandRc(prc, -dExpand, -dExpand);
    prc->bottom -= 14;
    crFore = SetTextColor(hdc, 0);
    crBack = GetBkColor(hdc);
    if (sel.grobj == grobjFleet || sel.scan.grobj == grobjFleet || sel.scan.grobj == grobjThing || (sel.grobj == grobjPlanet && sel.pl.fStarbase != 0)) {
        IntersectClipRect(hdc, prc->left, prc->top, prc->right, prc->bottom);
        fOrdersVis = 0;
        DrawShipScanPath(hdc, 1);
        SelectClipRgn(hdc, hrgnHuge);
    }
    SetBkColor(hdc, crBack);
    SetTextColor(hdc, crFore);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    if (hdcScreen != 0) {
        SetWindowOrg(hdc, 0, 0);
        BitBlt(hdcScreen, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, hdc, prc->left & 7, prc->top & 7, SRCCOPY);
        SelectObject(hdc, hbmpXSav);
        DeleteObject(hbmpScreen);
        DeleteDC(hdc);
    }
    return 0;
}

void DrawScanFleetCount(FLEET *lpfl, int16_t x, int16_t y, HDC hdc, HDC hdcMem) {
    int32_t  l2;
    int16_t  f999;
    COLORREF cr;
    FLEET   *lpflWalk;
    int16_t  iPlr;
    HBITMAP  hbmpSav;
    int32_t  l;

    lpflWalk = lpfl;
    iPlr = (grbitScan & 0x2000) == 0 ? -2 : -1;
    l = 0;
    f999 = 0;
    do {
        l2 = CShipsScanVis(lpflWalk);
        if (l2 > 0) {
            l += l2;
            if (lpflWalk->iPlayer != iPlr && iPlr != -2) {
                if (iPlr == -1) {
                    iPlr = lpflWalk->iPlayer;
                } else {
                    iPlr = -2;
                }
            }
        }
        lpflWalk->fDone = 1;
        lpflWalk = lpflWalk->lpflNext;
    } while (lpflWalk != lpfl);
    if (l > 999) {
        l = 999;
    } else if (l <= 0) {
        return;
    }
    hbmpSav = SelectObject(hdcMem, hbmpNumbers);
    SetBkColor(hdc, 0);
    SetTextColor(hdc, 0xffffff);
    if (iPlr >= 0 && iPlr != idPlayer) {
        cr = rgcrPlrHistory[iPlr];
    } else {
        cr = 0xffffff;
    }
    x--;
    y -= 7;
    if (l > 99) {
        f999 = 1;
        x -= 5;
        if (cr != 0xffffff) {
            BitBlt(hdc, x, y, 4, 7, hdcMem, (int16_t)LOWORD(l) / 100 * 4, 0, 0x220326);
            SetTextColor(hdc, cr);
        }
        BitBlt(hdc, x, y, 4, 7, hdcMem, (int16_t)LOWORD(l) / 100 * 4, 0, SRCPAINT);
        if (cr != 0xffffff) {
            SetTextColor(hdc, 0xffffff);
        }
        x += 8;
        l = (int32_t)(l % 100);
    }
    if (l > 9 || f999 != 0) {
        x -= 3;
        if (cr != 0xffffff) {
            BitBlt(hdc, x, y, 4, 7, hdcMem, (int16_t)LOWORD(l) / 10 * 4, 0, 0x220326);
            SetTextColor(hdc, cr);
        }
        BitBlt(hdc, x, y, 4, 7, hdcMem, (int16_t)LOWORD(l) / 10 * 4, 0, SRCPAINT);
        if (cr != 0xffffff) {
            SetTextColor(hdc, 0xffffff);
        }
        x += 5;
        l = (int32_t)(l % 10);
    }
    if (cr != 0xffffff) {
        BitBlt(hdc, x, y, 4, 7, hdcMem, LOWORD(l) * 4, 0, 0x220326);
        SetTextColor(hdc, cr);
    }
    BitBlt(hdc, x, y, 4, 7, hdcMem, LOWORD(l) * 4, 0, SRCPAINT);
    if (cr != 0xffffff) {
        SetTextColor(hdc, 0xffffff);
    }
    SelectObject(hdcMem, hbmpSav);
    return;
}

int32_t CShipsScanVis(FLEET *lpfl) {
    int16_t  j;
    int32_t  csh;
    int16_t  k;
    uint16_t grbitSh;

    csh = 0;
    if ((grbitScan & 0x100) != 0) {
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->cord == 1) {
                switch (lpfl->lpplord->rgord[0].grTask) {
                case grTaskLayMines:
                case grTaskScrap:
                case grTaskMine:
                case grTaskPatrol:
                    break;
                default:
                    goto L_4c92;
                }
            }
            return 0;
        }
    L_4c92:
        if (lpfl->iPlayer != idPlayer && lpfl->iwarpFlt == 0) {
            return 0;
        }
    }
    if ((grbitScan & 0x200) != 0 && lpfl->iPlayer == idPlayer) {
        grbitSh = grbitScanShip;
        j = 0;
        for (; grbitSh != 0; grbitSh >>= 1) {
            if ((grbitSh & 1) != 0 && lpfl->rgcsh[j] > 0) {
                csh += lpfl->rgcsh[j];
            }
            j++;
        }
    } else if ((grbitScan & 0x800) != 0 && lpfl->iPlayer != idPlayer) {
        grbitSh = grbitScanEShip;
        j = 0;
        for (; grbitSh != 0; grbitSh >>= 1) {
            if ((grbitSh & 1) != 0) {
                for (k = 0; k < 16; k++) {
                    if (lpfl->rgcsh[k] > 0 && LphuldefFromId(rglpshdef[lpfl->iPlayer][k].hul.ihuldef)->imdCategory == j) {
                        csh += lpfl->rgcsh[k];
                    }
                }
            }
            j++;
        }
    } else {
        for (j = 0; j < 16; j++) {
            csh += lpfl->rgcsh[j];
        }
    }
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
    int32_t  t_scratch_m42;
    double   t_scratch_m3a_2;

    if (prc == 0 || (pdc->fCovered == 0 && IntersectRect(&rc, prc, &pdc->rcClip) != 0)) {
        if (prc != 0) {
            rad = (prc->right - prc->left) >> 1;
            x = prc->left + rad;
            y = prc->top + rad;
            r2 = (uint32_t)(rad * rad);
            for (i = 0; i < 4; i++) {
                x2 = i >= 2 ? pdc->rcClip.right : pdc->rcClip.left;
                y2 = (i & 1) == 0 ? pdc->rcClip.bottom : pdc->rcClip.top;
                dx = x2 - x;
                dy = y2 - y;
                if (dx > r2 || dy > r2 || (uint32_t)(dx * dx) + (uint32_t)(dy * dy) > r2)
                    break;
            }
            if (i == 4) {
                pdc->fCovered = 1;
                pdc->cCur = 0;
            } else if (pdc->cCur != pdc->cMax) {
                iFree = pdc->cCur;
                for (i = 0; i < pdc->cCur; i++) {
                    if (pdc->rgrad[i] <= 0) {
                        iFree = i;
                    } else {
                        x2 = pdc->rgx[i];
                        y2 = pdc->rgy[i];
                        dx = x - x2;
                        dy = y - y2;
                        l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                        if (pdc->rgrad[i] < rad) {
                            t_scratch_m42 = pdc->rgrad[i];
                            if (sqrt((double)l) + (double)t_scratch_m42 <= (double)rad) {
                                pdc->rgrad[i] = 0;
                                iFree = i;
                            }
                        } else if (pdc->rgrad[i] > rad) {
                            t_scratch_m3a_2 = (double)pdc->rgrad[i];
                            if (sqrt((double)l) + (double)rad <= t_scratch_m3a_2) {
                                return;
                            }
                        } else if (x2 == x && y2 == y) {
                            return;
                        }
                    }
                }
                pdc->rgx[iFree] = x;
                pdc->rgy[iFree] = y;
                pdc->rgrad[iFree] = rad;
                if (iFree != pdc->cCur) {
                    return;
                }
                pdc->cCur++;
                return;
            }
            if (pdc->fHollowOut != 0) {
                SetROP2(pdc->hdc, 9);
                crSav = SetBkColor(pdc->hdc, 0xffffff);
                Ellipse(pdc->hdc, prc->left, prc->top, prc->right, prc->bottom);
                SetBkColor(pdc->hdc, crSav);
                SetROP2(pdc->hdc, 15);
            }
            Ellipse(pdc->hdc, prc->left, prc->top, prc->right, prc->bottom);
        } else {
            if (pdc->fHollowOut != 0) {
                SetROP2(pdc->hdc, 9);
                crSav = SetBkColor(pdc->hdc, 0xffffff);
                l = 1;
            }
            while (1) {
                for (i = 0; i < pdc->cCur; i++) {
                    if (pdc->rgrad[i] > 0) {
                        x = pdc->rgx[i];
                        y = pdc->rgy[i];
                        rad = pdc->rgrad[i];
                        Ellipse(pdc->hdc, x - rad, y - rad, x + rad + 1, y + rad + 1);
                    }
                }
                if (pdc->fHollowOut == 0 || l != 1)
                    break;
                l = 0;
                SetBkColor(pdc->hdc, crSav);
                SetROP2(pdc->hdc, 15);
            }
            pdc->fCovered = 0;
            pdc->cCur = 0;
        }
    }
    return;
}

void DrawShipScanPath(HDC hdc, int16_t fShow) {
    ORDER  *lpord2;
    int16_t rgDup[87];
    int16_t j;
    HPEN    hpenSav;
    POINT16 pt2;
    POINT16 pt;
    int16_t iRopSav;
    int16_t dy;
    ORDER  *lpord1;
    POINT16 ptCur;
    FLEET  *lpfl;
    int16_t i;
    int16_t fHdc;
    int32_t lWarp2;
    int16_t dRad;
    int16_t dx;
    RECT    rc;
    THING  *lpth;
    double  dAngle;
    POINT16 rgptArrow[2];
    int16_t dx5;
    POINT16 ptTick;
    int16_t dy5;
    double  m;
    int16_t id;
    int16_t fDoneRoute;
    HGDIOBJ t_merge_624f_0001;
    HGDIOBJ t_call_624a;

    fHdc = 0;
    if ((sel.grobj == grobjFleet || sel.scan.grobj == grobjFleet || sel.scan.grobj == grobjThing ||
         (sel.grobj == grobjPlanet && ((sel.pl.fStarbase != 0 && sel.pl.idFling != 0) || sel.pl.idRoute != 0))) &&
        (fShow != fOrdersVis && gd.fNoScannerDraw == 0)) {
        fOrdersVis = fShow;
        if (hdc == 0) {
            hdc = GetDC(hwndScanner);
            fHdc = 1;
        }
        if (sel.scan.grobj == grobjThing && sel.scan.ith != -1) {
            lpth = lpThings + sel.scan.ith;
            ptCur = lpth->pt;
            if (lpth->ith == ithMysteryTrader) {
                dx = lpth->tht.ptDest.x - ptCur.x;
                dy = lpth->tht.ptDest.y - ptCur.y;
                lWarp2 = lpth->tht.iWarp;
            } else {
                if (lpth->ith != ithMineralPacket || lpth->thp.iWarp == 0)
                    goto LNextCheck;
                dx = rgptPlan[lpth->thp.idPlanet].x - ptCur.x;
                dy = rgptPlan[lpth->thp.idPlanet].y - ptCur.y;
                lWarp2 = (uint32_t)(lpth->thp.iWarp + 4);
            }
            if (dx != 0 || dy != 0) {
                lWarp2 = (uint32_t)(lWarp2 * (uint32_t)(lWarp2 * 5));
                lpfl = NULL;
                goto LCommonLineCode;
            }
        }
    LNextCheck:
        if (sel.scan.grobj != grobjFleet)
            goto LNoObjPath;
        lpfl = rglpfl[sel.scan.ifl];
        ptCur = lpfl->pt;
        if (lpfl->fdirValid == 0 || lpfl->iwarpFlt <= 0)
            goto LNoObjPath;
        dx = lpfl->dirFltX - 127;
        dy = lpfl->dirFltY - 127;
        if (dx == 0 && dy == 0)
            goto LNoObjPath;
        lWarp2 = (uint32_t)(lpfl->iwarpFlt * lpfl->iwarpFlt * 5);
    LCommonLineCode:
        GetClientRect(hwndScanner, &rc);
        ExcludeClipRect(hdc, rc.left, rc.bottom - dySBar, rc.right, rc.bottom);
        hpenSav = SelectObject(hdc, hpenStarbase);
        iRopSav = SetROP2(hdc, 7);
        if (dx == 0) {
            dx5 = 0;
            dy5 = dy >= 0 ? LOWORD(lWarp2) : -LOWORD(lWarp2);
        } else {
            lWarp2 = (uint32_t)(lWarp2 * lWarp2);
            m = (double)dy / (double)dx;
            dx5 = LOWORD((int32_t)sqrt((double)lWarp2 / (m * m + 1.0)));
            if (dx < 0) {
                dx5 = -dx5;
            }
            dy5 = LOWORD((int32_t)((int32_t)(dx5 * dy) / dx));
        }
        LogicalToScan(&ptCur);
        dx5 = PtToScan(dx5);
        dy5 = -PtToScan(dy5);
        MoveTo(hdc, ptCur.x - dx5, ptCur.y - dy5);
        LineTo(hdc, ptCur.x + dx5, ptCur.y + dy5);
        if (dy == 0) {
            ptTick.x = 0;
            ptTick.y = dx >= 0 ? 4 : -4;
        } else {
            m = (double)(int16_t)-dx / (double)dy;
            ptTick.x = LOWORD((int32_t)sqrt(24.0 / (m * m + 1.0)));
            if (ptTick.x == 0) {
                ptTick.y = dy >= 0 ? 4 : -4;
            } else {
                if (dx > 0) {
                    ptTick.x = -ptTick.x;
                }
                ptTick.y = LOWORD((int32_t)((int32_t)(ptTick.x * dx) / dy));
            }
        }
        dAngle = atan2((double)(int16_t)-dy, (double)(int16_t)-dx) - 0.7853982;
        for (i = 0; i < 2; i++) {
            rgptArrow[i].x = LOWORD((int32_t)(5.0 * cos(dAngle) + 0.5));
            rgptArrow[i].y = LOWORD((int32_t)(5.0 * sin(dAngle) + 0.5));
            dAngle += 1.5707964;
        }
        j = -5;
        for (i = -5; i <= 5; i++) {
            if (i == 0) {
                j = 5;
            } else {
                pt.x = LOWORD((int32_t)(((int32_t)((uint32_t)(dx5 * i) * 2) + j) / 10)) + ptCur.x;
                pt.y = LOWORD((int32_t)(((int32_t)((uint32_t)(dy5 * i) * 2) + j) / 10)) + ptCur.y;
                if (i > 0) {
                    for (j = 0; j < 2; j++) {
                        MoveTo(hdc, pt.x + rgptArrow[j].x, pt.y - rgptArrow[j].y);
                        LineTo(hdc, pt.x, pt.y);
                    }
                } else {
                    MoveTo(hdc, pt.x + ptTick.x, pt.y + ptTick.y);
                    LineTo(hdc, pt.x - ptTick.x, pt.y - ptTick.y);
                    LineTo(hdc, pt.x - ptTick.x, pt.y - ptTick.y - 1);
                }
            }
        }
        SetROP2(hdc, iRopSav);
        SelectObject(hdc, hpenSav);
    LNoObjPath:
        if (sel.grobj == grobjPlanet) {
            fDoneRoute = 0;
            if (sel.pl.fStarbase != 0 && sel.pl.idFling != 0) {
                hpenSav = SelectObject(hdc, hpenDkPurple);
                id = sel.pl.idFling - 1;
            } else {
            L_5daf:
                if (sel.pl.idRoute == 0 || fDoneRoute != 0)
                    goto LFinishUp;
                fDoneRoute = 1;
                id = sel.pl.idRoute - 1;
                hpenSav = SelectObject(hdc, hpenDkGreen);
            }
            iRopSav = SetROP2(hdc, 7);
            GetClientRect(hwndScanner, &rc);
            ExcludeClipRect(hdc, rc.left, rc.bottom - dySBar, rc.right, rc.bottom);
            pt = rgptPlan[sel.pl.id];
            LogicalToScan(&pt);
            MoveTo(hdc, pt.x, pt.y);
            pt = rgptPlan[id];
            LogicalToScan(&pt);
            LineTo(hdc, pt.x, pt.y);
            SetROP2(hdc, iRopSav);
            SelectObject(hdc, hpenSav);
            goto L_5daf;
        }
        if (sel.grobj == grobjFleet && sel.fl.cord > 1) {
            memset(rgDup, 0, sel.fl.cord * 2);
            lpord1 = sel.fl.lpplord->rgord;
            pt = lpord1->pt;
            i = 1;
            while (i < sel.fl.cord) {
                lpord2 = lpord1 + 1;
                pt2 = lpord2->pt;
                if (rgDup[i] == 0) {
                    j = i + 1;
                    while (j < sel.fl.cord) {
                        if ((pt.x == lpord2->pt.x && pt.y == lpord2->pt.y && pt2.x == lpord2[1].pt.x && pt2.y == lpord2[1].pt.y) ||
                            (pt2.x == lpord2->pt.x && pt2.y == lpord2->pt.y && pt.x == lpord2[1].pt.x && pt.y == lpord2[1].pt.y)) {
                            rgDup[i] = 1;
                            rgDup[j] = 2;
                        }
                        j++;
                        lpord2++;
                    }
                }
                pt = pt2;
                i++;
                lpord1++;
            }
            GetClientRect(hwndScanner, &rc);
            ExcludeClipRect(hdc, rc.left, rc.bottom - dySBar, rc.right, rc.bottom);
            if (fShow != 0 && (grbitScan & 0x80) != 0) {
                hpenSav = SelectObject(hdc, hpenStarbase);
                pt = sel.fl.lpplord->rgord[0].pt;
                LogicalToScan(&pt);
                MoveTo(hdc, pt.x, pt.y);
                for (i = 1; i < sel.fl.cord; i++) {
                    pt2 = sel.fl.lpplord->rgord[i].pt;
                    LogicalToScan(&pt2);
                    LineTo(hdc, pt2.x, pt2.y);
                    pt = pt2;
                }
                SelectObject(hdc, hpenSav);
            }
            hpenSav = SelectObject(hdc, hpenShip);
            iRopSav = SetROP2(hdc, 7);
            pt = sel.fl.lpplord->rgord[0].pt;
            dRad = pt.x == sel.pt.x && pt.y == sel.pt.y ? 5 : 5;
            LogicalToScan(&pt);
            ExcludeClipRect(hdc, pt.x - dRad, pt.y - dRad, pt.x + dRad + 1, pt.y + dRad + 1);
            MoveTo(hdc, pt.x, pt.y);
            for (i = 1; i < sel.fl.cord; i++) {
                pt2 = sel.fl.lpplord->rgord[i].pt;
                dRad = pt2.x == sel.pt.x && pt2.y == sel.pt.y ? 5 : 5;
                LogicalToScan(&pt2);
                ExcludeClipRect(hdc, pt2.x - dRad, pt2.y - dRad, pt2.x + dRad + 1, pt2.y + dRad + 1);
                if (rgDup[i] == 2) {
                    MoveTo(hdc, pt2.x, pt2.y);
                } else {
                    if (rgDup[i] == 1) {
                        if ((grbitScan & 0x80) != 0) {
                            t_merge_624f_0001 = hpenYellow;
                        } else {
                            t_call_624a = GetStockObject(WHITE_PEN);
                            t_merge_624f_0001 = t_call_624a;
                        }
                        SelectObject(hdc, t_merge_624f_0001);
                    }
                    LineTo(hdc, pt2.x, pt2.y);
                    if (rgDup[i] == 1) {
                        SelectObject(hdc, hpenShip);
                    }
                }
                pt = pt2;
            }
            SetROP2(hdc, iRopSav);
            SelectObject(hdc, hpenSav);
            SelectClipRgn(hdc, hrgnHuge);
        }
    LFinishUp:
        if (fHdc != 0) {
            ReleaseDC(hwndScanner, hdc);
        }
    }
    return;
}

void DrawScannerSBar(HDC hdc, RECT *prc, SBAR *psbar, int16_t fFullRedraw) {
    int16_t    fhdc;
    COLORREF   crText;
    POINT16    pt2;
    int16_t    id;
    POINT16    pt;
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

    fhdc = 0;
    fDoName = 1;
    GetClientRect(hwndScanner, &rc);
    rc.top = rc.bottom - dySBar;
    if (prc == 0 || IntersectRect(&rcT, prc, &rc) != 0) {
        if (hdc == 0) {
            fhdc = 1;
            hdc = GetDC(hwndScanner);
        }
        hfontSav = SelectObject(hdc, rghfontArial8[1]);
        if (psbar != 0) {
            grobj = psbar->grbit;
        } else {
            grobj = sel.scan.grobj;
        }
        iBkPrev = SetBkMode(hdc, TRANSPARENT);
        crBk = SetBkColor(hdc, crButtonFace);
        crText = SetTextColor(hdc, crButtonText);
        hbrSav = SelectObject(hdc, hbrButtonFace);
        if (fFullRedraw != 0) {
            PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, dySBar, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, rc.left, rc.top, 1, dySBar, PATCOPY);
            PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, 1, PATCOPY);
        }
        rc.bottom -= dySBar >> 1;
        rcT = rc;
        rcT.top += 4;
        rcT.bottom -= 4;
        rcT.left += 4;
        if (rc.right >= 360) {
            l = GetTextExtent(hdc, "ID #000", 7);
            dxHole = LOWORD(l) + 6;
            rcT.right = LOWORD(l) + 6 + rcT.left;
            DrawLockLight(hdc, &rcT, fFullRedraw);
            if ((grobj & 1) != 0) {
                if (psbar != 0) {
                    id = psbar->id;
                } else {
                    id = sel.scan.idpl;
                }
                if (id != -1) {
                    c = _wsprintf(szWork, "ID #%d", id + 1);
                    SetBkMode(hdc, OPAQUE);
                    TextOut(hdc, rcT.left + 3, rcT.top + 2, szWork, c);
                }
            } else if (grobj == grobjOther) {
                if (psbar != 0) {
                    id = psbar->id;
                } else {
                    id = sel.scan.iwp;
                }
                c = _wsprintf(szWork, "WP #%d", id);
                TextOut(hdc, rcT.left + 3, rcT.top + 2, szWork, c);
            }
            l = GetTextExtent(hdc, "X: 8888", 7);
            rcT.left += dxHole + 4;
            dxHole = LOWORD(l) + 6;
            rcT.right = LOWORD(l) + 6 + rcT.left;
            DrawLockLight(hdc, &rcT, fFullRedraw);
            if (psbar != 0) {
                pt = psbar->pt;
            } else if (grobj != grobjNone) {
                pt = sel.scan.pt;
            } else {
                pt.y = 0;
                pt.x = 0;
            }
            if (pt.x > 0) {
                c = _wsprintf(szWork, "X: %d", pt.x);
                SetBkMode(hdc, OPAQUE);
                TextOut(hdc, rcT.left + 3, rcT.top + 2, szWork, c);
            }
            rcT.left += dxHole + 4;
            dxHole = LOWORD(l) + 6;
            rcT.right = LOWORD(l) + 6 + rcT.left;
            DrawLockLight(hdc, &rcT, fFullRedraw);
            if (pt.y > 0) {
                c = _wsprintf(szWork, "Y: %d", pt.y);
                SetBkMode(hdc, OPAQUE);
                TextOut(hdc, rcT.left + 3, rcT.top + 2, szWork, c);
            }
            rcT.left += dxHole + 4;
        }
        dxHole = rc.right - rcT.left - 4;
        rcT.right = rc.right - rcT.left - 4 + rcT.left;
        DrawLockLight(hdc, &rcT, fFullRedraw);
        rcClip = rcT;
        ExpandRc(&rcClip, -2, -2);
        grReal = grobj == grobjOther ? sel.scan.grobjFull : grobj;
        if (psbar != 0 && psbar->psz != 0) {
            strcpy(szWork, psbar->psz);
        } else if ((grReal & 1) != 0) {
            PszGetPlanetName(psbar == 0 ? sel.scan.idpl : psbar->id);
        } else if ((grReal & 2) != 0) {
            PszGetFleetName(psbar == 0 ? rglpfl[sel.scan.ifl]->id : psbar->id);
        } else if ((grReal & 8) != 0) {
            PszGetThingName(psbar == 0 ? lpThings[sel.scan.ith].idFull : psbar->id);
        } else if (grReal == 0 && grobj == grobjOther) {
            CchGetString(idsDeepSpaceWaypoint, szWork);
        } else {
            fDoName = 0;
        }
        if (fDoName != 0) {
            ExtTextOut(hdc, rcT.left + 3, rcT.top + 2, 4, &rcClip, szWork, lstrlen(szWork), NULL);
        }
        OffsetRc(&rc, 0, dySBar >> 1);
        rcT = rc;
        rcT.top += 4;
        rcT.bottom -= 4;
        rcT.left += 4;
        rcT.right -= 4;
        DrawLockLight(hdc, &rcT, fFullRedraw);
        rcClip = rcT;
        ExpandRc(&rcClip, -2, -2);
        if (psbar != 0) {
            pt = psbar->pt;
        } else if (grobj != grobjNone) {
            pt = sel.scan.pt;
        } else {
            pt.y = 0;
            pt.x = 0;
        }
        if (psbar != 0 && psbar->pscan != 0) {
            pt2 = psbar->pscan->pt;
        } else if (sel.grobj == grobjFleet || sel.grobj == grobjPlanet) {
            pt2 = sel.pt;
        } else {
            pt2.x = -1;
        }
        if (pt.x != -1 && pt2.x != -1 && (pt.x != pt2.x || pt.y != pt2.y)) {
            strcpy(szBuf, PszGetDistance(pt.x, pt.y, pt2.x, pt2.y));
            for (psz = szBuf; (int16_t)(int8_t)*psz != ' '; psz++) {
            }
            CchGetString((rc.right < 350 ? 0 : 1) + 1366, psz + 1);
            if (psbar == 0 || psbar->pscan == 0) {
                CchGetString(idsFrom, psz + strlen(psz));
                strcat(psz + 8, PszGetLocName(sel.grobj, sel.id, pt2.x, pt2.y));
            }
            ExtTextOut(hdc, rcT.left + 3, rcT.top + 2, 4, &rcClip, szBuf, strlen(szBuf), NULL);
        }
        SelectObject(hdc, hbrSav);
        SetBkMode(hdc, iBkPrev);
        SetTextColor(hdc, crButtonText);
        SetBkColor(hdc, crBk);
        SelectObject(hdc, hfontSav);
        if (fhdc != 0) {
            ReleaseDC(hwndScanner, hdc);
        }
    }
    return;
}

void DrawLockLight(HDC hdc, RECT *prc, int16_t fFullRedraw) {
    int16_t dy;
    int16_t dx;
    RECT    rc;

    dx = prc->right - prc->left;
    dy = prc->bottom - prc->top;
    if (fFullRedraw != 0) {
        SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, prc->left, prc->top, 1, dy, PATCOPY);
        PatBlt(hdc, prc->left, prc->top, dx, 1, PATCOPY);
        SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, prc->right, prc->top, 1, dy + 1, PATCOPY);
        PatBlt(hdc, prc->left, prc->bottom, dx, 1, PATCOPY);
    } else {
        rc = *prc;
        ExpandRc(&rc, -2, -2);
        FillRect(hdc, &rc, hbrButtonFace);
    }
    return;
}

void SetScanScrollBars(HWND hwnd) {
    int16_t xMax;
    int16_t dy;
    int16_t yMax;
    int16_t dx;
    RECT    rc;

    fInScrollSet = 1;
    GetClientRect(hwnd, &rc);
    dx = ScanToPt(rc.right);
    dy = ScanToPt(rc.bottom - dySBar);
    xMax = (1000 <= dGalInv - 1000 - dx ? dGalInv - 0x3e8 - dx : 0x3e8) + 3 & 0xfffc;
    yMax = (1000 <= dGalInv - 1000 - dy ? dGalInv - 0x3e8 - dy : 0x3e8) + 3 & 0xfffc;
    SetScrollRange(hwnd, 0, 1000, xMax, 1);
    if (fInScrollSet != 0) {
        SetScrollRange(hwnd, 1, 1000, yMax, 1);
        if (fInScrollSet != 0) {
            dScanPage = (int16_t)(dx >= dy ? dy : dx) / 3 & 0xfffc;
            dScanInc = dScanPage / 8 + 2 & 0xfffc;
            fInScrollSet = 0;
        }
    }
    return;
}

void ScrollScanner(int16_t dx, int16_t dy) {
    HDC  hdc;
    RECT rcUpd;
    RECT rcUpd2;
    RECT rc;

    if ((dx != 0 || dy != 0) && IsWindowVisible(hwndScanner) != 0 && gd.fNoScannerDraw == 0) {
        hdc = GetDC(hwndScanner);
        GetClientRect(hwndScanner, &rc);
        if (abs(dx) > rc.right >> 1 || abs(dy) > rc.bottom >> 1 || fDlgUp != 0 || hwndBrowser != 0) {
            DrawScanner(hdc, &rc);
        } else {
            rc.bottom -= dySBar;
            UpdateWindow(hwndScanner);
            ScrollWindow(hwndScanner, dx, dy, &rc, &rc);
            if (dy > 0) {
                SetRect(&rcUpd2, 0, 0, rc.right, dy);
                rc.top += dy;
            } else if (dy < 0) {
                SetRect(&rcUpd2, 0, rc.bottom + dy, rc.right, rc.bottom);
                rc.bottom += dy;
            }
            if (dx > 0) {
                SetRect(&rcUpd, 0, rc.top, dx, rc.bottom);
                rc.right -= dx;
            } else if (dx < 0) {
                rcUpd = rc;
                rcUpd.left = rc.right + dx;
                rc.left -= dx;
            }
            if (dx != 0) {
                ValidateRect(hwndScanner, &rcUpd);
            }
            if (dy != 0) {
                ValidateRect(hwndScanner, &rcUpd2);
            }
            if (dx != 0) {
                DrawScanner(hdc, &rcUpd);
            }
            if (dy != 0) {
                DrawScanner(hdc, &rcUpd2);
            }
            UpdateWindow(hwndScanner);
        }
        ReleaseDC(hwndScanner, hdc);
    }
    return;
}

void RedrawScanSel(HDC hdc, int16_t fVis) {
    int16_t sel_grobj;
    int16_t fhdc;
    int16_t dOff;
    POINT16 pt;
    int16_t sel_id;
    int16_t fNoSelRedraw;
    SCAN    sel_scan;
    RECT    rc;
    int16_t sel_grobjFull;

    fhdc = 0;
    sel_id = sel.id;
    sel_grobj = sel.grobj;
    sel_grobjFull = sel.grobjFull;
    sel_scan = sel.scan;
    if (hwndScanner != 0 && IsWindowVisible(hwndScanner) != 0) {
        if (fVis == -1) {
            fVis = 0;
            fNoSelRedraw = 1;
        } else {
            fNoSelRedraw = 0;
        }
        if (hdc == 0) {
            fhdc = 1;
            hdc = GetDC(hwndScanner);
        }
        DrawShipScanPath(hdc, fVis);
        if (fVis == 0) {
            sel.scan.iwp = -1;
            sel.scan.ifl = -1;
            sel.scan.idpl = -1;
            sel.id = -1;
            sel.grobjFull = grobjNone;
            sel.grobj = grobjNone;
            sel.scan.grobjFull = grobjNone;
            sel.scan.grobj = grobjNone;
        }
        dOff = iScanZoom >= 3 && (grbitScan & 0xf) == 4 ? 11 : (grbitScan & 0x1000) != 0 ? 7 : 0;
        if (sel_grobj != 0 && fNoSelRedraw == 0) {
            pt = sel.pt;
            LogicalToScan(&pt);
            SetRect(&rc, pt.x - 11 - dOff, pt.y - 11 - dOff, pt.x + 12 + dOff, pt.y + 23 + dOff);
            DrawScanner(hdc, &rc);
        }
        if (sel_scan.grobj != grobjNone && (sel_scan.pt.x != sel.pt.x || sel_scan.pt.y != sel.pt.y)) {
            pt = sel_scan.pt;
            LogicalToScan(&pt);
            SetRect(&rc, pt.x - 6 - dOff, pt.y - 6 - dOff, pt.x + 7 + dOff, pt.y + 15 + dOff);
            DrawScanner(hdc, &rc);
        }
        if (fVis == 0) {
            sel.id = sel_id;
            sel.grobj = sel_grobj;
            sel.grobjFull = sel_grobjFull;
            sel.scan = sel_scan;
        }
        if (fhdc != 0) {
            ReleaseDC(hwndScanner, hdc);
        }
    }
    return;
}

int16_t FEnsurePointOnScreen(POINT16 pt, int16_t fScroll) {
    int16_t cy;
    int16_t fFix;
    int16_t cx;
    POINT16 ptCtr;
    RECT    rc;

    fFix = 0;
    GetClientRect(hwndScanner, &rc);
    rc.bottom -= dySBar;
    cx = ScanToPt(rc.right);
    cy = ScanToPt(rc.bottom);
    rc.left = xScanTop + 10;
    rc.right = xScanTop + cx - 20;
    rc.bottom = dGalInv - yScanTop - 10;
    rc.top = dGalInv - yScanTop - 10 - cy + 20;
    if (PtInRect(&rc, PointFrom16(pt)) != 0) {
        return 1;
    }
    ptCtr.x = (cx >> 1) + xScanTop;
    ptCtr.y = dGalInv - yScanTop - (cy >> 1);
    if (pt.x < rc.left) {
        ptCtr.x -= rc.left - pt.x;
    } else if (pt.x > rc.right) {
        ptCtr.x += pt.x - rc.right;
    }
    if (pt.y < rc.top) {
        ptCtr.y -= rc.top - pt.y;
    } else if (pt.y > rc.bottom) {
        ptCtr.y += pt.y - rc.bottom;
    }
    CtrPointScan(ptCtr, fScroll);
    return 0;
}

void CtrPointScan(POINT16 pt, int16_t fScroll) {
    int16_t dxCur;
    int16_t cy;
    int16_t y;
    int16_t cx;
    int16_t dyCur;
    int16_t x;
    RECT    rc;

    x = pt.x;
    y = pt.y;
    if (hwndScanner != 0) {
        GetClientRect(hwndScanner, &rc);
        rc.bottom -= dySBar;
        cx = ScanToPt(rc.right);
        cy = ScanToPt(rc.bottom);
        x = x - (cx >> 1) <= 1000 ? 1000 : x - (cx >> 1);
        y = (cy >> 1) + y >= dGalInv - 1000 ? dGalInv - 1000 : (cy >> 1) + y;
        x = x >= dGalInv - 1000 - cx ? dGalInv - 1000 - cx : x;
        y = dGalInv - (y <= cy + 1000 ? cy + 1000 : y);
        x = x <= 1000 ? 1000 : x;
        y = y <= 1000 ? 1000 : y;
        dxCur = xScanTop;
        dyCur = yScanTop;
        x = x + 2 & 0xfffc;
        y = y + 2 & 0xfffc;
        if (dxCur != x || dyCur != y) {
            xScanTop = x;
            SetScrollPos(hwndScanner, 0, x, 1);
            yScanTop = y;
            SetScrollPos(hwndScanner, 1, y, 1);
            if (fScroll != 0) {
                ScrollScanner(PtToScan(dxCur - x), PtToScan(dyCur - y));
            } else {
                InvalidateRect(hwndScanner, NULL, 0);
            }
        }
    }
    return;
}

void LogicalToScan(POINT16 *ppt) {
    ppt->x = PtToScan(ppt->x - xScanTop);
    ppt->y = PtToScan(dGalInv - ppt->y - yScanTop);
    return;
}

void ScanToLogical(POINT16 *ppt) {
    ppt->x = ScanToPt(ppt->x) + xScanTop;
    ppt->y = dGalInv - (ScanToPt(ppt->y) + yScanTop);
    if (ppt->x > dGal + 1000) {
        ppt->x = dGal + 1000;
    }
    if (ppt->y < 1000) {
        ppt->y = 1000;
    }
    return;
}

int16_t FAddWayPoint(POINT16 ptIn, SCAN *pscan) {
    HDC      hdc;
    int16_t  id;
    int16_t  dy;
    ORDER   *lpord;
    int16_t  lDist;
    POINT16  rgpt[3];
    int16_t  dx;
    int16_t  cpt;
    int16_t  ipt;
    RECT     rc;
    uint16_t t_scratch_m20_2;

    if (sel.fl.cord == 87) {
        MessageBeep(64);
        _wsprintf(szWork, PszGetCompressedString(idsCantHaveDWaypoints), 86);
        AlertSz(szWork, MB_ICONHAND);
        return 0;
    }
    if ((grbitScan & 0x80) != 0) {
        rgpt[0] = ptIn;
        rgpt[1] = pscan->pt;
        if (sel.iwpAct < sel.fl.cord - 1) {
            cpt = 3;
            rgpt[2] = sel.fl.lpplord->rgord[sel.iwpAct + 1].pt;
        } else {
            cpt = 2;
        }
    }
    dx = ptIn.x - pscan->pt.x;
    dy = ptIn.y - pscan->pt.y;
    lDist = ScanToPt(20);
    if ((int32_t)((uint32_t)(dx * dx) + (uint32_t)(dy * dy)) > (int16_t)(lDist * lDist)) {
        pscan->grobjFull = grobjOther;
        pscan->grobj = grobjOther;
        pscan->idpl = -1;
        pscan->ifl = -1;
        pscan->iwp = sel.iwpAct + 1;
        pscan->pt = ptIn;
    }
    lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
    if (pscan->pt.x == lpord->pt.x && pscan->pt.y == lpord->pt.y) {
        return 0;
    }
    if (sel.iwpAct < sel.fl.cord - 1 && pscan->pt.x == lpord[1].pt.x && pscan->pt.y == lpord[1].pt.y) {
        return 0;
    }
    hdc = GetDC(hwndScanner);
    DrawShipScanPath(hdc, 0);
    if (sel.fl.lpplord->iordMax == sel.fl.cord) {
        sel.fl.lpplord = (PLORD *)LpplReAlloc((PL *)sel.fl.lpplord, sel.fl.cord + 3);
        lpord = &sel.fl.lpplord->rgord[sel.iwpAct + 1];
    } else {
        lpord++;
    }
    if (sel.iwpAct != sel.fl.cord - 1) {
        fmemmove(lpord + 1, lpord, (sel.fl.cord - sel.iwpAct - 1) * sizeof(ORDER));
    }
    *lpord = *(lpord - 1);
    lpord->pt = pscan->pt;
    switch (pscan->grobj) {
    case grobjPlanet:
        id = pscan->idpl;
        break;
    case grobjOther:
        id = pscan->iwp;
        break;
    case grobjFleet:
        id = rglpfl[pscan->ifl]->id;
        break;
    case grobjThing:
        id = lpThings[pscan->ith].idFull;
    }
    lpord->id = id;
    lpord->grobj = pscan->grobj;
    sel.fl.cord++;
    sel.fl.lpplord->iordMac++;
    t_scratch_m20_2 = IWarpBestForWaypoint(&sel.fl, lpord);
    lpord->iWarp = t_scratch_m20_2;
    pscan->grobj = grobjOther;
    pscan->grobjFull |= 4;
    pscan->iwp = sel.iwpAct + 1;
    RedrawScanSel(NULL, 0);
    FLookupFleet(-1, &sel.fl);
    if (lpord[-1].grTask == grTaskLayMines && lpord[-1].tsell.iPlrX == 5) {
        fmemset((uint8_t *)lpord - 10, 0, 10);
        lpord[-1].grTask = grTaskNone;
        FLookupFleet(-1, &sel.fl);
    }
    ChangeScanSel(pscan, 1);
    ReleaseDC(hwndScanner, hdc);
    if ((grbitScan & 0x80) != 0) {
        for (ipt = 0; ipt < cpt; ipt++) {
            LogicalToScan(&rgpt[ipt]);
        }
        BoundPoints(&rc, rgpt, cpt);
        InvalidateRect(hwndScanner, &rc, 0);
    }
    return 1;
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
    int32_t t_call_7d2b;

    iWarpSav = lpord->iWarp;
    iWarp = IFindIdealWarp(NULL, 0);
    if (fAi != 0) {
        iWarpAi = IFindIdealWarp(NULL, 1);
    }
    for (iwp = lpfl->cord - 1; iwp >= 0 && lpord != &lpfl->lpplord->rgord[iwp]; iwp--) {
    }
    if (iwp <= 0) {
        return iWarp;
    }
    if (lpord->grTask == grTaskColonize || lpord->grTask == grTaskScrap) {
        fGoFlatOut = 1;
    } else {
        fGoFlatOut = 0;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                for (j = 0; j < rglpshdef[lpfl->iPlayer][i].hul.chs; j++) {
                    if (rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst == hstSpecialM &&
                        (rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem == 0 || rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem == 1)) {
                        fGoFlatOut = 1;
                        break;
                    }
                }
            }
        }
    }
    if (fGoFlatOut == 0 && fAi != 0) {
        fGoFlatOutAi = 1;
        fGoFlatOut = 1;
    } else {
        fGoFlatOutAi = 0;
    }
    if (iWarp < 9) {
        iWarpOld = iWarp;
        if (FFindNearestObject(lpord->pt, 0x81, &scan) != 0) {
            lppl = LpplFromId(scan.idpl);
        } else {
            lppl = NULL;
        }
        if (fGoFlatOut == 0 && (lppl == 0 || (lppl->iPlayer != -1 && lppl->iPlayer != idPlayer))) {
            if (iwp > 1 && lpord[-1].iWarp > (uint16_t)iWarp && lpord[-1].iWarp <= 10) {
                iWarp = lpord[-1].iWarp;
            }
            t_call_7d2b = LFuelUseToWaypoint(lpfl, iwp, 1);
            if ((int32_t)t_call_7d2b >= (int32_t)(LGetFleetStat(lpfl, 1) / 10) || lpfl->rgwtMin[4] < (int32_t)(LGetFleetStat(lpfl, 1) * 7) / 10)
                goto LOptimizeSpeed;
            iWarp++;
        } else {
            if (fGoFlatOutAi != 0 && lppl != 0 && lppl->iPlayer == idPlayer && rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef != ihuldefOrbitalFort) {
                fGoFlatOutAi = 0;
            }
            iWarp = 9;
        }
        for (; iWarp > iWarpOld; iWarp--) {
            lpord->iWarp = iWarp;
            lFuel = LFuelUseToWaypoint(lpfl, iwp, 1);
            if (lFuel <= lpfl->rgwtMin[4] && ((lppl != 0 && lppl->fStarbase != 0 && lppl->iPlayer == idPlayer &&
                                               LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0) ||
                                              lFuel <= (int32_t)(lpfl->rgwtMin[4] / 2) || fGoFlatOut != 0))
                break;
        }
        lpord->iWarp = iWarpSav;
    }
    if (fGoFlatOutAi != 0 && iWarp > iWarpAi) {
        iWarp = iWarpAi;
    }
LOptimizeSpeed:
    if (iWarp > 1 && lpord->grobj != grobjFleet) {
        lDist = LOWORD((int32_t)DGetDistance(lpord->pt.x, lpord->pt.y, lpord[-1].pt.x, lpord[-1].pt.y));
        cSpeed = iWarp * iWarp;
        cTravel = (int16_t)(iWarp * iWarp + lDist - 1) / cSpeed;
        do {
            iWarp--;
            if (iWarp <= 1)
                break;
            cSpeed = iWarp * iWarp;
        } while (cTravel == (int16_t)(lDist + cSpeed - 1) / cSpeed);
        iWarp++;
    } else {
        cTravel = 2;
    }
    if (FCanFleetUseStargates(lpfl, lpord[-1].pt, lpord->pt) == 1) {
        iWarp = 11;
    }
    if (iWarp > 11) {
        iWarp = 9;
    }
    return iWarp;
}

int16_t FNearAWayPoint(POINT16 pt, int16_t fLogical) {
    ORDER  *lpord;
    int16_t i;
    SCAN    scan;

    if (sel.grobj != grobjFleet) {
        return 0;
    }
    if (fLogical == 0) {
        ScanToLogical(&pt);
    }
    if (FFindNearestObject(pt, 0x4f, &scan) == 0) {
        return 0;
    }
    if ((scan.grobjFull & 4) != 0) {
        if (scan.pt.x == sel.pt.x && scan.pt.y == sel.pt.y) {
            lpord = &sel.fl.lpplord->rgord[1];
            i = 1;
            for (; i < sel.fl.cord && (lpord->pt.x != scan.pt.x || lpord->pt.y != scan.pt.y); lpord++) {
                i++;
            }
            if (i != sel.fl.cord) {
                return 1;
            }
            return 0;
        }
        return 1;
    }
    return 0;
}

int16_t FHandleWayPointDrag(POINT16 pt) {
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
    POINT16  ptLogical;
    POINT16  ptNext;
    int16_t  fDel;
    POINT16  rgpt[4];
    POINT16  ptNew;
    int16_t  cpt;
    POINT16  ptPrev;
    SCAN     scan;
    int16_t  fFirst;
    RECT     rc;
    int16_t  t_merge_8383_0001;
    int16_t  t_merge_83c2_0001;
    uint16_t t_scratch_m8a_2;

    fFirst = 1;
    fMarker = 0;
    LogicalToScan(&pt);
    if (sel.iwpAct == 0) {
        lpord = &sel.fl.lpplord->rgord[1];
        i = 1;
        for (; i < sel.fl.cord && (sel.fl.pt.x != lpord->pt.x || sel.fl.pt.y != lpord->pt.y); lpord++) {
            i++;
        }
        SetScanWp(i);
    }
    rgpt[2] = sel.fl.lpplord->rgord[sel.iwpAct - 1].pt;
    ptPrev.x = rgpt[2].x;
    ptPrev.y = rgpt[2].y;
    if (sel.iwpAct == sel.fl.cord - 1) {
        cpt = 3;
    } else {
        cpt = 4;
        rgpt[3] = sel.fl.lpplord->rgord[sel.iwpAct + 1].pt;
        ptNext.x = rgpt[3].x;
        ptNext.y = rgpt[3].y;
    }
    rgpt[0] = sel.fl.lpplord->rgord[sel.iwpAct].pt;
    rgpt[1] = rgpt[0];
    for (i = 0; i < cpt; i++) {
        LogicalToScan(&rgpt[i]);
    }
    GetClientRect(hwndScanner, &rc);
    rc.bottom -= dySBar;
    hdc = GetDC(hwndScanner);
    hcurSav = SetCursor(hcurCloseGrab);
    SetCapture(hwndScanner);
    ptNew = pt;
    while (FGetMouseMove(&ptNew) != 0) {
        t_merge_8383_0001 = 0 > (rc.right >= ptNew.x ? ptNew.x : rc.right) ? 0 : rc.right < ptNew.x ? rc.right : ptNew.x;
        ptNew.x = t_merge_8383_0001;
        t_merge_83c2_0001 = 0 > (rc.bottom >= ptNew.y ? ptNew.y : rc.bottom) ? 0 : rc.bottom < ptNew.y ? rc.bottom : ptNew.y;
        ptNew.y = t_merge_83c2_0001;
        if (pt.x != ptNew.x || pt.y != ptNew.y) {
            if (fFirst == 0 || FNearAWayPoint(ptNew, 0) == 0) {
                fFirst = 0;
                ptLogical = ptNew;
                ScanToLogical(&ptLogical);
                grTypeIn = (GetAsyncKeyState(16) & 0xfffe) == 0 ? 79 : 143;
                FFindNearestObject(ptLogical, grTypeIn, &scan);
                DrawScanXorLines(hdc, rgpt, cpt);
                if (scan.grobj == grobjNone) {
                    rgpt[0] = ptLogical;
                    sbar.id = -1;
                    CchGetString(idsDeepSpace, szDeepSpace);
                    sbar.psz = szDeepSpace;
                } else {
                    rgpt[0] = scan.pt;
                    switch (scan.grobj) {
                    case grobjPlanet:
                        sbar.id = scan.idpl;
                        break;
                    case grobjFleet:
                        sbar.id = rglpfl[scan.ifl]->id;
                        break;
                    case grobjThing:
                        sbar.id = lpThings[scan.ith].idFull;
                        break;
                    default:
                        sbar.id = scan.iwp;
                    }
                    sbar.psz = 0;
                }
                sbar.pt.x = rgpt[0].x;
                sbar.pt.y = rgpt[0].y;
                sbar.grbit = scan.grobj;
                LogicalToScan(rgpt);
                DrawScanXorLines(hdc, rgpt, cpt);
                sbar.pscan = 0;
                DrawScannerSBar(hdc, NULL, &sbar, 0);
            }
            pt = ptNew;
        }
    }
    fChg = rgpt[0].x != rgpt[1].x || rgpt[0].y != rgpt[1].y;
    if (fChg != 0) {
        if (scan.grobj == grobjNone) {
            scan.pt = ptLogical;
            scan.grobj = grobjOther;
            scan.iwp = sel.iwpAct;
        }
        fDup = scan.pt.x == ptPrev.x && scan.pt.y == ptPrev.y;
        if (fDup == 0 && sel.iwpAct < sel.fl.cord - 1) {
            fDup = (scan.pt.x == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.x && scan.pt.y == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.y) * 2;
        }
        GetClientRect(hwndScanner, &rc);
        if (fDup != 0) {
            fDel = AlertSz(PszFormatIds(idsSureWantDeleteCurrentWaypoint, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES ? 1 : 0;
            if ((grbitScan & 0x80) != 0) {
                hpenSav = SelectObject(hdc, hpenStarbase);
                MoveTo(hdc, rgpt[2].x, rgpt[2].y);
                LineTo(hdc, rgpt[0].x, rgpt[0].y);
                if (cpt > 3) {
                    ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
                    LineTo(hdc, rgpt[3].x, rgpt[3].y);
                }
                SelectObject(hdc, hpenSav);
            }
            DrawScanXorLines(hdc, rgpt, cpt);
            rgpt[0] = rgpt[1];
            if ((grbitScan & 0x80) != 0) {
                hpenSav = SelectObject(hdc, hpenStarbase);
                MoveTo(hdc, rgpt[2].x, rgpt[2].y);
                LineTo(hdc, rgpt[0].x, rgpt[0].y);
                if (cpt > 3) {
                    ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
                    LineTo(hdc, rgpt[3].x, rgpt[3].y);
                }
                SelectObject(hdc, hpenSav);
            }
            DrawScanXorLines(hdc, rgpt, cpt);
            if (fDel == 0)
                goto Done;
            DeleteCurWayPoint(fDup == 1 ? 1 : 0);
            goto Done;
        }
        if ((grbitScan & 0x80) != 0) {
            ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
            hpenSav = SelectObject(hdc, hpenStarbase);
            MoveTo(hdc, rgpt[2].x, rgpt[2].y);
            LineTo(hdc, rgpt[0].x, rgpt[0].y);
            if (cpt > 3) {
                LineTo(hdc, rgpt[3].x, rgpt[3].y);
            }
            SelectObject(hdc, hpenSav);
        }
        DrawScanXorLines(hdc, rgpt, cpt);
        rgpt[0] = rgpt[1];
        if ((grbitScan & 0x80) != 0) {
            ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
            hpenSav = SelectObject(hdc, hpenStarbase);
            MoveTo(hdc, rgpt[2].x, rgpt[2].y);
            LineTo(hdc, rgpt[0].x, rgpt[0].y);
            if (cpt > 3) {
                LineTo(hdc, rgpt[3].x, rgpt[3].y);
            }
            SelectObject(hdc, hpenSav);
        }
        DrawScanXorLines(hdc, rgpt, cpt);
        RedrawScanSel(NULL, 0);
        switch (scan.grobj) {
        case grobjPlanet:
            i = scan.idpl;
            break;
        case grobjFleet:
            i = rglpfl[scan.ifl]->id;
            break;
        case grobjThing:
            i = lpThings[scan.ith].idFull;
            break;
        default:
            i = scan.iwp;
        }
        lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
        lpord->grobj = scan.grobj;
        lpord->id = i;
        lpord->pt = scan.pt;
        t_scratch_m8a_2 = IWarpBestForWaypoint(&sel.fl, lpord);
        lpord->iWarp = t_scratch_m8a_2;
        FLookupFleet(-1, &sel.fl);
        scan.iwp = sel.iwpAct;
        scan.grobjFull |= 4;
        sel.iwpAct = -2;
        ChangeScanSel(&scan, 1);
    }
    DrawScannerSBar(hdc, NULL, NULL, 0);
    ReleaseCapture();
    SetCursor(hcurSav);
    InvalidateRect(hwndMine, NULL, 1);
    SetMineralTitleBar(hwndMine);
Done:
    ReleaseDC(hwndScanner, hdc);
    if (fChg != 0 && (grbitScan & 0x80) != 0) {
        rgpt[1] = ptNew;
        BoundPoints(&rc, rgpt, cpt);
        hdc = GetDC(hwndScanner);
        DrawScanner(hdc, &rc);
        ReleaseDC(hwndScanner, hdc);
    }
    return fChg;
}

void DrawScanXorLines(HDC hdc, POINT16 *rgpt, int16_t cpt) {
    HPEN    hpenSav;
    int16_t iRopSav;
    int16_t i;
    RECT    rc;

    GetClientRect(hwndScanner, &rc);
    ExcludeClipRect(hdc, 0, rc.bottom - dySBar, rc.right, rc.bottom);
    if (cpt == 4 && rgpt[2].x == rgpt[3].x && rgpt[2].y == rgpt[3].y) {
        cpt--;
        hpenSav = GetStockObject(WHITE_PEN);
    } else {
        hpenSav = hpenShip;
    }
    for (i = 1; i < cpt; i++) {
        ExcludeClipRect(hdc, rgpt[i].x - 5, rgpt[i].y - 5, rgpt[i].x + 6, rgpt[i].y + 6);
    }
    hpenSav = SelectObject(hdc, hpenSav);
    iRopSav = SetROP2(hdc, 7);
    MoveTo(hdc, rgpt[2].x, rgpt[2].y);
    LineTo(hdc, rgpt->x, rgpt->y);
    if (cpt > 3) {
        LineTo(hdc, rgpt[3].x, rgpt[3].y);
    }
    SetROP2(hdc, iRopSav);
    SelectObject(hdc, hpenSav);
    SelectClipRgn(hdc, hrgnHuge);
    return;
}

int16_t SetScanWp(int16_t iNew) {
    SCAN scan;

    if (iNew == sel.iwpAct) {
        return iNew;
    }
    FFindNearestObject(sel.fl.lpplord->rgord[iNew].pt, grobjOther, &scan);
    scan.iwp = iNew;
    ChangeScanSel(&scan, 1);
    return iNew;
}

void ChangeScanSel(SCAN *pscan, int16_t fValidScan) {
    int16_t fMineFieldSel;
    RECT    rcMine;
    int16_t fChgWp;
    int16_t iRad;
    HDC     hdc;
    POINT16 t_pt_8dfc;
    POINT16 t_pt_8e08;
    POINT16 t_pt_9001;
    POINT16 t_pt_900d;

    if (fValidScan == 0) {
        FFindNearestObject(pscan->pt, pscan->grobj, pscan);
    }
    if (memcmp(pscan, &sel.scan, sizeof(SCAN)) != 0) {
        fChgWp = pscan->iwp != -1 && pscan->iwp != sel.iwpAct;
        fMineFieldSel = sel.scan.grobj == grobjThing && lpThings[sel.scan.ith].ith == ithMinefield;
        if (fMineFieldSel != 0) {
            iRad = LOWORD((int32_t)(sqrt((double)lpThings[sel.scan.ith].thm.cMines) + 1.0));
            rcMine.left = lpThings[sel.scan.ith].pt.x;
            rcMine.top = lpThings[sel.scan.ith].pt.y;
            rcMine.right = rcMine.left + iRad;
            rcMine.bottom = rcMine.top - iRad;
            rcMine.left -= iRad;
            rcMine.top += iRad;
            t_pt_8dfc.x = rcMine.left;
            t_pt_8dfc.y = rcMine.top;
            LogicalToScan(&t_pt_8dfc);
            rcMine.left = t_pt_8dfc.x;
            rcMine.top = t_pt_8dfc.y;
            t_pt_8e08.x = rcMine.right;
            t_pt_8e08.y = rcMine.bottom;
            LogicalToScan(&t_pt_8e08);
            rcMine.right = t_pt_8e08.x;
            rcMine.bottom = t_pt_8e08.y;
            InflateRect(&rcMine, 1, 1);
        }
        RedrawScanSel(NULL, -1);
        sel.scan = *pscan;
        if ((sel.scan.grobjFull & 1) != 0 && fValidScan != 2) {
            sel.scan.grobj = grobjPlanet;
        }
        if (fChgWp != 0) {
            sel.iwpAct = pscan->iwp;
            FillOrdersLB();
            SetOrdersLbSel(pscan->iwp);
            UpdateOrdersDDs(0);
            DrawPlanShip(NULL, 290);
        }
        RedrawScanSel(NULL, 1);
        if (fChgWp != 0) {
            FEnsurePointOnScreen(pscan->pt, 1);
        }
        DrawScannerSBar(NULL, NULL, NULL, 0);
        InvalidateRect(hwndMine, NULL, 1);
        SetMineralTitleBar(hwndMine);
        if (fMineFieldSel != 0) {
            hdc = GetDC(hwndScanner);
            DrawScanner(hdc, &rcMine);
            ReleaseDC(hwndScanner, hdc);
        }
        fMineFieldSel = sel.scan.grobj == grobjThing && lpThings[sel.scan.ith].ith == ithMinefield;
        if (fMineFieldSel != 0) {
            iRad = LOWORD((int32_t)(sqrt((double)lpThings[sel.scan.ith].thm.cMines) + 1.0));
            rcMine.left = lpThings[sel.scan.ith].pt.x;
            rcMine.top = lpThings[sel.scan.ith].pt.y;
            rcMine.right = rcMine.left + iRad;
            rcMine.bottom = rcMine.top - iRad;
            rcMine.left -= iRad;
            rcMine.top += iRad;
            t_pt_9001.x = rcMine.left;
            t_pt_9001.y = rcMine.top;
            LogicalToScan(&t_pt_9001);
            rcMine.left = t_pt_9001.x;
            rcMine.top = t_pt_9001.y;
            t_pt_900d.x = rcMine.right;
            t_pt_900d.y = rcMine.bottom;
            LogicalToScan(&t_pt_900d);
            rcMine.right = t_pt_900d.x;
            rcMine.bottom = t_pt_900d.y;
            InflateRect(&rcMine, 1, 1);
        }
        if (fMineFieldSel != 0) {
            hdc = GetDC(hwndScanner);
            DrawScanner(hdc, &rcMine);
            ReleaseDC(hwndScanner, hdc);
        }
        if (sel.pl.id != -1) {
            DrawPlanShip(NULL, 16386);
        }
        if (gd.fTutorial != 0 && idPlayer == 0) {
            AdvanceTutor();
        }
    }
    return;
}

int16_t FGetNextObjHere(SCAN *pscan, int16_t fOnlyOurs) {
    FLEET  *lpfl;
    int16_t i;
    int16_t fFound;

    fFound = sel.grobj == grobjFleet ? 0 : 1;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if (fFound == 0) {
            if (lpfl->id == sel.id) {
                fFound = 1;
            }
        } else if (sel.pt.x == lpfl->pt.x && sel.pt.y == lpfl->pt.y && (fOnlyOurs == 0 || lpfl->iPlayer == idPlayer)) {
            break;
        }
    }
    if (fFound == 0) {
        return 0;
    }
    if (i < cFleet) {
        pscan->ifl = i;
        pscan->grobj = grobjFleet;
    } else if ((sel.grobjFull & 1) != 0 && sel.pl.iPlayer == idPlayer) {
        pscan->grobj = grobjPlanet;
        pscan->idpl = sel.pl.id;
    } else {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0 || (pscan->pt.x == lpfl->pt.x && pscan->pt.y == lpfl->pt.y && (fOnlyOurs == 0 || lpfl->iPlayer == idPlayer)))
                break;
        }
        if (i >= cFleet || lpfl->id == sel.id) {
            return 0;
        }
        pscan->ifl = i;
        pscan->grobj = grobjFleet;
    }
    return 1;
}

INT_PTR CALLBACK FindDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char szName[40];
    RECT rc;

    if (msg != WM_ERASEBKGND) {
        if (IS_WM_CTLCOLOR(msg) == 0) {
            if (msg == WM_INITDIALOG) {
                StickyDlgPos(hwnd, &ptStickyFindDlg, 1);
                SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0x27, 0);
                return 1;
            }
            if (msg == WM_COMMAND) {
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                case IDCANCEL:
                    if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                        GetDlgItemText(hwnd, IDC_EDIT1, szName, 40);
                        if (FSelectSz(szName) == 0) {
                            AlertSz(PszFormatIds(idsSorryCantFindPlanetFleetName, NULL), MB_ICONHAND);
                            SetFocus(GetDlgItem(hwnd, IDC_EDIT1));
                            SendDlgItemMessage(hwnd, 268, EM_SETSEL, 0, -1);
                            return 0;
                        }
                    }
                    StickyDlgPos(hwnd, &ptStickyFindDlg, 0);
                    EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                    return 1;
                case IDC_HELP:
                    WinHelp(hwnd, szHelpFile, 1, 1085);
                    return 1;
                }
            }
        } else if (HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
        return 0;
    }
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;
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

    iplPartial = -1;
    scan.iwp = -1;
    for (ipl = 0; ipl < game.cPlanMax && strcmpi(PszGetCompressedPlanet(rgidPlan[ipl]), szName) != 0; ipl++) {
        if (iplPartial == -1 && strnicmp(PszGetCompressedPlanet(rgidPlan[ipl]), szName, strlen(szName)) == 0) {
            iplPartial = ipl;
        }
    }
    for (; ipl == game.cPlanMax; ipl = iplPartial) {
        cch = CchGetString(idsFleet, szT);
        if (strnicmp(szName, szT, cch) == 0) {
            pch = szName + 6;
        } else {
            pch = szName;
        }
        for (; (int16_t)(int8_t)*pch == ' '; pch++) {
        }
        if ((int16_t)(int8_t)*pch == '#') {
            pch++;
        }
        for (; (int16_t)(int8_t)*pch == ' '; pch++) {
        }
        if ((int16_t)(int8_t)*pch >= '1' && (int16_t)(int8_t)*pch <= '9') {
            ifl = (int16_t)(int8_t)*pch - 48;
            pch++;
            while (isdigit((int16_t)(int8_t)*pch) != 0) {
                ifl = 10 * ifl + (int16_t)(int8_t)*pch - 48;
                pch++;
                if (ifl > 512)
                    goto LNotAFleetId;
            }
            ifl--;
            ifl |= idPlayer << 9;
            lpfl = LpflFromId(ifl);
            if (lpfl != 0)
                goto LFoundFleetId;
        }
    LNotAFleetId:
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            if (strcmpi(PszGetFleetName(lpfl->id), szName) == 0)
                goto LFoundFleetId;
        }
        if (iplPartial == -1) {
            return 0;
        }
    }
    FFindNearestObject(rgptPlan[ipl], grobjPlanet, &scan);
    ChangeScanSel(&scan, 1);
    FEnsurePointOnScreen(scan.pt, 1);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 0x76, 0);
    return 1;
LFoundFleetId:
    FFindNearestObject(lpfl->pt, grobjFleet, &scan);
    scan.ifl = IflFromLpfl(lpfl);
    ChangeScanSel(&scan, 2);
    FEnsurePointOnScreen(scan.pt, 1);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 0x76, 0);
    return 1;
}

void GetScanFleetOrientation(FLEET *lpfl, POINT16 *ppt, POINT16 *pptD) {
    int16_t dy;
    int16_t dx;

    if (lpfl->iPlayer == idPlayer) {
        if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].iWarp != 0) {
            dx = lpfl->lpplord->rgord[1].pt.x - lpfl->pt.x;
            dy = lpfl->lpplord->rgord[1].pt.y - lpfl->pt.y;
            goto L_985e;
        }
    } else if (lpfl->fdirValid != 0 && lpfl->iwarpFlt != 0) {
        dx = lpfl->dirFltX - 127;
        dy = lpfl->dirFltY - 127;
        goto L_985e;
    }
    dy = 0;
    dx = 0;
L_985e:
    GetDxDyOrientation(dx, dy, ppt, pptD);
    return;
}

void GetDxDyOrientation(int16_t dx, int16_t dy, POINT16 *ppt, POINT16 *pptD) {
    double  dbl;
    int16_t iBmp;
    int16_t t_merge_9938_0001;

    iBmp = 0;
    if (dx != 0 || dy != 0) {
        dbl = (atan2((double)dy, (double)dx) + 3.1415927) * 4.0 / 3.141592654 + 0.5;
        iBmp = 8 - (LOWORD((int32_t)dbl) & 7);
        iBmp = 8 - (LOWORD((int32_t)dbl) & 7) + 1 & 7;
    }
    t_merge_9938_0001 = iScanZoom < 0 ? 7 : 9;
    pptD->y = t_merge_9938_0001;
    pptD->x = t_merge_9938_0001;
    if (iScanZoom >= 0) {
        ppt->x = 7;
    } else {
        ppt->x = 0;
    }
    ppt->y = iBmp * pptD->y;
    return;
}

int16_t FHandleMeasuringTape(SCAN *pscan, POINT16 pt) {
    HDC     hdc;
    HPEN    hpenSav;
    SBAR    sbar;
    POINT16 ptLogLast;
    int16_t grTypeIn;
    POINT16 ptLogical;
    POINT16 ptBase;
    int16_t iropSav;
    POINT16 ptNew;
    char    szT[20];
    int16_t fVirgin;
    SCAN    scan;
    RECT    rc;
    int16_t t_merge_9a5d_0001;
    int16_t t_merge_9a98_0001;

    fVirgin = 1;
    ptLogLast = pscan->pt;
    ptBase = pscan->pt;
    LogicalToScan(&ptBase);
    GetClientRect(hwndScanner, &rc);
    rc.bottom -= dySBar;
    hdc = GetDC(hwndScanner);
    SetCapture(hwndScanner);
    sbar.pscan = pscan;
    hpenSav = SelectObject(hdc, hpenShip);
    iropSav = SetROP2(hdc, 7);
    ptNew = pt;
    LogicalToScan(&ptNew);
    while (FGetRMouseMove(&ptNew) != 0) {
        t_merge_9a5d_0001 = 0 > (rc.right >= ptNew.x ? ptNew.x : rc.right) ? 0 : rc.right < ptNew.x ? rc.right : ptNew.x;
        ptNew.x = t_merge_9a5d_0001;
        t_merge_9a98_0001 = 0 > (rc.bottom >= ptNew.y ? ptNew.y : rc.bottom) ? 0 : rc.bottom < ptNew.y ? rc.bottom : ptNew.y;
        ptNew.y = t_merge_9a98_0001;
        ptLogical = ptNew;
        ScanToLogical(&ptLogical);
        grTypeIn = (GetAsyncKeyState(16) & 0xfffe) == 0 ? 79 : 143;
        if (FFindNearestObject(ptLogical, grTypeIn, &scan) != 0) {
            ptLogical = scan.pt;
        }
        if (ptLogLast.x != ptLogical.x || ptLogLast.y != ptLogical.y) {
            ptNew = ptLogical;
            LogicalToScan(&ptNew);
            if (fVirgin != 0) {
                if (abs(ptLogical.x - ptLogLast.x) < 3 && abs(ptLogical.y - ptLogLast.y) < 3)
                    continue;
                fVirgin = 0;
            } else {
                MoveTo(hdc, ptBase.x, ptBase.y);
                LineTo(hdc, pt.x, pt.y);
            }
            MoveTo(hdc, ptBase.x, ptBase.y);
            LineTo(hdc, ptNew.x, ptNew.y);
            switch (scan.grobj) {
            case grobjNone:
                sbar.id = -1;
                CchGetString(idsDeepSpace, szT);
                sbar.psz = szT;
                break;
            case grobjPlanet:
                sbar.id = scan.idpl;
                goto L_9c32;
            case grobjFleet:
                sbar.id = rglpfl[scan.ifl]->id;
                goto L_9c32;
            case grobjThing:
                sbar.id = lpThings[scan.ith].idFull;
                goto L_9c32;
            default:
                sbar.id = scan.iwp;
                goto L_9c32;
            }
            goto L_9c37;
        L_9c32:
            sbar.psz = 0;
        L_9c37:
            sbar.pt = ptLogical;
            sbar.grbit = scan.grobj;
            DrawScannerSBar(hdc, NULL, &sbar, 0);
            pt = ptNew;
            ptLogLast = ptLogical;
        }
    }
    if (fVirgin == 0) {
        MoveTo(hdc, ptBase.x, ptBase.y);
        LineTo(hdc, pt.x, pt.y);
    }
    SetROP2(hdc, iropSav);
    SelectObject(hdc, hpenSav);
    DrawScannerSBar(hdc, NULL, NULL, 0);
    ReleaseCapture();
    ReleaseDC(hwndScanner, hdc);
    if (fVirgin == 0) {
        return 1;
    }
    return 0;
}
