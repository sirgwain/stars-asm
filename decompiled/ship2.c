#include "common.h"

INT_PTR CALLBACK ZipOrderDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC            hdc;
    int16_t        i;
    PAINTSTRUCT    ps;
    RECT           rc;
    HWND           hwndRad;
    char          *psz;
    char          *pszT;
    RECT           rcGBox;
    int16_t        cch;
    int16_t        xCtr;
    XferActionType iAction;
    FARPROC        lpProc;
    char          *t_00bd;
    char          *t_00c6;
    char          *t_00d8;
    HWND           t_scratch_m30;
    char          *t_065c;
    char          *t_0665;
    char          *t_0677;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        GetWindowRect(GetDlgItem(hwnd, 0x431), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0434), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 0x1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        cch = CchGetString(idsCustomOrders, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, cch);
        rcGBox.top = rcGBox.bottom + 8;
        if (vrgZip[iResTechNow].fValid == 0x0) {
            cch = CchGetString(idsEmptyCustomSlot, szWork);
            TextOut(hdc, 12, rcGBox.top, szWork, cch);
        } else {
            xCtr = LOWORD(GetTextExtent(hdc, rgszMinerals[2], 9)) + 8;
            for (i = 0; i < 5; i++) {
                SetTextColor(hdc, rgcrMinerals[i]);
                RightTextOut(hdc, xCtr, rcGBox.top, rgszMinerals[i], 0, 0);
                SetTextColor(hdc, 0x0);
                iAction = vrgZip[iResTechNow].txp.rgia[i].iAction;
                cch = CchGetString(iAction + 109, szWork);
                if ((int16_t)szWork[cch - 1] == '.') {
                    _wsprintf(&szWork[cch - 3], " %dkT", vrgZip[iResTechNow].txp.rgia[i].cQuan);
                }
                TextOut(hdc, xCtr + 6, rcGBox.top, szWork, strlen(szWork));
                rcGBox.top = rcGBox.top + dyArial8;
            }
        }
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 1073; i <= 1076; i++) {
            t_scratch_m30 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m30 == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 1076) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            SetWindowText(hwnd, PszGetCompressedString(idsCustomizeZipOrders));
            ShowWindow(GetDlgItem(hwnd, IDC_U16_0x0417), SW_HIDE);
            hwndZipOrderDlg = hwnd;
            CheckRadioButton(hwnd, 1073, 1076, 1073);
            EnableZipBtns(hwnd, 0);
            iResTechNow = 0;
            for (i = 1073; i <= 1076; i++) {
                if (vrgZip[i - 1073].fValid == 0x0) {
                    psz = PszGetCompressedString(idsUnusedD);
                    _wsprintf(szWork, psz, i - 1072);
                    psz = szWork;
                } else {
                    pszT = szWork;
                    psz = vrgZip[i - 1073].szName;
                    while ((int16_t)*psz != 0) {
                        t_00bd = psz;
                        psz = psz + 1;
                        t_00c6 = pszT;
                        pszT = pszT + 1;
                        *t_00c6 = *t_00bd;
                        if ((int16_t)*t_00bd == '&') {
                            t_00d8 = pszT;
                            pszT = pszT + 1;
                            *t_00d8 = '&';
                        }
                    }
                    *pszT = 0;
                    psz = szWork;
                }
                hwndRad = GetDlgItem(hwnd, i);
                SetWindowText(hwndRad, psz);
            }
            StickyDlgPos(hwnd, &ptStickyZipOrderDlg, 1);
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        }
        if (message == WM_COMMAND) {
            if (GET_WM_COMMAND_CMD(wParam, lParam) != 0x0 || GET_WM_COMMAND_ID(wParam, lParam) < 0x431 || GET_WM_COMMAND_ID(wParam, lParam) > IDC_U16_0x0434) {
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                case IDCANCEL:
                    hwndZipOrderDlg = 0x0;
                    StickyDlgPos(hwnd, &ptStickyZipOrderDlg, 0);
                    EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                    if (gd.fTutorial != 0x0) {
                        AdvanceTutor();
                    }
                    return 1;
                case IDC_IMPORT:
                case IDC_RENAME:
                    if (vrgZip[iResTechNow].fValid == 0x0) {
                        _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                    } else {
                        strcpy(szWork, vrgZip[iResTechNow].szName);
                    }
                    lpProc = MakeProcInstance(RenameZipDlg, hInst);
                    if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc) != 0) {
                        if ((int16_t)szWork[0] == 0) {
                            _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                        }
                        strcpy(vrgZip[iResTechNow].szName, szWork);
                        pszT = &szWork[64];
                        psz = szWork;
                        while ((int16_t)*psz != 0) {
                            t_065c = psz;
                            psz = psz + 1;
                            t_0665 = pszT;
                            pszT = pszT + 1;
                            *t_0665 = *t_065c;
                            if ((int16_t)*t_065c == '&') {
                                t_0677 = pszT;
                                pszT = pszT + 1;
                                *t_0677 = '&';
                            }
                        }
                        *pszT = 0;
                        SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), &szWork[64]);
                        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_IMPORT) {
                            vrgZip[iResTechNow].fValid = 0x1;
                            vrgZip[iResTechNow].txp = sel.fl.lpplord->rgord[sel.iwpAct].txp;
                            InvalidateRect(hwnd, 0x0, 1);
                        }
                        EnableZipBtns(hwnd, iResTechNow);
                    }
                    FreeProcInstance(lpProc);
                    SetFocus(hwnd);
                    gd.fChgZipOrd = 0x1;
                    break;
                case IDC_DELETE:
                    vrgZip[iResTechNow].fValid = 0x0;
                    _wsprintf(szWork, PszGetCompressedString(idsUnusedD), iResTechNow + 1);
                    SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), szWork);
                    InvalidateRect(hwnd, 0x0, 1);
                    gd.fChgZipOrd = 0x1;
                    break;
                case IDC_HELP:
                    WinHelp(hwnd, szHelpFile, 0x1, 0x44a);
                    return 1;
                default:
                }
            } else {
                iResTechNow = GET_WM_COMMAND_ID(wParam, lParam) - 1073;
                EnableZipBtns(hwnd, iResTechNow);
                InvalidateRect(hwnd, 0x0, 1);
            }
        }
    }
    return 0;
}

