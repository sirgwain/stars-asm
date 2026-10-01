#include "common.h"

LRESULT CALLBACK PlanetWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC                hdc;
    PAINTSTRUCT        ps;
    XFER               xf;
    int16_t            i;
    char              *psz;
    int32_t            lSel;
    RECT               rc;
    POINT16            pt;
    HCURSOR            hcs;
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    PLANET            *lpplMac;
    PLANET            *lppl;
    FLEET             *lpfl;
    POINT              t_pt_05b0;
    POINT              t_pt_05c0_1;

    switch (message) {
    case WM_CREATE:
        SetPlanetTitleBar(hwnd);
        for (i = 0; i < 3; i++) {
            rghwndOrderDD[i] = CreateWindow(szCombobox, "OrdDD", (i == 1 ? 528 : 0) | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200,
                                            i == 2 ? 160 : 80, hwnd, NULL, hInst, NULL);
            SendMessage(rghwndOrderDD[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        }
        for (i = 99; i < 109; i++) {
            psz = PszGetCompressedString(i);
            SendMessage(rghwndOrderDD[0], CB_ADDSTRING, 0, (LPARAM)psz);
        }
        hwndOrderED = CreateWindow(szEdit, NULL, ES_RIGHT | WS_CHILD | WS_BORDER, 100, 100, 200, 50, hwnd, NULL, hInst, NULL);
        SendMessage(hwndOrderED, EM_LIMITTEXT, 4, 0);
        SendMessage(hwndOrderED, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        lpfnRealEditProc = GetWindowLong(hwndOrderED, 0xfffc);
        SetWindowLong(hwndOrderED, 0xfffc, lpfnFakeEditProc);
        hwndBattleDD = CreateWindow(szCombobox, "BattleDD", CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd, NULL, hInst, NULL);
        SendMessage(hwndBattleDD, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndShipDD = CreateWindow(szCombobox, "ShipDD", CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd,
                                  NULL, hInst, NULL);
        SendMessage(hwndShipDD, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        GetClientRect(hwndShipDD, &rc);
        dyShipDD = rc.bottom;
        hwndShipLB = CreateWindow(szListbox, "ShipLB", LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | LBS_DISABLENOSCROLL | WS_CHILD | WS_BORDER | WS_VSCROLL, 100, 100,
                                  200, 80, hwnd, NULL, hInst, NULL);
        SendMessage(hwndShipLB, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndFleetCompLB =
            CreateWindow(szListbox, "FleetCompLB", LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | WS_VSCROLL,
                         100, 100, 200, 80, hwnd, NULL, hInst, NULL);
        SendMessage(hwndFleetCompLB, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
        hwndPlanetProdLB =
            CreateWindow(szListbox, "PlanetProdLB", LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | WS_VSCROLL,
                         100, 100, 200, 80, hwnd, NULL, hInst, NULL);
        SendMessage(hwndPlanetProdLB, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
        for (i = 0; i < 13; i++) {
            psz = PszGetCompressedString(i + 415);
            rghwndBtn[i] = CreateWindow(szButton, psz, WS_CHILD, 100, 100, 100, dyArial8 * 2, hwnd, NULL, hInst, NULL);
            SendMessage(rghwndBtn[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        }
        hwndRepCB =
            CreateWindow(szButton, PszGetCompressedString(idsRepeatOrders), BS_AUTOCHECKBOX | WS_CHILD, 100, 100, 150, dyArial8, hwnd, NULL, hInst, NULL);
        SendMessage(hwndRepCB, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        break;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawPlanShip(hdc, 4095);
        EndPaint(hwnd, &ps);
        break;
    default:
        if (IS_WM_CTLCOLOR(message) == 0) {
            switch (message) {
            case WM_MDIACTIVATE:
                hwndActive = wParam == 0 ? NULL : hwnd;
                return 0;
            case WM_GETMINMAXINFO:
                ((MINMAXINFO *)lParam)->ptMinTrackSize.x = dxWinFrame * 2 + 198;
                ((MINMAXINFO *)lParam)->ptMinTrackSize.y = dyWinFrame * 2 + 198 + dyTitleBar;
                break;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
                SetFocus(hwndFrame);
                PlanetClick(LOWORD(lParam), HIWORD(lParam), wParam, message == WM_RBUTTONDOWN ? 1 : 0);
                return 0;
            case WM_SETCURSOR:
                hcs = 0;
                GetCursorPos(&t_pt_05b0);
                pt = PointTo16(t_pt_05b0);
                t_pt_05c0_1 = PointFrom16(pt);
                ScreenToClient(hwnd, &t_pt_05c0_1);
                pt = PointTo16(t_pt_05c0_1);
                GetClientRect(hwnd, &rc);
                if (PtInRect(&rc, PointFrom16(pt)) == 0)
                    break;
                hcs = ClickInShipOrders(pt, 0, 1, 0);
                if (hcs == 0) {
                    hcs = ClickInPlanetOrders(pt, 0, 1, 0);
                }
                if (hcs == 0)
                    break;
                SetCursor(hcs);
                return 1;
            case WM_DRAWITEM:
                lpdis = (DRAWITEMSTRUCT *)lParam;
                if (lpdis->itemID == -1) {
                    HandleFocusState(lpdis, -2);
                } else {
                    switch (lpdis->itemAction) {
                    case 1:
                        DrawCBEntireItem(lpdis, -4);
                        break;
                    case 2:
                        DrawCBEntireItem(lpdis, -4);
                        break;
                    case 4:
                        DrawCBEntireItem(lpdis, -4);
                    }
                }
                return 1;
            case WM_MEASUREITEM:
                lpmis = (MEASUREITEMSTRUCT *)lParam;
                lpmis->itemHeight = dyArial8 + 2;
                return 1;
            case WM_CHAR:
                if (wParam != 102 && wParam != 70) {
                    return 0;
                }
                lppl = lpPlanets;
                lpplMac = lpPlanets + cPlanet;
                for (; lppl < lpplMac; lppl++) {
                    if (lppl->iPlayer == idPlayer) {
                        if (sel.grobj == grobjPlanet && sel.id == lppl->id)
                            break;
                        SelectAdjPlanet(0, lppl->id);
                        return 0;
                    }
                }
                for (i = 0; i < cFleet; i++) {
                    lpfl = rglpfl[i];
                    if (rglpfl[i] == 0)
                        break;
                    if (lpfl->iPlayer == idPlayer) {
                        if (sel.grobj == grobjFleet && sel.id == lpfl->id)
                            break;
                        SelectAdjFleet(0, lpfl->id);
                        return 0;
                    }
                }
                return 0;
            case WM_COMMAND:
                if (sel.grobj == grobjFleet) {
                    ShipCommandProc(hwnd, wParam, lParam);
                    return 0;
                }
                if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndShipDD) {
                    if (GET_WM_COMMAND_CMD(wParam, lParam) == 1) {
                        DrawPlanShip(NULL, -32764);
                    }
                } else {
                    if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[4] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        if (GetKeyState(VK_SHIFT) < 0) {
                            SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, 0));
                        } else {
                            SelectAdjPlanet(-1, 0);
                        }
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[5] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        if (GetKeyState(VK_SHIFT) < 0) {
                            SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, 1));
                        } else {
                            SelectAdjPlanet(1, 0);
                        }
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[0] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
                        if (lSel == -1 || FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, -1) == 0) {
                            return 0;
                        }
                        TransferStuff(sel.pl.id, grobjPlanet, xf.id, xf.grobj, mdXferCargo);
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[1] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
                        if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, -1) != 0 && xf.grobj == grobjFleet) {
                            SelectAdjFleet(0, xf.id);
                        }
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[2] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        pt.x = 610;
                        pt.y = 470;
                        ShipBuilder(pt);
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[11] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        ChangeProduction(0);
                    } else if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndBtn[12] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                        if (AlertSz(PszFormatIds(idsSureWantDeleteEverythingPlanetsProductionQueue, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) !=
                            IDYES) {
                            return 0;
                        }
                        ChangeProduction(1);
                    } else {
                        if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndPlanetProdLB || GET_WM_COMMAND_CMD(wParam, lParam) != 1)
                            break;
                        DrawPlanShip(NULL, 64);
                        if (gd.fTutorial == 0)
                            break;
                        tutor.fProgress = 1;
                        AdvanceTutor();
                        break;
                    }
                    SetFocus(hwndFrame);
                    return 0;
                }
            }
        } else if (GET_WM_CTLCOLOR_HWND(wParam, lParam) == hwndRepCB) {
            SetBkColor((HDC)wParam, crButtonFace);
            SetTextColor((HDC)wParam, crButtonText);
            return (LRESULT)hbrButtonFace;
        }
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void DrawPlanShip(HDC hdc, int16_t grbit) {
    HFONT    hfontSav;
    OBJ      objNull;
    int16_t  ctile;
    COLORREF crFore;
    OBJ      obj;
    int16_t  fMin;
    int16_t  i;
    COLORREF crBack;
    int16_t  fErase;
    TILE    *ptile;
    int16_t  fDC;
    RECT     rc;

    fDC = 0;
    objNull.pfl = 0;
    if (sel.id == -1) {
        for (i = 0; i < 13; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        for (i = 0; i < 3; i++) {
            ShowWindow(rghwndOrderDD[i], SW_HIDE);
        }
        ShowWindow(hwndOrderED, SW_HIDE);
        ShowWindow(hwndShipDD, SW_HIDE);
        ShowWindow(hwndBattleDD, SW_HIDE);
        ShowWindow(hwndShipLB, SW_HIDE);
        ShowWindow(hwndFleetCompLB, SW_HIDE);
        ShowWindow(hwndPlanetProdLB, SW_HIDE);
        ShowWindow(hwndRepCB, SW_HIDE);
        for (i = 0; i < 19; i++) {
            rgrcRef[i].bottom = -6;
            rgrcRef[i].top = -5;
        }
        if (rgplr[idPlayer].fDead != 0 && hdc != 0) {
            GetClientRect(hwndPlanet, &rc);
            SetBkColor(hdc, crButtonFace);
            SetTextColor(hdc, crButtonText);
            i = CchGetString(idsDeceased, szWork);
            DiaganolTextOut(hdc, &rc, szWork, i);
        }
    } else {
        if (sel.grobj == grobjFleet) {
            ptile = rgtileShip;
            ctile = 7;
            obj.pfl = &sel.fl;
        } else {
            ptile = rgtilePlanet;
            ctile = 6;
            obj.ppl = &sel.pl;
        }
        if (hdc == 0) {
            fDC = 1;
            hdc = GetDC(hwndPlanet);
        }
        hfontSav = SelectObject(hdc, rghfontArial8[0]);
        crBack = SetBkColor(hdc, crButtonFace);
        crFore = SetTextColor(hdc, crButtonText);
        fErase = (grbit & 0x8000) == 0 ? 0 : 1;
        fMin = (grbit & 0x4000) == 0 ? 0 : 1;
        for (i = 0; i < ctile; i++) {
            if ((grbit & ptile[i].grbit) != 0) {
                ptile[i].fErase = fErase;
                ptile[i].fMinDraw = fMin;
                ptile[i].pfn(hdc, ptile + i, ptile[i].fNullPtr == 0 ? obj : objNull);
            }
        }
        SetTextColor(hdc, crFore);
        SetBkColor(hdc, crBack);
        SelectObject(hdc, hfontSav);
        if (fDC != 0) {
            ReleaseDC(hwndPlanet, hdc);
        }
    }
    return;
}

int16_t FDrawTileNC(HDC hdc, TILE *ptile, RECT *prc, char *pszTitle) {
    int16_t bt;
    RECT    rcT;

    bt = 112;
    prc->left = ptile->iCol * 198 + 4;
    prc->right = prc->left + 190;
    prc->top = ptile->yTop;
    prc->bottom = (ptile->fPopped == 0 ? dyArial8 + 3 : ptile->dyFull) + prc->top;
    if (ptile->fMinDraw == 0 || ptile->fMinTitle != 0) {
        if (ptile->fMinDraw == 0) {
            _Draw3dFrame(hdc, prc, 0);
        }
        rcT = *prc;
        ExpandRc(&rcT, -1, -1);
        rcT.bottom = rcT.top + dyArial8 + 2;
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, crButtonText);
        SetBkColor(hdc, crButtonFace);
        if (ptile->fMinDraw == 0) {
            _Draw3dFrame(hdc, &rcT, 0);
        }
        RcCtrTextOut(hdc, &rcT, pszTitle, -1);
        SetRect(&rcT, prc->right - 17, prc->top + 1, prc->right, rcT.bottom + 1);
        if (ptile->fPopped != 0) {
            bt = bt;
        } else {
            bt |= 1;
        }
        DrawBtn(hdc, &rcT, bt, 0, NULL);
        SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, prc->right - 18, prc->top + 1, 1, rcT.bottom - prc->top - 1, PATCOPY);
    }
    prc->top += dyArial8 + 4;
    return ptile->fPopped;
}

void DrawPlanetMinSum(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t dxRight;
    int16_t yTop;
    int16_t xRight;
    int16_t c;
    int16_t i;
    int16_t xLeft;
    HBRUSH  hbrSav;
    PLANET *ppl;
    RECT    rc;

    ppl = obj.ppl;
    if (ptile->fFixCtls != 0) {
        rgrcRef[6].top = -5;
        rgrcRef[6].bottom = -6;
        rgrcRef[7].top = -5;
        rgrcRef[7].bottom = -6;
        rgrcRef[8].top = -5;
        rgrcRef[8].bottom = -6;
        rgrcRef[9].top = -5;
        rgrcRef[9].bottom = -6;
        rgrcRef[11].top = -5;
        rgrcRef[11].bottom = -6;
        rgrcRef[10].top = -5;
        rgrcRef[10].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsMineralsHand)) != 0) {
        if (ppl == 0) {
            ppl = &sel.pl;
        }
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        dxRight = dxMaxMineralQuan;
        SetRect(&rgrcRef[6], xLeft, yTop, xRight, 3 * dyArial8 + yTop);
        for (i = 0; i <= 2; i++) {
            if (ptile->fMinDraw == 0) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, rgcrMinerals[i]);
                TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));
            }
            SelectObject(hdc, rghfontArial8[0]);
            SetTextColor(hdc, crButtonText);
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), ppl->rgwtMin[i]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
        }
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, rc.left, yTop++, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        SetRect(&rgrcRef[7], xLeft, yTop, xRight, dyArial8 * 2 + yTop);
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsMines4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            c = _wsprintf(szWork, PszGetCompressedString(idsD), CMinesOperating(ppl));
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), ppl->cMines, CMaxOperableMines(ppl, idPlayer, 0));
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight * 2);
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsFactories4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            c = CchGetString(idsN, szWork);
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), ppl->cFactories, CMaxOperableFactories(ppl, idPlayer, 0));
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight * 2);
        yTop += dyArial8;
    }
    return;
}

