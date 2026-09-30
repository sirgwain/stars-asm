#include "common.h"

uint16_t rggrbitBrParts[17] = {6655, 8, 16, 64, 2048, 1, 4096, 256, 128, 512, 32768, 2, 4, 16384, 1024, 8192, 32};
int32_t  rglTechCost[27] = {0,     50,    80,    130,   210,   340,   550,   890,   1440,  2330,  3770,  6100,  9870, 13850,
                            18040, 22440, 27050, 31870, 36900, 42140, 47590, 53250, 59120, 65200, 71490, 77990, 84700};

INT_PTR CALLBACK ResearchDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     y;
    int16_t     i;
    PAINTSTRUCT ps;
    int16_t     dx;
    RECT        rc;
    HWND        hwndRad;
    int16_t     dxCurrent;
    PLANET     *lppl;
    int16_t     c;
    PLANET     *lpplMac;
    HFONT       hfontSav;
    char       *psz;
    RECT        rcWindow;
    POINT16     pt;
    int16_t     iResTechNext;
    int16_t     fChg;
    HWND        t_scratch_m34;
    POINT       t_pt_05aa;
    POINT       t_pt_05b9_1;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawResearchDlg(hwnd, hdc, &rc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            pctResGlob = rgplr[idPlayer].pctResearch;
            iResTechNow = rgplr[idPlayer].iTechCur & 0xf;
            CheckRadioButton(hwnd, 1073, 1078, iResTechNow + 1073);
            hdc = GetDC(hwnd);
            hfontSav = SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsCurrent, szWork);
            dxCurrent = LOWORD(GetTextExtent(hdc, szWork, c));
            dxResRadio = 0;
            for (i = 1073; i <= 1078; i++) {
                c = CchGetString(i - 989, szWork);
                dx = LOWORD(GetTextExtent(hdc, szWork, c));
                if (dx > dxResRadio) {
                    dxResRadio = dx;
                }
                hwndRad = GetDlgItem(hwnd, i);
                SetWindowText(hwndRad, szWork);
                SendMessage(hwndRad, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
            }
            dxResRadio += 64;
            y = dyArial8 * 4 - (dyArial8 >> 1) + 1;
            for (i = 1073; i <= 1078; i++) {
                hwndRad = GetDlgItem(hwnd, i);
                SetWindowPos(hwndRad, NULL, 16, y, dxResRadio, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER | SWP_SHOWWINDOW);
                y += (int16_t)(3 * dyArial8) / 2;
            }
            hwndRad = GetDlgItem(hwnd, 0x43b);
            for (i = 0; i <= 7; i++) {
                psz = PszGetCompressedString(i + 83);
                SendMessage(hwndRad, CB_ADDSTRING, 0, (LPARAM)psz);
            }
            i = rgplr[idPlayer].iTechCur >> 4;
            if (i == 6) {
                i = 0;
            } else if (i < 6) {
                i++;
            }
            SendMessage(hwndRad, CB_SETCURSEL, i, 0);
            dxResLeft = dxResRadio + dxCurrent + 40;
            dxResRight = 0;
            for (i = 76; i <= 81; i++) {
                c = CchGetString(i, szWork);
                dx = LOWORD(GetTextExtent(hdc, szWork, c));
                if (dx > dxResRight) {
                    dxResRight = dx;
                }
            }
            dxResStrRight = dxResRight;
            dx = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999992), 7));
            dxResRight += dx + 48;
            y += 10 * dyArial8;
            SetWindowPos(GetDlgItem(hwnd, IDCANCEL), NULL, dxResLeft + dxResRight - 152, y - (int16_t)(3 * dyArial8) / 2 - 8, 70, (int16_t)(3 * dyArial8) / 2,
                         SWP_NOZORDER | SWP_SHOWWINDOW);
            SetWindowPos(GetDlgItem(hwnd, IDC_HELP), NULL, dxResLeft + dxResRight - 76, y - (int16_t)(3 * dyArial8) / 2 - 8, 70, (int16_t)(3 * dyArial8) / 2,
                         SWP_NOZORDER | SWP_SHOWWINDOW);
            GetWindowRect(hwnd, &rcWindow);
            GetClientRect(hwnd, &rc);
            SetWindowPos(hwnd, NULL, 0, 0, dxResLeft + dxResRight + rcWindow.right - rcWindow.left - rc.right, y + rcWindow.bottom - rcWindow.top - rc.bottom,
                         SWP_NOMOVE | SWP_NOZORDER);
            StickyDlgPos(hwnd, &ptStickyResDlg, 1);
            SelectObject(hdc, hfontSav);
            ReleaseDC(hwnd, hdc);
            lResTotal = 0;
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                if (lppl->iPlayer == idPlayer) {
                    lResTotal += CResourcesAtPlanet(lppl, idPlayer);
                }
            }
            lResBudget = ProjectedResearchSpending(pctResGlob);
            if (gd.fTutorial != 0) {
                AdvanceTutor();
            }
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            return FTrackResearchDlg(hwnd, LOWORD(lParam), HIWORD(lParam), wParam);
        case WM_SETCURSOR:
            GetCursorPos(&t_pt_05aa);
            pt = PointTo16(t_pt_05aa);
            t_pt_05b9_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_05b9_1);
            pt = PointTo16(t_pt_05b9_1);
            if (pt.x > 12 && pt.x < dxResLeft - 12 && pt.y >= yTopFutureTech && pt.y < cFutureTech * dyArial8 + yTopFutureTech) {
                SetCursor(hcurArrowHelp);
                return 1;
            }
            if (yTopTechNote != -1 && pt.y >= yTopTechNote && pt.y < dyArial8 * 2 + yTopTechNote && pt.x > dxResLeft) {
                SetCursor(hcurArrowHelp);
                return 1;
            }
        case WM_COMMAND:
            if (HIWORD(lParam) == 0 && wParam >= 1073 && wParam <= 1078) {
                if (IsDlgButtonChecked(hwnd, wParam) != 0) {
                    iResTechNow = wParam - 1073;
                    GetClientRect(hwnd, &rc);
                    DrawResearchDlg(hwnd, NULL, &rc, 4);
                }
            } else {
                if (wParam == 2) {
                    fChg = 0;
                    iResTechNext = LOWORD(SendMessage(GetDlgItem(hwnd, 0x43b), CB_GETCURSEL, 0, 0));
                    if (iResTechNext == 0) {
                        iResTechNext = 6;
                    } else if (iResTechNext <= 6) {
                        iResTechNext--;
                    }
                    if (iResTechNow != (rgplr[idPlayer].iTechCur & 0xf) || iResTechNext != rgplr[idPlayer].iTechCur >> 4 ||
                        pctResGlob != rgplr[idPlayer].pctResearch) {
                        rgplr[idPlayer].pctResearch = LOBYTE(pctResGlob);
                        rgplr[idPlayer].iTechCur = LOBYTE((rgplr[idPlayer].iTechCur & 0xfff0) | iResTechNow);
                        rgplr[idPlayer].iTechCur = LOBYTE((rgplr[idPlayer].iTechCur & 0xff0f) | iResTechNext * 0x10);
                        i = rgplr[idPlayer].iTechCur * 0 + pctResGlob;
                        WriteMemRt(34, 2, &i);
                        fChg = 1;
                        if (gd.fTutorial != 0 && idPlayer == 0) {
                            tutor.fChange = 1;
                            AdvanceTutor();
                        }
                    }
                    StickyDlgPos(hwnd, &ptStickyResDlg, 0);
                    EndDialog(hwnd, fChg);
                    pctResGlob = -1;
                    if (gd.fTutorial != 0) {
                        AdvanceTutor();
                    }
                    return 1;
                }
                if (wParam == 118) {
                    WinHelp(hwnd, szHelpFile, 1, 1070);
                    return 1;
                }
            }
        }
    } else {
        for (i = 1073; i <= 1078; i++) {
            t_scratch_m34 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m34 == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 1078 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
}

void DrawResearchDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t grbitDraw) {
    int16_t  dxCurrent;
    char     szTemp[60];
    RECT     rcT;
    int16_t  iMax;
    int16_t  iTechSav;
    int16_t  iter;
    int16_t  fCreatedDC;
    int16_t  mdAvail;
    int16_t  i;
    int16_t  c;
    int16_t  grbitCur;
    COLORREF crBackSav;
    COLORREF crForeSav;
    HFONT    hfontSav;
    int16_t  xNum;
    int16_t  xCtr;
    int16_t  dx;
    char     szTemp2[60];
    PART     part;
    int32_t  l;
    int16_t  iMin;
    RECT     rc;
    int32_t  lSpent;
    HBRUSH   hbrSav;
    int32_t  lRBEffective;
    int16_t  cch;
    int16_t  t_0e66;
    uint16_t t_merge_1406_0001;

    fCreatedDC = 0;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    crForeSav = SetTextColor(hdc, 0);
    crBackSav = SetBkColor(hdc, crButtonFace);
    if ((grbitDraw & 0xff) != 0) {
        c = CchGetString(idsCurrent, szWork);
        dxCurrent = LOWORD(GetTextExtent(hdc, szWork, c));
        SetRect(&rc, 8, dyArial8, dxResLeft - 8, 12 * dyArial8 + dyArial8);
        if ((grbitDraw & 3) != 0) {
            _Draw3dFrame(hdc, &rc, -1);
            c = CchGetString(idsTechnologyStatus, szWork);
            TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, c);
            rc.top += dyArial8 >> 1;
            c = CchGetString(idsCurrent, szWork);
            TextOut(hdc, rc.right - 8 - dxCurrent, rc.top, szWork, c);
            rc.top += dyArial8;
            i = CchGetString(idsFieldStudy, szWork);
            TextOut(hdc, rc.left + 26, rc.top, szWork, i);
            xCtr = rc.right - 8 - (dxCurrent >> 1);
            i = CchGetString(idsLevel, szWork);
            CtrTextOut(hdc, xCtr, rc.top, szWork, i);
            PatBlt(hdc, rc.left + 8, rc.top + dyArial8, rc.right - rc.left - 16, 1, BLACKNESS);
            rc.top += (dyArial8 >> 2) + dyArial8 + 2;
            for (i = 0; i < 6; i++) {
                c = _wsprintf(szWork, PCTD, rgplr[idPlayer].rgTech[i]);
                CtrTextOut(hdc, xCtr, rc.top, szWork, c);
                rc.top += (int16_t)(3 * dyArial8) / 2;
            }
        }
        SetRect(&rc, 8, (int16_t)(3 * dyArial8) / 2 + rc.bottom, dxResLeft - 8, prc->bottom - 8);
        if ((grbitDraw & 4) != 0 && (grbitDraw & 3) == 0) {
            ExpandRc(&rc, -6, -(dyArial8 >> 1));
            rc.top += dyArial8 >> 1;
            InvalidateRect(hwnd, &rc, 1);
        } else {
            _Draw3dFrame(hdc, &rc, -1);
            c = CchGetString(idsExpectedResearchBenefits, szWork);
            TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, c);
            iTechSav = rgplr[idPlayer].iTechCur & 0xf;
            cFutureTech = 0;
            yTopFutureTech = rc.top + dyArial8;
            for (iter = 1; iter < 10; iter++) {
                iMax = iter;
                iMin = iter;
                if (iter == 1) {
                    SetTextColor(hdc, 32512);
                } else if (iter >= 2 && iter <= 4) {
                    SetTextColor(hdc, 8323072);
                } else {
                    SetTextColor(hdc, 0);
                    if (iter == 9) {
                        iMax = 26;
                    }
                }
                grbitCur = 1;
                rgplr[idPlayer].iTechCur = LOBYTE((rgplr[idPlayer].iTechCur & 0xfff0) | iResTechNow);
                for (; grbitCur != 0; grbitCur *= 2) {
                    if ((grbitCur & 0xffff) != 0) {
                        i = 0;
                        part.hs.grhst = grbitCur;
                        while (1) {
                            part.hs.iItem = i;
                            mdAvail = FLookupPart(&part);
                            if (mdAvail == 0)
                                break;
                            if (part.hs.grhst == hstTerra && GetRaceGrbit(&rgplr[idPlayer], ibitRaceTT) != 0) {
                                switch (part.hs.iItem) {
                                default:
                                    goto L_0dde;
                                case iterraGravityTerraform3:
                                case iterraTempTerraform3:
                                case iterraRadiationTerraform3:
                                    goto L_0e8a;
                                }
                                goto TooManyToFinish;
                            }
                        L_0dde:
                            if (iMin <= mdAvail - 1 && iMax >= mdAvail - 1) {
                                rc.top += dyArial8;
                                if (rc.top + dyArial8 > rc.bottom)
                                    goto TooManyToFinish;
                                fstrcpy(szWork, part.pcom->szName);
                                TextOut(hdc, rc.left + 8, rc.top, szWork, strlen(szWork));
                                if (cFutureTech < 8) {
                                    t_0e66 = cFutureTech;
                                    cFutureTech++;
                                    rghsFutureTech[t_0e66] = part.hs;
                                }
                            }
                            i++;
                        }
                    }
                L_0e8a:;
                }
            }
        TooManyToFinish:
            rgplr[idPlayer].iTechCur = LOBYTE((rgplr[idPlayer].iTechCur & 0xfff0) | iTechSav);
            SetTextColor(hdc, 0);
        }
    }
    SetRect(&rc, dxResLeft + 8, dyArial8, dxResLeft + dxResRight - 8, (int16_t)(14 * dyArial8) / 2 + dyArial8);
    xCtr = rc.left + 8 + dxResStrRight;
    xNum = LOWORD(GetTextExtent(hdc, "999999", 6)) + xCtr;
    if ((grbitDraw & 4) != 0 && (grbitDraw & 3) == 0) {
        ExpandRc(&rc, -6, -(dyArial8 >> 1));
        rc.top += dyArial8 >> 1;
        InvalidateRect(hwnd, &rc, 1);
    } else {
        if ((grbitDraw & 0x300) != 0) {
            if (rgplr[idPlayer].rgTech[iResTechNow] >= 26) {
                l = -1;
            } else {
                l = GetTechLevelCost(iResTechNow, rgplr[idPlayer].rgTech[iResTechNow] + 1, idPlayer);
                lSpent = rgplr[idPlayer].rgResSpent[iResTechNow];
                if (game.fSlowTech != 0) {
                    lSpent = (int32_t)(lSpent * 2);
                }
                l = 0 <= l - lSpent ? l - lSpent : 0;
            }
            if ((grbitDraw & 0x200) != 0 && (grbitDraw & 0xf) == 0) {
                rc.top += (int16_t)(3 * dyArial8) / 2 * 2 + dyArial8;
                hbrSav = SelectObject(hdc, hbrButtonFace);
                PatBlt(hdc, xCtr, rc.top, xNum - xCtr, dyArial8, PATCOPY);
                SelectObject(hdc, hbrSav);
            } else {
                _Draw3dFrame(hdc, &rc, -1);
                c = CchGetString(idsCurrentlyResearching, szWork);
                TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, c);
                rc.top += dyArial8;
                CchGetString(iResTechNow + 84, szTemp);
                CchGetString(idsSTechLevelD, szTemp2);
                c = _wsprintf(szWork, szTemp2, szTemp, rgplr[idPlayer].rgTech[iResTechNow] + 1);
                RightTextOut(hdc, xCtr, rc.top, szWork, c, 0);
                rc.top += (int16_t)(3 * dyArial8) / 2;
                RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsResourcesNeededComplete), 0, 0);
                if (l == -1) {
                    c = CchGetString(idsMaxed, szWork);
                } else {
                    c = _wsprintf(szWork, PCTLD, l);
                }
                TextOut(hdc, xCtr, rc.top, szWork, c);
                rc.top += (int16_t)(3 * dyArial8) / 2;
                RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsEstimatedTimeCompletion), 0, 0);
            }
            if (l == -1) {
                c = CchGetString(idsMaxed, szWork);
                TextOut(hdc, xCtr, rc.top, szWork, c);
            } else {
                if (l == 0) {
                    l = 1;
                } else {
                    if (lResBudget == 0) {
                        c = CchGetString(idsNever2, szWork);
                        TextOut(hdc, xCtr, rc.top, szWork, c);
                        goto L_144a;
                    }
                    lRBEffective = lResBudget;
                    if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceGeneralizedResearch) != 0) {
                        lRBEffective -= (int32_t)(lRBEffective >> 1);
                    }
                    l = (int32_t)((l + lRBEffective - 1) / lRBEffective);
                }
                t_merge_1406_0001 = l == 1 ? 32 : 115;
                c = _wsprintf(szWork, PszGetCompressedString(idsLdYearC), l, t_merge_1406_0001);
                TextOut(hdc, xCtr, rc.top, szWork, c);
            }
        L_144a:
            RightTextOut(hdc, xCtr - 60, rc.top + dyArial8 + 5, PszGetCompressedString(idsFieldResearch), 0, 0);
            GetClientRect(GetDlgItem(hwnd, 0x43b), &rcT);
            MapWindowPoints(GetDlgItem(hwnd, 0x43b), hwnd, (POINT *)&rcT, 2);
            if (rcT.top != rc.top + dyArial8 + 2) {
                SetWindowPos(GetDlgItem(hwnd, 0x43b), NULL, xCtr - 60, rc.top + dyArial8 + 2, rc.right - xCtr + 50, 9 * dyArial8, SWP_NOZORDER | SWP_NOREDRAW);
            }
        }
        rc.top = (int16_t)(3 * dyArial8) / 2 + rc.bottom;
        rc.bottom = dyArial8 * 8 + rc.top;
        if ((grbitDraw & 0x400) != 0) {
            _Draw3dFrame(hdc, &rc, -1);
            c = CchGetString(idsResourceAllocation, szWork);
            TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, c);
        }
        rc.top += dyArial8;
        if ((grbitDraw & 0x800) != 0) {
            RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsAnnualResourcesPlanets), 0, 0);
            c = _wsprintf(szWork, PCTLD, lResTotal);
            RightTextOut(hdc, xNum, rc.top, szWork, c, 0);
        }
        rc.top += (int16_t)(3 * dyArial8) / 2;
        if ((grbitDraw & 0x1000) != 0) {
            RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsTotalResourcesSpentResearchLastYear), 0, 0);
            c = _wsprintf(szWork, PCTLD, rgplr[idPlayer].lResLastYear);
            RightTextOut(hdc, xNum, rc.top, szWork, c, 0);
        }
        rc.top += (int16_t)(3 * dyArial8) / 2;
        dx = 0;
        if ((grbitDraw & 0x2000) != 0) {
            RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsResourcesBudgetedResearch), 0, 0);
        } else if ((grbitDraw & 0x4000) != 0 && (grbitDraw & 0xf) == 0) {
            dx = LOWORD(GetTextExtent(hdc, "100", 3));
        }
        c = _wsprintf(szWork, PCTD, pctResGlob);
        RightTextOut(hdc, xNum, rc.top, szWork, c, dx);
        if ((grbitDraw & 0x4000) == 0 || (grbitDraw & 0xf) != 0) {
            dx = LOWORD(GetTextExtent(hdc, "%", 1));
            TextOut(hdc, xNum, rc.top, "%", 1);
            rcSpinTop.left = xNum + dx + 4;
            rcSpinTop.top = rc.top - 4;
            rcSpinTop.right = rcSpinTop.left + 15;
            rcSpinTop.bottom = (dyArial8 >> 1) + rc.top + 1;
            rcSpinBot = rcSpinTop;
            OffsetRect(&rcSpinBot, 0, rcSpinTop.bottom - rcSpinTop.top - 1);
            DrawBtn(hdc, &rcSpinTop, 160, 0, NULL);
            DrawBtn(hdc, &rcSpinBot, 161, 0, NULL);
        }
        rc.top += (int16_t)(3 * dyArial8) / 2;
        if ((grbitDraw & 0x4000) == 0 || (grbitDraw & 0xf) != 0) {
            RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsYearsProjectedResearchBudget), 0, 0);
        }
        c = _wsprintf(szWork, PCTLD, lResBudget);
        RightTextOut(hdc, xNum, rc.top, szWork, c, xNum - xCtr);
        if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceGeneralizedResearch) != 0 || GetRaceGrbit(&rgplr[idPlayer], ibitRaceBleedingEdgeTech) != 0) {
            cch = CchGetString(idsRaceHas, szTemp);
            SelectObject(hdc, rghfontArial8[1]);
            rc.top += 3 * dyArial8;
            rc.left = dxResLeft + 8;
            yTopTechNote = rc.top;
            dx = LOWORD(GetTextExtent(hdc, szTemp, cch));
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceGeneralizedResearch) != 0) {
                TextOut(hdc, rc.left, rc.top, szTemp, cch);
                c = CchGetString(idsGeneralizedResearch, szWork);
                TextOut(hdc, rc.left + dx, rc.top, szWork, c);
                rc.top += dyArial8 + 2;
            }
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceBleedingEdgeTech) != 0) {
                TextOut(hdc, rc.left, rc.top, szTemp, cch);
                c = CchGetString(idsBleedingEdgeTechnology, szWork);
                TextOut(hdc, rc.left + dx, rc.top, szWork, c);
            }
        } else {
            yTopTechNote = -1;
        }
    }
    SetBkColor(hdc, crBackSav);
    SetTextColor(hdc, crForeSav);
    SelectObject(hdc, hfontSav);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