void EnableZipBtns(HWND hwnd, int16_t iSel) {
    int16_t fEnabled;

    fEnabled = vrgZip[iSel].fValid;
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), fEnabled);
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), fEnabled);
    return;
}

INT_PTR CALLBACK RenameZipDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    StringId ids;
    RECT     rc;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        if (HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            if (hwndZipOrderDlg == 0x0) {
                ids = idsRenameProductionTemplate;
            } else {
                ids = idsRenameZipOrder;
            }
            SetWindowText(hwnd, PszGetCompressedString(ids));
            SetWindowPos(hwnd, 0x0, ptStickyRenameDlg.x + 70, ptStickyRenameDlg.y + 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0xc, 0);
            SetWindowText(GetDlgItem(hwnd, IDC_EDIT1), szWork);
            StickyDlgPos(hwnd, &ptStickyRenameDlg, 1);
            return 1;
        }
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                    GetDlgItemText(hwnd, IDC_EDIT1, szWork, 14);
                }
                StickyDlgPos(hwnd, &ptStickyRenameDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, (uint32_t)(hwndZipOrderDlg == 0x0 ? 0x452 : 0xc1f));
                return 1;
            default:
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK RenameDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT    rc;
    int32_t lSel;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        if (HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            SetWindowText(hwnd, PszGetCompressedString(idsRenameFleet));
            SetWindowPos(hwnd, 0x0, ptStickyRenameDlg.x + 70, ptStickyRenameDlg.y + 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0x1f, 0);
            SetWindowText(GetDlgItem(hwnd, IDC_EDIT1), szWork);
            StickyDlgPos(hwnd, &ptStickyRenameDlg, 1);
            return 1;
        }
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                    GetDlgItemText(hwnd, IDC_EDIT1, szWork, 32);
                    FStringFitsScreen(szWork, 160);
                }
                StickyDlgPos(hwnd, &ptStickyRenameDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                return 1;
            case IDC_EDIT1:
                if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x400 && fInEditUpdate == 0) {
                    fInEditUpdate = 1;
                    GetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), szWork, 250);
                    lSel = SendMessage(GET_WM_COMMAND_HWND(wParam, lParam), EM_GETSEL, 0x0, 0);
                    if (FStringFitsScreen(szWork, 160) == 0) {
                        SetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), szWork);
                        SendMessage(GET_WM_COMMAND_HWND(wParam, lParam), EM_SETSEL, LOWORD(lSel), (int16_t)HIWORD(lSel));
                    }
                    fInEditUpdate = 0;
                    break;
                }
            default:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                    WinHelp(hwnd, szHelpFile, 0x1, 0x447);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int16_t FStargateJump(FLEET *lpfl, int16_t isbsSrc, int16_t isbsDst, int16_t dDist) {
    int16_t   dpPerShdefNew;
    int16_t   dpShdef;
    POINT16   pt;
    int16_t   id;
    FLEET     flSrc;
    int16_t   cshT;
    uint8_t   pctKill;
    int16_t   i;
    int32_t   cshOrig;
    MessageId idm;
    int32_t   cshKill;
    int16_t   pct;
    int16_t   rgpct[16];
    int16_t   cshdef;
    int16_t   ishdef;
    int32_t   dp;
    int16_t   dpPerShdefOld;
    int16_t   cshDamagedOld;
    FLEET     flDead;
    int16_t   t_call_0e31;
    int16_t   t_scratch_m144;

    cshdef = 0;
    cshKill = 0;
    cshOrig = 0;
    memset(rgpct, 0, 0x20);
    flSrc = *lpfl;
    if (flSrc.lpplord->rgord[1].grobj != grobjPlanet) {
        pt = flSrc.lpplord->rgord[1].pt;
        for (id = 0; id < game.cPlanMax && (pt.x != rgptPlan[id].x || pt.y != rgptPlan[id].y); id++) {
        }
    } else {
        id = flSrc.lpplord->rgord[1].id;
    }
    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (flSrc.rgcsh[ishdef] != 0) {
            cshdef = cshdef + 1;
            cshOrig = cshOrig + (int32_t)flSrc.rgcsh[ishdef];
            t_call_0e31 = MdCalcStargateDamage(isbsSrc, isbsDst, dDist, rglpshdef[flSrc.iPlayer][ishdef].hul.wtEmpty, &rgpct[ishdef]);
            switch (t_call_0e31) {
            case -1:
                goto L_0e3c;
            case -2:
                goto L_0e75;
            case 0:
                cshdef = cshdef - 1;
                break;
            case 1:
                flSrc.fNoHeal = 0x1;
            default:
            }
        }
    }
    if (cshdef != 0) {
        memset(&flDead, 0, sizeof(FLEET));
        for (ishdef = 0; ishdef < 16; ishdef++) {
            if (flSrc.rgcsh[ishdef] != 0 && rgpct[ishdef] != 0) {
                if (rgpct[ishdef] != 100) {
                    cshT = flSrc.rgcsh[ishdef];
                    if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) != raStargate) {
                        pctKill = LOBYTE((int32_t)rgpct[ishdef] / 3);
                    } else {
                        pctKill = 0x0;
                    }
                    dpShdef = rglpshdef[lpfl->iPlayer][ishdef].hul.dp;
                    if (flSrc.rgdv[ishdef].dp == 0x0) {
                        cshDamagedOld = 0;
                    } else {
                        cshDamagedOld = LOWORD((int32_t)((int32_t)((int32_t)cshT * (int32_t)flSrc.rgdv[ishdef].pctSh) / 0x64));
                        if (cshDamagedOld == 0) {
                            cshDamagedOld = 1;
                        }
                    }
                    if (pctKill > 0x0) {
                        for (i = 0; i < flSrc.rgcsh[ishdef]; i++) {
                            t_scratch_m144 = Random(100);
                            if (t_scratch_m144 < pctKill) {
                                cshT = cshT - 1;
                                if (cshDamagedOld != 0 && (uint16_t)Random(500) < flSrc.rgdv[ishdef].pctDp) {
                                    cshDamagedOld = cshDamagedOld - 1;
                                }
                            }
                        }
                        cshKill = cshKill + (int32_t)(flSrc.rgcsh[ishdef] - cshT);
                    }
                    if (cshT != 0) {
                        if (flSrc.rgdv[ishdef].dp == 0x0) {
                            dpPerShdefOld = 0;
                        } else {
                            dpPerShdefOld = LOWORD((int32_t)((int32_t)((int32_t)dpShdef * (int32_t)flSrc.rgdv[ishdef].pctDp) / 0x1f4));
                            if (dpPerShdefOld == 0) {
                                dpPerShdefOld = 1;
                            }
                        }
                        dpPerShdefNew = LOWORD((int32_t)((int32_t)((int32_t)dpShdef * (int32_t)rgpct[ishdef]) / 0x64));
                        if (dpPerShdefNew == 0) {
                            dpPerShdefNew = 1;
                        }
                        if (cshDamagedOld != 0 && dpPerShdefNew + dpPerShdefOld >= dpShdef) {
                            cshKill = cshKill + (int32_t)cshDamagedOld;
                            cshT = cshT - cshDamagedOld;
                        }
                        if (cshT != 0) {
                            dp = (uint32_t)((int32_t)dpPerShdefNew * (int32_t)cshT) + (uint32_t)((int32_t)dpPerShdefOld * (int32_t)cshDamagedOld);
                            pct = LOWORD((int32_t)((int32_t)((int32_t)(dp / (int32_t)cshT) * 500) / (int32_t)dpShdef));
                            if (pct == 0) {
                                pct = 1;
                            }
                            flSrc.rgdv[ishdef].pctDp = pct;
                            flSrc.rgdv[ishdef].pctSh = 0x64;
                        }
                    }
                    flSrc.rgcsh[ishdef] = cshT;
                    if (cshT == 0) {
                        flSrc.rgdv[ishdef].dp = 0x0;
                        cshdef = cshdef - 1;
                    }
                } else {
                    cshKill = cshKill + (int32_t)flSrc.rgcsh[ishdef];
                    flSrc.rgcsh[ishdef] = 0;
                    flSrc.rgdv[ishdef].dp = 0x0;
                    cshdef = cshdef - 1;
                }
                flDead.rgcsh[ishdef] = lpfl->rgcsh[ishdef] - flSrc.rgcsh[ishdef];
            }
        }
        if (cshdef != 0) {
            if (cshKill != 0) {
                if ((cshKill & 0xffff0000) != 0x0) {
                    FSendPlrMsg(lpfl->iPlayer, 235, lpfl->id | 0x8000, lpfl->id, lpfl->idPlanet, id, LOWORD(cshKill), HIWORD(cshKill), 0, 0);
                } else {
                    if (cshKill < (int32_t)(cshOrig >> 0x2)) {
                        idm = idmUsedStargateReachLosingShipsTreacherousVoid;
                    } else if (cshKill <= (int32_t)(cshOrig >> 0x1)) {
                        idm = idmUsedStargateReachLosingShipsUnforgivingVoid;
                    } else {
                        idm = idmUsedStargateReachUnfortunatelyLosingShipsGreat;
                    }
                    FSendPlrMsg(lpfl->iPlayer, idm, lpfl->id | 0x8000, lpfl->id, lpfl->idPlanet, id, LOWORD(cshKill), 0, 0, 0);
                }
                flDead.iPlayer = flSrc.iPlayer;
                flDead.fDead = 0x1;
                flDead.det = 0x7;
                FleetTransferCargoBalance(&flSrc, &flDead);
            }
            *lpfl = flSrc;
            return 1;
        }
    }
    lpfl->fDead = 0x1;
    FSendPlrMsg(flSrc.iPlayer, 231, flSrc.id | 0x8000, flSrc.id, flSrc.idPlanet, id, 0, 0, 0, 0);
    return 0;
