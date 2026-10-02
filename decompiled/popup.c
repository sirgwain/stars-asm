#include "common.h"

BattleUnitFlags mpimdgrbitBU[8] = {grBuClassUnarmed, grBuClassUnarmed, grBuClassScout,   grBuClassWarship,
                                   grBuClassUtility, grBuClassBomber,  grBuClassUnarmed, grBuClassUnarmed};

LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rc;

    switch (message) {
    case WM_ERASEBKGND:
        switch (GlobalPD.grPopup) {
        case grPopupComponent:
        case grPopupShdef:
        case grPopupShdefSB:
            GetClientRect(hwnd, &rc);
            FillRect((HDC)wParam, &rc, hbrButtonFace);
            return 1;
        default:
            goto Default;
        }
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawPopup(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        if (hwndPopup == 0) {
            return 0;
        }
        DestroyWindow(hwndPopup);
        hwndPopup = 0;
        GlobalPD.grPopup = 0;
        ReleaseCapture();
        if (gd.fTutorial == 0) {
            return 0;
        }
        tutor.fProgress = 1;
        AdvanceTutor();
        return 0;
    default:
    Default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    case WM_CREATE:
        return 0;
    }
}

int16_t FIsPopupHullType(int16_t ishdef) {
    HullCategory imd;

    if (GlobalPD.grbit == 0 || GlobalPD.grbit == 0xff) {
        return 1;
    }
    imd = LphuldefFromId(rglpshdef[GlobalPD.lpfl->iPlayer][ishdef].hul.ihuldef)->imdCategory;
    return mpimdgrbitBU[imd] & GlobalPD.grbit;
}

