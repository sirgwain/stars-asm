#include "common.h"

uint8_t vrgbShuffleSerial[21] = {11, 4, 5, 16, 17, 12, 19, 15, 10, 1, 14, 13, 3, 18, 2, 20, 9, 7, 0, 8, 6};
char    rgTOWidth[2][2] = {{-3}, {2, 1}};

int16_t InitMDIApp() {
    WNDCLASS wc;

    wc.style = 11;
    wc.lpfnWndProc = (WNDPROC)FrameWndProc16;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, "StarsIco");
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32512));
    wc.hbrBackground = (HBRUSH)13;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = szFrame;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)MessageWndProc;
    wc.hIcon = 0;
    wc.lpszMenuName = NULL;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMessage;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)ScannerWndProc;
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = szScan;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)MineWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMine;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 520;
    wc.lpfnWndProc = (WNDPROC)TbWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szTb;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0x200;
    wc.lpfnWndProc = (WNDPROC)PlanetWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szPlanet;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 2560;
    wc.lpfnWndProc = (WNDPROC)PopupWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szPopup;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 2560;
    wc.lpfnWndProc = (WNDPROC)TooltipWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szTooltip;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0x200;
    wc.lpfnWndProc = (WNDPROC)BrowserWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szBrowser;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)TitleWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32512));
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szTitle;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 11;
    wc.lpfnWndProc = (WNDPROC)ReportDlg;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32512));
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szReport;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    return 1;
}

void CreateChildWindows() {
    char    szData[100];
    POINT16 pt;
    char   *psz;
    char    szGame[15];

    if (idPlayer != -1) {
        for (psz = &szBase[strlen(szBase) - 1]; psz > szBase && psz[-1] != '\\' && psz[-1] != ':'; psz--) {
        }
        szGame[8] = 0;
        strncpy(szGame, psz, 8);
        strlwr(szGame);
        _wsprintf(&szGame[strlen(szGame)], ".m%d", idPlayer + 1);
        _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, NULL), szGame);
    } else {
        CchGetString(idsStarsSHostMode, szWork);
        _wsprintf(szData, szWork, game.szName);
    }
    SetWindowText(hwndFrame, szData);
    if (idPlayer != -1) {
        if (hwndScanner == 0) {
            hwndScanner = CreateWindow(szScan, NULL, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndScanner, NULL, 1);
            yScanTop = 1000;
            xScanTop = 1000;
            SetScanScrollBars(hwndScanner);
        }
        if (hwndMine == 0) {
            hwndMine = CreateWindow(szMine, NULL, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndMine, NULL, 1);
        }
        if (hwndPlanet == 0) {
            hwndPlanet = CreateWindow(szPlanet, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndPlanet, NULL, 1);
        }
        if (hwndTb == 0) {
            hwndTb = CreateWindow(szTb, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        } else {
            InvalidateRect(hwndTb, NULL, 1);
        }
        if (hwndMessage != 0) {
            DestroyWindow(hwndMessage);
        }
        hwndMessage = CreateWindow(szMessage, NULL, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, NULL, hInst, NULL);
        RefitFrameChildren();
    }
    return;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC          hdc;
    int16_t      i;
    HPALETTE     hpalSav;
    TEXTMETRIC   tm;
    int16_t      ich;
    POINT16      pt;
    char        *pch;
    char         szTemp[80];
    FARPROC      lpProc;
    int16_t      fRet;
    int32_t      lSerial;
    int16_t      fErrSav;
    int16_t      idCur;
    int16_t      id;
    uint16_t     uTimerIdOld;
    char         szExt[4];
    int16_t      iOffset;
    int16_t      idPlanet;
    RECT         rc;
    RECT         rc2;
    PAINTSTRUCT  ps;
    int16_t      yOffset;
    HBRUSH       hbrSav;
    HCURSOR      hcs;
    POINT16      ptOld;
    PaneSplitter grSel;
    POINT16      ptAct;
    POINT16      ptD;
    POINT16      ptStart;
    POINT16      ptChg;

    switch (msg) {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        FCreateFonts(hdc);
        GetTextMetrics(hdc, &tm);
        dySysFont = tm.tmHeight;
        dySBar = (dyArial8 + 12) * 2;
        ReleaseDC(hwnd, hdc);
        InitTiles();
        EnsureTileSize(iWindowLayout == layoutSmall ? 1 : 0);
        return 0;
    case WM_QUERYNEWPALETTE:
    MapIt:
        if (hwndTitle != 0) {
            return SendMessage(hwndTitle, msg, wParam, lParam);
        }
        hdc = GetDC(hwnd);
        hpalSav = SelectPalette(hdc, vhpal, 0);
        i = RealizePalette(hdc);
        SelectPalette(hdc, hpalSav, 0);
        ReleaseDC(hwnd, hdc);
        if (i != 0) {
            InvalidateRect(hwnd, NULL, 1);
            return 1;
        }
        return 0;
    case WM_PALETTECHANGED:
        if ((HWND)wParam != hwnd)
            goto MapIt;
        return 0;
    case WM_INITMENU:
        InitializeMenu((HMENU)wParam);
        return 0;
    case WM_STARS_STARTUP:
        idPlayer = -1;
        if (ini.fCmdLine != 0) {
            ini.fCmdLine = 0;
            if (ini.fValidate != 0) {
                fFileErrSilent = 1;
                ClearFile(7);
                if (FLoadGame(szBase, "hst") != 0) {
                    VerifyTurns();
                    DestroyCurGame();
                    EnsureAis();
                    _wsprintf(szTemp, "\"%s\" Year: %d", game.szName, game.turn + 2400);
                    OutputSz(7, szTemp);
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgOut[i] + 1 > 3) {
                            ich = _wsprintf(szTemp, "Error: %d: ", i + 1);
                        } else {
                            ich = _wsprintf(szTemp, "%d: ", i + 1);
                        }
                        if (gd.fNoHostNames == 0) {
                            ich += _wsprintf(&szTemp[ich], "\"%s\" ", PszPlayerName(i, 1, 1, 1, 0, NULL));
                        }
                        strcat(szTemp, PszGetCompressedString(rgOut[i] + 716));
                        if (rgplr[i].fHacker != 0) {
                            strcat(szTemp, " - HACKER");
                        }
                        OutputSz(7, szTemp);
                    }
                }
            } else if (ini.fNewGame != 0) {
                if (vSerialNumber != 0) {
                    GenNewGameFromFile(szBase);
                }
            } else {
                if (ini.fGen != 0) {
                    while (1) {
                        if ((ini.fWait == 0 && ini.fTry == 0) || CTurnsOutSafe() == 0) {
                            EnsureAis();
                            FGenerateTurn();
                            if (ini.fBatch == 0 || lpchBatch >= lpchBatchMac) {
                                if (ini.cTurnGen == 0)
                                    goto LExit;
                                ini.cTurnGen--;
                                continue;
                            }
                        } else {
                            if (ini.fTry == 0)
                                break;
                            if (ini.fBatch == 0 || lpchBatch >= lpchBatchMac)
                                goto LExit;
                        }
                        DestroyCurGame();
                        pch = szBase;
                        while (*lpchBatch != '\n' && lpchBatch != lpchBatchMac) {
                            *pch = *lpchBatch;
                            lpchBatch++;
                            pch++;
                        }
                        lpchBatch++;
                        pch[-1] = 0;
                        ini.fStartupFile = 1;
                    }
                }
                CommandHandler(hwnd, 0xed9);
                if (ini.fTry == 0) {
                    if (ini.fGen != 0)
                        goto LNop;
                    if (game.lid == 0)
                        goto LShowStartup;
                    if (idPlayer != -1 && (ini.fDumpPlanets != 0 || ini.fDumpFleets != 0 || ini.fDumpMap != 0)) {
                        if (ini.fDumpMap != 0) {
                            PostMessage(hwndFrame, WM_COMMAND, IDM_DEBUG_DUMP_UNIVERSE, 0);
                        }
                        if (ini.fDumpPlanets != 0) {
                            PostMessage(hwndFrame, WM_COMMAND, IDM_DEBUG_DUMP_PLANETS, 0);
                        }
                        if (ini.fDumpFleets != 0) {
                            PostMessage(hwndFrame, WM_COMMAND, IDM_DEBUG_DUMP_FLEETS, 0);
                        }
                    } else {
                        ShowWindow(hwndFrame, SW_SHOW);
                        InitializeMenu(NULL);
                        PostMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
                        if (ini.fWait == 0)
                            goto LNop;
                        ini.fWait = 0;
                        CommandHandler(hwnd, 0x6a);
                        goto LNop;
                    }
                }
            }
        LExit:
            if (gd.fExitWindows != 0) {
                ExitWindows(vretExitValue, 0);
                return 0;
            }
            PostQuitMessage(vretExitValue);
            return 0;
        }
    LShowStartup:
        if (hwndTitle == 0) {
            pt.x = GetSystemMetrics(SM_CXSCREEN);
            pt.y = GetSystemMetrics(SM_CYSCREEN);
            hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
            fFreeingTitle = 0;
        }
        ini.fStartupFile = 0;
        DestroyCurGame();
    LNop:
        if (vSerialNumber != 0 && memcmp(vrgbMachineConfig, vrgbEnvCur, 11) == 0) {
            return 0;
        }
        szWork[200] = LOBYTE(vSerialNumber == 0 ? 0 : 1);
        lpProc = MakeProcInstance(MsgDlg, hInst);
        fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SERIAL_NUMBER), hwndTitle == 0 ? hwndFrame : hwndTitle, lpProc);
        FreeProcInstance(lpProc);
        if (fRet == 0) {
            vSerialNumber = 0;
            memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
            PostQuitMessage(vretExitValue);
        } else if (FValidSerialNo(szWork, &lSerial) != 0) {
            vSerialNumber = lSerial;
            memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
        } else if (vSerialNumber == 0) {
            memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
            PostQuitMessage(vretExitValue);
        }
        WriteIniSettings();
        return 0;
    case WM_ENTERIDLE:
        if (gd.fTutorial != 0 && tutor.fChange != 0) {
            AdvanceTutor();
        }
        return 0;
    case WM_SYSCOLORCHANGE:
    case WM_WININICHANGE:
        FGetSystemColors();
        return 0;
    case WM_GETMINMAXINFO:
        ((MINMAXINFO *)lParam)->ptMinTrackSize.x = 520;
        ((MINMAXINFO *)lParam)->ptMinTrackSize.y = 380;
        return 0;
    case WM_SIZE:
        if (wParam != 2 && wParam != 0)
            goto Default;
        vfs.dx = LOWORD(lParam);
        vfs.dy = HIWORD(lParam);
        RefitFrameChildren();
        return 0;
    case WM_COMMAND:
        CommandHandler(hwnd, wParam);
        return 0;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_STARS_HOST:
        BringUpHostDlg();
        return 1;
    case WM_STARS_CONTINUE:
        ShowTutor(0);
        game.fDirty = 0;
        DestroyCurGame();
        fErrSav = fFileErrSilent;
        fFileErrSilent = 1;
        gd.fDontDoLogFiles = 1;
        if (FLoadGame(szBase, "m1") == 0) {
            fFileErrSilent = fErrSav;
            gd.fDontDoLogFiles = 0;
            return 0;
        }
        gd.fDontDoLogFiles = 0;
        fFileErrSilent = fErrSav;
        idPlayer = 0;
        if (wParam == 2506) {
            gd.fGeneratingTurn = 1;
            _wsprintf(szWork, "%s.x1", szBase);
            if (FLoadLogFile(szWork) != 0) {
                FRunLogFile();
            }
            gd.fGeneratingTurn = 0;
        }
        CreateChildWindows();
        SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
        if (wParam == 2506) {
            SendMessage(hwndMessage, WM_KEYDOWN, VK_END, 0);
        }
        tutor.idt = idtWelcomeStarsTutorialWillGuideThrough36;
        tutor.fTurnDone = 0;
        tutor.fAutoComplete = wParam == 2506 ? 1 : 0;
        AdvanceTutor();
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) != 0xf030 && (wParam & 0xfff0) != 0xf120)
            goto Default;
        idCur = idPlayer;
        uTimerIdOld = uTimerId;
        if (uTimerId != 0) {
            KillTimer(NULL, uTimerId);
            uTimerId = 0;
            CreateChildWindows();
        }
        if (idPlayer != -1 && FNewTurnAvail(idPlayer) != 0) {
            if (uTimerIdOld != 0) {
                AlertSz(PszFormatIds(idsNewTurnAvailable, NULL), MB_ICONASTERISK);
                id = 6;
            } else {
                id = AlertSz(PszFormatIds(idsNewTurnAvailableWouldLikeLoad, NULL), MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
            }
            if (id == 6) {
                _wsprintf(szExt, MPCTD, idPlayer + 1);
                DestroyCurGame();
                if (FLoadGame(szBase, szExt) == 0) {
                    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
                } else {
                    CreateChildWindows();
                }
            } else if (id == 2) {
                if (uTimerIdOld != 0) {
                    PostMessage(hwndFrame, WM_COMMAND, IDM_TURN_WAIT_NEW, 0);
                } else {
                    PostMessage(hwndFrame, WM_SYSCOMMAND, 0xf020, 0);
                }
                return 1;
            }
            SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
            goto Default;
        }
        if (uTimerIdOld == 0)
            goto Default;
        if (uTimerType == hostTimerAutoGen) {
            PostMessage(hwnd, WM_STARS_HOST, 0, 0);
            goto Default;
        }
        if (uTimerType == hostTimerWaitTurn) {
            id = AlertSz(PszFormatIds(idsTurnHasSubmittedChangesMadeAfterTurn, NULL), MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
            if (id == 6 && FMarkFile(dtLog, idPlayer, mdMarkDone, 0) == 0) {
                AlertSz(PszFormatIds(idsNewTurnCurrentlyGeneratedHostNewTurn, NULL), MB_ICONHAND);
                _wsprintf(szExt, MPCTD, idPlayer + 1);
                DestroyCurGame();
                if (FLoadGame(szBase, szExt) == 0) {
                    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
                } else {
                    CreateChildWindows();
                }
            } else if (id == 2) {
                PostMessage(hwndFrame, WM_COMMAND, IDM_TURN_WAIT_NEW, 0);
                return 1;
            }
        }
        SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
        if (sel.pt.x <= 1000 || sel.pt.y <= 1000)
            goto Default;
        CtrPointScan(sel.pt, 1);
        goto Default;
    case WM_CHAR:
        if (hwndScanner != 0 && (wParam == '-' || wParam == '+')) {
            SendMessage(hwndScanner, WM_CHAR, wParam, lParam);
            return 0;
        }
        if (hwndMessage != 0) {
            switch (wParam) {
            case '-':
            case '+':
            case '\r':
                SendMessage(hwndMessage, WM_CHAR, wParam, lParam);
                break;
            default:
                goto L_1453;
            }
            return 0;
        }
    L_1453:
        if (hwndPlanet != 0 && (wParam == 'f' || wParam == 'F')) {
            SendMessage(hwndPlanet, WM_CHAR, wParam, lParam);
            return 0;
        }
        if (hwndPlanet != 0 && sel.grobj == grobjPlanet && (wParam == 'q' || wParam == 'Q')) {
            ChangeProduction(0);
            return 0;
        }
        if ((sel.grobj & (grobjPlanet | grobjFleet)) == 0) {
            return 0;
        }
        iOffset = 0;
        idPlanet = 0;
        switch (wParam) {
        case 'n':
            iOffset = 1;
            break;
        case 'N':
        case 'P':
            if (sel.grobj == grobjPlanet) {
                idPlanet = IdFindAdjStarbase(sel.pl.id, wParam == 'N' ? 1 : 0);
                break;
            }
            iOffset = wParam == 'N' ? 1 : -1;
            break;
        case 'p':
            iOffset = -1;
            break;
        case 'r':
        case 'R':
            if (sel.grobj == grobjFleet) {
                ShipCommandProc(hwndPlanet, 0, (LPARAM)rghwndBtn[6]);
            }
        }
        if (iOffset == 0 && idPlanet == 0) {
            return 0;
        }
        if (sel.grobj == grobjFleet) {
            SelectAdjFleet(iOffset, idPlanet);
            return 0;
        }
        SelectAdjPlanet(iOffset, idPlanet);
        return 0;
    case WM_QUERYDRAGICON:
        if (idPlayer == -1) {
            return (LRESULT)hiconHost;
        }
        if (uTimerId == 0) {
            return (LRESULT)hiconStars;
        }
        return (LRESULT)hiconWait;
    case WM_ERASEBKGND:
        if (IsIconic(hwnd) != 0) {
            GetClientRect(hwnd, &rc);
            FillRect((HDC)wParam, &rc, hbrDesktop);
            return 0;
        }
        GetClientRect(hwnd, &rc);
        if (hwndScanner != 0) {
            GetClientRect(hwndScanner, &rc2);
            MapWindowPoints(hwndScanner, hwnd, (POINT *)&rc2, 2);
            ExcludeClipRect((HDC)wParam, rc2.left, rc2.top, rc2.right, rc2.bottom);
        }
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        if (IsIconic(hwnd) != 0) {
            hdc = BeginPaint(hwnd, &ps);
            DrawIcon(hdc, 2, 2, idPlayer == -1 && game.lid != 0 ? hiconHost : uTimerId == 0 ? hiconStars : hiconWait);
            EndPaint(hwnd, &ps);
            return 0;
        }
        hdc = BeginPaint(hwnd, &ps);
        hbrSav = SelectObject(hdc, hbrButtonShadow);
        switch (iWindowLayout) {
        case layoutLarge:
        default:
            PatBlt(hdc, vfs.xTop + 5, 0, 2, vfs.dy, PATCOPY);
            PatBlt(hdc, 0, vfs.y1 + 5, vfs.xTop + 2, 2, PATCOPY);
            PatBlt(hdc, 0, vfs.y2 + 5, vfs.xTop + 2, 2, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, vfs.xTop + 1, 0, 1, vfs.y1 + 2, PATCOPY);
            PatBlt(hdc, 0, vfs.y1 + 1, vfs.xTop + 1, 1, PATCOPY);
            PatBlt(hdc, vfs.xTop + 1, vfs.y1 + 6, 1, vfs.y2 - vfs.y1 - 4, PATCOPY);
            PatBlt(hdc, 0, vfs.y2 + 1, vfs.xTop + 1, 1, PATCOPY);
            PatBlt(hdc, vfs.xTop + 1, vfs.y2 + 6, 1, vfs.dy - vfs.y2 - 6, PATCOPY);
            break;
        case layoutMedium:
        case layoutSmall:
            yOffset = gd.fToolbar == 0 ? 0 : 36;
            PatBlt(hdc, vfs.xTop + 5, yOffset, 2, vfs.y2 + 2 - yOffset, PATCOPY);
            PatBlt(hdc, 0, vfs.y1 + 5, vfs.xTop + 2, 2, PATCOPY);
            PatBlt(hdc, vfs.xTop + 5, vfs.y2 + 5, vfs.dx - vfs.xTop - 5, 2, PATCOPY);
            PatBlt(hdc, vfs.xTop + 5, vfs.y2 + 6, 2, vfs.dy - vfs.y2 - 5, PATCOPY);
            SelectObject(hdc, hbrButtonHilite);
            PatBlt(hdc, vfs.xTop + 1, yOffset, 1, vfs.y1 + 2 - yOffset, PATCOPY);
            PatBlt(hdc, 0, vfs.y1 + 1, vfs.xTop + 1, 1, PATCOPY);
            PatBlt(hdc, vfs.xTop + 1, vfs.y1 + 6, 1, vfs.dy - vfs.y1 - 6, PATCOPY);
            PatBlt(hdc, vfs.xTop + 6, vfs.y2 + 1, vfs.dx - vfs.xTop - 6, 1, PATCOPY);
        }
        SelectObject(hdc, hbrSav);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_SETCURSOR:
        hcs = 0;
        if (IsIconic(hwnd) != 0)
            goto Default;
        GetCursorPos16(&pt);
        ScreenToClient16(hwndFrame, &pt);
        GetClientRect(hwnd, &rc);
        if (PtInRect(&rc, PointFrom16(pt)) == 0)
            goto Default;
        hcs = HcrsFromFrameWindowPt(pt, NULL);
        if (hcs == 0)
            goto Default;
        SetCursor(hcs);
        return 1;
    case WM_LBUTTONDOWN:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (HcrsFromFrameWindowPt(pt, (int16_t *)&grSel) == 0) {
            return 0;
        }
        hdc = GetDC(hwnd);
        hbrSav = SelectObject(hdc, hbr50Screen);
        ptD.y = 0;
        ptD.x = 0;
        InvertPaneBorder(hdc, grSel, ptD, NULL);
        ptStart = pt;
        SetCapture(hwnd);
        ptOld = pt;
        ptAct.y = 0;
        ptAct.x = 0;
        while (FGetMouseMove(&pt) != 0) {
            if (pt.x != ptOld.x || pt.y != ptOld.y) {
                ptD.x = pt.x - ptStart.x;
                ptD.y = pt.y - ptStart.y;
                ptChg.x = pt.x - ptOld.x;
                ptChg.y = pt.y - ptOld.y;
                ptAct = InvertPaneBorder(hdc, grSel, ptD, &ptChg);
                ptOld = pt;
            }
        }
        InvertPaneBorder(hdc, grSel, ptD, NULL);
        ReleaseCapture();
        SelectObject(hdc, hbrSav);
        ReleaseDC(hwnd, hdc);
        if (ptAct.x == 0 && ptAct.y == 0) {
            return 0;
        }
        if ((grSel & splitVertical) != 0) {
            if (iWindowLayout == layoutLarge) {
                vfs.dxPlanWant = vfs.xTop + ptAct.x;
            } else {
                vfs.dx2PlanWant = vfs.xTop + ptAct.x;
            }
        }
        if ((grSel & splitMessages) != 0) {
            if (iWindowLayout == layoutLarge) {
                vfs.dyMsgWant = vfs.y2 - vfs.y1 - 8 - ptAct.y;
            } else {
                vfs.dy2MsgWant = vfs.dy - vfs.y1 - 8 - ptAct.y;
            }
        }
        if ((grSel & splitLower) != 0) {
            if (iWindowLayout == layoutLarge) {
                vfs.dyMsgWant = vfs.y2 - vfs.y1 - 8 + ptAct.y;
                vfs.dyMinWant = vfs.dy - vfs.y2 - 8 - ptAct.y;
            } else {
                vfs.dy2MinWant = vfs.dy - vfs.y2 - 8 - ptAct.y;
            }
        }
        InvalidateRect(hwnd, NULL, 1);
        RefitFrameChildren();
        return 0;
    case WM_DESTROY:
        if (uTimerId != 0) {
            KillTimer(NULL, uTimerId);
        }
        WriteIniSettings();
        uTimerId = 0;
        if (gd.fHostMode != 0) {
            FMarkFile(dtHost, -1, mdMarkInUse, 0);
        }
        DestroyCurGame();
        if (gd.fExitWindows != 0) {
            ExitWindows(vretExitValue, 0);
            return 0;
        }
        PostQuitMessage(vretExitValue);
        return 0;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    case WM_ACTIVATE:
        return 0;
    }
}

