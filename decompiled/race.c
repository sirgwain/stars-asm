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
                if (FCheckPassword() == 0 || FSaveRace((int16_t)(int8_t)szRaceFile[0] == 0 ? "stars.r1" : szRaceFile, &vplr) == 0)
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
        for (j = 271; j <= 278 && IsDlgButtonChecked(hwnd, j) == 0; j++) {
        }
        if (j <= 277) {
            k = j - 271;
            pplr = &vrgplrDef[k];
        } else {
            pplr = &vplr;
        }
        GetClientRect(hwnd, &rc);
        DrawRaceAdvantagePoints(hdc, &rc, pplr);
        GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0116), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        cch = CchGetString(idsPredefinedRaces, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
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
        DibBlt(hdc, pt.x, pt.y, 32, 32, hdibRaces, (iOffset & 7) * 0x20, (3 - (iOffset >> 3)) * 0x20, 32, 32, 13369376);
        if (fRCWReadOnly == 0) {
            SetRect(rgrcBuildSpin, pt.x + 2, pt.y + 35, pt.x + 16, pt.y + 49);
            rgrcBuildSpin[1] = rgrcBuildSpin[0];
            OffsetRect(&rgrcBuildSpin[1], 14, 0);
            for (i = 0; i < 2; i++) {
                DrawBtn(hdc, &rgrcBuildSpin[i], (i == 0 ? 2 : 3) | 0x20, 0, NULL);
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
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            iPlrBmp = vplr.iPlrBmp;
            SetRCWTitle(hwnd, iPanelActive);
            SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
            SetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames);
            if ((int16_t)(int8_t)vplr.szName[0] == 0) {
                GetDlgItemText(hwnd, IDC_RADRACE1, vplr.szName, 16);
                SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
            }
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 1);
            if (game.fTutorial != 0 && idPlayer == 0 && fRCWReadOnly != 0) {
                i = 0;
            } else {
                for (i = 0; i < 7; i++) {
                    vplr.iPlrBmp = vrgplrDef[i].iPlrBmp;
                    if (fmemcmp(&vplr, &vrgplrDef[i], 128) == 0)
                        break;
                }
                vplr.iPlrBmp = iPlrBmp;
            }
            CheckRadioButton(hwnd, 271, 278, i + 271);
            SendDlgItemMessage(hwnd, 268, EM_LIMITTEXT, 0xf, 0);
            SendDlgItemMessage(hwnd, 2075, EM_LIMITTEXT, 0xf, 0);
            SendDlgItemMessage(hwnd, 269, EM_LIMITTEXT, 0x10, 0);
            hwndCB = GetDlgItem(hwnd, IDC_COMBOBOX);
            for (i = 262; i <= 266; i++) {
                psz = PszGetCompressedString(i);
                SendMessage(hwndCB, CB_ADDSTRING, 0, (LPARAM)psz);
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
            if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) != 0) {
                iDir = -1;
                bt = 34;
                prc = rgrcBuildSpin;
            } else {
                iDir = 1;
                bt = 35;
                prc = &rgrcBuildSpin[1];
            }
            iCur = vplr.iPlrBmp;
            if (iCur >= 32) {
                iCur = 0;
            }
            hdc = GetDC(hwnd);
            SelectPalette(hdc, vhpal, 0);
            RealizePalette(hdc);
            InitBtnTrack(&btnt, hwnd, NULL, prc, bt, 80, 0, 0, NULL);
            while (FTrackBtn(&btnt) != 0) {
                iCur = (int16_t)(iCur + 32 + iDir) % 32;
                DibBlt(hdc, rgrcBuildSpin[0].left - 2, rgrcBuildSpin[0].top - 35, 32, 32, hdibRaces, (iCur & 7) * 0x20, (3 - (iCur >> 3)) * 0x20, 32, 32,
                       13369376);
            }
            vplr.iPlrBmp = iCur;
            ReleaseDC(hwnd, hdc);
            return 1;
        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HELP) {
                WinHelp(hwnd, szHelpFile, 1, 0x3ff);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL) {
                    iPlrBmp = vplr.iPlrBmp;
                    for (j = 271; j <= 278 && IsDlgButtonChecked(hwnd, j) == 0; j++) {
                    }
                    if (j <= 277) {
                        k = j - 271;
                        vplr = vrgplrDef[k];
                    }
                    GetDlgItemText(hwnd, IDC_EDIT1, vplr.szName, 32);
                    GetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames, 32);
                    GetRaceStat(&vplr, rsUseLeftover);
                    GetDlgItemText(hwnd, IDC_U16_0x010D, szRacePass, 16);
                    j = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_GETCURSEL, 0, 0));
                    SetRaceStat(&vplr, rsUseLeftover, j);
                    vplr.lSalt = LSaltFromSz(szRacePass);
                    vplr.iPlrBmp = iPlrBmp;
                }
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_RADRACE1 &&
                GET_WM_COMMAND_ID(wParam, lParam) <= IDC_U16_0x0116) {
                memset(vplr.szName, 0, 32);
                GetDlgItemText(hwnd, IDC_EDIT1, vplr.szName, 32);
                memset(vplr.szNames, 0, 32);
                GetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames, 32);
                GetDlgItemText(hwnd, GET_WM_COMMAND_ID(wParam, lParam), szBuf, 32);
                for (i = 0; i < 7 && strcmp(vplr.szName, PszGetCompressedString(i + 1383)) != 0; i++) {
                }
                if (i < 7 && GET_WM_COMMAND_ID(wParam, lParam) < IDC_U16_0x0116) {
                    memset(vplr.szName, 0, 32);
                    CchGetString(GET_WM_COMMAND_ID(wParam, lParam) + 1112, vplr.szName);
                    SetDlgItemText(hwnd, IDC_EDIT1, vplr.szName);
                    memset(vplr.szNames, 0, 32);
                    psz = PszPlayerName(0, 1, 1, 0, 0, &vplr);
                    strcpy(vplr.szNames, psz);
                    SetDlgItemText(hwnd, IDC_EDITNAME, vplr.szNames);
                }
                if (GET_WM_COMMAND_ID(wParam, lParam) <= 0x115) {
                    pplr = &vrgplrDef[GET_WM_COMMAND_ID(wParam, lParam) - 271];
                } else {
                    pplr = &vplr;
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
        }
    } else {
        for (i = 271; i <= 278; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 278 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
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
    if (IS_WM_CTLCOLOR(message) == 0) {
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
            dy = (int16_t)(3 * dyArial8) / 2;
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
                OffsetRect((RECT *)&vrgrcRCW[i + 0xa].left, 0, 6 * dy);
            }
            for (i = 0; i < 3; i++) {
                SetWindowPos(GetDlgItem(hwnd, i + 291), NULL, 3 * dy + dxLabel + 6, vrgrcRCW[5 * i + 3].top, dxMiddle - 6 * dy - 12, dy, SWP_NOZORDER);
                CheckDlgButton(hwnd, i + 291, vplr.rgEnvVarMax[i] >= 0 ? 0 : 1);
            }
            cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
            t_scratch_m30 = LOWORD(GetTextExtent(hdc, "15%", 3));
            vrgrcRCW[15].left = dxLabel + LOWORD(GetTextExtent(hdc, szWork, cch)) + t_scratch_m30 + 4;
            vrgrcRCW[15].top = 9 * dy + yTop - 3;
            vrgrcRCW[15].right = vrgrcRCW[15].left + 15;
            vrgrcRCW[15].bottom = (dyArial8 >> 1) + vrgrcRCW[15].top + 3;
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
                WinHelp(hwnd, szHelpFile, 1, 1053);
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
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0, 0));
                iVar = GET_WM_COMMAND_ID(wParam, lParam) - 291;
                if (i == 1) {
                    vplr.rgEnvVar[iVar] = -1;
                    vplr.rgEnvVarMax[iVar] = -1;
                    vplr.rgEnvVarMin[iVar] = -1;
                } else {
                    vplr.rgEnvVarMin[iVar] = 20;
                    vplr.rgEnvVarMax[iVar] = 80;
                    vplr.rgEnvVar[iVar] = 50;
                }
                DrawRace2(hwnd, NULL, 1 << iVar | 0xff00);
            }
        }
    } else {
        for (i = 291; i <= 293; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 293 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
}

void DrawRace2(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t iPit;
    int16_t bt;
    int16_t iMax;
    char    szT[32];
    int16_t dy;
    int16_t iMin;
    int16_t bkMode;
    int16_t fCreatedDC;
    int16_t xRLabel;
    int16_t i;
    int16_t iMod;
    char   *psz;
    int16_t dx;
    int16_t cch;
    int16_t bt1;
    RECT    rc;
    int32_t l2;
    int16_t iStore;
    int32_t l;

    fCreatedDC = 0;
    bt1 = fRCWReadOnly == 0 ? 0 : 4;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    DrawRaceAdvantagePoints(hdc, &rc, NULL);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    xRLabel = (int16_t)(rc.right - vrgrcRCW[2].right) / 2 + vrgrcRCW[2].right;
    for (i = 0; i < 3; i++) {
        if ((1 << i & iDraw) != 0) {
            if (iDraw != -1) {
                SelectObject(hdc, hbrButtonFace);
                PatBlt(hdc, vrgrcRCW[2].right, vrgrcRCW[5 * i + 2].top, rc.right - vrgrcRCW[2].right, vrgrcRCW[5 * i + 4].bottom - vrgrcRCW[5 * i + 2].top,
                       PATCOPY);
            }
            if (vplr.rgEnvVarMax[i] < 0) {
                cch = CchGetString(idsN2, szT);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8, szT, cch);
            } else {
                psz = PszCalcEnvVar(i, vplr.rgEnvVarMin[i]);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1, psz, 0);
                cch = CchGetString(idsTo2, szT);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8, szT, cch);
                psz = PszCalcEnvVar(i, vplr.rgEnvVarMax[i]);
                CtrTextOut(hdc, xRLabel, vrgrcRCW[5 * i + 2].top + 1 + dyArial8 * 2, psz, 0);
            }
            if (iDraw == -1) {
                RightTextOut(hdc, vrgrcRCW[5 * i].left - 4, dyArial8 / 4 + vrgrcRCW[5 * i].top, rgszPlanetAttr[i], 0, 0);
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
        bt = vplr.rgEnvVarMax[i / 5] >= 0 ? 0 : 4;
        switch (iMod) {
        case 0:
        case 2:
            if ((iDraw & 0xfff0) == 0)
                break;
            DrawBtn(hdc, vrgrcRCW + i, (iMod == 0 ? 2 : 3) | bt | bt1, 0, NULL);
            break;
        default:
            if ((iDraw & 0xfff0) == 0)
                break;
            DrawBtn(hdc, vrgrcRCW + i, 8 | bt | bt1, 0, iMod == 3 ? "<<     >>" : ">>     <<");
            break;
        case 1:
            if ((1 << iPit & iDraw) != 0) {
                PatBlt(hdc, vrgrcRCW[i].left, vrgrcRCW[i].top, vrgrcRCW[i].right - vrgrcRCW[i].left, vrgrcRCW[i].bottom - vrgrcRCW[i].top, BLACKNESS);
                if (bt == 0) {
                    iMin = vplr.rgEnvVarMin[iPit];
                    iMax = vplr.rgEnvVarMax[iPit];
                    dx = vrgrcRCW[i].right - vrgrcRCW[i].left - 2;
                    dy = vrgrcRCW[i].bottom - vrgrcRCW[i].top - 2;
                    SelectObject(hdc, rghbrPlanetAttr[iPit][0]);
                    PatBlt(hdc, MulDiv(iMin, dx, 100) + vrgrcRCW[i].left + 1, vrgrcRCW[i].top + 1, MulDiv(iMax - iMin, dx, 100), dy, PATCOPY);
                }
            }
            iPit++;
        }
        i++;
        iMod++;
    }
    if ((iDraw & 8) != 0) {
        if (iDraw == -1) {
            cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
            TextOut(hdc, vrgrcRCW->left, vrgrcRCW[15].top + 3, szWork, cch);
            DrawBtn(hdc, vrgrcRCW + 15, 0xa0 | bt1, 0, NULL);
            DrawBtn(hdc, vrgrcRCW + 16, 0xa1 | bt1, 0, NULL);
        } else {
            dx = LOWORD(GetTextExtent(hdc, "15%", 3));
            SelectObject(hdc, hbrButtonFace);
            PatBlt(hdc, vrgrcRCW[15].left - 4 - dx, vrgrcRCW[15].top + 3, dx, dyArial8, PATCOPY);
        }
        cch = _wsprintf(szWork, PCTDPCTPCT, vplr.pctIdealGrowth);
        RightTextOut(hdc, vrgrcRCW[15].left - 4, vrgrcRCW[15].top + 3, szWork, cch, 0);
    }
    if ((iDraw & 7) != 0) {
        l = 1;
        for (i = 0; i < 3; i++) {
            if (vplr.rgEnvVarMax[i] < 0 || vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i] == 100) {
                l = (uint32_t)(l * 100);
            } else if (i == 2) {
                l = (uint32_t)(l * (int16_t)(vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i]));
            } else {
                l2 = 0;
                for (iStore = vplr.rgEnvVarMin[i]; iStore <= vplr.rgEnvVarMax[i]; iStore++) {
                    if (iStore < 10) {
                        l2 += iStore;
                    } else if (iStore < 90) {
                        l2 += 10;
                    } else {
                        l2 += (int16_t)(100 - iStore);
                    }
                }
                l = (int32_t)(l * l2) / 9;
            }
        }
        if (l < 1) {
            l = 1;
        }
        l2 = (int32_t)(((int32_t)(l >> 1) + 0xf4240) / l);
        iStore = LOWORD(l2);
        if (l == 1000000) {
            iStore = 0;
        }
        if (iStore != viStore) {
            viStore = iStore;
            if (l2 == 1) {
                cch = CchGetString(l == 1000000 ? idsPlanetsWillHabitableRace : idsVirtuallyPlanetsWillHabitableRace, szWork);
            } else {
                CchGetString(idsCanExpect1DPlanetsWillHabitable, szT);
                cch = _wsprintf(szWork, szT, l2);
            }
            rc.left = vrgrcRCW->left;
            rc.top = vrgrcRCW[15].top + dyArial8 + 8;
            rc.right = vrgrcRCW[15].right + 20;
            rc.bottom = 3 * dyArial8 + rc.top;
            SelectObject(hdc, hbrButtonFace);
            PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, PATCOPY);
            DrawText(hdc, szWork, cch, &rc, 16);
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

    if (fRCWReadOnly != 0) {
        return -1;
    }
    for (i = 0; i < crcRCW && PtInRect((RECT *)&vrgrcRCW[i].left, PointFrom16(pt)) == 0; i++) {
    }
    if (i < crcRCW) {
        if (iPanelActive == 4 && vplr.rgEnvVarMax[i / 5] < 0) {
            return -1;
        }
        return i;
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
    uint16_t t_merge_2587_0001;

    irc = IrcRaceDlgHitTest(pt);
    if (irc < 0) {
        return 0;
    }
    iMod = irc % 5;
    i = irc / 5;
    if (iMod == 1 && irc < 15) {
        SetCapture(hwnd);
        SetCursor(hcurCloseGrab);
        dWidth = (int16_t)(vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i]) / 2;
        while (FGetMouseMove(&pt) != 0) {
            if (pt.x < vrgrcRCW[irc].left) {
                pt.x = vrgrcRCW[irc].left;
            }
            if (pt.x > vrgrcRCW[irc].right) {
                pt.x = vrgrcRCW[irc].right;
            }
            dShift = MulDiv(pt.x - vrgrcRCW[irc].left, 100, vrgrcRCW[irc].right - vrgrcRCW[irc].left);
            if (dWidth > (dShift >= 100 - dWidth ? 100 - dWidth : dShift)) {
                dShift = dWidth;
            } else if (dShift >= 100 - dWidth) {
                dShift = 100 - dWidth;
            }
            if (vplr.rgEnvVarMax[i] != dShift + dWidth) {
                vplr.rgEnvVarMin[i] = LOBYTE(dShift - dWidth);
                vplr.rgEnvVarMax[i] = LOBYTE(dShift + dWidth);
                vplr.rgEnvVar[i] = LOBYTE(vplr.rgEnvVarMin[i] + (int16_t)(vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i]) / 2);
                DrawRace2(hwnd, NULL, 1 << i);
            }
        }
        ReleaseCapture();
        return 1;
    }
    dWidth = 0;
    dShift = 0;
    psz = 0;
    if (irc == 15) {
        dShift = 1;
        bt = 160;
    } else if (irc == 16) {
        dShift = -1;
        bt = 161;
    } else {
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
    }
    InitBtnTrack(&btnt, hwnd, NULL, vrgrcRCW + irc, bt, 80, 0, 0, psz);
    if ((kbd & 4) != 0) {
        dWidth = 10 * dWidth;
        dShift = 10 * dShift;
    }
    while (FTrackBtn(&btnt) != 0) {
        if (irc == 15 || irc == 16) {
            iMin = LOBYTE(vplr.pctIdealGrowth + dShift);
            t_merge_2587_0001 = 20 < (1 <= iMin ? iMin : 1) ? 20 : 1 > iMin ? 1 : iMin;
            iMin = LOBYTE(t_merge_2587_0001);
            if (iMin != vplr.pctIdealGrowth) {
                vplr.pctIdealGrowth = iMin;
                DrawRace2(hwnd, btnt.hdc, 8);
            }
        } else {
            iMin = LOBYTE(vplr.rgEnvVarMin[i] - (dWidth - dShift));
            iMax = LOBYTE(vplr.rgEnvVarMax[i] + (dWidth + dShift));
            if (iMax > 100) {
                iMin -= LOBYTE(iMax - 100);
                iMax = 100;
            }
            if (iMin < 0) {
                iMax = LOBYTE(100 >= iMax - iMin ? iMax - iMin : 100);
                iMin = 0;
            }
            dx = iMax - iMin;
            if (dx < 20) {
                dx = (0x14 - dx) >> 1;
                iMin -= LOBYTE(dx);
                iMax += LOBYTE(dx);
            }
            if (vplr.rgEnvVarMin[i] != iMin || vplr.rgEnvVarMax[i] != iMax) {
                vplr.rgEnvVarMin[i] = iMin;
                vplr.rgEnvVarMax[i] = iMax;
                vplr.rgEnvVar[i] = LOBYTE(vplr.rgEnvVarMin[i] + (int16_t)(vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i]) / 2);
                DrawRace2(hwnd, btnt.hdc, 1 << i);
            }
        }
    }
    if (irc < 15) {
        vplr.rgEnvVar[i] = LOBYTE(vplr.rgEnvVarMin[i] + (int16_t)(vplr.rgEnvVarMax[i] - vplr.rgEnvVarMin[i]) / 2);
    }
    return 1;
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
    if (IS_WM_CTLCOLOR(message) == 0) {
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
                WinHelp(hwnd, szHelpFile, 1, 1056);
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
                i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), BM_GETCHECK, 0, 0));
                SetRaceGrbit(&vplr, ibitRaceCheapFact, i);
                DrawRace3(hwnd, NULL, 99);
            }
        }
    } else {
        t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
        if (t_scratch_me == GetDlgItem(hwnd, IDC_U16_0x0123) || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
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
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwnd, &rc);
    DrawRaceAdvantagePoints(hdc, &rc, NULL);
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
                SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0123), NULL, 6, yTop, rc.right - 12, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
            }
            yTop += (int16_t)(5 * dyArial8) / 2;
        }
        if (fMacintosh != 0 && ids == idsOneResourceGeneratedEachYearEvery) {
            idsT = 260;
        } else {
            idsT = ids;
        }
        ids++;
        cch = CchGetString(idsT, szWork);
        if (iDraw == -1) {
            TextOut(hdc, 6, yTop, szWork, cch);
        }
        dx = LOWORD(GetTextExtent(hdc, szWork, cch)) + 6;
        dxItem = abs((int16_t)(int8_t)rgRW3Width[i]) * dxDig;
        _wsprintf(szWork, PCTD, GetRaceStat(&vplr, (int16_t)(int8_t)rgRW3IStat[i]));
        if ((int16_t)(int8_t)rgRW3Width[i] < 0 && (i > 0 || fMacintosh == 0)) {
            dxItem += dxkT;
            if (i == 0) {
                strcat(szWork, "00");
            } else {
                strcat(szWork, "kT");
            }
        }
        dx += dxItem;
        if (iDraw == -1 || iDraw == i) {
            RightTextOut(hdc, dx, yTop, szWork, 0, dxItem);
        }
        vrgrcRCW[irc].left = dx + 4;
        vrgrcRCW[irc].top = yTop - 3;
        vrgrcRCW[irc].right = vrgrcRCW[irc].left + 15;
        vrgrcRCW[irc].bottom = (dyArial8 >> 1) + vrgrcRCW[irc].top + 3;
        vrgrcRCW[irc + 1] = vrgrcRCW[irc];
        OffsetRect((RECT *)&vrgrcRCW[irc + 1].left, 0, vrgrcRCW[irc].bottom - vrgrcRCW[irc].top - 1);
        if (iDraw == -1) {
            DrawBtn(hdc, vrgrcRCW + irc, 0xa0 | bt, 0, NULL);
            DrawBtn(hdc, vrgrcRCW + (irc + 1), 0xa1 | bt, 0, NULL);
        }
        if (iDraw == -1) {
            if (fMacintosh != 0 && ids == idsColonists) {
                idsT = 261;
            } else {
                idsT = ids;
            }
            cch = CchGetString(idsT, szWork);
            TextOut(hdc, vrgrcRCW[irc].right + 4, yTop, szWork, cch);
        }
        ids++;
        irc += 2;
        yTop += (int16_t)((int16_t)(int8_t)rgRW3Spacing[i] * dyArial8) / 2;
    }
    if (fMacintosh != 0) {
        crcRCW = 2;
    } else {
        crcRCW = irc;
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
    if (irc < 0) {
        return 0;
    }
    iMod = irc & 1;
    i = irc >> 1;
    if (iMod == 0) {
        dShift = 1;
        bt = 160;
    } else {
        dShift = -1;
        bt = 161;
    }
    InitBtnTrack(&btnt, hwnd, NULL, vrgrcRCW + irc, bt, 80, 0, 0, NULL);
    if ((kbd & 4) != 0) {
        dShift = 3 * dShift;
    }
    while (FTrackBtn(&btnt) != 0) {
        iStat = GetRaceStat(&vplr, (int16_t)(int8_t)rgRW3IStat[i]);
        if (SetRaceStat(&vplr, (int16_t)(int8_t)rgRW3IStat[i], iStat + dShift) != iStat) {
            DrawRace3(hwnd, btnt.hdc, i);
        }
    }
    return 1;
}

