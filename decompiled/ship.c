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
    if (ptile->fFixCtls != 0) {
        rgrcRef[0].top = -5;
        rgrcRef[0].bottom = -6;
        rgrcRef[12].top = -5;
        rgrcRef[12].bottom = -6;
        ShowWindow(hwndShipLB, SW_HIDE);
        ShowWindow(hwndRepCB, SW_HIDE);
        ptile->fFixCtls = FALSE;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFleetWaypoints)) == 0) {
        ShowWindow(hwndShipLB, SW_HIDE);
        ShowWindow(hwndRepCB, SW_HIDE);
    } else {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0 ? 4 : 2) + rc.top;
        rgrcRef[12].top = -5;
        rgrcRef[12].bottom = -6;
        GetClientRect(hwndShipLB, &rcT);
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        dyShipLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0 ? 4 : 3);
        dWrong = dyShipLB - (rcT.bottom - rcT.top);
        if (dxShipLB == xRight - xLeft && dWrong >= 0 && dWrong < dyArial8) {
            swp |= SWP_NOSIZE;
        } else {
            dxShipLB = xRight - xLeft;
        }
        SetWindowPos(hwndShipLB, NULL, xLeft, yTop, xRight - xLeft, dyShipLB, swp);
        ShowWindow(hwndShipLB, SW_SHOW);
        GetClientRect(hwndShipLB, &rcT);
        dyShipLB = rcT.bottom - rcT.top;
        yTop += (gd.fSmallTileMode == 0 ? 4 : 2) + dyShipLB;
        SelectObject(hdc, rghfontArial8[1]);
        if (ptile->fMinDraw == 0) {
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
        if (ptile->fMinDraw == 0) {
            c = CchGetString(sel.iwpAct <= 0 ? idsWayPt : idsComing, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        if (sel.iwpAct > 0) {
            ord = sel.fl.lpplord->rgord[sel.iwpAct - 1];
            psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
            iScanActual = sel.iwpAct;
        } else if (sel.fl.cord <= 1) {
            psz = "";
        } else {
            ord = sel.fl.lpplord->rgord[1];
            psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
            ord = sel.fl.lpplord->rgord[0];
            iScanActual = 1;
        }
        RightTextOut(hdc, xRight, yTop, psz, 0, dxRight);
        yTop += dyArial8;
        if (sel.fl.cord <= 1 || sel.iwpAct == 0) {
            rgrcRef[0].top = -5;
            rgrcRef[0].bottom = -6;
            if (sel.fl.cord <= 1) {
                SetRect(&rc, xLeft - 1, yTop, xRight - 1, dyArial8 * 4 + yTop);
                FillRect(hdc, &rc, hbrButtonFace);
                yTop += (dyArial8 - gd.fSmallTileMode) * 4 + gd.fSmallTileMode * 2;
                goto DoCheckBox;
            }
        }
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDistance, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        pt = sel.fl.lpplord->rgord[iScanActual].pt;
        RightTextOut(hdc, xRight, yTop, PszGetDistance(ord.pt.x, ord.pt.y, pt.x, pt.y), 0, dxRight);
        yTop += dyArial8 - gd.fSmallTileMode;
        SelectObject(hdc, rghfontArial8[1]);
        if (ptile->fMinDraw == 0) {
            c = CchGetString(idsWarpFactor, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        iWarp = sel.fl.lpplord->rgord[iScanActual].iWarp;
        if (sel.iwpAct != 0) {
            SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight - 1, yTop + dyArial8);
            rgrcRef[0] = rcGauge;
            DrawFleetGauge(hdc, &rcGauge, NULL, 6);
        } else {
            SelectObject(hdc, rghfontArial8[0]);
            if (iWarp < 11) {
                c = _wsprintf(szWork, PszGetCompressedString(idsWarpD2), iWarp);
            } else {
                c = CchGetString(idsUseStargate, szWork);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        }
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsTravelTime, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        c = CchGetETA(hdc, pfl, szWork, iScanActual, FALSE);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        SetTextColor(hdc, 0);
        yTop += dyArial8 - gd.fSmallTileMode;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsEstFuelUsage, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        lTot = LFuelUseToWaypoint(&sel.fl, iScanActual, FALSE);
        c = _wsprintf(szWork, PszGetCompressedString(idsLdmg), lTot);
        if (lTot > sel.fl.rgwtMin[4]) {
            SetTextColor(hdc, 0xff);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight - 20);
        if (lTot > sel.fl.rgwtMin[4]) {
            SetTextColor(hdc, 0);
        }
        yTop += dyArial8;
    DoCheckBox:
        SendMessage(hwndRepCB, BM_SETCHECK, sel.fl.fRepOrders, 0);
        SetWindowPos(hwndRepCB, NULL, xLeft, yTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(hwndRepCB, SW_SHOW);
        SetRect(&rgrcRef[12], xRight - (dyArial8 | 1), yTop, xRight, (dyArial8 | 1) + yTop);
        DrawDiamond(hdc, &rgrcRef[12], hbrBBlue);
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
    TaskType grtask;
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

    if (ptile->fFixCtls != 0) {
        for (i = 0; i < 3; i++) {
            ShowWindow(rghwndOrderDD[i], SW_HIDE);
        }
        ShowWindow(hwndOrderED, SW_HIDE);
        ptile->fFixCtls = FALSE;
        rgrcRef[5].top = -5;
        rgrcRef[5].bottom = -6;
        rgrcRef[18].top = -5;
        rgrcRef[18].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsWaypointTask)) == 0) {
        for (i = 0; i < 3; i++) {
            ShowWindow(rghwndOrderDD[i], SW_HIDE);
        }
        ShowWindow(hwndOrderED, SW_HIDE);
    } else {
        pfl = obj.pfl;
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0 ? 4 : 2) + rc.top;
        yBot = rc.bottom - 4;
        dxRight = xRight - xLeft;
        rgrcRef[5].top = -5;
        rgrcRef[5].bottom = -6;
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        if (rgdxOrderDD[0] == dxRight) {
            swp |= SWP_NOSIZE;
        } else {
            rgdxOrderDD[0] = dxRight;
        }
        SetWindowPos(rghwndOrderDD[0], NULL, xLeft, yTop, dxRight, 10 * dyArial8 + dyShipDD, swp);
        ShowWindow(rghwndOrderDD[0], SW_SHOW);
        yTop += dyShipDD + 3;
        yTopMsg = yTop;
        l = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0, 0);
        grtask = LOWORD(l);
        if (IsWindowVisible(rghwndOrderDD[1]) == 0) {
            SetRect(&rc, xLeft - 1, yTop, xRight + 1, yBot + 2);
            FillRect(hdc, &rc, hbrButtonFace);
        } else if (IsWindowVisible(rghwndOrderDD[2]) == 0) {
            SetRect(&rc, xLeft - 1, yTop + dyShipDD + 3, xRight + 1, yBot + 2);
            FillRect(hdc, &rc, hbrButtonFace);
        }
        switch (grtask) {
        case grTaskXfer:
        case grTaskLayMines:
        case grTaskPatrol:
        case grTaskGive:
            swp = SWP_NOZORDER | SWP_NOACTIVATE;
            switch (grtask) {
            case grTaskXfer:
                dxRight2 = dxRight - dyShipDD + 2;
                break;
            case grTaskPatrol:
                SelectObject(hdc, rghfontArial8[1]);
                psz = PszGetCompressedString(idsWarpFactor);
                cch = strlen(psz);
                dxT = LOWORD(GetTextExtent(hdc, psz, cch)) + 2;
                psz = PszGetCompressedString(idsIntercept);
                cch = strlen(psz);
                SetRect(&rc, xLeft, yTop + 4, xLeft + dxT, yBot);
                FillRect(hdc, &rc, hbrButtonFace);
                DrawText(hdc, psz, cch, &rc, DT_NOPREFIX);
                dxRight2 = dxRight - dxT - 2;
                xLeft += dxT + 2;
                break;
            case grTaskGive:
                SelectObject(hdc, rghfontArial8[1]);
                cch = CchGetString(idsTo3, szT);
                szT[cch] = ' ';
                cch++;
                szT[cch] = 0;
                dxT = LOWORD(GetTextExtent(hdc, psz, cch)) + 2;
                SetRect(&rc, xLeft, yTop + 4, xLeft + dxT, yBot);
                FillRect(hdc, &rc, hbrButtonFace);
                DrawText(hdc, szT, cch, &rc, DT_NOPREFIX);
                dxRight2 = dxRight - dxT - 2;
                xLeft += dxT + 2;
                break;
            default:
                dxRight2 = dxRight;
            }
            if (rgdxOrderDD[1] == dxRight2) {
                swp |= SWP_NOSIZE;
            } else {
                rgdxOrderDD[1] = dxRight2;
            }
            SetWindowPos(rghwndOrderDD[1], NULL, xLeft, yTop, dxRight2, 6 * dyShipDD, swp);
            ShowWindow(rghwndOrderDD[1], SW_SHOW);
            if (grtask == grTaskXfer) {
                SetRect(&rcT, xLeft + dxRight - (dyShipDD | 1) + 8, yTop + 3, xLeft + dxRight, (dyShipDD | 1) + yTop - 5);
                rgrcRef[5] = rcT;
                DrawDiamond(hdc, &rcT, hbrBBlue);
                break;
            }
            if (grtask != grTaskPatrol)
                break;
            yTop += dyShipDD + 4;
            SetRect(&rcT, xLeft - dxT - 2, yTop, xRight - 1, yTop + dyArial8);
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetCompressedString(idsWarpFactor);
            cch = strlen(psz);
            DrawText(hdc, psz, cch, &rcT, DT_NOPREFIX);
            rcT.left += dxT + 2;
            rgrcRef[18] = rcT;
            DrawFleetGauge(hdc, &rcT, NULL, 7);
            break;
        default:
            if (IsWindowVisible(rghwndOrderDD[1]) != 0) {
                ShowWindow(rghwndOrderDD[1], SW_HIDE);
                SetRect(&rc, xLeft - 1, yTop, xRight + 1, yBot + 2);
                FillRect(hdc, &rc, hbrButtonFace);
            }
        }
        yTop += dyShipDD + 3;
        lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
        if (lpord->grobj == grobjPlanet) {
            lppl = LpplFromId(lpord->id);
        } else {
            lppl = NULL;
        }
        if (grtask == grTaskXfer) {
            dxKt = 0;
            for (i = 0; i < 5; i++) {
                if ((int16_t)LOWORD(GetTextExtent(hdc, vrgszUnits[i], 2)) > dxKt) {
                    dxKt = LOWORD(GetTextExtent(hdc, vrgszUnits[i], 2));
                }
            }
            i = LOWORD(SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0));
            if (i == 0) {
                i = 4;
            } else {
                i--;
            }
            SelectObject(hdc, rghfontArial8[1]);
            edWid = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN99999Kt), 9));
            dxRight -= edWid + 8;
            swp = SWP_NOZORDER | SWP_NOACTIVATE;
            if (rgdxOrderDD[2] == dxRight) {
                swp |= SWP_NOSIZE;
            } else {
                rgdxOrderDD[2] = dxRight;
            }
            SetWindowPos(rghwndOrderDD[2], NULL, xLeft, yTop, dxRight, 9 * dyShipDD, swp);
            ShowWindow(rghwndOrderDD[2], SW_SHOW);
            l = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0, 0);
            fActive = TRUE;
            switch (l) {
            case 0:
            case 1:
            case 2:
            case 7:
                fActive = FALSE;
                break;
            case 5:
            case 6:
                i = 5;
            }
            psz = vrgszUnits[i];
            swp = SWP_NOZORDER | SWP_NOACTIVATE;
            if (dxOrderED == edWid - dxKt) {
                swp |= SWP_NOSIZE;
            } else {
                dxOrderED = edWid - dxKt;
            }
            SetWindowPos(hwndOrderED, NULL, xRight - edWid - 4, yTop, edWid - dxKt, dyShipDD, swp);
            EnableWindow(hwndOrderED, fActive);
            ShowWindow(hwndOrderED, SW_SHOW);
            SetRect(&rc, xRight - dxKt - 1, yTop + 2, xRight - 1, yBot);
            FillRect(hdc, &rc, hbrButtonFace);
            if (fActive != 0) {
                DrawText(hdc, psz, 2, &rc, DT_NOPREFIX);
            }
        } else {
            ShowWindow(rghwndOrderDD[2], SW_HIDE);
            ShowWindow(hwndOrderED, SW_HIDE);
        }
        switch (grtask) {
        case grTaskScrap:
            ids = idsNoteShipsFleetWillDismantledMineralsCan;
            goto LDisplayMsg;
        case grTaskMerge:
            if (sel.fl.lpplord->rgord[sel.iwpAct].grobj == grobjFleet)
                break;
            ids = idsWarningDestinationWaypointFleetMergeWillSucessfu;
            psz = PszGetCompressedString(ids);
            SetTextColor(hdc, 127);
            SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
            SelectObject(hdc, rghfontArial7[0]);
            DrawText(hdc, psz, strlen(psz), &rc, DT_WORDBREAK | DT_NOPREFIX);
            SetTextColor(hdc, crButtonText);
            break;
        case grTaskLayMines:
            yTopMsg = yTop;
            l = CLayMinesFromLpfl(&sel.fl, 0xffff, -1);
            if (l <= 0) {
                ids = idsWarningFleetHasMineLayingPods;
                goto LDisplayMsg;
            }
            pszT = PszGetCompressedString(idsFleetCanLayLdMinesPerYear);
            _wsprintf(szWork, pszT, l);
            psz = szWork;
            goto LDisplayMsg2;
        case grTaskColonize:
            fActive = FALSE;
            for (i = 0; i < 16; i++) {
                if (sel.fl.rgcsh[i] > 0) {
                    for (j = 0; j < rglpshdef[sel.fl.iPlayer][i].hul.chs; j++) {
                        if (rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].grhst == hstSpecialM &&
                            (rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].iItem == ispecialMColonizationModule ||
                             rglpshdef[sel.fl.iPlayer][i].hul.rghs[j].iItem == ispecialMOrbitalConstructionModule))
                            goto L_1457;
                    }
                }
            }
            goto FoundColony;
        L_1457:
            fActive = TRUE;
        FoundColony:
            if (fActive != 0) {
                if (sel.fl.rgwtMin[3] == 0) {
                    ids = idsRememberLoadColonistsBeforeEmbarkingMission;
                    goto LDisplayMsg;
                }
                ids = idsNoteShipsFleetWillDismantledProvideSupplies;
                goto LDisplayMsg;
            }
            ids = idsWarningColonizeMissionCannotCarriedBecauseNone;
            goto LDisplayMsg;
        case grTaskMine:
            cMine = CMineFromLpfl(&sel.fl);
            if (cMine > 0) {
                if (lppl != 0 && (lppl->iPlayer == -1 || (lppl->iPlayer == sel.fl.iPlayer && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh))) {
                    if (dyArial8 > 14) {
                        SelectObject(hdc, rghfontArial7[0]);
                        dyCur = dyArial7;
                    } else {
                        SelectObject(hdc, rghfontArial8[0]);
                        dyCur = dyArial8;
                    }
                    if (lppl->det <= detMinimal) {
                        ids = 243;
                        goto ShowString;
                    }
                    EstMineralsMined(lppl, rgl, cMine, FALSE);
                    c = CchGetString(idsMiningRatePerYear, szWork);
                    TextOut(hdc, xLeft, yTopMsg, szWork, c);
                    yTopMsg += dyCur;
                    dxRight = xLeft;
                    for (i = 0; i < 3; i++) {
                        SetTextColor(hdc, rgcrMinerals[i]);
                        c = _wsprintf(szWork, PCTLD, rgl[i]);
                        DxStreamTextOut(hdc, &dxRight, yTopMsg, szWork, c, TRUE);
                        SetTextColor(hdc, crButtonText);
                        DxStreamTextOut(hdc, &dxRight, yTopMsg, "kT  ", 4, TRUE);
                    }
                    goto DoneMine;
                }
                ids = 229;
            } else {
                ids = 226;
            }
            if (dyArial8 > 14) {
                SelectObject(hdc, rghfontArial6[0]);
            } else {
                SelectObject(hdc, rghfontArial7[0]);
            }
        ShowString:
            SetTextColor(hdc, ids == 229 ? crButtonText : 127);
            psz = PszGetCompressedString(ids);
            SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
            DrawText(hdc, psz, strlen(psz), &rc, DT_WORDBREAK | DT_NOPREFIX);
        DoneMine:
            SetTextColor(hdc, crButtonText);
        }
        return;
    LDisplayMsg:
        psz = PszGetCompressedString(ids);
    LDisplayMsg2:
        SetTextColor(hdc, ids == idsNoteShipsFleetWillDismantledProvideSupplies ? crButtonText : 127);
        SetRect(&rc, xLeft, yTopMsg, xRight, yBot + 2);
        SelectObject(hdc, rghfontArial7[0]);
        DrawText(hdc, psz, strlen(psz), &rc, DT_WORDBREAK | DT_NOPREFIX);
        SetTextColor(hdc, crButtonText);
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

    if (obj.pfl->idPlanet != -1) {
        psz = PszGetPlanetName(obj.pfl->idPlanet | 0x8000);
    } else {
        psz = PszGetCompressedString(idsDeepSpace2);
    }
    if (ptile->fFixCtls != 0) {
        ShowWindow(rghwndBtn[3], SW_HIDE);
        ShowWindow(rghwndBtn[7], SW_HIDE);
        ptile->fFixCtls = FALSE;
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) == 0) {
        ShowWindow(rghwndBtn[3], SW_HIDE);
        ShowWindow(rghwndBtn[7], SW_HIDE);
    } else {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = (gd.fSmallTileMode == 0 ? 4 : 1) + rc.top;
        dx = (int16_t)(xRight - xLeft - 16) / 3;
        dy = 3 * dyArial8 >> 1;
        EnableWindow(rghwndBtn[3], obj.pfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer);
        SetWindowText(rghwndBtn[7], PszGetCompressedString(obj.pfl->idPlanet == -1 ? idsJettison2 : idsXFer));
        if (obj.pfl->idPlanet == -1) {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac && (lpth->ith != ithMineralPacket || obj.pfl->pt.x != lpth->pt.x || obj.pfl->pt.y != lpth->pt.y); lpth++) {
            }
            EnableWindow(rghwndBtn[7], lpth == lpthMac);
        } else {
            EnableWindow(rghwndBtn[7], TRUE);
        }
        if (ptile->fMinDraw == 0) {
            i = 3;
            while (i <= 7) {
                SetWindowPos(rghwndBtn[i], NULL, xLeft, yTop, dx, dy, SWP_NOZORDER | SWP_NOACTIVATE);
                ShowWindow(rghwndBtn[i], SW_SHOW);
                i += 4;
                xLeft += dx * 2 + 16;
            }
        }
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
    if (ptile->fFixCtls != 0) {
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
        if (ptile->fMinDraw == 0) {
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[2] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 4);
        yTop += (gd.fSmallTileMode == 0 ? 4 : 2) + dyArial8;
        if (ptile->fMinDraw == 0) {
            c = CchGetString(idsCargo3, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
        rgrcRef[3] = rcGauge;
        DrawFleetGauge(hdc, &rcGauge, pfl, 5);
        yTop += dyArial8 + 4;
        if (gd.fSmallTileMode == 0) {
            for (i = 0; i <= 2; i++) {
                if (ptile->fMinDraw == 0) {
                    SelectObject(hdc, rghfontArial8[1]);
                    SetTextColor(hdc, rgcrMinerals[i]);
                    TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));
                }
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
                c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[i]);
                RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
                yTop += dyArial8;
            }
            if (ptile->fMinDraw == 0) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, 0xffffff);
                c = CchGetString(idsColonists2, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crButtonText);
            }
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pfl->rgwtMin[3]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
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
    if (ptile->fFixCtls != 0) {
        ShowWindow(hwndFleetCompLB, SW_HIDE);
        ShowWindow(rghwndBtn[8], SW_HIDE);
        ShowWindow(rghwndBtn[9], SW_HIDE);
        ShowWindow(rghwndBtn[10], SW_HIDE);
        ShowWindow(hwndBattleDD, SW_HIDE);
        ptile->fFixCtls = FALSE;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsFleetComposition)) == 0) {
        ShowWindow(hwndFleetCompLB, SW_HIDE);
        ShowWindow(rghwndBtn[8], SW_HIDE);
        ShowWindow(rghwndBtn[9], SW_HIDE);
        ShowWindow(rghwndBtn[10], SW_HIDE);
        ShowWindow(hwndBattleDD, SW_HIDE);
    } else {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 3;
        GetClientRect(hwndFleetCompLB, &rcT);
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        dyFleetCompLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0 ? 5 : 3);
        dyWrong = dyFleetCompLB - (rcT.bottom - rcT.top);
        if (dxFleetCompLB == xRight - xLeft && dyWrong >= 0 && dyWrong < dyArial8) {
            swp |= SWP_NOSIZE;
        } else {
            dxFleetCompLB = xRight - xLeft;
        }
        SetWindowPos(hwndFleetCompLB, NULL, xLeft, yTop, xRight - xLeft, dyFleetCompLB, swp);
        ShowWindow(hwndFleetCompLB, SW_SHOW);
        GetClientRect(hwndFleetCompLB, &rcT);
        dyFleetCompLB = rcT.bottom - rcT.top;
        yTop += (gd.fSmallTileMode == 0 ? 4 : 2) + dyFleetCompLB;
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsBattlePlan2, szWork);
        dxLabel = LOWORD(GetTextExtent(hdc, szWork, c));
        if (ptile->fMinDraw == 0) {
            TextOut(hdc, xLeft, yTop + 4, szWork, c);
        }
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        if (dxBattleDD == xRight - xLeft - dxLabel) {
            swp |= SWP_NOSIZE;
        } else {
            dxBattleDD = xRight - xLeft - dxLabel;
        }
        SetWindowPos(hwndBattleDD, NULL, xLeft + dxLabel, yTop, dxBattleDD, 5 * dyShipDD, swp);
        ShowWindow(hwndBattleDD, SW_SHOW);
        yTop += dyShipDD + 3;
        c = CchGetString(idsEstRange, szWork);
        l = GetTextExtent(hdc, szWork, c);
        if (ptile->fMinDraw == 0) {
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        c = CchGetString(idsN9999LY, szWork);
        dxRight = LOWORD(GetTextExtent(hdc, szWork, c)) + 6;
        i = IFindIdealWarp(NULL, FALSE);
        l = EstFuelUse(&sel.fl, 0, i, -1, TRUE);
        if (l >= 1000000000) {
            c = CchGetString(idsInfinite, szWork);
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsLdLY), l);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        if (gd.fSmallTileMode == 0) {
            yTop += dyArial8;
            if (ptile->fMinDraw == 0) {
                SelectObject(hdc, rghfontArial8[1]);
                c = CchGetString(idsPercentCloaked, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
            }
            SelectObject(hdc, rghfontArial8[0]);
            i = PctCloakFromLpfl(&sel.fl);
            if (i == 0) {
                c = CchGetString(idsNone2, szWork);
            } else {
                c = _wsprintf(szWork, PCTDPCTPCT, i);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        }
        yTop += dyArial8 + 2 - gd.fSmallTileMode;
        xStart = xLeft;
        c = (int16_t)(xRight - xLeft - 10) / 3;
        i = 8;
        while (i <= 10) {
            SetWindowPos(rghwndBtn[i], NULL, xStart, yTop, c, (dyArial8 >> 1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(rghwndBtn[i], SW_SHOW);
            i++;
            xStart += c + 6;
        }
        cBoat = 0;
        for (i = 0; i < 16; i++) {
            cBoat += sel.fl.rgcsh[i];
        }
        EnableWindow(rghwndBtn[8], FCanSplit(cBoat));
        EnableWindow(rghwndBtn[9], FCanSplitAll(cBoat));
        EnableWindow(rghwndBtn[10], FCanMerge(pfl));
    }
    return;
}

int16_t FCanSplit(int32_t cBoat) {
    if (rgplr[idPlayer].cFleet == 0x200) {
        return FALSE;
    }
    if (cBoat > 1) {
        return TRUE;
    }
    return FALSE;
}

int16_t FCanSplitAll(int32_t cBoat) {
    if (cBoat - 1 + rgplr[idPlayer].cFleet > 0x200) {
        return FALSE;
    }
    if (cBoat > 1) {
        return TRUE;
    }
    return FALSE;
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
        if (rglpfl[i] == 0)
            break;
        if (lpfl->iPlayer == pfl->iPlayer && lpfl->pt.x == sel.fl.pt.x && lpfl->pt.y == sel.fl.pt.y) {
            cfl++;
            for (ishdef = 0; ishdef < 16; ishdef++) {
                csh += lpfl->rgcsh[ishdef];
            }
        }
    }
    if (cfl == 1 || csh > (int32_t)(uint32_t)(32766 - (rgplr[pfl->iPlayer].cFleet - 1))) {
        return FALSE;
    }
    return TRUE;
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

    fPercent = FALSE;
    if (GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        SetFocus(hwndFrame);
    }
    if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[4] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        SelectAdjFleet(-1, 0);
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[5] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        SelectAdjFleet(1, 0);
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[6] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        strcpy(szWork, PszGetFleetName(sel.fl.id));
        StickyDlgPos(hwnd, &ptStickyRenameDlg, FALSE);
        lpProc = MakeProcInstance(RenameDlg, hInst);
        if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc) != 0) {
            FreeProcInstance(lpProc);
            strcpy(szT, szWork);
            if (strcmp(szT, PszGetFleetName(sel.fl.id)) != 0) {
                LogChangeName(grobjFleet, sel.fl.id, szT);
                InvalidateReport(rptFleets, 1);
                FillOrdersLB();
                DrawPlanShip(NULL, tileFleetOrders | tileBitmap | tileErase);
                InvalidateRect(hwndMessage, NULL, TRUE);
                InvalidateRect(hwndScanner, NULL, TRUE);
                SetMineralTitleBar(hwndMine);
            }
        } else {
            FreeProcInstance(lpProc);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[3] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        SelectAdjPlanet(0, sel.fl.idPlanet);
        SetFleetDropDownSel(sel.fl.id);
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[7] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        if (sel.fl.idPlanet != -1) {
            TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
        } else {
            TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndShipDD) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            DrawPlanShip(NULL, tileShipList | tileErase);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndShipLB) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(hwndShipLB, LB_GETCURSEL, 0, 0);
            SetScanWp(LOWORD(lSel));
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndFleetCompLB) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(hwndFleetCompLB, LB_GETCURSEL, 0, 0);
            if (lSel >= 0) {
                for (ishdef = 0; ishdef < 16 && (sel.fl.rgcsh[ishdef] <= 0 || lSel-- > 0); ishdef++) {
                }
                GlobalPD.grPopup = grPopupShdef;
                GlobalPD.lpshdef = &rgshdef[ishdef];
                GlobalPD.fHideCounts = FALSE;
                GlobalPD.fShowDamage = TRUE;
                GlobalPD.fToken = FALSE;
                GlobalPD.fSummary = FALSE;
                Popup(hwndFleetCompLB, 10, 10);
            }
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndBattleDD) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(hwndBattleDD, CB_GETCURSEL, 0, 0);
            if (lSel != -1) {
                if (lSel == 0) {
                    lpProc = MakeProcInstance(BattlePlansDlg, hInst);
                    lSel = DialogBox(hInst, MAKEINTRESOURCE(IDD_BATTLE_PLANS), hwndFrame, lpProc);
                    FreeProcInstance(lpProc);
                    sel.fl.iplan = lSel;
                } else {
                    sel.fl.iplan = lSel - 1;
                }
                FLookupFleet(-1, &sel.fl);
            }
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[0] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
        if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0) {
            TransferStuff(sel.fl.id, grobjFleet, xf.id, xf.grobj, mdXferCargo);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[1] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
        if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0 && xf.grobj == grobjFleet) {
            SelectAdjFleet(0, xf.id);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[2] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
        if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.fl.id) != 0 && xf.grobj == grobjFleet) {
            TransferStuff(sel.fl.id, grobjFleet, xf.id, grobjFleet, mdXferShips);
            if ((grbitScan & grbitScanFleetPaths) != 0) {
                InvalidateRect(hwndScanner, NULL, TRUE);
            }
            InvalidateReport(rptFleets, 1);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[8] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferShips);
        InvalidateReport(rptFleets, 1);
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[9] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        FFleetSplitAll(&sel.fl);
        FillShipDD(sel.fl.id);
        grbit = -31819;
        FLookupFleet(sel.fl.id, &sel.fl);
        FillFleetCompLB();
        DrawPlanShip(NULL, grbit);
        InvalidateRect(hwndMine, NULL, TRUE);
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[10] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        vrgiflMerge = rgifl;
        vcflMerge = 0;
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            if (lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->pt.x == sel.fl.pt.x && lpfl->pt.y == sel.fl.pt.y) {
                rgifl[vcflMerge++] = ifl;
            }
        }
        lpfl = NULL;
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
                    if (lpfl == 0) {
                        lpfl = LpflFromId(vrgiflMerge[ifl]);
                    } else if (vrgiflMerge[ifl] != lpfl->id) {
                        break;
                    }
                }
            }
            if (ifl == vcflMerge) {
                lpfl = NULL;
            } else if (lpfl->id != sel.fl.id) {
                SelectAdjFleet(0, lpfl->id);
            }
        }
        FreeProcInstance(lpProc);
        if (lpfl != 0) {
            FFleetMergeAll(&sel.fl);
            FillShipDD(sel.fl.id);
            grbit = -31819;
            FLookupFleet(sel.fl.id, &sel.fl);
            FillFleetCompLB();
            DrawPlanShip(NULL, grbit);
            InvalidateRect(hwndMine, NULL, TRUE);
            if ((grbitScan & grbitScanFleetPaths) != 0) {
                InvalidateRect(hwndScanner, NULL, TRUE);
            }
            vrgiflMerge = 0;
            vcflMerge = 0;
            if (gd.fTutorial != 0) {
                AdvanceTutor();
            }
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndRepCB && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
        sel.fl.fRepOrders = LOWORD(SendMessage(hwndRepCB, BM_GETCHECK, 0, 0));
        FLookupFleet(-1, &sel.fl);
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndOrderDD[0]) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0, 0);
            if (LOWORD(lSel) != sel.fl.lpplord->rgord[sel.iwpAct].grTask) {
                if (lSel == 3 || sel.fl.lpplord->rgord[sel.iwpAct].grTask == grTaskMine) {
                    InvalidateRect(hwndMine, NULL, TRUE);
                }
                sel.fl.lpplord->rgord[sel.iwpAct].grTask = LOWORD(lSel);
                fmemset((uint8_t *)&sel.fl.lpplord->rgord[sel.iwpAct] + 8, 0, 10);
                switch (LOWORD(lSel)) {
                case 7:
                    sel.fl.lpplord->rgord[sel.iwpAct].tptl.iDist = 0;
                    sel.fl.lpplord->rgord[sel.iwpAct].tptl.iWarp = 0;
                    break;
                case 9:
                    sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = 0;
                    break;
                case 6:
                    sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = 5;
                    break;
                case 4:
                    if (sel.fl.lpplord->rgord[sel.iwpAct].grobj != grobjFleet) {
                        lpflBest = NULL;
                        ishPrimary = 0;
                        for (ish = 1; ish < 16; ish++) {
                            if (sel.fl.rgcsh[ish] > sel.fl.rgcsh[ishPrimary]) {
                                ishPrimary = ish;
                            }
                        }
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->pt.x == sel.fl.lpplord->rgord[sel.iwpAct].pt.x && lpfl->pt.y == sel.fl.lpplord->rgord[sel.iwpAct].pt.y &&
                                lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->id != sel.fl.id) {
                                if (lpfl->rgcsh[ishPrimary] > 0) {
                                    lpflBest = lpfl;
                                    if (lpfl->cord == 1)
                                        break;
                                } else if (lpflBest == 0 || (lpflBest->cord > 1 && lpfl->cord == 1)) {
                                    lpflBest = lpfl;
                                }
                            }
                        }
                        if (lpflBest != 0) {
                            sel.fl.lpplord->rgord[sel.iwpAct].grobj = grobjFleet;
                            sel.fl.lpplord->rgord[sel.iwpAct].id = lpflBest->id;
                            FLookupFleet(-1, &sel.fl);
                            FillOrdersLB();
                        }
                    }
                }
                FLookupFleet(-1, &sel.fl);
                UpdateOrdersDDs(1);
                DrawPlanShip(NULL, tileStarbaseOrWaypoint | tileErase);
            }
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndOrderDD[1]) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0);
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
                DrawPlanShip(NULL, tileStarbaseOrWaypoint);
                break;
            default:
                sel.fl.lpplord->rgord[sel.iwpAct].tlm.cTime = LOWORD(lSel);
                sel.fl.lpplord->rgord[sel.iwpAct].tlm.cTimeOld = LOWORD(lSel);
                FLookupFleet(-1, &sel.fl);
            }
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndOrderDD[2]) {
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
            lSel = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0, 0);
            lMin = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0);
            if (lMin == 0) {
                lMin = 4;
            } else {
                lMin--;
            }
            sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[lMin].iAction = LOWORD(lSel);
            FLookupFleet(-1, &sel.fl);
            UpdateOrdersDDs(3);
            DrawPlanShip(NULL, tileStarbaseOrWaypoint);
        }
    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndOrderED && GET_WM_COMMAND_CMD(wParam, lParam) == 768) {
        lSel = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0, 0);
        if (lSel == 5 || lSel == 6) {
            fPercent = TRUE;
        }
        GetWindowText(hwndOrderED, rgb, 8);
        iInit = atoi(rgb);
        i = 0 <= iInit ? iInit : 0;
        if (fPercent != 0) {
            i = 100 >= i ? i : 100;
        } else {
            i = 4000 >= i ? i : 4000;
        }
        if (iInit != i) {
            AlertSz(PszFormatIds(idsAmountCargoMaySpecifyHereMustBetween, NULL), MB_ICONHAND);
            _wsprintf(szWork, PCTD, i);
            SetWindowText(hwndOrderED, szWork);
        }
        lMin = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0);
        if (lMin == 0) {
            lMin = 4;
        } else {
            lMin--;
        }
        sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[lMin].cQuan = i;
        FLookupFleet(-1, &sel.fl);
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
            InvalidateReport(rptFleets, 1);
        }
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0 || lpfl->id == idFleet)
                break;
        }
        if (i == cFleet || lpfl->iPlayer != idPlayer) {
            if (i == cFleet) {
                return;
            }
            pt = lpfl->pt;
            scan.pt = lpfl->pt;
            scan.grobj = grobjPlanet | grobjFleet | mdExact;
            ChangeScanSel(&scan, 0);
        } else {
            if (dInc != 0) {
                for (i = 0; i < (int16_t)rgplr[idPlayer].cFleet && rglpfl[vlprgidFleet[i]]->id != idFleet; i++) {
                }
                i += dInc;
                if (i >= (int16_t)rgplr[idPlayer].cFleet) {
                    i = 0;
                } else if (i < 0) {
                    i = rgplr[idPlayer].cFleet - 1;
                }
                i = vlprgidFleet[i];
            } else if (sel.grobj == grobjFleet && sel.fl.pt.x == lpfl->pt.x && sel.fl.pt.y == lpfl->pt.y) {
                idOld = sel.fl.id;
            }
            lpflT = rglpfl[i];
            idNew = lpflT->id;
            pt = lpflT->pt;
            scan.pt = lpflT->pt;
            scan.grobj = grobjFleet | mdExact;
            ChangeScanSel(&scan, 0);
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(grobjFleet, idNew);
            RedrawScanSel(NULL, 1);
        }
        CtrPointScan(pt, TRUE);
        DrawScannerSBar(NULL, NULL, NULL, FALSE);
        InvalidateRect(hwndMine, NULL, TRUE);
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
            iOffset++;
        }
    }
    SendMessage(hwndShipDD, CB_SETCURSEL, iOffset, 0);
    DrawPlanShip(NULL, tileShipList | tileMinimized);
    return;
}

