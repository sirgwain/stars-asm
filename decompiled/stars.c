#include "common.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char   *pch;
    char   *lpT;
    int16_t i;
    MSG     msg;

    hInst = hInstance;
    szBase[0] = 0;
    ini.wFlags = 0;
    memset(&tutor, 0, sizeof(TUTOR));
    memset(&vtimer, 0, sizeof(TIMER));
    vtimer.fAutoGenWhenIn = 1;
    if (hPrevInstance == 0 && InitMDIApp() == 0) {
        AlertSz(PszFormatIds(idsUnableInitializeStars, NULL), MB_ICONHAND);
        return 0;
    }
    Randomize2(GetTickCount());
    if (FCreateStuff() == 0) {
        return 0;
    }
    if (FGetSystemColors() == 0) {
        AlertSz(PszFormatIds(idsUnableInitializeStars, NULL), MB_ICONHAND);
        return 0;
    }
    if (InitInstance(nCmdShow) == 0) {
        AlertSz(PszFormatIds(idsUnableInitializeStars, NULL), MB_ICONHAND);
        return 0;
    }
    lpT = lpCmdLine;
L_0121:
    while (*lpT != 0) {
        while (1) {
            switch (*lpT) {
            case ' ':
                lpT++;
                continue;
            case '-':
            case '/':
                goto L_0164;
            default:
                goto L_0517;
            }
            goto L_0121;
        }
    L_0164:
        for (lpT++; *lpT != 0 && *lpT != ' '; lpT++) {
            if ((uint16_t)(*lpT - 65) <= 55) {
                switch (*lpT) {
                case 'W':
                case 'w':
                    ini.fWait = 1;
                    break;
                case 'D':
                case 'd':
                    lpT++;
                    while (1) {
                        switch (*lpT) {
                        case 'F':
                        case 'f':
                            ini.fDumpFleets = 1;
                            goto L_0231;
                        case 'P':
                        case 'p':
                            ini.fDumpPlanets = 1;
                            goto L_0231;
                        case 'M':
                        case 'm':
                            ini.fDumpMap = 1;
                        default:
                        L_0231:
                            lpT++;
                            continue;
                        case 0:
                        case ' ':
                            break;
                        }
                        break;
                    }
                    lpT--;
                    break;
                case 'G':
                case 'g':
                    ini.fGen = 1;
                    i = 0;
                    while (lpT[1] >= '0' && lpT[1] <= '9') {
                        lpT++;
                        i = 10 * i + *lpT - 48;
                        if (i > 1000) {
                            i = 1000;
                            for (; lpT[1] >= '0' && lpT[1] <= '9'; lpT++) {
                            }
                            break;
                        }
                    }
                    if (i <= 0)
                        break;
                    ini.cTurnGen = i - 1;
                    break;
                case 'A':
                case 'a':
                    ini.fNewGame = 1;
                    break;
                case 'H':
                case 'h':
                    gd.fHotSeat = 1;
                    break;
                case 'X':
                case 'x':
                    gd.fExitWindows = 1;
                    break;
                case 'B':
                case 'b':
                    for (lpT++; *lpT == ' '; lpT++) {
                    }
                    pch = szBase;
                    for (; *lpT != 0 && *lpT != ' '; lpT++) {
                        *pch = *lpT;
                        pch++;
                    }
                    *pch = 0;
                    lpT--;
                    if (FSetUpBatchProcessing() == 0)
                        break;
                    ini.fBatch = 1;
                    ini.fGen = 1;
                    ini.fStartupFile = 1;
                    ini.fCmdLine = 1;
                    break;
                case 'V':
                case 'v':
                    ini.fValidate = 1;
                    break;
                case 'L':
                case 'l':
                    ini.fLogging = 1;
                    break;
                case 'T':
                case 't':
                    ini.fTry = 1;
                    break;
                case 'C':
                case 'c':
                    ini.fCmdLine = szBase[0] == 0 ? 0 : 1;
                    break;
                case 'P':
                case 'p':
                    for (lpT++; *lpT == ' '; lpT++) {
                    }
                    pch = szPassLast;
                    for (; *lpT != 0 && *lpT != ' ' && pch < &szPassLast[15]; lpT++) {
                        *pch = *lpT;
                        pch++;
                    }
                    *pch = 0;
                    lpT--;
                    lSaltLast = LSaltFromSz(szPassLast);
                }
            }
        }
        continue;
    L_0517:
        pch = szBase;
        while (*lpT != 0 && *lpT != ' ') {
            *pch = *lpT;
            lpT++;
            pch++;
        }
        *pch = 0;
        ini.fStartupFile = 1;
        ini.fCmdLine = 1;
    }
    PostMessage(hwndFrame, WM_STARS_STARTUP, 0, 0);
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (hwndTitle != 0) {
            if (TranslateAccelerator(hwndFrame, hAccelTitle, &msg) == 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else if (IsIconic(hwndFrame) != 0 || TranslateAccelerator(hwndFrame, hAccel, &msg) == 0) {
            TranslateMessage(&msg);
            if (((msg.message != WM_KEYDOWN && msg.message != WM_KEYUP) || FHandleKey(msg.hwnd, msg.message, msg.wParam, msg.lParam) == 0) &&
                (msg.message != WM_CHAR || FHandleChar(msg.hwnd, msg.wParam, msg.lParam) == 0)) {
                DispatchMessage(&msg);
            }
        }
    }
    FreeStuff();
    return (int16_t)msg.wParam;
}