void DrawPopup(HWND hwnd, HDC hdc) {
    COLORREF crBack;
    char     szT[80];
    int16_t  yCur;
    int16_t  i;
    int16_t  c;
    int16_t  bkMode;
    HFONT    hfontSav;
    char    *psz;
    int16_t  dx;
    COLORREF crFore;
    RECT     rc;
    char    *lpsz;
    int16_t  csh;
    int16_t  dpT;
    char     szTB[40];

    crBack = SetBkColor(hdc, 0xffffff);
    crFore = SetTextColor(hdc, 0);
    bkMode = SetBkMode(hdc, OPAQUE);
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    GetClientRect(hwnd, &rc);
    if ((uint16_t)(GlobalPD.grPopup - 1) <= 13) {
        switch (GlobalPD.grPopup) {
        case grPopupMineral:
            CtrTextOut(hdc, rc.right >> 1, 4, rgszMinerals[GlobalPD.rgi[0]], 0);
            SelectObject(hdc, rghfontArial8[0]);
            psz = PszGetCompressedString(idsMineralConcentration);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;
            RightTextOut(hdc, dx, dyArial8 + 4, PszGetCompressedString(idsSurface), 0, 0);
            if (GlobalPD.rgi[2] >= 0) {
                c = _wsprintf(szWork, "%ldkT", GlobalPD.rgi[2]);
            } else {
                strcpy(szWork, PszGetCompressedString(idsUnknown2));
                c = strlen(szWork);
            }
            TextOut(hdc, dx, dyArial8 + 4, szWork, c);
            RightTextOut(hdc, dx, dyArial8 * 2 + 4, PszGetCompressedString(idsMineralConcentration), 0, 0);
            if (GlobalPD.rgi[3] > 0) {
                c = _wsprintf(szWork, PCTLD, GlobalPD.rgi[3]);
                if (GlobalPD.rgi[1] != 0) {
                    c += _wsprintf(&szWork[c], PszGetCompressedString(GlobalPD.rgi[3] < 30 ? idsN30 : idsHw));
                }
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsUnknown2));
            }
            TextOut(hdc, dx, dyArial8 * 2 + 4, szWork, c);
            if (GlobalPD.rgi[4] < 0)
                break;
            RightTextOut(hdc, dx, 3 * dyArial8 + 4, PszGetCompressedString(idsMiningRate), 0, 0);
            CchGetString(idsLdktYr, szT);
            c = _wsprintf(szWork, szT, GlobalPD.rgi[4]);
            TextOut(hdc, dx, 3 * dyArial8 + 4, szWork, c);
            break;
        case grPopupPlayer:
            CtrTextOut(hdc, rc.right >> 1, 4, PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, NULL), 0);
            c = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), GlobalPD.iPlayer + 1);
            CtrTextOut(hdc, rc.right >> 1, dyArial8 + 4, szWork, c);
            break;
        case grPopupFleet:
            if (rc.bottom - rc.top < dyArial8 * 2) {
                c = CchGetString(idsNone2, szWork);
                TextOut(hdc, 4, 4, szWork, c);
                SelectObject(hdc, rghfontArial8[0]);
                break;
            }
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsShipName, szWork);
            TextOut(hdc, 4, 4, szWork, c);
            RightTextOut(hdc, rc.right - 4 - GlobalPD.dxDamage, 4, "#", 1, 0);
            if (GlobalPD.dxDamage != 0) {
                c = CchGetString(idsDamage2, szWork);
                RightTextOut(hdc, rc.right - 4, 4, szWork, c, 0);
                PatBlt(hdc, 4, dyArial8 + 2, rc.right - 8, 1, BLACKNESS);
            }
            SelectObject(hdc, rghfontArial8[0]);
            yCur = dyArial8 + 4;
            for (i = 0; i < 16; i++) {
                if (GlobalPD.lpfl->rgcsh[i] > 0 && (GlobalPD.grbit == 0 || FIsPopupHullType(i) != 0)) {
                    if (GlobalPD.fRedDamage != 0) {
                        if (GlobalPD.lpfl->rgdv[i].dp != 0) {
                            SetTextColor(hdc, 0xff);
                        } else {
                            SetTextColor(hdc, 0);
                        }
                    }
                    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
                    lpsz = szTB;
                    TextOut(hdc, 4, yCur, lpsz, fstrlen(lpsz));
                    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
                    RightTextOut(hdc, rc.right - 4 - GlobalPD.dxDamage, yCur, szWork, c, 0);
                    if (GlobalPD.fRedDamage != 0 && GlobalPD.lpfl->rgdv[i].dp != 0) {
                        csh = GlobalPD.lpfl->rgcsh[i];
                        csh = LOWORD((int32_t)((uint32_t)(GlobalPD.lpfl->rgdv[i].dp & 0x7f) * csh) / 0x64);
                        if (csh <= 0) {
                            csh = 1;
                        }
                        dpT = (uint32_t)(GlobalPD.lpfl->rgdv[i].dp >> 7 & 0x1ff) / 5;
                        if (dpT == 0) {
                            dpT = 1;
                        }
                        c = _wsprintf(szWork, "%d@%d%%", csh, dpT);
                        RightTextOut(hdc, rc.right - 4, yCur, szWork, c, 0);
                    }
                    yCur += dyArial8;
                }
            }
            if (GlobalPD.fRedDamage == 0)
                break;
            SetTextColor(hdc, 0);
            break;
        case grPopupUnknownObj:
            psz = PszGetCompressedString(idsPlanet);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, dx, 4, psz, strlen(psz), 0);
            RightTextOut(hdc, dx, dyArial8 + 4, PszGetCompressedString(idsId), 0, 0);
            RightTextOut(hdc, dx, dyArial8 * 2 + 4, PszGetCompressedString(idsX), 0, 0);
            RightTextOut(hdc, dx, 3 * dyArial8 + 4, PszGetCompressedString(idsY), 0, 0);
            psz = PszGetPlanetName(sel.scan.idpl);
            SelectObject(hdc, rghfontArial8[0]);
            TextOut(hdc, dx, 4, psz, strlen(psz));
            c = _wsprintf(szWork, PCTD, sel.scan.idpl + 1);
            TextOut(hdc, dx, dyArial8 + 4, szWork, c);
            c = _wsprintf(szWork, PCTD, sel.scan.pt.x);
            TextOut(hdc, dx, dyArial8 * 2 + 4, szWork, c);
            c = _wsprintf(szWork, PCTD, sel.scan.pt.y);
            TextOut(hdc, dx, 3 * dyArial8 + 4, szWork, c);
            break;
        case grPopupPlanetEnv:
            PtDisplayPlanetStateInfo(hdc, 1);
            break;
        case grPopupShipOrders:
            PtDisplayZipOrdInfo(hdc, rc.right >> 1, 1);
            break;
        case grPopupPlanet:
            PtDisplayPlanetPopInfo(hdc, 1);
            break;
        case grPopupPlanetIndustry:
            PtDisplayFactoryMineInfo(hdc, rc.right, 1);
            break;
        case grPopupResources:
            PtDisplayResourceInfo(hdc, rc.right, 1);
            break;
        case grPopupComponent:
            DisplayComponentInfo(hdc, rc.right, rc.bottom, &GlobalPD.part);
            break;
        case grPopupString:
            PtDisplayString(hdc, rc.right, 1);
            break;
        case grPopupShdef:
        case grPopupShdefSB:
            fStarbaseMode = (int16_t)GlobalPD.lpshdef->hul.ihuldef < ihuldefOrbitalFort ? 0 : 1;
            DrawSlotDlg(hwnd, hdc, &rc, -1);
            rc.top = dyArial8 + 306;
            rc.left += 6;
            SelectObject(hdc, rghfontArial8[1]);
            SetBkMode(hdc, TRANSPARENT);
            fstrcpy(szWork, GlobalPD.lpshdef->hul.szClass);
            CtrTextOut(hdc, ((rc.right - 0x4c) >> 1) + 0x4c, 6, szWork, 0);
            DrawBuildSelHull(hwnd, hdc, -1, &rc);
        }
    }
    SelectObject(hdc, hfontSav);
    SetBkMode(hdc, bkMode);
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    return;
}

