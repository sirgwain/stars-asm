#include "common.h"

void DrawShipOrders(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t swp;
    int16_t dxRight;
    int16_t iWarp;
    int16_t yTop;
    POINT16 pt;
    RECT    rcT;
    int16_t dWrong;
    int32_t lTot;
    int16_t c;
    FLEET  *pfl;
    int16_t xRight;
    int16_t iScanActual;
    RECT    rcGauge;
    char   *psz;
    int16_t xLeft;
    ORDER   ord;
    int32_t l;
    RECT    rc;

    pfl = obj.pfl;
    if (ptile->fFixCtls != 0x0) {
        rgrcRef[0].top = -5;
        rgrcRef[0].bottom = -6;
        rgrcRef[12].top = -5;
        rgrcRef[12].bottom = -6;
        ShowWindow(hwndShipLB, SW_HIDE);
        ShowWindow(hwndRepCB, SW_HIDE);
        ptile->fFixCtls = 0x0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFleetWaypoints)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0x0 ? 4 : 2) + rc.top;
        rgrcRef[12].top = -5;
        rgrcRef[12].bottom = -6;
        GetClientRect(hwndShipLB, &rcT);
        swp = 20;
        dyShipLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0x0 ? 0x4 : 0x3);
        dWrong = dyShipLB - (rcT.bottom - rcT.top);
        if (dxShipLB != xRight - xLeft || dWrong < 0 || dWrong >= dyArial8) {
            dxShipLB = xRight - xLeft;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(hwndShipLB, 0x0, xLeft, yTop, xRight - xLeft, dyShipLB, swp);
        ShowWindow(hwndShipLB, SW_SHOW);
        GetClientRect(hwndShipLB, &rcT);
        dyShipLB = rcT.bottom - rcT.top;
        yTop = yTop + ((gd.fSmallTileMode == 0x0 ? 4 : 2) + dyShipLB);
        SelectObject(hdc, rghfontArial8[1]);
        if (ptile->fMinDraw == 0x0) {
            rcT.top = yTop;
            rcT.bottom = yTop + dyArial8;
            rcT.left = xLeft;
            rcT.right = xRight;
            FillRect(hdc, &rcT, hbrButtonFace);
        }
        c = CchGetString(idsComing, szWork);
        l = GetTextExtent(hdc, szWork, c);
        c = CchGetString(idsWayPt, szWork);
        lTot = GetTextExtent(hdc, szWork, c);
        if (lTot > l) {
            l = lTot;
        }
        dxRight = xRight - xLeft - LOWORD(l);
        if (ptile->fMinDraw == 0x0) {
            c = CchGetString(sel.iwpAct <= 0 ? idsWayPt : idsComing, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        if (sel.iwpAct <= 0) {
            if (sel.fl.cord > 1) {
                ord = sel.fl.lpplord->rgord[1];
                psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
                ord = sel.fl.lpplord->rgord[0];
                iScanActual = 1;
            } else {
                psz = "";
            }
        } else {
            ord = sel.fl.lpplord->rgord[sel.iwpAct - 1];
            psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
            iScanActual = sel.iwpAct;
        }
        RightTextOut(hdc, xRight, yTop, psz, 0, dxRight);
        yTop = yTop + dyArial8;
        if (sel.fl.cord <= 1 || sel.iwpAct == 0) {
            rgrcRef[0].top = -5;
            rgrcRef[0].bottom = -6;
            if (sel.fl.cord <= 1) {
                SetRect(&rc, xLeft - 1, yTop, xRight - 1, dyArial8 * 4 + yTop);
                FillRect(hdc, &rc, hbrButtonFace);
                yTop = yTop + ((dyArial8 - gd.fSmallTileMode) * 4 + gd.fSmallTileMode * 2);
                goto DoCheckBox;
            }
        }
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDistance, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        pt = sel.fl.lpplord->rgord[iScanActual].pt;
        RightTextOut(hdc, xRight, yTop, PszGetDistance(ord.pt.x, ord.pt.y, pt.x, pt.y), 0, dxRight);
        yTop = yTop + (dyArial8 - gd.fSmallTileMode);
        SelectObject(hdc, rghfontArial8[1]);
        if (ptile->fMinDraw == 0x0) {
            c = CchGetString(idsWarpFactor, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        iWarp = sel.fl.lpplord->rgord[iScanActual].iWarp;
        if (sel.iwpAct == 0) {
            SelectObject(hdc, rghfontArial8[0]);
            if (iWarp >= 11) {
                c = CchGetString(idsUseStargate, szWork);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsWarpD2), iWarp);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        } else {
            SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight - 1, yTop + dyArial8);
            rgrcRef[0] = rcGauge;
            DrawFleetGauge(hdc, &rcGauge, 0x0, 6);
        }
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsTravelTime, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        c = CchGetETA(hdc, pfl, szWork, iScanActual, 0);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        SetTextColor(hdc, 0x0);
        yTop = yTop + (dyArial8 - gd.fSmallTileMode);
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsEstFuelUsage, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        lTot = LFuelUseToWaypoint(&sel.fl, iScanActual, 0);
        c = _wsprintf(szWork, PszGetCompressedString(idsLdmg), lTot);
        if (lTot > sel.fl.rgwtMin[4]) {
            SetTextColor(hdc, 0xff);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight - 20);
        if (lTot > sel.fl.rgwtMin[4]) {
            SetTextColor(hdc, 0x0);
        }
        yTop = yTop + dyArial8;
    DoCheckBox:
        SendMessage(hwndRepCB, BM_SETCHECK, sel.fl.fRepOrders, 0);
        SetWindowPos(hwndRepCB, 0x0, xLeft, yTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(hwndRepCB, SW_SHOW);
        SetRect(&rgrcRef[12], xRight - (dyArial8 | 0x1), yTop, xRight, (dyArial8 | 0x1) + yTop);
        DrawDiamond(hdc, &rgrcRef[12], hbrBBlue);
    } else {
        ShowWindow(hwndShipLB, SW_HIDE);
        ShowWindow(hwndRepCB, SW_HIDE);
    }
    return;
}

void DrawShipWayPtOrders(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  dxKt;
    int16_t  dxT;
    int16_t  swp;
    int16_t  dxRight;
    int16_t  yTop;
    int16_t  yTopMsg;
    StringId ids;
    int16_t  edWid;
    PLANET  *lppl;
    ORDER   *lpord;
    FLEET   *pfl;
    int16_t  i;
    int16_t  fActive;
    int16_t  xRight;
    uint16_t grtask;
    char     szT[8];
    int16_t  yBot;
    int16_t  dxRight2;
    char    *psz;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    RECT     rc;
    RECT     rcT;
    char    *pszT;
    int16_t  j;
    int32_t  cMine;
    int16_t  dyCur;
    int16_t  c;
    int32_t  rgl[4];
    int32_t  t_call_12f6;

    if (ptile->fFixCtls != 0x0) {
        for (i = 0; i < 3; i++) {
            ShowWindow(rghwndOrderDD[i], SW_HIDE);
        }
        ShowWindow(hwndOrderED, SW_HIDE);
        ptile->fFixCtls = 0x0;
        rgrcRef[5].top = -5;
        rgrcRef[5].bottom = -6;
        rgrcRef[18].top = -5;
        rgrcRef[18].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsWaypointTask)) != 0) {
        pfl = obj.pfl;
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0x0 ? 4 : 2) + rc.top;
        yBot = rc.bottom - 4;
        dxRight = xRight - xLeft;
        rgrcRef[5].top = -5;
        rgrcRef[5].bottom = -6;
        swp = 20;
        if (rgdxOrderDD[0] != dxRight) {
            rgdxOrderDD[0] = dxRight;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(rghwndOrderDD[0], 0x0, xLeft, yTop, dxRight, 10 * dyArial8 + dyShipDD, swp);
        ShowWindow(rghwndOrderDD[0], SW_SHOW);
        yTop = yTop + (dyShipDD + 3);
        yTopMsg = yTop;
        l = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0x0, 0);
        grtask = LOWORD(l);
        if (IsWindowVisible(rghwndOrderDD[1]) != 0) {
            if (IsWindowVisible(rghwndOrderDD[2]) == 0) {
                SetRect(&rc, xLeft - 1, yTop + dyShipDD + 3, xRight + 1, yBot + 2);
                FillRect(hdc, &rc, hbrButtonFace);
            }
        } else {
            SetRect(&rc, xLeft - 1, yTop, xRight + 1, yBot + 2);
            FillRect(hdc, &rc, hbrButtonFace);
        }
        switch (grtask) {
        case 0x1:
        case 0x6:
        case 0x7:
        case 0x9:
            swp = 20;
            switch (grtask) {
            case 0x1:
                dxRight2 = dxRight - dyShipDD + 2;
                break;
            case 0x7:
                SelectObject(hdc, rghfontArial8[1]);
                psz = PszGetCompressedString(idsWarpFactor);
                cch = strlen(psz);
                dxT = LOWORD(GetTextExtent(hdc, psz, cch)) + 2;
                psz = PszGetCompressedString(idsIntercept);
                cch = strlen(psz);
                SetRect(&rc, xLeft, yTop + 4, xLeft + dxT, yBot);
                FillRect(hdc, &rc, hbrButtonFace);
                DrawText(hdc, psz, cch, &rc, 0x800);
                dxRight2 = dxRight - dxT - 2;
                xLeft = xLeft + (dxT + 2);
                break;
            case 0x9:
                SelectObject(hdc, rghfontArial8[1]);
                cch = CchGetString(idsTo3, szT);
                szT[cch] = ' ';
                cch = cch + 1;
                szT[cch] = 0;
                dxT = LOWORD(GetTextExtent(hdc, psz, cch)) + 2;
                SetRect(&rc, xLeft, yTop + 4, xLeft + dxT, yBot);
                FillRect(hdc, &rc, hbrButtonFace);
                DrawText(hdc, szT, cch, &rc, 0x800);
                dxRight2 = dxRight - dxT - 2;
                xLeft = xLeft + (dxT + 2);
                break;
            default:
                dxRight2 = dxRight;
            }
            if (rgdxOrderDD[1] != dxRight2) {
                rgdxOrderDD[1] = dxRight2;
            } else {
                swp = swp | 0x1;
            }
            SetWindowPos(rghwndOrderDD[1], 0x0, xLeft, yTop, dxRight2, 6 * dyShipDD, swp);
            ShowWindow(rghwndOrderDD[1], SW_SHOW);
            if (grtask != 0x1) {
                if (grtask != 0x7)
                    break;
                yTop = yTop + (dyShipDD + 4);
                SetRect(&rcT, xLeft - dxT - 2, yTop, xRight - 1, yTop + dyArial8);
                SelectObject(hdc, rghfontArial8[1]);
                psz = PszGetCompressedString(idsWarpFactor);
                cch = strlen(psz);
                DrawText(hdc, psz, cch, &rcT, 0x800);
                rcT.left = rcT.left + (dxT + 2);
                rgrcRef[18] = rcT;
                DrawFleetGauge(hdc, &rcT, 0x0, 7);
                break;
            }
            SetRect(&rcT, xLeft + dxRight - (dyShipDD | 0x1) + 0x8, yTop + 3, xLeft + dxRight, (dyShipDD | 0x1) + yTop - 0x5);
            rgrcRef[5] = rcT;
            DrawDiamond(hdc, &rcT, hbrBBlue);
            break;
        default:
            if (IsWindowVisible(rghwndOrderDD[1]) != 0) {
                ShowWindow(rghwndOrderDD[1], SW_HIDE);
                SetRect(&rc, xLeft - 1, yTop, xRight + 1, yBot + 2);
                FillRect(hdc, &rc, hbrButtonFace);
            }
        }
        yTop = yTop + (dyShipDD + 3);
        lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
        if (lpord->grobj != grobjPlanet) {
            lppl = 0x0;
        } else {
            lppl = LpplFromId(lpord->id);
        }
        if (grtask != 0x1) {
            ShowWindow(rghwndOrderDD[2], SW_HIDE);
            ShowWindow(hwndOrderED, SW_HIDE);
        } else {
            dxKt = 0;
            for (i = 0; i < 5; i++) {
                if (LOWORD(GetTextExtent(hdc, vrgszUnits[i], 2)) > dxKt) {
                    dxKt = LOWORD(GetTextExtent(hdc, vrgszUnits[i], 2));
                }
            }
            i = LOWORD(SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0));
            if (i != 0) {
                i = i - 1;
            } else {
                i = 4;
            }
            SelectObject(hdc, rghfontArial8[1]);
            edWid = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN99999Kt), 9));
            dxRight = dxRight - (edWid + 8);
            swp = 20;
            if (rgdxOrderDD[2] != dxRight) {
                rgdxOrderDD[2] = dxRight;
            } else {
                swp = swp | 0x1;
            }
            SetWindowPos(rghwndOrderDD[2], 0x0, xLeft, yTop, dxRight, 9 * dyShipDD, swp);
            ShowWindow(rghwndOrderDD[2], SW_SHOW);
            l = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0x0, 0);
            fActive = 1;
            switch (l) {
            case 0:
            case 1:
            case 2:
            case 7:
                fActive = 0;
                break;
            case 5:
            case 6:
                i = 5;
            default:
            }
            psz = vrgszUnits[i];
            swp = 20;
            if (dxOrderED != edWid - dxKt) {
                dxOrderED = edWid - dxKt;
            } else {
                swp = swp | 0x1;
            }
            SetWindowPos(hwndOrderED, 0x0, xRight - edWid - 4, yTop, edWid - dxKt, dyShipDD, swp);
            EnableWindow(hwndOrderED, fActive);
            ShowWindow(hwndOrderED, SW_SHOW);
            SetRect(&rc, xRight - dxKt - 1, yTop + 2, xRight - 1, yBot);
            FillRect(hdc, &rc, hbrButtonFace);
            if (fActive != 0) {
                DrawText(hdc, psz, 2, &rc, 0x800);
            }
        }
        switch (grtask) {
        case 0x5:
            ids = idsNoteShipsFleetWillDismantledMineralsCan;
            goto LDisplayMsg;
        case 0x4:
            if (sel.fl.lpplord->rgord[sel.iwpAct].grobj == grobjFleet)
                break;
            ids = idsWarningDestinationWaypointFleetMergeWillSucessfu;
            psz = PszGetCompressedString(ids);
            SetTextColor(hdc, 0x7f);
            SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
            SelectObject(hdc, rghfontArial7[0]);
            DrawText(hdc, psz, strlen(psz), &rc, 0x810);
            SetTextColor(hdc, crButtonText);
            break;
        case 0x6:
            yTopMsg = yTop;
            t_call_12f6 = CLayMinesFromLpfl(&sel.fl, -1, -1);
            l = t_call_12f6;
            if (t_call_12f6 <= 0) {
                ids = idsWarningFleetHasMineLayingPods;
                goto LDisplayMsg;
            }
            pszT = PszGetCompressedString(idsFleetCanLayLdMinesPerYear);
            _wsprintf(szWork, pszT, l);
            psz = szWork;
            goto LDisplayMsg2;
        case 0x2:
            fActive = 0;
            i = 0;
            while (1) {
                if (i >= 16)
                    goto FoundColony;
                if (sel.fl.rgcsh[i] > 0) {
                    for (j = 0; j < rglpshdef[sel.fl.iPlayer][i].hul.chs; j++) {
                        if (rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].grhst == hstSpecialM &&
                            (rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].iItem == 0x0 || rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].iItem == 0x1))
                            goto L_1457;
                    }
                }
                i = i + 1;
            }
        L_1457:
            fActive = 1;
        FoundColony:
            if (fActive == 0) {
                ids = idsWarningColonizeMissionCannotCarriedBecauseNone;
                goto LDisplayMsg;
            }
            if (sel.fl.rgwtMin[3] != 0) {
                ids = idsNoteShipsFleetWillDismantledProvideSupplies;
                goto LDisplayMsg;
            }
            ids = idsRememberLoadColonistsBeforeEmbarkingMission;
            goto LDisplayMsg;
        case 0x3:
            cMine = CMineFromLpfl(&sel.fl);
            if (cMine <= 0) {
                ids = 226;
            } else {
                if (lppl != 0x0 && (lppl->iPlayer == -1 || (lppl->iPlayer == sel.fl.iPlayer && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh))) {
                    if (dyArial8 <= 14) {
                        SelectObject(hdc, rghfontArial8[0]);
                        dyCur = dyArial8;
                    } else {
                        SelectObject(hdc, rghfontArial7[0]);
                        dyCur = dyArial7;
                    }
                    if (lppl->det > 0x1) {
                        EstMineralsMined(lppl, rgl, cMine, 0);
                        c = CchGetString(idsMiningRatePerYear, szWork);
                        TextOut(hdc, xLeft, yTopMsg, szWork, c);
                        yTopMsg = yTopMsg + dyCur;
                        dxRight = xLeft;
                        for (i = 0; i < 3; i++) {
                            SetTextColor(hdc, rgcrMinerals[i]);
                            c = _wsprintf(szWork, PCTLD, rgl[i]);
                            DxStreamTextOut(hdc, &dxRight, yTopMsg, szWork, c, 1);
                            SetTextColor(hdc, crButtonText);
                            DxStreamTextOut(hdc, &dxRight, yTopMsg, "kT  ", 4, 1);
                        }
                        goto DoneMine;
                    }
                    ids = 243;
                    goto ShowString;
                }
                ids = 229;
            }
            if (dyArial8 <= 14) {
                SelectObject(hdc, rghfontArial7[0]);
            } else {
                SelectObject(hdc, rghfontArial6[0]);
            }
        ShowString:
            SetTextColor(hdc, ids == 229 ? crButtonText : 0x7f);
            psz = PszGetCompressedString(ids);
            SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
            DrawText(hdc, psz, strlen(psz), &rc, 0x810);
        DoneMine:
            SetTextColor(hdc, crButtonText);
        default:
        }
        return;
    LDisplayMsg:
        psz = PszGetCompressedString(ids);
    LDisplayMsg2:
        SetTextColor(hdc, ids == idsNoteShipsFleetWillDismantledProvideSupplies ? crButtonText : 0x7f);
        SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
        SelectObject(hdc, rghfontArial7[0]);
        DrawText(hdc, psz, strlen(psz), &rc, 0x810);
        SetTextColor(hdc, crButtonText);
    } else {
        for (i = 0; i < 3; i++) {
            ShowWindow(rghwndOrderDD[i], SW_HIDE);
        }
        ShowWindow(hwndOrderED, SW_HIDE);
    }
    return;
}