POINT16 InvertPaneBorder(HDC hdc, PaneSplitter grSel, POINT16 dpt, POINT16 *pdptPrev) {
    int16_t notMin;
    int16_t dChg;
    POINT16 dptT;
    POINT16 dptPrev;
    int16_t dyAboveMinCur;
    POINT16 dptOld;
    int16_t dyMsgCur;
    int16_t dyMinAboveH2;
    int16_t dyPlanMin;
    int16_t dxScanMin;
    int16_t x;
    POINT16 pt;
    int16_t dyMin;

    dptOld = dpt;
    if (pdptPrev != 0 && grSel != splitVertical) {
        pt.x = dpt.x - pdptPrev->x;
        pt.y = dpt.y - pdptPrev->y;
        InvertPaneBorder(hdc, grSel, pt, NULL);
    }
    switch (iWindowLayout) {
    case layoutLarge:
    default:
        dxScanMin = 100;
        dyPlanMin = 50;
        dyMsgCur = vfs.y2 - vfs.y1 - 8;
        dyAboveMinCur = vfs.y2 - vfs.y1 - 8;
        notMin = 1;
        dyMinAboveH2 = (0xd * dyArial8 >> 1) + 0xa;
        break;
    case layoutMedium:
    case layoutSmall:
        dxScanMin = 200;
        dyPlanMin = 100;
        dyMsgCur = vfs.dy - vfs.y1 - 8;
        dyAboveMinCur = vfs.y2;
        notMin = -1;
        dyMinAboveH2 = 100;
    }
    if (vfs.xTop + dpt.x < 198) {
        dpt.x = 198 - vfs.xTop;
    }
    if (vfs.dx - vfs.xTop - dpt.x < dxScanMin) {
        dpt.x = vfs.dx - vfs.xTop - dxScanMin;
    }
    if (vfs.xTop + dpt.x > 396) {
        dpt.x = 396 - vfs.xTop;
    }
    if ((grSel & splitMessages) != 0) {
        if (vfs.y1 + dpt.y < dyPlanMin) {
            dpt.y = dyPlanMin - vfs.y1;
        } else {
            dyMin = (0xd * dyArial8 >> 1) + 0xa;
            if (dyMsgCur - dpt.y < dyMin) {
                dpt.y = dyMsgCur - dyMin;
            }
        }
    }
    if ((grSel & splitLower) != 0) {
        if (dyAboveMinCur + dpt.y < dyMinAboveH2) {
            dpt.y = (dyMinAboveH2 - dyAboveMinCur) * notMin;
        } else {
            dyMin = 13 * dyArial8 - 36;
            if (vfs.dy - vfs.y2 - 8 - dpt.y < dyMin) {
                dpt.y = vfs.dy - vfs.y2 - 8 - dyMin;
            }
        }
    }
    if (pdptPrev != 0) {
        dptT = dpt;
        dpt.x = dptOld.x - pdptPrev->x;
        dpt.y = dptOld.y - pdptPrev->y;
        if (vfs.xTop + dpt.x < 198) {
            dpt.x = 198 - vfs.xTop;
        }
        if (vfs.dx - vfs.xTop - dpt.x < dxScanMin) {
            dpt.x = vfs.dx - vfs.xTop - dxScanMin;
        }
        if (vfs.xTop + dpt.x > 396) {
            dpt.x = 396 - vfs.xTop;
        }
        if ((grSel & splitMessages) != 0) {
            if (vfs.y1 + dpt.y < 50) {
                dpt.y = 50 - vfs.y1;
            } else {
                dyMin = (0xd * dyArial8 >> 1) + 0xa;
                if (dyMsgCur - dpt.y < dyMin) {
                    dpt.y = dyMsgCur - dyMin;
                }
            }
        } else if ((grSel & splitLower) != 0) {
            if (dyAboveMinCur + dpt.y < dyMinAboveH2) {
                dpt.y = (dyMinAboveH2 - dyAboveMinCur) * notMin;
            } else {
                dyMin = 13 * dyArial8 - 36;
                if (vfs.dy - vfs.y2 - 8 - dpt.y < dyMin) {
                    dpt.y = vfs.dy - vfs.y2 - 8 - dyMin;
                }
            }
        }
        dptPrev.x = dptT.x - dpt.x;
        dptPrev.y = dptT.y - dpt.y;
        dpt = dptT;
    }
    if ((uint16_t)(grSel - 1) <= 6) {
        switch (grSel) {
        case splitVertical:
        case 6:
            goto L_2164;
        case splitMessages:
            dpt.x = 0;
            PatBlt(hdc, 0, vfs.y1 + dpt.y + 1, vfs.xTop + 1, 6, PATINVERT);
            break;
        case splitLower:
            dpt.x = 0;
            if (iWindowLayout == layoutLarge) {
                PatBlt(hdc, 0, vfs.y2 + dpt.y + 1, vfs.xTop + 1, 6, PATINVERT);
                break;
            }
            PatBlt(hdc, vfs.xTop + 7, vfs.y2 + dpt.y + 1, vfs.dx - vfs.xTop - 7, 6, PATINVERT);
            break;
        case 3:
            PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
            PatBlt(hdc, 0, vfs.y1 + dpt.y + 1, vfs.xTop + dpt.x + 1, 6, PATINVERT);
            break;
        case 5:
            PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
            if (iWindowLayout == layoutLarge) {
                PatBlt(hdc, 0, vfs.y2 + dpt.y + 1, vfs.xTop + dpt.x + 1, 6, PATINVERT);
                break;
            }
            PatBlt(hdc, vfs.xTop + dpt.x + 7, vfs.y2 + dpt.y + 1, vfs.dx - dpt.x - vfs.xTop - 7, 6, PATINVERT);
            break;
        case 7:
            PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
            PatBlt(hdc, 0, vfs.y1 + dpt.y + 1, vfs.xTop + dpt.x + 1, 6, PATINVERT);
            PatBlt(hdc, vfs.xTop + dpt.x + 7, vfs.y2 + dpt.y + 1, vfs.dx - dpt.x - vfs.xTop - 7, 6, PATINVERT);
        }
        return dpt;
    }
L_2164:
    dpt.y = 0;
    if (pdptPrev == 0 || abs(dptPrev.x) >= 6) {
        if (pdptPrev != 0) {
            PatBlt(hdc, vfs.xTop + dpt.x - dptPrev.x + 1, 0, 6, vfs.dy, PATINVERT);
        }
        PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
    } else {
        dChg = dptPrev.x;
        if (dChg != 0) {
            if (dChg < 0) {
                x = vfs.xTop + dpt.x + 1;
                dChg = -dChg;
            } else {
                x = vfs.xTop + dpt.x - dptPrev.x + 1;
            }
            PatBlt(hdc, x, 0, dChg, vfs.dy, PATINVERT);
            PatBlt(hdc, x + 6, 0, dChg, vfs.dy, PATINVERT);
        }
    }
    return dpt;
}

