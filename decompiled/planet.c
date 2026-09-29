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
    uint16_t           t_merge_0062_0001;
    POINT              t_pt_05b0;
    POINT              t_pt_05c0_1;

    switch (message) {
    case WM_CREATE:
        SetPlanetTitleBar(hwnd);
        for (i = 0; i < 3; i++) {
            t_merge_0062_0001 = i == 1 ? 0x210 : 0x0;
            rghwndOrderDD[i] = CreateWindow(szCombobox, "OrdDD", t_merge_0062_0001 | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200, i == 2 ? 160 : 80,
                                            hwnd, 0x0, hInst, 0x0);
            SendMessage(rghwndOrderDD[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        }
        for (i = 99; i < 109; i++) {
            psz = PszGetCompressedString(i);
            SendMessage(rghwndOrderDD[0], CB_ADDSTRING, 0x0, (LPARAM)psz);
        }
        hwndOrderED = CreateWindow(szEdit, 0x0, ES_RIGHT | WS_CHILD | WS_BORDER, 100, 100, 200, 50, hwnd, 0x0, hInst, 0x0);
        SendMessage(hwndOrderED, EM_LIMITTEXT, 0x4, 0);
        SendMessage(hwndOrderED, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        lpfnRealEditProc = GetWindowLong(hwndOrderED, 0xfffc);
        SetWindowLong(hwndOrderED, 0xfffc, lpfnFakeEditProc);
        hwndBattleDD = CreateWindow(szCombobox, "BattleDD", CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd, 0x0, hInst, 0x0);
        SendMessage(hwndBattleDD, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndShipDD = CreateWindow(szCombobox, "ShipDD", CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd,
                                  0x0, hInst, 0x0);
        SendMessage(hwndShipDD, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        GetClientRect(hwndShipDD, &rc);
        dyShipDD = rc.bottom;
        hwndShipLB = CreateWindow(szListbox, "ShipLB", LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | LBS_DISABLENOSCROLL | WS_CHILD | WS_BORDER | WS_VSCROLL, 100, 100,
                                  200, 80, hwnd, 0x0, hInst, 0x0);
        SendMessage(hwndShipLB, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndFleetCompLB =
            CreateWindow(szListbox, "FleetCompLB", LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | WS_VSCROLL,
                         100, 100, 200, 80, hwnd, 0x0, hInst, 0x0);
        SendMessage(hwndFleetCompLB, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
        hwndPlanetProdLB =
            CreateWindow(szListbox, "PlanetProdLB", LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_BORDER | WS_VSCROLL,
                         100, 100, 200, 80, hwnd, 0x0, hInst, 0x0);
        SendMessage(hwndPlanetProdLB, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
        for (i = 0; i < 13; i++) {
            psz = PszGetCompressedString(i + 415);
            rghwndBtn[i] = CreateWindow(szButton, psz, WS_CHILD, 100, 100, 100, dyArial8 * 2, hwnd, 0x0, hInst, 0x0);
            SendMessage(rghwndBtn[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        }
        hwndRepCB = CreateWindow(szButton, PszGetCompressedString(idsRepeatOrders), BS_AUTOCHECKBOX | WS_CHILD, 100, 100, 150, dyArial8, hwnd, 0x0, hInst, 0x0);
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
        if (IS_WM_CTLCOLOR(message) != 0) {
            if (GET_WM_CTLCOLOR_HWND(wParam, lParam) == hwndRepCB) {
                SetBkColor((HDC)wParam, crButtonFace);
                SetTextColor((HDC)wParam, crButtonText);
                return (LRESULT)hbrButtonFace;
            }
        } else {
            switch (message) {
            case WM_MDIACTIVATE:
                hwndActive = wParam == 0x0 ? 0x0 : hwnd;
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
                hcs = 0x0;
                GetCursorPos(&t_pt_05b0);
                pt = PointTo16(t_pt_05b0);
                t_pt_05c0_1 = PointFrom16(pt);
                ScreenToClient(hwnd, &t_pt_05c0_1);
                pt = PointTo16(t_pt_05c0_1);
                GetClientRect(hwnd, &rc);
                if (PtInRect(&rc, PointFrom16(pt)) == 0)
                    break;
                hcs = ClickInShipOrders(pt, 0, 1, 0);
                if (hcs == 0x0) {
                    hcs = ClickInPlanetOrders(pt, 0, 1, 0);
                }
                if (hcs == 0x0)
                    break;
                SetCursor(hcs);
                return 1;
            case WM_DRAWITEM:
                lpdis = (DRAWITEMSTRUCT *)lParam;
                if (lpdis->itemID != -1) {
                    switch (lpdis->itemAction) {
                    case 0x1:
                        DrawCBEntireItem(lpdis, -4);
                        break;
                    case 0x2:
                        DrawCBEntireItem(lpdis, -4);
                        break;
                    case 0x4:
                        DrawCBEntireItem(lpdis, -4);
                    default:
                    }
                } else {
                    HandleFocusState(lpdis, -2);
                }
                return 1;
            case WM_MEASUREITEM:
                lpmis = (MEASUREITEMSTRUCT *)lParam;
                lpmis->itemHeight = dyArial8 + 2;
                return 1;
            case WM_CHAR:
                if (wParam != 0x66 && wParam != 0x46) {
                    return 0;
                }
                lppl = lpPlanets;
                lpplMac = lpPlanets + cPlanet;
                while (1) {
                    if (lppl >= lpplMac)
                        goto L_07ce;
                    if (lppl->iPlayer == idPlayer)
                        break;
                    lppl = lppl + 1;
                }
                if (sel.grobj != grobjPlanet || sel.id != lppl->id) {
                    SelectAdjPlanet(0, lppl->id);
                    return 0;
                }
            L_07ce:
                i = 0;
                while (1) {
                    if (i >= cFleet) {
                        return 0;
                    }
                    lpfl = rglpfl[i];
                    if (rglpfl[i] == 0x0) {
                        return 0;
                    }
                    if (lpfl->iPlayer == idPlayer)
                        break;
                    i = i + 1;
                }
                if (sel.grobj == grobjFleet && sel.id == lpfl->id) {
                    return 0;
                }
                SelectAdjFleet(0, lpfl->id);
                return 0;
            case WM_COMMAND:
                if (sel.grobj == grobjFleet) {
                    ShipCommandProc(hwnd, wParam, lParam);
                    return 0;
                }
                if (GET_WM_COMMAND_HWND(wParam, lParam) != hwndShipDD) {
                    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[4] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[5] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[0] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[1] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[2] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                        if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[11] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                            if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndBtn[12] || GET_WM_COMMAND_CMD(wParam, lParam) != 0x0) {
                                                if (GET_WM_COMMAND_HWND(wParam, lParam) == hwndPlanetProdLB && GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                                                    DrawPlanShip(0x0, 64);
                                                    if (gd.fTutorial == 0x0)
                                                        break;
                                                    tutor.fProgress = 0x1;
                                                    AdvanceTutor();
                                                    break;
                                                }
                                                break;
                                            }
                                            if (AlertSz(PszFormatIds(idsSureWantDeleteEverythingPlanetsProductionQueue, 0x0),
                                                        MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES) {
                                                return 0;
                                            }
                                            ChangeProduction(1);
                                        } else {
                                            ChangeProduction(0);
                                        }
                                    } else {
                                        pt.x = 610;
                                        pt.y = 470;
                                        ShipBuilder(pt);
                                    }
                                } else {
                                    lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                    if (lSel != -1 && FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, -1) != 0 && xf.grobj == grobjFleet) {
                                        SelectAdjFleet(0, xf.id);
                                    }
                                }
                            } else {
                                lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
                                if (lSel == -1 || FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, -1) == 0) {
                                    return 0;
                                }
                                TransferStuff(sel.pl.id, grobjPlanet, xf.id, xf.grobj, mdXferCargo);
                            }
                        } else if (GetKeyState(16) >= 0) {
                            SelectAdjPlanet(1, 0);
                        } else {
                            SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, 1));
                        }
                    } else if (GetKeyState(16) >= 0) {
                        SelectAdjPlanet(-1, 0);
                    } else {
                        SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, 0));
                    }
                    SetFocus(hwndFrame);
                    return 0;
                }
                if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x1) {
                    DrawPlanShip(0x0, -32764);
                }
            default:
            }
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
    objNull.pfl = 0x0;
    if (sel.id != -1) {
        if (sel.grobj != grobjFleet) {
            ptile = rgtilePlanet;
            ctile = 6;
            obj.ppl = &sel.pl;
        } else {
            ptile = rgtileShip;
            ctile = 7;
            obj.pfl = &sel.fl;
        }
        if (hdc == 0x0) {
            fDC = 1;
            hdc = GetDC(hwndPlanet);
        }
        hfontSav = SelectObject(hdc, rghfontArial8[0]);
        crBack = SetBkColor(hdc, crButtonFace);
        crFore = SetTextColor(hdc, crButtonText);
        fErase = (grbit & 0x8000) == 0x0 ? 0 : 1;
        fMin = (grbit & 0x4000) == 0x0 ? 0 : 1;
        for (i = 0; i < ctile; i++) {
            if ((grbit & ptile[i].grbit) != 0x0) {
                ptile[i].fErase = fErase;
                ptile[i].fMinDraw = fMin;
                ptile[i].pfn(hdc, ptile + i, ptile[i].fNullPtr == 0x0 ? obj : objNull);
            }
        }
        SetTextColor(hdc, crFore);
        SetBkColor(hdc, crBack);
        SelectObject(hdc, hfontSav);
        if (fDC != 0) {
            ReleaseDC(hwndPlanet, hdc);
        }
    } else {
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
        if (rgplr[idPlayer].fDead != 0x0 && hdc != 0x0) {
            GetClientRect(hwndPlanet, &rc);
            SetBkColor(hdc, crButtonFace);
            SetTextColor(hdc, crButtonText);
            i = CchGetString(idsDeceased, szWork);
            DiaganolTextOut(hdc, &rc, szWork, i);
        }
    }
    return;
}