int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat) {
    int16_t i;
    int32_t l;

    l = 0;
    if (lpfl->det != detAll) {
        return 32000;
    }
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] != 0) {
            l += (uint32_t)(lpfl->rgcsh[i] * WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, grStat));
        }
    }
    return l;
}

int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat) {
    int16_t wt;
    int16_t j;
    HUL    *lphul;

    lphul = &lpshdef->hul;
    if (grStat != 1) {
        if (grStat != 2) {
            return 0;
        }
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtCargoMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst == hstSpecialM) {
                switch (lphul->rghs[j].iItem) {
                default:
                    break;
                case ispecialMCargoPod:
                    wt += lphul->rghs[j].cItem * 50;
                    break;
                case ispecialMSuperCargoPod:
                    wt += lphul->rghs[j].cItem * 100;
                    break;
                case ispecialMMultiCargoPod:
                    wt += lphul->rghs[j].cItem * 250;
                }
            }
        }
    } else {
        wt = LphuldefFromId(lphul->ihuldef)->hul.wtFuelMax;
        for (j = 0; j < lphul->chs; j++) {
            if (lphul->rghs[j].grhst != hstSpecialM) {
                if (lphul->rghs[j].grhst == hstSpecialE && lphul->rghs[j].iItem == ispecialEAntiMatterGenerator) {
                    wt += lphul->rghs[j].cItem * 200;
                }
            } else if (lphul->rghs[j].iItem == ispecialMFuelTank) {
                wt += lphul->rghs[j].cItem * 250;
            } else if (lphul->rghs[j].iItem == ispecialMSuperFuelTank) {
                wt += lphul->rghs[j].cItem * 500;
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

    if (lpfl == 0) {
        lpfl = &sel.fl;
    }
    SelectObject(hdc, rghfontArial8[1]);
    cSections = 1;
    lMax = LGetFleetStat(lpfl, 2);
    if (grbit >= 0 && grbit <= 4) {
        rghbr[0] = rghbrMineral[grbit];
        rgSize[0] = lpfl->rgwtMin[grbit];
        if (grbit == 4) {
            lMax = LGetFleetStat(lpfl, 1);
        }
    } else {
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
            if (rgSize[0] > 10 || (rgSize[0] == 10 && IFindIdealWarp(&sel.fl, FALSE) < 10)) {
                rghbr[0] = rghbrMineral[2];
                break;
            }
            rghbr[0] = rghbrMineral[4];
            break;
        case 7:
            lMax = 10;
            rgSize[0] = (uint32_t)sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
            if (rgSize[0] > 10 || (rgSize[0] == 10 && IFindIdealWarp(&sel.fl, FALSE) < 10)) {
                rghbr[0] = rghbrMineral[2];
            } else {
                rghbr[0] = rghbrMineral[4];
            }
        }
    }
    l = LDrawGauge(hdc, prc, cSections, rgSize, rghbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if (grbit == 6) {
        if (l == 0) {
            c = CchGetString(idsStopped, szWork);
        } else if (l < 11) {
            c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l);
        } else {
            c = CchGetString(idsUseStargate, szWork);
        }
    } else if (grbit == 7) {
        if (l != 0) {
            c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l);
        } else {
            c = CchGetString(idsAutomatic, szWork);
        }
    } else if (cSections == 1 && grbit != 4) {
        c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), l);
    } else if (grbit == 4) {
        c = _wsprintf(szWork, PszGetCompressedString(idsLdLdmg), l, lMax);
    } else {
        c = _wsprintf(szWork, PszGetCompressedString(idsLdLdkt), l, lMax);
    }
    l = GetTextExtent(hdc, szWork, c);
    if ((int16_t)LOWORD(l) < prc->right - prc->left - 3) {
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
        x += 2;
        y += 2;
    }
    if (ibmp < 0) {
        i = IshdefPrimaryFromLpfl(lpfl, &cDiff);
        ibmp = rglpshdef[lpfl->iPlayer][i].hul.ibmp;
    }
    ibmp %= 148;
    SelectPalette(hdc, vhpal, FALSE);
    RealizePalette(hdc);
    if (fShrink != 0) {
        DibBlt(hdc, x, y, 32, 32, rghdibShipsT[ibmp >> 5], ((ibmp & 0x1f) >> 2) * 0x20, (3 - (ibmp & 3)) * 0x20, 32, 32, 13369376);
        dxy = 32;
        dxyPlus = 5;
        dxyPlusWidth = 1;
        if (ibmpRace >= 0) {
            DibBlt(hdc, x, y + 24, 8, 8, hdibRacesX, (ibmpRace & 7) * 8, (3 - ((ibmpRace & 0x1f) >> 3)) * 8, 8, 8, 13369376);
        }
    } else {
        DibBlt(hdc, x, y, 64, 64, rghdibShips[ibmp >> 5], ((ibmp & 0x1f) >> 2) * 0x40, (3 - (ibmp & 3)) * 0x40, 64, 64, 13369376);
        dxy = 64;
        dxyPlus = 8;
        dxyPlusWidth = 2;
        if (ibmpRace >= 0) {
            DibBlt(hdc, x, y + 48, 16, 16, hdibRacesT, (ibmpRace & 7) * 0x10, (3 - ((ibmpRace & 0x1f) >> 3)) * 0x10, 16, 16, 13369376);
        }
    }
    if (csh == 0 || fShrink != 0) {
        if (cDiff > 4) {
            cDiff = 4;
        }
        cDiff--;
        for (i = 0; i < cDiff; i++) {
            xCur = ((i & 1) ^ ((i & 2) == 2)) == 0 ? x + 2 : x + dxy - 2 - dxyPlus;
            yCur = (i & 2) == 0 ? y + 2 : y + dxy - 2 - dxyPlus;
            PatBlt(hdc, xCur, (int16_t)(dxyPlus - 1) / 2 + yCur, dxyPlus, dxyPlusWidth, WHITENESS);
            PatBlt(hdc, (int16_t)(dxyPlus - 1) / 2 + xCur, yCur, dxyPlusWidth, dxyPlus, WHITENESS);
        }
    } else {
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
            return TRUE;
        }
        if (FLookupFleet(iFleet, &fl) == 0) {
            return TRUE;
        }
        pt = fl.pt;
        if (FLookupFleet(prtxfer->id1, &fl) == 0) {
            return TRUE;
        }
        if (fl.pt.x != pt.x || fl.pt.y != pt.y) {
            return TRUE;
        }
        grbit = prtxfer->grbitItems;
        if (rt == rtLogCargoXfer16) {
            prtxferx = lprt;
        }
        j = 0;
        i = 0;
        while (i < 5) {
            if ((grbit & 1) != 0) {
                if (rt == rtLogCargoXfer8) {
                    lppl->rgwtMin[i] -= (int16_t)prtxfer->rgcQuan[j];
                } else {
                    lppl->rgwtMin[i] -= prtxferx->rgcQuan[j];
                }
                j++;
            }
            i++;
            grbit >>= 1;
        }
    }
    return TRUE;
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

    lPopPrev = -1;
    xfer[0].id = id1;
    xfer[0].grobj = grobj1;
    xfer[1].id = id2;
    xfer[1].grobj = grobj2;
    pxfer = xfer;
    mdXferDlg = mdXfer;
    for (i = 0; i < 2; i++) {
        if (xfer[i].grobj != grobjOther) {
            if (FLookupObject(xfer[i].grobj, xfer[i].id, &xfer[i].fl) == 0) {
                return 0;
            }
            if (xfer[i].grobj == grobjPlanet) {
                lPopPrev = xfer[i].pl.rgwtMin[3];
            }
        } else if (mdXfer == mdXferShips) {
            lpfl = LpflNewSplit(&xfer[0].fl);
            xfer[1].fl = *lpfl;
            xfer[1].id = lpfl->id;
            xfer[1].grobj = grobjFleet;
        } else {
            xfer[i].fl.id = -1;
            for (j = 0; j < 4; j++) {
                xfer[i].pl.rgwtMin[j] = 0;
            }
            EnumLogRts((int16_t (*)(void *, int16_t, int16_t, void *, int16_t))FEnumCalcJettison, &xfer[i].pl, id1);
        }
    }
    if (mdXfer == mdXferShips) {
        cXferValidHulls = 0;
        for (i = 0; i < 16; i++) {
            if (xfer[0].fl.rgcsh[i] != 0 || (xfer[1].grobj == grobjFleet && xfer[1].fl.rgcsh[i] != 0)) {
                rgValidHull[cXferValidHulls++] = i;
            }
        }
        rgXferValidHulls = rgValidHull;
    }
    rgbtnXfer = rgbtn;
    crgbtnXfer = 32;
    if (gd.fTutorial != 0) {
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
            }
        }
        if (iDelFleet != -1) {
            FDeleteFleet(xfer[iDelFleet].fl.id, grobjFleet, xfer[iDelFleet == 0].fl.id);
        }
        if (mdXfer == mdXferShips) {
            FillShipDD(sel.fl.id);
            if ((grbitScan & grbitScanFleetPaths) != 0) {
                InvalidateRect(hwndScanner, NULL, TRUE);
            }
        }
        if (sel.grobj == grobjPlanet) {
            grbit = -32691;
            FLookupPlanet(sel.pl.id, &sel.pl);
            FillPlanetProdLB(NULL, NULL, NULL);
        } else {
            grbit = -31819;
            FLookupFleet(sel.fl.id, &sel.fl);
            FillFleetCompLB();
        }
        DrawPlanShip(NULL, grbit);
        if (sel.scan.grobj == grobjFleet) {
            InvalidateRect(hwndMine, NULL, TRUE);
        } else {
            InvalidateMineralBars();
        }
        if (lPopPrev != -1 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh && (grbitScan & grbitScanCoverage) != 0) {
            InvalidateRect(hwndScanner, NULL, FALSE);
            goto L_5673;
        }
        if ((lPopPrev == -1 || (grbitScan & grbitScanViewMask) != 4) && (grbitScan & grbitScanViewMask) != 1)
            goto L_5673;
        pt = sel.pt;
        LogicalToScan(&pt);
        rc.right = pt.x;
        rc.bottom = pt.y;
        rc.left = pt.x;
        rc.top = pt.y;
        InflateRect(&rc, 20, 20);
        rc.top -= 20;
        InvalidateRect(hwndScanner, &rc, FALSE);
        goto L_5673;
    }
    if (mdXfer != mdXferShips || grobj2 != grobjOther)
        goto L_5673;
