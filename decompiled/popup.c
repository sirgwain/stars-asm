#include "common.h"

uint16_t mpimdgrbitBU[8] = {8, 8, 16, 32, 128, 64, 8, 8};

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
        if (hwndPopup == 0x0) {
            return 0;
        }
        DestroyWindow(hwndPopup);
        hwndPopup = 0x0;
        GlobalPD.grPopup = 0x0;
        ReleaseCapture();
        if (gd.fTutorial == 0x0) {
            return 0;
        }
        tutor.fProgress = 0x1;
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
    uint16_t imd;

    if (GlobalPD.grbit != 0x0 && GlobalPD.grbit != 0xff) {
        imd = LphuldefFromId(rglpshdef[GlobalPD.lpfl->iPlayer][ishdef].hul.ihuldef)->imdCategory;
        return mpimdgrbitBU[imd] & GlobalPD.grbit;
    }
    return 1;
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
    StringId t_merge_03dc_0001;

    crBack = SetBkColor(hdc, 0xffffff);
    crFore = SetTextColor(hdc, 0x0);
    bkMode = SetBkMode(hdc, OPAQUE);
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    GetClientRect(hwnd, &rc);
    if (GlobalPD.grPopup - 1 <= 0xd) {
        switch (GlobalPD.grPopup) {
        case 1:
            CtrTextOut(hdc, rc.right >> 0x1, 4, rgszMinerals[GlobalPD.rgi[0]], 0);
            SelectObject(hdc, rghfontArial8[0]);
            psz = PszGetCompressedString(idsMineralConcentration);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 4;
            RightTextOut(hdc, dx, dyArial8 + 4, PszGetCompressedString(idsSurface), 0, 0);
            if (GlobalPD.rgi[2] < 0) {
                strcpy(szWork, PszGetCompressedString(idsUnknown2));
                c = strlen(szWork);
            } else {
                c = _wsprintf(szWork, "%ldkT", GlobalPD.rgi[2]);
            }
            TextOut(hdc, dx, dyArial8 + 4, szWork, c);
            RightTextOut(hdc, dx, dyArial8 * 2 + 4, PszGetCompressedString(idsMineralConcentration), 0, 0);
            if (GlobalPD.rgi[3] <= 0) {
                c = _wsprintf(szWork, PszGetCompressedString(idsUnknown2));
            } else {
                c = _wsprintf(szWork, PCTLD, GlobalPD.rgi[3]);
                if (GlobalPD.rgi[1] != 0) {
                    t_merge_03dc_0001 = GlobalPD.rgi[3] < 30 ? idsN30 : idsHw;
                    c = c + _wsprintf(&szWork[c], PszGetCompressedString(t_merge_03dc_0001));
                }
            }
            TextOut(hdc, dx, dyArial8 * 2 + 4, szWork, c);
            if (GlobalPD.rgi[4] < 0)
                break;
            RightTextOut(hdc, dx, 3 * dyArial8 + 4, PszGetCompressedString(idsMiningRate), 0, 0);
            CchGetString(idsLdktYr, szT);
            c = _wsprintf(szWork, szT, GlobalPD.rgi[4]);
            TextOut(hdc, dx, 3 * dyArial8 + 4, szWork, c);
            break;
        case 2:
            CtrTextOut(hdc, rc.right >> 0x1, 4, PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, 0x0), 0);
            c = _wsprintf(szWork, PszGetCompressedString(idsPlayerD), GlobalPD.iPlayer + 1);
            CtrTextOut(hdc, rc.right >> 0x1, dyArial8 + 4, szWork, c);
            break;
        case 3:
            if (rc.bottom - rc.top >= dyArial8 * 2) {
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
                    if (GlobalPD.lpfl->rgcsh[i] > 0 && (GlobalPD.grbit == 0x0 || FIsPopupHullType(i) != 0)) {
                        if (GlobalPD.fRedDamage != 0) {
                            if (GlobalPD.lpfl->rgdv[i].dp == 0x0) {
                                SetTextColor(hdc, 0x0);
                            } else {
                                SetTextColor(hdc, 0xff);
                            }
                        }
                        DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
                        lpsz = szTB;
                        TextOut(hdc, 4, yCur, lpsz, fstrlen(lpsz));
                        c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
                        RightTextOut(hdc, rc.right - 4 - GlobalPD.dxDamage, yCur, szWork, c, 0);
                        if (GlobalPD.fRedDamage != 0 && GlobalPD.lpfl->rgdv[i].dp != 0x0) {
                            csh = GlobalPD.lpfl->rgcsh[i];
                            csh = LOWORD((int32_t)((int32_t)((uint32_t)(GlobalPD.lpfl->rgdv[i].dp & 0x7f) * (int32_t)csh) / 0x64));
                            if (csh <= 0) {
                                csh = 1;
                            }
                            dpT = (uint32_t)(GlobalPD.lpfl->rgdv[i].dp >> 0x7 & 0x1ff) / 0x5;
                            if (dpT == 0) {
                                dpT = 1;
                            }
                            c = _wsprintf(szWork, "%d@%d%%", csh, dpT);
                            RightTextOut(hdc, rc.right - 4, yCur, szWork, c, 0);
                        }
                        yCur = yCur + dyArial8;
                    }
                }
                if (GlobalPD.fRedDamage == 0)
                    break;
                SetTextColor(hdc, 0x0);
                break;
            }
            c = CchGetString(idsNone2, szWork);
            TextOut(hdc, 4, 4, szWork, c);
            SelectObject(hdc, rghfontArial8[0]);
            break;
        case 4:
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
        case 5:
            PtDisplayPlanetStateInfo(hdc, 1);
            break;
        case 6:
            PtDisplayZipOrdInfo(hdc, rc.right >> 0x1, 1);
            break;
        case 7:
            PtDisplayPlanetPopInfo(hdc, 1);
            break;
        case 8:
            PtDisplayFactoryMineInfo(hdc, rc.right, 1);
            break;
        case 12:
            PtDisplayResourceInfo(hdc, rc.right, 1);
            break;
        case 9:
            DisplayComponentInfo(hdc, rc.right, rc.bottom, &GlobalPD.part);
            break;
        case 10:
            PtDisplayString(hdc, rc.right, 1);
            break;
        case 11:
        case 14:
            fStarbaseMode = GlobalPD.lpshdef->hul.ihuldef < ihuldefOrbitalFort ? 0 : 1;
            DrawSlotDlg(hwnd, hdc, &rc, -1);
            rc.top = dyArial8 + 306;
            rc.left = rc.left + 6;
            SelectObject(hdc, rghfontArial8[1]);
            SetBkMode(hdc, TRANSPARENT);
            fstrcpy(szWork, GlobalPD.lpshdef->hul.szClass);
            CtrTextOut(hdc, ((rc.right - 76) >> 0x1) + 0x4c, 6, szWork, 0);
            DrawBuildSelHull(hwnd, hdc, -1, &rc);
        case 13:
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
    POINT   t_pt_0c9b_1;
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
    t_pt_0c9b_1 = PointFrom16(pt);
    ClientToScreen(hwnd, &t_pt_0c9b_1);
    pt = PointTo16(t_pt_0c9b_1);
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    if (GlobalPD.grPopup - 1 <= 0xd) {
        switch (GlobalPD.grPopup) {
        case 1:
            psz = PszGetCompressedString(idsMineralConcentration0000000kt);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dy = 3 * dyArial8 + 8;
            if (GlobalPD.rgi[4] < 0)
                break;
            dy = dy + dyArial8;
            break;
        case 2:
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszPlayerName(GlobalPD.iPlayer, 1, 1, 1, 0, 0x0);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dx2 = LOWORD(GetTextExtent(hdc, "Player #16", 10)) + 8;
            if (dx2 > dx) {
                dx = dx2;
            }
            dy = dyArial8 * 2 + 8;
            break;
        case 3:
            dxR = 0;
            dxDamage = 0;
            dy = dyArial8 + 8;
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetCompressedString(idsShipName);
            dxL = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            SelectObject(hdc, rghfontArial8[0]);
            for (i = 0; i < 16; i++) {
                if (GlobalPD.lpfl->rgcsh[i] > 0 && (GlobalPD.grbit == 0x0 || FIsPopupHullType(i) != 0)) {
                    dy = dy + dyArial8;
                    DecorateHullName(GlobalPD.lpfl->iplr, i, szTB);
                    lpsz = szTB;
                    dx = LOWORD(GetTextExtent(hdc, lpsz, fstrlen(lpsz)));
                    dxL = dxL <= dx ? dx : dxL;
                    c = _wsprintf(szWork, PCTD, GlobalPD.lpfl->rgcsh[i]);
                    dx = LOWORD(GetTextExtent(hdc, szWork, c));
                    dxR = dxR <= dx ? dx : dxR;
                    if (GlobalPD.fRedDamage != 0 && (GlobalPD.lpfl->rgdv[i].dp >> 0x7 & 0x1ff) != 0x0 && dxDamage == 0) {
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
        case 4:
            SelectObject(hdc, rghfontArial8[1]);
            dy = dyArial8 * 4 + 8;
            psz = PszGetCompressedString(idsPlanet);
            dx = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            psz = PszGetPlanetName(sel.scan.idpl);
            SelectObject(hdc, rghfontArial8[0]);
            dxName = LOWORD(GetTextExtent(hdc, psz, strlen(psz)));
            dxCoord = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999), 4));
            dx = dx + (dxName <= dxCoord ? dxCoord : dxName);
            break;
        case 5:
            ptT = PtDisplayPlanetStateInfo(hdc, 0);
            goto SetDxDy;
        case 6:
            ptT = PtDisplayZipOrdInfo(hdc, 0, 0);
            goto SetDxDy;
        case 7:
            ptT = PtDisplayPlanetPopInfo(hdc, 0);
            goto SetDxDy;
        case 12:
            ptT = PtDisplayResourceInfo(hdc, 200, 0);
            goto SetDxDy;
        case 8:
            ptT = PtDisplayFactoryMineInfo(hdc, 200, 0);
            goto SetDxDy;
        case 9:
            dx = (dyArial8 <= 14 ? 0 : 40) + 344;
            dy = dyArial10 + 72 + 12 * dyArial8 + 6;
            break;
        case 10:
            ptT = PtDisplayString(hdc, GlobalPD.dxOut, 0);
            goto SetDxDy;
        case 11:
        case 14:
            mdBuild = GlobalPD.grPopup == grPopupShdef ? mdBuildShdef : mdBuildHuldef;
            lpshdefBuild = GlobalPD.lpshdef;
            UpdateSlotGlobals();
            dx = 340;
            dy = dyArial8 + 306 + 6 * dyArial8 + 8;
            if (gd.mdScreenSize <= 0x0 || GlobalPD.grPopup != grPopupShdef)
                break;
            dy = dy + 3 * dyArial8;
            break;
        case 13:
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
    pt.x = pt.x - dx;
    pt.y = pt.y - dy;
    if (pt.x >= GetSystemMetrics(SM_CXSCREEN) - dx) {
        t_call_1265 = GetSystemMetrics(SM_CXSCREEN);
        t_merge_126d_0001 = t_call_1265 - dx;
    } else {
        t_merge_126d_0001 = pt.x;
    }
    if (0 <= t_merge_126d_0001) {
        if (pt.x >= GetSystemMetrics(SM_CXSCREEN) - dx) {
            t_call_129b = GetSystemMetrics(SM_CXSCREEN);
            t_merge_12a3_0001 = t_call_129b - dx;
        } else {
            t_merge_12a3_0001 = pt.x;
        }
    } else {
        t_merge_12a3_0001 = 0;
    }
    pt.x = t_merge_12a3_0001;
    if (pt.y >= GetSystemMetrics(SM_CYSCREEN) - dy) {
        t_call_12c4 = GetSystemMetrics(SM_CYSCREEN);
        t_merge_12cc_0001 = t_call_12c4 - dy;
    } else {
        t_merge_12cc_0001 = pt.y;
    }
    if (0 <= t_merge_12cc_0001) {
        if (pt.y >= GetSystemMetrics(SM_CYSCREEN) - dy) {
            t_call_12fa = GetSystemMetrics(SM_CYSCREEN);
            t_merge_1302_0001 = t_call_12fa - dy;
        } else {
            t_merge_1302_0001 = pt.y;
        }
    } else {
        t_merge_1302_0001 = 0;
    }
    pt.y = t_merge_1302_0001;
    hwndPopup = CreateWindow(szPopup, 0x0, WS_POPUP | WS_VISIBLE | WS_BORDER, pt.x, pt.y, dx, dy, hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndPopup, WM_SETFONT, (WPARAM)rghfontArial8[0], 0);
    SetCapture(hwndPopup);
    return;
}

int16_t PopupMenu(HWND hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char **rgsz, int16_t iChecked, int16_t fRightBtn) {
    char    *pszTitle;
    int16_t  tpm;
    POINT16  pt;
    int16_t  i;
    char     szTemp[128];
    HMENU    hmenuSub;
    HMENU    hmenuPopup;
    char    *pszT;
    char    *psz;
    MSG      msg;
    int16_t  fChecked;
    int16_t  fCheckedCur;
    POINT    t_pt_1391_1;
    char    *t_1545;
    char    *t_1550;
    char    *t_1564;
    char    *t_16d0;
    char    *t_16db;
    char    *t_16ef;
    char    *t_17e3;
    char    *t_17ee;
    char    *t_1802;
    int32_t  t_merge_184f_0001;
    uint16_t t_merge_186e_0001;

    hmenuSub = 0x0;
    pt.x = x;
    pt.y = y;
    t_pt_1391_1 = PointFrom16(pt);
    ClientToScreen(hwnd, &t_pt_1391_1);
    pt = PointTo16(t_pt_1391_1);
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = -1;
    for (i = 0; i < cString; i++) {
        if (rgids == 0x0 || (iChecked == -2 && rgsz != 0x0)) {
            if (rgsz[i] != 0x0) {
                if ((int16_t)*rgsz[i] != -1 || (int16_t)rgsz[i][1] != 0) {
                    pszT = szTemp;
                    psz = rgsz[i];
                    while ((int16_t)*psz != 0) {
                        t_17e3 = psz;
                        psz = psz + 1;
                        t_17ee = pszT;
                        pszT = pszT + 1;
                        *t_17ee = *t_17e3;
                        if ((int16_t)*t_17e3 == '&') {
                            t_1802 = pszT;
                            pszT = pszT + 1;
                            *t_1802 = '&';
                        }
                    }
                    *pszT = 0;
                    if (iChecked != -2) {
                        if (i != iChecked) {
                            t_merge_184f_0001 = 0;
                        } else {
                            t_merge_184f_0001 = 1;
                        }
                    } else {
                        t_merge_184f_0001 = rgids[i];
                    }
                    t_merge_186e_0001 = t_merge_184f_0001 == 0 ? 0x0 : 0x8;
                    AppendMenu(hmenuPopup, t_merge_186e_0001, i + 15000, szTemp);
                } else {
                    AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
                }
            } else {
                pszTitle = rgsz[i + 1];
                fChecked = rgids == 0x0 ? 0 : LOWORD(rgids[i + 1]);
                hmenuSub = CreatePopupMenu();
                for (i = i + 2; i < cString && rgsz[i] != 0x0; i++) {
                    if (rgids != 0x0) {
                        fCheckedCur = LOWORD(rgids[i]);
                    } else {
                        fCheckedCur = i == iChecked ? 1 : 0;
                        fChecked = fChecked | fCheckedCur;
                    }
                    if ((int16_t)*rgsz[i] != -1 || (int16_t)rgsz[i][1] != 0) {
                        pszT = szTemp;
                        psz = rgsz[i];
                        while ((int16_t)*psz != 0) {
                            t_16d0 = psz;
                            psz = psz + 1;
                            t_16db = pszT;
                            pszT = pszT + 1;
                            *t_16db = *t_16d0;
                            if ((int16_t)*t_16d0 == '&') {
                                t_16ef = pszT;
                                pszT = pszT + 1;
                                *t_16ef = '&';
                            }
                        }
                        *pszT = 0;
                        AppendMenu(hmenuSub, fCheckedCur == 0 ? 0x0 : 0x8, i + 15000, szTemp);
                    } else {
                        AppendMenu(hmenuSub, 0x800, 0x0, 0x0);
                    }
                }
                AppendMenu(hmenuPopup, (fChecked == 0 ? 0x0 : 0x8) | 0x10, (UINT_PTR)hmenuSub, pszTitle);
            }
        } else if (rgids[i] != -1) {
            if ((rgids[i] & 0x10000000) != 0x0) {
                psz = "Deep Space";
            } else if ((rgids[i] & 0x40000000) != 0x0) {
                psz = PszGetCompressedString(LOWORD(rgids[i]));
            } else if ((rgids[i] & 0x20000000) != 0x0) {
                psz = PszGetThingName(LOWORD(rgids[i]));
            } else if ((rgids[i] & 0x80000000) != 0x0) {
                psz = PszGetFleetName(LOWORD(rgids[i]) | 0x8000);
            } else {
                psz = PszGetPlanetName(LOWORD(rgids[i]));
            }
            pszT = szTemp;
            while ((int16_t)*psz != 0) {
                t_1545 = psz;
                psz = psz + 1;
                t_1550 = pszT;
                pszT = pszT + 1;
                *t_1550 = *t_1545;
                if ((int16_t)*t_1545 == '&') {
                    t_1564 = pszT;
                    pszT = pszT + 1;
                    *t_1564 = '&';
                }
            }
            *pszT = 0;
            AppendMenu(hmenuPopup, i == iChecked ? 0x8 : 0x0, i + 15000, szTemp);
        } else {
            AppendMenu(hmenuPopup, 0x800, 0x0, 0x0);
        }
    }
    if (fRightBtn == 0) {
        tpm = 0;
    } else {
        tpm = 2;
    }
    TrackPopupMenu(hmenuPopup, tpm, pt.x, pt.y, 0, hwndFrame, 0x0);
    DestroyMenu(hmenuPopup);
    if (hmenuSub != 0x0) {
        DestroyMenu(hmenuSub);
    }
    if (PeekMessage(&msg, hwndFrame, 0x111, 0x111, 0x2) != 0 && msg.wParam >= 0x3a98 && msg.wParam < 0x3afc) {
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
    if (GlobalPD.iPlanVal < 0) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnknown2), 0, fPrint);
    } else {
        DxStreamTextOut(hdc, &x, y, PszCalcEnvVar(GlobalPD.iPlanetVar, GlobalPD.iPlanVal), 0, fPrint);
    }
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    if (GlobalPD.iPlrMin != -1) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsPreferPlanetsWhere), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y = y + dyArial8;
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
    } else {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsImmune), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y = y + dyArial8;
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsEffects), 0, fPrint);
        DxStreamTextOut(hdc, &x, y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, fPrint);
        DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
    }
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    if (GlobalPD.iPlanMin > -1) {
        DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsCurrentlyPossessTechnology), 0, fPrint);
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y = y + dyArial8;
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
        y = y + dyArial8;
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
        y = y + dyArial8;
    }
    x = 4;
    if (GlobalPD.iPlanMin > -1 && GlobalPD.iPlrMin != -1 && GlobalPD.iPlanVal != GlobalPD.iPlrVal) {
        iNewVal = -1;
        if (GlobalPD.iPlanVal >= GlobalPD.iPlrVal) {
            if (GlobalPD.iPlanMin < GlobalPD.iPlanVal) {
                iNewVal = GlobalPD.iPlrVal <= GlobalPD.iPlanMin ? GlobalPD.iPlanMin : GlobalPD.iPlrVal;
            }
        } else if (GlobalPD.iPlanMax > GlobalPD.iPlanVal) {
            iNewVal = GlobalPD.iPlrVal >= GlobalPD.iPlanMax ? GlobalPD.iPlanMax : GlobalPD.iPlrVal;
        }
        if (iNewVal != -1) {
            lppl = LpplFromId(GlobalPD.idPlanet);
            pctDesireOld = PctPlanetDesirability(lppl, idPlayer);
            iValSav = (int16_t)lppl->rgEnvVar[GlobalPD.iPlanetVar];
            lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iNewVal);
            pctDesire = PctPlanetDesirability(lppl, idPlayer);
            lppl->rgEnvVar[GlobalPD.iPlanetVar] = LOBYTE(iValSav);
            if (pctDesireOld < pctDesire) {
                cch = CchGetString(idsIfTerraform, szWork);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, 0x0, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                WrapTextOut(hdc, &x, &y, rgszPlanetAttr[GlobalPD.iPlanetVar], 0, 4, xMax - 4, 0x0, 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                WrapTextOut(hdc, &x, &y, PszGetCompressedString(idsTo), 0, 4, xMax - 4, 0x0, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                WrapTextOut(hdc, &x, &y, PszCalcEnvVar(GlobalPD.iPlanetVar, iNewVal), 0, 4, xMax - 4, 0x0, 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                cch = CchGetString(idsPlanetsValueWouldImprove, szWork);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, 0x0, 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                cch = _wsprintf(szWork, "%d%%.  ", pctDesire);
                WrapTextOut(hdc, &x, &y, szWork, cch, 4, xMax - 4, 0x0, 0, fPrint);
            }
        }
    }
    if (GlobalPD.iPlrMin != -1 && GlobalPD.iPlanVal != GlobalPD.iPlrVal && GlobalPD.iPlanVal != -1) {
        if (GlobalPD.iPlanVal >= GlobalPD.iPlrMin) {
            if (GlobalPD.iPlanVal <= GlobalPD.iPlrMax) {
                dChg = -abs(GlobalPD.iPlanVal - GlobalPD.iPlrVal);
            } else {
                dChg = GlobalPD.iPlanVal - GlobalPD.iPlrMax;
            }
        } else {
            dChg = GlobalPD.iPlrMin - GlobalPD.iPlanVal;
        }
        if (dChg >= 0) {
            ids = idsValueDOutsideHabitableRangeRace;
        } else {
            dChg = -dChg;
            ids = idsValueDAwayIdealValueRace;
        }
        CchGetString(ids, szWork);
        cch = _wsprintf(szOut, szWork, dChg);
        SelectObject(hdc, rghfontArial8[0]);
        WrapTextOut(hdc, &x, &y, szOut, cch, 4, xMax - 4, 0x0, 0, fPrint);
    }
    if (x > 4) {
        y = y + dyArial8;
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
    if (pl.iPlayer != idPlayer) {
        if (pl.iPlayer == -1) {
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUninhabited2), 0, fPrint);
        } else {
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsEnemyPopulation), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            if (pl.det < 0x3) {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnknown), 0, fPrint);
            } else {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsApproximately), 0, fPrint);
                SelectObject(hdc, rghfontArial8[1]);
                _wsprintf(szWork, "%d00", pl.uPopGuess * 0x4);
                if (pl.uPopGuess == 0x0) {
                    szWork[1] = 0;
                }
                DxStreamTextOut(hdc, &x, y, szWork, 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
            }
            DxStreamTextOut(hdc, &x, y, ".", 1, fPrint);
        }
    } else {
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
    }
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    if (pl.det >= 0x3) {
        lMax = CalcPlanetMaxPop(GlobalPD.idPlanet, idPlayer);
        pctDesire = PctPlanetDesirability(&pl, idPlayer);
        if (pctDesire >= 0) {
            if (pl.iPlayer != idPlayer || lMax <= 0) {
                if (lMax <= 0) {
                    y = y - dyArial8;
                } else {
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
                    y = y + dyArial8;
                    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsSupport), 0, fPrint);
                    SelectObject(hdc, rghfontArial8[1]);
                    c = _wsprintf(szWork, PCTLD00, lMax);
                    DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
                    SelectObject(hdc, rghfontArial8[0]);
                    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonists3), 0, fPrint);
                }
            } else {
                SelectObject(hdc, rghfontArial8[1]);
                DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWillSupportPopulation), 0, fPrint);
                if (x > xMax) {
                    xMax = x;
                }
                x = 4;
                y = y + dyArial8;
                SelectObject(hdc, rghfontArial8[1]);
                c = _wsprintf(szWork, PCTLD00, lMax);
                DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
                SelectObject(hdc, rghfontArial8[0]);
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonists3), 0, fPrint);
            }
        } else {
            SelectObject(hdc, rghfontArial8[1]);
            DxStreamTextOut(hdc, &x, y, PszGetPlanetName(GlobalPD.idPlanet), 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWillKillOffApproximately), 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            c = _wsprintf(szWork, PCTDXPCTDPCTPCT, (int32_t)-pctDesire / 0xa, (int32_t)-pctDesire % 0xa);
            DxStreamTextOut(hdc, &x, y, szWork, c, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsOf), 0, fPrint);
            if (x > xMax) {
                xMax = x;
            }
            x = 4;
            y = y + dyArial8;
            if (pl.iPlayer != idPlayer) {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsSettleEveryTurn), 0, fPrint);
            } else {
                DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsColonistsEachTurn), 0, fPrint);
            }
        }
        if (x > xMax) {
            xMax = x;
        }
        x = 4;
        y = y + dyArial8;
        if (pl.iPlayer == idPlayer && pctDesire >= 0 && pl.rgwtMin[3] < lMax) {
            c = CchGetString(idsPopulation, szT);
            WrapTextOut(hdc, &x, &y, szT, c, 4, xMax, 0x0, 0, fPrint);
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetPlanetName(pl.id);
            WrapTextOut(hdc, &x, &y, psz, 0, 4, xMax, 0x0, 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            lPopChg = ChgPopFromPlanet(&pl, 0);
            if (pctDesire == 0 || lPopChg <= 0) {
                c = CchGetString(idsWillGrowYear, szT);
            } else {
                psz = PszGetCompressedString(idsWillGrowLd00Ld00Year);
                c = _wsprintf(szT, psz, lPopChg, pl.rgwtMin[3] + lPopChg);
            }
            WrapTextOut(hdc, &x, &y, szT, c, 4, xMax, 0x0, 0, fPrint);
            x = 4;
            y = y + dyArial8;
        } else if (pl.iPlayer != idPlayer && pl.iPlayer != -1) {
            SelectObject(hdc, rghfontArial8[1]);
            psz = PszGetPlanetName(pl.id);
            WrapTextOut(hdc, &x, &y, psz, 0, 4, xMax, 0x0, 0, fPrint);
            SelectObject(hdc, rghfontArial8[0]);
            if (pl.uDefGuess != 0x0) {
                psz = PszGetCompressedString(idsHasPlanetaryDefensesApproximatelyDCoverage);
                c = _wsprintf(szT, psz, pl.uDefGuess * 0x6 + 0x3);
                psz = szT;
            } else {
                c = CchGetString(idsAppearsHavePlanetaryDefenses, szWork);
            }
            WrapTextOut(hdc, &x, &y, psz, c, 4, xMax, 0x0, 0, fPrint);
            x = 4;
            y = y + dyArial8;
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
    y = y + dyArial8;
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsZipordProvidesAbilityQuicklySetFleets), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsTransportOrdersOne3CommonSetsSelect), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsZipordClickDiamondRightMouse), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsButtonClickOrderChoice), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8 * 2;
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsQuikload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsLoadMineralsAvailable), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + ((dyArial8 >> 0x1) + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsQuikdrop), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsUnloadEverythingFleetCarrying), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + ((dyArial8 >> 0x1) + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWaitload), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsWaitFullLoadMinerals), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + ((dyArial8 >> 0x1) + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsClear2), 0, fPrint);
    SelectObject(hdc, rghfontArial8[0]);
    DxStreamTextOut(hdc, &x, y, PszGetCompressedString(idsRemoveTransportOrders), 0, fPrint);
    if (x > xMax) {
        xMax = x;
    }
    x = 4;
    y = y + dyArial8;
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
    dx = dx - 8;
    xMax = 4;
    x = 4;
    y = 2;
    if (GlobalPD.grbit == 0x0) {
        pszType = "Mine";
        pszTypes = "Mines";
    } else {
        pszType = "Factory";
        pszTypes = "Factories";
    }
    SelectObject(hdc, rghfontArial8[1]);
    if (fPrint != 0) {
        CchGetString(idsSInfo, szT);
        c = _wsprintf(szWork, szT, pszType);
        CtrTextOut(hdc, dx >> 0x1, y, szWork, c);
    }
    y = y + (dyArial8 + 4);
    if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
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
                    ids = ids + 1;
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
    } else {
        SelectObject(hdc, rghfontArial8[0]);
        psz = PszGetCompressedString(GlobalPD.grbit == 0x0 ? idsRaceIncapableBuildingMinesHoweverColonistsHave : idsRaceIncapableBuildingFactories);
        WrapTextOut(hdc, &x, &y, psz, 0, 4, dx, &xMax, 0, fPrint);
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
    dx = dx - 8;
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[1]);
    if (fPrint != 0) {
        psz = PszGetCompressedString(idsResourceInfo);
        CtrTextOut(hdc, dx >> 0x1, y, psz, strlen(psz));
    }
    y = y + (dyArial8 + 4);
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
                ids = ids + 1;
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

    dx = dx - 8;
    xMax = 4;
    x = 4;
    y = 2;
    SelectObject(hdc, rghfontArial8[0]);
    WrapTextOut(hdc, &x, &y, GlobalPD.psz, 0, 4, dx, &xMax, 0, fPrint);
    pt.x = xMax + 4;
    pt.y = y + dyArial8 + 2;
    return pt;
}