L_0e3c:
    FSendPlrMsg(flSrc.iPlayer, 227, flSrc.id | 0x8000, flSrc.id, flSrc.idPlanet, id, 0, 0, 0, 0);
    return 0;
L_0e75:
    FSendPlrMsg(flSrc.iPlayer, 228, flSrc.id | 0x8000, flSrc.id, flSrc.idPlanet, id, ishdef, 0, 0, 0);
    return 0;
}

int16_t MdCalcStargateDamage(int16_t isbsSrc, int16_t isbsDst, int16_t dDist, int16_t wt, int16_t *ppctDmg) {
    int32_t dBaseDistance;
    PART    partDst;
    PART    partSrc;
    int32_t pctSurviveT;
    int32_t pctSurvive;

    pctSurvive = 10000;
    partDst.hs.grhst = hstSpecialSB;
    partSrc.hs.grhst = hstSpecialSB;
    partSrc.hs.iItem = isbsSrc;
    partDst.hs.iItem = isbsDst;
    FLookupPart(&partSrc);
    FLookupPart(&partDst);
    dBaseDistance = (int32_t)partSrc.pspecialsb->grAbility2;
    if (dBaseDistance == -1) {
        dBaseDistance = 8000;
    }
    if ((int32_t)(int32_t)dDist <= (int32_t)(uint32_t)(dBaseDistance * 5)) {
        if ((partSrc.pspecialsb->grAbility <= 0 || (int32_t)(int32_t)wt <= (int32_t)(uint32_t)((int32_t)partSrc.pspecialsb->grAbility * 5)) &&
            (partDst.pspecialsb->grAbility <= 0 || (int32_t)(int32_t)wt <= (int32_t)(uint32_t)((int32_t)partDst.pspecialsb->grAbility * 5))) {
            if ((int32_t)dDist > dBaseDistance) {
                pctSurvive = (int32_t)((int32_t)(((uint32_t)(dBaseDistance * 5) - (int32_t)dDist) * 0x9c4) / dBaseDistance);
                if (pctSurvive <= 0)
                    goto TotalDeath;
            }
            if (wt > partSrc.pspecialsb->grAbility && partSrc.pspecialsb->grAbility > 0) {
                pctSurviveT = (int32_t)((int32_t)(((uint32_t)((int32_t)partSrc.pspecialsb->grAbility * 5) - (int32_t)wt) * 0x9c4) /
                                        (int32_t)partSrc.pspecialsb->grAbility);
                if (pctSurviveT <= 0)
                    goto TotalDeath;
                pctSurvive = (int32_t)((int32_t)(pctSurvive * pctSurviveT) / 10000);
            }
            if (wt > partDst.pspecialsb->grAbility && partDst.pspecialsb->grAbility > 0) {
                pctSurviveT = (int32_t)((int32_t)(((uint32_t)((int32_t)partDst.pspecialsb->grAbility * 5) - (int32_t)wt) * 0x9c4) /
                                        (int32_t)partDst.pspecialsb->grAbility);
                if (pctSurviveT <= 0)
                    goto TotalDeath;
                pctSurvive = (int32_t)((int32_t)(pctSurvive * pctSurviveT) / 10000);
            }
            *ppctDmg = LOWORD((int32_t)((10000 - pctSurvive) / 0x64));
            return 1;
        TotalDeath:
            *ppctDmg = 100;
            return 0;
        }
        return -2;
    }
    return -1;
}

