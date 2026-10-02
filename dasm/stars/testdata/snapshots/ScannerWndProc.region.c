LRESULT CALLBACK ScannerWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    POINT16     pt;
    PAINTSTRUCT ps;
    RECT        rc;
    ScanZoom    iScanNew;
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

    switch (msg) {
    case WM_MDIACTIVATE:
        hwndActive = GET_WM_MDIACTIVATE_FACTIVATE(hwnd, wParam, lParam) == 0 ? NULL : hwnd;
        break;
    case WM_CREATE:
        yScanTop = 1000;
        xScanTop = 1000;
        break;
    case WM_CHAR:
        switch (wParam) {
        case 'v':
        case 'V':
            hdc = GetDC(hwndScanner);
            pt = sel.scan.pt;
            LogicalToScan(&pt);
            iRopSav = SetROP2(hdc, R2_XORPEN);
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
        case '-':
            iScanNew = iScanZoom - 1;
            if (iScanNew >= zoom25)
                goto L_01ca;
            iScanNew = zoom25;
            goto L_01ca;
        default:
            iScanNew = iScanZoom + 1;
            if (iScanNew <= zoom400)
                goto L_01ca;
            iScanNew = zoom400;
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
        GetCursorPos16(&pt);
        ScreenToClient16(hwndScanner, &pt);
        GetClientRect(hwnd, &rc);
        if (PtInRect(&rc, PointFrom16(pt)) == 0)
            goto Default;
        rc.bottom -= dySBar;
        if (PtInRect(&rc, PointFrom16(pt)) == 0) {
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32512)));
        } else if (sel.grobj == grobjFleet && ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0 || (grbitScan & grbitScanAddWaypoints) != 0)) {
            SetCursor(hcurScanAdd);
        } else if (FNearAWayPoint(pt, 0) != 0) {
            SetCursor(hcurOpenGrab);
        } else if (gd.fSetMassMode != 0 || gd.fSetRouteMode != 0 ||
                   (sel.grobj == grobjPlanet &&
                    (((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0 && IWarpMAFromLppl(&sel.pl, NULL) > 0) || (GetAsyncKeyState(VK_CONTROL) & 0xfffe) != 0))) {
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
            if (msg != WM_LBUTTONDOWN || pt.y >= rc.bottom - (dySBar >> 1) || (sel.scan.grobjFull & (grobjPlanet | grobjFleet)) == 0)
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
        FFindNearestObject(pt, gd.fSetMassMode != 0 || gd.fSetRouteMode != 0 ? grobjPlanet : grobjPlanet | grobjFleet | grobjOther | grobjThing, &scan);
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
            DrawPlanShip(NULL, tileStarbaseOrWaypoint | tileMinimized);
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
            DrawPlanShip(NULL, tileProductionOrOrbit | tileMinimized);
            break;
        }
        if (msg == WM_MBUTTONDOWN || (msg == WM_RBUTTONDOWN && (wParam & 4) != 0)) {
            FHandleMeasuringTape(&scan, pt);
            break;
        }
        if (msg == WM_RBUTTONDOWN) {
            iChecked = -1;
            pt = scan.pt;
            if ((scan.grobjFull & grobjPlanet) != 0) {
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
            if (c == 2 && (scan.grobjFull & grobjPlanet) != 0) {
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
                    rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;
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
            if (sel.grobj == grobjFleet && ((wParam & 4) != 0 || (grbitScan & grbitScanAddWaypoints) != 0)) {
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
                if (plT.iPlayer != idPlayer || ((scan.grobjFull & grobjFleet) != 0 && sel.grobj == grobjPlanet && scan.idpl == sel.id)) {
                    if ((scan.grobjFull & grobjFleet) == 0)
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
        if (scan.grobj != grobjFleet || (scan.grobjFull & grobjPlanet) == 0)
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
        if (GET_WM_HSCROLL_CODE(wParam, lParam) <= SB_THUMBTRACK) {
            switch (GET_WM_HSCROLL_CODE(wParam, lParam)) {
            case SB_LINEUP:
                d = -dScanInc;
                break;
            case SB_LINEDOWN:
                d = dScanInc;
                break;
            case SB_PAGEUP:
                d = -dScanPage;
                break;
            case SB_PAGEDOWN:
                d = dScanPage;
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                d = GET_WM_HSCROLL_POS(wParam, lParam) - (msg == WM_VSCROLL ? yScanTop : xScanTop);
                d &= 0xfffc;
            }
        } else {
            d = 0;
        }
        if (d == 0)
            break;
        if (msg == WM_VSCROLL) {
            dy = yScanTop;
            SetScrollPos(hwnd, SB_VERT, yScanTop + d, 1);
            yScanTop = GetScrollPos(hwnd, SB_VERT);
            ScrollScanner(0, PtToScan(dy - yScanTop));
            break;
        }
        dx = xScanTop;
        SetScrollPos(hwnd, SB_HORZ, xScanTop + d, 1);
        xScanTop = GetScrollPos(hwnd, SB_HORZ);
        ScrollScanner(PtToScan(dx - xScanTop), 0);
        break;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