void Popup(HWND hwnd, int16_t x, int16_t y) {
    HDC     hdc;
    POINT16 pt;
    int16_t dy;
    int16_t i;
    int16_t c;
    HFONT   hfontSav;
    char   *psz;
    int16_t dx;
    POINT16 ptT;
    int16_t dx2;
    int16_t dxDamage;
    int16_t dxL;
    char   *lpsz;
    int16_t dxR;
    char    szTB[40];
    int16_t dxName;
    int16_t dxCoord;
    int16_t t_merge_126d_0001;
    int16_t t_call_1265;
    int16_t t_merge_12a3_0001;
    int16_t t_call_129b;
    int16_t t_merge_12cc_0001;
    int16_t t_call_12c4;
    int16_t t_merge_1302_0001;
    int16_t t_call_12fa;

    pt.x = x;
    pt.y = y;
    ClientToScreen16(hwnd, &pt);
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    if ((uint16_t)(GlobalPD.grPopup - 1) <= 13) {
        switch (GlobalPD.grPopup) {
        case grPopupMineral:
            psz = PszGetCompressedString(idsMineralConcentration0000000kt);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dy = 3 * dyArial8 + 8;
            if (GlobalPD.rgi[4] < 0)
                break;
            dy += dyArial8;
            break;
        case grPopupPlayer:
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, NULL);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dx2 = LOWORD(GetTextExtent(hdc, "Player #16", 10)) + 8;
            if (dx2 > dx) {
                dx = dx2;
            }
            dy = dyArial8 * 2 + 8;
            break;
        case grPopupFleet:
            dxR = 0;
            dxDamage = 0;
            dy = dyArial8 + 8;
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetCompressedString(idsShipName);
            dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            SelectObject(hdc, rghfontArial8[0]);
            for (i = 0; i < 16; i++) {
                if (GlobalPD.lpfl->rgcsh[i] > 0 && (GlobalPD.grbit == 0 || FIsPopupHullType(i) != 0)) {
                    dy += dyArial8;
                    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
                    lpsz = szTB;
                    dx = LOWORD(GetTextExtent(hdc, lpsz, fstrlen(lpsz)));
                    dxL = dxL <= dx ? dx : dxL;
                    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
                    dx = LOWORD(GetTextExtent(hdc, szWork, c));
                    dxR = dxR <= dx ? dx : dxR;
                    if (GlobalPD.fRedDamage != 0 && (GlobalPD.lpfl->rgdv[i].dp >> 7 & 0x1ff) != 0 && dxDamage == 0) {
                        psz = PszGetCompressedString(idsN9999999);
                        dxDamage = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;
                    }
                }
            }
            if (dy == dyArial8 + 8) {
                psz = PszGetCompressedString(idsShipName);
                dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            }
            GlobalPD.dxDamage = dxDamage;
            dx = dxL + dxR + 16 + dxDamage;
            break;
        case grPopupUnknownObj:
            SelectObject(hdc, rghfontArial8[1]);
            dy = dyArial8 * 4 + 8;
            psz = PszGetCompressedString(idsPlanet);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            psz = PszGetPlanetName(sel.scan.idpl);
            SelectObject(hdc, rghfontArial8[0]);
            dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 4));
            dx += dxName <= dxCoord ? dxCoord : dxName;
            break;
        case grPopupPlanetEnv:
            ptT = PtDisplayPlanetStateInfo(hdc, 0);
            goto SetDxDy;
        case grPopupShipOrders:
            ptT = PtDisplayZipOrdInfo(hdc, 0, 0);
            goto SetDxDy;
        case grPopupPlanet:
            ptT = PtDisplayPlanetPopInfo(hdc, 0);
            goto SetDxDy;
        case grPopupResources:
            ptT = PtDisplayResourceInfo(hdc, 200, 0);
            goto SetDxDy;
        case grPopupPlanetIndustry:
            ptT = PtDisplayFactoryMineInfo(hdc, 200, 0);
            goto SetDxDy;
        case grPopupComponent:
            dx = (dyArial8 <= 14 ? 0 : 40) + 344;
            dy = dyArial10 + 72 + 12 * dyArial8 + 6;
            break;
        case grPopupString:
            ptT = PtDisplayString(hdc, GlobalPD.dxOut, 0);
            goto SetDxDy;
        case grPopupShdef:
        case grPopupShdefSB:
            mdBuild = GlobalPD.grPopup == grPopupShdef ? mdBuildShdef : mdBuildHuldef;
            lpshdefBuild = GlobalPD.lpshdef;
            UpdateSlotGlobals();
            dx = 340;
            dy = dyArial8 + 306 + 6 * dyArial8 + 8;
            if (gd.mdScreenSize <= 0 || GlobalPD.grPopup != grPopupShdef)
                break;
            dy += 3 * dyArial8;
            break;
        case grPopupUnknown:
            dx = 120;
            dy = 80;
        }
        goto L_1225;
    SetDxDy:
        dx = ptT.x + 2;
        dy = ptT.y + 2;
    }