HCURSOR HcrsFromFrameWindowPt(POINT16 pt, int16_t *pgrSel) {
    HCURSOR hcs;
    int16_t fInHBar2;
    int16_t fInHBar1;
    int16_t fInVBar;

    hcs = 0;
    fInVBar = pt.x >= vfs.xTop && pt.x < vfs.xTop + 8;
    fInHBar1 = pt.x < vfs.xTop + 8 && pt.y >= vfs.y1 && pt.y < vfs.y1 + 8;
    switch (iWindowLayout) {
    case layoutLarge:
    default:
        if (pt.x < vfs.xTop + 8 && pt.y >= vfs.y2 && pt.y < vfs.y2 + 8) {
            fInHBar2 = 1;
            break;
        }
        fInHBar2 = 0;
        break;
    case layoutMedium:
    case layoutSmall:
        fInHBar2 = pt.x >= vfs.xTop && pt.y >= vfs.y2 && pt.y < vfs.y2 + 8;
    }
    if (fInVBar == 0) {
        if (fInHBar1 != 0 || fInHBar2 != 0) {
            hcs = hcurResizeNS;
        }
    } else if (fInHBar1 != 0 || fInHBar2 != 0) {
        hcs = hcurResize4Way;
    } else {
        hcs = hcurResizeWE;
    }
    if (pgrSel != 0) {
        *pgrSel = fInHBar1 * 2 + fInVBar + fInHBar2 * 4;
    }
    return hcs;
}

void RestoreSelection() {
    PLANET *lppl;

    if (ini.idPlayer == idPlayer && ini.lid == game.lid) {
        if (ini.grobjSel == 2) {
            if (LpflFromId(ini.iObjSel) != 0) {
                SelectAdjFleet(0, ini.iObjSel);
                ini.grobjSel = 0;
            } else {
                ini.grobjSel = 1;
                ini.iObjSel = rgplr[idPlayer].idPlanetHome;
            }
        } else if (ini.grobjSel == 1) {
            lppl = LpplFromId(ini.iObjSel);
            if (lppl == 0 || lppl->iPlayer != idPlayer) {
                ini.iObjSel = rgplr[idPlayer].idPlanetHome;
            } else {
                SelectAdjPlanet(0, ini.iObjSel);
                ini.grobjSel = 0;
            }
        }
        if (ini.turn == game.turn && ini.iMsg > 0) {
            iMsgCur = ini.iMsg - 1;
            if (iMsgCur >= cMsg + vcmsgplrIn) {
                iMsgCur = -1;
            }
            iMsgCur = IMsgNext(0);
            gd.fGotoVCR = 0;
            SetMsgTitle(hwndMessage);
            InvalidateRect(hwndMessage, NULL, 1);
            if (gd.fTutorial != 0) {
                tutor.fChange = 1;
                AdvanceTutor();
            }
        }
    } else {
        ini.grobjSel = 1;
        ini.iObjSel = rgplr[idPlayer].idPlanetHome;
    }
    if (ini.grobjSel != 0) {
        if (ini.iObjSel != -1) {
            lppl = LpplFromId(ini.iObjSel);
            if (lppl == 0 || lppl->iPlayer != idPlayer) {
                FFindSomethingAndSelectIt();
            } else {
                SelectAdjPlanet(0, ini.iObjSel);
            }
        } else {
            FFindSomethingAndSelectIt();
        }
        ini.grobjSel = 0;
    }
    return;
}

void FormatSerialAndEnv(int32_t lSerial, uint8_t *pbEnv, char *pszOut) {
    uint8_t rgbRaw[21];
    int16_t j;
    uint8_t bXor;
    int16_t i;
    int16_t cBits;
    int16_t iRaw;
    int16_t iPass;
    int32_t lTank;
    uint8_t rgbRaw2[21];
    uint8_t b64;
    int16_t t_2945;
    int16_t t_29a3;

    iPass = 0;
    PushRandom(11, 17);
    Randomize(lSerial);
    RawStore32(rgbRaw, lSerial);
    memcpy(&rgbRaw[4], pbEnv, 11);
    iRaw = 15;
    for (i = 0; i < 11; i++) {
        for (j = pbEnv[i]; j > 0; j--) {
            Random(16);
        }
        if (iPass == 0) {
            rgbRaw[iRaw] = LOBYTE(Random(16));
        } else {
            t_2945 = iRaw;
            iRaw++;
            rgbRaw[t_2945] |= LOBYTE(Random(16) << 4 & 0xff);
        }
        iPass = iPass + 1 & 1;
    }
    bXor = 0;
    for (i = 0; i < 15; i++) {
        bXor ^= LOBYTE(rgbRaw[i]);
    }
    t_29a3 = iRaw;
    iRaw++;
    rgbRaw[t_29a3] |= LOBYTE(bXor << 4);
    PopRandom();
    for (i = 0; i < 21; i++) {
        rgbRaw2[i] = rgbRaw[vrgbShuffleSerial[i]];
    }
    iRaw = 0;
    cBits = 0;
    lTank = 0;
    for (i = 0; i < 28; i++) {
        if (cBits < 6) {
            lTank |= (int16_t)(rgbRaw2[iRaw++] << cBits);
            cBits += 8;
        }
        b64 = LOBYTE(LOWORD(lTank) & 0x3f);
        lTank = (int32_t)(lTank >> 6);
        cBits -= 6;
        if (b64 < 26) {
            *pszOut = LOBYTE(b64 + 65);
        } else if (b64 < 52) {
            *pszOut = LOBYTE(b64 + 71);
        } else if (b64 < 62) {
            *pszOut = LOBYTE(b64 - 4);
        } else if (b64 == 62) {
            *pszOut = '-';
        } else {
            *pszOut = '*';
        }
        pszOut++;
    }
    *pszOut = 0;
    return;
}

int16_t FSerialAndEnvFromSz(int32_t *plSerial, uint8_t *pbEnv, char *pszIn) {
    uint8_t  rgbRaw[21];
    int16_t  fSuccess;
    int16_t  j;
    uint8_t  bXor;
    int16_t  i;
    int16_t  cBits;
    int16_t  iRaw;
    int16_t  iPass;
    int32_t  lSerial;
    int32_t  lTank;
    uint8_t  rgbRaw2[21];
    uint8_t  b64;
    uint16_t t_scratch_m48_2;
    uint16_t t_scratch_m48_3;

    iPass = 0;
    *plSerial = 0;
    memset(pbEnv, 0, 11);
    iRaw = 0;
    cBits = 0;
    lTank = 0;
    for (i = 0; i < 21; i++) {
        while (cBits < 8) {
            if (*pszIn >= 'A' && *pszIn <= 'Z') {
                b64 = LOBYTE(*pszIn - 65);
            } else if (*pszIn >= 'a' && *pszIn <= 'z') {
                b64 = LOBYTE(*pszIn - 71);
            } else if (*pszIn >= '0' && *pszIn <= '9') {
                b64 = LOBYTE(*pszIn + 4);
            } else if (*pszIn == '-') {
                b64 = 62;
            } else {
                b64 = 63;
            }
            lTank |= (int16_t)(b64 << cBits);
            cBits += 6;
            pszIn++;
        }
        rgbRaw2[iRaw++] = LOBYTE(LOWORD(lTank) & 0xff);
        cBits -= 8;
        lTank = (int32_t)(lTank >> 8);
    }
    for (i = 0; i < 21; i++) {
        rgbRaw[vrgbShuffleSerial[i]] = LOBYTE((int16_t)(((uint16_t)i & 0xff00) | ((uint16_t)rgbRaw2[i] & 0xff)));
    }
    lSerial = RawLoad32(rgbRaw);
    if (FValidSerialLong(lSerial) == 0) {
        return 0;
    }
    fSuccess = 1;
    PushRandom(11, 17);
    Randomize(lSerial);
    iRaw = 15;
    for (i = 0; i < 11; i++) {
        for (j = rgbRaw[i + 4]; j > 0; j--) {
            Random(16);
        }
        if (iPass == 0) {
            t_scratch_m48_2 = rgbRaw[iRaw] & 0xf;
            if (t_scratch_m48_2 != (Random(16) & 0xff)) {
                fSuccess = 0;
            }
        } else {
            t_scratch_m48_3 = rgbRaw[iRaw] >> 4;
            if (t_scratch_m48_3 != (Random(16) & 0xff)) {
                fSuccess = 0;
            }
            iRaw++;
        }
        iPass = iPass + 1 & 1;
    }
    bXor = 0;
    for (i = 0; i < 15; i++) {
        bXor ^= LOBYTE(rgbRaw[i]);
    }
    if (rgbRaw[iRaw] >> 4 != (bXor & 0xf)) {
        fSuccess = 0;
    }
    PopRandom();
    if (fSuccess != 0) {
        *plSerial = lSerial;
        memcpy(pbEnv, &rgbRaw[4], 11);
    }
    return fSuccess;
}

int16_t FFindSomethingAndSelectIt() {
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    FLEET  *lpfl;

    lppl = LpplFromId(rgplr[idPlayer].idPlanetHome);
    if (lppl == 0 || lppl->iPlayer != idPlayer) {
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac && lppl->iPlayer != idPlayer; lppl++) {
        }
        if (lppl == lpplMac) {
            lppl = NULL;
        }
    }
    if (lppl != 0) {
        SelectAdjPlanet(0, lppl->id);
        return 1;
    }
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            SelectAdjFleet(0, lpfl->id);
            return 1;
        }
    }
    return 0;
}