int16_t FTrackResearchDlg(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    int16_t bt;
    POINT16 pt;
    int16_t dChg;
    int16_t i;
    int16_t cNew;
    RECT   *prc;
    BTNT    btnt;
    RECT    rc;

    pt.x = x;
    pt.y = y;
    if (PtInRect(&rcSpinTop, PointFrom16(pt)) != 0) {
        i = 1;
        prc = &rcSpinTop;
        bt = 160;
    } else if (PtInRect(&rcSpinBot, PointFrom16(pt)) != 0) {
        i = -1;
        prc = &rcSpinBot;
        bt = 161;
    } else {
        if (y >= yTopFutureTech && y < cFutureTech * dyArial8 + yTopFutureTech && x > 12 && x < dxResLeft - 12) {
            i = (int16_t)(y - yTopFutureTech) / dyArial8;
            GlobalPD.part.hs = rghsFutureTech[i];
            FLookupPart(&GlobalPD.part);
            GlobalPD.grPopup = grPopupComponent;
            Popup(hwnd, x, y);
            return 1;
        }
        if (yTopTechNote != -1 && y >= yTopTechNote && y < 3 * dyArial8 + yTopTechNote && x > dxResLeft) {
            if (GetRaceGrbit(&rgplr[idPlayer], ibitRaceGeneralizedResearch) != 0 &&
                (y < (int16_t)(3 * dyArial8) / 2 + yTopTechNote || GetRaceGrbit(&rgplr[idPlayer], ibitRaceBleedingEdgeTech) == 0)) {
                i = 324;
            } else {
                i = 332;
            }
            GlobalPD.psz = PszGetCompressedString(i);
            GlobalPD.dxOut = dxResRight;
            GlobalPD.grPopup = grPopupString;
            Popup(hwnd, x, y);
            return 1;
        }
        return 0;
    }
    GetClientRect(hwnd, &rc);
    InitBtnTrack(&btnt, hwnd, NULL, prc, bt, 80, 0, 0, NULL);
    if ((fkb & 4) != 0) {
        dChg = 10;
    } else {
        dChg = 1;
    }
    while (FTrackBtn(&btnt) != 0) {
        cNew = dChg * i + pctResGlob;
        if (100 < (0 <= cNew ? cNew : 0)) {
            cNew = 100;
        } else if (0 > cNew) {
            cNew = 0;
        }
        if (cNew != pctResGlob) {
            pctResGlob = cNew;
            lResBudget = ProjectedResearchSpending(pctResGlob);
            DrawResearchDlg(hwnd, btnt.hdc, &rc, 16896);
        }
    }
    return 1;
}

