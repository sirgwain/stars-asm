#include "common.h"

LRESULT CALLBACK MineWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rc;
    POINT16     pt;
    HtMineType  ht;
    COLORREF    crFore;
    uint16_t    dxMax;
    COLORREF    crBack;
    int16_t     cch;
    RECT        rc2;
    int16_t     fDetonate;
    RTLOGTHING  rtlt;
    POINT       t_pt_0104;
    POINT       t_pt_0113_1;

    switch (message) {
    case WM_CREATE:
        hwndMineCB = CreateWindow(szButton, PszGetCompressedString(idsDetonateMineFieldYear), BS_AUTOCHECKBOX | WS_CHILD, 100, 100, 150, dyArial8, hwnd, NULL,
                                  hInst, NULL);
        SendMessage(hwndMineCB, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        SetMineralTitleBar(hwnd);
        break;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        SetRect(&rc2, 4, 4, rc.right - 4, dyArial8 * 2 - 4);
        _Draw3dFrame(hdc, &rc2, 0);
        crFore = SetTextColor(hdc, crButtonText);
        crBack = SetBkColor(hdc, crButtonFace);
        cch = strlen(szMineralTitle);
        dxMax = rc2.right - rc2.left - 24;
        for (; cch > 0 && LOWORD(GetTextExtent(hdc, szMineralTitle, cch)) > dxMax; cch--) {
        }
        rc2.right -= 8;
        RcCtrTextOut(hdc, &rc2, szMineralTitle, cch);
        rc2.right += 8;
        SetTextColor(hdc, crFore);
        SetBkColor(hdc, crBack);
        SetRect(&rc2, rc2.right - (rc2.bottom - rc2.top) + 2, rc2.top + 3, rc2.right - 4, rc2.bottom - 2);
        DrawSelectionArrow(hdc, &rc2, FOtherStuffAtScanSel());
        rc.top += dyArial8 * 2 - 4;
        DrawMineSurvey(hdc, &rc);
        EndPaint(hwnd, &ps);
        break;
    default:
        if (IS_WM_CTLCOLOR(message) != 0) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (LRESULT)hbrButtonFace;
        }
        switch (message) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
            if (hwndPopup != 0)
                break;
            MineClick(LOWORD(lParam), HIWORD(lParam), message, wParam);
            break;
        case WM_SETCURSOR:
            GetCursorPos(&t_pt_0104);
            pt = PointTo16(t_pt_0104);
            t_pt_0113_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_0113_1);
            pt = PointTo16(t_pt_0113_1);
            GetClientRect(hwnd, &rc);
            if (PtInRect(&rc, PointFrom16(pt)) == 0)
                goto Default;
            ht = HtMineWindow(hwnd, pt.x, pt.y);
            if (ht == htMineNone)
                goto Default;
            SetCursor(ht == htMineScanSel ? hcurHand : hcurArrowHelp);
            return 1;
        case WM_COMMAND:
            if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndMineCB || GET_WM_COMMAND_CMD(wParam, lParam) != 0)
                break;
            fDetonate = LOWORD(SendMessage(hwndMineCB, BM_GETCHECK, 0, 0));
            rtlt.idFull = lpThings[sel.scan.ith].idFull;
            rtlt.fDetonate = fDetonate;
            WriteMemRt(43, 4, &rtlt);
            lpThings[sel.scan.ith].thm.fDetonate = LOBYTE(fDetonate);
            break;
        default:
        Default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

void InvalidateMineralBars() {
    HDC     hdc;
    int16_t dyRow;
    HFONT   hfontSav;
    RECT    rcPop;
    int16_t dx;
    int16_t dxPop;
    RECT    rc;

    GetClientRect(hwndMine, &rc);
    hdc = GetDC(hwndMine);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    rc.right -= LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN999mr), 5)) + 6;
    SelectObject(hdc, rghfontArial8[1]);
    dxPop = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsPopulation1000000), 21)) + 6;
    dx = LOWORD(GetTextExtent(hdc, rgszPlanetAttr[1], strlen(rgszPlanetAttr[1]))) + 6;
    if (dx * 4 > rc.right) {
        rc.left += LOWORD(GetTextExtent(hdc, rgszPlanetAttr[1], 4)) + 6;
    } else {
        rc.left += dx;
    }
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwndMine, hdc);
    rc.top += dyArial8 * 2 - 4;
    rcPop.top = rc.top + 2;
    rcPop.bottom = rcPop.top + dyArial8;
    rcPop.right = rc.right;
    rcPop.left = rc.right - dxPop;
    InvalidateRect(hwndMine, &rcPop, 1);
    dyRow = (int16_t)(rc.bottom - rc.top - dyArial8 * 4 - 2) / 6;
    dyRow = dyRow + 1 & 0xfffe;
    rc.top += (5 * dyArial8 >> 1) + 3 * dyRow + 1;
    rc.bottom = 3 * dyRow + rc.top;
    InvalidateRect(hwndMine, &rc, 0);
    return;
}