int16_t FSetUpBatchProcessing() {
    char   *pch;
    jmp_buf env;
    int16_t fSuccess;
    int16_t cb;

    fSuccess = 0;
    penvMem = &env;
    if (setjmp(env) == 0) {
        StreamOpen(szBase, mdRead);
        cb = LOWORD(filelength(hf));
        lpchBatch = LpAlloc(cb, htPerm);
        RgFromStream(lpchBatch, cb);
        lpchBatchMac = lpchBatch + cb;
        pch = szBase;
        while (*lpchBatch != '\n' && lpchBatch != lpchBatchMac) {
            *pch = *lpchBatch;
            lpchBatch++;
            pch++;
        }
        lpchBatch++;
        pch[-1] = 0;
        fSuccess = 1;
    }
    penvMem = 0;
    StreamClose();
    if (fSuccess == 0) {
        szBase[0] = 0;
    }
    return fSuccess;
}

int16_t IPlrAlsoCheater(int16_t iplr) {
    int16_t i;

    if (FValidSerialLong(vrgts[iplr].lSerialNumber) == 0) {
        return -1;
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (i != iplr && rgplr[i].fCheater != 0 && vrgts[iplr].lSerialNumber == vrgts[i].lSerialNumber &&
            fmemcmp(vrgts[iplr].rgbConfig, vrgts[i].rgbConfig, 11) != 0) {
            return i;
        }
    }
    return -1;
}