void DrawShipPlanet(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t yTop;
    int16_t dy;
    int16_t i;
    int16_t xRight;
    char   *psz;
    int16_t dx;
    int16_t xLeft;
    RECT    rc;
    THING  *lpth;
    THING  *lpthMac;
    int16_t t_merge_18e6_0001;
    int16_t t_merge_19c4_0001;

    if (obj.pfl->idPlanet == -1) {
        psz = PszGetCompressedString(idsDeepSpace2);
    } else {
        psz = PszGetPlanetName(obj.pfl->idPlanet | 0x8000);
    }
    if (ptile->fFixCtls != 0x0) {
        ShowWindow(rghwndBtn[3], SW_HIDE);
        ShowWindow(rghwndBtn[7], SW_HIDE);
        ptile->fFixCtls = 0x0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0x0 ? 4 : 1) + rc.top;
        dx = (int32_t)(xRight - xLeft - 16) / 3;
        dy = 0x3 * dyArial8 >> 0x1;
        if (obj.pfl->idPlanet == -1 || sel.pl.iPlayer != idPlayer) {
            t_merge_18e6_0001 = 0;
        } else {
            t_merge_18e6_0001 = 1;
        }
        EnableWindow(rghwndBtn[3], t_merge_18e6_0001);
        SetWindowText(rghwndBtn[7], PszGetCompressedString(obj.pfl->idPlanet == -1 ? idsJettison2 : idsXFer));
        if (obj.pfl->idPlanet != -1) {
            EnableWindow(rghwndBtn[7], 1);
        } else {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac && (lpth->ith != ithMineralPacket || obj.pfl->pt.x != lpth->pt.x || obj.pfl->pt.y != lpth->pt.y); lpth++) {
            }
            t_merge_19c4_0001 = lpth == lpthMac ? 1 : 0;
            EnableWindow(rghwndBtn[7], t_merge_19c4_0001);
        }
        if (ptile->fMinDraw == 0x0) {
            i = 3;
            while (i <= 7) {
                SetWindowPos(rghwndBtn[i], 0x0, xLeft, yTop, dx, dy, SWP_NOZORDER | SWP_NOACTIVATE);
                ShowWindow(rghwndBtn[i], SW_SHOW);
                i = i + 4;
                xLeft = xLeft + (dx * 2 + 16);
            }
        }
    } else {
        ShowWindow(rghwndBtn[3], SW_HIDE);
        ShowWindow(rghwndBtn[7], SW_HIDE);
    }
    return;
}

void DrawShipCargo(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t dxRight;
    int32_t l2;
    int16_t yTop;
    int16_t i;
    int16_t c;
    FLEET  *pfl;
    int16_t xRight;
    RECT    rcGauge;
    int16_t xLeft;
    int32_t l;
    RECT    rc;

    pfl = obj.pfl;
    if (ptile->fFixCtls != 0x0) {
        rgrcRef[2].top = -5;
        rgrcRef[2].bottom = -6;
        rgrcRef[3].top = -5;
        rgrcRef[3].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFuelCargo)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 1;
        dxRight = dxMaxMineralQuan;
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsCargo3, szWork);
        l = GetTextExtent(hdc, szWork, c);
        c = CchGetString(idsFuel3, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        if (ptile->fMinDraw == 0x0) {
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[2] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 4);
        yTop = yTop + ((gd.fSmallTileMode == 0x0 ? 4 : 2) + dyArial8);
        if (ptile->fMinDraw == 0x0) {
            c = CchGetString(idsCargo3, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[3] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 5);
        yTop = yTop + (dyArial8 + 4);
        if (gd.fSmallTileMode == 0x0) {
            for (i = 0; i <= 2; i++) {
                if (ptile->fMinDraw == 0x0) {
                    SelectObject(hdc, rghfontArial8[1]);
                    SetTextColor(hdc, rgcrMinerals[i]);
                    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));
                }
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
                c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[i]);
                RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
                yTop = yTop + dyArial8;
            }
            if (ptile->fMinDraw == 0x0) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, 0xffffff);
                c = CchGetString(idsColonists2, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
            }
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[3]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
        }
    }
    return;
}

void DrawFleetComp(HDC hdc, TILE *ptile, OBJ obj) {
    int32_t cBoat;
    int16_t swp;
    int16_t dxRight;
    int16_t yTop;
    RECT    rcT;
    int16_t dyWrong;
    int16_t c;
    int16_t i;
    FLEET  *pfl;
    int16_t xStart;
    int16_t xRight;
    int16_t dxLabel;
    int16_t xLeft;
    int32_t l;
    RECT    rc;

    pfl = obj.pfl;
    if (ptile->fFixCtls != 0x0) {
        ShowWindow(hwndFleetCompLB, SW_HIDE);
        ShowWindow(rghwndBtn[8], SW_HIDE);
        ShowWindow(rghwndBtn[9], SW_HIDE);
        ShowWindow(rghwndBtn[10], SW_HIDE);
        ShowWindow(hwndBattleDD, SW_HIDE);
        ptile->fFixCtls = 0x0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFleetComposition)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 3;
        GetClientRect(hwndFleetCompLB, &rcT);
        swp = 20;
        dyFleetCompLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0x0 ? 0x5 : 0x3);
        dyWrong = dyFleetCompLB - (rcT.bottom - rcT.top);
        if (dxFleetCompLB != xRight - xLeft || dyWrong < 0 || dyWrong >= dyArial8) {
            dxFleetCompLB = xRight - xLeft;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(hwndFleetCompLB, 0x0, xLeft, yTop, xRight - xLeft, dyFleetCompLB, swp);
        ShowWindow(hwndFleetCompLB, SW_SHOW);
        GetClientRect(hwndFleetCompLB, &rcT);
        dyFleetCompLB = rcT.bottom - rcT.top;
        yTop = yTop + ((gd.fSmallTileMode == 0x0 ? 4 : 2) + dyFleetCompLB);
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsBattlePlan2, szWork);
        dxLabel = LOWORD(GetTextExtent(hdc, szWork, c));
        if (ptile->fMinDraw == 0x0) {
            TextOut(hdc, xLeft, yTop + 4, szWork, c);
        }
        swp = 20;
        if (dxBattleDD != xRight - xLeft - dxLabel) {
            dxBattleDD = xRight - xLeft - dxLabel;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(hwndBattleDD, 0x0, xLeft + dxLabel, yTop, dxBattleDD, 5 * dyShipDD, swp);
        ShowWindow(hwndBattleDD, SW_SHOW);
        yTop = yTop + (dyShipDD + 3);
        c = CchGetString(idsEstRange, szWork);
        l = GetTextExtent(hdc, szWork, c);
        if (ptile->fMinDraw == 0x0) {
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        c = CchGetString(idsN9999LY, szWork);
        dxRight = LOWORD(GetTextExtent(hdc, szWork, c)) + 6;
        i = IFindIdealWarp(0x0, 0);
        l = EstFuelUse(&sel.fl, 0, i, -1, 1);
        if (l < 1000000000) {
            c = _wsprintf(szWork, PszGetCompressedString(idsLdLY), l);
        } else {
            c = CchGetString(idsInfinite, szWork);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        if (gd.fSmallTileMode == 0x0) {
            yTop = yTop + dyArial8;
            if (ptile->fMinDraw == 0x0) {
                SelectObject(hdc, rghfontArial8[1]);
                c = CchGetString(idsPercentCloaked, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
            }
            SelectObject(hdc, rghfontArial8[0]);
            i = PctCloakFromLpfl(&sel.fl);
            if (i != 0) {
                c = _wsprintf(szWork, PCTDPCTPCT, i);
            } else {
                c = CchGetString(idsNone2, szWork);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        }
        yTop = yTop + (dyArial8 + 2 - gd.fSmallTileMode);
        xStart = xLeft;
        c = (int32_t)(xRight - xLeft - 10) / 3;
        i = 8;
        while (i <= 10) {
            SetWindowPos(rghwndBtn[i], 0x0, xStart, yTop, c, (dyArial8 >> 0x1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(rghwndBtn[i], SW_SHOW);
            i = i + 1;
            xStart = xStart + (c + 6);
        }
        cBoat = 0;
        for (i = 0; i < 16; i++) {
            cBoat = cBoat + (int32_t)sel.fl.rgcsh[i];
        }
        EnableWindow(rghwndBtn[8], FCanSplit(cBoat));
        EnableWindow(rghwndBtn[9], FCanSplitAll(cBoat));
        EnableWindow(rghwndBtn[10], FCanMerge(pfl));
    } else {
        ShowWindow(hwndFleetCompLB, SW_HIDE);
        ShowWindow(rghwndBtn[8], SW_HIDE);
        ShowWindow(rghwndBtn[9], SW_HIDE);
        ShowWindow(rghwndBtn[10], SW_HIDE);
        ShowWindow(hwndBattleDD, SW_HIDE);
    }
    return;
}

int16_t FCanSplit(int32_t cBoat) {
    if (rgplr[idPlayer].cFleet != 0x200) {
        if (cBoat <= 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}

int16_t FCanSplitAll(int32_t cBoat) {
    if (cBoat - 1 + rgplr[idPlayer].cFleet <= 0x200) {
        if (cBoat <= 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}

int16_t FCanMerge(FLEET *pfl) {
    int16_t i;
    FLEET  *lpfl;
    int32_t csh;
    int16_t cfl;
    int16_t ishdef;

    cfl = 0;
    csh = 0;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0)
            break;
        if (lpfl->iPlayer == pfl->iPlayer && lpfl->pt.x == sel.fl.pt.x && lpfl->pt.y == sel.fl.pt.y) {
            cfl = cfl + 1;
            for (ishdef = 0; ishdef < 16; ishdef++) {
                csh = csh + (int32_t)lpfl->rgcsh[ishdef];
            }
        }
    }
    if (cfl != 1 && csh <= (int32_t)(uint32_t)(0x7ffe - (rgplr[pfl->iPlayer].cFleet - 0x1))) {
        return 1;
    }
    return 0;
}

void ShipCommandProc(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    int16_t fPercent;
    FARPROC lpProc;
    int32_t lSel;
    XFER    xf;
    char    szT[34];
    int32_t lMin;
    int16_t ishdef;
    int16_t grbit;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t rgifl[512];
    int16_t ish;
    int16_t ishPrimary;
    FLEET  *lpflBest;
    char    rgb[8];
    int16_t i;
    int16_t iInit;
    int32_t t_2a2f;
    int16_t t_2fab;

    fPercent = 0;
    if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x0) {
        SetFocus(hwndFrame);
    }
    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[4] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[5] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[6] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[3] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[7] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                        if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndShipDD) {
                            if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndShipLB) {
                                if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndFleetCompLB) {
                                    if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndBattleDD) {
                                        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[0] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[1] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[2] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[8] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[9] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[10] ||
                                                                GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                                if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndRepCB ||
                                                                    GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                                    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndOrderDD[0]) {
                                                                        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndOrderDD[1]) {
                                                                            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndOrderDD[2]) {
                                                                                if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndOrderED &&
                                                                                    GET_WM_COMMAND_CMD(wParam, lParam) == 0x300) {
                                                                                    lSel = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0x0, 0);
                                                                                    if (lSel == 5 || lSel == 6) {
                                                                                        fPercent = 1;
                                                                                    }
                                                                                    GetWindowText(hwndOrderED, rgb, 8);
                                                                                    iInit = atoi(rgb);
                                                                                    i = 0 <= iInit ? iInit : 0;
                                                                                    if (fPercent == 0) {
                                                                                        i = 4000 >= i ? i : 4000;
                                                                                    } else {
                                                                                        i = 100 >= i ? i : 100;
                                                                                    }
                                                                                    if (iInit != i) {
                                                                                        AlertSz(PszFormatIds(idsAmountCargoMaySpecifyHereMustBetween, 0x0),
                                                                                                MB_ICONHAND);
                                                                                        _wsprintf(szWork, PCTD, i);
                                                                                        SetWindowText(hwndOrderED, szWork);
                                                                                    }
                                                                                    lMin = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0);
                                                                                    if (lMin != 0) {
                                                                                        lMin = lMin - 1;
                                                                                    } else {
                                                                                        lMin = 4;
                                                                                    }
                                                                                    sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[lMin].cQuan = i;
                                                                                    FLookupFleet(-1, &sel.fl);
                                                                                }
                                                                            } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                                                                lSel = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0x0, 0);
                                                                                lMin = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0);
                                                                                if (lMin != 0) {
                                                                                    lMin = lMin - 1;
                                                                                } else {
                                                                                    lMin = 4;
                                                                                }
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[lMin].iAction = LOWORD(lSel);
                                                                                FLookupFleet(-1, &sel.fl);
                                                                                UpdateOrdersDDs(3);
                                                                                DrawPlanShip(0x0, 256);
                                                                            }
                                                                        } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                                                            lSel = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0);
                                                                            switch (sel.fl.lpplord->rgord[sel.iwpAct].grTask) {
                                                                            case grTaskPatrol:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tptl.iDist = LOWORD(lSel);
                                                                                FLookupFleet(-1, &sel.fl);
                                                                                UpdateOrdersDDs(1);
                                                                                break;
                                                                            case grTaskGive:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = LOWORD(lSel);
                                                                                FLookupFleet(-1, &sel.fl);
                                                                                UpdateOrdersDDs(1);
                                                                                break;
                                                                            case grTaskXfer:
                                                                                UpdateOrdersDDs(2);
                                                                                DrawPlanShip(0x0, 256);
                                                                                break;
                                                                            default:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tlm.cTime = LOWORD(lSel);
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tlm.cTimeOld = LOWORD(lSel);
                                                                                FLookupFleet(-1, &sel.fl);
                                                                            }
                                                                        }
                                                                    } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                                                        lSel = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0x0, 0);
                                                                        if (LOWORD(lSel) != sel.fl.lpplord->rgord[sel.iwpAct].grTask) {
                                                                            if (lSel == 3 || sel.fl.lpplord->rgord[sel.iwpAct].grTask == grTaskMine) {
                                                                                InvalidateRect(hwndMine, 0x0, 1);
                                                                            }
                                                                            sel.fl.lpplord->rgord[sel.iwpAct].grTask = LOWORD(lSel);
                                                                            fmemset((uint8_t *)&sel.fl.lpplord->rgord[sel.iwpAct] + 0x8, 0, 0xa);
                                                                            switch (LOWORD(lSel)) {
                                                                            case 0x7:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tptl.iDist = 0x0;
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tptl.iWarp = 0x0;
                                                                                break;
                                                                            case 0x9:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = 0x0;
                                                                                break;
                                                                            case 0x6:
                                                                                sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = 0x5;
                                                                                break;
                                                                            case 0x4:
                                                                                if (sel.fl.lpplord->rgord[sel.iwpAct].grobj != grobjFleet) {
                                                                                    lpflBest = 0x0;
                                                                                    ishPrimary = 0;
                                                                                    for (ish = 1; ish < 16; ish++) {
                                                                                        if (sel.fl.rgcsh[ish] > sel.fl.rgcsh[ishPrimary]) {
                                                                                            ishPrimary = ish;
                                                                                        }
                                                                                    }
                                                                                    for (ifl = 0; ifl < cFleet; ifl++) {
                                                                                        lpfl = rglpfl[ifl];
                                                                                        if (rglpfl[ifl] == 0x0)
                                                                                            break;
                                                                                        if (lpfl->pt.x == sel.fl.lpplord->rgord[sel.iwpAct].pt.x &&
                                                                                            lpfl->pt.y == sel.fl.lpplord->rgord[sel.iwpAct].pt.y &&
                                                                                            lpfl->iPlayer == idPlayer && lpfl->fDead == 0x0 &&
                                                                                            lpfl->id != sel.fl.id) {
                                                                                            if (lpfl->rgcsh[ishPrimary] <= 0) {
                                                                                                if (lpflBest == 0x0 ||
                                                                                                    (lpflBest->cord > 1 && lpfl->cord == 1)) {
                                                                                                    lpflBest = lpfl;
                                                                                                }
                                                                                            } else {
                                                                                                lpflBest = lpfl;
                                                                                                if (lpfl->cord == 1)
                                                                                                    break;
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                    if (lpflBest != 0x0) {
                                                                                        sel.fl.lpplord->rgord[sel.iwpAct].grobj = grobjFleet;
                                                                                        sel.fl.lpplord->rgord[sel.iwpAct].id = lpflBest->id;
                                                                                        FLookupFleet(-1, &sel.fl);
                                                                                        FillOrdersLB();
                                                                                    }
                                                                                }
                                                                            default:
                                                                            }
                                                                            FLookupFleet(-1, &sel.fl);
                                                                            UpdateOrdersDDs(1);
                                                                            DrawPlanShip(0x0, -32512);
                                                                        }
                                                                    }
                                                                } else {
                                                                    sel.fl.fRepOrders = LOWORD(SendMessage(hwndRepCB, BM_GETCHECK, 0x0, 0));
                                                                    FLookupFleet(-1, &sel.fl);
                                                                }
                                                            } else {
                                                                vrgiflMerge = rgifl;
                                                                vcflMerge = 0;
                                                                for (ifl = 0; ifl < cFleet; ifl++) {
                                                                    lpfl = rglpfl[ifl];
                                                                    if (rglpfl[ifl] == 0x0)
                                                                        break;
                                                                    if (lpfl->iPlayer == idPlayer && lpfl->fDead == 0x0 && lpfl->pt.x == sel.fl.pt.x &&
                                                                        lpfl->pt.y == sel.fl.pt.y) {
                                                                        t_2fab = vcflMerge;
                                                                        vcflMerge = vcflMerge + 1;
                                                                        rgifl[t_2fab] = ifl;
                                                                    }
                                                                }
                                                                lpfl = 0x0;
                                                                lpProc = MakeProcInstance(MergeFleetsDlg, hInst);
                                                                if (DialogBox(hInst, MAKEINTRESOURCE(IDD_MERGE_FLEETS), hwndFrame, lpProc) != 0) {
                                                                    for (ifl = 0; ifl < vcflMerge; ifl++) {
                                                                        if (vrgiflMerge[ifl] != -1) {
                                                                            if (rglpfl[vrgiflMerge[ifl]]->id == sel.fl.id) {
                                                                                lpfl = rglpfl[vrgiflMerge[ifl]];
                                                                            }
                                                                            vrgiflMerge[ifl] = rglpfl[vrgiflMerge[ifl]]->id;
                                                                        }
                                                                    }
                                                                    for (ifl = 0; ifl < vcflMerge; ifl++) {
                                                                        if (vrgiflMerge[ifl] != -1) {
                                                                            if (lpfl != 0x0) {
                                                                                if (vrgiflMerge[ifl] != lpfl->id)
                                                                                    break;
                                                                            } else {
                                                                                lpfl = LpflFromId(vrgiflMerge[ifl]);
                                                                            }
                                                                        }
                                                                    }
                                                                    if (ifl == vcflMerge) {
                                                                        lpfl = 0x0;
                                                                    } else if (lpfl->id != sel.fl.id) {
                                                                        SelectAdjFleet(0, lpfl->id);
                                                                    }
                                                                }
                                                                FreeProcInstance(lpProc);
                                                                if (lpfl != 0x0) {
                                                                    FFleetMergeAll(&sel.fl);
                                                                    FillShipDD(sel.fl.id);
                                                                    grbit = -31819;
                                                                    FLookupFleet(sel.fl.id, &sel.fl);
                                                                    FillFleetCompLB();
                                                                    DrawPlanShip(0x0, grbit);
                                                                    InvalidateRect(hwndMine, 0x0, 1);
                                                                    if ((grbitScan & 0x80) != 0x0) {
                                                                        InvalidateRect(hwndScanner, 0x0, 1);
                                                                    }
                                                                    vrgiflMerge = 0x0;
                                                                    vcflMerge = 0;
                                                                    if (gd.fTutorial != 0x0) {
                                                                        AdvanceTutor();
                                                                    }
                                                                }
                                                            }
                                                        } else {
                                                            FFleetSplitAll(&sel.fl);
                                                            FillShipDD(sel.fl.id);
                                                            grbit = -31819;
                                                            FLookupFleet(sel.fl.id, &sel.fl);
                                                            FillFleetCompLB();
                                                            DrawPlanShip(0x0, grbit);
                                                            InvalidateRect(hwndMine, 0x0, 1);
                                                        }
                                                    } else {
                                                        TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferShips);
                                                        InvalidateReport(1, 1);
                                                        if (gd.fTutorial != 0x0) {
                                                            AdvanceTutor();
                                                        }
                                                    }
                                                } else {
                                                    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                                    if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0 &&
                                                        xf.grobj == grobjFleet) {
                                                        TransferStuff(sel.fl.id, grobjFleet, xf.id, grobjFleet, mdXferShips);
                                                        if ((grbitScan & 0x80) != 0x0) {
                                                            InvalidateRect(hwndScanner, 0x0, 1);
                                                        }
                                                        InvalidateReport(1, 1);
                                                    }
                                                }
                                            } else {
                                                lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                                if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0 && xf.grobj == grobjFleet) {
                                                    SelectAdjFleet(0, xf.id);
                                                }
                                            }
                                        } else {
                                            lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                            if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0) {
                                                TransferStuff(sel.fl.id, grobjFleet, xf.id, xf.grobj, mdXferCargo);
                                            }
                                        }
                                    } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                        lSel = SendMessage(hwndBattleDD, CB_GETCURSEL, 0x0, 0);
                                        if (lSel != -1) {
                                            if (lSel != 0) {
                                                sel.fl.iplan = LOBYTE(LOWORD(lSel) - 0x1);
                                            } else {
                                                lpProc = MakeProcInstance(BattlePlansDlg, hInst);
                                                lSel = (int32_t)DialogBox(hInst, MAKEINTRESOURCE(IDD_BATTLE_PLANS), hwndFrame, lpProc);
                                                FreeProcInstance(lpProc);
                                                sel.fl.iplan = LOBYTE(LOWORD(lSel));
                                            }
                                            FLookupFleet(-1, &sel.fl);
                                        }
                                    }
                                } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                    lSel = SendMessage(hwndFleetCompLB, LB_GETCURSEL, 0x0, 0);
                                    if (lSel >= 0) {
                                        for (ishdef = 0; ishdef < 16; ishdef++) {
                                            if (sel.fl.rgcsh[ishdef] > 0) {
                                                t_2a2f = lSel;
                                                lSel = lSel - 1;
                                                if (t_2a2f <= 0)
                                                    break;
                                            }
                                        }
                                        GlobalPD.grPopup = grPopupShdef;
                                        GlobalPD.lpshdef = &rgshdef[ishdef];
                                        GlobalPD.fHideCounts = 0;
                                        GlobalPD.fShowDamage = 1;
                                        GlobalPD.fToken = 0;
                                        GlobalPD.fSummary = 0;
                                        Popup(hwndFleetCompLB, 10, 10);
                                    }
                                }
                            } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                lSel = SendMessage(hwndShipLB, LB_GETCURSEL, 0x0, 0);
                                SetScanWp(LOWORD(lSel));
                            }
                        } else if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                            DrawPlanShip(0x0, -32764);
                        }
                    } else if (sel.fl.idPlanet == -1) {
                        TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
                    } else {
                        TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
                    }
                } else {
                    SelectAdjPlanet(0, sel.fl.idPlanet);
                    SetFleetDropDownSel(sel.fl.id);
                }
            } else {
                strcpy(szWork, PszGetFleetName(sel.fl.id));
                StickyDlgPos(hwnd, &ptStickyRenameDlg, 0);
                lpProc = MakeProcInstance(RenameDlg, hInst);
                if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc) == 0) {
                    FreeProcInstance(lpProc);
                } else {
                    FreeProcInstance(lpProc);
                    strcpy(szT, szWork);
                    if (strcmp(szT, PszGetFleetName(sel.fl.id)) != 0) {
                        LogChangeName(grobjFleet, sel.fl.id, szT);
                        InvalidateReport(1, 1);
                        FillOrdersLB();
                        DrawPlanShip(0x0, -32608);
                        InvalidateRect(hwndMessage, 0x0, 1);
                        InvalidateRect(hwndScanner, 0x0, 1);
                        SetMineralTitleBar(hwndMine);
                    }
                }
            }
        } else {
            SelectAdjFleet(1, 0);
        }
    } else {
        SelectAdjFleet(-1, 0);
    }
    return;
}