void GetMineFieldCounts(uint16_t id, int16_t *pithm, int16_t *pcthm) {
    int16_t cthTotal;
    int16_t ithFound;
    THING  *lpth;
    THING  *lpthMac;

    ithFound = 0;
    cthTotal = 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMinefield && lpth->iplr == idPlayer) {
            cthTotal++;
            if (lpth->idFull == id) {
                ithFound = cthTotal;
            }
        }
    }
    *pithm = ithFound;
    *pcthm = cthTotal;
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
    POINT16    pt;
    int16_t    iT;
    int32_t    rglT[3];
    int16_t    ifl;
    int32_t    cMines;
    int16_t    iPass;
    uint16_t   t_merge_1d8c_0001;
    int16_t    t_merge_3357_0001;
    uint16_t   t_call_334f;
    int32_t    t_merge_33e5_0001;
    int32_t    t_merge_3519_0001;

    hdcMem = CreateCompatibleDC(hdc);
    crBack = SetBkColor(hdc, crButtonFace);
    crFore = SetTextColor(hdc, 0xffff);
    bkMode = SetBkMode(hdc, OPAQUE);
    grobj = sel.scan.grobj;
    if (grobj == grobjOther) {
        if ((sel.scan.grobjFull & 1) != 0) {
            grobj = grobjPlanet;
        } else if ((sel.scan.grobjFull & 2) != 0) {
            grobj = grobjFleet;
        } else if ((sel.scan.grobjFull & 8) != 0) {
            grobj = grobjThing;
        } else {
            grobj = grobjNone;
        }
    }
    switch (grobj) {
    case grobjFleet:
        cMass = 0;
        if (sel.scan.ifl == -1)
            break;
        xLeft = prc->left + 6;
        yTop = prc->top + 6;
        hbrSav = SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, xLeft, yTop, 70, 2, PATCOPY);
        PatBlt(hdc, xLeft, yTop + 2, 2, 68, PATCOPY);
        PatBlt(hdc, xLeft + 16, yTop + 68, 2, 38, PATCOPY);
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, xLeft + 2, yTop + 68, 15, 1, PATCOPY);
        PatBlt(hdc, xLeft + 1, yTop + 69, 15, 1, PATCOPY);
        PatBlt(hdc, xLeft + 52, yTop + 68, 18, 2, PATCOPY);
        PatBlt(hdc, xLeft + 68, yTop + 2, 2, 66, PATCOPY);
        PatBlt(hdc, xLeft + 69, yTop + 1, 1, 1, PATCOPY);
        PatBlt(hdc, xLeft + 17, yTop + 104, 37, 2, PATCOPY);
        PatBlt(hdc, xLeft + 16, yTop + 105, 1, 1, PATCOPY);
        PatBlt(hdc, xLeft + 52, yTop + 70, 2, 34, PATCOPY);
        PatBlt(hdc, xLeft + 2, yTop + 2, 66, 66, BLACKNESS);
        PatBlt(hdc, xLeft + 18, yTop + 68, 34, 36, BLACKNESS);
        SelectObject(hdc, hbrSav);
        lpfl = rglpfl[sel.scan.ifl];
        DrawFleetBitmap(lpfl, hdc, xLeft + 2, yTop + 2, 0, -1, 0, 0, -1, 0);
        iOffset = rgplr[lpfl->iplr].iPlrBmp;
        SelectPalette(hdc, vhpal, 0);
        RealizePalette(hdc);
        DibBlt(hdc, xLeft + 19, yTop + 71, 32, 32, hdibRaces, (iOffset & 7) * 0x20, (3 - (iOffset >> 3)) * 0x20, 32, 32, 13369376);
        SetTextColor(hdc, crButtonText);
        SetBkColor(hdc, crButtonFace);
        SelectObject(hdc, rghfontArial8[1]);
        cShip = 0;
        for (i = 0; i < 16; i++) {
            cShip += lpfl->rgcsh[i];
        }
        CchGetString(idsShipCountLd, szT);
        c = _wsprintf(szWork, szT, cShip);
        TextOut(hdc, prc->left + 86, yTop, szWork, c);
        yTop += dyArial8 + 2;
        if (lpfl->det == 7 || lpfl->det == 4) {
            cMass = WtFromLpfl(lpfl);
            c = CchGetString(idsFuel2, szT);
            l = GetTextExtent(hdc, szT, c);
            c2 = CchGetString(idsCargo, szWork);
            l2 = GetTextExtent(hdc, szWork, c2);
            if (l2 > l) {
                l = l2;
            }
            SetRect(&rcGauge, prc->left + 90 + LOWORD(l), yTop, prc->right - 4, yTop + dyArial8);
            if (rcGauge.left < rcGauge.right) {
                TextOut(hdc, prc->left + 86, yTop, szT, c);
                yTop += dyArial8 + 2;
                TextOut(hdc, prc->left + 86, yTop, szWork, c2);
                DrawFleetGauge(hdc, &rcGauge, lpfl, 4);
                OffsetRc(&rcGauge, 0, dyArial8 + 4);
                DrawFleetGauge(hdc, &rcGauge, lpfl, 5);
                yTop += dyArial8 + 2;
            }
        } else {
            cMass = lpfl->wtFleet;
        }
        c = CchGetString(gd.fSmallTileMode == 0 ? idsFleetMassLdkt : idsMassLdkt, szT);
        c = _wsprintf(szWork, szT, cMass);
        TextOut(hdc, prc->left + 86, yTop, szWork, c);
        yTop += dyArial8 + 2;
        if (lpfl->det == 7) {
            lpord = lpfl->lpplord->rgord;
            if (lpfl->cord > 1) {
                lpord++;
            }
            CchGetString(gd.fSmallTileMode == 0 ? idsWaypointS : idsWpS, szWP);
            if (lpfl->cord == 1) {
                c = _wsprintf(szT, szWP, PszGetCompressedString(idsNone));
            } else {
                c = _wsprintf(szT, szWP, PszGetLocName(lpord->grobj, lpord->id, lpord->pt.x, lpord->pt.y));
            }
            TextOut(hdc, prc->left + 86, yTop, szT, c);
            yTop += dyArial8 + 2;
            CchGetString(gd.fSmallTileMode == 0 ? idsWaypointTaskS : idsTaskS, szT);
            c = _wsprintf(szWork, szT, PszGetCompressedString(lpord->grTask + 99));
            TextOut(hdc, prc->left + 86, yTop, szWork, c);
            yTop += dyArial8 + 2;
            if (lpfl->cord <= 1) {
                c = CchGetString(gd.fSmallTileMode == 0 ? idsWarpSpeedStopped : idsWarpStopped, szWork);
            } else if (lpord->iWarp == 11) {
                c = CchGetString(idsUseStargate, szWork);
            } else {
                CchGetString(gd.fSmallTileMode == 0 ? idsWarpSpeedD : idsWarpD, szT);
                c = _wsprintf(szWork, szT, lpord->iWarp);
            }
            TextOut(hdc, prc->left + 86, yTop, szWork, c);
            yTop += dyArial8 + 2;
            l = CMineSweepFromLpfl(lpfl);
            if (l <= 0)
                break;
            pszT = PszGetCompressedString(idsFleetCanDestroyLdMinesPerYear);
            c = _wsprintf(szWork, pszT, l);
            TextOut(hdc, prc->left + 86, yTop, szWork, c);
            yTop += dyArial8 + 2;
            break;
        }
        if (lpfl->fdirValid == 0)
            break;
        if (lpfl->iwarpFlt != 0) {
            CchGetString(gd.fSmallTileMode == 0 ? idsWarpSpeedD : idsWarpD, szT);
            c = _wsprintf(szWork, szT, lpfl->iwarpFlt);
        } else {
            c = CchGetString(gd.fSmallTileMode == 0 ? idsWarpSpeedStopped : idsWarpStopped, szWork);
        }
        TextOut(hdc, prc->left + 86, yTop, szWork, c);
        yTop += dyArial8 + 2;
        break;
    case grobjThing:
        if (sel.scan.ith == -1)
            break;
        lpth = lpThings + sel.scan.ith;
        iplrbmp = rgplr[lpth->iplr].iPlrBmp;
        xLeft = prc->left + 6;
        yTop = prc->top + 6;
        hbrSav = SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, xLeft, yTop, 70, 2, PATCOPY);
        PatBlt(hdc, xLeft, yTop + 2, 2, 68, PATCOPY);
        switch (lpth->ith) {
        case ithMinefield:
            ibmp = lpth->thm.iType;
            break;
        case ithMineralPacket:
            ibmp = (lpth->thp.iWarp == 0 ? 0 : 1) + 3;
            break;
        case ithMysteryTrader:
            ibmp = 6;
            break;
        default:
            ibmp = 5;
        }
        SelectPalette(hdc, vhpal, 0);
        RealizePalette(hdc);
        if (lpth->ith != ithWormhole && lpth->ith != ithMysteryTrader) {
            PatBlt(hdc, xLeft + 16, yTop + 68, 2, 38, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, xLeft + 2, yTop + 68, 15, 1, PATCOPY);
            PatBlt(hdc, xLeft + 1, yTop + 69, 15, 1, PATCOPY);
            PatBlt(hdc, xLeft + 52, yTop + 68, 18, 2, PATCOPY);
            PatBlt(hdc, xLeft + 68, yTop + 2, 2, 66, PATCOPY);
            PatBlt(hdc, xLeft + 69, yTop + 1, 1, 1, PATCOPY);
            PatBlt(hdc, xLeft + 17, yTop + 104, 37, 2, PATCOPY);
            PatBlt(hdc, xLeft + 16, yTop + 105, 1, 1, PATCOPY);
            PatBlt(hdc, xLeft + 52, yTop + 70, 2, 34, PATCOPY);
            PatBlt(hdc, xLeft + 2, yTop + 2, 66, 66, BLACKNESS);
            PatBlt(hdc, xLeft + 18, yTop + 68, 34, 36, BLACKNESS);
            DibBlt(hdc, xLeft + 19, yTop + 71, 32, 32, hdibRaces, (iplrbmp & 7) * 0x20, (3 - (iplrbmp >> 3)) * 0x20, 32, 32, 13369376);
        } else {
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, xLeft + 2, yTop + 68, 68, 1, PATCOPY);
            PatBlt(hdc, xLeft + 1, yTop + 69, 69, 1, PATCOPY);
            PatBlt(hdc, xLeft + 68, yTop + 2, 2, 66, PATCOPY);
            PatBlt(hdc, xLeft + 69, yTop + 1, 1, 1, PATCOPY);
            PatBlt(hdc, xLeft + 2, yTop + 2, 66, 66, BLACKNESS);
        }
        SelectObject(hdc, hbrSav);
        DibBlt(hdc, xLeft + 2, yTop + 2, 64, 64, hdibThings, ibmp * 64, 0, 64, 64, 13369376);
        switch (lpth->ith) {
        case ithMineralPacket:
            xLeft += 80;
            SetTextColor(hdc, crButtonText);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            if (lpth->thp.iWarp != 0) {
                CchGetString(idsTravelingWarpD, szT);
                c = _wsprintf(szWork, szT, lpth->thp.iWarp + 4);
                TextOut(hdc, xLeft, yTop, szWork, c);
                yTop += dyArial8 + 2;
                CchGetString(idsDestination, szT);
                psz = PszGetPlanetName(lpth->thp.idPlanet);
                strcat(szT, psz);
                TextOut(hdc, xLeft, yTop, szT, strlen(szT));
            }
            yTop += (int16_t)(3 * dyArial8) / 2;
            xLeft += LOWORD(GetTextExtent(hdc, rgszMinerals[2], strlen(rgszMinerals[2])));
            for (i = 0; i < 3; i++) {
                c = _wsprintf(szWork, PszGetCompressedString(idsS2), rgszMinerals[i]);
                RightTextOut(hdc, xLeft, yTop, szWork, c, 0);
                c = _wsprintf(szWork, PCTDKT, lpth->thp.rgwtMin[i]);
                TextOut(hdc, xLeft, yTop, szWork, c);
                yTop += dyArial8 + 2;
            }
            break;
        case ithWormhole:
            xLeft += 94;
            SetTextColor(hdc, crButtonText);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsLocation, szT);
            xLeft += LOWORD(GetTextExtent(hdc, szT, cch)) + 20;
            RightTextOut(hdc, xLeft - 4, yTop, szT, cch, 0);
            cch = CchGetString(idsDD5, szT);
            c = _wsprintf(szWork, szT, lpth->pt.x, lpth->pt.y);
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += (int16_t)(3 * dyArial8) / 2;
            c = CchGetString(idsDestination2, szWork);
            RightTextOut(hdc, xLeft - 4, yTop, szWork, c, 0);
            if ((1 << idPlayer & lpth->thw.grbitPlrTrav) != 0) {
                lpthDest = LpthFromId(lpth->thw.idPartner);
                if (lpthDest != 0) {
                    c = _wsprintf(szWork, szT, lpthDest->pt.x, lpthDest->pt.y);
                    goto L_1a0b;
                }
            }
            c = CchGetString(idsUnknown2, szWork);
        L_1a0b:
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += (int16_t)(3 * dyArial8) / 2;
            c = CchGetString(idsStability, szWork);
            RightTextOut(hdc, xLeft - 4, yTop, szWork, c, 0);
            c = CchGetString(PctWormholeMoves(lpth) + 967, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            break;
        case ithMysteryTrader:
            xLeft += 80;
            SetTextColor(hdc, crButtonText);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            SetRect(&rc, xLeft, yTop, prc->right - 8, prc->bottom - 8);
            if ((1 << idPlayer & lpth->tht.grbitPlr) == 0) {
                psz = PszGetCompressedString(idsTraderRequestsInterestedPartiesSendFleetLeast);
                yTop += DrawText(hdc, psz, strlen(psz), &rc, 2064) + 8;
            }
            cch = _wsprintf(szWork, PszGetCompressedString(idsTraderTravelingWarpD), lpth->tht.iWarp);
            TextOut(hdc, xLeft, yTop, szWork, cch);
            break;
        case ithMinefield:
            xLeft += 80;
            SetTextColor(hdc, crButtonText);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            CchGetString(idsLocationDD, szT);
            c = _wsprintf(szWork, szT, lpth->pt.x, lpth->pt.y);
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += dyArial8 + 2;
            CchGetString(idsFieldTypeS, szT);
            c = _wsprintf(szWork, szT, rgszMineField[lpth->thm.iType]);
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += dyArial8 + 2;
            CchGetString(idsFieldRadiusDLYLdMines, szT);
            c = _wsprintf(szWork, szT, LOWORD((int32_t)sqrt((double)lpth->thm.cMines)), lpth->thm.cMines);
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += dyArial8 + 2;
            t_merge_1d8c_0001 = GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMines ? 0 : 1;
            pctDecay = (int16_t)((t_merge_1d8c_0001 * 3 + 1) * CPlanetsInCircle(lpth->pt, lpth->thm.cMines) + 2);
            if (pctDecay > 50) {
                pctDecay = 50;
            }
            if (lpth->thm.fDetonate != 0) {
                pctDecay += 25;
            }
            lDecay = (int32_t)(lpth->thm.cMines * pctDecay) / 100;
            if (lDecay < pctDecay) {
                lDecay = pctDecay;
            }
            if (lpth->thm.iType != 2) {
                lDecay = 10 <= lDecay ? lDecay : 10;
            }
            CchGetString(idsDecayRateLdYear, szT);
            c = _wsprintf(szWork, szT, lDecay);
            TextOut(hdc, xLeft, yTop, szWork, c);
            yTop += dyArial8 + 2;
            if (lpth->iplr == idPlayer) {
                GetMineFieldCounts(lpth->idFull, &i, &c2);
                CchGetString(idsFieldDD, szT);
                c = _wsprintf(szWork, szT, i, c2);
                TextOut(hdc, xLeft, yTop, szWork, c);
                yTop += dyArial8 + 2;
            }
        }
        break;
    default:
        if (sel.scan.idpl != -1 && FLookupPlanet(sel.scan.idpl, &pl) != 0) {
            fShortLabels = 0;
            plrSav = rgplr[idPlayer];
            rc = *prc;
            if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raTerra && pl.iPlayer != -1 &&
                (idPlayer == pl.iPlayer || rgplr[idPlayer].rgmdRelation[pl.iPlayer] == 1)) {
                for (c = 0; c < 3; c++) {
                    rgplr[idPlayer].rgEnvVar[c] = LOBYTE((int16_t)(((uint16_t)c & 0xff00) | ((uint16_t)rgplr[pl.iPlayer].rgEnvVar[c] & 0xff)));
                    rgplr[idPlayer].rgEnvVarMin[c] = LOBYTE((int16_t)(((uint16_t)c & 0xff00) | ((uint16_t)rgplr[pl.iPlayer].rgEnvVarMin[c] & 0xff)));
                    rgplr[idPlayer].rgEnvVarMax[c] = LOBYTE((int16_t)(((uint16_t)c & 0xff00) | ((uint16_t)rgplr[pl.iPlayer].rgEnvVarMax[c] & 0xff)));
                }
            }
            SelectObject(hdc, rghfontArial8[0]);
            dxRLabels = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN999mr), 5)) + 6;
            SelectObject(hdc, rghfontArial8[1]);
            dxLabels = LOWORD(GetTextExtent(hdc, rgszPlanetAttr[1], strlen(rgszPlanetAttr[1]))) + 6;
            if (dxLabels * 4 > rc.right) {
                fShortLabels = 1;
                dxLabels = LOWORD(GetTextExtent(hdc, rgszPlanetAttr[1], 4)) + 6;
            }
            xL = rc.left + dxLabels;
            xR = rc.right - dxRLabels;
            dyRow = (int16_t)(rc.bottom - rc.top - dyArial8 * 4 - 2) / 6;
            dyRow = dyRow + 1 & 0xfffe;
            yCur = rc.top + 2;
            if (pl.fStarbase != 0) {
                hdcMem = CreateCompatibleDC(hdc);
                hbmpSav = SelectObject(hdcMem, hbmpMono);
                crTextSav = SetTextColor(hdc, 0);
                crBkSav = SetBkColor(hdc, 0xffffff);
                BitBlt(hdc, rc.right - 22, yCur + 2, 13, 16, hdcMem, 0, 12, SRCAND);
                SetTextColor(hdc, 0xffff);
                SetBkColor(hdc, 0);
                BitBlt(hdc, rc.right - 22, yCur + 2, 13, 16, hdcMem, 0, 12, SRCPAINT);
                SetTextColor(hdc, crTextSav);
                SetBkColor(hdc, crBkSav);
                SelectObject(hdcMem, hbmpSav);
                DeleteDC(hdcMem);
            }
            if (pl.det >= 3) {
                c = CchGetString((fShortLabels == 0 ? 1 : 0) + 548, szWork);
                dx = LOWORD(GetTextExtent(hdc, szWork, c));
                SetTextColor(hdc, crButtonText);
                TextOut(hdc, xL, yCur, szWork, c);
                dNum = PctPlanetDesirability(&pl, idPlayer);
                c = _wsprintf(szWork, PCTDPCTPCT, dNum);
                SetTextColor(hdc, dNum >= 0 ? 32512 : 0xff);
                TextOut(hdc, xL + dx, yCur, szWork, c);
                if (fShortLabels == 0) {
                    dx += LOWORD(GetTextExtent(hdc, szWork, c));
                    dBest = PctPlanetOptValue(&pl, idPlayer);
                    if (dBest > dNum) {
                        if (dBest > 0) {
                            SetTextColor(hdc, dBest > 10 ? 32512 : 32639);
                        }
                        c = _wsprintf(szWork, " (%d%%)", dBest);
                        TextOut(hdc, xL + dx, yCur, szWork, c);
                    }
                }
            }
            SetTextColor(hdc, crButtonText);
            if (pl.iPlayer != -1) {
                strcpy(szT, PszGetCompressedString((fShortLabels == 0 ? 1 : 0) + 546));
            }
            if (pl.det == 7) {
                c = strlen(szT);
                c += CommaFormatLong(&szT[c], (uint32_t)(pl.rgwtMin[3] * 100));
                RightTextOut(hdc, xR, yCur, szT, c, 0);
            } else if (pl.iPlayer == -1) {
                c = CchGetString(idsUninhabited, szT);
                RightTextOut(hdc, xR, yCur, szT, c, 0);
            } else {
                if (pl.det >= 3) {
                    l = (int32_t)(pl.uPopGuess * 4);
                    strcpy(szWork, szT);
                    c = strlen(szT);
                    if (l > 0) {
                        c += _wsprintf(&szWork[c], PszGetCompressedString(idsCLd00), 177, l);
                    } else {
                        c += CchGetString(idsMsg1264, &szWork[c]);
                    }
                    RightTextOut(hdc, xR, yCur, szWork, c, 0);
                }
                SetTextColor(hdc, 0xff);
                RightTextOut(hdc, xR, yCur + dyArial8 - 2, PszPlayerName(pl.iPlayer, 0, 1, 0, 0, NULL), 0, 0);
            }
            yCur += dyArial8 - 2;
            dNum = game.turn - pl.turn;
            if (dNum == 0) {
                c = _wsprintf(szWork, PszGetCompressedString(fShortLabels == 0 ? idsReportCurrent : idsCurrent));
            } else if (fShortLabels != 0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsOld2), dNum);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsReportDYear), dNum);
                if (dNum > 1) {
                    strcat(szWork, "s");
                    c++;
                }
                c += CchGetString(idsOld, &szWork[c]);
            }
            SetTextColor(hdc, dNum > 5 ? 0xff : 0);
            TextOut(hdc, xL, yCur, szWork, c);
            yCur += dyArial8;
            hbrSav = SelectObject(hdc, hbrButtonShadow);
            PatBlt(hdc, xL, yCur, xR - xL, 1, PATCOPY);
            PatBlt(hdc, xL + 1, yCur + 1, xR - xL - 2, 3 * dyRow - 1, BLACKNESS);
            PatBlt(hdc, xL, yCur, 1, 3 * dyRow, PATCOPY);
            PatBlt(hdc, xL + 2, yCur + dyRow, xR - xL - 4, 1, PATCOPY);
            PatBlt(hdc, xL + 2, dyRow * 2 + yCur, xR - xL - 4, 1, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, xR - 1, yCur + 1, 1, 3 * dyRow, PATCOPY);
            PatBlt(hdc, xL, 3 * dyRow + yCur, xR - xL, 1, PATCOPY);
            SetTextColor(hdc, crButtonText);
            SetBkMode(hdc, TRANSPARENT);
            fCanTerraform = FCanTerraformLppl(&pl, rgMin, rgMax, rgCost, 1);
            dx = xR - xL - 4;
            dy = (dyRow - dyArial8) >> 1;
            i = 0;
            while (i < 3) {
                iMin = rgplr[idPlayer].rgEnvVarMin[i];
                iMax = rgplr[idPlayer].rgEnvVarMax[i];
                iCur = pl.rgEnvVar[i];
                RightTextOut(hdc, xL - 2, yCur + dy, fShortLabels == 0 ? rgszPlanetAttr[i] : rgszPlanetAttrAbbr[i], 0, 0);
                if (pl.det >= 3) {
                    psz = PszCalcEnvVar(i, iCur);
                    c = strlen(psz);
                    SelectObject(hdc, rghfontArial8[0]);
                    TextOut(hdc, xR + 2, yCur + dy, psz, c);
                }
                SelectObject(hdc, rghfontArial8[1]);
                SelectObject(hdc, rghbrPlanetAttr[i][0]);
                PatBlt(hdc, xL + 2 + MulDiv(iMin, dx, 100), yCur + 2, MulDiv(iMax - iMin, dx, 100), dyRow - 3, PATCOPY);
                if (pl.det >= 3) {
                    SelectObject(hdc, rghbrPlanetAttr[i][1]);
                    pt.x = xL + 2 + MulDiv(pl.rgEnvVarOrig[i], dx, 100);
                    pt.y = dyRow / 2 + yCur;
                    c = dyRow / 4 - 1;
                    if (c < 2) {
                        c = 2;
                    }
                    PatBlt(hdc, pt.x - c, pt.y, c * 2 + 1, 1, PATCOPY);
                    PatBlt(hdc, pt.x, pt.y - c, 1, c * 2 + 1, PATCOPY);
                    yTop = yCur + 3;
                    yBot = yCur + dyRow - 3;
                    c = 1;
                    xBeg = xL + 2 + MulDiv(iCur, dx, 100);
                    while (1) {
                        PatBlt(hdc, xBeg, yTop, 1, 1, PATCOPY);
                        yTop++;
                        PatBlt(hdc, xBeg + c - 1, yTop, 1, 1, PATCOPY);
                        if (yTop > yBot)
                            break;
                        PatBlt(hdc, xBeg, yBot, 1, 1, PATCOPY);
                        yBot--;
                        PatBlt(hdc, xBeg + c - 1, yBot, 1, 1, PATCOPY);
                        xBeg--;
                        c += 2;
                    }
                    if (fCanTerraform != 0) {
                        yTop--;
                        if (rgMin[i] != -1) {
                            dxBar = 0 <= iCur - rgMin[i] ? iCur - rgMin[i] : 0;
                        } else {
                            dxBar = 0;
                        }
                        xBeg = xL + 2 + MulDiv(iCur - dxBar, dx, 100);
                        if (rgMax[i] != -1) {
                            dxBar = 0 <= rgMax[i] - iCur ? rgMax[i] - iCur : 0;
                        } else {
                            dxBar = 0;
                        }
                        xEnd = xL + 2 + MulDiv(iCur + dxBar, dx, 100);
                        PatBlt(hdc, xBeg, yTop, xEnd - xBeg + 1, 1, PATCOPY);
                    }
                }
                i++;
                yCur += dyRow;
            }
            yCur += dyArial8 >> 1;
            SelectObject(hdc, hbrButtonShadow);
            PatBlt(hdc, xL, yCur, xR - xL, 1, PATCOPY);
            PatBlt(hdc, xL + 1, yCur + 1, xR - xL - 2, 3 * dyRow + 1, BLACKNESS);
            PatBlt(hdc, xL, yCur, 1, 3 * dyRow + 2, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, xR - 1, yCur + 1, 1, 3 * dyRow + 2, PATCOPY);
            PatBlt(hdc, xL, 3 * dyRow + yCur + 2, xR - xL, 1, PATCOPY);
            SelectObject(hdc, rghfontArial7[0]);
            c = _wsprintf(szWork, PCTD, cMinGrafMax);
            dxNum = LOWORD(GetTextExtent(hdc, szWork, c));
            dxBar = xR - xL - 3;
            cNum = dxBar / ((dxNum >> 1) + dxNum);
            dNum = cMinGrafMax / cNum;
            if (cMinGrafMax < 500) {
                dNum = (int16_t)(dNum + 49) / 50 * 10;
            } else if (cMinGrafMax < 1000) {
                dNum = (int16_t)(dNum + 49) / 50 * 50;
            } else if (cMinGrafMax < 2500) {
                dNum = (int16_t)(dNum + 99) / 100 * 100;
            } else if (cMinGrafMax < 7500) {
                dNum = (int16_t)(dNum + 249) / 250 * 250;
            } else if (cMinGrafMax < 15000) {
                dNum = (int16_t)(dNum + 499) / 500 * 500;
            } else {
                dNum = (int16_t)(dNum + 999) / 1000 * 1000;
            }
            cNum = cMinGrafMax / dNum;
            SetTextColor(hdc, crButtonText);
            dy = 3 * dyRow + yCur + 4;
            for (i = 0; i <= cNum; i++) {
                xBeg = LOWORD((int32_t)((int32_t)((uint32_t)(i * dNum) * dxBar) / cMinGrafMax)) + xL;
                PatBlt(hdc, xBeg, yCur + 2, 1, 3 * dyRow - 1, PATCOPY);
                c = _wsprintf(szWork, PCTD, i * dNum);
                CtrTextOut(hdc, xBeg, dy, szWork, c);
            }
            RightTextOut(hdc, xL - 4, dy, "kT", 2, 0);
            if (pl.iPlayer == idPlayer) {
                EstMineralsMined(&pl, rgl, -1, 0);
            } else {
                rgl[2] = 0;
                rgl[1] = 0;
                rgl[0] = 0;
                if (pl.iPlayer == -1) {
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->idPlanet == pl.id && lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->lpplord->rgord[0].grTask == grTaskMine) {
                            cMines = CMineFromLpfl(lpfl);
                            if (cMines > 0) {
                                EstMineralsMined(&pl, rglT, cMines, 0);
                                for (iT = 0; iT < 3; iT++) {
                                    rgl[iT] += rglT[iT];
                                }
                            }
                        }
                    }
                }
            }
            SelectObject(hdc, rghfontArial8[1]);
            dy = ((dyRow - dyArial8) >> 1) + 2;
            i = 0;
            while (i <= 2) {
                SetTextColor(hdc, rgcrMinerals[i]);
                if (fShortLabels != 0) {
                    t_merge_3357_0001 = 4;
                } else {
                    t_call_334f = strlen(rgszMinerals[i]);
                    t_merge_3357_0001 = t_call_334f;
                }
                RightTextOut(hdc, xL - 2, yCur + dy, rgszMinerals[i], t_merge_3357_0001, 0);
                t_merge_33e5_0001 = cMinGrafMax < rgl[i] + pl.rgwtMin[i] ? cMinGrafMax : rgl[i] + pl.rgwtMin[i];
                dx = LOWORD((int32_t)((int32_t)(dxBar * t_merge_33e5_0001) / cMinGrafMax));
                SetRect(&rcGauge, xL + 1, yCur + 4, xL + dx + 1, yCur + dyRow - 1);
                if (dx != 0) {
                    FillRect(hdc, &rcGauge, rghbrMinSum[i][1]);
                }
                if ((int32_t)(rgl[i] + pl.rgwtMin[i]) > cMinGrafMax) {
                    SetTextColor(hdc, crButtonText);
                    TextOut(hdc, xR, yCur + dy, "+", 1);
                }
                t_merge_3519_0001 = cMinGrafMax < pl.rgwtMin[i] ? cMinGrafMax : pl.rgwtMin[i];
                dx = LOWORD((int32_t)((int32_t)(dxBar * t_merge_3519_0001) / cMinGrafMax));
                SetRect(&rcGauge, xL + 1, yCur + 4, xL + dx + 1, yCur + dyRow - 1);
                if (dx != 0) {
                    FillRect(hdc, &rcGauge, rghbrMinSum[i][0]);
                }
                if (pl.det >= 3) {
                    for (iPass = 0; iPass < 2; iPass++) {
                        SelectObject(hdc, rghbrMinSum[i][iPass]);
                        yTop = yCur + 4;
                        yBot = yCur + dyRow - 2;
                        c = 1;
                        xBeg = xL + 1 + iPass + MulDiv(100 >= pl.rgMinConc[i] ? pl.rgMinConc[i] : 100, dxBar - 8, 100);
                        while (1) {
                            yTop++;
                            PatBlt(hdc, xBeg, yTop, c, 1, PATCOPY);
                            if (yTop > yBot)
                                break;
                            xBeg--;
                            yBot--;
                            PatBlt(hdc, xBeg, yBot, c, 1, PATCOPY);
                            c += 2;
                        }
                    }
                }
                i++;
                yCur += dyRow;
            }
            SetBkMode(hdc, OPAQUE);
            SelectObject(hdc, hbrSav);
            rgplr[idPlayer] = plrSav;
        } else if (sel.scan.idpl != -1) {
            hbmpSav = SelectObject(hdcMem, hbmpUnknownPlanet);
            SetTextColor(hdc, 0);
            BitBlt(hdc, ((prc->right - prc->left - 0x40) >> 1) + prc->left, ((prc->bottom - prc->top - 0x40) >> 1) + prc->top, 64, 64, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hbmpSav);
        }
    case grobjNone:
        break;
    }
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

    grobj = sel.scan.grobj;
    if (grobj == grobjOther) {
        if ((sel.scan.grobjFull & 1) != 0) {
            grobj = grobjPlanet;
        } else if ((sel.scan.grobjFull & 2) != 0) {
            grobj = grobjFleet;
        } else {
            grobj = grobjNone;
        }
    }
    GetClientRect(hwnd, &rc);
    rc.top += dyArial8 * 2;
    if (y < rc.top - 5 && y > 4 && x < rc.right - 4 && x > rc.right - rc.top && FOtherStuffAtScanSel() != 0) {
        return htMineScanSel;
    }
    if (grobj == grobjFleet || (grobj == grobjThing && lpThings[sel.scan.ith].ith != ithWormhole && lpThings[sel.scan.ith].ith != ithMysteryTrader)) {
        if (x >= rc.left + 25 && x < rc.left + 57 && y >= rc.top + 71 && y < rc.top + 103) {
            return htMineOwner;
        }
        if (grobj == grobjThing && lpThings[sel.scan.ith].ith == ithMinefield && x >= rc.left + 9 && x < rc.left + 73 && y >= rc.top + 9 && y < rc.top + 73) {
            return htMineMinefieldType;
        }
        if (grobj == grobjThing) {
            return htMineNone;
        }
        if (x >= rc.left + 9 && x < rc.left + 73 && y >= rc.top + 9 && y < rc.top + 73) {
            return htMineShipOrFleet;
        }
        if (x >= rc.left + 92 && y >= rc.top + 4 && y < rc.top + 7 + dyArial8) {
            return htMineShipOrFleet;
        }
        return htMineNone;
    }
    if (sel.scan.idpl == -1 || FLookupPlanet(sel.scan.idpl, &pl) == 0) {
        return htMineNone;
    }
    rc.top -= 4;
    yCur = rc.top + 2;
    dyRow = (int16_t)(rc.bottom - rc.top - dyArial8 * 4 - 2) / 6;
    dyRow = dyRow + 1 & 0xfffe;
    yCur += dyArial8 * 2 - 2;
    if (y < yCur) {
        if (y > rc.top) {
            if (x < rc.right - 24) {
                if (pl.iPlayer != idPlayer && x > (int16_t)(3 * rc.right) / 5 && y >= yCur - dyArial8 && pl.iPlayer != -1) {
                    return htMineOwner;
                }
                return htMinePlanet;
            }
            if (pl.fStarbase != 0) {
                return htMineStarbase;
            }
            return htMineNone;
        }
        return htMineNone;
    }
    if (y < 3 * dyRow + yCur) {
        return (int16_t)(y - yCur) / dyRow + 6;
    }
    yCur += 3 * dyRow + (dyArial8 >> 1) + 1;
    if (y < 3 * dyRow + yCur) {
        return (int16_t)(y - yCur) / dyRow + 1;
    }
    return htMineScale;
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

    ht = HtMineWindow(hwndMine, x, y);
    if ((msg != 516 || ht == htMineScanSel || ht == htMineShipOrFleet) && ht <= htMineMinefieldType) {
        switch (ht) {
        case 10:
            if (sel.scan.grobj == grobjPlanet) {
                lppl = LpplFromId(sel.scan.idpl);
                GlobalPD.iPlayer = lppl->iPlayer;
            } else if (sel.scan.grobj == grobjThing) {
                GlobalPD.iPlayer = lpThings[sel.scan.ith].iplr;
            } else {
                GlobalPD.iPlayer = rglpfl[sel.scan.ifl]->iplr;
            }
            GlobalPD.grPopup = grPopupPlayer;
            Popup(hwndMine, x, y);
            break;
        case 14:
            part.hs.iItem = mpiTypeiItem[lpThings[sel.scan.ith].thm.iType];
            part.hs.grhst = hstMines;
            FLookupPart(&part);
            GlobalPD.grPopup = grPopupComponent;
            GlobalPD.part = part;
            Popup(hwndMine, x, y);
            break;
        case 11:
            if (msg == 516) {
                lpfl = rglpfl[sel.scan.ifl];
                c = 0;
                for (ishdef = 0; ishdef < 16; ishdef++) {
                    if (lpfl->rgcsh[ishdef] > 0) {
                        rgid[c] = ishdef;
                        fstrcpy(rgsz[c], rglpshdef[lpfl->iPlayer][ishdef].hul.szClass);
                        rgpsz[c] = rgsz[c];
                        c++;
                    }
                }
                if (c > 1) {
                    c = PopupMenu(hwndMine, x, y, c, NULL, rgpsz, -1, 1);
                    if (c == -1)
                        break;
                } else {
                    c = 0;
                }
                GlobalPD.grPopup = grPopupShdef;
                GlobalPD.lpshdef = rglpshdef[lpfl->iPlayer] + rgid[c];
                GlobalPD.fHideCounts = idPlayer == lpfl->iPlayer ? 0 : 1;
                GlobalPD.fShowDamage = 0;
                GlobalPD.fToken = 0;
                GlobalPD.fSummary = 1;
            } else {
                GlobalPD.grPopup = grPopupFleet;
                GlobalPD.lpfl = rglpfl[sel.scan.ifl];
                GlobalPD.fRedDamage = GlobalPD.lpfl->det == 7 ? 1 : 0;
                GlobalPD.grbit = 0xff;
            }
            Popup(hwndMine, x, y);
            break;
        case 13:
            GlobalPD.grPopup = grPopupShdef;
            lppl = LpplFromId(sel.scan.idpl);
            GlobalPD.lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
            GlobalPD.fHideCounts = idPlayer == lppl->iPlayer ? 0 : 1;
            GlobalPD.fShowDamage = 1;
            GlobalPD.fToken = 0;
            GlobalPD.fSummary = 1;
            Popup(hwndMine, x, y);
            break;
        case 12:
            GlobalPD.grPopup = grPopupPlanet;
            GlobalPD.idPlanet = sel.scan.idpl;
            Popup(hwndMine, x, y);
            break;
        case 6:
        case 7:
        case 8:
            FLookupPlanet(sel.scan.idpl, &pl);
            GlobalPD.grPopup = grPopupPlanetEnv;
            GlobalPD.idPlanet = pl.id;
            GlobalPD.iPlanetVar = ht - 6;
            if (pl.det >= 3) {
                GlobalPD.iPlanVal = pl.rgEnvVar[GlobalPD.iPlanetVar];
            } else {
                GlobalPD.iPlanVal = -1;
            }
            if (pl.det >= 3 && FCanTerraformLppl(&pl, rgMin, rgMax, rgCost, 1) != 0) {
                GlobalPD.iPlanMin = rgMin[GlobalPD.iPlanetVar];
                GlobalPD.iPlanMax = rgMax[GlobalPD.iPlanetVar];
                if (GlobalPD.iPlanMin == -1) {
                    GlobalPD.iPlanMin = GlobalPD.iPlanVal;
                }
                if (GlobalPD.iPlanMax == -1) {
                    GlobalPD.iPlanMax = GlobalPD.iPlanVal;
                }
                if (GlobalPD.iPlanMin != GlobalPD.iPlanMax)
                    goto L_401c;
            }
            GlobalPD.iPlanMin = -1;
            GlobalPD.iPlanMax = -1;
        L_401c:
            GlobalPD.iPlrVal = rgplr[idPlayer].rgEnvVar[GlobalPD.iPlanetVar];
            GlobalPD.iPlrMin = rgplr[idPlayer].rgEnvVarMin[GlobalPD.iPlanetVar];
            GlobalPD.iPlrMax = rgplr[idPlayer].rgEnvVarMax[GlobalPD.iPlanetVar];
            Popup(hwndMine, x, y);
            break;
        case 9:
            if (msg == 516) {
                PopupMineralScanChoices(hwndMine, x, y);
                break;
            }
            scan = sel.scan;
            scan.iwp = 0;
            if (scan.grobj == grobjThing) {
                i = scan.ith + 1;
                goto L_4113;
            }
        L_4192:
            for (i = scan.grobj == grobjFleet ? scan.ifl + 1 : 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y))
                    break;
            }
            if (i < cFleet) {
                scan.ifl = i;
                scan.grobj = grobjFleet;
                idNew = rglpfl[i]->id;
                fOurs = rglpfl[i]->iPlayer == idPlayer ? 1 : 0;
                goto ChangeIt;
            }
            if ((scan.grobjFull & 8) != 0) {
                i = 0;
                goto L_4113;
            }
            if ((scan.grobjFull & 1) != 0)
                goto CheckPlanet;
        CheckFleet:
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y))
                    break;
            }
            if (i != cFleet || (scan.grobjFull & 8) == 0) {
                scan.grobj = grobjFleet;
                scan.ifl = i;
                idNew = rglpfl[i]->id;
                fOurs = rglpfl[i]->iPlayer == idPlayer ? 1 : 0;
                goto ChangeIt;
            }
            i = 0;
        L_4113:
            while (1) {
                if (i >= cThing || (lpThings[i].pt.x == scan.pt.x && lpThings[i].pt.y == scan.pt.y)) {
                    if (i < cThing)
                        break;
                    if ((scan.grobjFull & 1) != 0)
                        goto CheckPlanet;
                    if ((scan.grobjFull & 2) != 0)
                        goto CheckFleet;
                    if ((scan.grobjFull & 8) == 0)
                        goto L_4192;
                    i = 0;
                } else {
                    i++;
                }
            }
            scan.ith = i;
            scan.grobj = grobjThing;
            idNew = lpThings[i].idFull;
            fOurs = 0;
            goto ChangeIt;
        CheckPlanet:
            scan.grobj = grobjPlanet;
            idNew = scan.idpl;
            lppl = LpplFromId(idNew);
            if (lppl == 0) {
                fOurs = 0;
            } else {
                fOurs = lppl->iPlayer == idPlayer ? 1 : 0;
            }
        ChangeIt:
            if (fOurs == 0 || sel.grobj != grobjFleet) {
                scan.iwp = sel.scan.iwp;
            }
            ChangeScanSel(&scan, 2);
            if (fOurs == 0)
                break;
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(scan.grobj, idNew);
            RedrawScanSel(NULL, 1);
            break;
        case 5:
            iChecked = -1;
            rgi[0] = 100;
            rgi[1] = 500;
            rgi[2] = 1000;
            rgi[3] = 2500;
            rgi[4] = 5000;
            rgi[5] = 7500;
            rgi[6] = 10000;
            rgi[7] = 20000;
            rgi[8] = 30000;
            for (i = 0; i < 9; i++) {
                _wsprintf(rgsz[i], PCTDKT, rgi[i]);
                psz[i] = rgsz[i];
                if (rgi[i] == cMinGrafMax) {
                    iChecked = i;
                }
            }
            i = PopupMenu(hwndMine, x, y, 9, NULL, psz, iChecked, 1);
            if (i == -1 || rgi[i] == cMinGrafMax)
                break;
            cMinGrafMax = rgi[i];
            InvalidateRect(hwndMine, NULL, 1);
            if ((grbitScan & 0xf) != 1)
                break;
            InvalidateRect(hwndScanner, NULL, 1);
            break;
        case 1:
        case 2:
        case 3:
            FLookupPlanet(sel.scan.idpl, &pl);
            GlobalPD.grPopup = grPopupMineral;
            GlobalPD.rgi[0] = (int16_t)(ht - 1);
            for (i = 1; i <= 4; i++) {
                GlobalPD.rgi[i] = -1;
            }
            if (pl.det >= 3) {
                GlobalPD.rgi[3] = (uint32_t)pl.rgpctMinLevel[ht + 2];
                GlobalPD.rgi[1] = pl.fHomeworld;
                if (pl.det > 3) {
                    lVal = 0;
                    GlobalPD.rgi[2] = pl.rgwtMin[ht - 1];
                    EstMineralsMined(&pl, rglQuan, -1, 0);
                    GlobalPD.rgi[4] = rglQuan[ht - 1];
                    if (pl.iPlayer == -1) {
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == pl.id && lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->lpplord->rgord[0].grTask == grTaskMine) {
                                cMines = CMineFromLpfl(lpfl);
                                if (cMines > 0) {
                                    EstMineralsMined(&pl, rglT, cMines, 0);
                                    lVal += rglT[ht - 1];
                                }
                            }
                        }
                        if (lVal > 0) {
                            GlobalPD.rgi[4] = lVal;
                        }
                    }
                }
            }
            Popup(hwndMine, x, y);
        }
    }
    return;
}