int16_t FDrawTileNC(HDC hdc, TILE *ptile, RECT *prc, char *pszTitle) {
    int16_t bt;
    RECT    rcT;

    bt = 112;
    prc->left = ptile->iCol * 0xc6 + 4;
    prc->right = prc->left + 190;
    prc->top = ptile->yTop;
    prc->bottom = (ptile->fPopped == 0x0 ? dyArial8 + 3 : ptile->dyFull) + prc->top;
    if (ptile->fMinDraw == 0x0 || ptile->fMinTitle != 0x0) {
        if (ptile->fMinDraw == 0x0) {
            _Draw3dFrame(hdc, prc, 0);
        }
        rcT = *prc;
        ExpandRc(&rcT, -1, -1);
        rcT.bottom = rcT.top + dyArial8 + 2;
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, crButtonText);
        SetBkColor(hdc, crButtonFace);
        if (ptile->fMinDraw == 0x0) {
            _Draw3dFrame(hdc, &rcT, 0);
        }
        RcCtrTextOut(hdc, &rcT, pszTitle, -1);
        SetRect(&rcT, prc->right - 17, prc->top + 1, prc->right, rcT.bottom + 1);
        if (ptile->fPopped == 0x0) {
            bt = bt | 0x1;
        } else {
            bt = bt;
        }
        DrawBtn(hdc, &rcT, bt, 0, 0x0);
        SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, prc->right - 18, prc->top + 1, 1, rcT.bottom - prc->top - 1, PATCOPY);
    }
    prc->top = prc->top + (dyArial8 + 4);
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
    int16_t t_149b;

    ppl = obj.ppl;
    if (ptile->fFixCtls != 0x0) {
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
        if (ppl == 0x0) {
            ppl = &sel.pl;
        }
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        dxRight = dxMaxMineralQuan;
        SetRect(&rgrcRef[6], xLeft, yTop, xRight, 3 * dyArial8 + yTop);
        for (i = 0; i <= 2; i++) {
            if (ptile->fMinDraw == 0x0) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, rgcrMinerals[i]);
                TextOut(hdc, xLeft, yTop, rgszMinerals[i], lstrlen(rgszMinerals[i]));
            }
            SelectObject(hdc, rghfontArial8[0]);
            SetTextColor(hdc, crButtonText);
            c = _wsprintf(szWork, PszGetCompressedString(idsLdkt), ppl->rgwtMin[i]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
        }
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        t_149b = yTop;
        yTop = yTop + 1;
        PatBlt(hdc, rc.left, yTop, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        SetRect(&rgrcRef[7], xLeft, yTop, xRight, dyArial8 * 2 + yTop);
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsMines4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), ppl->cMines, CMaxOperableMines(ppl, idPlayer, 0));
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsD), CMinesOperating(ppl));
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight * 2);
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsFactories4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), ppl->cFactories, CMaxOperableFactories(ppl, idPlayer, 0));
        } else {
            c = CchGetString(idsN, szWork);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight * 2);
        yTop = yTop + dyArial8;
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
    int16_t t_call_1ac7;
    int16_t t_1b78;
    int16_t t_1e3b;

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
        if (ptile->fMinDraw == 0x0) {
            c = CchGetString(idsPopulation4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        SetRect(&rgrcRef[9], xLeft, yTop, xRight, yTop + dyArial8);
        c = CommaFormatLong(szWork, (uint32_t)(sel.pl.rgwtMin[3] * 100));
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsResourcesYear, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        SetRect(&rgrcRef[8], xLeft, yTop, xRight, yTop + dyArial8);
        t_call_1ac7 = CResourcesAtPlanet(&sel.pl, idPlayer);
        cResAvail = t_call_1ac7;
        cRes = t_call_1ac7;
        if (sel.pl.fNoResearch == 0x0) {
            cResAvail = cResAvail - MulDiv(cRes, (int16_t)rgplr[idPlayer].pctResearch, 100);
        }
        c = _wsprintf(szWork, PszGetCompressedString(idsDD), cResAvail, cRes);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop = yTop + dyArial8;
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        t_1b78 = yTop;
        yTop = yTop + 1;
        PatBlt(hdc, rc.left, yTop, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsScannerType, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        SetRect(&rgrcRef[11], xLeft, yTop, xRight, dyArial8 * 2 + yTop);
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
            if (sel.pl.iScanner != 0x1f) {
                LookupBestPlanetaryScanner(&part);
                fstrcpy(szWork, part.pcom->szName);
                dRange = GetPlanetScannerRange(&sel.pl, &dRangeP);
            } else {
                CchGetString(idsNone4, szWork);
                dRange = 0;
            }
        } else {
            CchGetString(idsOrganic, szWork);
            dRange = GetPlanetScannerRange(&sel.pl, &dRangeP);
        }
        RightTextOut(hdc, xRight, yTop, szWork, 0, dxRight);
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsScannerRange, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (dRange <= 0) {
            CchGetString(idsNone4, szWork);
            c = 0;
        } else if (dRangeP <= 0) {
            if (dRange >= 100) {
                c = _wsprintf(szWork, PszGetCompressedString(idsDLY), dRange);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsDLightYears), dRange);
            }
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDDLY), dRangeP, dRange);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop = yTop + dyArial8;
        hbrSav = SelectObject(hdc, hbrButtonHilite);
        t_1e3b = yTop;
        yTop = yTop + 1;
        PatBlt(hdc, rc.left, yTop, rc.right - rc.left, 1, PATCOPY);
        SelectObject(hdc, hbrSav);
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefenses4, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
        }
        SelectObject(hdc, rghfontArial8[0]);
        SetRect(&rgrcRef[10], xLeft, yTop, xRight, 3 * dyArial8 + yTop);
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
            c = _wsprintf(szWork, PszGetCompressedString(idsDD), sel.pl.cDefenses, CMaxOperableDefenses(&sel.pl, idPlayer, 0));
        } else {
            c = CchGetString(idsN, szWork);
        }
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefenseType, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (sel.pl.cDefenses != 0x0) {
            FGetBestDefensePart(&part);
            fstrcpy(szWork, part.pcom->szName);
            dRange = 1;
        } else {
            CchGetString(GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh ? idsN : idsNone4, szWork);
            dRange = 0;
        }
        RightTextOut(hdc, xRight, yTop, szWork, 0, dxRight);
        yTop = yTop + dyArial8;
        if (ptile->fMinDraw == 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDefCoverage, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
        }
        if (dRange == 0) {
            c = CchGetString(GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh ? idsN : idsNone4, szWork);
            psz = szWork;
        } else {
            CalcPctSurvive(&sel.pl, &pct, 0x0);
            pct = 1.0 - pct;
            c = _wsprintf(szWork, PCTDXPCTDPCTPCT, LOWORD((int32_t)(pct * 100.0)),
                          LOWORD((int32_t)((pct - (double)(int32_t)LOWORD((int32_t)(pct * 100.0)) / 100.0) * 10000.0)));
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
        i = i + 1;
        part.hs.iItem = part.hs.iItem + 0x1;
    }
    if (i <= 0) {
        fRet = 0;
    } else {
        i = i - 1;
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
    int16_t  t_280d;
    int16_t  t_28d9;

    if (ptile->fFixCtls != 0x0) {
        rgrcRef[13].top = -5;
        rgrcRef[13].bottom = -6;
        rgrcRef[14].top = -5;
        rgrcRef[14].bottom = -6;
        rgrcRef[16].top = -5;
        rgrcRef[16].bottom = -6;
        rgrcRef[15].top = -5;
        rgrcRef[15].bottom = -6;
        ptile->fFixCtls = 0x0;
    }
    if (sel.pl.fStarbase == 0x0) {
        psz = PszGetCompressedString(idsStarbase2);
    } else {
        lpshdef = rglpshdefSB[idPlayer] + sel.pl.isb;
        fstrcpy(szWork, lpshdef->hul.szClass);
        psz = szWork;
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        SetRect(&rc, xLeft - 2, yTop, xRight + 2, rc.bottom - 2);
        FillRect(hdc, &rc, hbrButtonFace);
        if (sel.pl.fStarbase != 0x0) {
            SetRect(&rgrcRef[14], xLeft, yTop, xRight, dyArial8 * 4 + yTop);
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDockCapacity, szWork);
            l = GetTextExtent(hdc, szWork, c);
            dxRight = xRight - xLeft - LOWORD(l);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
            w = LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax;
            if (w != 0x0) {
                if ((uint32_t)w != 0xffff) {
                    c = _wsprintf(szWork, PCTDKT, w);
                } else {
                    c = CchGetString(idsUnlimited, szWork);
                }
            } else {
                c = CchGetString(idsNone4, szWork);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsArmor2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            w = lpshdef->hul.dp;
            if (w != 0x0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsLddp), w, 0x0);
            } else {
                c = CchGetString(idsNone4, szWork);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsShields2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            l = DpShieldOfShdef(lpshdef, idPlayer);
            if (l != 0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsLddp), l);
            } else {
                c = CchGetString(idsNone4, szWork);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDamage2, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
            w = sel.pl.pctDp;
            if (w == 0x0) {
                c = CchGetString(idsNone4, szWork);
            } else {
                if (w < 0x5) {
                    w = 0x5;
                }
                c = _wsprintf(szWork, PCTDPCTPCT, (uint32_t)w / 0x5);
                crForeSav = SetTextColor(hdc, 0x7f);
            }
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            if (w != 0x0) {
                SetTextColor(hdc, crForeSav);
            }
            hbrSav = SelectObject(hdc, hbrButtonHilite);
            t_280d = yTop;
            yTop = yTop + 1;
            PatBlt(hdc, rc.left, yTop, rc.right - rc.left, 1, PATCOPY);
            SelectObject(hdc, hbrSav);
            SelectObject(hdc, rghfontArial8[1]);
            SetRect(&rgrcRef[16], xLeft, yTop, xRight, yTop + dyArial8);
            c = CchGetString(idsMassDriver, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            iWarp = IWarpMAFromLppl(&sel.pl, &fTwo);
            if (iWarp <= 0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsNone4));
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsWarpD), iWarp);
                if (fTwo != 0) {
                    t_28d9 = c;
                    c = c + 1;
                    szWork[t_28d9] = '+';
                }
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsDestination3, szWork);
            TextOut(hdc, xLeft, yTop, szWork, c);
            if (sel.pl.idFling != 0x0) {
                psz = PszGetCompressedPlanet(rgidPlan[sel.pl.idFling - 1]);
                c = 0;
                strcpy(szWork, psz);
            } else {
                c = CchGetString(idsNone4, szWork);
            }
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
            yTop = yTop + dyArial8;
            c = (int32_t)(xRight - xLeft) / 3;
            SetRect(&rgrcRef[13], xLeft, yTop, xLeft + c, yTop + dyArial8 + 6);
            bt = 8;
            if (iWarp == 0) {
                bt = bt | 0x4;
            }
            DrawBtn(hdc, &rgrcRef[13], bt, gd.fSetMassMode, PszGetCompressedString(idsSetDest));
            if (iWarp <= 0) {
                SetRect(&rgrcRef[15], -5, -5, -6, -6);
                rgrcRef[16] = rgrcRef[15];
            } else {
                SetRect(&rgrcRef[15], xLeft + c + 4, yTop + 3, xRight, yTop + dyArial8 + 3);
                DrawMassWarpGauge(hdc, &rgrcRef[15], fTwo == 0 ? iWarp : -iWarp, sel.pl.iWarpFling + 4);
            }
        } else {
            SetRect(&rgrcRef[14], -5, -5, -6, -6);
            rgrcRef[15] = rgrcRef[14];
            rgrcRef[16] = rgrcRef[14];
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
    lMax = (int32_t)(iBest - 1);
    if (iCur > iBest + fTwoMAs) {
        if (iCur >= iBest + fTwoMAs + 3) {
            hbr = hbrRed;
        } else {
            hbr = hbrYellow;
        }
    } else {
        hbr = hbrPurple;
    }
    lCur = (int32_t)(iCur - 4);
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
    int16_t t_merge_30ab_0001;

    ppl = obj.ppl;
    if (ptile->fFixCtls != 0x0) {
        ShowWindow(hwndPlanetProdLB, SW_HIDE);
        ShowWindow(rghwndBtn[11], SW_HIDE);
        ShowWindow(rghwndBtn[12], SW_HIDE);
        rgrcRef[17].top = -5;
        rgrcRef[17].bottom = -6;
        ptile->fFixCtls = 0x0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(idsProduction)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top;
        yTop = yTop + 4;
        GetClientRect(hwndPlanetProdLB, &rcT);
        swp = 20;
        dyPlanetProdLB = (dyArial8 + 2) * (gd.fSmallTileMode == 0x0 ? 0x5 : 0x3);
        dyWrong = dyPlanetProdLB - (rcT.bottom - rcT.top);
        if (dxPlanetProdLB != xRight - xLeft || dyWrong < 0 || dyWrong >= dyArial8) {
            dxPlanetProdLB = xRight - xLeft;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(hwndPlanetProdLB, 0x0, xLeft, yTop, xRight - xLeft, dyPlanetProdLB, swp);
        ShowWindow(hwndPlanetProdLB, SW_SHOW);
        GetClientRect(hwndPlanetProdLB, &rcT);
        dyPlanetProdLB = rcT.bottom - rcT.top;
        yTop = yTop + (dyPlanetProdLB + 4);
        iSel = LOWORD(SendMessage(hwndPlanetProdLB, LB_GETCURSEL, 0x0, 0));
        if (iSel < 0) {
            iSel = 0;
        }
        if (ppl->lpplprod != 0x0 && ppl->lpplprod->iprodMac != 0x0) {
            psz = PszProductionETA(ppl, 0x0, iSel, 0x0, 0x0);
            c = strlen(psz);
        } else {
            c = 0;
            szWork[0] = 0;
        }
        if (c == 0) {
            RightTextOut(hdc, xRight, yTop, szWork, 0, xRight - xLeft);
        } else {
            SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsCompletion, szT);
            TextOut(hdc, xLeft, yTop, szT, cch);
            dxRight = xRight - xLeft - LOWORD(GetTextExtent(hdc, szT, cch));
            SelectObject(hdc, rghfontArial8[0]);
            RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        }
        yTop = yTop + dyArial8;
        SelectObject(hdc, rghfontArial8[1]);
        cch = CchGetString(idsRoute3, szT);
        TextOut(hdc, xLeft, yTop, szT, cch);
        dxRight = xRight - xLeft - LOWORD(GetTextExtent(hdc, szT, cch));
        if (sel.pl.idRoute != 0x0) {
            psz = PszGetCompressedPlanet(rgidPlan[sel.pl.idRoute - 1]);
            c = 0;
            strcpy(szWork, psz);
        } else {
            c = CchGetString(idsNone4, szWork);
        }
        SelectObject(hdc, rghfontArial8[0]);
        RightTextOut(hdc, xRight, yTop, szWork, c, dxRight);
        yTop = yTop + dyArial8;
        c = (int32_t)(xRight - xLeft - 16) / 3;
        xStart = xLeft;
        i = 11;
        while (i <= 12) {
            SetWindowPos(rghwndBtn[i], 0x0, xStart, yTop, c, (dyArial8 >> 0x1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            ShowWindow(rghwndBtn[i], SW_SHOW);
            i = i + 1;
            xStart = xStart + (c + 8);
        }
        if (sel.pl.lpplprod == 0x0 || sel.pl.lpplprod->iprodMac == 0x0) {
            t_merge_30ab_0001 = 0;
        } else {
            t_merge_30ab_0001 = 1;
        }
        EnableWindow(rghwndBtn[12], t_merge_30ab_0001);
        SetRect(&rgrcRef[17], xStart, yTop, xStart + c, yTop + dyArial8 + (dyArial8 >> 0x1));
        DrawBtn(hdc, &rgrcRef[17], 8, gd.fSetRouteMode, PszGetCompressedString(idsRoute2));
    } else {
        ShowWindow(hwndPlanetProdLB, SW_HIDE);
        ShowWindow(rghwndBtn[11], SW_HIDE);
        ShowWindow(rghwndBtn[12], SW_HIDE);
    }
    return;
}

char *PszProductionETA(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *etaFirst, int16_t *etaLast) {
    int16_t  iTurnEnd;
    int16_t  iTurnBegin;
    int16_t  c;
    StringId ids;

    if (lpplprod == 0x0) {
        lpplprod = lppl->lpplprod;
    }
    EstimateItemProdSched(lppl, lpplprod, iItem, &iTurnBegin, &iTurnEnd);
    if (iTurnBegin != 100) {
        if (iTurnEnd != 100) {
            if (iTurnBegin != iTurnEnd) {
                c = _wsprintf(szWork, PszGetCompressedString(idsDDYears), iTurnBegin, iTurnEnd);
            } else if (iTurnBegin != 0) {
                if (iTurnBegin != -1) {
                    c = _wsprintf(szWork, PszGetCompressedString(idsDYear), iTurnBegin);
                    if (iTurnBegin != 1) {
                        szWork[c] = 's';
                        c = c + 1;
                        szWork[c] = 0;
                    }
                } else {
                    c = CchGetString(idsNeeded, szWork);
                }
            } else {
                c = CchGetString(idsSkipped, szWork);
            }
        } else {
            c = _wsprintf(szWork, PszGetCompressedString(idsDYears), iTurnBegin);
        }
    } else {
        if (lpplprod != 0x0 && lpplprod->iprodMac > iItem && lpplprod->rgprod[iItem].grobj == grobjPlanet && lpplprod->rgprod[iItem].iItem < mdIdleFactory) {
            ids = idsUnknown2;
        } else {
            ids = idsNever;
        }
        c = CchGetString(ids, szWork);
    }
    if (etaFirst != 0x0) {
        *etaFirst = iTurnBegin;
    }
    if (etaLast != 0x0) {
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

    if (sel.grobj != grobjPlanet) {
        psz = PszGetFleetName(obj.pfl->id);
    } else {
        psz = PszGetPlanetName(obj.ppl->id);
        i = obj.ppl->id;
        i = i + 8;
        iOffset = (int32_t)i % 28;
    }
    if (ptile->fFixCtls != 0x0) {
        for (i = 4; i <= 6; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ptile->fFixCtls = 0x0;
    }
    if (FDrawTileNC(hdc, ptile, &rc, psz) != 0) {
        xLeft = rc.left + 12;
        xRight = rc.right - 12;
        yTop = (gd.fSmallTileMode == 0x0 ? 6 : 2) + rc.top;
        if (sel.grobj != grobjFleet) {
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
            DibBlt(hdc, xLeft + 3, yTop + 3, 64, 64, hdibPlanets, (int32_t)iOffset % 7 * 64, (int32_t)iOffset / 7 * 64, 64, 64, 13369376);
        } else {
            DrawFleetBitmap(&sel.fl, hdc, xLeft, yTop, 1, -1, 0, 0, -1, 0);
        }
        dx = xRight - xLeft - 95;
        dy = 0x3 * dyArial8 >> 0x1;
        xLeft = xRight - dx;
        if (ptile->fMinDraw == 0x0) {
            if (gd.fSmallTileMode == 0x0) {
                yTop = yTop - 4;
            } else {
                yTop = yTop - 2;
                dy = dy - 2;
            }
            iOffset = sel.grobj == grobjFleet ? 6 : 5;
            i = 4;
            while (i <= iOffset) {
                SetWindowPos(rghwndBtn[i], 0x0, xLeft, yTop, dx, dy, SWP_NOZORDER | SWP_NOACTIVATE);
                ShowWindow(rghwndBtn[i], SW_SHOW);
                i = i + 1;
                yTop = yTop + ((gd.fSmallTileMode == 0x0 ? 3 : 2) + dy);
            }
        }
    } else {
        for (i = 4; i <= 6; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
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
    int16_t t_merge_3940_0001;
    int16_t t_merge_3a16_0001;
    int16_t t_merge_3a55_0001;
    int16_t t_merge_3de0_0001;

    pfl = obj.pfl;
    fDoneDrawing = 0;
    fObjIsThing = 0;
    if (ptile->fFixCtls != 0x0) {
        for (i = 0; i <= 2; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ShowWindow(hwndShipDD, SW_HIDE);
        ptile->fFixCtls = 0x0;
        rgrcRef[1].top = -5;
        rgrcRef[1].bottom = -6;
        rgrcRef[4].top = -5;
        rgrcRef[4].bottom = -6;
    }
    if (FDrawTileNC(hdc, ptile, &rc, PszGetCompressedString(pfl == 0x0 ? idsFleetsOrbit : idsOtherFleetsHere)) != 0) {
        xLeft = rc.left + 4;
        xRight = rc.right - 4;
        yTop = rc.top + 2;
        swp = 20;
        if (dxShipDD != xRight - xLeft) {
            dxShipDD = xRight - xLeft;
        } else {
            swp = swp | 0x1;
        }
        SetWindowPos(hwndShipDD, 0x0, xLeft, yTop, xRight - xLeft, 5 * dyShipDD, swp);
        ShowWindow(hwndShipDD, SW_SHOW);
        yTop = yTop + (dyShipDD + 3);
        lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0x0, 0);
        t_merge_3940_0001 = lSel == -1 ? 0 : 1;
        EnableWindow(rghwndBtn[0], t_merge_3940_0001);
        if (lSel == -1) {
            fDoneDrawing = 1;
            fUnknown = 1;
        }
        if (pfl == 0x0) {
            idSkip = -1;
        } else {
            idSkip = pfl->id;
        }
        if (fDoneDrawing == 0 && FLookupOrbitingXfer(idSkip == -1 ? sel.pl.id : pfl->idPlanet, LOWORD(lSel), &xf, idSkip) == 0) {
            fDoneDrawing = 1;
        }
        if (fDoneDrawing == 0) {
            fObjIsThing = xf.grobj == grobjThing ? 1 : 0;
            if (fObjIsThing != 0 || xf.fl.iPlayer != idPlayer) {
                t_merge_3a16_0001 = 0;
            } else {
                t_merge_3a16_0001 = 1;
            }
            EnableWindow(rghwndBtn[1], t_merge_3a16_0001);
        } else {
            EnableWindow(rghwndBtn[1], 0);
        }
        rgrcRef[1].top = -5;
        rgrcRef[1].bottom = -6;
        rgrcRef[4].top = -5;
        rgrcRef[4].bottom = -6;
        if (fObjIsThing != 0 || xf.fl.det == 0x7) {
            t_merge_3a55_0001 = 0;
        } else {
            t_merge_3a55_0001 = 1;
        }
        fUnknown = t_merge_3a55_0001;
        if (gd.fSmallTileMode == 0x0) {
            if (fDoneDrawing == 0 && fUnknown == 0) {
                SelectObject(hdc, rghfontArial8[1]);
                c = CchGetString(idsCargo3, szWork);
                l = GetTextExtent(hdc, szWork, c);
            }
            if (fDoneDrawing != 0 || fUnknown != 0 || fObjIsThing != 0) {
                if (ptile->fMinDraw != 0x0 || fUnknown != gd.fUnknownShip) {
                    SetRect(&rcGauge, xLeft, yTop, xRight, dyArial8 * 2 + yTop + 8);
                    FillRect(hdc, &rcGauge, hbrButtonFace);
                }
            } else {
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
            }
            gd.fUnknownShip = fUnknown;
            if (fObjIsThing == 0) {
                yTop = yTop + (dyArial8 + 4);
            }
            if (fDoneDrawing == 0 && fUnknown == 0) {
                c = CchGetString(idsCargo3, szWork);
                TextOut(hdc, xLeft, yTop, szWork, c);
                SetRect(&rcGauge, xLeft + LOWORD(l), yTop, xRight, yTop + dyArial8);
                rgrcRef[4] = rcGauge;
                if (fObjIsThing == 0) {
                    DrawFleetGauge(hdc, &rcGauge, &xf.fl, 5);
                } else {
                    DrawThingGauge(hdc, &rcGauge, &xf.th, 5);
                }
                if (fObjIsThing != 0) {
                    OffsetRect(&rcGauge, 0, dyArial8 + 4);
                    FillRect(hdc, &rcGauge, hbrButtonFace);
                    yTop = yTop + (dyArial8 + 4);
                }
            }
            yTop = yTop + (dyArial8 + 4);
        }
        c = (int32_t)(xRight - xLeft - 10) / 3;
        xStart = xLeft;
        i = 0;
        while (i <= 2) {
            SetWindowPos(rghwndBtn[(int32_t)(i + 1) % 3], 0x0, xStart, yTop, c, (dyArial8 >> 0x1) + dyArial8, SWP_NOZORDER | SWP_NOACTIVATE);
            if (i != 2 || pfl != 0x0) {
                ShowWindow(rghwndBtn[i], SW_SHOW);
            }
            i = i + 1;
            xStart = xStart + (c + 6);
        }
        if ((idSkip != -1 && fUnknown != 0) || fObjIsThing != 0) {
            t_merge_3de0_0001 = 0;
        } else {
            t_merge_3de0_0001 = 1;
        }
        EnableWindow(rghwndBtn[2], t_merge_3de0_0001);
    } else {
        for (i = 0; i <= 2; i++) {
            ShowWindow(rghwndBtn[i], SW_HIDE);
        }
        ShowWindow(hwndShipDD, SW_HIDE);
    }
    return;
}

void SetPlanetTitleBar(HWND hwnd) {
    char  szTitle[30];
    char *psz;

    if (sel.grobj != grobjPlanet) {
        if (sel.grobj != grobjFleet) {
            psz = PszGetCompressedString(idsPlanetView);
        } else {
            psz = PszGetFleetName(sel.fl.id);
        }
    } else {
        psz = PszGetPlanetName(sel.pl.id);
        CchGetString(idsPlanet2, szTitle);
        lstrcat(szTitle, psz);
        psz = szTitle;
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
        if (grobjNew != grobjPlanet) {
            InvalidateReport(1, 0);
            if (FLookupFleet(iObjSel, &sel.fl) == 0) {
                return;
            }
            sel.pt = sel.fl.pt;
            if (sel.fl.idPlanet == -1 || (sel.fl.idPlanet != sel.pl.id && FLookupPlanet(sel.fl.idPlanet, &sel.pl) == 0)) {
                sel.pl.id = -1;
            }
            sel.grobjFull = (sel.pl.id == -1 ? 0x0 : 0x1) | 0x2;
            sel.iwpAct = 0;
            if (fAi == 0) {
                FillOrdersLB();
                FillFleetCompLB();
                FillBattleDD(sel.fl.iplan + 1);
                idSkip = iObjSel;
                SendMessage(rghwndOrderDD[0], CB_SETCURSEL, sel.fl.lpplord->rgord[0].grTask, 0);
            }
        } else {
            InvalidateReport(0, 0);
            if (FLookupPlanet(iObjSel, &sel.pl) == 0) {
                return;
            }
            sel.pt = rgptPlan[iObjSel];
            sel.scan.iwp = -1;
            sel.iwpAct = -1;
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0x0 || (lpfl->idPlanet == iObjSel && lpfl->iPlayer == idPlayer))
                    break;
            }
            if (i == cFleet) {
                sel.fl.id = -1;
                sel.grobjFull = grobjPlanet;
            } else {
                FDupFleet(lpfl, &sel.fl);
                sel.grobjFull = grobjPlanet | grobjFleet;
            }
            if (fAi == 0) {
                FillPlanetProdLB(0x0, 0x0, 0x0);
                SendMessage(hwndPlanetProdLB, LB_SETCURSEL, 0x0, 0);
            }
        }
        sel.grobj = grobjNew;
        sel.id = iObjSel;
        gd.fSetMassMode = 0x0;
        gd.fSetRouteMode = 0x0;
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
            if (fSameType == 0) {
                InvalidateRect(hwndPlanet, 0x0, 1);
                if ((grbitScan & 0x10) != 0x0 && sel.grobj == grobjPlanet) {
                    grbitScan = grbitScan & 0xffef;
                    InvalidateRect(hwndTb, 0x0, 1);
                }
            } else {
                DrawPlanShip(0x0, 20479);
            }
            SetPlanetTitleBar(hwndPlanet);
            if (gd.fTutorial != 0x0) {
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

    SendMessage(hwndShipDD, CB_RESETCONTENT, 0x0, 0);
    if (sel.grobj != grobjPlanet) {
        ptSel = sel.fl.pt;
    } else {
        ptSel = rgptPlan[sel.id];
    }
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0)
            break;
        if ((idSkip == -1 && sel.id == lpfl->idPlanet) || (idSkip != -1 && lpfl->id != idSkip && lpfl->pt.x == sel.fl.pt.x && lpfl->pt.y == sel.fl.pt.y)) {
            PszGetFleetName(lpfl->id);
            memmove(&szWork[1], szWork, 0x32);
            szWork[0] = LOBYTE(lpfl->iPlayer == idPlayer ? 0x20 : 0x78);
            SendMessage(hwndShipDD, CB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMineralPacket && lpth->pt.x == ptSel.x && lpth->pt.y == ptSel.y) {
            PszGetThingName(lpth->idFull);
            memmove(&szWork[1], szWork, 0x32);
            szWork[0] = LOBYTE(lpth->iplr == idPlayer ? 0x20 : 0x78);
            SendMessage(hwndShipDD, CB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
    }
    SendMessage(hwndShipDD, CB_SETCURSEL, 0x0, 0);
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
            i = i + 1;
        }
        if (i != cPlanet && lpPl->det == 0x7) {
            if (dInc != 0) {
                for (i = 0; i < rgplr[idPlayer].cPlanet && lpPlT[vlprgidPlanet[i]].id != idPlanet; i++) {
                }
                i = i + dInc;
                if (i < rgplr[idPlayer].cPlanet) {
                    if (i < 0) {
                        i = rgplr[idPlayer].cPlanet - 1;
                    }
                } else {
                    i = 0;
                }
                i = vlprgidPlanet[i];
            }
            lpPlT = lpPlT + i;
            idPlanet = lpPlT->id;
            if (lpPlT->iPlayer != idPlayer) {
                return;
            }
            scan.pt = rgptPlan[idPlanet];
            scan.grobj = 0x81;
            ChangeScanSel(&scan, 0);
            RedrawScanSel(0x0, 0);
            ChangeMainObjSel(grobjPlanet, idPlanet);
            RedrawScanSel(0x0, 1);
        } else {
            scan.pt = rgptPlan[idPlanet];
            scan.grobj = 0x81;
            ChangeScanSel(&scan, 0);
        }
        CtrPointScan(rgptPlan[idPlanet], 1);
        DrawScannerSBar(0x0, 0x0, 0x0, 0);
        InvalidateRect(hwndMine, 0x0, 1);
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
        if (lppl->iPlayer == idPlayer && lppl->fStarbase != 0x0 && LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0x0) {
            if (lppl->id <= idPlanet) {
                if (lppl->id < idPlanet) {
                    idBefore = lppl->id;
                }
            } else if (idAfter == -1) {
                idAfter = lppl->id;
            }
            if (idFirst == -1) {
                idFirst = lppl->id;
            }
            idLast = lppl->id;
        }
    }
    if (fNext == 0) {
        if (idBefore != -1) {
            return idBefore;
        }
        return idLast;
    }
    if (idAfter != -1) {
        return idAfter;
    }
    return idFirst;
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
    uint16_t t_merge_4d28_0001;

    bt = 112;
    if (sel.grobj != grobjPlanet) {
        if (sel.grobj != grobjFleet) {
            return;
        }
        prgtile = rgtileShip;
        ctile = 7;
    } else {
        prgtile = rgtilePlanet;
        ctile = 6;
    }
    iCol = (uint32_t)x / 198;
    xRel = x - iCol * 0xc6;
    if (xRel >= 4 && xRel < 194) {
        for (i = 0; i < ctile; i++) {
            if (prgtile[i].iCol >= iCol) {
                if (prgtile[i].iCol > iCol) {
                    return;
                }
                if (y >= prgtile[i].yTop && y < (prgtile[i].fPopped == 0x0 ? dyArial8 + 3 : prgtile[i].dyFull) + prgtile[i].yTop)
                    break;
            }
        }
        if (i != ctile) {
            pt.x = x;
            pt.y = y;
            rcTitle.top = prgtile[i].yTop;
            rcTitle.bottom = dyArial8 + 3 + rcTitle.top + 1;
            rcTitle.left = iCol * 0xc6 + 4;
            rcTitle.right = rcTitle.left + 191;
            if (PtInRect(&rcTitle, PointFrom16(pt)) == 0 || fRightBtn != 0) {
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
                    default:
                    }
                    return;
                }
            L_50b3:
                ClickInShipOrders(pt, sks, 0, fRightBtn);
            } else {
                rc = rcTitle;
                rc.top = rc.top + 1;
                rc.left = rc.right - 17;
                if (PtInRect(&rc, PointFrom16(pt)) == 0) {
                    if (prgtile[i].fPopped != 0x0) {
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
                    pt.x = ((rcTitle.right - rcTitle.left) >> 0x1) + rcTitle.left;
                    pt.y = ((rcTitle.bottom - rcTitle.top) >> 0x1) + rcTitle.top;
                    if (0x0 <= (0x1 >= (int32_t)pt.x / 198 ? (int32_t)pt.x / 198 : 0x1)) {
                        if (0x1 >= (int32_t)pt.x / 198) {
                            t_merge_4d28_0001 = (int32_t)pt.x / 198;
                        } else {
                            t_merge_4d28_0001 = 0x1;
                        }
                    } else {
                        t_merge_4d28_0001 = 0x0;
                    }
                    iCol = t_merge_4d28_0001;
                    iCur = i;
                    for (i = 0; i < ctile && prgtile[i].iCol < iCol; i++) {
                    }
                    for (; i < ctile && prgtile[i].iCol == iCol; i++) {
                        dy = prgtile[i].fPopped == 0x0 ? dyArial8 + 3 : prgtile[i].dyFull;
                        if (pt.y < (dy >> 0x1) + prgtile[i].yTop)
                            break;
                    }
                    if (i != iCur && i != iCur + 1) {
                        tile = prgtile[iCur];
                        if (i >= iCur) {
                            memmove(prgtile + iCur, prgtile + (iCur + 1), (i - iCur - 1) * sizeof(TILE));
                            i = i - 1;
                        } else {
                            memmove(prgtile + (i + 1), prgtile + i, (iCur - i) * sizeof(TILE));
                            iCur = iCur + 1;
                        }
                        prgtile[i] = tile;
                        prgtile[i].iCol = iCol;
                        if (tile.iCol != iCol) {
                            ReflowColumn(iCol, i, 1);
                            ReflowColumn(tile.iCol, iCur, 1);
                        } else {
                            ReflowColumn(iCol, i >= iCur ? iCur : i, 1);
                        }
                    } else {
                        i = prgtile[iCur].iCol;
                        if (iCol != i) {
                            prgtile[iCur].iCol = iCol;
                            ReflowColumn(iCol, iCur, 1);
                            if (iCol >= i) {
                                ReflowColumn(i, iCur, 1);
                            } else {
                                ReflowColumn(i, iCur + 1, 1);
                            }
                        }
                    }
                } else {
                    OffsetRc(&rc, -1, 0);
                    if (prgtile[i].fPopped == 0x0) {
                        bt = bt | 0x1;
                    } else {
                        bt = bt;
                    }
                    InitBtnTrack(&btnt, hwndPlanet, 0x0, &rc, bt, 0, 0, 0, 0x0);
                    while (FTrackBtn(&btnt) != 0) {
                    }
                    if (btnt.fDown != 0x0) {
                        prgtile[i].fPopped = prgtile[i].fPopped == 0x0 ? 0x1 : 0x0;
                        ReflowColumn(prgtile[i].iCol, i, 1);
                    }
                }
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

    if (sel.grobj == grobjPlanet) {
        if (fRightBtn == 0) {
            if (PtInRect(&rgrcRef[6], PointFrom16(pt)) == 0) {
                if (PtInRect(&rgrcRef[7], PointFrom16(pt)) == 0) {
                    if (PtInRect(&rgrcRef[8], PointFrom16(pt)) == 0) {
                        if (PtInRect(&rgrcRef[9], PointFrom16(pt)) == 0) {
                            if (PtInRect(&rgrcRef[10], PointFrom16(pt)) == 0) {
                                if (PtInRect(&rgrcRef[11], PointFrom16(pt)) == 0) {
                                    if (PtInRect(&rgrcRef[13], PointFrom16(pt)) == 0) {
                                        if (PtInRect(&rgrcRef[14], PointFrom16(pt)) == 0) {
                                            if (PtInRect(&rgrcRef[16], PointFrom16(pt)) == 0) {
                                                if (PtInRect(&rgrcRef[15], PointFrom16(pt)) == 0) {
                                                    if (PtInRect(&rgrcRef[17], PointFrom16(pt)) != 0) {
                                                        if (fCursor != 0) {
                                                            return hcurHand;
                                                        }
                                                        InitBtnTrack(&btnt, hwndPlanet, 0x0, &rgrcRef[17], 8, 80, gd.fSetRouteMode, 1,
                                                                     PszGetCompressedString(idsRoute2));
                                                        while (FTrackBtn(&btnt) != 0) {
                                                        }
                                                        gd.fSetRouteMode = gd.fSetRouteMode ^ btnt.fDown;
                                                    }
                                                } else {
                                                    ClickInShipOrders(pt, sks, 0, fRightBtn);
                                                }
                                            } else {
                                                iWarp = IWarpMAFromLppl(&sel.pl, 0x0);
                                                if (iWarp == 0) {
                                                    return 0x0;
                                                }
                                                if (fCursor != 0) {
                                                    return hcurArrowHelp;
                                                }
                                                GlobalPD.part.hs.grhst = hstSpecialSB;
                                                GlobalPD.part.hs.iItem = iWarp + 2;
                                                FLookupPart(&GlobalPD.part);
                                                GlobalPD.grPopup = grPopupComponent;
                                                Popup(hwndPlanet, pt.x, pt.y);
                                            }
                                        } else {
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
                                        }
                                    } else {
                                        iWarp = IWarpMAFromLppl(&sel.pl, 0x0);
                                        if (iWarp == 0) {
                                            return 0x0;
                                        }
                                        if (fCursor != 0) {
                                            return hcurHand;
                                        }
                                        InitBtnTrack(&btnt, hwndPlanet, 0x0, &rgrcRef[13], 8, 80, gd.fSetMassMode, 1, PszGetCompressedString(idsSetDest));
                                        while (FTrackBtn(&btnt) != 0) {
                                        }
                                        gd.fSetMassMode = gd.fSetMassMode ^ btnt.fDown;
                                    }
                                } else {
                                    if (sel.pl.iScanner == 0x1f && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
                                        return 0x0;
                                    }
                                    if (fCursor != 0) {
                                        return hcurArrowHelp;
                                    }
                                    if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
                                        LookupBestPlanetaryScanner(&GlobalPD.part);
                                        GlobalPD.grPopup = grPopupComponent;
                                    } else {
                                        GlobalPD.grPopup = grPopupString;
                                        GlobalPD.dxOut = 180;
                                        GlobalPD.psz = szPopupBuffer;
                                        CchGetString(idsRaceCannotBuildPlanetaryScannersStarbasesHave, szPopupBuffer);
                                    }
                                    Popup(hwndPlanet, pt.x, pt.y);
                                }
                            } else {
                                if (sel.pl.cDefenses == 0x0) {
                                    return 0x0;
                                }
                                if (fCursor != 0) {
                                    return hcurArrowHelp;
                                }
                                FGetBestDefensePart(&GlobalPD.part);
                                GlobalPD.grPopup = grPopupComponent;
                                Popup(hwndPlanet, pt.x, pt.y);
                            }
                        } else {
                            if (fCursor != 0) {
                                return hcurArrowHelp;
                            }
                            GlobalPD.grPopup = grPopupPlanet;
                            GlobalPD.idPlanet = sel.pl.id;
                            Popup(hwndPlanet, pt.x, pt.y);
                        }
                    } else {
                        if (fCursor != 0) {
                            return hcurArrowHelp;
                        }
                        GlobalPD.grPopup = grPopupResources;
                        GlobalPD.idPlanet = sel.pl.id;
                        t_call_53a2 = CResourcesAtPlanet(&sel.pl, idPlayer);
                        GlobalPD.iPlanVal = t_call_53a2;
                        GlobalPD.iPlanetVar = t_call_53a2;
                        if (sel.pl.fNoResearch == 0x0) {
                            GlobalPD.iPlanVal = GlobalPD.iPlanVal - MulDiv(GlobalPD.iPlanetVar, (int16_t)rgplr[idPlayer].pctResearch, 100);
                        }
                        Popup(hwndPlanet, pt.x, pt.y);
                    }
                } else {
                    if (fCursor != 0) {
                        return hcurArrowHelp;
                    }
                    GlobalPD.grPopup = grPopupPlanetIndustry;
                    GlobalPD.idPlan = sel.pl.id;
                    GlobalPD.fFactory = pt.y < rgrcRef[7].top + dyArial8 ? 0 : 1;
                    if (GlobalPD.fFactory == 0) {
                        GlobalPD.cMax = CMaxMines(&sel.pl, idPlayer);
                        GlobalPD.cCur = sel.pl.cMines;
                        GlobalPD.cOperate = CMaxOperableMines(&sel.pl, idPlayer, 0);
                    } else {
                        GlobalPD.cMax = CMaxFactories(&sel.pl, idPlayer);
                        GlobalPD.cCur = sel.pl.cFactories;
                        GlobalPD.cOperate = CMaxOperableFactories(&sel.pl, idPlayer, 0);
                    }
                    Popup(hwndPlanet, pt.x, pt.y);
                }
            } else {
                if (fCursor != 0) {
                    return hcurArrowHelp;
                }
                i = (int32_t)(pt.y - rgrcRef[6].top) / dyArial8;
                GlobalPD.grPopup = grPopupMineral;
                GlobalPD.rgi[0] = (int32_t)i;
                GlobalPD.rgi[2] = sel.pl.rgwtMin[i];
                GlobalPD.rgi[3] = (uint32_t)sel.pl.rgMinConc[i];
                EstMineralsMined(&sel.pl, rglQuan, -1, 0);
                GlobalPD.rgi[4] = rglQuan[i];
                GlobalPD.rgi[1] = sel.pl.fHomeworld;
                Popup(hwndPlanet, pt.x, pt.y);
            }
            return 0x0;
        }
        return 0x0;
    }
    return 0x0;
}

void EnsureTileSize(int16_t fSmallTiles) {
    int16_t iMul;
    int16_t i;
    int16_t grobjSav;

    if (fSmallTiles != gd.fSmallTileMode) {
        gd.fSmallTileMode = fSmallTiles;
        iMul = fSmallTiles == 0 ? 1 : -1;
        for (i = 0; i < 6; i++) {
            if (rgtilePlanet[i].grbit == 64) {
                rgtilePlanet[i].dyFull = rgtilePlanet[i].dyFull + (dyArial8 + 2) * 2 * iMul;
            }
            if (rgtilePlanet[i].grbit == 4) {
                rgtilePlanet[i].dyFull = rgtilePlanet[i].dyFull + (dyArial8 + 4) * 2 * iMul;
            }
            if (rgtilePlanet[i].grbit == 128) {
                rgtilePlanet[i].dyFull = rgtilePlanet[i].dyFull + 10 * iMul;
            }
        }
        for (i = 0; i < 7; i++) {
            if (rgtileShip[i].grbit == 1) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + (dyArial8 * 4 + 2) * iMul;
            }
            if (rgtileShip[i].grbit == 512) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + ((dyArial8 + 2) * 2 + 4 + dyArial8) * iMul;
            }
            if (rgtileShip[i].grbit == 32) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + (dyArial8 + 9) * iMul;
            }
            if (rgtileShip[i].grbit == 4) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + (dyArial8 + 4) * 2 * iMul;
            }
            if (rgtileShip[i].grbit == 128) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + 10 * iMul;
            }
            if (rgtileShip[i].grbit == 256) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + iMul * 2;
            }
            if (rgtileShip[i].grbit == 64) {
                rgtileShip[i].dyFull = rgtileShip[i].dyFull + 6 * iMul;
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
    if (sel.grobj != grobjFleet) {
        ptile = rgtilePlanet;
        ctile = 6;
    } else {
        ptile = rgtileShip;
        ctile = 7;
    }
    yTop = 4;
    if (iTile != -1) {
        for (i = 0; i < iTile; i++) {
            if (ptile[i].iCol == iCol) {
                yTop = yTop + ((ptile[i].fPopped == 0x0 ? dyArial8 + 3 : ptile[i].dyFull) + 4);
            }
        }
    } else {
        for (i = 0; i < ctile && ptile[i].iCol != iCol; i++) {
        }
        if (i == ctile) {
            return;
        }
        iTile = i;
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
        ptile[iTile].fFixCtls = 0x1;
        grbit = grbit | ptile[iTile].grbit;
        yTop = yTop + ((ptile[iTile].fPopped == 0x0 ? dyArial8 + 3 : ptile[iTile].dyFull) + 4);
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
    if (iPlr != -1) {
        idPlayer = iPlr;
        if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, fHelp) != 0) {
            pctCur = PctPlanetDesirability(lppl, iPlr);
            for (i = 0; i < 3; i++) {
                if (rgMin[i] == -1) {
                    if (rgMax[i] == -1) {
                        rgpctBest[i] = 0;
                        continue;
                    }
                    iEnv = rgMax[i];
                } else {
                    iEnv = rgMin[i];
                }
                iSave = (int16_t)lppl->rgEnvVar[i];
                lppl->rgEnvVar[i] = LOBYTE(iEnv);
                pctT = PctPlanetDesirability(lppl, iPlr) - pctCur;
                if (pctT < 0) {
                    pctT = -pctT;
                }
                rgpctBest[i] = (int32_t)(100 * pctT) / abs(iSave - iEnv) + 1;
                lppl->rgEnvVar[i] = LOBYTE(iSave);
            }
            iSave = 0;
            for (i = 1; i < 3; i++) {
                if (rgpctBest[i] > rgpctBest[iSave]) {
                    iSave = i;
                }
            }
            if (rgMin[iSave] == -1) {
                iBest = iSave + 1;
            } else {
                iBest = -(iSave + 1);
            }
            idPlayer = iPlrSav;
            return iBest;
        }
        idPlayer = iPlrSav;
        return 0;
    }
    return 0;
}

char *PszCalcEnvVar(int16_t iEnv, int16_t iVar) {
    switch (iEnv) {
    case 0:
    default:
        return PszCalcGravity(iVar);
    case 1:
        _wsprintf(szWork, "%d%cC", iVar * 4 - 200, 0xba);
        break;
    case 2:
        _wsprintf(szWork, "%dmR", iVar);
    }
    return szWork;
}

char *PszCalcGravity(int16_t iGravity) {
    int16_t d;
    int16_t iVal;

    d = abs(iGravity - 50);
    if (d > 25) {
        iVal = (d - 25) * 24 + 200;
    } else {
        iVal = d * 4 + 100;
    }
    if (iGravity < 50) {
        iVal = 10000 / iVal;
    }
    _wsprintf(szWork, "%d.%02dg", (int32_t)iVal / 100, (int32_t)iVal % 100);
    return szWork;
}

void HandleFocusState(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    if ((lpdis->itemState & 0x10) != 0x0) {
        FrameRect(lpdis->hDC, &lpdis->rcItem, hbr50Screen);
    }
    return;
}

void DrawCBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    int16_t fListbox;
    int16_t fSelected;
    RECT    rc;
    int16_t t_merge_618b_0001;

    fSelected = lpdis->itemState & 0x1;
    rc = lpdis->rcItem;
    if (lpdis->hwndItem != hwndFleetCompLB && lpdis->hwndItem != hwndPlanetProdLB && inflate <= 0) {
        t_merge_618b_0001 = 0;
    } else {
        t_merge_618b_0001 = 1;
    }
    fListbox = t_merge_618b_0001;
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
    int16_t  t_6648;

    fItalic = 0;
    fDoubleDraw = 0;
    fFleet = 0;
    rcIn = *prc;
    rc = rcIn;
    if (fSelected != 0) {
        cr = crWindow;
        switch ((int16_t)*psz) {
        default:
            hbr = hbrRed;
            break;
        case 'I':
            fItalic = 1;
        case ' ':
        LDefCaseSel:
            if (crWindow != 0x0) {
                hbr = GetStockObject(BLACK_BRUSH);
                break;
            }
            hbr = GetStockObject(WHITE_BRUSH);
            break;
        case 'P':
            fDoubleDraw = 1;
            pctDmg = (int16_t)psz[1];
        case 'Q':
            fFleet = 1;
            goto LDefCaseSel;
        case '*':
            hbr = hbrGreen;
            break;
        case '#':
            hbr = hbrBlue;
            break;
        case '&':
            hbr = hbrGray;
        }
    } else {
        hbr = hbrWindow;
        switch ((int16_t)*psz) {
        default:
            cr = 0xff;
            break;
        case 'I':
            fItalic = 1;
        case ' ':
        LDefCase:
            if (crWindow != 0x0) {
                cr = 0x0;
                break;
            }
            cr = 0xffffff;
            break;
        case 'P':
            fDoubleDraw = 1;
            pctDmg = (int16_t)psz[1];
        case 'Q':
            fFleet = 1;
            goto LDefCase;
        case '*':
            cr = 0x7f00;
            break;
        case '#':
            cr = 0x7f0000;
            break;
        case '&':
            cr = 0x7f7f7f;
        }
    }
    if (fListbox == 2) {
        hbr = hbrButtonFace;
    }
    FillRect(hdc, &rcIn, hbr);
    if (fDoubleDraw != 0) {
        rcDraw = rcIn;
        dx = rcIn.right - rcIn.left;
        dx = (int32_t)(dx * pctDmg) / 100;
        rcDraw.right = rcDraw.left + dx;
        FillRect(hdc, &rcDraw, hbrRed);
    }
    if (inflate != 0) {
        InflateRect(&rcIn, -2, -1);
    }
    if (fListbox == 0) {
        ich = 1;
    } else if (fFleet == 0) {
        ich = 6;
        if (((int16_t)psz[1] - 32 & 0x2) != 0x0) {
            fItalic = 1;
        }
    } else {
        ich = 7;
    }
    crForeSav = SetTextColor(hdc, cr);
    bkSav = SetBkMode(hdc, TRANSPARENT);
    if (fItalic != 0) {
        hfntSav = SelectObject(hdc, rghfontArial8[3]);
    }
    pch = psz + ich;
    cch = strlen(pch) + 1;
    do {
        cch = cch - 1;
        dx = LOWORD(GetTextExtent(hdc, pch, cch));
    } while (dx > rcIn.right - rcIn.left);
    TextOut(hdc, rcIn.left, rcIn.top, pch, cch);
    if (fItalic != 0) {
        SelectObject(hdc, hfntSav);
    }
    if (ich >= 6) {
        if (((int16_t)psz[ich - 5] - 32 & 0x2) == 0x0) {
            szT[0] = 0;
        } else {
            if ((int16_t)psz[ich - 1] == '*') {
                ich = CchGetString(idsNeeded, szT);
                goto LRightOut;
            }
            CchGetString(idsUpTo, szT);
        }
        ich = strlen(szT);
        for (ichT = 2 - fFleet; ichT < 6 && (int16_t)psz[ichT + fFleet] == ' '; ichT++) {
        }
        strncpy(&szT[ich], psz + (ichT + fFleet), 6 - ichT);
        ich = ich + (6 - ichT);
        if (fFleet == 0 && ((int16_t)psz[fDoubleDraw + 1] - 32 & 0x1) != 0x0) {
            t_6648 = ich;
            ich = ich + 1;
            szT[t_6648] = '%';
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
    int16_t t_merge_66b6_0001;

    t_merge_66b6_0001 = lppl == 0x0 ? 0 : 1;
    fMinimal = t_merge_66b6_0001;
    if (fMinimal == 0) {
        lppl = &sel.pl;
        if (hwnd == 0x0) {
            hwnd = hwndPlanetProdLB;
        }
        SendMessage(hwnd, LB_RESETCONTENT, 0x0, 0);
    }
    if (lpplprod == 0x0) {
        lpplprod = lppl->lpplprod;
    }
    if (lpplprod != 0x0 && lpplprod->iprodMac != 0x0) {
        if (hwndProdDlg == 0x0)
            goto NoMsg;
        psz = PszGetCompressedString(idsTopQueue);
    } else {
        psz = PszGetCompressedString(idsQueueEmpty);
    }
    if (fMinimal != 0) {
        if (psz != szWork) {
            strcpy(szWork, psz);
        }
    } else {
        SendMessage(hwnd, LB_ADDSTRING, 0x0, (LPARAM)psz);
    }
NoMsg:
    if (lpplprod != 0x0) {
        resCost = 0;
        for (i = 0; i < 4; i++) {
            rgwtMin[i] = 0;
        }
        i = 0;
        lpprod = lpplprod->rgprod;
        while (1) {
            if (i >= lpplprod->iprodMac)
                goto L_6aab;
            psz = PszNameProdItem(lpprod);
            EstimateItemProdSched(lppl, lpplprod, i, &etaFirst, &etaLast);
            if ((etaFirst != 0 || etaLast != 0) && (etaFirst != -1 || etaLast != -1)) {
                if ((etaFirst > 1 && etaFirst < 100) || (etaFirst == 100 && lpprod->grobj == grobjPlanet && lpprod->iItem < mdIdleFactory)) {
                    ch = ' ';
                } else if (etaFirst != 1 || etaLast != 1) {
                    if (etaFirst >= 100) {
                        ch = '!';
                    } else {
                        ch = '#';
                    }
                } else {
                    ch = '*';
                }
            } else {
                if (fMinimal != 0)
                    goto L_680c;
                ch = '&';
            }
            cItem = lpprod->cItem;
            _wsprintf(szTemp, "%c%5d%s", (int16_t)ch, cItem, psz);
            if (lpprod->grobj == grobjPlanet) {
                if (lpprod->iItem < mdIdleFactory) {
                    szTemp[1] = szTemp[1] + 2;
                    if (lpprod->iItem == iobjAlchemy) {
                        szTemp[5] = '*';
                    }
                }
                switch (lpprod->iItem) {
                case mdIdleTerraform:
                case iobjMinTerraform:
                case iobjMaxTerraform:
                    szTemp[1] = szTemp[1] + 1;
                default:
                }
            }
            if (fMinimal != 0)
                break;
            SendMessage(hwnd, LB_ADDSTRING, 0x0, (LPARAM)szTemp);
        L_680c:
            i = i + 1;
            lpprod = lpprod + 1;
        }
        strcpy(szWork, szTemp);
        return;
    L_6aab:
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

    if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) != 0) {
        for (i = 0; i < 3; i++) {
            rgiValSav[i] = (int16_t)lppl->rgEnvVar[i];
            if ((int16_t)rgplr[iPlr].rgEnvVarMin[i] != -1 && (int16_t)lppl->rgEnvVar[i] != (int16_t)rgplr[iPlr].rgEnvVar[i]) {
                iNewVal = -1;
                if ((int16_t)lppl->rgEnvVar[i] >= (int16_t)rgplr[iPlr].rgEnvVar[i]) {
                    if (rgMin[i] != -1 && rgMin[i] < (int16_t)lppl->rgEnvVar[i]) {
                        iNewVal = (int16_t)rgplr[iPlr].rgEnvVar[i] <= rgMin[i] ? rgMin[i] : (int16_t)rgplr[iPlr].rgEnvVar[i];
                    }
                } else if (rgMax[i] > (int16_t)lppl->rgEnvVar[i]) {
                    iNewVal = (int16_t)rgplr[iPlr].rgEnvVar[i] >= rgMax[i] ? rgMax[i] : (int16_t)rgplr[iPlr].rgEnvVar[i];
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
    return PctPlanetDesirability(lppl, iPlr);
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
        iPlanet = (int16_t)lppl->rgEnvVar[i];
        iPref = (int16_t)rgplr[iPlr].rgEnvVar[i];
        iMin = (int16_t)rgplr[iPlr].rgEnvVarMin[i];
        iMax = (int16_t)rgplr[iPlr].rgEnvVarMax[i];
        if (iMax >= 0) {
            if (iPlanet < iMin || iPlanet > iMax) {
                if (iPlanet >= iMin) {
                    pctNeg = pctNeg + (15 >= iPlanet - iMax ? (int32_t)(iPlanet - iMax) : 15);
                } else {
                    pctNeg = pctNeg + (15 >= iMin - iPlanet ? (int32_t)(iMin - iPlanet) : 15);
                }
            } else {
                pctVar = abs(iPlanet - iPref) * 100;
                if (iPlanet >= iPref) {
                    d = iMax - iPref;
                    pctVar = (int32_t)pctVar / d;
                    dPenalty = (iPlanet - iPref) * 2 - d;
                } else {
                    d = iPref - iMin;
                    pctVar = (int32_t)pctVar / d;
                    dPenalty = (iPref - iPlanet) * 2 - d;
                }
                pctVar = 100 - pctVar;
                pctPos = pctPos + (uint32_t)((int32_t)pctVar * (int32_t)pctVar);
                if (dPenalty > 0) {
                    pctMod = (uint32_t)(pctMod * (int32_t)(d * 2 - dPenalty));
                    pctMod = (int32_t)(pctMod / (int32_t)(d * 2));
                }
            }
        } else {
            pctPos = pctPos + 10000;
        }
    }
    if (pctNeg != 0) {
        return -LOWORD(pctNeg);
    }
    pctPos = (int32_t)(sqrt((double)pctPos / 3.0) + 0.9);
    pctPos = (int32_t)((int32_t)(pctPos * pctMod) / 10000);
    return LOWORD(pctPos);
}

int32_t CalcPlanetMaxPop(int16_t idpl, int16_t iplr) {
    PLANET  pl;
    int32_t lMaxPop;
    int32_t pctDesire;
    int16_t ihuldef;

    FLookupPlanet(idpl, &pl);
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raMacintosh) {
        pctDesire = (int32_t)PctPlanetDesirability(&pl, iplr);
        if (pctDesire < 5) {
            lMaxPop = 500;
        } else {
            lMaxPop = (uint32_t)(pctDesire * 100);
        }
        if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raCheapCol) {
            if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raNone) {
                lMaxPop = lMaxPop + (int32_t)(lMaxPop / 5);
            }
        } else {
            lMaxPop = lMaxPop - (int32_t)(lMaxPop / 2);
        }
    } else {
        if (pl.iPlayer != iplr || pl.fStarbase == 0x0) {
            return 0;
        }
        ihuldef = rglpshdefSB[iplr][pl.isb].hul.ihuldef - 32;
        lMaxPop = rglPopMac[ihuldef];
    }
    if (GetRaceGrbit(&rgplr[iplr], ibitRaceOBRM) != 0) {
        lMaxPop = lMaxPop + (int32_t)(lMaxPop / 10);
    }
    return lMaxPop;
}

int16_t CMaxMines(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsMineOperate);
    cMax = (int32_t)((int32_t)(lPopMax * (int32_t)iEff) / 0x64);
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
    int16_t t_merge_73ba_0001;

    cMax = CMaxMines(lppl, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsMineOperate);
    lPop = lppl->rgwtMin[3];
    if (fNextYear != 0) {
        lPop = lPop + ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)((int32_t)(lPop * (int32_t)iEff) / 0x64);
    t_merge_73ba_0001 = (int32_t)cMax < cCur ? cMax : LOWORD(cCur);
    cMax = t_merge_73ba_0001;
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
    if (iplr != -1) {
        if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raMacintosh) {
            cMines = lppl->cMines;
            cMinesOp = CMaxOperableMines(lppl, lppl->iPlayer, 0);
            if (cMines > cMinesOp) {
                cMines = cMinesOp;
            }
            return cMines;
        }
        t_call_7452 = sqrt((double)lppl->rgwtMin[3]);
        return LOWORD((int32_t)t_call_7452);
    }
    return 0;
}