void DrawPlanetStats(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t dxRight;
    int32_t l2;
    int16_t yTop;
    int16_t xRight;
    int16_t c;
    int16_t cRes;
    int16_t dRangeP;
    float   pct;
    int16_t cResAvail;
    int16_t dRange;
    char   *psz;
    int16_t xLeft;
    HBRUSH  hbrSav;
    int32_t l;
    RECT    rc;
    PART    part;

    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsStatus)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        SelectObject(hdc, rghfontArial8[1]);
        c = CchGetString(idsPopulation4, szWork);
        l = GetTextExtent(hdc, szWork, c);
        c = CchGetString(idsScannerType, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        c = CchGetString(idsScannerRange, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        c = CchGetString(idsDefenses4, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        c = CchGetString(idsDefenseType, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        c = CchGetString(idsDefCoverage, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        c = CchGetString(idsResourcesYear, szWork);
        l2 = GetTextExtent(hdc, szWork, c);
        if (l2 > l) {
            l = l2;
        }
        dxRight = xRight - xLeft - LOWORD(l);
        if (ptile->fMinDraw == 0) {
            c = CchGetString(idsPopulation4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        SetRect(&rgrcRef[9], xLeft, yTop, xRight, yTop + dyArial8);
        c = CommaFormatLong(szWork, (uint32_t)(sel.pl.rgwtMin[3] * 100));
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsResourcesYear, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        SetRect(&rgrcRef[8], xLeft, yTop, xRight, yTop + dyArial8);
        cResAvail = CResourcesAtPlanet(&sel.pl, idPlayer);
        cRes = cResAvail;
        if (sel.pl.fNoResearch == 0) {
            cResAvail -= MulDiv(cRes, rgplr[idPlayer].pctResearch, 100);
        }
        c = _wsprintf(szWork, PszGetCompressedString(idsDD), cResAvail, cRes);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop += dyArial8;
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, rc.left, yTop++, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsScannerType, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        SetRect(&rgrcRef[11], xLeft, yTop, xRight, dyArial8 * 2 + yTop);
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            CchGetString(idsOrganic, szWork);
            dRange = GetPlanetScannerRange(&sel.pl, &dRangeP);
        } else if (sel.pl.iScanner == 31) {
            CchGetString(idsNone4, szWork);
            dRange = 0;
        } else {
            LookupBestPlanetaryScanner(&part);
            fstrcpy(szWork, part.pcom->szName);
            dRange = GetPlanetScannerRange(&sel.pl, &dRangeP);
        }
        RightTextOut(hdc, xRight, yTop, szWork, 0, dxRight);
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsScannerRange, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (dRange <= 0) {
            CchGetString(idsNone4, szWork);
            c = 0;
        } else if (dRangeP > 0) {
            c = _wsprintf(szWork, PszGetCompressedString(idsDDLY), dRangeP, dRange);
        } else if (dRange < 100) {
            c = _wsprintf(szWork, PszGetCompressedString(idsDLightYears), dRange);
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDLY), dRange);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop += dyArial8;
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, rc.left, yTop++, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefenses4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        SetRect(&rgrcRef[10], xLeft, yTop, xRight, 3 * dyArial8 + yTop);
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            c = CchGetString(idsN, szWork);
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), sel.pl.cDefenses, CMaxOperableDefenses(&sel.pl, idPlayer, 0));
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefenseType, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (sel.pl.cDefenses == 0) {
            CchGetString(GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh ? idsN : idsNone4, szWork);
            dRange = 0;
        } else {
            FGetBestDefensePart(&part);
            fstrcpy(szWork, part.pcom->szName);
            dRange = 1;
        }
        RightTextOut(hdc, xRight, yTop, szWork, 0, dxRight);
        yTop += dyArial8;
        if (ptile->fMinDraw == 0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefCoverage, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (dRange != 0) {
            CalcPctSurvive(&sel.pl, &pct, NULL);
            pct = (float)((long double)1.0 - pct);
            c = _wsprintf(szWork, PCTDXPCTDPCTPCT, LOWORD((int32_t)((long double)pct * 100)),
                          LOWORD((int32_t)((pct - (long double)(int16_t)LOWORD((int32_t)((long double)pct * 100)) / 100.0) * 10000)));
        } else {
            c = CchGetString(GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh ? idsN : idsNone4, szWork);
            psz = szWork;
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
    }
    return;
}

int16_t FGetBestDefensePart(PART *ppart) {
    int16_t fRet;
    int16_t i;
    PART    part;

    fRet = 1;
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = iplanetarySDI;
    i = 0;
    while (i < 5 && FLookupPart(&part) == 1) {
        i++;
        part.hs.iItem++;
    }
    if (i > 0) {
        i--;
    } else {
        fRet = 0;
    }
    part.hs.iItem = i + 9;
    FLookupPart(&part);
    *ppart = part;
    return fRet;
}

void DrawPlanetStarbase(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t  fTwo;
    int16_t  dxRight;
    int16_t  iWarp;
    int16_t  bt;
    int16_t  yTop;
    int16_t  xRight;
    int16_t  c;
    SHDEF   *lpshdef;
    COLORREF crForeSav;
    uint16_t w;
    char    *psz;
    int16_t  xLeft;
    HBRUSH   hbrSav;
    int32_t  l;
    RECT     rc;

    if (ptile->fFixCtls != 0) {
        rgrcRef[13].top = -5;
        rgrcRef[13].bottom = -6;
        rgrcRef[14].top = -5;
        rgrcRef[14].bottom = -6;
        rgrcRef[16].top = -5;
        rgrcRef[16].bottom = -6;
        rgrcRef[15].top = -5;
        rgrcRef[15].bottom = -6;
        ptile->fFixCtls = 0;
    }
    if (sel.pl.fStarbase != 0) {
        lpshdef = rglpshdefSB[idPlayer] + sel.pl.isb;
        fstrcpy(szWork, lpshdef->hul.szClass);
        psz = szWork;
    } else {
        psz = PszGetCompressedString(idsStarbase2);
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        SetRect(&rc, xLeft - 2, yTop, xRight + 2, rc.bottom - 2);
        FillRect(hdc, &rc, hbrButtonFace);
        if (sel.pl.fStarbase == 0) {
            SetRect(&rgrcRef[14], -5, -5, -6, -6);
            rgrcRef[15] = rgrcRef[14];
            rgrcRef[16] = rgrcRef[14];
        } else {
            SetRect(&rgrcRef[14], xLeft, yTop, xRight, dyArial8 * 4 + yTop);
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDockCapacity, szWork);
            l = GetTextExtent(hdc, szWork, c);
            dxRight = xRight - xLeft - LOWORD(l);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
            w = LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax;
            if (w == 0) {
                c = CchGetString(idsNone4, szWork);
            } else if ((uint32_t)w == 0xffff) {
                c = CchGetString(idsUnlimited, szWork);
            } else {
                c = _wsprintf(szWork, PCTDKT, w);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsArmor2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            w = lpshdef->hul.dp;
            if (w == 0) {
                c = CchGetString(idsNone4, szWork);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsLddp), w, 0);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsShields2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            l = DpShieldOfShdef(lpshdef, idPlayer);
            if (l == 0) {
                c = CchGetString(idsNone4, szWork);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsLddp), l);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDamage2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
            w = sel.pl.pctDp;
            if (w != 0) {
                if (w < 5) {
                    w = 5;
                }
                c = _wsprintf(szWork, PCTDPCTPCT, (uint32_t)w / 5);
                crForeSav = SetTextColor(hdc, 127);
            } else {
                c = CchGetString(idsNone4, szWork);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            if (w != 0) {
                SetTextColor(hdc, crForeSav);
            }
            hbrSav = SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, rc.left, yTop++, rc.right - rc.left, 1, PATCOPY);
            SelectObject(hdc, hbrSav);
            SelectObject(hdc, rghfontArial8[1]);
            SetRect(&rgrcRef[16], xLeft, yTop, xRight, yTop + dyArial8);
            c = CchGetString(idsMassDriver, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            iWarp = IWarpMAFromLppl(&sel.pl, &fTwo);
            if (iWarp > 0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsWarpD), iWarp);
                if (fTwo != 0) {
                    szWork[c++] = '+';
                }
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsNone4));
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDestination3, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            if (sel.pl.idFling == 0) {
                c = CchGetString(idsNone4, szWork);
            } else {
                psz = PszGetCompressedPlanet(rgidPlan[sel.pl.idFling - 1]);
                c = 0;
                strcpy(szWork, psz);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop += dyArial8;
            c = (int16_t)(xRight - xLeft) / 3;
            SetRect(&rgrcRef[13], xLeft, yTop, xLeft + c, yTop + dyArial8 + 6);
            bt = 8;
            if (iWarp == 0) {
                bt |= 4;
            }
            DrawBtn(hdc, &rgrcRef[13], bt, gd.fSetMassMode, PszGetCompressedString(idsSetDest));
            if (iWarp > 0) {
                SetRect(&rgrcRef[15], xLeft + c + 4, yTop + 3, xRight, yTop + dyArial8 + 3);
                DrawMassWarpGauge(hdc, &rgrcRef[15], fTwo == 0 ? iWarp : -iWarp, sel.pl.iWarpFling + 4);
            } else {
                SetRect(&rgrcRef[15], -5, -5, -6, -6);
                rgrcRef[16] = rgrcRef[15];
            }
        }
    }
    return;
}