void SetMineralTitleBar(HWND hwnd) {
    char       szDeepSpace[40];
    char       szSummary[40];
    int16_t    fVisCB;
    char      *psz;
    GrobjClass grobj;
    RECT       rc;

    fVisCB = 0;
    CchGetString(idsDeepSpace, szDeepSpace);
    CchGetString(idsSummary, szSummary);
    grobj = sel.scan.grobj == grobjOther ? sel.scan.grobjFull : sel.scan.grobj;
    if ((grobj & 1) != 0) {
        if (sel.scan.idpl != -1) {
            psz = PszGetPlanetName(sel.scan.idpl);
            strcat(psz, szSummary);
        }
    } else if ((grobj & 2) != 0) {
        if (sel.scan.ifl != -1) {
            psz = PszGetFleetName(rglpfl[sel.scan.ifl]->id);
            strcat(psz, szSummary);
        }
    } else if ((grobj & 8) == 0) {
        psz = szDeepSpace;
    } else if (sel.scan.ith != -1) {
        fVisCB = 1;
        psz = PszGetThingName(lpThings[sel.scan.ith].idFull);
        strcat(psz, szSummary);
    }
    strcpy(szMineralTitle, psz);
    InvalidateRect(hwnd, NULL, 1);
    if (fVisCB != 0) {
        fVisCB = lpThings[sel.scan.ith].ith == ithMinefield && lpThings[sel.scan.ith].thm.iType == 0 && lpThings[sel.scan.ith].iplr == idPlayer &&
                 GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMines;
    }
    if (fVisCB != 0) {
        SendMessage(hwndMineCB, BM_SETCHECK, lpThings[sel.scan.ith].thm.fDetonate == 0 ? 0 : 1, 0);
    }
    GetClientRect(hwnd, &rc);
    SetWindowPos(hwndMineCB, NULL, 80, rc.bottom - dyArial8 * 2, rc.right - 88, dyArial8 + 4, (fVisCB == 0 ? 0x80 : 0x40) | 4);
    return;
}