CancelSplit:
    FDeleteFleet(xfer[1].fl.id, grobjNone, 0);
    CancelMemRt(rtLogFleetSplit);
L_5673:
    mdXferDlg = mdXferNone;
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

    switch (message) {
    case WM_INITDIALOG:
        StickyDlgPos(hwnd, &ptStickyTransferDlg, TRUE);
        GetClientRect(hwnd, &rc);
        if (mdXferDlg == mdXferShips) {
            SetWindowText(hwnd, PszGetCompressedString(idsShipTransfer));
            if (cXferValidHulls > 10) {
                dyMore = rc.bottom / 2;
                rc.bottom += dyMore;
                SetWindowPos(hwnd, NULL, 0, 0, rc.right, rc.bottom, SWP_NOMOVE | SWP_NOZORDER);
                GetClientRect(hwnd, &rc);
                dyMore -= GetSystemMetrics(SM_CYCAPTION) + 2;
                dx = GetSystemMetrics(SM_CXDLGFRAME) + 4;
                hwndBtn = GetDlgItem(hwnd, IDOK);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                ScreenToClient16(hwnd, &pt);
                pt.y += dyMore;
                pt.x -= dx;
                SetWindowPos(hwndBtn, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
                hwndBtn = GetDlgItem(hwnd, IDC_HELP);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                ScreenToClient16(hwnd, &pt);
                pt.y += dyMore;
                pt.x -= dx;
                SetWindowPos(hwndBtn, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
                hwndBtn = GetDlgItem(hwnd, IDCANCEL);
                GetWindowRect(hwndBtn, &rcBtn);
                pt.x = rcBtn.left;
                pt.y = rcBtn.top;
                ScreenToClient16(hwnd, &pt);
                pt.y += dyMore;
                pt.x -= dx;
                SetWindowPos(hwndBtn, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
            }
        }
        FSetupXferBtns(&rc);
        if (gd.fTutorial != 0) {
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
        DrawXferDlg(hwnd, hdc, &rc, SupplyAll);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        return FTrackXfer(hwnd, LOWORD(lParam), HIWORD(lParam), wParam);
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case IDOK:
        case IDCANCEL:
            StickyDlgPos(hwnd, &ptStickyTransferDlg, FALSE);
            EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK);
            if (gd.fTutorial != 0) {
                AdvanceTutor();
            }
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, HELP_CONTEXT, (uint32_t)(mdXferDlg == mdXferShips ? 1080 : 1075));
            return 1;
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
    uint16_t t_merge_5ecd_0001;

    GetClientRect(hwnd, &rc);
    pt.x = x;
    pt.y = y;
    for (i = 0; i < crgbtnXfer && ((rgbtnXfer[i].bt & 4) != 0 || PtInRect(&rgbtnXfer[i].rc, PointFrom16(pt)) == 0); i++) {
    }
    if (i == crgbtnXfer) {
        return FALSE;
    }
    iBtn = i >> 1;
    btn = rgbtnXfer[i];
    iVal = btn.iVal & 0x7f;
    if (btn.fVisible != 0) {
        InitBtnTrack(&btnt, hwnd, NULL, &btn.rc, btn.bt, 80, FALSE, FALSE, NULL);
        if ((fkb & 8) != 0) {
            dChg = (uint32_t)((fkb & 4) == 0 ? 100 : 1000);
        } else if ((fkb & 4) != 0) {
            dChg = 10;
        } else {
            dChg = 1;
        }
        while (FTrackBtn(&btnt) != 0) {
            if (mdXferDlg == mdXferShips) {
                i = (int16_t)LOWORD(dChg) < pxfer[btn.iSide == 0].fl.rgcsh[iVal] ? LOWORD(dChg) : pxfer[btn.iSide == 0].fl.rgcsh[iVal];
                if (i != 0) {
                    if (pxfer[btn.iSide].fl.rgcsh[iVal] >= 32766 - i) {
                        i = 1;
                    }
                    pxfer[btn.iSide].fl.rgcsh[iVal] = pxfer[btn.iSide].fl.rgcsh[iVal] + i;
                    t_merge_5ecd_0001 = btn.iSide == 0;
                    pxfer[t_merge_5ecd_0001].fl.rgcsh[iVal] -= i;
                    DrawXferDlg(hwnd, btnt.hdc, &rc, iBtn);
                }
            } else if (iVal >= 0 && iVal <= 4 && XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0) {
                DrawXferDlg(hwnd, btnt.hdc, &rc, iVal);
            }
        }
    } else if (iVal <= 4) {
        if (pxfer[1].grobj == grobjThing) {
            if (iVal == 4 || iVal == 3)
                goto FinishUp;
        } else if (pxfer[btn.iSide].fl.iPlayer != idPlayer) {
            goto FinishUp;
        }
        SetCapture(hwnd);
        ptOld.y = -1;
        ptOld.x = -1;
        while (FGetMouseMove(&pt) != 0) {
            if (pt.x != ptOld.x || pt.y != ptOld.y) {
                ptOld = pt;
                if (btn.iSide == 1 && pxfer[1].grobj == grobjThing) {
                    cNew = (uint32_t)(pxfer[1].th.thp.wtMax * 10);
                } else if (iVal == 4) {
                    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 1);
                } else {
                    cNew = LGetFleetStat(&pxfer[btn.iSide].fl, 2);
                }
                cNew = (int32_t)((int16_t)(pt.x - btn.rc.left) * cNew) / (int16_t)(btn.rc.right - btn.rc.left - 2);
                cCur = ChgCargo(pxfer[btn.iSide].grobj, pxfer[btn.iSide].id, iVal, 0, (uint8_t *)(pxfer + btn.iSide) + 4);
                dChg = cNew - cCur;
                if (XferSupply(iVal, btn.iSide == 0 ? dChg : -dChg) != 0) {
                    DrawXferDlg(hwnd, NULL, &rc, iVal);
                }
            }
        }
        ReleaseCapture();
    }
FinishUp:
    UpdateXferBtns();
    DrawXferDlg(hwnd, NULL, &rc, SupplyButtonsOnly);
    return TRUE;
}

int32_t GetCargoFree(FLEET *lpfl) {
    int32_t cHave;
    int16_t i;

    cHave = 0;
    for (i = 0; i <= 3; i++) {
        cHave += lpfl->rgwtMin[i];
    }
    return LGetFleetStat(lpfl, 2) - cHave;
}

int32_t GetFuelFree(FLEET *lpfl) {
    int32_t t_call_6017;

    t_call_6017 = LGetFleetStat(lpfl, 1);
    return t_call_6017 - lpfl->rgwtMin[4];
}

int32_t ChgCargo(GrobjClass grobj, int16_t id, MineralType iSupply, int32_t dChg, void *pobj) {
    THING  *pth;
    XFER    xfer;
    int16_t i;
    FLEET  *pfl;
    PLANET *ppl;
    int32_t wtFree;
    int32_t t_merge_6425_0001;

    switch (grobj) {
    case grobjPlanet:
    case grobjOther:
        if (pobj != 0) {
            ppl = pobj;
        } else if (grobj == grobjPlanet) {
            FLookupPlanet(id, &xfer.pl);
            ppl = &xfer.pl;
        } else {
            memset(&xfer.pl, 0, sizeof(PLANET));
            ppl = &xfer.pl;
        }
        if (iSupply <= Fuel) {
            if (iSupply == Fuel) {
                return 0;
            }
            if (dChg == 0) {
                return ppl->rgwtMin[iSupply];
            }
            if (ppl->rgwtMin[iSupply] + dChg < 0) {
                dChg = -ppl->rgwtMin[iSupply];
            }
            ppl->rgwtMin[iSupply] += dChg;
        }
        if (dChg == 0 || pobj != 0 || grobj == grobjOther)
            break;
        FLookupPlanet(-1, &xfer.pl);
        break;
    case grobjThing:
        if (pobj != 0) {
            pth = pobj;
        } else {
            FLookupThing(id, &xfer.th);
            pth = &xfer.th;
        }
        if (iSupply >= Colonists) {
            return 0;
        }
        if (iSupply <= Fuel) {
            if (dChg == 0) {
                return pth->thp.rgwtMin[iSupply];
            }
            if (pth->thp.rgwtMin[iSupply] + dChg < 0) {
                dChg = (int16_t)-pth->thp.rgwtMin[iSupply];
            }
            wtFree = (uint32_t)(pth->thp.wtMax * 10);
            for (i = 0; i < 3; i++) {
                wtFree -= pth->thp.rgwtMin[i];
            }
            if (dChg > wtFree) {
                dChg = wtFree;
            }
            pth->thp.rgwtMin[iSupply] += LOWORD(dChg);
        }
        if (dChg == 0 || pobj != 0)
            break;
        FLookupThing(-1, pth);
        break;
    default:
        if (pobj != 0) {
            pfl = pobj;
        } else {
            FLookupFleet(id, &xfer.fl);
            pfl = &xfer.fl;
        }
        if (iSupply <= Fuel) {
            if (dChg == 0) {
                return pfl->rgwtMin[iSupply];
            }
            if (pfl->rgwtMin[iSupply] + dChg < 0) {
                dChg = -pfl->rgwtMin[iSupply];
            }
            if (iSupply == Colonists && pfl->det != detAll) {
                dChg = 0;
            }
            t_merge_6425_0001 = iSupply == Fuel ? GetFuelFree(pfl) : GetCargoFree(pfl);
            if (dChg >= t_merge_6425_0001) {
                if (iSupply == Fuel) {
                    dChg = GetFuelFree(pfl);
                } else {
                    dChg = GetCargoFree(pfl);
                }
            }
            pfl->rgwtMin[iSupply] += dChg;
        }
        if (dChg != 0 && pobj == 0) {
            FLookupFleet(-1, pfl);
        }
    }
    return dChg;
}

int32_t XferSupply(MineralType iSupply, int32_t cQuan) {
    int16_t iSrc;
    int32_t dChg;
    int32_t cAvailable;

    if (cQuan == 0) {
        return 0;
    }
    iSrc = cQuan > 0;
    if (iSrc == 0) {
        cQuan = -cQuan;
    }
    cAvailable = ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, 0, (uint8_t *)(pxfer + iSrc) + 4);
    if (cQuan > cAvailable) {
        cQuan = cAvailable;
    }
    if (cQuan == 0) {
        return 0;
    }
    dChg = ChgCargo(pxfer[iSrc == 0].grobj, pxfer[iSrc == 0].id, iSupply, cQuan, (uint8_t *)(pxfer + (iSrc == 0)) + 4);
    if (dChg != 0) {
        ChgCargo(pxfer[iSrc].grobj, pxfer[iSrc].id, iSupply, -dChg, (uint8_t *)(pxfer + iSrc) + 4);
    }
    return dChg;
}