int16_t FGetSystemColors() {
    HDC         hdc;
    BITMAPINFO *lpbi;
    int16_t     t_scratch_m6;

    if (hbrButtonFace != 0) {
        FreeHbr(hbrButtonFace);
    }
    if (hbrButtonHilite != 0) {
        FreeHbr(hbrButtonHilite);
    }
    if (hbrButtonShadow != 0) {
        FreeHbr(hbrButtonShadow);
    }
    if (hbrButtonText != 0) {
        FreeHbr(hbrButtonText);
    }
    if (hbrWindowText != 0) {
        FreeHbr(hbrWindowText);
    }
    if (hbrWindow != 0) {
        FreeHbr(hbrWindow);
    }
    if (hbrWindowFrame != 0) {
        FreeHbr(hbrWindowFrame);
    }
    if (hbrDesktop != 0) {
        FreeHbr(hbrDesktop);
    }
    crButtonFace = GetSysColor(COLOR_BTNFACE);
    hbrButtonFace = HbrGet(crButtonFace);
    crButtonHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
    hbrButtonHilite = HbrGet(crButtonHilite);
    crButtonShadow = GetSysColor(COLOR_BTNSHADOW);
    hbrButtonShadow = HbrGet(crButtonShadow);
    crButtonText = GetSysColor(COLOR_BTNTEXT);
    hbrButtonText = HbrGet(crButtonText);
    hbrWindowFrame = HbrGet(GetSysColor(COLOR_WINDOWFRAME));
    hbrDesktop = HbrGet(GetSysColor(COLOR_BACKGROUND));
    crWindow = GetSysColor(COLOR_WINDOW);
    hbrWindow = HbrGet(crWindow);
    crWindowText = GetSysColor(COLOR_WINDOWTEXT);
    hbrWindowText = HbrGet(crWindowText);
    dyTitleBar = GetSystemMetrics(SM_CYCAPTION);
    dxWinFrame = GetSystemMetrics(SM_CXFRAME);
    dyWinFrame = GetSystemMetrics(SM_CYFRAME);
    if (hdibPlaque != 0) {
        lpbi = (BITMAPINFO *)GlobalLock(hdibPlaque);
        lpbi->bmiColors[249].rgbRed = LOBYTE(LOWORD(crButtonFace));
        lpbi->bmiColors[249].rgbGreen = LOBYTE(LOWORD(crButtonFace) >> 8);
        lpbi->bmiColors[249].rgbBlue = LOBYTE(HIWORD(crButtonFace));
        GlobalUnlock(hdibPlaque);
    }
    if (hdibToolbar != 0) {
        lpbi = (BITMAPINFO *)GlobalLock(hdibToolbar);
        lpbi->bmiColors[253].rgbRed = LOBYTE(LOWORD(crButtonFace));
        lpbi->bmiColors[253].rgbGreen = LOBYTE(LOWORD(crButtonFace) >> 8);
        lpbi->bmiColors[253].rgbBlue = LOBYTE(HIWORD(crButtonFace));
        GlobalUnlock(hdibToolbar);
    }
    hdc = GetDC(NULL);
    t_scratch_m6 = GetDeviceCaps(hdc, BITSPIXEL);
    vcScreenColors = t_scratch_m6 * GetDeviceCaps(hdc, PLANES);
    ReleaseDC(NULL, hdc);
    return 1;
}