int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr) {
    int32_t lCost;
    int16_t i;
    int16_t cTech;

    cTech = 0;
    for (i = 0; i < 6; i++) {
        cTech += rgplr[iplr].rgTech[i];
    }
    lCost = (int16_t)(10 * cTech) + rglTechCost[iLevel];
    i = GetRaceStat(&rgplr[iplr], iTech + 8) - 1;
    if (i != 0) {
        if (i < 0) {
            lCost += lCost - (int32_t)(lCost >> 2);
        } else {
            lCost = (int32_t)(lCost / 2);
        }
    }
    if (game.fSlowTech != 0) {
        lCost = (int32_t)(lCost * 2);
    }
    return lCost;
}

INT_PTR CALLBACK BrowserDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    HMENU       hmenu;
    int16_t     i;
    int16_t     c;
    PAINTSTRUCT ps;
    HFONT       hfontSav;
    int16_t     dx;
    RECT        rc;
    HWND        hwndDD;
    int32_t     lSel;
    int16_t     md;
    int16_t     fShowAll;
    int16_t     fAllHsts;
    uint16_t    iItemStart;
    int16_t     iStart;
    int16_t     cIter;
    int16_t     iOff;
    uint16_t    t_scratch_m3a;
    uint16_t    t_merge_1f1d_0001;
    int16_t     t_scratch_m3c;
    uint16_t    t_scratch_m3e;
    HWND        t_scratch_m38;
    uint16_t    t_scratch_m4a;
    HS         *t_fields_1;
    uint32_t    t_fields_2;
    uint32_t    t_fields_3;

    switch (message) {
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_DESTROY:
        StickyDlgPos(hwnd, &ptStickyBrowserDlg, 0);
        hwndBrowser = 0;
        fBrowserValid = 0;
        hmenu = GetASubMenu(hwndFrame, 5);
        CheckMenuItem(hmenu, 0x100, 0);
        break;
    default:
        if (IS_WM_CTLCOLOR(message) == 0) {
            if (message == WM_INITDIALOG) {
                hwndBrowser = hwnd;
                t_scratch_m3a = GetSystemMetrics(SM_CXDLGFRAME) * 2;
                t_merge_1f1d_0001 = dyArial8 <= 14 ? 0 : 40;
                t_scratch_m3c = GetSystemMetrics(SM_CYCAPTION);
                t_scratch_m3e = GetSystemMetrics(SM_CYDLGFRAME) * 2;
                SetWindowPos(hwnd, NULL, 0, 0, t_merge_1f1d_0001 + 358 + t_scratch_m3a,
                             dyArial10 + 72 + 12 * dyArial8 + 6 + 3 * dyArial8 + 25 + t_scratch_m3e + t_scratch_m3c, SWP_NOMOVE | SWP_NOZORDER);
                StickyDlgPos(hwnd, &ptStickyBrowserDlg, 1);
                hdc = GetDC(hwnd);
                hfontSav = SelectObject(hdc, rghfontArial8[1]);
                hwndDD = GetDlgItem(hwnd, IDC_U16_0x010B);
                c = GetDlgItemText(hwnd, IDC_U16_0x042E, szWork, 80);
                dx = LOWORD(GetTextExtent(hdc, szWork, c)) + 14;
                SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x042E), NULL, 6, 6, dx, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                SetWindowPos(GetDlgItem(hwnd, IDC_NEXT), NULL, (dyArial8 <= 14 ? 0 : 40) + 350 - dx, 6, dx, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                SetWindowPos(hwndDD, NULL, dx + 12, 6, (dyArial8 <= 14 ? 0 : 40) + 344 - dx * 2 - 12, 18 * dyArial8, SWP_NOZORDER);
                SetWindowPos(GetDlgItem(hwnd, IDCANCEL), NULL, (dyArial8 <= 14 ? 0 : 40) + 350 - dx,
                             dyArial10 + 72 + 12 * dyArial8 + 6 + (int16_t)(3 * dyArial8) / 2 + 18, dx, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                SetWindowPos(GetDlgItem(hwnd, 0x10a), NULL, 6, dyArial10 + 72 + 12 * dyArial8 + 6 + (int16_t)(3 * dyArial8) / 2 + 18,
                             (dyArial8 <= 14 ? 0 : 40) + 344 - dx, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                SelectObject(hdc, hfontSav);
                ReleaseDC(hwnd, hdc);
                if (fBrowserValid == 0) {
                    vpartBrowser.hs.grhst = hstArmor;
                    vpartBrowser.hs.iItem = 0;
                }
                FLookupPart(&vpartBrowser);
                hwndBrowserChild = CreateWindow(szBrowser, NULL, WS_CHILD | WS_VISIBLE, 6, (int16_t)(3 * dyArial8) / 2 + 12, (dyArial8 <= 14 ? 0 : 40) + 344,
                                                dyArial10 + 72 + 12 * dyArial8 + 6, hwnd, NULL, hInst, NULL);
                for (i = 1087; i < 1104; i++) {
                    SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
                }
                SendMessage(hwndDD, CB_SETCURSEL, 0, 0);
                if (gd.fTutorial != 0) {
                    AdvanceTutor();
                }
                return 1;
            }
            if (message == WM_COMMAND) {
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDCANCEL:
                    StickyDlgPos(hwnd, &ptStickyBrowserDlg, 0);
                    hwndBrowser = 0;
                    fBrowserValid = 0;
                    hmenu = GetASubMenu(hwndFrame, 5);
                    CheckMenuItem(hmenu, 0x100, 0);
                    EndDialog(hwnd, 1);
                    if (gd.fTutorial != 0) {
                        AdvanceTutor();
                    }
                    return 1;
                case IDC_U16_0x010B:
                    if (GET_WM_COMMAND_CMD(wParam, lParam) != 1)
                        break;
                    fShowAll = IsDlgButtonChecked(hwnd, 266) == 0 ? 1 : 0;
                    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x010B), CB_GETCURSEL, 0, 0);
                    if (lSel < 0)
                        break;
                    vpartBrowser.hs.grhst = rggrbitBrParts[lSel <= 1 ? 1 : lSel];
                    vpartBrowser.hs.iItem = 0;
                    while (1) {
                        md = FLookupPart(&vpartBrowser);
                        if (md == 0)
                            break;
                        if (md == 1 || fShowAll != 0)
                            goto L_2557;
                        vpartBrowser.hs.iItem++;
                    }
                    vpartBrowser.pcom = NULL;
                L_2557:
                    InvalidateRect(hwndBrowserChild, NULL, 1);
                    break;
                case IDC_NEXT:
                case IDC_U16_0x042E:
                    iItemStart = vpartBrowser.hs.iItem;
                    cIter = 0;
                    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x010B), CB_GETCURSEL, 0, 0);
                    fAllHsts = lSel == 0 ? 1 : 0;
                    for (i = 0; i < 17 && vpartBrowser.hs.grhst != rggrbitBrParts[i]; i++) {
                    }
                    iStart = i;
                    fShowAll = IsDlgButtonChecked(hwnd, 266) == 0 ? 1 : 0;
                    iOff = GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT ? 1 : -1;
                    while (1) {
                        t_scratch_m4a = iOff + (vpartBrowser.hs.iItem | vpartBrowser.hs.cItem << 8) & 0xff;
                        vpartBrowser.hs.iItem = 0;
                        t_fields_1 = &vpartBrowser.hs;
                        t_fields_2 = t_fields_1->iItem | ((uint32_t)t_scratch_m4a & 0xff);
                        t_fields_3 = t_fields_1->cItem | ((uint32_t)t_scratch_m4a >> 8 & 0xff);
                        t_fields_1->iItem = t_fields_2;
                        t_fields_1->cItem = t_fields_3;
                        if (t_scratch_m4a == iItemStart && fAllHsts == 0)
                            goto L_27b3;
                        while (1) {
                            if (cIter++ > 350)
                                goto NullItem;
                            md = FLookupPart(&vpartBrowser);
                            if (md != 0)
                                break;
                            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT && fAllHsts != 0) {
                                i++;
                                if (i >= 17) {
                                    i = 1;
                                }
                                vpartBrowser.hs.grhst = rggrbitBrParts[i];
                                vpartBrowser.hs.iItem = 0;
                            } else {
                                if (vpartBrowser.hs.iItem == 0 && fAllHsts == 0)
                                    goto L_26f9;
                                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x042E && fAllHsts != 0) {
                                    if (vpartBrowser.hs.iItem <= 100)
                                        break;
                                    i--;
                                    if (i <= 0) {
                                        i = 16;
                                    }
                                    vpartBrowser.hs.grhst = rggrbitBrParts[i];
                                    vpartBrowser.hs.iItem = 100;
                                } else {
                                    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_NEXT)
                                        break;
                                    vpartBrowser.hs.iItem = 0;
                                }
                            }
                        }
                        if (md == 1 || (md != 0 && fShowAll != 0 && (md != -1 || FShouldPartBeHidden(&vpartBrowser) == 0)))
                            goto L_27b3;
                    }
                L_26f9:
                    vpartBrowser.pcom = NULL;
                L_27b3:
                    if (vpartBrowser.hs.iItem == iItemStart && iStart == i && (md == 1 || fShowAll != 0))
                        break;
                    if (vpartBrowser.hs.iItem != iItemStart || iStart != i)
                        goto L_281a;
                    if (FLookupPart(&vpartBrowser) == 1)
                        break;
                NullItem:
                    vpartBrowser.pcom = NULL;
                L_281a:
                    InvalidateRect(hwndBrowserChild, NULL, 1);
                }
            }
        } else {
            for (i = 266; i <= 267; i++) {
                t_scratch_m38 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
                if (t_scratch_m38 == GetDlgItem(hwnd, i))
                    break;
            }
            if (i <= 267 || HIWORD(lParam) == 6) {
                SetBkColor((HDC)wParam, crButtonFace);
                return (INT_PTR)hbrButtonFace;
            }
        }
    }
    return 0;
}