int16_t CFactoriesOperating(PLANET *lppl) {
    int16_t iplr;
    int16_t cFacts;
    int16_t cFactsOp;

    iplr = lppl->iPlayer;
    if (iplr != -1) {
        if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raMacintosh) {
            cFacts = lppl->cFactories;
            cFactsOp = CMaxOperableFactories(lppl, lppl->iPlayer, 0);
            if (cFacts > cFactsOp) {
                cFacts = cFactsOp;
            }
            return cFacts;
        }
        return 0;
    }
    return 0;
}

int16_t CMaxFactories(PLANET *lppl, int16_t iplr) {
    int32_t cMax;
    int32_t lPopMax;
    int16_t iEff;

    lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsFactOperate);
    cMax = (int32_t)((int32_t)(lPopMax * (int32_t)iEff) / 0x64);
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
    int16_t t_merge_76ce_0001;

    cMax = CMaxFactories(lppl, iplr);
    iEff = GetRaceStat(&rgplr[iplr], rsFactOperate);
    lPop = lppl->rgwtMin[3];
    if (fNextYear != 0) {
        lPop = lPop + ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)((int32_t)(lPop * (int32_t)iEff) / 0x64);
    t_merge_76ce_0001 = (int32_t)cMax < cCur ? cMax : LOWORD(cCur);
    cMax = t_merge_76ce_0001;
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
    int16_t t_merge_7779_0001;

    pctDesire = PctPlanetDesirability(lppl, iplr);
    if (0x64 >= (10 <= pctDesire * 4 ? pctDesire * 4 : 0xa)) {
        if (10 <= pctDesire * 4) {
            t_merge_7779_0001 = pctDesire * 4;
        } else {
            t_merge_7779_0001 = 10;
        }
    } else {
        t_merge_7779_0001 = 100;
    }
    cMax = t_merge_7779_0001;
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
        lPop = lPop + ChgPopFromPlanet(lppl, 0);
    }
    cCur = (int32_t)((lPop + 24) / 0x19);
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

    if (lppl->rgwtMin[3] != 0) {
        iEff = GetRaceStat(&rgplr[iplr], rsResGen);
        lPop = lppl->rgwtMin[3];
        lPopMax = CalcPlanetMaxPop(lppl->id, iplr);
        if (lPop > lPopMax) {
            lPop = (int32_t)((lPop - lPopMax) / 0x2) + lPopMax;
            if (lPop > (int32_t)(lPopMax * 2)) {
                lPop = (int32_t)(lPopMax * 2);
            }
        }
        if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raMacintosh) {
            cRes = LOWORD((int32_t)(lPop / (int32_t)iEff));
            cFact = CMaxOperableFactories(lppl, iplr, 0);
            if (lppl->cFactories < cFact) {
                cFact = lppl->cFactories;
            }
            iEff = GetRaceStat(&rgplr[iplr], rsFactProd);
            cRes = cRes + LOWORD((int32_t)((int32_t)((uint32_t)((int32_t)cFact * (int32_t)iEff) + 0x9) / 0xa));
        } else {
            iEnergy = (int16_t)rgplr[iplr].rgTech[0];
            pctVal = PctPlanetDesirability(lppl, iplr);
            if (iEnergy < 1) {
                iEnergy = 1;
            }
            if (pctVal < 25) {
                pctVal = 25;
            }
            cRes = LOWORD((int32_t)(sqrt((double)lPop * (double)(int32_t)iEnergy / (double)(int32_t)iEff) * (double)(int32_t)pctVal / 10.0 + 0.999));
        }
        if (cRes == 0) {
            cRes = 1;
        }
        return cRes;
    }
    return 0;
}

