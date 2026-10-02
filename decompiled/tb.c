#include "common.h"

char    vrgTBBtn[29] = {tbNormalView,
                        tbSurfaceMineralView,
                        tbMineralConcView,
                        tbPlanetValueView,
                        tbPopulationView,
                        tbNoPlayerInfoView,
                        tbSeparator,
                        tbAddWaypoints,
                        tbSeparator,
                        tbScannerCoverage,
                        tbSpacer,
                        tbScannerRange,
                        tbSeparator,
                        tbMineFields,
                        tbSeparator,
                        tbFleetPaths,
                        tbSeparator,
                        tbPlanetNames,
                        tbShipCounts,
                        tbSeparator,
                        tbIdleFleets,
                        tbSeparator,
                        tbShipDesignFilter,
                        tbShipDesignFilterMenu,
                        tbSeparator,
                        tbEnemyClassFilter,
                        tbEnemyClassFilterMenu,
                        tbSeparator,
                        tbZoomMenu};
int16_t vrgpctZoom[9] = {25, 38, 50, 75, 100, 125, 150, 200, 400};

LRESULT CALLBACK TbWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC           hdc;
    int16_t       fInside;
    POINT16       pt;
    StringId      ids;
    ToolbarButton itb;
    PAINTSTRUCT   ps;
    int16_t       i;
    int16_t       fCur;
    int16_t       fDown;
    int16_t       iSel;
    int16_t       dx;
    POINT16       ptBtn;
    int16_t       j;
    int16_t       x;
    RECT          rc;
    HWND          hwndCE;
    int16_t       pct;

    switch (msg) {
    case WM_CREATE:
        x = 4;
        for (i = 0; i < 29; i++) {
            itb = (int16_t)(int8_t)vrgTBBtn[i];
            dx = DxOfBtn(itb);
            if (itb <= tbScannerRange && itb == tbScannerRange) {
                hwndTBRadar = CreateWindow("COMBOBOX", NULL, CBS_DROPDOWN | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_VSCROLL, x,
                                           (int16_t)(28 - dyArial8 - 8) / 2 + 4, dx, 11 * dyArial8 + 28, hwnd, NULL, hInst, NULL);
                SendMessage(hwndTBRadar, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
                iSel = -1;
                for (j = 0; j < 10; j++) {
                    pct = 100 - 10 * j;
                    if (pct == vpctRadarView) {
                        iSel = j;
                    }
                    _wsprintf(szWork, PCTDPCTPCT, pct);
                    SendMessage(hwndTBRadar, CB_ADDSTRING, 0, (LPARAM)szWork);
                }
                SendMessage(hwndTBRadar, CB_SETEXTENDEDUI, 4, 0);
                SendMessage(hwndTBRadar, CB_SETCURSEL, iSel, 0);
                _wsprintf(szWork, PCTDPCTPCT, vpctRadarView);
                SetWindowText(hwndTBRadar, szWork);
                lpfnRealComboProc = GetWindowLong(hwndTBRadar, GWL_WNDPROC);
                SetWindowLong(hwndTBRadar, GWL_WNDPROC, lpfnFakeComboProc);
                hwndCE = GetWindow(hwndTBRadar, GW_CHILD);
                if (hwndCE != 0) {
                    lpfnRealCEProc = GetWindowLong(hwndCE, GWL_WNDPROC);
                    SetWindowLong(hwndCE, GWL_WNDPROC, lpfnFakeCEProc);
                }
            }
            x += dx;
        }
        break;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        PatBlt(hdc, 0, 0, rc.right, 1, BLACKNESS);
        PatBlt(hdc, 0, rc.bottom - 1, rc.right, 1, BLACKNESS);
        if (iWindowLayout == layoutLarge) {
            PatBlt(hdc, 0, 0, 1, rc.bottom, BLACKNESS);
        }
        DrawToolbar(hdc, &rc);
        EndPaint(hwnd, &ps);
        break;
    default:
        if (IS_WM_CTLCOLOR(msg) != 0) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (LRESULT)hbrButtonFace;
        }
        switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            ShowTooltip(0xffff, NULL);
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            ptBtn = pt;
            itb = ItbFromPpt(&ptBtn);
            if (itb < tbNormalView)
                break;
            fDown = FIsButtonDown(itb);
            if (fDown != 0 && itb <= tbNoPlayerInfoView) {
                MessageBeep(MB_OK);
                break;
            }
            switch (itb) {
            case tbShipDesignFilterMenu:
            case tbEnemyClassFilterMenu:
            case tbZoomMenu:
            case tbMineFields:
                ExecuteButton(itb, fDown == 0);
                break;
            default:
                dx = DxOfBtn(itb);
                rc.left = ptBtn.x;
                rc.top = ptBtn.y;
                rc.right = rc.left + dx;
                rc.bottom = rc.top + 28;
                hdc = GetDC(hwnd);
                SelectPalette(hdc, vhpal, FALSE);
                RealizePalette(hdc);
                SetCapture(hwnd);
                fCur = -1;
                while (FGetMouseMove(&pt) != 0) {
                    fInside = PtInRect(&rc, PointFrom16(pt));
                    if (fCur != fInside) {
                        DrawBitmapButton(hdc, ptBtn, itb, fDown + fInside);
                        fCur = fInside;
                    }
                }
                ReleaseCapture();
                if (fInside != 0) {
                    ExecuteButton(itb, fDown == 0);
                    fDown = FIsButtonDown(itb);
                }
                if (itb <= tbNoPlayerInfoView) {
                    GetClientRect(hwnd, &rc);
                    DrawToolbar(hdc, &rc);
                } else {
                    DrawBitmapButton(hdc, ptBtn, itb, fDown);
                }
                ReleaseDC(hwnd, hdc);
            }
            break;
        case WM_MOUSEMOVE:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            if (hwnd != hwndTb) {
                MapWindowPoints16(hwnd, hwndTb, &pt, 1);
            }
            if (pt.x == vptTbLast.x && pt.y == vptTbLast.y)
                break;
            vptTbLast = pt;
            itb = ItbFromPpt(&pt);
            if (itb >= tbNormalView) {
                ids = itb + 362;
            } else {
                if (itb != tbScannerRange)
                    break;
                ids = idsScannerEffective;
            }
            rc.left = pt.x;
            rc.right = DxOfBtn(itb) + rc.left;
            rc.top = pt.y;
            rc.bottom = 28;
            MapWindowPoints(hwndTb, NULL, (POINT *)&rc, 2);
            ShowTooltip(ids, &rc);
            break;
        case WM_SETCURSOR:
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32512)));
            return 1;
        case WM_COMMAND:
            if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndTBRadar || GET_WM_COMMAND_CMD(wParam, lParam) != 8)
                break;
            PostMessage(hwnd, 1524, 0, 0);
            break;
        case 1524:
            TerminateToolbarFocus(FALSE);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return 0;
}