void DrawMassWarpGauge(HDC hdc, RECT *prc, int16_t iBest, int16_t iCur) {
    int32_t lMax;
    int16_t c;
    int16_t fTwoMAs;
    int16_t iMode;
    HBRUSH  hbr;
    int32_t lCur;
    int32_t l;

    fTwoMAs = iBest >= 0 ? 0 : 1;
    SelectObject(hdc, rghfontArial8[1]);
    if (iCur < 5) {
        iCur = 5;
    }
    if (iBest < 0) {
        iBest = -iBest;
    }
    lMax = (int16_t)(iBest - 1);
    if (iCur <= iBest + fTwoMAs) {
        hbr = hbrPurple;
    } else if (iCur < iBest + fTwoMAs + 3) {
        hbr = hbrYellow;
    } else {
        hbr = hbrRed;
    }
    lCur = (int16_t)(iCur - 4);
    l = LDrawGauge(hdc, prc, 1, &lCur, &hbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpLd), l + 4);
    l = GetTextExtent(hdc, szWork, c);
    RcCtrTextOut(hdc, prc, szWork, c);
    SetBkMode(hdc, iMode);
    return;
}

void DrawPlanetProduction(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t swp;
    int16_t dxRight;
    int16_t yTop;
    int16_t xStart;
    int16_t xRight;
    char    szT[40];
    int16_t i;
    int16_t c;
    int16_t dyWrong;
    char   *psz;
    int16_t iSel;
    int16_t cch;
    int16_t xLeft;
    RECT    rcT;
    PLANET *ppl;
    RECT    rc;

    ppl = obj.ppl;
    if (ptile->fFixCtls != 0) {
        ShowWindow(hwndPlanetProdLB, SW_HIDE);
        ShowWindow(rghwndBtn[11], SW_HIDE);
        ShowWindow(rghwndBtn[12], SW_HIDE);
        rgrcRef[17].top = -5;
        rgrcRef[17].bottom = -6;
        ptile->fFixCtls = 0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsProduction)) == 0) {
        ShowWindow(hwndPlanetProdLB, SW_HIDE);
        ShowWindow(rghwndBtn[11], SW_HIDE);
        ShowWindow(rghwndBtn[12], SW_HIDE);
    } else {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        yTop += 4;
        GetClientRect(hwndPlanetProdLB, &rcT);
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        dyPlanetProdLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0 ? 5 : 3);
        dyWrong = dyPlanetProdLB - (rcT.bottom - rcT.top);
        if (dxPlanetProdLB == xRight - xLeft && dyWrong >= 0 && dyWrong < dyArial8) {
            swp |= 1;
        } else {
            dxPlanetProdLB = xRight - xLeft;
        }
        SetWindowPos(hwndPlanetProdLB, NULL, xLeft, yTop, xRight - xLeft, dyPlanetProdLB, swp);
        ShowWindow(hwndPlanetProdLB, SW_SHOW);
        GetClientRect(hwndPlanetProdLB, &rcT);
        dyPlanetProdLB = rcT.bottom - rcT.top;
        yTop += dyPlanetProdLB + 4;
        iSel = LOWORD(SendMessage(hwndPlanetProdLB, LB_GETCURSEL, 0, 0));
        if (iSel < 0) {
            iSel = 0;
        }
        if (ppl->lpplprod == 0 || ppl->lpplprod->iprodMac == 0) {
            c = 0;
            szWork[0] = 0;
        } else {
            psz = PszProductionETA(ppl, NULL, iSel, NULL, NULL);
            c = strlen(psz);
        }
        if (c != 0) {
            SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsCompletion, szT);
            TextOut(hdc, xLeft, yTop, szT, cch);
            dxRight = xRight - xLeft - LOWORD(GetTextExtent(hdc, szT, cch));
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        } else {
            RightTextOut(hdc, xRight, yTop, szWork, 0, xRight - xLeft);
        }
        yTop += dyArial8;
        SelectObject(hdc, rghfontArial8[1]);
        cch = CchGetString(idsRoute3, szT);
        TextOut(hdc, xLeft, yTop, szT, cch);
        dxRight = xRight - xLeft - LOWORD(GetTextExtent(hdc, szT, cch));
        if (sel.pl.idRoute == 0) {
            c = CchGetString(idsNone4, szWork);
        } else {
            psz = PszGetCompressedPlanet(rgidPlan[sel.pl.idRoute - 1]);
            c = 0;
            strcpy(szWork, psz);
        }
        SelectObject(hdc, rghfontArial8[0]);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop += dyArial8;
        c = (int16_t)(xRight - xLeft - 16) / 3;
        xStart = xLeft;
        i = 11;
        while (i <= 12) {
            SetWindowPos(rghwndBtn[i], NULL, xStart, yTop, c, (dyArial8 >> 1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(rghwndBtn[i], SW_SHOW);
            i++;
            xStart += c + 8;
        }
        EnableWindow(rghwndBtn[12], sel.pl.lpplprod != 0 && sel.pl.lpplprod->iprodMac != 0);
        SetRect(&rgrcRef[17], xStart, yTop, xStart + c, yTop + dyArial8 + (dyArial8 >> 1));
        DrawBtn(hdc, &rgrcRef[17], 8, gd.fSetRouteMode, PszGetCompressedString(idsRoute2));
    }
    return;
}

char *PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast) {
    int16_t  iTurnEnd;
    int16_t  iTurnBegin;
    int16_t  c;
    StringId ids;

    if (lpplprod == 0) {
        lpplprod = lppl->lpplprod;
    }
    EstimateItemProdSched(lppl, lpplprod, iItem, &iTurnBegin, &iTurnEnd);
    if (iTurnBegin == 100) {
        if (lpplprod != 0 && lpplprod->iprodMac > iItem && lpplprod->rgprod[iItem].grobj == grobjPlanet && lpplprod->rgprod[iItem].iItem < mdIdleFactory) {
            ids = idsUnknown2;
        } else {
            ids = idsNever;
        }
        c = CchGetString(ids, szWork);
    } else if (iTurnEnd == 100) {
        c = _wsprintf(szWork, PszGetCompressedString(idsDYears), iTurnBegin);
    } else if (iTurnBegin != iTurnEnd) {
        c = _wsprintf(szWork, PszGetCompressedString(idsDDYears), iTurnBegin, iTurnEnd);
    } else if (iTurnBegin == 0) {
        c = CchGetString(idsSkipped, szWork);
    } else if (iTurnBegin == -1) {
        c = CchGetString(idsNeeded, szWork);
    } else {
        c = _wsprintf(szWork, PszGetCompressedString(idsDYear), iTurnBegin);
        if (iTurnBegin != 1) {
            szWork[c] = 's';
            c++;
            szWork[c] = 0;
        }
    }
    if (etaFirst != 0) {
        *etaFirst = iTurnBegin;
    }
    if (etaLast != 0) {
        *etaLast = iTurnEnd;
    }
    return szWork;
}