void KillUsedWaypoints() {
    int16_t j;
    int16_t i;
    FLEET  *lpfl;
    int16_t fRep;
    PLANET *lppl;
    int16_t t_scratch_me;

    if (cFleet > 0) {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0x0)
                break;
            if (lpfl->cord > 1) {
                if (lpfl->fMark == 0x0) {
                    for (j = 1; j < lpfl->cord; j++) {
                        if (lpfl->lpplord->rgord[j].grobj == grobjFleet) {
                            lpfl->lpflNext = LpflFromId(lpfl->lpplord->rgord[j].id);
                            if (lpfl->lpflNext != 0x0) {
                                if (lpfl->lpplord->rgord[j].fNoAutoTrack == 0x0) {
                                    t_scratch_me = lpfl->lpflNext->pt.y;
                                    lpfl->lpplord->rgord[j].pt.x = lpfl->lpflNext->pt.x;
                                    lpfl->lpplord->rgord[j].pt.y = t_scratch_me;
                                }
                                lpfl->lpflNext = 0x0;
                            } else {
                                lpfl->lpplord->rgord[j].grobj = grobjOther;
                                lpfl->lpplord->rgord[j].id = 0;
                            }
                        }
                    }
                    if (lpfl->pt.x == lpfl->lpplord->rgord[1].pt.x && lpfl->pt.y == lpfl->lpplord->rgord[1].pt.y) {
                        lpfl->lpplord->rgord[0] = lpfl->lpplord->rgord[1];
                        if (lpfl->lpplord->rgord[0].grobj == grobjFleet && lpfl->lpplord->rgord[0].grTask != grTaskXfer &&
                            lpfl->lpplord->rgord[0].grTask != grTaskMerge) {
                            if (lpfl->idPlanet != -1) {
                                lpfl->lpplord->rgord[0].grobj = grobjPlanet;
                                lpfl->lpplord->rgord[0].id = lpfl->idPlanet;
                            } else {
                                lpfl->lpplord->rgord[0].grobj = grobjOther;
                                lpfl->lpplord->rgord[0].id = 0;
                            }
                        }
                        if (lpfl->lpplord->rgord[1].grTask == grTaskPatrol && lpfl->lpplord->rgord[1].grobj == grobjFleet) {
                            fRep = 0;
                        } else {
                            fRep = lpfl->fRepOrders;
                        }
                        DeleteWpFar(lpfl, 1, fRep);
                        if (lpfl->cord == 1) {
                            if (lpfl->lpplord->rgord[0].grTask - 1 <= 0x7) {
                                switch (lpfl->lpplord->rgord[0].grTask) {
                                case 1:
                                case 2:
                                case 3:
                                case 5:
                                case 6:
                                case 7:
                                    break;
                                case 8:
                                    if (lpfl->idPlanet != -1) {
                                        lppl = LpplFromId(lpfl->idPlanet);
                                        if (lppl->iPlayer == lpfl->iPlayer && lppl->idRoute != 0x0)
                                            break;
                                    }
                                case 4:
                                    goto L_1cc6;
                                }
                                continue;
                            }
                        L_1cc6:
                            FSendPlrMsg(lpfl->iPlayer, 78, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                        }
                    }
                } else {
                    DeleteWpFar(lpfl, 1, 0);
                    FSendPlrMsg(lpfl->iPlayer, 311, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                }
            }
        }
    }
    return;
}