void DrawToolbar(HDC hdc, RECT *prc) {
    POINT16       pt;
    int16_t       i;
    ToolbarButton ibtn;

    SelectPalette(hdc, vhpal, FALSE);
    RealizePalette(hdc);
    pt.x = 4;
    pt.y = 4;
    for (i = 0; i < 29; i++) {
        ibtn = (int16_t)(int8_t)vrgTBBtn[i];
        if (ibtn >= tbNormalView) {
            DrawBitmapButton(hdc, pt, (int16_t)(int8_t)vrgTBBtn[i], FIsButtonDown(ibtn));
        } else if (ibtn <= tbScannerRange) {
        }
        pt.x += DxOfBtn(ibtn);
    }
    return;
}

void DrawBitmapButton(HDC hdc, POINT16 pt, ToolbarButton ibtn, int16_t fDown) {
    int16_t dx;
    HBRUSH  hbrBotRight;
    HBRUSH  hbrTopLeft;
    int16_t dxDraw;

    dx = DxOfBtn(ibtn);
    dxDraw = dx >= 24 ? 24 : 7;
    if (fDown != 0) {
        hbrTopLeft = hbrButtonShadow;
        hbrBotRight = hbrButtonHilite;
    } else {
        hbrTopLeft = hbrButtonHilite;
        hbrBotRight = hbrButtonShadow;
    }
    SelectObject(hdc, hbrTopLeft);
    PatBlt(hdc, pt.x + 2, pt.y, dx - 4, 1, PATCOPY);
    PatBlt(hdc, pt.x, pt.y + 2, 1, 24, PATCOPY);
    PatBlt(hdc, pt.x + 1, pt.y + 1, 1, 1, PATCOPY);
    PatBlt(hdc, pt.x + 1, pt.y + 26, 1, 1, PATCOPY);
    SelectObject(hdc, hbrBotRight);
    PatBlt(hdc, pt.x + 2, pt.y + 27, dx - 4, 1, PATCOPY);
    PatBlt(hdc, pt.x + dx - 1, pt.y + 2, 1, 24, PATCOPY);
    PatBlt(hdc, pt.x + dx - 2, pt.y + 1, 1, 1, PATCOPY);
    PatBlt(hdc, pt.x + dx - 2, pt.y + 26, 1, 1, PATCOPY);
    SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, pt.x + 2, pt.y + 1, dx - 4, 1, PATCOPY);
    PatBlt(hdc, pt.x + 1, pt.y + 2, 1, 24, PATCOPY);
    if (fDown != 0) {
        PatBlt(hdc, pt.x + 2, pt.y + 2, dx - 4, fDown, PATCOPY);
        PatBlt(hdc, pt.x + 2, pt.y + 2, fDown, 24, PATCOPY);
        if (fDown == 1) {
            PatBlt(hdc, pt.x + 2, pt.y + 26, dx - 4, 1, PATCOPY);
            PatBlt(hdc, pt.x + dx - 2, pt.y + 2, 1, 24, PATCOPY);
        }
    } else {
        PatBlt(hdc, pt.x + 2, pt.y + 25, dx - 4, 2, PATCOPY);
        PatBlt(hdc, pt.x + dx - 3, pt.y + 2, 2, 24, PATCOPY);
    }
    DibBlt(hdc, pt.x + 2 + fDown, pt.y + 2 + fDown, dxDraw, 23, hdibToolbar, 24 * ibtn, 0, dxDraw, 23, 13369376);
    if (fDown > 1) {
        SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, pt.x + dx - 2, pt.y + 26, 1, 1, PATCOPY);
    }
    return;
}