void DrawPlanShipBitmap(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t yTop;
    int16_t dy;
    int16_t xRight;
    int16_t i;
    char   *psz;
    int16_t dx;
    int16_t xLeft;
    HBRUSH  hbrSav;
    int16_t iOffset;
    RECT    rc;

    if (sel.grobj == grobjPlanet) {
        psz = PszGetPlanetName(obj.ppl->id);
        i = obj.ppl->id;
        i += 8;
        iOffset = i % 28;
    } else {
        psz = PszGetFleetName(obj.pfl->id);
    }
    if (ptile->fFixCtls != 0) {
        for (i = 4; i <= 6; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ptile->fFixCtls = 0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) == 0) {
        for (i = 4; i <= 6; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
    } else {
        xLeft = rc.left + 12;
        xRight = rc.right - 12;
        yTop = (gd.fSmallTileMode == 0 ? 6 : 2) + rc.top;
        if (sel.grobj == grobjFleet) {
            DrawFleetBitmap(&sel.fl, hdc, xLeft, yTop, 1, -1, 0, 0, -1, 0);
        } else {
            hbrSav = SelectObject(hdc, hbrButtonShadow);
            PatBlt(hdc, xLeft, yTop, 70, 2, PATCOPY);
            PatBlt(hdc, xLeft, yTop + 2, 2, 68, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, xLeft + 2, yTop + 68, 68, 2, PATCOPY);
            PatBlt(hdc, xLeft + 68, yTop + 2, 2, 66, PATCOPY);
            PatBlt(hdc, xLeft + 1, yTop + 69, 1, 1, PATCOPY);
            PatBlt(hdc, xLeft + 69, yTop + 1, 1, 1, PATCOPY);
            PatBlt(hdc, xLeft + 2, yTop + 2, 66, 1, BLACKNESS);
            PatBlt(hdc, xLeft + 2, yTop + 3, 1, 65, BLACKNESS);
            PatBlt(hdc, xLeft + 3, yTop + 67, 65, 1, BLACKNESS);
            PatBlt(hdc, xLeft + 67, yTop + 3, 1, 64, BLACKNESS);
            SelectObject(hdc, hbrSav);
            SelectPalette(hdc, vhpal, 0);
            RealizePalette(hdc);
            DibBlt(hdc, xLeft + 3, yTop + 3, 64, 64, hdibPlanets, iOffset % 7 * 64, iOffset / 7 * 64, 64, 64, 13369376);
        }
        dx = xRight - xLeft - 95;
        dy = 3 * dyArial8 >> 1;
        xLeft = xRight - dx;
        if (ptile->fMinDraw == 0) {
            if (gd.fSmallTileMode != 0) {
                yTop -= 2;
                dy -= 2;
            } else {
                yTop -= 4;
            }
            iOffset = sel.grobj == grobjFleet ? 6 : 5;
            i = 4;
            while (i <= iOffset) {
                SetWindowPos(rghwndBtn[i], NULL, xLeft, yTop, dx, dy, SWP_NOZORDER | SWP_NOACTIVATE);
                ShowWindow(rghwndBtn[i], SW_SHOW);
                i++;
                yTop += (gd.fSmallTileMode == 0 ? 3 : 2) + dy;
            }
        }
    }
    return;
}

void DrawPlanetShipList(HDC hdc, TILE *ptile, OBJ obj) {
    int16_t swp;
    int16_t fDoneDrawing;
    int32_t l2;
    int16_t yTop;
    int16_t fObjIsThing;
    int16_t fUnknown;
    int16_t idSkip;
    int16_t xStart;
    int16_t xRight;
    int16_t i;
    int16_t c;
    RECT    rcGauge;
    XFER    xf;
    FLEET  *pfl;
    int32_t lSel;
    int16_t xLeft;
    int32_t l;
    RECT    rc;

    pfl = obj.pfl;
    fDoneDrawing = 0;
    fObjIsThing = 0;
    if (ptile->fFixCtls != 0) {
        for (i = 0; i <= 2; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ShowWindow(hwndShipDD, SW_HIDE);
        ptile->fFixCtls = 0;
        rgrcRef[1].top = -5;
        rgrcRef[1].bottom = -6;
        rgrcRef[4].top = -5;
        rgrcRef[4].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(pfl == 0 ? idsFleetsOrbit : idsOtherFleetsHere)) == 0) {
        for (i = 0; i <= 2; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ShowWindow(hwndShipDD, SW_HIDE);
    } else {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 2;
        swp = SWP_NOZORDER | SWP_NOACTIVATE;
        if (dxShipDD == xRight - xLeft) {
            swp |= 1;
        } else {
            dxShipDD = xRight - xLeft;
        }
        SetWindowPos(hwndShipDD, NULL, xLeft, yTop, xRight - xLeft, 5 * dyShipDD, swp);
        ShowWindow(hwndShipDD, SW_SHOW);
        yTop += dyShipDD + 3;
        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
        EnableWindow(rghwndBtn[0], lSel == -1 ? 0 : 1);
        if (lSel == -1) {
            fDoneDrawing = 1;
            fUnknown = 1;
        }
        if (pfl != 0) {
            idSkip = pfl->id;
        } else {
            idSkip = -1;
        }
        if (fDoneDrawing == 0 && FLookupOrbitingXfer(idSkip == -1 ? sel.pl.id : pfl->idPlanet, LOWORD(lSel), &xf, idSkip) == 0) {
            fDoneDrawing = 1;
        }
        if (fDoneDrawing != 0) {
            EnableWindow(rghwndBtn[1], 0);
        } else {
            fObjIsThing = xf.grobj == grobjThing ? 1 : 0;
            EnableWindow(rghwndBtn[1], fObjIsThing == 0 && xf.fl.iPlayer == idPlayer);
        }
        rgrcRef[1].top = -5;
        rgrcRef[1].bottom = -6;
        rgrcRef[4].top = -5;
        rgrcRef[4].bottom = -6;
        fUnknown = fObjIsThing == 0 && xf.fl.det != detAll;
        if (gd.fSmallTileMode == 0) {
            if (fDoneDrawing == 0 && fUnknown == 0) {
                SelectObject(hdc, rghfontArial8[1]);
                c = CchGetString(idsCargo3, szWork);
                l = GetTextExtent(hdc, szWork, c);
            }
            if (fDoneDrawing == 0 && fUnknown == 0 && fObjIsThing == 0) {
                c = CchGetString(idsFuel3, szWork);
                l2 = GetTextExtent(hdc, szWork, c);
                if (l2 > l) {
                    l = l2;
                }
                TextOut(hdc, xLeft, yTop, szWork, c);
                SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
                if (idSkip != -1) {
                    rgrcRef[1] = rcGauge;
                }
                DrawFleetGauge(hdc, &rcGauge, &xf.fl, 4);
            } else if (ptile->fMinDraw != 0 || fUnknown != gd.fUnknownShip) {
                SetRect(&rcGauge, xLeft, yTop, xRight, dyArial8 * 2 + yTop + 8);
                FillRect(hdc, &rcGauge, hbrButtonFace);
            }
            gd.fUnknownShip = fUnknown;
            if (fObjIsThing == 0) {
                yTop += dyArial8 + 4;
            }
            if (fDoneDrawing == 0 && fUnknown == 0) {
                c = CchGetString(idsCargo3, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
                SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
                rgrcRef[4] = rcGauge;
                if (fObjIsThing != 0) {
                    DrawThingGauge(hdc, &rcGauge, &xf.th, 5);
                } else {
                    DrawFleetGauge(hdc, &rcGauge, &xf.fl, 5);
                }
                if (fObjIsThing != 0) {
                    OffsetRect(&rcGauge, 0, dyArial8 + 4);
                    FillRect(hdc, &rcGauge, hbrButtonFace);
                    yTop += dyArial8 + 4;
                }
            }
            yTop += dyArial8 + 4;
        }
        c = (int16_t)(xRight - xLeft - 10) / 3;
        xStart = xLeft;
        i = 0;
        while (i <= 2) {
            SetWindowPos(rghwndBtn[(int16_t)(i + 1) % 3], NULL, xStart, yTop, c, (dyArial8 >> 1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            if (i != 2 || pfl != 0) {
                ShowWindow(rghwndBtn[i], SW_SHOW);
            }
            i++;
            xStart += c + 6;
        }
        EnableWindow(rghwndBtn[2], (idSkip == -1 || fUnknown == 0) && fObjIsThing == 0);
    }
    return;
}

void SetPlanetTitleBar(HWND hwnd) {
    char  szTitle[30];
    char *psz;

    if (sel.grobj == grobjPlanet) {
        psz = PszGetPlanetName(sel.pl.id);
        CchGetString(idsPlanet2, szTitle);
        lstrcat(szTitle, psz);
        psz = szTitle;
    } else if (sel.grobj == grobjFleet) {
        psz = PszGetFleetName(sel.fl.id);
    } else {
        psz = PszGetCompressedString(idsPlanetView);
    }
    SetWindowText(hwnd, psz);
    return;
}

void ChangeMainObjSel(GrobjClass grobjNew, int16_t iObjSel) {
    int16_t fSameType;
    int16_t idSkip;
    int16_t i;
    FLEET  *lpfl;

    idSkip = -1;
    fSameType = grobjNew == sel.grobj ? 1 : 0;
    if (fAi == 0 || fSameType == 0 || iObjSel != sel.id) {
        InvalidateReport(sel.grobj == grobjPlanet ? 0 : 1, 0);
        if (grobjNew == grobjPlanet) {
            InvalidateReport(0, 0);
            if (FLookupPlanet(iObjSel, &sel.pl) == 0) {
                return;
            }
            sel.pt = rgptPlan[iObjSel];
            sel.scan.iwp = -1;
            sel.iwpAct = -1;
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || (lpfl->idPlanet == iObjSel && lpfl->iPlayer == idPlayer))
                    break;
            }
            if (i != cFleet) {
                FDupFleet(lpfl, &sel.fl);
                sel.grobjFull = grobjPlanet | grobjFleet;
            } else {
                sel.fl.id = -1;
                sel.grobjFull = grobjPlanet;
            }
            if (fAi == 0) {
                FillPlanetProdLB(NULL, NULL, NULL);
                SendMessage(hwndPlanetProdLB, LB_SETCURSEL, 0, 0);
            }
        } else {
            InvalidateReport(1, 0);
            if (FLookupFleet(iObjSel, &sel.fl) == 0) {
                return;
            }
            sel.pt = sel.fl.pt;
            if (sel.fl.idPlanet == -1 || (sel.fl.idPlanet != sel.pl.id && FLookupPlanet(sel.fl.idPlanet, &sel.pl) == 0)) {
                sel.pl.id = -1;
            }
            sel.grobjFull = (sel.pl.id == -1 ? 0 : 1) | 2;
            sel.iwpAct = 0;
            if (fAi == 0) {
                FillOrdersLB();
                FillFleetCompLB();
                FillBattleDD(sel.fl.iplan + 1);
                idSkip = iObjSel;
                SendMessage(rghwndOrderDD[0], CB_SETCURSEL, sel.fl.lpplord->rgord[0].grTask, 0);
            }
        }
        sel.grobj = grobjNew;
        sel.id = iObjSel;
        gd.fSetMassMode = 0;
        gd.fSetRouteMode = 0;
        if (fAi == 0) {
            if (fSameType == 0) {
                for (i = 0; i < 13; i++) {
                    ShowWindow(rghwndBtn[i], SW_HIDE);
                }
                for (i = 0; i < 3; i++) {
                    ShowWindow(rghwndOrderDD[i], SW_HIDE);
                }
                ShowWindow(hwndOrderED, SW_HIDE);
                ShowWindow(hwndShipDD, SW_HIDE);
                ShowWindow(hwndBattleDD, SW_HIDE);
                ShowWindow(hwndShipLB, SW_HIDE);
                ShowWindow(hwndFleetCompLB, SW_HIDE);
                ShowWindow(hwndPlanetProdLB, SW_HIDE);
                ShowWindow(hwndRepCB, SW_HIDE);
                for (i = 0; i < 19; i++) {
                    rgrcRef[i].bottom = -6;
                    rgrcRef[i].top = -5;
                }
            }
            FillShipDD(idSkip);
            if (fSameType != 0) {
                DrawPlanShip(NULL, 20479);
            } else {
                InvalidateRect(hwndPlanet, NULL, 1);
                if ((grbitScan & 0x10) != 0 && sel.grobj == grobjPlanet) {
                    grbitScan &= 0xffef;
                    InvalidateRect(hwndTb, NULL, 1);
                }
            }
            SetPlanetTitleBar(hwndPlanet);
            if (gd.fTutorial != 0) {
                AdvanceTutor();
            }
        }
    }
    return;
}

void FillShipDD(int16_t idSkip) {
    THING  *lpthMac;
    int16_t i;
    THING  *lpth;
    FLEET  *lpfl;
    POINT16 ptSel;

    SendMessage(hwndShipDD, CB_RESETCONTENT, 0, 0);
    if (sel.grobj == grobjPlanet) {
        ptSel = rgptPlan[sel.id];
    } else {
        ptSel = sel.fl.pt;
    }
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if ((idSkip == -1 && sel.id == lpfl->idPlanet) || (idSkip != -1 && lpfl->id != idSkip && lpfl->pt.x == sel.fl.pt.x && lpfl->pt.y == sel.fl.pt.y)) {
            PszGetFleetName(lpfl->id);
            memmove(&szWork[1], szWork, 50);
            szWork[0] = LOBYTE(lpfl->iPlayer == idPlayer ? 32 : 120);
            SendMessage(hwndShipDD, CB_ADDSTRING, 0, (LPARAM)szWork);
        }
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMineralPacket && lpth->pt.x == ptSel.x && lpth->pt.y == ptSel.y) {
            PszGetThingName(lpth->idFull);
            memmove(&szWork[1], szWork, 50);
            szWork[0] = LOBYTE(lpth->iplr == idPlayer ? 32 : 120);
            SendMessage(hwndShipDD, CB_ADDSTRING, 0, (LPARAM)szWork);
        }
    }
    SendMessage(hwndShipDD, CB_SETCURSEL, 0, 0);
    return;
}

void SelectAdjPlanet(int16_t dInc, int16_t idPlanet) {
    PLANET *lpPlT;
    int16_t i;
    PLANET *lpPl;
    SCAN    scan;
    int16_t fWrap;

    fWrap = 0;
    if (cPlanet > 0 && idPlanet != -1) {
        if (dInc != 0) {
            idPlanet = sel.pl.id;
        }
        if (vrptPlanet.fCached == 0) {
            InvalidateReport(0, 1);
        }
        lpPlT = lpPlanets;
        lpPl = lpPlanets;
        i = 0;
        for (; i < cPlanet && lpPl->id != idPlanet; lpPl++) {
            i++;
        }
        if (i == cPlanet || lpPl->det != detAll) {
            scan.pt = rgptPlan[idPlanet];
            scan.grobj = grobjPlanet | mdExact;
            ChangeScanSel(&scan, 0);
        } else {
            if (dInc != 0) {
                for (i = 0; i < rgplr[idPlayer].cPlanet && lpPlT[vlprgidPlanet[i]].id != idPlanet; i++) {
                }
                i += dInc;
                if (i >= rgplr[idPlayer].cPlanet) {
                    i = 0;
                } else if (i < 0) {
                    i = rgplr[idPlayer].cPlanet - 1;
                }
                i = vlprgidPlanet[i];
            }
            lpPlT += i;
            idPlanet = lpPlT->id;
            if (lpPlT->iPlayer != idPlayer) {
                return;
            }
            scan.pt = rgptPlan[idPlanet];
            scan.grobj = grobjPlanet | mdExact;
            ChangeScanSel(&scan, 0);
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(grobjPlanet, idPlanet);
            RedrawScanSel(NULL, 1);
        }
        CtrPointScan(rgptPlan[idPlanet], 1);
        DrawScannerSBar(NULL, NULL, NULL, 0);
        InvalidateRect(hwndMine, NULL, 1);
        SetMineralTitleBar(hwndMine);
    }
    return;
}

int16_t IdFindAdjStarbase(int16_t idPlanet, int16_t fNext) {
    PLANET *lpplMac;
    int16_t idLast;
    int16_t idFirst;
    PLANET *lppl;
    int16_t idAfter;
    int16_t idBefore;

    idLast = -1;
    idFirst = -1;
    idAfter = -1;
    idBefore = -1;
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == idPlayer && lppl->fStarbase != 0 && LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0) {
            if (lppl->id > idPlanet) {
                if (idAfter == -1) {
                    idAfter = lppl->id;
                }
            } else if (lppl->id < idPlanet) {
                idBefore = lppl->id;
            }
            if (idFirst == -1) {
                idFirst = lppl->id;
            }
            idLast = lppl->id;
        }
    }
    if (fNext != 0) {
        if (idAfter == -1) {
            return idFirst;
        }
        return idAfter;
    }
    if (idBefore == -1) {
        return idLast;
    }
    return idBefore;
}