void SelectAdjFleet(int16_t dInc, int16_t idFleet) {
    POINT16 pt;
    int16_t idOld;
    int16_t i;
    FLEET  *lpfl;
    int16_t idNew;
    FLEET  *lpflT;
    SCAN    scan;

    idOld = -1;
    if (cFleet > 0) {
        if (dInc != 0) {
            idFleet = sel.fl.id;
        }
        if (vrptFleet.fCached == 0) {
            InvalidateReport(1, 1);
        }
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0x0 || lpfl->id == idFleet)
                break;
        }
        if (i != cFleet && lpfl->iPlayer == idPlayer) {
            if (dInc == 0) {
                if (sel.grobj == grobjFleet && sel.fl.pt.x == lpfl->pt.x && sel.fl.pt.y == lpfl->pt.y) {
                    idOld = sel.fl.id;
                }
            } else {
                for (i = 0; i < rgplr[idPlayer].cFleet && rglpfl[vlprgidFleet[i]]->id != idFleet; i++) {
                }
                i = i + dInc;
                if (i < rgplr[idPlayer].cFleet) {
                    if (i < 0) {
                        i = rgplr[idPlayer].cFleet - 1;
                    }
                } else {
                    i = 0;
                }
                i = vlprgidFleet[i];
            }
            lpflT = rglpfl[i];
            idNew = lpflT->id;
            pt = lpflT->pt;
            scan.pt = lpflT->pt;
            scan.grobj = 0x82;
            ChangeScanSel(&scan, 0);
            RedrawScanSel(0x0, 0);
            ChangeMainObjSel(grobjFleet, idNew);
            RedrawScanSel(0x0, 1);
        } else {
            if (i == cFleet) {
                return;
            }
            pt = lpfl->pt;
            scan.pt = lpfl->pt;
            scan.grobj = 0x83;
            ChangeScanSel(&scan, 0);
        }
        CtrPointScan(pt, 1);
        DrawScannerSBar(0x0, 0x0, 0x0, 0);
        InvalidateRect(hwndMine, 0x0, 1);
        SetMineralTitleBar(hwndMine);
        if (idOld != -1) {
            SetFleetDropDownSel(idOld);
        }
    }
    return;
}

void SetFleetDropDownSel(int16_t id) {
    int16_t idSkip;
    int16_t i;
    FLEET  *lpfl;
    int16_t iOffset;

    iOffset = 0;
    idSkip = sel.grobj == grobjFleet ? sel.fl.id : -1;
    for (i = 0; i < cFleet && rglpfl[i]->id != id; i++) {
        lpfl = rglpfl[i];
        if (sel.pt.x == lpfl->pt.x && sel.pt.y == lpfl->pt.y && lpfl->id != idSkip) {
            iOffset = iOffset + 1;
        }
    }
    SendMessage(hwndShipDD, CB_SETCURSEL, iOffset, 0);
    DrawPlanShip(0x0, 16388);
    return;
}

int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat) {
    int16_t i;
    int32_t l;

    l = 0;
    if (lpfl->det == 0x7) {
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] != 0) {
                l = l + (uint32_t)((int32_t)lpfl->rgcsh[i] * (int32_t)WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, grStat));
            }
        }
        return l;
    }
    return 32000;
}

int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat) {
    int16_t wt;
    int16_t j;
    HUL    *lphul;

    lphul = &lpshdef->hul;
    if (grStat == 1) {
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtFuelMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst != hstSpecialM) {
                if (lphul->rghs[j].grhst == hstSpecialE && lphul->rghs[j].iItem == 0x10) {
                    wt = wt + lphul->rghs[j].cItem * 0xc8;
                }
            } else if (lphul->rghs[j].iItem != 0x5) {
                if (lphul->rghs[j].iItem == 0x6) {
                    wt = wt + lphul->rghs[j].cItem * 0x1f4;
                }
            } else {
                wt = wt + lphul->rghs[j].cItem * 0xfa;
            }
        }
    } else {
        if (grStat != 2) {
            return 0;
        }
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtCargoMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst == hstSpecialM) {
                switch (lphul->rghs[j].iItem) {
                default:
                    break;
                case 0x2:
                    wt = wt + lphul->rghs[j].cItem * 0x32;
                    break;
                case 0x3:
                    wt = wt + lphul->rghs[j].cItem * 0x64;
                    break;
                case 0x4:
                    wt = wt + lphul->rghs[j].cItem * 0xfa;
                }
            }
        }
    }
    return wt;
}

void DrawFleetGauge(HDC hdc, RECT *prc, FLEET *lpfl, int16_t grbit) {
    HBRUSH  rghbr[5];
    int32_t lMax;
    int16_t c;
    int16_t i;
    int32_t rgSize[5];
    int16_t iMode;
    int16_t cSections;
    int32_t l;

    if (lpfl == 0x0) {
        lpfl = &sel.fl;
    }
    SelectObject(hdc, rghfontArial8[1]);
    cSections = 1;
    lMax = LGetFleetStat(lpfl, 2);
    if (grbit < 0 || grbit > 4) {
        switch (grbit) {
        case 5:
            for (i = 0; i <= 3; i++) {
                rghbr[i] = rghbrMineral[i];
                rgSize[i] = lpfl->rgwtMin[i];
            }
            cSections = 4;
            break;
        case 6:
            lMax = 11;
            rgSize[0] = sel.fl.lpplord->rgord[sel.iwpAct].iWarp;
            if (rgSize[0] <= 10 && (rgSize[0] != 10 || IFindIdealWarp(&sel.fl, 0) >= 10)) {
                rghbr[0] = rghbrMineral[4];
                break;
            }
            rghbr[0] = rghbrMineral[2];
            break;
        case 7:
            lMax = 10;
            rgSize[0] = (uint32_t)sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
            if (rgSize[0] <= 10 && (rgSize[0] != 10 || IFindIdealWarp(&sel.fl, 0) >= 10)) {
                rghbr[0] = rghbrMineral[4];
            } else {
                rghbr[0] = rghbrMineral[2];
            }
        default:
        }
    } else {
        rghbr[0] = rghbrMineral[grbit];
        rgSize[0] = lpfl->rgwtMin[grbit];
        if (grbit == 4) {
            lMax = LGetFleetStat(lpfl, 1);
        }
    }
    l = LDrawGauge(hdc, prc, cSections, rgSize, rghbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if (grbit != 6) {
        if (grbit != 7) {
            if (cSections != 1 || grbit == 4) {
                if (grbit != 4) {
                    c = _wsprintf(szWork, PszGetCompressedString(idsLdLdkt), l, lMax);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsLdLdmg), l, lMax);
                }
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), l);
            }
        } else if (l != 0) {
            c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l);
        } else {
            c = CchGetString(idsAutomatic, szWork);
        }
    } else if (l != 0) {
        if (l < 11) {
            c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l);
        } else {
            c = CchGetString(idsUseStargate, szWork);
        }
    } else {
        c = CchGetString(idsStopped, szWork);
    }
    l = GetTextExtent(hdc, szWork, c);
    if (LOWORD(l) < prc->right - prc->left - 3) {
        RcCtrTextOut(hdc, prc, szWork, c);
    }
    SetBkMode(hdc, iMode);
    return;
}

void DrawFleetBitmap(FLEET *lpfl, HDC hdc, int16_t x, int16_t y, int16_t fFrame, int16_t ibmp, int16_t cDiff, int16_t fShrink, int16_t ibmpRace, int16_t csh) {
    int16_t dxyPlus;
    int16_t yCur;
    int16_t c;
    int16_t i;
    int16_t dxy;
    int16_t dx;
    int16_t xCur;
    int16_t dxyPlusWidth;
    HBRUSH  hbrSav;

    if (fFrame != 0) {
        hbrSav = SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, x, y, 68, 2, PATCOPY);
        PatBlt(hdc, x, y, 2, 68, PATCOPY);
        SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, x + 2, y + 66, 66, 1, PATCOPY);
        PatBlt(hdc, x + 3, y + 66, 65, 1, PATCOPY);
        PatBlt(hdc, x + 66, y + 2, 1, 64, PATCOPY);
        PatBlt(hdc, x + 67, y + 1, 1, 65, PATCOPY);
        SelectObject(hdc, hbrSav);
        x = x + 2;
        y = y + 2;
    }
    if (ibmp < 0) {
        i = IshdefPrimaryFromLpfl(lpfl, &cDiff);
        ibmp = rglpshdef[lpfl->iPlayer][i].hul.ibmp;
    }
    ibmp = (int32_t)ibmp % 148;
    SelectPalette(hdc, vhpal, 0);
    RealizePalette(hdc);
    if (fShrink == 0) {
        DibBlt(hdc, x, y, 64, 64, rghdibShips[ibmp >> 0x5], ((ibmp & 0x1f) >> 0x2) * 0x40, (0x3 - (ibmp & 0x3)) * 0x40, 64, 64, 13369376);
        dxy = 64;
        dxyPlus = 8;
        dxyPlusWidth = 2;
        if (ibmpRace >= 0) {
            DibBlt(hdc, x, y + 48, 16, 16, hdibRacesT, (ibmpRace & 0x7) * 0x10, (0x3 - ((ibmpRace & 0x1f) >> 0x3)) * 0x10, 16, 16, 13369376);
        }
    } else {
        DibBlt(hdc, x, y, 32, 32, rghdibShipsT[ibmp >> 0x5], ((ibmp & 0x1f) >> 0x2) * 0x20, (0x3 - (ibmp & 0x3)) * 0x20, 32, 32, 13369376);
        dxy = 32;
        dxyPlus = 5;
        dxyPlusWidth = 1;
        if (ibmpRace >= 0) {
            DibBlt(hdc, x, y + 24, 8, 8, hdibRacesX, (ibmpRace & 0x7) * 0x8, (0x3 - ((ibmpRace & 0x1f) >> 0x3)) * 0x8, 8, 8, 13369376);
        }
    }
    if (csh != 0 && fShrink == 0) {
        SelectObject(hdc, rghfontArial7[0]);
        SetTextColor(hdc, 0xffffff);
        if (cDiff > 1) {
            c = _wsprintf(szWork, PCTD, cDiff);
            TextOut(hdc, x + 1, y + 1, szWork, c);
        }
        if (csh > 1) {
            c = _wsprintf(szWork, PCTD, csh);
            dx = LOWORD(GetTextExtent(hdc, szWork, c));
            TextOut(hdc, x + dxy - 1 - dx, y + 1, szWork, c);
        }
    } else {
        if (cDiff > 4) {
            cDiff = 4;
        }
        cDiff = cDiff - 1;
        for (i = 0; i < cDiff; i++) {
            xCur = ((i & 0x1) ^ ((i & 0x2) == 0x2 ? 0x1 : 0x0)) == 0x0 ? x + 2 : x + dxy - 2 - dxyPlus;
            yCur = (i & 0x2) == 0x0 ? y + 2 : y + dxy - 2 - dxyPlus;
            PatBlt(hdc, xCur, (int32_t)(dxyPlus - 1) / 2 + yCur, dxyPlus, dxyPlusWidth, WHITENESS);
            PatBlt(hdc, (int32_t)(dxyPlus - 1) / 2 + xCur, yCur, dxyPlusWidth, dxyPlus, WHITENESS);
        }
    }
    return;
}

int16_t FEnumCalcJettison(void *lprt, RecordType rt, int16_t cb, PLANET *lppl, int16_t iFleet) {
    POINT16  pt;
    int16_t  i;
    int16_t  grbit;
    FLEET    fl;
    int16_t  j;
    RTXFERX *prtxferx;
    RTXFER  *prtxfer;

    if (rt == rtLogCargoXfer8 || rt == rtLogCargoXfer16) {
        prtxfer = lprt;
        if (prtxfer->grobj1 != grobjFleet || prtxfer->grobj2 != grobjOther) {
            return 1;
        }
        if (FLookupFleet(iFleet, &fl) == 0) {
            return 1;
        }
        pt = fl.pt;
        if (FLookupFleet(prtxfer->id1, &fl) == 0) {
            return 1;
        }
        if (fl.pt.x != pt.x || fl.pt.y != pt.y) {
            return 1;
        }
        grbit = prtxfer->grbitItems;
        if (rt == rtLogCargoXfer16) {
            prtxferx = lprt;
        }
        j = 0;
        i = 0;
        while (i < 5) {
            if ((grbit & 0x1) != 0x0) {
                if (rt != rtLogCargoXfer8) {
                    lppl->rgwtMin[i] = lppl->rgwtMin[i] - (int32_t)prtxferx->rgcQuan[j];
                } else {
                    lppl->rgwtMin[i] = lppl->rgwtMin[i] - (int32_t)(int16_t)prtxfer->rgcQuan[j];
                }
                j = j + 1;
            }
            i = i + 1;
            grbit = grbit >> 0x1;
        }
    }
    return 1;
}

