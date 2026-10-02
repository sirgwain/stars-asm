INT_PTR CALLBACK BattlePlansDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    FARPROC lpProc;
    int16_t idc;
    int16_t i;
    int16_t fRet;
    RECT    rc;
    int16_t cLen;

L_0652:
    goto L_16d5;

L_0661:
    StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 1);
    iPlanSelDlg = 0;
    if (sel.grobj != grobjFleet)
        goto L_068d;
    else
        goto L_0684;

L_0684:
    iPlanSelDlg = sel.fl.iplan;

L_068d:
    btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
    i = 0;
    goto L_06c5;

L_06c1:
    i++;

L_06c5:
    if (i >= rgcbtlplan[idPlayer])
        goto L_0714;
    else
        goto L_06d8;

L_06d8:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
    goto L_06c1;

L_0714:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
    i = 408;
    goto L_07b7;

L_078a:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
    i++;

L_07b7:
    if (i <= 413)
        goto L_078a;
    else
        goto L_07c1;

L_07c1:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
    i = 400;
    goto L_0820;

L_07f3:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
    i++;

L_0820:
    if (i <= 407)
        goto L_07f3;
    else
        goto L_082a;

L_082a:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
    if (game.fSinglePlr != 0)
        goto L_0946;
    else
        goto L_0861;

L_0861:
    i = 120;
    goto L_0896;

L_0869:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
    i++;

L_0896:
    if (i <= 123)
        goto L_0869;
    else
        goto L_089f;

L_089f:
    i = 0;
    goto L_08ab;

L_08a7:
    i++;

L_08ab:
    if (i >= game.cPlayer)
        goto L_0901;
    else
        goto L_08b6;

L_08b6:
    if (i == idPlayer)
        goto L_08a7;
    else
        goto L_08c1;

L_08c1:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszPlayerName(i, 0, 1, 0, 0, NULL));

L_08fe:
    goto L_08a7;

L_0901:
    i = btlplan.iplrAttack;
    if (i < idPlayer + 4)
        goto L_0921;
    else
        goto L_091d;

L_091d:
    i--;

L_0921:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);
    goto L_09a8;

L_0946:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsEveryone));
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, 0, 0);
    EnableWindow(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), 0);

L_09a8:
    i = 400;
    goto L_09dd;

L_09b0:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(i));
    i++;

L_09dd:
    if (i <= 407)
        goto L_09b0;
    else
        goto L_09e7;

L_09e7:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
    fDirtyPlan = 0;
    if (gd.fTutorial == 0)
        goto L_0a5c;
    else
        goto L_0a57;

L_0a57:
    AdvanceTutor();

L_0a5c:
    return 1;

L_0a62:
    idc = 1053;
    goto L_0a8f;

L_0a6a:
    if (GET_WM_CTLCOLOR_HWND(wParam, lParam) == GetDlgItem(hwnd, idc))
        goto L_0a99;
    else
        goto L_0a8b;

L_0a8b:
    idc++;

L_0a8f:
    if (idc <= 1058)
        goto L_0a6a;
    else
        goto L_0a99;

L_0a99:
    if (idc >= 1053)
        goto L_0abf;
    else
        goto L_0aa3;

L_0aa3:
    if (HIWORD(lParam) != 6)
        goto L_16f8;
    else
        goto L_0abf;

L_0abf:
    SetBkColor((HDC)wParam, crButtonFace);
    return (INT_PTR)hbrButtonFace;

L_0ad8:
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;

L_0b00:
    if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK)
        goto L_0b12;
    else
        goto L_0b09;

L_0b09:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL)
        goto L_0bb8;
    else
        goto L_0b12;

L_0b12:
    if (fDirtyPlan == 0)
        goto L_0b6e;
    else
        goto L_0b1c;

L_0b1c:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    LogChangeBtlplan(&btlplan);

L_0b6e:
    StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 0);
    EndDialog(hwnd, iPlanSelDlg);
    if (sel.grobj != grobjFleet)
        goto L_0ba9;
    else
        goto L_0b97;

L_0b97:
    FillBattleDD(sel.fl.iplan + 1);