void NoAutoTrackFleet(FLEET *lpflTarget) {
    int16_t iplr;
    int16_t idTarget;
    int16_t i;
    ORDER  *lpord;
    int16_t ifl;
    FLEET  *lpfl;

    iplr = lpflTarget->iPlayer;
    idTarget = lpflTarget->id;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer != iplr && lpfl->cord > 1) {
            lpord = &lpfl->lpplord->rgord[1];
            i = 1;
            while (i < lpfl->cord) {
                if (lpord->grobj == grobjFleet && lpord->id == idTarget) {
                    lpord->fNoAutoTrack = 0x1;
                    lpord->pt = lpflTarget->pt;
                }
                i = i + 1;
                lpord = lpord + 1;
            }
        }
    }
    return;
}

void AutoRouteFleet(FLEET *lpfl, PLANET *lppl) {
    int32_t dTravel;
    int16_t iWarp;
    int16_t pctDmg;
    int16_t wt;
    int32_t cTurns;
    int16_t i;
    ORDER  *lpord;
    PLANET *lpplRoute;
    int16_t isbsDst;
    int16_t wtBig;
    int16_t ishdef;
    int16_t ishdefBig;
    int16_t isbsSrc;

    lpfl->cord = 2;
    lpfl->lpplord->iordMac = 0x2;
    lpord = &lpfl->lpplord->rgord[1];
    lpord->grTask = grTaskAutoRoute;
    lpord->grobj = grobjPlanet;
    lpord->id = lppl->idRoute - 1;
    lpplRoute = LpplFromId(lppl->idRoute - 1);
    lpord->pt = rgptPlan[lppl->idRoute - 1];
    lpord->fValidTask = 0x1;
    iWarp = IFindIdealWarp(lpfl, 0);
    dTravel = (int32_t)(DGetDistance(lpfl->pt.x, lpfl->pt.y, lpord->pt.x, lpord->pt.y) + 0.999);
    if (lppl->iPlayer == lpplRoute->iPlayer && lppl->fStarbase != 0x0 && lpplRoute->fStarbase != 0x0) {
        isbsDst = IStargateFromLppl(lpplRoute);
        isbsSrc = IStargateFromLppl(lppl);
        for (i = 0; i < 4 && lpfl->rgwtMin[i] == 0; i++) {
        }
        if (i == 4 && isbsDst != -1 && isbsSrc != -1) {
            wt = 0;
            wtBig = 0;
            ishdefBig = -1;
            for (ishdef = 0; ishdef < 16; ishdef++) {
                if (lpfl->rgcsh[ishdef] != 0) {
                    wt = rglpshdef[lpfl->iPlayer][ishdef].hul.wtEmpty;
                    if (wt > wtBig) {
                        wtBig = wt;
                        ishdefBig = ishdef;
                    }
                }
            }
            if (MdCalcStargateDamage(isbsSrc, isbsDst, LOWORD(dTravel), wtBig, &pctDmg) == 1 && pctDmg == 0) {
                iWarp = 11;
            }
        }
        if (iWarp < 9 && LphuldefFromId(rglpshdefSB[lpfl->iPlayer][lpplRoute->isb].hul.ihuldef)->hul.wtCargoMax != 0x0) {
            for (iWarp = 9; iWarp > 0 && EstFuelUse(lpfl, 0, iWarp, -1, 1) < dTravel; iWarp--) {
            }
            lpord->iWarp = iWarp;
        }
    }
    if (iWarp < 11 && iWarp != 0) {
        cTurns = (int32_t)((int32_t)(dTravel / (int32_t)iWarp) / (int32_t)iWarp);
        do {
            if (iWarp <= 2)
                goto L_220b;
            iWarp = iWarp - 1;
        } while ((int32_t)((int32_t)(dTravel / (int32_t)iWarp) / (int32_t)iWarp) <= cTurns);
        iWarp = iWarp + 1;
    L_220b:
        for (; iWarp != 0 && EstFuelUse(lpfl, 0, iWarp, dTravel, 0) > lpfl->rgwtMin[4]; iWarp--) {
        }
    }
    lpord->iWarp = iWarp;
    return;
}

int16_t FColonizer(FLEET *lpfl) {
    int16_t i;
    int32_t l;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] != 0) {
            l = (int32_t)(0x1 << rglpshdef[lpfl->iPlayer][i].hul.ihuldef);
            if ((l & 0xc000) != 0x0) {
                return 1;
            }
        }
    }
    return 0;
}

int16_t FScout(FLEET *lpfl) {
    int16_t i;
    int32_t l;

    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] != 0) {
            l = (int32_t)(0x1 << rglpshdef[lpfl->iPlayer][i].hul.ihuldef);
            if ((l & 0x70) != 0x0) {
                return 1;
            }
        }
    }
    return 0;
}

void AutoFleetOrder(FLEET *lpfl, PLANET *lppl) {
    int32_t cMine;
    int16_t ifl;
    ORDER  *lpord;
    FLEET  *lpflT;
    int16_t fFoundFleet;

    fFoundFleet = 0;
    lpord = lpfl->lpplord->rgord;
    if ((lppl->iPlayer == -1 || (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raMacintosh && lppl->iPlayer == lpfl->iPlayer)) &&
        CMineFromLpfl(lpfl) != 0) {
        if (lppl->iPlayer == -1) {
            ifl = 0;
            while (1) {
                if (ifl >= cFleet)
                    goto L_256f;
                lpflT = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    goto L_256f;
                if (lpflT->iPlayer >= lpfl->iPlayer && lpflT->fDead == 0x0 && lpfl != lpflT) {
                    if (lpflT->iPlayer > lpfl->iPlayer)
                        goto L_256f;
                    if (lpflT->pt.x == lpfl->pt.x && lpflT->pt.y == lpfl->pt.y) {
                        cMine = CMineFromLpfl(lpflT);
                        if (cMine > 0 && cMine < 4000)
                            break;
                    }
                }
                ifl = ifl + 1;
            }
            fFoundFleet = 1;
        }
    L_256f:
        if (fFoundFleet == 0) {
            lpord->grTask = grTaskMine;
        } else {
            lpord->grTask = grTaskMerge;
            lpord->grobj = grobjFleet;
            lpord->id = lpflT->id;
        }
    }
    lpfl->iplan = 0x0;
    return;
}