void UpdateXferBtns() {
    int16_t iSide;
    int16_t i;
    int16_t iLastButton;
    int16_t iVal;
    int32_t lLeft;

    iLastButton = mdXferDlg == mdXferShips ? cXferValidHulls * 2 : 4;
    for (i = 0; i < crgbtnXfer; i++) {
        iVal = rgbtnXfer[i].iVal;
        if (rgbtnXfer[i].fVisible != 0 && (iVal <= iLastButton || mdXferDlg == mdXferShips)) {
            iSide = rgbtnXfer[i].iSide;
            if (mdXferDlg == mdXferShips) {
                if (pxfer[iSide].fl.rgcsh[iVal] == 32766) {
                    lLeft = 0;
                } else {
                    lLeft = pxfer[iSide == 0].fl.rgcsh[iVal];
                }
            } else {
                lLeft = ChgCargo(pxfer[iSide == 0].grobj, pxfer[iSide == 0].id, iVal, 0, (uint8_t *)(pxfer + (iSide == 0)) + 4);
                if (lLeft == 0 || pxfer[iSide].grobj != grobjFleet) {
                    if (pxfer[iSide].grobj == grobjPlanet && iVal == 4) {
                        lLeft = 0;
                    }
                } else if (iVal == 4) {
                    lLeft = GetFuelFree(&pxfer[iSide].fl);
                } else {
                    lLeft = GetCargoFree(&pxfer[iSide].fl);
                }
            }
            if (lLeft == 0) {
                rgbtnXfer[i].bt |= 4;
            } else {
                rgbtnXfer[i].bt &= 0xfffb;
            }
        }
    }
    return;
}