LRESULT CALLBACK BrowserWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    POINT16     pt;
    int16_t     i;
    PAINTSTRUCT ps;
    RECT        rc;
    POINT       t_pt_28d7;
    POINT       t_pt_28e6_1;

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DisplayComponentInfo(hdc, rc.right, rc.bottom, &vpartBrowser);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_SETCURSOR:
        GetCursorPos(&t_pt_28d7);
        pt = PointTo16(t_pt_28d7);
        t_pt_28e6_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_28e6_1);
        pt = PointTo16(t_pt_28e6_1);
        break;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_LBUTTONDOWN:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        break;
    default:
    Default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    case WM_CREATE:
        return 0;
    }
    if ((vpartBrowser.hs.grhst != hstHull && vpartBrowser.hs.grhst != hstSBHull) || hwndSlotDlg != 0 || pt.x < 5 || pt.x >= 69 || pt.y < dyArial10 + 5 ||
        pt.y >= dyArial10 + 69)
        goto Default;
    if (message == WM_LBUTTONDOWN) {
        GlobalPD.grPopup = grPopupShdefSB;
        if (vpartBrowser.hs.grhst == hstSBHull) {
            shdefBuild.hul = LphuldefSBFromId(vpartBrowser.hs.iItem)->hul;
        } else {
            shdefBuild.hul = LphuldefFromId(vpartBrowser.hs.iItem)->hul;
        }
        for (i = 0; i < shdefBuild.hul.chs; i++) {
            shdefBuild.hul.rghs[i].cItem = 0;
        }
        GlobalPD.lpshdef = &shdefBuild;
        GlobalPD.fShowDamage = 0;
        Popup(hwnd, pt.x, pt.y);
    } else {
        SetCursor(hcurArrowHelp);
    }
    return 1;
}