void PlanetClick(int16_t x, int16_t y, int16_t sks, int16_t fRightBtn) {
    int16_t  bt;
    POINT16  pt;
    int16_t  ctile;
    int16_t  dy;
    RECT     rcTitle;
    int16_t  i;
    int16_t  xRel;
    uint16_t iCol;
    int16_t  iCur;
    TILE    *prgtile;
    RECT     rc;
    BTNT     btnt;
    HDC      hdc;
    TILE     tile;
    POINT16  ptNew;

    bt = 112;
    if (sel.grobj == grobjPlanet) {
        prgtile = rgtilePlanet;
        ctile = 6;
    } else {
        if (sel.grobj != grobjFleet) {
            return;
        }
        prgtile = rgtileShip;
        ctile = 7;
    }
    iCol = (uint32_t)x / 198;
    xRel = x - iCol * 198;
    if (xRel >= 4 && xRel < 194) {
        for (i = 0; i < ctile; i++) {
            if (prgtile[i].iCol >= iCol) {
                if (prgtile[i].iCol > iCol) {
                    return;
                }
                if (y >= prgtile[i].yTop && y < (prgtile[i].fPopped == 0 ? dyArial8 + 3 : prgtile[i].dyFull) + prgtile[i].yTop)
                    break;
            }
        }
        if (i != ctile) {
            pt.x = x;
            pt.y = y;
            rcTitle.top = prgtile[i].yTop;
            rcTitle.bottom = dyArial8 + 3 + rcTitle.top + 1;
            rcTitle.left = iCol * 198 + 4;
            rcTitle.right = rcTitle.left + 191;
            if (PtInRect(&rcTitle, PointFrom16(pt)) != 0 && fRightBtn == 0) {
                rc = rcTitle;
                rc.top++;
                rc.left = rc.right - 17;
                if (PtInRect(&rc, PointFrom16(pt)) != 0) {
                    OffsetRc(&rc, -1, 0);
                    if (prgtile[i].fPopped != 0) {
                        bt = bt;
                    } else {
                        bt |= 1;
                    }
                    InitBtnTrack(&btnt, hwndPlanet, NULL, &rc, bt, 0, 0, 0, NULL);
                    while (FTrackBtn(&btnt) != 0) {
                    }
                    if (btnt.fDown != 0) {
                        prgtile[i].fPopped = prgtile[i].fPopped == 0 ? 1 : 0;
                        ReflowColumn(prgtile[i].iCol, i, 1);
                    }
                } else {
                    if (prgtile[i].fPopped != 0) {
                        rcTitle.bottom = rcTitle.top + prgtile[i].dyFull + 1;
                    }
                    hdc = GetDC(hwndPlanet);
                    DrawFuzzyBorder(hdc, &rcTitle);
                    SetCapture(hwndPlanet);
                    ptNew = pt;
                    while (FGetMouseMove(&ptNew) != 0) {
                        if (pt.x != ptNew.x || pt.y != ptNew.y) {
                            DrawFuzzyBorder(hdc, &rcTitle);
                            OffsetRc(&rcTitle, ptNew.x - pt.x, ptNew.y - pt.y);
                            pt = ptNew;
                            DrawFuzzyBorder(hdc, &rcTitle);
                        }
                    }
                    DrawFuzzyBorder(hdc, &rcTitle);
                    ReleaseCapture();
                    ReleaseDC(hwndPlanet, hdc);
                    pt.x = ((rcTitle.right - rcTitle.left) >> 1) + rcTitle.left;
                    pt.y = ((rcTitle.bottom - rcTitle.top) >> 1) + rcTitle.top;
                    iCol = 0 > (1 >= pt.x / 198 ? pt.x / 198 : 1) ? 0 : 1 < pt.x / 198 ? 1 : pt.x / 198;
                    iCur = i;
                    for (i = 0; i < ctile && prgtile[i].iCol < iCol; i++) {
                    }
                    for (; i < ctile && prgtile[i].iCol == iCol; i++) {
                        dy = prgtile[i].fPopped == 0 ? dyArial8 + 3 : prgtile[i].dyFull;
                        if (pt.y < (dy >> 1) + prgtile[i].yTop)
                            break;
                    }
                    if (i == iCur || i == iCur + 1) {
                        i = prgtile[iCur].iCol;
                        if (iCol != i) {
                            prgtile[iCur].iCol = iCol;
                            ReflowColumn(iCol, iCur, 1);
                            if (iCol < i) {
                                ReflowColumn(i, iCur + 1, 1);
                            } else {
                                ReflowColumn(i, iCur, 1);
                            }
                        }
                    } else {
                        tile = prgtile[iCur];
                        if (i < iCur) {
                            memmove(prgtile + (i + 1), prgtile + i, (iCur - i) * sizeof(TILE));
                            iCur++;
                        } else {
                            memmove(prgtile + iCur, prgtile + (iCur + 1), (i - iCur - 1) * sizeof(TILE));
                            i--;
                        }
                        prgtile[i] = tile;
                        prgtile[i].iCol = iCol;
                        if (tile.iCol == iCol) {
                            ReflowColumn(iCol, i >= iCur ? iCur : i, 1);
                        } else {
                            ReflowColumn(iCol, i, 1);
                            ReflowColumn(tile.iCol, iCur, 1);
                        }
                    }
                }
            } else {
                if (sel.grobj == grobjFleet) {
                    switch (prgtile[i].grbit) {
                    default:
                        goto L_509a;
                    case 32:
                    case 256:
                    case 1:
                    case 16:
                        goto L_50b3;
                    }
                    return;
                }
            L_509a:
                if (prgtile[i].grbit != 4) {
                    if (sel.grobj != grobjPlanet) {
                        return;
                    }
                    switch (prgtile[i].grbit) {
                    case 1:
                    case 256:
                    case 64:
                    case 8:
                        ClickInPlanetOrders(pt, sks, 0, fRightBtn);
                    }
                    return;
                }
            L_50b3:
                ClickInShipOrders(pt, sks, 0, fRightBtn);
            }
        }
    }
    return;
}

HCURSOR ClickInPlanetOrders(POINT16 pt, int16_t sks, int16_t fCursor, int16_t fRightBtn) {
    int16_t i;
    int32_t rglQuan[3];
    int16_t iWarp;
    BTNT    btnt;
    int16_t t_call_53a2;

    if (sel.grobj != grobjPlanet) {
        return NULL;
    }
    if (fRightBtn != 0) {
        return NULL;
    }
    if (PtInRect(&rgrcRef[6], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        i = (int16_t)(pt.y - rgrcRef[6].top) / dyArial8;
        GlobalPD.grPopup = grPopupMineral;
        GlobalPD.rgi[0] = i;
        GlobalPD.rgi[2] = sel.pl.rgwtMin[i];
        GlobalPD.rgi[3] = (uint32_t)sel.pl.rgMinConc[i];
        EstMineralsMined(&sel.pl, rglQuan, -1, 0);
        GlobalPD.rgi[4] = rglQuan[i];
        GlobalPD.rgi[1] = sel.pl.fHomeworld;
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[7], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        GlobalPD.grPopup = grPopupPlanetIndustry;
        GlobalPD.idPlan = sel.pl.id;
        GlobalPD.fFactory = pt.y < rgrcRef[7].top + dyArial8 ? 0 : 1;
        if (GlobalPD.fFactory != 0) {
            GlobalPD.cMax = CMaxFactories(&sel.pl, idPlayer);
            GlobalPD.cCur = sel.pl.cFactories;
            GlobalPD.cOperate = CMaxOperableFactories(&sel.pl, idPlayer, 0);
        } else {
            GlobalPD.cMax = CMaxMines(&sel.pl, idPlayer);
            GlobalPD.cCur = sel.pl.cMines;
            GlobalPD.cOperate = CMaxOperableMines(&sel.pl, idPlayer, 0);
        }
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[8], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        GlobalPD.grPopup = grPopupResources;
        GlobalPD.idPlanet = sel.pl.id;
        t_call_53a2 = CResourcesAtPlanet(&sel.pl, idPlayer);
        GlobalPD.iPlanVal = t_call_53a2;
        GlobalPD.iPlanetVar = t_call_53a2;
        if (sel.pl.fNoResearch == 0) {
            GlobalPD.iPlanVal -= MulDiv(GlobalPD.iPlanetVar, rgplr[idPlayer].pctResearch, 100);
        }
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[9], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        GlobalPD.grPopup = grPopupPlanet;
        GlobalPD.idPlanet = sel.pl.id;
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[10], PointFrom16(pt)) != 0) {
        if (sel.pl.cDefenses == 0) {
            return NULL;
        }
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        FGetBestDefensePart(&GlobalPD.part);
        GlobalPD.grPopup = grPopupComponent;
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[11], PointFrom16(pt)) != 0) {
        if (sel.pl.iScanner == 31 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
            return NULL;
        }
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsRaceCannotBuildPlanetaryScannersStarbasesHave, szPopupBuffer);
        } else {
            LookupBestPlanetaryScanner(&GlobalPD.part);
            GlobalPD.grPopup = grPopupComponent;
        }
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[13], PointFrom16(pt)) != 0) {
        iWarp = IWarpMAFromLppl(&sel.pl, NULL);
        if (iWarp == 0) {
            return NULL;
        }
        if (fCursor != 0) {
            return hcurHand;
        }
        InitBtnTrack(&btnt, hwndPlanet, NULL, &rgrcRef[13], 8, 80, gd.fSetMassMode, 1, PszGetCompressedString(idsSetDest));
        while (FTrackBtn(&btnt) != 0) {
        }
        gd.fSetMassMode ^= btnt.fDown;
    } else if (PtInRect(&rgrcRef[14], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        GlobalPD.grPopup = grPopupShdef;
        GlobalPD.lpshdef = rglpshdefSB[idPlayer] + sel.pl.isb;
        GlobalPD.fHideCounts = 0;
        GlobalPD.fShowDamage = 1;
        GlobalPD.fToken = 0;
        GlobalPD.fSummary = 0;
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[16], PointFrom16(pt)) != 0) {
        iWarp = IWarpMAFromLppl(&sel.pl, NULL);
        if (iWarp == 0) {
            return NULL;
        }
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        GlobalPD.part.hs.grhst = hstSpecialSB;
        GlobalPD.part.hs.iItem = iWarp + 2;
        FLookupPart(&GlobalPD.part);
        GlobalPD.grPopup = grPopupComponent;
        Popup(hwndPlanet, pt.x, pt.y);
    } else if (PtInRect(&rgrcRef[15], PointFrom16(pt)) != 0) {
        ClickInShipOrders(pt, sks, 0, fRightBtn);
    } else if (PtInRect(&rgrcRef[17], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurHand;
        }
        InitBtnTrack(&btnt, hwndPlanet, NULL, &rgrcRef[17], 8, 80, gd.fSetRouteMode, 1, PszGetCompressedString(idsRoute2));
        while (FTrackBtn(&btnt) != 0) {
        }
        gd.fSetRouteMode ^= btnt.fDown;
    }
    return NULL;
}

