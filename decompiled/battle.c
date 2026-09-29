#include "common.h"

uint8_t rgbrcStart[136] = {68,  65,  88,  20,  136, 129, 17,  136, 129, 24,  20,  134, 65,  72,  130, 65,  88,  130, 23,  134, 19,  17,  81,
                           130, 134, 104, 40,  21,  49,  97,  131, 134, 104, 56,  22,  19,  49,  104, 131, 22,  97,  56,  134, 19,  68,  18,
                           21,  24,  65,  72,  84,  113, 120, 131, 134, 49,  104, 131, 22,  97,  56,  134, 19,  67,  54,  102, 65,  88,  130,
                           23,  134, 19,  97,  56,  33,  132, 21,  120, 17,  49,  81,  113, 19,  21,  23,  56,  88,  131, 133, 135, 68,  17,
                           49,  81,  113, 130, 132, 134, 136, 104, 72,  40,  23,  21,  19,  17,  49,  81,  113, 130, 132, 134, 136, 104, 72,
                           40,  23,  21,  19,  68,  17,  49,  81,  113, 130, 132, 134, 136, 104, 72,  40,  23,  21,  19,  51,  102};

INT_PTR CALLBACK RelationsDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rcGBox;
    int16_t     mdSBase;
    HWND        t_scratch_me_2;
    uint16_t    t_scratch_me_3;

    switch (message) {
    case WM_ERASEBKGND:
    L_0177:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x07D5), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x07D6), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 0x1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SetBkColor(hdc, crButtonFace);
        SelectObject(hdc, rghfontArial8[1]);
        i = CchGetString(idsRelation, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, i);
        SelectObject(hdc, rghfontArial8[0]);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_DESTROY:
        if (fDirtyPlan == 0)
            break;
        mdSBase = grbitScan & 0xf;
        LogChangeRelations();
        InvalidateRect(hwndScanner, 0x0, 1);
        break;
    default:
        if (IS_WM_CTLCOLOR(message) != 0) {
            t_scratch_me_2 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me_2 != GetDlgItem(hwnd, IDC_U16_0x07D3)) {
                SetBkColor((HDC)wParam, crButtonFace);
                return (INT_PTR)hbrButtonFace;
            }
        } else {
            if (message == WM_INITDIALOG) {
                StickyDlgPos(hwnd, &ptStickyRelationsDlg, 1);
                CheckRadioButton(hwnd, 2004, 2006, (int16_t)rgplr[idPlayer].rgmdRelation[idPlayer == 0 ? 1 : 0] - 44);
                for (i = 0; i < game.cPlayer; i++) {
                    if (i != idPlayer) {
                        SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), LB_ADDSTRING, 0x0, (LPARAM)PszPlayerName(i, 0, 0, 0, 0, 0x0));
                    }
                }
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), LB_SETCURSEL, 0x0, 0);
                fDirtyPlan = 0;
                goto L_0177;
            }
            if (message == WM_COMMAND) {
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) {
                    StickyDlgPos(hwnd, &ptStickyRelationsDlg, 0);
                    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), LB_GETCURSEL, 0x0, 0));
                    if (i >= idPlayer) {
                        i = i + 1;
                    }
                    EndDialog(hwnd, i + 3);
                    return 1;
                }
                if (GET_WM_COMMAND_ID(wParam, lParam) < 0x7d4 || GET_WM_COMMAND_ID(wParam, lParam) > IDC_U16_0x07D6) {
                    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_U16_0x07D3) {
                        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                            WinHelp(hwnd, szHelpFile, 0x1, 0x43b);
                            return 1;
                        }
                    } else {
                        i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), LB_GETCURSEL, 0x0, 0));
                        if (i >= idPlayer) {
                            i = i + 1;
                        }
                        CheckRadioButton(hwnd, 2004, 2006, (int16_t)rgplr[idPlayer].rgmdRelation[i] - 44);
                    }
                } else {
                    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), LB_GETCURSEL, 0x0, 0));
                    if (i >= idPlayer) {
                        i = i + 1;
                    }
                    t_scratch_me_3 = GET_WM_COMMAND_ID(wParam, lParam) - 2004;
                    rgplr[idPlayer].rgmdRelation[i] = LOBYTE(t_scratch_me_3);
                    fDirtyPlan = 1;
                }
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK NewPlanNameDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT rc;

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
            SetWindowPos(hwnd, 0x0, ptStickyBattlePlansDlg.x + 70, ptStickyBattlePlansDlg.y + 70, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0x1f, 0);
            SetDlgItemText(hwnd, IDC_EDIT1, btlplan.szName);
            return 1;
        }
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                    GetDlgItemText(hwnd, IDC_EDIT1, btlplan.szName, 32);
                    fDirtyPlan = 1;
                }
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, 0x439);
                return 1;
            default:
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK BattlePlansDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    FARPROC lpProc;
    int16_t idc;
    int16_t i;
    int16_t fRet;
    RECT    rc;
    int16_t cLen;
    HWND    t_scratch_m16;
    uint8_t t_11bd;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (idc = 1053; idc <= 1058; idc++) {
            t_scratch_m16 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m16 == GetDlgItem(hwnd, idc))
                break;
        }
        if (idc >= 1053 || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 1);
            iPlanSelDlg = 0;
            if (sel.grobj == grobjFleet) {
                iPlanSelDlg = sel.fl.iplan;
            }
            btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
            for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
            }
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0);
            EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
            EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
            for (i = 408; i <= 413; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(i));
            }
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0);
            for (i = 400; i <= 407; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(i));
            }
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0);
            if (game.fSinglePlr != 0x0) {
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(idsEveryone));
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, 0x0, 0);
                EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0422), 0);
            } else {
                for (i = 120; i <= 123; i++) {
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(i));
                }
                for (i = 0; i < game.cPlayer; i++) {
                    if (i != idPlayer) {
                        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, (LPARAM)PszPlayerName(i, 0, 1, 0, 0, 0x0));
                    }
                }
                i = btlplan.iplrAttack;
                if (i >= idPlayer + 4) {
                    i = i - 1;
                }
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0);
            }
            for (i = 400; i <= 407; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(i));
            }
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0);
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), BM_SETCHECK, btlplan.fDumpCargo, 0);
            fDirtyPlan = 0;
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        }
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                if (fDirtyPlan != 0) {
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    LogChangeBtlplan(&btlplan);
                }
                StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 0);
                EndDialog(hwnd, iPlanSelDlg);
                if (sel.grobj == grobjFleet) {
                    FillBattleDD(sel.fl.iplan + 1);
                }
                iPlanSelDlg = -1;
                return 1;
            case IDC_U16_0x041D:
                btlplan.fDumpCargo = LOWORD(SendDlgItemMessage(hwnd, 1053, BM_GETCHECK, 0x0, 0));
                fDirtyPlan = 1;
                break;
            case IDC_DELETE:
                if (fDirtyPlan != 0) {
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    LogChangeBtlplan(&btlplan);
                    fDirtyPlan = 0;
                }
                btlplan.fDelete = 0x1;
                rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                btlplan.iplan = iPlanSelDlg;
                if (FDeleteBattlePlan(iPlanSelDlg, 1) == 0) {
                    btlplan.fDelete = 0x0;
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    break;
                }
                LogChangeBtlplan(&btlplan);
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg - 1, 0);
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0);
                for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
                }
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg - 1, 0);
                goto LSelectName;
            case IDC_U16_0x041F:
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 0x407, 0x0, 0));
                btlplan.mdTarget1 = i;
                fDirtyPlan = 1;
                break;
            case IDC_U16_0x0420:
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 0x407, 0x0, 0));
                btlplan.mdTarget2 = i;
                fDirtyPlan = 1;
                break;
            case IDC_U16_0x0422:
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 0x407, 0x0, 0));
                if (game.fSinglePlr == 0x0) {
                    if (i >= idPlayer + 4) {
                        i = i + 1;
                    }
                } else {
                    i = 3;
                }
                btlplan.iplrAttack = i;
                fDirtyPlan = 1;
                break;
            case IDC_U16_0x0421:
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 0x407, 0x0, 0));
                btlplan.mdTactic = i;
                fDirtyPlan = 1;
                break;
            case IDC_RENAME:
            LRename:
                StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 0);
                lpProc = MakeProcInstance(NewPlanNameDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc);
                FreeProcInstance(lpProc);
                SetFocus(hwnd);
                if (fRet != 0) {
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0);
                    for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                        SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
                    }
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0);
                }
                EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
                EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
                break;
            case 0x41c:
                if (rgcbtlplan[idPlayer] != 0xf) {
                    if (fDirtyPlan != 0) {
                        rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                        LogChangeBtlplan(&btlplan);
                        fDirtyPlan = 0;
                    }
                    t_11bd = rgcbtlplan[idPlayer];
                    rgcbtlplan[idPlayer] = rgcbtlplan[idPlayer] + 0x1;
                    iPlanSelDlg = t_11bd;
                    cLen = strlen(btlplan.szName);
                    if (cLen <= 27) {
                        if (btlplan.szName[cLen - 1] == ')' && isdigit(btlplan.szName[cLen - 2]) != 0x0 && btlplan.szName[cLen - 3] == '(') {
                            if (btlplan.szName[cLen - 2] != '9') {
                                btlplan.szName[cLen - 2] = btlplan.szName[cLen - 2] + 1;
                            } else {
                                btlplan.szName[cLen - 2] = '0';
                            }
                        } else {
                            strcpy(&btlplan.szName[cLen], " (2)");
                        }
                    }
                    btlplan.iplan = iPlanSelDlg;
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0);
                    for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                        SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
                    }
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0);
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), BM_SETCHECK, btlplan.fDumpCargo, 0);
                    i = btlplan.iplrAttack;
                    if (i >= idPlayer + 4) {
                        i = i - 1;
                    }
                    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0);
                    fDirtyPlan = 1;
                    wParam = 0x41f;
                    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), 1);
                    goto LRename;
                }
                return 0;
            case IDC_U16_0x041E:
            LSelectName:
                i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_GETCURSEL, 0x0, 0));
                if (i == iPlanSelDlg)
                    break;
                if (fDirtyPlan != 0) {
                    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                    LogChangeBtlplan(&btlplan);
                    fDirtyPlan = 0;
                }
                iPlanSelDlg = i;
                btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0);
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0);
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), BM_SETCHECK, btlplan.fDumpCargo, 0);
                wParam = 0x41f;
                EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
                EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0);
                i = btlplan.iplrAttack;
                if (i >= idPlayer + 4) {
                    i = i - 1;
                }
                SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0);
                break;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, 0x439);
                return 1;
            default:
            }
        }
    }
    return 0;
}

int16_t FDeleteBattlePlan(int16_t iplan, int16_t fWarn) {
    int16_t fFoundBigger;
    int16_t iflMac;
    int16_t i;
    FLEET  *lpfl;

    fFoundBigger = 0;
    while (1) {
        iflMac = 0;
        while (1) {
            if (iflMac >= cFleet)
                goto L_181a;
            lpfl = rglpfl[iflMac];
            if (rglpfl[iflMac] == 0x0)
                goto L_181a;
            if (lpfl->iPlayer >= idPlayer) {
                if (lpfl->iPlayer > idPlayer)
                    goto L_181a;
                if (lpfl->iplan >= (uint16_t)iplan) {
                    if (lpfl->iplan <= (uint16_t)iplan) {
                        if (fWarn != 0)
                            break;
                        lpfl->iplan = lpfl->iplan - 0x1;
                    } else if (fWarn != 0) {
                        fFoundBigger = 1;
                    } else {
                        lpfl->iplan = lpfl->iplan - 0x1;
                    }
                }
            }
            iflMac = iflMac + 1;
        }
        if (AlertSz(PszFormatIds(idsCurrentlyHaveFleetsUsingBattlePlanIf, 0x0), MB_OKCANCEL | MB_ICONEXCLAMATION) != IDCANCEL) {
            fWarn = 0;
            continue;
        }
        break;
    L_181a:
        if (fWarn == 0 || fFoundBigger == 0)
            goto L_1834;
        fWarn = 0;
    }
    return 0;
L_1834:
    rgcbtlplan[idPlayer] = rgcbtlplan[idPlayer] - 0x1;
    for (i = iplan; i < rgcbtlplan[idPlayer]; i++) {
        rglpbtlplan[idPlayer][i] = rglpbtlplan[idPlayer][i + 1];
        rglpbtlplan[idPlayer][i].iplan = i;
    }
    return 1;
}

void SpankTheCheaters() {
    int32_t lSell;
    PLANET *lppl;
    FLEET  *lpfl;
    int16_t ifl;
    int16_t i;
    int32_t pctSell;
    int16_t fCheater;
    int16_t fSellOff;
    char    rgfCheater[16];
    PLANET *lpplMac;

    fCheater = 0;
    for (i = 0; i < game.cPlayer; i++) {
        rgfCheater[i] = LOBYTE(rgplr[i].fCheater);
        if ((int16_t)LOBYTE(rgplr[i].fCheater) != 0x0) {
            fCheater = 1;
        }
    }
    if (fCheater != 0 && game.turn >= 0xa) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0x0)
                break;
            if (lpfl->fDead == 0x0 && (int16_t)rgfCheater[lpfl->iPlayer] != 0) {
                if (Random(12) != 0) {
                    fSellOff = 0;
                    for (i = 0; i <= 2; i++) {
                        if (lpfl->rgwtMin[i] > 0) {
                            if (fSellOff == 0) {
                                pctSell = (int32_t)(Random(11) + 10);
                                fSellOff = 1;
                            }
                            lSell = (int32_t)((int32_t)(lpfl->rgwtMin[i] * pctSell) / 100);
                            if (lSell == 0) {
                                lSell = 1;
                            }
                            lpfl->rgwtMin[i] = lpfl->rgwtMin[i] - lSell;
                        }
                    }
                    if (fSellOff != 0) {
                        FSendPlrMsg2(lpfl->iPlayer, 261, -5, lpfl->id, LOWORD(pctSell));
                    }
                } else {
                    lpfl->fDead = 0x1;
                    FSendPlrMsg2(lpfl->iPlayer, 260, -5, lpfl->id, 0);
                }
            }
        }
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            if (lppl->iPlayer != -1 && (int16_t)rgfCheater[lppl->iPlayer] != 0) {
                if (lppl->cMines <= 0x0 || Random(8) != 0) {
                    if (Random(15) == 0) {
                        i = Random(3);
                        pctSell = (int32_t)(Random(41) + 5);
                        lSell = (int32_t)((int32_t)(lppl->rgwtMin[i] * pctSell) / 100);
                        if (lSell > 0) {
                            if (lSell > 30000) {
                                lSell = 30000;
                            }
                            lppl->rgwtMin[i] = lppl->rgwtMin[i] - lSell;
                            FSendPlrMsg(lppl->iPlayer, 263, -5, lppl->id, LOWORD(lSell), i + 1, 0, 0, 0, 0);
                        }
                    }
                } else {
                    pctSell = (int32_t)(Random(31) + 5);
                    lSell = (int32_t)((int32_t)(lppl->cMines * pctSell) / 0x64);
                    if (lSell <= 0) {
                        lSell = 1;
                    }
                    lppl->cMines = lppl->cMines - LOWORD(lSell);
                    FSendPlrMsg2(lppl->iPlayer, 262, -5, lppl->id, LOWORD(lSell));
                }
            }
        }
    }
    return;
}