void CommandHandler(HWND hwnd, WPARAM wParam) {
    POINT16      pt;
    HMENU        hmenu;
    FARPROC      lpProc;
    int16_t      dy;
    char         szExt[4];
    int16_t      dx;
    int16_t      fRet;
    RECT         rc;
    int16_t      iplrOld;
    char        *psz;
    char         szT[256];
    int16_t      cPageX;
    PLANET      *lpplMac;
    PLANET      *lppl;
    int16_t      i;
    PRINTDLG     pd;
    int16_t      cPageY;
    int16_t      xPage;
    int16_t      dxMax;
    int16_t      dxDPI;
    int16_t      dyPrintTiny;
    int16_t      dMargin;
    int16_t      y;
    int32_t      ldx;
    HFONT        hfontPrintTiny;
    HFONT        hfontPrint;
    POINT16      ptLegendB;
    POINT16      ptLegendA;
    int16_t      dyPrint;
    int16_t      dyMax;
    HFONT        hfontSav;
    int16_t      dSize;
    int16_t      yPage;
    int16_t      cch;
    int32_t      ldy;
    int16_t      dyDPI;
    int16_t      yOff;
    int16_t      xOff;
    int32_t      x;
    int16_t      idCur;
    HCURSOR      hcurSav;
    tagTIMERINFO ti;
    uint32_t     dwTickCur;
    uint32_t     dwTickBase;
    int16_t      mf;
    StringId     ids;
    int16_t      cObj;
    int16_t      ifl;
    FLEET       *lpfl;
    int16_t      id;

    if (GET_WM_COMMAND_ID(wParam, 0) >= IDM_POPUP_BASE && GET_WM_COMMAND_ID(wParam, 0) < 15100) {
        iPopMenuSel = GET_WM_COMMAND_ID(wParam, 0) - 15000;
    } else {
        switch (GET_WM_COMMAND_ID(wParam, 0)) {
        case IDM_HELP_ABOUT:
            lpProc = MakeProcInstance(About, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_FILE_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case IDM_VIEW_LAYOUT_0:
        case IDM_VIEW_LAYOUT_1:
        case IDM_VIEW_LAYOUT_2:
            if (idPlayer == -1)
                break;
            iWindowLayout = GET_WM_COMMAND_ID(wParam, 0) - 130;
            InvalidateRect(hwndFrame, NULL, 1);
            EnsureTileSize(iWindowLayout == layoutSmall ? 1 : 0);
            RefitFrameChildren();
            break;
        case IDM_UNKNOWN_09C4:
        case IDM_HELP_TUTORIAL:
            if (gd.fTutorial != 0) {
                ShowTutor(1);
                break;
            }
            StartTutor(0);
            break;
        case IDM_FILE_NEW_GAME:
        case IDM_TOOL_NEW_GAME:
            if (gd.fTutorial != 0 && FAskKillTutor() == 0)
                break;
            NewGameWizard(hwnd, 0);
            break;
        case IDM_FILE_MRU1:
        case IDM_FILE_MRU2:
        case IDM_FILE_MRU3:
        case IDM_FILE_MRU4:
        case IDM_FILE_MRU5:
        case IDM_FILE_MRU6:
        case IDM_FILE_MRU7:
        case IDM_FILE_MRU8:
        case IDM_FILE_MRU9:
            if ((gd.fTutorial != 0 && FAskKillTutor() == 0) || vrgszMRU == 0 || vrgszMRU[(GET_WM_COMMAND_ID(wParam, 0) - 4300) * 256] == 0)
                break;
            iplrOld = idPlayer;
            fstrcpy(szT, vrgszMRU + 256 * (GET_WM_COMMAND_ID(wParam, 0) - 4300));
            psz = strrchr(szT, 46);
            if (psz != 0 && access(szT, 0) != -1) {
                ini.fStartupFile = 1;
                DestroyCurGame();
                strcpy(szBase, szT);
                if (FOpenGame(hwnd, 0) <= 0)
                    break;
                InitializeMenu(NULL);
                CreateChildWindows();
                if (uTimerId == 0) {
                    PostMessage(hwnd, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
                }
                if (game.fTutorial == 0 || idPlayer != 0)
                    break;
                StartTutor(0);
                break;
            }
            strcpy(szWork, szT);
            AlertSz(PszFormatIds(idsCantOpenFile, NULL), MB_ICONHAND);
            break;
        case IDM_RACE_CREATE:
            vplr = vrgplrDef[0];
            RaceCreationWizard(hwnd, 0, 0);
            break;
        case IDM_VIEW_TOOLBAR:
            hmenu = GetASubMenu(hwnd, menuView);
            gd.fToolbar = gd.fToolbar == 0 ? 1 : 0;
            CheckMenuItem(hmenu, IDM_VIEW_TOOLBAR, gd.fToolbar == 0 ? MF_UNCHECKED : MF_CHECKED);
            RefitFrameChildren();
            break;
        case IDM_FILE_PRINT_MAP:
            if (idPlayer == -1)
                break;
            lpProc = MakeProcInstance(PrintMapDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_PRINT_MAP), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == 0)
                break;
            cPageX = vrgcPrintMapPage[0];
            cPageY = vrgcPrintMapPage[1];
            memset(&pd, 0, sizeof(PRINTDLG));
            pd.lStructSize = sizeof(PRINTDLG);
            pd.Flags = PD_RETURNDC | PD_RETURNDEFAULT;
            if (PrintDlg(&pd) == 0) {
                AlertSz(PszFormatIds(idsUnablePrintGameMapPrinterMayOff, NULL), MB_ICONHAND);
                break;
            }
            dxMax = GetDeviceCaps(pd.hDC, HORZRES);
            dyMax = GetDeviceCaps(pd.hDC, VERTRES);
            dxDPI = GetDeviceCaps(pd.hDC, LOGPIXELSX);
            dyDPI = GetDeviceCaps(pd.hDC, LOGPIXELSY);
            dMargin = 32;
            hfontPrint = HfontPrinterCreate(pd.hDC, 8, &dyPrint);
            hfontPrintTiny = HfontPrinterCreate(pd.hDC, 5, &dyPrintTiny);
            SetBkMode(pd.hDC, TRANSPARENT);
            rc.top = 0;
            rc.left = 0;
            if ((cPageX <= cPageY ? 0 : 1) == (dxMax <= dyMax ? 0 : 1)) {
                i = cPageX;
                cPageX = cPageY;
                cPageY = i;
            }
            ldx = (uint32_t)(dxMax * cPageX);
            ldy = (uint32_t)(dyMax * cPageY);
            if (ldx > 32000) {
                ldx = 32000;
            }
            if (ldy > 32000) {
                ldy = 32000;
            }
            if (ldx < ldy) {
                if (ldy - dyDPI < ldx) {
                    ldx = ldy - dyDPI;
                }
                dSize = LOWORD(ldx);
                ptLegendA.x = dMargin;
                ptLegendA.y = dSize + dMargin;
                ptLegendB.x = dSize / 2;
                ptLegendB.y = ptLegendA.y;
            } else {
                if (ldx - (int16_t)((int16_t)(3 * dxDPI) / 2) < ldy) {
                    ldy = ldx - (int16_t)((int16_t)(3 * dxDPI) / 2);
                }
                dSize = LOWORD(ldy);
                ptLegendA.x = dSize + dMargin;
                ptLegendA.y = dMargin;
                ptLegendB.x = ptLegendA.x;
                ptLegendB.y = dSize / 2;
            }
            rc.bottom = dSize;
            rc.right = dSize;
            dSize -= dMargin * 2;
            for (xPage = 0; xPage < cPageX; xPage++) {
                for (yPage = 0; yPage < cPageY; yPage++) {
                    xOff = -dxMax * xPage;
                    yOff = -dyMax * yPage;
                    cch = CchGetString(idsStarsUniverseMap, szWork);
                    Escape(pd.hDC, 10, cch, szWork, NULL);
                    Rectangle(pd.hDC, xOff, yOff, xOff + rc.right, yOff + rc.bottom);
                    if (hfontPrint != 0) {
                        hfontSav = SelectObject(pd.hDC, hfontPrint);
                    } else {
                        hfontSav = 0;
                    }
                    y = ptLegendA.y + yOff;
                    cch = CchGetString(idsStarsUniverseMap, szWork);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, szWork, cch);
                    y += dyPrint;
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, game.szName, strlen(game.szName));
                    y += dyPrint;
                    psz = PszPlayerName(idPlayer, 1, 1, 1, 0, NULL);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, psz, strlen(psz));
                    y += dyPrint;
                    cch = _wsprintf(szWork, PszGetCompressedString(idsYearD), game.turn + 2400);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, szWork, cch);
                    if ((grbitScan & grbitScanViewMask) != 5) {
                        y = ptLegendB.y + yOff;
                        for (i = 0; i < 5; i++) {
                            cch = CchGetString(i + 1314, szWork);
                            TextOut(pd.hDC, ptLegendB.x + xOff, y, szWork, cch);
                            y += dyPrint;
                        }
                        y = ptLegendB.y + yOff;
                        if (hfontPrintTiny != 0) {
                            SelectObject(pd.hDC, hfontPrintTiny);
                        }
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int16_t)(3 * dyPrint) / 2 + y - dyPrintTiny / 2, "|", 1);
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int16_t)(5 * dyPrint) / 2 + y - dyPrintTiny / 2, "+", 1);
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int16_t)(9 * dyPrint) / 2 + y + 8 - dyPrintTiny, "2", 1);
                        if (hfontPrint != 0) {
                            SelectObject(pd.hDC, hfontPrint);
                        }
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, y - 4 + dyPrint / 2, 1);
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, y - 4 + (int16_t)(7 * dyPrint) / 2, 0);
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, (int16_t)(9 * dyPrint) / 2 + y + 8, 0);
                    }
                    if ((grbitScan & grbitScanViewMask) != 5) {
                        if (hfontPrintTiny != 0) {
                            SelectObject(pd.hDC, hfontPrintTiny);
                        }
                        lppl = lpPlanets;
                        lpplMac = lpPlanets + cPlanet;
                        for (; lppl < lpplMac; lppl++) {
                            x = (int16_t)(rgptPlan[lppl->id].x - 1000);
                            y = (int16_t)(dGalInv - 1000 - rgptPlan[lppl->id].y);
                            x = (int32_t)((int32_t)(x * dSize) / dGal) + dMargin + xOff;
                            y = (int32_t)((int32_t)(y * dSize) / dGal) + dMargin + yOff;
                            if (lppl->iPlayer == idPlayer) {
                                if (lppl->fStarbase != 0) {
                                    CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) + 4 - dyPrintTiny,
                                               rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef == ihuldefOrbitalFort ? "|" : "+", 1);
                                }
                                DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 1);
                            } else if (lppl->iPlayer != -1) {
                                cch = _wsprintf(szWork, PCTD, lppl->iPlayer + 1);
                                CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) - dyPrintTiny, szWork, cch);
                            }
                        }
                    }
                    if (hfontPrint != 0) {
                        SelectObject(pd.hDC, hfontPrint);
                    }
                    for (i = 0; i < game.cPlanMax; i++) {
                        x = (int16_t)(rgptPlan[i].x - 1000);
                        y = (int16_t)(dGalInv - 1000 - rgptPlan[i].y);
                        x = (int32_t)((int32_t)(x * dSize) / dGal) + dMargin + xOff;
                        y = (int32_t)((int32_t)(y * dSize) / dGal) + dMargin + yOff;
                        DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 0);
                        if ((grbitScan & grbitScanPlanetNames) != 0) {
                            CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) + 14, PszGetPlanetName(i), 0);
                        }
                    }
                    if (hfontSav != 0) {
                        SelectObject(pd.hDC, hfontSav);
                    }
                    Escape(pd.hDC, 1, 0, NULL, NULL);
                }
            }
            Escape(pd.hDC, 11, 0, NULL, NULL);
            if (hfontPrint != 0) {
                DeleteObject(hfontPrint);
            }
            if (hfontPrintTiny != 0) {
                DeleteObject(hfontPrintTiny);
            }
            DeleteDC(pd.hDC);
            if (pd.hDevMode != 0) {
                GlobalFree(pd.hDevMode);
            }
            if (pd.hDevNames == 0)
                break;
            GlobalFree(pd.hDevNames);
            break;
        case IDM_VIEW_PLAYER_COLORS:
            hmenu = GetASubMenu(hwnd, menuView);
            grbitScan ^= grbitScanPlayerColors;
            CheckMenuItem(hmenu, IDM_VIEW_PLAYER_COLORS, (grbitScan & grbitScanPlayerColors) == 0 ? MF_UNCHECKED : MF_CHECKED);
            gd.fChgScanner = 1;
            if ((grbitScan & (grbitScanPlanetNames | grbitScanShipCounts)) == 0)
                break;
            InvalidateRect(hwndScanner, NULL, 0);
            break;
        case IDM_VIEW_FIND:
        case IDM_UNKNOWN_1069:
            if (idPlayer == -1)
                break;
            lpProc = MakeProcInstance(FindDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_GAME_SCORE:
        case IDM_GAME_SCORE2:
            if (idPlayer == -1)
                break;
            lpProc = MakeProcInstance(ScoreXDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SCORE), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_FILE_HOST_GAME:
            if (game.fSinglePlr == 0 &&
                AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES)
                break;
        case WMX_UNKNOWN_006C:
        case IDM_DEBUG_GEN_10_TURNS:
        case IDM_DEBUG_GEN_100_TURNS:
        case IDM_DEBUG_GEN_1000_TURNS:
            idCur = idPlayer;
            switch (GET_WM_COMMAND_ID(wParam, 0)) {
            case IDM_DEBUG_GEN_100_TURNS:
                iPassCnt = 100;
                break;
            case IDM_DEBUG_GEN_1000_TURNS:
                iPassCnt = 1000;
                break;
            case IDM_DEBUG_GEN_10_TURNS:
                iPassCnt = 10;
                break;
            default:
                iPassCnt = 0;
            }
            if (game.fSinglePlr != 0 && iPassCnt != 0) {
                _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), iPassCnt);
                if (MessageBox(GetFocus(), szWork, "Stars!", MB_YESNO | MB_ICONEXCLAMATION | MB_TASKMODAL) != IDYES)
                    break;
            }
            if (idPlayer == -1)
                break;
            if (gd.fTutorial != 0) {
                AdvanceTutor();
                if (tutor.fTurnDone == 0) {
                    AlertSz(PszFormatIds(idsTutorialTurnWillGeneratedHaveYetCompleted, NULL), MB_ICONHAND);
                    break;
                }
                ShowTutor(0);
                if (game.turn <= 35) {
                    Randomize(1234567890);
                    FWriteHistFile(idCur);
                    if (FWriteTutorialMFile(game.turn + 1) == 0) {
                        AlertSz(PszFormatIds(idsFileError, NULL), MB_ICONHAND);
                        break;
                    }
                    if (game.turn == 35 && FWriteTutorialMFile(game.turn + 2) == 0) {
                        AlertSz(PszFormatIds(idsFileError, NULL), MB_ICONHAND);
                        break;
                    }
                    strcpy(szWork, szBase);
                    strcat(szWork, ".x1");
                    remove(szWork);
                    DirtyGame(0);
                    ShowProgressGauge();
                    ti.dwSize = 12;
                    TimerCount(&ti);
                    dwTickBase = ti.dwmsSinceStart;
                    do {
                        UpdateProgressGauge((LOWORD(dwTickCur) - LOWORD(dwTickBase)) * 2);
                        TimerCount(&ti);
                        dwTickCur = ti.dwmsSinceStart;
                    } while (dwTickCur >= dwTickBase && dwTickCur < dwTickBase + 500);
                    goto LTutorialFinishUp;
                }
            }
            if (game.fSinglePlr == 0)
                goto LWaitForTurn;
            idsFileError = -1;
            if (FCheckFile(dtHost, -1, mdMarkInUse) != 0) {
                if (FBadFileError(idsFileError) != 0) {
                    AlertSz(PszFormatIds(idsFileError, NULL), MB_ICONHAND);
                    break;
                }
                if (AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
                    goto LWaitForTurn;
                break;
            }
            hcurSav = SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32514)));
            FWriteLogFile(szBase, idCur);
            FWriteHistFile(idCur);
            while (1) {
                ShowProgressGauge();
                EnsureAis();
                FGenerateTurn();
                switch (GET_WM_COMMAND_ID(wParam, 0)) {
                case IDM_DEBUG_GEN_10_TURNS:
                case IDM_DEBUG_GEN_100_TURNS:
                case IDM_DEBUG_GEN_1000_TURNS:
                    iPassCnt--;
                    if (iPassCnt > 0) {
                        HideProgressGauge();
                        if (GetAsyncKeyState(VK_SHIFT) >= 0 || GetAsyncKeyState(VK_CONTROL) >= 0)
                            continue;
                    }
                }
                break;
            }
            iPassCnt = 0;
        LTutorialFinishUp:
            DestroyCurGame();
            _wsprintf(szExt, MPCTD, idCur + 1);
            if (FLoadGame(szBase, szExt) == 0) {
                SetCursor(hcurSav);
                HideProgressGauge();
                AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
                break;
            }
            HideProgressGauge();
            idPlayer = idCur;
            CreateChildWindows();
            SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
            SetCursor(hcurSav);
            if (gd.fTutorial == 0)
                break;
            tutor.fTurnDone = 0;
            tutor.fAutoComplete = 0;
            AdvanceTutor();
            break;
        case IDM_VIEW_BROWSER_TOGGLE:
        case IDM_VIEW_BROWSER_TOGGLE2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            if (hwndBrowser == 0) {
                mf = 8;
                CreateDialog(hInst, MAKEINTRESOURCE(IDD_BROWSER), hwndFrame, lpfnBrowserDlgProc);
            } else {
                mf = 0;
                DestroyWindow(hwndBrowser);
            }
            hmenu = GetASubMenu(hwnd, menuHelp);
            CheckMenuItem(hmenu, IDM_VIEW_BROWSER_TOGGLE2, mf);
            break;
        case IDM_REPORT_CYCLE:
            if (hwndReportDlg == 0) {
                wParam = IDM_REPORT_PLANET;
            } else if (vprptCur == &vrptPlanet) {
                wParam = IDM_REPORT_FLEET;
            } else if (vprptCur == &vrptFleet) {
                wParam = IDM_REPORT_ENEMY_FLEET;
            } else {
                wParam = IDM_REPORT_BATTLE;
            }
        case IDM_REPORT_PLANET:
        case IDM_REPORT_FLEET:
        case IDM_REPORT_ENEMY_FLEET:
        case IDM_REPORT_BATTLE:
            cObj = 0;
            if (game.lid == 0 || idPlayer == -1)
                break;
            hmenu = GetASubMenu(hwnd, menuReport);
            while (hwndReportDlg != 0) {
                if (GET_WM_COMMAND_ID(wParam, 0) != IDM_REPORT_BATTLE || vprptCur != &vrptBattle) {
                    ids = idsPlayerLogFileAppearsCorruptUnableLoad;
                } else {
                    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
                }
                mf = 0;
                DestroyWindow(hwndReportDlg);
                CheckMenuItem(hmenu, GET_WM_COMMAND_ID(wParam, 0) == IDM_REPORT_FLEET ? IDM_REPORT_FLEET : IDM_REPORT_PLANET, mf);
                if (ids != idsPlayerLogFileAppearsCorruptUnableLoad) {
                    return;
                }
            }
            mf = 8;
            switch (GET_WM_COMMAND_ID(wParam, 0)) {
            case IDM_REPORT_FLEET:
                ids = idsFleetSummaryReportDFleetC;
                vprptCur = &vrptFleet;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0)
                        break;
                    if (lpfl->iPlayer == idPlayer) {
                        cObj++;
                    }
                }
                break;
            case IDM_REPORT_ENEMY_FLEET:
                ids = idsOthersFleetsSummaryReportDFleetC;
                vprptCur = &vrptEFleet;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0)
                        break;
                    if (lpfl->iPlayer != idPlayer) {
                        cObj++;
                    }
                }
                break;
            case IDM_REPORT_BATTLE:
                vprptCur = &vrptBattle;
                ids = idsBattleSummaryReportDBattleC;
                cObj = CBattles();
                break;
            default:
                vprptCur = &vrptPlanet;
                ids = idsPlanetSummaryReportDPlanetC;
                lppl = lpPlanets;
                lpplMac = lpPlanets + cPlanet;
                for (; lppl < lpplMac; lppl++) {
                    if (lppl->iPlayer == idPlayer) {
                        cObj++;
                    }
                }
            }
            psz = PszGetCompressedString(ids);
            _wsprintf(szWork, psz, cObj, cObj == 1 ? 32 : 115);
            hwndReportDlg = CreateWindow(szReport, szWork, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 100, 100,
                                         hwndFrame, NULL, hInst, NULL);
            SetWindowPos(hwndReportDlg, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
            CheckMenuItem(hmenu, GET_WM_COMMAND_ID(wParam, 0), mf);
            break;
        case IDM_UNKNOWN_09C1:
        case IDM_HELP_INTRO:
            WinHelp(hwnd, szHelpFile, HELP_CONTEXT, (uint32_t)(GET_WM_COMMAND_ID(wParam, 0) == IDM_HELP_INTRO ? 4501 : 13002));
            break;
        case IDM_HELP_CONTENTS:
        case IDM_HELP_CONTENTS2:
            WinHelp(hwnd, szHelpFile, HELP_INDEX, 0);
            break;
        case IDM_TURN_WAIT_NEW:
        case IDM_GAME_WAIT_FOR_TURN:
        LWaitForTurn:
            if (lpPlanets == 0 || idPlayer == -1 || game.fSinglePlr != 0)
                break;
            if (FNewTurnAvail(idPlayer) != 0)
                goto LNewTurnAvail;
            gd.fSubmit = 1;
            FWriteLogFile(szBase, idPlayer);
            FWriteHistFile(idPlayer);
            SetWindowText(hwndFrame, PszGetCompressedString(idsWaitingNewTurn));
            ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
            uTimerId = SetTimer(NULL, hostTimerWaitTurn, 10000, lpfnHostTimerProc);
            uTimerType = hostTimerWaitTurn;
            HostTimerProc(NULL, WM_NULL, uTimerId, 0);
            break;
        case IDM_TURN_SAVE_SUBMIT:
            wParam = IDM_TURN_END_B;
        case WMX_UNKNOWN_006F:
        case IDM_TURN_END_A:
        case IDM_TURN_END_B:
            if (hwndScanner == 0) {
                AlertSz(PszFormatIds(idsGameCurrentlyLoaded, NULL), MB_ICONHAND);
                break;
            }
            if (idPlayer == -1) {
                FWriteDataFile(szBase, idPlayer, 0);
                break;
            }
            if (FNewTurnAvail(idPlayer) != 0)
                goto LNewTurnAvail;
            gd.fSubmit = GET_WM_COMMAND_ID(wParam, 0) == IDM_TURN_END_B ? 1 : 0;
            FWriteLogFile(szBase, idPlayer);
            FWriteHistFile(idPlayer);
            break;
        case IDM_FILE_OPEN_GAME:
        case IDM_TOOL_OPEN_GAME:
            if ((gd.fTutorial != 0 && FAskKillTutor() == 0) || FOpenGame(hwnd, 0) <= 0)
                break;
            InitializeMenu(NULL);
            if (uTimerId == 0) {
                PostMessage(hwnd, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
            }
            if (game.fTutorial == 0 || idPlayer != 0)
                break;
            StartTutor(0);
            break;
        case IDM_TITLE_NEW_GAME:
        case IDM_TITLE_OPEN_GAME:
        case IDM_TITLE_CONTINUE:
        case IDM_TITLE_EXIT:
            if (hwndTitle == 0)
                break;
            PostMessage(hwndTitle, WM_COMMAND, GET_WM_COMMAND_ID(wParam, 0) - 250, 0);
            break;
        case IDM_FILE_RETURN_TO_TITLE:
            if (gd.fTutorial != 0 && FAskKillTutor() == 0)
                break;
            WriteIniSettings();
            DestroyCurGame();
            strcpy(szBase, szWork);
            ini.grobjSel = 0;
            ini.iObjSel = 0;
            ini.idPlayer = -1;
            InitializeMenu(NULL);
            pt.x = GetSystemMetrics(SM_CXSCREEN);
            pt.y = GetSystemMetrics(SM_CYSCREEN);
            hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
            fFreeingTitle = 0;
            ShowWindow(hwndFrame, SW_HIDE);
            break;
        case IDM_GAME_RESEARCH:
        case IDM_GAME_RESEARCH2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            lpProc = MakeProcInstance(ResearchDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RESEARCH), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == 0 || sel.grobj != grobjPlanet)
                break;
            if (sel.pl.lpplprod != 0) {
                FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, NULL);
            }
            DrawPlanShip(NULL, tilePlanetStats | tileProductionOrOrbit);
            break;
        case IDM_GAME_BATTLE_PLANS1:
        case IDM_GAME_BATTLE_PLANS2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            lpProc = MakeProcInstance(BattlePlansDlg, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_BATTLE_PLANS), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_GAME_RELATIONS:
        case IDM_GAME_RELATIONS2:
            if (game.lid == 0 || idPlayer == -1 || game.fSinglePlr != 0)
                break;
            lpProc = MakeProcInstance(RelationsDlg, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_RELATIONS), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_RACE_EDIT1:
        case IDM_RACE_EDIT2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            vplr = rgplr[idPlayer];
            RaceCreationWizard(hwnd, 1, 0);
            break;
        case IDM_VIEW_GAME_PARAMS:
        case IDM_VIEW_GAME_PARAMS2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            NewGameWizard(hwnd, 1);
            break;
        case IDM_GAME_SHIP_BUILDER:
        case IDM_GAME_SHIP_BUILDER2:
            if (game.lid == 0 || idPlayer == -1)
                break;
            pt.x = 610;
            pt.y = 450;
            if (hwndPopup != 0) {
                SendMessage(hwndPopup, WM_RBUTTONUP, 0, 0);
            }
            ShipBuilder(pt);
            break;
        case IDM_DEBUG_DUMP_UNIVERSE:
            DumpUniverse();
            break;
        case IDM_DEBUG_DUMP_PLANETS:
            DumpPlanets();
            break;
        case IDM_DEBUG_DUMP_FLEETS:
            DumpFleets();
            break;
        case IDM_CMD_CHANGE_PASSWORD:
            if (game.lid == 0 || idPlayer == -1 || ((game.fSinglePlr != 0 && lSaltCur <= 0) || FCheckPassword() == 0))
                break;
            lpProc = MakeProcInstance(NewPasswordDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_PASSWORD), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case IDM_SCAN_ZOOM_0:
        case IDM_SCAN_ZOOM_1:
        case IDM_SCAN_ZOOM_2:
        case IDM_SCAN_ZOOM_3:
        case IDM_SCAN_ZOOM_4:
        case IDM_SCAN_ZOOM_5:
        case IDM_SCAN_ZOOM_6:
        case IDM_SCAN_ZOOM_7:
        case IDM_SCAN_ZOOM_8:
            if (hwndScanner == 0) {
                AlertSz(PszFormatIds(idsCantChangeZoomFactorUntilGameOpen, NULL), MB_ICONHAND);
                break;
            }
            hmenu = GetASubMenu(hwnd, menuView);
            hmenu = GetSubMenu(hmenu, 3);
            CheckMenuItem(hmenu, iScanZoom + 4, MF_BYPOSITION);
            GetClientRect(hwndScanner, &rc);
            rc.right = ScanToPt(rc.right) >> 1;
            rc.bottom = ScanToPt(rc.bottom) >> 1;
            dx = xScanTop;
            dy = dGalInv - yScanTop;
            iScanZoom = GET_WM_COMMAND_ID(wParam, 0) - 3905;
            CheckMenuItem(hmenu, iScanZoom + 4, MF_CHECKED | MF_BYPOSITION);
            DrawMenuBar(hwnd);
            SetScanScrollBars(hwndScanner);
            InvalidateRect(hwndScanner, NULL, 1);
            if (sel.scan.grobj != grobjNone) {
                pt = sel.scan.pt;
            } else {
                pt.x = dx + rc.right;
                pt.y = dy - rc.bottom;
            }
            CtrPointScan(pt, 0);
            break;
        case IDM_FRAME_POST_OPEN:
            if (hwndScanner == 0)
                break;
            gd.fNoScannerDraw = 1;
            RestoreSelection();
            RefitFrameChildren();
            gd.fNoScannerDraw = 0;
            InvalidateRect(hwndScanner, NULL, 1);
            UpdateWindow(hwndScanner);
            break;
        case IDM_FLEET_DELETE_WAYPOINT:
        case IDM_FLEET_INSERT_WAYPOINT:
            if (sel.grobj == grobjFleet && GetFocus() != hwndOrderED) {
                DeleteCurWayPoint(GET_WM_COMMAND_ID(wParam, 0) == IDM_FLEET_DELETE_WAYPOINT ? 1 : 0);
            }
        default:
            DefWindowProc(hwnd, WM_COMMAND, wParam, 0);
        }
        return;
    LNewTurnAvail:
        FWriteHistFile(idPlayer);
        if (game.fDirty != 0) {
            id = AlertSz(PszFormatIds(idsSorryTurnHasAlreadyGeneratedAnyChanges, NULL), MB_OKCANCEL | MB_ICONEXCLAMATION);
            if (id == 2) {
                return;
            }
        } else {
            AlertSz(PszFormatIds(idsNewTurnAvailable, NULL), MB_ICONASTERISK);
        }
        _wsprintf(szExt, MPCTD, idPlayer + 1);
        game.fDirty = 0;
        DestroyCurGame();
        if (FLoadGame(szBase, szExt) == 0) {
            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
        } else {
            CreateChildWindows();
            SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
        }
    }
    return;
}