L_0ba9:
    iPlanSelDlg = -1;
    return 1;

L_0bb8:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_DUMP_CARGO)
        goto L_0c05;
    else
        goto L_0bc2;

L_0bc2:
    btlplan.fDumpCargo = LOWORD(SendDlgItemMessage(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO, BM_GETCHECK, 0, 0));
    fDirtyPlan = 1;
    goto L_16f8;

L_0c05:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_DELETE)
        goto L_0e25;
    else
        goto L_0c0f;

L_0c0f:
    if (fDirtyPlan == 0)
        goto L_0c71;
    else
        goto L_0c19;

L_0c19:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    LogChangeBtlplan(&btlplan);
    fDirtyPlan = 0;

L_0c71:
    btlplan.fDelete = 1;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    btlplan.iplan = iPlanSelDlg;
    if (FDeleteBattlePlan(iPlanSelDlg, 1) == 0)
        goto L_0dd0;
    else
        goto L_0cf7;

L_0cf7:
    LogChangeBtlplan(&btlplan);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg - 1, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
    i = 0;
    goto L_0d56;

L_0d52:
    i++;

L_0d56:
    if (i >= rgcbtlplan[idPlayer])
        goto L_0da5;
    else
        goto L_0d69;

L_0d69:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
    goto L_0d52;

L_0da5:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg - 1, 0);
    goto LSelectName;

L_0dd0:
    btlplan.fDelete = 0;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    goto L_16f8;

L_0e25:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_PRIMARY_TARGET)
        goto L_0e70;
    else
        goto L_0e2f;

L_0e2f:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
    btlplan.mdTarget1 = i;
    fDirtyPlan = 1;
    goto L_16f8;

L_0e70:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_SECONDARY_TARGET)
        goto L_0ec4;
    else
        goto L_0e7a;

L_0e7a:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
    btlplan.mdTarget2 = i;
    fDirtyPlan = 1;
    goto L_16f8;

L_0ec4:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_ATTACK_WHO)
        goto L_0f41;
    else
        goto L_0ece;

L_0ece:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
    if (game.fSinglePlr == 0)
        goto L_0f0d;
    else
        goto L_0f05;

L_0f05:
    i = 3;
    goto L_0f1f;

L_0f0d:
    if (i < idPlayer + 4)
        goto L_0f1f;
    else
        goto L_0f1b;

L_0f1b:
    i++;

L_0f1f:
    btlplan.iplrAttack = i;
    fDirtyPlan = 1;
    goto L_16f8;

L_0f41:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_TACTIC)
        goto L_0f92;
    else
        goto L_0f4b;

L_0f4b:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), 1031, 0, 0));
    btlplan.mdTactic = i;
    fDirtyPlan = 1;
    goto L_16f8;

L_0f92:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_RENAME)
        goto L_1134;
    else
        goto LRename;

LRename:
    StickyDlgPos(hwnd, &ptStickyBattlePlansDlg, 0);
    lpProc = MakeProcInstance(NewPlanNameDlg, hInst);
    fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc);
    FreeProcInstance(lpProc);
    SetFocus(hwnd);
    if (fRet == 0)
        goto L_10e5;
    else
        goto L_1000;

L_1000:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
    i = 0;
    goto L_1074;

L_1070:
    i++;

L_1074:
    if (i >= rgcbtlplan[idPlayer])
        goto L_10c3;
    else
        goto L_1087;

L_1087:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
    goto L_1070;

L_10c3:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);

L_10e5:
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
    goto L_16f8;

L_1134:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_COPY)
        goto L_149b;
    else
        goto L_113e;

L_113e:
    if (rgcbtlplan[idPlayer] != 15)
        goto L_1157;
    else
        goto L_1151;

L_1151:
    return 0;

L_1157:
    if (fDirtyPlan == 0)
        goto L_11b9;
    else
        goto L_1161;

L_1161:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    LogChangeBtlplan(&btlplan);
    fDirtyPlan = 0;