int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat) { return pplr->rgAttr[iStat]; }

int16_t SetRaceStat(PLAYER *pplr, RaceStat iStat, int16_t iVal) {
    if (iVal < (int16_t)(int8_t)rgRaceStatMin[iStat]) {
        iVal = (int16_t)(int8_t)rgRaceStatMin[iStat];
    }
    if (iVal > (int16_t)(int8_t)rgRaceStatMax[iStat]) {
        iVal = (int16_t)(int8_t)rgRaceStatMax[iStat];
    }
    pplr->rgAttr[iStat] = LOBYTE(iVal);
    return iVal;
}

int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit) {
    if ((1 << ibit & pplr->grbitAttr) != 0) {
        return 1;
    }
    return 0;
}

void SetRaceGrbit(PLAYER *pplr, RaceGrbit ibit, int16_t fSet) {
    uint32_t grMask;

    grMask = 1 << ibit;
    if (fSet != 0) {
        pplr->grbitAttr |= grMask;
    } else {
        pplr->grbitAttr &= ~grMask;
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
        DrawRaceAdvantagePoints(hdc, &rc, NULL);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0118), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8 + 2, dyArial8 >> 1);
        rcGBox.top -= 4;
        _Draw3dFrame(hdc, &rcGBox, -1);
        cch = CchGetString(idsPrimaryRacialTrait, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
        GetClientRect(hwnd, &rc);
        rc.top = rcGBox.bottom + 12;
        rc.left += 12;
        rc.right -= 12;
        GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        rc.bottom = rcGBox.top - 6;
        _Draw3dFrame(hdc, &rc, -1);
        cch = CchGetString(idsDescriptionTrait, szWork);
        TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, cch);
        ids = GetRaceStat(&vplr, rsMajorAdv) * 3 + 276;
        cch = 0;
        i = 0;
        while (i < 3) {
            cch += CchGetString(ids, &szT[cch]);
            i++;
            ids++;
        }
        ExpandRc(&rc, -dyArial8 - 2, -(dyArial8 >> 1));
        rc.top += 4;
        DrawText(hdc, szT, cch, &rc, 16);
        rcCargo = rc;
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
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
                WinHelp(hwnd, szHelpFile, 1, 1032);
                return 1;
            }
            for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
            }
            if (i < 4) {
                StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
                EndDialog(hwnd, i);
                return 1;
            }
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_RADRACE1 &&
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
    } else {
        for (i = 271; i <= 280; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 280 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
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
        DrawRaceAdvantagePoints(hdc, &rc, NULL);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0130), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox.right);
        GetClientRect(hwnd, &rc);
        rc.top = rcGBox.bottom + 12;
        rc.left += 12;
        rc.right -= 12;
        GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        rc.bottom = rcGBox.top - 12;
        _Draw3dFrame(hdc, &rc, -1);
        rcCargo = rc;
        rcCargo.top -= dyArial8 >> 1;
        cch = CchGetString(cColDrop + 306, szWork);
        TextOut(hdc, rc.left + 8, rc.top - (dyArial8 >> 1), szWork, cch);
        cch = CchGetString(cColDrop + 320, szWork);
        ExpandRc(&rc, -dyArial8 - 2, -(dyArial8 >> 1));
        rc.top += 4;
        DrawText(hdc, szWork, cch, &rc, 16);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
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
                WinHelp(hwnd, szHelpFile, 1, 1041);
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
                i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0, 0));
                cColDrop = GET_WM_COMMAND_ID(wParam, lParam) - 291;
                SetRaceGrbit(&vplr, cColDrop, i);
                InvalidateAdvPtsRect(hwnd);
                InvalidateRect(hwnd, &rcCargo, 0);
            }
        }
    } else {
        for (i = 291; i <= 304; i++) {
            t_scratch_me = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_me == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 304 || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
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
        DrawRaceAdvantagePoints(hdc, &rc, NULL);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        for (i = 0; i < 6; i++) {
            GetWindowRect(GetDlgItem(hwnd, 3 * i + 271), &rcGBox);
            ScreenToClient(hwnd, (POINT *)&rcGBox);
            GetWindowRect(GetDlgItem(hwnd, 3 * i + 273), &rc);
            ScreenToClient(hwnd, (POINT *)&rc.right);
            rcGBox.right = rc.right;
            rcGBox.bottom = rc.bottom;
            ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
            _Draw3dFrame(hdc, &rcGBox, -1);
            cch = CchGetString(i + 84, szWork);
            cch += CchGetString(idsResearch, &szWork[cch]);
            TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
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
            if (t_scratch_me_3 != GetDlgItem(hwnd, IDC_U16_0x0123) && HIWORD(lParam) != 6) {
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
        t_merge_3c78_0001 = GetRaceStat(&vplr, rsMajorAdv) == raNone ? 1 : 0;
        _wsprintf(szWork, PszGetCompressedString(idsCosts75ExtraResearchFieldsStartTech), t_merge_3c78_0001 + 3);
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
            WinHelp(hwnd, szHelpFile, 1, 1057);
            return 1;
        }
        for (i = 0; i < 4 && GET_WM_COMMAND_ID(wParam, lParam) != rgidRaceBtn[i]; i++) {
        }
        if (i < 4) {
            StickyDlgPos(hwnd, &ptStickyRaceDlg, 0);
            EndDialog(hwnd, i);
            return 1;
        }
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_RADRACE1 && GET_WM_COMMAND_ID(wParam, lParam) <= 0x120) {
            i = GET_WM_COMMAND_ID(wParam, lParam) - 271;
            SetRaceStat(&vplr, i / 3 + 8, i % 3);
            InvalidateAdvPtsRect(hwnd);
        } else if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x0123) {
            i = LOWORD(SendMessage(GetDlgItem(hwnd, GET_WM_COMMAND_ID(wParam, lParam)), BM_GETCHECK, 0, 0));
            SetRaceGrbit(&vplr, ibitRaceTech3, i);
            InvalidateAdvPtsRect(hwnd);
        }
    }
    return 0;
}

