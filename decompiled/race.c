#include "common.h"

int16_t rgRacePrimaryTrait[10] = {40, 95, 45, 10, -100, -150, 120, 180, 90, -66};
char    rgRW3Spacing[7] = {4, 3, 3, 3, 3, 3, 3};
int16_t rgRaceAdvDisPts[14] = {-235, -25, -159, -201, 40, -240, -155, 160, 240, 255, 325, 180, 70, 30};
char    rgRW3IStat[7] = {0, 1, 2, 3, 4, 5, 6};
int16_t rgRaceDisEnvPts[6] = {150, 330, 540, 780, 1050, 1380};
char    rgRW3Width[7] = {-2, 2, 2, 2, -2, 2, 2};
char    rgRaceStatMax[16] = {25, 15, 25, 25, 25, 15, 25, 6, 2, 2, 2, 2, 2, 2, 9};
char    rgRaceStatMin[16] = {7, 5, 5, 5, 5, 2, 5};

int16_t RaceCreationWizard(HWND hwndParent, int16_t fReadOnly, int16_t fDontWrite) {
    int16_t mdRet;
    FARPROC lpProc;
    RECT    rgrcStack[17];
    int16_t cpts;

    vrgrcRCW = rgrcStack;
    fRCWReadOnly = fReadOnly;
    hwndRaceParent = hwndParent;
    while (1) {
        iPanelActive = 1;
        lpProc = MakeProcInstance(RaceWizardDlg1, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_1), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        if (mdRet == 0)
            break;
        if (mdRet == 3) {
        Finish:
            if (fRCWReadOnly != 0) {
                return 0;
            }
            cpts = CAdvantagePoints(&vplr);
            if (cpts >= 0) {
                lSaltCur = LSaltFromSz(szRacePass);
                lSaltLast = -5;
                if (FCheckPassword() == 0 || FSaveRace((int16_t)szRaceFile[0] == 0 ? "stars.r1" : szRaceFile, &vplr) == 0)
                    continue;
                return 1;
            }
            _wsprintf(szWork, PszGetCompressedString(idsAdvantagePointsCurrentlyHoleDPointsCannot), -cpts);
            AlertSz(szWork, MB_ICONHAND);
            switch (iPanelActive) {
            default:
                continue;
            case 2:
                break;
            case 3:
                goto Step3;
            case 4:
                goto Step4;
            case 5:
                goto Step5;
            case 6:
                goto Step6;
            }
        }
    Step2:
        iPanelActive = 2;
        lpProc = MakeProcInstance(RaceWizardDlg4, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_4), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        switch (mdRet) {
        case 1:
            continue;
        case 0:
            return 0;
        default:
            break;
        case 3:
            goto Finish;
        }
    Step3:
        iPanelActive = 3;
        lpProc = MakeProcInstance(RaceWizardDlg5, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_5), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        switch (mdRet) {
        case 1:
            goto Step2;
        case 0:
            return 0;
        default:
            break;
        case 3:
            goto Finish;
        }
    Step4:
        iPanelActive = 4;
        lpProc = MakeProcInstance(RaceWizardDlg2, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_2), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        switch (mdRet) {
        case 1:
            goto Step3;
        case 0:
            return 0;
        default:
            break;
        case 3:
            goto Finish;
        }
    Step5:
        iPanelActive = 5;
        lpProc = MakeProcInstance(RaceWizardDlg3, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_3), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        switch (mdRet) {
        case 1:
            goto Step4;
        case 0:
            return 0;
        default:
            break;
        case 3:
            goto Finish;
        }
    Step6:
        iPanelActive = 6;
        lpProc = MakeProcInstance(RaceWizardDlg6, hInst);
        mdRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RACE_WIZARD_6), hwndRaceParent, lpProc);
        FreeProcInstance(lpProc);
        switch (mdRet) {
        case 1:
            goto Step5;
        case 0:
            return 0;
        case 3:
        default:
            goto Finish;
        }
    }
    return 0;
}

INT_PTR CALLBACK RaceWizardDlg1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    int16_t     iPlrBmp;
    HWND        hwndCB;
    char       *psz;
    POINT16     pt;
    HDC         hdc;
    BTNT        btnt;
    int16_t     bt;
    RECT       *prc;
    int16_t     iDir;
    int16_t     iCur;
    int16_t     iOffset;
    PLAYER     *pplr;
    PAINTSTRUCT ps;
    int16_t     j;
    int16_t     cch;
    RECT        rcGBox;
    int16_t     k;
    char        szBuf[32];
    HWND        t_scratch_me;
    HWND        t_call_0f64;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        for (j = 271; j <= 278 && IsDlgButtonChecked(hwnd, j) == 0x0; j++) {
        }
        if (j > 277) {
            pplr = &vplr;
        } else {
            k = j - 271;
            pplr = &vrgplrDef[k];
        }
        GetClientRect(hwnd, &rc);
        DrawRaceAdvantagePoints(hdc, &rc, pplr);
        GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0116), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 0x1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        cch = CchGetString(idsPredefinedRaces, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, cch);
        GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox.right);
        pt.x = rcGBox.right + 32;
        pt.y = rcGBox.bottom - 32;
        iOffset = vplr.iPlrBmp;
        if (iOffset >= 32) {
            iOffset = 0;
        }
        SelectPalette(hdc, vhpal, 0);
        RealizePalette(hdc);
        DibBlt(hdc, pt.x, pt.y, 32, 32, hdibRaces, (iOffset & 0x7) * 0x20, (0x3 - (iOffset >> 0x3)) * 0x20, 32, 32, 13369376);
        if (fRCWReadOnly == 0) {
            SetRect(rgrcBuildSpin, pt.x + 2, pt.y + 35, pt.x + 16, pt.y + 49);
            rgrcBuildSpin[1] = rgrcBuildSpin[0];
            OffsetRect(&rgrcBuildSpin[1], 14, 0);
            for (i = 0; i < 2; i++) {
                DrawBtn(hdc, &rgrcBuildSpin[i], (i == 0 ? 0x2 : 0x3) | 0x20, 0, 0x0);
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
        for (i = 271; i <= 278; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 278 || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        switch (message) {
        case WM_INITDIALOG:
            iPlrBmp = vplr.iPlrBmp;
            SetRCWTitle(hwnd, iPanelActive);
            SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
            SetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames);
            if ((int16_t)vplr.szName[0] == 0) {
                GetDlgItemText(hwnd, IDC_RADRACE1, vplr.szName, 16);
                SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            if (game.fTutorial == 0x0 || idPlayer != 0 || fRCWReadOnly == 0) {
                for (i = 0; i < 7; i++) {
                    vplr.iPlrBmp = vrgplrDef[i].iPlrBmp;
                    if (fmemcmp(&vplr, &vrgplrDef[i], 0x80) == 0)
                        break;
                }
                vplr.iPlrBmp = iPlrBmp;
            } else {
                i = 0;
            }
            CheckRadioButton(hwnd, 271, 278, i + 271);
            SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0xf, 0);
            SendDlgItemMessage(hwnd, 2075, EM_LIMITTEXT, 0xf, 0);
            SendDlgItemMessage(hwnd, 269, EM_LIMITTEXT, 0x10, 0);
            hwndCB = GetDlgItem(hwnd, IDC_COMBOBOX);
            for (i = 262; i <= 266; i++) {
                psz = PszGetCompressedString(i);
                SendMessage(hwndCB, CB_ADDSTRING, 0x0, (LPARAM)psz);
            }
            i = GetRaceStat(&vplr, rsUseLeftover);
            SendMessage(hwndCB, CB_SETCURSEL, i, 0);
            if (vplr.lSalt != 0) {
                SetDlgItemText(hwnd, IDC_U16_0x010D, szRacePass);
            }
            if (fRCWReadOnly != 0) {
                for (i = 268; i <= 269; i++) {
                    EnableWindow(GetDlgItem(hwnd, i), 0);
                }
                EnableWindow(GetDlgItem(hwnd, IDC_EDITNAME), 0);
                for (i = 271; i <= 278; i++) {
                    EnableWindow(GetDlgItem(hwnd, i), 0);
                }
                EnableWindow(hwndCB, 0);
            }
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            if (fRCWReadOnly != 0 || (PtInRect(rgrcBuildSpin, PointFrom16(pt)) == 0 && PtInRect(&rgrcBuildSpin[1], PointFrom16(pt)) == 0))
                break;
            if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) == 0) {
                iDir = 1;
                bt = 35;
                prc = &rgrcBuildSpin[1];
            } else {
                iDir = -1;
                bt = 34;
                prc = rgrcBuildSpin;
            }
            iCur = vplr.iPlrBmp;
            if (iCur >= 32) {
                iCur = 0;
            }
            hdc = GetDC(hwnd);
            SelectPalette(hdc, vhpal, 0);
            RealizePalette(hdc);
            InitBtnTrack(&btnt, hwnd, 0x0, prc, bt, 80, 0, 0, 0x0);
            while (FTrackBtn(&btnt) != 0) {
                iCur = (int32_t)(iCur + 32 + iDir) % 32;
                DibBlt(hdc, rgrcBuildSpin[0].left - 2, rgrcBuildSpin[0].top - 35, 32, 32, hdibRaces, (iCur & 0x7) * 0x20, (0x3 - (iCur >> 0x3)) * 0x20, 32, 32,
                       13369376);
            }
            vplr.iPlrBmp = iCur;
            ReleaseDC(hwnd, hdc);
            return 1;
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 0x1, 0x3ff);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL) {
                    iPlrBmp = vplr.iPlrBmp;
                    for (j = 271; j <= 278 && IsDlgButtonChecked(hwnd, j) == 0x0; j++) {
                    }
                    if (j <= 277) {
                        k = j - 271;
                        vplr = vrgplrDef[k];
                    }
                    GetDlgItemText(hwnd, IDC_EDIT1, vplr.szName, 32);
                    GetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames, 32);
                    GetRaceStat(&vplr, rsUseLeftover);
                    GetDlgItemText(hwnd, IDC_U16_0x010D, szRacePass, 16);
                    j = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_GETCURSEL, 0x0, 0));
                    SetRaceStat(&vplr, rsUseLeftover, j);
                    vplr.lSalt = LSaltFromSz(szRacePass);
                    vplr.iPlrBmp = iPlrBmp;
                }
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_RADRACE1 &&
                GET_WM_COMMAND_ID(wParam, lParam) <= IDC_U16_0x0116) {
                memset(vplr.szName, 0, 0x20);
                GetDlgItemText(hwnd, IDC_EDIT1, vplr.szName, 32);
                memset(vplr.szNames, 0, 0x20);
                GetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames, 32);
                GetDlgItemText(hwnd, GET_WM_COMMAND_ID(wParam, lParam), szBuf, 32);
                for (i = 0; i < 7 && strcmp(vplr.szName, PszGetCompressedString(i + 1383)) != 0; i++) {
                }
                if (i < 7 && GET_WM_COMMAND_ID(wParam, lParam) < IDC_U16_0x0116) {
                    memset(vplr.szName, 0, 0x20);
                    CchGetString(GET_WM_COMMAND_ID(wParam, lParam) + 1112, vplr.szName);
                    SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
                    memset(vplr.szNames, 0, 0x20);
                    psz = PszPlayerName(0, 1, 1, 0, 0, &vplr);
                    strcpy(vplr.szNames, psz);
                    SetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames);
                }
                if (GET_WM_COMMAND_ID(wParam, lParam) > 0x115) {
                    pplr = &vplr;
                } else {
                    pplr = &vrgplrDef[GET_WM_COMMAND_ID(wParam, lParam) - 271];
                }
                InvalidateAdvPtsRect(hwnd);
                i = GetRaceStat(pplr, rsUseLeftover);
                SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_SETCURSEL, i, 0);
                t_call_0f64 = GetDlgItem(hwnd, IDC_NEXT);
                EnableWindow(t_call_0f64, GET_WM_COMMAND_ID(wParam, lParam) == 0x115 ? 0 : 1);
                vplr.iPlrBmp = pplr->iPlrBmp;
                GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &rc);
                ScreenToClient(hwnd, (POINT *)&rc.right);
                rc.left = rc.right + 32;
                rc.top = rc.bottom - 32;
                rc.right = rc.left + 32;
                rc.bottom = rc.top + 32;
                InvalidateRect(hwnd, &rc, 1);
            }
        default:
        }
    }
    return 0;
}