int16_t FFleetHasBombs(FLEET *lpfl) {
    HUL    *lphul;
    int16_t imd;
    int16_t ishdef;

    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (lpfl->rgcsh[ishdef] != 0) {
            lphul = &rglpshdef[lpfl->iplr][ishdef].hul;
            imd = LphuldefFromId(lphul->ihuldef)->imdAttack;
            if (FHullHasBombs(lphul) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

int16_t FHullHasBombs(HUL *lphul) {
    HS     *lphs;
    int16_t ihs;

    lphs = lphul->rghs;
    ihs = 0;
    while (1) {
        if (ihs >= lphul->chs) {
            return 0;
        }
        if (lphs->grhst == hstBomb && lphs->cItem != 0x0)
            break;
        if (lphs->grhst == hstBeam && lphs->iItem == ibeamMultiContainedMunition && lphs->cItem > 0x0) {
            return 1;
        }
        if (lphs->grhst == hstSpecialM && lphs->iItem == ispecialMOrbitalConstructionModule && lphs->cItem > 0x0) {
            return 1;
        }
        ihs = ihs + 1;
        lphs = lphs + 1;
    }
    return 1;
}

int16_t FFleetHasTeeth(FLEET *lpfl) {
    int16_t ishdef;

    for (ishdef = 0; ishdef < 16; ishdef++) {
        if (lpfl->rgcsh[ishdef] != 0 && FHullHasTeeth(&rglpshdef[lpfl->iplr][ishdef].hul) != 0 && rglpshdef[lpfl->iplr][ishdef].det == 0x7) {
            return 1;
        }
    }
    return 0;
}

int16_t FHullHasTeeth(HUL *lphul) {
    HS     *lphs;
    int16_t ihs;

    lphs = lphul->rghs;
    ihs = 0;
    while (1) {
        if (ihs >= lphul->chs) {
            return 0;
        }
        if ((lphs->grhst & 0x30) != 0x0 && lphs->cItem > 0x0)
            break;
        ihs = ihs + 1;
        lphs = lphs + 1;
    }
    return 1;
}

int16_t FFuelTanker(SHDEF *lpshdef) {
    if (lpshdef->hul.ihuldef != ihuldefFuelTransport && lpshdef->hul.ihuldef != ihuldefSuperFuelXport) {
        return 0;
    }
    return 1;
}

void CheckTarget(TOK *ptok, FLEET *lpfl, int16_t ishdef) {
    int16_t  iplr;
    BTLPLAN *lpbtlplan;
    int16_t  ibp;
    SHDEF   *lpshdef;

    iplr = lpfl->iplr;
    lpshdef = rglpshdef[iplr] + ishdef;
    if (FHullHasTeeth(&lpshdef->hul) == 0) {
        if (FHullHasBombs(&lpshdef->hul) == 0) {
            if (FFuelTanker(lpshdef) == 0) {
                if (WtMaxShdefStat(lpshdef, 2) == 0) {
                    ptok->mdTarget0 = 0x5;
                } else {
                    ptok->mdTarget0 = 0x7;
                }
            } else {
                ptok->mdTarget0 = 0x6;
            }
        } else {
            ptok->mdTarget0 = 0x4;
        }
    } else {
        ptok->mdTarget0 = 0x3;
    }
    ibp = lpfl->iplan;
    lpbtlplan = rglpbtlplan[iplr] + ibp;
    ptok->mdTarget1 = lpbtlplan->mdTarget1;
    ptok->mdTarget2 = lpbtlplan->mdTarget2;
    if (ptok->mdTarget0 != 0x3) {
        ptok->mdTactic = 0x0;
    } else {
        ptok->mdTactic = lpbtlplan->mdTactic;
    }
    if (ptok->mdTactic == 0x0) {
        ptok->dzDis = 0x7;
    }
    return;
}

int16_t FDumpCargo(FLEET *lpfl) {
    POINT16 pt;
    PLANET *lppl;
    int16_t i;

    for (i = 0; i <= 2 && lpfl->rgwtMin[i] == 0; i++) {
    }
    if (i <= 2) {
        if (rglpbtlplan[lpfl->iplr][lpfl->iplan].fDumpCargo != 0x0) {
            if (lpfl->idPlanet == -1) {
                pt = lpfl->pt;
                DropSalvage(&lpthBattle, lpfl->rgwtMin, lpfl->iplr, &pt);
            } else {
                lppl = LpplFromId(lpfl->idPlanet);
                for (i = 0; i <= 2; i++) {
                    lppl->rgwtMin[i] = lppl->rgwtMin[i] + lpfl->rgwtMin[i];
                }
            }
            for (i = 0; i <= 2; i++) {
                lpfl->rgwtMin[i] = 0;
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT16 *ppt) {
    int32_t wtTotal;
    int32_t wt;
    int16_t i;
    THING  *lpth;

    lpth = *plpth;
    wtTotal = 0;
    for (i = 0; i < game.cPlanMax; i++) {
        if (ppt->x == rgptPlan[i].x && ppt->y == rgptPlan[i].y) {
            return;
        }
    }
    for (i = 0; i < 3; i++) {
        wtTotal = wtTotal + rgwtMinerals[i];
    }
    while (wtTotal == 0) {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] = (int32_t)Random(10);
            wtTotal = wtTotal + rgwtMinerals[i];
        }
    }
    if (lpth != 0x0) {
        for (i = 0; i < 3; i++) {
            rgwtMinerals[i] = rgwtMinerals[i] + (int32_t)lpth->thp.rgwtMin[i];
            wtTotal = wtTotal + (int32_t)lpth->thp.rgwtMin[i];
            lpth->thp.rgwtMin[i] = 0;
        }
        lpth->thp.wtMax = 0x0;
    } else {
        lpth = LpthNew(iplr, ithMineralPacket);
        if (lpth == 0x0) {
            return;
        }
        lpth->thp.iWarp = 0x0;
        lpth->pt.x = ppt->x;
        lpth->pt.y = ppt->y;
        lpth->thp.idPlanet = 0x3ff;
    }
    lpth->thp.fMoved = 0x1;
    while (wtTotal > 0) {
        for (i = 0; i < 3; i++) {
            if ((uint32_t)(lpth->thp.wtMax * 0xa) + rgwtMinerals[i] <= 0x7530) {
                lpth->thp.wtMax = lpth->thp.wtMax + (rgwtMinerals[i] + 9) / 0xa;
                lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + LOWORD(rgwtMinerals[i]);
                wtTotal = wtTotal - rgwtMinerals[i];
                rgwtMinerals[i] = 0;
            } else {
                wt = 30000 - (uint32_t)(lpth->thp.wtMax * 0xa);
                wtTotal = wtTotal - wt;
                lpth->thp.wtMax = 0xbb8;
                lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + LOWORD(wt);
                rgwtMinerals[i] = rgwtMinerals[i] - wt;
                lpth = LpthNew(iplr, ithMineralPacket);
                if (lpth == 0x0) {
                    return;
                }
                lpth->thp.iWarp = 0x0;
                lpth->thp.idPlanet = 0x3ff;
                lpth->pt.x = ppt->x;
                lpth->pt.y = ppt->y;
            }
            if (wtTotal <= 0)
                break;
        }
    }
    *plpth = lpth;
    return;
}

int16_t CplrBattle(FLEET *lpfl, uint16_t *rggrfAttack, uint16_t *pgrfPlayer, uint16_t *pgrfSpectator) {
    int16_t  iplrStarbase;
    FLEET   *lpflCur;
    int32_t  rgcsh[16];
    uint16_t grPlr;
    int16_t  iplrCur;
    PLANET  *lppl;
    int16_t  cplr;
    int16_t  i;
    int16_t  mdRel;
    uint8_t  rgctok[16];
    int16_t  fChange;
    uint16_t iplrAttack;
    int16_t  fAttack;
    int16_t  cshdef;
    int16_t  ishdef;
    int16_t  cflTotal;
    uint16_t grfPlayer;
    int16_t  ctokNew;
    int16_t  ctokFleet;

    fAttack = 0;
    iplrStarbase = -1;
    grfPlayer = 0x0;
    cshdef = 0;
    grfMissed = 0x0;
    cflTotal = 0;
    *pgrfSpectator = 0x0;
    memset(rggrfAttack, 0, 0x20);
    memset(rgcsh, 0, 0x40);
    if (lpfl->idPlanet != -1) {
        lppl = LpplFromId(lpfl->idPlanet);
        if (lppl->fStarbase == 0x0) {
            if (lppl->iPlayer != -1) {
                *pgrfSpectator = *pgrfSpectator | 0x1 << lppl->iPlayer;
            }
        } else {
            iplrStarbase = lppl->iPlayer;
            grfPlayer = grfPlayer | 0x1 << iplrStarbase;
            iplrAttack = rglpbtlplan[iplrStarbase]->iplrAttack;
            if (FHullHasTeeth(&rglpshdefSB[iplrStarbase][lppl->isb].hul) != 0) {
                switch (iplrAttack) {
                case 0x3:
                    rggrfAttack[iplrCur] = ~(0x1 << iplrStarbase) & 0xffff;
                    break;
                case 0x1:
                case 0x2:
                    for (i = 0; i < game.cPlayer; i++) {
                        if (i != iplrStarbase) {
                            mdRel = (int16_t)rgplr[iplrStarbase].rgmdRelation[i];
                            if (mdRel == 2 || (mdRel == 0 && iplrAttack == 0x2)) {
                                rggrfAttack[iplrStarbase] = rggrfAttack[iplrStarbase] | 0x1 << i;
                            }
                        }
                    }
                    break;
                default:
                    rggrfAttack[iplrCur] = rggrfAttack[iplrCur] | 0x1 << (iplrAttack - 0x4);
                case 0x0:
                }
            }
        }
    }
    lpflCur = lpfl;
    do {
        if (lpflCur->fDead == 0x0) {
            iplrCur = lpflCur->iPlayer;
            grfPlayer = grfPlayer | 0x1 << iplrCur;
            if (rglpbtlplan[lpflCur->iplr][lpflCur->iplan].mdTarget1 != mdTargetNone && rglpbtlplan[lpflCur->iplr][lpflCur->iplan].iplrAttack != 0x0 &&
                FFleetHasTeeth(lpflCur) != 0) {
                fAttack = 1;
                iplrAttack = rglpbtlplan[iplrCur][lpflCur->iplan].iplrAttack;
                if (rglpbtlplan[lpflCur->iplr][lpflCur->iplan].mdTarget1 == mdTargetNone) {
                    iplrAttack = 0x0;
                }
                switch (iplrAttack) {
                case 0x3:
                    rggrfAttack[iplrCur] = ~(0x1 << iplrCur) & 0xffff;
                    break;
                case 0x1:
                case 0x2:
                    for (i = 0; i < game.cPlayer; i++) {
                        if (i != iplrCur) {
                            mdRel = (int16_t)rgplr[iplrCur].rgmdRelation[i];
                            if (mdRel == 2 || (mdRel == 0 && iplrAttack == 0x2)) {
                                rggrfAttack[iplrCur] = rggrfAttack[iplrCur] | 0x1 << i;
                            }
                        }
                    }
                    break;
                default:
                    rggrfAttack[iplrCur] = rggrfAttack[iplrCur] | 0x1 << (iplrAttack - 0x4);
                case 0x0:
                }
            }
            lpflCur->fDone = 0x1;
            lpflCur->fInclude = 0x1;
        }
        lpflCur = lpflCur->lpflNext;
    } while (lpflCur != lpfl);
    if (fAttack != 0) {
        iplrAttack = 0x0;
        for (i = 0; i < game.cPlayer; i++) {
            if (rggrfAttack[i] != 0x0) {
                iplrAttack = iplrAttack | (grfPlayer & rggrfAttack[i]);
            }
        }
        if (iplrAttack != 0x0) {
            for (i = 0; i < game.cPlayer; i++) {
                if ((rggrfAttack[i] & iplrAttack) != 0x0) {
                    iplrAttack = iplrAttack | 0x1 << i;
                }
                if ((0x1 << i & iplrAttack) != 0x0) {
                    for (iplrCur = 0; iplrCur < game.cPlayer; iplrCur++) {
                        if ((0x1 << i & rggrfAttack[iplrCur]) != 0x0) {
                            rggrfAttack[i] = rggrfAttack[i] | 0x1 << iplrCur;
                        }
                    }
                }
            }
            do {
                if (lpflCur == lpfl) {
                    fChange = 0;
                }
                iplrCur = lpflCur->iPlayer;
                grPlr = 0x1 << iplrCur;
                if ((grfPlayer & grPlr) != 0x0 && (iplrAttack & grPlr) == 0x0) {
                    rggrfAttack[iplrCur] = 0x0;
                    i = 0;
                    while (1) {
                        if (i >= game.cPlayer)
                            goto L_2f94;
                        if (i != iplrCur && (int16_t)rgplr[iplrCur].rgmdRelation[i] == 1 && (0x1 << i & iplrAttack) != 0x0) {
                            if ((0x1 << i & rggrfAttack[iplrCur]) != 0x0)
                                break;
                            rggrfAttack[iplrCur] = rggrfAttack[iplrCur] | rggrfAttack[i];
                        }
                        i = i + 1;
                    }
                    rggrfAttack[iplrCur] = 0x0;
                L_2f94:
                    if (rggrfAttack[iplrCur] == 0x0) {
                        grfPlayer = grfPlayer & ~grPlr;
                    } else {
                        iplrAttack = iplrAttack | grPlr;
                    }
                    fChange = 1;
                }
                lpflCur = lpflCur->lpflNext;
            } while (fChange != 0 || lpflCur != lpfl);
            if (iplrStarbase != -1 && (0x1 << iplrStarbase & grfPlayer) != 0x0) {
                rgcsh[iplrStarbase] = 1;
                cshdef = 1;
            }
            fChange = 0;
            do {
                iplrCur = lpflCur->iPlayer;
                grPlr = 0x1 << iplrCur;
                if ((grfPlayer & grPlr) == 0x0) {
                    *pgrfSpectator = *pgrfSpectator | grPlr;
                    lpflCur->fInclude = 0x0;
                } else {
                    for (ishdef = 0; ishdef < 16; ishdef++) {
                        if (lpflCur->rgcsh[ishdef] != 0) {
                            if (LphuldefFromId(rglpshdef[iplrCur][ishdef].hul.ihuldef)->imdAttack != 0x0) {
                                rgcsh[iplrCur] = rgcsh[iplrCur] + (int32_t)lpflCur->rgcsh[ishdef];
                            }
                            cshdef = cshdef + 1;
                        }
                    }
                }
                lpflCur = lpflCur->lpflNext;
            } while (lpflCur != lpfl);
            cplr = 0;
            for (; iplrAttack != 0x0; iplrAttack = iplrAttack >> 0x1) {
                if ((iplrAttack & 0x1) != 0x0) {
                    cplr = cplr + 1;
                }
            }
            if (cshdef > 255) {
                ctokNew = 0;
                i = 255 / cplr;
                if (iplrStarbase != -1 && (0x1 << iplrStarbase & grfPlayer) != 0x0) {
                    ctokNew = ctokNew + 1;
                }
                memset(rgctok, 0, 0x10);
                do {
                    if (lpflCur->fInclude != 0x0) {
                        ctokFleet = 0;
                        iplrCur = lpflCur->iPlayer;
                        for (ishdef = 0; ishdef < 16; ishdef++) {
                            if (lpflCur->rgcsh[ishdef] != 0) {
                                ctokFleet = ctokFleet + 1;
                            }
                        }
                        if (rgctok[iplrCur] + ctokFleet <= i) {
                            rgctok[iplrCur] = rgctok[iplrCur] + LOBYTE(ctokFleet);
                            ctokNew = ctokNew + ctokFleet;
                        } else {
                            lpflCur->fInclude = 0x0;
                            lpflCur->fBombed = 0x1;
                            lpflCur->fSkipped = 0x1;
                        }
                    }
                    lpflCur = lpflCur->lpflNext;
                } while (lpflCur != lpfl);
                if (ctokNew < 255) {
                    do {
                        if (lpflCur->fSkipped != 0x0) {
                            ctokFleet = 0;
                            iplrCur = lpflCur->iPlayer;
                            for (ishdef = 0; ishdef < 16; ishdef++) {
                                if (lpflCur->rgcsh[ishdef] != 0) {
                                    ctokFleet = ctokFleet + 1;
                                }
                            }
                            if (ctokNew + ctokFleet <= 255) {
                                lpflCur->fInclude = 0x1;
                                lpflCur->fBombed = 0x0;
                                lpflCur->fSkipped = 0x0;
                                rgctok[iplrCur] = rgctok[iplrCur] + LOBYTE(ctokFleet);
                                ctokNew = ctokNew + ctokFleet;
                            }
                        }
                        lpflCur = lpflCur->lpflNext;
                    } while (lpflCur != lpfl);
                }
            }
            *pgrfPlayer = grfPlayer;
            if (fChange == 0) {
                return cplr;
            }
            return -1;
        }
        return 0;
    }
    return 0;
}

int16_t SpdOfShip(FLEET *lpfl, int16_t ishdef, TOK *ptok, int16_t fDumpCargo, SHDEF *lpshdef) {
    int32_t  wtCargoFleetMax;
    int16_t  spd;
    int16_t  iWarp;
    uint16_t wt;
    int16_t  cHalfThruster;
    int16_t  cThruster;
    int32_t  wtFleetCargo;
    int16_t  j;
    int16_t  cEngineT;
    uint16_t wtCargoShdefMax;
    int16_t  iEngine;
    ENGINE  *lpengine;
    uint16_t t_scratch_m22;
    int16_t  t_merge_387e_0001;

    if (lpshdef == 0x0) {
        lpshdef = rglpshdef[lpfl->iPlayer] + ishdef;
    }
    iEngine = -1;
    cHalfThruster = 0;
    cThruster = 0;
    for (j = 0; j < lpshdef->hul.chs; j++) {
        if (lpshdef->hul.rghs[j].cItem != 0x0) {
            switch (lpshdef->hul.rghs[j].grhst) {
            default:
                break;
            case hstEngine:
                iEngine = lpshdef->hul.rghs[j].iItem;
                cEngineT = lpshdef->hul.rghs[j].cItem;
                if (iEngine != 8)
                    break;
                cHalfThruster = cHalfThruster + lpshdef->hul.rghs[j].cItem;
                break;
            case hstSpecialM:
                if (lpshdef->hul.rghs[j].iItem == 0x7) {
                    cThruster = cThruster + lpshdef->hul.rghs[j].cItem;
                    break;
                }
                if (lpshdef->hul.rghs[j].iItem != 0x8)
                    break;
                cThruster = cThruster + lpshdef->hul.rghs[j].cItem * 2;
                break;
            case hstSpecialE:
                if (lpshdef->hul.rghs[j].iItem != 0x4)
                    break;
                cThruster = cThruster + lpshdef->hul.rghs[j].cItem;
                break;
            case hstMining:
                if (lpshdef->hul.rghs[j].iItem == 0x6) {
                    cHalfThruster = cHalfThruster + lpshdef->hul.rghs[j].cItem;
                }
            }
        }
    }
    cThruster = cThruster + (int32_t)(cHalfThruster + 1) / 2;
    if (iEngine != -1 && cEngineT != 0) {
        lpengine = LpengineFromId(iEngine);
        switch (iEngine) {
        case 7:
        case 8:
        case 9:
        case 14:
        case 15:
            iWarp = 10;
            break;
        default:
            for (iWarp = 9; iWarp > 0 && lpengine->rgcFuelUsed[iWarp] > 120; iWarp--) {
            }
        }
        spd = iWarp - 4 + cThruster;
        if (lpfl != 0x0) {
            spd = spd + (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raAttack ? 1 : 0) * 2;
        }
        wt = lpshdef->hul.wtEmpty;
        if (lpfl != 0x0) {
            wtCargoShdefMax = WtMaxShdefStat(lpshdef, 2);
            if (wtCargoShdefMax == 0x0) {
                fDumpCargo = 0;
            } else {
                wtCargoFleetMax = LGetFleetStat(lpfl, 2);
                wtFleetCargo = lpfl->rgwtMin[0] + lpfl->rgwtMin[1] + lpfl->rgwtMin[2] + lpfl->rgwtMin[3];
                wtFleetCargo = (int32_t)((int32_t)(wtFleetCargo * (uint32_t)wtCargoShdefMax) / wtCargoFleetMax);
                wt = wt + LOWORD(wtFleetCargo);
            }
            if (fDumpCargo != 0) {
                spd = spd - 1;
            }
            t_scratch_m22 = Random(15);
            ptok->dwt = t_scratch_m22;
        }
        if (ptok != 0x0) {
            ptok->wt = wt;
        }
        spd = spd - (uint32_t)((uint32_t)wt / 0x46) / lpshdef->hul.rghs[0].cItem;
        if (0x0 <= (8 >= spd ? spd : 0x8)) {
            if (8 >= spd) {
                t_merge_387e_0001 = spd;
            } else {
                t_merge_387e_0001 = 8;
            }
        } else {
            t_merge_387e_0001 = 0;
        }
        spd = t_merge_387e_0001;
        return spd;
    }
    return 0;
}

SHDEF *LpshdefFromTok(TOK *ptok) {
    if (ptok->ishdef < 0x10) {
        return rglpshdef[ptok->iplr] + ptok->ishdef;
    }
    return rglpshdefSB[ptok->iplr] + (ptok->ishdef - 16);
}

int16_t FCanKillTok(TOK *ptok1, TOK *ptok2) {
    int32_t lp1;
    int32_t lp2;

    lp1 = LpshdefFromTok(ptok1)->lPower;
    lp2 = LpshdefFromTok(ptok2)->lPower;
    if (lp2 <= lp1) {
        if ((lp2 & 0x7ffff000) < (lp1 & 0x7ffff000)) {
            return 1;
        }
        if ((lp2 & 0x7fffff00) != (lp1 & 0x7fffff00) || ptok1->spd < ptok2->spd) {
            return 0;
        }
        return 1;
    }
    return 0;
}

void DoBattles(int16_t fPostMovement) {
    int16_t  cplr;
    int16_t  ifl;
    FLEET   *lpfl;
    uint16_t grfSpectator;
    uint16_t grfPlayer;
    uint16_t rggrfAttack[16];
    int16_t  t_call_3b1d;

    LinkFleets(fPostMovement);
    vrgtok = LpAlloc(256 * sizeof(TOK), htMisc);
    vlpwtCargo = LpAlloc(0x200, htMisc);
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        lpfl->fBombed = 0x0;
        if (lpfl->fDone == 0x0 && lpfl->fDead == 0x0 && lpfl->lpflNext != 0x0) {
            t_call_3b1d = CplrBattle(lpfl, rggrfAttack, &grfPlayer, &grfSpectator);
            cplr = t_call_3b1d;
            if (t_call_3b1d != -1 && t_call_3b1d != 0 && FDoCoolBattle(lpfl, cplr, rggrfAttack, grfPlayer, grfSpectator) != 0) {
            }
        }
    }
    FreeLp(vlpwtCargo, htMisc);
    FreeLp(vrgtok, htMisc);
    vlpwtCargo = 0x0;
    vrgtok = 0x0;
    if (lpbBattleT != 0x0) {
        RawStore16(lpbBattleT, 0xffff);
        FreeLp(lpbBattleT, htBattle);
        lpbBattleT = 0x0;
    }
    if (lpbBattleCur != 0x0) {
        RawStore16(lpbBattleCur, 0xffff);
    }
    DoBombing();
    return;
}

void RegenShield(TOK *ptok) {
    int32_t dpNew;
    int32_t dpOrig;

    dpOrig = DpShieldOfShdef(LpshdefFromTok(ptok), ptok->iplr);
    if (ptok->dpShield != 0x0) {
        dpNew = (uint32_t)ptok->dpShield + (int32_t)(dpOrig / 10);
        if (dpNew > dpOrig) {
            dpNew = dpOrig;
        }
        ptok->dpShield = LOWORD(dpNew);
    }
    return;
}

int16_t InitFromHuldef(HUL *lphul, int16_t *ppctBC) {
    int16_t ihs;
    int16_t i;
    int16_t pct;
    int16_t initBase;
    int16_t cbc;
    int16_t pctBC;
    PART    part;

    pct = 0;
    cbc = 0;
    initBase = LphuldefFromId(lphul->ihuldef)->init;
    for (ihs = 0; ihs < lphul->chs; ihs++) {
        part.hs = lphul->rghs[ihs];
        if (part.hs.cItem != 0x0) {
            if ((part.hs.grhst & 0x800) == 0x0) {
                if ((part.hs.grhst & 0x10) != 0x0 && part.hs.iItem == 0x12) {
                    pctBC = 10;
                    for (i = 0; i < part.hs.cItem; i++) {
                        pct = pct + (int32_t)((100 - pct) * pctBC) / 100;
                    }
                }
            } else {
                switch (part.hs.iItem) {
                default:
                    break;
                case 0x5:
                case 0x6:
                case 0x7:
                    FLookupPart(&part);
                    cbc = cbc + (part.hs.iItem - 0x4) * part.hs.cItem;
                    pctBC = part.pspecial->grAbility;
                    for (i = 0; i < part.hs.cItem; i++) {
                        pct = pct + (int32_t)((100 - pct) * pctBC) / 100;
                    }
                }
            }
        }
    }
    if (ppctBC != 0x0) {
        *ppctBC = pct;
    }
    initBase = initBase + cbc;
    if (initBase >= 64) {
        initBase = 63;
    }
    return initBase;
}

void CheckInitiative(TOK *ptok) {
    SHDEF  *lpshdef;
    int16_t pctBC;

    lpshdef = LpshdefFromTok(ptok);
    idPlayer = ptok->iplr;
    ptok->initBase = LOBYTE(InitFromHuldef(&lpshdef->hul, &pctBC));
    idPlayer = -1;
    ptok->pctBC = LOBYTE(pctBC);
    return;
}

void CheckWeapons(TOK *ptok, int16_t *pfDampeningField, uint8_t *pinit) {
    int16_t pctJam;
    int32_t ldp;
    int32_t pctBeamDef;
    int16_t ihs;
    int16_t initMac;
    int16_t init;
    int16_t dxyMax;
    int16_t i;
    int32_t pctCap;
    int16_t initMin;
    int32_t pctHit;
    SHDEF  *lpshdef;
    int16_t dxyLim;
    int16_t initBase;
    HUL    *lphul;
    int16_t dxyPart;
    PART    part;

    pctCap = 1000;
    pctBeamDef = 1000;
    pctHit = 10000;
    initBase = ptok->initBase;
    initMin = -1;
    initMac = -1;
    lpshdef = LpshdefFromTok(ptok);
    lphul = &lpshdef->hul;
    dxyMax = -1;
    dxyLim = -1;
    ldp = DpShieldOfShdef(lpshdef, ptok->iplr);
    for (ihs = 0; ihs < lphul->chs; ihs++) {
        if ((lphul->rghs[ihs].grhst & 0x18be) != 0x0 && lphul->rghs[ihs].cItem != 0x0) {
            pctJam = 100;
            dxyPart = -1;
            part.hs = lphul->rghs[ihs];
            if ((part.hs.grhst & 0x30) == 0x0) {
                init = -1;
            } else {
                idPlayer = ptok->iplr;
                FLookupPart(&part);
                idPlayer = -1;
                init = initBase + part.pbeam->init;
                if (init >= 64) {
                    init = 63;
                }
            }
            switch (part.hs.grhst) {
            case hstSpecialE:
                if (part.hs.iItem - 0x4 > 0xb)
                    break;
                switch (part.hs.iItem) {
                case 8:
                case 9:
                case 10:
                case 11:
                    idPlayer = ptok->iplr;
                    FLookupPart(&part);
                    idPlayer = -1;
                    pctJam = 100 - part.pspecial->grAbility;
                    break;
                case 4:
                    pctJam = 90;
                    break;
                case 14:
                    *pfDampeningField = 1;
                    break;
                case 15:
                    ptok->fDetector = 0x1;
                    break;
                case 12:
                case 13:
                    idPlayer = ptok->iplr;
                    FLookupPart(&part);
                    idPlayer = -1;
                    for (i = part.hs.cItem; i > 0; i--) {
                        pctCap = (int32_t)((int32_t)(pctCap * ((int32_t)part.pspecial->grAbility + 100)) / 0x64);
                    }
                case 5:
                case 6:
                case 7:
                }
                break;
            case hstSpecialM:
                if (part.hs.iItem != ispecialMBeamDeflector)
                    break;
                idPlayer = ptok->iplr;
                FLookupPart(&part);
                idPlayer = -1;
                for (i = part.hs.cItem; i > 0; i--) {
                    pctBeamDef = (int32_t)((int32_t)(pctBeamDef * (100 - (int32_t)part.pspecial->grAbility)) / 0x64);
                }
                break;
            case hstMining:
                if (part.hs.iItem != iminingAlienMiner)
                    break;
                pctJam = 70;
                break;
            case hstArmor:
                if (part.hs.iItem != iarmorMegaPolyShell)
                    break;
                pctJam = 80;
                break;
            case hstShield:
                if (part.hs.iItem != ishieldLangstonShell)
                    break;
                pctJam = 95;
                break;
            case hstBeam:
                dxyPart = part.pbeam->dRangeMax;
                break;
            case hstTorp:
                ptok->fTorp = 0x1;
                dxyPart = part.ptorp->dRangeMax;
            default:
            }
            if (pctJam < 100) {
                for (i = part.hs.cItem; i > 0; i--) {
                    pctHit = (uint32_t)(pctHit * (int32_t)pctJam);
                    pctHit = (int32_t)(pctHit / 100);
                }
            }
            if (dxyPart != -1) {
                if (ptok->grobj == grobjPlanet) {
                    dxyPart = dxyPart + 1;
                }
                if (dxyMax < 0 || dxyMax > dxyPart) {
                    dxyMax = dxyPart;
                }
                if (dxyPart > dxyLim) {
                    dxyLim = dxyPart;
                }
                pinit[init] = 0x1;
                if (initMin == -1 || init < initMin) {
                    initMin = init;
                }
                if (init > initMac) {
                    initMac = init;
                }
            }
        }
    }
    if (pctHit != 10000) {
        ptok->pctJam = LOBYTE(0x64 - LOWORD((int32_t)((pctHit + 50) / 0x64)));
        if (ptok->pctJam > 0x5f) {
            ptok->pctJam = 0x5f;
        }
    } else {
        ptok->pctJam = 0x0;
    }
    if (ptok->grobj == grobjPlanet) {
        ptok->pctJam = ptok->pctJam - LOBYTE((int32_t)ptok->pctJam / 0x4);
    }
    if (pctCap != 1000) {
        if (pctCap > 2550) {
            pctCap = 2550;
        }
        ptok->pctCap = LOBYTE(LOWORD((int32_t)(pctCap / 10)));
    }
    ptok->pctBeamDef = LOBYTE(LOWORD((int32_t)(pctBeamDef / 10)));
    ptok->dxyMax = dxyMax;
    ptok->dxyLim = dxyLim;
    ptok->initMin = LOBYTE(initMin);
    ptok->initMac = LOBYTE(initMac);
    if ((ldp & 0xffff0000) != 0x0) {
        ptok->dpShield = 0xffff;
    } else {
        ptok->dpShield = LOWORD(ldp);
    }
    return;
}

void RandomizeTokOrder() {
    TOK     tok;
    int16_t itokSwap;
    int16_t itok;

    itokSwap = -1;
    for (itok = 0; itok < vctok; itok++) {
        itokSwap = Random(vctok - itok) + itok;
        if (itokSwap != itok) {
            tok = vrgtok[itokSwap];
            vrgtok[itokSwap] = vrgtok[itok];
            vrgtok[itok] = tok;
        }
    }
    return;
}

void InitializeBoard(FLEET *lpfl, int16_t ibrc, uint16_t grfPlayer, uint8_t *pinit, int16_t *pinitMin, int16_t *pinitMac) {
    int16_t   iplr;
    FLEET    *lpflCur;
    TOK      *ptok;
    int16_t   initMac;
    PLANET   *lppl;
    int16_t   fDampeningField;
    int16_t   initMin;
    uint16_t *lpwtCargoCur;
    TOK      *ptokT;
    uint8_t   mpiplrdibrc[16];
    int16_t   fDumpCargo;
    int16_t   ishdef;
    uint8_t   rgfTorp[16];
    int16_t   t_461c;
    uint16_t  t_scratch_m44_2;
    uint16_t  t_merge_4b51_0001;

    initMin = -1;
    initMac = -1;
    fDampeningField = 0;
    lpwtCargoCur = vlpwtCargo;
    ishdef = 0;
    memset(mpiplrdibrc, 255, 0x10);
    for (iplr = 0; iplr < game.cPlayer; iplr++) {
        if ((0x1 << iplr & grfPlayer) != 0x0) {
            t_461c = ishdef;
            ishdef = ishdef + 1;
            mpiplrdibrc[iplr] = LOBYTE(t_461c);
        }
    }
    memset(rgfTorp, 0, 0x10);
    lpflCur = lpfl;
    ptok = vrgtok;
    if (lpfl->idPlanet != -1) {
        lppl = LpplFromId(lpfl->idPlanet);
        iplr = lppl->iPlayer;
        if (iplr != -1 && lppl->fStarbase != 0x0 && (0x1 << iplr & grfPlayer) != 0x0) {
            ptok->grobj = grobjPlanet;
            lppl->fNoHeal = 0x1;
            ptok->brc = rgbrcStart[mpiplrdibrc[iplr] + ibrc];
            ptok->id = lppl->id;
            ptok->iplr = LOBYTE(iplr);
            ptok->csh = 0x1;
            ptok->ishdef = LOBYTE(lppl->isb + 0x10);
            CheckInitiative(ptok);
            CheckWeapons(ptok, &fDampeningField, pinit);
            rgfTorp[iplr] = rgfTorp[iplr] | LOBYTE(ptok->fTorp);
            if (ptok->initBase != 0xff) {
                ptok->mdTarget0 = 0x3;
            } else {
                ptok->mdTarget0 = 0x5;
            }
            ptok->mdTarget1 = 0x1;
            ptok->mdTarget2 = 0x1;
            ptok->dv.pctDp = lppl->pctDp;
            ptok->mdTactic = 0x5;
            if (ptok->dv.pctDp != 0x0) {
                ptok->dv.pctSh = 0x64;
            }
            ptok->spd = 0x0;
            ptok->wt = 0xffff;
            ptok = ptok + 1;
        }
    }
    do {
        if (lpflCur->fDead == 0x0) {
            if (lpflCur->fSkipped == 0x0) {
                if (lpflCur->fInclude != 0x0) {
                    lpflCur->fNoHeal = 0x1;
                    iplr = lpflCur->iPlayer;
                    fDumpCargo = FDumpCargo(lpflCur);
                    for (ishdef = 0; ishdef < 16; ishdef++) {
                        if (lpflCur->rgcsh[ishdef] != 0) {
                            ptok->grobj = grobjFleet;
                            ptok->brc = rgbrcStart[mpiplrdibrc[iplr] + ibrc];
                            ptok->id = lpflCur->id;
                            ptok->iplr = LOBYTE(lpflCur->iplr);
                            ptok->ishdef = LOBYTE(ishdef);
                            ptok->csh = lpflCur->rgcsh[ishdef];
                            ptok->dv.dp = lpflCur->rgdv[ishdef].dp;
                            CheckInitiative(ptok);
                            CheckWeapons(ptok, &fDampeningField, pinit);
                            rgfTorp[iplr] = rgfTorp[iplr] | LOBYTE(ptok->fTorp);
                            CheckTarget(ptok, lpflCur, ishdef);
                            t_scratch_m44_2 = SpdOfShip(lpflCur, ishdef, ptok, fDumpCargo, 0x0);
                            ptok->spd = t_scratch_m44_2;
                            ptok = ptok + 1;
                            if ((int32_t)((uint8_t *)ptok - (uint8_t *)vrgtok) / 0x1d > 0xff)
                                goto LTooManyTokens;
                        }
                    }
                }
            } else {
                grfMissed = grfMissed | 0x1 << lpflCur->iPlayer;
            }
        }
        lpflCur = lpflCur->lpflNext;
    } while (lpflCur != lpfl);
LTooManyTokens:
    vctok = (int32_t)((uint8_t *)ptok - (uint8_t *)vrgtok) / 29;
    RandomizeTokOrder();
    for (ptokT = vrgtok; ptokT < ptok; ptokT++) {
        ptokT->fMoved = 0x1;
        ptokT->fActive = 0x1;
        if (ptokT->initMin == 0xff) {
            ptokT->mdTarget1 = 0x0;
        }
        if (ptokT->dpShield == 0x0 || GetRaceGrbit(&rgplr[ptokT->iplr], ibitRaceRegeneratingShields) == 0) {
            t_merge_4b51_0001 = 0x0;
        } else {
            t_merge_4b51_0001 = 0x1;
        }
        ptokT->fRegen = t_merge_4b51_0001;
        if (fDampeningField != 0 && ptokT->grobj != grobjPlanet) {
            ptokT->spd = ptokT->spd - 0x4 <= 0x0 ? 0x0 : ptokT->spd - 0x4;
        }
        *(TOK *)lpbBattleCur = *ptokT;
        lpbBattleCur = lpbBattleCur + 29;
        if (ptokT->initMin != 0xff && (initMin == -1 || ptokT->initMin < initMin)) {
            initMin = ptokT->initMin;
        }
        if (ptokT->initMac != 0xff && (initMac == -1 || ptokT->initMac > initMac)) {
            initMac = ptokT->initMac;
        }
    }
    *pinitMin = initMin & 0xff;
    *pinitMac = initMac & 0xff;
    return;
}

int16_t DzFromBrcBrc(uint8_t brc1, uint8_t brc2) {
    int16_t dy;
    int16_t dx;

    dx = (brc1 & 0xf) - (brc2 & 0xf);
    dx = abs(dx);
    dy = (brc1 >> 0x4) - (brc2 >> 0x4);
    dy = abs(dy);
    if (dx <= dy) {
        return dy;
    }
    return dx;
}

int32_t DpFromPtokBrcToBrc(TOK *ptok, uint8_t brcSrc, uint8_t brcTarget, TOK *ptokTarget, int16_t fProximity) {
    int16_t dz;
    int32_t dpMax;
    int32_t dpShdef;
    int16_t ihs;
    int32_t cTorpBase;
    int32_t dpTotal;
    int16_t fOutOfRange;
    int32_t dRange;
    HUL    *lphul;
    int32_t cTorpHit;
    int32_t dp;
    PART    part;
    int32_t dpShieldsLeft;
    int16_t t_merge_4e8b_0001;

    dz = DzFromBrcBrc(brcSrc, brcTarget);
    dpTotal = 0;
    if (fProximity != 0 || dz <= ptok->dxyLim) {
        lphul = &LpshdefFromTok(ptok)->hul;
        for (ihs = 0; ihs < lphul->chs; ihs++) {
            if ((lphul->rghs[ihs].grhst & 0x30) != 0x0 && lphul->rghs[ihs].cItem != 0x0) {
                part.hs = lphul->rghs[ihs];
                idPlayer = ptok->iplr;
                FLookupPart(&part);
                idPlayer = -1;
                dRange = (int32_t)((ptok->grobj == grobjPlanet ? 0x1 : 0x0) + part.pbeam->dRangeMax);
                t_merge_4e8b_0001 = dRange < (int32_t)dz ? 1 : 0;
                fOutOfRange = t_merge_4e8b_0001;
                if (fOutOfRange == 0 || fProximity != 0) {
                    dp = (uint32_t)((int32_t)part.pbeam->dp * part.hs.cItem);
                    if (part.hs.grhst == hstBeam) {
                        if (ptok->pctCap != 0x0) {
                            dp = (int32_t)((int32_t)(dp * (int32_t)ptok->pctCap) / 0x64);
                        }
                        if (dz > 0 && dRange > 0) {
                            dp = dp - (int32_t)((int32_t)((int32_t)(dp * (int32_t)dz) / 0xa) / dRange);
                        }
                        if (ptokTarget->pctBeamDef < 0x64) {
                            dp = (int32_t)((int32_t)(dp * (int32_t)ptokTarget->pctBeamDef) / 0x64);
                        }
                        if ((part.pbeam->grfAbilities & 0x1) != 0x0) {
                            dpShieldsLeft = (uint32_t)((uint32_t)ptokTarget->dpShield * (uint32_t)ptok->csh);
                            if (dp > dpShieldsLeft) {
                                dp = dpShieldsLeft;
                            }
                        }
                        if (fOutOfRange != 0) {
                            dp = (int32_t)(dp / ((int32_t)(dz + 10) - dRange));
                            if (dp < part.hs.cItem) {
                                dp = part.hs.cItem;
                            }
                        }
                        dpTotal = dpTotal + (uint32_t)(dp * (uint32_t)ptok->csh);
                    } else if (part.hs.grhst == hstTorp) {
                        cTorpBase = (uint32_t)((uint32_t)(part.hs.cItem * (uint32_t)ptok->csh) * 0xc8);
                        cTorpHit = CTorpHit(cTorpBase, ptokTarget, part.ptorp->dHitChance, ptok->pctBC);
                        dp = (int32_t)((int32_t)((int32_t)part.ptorp->dp * cTorpHit) / 0xc8);
                        if (ptokTarget->dpShield > 0x0) {
                            dp = dp + (int32_t)((int32_t)((cTorpBase - cTorpHit) * (int32_t)part.ptorp->dp) / 0x640);
                        }
                        if (fOutOfRange != 0) {
                            dp = (int32_t)(dp / ((int32_t)(dz + 10) - dRange));
                            if (dp < part.hs.cItem) {
                                dp = part.hs.cItem;
                            }
                        }
                        dpTotal = dpTotal + dp;
                    }
                }
            }
        }
        dpShdef = (uint32_t)LpshdefFromTok(ptokTarget)->hul.dp;
        dpMax = (uint32_t)(((uint32_t)ptokTarget->dpShield + dpShdef) * (uint32_t)ptokTarget->csh);
        if (ptokTarget->dv.dp != 0x0 && dpMax > 0) {
            dpMax = dpMax - (int32_t)((int32_t)((int32_t)((int32_t)((int32_t)((int32_t)(dpShdef * ptokTarget->dv.pctDp) / 0xa) * ptokTarget->dv.pctSh) / 0xa) *
                                                (uint32_t)ptokTarget->csh) /
                                      0x1f4);
            if (dpMax <= 0) {
                dpMax = 1;
            }
        }
        if (dpTotal > dpMax && fProximity == 0) {
            dpTotal = dpMax;
        }
        return dpTotal;
    }
    return 0;
}

int16_t DzMoveRangeToConsider(TOK *ptok, uint16_t grfAttack, uint8_t *pbrc) {
    int16_t  dzNonSapper;
    uint8_t  dz;
    int16_t  iplr;
    uint16_t mdTarget;
    uint8_t  dzBest;
    int16_t  itokLook;
    int16_t  iplrTarget;
    TOK     *ptokTarget;
    int16_t  dzMax;
    uint8_t  brcCur;
    int16_t  ihs;
    SHDEF   *lpshdef;
    HUL     *lphul;
    PART     part;
    uint16_t t_scratch_m1a;

    brcCur = ptok->brc;
    dzMax = ptok->dxyLim + ptok->dMovesLeft;
    mdTarget = FDoesPrimaryTargetTypeExist(ptok, grfAttack) == 0 ? ptok->mdTarget2 : ptok->mdTarget1;
    iplr = ptok->iplr;
    dzBest = 0xa;
    *pbrc = 0xff;
    if (ptok->dxyLim != 0x3) {
        dzNonSapper = ptok->dxyLim;
    } else {
        dzNonSapper = -1;
        lpshdef = LpshdefFromTok(ptok);
        lphul = &lpshdef->hul;
        for (ihs = 0; ihs < lphul->chs; ihs++) {
            if (lphul->rghs[ihs].grhst == hstBeam && lphul->rghs[ihs].cItem != 0x0) {
                part.hs = lphul->rghs[ihs];
                idPlayer = ptok->iplr;
                FLookupPart(&part);
                idPlayer = -1;
                if ((part.pbeam->grfAbilities & 0x1) == 0x0 && part.pbeam->dRangeMax > dzNonSapper) {
                    dzNonSapper = part.pbeam->dRangeMax;
                }
            }
        }
    }
    if (ptok->dxyMax < ptok->dxyLim && (ptok->mdTactic == 0x5 || ptok->mdTactic == 0x3)) {
        dzMax = ptok->dxyMax + ptok->dMovesLeft;
    }
    ptokTarget = vrgtok;
    itokLook = 0;
    while (1) {
        if (itokLook >= vctok) {
            return 1;
        }
        iplrTarget = ptokTarget->iplr;
        if (iplrTarget != iplr && (0x1 << iplrTarget & grfAttack) != 0x0 && ptokTarget->fActive != 0x0 && FIsTargetOfMdTarget(ptokTarget, mdTarget) != 0) {
            dz = LOBYTE(DzFromBrcBrc(brcCur, ptokTarget->brc));
            if (ptokTarget->dMovesLeft >= ptok->dMovesLeft) {
                dz = dz + 0x1;
            }
            if (dz <= dzMax && (ptokTarget->dpShield > 0x0 || dzNonSapper == ptok->dxyLim || dz <= dzNonSapper + ptok->dMovesLeft))
                break;
            t_scratch_m1a = dz;
            if (t_scratch_m1a < dzBest && DpFromPtokBrcToBrc(ptok, 0x0, 0x0, ptokTarget, 0) > 0) {
                dzBest = dz;
                *pbrc = ptokTarget->brc;
            }
        }
        itokLook = itokLook + 1;
        ptokTarget = ptokTarget + 1;
    }
    *pbrc = 0xff;
    return ptok->dMovesLeft;
}

int16_t FDoesPrimaryTargetTypeExist(TOK *ptok, uint16_t grfAttack) {
    uint16_t mdTarget;
    int16_t  iplr;
    int16_t  iplrLook;
    TOK      tok;
    int16_t  itokLook;

    iplr = ptok->iplr;
    mdTarget = ptok->mdTarget1;
    if (mdTarget != 0x0) {
        for (itokLook = 0; itokLook < vctok; itokLook++) {
            iplrLook = vrgtok[itokLook].iplr;
            if (iplrLook != iplr && (0x1 << iplrLook & grfAttack) != 0x0) {
                tok = vrgtok[itokLook];
                if (tok.fActive != 0x0 && mdTarget - 0x1 <= 0x6) {
                    switch (mdTarget) {
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        switch (mdTarget) {
                        case 0x3:
                        case 0x6:
                        case 0x7:
                            if (tok.mdTarget0 != mdTarget)
                                continue;
                            break;
                        case 0x5:
                            if (tok.mdTarget0 < mdTarget)
                                continue;
                            break;
                        case 0x4:
                            if (tok.mdTarget0 != 0x4 && tok.mdTarget0 != 0x7)
                                continue;
                        default:
                        }
                    case 1:
                        return 1;
                    case 2:
                        if (tok.grobj == grobjPlanet) {
                            return 1;
                        }
                    }
                }
            }
        }
        return 0;
    }
    return 0;
}

int16_t FIsTargetOfMdTarget(TOK *ptok, int16_t mdTarget) {
    if ((uint16_t)mdTarget <= 7) {
        switch (mdTarget) {
        case 0:
            break;
        case 1:
            return 1;
        case 2:
            if (ptok->grobj != grobjPlanet) {
                return 0;
            }
            return 1;
        case 4:
            if (ptok->mdTarget0 != 0x4 && ptok->mdTarget0 != 0x7) {
                return 0;
            }
            return 1;
        case 3:
        case 6:
        case 7:
            if (ptok->mdTarget0 != mdTarget) {
                return 0;
            }
            return 1;
        case 5:
            switch (ptok->mdTarget0) {
            case 0x5:
            case 0x7:
            case 0x6:
                return 1;
            default:
                return 0;
            }
        }
    }
    return 0;
}

int32_t ScoreGuessBattleDamage(TOK *ptokSrc, uint8_t brc, int16_t fPrimary, uint16_t grfAttack) {
    int16_t  iBest;
    int16_t  dMoves;
    int16_t  rgy[2];
    TOK     *ptok;
    int16_t  yEnemy;
    int16_t  dzEnemy;
    int32_t  dpGivenBest;
    int32_t  dpTakenBest;
    int16_t  y;
    int32_t  dpTakenTotal;
    int32_t  dpGivenCur;
    int16_t  i;
    int16_t  xEnemy;
    int16_t  yCur;
    int32_t  dpTaken;
    int32_t  scoreThemBest;
    int32_t  scoreThem;
    int16_t  dzCur;
    int16_t  rgx[2];
    uint8_t  brcEnemy;
    int32_t  dpGiven;
    int16_t  dMax;
    int32_t  scoreUs;
    uint8_t  iplrSrc;
    int16_t  fWeAttack;
    int16_t  xCur;
    int16_t  x;
    int16_t  dMin;
    int16_t  itok;
    uint16_t t_scratch_m56;
    uint16_t t_merge_5b99_0001;
    uint16_t t_merge_5bfb_0001;

    iplrSrc = ptokSrc->iplr;
    xCur = ptokSrc->brc & 0xf;
    yCur = ptokSrc->brc >> 0x4;
    dpGivenBest = 0;
    dpTakenTotal = 0;
    ptok = vrgtok;
    for (itok = 0; itok < vctok; itok++) {
        if (ptok->fActive != 0x0) {
            t_scratch_m56 = iplrSrc;
            if (t_scratch_m56 != ptok->iplr && (0x1 << ptok->iplr & grfAttack) != 0x0) {
                dzCur = DzFromBrcBrc(ptok->brc, brc);
                dMoves = ptok->dMovesLeft < ptokSrc->dMovesLeft ? 0 : 1;
                if (dMoves != 0) {
                    dMin = 0 <= dzCur - dMoves ? dzCur - dMoves : 0;
                    xEnemy = ptok->brc & 0xf;
                    yEnemy = ptok->brc >> 0x4;
                    rgx[0] = xEnemy - dMoves;
                    rgx[1] = xEnemy + dMoves;
                    rgy[0] = yEnemy - dMoves;
                    rgy[1] = yEnemy + dMoves;
                    dMax = dzCur;
                    for (x = 0; x < 2; x++) {
                        for (y = 0; y < 2; y++) {
                            if (0x9 >= (0 <= rgx[x] ? rgx[x] : 0x0)) {
                                if (0 <= rgx[x]) {
                                    t_merge_5b99_0001 = rgx[x];
                                } else {
                                    t_merge_5b99_0001 = 0x0;
                                }
                            } else {
                                t_merge_5b99_0001 = 0x9;
                            }
                            if (0x9 >= (0 <= rgy[y] ? rgy[y] : 0x0)) {
                                if (0 <= rgy[y]) {
                                    t_merge_5bfb_0001 = rgy[y];
                                } else {
                                    t_merge_5bfb_0001 = 0x0;
                                }
                            } else {
                                t_merge_5bfb_0001 = 0x9;
                            }
                            brcEnemy = LOBYTE((t_merge_5bfb_0001 & 0xf) << 0x4 | (t_merge_5b99_0001 & 0xf));
                            dzEnemy = DzFromBrcBrc(brc, brcEnemy);
                            if (dzEnemy > dMax) {
                                dMax = dzEnemy;
                            }
                        }
                    }
                } else {
                    dMax = dzCur;
                    dMin = dzCur;
                }
                fWeAttack = FIsTargetOfMdTarget(ptok, fPrimary == 0 ? ptokSrc->mdTarget2 : ptokSrc->mdTarget1);
                scoreThemBest = 30000000;
                iBest = dMin;
                for (i = dMin; i <= dMax; i++) {
                    if (fWeAttack == 0) {
                        dpGiven = 0;
                    } else {
                        dpGiven = DpFromPtokBrcToBrc(ptokSrc, 0x0, (i & 0xf) << 0x4 & 0xff, ptok, 0);
                    }
                    dpTaken = DpFromPtokBrcToBrc(ptok, 0x0, (i & 0xf) << 0x4 & 0xff, ptokSrc, ptokSrc->mdTactic == 0x0 ? 1 : 0);
                    scoreThem = ScoreFromGiveAndTakeAndTactic(dpTaken, dpGiven, ptok->mdTactic);
                    if (scoreThem <= scoreThemBest) {
                        scoreThemBest = scoreThem;
                        iBest = i;
                        dpTakenBest = dpTaken;
                        dpGivenCur = dpGiven;
                    }
                }
                if (dpGivenCur > dpGivenBest) {
                    dpGivenBest = dpGivenCur;
                }
                dpTakenTotal = dpTakenTotal + dpTakenBest;
            }
        }
        ptok = ptok + 1;
    }
    scoreUs = ScoreFromGiveAndTakeAndTactic(dpGivenBest, dpTakenTotal, ptokSrc->mdTactic);
    return scoreUs;
}

int32_t ScoreFromGiveAndTakeAndTactic(int32_t dpGive, int32_t dpTake, int16_t mdTactic) {
    int32_t score;

    if ((uint16_t)mdTactic > 5) {
        return 0;
    }
    switch (mdTactic) {
    case 0:
    case 2:
        return dpTake;
    case 1:
    case 5:
        return -dpGive;
    case 3:
    case 4:
        score = -dpGive;
        if (score != 0) {
            score = (int32_t)((int32_t)(score * 100) / (dpTake + 1));
            if (score >= 0) {
                score = -1;
            }
        } else {
            score = dpTake;
        }
        return score;
    }
}

int16_t DxyMoveTokTo(TOK *ptok, int16_t spdMove, uint16_t grfAttack) {
    uint16_t iplr;
    int16_t  xMax;
    int16_t  dz;
    int32_t  scoreBest;
    uint8_t  brc;
    int32_t  rgscoreNear[3][3];
    int32_t  score;
    int16_t  cBest;
    int16_t  yMin;
    int16_t  dy;
    int16_t  mdTactic;
    int16_t  y;
    uint8_t  brcOOR;
    int16_t  i;
    int16_t  yCur;
    uint8_t  brcBest;
    int16_t  dzAwayBest;
    int16_t  yMax;
    int16_t  dx;
    int16_t  xCur;
    int16_t  fPrimary;
    int32_t  dp;
    int16_t  dzAway;
    int16_t  x;
    int32_t  lLow;
    int16_t  cLow;
    int16_t  fXMajor;
    POINT16  rgptDeltas[2];
    uint16_t t_scratch_m5c;
    uint16_t t_scratch_m5c_2;
    int16_t  t_scratch_m5c_3;
    int16_t  t_643a;
    int16_t  t_6576;
    int16_t  t_scratch_m66;

    iplr = ptok->iplr;
    dp = 0;
    xCur = ptok->brc & 0xf;
    yCur = ptok->brc >> 0x4;
    if (ptok->grobj != grobjPlanet && spdMove != 0) {
        scoreBest = 30000000;
        mdTactic = ptok->mdTactic;
        fPrimary = FDoesPrimaryTargetTypeExist(ptok, grfAttack);
        for (x = 0; x < 3; x++) {
            for (y = 0; y < 3; y++) {
                rgscoreNear[x][y] = 30000000;
            }
        }
        dz = DzMoveRangeToConsider(ptok, grfAttack, &brcOOR);
        x = xCur - dz;
        if (x < 0) {
            x = 0;
        }
        yMin = yCur - dz;
        if (yMin < 0) {
            yMin = 0;
        }
        xMax = xCur + dz;
        if (xMax >= 10) {
            xMax = 9;
        }
        yMax = yCur + dz;
        if (yMax >= 10) {
            yMax = 9;
        }
        for (; x <= xMax; x++) {
            for (y = yMin; y <= yMax; y++) {
                brc = LOBYTE((y & 0xf) << 0x4 | (x & 0xf));
                dx = xCur - x;
                dy = yCur - y;
                dx = abs(dx);
                dy = abs(dy);
                score = ScoreGuessBattleDamage(ptok, brc, fPrimary, grfAttack);
                if (mdTactic == 0) {
                    for (i = 0; i < vctok; i++) {
                        t_scratch_m5c = vrgtok[i].brc;
                        if (t_scratch_m5c == brc && vrgtok[i].iplr == iplr) {
                            score = score + 2;
                        }
                    }
                    t_scratch_m5c_2 = brc;
                    if (t_scratch_m5c_2 == ptok->brc) {
                        score = score - 1;
                    }
                }
                dzAway = DzFromBrcBrc(ptok->brc, brc);
                if (dzAway <= 1) {
                    rgscoreNear[x - xCur + 1][y - yCur + 1] = score;
                }
                if (score < scoreBest || (score == scoreBest && dzAway <= dzAwayBest)) {
                    if (score != scoreBest || dzAway != dzAwayBest) {
                        cBest = 1;
                        scoreBest = score;
                        dzAwayBest = dzAway;
                    } else {
                        cBest = cBest + 1;
                        if (Random(cBest) != 0)
                            continue;
                    }
                    brcBest = brc;
                }
            }
        }
        if (brcOOR != 0xff) {
            brcBest = brcOOR;
        }
        dzAway = DzFromBrcBrc(ptok->brc, brcBest);
        if (dzAway > 1) {
            dx = (brcBest & 0xf) - xCur;
            dy = (brcBest >> 0x4) - yCur;
            t_scratch_m5c_3 = abs(dx);
            if (t_scratch_m5c_3 != abs(dy)) {
                if (dx != 0) {
                    if (dy != 0) {
                        t_scratch_m66 = abs(dx);
                        fXMajor = t_scratch_m66 <= abs(dy) ? 0 : 1;
                        dx = dx <= 0 ? 0 : 2;
                        dy = dy <= 0 ? 0 : 2;
                        rgptDeltas[0].x = dx;
                        rgptDeltas[0].y = dy;
                        if (fXMajor == 0) {
                            rgptDeltas[1].x = 1;
                            rgptDeltas[1].y = dy;
                        } else {
                            rgptDeltas[1].x = dx;
                            rgptDeltas[1].y = 1;
                        }
                        if (rgscoreNear[rgptDeltas[0].x][rgptDeltas[0].y] >= (int32_t)rgscoreNear[rgptDeltas[1].x][rgptDeltas[1].y] &&
                            (rgscoreNear[rgptDeltas[0].x][rgptDeltas[0].y] != rgscoreNear[rgptDeltas[1].x][rgptDeltas[1].y] || Random(2) != 0)) {
                            i = 1;
                        } else {
                            i = 0;
                        }
                        xCur = xCur + (rgptDeltas[i].x - 1);
                        yCur = yCur + (rgptDeltas[i].y - 1);
                    } else {
                        lLow = 300000000;
                        cLow = 0;
                        dx = dx >= 0 ? 2 : 0;
                        xCur = xCur + (dx - 1);
                        for (i = 0; i < 3; i++) {
                            if (rgscoreNear[dx][i] <= lLow) {
                                if (rgscoreNear[dx][i] < lLow) {
                                    lLow = rgscoreNear[dx][i];
                                    cLow = 1;
                                } else {
                                    cLow = cLow + 1;
                                }
                            }
                        }
                        x = Random(cLow);
                        for (i = 0; i < 3; i++) {
                            if (rgscoreNear[dx][i] == lLow) {
                                t_6576 = x;
                                x = x - 1;
                                if (t_6576 == 0)
                                    break;
                            }
                        }
                        yCur = yCur + (i - 1);
                    }
                } else {
                    lLow = 300000000;
                    cLow = 0;
                    dy = dy >= 0 ? 2 : 0;
                    yCur = yCur + (dy - 1);
                    for (i = 0; i < 3; i++) {
                        if (rgscoreNear[i][dy] <= lLow) {
                            if (rgscoreNear[i][dy] < lLow) {
                                lLow = rgscoreNear[i][dy];
                                cLow = 1;
                            } else {
                                cLow = cLow + 1;
                            }
                        }
                    }
                    x = Random(cLow);
                    for (i = 0; i < 3; i++) {
                        if (rgscoreNear[i][dy] == lLow) {
                            t_643a = x;
                            x = x - 1;
                            if (t_643a == 0)
                                break;
                        }
                    }
                    xCur = xCur + (i - 1);
                }
            } else {
                if (dx <= 0) {
                    xCur = xCur - 1;
                } else {
                    xCur = xCur + 1;
                }
                if (dy <= 0) {
                    yCur = yCur - 1;
                } else {
                    yCur = yCur + 1;
                }
            }
            brcBest = LOBYTE((yCur & 0xf) << 0x4 | (xCur & 0xf));
        }
        if (scoreBest != 30000000) {
            if ((brcBest & 0xf) > 0x9 || brcBest >> 0x4 > 9) {
                brcBest = ptok->brc;
            }
            ptok->brc = brcBest;
        }
    }
    ptok->fMoved = 0x1;
    return 1;
}

int32_t CTorpHit(int32_t cTorpBase, TOK *ptok, int16_t pctBase, int16_t pctBC) {
    int32_t pctJam;
    int16_t i;
    int32_t pctHit;
    int32_t cTorpHit;
    int16_t t_scratch_m12;

    if (cTorpBase != 0 && pctBase != 0) {
        pctJam = (uint32_t)ptok->pctJam;
        if (pctJam != 0 && pctBC != 0) {
            pctJam = pctJam - (int32_t)pctBC;
            if (pctJam < 0) {
                pctBC = -LOWORD(pctJam);
                pctJam = 0;
            } else {
                pctBC = 0;
            }
        }
        if (pctBC == 0) {
            if (pctJam != 0) {
                pctHit = (int32_t)((int32_t)((int32_t)pctBase * (100 - pctJam)) / 0x64);
            } else {
                pctHit = (int32_t)pctBase;
            }
        } else {
            pctHit = 100 - (int32_t)((int32_t)((100 - (int32_t)pctBase) * (int32_t)(100 - pctBC)) / 0x64);
        }
        if (pctHit < 1) {
            pctHit = 1;
        }
        if (pctHit < 100) {
            if (cTorpBase <= 200) {
                cTorpHit = 0;
                for (i = 0; (int32_t)i < cTorpBase; i++) {
                    t_scratch_m12 = Random(100);
                    if (t_scratch_m12 < LOWORD(pctHit)) {
                        cTorpHit = cTorpHit + 1;
                    }
                }
            } else {
                cTorpHit = (int32_t)((int32_t)(cTorpBase * pctHit) / 100);
            }
            return cTorpHit;
        }
        return cTorpBase;
    }
    return 0;
}

int16_t FAttack(int16_t itokAttacker, int16_t init, BTLREC *lpbtlrec, uint16_t grfAttack) {
    int32_t  dpShieldLeft;
    int16_t  dz;
    SHDEF   *lpshdefE;
    int32_t  dpArmorLeft;
    int32_t  dpSingle;
    int32_t  scoreBest;
    TOK     *ptok;
    int16_t  ctokDamaged;
    int16_t  itokTarget;
    int32_t  dpMain;
    int32_t  score;
    int16_t  fSetItok;
    int16_t  dxRangeCur;
    int16_t  ihs;
    int32_t  cTorpMiss;
    int32_t  cTorpFire;
    int32_t  cTorpsLeft;
    int16_t  i;
    int32_t  cTorpBase;
    uint16_t grfWeapon;
    int16_t  cItem;
    int32_t  pctHit;
    TOK     *ptokTarget;
    SHDEF   *lpshdef;
    int32_t  lValue;
    int32_t  dpT;
    HUL     *lphul;
    int32_t  cTorpHit;
    int16_t  fPrimary;
    int32_t  dp;
    int16_t  itok;
    int32_t  dpCol;
    TOK     *ptokE;
    PART     part;
    int32_t  nds;
    int16_t  fCapMissile;
    int32_t  nts;
    int32_t  ntk;
    int32_t  dpShieldCur;
    int32_t  dpHitArmor;
    uint16_t t_scratch_m7a;
    uint16_t t_scratch_m7a_2;
    uint16_t t_scratch_m7a_3;
    uint16_t t_scratch_m7a_4;
    uint16_t t_scratch_m7a_5;
    int16_t  t_merge_7293_0001;
    int32_t  t_merge_7484_0001;
    int32_t  t_merge_77fb_0001;

    dxRangeCur = 0;
    fSetItok = 0;
    ctokDamaged = 0;
    ptok = vrgtok + itokAttacker;
    lpshdef = LpshdefFromTok(ptok);
    lphul = &lpshdef->hul;
    for (ihs = 0; ihs < lphul->chs; ihs++) {
        if ((lphul->rghs[ihs].grhst & 0x30) != 0x0 && lphul->rghs[ihs].cItem != 0x0) {
            part.hs = lphul->rghs[ihs];
            idPlayer = ptok->iplr;
            if (FLookupPart(&part) == 0) {
            }
            idPlayer = -1;
            cItem = lphul->rghs[ihs].cItem;
            i = ptok->initBase + part.pbeam->init;
            if (i >= 64) {
                i = 63;
            }
            if (i == init) {
                dxRangeCur = (ptok->grobj == grobjPlanet ? 1 : 0) + part.pbeam->dRangeMax;
                if (part.hs.grhst != hstBeam || (part.pbeam->grfAbilities & 0x2) == 0x0) {
                    if (part.hs.grhst != hstBeam) {
                        dpMain = 0;
                        cTorpsLeft = (uint32_t)((int32_t)cItem * (uint32_t)ptok->csh);
                    } else {
                        dpMain = (uint32_t)((uint32_t)((int32_t)part.pbeam->dp * (int32_t)cItem) * (uint32_t)ptok->csh);
                        cTorpsLeft = 0;
                    }
                    do {
                        for (fPrimary = 1; fPrimary >= 0; fPrimary--) {
                            scoreBest = 0;
                            ptokTarget = 0x0;
                            ptokE = vrgtok;
                            for (itok = 0; itok < vctok; itok++) {
                                if (ptokE->fActive != 0x0) {
                                    t_scratch_m7a_2 = ptokE->iplr;
                                    if (t_scratch_m7a_2 != ptok->iplr && (0x1 << ptokE->iplr & grfAttack) != 0x0 &&
                                        DzFromBrcBrc(ptokE->brc, ptok->brc) <= dxRangeCur &&
                                        FIsTargetOfMdTarget(ptokE, fPrimary == 0 ? ptok->mdTarget2 : ptok->mdTarget1) != 0) {
                                        lpshdefE = LpshdefFromTok(ptokE);
                                        lValue = (uint32_t)(((uint32_t)lpshdefE->hul.resCost + (uint32_t)lpshdefE->hul.rgwtOreCost[1]) * (uint32_t)ptokE->csh);
                                        if (lValue < 100000) {
                                            lValue = (uint32_t)(lValue * 100);
                                        } else {
                                            lValue = 10000000;
                                        }
                                        dpSingle = (uint32_t)lpshdefE->hul.dp;
                                        dpShieldLeft = (uint32_t)((uint32_t)ptokE->dpShield * (uint32_t)ptokE->csh);
                                        dpArmorLeft = (uint32_t)(dpSingle * (uint32_t)ptokE->csh);
                                        if (ptokE->dv.dp != 0x0) {
                                            dpArmorLeft =
                                                dpArmorLeft - (int32_t)((int32_t)((int32_t)((int32_t)((int32_t)((int32_t)(dpSingle * ptokE->dv.pctDp) / 0xa) *
                                                                                                      ptokE->dv.pctSh) /
                                                                                            0xa) *
                                                                                  (uint32_t)ptokE->csh) /
                                                                        0x1f4);
                                        }
                                        if (dpArmorLeft <= 0) {
                                            dpArmorLeft = 1;
                                        }
                                        if (part.hs.grhst == hstBeam || part.hs.grhst != hstTorp) {
                                            if (ptokE->pctBeamDef < 0x64) {
                                                lValue = (int32_t)((int32_t)(lValue * (uint32_t)ptokE->pctBeamDef) / 0x64);
                                            }
                                            if ((part.pbeam->grfAbilities & 0x1) == 0x0) {
                                                score = (int32_t)((int32_t)(lValue * 100) / (dpArmorLeft + dpShieldLeft + 0x1));
                                                if (score <= 0) {
                                                    score = 1;
                                                }
                                            } else if (dpShieldLeft <= 0) {
                                                score = 0;
                                            } else {
                                                score = (int32_t)((int32_t)((uint32_t)(lValue * 100) + dpShieldLeft - 0x1) / dpShieldLeft);
                                            }
                                        } else {
                                            pctHit = (int32_t)part.ptorp->dHitChance;
                                            t_scratch_m7a_3 = ptok->pctBC;
                                            if (t_scratch_m7a_3 < ptokE->pctJam) {
                                                t_scratch_m7a_5 = ptok->pctBC;
                                                pctHit = pctHit - (int32_t)((int32_t)(pctHit * (int32_t)(ptokE->pctJam - t_scratch_m7a_5)) / 0x64);
                                            } else {
                                                t_scratch_m7a_4 = ptokE->pctJam;
                                                pctHit = pctHit + (int32_t)((int32_t)((100 - pctHit) * (int32_t)(ptok->pctBC - t_scratch_m7a_4)) / 0x64);
                                            }
                                            if (pctHit <= 0) {
                                                score = 0;
                                            } else {
                                                if (part.hs.iItem < itorpJihadMissile || part.hs.iItem > itorpArmageddonMissile) {
                                                    t_merge_7293_0001 = 0;
                                                } else {
                                                    t_merge_7293_0001 = 1;
                                                }
                                                fCapMissile = t_merge_7293_0001;
                                                if (dpArmorLeft < 100000) {
                                                    nts = (int32_t)((int32_t)((uint32_t)(dpArmorLeft * 100) * 2) / pctHit);
                                                } else {
                                                    nts = (uint32_t)((int32_t)(dpArmorLeft / pctHit) * 200);
                                                }
                                                if (dpShieldLeft < 100000) {
                                                    nds = (int32_t)((int32_t)(dpShieldLeft * 100) / ((int32_t)(pctHit / 2) + (int32_t)((100 - pctHit) / 0x8)));
                                                } else {
                                                    nds = (uint32_t)((int32_t)(dpShieldLeft / ((int32_t)(pctHit / 2) + (int32_t)((100 - pctHit) / 0x8))) * 100);
                                                }
                                                ntk = (int32_t)((int32_t)((dpArmorLeft - (int32_t)((int32_t)(nds * pctHit) / 200)) * 0x64) /
                                                                (int32_t)(pctHit * (int32_t)(fCapMissile + 1)));
                                                t_merge_7484_0001 = nts < nds + ntk ? nts : nds + ntk;
                                                score = t_merge_7484_0001;
                                                if (score <= 0) {
                                                    score = 0;
                                                } else {
                                                    score = (int32_t)(lValue / score);
                                                    if (score <= 0) {
                                                        score = 1;
                                                    }
                                                }
                                            }
                                        }
                                        if (score > scoreBest) {
                                            scoreBest = score;
                                            ptokTarget = ptokE;
                                            itokTarget = itok;
                                        }
                                    }
                                }
                                ptokE = ptokE + 1;
                            }
                            if (ptokTarget != 0x0)
                                break;
                        }
                        if (ptokTarget == 0x0)
                            break;
                        dz = DzFromBrcBrc(ptokTarget->brc, ptok->brc);
                        if (part.hs.grhst == hstBeam) {
                            dp = dpMain;
                            if (ptok->pctCap != 0x0) {
                                dp = (int32_t)((int32_t)(dp * (int32_t)ptok->pctCap) / 0x64);
                            }
                            if (ptokTarget->pctBeamDef < 0x64) {
                                dp = (int32_t)((int32_t)(dp * (int32_t)ptokTarget->pctBeamDef) / 0x64);
                            }
                            if (dz > 0 && part.pbeam->dRangeMax > 0) {
                                dp = (int32_t)((int32_t)(dp * (100 - (int32_t)((int32_t)((int32_t)dz * 10) / (int32_t)part.pbeam->dRangeMax))) / 0x64);
                            }
                            if (part.pbeam->dp < 200) {
                                grfWeapon = 0x1;
                            } else {
                                grfWeapon = 0x2;
                            }
                            dpT = dp;
                            if (FDamageTok(ptokTarget, itokTarget, &dp, 0, grfWeapon, part.pbeam->grfAbilities & 0x1, 0x0) != 0) {
                                if (fSetItok == 0) {
                                    lpbtlrec->itokAttack = itokTarget;
                                    fSetItok = 1;
                                }
                                ctokDamaged = ctokDamaged + 1;
                            }
                            if (dp <= 0 || dpT <= 0) {
                                dpMain = 0;
                            } else {
                                if (dpMain < 65536 && dp < 65536) {
                                    lValue = (int32_t)((int32_t)(dpMain * dp) / dpT);
                                } else {
                                    lValue = (int32_t)((double)dpMain * (double)dp / (double)dpT);
                                }
                                t_merge_77fb_0001 = dpMain - 1 < lValue ? dpMain - 1 : lValue;
                                dpMain = t_merge_77fb_0001;
                            }
                        } else if (part.hs.grhst == hstTorp && cTorpsLeft > 0) {
                            grfWeapon = 0x4;
                            cTorpBase = cTorpsLeft;
                            cTorpHit = CTorpHit(cTorpBase, ptokTarget, part.ptorp->dHitChance, ptok->pctBC);
                            lpshdefE = LpshdefFromTok(ptokTarget);
                            dpSingle = (uint32_t)lpshdefE->hul.dp;
                            dpShieldLeft = (uint32_t)((uint32_t)ptokTarget->dpShield * (uint32_t)ptokTarget->csh);
                            dpArmorLeft = (uint32_t)(dpSingle * (uint32_t)ptokTarget->csh);
                            if (ptokTarget->dv.dp != 0x0) {
                                dpArmorLeft =
                                    dpArmorLeft - (int32_t)((int32_t)((int32_t)((int32_t)((int32_t)((int32_t)(dpSingle * ptokTarget->dv.pctDp) / 0xa) *
                                                                                          ptokTarget->dv.pctSh) /
                                                                                0xa) *
                                                                      (uint32_t)ptokTarget->csh) /
                                                            0x1f4);
                            }
                            dp = (int32_t)part.ptorp->dp;
                            if (part.hs.iItem >= itorpJihadMissile && part.hs.iItem <= itorpArmageddonMissile) {
                                if (dpShieldLeft <= 0) {
                                    dp = (int32_t)(dp * 2);
                                }
                                grfWeapon = grfWeapon | 0x8;
                            }
                            i = ptokTarget->csh;
                            if ((int32_t)i >= cTorpBase || (int32_t)(uint32_t)(cTorpHit * dp) <= dpArmorLeft) {
                                cTorpFire = cTorpHit;
                                cTorpMiss = cTorpBase - cTorpHit;
                            } else {
                                for (; (int32_t)i <= cTorpBase; i++) {
                                    cTorpFire = (int32_t)((int32_t)((uint32_t)((int32_t)i * cTorpHit) + cTorpBase - 0x1) / cTorpBase);
                                    cTorpMiss = (int32_t)i - cTorpFire;
                                    dpShieldCur = dpShieldLeft - (int32_t)((int32_t)(cTorpMiss * dp) / 8);
                                    if (dpShieldCur < 0) {
                                        dpShieldCur = 0;
                                    }
                                    dpShieldCur = dpShieldCur - (int32_t)((int32_t)(cTorpFire * dp) / 2);
                                    dpHitArmor = (int32_t)((int32_t)(cTorpFire * dp) / 2);
                                    if (dpShieldCur < 0) {
                                        dpHitArmor = dpHitArmor - dpShieldCur;
                                    }
                                    if (dpHitArmor >= dpArmorLeft)
                                        break;
                                }
                            }
                            dpCol = (int32_t)((int32_t)(cTorpMiss * dp) / 8);
                            if (dpCol > 0 && FDamageTok(ptokTarget, itokTarget, &dpCol, 0, grfWeapon | 0x80, 1, 0x0) != 0) {
                                ctokDamaged = ctokDamaged + 1;
                            }
                            dpT = (int32_t)((int32_t)(cTorpFire * dp) / 2);
                            cTorpBase = cTorpFire + cTorpMiss;
                            FDamageTok(ptokTarget, itokTarget, &dpT, dpT, grfWeapon, 0, &cTorpBase);
                            ctokDamaged = ctokDamaged + 1;
                            if (fSetItok == 0) {
                                fSetItok = 1;
                                lpbtlrec->itokAttack = itokTarget;
                            }
                            cTorpsLeft = cTorpsLeft - (cTorpFire + cTorpMiss);
                        }
                    } while (dpMain > 0 || cTorpsLeft > 0);
                } else {
                    dp = (uint32_t)((uint32_t)((int32_t)part.pbeam->dp * (int32_t)cItem) * (uint32_t)ptok->csh);
                    if (part.pbeam->dp < 200) {
                        grfWeapon = 0x1;
                    } else {
                        grfWeapon = 0x2;
                    }
                    if (ptok->pctCap != 0x0) {
                        dp = (int32_t)((int32_t)(dp * (int32_t)ptok->pctCap) / 0x64);
                    }
                    dpT = dp;
                    ptokE = vrgtok;
                    for (itok = 0; itok < vctok; itok++) {
                        if (ptokE->fActive != 0x0) {
                            t_scratch_m7a = ptokE->iplr;
                            if (t_scratch_m7a != ptok->iplr && (0x1 << ptokE->iplr & grfAttack) != 0x0 && DzFromBrcBrc(ptokE->brc, ptok->brc) <= dxRangeCur &&
                                (FIsTargetOfMdTarget(ptokE, ptok->mdTarget1) != 0 || FIsTargetOfMdTarget(ptokE, ptok->mdTarget2) != 0)) {
                                if (ptokE->pctBeamDef < 0x64) {
                                    dp = (int32_t)((int32_t)(dp * (int32_t)ptokE->pctBeamDef) / 0x64);
                                }
                                if (FDamageTok(ptokE, itok, &dp, 0, grfWeapon, part.pbeam->grfAbilities & 0x1, 0x0) != 0) {
                                    if (fSetItok == 0) {
                                        fSetItok = 1;
                                        lpbtlrec->itokAttack = itok;
                                    }
                                    ctokDamaged = ctokDamaged + 1;
                                }
                                dp = dpT;
                            }
                        }
                        ptokE = ptokE + 1;
                    }
                }
            }
        }
    }
    lpbtlrec->ctok = ctokDamaged;
    if (ctokDamaged == 0) {
        return 0;
    }
    return 1;
}

void KillShips(TOK *ptok, int16_t cshKill, int16_t ishdef, FLEET *lpfl, int16_t fFallout) {
    FLEET   flDead;
    int16_t i;
    FLEET   flSrc;
    int16_t csh;

    if (cshKill != 0) {
        if (fFallout != 0) {
            MarkTechsSeen(&LpshdefFromTok(ptok)->hul, ptok->iplr);
        }
        flSrc = *lpfl;
        memset(&flDead, 0, sizeof(FLEET));
        csh = lpfl->rgcsh[ishdef] - cshKill;
        flDead.rgcsh[ishdef] = cshKill;
        flSrc.rgcsh[ishdef] = csh;
        ptok->csh = csh;
        if (csh == 0) {
            ptok->fActive = 0x0;
            for (ishdef = 0; ishdef < 16 && flSrc.rgcsh[ishdef] == 0; ishdef++) {
            }
            if (ishdef == 16) {
                lpfl->fDead = 0x1;
                if (fFallout != 0) {
                    for (i = 0; i <= 2; i++) {
                        flDead.rgwtMin[i] = flSrc.rgwtMin[i];
                    }
                }
            }
        }
        if (lpfl->fDead == 0x0) {
            flDead.iPlayer = flSrc.iPlayer;
            flDead.fDead = 0x1;
            flDead.det = 0x7;
            FleetTransferCargoBalance(&flSrc, &flDead);
        }
        if (fFallout != 0) {
            flDead.iPlayer = flSrc.iPlayer;
            flDead.pt = flSrc.pt;
            flDead.idPlanet = flSrc.idPlanet;
            CreateSalvage(&flDead, &lpthBattle);
        }
        if (lpfl->fDead == 0x0) {
            *lpfl = flSrc;
        }
    }
    return;
}

void CreateSalvage(FLEET *pfl, THING **plpth) {
    int32_t wtTotal;
    SHDEF  *lpshdefT;
    PLANET *lppl;
    int16_t i;
    int32_t rgwtMinerals[3];
    int16_t j;
    int16_t fBleeding;
    SHDEF   shdefT;

    fBleeding = GetRaceGrbit(&rgplr[pfl->iPlayer], ibitRaceBleedingEdgeTech);
    gd.fDontCalcBleed = 0x1;
    idPlayer = pfl->iPlayer;
    if (pfl->idPlanet == -1) {
        lppl = 0x0;
    } else {
        lppl = LpplFromId(pfl->idPlanet);
    }
    for (i = 0; i <= 2; i++) {
        rgwtMinerals[i] = 0;
        for (j = 0; j < 16; j++) {
            if (pfl->rgcsh[j] > 0) {
                if (fBleeding == 0) {
                    lpshdefT = rglpshdef[pfl->iPlayer] + j;
                } else {
                    shdefT = rglpshdef[pfl->iPlayer][j];
                    UpdateShdefCost(&shdefT);
                    lpshdefT = &shdefT;
                }
                rgwtMinerals[i] = rgwtMinerals[i] + (int32_t)((int32_t)((int32_t)pfl->rgcsh[j] * (uint32_t)lpshdefT->hul.rgwtOreCost[i]) / 0x3);
            }
        }
        rgwtMinerals[i] = rgwtMinerals[i] + pfl->rgwtMin[i];
        if (lppl != 0x0) {
            lppl->rgwtMin[i] = lppl->rgwtMin[i] + (int32_t)((int32_t)(rgwtMinerals[i] * (uint32_t)(lppl->fStarbase == 0x0 ? 0x5 : 0x8)) / 0xa);
        }
    }
    if (lppl == 0x0) {
        wtTotal = 0;
        for (i = 0; i <= 2; i++) {
            rgwtMinerals[i] = rgwtMinerals[i] - (int32_t)(rgwtMinerals[i] >> 0x2);
            wtTotal = wtTotal + rgwtMinerals[i];
        }
        if (wtTotal != 0) {
            DropSalvage(plpth, rgwtMinerals, pfl->iPlayer, &pfl->pt);
        }
    }
    gd.fDontCalcBleed = 0x0;
    idPlayer = -1;
    return;
}

int16_t FDamageTok(TOK *ptok, int16_t itok, int32_t *pdpBeam, int32_t dpTorp, uint16_t grfWeapon, int16_t fShieldsOnly, int32_t *pcTorp) {
    int16_t   pctSh;
    DV        dv;
    uint16_t *pwLosses;
    int16_t   cshOrigDamaged;
    int32_t   dpShdef;
    int32_t   ddpOrig;
    int32_t   dpOrig;
    PLANET   *lppl;
    int16_t   i;
    int16_t   cshOrig;
    FLEET    *lpfl;
    int32_t   cKillMax;
    int16_t   csh;
    int32_t   dpT;
    int16_t   pctDp;
    int16_t   ishdef;
    int32_t   dp;
    uint16_t  pctDpNew;

    dp = *pdpBeam;
    fmemset(lpbBattleCur, 0, 0x8);
    *lpbBattleCur = LOBYTE(itok);
    lpbBattleCur[1] = LOBYTE(grfWeapon);
    if (ptok->dpShield == 0x0) {
        if (fShieldsOnly != 0) {
            return 0;
        }
    } else {
        dpOrig = (uint32_t)ptok->dpShield;
        dpT = (uint32_t)ptok->dpShield - dpOrig;
        dpOrig = (uint32_t)(dpOrig * (uint32_t)ptok->csh);
        if (dpOrig <= dp) {
            dp = dp - dpOrig;
            RawStore16((uint8_t *)lpbBattleCur + 0x4, WPackLong(dpOrig));
            ptok->dpShield = 0x0;
        } else {
            dpOrig = dpOrig - dp;
            RawStore16((uint8_t *)lpbBattleCur + 0x4, WPackLong(dp));
            ptok->dpShield = LOWORD((int32_t)(dpOrig / (int32_t)ptok->csh)) + LOWORD(dpT);
            dp = 0;
        }
    }
    if ((dp != 0 && fShieldsOnly == 0) || dpTorp != 0) {
        if (pcTorp == 0x0) {
            cKillMax = 2147483647;
        } else {
            cKillMax = *pcTorp;
        }
        dp = dp + dpTorp;
        ishdef = ptok->ishdef;
        dpShdef = (uint32_t)LpshdefFromTok(ptok)->hul.dp;
        dv.dp = ptok->dv.dp;
        if (ptok->grobj != grobjPlanet) {
            if (ptok->mdTactic == 0x1) {
                ptok->mdTactic = 0x0;
                ptok->dzDis = 0x7;
            }
            lpfl = LpflFromId(ptok->id);
            dpOrig = dp;
            csh = ptok->csh;
            cshOrig = csh;
            if (dv.pctDp == 0x0) {
                cshOrigDamaged = 0;
                ddpOrig = 0;
            } else {
                cshOrigDamaged = LOWORD((int32_t)((int32_t)((int32_t)csh * dv.pctSh) / 0x64));
                if (cshOrigDamaged == 0) {
                    cshOrigDamaged = 1;
                }
                ddpOrig = (int32_t)((int32_t)(dpShdef * dv.pctDp) / 0x1f4);
                if (ddpOrig == 0) {
                    ddpOrig = 1;
                }
            }
            pwLosses = vrgPlrLosses + ((ptok->iplr << 0x4) + ishdef);
            *pwLosses = *pwLosses | 0x8000;
            if (cshOrigDamaged != 0) {
                csh = cshOrigDamaged;
                dpShdef = dpShdef - ddpOrig;
                while (dp >= dpShdef && csh != 0 && cKillMax != 0) {
                    dp = dp - dpShdef;
                    csh = csh - 1;
                    cKillMax = cKillMax - 1;
                    if ((*pwLosses & 0x1fff) < 0x1fff) {
                        *pwLosses = *pwLosses + 0x1;
                    }
                }
                dpShdef = dpShdef + ddpOrig;
                i = cshOrigDamaged;
                cshOrigDamaged = csh;
                csh = csh + (cshOrig - i);
            }
            while (dp >= dpShdef && csh != 0 && cKillMax != 0) {
                dp = dp - dpShdef;
                csh = csh - 1;
                cKillMax = cKillMax - 1;
                if ((*pwLosses & 0x1fff) < 0x1fff) {
                    *pwLosses = *pwLosses + 0x1;
                }
            }
            if (cKillMax <= 0) {
                dp = 0;
            }
            if (dp == 0 || csh == 0) {
                if (cshOrigDamaged == 0) {
                    pctSh = 0;
                    pctDp = 0;
                } else {
                    pctSh = LOWORD((int32_t)((int32_t)((uint32_t)((int32_t)cshOrigDamaged * 100) + (int32_t)csh - 0x1) / (int32_t)csh));
                    pctDp = ptok->dv.pctDp;
                }
            } else {
                if (cshOrigDamaged != 0) {
                    dp = dp + ((uint32_t)(ddpOrig * (int32_t)cshOrigDamaged) + (int32_t)csh - 1);
                }
                dp = (int32_t)(dp / (int32_t)csh);
                if (dp == 0) {
                    dp = 1;
                }
                pctDp = LOWORD((int32_t)((int32_t)((uint32_t)(dp * 500) + dpShdef - 0x1) / dpShdef));
                if (pctDp == 0) {
                    pctDp = 1;
                }
                pctSh = 100;
            }
            RawStore16((uint8_t *)lpbBattleCur + 0x2, ptok->csh - csh);
            if (csh != ptok->csh) {
                KillShips(ptok, RawLoad16((uint8_t *)lpbBattleCur + 0x2), ishdef, lpfl, 1);
            }
            if (csh != 0) {
                if (pctDp > 499) {
                    pctDp = 499;
                }
                dv.pctDp = pctDp;
                dv.pctSh = pctSh;
                ptok->dv.dp = dv.dp;
                lpfl->rgdv[ishdef].dp = dv.dp;
                dp = 0;
            }
            if (dp <= dpTorp) {
                *pdpBeam = 0;
            } else {
                *pdpBeam = dp - dpTorp;
            }
            dpOrig = dpOrig - *pdpBeam;
            RawStore16((uint8_t *)lpbBattleCur + 0x6, ptok->dv.dp);
            lpbBattleCur = lpbBattleCur + 8;
            if (pcTorp != 0x0) {
                *pcTorp = cKillMax;
            }
            return 1;
        }
        lppl = LpplFromId(ptok->id);
        cKillMax = cKillMax - 1;
        if (dv.pctDp != 0x0) {
            dp = dp + (int32_t)((int32_t)(dpShdef * dv.pctDp) / 0x1f4);
        }
        if (dp < dpShdef) {
            pctDpNew = LOWORD((int32_t)((int32_t)(dp * 500) / dpShdef));
            if (lppl->pctDp == pctDpNew) {
                lppl->pctDp = lppl->pctDp + 0x1;
            } else {
                lppl->pctDp = LOWORD((int32_t)((int32_t)(dp * 500) / dpShdef));
            }
            RawStore16((uint8_t *)lpbBattleCur + 0x6, (RawLoad16((uint8_t *)lpbBattleCur + 0x6) & 0x7f) | (lppl->pctDp & 0x1ff) << 0x7);
            fStarbaseDamaged = 1;
        } else {
            RawStore16((uint8_t *)lpbBattleCur + 0x6, (RawLoad16((uint8_t *)lpbBattleCur + 0x6) & 0x7f) | 0xfa00);
            RawStore16((uint8_t *)lpbBattleCur + 0x2, 0x1);
            ptok->fActive = 0x0;
            ptok->csh = 0x0;
            fStarbaseDied = 1;
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->fStarbase = 0x0;
                KillQueuedShips(lppl);
                KillQueuedMassPackets(lppl);
            }
        }
        if ((RawLoad16((uint8_t *)lpbBattleCur + 0x6) >> 0x7 & 0x1ff) != 0x0) {
            RawStore16((uint8_t *)lpbBattleCur + 0x6, (RawLoad16((uint8_t *)lpbBattleCur + 0x6) & 0xff80) | 0x64);
            ptok->dv.dp = RawLoad16((uint8_t *)lpbBattleCur + 0x6);
        }
        *pdpBeam = 0;
        lpbBattleCur = lpbBattleCur + 8;
        return 1;
    }
    RawStore16((uint8_t *)lpbBattleCur + 0x6, ptok->dv.dp);
    *pdpBeam = dp;
    if ((lpbBattleCur[1] & 0x4) != 0x0) {
        lpbBattleCur[1] = lpbBattleCur[1] | 0xc0;
    }
    lpbBattleCur = lpbBattleCur + 8;
    return 1;
}

int16_t DxyFromSpdRound(uint16_t spd, int16_t iRound) {
    int16_t dxy;

    dxy = (uint32_t)(spd + 0x2) / 4;
    switch (spd & 0x3) {
    case 0x0:
        dxy = dxy + ((iRound & 0x1) == 0x0 ? 1 : 0);
        break;
    case 0x1:
        dxy = dxy + ((iRound & 0x3) == 0x2 ? 0 : 1);
        break;
    case 0x3:
        dxy = dxy + ((iRound & 0x3) == 0x0 ? 1 : 0);
    default:
    }
    return dxy;
}

int16_t FDoCoolBattle(FLEET *lpfl, int16_t cplr, uint16_t *rggrfAttack, uint16_t grfPlayer, uint16_t grfSpectator) {
    int16_t  cShipsInvolved;
    uint8_t *lpbMax;
    TOK     *ptok;
    uint16_t wt;
    int16_t  cShdefsInvolved;
    uint8_t *lpbSav;
    int16_t  initMac;
    int16_t  init;
    uint16_t wtT;
    uint16_t grplrLeft;
    int16_t  i;
    int16_t  j;
    int16_t  initMin;
    BTLREC  *lpbtlrec;
    int16_t  iRound;
    FLEET   *lpflT;
    uint16_t brcOrig;
    BTLDATA *lpbtldata;
    uint8_t  rgfInit[64];
    uint16_t rgPlrLosses[256];
    uint16_t wtNext;
    int16_t  itok;
    jmp_buf  env;
    jmp_buf *penvMemSav;
    PLANET  *lppl;
    int32_t  lwt;
    int16_t  t_8f4b;
    uint16_t t_scratch_m278_2;
    int16_t  t_scratch_m278_3;

    if (lpbBattleLog == 0x0) {
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) != 0) {
            penvMem = penvMemSav;
            return -1;
        }
        lpbBattleLog = LpAlloc(0xffc8, htBattle);
        lpbBattleCur = lpbBattleLog;
    }
    if (lpbBattleT == 0x0) {
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) != 0) {
            penvMem = penvMemSav;
            return -1;
        }
        lpbBattleT = LpAlloc(0xffc8, htBattle);
    }
    lpbSav = lpbBattleCur;
    lpbBattleCur = lpbBattleT;
    lpbMax = lpbBattleT - 72;
    memset(rgPlrLosses, 0, 0x200);
    vrgPlrLosses = rgPlrLosses;
    memset(rgfInit, 0, 0x40);
    fmemset(vrgtok, 0, 256 * sizeof(TOK));
    vctok = 0;
    lpbtldata = (BTLDATA *)lpbBattleCur;
    lpbBattleCur = lpbBattleCur + 14;
    memset(rgTechBattle, 0, 0x6);
    memset(rgTechTrader, 0, 0xd);
    lpthBattle = 0x0;
    cShdefsInvolved = 0;
    cShipsInvolved = 0;
    fStarbaseDied = 0;
    fStarbaseDamaged = 0;
    lpflT = lpfl;
    do {
        if (lpflT->fDead == 0x0) {
            for (i = 0; i < 16; i++) {
                if (lpflT->rgcsh[i] > 0) {
                    cShipsInvolved = cShipsInvolved + lpflT->rgcsh[i];
                    rgPlrLosses[lpflT->iPlayer * 16 + i] = 0x8000;
                }
            }
        }
        lpflT = lpflT->lpflNext;
    } while (lpflT != lpfl && lpflT != 0x0);
    for (i = 0; i < 256; i++) {
        if (rgPlrLosses[i] != 0x0) {
            rgPlrLosses[i] = 0x0;
            cShdefsInvolved = cShdefsInvolved + 1;
        }
    }
    if (lpfl->idPlanet != -1) {
        lppl = LpplFromId(lpfl->idPlanet);
        if (lppl->fStarbase != 0x0 && (0x1 << lppl->iPlayer & grfPlayer) != 0x0) {
            cShdefsInvolved = cShdefsInvolved + 1;
            cShipsInvolved = cShipsInvolved + 1;
            lppl->fNoHeal = 0x1;
        }
    }
    InitializeBoard(lpfl, (int32_t)((cplr - 1) * cplr) / 2, grfPlayer, rgfInit, &initMin, &initMac);
    lpbtldata->cplr = LOBYTE(cplr);
    lpbtldata->ctok = LOBYTE(vctok);
    lpbtldata->idPlanet = lpfl->idPlanet;
    lpbtldata->pt = lpfl->pt;
    t_8f4b = idBattle;
    idBattle = idBattle + 1;
    lpbtldata->id = t_8f4b;
    for (iRound = 0; iRound < 16; iRound++) {
        grplrLeft = 0x0;
        for (itok = 0; itok < vctok; itok++) {
            if (vrgtok[itok].fActive != 0x0) {
                grplrLeft = grplrLeft | 0x1 << vrgtok[itok].iplr;
                vrgtok[itok].cTarget = 0x0;
                if (iRound > 0 && vrgtok[itok].dpShield > 0x0 && vrgtok[itok].fActive != 0x0 &&
                    GetRaceGrbit(&rgplr[vrgtok[itok].iplr], ibitRaceRegeneratingShields) != 0) {
                    RegenShield(vrgtok + itok);
                }
            }
        }
        if ((grplrLeft - 0x1 & grplrLeft) == 0x0)
            break;
        ptok = vrgtok;
        for (itok = 0; itok < vctok; itok++) {
            if (ptok->fActive != 0x0) {
                if (ptok->grobj != grobjPlanet) {
                    t_scratch_m278_2 = DxyFromSpdRound(ptok->spd, iRound);
                    ptok->dMovesLeft = t_scratch_m278_2;
                } else {
                    ptok->dMovesLeft = 0x0;
                }
            }
            ptok = ptok + 1;
        }
        for (j = 3; j > 0; j--) {
            wtNext = 0x0;
            wt = 0x7530;
            i = vctok;
            do {
                wtNext = 0x0;
                ptok = vrgtok;
                for (itok = 0; itok < vctok; itok++) {
                    if (ptok->fActive != 0x0 && ptok->wt != 0xffff) {
                        lwt = ptok->dwt;
                        lwt = lwt - 7;
                        lwt = (int32_t)(lwt * 2);
                        lwt = (uint32_t)ptok->wt + (int32_t)((int32_t)((uint32_t)ptok->wt * lwt) / 0x64);
                        wtT = LOWORD(lwt);
                        if (wtT > wtNext && wtT < wt && DxyFromSpdRound(ptok->spd, iRound) != 0) {
                            wtNext = wtT;
                        }
                        if (wtT == wt) {
                            i = i - 1;
                            if (ptok->dMovesLeft >= j) {
                                lpbtlrec = (BTLREC *)lpbBattleCur;
                                lpbBattleCur = lpbBattleCur + 6;
                                lpbtlrec->itok = LOBYTE(itok);
                                lpbtlrec->ctok = 0;
                                lpbtlrec->itokAttack = itok;
                                lpbtlrec->iRound = iRound;
                                lpbtlrec->dzDis = ptok->dzDis;
                                brcOrig = vrgtok[itok].brc;
                                if (ptok->mdTactic == 0x0) {
                                    brcOrig = 0xff;
                                    if (ptok->dzDis == 0x0) {
                                        lpbtlrec->brcDest = 0xff;
                                        ptok->fActive = 0x0;
                                        goto L_91a1;
                                    }
                                    ptok->dzDis = ptok->dzDis + 0x7ff;
                                }
                                DxyMoveTokTo(ptok, j, rggrfAttack[ptok->iplr]);
                                ptok->dMovesLeft = ptok->dMovesLeft + 0x3;
                                if (ptok->grobj == grobjPlanet || (brcOrig == ptok->brc && ptok->initMin != 0xff)) {
                                    lpbBattleCur = lpbBattleCur - 6;
                                } else {
                                    lpbtlrec->brcDest = ptok->brc;
                                }
                            }
                        }
                    } else {
                        i = i - 1;
                    }
                L_91a1:
                    ptok = ptok + 1;
                }
                wt = wtNext;
            } while (wtNext != 0x0);
        }
        grplrLeft = 0x0;
        for (i = 0; i < vctok; i++) {
            if (vrgtok[i].fActive != 0x0) {
                t_scratch_m278_3 = Random(15);
                vrgtok[i].wFlags = (vrgtok[i].wFlags & 0xc3ff) | (t_scratch_m278_3 & 0xf) * 0x400;
                grplrLeft = grplrLeft | 0x1 << vrgtok[i].iplr;
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            if ((0x1 << i & grplrLeft) != 0x0 && (grplrLeft & rggrfAttack[i]) == 0x0) {
                grplrLeft = grplrLeft & ~(0x1 << i);
            }
        }
        if ((grplrLeft - 0x1 & grplrLeft) == 0x0)
            break;
        for (init = initMac; init >= initMin; init--) {
            if (rgfInit[init] != 0x0) {
                for (itok = vctok - 1; itok >= 0; itok--) {
                    if (init >= vrgtok[itok].initMin && init <= vrgtok[itok].initMac) {
                        grplrLeft = 0x0;
                        for (i = 0; i < vctok; i++) {
                            if (vrgtok[i].fActive != 0x0) {
                                grplrLeft = grplrLeft | 0x1 << vrgtok[i].iplr;
                            }
                        }
                        if ((grplrLeft - 0x1 & grplrLeft) == 0x0)
                            break;
                        ptok = vrgtok + itok;
                        if (ptok->fActive != 0x0) {
                            lpbtlrec = (BTLREC *)lpbBattleCur;
                            lpbBattleCur = lpbBattleCur + 6;
                            lpbtlrec->itok = LOBYTE(itok);
                            lpbtlrec->ctok = 0;
                            lpbtlrec->iRound = iRound;
                            lpbtlrec->brcDest = ptok->brc;
                            lpbtlrec->itokAttack = itok;
                            lpbtlrec->dzDis = ptok->dzDis;
                            if (FAttack(itok, init, lpbtlrec, rggrfAttack[ptok->iplr]) != 0) {
                                ptok->fMoved = 0x0;
                            } else {
                                lpbBattleCur = lpbBattleCur - 6;
                            }
                        }
                    }
                }
            }
        }
        if ((grplrLeft - 0x1 & grplrLeft) == 0x0)
            break;
    }
    lpbtldata->cbData = lpbBattleCur - (uint8_t *)lpbtldata;
    SendBattleMessages(lpfl, cplr, lpbtldata->id, rgPlrLosses, grfPlayer, cShipsInvolved, cShdefsInvolved, grfSpectator);
    lpbtldata->grfPlr = grfPlayer;
    if (0xffc8 - (uint32_t)(LOWORD(lpbSav) & 0xffff) < (uint32_t)lpbtldata->cbData) {
        RawStore16(lpbSav, 0xffff);
        lpbBattleT = 0x0;
    } else {
        fmemmove(lpbSav, lpbtldata, lpbtldata->cbData);
        lpbBattleCur = lpbSav + lpbtldata->cbData;
    }
    return 1;
}