void FreeStuff() {
    int16_t i;
    int16_t j;

    if (hbrButtonFace != 0) {
        FreeHbr(hbrButtonFace);
    }
    if (hbrButtonHilite != 0) {
        FreeHbr(hbrButtonHilite);
    }
    if (hbrButtonShadow != 0) {
        FreeHbr(hbrButtonShadow);
    }
    if (hbrButtonText != 0) {
        FreeHbr(hbrButtonText);
    }
    if (hbrWindowText != 0) {
        FreeHbr(hbrWindowText);
    }
    if (hbrWindow != 0) {
        FreeHbr(hbrWindow);
    }
    if (hbrWindowFrame != 0) {
        FreeHbr(hbrWindowFrame);
    }
    if (hbrDesktop != 0) {
        FreeHbr(hbrDesktop);
    }
    if (hbrRed != 0) {
        FreeHbr(hbrRed);
    }
    if (hbrGreen != 0) {
        FreeHbr(hbrGreen);
    }
    if (hbrBlue != 0) {
        FreeHbr(hbrBlue);
    }
    if (hbrPurple != 0) {
        FreeHbr(hbrPurple);
    }
    if (hbrTooltip != 0) {
        FreeHbr(hbrTooltip);
    }
    for (i = 0; i <= 4; i++) {
        if (rghbrMineral[i] != 0) {
            FreeHbr(rghbrMineral[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            FreeHbr(rghbrPlanetAttr[i][j]);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            FreeHbr(rghbrMinSum[i][j]);
        }
    }
    FreeProcInstance(lpfnFakeComboProc);
    FreeProcInstance(lpfnFakeCEProc);
    FreeProcInstance(lpfnFakeEditProc);
    FreeProcInstance(lpfnFakeListProc);
    FreeProcInstance(lpfnHostTimerProc);
    FreeProcInstance(lpfnBrowserDlgProc);
    if (lpfnTutorDlgProc != 0) {
        FreeProcInstance(lpfnTutorDlgProc);
    }
    DeleteObject(hrgnHuge);
    DeleteObject(hrgnScratch);
    SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32512)));
    DestroyCursor(hcurScanner);
    DestroyCursor(hcurOpenGrab);
    DestroyCursor(hcurCloseGrab);
    DestroyCursor(hcurScanAdd);
    DestroyCursor(hcurTrashCan);
    DestroyCursor(hcurNoWay);
    DestroyCursor(hcurResizeWE);
    DestroyCursor(hcurResizeNS);
    DestroyCursor(hcurResize4Way);
    DestroyCursor(hcurArrowHelp);
    DestroyCursor(hcurHand);
    DeleteObject(hbmpScanner);
    DeleteObject(hbmpNumbers);
    DeleteObject(hbmpScanShip);
    DeleteObject(hbmpUnknownPlanet);
    DestroyIcon(hiconStars);
    DestroyIcon(hiconHost);
    DestroyIcon(hiconWait);
    for (i = 0; i < 7; i++) {
        DestroyIcon(rghiconVCR[i]);
    }
    GlobalUnlock(hdibPlanets);
    FreeResource(hdibPlanets);
    GlobalUnlock(hdibThings);
    FreeResource(hdibThings);
    GlobalUnlock(hdibToolbar);
    FreeResource(hdibToolbar);
    GlobalUnlock(hdibRaces);
    FreeResource(hdibRaces);
    GlobalUnlock(hdibRacesT);
    FreeResource(hdibRacesT);
    GlobalUnlock(hdibRacesX);
    FreeResource(hdibRacesX);
    DeleteObject(hbmpBackBld);
    DeleteObject(hbmpMsg);
    DeleteObject(hbmpMono);
    FreeResource(hdibPlaque);
    for (i = 0; i < 5; i++) {
        GlobalUnlock(rghdibShips[i]);
        FreeResource(rghdibShips[i]);
        GlobalUnlock(rghdibShipsT[i]);
        FreeResource(rghdibShipsT[i]);
    }
    for (i = 0; i < 7; i++) {
        GlobalUnlock(rghdibInventory[i]);
        FreeResource(rghdibInventory[i]);
    }
    FreeLp(lpLog, htLog);
    lpLog = NULL;
    FreeLp(lpMsg, htMsg);
    lpMsg = NULL;
    DeleteObject(vhpal);
    if (vhpalSplash != 0) {
        DeleteObject(vhpalSplash);
    }
    FreeHbr(hbrShip);
    FreeHbr(hbrStarbase);
    FreeHbr(hbrBBlue);
    FreeHbr(hbrEnemy);
    FreeHbr(hbrSelect);
    FreeHbr(hbrRadar);
    if (hbrRadarNear != 0) {
        FreeHbr(hbrRadarNear);
    }
    FreeHbr(hbrLightGray);
    FreeHbr(hbrGray);
    FreeHbr(hbrYellow);
    FreeHbr(hbrDkYellow);
    DeleteObject(hbr50Screen);
    for (i = 0; i < 3; i++) {
        DeleteObject(rghbrPat[i]);
    }
    DeleteObject(hbrCargo);
    DeleteObject(hbrDock);
    DeleteObject(hpenShip);
    DeleteObject(hpenDkGreen);
    DeleteObject(hpenDkPurple);
    DeleteObject(hpenStarbase);
    DeleteObject(hpenEnemy);
    DeleteObject(hpenMassPath);
    DeleteObject(hpenRadar);
    if (hpenRadarNear != 0) {
        DeleteObject(hpenRadarNear);
    }
    DeleteObject(hpenDkBlue);
    DeleteObject(hpenYellow);
    DeleteObject(hpenDkYellow);
    DeleteObject(rghfontArial10[0]);
    DeleteObject(rghfontArial10[1]);
    for (i = 0; i < 5; i++) {
        DeleteObject(rghfontArial8[i]);
    }
    DeleteObject(rghfontArial6[0]);
    DeleteObject(rghfontArial7[0]);
    for (i = 0; i < 12; i++) {
        FreeHb(rglphb[i]);
    }
    return;
}

char *SzVersion() {
    _wsprintf(szWork, PszGetCompressedString(idsVersionD02dC), 2, 60, 106);
    return szWork;
}