void DrawXferDlg(HWND hwnd, HDC hdc, RECT *prc, MineralType iSupply) {
    RECT    rgrc[2];
    int16_t fCreatedDC;
    int16_t i;
    int16_t dxCtr;

    fCreatedDC = FALSE;
    if (hdc == 0) {
        fCreatedDC = TRUE;
        hdc = GetDC(hwnd);
    }
    dxCtr = prc->right >> 1;
    if (iSupply < Ironium) {
        PatBlt(hdc, dxCtr, 0, 1, prc->bottom, BLACKNESS);
        for (i = 0; i < crgbtnXfer; i++) {
            if (rgbtnXfer[i].fVisible != 0) {
                DrawBtn(hdc, &rgbtnXfer[i].rc, rgbtnXfer[i].bt, FALSE, NULL);
            }
        }
        if (iSupply == SupplyButtonsOnly)
            goto RelDC;
    }
    GetXferLeftRightRcs(prc, rgrc, &rgrc[1]);
    for (i = 0; i < 2; i++) {
        if (mdXferDlg == mdXferShips) {
            DrawFleetShipsXferSide(hdc, &rgrc[i], &pxfer[i].fl, iSupply);
        } else {
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
            }
        }
    }
RelDC:
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

void GetXferLeftRightRcs(RECT *prcWhole, RECT *prcLeft, RECT *prcRight) {
    SetRect(prcLeft, 0, 0, prcWhole->right >> 1, prcWhole->bottom);
    ExpandRc(prcLeft, -(dyArial8 + 3) - 4, -4);
    prcLeft->left -= dyArial8 + 1;
    SetRect(prcRight, prcWhole->right >> 1, 0, prcWhole->right, prcWhole->bottom);
    ExpandRc(prcRight, -(dyArial8 + 3) - 4, -4);
    prcRight->right += dyArial8 + 1;
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
    fThingXfer = pxfer[1].grobj == grobjThing;
    dxCtr = prc->right >> 1;
    dy = dyArial8 + 10;
    iMax = mdXferDlg == mdXferShips ? cXferValidHulls : 5;
    if (mdXferDlg != mdXferShips) {
        dy += (dyArial8 + 6) * 2;
    }
    i = 0;
    while (i < iMax) {
        if (i == 4 && mdXferDlg != mdXferShips) {
            dy -= (dyArial8 + 6) * 6;
        }
        SetRect(&rcBtn, dxCtr - (dyArial8 + 3) + 1, dy, dxCtr + 1, dyArial8 + 3 + dy);
        for (j = 0; j < 2; j++) {
            rgbtnXfer[cBtn].rc = rcBtn;
            rgbtnXfer[cBtn].bt = j == 0 ? 2 : 3;
            if (fThingXfer != 0 && i >= 4) {
                rgbtnXfer[cBtn].fVisible = FALSE;
                rgbtnXfer[cBtn].rc.bottom = -100;
            } else {
                rgbtnXfer[cBtn].fVisible = TRUE;
            }
            rgbtnXfer[cBtn].iSide = j;
            rgbtnXfer[cBtn].iVal = mdXferDlg == mdXferShips ? rgXferValidHulls[i] : i;
            cBtn++;
            OffsetRc(&rcBtn, dyArial8 + 2, 0);
        }
        i++;
        dy += dyArial8 + 6;
    }
    GetXferLeftRightRcs(prc, &rcLeft, &rcRight);
    if (mdXferDlg != mdXferShips) {
        if (pxfer->grobj != grobjPlanet) {
            rc = rcLeft;
            i = 0;
        } else {
            if (pxfer[1].grobj == grobjPlanet)
                goto NoGauges;
            rc = rcRight;
            i = 1;
        }
        for (; i < 2; i++) {
            SetRect(&rcBtn, rc.left + dxLabels + 10, rc.top + dyArial8 + 6, rc.right - 4, dyArial8 * 2 + rc.top + 6);
            if (mdXferDlg == mdXferShips) {
                iMin = 0;
                iMax = cXferValidHulls;
            } else {
                iMin = 0;
                iMax = 5;
            }
            for (j = iMin; j < iMax; j++) {
                if (mdXferDlg != mdXferShips) {
                    if (j == 0) {
                        OffsetRc(&rcBtn, 0, (dyArial8 + 6) * 2);
                    } else if (j == 4) {
                        OffsetRc(&rcBtn, 0, -(dyArial8 + 6) * 6);
                    }
                }
                rgbtnXfer[cBtn].rc = rcBtn;
                rgbtnXfer[cBtn].bt = 0;
                rgbtnXfer[cBtn].fVisible = FALSE;
                rgbtnXfer[cBtn].iSide = i;
                rgbtnXfer[cBtn].iVal = j + 128;
                cBtn++;
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
    return TRUE;
}

void DrawThingXferSide(HDC hdc, RECT *prc, THING *pth, MineralType iSupply) {
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
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == SupplyAll) {
        RcCtrTextOut(hdc, &rc, PszGetThingName(pth->idFull), 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3 + (dyArial8 + 6);
    if (iSupply == SupplyAll) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = -1; i < 3; i++) {
            if (i != 4 && i != 4) {
                RightTextOut(hdc, xLeft + dxLabels, yTop, PszGetCompressedString(i == -1 ? idsPacketShell : i + 430), 0, 0);
            }
            yTop += dyArial8 + 6;
        }
    }
    if (iSupply != Fuel && iSupply != Colonists) {
        yTop = rc.bottom + 3 + (dyArial8 + 6);
        xLeft += dxLabels + 6;
        SetRect(&rcGauge, xLeft, yTop, xRight, yTop + dyArial8);
        DrawThingGauge(hdc, &rcGauge, pth, 5);
        for (i = 0; i < 3; i++) {
            OffsetRc(&rcGauge, 0, dyArial8 + 6);
            if (iSupply == SupplyAll || iSupply == i) {
                DrawThingGauge(hdc, &rcGauge, pth, i);
                if (iSupply == i)
                    break;
            }
        }
    }
    return;
}

void DrawFleetCargoXferSide(HDC hdc, RECT *prc, FLEET *pfl, MineralType iSupply) {
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

    fOtherPlr = pfl->iPlayer != idPlayer;
    dxLabels = 75;
    fl = *pfl;
    rc = *prc;
    rc.bottom = rc.top + rc.right - rc.left;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == SupplyAll) {
        RcCtrTextOut(hdc, &rc, PszGetFleetName(fl.id), 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3;
    if (iSupply == SupplyAll) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < 6; i++) {
            if (i != 6 || fOtherPlr == 0) {
                RightTextOut(hdc, xLeft + dxLabels, yTop, PszGetCompressedString(i + 428), 0, 0);
            }
            yTop += dyArial8 + 6;
        }
    }
    yTop = rc.bottom + 3;
    xLeft += dxLabels + 6;
    if (fOtherPlr != 0) {
        xRight = xLeft + dxMaxMineralQuan;
        SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 2, yTop + dyArial8 + 1);
        i = 0;
        while (i < 6) {
            if (i == 1) {
                OffsetRc(&rc, 0, dyArial8 + 6);
            } else {
                if (i == 0) {
                    iMap = 4;
                } else {
                    iMap = i - 2;
                }
                if (iSupply == SupplyAll || iSupply == iMap) {
                    _Draw3dFrame(hdc, &rc, iSupply == iMap);
                    c = _wsprintf(szWork, PszGetCompressedString((iMap == 4) + 892), fl.rgwtMin[iMap]);
                    RightTextOut(hdc, xRight, yTop, szWork, c, 0);
                    if (iSupply == i)
                        break;
                }
                OffsetRc(&rc, 0, dyArial8 + 6);
            }
            i++;
            yTop += dyArial8 + 6;
        }
    } else {
        SetRect(&rcGauge, xLeft, yTop, xRight, yTop + dyArial8);
        if (iSupply == SupplyAll || iSupply == Fuel) {
            DrawFleetGauge(hdc, &rcGauge, &fl, 4);
        }
        if (iSupply != Fuel) {
            yTop += dyArial8 + 6;
            OffsetRc(&rcGauge, 0, dyArial8 + 6);
            DrawFleetGauge(hdc, &rcGauge, &fl, 5);
            yTop += dyArial8 + 6;
            for (i = 0; i <= 3; i++) {
                OffsetRc(&rcGauge, 0, dyArial8 + 6);
                if (iSupply == SupplyAll || iSupply == i) {
                    DrawFleetGauge(hdc, &rcGauge, &fl, i);
                    if (iSupply == i)
                        break;
                }
                yTop += dyArial8 + 6;
            }
        }
    }
    return;
}

void DrawFleetShipsXferSide(HDC hdc, RECT *prc, FLEET *pfl, MineralType iSupply) {
    int16_t yTop;
    int16_t fOtherPlr;
    int16_t c;
    int16_t i;
    int16_t xRight;
    FLEET   fl;
    int16_t xLeft;
    RECT    rc;

    fOtherPlr = pfl->iPlayer != idPlayer;
    fl = *pfl;
    rc = *prc;
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    if (iSupply == SupplyAll) {
        RcCtrTextOut(hdc, &rc, PszGetFleetName(fl.id), 0);
    }
    xLeft = prc->right - 4 - dxMaxMineralQuan - 2;
    xRight = xLeft + dxMaxMineralQuan;
    yTop = rc.bottom + 3;
    if (iSupply == SupplyAll) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < cXferValidHulls; i++) {
            RightTextOut(hdc, xLeft - 8, yTop, rgshdef[rgXferValidHulls[i]].hul.szClass, 0, 0);
            yTop += dyArial8 + 6;
        }
    }
    yTop = rc.bottom + 3;
    SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 2, yTop + dyArial8 + 1);
    for (i = 0; i < cXferValidHulls; i++) {
        if (iSupply == SupplyAll || iSupply == i) {
            _Draw3dFrame(hdc, &rc, iSupply == i);
            c = _wsprintf(szWork, PCTD, pfl->rgcsh[rgXferValidHulls[i]]);
            RightTextOut(hdc, xRight, yTop, szWork, c, 0);
            if (iSupply == i)
                break;
        }
        OffsetRc(&rc, 0, dyArial8 + 6);
        yTop += dyArial8 + 6;
    }
    return;
}