L_1225:
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    pt.x -= dx;
    pt.y -= dy;
    if (pt.x < GetSystemMetrics(SM_CXSCREEN) - dx) {
        t_merge_126d_0001 = pt.x;
    } else {
        t_call_1265 = GetSystemMetrics(SM_CXSCREEN);
        t_merge_126d_0001 = t_call_1265 - dx;
    }
    if (0 > t_merge_126d_0001) {
        t_merge_12a3_0001 = 0;
    } else if (pt.x < GetSystemMetrics(SM_CXSCREEN) - dx) {
        t_merge_12a3_0001 = pt.x;
    } else {
        t_call_129b = GetSystemMetrics(SM_CXSCREEN);
        t_merge_12a3_0001 = t_call_129b - dx;
    }
    pt.x = t_merge_12a3_0001;
    if (pt.y < GetSystemMetrics(SM_CYSCREEN) - dy) {
        t_merge_12cc_0001 = pt.y;
    } else {
        t_call_12c4 = GetSystemMetrics(SM_CYSCREEN);
        t_merge_12cc_0001 = t_call_12c4 - dy;
    }
    if (0 > t_merge_12cc_0001) {
        t_merge_1302_0001 = 0;
    } else if (pt.y < GetSystemMetrics(SM_CYSCREEN) - dy) {
        t_merge_1302_0001 = pt.y;
    } else {
        t_call_12fa = GetSystemMetrics(SM_CYSCREEN);
        t_merge_1302_0001 = t_call_12fa - dy;
    }
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, NULL, WS_POPUP | WS_VISIBLE | WS_BORDER, pt.x, pt.y, dx, dy, hwnd, NULL, hInst, NULL);
    SendMessage(hwndPopup, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
    SetCapture(hwndPopup);
    return;
}