void EnsureTileSize(int16_t fSmallTiles) {
    int16_t    iMul;
    int16_t    i;
    GrobjClass grobjSav;

    if (fSmallTiles != gd.fSmallTileMode) {
        gd.fSmallTileMode = fSmallTiles;
        iMul = fSmallTiles == 0 ? 1 : -1;
        for (i = 0; i < 6; i++) {
            if (rgtilePlanet[i].grbit == 64) {
                rgtilePlanet[i].dyFull += (dyArial8 + 2) * 2 * iMul;
            }
            if (rgtilePlanet[i].grbit == 4) {
                rgtilePlanet[i].dyFull += (dyArial8 + 4) * 2 * iMul;
            }
            if (rgtilePlanet[i].grbit == 128) {
                rgtilePlanet[i].dyFull += 10 * iMul;
            }
        }
        for (i = 0; i < 7; i++) {
            if (rgtileShip[i].grbit == 1) {
                rgtileShip[i].dyFull += (dyArial8 * 4 + 2) * iMul;
            }
            if (rgtileShip[i].grbit == 512) {
                rgtileShip[i].dyFull += ((dyArial8 + 2) * 2 + 4 + dyArial8) * iMul;
            }
            if (rgtileShip[i].grbit == 32) {
                rgtileShip[i].dyFull += (dyArial8 + 9) * iMul;
            }
            if (rgtileShip[i].grbit == 4) {
                rgtileShip[i].dyFull += (dyArial8 + 4) * 2 * iMul;
            }
            if (rgtileShip[i].grbit == 128) {
                rgtileShip[i].dyFull += 10 * iMul;
            }
            if (rgtileShip[i].grbit == 256) {
                rgtileShip[i].dyFull += iMul * 2;
            }
            if (rgtileShip[i].grbit == 64) {
                rgtileShip[i].dyFull += 6 * iMul;
            }
        }
        grobjSav = sel.grobj;
        sel.grobj = grobjPlanet;
        for (i = 0; i < 4; i++) {
            ReflowColumn(i, -1, 0);
        }
        sel.grobj = grobjFleet;
        for (i = 0; i < 4; i++) {
            ReflowColumn(i, -1, 0);
        }
        sel.grobj = grobjSav;
    }
    return;
}

void ReflowColumn(int16_t iCol, int16_t iTile, int16_t fRedraw) {
    HDC     hdc;
    int16_t yTop;
    int16_t ctile;
    int16_t i;
    int16_t grbit;
    TILE   *ptile;
    RECT    rc;

    grbit = 0;
    if (sel.grobj == grobjFleet) {
        ptile = rgtileShip;
        ctile = 7;
    } else {
        ptile = rgtilePlanet;
        ctile = 6;
    }
    yTop = 4;
    if (iTile == -1) {
        for (i = 0; i < ctile && ptile[i].iCol != iCol; i++) {
        }
        if (i == ctile) {
            return;
        }
        iTile = i;
    } else {
        for (i = 0; i < iTile; i++) {
            if (ptile[i].iCol == iCol) {
                yTop += (ptile[i].fPopped == 0 ? dyArial8 + 3 : ptile[i].dyFull) + 4;
            }
        }
    }
    if (fRedraw != 0) {
        GetClientRect(hwndPlanet, &rc);
        rc.top = yTop;
        rc.left = 198 * iCol + 4;
        rc.right = rc.left + 191;
        hdc = GetDC(hwndPlanet);
        FillRect(hdc, &rc, hbrButtonFace);
    }
    for (; iTile < ctile && ptile[iTile].iCol == iCol; iTile++) {
        ptile[iTile].yTop = yTop;
        ptile[iTile].fFixCtls = 1;
        grbit |= ptile[iTile].grbit;
        yTop += (ptile[iTile].fPopped == 0 ? dyArial8 + 3 : ptile[iTile].dyFull) + 4;
    }
    if (fRedraw != 0) {
        DrawPlanShip(hdc, grbit);
        ReleaseDC(hwndPlanet, hdc);
    }
    return;
}

int16_t IBestTerraform(PLANET *lppl, int16_t fHelp) {
    int16_t iSave;
    int16_t iBest;
    int16_t rgMax[3];
    int16_t pctT;
    int16_t i;
    int16_t iPlr;
    int16_t iEnv;
    int16_t pctCur;
    int16_t rgMin[3];
    int16_t rgpctBest[3];
    int16_t rgCost[3];
    int16_t iPlrSav;

    iPlrSav = idPlayer;
    iPlr = lppl->iPlayer;
    if (iPlr == -1) {
        return 0;
    }
    idPlayer = iPlr;
    if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, fHelp) == 0) {
        idPlayer = iPlrSav;
        return 0;
    }
    pctCur = PctPlanetDesirability(lppl, iPlr);
    for (i = 0; i < 3; i++) {
        if (rgMin[i] != -1) {
            iEnv = rgMin[i];
        } else if (rgMax[i] != -1) {
            iEnv = rgMax[i];
        } else {
            rgpctBest[i] = 0;
            continue;
        }
        iSave = lppl->rgEnvVar[i];
        lppl->rgEnvVar[i] = LOBYTE(iEnv);
        pctT = PctPlanetDesirability(lppl, iPlr) - pctCur;
        if (pctT < 0) {
            pctT = -pctT;
        }
        rgpctBest[i] = (int16_t)(100 * pctT) / abs(iSave - iEnv) + 1;
        lppl->rgEnvVar[i] = LOBYTE(iSave);
    }
    iSave = 0;
    for (i = 1; i < 3; i++) {
        if (rgpctBest[i] > rgpctBest[iSave]) {
            iSave = i;
        }
    }
    if (rgMin[iSave] != -1) {
        iBest = -(iSave + 1);
    } else {
        iBest = iSave + 1;
    }
    idPlayer = iPlrSav;
    return iBest;
}

char *PszCalcEnvVar(EnvType iEnv, int16_t iVar) {
    switch (iEnv) {
    case Gravity:
    default:
        return PszCalcGravity(iVar);
    case Temperature:
        _wsprintf(szWork, "%d%cC", iVar * 4 - 200, 186);
        break;
    case Radiation:
        _wsprintf(szWork, "%dmR", iVar);
    }
    return szWork;
}

char *PszCalcGravity(int16_t iGravity) {
    int16_t d;
    int16_t iVal;

    d = abs(iGravity - 50);
    if (d <= 25) {
        iVal = d * 4 + 100;
    } else {
        iVal = (d - 25) * 24 + 200;
    }
    if (iGravity < 50) {
        iVal = 10000 / iVal;
    }
    _wsprintf(szWork, "%d.%02dg", iVal / 100, iVal % 100);
    return szWork;
}

void HandleFocusState(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    if ((lpdis->itemState & 0x10) != 0) {
        FrameRect(lpdis->hDC, &lpdis->rcItem, hbr50Screen);
    }
    return;
}

void DrawCBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    int16_t fListbox;
    int16_t fSelected;
    RECT    rc;

    fSelected = lpdis->itemState & 1;
    rc = lpdis->rcItem;
    fListbox = lpdis->hwndItem == hwndFleetCompLB || lpdis->hwndItem == hwndPlanetProdLB || inflate > 0;
    if (inflate > 0) {
        inflate = -inflate;
    }
    SendMessage(lpdis->hwndItem, fListbox == 0 ? CB_GETLBTEXT : LB_GETTEXT, lpdis->itemID, (LPARAM)szWork);
    DrawProductionItem(lpdis->hDC, &rc, szWork, inflate, fSelected, fListbox);
    HandleFocusState(lpdis, inflate + 2);
    return;
}

void DrawProductionItem(HDC hdc, RECT *prc, char *psz, int16_t inflate, int16_t fSelected, int16_t fListbox) {
    HFONT    hfntSav;
    char    *pch;
    int16_t  ichT;
    COLORREF cr;
    int16_t  pctDmg;
    char     szT[20];
    RECT     rcIn;
    int16_t  ich;
    int16_t  fDoubleDraw;
    COLORREF crForeSav;
    int16_t  fFleet;
    RECT     rcDraw;
    int16_t  dx;
    HBRUSH   hbr;
    int16_t  fItalic;
    int16_t  cch;
    int16_t  bkSav;
    RECT     rc;

    fItalic = 0;
    fDoubleDraw = 0;
    fFleet = 0;
    rcIn = *prc;
    rc = rcIn;
    if (fSelected == 0) {
        hbr = hbrWindow;
        switch ((int16_t)(int8_t)*psz) {
        default:
            cr = 0xff;
            break;
        case 73:
            fItalic = 1;
        case 32:
        LDefCase:
            if (crWindow == 0) {
                cr = 0xffffff;
                break;
            }
            cr = 0;
            break;
        case 80:
            fDoubleDraw = 1;
            pctDmg = (int16_t)(int8_t)psz[1];
        case 81:
            fFleet = 1;
            goto LDefCase;
        case 42:
            cr = 32512;
            break;
        case 35:
            cr = 8323072;
            break;
        case 38:
            cr = 8355711;
        }
    } else {
        cr = crWindow;
        switch ((int16_t)(int8_t)*psz) {
        default:
            hbr = hbrRed;
            break;
        case 73:
            fItalic = 1;
        case 32:
        LDefCaseSel:
            if (crWindow == 0) {
                hbr = GetStockObject(WHITE_BRUSH);
                break;
            }
            hbr = GetStockObject(BLACK_BRUSH);
            break;
        case 80:
            fDoubleDraw = 1;
            pctDmg = (int16_t)(int8_t)psz[1];
        case 81:
            fFleet = 1;
            goto LDefCaseSel;
        case 42:
            hbr = hbrGreen;
            break;
        case 35:
            hbr = hbrBlue;
            break;
        case 38:
            hbr = hbrGray;
        }
    }
    if (fListbox == 2) {
        hbr = hbrButtonFace;
    }
    FillRect(hdc, &rcIn, hbr);
    if (fDoubleDraw != 0) {
        rcDraw = rcIn;
        dx = rcIn.right - rcIn.left;
        dx = (int16_t)(dx * pctDmg) / 100;
        rcDraw.right = rcDraw.left + dx;
        FillRect(hdc, &rcDraw, hbrRed);
    }
    if (inflate != 0) {
        InflateRect(&rcIn, -2, -1);
    }
    if (fListbox == 0) {
        ich = 1;
    } else if (fFleet != 0) {
        ich = 7;
    } else {
        ich = 6;
        if (((int16_t)(int8_t)psz[1] - 0x20 & 2) != 0) {
            fItalic = 1;
        }
    }
    crForeSav = SetTextColor(hdc, cr);
    bkSav = SetBkMode(hdc, TRANSPARENT);
    if (fItalic != 0) {
        hfntSav = SelectObject(hdc, rghfontArial8[3]);
    }
    pch = psz + ich;
    cch = strlen(pch) + 1;
    do {
        cch--;
        dx = LOWORD(GetTextExtent(hdc, pch, cch));
    } while (dx > rcIn.right - rcIn.left);
    TextOut(hdc, rcIn.left, rcIn.top, pch, cch);
    if (fItalic != 0) {
        SelectObject(hdc, hfntSav);
    }
    if (ich >= 6) {
        if (((int16_t)(int8_t)psz[ich - 5] - 0x20 & 2) != 0) {
            if ((int16_t)(int8_t)psz[ich - 1] == 42) {
                ich = CchGetString(idsNeeded, szT);
                goto LRightOut;
            }
            CchGetString(idsUpTo, szT);
        } else {
            szT[0] = 0;
        }
        ich = strlen(szT);
        for (ichT = 2 - fFleet; ichT < 6 && (int16_t)(int8_t)psz[ichT + fFleet] == 32; ichT++) {
        }
        strncpy(&szT[ich], psz + (ichT + fFleet), 6 - ichT);
        ich += 6 - ichT;
        if (fFleet == 0 && ((int16_t)(int8_t)psz[fDoubleDraw + 1] - 0x20 & 1) != 0) {
            szT[ich++] = '%';
        }
    LRightOut:
        RightTextOut(hdc, rcIn.right, rcIn.top, szT, ich, 0);
    }
    SetTextColor(hdc, crForeSav);
    SetBkMode(hdc, bkSav);
    return;
}