void DrawPlanetXferSide(HDC hdc, RECT *prc, PLANET *ppl, MineralType iSupply) {
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
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
    }
    ExpandRc(&rc, -1, -1);
    rc.bottom = rc.top + dyArial8 + 2;
    if (iSupply == SupplyAll) {
        _Draw3dFrame(hdc, &rc, 0);
        if (pl.id != -1) {
            psz = PszGetPlanetName(pl.id);
        } else {
            psz = PszGetCompressedString(idsDeepSpace);
        }
        RcCtrTextOut(hdc, &rc, psz, 0);
    }
    xLeft = prc->left + 4;
    xRight = prc->right - 4;
    yTop = rc.bottom + 3;
    if (iSupply == SupplyAll) {
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < 6; i++) {
            if (i > 1) {
                RightTextOut(hdc, xLeft + 75, yTop, PszGetCompressedString(i + 428), 0, 0);
            }
            yTop += dyArial8 + 6;
        }
    }
    yTop = rc.bottom + 3;
    xLeft += 81;
    xRight = xLeft + dxMaxMineralQuan + 12;
    SetRect(&rc, xLeft - 2, yTop - 1, xLeft + dxMaxMineralQuan + 14, yTop + dyArial8 + 1);
    i = 0;
    while (i <= 4) {
        if (i == 0) {
            yTop += (dyArial8 + 6) * 2;
            OffsetRc(&rc, 0, (dyArial8 + 6) * 2);
        } else if (i == 4) {
            yTop -= (dyArial8 + 6) * 6;
            OffsetRc(&rc, 0, (dyArial8 + 6) * 6);
        }
        if ((iSupply == SupplyAll || iSupply == i) && i != 4) {
            _Draw3dFrame(hdc, &rc, iSupply == i);
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), pl.rgwtMin[i]);
            RightTextOut(hdc, xRight, yTop, szWork, c, 0);
            if (iSupply == i)
                break;
        }
        OffsetRc(&rc, 0, dyArial8 + 6);
        i++;
        yTop += dyArial8 + 6;
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

    lTempMin = 0;
    irc = -1;
    if (sel.grobj == grobjNone) {
        return NULL;
    }
    if (PtInRect(&rgrcRef[5], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        if (fRightBtn != 0) {
            sz255[0] = -1;
            sz255[1] = 0;
            for (i = 0; i < 4; i++) {
                rgszZip[i] = rgszZipOrder[i];
            }
            rgszZip[4] = sz255;
            cMax = 5;
            for (i = 0; i < 4; i++) {
                if (vrgZip[i].fValid != 0) {
                    rgszZip[cMax++] = vrgZip[i].szName;
                }
            }
            if (cMax > 5) {
                rgszZip[cMax++] = sz255;
            }
            rgszZip[cMax++] = PszGetCompressedString(idsCustomize);
            i = PopupMenu(hwndPlanet, pt.x, pt.y, cMax, NULL, rgszZip, -1, TRUE);
            if (i == cMax - 1) {
                memcpy(rgzo, vrgZip, 96);
                lpProc = MakeProcInstance(ZipOrderDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_ZIP_PROD), hwndFrame, lpProc);
                FreeProcInstance(lpProc);
                if (fRet == 0) {
                    memcpy(vrgZip, rgzo, 96);
                }
            } else {
                if (i > 4) {
                    i -= 4;
                    iSkip = 0;
                    while (i != 0) {
                        if (vrgZip[iSkip].fValid != 0) {
                            i--;
                            if (i == 0)
                                break;
                        }
                        iSkip++;
                    }
                    sel.fl.lpplord->rgord[sel.iwpAct].txp = vrgZip[iSkip].txp;
                } else {
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
                            lptxp->rgia[i].cQuan = 100;
                        }
                        lptxp->rgia[3].iAction = iActionNone;
                        break;
                    case 3:
                        for (i = 0; i < 5; i++) {
                            lptxp->rgia[i].iAction = iActionNone;
                            lptxp->rgia[i].cQuan = 0;
                        }
                    }
                }
                FLookupFleet(-1, &sel.fl);
                UpdateOrdersDDs(1);
                DrawPlanShip(NULL, tileStarbaseOrWaypoint);
            }
        } else {
            GlobalPD.grPopup = grPopupShipOrders;
            Popup(hwndPlanet, pt.x, pt.y);
        }
    } else if (PtInRect(&rgrcRef[12], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        if (fRightBtn != 0) {
            iChecked = -1;
            lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
            FFindNearestObject(lpord->pt, grobjPlanet | grobjFleet | grobjOther | grobjThing | mdExact, &scan);
            if (scan.idpl != -1) {
                rgid[0] = scan.idpl;
            } else {
                rgid[0] = 268435456;
            }
            rgid[1] = -1;
            c = 2;
            if (lpord->grobj == grobjPlanet || lpord->grobj == grobjOther) {
                iChecked = 0;
            }
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0)
                    break;
                if (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y && lpfl->id != sel.fl.id) {
                    if (lpord->grobj == grobjFleet && lpord->id == lpfl->id) {
                        iChecked = c;
                    }
                    rgid[c++] = lpfl->id | 0x80000000;
                    if (c >= 100)
                        break;
                }
            }
            if (c == 2) {
                c = 1;
            }
            fSep = c == 0;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (scan.pt.x == lpth->pt.x && scan.pt.y == lpth->pt.y) {
                    if (fSep == 0) {
                        if (c >= 100)
                            break;
                        rgid[c++] = -1;
                        fSep = TRUE;
                    }
                    if (c >= 100)
                        break;
                    rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;
                }
            }
            i = PopupMenu(hwndPlanet, pt.x, pt.y, c, rgid, NULL, iChecked, TRUE);
            if (i >= 0) {
                if (i == 0 && rgid[0] == 268435456) {
                    lpord->grobj = grobjOther;
                    lpord->id = -1;
                } else if ((rgid[i] & 0x20000000) != 0) {
                    lpord->grobj = grobjThing;
                    lpord->id = LOWORD(rgid[i]);
                } else if ((rgid[i] & 0x80000000) != 0) {
                    lpord->grobj = grobjFleet;
                    lpord->id = LOWORD(rgid[i]);
                } else {
                    lpord->grobj = grobjPlanet;
                    lpord->id = LOWORD(rgid[0]);
                }
                FLookupFleet(-1, &sel.fl);
                FillOrdersLB();
                SetOrdersLbSel(sel.iwpAct);
            }
        } else {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsRightClickBlueDiamondBringPopupMenu, szPopupBuffer);
            Popup(hwndPlanet, pt.x, pt.y);
        }
    } else {
        if (fRightBtn != 0) {
            return NULL;
        }
        if (PtInRect(rgrcRef, PointFrom16(pt)) != 0) {
            if (sel.grobj != grobjFleet && fCursor != 0) {
                return NULL;
            }
            irc = 0;
            lTempMax = 11;
            lMax = 11;
            lCur = sel.fl.lpplord->rgord[sel.iwpAct].iWarp;
            grbit = 6;
        } else if (PtInRect(&rgrcRef[15], PointFrom16(pt)) != 0) {
            irc = 15;
            iWarp = IWarpMAFromLppl(&sel.pl, &fTwoMAs);
            lTempMax = (int16_t)(iWarp - 1);
            lMax = (int16_t)(iWarp - 1);
            lTempMin = 1;
            lCur = sel.pl.iWarpFling;
        } else if (PtInRect(&rgrcRef[1], PointFrom16(pt)) != 0) {
            if (sel.grobj == grobjFleet) {
                idPlan = sel.fl.idPlanet;
                iSkip = sel.fl.id;
            } else {
                iSkip = -1;
                idPlan = sel.pl.id;
            }
            lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
            FLookupOrbitingXfer(idPlan, LOWORD(lSel), &xf, iSkip);
            if (xf.grobj != grobjFleet || xf.fl.iPlayer != idPlayer) {
                return NULL;
            }
            irc = 1;
            lMax = LGetFleetStat(&xf.fl, 1);
            lCur = xf.fl.rgwtMin[4];
            grbit = 4;
            if (sel.grobj == grobjFleet) {
                lTempMax = lCur + sel.fl.rgwtMin[4];
                lTempMin = lCur - (LGetFleetStat(&sel.fl, 1) - sel.fl.rgwtMin[4]);
            } else {
                lTempMax = lCur;
            }
        } else {
            if (PtInRect(&rgrcRef[3], PointFrom16(pt)) != 0) {
                if (fCursor != 0) {
                    return hcurHand;
                }
                if (sel.fl.idPlanet != -1) {
                    TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
                } else {
                    lpth = lpThings;
                    lpthMac = lpThings + cThing;
                    for (; lpth < lpthMac && (lpth->ith != ithMineralPacket || sel.fl.pt.x != lpth->pt.x || sel.fl.pt.y != lpth->pt.y); lpth++) {
                    }
                    if (lpth == lpthMac) {
                        TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
                    } else {
                        MessageBeep(MB_OK);
                    }
                }
                return NULL;
            }
            if (PtInRect(&rgrcRef[4], PointFrom16(pt)) != 0) {
                if (fCursor != 0) {
                    return hcurHand;
                }
                lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
                if (lSel == -1) {
                    return NULL;
                }
                if (FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.grobj == grobjFleet ? sel.fl.id : -1) != 0) {
                    TransferStuff(sel.id, sel.grobj, xf.id, xf.grobj, mdXferCargo);
                }
                return NULL;
            }
            if (PtInRect(&rgrcRef[18], PointFrom16(pt)) != 0) {
                if ((sel.grobj != grobjFleet && fCursor != 0) || sel.fl.lpplord->rgord[sel.iwpAct].grTask != grTaskPatrol) {
                    return NULL;
                }
                irc = 18;
                lTempMax = 10;
                lMax = 10;
                lCur = (uint32_t)sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
                grbit = 7;
            }
        }
    }
L_8b82:
    if (irc == -1) {
        return NULL;
    }
    if (fCursor != 0) {
        return hcurHand;
    }
    dx = (int16_t)(rgrcRef[irc].right - rgrcRef[irc].left - 2);
    xRnd = (int32_t)(dx / (lMax + 1)) >> 1;
    hdc = GetDC(hwndPlanet);
    SetCapture(hwndPlanet);
    ptOld.y = -1;
    ptOld.x = -1;
    lTempMax = lMax < lTempMax ? lMax : lTempMax;
    lTempMin = 0 <= lTempMin ? lTempMin : 0;
    fFirst = TRUE;
    while (fFirst != 0 || FGetMouseMove(&pt) != 0) {
        fFirst = FALSE;
        if (pt.x != ptOld.x || pt.y != ptOld.y) {
            ptOld = pt;
            lNew = (int32_t)((int32_t)(((int16_t)(pt.x - rgrcRef[irc].left) + xRnd) * lMax) / dx);
            if (lTempMin > (lNew < lTempMax ? lNew : lTempMax)) {
                lNew = lTempMin;
            } else if (lNew >= lTempMax) {
                lNew = lTempMax;
            }
            if (lNew != lCur) {
                switch (irc) {
                case 0:
                    sel.fl.lpplord->rgord[sel.iwpAct].iWarp = LOWORD(lNew);
                    DrawPlanShip(NULL, tileFleetOrders | tileMinimized);
                    break;
                case 18:
                    sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = LOWORD(lNew);
                    DrawPlanShip(NULL, tileStarbaseOrWaypoint | tileMinimized);
                    break;
                case 15:
                    DrawMassWarpGauge(hdc, &rgrcRef[15], fTwoMAs == 0 ? iWarp : -iWarp, LOWORD(lNew) + 4);
                    break;
                case 2:
                    sel.fl.rgwtMin[4] = lNew;
                    DrawFleetGauge(hdc, &rgrcRef[irc], NULL, grbit);
                    break;
                case 1:
                    if (sel.grobj == grobjFleet) {
                        sel.fl.rgwtMin[4] -= lNew - lCur;
                        DrawFleetGauge(hdc, &rgrcRef[2], &sel.fl, grbit);
                    } else {
                        DrawPlanShip(NULL, tileMineralsOrCargo | tileMinimized);
                    }
                    xf.fl.rgwtMin[4] = lNew;
                    DrawFleetGauge(hdc, &rgrcRef[irc], &xf.fl, grbit);
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
        DrawPlanShip(NULL, tileFleetOrders | tileFleetComp | tileMinimized);
        if ((grbit & 1) != 0 && sel.fl.idPlanet == sel.scan.idpl)
            goto FixMinWin;
        if ((grbit & 2) == 0 || sel.fl.id != rglpfl[sel.scan.ifl]->id)
            break;
        InvalidateRect(hwndMine, NULL, TRUE);
        break;
    case 1:
        FLookupFleet(-1, &xf.fl);
        if (sel.grobj == grobjFleet) {
            FLookupFleet(-1, &sel.fl);
            DrawPlanShip(NULL, tileMineralsOrCargo | tileFleetOrders | tileFleetComp | tileMinimized);
            break;
        }
        FLookupPlanet(-1, &sel.pl);
        if ((grbit & 1) != 0 && sel.pl.id == sel.scan.idpl)
            goto FixMinWin;
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
    }
    goto L_914e;
FixMinWin:
    InvalidateMineralBars();
L_914e:
    ReleaseCapture();
    return (HCURSOR)(uintptr_t)ReleaseDC(hwndPlanet, hdc);
}

void FillFleetCompLB() {
    int16_t i;
    int32_t pctDmg;

    SendMessage(hwndFleetCompLB, LB_RESETCONTENT, 0, 0);
    for (i = 0; i < 16; i++) {
        if (sel.fl.rgcsh[i] > 0) {
            pctDmg = (int32_t)((uint32_t)(sel.fl.rgdv[i].pctSh * sel.fl.rgdv[i].pctDp) + 250) / 500;
            _wsprintf(szWork, "%c%c%5d%s", pctDmg == 0 ? 81 : 80, pctDmg == 0 ? 32 : (int16_t)(int8_t)LOBYTE(LOWORD(pctDmg)), sel.fl.rgcsh[i],
                      rgshdef[i].hul.szClass);
            SendMessage(hwndFleetCompLB, LB_ADDSTRING, 0, (LPARAM)szWork);
        }
    }
    return;
}

void FillOrdersLB() {
    int16_t i;
    char   *psz;
    ORDER   ord;

    SendMessage(hwndShipLB, LB_RESETCONTENT, 0, 0);
    for (i = 0; i < sel.fl.cord; i++) {
        ord = sel.fl.lpplord->rgord[i];
        psz = PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
        SendMessage(hwndShipLB, LB_ADDSTRING, 0, (LPARAM)psz);
    }
    SetOrdersLbSel(sel.iwpAct);
    if (sel.grobj == grobjFleet) {
        DrawPlanShip(NULL, tileFleetOrders | tileStarbaseOrWaypoint);
    }
    return;
}

void SetOrdersLbSel(int16_t iSel) {
    SendMessage(hwndShipLB, LB_SETCURSEL, iSel, 0);
    if (iSel > (gd.fSmallTileMode == 0 ? 2 : 1)) {
        SendMessage(hwndShipLB, LB_SETTOPINDEX, iSel - (gd.fSmallTileMode == 0 ? 2 : 1), 0);
    }
    UpdateWindow(hwndShipLB);
    UpdateOrdersDDs(0);
    return;
}

void UpdateOrdersDDs(int16_t iLevel) {
    int32_t rglSel[3];
    int16_t iMin;
    int16_t i;
    char   *psz;
    int16_t iSel;
    int16_t iMax;
    char    szT[80];

    iSel = -1;
    if (iLevel == 0) {
        rglSel[0] = SendMessage(rghwndOrderDD[0], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].grTask, 0);
    } else {
        rglSel[0] = SendMessage(rghwndOrderDD[0], CB_GETCURSEL, 0, 0);
    }
    if (iLevel > 1) {
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0, 0);
        if (rglSel[0] != 1 || iLevel > 3)
            goto L_987f;
        iSel = LOWORD(rglSel[1]);
        if (iSel == 0) {
            iSel = 4;
        } else {
            iSel--;
        }
    }
    SendMessage(rghwndOrderDD[1], CB_RESETCONTENT, 0, 0);
    switch (rglSel[0]) {
    case 1:
        iMax = LGetFleetStat(&sel.fl, 2) == 0 ? 1 : 5;
        for (i = 0; i < iMax; i++) {
            if (i == 0) {
                iMin = 4;
            } else {
                iMin = i - 1;
            }
            strcpy(&szWork[1], rgszMinerals[iMin]);
            if (sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iMin].iAction != iActionNone) {
                szWork[0] = '*';
                if (iSel == -1) {
                    iSel = iMin;
                }
            } else {
                szWork[0] = ' ';
            }
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)szWork);
        }
        if (iSel == -1 || iSel == 4) {
            iSel = 0;
        } else {
            iSel++;
        }
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 7:
        psz = PszGetCompressedString(idsWithinDLY);
        for (i = 0; i < 11; i++) {
            _wsprintf(szWork, psz, 50 * i + 50);
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)szWork);
        }
        psz = PszGetCompressedString(idsAnyEnemy);
        SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)psz);
        iSel = sel.fl.lpplord->rgord[sel.iwpAct].tptl.iDist;
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 9:
        szT[0] = ' ';
        for (i = 0; i < game.cPlayer; i++) {
            if (i != idPlayer) {
                psz = PszPlayerName(i, TRUE, TRUE, TRUE, 0, NULL);
                strcpy(&szT[1], psz);
                SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)szT);
            }
        }
        iSel = sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, iSel, 0);
        break;
    case 6:
        for (i = 0; i < 5; i++) {
            _wsprintf(szWork, PszGetCompressedString(idsDYearC), i + 1, i == 0 ? 32 : 115);
            SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)szWork);
        }
        SendMessage(rghwndOrderDD[1], CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsIindefinitely));
        rglSel[1] = SendMessage(rghwndOrderDD[1], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX, 0);
    }