int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char   *pszTitle;
    int16_t tpm;
    POINT16 pt;
    int16_t i;
    char    szTemp[128];
    HMENU   hmenuSub;
    HMENU   hmenuPopup;
    char   *pszT;
    char   *psz;
    MSG     msg;
    int16_t fChecked;
    int16_t fCheckedCur;
    char   *t_1545;
    char   *t_16d0;
    char   *t_17e3;
    int32_t t_merge_184f_0001;

    hmenuSub = 0;
    pt.x = x;
    pt.y = y;
    ClientToScreen16(hwnd, &pt);
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    for (i = 0; i < cString; i++) {
        if (rgids != 0 && (iChecked != -2 || rgsz == 0)) {
            if (rgids[i] == -1) {
                AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
            } else {
                if ((rgids[i] & 0x10000000) != 0) {
                    psz = "Deep Space";
                } else if ((rgids[i] & 0x40000000) != 0) {
                    psz = PszGetCompressedString(LOWORD(rgids[i]));
                } else if ((rgids[i] & 0x20000000) != 0) {
                    psz = PszGetThingName(LOWORD(rgids[i]));
                } else if ((rgids[i] & 0x80000000) != 0) {
                    psz = PszGetFleetName(LOWORD(rgids[i]) | 0x8000);
                } else {
                    psz = PszGetPlanetName(LOWORD(rgids[i]));
                }
                pszT = szTemp;
                while (*psz != 0) {
                    t_1545 = psz;
                    psz++;
                    *pszT++ = *t_1545;
                    if (*t_1545 == '&') {
                        *pszT++ = '&';
                    }
                }
                *pszT = 0;
                AppendMenu(hmenuPopup, i == iChecked ? MF_CHECKED : MF_BYCOMMAND, i + 15000, szTemp);
            }
        } else if (rgsz[i] == 0) {
            pszTitle = rgsz[i + 1];
            fChecked = rgids == 0 ? 0 : LOWORD(rgids[i + 1]);
            hmenuSub = CreatePopupMenu();
            for (i += 2; i < cString && rgsz[i] != 0; i++) {
                if (rgids == 0) {
                    fCheckedCur = i == iChecked ? 1 : 0;
                    fChecked |= fCheckedCur;
                } else {
                    fCheckedCur = LOWORD(rgids[i]);
                }
                if (*rgsz[i] == -1 && rgsz[i][1] == 0) {
                    AppendMenu(hmenuSub, MF_SEPARATOR, 0, NULL);
                } else {
                    pszT = szTemp;
                    psz = rgsz[i];
                    while (*psz != 0) {
                        t_16d0 = psz;
                        psz++;
                        *pszT++ = *t_16d0;
                        if (*t_16d0 == '&') {
                            *pszT++ = '&';
                        }
                    }
                    *pszT = 0;
                    AppendMenu(hmenuSub, fCheckedCur == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);
                }
            }
            AppendMenu(hmenuPopup, (fChecked == 0 ? 0 : 8) | 0x10, (UINT_PTR)hmenuSub, pszTitle);
        } else if (*rgsz[i] == -1 && rgsz[i][1] == 0) {
            AppendMenu(hmenuPopup, MF_SEPARATOR, 0, NULL);
        } else {
            pszT = szTemp;
            psz = rgsz[i];
            while (*psz != 0) {
                t_17e3 = psz;
                psz++;
                *pszT++ = *t_17e3;
                if (*t_17e3 == '&') {
                    *pszT++ = '&';
                }
            }
            *pszT = 0;
            t_merge_184f_0001 = iChecked == -2 ? rgids[i] : i == iChecked;
            AppendMenu(hmenuPopup, t_merge_184f_0001 == 0 ? MF_BYCOMMAND : MF_CHECKED, i + 15000, szTemp);
        }
    }
    if (fRightBtn != 0) {
        tpm = TPM_RIGHTBUTTON;
    } else {
        tpm = TPM_LEFTBUTTON;
    }
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, NULL);
    DestroyMenu(hmenuPopup);
    if (hmenuSub != 0) {
        DestroyMenu(hmenuSub);
    }
    if (PeekMessage(&msg, hwndFrame, 273, 273, 2) != 0 && msg.wParam >= 15000 && msg.wParam < 15100) {
        iPopMenuSel = msg.wParam - 15000;
    }
    return iPopMenuSel;
}