int32_t CMineFromLpfl(FLEET *lpfl) {
    int32_t  cMine;
    int16_t  j;
    int16_t  i;
    HUL     *lphuldef;
    PART     part;
    int32_t  cMineTot;
    int16_t  chs;
    HS      *lphs;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    cMineTot = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphuldef = &rglpshdef[lpfl->iPlayer][i].hul;
            chs = lphuldef->chs;
            cMine = 0;
            j = 0;
            lphs = lphuldef->rghs;
            while (j < chs) {
                if (lphs->grhst == hstMining && lphs->iItem >= iminingRoboMidgetMiner && lphs->iItem <= iminingAlienMiner) {
                    part.hs.grhst = lphs->grhst;
                    t_fields_1 = &part.hs;
                    t_fields_2 = lphs->iItem;
                    t_fields_3 = lphs->cItem;
                    t_fields_1->iItem = t_fields_2;
                    t_fields_1->cItem = t_fields_3;
                    FLookupPart(&part);
                    cMine = cMine + (uint32_t)(lphs->cItem * (int32_t)part.pmining->grAbility);
                }
                j = j + 1;
                lphs = lphs + 1;
            }
            cMineTot = cMineTot + (uint32_t)(cMine * (int32_t)lpfl->rgcsh[i]);
        }
    }
    if (cMineTot < 4000) {
        return cMineTot;
    }
    return 4000;
}

int32_t PctTerraFromLpfl(FLEET *lpfl) {
    int16_t j;
    int32_t pctTot;
    int16_t i;
    int32_t pct;
    HUL    *lphuldef;
    int16_t chs;
    HS     *lphs;

    pctTot = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphuldef = &rglpshdef[lpfl->iPlayer][i].hul;
            chs = lphuldef->chs;
            pct = 0;
            j = 0;
            lphs = lphuldef->rghs;
            while (j < chs) {
                if (lphs->grhst == hstMining && lphs->iItem == iminingOrbitalAdjuster) {
                    pct = pct + lphs->cItem;
                }
                j = j + 1;
                lphs = lphs + 1;
            }
            pctTot = pctTot + (uint32_t)(pct * (int32_t)lpfl->rgcsh[i]);
        }
    }
    return pctTot;
}

int32_t CLayMinesFromLpfl(FLEET *lpfl, int16_t iType, int16_t ishdef) {
    uint16_t iMin;
    uint16_t iMax;
    int32_t  cMine;
    int16_t  j;
    int16_t  i;
    HUL     *lphul;
    PART     part;
    int32_t  cMineTot;
    int16_t  chs;
    HS      *lphs;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    cMineTot = 0;
    switch (iType) {
    case -1:
    default:
        iMin = 0x0;
        iMax = 0x9;
        break;
    case 0:
        iMin = 0x0;
        iMax = 0x3;
        break;
    case 1:
        iMin = 0x4;
        iMax = 0x6;
        break;
    case 2:
        iMin = 0x7;
        iMax = 0x9;
    }
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0 && (ishdef == -1 || i == ishdef)) {
            lphul = &rglpshdef[lpfl->iPlayer][i].hul;
            chs = lphul->chs;
            cMine = 0;
            j = 0;
            lphs = lphul->rghs;
            while (j < chs) {
                if (lphs->grhst != hstMines || lphs->iItem < iMin || lphs->iItem > iMax) {
                    if (iType <= 0 && lphs->grhst == hstBeam && lphs->iItem == ibeamMultiContainedMunition) {
                        cMine = cMine + (int32_t)(lphs->cItem * 0x4);
                    }
                } else {
                    part.hs.grhst = lphs->grhst;
                    t_fields_1 = &part.hs;
                    t_fields_2 = lphs->iItem;
                    t_fields_3 = lphs->cItem;
                    t_fields_1->iItem = t_fields_2;
                    t_fields_1->cItem = t_fields_3;
                    FLookupPart(&part);
                    cMine = cMine + (uint32_t)(lphs->cItem * (int32_t)part.pmines->grAbility);
                }
                j = j + 1;
                lphs = lphs + 1;
            }
            if (lphul->ihuldef == ihuldefMiniMineLayer || lphul->ihuldef == ihuldefSuperMineLayer) {
                cMine = (int32_t)(cMine * 2);
            }
            cMineTot = cMineTot + (uint32_t)(cMine * (int32_t)lpfl->rgcsh[i]);
        }
    }
    if (cMineTot > 10000000 || cMineTot < 0) {
        return 100000000;
    }
    return (uint32_t)(cMineTot * 10);
}

int32_t CMineSweepFromLpfl(FLEET *lpfl) {
    int32_t lPowTot;
    int16_t i;
    HUL    *lphul;
    int32_t lPow;

    lPowTot = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphul = &rglpshdef[lpfl->iPlayer][i].hul;
            lPow = CMineSweepFromLphul(lphul);
            lPowTot = lPowTot + (uint32_t)(lPow * (int32_t)lpfl->rgcsh[i]);
        }
    }
    if (lPowTot <= 0) {
        return 0;
    }
    return lPowTot;
}

