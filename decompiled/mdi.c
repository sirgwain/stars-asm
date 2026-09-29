#include "common.h"

uint8_t vrgbShuffleSerial[21] = {11, 4, 5, 16, 17, 12, 19, 15, 10, 1, 14, 13, 3, 18, 2, 20, 9, 7, 0, 8, 6};
char    rgTOWidth[2][2] = {{-3}, {2, 1}};

int16_t InitMDIApp() {
    WNDCLASS wc;

    wc.style = 0xb;
    wc.lpfnWndProc = (WNDPROC)FrameWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, "StarsIco");
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = (HBRUSH)13;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = szFrame;
    if (RegisterClass(&wc) != 0x0) {
        wc.style = 0x20b;
        wc.lpfnWndProc = (WNDPROC)MessageWndProc;
        wc.hIcon = 0x0;
        wc.lpszMenuName = 0x0;
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszClassName = szMessage;
        if (RegisterClass(&wc) != 0x0) {
            wc.style = 0x20b;
            wc.lpfnWndProc = (WNDPROC)ScannerWndProc;
            wc.hbrBackground = GetStockObject(BLACK_BRUSH);
            wc.lpszClassName = szScan;
            if (RegisterClass(&wc) != 0x0) {
                wc.style = 0x20b;
                wc.lpfnWndProc = (WNDPROC)MineWndProc;
                wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                wc.lpszClassName = szMine;
                if (RegisterClass(&wc) != 0x0) {
                    wc.style = 0x208;
                    wc.lpfnWndProc = (WNDPROC)TbWndProc;
                    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                    wc.lpszClassName = szTb;
                    if (RegisterClass(&wc) != 0x0) {
                        wc.style = 0x200;
                        wc.lpfnWndProc = (WNDPROC)PlanetWndProc;
                        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                        wc.hIcon = 0x0;
                        wc.lpszClassName = szPlanet;
                        if (RegisterClass(&wc) != 0x0) {
                            wc.style = 0xa00;
                            wc.lpfnWndProc = (WNDPROC)PopupWndProc;
                            wc.hbrBackground = GetStockObject(WHITE_BRUSH);
                            wc.hIcon = 0x0;
                            wc.lpszClassName = szPopup;
                            if (RegisterClass(&wc) != 0x0) {
                                wc.style = 0xa00;
                                wc.lpfnWndProc = (WNDPROC)TooltipWndProc;
                                wc.hbrBackground = GetStockObject(WHITE_BRUSH);
                                wc.hIcon = 0x0;
                                wc.lpszClassName = szTooltip;
                                if (RegisterClass(&wc) != 0x0) {
                                    wc.style = 0x200;
                                    wc.lpfnWndProc = (WNDPROC)BrowserWndProc;
                                    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                                    wc.hIcon = 0x0;
                                    wc.lpszClassName = szBrowser;
                                    if (RegisterClass(&wc) != 0x0) {
                                        wc.style = 0x0;
                                        wc.lpfnWndProc = (WNDPROC)TitleWndProc;
                                        wc.cbClsExtra = 0;
                                        wc.cbWndExtra = 0;
                                        wc.hInstance = hInst;
                                        wc.hIcon = 0x0;
                                        wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
                                        wc.hbrBackground = GetStockObject(BLACK_BRUSH);
                                        wc.lpszMenuName = 0x0;
                                        wc.lpszClassName = szTitle;
                                        if (RegisterClass(&wc) != 0x0) {
                                            wc.style = 0xb;
                                            wc.lpfnWndProc = (WNDPROC)ReportDlg;
                                            wc.cbClsExtra = 0;
                                            wc.cbWndExtra = 0;
                                            wc.hInstance = hInst;
                                            wc.hIcon = 0x0;
                                            wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
                                            wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                                            wc.lpszMenuName = 0x0;
                                            wc.lpszClassName = szReport;
                                            if (RegisterClass(&wc) != 0x0) {
                                                return 1;
                                            }
                                            return 0;
                                        }
                                        return 0;
                                    }
                                    return 0;
                                }
                                return 0;
                            }
                            return 0;
                        }
                        return 0;
                    }
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

void CreateChildWindows() {
    char    szData[100];
    POINT16 pt;
    char   *psz;
    char    szGame[15];

    if (idPlayer == -1) {
        CchGetString(idsStarsSHostMode, szWork);
        _wsprintf(szData, szWork, game.szName);
    } else {
        for (psz = &szBase[strlen(szBase) - 1]; psz > szBase && (int16_t)psz[-1] != '\\' && (int16_t)psz[-1] != ':'; psz--) {
        }
        szGame[8] = 0;
        strncpy(szGame, psz, 0x8);
        strlwr(szGame);
        _wsprintf(&szGame[strlen(szGame)], ".m%d", idPlayer + 1);
        _wsprintf(szData, "Stars! -- %s -- %s -- %s", game.szName, PszPlayerName(idPlayer, 0, 1, 0, 0, 0x0), szGame);
    }
    SetWindowText(hwndFrame, szData);
    if (idPlayer != -1) {
        if (hwndScanner != 0x0) {
            InvalidateRect(hwndScanner, 0x0, 1);
            yScanTop = 1000;
            xScanTop = 1000;
            SetScanScrollBars(hwndScanner);
        } else {
            hwndScanner = CreateWindow(szScan, 0x0, WS_CHILD | WS_VISIBLE, -200, -200, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndMine != 0x0) {
            InvalidateRect(hwndMine, 0x0, 1);
        } else {
            hwndMine = CreateWindow(szMine, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndPlanet != 0x0) {
            InvalidateRect(hwndPlanet, 0x0, 1);
        } else {
            hwndPlanet = CreateWindow(szPlanet, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndTb != 0x0) {
            InvalidateRect(hwndTb, 0x0, 1);
        } else {
            hwndTb = CreateWindow(szTb, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        }
        if (hwndMessage != 0x0) {
            DestroyWindow(hwndMessage);
        }
        hwndMessage = CreateWindow(szMessage, 0x0, WS_CHILD | WS_VISIBLE, -500, -500, 10, 10, hwndFrame, 0x0, hInst, 0x0);
        RefitFrameChildren();
    }
    return;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     i;
    HPALETTE    hpalSav;
    TEXTMETRIC  tm;
    int16_t     ich;
    POINT16     pt;
    char       *pch;
    char        szTemp[80];
    FARPROC     lpProc;
    int16_t     fRet;
    int32_t     lSerial;
    int16_t     fErrSav;
    int16_t     idCur;
    int16_t     id;
    uint16_t    uTimerIdOld;
    char        szExt[4];
    int16_t     iOffset;
    int16_t     idPlanet;
    RECT        rc;
    RECT        rc2;
    PAINTSTRUCT ps;
    int16_t     yOffset;
    HBRUSH      hbrSav;
    HCURSOR     hcs;
    POINT16     ptOld;
    int16_t     grSel;
    POINT16     ptAct;
    POINT16     ptD;
    POINT16     ptStart;
    POINT16     ptChg;
    uint16_t    t_merge_0db4_0001;
    HICON       t_merge_16e3_0001;
    POINT       t_pt_1a3c;
    POINT       t_pt_1a4c_1;

    switch (msg) {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        FCreateFonts(hdc);
        GetTextMetrics(hdc, &tm);
        dySysFont = tm.tmHeight;
        dySBar = (dyArial8 + 12) * 2;
        ReleaseDC(hwnd, hdc);
        InitTiles();
        EnsureTileSize(iWindowLayout == 2 ? 1 : 0);
        return 0;
    case WM_QUERYNEWPALETTE:
    MapIt:
        if (hwndTitle == 0x0) {
            hdc = GetDC(hwnd);
            hpalSav = SelectPalette(hdc, vhpal, 0);
            i = RealizePalette(hdc);
            SelectPalette(hdc, hpalSav, 0);
            ReleaseDC(hwnd, hdc);
            if (i == 0) {
                return 0;
            }
            InvalidateRect(hwnd, 0x0, 1);
            return 1;
        }
        return SendMessage(hwndTitle, msg, wParam, lParam);
    case WM_PALETTECHANGED:
        if ((HWND)wParam != hwnd)
            goto MapIt;
        return 0;
    case WM_INITMENU:
        InitializeMenu((HMENU)wParam);
        return 0;
    case WM_STARS_STARTUP:
        idPlayer = -1;
        if (ini.fCmdLine != 0x0) {
            ini.fCmdLine = 0x0;
            if (ini.fValidate == 0x0) {
                if (ini.fNewGame == 0x0) {
                    if (ini.fGen != 0x0) {
                        while (1) {
                            if ((ini.fWait != 0x0 || ini.fTry != 0x0) && CTurnsOutSafe() != 0) {
                                if (ini.fTry == 0x0)
                                    break;
                                if (ini.fBatch == 0x0 || lpchBatch >= lpchBatchMac)
                                    goto LExit;
                            } else {
                                EnsureAis();
                                FGenerateTurn();
                                if (ini.fBatch == 0x0 || lpchBatch >= lpchBatchMac) {
                                    if (ini.cTurnGen == 0)
                                        goto LExit;
                                    ini.cTurnGen = ini.cTurnGen - 1;
                                    continue;
                                }
                            }
                            DestroyCurGame();
                            pch = szBase;
                            while ((int16_t)*lpchBatch != 10 && lpchBatch != lpchBatchMac) {
                                *pch = *lpchBatch;
                                lpchBatch = lpchBatch + 1;
                                pch = pch + 1;
                            }
                            lpchBatch = lpchBatch + 1;
                            pch[-1] = 0;
                            ini.fStartupFile = 0x1;
                        }
                    }
                    CommandHandler(hwnd, 0xed9);
                    if (ini.fTry == 0x0) {
                        if (ini.fGen != 0x0)
                            goto LNop;
                        if (game.lid == 0)
                            goto LShowStartup;
                        if (idPlayer == -1 || (ini.fDumpPlanets == 0x0 && ini.fDumpFleets == 0x0 && ini.fDumpMap == 0x0)) {
                            ShowWindow(hwndFrame, SW_SHOW);
                            InitializeMenu(0x0);
                            PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
                            if (ini.fWait == 0x0)
                                goto LNop;
                            ini.fWait = 0x0;
                            CommandHandler(hwnd, 0x6a);
                            goto LNop;
                        }
                        if (ini.fDumpMap != 0x0) {
                            PostMessage(hwndFrame, WM_COMMAND, 0x55, 0);
                        }
                        if (ini.fDumpPlanets != 0x0) {
                            PostMessage(hwndFrame, WM_COMMAND, 0x54, 0);
                        }
                        if (ini.fDumpFleets != 0x0) {
                            PostMessage(hwndFrame, WM_COMMAND, 0x53, 0);
                        }
                    }
                } else if (vSerialNumber != 0) {
                    GenNewGameFromFile(szBase);
                }
            } else {
                fFileErrSilent = 1;
                ClearFile(7);
                if (FLoadGame(szBase, "hst") != 0) {
                    VerifyTurns();
                    DestroyCurGame();
                    EnsureAis();
                    _wsprintf(szTemp, "\"%s\" Year: %d", game.szName, game.turn + 0x960);
                    OutputSz(7, szTemp);
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgOut[i] + 1 <= 3) {
                            ich = _wsprintf(szTemp, "%d: ", i + 1);
                        } else {
                            ich = _wsprintf(szTemp, "Error: %d: ", i + 1);
                        }
                        if (gd.fNoHostNames == 0x0) {
                            ich = ich + _wsprintf(&szTemp[ich], "\"%s\" ", PszPlayerName(i, 1, 1, 1, 0, 0x0));
                        }
                        strcat(szTemp, PszGetCompressedString(rgOut[i] + 716));
                        if (rgplr[i].fHacker != 0x0) {
                            strcat(szTemp, " - HACKER");
                        }
                        OutputSz(7, szTemp);
                    }
                }
            }
        LExit:
            if (gd.fExitWindows == 0x0) {
                PostQuitMessage(vretExitValue);
                return 0;
            }
            ExitWindows((int32_t)vretExitValue, 0x0);
            return 0;
        }
    LShowStartup:
        if (hwndTitle == 0x0) {
            pt.x = GetSystemMetrics(SM_CXSCREEN);
            pt.y = GetSystemMetrics(SM_CYSCREEN);
            hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
            fFreeingTitle = 0;
        }
        ini.fStartupFile = 0x0;
        DestroyCurGame();
    LNop:
        if (vSerialNumber != 0 && memcmp(vrgbMachineConfig, vrgbEnvCur, 0xb) == 0) {
            return 0;
        }
        t_merge_0db4_0001 = vSerialNumber == 0 ? 0x0 : 0x1;
        szWork[200] = LOBYTE(t_merge_0db4_0001);
        lpProc = MakeProcInstance(MsgDlg, hInst);
        fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SERIAL_NUMBER), hwndTitle == 0x0 ? hwndFrame : hwndTitle, lpProc);
        FreeProcInstance(lpProc);
        if (fRet == 0) {
            vSerialNumber = 0;
            memcpy(vrgbMachineConfig, vrgbEnvCur, 0xb);
            PostQuitMessage(vretExitValue);
        } else if (FValidSerialNo(szWork, &lSerial) == 0) {
            if (vSerialNumber == 0) {
                memcpy(vrgbMachineConfig, vrgbEnvCur, 0xb);
                PostQuitMessage(vretExitValue);
            }
        } else {
            vSerialNumber = lSerial;
            memcpy(vrgbMachineConfig, vrgbEnvCur, 0xb);
        }
        WriteIniSettings();
        return 0;
    case WM_ENTERIDLE:
        if (gd.fTutorial != 0x0 && tutor.fChange != 0x0) {
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
        if (wParam != 0x2 && wParam != 0x0)
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
        gd.fDontDoLogFiles = 0x1;
        if (FLoadGame(szBase, "m1") != 0) {
            gd.fDontDoLogFiles = 0x0;
            fFileErrSilent = fErrSav;
            idPlayer = 0;
            if (wParam == 0x9ca) {
                gd.fGeneratingTurn = 0x1;
                _wsprintf(szWork, "%s.x1", szBase);
                if (FLoadLogFile(szWork) != 0) {
                    FRunLogFile();
                }
                gd.fGeneratingTurn = 0x0;
            }
            CreateChildWindows();
            SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
            if (wParam == 0x9ca) {
                SendMessage(hwndMessage, WM_KEYDOWN, 0x23, 0);
            }
            tutor.idt = 0;
            tutor.fTurnDone = 0x0;
            tutor.fAutoComplete = wParam == 0x9ca ? 0x1 : 0x0;
            AdvanceTutor();
            return 0;
        }
        fFileErrSilent = fErrSav;
        gd.fDontDoLogFiles = 0x0;
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) != 0xf030 && (wParam & 0xfff0) != 0xf120)
            goto Default;
        idCur = idPlayer;
        uTimerIdOld = uTimerId;
        if (uTimerId != 0x0) {
            KillTimer(0x0, uTimerId);
            uTimerId = 0x0;
            CreateChildWindows();
        }
        if (idPlayer == -1 || FNewTurnAvail(idPlayer) == 0) {
            if (uTimerIdOld == 0x0)
                goto Default;
            if (uTimerType != 0xd) {
                if (uTimerType == 0xe) {
                    id = AlertSz(PszFormatIds(idsTurnHasSubmittedChangesMadeAfterTurn, 0x0), MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
                    if (id != 6 || FMarkFile(dtLog, idPlayer, 2, 0) != 0) {
                        if (id == 2) {
                            PostMessage(hwndFrame, WM_COMMAND, 0x6a, 0);
                            return 1;
                        }
                    } else {
                        AlertSz(PszFormatIds(idsNewTurnCurrentlyGeneratedHostNewTurn, 0x0), MB_ICONHAND);
                        _wsprintf(szExt, MPCTD, idPlayer + 1);
                        DestroyCurGame();
                        if (FLoadGame(szBase, szExt) != 0) {
                            CreateChildWindows();
                        } else {
                            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
                        }
                    }
                }
                SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
                if (sel.pt.x <= 1000 || sel.pt.y <= 1000)
                    goto Default;
                CtrPointScan(sel.pt, 1);
                goto Default;
            }
            PostMessage(hwnd, WM_STARS_HOST, 0x0, 0);
            goto Default;
        }
        if (uTimerIdOld == 0x0) {
            id = AlertSz(PszFormatIds(idsNewTurnAvailableWouldLikeLoad, 0x0), MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);
        } else {
            AlertSz(PszFormatIds(idsNewTurnAvailable, 0x0), MB_ICONASTERISK);
            id = 6;
        }
        if (id != 6) {
            if (id == 2) {
                if (uTimerIdOld == 0x0) {
                    PostMessage(hwndFrame, WM_SYSCOMMAND, 0xf020, 0);
                } else {
                    PostMessage(hwndFrame, WM_COMMAND, 0x6a, 0);
                }
                return 1;
            }
        } else {
            _wsprintf(szExt, MPCTD, idPlayer + 1);
            DestroyCurGame();
            if (FLoadGame(szBase, szExt) != 0) {
                CreateChildWindows();
            } else {
                AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
            }
        }
        SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
        goto Default;
    case WM_CHAR:
        if (hwndScanner == 0x0 || (wParam != 0x2d && wParam != 0x2b)) {
            if (hwndMessage != 0x0) {
                switch (wParam) {
                case 0x2d:
                case 0x2b:
                case 0xd:
                    SendMessage(hwndMessage, WM_CHAR, wParam, lParam);
                    break;
                default:
                    goto L_1453;
                }
                return 0;
            }
        L_1453:
            if (hwndPlanet == 0x0 || (wParam != 0x66 && wParam != 0x46)) {
                if (hwndPlanet == 0x0 || sel.grobj != grobjPlanet || (wParam != 0x71 && wParam != 0x51)) {
                    if ((sel.grobj & 0x3) == 0x0) {
                        return 0;
                    }
                    iOffset = 0;
                    idPlanet = 0;
                    switch (wParam) {
                    case 0x6e:
                        iOffset = 1;
                        break;
                    case 0x4e:
                    case 0x50:
                        if (sel.grobj != grobjPlanet) {
                            iOffset = wParam == 0x4e ? 1 : -1;
                            break;
                        }
                        idPlanet = IdFindAdjStarbase(sel.pl.id, wParam == 0x4e ? 1 : 0);
                        break;
                    case 0x70:
                        iOffset = -1;
                        break;
                    case 0x72:
                    case 0x52:
                        if (sel.grobj == grobjFleet) {
                            ShipCommandProc(hwndPlanet, 0x0, (LPARAM)rghwndBtn[6]);
                        }
                    default:
                    }
                    if (iOffset == 0 && idPlanet == 0) {
                        return 0;
                    }
                    if (sel.grobj != grobjFleet) {
                        SelectAdjPlanet(iOffset, idPlanet);
                        return 0;
                    }
                    SelectAdjFleet(iOffset, idPlanet);
                    return 0;
                }
                ChangeProduction(0);
                return 0;
            }
            SendMessage(hwndPlanet, WM_CHAR, wParam, lParam);
            return 0;
        }
        SendMessage(hwndScanner, WM_CHAR, wParam, lParam);
        return 0;
    case WM_QUERYDRAGICON:
        if (idPlayer != -1) {
            if (uTimerId != 0x0) {
                return (LRESULT)hiconWait;
            }
            return (LRESULT)hiconStars;
        }
        return (LRESULT)hiconHost;
    case WM_ERASEBKGND:
        if (IsIconic(hwnd) == 0) {
            GetClientRect(hwnd, &rc);
            if (hwndScanner != 0x0) {
                GetClientRect(hwndScanner, &rc2);
                MapWindowPoints(hwndScanner, hwnd, (POINT *)&rc2, 0x2);
                ExcludeClipRect((HDC)wParam, rc2.left, rc2.top, rc2.right, rc2.bottom);
            }
            FillRect((HDC)wParam, &rc, hbrButtonFace);
            return 1;
        }
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrDesktop);
        return 0;
    case WM_PAINT:
        if (IsIconic(hwnd) == 0) {
            hdc = BeginPaint(hwnd, &ps);
            hbrSav = SelectObject(hdc, hbrButtonShadow);
            switch (iWindowLayout) {
            case 0:
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
            case 1:
            case 2:
                yOffset = gd.fToolbar == 0x0 ? 0 : 36;
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
        }
        hdc = BeginPaint(hwnd, &ps);
        if (idPlayer == -1 && game.lid != 0) {
            t_merge_16e3_0001 = hiconHost;
        } else if (uTimerId != 0x0) {
            t_merge_16e3_0001 = hiconWait;
        } else {
            t_merge_16e3_0001 = hiconStars;
        }
        DrawIcon(hdc, 2, 2, t_merge_16e3_0001);
        EndPaint(hwnd, &ps);
        return 0;
    case WM_SETCURSOR:
        hcs = 0x0;
        if (IsIconic(hwnd) != 0)
            goto Default;
        GetCursorPos(&t_pt_1a3c);
        pt = PointTo16(t_pt_1a3c);
        t_pt_1a4c_1 = PointFrom16(pt);
        ScreenToClient(hwndFrame, &t_pt_1a4c_1);
        pt = PointTo16(t_pt_1a4c_1);
        GetClientRect(hwnd, &rc);
        if (PtInRect(&rc, PointFrom16(pt)) == 0)
            goto Default;
        hcs = HcrsFromFrameWindowPt(pt, 0x0);
        if (hcs == 0x0)
            goto Default;
        SetCursor(hcs);
        return 1;
    case WM_LBUTTONDOWN:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (HcrsFromFrameWindowPt(pt, &grSel) == 0x0) {
            return 0;
        }
        hdc = GetDC(hwnd);
        hbrSav = SelectObject(hdc, hbr50Screen);
        ptD.y = 0;
        ptD.x = 0;
        InvertPaneBorder(hdc, grSel, ptD, 0x0);
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
        InvertPaneBorder(hdc, grSel, ptD, 0x0);
        ReleaseCapture();
        SelectObject(hdc, hbrSav);
        ReleaseDC(hwnd, hdc);
        if (ptAct.x == 0 && ptAct.y == 0) {
            return 0;
        }
        if ((grSel & 0x1) != 0x0) {
            if (iWindowLayout != 0) {
                vfs.dx2PlanWant = vfs.xTop + ptAct.x;
            } else {
                vfs.dxPlanWant = vfs.xTop + ptAct.x;
            }
        }
        if ((grSel & 0x2) != 0x0) {
            if (iWindowLayout != 0) {
                vfs.dy2MsgWant = vfs.dy - vfs.y1 - 8 - ptAct.y;
            } else {
                vfs.dyMsgWant = vfs.y2 - vfs.y1 - 8 - ptAct.y;
            }
        }
        if ((grSel & 0x4) != 0x0) {
            if (iWindowLayout != 0) {
                vfs.dy2MinWant = vfs.dy - vfs.y2 - 8 - ptAct.y;
            } else {
                vfs.dyMsgWant = vfs.y2 - vfs.y1 - 8 + ptAct.y;
                vfs.dyMinWant = vfs.dy - vfs.y2 - 8 - ptAct.y;
            }
        }
        InvalidateRect(hwnd, 0x0, 1);
        RefitFrameChildren();
        return 0;
    case WM_DESTROY:
        if (uTimerId != 0x0) {
            KillTimer(0x0, uTimerId);
        }
        WriteIniSettings();
        uTimerId = 0x0;
        if (gd.fHostMode != 0x0) {
            FMarkFile(dtHost, -1, 1, 0);
        }
        DestroyCurGame();
        if (gd.fExitWindows == 0x0) {
            PostQuitMessage(vretExitValue);
            return 0;
        }
        ExitWindows((int32_t)vretExitValue, 0x0);
        return 0;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    case WM_ACTIVATE:
        return 0;
    }
}

POINT16 InvertPaneBorder(HDC hdc, int16_t grSel, POINT16 dpt, POINT16 *pdptPrev) {
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
    if (pdptPrev != 0x0 && grSel != 1) {
        pt.x = dpt.x - pdptPrev->x;
        pt.y = dpt.y - pdptPrev->y;
        InvertPaneBorder(hdc, grSel, pt, 0x0);
    }
    switch (iWindowLayout) {
    case 0:
    default:
        dxScanMin = 100;
        dyPlanMin = 50;
        dyMsgCur = vfs.y2 - vfs.y1 - 8;
        dyAboveMinCur = vfs.y2 - vfs.y1 - 8;
        notMin = 1;
        dyMinAboveH2 = (0xd * dyArial8 >> 0x1) + 0xa;
        break;
    case 1:
    case 2:
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
    if ((grSel & 0x2) != 0x0) {
        if (vfs.y1 + dpt.y >= dyPlanMin) {
            dyMin = (0xd * dyArial8 >> 0x1) + 0xa;
            if (dyMsgCur - dpt.y < dyMin) {
                dpt.y = dyMsgCur - dyMin;
            }
        } else {
            dpt.y = dyPlanMin - vfs.y1;
        }
    }
    if ((grSel & 0x4) != 0x0) {
        if (dyAboveMinCur + dpt.y >= dyMinAboveH2) {
            dyMin = 13 * dyArial8 - 36;
            if (vfs.dy - vfs.y2 - 8 - dpt.y < dyMin) {
                dpt.y = vfs.dy - vfs.y2 - 8 - dyMin;
            }
        } else {
            dpt.y = (dyMinAboveH2 - dyAboveMinCur) * notMin;
        }
    }
    if (pdptPrev != 0x0) {
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
        if ((grSel & 0x2) == 0x0) {
            if ((grSel & 0x4) != 0x0) {
                if (dyAboveMinCur + dpt.y >= dyMinAboveH2) {
                    dyMin = 13 * dyArial8 - 36;
                    if (vfs.dy - vfs.y2 - 8 - dpt.y < dyMin) {
                        dpt.y = vfs.dy - vfs.y2 - 8 - dyMin;
                    }
                } else {
                    dpt.y = (dyMinAboveH2 - dyAboveMinCur) * notMin;
                }
            }
        } else if (vfs.y1 + dpt.y >= 50) {
            dyMin = (0xd * dyArial8 >> 0x1) + 0xa;
            if (dyMsgCur - dpt.y < dyMin) {
                dpt.y = dyMsgCur - dyMin;
            }
        } else {
            dpt.y = 50 - vfs.y1;
        }
        dptPrev.x = dptT.x - dpt.x;
        dptPrev.y = dptT.y - dpt.y;
        dpt = dptT;
    }
    if ((uint16_t)(grSel - 1) <= 6) {
        switch (grSel) {
        case 1:
        case 6:
            goto L_2164;
        case 2:
            dpt.x = 0;
            PatBlt(hdc, 0, vfs.y1 + dpt.y + 1, vfs.xTop + 1, 6, PATINVERT);
            break;
        case 4:
            dpt.x = 0;
            if (iWindowLayout != 0) {
                PatBlt(hdc, vfs.xTop + 7, vfs.y2 + dpt.y + 1, vfs.dx - vfs.xTop - 7, 6, PATINVERT);
                break;
            }
            PatBlt(hdc, 0, vfs.y2 + dpt.y + 1, vfs.xTop + 1, 6, PATINVERT);
            break;
        case 3:
            PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
            PatBlt(hdc, 0, vfs.y1 + dpt.y + 1, vfs.xTop + dpt.x + 1, 6, PATINVERT);
            break;
        case 5:
            PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
            if (iWindowLayout != 0) {
                PatBlt(hdc, vfs.xTop + dpt.x + 7, vfs.y2 + dpt.y + 1, vfs.dx - dpt.x - vfs.xTop - 7, 6, PATINVERT);
                break;
            }
            PatBlt(hdc, 0, vfs.y2 + dpt.y + 1, vfs.xTop + dpt.x + 1, 6, PATINVERT);
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
    if (pdptPrev != 0x0 && abs(dptPrev.x) < 6) {
        dChg = dptPrev.x;
        if (dChg != 0) {
            if (dChg >= 0) {
                x = vfs.xTop + dpt.x - dptPrev.x + 1;
            } else {
                x = vfs.xTop + dpt.x + 1;
                dChg = -dChg;
            }
            PatBlt(hdc, x, 0, dChg, vfs.dy, PATINVERT);
            PatBlt(hdc, x + 6, 0, dChg, vfs.dy, PATINVERT);
        }
    } else {
        if (pdptPrev != 0x0) {
            PatBlt(hdc, vfs.xTop + dpt.x - dptPrev.x + 1, 0, 6, vfs.dy, PATINVERT);
        }
        PatBlt(hdc, vfs.xTop + dpt.x + 1, 0, 6, vfs.dy, PATINVERT);
    }
    return dpt;
}

HCURSOR HcrsFromFrameWindowPt(POINT16 pt, int16_t *pgrSel) {
    HCURSOR hcs;
    int16_t fInHBar2;
    int16_t fInHBar1;
    int16_t fInVBar;
    int16_t t_merge_24e0_0001;
    int16_t t_merge_2513_0001;
    int16_t t_merge_254c_0001;
    int16_t t_merge_257f_0001;

    hcs = 0x0;
    if (pt.x < vfs.xTop || pt.x >= vfs.xTop + 8) {
        t_merge_24e0_0001 = 0;
    } else {
        t_merge_24e0_0001 = 1;
    }
    fInVBar = t_merge_24e0_0001;
    if (pt.x >= vfs.xTop + 8 || pt.y < vfs.y1 || pt.y >= vfs.y1 + 8) {
        t_merge_2513_0001 = 0;
    } else {
        t_merge_2513_0001 = 1;
    }
    fInHBar1 = t_merge_2513_0001;
    switch (iWindowLayout) {
    case 0:
    default:
        if (pt.x >= vfs.xTop + 8 || pt.y < vfs.y2 || pt.y >= vfs.y2 + 8) {
            t_merge_254c_0001 = 0;
        } else {
            t_merge_254c_0001 = 1;
        }
        fInHBar2 = t_merge_254c_0001;
        break;
    case 1:
    case 2:
        if (pt.x < vfs.xTop || pt.y < vfs.y2 || pt.y >= vfs.y2 + 8) {
            t_merge_257f_0001 = 0;
        } else {
            t_merge_257f_0001 = 1;
        }
        fInHBar2 = t_merge_257f_0001;
    }
    if (fInVBar == 0) {
        if (fInHBar1 != 0 || fInHBar2 != 0) {
            hcs = hcurResizeNS;
        }
    } else if (fInHBar1 == 0 && fInHBar2 == 0) {
        hcs = hcurResizeWE;
    } else {
        hcs = hcurResize4Way;
    }
    if (pgrSel != 0x0) {
        *pgrSel = fInHBar1 * 2 + fInVBar + fInHBar2 * 4;
    }
    return hcs;
}

void RestoreSelection() {
    PLANET *lppl;

    if (ini.idPlayer != idPlayer || ini.lid != game.lid) {
        ini.grobjSel = 0x1;
        ini.iObjSel = rgplr[idPlayer].idPlanetHome;
    } else {
        if (ini.grobjSel != 0x2) {
            if (ini.grobjSel == 0x1) {
                lppl = LpplFromId(ini.iObjSel);
                if (lppl != 0x0 && lppl->iPlayer == idPlayer) {
                    SelectAdjPlanet(0, ini.iObjSel);
                    ini.grobjSel = 0x0;
                } else {
                    ini.iObjSel = rgplr[idPlayer].idPlanetHome;
                }
            }
        } else if (LpflFromId(ini.iObjSel) != 0x0) {
            SelectAdjFleet(0, ini.iObjSel);
            ini.grobjSel = 0x0;
        } else {
            ini.grobjSel = 0x1;
            ini.iObjSel = rgplr[idPlayer].idPlanetHome;
        }
        if (ini.turn == game.turn && ini.iMsg > 0) {
            iMsgCur = ini.iMsg - 1;
            if (iMsgCur >= cMsg + vcmsgplrIn) {
                iMsgCur = -1;
            }
            iMsgCur = IMsgNext(0);
            gd.fGotoVCR = 0x0;
            SetMsgTitle(hwndMessage);
            InvalidateRect(hwndMessage, 0x0, 1);
            if (gd.fTutorial != 0x0) {
                tutor.fChange = 0x1;
                AdvanceTutor();
            }
        }
    }
    if (ini.grobjSel != 0x0) {
        if (ini.iObjSel == -1) {
            FFindSomethingAndSelectIt();
        } else {
            lppl = LpplFromId(ini.iObjSel);
            if (lppl != 0x0 && lppl->iPlayer == idPlayer) {
                SelectAdjPlanet(0, ini.iObjSel);
            } else {
                FFindSomethingAndSelectIt();
            }
        }
        ini.grobjSel = 0x0;
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
    int16_t t_2a0c;

    iPass = 0;
    PushRandom(11, 17);
    Randomize(lSerial);
    RawStore32(rgbRaw, lSerial);
    memcpy(&rgbRaw[4], pbEnv, 0xb);
    iRaw = 15;
    for (i = 0; i < 11; i++) {
        for (j = pbEnv[i]; j > 0; j--) {
            Random(16);
        }
        if (iPass != 0) {
            t_2945 = iRaw;
            iRaw = iRaw + 1;
            rgbRaw[t_2945] = rgbRaw[t_2945] | LOBYTE(Random(16) << 0x4 & 0xff);
        } else {
            rgbRaw[iRaw] = LOBYTE(Random(16));
        }
        iPass = iPass + 0x1 & 0x1;
    }
    bXor = 0x0;
    for (i = 0; i < 15; i++) {
        bXor = bXor ^ LOBYTE(rgbRaw[i]);
    }
    t_29a3 = iRaw;
    iRaw = iRaw + 1;
    rgbRaw[t_29a3] = rgbRaw[t_29a3] | LOBYTE(bXor << 0x4);
    PopRandom();
    for (i = 0; i < 21; i++) {
        rgbRaw2[i] = rgbRaw[vrgbShuffleSerial[i]];
    }
    iRaw = 0;
    cBits = 0;
    lTank = 0;
    for (i = 0; i < 28; i++) {
        if (cBits < 6) {
            t_2a0c = iRaw;
            iRaw = iRaw + 1;
            lTank = lTank | (int32_t)(rgbRaw2[t_2a0c] << cBits);
            cBits = cBits + 8;
        }
        b64 = LOBYTE(LOWORD(lTank) & 0x3f);
        lTank = (int32_t)(lTank >> 0x6);
        cBits = cBits - 6;
        if (b64 >= 0x1a) {
            if (b64 >= 0x34) {
                if (b64 >= 0x3e) {
                    if (b64 != 0x3e) {
                        *pszOut = '*';
                    } else {
                        *pszOut = '-';
                    }
                } else {
                    *pszOut = LOBYTE(b64 - 0x4);
                }
            } else {
                *pszOut = LOBYTE(b64 + 0x47);
            }
        } else {
            *pszOut = LOBYTE(b64 + 0x41);
        }
        pszOut = pszOut + 1;
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
    int16_t  t_2c01;
    uint16_t t_scratch_m48_2;
    uint16_t t_scratch_m48_3;

    iPass = 0;
    *plSerial = 0;
    memset(pbEnv, 0, 0xb);
    iRaw = 0;
    cBits = 0;
    lTank = 0;
    for (i = 0; i < 21; i++) {
        while (cBits < 8) {
            if ((int16_t)*pszIn < 'A' || (int16_t)*pszIn > 'Z') {
                if ((int16_t)*pszIn < 'a' || (int16_t)*pszIn > 'z') {
                    if ((int16_t)*pszIn < '0' || (int16_t)*pszIn > '9') {
                        if ((int16_t)*pszIn != '-') {
                            b64 = 0x3f;
                        } else {
                            b64 = 0x3e;
                        }
                    } else {
                        b64 = LOBYTE((int16_t)*pszIn + 4);
                    }
                } else {
                    b64 = LOBYTE((int16_t)*pszIn - 71);
                }
            } else {
                b64 = LOBYTE((int16_t)*pszIn - 65);
            }
            lTank = lTank | (int32_t)(b64 << cBits);
            cBits = cBits + 6;
            pszIn = pszIn + 1;
        }
        t_2c01 = iRaw;
        iRaw = iRaw + 1;
        rgbRaw2[t_2c01] = LOBYTE(LOWORD(lTank) & 0xff);
        cBits = cBits - 8;
        lTank = (int32_t)(lTank >> 0x8);
    }
    for (i = 0; i < 21; i++) {
        rgbRaw[vrgbShuffleSerial[i]] = LOBYTE((int16_t)(((uint16_t)i & 0xff00) | ((uint16_t)rgbRaw2[i] & 0xff)));
    }
    lSerial = RawLoad32(rgbRaw);
    if (FValidSerialLong(lSerial) != 0) {
        fSuccess = 1;
        PushRandom(11, 17);
        Randomize(lSerial);
        iRaw = 15;
        for (i = 0; i < 11; i++) {
            for (j = rgbRaw[i + 4]; j > 0; j--) {
                Random(16);
            }
            if (iPass != 0) {
                t_scratch_m48_3 = rgbRaw[iRaw] >> 0x4;
                if (t_scratch_m48_3 != (Random(16) & 0xff)) {
                    fSuccess = 0;
                }
                iRaw = iRaw + 1;
            } else {
                t_scratch_m48_2 = rgbRaw[iRaw] & 0xf;
                if (t_scratch_m48_2 != (Random(16) & 0xff)) {
                    fSuccess = 0;
                }
            }
            iPass = iPass + 0x1 & 0x1;
        }
        bXor = 0x0;
        for (i = 0; i < 15; i++) {
            bXor = bXor ^ LOBYTE(rgbRaw[i]);
        }
        if (rgbRaw[iRaw] >> 0x4 != (bXor & 0xf)) {
            fSuccess = 0;
        }
        PopRandom();
        if (fSuccess != 0) {
            *plSerial = lSerial;
            memcpy(pbEnv, &rgbRaw[4], 0xb);
        }
        return fSuccess;
    }
    return 0;
}

int16_t FFindSomethingAndSelectIt() {
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    FLEET  *lpfl;

    lppl = LpplFromId(rgplr[idPlayer].idPlanetHome);
    if (lppl == 0x0 || lppl->iPlayer != idPlayer) {
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac && lppl->iPlayer != idPlayer; lppl++) {
        }
        if (lppl == lpplMac) {
            lppl = 0x0;
        }
    }
    if (lppl != 0x0) {
        SelectAdjPlanet(0, lppl->id);
        return 1;
    }
    i = 0;
    while (1) {
        if (i >= cFleet) {
            return 0;
        }
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0) {
            return 0;
        }
        if (lpfl->iPlayer == idPlayer)
            break;
        i = i + 1;
    }
    SelectAdjFleet(0, lpfl->id);
    return 1;
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
    uint16_t     t_merge_3424_0001;

    if (GET_WM_COMMAND_ID(wParam, 0) < 0x3a98 || GET_WM_COMMAND_ID(wParam, 0) >= 0x3afc) {
        switch (GET_WM_COMMAND_ID(wParam, 0)) {
        case 0x63:
            lpProc = MakeProcInstance(About, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0xee2:
            SendMessage(hwnd, WM_CLOSE, 0x0, 0);
            break;
        case 0x82:
        case 0x83:
        case 0x84:
            if (idPlayer == -1)
                break;
            iWindowLayout = GET_WM_COMMAND_ID(wParam, 0) - 130;
            InvalidateRect(hwndFrame, 0x0, 1);
            EnsureTileSize(iWindowLayout == 2 ? 1 : 0);
            RefitFrameChildren();
            break;
        case 0x9c4:
        case 0x9c5:
            if (gd.fTutorial == 0x0) {
                StartTutor(0);
                break;
            }
            ShowTutor(1);
            break;
        case 0x6e:
        case 0xed8:
            if (gd.fTutorial != 0x0 && FAskKillTutor() == 0)
                break;
            NewGameWizard(hwnd, 0);
            break;
        case 0x10cc:
        case 0x10cd:
        case 0x10ce:
        case 0x10cf:
        case 0x10d0:
        case 0x10d1:
        case 0x10d2:
        case 0x10d3:
        case 0x10d4:
            if ((gd.fTutorial != 0x0 && FAskKillTutor() == 0) || vrgszMRU == 0x0 || (int16_t)vrgszMRU[(GET_WM_COMMAND_ID(wParam, 0) - 4300) * 256] == 0)
                break;
            iplrOld = idPlayer;
            fstrcpy(szT, vrgszMRU + 256 * (GET_WM_COMMAND_ID(wParam, 0) - 4300));
            psz = strrchr(szT, 46);
            if (psz == 0x0 || access(szT, 0) == -1) {
                strcpy(szWork, szT);
                AlertSz(PszFormatIds(idsCantOpenFile, 0x0), MB_ICONHAND);
                break;
            }
            ini.fStartupFile = 0x1;
            DestroyCurGame();
            strcpy(szBase, szT);
            if (FOpenGame(hwnd, 0) <= 0)
                break;
            InitializeMenu(0x0);
            CreateChildWindows();
            if (uTimerId == 0x0) {
                PostMessage(hwnd, WM_COMMAND, 0xfa1, 0);
            }
            if (game.fTutorial == 0x0 || idPlayer != 0)
                break;
            StartTutor(0);
            break;
        case 0x81:
            vplr = vrgplrDef[0];
            RaceCreationWizard(hwnd, 0, 0);
            break;
        case 0xb3:
            hmenu = GetASubMenu(hwnd, 1);
            gd.fToolbar = gd.fToolbar == 0x0 ? 0x1 : 0x0;
            CheckMenuItem(hmenu, 0xb3, gd.fToolbar == 0x0 ? 0x0 : 0x8);
            RefitFrameChildren();
            break;
        case 0xd5:
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
                AlertSz(PszFormatIds(idsUnablePrintGameMapPrinterMayOff, 0x0), MB_ICONHAND);
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
            t_merge_3424_0001 = cPageX <= cPageY ? 0x0 : 0x1;
            if (t_merge_3424_0001 == (dxMax <= dyMax ? 0x0 : 0x1)) {
                i = cPageX;
                cPageX = cPageY;
                cPageY = i;
            }
            ldx = (uint32_t)((int32_t)dxMax * (int32_t)cPageX);
            ldy = (uint32_t)((int32_t)dyMax * (int32_t)cPageY);
            if (ldx > 32000) {
                ldx = 32000;
            }
            if (ldy > 32000) {
                ldy = 32000;
            }
            if (ldx < ldy) {
                if (ldy - (int32_t)dyDPI < ldx) {
                    ldx = ldy - (int32_t)dyDPI;
                }
                dSize = LOWORD(ldx);
                ptLegendA.x = dMargin;
                ptLegendA.y = dSize + dMargin;
                ptLegendB.x = (int32_t)dSize / 2;
                ptLegendB.y = ptLegendA.y;
            } else {
                if (ldx - (int32_t)((int32_t)(3 * dxDPI) / 0x2) < ldy) {
                    ldy = ldx - (int32_t)((int32_t)(3 * dxDPI) / 0x2);
                }
                dSize = LOWORD(ldy);
                ptLegendA.x = dSize + dMargin;
                ptLegendA.y = dMargin;
                ptLegendB.x = ptLegendA.x;
                ptLegendB.y = (int32_t)dSize / 2;
            }
            rc.bottom = dSize;
            rc.right = dSize;
            dSize = dSize - dMargin * 2;
            for (xPage = 0; xPage < cPageX; xPage++) {
                for (yPage = 0; yPage < cPageY; yPage++) {
                    xOff = -dxMax * xPage;
                    yOff = -dyMax * yPage;
                    cch = CchGetString(idsStarsUniverseMap, szWork);
                    Escape(pd.hDC, 10, cch, szWork, 0x0);
                    Rectangle(pd.hDC, xOff, yOff, xOff + rc.right, yOff + rc.bottom);
                    if (hfontPrint == 0x0) {
                        hfontSav = 0x0;
                    } else {
                        hfontSav = SelectObject(pd.hDC, hfontPrint);
                    }
                    y = ptLegendA.y + yOff;
                    cch = CchGetString(idsStarsUniverseMap, szWork);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, szWork, cch);
                    y = y + dyPrint;
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, game.szName, strlen(game.szName));
                    y = y + dyPrint;
                    psz = PszPlayerName(idPlayer, 1, 1, 1, 0, 0x0);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, psz, strlen(psz));
                    y = y + dyPrint;
                    cch = _wsprintf(szWork, PszGetCompressedString(idsYearD), game.turn + 0x960);
                    TextOut(pd.hDC, ptLegendA.x + xOff, y, szWork, cch);
                    if ((grbitScan & 0xf) != 0x5) {
                        y = ptLegendB.y + yOff;
                        for (i = 0; i < 5; i++) {
                            cch = CchGetString(i + 1314, szWork);
                            TextOut(pd.hDC, ptLegendB.x + xOff, y, szWork, cch);
                            y = y + dyPrint;
                        }
                        y = ptLegendB.y + yOff;
                        if (hfontPrintTiny != 0x0) {
                            SelectObject(pd.hDC, hfontPrintTiny);
                        }
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int32_t)(3 * dyPrint) / 2 + y - (int32_t)dyPrintTiny / 2, "|", 1);
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int32_t)(5 * dyPrint) / 2 + y - (int32_t)dyPrintTiny / 2, "+", 1);
                        CtrTextOut(pd.hDC, ptLegendB.x + xOff, (int32_t)(9 * dyPrint) / 2 + y + 8 - dyPrintTiny, "2", 1);
                        if (hfontPrint != 0x0) {
                            SelectObject(pd.hDC, hfontPrint);
                        }
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, y - 4 + (int32_t)dyPrint / 2, 1);
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, y - 4 + (int32_t)(7 * dyPrint) / 2, 0);
                        DrawPlanetPrintDot(pd.hDC, ptLegendB.x + xOff, (int32_t)(9 * dyPrint) / 2 + y + 8, 0);
                    }
                    if ((grbitScan & 0xf) != 0x5) {
                        if (hfontPrintTiny != 0x0) {
                            SelectObject(pd.hDC, hfontPrintTiny);
                        }
                        lppl = lpPlanets;
                        lpplMac = lpPlanets + cPlanet;
                        for (; lppl < lpplMac; lppl++) {
                            x = (int32_t)(rgptPlan[lppl->id].x - 1000);
                            y = (int32_t)(dGalInv - 1000 - rgptPlan[lppl->id].y);
                            x = (int32_t)((int32_t)(x * (int32_t)dSize) / (int32_t)dGal) + (int32_t)dMargin + (int32_t)xOff;
                            y = (int32_t)((int32_t)(y * (int32_t)dSize) / (int32_t)dGal) + (int32_t)dMargin + (int32_t)yOff;
                            if (lppl->iPlayer != idPlayer) {
                                if (lppl->iPlayer != -1) {
                                    cch = _wsprintf(szWork, PCTD, lppl->iPlayer + 1);
                                    CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) - dyPrintTiny, szWork, cch);
                                }
                            } else {
                                if (lppl->fStarbase != 0x0) {
                                    CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) + 4 - dyPrintTiny,
                                               rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef == ihuldefOrbitalFort ? "|" : "+", 1);
                                }
                                DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 1);
                            }
                        }
                    }
                    if (hfontPrint != 0x0) {
                        SelectObject(pd.hDC, hfontPrint);
                    }
                    for (i = 0; i < game.cPlanMax; i++) {
                        x = (int32_t)(rgptPlan[i].x - 1000);
                        y = (int32_t)(dGalInv - 1000 - rgptPlan[i].y);
                        x = (int32_t)((int32_t)(x * (int32_t)dSize) / (int32_t)dGal) + (int32_t)dMargin + (int32_t)xOff;
                        y = (int32_t)((int32_t)(y * (int32_t)dSize) / (int32_t)dGal) + (int32_t)dMargin + (int32_t)yOff;
                        DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 0);
                        if ((grbitScan & 0x400) != 0x0) {
                            CtrTextOut(pd.hDC, LOWORD(x), LOWORD(y) + 14, PszGetPlanetName(i), 0);
                        }
                    }
                    if (hfontSav != 0x0) {
                        SelectObject(pd.hDC, hfontSav);
                    }
                    Escape(pd.hDC, 1, 0, 0x0, 0x0);
                }
            }
            Escape(pd.hDC, 11, 0, 0x0, 0x0);
            if (hfontPrint != 0x0) {
                DeleteObject(hfontPrint);
            }
            if (hfontPrintTiny != 0x0) {
                DeleteObject(hfontPrintTiny);
            }
            DeleteDC(pd.hDC);
            if (pd.hDevMode != 0x0) {
                GlobalFree(pd.hDevMode);
            }
            if (pd.hDevNames == 0x0)
                break;
            GlobalFree(pd.hDevNames);
            break;
        case 0x98d:
            hmenu = GetASubMenu(hwnd, 1);
            grbitScan = grbitScan ^ 0x2000;
            CheckMenuItem(hmenu, 0x98d, (grbitScan & 0x2000) == 0x0 ? 0x0 : 0x8);
            gd.fChgScanner = 0x1;
            if ((grbitScan & 0x1400) == 0x0)
                break;
            InvalidateRect(hwndScanner, 0x0, 0);
            break;
        case 0x1068:
        case 0x1069:
            if (idPlayer == -1)
                break;
            lpProc = MakeProcInstance(FindDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0x5f:
        case 0x60:
            if (idPlayer == -1)
                break;
            lpProc = MakeProcInstance(ScoreXDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SCORE), hwnd, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0x69:
            if (game.fSinglePlr == 0x0 &&
                AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, 0x0), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES)
                break;
        case 0x6c:
        case 0x5208:
        case 0x5209:
        case 0x520a:
            idCur = idPlayer;
            switch (GET_WM_COMMAND_ID(wParam, 0)) {
            case 0x5209:
                iPassCnt = 100;
                break;
            case 0x520a:
                iPassCnt = 1000;
                break;
            case 0x5208:
                iPassCnt = 10;
                break;
            default:
                iPassCnt = 0;
            }
            if (game.fSinglePlr != 0x0 && iPassCnt != 0) {
                _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), iPassCnt);
                if (MessageBox(GetFocus(), szWork, "Stars!", MB_YESNO | MB_ICONEXCLAMATION | MB_TASKMODAL) != IDYES)
                    break;
            }
            if (idPlayer == -1)
                break;
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
                if (tutor.fTurnDone == 0x0) {
                    AlertSz(PszFormatIds(idsTutorialTurnWillGeneratedHaveYetCompleted, 0x0), MB_ICONHAND);
                    break;
                }
                ShowTutor(0);
                if (game.turn <= 0x23) {
                    Randomize(0x499602d2);
                    FWriteHistFile(idCur);
                    if (FWriteTutorialMFile(game.turn + 1) != 0) {
                        if (game.turn != 0x23 || FWriteTutorialMFile(game.turn + 2) != 0) {
                            strcpy(szWork, szBase);
                            strcat(szWork, ".x1");
                            remove(szWork);
                            DirtyGame(0);
                            ShowProgressGauge();
                            ti.dwSize = 0xc;
                            TimerCount(&ti);
                            dwTickBase = ti.dwmsSinceStart;
                            do {
                                UpdateProgressGauge((LOWORD(dwTickCur) - LOWORD(dwTickBase)) * 2);
                                TimerCount(&ti);
                                dwTickCur = ti.dwmsSinceStart;
                            } while (dwTickCur >= dwTickBase && dwTickCur < dwTickBase + 0x1f4);
                            goto LTutorialFinishUp;
                        }
                        AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
                        break;
                    }
                    AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
                    break;
                }
            }
            if (game.fSinglePlr == 0x0)
                goto LWaitForTurn;
            idsFileError = -1;
            if (FCheckFile(dtHost, -1, 0x1) != 0) {
                if (FBadFileError(idsFileError) == 0) {
                    if (AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, 0x0), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) == IDYES)
                        goto LWaitForTurn;
                    break;
                }
                AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
                break;
            }
            hcurSav = SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f02)));
            FWriteLogFile(szBase, idCur);
            FWriteHistFile(idCur);
            while (1) {
                ShowProgressGauge();
                EnsureAis();
                FGenerateTurn();
                switch (GET_WM_COMMAND_ID(wParam, 0)) {
                case 0x5208:
                case 0x5209:
                case 0x520a:
                    iPassCnt = iPassCnt - 1;
                    if (iPassCnt > 0) {
                        HideProgressGauge();
                        if (GetAsyncKeyState(16) >= 0 || GetAsyncKeyState(17) >= 0)
                            continue;
                    }
                default:
                }
                break;
            }
            iPassCnt = 0;
        LTutorialFinishUp:
            DestroyCurGame();
            _wsprintf(szExt, MPCTD, idCur + 1);
            if (FLoadGame(szBase, szExt) != 0) {
                HideProgressGauge();
                idPlayer = idCur;
                CreateChildWindows();
                SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
                SetCursor(hcurSav);
                if (gd.fTutorial == 0x0)
                    break;
                tutor.fTurnDone = 0x0;
                tutor.fAutoComplete = 0x0;
                AdvanceTutor();
                break;
            }
            SetCursor(hcurSav);
            HideProgressGauge();
            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
            break;
        case 0x88:
        case 0x100:
            if (game.lid == 0 || idPlayer == -1)
                break;
            if (hwndBrowser != 0x0) {
                mf = 0;
                DestroyWindow(hwndBrowser);
            } else {
                mf = 8;
                CreateDialog(hInst, MAKEINTRESOURCE(IDD_BROWSER), hwndFrame, lpfnBrowserDlgProc);
            }
            hmenu = GetASubMenu(hwnd, 5);
            CheckMenuItem(hmenu, 0x100, mf);
            break;
        case 0x8fe:
            if (hwndReportDlg != 0x0) {
                if (vprptCur != &vrptPlanet) {
                    if (vprptCur != &vrptFleet) {
                        wParam = 0x901;
                    } else {
                        wParam = 0x900;
                    }
                } else {
                    wParam = 0x8ff;
                }
            } else {
                wParam = 0x8fd;
            }
        case 0x8fd:
        case 0x8ff:
        case 0x900:
        case 0x901:
            cObj = 0;
            if (game.lid == 0 || idPlayer == -1)
                break;
            hmenu = GetASubMenu(hwnd, 4);
            while (hwndReportDlg != 0x0) {
                if (GET_WM_COMMAND_ID(wParam, 0) == 0x901 && vprptCur == &vrptBattle) {
                    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
                } else {
                    ids = idsPlayerLogFileAppearsCorruptUnableLoad;
                }
                mf = 0;
                DestroyWindow(hwndReportDlg);
                CheckMenuItem(hmenu, GET_WM_COMMAND_ID(wParam, 0) == 0x8ff ? 0x8ff : 0x8fd, mf);
                if (ids != idsPlayerLogFileAppearsCorruptUnableLoad) {
                    return;
                }
            }
            mf = 8;
            switch (GET_WM_COMMAND_ID(wParam, 0)) {
            case 0x8ff:
                ids = idsFleetSummaryReportDFleetC;
                vprptCur = &vrptFleet;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0x0)
                        break;
                    if (lpfl->iPlayer == idPlayer) {
                        cObj = cObj + 1;
                    }
                }
                break;
            case 0x900:
                ids = idsOthersFleetsSummaryReportDFleetC;
                vprptCur = &vrptEFleet;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0x0)
                        break;
                    if (lpfl->iPlayer != idPlayer) {
                        cObj = cObj + 1;
                    }
                }
                break;
            case 0x901:
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
                        cObj = cObj + 1;
                    }
                }
            }
            psz = PszGetCompressedString(ids);
            _wsprintf(szWork, psz, cObj, cObj == 1 ? 0x20 : 0x73);
            hwndReportDlg = CreateWindow(szReport, szWork, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 100, 100,
                                         hwndFrame, 0x0, hInst, 0x0);
            SetWindowPos(hwndReportDlg, 0x0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
            CheckMenuItem(hmenu, GET_WM_COMMAND_ID(wParam, 0), mf);
            break;
        case 0x9c1:
        case 0x9c2:
            WinHelp(hwnd, szHelpFile, 0x1, (uint32_t)(GET_WM_COMMAND_ID(wParam, 0) == 0x9c2 ? 0x1195 : 0x32ca));
            break;
        case 0x8a:
        case 0x101:
            WinHelp(hwnd, szHelpFile, 0x3, 0x0);
            break;
        case 0x6a:
        case 0x7da:
        LWaitForTurn:
            if (lpPlanets == 0x0 || idPlayer == -1 || game.fSinglePlr != 0x0)
                break;
            if (FNewTurnAvail(idPlayer) == 0) {
                gd.fSubmit = 0x1;
                FWriteLogFile(szBase, idPlayer);
                FWriteHistFile(idPlayer);
                SetWindowText(hwndFrame, PszGetCompressedString(idsWaitingNewTurn));
                ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
                uTimerId = SetTimer(0x0, 0xe, 0x2710, lpfnHostTimerProc);
                uTimerType = 0xe;
                HostTimerProc(0x0, WM_NULL, uTimerId, 0x0);
                break;
            }
            goto LNewTurnAvail;
        case 0x428:
            wParam = 0xedb;
        case 0x6f:
        case 0xeda:
        case 0xedb:
            if (hwndScanner != 0x0) {
                if (idPlayer != -1) {
                    if (FNewTurnAvail(idPlayer) != 0)
                        goto LNewTurnAvail;
                    gd.fSubmit = GET_WM_COMMAND_ID(wParam, 0) == 0xedb ? 0x1 : 0x0;
                    FWriteLogFile(szBase, idPlayer);
                    FWriteHistFile(idPlayer);
                    break;
                }
                FWriteDataFile(szBase, idPlayer, 0);
                break;
            }
            AlertSz(PszFormatIds(idsGameCurrentlyLoaded, 0x0), MB_ICONHAND);
            break;
        case 0x6d:
        case 0xed9:
            if ((gd.fTutorial != 0x0 && FAskKillTutor() == 0) || FOpenGame(hwnd, 0) <= 0)
                break;
            InitializeMenu(0x0);
            if (uTimerId == 0x0) {
                PostMessage(hwnd, WM_COMMAND, 0xfa1, 0);
            }
            if (game.fTutorial == 0x0 || idPlayer != 0)
                break;
            StartTutor(0);
            break;
        case 0xfa:
        case 0xfb:
        case 0xfc:
        case 0xfd:
            if (hwndTitle == 0x0)
                break;
            PostMessage(hwndTitle, WM_COMMAND, GET_WM_COMMAND_ID(wParam, 0) - 250, 0);
            break;
        case 0x71:
            if (gd.fTutorial != 0x0 && FAskKillTutor() == 0)
                break;
            WriteIniSettings();
            DestroyCurGame();
            strcpy(szBase, szWork);
            ini.grobjSel = 0x0;
            ini.iObjSel = 0;
            ini.idPlayer = -1;
            InitializeMenu(0x0);
            pt.x = GetSystemMetrics(SM_CXSCREEN);
            pt.y = GetSystemMetrics(SM_CYSCREEN);
            hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
            fFreeingTitle = 0;
            ShowWindow(hwndFrame, SW_HIDE);
            break;
        case 0x7e:
        case 0x87:
            if (game.lid == 0 || idPlayer == -1)
                break;
            lpProc = MakeProcInstance(ResearchDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RESEARCH), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == 0 || sel.grobj != grobjPlanet)
                break;
            if (sel.pl.lpplprod != 0x0) {
                FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);
            }
            DrawPlanShip(0x0, 72);
            break;
        case 0x7db:
        case 0x7dc:
            if (game.lid == 0 || idPlayer == -1)
                break;
            lpProc = MakeProcInstance(BattlePlansDlg, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_BATTLE_PLANS), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0x7d9:
        case 0x7de:
            if (game.lid == 0 || idPlayer == -1 || game.fSinglePlr != 0x0)
                break;
            lpProc = MakeProcInstance(RelationsDlg, hInst);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_RELATIONS), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0x9c:
        case 0x9d:
            if (game.lid == 0 || idPlayer == -1)
                break;
            vplr = rgplr[idPlayer];
            RaceCreationWizard(hwnd, 1, 0);
            break;
        case 0x9e:
        case 0x9f:
            if (game.lid == 0 || idPlayer == -1)
                break;
            NewGameWizard(hwnd, 1);
            break;
        case 0x7d:
        case 0x89:
            if (game.lid == 0 || idPlayer == -1)
                break;
            pt.x = 610;
            pt.y = 450;
            if (hwndPopup != 0x0) {
                SendMessage(hwndPopup, WM_RBUTTONUP, 0x0, 0);
            }
            ShipBuilder(pt);
            break;
        case 0x55:
            DumpUniverse();
            break;
        case 0x54:
            DumpPlanets();
            break;
        case 0x53:
            DumpFleets();
            break;
        case 0x10e:
            if (game.lid == 0 || idPlayer == -1 || ((game.fSinglePlr != 0x0 && lSaltCur <= 0) || FCheckPassword() == 0))
                break;
            lpProc = MakeProcInstance(NewPasswordDlg, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW_PASSWORD), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            break;
        case 0xf3d:
        case 0xf3e:
        case 0xf3f:
        case 0xf40:
        case 0xf41:
        case 0xf42:
        case 0xf43:
        case 0xf44:
        case 0xf45:
            if (hwndScanner != 0x0) {
                hmenu = GetASubMenu(hwnd, 1);
                hmenu = GetSubMenu(hmenu, 3);
                CheckMenuItem(hmenu, iScanZoom + 4, 0x400);
                GetClientRect(hwndScanner, &rc);
                rc.right = ScanToPt(rc.right) >> 0x1;
                rc.bottom = ScanToPt(rc.bottom) >> 0x1;
                dx = xScanTop;
                dy = dGalInv - yScanTop;
                iScanZoom = GET_WM_COMMAND_ID(wParam, 0) - 3905;
                CheckMenuItem(hmenu, iScanZoom + 4, 0x408);
                DrawMenuBar(hwnd);
                SetScanScrollBars(hwndScanner);
                InvalidateRect(hwndScanner, 0x0, 1);
                if (sel.scan.grobj == grobjNone) {
                    pt.x = dx + rc.right;
                    pt.y = dy - rc.bottom;
                } else {
                    pt = sel.scan.pt;
                }
                CtrPointScan(pt, 0);
                break;
            }
            AlertSz(PszFormatIds(idsCantChangeZoomFactorUntilGameOpen, 0x0), MB_ICONHAND);
            break;
        case 0xfa1:
            if (hwndScanner == 0x0)
                break;
            gd.fNoScannerDraw = 0x1;
            RestoreSelection();
            RefitFrameChildren();
            gd.fNoScannerDraw = 0x0;
            InvalidateRect(hwndScanner, 0x0, 1);
            UpdateWindow(hwndScanner);
            break;
        case 0x67:
        case 0x68:
            if (sel.grobj == grobjFleet && GetFocus() != hwndOrderED) {
                DeleteCurWayPoint(GET_WM_COMMAND_ID(wParam, 0) == 0x67 ? 1 : 0);
            }
        default:
            DefWindowProc(hwnd, 0x111, wParam, 0);
        }
        return;
    LNewTurnAvail:
        FWriteHistFile(idPlayer);
        if (game.fDirty == 0) {
            AlertSz(PszFormatIds(idsNewTurnAvailable, 0x0), MB_ICONASTERISK);
        } else {
            id = AlertSz(PszFormatIds(idsSorryTurnHasAlreadyGeneratedAnyChanges, 0x0), MB_OKCANCEL | MB_ICONEXCLAMATION);
            if (id == 2) {
                return;
            }
        }
        _wsprintf(szExt, MPCTD, idPlayer + 1);
        game.fDirty = 0;
        DestroyCurGame();
        if (FLoadGame(szBase, szExt) != 0) {
            CreateChildWindows();
            SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
        } else {
            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
        }
    } else {
        iPopMenuSel = GET_WM_COMMAND_ID(wParam, 0) - 15000;
    }
    return;
}

