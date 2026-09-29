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