void FillPlanetProdLB(HWND hwnd, PLPROD *lpplprod, PLANET *lppl) {
    int16_t fMinimal;
    int32_t rgwtMin[4];
    int16_t i;
    int16_t cItem;
    char    szTemp[80];
    int32_t resCost;
    char   *psz;
    char    ch;
    PROD   *lpprod;
    int16_t etaLast;
    int16_t etaFirst;

    fMinimal = lppl == 0 ? 0 : 1;
    if (fMinimal == 0) {
        lppl = &sel.pl;
        if (hwnd == 0) {
            hwnd = hwndPlanetProdLB;
        }
        SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
    }
    if (lpplprod == 0) {
        lpplprod = lppl->lpplprod;
    }
    if (lpplprod == 0 || lpplprod->iprodMac == 0) {
        psz = PszGetCompressedString(idsQueueEmpty);
    } else {
        if (hwndProdDlg == 0)
            goto NoMsg;
        psz = PszGetCompressedString(idsTopQueue);
    }
    if (fMinimal == 0) {
        SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)psz);
    } else if (psz != szWork) {
        strcpy(szWork, psz);
    }
NoMsg:
    if (lpplprod != 0) {
        resCost = 0;
        for (i = 0; i < 4; i++) {
            rgwtMin[i] = 0;
        }
        i = 0;
        lpprod = lpplprod->rgprod;
        while (i < lpplprod->iprodMac) {
            psz = PszNameProdItem(lpprod);
            EstimateItemProdSched(lppl, lpplprod, i, &etaFirst, &etaLast);
            if ((etaFirst == 0 && etaLast == 0) || (etaFirst == -1 && etaLast == -1)) {
                if (fMinimal != 0)
                    goto L_680c;
                ch = '&';
            } else if ((etaFirst > 1 && etaFirst < 100) || (etaFirst == 100 && lpprod->grobj == grobjPlanet && lpprod->iItem < mdIdleFactory)) {
                ch = ' ';
            } else if (etaFirst == 1 && etaLast == 1) {
                ch = '*';
            } else if (etaFirst < 100) {
                ch = '#';
            } else {
                ch = '!';
            }
            cItem = lpprod->cItem;
            _wsprintf(szTemp, "%c%5d%s", (int16_t)(int8_t)ch, cItem, psz);
            if (lpprod->grobj == grobjPlanet) {
                if (lpprod->iItem < mdIdleFactory) {
                    szTemp[1] += 2;
                    if (lpprod->iItem == iobjAlchemy) {
                        szTemp[5] = '*';
                    }
                }
                switch (lpprod->iItem) {
                case mdIdleTerraform:
                case iobjMinTerraform:
                case iobjMaxTerraform:
                    szTemp[1]++;
                }
            }
            if (fMinimal != 0) {
                strcpy(szWork, szTemp);
                return;
            }
            SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)szTemp);
        L_680c:
            i++;
            lpprod++;
        }
        if (fMinimal != 0) {
            CchGetString(idsQueueEmpty, szWork);
        }
    }
    return;
}

int16_t PctPlanetCapacity(PLANET *lppl) {
    int32_t pctCap;
    int32_t lPopMax;

    lPopMax = CalcPlanetMaxPop(lppl->id, idPlayer);
    if (lPopMax <= 0) {
        return 0;
    }
    pctCap = (int32_t)((int32_t)((uint32_t)(lppl->rgwtMin[3] * 100) + (int32_t)(lPopMax / 2)) / lPopMax);
    if (pctCap >= 1000) {
        pctCap = 999;
    }
    return LOWORD(pctCap);
}

int16_t PctPlanetOptValue(PLANET *lppl, int16_t iPlr) {
    int16_t rgMax[3];
    int16_t i;
    int16_t rgMin[3];
    int16_t pctDesire;
    int16_t rgCost[3];
    int16_t rgiValSav[3];
    int16_t iNewVal;

    if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) == 0) {
        return PctPlanetDesirability(lppl, iPlr);
    }
    for (i = 0; i < 3; i++) {
        rgiValSav[i] = lppl->rgEnvVar[i];
        if (rgplr[iPlr].rgEnvVarMin[i] != -1 && lppl->rgEnvVar[i] != rgplr[iPlr].rgEnvVar[i]) {
            iNewVal = -1;
            if (lppl->rgEnvVar[i] < rgplr[iPlr].rgEnvVar[i]) {
                if (rgMax[i] > lppl->rgEnvVar[i]) {
                    iNewVal = rgplr[iPlr].rgEnvVar[i] >= rgMax[i] ? rgMax[i] : rgplr[iPlr].rgEnvVar[i];
                }
            } else if (rgMin[i] != -1 && rgMin[i] < lppl->rgEnvVar[i]) {
                iNewVal = rgplr[iPlr].rgEnvVar[i] <= rgMin[i] ? rgMin[i] : rgplr[iPlr].rgEnvVar[i];
            }
            if (iNewVal != -1) {
                lppl->rgEnvVar[i] = LOBYTE(iNewVal);
            }
        }
    }
    pctDesire = PctPlanetDesirability(lppl, idPlayer);
    for (i = 0; i < 3; i++) {
        lppl->rgEnvVar[i] = LOBYTE(rgiValSav[i]);
    }
    return pctDesire;
}

int16_t PctPlanetDesirability(PLANET *lppl, int16_t iPlr) {
    int16_t iMin;
    int16_t d;
    int16_t iMax;
    int32_t pctNeg;
    int16_t iPref;
    int16_t i;
    int16_t dPenalty;
    int32_t pctPos;
    int16_t pctVar;
    int16_t iPlanet;
    int32_t pctMod;

    pctPos = 0;
    pctNeg = 0;
    pctMod = 10000;
    for (i = 0; i < 3; i++) {
        iPlanet = lppl->rgEnvVar[i];
        iPref = rgplr[iPlr].rgEnvVar[i];
        iMin = rgplr[iPlr].rgEnvVarMin[i];
        iMax = rgplr[iPlr].rgEnvVarMax[i];
        if (iMax < 0) {
            pctPos += 10000;
        } else if (iPlanet >= iMin && iPlanet <= iMax) {
            pctVar = abs(iPlanet - iPref) * 100;
            if (iPlanet < iPref) {
                d = iPref - iMin;
                pctVar /= d;
                dPenalty = (iPref - iPlanet) * 2 - d;
            } else {
                d = iMax - iPref;
                pctVar /= d;
                dPenalty = (iPlanet - iPref) * 2 - d;
            }
            pctVar = 100 - pctVar;
            pctPos += (uint32_t)(pctVar * pctVar);
            if (dPenalty > 0) {
                pctMod = (uint32_t)(pctMod * (int16_t)(d * 2 - dPenalty));
                pctMod = (int32_t)(pctMod / (int16_t)(d * 2));
            }
        } else if (iPlanet < iMin) {
            pctNeg += 15 >= iMin - iPlanet ? (int16_t)(iMin - iPlanet) : 15;
        } else {
            pctNeg += 15 >= iPlanet - iMax ? (int16_t)(iPlanet - iMax) : 15;
        }
    }
    if (pctNeg != 0) {
        return -LOWORD(pctNeg);
    }
    pctPos = (int32_t)((long double)sqrt((double)((long double)pctPos / 3.0)) + 0.9);
    pctPos = (int32_t)(pctPos * pctMod) / 10000;
    return LOWORD(pctPos);
}

int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr) {
    PLANET  pl;
    int32_t lMaxPop;
    int32_t pctDesire;
    int16_t ihuldef;

    FLookupPlanet(idpl, &pl);
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        if (pl.iPlayer != iplr || pl.fStarbase == 0) {
            return 0;
        }
        ihuldef = rglpshdefSB[iplr][pl.isb].hul.ihuldef - 32;
        lMaxPop = rglPopMac[ihuldef];
    } else {
        pctDesire = PctPlanetDesirability(&pl, iplr);
        if (pctDesire < 5) {
            lMaxPop = 500;
        } else {
            lMaxPop = (uint32_t)(pctDesire * 100);
        }
        if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raCheapCol) {
            lMaxPop -= (int32_t)(lMaxPop / 2);
        } else if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raNone) {
            lMaxPop += (int32_t)(lMaxPop / 5);
        }
    }
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceOBRM) != 0) {
        lMaxPop += (int32_t)(lMaxPop / 10);
    }
    return lMaxPop;
}

int16_t CMaxMines(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsMineOperate);
    cMax = (int32_t)(lPopMax * iEff) / 100;
    if (cMax < 10) {
        cMax = 10;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return LOWORD(cMax);
}

int16_t CMaxOperableMines(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;
    int16_t iEff;

    cMax = CMaxMines(lppl, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsMineOperate);
    lPop = lppl->rgwtMin[3];
    if (fNextYear != 0) {
        lPop += ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)(lPop * iEff) / 100;
    cMax = cMax < cCur ? cMax : LOWORD(cCur);
    if (cMax <= 0) {
        cMax = 1;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return cMax;
}

int16_t CMinesOperating(PLANET *lppl) {
    int16_t iplr;
    int16_t cMinesOp;
    int16_t cMines;
    double  t_call_7452;

    iplr = lppl->iPlayer;
    if (iplr == -1) {
        return 0;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        t_call_7452 = sqrt((double)lppl->rgwtMin[3]);
        return LOWORD((int32_t)t_call_7452);
    }
    cMines = lppl->cMines;
    cMinesOp = CMaxOperableMines(lppl, lppl->iPlayer, 0);
    if (cMines > cMinesOp) {
        cMines = cMinesOp;
    }
    return cMines;
}

int16_t CFactoriesOperating(PLANET *lppl) {
    int16_t iplr;
    int16_t cFacts;
    int16_t cFactsOp;

    iplr = lppl->iPlayer;
    if (iplr == -1) {
        return 0;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        return 0;
    }
    cFacts = lppl->cFactories;
    cFactsOp = CMaxOperableFactories(lppl, lppl->iPlayer, 0);
    if (cFacts > cFactsOp) {
        cFacts = cFactsOp;
    }
    return cFacts;
}

int16_t CMaxFactories(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsFactOperate);
    cMax = (int32_t)(lPopMax * iEff) / 100;
    if (cMax < 10) {
        cMax = 10;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return LOWORD(cMax);
}

int16_t CMaxOperableFactories(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;
    int16_t iEff;

    cMax = CMaxFactories(lppl, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsFactOperate);
    lPop = lppl->rgwtMin[3];
    if (fNextYear != 0) {
        lPop += ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)(lPop * iEff) / 100;
    cMax = cMax < cCur ? cMax : LOWORD(cCur);
    if (cMax <= 0) {
        cMax = 1;
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return cMax;
}

int16_t CMaxDefenses(PLANET *lppl, int16_t iplr) {
    int16_t cMax;
    int16_t pctDesire;

    pctDesire = PctPlanetDesirability(lppl, iplr);
    cMax = 100 < (10 <= pctDesire * 4 ? pctDesire * 4 : 10) ? 100 : 10 > pctDesire * 4 ? 10 : pctDesire * 4;
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return cMax;
}

int16_t CMaxOperableDefenses(PLANET *lppl, int16_t iplr, int16_t fNextYear) {
    int16_t cMax;
    int32_t cCur;
    int32_t lPop;

    cMax = CMaxDefenses(lppl, iplr);
    lPop = lppl->rgwtMin[3];
    if (fNextYear != 0) {
        lPop += ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)((lPop + 24) / 25);
    if (cCur > 1000) {
        cCur = 1000;
    }
    cMax = cMax >= LOWORD(cCur) ? LOWORD(cCur) : cMax;
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        cMax = 0;
    }
    return cMax;
}

int16_t CResourcesAtPlanet(PLANET *lppl, int16_t iplr) {
    int16_t cRes;
    int32_t lPop;
    int16_t cFact;
    int32_t lPopMax;
    int16_t iEff;
    int16_t pctVal;
    int16_t iEnergy;

    if (lppl->rgwtMin[3] == 0) {
        return 0;
    }
    iEff = GetRaceStat(&rgplr[iplr], rsResGen);
    lPop = lppl->rgwtMin[3];
    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    if (lPop > lPopMax) {
        lPop = (int32_t)((lPop - lPopMax) / 2) + lPopMax;
        if (lPop > (int32_t)(lPopMax * 2)) {
            lPop = (int32_t)(lPopMax * 2);
        }
    }
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh) {
        iEnergy = rgplr[iplr].rgTech[0];
        pctVal = PctPlanetDesirability(lppl, iplr);
        if (iEnergy < 1) {
            iEnergy = 1;
        }
        if (pctVal < 25) {
            pctVal = 25;
        }
        cRes = LOWORD((int32_t)((long double)sqrt((double)((long double)lPop * iEnergy / iEff)) * pctVal / 10 + 0.999));
    } else {
        cRes = LOWORD((int32_t)(lPop / iEff));
        cFact = CMaxOperableFactories(lppl, iplr, 0);
        if (lppl->cFactories < cFact) {
            cFact = lppl->cFactories;
        }
        iEff = GetRaceStat(&rgplr[iplr], rsFactProd);
        cRes += LOWORD((int32_t)((uint32_t)(cFact * iEff) + 9) / 10);
    }
    if (cRes == 0) {
        cRes = 1;
    }
    return cRes;
}