POINT16 PtDisplayPlanetStateInfo(HDC hdc, int16_t fPrint) {
    POINT16  pt;
    int16_t  y;
    int16_t  xMax;
    int16_t  cch;
    int16_t  x;
    int16_t  iNewVal;
    PLANET  *lppl;
    int16_t  pctDesireOld;
    int16_t  pctDesire;
    int16_t  iValSav;
    StringId ids;
    int16_t  dChg;
    char     szOut[90];

    y = 4;
    xMax = 4;
    x = 4;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsCurrently), 0, fPrint);
    SelectObject(hdc, rghfontArial8[1]);
    if (GlobalPD.iPlanVal >= 0) {
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanVal), 0, fPrint);
    } else {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnknown2), 0, fPrint);
    }
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    if (GlobalPD.iPlrMin == -1) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsImmune), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y += dyArial8;
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsEffects), 0, fPrint);
        DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    } else {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsPreferPlanetsWhere), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y += dyArial8;
        DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsBetween), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlrMin), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsAnd), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlrMax), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    }
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    if (GlobalPD.iPlanMin > -1) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsCurrentlyPossessTechnology), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y += dyArial8;
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsModify), 0, fPrint);
        DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsOn2), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y += dyArial8;
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWithinRange), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanMin), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsTo), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanMax), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        y += dyArial8;
    }
    x = 4;
    if (GlobalPD.iPlanMin > -1 && GlobalPD.iPlrMin != -1 && GlobalPD.iPlanVal != GlobalPD.iPlrVal) {
        iNewVal = -1;
        if (GlobalPD.iPlanVal < GlobalPD.iPlrVal) {
            if (GlobalPD.iPlanMax > GlobalPD.iPlanVal) {
                iNewVal = GlobalPD.iPlrVal >= GlobalPD.iPlanMax ? GlobalPD.iPlanMax : GlobalPD.iPlrVal;
            }
        } else if (GlobalPD.iPlanMin < GlobalPD.iPlanVal) {
            iNewVal = GlobalPD.iPlrVal <= GlobalPD.iPlanMin ? GlobalPD.iPlanMin : GlobalPD.iPlrVal;
        }
        if (iNewVal != -1) {
            lppl = LpplFromId(GlobalPD.idPlanet);
            pctDesireOld = PctPlanetDesirability(lppl, idPlayer);
            iValSav = lppl->rgEnvVar[GlobalPD.iPlanetVar];
            lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iNewVal);
            pctDesire = PctPlanetDesirability(lppl, idPlayer);
            lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iValSav);
            if (pctDesireOld < pctDesire) {
                cch = CchGetString(idsIfTerraform, szWork);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, NULL, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                WrapTextOut(hdc, &x, &y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, 4, xMax - 4, NULL, 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                WrapTextOut(hdc, &x, &y, PszGetCompressedString(idsTo), 0, 4, xMax - 4, NULL, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                WrapTextOut(hdc, &x, &y, PszCalcEnvVar(GlobalPD.iPlanetVar, iNewVal), 0, 4, xMax - 4, NULL, 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                cch = CchGetString(idsPlanetsValueWouldImprove, szWork);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, NULL, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                cch = _wsprintf(szWork, "%d%%.  ", pctDesire);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, NULL, 0, fPrint);
            }
        }
    }
    if (GlobalPD.iPlrMin != -1 && GlobalPD.iPlanVal != GlobalPD.iPlrVal && GlobalPD.iPlanVal != -1) {
        if (GlobalPD.iPlanVal < GlobalPD.iPlrMin) {
            dChg = GlobalPD.iPlrMin - GlobalPD.iPlanVal;
        } else if (GlobalPD.iPlanVal > GlobalPD.iPlrMax) {
            dChg = GlobalPD.iPlanVal - GlobalPD.iPlrMax;
        } else {
            dChg = -abs(GlobalPD.iPlanVal - GlobalPD.iPlrVal);
        }
        if (dChg < 0) {
            dChg = -dChg;
            ids = idsValueDAwayIdealValueRace;
        } else {
            ids = idsValueDOutsideHabitableRangeRace;
        }
        CchGetString(ids, szWork);
        cch = _wsprintf(szOut, szWork, dChg);
        SelectObject(hdc, rghfontArial8[0]);
        WrapTextOut(hdc, &x, &y, szOut, cch, 4, xMax - 4, NULL, 0, fPrint);
    }
    if (x > 4) {
        y += dyArial8;
    }
    pt.x = xMax + 4;
    pt.y = y + 4;
    return pt;
}