int32_t CMineSweepFromLphul(HUL *lphul) {
    int16_t  chs;
    HS      *lphs;
    int32_t  lRange;
    int16_t  j;
    int16_t  fStarbase;
    int32_t  lPow;
    PART     part;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    fStarbase = lphul->ihuldef < ihuldefOrbitalFort ? 0 : 1;
    chs = lphul->chs;
    lPow = 0;
    j = 0;
    lphs = lphul->rghs;
    while (j < chs) {
        if (lphs->grhst == hstBeam) {
            part.hs.grhst = lphs->grhst;
            t_fields_1 = &part.hs;
            t_fields_2 = lphs->iItem;
            t_fields_3 = lphs->cItem;
            t_fields_1->iItem = t_fields_2;
            t_fields_1->cItem = t_fields_3;
            FLookupPart(&part);
            if ((part.pbeam->grfAbilities & 0x2) == 0x0) {
                if ((part.pbeam->grfAbilities & 0x1) != 0x0)
                    goto L_2c4d;
                lRange = (int32_t)part.pbeam->dRangeMax;
            } else {
                lRange = 4;
            }
            if (fStarbase != 0) {
                lRange = lRange + 1;
            }
            lPow = lPow + (uint32_t)((uint32_t)((uint32_t)(lRange * lRange) * lphs->cItem) * (int32_t)part.pbeam->dp);
        }
    L_2c4d:
        j = j + 1;
        lphs = lphs + 1;
    }
    if (lPow <= 0) {
        return 0;
    }
    return lPow;
}

int16_t PctCloakFromLpfl(FLEET *lpfl) {
    int16_t j;
    double  dcPts;
    double  dwtFleet;
    int16_t i;
    int32_t cPtsCur;
    int16_t fUseFloat;
    HUL    *lphul;
    int32_t wtFleet;
    int16_t cScore;
    int32_t cPts;
    int32_t wtFleetCur;
    int16_t chs;
    HS     *lphs;

    wtFleet = 0;
    cPts = 0;
    dwtFleet = 0.0;
    dcPts = 0.0;
    fUseFloat = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            lphul = &rglpshdef[lpfl->iPlayer][i].hul;
            chs = lphul->chs;
            wtFleetCur = (uint32_t)((int32_t)lpfl->rgcsh[i] * (uint32_t)lphul->wtEmpty);
            cPtsCur = 0;
            if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raStealth) {
                cPtsCur = cPtsCur + 300;
            }
            j = 0;
            lphs = lphul->rghs;
            while (j < chs) {
                cPtsCur = cPtsCur + (int32_t)CPtsCloakFromLphs(lphs);
                j = j + 1;
                lphs = lphs + 1;
            }
            if (cPtsCur > 0) {
                if (fUseFloat == 0 && (cPtsCur > 4000 || wtFleetCur > 500000 || cPts > 100000000 || wtFleet > 50000000)) {
                    dcPts = (double)cPts;
                    dwtFleet = (double)wtFleet;
                    fUseFloat = 1;
                }
                if (fUseFloat == 0) {
                    cPts = cPts + (uint32_t)(cPtsCur * wtFleetCur);
                } else {
                    dcPts = dcPts + (double)cPtsCur * (double)wtFleetCur;
                }
            }
            if (fUseFloat == 0) {
                wtFleet = wtFleet + wtFleetCur;
            } else {
                dwtFleet = dwtFleet + (double)wtFleetCur;
            }
        }
    }
    if (fUseFloat != 0 || cPts != 0) {
        if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) != raStealth) {
            if (fUseFloat == 0) {
                for (i = 0; i <= 3; i++) {
                    wtFleet = wtFleet + lpfl->rgwtMin[i];
                }
            } else {
                for (i = 0; i <= 3; i++) {
                    dwtFleet = dwtFleet + (double)lpfl->rgwtMin[i];
                }
            }
        }
        if (fUseFloat == 0) {
            cPts = (int32_t)(cPts / wtFleet);
        } else {
            cPts = (int32_t)(dcPts / dwtFleet);
        }
        if (cPts < 0) {
            return 0;
        }
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

int16_t CPtsCloakFromLphs(HS *lphs) {
    int16_t  cPts;
    PART     part;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    cPts = 0;
    if (lphs->cItem > 0x0) {
        switch (lphs->grhst) {
        case hstSpecialE:
            if (lphs->iItem < ispecialETransportCloaking || lphs->iItem > ispecialEMultiFunctionPod)
                break;
            part.hs.grhst = lphs->grhst;
            t_fields_1 = &part.hs;
            t_fields_2 = lphs->iItem;
            t_fields_3 = lphs->cItem;
            t_fields_1->iItem = t_fields_2;
            t_fields_1->cItem = t_fields_3;
            FLookupPart(&part);
            cPts = part.pspecial->grAbility;
            break;
        case hstSpecialM:
            if (lphs->iItem != ispecialMMultiCargoPod)
                break;
            cPts = 20;
            break;
        case hstScanner:
            if (lphs->iItem != iscannerChameleonScanner)
                break;
            cPts = 40;
            break;
        case hstArmor:
            if (lphs->iItem != iarmorDepletedNeutronium) {
                if (lphs->iItem != iarmorMegaPolyShell)
                    break;
                cPts = 40;
                break;
            }
            cPts = 50;
            break;
        case hstShield:
            if (lphs->iItem != ishieldShadowShield) {
                if (lphs->iItem != ishieldLangstonShell)
                    break;
                cPts = 20;
                break;
            }
            cPts = 70;
            break;
        case hstBeam:
            if (lphs->iItem != ibeamMultiContainedMunition)
                break;
            cPts = 20;
            break;
        case hstEngine:
            if (lphs->iItem != iengineEnigmaPulsar)
                break;
            cPts = 20;
            break;
        case hstMining:
            if (lphs->iItem != iminingAlienMiner) {
                if (lphs->iItem == iminingOrbitalAdjuster) {
                    cPts = 50;
                }
            } else {
                cPts = 60;
            }
        default:
        }
        if (lphs->cItem > 0x1) {
            cPts = cPts * lphs->cItem;
        }
        return cPts;
    }
    return 0;
}