void DrawSelectionArrow(HDC hdc, RECT *prc, int16_t fEnabled) {
    HBITMAP hbmpSav;
    HDC     hdcMem;
    int16_t xCtr;

    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpScanner);
    xCtr = (int16_t)(prc->right - prc->left) / 2 + prc->left;
    BitBlt(hdc, xCtr - 5, ((prc->bottom - prc->top) >> 1) + prc->top - 5, 11, 12, hdcMem, 35, 57, SRCAND);
    BitBlt(hdc, xCtr - 5, ((prc->bottom - prc->top) >> 1) + prc->top - 5, 11, 12, hdcMem, fEnabled == 0 ? 0 : 24, fEnabled == 0 ? 45 : 57, SRCPAINT);
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

    xCtr = (int16_t)(prc->right - prc->left) / 2 + prc->left;
    yTop = prc->top + 1;
    yBot = prc->bottom - 2;
    xCur = xCtr - 1;
    hbrSav = SelectObject(hdc, hbrButtonHilite);
    while (yTop <= yBot) {
        yTop++;
        PatBlt(hdc, xCur, yTop, 2, 1, PATCOPY);
        xCur--;
        yBot--;
        PatBlt(hdc, xCur, yBot, 2, 1, PATCOPY);
    }
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, xCtr, prc->top, 1, 1, PATCOPY);
    PatBlt(hdc, xCtr, prc->bottom - 1, 1, 1, PATCOPY);
    yTop = prc->top + 1;
    yBot = prc->bottom - 2;
    xCur = xCtr;
    while (yTop <= yBot) {
        yTop++;
        PatBlt(hdc, xCur, yTop, 2, 1, PATCOPY);
        xCur++;
        yBot--;
        PatBlt(hdc, xCur, yBot, 2, 1, PATCOPY);
    }
    yTop = prc->top + 4;
    yBot = prc->bottom - 5;
    xCur = xCtr;
    dx = 1;
    SelectObject(hdc, hbr);
    while (yTop <= yBot) {
        yTop++;
        PatBlt(hdc, xCur, yTop, dx, 1, PATCOPY);
        xCur--;
        yBot--;
        PatBlt(hdc, xCur, yBot, dx, 1, PATCOPY);
        dx += 2;
    }
    SelectObject(hdc, hbrSav);
    return;
}