void InitializeMenu(HMENU hmenu) {
    int16_t cMenu;
    int16_t i;
    HMENU   hmenuSub;

    if (hmenu == 0) {
        hmenu = GetMenu(hwndFrame);
    }
    hmenuSub = GetASubMenu(hwndFrame, menuFile);
    for (i = 4300; i <= 4308; i++) {
        DeleteMenu(hmenuSub, i, MF_BYCOMMAND);
    }
    for (i = 0; i < 9 && vrgszMRU[i * 256] != 0; i++) {
        szWork[0] = '&';
        szWork[1] = LOBYTE(i + 49);
        szWork[2] = ' ';
        fstrcpy(&szWork[3], vrgszMRU + 256 * i);
        InsertMenu(hmenuSub, i + 9, MF_BYPOSITION, i + 4300, szWork);
    }
    EnableMenuItem(hmenu, 106, szBase[0] != 0 && game.fSinglePlr == 0 ? MF_ENABLED : MF_GRAYED | MF_DISABLED);
    EnableMenuItem(hmenu, 105, szBase[0] == 0 ? MF_GRAYED | MF_DISABLED : MF_ENABLED);
    EnableMenuItem(hmenu, 270, szBase[0] != 0 && (game.fSinglePlr == 0 || lSaltCur > 0) ? MF_ENABLED : MF_GRAYED | MF_DISABLED);
    EnableMenuItem(hmenu, 2014, szBase[0] != 0 && game.fSinglePlr == 0 ? MF_ENABLED : MF_GRAYED | MF_DISABLED);
    EnableMenuItem(hmenu, 3803, szBase[0] != 0 && game.fSinglePlr == 0 ? MF_ENABLED : MF_GRAYED | MF_DISABLED);
    hmenu = GetASubMenu(hwndFrame, menuView);
    CheckMenuItem(hmenu, IDM_VIEW_TOOLBAR, gd.fToolbar == 0 ? MF_UNCHECKED : MF_CHECKED);
    CheckMenuItem(hmenu, IDM_VIEW_PLAYER_COLORS, (grbitScan & grbitScanPlayerColors) == 0 ? MF_UNCHECKED : MF_CHECKED);
    if (hwndScanner == 0) {
        EnableMenuItem(GetMenu(hwndFrame), 1, MF_GRAYED | MF_DISABLED | MF_BYPOSITION);
    } else {
        EnableMenuItem(GetMenu(hwndFrame), 1, MF_BYPOSITION);
        hmenu = GetASubMenu(hwndFrame, menuView);
        hmenu = GetSubMenu(hmenu, 3);
        CheckMenuItem(hmenu, iScanZoom + 4, MF_CHECKED | MF_BYPOSITION);
        cMenu = GetMenuItemCount(hmenu);
        for (i = 0; i < cMenu; i++) {
            EnableMenuItem(hmenu, i, MF_BYPOSITION);
        }
        hmenu = GetASubMenu(hwndFrame, menuView);
        hmenu = GetSubMenu(hmenu, 4);
        CheckMenuItem(hmenu, iWindowLayout, MF_CHECKED | MF_BYPOSITION);
    }
    DrawMenuBar(hwndFrame);
    return;
}

void EnsureAis() {
    int16_t fHostSav;
    int16_t fErrSav;
    int16_t fOpened;
    int16_t fWorkDone;
    int16_t fSubmitSav;
    int16_t iPlayer;
    MDPLR   rgmdplr[16];

    fSubmitSav = gd.fSubmit;
    fWorkDone = 0;
    if (gd.fAisDone == 0) {
        fHostSav = gd.fHostMode;
        if (gd.fHostMode == 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            *(uint16_t *)&rgmdplr[iPlayer] = rgplr[iPlayer].wMdPlr;
        }
        gd.fSubmit = 1;
        fErrSav = fFileErrSilent;
        fFileErrSilent = 1;
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            UpdateProgressGauge(MulDiv(340, iPlayer + 1, game.cPlayer));
            if (rgmdplr[iPlayer].fAi != 0) {
                fWorkDone = 1;
                gd.fGeneratingTurn = 1;
                gd.fHostMode = 1;
                fOpened = FOpenFile(dtLog, iPlayer, 32);
                gd.fGeneratingTurn = 0;
                gd.fHostMode = fHostSav;
                if (fOpened != 0) {
                    StreamClose();
                } else {
                    DoAiTurn(iPlayer, *(uint16_t *)&rgmdplr[iPlayer]);
                }
            }
        }
        gd.fSubmit = fSubmitSav;
        if (fWorkDone != 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        fFileErrSilent = fErrSav;
        gd.fAisDone = 1;
    }
    return;
}

HMENU GetASubMenu(HWND hwnd, MainMenu iMenu) {
    int16_t fChildMenu;
    HMENU   hmenu;

    fChildMenu = hwndActive != 0 && IsZoomed(hwndActive) != 0;
    hmenu = GetMenu(hwnd);
    hmenu = GetSubMenu(hmenu, iMenu + fChildMenu);
    return hmenu;
}