INT_PTR CALLBACK MergeFleetsDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int16_t i;
    RECT    rc;
    char    szT[80];
    char   *psz;
    HWND    t_call_343f;
    WPARAM  t_merge_3487_0001;
    HWND    t_scratch_me;
    HWND    t_call_3640;

    if (msg == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(msg) != 0) {
        t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
        if (t_scratch_me != GetDlgItem(hwnd, IDC_U16_0x0051)) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (msg == WM_INITDIALOG) {
            StickyDlgPos(hwnd, &ptStickyMergeFleetsDlg, 1);
            for (i = 0; i < vcflMerge; i++) {
                psz = PszGetFleetName(rglpfl[vrgiflMerge[i]]->id);
                strcpy(szT, psz);
                if (rglpfl[vrgiflMerge[i]]->cord > 1) {
                    strcat(szT, " *");
                }
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0051), LB_ADDSTRING, 0x0, (LPARAM)szT);
                t_call_343f = GetDlgItem(hwnd, IDC_U16_0x0051);
                if (vcflMerge != 2 && rglpfl[vrgiflMerge[i]]->id != sel.fl.id) {
                    t_merge_3487_0001 = 0x0;
                } else {
                    t_merge_3487_0001 = 0x1;
                }
                SendMessage(t_call_343f, LB_SETSEL, t_merge_3487_0001, (int32_t)i);
            }
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        }
        if (msg == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                for (i = 0; i < vcflMerge; i++) {
                    if (SendMessage(GetDlgItem(hwnd, IDC_U16_0x0051), LB_GETSEL, i, 0) == 0) {
                        vrgiflMerge[i] = -1;
                    }
                }
                if (GET_WM_COMMAND_ID(wParam, lParam) != IDOK || gd.fTutorial == 0x0 || FOKMergeDialog() != 0) {
                    StickyDlgPos(hwnd, &ptStickyMergeFleetsDlg, 0);
                    EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                    return 1;
                }
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, 0x453);
                return 1;
            case 0x7f8:
            case 0x7f9:
                for (i = 0; i < vcflMerge; i++) {
                    t_call_3640 = GetDlgItem(hwnd, IDC_U16_0x0051);
                    SendMessage(t_call_3640, LB_SETSEL, GET_WM_COMMAND_ID(wParam, lParam) == 0x7f8 ? 0x1 : 0x0, (int32_t)i);
                }
                return 1;
            default:
            }
        }
    }
    return 0;
}

void MarkTechsSeen(HUL *lphul, int16_t iplr) {
    int16_t  iplrSav;
    int16_t  iTech;
    int16_t  ihs;
    PART     part;
    uint16_t t_scratch_m12_2;
    uint16_t t_scratch_m12_3;

    iplrSav = idPlayer;
    idPlayer = iplr;
    part.hs.grhst = hstHull;
    part.hs.iItem = lphul->ihuldef;
    FLookupPart(&part);
    for (iTech = 0; iTech < 6; iTech++) {
        t_scratch_m12_2 = rgTechBattle[iTech];
        rgTechBattle[iTech] = LOBYTE(t_scratch_m12_2 <= (int16_t)part.phul->rgTech[iTech] ? (int16_t)part.phul->rgTech[iTech] : rgTechBattle[iTech]);
    }
    for (ihs = 0; ihs < lphul->chs; ihs++) {
        if (lphul->rghs[ihs].cItem != 0x0) {
            part.hs = lphul->rghs[ihs];
            FLookupPart(&part);
            for (iTech = 0; iTech < 6; iTech++) {
                t_scratch_m12_3 = rgTechBattle[iTech];
                rgTechBattle[iTech] = LOBYTE(t_scratch_m12_3 <= (int16_t)part.pcom->rgTech[iTech] ? (int16_t)part.pcom->rgTech[iTech] : rgTechBattle[iTech]);
            }
            iTech = -1;
            switch (part.hs.grhst) {
            case hstSpecialM:
                if (part.hs.iItem != ispecialMMultiCargoPod) {
                    if (part.hs.iItem != ispecialMJumpGate)
                        break;
                    iTech = 11;
                    break;
                }
                iTech = 0;
                break;
            case hstSpecialE:
                if (part.hs.iItem != ispecialEMultiFunctionPod)
                    break;
                iTech = 1;
                break;
            case hstShield:
                if (part.hs.iItem != ishieldLangstonShell)
                    break;
                iTech = 2;
                break;
            case hstArmor:
                if (part.hs.iItem != iarmorMegaPolyShell)
                    break;
                iTech = 3;
                break;
            case hstMining:
                if (part.hs.iItem != iminingAlienMiner)
                    break;
                iTech = 4;
                break;
            case hstBomb:
                if (part.hs.iItem != ibombHushABoom)
                    break;
                iTech = 5;
                break;
            case hstBeam:
                if (part.hs.iItem != ibeamMultiContainedMunition)
                    break;
                iTech = 7;
                break;
            case hstTorp:
                if (part.hs.iItem != itorpAntiMatterTorpedo)
                    break;
                iTech = 6;
                break;
            case hstEngine:
                if (part.hs.iItem == iengineEnigmaPulsar) {
                    iTech = 9;
                }
            default:
            }
            if (iTech != -1 && rgTechTrader[iTech] < 0x19) {
                rgTechTrader[iTech] = rgTechTrader[iTech] + LOBYTE(part.hs.cItem);
                if (rgTechTrader[iTech] > 0x19) {
                    rgTechTrader[iTech] = 0x19;
                }
            }
        }
    }
    idPlayer = iplrSav;
    return;
}