INT_PTR CALLBACK About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT    rc;
    HDC     hdc;
    int16_t i;
    HWND    hwndCtl;
    FARPROC lpProc;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            iAbout1st = -11;
            iAboutPartial = 0;
            SetWindowText(GetDlgItem(hwnd, IDC_ABOUT_DEMO_TEXT), SzVersion());
            uTimerId = SetTimer(hwnd, 14, 50, NULL);
            return 1;
        case WM_TIMER:
            hwndCtl = GetDlgItem(hwnd, IDC_ABOUT_CREDITS_TEXT);
            iAboutPartial += 2;
            if (iAboutPartial >= dyArial8) {
                iAboutPartial = 0;
                iAbout1st++;
                if (iAbout1st > 78) {
                    iAbout1st = -11;
                }
            }
            GetClientRect(hwndCtl, &rc);
            hdc = GetDC(hwndCtl);
            SelectObject(hdc, rghfontArial8[1]);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, crButtonFace);
            SetTextColor(hdc, crButtonText);
            IntersectClipRect(hdc, 0, 0, rc.right, rc.bottom);
            rc.top -= iAboutPartial;
            rc.bottom = rc.top + dyArial8;
            for (i = iAbout1st; i < iAbout1st + 10; i++) {
                if (i >= 0 && i < 77) {
                    RcCtrTextOut(hdc, &rc, PszGetCompressedString(i + 631), -1);
                } else if (i >= 77) {
                    break;
                }
                OffsetRect(&rc, 0, dyArial8);
            }
            rc.bottom = 1000;
            FillRect(hdc, &rc, hbrButtonFace);
            SelectClipRgn(hdc, NULL);
            ReleaseDC(hwnd, hdc);
            break;
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                KillTimer(hwnd, uTimerId);
                uTimerId = 0;
                EndDialog(hwnd, 1);
                return 1;
            case IDC_ABOUT_ORDER_INFO:
                lpProc = MakeProcInstance(OrderInfoDlg, hInst);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ORDER_INFO), hwnd, lpProc);
                FreeProcInstance(lpProc);
            }
        }
    } else if (HIWORD(lParam) == 6) {
        SetBkColor((HDC)wParam, crButtonFace);
        return (INT_PTR)hbrButtonFace;
    }
    return 0;
}

INT_PTR CALLBACK OrderInfoDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT rc;

    if (message != WM_ERASEBKGND) {
        if (IS_WM_CTLCOLOR(message) == 0) {
            if (message == WM_COMMAND && (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL || GET_WM_COMMAND_ID(wParam, lParam) == IDOK)) {
                EndDialog(hwnd, 1);
                return 1;
            }
        } else if (HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
        return 0;
    }
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;
}

int16_t FHandleChar(HWND hwnd, uint16_t ch, int32_t lParam) {
    HWND hwndF;

    if ((hwndScanner != 0 && (ch == 43 || ch == 45)) || (ch == 118 || ch == 86)) {
        hwndF = GetFocus();
        if (hwndMessage == 0 || hwndF != hwndMsgEdit) {
            SendMessage(hwndScanner, WM_CHAR, ch, lParam);
            return 1;
        }
    }
    return 0;
}