int16_t ITechLearnATech(int16_t iplr, int16_t x, int16_t y, MessageId idm, uint16_t *piGoto) {
    uint16_t iGoto;
    int16_t  fBattle;
    int16_t  i;
    int16_t  iTech;
    int32_t  l;
    int16_t  t_scratch_m10_2;

    fBattle = idm == 0xffff ? 0 : 1;
    if (rgplr[iplr].fLearned == 0x0 && Random(100) >= 50) {
        for (i = 0; i < 13; i++) {
            iTech = Random(13);
            if (rgTechTrader[iTech] != 0x0 && (0x1 << iTech & rgplr[iplr].grbitTrader) == 0x0) {
                t_scratch_m10_2 = Random(100);
                if (t_scratch_m10_2 < rgTechTrader[iTech])
                    goto L_99e4;
            }
        }
        for (i = 0; i < 6; i++) {
            iTech = Random(6);
            if ((int16_t)rgplr[iplr].rgTech[iTech] < rgTechBattle[iTech])
                goto L_9ac0;
        }
        return 0;
    L_9ac0:
        l = GetTechLevelCost(iTech, (int16_t)rgplr[iplr].rgTech[iTech] + 1, iplr);
        if (game.fSlowTech != 0x0) {
            l = (int32_t)(l >> 0x1);
        }
        rgplr[iplr].rgResSpent[iTech] = rgplr[iplr].rgResSpent[iTech] + l;
        if (fBattle == 0) {
            if (piGoto != 0x0) {
                *piGoto = 0xfffe;
            }
        } else {
            if (game.fSlowTech != 0x0) {
                l = (int32_t)(l * 2);
            }
            FSendPlrMsg(iplr, idm, -2, x, y, iTech, LOWORD(l), HIWORD(l), 0, 0);
        }
        rgplr[iplr].wFlags = (rgplr[iplr].wFlags & 0xfff7) | 0x8;
        return iTech + 1;
    L_99e4:
        idm = IdmGiveTraderPart(0x1 << iTech, iplr, &iGoto);
        if (fBattle == 0) {
            if (piGoto != 0x0) {
                *piGoto = iGoto;
            }
        } else {
            idm = idm + 47;
            FSendPlrMsg2(iplr, idm, iGoto, x, y);
        }
        rgplr[iplr].wFlags = (rgplr[iplr].wFlags & 0xfff7) | 0x8;
        return -(iTech + 1);
    }
    return 0;
}