int16_t IWarpMAFromLppl(PLANET *lppl, int16_t *pfTwo) {
    int16_t fTwo;
    int16_t iWarp;
    int16_t i;
    HUL    *lphul;
    int16_t iNew;

    iWarp = 0;
    fTwo = 0;
    if (pfTwo != 0x0) {
        *pfTwo = 0;
    }
    if (lppl->iPlayer != -1 && lppl->fStarbase != 0x0) {
        if (lppl->iPlayer == idPlayer || idPlayer == -1 || rglpshdefSB[lppl->iPlayer][lppl->isb].det == 0x7) {
            lphul = &rglpshdefSB[lppl->iPlayer][lppl->isb].hul;
            for (i = 0; i < lphul->chs; i++) {
                if (lphul->rghs[i].grhst == hstSpecialSB && lphul->rghs[i].cItem > 0x0 && lphul->rghs[i].iItem >= 0x7 && lphul->rghs[i].iItem <= 0xf) {
                    iNew = lphul->rghs[i].iItem - 2;
                    if (iNew <= iWarp) {
                        if (iNew == iWarp) {
                            fTwo = 1;
                        }
                    } else {
                        fTwo = 0;
                        iWarp = iNew;
                    }
                }
            }
            if (pfTwo != 0x0) {
                *pfTwo = fTwo;
            }
            return iWarp;
        }
        return 0;
    }
    return 0;
}