void InitializeMenu(HMENU hmenu) {
    int16_t  cMenu;
    int16_t  i;
    HMENU    hmenuSub;
    uint16_t t_merge_548d_0001;
    uint16_t t_merge_5502_0001;
    uint16_t t_merge_5539_0001;
    uint16_t t_merge_5570_0001;

    if (hmenu == 0x0) {
        hmenu = GetMenu(hwndFrame);
    }
    hmenuSub = GetASubMenu(hwndFrame, 0);
    for (i = 4300; i <= 4308; i++) {
        DeleteMenu(hmenuSub, i, 0x0);
    }
    for (i = 0; i < 9 && (int16_t)vrgszMRU[i * 256] != 0; i++) {
        szWork[0] = '&';
        szWork[1] = LOBYTE(i + 49);
        szWork[2] = ' ';
        fstrcpy(&szWork[3], vrgszMRU + 256 * i);
        InsertMenu(hmenuSub, i + 9, 0x400, i + 4300, szWork);
    }
    if ((int16_t)szBase[0] == 0 || game.fSinglePlr != 0x0) {
        t_merge_548d_0001 = 0x3;
    } else {
        t_merge_548d_0001 = 0x0;
    }
    EnableMenuItem(hmenu, 0x6a, t_merge_548d_0001);
    EnableMenuItem(hmenu, 0x69, (int16_t)szBase[0] == 0 ? 0x3 : 0x0);
    if ((int16_t)szBase[0] == 0 || (game.fSinglePlr != 0x0 && lSaltCur <= 0)) {
        t_merge_5502_0001 = 0x3;
    } else {
        t_merge_5502_0001 = 0x0;
    }
    EnableMenuItem(hmenu, 0x10e, t_merge_5502_0001);
    if ((int16_t)szBase[0] == 0 || game.fSinglePlr != 0x0) {
        t_merge_5539_0001 = 0x3;
    } else {
        t_merge_5539_0001 = 0x0;
    }
    EnableMenuItem(hmenu, 0x7de, t_merge_5539_0001);
    if ((int16_t)szBase[0] == 0 || game.fSinglePlr != 0x0) {
        t_merge_5570_0001 = 0x3;
    } else {
        t_merge_5570_0001 = 0x0;
    }
    EnableMenuItem(hmenu, 0xedb, t_merge_5570_0001);
    hmenu = GetASubMenu(hwndFrame, 1);
    CheckMenuItem(hmenu, 0xb3, gd.fToolbar == 0x0 ? 0x0 : 0x8);
    CheckMenuItem(hmenu, 0x98d, (grbitScan & 0x2000) == 0x0 ? 0x0 : 0x8);
    if (hwndScanner != 0x0) {
        EnableMenuItem(GetMenu(hwndFrame), 0x1, 0x400);
        hmenu = GetASubMenu(hwndFrame, 1);
        hmenu = GetSubMenu(hmenu, 3);
        CheckMenuItem(hmenu, iScanZoom + 4, 0x408);
        cMenu = GetMenuItemCount(hmenu);
        for (i = 0; i < cMenu; i++) {
            EnableMenuItem(hmenu, i, 0x400);
        }
        hmenu = GetASubMenu(hwndFrame, 1);
        hmenu = GetSubMenu(hmenu, 4);
        CheckMenuItem(hmenu, iWindowLayout, 0x408);
    } else {
        EnableMenuItem(GetMenu(hwndFrame), 0x1, 0x403);
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
    if (gd.fAisDone == 0x0) {
        fHostSav = gd.fHostMode;
        if (gd.fHostMode == 0x0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            *(uint16_t *)&rgmdplr[iPlayer] = rgplr[iPlayer].wMdPlr;
        }
        gd.fSubmit = 0x1;
        fErrSav = fFileErrSilent;
        fFileErrSilent = 1;
        for (iPlayer = 0; iPlayer < game.cPlayer; iPlayer++) {
            UpdateProgressGauge(MulDiv(340, iPlayer + 1, game.cPlayer));
            if (rgmdplr[iPlayer].fAi != 0x0) {
                fWorkDone = 1;
                gd.fGeneratingTurn = 0x1;
                gd.fHostMode = 0x1;
                fOpened = FOpenFile(dtLog, iPlayer, 32);
                gd.fGeneratingTurn = 0x0;
                gd.fHostMode = fHostSav;
                if (fOpened == 0) {
                    DoAiTurn(iPlayer, *(uint16_t *)&rgmdplr[iPlayer]);
                } else {
                    StreamClose();
                }
            }
        }
        gd.fSubmit = fSubmitSav;
        if (fWorkDone != 0) {
            DestroyCurGame();
            FLoadGame(szBase, "hst");
        }
        fFileErrSilent = fErrSav;
        gd.fAisDone = 0x1;
    }
    return;
}

HMENU GetASubMenu(HWND hwnd, int16_t iMenu) {
    int16_t fChildMenu;
    HMENU   hmenu;
    int16_t t_merge_58c7_0001;

    if (hwndActive == 0x0 || IsZoomed(hwndActive) == 0) {
        t_merge_58c7_0001 = 0;
    } else {
        t_merge_58c7_0001 = 1;
    }
    fChildMenu = t_merge_58c7_0001;
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
    char        *t_call_593a;

    if (ini.fStartupFile == 0x0) {
        szFile[0] = 0;
        CchGetString(fRaceOnly == 0 ? idsStarsGameFilesMHstRStars : idsStarsGameFilesRFiles, szFilter);
        for (i = 0x0; (int16_t)szFilter[i] != 0; i++) {
            if ((int16_t)szFilter[i] == '|') {
                szFilter[i] = 0;
            }
        }
        memset(&ofn, 0, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = szFilter;
        ofn.nFilterIndex = 0x1;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = 0x100;
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle = 0x100;
        ofn.lpstrInitialDir = szDirName;
        ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if (GetOpenFileName(&ofn) == 0) {
            return 0;
        }
    } else {
        strcpy(szFile, szBase);
        *strrchr(szBase, 92) = 0;
        t_call_593a = strrchr(szFile, 46);
        pch = t_call_593a;
        if (t_call_593a == 0x0) {
            SetSzWorkFromDt(dtHost, -1);
            strcpy(szFile, szWork);
            pch = strrchr(szFile, 46);
        }
        ofn.nFileExtension = pch - szFile + 0x1;
        ofn.nFileOffset = 0x0;
        fFileErrSilent = 1;
    }
    szDirName[0] = 0;
    fRet = FWasRaceFile(&szFile[ofn.nFileOffset], fRaceOnly == 0 ? 1 : 0);
    if (fRaceOnly == 0) {
        if (fRet == 0) {
            szFile[ofn.nFileExtension - 1] = 0;
            DestroyCurGame();
            strcpy(szBase, szFile);
            if (FLoadGame(szFile, &szFile[ofn.nFileExtension]) != 0) {
                if (ini.fStartupFile != 0x0) {
                    fFileErrSilent = 0;
                    ini.fStartupFile = 0x0;
                    pch = strrchr(szFile, 92);
                    if (pch != 0x0) {
                        i = pch - szFile;
                        strncpy(szDirName, szFile, i);
                        szDirName[i] = 0;
                    }
                    if (idPlayer == -1 && ini.fGen == 0x0) {
                        gd.fClose = 0x1;
                    }
                    if (game.lid != ini.lid || game.turn > ini.turn) {
                        ini.fTry = 0x0;
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
                if (idPlayer == -1) {
                    if (hwndTitle != 0x0 && fFreeingTitle == 0) {
                        fFreeingTitle = 1;
                        DestroyWindow(hwndTitle);
                        hwndTitle = 0x0;
                    }
                    BringUpHostDlg();
                    return 0;
                }
                grobjIni = ini.grobjSel;
                SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
                if (grobjIni == grobjNone || ini.grobjSel != 0x0) {
                    ini.grobjSel = 0x0;
                    if (ini.grobjSel == 0x0 && cPlanet != 0) {
                        FFindSomethingAndSelectIt();
                    }
                    return 1;
                }
                return 1;
            }
            if (ini.fStartupFile != 0x0) {
                ini.wFlags = 0x0;
                fFileErrSilent = 0;
            }
            return 0;
        }
        if (ini.fStartupFile != 0x0 && vSerialNumber == 0) {
            fRet = -1;
        }
        fFileErrSilent = 0;
        ini.fStartupFile = 0x0;
        if (fRet != -1) {
            RaceCreationWizard(hwnd, 0, 0);
            return 0;
        }
        return -1;
    }
    if (fRet > 0) {
        strcpy(szRaceFile, &szFile[ofn.nFileOffset]);
    }
    return fRet;
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
        StreamOpen(szFile, 32);
        ReadRt();
        if (hdrCur.rt == rtBOF && (RawLoad16(&rgbCur[8]) >> 0xc & 0xf) == 0x2 && (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) >= 0x31 &&
            (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) < 0x54) {
            wVersFile = RawLoad16(&rgbCur[8]);
            if ((RawLoad16(&rgbCur[14]) & 0xff) == 0x5) {
                ReadRt();
                if (hdrCur.rt == rtPlr) {
                    idsError = 3;
                    ReadRtPlr(&plr, rgbCur);
                    ReadRt();
                    if (hdrCur.rt == rtEOF && RawLoad16(rgbCur) == IRaceChecksum(&plr)) {
                        lSaltSav = lSaltCur;
                        lSaltCur = plr.lSalt;
                        if (fChkPass == 0 || FCheckPassword() != 0) {
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
                        lSaltCur = lSaltSav;
                        fRet = -1;
                    }
                }
            }
        } else {
            idsError = 13;
            fRet = -1;
        }
    }
    StreamClose();
    penvMem = penvMemSav;
    fFileErrSilent = fSav;
    if (fFileErrSilent == 0 && idsError != -1) {
        strcpy(szWork, szFile);
        AlertSz(PszFormatIds(idsError, 0x0), MB_ICONHAND);
    }
    return fRet;
}

void BringUpHostDlg() {
    POINT16 pt;
    FARPROC lpProc;
    int16_t fRet;

    if (gd.fHostMode == 0x0) {
        if (gd.fReadOnly == 0x0) {
            FMarkFile(dtHost, -1, 1, 1);
        }
        gd.fHostMode = 0x1;
    }
    ShowWindow(hwndFrame, SW_HIDE);
    if (ini.fWait == 0x0) {
        while (1) {
            lpProc = MakeProcInstance(HostModeDialog, hInst);
            fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_HOST_MODE), hwndFrame, lpProc);
            FreeProcInstance(lpProc);
            if (fRet == -1)
                goto LAutoMode;
            if (fRet == 0)
                break;
            do {
                if (gd.fProgressTxt != 0x0) {
                    ShowProgressGauge();
                }
                EnsureAis();
                FGenerateTurn();
                if (iPassCnt == 0)
                    break;
                iPassCnt = iPassCnt - 1;
            } while (GetAsyncKeyState(16) >= 0 && GetAsyncKeyState(17) >= 0);
            iPassCnt = 0;
            HideProgressGauge();
        }
        if (gd.fReadOnly == 0x0) {
            FMarkFile(dtHost, -1, 1, 0);
        }
        gd.fHostMode = 0x0;
        DestroyCurGame();
        if (ini.fGen != 0x0) {
            return;
        }
        pt.x = GetSystemMetrics(SM_CXSCREEN);
        pt.y = GetSystemMetrics(SM_CYSCREEN);
        hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
        fFreeingTitle = 0;
        return;
    }
    ini.fWait = 0x0;
LAutoMode:
    ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
    uTimerId = SetTimer(0x0, 0xd, 0x2710, lpfnHostTimerProc);
    uTimerType = 0xd;
    HostTimerProc(0x0, WM_NULL, uTimerId, 0x0);
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

    if (hdcIn == 0x0) {
        hdc = GetDC(hwnd);
    } else {
        hdc = hdcIn;
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
        SetTextColor(hdc, rgOut[i] <= 0 ? 0x7f00 : 0x7f);
        CchGetString(rgOut[i] + 716, szStat);
        if (gd.fNoHostNames == 0x0) {
            cch = _wsprintf(szWork, PszGetCompressedString(idsSS), PszPlayerName(i, 1, 1, 1, 0, 0x0), szStat);
        } else {
            cch = _wsprintf(szWork, " %s", szStat);
        }
        if (rgplr[i].fHacker != 0x0) {
            strcat(szWork, " - HACKER");
            cch = cch + 9;
        }
        TextOut(hdc, x + 4, yCur, szWork, cch);
        SetTextColor(hdc, crWindowText);
        OffsetRect(&rcDiamond, 0, dyArial8 + 4);
        yCur = yCur + (dyArial8 + 4);
    }
    cch = _wsprintf(szWork, PCTD, game.turn + 0x961);
    SetWindowText(GetDlgItem(hwnd, IDC_HOST_NEXT_YEAR_TEXT), szWork);
    dsec = (uint32_t)((GetTickCount() - ctickLast) / 0x3e8);
    if (dsec < 0x3c) {
        cch = _wsprintf(szWork, PszGetCompressedString(idsDSeconds), LOWORD(dsec));
    } else {
        dmin = LOWORD((uint32_t)(dsec / 0x3c));
        dsec = dsec - (uint32_t)(0x3c * dmin);
        if (dmin >= 0x3c) {
            dhour = (uint32_t)dmin / 0x3c;
            dmin = dmin - 0x3c * dhour;
            if (dhour >= 0x18) {
                dday = (uint32_t)dhour / 24;
                dhour = dhour - dday * 24;
                cch = _wsprintf(szWork, PszGetCompressedString(idsDDaysD02d02d), dday, dhour, dmin, LOWORD(dsec));
            } else {
                cch = _wsprintf(szWork, PszGetCompressedString(idsD02d02d), dhour, dmin, LOWORD(dsec));
            }
        } else {
            cch = _wsprintf(szWork, PszGetCompressedString(idsD02d), dmin, LOWORD(dsec));
        }
    }
    SetWindowText(GetDlgItem(hwnd, IDC_HOST_TIME_SINCE_TEXT), szWork);
    SetBkMode(hdc, bkMode);
    SetBkColor(hdc, crBackSav);
    if (hdcIn == 0x0) {
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
        if (rgplr[i].fAi == 0x0 && FCheckLogFile(i, &idsError) == 0) {
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
                if (rgplr[i].fDead == 0x0) {
                    if (gd.fPartialTurn == 0x0) {
                        rgOut[i] = 1;
                        cOut = cOut + 1;
                    } else {
                        rgOut[i] = 2;
                        cOut = cOut + 1;
                    }
                } else {
                    rgOut[i] = -1;
                }
            }
            goto L_68d6;
        L_6874:
            cOut = cOut + 1;
        } else if (rgplr[i].fAi == 0x0) {
            _wsprintf(szWork, "%s.x%d", szBase, i + 1);
            idPlayer = i;
            if (FLoadLogFile(szWork) == 0 || FRunLogFile() != 0) {
                rgOut[i] = 0;
            } else {
                rgOut[i] = 3;
            }
        } else {
            cAi = cAi + 1;
            rgOut[i] = 0;
        }
    L_68d6:
        if (ctickLast == 0x0 || rgOut[i] != fOut) {
            ctickLast = GetTickCount();
        }
    }
    FreeLp(vrgPlanResExtra, htMisc);
    vrgPlanResExtra = 0x0;
    FreeLp(vrgts, htMisc);
    vrgts = 0x0;
    FreeLp(lpcd, htMisc);
    lpcd = 0x0;
    FreeLp(lpxf, htMisc);
    lpxf = 0x0;
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
    gd.fHostMode = 0x1;
    gd.fGeneratingTurn = 0x0;
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
    gd.fGeneratingTurn = 0x1;
    for (i = 0; i < game.cPlayer; i++) {
        fOut = rgOut[i];
        idsError = 0;
        if (rgplr[i].fAi == 0x0 && FCheckLogFile(i, &idsError) == 0) {
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
                if (rgplr[i].fDead == 0x0) {
                    if (gd.fPartialTurn == 0x0) {
                        rgOut[i] = 1;
                        cOut = cOut + 1;
                    } else {
                        rgOut[i] = 2;
                        cOut = cOut + 1;
                    }
                } else {
                    rgOut[i] = -1;
                }
            }
            goto L_6b8f;
        L_6b2d:
            cOut = cOut + 1;
        } else {
            if (rgplr[i].fAi != 0x0) {
                cAi = cAi + 1;
            }
            rgOut[i] = 0;
        }
    L_6b8f:
        if (ctickLast == 0x0 || rgOut[i] != fOut) {
            ctickLast = GetTickCount();
        }
    }
    gd.fGeneratingTurn = 0x0;
    gd.fAllAis = cAi == game.cPlayer ? 0x1 : 0x0;
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
    int16_t     t_merge_6cac_0001;
    POINT       t_pt_6dbd;
    POINT       t_pt_6dcc_1;
    int16_t     t_merge_6f12_0001;
    int16_t     t_merge_6f5d_0001;
    POINT       t_pt_6fa5_1;
    HWND        t_call_715f;
    int16_t     t_merge_7198_0001;
    HWND        t_call_7203;
    int16_t     t_merge_723c_0001;
    int16_t     t_merge_73b8_0001;
    HWND        t_call_74e0;
    int16_t     t_merge_7519_0001;

    switch (message) {
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (ctickLast == 0x0) {
            CFindTurnsOutstanding();
        }
        if (gd.fReadOnly == 0x0) {
            t_call_7203 = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
            if (gd.fAllAis != 0x0 || (vtimer.fAutoGenWhenIn == 0 && vtimer.mdForce == 0)) {
                t_merge_723c_0001 = 0;
            } else {
                t_merge_723c_0001 = 1;
            }
            EnableWindow(t_call_7203, t_merge_723c_0001);
        }
        DrawHostDialog2(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_DESTROY:
        KillTimer(hwnd, uTimerId);
        uTimerId = 0x0;
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
            if (gd.fReadOnly != 0x0 || (vtimer.fAutoGenWhenIn == 0 && vtimer.mdForce == 0)) {
                t_merge_6cac_0001 = 0;
            } else {
                t_merge_6cac_0001 = 1;
            }
            EnableWindow(t_call_6c71, t_merge_6cac_0001);
            EnableWindow(GetDlgItem(hwnd, IDC_HOST_GENERATE_NOW), gd.fReadOnly == 0x0 ? 1 : 0);
            EnableWindow(GetDlgItem(hwnd, IDC_HOST_PASSWORD), gd.fReadOnly == 0x0 ? 1 : 0);
            uTimerId = SetTimer(hwnd, 0xd, 0x2710, 0x0);
        case WM_TIMER:
            if (fProcessingTimer == 0) {
                fProcessingTimer = 1;
                CFindTurnsOutstanding();
                DrawHostDialog2(hwnd, 0x0);
                fProcessingTimer = 0;
            }
            if (message == WM_TIMER) {
                return 0;
            }
            return 1;
        case WM_SETCURSOR:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            GetCursorPos(&t_pt_6dbd);
            pt = PointTo16(t_pt_6dbd);
            t_pt_6dcc_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_6dcc_1);
            pt = PointTo16(t_pt_6dcc_1);
            if (pt.x < 6 || pt.x >= dyArial8 + 7 || pt.y < 48)
                break;
            iDiamond = (int32_t)(pt.y - 48) / (dyArial8 + 4);
            if (iDiamond >= game.cPlayer || (int32_t)(pt.y - 48) % (dyArial8 + 4) >= dyArial8 + 1)
                break;
            if (message != WM_SETCURSOR) {
                if (rgplr[iDiamond].fAi == 0x0) {
                    iSel = 0;
                } else if (rgplr[iDiamond].idAi != 0x7) {
                    iSel = 1;
                } else {
                    iSel = 2;
                }
                hmenuPopup = CreatePopupMenu();
                iPopMenuSel = -1;
                for (i = 0; i < 3; i++) {
                    CchGetString(i + 527, szWork);
                    if (i != 1) {
                        if (rgplr[iDiamond].fAi != 0x0 && rgplr[iDiamond].idAi != 0x7) {
                            t_merge_6f5d_0001 = 3;
                        } else {
                            t_merge_6f5d_0001 = 0;
                        }
                        mf = t_merge_6f5d_0001;
                    } else {
                        if (rgplr[iDiamond].fAi == 0x0 || rgplr[iDiamond].idAi == 0x7) {
                            t_merge_6f12_0001 = 3;
                        } else {
                            t_merge_6f12_0001 = 0;
                        }
                        mf = t_merge_6f12_0001;
                    }
                    AppendMenu(hmenuPopup, (i == iSel ? 0x8 : 0x0) | mf, i + 15000, szWork);
                }
                t_pt_6fa5_1 = PointFrom16(pt);
                ClientToScreen(hwnd, &t_pt_6fa5_1);
                pt = PointTo16(t_pt_6fa5_1);
                tpm = message == WM_LBUTTONDOWN ? 0 : 2;
                TrackPopupMenu(hmenuPopup, 0x4 | tpm, pt.x, pt.y, 0, hwnd, 0x0);
                DestroyMenu(hmenuPopup);
                iRet = -1;
                if (PeekMessage(&msg, hwnd, 0x111, 0x111, 0x2) != 0 && msg.wParam >= 0x3a98 && msg.wParam < 0x3afc) {
                    iRet = msg.wParam - 15000;
                }
                if (iRet != -1 && iSel != iRet) {
                    rgplr[iDiamond].wMdPlr = (rgplr[iDiamond].wMdPlr & 0xfdff) | ((iRet <= 0 ? 0x0 : 0x1) & 0x1) * 0x200;
                    if (iRet == 2) {
                        rgplr[iDiamond].wMdPlr = (rgplr[iDiamond].wMdPlr & 0x1fff) | 0xe000;
                    }
                    rgplr[iDiamond].lSalt = ~rgplr[iDiamond].lSalt;
                    FMarkFile(dtTurn, iDiamond, 8, iRet == 0 ? 0 : 1);
                    FMarkFile(dtHost, iDiamond, 8, iRet == 0 ? 0 : 1);
                    gd.fAisDone = 0x0;
                    fProcessingTimer = 1;
                    CFindTurnsOutstanding();
                    t_call_715f = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
                    if (gd.fAllAis != 0x0 || (vtimer.fAutoGenWhenIn == 0 && vtimer.mdForce == 0)) {
                        t_merge_7198_0001 = 0;
                    } else {
                        t_merge_7198_0001 = 1;
                    }
                    EnableWindow(t_call_715f, t_merge_7198_0001);
                    DrawHostDialog2(hwnd, 0x0);
                    fProcessingTimer = 0;
                }
                return 0;
            }
            SetCursor(hcurHand);
            return 1;
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDC_HOST_GENERATE_NOW:
            case IDCANCEL:
            case IDC_HOST_AUTO_GENERATE:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_HOST_GENERATE_NOW) {
                    if (GetAsyncKeyState(16) >= 0) {
                        if (GetAsyncKeyState(17) >= 0) {
                            iPassCnt = 0;
                        } else {
                            iPassCnt = 99;
                        }
                    } else if (GetAsyncKeyState(17) >= 0) {
                        iPassCnt = 9;
                    } else {
                        iPassCnt = 999;
                    }
                    if (iPassCnt == 0) {
                        if (CFindTurnsOutstanding() != 0 &&
                            AlertSz(PszFormatIds(idsSureWishGenerateOptionDoesGuaranteePlayers, 0x0), MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL) != IDYES) {
                            return 1;
                        }
                    } else {
                        _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), iPassCnt + 1);
                        if (MessageBox(GetFocus(), szWork, "Stars!", MB_YESNO | MB_ICONEXCLAMATION | MB_TASKMODAL) != IDYES) {
                            iPassCnt = 0;
                            return 1;
                        }
                    }
                }
                StickyDlgPos(hwnd, &ptStickyHostModeDlg, 0);
                if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL) {
                    if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_HOST_AUTO_GENERATE) {
                        t_merge_73b8_0001 = 1;
                    } else {
                        t_merge_73b8_0001 = -1;
                    }
                } else {
                    t_merge_73b8_0001 = 0;
                }
                EndDialog(hwnd, t_merge_73b8_0001);
                if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_HOST_AUTO_GENERATE) {
                    if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL && gd.fClose != 0x0) {
                        PostQuitMessage(vretExitValue);
                    }
                } else {
                    EnsureAis();
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
            case 0x405:
                lpProc = MakeProcInstance(HostOptionsDialog, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_HOST_OPTIONS), hwnd, lpProc);
                FreeProcInstance(lpProc);
                SetFocus(hwnd);
                if (fRet != 0 && gd.fReadOnly == 0x0) {
                    t_call_74e0 = GetDlgItem(hwnd, IDC_HOST_AUTO_GENERATE);
                    if (gd.fAllAis != 0x0 || (vtimer.fAutoGenWhenIn == 0 && vtimer.mdForce == 0)) {
                        t_merge_7519_0001 = 0;
                    } else {
                        t_merge_7519_0001 = 1;
                    }
                    EnableWindow(t_call_74e0, t_merge_7519_0001);
                }
                return fRet;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, 0x440);
                return 1;
            default:
            }
        default:
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
                WinHelp(hwnd, szHelpFile, 0x1, 0x440);
                return 1;
            default:
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
        if (uTimerType == 0xe) {
            if (FNewTurnAvail(idPlayer) == 0)
                goto Done;
            idCur = idPlayer;
            KillTimer(hwnd, uTimerId);
            _wsprintf(szExt, MPCTD, idPlayer + 1);
            DestroyCurGame();
            if (FLoadGame(szBase, szExt) != 0) {
                idPlayer = idCur;
                CreateChildWindows();
                uTimerId = SetTimer(0x0, 0xf, 0x3e8, lpfnHostTimerProc);
                uTimerType = 0xf;
                FlashWindow(hwndFrame, 1);
                SetWindowText(hwndFrame, PszGetCompressedString(idsNewTurnAvailable2));
                MessageBeep(0x30);
                cOut = 1;
                goto RedrawText;
            }
            AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
            goto Done;
        }
        if (uTimerType == 0xf) {
            FlashWindow(hwndFrame, 1);
            goto Done;
        }
    Loop:
        cOut = CFindTurnsOutstanding();
        if (gd.fAllAis != 0x0) {
            AlertSz(PszFormatIds(idsAutoGenerateDisabledBecauseHumanPlayersDead, 0x0), MB_ICONHAND);
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
            InvalidateRect(hwndT, 0x0, 1);
        }
        if (cOut == 0) {
            if (gd.fProgressTxt != 0x0) {
                ShowProgressGauge();
            }
            EnsureAis();
            FGenerateTurn();
            HideProgressGauge();
            if (ini.fGen == 0x0) {
                EnsureAis();
                goto Loop;
            }
            PostQuitMessage(vretExitValue);
        }
    Done:
        fProcessingTimer = 0;
        fFileErrSilent = fSav;
    }
    return;
}