POINT16 PtDisplayPlanetPopInfo(HDC hdc, int16_t fPrint) {
    PLANET  pl;
    char    szT[150];
    POINT16 pt;
    int16_t y;
    int16_t xMax;
    int16_t c;
    char   *psz;
    int32_t lMax;
    int16_t pctDesire;
    int16_t x;
    int32_t lPopChg;

    y = 4;
    xMax = 4;
    x = 4;
    FLookupPlanet(GlobalPD.idPlanet, &pl);
    SelectObject(hdc, rghfontArial8[0]);
    if (pl.iPlayer == idPlayer) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsPopulation3), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsIs), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        _wsprintf(szWork, PCTLD00, pl.rgwtMin[3]);
        if (pl.rgwtMin[3] == 0) {
            szWork[1] = 0;
        }
        DxStreamTextOut(hdc, &x, y, szWork, 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    } else if (pl.iPlayer != -1) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsEnemyPopulation), 0, fPrint);
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        if (pl.det >= detSome) {
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsApproximately), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            _wsprintf(szWork, "%d00", pl.uPopGuess * 4);
            if (pl.uPopGuess == 0) {
                szWork[1] = 0;
            }
            DxStreamTextOut(hdc, &x, y, szWork, 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
        } else {
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnknown), 0, fPrint);
        }
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    } else {
        SelectObject(hdc, rghfontArial8[1]);
        DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
        SelectObject(hdc, rghfontArial8[0]);
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUninhabited2), 0, fPrint);
    }
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    if (pl.det >= detSome) {
        lMax = CalcPlanetMaxPop(GlobalPD.idPlanet, idPlayer);
        pctDesire = PctPlanetDesirability(&pl, idPlayer);
        if (pctDesire < 0) {
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWillKillOffApproximately), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            c = _wsprintf(szWork, PCTDXPCTDPCTPCT, (int16_t)-pctDesire / 10, (int16_t)-pctDesire % 10);
            DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsOf), 0, fPrint);
            if (x > xMax) {
                xMax = x;
            }
            x = 4;
            y += dyArial8;
            if (pl.iPlayer == idPlayer) {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsEachTurn), 0, fPrint);
            } else {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsSettleEveryTurn), 0, fPrint);
            }
        } else if (pl.iPlayer == idPlayer && lMax > 0) {
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWillSupportPopulation), 0, fPrint);
            if (x > xMax) {
                xMax = x;
            }
            x = 4;
            y += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            c = _wsprintf(szWork, PCTLD00, lMax);
            DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonists3), 0, fPrint);
        } else if (lMax > 0) {
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsIfColonize), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWould), 0, fPrint);
            if (x > xMax) {
                xMax = x;
            }
            x = 4;
            y += dyArial8;
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsSupport), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            c = _wsprintf(szWork, PCTLD00, lMax);
            DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonists3), 0, fPrint);
        } else {
            y -= dyArial8;
        }
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y += dyArial8;
        if (pl.iPlayer == idPlayer && pctDesire >= 0 && pl.rgwtMin[3] < lMax) {
            c = CchGetString(idsPopulation, szT);
            WrapTextOut(hdc, &x, &y, szT, c, 4, xMax, NULL, 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetPlanetName(pl.id);
            WrapTextOut(hdc, &x, &y, psz, 0, 4, xMax, NULL, 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            lPopChg = ChgPopFromPlanet(&pl, 0);
            if (pctDesire == 0 || lPopChg <= 0) {
                c = CchGetString(idsWillGrowYear, szT);
            } else {
                psz = PszGetCompressedString(idsWillGrowLd00Ld00Year);
                c = _wsprintf(szT, psz, lPopChg, pl.rgwtMin[3] + lPopChg);
            }
            WrapTextOut(hdc, &x, &y, szT, c, 4, xMax, NULL, 0, fPrint);
            x = 4;
            y += dyArial8;
        } else if (pl.iPlayer != idPlayer && pl.iPlayer != -1) {
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetPlanetName(pl.id);
            WrapTextOut(hdc, &x, &y, psz, 0, 4, xMax, NULL, 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            if (pl.uDefGuess == 0) {
                c = CchGetString(idsAppearsHavePlanetaryDefenses, szWork);
            } else {
                psz = PszGetCompressedString(idsHasPlanetaryDefensesApproximatelyDCoverage);
                c = _wsprintf(szT, psz, pl.uDefGuess * 6 + 3);
                psz = szT;
            }
            WrapTextOut(hdc, &x, &y, psz, c, 4, xMax, NULL, 0, fPrint);
            x = 4;
            y += dyArial8;
        }
    }
    pt.x = xMax + 4;
    pt.y = y + 4;
    return pt;
}

POINT16 PtDisplayZipOrdInfo(HDC hdc, int16_t xCtr, int16_t fPrint) {
    POINT16 pt;
    int16_t y;
    int16_t xMax;
    char   *psz;
    int16_t x;

    y = 4;
    xMax = 4;
    x = 4;
    SelectObject(hdc, rghfontArial8[1]);
    if (fPrint != 0) {
        psz = PszGetCompressedString(idsZipord);
        CtrTextOut(hdc, xCtr, y, psz, strlen(psz));
    }
    y += dyArial8;
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsZipordProvidesAbilityQuicklySetFleets), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsTransportOrdersOne3CommonSetsSelect), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsZipordClickDiamondRightMouse), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsButtonClickOrderChoice), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8 * 2;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsQuikload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsLoadMineralsAvailable), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += (dyArial8 >> 1) + dyArial8;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsQuikdrop), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnloadEverythingFleetCarrying), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += (dyArial8 >> 1) + dyArial8;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWaitload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWaitFullLoadMinerals), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += (dyArial8 >> 1) + dyArial8;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsClear2), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsRemoveTransportOrders), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y += dyArial8;
    pt.x = xMax + 4;
    pt.y = y + 4;
    return pt;
}

