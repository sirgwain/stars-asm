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
    vtimer.fAutoGenWhenIn = TRUE;
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
    while (*lpT != 0) {
        for (; *lpT == ' '; lpT++) {
        }
        if (*lpT == '-' || *lpT == '/') {
            for (lpT++; *lpT != 0 && *lpT != ' '; lpT++) {
                switch (*lpT) {
                case 'W':
                case 'w':
                    ini.fWait = TRUE;
                    break;
                case 'D':
                case 'd':
                    for (lpT++; *lpT != 0 && *lpT != ' '; lpT++) {
                        switch (*lpT) {
                        case 'F':
                        case 'f':
                            ini.fDumpFleets = TRUE;
                            break;
                        case 'P':
                        case 'p':
                            ini.fDumpPlanets = TRUE;
                            break;
                        case 'M':
                        case 'm':
                            ini.fDumpMap = TRUE;
                        }
                    }
                    lpT--;
                    break;
                case 'G':
                case 'g':
                    ini.fGen = TRUE;
                    i = 0;
                    while (lpT[1] >= '0' && lpT[1] <= '9') {
                        lpT++;
                        i = 10 * i + *lpT - '0';
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
                    ini.fNewGame = TRUE;
                    break;
                case 'H':
                case 'h':
                    gd.fHotSeat = TRUE;
                    break;
                case 'X':
                case 'x':
                    gd.fExitWindows = TRUE;
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
                    ini.fBatch = TRUE;
                    ini.fGen = TRUE;
                    ini.fStartupFile = TRUE;
                    ini.fCmdLine = TRUE;
                    break;
                case 'V':
                case 'v':
                    ini.fValidate = TRUE;
                    break;
                case 'L':
                case 'l':
                    ini.fLogging = TRUE;
                    break;
                case 'T':
                case 't':
                    ini.fTry = TRUE;
                    break;
                case 'C':
                case 'c':
                    ini.fCmdLine = szBase[0] != 0;
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
        } else {
            pch = szBase;
            while (*lpT != 0 && *lpT != ' ') {
                *pch = *lpT;
                lpT++;
                pch++;
            }
            *pch = 0;
            ini.fStartupFile = TRUE;
            ini.fCmdLine = TRUE;
        }
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