int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo) {
    int16_t fTwo;
    int16_t iWarp;
    int16_t i;
    HUL    *lphul;
    int16_t iNew;

    iWarp = 0;
    fTwo = 0;
    if (pfTwo != 0) {
        *pfTwo = 0;
    }
    if (lppl->iPlayer == -1 || lppl->fStarbase == 0) {
        return 0;
    }
    if (lppl->iPlayer != idPlayer && idPlayer != -1 && rglpshdefSB[lppl->iPlayer][lppl->isb].det != detAll) {
        return 0;
    }
    lphul = &rglpshdefSB[lppl->iPlayer][lppl->isb].hul;
    for (i = 0; i < lphul->chs; i++) {
        if (lphul->rghs[i].grhst == hstSpecialSB && lphul->rghs[i].cItem > 0 && lphul->rghs[i].iItem >= 7 && lphul->rghs[i].iItem <= 15) {
            iNew = lphul->rghs[i].iItem - 2;
            if (iNew > iWarp) {
                fTwo = 0;
                iWarp = iNew;
            } else if (iNew == iWarp) {
                fTwo = 1;
            }
        }
    }
    if (pfTwo != 0) {
        *pfTwo = fTwo;
    }
    return iWarp;
}

int16_t StargateRangeFromLppl(PLANET *lppl, int16_t iplr, int16_t ish) {
    int16_t i;
    HUL    *lphul;
    PART    part;

    if (lppl != 0) {
        if (lppl->iPlayer == -1 || lppl->fStarbase == 0) {
            return 0;
        }
        lphul = &rglpshdefSB[lppl->iPlayer][lppl->isb].hul;
    } else {
        lphul = &rglpshdefSB[iplr][ish].hul;
    }
    for (i = 0; i < lphul->chs; i++) {
        if (lphul->rghs[i].grhst == hstSpecialSB && lphul->rghs[i].cItem > 0 && lphul->rghs[i].iItem >= 0 && lphul->rghs[i].iItem <= 6) {
            part.hs = lphul->rghs[i];
            FLookupPart(&part);
            if (part.pspecialsb->grAbility2 == -1) {
                return 10000;
            }
            return part.pspecialsb->grAbility2;
        }
    }
    return 0;
}

int16_t FProdIsTerra(PROD *lpprod) {
    if (lpprod->grobj == grobjPlanet) {
        switch (lpprod->iItem) {
        case mdIdleTerraform:
        case iobjMinTerraform:
        case iobjMaxTerraform:
            return 1;
        }
    }
    return 0;
}

int16_t IpctCanTerraformLppl(PLANET *lppl) {
    int16_t rgMax[3];
    int16_t i;
    int16_t rgMin[3];
    int16_t rgCost[3];
    int16_t ipct;

    if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) == 0) {
        return 0;
    }
    ipct = 0;
    for (i = 0; i < 3; i++) {
        if (rgMin[i] != -1) {
            ipct += lppl->rgEnvVar[i] - rgMin[i];
        }
        if (rgMax[i] != -1) {
            ipct += rgMax[i] - lppl->rgEnvVar[i];
        }
    }
    return ipct;
}

int16_t FCanTerraformLppl(PLANET *lppl, int16_t *rgEnvMin, int16_t *rgEnvMax, int16_t *rgEnvCost, int16_t fHelp) {
    int16_t fRet;
    int16_t i;
    int16_t rgMove[3];
    int16_t iPlrSav;
    PART    part;
    int16_t dMin;
    int16_t dMax;
    int16_t dCur;
    int16_t ienvIdeal;

    iPlrSav = idPlayer;
    if (idPlayer == -1) {
        idPlayer = lppl->iPlayer;
    }
    part.hs.grhst = hstTerra;
    for (i = 7; i >= 0; i--) {
        part.hs.iItem = i;
        if (FLookupPart(&part) == 1)
            break;
    }
    if (i >= 0) {
        fRet = 1;
        for (i = 0; i < 3; i++) {
            rgMove[i] = part.pterra->grAbility;
            rgEnvCost[i] = part.pterra->resCost;
        }
    } else {
        fRet = 0;
        for (i = 0; i < 3; i++) {
            rgMove[i] = 0;
        }
    }
    for (i = 3; i >= 0; i--) {
        part.hs.iItem = i + 8;
        if (FLookupPart(&part) == 1)
            break;
    }
    if (i >= 0 && part.pterra->grAbility > rgMove[0]) {
        fRet = 1;
        rgMove[0] = part.pterra->grAbility;
        *rgEnvCost = part.pterra->resCost;
    }
    for (i = 3; i >= 0; i--) {
        part.hs.iItem = i + 12;
        if (FLookupPart(&part) == 1)
            break;
    }
    if (i >= 0 && part.pterra->grAbility > rgMove[1]) {
        fRet = 1;
        rgMove[1] = part.pterra->grAbility;
        rgEnvCost[1] = part.pterra->resCost;
    }
    for (i = 3; i >= 0; i--) {
        part.hs.iItem = i + 16;
        if (FLookupPart(&part) == 1)
            break;
    }
    if (i >= 0 && part.pterra->grAbility > rgMove[2]) {
        fRet = 1;
        rgMove[2] = part.pterra->grAbility;
        rgEnvCost[2] = part.pterra->resCost;
    }
    if (fRet == 0) {
        idPlayer = iPlrSav;
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (rgMove[i] == 0 || rgplr[idPlayer].rgEnvVarMin[i] == -1) {
            rgEnvMax[i] = -1;
            rgEnvMin[i] = -1;
        } else {
            rgEnvMin[i] = lppl->rgEnvVarOrig[i] - rgMove[i];
            rgEnvMax[i] = lppl->rgEnvVarOrig[i] + rgMove[i];
            if (rgEnvMin[i] >= lppl->rgEnvVar[i]) {
                rgEnvMin[i] = -1;
            } else {
                rgEnvMin[i] = 1 <= rgEnvMin[i] ? rgEnvMin[i] : 1;
            }
            if (rgEnvMax[i] <= lppl->rgEnvVar[i]) {
                rgEnvMax[i] = -1;
            } else {
                rgEnvMax[i] = 99 >= rgEnvMax[i] ? rgEnvMax[i] : 99;
            }
            if (fHelp != 0) {
                if (lppl->rgEnvVar[i] == rgplr[idPlayer].rgEnvVar[i]) {
                    rgEnvMax[i] = -1;
                    rgEnvMin[i] = -1;
                } else if (lppl->rgEnvVar[i] > rgplr[idPlayer].rgEnvVar[i]) {
                    rgEnvMax[i] = -1;
                    if (rgEnvMin[i] != -1) {
                        rgEnvMin[i] = rgEnvMin[i] <= rgplr[idPlayer].rgEnvVar[i] ? rgplr[idPlayer].rgEnvVar[i] : rgEnvMin[i];
                    }
                } else {
                    rgEnvMin[i] = -1;
                    if (rgEnvMax[i] != -1) {
                        rgEnvMax[i] = rgEnvMax[i] >= rgplr[idPlayer].rgEnvVar[i] ? rgplr[idPlayer].rgEnvVar[i] : rgEnvMax[i];
                    }
                }
            } else {
                ienvIdeal = rgplr[idPlayer].rgEnvVar[i];
                dCur = abs(lppl->rgEnvVar[i] - ienvIdeal);
                if (rgEnvMin[i] != -1) {
                    dMin = abs(rgEnvMin[i] - ienvIdeal);
                } else {
                    dMin = 0;
                }
                if (rgEnvMax[i] != -1) {
                    dMax = abs(rgEnvMax[i] - ienvIdeal);
                } else {
                    dMax = 0;
                }
                if (dCur >= dMin && dCur >= dMax) {
                    rgEnvMax[i] = -1;
                    rgEnvMin[i] = -1;
                } else if (dMin >= dMax) {
                    rgEnvMax[i] = -1;
                } else {
                    rgEnvMin[i] = -1;
                }
            }
        }
    }
    for (i = 0; i < 3 && (rgEnvMax[i] == -1 && rgEnvMin[i] == -1); i++) {
    }
    idPlayer = iPlrSav;
    if (i != 3) {
        return 1;
    }
    return 0;
}

void UninhabitPlanet(PLANET *lppl) {
    int16_t i;

    if (lppl->iPlayer >= 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raTerra) {
        for (i = 0; i < 3; i++) {
            lppl->rgEnvVar[i] = lppl->rgEnvVarOrig[i];
        }
    }
    lppl->iPlayer = -1;
    lppl->rgwtMin[3] = 0;
    if (lppl->lpplprod != 0) {
        FreePl((PL *)lpPlanets[lppl->id].lpplprod);
        lpPlanets[lppl->id].lpplprod = NULL;
        lppl->lpplprod = NULL;
    }
    lppl->fNoResearch = 0;
    lppl->fStarbase = 0;
    lppl->cDefenses = 0;
    lppl->iScanner = 31;
    lppl->lStarbase = 0;
    return;
}

int16_t PctCloakFromHuldef(HUL *lphul, int16_t iplr, int16_t *ppctSteal) {
    int16_t chs;
    HS     *lphs;
    int32_t cPts;
    int16_t cScore;
    int16_t j;

    cPts = 0;
    chs = lphul->chs;
    if (iplr != -1 && lphul->ihuldef >= ihuldefOrbitalFort && GetRaceGrbit(&rgplr[iplr], ibitRaceISB) != 0) {
        cPts = 40;
    } else {
        cPts = 0;
    }
    if (iplr != -1 && GetRaceStat(&rgplr[iplr], rsMajorAdv) == raStealth) {
        cPts += 300;
    }
    if (ppctSteal != 0) {
        *ppctSteal = 0;
    }
    j = 0;
    lphs = lphul->rghs;
    while (j < chs) {
        cPts += CPtsCloakFromLphs(lphs);
        if (lphs->grhst == hstScanner && ppctSteal != 0) {
            if (lphs->iItem == iscannerPickPocketScanner) {
                if (*ppctSteal < 70) {
                    *ppctSteal = 70;
                }
            } else if (lphs->iItem == iscannerRobberBaronScanner && *ppctSteal < 80) {
                *ppctSteal = 80;
            }
        }
        j++;
        lphs++;
    }
    if (cPts == 0) {
        return 0;
    }
    if (cPts < 0 || cPts > 25000) {
        return 0;
    }
    cScore = LOWORD(cPts);
    if (cScore <= 100) {
        return cScore >> 1;
    }
    cScore -= 100;
    if (cScore <= 200) {
        return (cScore >> 3) + 0x32;
    }
    cScore -= 200;
    if (cScore <= 312) {
        return cScore / 24 + 75;
    }
    cScore -= 312;
    if (cScore <= 512) {
        return (cScore >> 6) + 0x58;
    }
    if (cScore < 1000) {
        return (cScore < 768 ? 0 : 1) + 96;
    }
    return 98;
}