int16_t StargateRangeFromLppl(PLANET *lppl, int16_t iplr, int16_t ish) {
    int16_t i;
    HUL    *lphul;
    PART    part;

    if (lppl != 0x0) {
        if (lppl->iPlayer == -1 || lppl->fStarbase == 0x0) {
            return 0;
        }
        lphul = &rglpshdefSB[lppl->iPlayer][lppl->isb].hul;
    } else {
        lphul = &rglpshdefSB[iplr][ish].hul;
    }
    i = 0;
    while (1) {
        if (i >= lphul->chs) {
            return 0;
        }
        if (lphul->rghs[i].grhst == hstSpecialSB && lphul->rghs[i].cItem > 0x0 && lphul->rghs[i].iItem >= 0x0 && lphul->rghs[i].iItem <= 0x6)
            break;
        i = i + 1;
    }
    part.hs = lphul->rghs[i];
    FLookupPart(&part);
    if (part.pspecialsb->grAbility2 != -1) {
        return part.pspecialsb->grAbility2;
    }
    return 10000;
}

int16_t FProdIsTerra(PROD *lpprod) {
    if (lpprod->grobj == grobjPlanet) {
        switch (lpprod->iItem) {
        case mdIdleTerraform:
        case iobjMinTerraform:
        case iobjMaxTerraform:
            return 1;
        default:
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

    if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) != 0) {
        ipct = 0;
        for (i = 0; i < 3; i++) {
            if (rgMin[i] != -1) {
                ipct = ipct + ((int16_t)lppl->rgEnvVar[i] - rgMin[i]);
            }
            if (rgMax[i] != -1) {
                ipct = ipct + (rgMax[i] - (int16_t)lppl->rgEnvVar[i]);
            }
        }
        return ipct;
    }
    return 0;
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
    if (i < 0) {
        fRet = 0;
        for (i = 0; i < 3; i++) {
            rgMove[i] = 0;
        }
    } else {
        fRet = 1;
        for (i = 0; i < 3; i++) {
            rgMove[i] = part.pterra->grAbility;
            rgEnvCost[i] = part.pterra->resCost;
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
    if (fRet != 0) {
        for (i = 0; i < 3; i++) {
            if (rgMove[i] != 0 && (int16_t)rgplr[idPlayer].rgEnvVarMin[i] != -1) {
                rgEnvMin[i] = (int16_t)lppl->rgEnvVarOrig[i] - rgMove[i];
                rgEnvMax[i] = (int16_t)lppl->rgEnvVarOrig[i] + rgMove[i];
                if (rgEnvMin[i] < (int16_t)lppl->rgEnvVar[i]) {
                    rgEnvMin[i] = 1 <= rgEnvMin[i] ? rgEnvMin[i] : 1;
                } else {
                    rgEnvMin[i] = -1;
                }
                if (rgEnvMax[i] > (int16_t)lppl->rgEnvVar[i]) {
                    rgEnvMax[i] = 99 >= rgEnvMax[i] ? rgEnvMax[i] : 99;
                } else {
                    rgEnvMax[i] = -1;
                }
                if (fHelp == 0) {
                    ienvIdeal = (int16_t)rgplr[idPlayer].rgEnvVar[i];
                    dCur = abs((int16_t)lppl->rgEnvVar[i] - ienvIdeal);
                    if (rgEnvMin[i] == -1) {
                        dMin = 0;
                    } else {
                        dMin = abs(rgEnvMin[i] - ienvIdeal);
                    }
                    if (rgEnvMax[i] == -1) {
                        dMax = 0;
                    } else {
                        dMax = abs(rgEnvMax[i] - ienvIdeal);
                    }
                    if (dCur < dMin || dCur < dMax) {
                        if (dMin < dMax) {
                            rgEnvMin[i] = -1;
                        } else {
                            rgEnvMax[i] = -1;
                        }
                    } else {
                        rgEnvMax[i] = -1;
                        rgEnvMin[i] = -1;
                    }
                } else if ((int16_t)lppl->rgEnvVar[i] != (int16_t)rgplr[idPlayer].rgEnvVar[i]) {
                    if ((int16_t)lppl->rgEnvVar[i] <= (int16_t)rgplr[idPlayer].rgEnvVar[i]) {
                        rgEnvMin[i] = -1;
                        if (rgEnvMax[i] != -1) {
                            rgEnvMax[i] = rgEnvMax[i] >= (int16_t)rgplr[idPlayer].rgEnvVar[i] ? (int16_t)rgplr[idPlayer].rgEnvVar[i] : rgEnvMax[i];
                        }
                    } else {
                        rgEnvMax[i] = -1;
                        if (rgEnvMin[i] != -1) {
                            rgEnvMin[i] = rgEnvMin[i] <= (int16_t)rgplr[idPlayer].rgEnvVar[i] ? (int16_t)rgplr[idPlayer].rgEnvVar[i] : rgEnvMin[i];
                        }
                    }
                } else {
                    rgEnvMax[i] = -1;
                    rgEnvMin[i] = -1;
                }
            } else {
                rgEnvMax[i] = -1;
                rgEnvMin[i] = -1;
            }
        }
        for (i = 0; i < 3 && (rgEnvMax[i] == -1 && rgEnvMin[i] == -1); i++) {
        }
        idPlayer = iPlrSav;
        if (i == 3) {
            return 0;
        }
        return 1;
    }
    idPlayer = iPlrSav;
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
    if (lppl->lpplprod != 0x0) {
        FreePl((PL *)lpPlanets[lppl->id].lpplprod);
        lpPlanets[lppl->id].lpplprod = 0x0;
        lppl->lpplprod = 0x0;
    }
    lppl->fNoResearch = 0x0;
    lppl->fStarbase = 0x0;
    lppl->cDefenses = 0x0;
    lppl->iScanner = 0x1f;
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
    if (iplr == -1 || lphul->ihuldef < ihuldefOrbitalFort || GetRaceGrbit(&rgplr[iplr], ibitRaceISB) == 0) {
        cPts = 0;
    } else {
        cPts = 40;
    }
    if (iplr != -1 && GetRaceStat(&rgplr[iplr], rsMajorAdv) == raStealth) {
        cPts = cPts + 300;
    }
    if (ppctSteal != 0x0) {
        *ppctSteal = 0;
    }
    j = 0;
    lphs = lphul->rghs;
    while (j < chs) {
        cPts = cPts + (int32_t)CPtsCloakFromLphs(lphs);
        if (lphs->grhst == hstScanner && ppctSteal != 0x0) {
            if (lphs->iItem != iscannerPickPocketScanner) {
                if (lphs->iItem == iscannerRobberBaronScanner && *ppctSteal < 80) {
                    *ppctSteal = 80;
                }
            } else if (*ppctSteal < 70) {
                *ppctSteal = 70;
            }
        }
        j = j + 1;
        lphs = lphs + 1;
    }
    if (cPts != 0) {
        if (cPts >= 0 && cPts <= 25000) {
            cScore = LOWORD(cPts);
            if (cScore > 100) {
                cScore = cScore - 100;
                if (cScore > 200) {
                    cScore = cScore - 200;
                    if (cScore > 312) {
                        cScore = cScore - 312;
                        if (cScore > 512) {
                            if (cScore >= 1000) {
                                return 98;
                            }
                            return (cScore < 768 ? 0x0 : 0x1) + 0x60;
                        }
                        return (cScore >> 0x6) + 0x58;
                    }
                    return (int32_t)cScore / 24 + 0x4b;
                }
                return (cScore >> 0x3) + 0x32;
            }
            return cScore >> 0x1;
        }
        return 0;
    }
    return 0;
}