int16_t TransferStuff(int16_t id1, GrobjClass grobj1, int16_t id2, GrobjClass grobj2, MdXfer mdXfer) {
    XFER    xfer[2];
    FARPROC lpProcXfer;
    int16_t rgValidHull[16];
    int32_t lPopPrev;
    int16_t iDelFleet;
    int16_t i;
    FLEET  *lpfl;
    int16_t fSuccess;
    int16_t grbit;
    int16_t j;
    BTN     rgbtn[32];
    POINT16 pt;
    RECT    rc;
    int16_t t_51ad;

    lPopPrev = -1;
    xfer[0].id = id1;
    xfer[0].grobj = grobj1;
    xfer[1].id = id2;
    xfer[1].grobj = grobj2;
    pxfer = xfer;
    mdXferDlg = mdXfer;
    for (i = 0; i < 2; i++) {
        if (xfer[i].grobj == grobjOther) {
            if (mdXfer != mdXferShips) {
                xfer[i].fl.id = -1;
                for (j = 0; j < 4; j++) {
                    xfer[i].pl.rgwtMin[j] = 0;
                }
                EnumLogRts((int16_t (*)(void *, int16_t, int16_t, void *, int16_t))FEnumCalcJettison, &xfer[i].pl, id1);
            } else {
                lpfl = LpflNewSplit(&xfer[0].fl);
                xfer[1].fl = *lpfl;
                xfer[1].id = lpfl->id;
                xfer[1].grobj = grobjFleet;
            }
        } else {
            if (FLookupObject(xfer[i].grobj, xfer[i].id, &xfer[i].fl) == 0) {
                return 0;
            }
            if (xfer[i].grobj == grobjPlanet) {
                lPopPrev = xfer[i].pl.rgwtMin[3];
            }
        }
    }
    if (mdXfer == mdXferShips) {
        cXferValidHulls = 0;
        for (i = 0; i < 16; i++) {
            if (xfer[0].fl.rgcsh[i] != 0 || (xfer[1].grobj == grobjFleet && xfer[1].fl.rgcsh[i] != 0)) {
                t_51ad = cXferValidHulls;
                cXferValidHulls = cXferValidHulls + 1;
                rgValidHull[t_51ad] = i;
            }
        }
        rgXferValidHulls = rgValidHull;
    }
    rgbtnXfer = rgbtn;
    crgbtnXfer = 32;
    if (gd.fTutorial != 0x0) {
        AdvanceTutor();
    }
    lpProcXfer = MakeProcInstance(TransferDlg, hInst);
    fSuccess = DialogBox(hInst, MAKEINTRESOURCE(IDD_TRANSFER), hwndFrame, lpProcXfer);
    FreeProcInstance(lpProcXfer);
    if (fSuccess != 0) {
        iDelFleet = -1;
        if (mdXfer == mdXferShips) {
            for (i = 0; i < 16 && xfer[1].fl.rgcsh[i] <= 0; i++) {
            }
            if (i == 16 && grobj2 == grobjOther)
                goto CancelSplit;
            FleetTransferCargoBalance(&xfer[0].fl, &xfer[1].fl);
        }
        for (i = 0; i < 2; i++) {
            switch (xfer[i].grobj) {
            case grobjPlanet:
            case grobjOther:
                FLookupPlanet(-1, &xfer[i].pl);
                if (xfer[i].grobj != grobjPlanet || lPopPrev != xfer[i].pl.rgwtMin[3])
                    break;
                lPopPrev = -1;
                break;
            case grobjFleet:
                FLookupFleet(-1, &xfer[i].fl);
                if (mdXfer != mdXferShips)
                    break;
                for (j = 0; j < 16 && xfer[i].fl.rgcsh[j] == 0; j++) {
                }
                if (j != 16)
                    break;
                iDelFleet = i;
                break;
            case grobjThing:
                FLookupThing(-1, &xfer[i].th);
            default:
            }
        }
        if (iDelFleet != -1) {
            FDeleteFleet(xfer[iDelFleet].fl.id, grobjFleet, xfer[iDelFleet == 0 ? 1 : 0].fl.id);
        }
        if (mdXfer == mdXferShips) {
            FillShipDD(sel.fl.id);
            if ((grbitScan & 0x80) != 0x0) {
                InvalidateRect(hwndScanner, 0x0, 1);
            }
        }
        if (sel.grobj != grobjPlanet) {
            grbit = -31819;
            FLookupFleet(sel.fl.id, &sel.fl);
            FillFleetCompLB();
        } else {
            grbit = -32691;
            FLookupPlanet(sel.pl.id, &sel.pl);
            FillPlanetProdLB(0x0, 0x0, 0x0);
        }
        DrawPlanShip(0x0, grbit);
        if (sel.scan.grobj != grobjFleet) {
            InvalidateMineralBars();
        } else {
            InvalidateRect(hwndMine, 0x0, 1);
        }
        if (lPopPrev == -1 || GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh || (grbitScan & 0x20) == 0x0) {
            if ((lPopPrev != -1 && (grbitScan & 0xf) == 0x4) || (grbitScan & 0xf) == 0x1) {
                pt = sel.pt;
                LogicalToScan(&pt);
                rc.right = pt.x;
                rc.bottom = pt.y;
                rc.left = pt.x;
                rc.top = pt.y;
                InflateRect(&rc, 20, 20);
                rc.top = rc.top - 20;
                InvalidateRect(hwndScanner, &rc, 0);
                goto L_5673;
            }
            goto L_5673;
        }
        InvalidateRect(hwndScanner, 0x0, 0);
        goto L_5673;
    }
    if (mdXfer != mdXferShips || grobj2 != grobjOther)
        goto L_5673;
CancelSplit:
    FDeleteFleet(xfer[1].fl.id, grobjNone, 0);
    CancelMemRt(rtLogFleetSplit);
L_5673:
    mdXferDlg = 0xffff;
    return 0;
}

INT_PTR CALLBACK TransferDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     dyMore;
    PAINTSTRUCT ps;
    POINT16     pt;
    HWND        hwndBtn;
    RECT        rcBtn;
    int16_t     dx;
    RECT        rc;
    POINT       t_pt_5774_1;
    POINT       t_pt_57d7_1;
    POINT       t_pt_583a_1;

    switch (message) {
    case WM_INITDIALOG:
        StickyDlgPos(hwnd, &ptStickyTransferDlg, 1);
        GetClientRect(hwnd, &rc);
        if (mdXferDlg == mdXferShips) {
            SetWindowText(hwnd, PszGetCompressedString(idsShipTransfer));
            if (cXferValidHulls > 10) {
                dyMore = (int32_t)rc.bottom / 2;
                rc.bottom = rc.bottom + dyMore;
                SetWindowPos(hwnd, 0x0, 0, 0, rc.right, rc.bottom, SWP_NOMOVE | SWP_NOZORDER);
                GetClientRect(hwnd, &rc);
                dyMore = dyMore - (GetSystemMetrics(SM_CYCAPTION) + 2);
                dx = GetSystemMetrics(SM_CXDLGFRAME) + 4;
                hwndBtn = GetDlgItem(hwnd, IDOK);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                t_pt_5774_1 = PointFrom16(pt);
                ScreenToClient(hwnd, &t_pt_5774_1);
                pt = PointTo16(t_pt_5774_1);
                pt.y = pt.y + dyMore;
                pt.x = pt.x - dx;
                SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0, 0, SWP_NOSIZE);
                hwndBtn = GetDlgItem(hwnd, IDC_HELP);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                t_pt_57d7_1 = PointFrom16(pt);
                ScreenToClient(hwnd, &t_pt_57d7_1);
                pt = PointTo16(t_pt_57d7_1);
                pt.y = pt.y + dyMore;
                pt.x = pt.x - dx;
                SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0, 0, SWP_NOSIZE);
                hwndBtn = GetDlgItem(hwnd, IDCANCEL);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                t_pt_583a_1 = PointFrom16(pt);
                ScreenToClient(hwnd, &t_pt_583a_1);
                pt = PointTo16(t_pt_583a_1);
                pt.y = pt.y + dyMore;
                pt.x = pt.x - dx;
                SetWindowPos(hwndBtn, 0x0, pt.x, pt.y, 0, 0, SWP_NOSIZE);
            }
        }
        FSetupXferBtns(&rc);
        if (gd.fTutorial != 0x0) {
            AdvanceTutor();
        }
        return 1;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawXferDlg(hwnd, hdc, &rc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        return FTrackXfer(hwnd, LOWORD(lParam), HIWORD(lParam), wParam);
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case IDOK:
        case IDCANCEL:
            StickyDlgPos(hwnd, &ptStickyTransferDlg, 0);
            EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, 0x1, (uint32_t)(mdXferDlg == mdXferShips ? 0x438 : 0x433));
            return 1;
        default:
        }
    default:
        return 0;
    }
}

int16_t FTrackXfer(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    POINT16  ptOld;
    POINT16  pt;
    int32_t  dChg;
    BTNT     btnt;
    int32_t  cCur;
    int16_t  i;
    int16_t  iBtn;
    int16_t  iVal;
    BTN      btn;
    int32_t  cNew;
    RECT     rc;
    int32_t  t_call_5beb;
    int32_t  t_merge_5c20_0001;
    int32_t  t_call_5c18;
    int16_t  t_merge_5e4d_0001;
    uint16_t t_merge_5ecd_0001;

    GetClientRect(hwnd, &rc);
    pt.x = x;
    pt.y = y;
    for (i = 0; i < crgbtnXfer && ((rgbtnXfer[i].bt & 0x4) != 0x0 || PtInRect(&rgbtnXfer[i].rc, PointFrom16(pt)) == 0); i++) {
    }
    if (i != crgbtnXfer) {
        iBtn = i >> 0x1;
        btn = rgbtnXfer[i];
        iVal = btn.iVal & 0x7f;
        if (btn.fVisible != 0x0) {
            InitBtnTrack(&btnt, hwnd, 0x0, &btn.rc, btn.bt, 80, 0, 0, 0x0);
            if ((fkb & 0x8) == 0x0) {
                if ((fkb & 0x4) == 0x0) {
                    dChg = 1;
                } else {
                    dChg = 10;
                }
            } else {
                dChg = (uint32_t)((fkb & 0x4) == 0x0 ? 0x64 : 0x3e8);
            }
            while (FTrackBtn(&btnt) != 0) {
                if (mdXferDlg != mdXferShips) {
                    if (iVal >= 0 && iVal <= 4 && XferSupply(iVal, btn.iSide == 0x0 ? dChg : -dChg) != 0) {
                        DrawXferDlg(hwnd, btnt.hdc, &rc, iVal);
                    }
                } else {
                    if (LOWORD(dChg) >= pxfer[btn.iSide == 0x0 ? 1 : 0].fl.rgcsh[iVal]) {
                        t_merge_5e4d_0001 = pxfer[btn.iSide == 0x0 ? 1 : 0].fl.rgcsh[iVal];
                    } else {
                        t_merge_5e4d_0001 = LOWORD(dChg);
                    }
                    i = t_merge_5e4d_0001;
                    if (i != 0) {
                        if (pxfer[btn.iSide].fl.rgcsh[iVal] >= 32766 - i) {
                            i = 1;
                        }
                        pxfer[btn.iSide].fl.rgcsh[iVal] = pxfer[btn.iSide].fl.rgcsh[iVal] + i;
                        t_merge_5ecd_0001 = btn.iSide == 0x0 ? 0x1 : 0x0;
                        pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] = pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] - i;
                        DrawXferDlg(hwnd, btnt.hdc, &rc, iBtn);
                    }
                }
            }
        } else if (iVal <= 4) {
            if (pxfer[1].grobj != grobjThing) {
                if (pxfer[btn.iSide].fl.iPlayer != idPlayer)
                    goto FinishUp;
            } else if (iVal == 4 || iVal == 3) {
                goto FinishUp;
            }
            SetCapture(hwnd);
            ptOld.y = -1;
            ptOld.x = -1;
            while (FGetMouseMove(&pt) != 0) {
                if (pt.x != ptOld.x || pt.y != ptOld.y) {
                    ptOld = pt;
                    if (btn.iSide != 0x1 || pxfer[1].grobj != grobjThing) {
                        if (iVal != 4) {
                            t_call_5c18 = LGetFleetStat(&pxfer[btn.iSide].fl, 2);
                            t_merge_5c20_0001 = t_call_5c18;
                        } else {
                            t_call_5beb = LGetFleetStat(&pxfer[btn.iSide].fl, 1);
                            t_merge_5c20_0001 = t_call_5beb;
                        }
                        cNew = t_merge_5c20_0001;
                    } else {
                        cNew = (uint32_t)(pxfer[1].th.thp.wtMax * 0xa);
                    }
                    cNew = (int32_t)((int32_t)((int32_t)(pt.x - btn.rc.left) * cNew) / (int32_t)(btn.rc.right - btn.rc.left - 2));
                    cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, (uint8_t *)(pxfer + btn.iSide) + 4);
                    dChg = cNew - cCur;
                    if (XferSupply(iVal, btn.iSide == 0x0 ? dChg : -dChg) != 0) {
                        DrawXferDlg(hwnd, 0x0, &rc, iVal);
                    }
                }
            }
            ReleaseCapture();
        }
    FinishUp:
        UpdateXferBtns();
        DrawXferDlg(hwnd, 0x0, &rc, -2);
        return 1;
    }
    return 0;
}

int32_t GetCargoFree(FLEET *lpfl) {
    int32_t cHave;
    int16_t i;
    int32_t t_call_5fed;

    cHave = 0;
    for (i = 0; i <= 3; i++) {
        cHave = cHave + lpfl->rgwtMin[i];
    }
    t_call_5fed = LGetFleetStat(lpfl, 2);
    return t_call_5fed - cHave;
}

int32_t GetFuelFree(FLEET *lpfl) {
    int32_t t_call_6017;

    t_call_6017 = LGetFleetStat(lpfl, 1);
    return t_call_6017 - lpfl->rgwtMin[4];
}

int32_t ChgCargo(GrobjClass grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj) {
    THING  *pth;
    XFER    xfer;
    int16_t i;
    FLEET  *pfl;
    PLANET *ppl;
    int32_t wtFree;
    int32_t t_call_640a;
    int32_t t_merge_6425_0001;
    int32_t t_call_641d;
    int32_t t_merge_646f_0001;
    int32_t t_call_6454;
    int32_t t_call_6467;

    switch (grobj) {
    case grobjPlanet:
    case grobjOther:
        if (pobj == 0x0) {
            if (grobj != grobjPlanet) {
                memset(&xfer.pl, 0, sizeof(PLANET));
                ppl = &xfer.pl;
            } else {
                FLookupPlanet(id, &xfer.pl);
                ppl = &xfer.pl;
            }
        } else {
            ppl = pobj;
        }
        if (iSupply <= 4) {
            if (iSupply == 4) {
                return 0;
            }
            if (dChg == 0) {
                return ppl->rgwtMin[iSupply];
            }
            if (ppl->rgwtMin[iSupply] + dChg < 0x0) {
                dChg = -ppl->rgwtMin[iSupply];
            }
            ppl->rgwtMin[iSupply] = ppl->rgwtMin[iSupply] + dChg;
        }
        if (dChg == 0 || pobj != 0x0 || grobj == grobjOther)
            break;
        FLookupPlanet(-1, &xfer.pl);
        break;
    case grobjThing:
        if (pobj == 0x0) {
            FLookupThing(id, &xfer.th);
            pth = &xfer.th;
        } else {
            pth = pobj;
        }
        if (iSupply < 3) {
            if (iSupply <= 4) {
                if (dChg == 0) {
                    return (int32_t)pth->thp.rgwtMin[iSupply];
                }
                if ((int32_t)pth->thp.rgwtMin[iSupply] + dChg < 0x0) {
                    dChg = (int32_t)-pth->thp.rgwtMin[iSupply];
                }
                wtFree = (uint32_t)(pth->thp.wtMax * 0xa);
                for (i = 0; i < 3; i++) {
                    wtFree = wtFree - (int32_t)pth->thp.rgwtMin[i];
                }
                if (dChg > wtFree) {
                    dChg = wtFree;
                }
                pth->thp.rgwtMin[iSupply] = pth->thp.rgwtMin[iSupply] + LOWORD(dChg);
            }
            if (dChg == 0 || pobj != 0x0)
                break;
            FLookupThing(-1, pth);
            break;
        }
        return 0;
    default:
        if (pobj == 0x0) {
            FLookupFleet(id, &xfer.fl);
            pfl = &xfer.fl;
        } else {
            pfl = pobj;
        }
        if (iSupply <= 4) {
            if (dChg == 0) {
                return pfl->rgwtMin[iSupply];
            }
            if (pfl->rgwtMin[iSupply] + dChg < 0x0) {
                dChg = -pfl->rgwtMin[iSupply];
            }
            if (iSupply == 3 && pfl->det != 0x7) {
                dChg = 0;
            }
            if (iSupply != 4) {
                t_call_641d = GetCargoFree(pfl);
                t_merge_6425_0001 = t_call_641d;
            } else {
                t_call_640a = GetFuelFree(pfl);
                t_merge_6425_0001 = t_call_640a;
            }
            if (dChg < t_merge_6425_0001) {
                t_merge_646f_0001 = dChg;
            } else if (iSupply != 4) {
                t_call_6467 = GetCargoFree(pfl);
                t_merge_646f_0001 = t_call_6467;
            } else {
                t_call_6454 = GetFuelFree(pfl);
                t_merge_646f_0001 = t_call_6454;
            }
            dChg = t_merge_646f_0001;
            pfl->rgwtMin[iSupply] = pfl->rgwtMin[iSupply] + dChg;
        }
        if (dChg != 0 && pobj == 0x0) {
            FLookupFleet(-1, pfl);
        }
    }
    return dChg;
}

int32_t XferSupply(int16_t iSupply, int32_t cQuan) {
    int16_t  iSrc;
    int32_t  dChg;
    int32_t  cAvailable;
    int16_t  t_merge_6510_0001;
    uint16_t t_merge_65d1_0001;
    uint16_t t_merge_65fd_0001;

    if (cQuan != 0) {
        t_merge_6510_0001 = cQuan <= 0 ? 0 : 1;
        iSrc = t_merge_6510_0001;
        if (iSrc == 0) {
            cQuan = -cQuan;
        }
        cAvailable = ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, 0, (uint8_t *)(pxfer + iSrc) + 4);
        if (cQuan > cAvailable) {
            cQuan = cAvailable;
        }
        if (cQuan != 0) {
            t_merge_65d1_0001 = iSrc == 0 ? 0x1 : 0x0;
            t_merge_65fd_0001 = iSrc == 0 ? 0x1 : 0x0;
            dChg = ChgCargo(pxfer[iSrc == 0 ? 1 : 0].grobj, pxfer[t_merge_65fd_0001].id, iSupply, cQuan, (uint8_t *)(pxfer + t_merge_65d1_0001) + 4);
            if (dChg != 0) {
                ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, -dChg, (uint8_t *)(pxfer + iSrc) + 4);
            }
            return dChg;
        }
        return 0;
    }
    return 0;
}

void UpdateXferBtns() {
    int16_t  iSide;
    int16_t  i;
    int16_t  iLastButton;
    int16_t  iVal;
    int32_t  lLeft;
    uint16_t t_merge_67af_0001;
    uint16_t t_merge_67dd_0001;
    int32_t  t_call_6861;
    int32_t  t_merge_688b_0001;
    int32_t  t_call_6883;

    iLastButton = mdXferDlg == mdXferShips ? cXferValidHulls * 2 : 4;
    for (i = 0; i < crgbtnXfer; i++) {
        iVal = rgbtnXfer[i].iVal;
        if (rgbtnXfer[i].fVisible != 0x0 && (iVal <= iLastButton || mdXferDlg == mdXferShips)) {
            iSide = rgbtnXfer[i].iSide;
            if (mdXferDlg != mdXferShips) {
                t_merge_67af_0001 = iSide == 0 ? 0x1 : 0x0;
                t_merge_67dd_0001 = iSide == 0 ? 0x1 : 0x0;
                lLeft = ChgCargo(pxfer[iSide == 0 ? 1 : 0].grobj, pxfer[t_merge_67dd_0001].id, iVal, 0, (uint8_t *)(pxfer + t_merge_67af_0001) + 4);
                if (lLeft == 0 || pxfer[iSide].grobj != grobjFleet) {
                    if (pxfer[iSide].grobj == grobjPlanet && iVal == 4) {
                        lLeft = 0;
                    }
                } else {
                    if (iVal != 4) {
                        t_call_6883 = GetCargoFree(&pxfer[iSide].fl);
                        t_merge_688b_0001 = t_call_6883;
                    } else {
                        t_call_6861 = GetFuelFree(&pxfer[iSide].fl);
                        t_merge_688b_0001 = t_call_6861;
                    }
                    lLeft = t_merge_688b_0001;
                }
            } else if (pxfer[iSide].fl.rgcsh[iVal] != 32766) {
                lLeft = (int32_t)pxfer[iSide == 0 ? 1 : 0].fl.rgcsh[iVal];
            } else {
                lLeft = 0;
            }
            if (lLeft != 0) {
                rgbtnXfer[i].bt = rgbtnXfer[i].bt & 0xfffb;
            } else {
                rgbtnXfer[i].bt = rgbtnXfer[i].bt | 0x4;
            }
        }
    }
    return;
}

