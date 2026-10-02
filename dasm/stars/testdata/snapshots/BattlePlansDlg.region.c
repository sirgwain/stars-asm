INT_PTR CALLBACK BattlePlansDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    FARPROC lpProc;
    int16_t idc;
    int16_t i;
    int16_t fRet;
    RECT    rc;
    int16_t cLen;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (idc = 1053; idc <= 1058 && GET_WM_CTLCOLOR_HWND(wParam, lParam) != GetDlgItem(hwnd, idc); idc++) {
        }
        if (idc >= 1053 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
        return 0;
    }
    if (message == WM_INITDIALOG) {
        StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 1);
        iPlanSelDlg = 0;
        if (sel.grobj == grobjFleet) {
            iPlanSelDlg = sel.fl.iplan;
        }
        btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
        for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
        }
        SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);
        EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg > 0);
        EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg > 0);
        for (i = 408; i <= 413; i++) {
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
        }
        SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
        for (i = 400; i <= 407; i++) {
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
        }
        SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
        if (game.fSinglePlr == 0) {
            for (i = 120; i <= 123; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (i != idPlayer) {
                    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszPlayerName(i, 0, 1, 0, 0, NULL));
                }
            }
            i = btlplan.iplrAttack;
            if (i >= idPlayer + 4) {
                i--;
            }
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);
        } else {
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsEveryone));
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, 0, 0);
            EnableWindow(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), 0);
        }
        for (i = 400; i <= 407; i++) {
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
        }
        SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
        SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
        fDirtyPlan = 0;
        if (gd.fTutorial != 0) {
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
        case IDC_BATTLE_PLAN_DUMP_CARGO:
            btlplan.fDumpCargo = LOWORD(SendDlgItemMessage(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO, BM_GETCHECK, 0, 0));
            fDirtyPlan = 1;
            break;
        case IDC_DELETE:
            if (fDirtyPlan != 0) {
                rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                LogChangeBtlplan(&btlplan);
                fDirtyPlan = 0;
            }
            btlplan.fDelete = 1;
            rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
            btlplan.iplan = iPlanSelDlg;
            if (FDeleteBattlePlan(iPlanSelDlg, 1) != 0) {
                LogChangeBtlplan(&btlplan);
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg - 1, 0);
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
                for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
                }
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg - 1, 0);
                goto LSelectName;
            }
            btlplan.fDelete = 0;
            rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
            break;
        case IDC_BATTLE_PLAN_PRIMARY_TARGET:
            i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
            btlplan.mdTarget1 = i;
            fDirtyPlan = 1;
            break;
        case IDC_BATTLE_PLAN_SECONDARY_TARGET:
            i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
            btlplan.mdTarget2 = i;
            fDirtyPlan = 1;
            break;
        case IDC_BATTLE_PLAN_ATTACK_WHO:
            i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
            if (game.fSinglePlr != 0) {
                i = 3;
            } else if (i >= idPlayer + 4) {
                i++;
            }
            btlplan.iplrAttack = i;
            fDirtyPlan = 1;
            break;
        case IDC_BATTLE_PLAN_TACTIC:
            i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
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
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
                for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
                }
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);
            }
            EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg > 0);
            EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg > 0);
            break;
        case IDC_BATTLE_PLAN_COPY:
            if (rgcbtlplan[idPlayer] == 15) {
                return 0;
            }
            if (fDirtyPlan != 0) {
                rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                LogChangeBtlplan(&btlplan);
                fDirtyPlan = 0;
            }
            iPlanSelDlg = rgcbtlplan[idPlayer]++;
            cLen = strlen(btlplan.szName);
            if (cLen <= 27) {
                if (btlplan.szName[cLen - 1] != ')' || isdigit(btlplan.szName[cLen - 2]) == 0 || btlplan.szName[cLen - 3] != '(') {
                    strcpy(&btlplan.szName[cLen], " (2)");
                } else if (btlplan.szName[cLen - 2] == '9') {
                    btlplan.szName[cLen - 2] = '0';
                } else {
                    btlplan.szName[cLen - 2] = btlplan.szName[cLen - 2] + 1;
                }
            }
            btlplan.iplan = iPlanSelDlg;
            rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
            for (i = 0; i < rgcbtlplan[idPlayer]; i++) {
                SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
            }
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
            i = btlplan.iplrAttack;
            if (i >= idPlayer + 4) {
                i--;
            }
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);
            fDirtyPlan = 1;
            wParam = IDC_BATTLE_PLAN_PRIMARY_TARGET;
            EnableWindow(GetDlgItem(hwnd, IDC_RENAME), 1);
            goto LRename;
        case IDC_BATTLE_PLAN_SELECT:
        LSelectName:
            i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_GETCURSEL, 0, 0));
            if (i == iPlanSelDlg)
                break;
            if (fDirtyPlan != 0) {
                rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
                LogChangeBtlplan(&btlplan);
                fDirtyPlan = 0;
            }
            iPlanSelDlg = i;
            btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
            wParam = IDC_BATTLE_PLAN_PRIMARY_TARGET;
            EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg > 0);
            EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg > 0);
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
            i = btlplan.iplrAttack;
            if (i >= idPlayer + 4) {
                i--;
            }
            SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);
            break;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, HELP_CONTEXT, idhBattlePlansDialog);
            return 1;
        }
    }
    return 0;
}