INT_PTR CALLBACK RaceWizardDlg2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    int16_t     yTop;
    int16_t     dy;
    int16_t     dxMiddle;
    int16_t     dxLabel;
    int16_t     cch;
    char        szTemp[20];
    HFONT       hfontSav;
    PAINTSTRUCT ps;
    POINT16     pt;
    int16_t     iVar;
    uint16_t    t_scratch_m30;
    HWND        t_scratch_me;
    POINT       t_pt_15a4;
    POINT       t_pt_15b3_1;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        viStore = -2;
        DrawRace2(hwnd, hdc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 291; i <= 293; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 293 || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        switch (message) {
        case WM_INITDIALOG:
            SetRCWTitle(hwnd, iPanelActive);
            viStore = -1;
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &rc);
            hfontSav = SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsTemperature, szTemp);
            dxLabel = LOWORD(GetTextExtent(hdc, szTemp, cch)) + 10;
            dxMiddle = LOWORD(GetTextExtent(hdc, "200mR", 5)) + 10;
            dxMiddle = rc.right - dxLabel - dxMiddle;
            dy = (int32_t)(3 * dyArial8) / 2;
            yTop = 3 * dyArial8;
            SetRect(vrgrcRCW, dxLabel, yTop, dxLabel + dy, yTop + dy);
            SetRect(vrgrcRCW + 1, dxLabel + dy + 6, yTop, dxLabel + dxMiddle - dy - 6, yTop + dy);
            SetRect(vrgrcRCW + 2, dxLabel + dxMiddle - dy, yTop, dxLabel + dxMiddle, yTop + dy);
            SetRect(vrgrcRCW + 3, dxLabel, yTop + dy + 4, 3 * dy + dxLabel, dy * 2 + yTop + 4);
            SetRect(vrgrcRCW + 4, dxLabel + dxMiddle - 3 * dy, yTop + dy + 4, dxLabel + dxMiddle, dy * 2 + yTop + 4);
            for (i = 0; i < 5; i++) {
                vrgrcRCW[i + 5] = vrgrcRCW[i];
                OffsetRect((RECT *)&vrgrcRCW[i + 5].left, 0, 3 * dy);
                vrgrcRCW[i + 10] = vrgrcRCW[i];
                OffsetRect((RECT *)&vrgrcRCW[i + 10].left, 0, 6 * dy);
            }
            for (i = 0; i < 3; i++) {
                SetWindowPos(GetDlgItem(hwnd, i + 291), 0x0, 3 * dy + dxLabel + 6, vrgrcRCW[5 * i + 3].top, dxMiddle - 6 * dy - 12, dy, SWP_NOZORDER);
                CheckDlgButton(hwnd, i + 291, (int16_t)vplr.rgEnvVarMax[i] >= 0 ? 0x0 : 0x1);
            }
            cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
            t_scratch_m30 = LOWORD(GetTextExtent(hdc, "15%", 3));
            vrgrcRCW[15].left = dxLabel + LOWORD(GetTextExtent(hdc, szWork, cch)) + t_scratch_m30 + 4;
            vrgrcRCW[15].top = 9 * dy + yTop - 3;
            vrgrcRCW[15].right = vrgrcRCW[15].left + 15;
            vrgrcRCW[15].bottom = (dyArial8 >> 0x1) + vrgrcRCW[15].top + 0x3;
            vrgrcRCW[16] = vrgrcRCW[15];
            OffsetRect(vrgrcRCW + 16, 0, vrgrcRCW[15].bottom - vrgrcRCW[15].top - 1);
            crcRCW = 17;
            SelectObject(hdc, hfontSav);
            ReleaseDC(hwnd, hdc);
            if (fRCWReadOnly != 0) {
                for (i = 291; i <= 293; i++) {
                    EnableWindow(GetDlgItem(hwnd, i), 0);
                }
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            return 1;
        case WM_SETCURSOR:
            GetCursorPos(&t_pt_15a4);
            pt = PointTo16(t_pt_15a4);
            t_pt_15b3_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_15b3_1);
            pt = PointTo16(t_pt_15b3_1);
            if (IrcRaceDlgHitTest(pt) < 0)
                break;
            SetCursor(hcurHand);
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            return FTrackRaceDlg2(hwnd, pt, wParam);
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 0x1, 0x41d);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_ID(wParam, lParam) >= IDC_U16_0x0123 && GET_WM_COMMAND_ID(wParam, lParam) <= IDC_IMMUNE_TO_RADIATION) {
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0x0, 0));
                iVar = GET_WM_COMMAND_ID(wParam, lParam) - 291;
                if (i != 1) {
                    vplr.rgEnvVarMin[iVar] = 20;
                    vplr.rgEnvVarMax[iVar] = 80;
                    vplr.rgEnvVar[iVar] = 50;
                } else {
                    vplr.rgEnvVar[iVar] = -1;
                    vplr.rgEnvVarMax[iVar] = -1;
                    vplr.rgEnvVarMin[iVar] = -1;
                }
                DrawRace2(hwnd, 0x0, 0x1 << iVar | 0xff00);
            }
        default:
        }
    }
    return 0;
}