void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iSupply) {
    RECT    rgrc[2];
    int16_t fCreatedDC;
    int16_t i;
    int16_t dxCtr;

    fCreatedDC = 0;
    if (hdc == 0x0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    dxCtr = prc->right >> 0x1;
    if (iSupply < 0) {
        PatBlt(hdc, dxCtr, 0, 1, prc->bottom, BLACKNESS);
        for (i = 0; i < crgbtnXfer; i++) {
            if (rgbtnXfer[i].fVisible != 0x0) {
                DrawBtn(hdc, &rgbtnXfer[i].rc, rgbtnXfer[i].bt, 0, 0x0);
            }
        }
        if (iSupply == -2)
            goto RelDC;
    }
    GetXferLeftRightRcs(prc, rgrc, &rgrc[1]);
    for (i = 0; i < 2; i++) {
        if (mdXferDlg != mdXferShips) {
            switch (pxfer[i].grobj) {
            case grobjFleet:
                DrawFleetCargoXferSide(hdc, &rgrc[i], &pxfer[i].fl, iSupply);
                break;
            case grobjPlanet:
            case grobjOther:
                DrawPlanetXferSide(hdc, &rgrc[i], &pxfer[i].pl, iSupply);
                break;
            case grobjThing:
                DrawThingXferSide(hdc, &rgrc[i], &pxfer[i].th, iSupply);
            default:
            }
        } else {
            DrawFleetShipsXferSide(hdc, &rgrc[i], &pxfer[i].fl, iSupply);
        }
    }
RelDC:
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

void GetXferLeftRightRcs(RECT *prcWhole, RECT *prcLeft, RECT *prcRight) {
    SetRect(prcLeft, 0, 0, prcWhole->right >> 0x1, prcWhole->bottom);
    ExpandRc(prcLeft, -(dyArial8 + 3) - 4, -4);
    prcLeft->left = prcLeft->left - (dyArial8 + 1);
    SetRect(prcRight, prcWhole->right >> 0x1, 0, prcWhole->right, prcWhole->bottom);
    ExpandRc(prcRight, -(dyArial8 + 3) - 4, -4);
    prcRight->right = prcRight->right + (dyArial8 + 1);
    return;
}

int16_t FSetupXferBtns(RECT *prc) {
    int16_t cBtn;
    int16_t iMax;
    int16_t dy;
    int16_t iMin;
    int16_t i;
    int16_t fThingXfer;
    int16_t j;
    int16_t dxCtr;
    RECT    rcRight;
    int16_t dxLabels;
    RECT    rcBtn;
    RECT    rcLeft;
    RECT    rc;

    cBtn = 0;
    dxLabels = mdXferDlg == mdXferShips ? 140 : 75;
    fThingXfer = pxfer[1].grobj == grobjThing ? 1 : 0;
    dxCtr = prc->right >> 0x1;
    dy = dyArial8 + 10;
    iMax = mdXferDlg == mdXferShips ? cXferValidHulls : 5;
    if (mdXferDlg != mdXferShips) {
        dy = dy + (dyArial8 + 6) * 2;
    }
    i = 0;
    while (i < iMax) {
        if (i == 4 && mdXferDlg != mdXferShips) {
            dy = dy - (dyArial8 + 6) * 6;
        }
        SetRect(&rcBtn, dxCtr - (dyArial8 + 3) + 1, dy, dxCtr + 1, dyArial8 + 3 + dy);
        for (j = 0; j < 2; j++) {
            rgbtnXfer[cBtn].rc = rcBtn;
            rgbtnXfer[cBtn].bt = j == 0 ? 2 : 3;
            if (fThingXfer == 0 || i < 4) {
                rgbtnXfer[cBtn].fVisible = 0x1;
            } else {
                rgbtnXfer[cBtn].fVisible = 0x0;
                rgbtnXfer[cBtn].rc.bottom = -100;
            }
            rgbtnXfer[cBtn].iSide = j;
            rgbtnXfer[cBtn].iVal = mdXferDlg == mdXferShips ? rgXferValidHulls[i] : i;
            cBtn = cBtn + 1;
            OffsetRc(&rcBtn, dyArial8 + 2, 0);
        }
        i = i + 1;
        dy = dy + (dyArial8 + 6);
    }
    GetXferLeftRightRcs(prc, &rcLeft, &rcRight);
    if (mdXferDlg != mdXferShips) {
        if (pxfer->grobj == grobjPlanet) {
            if (pxfer[1].grobj == grobjPlanet)
                goto NoGauges;
            rc = rcRight;
            i = 1;
        } else {
            rc = rcLeft;
            i = 0;
        }
        for (; i < 2; i++) {
            SetRect(&rcBtn, rc.left + dxLabels + 10, rc.top + dyArial8 + 6, rc.right - 4, dyArial8 * 2 + rc.top + 6);
            if (mdXferDlg != mdXferShips) {
                iMin = 0;
                iMax = 5;
            } else {
                iMin = 0;
                iMax = cXferValidHulls;
            }
            for (j = iMin; j < iMax; j++) {
                if (mdXferDlg != mdXferShips) {
                    if (j != 0) {
                        if (j == 4) {
                            OffsetRc(&rcBtn, 0, -(dyArial8 + 6) * 6);
                        }
                    } else {
                        OffsetRc(&rcBtn, 0, (dyArial8 + 6) * 2);
                    }
                }
                rgbtnXfer[cBtn].rc = rcBtn;
                rgbtnXfer[cBtn].bt = 0;
                rgbtnXfer[cBtn].fVisible = 0x0;
                rgbtnXfer[cBtn].iSide = i;
                rgbtnXfer[cBtn].iVal = j + 128;
                cBtn = cBtn + 1;
                OffsetRc(&rcBtn, 0, dyArial8 + 6);
            }
            if (pxfer[1].grobj == grobjPlanet)
                break;
            rc = rcRight;
        }
    }
NoGauges:
    crgbtnXfer = cBtn;
    UpdateXferBtns();
    return 1;
}

void DrawThingXferSide(HDC hdc, RECT *prc, THING *pth, int16_t iSupply) {
    int16_t yTop;
    int16_t i;
    int16_t xRight;
    int16_t dxLabels;
    RECT    rcGauge;
    int16_t xLeft;
    RECT    rc;

    dxLabels = 75;
    rc = *prc;
    rc.bottom = rc.top + rc.right - rc.left;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == -1) {
        RcCtrTextOut(hdc, &rc, PszGetThingName(pth->idFull), 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3 + (dyArial8 + 6);
    if (iSupply == -1) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = -1; i < 3; i++) {
            if (i != 4 && i != 4) {
                RightTextOut(hdc, xLeft + dxLabels, yTop, PszGetCompressedString(i == -1 ? idsPacketShell : i + 430), 0, 0);
            }
            yTop = yTop + (dyArial8 + 6);
        }
    }
    if (iSupply != 4 && iSupply != 3) {
        yTop = rc.bottom + 3 + (dyArial8 + 6);
        xLeft = xLeft + (dxLabels + 6);
        SetRect(&rcGauge, xLeft, yTop, xRight, yTop + dyArial8);
        DrawThingGauge(hdc, &rcGauge, pth, 5);
        for (i = 0; i < 3; i++) {
            OffsetRc(&rcGauge, 0, dyArial8 + 6);
            if (iSupply == -1 || iSupply == i) {
                DrawThingGauge(hdc, &rcGauge, pth, i);
                if (iSupply == i)
                    break;
            }
        }
    }
    return;
}

void DrawFleetCargoXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply) {
    int16_t yTop;
    int16_t fOtherPlr;
    int16_t c;
    int16_t i;
    int16_t xRight;
    FLEET   fl;
    int16_t dxLabels;
    RECT    rcGauge;
    int16_t xLeft;
    RECT    rc;
    int16_t iMap;

    fOtherPlr = pfl->iPlayer == idPlayer ? 0 : 1;
    dxLabels = 75;
    fl = *pfl;
    rc = *prc;
    rc.bottom = rc.top + rc.right - rc.left;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == -1) {
        RcCtrTextOut(hdc, &rc, PszGetFleetName(fl.id), 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3;
    if (iSupply == -1) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < 6; i++) {
            if (i != 6 || fOtherPlr == 0) {
                RightTextOut(hdc, xLeft + dxLabels, yTop, PszGetCompressedString(i + 428), 0, 0);
            }
            yTop = yTop + (dyArial8 + 6);
        }
    }
    yTop = rc.bottom + 3;
    xLeft = xLeft + (dxLabels + 6);
    if (fOtherPlr == 0) {
        SetRect(&rcGauge, xLeft, yTop, xRight, yTop + dyArial8);
        if (iSupply == -1 || iSupply == 4) {
            DrawFleetGauge(hdc, &rcGauge, &fl, 4);
        }
        if (iSupply != 4) {
            yTop = yTop + (dyArial8 + 6);
            OffsetRc(&rcGauge, 0, dyArial8 + 6);
            DrawFleetGauge(hdc, &rcGauge, &fl, 5);
            yTop = yTop + (dyArial8 + 6);
            for (i = 0; i <= 3; i++) {
                OffsetRc(&rcGauge, 0, dyArial8 + 6);
                if (iSupply == -1 || iSupply == i) {
                    DrawFleetGauge(hdc, &rcGauge, &fl, i);
                    if (iSupply == i)
                        break;
                }
                yTop = yTop + (dyArial8 + 6);
            }
        }
    } else {
        xRight = xLeft + dxMaxMineralQuan;
        SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 2, yTop + dyArial8 + 1);
        i = 0;
        while (i < 6) {
            if (i != 1) {
                if (i != 0) {
                    iMap = i - 2;
                } else {
                    iMap = 4;
                }
                if (iSupply == -1 || iSupply == iMap) {
                    _Draw3dFrame(hdc, &rc, iSupply == iMap ? 1 : 0);
                    c = _wsprintf(szWork, PszGetCompressedString((iMap == 4 ? 0x1 : 0x0) + 0x37c), fl.rgwtMin[iMap]);
                    RightTextOut(hdc, xRight, yTop, szWork, c, 0);
                    if (iSupply == i)
                        break;
                }
                OffsetRc(&rc, 0, dyArial8 + 6);
            } else {
                OffsetRc(&rc, 0, dyArial8 + 6);
            }
            i = i + 1;
            yTop = yTop + (dyArial8 + 6);
        }
    }
    return;
}

void DrawFleetShipsXferSide(HDC hdc, RECT *prc, FLEET *pfl, int16_t iSupply) {
    int16_t yTop;
    int16_t fOtherPlr;
    int16_t c;
    int16_t i;
    int16_t xRight;
    FLEET   fl;
    int16_t xLeft;
    RECT    rc;

    fOtherPlr = pfl->iPlayer == idPlayer ? 0 : 1;
    fl = *pfl;
    rc = *prc;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == -1) {
        RcCtrTextOut(hdc, &rc, PszGetFleetName(fl.id), 0);
    }
    xLeft = prc->right - 4 - dxMaxMineralQuan - 2;
    xRight = xLeft + dxMaxMineralQuan;
    yTop = rc.bottom + 3;
    if (iSupply == -1) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < cXferValidHulls; i++) {
            RightTextOut(hdc, xLeft - 8, yTop, rgshdef[rgXferValidHulls[i]].hul.szClass, 0, 0);
            yTop = yTop + (dyArial8 + 6);
        }
    }
    yTop = rc.bottom + 3;
    SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 2, yTop + dyArial8 + 1);
    for (i = 0; i < cXferValidHulls; i++) {
        if (iSupply == -1 || iSupply == i) {
            _Draw3dFrame(hdc, &rc, iSupply == i ? 1 : 0);
            c = _wsprintf(szWork, PCTD, pfl->rgcsh[rgXferValidHulls[i]]);
            RightTextOut(hdc, xRight, yTop, szWork, c, 0);
            if (iSupply == i)
                break;
        }
        OffsetRc(&rc, 0, dyArial8 + 6);
        yTop = yTop + (dyArial8 + 6);
    }
    return;
}

void DrawPlanetXferSide(HDC hdc, RECT *prc, PLANET *ppl, int16_t iSupply) {
    PLANET  pl;
    int16_t yTop;
    int16_t c;
    int16_t i;
    int16_t xRight;
    char   *psz;
    int16_t xLeft;
    RECT    rc;

    pl = *ppl;
    rc = *prc;
    rc.bottom = rc.top + rc.right - rc.left;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == -1) {
        _Draw3dFrame(hdc, &rc, 0);
        if (pl.id == -1) {
            psz = PszGetCompressedString(idsDeepSpace);
        } else {
            psz = PszGetPlanetName(pl.id);
        }
        RcCtrTextOut(hdc, &rc, psz, 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3;
    if (iSupply == -1) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < 6; i++) {
            if (i > 1) {
                RightTextOut(hdc, xLeft + 75, yTop, PszGetCompressedString(i + 428), 0, 0);
            }
            yTop = yTop + (dyArial8 + 6);
        }
    }
    yTop = rc.bottom + 3;
    xLeft = xLeft + 81;
    xRight = xLeft + dxMaxMineralQuan + 12;
    SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 14, yTop + dyArial8 + 1);
    i = 0;
    while (i <= 4) {
        if (i != 0) {
            if (i == 4) {
                yTop = yTop - (dyArial8 + 6) * 6;
                OffsetRc(&rc, 0, (dyArial8 + 6) * 6);
            }
        } else {
            yTop = yTop + (dyArial8 + 6) * 2;
            OffsetRc(&rc, 0, (dyArial8 + 6) * 2);
        }
        if ((iSupply == -1 || iSupply == i) && i != 4) {
            _Draw3dFrame(hdc, &rc, iSupply == i ? 1 : 0);
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pl.rgwtMin[i]);
            RightTextOut(hdc, xRight, yTop, szWork, c, 0);
            if (iSupply == i)
                break;
        }
        OffsetRc(&rc, 0, dyArial8 + 6);
        i = i + 1;
        yTop = yTop + (dyArial8 + 6);
    }
    return;
}