void BoundsCheckPlayer(PLAYER *pplr) {
    int16_t i;

    for (i = 0; i < 3; i++) {
        if (pplr->rgEnvVarMin[i] == -1) {
            if (pplr->rgEnvVarMax[i] != -1 || pplr->rgEnvVar[i] != -1) {
                pplr->rgEnvVar[i] = -1;
                pplr->rgEnvVarMax[i] = -1;
                pplr->fHacker = 1;
            }
        } else {
            if (pplr->rgEnvVarMin[i] < 0) {
                pplr->rgEnvVarMin[i] = 0;
                pplr->fHacker = 1;
            }
            if (pplr->rgEnvVarMin[i] > 100) {
                pplr->rgEnvVarMin[i] = 100;
                pplr->fHacker = 1;
            }
            if (pplr->rgEnvVarMax[i] > 100) {
                pplr->rgEnvVarMax[i] = 100;
                pplr->fHacker = 1;
            }
            if (pplr->rgEnvVarMax[i] < pplr->rgEnvVarMin[i]) {
                pplr->rgEnvVarMax[i] = pplr->rgEnvVarMin[i];
                pplr->fHacker = 1;
            }
            if (pplr->rgEnvVar[i] != pplr->rgEnvVarMin[i] + (int16_t)(pplr->rgEnvVarMax[i] - pplr->rgEnvVarMin[i]) / 2) {
                pplr->rgEnvVar[i] = LOBYTE(pplr->rgEnvVarMin[i] + (int16_t)(pplr->rgEnvVarMax[i] - pplr->rgEnvVarMin[i]) / 2);
                pplr->fHacker = 1;
            }
        }
    }
    if (pplr->pctIdealGrowth > 20) {
        pplr->pctIdealGrowth = 20;
        pplr->fHacker = 1;
    }
    for (i = 0; i < 16; i++) {
        if (pplr->rgAttr[i] < (int16_t)(int8_t)rgRaceStatMin[i]) {
            pplr->rgAttr[i] = rgRaceStatMin[i];
            pplr->fHacker = 1;
        }
        if (pplr->rgAttr[i] > (int16_t)(int8_t)rgRaceStatMax[i]) {
            pplr->rgAttr[i] = rgRaceStatMax[i];
            pplr->fHacker = 1;
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

    cPoints = 0;
    cPoints = 1650;
    BoundsCheckPlayer(pplr);
    raMajor = GetRaceStat(pplr, rsMajorAdv);
    lInnate = (int32_t)(LInnateRaceHabitability(pplr) / 2000);
    iSpread = 1 > (20 >= pplr->pctIdealGrowth ? pplr->pctIdealGrowth : 20) ? 1 : 20 < pplr->pctIdealGrowth ? 20 : pplr->pctIdealGrowth;
    if (iSpread != pplr->pctIdealGrowth) {
        iSpread = 1;
        pplr->pctIdealGrowth = 1;
        pplr->fHacker = 1;
    }
    pctGrowth = iSpread;
    if (iSpread <= 5) {
        cPoints += (uint32_t)((int16_t)(6 - iSpread) * 4200);
    } else if (iSpread > 13) {
        if (iSpread < 20) {
            iSpread = (iSpread - 13) * 3 + 21;
        } else {
            iSpread = 45;
        }
    } else {
        switch (iSpread) {
        case 6:
            cPoints += 3600;
            break;
        case 7:
            cPoints += 2250;
            break;
        case 8:
            cPoints += 600;
            break;
        case 9:
            cPoints += 225;
        }
        iSpread = (iSpread - 5) * 2 + 5;
    }
    lInnate = (int32_t)(lInnate * iSpread) / 24;
    cPoints -= lInnate;
    cGood = 0;
    for (i = 0; i < 3; i++) {
        if (pplr->rgEnvVar[i] >= 0) {
            cPoints += (int16_t)(abs(pplr->rgEnvVar[i] - 50) * 4);
        } else {
            cGood++;
        }
    }
    if (cGood > 1) {
        cPoints -= 150;
    }
    cOperate = GetRaceStat(pplr, rsFactOperate);
    cProduce = GetRaceStat(pplr, rsFactProd);
    if (cOperate > 10 || cProduce > 10) {
        cOperate = 1 <= cOperate - 9 ? cOperate - 9 : 1;
        cProduce = 1 <= cProduce - 9 ? cProduce - 9 : 1;
        cProduce = (raMajor == 0 ? 3 : 2) * cProduce;
        if (cGood >= 2) {
            cPoints -= (int32_t)((uint32_t)(cOperate * cProduce) * pctGrowth) / 2;
        } else {
            cPoints -= (int32_t)((uint32_t)(cOperate * cProduce) * pctGrowth) / 9;
        }
    }
    i = GetRaceStat(pplr, rsResGen);
    i = i >= 25 ? 25 : i;
    if (i <= 7) {
        cPoints -= 2400;
    } else if (i == 8) {
        cPoints -= 1260;
    } else if (i == 9) {
        cPoints -= 600;
    } else if (i > 10) {
        cPoints += (int16_t)((i - 10) * 120);
    }
    if (raMajor != 8) {
        rgi[0] = 10 - GetRaceStat(pplr, rsFactProd);
        rgi[1] = 10 - GetRaceStat(pplr, rsFactBuild);
        rgi[2] = 10 - GetRaceStat(pplr, rsFactOperate);
        cCur = 0;
        if (rgi[0] > 0) {
            cCur += 100 * rgi[0];
        } else {
            cCur += 121 * rgi[0];
        }
        if (rgi[1] < 0) {
            cCur -= 55 * rgi[1];
        } else {
            cCur -= rgi[1] * rgi[1] * 60;
        }
        if (rgi[2] > 0) {
            cCur += 40 * rgi[2];
        } else {
            cCur += 35 * rgi[2];
        }
        if (cCur > 700) {
            cCur = (int16_t)(cCur - 700) / 3 + 700;
        }
        if (rgi[2] <= -7) {
            if (rgi[2] >= -11) {
                cCur -= (-6 - rgi[2]) * 30;
            } else if (rgi[2] >= -14) {
                cCur -= (-12 - rgi[2]) * 45 + 225;
            } else {
                cCur -= 360;
            }
        }
        if (rgi[0] <= -3) {
            cCur -= (-2 - rgi[0]) * 20 * 3;
        }
        cPoints += cCur;
        if (GetRaceGrbit(pplr, ibitRaceCheapFact) != 0) {
            cPoints -= 175;
        }
        rgi[0] = 10 - GetRaceStat(pplr, rsMineProd);
        rgi[1] = 3 - GetRaceStat(pplr, rsMineBuild);
        rgi[2] = 10 - GetRaceStat(pplr, rsMineOperate);
        cCur = 0;
        if (rgi[0] > 0) {
            cCur += 100 * rgi[0];
        } else {
            cCur += 169 * rgi[0];
        }
        if (rgi[1] <= 0) {
            cCur -= 65 * rgi[1] - 80;
        } else {
            cCur -= 360;
        }
        if (rgi[2] > 0) {
            cCur += 40 * rgi[2];
        } else {
            cCur += 35 * rgi[2];
        }
        cPoints += cCur;
    } else {
        cPoints += 210;
    }
    cPoints -= rgRacePrimaryTrait[raMajor];
    cBad = 0;
    cGood = 0;
    for (i = 0; i <= 13; i++) {
        if (GetRaceGrbit(pplr, i) != 0) {
            if (rgRaceAdvDisPts[i] < 0) {
                cBad++;
            } else {
                cGood++;
            }
            cPoints += rgRaceAdvDisPts[i];
        }
    }
    if (cBad + cGood > 4) {
        cPoints -= (int16_t)((cBad + cGood) * 10 * (cBad + cGood - 4));
    }
    if (cGood - cBad > 3) {
        cPoints -= (int16_t)((cGood - cBad - 3) * 60);
    }
    if (cBad - cGood > 3) {
        cPoints -= (int16_t)((cBad - cGood - 3) * 40);
    }
    if (GetRaceGrbit(pplr, ibitRaceNoAdvScanner) != 0) {
        switch (raMajor) {
        case 6:
            cPoints -= 280;
            break;
        case 1:
            cPoints -= 200;
            break;
        case 9:
            cPoints -= 40;
        }
    }
    cCur = 0;
    for (i = 8; i <= 13; i++) {
        cCur += GetRaceStat(pplr, i) - 1;
    }
    if (cCur > 0) {
        cPoints -= (int16_t)(cCur * cCur * 130);
        if (cCur == 6) {
            cPoints += 1430;
        } else if (cCur == 5) {
            cPoints += 520;
        }
    } else if (cCur < 0) {
        cPoints += rgRaceDisEnvPts[-cCur - 1];
        if (-cCur > 4 && GetRaceStat(pplr, rsResGen) < 10) {
            cPoints -= 190;
        }
    }
    if (GetRaceGrbit(pplr, ibitRaceTech3) != 0) {
        cPoints -= 180;
    }
    if (raMajor == 8 && GetRaceStat(pplr, rsTechBonus1) == 2) {
        cPoints -= 100;
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
        if (iTerra == 0) {
            pctTerra = 0;
        } else if (iTerra == 1) {
            pctTerra = fTotalTerra == 0 ? 5 : 8;
        } else {
            pctTerra = fTotalTerra == 0 ? 15 : 17;
        }
        for (i = 0; i < 3; i++) {
            if ((pplr->rgEnvVar[i] > 100 || pplr->rgEnvVarMin[i] > 100 || pplr->rgEnvVarMax[i] > 100 || pplr->rgEnvVar[i] < 0 || pplr->rgEnvVarMin[i] < 0 ||
                 pplr->rgEnvVarMax[i] < 0) &&
                (pplr->rgEnvVar[i] != -1 || pplr->rgEnvVarMin[i] != -1 || pplr->rgEnvVarMax[i] != -1)) {
                pplr->rgEnvVarMax[i] = -1;
                pplr->rgEnvVarMin[i] = -1;
                pplr->rgEnvVar[i] = -1;
                pplr->fHacker = 1;
                rgplr[0] = *pplr;
            }
            if (pplr->rgEnvVar[i] < 0) {
                rgBase[i] = 50;
                rgInc[i] = 11;
                rgSteps[i] = 1;
            } else {
                rgBase[i] = pplr->rgEnvVarMin[i] - pctTerra;
                if (rgBase[i] < 0) {
                    rgBase[i] = 0;
                }
                iTry = pplr->rgEnvVarMax[i] + pctTerra;
                if (iTry > 100) {
                    iTry = 100;
                }
                rgInc[i] = iTry - rgBase[i];
                rgSteps[i] = 11;
            }
        }
        l3 = 0.0;
        for (i = 0; i < rgSteps[0]; i++) {
            if (i == 0 || rgSteps[0] <= 1) {
                iTry = rgBase[0];
            } else {
                iTry = (int16_t)(i * rgInc[0]) / (rgSteps[0] - 1) + rgBase[0];
            }
            if (iTerra != 0 && pplr->rgEnvVar[0] >= 0) {
                iDelta = pplr->rgEnvVar[0] - iTry;
                if (abs(iDelta) <= pctTerra) {
                    iDelta = 0;
                } else if (iDelta < 0) {
                    iDelta += pctTerra;
                } else {
                    iDelta -= pctTerra;
                }
                rgDelta[0] = iDelta;
                iTry = pplr->rgEnvVar[0] - iDelta;
            }
            pl.rgEnvVar[0] = LOBYTE(iTry);
            l2 = 0.0;
            for (j = 0; j < rgSteps[1]; j++) {
                if (j == 0 || rgSteps[1] <= 1) {
                    iTry = rgBase[1];
                } else {
                    iTry = (int16_t)(j * rgInc[1]) / (rgSteps[1] - 1) + rgBase[1];
                }
                if (iTerra != 0 && pplr->rgEnvVar[1] >= 0) {
                    iDelta = pplr->rgEnvVar[1] - iTry;
                    if (abs(iDelta) <= pctTerra) {
                        iDelta = 0;
                    } else if (iDelta < 0) {
                        iDelta += pctTerra;
                    } else {
                        iDelta -= pctTerra;
                    }
                    rgDelta[1] = iDelta;
                    iTry = pplr->rgEnvVar[1] - iDelta;
                }
                pl.rgEnvVar[1] = LOBYTE(iTry);
                l1 = 0;
                for (k = 0; k < rgSteps[2]; k++) {
                    if (k == 0 || rgSteps[2] <= 1) {
                        iTry = rgBase[2];
                    } else {
                        iTry = (int16_t)(k * rgInc[2]) / (rgSteps[2] - 1) + rgBase[2];
                    }
                    if (iTerra != 0 && pplr->rgEnvVar[2] >= 0) {
                        iDelta = pplr->rgEnvVar[2] - iTry;
                        if (abs(iDelta) <= pctTerra) {
                            iDelta = 0;
                        } else if (iDelta < 0) {
                            iDelta += pctTerra;
                        } else {
                            iDelta -= pctTerra;
                        }
                        rgDelta[2] = iDelta;
                        iTry = pplr->rgEnvVar[2] - iDelta;
                    }
                    pl.rgEnvVar[2] = LOBYTE(iTry);
                    pctDesire = PctPlanetDesirability(&pl, 0);
                    iDelta = rgDelta[0] + rgDelta[1] + rgDelta[2];
                    if (iDelta > pctTerra) {
                        pctDesire -= (int16_t)(iDelta - pctTerra);
                        if (pctDesire < 0) {
                            pctDesire = 0;
                        }
                    }
                    pctDesire = (uint32_t)(pctDesire * pctDesire);
                    if (iTerra == 0) {
                        pctDesire = (uint32_t)(pctDesire * 7);
                    } else if (iTerra == 1) {
                        pctDesire = (uint32_t)(pctDesire * 5);
                    } else {
                        pctDesire = (uint32_t)(pctDesire * 6);
                    }
                    l1 += pctDesire;
                }
                if (pplr->rgEnvVar[2] >= 0) {
                    l1 = (int32_t)(l1 * rgInc[2]) / 100;
                } else {
                    l1 = (uint32_t)(l1 * 11);
                }
                l2 = (double)l1 + l2;
            }
            if (pplr->rgEnvVar[1] >= 0) {
                l2 = l2 * (double)rgInc[1] / 100.0;
            } else {
                l2 *= 11.0;
            }
            l3 += l2;
        }
        if (pplr->rgEnvVar[0] >= 0) {
            l3 = l3 * (double)rgInc[0] / 100.0;
        } else {
            l3 *= 11.0;
        }
        lInnate += l3;
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

    plf = LocalAlloc(64, sizeof(LOGFONT));
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

    plf = LocalAlloc(64, sizeof(LOGFONT));
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
    if (pplr == 0) {
        pplr = &vplr;
    }
    iPts = CAdvantagePoints(pplr);
    bkMode = SetBkMode(hdc, OPAQUE);
    crSav = SetTextColor(hdc, iPts < 0 ? 127 : 0);
    crBkSav = SetBkColor(hdc, crButtonFace);
    c = _wsprintf(szWork, PCTD, iPts);
    RcCtrTextOut(hdc, &rc, szWork, -1);
    SetTextColor(hdc, 0);
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsPointsLeft, szWork);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    cch = CchGetString(idsAdvantage, szAdvantage);
    RightTextOut(hdc, rc.left, 3, szAdvantage, cch, 0);
    RightTextOut(hdc, rc.left, dyArial8 + 3, szWork, c, 0);
    rc.left -= dx + 4;
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
    ick = 0;
    for (i = 0; i < cs; i++) {
        ick ^= p[i];
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

    if (szFileSuggest != 0) {
        strcpy(szFile, szFileSuggest);
    } else {
        szFile[0] = 0;
    }
    szDirName[0] = 0;
    CchGetString(idsStarsRaceFilesR, szFilter);
    for (i = 0; (int16_t)(int8_t)szFilter[i] != 0; i++) {
        if ((int16_t)(int8_t)szFilter[i] == '|') {
            szFilter[i] = 0;
        }
    }
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndRaceParent;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 0x100;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 0x100;
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt = "r1";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN;
    if (GetSaveFileName(&ofn) != 0) {
        if (FCreateFile(5, -1, szFile) == 0) {
            AlertSz(PszFormatIds(idsStarsUnableSaveRaceDataFilePlease, NULL), MB_ICONHAND);
            return 0;
        }
        WriteRtPlr(pplr, NULL);
        icksum = IRaceChecksum(pplr);
        WriteRt(rtEOF, 2, &icksum);
        StreamClose();
        strcpy(szRaceFile, &szFile[ofn.nFileOffset]);
        return 1;
    }
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
    int16_t t_merge_5e8b_0001;
    int16_t t_merge_5ef5_0001;
    int16_t t_scratch_m16;

    pplr->szNames[0] = 0;
    iVal = Random(25);
    if (iVal < 4) {
        for (i = 0; i < 3; i++) {
            pplr->rgEnvVarMax[i] = -1;
            pplr->rgEnvVarMin[i] = -1;
            pplr->rgEnvVar[i] = -1;
        }
        pplr->pctIdealGrowth = LOBYTE(Random(4) + 2);
    } else if (iVal < 7) {
        for (i = 0; i < 3; i++) {
            pplr->rgEnvVar[i] = 50;
            pplr->rgEnvVarMin[i] = 0;
            pplr->rgEnvVarMax[i] = 100;
        }
        pplr->pctIdealGrowth = LOBYTE(Random(4) + 3);
    } else if (iVal < 9) {
        for (i = 0; i < 3; i++) {
            j = Random(2);
            if (i == 2 && pplr->rgEnvVar[0] == pplr->rgEnvVar[1]) {
                j = pplr->rgEnvVar[0] == 0 ? 0 : 1;
            }
            if (j == 0) {
                pplr->rgEnvVar[i] = 50;
                pplr->rgEnvVarMin[i] = 0;
                pplr->rgEnvVarMax[i] = 100;
            } else {
                pplr->pctIdealGrowth = LOBYTE(Random(4) + 2);
            }
        }
        pplr->pctIdealGrowth = LOBYTE(Random(5) + 2);
    } else {
        for (i = 0; i < 3; i++) {
            j = Random(40) * 2 + 20;
            k = Random(100 - j + 1);
            pplr->rgEnvVar[i] = LOBYTE(j / 2 + k);
            pplr->rgEnvVarMin[i] = LOBYTE(k);
            pplr->rgEnvVarMax[i] = LOBYTE(k + j);
        }
        if (iVal < 12) {
            i = Random(3);
            pplr->rgEnvVarMax[i] = -1;
            pplr->rgEnvVarMin[i] = -1;
            pplr->rgEnvVar[i] = -1;
        } else if (iVal < 14) {
            i = Random(3);
            pplr->rgEnvVar[i] = 50;
            pplr->rgEnvVarMin[i] = 0;
            pplr->rgEnvVarMax[i] = 100;
        } else if (iVal < 17) {
            i = Random(3);
            j = Random(81);
            pplr->rgEnvVar[i] = LOBYTE(j + 10);
            pplr->rgEnvVarMin[i] = LOBYTE(j);
            pplr->rgEnvVarMax[i] = LOBYTE(j + 20);
        }
        pplr->pctIdealGrowth = LOBYTE(Random(9) + 7);
    }
    iVal = Random(3);
    for (i = 8; i <= 13; i++) {
        t_merge_5e8b_0001 = iVal != 0 ? Random(3) : 1;
        SetRaceStat(pplr, i, t_merge_5e8b_0001);
    }
    SetRaceStat(pplr, rsMajorAdv, Random(10));
    iVal = Random(4);
    for (i = 0; i <= 13; i++) {
        t_merge_5ef5_0001 = iVal != 0 ? Random(2) : 0;
        SetRaceGrbit(pplr, i, t_merge_5ef5_0001);
    }
    SetRaceGrbit(pplr, ibitRaceTech3, Random(2));
    SetRaceGrbit(pplr, ibitRaceCheapFact, Random(2));
    iVal = Random(3);
    if (iVal == 0) {
        for (i = 0; i <= 6; i++) {
            pplr->rgAttr[i] = vrgplrDef[0].rgAttr[i];
        }
        pplr->rgAttr[7] = LOBYTE(Random(5));
    } else {
        for (i = 0; i <= 7; i++) {
            t_scratch_m16 = Random((int16_t)(int8_t)rgRaceStatMax[i] + 1 - (int16_t)(int8_t)rgRaceStatMin[i]);
            pplr->rgAttr[i] = LOBYTE((int16_t)(int8_t)rgRaceStatMin[i] + t_scratch_m16);
        }
    }
    if (strcmp(pplr->szName, PszGetCompressedString(idsRandom2)) == 0) {
        CchGetString(Random(24) + 1390, pplr->szName);
    }
    cPts = CAdvantagePoints(pplr);
    if (cPts < 0 || cPts > 50) {
        cPass = 0;
        while (1) {
            cPts = CAdvantagePoints(pplr);
            if (cPts >= 0 && cPts <= 50) {
                return;
            }
            if (cPass++ > 250)
                break;
            iVal = Random(10);
            dAwayCur = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
            if (iVal < 3) {
                i = Random(6);
                j = pplr->rgAttr[i + 8];
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
            } else if (iVal < 6) {
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
            } else if (iVal < 9) {
                iVal = Random(7);
                j = GetRaceStat(pplr, iVal);
                for (i = -1; i <= 1; i += 2) {
                    SetRaceStat(pplr, iVal, j + i);
                    cPts = CAdvantagePoints(pplr);
                    dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                    if (dAwayNew < dAwayCur)
                        break;
                }
                if (i > 1) {
                    SetRaceStat(pplr, iVal, j);
                }
            } else if (Random(2) != 0) {
                j = pplr->pctIdealGrowth;
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
            } else {
                iVal = Random(3);
                if (pplr->rgEnvVar[iVal] < 0) {
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
                } else {
                    plrT = *pplr;
                    pplr->rgEnvVarMax[iVal] = -1;
                    pplr->rgEnvVarMin[iVal] = -1;
                    pplr->rgEnvVar[iVal] = -1;
                    cPts = CAdvantagePoints(pplr);
                    dAwayNew = -cPts <= cPts - 50 ? cPts - 50 : -cPts;
                    if (dAwayNew >= dAwayCur) {
                        *pplr = plrT;
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
    if (GetRaceStat(&rgplr[iplr], rsMajorAdv) == raCheapCol) {
        return rgplr[iplr].pctIdealGrowth * 2;
    }
    return rgplr[iplr].pctIdealGrowth;
}