void DrawRace2(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  iPit;
    int16_t  bt;
    int16_t  iMax;
    char     szT[32];
    int16_t  dy;
    int16_t  iMin;
    int16_t  bkMode;
    int16_t  fCreatedDC;
    int16_t  xRLabel;
    int16_t  i;
    int16_t  iMod;
    char    *psz;
    int16_t  dx;
    int16_t  cch;
    int16_t  bt1;
    RECT     rc;
    int32_t  l2;
    int16_t  iStore;
    int32_t  l;
    StringId t_merge_207b_0001;

    fCreatedDC = 0;
    bt1 = fRCWReadOnly == 0 ? 0 : 4;
    if (hdc == 0x0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    DrawRaceAdvantagePoints(hdc, &rc, 0x0);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    xRLabel = (int32_t)(rc.right - vrgrcRCW[2].right) / 2 + vrgrcRCW[2].right;
    for (i = 0; i < 3; i++) {
        if ((0x1 << i & iDraw) != 0x0) {
            if (iDraw != -1) {
                SelectObject(hdc, hbrButtonFace);
                PatBlt(hdc, vrgrcRCW[2].right, vrgrcRCW[5 * i + 2].top, rc.right - vrgrcRCW[2].right, vrgrcRCW[5 * i + 4].bottom - vrgrcRCW[5 * i + 2].top,
                       PATCOPY);
            }
            if ((int16_t)vplr.rgEnvVarMax[i] >= 0) {
                psz = PszCalcEnvVar(i, (int16_t)vplr.rgEnvVarMin[i]);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1, psz, 0);
                cch = CchGetString(idsTo2, szT);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8, szT, cch);
                psz = PszCalcEnvVar(i, (int16_t)vplr.rgEnvVarMax[i]);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8 * 2, psz, 0);
            } else {
                cch = CchGetString(idsN2, szT);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8, szT, cch);
            }
            if (iDraw == -1) {
                RightTextOut(hdc, vrgrcRCW[5 * i].left - 4, (int32_t)dyArial8 / 4 + vrgrcRCW[5 * i].top, rgszPlanetAttr[i], 0, 0);
            }
        }
    }
    iPit = 0;
    iMod = 0;
    i = 0;
    while (i < 15) {
        if (iMod >= 5) {
            iMod = 0;
        }
        bt = (int16_t)vplr.rgEnvVarMax[(int32_t)i / 5] >= 0 ? 0 : 4;
        switch (iMod) {
        case 0:
        case 2:
            if ((iDraw & 0xfff0) == 0x0)
                break;
            DrawBtn(hdc, vrgrcRCW + i, (iMod == 0 ? 0x2 : 0x3) | bt | bt1, 0, 0x0);
            break;
        default:
            if ((iDraw & 0xfff0) == 0x0)
                break;
            DrawBtn(hdc, vrgrcRCW + i, 0x8 | bt | bt1, 0, iMod == 3 ? "<<     >>" : ">>     <<");
            break;
        case 1:
            if ((0x1 << iPit & iDraw) != 0x0) {
                PatBlt(hdc, vrgrcRCW[i].left, vrgrcRCW[i].top, vrgrcRCW[i].right - vrgrcRCW[i].left, vrgrcRCW[i].bottom - vrgrcRCW[i].top, BLACKNESS);
                if (bt == 0) {
                    iMin = (int16_t)vplr.rgEnvVarMin[iPit];
                    iMax = (int16_t)vplr.rgEnvVarMax[iPit];
                    dx = vrgrcRCW[i].right - vrgrcRCW[i].left - 2;
                    dy = vrgrcRCW[i].bottom - vrgrcRCW[i].top - 2;
                    SelectObject(hdc, rghbrPlanetAttr[iPit][0]);
                    PatBlt(hdc, MulDiv(iMin, dx, 100) + vrgrcRCW[i].left + 1, vrgrcRCW[i].top + 1, MulDiv(iMax - iMin, dx, 100), dy, PATCOPY);
                }
            }
            iPit = iPit + 1;
        }
        i = i + 1;
        iMod = iMod + 1;
    }
    if ((iDraw & 0x8) != 0x0) {
        if (iDraw != -1) {
            dx = LOWORD(GetTextExtent(hdc, "15%", 3));
            SelectObject(hdc, hbrButtonFace);
            PatBlt(hdc, vrgrcRCW[15].left - 4 - dx, vrgrcRCW[15].top + 3, dx, dyArial8, PATCOPY);
        } else {
            cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
            TextOut(hdc, vrgrcRCW->left, vrgrcRCW[15].top + 3, szWork, cch);
            DrawBtn(hdc, vrgrcRCW + 15, 0xa0 | bt1, 0, 0x0);
            DrawBtn(hdc, vrgrcRCW + 16, 0xa1 | bt1, 0, 0x0);
        }
        cch = _wsprintf(szWork, PCTDPCTPCT, (int16_t)vplr.pctIdealGrowth);
        RightTextOut(hdc, vrgrcRCW[15].left - 4, vrgrcRCW[15].top + 3, szWork, cch, 0);
    }
    if ((iDraw & 0x7) != 0x0) {
        l = 1;
        for (i = 0; i < 3; i++) {
            if ((int16_t)vplr.rgEnvVarMax[i] >= 0 && (int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i] != 0x64) {
                if (i != 2) {
                    l2 = 0;
                    for (iStore = (int16_t)vplr.rgEnvVarMin[i]; iStore <= (int16_t)vplr.rgEnvVarMax[i]; iStore++) {
                        if (iStore >= 10) {
                            if (iStore >= 90) {
                                l2 = l2 + (int32_t)(100 - iStore);
                            } else {
                                l2 = l2 + 10;
                            }
                        } else {
                            l2 = l2 + (int32_t)iStore;
                        }
                    }
                    l = (int32_t)((int32_t)(l * l2) / 9);
                } else {
                    l = (uint32_t)(l * (int32_t)((int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i]));
                }
            } else {
                l = (uint32_t)(l * 100);
            }
        }
        if (l < 1) {
            l = 1;
        }
        l2 = (int32_t)(((int32_t)(l >> 0x1) + 0xf4240) / l);
        iStore = LOWORD(l2);
        if (l == 1000000) {
            iStore = 0;
        }
        if (iStore != viStore) {
            viStore = iStore;
            if (l2 != 1) {
                CchGetString(idsCanExpect1DPlanetsWillHabitable, szT);
                cch = _wsprintf(szWork, szT, l2);
            } else {
                t_merge_207b_0001 = l == 1000000 ? idsPlanetsWillHabitableRace : idsVirtuallyPlanetsWillHabitableRace;
                cch = CchGetString(t_merge_207b_0001, szWork);
            }
            rc.left = vrgrcRCW->left;
            rc.top = vrgrcRCW[15].top + dyArial8 + 8;
            rc.right = vrgrcRCW[15].right + 20;
            rc.bottom = 3 * dyArial8 + rc.top;
            SelectObject(hdc, hbrButtonFace);
            PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, PATCOPY);
            DrawText(hdc, szWork, cch, &rc, 0x10);
        }
    }
    SetBkMode(hdc, bkMode);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

int16_t IrcRaceDlgHitTest(POINT16 pt) {
    int16_t i;

    if (fRCWReadOnly == 0) {
        for (i = 0; i < crcRCW && PtInRect((RECT *)&vrgrcRCW[i].left, PointFrom16(pt)) == 0; i++) {
        }
        if (i >= crcRCW) {
            return -1;
        }
        if (iPanelActive != 4 || (int16_t)vplr.rgEnvVarMax[(int32_t)i / 5] >= 0) {
            return i;
        }
        return -1;
    }
    return -1;
}

int16_t FTrackRaceDlg2(HWND hwnd, POINT16 pt, int16_t kbd) {
    BTNT     btnt;
    int16_t  bt;
    int16_t  dShift;
    int8_t   iMax;
    int8_t   iMin;
    int16_t  i;
    int16_t  irc;
    int16_t  iMod;
    char    *psz;
    int16_t  dWidth;
    int16_t  dx;
    int16_t  t_merge_2388_0001;
    uint16_t t_merge_2587_0001;

    irc = IrcRaceDlgHitTest(pt);
    if (irc >= 0) {
        iMod = (int32_t)irc % 5;
        i = (int32_t)irc / 5;
        if (iMod != 1 || irc >= 15) {
            dWidth = 0;
            dShift = 0;
            psz = 0x0;
            if (irc != 15) {
                if (irc != 16) {
                    switch (iMod) {
                    case 0:
                        dShift = -1;
                        bt = 2;
                        break;
                    case 2:
                        dShift = 1;
                        bt = 3;
                        break;
                    case 3:
                        dWidth = 1;
                        bt = 8;
                        psz = vrgszRCWWidth[0];
                        break;
                    default:
                        dWidth = -1;
                        bt = 8;
                        psz = vrgszRCWWidth[1];
                    }
                } else {
                    dShift = -1;
                    bt = 161;
                }
            } else {
                dShift = 1;
                bt = 160;
            }
            InitBtnTrack(&btnt, hwnd, 0x0, vrgrcRCW + irc, bt, 80, 0, 0, psz);
            if ((kbd & 0x4) != 0x0) {
                dWidth = 10 * dWidth;
                dShift = 10 * dShift;
            }
            while (FTrackBtn(&btnt) != 0) {
                if (irc != 15 && irc != 16) {
                    iMin = LOBYTE((int16_t)vplr.rgEnvVarMin[i] - (dWidth - dShift));
                    iMax = LOBYTE((int16_t)vplr.rgEnvVarMax[i] + (dWidth + dShift));
                    if ((int16_t)iMax > 100) {
                        iMin = iMin - LOBYTE((int16_t)iMax - 100);
                        iMax = 100;
                    }
                    if ((int16_t)iMin < 0) {
                        iMax = LOBYTE(0x64 >= (int16_t)iMax - (int16_t)iMin ? (int16_t)iMax - (int16_t)iMin : 0x64);
                        iMin = 0;
                    }
                    dx = (int16_t)iMax - (int16_t)iMin;
                    if (dx < 20) {
                        dx = (0x14 - dx) >> 0x1;
                        iMin = iMin - LOBYTE(dx);
                        iMax = iMax + LOBYTE(dx);
                    }
                    if ((int16_t)vplr.rgEnvVarMin[i] != (int16_t)iMin || (int16_t)vplr.rgEnvVarMax[i] != (int16_t)iMax) {
                        vplr.rgEnvVarMin[i] = iMin;
                        vplr.rgEnvVarMax[i] = iMax;
                        vplr.rgEnvVar[i] = LOBYTE((int16_t)vplr.rgEnvVarMin[i] + (int32_t)((int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i]) / 0x2);
                        DrawRace2(hwnd, btnt.hdc, 0x1 << i);
                    }
                } else {
                    iMin = LOBYTE((int16_t)vplr.pctIdealGrowth + dShift);
                    if (0x14 >= (1 <= (int16_t)iMin ? (int16_t)iMin : 0x1)) {
                        if (1 <= (int16_t)iMin) {
                            t_merge_2587_0001 = (int16_t)iMin;
                        } else {
                            t_merge_2587_0001 = 0x1;
                        }
                    } else {
                        t_merge_2587_0001 = 0x14;
                    }
                    iMin = LOBYTE(t_merge_2587_0001);
                    if ((int16_t)iMin != (int16_t)vplr.pctIdealGrowth) {
                        vplr.pctIdealGrowth = iMin;
                        DrawRace2(hwnd, btnt.hdc, 8);
                    }
                }
            }
            if (irc < 15) {
                vplr.rgEnvVar[i] = LOBYTE((int16_t)vplr.rgEnvVarMin[i] + (int32_t)((int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i]) / 0x2);
            }
            return 1;
        }
        SetCapture(hwnd);
        SetCursor(hcurCloseGrab);
        dWidth = (int32_t)((int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i]) / 2;
        while (FGetMouseMove(&pt) != 0) {
            if (pt.x < vrgrcRCW[irc].left) {
                pt.x = vrgrcRCW[irc].left;
            }
            if (pt.x > vrgrcRCW[irc].right) {
                pt.x = vrgrcRCW[irc].right;
            }
            dShift = MulDiv(pt.x - vrgrcRCW[irc].left, 100, vrgrcRCW[irc].right - vrgrcRCW[irc].left);
            if (dWidth <= (dShift >= 100 - dWidth ? 100 - dWidth : dShift)) {
                if (dShift >= 100 - dWidth) {
                    t_merge_2388_0001 = 100 - dWidth;
                } else {
                    t_merge_2388_0001 = dShift;
                }
            } else {
                t_merge_2388_0001 = dWidth;
            }
            dShift = t_merge_2388_0001;
            if ((int16_t)vplr.rgEnvVarMax[i] != dShift + dWidth) {
                vplr.rgEnvVarMin[i] = LOBYTE(dShift - dWidth);
                vplr.rgEnvVarMax[i] = LOBYTE(dShift + dWidth);
                vplr.rgEnvVar[i] = LOBYTE((int16_t)vplr.rgEnvVarMin[i] + (int32_t)((int16_t)vplr.rgEnvVarMax[i] - (int16_t)vplr.rgEnvVarMin[i]) / 0x2);
                DrawRace2(hwnd, 0x0, 0x1 << i);
            }
        }
        ReleaseCapture();
        return 1;
    }
    return 0;
}