void DisplayComponentInfo(HDC hdc, int16_t dx, int16_t dy, PART *ppart) {
    uint16_t rgCosts[4];
    int16_t  idsT;
    StringId ids;
    int16_t  dxStr;
    int16_t  c;
    int16_t  yText;
    int16_t  i;
    int16_t  yCur;
    int16_t  fReq;
    int16_t  yStart;
    int16_t  xNum;
    int16_t  xText;
    RECT     rcData;
    int32_t  l;
    int16_t  dxT;
    char     rgch[2];
    int16_t  dyPct;
    int16_t  dxDigit;
    int16_t  yBase;
    int16_t  y;
    int16_t  dxWarp;
    int16_t  pct;
    int16_t  fWarp10;
    int16_t  iEff;
    COLORREF crFore;
    int16_t  cch;
    int16_t  x;
    HPEN     hpenSav;
    COLORREF crBack;
    HBRUSH   hbrSav;
    int16_t  pctT;
    char     szT[256];
    int16_t  dyText;
    char    *psz;
    int16_t  dmgFloor;
    int16_t  dmgMin;
    int16_t  dmgShipRam;
    int16_t  iWarp;
    int16_t  dmgShip;
    int16_t  pctHit;
    int16_t  dxLabel;
    int16_t  dmgMinRam;
    int32_t  lpct;
    int16_t  xBase;
    int16_t  dxQuan;
    char     ch;
    int32_t  ldelta;
    RECT     rcT;
    uint16_t t_merge_5094_0001;
    uint16_t t_merge_522a_0001;
    uint16_t t_merge_526a_0001;
    uint16_t t_merge_52aa_0001;
    uint16_t t_merge_6105_0001;
    COLORREF t_merge_6561_0001;

    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, 0, dy - 1, dx, 1, PATCOPY);
    PatBlt(hdc, dx - 1, 0, 1, dy, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, 0, 0, dx, 1, PATCOPY);
    PatBlt(hdc, 0, 0, 1, dy, PATCOPY);
    PatBlt(hdc, 4, dy - dyArial8 * 2 - 2, dx - 8, 1, PATCOPY);
    SetBkMode(hdc, TRANSPARENT);
    if (ppart->pcom == 0) {
        SetRect(&rcData, 5, 5, dx - 5, dy - 5);
        c = CchGetString(idsNoneAvailable, szWork);
        DiaganolTextOut(hdc, &rcData, szWork, c);
    } else {
        SelectObject(hdc, rghfontArial10[1]);
        fstrcpy(szWork, ppart->pcom->szName);
        CtrTextOut(hdc, dx >> 1, 3, szWork, 0);
        if (ppart->hs.grhst == hstHull || ppart->hs.grhst == hstSBHull) {
            DrawFleetBitmap(NULL, hdc, 5, dyArial10 + 5, 0, ppart->pcom->ibmp, 0, 0, -1, 0);
        } else {
            SelectPalette(hdc, vhpal, 0);
            RealizePalette(hdc);
            DibBlt(hdc, 5, dyArial10 + 5, 64, 64, rghdibInventory[ppart->pcom->ibmp >> 5], (ppart->pcom->ibmp & 7) * 0x40,
                   (3 - (ppart->pcom->ibmp >> 3) & 3) * 0x40, 64, 64, 13369376);
        }
        SelectObject(hdc, rghfontArial8[1]);
        yCur = dyArial10 + 71;
        c = CchGetString(idsTechReq, szWork);
        TextOut(hdc, 5, yCur, szWork, c);
        dxStr = 0;
        for (i = 0; i < 6; i++) {
            c = CchGetString(i + 91, szWork);
            dxT = LOWORD(GetTextExtent(hdc, szWork, c));
            if (dxT > dxStr) {
                dxStr = dxT;
            }
        }
        dxStr += 5;
        xNum = dxStr + LOWORD(GetTextExtent(hdc, "99", 2));
        fReq = 0;
        for (i = 0; i < 6; i++) {
            if (ppart->pcom->rgTech[i] > 0) {
                yCur += dyArial8;
                SetTextColor(hdc, ppart->pcom->rgTech[i] <= rgplr[idPlayer].rgTech[i] ? 0 : 127);
                c = CchGetString(i + 91, szWork);
                RightTextOut(hdc, dxStr, yCur, szWork, c, 0);
                SetTextColor(hdc, 0);
                c = _wsprintf(szWork, PCTD, ppart->pcom->rgTech[i]);
                RightTextOut(hdc, xNum, yCur, szWork, c, 0);
                fReq = 1;
            }
        }
        if (fReq == 0) {
            yCur += dyArial8;
            c = CchGetString(idsNone3, szWork);
            CtrTextOut(hdc, 37, yCur, szWork, c);
        }
        if (FLookupPart(ppart) <= 0) {
            l = -1;
        } else {
            l = CostOfDevelopingItem(ppart->pcom->rgTech);
        }
        if (l > 99999) {
            c = _wsprintf(szWork, PszGetCompressedString(idsCostLdk), (int32_t)((l + 500) / 1000));
            TextOut(hdc, 5, yCur + dyArial8 + 4, szWork, c);
        } else if (l > 0) {
            c = _wsprintf(szWork, PszGetCompressedString(idsCostLd), l);
            TextOut(hdc, 5, yCur + dyArial8 + 4, szWork, c);
        } else if (l == -1) {
            SetTextColor(hdc, 127);
            CtrTextOut(hdc, 37, yCur + dyArial8 + 4, PszGetCompressedString(idsUnavail), 0);
            SetTextColor(hdc, 0);
        } else {
            CtrTextOut(hdc, 37, yCur + dyArial8 + 4, PszGetCompressedString(idsAvailable), 0);
        }
        yStart = dyArial10 + 5;
        if (dyArial8 * 4 < 64) {
            yStart += (0x40 - (dyArial8 << 2)) >> 1;
        }
        yCur = yStart;
        dxStr = 0;
        for (i = 0; i <= 5; i++) {
            dxT = LOWORD(GetTextExtent(hdc, rgszMinerals[i], lstrlen(rgszMinerals[i])));
            if (dxT > dxStr) {
                dxStr = dxT;
            }
        }
        dxStr += 77;
        GetTruePartCost(idPlayer, ppart, rgCosts);
        for (i = 0; i <= 5; i++) {
            if (i != 4 && i != 3) {
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, rgcrMinerals[i]);
                RightTextOut(hdc, dxStr, yCur, rgszMinerals[i], 0, 0);
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crWindowText);
                if (i != 5) {
                    c = rgCosts[i];
                } else {
                    c = rgCosts[3];
                }
                if (ppart->hs.grhst == hstSBHull || ppart->hs.grhst == hstSpecialSB) {
                    c -= c / 2;
                }
                c = _wsprintf(szWork, PCTD, c);
                RightTextOut(hdc, dxStr + dxMaxMineralQuan, yCur, szWork, c, 0);
                if (i < 5) {
                    TextOut(hdc, dxStr + dxMaxMineralQuan, yCur, "kT", 2);
                }
                yCur += dyArial8;
            }
        }
        SelectObject(hdc, rghfontArial8[1]);
        if (ppart->pcom->cMass != 0) {
            c = _wsprintf(szWork, PszGetCompressedString(idsMassDkt), ppart->pcom->cMass);
            TextOut(hdc, dxStr + dxMaxMineralQuan + 32, yStart, szWork, c);
        }
        SetRect(&rcData, 73 <= xNum + 4 ? xNum + 4 : 73, dyArial10 + 71, dx - 5, dy - 5 - dyArial8 * 2 - 4);
        if (dyArial8 > 14) {
            rcData.left += 4;
        }
        SelectObject(hdc, hbrButtonShadow);
        PatBlt(hdc, rcData.left, rcData.top, rcData.right - rcData.left, 1, PATCOPY);
        PatBlt(hdc, rcData.left, rcData.top, 1, rcData.bottom - rcData.top, PATCOPY);
        SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, rcData.left, rcData.bottom - 1, rcData.right - rcData.left, 1, PATCOPY);
        PatBlt(hdc, rcData.right - 1, rcData.top, 1, rcData.bottom - rcData.top, PATCOPY);
        ExpandRc(&rcData, -3, -3);
        if (gd.fBleedingEdge != 0 && l == 0) {
            SetTextColor(hdc, crButtonHilite);
            c = CchGetString(idsBleedingEdge, szWork);
            DiaganolTextOut(hdc, &rcData, szWork, c);
            SetTextColor(hdc, crWindowText);
        }
        ids = 0xffff;
        switch (ppart->hs.grhst) {
        case hstEngine:
            SelectObject(hdc, rghfontArial8[1]);
            rcData.right -= 2;
            c = CchGetString(idsFuelUsageVsWarpSpeed, szWork);
            CtrTextOut(hdc, ((rcData.right - rcData.left) >> 1) + rcData.left, rcData.top, szWork, c);
            rcData.top += dyArial8;
            c = CchGetString(idsWarp, szWork);
            dxStr = LOWORD(GetTextExtent(hdc, szWork, c));
            dxWarp = (int16_t)(rcData.right - rcData.left - dxStr - 8) / 10;
            dyPct = (int16_t)(rcData.bottom - rcData.top - dyArial8 - 8) / 6;
            PatBlt(hdc, rcData.left + dxStr + 6, rcData.top, 1, rcData.bottom - rcData.top - dyArial8 - 4, BLACKNESS);
            PatBlt(hdc, rcData.left + dxStr + 6, rcData.bottom - dyArial8 - 4, rcData.right - rcData.left - dxStr - 6, 1, BLACKNESS);
            x = rcData.left + dxStr;
            y = rcData.bottom - dyArial8 - 4 - 6 * dyPct;
            pct = 800;
            SetTextColor(hdc, 127);
            while (pct >= 25) {
                if (pct == 100) {
                    SetTextColor(hdc, 0);
                    SelectObject(hdc, hbrGray);
                    PatBlt(hdc, x + 4, y, rcData.right - rcData.left - dxStr - 4, 1, PATCOPY);
                }
                c = _wsprintf(szWork, PCTDPCTPCT, pct);
                RightTextOut(hdc, x, y - (dyArial8 >> 1), szWork, c, 0);
                PatBlt(hdc, x + 4, y, 5, 1, BLACKNESS);
                pct >>= 1;
                y += dyPct;
            }
            y = rcData.bottom - dyArial8;
            RightTextOut(hdc, x, y, PszGetCompressedString(idsWarp), 0, 0);
            x += 6;
            yBase = rcData.bottom - dyArial8 - 4;
            dxDigit = LOWORD(GetTextExtent(hdc, "0", 1)) >> 1;
            rgch[0] = '0';
            cch = 1;
            crFore = SetTextColor(hdc, crButtonText);
            switch (ppart->hs.iItem) {
            case iengineInterspace10:
            case iengineTransStar10:
            case iengineTransGalacticMizerScoop:
            case iengineGalaxyScoop:
            case iengineEnigmaPulsar:
                fWarp10 = 1;
                break;
            default:
                fWarp10 = 0;
            }
            for (i = 0; i <= 10; i++) {
                if (i == 10) {
                    cch = 2;
                    rgch[0] = '1';
                    rgch[1] = '0';
                    if (fWarp10 == 0) {
                        hbrSav = SelectObject(hdc, rghbrPat[0]);
                        SetTextColor(hdc, 0xffff);
                        crBack = SetBkColor(hdc, crButtonFace);
                        PatBlt(hdc, x - dxWarp + 1, rcData.top, dxWarp, yBase - rcData.top, PATCOPY);
                        SetBkColor(hdc, crBack);
                        SelectObject(hdc, hbrSav);
                    }
                }
                if (ppart->pengine->rgcFuelUsed[i] <= 120 &&
                    ((i == 10 && fWarp10 != 0) || ppart->pengine->rgcFuelUsed[i + 1] > 120 || (i == 9 && fWarp10 == 0))) {
                    SetTextColor(hdc, 8323072);
                } else {
                    SetTextColor(hdc, crButtonText);
                }
                TextOut(hdc, x - dxDigit * cch, y, rgch, cch);
                if (i > 0) {
                    PatBlt(hdc, x, y - 6, 1, 5, BLACKNESS);
                }
                rgch[0]++;
                x += dxWarp;
            }
            SetTextColor(hdc, crFore);
            hpenSav = SelectObject(hdc, hpenDkBlue);
            x = rcData.left + dxStr + 6;
            for (i = 0; i <= 10; i++) {
                y = yBase;
                pct = 25;
                iEff = ppart->pengine->rgcFuelUsed[i];
                while (iEff >= pct) {
                    pct *= 2;
                    y -= dyPct;
                }
                if (pct != 25) {
                    pctT = MulDiv(iEff - (pct >> 1), 100, pct >> 1);
                    y -= (int16_t)(pctT * dyPct) / 100;
                } else if (iEff == 0) {
                    y--;
                } else {
                    y -= 1 <= (int16_t)(iEff * 4 * dyPct) / 100 ? (int16_t)(iEff * 4 * dyPct) / 100 : 1;
                }
                if (i == 0) {
                    MoveTo(hdc, x, y);
                } else {
                    LineTo(hdc, x, y);
                }
                x += dxWarp;
            }
            SelectObject(hdc, hpenSav);
            if ((uint16_t)ppart->pengine->grfAbilities <= 6) {
                switch (ppart->pengine->grfAbilities) {
                case 0:
                    goto L_3a6c;
                case 1:
                    ids = idsEngineCanMountedMiniColonizerHullRequires;
                    break;
                case 2:
                    ids = idsEngineCreatesPowerfulWavesRadiationWillKill;
                    break;
                case 3:
                    ids = idsEngineRequiresLesserRacialTraitImprovedFuel;
                    break;
                case 4:
                    ids = idsEngineRequiresLesserRacialTraitImprovedFuel2;
                    break;
                case 5:
                    ids = idsEngineRequiresLesserRacialTraitRamScoop;
                    break;
                case 6:
                    ids = idsOriginEngineUnknownAdds14Square;
                }
                break;
            }
        L_3a6c:
            if (ppart->hs.iItem < iengineSubGalacticFuelScoop || ppart->hs.iItem > iengineTransGalacticMizerScoop)
                break;
            ids = idsEngineWillUnavailableIfHaveLesserRacial;
            break;
        case hstScanner:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[0]);
            i = ppart->pscanner->dRange;
            if (i == 0) {
                c = CchGetString(idsEnemyFleetsCannotDetectedScannerUnlessSame, szWork);
            } else {
                c = CchGetString(idsEnemyFleetsOrbitingPlanetCanDetectedD, szT);
                c = _wsprintf(szWork, szT, i);
            }
            dyText = DrawText(hdc, szWork, c, &rcData, 2064);
            rcData.top += (dyArial8 >> 1) + dyText;
            i = ppart->pscanner->grfAbilities;
            if (i == 0) {
                c = CchGetString(idsScannerCapableDeterminingPlanetsEnvironmentCompo, szWork);
            } else if (i != 4) {
                c = CchGetString(idsScannerCanDeterminePlanetsBasicStatsDistance, szT);
                c = _wsprintf(szWork, szT, i == 1 ? 50 : i == 2 ? 100 : 200);
                ids = idsScannerWillUnavailableIfHaveLesserRacial;
            } else {
                if (ppart->hs.iItem == iscannerPickPocketScanner) {
                    ids = idsScannerCapablePenetratingDefensesEnemyFleetsAllo;
                } else if (ppart->hs.iItem == iscannerRobberBaronScanner) {
                    ids = idsScannerCanDeterminePlanetsStatsDistance120;
                } else {
                    ids = idsScannerCanDeterminePlanetsBasicStatsDistance2;
                }
                c = CchGetString(ids, szWork);
                ids = idsScannerRequiresPrimaryRacialTraitSuperStealth;
            }
            dyText = DrawText(hdc, szWork, c, &rcData, 2064);
            break;
        case hstHull:
            ExpandRc(&rcData, -4, -4);
            xText = (int16_t)(rcData.right - rcData.left) / 2 + rcData.left;
            idsT = 72;
            switch (ppart->hs.iItem) {
            case ihuldefMiniColonyShip:
            case ihuldefMetaMorph:
                ids = idsHullRequiresPrimaryRacialTraitHyperExpansion;
                break;
            case ihuldefSuperFreighter:
            case ihuldefFuelTransport:
                ids = idsHullRequiresPrimaryRacialTraitInnerStrength;
                break;
            case ihuldefMaxiMiner:
                ids = idsHullUnavailableIfHaveRaceDisadvantageBasic;
                break;
            case ihuldefMidgetMiner:
            case ihuldefMiner:
            case ihuldefUltraMiner:
                ids = idsMiningHullRequiresLesserRacialTraitAdvanced;
                break;
            case ihuldefDreadnought:
            case ihuldefBattleCruiser:
                ids = idsHullRequiresPrimaryRacialTraitWarMonger;
                break;
            case ihuldefRogue:
            case ihuldefStealthBomber:
                ids = idsHullRequiresPrimaryRacialTraitSuperStealth;
                break;
            case ihuldefMiniMineLayer:
            case ihuldefSuperMineLayer:
                ids = idsHullRequiresPrimaryRaceTraitSpaceDemolition;
                break;
            case ihuldefScout:
            case ihuldefFrigate:
            case ihuldefDestroyer:
                ids = idsHullWillHaveBuiltScannerIfJack;
                break;
            case ihuldefMiniMorph:
                ids = idsOriginHullUnknown;
            }
            i = 0;
            while (i < 4) {
                c = CchGetString(idsT, szWork);
                SelectObject(hdc, rghfontArial8[1]);
                RightTextOut(hdc, xText, rcData.top, szWork, c, 0);
                SelectObject(hdc, rghfontArial8[0]);
                switch (i) {
                case 0:
                    c = _wsprintf(szWork, "%dmg", ppart->phul->wtFuelMax);
                    break;
                case 1:
                    c = _wsprintf(szWork, PCTDKT, ppart->phul->wtCargoMax);
                    break;
                case 2:
                    c = _wsprintf(szWork, PCTD, ppart->phul->dp);
                    break;
                case 3:
                    c = _wsprintf(szWork, PCTD, LphuldefFromId(ppart->phul->ihuldef)->init);
                }
                TextOut(hdc, xText, rcData.top, szWork, c);
                i++;
                idsT++;
                rcData.top += (int16_t)(3 * dyArial8) / 2;
            }
            switch (ppart->hs.iItem) {
            case ihuldefFuelTransport:
            case ihuldefSuperFuelXport:
                psz = PszGetCompressedString(idsHullWillManufacture200UnitsFuelEach);
                if (ppart->hs.iItem == ihuldefFuelTransport) {
                    pct = 5;
                } else {
                    pct = 10;
                }
                c = _wsprintf(szWork, psz, pct);
                SelectObject(hdc, rghfontArial8[0]);
                xText = rcData.left;
                yText = rcData.top;
                WrapTextOut(hdc, &xText, &yText, szWork, c, xText, rcData.right - rcData.left, NULL, 0, 1);
                break;
            case ihuldefMiniMineLayer:
            case ihuldefSuperMineLayer:
                c = CchGetString(idsHullWillDoubleEfficiencyMineLayingPods, szWork);
                SelectObject(hdc, rghfontArial8[0]);
                xText = rcData.left;
                yText = rcData.top;
                WrapTextOut(hdc, &xText, &yText, szWork, c, xText, rcData.right - rcData.left, NULL, 0, 1);
            }
            break;
        case hstSBHull:
            ExpandRc(&rcData, -4, -4);
            idsT = 74;
            xText = rcData.left;
            yText = rcData.top;
            switch (ppart->hs.iItem) {
            case 1:
            case 3:
                ids = idsStarbaseHullRequiresLesserRacialTraitImproved;
                break;
            case 4:
                ids = idsHullRequiresPrimaryRacialTraitAlternateReality;
            }
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->phul->wtCargoMax != 0) {
                c = CchGetString(idsStarbaseHullHasSpaceDockCanBuild, szWork);
                WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
                SelectObject(hdc, rghfontArial8[1]);
                yText = 3 * dyArial8 + rcData.top;
                xText = (int16_t)(rcData.right - rcData.left) / 2 + rcData.left;
                c = CchGetString(idsDockCapacity2, szWork);
                RightTextOut(hdc, xText, yText, szWork, c, 0);
                SelectObject(hdc, rghfontArial8[0]);
                if (ppart->phul->wtCargoMax != 0xffff) {
                    c = _wsprintf(szWork, PCTDKT, ppart->phul->wtCargoMax);
                } else {
                    c = CchGetString(idsUnlimited, szWork);
                }
                TextOut(hdc, xText, yText, szWork, c);
            } else {
                c = CchGetString(idsStarbaseHullDoesHaveSpaceDockCan, szWork);
                WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
                yText = 3 * dyArial8 + rcData.top;
            }
            yText += (int16_t)(3 * dyArial8) / 2;
            xText = (int16_t)(rcData.right - rcData.left) / 2 + rcData.left;
            i = 0;
            while (i < 2) {
                c = CchGetString(idsT, szWork);
                SelectObject(hdc, rghfontArial8[1]);
                RightTextOut(hdc, xText, yText, szWork, c, 0);
                SelectObject(hdc, rghfontArial8[0]);
                if (i == 0) {
                    c = _wsprintf(szWork, PCTD, ppart->phul->dp);
                } else if (i == 1) {
                    c = _wsprintf(szWork, PCTD, LphuldefFromId(ppart->phul->ihuldef)->init);
                }
                TextOut(hdc, xText, yText, szWork, c);
                i++;
                idsT++;
                yText += (int16_t)(3 * dyArial8) / 2;
            }
            break;
        case hstShield:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            DxStreamTextOut(hdc, &xText, rcData.top, PszGetCompressedString(idsShieldStrength), 0, 1);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->pshield->dp);
            DxStreamTextOut(hdc, &xText, rcData.top, szWork, c, 1);
            switch (ppart->hs.iItem) {
            case ishieldShadowShield:
                ids = idsArmorShieldRequiresPrimaryRacialTraitSuper;
                idsT = 60;
                goto LShieldDisp;
            case ishieldCrobySharmor:
                ids = idsShieldRequiresPrimaryRacialTraitInnerStrength;
                idsT = 63;
                goto LShieldDisp;
            case ishieldLangstonShell:
                ids = idsOriginPartUnknown;
                idsT = 64;
                goto LShieldDisp;
            }
            break;
        LShieldDisp:
            c = CchGetString(idsT, szWork);
            rcData.top += dyArial8 + 4;
            DrawText(hdc, szWork, c, &rcData, 2064);
            break;
        case hstArmor:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            DxStreamTextOut(hdc, &xText, rcData.top, PszGetCompressedString(idsArmorStrength2), 0, 1);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->parmor->dp);
            DxStreamTextOut(hdc, &xText, rcData.top, szWork, c, 1);
            switch (ppart->hs.iItem) {
            case iarmorDepletedNeutronium:
                ids = idsArmorShieldRequiresPrimaryRacialTraitSuper;
                idsT = 61;
                goto LArmDisp;
            case iarmorFieldedKelarium:
                ids = idsArmorRequiresPrimaryRacialTraitInnerStrength;
                idsT = 62;
                goto LArmDisp;
            case iarmorMegaPolyShell:
                ids = idsOriginPartUnknown;
                c = CchGetString(idsPartAlsoActs100dpShield20Cloak, szWork);
                rcData.top += dyArial8 + 4;
                SelectObject(hdc, rghfontArial8[0]);
                DrawText(hdc, szWork, c, &rcData, 2064);
            }
            break;
        LArmDisp:
            c = CchGetString(idsT, szWork);
            rcData.top += dyArial8 + 4;
            DrawText(hdc, szWork, c, &rcData, 2064);
            break;
        case hstBeam:
            ExpandRc(&rcData, -4, -4);
            xText = (int16_t)(rcData.right - rcData.left) / 3 + rcData.left;
            yText = rcData.top;
            c = CchGetString(idsPower, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->pbeam->dp);
            TextOut(hdc, xText, yText, szWork, c);
            yText += dyArial8;
            c = CchGetString(idsRange, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->pbeam->dRangeMax);
            TextOut(hdc, xText, yText, szWork, c);
            yText += dyArial8;
            c = CchGetString(idsInitiative, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->pbeam->init);
            TextOut(hdc, xText, yText, szWork, c);
            yText += (int16_t)(3 * dyArial8) / 2;
            rcData.top = yText;
            if (ppart->pbeam->grfAbilities != 0) {
                SelectObject(hdc, rghfontArial7[0]);
                if ((ppart->pbeam->grfAbilities & 1) != 0) {
                    c = CchGetString(idsWeaponWillDamageShieldsHasEffectArmor, szWork);
                    rcData.top += DrawText(hdc, szWork, c, &rcData, 2064);
                }
                if ((ppart->pbeam->grfAbilities & 2) != 0) {
                    c = CchGetString(idsWeaponHitsTargetsRangeEachTimeFired, szWork);
                    rcData.top += DrawText(hdc, szWork, c, &rcData, 2064);
                    c = _wsprintf(szWork, PszGetCompressedString(idsWeaponAlsoMakesExcellentMineSweeperCapable), ppart->pbeam->dp * 16);
                    DrawText(hdc, szWork, c, &rcData, 2064);
                }
                SelectObject(hdc, rghfontArial8[0]);
            } else if (ppart->hs.iItem == ibeamMultiContainedMunition) {
                ids = idsOriginPartUnknown;
                c = CchGetString(idsPartAlsoActs10CloakIncreasesTorpedo, szWork);
                SelectObject(hdc, rghfontArial7[0]);
                rcData.top += DrawText(hdc, szWork, c, &rcData, 2064);
                c = CchGetString(idsWeaponCanAlsoBombPlanets2Colonists, szWork);
                DrawText(hdc, szWork, c, &rcData, 2064);
            }
            switch (ppart->hs.iItem) {
            case ibeamMiniGun:
                ids = idsPartRequiresPrimaryRacialTraitInnerStrength;
                break;
            case ibeamGatlingNeutrinoCannon:
            case ibeamBlunderbuss:
                ids = idsPartRequiresPrimaryRacialTraitWarMonger;
            }
            break;
        case hstTorp:
            ExpandRc(&rcData, -4, -4);
            xText = (int16_t)(rcData.right - rcData.left) / 3 + rcData.left;
            yText = rcData.top;
            c = CchGetString(idsPower, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->ptorp->dp);
            TextOut(hdc, xText, yText, szWork, c);
            yText += dyArial8;
            c = CchGetString(idsRange, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->ptorp->dRangeMax);
            TextOut(hdc, xText, yText, szWork, c);
            yText += dyArial8;
            c = CchGetString(idsInitiative, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->ptorp->init);
            TextOut(hdc, xText, yText, szWork, c);
            yText += dyArial8;
            c = CchGetString(idsAccuracy, szWork);
            SelectObject(hdc, rghfontArial8[1]);
            RightTextOut(hdc, xText, yText, szWork, c, 0);
            SelectObject(hdc, rghfontArial8[0]);
            c = _wsprintf(szWork, PCTD, ppart->ptorp->dHitChance);
            TextOut(hdc, xText, yText, szWork, c);
            yText += (int16_t)(3 * dyArial8) / 2;
            if (ppart->hs.iItem == itorpAntiMatterTorpedo) {
                ids = idsOriginPartUnknown;
                break;
            }
            if (ppart->hs.iItem < itorpJihadMissile || ppart->hs.iItem > itorpArmageddonMissile)
                break;
            ids = idsCapitalShipMissilesDoTwiceStatedDamage;
            break;
        case hstBomb:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem == ibombRetroBomb) {
                c = CchGetString(idsBombDoesKillColonistsDestroyInstallationsBomb, szWork);
            } else if (ppart->pbomb->dDmgCol == 0) {
                c = CchGetString(idsBombWillKillAnyPlanetsPopulation, szWork);
            } else {
                CchGetString(idsBombWillKillApproximatelyDDPlanets, szWork);
                c = _wsprintf(szT, szWork, ppart->pbomb->dDmgCol / 10, ppart->pbomb->dDmgCol % 10);
                if (ppart->hs.iItem >= ibombLadyFingerBomb && ppart->hs.iItem <= ibombCherryBomb) {
                    dmgFloor = 3;
                } else if (ppart->hs.iItem >= ibombSmartBomb && ppart->hs.iItem <= ibombAnnihilatorBomb) {
                    dmgFloor = 999;
                } else {
                    dmgFloor = 0;
                }
                if (dmgFloor > 0) {
                    CchGetString(dmgFloor == 999 ? idsSmartBombsStrictlyAdditiveHaveMinimumKill : idsIfPlanetHasDefensesBombGuaranteedKill, szWork);
                    c += _wsprintf(&szT[c], szWork, 100 * dmgFloor);
                }
                strcpy(szWork, szT);
            }
            WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
            xText = rcData.left;
            yText += dyArial8 >> 1;
            if (ppart->hs.iItem == ibombRetroBomb) {
                ids = idsPartRequiresPrimaryRacialTraitClaimAdjuster;
                break;
            }
            if (ppart->pbomb->dDmgBldg == 0) {
                c = CchGetString(idsBombWillDamagePlanetsMinesFactories, szWork);
            } else {
                CchGetString(idsBombWillDestroyApproximatelyDPlanetsMines, szT);
                c = _wsprintf(szWork, szT, ppart->pbomb->dDmgBldg);
            }
            WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 1, 1);
            if (ppart->hs.iItem >= ibombSmartBomb && ppart->hs.iItem <= ibombAnnihilatorBomb) {
                ids = idsBombWillAvailableIfPrimaryRaceTrait;
                break;
            }
            if (ppart->hs.iItem != ibombHushABoom)
                break;
            ids = idsOriginPartUnknown;
            break;
        case hstSpecialE:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem <= ispecialEAntiMatterGenerator) {
                switch (ppart->hs.iItem) {
                case 0:
                    idsT = 219;
                    ids = idsCloakRequiresPrimaryRacialTraitSuperStealth;
                    goto L_5168;
                case 1:
                case 2:
                case 3:
                    if (ppart->hs.iItem == ispecialEUltraStealthCloak) {
                        ids = idsCloakRequiresPrimaryRacialTraitSuperStealth;
                        idsT = 85;
                    } else {
                        idsT = ppart->hs.iItem == ispecialEStealthCloak ? 35 : 55;
                    }
                    c = _wsprintf(szWork, PszGetCompressedString(idsCloaksAnyShipReducingRangeWhichScanners), idsT);
                    goto PrintSpecial;
                case 4:
                    idsT = 221;
                    ids = idsOriginPartUnknown;
                    goto L_5168;
                case 14:
                    ids = idsDeviceRequiresPrimaryRacialTraitSpaceDemolition;
                    idsT = 923;
                    goto L_5168;
                case 15:
                    ids = idsDeviceRequiresPrimaryRacialTraitInnerStrength;
                    idsT = 924;
                    goto L_5168;
                case 16:
                    ids = idsDeviceRequiresPrimaryRacialTraitInterstellarTrav;
                    idsT = 925;
                    goto L_5168;
                case 13:
                    ids = idsDeviceRequiresPrimaryRacialTraitHyperExpansion;
                case 12:
                    t_merge_5094_0001 = ppart->hs.iItem == ispecialEEnergyCapacitor ? 10 : 20;
                    c = _wsprintf(szWork, PszGetCompressedString(idsIncreasesDamageDoneBeamWeaponsShipD), t_merge_5094_0001);
                    goto PrintSpecial;
                case 8:
                case 11:
                    ids = idsJammingDeviceRequiresPrimaryRacialTraitInner;
                case 9:
                case 10:
                    idsT = 928;
                    c = _wsprintf(szWork, PszGetCompressedString(idsT), ppart->pspecial->grAbility);
                    goto PrintSpecial;
                case 5:
                case 6:
                case 7:
                    idsT = 929;
                    c = _wsprintf(szWork, PszGetCompressedString(idsT), ppart->pspecial->grAbility, ppart->hs.iItem - 4);
                    goto PrintSpecial;
                }
                break;
            }
            idsT = -1;
        L_5168:
            if (idsT == -1)
                break;
            c = CchGetString(idsT, szWork);
            goto PrintSpecial;
        case hstSpecialM:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem <= ispecialMBeamDeflector) {
                switch (ppart->hs.iItem) {
                case 7:
                case 8:
                    t_merge_522a_0001 = ppart->hs.iItem == ispecialMManeuveringJet ? 4 : 2;
                    c = _wsprintf(szWork, PszGetCompressedString(idsIncreasesSpeedBattle1DSquareMovement), t_merge_522a_0001);
                    goto PrintSpecial;
                case 5:
                case 6:
                    t_merge_526a_0001 = ppart->hs.iItem == ispecialMFuelTank ? 250 : 500;
                    c = _wsprintf(szWork, PszGetCompressedString(idsPodIncreasesFuelCapacityShipDmg), t_merge_526a_0001);
                    goto PrintSpecial;
                case 2:
                case 3:
                    t_merge_52aa_0001 = ppart->hs.iItem == ispecialMCargoPod ? 50 : 100;
                    c = _wsprintf(szWork, PszGetCompressedString(idsPodIncreasesCargoCapacityShipDkt), t_merge_52aa_0001);
                    goto PrintSpecial;
                case 4:
                    idsT = 214;
                    ids = idsOriginPartUnknown;
                    goto L_5336;
                case 9:
                    idsT = 217;
                    ids = idsOriginPartUnknown;
                    goto L_5336;
                case 0:
                    idsT = 215;
                    ids = idsPartAvailableAlternateRealityRaces;
                    goto L_5336;
                case 1:
                    idsT = 216;
                    ids = idsPartRequiresPrimaryRacialTraitAlternateReality;
                    goto L_5336;
                case 10:
                    idsT = 218;
                    goto L_5336;
                }
                break;
            }
            idsT = -1;
        L_5336:
            if (idsT == -1)
                break;
            c = CchGetString(idsT, szWork);
            goto PrintSpecial;
        case hstSpecialSB:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem <= ispecialSBUltraDriver13) {
                switch (ppart->hs.iItem) {
                case 1:
                case 4:
                case 5:
                case 6:
                    ids = idsStargateRequiresPrimaryRacialTraitInterstellarTr;
                    idsT = 736;
                    break;
                case 0:
                case 2:
                case 3:
                    ids = idsStargatesAvailableIfPrimaryRaceTraitHyper;
                    idsT = 736;
                    break;
                case 7:
                case 8:
                case 10:
                case 11:
                case 13:
                case 14:
                case 15:
                    ids = idsMassDriverRequiresPrimaryRacialTraitPacket;
                case 9:
                case 12:
                    idsT = 737;
                }
            } else {
                idsT = -1;
            }
            c = CchGetString(idsT, szWork);
            WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
            xText = rcData.left;
            yText += (int16_t)(3 * dyArial8) / 2;
            if (idsT != 736) {
                if (idsT == 737) {
                    SelectObject(hdc, rghfontArial8[1]);
                    DxStreamTextOut(hdc, &xText, yText, PszGetCompressedString(idsWarp), 0, 1);
                    SelectObject(hdc, rghfontArial8[0]);
                    c = _wsprintf(szWork, PCTD, ppart->pspecialsb->grAbility);
                    DxStreamTextOut(hdc, &xText, yText, szWork, c, 1);
                    xText = rcData.left;
                    yText += dyArial8;
                    idsT = 1111;
                    if (ppart->hs.iItem > ispecialSBMassDriver5) {
                        idsT++;
                    }
                    c = CchGetString(idsT, szWork);
                }
            } else {
                SelectObject(hdc, rghfontArial8[1]);
                DxStreamTextOut(hdc, &xText, yText, PszGetCompressedString(idsSafeHullMass), 0, 1);
                SelectObject(hdc, rghfontArial8[0]);
                if (ppart->pspecialsb->grAbility == -1) {
                    c = CchGetString(idsUnlimited, szWork);
                } else {
                    c = _wsprintf(szWork, PCTDKT, ppart->pspecialsb->grAbility);
                }
                DxStreamTextOut(hdc, &xText, yText, szWork, c, 1);
                xText = rcData.left;
                yText += dyArial8;
                SelectObject(hdc, rghfontArial8[1]);
                DxStreamTextOut(hdc, &xText, yText, PszGetCompressedString(idsSafeRange), 0, 1);
                SelectObject(hdc, rghfontArial8[0]);
                if (ppart->pspecialsb->grAbility2 == -1) {
                    c = CchGetString(idsUnlimited, szWork);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsDLightYears), ppart->pspecialsb->grAbility2);
                }
                DxStreamTextOut(hdc, &xText, yText, szWork, c, 1);
                if (ppart->pspecialsb->grAbility == -1) {
                    if (ppart->pspecialsb->grAbility2 == -1) {
                        idsT = -1;
                    } else {
                        c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsCanSuccessfullyGatedDL), 5 * ppart->pspecialsb->grAbility2);
                    }
                } else if (ppart->pspecialsb->grAbility2 == -1) {
                    c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsDktCanSuccessfullyGatedExceeding), 5 * ppart->pspecialsb->grAbility);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsDktMightSuccessfullyGatedD), 5 * ppart->pspecialsb->grAbility,
                                  5 * ppart->pspecialsb->grAbility2);
                }
            }
            if (idsT == -1)
                break;
            xText = rcData.left;
            yText += (dyArial8 > 14 ? 0 : 4) + dyArial8;
            SelectObject(hdc, rghfontArial7[0]);
            SetTextColor(hdc, 127);
            WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
            SelectObject(hdc, rghfontArial8[0]);
            SetTextColor(hdc, 0);
            break;
        case hstMines:
            dxLabel = -1;
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            rcData.top += 5 * dyArial8;
            for (i = 4; i >= 0; i--) {
                rcData.top -= dyArial8;
                c = CchGetString(i + 726, szWork);
                if (dxLabel == -1) {
                    dxLabel = LOWORD(GetTextExtent(hdc, szWork, c));
                }
                RightTextOut(hdc, rcData.left + dxLabel, rcData.top, szWork, c, 0);
            }
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem <= iminesSpeedTrap50) {
                switch (ppart->hs.iItem) {
                case 0:
                case 1:
                case 2:
                case 3:
                    iWarp = 4;
                    pctHit = 3;
                    dmgShip = 100;
                    dmgShipRam = 125;
                    dmgMin = 500;
                    dmgMinRam = 600;
                    if (ppart->hs.iItem != iminesMineDispenser50) {
                        ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
                        break;
                    }
                    ids = idsPartUnavailbleWarMonger;
                    break;
                case 4:
                case 5:
                case 6:
                    iWarp = 6;
                    pctHit = 10;
                    dmgShip = 500;
                    dmgShipRam = 600;
                    dmgMin = 2000;
                    dmgMinRam = 2500;
                    ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
                    break;
                case 7:
                case 8:
                case 9:
                    iWarp = 5;
                    pctHit = 35;
                    dmgShip = 0;
                    dmgShipRam = 0;
                    dmgMin = 0;
                    dmgMinRam = 0;
                    if (ppart->hs.iItem != iminesSpeedTrap20) {
                        ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
                    } else {
                        ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition2;
                    }
                }
            }
            c = _wsprintf(szWork, PCTD, 10 * ppart->pmines->grAbility);
            TextOut(hdc, rcData.left + dxLabel + 4, rcData.top, szWork, c);
            rcData.top += dyArial8;
            c = _wsprintf(szWork, PszGetCompressedString(idsWarpD2), iWarp);
            TextOut(hdc, rcData.left + dxLabel + 4, rcData.top, szWork, c);
            rcData.top += dyArial8;
            c = _wsprintf(szWork, PszGetCompressedString(idsDD2), pctHit / 10, pctHit % 10);
            TextOut(hdc, rcData.left + dxLabel + 4, rcData.top, szWork, c);
            rcData.top += dyArial8;
            c = _wsprintf(szWork, PszGetCompressedString(idsDDEngine), dmgShip, dmgShipRam);
            TextOut(hdc, rcData.left + dxLabel + 4, rcData.top, szWork, c);
            rcData.top += dyArial8;
            c = _wsprintf(szWork, PszGetCompressedString(idsDD3), dmgMin, dmgMinRam);
            TextOut(hdc, rcData.left + dxLabel + 4, rcData.top, szWork, c);
            rcData.top += dyArial8;
            SelectObject(hdc, rghfontArial7[0]);
            c = CchGetString(idsNumbersParenthesisFleetsContainingShipRamScoop, szWork);
            DrawText(hdc, szWork, c, &rcData, 2064);
            break;
        case hstMining:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            if (ppart->hs.iItem > iminingOrbitalAdjuster)
                break;
            switch (ppart->hs.iItem) {
            case 0:
            case 5:
                ids = idsRobotMinerRequiresLesserRacialTraitAdvanced;
            case 2:
            case 3:
            case 4:
                if (ids == 0xffff) {
                    ids = idsRobotMinerWillAvailableIfLesserRacial;
                }
            case 1:
            case 6:
                c = CchGetString(idsModuleContainsRobotsCapableMining, szWork);
                c += _wsprintf(&szWork[c], PCTD, ppart->pmining->grAbility);
                c += CchGetString(idsKtEachMineralDependingConcentrationUninhabitedPl, &szWork[c]);
                if (ppart->hs.iItem != iminingAlienMiner)
                    break;
                ids = idsOriginPartUnknown;
                c += CchGetString(idsModuleAlsoActs30Cloak30Jammer, &szWork[c]);
                break;
            case 7:
                ids = idsPartRequiresPrimaryRacialTraitClaimAdjuster;
                c = CchGetString(idsModifiedMiningRobotTerraformsInhabitedPlanets1, szWork);
            }
            goto PrintSpecial;
        case hstTerra:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            c = 0;
            if (ppart->hs.iItem <= iterraTotalTerraform30) {
                ids = idsTotalTerraformingRequiresLesserRacialTraitTotal;
                c = CchGetString(idsAllowsModifyAnyPlanetsThreeEnvironmentVariables, szT);
                c = _wsprintf(szWork, szT, ppart->pterra->grAbility);
            } else {
                c = CchGetString(idsAllowsModifyPlanetsSDOriginalValue, szT);
                c = _wsprintf(szWork, szT, rgszPlanetAttr[(int16_t)(ppart->hs.iItem - 8) / 4], ppart->pterra->grAbility);
            }
            if (c <= 0)
                break;
            WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
            break;
        case hstPlanetary:
            ExpandRc(&rcData, -4, -4);
            SelectObject(hdc, rghfontArial8[1]);
            xText = rcData.left;
            yText = rcData.top;
            SelectObject(hdc, rghfontArial8[0]);
            c = 0;
            if (ppart->hs.iItem >= iplanetarySDI && ppart->hs.iItem <= iplanetaryNeutronShield) {
                SelectObject(hdc, rghfontArial8[1]);
                rcData.right -= 2;
                c = CchGetString(idsShieldCoverageVsDefenseQuan, szWork);
                CtrTextOut(hdc, ((rcData.right - rcData.left) >> 1) + rcData.left, rcData.top, szWork, c);
                rcData.top += dyArial8;
                cch = LOBYTE(CchGetString(idsNum, szT));
                dxStr = LOWORD(GetTextExtent(hdc, szT, (int16_t)(int8_t)cch));
                dxQuan = (int16_t)(rcData.right - rcData.left - dxStr - 10) / 5;
                dyPct = (int16_t)(rcData.bottom - rcData.top - dyArial8 - 8) / 5;
                PatBlt(hdc, rcData.left + dxStr + 6, rcData.top, 1, rcData.bottom - rcData.top - dyArial8 - 4, BLACKNESS);
                PatBlt(hdc, rcData.left + dxStr + 6, rcData.bottom - dyArial8 - 4, rcData.right - rcData.left - dxStr - 6, 1, BLACKNESS);
                x = rcData.left + dxStr;
                y = rcData.bottom - dyArial8 - 4 - 5 * dyPct;
                SetTextColor(hdc, 8323072);
                cch = LOBYTE(CchGetString(idsStandard, szWork));
                RightTextOut(hdc, rcData.right, rcData.bottom - 3 - 3 * dyArial8, szWork, (int16_t)(int8_t)cch, 0);
                SetTextColor(hdc, 127);
                cch = LOBYTE(CchGetString(idsSmart, szWork));
                RightTextOut(hdc, rcData.right, rcData.bottom - 4 - dyArial8 * 2, szWork, (int16_t)(int8_t)cch, 0);
                pct = 100;
                SetTextColor(hdc, 0);
                while (pct > 0) {
                    c = _wsprintf(szWork, PCTDPCTPCT, pct);
                    RightTextOut(hdc, x, y - (dyArial8 >> 1), szWork, c, 0);
                    PatBlt(hdc, x + 4, y, 5, 1, BLACKNESS);
                    pct -= 20;
                    y += dyPct;
                }
                y = rcData.bottom - dyArial8;
                RightTextOut(hdc, x, y, szT, strlen(szT), 0);
                x += 6;
                dxDigit = LOWORD(GetTextExtent(hdc, "0", 1));
                ch = '0';
                for (i = 0; i <= 5; i++) {
                    cch = LOBYTE(_wsprintf(szWork, PCTD, 20 * i));
                    t_merge_6105_0001 = i == 0 ? dxDigit >> 1 : i == 5 ? (int16_t)(3 * dxDigit) / 2 : dxDigit;
                    TextOut(hdc, x - t_merge_6105_0001, y, szWork, (int16_t)(int8_t)cch);
                    if (i > 0) {
                        PatBlt(hdc, x, y - 6, 1, 5, BLACKNESS);
                    }
                    ch++;
                    x += dxQuan;
                }
                hpenSav = SelectObject(hdc, hpenDkBlue);
                xBase = rcData.left + dxStr + 6;
                yBase = rcData.bottom - dyArial8 - 4;
                ldelta = (int16_t)(1000 - ppart->pplanetary->grAbility);
                for (c = 0; c < 2; c++) {
                    lpct = 1000000;
                    for (i = 0; i <= 100; i++) {
                        x = MulDiv(i, dxQuan, 20) + xBase;
                        y = yBase;
                        y -= LOWORD((int32_t)((1000000 - lpct) * dyPct) / 0x30d40);
                        lpct = (int32_t)(lpct * ldelta) / 1000;
                        if (i == 0) {
                            MoveTo(hdc, x, y);
                        } else {
                            LineTo(hdc, x, y);
                        }
                    }
                    ldelta = (int16_t)(1000 - ppart->pplanetary->grAbility / 2);
                    SelectObject(hdc, hpenRadar);
                }
                SelectObject(hdc, hpenSav);
                c = 0;
                if (ppart->hs.iItem <= iplanetaryMissileBattery) {
                    ids = idsPlanetaryScannersDefensesAvailableAlternateReali;
                } else {
                    ids = idsPlanetaryDefenseUnavailablePrimaryRacialTraitWar;
                }
            } else if (ppart->hs.iItem >= iplanetaryViewer50 && ppart->hs.iItem <= iplanetarySnooper620X) {
                i = ppart->pplanetary->grAbility;
                c = CchGetString(idsEnemyFleetsOrbitingPlanetCanDetectedD, szT);
                c = _wsprintf(szWork, szT, abs(i));
                if (i < 0) {
                    WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
                    xText = rcData.left;
                    yText += (int16_t)(3 * dyArial8) / 2;
                    c = CchGetString(idsScannerCanDeterminePlanetsBasicStatsDistance, szT);
                    c = _wsprintf(szWork, szT, -i >> 1);
                    ids = idsScannerWillUnavailableIfHaveLesserRacial;
                } else {
                    ids = idsPlanetaryScannersDefensesAvailableAlternateReali;
                }
            } else if (ppart->hs.iItem == iplanetaryGenesisDevice) {
                ids = idsOriginProcessUnknown;
                c = CchGetString(idsProcessGivesPlanetNewBirthTracesCivilization, szWork);
            }
            if (c > 0) {
                WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
            }
        }
        goto L_64d4;
    PrintSpecial:
        WrapTextOut(hdc, &xText, &yText, szWork, c, rcData.left, rcData.right - rcData.left, NULL, 0, 1);
        xText = rcData.left;
        yText += dyArial8 * 2;
    L_64d4:
        if (ids != 0xffff) {
            c = CchGetString(ids, szWork);
            SetRect(&rcT, 4, dy - dyArial8 * 2, dx - 4, dy - 5);
            if (dyArial8 > 14) {
                SelectObject(hdc, rghfontArial6[0]);
            } else {
                SelectObject(hdc, rghfontArial7[0]);
            }
            t_merge_6561_0001 = l == -1 ? 127 : 0;
            SetTextColor(hdc, t_merge_6561_0001);
            DrawText(hdc, szWork, c, &rcT, 2064);
            if (l == -1) {
                SetTextColor(hdc, 0);
            }
        }
    }
    return;
}