int16_t FHandleKey(HWND hwnd, int16_t iMsg, int16_t iKey, uint32_t dw) {
    HWND          hwndF;
    POINT16       pt;
    HWND          hwndOver;
    int16_t       i;
    ToolbarButton itb;
    uint16_t      md;
    int16_t       iWarp;
    int16_t       iwp;

    if (iMsg == WM_KEYDOWN) {
        if (iKey == VK_ESCAPE && hwndBrowser != 0 && GetActiveWindow() == hwndBrowser) {
            DestroyWindow(hwndBrowser);
            return 1;
        }
        if (iKey == VK_ESCAPE && hwndPopup != 0) {
            SendMessage(hwndPopup, WM_LBUTTONUP, 0, 0);
            return 1;
        }
        if (iKey == VK_ESCAPE && hwndReportDlg != 0) {
            DestroyWindow(hwndReportDlg);
            return 1;
        }
    } else if (iMsg == WM_KEYUP && hwndTb != 0 && (iKey == VK_ESCAPE || iKey == VK_RETURN)) {
        hwndF = GetParent(GetFocus());
        if (hwndF == hwndTb || GetParent(hwndF) == hwndTb) {
            TerminateToolbarFocus(iKey == VK_ESCAPE ? 1 : 0);
        }
    }
    if (iKey == VK_SHIFT && hwndScanner != 0) {
        GetCursorPos16(&pt);
        hwndOver = WindowFromPoint(PointFrom16(pt));
        if (hwndOver == hwndScanner) {
            SendMessage(hwndOver, WM_SETCURSOR, (WPARAM)hwndOver, 0);
        }
    }
    if (iMsg != WM_KEYDOWN) {
        return 0;
    }
    switch (iKey) {
    default:
        if (iKey < '0' || iKey > '9') {
            switch (iKey) {
            default:
                return 0;
            case VK_OEM_COMMA:
            case VK_OEM_PERIOD:
            case VK_OEM_4:
            case VK_OEM_6:
                break;
            }
        }
    case VK_BACK:
    case VK_DELETE:
    case VK_DOWN:
    case VK_UP:
    case VK_HOME:
    case VK_END:
        hwndF = GetFocus();
        if (hwndMessage != 0) {
            if (hwndTb != 0 && (hwndTb == hwndF || GetParent(hwndF) == hwndTb || GetParent(GetParent(hwndF)) == hwndTb)) {
                return 0;
            }
            for (i = 0; i < 3; i++) {
                if (hwndF == rghwndOrderDD[i]) {
                    return 0;
                }
            }
            if (hwndF == hwndFleetCompLB || hwndF == hwndPlanetProdLB || hwndF == hwndMsgEdit || hwndF == hwndMsgDrop || hwndF == hwndOrderED ||
                hwndF == hwndMsgScroll || hwndF == hwndFleetCompLB || hwndF == hwndShipDD) {
                return 0;
            }
            if (hwndBrowser == 0 || hwndF != GetDlgItem(hwndBrowser, IDC_BROWSER_COMPONENT_CATEGORY))
                goto L_1939;
            return 0;
        }
    L_1939:
        if (iKey >= '0' && iKey <= '9') {
            if (iKey >= '1' && iKey <= '6') {
                md = iKey - 49;
                if (md != (grbitScan & grbitScanViewMask)) {
                    ExecuteButton(iKey - 49, 1);
                    InvalidateRect(hwndTb, NULL, 0);
                }
                return 1;
            }
            switch (iKey) {
            case '7':
                itb = tbScannerCoverage;
                break;
            case '8':
                itb = tbMineFields;
                break;
            case '9':
                itb = tbFleetPaths;
                break;
            case '0':
                if (GetKeyState(VK_SHIFT) < 0) {
                    itb = tbShipCounts;
                } else {
                    itb = tbPlanetNames;
                }
            }
            ExecuteButton(itb, FIsButtonDown(itb) == 0 ? 1 : 0);
            InvalidateRect(hwndTb, NULL, 0);
            return 1;
        }
        switch (iKey) {
        default:
            return 0;
        case VK_BACK:
        case VK_DELETE:
            if (sel.grobj != grobjFleet)
                break;
            iKey = VK_BACK;
            DeleteCurWayPoint(8);
            break;
        case VK_END:
        case VK_HOME:
        case VK_UP:
        case VK_DOWN:
            if (hwndF == hwndShipLB) {
                return 0;
            }
            SendMessage(hwndMessage, WM_KEYDOWN, iKey, dw);
            break;
        case VK_OEM_COMMA:
        case VK_OEM_PERIOD:
            if (sel.grobj == grobjFleet && (sel.iwpAct > 0 || sel.fl.cord > 1)) {
                iwp = sel.iwpAct <= 0 ? 1 : sel.iwpAct;
                iWarp = sel.fl.lpplord->rgord[iwp].iWarp;
                if (iKey == VK_OEM_COMMA) {
                    iWarp--;
                } else {
                    iWarp++;
                }
                if (iWarp >= 0 && iWarp <= 11) {
                    sel.fl.lpplord->rgord[iwp].iWarp = iWarp;
                    FLookupFleet(-1, &sel.fl);
                    DrawPlanShip(NULL, tileFleetOrders | tileFleetComp | tileMinimized);
                }
            }
            return 1;
        case VK_OEM_4:
        case VK_OEM_6:
            pt.x = 0;
            pt.y = 0;
            ExecuteReportClick(pt, rptEnemyFleets, 0, iKey == VK_OEM_4 ? -2 : -1);
            return 1;
        }
        return 1;
    }
}