L_11b9:
    iPlanSelDlg = rgcbtlplan[idPlayer]++;
    cLen = strlen(btlplan.szName);
    if (cLen > 27)
        goto L_128b;
    else
        goto L_11e7;

L_11e7:
    if (btlplan.szName[cLen - 1] != ')')
        goto L_123c;
    else
        goto L_11ff;

L_11ff:
    if (isdigit(btlplan.szName[cLen - 2]) == 0)
        goto L_123c;
    else
        goto L_1224;

L_1224:
    if (btlplan.szName[cLen - 3] == '(')
        goto L_1254;
    else
        goto L_123c;

L_123c:
    strcpy(&btlplan.szName[cLen], " (2)");
    goto L_128b;

L_1254:
    if (btlplan.szName[cLen - 2] != '9')
        goto L_127d;
    else
        goto L_126c;

L_126c:
    btlplan.szName[cLen - 2] = '0';
    goto L_128b;

L_127d:
    btlplan.szName[cLen - 2] = btlplan.szName[cLen - 2] + 1;

L_128b:
    btlplan.iplan = iPlanSelDlg;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_RESETCONTENT, 0, 0);
    i = 0;
    goto L_1345;

L_1341:
    i++;

L_1345:
    if (i >= rgcbtlplan[idPlayer])
        goto L_1394;
    else
        goto L_1358;

L_1358:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_ADDSTRING, 0, (LPARAM)rglpbtlplan[idPlayer][i].szName);
    goto L_1341;

L_1394:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_SETCURSEL, iPlanSelDlg, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
    i = btlplan.iplrAttack;
    if (i < idPlayer + 4)
        goto L_1452;
    else
        goto L_144e;

L_144e:
    i--;

L_1452:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);
    fDirtyPlan = 1;
    wParam = IDC_BATTLE_PLAN_PRIMARY_TARGET;
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), 1);
    goto LRename;

L_149b:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_BATTLE_PLAN_SELECT)
        goto L_16a5;
    else
        goto LSelectName;

LSelectName:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SELECT), CB_GETCURSEL, 0, 0));
    if (i == iPlanSelDlg)
        goto L_16f8;
    else
        goto L_14d5;

L_14d5:
    if (fDirtyPlan == 0)
        goto L_1537;
    else
        goto L_14df;

L_14df:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    LogChangeBtlplan(&btlplan);
    fDirtyPlan = 0;

L_1537:
    iPlanSelDlg = i;
    btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_PRIMARY_TARGET), CB_SETCURSEL, btlplan.mdTarget1, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_SECONDARY_TARGET), CB_SETCURSEL, btlplan.mdTarget2, 0);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_DUMP_CARGO), BM_SETCHECK, btlplan.fDumpCargo, 0);
    wParam = IDC_BATTLE_PLAN_PRIMARY_TARGET;
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), iPlanSelDlg <= 0 ? 0 : 1);
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), iPlanSelDlg <= 0 ? 0 : 1);
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_TACTIC), CB_SETCURSEL, btlplan.mdTactic, 0);
    i = btlplan.iplrAttack;
    if (i < idPlayer + 4)
        goto L_1680;
    else
        goto L_167c;

L_167c:
    i--;

L_1680:
    SendMessage(GetDlgItem(hwnd, IDC_BATTLE_PLAN_ATTACK_WHO), CB_SETCURSEL, i, 0);

L_16a2:
    goto L_16f8;

L_16a5:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_HELP)
        goto L_16f8;
    else
        goto L_16ae;

L_16ae:
    WinHelp(hwnd, szHelpFile, HELP_CONTEXT, idhBattlePlansDialog);
    return 1;

L_16d5:
    if (message == WM_ERASEBKGND)
        goto L_0ad8;
    else
        goto L_16dd;

L_16dd:
    if (IS_WM_CTLCOLOR(message) != 0)
        goto L_0a62;
    else
        goto L_16e5;

L_16e5:
    if (message == WM_INITDIALOG)
        goto L_0661;
    else
        goto L_16ed;

L_16ed:
    if (message == WM_COMMAND)
        goto L_0b00;
    else
        goto L_16f8;

L_16f8:
    return 0;
}