HCURSOR ClickInShipOrders(POINT16 pt, int16_t sks, int16_t fCursor, int16_t fRightBtn) {
    int32_t    lCur;
    HDC        hdc;
    PLANET     pl;
    int16_t    iWarp;
    POINT16    ptOld;
    int16_t    idPlan;
    int32_t    lMax;
    int32_t    lSel;
    int16_t    iSkip;
    int32_t    xRnd;
    int16_t    grbit;
    XFER       xf;
    int32_t    lNew;
    int16_t    irc;
    int32_t    dx;
    int32_t    lTempMin;
    int16_t    fFirst;
    int16_t    fTwoMAs;
    int32_t    lTempMax;
    int16_t    cMax;
    char       sz255[2];
    int16_t    i;
    char      *rgszZip[11];
    ZIPORDER   rgzo[4];
    FARPROC    lpProc;
    int16_t    fRet;
    TASKXPORT *lptxp;
    int16_t    fSep;
    int16_t    c;
    ORDER     *lpord;
    THING     *lpth;
    FLEET     *lpfl;
    int32_t    rgid[100];
    int16_t    iChecked;
    THING     *lpthMac;
    SCAN       scan;
    int16_t    t_7db3;
    int16_t    t_7ddf;
    int16_t    t_7e02;
    int16_t    t_83ee;
    int16_t    t_84ac;
    int16_t    t_84ec;
    int32_t   *t_assign_1;
    int32_t   *t_assign_2;
    int32_t    t_merge_8c47_0001;
    int32_t    t_merge_8c7d_0001;
    int32_t    t_merge_8d3f_0001;
    int32_t    t_merge_8d93_0001;

    lTempMin = 0;
    irc = -1;
    if (sel.grobj != grobjNone) {
        if (PtInRect(&rgrcRef[5], PointFrom16(pt)) == 0) {
            if (PtInRect(&rgrcRef[12], PointFrom16(pt)) == 0) {
                if (fRightBtn != 0) {
                    return 0x0;
                }
                if (PtInRect(rgrcRef, PointFrom16(pt)) == 0) {
                    if (PtInRect(&rgrcRef[15], PointFrom16(pt)) == 0) {
                        if (PtInRect(&rgrcRef[1], PointFrom16(pt)) == 0) {
                            if (PtInRect(&rgrcRef[3], PointFrom16(pt)) != 0) {
                                if (fCursor == 0) {
                                    if (sel.fl.idPlanet == -1) {
                                        lpth = lpThings;
                                        lpthMac = lpThings + cThing;
                                        for (; lpth < lpthMac && (lpth->ith != ithMineralPacket || sel.fl.pt.x != lpth->pt.x || sel.fl.pt.y != lpth->pt.y);
                                             lpth++) {
                                        }
                                        if (lpth != lpthMac) {
                                            MessageBeep(0x0);
                                        } else {
                                            TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
                                        }
                                    } else {
                                        TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
                                    }
                                    return 0x0;
                                }
                                return hcurHand;
                            }
                            if (PtInRect(&rgrcRef[4], PointFrom16(pt)) != 0) {
                                if (fCursor == 0) {
                                    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                    if (lSel != -1) {
                                        if (FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.grobj == grobjFleet ? sel.fl.id : -1) != 0) {
                                            TransferStuff(sel.id, sel.grobj, xf.id, xf.grobj, mdXferCargo);
                                        }
                                        return 0x0;
                                    }
                                    return 0x0;
                                }
                                return hcurHand;
                            }
                            if (PtInRect(&rgrcRef[18], PointFrom16(pt)) != 0) {
                                if ((sel.grobj != grobjFleet && fCursor != 0) || sel.fl.lpplord->rgord[sel.iwpAct].grTask != grTaskPatrol) {
                                    return 0x0;
                                }
                                irc = 18;
                                lTempMax = 10;
                                lMax = 10;
                                lCur = (uint32_t)sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
                                grbit = 7;
                            }
                        } else {
                            if (sel.grobj != grobjFleet) {
                                iSkip = -1;
                                idPlan = sel.pl.id;
                            } else {
                                idPlan = sel.fl.idPlanet;
                                iSkip = sel.fl.id;
                            }
                            lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                            FLookupOrbitingXfer(idPlan, LOWORD(lSel), &xf, iSkip);
                            if (xf.grobj != grobjFleet || xf.fl.iPlayer != idPlayer) {
                                return 0x0;
                            }
                            irc = 1;
                            lMax = LGetFleetStat(&xf.fl, 1);
                            lCur = xf.fl.rgwtMin[4];
                            grbit = 4;
                            if (sel.grobj != grobjFleet) {
                                lTempMax = lCur;
                            } else {
                                lTempMax = lCur + sel.fl.rgwtMin[4];
                                lTempMin = lCur - (LGetFleetStat(&sel.fl, 1) - sel.fl.rgwtMin[4]);
                            }
                        }
                    } else {
                        irc = 15;
                        iWarp = IWarpMAFromLppl(&sel.pl, &fTwoMAs);
                        lTempMax = (int32_t)(iWarp - 1);
                        lMax = (int32_t)(iWarp - 1);
                        lTempMin = 1;
                        lCur = sel.pl.iWarpFling;
                    }
                } else {
                    if (sel.grobj != grobjFleet && fCursor != 0) {
                        return 0x0;
                    }
                    irc = 0;
                    lTempMax = 11;
                    lMax = 11;
                    lCur = sel.fl.lpplord->rgord[sel.iwpAct].iWarp;
                    grbit = 6;
                }
            } else {
                if (fCursor != 0) {
                    return hcurArrowHelp;
                }
                if (fRightBtn == 0) {
                    GlobalPD.grPopup = grPopupString;
                    GlobalPD.dxOut = 180;
                    GlobalPD.psz = szPopupBuffer;
                    CchGetString(idsRightClickBlueDiamondBringPopupMenu, szPopupBuffer);
                    Popup(hwndPlanet, pt.x, pt.y);
                } else {
                    iChecked = -1;
                    lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
                    FFindNearestObject(lpord->pt, 0x8f, &scan);
                    if (scan.idpl == -1) {
                        rgid[0] = 268435456;
                    } else {
                        rgid[0] = (int32_t)scan.idpl;
                    }
                    rgid[1] = -1;
                    c = 2;
                    if (lpord->grobj == grobjPlanet || lpord->grobj == grobjOther) {
                        iChecked = 0;
                    }
                    for (i = 0; i < cFleet; i++) {
                        lpfl = rglpfl[i];
                        if (rglpfl[i] == 0x0)
                            break;
                        if (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y && lpfl->id != sel.fl.id) {
                            if (lpord->grobj == grobjFleet && lpord->id == lpfl->id) {
                                iChecked = c;
                            }
                            t_83ee = c;
                            c = c + 1;
                            rgid[t_83ee] = (int32_t)lpfl->id | 0x80000000;
                            if (c >= 100)
                                break;
                        }
                    }
                    if (c == 2) {
                        c = 1;
                    }
                    fSep = c == 0 ? 1 : 0;
                    lpth = lpThings;
                    lpthMac = lpThings + cThing;
                    for (; lpth < lpthMac; lpth++) {
                        if (scan.pt.x == lpth->pt.x && scan.pt.y == lpth->pt.y) {
                            if (fSep == 0) {
                                if (c >= 100)
                                    break;
                                t_84ac = c;
                                c = c + 1;
                                rgid[t_84ac] = -1;
                                fSep = 1;
                            }
                            if (c >= 100)
                                break;
                            t_84ec = c;
                            c = c + 1;
                            t_assign_1 = &rgid[t_84ec];
                            *t_assign_1 = (int32_t)(((uint32_t)*t_assign_1 & 0xffff0000) | ((uint32_t)lpth->idFull & 0xffff));
                            t_assign_2 = &rgid[t_84ec];
                            *t_assign_2 = (int32_t)(((uint32_t)*t_assign_2 & 0xffff) | ((uint32_t)0x2000 & 0xffff) << 0x10);
                        }
                    }
                    i = PopupMenu(hwndPlanet, pt.x, pt.y, c, rgid, 0x0, iChecked, 1);
                    if (i >= 0) {
                        if (i != 0 || rgid[0] != 268435456) {
                            if ((rgid[i] & 0x20000000) != 0x0) {
                                lpord->grobj = grobjThing;
                                lpord->id = LOWORD(rgid[i]);
                            } else if ((rgid[i] & 0x80000000) != 0x0) {
                                lpord->grobj = grobjFleet;
                                lpord->id = LOWORD(rgid[i]);
                            } else {
                                lpord->grobj = grobjPlanet;
                                lpord->id = LOWORD(rgid[0]);
                            }
                        } else {
                            lpord->grobj = grobjOther;
                            lpord->id = -1;
                        }
                        FLookupFleet(-1, &sel.fl);
                        FillOrdersLB();
                        SetOrdersLbSel(sel.iwpAct);
                    }
                }
            }
        } else {
            if (fCursor != 0) {
                return hcurArrowHelp;
            }
            if (fRightBtn == 0) {
                GlobalPD.grPopup = grPopupShipOrders;
                Popup(hwndPlanet, pt.x, pt.y);
            } else {
                sz255[0] = -1;
                sz255[1] = 0;
                for (i = 0; i < 4; i++) {
                    rgszZip[i] = rgszZipOrder[i];
                }
                rgszZip[4] = sz255;
                cMax = 5;
                for (i = 0; i < 4; i++) {
                    if (vrgZip[i].fValid != 0x0) {
                        t_7db3 = cMax;
                        cMax = cMax + 1;
                        rgszZip[t_7db3] = vrgZip[i].szName;
                    }
                }
                if (cMax > 5) {
                    t_7ddf = cMax;
                    cMax = cMax + 1;
                    rgszZip[t_7ddf] = sz255;
                }
                t_7e02 = cMax;
                cMax = cMax + 1;
                rgszZip[t_7e02] = PszGetCompressedString(idsCustomize);
                i = PopupMenu(hwndPlanet, pt.x, pt.y, cMax, 0x0, rgszZip, -1, 1);
                if (i != cMax - 1) {
                    if (i <= 4) {
                        if (i == -1)
                            goto L_8b82;
                        lptxp = (TASKXPORT *)&sel.fl.lpplord->rgord[sel.iwpAct].txp;
                        switch (i) {
                        case 0:
                            lptxp->rgia[4].iAction = iActionLoadDunnage;
                            for (i = 0; i < 3; i++) {
                                lptxp->rgia[i].iAction = iActionLoadAll;
                            }
                            lptxp->rgia[3].iAction = iActionNone;
                            break;
                        case 1:
                            lptxp->rgia[4].iAction = iActionLoadDunnage;
                            for (i = 0; i <= 3; i++) {
                                lptxp->rgia[i].iAction = iActionUnloadAll;
                            }
                            break;
                        case 2:
                            lptxp->rgia[4].iAction = iActionLoadDunnage;
                            for (i = 0; i < 3; i++) {
                                lptxp->rgia[i].iAction = iActionWaitPercent;
                                lptxp->rgia[i].cQuan = 0x64;
                            }
                            lptxp->rgia[3].iAction = iActionNone;
                            break;
                        case 3:
                            for (i = 0; i < 5; i++) {
                                lptxp->rgia[i].iAction = iActionNone;
                                lptxp->rgia[i].cQuan = 0x0;
                            }
                        default:
                        }
                    } else {
                        i = i - 4;
                        iSkip = 0;
                        while (i != 0) {
                            if (vrgZip[iSkip].fValid != 0x0) {
                                i = i - 1;
                                if (i == 0)
                                    break;
                            }
                            iSkip = iSkip + 1;
                        }
                        sel.fl.lpplord->rgord[sel.iwpAct].txp = vrgZip[iSkip].txp;
                    }
                    FLookupFleet(-1, &sel.fl);
                    UpdateOrdersDDs(1);
                    DrawPlanShip(0x0, 256);
                } else {
                    memcpy(rgzo, vrgZip, 0x60);
                    lpProc = MakeProcInstance(ZipOrderDlg, hInst);
                    fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_ZIP_PROD), hwndFrame, lpProc);
                    FreeProcInstance(lpProc);
                    if (fRet == 0) {
                        memcpy(vrgZip, rgzo, 0x60);
                    }
                }
            }
        }
    L_8b82:
        if (irc != -1) {
            if (fCursor == 0) {
                dx = (int32_t)(rgrcRef[irc].right - rgrcRef[irc].left - 2);
                xRnd = (int32_t)((int32_t)(dx / (lMax + 0x1)) >> 0x1);
                hdc = GetDC(hwndPlanet);
                SetCapture(hwndPlanet);
                ptOld.y = -1;
                ptOld.x = -1;
                t_merge_8c47_0001 = lMax < lTempMax ? lMax : lTempMax;
                lTempMax = t_merge_8c47_0001;
                t_merge_8c7d_0001 = 0 <= lTempMin ? lTempMin : 0;
                lTempMin = t_merge_8c7d_0001;
                fFirst = 1;
                while (fFirst != 0 || FGetMouseMove(&pt) != 0) {
                    fFirst = 0;
                    if (pt.x != ptOld.x || pt.y != ptOld.y) {
                        ptOld = pt;
                        lNew = (int32_t)((int32_t)(((int32_t)(pt.x - rgrcRef[irc].left) + xRnd) * lMax) / dx);
                        t_merge_8d3f_0001 = lNew < lTempMax ? lNew : lTempMax;
                        if (lTempMin <= t_merge_8d3f_0001) {
                            if (lNew < lTempMax) {
                                t_merge_8d93_0001 = lNew;
                            } else {
                                t_merge_8d93_0001 = lTempMax;
                            }
                        } else {
                            t_merge_8d93_0001 = lTempMin;
                        }
                        lNew = t_merge_8d93_0001;
                        if (lNew != lCur) {
                            switch (irc) {
                            case 0:
                                sel.fl.lpplord->rgord[sel.iwpAct].iWarp = LOWORD(lNew);
                                DrawPlanShip(0x0, 16416);
                                break;
                            case 18:
                                sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = LOWORD(lNew);
                                DrawPlanShip(0x0, 16640);
                                break;
                            case 15:
                                DrawMassWarpGauge(hdc, &rgrcRef[15], fTwoMAs == 0 ? iWarp : -iWarp, LOWORD(lNew) + 4);
                                break;
                            case 2:
                                sel.fl.rgwtMin[4] = lNew;
                                DrawFleetGauge(hdc, &rgrcRef[irc], 0x0, grbit);
                                break;
                            case 1:
                                if (sel.grobj != grobjFleet) {
                                    DrawPlanShip(0x0, 16385);
                                } else {
                                    sel.fl.rgwtMin[4] = sel.fl.rgwtMin[4] - (lNew - lCur);
                                    DrawFleetGauge(hdc, &rgrcRef[2], &sel.fl, grbit);
                                }
                                xf.fl.rgwtMin[4] = lNew;
                                DrawFleetGauge(hdc, &rgrcRef[irc], &xf.fl, grbit);
                            default:
                            }
                            lCur = lNew;
                        }
                    }
                }
                grbit = sel.scan.grobj == grobjOther ? sel.scan.grobjFull : sel.scan.grobj;
                switch (irc) {
                case 2:
                    FLookupFleet(-1, &sel.fl);
                    FLookupPlanet(-1, &pl);
                    DrawPlanShip(0x0, 16928);
                    if ((grbit & 0x1) == 0x0 || sel.fl.idPlanet != sel.scan.idpl) {
                        if ((grbit & 0x2) == 0x0 || sel.fl.id != rglpfl[sel.scan.ifl]->id)
                            break;
                        InvalidateRect(hwndMine, 0x0, 1);
                        break;
                    }
                    goto FixMinWin;
                case 1:
                    FLookupFleet(-1, &xf.fl);
                    if (sel.grobj != grobjFleet) {
                        FLookupPlanet(-1, &sel.pl);
                        if ((grbit & 0x1) != 0x0 && sel.pl.id == sel.scan.idpl)
                            goto FixMinWin;
                        break;
                    }
                    FLookupFleet(-1, &sel.fl);
                    DrawPlanShip(0x0, 16929);
                    break;
                case 0:
                case 18:
                    FLookupFleet(-1, &sel.fl);
                    break;
                case 15:
                    if (LOWORD(lCur) != sel.pl.iWarpFling) {
                        sel.pl.iWarpFling = LOWORD(lCur);
                        FLookupPlanet(-1, &sel.pl);
                    }
                default:
                }
                goto L_914e;
            FixMinWin:
                InvalidateMineralBars();
            L_914e:
                ReleaseCapture();
                return (HCURSOR)(uintptr_t)ReleaseDC(hwndPlanet, hdc);
            }
            return hcurHand;
        }
        return 0x0;
    }
    return 0x0;
}

void FillFleetCompLB() {
    int16_t  i;
    int32_t  pctDmg;
    int16_t  t_merge_922e_0001;
    uint16_t t_merge_924a_0001;

    SendMessage(hwndFleetCompLB, LB_RESETCONTENT, 0x0, 0);
    for (i = 0; i < 16; i++) {
        if (sel.fl.rgcsh[i] > 0) {
            pctDmg = (int32_t)((int32_t)((uint32_t)(sel.fl.rgdv[i].pctSh * sel.fl.rgdv[i].pctDp) + 0xfa) / 0x1f4);
            t_merge_922e_0001 = pctDmg == 0 ? 32 : (int16_t)LOBYTE(LOWORD(pctDmg));
            t_merge_924a_0001 = pctDmg == 0 ? 0x51 : 0x50;
            _wsprintf(szWork, "%c%c%5d%s", t_merge_924a_0001, t_merge_922e_0001, sel.fl.rgcsh[i], rgshdef[i].hul.szClass);
            SendMessage(hwndFleetCompLB, LB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
    }
    return;
}

void FillOrdersLB() {
    int16_t i;
    char   *psz;
    ORDER   ord;

    SendMessage(hwndShipLB, LB_RESETCONTENT, 0x0, 0);
    for (i = 0; i < sel.fl.cord; i++) {
        ord = sel.fl.lpplord->rgord[i];
        psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
        SendMessage(hwndShipLB, LB_ADDSTRING, 0x0, (LPARAM)psz);
    }
    SetOrdersLbSel(sel.iwpAct);
    if (sel.grobj == grobjFleet) {
        DrawPlanShip(0x0, 288);
    }
    return;
}

void SetOrdersLbSel(int16_t iSel) {
    SendMessage(hwndShipLB, LB_SETCURSEL, iSel, 0);
    if (iSel > (gd.fSmallTileMode == 0x0 ? 0x2 : 0x1)) {
        SendMessage(hwndShipLB, LB_SETTOPINDEX, iSel - (gd.fSmallTileMode == 0x0 ? 0x2 : 0x1), 0);
    }
    UpdateWindow(hwndShipLB);
    UpdateOrdersDDs(0);
    return;
}

void UpdateOrdersDDs(int16_t iLevel) {
    int32_t  rglSel[3];
    int16_t  iMin;
    int16_t  i;
    char    *psz;
    int16_t  iSel;
    int16_t  iMax;
    char     szT[80];
    int16_t  t_merge_94c2_0001;
    uint16_t t_merge_977d_0001;

    iSel = -1;
    if (iLevel != 0) {
        rglSel[0] = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0x0, 0);
    } else {
        rglSel[0] = SendMessage(rghwndOrderDD[0], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].grTask, 0);
    }
    if (iLevel > 1) {
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0);
        if (rglSel[0] != 1 || iLevel > 3)
            goto L_987f;
        iSel = LOWORD(rglSel[1]);
        if (iSel != 0) {
            iSel = iSel - 1;
        } else {
            iSel = 4;
        }
    }
    SendMessage(rghwndOrderDD[1], CB_RESETCONTENT, 0x0, 0);
    switch (rglSel[0]) {
    case 1:
        t_merge_94c2_0001 = LGetFleetStat(&sel.fl, 2) == 0 ? 1 : 5;
        iMax = t_merge_94c2_0001;
        for (i = 0; i < iMax; i++) {
            if (i != 0) {
                iMin = i - 1;
            } else {
                iMin = 4;
            }
            strcpy(&szWork[1], rgszMinerals[iMin]);
            if (sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iMin].iAction == iActionNone) {
                szWork[0] = ' ';
            } else {
                szWork[0] = '*';
                if (iSel == -1) {
                    iSel = iMin;
                }
            }
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
        if (iSel != -1 && iSel != 4) {
            iSel = iSel + 1;
        } else {
            iSel = 0;
        }
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 7:
        psz = PszGetCompressedString(idsWithinDLY);
        for (i = 0; i < 11; i++) {
            _wsprintf(szWork, psz, 50 * i + 0x32);
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
        psz = PszGetCompressedString(idsAnyEnemy);
        SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)psz);
        iSel = sel.fl.lpplord->rgord[sel.iwpAct].tptl.iDist;
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 9:
        szT[0] = ' ';
        for (i = 0; i < game.cPlayer; i++) {
            if (i != idPlayer) {
                psz = PszPlayerName(i, 1, 1, 1, 0, 0x0);
                strcpy(&szT[1], psz);
                SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)szT);
            }
        }
        iSel = sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 6:
        for (i = 0; i < 5; i++) {
            t_merge_977d_0001 = i == 0 ? 0x20 : 0x73;
            _wsprintf(szWork, PszGetCompressedString(idsDYearC), i + 1, t_merge_977d_0001);
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
        SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(idsIindefinitely));
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX, 0);
    default:
    }
L_987f:
    if (iLevel > 2) {
        rglSel[2] = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0x0, 0);
    } else {
        SendMessage(rghwndOrderDD[2], CB_RESETCONTENT, 0x0, 0);
        if (rglSel[0] == 1) {
            for (i = 109; i < 119; i++) {
                if (i != 116 || rglSel[1] != 0) {
                    psz = PszGetCompressedString(i);
                } else {
                    psz = PszGetCompressedString(idsLoadOptimal);
                }
                SendMessage(rghwndOrderDD[2], CB_ADDSTRING, 0x0, (LPARAM)psz);
            }
            iSel = LOWORD(rglSel[1]);
            if (iSel != 0) {
                iSel = iSel - 1;
            } else {
                iSel = 4;
            }
            rglSel[2] = SendMessage(rghwndOrderDD[2], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iSel].iAction, 0);
        }
    }
    if (iLevel <= 3 && rglSel[0] == 1) {
        iSel = LOWORD(rglSel[1]);
        if (iSel != 0) {
            iSel = iSel - 1;
        } else {
            iSel = 4;
        }
        _wsprintf(szWork, "%u", sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iSel].cQuan);
        SetWindowText(hwndOrderED, szWork);
    }
    return;
}