int32_t ProjectedResearchSpending(int32_t pct) {
    int32_t lRes;
    PLANET *lppl;
    int16_t cRes;
    int32_t lSpend;
    PLANET *lpplMac;
    char    pctSav;
    int16_t cBogus;

    lSpend = 0;
    pctSav = rgplr[idPlayer].pctResearch;
    rgplr[idPlayer].pctResearch = LOBYTE(LOWORD(pct));
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == idPlayer) {
            lRes = CResourcesAtPlanet(lppl, idPlayer);
            if (lppl->lpplprod == 0 || lppl->lpplprod->iprodMac == 0) {
                lSpend += lRes;
            } else {
                EstimateItemProdSched(lppl, NULL, 0xffff, &cRes, &cBogus);
                lSpend += cRes;
            }
        }
    }
    rgplr[idPlayer].pctResearch = pctSav;
    return lSpend;
}

int32_t CostOfDevelopingItem(char *rgTech) {
    int32_t  lSpent;
    char    *pTech;
    char     rgTechSav[6];
    int32_t  lCost;
    int16_t  fUnreachable;
    int16_t  i;
    int32_t  lCur;
    uint16_t t_merge_6879_0001;

    fUnreachable = 0;
    lCost = 0;
    pTech = rgplr[idPlayer].rgTech;
    for (i = 0; i < 6 && (int16_t)(int8_t)rgTech[i] <= 26; i++) {
    }
    if (i < 6) {
        return -1;
    }
    for (i = 0; i < 6; i++) {
        rgTechSav[i] = pTech[i];
        if ((int16_t)(int8_t)rgTech[i] > (int16_t)(int8_t)pTech[i]) {
            lSpent = rgplr[idPlayer].rgResSpent[i];
            if (game.fSlowTech != 0) {
                lSpent = (int32_t)(lSpent * 2);
            }
            lCur = -lSpent;
            while ((int16_t)(int8_t)rgTech[i] > (int16_t)(int8_t)pTech[i]) {
                lCur += GetTechLevelCost(i, (int16_t)(int8_t)pTech[i] + 1, idPlayer);
                pTech[i]++;
            }
            t_merge_6879_0001 = 0 <= lCur ? lCur : 0;
            lCost += t_merge_6879_0001;
        }
    }
    for (i = 0; i < 6; i++) {
        pTech[i] = rgTechSav[i];
    }
    return lCost;
}