INT_PTR CALLBACK RaceWizardDlg3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    POINT16     pt;
    HDC         hdc;
    PAINTSTRUCT ps;
    HWND        t_scratch_me;
    POINT       t_pt_28b3;
    POINT       t_pt_28c2_1;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        DrawRace3(hwnd, hdc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
        if (t_scratch_me == GetDlgItem(hwnd, IDC_U16_0x0123) || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        switch (message) {
        case WM_INITDIALOG:
            SetRCWTitle(hwnd, iPanelActive);
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), BM_SETCHECK, GetRaceGrbit(&vplr, ibitRaceCheapFact), 0);
            if (fRCWReadOnly != 0 || GetRaceStat(&vplr, rsMajorAdv) == raMacintosh) {
                EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0123), 0);
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            return 1;
        case WM_SETCURSOR:
            GetCursorPos(&t_pt_28b3);
            pt = PointTo16(t_pt_28b3);
            t_pt_28c2_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_28c2_1);
            pt = PointTo16(t_pt_28c2_1);
            if (IrcRaceDlgHitTest(pt) < 0)
                break;
            SetCursor(hcurHand);
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            return FTrackRaceDlg3(hwnd, pt, wParam);
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 0x1, 0x420);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x0123) {
                i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), BM_GETCHECK, 0x0, 0));
                SetRaceGrbit(&vplr, ibitRaceCheapFact, i);
                DrawRace3(hwnd, 0x0, 99);
            }
        default:
        }
    }
    return 0;
}

void DrawRace3(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  dxItem;
    int16_t  idsT;
    int16_t  fMacintosh;
    int16_t  yTop;
    int16_t  bt;
    StringId ids;
    COLORREF crBkSav;
    int16_t  bkMode;
    int16_t  fCreatedDC;
    int16_t  dxkT;
    int16_t  i;
    int16_t  irc;
    int16_t  dxDig;
    int16_t  dx;
    int16_t  cch;
    RECT     rc;

    fCreatedDC = 0;
    bt = fRCWReadOnly == 0 ? 0 : 4;
    fMacintosh = GetRaceStat(&vplr, rsMajorAdv) == raMacintosh ? 1 : 0;
    if (hdc == 0x0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    DrawRaceAdvantagePoints(hdc, &rc, 0x0);
    bkMode = SetBkMode(hdc, OPAQUE);
    crBkSav = SetBkColor(hdc, crButtonFace);
    SetTextColor(hdc, crWindowText);
    SelectObject(hdc, rghfontArial8[1]);
    yTop = 3 * dyArial8 + 6;
    dxDig = LOWORD(GetTextExtent(hdc, "9", 1));
    dxkT = LOWORD(GetTextExtent(hdc, "kT", 2));
    ids = idsOneResourceGeneratedEachYearEvery;
    irc = 0;
    for (i = 0; i < 7; i++) {
        if (i == 1 && fMacintosh != 0) {
            SetTextColor(hdc, crButtonShadow);
            bt = 4;
        }
        if (i == 4) {
            if (iDraw == -1) {
                SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0123), 0x0, 6, yTop, rc.right - 12, (int32_t)(3 * dyArial8) / 2, SWP_NOZORDER);
            }
            yTop = yTop + (int32_t)(5 * dyArial8) / 2;
        }
        if (fMacintosh == 0 || ids != idsOneResourceGeneratedEachYearEvery) {
            idsT = ids;
        } else {
            idsT = 260;
        }
        ids = ids + 1;
        cch = CchGetString(idsT, szWork);
        if (iDraw == -1) {
            TextOut(hdc, 6, yTop, szWork, cch);
        }
        dx = LOWORD(GetTextExtent(hdc, szWork, cch)) + 6;
        dxItem = abs((int16_t)rgRW3Width[i]) * dxDig;
        _wsprintf(szWork, PCTD, GetRaceStat(&vplr, (int16_t)rgRW3IStat[i]));
        if ((int16_t)rgRW3Width[i] < 0 && (i > 0 || fMacintosh == 0)) {
            dxItem = dxItem + dxkT;
            if (i != 0) {
                strcat(szWork, "kT");
            } else {
                strcat(szWork, "00");
            }
        }
        dx = dx + dxItem;
        if (iDraw == -1 || iDraw == i) {
            RightTextOut(hdc, dx, yTop, szWork, 0, dxItem);
        }
        vrgrcRCW[irc].left = dx + 4;
        vrgrcRCW[irc].top = yTop - 3;
        vrgrcRCW[irc].right = vrgrcRCW[irc].left + 15;
        vrgrcRCW[irc].bottom = (dyArial8 >> 0x1) + vrgrcRCW[irc].top + 0x3;
        vrgrcRCW[irc + 1] = vrgrcRCW[irc];
        OffsetRect((RECT *)&vrgrcRCW[irc + 1].left, 0, vrgrcRCW[irc].bottom - vrgrcRCW[irc].top - 1);
        if (iDraw == -1) {
            DrawBtn(hdc, vrgrcRCW + irc, 0xa0 | bt, 0, 0x0);
            DrawBtn(hdc, vrgrcRCW + (irc + 1), 0xa1 | bt, 0, 0x0);
        }
        if (iDraw == -1) {
            if (fMacintosh == 0 || ids != idsColonists) {
                idsT = ids;
            } else {
                idsT = 261;
            }
            cch = CchGetString(idsT, szWork);
            TextOut(hdc, vrgrcRCW[irc].right + 4, yTop, szWork, cch);
        }
        ids = ids + 1;
        irc = irc + 2;
        yTop = yTop + (int32_t)((int16_t)rgRW3Spacing[i] * dyArial8) / 2;
    }
    if (fMacintosh == 0) {
        crcRCW = irc;
    } else {
        crcRCW = 2;
    }
    SetBkColor(hdc, crBkSav);
    SetBkMode(hdc, bkMode);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

int16_t FTrackRaceDlg3(HWND hwnd, POINT16 pt, int16_t kbd) {
    BTNT    btnt;
    int16_t bt;
    int16_t dShift;
    int16_t i;
    int16_t irc;
    int16_t iMod;
    int16_t iStat;

    irc = IrcRaceDlgHitTest(pt);
    if (irc >= 0) {
        iMod = irc & 0x1;
        i = irc >> 0x1;
        if (iMod != 0) {
            dShift = -1;
            bt = 161;
        } else {
            dShift = 1;
            bt = 160;
        }
        InitBtnTrack(&btnt, hwnd, 0x0, vrgrcRCW + irc, bt, 80, 0, 0, 0x0);
        if ((kbd & 0x4) != 0x0) {
            dShift = 3 * dShift;
        }
        while (FTrackBtn(&btnt) != 0) {
            iStat = GetRaceStat(&vplr, (int16_t)rgRW3IStat[i]);
            if (SetRaceStat(&vplr, (int16_t)rgRW3IStat[i], iStat + dShift) != iStat) {
                DrawRace3(hwnd, btnt.hdc, i);
            }
        }
        return 1;
    }
    return 0;
}

int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat) { return (int16_t)pplr->rgAttr[iStat]; }

int16_t SetRaceStat(PLAYER *pplr, RaceStat iStat, int16_t iVal) {
    if (iVal < (int16_t)rgRaceStatMin[iStat]) {
        iVal = (int16_t)rgRaceStatMin[iStat];
    }
    if (iVal > (int16_t)rgRaceStatMax[iStat]) {
        iVal = (int16_t)rgRaceStatMax[iStat];
    }
    pplr->rgAttr[iStat] = LOBYTE(iVal);
    return iVal;
}

int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit) {
    if (((int32_t)(0x1 << ibit) & pplr->grbitAttr) != 0x0) {
        return 1;
    }
    return 0;
}

void SetRaceGrbit(PLAYER *pplr, RaceGrbit ibit, int16_t fSet) {
    uint32_t grMask;

    grMask = (int32_t)(0x1 << ibit);
    if (fSet == 0) {
        pplr->grbitAttr = pplr->grbitAttr & ~grMask;
    } else {
        pplr->grbitAttr = pplr->grbitAttr | grMask;
    }
    return;
}