int16_t FOtherStuffAtScanSel() {
    int16_t c;
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    THING  *lpthMac;

    if (sel.scan.idpl != -1 && (sel.scan.ifl != -1 || sel.scan.ith != -1)) {
        return 1;
    }
    if (sel.scan.ifl != -1) {
        if (sel.scan.ith != -1) {
            return 1;
        }
        c = 1;
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpfl->pt.x == sel.scan.pt.x && lpfl->pt.y == sel.scan.pt.y && c-- == 0) {
                return 1;
            }
        }
    }
    if (sel.scan.ith != -1) {
        c = 1;
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->pt.x == sel.scan.pt.x && lpth->pt.y == sel.scan.pt.y && c-- == 0) {
                return 1;
            }
        }
    }
    return 0;
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
    int16_t  t_50dd;
    int32_t *t_assign_1;
    int32_t *t_assign_2;

    iChecked = -1;
    if (sel.scan.idpl != -1) {
        rgid[0] = sel.scan.idpl;
        rgid[1] = -1;
        c = 2;
        if (sel.scan.grobj == grobjPlanet) {
            iChecked = 0;
        }
    } else {
        c = 0;
    }
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if (sel.scan.pt.x == lpfl->pt.x && sel.scan.pt.y == lpfl->pt.y) {
            if (sel.scan.grobj == grobjFleet && rglpfl[sel.scan.ifl]->id == lpfl->id) {
                iChecked = c;
            }
            rgid[c++] = lpfl->id | 0x80000000;
            if (c >= 100)
                break;
        }
    }
    if (c == 2 && sel.scan.idpl != -1) {
        c = 1;
    }
    fSep = c == 0 ? 1 : 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (sel.scan.pt.x == lpth->pt.x && sel.scan.pt.y == lpth->pt.y) {
            if (fSep == 0) {
                rgid[c++] = -1;
                fSep = 1;
            }
            if (sel.scan.grobj == grobjThing && (int16_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18 == sel.scan.ith) {
                iChecked = c;
            }
            t_50dd = c;
            c++;
            t_assign_1 = &rgid[t_50dd];
            *t_assign_1 = (int32_t)(((uint32_t)*t_assign_1 & 0xffff0000) | ((uint32_t)lpth->idFull & 0xffff));
            t_assign_2 = &rgid[t_50dd];
            *t_assign_2 = (int32_t)(((uint32_t)*t_assign_2 & 0xffff) | ((uint32_t)0x2000 & 0xffff) << 0x10);
        }
    }
    i = PopupMenu(hwnd, x, y, c, rgid, NULL, iChecked, 1);
    if (i >= 0) {
        scan = sel.scan;
        if ((rgid[i] & 0x80000000) != 0) {
            scan.grobj = grobjFleet;
            id = LOWORD(rgid[i]);
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || lpfl->id == id)
                    break;
            }
            scan.ifl = i;
            idNew = id;
            fOurs = lpfl->iPlayer == idPlayer ? 1 : 0;
        } else if ((rgid[i] & 0x20000000) != 0) {
            scan.grobj = grobjThing;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac && lpth->idFull != LOWORD(rgid[i]); lpth++) {
            }
            scan.ith = (int16_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 18;
            idNew = lpth->idFull;
            fOurs = 0;
        } else {
            scan.grobj = grobjPlanet;
            idNew = sel.scan.idpl;
            lppl = LpplFromId(idNew);
            fOurs = lppl->iPlayer == idPlayer ? 1 : 0;
        }
        ChangeScanSel(&scan, 2);
        if (fOurs != 0) {
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(scan.grobj, idNew);
            RedrawScanSel(NULL, 1);
        }
    }
    return;
}

void EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply) {
    int32_t lQuanRem;
    int32_t lQuanAct;
    int16_t i;
    int32_t lQuan;
    int16_t fMacintosh;
    int16_t fRemote;
    int32_t lMine;
    int32_t lMineEff;
    int32_t lConc;
    int32_t lLeft;
    int32_t lLevel;
    int32_t lLength;
    int32_t rglQuan[3];
    int16_t ifl;
    FLEET  *lpfl;
    int16_t t_scratch_m22;

    fRemote = cMines == -1 ? 0 : 1;
    fMacintosh = lppl->iPlayer != -1 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh;
    if (cMines == -1) {
        if (lppl->iPlayer == -1 || lppl->rgwtMin[3] == 0) {
            for (i = 0; i < 3; i++) {
                plQuan[i] = -1;
            }
            return;
        }
        lMine = CMinesOperating(lppl);
        if (fMacintosh != 0) {
            lMineEff = 10;
        } else {
            lMineEff = GetRaceStat(&rgplr[lppl->iPlayer], rsMineProd);
        }
    } else {
        lMine = cMines;
        lMineEff = 10;
    }
    for (i = 0; i <= 2; i++) {
        lConc = (uint32_t)lppl->rgMinConc[i];
        if (lConc < 30 && lppl->fHomeworld != 0 && (fRemote == 0 || fMacintosh != 0)) {
            lConc = 30;
        }
        lQuanAct = (uint32_t)(lMine * lConc);
        if (fRemote == 0) {
            lQuan = (int32_t)(lQuanAct * lMineEff) / 10;
        } else {
            lQuan = lQuanAct;
        }
        lQuanRem = (int32_t)(lQuan % 100);
        lQuan = (int32_t)(lQuan / 100);
        if (lQuanRem != 0 && gd.fGeneratingTurn != 0) {
            t_scratch_m22 = Random(100);
            if (t_scratch_m22 < LOWORD(lQuanRem)) {
                lQuan++;
            }
        }
        plQuan[i] = lQuan;
        if (fApply != 0) {
            lppl->rgwtMin[i] += lQuan;
            lQuanAct = (int32_t)(lQuanAct / 100);
            while (lQuanAct > 0 && lppl->rgMinConc[i] > 1) {
                lLevel = (uint32_t)lppl->rgpctMinLevel[i];
                lConc = (uint32_t)lppl->rgMinConc[i];
                if (lLevel == 0) {
                    lLevel = 256;
                }
                if (lConc > 100) {
                    lConc = 100;
                } else if (lConc < 5) {
                    lConc = 10;
                } else if (lConc < 25) {
                    lConc = 25;
                }
                lLeft = (int32_t)((int32_t)(lLevel * 12500) / 256 / lConc);
                if (lLeft <= lQuanAct) {
                    lQuanAct -= lLeft;
                    lppl->rgMinConc[i]--;
                    lppl->rgpctMinLevel[i] = 0;
                } else {
                    lLength = (int32_t)(12500 / lConc);
                    lLeft = (int32_t)((int32_t)((lLeft - lQuanAct) * 0x100) / lLength);
                    if (lLeft < 1) {
                        lLeft = 1;
                    }
                    if (lLeft >= lLevel) {
                        lLeft = lLevel - 1;
                    }
                    lppl->rgpctMinLevel[i] = LOBYTE(LOWORD(lLeft));
                    if (lLeft == 0) {
                        lppl->rgMinConc[i]--;
                    }
                    break;
                }
            }
        }
    }
    if (fMacintosh != 0 && fRemote == 0) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            if (lpfl->idPlanet == lppl->id && lpfl->iPlayer == lppl->iPlayer && lpfl->fDead == 0 && lpfl->cord <= 1 &&
                lpfl->lpplord->rgord[0].grTask == grTaskMine) {
                cMines = CMineFromLpfl(lpfl);
                if (cMines > 0) {
                    EstMineralsMined(lppl, rglQuan, cMines, fApply);
                    for (i = 0; i < 3; i++) {
                        plQuan[i] += rglQuan[i];
                    }
                    if (fApply != 0) {
                        lpfl->fHereAllTurn = 0;
                    }
                }
            }
        }
    }
    return;
}