void GetWindowRc(HWND hwnd, RECT *prc) {
    WINDOWPLACEMENT wndpl;

    wndpl.length = 0x16;
    GetWindowPlacement(hwnd, &wndpl);
    *prc = wndpl.rcNormalPosition;
    prc->right = prc->right - prc->left;
    prc->bottom = prc->bottom - prc->top;
    return;
}

void SetWindowIniString(char *sz, HWND hwnd) {
    char ch;
    RECT rc;

    if (IsZoomed(hwnd) == 0) {
        if (IsIconic(hwnd) == 0) {
            ch = 'R';
        } else {
            ch = 'I';
        }
    } else {
        ch = 'M';
    }
    GetWindowRc(hwnd, &rc);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), (int16_t)ch, rc.left, rc.top, rc.right, rc.bottom);
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
    int16_t  t_7fd8;
    char    *t_8996;
    char    *t_89bf;
    char    *t_89f0;
    char    *t_8a1e;
    char    *t_8ada;
    char    *t_8b28;
    char    *t_8b45;
    char    *t_8b95;
    char    *t_8bc8;
    char    *t_8bf8;
    char    *t_8c28;

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
        i = i | 0x1;
    }
    if (gd.mdScreenSize == 0x0) {
        i = i | 0x2;
    }
    _wsprintf(szWork, szPd, i);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsMain, szEntry);
    SetWindowIniString(szWork, hwndFrame);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportfleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptFleet.ptDlg.x, vrptFleet.ptDlg.y, vrptFleet.ptDlg.x + vrptFleet.ptSize.x,
              vrptFleet.ptDlg.y + vrptFleet.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportefleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptEFleet.ptDlg.x, vrptEFleet.ptDlg.y, vrptEFleet.ptDlg.x + vrptEFleet.ptSize.x,
              vrptEFleet.ptDlg.y + vrptEFleet.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportbtlwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptBattle.ptDlg.x, vrptBattle.ptDlg.y, vrptBattle.ptDlg.x + vrptBattle.ptSize.x,
              vrptBattle.ptDlg.y + vrptBattle.ptSize.y);
    WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    CchGetString(idsReportplanwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptPlanet.ptDlg.x, vrptPlanet.ptDlg.y, vrptPlanet.ptDlg.x + vrptPlanet.ptSize.x,
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
    while (1) {
        t_7fd8 = iPass;
        iPass = iPass - 1;
        if (t_7fd8 == 0)
            break;
        psz = szWork;
        iCol = 0x0;
        i = 0;
        while (i < ctile) {
            while (rgtile[i].iCol > iCol) {
                iCol = iCol + 0x1;
                *psz = '*';
                psz = psz + 1;
            }
            *psz = LOBYTE(rgtile[i].fPopped == 0x0 ? 0x61 : 0x41);
            *psz = *psz + LOBYTE(rgtile[i].id);
            i = i + 1;
            psz = psz + 1;
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
    default:
    }
    _wsprintf(szWork, PszGetCompressedString(idsCCD), (int16_t)ch, (int16_t)LOBYTE(idPlayer + 66), sel.id);
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
    if (gd.fChgScanner != 0x0) {
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
        szWork[0] = LOBYTE((uTimerId == 0x0 ? 0x0 : 0x1) + 0x30);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        if (gd.fWriteTurnNum != 0x0) {
            itoa(game.turn, szWork, 10);
            CchGetString(idsTurn, szEntry);
            WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
            gd.fWriteTurnNum = 0x0;
        }
        CchGetString(idsFile1, szEntry);
        _wsprintf(szWork, "%s.m%d", szBase, idPlayer + 1);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
    }
    CchGetString(idsMisc, szSection);
    if (gd.fChgReports != 0x0) {
        CchGetString(idsReportplanfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptPlanet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportplansort, szEntry);
        i = vrptPlanet.icolSort;
        if (vrptPlanet.fAscending != 0) {
            i = i | 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportfleetfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptFleet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportfleetsort, szEntry);
        i = vrptFleet.icolSort;
        if (vrptFleet.fAscending != 0) {
            i = i | 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportefleetfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptEFleet.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportefltsort, szEntry);
        i = vrptEFleet.icolSort;
        if (vrptEFleet.fAscending != 0) {
            i = i | 0x100;
        }
        _wsprintf(szWork, PCTD, i);
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportbtlfld, szEntry);
        _wsprintf(szWork, PCTD, LOWORD(vrptBattle.grbitVisible));
        WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        CchGetString(idsReportbtlsort, szEntry);
        i = vrptBattle.icolSort;
        if (vrptBattle.fAscending != 0) {
            i = i | 0x100;
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
    if (gd.fChgZipOrd != 0x0) {
        CchGetString(idsZiporders, szSection);
        for (i = 0; i < 4; i++) {
            strcpy(szEntry, szSection);
            psz = &szEntry[strlen(szEntry)];
            *psz = LOBYTE(i + 49);
            psz[1] = 0;
            if (vrgZip[i].fValid == 0x0) {
                szWork[0] = 0;
            } else {
                psz = szWork;
                for (iPass = 0; iPass < 5; iPass++) {
                    t_8996 = psz;
                    psz = psz + 1;
                    *t_8996 = LOBYTE((vrgZip[i].txp.rgia[iPass].iAction & 0xff) + 0x61);
                    t_89bf = psz;
                    psz = psz + 1;
                    *t_89bf = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan & 0xf & 0xff) + 0x61);
                    t_89f0 = psz;
                    psz = psz + 1;
                    *t_89f0 = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan >> 0x4 & 0xf & 0xff) + 0x61);
                    t_8a1e = psz;
                    psz = psz + 1;
                    *t_8a1e = LOBYTE((vrgZip[i].txp.rgia[iPass].cQuan >> 0x8 & 0xf & 0xff) + 0x61);
                }
                strcpy(psz, vrgZip[i].szName);
            }
            WritePrivateProfileString(szSection, szEntry, szWork, szIniFile);
        }
    }
    if (gd.fChgZipProd != 0x0) {
        for (i = 0; i < 5; i++) {
            CchGetString(idsZiporders, szSection);
            strcpy(szEntry, szSection);
            psz = &szEntry[strlen(szEntry)];
            t_8ada = psz;
            psz = psz + 1;
            *t_8ada = 'P';
            *psz = LOBYTE(i + 49);
            psz[1] = 0;
            if (vrgZipProd[i].fValid == 0x0) {
                szWork[0] = 0;
            } else {
                psz = szWork;
                t_8b28 = psz;
                psz = psz + 1;
                *t_8b28 = LOBYTE(vrgZipProd[i].fNoResearch + 0x61);
                t_8b45 = psz;
                psz = psz + 1;
                *t_8b45 = LOBYTE(vrgZipProd[i].cpq + 0x61);
                for (iPass = 0; iPass < vrgZipProd[i].cpq; iPass++) {
                    t_8b95 = psz;
                    psz = psz + 1;
                    *t_8b95 = LOBYTE((vrgZipProd[i].rgpq[iPass].w & 0xf & 0xff) + 0x61);
                    t_8bc8 = psz;
                    psz = psz + 1;
                    *t_8bc8 = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 0x4 & 0xf & 0xff) + 0x61);
                    t_8bf8 = psz;
                    psz = psz + 1;
                    *t_8bf8 = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 0x8 & 0xf & 0xff) + 0x61);
                    t_8c28 = psz;
                    psz = psz + 1;
                    *t_8c28 = LOBYTE((vrgZipProd[i].rgpq[iPass].w >> 0xc & 0xf & 0xff) + 0x61);
                }
                strcpy(psz, vrgZipProd[i].szName);
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
    int16_t t_merge_8d1e_0001;
    int16_t t_merge_8d39_0001;
    int16_t t_merge_8f3a_0001;
    int16_t t_merge_8f55_0001;

    if (hwndFrame != 0x0 && IsIconic(hwndFrame) == 0) {
        switch (iWindowLayout) {
        case 0:
        default:
            if (vfs.dx - vfs.dxPlanWant >= 100) {
                vfs.xTop = vfs.dxPlanWant;
            } else {
                vfs.xTop = vfs.dx - 100;
            }
            vfs.xTop = vfs.xTop <= 198 ? 198 : vfs.xTop;
            dyMsgMin = (0xd * dyArial8 >> 0x1) + 0xa;
            dyMinMin = 13 * dyArial8 - 36;
            t_merge_8d1e_0001 = vfs.dyMsgWant <= dyMsgMin ? dyMsgMin : vfs.dyMsgWant;
            dyMsg = t_merge_8d1e_0001;
            vfs.dyMsgWant = t_merge_8d1e_0001;
            t_merge_8d39_0001 = vfs.dyMinWant <= dyMinMin ? dyMinMin : vfs.dyMinWant;
            dyMin = t_merge_8d39_0001;
            vfs.dyMinWant = t_merge_8d39_0001;
            if (vfs.dy - (dyMsg + dyMin + 16) < 50) {
                dyT = vfs.dy - 66;
                dyTot = dyMsg + dyMin;
                dyMsg = MulDiv(dyT, dyMsg, dyTot);
                dyMin = MulDiv(dyT, dyMin, dyTot);
                if (dyMsg >= dyMsgMin) {
                    if (dyMin < dyMinMin) {
                        dyMsg = dyMsg - (dyMinMin - dyMin);
                        dyMin = dyMinMin;
                    }
                } else {
                    dyMin = dyMin - (dyMsgMin - dyMsg);
                    dyMsg = dyMsgMin;
                }
            }
            vfs.y1 = vfs.dy - dyMin - dyMsg - 16;
            vfs.y2 = vfs.y1 + dyMsg + 8;
            if (hwndScanner == 0x0)
                break;
            if (gd.fToolbar == 0x0) {
                MoveWindow(hwndTb, 0, -100, 50, 50, 1);
                yScanner = 0;
            } else {
                MoveWindow(hwndTb, vfs.xTop + 8, 0, vfs.dx - vfs.xTop - 8, 36, 1);
                yScanner = 36;
            }
            MoveWindow(hwndScanner, vfs.xTop + 8, yScanner, vfs.dx - vfs.xTop - 8, vfs.dy - yScanner, 1);
            MoveWindow(hwndPlanet, 0, 0, vfs.xTop, vfs.y1, 1);
            MoveWindow(hwndMessage, 0, vfs.y1 + 8, vfs.xTop, dyMsg, 1);
            MoveWindow(hwndMine, 0, vfs.y2 + 8, vfs.xTop, dyMin, 1);
            break;
        case 1:
        case 2:
            if (vfs.dx - vfs.dx2PlanWant >= 200) {
                vfs.xTop = vfs.dx2PlanWant;
            } else {
                vfs.xTop = vfs.dx - 200;
            }
            vfs.xTop = vfs.xTop <= 198 ? 198 : vfs.xTop;
            dyMsgMin = (0xd * dyArial8 >> 0x1) + 0xa;
            dyMinMin = 13 * dyArial8 - 36;
            t_merge_8f3a_0001 = vfs.dy2MsgWant <= dyMsgMin ? dyMsgMin : vfs.dy2MsgWant;
            dyMsg = t_merge_8f3a_0001;
            vfs.dy2MsgWant = t_merge_8f3a_0001;
            t_merge_8f55_0001 = vfs.dy2MinWant <= dyMinMin ? dyMinMin : vfs.dy2MinWant;
            dyMin = t_merge_8f55_0001;
            vfs.dy2MinWant = t_merge_8f55_0001;
            if (vfs.dy - (dyMsg + 8) < 100) {
                dyMsg = vfs.dy - 108;
            }
            if (vfs.dy - (dyMin + 8) < 100) {
                dyMin = vfs.dy - 108;
            }
            vfs.y1 = vfs.dy - dyMsg - 8;
            vfs.y2 = vfs.dy - dyMin - 8;
            if (hwndScanner != 0x0) {
                if (gd.fToolbar == 0x0) {
                    MoveWindow(hwndTb, 0, -100, 50, 50, 1);
                    yScanner = 0;
                } else {
                    MoveWindow(hwndTb, 0, 0, vfs.dx, 36, 1);
                    yScanner = 36;
                }
                MoveWindow(hwndScanner, vfs.xTop + 8, yScanner, vfs.dx - vfs.xTop - 8, vfs.y2 - yScanner, 1);
                MoveWindow(hwndPlanet, 0, yScanner, vfs.xTop, vfs.y1 - yScanner, 1);
                MoveWindow(hwndMessage, 0, vfs.y1 + 8, vfs.xTop, dyMsg, 1);
                MoveWindow(hwndMine, vfs.xTop + 8, vfs.y2 + 8, vfs.dx - vfs.xTop - 8, dyMin, 1);
            }
        }
        hmenu = GetASubMenu(hwndFrame, 1);
        hmenu = GetSubMenu(hmenu, 4);
        for (i = 130; i <= 132; i++) {
            CheckMenuItem(hmenu, i, i - 130 == iWindowLayout ? 0x8 : 0x0);
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
            if (vhpalSplash == 0x0 && vhdibTitle != 0x0) {
                vhpalSplash = HpalFromDib(vhdibTitle);
            }
        }
        GetClientRect(hwnd, &rc);
        dx = 120 <= rc.right >> 0x3 ? rc.right >> 0x3 : 120;
        if (rc.bottom < 650) {
            dx = dx + (int32_t)dx / 6;
        }
        dxGap = (rc.right - (dx << 0x2)) >> 0x2;
        xCur = dxGap >> 0x1;
        dy = rc.bottom <= 500 ? dyArial8 * 2 : (int32_t)(5 * dyArial8) / 2;
        for (i = 0; i < 4; i++) {
            psz = PszGetCompressedString(i + 479);
            rghwndBtnSplash[i] = CreateWindow("BUTTON", psz, WS_CHILD | WS_VISIBLE, xCur, rc.bottom - dy - (int32_t)(5 * dyArial8) / 2, dx, dy, hwnd,
                                              (HMENU)(uintptr_t)i, hInst, 0x0);
            if (i == 2 && ((int16_t)szBase[0] == 0 || access(szBase, 0) == -1)) {
                EnableWindow(rghwndBtnSplash[2], 0);
            }
            if (rc.bottom < 500) {
                SendMessage(rghwndBtnSplash[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
            }
            xCur = xCur + (dx + dxGap);
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
        if (i == 0) {
            return 0;
        }
        InvalidateRect(hwnd, 0x0, 1);
        return 1;
    case WM_PALETTECHANGED:
        if ((HWND)wParam != hwnd)
            goto MapIt;
        break;
    case WM_DESTROY:
        if (vhdibTitle != 0x0) {
            GlobalUnlock(vhdibTitle);
            FreeResource(vhdibTitle);
            vhdibTitle = 0x0;
        }
        if (fFreeingTitle != 0)
            goto Default;
        if (gd.fExitWindows == 0x0) {
            WriteIniSettings();
            PostQuitMessage(vretExitValue);
            goto Default;
        }
        ExitWindows((int32_t)vretExitValue, 0x0);
        goto Default;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case 0x0:
            NewGameWizard(hwnd, 0);
            if (lpPlanets != 0x0 || game.lid != 0) {
                if (fFreeingTitle == 0) {
                    fFreeingTitle = 1;
                    DestroyWindow(hwndTitle);
                    hwndTitle = 0x0;
                }
                ShowWindow(hwndFrame, SW_SHOW);
                break;
            }
            SetFocus(hwnd);
            break;
        case IDOK:
        LOpenGame:
            if (FOpenGame(hwnd, 0) <= 0) {
                SetFocus(hwnd);
            } else {
                if (fFreeingTitle == 0) {
                    fFreeingTitle = 1;
                    DestroyWindow(hwndTitle);
                    hwndTitle = 0x0;
                }
                if (idPlayer != -1) {
                    ShowWindow(hwndFrame, SW_SHOW);
                }
                InitializeMenu(0x0);
                PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0);
                if (game.fTutorial != 0x0 && idPlayer == 0) {
                    StartTutor(0);
                }
            }
            ini.fStartupFile = 0x0;
            break;
        case IDCANCEL:
            ini.fStartupFile = 0x1;
            goto LOpenGame;
        case 0x3:
            if (gd.fExitWindows == 0x0) {
                WriteIniSettings();
                PostQuitMessage(vretExitValue);
            } else {
                ExitWindows((int32_t)vretExitValue, 0x0);
            }
        default:
        }
        break;
    case WM_PAINT:
        if (IsIconic(hwnd) == 0) {
            plf = LocalAlloc(0x40, sizeof(LOGFONT));
            hdc = BeginPaint(hwnd, &ps);
            hbrSav = SelectObject(hdc, hbrButtonFace);
            GetClientRect(hwnd, &rcWnd);
            if (vcScreenColors >= 8) {
                SetTextColor(hdc, 0x2ff7fff);
                if (vhdibTitle != 0x0) {
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
            plf->lfHeight = (int32_t)-rcWnd.bottom / 3;
            strcpy(plf->lfFaceName, rgszArial[3]);
            hfont = CreateFontIndirect(plf);
            SetTextColor(hdc, 0x9b009b);
            if (hfont != 0x0) {
                hfontSav = SelectObject(hdc, hfont);
                SetBkMode(hdc, TRANSPARENT);
                rcT = rcWnd;
                rcT.bottom = (int32_t)(3 * rcT.bottom) / 4;
                RcCtrTextOut(hdc, &rcT, "Stars!", 6);
                SelectObject(hdc, hfontSav);
                DeleteObject(hfont);
            }
        L_9752:
            SelectObject(hdc, rghfontArial10[1]);
            SetBkMode(hdc, TRANSPARENT);
            SzVersion();
            GetWindowRect(rghwndBtnSplash[0], &rcT);
            rcWnd.top = rcT.top - (int32_t)(9 * dyArial8) / 2;
            rcWnd.bottom = (int32_t)(3 * dyArial8) / 2 + rcWnd.top;
            RcCtrTextOut(hdc, &rcWnd, szWork, strlen(szWork));
            EndPaint(hwnd, &ps);
            LocalFree(plf);
            break;
        }
        hdc = BeginPaint(hwnd, &ps);
        DrawIcon(hdc, 2, 2, hiconStars);
        EndPaint(hwnd, &ps);
        break;
    default:
    Default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