L_987f:
    if (iLevel <= 2) {
        SendMessage(rghwndOrderDD[2], CB_RESETCONTENT, 0, 0);
        if (rglSel[0] == 1) {
            for (i = 109; i < 119; i++) {
                if (i == 116 && rglSel[1] == 0) {
                    psz = PszGetCompressedString(idsLoadOptimal);
                } else {
                    psz = PszGetCompressedString(i);
                }
                SendMessage(rghwndOrderDD[2], CB_ADDSTRING, 0, (LPARAM)psz);
            }
            iSel = LOWORD(rglSel[1]);
            if (iSel == 0) {
                iSel = 4;
            } else {
                iSel--;
            }
            rglSel[2] = SendMessage(rghwndOrderDD[2], CB_SETCURSEL, sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iSel].iAction, 0);
        }
    } else {
        rglSel[2] = SendMessage(rghwndOrderDD[2], CB_GETCURSEL, 0, 0);
    }
    if (iLevel <= 3 && rglSel[0] == 1) {
        iSel = LOWORD(rglSel[1]);
        if (iSel == 0) {
            iSel = 4;
        } else {
            iSel--;
        }
        _wsprintf(szWork, "%u", sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia[iSel].cQuan);
        SetWindowText(hwndOrderED, szWork);
    }
    return;
}

void FillBattleDD(int16_t iSel) {
    int16_t i;

    SendMessage(hwndBattleDD, CB_RESETCONTENT, 0, 0);
    CchGetString(idsBattlePlans, szWork);
    SendMessage(hwndBattleDD, CB_ADDSTRING, 0, (LPARAM)szWork);
    for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
        fstrcpy(szWork, rglpbtlplan[idPlayer][i].szName);
        SendMessage(hwndBattleDD, CB_ADDSTRING, 0, (LPARAM)szWork);
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

    if (sel.fl.cord < 2 || sel.iwpAct == 0) {
        MessageBeep(MB_ICONASTERISK);
    } else {
        if ((grbitScan & grbitScanFleetPaths) != 0) {
            rgpt[0] = sel.fl.lpplord->rgord[sel.iwpAct].pt;
            rgpt[1] = sel.fl.lpplord->rgord[sel.iwpAct - 1].pt;
            if (sel.iwpAct < sel.fl.cord - 1) {
                cpt = 3;
                rgpt[2] = sel.fl.lpplord->rgord[sel.iwpAct + 1].pt;
            } else {
                cpt = 2;
            }
        }
        RedrawScanSel(NULL, 0);
        fmemmove(&sel.fl.lpplord->rgord[sel.iwpAct], &sel.fl.lpplord->rgord[sel.iwpAct + 1], (sel.fl.cord - sel.iwpAct - 1) * sizeof(ORDER));
        sel.fl.cord--;
        sel.fl.lpplord->iordMac--;
        sel.iwpAct--;
        if (sel.iwpAct < sel.fl.cord - 1) {
            pt = sel.fl.lpplord->rgord[sel.iwpAct].pt;
            if (pt.x == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.x && pt.y == sel.fl.lpplord->rgord[sel.iwpAct + 1].pt.y) {
                fmemmove(&sel.fl.lpplord->rgord[sel.iwpAct + 1], &sel.fl.lpplord->rgord[sel.iwpAct + 2], (sel.fl.cord - sel.iwpAct - 2) * sizeof(ORDER));
                sel.fl.cord--;
                sel.fl.lpplord->iordMac--;
            }
        }
        if (fBackup == 0 && sel.iwpAct < sel.fl.cord - 1) {
            sel.iwpAct++;
        }
        RedrawScanSel(NULL, 0);
        FLookupFleet(-1, &sel.fl);
        FFindNearestObject(sel.fl.lpplord->rgord[sel.iwpAct].pt, grobjPlanet | grobjFleet | grobjOther | grobjThing | mdExact, &scan);
        sel.iwpAct = -2;
        ChangeScanSel(&scan, 1);
        if ((grbitScan & grbitScanFleetPaths) != 0) {
            for (ipt = 0; ipt < cpt; ipt++) {
                LogicalToScan(&rgpt[ipt]);
            }
            BoundPoints(&rc, rgpt, cpt);
            InvalidateRect(hwndScanner, &rc, TRUE);
        }
    }
    return;
}

void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle) {
    ORDER ord;

    if (fRecycle != 0) {
        if (iDel == 86 || lpfl->cord == 2 ||
            (lpfl->lpplord->rgord[lpfl->cord - 1].pt.x == lpfl->lpplord->rgord[iDel].pt.x &&
             lpfl->lpplord->rgord[lpfl->cord - 1].pt.y == lpfl->lpplord->rgord[iDel].pt.y)) {
            fRecycle = FALSE;
        } else {
            ord = lpfl->lpplord->rgord[iDel];
        }
    }
    fmemmove(&lpfl->lpplord->rgord[iDel], &lpfl->lpplord->rgord[iDel + 1], (lpfl->cord - iDel - 1) * sizeof(ORDER));
    if (fRecycle != 0) {
        lpfl->lpplord->rgord[lpfl->cord - 1] = ord;
    } else {
        lpfl->cord--;
        lpfl->lpplord->iordMac--;
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
    int16_t  t_call_a40a;

    iEffCur = 0;
    gd.fRadiatingEngine = FALSE;
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
                if (t_scratch_m76 >= LphuldefFromId(lpshdef->hul.ihuldef)->hul.rghs[j].cItem) {
                    rgieff[i] = LpengineFromId(lpshdef->hul.rghs[j].iItem)->rgcFuelUsed[iWarp];
                    if (fEfficient != 0) {
                        rgieff[i] -= (int32_t)(rgieff[i] * 15) / 100;
                    }
                    if (lpshdef->hul.rghs[j].iItem != 10)
                        goto L_a07d;
                    gd.fRadiatingEngine = TRUE;
                    goto L_a07d;
                }
            }
            rgieff[i] = 99999;
        }
    L_a07d:
        i++;
        lpshdef++;
    }
    wtCargo = 0;
    for (i = 0; i <= 3; i++) {
        wtCargo += lpfl->rgwtMin[i];
    }
    if (dTravel == -1) {
        if (fRangeOnly != 0) {
            dTravel = 1000;
        } else {
            lpord = &lpfl->lpplord->rgord[iOrd];
            d = DGetDistance(lpord->pt.x, lpord->pt.y, lpord[1].pt.x, lpord[1].pt.y);
            dTravel = (int16_t)LOWORD((int32_t)((long double)d + 0.9999));
        }
    }
    lFuel = 0;
    while (1) {
        iEffNext = 999999;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                if (rgieff[i] == iEffCur) {
                    if (wtCargo < (int32_t)(uint32_t)(lpfl->rgcsh[i] * WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, 2))) {
                        wtCargoT = wtCargo;
                    } else {
                        t_call_a40a = WtMaxShdefStat(rglpshdef[lpfl->iPlayer] + i, 2);
                        wtCargoT = (uint32_t)(lpfl->rgcsh[i] * (int16_t)t_call_a40a);
                    }
                    wtCargo -= wtCargoT;
                    if (rgieff[i] > 0) {
                        wtMass = (uint32_t)(lpfl->rgcsh[i] * (uint32_t)rglpshdef[lpfl->iPlayer][i].hul.wtEmpty) + wtCargoT;
                        lT = (uint32_t)(iEffCur * dTravel);
                        if (wtMass < 200 || (lT < 500000 && wtMass < 4000) || (lT < 100000 && wtMass < 20000)) {
                            lFuel += (int32_t)(wtMass * lT) / 2000;
                        } else {
                            lFuel = (int32_t)((long double)lT * wtMass / 2000.0) + lFuel;
                        }
                    }
                } else if (rgieff[i] > iEffCur && rgieff[i] < iEffNext) {
                    iEffNext = rgieff[i];
                }
            }
        }
        if (iEffNext == 999999)
            break;
        iEffCur = iEffNext;
    }
    if (fRangeOnly == 0) {
        lFuel += 9;
    }
    lFuel = (int32_t)(lFuel / 10);
    if (fRangeOnly != 0) {
        if (lFuel == 0) {
            lFuel = 1000000000;
        } else if (lFuel > 100000) {
            lFuel = (int32_t)(lpfl->rgwtMin[4] / (int32_t)(lFuel / 1000));
        } else {
            lFuel = (int32_t)((int32_t)(lpfl->rgwtMin[4] * 1000) / lFuel);
        }
    }
    return lFuel;
}