INT_PTR CALLBACK RaceWizardDlg4(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    char        szT[600];
    StringId    ids;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;
    HWND        t_scratch_me;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawRaceAdvantagePoints(hdc, &rc, 0x0);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0118), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8 + 2, dyArial8 >> 0x1);
        rcGBox.top = rcGBox.top - 4;
        _Draw3dFrame(hdc, &rcGBox, -1);
        cch = CchGetString(idsPrimaryRacialTrait, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, cch);
        GetClientRect(hwnd, &rc);
        rc.top = rcGBox.bottom + 12;
        rc.left = rc.left + 12;
        rc.right = rc.right - 12;
        GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        rc.bottom = rcGBox.top - 6;
        _Draw3dFrame(hdc, &rc, -1);
        cch = CchGetString(idsDescriptionTrait, szWork);
        TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 0x1), szWork, cch);
        ids = GetRaceStat(&vplr, rsMajorAdv) * 0x3 + 0x114;
        cch = 0;
        i = 0;
        while (i < 3) {
            cch = cch + CchGetString(ids, &szT[cch]);
            i = i + 1;
            ids = ids + 1;
        }
        ExpandRc(&rc, -dyArial8 - 2, -(dyArial8 >> 0x1));
        rc.top = rc.top + 4;
        DrawText(hdc, szT, cch, &rc, 0x10);
        rcCargo = rc;
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 271; i <= 280; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 280 || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            SetRCWTitle(hwnd, iPanelActive);
            CheckRadioButton(hwnd, 271, 280, GetRaceStat(&vplr, rsMajorAdv) + 271);
            if (fRCWReadOnly != 0) {
                for (i = 271; i <= 280; i++) {
                    EnableWindow(GetDlgItem(hwnd, i), 0);
                }
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            return 1;
        }
        if (message == WM_COMMAND) {
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 0x1, 0x408);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_RADRACE1 &&
                GET_WM_COMMAND_ID(wParam, lParam) <= IDC_U16_0x0118) {
                i = GET_WM_COMMAND_ID(wParam, lParam) - 271;
                SetRaceStat(&vplr, rsMajorAdv, i);
                if (GetRaceStat(&vplr, rsMajorAdv) == raMacintosh) {
                    SetRaceStat(&vplr, rsFactProd, 10);
                    SetRaceStat(&vplr, rsFactBuild, 10);
                    SetRaceStat(&vplr, rsFactOperate, 10);
                    SetRaceStat(&vplr, rsMineProd, 10);
                    SetRaceStat(&vplr, rsMineBuild, 5);
                    SetRaceStat(&vplr, rsMineOperate, 10);
                    SetRaceGrbit(&vplr, ibitRaceCheapFact, 0);
                }
                InvalidateAdvPtsRect(hwnd);
                InvalidateRect(hwnd, &rcCargo, 0);
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK RaceWizardDlg5(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HWND        hwndCtl;
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;
    HWND        t_scratch_me;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawRaceAdvantagePoints(hdc, &rc, 0x0);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0130), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox.right);
        GetClientRect(hwnd, &rc);
        rc.top = rcGBox.bottom + 12;
        rc.left = rc.left + 12;
        rc.right = rc.right - 12;
        GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        rc.bottom = rcGBox.top - 12;
        _Draw3dFrame(hdc, &rc, -1);
        rcCargo = rc;
        rcCargo.top = rcCargo.top - (dyArial8 >> 0x1);
        cch = CchGetString(cColDrop + 306, szWork);
        TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 0x1), szWork, cch);
        cch = CchGetString(cColDrop + 320, szWork);
        ExpandRc(&rc, -dyArial8 - 2, -(dyArial8 >> 0x1));
        rc.top = rc.top + 4;
        DrawText(hdc, szWork, cch, &rc, 0x10);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 291; i <= 304; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 304 || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            SetRCWTitle(hwnd, iPanelActive);
            cColDrop = 0;
            for (i = 0; i <= 13; i++) {
                hwndCtl = GetDlgItem(hwnd, i + 291);
                SetWindowText(hwndCtl, PszGetCompressedString(i + 306));
                SendMessage(hwndCtl, BM_SETCHECK, GetRaceGrbit(&vplr, i), 0);
                if (fRCWReadOnly != 0) {
                    EnableWindow(hwndCtl, 0);
                }
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            return 1;
        }
        if (message == WM_COMMAND) {
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 0x1, 0x411);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_ID(wParam, lParam) >= IDC_U16_0x0123 && GET_WM_COMMAND_ID(wParam, lParam) <= IDC_U16_0x0130) {
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0x0, 0));
                cColDrop = GET_WM_COMMAND_ID(wParam, lParam) - 291;
                SetRaceGrbit(&vplr, cColDrop, i);
                InvalidateAdvPtsRect(hwnd);
                InvalidateRect(hwnd, &rcCargo, 0);
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK RaceWizardDlg6(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;
    int16_t     t_scratch_me;
    uint16_t    t_merge_3c78_0001;
    HWND        t_scratch_me_2;
    HWND        t_scratch_me_3;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawRaceAdvantagePoints(hdc, &rc, 0x0);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        for (i = 0; i < 6; i++) {
            GetWindowRect(GetDlgItem(hwnd, 3 * i + 0x10f), &rcGBox);
            ScreenToClient(hwnd, (POINT *)&rcGBox);
            GetWindowRect(GetDlgItem(hwnd, 3 * i + 0x111), &rc);
            ScreenToClient(hwnd, (POINT *)&rc.right);
            rcGBox.right = rc.right;
            rcGBox.bottom = rc.bottom;
            ExpandRc(&rcGBox, dyArial8, dyArial8 >> 0x1);
            _Draw3dFrame(hdc, &rcGBox, -1);
            cch = CchGetString(i + 84, szWork);
            cch = cch + CchGetString(idsResearch, &szWork[cch]);
            TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, cch);
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
        for (i = 271; i <= 288; i++) {
            t_scratch_me_2 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me_2 == GetDlgItem(hwnd, i))
                break;
        }
        if (i > 288) {
            t_scratch_me_3 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me_3 != GetDlgItem(hwnd, IDC_U16_0x0123) && HIWORD(lParam) != 0x6) {
                return 0;
            }
        }
        SetBkColor((HDC)wParam, crButtonFace);
        return (INT_PTR)hbrButtonFace;
    }
    if (message == WM_INITDIALOG) {
        SetRCWTitle(hwnd, iPanelActive);
        for (i = 0; i < 6; i++) {
            t_scratch_me = GetRaceStat(&vplr, i + 8);
            CheckRadioButton(hwnd, 3 * i + 271, 3 * i + 273, 3 * i + 271 + t_scratch_me);
        }
        if (fRCWReadOnly != 0) {
            for (i = 271; i <= 288; i++) {
                EnableWindow(GetDlgItem(hwnd, i), 0);
            }
        }
        t_merge_3c78_0001 = GetRaceStat(&vplr, rsMajorAdv) == raNone ? 0x1 : 0x0;
        _wsprintf(szWork, PszGetCompressedString(idsCosts75ExtraResearchFieldsStartTech), t_merge_3c78_0001 + 0x3);
        SetWindowText(GetDlgItem(hwnd, IDC_U16_0x0123), szWork);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), BM_SETCHECK, GetRaceGrbit(&vplr, ibitRaceTech3), 0);
        if (fRCWReadOnly != 0) {
            EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0123), 0);
        }
        StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
        return 1;
    }
    if (message == WM_COMMAND) {
        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
            WinHelp(hwnd, szHelpFile, 0x1, 0x421);
            return 1;
        }
        for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
        }
        if (i < 4) {
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
            EndDialog(hwnd, i);
            return 1;
        }
        if (GET_WM_COMMAND_CMD(wParam, lParam) != 0x0 || GET_WM_COMMAND_ID(wParam, lParam) < IDC_RADRACE1 || GET_WM_COMMAND_ID(wParam, lParam) > 0x120) {
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x0123) {
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0x0, 0));
                SetRaceGrbit(&vplr, ibitRaceTech3, i);
                InvalidateAdvPtsRect(hwnd);
            }
        } else {
            i = GET_WM_COMMAND_ID(wParam, lParam) - 271;
            SetRaceStat(&vplr, (int32_t)i / 3 + 0x8, (int32_t)i % 3);
            InvalidateAdvPtsRect(hwnd);
        }
    }
    return 0;
}

void BoundsCheckPlayer(PLAYER *pplr) {
    int16_t i;

    for (i = 0; i < 3; i++) {
        if ((int16_t)pplr->rgEnvVarMin[i] != -1) {
            if ((int16_t)pplr->rgEnvVarMin[i] < 0) {
                pplr->rgEnvVarMin[i] = 0;
                pplr->fHacker = 0x1;
            }
            if ((int16_t)pplr->rgEnvVarMin[i] > 100) {
                pplr->rgEnvVarMin[i] = 100;
                pplr->fHacker = 0x1;
            }
            if ((int16_t)pplr->rgEnvVarMax[i] > 100) {
                pplr->rgEnvVarMax[i] = 100;
                pplr->fHacker = 0x1;
            }
            if ((int16_t)pplr->rgEnvVarMax[i] < (int16_t)pplr->rgEnvVarMin[i]) {
                pplr->rgEnvVarMax[i] = pplr->rgEnvVarMin[i];
                pplr->fHacker = 0x1;
            }
            if ((int16_t)pplr->rgEnvVar[i] != (int16_t)pplr->rgEnvVarMin[i] + (int32_t)((int16_t)pplr->rgEnvVarMax[i] - (int16_t)pplr->rgEnvVarMin[i]) / 0x2) {
                pplr->rgEnvVar[i] = LOBYTE((int16_t)pplr->rgEnvVarMin[i] + (int32_t)((int16_t)pplr->rgEnvVarMax[i] - (int16_t)pplr->rgEnvVarMin[i]) / 0x2);
                pplr->fHacker = 0x1;
            }
        } else if ((int16_t)pplr->rgEnvVarMax[i] != -1 || (int16_t)pplr->rgEnvVar[i] != -1) {
            pplr->rgEnvVar[i] = -1;
            pplr->rgEnvVarMax[i] = -1;
            pplr->fHacker = 0x1;
        }
    }
    if ((int16_t)pplr->pctIdealGrowth > 20) {
        pplr->pctIdealGrowth = 20;
        pplr->fHacker = 0x1;
    }
    for (i = 0; i < 16; i++) {
        if ((int16_t)pplr->rgAttr[i] < (int16_t)rgRaceStatMin[i]) {
            pplr->rgAttr[i] = rgRaceStatMin[i];
            pplr->fHacker = 0x1;
        }
        if ((int16_t)pplr->rgAttr[i] > (int16_t)rgRaceStatMax[i]) {
            pplr->rgAttr[i] = rgRaceStatMax[i];
            pplr->fHacker = 0x1;
        }
    }
    return;
}