int16_t FOpenGame(HWND hwnd, int16_t fRaceOnly) {
    OPENFILENAME ofn;
    uint16_t     i;
    char         szFile[256];
    char        *pch;
    char         szFileTitle[256];
    char         szFilter[256];
    int16_t      fRet;
    GrobjClass   grobjIni;

    if (ini.fStartupFile != 0) {
        strcpy(szFile, szBase);
        *strrchr(szBase, 92) = 0;
        pch = strrchr(szFile, 46);
        if (pch == 0) {
            SetSzWorkFromDt(dtHost, -1);
            strcpy(szFile, szWork);
            pch = strrchr(szFile, 46);
        }
        ofn.nFileExtension = pch - szFile + 1;
        ofn.nFileOffset = 0;
        fFileErrSilent = 1;
    } else {
        szFile[0] = 0;
        CchGetString(fRaceOnly == 0 ? idsStarsGameFilesMHstRStars : idsStarsGameFilesRFiles, szFilter);
        for (i = 0; szFilter[i] != 0; i++) {
            if (szFilter[i] == '|') {
                szFilter[i] = 0;
            }
        }
        memset(&ofn, 0, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = szFilter;
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = 0x100;
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle = 0x100;
        ofn.lpstrInitialDir = szDirName;
        ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if (GetOpenFileName(&ofn) == 0) {
            return 0;
        }
    }
    szDirName[0] = 0;
    fRet = FWasRaceFile(&szFile[ofn.nFileOffset], fRaceOnly == 0 ? 1 : 0);
    if (fRaceOnly != 0) {
        if (fRet > 0) {
            strcpy(szRaceFile, &szFile[ofn.nFileOffset]);
        }
        return fRet;
    }
    if (fRet != 0) {
        if (ini.fStartupFile != 0 && vSerialNumber == 0) {
            fRet = -1;
        }
        fFileErrSilent = 0;
        ini.fStartupFile = 0;
        if (fRet == -1) {
            return -1;
        }
        RaceCreationWizard(hwnd, 0, 0);
        return 0;
    }
    szFile[ofn.nFileExtension - 1] = 0;
    DestroyCurGame();
    strcpy(szBase, szFile);
    if (FLoadGame(szFile, &szFile[ofn.nFileExtension]) == 0) {
        if (ini.fStartupFile != 0) {
            ini.wFlags = 0;
            fFileErrSilent = 0;
        }
        return 0;
    }
    if (ini.fStartupFile != 0) {
        fFileErrSilent = 0;
        ini.fStartupFile = 0;
        pch = strrchr(szFile, 92);
        if (pch != 0) {
            i = pch - szFile;
            strncpy(szDirName, szFile, i);
            szDirName[i] = 0;
        }
        if (idPlayer == -1 && ini.fGen == 0) {
            gd.fClose = 1;
        }
        if (game.lid != ini.lid || game.turn > ini.turn) {
            ini.fTry = 0;
        }
    }
    sel.grobjFull = grobjNone;
    sel.grobj = grobjNone;
    sel.iwpAct = -1;
    sel.id = -1;
    sel.scan.grobjFull = grobjNone;
    sel.scan.grobj = grobjNone;
    sel.scan.iwp = -1;
    sel.scan.ifl = -1;
    sel.scan.idpl = -1;
    fOrdersVis = 0;
    CreateChildWindows();
    if (idPlayer != -1) {
        grobjIni = ini.grobjSel;
        SendMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
        if (grobjIni != grobjNone && ini.grobjSel == 0) {
            return 1;
        }
        ini.grobjSel = 0;
        if (ini.grobjSel == 0 && cPlanet != 0) {
            FFindSomethingAndSelectIt();
        }
        return 1;
    }
    if (hwndTitle != 0 && fFreeingTitle == 0) {
        fFreeingTitle = 1;
        DestroyWindow(hwndTitle);
        hwndTitle = 0;
    }
    BringUpHostDlg();
    return 0;
}

int16_t FWasRaceFile(char *szFile, int16_t fChkPass) {
    int16_t  idsError;
    int32_t  lSaltSav;
    PLAYER   plr;
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  fRet;
    int16_t  fSav;

    idsError = -1;
    fRet = 0;
    fSav = fFileErrSilent;
    fFileErrSilent = 1;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0) {
        StreamOpen(szFile, mdRead);
        ReadRt();
        if (hdrCur.rt != rtBOF || ((RTBOF *)rgbCur)->verMajor != 2 || ((RTBOF *)rgbCur)->verMinor < 49 || ((RTBOF *)rgbCur)->verMinor >= 84) {
            idsError = 13;
            fRet = -1;
        } else {
            wVersFile = ((RTBOF *)rgbCur)->wVersion;
            if (((RTBOF *)rgbCur)->dt == 5) {
                ReadRt();
                if (hdrCur.rt == rtPlr) {
                    idsError = 3;
                    ReadRtPlr(&plr, rgbCur);
                    ReadRt();
                    if (hdrCur.rt == rtEOF && RawLoad16(rgbCur) == IRaceChecksum(&plr)) {
                        lSaltSav = lSaltCur;
                        lSaltCur = plr.lSalt;
                        if (fChkPass != 0 && FCheckPassword() == 0) {
                            lSaltCur = lSaltSav;
                            fRet = -1;
                        } else {
                            lSaltCur = lSaltSav;
                            if (plr.lSalt != 0) {
                                strcpy(szRacePass, szPassLast);
                            } else {
                                szRacePass[0] = 0;
                            }
                            vplr = plr;
                            strcpy(szRaceFile, szFile);
                            StreamClose();
                            fFileErrSilent = fSav;
                            penvMem = penvMemSav;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    StreamClose();
    penvMem = penvMemSav;
    fFileErrSilent = fSav;
    if (fFileErrSilent == 0 && idsError != -1) {
        strcpy(szWork, szFile);
        AlertSz(PszFormatIds(idsError, NULL), MB_ICONHAND);
    }
    return fRet;
}

void BringUpHostDlg() {
    POINT16 pt;
    FARPROC lpProc;
    int16_t fRet;

    if (gd.fHostMode == 0) {
        if (gd.fReadOnly == 0) {
            FMarkFile(dtHost, -1, mdMarkInUse, 1);
        }
        gd.fHostMode = 1;
    }
    ShowWindow(hwndFrame, SW_HIDE);
    if (ini.fWait == 0) {
        while (1) {
            lpProc = MakeProcInstance(HostModeDialog, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_HOST_MODE), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == -1)
                goto LAutoMode;
            if (fRet == 0)
                break;
            do {
                if (gd.fProgressTxt != 0) {
                    ShowProgressGauge();
                }
                EnsureAis();
                FGenerateTurn();
                if (iPassCnt == 0)
                    break;
                iPassCnt--;
            } while (GetAsyncKeyState(VK_SHIFT) >= 0 && GetAsyncKeyState(VK_CONTROL) >= 0);
            iPassCnt = 0;
            HideProgressGauge();
        }
        if (gd.fReadOnly == 0) {
            FMarkFile(dtHost, -1, mdMarkInUse, 0);
        }
        gd.fHostMode = 0;
        DestroyCurGame();
        if (ini.fGen != 0) {
            return;
        }
        pt.x = GetSystemMetrics(SM_CXSCREEN);
        pt.y = GetSystemMetrics(SM_CYSCREEN);
        hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
        fFreeingTitle = 0;
        return;
    }
    ini.fWait = 0;
LAutoMode:
    ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
    uTimerId = SetTimer(NULL, hostTimerAutoGen, 10000, lpfnHostTimerProc);
    uTimerType = hostTimerAutoGen;
    HostTimerProc(NULL, WM_NULL, uTimerId, 0);
    return;
}

void DrawHostDialog2(HWND hwnd, HDC hdcIn) {
    uint32_t dsec;
    HDC      hdc;
    uint16_t dhour;
    int16_t  bkMode;
    int16_t  yCur;
    int16_t  i;
    uint16_t dmin;
    int16_t  dday;
    int16_t  cch;
    RECT     rcDiamond;
    COLORREF crBackSav;
    int16_t  x;
    char     szStat[30];

    if (hdcIn != 0) {
        hdc = hdcIn;
    } else {
        hdc = GetDC(hwnd);
    }
    bkMode = SetBkMode(hdc, OPAQUE);
    crBackSav = SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    x = dyArial8 + 10 + LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN16), 4));
    yCur = 48;
    SetRect(&rcDiamond, 6, yCur, dyArial8 + 7, yCur + dyArial8 + 1);
    for (i = 0; i < game.cPlayer; i++) {
        DrawDiamond(hdc, &rcDiamond, hbrBBlue);
        cch = _wsprintf(szWork, PszGetCompressedString(idsD2), i + 1);
        RightTextOut(hdc, x, yCur, szWork, cch, 0);
        SetTextColor(hdc, rgOut[i] <= 0 ? 32512 : 127);
        CchGetString(rgOut[i] + 716, szStat);
        if (gd.fNoHostNames != 0) {
            cch = _wsprintf(szWork, " %s", szStat);
        } else {
            cch = _wsprintf(szWork, PszGetCompressedString(idsSS), PszPlayerName(i, 1, 1, 1, 0, NULL), szStat);
        }
        if (rgplr[i].fHacker != 0) {
            strcat(szWork, " - HACKER");
            cch += 9;
        }
        TextOut(hdc, x + 4, yCur, szWork, cch);
        SetTextColor(hdc, crWindowText);
        OffsetRect(&rcDiamond, 0, dyArial8 + 4);
        yCur += dyArial8 + 4;
    }
    cch = _wsprintf(szWork, PCTD, game.turn + 2401);
    SetWindowText(GetDlgItem(hwnd, IDC_HOST_NEXT_YEAR_TEXT), szWork);
    dsec = (uint32_t)((GetTickCount() - ctickLast) / 1000);
    if (dsec < 60) {
        cch = _wsprintf(szWork, PszGetCompressedString(idsDSeconds), LOWORD(dsec));
    } else {
        dmin = LOWORD((uint32_t)(dsec / 60));
        dsec -= (uint32_t)(60 * dmin);
        if (dmin < 60) {
            cch = _wsprintf(szWork, PszGetCompressedString(idsD02d), dmin, LOWORD(dsec));
        } else {
            dhour = (uint32_t)dmin / 60;
            dmin -= 60 * dhour;
            if (dhour < 24) {
                cch = _wsprintf(szWork, PszGetCompressedString(idsD02d02d), dhour, dmin, LOWORD(dsec));
            } else {
                dday = (uint32_t)dhour / 24;
                dhour -= dday * 24;
                cch = _wsprintf(szWork, PszGetCompressedString(idsDDaysD02d02d), dday, dhour, dmin, LOWORD(dsec));
            }
        }
    }
    SetWindowText(GetDlgItem(hwnd, IDC_HOST_TIME_SINCE_TEXT), szWork);
    SetBkMode(hdc, bkMode);
    SetBkColor(hdc, crBackSav);
    if (hdcIn == 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

void VerifyTurns() {
    int16_t idsError;
    int16_t idCur;
    int16_t cAi;
    int16_t i;
    int16_t cOut;
    int16_t fOut;

    idCur = idPlayer;
    cOut = 0;
    cAi = 0;
    lpcd = LpAlloc(1000 * sizeof(COLDROP), htMisc);
    lpxf = LpAlloc(1000 * sizeof(XFERFULL), htMisc);
    vrgPlanResExtra = LpAlloc(game.cPlanMax * 2, htMisc);
    fmemset(vrgPlanResExtra, 0, game.cPlanMax * 2);
    vrgts = LpAlloc(game.cPlayer * sizeof(TURNSERIAL), htMisc);
    cColDrop = 0;
    cXferFull = 0;
    imemMsgCur = 0;
    for (i = 0; i < game.cPlayer; i++) {
        fOut = rgOut[i];
        idsError = 0;
        if (rgplr[i].fAi != 0 || FCheckLogFile(i, &idsError) != 0) {
            if (rgplr[i].fAi != 0) {
                cAi++;
                rgOut[i] = 0;
            } else {
                _wsprintf(szWork, "%s.x%d", szBase, i + 1);
                idPlayer = i;
                if (FLoadLogFile(szWork) != 0 && FRunLogFile() == 0) {
                    rgOut[i] = 3;
                } else {
                    rgOut[i] = 0;
                }
            }
        } else {
            switch (idsError) {
            case 29:
                rgOut[i] = 5;
                goto L_6874;
            case 28:
                rgOut[i] = 4;
                goto L_6874;
            default:
                rgOut[i] = 3;
                goto L_6874;
            case 0:
                if (rgplr[i].fDead != 0) {
                    rgOut[i] = -1;
                } else if (gd.fPartialTurn != 0) {
                    rgOut[i] = 2;
                    cOut++;
                } else {
                    rgOut[i] = 1;
                    cOut++;
                }
            }
            goto L_68d6;
        L_6874:
            cOut++;
        }
    L_68d6:
        if (ctickLast == 0 || rgOut[i] != fOut) {
            ctickLast = GetTickCount();
        }
    }
    FreeLp(vrgPlanResExtra, htMisc);
    vrgPlanResExtra = NULL;
    FreeLp(vrgts, htMisc);
    vrgts = NULL;
    FreeLp(lpcd, htMisc);
    lpcd = NULL;
    FreeLp(lpxf, htMisc);
    lpxf = NULL;
    idPlayer = idCur;
    return;
}

int16_t CTurnsOutSafe() {
    int16_t idPlayerSav;
    int16_t fHostModeSav;
    int16_t fGenSav;
    int16_t cturn;

    fHostModeSav = gd.fHostMode;
    fGenSav = gd.fGeneratingTurn;
    idPlayerSav = idPlayer;
    idPlayer = -1;
    gd.fHostMode = 1;
    gd.fGeneratingTurn = 0;
    cturn = CFindTurnsOutstanding();
    gd.fGeneratingTurn = fGenSav;
    gd.fHostMode = fHostModeSav;
    idPlayer = idPlayerSav;
    return cturn;
}

int16_t CFindTurnsOutstanding() {
    int16_t idsError;
    int16_t cAi;
    int16_t i;
    int16_t cOut;
    int16_t fSav;
    int16_t fOut;

    cOut = 0;
    cAi = 0;
    fSav = fFileErrSilent;
    fFileErrSilent = 1;
    gd.fGeneratingTurn = 1;
    for (i = 0; i < game.cPlayer; i++) {
        fOut = rgOut[i];
        idsError = 0;
        if (rgplr[i].fAi != 0 || FCheckLogFile(i, &idsError) != 0) {
            if (rgplr[i].fAi != 0) {
                cAi++;
            }
            rgOut[i] = 0;
        } else {
            switch (idsError) {
            case 29:
                rgOut[i] = 5;
                goto L_6b2d;
            case 28:
                rgOut[i] = 4;
                goto L_6b2d;
            default:
                rgOut[i] = 3;
                goto L_6b2d;
            case 0:
                if (rgplr[i].fDead != 0) {
                    rgOut[i] = -1;
                } else if (gd.fPartialTurn != 0) {
                    rgOut[i] = 2;
                    cOut++;
                } else {
                    rgOut[i] = 1;
                    cOut++;
                }
            }
            goto L_6b8f;
        L_6b2d:
            cOut++;
        }
    L_6b8f:
        if (ctickLast == 0 || rgOut[i] != fOut) {
            ctickLast = GetTickCount();
        }
    }
    gd.fGeneratingTurn = 0;
    gd.fAllAis = cAi == game.cPlayer ? 1 : 0;
    fFileErrSilent = 0;
    return cOut;
}

INT_PTR CALLBACK HostModeDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    FARPROC     lpProc;
    int16_t     fRet;
    RECT        rc;
    int16_t     mf;
    POINT16     pt;
    int16_t     tpm;
    int16_t     i;
    int16_t     iRet;
    int16_t     iSel;
    int16_t     iDiamond;
    HMENU       hmenuPopup;
    MSG         msg;
    HDC         hdc;
    PAINTSTRUCT ps;
    HWND        t_call_6c71;
    HWND        t_call_715f;
    HWND        t_call_7203;
    HWND        t_call_74e0;

    switch (message) {
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (ctickLast == 0) {
            CFindTurnsOutstanding();
        }
        if (gd.fReadOnly == 0) {
            t_call_7203 = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
            EnableWindow(t_call_7203, gd.fAllAis == 0 && (vtimer.fAutoGenWhenIn != 0 || vtimer.mdForce != 0));
        }
        DrawHostDialog2(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_DESTROY:
        KillTimer(hwnd, uTimerId);
        uTimerId = 0;
        break;
    default:
        if (IS_WM_CTLCOLOR(message) != 0) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
        switch (message) {
        case WM_INITDIALOG:
            StickyDlgPos(hwnd, &ptStickyHostModeDlg, 1);
            SetWindowText(GetDlgItem(hwnd, IDC_HOST_GAME_NAME_TEXT), game.szName);
            SetWindowText(GetDlgItem(hwnd, IDC_HOST_FILE_TEXT), szBase);
            t_call_6c71 = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
            EnableWindow(t_call_6c71, gd.fReadOnly == 0 && (vtimer.fAutoGenWhenIn != 0 || vtimer.mdForce != 0));
            EnableWindow(GetDlgItem(hwnd, IDC_HOST_GENERATE_NOW), gd.fReadOnly == 0 ? 1 : 0);
            EnableWindow(GetDlgItem(hwnd, IDC_HOST_PASSWORD), gd.fReadOnly == 0 ? 1 : 0);
            uTimerId = SetTimer(hwnd, 13, 10000, NULL);
        case WM_TIMER:
            if (fProcessingTimer == 0) {
                fProcessingTimer = 1;
                CFindTurnsOutstanding();
                DrawHostDialog2(hwnd, NULL);
                fProcessingTimer = 0;
            }
            if (message != WM_TIMER) {
                return 1;
            }
            return 0;
        case WM_SETCURSOR:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            GetCursorPos16(&pt);
            ScreenToClient16(hwnd, &pt);
            if (pt.x < 6 || pt.x >= dyArial8 + 7 || pt.y < 48)
                break;
            iDiamond = (int16_t)(pt.y - 48) / (dyArial8 + 4);
            if (iDiamond >= game.cPlayer || (int16_t)(pt.y - 48) % (dyArial8 + 4) >= dyArial8 + 1)
                break;
            if (message == WM_SETCURSOR) {
                SetCursor(hcurHand);
                return 1;
            }
            if (rgplr[iDiamond].fAi == 0) {
                iSel = 0;
            } else if (rgplr[iDiamond].idAi == idAiMaid) {
                iSel = 2;
            } else {
                iSel = 1;
            }
            hmenuPopup = CreatePopupMenu();
            iPopMenuSel = -1;
            for (i = 0; i < 3; i++) {
                CchGetString(i + 527, szWork);
                mf = i == 1                                                         ? rgplr[iDiamond].fAi != 0 && rgplr[iDiamond].idAi != idAiMaid ? 0 : 3
                     : rgplr[iDiamond].fAi == 0 || rgplr[iDiamond].idAi == idAiMaid ? 0
                                                                                    : 3;
                AppendMenu(hmenuPopup, (i == iSel ? 8 : 0) | mf, i + 15000, szWork);
            }
            ClientToScreen16(hwnd, &pt);
            tpm = message == WM_LBUTTONDOWN ? TPM_LEFTBUTTON : TPM_RIGHTBUTTON;
            TrackPopupMenu(hmenuPopup, TPM_CENTERALIGN | tpm, pt.x, pt.y, 0, hwnd, NULL);
            DestroyMenu(hmenuPopup);
            iRet = -1;
            if (PeekMessage(&msg, hwnd, 273, 273, 2) != 0 && msg.wParam >= 15000 && msg.wParam < 15100) {
                iRet = msg.wParam - 15000;
            }
            if (iRet != -1 && iSel != iRet) {
                rgplr[iDiamond].wMdPlr = (rgplr[iDiamond].wMdPlr & 0xfdff) | ((iRet <= 0 ? 0 : 1) & 1) * 0x200;
                if (iRet == 2) {
                    rgplr[iDiamond].wMdPlr = (rgplr[iDiamond].wMdPlr & 0x1fff) | 0xe000;
                }
                rgplr[iDiamond].lSalt = ~rgplr[iDiamond].lSalt;
                FMarkFile(dtTurn, iDiamond, mdMarkAi, iRet == 0 ? 0 : 1);
                FMarkFile(dtHost, iDiamond, mdMarkAi, iRet == 0 ? 0 : 1);
                gd.fAisDone = 0;
                fProcessingTimer = 1;
                CFindTurnsOutstanding();
                t_call_715f = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
                EnableWindow(t_call_715f, gd.fAllAis == 0 && (vtimer.fAutoGenWhenIn != 0 || vtimer.mdForce != 0));
                DrawHostDialog2(hwnd, NULL);
                fProcessingTimer = 0;
            }
            return 0;
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDC_HOST_GENERATE_NOW:
            case IDCANCEL:
            case IDC_HOST_AUTO_GENERATE:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HOST_GENERATE_NOW) {
                    if (GetAsyncKeyState(VK_SHIFT) < 0) {
                        if (GetAsyncKeyState(VK_CONTROL) < 0) {
                            iPassCnt = 999;
                        } else {
                            iPassCnt = 9;
                        }
                    } else if (GetAsyncKeyState(VK_CONTROL) < 0) {
                        iPassCnt = 99;
                    } else {
                        iPassCnt = 0;
                    }
                    if (iPassCnt != 0) {
                        _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), iPassCnt + 1);
                        if (MessageBox(GetFocus(), szWork, "Stars!", MB_YESNO | MB_ICONEXCLAMATION | MB_TASKMODAL) != IDYES) {
                            iPassCnt = 0;
                            return 1;
                        }
                    } else if (CFindTurnsOutstanding() != 0 && AlertSz(PszFormatIds(idsSureWishGenerateOptionDoesGuaranteePlayers, NULL),
                                                                       MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL) != IDYES) {
                        return 1;
                    }
                }
                StickyDlgPos(hwnd, &ptStickyHostModeDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL ? 0 : GET_WM_COMMAND_ID(wParam, lParam) == IDC_HOST_AUTO_GENERATE ? -1 : 1);
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HOST_AUTO_GENERATE) {
                    EnsureAis();
                } else if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL && gd.fClose != 0) {
                    PostQuitMessage(vretExitValue);
                }
                return 1;
            case IDC_HOST_PASSWORD:
                if (FCheckPassword() == 0)
                    break;
                lpProc = MakeProcInstance(NewPasswordDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_PASSWORD), hwnd, lpProc);
                FreeProcInstance(lpProc);
                SetFocus(hwnd);
                return fRet;
            case 1029:
                lpProc = MakeProcInstance(HostOptionsDialog, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_HOST_OPTIONS), hwnd, lpProc);
                FreeProcInstance(lpProc);
                SetFocus(hwnd);
                if (fRet != 0 && gd.fReadOnly == 0) {
                    t_call_74e0 = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
                    EnableWindow(t_call_74e0, gd.fAllAis == 0 && (vtimer.fAutoGenWhenIn != 0 || vtimer.mdForce != 0));
                }
                return fRet;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, HELP_CONTEXT, idhHostModeDialog);
                return 1;
            }
        }
    }
    return 0;
}