void FillBattleDD(int16_t iSel) {
    int16_t i;

    SendMessage(hwndBattleDD, CB_RESETCONTENT, 0x0, 0);
    CchGetString(idsBattlePlans, szWork);
    SendMessage(hwndBattleDD, CB_ADDSTRING, 0x0, (LPARAM)szWork);
    for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
        fstrcpy(szWork, rglpbtlplan[idPlayer][i].szName);
        SendMessage(hwndBattleDD, CB_ADDSTRING, 0x0, (LPARAM)szWork);
    }
    SendMessage(hwndBattleDD, CB_SETCURSEL, iSel, 0);
    return;
}

void DeleteCurWayPoint(int16_t fBackup) {
    POINT16 pt;
    POINT16 rgpt[3];
    int16_t cpt;
    SCAN    scan;
    int16_t ipt;
    RECT    rc;

    if (sel.fl.cord >= 2 && sel.iwpAct != 0) {
        if ((grbitScan & 0x80) != 0x0) {
            rgpt[0] = sel.fl.lpplord->rgord[sel.iwpAct].pt;
            rgpt[1] = sel.fl.lpplord->rgord[sel.iwpAct - 1].pt;
            if (sel.iwpAct >= sel.fl.cord - 1) {
                cpt = 2;
            } else {
                cpt = 3;
                rgpt[2] = sel.fl.lpplord->rgord[sel.iwpAct + 1].pt;
            }
        }
        RedrawScanSel(0x0, 0);
        fmemmove(&sel.fl.lpplord->rgord[sel.iwpAct], &sel.fl.lpplord->rgord[sel.iwpAct + 1], (sel.fl.cord - sel.iwpAct - 1) * sizeof(ORDER));
        sel.fl.cord = sel.fl.cord - 1;
        sel.fl.lpplord->iordMac = sel.fl.lpplord->iordMac - 0x1;
        sel.iwpAct = sel.iwpAct - 1;
        if (sel.iwpAct < sel.fl.cord - 1) {
            pt = sel.fl.lpplord->rgord[sel.iwpAct].pt;
            if (pt.x == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.x && pt.y == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.y) {
                fmemmove(&sel.fl.lpplord->rgord[sel.iwpAct + 1], &sel.fl.lpplord->rgord[sel.iwpAct + 2], (sel.fl.cord - sel.iwpAct - 2) * sizeof(ORDER));
                sel.fl.cord = sel.fl.cord - 1;
                sel.fl.lpplord->iordMac = sel.fl.lpplord->iordMac - 0x1;
            }
        }
        if (fBackup == 0 && sel.iwpAct < sel.fl.cord - 1) {
            sel.iwpAct = sel.iwpAct + 1;
        }
        RedrawScanSel(0x0, 0);
        FLookupFleet(-1, &sel.fl);
        FFindNearestObject(sel.fl.lpplord->rgord[sel.iwpAct].pt, 0x8f, &scan);
        sel.iwpAct = -2;
        ChangeScanSel(&scan, 1);
        if ((grbitScan & 0x80) != 0x0) {
            for (ipt = 0; ipt < cpt; ipt++) {
                LogicalToScan(&rgpt[ipt]);
            }
            BoundPoints(&rc, rgpt, cpt);
            InvalidateRect(hwndScanner, &rc, 1);
        }
    } else {
        MessageBeep(0x40);
    }
    return;
}

void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

    if (fRecycle != 0) {
        if (iDel != 86 && lpfl->cord != 2 &&
            (lpfl->lpplord->rgord[lpfl->cord - 1].pt.x != lpfl->lpplord->rgord[iDel].pt.x ||
             lpfl->lpplord->rgord[lpfl->cord - 1].pt.y != lpfl->lpplord->rgord[iDel].pt.y)) {
            ord = lpfl->lpplord->rgord[iDel];
        } else {
            fRecycle = 0;
        }
    }
    fmemmove(&lpfl->lpplord->rgord[iDel], &lpfl->lpplord->rgord[iDel + 1], (lpfl->cord - iDel - 1) * sizeof(ORDER));
    if (fRecycle == 0) {
        lpfl->cord = lpfl->cord - 1;
        lpfl->lpplord->iordMac = lpfl->lpplord->iordMac - 0x1;
    } else {
        lpfl->lpplord->rgord[lpfl->cord - 1] = ord;
    }
    return;
}

int32_t EstFuelUse(FLEET *lpfl, int16_t iOrd, int16_t iWarp, int32_t dTravel, int16_t fRangeOnly) {
    int32_t  iEffNext;
    int32_t  lT;
    int16_t  fEfficient;
    double   d;
    int32_t  iEffCur;
    int32_t  wtCargoT;
    int32_t  lFuel;
    ORDER   *lpord;
    int16_t  i;
    SHDEF   *lpshdef;
    int32_t  wtCargo;
    int16_t  j;
    int32_t  wtMass;
    int32_t  rgieff[16];
    uint16_t t_scratch_m76;
    HULDEF  *t_call_a137;
    ENGINE  *t_call_a19f;
    int32_t  t_merge_a434_0001;
    int16_t  t_call_a40a;

    iEffCur = 0;
    gd.fRadiatingEngine = 0x0;
    if (iWarp == -1) {
        iWarp = lpfl->lpplord->rgord[iOrd + 1].iWarp;
    }
    fEfficient = GetRaceGrbit(&rgplr[lpfl->iPlayer], ibitRaceIFE);
    i = 0;
    lpshdef = rglpshdef[lpfl->iPlayer];
    while (i < 16) {
        if (lpfl->rgcsh[i] != 0) {
            for (j = 0; j < lpshdef->hul.chs && lpshdef->hul.rghs[j].grhst != hstEngine; j++) {
            }
            if (j != lpshdef->hul.chs) {
                t_scratch_m76 = lpshdef->hul.rghs[j].cItem;
                t_call_a137 = LphuldefFromId(lpshdef->hul.ihuldef);
                if (t_scratch_m76 >= t_call_a137->hul.rghs[j].cItem) {
                    t_call_a19f = LpengineFromId(lpshdef->hul.rghs[j].iItem);
                    rgieff[i] = (int32_t)t_call_a19f->rgcFuelUsed[iWarp];
                    if (fEfficient != 0) {
                        rgieff[i] = rgieff[i] - (int32_t)((int32_t)(rgieff[i] * 15) / 100);
                    }
                    if (lpshdef->hul.rghs[j].iItem != 0xa)
                        goto L_a07d;
                    gd.fRadiatingEngine = 0x1;
                    goto L_a07d;
                }
            }
            rgieff[i] = 99999;
        }
    L_a07d:
        i = i + 1;
        lpshdef = lpshdef + 1;
    }
    wtCargo = 0;
    for (i = 0; i <= 3; i++) {
        wtCargo = wtCargo + lpfl->rgwtMin[i];
    }
    if (dTravel == -1) {
        if (fRangeOnly == 0) {
            lpord = &lpfl->lpplord->rgord[iOrd];
            d = DGetDistance(lpord->pt.x, lpord->pt.y, lpord[1].pt.x, lpord[1].pt.y);
            dTravel = (int32_t)LOWORD((int32_t)(d + 0.9999));
        } else {
            dTravel = 1000;
        }
    }
    lFuel = 0;
    while (1) {
        iEffNext = 999999;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                if (rgieff[i] != iEffCur) {
                    if (rgieff[i] > iEffCur && rgieff[i] < iEffNext) {
                        iEffNext = rgieff[i];
                    }
                } else {
                    if (wtCargo < (int32_t)(uint32_t)((int32_t)lpfl->rgcsh[i] * (int32_t)WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, 2))) {
                        t_merge_a434_0001 = wtCargo;
                    } else {
                        t_call_a40a = WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, 2);
                        t_merge_a434_0001 = (uint32_t)((int32_t)lpfl->rgcsh[i] * (int32_t)t_call_a40a);
                    }
                    wtCargoT = t_merge_a434_0001;
                    wtCargo = wtCargo - wtCargoT;
                    if (rgieff[i] > 0) {
                        wtMass = (uint32_t)((int32_t)lpfl->rgcsh[i] * (uint32_t)rglpshdef[lpfl->iPlayer][i].hul.wtEmpty) + wtCargoT;
                        lT = (uint32_t)(iEffCur * dTravel);
                        if (wtMass < 200 || (lT < 500000 && wtMass < 4000) || (lT < 100000 && wtMass < 20000)) {
                            lFuel = lFuel + (int32_t)((int32_t)(wtMass * lT) / 2000);
                        } else {
                            lFuel = (int32_t)((double)lT * (double)wtMass / 2000.0) + lFuel;
                        }
                    }
                }
            }
        }
        if (iEffNext == 999999)
            break;
        iEffCur = iEffNext;
    }
    if (fRangeOnly == 0) {
        lFuel = lFuel + 9;
    }
    lFuel = (int32_t)(lFuel / 10);
    if (fRangeOnly != 0) {
        if (lFuel != 0) {
            if (lFuel <= 100000) {
                lFuel = (int32_t)((int32_t)(lpfl->rgwtMin[4] * 1000) / lFuel);
            } else {
                lFuel = (int32_t)(lpfl->rgwtMin[4] / (int32_t)(lFuel / 1000));
            }
        } else {
            lFuel = 1000000000;
        }
    }
    return lFuel;
}

LRESULT CALLBACK FakeEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg != WM_CHAR || ((wParam >= 0x30 && wParam <= 0x39) || wParam == 0x8)) {
        return CallWindowProc(lpfnRealEditProc, hwnd, msg, wParam, lParam);
    }
    return 0;
}

int16_t IFindIdealWarp(FLEET *lpfl, int16_t fIgnoreScoops) {
    int16_t i;
    int16_t j;
    int16_t iWorst;
    ENGINE *lpengine;

    iWorst = 10;
    if (lpfl == 0x0) {
        lpfl = &sel.fl;
    }
    i = 0;
    while (1) {
        if (i >= 16) {
            return iWorst;
        }
        if (lpfl->rgcsh[i] > 0) {
            for (j = 0; j < rglpshdef[lpfl->iPlayer][i].hul.chs && rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst != hstEngine; j++) {
            }
            if (j == rglpshdef[lpfl->iPlayer][i].hul.chs)
                break;
            j = rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem;
            lpengine = LpengineFromId(j);
            while (1) {
                if (iWorst <= 0)
                    goto L_a9da;
                if (lpengine->rgcFuelUsed[iWorst] <= 120)
                    break;
                iWorst = iWorst - 1;
            }
            if (lpengine->rgcFuelUsed[iWorst] > 0 && fIgnoreScoops == 0 && j != 14 && j != 15) {
                if (iWorst < 5 || lpengine->rgcFuelUsed[iWorst - 1] != 0) {
                    if (iWorst < 6 || lpengine->rgcFuelUsed[iWorst - 2] != 0) {
                        if (iWorst >= 7 && lpengine->rgcFuelUsed[iWorst - 3] == 0) {
                            iWorst = iWorst - 3;
                        }
                    } else {
                        iWorst = iWorst - 2;
                    }
                } else {
                    iWorst = iWorst - 1;
                }
            }
            if (iWorst == 10) {
                switch (j) {
                default:
                    iWorst = 9;
                case 7:
                case 8:
                case 9:
                case 14:
                case 15:
                }
            }
        }
    L_a9da:
        i = i + 1;
    }
    iWorst = 0;
    return iWorst;
}

int32_t LFuelUseToWaypoint(FLEET *lpfl, int16_t iwp, int16_t fMaxCargo) {
    int32_t lCur;
    int16_t iWarp;
    int16_t dist;
    PLANET *lppl;
    int16_t i;
    int32_t lTot;
    ORDER  *lpord;
    int16_t cYears;
    SHDEF  *lpshdef;
    int16_t j;
    double  dbl;
    int32_t l;
    int32_t lOneYearUse;
    int32_t lFuelGain;
    PLANET *t_call_adcb;

    lTot = 0;
    lCur = 0;
    lpord = lpfl->lpplord->rgord;
    for (i = 0; i < iwp; i++) {
        iWarp = lpord[i + 1].iWarp;
        if (iWarp <= 0 || iWarp >= 11) {
            cYears = 1;
            l = 0;
        } else {
            dbl = DGetDistance(lpord[i].pt.x, lpord[i].pt.y, lpord[i + 1].pt.x, lpord[i + 1].pt.y) + 0.99999;
            dist = LOWORD((int32_t)dbl);
            dbl = dbl / (double)(int32_t)iWarp / (double)(int32_t)iWarp;
            cYears = LOWORD((int32_t)(dbl + 0.9999));
            l = EstFuelUse(lpfl, i, iWarp, -1, 0);
        }
        if (cYears > 1) {
            lOneYearUse = EstFuelUse(lpfl, i, iWarp, (int32_t)(iWarp * iWarp), 0);
            lFuelGain = (uint32_t)(lOneYearUse * (int32_t)(cYears - 1));
            lFuelGain = lFuelGain + EstFuelUse(lpfl, i, iWarp, (int32_t)(dist - iWarp * iWarp * (cYears - 1)), 0);
            if (lFuelGain > l) {
                l = lFuelGain;
            }
            lFuelGain = LCalcFuelGainFromRamScoops(lpfl, iWarp, (int32_t)(iWarp * iWarp));
            for (j = 0; j < 16; j++) {
                if (lpfl->rgcsh[j] != 0) {
                    lpshdef = rglpshdef[idPlayer] + j;
                    if (lpshdef->hul.ihuldef == ihuldefFuelTransport || lpshdef->hul.ihuldef == ihuldefSuperFuelXport) {
                        lFuelGain = lFuelGain + (uint32_t)((int32_t)lpfl->rgcsh[j] * 200);
                    }
                }
            }
            if (lFuelGain > 0) {
                if (lOneYearUse <= lFuelGain) {
                    l = lOneYearUse;
                } else {
                    lOneYearUse = (uint32_t)((lOneYearUse - lFuelGain) * (int32_t)(cYears - 1)) + lOneYearUse;
                    if (lOneYearUse < l) {
                        l = lOneYearUse;
                    }
                }
            }
        }
        lCur = lCur + l;
        if (lCur > lTot) {
            lTot = lCur;
        }
        if (lpord[i + 1].grobj == grobjPlanet) {
            t_call_adcb = LpplFromId(lpord[i + 1].id);
            lppl = t_call_adcb;
            if (t_call_adcb != 0x0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0x0 &&
                LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0x0) {
                lCur = 0;
            }
        }
    }
    return lTot;
}