void SendBattleMessages(FLEET *lpflBtl, int16_t cplr, int16_t idBtl, uint16_t *rgPlrLosses, int16_t grfPlayer, int16_t cShipsInvolved, int16_t cShdefsInvolved,
                        uint16_t grfSpectator) {
    int16_t   iplrStarbase;
    int16_t   iplr;
    uint8_t   rgcfl[16];
    int32_t   lpopStarbase;
    uint16_t *pw;
    int16_t   isb;
    PLANET   *lppl;
    uint16_t *pwThem;
    int16_t   fAlive;
    int16_t   cUs;
    int16_t   y;
    FLEET    *lpfl;
    int16_t   cThemDead;
    int16_t   i;
    MessageId idm;
    int16_t   j;
    FLEET    *lpflT;
    int16_t   cUsDead;
    int16_t   iThem;
    uint16_t *pwUs;
    int16_t   cThem;
    int16_t   x;

    iplrStarbase = -1;
    lppl = 0x0;
    memset(rgcfl, 0, 0x10);
    if (lpflBtl->idPlanet == -1) {
        x = lpflBtl->pt.x;
        y = lpflBtl->pt.y;
    } else {
        x = -1;
        y = lpflBtl->idPlanet;
        lppl = LpplFromId(y);
        iThem = lppl->iPlayer;
        iplr = lppl->iPlayer;
        if (iplr != -1) {
            if (lppl->fStarbase != 0x0 || fStarbaseDied != 0) {
                iplrStarbase = iplr;
                isb = lppl->isb;
            }
            if (fStarbaseDied != 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh) {
                lpopStarbase = lppl->rgwtMin[3];
                UninhabitPlanet(lppl);
            }
        }
    }
    lpfl = lpflBtl;
    lpflT = 0x0;
    do {
        if (lpfl->fDead == 0x0) {
            for (i = 0; i < 16; i++) {
                if (lpfl->rgcsh[i] > 0) {
                    rgPlrLosses[(lpfl->iPlayer << 0x4) + i] = rgPlrLosses[(lpfl->iPlayer << 0x4) + i] | 0x4000;
                }
            }
        }
        lpfl = lpfl->lpflNext;
    } while (lpfl != lpflBtl && lpfl != 0x0);
    for (iplr = 0; iplr < game.cPlayer; iplr++) {
        if ((0x1 << iplr & grfPlayer) == 0x0) {
            if (lppl == 0x0 || lppl->iPlayer != iplr) {
                if ((iplr & grfSpectator) != 0x0) {
                    lpfl = lpflBtl;
                    while (lpfl->iPlayer != iplr) {
                        lpfl = lpfl->lpflNext;
                        if (lpfl == lpflBtl || lpfl == 0x0)
                            break;
                    }
                    if (lpfl != 0x0 && lpfl->iPlayer == iplr) {
                        FSendPlrMsg(iplr, 250, lpfl->id | 0x8000, lpfl->id, lpfl->pt.x, lpfl->pt.y, 0, 0, 0, 0);
                        ITechLearnATech(iplr, x, y, idmWreckageBattleOccurredOrbitHasBoostedResearch, 0x0);
                    }
                }
            } else {
                FSendPlrMsg2(iplr, 249, lppl->id, lppl->id, 0);
                ITechLearnATech(iplr, x, y, idmFleetFoundWreckageBattleWhichHasBoosted, 0x0);
            }
        } else {
            fAlive = 1;
            if (fStarbaseDied != 0 && GetRaceStat(&rgplr[iplrStarbase], rsMajorAdv) == raMacintosh) {
                if (iplr != iplrStarbase) {
                    idm = idmBattleTookPlaceDestroyedKillingColonistsBargain;
                } else if (lpopStarbase <= 1000) {
                    idm = idmBattleTookPlaceDestroyedColonistsHaveJoined;
                } else {
                    idm = idmBattleTookPlaceDestroyedScreamsColonistsEcho;
                }
                j = iplrStarbase << 0x5 | isb + 0x10;
                FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, j, LOWORD(lpopStarbase), HIWORD(lpopStarbase), 0, 0);
                continue;
            }
            if (cplr == 2) {
                if (cShipsInvolved == 2) {
                    pwThem = 0x0;
                    pwUs = 0x0;
                    pw = rgPlrLosses;
                    for (i = 0; i < 16; i++) {
                        j = 0;
                        while (j < 16) {
                            if (*pw != 0x0) {
                                if (i != iplr) {
                                    pwThem = pw;
                                } else {
                                    pwUs = pw;
                                }
                            }
                            j = j + 1;
                            pw = pw + 1;
                        }
                    }
                    if ((pwUs != 0x0 || fStarbaseDied == 0) && (pwUs == 0x0 || (*pwUs & 0x3fff) == 0x0)) {
                        if ((pwThem != 0x0 || fStarbaseDied == 0) && (pwThem == 0x0 || (*pwThem & 0x3fff) == 0x0)) {
                            idm = idmBattleTookPlaceNeitherNorDestroyedIncident;
                        } else if ((pwUs != 0x0 || fStarbaseDamaged == 0) && (pwUs == 0x0 || (*pwUs & 0x8000) == 0x0)) {
                            idm = idmBattleTookPlaceDestroyedTakingDamage;
                        } else {
                            idm = idmBattleTookPlaceDestroyedHoweverTookDamage;
                        }
                    } else {
                        if ((pwThem != 0x0 || fStarbaseDamaged == 0) && (pwThem == 0x0 || (*pwThem & 0x8000) == 0x0)) {
                            idm = idmBattleTookPlaceDestroyedWhichTookDamage;
                        } else {
                            idm = idmBattleTookPlaceDestroyedWhichDamagedFray;
                        }
                        fAlive = 0;
                    }
                    if (pwUs == 0x0) {
                        i = iplrStarbase << 0x5 | isb + 0x10;
                    } else {
                        i = ((uint8_t *)pwUs - (uint8_t *)rgPlrLosses) >> 0x1;
                        i = (i & 0xf0) << 0x1 | (i & 0xf);
                    }
                    if (pwThem == 0x0) {
                        j = iplrStarbase << 0x5 | isb + 0x10;
                    } else {
                        j = ((uint8_t *)pwThem - (uint8_t *)rgPlrLosses) >> 0x1;
                        j = (j & 0xf0) << 0x1 | (j & 0xf);
                    }
                    FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, i, j, 0, 0, 0);
                    if (fAlive == 0 || (lppl != 0x0 && lppl->iPlayer != -1 && iplr != lppl->iPlayer))
                        continue;
                    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
                    continue;
                }
                if (cShdefsInvolved == 2) {
                    pwThem = 0x0;
                    pwUs = 0x0;
                    pw = rgPlrLosses;
                    for (i = 0; i < 16; i++) {
                        j = 0;
                        while (j < 16) {
                            if (*pw != 0x0) {
                                if (i != iplr) {
                                    pwThem = pw;
                                } else {
                                    pwUs = pw;
                                }
                            }
                            j = j + 1;
                            pw = pw + 1;
                        }
                    }
                    if ((pwUs != 0x0 || fStarbaseDied == 0) && (pwUs == 0x0 || (*pwUs & 0x4000) != 0x0)) {
                        if ((pwThem != 0x0 || fStarbaseDied == 0) && (pwThem == 0x0 || (*pwThem & 0x4000) != 0x0)) {
                            idm = idmBattleTookPlaceNeitherNorCompletelyDestroyed;
                        } else if ((pwUs != 0x0 || fStarbaseDamaged == 0) && (pwUs == 0x0 || (*pwUs & 0x8000) == 0x0)) {
                            idm = idmBattleTookPlaceDestroyedTakingDamage2;
                        } else {
                            idm = idmBattleTookPlaceDestroyedHoweverTookDamage2;
                        }
                    } else {
                        if ((pwThem != 0x0 || fStarbaseDamaged == 0) && (pwThem == 0x0 || (*pwThem & 0x8000) == 0x0)) {
                            idm = idmBattleTookPlaceDestroyedWhichTookDamage2;
                        } else {
                            idm = idmBattleTookPlaceDestroyedWhichDamagedFray2;
                        }
                        fAlive = 0;
                    }
                    if (pwUs != 0x0) {
                        cUs = *pwUs & 0x1fff;
                    } else {
                        cUs = 1;
                    }
                    if (pwThem != 0x0) {
                        cThem = *pwThem & 0x1fff;
                    } else {
                        cThem = 1;
                    }
                    lpfl = lpflBtl;
                    do {
                        if (lpfl->fDead == 0x0) {
                            if (lpfl->iPlayer != iplr || pwUs == 0x0) {
                                if (pwThem != 0x0) {
                                    cThem = cThem + lpfl->rgcsh[(((uint8_t *)pwThem - (uint8_t *)rgPlrLosses) >> 0x1) - (lpfl->iPlayer << 0x4)];
                                }
                            } else {
                                cUs = cUs + lpfl->rgcsh[(((uint8_t *)pwUs - (uint8_t *)rgPlrLosses) >> 0x1) - (lpfl->iPlayer << 0x4)];
                            }
                        }
                        lpfl = lpfl->lpflNext;
                    } while (lpfl != lpflBtl && lpfl != 0x0);
                    if (pwUs == 0x0) {
                        i = iplrStarbase << 0x5 | isb + 0x10;
                    } else {
                        i = ((uint8_t *)pwUs - (uint8_t *)rgPlrLosses) >> 0x1;
                        i = (i & 0xf0) << 0x1 | (i & 0xf);
                    }
                    if (pwThem == 0x0) {
                        j = iplrStarbase << 0x5 | isb + 0x10;
                    } else {
                        j = ((uint8_t *)pwThem - (uint8_t *)rgPlrLosses) >> 0x1;
                        j = (j & 0xf0) << 0x1 | (j & 0xf);
                    }
                    FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, i, cUs, j, cThem, 0);
                    if (fAlive == 0 || (lppl != 0x0 && lppl->iPlayer != -1 && iplr != lppl->iPlayer))
                        continue;
                    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
                    continue;
                }
            }
            cThem = 0;
            cUs = 0;
            pw = rgPlrLosses;
            for (i = 0; i < 16; i++) {
                j = 0;
                while (j < 16) {
                    if (*pw != 0x0) {
                        if (i != iplr) {
                            pwThem = pw;
                            cThem = cThem + (*pw & 0x1fff);
                            iThem = i;
                        } else {
                            pwUs = pw;
                            cUs = cUs + (*pw & 0x1fff);
                        }
                    }
                    j = j + 1;
                    pw = pw + 1;
                }
            }
            iThem = iThem | 0x30;
            cUsDead = cUs;
            cThemDead = cThem;
            if (fStarbaseDied != 0) {
                if (iplrStarbase != iplr) {
                    if (iplrStarbase != -1) {
                        cThemDead = cThemDead + 1;
                    }
                } else {
                    cUsDead = cUsDead + 1;
                }
            }
            lpfl = lpflBtl;
            do {
                if (lpfl->fDead == 0x0) {
                    if (lpfl->iPlayer != iplr) {
                        for (i = 0; i < 16; i++) {
                            cThem = cThem + lpfl->rgcsh[i];
                        }
                    } else {
                        for (i = 0; i < 16; i++) {
                            cUs = cUs + lpfl->rgcsh[i];
                        }
                    }
                }
                lpfl = lpfl->lpflNext;
            } while (lpfl != lpflBtl && lpfl != 0x0);
            if (iplrStarbase != iplr) {
                if (iplrStarbase != -1) {
                    cThem = cThem + 1;
                    iThem = iplrStarbase | 0x10 | 0x20;
                }
            } else {
                cUs = cUs + 1;
            }
            if (cplr == 2) {
                if (cThem == 1) {
                    if ((iThem & 0xf) != iplrStarbase) {
                        j = ((uint8_t *)pwThem - (uint8_t *)rgPlrLosses) >> 0x1;
                        j = (j & 0xf0) << 0x1 | (j & 0xf);
                    } else {
                        j = iplrStarbase << 0x5 | isb + 0x10;
                    }
                }
                if (cUs == 1) {
                    if (iplr != iplrStarbase) {
                        i = ((uint8_t *)pwUs - (uint8_t *)rgPlrLosses) >> 0x1;
                        i = (i & 0xf0) << 0x1 | (i & 0xf);
                    } else {
                        i = iplrStarbase << 0x5 | isb + 0x10;
                    }
                }
                if (cThemDead != cThem) {
                    if (cUsDead != cUs) {
                        if (cUs != 1) {
                            if (cThem != 1) {
                                FSendPlrMsg(iplr, 159, idBtl | 0x4000, x, y, iThem, cUs, cThem, cUsDead, cThemDead);
                            } else {
                                FSendPlrMsg(iplr, 278, idBtl | 0x4000, x, y, iThem, cUs, j, cUsDead, 0);
                            }
                        } else {
                            FSendPlrMsg(iplr, 277, idBtl | 0x4000, x, y, iThem, i, cThem, cThemDead, 0);
                        }
                    } else {
                        idm = idmBattleTookPlaceAgainstForcesDestroyedEnemys;
                        if (cUsDead != 1) {
                            if (cThemDead != 0) {
                                FSendPlrMsg(iplr, idm + 2, idBtl | 0x4000, x, y, iThem, cThem, cThemDead, 0, 0);
                            } else if (cThem != 1) {
                                FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, iThem, cThem, 0, 0, 0);
                            } else {
                                FSendPlrMsg(iplr, 276, idBtl | 0x4000, x, y, iThem, cUsDead, j, 0, 0);
                            }
                        } else {
                            idm = idm + 5;
                            if (cThemDead != 0) {
                                FSendPlrMsg(iplr, idm + 2, idBtl | 0x4000, x, y, iThem, i, cThem, cThemDead, 0);
                            } else {
                                FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, iThem, i, cThem, 0, 0);
                            }
                        }
                    }
                } else {
                    idm = idmBattleTookPlaceAgainstForcesDestroyedEnemy;
                    if (cThemDead != 1) {
                        if (cUsDead != 0) {
                            FSendPlrMsg(iplr, idm + 2, idBtl | 0x4000, x, y, iThem, cUs, cUsDead, 0, 0);
                        } else if (cUs != 1) {
                            FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, iThem, cUs, 0, 0, 0);
                        } else {
                            FSendPlrMsg(iplr, 275, idBtl | 0x4000, x, y, iThem, i, cThemDead, 0, 0);
                        }
                    } else {
                        idm = idm + 5;
                        if (cUsDead != 0) {
                            FSendPlrMsg(iplr, idm + 2, idBtl | 0x4000, x, y, iThem, cUs, j, cUsDead, 0);
                        } else {
                            FSendPlrMsg(iplr, idm, idBtl | 0x4000, x, y, iThem, cUs, j, 0, 0);
                        }
                    }
                }
                if (cUsDead == cUs || (lppl != 0x0 && lppl->iPlayer != -1 && iplr != lppl->iPlayer))
                    continue;
                ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
                continue;
            }
            if (cUsDead != 0) {
                if (cThemDead != 0) {
                    if (cThemDead != cThem) {
                        if (cUsDead != cUs) {
                            FSendPlrMsg2(iplr, 126, idBtl | 0x4000, x, y);
                            goto L_ab98;
                        }
                        FSendPlrMsg(iplr, 167, idBtl | 0x4000, x, y, cplr, cUs, cThem, cThemDead, 0);
                        goto L_ab98;
                    }
                    FSendPlrMsg(iplr, 165, idBtl | 0x4000, x, y, cplr, cUsDead, cUs, 0, 0);
                    goto L_ab98;
                }
                if (cUs == cUsDead) {
                    FSendPlrMsg(iplr, 166, idBtl | 0x4000, x, y, cplr, cUs, cThem, 0, 0);
                    goto L_ab98;
                }
            } else if (cThem == cThemDead) {
                FSendPlrMsg(iplr, 164, idBtl | 0x4000, x, y, cplr, cUs, 0, 0, 0);
                goto L_ab98;
            }
            FSendPlrMsg(iplr, 168, idBtl | 0x4000, x, y, cplr, cUsDead, cUs, cThemDead, cThem);
        L_ab98:
            if (fAlive != 0 && (lppl == 0x0 || lppl->iPlayer == -1 || iplr == lppl->iPlayer)) {
                ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
            }
        }
        if ((0x1 << iplr & grfMissed) != 0x0) {
            lpfl = lpflBtl;
            while (lpfl->iPlayer != iplr || lpfl->fSkipped == 0x0) {
                lpfl = lpfl->lpflNext;
                if (lpfl == lpflBtl || lpfl == 0x0)
                    break;
            }
            if (lpfl != 0x0 && lpfl->iPlayer == iplr && lpfl->fSkipped != 0x0) {
                FSendPlrMsg2(iplr, 384, lpfl->id | 0x8000, x, y);
            }
        }
    }
    return;
}