int16_t CAdvantagePoints(PLAYER *pplr) {
    int16_t pctGrowth;
    int16_t iSpread;
    int32_t cPoints;
    int16_t cBad;
    int16_t cCur;
    int16_t i;
    int16_t rgi[3];
    int16_t cGood;
    int32_t lInnate;
    int16_t raMajor;
    int16_t cOperate;
    int16_t cProduce;
    int16_t t_merge_44f2_0001;

    cPoints = 0;
    cPoints = 1650;
    BoundsCheckPlayer(pplr);
    raMajor = GetRaceStat(pplr, rsMajorAdv);
    lInnate = (int32_t)(LInnateRaceHabitability(pplr) / 2000);
    if (0x1 <= (20 >= (int16_t)pplr->pctIdealGrowth ? (int16_t)pplr->pctIdealGrowth : 0x14)) {
        if (20 >= (int16_t)pplr->pctIdealGrowth) {
            t_merge_44f2_0001 = (int16_t)pplr->pctIdealGrowth;
        } else {
            t_merge_44f2_0001 = 20;
        }
    } else {
        t_merge_44f2_0001 = 1;
    }
    iSpread = t_merge_44f2_0001;
    if (iSpread != (int16_t)pplr->pctIdealGrowth) {
        iSpread = 1;
        pplr->pctIdealGrowth = 1;
        pplr->fHacker = 0x1;
    }
    pctGrowth = iSpread;
    if (iSpread <= 5) {
        cPoints = cPoints + (uint32_t)((int32_t)(6 - iSpread) * 4200);
    } else if (iSpread <= 13) {
        switch (iSpread) {
        case 6:
            cPoints = cPoints + 3600;
            break;
        case 7:
            cPoints = cPoints + 2250;
            break;
        case 8:
            cPoints = cPoints + 600;
            break;
        case 9:
            cPoints = cPoints + 225;
        default:
        }
        iSpread = (iSpread - 5) * 2 + 5;
    } else if (iSpread >= 20) {
        iSpread = 45;
    } else {
        iSpread = (iSpread - 13) * 3 + 21;
    }
    lInnate = (int32_t)((int32_t)(lInnate * (int32_t)iSpread) / 0x18);
    cPoints = cPoints - lInnate;
    cGood = 0;
    for (i = 0; i < 3; i++) {
        if ((int16_t)pplr->rgEnvVar[i] < 0) {
            cGood = cGood + 1;
        } else {
            cPoints = cPoints + (int32_t)(abs((int16_t)pplr->rgEnvVar[i] - 50) * 4);
        }
    }
    if (cGood > 1) {
        cPoints = cPoints - 150;
    }
    cOperate = GetRaceStat(pplr, rsFactOperate);
    cProduce = GetRaceStat(pplr, rsFactProd);
    if (cOperate > 10 || cProduce > 10) {
        cOperate = 1 <= cOperate - 9 ? cOperate - 9 : 1;
        cProduce = 1 <= cProduce - 9 ? cProduce - 9 : 1;
        cProduce = (raMajor == 0 ? 0x3 : 0x2) * cProduce;
        if (cGood < 2) {
            cPoints = cPoints - (int32_t)((int32_t)((uint32_t)((int32_t)cOperate * (int32_t)cProduce) * (int32_t)pctGrowth) / 0x9);
        } else {
            cPoints = cPoints - (int32_t)((int32_t)((uint32_t)((int32_t)cOperate * (int32_t)cProduce) * (int32_t)pctGrowth) / 0x2);
        }
    }
    i = GetRaceStat(pplr, rsResGen);
    i = i >= 25 ? 25 : i;
    if (i > 7) {
        if (i != 8) {
            if (i != 9) {
                if (i > 10) {
                    cPoints = cPoints + (int32_t)((i - 10) * 120);
                }
            } else {
                cPoints = cPoints - 600;
            }
        } else {
            cPoints = cPoints - 1260;
        }
    } else {
        cPoints = cPoints - 2400;
    }
    if (raMajor == 8) {
        cPoints = cPoints + 210;
    } else {
        rgi[0] = 10 - GetRaceStat(pplr, rsFactProd);
        rgi[1] = 10 - GetRaceStat(pplr, rsFactBuild);
        rgi[2] = 10 - GetRaceStat(pplr, rsFactOperate);
        cCur = 0;
        if (rgi[0] <= 0) {
            cCur = cCur + 121 * rgi[0];
        } else {
            cCur = cCur + 100 * rgi[0];
        }
        if (rgi[1] >= 0) {
            cCur = cCur - rgi[1] * rgi[1] * 0x3c;
        } else {
            cCur = cCur - 55 * rgi[1];
        }
        if (rgi[2] <= 0) {
            cCur = cCur + 35 * rgi[2];
        } else {
            cCur = cCur + 40 * rgi[2];
        }
        if (cCur > 700) {
            cCur = (int32_t)(cCur - 700) / 3 + 700;
        }
        if (rgi[2] <= -7) {
            if (rgi[2] < -11) {
                if (rgi[2] < -14) {
                    cCur = cCur - 360;
                } else {
                    cCur = cCur - ((-12 - rgi[2]) * 45 + 225);
                }
            } else {
                cCur = cCur - (-6 - rgi[2]) * 30;
            }
        }
        if (rgi[0] <= -3) {
            cCur = cCur - (-2 - rgi[0]) * 20 * 0x3;
        }
        cPoints = cPoints + (int32_t)cCur;
        if (GetRaceGrbit(pplr, ibitRaceCheapFact) != 0) {
            cPoints = cPoints - 175;
        }
        rgi[0] = 10 - GetRaceStat(pplr, rsMineProd);
        rgi[1] = 3 - GetRaceStat(pplr, rsMineBuild);
        rgi[2] = 10 - GetRaceStat(pplr, rsMineOperate);
        cCur = 0;
        if (rgi[0] <= 0) {
            cCur = cCur + 169 * rgi[0];
        } else {
            cCur = cCur + 100 * rgi[0];
        }
        if (rgi[1] > 0) {
            cCur = cCur - 360;
        } else {
            cCur = cCur - (65 * rgi[1] - 80);
        }
        if (rgi[2] <= 0) {
            cCur = cCur + 35 * rgi[2];
        } else {
            cCur = cCur + 40 * rgi[2];
        }
        cPoints = cPoints + (int32_t)cCur;
    }
    cPoints = cPoints - (int32_t)rgRacePrimaryTrait[raMajor];
    cBad = 0;
    cGood = 0;
    for (i = 0; i <= 13; i++) {
        if (GetRaceGrbit(pplr, i) != 0) {
            if (rgRaceAdvDisPts[i] >= 0) {
                cGood = cGood + 1;
            } else {
                cBad = cBad + 1;
            }
            cPoints = cPoints + (int32_t)rgRaceAdvDisPts[i];
        }
    }
    if (cBad + cGood > 4) {
        cPoints = cPoints - (int32_t)((cBad + cGood) * 10 * (cBad + cGood - 4));
    }
    if (cGood - cBad > 3) {
        cPoints = cPoints - (int32_t)((cGood - cBad - 3) * 60);
    }
    if (cBad - cGood > 3) {
        cPoints = cPoints - (int32_t)((cBad - cGood - 3) * 40);
    }
    if (GetRaceGrbit(pplr, ibitRaceNoAdvScanner) != 0) {
        switch (raMajor) {
        case 6:
            cPoints = cPoints - 280;
            break;
        case 1:
            cPoints = cPoints - 200;
            break;
        case 9:
            cPoints = cPoints - 40;
        default:
        }
    }
    cCur = 0;
    for (i = 8; i <= 13; i++) {
        cCur = cCur + (GetRaceStat(pplr, i) - 1);
    }
    if (cCur <= 0) {
        if (cCur < 0) {
            cPoints = cPoints + (int32_t)rgRaceDisEnvPts[-cCur - 1];
            if (-cCur > 0x4 && GetRaceStat(pplr, rsResGen) < 10) {
                cPoints = cPoints - 190;
            }
        }
    } else {
        cPoints = cPoints - (int32_t)(cCur * cCur * 0x82);
        if (cCur != 6) {
            if (cCur == 5) {
                cPoints = cPoints + 520;
            }
        } else {
            cPoints = cPoints + 1430;
        }
    }
    if (GetRaceGrbit(pplr, ibitRaceTech3) != 0) {
        cPoints = cPoints - 180;
    }
    if (raMajor == 8 && GetRaceStat(pplr, rsTechBonus1) == 2) {
        cPoints = cPoints - 100;
    }
    return LOWORD((int32_t)(cPoints / 3));
}