void FleetTransferCargoBalance(FLEET *pflNew1, FLEET *pflNew2) {
    int16_t  iplr;
    int32_t  rgCargoCapLoss[2];
    int32_t  wtCargoXfer;
    int16_t  fDeadFleet;
    int32_t  wtCargoTot;
    int16_t  rgrgcshLoss[2][16];
    int32_t  rgrgCargoDelta[2][5];
    int32_t  rgFuelCapacity[2];
    FLEET   *rgpflNew[2];
    int16_t  wtCargoMax;
    int16_t  wtFuelMax;
    int16_t  i;
    int32_t  lChg;
    int32_t  rgFuelCapLoss[2];
    FLEET    rgflCur[2];
    int16_t  j;
    SHDEF   *lpshdef;
    int32_t  rgCargoCapacity[2];
    int16_t  ishdef;
    int32_t  l;
    int32_t  cshDmgDst;
    int32_t  cshDmgSrc;
    int16_t  iSrc;
    int32_t  pctNew;
    int32_t  cshDmgMoved;
    uint16_t t_merge_b325_0001;
    uint16_t t_merge_b443_0001;
    uint16_t t_merge_b472_0001;
    uint16_t t_merge_b575_0001;
    uint16_t t_merge_b5de_0001;
    uint16_t t_merge_b699_0001;
    uint16_t t_merge_b8d6_0001;
    uint16_t t_merge_b93f_0001;
    uint16_t t_merge_b9f0_0001;
    uint16_t t_merge_bb86_0001;
    uint16_t t_merge_bc37_0001;
    uint16_t t_merge_bca3_0001;
    int32_t  t_merge_c082_0001;
    uint16_t t_merge_c236_0001;

    fDeadFleet = 0;
    rgpflNew[0] = pflNew1;
    rgpflNew[1] = pflNew2;
    iplr = pflNew1->iPlayer;
    for (i = 0; i < 2; i++) {
        if (rgpflNew[i]->fDead == 0x0) {
            FLookupFleet(rgpflNew[i]->id, &rgflCur[i]);
        } else {
            fDeadFleet = 1;
            memset(&rgflCur[i], 0, sizeof(FLEET));
            rgflCur[i].iPlayer = iplr;
        }
        rgCargoCapLoss[i] = 0;
        rgCargoCapacity[i] = 0;
        rgFuelCapLoss[i] = 0;
        rgFuelCapacity[i] = 0;
        for (j = 0; j < 5; j++) {
            rgrgCargoDelta[i][j] = 0;
        }
    }
    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (rgflCur[0].rgcsh[ishdef] != 0 || rgflCur[1].rgcsh[ishdef] != 0) {
            lpshdef = rglpshdef[iplr] + ishdef;
            wtFuelMax = WtMaxShdefStat(lpshdef, 1);
            wtCargoMax = WtMaxShdefStat(lpshdef, 2);
            for (i = 0; i < 2; i++) {
                rgrgcshLoss[i][ishdef] = rgflCur[i].rgcsh[ishdef] - rgpflNew[i]->rgcsh[ishdef];
                if (rgflCur[i].rgcsh[ishdef] != 0) {
                    rgFuelCapacity[i] = rgFuelCapacity[i] + (uint32_t)((int32_t)rgflCur[i].rgcsh[ishdef] * (int32_t)wtFuelMax);
                    rgCargoCapacity[i] = rgCargoCapacity[i] + (uint32_t)((int32_t)rgflCur[i].rgcsh[ishdef] * (int32_t)wtCargoMax);
                    if (rgrgcshLoss[i][ishdef] > 0) {
                        rgFuelCapLoss[i] = rgFuelCapLoss[i] + (uint32_t)((int32_t)rgrgcshLoss[i][ishdef] * (int32_t)wtFuelMax);
                        rgCargoCapLoss[i] = rgCargoCapLoss[i] + (uint32_t)((int32_t)rgrgcshLoss[i][ishdef] * (int32_t)wtCargoMax);
                    }
                }
            }
            if (fDeadFleet == 0 && rgrgcshLoss[0][ishdef] == -rgrgcshLoss[1][ishdef] && rgrgcshLoss[0][ishdef] != 0) {
                iSrc = rgrgcshLoss[0][ishdef] >= 0 ? 0 : 1;
                if (rgflCur[iSrc].rgcsh[ishdef] <= 0) {
                    cshDmgSrc = 0;
                } else {
                    cshDmgSrc = (int32_t)((int32_t)(rgflCur[iSrc].rgdv[ishdef].pctSh * (int32_t)rgflCur[iSrc].rgcsh[ishdef]) / 0x64);
                }
                if (rgflCur[iSrc == 0 ? 1 : 0].rgcsh[ishdef] <= 0) {
                    cshDmgDst = 0;
                } else {
                    t_merge_b325_0001 = iSrc == 0 ? 0x1 : 0x0;
                    cshDmgDst = (int32_t)((int32_t)(rgflCur[iSrc == 0 ? 1 : 0].rgdv[ishdef].pctSh * (int32_t)rgflCur[t_merge_b325_0001].rgcsh[ishdef]) / 0x64);
                }
                if (cshDmgSrc != 0 && cshDmgDst != 0) {
                    if (cshDmgSrc <= (int32_t)rgrgcshLoss[iSrc][ishdef]) {
                        cshDmgMoved = cshDmgSrc;
                    } else {
                        cshDmgMoved = (int32_t)rgrgcshLoss[iSrc][ishdef];
                    }
                    t_merge_b443_0001 = iSrc == 0 ? 0x1 : 0x0;
                    t_merge_b472_0001 = iSrc == 0 ? 0x1 : 0x0;
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgDst * rgflCur[iSrc == 0 ? 1 : 0].rgdv[ishdef].pctDp) +
                                                 (uint32_t)(cshDmgMoved * rgflCur[iSrc].rgdv[ishdef].pctDp) +
                                                 (int32_t)rgpflNew[t_merge_b472_0001]->rgcsh[ishdef] - 0x1) /
                                       (int32_t)rgpflNew[t_merge_b443_0001]->rgcsh[ishdef]);
                    t_merge_b575_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp = rgpflNew[t_merge_b575_0001]->rgdv[ishdef].pctSh | (LOWORD(pctNew) & 0x1ff) * 0x80;
                    t_merge_b5de_0001 = iSrc == 0 ? 0x1 : 0x0;
                    pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgDst + cshDmgMoved) * 0x64) + (int32_t)rgpflNew[iSrc == 0 ? 1 : 0]->rgcsh[ishdef] - 0x1) /
                                       (int32_t)rgpflNew[t_merge_b5de_0001]->rgcsh[ishdef]);
                    t_merge_b699_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp = (rgpflNew[t_merge_b699_0001]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                    if (cshDmgMoved != cshDmgSrc) {
                        pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgSrc - cshDmgMoved) * 0x64) + (int32_t)rgpflNew[iSrc]->rgcsh[ishdef] - 0x1) /
                                           (int32_t)rgpflNew[iSrc]->rgcsh[ishdef]);
                        rgpflNew[iSrc]->rgdv[ishdef].pctSh = LOWORD(pctNew);
                    } else {
                        rgpflNew[iSrc]->rgdv[ishdef].dp = 0x0;
                    }
                } else if (cshDmgSrc != 0) {
                    if (cshDmgSrc <= (int32_t)rgrgcshLoss[iSrc][ishdef]) {
                        cshDmgMoved = cshDmgSrc;
                    } else {
                        cshDmgMoved = (int32_t)rgrgcshLoss[iSrc][ishdef];
                    }
                    t_merge_b8d6_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp =
                        rgpflNew[t_merge_b8d6_0001]->rgdv[ishdef].pctSh | (rgpflNew[iSrc]->rgdv[ishdef].pctDp & 0x1ff) * 0x80;
                    t_merge_b93f_0001 = iSrc == 0 ? 0x1 : 0x0;
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgMoved * 100) + (int32_t)rgpflNew[iSrc == 0 ? 1 : 0]->rgcsh[ishdef] - 0x1) /
                                       (int32_t)rgpflNew[t_merge_b93f_0001]->rgcsh[ishdef]);
                    t_merge_b9f0_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp = (rgpflNew[t_merge_b9f0_0001]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                    if (cshDmgMoved != cshDmgSrc) {
                        pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgSrc - cshDmgMoved) * 0x64) + (int32_t)rgpflNew[iSrc]->rgcsh[ishdef] - 0x1) /
                                           (int32_t)rgpflNew[iSrc]->rgcsh[ishdef]);
                        rgpflNew[iSrc]->rgdv[ishdef].pctSh = LOWORD(pctNew);
                    } else {
                        rgpflNew[iSrc]->rgdv[ishdef].dp = 0x0;
                    }
                } else if (cshDmgDst != 0) {
                    t_merge_bb86_0001 = iSrc == 0 ? 0x1 : 0x0;
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgDst * 100) + (int32_t)rgpflNew[iSrc == 0 ? 1 : 0]->rgcsh[ishdef] - 0x1) /
                                       (int32_t)rgpflNew[t_merge_bb86_0001]->rgcsh[ishdef]);
                    t_merge_bc37_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp = (rgpflNew[t_merge_bc37_0001]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                } else {
                    t_merge_bca3_0001 = iSrc == 0 ? 0x1 : 0x0;
                    rgpflNew[iSrc == 0 ? 1 : 0]->rgdv[ishdef].dp = rgpflNew[t_merge_bca3_0001]->rgdv[ishdef].dp & 0xff80;
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (rgFuelCapacity[i] != 0) {
            if (rgpflNew[i]->rgwtMin[4] <= 45000 && rgFuelCapLoss[i] <= 45000) {
                lChg = (int32_t)((int32_t)(rgpflNew[i]->rgwtMin[4] * rgFuelCapLoss[i]) / rgFuelCapacity[i]);
            } else {
                lChg = (int32_t)((double)rgpflNew[i]->rgwtMin[4] * (double)rgFuelCapLoss[i] / (double)rgFuelCapacity[i]);
            }
            rgrgCargoDelta[i][4] = rgrgCargoDelta[i][4] - lChg;
        }
        if (rgCargoCapacity[i] != 0) {
            wtCargoTot = 0;
            for (j = 0; j <= 3; j++) {
                wtCargoTot = wtCargoTot + rgpflNew[i]->rgwtMin[j];
            }
            if (wtCargoTot <= 45000 && rgCargoCapLoss[i] <= 45000) {
                wtCargoXfer = (int32_t)((int32_t)(wtCargoTot * rgCargoCapLoss[i]) / rgCargoCapacity[i]);
            } else {
                wtCargoXfer = (int32_t)((double)wtCargoTot * (double)rgCargoCapLoss[i] / (double)rgCargoCapacity[i]);
            }
            lChg = wtCargoXfer;
            if (wtCargoXfer != 0 && wtCargoTot != 0) {
                for (j = 0; j <= 3; j++) {
                    if (rgpflNew[i]->rgwtMin[j] <= 45000 && wtCargoXfer <= 45000) {
                        l = (int32_t)((int32_t)(rgpflNew[i]->rgwtMin[j] * wtCargoXfer) / wtCargoTot);
                    } else {
                        l = (int32_t)((double)rgpflNew[i]->rgwtMin[j] * (double)wtCargoXfer / (double)wtCargoTot);
                    }
                    t_merge_c082_0001 = l < lChg ? l : lChg;
                    l = t_merge_c082_0001;
                    rgrgCargoDelta[i][j] = rgrgCargoDelta[i][j] - l;
                    lChg = lChg - l;
                }
                if (lChg > 0) {
                    for (j = 0; j <= 3 && lChg > 0; j++) {
                        if (rgpflNew[i]->rgwtMin[j] + rgrgCargoDelta[i][j] > 0x0) {
                            rgrgCargoDelta[i][j] = rgrgCargoDelta[i][j] - 1;
                            lChg = lChg - 1;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            rgpflNew[i]->rgwtMin[j] = rgpflNew[i]->rgwtMin[j] + rgrgCargoDelta[i][j];
            t_merge_c236_0001 = i == 0 ? 0x1 : 0x0;
            rgpflNew[t_merge_c236_0001]->rgwtMin[j] = rgpflNew[t_merge_c236_0001]->rgwtMin[j] - rgrgCargoDelta[i][j];
        }
    }
    return;
}

void DestroyAllIshdefSB(int16_t ishdefSB, int16_t iplr) {
    PLANET *lppl;
    PLANET *lpplMac;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iplr && lppl->fStarbase != 0x0 && lppl->isb == ishdefSB) {
            lppl->fStarbase = 0x0;
            KillQueuedShips(lppl);
            KillQueuedMassPackets(lppl);
        }
    }
    return;
}

void DestroyAllIshdef(int16_t ishdef, int16_t iplr) {
    FLEET   flDead;
    int16_t cKill;
    FLEET  *lpfl;
    int16_t i;
    int16_t grbit;
    int16_t j;
    int16_t cDel;
    FLEET   flNew;

    cDel = 0;
    if (ishdef < 16) {
        lpfl = *rglpfl;
        i = 0;
        while (i < cFleet) {
            if (lpfl->iPlayer == iplr && lpfl->rgcsh[ishdef] > 0) {
                memset(&flDead, 0, sizeof(FLEET));
                cKill = lpfl->rgcsh[ishdef];
                cDel = cDel + cKill;
                for (j = 0; j < 16 && (j == ishdef || lpfl->rgcsh[j] == 0); j++) {
                }
                if (j == 16) {
                    lpfl->rgcsh[ishdef] = 0;
                    FDeleteFleet(lpfl->id, grobjNone, -1);
                    goto L_c392;
                }
                flDead.iplr = iplr;
                flDead.fDead = 0x1;
                flDead.rgcsh[ishdef] = cKill;
                flNew = *lpfl;
                flNew.rgcsh[ishdef] = 0;
                FleetTransferCargoBalance(&flNew, &flDead);
                *lpfl = flNew;
                if (sel.grobj == grobjFleet && sel.fl.id == flNew.id) {
                    FLookupFleet(flNew.id, &sel.fl);
                    RedrawScanSel(0x0, 0);
                    FillShipDD(sel.fl.id);
                    grbit = -31819;
                    FLookupFleet(sel.fl.id, &sel.fl);
                    FillFleetCompLB();
                    DrawPlanShip(0x0, grbit);
                    InvalidateRect(hwndMine, 0x0, 1);
                }
            }
            i = i + 1;
        L_c392:
            lpfl = rglpfl[i];
        }
        InvalidateReport(1, 1);
    } else {
        DestroyAllIshdefSB(ishdef - 16, iplr);
        InvalidateReport(0, 1);
    }
    RemoveIshdefFromAllQueues(ishdef, 0);
    return;
}

void RemoveIshdefFromAllQueues(int16_t ishdef, int16_t fSpaceDocks) {
    int16_t iprod;
    PLANET *lppl;
    int16_t iDst;
    PLANET *lpplMac;
    PROD   *lpprod;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->lpplprod != 0x0 && lppl->lpplprod->iprodMac != 0x0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0x0 &&
            (fSpaceDocks == 0 || rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef == ihuldefSpaceDock)) {
            iDst = 0;
            iprod = 0;
            lpprod = lppl->lpplprod->rgprod;
            while (iprod < lppl->lpplprod->iprodMac) {
                if (lpprod->grobj != grobjFleet || lpprod->iItem != (uint32_t)ishdef) {
                    if (iDst != iprod) {
                        lppl->lpplprod->rgprod[iDst] = *lpprod;
                    }
                    iDst = iDst + 1;
                }
                iprod = iprod + 1;
                lpprod = lpprod + 1;
            }
            if (iDst != 0) {
                if (iDst != iprod) {
                    lppl->lpplprod->iprodMac = LOBYTE(iDst);
                }
            } else {
                FreePl((PL *)lppl->lpplprod);
                lppl->lpplprod = 0x0;
            }
        }
    }
    if (sel.grobj == grobjPlanet && sel.pl.lpplprod != 0x0) {
        FLookupPlanet(sel.pl.id, &sel.pl);
        FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);
    }
    return;
}

int16_t CshQueued(int16_t ishdef, int16_t *pfProgress, int16_t fSpaceDocks) {
    int16_t iprod;
    PLANET *lppl;
    int16_t csh;
    PLANET *lpplMac;
    PROD   *lpprod;

    csh = 0;
    *pfProgress = 0;
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->lpplprod != 0x0 && lppl->lpplprod->iprodMac != 0x0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0x0 &&
            (fSpaceDocks == 0 || rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef == ihuldefSpaceDock)) {
            iprod = 0;
            lpprod = lppl->lpplprod->rgprod;
            while (iprod < lppl->lpplprod->iprodMac) {
                if (lpprod->grobj == grobjFleet && lpprod->iItem == (uint32_t)ishdef) {
                    csh = csh + lpprod->cItem;
                    if (lpprod->pct != 0x0) {
                        *pfProgress = 1;
                    }
                }
                iprod = iprod + 1;
                lpprod = lpprod + 1;
            }
        }
    }
    return csh;
}

void Merge2Fleets(FLEET *lpflDst, FLEET *lpflDel, int16_t fNoDelete) {
    FLEET   rgfl[2];
    int16_t i;

    rgfl[0] = *lpflDst;
    rgfl[1] = *lpflDel;
    for (i = 0; i < 16; i++) {
        rgfl[0].rgcsh[i] = rgfl[0].rgcsh[i] + rgfl[1].rgcsh[i];
        rgfl[1].rgcsh[i] = 0;
    }
    FleetTransferCargoBalance(rgfl, &rgfl[1]);
    for (i = 0; i < 2; i++) {
        FLookupFleet(-1, &rgfl[i]);
    }
    if (fNoDelete == 0) {
        FDeleteFleet(rgfl[1].id, grobjFleet, rgfl[0].id);
        InvalidateReport(1, 2);
    } else {
        lpflDel->fDead = 0x1;
    }
    return;
}

void FleetOrdersChangeTarget(FLEET *lpflOld) {
    int16_t    id;
    POINT16    pt;
    int16_t    fChg;
    FLEET     *lpfl;
    int16_t    iord;
    int16_t    iflMac;
    SCAN       scan;
    GrobjClass grobj;

    fChg = 0;
    for (iflMac = 0; iflMac < cFleet; iflMac++) {
        lpfl = rglpfl[iflMac];
        if (rglpfl[iflMac] == 0x0)
            break;
        if (lpfl->lpplord != 0x0) {
            for (iord = lpfl->cord - 1; iord >= 0; iord--) {
                if (lpfl->lpplord->rgord[iord].grobj == grobjFleet && lpfl->lpplord->rgord[iord].id == lpflOld->id) {
                    if (fChg == 0) {
                        pt = lpflOld->pt;
                        lpflOld->pt.x = lpflOld->pt.x + 1;
                        if (FFindNearestObject(pt, 0x83, &scan) == 0) {
                            grobj = grobjOther;
                            id = iord;
                        } else if ((scan.grobjFull & 0x2) == 0x0) {
                            grobj = grobjPlanet;
                            id = scan.idpl;
                        } else {
                            grobj = grobjFleet;
                            id = rglpfl[scan.ifl]->id;
                        }
                        lpflOld->pt.x = lpflOld->pt.x - 1;
                    }
                    lpfl->lpplord->rgord[iord].id = id;
                    lpfl->lpplord->rgord[iord].grobj = grobj;
                }
            }
        }
    }
    return;
}

void GetTruePartCost(int16_t iPlayer, PART *ppart, uint16_t *rgCost) {
    int16_t  cExcess;
    int16_t  cCur;
    int16_t  i;
    COMPART *lpcom;

    lpcom = ppart->pcom;
    for (i = 0; i < 3; i++) {
        rgCost[i] = lpcom->rgwtOreCost[i];
    }
    rgCost[3] = lpcom->resCost;
    if (iPlayer != -1) {
        if ((ppart->hs.grhst & 0x2000) == 0x0 && ((ppart->hs.grhst & 0x8000) == 0x0 || ppart->hs.iItem < 0x9 || ppart->hs.iItem > 0xd) &&
            ((ppart->hs.grhst & 0x8000) == 0x0 || ppart->hs.iItem < 0x0 || ppart->hs.iItem > 0x8)) {
            cExcess = 100;
            for (i = 0; i < 6; i++) {
                cCur = (int16_t)rgplr[iPlayer].rgTech[i] - (int16_t)lpcom->rgTech[i];
                if ((int16_t)lpcom->rgTech[i] > 0 && cCur < cExcess) {
                    cExcess = cCur;
                }
            }
            if (cExcess == 100) {
                for (i = 0; i < 6; i++) {
                    if ((int16_t)rgplr[iPlayer].rgTech[i] < cExcess) {
                        cExcess = (int16_t)rgplr[iPlayer].rgTech[i];
                    }
                }
            }
            if (cExcess >= 1) {
                if (cExcess > 19) {
                    cExcess = 19;
                }
                if (GetRaceGrbit(&rgplr[iPlayer], ibitRaceBleedingEdgeTech) == 0) {
                    cExcess = cExcess * 4;
                    if (cExcess > 75) {
                        cExcess = 75;
                    }
                } else {
                    cExcess = 5 * cExcess;
                    if (cExcess > 80) {
                        cExcess = 80;
                    }
                }
                for (i = 0; i < 4; i++) {
                    if (rgCost[i] > 0x0) {
                        rgCost[i] = rgCost[i] - MulDiv(rgCost[i], cExcess, 100);
                        if (rgCost[i] == 0x0) {
                            rgCost[i] = 0x1;
                        }
                    }
                }
            }
        }
        if (ppart->hs.grhst != hstSpecialSB || GetRaceStat(&rgplr[iPlayer], rsMajorAdv) != raStargate ||
            GetRaceStat(&rgplr[iPlayer], rsMajorAdv) != raStargate || ppart->hs.iItem < ispecialSBStargate100250 ||
            ppart->hs.iItem > ispecialSBStargateAnyAny) {
            switch (ppart->hs.grhst) {
            case hstBeam:
            case hstTorp:
            case hstBomb:
                if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raAttack) {
                    for (i = 0; i < 4; i++) {
                        rgCost[i] = rgCost[i] - (rgCost[i] >> 0x2);
                    }
                    break;
                }
            default:
                switch (ppart->hs.grhst) {
                case hstBeam:
                case hstTorp:
                case hstBomb:
                    if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raDefend) {
                        for (i = 0; i < 4; i++) {
                            rgCost[i] = rgCost[i] + (rgCost[i] >> 0x2);
                        }
                        break;
                    }
                default:
                    if (ppart->hs.grhst != hstTerra || GetRaceStat(&rgplr[iPlayer], rsMajorAdv) != raTerra) {
                        if (ppart->hs.grhst == hstEngine && GetRaceGrbit(&rgplr[iPlayer], ibitRaceCheapEngines) != 0) {
                            for (i = 0; i < 4; i++) {
                                rgCost[i] = rgCost[i] - (rgCost[i] >> 0x1);
                            }
                        }
                    } else {
                        rgCost[3] = (uint32_t)rgCost[3] / 0x2;
                    }
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                rgCost[i] = rgCost[i] - (rgCost[i] >> 0x2);
            }
        }
        if (cExcess >= 1 || GetRaceGrbit(&rgplr[iPlayer], ibitRaceBleedingEdgeTech) == 0 || gd.fDontCalcBleed != 0x0) {
            gd.fBleedingEdge = 0x0;
        } else {
            for (i = 0; i < 6 && (int16_t)lpcom->rgTech[i] <= 0; i++) {
            }
            if (i >= 6) {
                gd.fBleedingEdge = 0x0;
            } else {
                gd.fBleedingEdge = 0x1;
                for (i = 0; i < 4; i++) {
                    rgCost[i] = rgCost[i] * 0x2;
                }
            }
        }
    }
    return;
}