LRESULT CALLBACK FakeEditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg != WM_CHAR || ((wParam >= '0' && wParam <= '9') || wParam == 8)) {
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
    if (lpfl == 0) {
        lpfl = &sel.fl;
    }
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            for (j = 0; j < rglpshdef[lpfl->iPlayer][i].hul.chs && rglpshdef[lpfl->iPlayer][i].hul.rghs[j].grhst != hstEngine; j++) {
            }
            if (j == rglpshdef[lpfl->iPlayer][i].hul.chs) {
                iWorst = 0;
                break;
            }
            j = rglpshdef[lpfl->iPlayer][i].hul.rghs[j].iItem;
            lpengine = LpengineFromId(j);
            for (; iWorst > 0; iWorst--) {
                if (lpengine->rgcFuelUsed[iWorst] <= 120) {
                    if (lpengine->rgcFuelUsed[iWorst] > 0 && fIgnoreScoops == 0 && j != 14 && j != 15) {
                        if (iWorst >= 5 && lpengine->rgcFuelUsed[iWorst - 1] == 0) {
                            iWorst--;
                        } else if (iWorst >= 6 && lpengine->rgcFuelUsed[iWorst - 2] == 0) {
                            iWorst -= 2;
                        } else if (iWorst >= 7 && lpengine->rgcFuelUsed[iWorst - 3] == 0) {
                            iWorst -= 3;
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
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
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

    lTot = 0;
    lCur = 0;
    lpord = lpfl->lpplord->rgord;
    for (i = 0; i < iwp; i++) {
        iWarp = lpord[i + 1].iWarp;
        if (iWarp > 0 && iWarp < 11) {
            dbl = (double)((long double)DGetDistance(lpord[i].pt.x, lpord[i].pt.y, lpord[i + 1].pt.x, lpord[i + 1].pt.y) + 0.99999);
            dist = LOWORD((int32_t)dbl);
            dbl = (double)((long double)dbl / iWarp / iWarp);
            cYears = LOWORD((int32_t)((long double)dbl + 0.9999));
            l = EstFuelUse(lpfl, i, iWarp, -1, FALSE);
        } else {
            cYears = 1;
            l = 0;
        }
        if (cYears > 1) {
            lOneYearUse = EstFuelUse(lpfl, i, iWarp, (int16_t)(iWarp * iWarp), FALSE);
            lFuelGain = (uint32_t)(lOneYearUse * (int16_t)(cYears - 1));
            lFuelGain += EstFuelUse(lpfl, i, iWarp, (int16_t)(dist - iWarp * iWarp * (cYears - 1)), FALSE);
            if (lFuelGain > l) {
                l = lFuelGain;
            }
            lFuelGain = LCalcFuelGainFromRamScoops(lpfl, iWarp, (int16_t)(iWarp * iWarp));
            for (j = 0; j < 16; j++) {
                if (lpfl->rgcsh[j] != 0) {
                    lpshdef = rglpshdef[idPlayer] + j;
                    if (lpshdef->hul.ihuldef == ihuldefFuelTransport || lpshdef->hul.ihuldef == ihuldefSuperFuelXport) {
                        lFuelGain += (uint32_t)(lpfl->rgcsh[j] * 200);
                    }
                }
            }
            if (lFuelGain > 0) {
                if (lOneYearUse <= lFuelGain) {
                    l = lOneYearUse;
                } else {
                    lOneYearUse = (uint32_t)((lOneYearUse - lFuelGain) * (int16_t)(cYears - 1)) + lOneYearUse;
                    if (lOneYearUse < l) {
                        l = lOneYearUse;
                    }
                }
            }
        }
        lCur += l;
        if (lCur > lTot) {
            lTot = lCur;
        }
        if (lpord[i + 1].grobj == grobjPlanet) {
            lppl = LpplFromId(lpord[i + 1].id);
            if (lppl != 0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0 &&
                LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0) {
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
    uint16_t t_merge_c236_0001;

    fDeadFleet = FALSE;
    rgpflNew[0] = pflNew1;
    rgpflNew[1] = pflNew2;
    iplr = pflNew1->iPlayer;
    for (i = 0; i < 2; i++) {
        if (rgpflNew[i]->fDead != 0) {
            fDeadFleet = TRUE;
            memset(&rgflCur[i], 0, sizeof(FLEET));
            rgflCur[i].iPlayer = iplr;
        } else {
            FLookupFleet(rgpflNew[i]->id, &rgflCur[i]);
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
                    rgFuelCapacity[i] += (uint32_t)(rgflCur[i].rgcsh[ishdef] * wtFuelMax);
                    rgCargoCapacity[i] += (uint32_t)(rgflCur[i].rgcsh[ishdef] * wtCargoMax);
                    if (rgrgcshLoss[i][ishdef] > 0) {
                        rgFuelCapLoss[i] += (uint32_t)(rgrgcshLoss[i][ishdef] * wtFuelMax);
                        rgCargoCapLoss[i] += (uint32_t)(rgrgcshLoss[i][ishdef] * wtCargoMax);
                    }
                }
            }
            if (fDeadFleet == 0 && rgrgcshLoss[0][ishdef] == -rgrgcshLoss[1][ishdef] && rgrgcshLoss[0][ishdef] != 0) {
                iSrc = rgrgcshLoss[0][ishdef] < 0;
                if (rgflCur[iSrc].rgcsh[ishdef] > 0) {
                    cshDmgSrc = (int32_t)(rgflCur[iSrc].rgdv[ishdef].pctSh * rgflCur[iSrc].rgcsh[ishdef]) / 100;
                } else {
                    cshDmgSrc = 0;
                }
                if (rgflCur[iSrc == 0].rgcsh[ishdef] > 0) {
                    cshDmgDst = (int32_t)(rgflCur[iSrc == 0].rgdv[ishdef].pctSh * rgflCur[iSrc == 0].rgcsh[ishdef]) / 100;
                } else {
                    cshDmgDst = 0;
                }
                if (cshDmgSrc != 0 && cshDmgDst != 0) {
                    if (cshDmgSrc > rgrgcshLoss[iSrc][ishdef]) {
                        cshDmgMoved = rgrgcshLoss[iSrc][ishdef];
                    } else {
                        cshDmgMoved = cshDmgSrc;
                    }
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgDst * rgflCur[iSrc == 0].rgdv[ishdef].pctDp) +
                                                 (uint32_t)(cshDmgMoved * rgflCur[iSrc].rgdv[ishdef].pctDp) + rgpflNew[iSrc == 0]->rgcsh[ishdef] - 1) /
                                       rgpflNew[iSrc == 0]->rgcsh[ishdef]);
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = rgpflNew[iSrc == 0]->rgdv[ishdef].pctSh | (LOWORD(pctNew) & 0x1ff) * 0x80;
                    pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgDst + cshDmgMoved) * 100) + rgpflNew[iSrc == 0]->rgcsh[ishdef] - 1) /
                                       rgpflNew[iSrc == 0]->rgcsh[ishdef]);
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = (rgpflNew[iSrc == 0]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                    if (cshDmgMoved == cshDmgSrc) {
                        rgpflNew[iSrc]->rgdv[ishdef].dp = 0;
                    } else {
                        pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgSrc - cshDmgMoved) * 100) + rgpflNew[iSrc]->rgcsh[ishdef] - 1) /
                                           rgpflNew[iSrc]->rgcsh[ishdef]);
                        rgpflNew[iSrc]->rgdv[ishdef].pctSh = LOWORD(pctNew);
                    }
                } else if (cshDmgSrc != 0) {
                    if (cshDmgSrc > rgrgcshLoss[iSrc][ishdef]) {
                        cshDmgMoved = rgrgcshLoss[iSrc][ishdef];
                    } else {
                        cshDmgMoved = cshDmgSrc;
                    }
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = rgpflNew[iSrc == 0]->rgdv[ishdef].pctSh | (rgpflNew[iSrc]->rgdv[ishdef].pctDp & 0x1ff) * 0x80;
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgMoved * 100) + rgpflNew[iSrc == 0]->rgcsh[ishdef] - 1) / rgpflNew[iSrc == 0]->rgcsh[ishdef]);
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = (rgpflNew[iSrc == 0]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                    if (cshDmgMoved == cshDmgSrc) {
                        rgpflNew[iSrc]->rgdv[ishdef].dp = 0;
                    } else {
                        pctNew = (int32_t)((int32_t)((uint32_t)((cshDmgSrc - cshDmgMoved) * 100) + rgpflNew[iSrc]->rgcsh[ishdef] - 1) /
                                           rgpflNew[iSrc]->rgcsh[ishdef]);
                        rgpflNew[iSrc]->rgdv[ishdef].pctSh = LOWORD(pctNew);
                    }
                } else if (cshDmgDst != 0) {
                    pctNew = (int32_t)((int32_t)((uint32_t)(cshDmgDst * 100) + rgpflNew[iSrc == 0]->rgcsh[ishdef] - 1) / rgpflNew[iSrc == 0]->rgcsh[ishdef]);
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = (rgpflNew[iSrc == 0]->rgdv[ishdef].dp & 0xff80) | (LOWORD(pctNew) & 0x7f);
                } else {
                    rgpflNew[iSrc == 0]->rgdv[ishdef].dp = rgpflNew[iSrc == 0]->rgdv[ishdef].dp & 0xff80;
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (rgFuelCapacity[i] != 0) {
            if (rgpflNew[i]->rgwtMin[4] > 45000 || rgFuelCapLoss[i] > 45000) {
                lChg = (int32_t)((long double)rgpflNew[i]->rgwtMin[4] * rgFuelCapLoss[i] / rgFuelCapacity[i]);
            } else {
                lChg = (int32_t)((int32_t)(rgpflNew[i]->rgwtMin[4] * rgFuelCapLoss[i]) / rgFuelCapacity[i]);
            }
            rgrgCargoDelta[i][4] -= lChg;
        }
        if (rgCargoCapacity[i] != 0) {
            wtCargoTot = 0;
            for (j = 0; j <= 3; j++) {
                wtCargoTot += rgpflNew[i]->rgwtMin[j];
            }
            if (wtCargoTot > 45000 || rgCargoCapLoss[i] > 45000) {
                wtCargoXfer = (int32_t)((long double)wtCargoTot * rgCargoCapLoss[i] / rgCargoCapacity[i]);
            } else {
                wtCargoXfer = (int32_t)((int32_t)(wtCargoTot * rgCargoCapLoss[i]) / rgCargoCapacity[i]);
            }
            lChg = wtCargoXfer;
            if (wtCargoXfer != 0 && wtCargoTot != 0) {
                for (j = 0; j <= 3; j++) {
                    if (rgpflNew[i]->rgwtMin[j] > 45000 || wtCargoXfer > 45000) {
                        l = (int32_t)((long double)rgpflNew[i]->rgwtMin[j] * wtCargoXfer / wtCargoTot);
                    } else {
                        l = (int32_t)((int32_t)(rgpflNew[i]->rgwtMin[j] * wtCargoXfer) / wtCargoTot);
                    }
                    l = l < lChg ? l : lChg;
                    rgrgCargoDelta[i][j] -= l;
                    lChg -= l;
                }
                if (lChg > 0) {
                    for (j = 0; j <= 3 && lChg > 0; j++) {
                        if (rgpflNew[i]->rgwtMin[j] + rgrgCargoDelta[i][j] > 0) {
                            rgrgCargoDelta[i][j]--;
                            lChg--;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            rgpflNew[i]->rgwtMin[j] += rgrgCargoDelta[i][j];
            t_merge_c236_0001 = i == 0;
            rgpflNew[t_merge_c236_0001]->rgwtMin[j] -= rgrgCargoDelta[i][j];
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
        if (lppl->iPlayer == iplr && lppl->fStarbase != 0 && lppl->isb == ishdefSB) {
            lppl->fStarbase = FALSE;
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
    if (ishdef >= 16) {
        DestroyAllIshdefSB(ishdef - 16, iplr);
        InvalidateReport(rptPlanets, 1);
    } else {
        lpfl = *rglpfl;
        i = 0;
        while (i < cFleet) {
            if (lpfl->iPlayer == iplr && lpfl->rgcsh[ishdef] > 0) {
                memset(&flDead, 0, sizeof(FLEET));
                cKill = lpfl->rgcsh[ishdef];
                cDel += cKill;
                for (j = 0; j < 16 && (j == ishdef || lpfl->rgcsh[j] == 0); j++) {
                }
                if (j == 16) {
                    lpfl->rgcsh[ishdef] = 0;
                    FDeleteFleet(lpfl->id, grobjNone, -1);
                    goto L_c392;
                }
                flDead.iplr = iplr;
                flDead.fDead = TRUE;
                flDead.rgcsh[ishdef] = cKill;
                flNew = *lpfl;
                flNew.rgcsh[ishdef] = 0;
                FleetTransferCargoBalance(&flNew, &flDead);
                *lpfl = flNew;
                if (sel.grobj == grobjFleet && sel.fl.id == flNew.id) {
                    FLookupFleet(flNew.id, &sel.fl);
                    RedrawScanSel(NULL, 0);
                    FillShipDD(sel.fl.id);
                    grbit = -31819;
                    FLookupFleet(sel.fl.id, &sel.fl);
                    FillFleetCompLB();
                    DrawPlanShip(NULL, grbit);
                    InvalidateRect(hwndMine, NULL, TRUE);
                }
            }
            i++;
        L_c392:
            lpfl = rglpfl[i];
        }
        InvalidateReport(rptFleets, 1);
    }
    RemoveIshdefFromAllQueues(ishdef, FALSE);
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
        if (lppl->lpplprod != 0 && lppl->lpplprod->iprodMac != 0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0 &&
            (fSpaceDocks == 0 || rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef == ihuldefSpaceDock)) {
            iDst = 0;
            iprod = 0;
            lpprod = lppl->lpplprod->rgprod;
            while (iprod < lppl->lpplprod->iprodMac) {
                if (lpprod->grobj != grobjFleet || lpprod->iItem != (uint32_t)ishdef) {
                    if (iDst != iprod) {
                        lppl->lpplprod->rgprod[iDst] = *lpprod;
                    }
                    iDst++;
                }
                iprod++;
                lpprod++;
            }
            if (iDst == 0) {
                FreePl((PL *)lppl->lpplprod);
                lppl->lpplprod = NULL;
            } else if (iDst != iprod) {
                lppl->lpplprod->iprodMac = iDst;
            }
        }
    }
    if (sel.grobj == grobjPlanet && sel.pl.lpplprod != 0) {
        FLookupPlanet(sel.pl.id, &sel.pl);
        FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, NULL);
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
        if (lppl->lpplprod != 0 && lppl->lpplprod->iprodMac != 0 && lppl->iPlayer == idPlayer && lppl->fStarbase != 0 &&
            (fSpaceDocks == 0 || rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef == ihuldefSpaceDock)) {
            iprod = 0;
            lpprod = lppl->lpplprod->rgprod;
            while (iprod < lppl->lpplprod->iprodMac) {
                if (lpprod->grobj == grobjFleet && lpprod->iItem == (uint32_t)ishdef) {
                    csh += lpprod->cItem;
                    if (lpprod->pct != 0) {
                        *pfProgress = 1;
                    }
                }
                iprod++;
                lpprod++;
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
        rgfl[0].rgcsh[i] += rgfl[1].rgcsh[i];
        rgfl[1].rgcsh[i] = 0;
    }
    FleetTransferCargoBalance(rgfl, &rgfl[1]);
    for (i = 0; i < 2; i++) {
        FLookupFleet(-1, &rgfl[i]);
    }
    if (fNoDelete != 0) {
        lpflDel->fDead = TRUE;
    } else {
        FDeleteFleet(rgfl[1].id, grobjFleet, rgfl[0].id);
        InvalidateReport(rptFleets, 2);
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

    fChg = FALSE;
    for (iflMac = 0; iflMac < cFleet; iflMac++) {
        lpfl = rglpfl[iflMac];
        if (rglpfl[iflMac] == 0)
            break;
        if (lpfl->lpplord != 0) {
            for (iord = lpfl->cord - 1; iord >= 0; iord--) {
                if (lpfl->lpplord->rgord[iord].grobj == grobjFleet && lpfl->lpplord->rgord[iord].id == lpflOld->id) {
                    if (fChg == 0) {
                        pt = lpflOld->pt;
                        lpflOld->pt.x++;
                        if (FFindNearestObject(pt, grobjPlanet | grobjFleet | mdExact, &scan) == 0) {
                            grobj = grobjOther;
                            id = iord;
                        } else if ((scan.grobjFull & grobjFleet) != 0) {
                            grobj = grobjFleet;
                            id = rglpfl[scan.ifl]->id;
                        } else {
                            grobj = grobjPlanet;
                            id = scan.idpl;
                        }
                        lpflOld->pt.x--;
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
        if ((ppart->hs.grhst & hstTerra) == 0 && ((ppart->hs.grhst & hstPlanetary) == 0 || ppart->hs.iItem < 9 || ppart->hs.iItem > 13) &&
            ((ppart->hs.grhst & hstPlanetary) == 0 || ppart->hs.iItem < 0 || ppart->hs.iItem > 8)) {
            cExcess = 100;
            for (i = 0; i < 6; i++) {
                cCur = rgplr[iPlayer].rgTech[i] - lpcom->rgTech[i];
                if (lpcom->rgTech[i] > 0 && cCur < cExcess) {
                    cExcess = cCur;
                }
            }
            if (cExcess == 100) {
                for (i = 0; i < 6; i++) {
                    if (rgplr[iPlayer].rgTech[i] < cExcess) {
                        cExcess = rgplr[iPlayer].rgTech[i];
                    }
                }
            }
            if (cExcess >= 1) {
                if (cExcess > 19) {
                    cExcess = 19;
                }
                if (GetRaceGrbit(&rgplr[iPlayer], ibitRaceBleedingEdgeTech) != 0) {
                    cExcess = 5 * cExcess;
                    if (cExcess > 80) {
                        cExcess = 80;
                    }
                } else {
                    cExcess *= 4;
                    if (cExcess > 75) {
                        cExcess = 75;
                    }
                }
                for (i = 0; i < 4; i++) {
                    if (rgCost[i] > 0) {
                        rgCost[i] -= MulDiv(rgCost[i], cExcess, 100);
                        if (rgCost[i] == 0) {
                            rgCost[i] = 1;
                        }
                    }
                }
            }
        }
        if (ppart->hs.grhst == hstSpecialSB && GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raStargate &&
            GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raStargate && ppart->hs.iItem >= ispecialSBStargate100250 &&
            ppart->hs.iItem <= ispecialSBStargateAnyAny) {
            for (i = 0; i < 4; i++) {
                rgCost[i] -= rgCost[i] >> 2;
            }
        } else {
            switch (ppart->hs.grhst) {
            case hstBeam:
            case hstTorp:
            case hstBomb:
                if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raAttack) {
                    for (i = 0; i < 4; i++) {
                        rgCost[i] -= rgCost[i] >> 2;
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
                            rgCost[i] += rgCost[i] >> 2;
                        }
                        break;
                    }
                default:
                    if (ppart->hs.grhst == hstTerra && GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raTerra) {
                        rgCost[3] = (uint32_t)rgCost[3] / 2;
                    } else if (ppart->hs.grhst == hstEngine && GetRaceGrbit(&rgplr[iPlayer], ibitRaceCheapEngines) != 0) {
                        for (i = 0; i < 4; i++) {
                            rgCost[i] -= rgCost[i] >> 1;
                        }
                    }
                }
            }
        }
        if (cExcess < 1 && GetRaceGrbit(&rgplr[iPlayer], ibitRaceBleedingEdgeTech) != 0 && gd.fDontCalcBleed == 0) {
            for (i = 0; i < 6 && lpcom->rgTech[i] <= 0; i++) {
            }
            if (i < 6) {
                gd.fBleedingEdge = TRUE;
                for (i = 0; i < 4; i++) {
                    rgCost[i] *= 2;
                }
            } else {
                gd.fBleedingEdge = FALSE;
            }
        } else {
            gd.fBleedingEdge = FALSE;
        }
    }
    return;
}