int32_t LInnateRaceHabitability(PLAYER *pplr) {
    int16_t iTry;
    PLANET  pl;
    double  l2;
    int16_t rgSteps[3];
    PLAYER  plrT;
    int16_t rgDelta[3];
    int16_t fTotalTerra;
    int16_t rgInc[3];
    int16_t i;
    int16_t iTerra;
    int16_t j;
    int32_t l1;
    int16_t rgBase[3];
    double  l3;
    int16_t iDelta;
    int32_t pctDesire;
    int16_t k;
    double  lInnate;
    int16_t pctTerra;

    plrT = rgplr[0];
    lInnate = 0.0;
    fTotalTerra = GetRaceGrbit(pplr, ibitRaceTT);
    rgplr[0] = *pplr;
    rgDelta[2] = 0;
    rgDelta[1] = 0;
    rgDelta[0] = 0;
    for (iTerra = 0; iTerra < 3; iTerra++) {
        if (iTerra != 0) {
            if (iTerra != 1) {
                pctTerra = fTotalTerra == 0 ? 15 : 17;
            } else {
                pctTerra = fTotalTerra == 0 ? 5 : 8;
            }
        } else {
            pctTerra = 0;
        }
        for (i = 0; i < 3; i++) {
            if (((int16_t)pplr->rgEnvVar[i] > 100 || (int16_t)pplr->rgEnvVarMin[i] > 100 || (int16_t)pplr->rgEnvVarMax[i] > 100 ||
                 (int16_t)pplr->rgEnvVar[i] < 0 || (int16_t)pplr->rgEnvVarMin[i] < 0 || (int16_t)pplr->rgEnvVarMax[i] < 0) &&
                ((int16_t)pplr->rgEnvVar[i] != -1 || (int16_t)pplr->rgEnvVarMin[i] != -1 || (int16_t)pplr->rgEnvVarMax[i] != -1)) {
                pplr->rgEnvVarMax[i] = -1;
                pplr->rgEnvVarMin[i] = -1;
                pplr->rgEnvVar[i] = -1;
                pplr->fHacker = 0x1;
                rgplr[0] = *pplr;
            }
            if ((int16_t)pplr->rgEnvVar[i] >= 0) {
                rgBase[i] = (int16_t)pplr->rgEnvVarMin[i] - pctTerra;
                if (rgBase[i] < 0) {
                    rgBase[i] = 0;
                }
                iTry = (int16_t)pplr->rgEnvVarMax[i] + pctTerra;
                if (iTry > 100) {
                    iTry = 100;
                }
                rgInc[i] = iTry - rgBase[i];
                rgSteps[i] = 11;
            } else {
                rgBase[i] = 50;
                rgInc[i] = 11;
                rgSteps[i] = 1;
            }
        }
        l3 = 0.0;
        for (i = 0; i < rgSteps[0]; i++) {
            if (i != 0 && rgSteps[0] > 1) {
                iTry = (int32_t)(i * rgInc[0]) / (rgSteps[0] - 1) + rgBase[0];
            } else {
                iTry = rgBase[0];
            }
            if (iTerra != 0 && (int16_t)pplr->rgEnvVar[0] >= 0) {
                iDelta = (int16_t)pplr->rgEnvVar[0] - iTry;
                if (abs(iDelta) > pctTerra) {
                    if (iDelta >= 0) {
                        iDelta = iDelta - pctTerra;
                    } else {
                        iDelta = iDelta + pctTerra;
                    }
                } else {
                    iDelta = 0;
                }
                rgDelta[0] = iDelta;
                iTry = (int16_t)pplr->rgEnvVar[0] - iDelta;
            }
            pl.rgEnvVar[0] = LOBYTE(iTry);
            l2 = 0.0;
            for (j = 0; j < rgSteps[1]; j++) {
                if (j != 0 && rgSteps[1] > 1) {
                    iTry = (int32_t)(j * rgInc[1]) / (rgSteps[1] - 1) + rgBase[1];
                } else {
                    iTry = rgBase[1];
                }
                if (iTerra != 0 && (int16_t)pplr->rgEnvVar[1] >= 0) {
                    iDelta = (int16_t)pplr->rgEnvVar[1] - iTry;
                    if (abs(iDelta) > pctTerra) {
                        if (iDelta >= 0) {
                            iDelta = iDelta - pctTerra;
                        } else {
                            iDelta = iDelta + pctTerra;
                        }
                    } else {
                        iDelta = 0;
                    }
                    rgDelta[1] = iDelta;
                    iTry = (int16_t)pplr->rgEnvVar[1] - iDelta;
                }
                pl.rgEnvVar[1] = LOBYTE(iTry);
                l1 = 0;
                for (k = 0; k < rgSteps[2]; k++) {
                    if (k != 0 && rgSteps[2] > 1) {
                        iTry = (int32_t)(k * rgInc[2]) / (rgSteps[2] - 1) + rgBase[2];
                    } else {
                        iTry = rgBase[2];
                    }
                    if (iTerra != 0 && (int16_t)pplr->rgEnvVar[2] >= 0) {
                        iDelta = (int16_t)pplr->rgEnvVar[2] - iTry;
                        if (abs(iDelta) > pctTerra) {
                            if (iDelta >= 0) {
                                iDelta = iDelta - pctTerra;
                            } else {
                                iDelta = iDelta + pctTerra;
                            }
                        } else {
                            iDelta = 0;
                        }
                        rgDelta[2] = iDelta;
                        iTry = (int16_t)pplr->rgEnvVar[2] - iDelta;
                    }
                    pl.rgEnvVar[2] = LOBYTE(iTry);
                    pctDesire = (int32_t)PctPlanetDesirability(&pl, 0);
                    iDelta = rgDelta[0] + rgDelta[1] + rgDelta[2];
                    if (iDelta > pctTerra) {
                        pctDesire = pctDesire - (int32_t)(iDelta - pctTerra);
                        if (pctDesire < 0) {
                            pctDesire = 0;
                        }
                    }
                    pctDesire = (uint32_t)(pctDesire * pctDesire);
                    if (iTerra != 0) {
                        if (iTerra != 1) {
                            pctDesire = (uint32_t)(pctDesire * 6);
                        } else {
                            pctDesire = (uint32_t)(pctDesire * 5);
                        }
                    } else {
                        pctDesire = (uint32_t)(pctDesire * 7);
                    }
                    l1 = l1 + pctDesire;
                }
                if ((int16_t)pplr->rgEnvVar[2] < 0) {
                    l1 = (uint32_t)(l1 * 11);
                } else {
                    l1 = (int32_t)((int32_t)(l1 * (int32_t)rgInc[2]) / 0x64);
                }
                l2 = (double)l1 + l2;
            }
            if ((int16_t)pplr->rgEnvVar[1] < 0) {
                l2 = l2 * 11.0;
            } else {
                l2 = l2 * (double)(int32_t)rgInc[1] / 100.0;
            }
            l3 = l3 + l2;
        }
        if ((int16_t)pplr->rgEnvVar[0] < 0) {
            l3 = l3 * 11.0;
        } else {
            l3 = l3 * (double)(int32_t)rgInc[0] / 100.0;
        }
        lInnate = lInnate + l3;
    }
    if (pplr != rgplr) {
        rgplr[0] = plrT;
    }
    return (int32_t)(lInnate / 10.0 + 0.5);
}

void InvalidateAdvPtsRect(HWND hwnd) {
    HDC        hdc;
    TEXTMETRIC tm;
    LOGFONT   *plf;
    int16_t    dyBig;
    HFONT      hfont;
    int16_t    dx;
    RECT       rc;
    HFONT      hfontSav;

    plf = LocalAlloc(0x40, sizeof(LOGFONT));
    hdc = GetDC(hwnd);
    plf->lfHeight = -24;
    strcpy(plf->lfFaceName, rgszArial[1]);
    hfont = CreateFontIndirect(plf);
    hfontSav = SelectObject(hdc, hfont);
    GetTextMetrics(hdc, &tm);
    dyBig = tm.tmHeight + tm.tmExternalLeading;
    dx = LOWORD(GetTextExtent(hdc, "-99999", 6)) + 8;
    GetClientRect(hwnd, &rc);
    rc.left = rc.right - dx;
    rc.bottom = rc.top + dyBig + 4;
    SelectObject(hdc, hfontSav);
    DeleteObject(hfont);
    LocalFree(plf);
    ReleaseDC(hwnd, hdc);
    InvalidateRect(hwnd, &rc, 1);
    return;
}

void DrawRaceAdvantagePoints(HDC hdc, RECT *prc, PLAYER *pplr) {
    TEXTMETRIC tm;
    LOGFONT   *plf;
    COLORREF   crBkSav;
    int16_t    bkMode;
    int16_t    dyBig;
    char       szAdvantage[32];
    int16_t    c;
    COLORREF   crSav;
    HFONT      hfont;
    int16_t    dx;
    int16_t    iPts;
    int16_t    cch;
    RECT       rc;
    HFONT      hfontSav;

    plf = LocalAlloc(0x40, sizeof(LOGFONT));
    plf->lfHeight = -24;
    CchGetString(idsArialBold, plf->lfFaceName);
    hfont = CreateFontIndirect(plf);
    hfontSav = SelectObject(hdc, hfont);
    GetTextMetrics(hdc, &tm);
    dyBig = tm.tmHeight + tm.tmExternalLeading;
    dx = LOWORD(GetTextExtent(hdc, "-99999", 6)) + 8;
    rc = *prc;
    rc.left = rc.right - dx;
    rc.bottom = rc.top + dyBig + 4;
    if (pplr == 0x0) {
        pplr = &vplr;
    }
    iPts = CAdvantagePoints(pplr);
    bkMode = SetBkMode(hdc, OPAQUE);
    crSav = SetTextColor(hdc, iPts < 0 ? 0x7f : 0x0);
    crBkSav = SetBkColor(hdc, crButtonFace);
    c = _wsprintf(szWork, PCTD, iPts);
    RcCtrTextOut(hdc, &rc, szWork, -1);
    SetTextColor(hdc, 0x0);
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsPointsLeft, szWork);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    cch = CchGetString(idsAdvantage, szAdvantage);
    RightTextOut(hdc, rc.left, 3, szAdvantage, cch, 0);
    RightTextOut(hdc, rc.left, dyArial8 + 3, szWork, c, 0);
    rc.left = rc.left - (dx + 4);
    PatBlt(hdc, rc.left - 1, 0, 1, rc.bottom + 1, BLACKNESS);
    PatBlt(hdc, rc.left - 4, 0, 2, rc.bottom + 4, BLACKNESS);
    PatBlt(hdc, rc.left, rc.bottom, rc.right - rc.left, 1, BLACKNESS);
    PatBlt(hdc, rc.left - 2, rc.bottom + 2, rc.right - rc.left + 2, 2, BLACKNESS);
    SetBkColor(hdc, crBkSav);
    SetTextColor(hdc, crSav);
    SetBkMode(hdc, bkMode);
    SelectObject(hdc, hfontSav);
    DeleteObject(hfont);
    LocalFree(plf);
    return;
}

uint16_t IRaceChecksum(PLAYER *pplr) {
    uint16_t  ick;
    uint16_t *p;
    int16_t   i;
    int16_t   cs;

    p = (uint16_t *)pplr;
    cs = 96;
    ick = 0x0;
    for (i = 0; i < cs; i++) {
        ick = ick ^ p[i];
    }
    return ick;
}

int16_t FSaveRace(char *szFileSuggest, PLAYER *pplr) {
    uint16_t     icksum;
    char         szFileTitle[256];
    char         szDirName[256];
    char         szFilter[256];
    uint16_t     i;
    char         szFile[256];
    OPENFILENAME ofn;

    if (szFileSuggest == 0x0) {
        szFile[0] = 0;
    } else {
        strcpy(szFile, szFileSuggest);
    }
    szDirName[0] = 0;
    CchGetString(idsStarsRaceFilesR, szFilter);
    for (i = 0x0; (int16_t)szFilter[i] != 0; i++) {
        if ((int16_t)szFilter[i] == '|') {
            szFilter[i] = 0;
        }
    }
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndRaceParent;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 0x1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 0x100;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 0x100;
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt = "r1";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN;
    if (GetSaveFileName(&ofn) == 0) {
        return 0;
    }
    if (FCreateFile(0x5, -1, szFile) != 0) {
        WriteRtPlr(pplr, 0x0);
        icksum = IRaceChecksum(pplr);
        WriteRt(rtEOF, 2, &icksum);
        StreamClose();
        strcpy(szRaceFile, &szFile[ofn.nFileOffset]);
        return 1;
    }
    AlertSz(PszFormatIds(idsStarsUnableSaveRaceDataFilePlease, 0x0), MB_ICONHAND);
    return 0;
}