int16_t FAttackPlayer(FLEET *lpfl, int16_t iplr) {
    int16_t iplrCur;
    int16_t iplrT;

    iplrCur = lpfl->iPlayer;
    iplrT = rglpbtlplan[iplrCur][lpfl->iplan].iplrAttack;
    switch (iplrT) {
    case 0:
        return 0;
    case 3:
        return 1;
    case 2:
        if ((int16_t)rgplr[iplrCur].rgmdRelation[iplr] == 1) {
            return 0;
        }
        return 1;
    case 1:
        if ((int16_t)rgplr[iplrCur].rgmdRelation[iplr] != 2) {
            return 0;
        }
        return 1;
    default:
        if (iplr != iplrT - 4) {
            return 0;
        }
        return 1;
    }
}

void DoBombing() {
    MessageId idmDst;
    int32_t   modKill;
    int16_t   fMulti;
    int32_t   cKillPeople;
    int32_t   dmgBombBldg;
    int32_t   cKillPeopleS;
    int32_t   cKillMine;
    int32_t   dmgBombFloor;
    MessageId idmSrc;
    int32_t   cKillDefenses;
    int32_t   cKillFact;
    int32_t   pctTerra;
    PLANET   *lppl;
    int16_t   ifl;
    FLEET    *lpfl;
    int32_t   cPPE;
    int32_t   dmgBombPeople;
    float     pctSmart;
    float     pctSuccess;
    int32_t   dmgPeopleSmart;
    double    pctSuccessHalf;
    int16_t   pctTot;
    int16_t   dChg;
    int16_t   i;
    uint16_t  t_merge_b2c2_0001;
    uint16_t  t_merge_b3b0_0001;
    uint16_t  t_merge_b5b9_0001;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->fDead == 0x0 && lpfl->idPlanet != -1 && lpfl->fBombed == 0x0) {
            lppl = lpPlanets + lpfl->idPlanet;
            if (lppl->iPlayer != lpfl->iPlayer && lppl->iPlayer != -1 && FAttackPlayer(lpfl, lppl->iPlayer) != 0 && lppl->fStarbase == 0x0 &&
                FCalcFleetBombDamage(lpfl, &dmgBombPeople, &dmgBombFloor, &dmgPeopleSmart, &dmgBombBldg, &pctTerra, &fMulti) != 0) {
                CalcPctSurvive(lppl, &pctSuccess, &pctSmart);
                if (pctSuccess < 1.0) {
                    if (dmgBombPeople > 0) {
                        dmgBombPeople = (int32_t)((double)dmgBombPeople * pctSuccess + 0.5);
                    }
                    if (dmgBombFloor > 0) {
                        dmgBombFloor = (int32_t)((double)dmgBombFloor * pctSuccess + 0.5);
                    }
                    if (dmgPeopleSmart > 0) {
                        dmgPeopleSmart = (int32_t)((double)dmgPeopleSmart * pctSmart + 0.5);
                    }
                    if (dmgBombBldg > 0) {
                        pctSuccessHalf = 1.0 - (1.0 - pctSuccess) / 2.0;
                        dmgBombBldg = (int32_t)((double)dmgBombBldg * pctSuccessHalf + 0.5);
                    }
                }
                cPPE = lppl->cMines + lppl->cFactories + (uint32_t)lppl->cDefenses;
                cKillDefenses = 0;
                cKillPeople = 0;
                cKillMine = 0;
                cKillFact = 0;
                if (dmgBombBldg > 0 && cPPE > 0) {
                    cKillFact = (uint32_t)(lppl->cFactories * dmgBombBldg);
                    modKill = (int32_t)(cKillFact % cPPE);
                    cKillFact = (int32_t)(cKillFact / cPPE);
                    if (modKill > 0) {
                        t_merge_b2c2_0001 = (int32_t)Random(LOWORD(cPPE)) < modKill ? 0x1 : 0x0;
                        cKillFact = cKillFact + (uint32_t)t_merge_b2c2_0001;
                    }
                    if (cKillFact > lppl->cFactories) {
                        cKillFact = lppl->cFactories;
                    }
                    cKillDefenses = (uint32_t)(lppl->cDefenses * dmgBombBldg);
                    modKill = (int32_t)(cKillDefenses % cPPE);
                    cKillDefenses = (int32_t)(cKillDefenses / cPPE);
                    if (modKill > 0) {
                        t_merge_b3b0_0001 = (int32_t)Random(LOWORD(cPPE)) < modKill ? 0x1 : 0x0;
                        cKillDefenses = cKillDefenses + (uint32_t)t_merge_b3b0_0001;
                    }
                    if (cKillDefenses > lppl->cDefenses) {
                        cKillDefenses = lppl->cDefenses;
                    }
                    cKillMine = dmgBombBldg - (cKillFact + cKillDefenses);
                    if (cKillMine > lppl->cMines) {
                        cKillMine = lppl->cMines;
                    }
                }
                if ((dmgBombPeople > 0 || dmgBombFloor > 0 || dmgPeopleSmart > 0) && lppl->rgwtMin[3] > 0) {
                    cKillPeopleS = (int32_t)((int32_t)(lppl->rgwtMin[3] * dmgPeopleSmart) / 1000);
                    if (cKillPeopleS >= lppl->rgwtMin[3]) {
                        cKillPeopleS = lppl->rgwtMin[3] - 1;
                    }
                    cKillPeople = (uint32_t)((lppl->rgwtMin[3] - cKillPeopleS) * dmgBombPeople);
                    modKill = (int32_t)(cKillPeople % 1000);
                    cKillPeople = (int32_t)(cKillPeople / 1000);
                    if (modKill > 0) {
                        t_merge_b5b9_0001 = (int32_t)Random(1000) <= modKill ? 0x1 : 0x0;
                        cKillPeople = cKillPeople + (uint32_t)t_merge_b5b9_0001;
                    }
                    cKillPeople = cKillPeople + cKillPeopleS;
                    if (dmgBombPeople > 0 && cKillPeople <= 0) {
                        cKillPeople = 1;
                    }
                    if (cKillPeople < dmgBombFloor) {
                        cKillPeople = dmgBombFloor;
                    }
                    if (cKillPeople > lppl->rgwtMin[3]) {
                        cKillPeople = lppl->rgwtMin[3];
                    }
                }
                if (cKillPeople > 0) {
                    lppl->rgwtMin[3] = lppl->rgwtMin[3] - cKillPeople;
                }
                if (cKillFact > 0) {
                    lppl->cFactories = lppl->cFactories - cKillFact;
                }
                if (cKillMine > 0) {
                    lppl->cMines = lppl->cMines - cKillMine;
                }
                if (cKillDefenses > 0) {
                    lppl->cDefenses = lppl->cDefenses - cKillDefenses;
                }
                if (pctTerra > 0) {
                    pctTot = 0;
                    pctTerra = pctTerra - (int32_t)((1.0 - pctSuccess) * (double)pctTerra / 2.0);
                    if (pctTerra > 500) {
                        pctTerra = 500;
                    }
                    for (i = 0; i < 3; i++) {
                        dChg = (int16_t)lppl->rgEnvVar[i] - (int16_t)lppl->rgEnvVarOrig[i];
                        if (dChg <= 0) {
                            if (dChg < 0) {
                                if ((int32_t)-dChg >= pctTerra) {
                                    dChg = -LOWORD(pctTerra);
                                }
                                lppl->rgEnvVar[i] = lppl->rgEnvVar[i] - LOBYTE(dChg);
                                pctTot = pctTot + -dChg;
                            }
                        } else {
                            if ((int32_t)dChg >= pctTerra) {
                                dChg = LOWORD(pctTerra);
                            }
                            lppl->rgEnvVar[i] = lppl->rgEnvVar[i] - LOBYTE(dChg);
                            pctTot = pctTot + dChg;
                        }
                    }
                    if (pctTot > 0) {
                        FSendPlrMsg(lpfl->iPlayer, fMulti == 0 ? 302 : 378, lpfl->id | 0x8000, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
                        FSendPlrMsg(lppl->iPlayer, fMulti == 0 ? 302 : 379, lppl->id, lpfl->id, lppl->id, pctTot, 0, 0, 0, 0);
                    }
                }
                cPPE = cKillMine + cKillFact + cKillDefenses;
                if (cPPE <= 0) {
                    if (cKillPeople > 0) {
                        if (lppl->rgwtMin[3] <= 0) {
                            idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
                            idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;
                        } else {
                            idmSrc = fMulti == 0 ? idmHasBombedKillingColonists : idmFleetsHaveBombedKillingColonists;
                            idmDst = fMulti == 0 ? idmHasBombedKillingColonists2 : idmFleetsHaveBombedKillingColonists2;
                        }
                        FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
                        FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), 0, 0, 0, 0);
                    }
                } else {
                    if (lppl->rgwtMin[3] <= 0) {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingOffEnemyColonists : idmFleetsHaveBombedKillingOffEnemyColonists;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonists3 : idmFleetsHaveBombedKillingColonists3;
                    } else {
                        idmSrc = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati : idmFleetsHaveBombedKillingColonistsDestroyingOne;
                        idmDst = fMulti == 0 ? idmHasBombedKillingColonistsDestroyingOneInstallati3 : idmFleetsHaveBombedKillingColonistsDestroyingOne3;
                        if (cPPE > 1) {
                            idmSrc = idmSrc + 1;
                            idmDst = idmDst + 1;
                        }
                        if (cKillPeople <= 0) {
                            idmSrc = idmSrc - 2;
                            idmDst = idmDst - 2;
                            if (pctSuccess != 1.0) {
                                idmSrc = idmSrc + 5;
                                idmDst = idmDst + 5;
                                FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE), (int32_t)((1.0 - pctSuccess) * 10000.0),
                                            0, 0, 0);
                                FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), (int32_t)((1.0 - pctSuccess) * 10000.0), 0, 0,
                                            0);
                                goto L_be65;
                            }
                            FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
                            FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), 0, 0, 0, 0);
                            goto L_be65;
                        }
                        if (pctSuccess != 1.0) {
                            idmSrc = idmSrc + 5;
                            idmDst = idmDst + 5;
                            FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE),
                                        (int32_t)((1.0 - pctSuccess) * 10000.0), 0, 0);
                            FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE),
                                        (int32_t)((1.0 - pctSuccess) * 10000.0), 0, 0);
                            goto L_be65;
                        }
                    }
                    FSendPlrMsg(lpfl->iPlayer, idmSrc, lpfl->id | 0x8000, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
                    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0, 0, 0);
                }
            L_be65:
                if (lppl->rgwtMin[3] == 0) {
                    UninhabitPlanet(lppl);
                }
            }
        }
    }
    return;
}