int16_t FShouldPartBeHidden(PART *ppart) {
    int16_t  iItem;
    uint16_t grbitTrader;

    if (idPlayer == -1) {
        return 0;
    }
    grbitTrader = 0;
    iItem = ppart->hs.iItem;
    switch (ppart->hs.grhst) {
    case hstBeam:
        if (iItem != 18)
            break;
        grbitTrader = 128;
        break;
    case hstTorp:
        if (iItem != 7)
            break;
        grbitTrader = 64;
        break;
    case hstArmor:
        if (iItem != 9)
            break;
        grbitTrader = 8;
        break;
    case hstShield:
        if (iItem != 6)
            break;
        grbitTrader = 4;
        break;
    case hstBomb:
        if (iItem != 8)
            break;
        grbitTrader = 32;
        break;
    case hstMining:
        if (iItem != 6)
            break;
        grbitTrader = 16;
        break;
    case hstEngine:
        if (iItem != 8)
            break;
        grbitTrader = 0x200;
        break;
    case hstHull:
        if (iItem != 30)
            break;
        grbitTrader = 0x100;
        break;
    case hstSpecialE:
        if (iItem != 4)
            break;
        grbitTrader = 2;
        break;
    case hstSpecialM:
        if (iItem == 4) {
            grbitTrader = 1;
            break;
        }
        if (iItem != 9)
            break;
        grbitTrader = 0x800;
        break;
    case hstPlanetary:
        if (iItem == 14) {
            grbitTrader = 0x400;
        }
    }
    if (grbitTrader != 0 && (rgplr[idPlayer].grbitTrader & grbitTrader) == 0) {
        return 1;
    }
    return 0;
}