INT_PTR CALLBACK HostOptionsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;

    switch (message) {
    case WM_ERASEBKGND:
    L_75dd:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawHostOptions(hwnd, hdc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    default:
        if (IS_WM_CTLCOLOR(message) != 0) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
        if (message == WM_INITDIALOG)
            goto L_75dd;
        if (message == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDCANCEL:
            case IDOK:
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, HELP_CONTEXT, idhHostModeDialog);
                return 1;
            }
        }
    case WM_DESTROY:
        return 0;
    }
}

void DrawHostOptions(HWND hwnd, HDC hdc, int16_t iDraw) { return; }

VOID CALLBACK HostTimerProc(HWND hwnd, UINT msg, UINT_PTR idTimer, DWORD dwTime) {
    HWND    hwndT;
    char    szExt[4];
    int16_t cOut;
    int16_t fSav;
    int16_t idCur;

    if (fProcessingTimer == 0) {
        fProcessingTimer = 1;
        fSav = fFileErrSilent;
        if (uTimerType == hostTimerWaitTurn) {
            if (FNewTurnAvail(idPlayer) == 0)
                goto Done;
            idCur = idPlayer;
            KillTimer(hwnd, uTimerId);
            _wsprintf(szExt, MPCTD, idPlayer + 1);
            DestroyCurGame();
            if (FLoadGame(szBase, szExt) == 0) {
                AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, NULL), MB_ICONHAND);
                goto Done;
            }
            idPlayer = idCur;
            CreateChildWindows();
            uTimerId = SetTimer(NULL, hostTimerTurnReady, 1000, lpfnHostTimerProc);
            uTimerType = hostTimerTurnReady;
            FlashWindow(hwndFrame, 1);
            SetWindowText(hwndFrame, PszGetCompressedString(idsNewTurnAvailable2));
            MessageBeep(MB_ICONEXCLAMATION);
            cOut = 1;
            goto RedrawText;
        }
        if (uTimerType == hostTimerTurnReady) {
            FlashWindow(hwndFrame, 1);
            goto Done;
        }
    Loop:
        cOut = CFindTurnsOutstanding();
        if (gd.fAllAis != 0) {
            AlertSz(PszFormatIds(idsAutoGenerateDisabledBecauseHumanPlayersDead, NULL), MB_ICONHAND);
            EnableWindow(GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE), 0);
            goto Done;
        }
        _wsprintf(szWork, PszGetCompressedString(idsHostModeDPlayer), cOut);
        if (cOut != 1) {
            strcat(szWork, "s");
        }
        strcat(szWork, PszGetCompressedString(idsOut));
        SetWindowText(hwndFrame, szWork);
    RedrawText:
        hwndT = GetWindow(hwndFrame, GW_HWNDPREV);
        if (GetWindow(hwndT, GW_OWNER) == hwndFrame) {
            InvalidateRect(hwndT, NULL, 1);
        }
        if (cOut == 0) {
            if (gd.fProgressTxt != 0) {
                ShowProgressGauge();
            }
            EnsureAis();
            FGenerateTurn();
            HideProgressGauge();
            if (ini.fGen != 0) {
                PostQuitMessage(vretExitValue);
            } else {
                EnsureAis();
                goto Loop;
            }
        }
    Done:
        fProcessingTimer = 0;
        fFileErrSilent = fSav;
    }
    return;
}

void GetWindowRc(HWND hwnd, RECT *prc) {
    WINDOWPLACEMENT wndpl;

    wndpl.length = 22;
    GetWindowPlacement(hwnd, &wndpl);
    *prc = wndpl.rcNormalPosition;
    prc->right -= prc->left;
    prc->bottom -= prc->top;
    return;
}

void SetWindowIniString(char *sz, HWND hwnd) {
    char ch;
    RECT rc;

    if (IsZoomed(hwnd) != 0) {
        ch = 'M';
    } else if (IsIconic(hwnd) != 0) {
        ch = 'I';
    } else {
        ch = 'R';
    }
    GetWindowRc(hwnd, &rc);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), ch, rc.left, rc.top, rc.right, rc.bottom);
    return;
}

void WriteIniSettings() {
    int16_t  ctile;
    char     szPd[3];
    TILE    *rgtile;
    int16_t  i;
    int16_t  iPass;
    char     szEntry[16];
    char     szIniFile[16];
    char    *psz;
    char     szSection[16];
    uint16_t iCol;
    char     ch;

    szPd[0] = '%';
    szPd[1] = 'd';
    szPd[2] = 0;
    CchGetString(idsWindows, szSection);
    CchGetString(idsStarsIni, szIniFile);
    CchGetString(idsGlobalsettings, szEntry);
    FormatSerialAndEnv(vSerialNumber, vrgbMachineConfig, szWork);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsResolution, szEntry);
    i = 0;
    if (vcScreenColors <= 4) {
        i |= 1;
    }
    if (gd.mdScreenSize == 0) {
        i |= 2;
    }
    _wsprintf(szWork, szPd, i);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsMain, szEntry);
    SetWindowIniString(szWork, hwndFrame);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportfleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 77, vrptFleet.ptDlg.x, vrptFleet.ptDlg.y, vrptFleet.ptDlg.x + vrptFleet.ptSize.x,
              vrptFleet.ptDlg.y + vrptFleet.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportefleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 77, vrptEFleet.ptDlg.x, vrptEFleet.ptDlg.y, vrptEFleet.ptDlg.x + vrptEFleet.ptSize.x,
              vrptEFleet.ptDlg.y + vrptEFleet.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportbtlwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 77, vrptBattle.ptDlg.x, vrptBattle.ptDlg.y, vrptBattle.ptDlg.x + vrptBattle.ptSize.x,
              vrptBattle.ptDlg.y + vrptBattle.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportplanwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 77, vrptPlanet.ptDlg.x, vrptPlanet.ptDlg.y, vrptPlanet.ptDlg.x + vrptPlanet.ptSize.x,
              vrptPlanet.ptDlg.y + vrptPlanet.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsLayout, szEntry);
    _wsprintf(szWork, szPd, iWindowLayout);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle1width, szEntry);
    _wsprintf(szWork, szPd, vfs.dxPlanWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle1height, szEntry);
    _wsprintf(szWork, szPd, vfs.dyMsgWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle1height2, szEntry);
    _wsprintf(szWork, szPd, vfs.dyMinWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle2width, szEntry);
    _wsprintf(szWork, szPd, vfs.dx2PlanWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle2height, szEntry);
    _wsprintf(szWork, szPd, vfs.dy2MsgWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsStyle2height2, szEntry);
    _wsprintf(szWork, szPd, vfs.dy2MinWant);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsToolbar, szEntry);
    _wsprintf(szWork, szPd, gd.fToolbar);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    iPass = 2;
    rgtile = rgtilePlanet;
    ctile = 6;
    CchGetString(idsPlanettiles, szEntry);
    while (iPass-- != 0) {
        psz = szWork;
        iCol = 0;
        i = 0;
        while (i < ctile) {
            while (rgtile[i].iCol > iCol) {
                iCol++;
                *psz = '*';
                psz++;
            }
            *psz = LOBYTE(rgtile[i].fPopped == 0 ? 97 : 65);
            *psz += LOBYTE(rgtile[i].id);
            i++;
            psz++;
        }
        *psz = 0;
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsShiptiles, szEntry);
        rgtile = rgtileShip;
        ctile = 7;
    }
    CchGetString(idsSelection, szEntry);
    switch (sel.grobj) {
    case grobjNone:
        ch = 'N';
        break;
    case grobjPlanet:
        ch = 'P';
        break;
    case grobjFleet:
        ch = 'S';
        break;
    case grobjOther:
        ch = 'E';
    }
    _wsprintf(szWork, PszGetCompressedString(idsCCD), ch, (int16_t)(int8_t)LOBYTE(idPlayer + 66), sel.id);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsMessage, szEntry);
    _wsprintf(szWork, PCTD, iMsgCur);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsGameid, szEntry);
    _wsprintf(szWork, "%lx", game.lid);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsScanzoom, szEntry);
    szWork[0] = LOBYTE(iScanZoom + 53);
    szWork[1] = 0;
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    if (gd.fChgScanner != 0) {
        itoa(grbitScan, szWork, 10);
        CchGetString(idsScanmodev25, szEntry);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        itoa(grbitScanShip, szWork, 10);
        CchGetString(idsScanfilterv25, szEntry);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        itoa(grbitScanEShip, szWork, 10);
        CchGetString(idsScanefilterv25, szEntry);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        itoa(grbitScanMines, szWork, 10);
        CchGetString(idsScanmines, szEntry);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        itoa(vpctRadarView, szWork, 10);
        CchGetString(idsScanradar, szEntry);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    }
    itoa(cMinGrafMax, szWork, 10);
    CchGetString(idsMineralscale, szEntry);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    if (idPlayer != -1) {
        CchGetString(idsFiles, szSection);
        CchGetString(idsWait2, szEntry);
        szWork[0] = LOBYTE((uTimerId == 0 ? 0 : 1) + 48);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        if (gd.fWriteTurnNum != 0) {
            itoa(game.turn, szWork, 10);
            CchGetString(idsTurn, szEntry);
            WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
            gd.fWriteTurnNum = 0;
        }
        CchGetString(idsFile1, szEntry);
        _wsprintf(szWork, "%s.m%d", szBase, idPlayer + 1);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    }
    CchGetString(idsMisc, szSection);
    if (gd.fChgReports != 0) {
        CchGetString(idsReportplanfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptPlanet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportplansort, szEntry);
        i = vrptPlanet.icolSort;
        if (vrptPlanet.fAscending != 0) {
            i |= 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportfleetfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptFleet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportfleetsort, szEntry);
        i = vrptFleet.icolSort;
        if (vrptFleet.fAscending != 0) {
            i |= 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportefleetfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptEFleet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportefltsort, szEntry);
        i = vrptEFleet.icolSort;
        if (vrptEFleet.fAscending != 0) {
            i |= 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportbtlfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptBattle.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportbtlsort, szEntry);
        i = vrptBattle.icolSort;
        if (vrptBattle.fAscending != 0) {
            i |= 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportdefgraph, szEntry);
        _wsprintf(szWork, PCTD, gd.iCurGraph);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    }
    CchGetString(idsHistoryinfo, szEntry);
    _wsprintf(szWork, PCTD, uDateInstalled);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsVcrspeed, szEntry);
    _wsprintf(szWork, PCTD, viSpeedVCR);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    if (gd.fChgZipOrd != 0) {
        CchGetString(idsZiporders, szSection);
        for (i = 0; i < 4; i++) {
            strcpy(szEntry, szSection);
            psz = &szEntry[strlen(szEntry)];
            *psz = LOBYTE(i + 49);
            psz[1] = 0;
            if (vrgZip[i].fValid != 0) {
                psz = szWork;
                for (iPass = 0; iPass < 5; iPass++) {
                    *psz++ = LOBYTE((vrgZip[i].txp.rgia[iPass].iAction & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan & 0xf & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan >> 4 & 0xf & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan >> 8 & 0xf & 0xff) + 0x61);
                }
                strcpy(psz, vrgZip[i].szName);
            } else {
                szWork[0] = 0;
            }
            WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        }
    }
    if (gd.fChgZipProd != 0) {
        for (i = 0; i < 5; i++) {
            CchGetString(idsZiporders, szSection);
            strcpy(szEntry, szSection);
            psz = &szEntry[strlen(szEntry)];
            *psz++ = 'P';
            *psz = LOBYTE(i + 49);
            psz[1] = 0;
            if (vrgZipProd[i].fValid != 0) {
                psz = szWork;
                *psz++ = LOBYTE(vrgZipProd[i].fNoResearch + 97);
                *psz++ = LOBYTE(vrgZipProd[i].cpq + 97);
                for (iPass = 0; iPass < vrgZipProd[i].cpq; iPass++) {
                    *psz++ = LOBYTE((vrgZipProd[i].rgpq[iPass].w & 0xf & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 4 & 0xf & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 8 & 0xf & 0xff) + 0x61);
                    *psz++ = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 0xc & 0xf & 0xff) + 0x61);
                }
                strcpy(psz, vrgZipProd[i].szName);
            } else {
                szWork[0] = 0;
            }
            WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        }
    }
    return;
}