int16_t ItbFromPpt(POINT16 *ppt) {
    int16_t i;
    int16_t dx;
    int16_t x;

    x = 4;
    if (ppt->x < 4 || ppt->y < 4 || ppt->y >= 32) {
        return -1;
    }
    for (i = 0; i < 29; i++) {
        dx = DxOfBtn((int16_t)(int8_t)vrgTBBtn[i]);
        if (x + dx > ppt->x) {
            ppt->x = x;
            ppt->y = 4;
            return (int16_t)(int8_t)vrgTBBtn[i];
        }
        x += dx;
    }
    return -1;
}

int16_t DxOfBtn(ToolbarButton itb) {
    if (itb >= tbNormalView) {
        if (itb != tbShipDesignFilterMenu && itb != tbEnemyClassFilterMenu) {
            return 29;
        }
        return 11;
    }
    switch (itb) {
    case tbSeparator:
        return 6;
    case tbSpacer:
        return 2;
    case tbScannerRange:
        if (dyArial8 < 16) {
            return 60;
        }
        return 70;
    default:
        return 0;
    }
}

int16_t FIsButtonDown(ToolbarButton itb) {
    if ((uint16_t)itb > tbShipCounts) {
        return FALSE;
    }
    switch (itb) {
    case tbNormalView:
    case tbSurfaceMineralView:
    case tbMineralConcView:
    case tbPlanetValueView:
    case tbPopulationView:
    case tbNoPlayerInfoView:
        if ((grbitScan & grbitScanViewMask) == itb) {
            return TRUE;
        }
        return FALSE;
    case tbAddWaypoints:
        if ((grbitScan & grbitScanAddWaypoints) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbScannerCoverage:
        if ((grbitScan & grbitScanCoverage) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbMineFields:
        if ((grbitScan & grbitScanMineFields) != 0 && grbitScanMines == 15) {
            return TRUE;
        }
        return FALSE;
    case tbFleetPaths:
        if ((grbitScan & grbitScanFleetPaths) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbIdleFleets:
        if ((grbitScan & grbitScanIdleFleets) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbPlanetNames:
        if ((grbitScan & grbitScanPlanetNames) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbShipCounts:
        if ((grbitScan & grbitScanShipCounts) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbShipDesignFilter:
        if ((grbitScan & grbitScanDesignFilter) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbEnemyClassFilter:
        if ((grbitScan & grbitScanEnemyFilter) != 0) {
            return TRUE;
        }
        return FALSE;
    case tbShipDesignFilterMenu:
    case tbEnemyClassFilterMenu:
    case tbZoomMenu:
        return FALSE;
    }
}

void ExecuteButton(ToolbarButton itb, int16_t fDown) {
    uint16_t grbitNew;
    POINT16  pt;
    char    *rgszScan[12];
    int16_t  c;
    int16_t  i;
    uint16_t grbit;
    int32_t  rgid[12];
    int16_t  iSel;
    uint16_t grbitSh;
    int16_t  ish;

    gd.fChgScanner = TRUE;
    if ((uint16_t)itb <= tbShipCounts) {
        switch (itb) {
        case tbNormalView:
        case tbSurfaceMineralView:
        case tbMineralConcView:
        case tbPlanetValueView:
        case tbPopulationView:
        case tbNoPlayerInfoView:
            if (fDown == 0)
                break;
            grbitScan = itb + (grbitScan & grbitScanToggleMask);
            goto L_1644;
        case tbAddWaypoints:
            grbitNew = 16;
            goto LBitDiddle;
        case tbScannerCoverage:
            grbitNew = 32;
            goto LBitDiddle;
        case tbFleetPaths:
            grbitNew = 128;
            goto LBitDiddle;
        case tbIdleFleets:
            grbitNew = 0x100;
            goto LBitDiddle;
        case tbPlanetNames:
            grbitNew = 0x400;
            goto LBitDiddle;
        case tbShipCounts:
            grbitNew = 0x1000;
            goto LBitDiddle;
        case tbShipDesignFilter:
            grbitNew = 0x200;
            goto LBitDiddle;
        case tbEnemyClassFilter:
            grbitNew = 0x800;
            goto LBitDiddle;
        case tbMineFields:
            grbit = 1;
            c = 0;
            if ((grbitScan & grbitScanMineFields) == 0) {
                grbitScanMines = 0;
            }
            for (i = 1278; i <= 1279; i++) {
                if (i == 1278) {
                    rgid[c] = (uint32_t)(grbitScanMines == 15);
                } else {
                    rgid[c] = (uint32_t)(grbitScanMines == 0);
                }
                CchGetString(i, &szWork[(i - 1278) * 30 + 160]);
                rgszScan[c++] = &szWork[(i - 1278) * 30 + 160];
            }
            rgid[c] = 0;
            szWork[250] = -1;
            szWork[251] = 0;
            rgszScan[c++] = &szWork[250];
            for (i = 0; i < 4; i++) {
                rgid[c] = (uint32_t)((1 << i & grbitScanMines) != 0);
                CchGetString(i + 1280, &szWork[i * 30]);
                rgszScan[c++] = &szWork[i * 30];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, FALSE);
            if (iSel == -1)
                break;
            if (iSel >= 3) {
                iSel -= 3;
                grbitScanMines ^= 1 << iSel;
            } else if (iSel == 0) {
                grbitScanMines = 15;
            } else {
                grbitScanMines = 0;
            }
            if (grbitScanMines != 0) {
                grbitScan |= grbitScanMineFields;
            } else {
                grbitScan &= 0xffbf;
            }
            InvalidateRect(hwndTb, NULL, TRUE);
            goto L_1644;
        case tbShipDesignFilterMenu:
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 20]);
                rgszScan[c++] = &szWork[(i - 1275) * 20];
            }
            rgid[c] = 0;
            szWork[200] = -1;
            szWork[201] = 0;
            rgszScan[c++] = &szWork[200];
            ish = 0;
            grbitSh = 1;
            while (ish < 16) {
                if (rgshdef[ish].fFree == 0) {
                    rgid[c] = (uint32_t)((grbitSh & grbitScanShip) != 0);
                    rgszScan[c++] = rgshdef[ish].hul.szClass;
                }
                ish++;
                grbitSh *= 2;
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, FALSE);
            if (iSel == -1)
                break;
            if (iSel < 4) {
                if (iSel == 0) {
                    grbitScanShip = 0xffff;
                } else if (iSel == 1) {
                    grbitScanShip ^= 0xffff;
                } else {
                    grbitScanShip = 0;
                }
                if ((grbitScan & grbitScanDesignFilter) != 0 || grbitScanShip == 0)
                    goto L_12e6;
            } else {
                iSel -= 4;
                for (ish = 0; ish < 16; ish++) {
                    if (rgshdef[ish].fFree == 0) {
                        iSel--;
                        if (iSel < 0)
                            break;
                    }
                }
                grbitScanShip ^= 1 << ish;
                if ((grbitScan & grbitScanDesignFilter) != 0 || (1 << ish & grbitScanShip) == 0)
                    goto L_12e6;
            }
            grbitScan |= grbitScanDesignFilter;
            InvalidateRect(hwndTb, NULL, TRUE);
        L_12e6:
            if ((grbitScan & grbitScanDesignFilter) == 0)
                break;
            goto L_1644;
        case tbEnemyClassFilterMenu:
            grbit = 1;
            c = 0;
            for (i = 1275; i <= 1277; i++) {
                rgid[c] = 0;
                CchGetString(i, &szWork[(i - 1275) * 25 + 200]);
                rgszScan[c++] = &szWork[(i - 1275) * 25 + 200];
            }
            rgid[c] = 0;
            szWork[300] = -1;
            szWork[301] = 0;
            rgszScan[c++] = &szWork[300];
            for (i = 0; i < 8; i++) {
                rgid[c] = (uint32_t)((1 << i & grbitScanEShip) != 0);
                CchGetString(i + 381, &szWork[i * 25]);
                rgszScan[c++] = &szWork[i * 25];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, FALSE);
            if (iSel == -1)
                break;
            if (iSel < 4) {
                if (iSel == 0) {
                    grbitScanEShip = 0xff;
                } else if (iSel == 1) {
                    grbitScanEShip ^= 0xff;
                } else {
                    grbitScanEShip = 0;
                }
                if ((grbitScan & grbitScanEnemyFilter) != 0 || grbitScanEShip == 0)
                    goto L_1505;
            } else {
                iSel -= 4;
                grbitScanEShip ^= 1 << iSel;
                if ((grbitScan & grbitScanEnemyFilter) != 0 || (1 << iSel & grbitScanEShip) == 0)
                    goto L_1505;
            }
            grbitScan |= grbitScanEnemyFilter;
            InvalidateRect(hwndTb, NULL, TRUE);
        L_1505:
            if ((grbitScan & grbitScanEnemyFilter) == 0)
                break;
            goto L_1644;
        case tbZoomMenu:
            c = 0;
            for (i = 0; i < 9; i++) {
                rgid[c] = (uint32_t)(iScanZoom + 4 == i);
                _wsprintf(&szWork[i * 8], PCTDPCTPCT, vrgpctZoom[i]);
                rgszScan[c++] = &szWork[i * 8];
            }
            GetCursorPos16(&pt);
            ScreenToClient16(hwndTb, &pt);
            iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, -2, FALSE);
            if (iSel != -1) {
                CommandHandler(hwndFrame, iSel + 3901);
            }
        }
        return;
    LBitDiddle:
        if (fDown != 0) {
            grbitScan |= grbitNew;
        } else {
            grbitScan &= ~grbitNew;
        }
    L_1644:
        if (itb != tbAddWaypoints) {
            InvalidateRect(hwndScanner, NULL, TRUE);
        }
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    }
    return;
}

void TerminateToolbarFocus(int16_t fCancel) {
    int16_t pct;
    char   *psz;

    gd.fChgScanner = TRUE;
    if (fCancel == 0) {
        GetWindowText(hwndTBRadar, szWork, 20);
        psz = szWork;
        pct = 0;
        for (; *psz >= '0' && *psz <= '9'; psz++) {
            pct = 10 * pct + (*psz - '0');
        }
        if (*psz != 0 && *psz != '%') {
            pct = 0;
        }
    } else {
        pct = vpctRadarView;
    }
    if (pct < 2) {
        pct = 2;
    } else if (pct > 100) {
        pct = 100;
    }
    SendMessage(hwndTBRadar, CB_SETCURSEL, (int16_t)(100 - pct) / 10, 0);
    _wsprintf(szWork, PCTDPCTPCT, pct);
    SetWindowText(hwndTBRadar, szWork);
    if (pct != vpctRadarView) {
        vpctRadarView = pct;
        if ((grbitScan & grbitScanCoverage) == 0) {
            InvalidateRect(hwndTb, NULL, TRUE);
        }
        grbitScan |= grbitScanCoverage;
        InvalidateRect(hwndScanner, NULL, TRUE);
    }
    SetFocus(hwndFrame);
    return;
}

void ShowTooltip(StringId ids, RECT *prc) {
    HDC      hdc;
    HFONT    hfontSav;
    int16_t  fVisCur;
    int16_t  cch;
    int16_t  fShowNow;
    uint32_t t_scratch_m10;

    fVisCur = hwndTooltip != 0 && IsWindowVisible(hwndTooltip) != 0;
    if ((int16_t)ids < idsUniverseDefinitionFileSeemsMissingCorrupt || prc == 0) {
        if (hwndTooltip != 0) {
            DestroyWindow(hwndTooltip);
        }
        if (fVisCur != 0) {
            vtickTooltipLast = GetTickCount();
        }
    } else if (ids != vidsTooltip || EqualRect(prc, &vrcTooltip) == 0) {
        vidsTooltip = ids;
        cch = CchGetString(vidsTooltip, szWork);
        vrcTooltip = *prc;
        hdc = GetDC(NULL);
        hfontSav = SelectObject(hdc, rghfontArial8[0]);
        dxTip = LOWORD(GetTextExtent(hdc, szWork, cch));
        SelectObject(hdc, hfontSav);
        ReleaseDC(NULL, hdc);
        if (hwndTooltip == 0) {
            CreateWindow(szTooltip, NULL, WS_POPUP, 100, 100, dxTip + 6, dyArial8 + 6, hwndTb, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndTooltip, NULL, TRUE);
        }
        SetWindowPos(hwndTooltip, (HWND)-1, 0, 0, dxTip + 6, dyArial8 + 6, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
        t_scratch_m10 = vtickTooltipLast + 400;
        fShowNow = t_scratch_m10 >= GetTickCount() || fVisCur != 0;
        TooltipWndProc(hwndTooltip, 1523, fShowNow, 0);
    }
    return;
}

LRESULT CALLBACK TooltipWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    POINT16     pt;
    PAINTSTRUCT ps;
    RECT        rc;
    int16_t     bkSav;
    int16_t     cch;

    switch (msg) {
    case WM_CREATE:
        hwndTooltip = hwnd;
        goto L_1d53;
    case WM_DESTROY:
        hwndTooltip = 0;
        if (vidTimerTooltip != -1) {
            KillTimer(hwnd, vidTimerTooltip);
            vidTimerTooltip = -1;
        }
        vidsTooltip = -1;
        goto L_1d53;
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        DestroyWindow(hwnd);
        return 0;
    case 1523:
        if (wParam == 0) {
            if (vidTimerTooltip != -1) {
                KillTimer(hwnd, vidTimerTooltip);
            }
            if (SetTimer(hwnd, 926, 700, NULL) != 0) {
                vidTimerTooltip = 926;
            } else {
                vidTimerTooltip = -1;
            }
            return 0;
        }
        wParam = 926;
    case WM_TIMER:
        if (wParam != 926)
            goto L_1d53;
        if (msg != WM_TIMER || IsWindowVisible(hwnd) == 0) {
            vtickTooltip1stVis = GetTickCount();
            GetCursorPos16(&pt);
            if (PtInRect(&vrcTooltip, PointFrom16(pt)) != 0) {
                ScreenToClient16(hwndFrame, &pt);
                if (pt.x + dxTip > vfs.dx) {
                    pt.x = vfs.dx - dxTip - 5;
                }
                ClientToScreen16(hwndFrame, &pt);
                SetWindowPos(hwnd, (HWND)-1, pt.x, (int16_t)(3 * dyArial8) / 2 + pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOOWNERZORDER);
                UpdateWindow(hwnd);
                if (vidTimerTooltip != -1) {
                    KillTimer(hwnd, vidTimerTooltip);
                }
                if (SetTimer(hwnd, 926, 50, NULL) != 0) {
                    vidTimerTooltip = 926;
                } else {
                    vidTimerTooltip = -1;
                }
                return 0;
            }
        } else {
            vtickTooltipLast = GetTickCount();
            GetCursorPos16(&pt);
            if (PtInRect(&vrcTooltip, PointFrom16(pt)) != 0 && vtickTooltip1stVis + 10000 >= vtickTooltipLast) {
                return 0;
            }
        }
        DestroyWindow(hwnd);
        return 0;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrTooltip);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        FrameRect(hdc, &rc, hbrWindowFrame);
        SelectObject(hdc, rghfontArial8[0]);
        cch = CchGetString(vidsTooltip, szWork);
        bkSav = SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, crWindowText);
        ExtTextOut(hdc, 3, 3, 0, NULL, szWork, cch, NULL);
        SetBkMode(hdc, bkSav);
        EndPaint(hwnd, &ps);
        return 0;
    default:
    L_1d53:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK FakeComboProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        ShowTooltip(0xffff, NULL);
        break;
    case WM_MOUSEMOVE:
        TbWndProc(hwnd, msg, wParam, lParam);
    }
    return CallWindowProc(lpfnRealComboProc, hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK FakeCEProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        ShowTooltip(0xffff, NULL);
        break;
    case WM_MOUSEMOVE:
        TbWndProc(hwnd, msg, wParam, lParam);
    }
    return CallWindowProc(lpfnRealCEProc, hwnd, msg, wParam, lParam);
}