POINT16 PtDisplayFactoryMineInfo(HDC hdc, int16_t dx, int16_t fPrint) {
    char    *pszTypes;
    char     szT[40];
    POINT16  pt;
    StringId ids;
    char    *pszType;
    int16_t  y;
    int16_t  xMax;
    int16_t  i;
    int16_t  c;
    char    *psz;
    int16_t  cnt;
    int16_t  x;

    ids = idsHave;
    dx -= 8;
    xMax = 4;
    x = 4;
    y = 2;
    if (GlobalPD.grbit != 0) {
        pszType = "Factory";
        pszTypes = "Factories";
    } else {
        pszType = "Mine";
        pszTypes = "Mines";
    }
    SelectObject(hdc, rghfontArial8[1]);
    if (fPrint != 0) {
        CchGetString(idsSInfo, szT);
        c = _wsprintf(szWork, szT, pszType);
        CtrTextOut(hdc, dx >> 1, y, szWork, c);
    }
    y += dyArial8 + 4;
    if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
        SelectObject(hdc, rghfontArial8[0]);
        psz = PszGetCompressedString(GlobalPD.grbit == 0 ? idsRaceIncapableBuildingMinesHoweverColonistsHave : idsRaceIncapableBuildingFactories);
        WrapTextOut(hdc, &x, &y, psz, 0, 4, dx, &xMax, 0, fPrint);
    } else {
        for (i = 0; i <= 8; i++) {
            SelectObject(hdc, rghfontArial8[1]);
            if ((uint16_t)i <= 8) {
                switch (i) {
                case 0:
                case 2:
                case 4:
                case 6:
                case 8:
                    SelectObject(hdc, rghfontArial8[0]);
                    psz = PszGetCompressedString(ids);
                    ids++;
                    break;
                case 1:
                    cnt = GlobalPD.cCur;
                    goto SetQuan;
                case 3:
                    psz = PszGetPlanetName(GlobalPD.idPlan);
                    break;
                case 5:
                    cnt = GlobalPD.cMax;
                    goto SetQuan;
                case 7:
                    _wsprintf(szWork, PCTD, GlobalPD.cOperate);
                    psz = szWork;
                }
                goto L_3319;
            SetQuan:
                _wsprintf(szWork, "%d %s", cnt, cnt == 1 ? pszType : pszTypes);
                psz = szWork;
            }
        L_3319:
            WrapTextOut(hdc, &x, &y, psz, 0, 4, dx, &xMax, 0, fPrint);
        }
    }
    pt.x = xMax + 4;
    pt.y = y + dyArial8 + 2;
    return pt;
}

POINT16 PtDisplayResourceInfo(HDC hdc, int16_t dx, int16_t fPrint) {
    int16_t  iMax;
    POINT16  pt;
    StringId ids;
    int16_t  y;
    int16_t  xMax;
    int16_t  i;
    char    *psz;
    int16_t  cnt;
    int16_t  x;

    iMax = (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh ? 1 : 0) + 8;
    ids = idsGenerates;
    dx -= 8;
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[1]);
    if (fPrint != 0) {
        psz = PszGetCompressedString(idsResourceInfo);
        CtrTextOut(hdc, dx >> 1, y, psz, strlen(psz));
    }
    y += dyArial8 + 4;
    for (i = 0; i <= iMax; i++) {
        SelectObject(hdc, rghfontArial8[1]);
        if ((uint16_t)i <= 9) {
            switch (i) {
            case 6:
                if (GlobalPD.iPlanVal == GlobalPD.iPlanetVar)
                    break;
            case 1:
            case 3:
            case 5:
            case 8:
            case 9:
                SelectObject(hdc, rghfontArial8[0]);
                psz = PszGetCompressedString(ids);
                ids++;
                goto L_34fb;
            case 2:
                cnt = GlobalPD.iPlanetVar;
                goto SetQuan;
            case 0:
                psz = PszGetPlanetName(GlobalPD.idPlanet);
                goto L_34fb;
            case 4:
                cnt = GlobalPD.iPlanetVar - GlobalPD.iPlanVal;
                if (cnt > 0)
                    goto SetQuan;
                psz = PszGetCompressedString(idsNone2);
                goto L_34fb;
            case 7:
                cnt = GlobalPD.iPlanVal;
                goto SetQuan;
            }
            break;
        SetQuan:
            _wsprintf(szWork, PCTD, cnt);
            psz = szWork;
        }
    L_34fb:
        WrapTextOut(hdc, &x, &y, psz, 0, 4, dx, &xMax, 0, fPrint);
    }
    pt.x = xMax + 4;
    pt.y = y + dyArial8 + 2;
    return pt;
}

POINT16 PtDisplayString(HDC hdc, int16_t dx, int16_t fPrint) {
    POINT16 pt;
    int16_t y;
    int16_t xMax;
    int16_t x;

    dx -= 8;
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &x, &y, GlobalPD.psz, 0, 4, dx, &xMax, 0, fPrint);
    pt.x = xMax + 4;
    pt.y = y + dyArial8 + 2;
    return pt;
}