void RefitFrameChildren() {
    int16_t dyMsg;
    HMENU   hmenu;
    int16_t i;
    int16_t dyMinMin;
    int16_t dyMsgMin;
    int16_t dyMin;
    int16_t dyT;
    int16_t dyTot;
    int16_t yScanner;

    if (hwndFrame != 0 && IsIconic(hwndFrame) == 0) {
        switch (iWindowLayout) {
        case layoutLarge:
        default:
            if (vfs.dx - vfs.dxPlanWant < 100) {
                vfs.xTop = vfs.dx - 100;
            } else {
                vfs.xTop = vfs.dxPlanWant;
            }
            vfs.xTop = vfs.xTop <= 198 ? 198 : vfs.xTop;
            dyMsgMin = (0xd * dyArial8 >> 1) + 0xa;
            dyMinMin = 13 * dyArial8 - 36;
            dyMsg = vfs.dyMsgWant <= dyMsgMin ? dyMsgMin : vfs.dyMsgWant;
            vfs.dyMsgWant = dyMsg;
            dyMin = vfs.dyMinWant <= dyMinMin ? dyMinMin : vfs.dyMinWant;
            vfs.dyMinWant = dyMin;
            if (vfs.dy - (dyMsg + dyMin + 16) < 50) {
                dyT = vfs.dy - 66;
                dyTot = dyMsg + dyMin;
                dyMsg = MulDiv(dyT, dyMsg, dyTot);
                dyMin = MulDiv(dyT, dyMin, dyTot);
                if (dyMsg < dyMsgMin) {
                    dyMin -= dyMsgMin - dyMsg;
                    dyMsg = dyMsgMin;
                } else if (dyMin < dyMinMin) {
                    dyMsg -= dyMinMin - dyMin;
                    dyMin = dyMinMin;
                }
            }
            vfs.y1 = vfs.dy - dyMin - dyMsg - 16;
            vfs.y2 = vfs.y1 + dyMsg + 8;
            if (hwndScanner == 0)
                break;
            if (gd.fToolbar != 0) {
                MoveWindow(hwndTb, vfs.xTop + 8, 0, vfs.dx - vfs.xTop - 8, 36, 1);
                yScanner = 36;
            } else {
                MoveWindow(hwndTb, 0, -100, 50, 50, 1);
                yScanner = 0;
            }
            MoveWindow(hwndScanner, vfs.xTop + 8, yScanner, vfs.dx - vfs.xTop - 8, vfs.dy - yScanner, 1);
            MoveWindow(hwndPlanet, 0, 0, vfs.xTop, vfs.y1, 1);
            MoveWindow(hwndMessage, 0, vfs.y1 + 8, vfs.xTop, dyMsg, 1);
            MoveWindow(hwndMine, 0, vfs.y2 + 8, vfs.xTop, dyMin, 1);
            break;
        case layoutMedium:
        case layoutSmall:
            if (vfs.dx - vfs.dx2PlanWant < 200) {
                vfs.xTop = vfs.dx - 200;
            } else {
                vfs.xTop = vfs.dx2PlanWant;
            }
            vfs.xTop = vfs.xTop <= 198 ? 198 : vfs.xTop;
            dyMsgMin = (0xd * dyArial8 >> 1) + 0xa;
            dyMinMin = 13 * dyArial8 - 36;
            dyMsg = vfs.dy2MsgWant <= dyMsgMin ? dyMsgMin : vfs.dy2MsgWant;
            vfs.dy2MsgWant = dyMsg;
            dyMin = vfs.dy2MinWant <= dyMinMin ? dyMinMin : vfs.dy2MinWant;
            vfs.dy2MinWant = dyMin;
            if (vfs.dy - (dyMsg + 8) < 100) {
                dyMsg = vfs.dy - 108;
            }
            if (vfs.dy - (dyMin + 8) < 100) {
                dyMin = vfs.dy - 108;
            }
            vfs.y1 = vfs.dy - dyMsg - 8;
            vfs.y2 = vfs.dy - dyMin - 8;
            if (hwndScanner != 0) {
                if (gd.fToolbar != 0) {
                    MoveWindow(hwndTb, 0, 0, vfs.dx, 36, 1);
                    yScanner = 36;
                } else {
                    MoveWindow(hwndTb, 0, -100, 50, 50, 1);
                    yScanner = 0;
                }
                MoveWindow(hwndScanner, vfs.xTop + 8, yScanner, vfs.dx - vfs.xTop - 8, vfs.y2 - yScanner, 1);
                MoveWindow(hwndPlanet, 0, yScanner, vfs.xTop, vfs.y1 - yScanner, 1);
                MoveWindow(hwndMessage, 0, vfs.y1 + 8, vfs.xTop, dyMsg, 1);
                MoveWindow(hwndMine, vfs.xTop + 8, vfs.y2 + 8, vfs.dx - vfs.xTop - 8, dyMin, 1);
            }
        }
        hmenu = GetASubMenu(hwndFrame, menuView);
        hmenu = GetSubMenu(hmenu, 4);
        for (i = 130; i <= 132; i++) {
            CheckMenuItem(hmenu, i, i - 130 == iWindowLayout ? MF_CHECKED : MF_UNCHECKED);
        }
    }
    return;
}

LRESULT CALLBACK TitleWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     i;
    HPALETTE    hpalSav;
    RECT        rc;
    int16_t     dy;
    int16_t     dxGap;
    int16_t     dx;
    int16_t     xCur;
    char       *psz;
    PAINTSTRUCT ps;
    RECT        rcWnd;
    HBRUSH      hbrSav;
    RECT        rcT;
    LOGFONT    *plf;
    HFONT       hfont;
    HFONT       hfontSav;

    switch (msg) {
    case WM_CREATE:
        if (vcScreenColors >= 8) {
            vhdibTitle = HdibLoadBigResource(IDDIB_SPLASH);
            if (vhpalSplash == 0 && vhdibTitle != 0) {
                vhpalSplash = HpalFromDib(vhdibTitle);
            }
        }
        GetClientRect(hwnd, &rc);
        dx = 120 <= rc.right >> 3 ? rc.right >> 3 : 120;
        if (rc.bottom < 650) {
            dx += dx / 6;
        }
        dxGap = (rc.right - (dx << 2)) >> 2;
        xCur = dxGap >> 1;
        dy = rc.bottom <= 500 ? dyArial8 * 2 : (int16_t)(5 * dyArial8) / 2;
        for (i = 0; i < 4; i++) {
            psz = PszGetCompressedString(i + 479);
            rghwndBtnSplash[i] = CreateWindow("BUTTON", psz, WS_CHILD | WS_VISIBLE, xCur, rc.bottom - dy - (int16_t)(5 * dyArial8) / 2, dx, dy, hwnd,
                                              (HMENU)(uintptr_t)i, hInst, NULL);
            if (i == 2 && (szBase[0] == 0 || access(szBase, 0) == -1)) {
                EnableWindow(rghwndBtnSplash[2], 0);
            }
            if (rc.bottom < 500) {
                SendMessage(rghwndBtnSplash[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
            }
            xCur += dx + dxGap;
        }
        break;
    case WM_QUERYNEWPALETTE:
    MapIt:
        if (vcScreenColors < 8)
            break;
        hdc = GetDC(hwnd);
        hpalSav = SelectPalette(hdc, vhpalSplash, 0);
        i = RealizePalette(hdc);
        SelectPalette(hdc, hpalSav, 0);
        ReleaseDC(hwnd, hdc);
        if (i != 0) {
            InvalidateRect(hwnd, NULL, 1);
            return 1;
        }
        return 0;
    case WM_PALETTECHANGED:
        if ((HWND)wParam != hwnd)
            goto MapIt;
        break;
    case WM_DESTROY:
        if (vhdibTitle != 0) {
            GlobalUnlock(vhdibTitle);
            FreeResource(vhdibTitle);
            vhdibTitle = 0;
        }
        if (fFreeingTitle != 0)
            goto Default;
        if (gd.fExitWindows != 0) {
            ExitWindows(vretExitValue, 0);
            goto Default;
        }
        WriteIniSettings();
        PostQuitMessage(vretExitValue);
        goto Default;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case 0:
            NewGameWizard(hwnd, 0);
            if (lpPlanets == 0 && game.lid == 0) {
                SetFocus(hwnd);
                break;
            }
            if (fFreeingTitle == 0) {
                fFreeingTitle = 1;
                DestroyWindow(hwndTitle);
                hwndTitle = 0;
            }
            ShowWindow(hwndFrame, SW_SHOW);
            break;
        case IDOK:
        LOpenGame:
            if (FOpenGame(hwnd, 0) > 0) {
                if (fFreeingTitle == 0) {
                    fFreeingTitle = 1;
                    DestroyWindow(hwndTitle);
                    hwndTitle = 0;
                }
                if (idPlayer != -1) {
                    ShowWindow(hwndFrame, SW_SHOW);
                }
                InitializeMenu(NULL);
                PostMessage(hwndFrame, WM_COMMAND, IDM_FRAME_POST_OPEN, 0);
                if (game.fTutorial != 0 && idPlayer == 0) {
                    StartTutor(0);
                }
            } else {
                SetFocus(hwnd);
            }
            ini.fStartupFile = 0;
            break;
        case IDCANCEL:
            ini.fStartupFile = 1;
            goto LOpenGame;
        case 3:
            if (gd.fExitWindows != 0) {
                ExitWindows(vretExitValue, 0);
            } else {
                WriteIniSettings();
                PostQuitMessage(vretExitValue);
            }
        }
        break;
    case WM_PAINT:
        if (IsIconic(hwnd) != 0) {
            hdc = BeginPaint(hwnd, &ps);
            DrawIcon(hdc, 2, 2, hiconStars);
            EndPaint(hwnd, &ps);
            break;
        }
        plf = LocalAlloc(64, sizeof(LOGFONT));
        hdc = BeginPaint(hwnd, &ps);
        hbrSav = SelectObject(hdc, hbrButtonFace);
        GetClientRect(hwnd, &rcWnd);
        if (vcScreenColors >= 8) {
            SetTextColor(hdc, 50298879);
            if (vhdibTitle != 0) {
                SelectPalette(hdc, vhpalSplash, 0);
                RealizePalette(hdc);
                DibBlt(hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), vhdibTitle, 0, 0, 800, 600, 13369376);
                goto L_9752;
            }
        }
        rc.left = 0;
        rc.right = rcWnd.right;
        rc.bottom = 0;
        dx = rcWnd.right;
        DrawABunchOfStars(hdc, &rcWnd);
        plf->lfHeight = (int16_t)-rcWnd.bottom / 3;
        strcpy(plf->lfFaceName, rgszArial[3]);
        hfont = CreateFontIndirect(plf);
        SetTextColor(hdc, 10158235);
        if (hfont != 0) {
            hfontSav = SelectObject(hdc, hfont);
            SetBkMode(hdc, TRANSPARENT);
            rcT = rcWnd;
            rcT.bottom = (int16_t)(3 * rcT.bottom) / 4;
            RcCtrTextOut(hdc, &rcT, "Stars!", 6);
            SelectObject(hdc, hfontSav);
            DeleteObject(hfont);
        }
    L_9752:
        SelectObject(hdc, rghfontArial10[1]);
        SetBkMode(hdc, TRANSPARENT);
        SzVersion();
        GetWindowRect(rghwndBtnSplash[0], &rcT);
        rcWnd.top = rcT.top - (int16_t)(9 * dyArial8) / 2;
        rcWnd.bottom = (int16_t)(3 * dyArial8) / 2 + rcWnd.top;
        RcCtrTextOut(hdc, &rcWnd, szWork, strlen(szWork));
        EndPaint(hwnd, &ps);
        LocalFree(plf);
        break;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