void SetRCWTitle(HWND hwnd, int16_t iStep) {
    char    szBuf[50];
    int16_t cch;

    cch = CchGetString(fRCWReadOnly + 270, szBuf);
    cch = _wsprintf(szWork, szBuf, iStep);
    SetWindowText(hwnd, szWork);
    return;
}

void CreateRandomRace(PLAYER *pplr) {
    int16_t cPts;
    int16_t i;
    int16_t cPass;
    int16_t j;
    int16_t iVal;
    int16_t dAwayNew;
    int16_t dAwayCur;
    int16_t k;
    PLAYER  plrT;
    int16_t t_call_5e7d;
    int16_t t_merge_5e8b_0001;
    int16_t t_call_5ee7;
    int16_t t_merge_5ef5_0001;
    int16_t t_scratch_m16;
    int16_t t_call_6075;
    int16_t t_6091;

    pplr->szNames[0] = 0;
    iVal = Random(25);
    if (iVal >= 4) {
        if (iVal >= 7) {
            if (iVal >= 9) {
                for (i = 0; i < 3; i++) {
                    j = Random(40) * 2 + 20;
                    k = Random(100 - j + 1);
                    pplr->rgEnvVar[i] = LOBYTE((int32_t)j / 2 + k);
                    pplr->rgEnvVarMin[i] = LOBYTE(k);
                    pplr->rgEnvVarMax[i] = LOBYTE(k + j);
                }
                if (iVal >= 12) {
                    if (iVal >= 14) {
                        if (iVal < 17) {
                            i = Random(3);
                            j = Random(81);
                            pplr->rgEnvVar[i] = LOBYTE(j + 10);
                            pplr->rgEnvVarMin[i] = LOBYTE(j);
                            pplr->rgEnvVarMax[i] = LOBYTE(j + 20);
                        }
                    } else {
                        i = Random(3);
                        pplr->rgEnvVar[i] = 50;
                        pplr->rgEnvVarMin[i] = 0;
                        pplr->rgEnvVarMax[i] = 100;
                    }
                } else {
                    i = Random(3);
                    pplr->rgEnvVarMax[i] = -1;
                    pplr->rgEnvVarMin[i] = -1;
                    pplr->rgEnvVar[i] = -1;
                }
                pplr->pctIdealGrowth = LOBYTE(Random(9) + 7);
            } else {
                for (i = 0; i < 3; i++) {
                    j = Random(2);
                    if (i == 2 && (int16_t)pplr->rgEnvVar[0] == (int16_t)pplr->rgEnvVar[1]) {
                        j = (int16_t)pplr->rgEnvVar[0] == 0 ? 0 : 1;
                    }
                    if (j != 0) {
                        pplr->pctIdealGrowth = LOBYTE(Random(4) + 2);
                    } else {
                        pplr->rgEnvVar[i] = 50;
                        pplr->rgEnvVarMin[i] = 0;
                        pplr->rgEnvVarMax[i] = 100;
                    }
                }
                pplr->pctIdealGrowth = LOBYTE(Random(5) + 2);
            }
        } else {
            for (i = 0; i < 3; i++) {
                pplr->rgEnvVar[i] = 50;
                pplr->rgEnvVarMin[i] = 0;
                pplr->rgEnvVarMax[i] = 100;
            }
            pplr->pctIdealGrowth = LOBYTE(Random(4) + 3);
        }
    } else {
        for (i = 0; i < 3; i++) {
            pplr->rgEnvVarMax[i] = -1;
            pplr->rgEnvVarMin[i] = -1;
            pplr->rgEnvVar[i] = -1;
        }
        pplr->pctIdealGrowth = LOBYTE(Random(4) + 2);
    }
    iVal = Random(3);
    for (i = 8; i <= 13; i++) {
        if (iVal == 0) {
            t_merge_5e8b_0001 = 1;
        } else {
            t_call_5e7d = Random(3);
            t_merge_5e8b_0001 = t_call_5e7d;
        }
        SetRaceStat(pplr, i, t_merge_5e8b_0001);
    }
    SetRaceStat(pplr, rsMajorAdv, Random(10));
    iVal = Random(4);
    for (i = 0; i <= 13; i++) {
        if (iVal == 0) {
            t_merge_5ef5_0001 = 0;
        } else {
            t_call_5ee7 = Random(2);
            t_merge_5ef5_0001 = t_call_5ee7;
        }
        SetRaceGrbit(pplr, i, t_merge_5ef5_0001);
    }
    SetRaceGrbit(pplr, ibitRaceTech3, Random(2));
    SetRaceGrbit(pplr, ibitRaceCheapFact, Random(2));
    iVal = Random(3);
    if (iVal != 0) {
        for (i = 0; i <= 7; i++) {
            t_scratch_m16 = Random((int16_t)rgRaceStatMax[i] + 1 - (int16_t)rgRaceStatMin[i]);
            pplr->rgAttr[i] = LOBYTE((int16_t)rgRaceStatMin[i] + t_scratch_m16);
        }
    } else {
        for (i = 0; i <= 6; i++) {
            pplr->rgAttr[i] = vrgplrDef[0].rgAttr[i];
        }
        pplr->rgAttr[7] = LOBYTE(Random(5));
    }
    if (strcmp(pplr->szName, PszGetCompressedString(idsRandom2)) == 0) {
        CchGetString(Random(24) + 1390, pplr->szName);
    }
    cPts = CAdvantagePoints(pplr);
    if (cPts < 0 || cPts > 50) {
        cPass = 0;
        while (1) {
            t_call_6075 = CAdvantagePoints(pplr);
            cPts = t_call_6075;
            if (t_call_6075 >= 0 && cPts <= 50) {
                return;
            }
            t_6091 = cPass;
            cPass = cPass + 1;
            if (t_6091 > 250)
                break;
            iVal = Random(10);
            dAwayCur = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
            if (iVal >= 3) {
                if (iVal >= 6) {
                    if (iVal >= 9) {
                        if (Random(2) == 0) {
                            iVal = Random(3);
                            if ((int16_t)pplr->rgEnvVar[iVal] >= 0) {
                                plrT = *pplr;
                                pplr->rgEnvVarMax[iVal] = -1;
                                pplr->rgEnvVarMin[iVal] = -1;
                                pplr->rgEnvVar[iVal] = -1;
                                cPts = CAdvantagePoints(pplr);
                                dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                                if (dAwayNew >= dAwayCur) {
                                    *pplr = plrT;
                                }
                            } else {
                                j = Random(31);
                                pplr->rgEnvVar[iVal] = LOBYTE(j + 35);
                                pplr->rgEnvVarMin[iVal] = LOBYTE(j);
                                pplr->rgEnvVarMax[iVal] = LOBYTE(j + 70);
                                cPts = CAdvantagePoints(pplr);
                                dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                                if (dAwayNew >= dAwayCur) {
                                    pplr->rgEnvVarMax[iVal] = -1;
                                    pplr->rgEnvVarMin[iVal] = -1;
                                    pplr->rgEnvVar[iVal] = -1;
                                }
                            }
                        } else {
                            j = (int16_t)pplr->pctIdealGrowth;
                            if (j > 1) {
                                pplr->pctIdealGrowth = LOBYTE(j - 1);
                                cPts = CAdvantagePoints(pplr);
                                dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                                if (dAwayNew < dAwayCur)
                                    continue;
                            }
                            if (j < 15) {
                                pplr->pctIdealGrowth = LOBYTE(j + 1);
                                cPts = CAdvantagePoints(pplr);
                                dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                                if (dAwayNew < dAwayCur)
                                    continue;
                            }
                            pplr->pctIdealGrowth = LOBYTE(j);
                        }
                    } else {
                        iVal = Random(7);
                        j = GetRaceStat(pplr, iVal);
                        for (i = -1; i <= 1; i = i + 2) {
                            SetRaceStat(pplr, iVal, j + i);
                            cPts = CAdvantagePoints(pplr);
                            dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                            if (dAwayNew < dAwayCur)
                                break;
                        }
                        if (i > 1) {
                            SetRaceStat(pplr, iVal, j);
                        }
                    }
                } else {
                    iVal = Random(14);
                    j = GetRaceGrbit(pplr, iVal);
                    for (i = 0; i < 2; i++) {
                        SetRaceGrbit(pplr, iVal, i);
                        cPts = CAdvantagePoints(pplr);
                        dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                        if (dAwayNew < dAwayCur)
                            break;
                    }
                    if (i >= 2) {
                        SetRaceGrbit(pplr, iVal, j);
                    }
                }
            } else {
                i = Random(6);
                j = (int16_t)pplr->rgAttr[i + 8];
                if (j > 0) {
                    pplr->rgAttr[i + 8] = pplr->rgAttr[i + 8] - 1;
                    cPts = CAdvantagePoints(pplr);
                    dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                    if (dAwayNew < dAwayCur)
                        continue;
                    pplr->rgAttr[i + 8] = LOBYTE(j);
                }
                if (j < 2) {
                    pplr->rgAttr[i + 8] = pplr->rgAttr[i + 8] + 1;
                    cPts = CAdvantagePoints(pplr);
                    dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                    if (dAwayNew >= dAwayCur) {
                        pplr->rgAttr[i + 8] = LOBYTE(j);
                    }
                }
            }
        }
        plrT = *pplr;
        *pplr = vrgplrDef[0];
        strcpy(pplr->szName, plrT.szName);
    }
    return;
}

int16_t PctTrueMaxGrowth(int16_t iplr) {
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raCheapCol) {
        return (int16_t)rgplr[iplr].pctIdealGrowth;
    }
    return (int16_t)rgplr[iplr].pctIdealGrowth * 2;
}
