#include "common.h"

uint8_t vrgbShuffleSerial[21] = {11, 4, 5, 16, 17, 12, 19, 15, 10, 1, 14, 13, 3, 18, 2, 20, 9, 7, 0, 8, 6};
char    rgTOWidth[2][2] = {{-3}, {2, 1}};

int16_t InitMDIApp() {
    WNDCLASS wc;

L_0000:
    wc.style = 0xb;
    wc.lpfnWndProc = FrameWndProc;
    wc.cbClsExtra = 0x0;
    wc.cbWndExtra = 0x0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = 0xd;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = "starsframe";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0076;
    else
        goto L_0070;

L_0070:
    return 0x0;

L_0076:
    wc.style = 0x20b;
    wc.lpfnWndProc = MessageWndProc;
    wc.hIcon = 0x0;
    wc.lpszMenuName = 0x0;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = "starsmessage";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_00c5;
    else
        goto L_00bf;

L_00bf:
    return 0x0;

L_00c5:
    wc.style = 0x20b;
    wc.lpfnWndProc = ScannerWndProc;
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "starsscan";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0105;
    else
        goto L_00ff;

L_00ff:
    return 0x0;

L_0105:
    wc.style = 0x20b;
    wc.lpfnWndProc = MineWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = "starsmine";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0145;
    else
        goto L_013f;

L_013f:
    return 0x0;

L_0145:
    wc.style = 0x208;
    wc.lpfnWndProc = TbWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = "starstb";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0185;
    else
        goto L_017f;

L_017f:
    return 0x0;

L_0185:
    wc.style = 0x200;
    wc.lpfnWndProc = PlanetWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = "starsplanet";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_01ca;
    else
        goto L_01c4;

L_01c4:
    return 0x0;

L_01ca:
    wc.style = 0xa00;
    wc.lpfnWndProc = PopupWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = "starspopup";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_020f;
    else
        goto L_0209;

L_0209:
    return 0x0;

L_020f:
    wc.style = 0xa00;
    wc.lpfnWndProc = TooltipWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = "starstt";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0254;
    else
        goto L_024e;

L_024e:
    return 0x0;

L_0254:
    wc.style = 0x200;
    wc.lpfnWndProc = BrowserWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = "starsbrowser";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0299;
    else
        goto L_0293;

L_0293:
    return 0x0;

L_0299:
    wc.style = 0x0;
    wc.lpfnWndProc = TitleWndProc;
    wc.cbClsExtra = 0x0;
    wc.cbWndExtra = 0x0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0x0;
    wc.lpszClassName = "starstitle";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_030c;
    else
        goto L_0306;

L_0306:
    return 0x0;

L_030c:
    wc.style = 0xb;
    wc.lpfnWndProc = ReportDlg;
    wc.cbClsExtra = 0x0;
    wc.cbWndExtra = 0x0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = 0x0;
    wc.lpszClassName = "starsreport";
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_037f;
    else
        goto L_0379;

L_0379:
    return 0x0;

L_037f:
    return 0x1;
}

void CreateChildWindows() {
    char  szData[100];
    POINT pt;
    char *psz;
    char  szGame[15];

L_038c:
    if ((idPlayer == 0xffff))
        goto L_0480;
    else
        goto L_039f;

L_039f:
    psz = (szBase[strlen(0x56a2)] + 0xffff);

L_03b8:
    if ((psz <= 0x56a2))
        goto L_03e7;
    else
        goto L_03c2;

L_03c2:
    if (((uint16_t)(*(psz - 0x1)) == 0x5c))
        goto L_03e7;
    else
        goto L_03d1;

L_03d1:
    if (((uint16_t)(*(psz - 0x1)) == 0x3a))
        goto L_03e7;
    else
        goto L_03e0;

L_03e0:
    psz = (psz - 0x1);
    goto L_03b8;

L_03e7:
    /* untranslated: part[8:1](szGame) = 0x0 */
    strncpy(szGame, psz, 0x8);
    strlwr(szGame);
    _wsprintf(&(szGame[strlen(szGame)]), ".m%d", (idPlayer + 0x1));
    /* untranslated: call _wsprintf(&szData, "Stars! -- %s -- %s -- %s", 0x90, 0x25, PszPlayerName(idPlayer, 0x0, 0x1, 0x0, 0x0, 0x0), 0x25, szGame, ss) ->
     * callresult(int16_t) */
    goto L_04ad;

L_0480:
    CchGetString(idsStarsSHostMode, 0x57a4);
    _wsprintf(&(szData), szWork, 0x90, 0x25);

L_04ad:
    SetWindowText(hwndFrame, &(szData));
    if ((idPlayer == 0xffff))
        goto L_06cf;
    else
        goto L_04c4;

L_04c4:

L_04ca:
    if ((hwndScanner != 0x0))
        goto L_051a;
    else
        goto L_04d4;

L_04d4:
    hwndScanner = CreateWindow(szScan, 0x0, 0x50000000, 0xff38, 0xff38, 0xa, 0xa, hwndFrame, 0x0, hInst, 0x0);
    goto L_0547;

L_051a:
    InvalidateRect(hwndScanner, 0x0, 0x1);
    yScanTop = 0x3e8;
    xScanTop = 0x3e8;
    SetScanScrollBars(hwndScanner);

L_0547:
    if ((hwndMine != 0x0))
        goto L_0595;
    else
        goto L_0551;

L_0551:
    hwndMine = CreateWindow(szMine, 0x0, 0x50000000, 0xfe0c, 0xfe0c, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    goto L_05aa;

L_0595:
    InvalidateRect(hwndMine, 0x0, 0x1);

L_05aa:
    if ((hwndPlanet != 0x0))
        goto L_05fa;
    else
        goto L_05b4;

L_05b4:
    hwndPlanet = CreateWindow(szPlanet, 0x0, 0x50000000, 0xfe0c, 0xfe0c, 0xa, 0xa, hwndFrame, 0x0, hInst, 0x0);
    goto L_060f;

L_05fa:
    InvalidateRect(hwndPlanet, 0x0, 0x1);

L_060f:
    if ((hwndTb != 0x0))
        goto L_065f;
    else
        goto L_0619;

L_0619:
    hwndTb = CreateWindow(szTb, 0x0, 0x50000000, 0xfe0c, 0xfe0c, 0xa, 0xa, hwndFrame, 0x0, hInst, 0x0);
    goto L_0674;

L_065f:
    InvalidateRect(hwndTb, 0x0, 0x1);

L_0674:
    if ((hwndMessage == 0x0))
        goto L_0687;
    else
        goto L_067e;

L_067e:
    DestroyWindow(hwndMessage);

L_0687:
    hwndMessage = CreateWindow(szMessage, 0x0, 0x50000000, 0xfe0c, 0xfe0c, 0xa, 0xa, hwndFrame, 0x0, hInst, 0x0);
    RefitFrameChildren();

L_06cf:
    return;
}

int32_t FrameWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    HDC        hdc;
    int16_t    i;
    HPALETTE   hpalSav;
    TEXTMETRIC tm;
    int16_t    ich;
    POINT      pt;
    char      *pch;
    char       szTemp[80];
    int16_t (**lpProc)();
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
    uint16_t    hcs;
    POINT       ptOld;
    int16_t     grSel;
    POINT       ptAct;
    POINT       ptD;
    POINT       ptStart;
    POINT       ptChg;
    uint16_t    t_merge_0740_0001;
    uint16_t    t_merge_0db4_0001;
    HWND        t_merge_0ded_0001;
    uint16_t    t_merge_10ae_0001;
    uint16_t    t_merge_150a_0001;
    uint16_t    t_merge_152e_0001;
    HICON       t_merge_16e3_0001;
    uint16_t    t_merge_1887_0001;

L_06d6:
    goto L_1d70;

L_06eb:
    hdc = GetDC(hwnd);
    FCreateFonts(hdc);
    GetTextMetrics(hdc, &(tm));
    dySysFont = tm.tmHeight;
    dySBar = ((dyArial8 + 0xc) * 0x2);
    ReleaseDC(hwnd, hdc);
    InitTiles();
    if ((iWindowLayout != 0x2))
        goto L_073d;
    else
        goto L_0737;

L_0737:
    t_merge_0740_0001 = 0x1;
    goto L_0740;

L_073d:
    t_merge_0740_0001 = 0x0;

L_0740:
    EnsureTileSize(t_merge_0740_0001);

MapIt:
    if ((hwndTitle == 0x0))
        goto L_076c;
    else
        goto L_0756;

L_0756:
    return SendMessage(hwndTitle, msg, wParam, lParam);

L_076c:
    hdc = GetDC(hwnd);
    hpalSav = SelectPalette(hdc, vhpal, 0x0);
    i = RealizePalette(hdc);
    SelectPalette(hdc, hpalSav, 0x0);
    ReleaseDC(hwnd, hdc);
    if ((i == 0x0))
        goto L_07d2;
    else
        goto L_07b4;

L_07b4:
    InvalidateRect(hwnd, 0x0, 0x1);
    return 0x10000;

L_07d2:
    return 0x0;

L_07de:
    if ((wParam != hwnd))
        goto MapIt;
    else
        goto L_07e2;

L_07e2:

L_07eb:
    InitializeMenu(wParam);
    goto L_1e2b;

L_07f7:
    idPlayer = 0xffff;
    if ((ini.fCmdLine == 0x0))
        goto LShowStartup;
    else
        goto L_080d;

L_080d:
    ini.fCmdLine = 0x0;
    if ((ini.fValidate == 0x0))
        goto L_09ca;
    else
        goto L_082c;

L_082c:
    fFileErrSilent = 0x1;
    ClearFile(0x7);
    if ((FLoadGame(0x56a2, 0x38e) == 0x0))
        goto LExit;
    else
        goto L_0856;

L_0856:
    VerifyTurns();
    DestroyCurGame();
    EnsureAis();
    _wsprintf(&(szTemp), "\"%s\" Year: %d", 0x90, 0x25, (game.turn + 0x960));
    OutputSz(0x7, &(szTemp));
    i = 0x0;
    goto L_08a8;

L_08a4:
    i = (i + 0x1);

L_08a8:
    if ((i >= game.cPlayer))
        goto LExit;
    else
        goto L_08b3;

L_08b3:
    if (((rgOut[i] + 0x1) <= 0x3))
        goto L_08ea;
    else
        goto L_08c7;

L_08c7:
    ich = _wsprintf(&(szTemp), "Error: %d: ", (i + 0x1));
    goto L_090a;

L_08ea:
    ich = _wsprintf(&(szTemp), "%d: ", (i + 0x1));

L_090a:
    if ((gd.fNoHostNames != 0x0))
        goto L_095e;
    else
        goto L_091d;

L_091d:
    ich = (ich + _wsprintf(&(szTemp[ich]), "\"%s\" ", PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0), 0x25));

L_095e:
    /* untranslated: call strcat(szTemp, PszGetCompressedString(part[122:0](szDirName[rgOut[i]]))) -> callresult(char *) */
    /* untranslated: branch ((part[84:2](rgplr[i]) >> 0x4) & 0x1) == 0x0 ? L_09b1 : L_09a1 */

L_09a1:
    strcat(szTemp, 0x3b7);

L_09b1:
    OutputSz(0x7, &(szTemp));
    goto L_08a4;

L_09ca:
    if ((ini.fNewGame == 0x0))
        goto L_0a00;
    else
        goto L_09dd;

L_09dd:
    if ((LOWORD(vSerialNumber) != 0x0))
        goto L_09f1;
    else
        goto L_09e7;

L_09e7:
    if ((HIWORD(vSerialNumber) == 0x0))
        goto LExit;
    else
        goto L_09f1;

L_09f1:
    GenNewGameFromFile(0x56a2);

L_0a00:
    if ((ini.fGen == 0x0))
        goto L_0b67;
    else
        goto LBatchNext;

LBatchNext:
    if ((ini.fWait != 0x0))
        goto L_0a3c;
    else
        goto L_0a26;

L_0a26:
    if ((ini.fTry == 0x0))
        goto L_0a49;
    else
        goto L_0a3c;

L_0a3c:
    if ((CTurnsOutSafe() != 0x0))
        goto L_0b25;
    else
        goto L_0a49;

L_0a49:
    EnsureAis();
    FGenerateTurn();
    if ((ini.fBatch == 0x0))
        goto L_0adf;
    else
        goto L_0a66;

L_0a66:
    if ((LOWORD(lpchBatch) >= LOWORD(lpchBatchMac)))
        goto L_0adf;
    else
        goto LTryNextBatch;

LTryNextBatch:
    DestroyCurGame();
    pch = 0x56a2;

L_0a80:
    if (((uint16_t)(*(lpchBatch)) == 0xa))
        goto L_0ac1;
    else
        goto L_0a90;

L_0a90:
    if ((LOWORD(lpchBatch) != LOWORD(lpchBatchMac)))
        goto L_0aa9;
    else
        goto L_0aa0;

L_0aa0:
    if ((HIWORD(lpchBatch) == HIWORD(lpchBatchMac)))
        goto L_0ac1;
    else
        goto L_0aa9;

L_0aa9:
    *(pch) = *(lpchBatch);
    lpchBatch = (lpchBatch + 0x1);
    pch = (pch + 0x1);
    goto L_0a80;

L_0ac1:
    lpchBatch = (lpchBatch + 0x1);
    *(pch - 0x1) = 0x0;
    ini.fStartupFile = 0x1;
    goto LBatchNext;

L_0adf:
    if ((ini.cTurnGen == 0x0))
        goto LExit;
    else
        goto L_0ae9;

L_0ae9:
    ini.cTurnGen = (ini.cTurnGen - 0x1);
    goto LBatchNext;

LExit:
    if ((gd.fExitWindows == 0x0))
        goto L_0b16;
    else
        goto L_0b04;

L_0b04:
    ExitWindows((uint32_t)(vretExitValue), 0x0);
    goto L_1e2b;

L_0b16:
    PostQuitMessage(vretExitValue);

L_0b25:
    if ((ini.fTry == 0x0))
        goto L_0b67;
    else
        goto L_0b3b;

L_0b3b:
    if ((ini.fBatch == 0x0))
        goto LExit;
    else
        goto L_0b4e;

L_0b4e:
    if ((LOWORD(lpchBatch) < LOWORD(lpchBatchMac)))
        goto LTryNextBatch;
    else
        goto L_0b5b;

L_0b5b:

L_0b67:
    CommandHandler(hwnd, 0xed9);
    if ((ini.fTry != 0x0))
        goto LExit;
    else
        goto L_0b87;

L_0b87:

L_0b90:
    if ((ini.fGen != 0x0))
        goto LNop;
    else
        goto L_0ba1;

L_0ba1:

L_0baa:
    if ((LOWORD(game) != 0x0))
        goto L_0bc4;
    else
        goto L_0bb4;

L_0bb4:
    if ((HIWORD(game) == 0x0))
        goto LShowStartup;
    else
        goto L_0bbb;

L_0bbb:

L_0bc4:
    if ((idPlayer == 0xffff))
        goto L_0c8e;
    else
        goto L_0bce;

L_0bce:
    if ((ini.fDumpPlanets != 0x0))
        goto L_0c07;
    else
        goto L_0be1;

L_0be1:
    if ((ini.fDumpFleets != 0x0))
        goto L_0c07;
    else
        goto L_0bf4;

L_0bf4:
    if ((ini.fDumpMap == 0x0))
        goto L_0c8e;
    else
        goto L_0c07;

L_0c07:
    if ((ini.fDumpMap == 0x0))
        goto L_0c33;
    else
        goto L_0c1a;

L_0c1a:
    PostMessage(hwndFrame, WM_COMMAND, 0x55, 0x0);

L_0c33:
    if ((ini.fDumpPlanets == 0x0))
        goto L_0c5f;
    else
        goto L_0c46;

L_0c46:
    PostMessage(hwndFrame, WM_COMMAND, 0x54, 0x0);

L_0c5f:
    if ((ini.fDumpFleets == 0x0))
        goto LExit;
    else
        goto L_0c72;

L_0c72:
    PostMessage(hwndFrame, WM_COMMAND, 0x53, 0x0);

L_0c8e:
    ShowWindow(hwndFrame, SW_SHOW);
    InitializeMenu(0x0);
    PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((ini.fWait == 0x0))
        goto LNop;
    else
        goto L_0cd2;

L_0cd2:
    ini.fWait = 0x0;
    CommandHandler(hwnd, 0x6a);

LShowStartup:
    if ((hwndTitle != 0x0))
        goto L_0d56;
    else
        goto L_0cf8;

L_0cf8:
    pt.x = GetSystemMetrics(SM_CXSCREEN);
    pt.y = GetSystemMetrics(SM_CYSCREEN);
    hwndTitle = CreateWindow(szTitle, "Stars!", 0x90000000, 0x0, 0x0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    fFreeingTitle = 0x0;

L_0d56:
    ini.fStartupFile = 0x0;
    DestroyCurGame();

LNop:
    if ((LOWORD(vSerialNumber) != 0x0))
        goto L_0d7b;
    else
        goto L_0d71;

L_0d71:
    if ((HIWORD(vSerialNumber) == 0x0))
        goto L_0d97;
    else
        goto L_0d7b;

L_0d7b:
    if ((memcmp(0x519e, 0x5468, 0xb) == 0x0))
        goto L_1e2b;
    else
        goto L_0d97;

L_0d97:
    if ((LOWORD(vSerialNumber) != 0x0))
        goto L_0dab;
    else
        goto L_0da1;

L_0da1:
    if ((HIWORD(vSerialNumber) == 0x0))
        goto L_0db1;
    else
        goto L_0dab;

L_0dab:
    t_merge_0db4_0001 = 0x1;
    goto L_0db4;

L_0db1:
    t_merge_0db4_0001 = 0x0;

L_0db4:
    /* untranslated: part[200:2](szWork) = lobyte(t_merge_0db4_0001) */
    lpProc = MakeProcInstance(MsgDlg, hInst);
    if ((hwndTitle == 0x0))
        goto L_0dea;
    else
        goto L_0de4;

L_0de4:
    t_merge_0ded_0001 = hwndTitle;
    goto L_0ded;

L_0dea:
    t_merge_0ded_0001 = hwndFrame;

L_0ded:
    fRet = DialogBox(hInst, IDD_GENERIC_SMALL, t_merge_0ded_0001, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet == 0x0))
        goto L_0e80;
    else
        goto L_0e10;

L_0e10:
    if ((FValidSerialNo(0x57a4, &(lSerial)) == 0x0))
        goto L_0e4c;
    else
        goto L_0e28;

L_0e28:
    vSerialNumber = lSerial;
    memcpy(0x519e, 0x5468, 0xb);
    goto L_0ea9;

L_0e4c:
    if ((LOWORD(vSerialNumber) != 0x0))
        goto L_0ea9;
    else
        goto L_0e56;

L_0e56:
    if ((HIWORD(vSerialNumber) != 0x0))
        goto L_0ea9;
    else
        goto L_0e60;

L_0e60:
    memcpy(0x519e, 0x5468, 0xb);
    PostQuitMessage(vretExitValue);

L_0e80:
    vSerialNumber = 0x0;
    memcpy(0x519e, 0x5468, 0xb);
    PostQuitMessage(vretExitValue);

L_0ea9:
    WriteIniSettings();

L_0eb1:
    if ((gd.fTutorial == 0x0))
        goto L_0edb;
    else
        goto L_0ec4;

L_0ec4:
    if ((tutor.fChange == 0x0))
        goto L_0edb;
    else
        goto L_0ed6;

L_0ed6:
    AdvanceTutor();

L_0edb:
    return 0x0;

L_0ee4:
    FGetSystemColors();
    return 0x0;

L_0ef2:
    /* untranslated: part[12:2](lParam) = 0x208 */
    /* untranslated: part[14:2](lParam) = 0x17c */
    return 0x0;

L_0f17:
    if ((wParam == 0x2))
        goto L_0f27;
    else
        goto L_0f1f;

L_0f1f:
    if ((wParam != 0x0))
        goto Default;
    else
        goto L_0f27;

L_0f27:
    vfs.dx = LOWORD(lParam);
    vfs.dy = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    RefitFrameChildren();
    return 0x0;

L_0f5b:
    CommandHandler(hwnd, wParam);
    goto L_1e2b;

L_0f68:
    DestroyWindow(hwnd);
    goto L_1e2b;

L_0f71:
    BringUpHostDlg();
    return 0x10000;

L_0f7f:
    ShowTutor(0x0);
    game.fDirty = 0x0;
    DestroyCurGame();
    fErrSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    gd.fDontDoLogFiles = 0x1;
    if ((FLoadGame(0x56a2, 0x3c8) != 0x0))
        goto L_0fdb;
    else
        goto L_0fc6;

L_0fc6:
    fFileErrSilent = fErrSav;
    gd.fDontDoLogFiles = 0x0;
    goto L_1e2b;

L_0fdb:
    gd.fDontDoLogFiles = 0x0;
    fFileErrSilent = fErrSav;
    idPlayer = 0x0;
    if ((wParam != 0x9ca))
        goto L_104a;
    else
        goto L_0ffc;

L_0ffc:
    gd.fGeneratingTurn = 0x1;
    _wsprintf(szWork, "%s.x1", 0x56a2, 0x25);
    if ((FLoadLogFile(0x57a4) == 0x0))
        goto L_103e;
    else
        goto L_1039;

L_1039:
    FRunLogFile();

L_103e:
    gd.fGeneratingTurn = 0x0;

L_104a:
    CreateChildWindows();
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((wParam != 0x9ca))
        goto L_108a;
    else
        goto L_1071;

L_1071:
    SendMessage(hwndMessage, WM_KEYDOWN, 0x23, 0x0);

L_108a:
    tutor.idt = 0x0;
    tutor.fTurnDone = 0x0;
    if ((wParam != 0x9ca))
        goto L_10ab;
    else
        goto L_10a5;

L_10a5:
    t_merge_10ae_0001 = 0x1;
    goto L_10ae;

L_10ab:
    t_merge_10ae_0001 = 0x0;

L_10ae:
    tutor.wFlags = ((tutor.wFlags & 0xfdff) | ((t_merge_10ae_0001 & 0x1) << 0x9));
    AdvanceTutor();
    goto L_1e2b;

L_10d4:
    if (((wParam & 0xfff0) == 0xf030))
        goto L_10ee;
    else
        goto L_10e1;

L_10e1:
    if (((wParam & 0xfff0) != 0xf120))
        goto Default;
    else
        goto L_10ee;

L_10ee:
    idCur = idPlayer;
    uTimerIdOld = uTimerId;
    if ((uTimerId == 0x0))
        goto L_111c;
    else
        goto L_1104;

L_1104:
    KillTimer(0x0, uTimerId);
    uTimerId = 0x0;
    CreateChildWindows();

L_111c:
    if ((idPlayer == 0xffff))
        goto L_126b;
    else
        goto L_1126;

L_1126:
    if ((FNewTurnAvail(idPlayer) == 0x0))
        goto L_126b;
    else
        goto L_113a;

L_113a:
    if ((uTimerIdOld == 0x0))
        goto L_116c;
    else
        goto L_1143;

L_1143:
    AlertSz(PszFormatIds(idsNewTurnAvailable, 0x0), MB_ICONASTERISK);
    id = 0x6;
    goto L_1190;

L_116c:
    id = AlertSz(PszFormatIds(idsNewTurnAvailableWouldLikeLoad, 0x0), 0x2023);

L_1190:
    if ((id != 0x6))
        goto L_11ff;
    else
        goto L_1199;

L_1199:
    _wsprintf(&(szExt), MPCTD, (idPlayer + 0x1));
    DestroyCurGame();
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_11f7;
    else
        goto L_11d3;

L_11d3:
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    goto L_124f;

L_11f7:
    CreateChildWindows();

L_11ff:
    if ((id != 0x2))
        goto L_124f;
    else
        goto L_1208;

L_1208:
    if ((uTimerIdOld == 0x0))
        goto L_122d;
    else
        goto L_1211;

L_1211:
    PostMessage(hwndFrame, WM_COMMAND, 0x6a, 0x0);
    goto L_1246;

L_122d:
    PostMessage(hwndFrame, WM_SYSCOMMAND, 0xf020, 0x0);

L_1246:
    return 0x10000;

L_124f:
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    goto Default;

L_126b:
    if ((uTimerIdOld == 0x0))
        goto Default;
    else
        goto L_1274;

L_1274:
    if ((uTimerType != 0xd))
        goto L_129a;
    else
        goto L_127e;

L_127e:
    PostMessage(hwnd, WM_STARS_HOST, 0x0, 0x0);
    goto Default;

L_129a:
    if ((uTimerType != 0xe))
        goto L_13a3;
    else
        goto L_12a4;

L_12a4:
    id = AlertSz(PszFormatIds(idsTurnHasSubmittedChangesMadeAfterTurn, 0x0), 0x2023);
    if ((id != 0x6))
        goto L_1378;
    else
        goto L_12d1;

L_12d1:
    if ((FMarkFile(dtLog, idPlayer, 0x2, 0x0) != 0x0))
        goto L_1378;
    else
        goto L_12f1;

L_12f1:
    AlertSz(PszFormatIds(idsNewTurnCurrentlyGeneratedHostNewTurn, 0x0), MB_ICONHAND);
    _wsprintf(&(szExt), MPCTD, (idPlayer + 0x1));
    DestroyCurGame();
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_1370;
    else
        goto L_134c;

L_134c:
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    goto L_13a3;

L_1370:
    CreateChildWindows();

L_1378:
    if ((id != 0x2))
        goto L_13a3;
    else
        goto L_1381;

L_1381:
    PostMessage(hwndFrame, WM_COMMAND, 0x6a, 0x0);
    return 0x10000;

L_13a3:
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((sel.pt.x <= 0x3e8))
        goto Default;
    else
        goto L_13c7;

L_13c7:
    if ((sel.pt.y <= 0x3e8))
        goto Default;
    else
        goto L_13d2;

L_13d2:
    /* untranslated: call CtrPointScan(words(sel.pt.y, sel.pt.x), 0x1) -> callresult(void) */

L_13e9:
    if ((hwndScanner == 0x0))
        goto L_141a;
    else
        goto L_13f3;

L_13f3:
    if ((wParam == 0x2d))
        goto L_1403;
    else
        goto L_13fb;

L_13fb:
    if ((wParam != 0x2b))
        goto L_141a;
    else
        goto L_1403;

L_1403:
    SendMessage(hwndScanner, WM_CHAR, wParam, lParam);
    goto L_1e2b;

L_141a:
    if ((hwndMessage == 0x0))
        goto L_1453;
    else
        goto L_1424;

L_1424:
    if ((wParam == 0x2d))
        goto L_143c;
    else
        goto L_142c;

L_142c:
    if ((wParam == 0x2b))
        goto L_143c;
    else
        goto L_1434;

L_1434:
    if ((wParam != 0xd))
        goto L_1453;
    else
        goto L_143c;

L_143c:
    SendMessage(hwndMessage, WM_CHAR, wParam, lParam);
    goto L_1e2b;

L_1453:
    if ((hwndPlanet == 0x0))
        goto L_1484;
    else
        goto L_145d;

L_145d:
    if ((wParam == 0x66))
        goto L_146d;
    else
        goto L_1465;

L_1465:
    if ((wParam != 0x46))
        goto L_1484;
    else
        goto L_146d;

L_146d:
    SendMessage(hwndPlanet, WM_CHAR, wParam, lParam);
    goto L_1e2b;

L_1484:
    if ((hwndPlanet == 0x0))
        goto L_14b7;
    else
        goto L_148e;

L_148e:
    if ((sel.grobj != grobjPlanet))
        goto L_14b7;
    else
        goto L_1498;

L_1498:
    if ((wParam == 0x71))
        goto L_14a8;
    else
        goto L_14a0;

L_14a0:
    if ((wParam != 0x51))
        goto L_14b7;
    else
        goto L_14a8;

L_14a8:
    ChangeProduction(0x0);
    goto L_1e2b;

L_14b7:
    if (((sel.grobj & 0x3) == 0x0))
        goto L_1e2b;
    else
        goto L_14c5;

L_14c5:
    iOffset = 0x0;
    idPlanet = 0x0;
    if ((wParam != 0x6e))
        goto L_14df;
    else
        goto L_14d7;

L_14d7:
    iOffset = 0x1;
    goto L_157c;

L_14df:
    if ((wParam == 0x4e))
        goto L_14ef;
    else
        goto L_14e7;

L_14e7:
    if ((wParam != 0x50))
        goto L_1534;
    else
        goto L_14ef;

L_14ef:
    if ((sel.grobj != grobjPlanet))
        goto L_151d;
    else
        goto L_14f9;

L_14f9:
    if ((wParam != 0x4e))
        goto L_1507;
    else
        goto L_1501;

L_1501:
    t_merge_150a_0001 = 0x1;
    goto L_150a;

L_1507:
    t_merge_150a_0001 = 0x0;

L_150a:
    idPlanet = IdFindAdjStarbase(sel.pl.id, t_merge_150a_0001);
    goto L_157c;

L_151d:
    if ((wParam != 0x4e))
        goto L_152b;
    else
        goto L_1525;

L_1525:
    t_merge_152e_0001 = 0x1;
    goto L_152e;

L_152b:
    t_merge_152e_0001 = 0xffff;

L_152e:
    iOffset = t_merge_152e_0001;

L_1534:
    if ((wParam != 0x70))
        goto L_1544;
    else
        goto L_153c;

L_153c:
    iOffset = 0xffff;
    goto L_157c;

L_1544:
    if ((wParam == 0x72))
        goto L_1554;
    else
        goto L_154c;

L_154c:
    if ((wParam != 0x52))
        goto L_157c;
    else
        goto L_1554;

L_1554:
    if ((sel.grobj != grobjFleet))
        goto L_157c;
    else
        goto L_155e;

L_155e:
    /* untranslated: call ShipCommandProc(hwndPlanet, 0x0, words(0x0, (part[12:2](rghwndBtn) | 0x0))) -> callresult(void) */

L_157c:
    if ((iOffset != 0x0))
        goto L_158e;
    else
        goto L_1585;

L_1585:
    if ((idPlanet == 0x0))
        goto L_1e2b;
    else
        goto L_158e;

L_158e:
    if ((sel.grobj != grobjFleet))
        goto L_15a9;
    else
        goto L_1598;

L_1598:
    SelectAdjFleet(iOffset, idPlanet);
    goto L_1e2b;

L_15a9:
    SelectAdjPlanet(iOffset, idPlanet);

L_15ba:
    if ((idPlayer != 0xffff))
        goto L_15cd;
    else
        goto L_15c4;

L_15c4:
    /* untranslated: return words(hiconHost, 0x0) */

L_15cd:
    if ((uTimerId != 0x0))
        goto L_15e0;
    else
        goto L_15d7;

L_15d7:
    /* untranslated: return words(hiconStars, 0x0) */

L_15e0:
    /* untranslated: return words(hiconWait, 0x0) */

L_15e9:
    if ((IsIconic(hwnd) == 0x0))
        goto L_161b;
    else
        goto L_15f7;

L_15f7:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrDesktop);
    goto L_1e2b;

L_161b:
    GetClientRect(hwnd, &(rc));
    if ((hwndScanner == 0x0))
        goto L_1669;
    else
        goto L_1632;

L_1632:
    GetClientRect(hwndScanner, &(rc2));
    MapWindowPoints(hwndScanner, hwnd, &(rc2), 0x2);
    ExcludeClipRect(wParam, rc2.left, rc2.top, rc2.right, rc2.bottom);

L_1669:
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_1683:
    if ((IsIconic(hwnd) == 0x0))
        goto L_16fc;
    else
        goto L_1691;

L_1691:
    hdc = BeginPaint(hwnd, &(ps));
    if ((idPlayer != 0xffff))
        goto L_16d0;
    else
        goto L_16b6;

L_16b6:
    if ((LOWORD(game) != 0x0))
        goto L_16ca;
    else
        goto L_16c0;

L_16c0:
    if ((HIWORD(game) == 0x0))
        goto L_16d0;
    else
        goto L_16ca;

L_16ca:
    t_merge_16e3_0001 = hiconHost;
    goto L_16e3;

L_16d0:
    if ((uTimerId != 0x0))
        goto L_16e0;
    else
        goto L_16da;

L_16da:
    t_merge_16e3_0001 = hiconStars;
    goto L_16e3;

L_16e0:
    t_merge_16e3_0001 = hiconWait;

L_16e3:
    DrawIcon(hdc, 0x2, 0x2, t_merge_16e3_0001);
    EndPaint(hwnd, &(ps));
    goto L_1e2b;

L_16fc:
    hdc = BeginPaint(hwnd, &(ps));
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    goto L_19e9;

L_1721:
    PatBlt(hdc, (vfs.xTop + 0x5), 0x0, 0x2, vfs.dy, PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y1 + 0x5), (vfs.xTop + 0x2), 0x2, PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y2 + 0x5), (vfs.xTop + 0x2), 0x2, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (vfs.xTop + 0x1), 0x0, 0x1, (vfs.y1 + 0x2), PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y1 + 0x1), (vfs.xTop + 0x1), 0x1, PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x1), (vfs.y1 + 0x6), 0x1, ((vfs.y2 - vfs.y1) + 0xfffc), PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y2 + 0x1), (vfs.xTop + 0x1), 0x1, PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x1), (vfs.y2 + 0x6), 0x1, ((vfs.dy - vfs.y2) + 0xfffa), PATCOPY);
    goto L_1a04;

L_186b:
    if ((gd.fToolbar == 0x0))
        goto L_1884;
    else
        goto L_187e;

L_187e:
    t_merge_1887_0001 = 0x24;
    goto L_1887;

L_1884:
    t_merge_1887_0001 = 0x0;

L_1887:
    yOffset = t_merge_1887_0001;
    PatBlt(hdc, (vfs.xTop + 0x5), yOffset, 0x2, ((vfs.y2 + 0x2) - yOffset), PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y1 + 0x5), (vfs.xTop + 0x2), 0x2, PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x5), (vfs.y2 + 0x5), ((vfs.dx - vfs.xTop) + 0xfffb), 0x2, PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x5), (vfs.y2 + 0x6), 0x2, ((vfs.dy - vfs.y2) + 0xfffb), PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, (vfs.xTop + 0x1), yOffset, 0x1, ((vfs.y1 + 0x2) - yOffset), PATCOPY);
    PatBlt(hdc, 0x0, (vfs.y1 + 0x1), (vfs.xTop + 0x1), 0x1, PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x1), (vfs.y1 + 0x6), 0x1, ((vfs.dy - vfs.y1) + 0xfffa), PATCOPY);
    PatBlt(hdc, (vfs.xTop + 0x6), (vfs.y2 + 0x1), ((vfs.dx - vfs.xTop) + 0xfffa), 0x1, PATCOPY);
    goto L_1a04;

L_19e9:
    if ((iWindowLayout == 0x0))
        goto L_1721;
    else
        goto L_19f1;

L_19f1:
    if ((iWindowLayout == 0x1))
        goto L_186b;
    else
        goto L_19f9;

L_19f9:
    if ((iWindowLayout != 0x2))
        goto L_1721;
    else
        goto L_19fe;

L_19fe:

L_1a04:
    SelectObject(hdc, hbrSav);
    EndPaint(hwnd, &(ps));
    goto L_1e2b;

L_1a1f:
    hcs = 0x0;
    if ((IsIconic(hwnd) != 0x0))
        goto Default;
    else
        goto L_1a2f;

L_1a2f:

L_1a35:
    GetCursorPos(&(pt));
    ScreenToClient(hwndFrame, &(pt));
    GetClientRect(hwnd, &(rc));
    /* untranslated: branch PtInRect(&rc, words(pt.y, pt.x)) == 0x0 ? L_1d5a : L_1a78 */

L_1a78:
    /* untranslated: hcs = HcrsFromFrameWindowPt(words(pt.y, pt.x), 0x0) */
    if ((hcs == 0x0))
        goto Default;
    else
        goto L_1a96;

L_1a96:
    SetCursor(hcs);
    return 0x10000;

L_1aaa:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    /* untranslated: branch HcrsFromFrameWindowPt(words(pt.y, pt.x), &grSel) == 0x0 ? L_1e2b : L_1ae1 */

L_1ae1:

L_1ae7:
    hdc = GetDC(hwnd);
    hbrSav = SelectObject(hdc, hbr50Screen);
    ptD.y = 0x0;
    ptD.x = 0x0;
    /* untranslated: call InvertPaneBorder(hdc, grSel, words(ptD.y, ptD.x), 0x0) -> callresult(POINT) */
    ptStart.x = pt.x;
    ptStart.y = pt.y;
    SetCapture(hwnd);
    ptOld.x = pt.x;
    ptOld.y = pt.y;
    ptAct.y = 0x0;
    ptAct.x = 0x0;

L_1b4b:
    if ((FGetMouseMove(&(pt)) == 0x0))
        goto L_1bc6;
    else
        goto L_1b5f;

L_1b5f:
    if ((pt.x != ptOld.x))
        goto L_1b75;
    else
        goto L_1b6a;

L_1b6a:
    if ((pt.y == ptOld.y))
        goto L_1b4b;
    else
        goto L_1b75;

L_1b75:
    ptD.x = (pt.x - ptStart.x);
    ptD.y = (pt.y - ptStart.y);
    ptChg.x = (pt.x - ptOld.x);
    ptChg.y = (pt.y - ptOld.y);
    /* untranslated: call InvertPaneBorder(hdc, grSel, words(ptD.y, ptD.x), &ptChg) -> callresult(POINT) */
    /* untranslated: ptAct.x = loword(callresult(POINT)) */
    /* untranslated: ptAct.y = hiword(callresult(POINT)) */
    ptOld.x = pt.x;
    ptOld.y = pt.y;

L_1bc6:
    /* untranslated: call InvertPaneBorder(hdc, grSel, words(ptD.y, ptD.x), 0x0) -> callresult(POINT) */
    ReleaseCapture();
    SelectObject(hdc, hbrSav);
    ReleaseDC(hwnd, hdc);
    if ((ptAct.x != 0x0))
        goto L_1c09;
    else
        goto L_1c00;

L_1c00:
    if ((ptAct.y == 0x0))
        goto L_1e2b;
    else
        goto L_1c09;

L_1c09:
    if (((grSel & 0x1) == 0x0))
        goto L_1c36;
    else
        goto L_1c17;

L_1c17:
    if ((iWindowLayout != 0x0))
        goto L_1c2d;
    else
        goto L_1c21;

L_1c21:
    vfs.dxPlanWant = (vfs.xTop + ptAct.x);
    goto L_1c36;

L_1c2d:
    vfs.dx2PlanWant = (vfs.xTop + ptAct.x);

L_1c36:
    if (((grSel & 0x2) == 0x0))
        goto L_1c71;
    else
        goto L_1c44;

L_1c44:
    if ((iWindowLayout != 0x0))
        goto L_1c61;
    else
        goto L_1c4e;

L_1c4e:
    vfs.dyMsgWant = (((vfs.y2 - vfs.y1) + 0xfff8) - ptAct.y);
    goto L_1c71;

L_1c61:
    vfs.dy2MsgWant = (((vfs.dy - vfs.y1) + 0xfff8) - ptAct.y);

L_1c71:
    if (((grSel & 0x4) == 0x0))
        goto L_1cbc;
    else
        goto L_1c7f;

L_1c7f:
    if ((iWindowLayout != 0x0))
        goto L_1cac;
    else
        goto L_1c89;

L_1c89:
    vfs.dyMsgWant = (((vfs.y2 - vfs.y1) + 0xfff8) + ptAct.y);
    vfs.dyMinWant = (((vfs.dy - vfs.y2) + 0xfff8) - ptAct.y);
    goto L_1cbc;

L_1cac:
    vfs.dy2MinWant = (((vfs.dy - vfs.y2) + 0xfff8) - ptAct.y);

L_1cbc:
    InvalidateRect(hwnd, 0x0, 0x1);
    RefitFrameChildren();

L_1cd6:
    if ((uTimerId == 0x0))
        goto L_1ced;
    else
        goto L_1ce0;

L_1ce0:
    KillTimer(0x0, uTimerId);

L_1ced:
    WriteIniSettings();
    uTimerId = 0x0;
    if ((gd.fHostMode == 0x0))
        goto L_1d24;
    else
        goto L_1d0c;

L_1d0c:
    FMarkFile(dtHost, 0xffff, 0x1, 0x0);

L_1d24:
    DestroyCurGame();
    if ((gd.fExitWindows == 0x0))
        goto L_1d4e;
    else
        goto L_1d3c;

L_1d3c:
    ExitWindows((uint32_t)(vretExitValue), 0x0);
    goto L_1e2b;

L_1d4e:
    PostQuitMessage(vretExitValue);

Default:
    return DefWindowProc(hwnd, msg, wParam, lParam);

L_1d70:
    if ((msg == WM_CREATE))
        goto L_06eb;
    else
        goto L_1d78;

L_1d78:
    if ((msg == WM_DESTROY))
        goto L_1cd6;
    else
        goto L_1d80;

L_1d80:
    if ((msg == WM_SIZE))
        goto L_0f17;
    else
        goto L_1d88;

L_1d88:
    if ((msg == WM_ACTIVATE))
        goto L_1e2b;
    else
        goto L_1d90;

L_1d90:
    if ((msg == WM_PAINT))
        goto L_1683;
    else
        goto L_1d98;

L_1d98:
    if ((msg == WM_CLOSE))
        goto L_0f68;
    else
        goto L_1da0;

L_1da0:
    if ((msg == WM_ERASEBKGND))
        goto L_15e9;
    else
        goto L_1da8;

L_1da8:
    if ((msg == WM_SYSCOLORCHANGE))
        goto L_0ee4;
    else
        goto L_1db0;

L_1db0:
    if ((msg == WM_WININICHANGE))
        goto L_0ee4;
    else
        goto L_1db8;

L_1db8:
    if ((msg == WM_SETCURSOR))
        goto L_1a1f;
    else
        goto L_1dc0;

L_1dc0:
    if ((msg == WM_GETMINMAXINFO))
        goto L_0ef2;
    else
        goto L_1dc8;

L_1dc8:
    if ((msg == WM_QUERYDRAGICON))
        goto L_15ba;
    else
        goto L_1dd0;

L_1dd0:
    if ((msg == WM_CHAR))
        goto L_13e9;
    else
        goto L_1dd8;

L_1dd8:
    if ((msg == WM_COMMAND))
        goto L_0f5b;
    else
        goto L_1de0;

L_1de0:
    if ((msg == WM_SYSCOMMAND))
        goto L_10d4;
    else
        goto L_1de8;

L_1de8:
    if ((msg == WM_INITMENU))
        goto L_07eb;
    else
        goto L_1df0;

L_1df0:
    if ((msg == WM_ENTERIDLE))
        goto L_0eb1;
    else
        goto L_1df8;

L_1df8:
    if ((msg == WM_LBUTTONDOWN))
        goto L_1aaa;
    else
        goto L_1e00;

L_1e00:
    if ((msg == WM_QUERYNEWPALETTE))
        goto MapIt;
    else
        goto L_1e08;

L_1e08:
    if ((msg == WM_PALETTECHANGED))
        goto L_07de;
    else
        goto L_1e10;

L_1e10:
    if ((msg == WM_STARS_STARTUP))
        goto L_07f7;
    else
        goto L_1e18;

L_1e18:
    if ((msg == WM_STARS_HOST))
        goto L_0f71;
    else
        goto L_1e20;

L_1e20:
    if ((msg != WM_STARS_CONTINUE))
        goto Default;
    else
        goto L_1e25;

L_1e25:

L_1e2b:
    return 0x0;
}

POINT InvertPaneBorder(HDC hdc, int16_t grSel, POINT dpt, POINT *pdptPrev) {
    int16_t notMin;
    int16_t dChg;
    POINT   dptT;
    POINT   dptPrev;
    int16_t dyAboveMinCur;
    POINT   dptOld;
    int16_t dyMsgCur;
    int16_t dyMinAboveH2;
    int16_t dyPlanMin;
    int16_t dxScanMin;
    int16_t x;
    POINT   pt;
    int16_t dyMin;

L_1e3c:
    dptOld.x = dpt.x;
    dptOld.y = dpt.y;
    if ((pdptPrev == 0x0))
        goto L_1e92;
    else
        goto L_1e5a;

L_1e5a:
    if ((grSel == 0x1))
        goto L_1e92;
    else
        goto L_1e63;

L_1e63:
    pt.x = (dpt.x - pdptPrev->x);
    pt.y = (dpt.y - pdptPrev->y);
    /* untranslated: call InvertPaneBorder(hdc, grSel, words(pt.y, pt.x), 0x0) -> callresult(POINT) */

L_1e92:
    goto L_1f00;

L_1e98:
    dxScanMin = 0x64;
    dyPlanMin = 0x32;
    dyMsgCur = ((vfs.y2 - vfs.y1) + 0xfff8);
    dyAboveMinCur = ((vfs.y2 - vfs.y1) + 0xfff8);
    notMin = 0x1;
    dyMinAboveH2 = ((LOWORD((0xd * dyArial8)) >> 0x1) + 0xa);
    goto L_1f1b;

L_1ed3:
    dxScanMin = 0xc8;
    dyPlanMin = 0x64;
    dyMsgCur = ((vfs.dy - vfs.y1) + 0xfff8);
    dyAboveMinCur = vfs.y2;
    notMin = 0xffff;
    dyMinAboveH2 = 0x64;
    goto L_1f1b;

L_1f00:
    if ((iWindowLayout == 0x0))
        goto L_1e98;
    else
        goto L_1f08;

L_1f08:
    if ((iWindowLayout == 0x1))
        goto L_1ed3;
    else
        goto L_1f10;

L_1f10:
    if ((iWindowLayout != 0x2))
        goto L_1e98;
    else
        goto L_1f15;

L_1f15:

L_1f1b:
    if (((vfs.xTop + dpt.x) >= 0xc6))
        goto L_1f33;
    else
        goto L_1f29;

L_1f29:
    dpt.x = (0xc6 - vfs.xTop);

L_1f33:
    if ((((vfs.dx - vfs.xTop) - dpt.x) >= dxScanMin))
        goto L_1f52;
    else
        goto L_1f45;

L_1f45:
    dpt.x = ((vfs.dx - vfs.xTop) - dxScanMin);

L_1f52:
    if (((vfs.xTop + dpt.x) <= 0x18c))
        goto L_1f6a;
    else
        goto L_1f60;

L_1f60:
    dpt.x = (0x18c - vfs.xTop);

L_1f6a:
    if (((grSel & 0x2) == 0x0))
        goto L_1fb9;
    else
        goto L_1f78;

L_1f78:
    if (((vfs.y1 + dpt.y) >= dyPlanMin))
        goto L_1f93;
    else
        goto L_1f86;

L_1f86:
    dpt.y = (dyPlanMin - vfs.y1);
    goto L_1fb9;

L_1f93:
    dyMin = ((LOWORD((0xd * dyArial8)) >> 0x1) + 0xa);
    if (((dyMsgCur - dpt.y) >= dyMin))
        goto L_1fb9;
    else
        goto L_1fb0;

L_1fb0:
    dpt.y = (dyMsgCur - dyMin);

L_1fb9:
    if (((grSel & 0x4) == 0x0))
        goto L_2016;
    else
        goto L_1fc7;

L_1fc7:
    if (((dyAboveMinCur + dpt.y) >= dyMinAboveH2))
        goto L_1fe4;
    else
        goto L_1fd5;

L_1fd5:
    dpt.y = LOWORD(((dyMinAboveH2 - dyAboveMinCur) * notMin));
    goto L_2016;

L_1fe4:
    dyMin = (LOWORD((0xd * dyArial8)) + 0xffdc);
    if (((((vfs.dy - vfs.y2) + 0xfff8) - dpt.y) >= dyMin))
        goto L_2016;
    else
        goto L_2006;

L_2006:
    dpt.y = (((vfs.dy - vfs.y2) + 0xfff8) - dyMin);

L_2016:
    if ((pdptPrev == 0x0))
        goto L_215e;
    else
        goto L_201f;

L_201f:
    dptT.x = dpt.x;
    dptT.y = dpt.y;
    dpt.x = (dptOld.x - pdptPrev->x);
    dpt.y = (dptOld.y - pdptPrev->y);
    if (((vfs.xTop + dpt.x) >= 0xc6))
        goto L_205a;
    else
        goto L_2050;

L_2050:
    dpt.x = (0xc6 - vfs.xTop);

L_205a:
    if ((((vfs.dx - vfs.xTop) - dpt.x) >= dxScanMin))
        goto L_2079;
    else
        goto L_206c;

L_206c:
    dpt.x = ((vfs.dx - vfs.xTop) - dxScanMin);

L_2079:
    if (((vfs.xTop + dpt.x) <= 0x18c))
        goto L_2091;
    else
        goto L_2087;

L_2087:
    dpt.x = (0x18c - vfs.xTop);

L_2091:
    if (((grSel & 0x2) == 0x0))
        goto L_20e3;
    else
        goto L_209f;

L_209f:
    if (((vfs.y1 + dpt.y) >= 0x32))
        goto L_20ba;
    else
        goto L_20ad;

L_20ad:
    dpt.y = (0x32 - vfs.y1);
    goto L_2140;

L_20ba:
    dyMin = ((LOWORD((0xd * dyArial8)) >> 0x1) + 0xa);
    if (((dyMsgCur - dpt.y) >= dyMin))
        goto L_2140;
    else
        goto L_20d7;

L_20d7:
    dpt.y = (dyMsgCur - dyMin);

L_20e3:
    if (((grSel & 0x4) == 0x0))
        goto L_2140;
    else
        goto L_20f1;

L_20f1:
    if (((dyAboveMinCur + dpt.y) >= dyMinAboveH2))
        goto L_210e;
    else
        goto L_20ff;

L_20ff:
    dpt.y = LOWORD(((dyMinAboveH2 - dyAboveMinCur) * notMin));
    goto L_2140;

L_210e:
    dyMin = (LOWORD((0xd * dyArial8)) + 0xffdc);
    if (((((vfs.dy - vfs.y2) + 0xfff8) - dpt.y) >= dyMin))
        goto L_2140;
    else
        goto L_2130;

L_2130:
    dpt.y = (((vfs.dy - vfs.y2) + 0xfff8) - dyMin);

L_2140:
    dptPrev.x = (dptT.x - dpt.x);
    dptPrev.y = (dptT.y - dpt.y);
    dpt.x = dptT.x;
    dpt.y = dptT.y;

L_215e:
    goto L_247e;

L_2164:
    dpt.y = 0x0;
    if ((pdptPrev == 0x0))
        goto L_2185;
    else
        goto L_2172;

L_2172:
    if ((abs(dptPrev.x) < 0x6))
        goto L_21e0;
    else
        goto L_2185;

L_2185:
    if ((pdptPrev == 0x0))
        goto L_21b7;
    else
        goto L_218e;

L_218e:
    PatBlt(hdc, (((vfs.xTop + dpt.x) - dptPrev.x) + 0x1), 0x0, 0x6, vfs.dy, PATINVERT);

L_21b7:
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x1), 0x0, 0x6, vfs.dy, PATINVERT);
    goto L_2261;

L_21e0:
    dChg = dptPrev.x;
    if ((dChg == 0x0))
        goto L_24a0;
    else
        goto L_21ec;

L_21ec:

L_21f2:
    if ((dChg >= 0x0))
        goto L_2212;
    else
        goto L_21fb;

L_21fb:
    x = ((vfs.xTop + dpt.x) + 0x1);
    /* untranslated: dChg = (dChg neg 0x0) */
    goto L_2221;

L_2212:
    x = (((vfs.xTop + dpt.x) - dptPrev.x) + 0x1);

L_2221:
    PatBlt(hdc, x, 0x0, dChg, vfs.dy, PATINVERT);
    PatBlt(hdc, (x + 0x6), 0x0, dChg, vfs.dy, PATINVERT);

L_2261:
    goto L_24a0;
    dpt.x = 0x0;
    PatBlt(hdc, 0x0, ((vfs.y1 + dpt.y) + 0x1), (vfs.xTop + 0x1), 0x6, PATINVERT);
    goto L_24a0;
    dpt.x = 0x0;
    if ((iWindowLayout != 0x0))
        goto L_22d0;
    else
        goto L_22a4;

L_22a4:
    PatBlt(hdc, 0x0, ((vfs.y2 + dpt.y) + 0x1), (vfs.xTop + 0x1), 0x6, PATINVERT);
    goto L_2300;

L_22d0:
    PatBlt(hdc, (vfs.xTop + 0x7), ((vfs.y2 + dpt.y) + 0x1), ((vfs.dx - vfs.xTop) + 0xfff9), 0x6, PATINVERT);

L_2300:
    goto L_24a0;
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x1), 0x0, 0x6, vfs.dy, PATINVERT);
    PatBlt(hdc, 0x0, ((vfs.y1 + dpt.y) + 0x1), ((vfs.xTop + dpt.x) + 0x1), 0x6, PATINVERT);
    goto L_24a0;
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x1), 0x0, 0x6, vfs.dy, PATINVERT);
    if ((iWindowLayout != 0x0))
        goto L_23b7;
    else
        goto L_2388;

L_2388:
    PatBlt(hdc, 0x0, ((vfs.y2 + dpt.y) + 0x1), ((vfs.xTop + dpt.x) + 0x1), 0x6, PATINVERT);
    goto L_23ed;

L_23b7:
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x7), ((vfs.y2 + dpt.y) + 0x1), (((vfs.dx - dpt.x) - vfs.xTop) + 0xfff9), 0x6, PATINVERT);

L_23ed:
    goto L_24a0;
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x1), 0x0, 0x6, vfs.dy, PATINVERT);
    PatBlt(hdc, 0x0, ((vfs.y1 + dpt.y) + 0x1), ((vfs.xTop + dpt.x) + 0x1), 0x6, PATINVERT);
    PatBlt(hdc, ((vfs.xTop + dpt.x) + 0x7), ((vfs.y2 + dpt.y) + 0x1), (((vfs.dx - dpt.x) - vfs.xTop) + 0xfff9), 0x6, PATINVERT);
    goto L_24a0;

L_247e:
    if (((grSel - 0x1) > 0x6))
        goto L_2164;
    else
        goto L_2489;

L_2489:
    goto L_ffffffff;

L_24a0:

L_24a9:
    /* untranslated: return words(dpt.x, dpt.y) */
}

uint16_t HcrsFromFrameWindowPt(POINT pt, int16_t *pgrSel) {
    uint16_t hcs;
    int16_t  fInHBar2;
    int16_t  fInHBar1;
    int16_t  fInVBar;
    uint16_t t_merge_24e0_0001;
    uint16_t t_merge_2513_0001;
    uint16_t t_merge_254c_0001;
    uint16_t t_merge_257f_0001;

L_24b0:
    hcs = 0x0;
    if ((pt.x < vfs.xTop))
        goto L_24dd;
    else
        goto L_24c9;

L_24c9:
    if ((pt.x >= (vfs.xTop + 0x8)))
        goto L_24dd;
    else
        goto L_24d7;

L_24d7:
    t_merge_24e0_0001 = 0x1;
    goto L_24e0;

L_24dd:
    t_merge_24e0_0001 = 0x0;

L_24e0:
    fInVBar = t_merge_24e0_0001;
    if ((pt.x >= (vfs.xTop + 0x8)))
        goto L_2510;
    else
        goto L_24f1;

L_24f1:
    if ((pt.y < vfs.y1))
        goto L_2510;
    else
        goto L_24fc;

L_24fc:
    if ((pt.y >= (vfs.y1 + 0x8)))
        goto L_2510;
    else
        goto L_250a;

L_250a:
    t_merge_2513_0001 = 0x1;
    goto L_2513;

L_2510:
    t_merge_2513_0001 = 0x0;

L_2513:
    fInHBar1 = t_merge_2513_0001;
    goto L_2588;

L_251c:
    if ((pt.x >= (vfs.xTop + 0x8)))
        goto L_2549;
    else
        goto L_252a;

L_252a:
    if ((pt.y < vfs.y2))
        goto L_2549;
    else
        goto L_2535;

L_2535:
    if ((pt.y >= (vfs.y2 + 0x8)))
        goto L_2549;
    else
        goto L_2543;

L_2543:
    t_merge_254c_0001 = 0x1;
    goto L_254c;

L_2549:
    t_merge_254c_0001 = 0x0;

L_254c:
    fInHBar2 = t_merge_254c_0001;
    goto L_25a3;

L_2552:
    if ((pt.x < vfs.xTop))
        goto L_257c;
    else
        goto L_255d;

L_255d:
    if ((pt.y < vfs.y2))
        goto L_257c;
    else
        goto L_2568;

L_2568:
    if ((pt.y >= (vfs.y2 + 0x8)))
        goto L_257c;
    else
        goto L_2576;

L_2576:
    t_merge_257f_0001 = 0x1;
    goto L_257f;

L_257c:
    t_merge_257f_0001 = 0x0;

L_257f:
    fInHBar2 = t_merge_257f_0001;
    goto L_25a3;

L_2588:
    if ((iWindowLayout == 0x0))
        goto L_251c;
    else
        goto L_2590;

L_2590:
    if ((iWindowLayout == 0x1))
        goto L_2552;
    else
        goto L_2598;

L_2598:
    if ((iWindowLayout != 0x2))
        goto L_251c;
    else
        goto L_259d;

L_259d:

L_25a3:
    if ((fInVBar == 0x0))
        goto L_25d0;
    else
        goto L_25ac;

L_25ac:
    if ((fInHBar1 != 0x0))
        goto L_25be;
    else
        goto L_25b5;

L_25b5:
    if ((fInHBar2 == 0x0))
        goto L_25c7;
    else
        goto L_25be;

L_25be:
    hcs = hcurResize4Way;
    goto L_25e8;

L_25c7:
    hcs = hcurResizeWE;

L_25d0:
    if ((fInHBar1 != 0x0))
        goto L_25e2;
    else
        goto L_25d9;

L_25d9:
    if ((fInHBar2 == 0x0))
        goto L_25e8;
    else
        goto L_25e2;

L_25e2:
    hcs = hcurResizeNS;

L_25e8:
    if ((pgrSel == 0x0))
        goto L_2607;
    else
        goto L_25f1;

L_25f1:
    *(pgrSel) = (((fInHBar1 * 0x2) + fInVBar) + (fInHBar2 * 0x4));

L_2607:

L_260d:
    return hcs;
}

void RestoreSelection() {
    PLANET *lppl;

L_2614:
    if ((ini.idPlayer != idPlayer))
        goto L_27ca;
    else
        goto L_2629;

L_2629:
    /* untranslated: branch part[18:2](ini) != LOWORD(game) ? L_27ca : L_2639 */

L_2639:
    /* untranslated: branch part[20:2](ini) != HIWORD(game) ? L_27ca : L_2642 */

L_2642:
    if ((ini.grobjSel != 0x2))
        goto L_26b6;
    else
        goto L_265a;

L_265a:
    if ((FAROFF(LpflFromId(ini.iObjSel)) != 0x0))
        goto L_2676;
    else
        goto L_266e;

L_266e:
    /* untranslated: branch farseg(callresult(FLEET *)) == 0x0 ? L_2695 : L_2676 */

L_2676:
    SelectAdjFleet(0x0, ini.iObjSel);
    ini.grobjSel = 0x0;
    goto L_2732;

L_2695:
    ini.grobjSel = 0x1;
    ini.iObjSel = rgplr[idPlayer].idPlanetHome;

L_26b6:
    if ((ini.grobjSel != 0x1))
        goto L_2732;
    else
        goto L_26ce;

L_26ce:
    lppl = LpplFromId(ini.iObjSel);
    if ((LOWORD(lppl) != 0x0))
        goto L_26f2;
    else
        goto L_26e9;

L_26e9:
    if ((HIWORD(lppl) == 0x0))
        goto L_2701;
    else
        goto L_26f2;

L_26f2:
    if ((lppl->iPlayer == idPlayer))
        goto L_2716;
    else
        goto L_2701;

L_2701:
    ini.iObjSel = rgplr[idPlayer].idPlanetHome;
    goto L_2732;

L_2716:
    SelectAdjPlanet(0x0, ini.iObjSel);
    ini.grobjSel = 0x0;

L_2732:
    if ((ini.turn != game.turn))
        goto L_27e8;
    else
        goto L_273e;

L_273e:
    if ((ini.iMsg <= 0x0))
        goto L_27e8;
    else
        goto L_2748;

L_2748:
    iMsgCur = (ini.iMsg + 0xffff);
    if ((iMsgCur < (cMsg + vcmsgplrIn)))
        goto L_2767;
    else
        goto L_2761;

L_2761:
    iMsgCur = 0xffff;

L_2767:
    iMsgCur = IMsgNext(0x0);
    gd.fGotoVCR = 0x0;
    SetMsgTitle(hwndMessage);
    InvalidateRect(hwndMessage, 0x0, 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_27e8;
    else
        goto L_27b6;

L_27b6:
    tutor.fChange = 0x1;
    AdvanceTutor();

L_27ca:
    ini.grobjSel = 0x1;
    ini.iObjSel = rgplr[idPlayer].idPlanetHome;

L_27e8:
    if ((ini.grobjSel == 0x0))
        goto L_2869;
    else
        goto L_2800;

L_2800:
    if ((ini.iObjSel == 0xffff))
        goto L_2858;
    else
        goto L_280a;

L_280a:
    lppl = LpplFromId(ini.iObjSel);
    if ((LOWORD(lppl) != 0x0))
        goto L_282e;
    else
        goto L_2825;

L_2825:
    if ((HIWORD(lppl) == 0x0))
        goto L_283d;
    else
        goto L_282e;

L_282e:
    if ((lppl->iPlayer == idPlayer))
        goto L_2845;
    else
        goto L_283d;

L_283d:
    FFindSomethingAndSelectIt();
    goto L_285d;

L_2845:
    SelectAdjPlanet(0x0, ini.iObjSel);

L_2858:
    FFindSomethingAndSelectIt();

L_285d:
    ini.grobjSel = 0x0;

L_2869:
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

L_2886:
    iPass = 0x0;
    PushRandom(0x11000b);
    Randomize(lSerial);
    LOWORD(rgbRaw) = LOWORD(lSerial);
    HIWORD(rgbRaw) = HIWORD(lSerial);
    /* untranslated: call memcpy(&part[4:2](rgbRaw), pbEnv, 0xb) -> callresult(void *) */
    iRaw = 0xf;
    i = 0x0;
    goto L_2963;

L_28e0:
    j = pbEnv[i];
    goto L_2903;

L_28f3:
    Random(0x10);
    j = (j - 0x1);

L_2903:
    if ((j > 0x0))
        goto L_28f3;
    else
        goto L_290c;

L_290c:
    if ((iPass != 0x0))
        goto L_292e;
    else
        goto L_2915;

L_2915:
    rgbRaw[iRaw] = LOBYTE(Random(0x10));
    goto L_2953;

L_292e:
    iRaw = (iRaw + 0x1);
    rgbRaw[iRaw] = (rgbRaw[iRaw] | LOBYTE(((Random(0x10) << 0x4) & 0xff)));

L_2953:
    iPass = ((iPass + 0x1) & 0x1);
    i = (i + 0x1);

L_2963:
    if ((i < 0xb))
        goto L_28e0;
    else
        goto L_296c;

L_296c:
    bXor = 0x0;
    i = 0x0;
    goto L_298c;

L_2978:
    bXor = (bXor ^ LOBYTE(rgbRaw[i]));
    i = (i + 0x1);

L_298c:
    if ((i < 0xf))
        goto L_2978;
    else
        goto L_2995;

L_2995:
    iRaw = (iRaw + 0x1);
    rgbRaw[iRaw] = (rgbRaw[iRaw] | LOBYTE((bXor << 0x4)));
    PopRandom();
    i = 0x0;
    goto L_29de;

L_29be:
    /* untranslated: rgbRaw2[i] = rgbRaw[byte 0x5:[i+0x2870]] */
    i = (i + 0x1);

L_29de:
    if ((i < 0x15))
        goto L_29be;
    else
        goto L_29e7;

L_29e7:
    iRaw = 0x0;
    cBits = 0x0;
    lTank = 0x0;
    i = 0x0;
    goto L_2ad6;

L_2a03:
    if ((cBits >= 0x6))
        goto L_2a2d;
    else
        goto L_2a0c;

L_2a0c:
    iRaw = (iRaw + 0x1);
    LOWORD(lTank) = (LOWORD(lTank) | (rgbRaw2[iRaw] << cBits));
    HIWORD(lTank) = (HIWORD(lTank) | SIGNHIWORD((rgbRaw2[iRaw] << cBits)));
    cBits = (cBits + 0x8);

L_2a2d:
    b64 = LOBYTE((LOWORD(lTank) & 0x3f));
    lTank = (int32_t)((lTank >> 0x6));
    cBits = (cBits - 0x6);
    if ((b64 >= 0x1a))
        goto L_2a73;
    else
        goto L_2a62;

L_2a62:
    *(pszOut) = LOBYTE((b64 + 0x41));
    goto L_2ace;

L_2a73:
    if ((b64 >= 0x34))
        goto L_2a92;
    else
        goto L_2a81;

L_2a81:
    *(pszOut) = LOBYTE((b64 + 0x47));
    goto L_2ace;

L_2a92:
    if ((b64 >= 0x3e))
        goto L_2ab1;
    else
        goto L_2aa0;

L_2aa0:
    *(pszOut) = LOBYTE((b64 + 0xfffc));
    goto L_2ace;

L_2ab1:
    if ((b64 != 0x3e))
        goto L_2ac8;
    else
        goto L_2abf;

L_2abf:
    *(pszOut) = 0x2d;
    goto L_2ace;

L_2ac8:
    *(pszOut) = 0x2a;

L_2ace:
    pszOut = (pszOut + 0x1);
    i = (i + 0x1);

L_2ad6:
    if ((i < 0x1c))
        goto L_2a03;
    else
        goto L_2adf;

L_2adf:
    *(pszOut) = 0x0;
    return;
}

int16_t FSerialAndEnvFromSz(int32_t *plSerial, uint8_t *pbEnv, char *pszIn) {
    uint8_t rgbRaw[21];
    int16_t fSuccess;
    int16_t j;
    uint8_t bXor;
    int16_t i;
    int16_t cBits;
    int16_t iRaw;
    int16_t iPass;
    int32_t lSerial;
    int32_t lTank;
    uint8_t rgbRaw2[21];
    uint8_t b64;

L_2aec:
    iPass = 0x0;
    plSerial = 0x0;
    *(plSerial + 0x2) = 0x0;
    memset(pbEnv, 0x0, 0xb);
    iRaw = 0x0;
    cBits = 0x0;
    lTank = 0x0;
    i = 0x0;
    goto L_2c2b;

L_2b38:
    if (((uint16_t)(*(pszIn)) < 0x41))
        goto L_2b63;
    else
        goto L_2b46;

L_2b46:
    if (((uint16_t)(*(pszIn)) > 0x5a))
        goto L_2b63;
    else
        goto L_2b54;

L_2b54:
    b64 = LOBYTE(((uint16_t)(*(pszIn)) + 0xffbf));
    goto L_2bd2;

L_2b63:
    if (((uint16_t)(*(pszIn)) < 0x61))
        goto L_2b8e;
    else
        goto L_2b71;

L_2b71:
    if (((uint16_t)(*(pszIn)) > 0x7a))
        goto L_2b8e;
    else
        goto L_2b7f;

L_2b7f:
    b64 = LOBYTE(((uint16_t)(*(pszIn)) + 0xffb9));
    goto L_2bd2;

L_2b8e:
    if (((uint16_t)(*(pszIn)) < 0x30))
        goto L_2bb9;
    else
        goto L_2b9c;

L_2b9c:
    if (((uint16_t)(*(pszIn)) > 0x39))
        goto L_2bb9;
    else
        goto L_2baa;

L_2baa:
    b64 = LOBYTE(((uint16_t)(*(pszIn)) + 0x4));
    goto L_2bd2;

L_2bb9:
    if (((uint16_t)(*(pszIn)) != 0x2d))
        goto L_2bce;
    else
        goto L_2bc7;

L_2bc7:
    b64 = 0x3e;
    goto L_2bd2;

L_2bce:
    b64 = 0x3f;

L_2bd2:
    LOWORD(lTank) = (LOWORD(lTank) | (b64 << cBits));
    HIWORD(lTank) = (HIWORD(lTank) | SIGNHIWORD((b64 << cBits)));
    cBits = (cBits + 0x6);
    pszIn = (pszIn + 0x1);

L_2bec:
    if ((cBits < 0x8))
        goto L_2b38;
    else
        goto L_2bf5;

L_2bf5:
    iRaw = (iRaw + 0x1);
    rgbRaw2[iRaw] = LOBYTE((LOWORD(lTank) & 0xff));
    cBits = (cBits - 0x8);
    lTank = (int32_t)((lTank >> 0x8));
    i = (i + 0x1);

L_2c2b:
    if ((i < 0x15))
        goto L_2bec;
    else
        goto L_2c34;

L_2c34:
    i = 0x0;
    goto L_2c64;

L_2c3c:
    /* untranslated: rgbRaw[byte 0x5:[i+0x2870]] = lobyte(setlobyte(i, rgbRaw2[i])) */
    i = (i + 0x1);

L_2c64:
    if ((i < 0x15))
        goto L_2c3c;
    else
        goto L_2c6d;

L_2c6d:
    LOWORD(lSerial) = LOWORD(rgbRaw);
    HIWORD(lSerial) = HIWORD(rgbRaw);
    if ((FValidSerialLong(lSerial) != 0x0))
        goto L_2c97;
    else
        goto L_2c91;

L_2c91:
    return 0x0;

L_2c97:
    fSuccess = 0x1;
    PushRandom(0x11000b);
    Randomize(lSerial);
    iRaw = 0xf;
    i = 0x0;
    goto L_2d7f;

L_2cc9:
    j = rgbRaw[(i + 0x4)];
    goto L_2cef;

L_2cdf:
    Random(0x10);
    j = (j - 0x1);

L_2cef:
    if ((j > 0x0))
        goto L_2cdf;
    else
        goto L_2cf8;

L_2cf8:
    if ((iPass != 0x0))
        goto L_2d35;
    else
        goto L_2d01;

L_2d01:
    /* untranslated: ss:[bp-0x48] = (rgbRaw[iRaw] & 0xf) */
    /* untranslated: branch ss:[bp-0x48] == (Random(0x10) & 0xff) ? L_2d6f : L_2d2d */

L_2d2d:
    fSuccess = 0x0;

L_2d35:
    /* untranslated: ss:[bp-0x48] = (rgbRaw[iRaw] >> 0x4) */
    /* untranslated: branch ss:[bp-0x48] == (Random(0x10) & 0xff) ? L_2d6b : L_2d66 */

L_2d66:
    fSuccess = 0x0;

L_2d6b:
    iRaw = (iRaw + 0x1);

L_2d6f:
    iPass = ((iPass + 0x1) & 0x1);
    i = (i + 0x1);

L_2d7f:
    if ((i < 0xb))
        goto L_2cc9;
    else
        goto L_2d88;

L_2d88:
    bXor = 0x0;
    i = 0x0;
    goto L_2da8;

L_2d94:
    bXor = (bXor ^ LOBYTE(rgbRaw[i]));
    i = (i + 0x1);

L_2da8:
    if ((i < 0xf))
        goto L_2d94;
    else
        goto L_2db1;

L_2db1:
    if (((rgbRaw[iRaw] >> 0x4) == (bXor & 0xf)))
        goto L_2de1;
    else
        goto L_2ddc;

L_2ddc:
    fSuccess = 0x0;

L_2de1:
    PopRandom();
    if ((fSuccess == 0x0))
        goto L_2e10;
    else
        goto L_2def;

L_2def:
    plSerial = LOWORD(lSerial);
    *(plSerial + 0x2) = HIWORD(lSerial);
    /* untranslated: call memcpy(pbEnv, &part[4:2](rgbRaw), 0xb) -> callresult(void *) */

L_2e10:
    return fSuccess;
}

int16_t FFindSomethingAndSelectIt() {
    PLANET *lpplMac;
    PLANET *lppl;
    int16_t i;
    FLEET  *lpfl;

L_2e1c:
    lppl = LpplFromId(rgplr[idPlayer].idPlanetHome);
    if ((LOWORD(lppl) != 0x0))
        goto L_2e54;
    else
        goto L_2e4b;

L_2e4b:
    if ((HIWORD(lppl) == 0x0))
        goto L_2e63;
    else
        goto L_2e54;

L_2e54:
    if ((lppl->iPlayer == idPlayer))
        goto L_2ed2;
    else
        goto L_2e63;

L_2e63:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_2ea4;

L_2e8e:
    if ((lppl->iPlayer == idPlayer))
        goto L_2eb2;
    else
        goto L_2e9a;

L_2e9a:

L_2ea0:
    lppl = (lppl + 0x1);

L_2ea4:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2e8e;
    else
        goto L_2eb2;

L_2eb2:
    if ((LOWORD(lppl) != LOWORD(lpplMac)))
        goto L_2ed2;
    else
        goto L_2ec0;

L_2ec0:
    if ((HIWORD(lppl) != HIWORD(lpplMac)))
        goto L_2ed2;
    else
        goto L_2ec8;

L_2ec8:
    lppl = 0x0;

L_2ed2:
    if ((LOWORD(lppl) != 0x0))
        goto L_2ee4;
    else
        goto L_2edb;

L_2edb:
    if ((HIWORD(lppl) == 0x0))
        goto L_2efc;
    else
        goto L_2ee4;

L_2ee4:
    SelectAdjPlanet(0x0, lppl->id);
    return 0x1;

L_2efc:
    i = 0x0;
    goto L_2f08;

L_2f04:
    i = (i + 0x1);

L_2f08:
    if ((i >= cFleet))
        goto L_2f6d;
    else
        goto L_2f13;

L_2f13:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_2f43;
    else
        goto L_2f3b;

L_2f3b:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_2f6d;
    else
        goto L_2f43;

L_2f43:
    if ((lpfl->iPlayer != idPlayer))
        goto L_2f04;
    else
        goto L_2f52;

L_2f52:
    SelectAdjFleet(0x0, LOWORD(lpfl));
    return 0x1;

L_2f6d:
    return 0x0;
}

void CommandHandler(HWND hwnd, uint16_t wParam) {
    POINT pt;
    HMENU hmenu;
    int16_t (**lpProc)();
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
    POINT        ptLegendB;
    POINT        ptLegendA;
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
    uint16_t     t_merge_3041_0001;
    uint16_t     t_merge_3297_0001;
    uint16_t     t_merge_32d8_0001;
    uint16_t     t_merge_343d_0001;
    uint16_t     t_3a25;
    uint16_t     t_3a7f;
    uint16_t     t_merge_3b11_0001;
    uint16_t     t_3c4f;
    uint16_t     t_3ca9;
    uint16_t     t_merge_3e5a_0001;
    uint16_t     t_merge_4698_0001;
    uint16_t     t_merge_476d_0001;
    uint16_t     t_merge_47a9_0001;
    uint16_t     t_merge_4a14_0001;
    uint16_t     t_merge_50ab_0001;

L_2f7a:
    if ((wParam < 0x3a98))
        goto L_2fa3;
    else
        goto L_2f8d;

L_2f8d:
    if ((wParam >= 0x3afc))
        goto L_2fa3;
    else
        goto L_2f97;

L_2f97:
    iPopMenuSel = (wParam + 0xc568);
    goto L_536f;

L_2fa3:
    goto L_50d4;

L_2fa9:
    lpProc = MakeProcInstance(About, hInst);
    DialogBox(hInst, IDD_ABOUT, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_2fe8:
    SendMessage(hwnd, WM_CLOSE, 0x0, 0x0);
    goto L_536f;

L_3003:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_300a;

L_300a:

L_3010:
    iWindowLayout = (wParam + 0xff7e);
    InvalidateRect(hwndFrame, 0x0, 0x1);
    if ((iWindowLayout != 0x2))
        goto L_303e;
    else
        goto L_3038;

L_3038:
    t_merge_3041_0001 = 0x1;
    goto L_3041;

L_303e:
    t_merge_3041_0001 = 0x0;

L_3041:
    EnsureTileSize(t_merge_3041_0001);
    RefitFrameChildren();
    goto L_536f;

L_3052:
    if ((gd.fTutorial == 0x0))
        goto L_3074;
    else
        goto L_3065;

L_3065:
    ShowTutor(0x1);
    goto L_536f;

L_3074:
    StartTutor(0x0);

L_3083:
    if ((gd.fTutorial == 0x0))
        goto L_30a6;
    else
        goto L_3096;

L_3096:
    if ((FAskKillTutor() == 0x0))
        goto L_536f;
    else
        goto L_30a0;

L_30a0:

L_30a6:
    NewGameWizard(hwnd, 0x0);
    goto L_536f;

L_30b8:
    if ((gd.fTutorial == 0x0))
        goto L_30db;
    else
        goto L_30cb;

L_30cb:
    if ((FAskKillTutor() == 0x0))
        goto L_536f;
    else
        goto L_30d5;

L_30d5:

L_30db:
    if ((LOWORD(vrgszMRU) != 0x0))
        goto L_30ef;
    else
        goto L_30e5;

L_30e5:
    if ((HIWORD(vrgszMRU) == 0x0))
        goto L_536f;
    else
        goto L_30ef;

L_30ef:
    /* untranslated: branch sext8to16(byte HIWORD(vrgszMRU):[(LOWORD(vrgszMRU) + ((wParam + 0xef34) * 0x100))]) == 0x0 ? L_536f : L_3112 */

L_3112:
    iplrOld = idPlayer;
    fstrcpy(&(szT), ((uint8_t *)(vrgszMRU) + ((wParam + 0xef34) * 0x100)));
    psz = strrchr(szT, 0x2e);
    if ((psz == 0x0))
        goto L_320e;
    else
        goto L_315c;

L_315c:
    if ((access(szT, 0x0) == 0xffff))
        goto L_320e;
    else
        goto L_3175;

L_3175:
    ini.fStartupFile = 0x1;
    DestroyCurGame();
    strcpy(0x56a2, szT);
    if ((FOpenGame(hwnd, 0x0) <= 0x0))
        goto L_536f;
    else
        goto L_31ae;

L_31ae:
    InitializeMenu(0x0);
    CreateChildWindows();
    if ((uTimerId != 0x0))
        goto L_31e1;
    else
        goto L_31c9;

L_31c9:
    PostMessage(hwnd, WM_COMMAND, 0xfa1, 0x0);

L_31e1:
    if ((game.fTutorial == 0x0))
        goto L_536f;
    else
        goto L_31f5;

L_31f5:
    if ((idPlayer != 0x0))
        goto L_536f;
    else
        goto L_31ff;

L_31ff:
    StartTutor(0x0);

L_320e:
    strcpy(0x57a4, szT);
    AlertSz(PszFormatIds(idsCantOpenFile, 0x0), MB_ICONHAND);

L_3243:
    /* untranslated: vplr = part[0:192](vrgplrDef) */
    RaceCreationWizard(hwnd, 0x0, 0x0);
    goto L_536f;

L_3269:
    hmenu = GetASubMenu(hwnd, 0x1);
    if ((gd.fToolbar != 0x0))
        goto L_3294;
    else
        goto L_328e;

L_328e:
    t_merge_3297_0001 = 0x1;
    goto L_3297;

L_3294:
    t_merge_3297_0001 = 0x0;

L_3297:
    HIWORD(gd) = ((HIWORD(gd) & 0x7fff) | ((t_merge_3297_0001 & 0x1) << 0xf));
    if ((gd.fToolbar == 0x0))
        goto L_32d5;
    else
        goto L_32cf;

L_32cf:
    t_merge_32d8_0001 = 0x8;
    goto L_32d8;

L_32d5:
    t_merge_32d8_0001 = 0x0;

L_32d8:
    CheckMenuItem(hmenu, 0xb3, t_merge_32d8_0001);
    RefitFrameChildren();
    goto L_536f;

L_32e6:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_32ed;

L_32ed:

L_32f3:
    lpProc = MakeProcInstance(PrintMapDlg, hInst);
    fRet = DialogBox(hInst, 0xd6, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet == 0x0))
        goto L_536f;
    else
        goto L_3339;

L_3339:

L_333f:
    cPageX = LOWORD(vrgcPrintMapPage);
    cPageY = HIWORD(vrgcPrintMapPage);
    memset(pd, 0x0, 0x34);
    LOWORD(pd) = 0x34;
    HIWORD(pd) = 0x0;
    /* untranslated: part[12:2](pd) = 0x500 */
    /* untranslated: part[14:2](pd) = 0x0 */
    if ((PrintDlg(&(pd)) == 0x0))
        goto L_3e00;
    else
        goto L_3387;

L_3387:
    dxMax = GetDeviceCaps(pd.hDC, HORZRES);
    dyMax = GetDeviceCaps(pd.hDC, VERTRES);
    dxDPI = GetDeviceCaps(pd.hDC, LOGPIXELSX);
    dyDPI = GetDeviceCaps(pd.hDC, LOGPIXELSY);
    dMargin = 0x20;
    hfontPrint = HfontPrinterCreate(pd.hDC, 0x8, &(dyPrint));
    hfontPrintTiny = HfontPrinterCreate(pd.hDC, 0x5, &(dyPrintTiny));
    SetBkMode(pd.hDC, TRANSPARENT);
    rc.top = 0x0;
    rc.left = 0x0;
    if ((cPageX <= cPageY))
        goto L_3421;
    else
        goto L_341b;

L_341b:
    goto L_3424;

L_3421:

L_3424:
    if ((dxMax <= dyMax))
        goto L_343a;
    else
        goto L_3434;

L_3434:
    t_merge_343d_0001 = 0x1;
    goto L_343d;

L_343a:
    t_merge_343d_0001 = 0x0;

L_343d:
    /* untranslated: branch ss:[bp-0xa0] != t_merge_343d_0001 ? L_345a : L_3448 */

L_3448:
    i = cPageX;
    cPageX = cPageY;
    cPageY = i;

L_345a:
    ldx = (uint32_t)(((uint32_t)(dxMax) * (uint32_t)(cPageX)));
    ldy = (uint32_t)(((uint32_t)(dyMax) * (uint32_t)(cPageY)));
    if ((HIWORD(ldx) < 0x0))
        goto L_34ad;
    else
        goto L_3494;

L_3494:
    if ((HIWORD(ldx) > 0x0))
        goto L_34a3;
    else
        goto L_3499;

L_3499:
    if ((LOWORD(ldx) <= 0x7d00))
        goto L_34ad;
    else
        goto L_34a3;

L_34a3:
    ldx = 0x7d00;

L_34ad:
    if ((HIWORD(ldy) < 0x0))
        goto L_34d3;
    else
        goto L_34b7;

L_34b7:
    if ((HIWORD(ldy) > 0x0))
        goto L_34c7;
    else
        goto L_34bc;

L_34bc:
    if ((LOWORD(ldy) <= 0x7d00))
        goto L_34d3;
    else
        goto L_34c7;

L_34c7:
    ldy = 0x7d00;

L_34d3:
    if ((HIWORD(ldx) > HIWORD(ldy)))
        goto L_355e;
    else
        goto L_34e3;

L_34e3:
    if ((HIWORD(ldx) < HIWORD(ldy)))
        goto L_34f0;
    else
        goto L_34e8;

L_34e8:
    if ((LOWORD(ldx) >= LOWORD(ldy)))
        goto L_355e;
    else
        goto L_34f0;

L_34f0:
    if (((HIWORD(ldy) - SIGNHIWORD(dyDPI)) > HIWORD(ldx)))
        goto L_352d;
    else
        goto L_3509;

L_3509:
    if (((HIWORD(ldy) - SIGNHIWORD(dyDPI)) < HIWORD(ldx)))
        goto L_3516;
    else
        goto L_350e;

L_350e:
    if (((LOWORD(ldy) - dyDPI) >= LOWORD(ldx)))
        goto L_352d;
    else
        goto L_3516;

L_3516:
    ldx = (ldy - (uint32_t)(dyDPI));

L_352d:
    dSize = LOWORD(ldx);
    ptLegendA.x = dMargin;
    ptLegendA.y = (dSize + dMargin);
    ptLegendB.x = ((uint32_t)(dSize) / 0x2);
    ptLegendB.y = ptLegendA.y;
    goto L_35dc;

L_355e:
    /* untranslated: branch (HIWORD(ldx) - signhiword((words(loword((0x3 * dxDPI)), signhiword(loword((0x3 * dxDPI)))) / 0x2))) > HIWORD(ldy) ? L_35ab : L_357e
     */

L_357e:
    /* untranslated: branch (HIWORD(ldx) - signhiword((words(loword((0x3 * dxDPI)), signhiword(loword((0x3 * dxDPI)))) / 0x2))) < HIWORD(ldy) ? L_358c : L_3583
     */

L_3583:
    /* untranslated: branch (LOWORD(ldx) - (words(loword((0x3 * dxDPI)), signhiword(loword((0x3 * dxDPI)))) / 0x2)) >= LOWORD(ldy) ? L_35ab : L_358c */

L_358c:
    /* untranslated: ldy = (ldx - sext16to32((words(loword((0x3 * dxDPI)), signhiword(loword((0x3 * dxDPI)))) / 0x2))) */

L_35ab:
    dSize = LOWORD(ldy);
    ptLegendA.x = (dSize + dMargin);
    ptLegendA.y = dMargin;
    ptLegendB.x = ptLegendA.x;
    ptLegendB.y = ((uint32_t)(dSize) / 0x2);

L_35dc:
    rc.bottom = dSize;
    rc.right = dSize;
    dSize = (dSize - (dMargin * 0x2));
    xPage = 0x0;
    goto L_3d86;

L_35f9:
    yPage = 0x0;
    goto L_3d76;

L_3602:
    /* untranslated: xOff = loword(((dxMax neg 0x0) * xPage)) */
    /* untranslated: yOff = loword(((dyMax neg 0x0) * yPage)) */
    cch = CchGetString(idsStarsUniverseMap, 0x57a4);
    Escape(pd.hDC, 0xa, cch, szWork, 0x0);
    Rectangle(pd.hDC, xOff, yOff, (xOff + rc.right), (yOff + rc.bottom));
    if ((hfontPrint == 0x0))
        goto L_368c;
    else
        goto L_367a;

L_367a:
    hfontSav = SelectObject(pd.hDC, hfontPrint);
    goto L_3692;

L_368c:
    hfontSav = 0x0;

L_3692:
    y = (ptLegendA.y + yOff);
    cch = CchGetString(idsStarsUniverseMap, 0x57a4);
    TextOut(pd.hDC, (ptLegendA.x + xOff), y, szWork, cch);
    y = (y + dyPrint);
    TextOut(pd.hDC, (ptLegendA.x + xOff), y, "", strlen(0x90));
    y = (y + dyPrint);
    psz = PszPlayerName(idPlayer, 0x1, 0x1, 0x1, 0x0, 0x0);
    TextOut(pd.hDC, (ptLegendA.x + xOff), y, psz, strlen(psz));
    y = (y + dyPrint);
    cch = _wsprintf(szWork, PszGetCompressedString(idsYearD), (game.turn + 0x960));
    TextOut(pd.hDC, (ptLegendA.x + xOff), y, szWork, cch);
    if (((grbitScan & 0xf) == 0x5))
        goto L_396f;
    else
        goto L_37b0;

L_37b0:
    y = (ptLegendB.y + yOff);
    i = 0x0;
    goto L_3802;

L_37c2:
    cch = CchGetString((i + 0x522), 0x57a4);
    TextOut(pd.hDC, (ptLegendB.x + xOff), y, szWork, cch);
    y = (y + dyPrint);
    i = (i + 0x1);

L_3802:
    if ((i < 0x5))
        goto L_37c2;
    else
        goto L_380b;

L_380b:
    y = (ptLegendB.y + yOff);
    if ((hfontPrintTiny == 0x0))
        goto L_3829;
    else
        goto L_381e;

L_381e:
    SelectObject(pd.hDC, hfontPrintTiny);

L_3829:
    /* untranslated: call CtrTextOut(pd.hDC, (ptLegendB.x + xOff), (((words(loword((0x3 * dyPrint)), signhiword(loword((0x3 * dyPrint)))) / 0x2) + y) -
     * (sext16to32(dyPrintTiny) / 0x2)), 0x3d4, 0x1) -> callresult(void) */
    /* untranslated: call CtrTextOut(pd.hDC, (ptLegendB.x + xOff), (((words(loword((0x5 * dyPrint)), signhiword(loword((0x5 * dyPrint)))) / 0x2) + y) -
     * (sext16to32(dyPrintTiny) / 0x2)), 0x3d6, 0x1) -> callresult(void) */
    /* untranslated: call CtrTextOut(pd.hDC, (ptLegendB.x + xOff), ((((words(loword((0x9 * dyPrint)), signhiword(loword((0x9 * dyPrint)))) / 0x2) + y) + 0x8) -
     * dyPrintTiny), 0x3d8, 0x1) -> callresult(void) */
    if ((hfontPrint == 0x0))
        goto L_38ed;
    else
        goto L_38e2;

L_38e2:
    SelectObject(pd.hDC, hfontPrint);

L_38ed:
    DrawPlanetPrintDot(pd.hDC, (ptLegendB.x + xOff), ((y + 0xfffc) + ((uint32_t)(dyPrint) / 0x2)), 0x1);
    /* untranslated: call DrawPlanetPrintDot(pd.hDC, (ptLegendB.x + xOff), ((y + 0xfffc) + (words(loword((0x7 * dyPrint)), signhiword(loword((0x7 * dyPrint))))
     * / 0x2)), 0x0) -> callresult(void) */
    /* untranslated: call DrawPlanetPrintDot(pd.hDC, (ptLegendB.x + xOff), (((words(loword((0x9 * dyPrint)), signhiword(loword((0x9 * dyPrint)))) / 0x2) + y) +
     * 0x8), 0x0) -> callresult(void) */

L_396f:
    if (((grbitScan & 0xf) == 0x5))
        goto L_3bc5;
    else
        goto L_397d;

L_397d:
    if ((hfontPrintTiny == 0x0))
        goto L_3991;
    else
        goto L_3986;

L_3986:
    SelectObject(pd.hDC, hfontPrintTiny);

L_3991:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_3bb7;

L_39c0:
    x = (uint32_t)((rgptPlan[lppl->id].x + 0xfc18));
    y = (uint32_t)(((dGalInv + 0xfc18) - rgptPlan[lppl->id].y));
    /* untranslated: ss:[bp-0xae] = xOff */
    /* untranslated: ss:[bp-0xac] = signhiword(xOff) */
    /* untranslated: ss:[bp-0xb2] = dMargin */
    /* untranslated: ss:[bp-0xb0] = signhiword(dMargin) */
    t_3a25 = LOWORD(x);
    /* untranslated: LOWORD(x) = ((loword((int32_t)((uint32_t)(x * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb2]) + ss:[bp-0xae]) */
    /* untranslated: HIWORD(x) = ((hiword((int32_t)((uint32_t)(words(HIWORD(x), t_3a25) * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb0]) +
     * ss:[bp-0xac]) */
    /* untranslated: ss:[bp-0xae] = yOff */
    /* untranslated: ss:[bp-0xac] = signhiword(yOff) */
    /* untranslated: ss:[bp-0xb2] = dMargin */
    /* untranslated: ss:[bp-0xb0] = signhiword(dMargin) */
    t_3a7f = LOWORD(y);
    /* untranslated: LOWORD(y) = ((loword((int32_t)((uint32_t)(y * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb2]) + ss:[bp-0xae]) */
    /* untranslated: HIWORD(y) = ((hiword((int32_t)((uint32_t)(words(HIWORD(y), t_3a7f) * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb0]) +
     * ss:[bp-0xac]) */
    if ((lppl->iPlayer != idPlayer))
        goto L_3b59;
    else
        goto L_3abe;

L_3abe:
    if ((lppl->fStarbase == 0x0))
        goto L_3b35;
    else
        goto L_3ad5;

L_3ad5:
    if ((rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef != ihuldefOrbitalFort))
        goto L_3b0e;
    else
        goto L_3b08;

L_3b08:
    t_merge_3b11_0001 = 0x3da;
    goto L_3b11;

L_3b0e:
    t_merge_3b11_0001 = 0x3dc;

L_3b11:
    CtrTextOut(pd.hDC, LOWORD(x), ((LOWORD(y) + 0x4) - dyPrintTiny), t_merge_3b11_0001, 0x1);

L_3b35:
    DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 0x1);
    goto L_3bb3;

L_3b59:
    if ((lppl->iPlayer == 0xffff))
        goto L_3bb3;
    else
        goto L_3b66;

L_3b66:
    cch = _wsprintf(szWork, PCTD, (lppl->iPlayer + 0x1));
    CtrTextOut(pd.hDC, LOWORD(x), (LOWORD(y) - dyPrintTiny), 0x57a4, cch);

L_3bb3:
    lppl = (lppl + 0x1);

L_3bb7:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_39c0;
    else
        goto L_3bc5;

L_3bc5:
    if ((hfontPrint == 0x0))
        goto L_3bd9;
    else
        goto L_3bce;

L_3bce:
    SelectObject(pd.hDC, hfontPrint);

L_3bd9:
    i = 0x0;
    goto L_3be5;

L_3be1:
    i = (i + 0x1);

L_3be5:
    if ((i >= game.cPlanMax))
        goto L_3d3b;
    else
        goto L_3bf0;

L_3bf0:
    x = (uint32_t)((rgptPlan[i].x + 0xfc18));
    y = (uint32_t)(((dGalInv + 0xfc18) - rgptPlan[i].y));
    /* untranslated: ss:[bp-0xae] = xOff */
    /* untranslated: ss:[bp-0xac] = signhiword(xOff) */
    /* untranslated: ss:[bp-0xb2] = dMargin */
    /* untranslated: ss:[bp-0xb0] = signhiword(dMargin) */
    t_3c4f = LOWORD(x);
    /* untranslated: LOWORD(x) = ((loword((int32_t)((uint32_t)(x * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb2]) + ss:[bp-0xae]) */
    /* untranslated: HIWORD(x) = ((hiword((int32_t)((uint32_t)(words(HIWORD(x), t_3c4f) * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb0]) +
     * ss:[bp-0xac]) */
    /* untranslated: ss:[bp-0xae] = yOff */
    /* untranslated: ss:[bp-0xac] = signhiword(yOff) */
    /* untranslated: ss:[bp-0xb2] = dMargin */
    /* untranslated: ss:[bp-0xb0] = signhiword(dMargin) */
    t_3ca9 = LOWORD(y);
    /* untranslated: LOWORD(y) = ((loword((int32_t)((uint32_t)(y * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb2]) + ss:[bp-0xae]) */
    /* untranslated: HIWORD(y) = ((hiword((int32_t)((uint32_t)(words(HIWORD(y), t_3ca9) * sext16to32(dSize)) / sext16to32(dGal))) + ss:[bp-0xb0]) +
     * ss:[bp-0xac]) */
    DrawPlanetPrintDot(pd.hDC, LOWORD(x), LOWORD(y), 0x0);
    if (((grbitScan & 0x400) == 0x0))
        goto L_3be1;
    else
        goto L_3d08;

L_3d08:
    CtrTextOut(pd.hDC, LOWORD(x), (LOWORD(y) + 0xe), PszGetPlanetName(i), 0x0);

L_3d3b:
    if ((hfontSav == 0x0))
        goto L_3d51;
    else
        goto L_3d45;

L_3d45:
    SelectObject(pd.hDC, hfontSav);

L_3d51:
    Escape(pd.hDC, 0x1, 0x0, 0x0, 0x0);
    yPage = (yPage + 0x1);

L_3d76:
    if ((yPage < cPageY))
        goto L_3602;
    else
        goto L_3d82;

L_3d82:
    xPage = (xPage + 0x1);

L_3d86:
    if ((xPage < cPageX))
        goto L_35f9;
    else
        goto L_3d91;

L_3d91:
    Escape(pd.hDC, 0xb, 0x0, 0x0, 0x0);
    if ((hfontPrint == 0x0))
        goto L_3dc2;
    else
        goto L_3dba;

L_3dba:
    DeleteObject(hfontPrint);

L_3dc2:
    if ((hfontPrintTiny == 0x0))
        goto L_3dd3;
    else
        goto L_3dcb;

L_3dcb:
    DeleteObject(hfontPrintTiny);

L_3dd3:
    DeleteDC(pd.hDC);
    if ((pd.hDevMode == 0x0))
        goto L_3dec;
    else
        goto L_3de4;

L_3de4:
    GlobalFree(pd.hDevMode);

L_3dec:
    if ((pd.hDevNames == 0x0))
        goto L_536f;
    else
        goto L_3df5;

L_3df5:
    GlobalFree(pd.hDevNames);

L_3e00:
    AlertSz(PszFormatIds(idsUnablePrintGameMapPrinterMayOff, 0x0), MB_ICONHAND);

L_3e24:
    hmenu = GetASubMenu(hwnd, 0x1);
    grbitScan = (grbitScan ^ 0x2000);
    if (((grbitScan & 0x2000) == 0x0))
        goto L_3e57;
    else
        goto L_3e51;

L_3e51:
    t_merge_3e5a_0001 = 0x8;
    goto L_3e5a;

L_3e57:
    t_merge_3e5a_0001 = 0x0;

L_3e5a:
    CheckMenuItem(hmenu, 0x98d, t_merge_3e5a_0001);
    gd.fChgScanner = 0x1;
    if (((grbitScan & 0x1400) == 0x0))
        goto L_536f;
    else
        goto L_3e7a;

L_3e7a:
    InvalidateRect(hwndScanner, 0x0, 0x0);

L_3e92:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_3e99;

L_3e99:

L_3e9f:
    lpProc = MakeProcInstance(FindDlg, hInst);
    fRet = DialogBox(hInst, IDD_FIND, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_3ee1:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_3ee8;

L_3ee8:

L_3eee:
    lpProc = MakeProcInstance(ScoreXDlg, hInst);
    fRet = DialogBox(hInst, IDD_SCORE, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_3f30:
    if ((game.fSinglePlr != 0x0))
        goto L_3f6e;
    else
        goto L_3f42;

L_3f42:
    if ((AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, 0x0), 0x2024) != 0x6))
        goto L_536f;
    else
        goto L_3f68;

L_3f68:

L_3f6e:
    idCur = idPlayer;
    if ((wParam != 0x5209))
        goto L_3f87;
    else
        goto L_3f7e;

L_3f7e:
    iPassCnt = 0x64;
    goto L_3fb3;

L_3f87:
    if ((wParam != 0x520a))
        goto L_3f9a;
    else
        goto L_3f91;

L_3f91:
    iPassCnt = 0x3e8;
    goto L_3fb3;

L_3f9a:
    if ((wParam != 0x5208))
        goto L_3fad;
    else
        goto L_3fa4;

L_3fa4:
    iPassCnt = 0xa;
    goto L_3fb3;

L_3fad:
    iPassCnt = 0x0;

L_3fb3:
    if ((game.fSinglePlr == 0x0))
        goto L_401a;
    else
        goto L_3fc5;

L_3fc5:
    if ((iPassCnt == 0x0))
        goto L_401a;
    else
        goto L_3fcf;

L_3fcf:
    _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), iPassCnt);
    if ((MessageBox(GetFocus(), szWork, "Stars!", 0x2034) != 0x6))
        goto L_536f;
    else
        goto L_4014;

L_4014:

L_401a:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4021;

L_4021:

L_4027:
    if ((gd.fTutorial == 0x0))
        goto L_41f8;
    else
        goto L_403a;

L_403a:
    AdvanceTutor();
    if ((tutor.fTurnDone != 0x0))
        goto L_407a;
    else
        goto L_4053;

L_4053:
    AlertSz(PszFormatIds(idsTutorialTurnWillGeneratedHaveYetCompleted, 0x0), MB_ICONHAND);
    goto L_536f;

L_407a:
    ShowTutor(0x0);
    if ((game.turn > 0x23))
        goto L_41f8;
    else
        goto L_4090;

L_4090:
    Randomize(0x499602d2);
    FWriteHistFile(idCur);
    if ((FWriteTutorialMFile((game.turn + 0x1)) != 0x0))
        goto L_40e6;
    else
        goto L_40c2;

L_40c2:
    AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
    goto L_536f;

L_40e6:
    if ((game.turn != 0x23))
        goto L_412b;
    else
        goto L_40f0;

L_40f0:
    if ((FWriteTutorialMFile((game.turn + 0x2)) != 0x0))
        goto L_412b;
    else
        goto L_4107;

L_4107:
    AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
    goto L_536f;

L_412b:
    strcpy(0x57a4, 0x56a2);
    strcat(0x57a4, 0x3e5);
    remove(0x57a4);
    DirtyGame(0x0);
    ShowProgressGauge();
    LOWORD(ti) = 0xc;
    HIWORD(ti) = 0x0;
    TimerCount(ti);
    /* untranslated: LOWORD(dwTickBase) = part[4:2](ti) */
    /* untranslated: HIWORD(dwTickBase) = part[6:2](ti) */

L_418a:
    UpdateProgressGauge(((LOWORD(dwTickCur) - LOWORD(dwTickBase)) * 0x2));
    TimerCount(ti);
    /* untranslated: LOWORD(dwTickCur) = part[4:2](ti) */
    /* untranslated: HIWORD(dwTickCur) = part[6:2](ti) */
    if ((HIWORD(dwTickCur) < HIWORD(dwTickBase)))
        goto LTutorialFinishUp;
    else
        goto L_41c7;

L_41c7:
    if ((HIWORD(dwTickCur) > HIWORD(dwTickBase)))
        goto L_41d4;
    else
        goto L_41cc;

L_41cc:
    if ((LOWORD(dwTickCur) < LOWORD(dwTickBase)))
        goto LTutorialFinishUp;
    else
        goto L_41d4;

L_41d4:
    if ((HIWORD(dwTickCur) < (HIWORD(dwTickBase) + 0x0)))
        goto L_418a;
    else
        goto L_41e8;

L_41e8:
    if ((HIWORD(dwTickCur) > (HIWORD(dwTickBase) + 0x0)))
        goto LTutorialFinishUp;
    else
        goto L_41ed;

L_41ed:
    /* untranslated: branch LOWORD(dwTickCur) >= part[8:0](szMine[LOWORD(dwTickBase)]) ? L_433d : L_41f2 */

L_41f2:

L_41f8:
    if ((game.fSinglePlr == 0x0))
        goto LWaitForTurn;
    else
        goto L_4207;

L_4207:

L_420d:
    idsFileError = 0xffff;
    if ((FCheckFile(dtHost, 0xffff, 0x1) == 0x0))
        goto L_429c;
    else
        goto L_422f;

L_422f:
    if ((FBadFileError(idsFileError) == 0x0))
        goto L_426a;
    else
        goto L_4243;

L_4243:
    AlertSz(PszFormatIds(idsFileError, 0x0), MB_ICONHAND);
    goto L_536f;

L_426a:
    if ((AlertSz(PszFormatIds(idsGameAlreadyHostedAnotherInstanceStarsWould, 0x0), 0x2024) == 0x6))
        goto LWaitForTurn;
    else
        goto L_4290;

L_4290:

L_429c:
    hcurSav = SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f02)));
    FWriteLogFile(0x56a2, idCur);
    FWriteHistFile(idCur);

RepGen:
    ShowProgressGauge();
    EnsureAis();
    FGenerateTurn();
    if ((wParam == 0x5208))
        goto L_42fd;
    else
        goto L_42e9;

L_42e9:
    if ((wParam == 0x5209))
        goto L_42fd;
    else
        goto L_42f3;

L_42f3:
    if ((wParam != 0x520a))
        goto L_4337;
    else
        goto L_42fd;

L_42fd:
    iPassCnt = (iPassCnt - 0x1);
    if ((iPassCnt <= 0x0))
        goto L_4337;
    else
        goto L_430d;

L_430d:
    HideProgressGauge();
    if ((GetAsyncKeyState(0x10) >= 0x0))
        goto RepGen;
    else
        goto L_4323;

L_4323:
    if ((GetAsyncKeyState(0x11) >= 0x0))
        goto RepGen;
    else
        goto L_4331;

L_4331:

L_4337:
    iPassCnt = 0x0;

LTutorialFinishUp:
    DestroyCurGame();
    _wsprintf(&(szExt), MPCTD, (idCur + 0x1));
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_43a8;
    else
        goto L_4377;

L_4377:
    SetCursor(hcurSav);
    HideProgressGauge();
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    goto L_536f;

L_43a8:
    HideProgressGauge();
    idPlayer = idCur;
    CreateChildWindows();
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    SetCursor(hcurSav);
    if ((gd.fTutorial == 0x0))
        goto L_536f;
    else
        goto L_43ec;

L_43ec:
    tutor.fTurnDone = 0x0;
    tutor.fAutoComplete = 0x0;
    AdvanceTutor();

L_440c:
    if ((LOWORD(game) != 0x0))
        goto L_4420;
    else
        goto L_4416;

L_4416:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4420;

L_4420:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4427;

L_4427:

L_442d:
    if ((hwndBrowser != 0x0))
        goto L_445c;
    else
        goto L_4437;

L_4437:
    mf = 0x8;
    CreateDialog(hInst, IDD_BROWSER, hwndFrame, &(lpfnBrowserDlgProc));
    goto L_446a;

L_445c:
    mf = 0x0;
    DestroyWindow(hwndBrowser);

L_446a:
    hmenu = GetASubMenu(hwnd, 0x5);
    CheckMenuItem(hmenu, 0x100, mf);
    goto L_536f;

L_448f:
    if ((hwndReportDlg != 0x0))
        goto L_44a1;
    else
        goto L_4499;

L_4499:
    wParam = 0x8fd;
    goto L_44cc;

L_44a1:
    if ((vprptCur != 0x1494))
        goto L_44b4;
    else
        goto L_44ac;

L_44ac:
    wParam = 0x8ff;
    goto L_44cc;

L_44b4:
    if ((vprptCur != 0x14ca))
        goto L_44c7;
    else
        goto L_44bf;

L_44bf:
    wParam = 0x900;
    goto L_44cc;

L_44c7:
    wParam = 0x901;

L_44cc:
    cObj = 0x0;
    if ((LOWORD(game) != 0x0))
        goto L_44e5;
    else
        goto L_44db;

L_44db:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_44e5;

L_44e5:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_44ec;

L_44ec:

L_44f2:
    hmenu = GetASubMenu(hwnd, 0x4);

LRetryReport:
    if ((hwndReportDlg != 0x0))
        goto L_4727;
    else
        goto L_450e;

L_450e:
    mf = 0x8;
    if ((wParam != 0x8ff))
        goto L_4588;
    else
        goto L_451d;

L_451d:
    ids = idsFleetSummaryReportDFleetC;
    vprptCur = 0x14ca;
    ifl = 0x0;
    goto L_4534;

L_4530:
    ifl = (ifl + 0x1);

L_4534:
    if ((ifl >= cFleet))
        goto L_4678;
    else
        goto L_453f;

L_453f:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_456f;
    else
        goto L_4567;

L_4567:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4678;
    else
        goto L_456f;

L_456f:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4530;
    else
        goto L_457e;

L_457e:
    cObj = (cObj + 0x1);

L_4588:
    if ((wParam != 0x900))
        goto L_45fd;
    else
        goto L_4592;

L_4592:
    ids = idsOthersFleetsSummaryReportDFleetC;
    vprptCur = 0x1500;
    ifl = 0x0;
    goto L_45a9;

L_45a5:
    ifl = (ifl + 0x1);

L_45a9:
    if ((ifl >= cFleet))
        goto L_4678;
    else
        goto L_45b4;

L_45b4:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_45e4;
    else
        goto L_45dc;

L_45dc:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4678;
    else
        goto L_45e4;

L_45e4:
    if ((lpfl->iPlayer == idPlayer))
        goto L_45a5;
    else
        goto L_45f3;

L_45f3:
    cObj = (cObj + 0x1);

L_45fd:
    if ((wParam != 0x901))
        goto L_461d;
    else
        goto L_4607;

L_4607:
    vprptCur = 0x1536;
    ids = idsBattleSummaryReportDBattleC;
    cObj = CBattles();
    goto L_4678;

L_461d:
    vprptCur = 0x1494;
    ids = idsPlanetSummaryReportDPlanetC;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_466a;

L_4653:
    if ((lppl->iPlayer != idPlayer))
        goto L_4666;
    else
        goto L_4662;

L_4662:
    cObj = (cObj + 0x1);

L_4666:
    lppl = (lppl + 0x1);

L_466a:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_4653;
    else
        goto L_4678;

L_4678:
    psz = PszGetCompressedString(ids);
    if ((cObj != 0x1))
        goto L_4695;
    else
        goto L_468f;

L_468f:
    t_merge_4698_0001 = 0x20;
    goto L_4698;

L_4695:
    t_merge_4698_0001 = 0x73;

L_4698:
    _wsprintf(szWork, psz, cObj, t_merge_4698_0001);
    hwndReportDlg = CreateWindow(szReport, szWork, 0x80cf0000, 0x0, 0x0, 0x64, 0x64, hwndFrame, 0x0, hInst, 0x0);
    SetWindowPos(hwndReportDlg, 0x0, 0x0, 0x0, 0x0, 0x0, 0x47);
    CheckMenuItem(hmenu, wParam, mf);
    goto L_536f;

L_4727:
    if ((wParam != 0x901))
        goto L_473c;
    else
        goto L_4731;

L_4731:
    if ((vprptCur == 0x1536))
        goto L_4744;
    else
        goto L_473c;

L_473c:
    ids = idsPlayerLogFileAppearsCorruptUnableLoad;
    goto L_4749;

L_4744:
    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;

L_4749:
    mf = 0x0;
    DestroyWindow(hwndReportDlg);
    if ((wParam != 0x8ff))
        goto L_476a;
    else
        goto L_4764;

L_4764:
    t_merge_476d_0001 = 0x8ff;
    goto L_476d;

L_476a:
    t_merge_476d_0001 = 0x8fd;

L_476d:
    CheckMenuItem(hmenu, t_merge_476d_0001, mf);
    if ((ids == idsPlayerLogFileAppearsCorruptUnableLoad))
        goto LRetryReport;
    else
        goto L_477d;

L_477d:

L_4786:
    if ((wParam != 0x9c2))
        goto L_47a5;
    else
        goto L_479e;

L_479e:
    t_merge_47a9_0001 = 0x1195;
    goto L_47a9;

L_47a5:
    t_merge_47a9_0001 = 0x32ca;

L_47a9:
    /* untranslated: call WinHelp(hwnd, szHelpFile, 0x1, words(0x0, t_merge_47a9_0001)) -> callresult(int16_t) */
    goto L_536f;

L_47b3:
    WinHelp(hwnd, szHelpFile, 0x3, 0x0);
    goto L_536f;

LWaitForTurn:
    if ((LOWORD(lpPlanets) != 0x0))
        goto L_47e5;
    else
        goto L_47db;

L_47db:
    if ((HIWORD(lpPlanets) == 0x0))
        goto L_536f;
    else
        goto L_47e5;

L_47e5:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_47ef;

L_47ef:
    if ((game.fSinglePlr != 0x0))
        goto L_536f;
    else
        goto L_47fe;

L_47fe:

L_4804:
    if ((FNewTurnAvail(idPlayer) == 0x0))
        goto L_4907;
    else
        goto LNewTurnAvail;

LNewTurnAvail:
    FWriteHistFile(idPlayer);
    if ((game.fDirty == 0x0))
        goto L_4861;
    else
        goto L_482e;

L_482e:
    id = AlertSz(PszFormatIds(idsSorryTurnHasAlreadyGeneratedAnyChanges, 0x0), 0x31);
    if ((id == 0x2))
        goto L_536f;
    else
        goto L_4858;

L_4858:

L_4861:
    AlertSz(PszFormatIds(idsNewTurnAvailable, 0x0), MB_ICONASTERISK);

L_4882:
    _wsprintf(&(szExt), MPCTD, (idPlayer + 0x1));
    game.fDirty = 0x0;
    DestroyCurGame();
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_48e6;
    else
        goto L_48c2;

L_48c2:
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    goto L_536f;

L_48e6:
    CreateChildWindows();
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    goto L_536f;

L_4907:
    gd.fSubmit = 0x1;
    FWriteLogFile(0x56a2, idPlayer);
    FWriteHistFile(idPlayer);
    SetWindowText(hwndFrame, PszGetCompressedString(idsWaitingNewTurn));
    ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
    uTimerId = SetTimer(0x0, 0xe, 0x2710, lpfnHostTimerProc);
    uTimerType = 0xe;
    HostTimerProc(0x0, WM_NULL, uTimerId, 0x0);
    goto L_536f;

L_4993:
    wParam = 0xedb;

L_4998:
    if ((hwndScanner != 0x0))
        goto L_49c6;
    else
        goto L_49a2;

L_49a2:
    AlertSz(PszFormatIds(idsGameCurrentlyLoaded, 0x0), MB_ICONHAND);
    goto L_536f;

L_49c6:
    if ((idPlayer != 0xffff))
        goto L_49e7;
    else
        goto L_49d0;

L_49d0:
    FWriteDataFile(0x56a2, idPlayer, 0x0);
    goto L_536f;

L_49e7:
    if ((FNewTurnAvail(idPlayer) != 0x0))
        goto LNewTurnAvail;
    else
        goto L_49f8;

L_49f8:

L_4a01:
    if ((wParam != 0xedb))
        goto L_4a11;
    else
        goto L_4a0b;

L_4a0b:
    t_merge_4a14_0001 = 0x1;
    goto L_4a14;

L_4a11:
    t_merge_4a14_0001 = 0x0;

L_4a14:
    LOWORD(gd) = ((LOWORD(gd) & 0xffef) | ((t_merge_4a14_0001 & 0x1) << 0x4));
    FWriteLogFile(0x56a2, idPlayer);
    FWriteHistFile(idPlayer);

L_4a53:
    if ((gd.fTutorial == 0x0))
        goto L_4a76;
    else
        goto L_4a66;

L_4a66:
    if ((FAskKillTutor() == 0x0))
        goto L_536f;
    else
        goto L_4a70;

L_4a70:

L_4a76:
    if ((FOpenGame(hwnd, 0x0) <= 0x0))
        goto L_536f;
    else
        goto L_4a8d;

L_4a8d:
    InitializeMenu(0x0);
    if ((uTimerId != 0x0))
        goto L_4abb;
    else
        goto L_4aa3;

L_4aa3:
    PostMessage(hwnd, WM_COMMAND, 0xfa1, 0x0);

L_4abb:
    if ((game.fTutorial == 0x0))
        goto L_536f;
    else
        goto L_4acf;

L_4acf:
    if ((idPlayer != 0x0))
        goto L_536f;
    else
        goto L_4ad9;

L_4ad9:
    StartTutor(0x0);

L_4ae8:
    if ((hwndTitle == 0x0))
        goto L_536f;
    else
        goto L_4aef;

L_4aef:

L_4af5:
    PostMessage(hwndTitle, WM_COMMAND, (wParam + 0xff06), 0x0);
    goto L_536f;

L_4b14:
    if ((gd.fTutorial == 0x0))
        goto L_4b37;
    else
        goto L_4b27;

L_4b27:
    if ((FAskKillTutor() == 0x0))
        goto L_536f;
    else
        goto L_4b31;

L_4b31:

L_4b37:
    WriteIniSettings();
    DestroyCurGame();
    strcpy(0x56a2, 0x57a4);
    ini.grobjSel = 0x0;
    ini.iObjSel = 0x0;
    ini.idPlayer = 0xffff;
    InitializeMenu(0x0);
    pt.x = GetSystemMetrics(SM_CXSCREEN);
    pt.y = GetSystemMetrics(SM_CYSCREEN);
    hwndTitle = CreateWindow(szTitle, "Stars!", 0x90000000, 0x0, 0x0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    fFreeingTitle = 0x0;
    ShowWindow(hwndFrame, SW_HIDE);
    goto L_536f;

L_4be3:
    if ((LOWORD(game) != 0x0))
        goto L_4bf7;
    else
        goto L_4bed;

L_4bed:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4bf7;

L_4bf7:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4bfe;

L_4bfe:

L_4c04:
    lpProc = MakeProcInstance(ResearchDlg, hInst);
    fRet = DialogBox(hInst, IDD_RESEARCH, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet == 0x0))
        goto L_536f;
    else
        goto L_4c4d;

L_4c4d:
    if ((sel.grobj != grobjPlanet))
        goto L_536f;
    else
        goto L_4c57;

L_4c57:
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_4c6b;
    else
        goto L_4c61;

L_4c61:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_4c87;
    else
        goto L_4c6b;

L_4c6b:
    FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);

L_4c87:
    DrawPlanShip(0x0, 0x48);

L_4c9a:
    if ((LOWORD(game) != 0x0))
        goto L_4cae;
    else
        goto L_4ca4;

L_4ca4:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4cae;

L_4cae:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4cb5;

L_4cb5:

L_4cbb:
    lpProc = MakeProcInstance(BattlePlansDlg, hInst);
    DialogBox(hInst, IDD_BATTLE_PLANS, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_4cfb:
    if ((LOWORD(game) != 0x0))
        goto L_4d0f;
    else
        goto L_4d05;

L_4d05:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4d0f;

L_4d0f:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4d19;

L_4d19:
    if ((game.fSinglePlr != 0x0))
        goto L_536f;
    else
        goto L_4d28;

L_4d28:

L_4d2e:
    lpProc = MakeProcInstance(RelationsDlg, hInst);
    DialogBox(hInst, IDD_RELATIONS, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_4d6e:
    if ((LOWORD(game) != 0x0))
        goto L_4d82;
    else
        goto L_4d78;

L_4d78:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4d82;

L_4d82:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4d89;

L_4d89:

L_4d8f:
    vplr = rgplr[idPlayer];
    RaceCreationWizard(hwnd, 0x1, 0x0);
    goto L_536f;

L_4dc0:
    if ((LOWORD(game) != 0x0))
        goto L_4dd4;
    else
        goto L_4dca;

L_4dca:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4dd4;

L_4dd4:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4ddb;

L_4ddb:

L_4de1:
    NewGameWizard(hwnd, 0x1);
    goto L_536f;

L_4df3:
    if ((LOWORD(game) != 0x0))
        goto L_4e07;
    else
        goto L_4dfd;

L_4dfd:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4e07;

L_4e07:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4e0e;

L_4e0e:

L_4e14:
    pt.x = 0x262;
    pt.y = 0x1c2;
    if ((hwndPopup == 0x0))
        goto L_4e41;
    else
        goto L_4e28;

L_4e28:
    SendMessage(hwndPopup, WM_RBUTTONUP, 0x0, 0x0);

L_4e41:
    /* untranslated: call ShipBuilder(words(pt.y, pt.x)) -> callresult(int16_t) */
    goto L_536f;

L_4e52:
    DumpUniverse();
    goto L_536f;

L_4e5a:
    DumpPlanets();
    goto L_536f;

L_4e62:
    DumpFleets();
    goto L_536f;

L_4e6a:
    if ((LOWORD(game) != 0x0))
        goto L_4e7e;
    else
        goto L_4e74;

L_4e74:
    if ((HIWORD(game) == 0x0))
        goto L_536f;
    else
        goto L_4e7e;

L_4e7e:
    if ((idPlayer == 0xffff))
        goto L_536f;
    else
        goto L_4e88;

L_4e88:
    if ((game.fSinglePlr == 0x0))
        goto L_4eb6;
    else
        goto L_4e9a;

L_4e9a:
    if ((HIWORD(lSaltCur) > 0x0))
        goto L_4eb6;
    else
        goto L_4ea4;

L_4ea4:
    if ((HIWORD(lSaltCur) < 0x0))
        goto L_536f;
    else
        goto L_4ea9;

L_4ea9:
    if ((LOWORD(lSaltCur) <= 0x0))
        goto L_536f;
    else
        goto L_4eb0;

L_4eb0:

L_4eb6:
    if ((FCheckPassword() == 0x0))
        goto L_536f;
    else
        goto L_4ec0;

L_4ec0:

L_4ec6:
    lpProc = MakeProcInstance(NewPasswordDlg, hInst);
    fRet = DialogBox(hInst, IDD_NEW_PASSWORD, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    goto L_536f;

L_4f09:
    if ((hwndScanner != 0x0))
        goto L_4f37;
    else
        goto L_4f13;

L_4f13:
    AlertSz(PszFormatIds(idsCantChangeZoomFactorUntilGameOpen, 0x0), MB_ICONHAND);
    goto L_536f;

L_4f37:
    hmenu = GetASubMenu(hwnd, 0x1);
    hmenu = GetSubMenu(hmenu, 0x3);
    CheckMenuItem(hmenu, (iScanZoom + 0x4), 0x400);
    GetClientRect(hwndScanner, &(rc));
    rc.right = (ScanToPt(rc.right) >> 0x1);
    rc.bottom = (ScanToPt(rc.bottom) >> 0x1);
    dx = xScanTop;
    dy = (dGalInv - yScanTop);
    iScanZoom = (wParam + 0xf0bf);
    CheckMenuItem(hmenu, (iScanZoom + 0x4), 0x408);
    DrawMenuBar(hwnd);
    SetScanScrollBars(hwndScanner);
    InvalidateRect(hwndScanner, 0x0, 0x1);
    if ((sel.scan.grobj == grobjNone))
        goto L_500a;
    else
        goto L_4ffa;

L_4ffa:
    pt.x = sel.scan.pt.x;
    pt.y = sel.scan.pt.y;
    goto L_501c;

L_500a:
    pt.x = (dx + rc.right);
    pt.y = (dy - rc.bottom);

L_501c:
    /* untranslated: call CtrPointScan(words(pt.y, pt.x), 0x0) -> callresult(void) */
    goto L_536f;

L_5031:
    if ((hwndScanner == 0x0))
        goto L_536f;
    else
        goto L_5038;

L_5038:

L_503e:
    gd.fNoScannerDraw = 0x1;
    RestoreSelection();
    RefitFrameChildren();
    gd.fNoScannerDraw = 0x0;
    InvalidateRect(hwndScanner, 0x0, 0x1);
    UpdateWindow(hwndScanner);
    goto L_536f;

L_5081:
    if ((sel.grobj != grobjFleet))
        goto Default;
    else
        goto L_508b;

L_508b:
    if ((GetFocus() == hwndOrderED))
        goto Default;
    else
        goto L_5099;

L_5099:
    if ((wParam != 0x67))
        goto L_50a8;
    else
        goto L_50a2;

L_50a2:
    t_merge_50ab_0001 = 0x1;
    goto L_50ab;

L_50a8:
    t_merge_50ab_0001 = 0x0;

L_50ab:
    DeleteCurWayPoint(t_merge_50ab_0001);

Default:
    DefWindowProc(hwnd, 0x111, wParam, 0x0);
    goto L_536f;

L_50d4:
    if ((wParam == 0x53))
        goto L_4e62;
    else
        goto L_50dc;

L_50dc:
    if ((wParam == 0x54))
        goto L_4e5a;
    else
        goto L_50e4;

L_50e4:
    if ((wParam == 0x55))
        goto L_4e52;
    else
        goto L_50ec;

L_50ec:
    if ((wParam == 0x5f))
        goto L_3ee1;
    else
        goto L_50f4;

L_50f4:
    if ((wParam == 0x60))
        goto L_3ee1;
    else
        goto L_50fc;

L_50fc:
    if ((wParam == 0x63))
        goto L_2fa9;
    else
        goto L_5104;

L_5104:
    if ((wParam == 0x67))
        goto L_5081;
    else
        goto L_510c;

L_510c:
    if ((wParam == 0x68))
        goto L_5081;
    else
        goto L_5114;

L_5114:
    if ((wParam == 0x69))
        goto L_3f30;
    else
        goto L_511c;

L_511c:
    if ((wParam == 0x6a))
        goto LWaitForTurn;
    else
        goto L_5124;

L_5124:
    if ((wParam == 0x6c))
        goto L_3f6e;
    else
        goto L_512c;

L_512c:
    if ((wParam == 0x6d))
        goto L_4a53;
    else
        goto L_5134;

L_5134:
    if ((wParam == 0x6e))
        goto L_3083;
    else
        goto L_513c;

L_513c:
    if ((wParam == 0x6f))
        goto L_4998;
    else
        goto L_5144;

L_5144:
    if ((wParam == 0x71))
        goto L_4b14;
    else
        goto L_514c;

L_514c:
    if ((wParam == 0x7d))
        goto L_4df3;
    else
        goto L_5154;

L_5154:
    if ((wParam == 0x7e))
        goto L_4be3;
    else
        goto L_515c;

L_515c:
    if ((wParam == 0x81))
        goto L_3243;
    else
        goto L_5164;

L_5164:
    if ((wParam == 0x82))
        goto L_3003;
    else
        goto L_516c;

L_516c:
    if ((wParam == 0x83))
        goto L_3003;
    else
        goto L_5174;

L_5174:
    if ((wParam == 0x84))
        goto L_3003;
    else
        goto L_517c;

L_517c:
    if ((wParam == 0x87))
        goto L_4be3;
    else
        goto L_5184;

L_5184:
    if ((wParam == 0x88))
        goto L_440c;
    else
        goto L_518c;

L_518c:
    if ((wParam == 0x89))
        goto L_4df3;
    else
        goto L_5194;

L_5194:
    if ((wParam == 0x8a))
        goto L_47b3;
    else
        goto L_519c;

L_519c:
    if ((wParam == 0x9c))
        goto L_4d6e;
    else
        goto L_51a4;

L_51a4:
    if ((wParam == 0x9d))
        goto L_4d6e;
    else
        goto L_51ac;

L_51ac:
    if ((wParam == 0x9e))
        goto L_4dc0;
    else
        goto L_51b4;

L_51b4:
    if ((wParam == 0x9f))
        goto L_4dc0;
    else
        goto L_51bc;

L_51bc:
    if ((wParam == 0xb3))
        goto L_3269;
    else
        goto L_51c4;

L_51c4:
    if ((wParam == 0xd5))
        goto L_32e6;
    else
        goto L_51cc;

L_51cc:
    if ((wParam == 0xfa))
        goto L_4ae8;
    else
        goto L_51d4;

L_51d4:
    if ((wParam == 0xfb))
        goto L_4ae8;
    else
        goto L_51dc;

L_51dc:
    if ((wParam == 0xfc))
        goto L_4ae8;
    else
        goto L_51e4;

L_51e4:
    if ((wParam == 0xfd))
        goto L_4ae8;
    else
        goto L_51ec;

L_51ec:
    if ((wParam == 0x100))
        goto L_440c;
    else
        goto L_51f4;

L_51f4:
    if ((wParam == 0x101))
        goto L_47b3;
    else
        goto L_51fc;

L_51fc:
    if ((wParam == 0x10e))
        goto L_4e6a;
    else
        goto L_5204;

L_5204:
    if ((wParam == 0x428))
        goto L_4993;
    else
        goto L_520c;

L_520c:
    if ((wParam == 0x7d9))
        goto L_4cfb;
    else
        goto L_5214;

L_5214:
    if ((wParam == 0x7da))
        goto LWaitForTurn;
    else
        goto L_521c;

L_521c:
    if ((wParam == 0x7db))
        goto L_4c9a;
    else
        goto L_5224;

L_5224:
    if ((wParam == 0x7dc))
        goto L_4c9a;
    else
        goto L_522c;

L_522c:
    if ((wParam == 0x7de))
        goto L_4cfb;
    else
        goto L_5234;

L_5234:
    if ((wParam == 0x8fd))
        goto L_44cc;
    else
        goto L_523c;

L_523c:
    if ((wParam == 0x8fe))
        goto L_448f;
    else
        goto L_5244;

L_5244:
    if ((wParam == 0x8ff))
        goto L_44cc;
    else
        goto L_524c;

L_524c:
    if ((wParam == 0x900))
        goto L_44cc;
    else
        goto L_5254;

L_5254:
    if ((wParam == 0x901))
        goto L_44cc;
    else
        goto L_525c;

L_525c:
    if ((wParam == 0x98d))
        goto L_3e24;
    else
        goto L_5264;

L_5264:
    if ((wParam == 0x9c1))
        goto L_4786;
    else
        goto L_526c;

L_526c:
    if ((wParam == 0x9c2))
        goto L_4786;
    else
        goto L_5274;

L_5274:
    if ((wParam == 0x9c4))
        goto L_3052;
    else
        goto L_527c;

L_527c:
    if ((wParam == 0x9c5))
        goto L_3052;
    else
        goto L_5284;

L_5284:
    if ((wParam == 0xed8))
        goto L_3083;
    else
        goto L_528c;

L_528c:
    if ((wParam == 0xed9))
        goto L_4a53;
    else
        goto L_5294;

L_5294:
    if ((wParam == 0xeda))
        goto L_4998;
    else
        goto L_529c;

L_529c:
    if ((wParam == 0xedb))
        goto L_4998;
    else
        goto L_52a4;

L_52a4:
    if ((wParam == 0xee2))
        goto L_2fe8;
    else
        goto L_52ac;

L_52ac:
    if ((wParam == 0xf3d))
        goto L_4f09;
    else
        goto L_52b4;

L_52b4:
    if ((wParam == 0xf3e))
        goto L_4f09;
    else
        goto L_52bc;

L_52bc:
    if ((wParam == 0xf3f))
        goto L_4f09;
    else
        goto L_52c4;

L_52c4:
    if ((wParam == 0xf40))
        goto L_4f09;
    else
        goto L_52cc;

L_52cc:
    if ((wParam == 0xf41))
        goto L_4f09;
    else
        goto L_52d4;

L_52d4:
    if ((wParam == 0xf42))
        goto L_4f09;
    else
        goto L_52dc;

L_52dc:
    if ((wParam == 0xf43))
        goto L_4f09;
    else
        goto L_52e4;

L_52e4:
    if ((wParam == 0xf44))
        goto L_4f09;
    else
        goto L_52ec;

L_52ec:
    if ((wParam == 0xf45))
        goto L_4f09;
    else
        goto L_52f4;

L_52f4:
    if ((wParam == 0xfa1))
        goto L_5031;
    else
        goto L_52fc;

L_52fc:
    if ((wParam == 0x1068))
        goto L_3e92;
    else
        goto L_5304;

L_5304:
    if ((wParam == 0x1069))
        goto L_3e92;
    else
        goto L_530c;

L_530c:
    if ((wParam == 0x10cc))
        goto L_30b8;
    else
        goto L_5314;

L_5314:
    if ((wParam == 0x10cd))
        goto L_30b8;
    else
        goto L_531c;

L_531c:
    if ((wParam == 0x10ce))
        goto L_30b8;
    else
        goto L_5324;

L_5324:
    if ((wParam == 0x10cf))
        goto L_30b8;
    else
        goto L_532c;

L_532c:
    if ((wParam == 0x10d0))
        goto L_30b8;
    else
        goto L_5334;

L_5334:
    if ((wParam == 0x10d1))
        goto L_30b8;
    else
        goto L_533c;

L_533c:
    if ((wParam == 0x10d2))
        goto L_30b8;
    else
        goto L_5344;

L_5344:
    if ((wParam == 0x10d3))
        goto L_30b8;
    else
        goto L_534c;

L_534c:
    if ((wParam == 0x10d4))
        goto L_30b8;
    else
        goto L_5354;

L_5354:
    if ((wParam == 0x5208))
        goto L_3f6e;
    else
        goto L_535c;

L_535c:
    if ((wParam == 0x5209))
        goto L_3f6e;
    else
        goto L_5364;

L_5364:
    if ((wParam != 0x520a))
        goto Default;
    else
        goto L_5369;

L_5369:

L_536f:
    return;
}

void InitializeMenu(HMENU hmenu) {
    int16_t  cMenu;
    int16_t  i;
    HMENU    hmenuSub;
    uint16_t t_merge_548d_0001;
    uint16_t t_merge_54b2_0001;
    uint16_t t_merge_5502_0001;
    uint16_t t_merge_5539_0001;
    uint16_t t_merge_5570_0001;
    uint16_t t_merge_55af_0001;
    uint16_t t_merge_55d3_0001;

L_5376:
    if ((hmenu != 0x0))
        goto L_5394;
    else
        goto L_5388;

L_5388:
    hmenu = GetMenu(hwndFrame);

L_5394:
    hmenuSub = GetASubMenu(hwndFrame, 0x0);
    i = 0x10cc;
    goto L_53c3;

L_53af:
    DeleteMenu(hmenuSub, i, 0x0);
    i = (i + 0x1);

L_53c3:
    if ((i <= 0x10d4))
        goto L_53af;
    else
        goto L_53cd;

L_53cd:
    i = 0x0;
    goto L_5456;

L_53d5:
    /* untranslated: branch sext8to16(byte HIWORD(vrgszMRU):[(LOWORD(vrgszMRU) + (i * 0x100))]) == 0x0 ? L_545f : L_53f5 */

L_53f5:
    /* untranslated: part[0:1](szWork) = 0x26 */
    /* untranslated: part[1:2](szWork) = lobyte((i + 0x31)) */
    /* untranslated: part[2:1](szWork) = 0x20 */
    fstrcpy("", ((uint8_t *)(vrgszMRU) + (i * 0x100)));
    InsertMenu(hmenuSub, (i + 0x9), 0x400, (i + 0x10cc), szWork);
    goto L_5452;

L_5452:
    i = (i + 0x1);

L_5456:
    if ((i < 0x9))
        goto L_53d5;
    else
        goto L_545f;

L_545f:
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_548a;
    else
        goto L_5472;

L_5472:
    if ((game.fSinglePlr != 0x0))
        goto L_548a;
    else
        goto L_5484;

L_5484:
    t_merge_548d_0001 = 0x0;
    goto L_548d;

L_548a:
    t_merge_548d_0001 = 0x3;

L_548d:
    EnableMenuItem(hmenu, 0x6a, (t_merge_548d_0001 | 0x0));
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_54af;
    else
        goto L_54a9;

L_54a9:
    t_merge_54b2_0001 = 0x0;
    goto L_54b2;

L_54af:
    t_merge_54b2_0001 = 0x3;

L_54b2:
    EnableMenuItem(hmenu, 0x69, (t_merge_54b2_0001 | 0x0));
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_54ff;
    else
        goto L_54ce;

L_54ce:
    if ((game.fSinglePlr == 0x0))
        goto L_54f9;
    else
        goto L_54e0;

L_54e0:
    if ((HIWORD(lSaltCur) < 0x0))
        goto L_54ff;
    else
        goto L_54ea;

L_54ea:
    if ((HIWORD(lSaltCur) > 0x0))
        goto L_54f9;
    else
        goto L_54ef;

L_54ef:
    if ((LOWORD(lSaltCur) <= 0x0))
        goto L_54ff;
    else
        goto L_54f9;

L_54f9:
    t_merge_5502_0001 = 0x0;
    goto L_5502;

L_54ff:
    t_merge_5502_0001 = 0x3;

L_5502:
    EnableMenuItem(hmenu, 0x10e, (t_merge_5502_0001 | 0x0));
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_5536;
    else
        goto L_551e;

L_551e:
    if ((game.fSinglePlr != 0x0))
        goto L_5536;
    else
        goto L_5530;

L_5530:
    t_merge_5539_0001 = 0x0;
    goto L_5539;

L_5536:
    t_merge_5539_0001 = 0x3;

L_5539:
    EnableMenuItem(hmenu, 0x7de, (t_merge_5539_0001 | 0x0));
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_556d;
    else
        goto L_5555;

L_5555:
    if ((game.fSinglePlr != 0x0))
        goto L_556d;
    else
        goto L_5567;

L_5567:
    t_merge_5570_0001 = 0x0;
    goto L_5570;

L_556d:
    t_merge_5570_0001 = 0x3;

L_5570:
    EnableMenuItem(hmenu, 0xedb, (t_merge_5570_0001 | 0x0));
    hmenu = GetASubMenu(hwndFrame, 0x1);
    if ((gd.fToolbar == 0x0))
        goto L_55ac;
    else
        goto L_55a6;

L_55a6:
    t_merge_55af_0001 = 0x8;
    goto L_55af;

L_55ac:
    t_merge_55af_0001 = 0x0;

L_55af:
    CheckMenuItem(hmenu, 0xb3, t_merge_55af_0001);
    if (((grbitScan & 0x2000) == 0x0))
        goto L_55d0;
    else
        goto L_55ca;

L_55ca:
    t_merge_55d3_0001 = 0x8;
    goto L_55d3;

L_55d0:
    t_merge_55d3_0001 = 0x0;

L_55d3:
    CheckMenuItem(hmenu, 0x98d, t_merge_55d3_0001);
    if ((hwndScanner != 0x0))
        goto L_55fd;
    else
        goto L_55e3;

L_55e3:
    EnableMenuItem(GetMenu(hwndFrame), 0x1, 0x403);
    goto L_56ad;

L_55fd:
    EnableMenuItem(GetMenu(hwndFrame), 0x1, 0x400);
    hmenu = GetASubMenu(hwndFrame, 0x1);
    hmenu = GetSubMenu(hmenu, 0x3);
    CheckMenuItem(hmenu, (iScanZoom + 0x4), 0x408);
    cMenu = GetMenuItemCount(hmenu);
    i = 0x0;
    goto L_5670;

L_565c:
    EnableMenuItem(hmenu, i, 0x400);
    i = (i + 0x1);

L_5670:
    if ((i < cMenu))
        goto L_565c;
    else
        goto L_567b;

L_567b:
    hmenu = GetASubMenu(hwndFrame, 0x1);
    hmenu = GetSubMenu(hmenu, 0x4);
    CheckMenuItem(hmenu, iWindowLayout, 0x408);

L_56ad:
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

L_56bc:
    fSubmitSav = gd.fSubmit;
    fWorkDone = 0x0;
    if ((gd.fAisDone != 0x0))
        goto L_5893;
    else
        goto L_56eb;

L_56eb:

L_56f1:
    fHostSav = gd.fHostMode;
    if ((gd.fHostMode != 0x0))
        goto L_5729;
    else
        goto L_5714;

L_5714:
    DestroyCurGame();
    FLoadGame(0x56a2, 0x3f0);

L_5729:
    iPlayer = 0x0;
    goto L_5735;

L_5731:
    iPlayer = (iPlayer + 0x1);

L_5735:
    if ((iPlayer >= game.cPlayer))
        goto L_575d;
    else
        goto L_5740;

L_5740:
    /* untranslated: rgmdplr[iPlayer] = part[6:2](rgplr[iPlayer]) */
    goto L_5731;

L_575d:
    gd.fSubmit = 0x1;
    fErrSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    iPlayer = 0x0;
    goto L_5781;

L_577d:
    iPlayer = (iPlayer + 0x1);

L_5781:
    if ((iPlayer >= game.cPlayer))
        goto L_5848;
    else
        goto L_578c;

L_578c:
    UpdateProgressGauge(MulDiv(0x154, (iPlayer + 0x1), game.cPlayer));
    if ((((rgmdplr[iPlayer] >> 0x9) & 0x1) == 0x0))
        goto L_577d;
    else
        goto L_57c5;

L_57c5:
    fWorkDone = 0x1;
    gd.fGeneratingTurn = 0x1;
    gd.fHostMode = 0x1;
    fOpened = FOpenFile(dtLog, iPlayer, 0x20);
    gd.fGeneratingTurn = 0x0;
    LOWORD(gd) = ((LOWORD(gd) & 0xfff7) | ((fHostSav & 0x1) << 0x3));
    if ((fOpened == 0x0))
        goto L_582e;
    else
        goto L_5826;

L_5826:
    StreamClose();
    goto L_577d;

L_582e:
    DoAiTurn(iPlayer, rgmdplr[iPlayer]);

L_5848:
    LOWORD(gd) = ((LOWORD(gd) & 0xffef) | ((fSubmitSav & 0x1) << 0x4));
    if ((fWorkDone == 0x0))
        goto L_5881;
    else
        goto L_586c;

L_586c:
    DestroyCurGame();
    FLoadGame(0x56a2, 0x3f4);

L_5881:
    fFileErrSilent = fErrSav;
    gd.fAisDone = 0x1;

L_5893:
    return;
}

uint16_t GetASubMenu(HWND hwnd, int16_t iMenu) {
    int16_t  fChildMenu;
    HMENU    hmenu;
    uint16_t t_merge_58c7_0001;

L_589a:
    if ((hwndActive == 0x0))
        goto L_58c4;
    else
        goto L_58ad;

L_58ad:
    if ((IsZoomed(hwndActive) == 0x0))
        goto L_58c4;
    else
        goto L_58be;

L_58be:
    t_merge_58c7_0001 = 0x1;
    goto L_58c7;

L_58c4:
    t_merge_58c7_0001 = 0x0;

L_58c7:
    fChildMenu = t_merge_58c7_0001;
    hmenu = GetMenu(hwnd);
    hmenu = GetSubMenu(hmenu, (iMenu + fChildMenu));

L_58ed:
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
    uint16_t     t_merge_59be_0001;
    uint16_t     t_merge_5ab0_0001;

L_58f4:
    if ((ini.fStartupFile == 0x0))
        goto L_59a2;
    else
        goto L_590b;

L_590b:
    strcpy(szFile, 0x56a2);
    /* untranslated: byte 0x25:[strrchr(0x56a2, 0x5c)] = 0x0 */
    strrchr(szFile, 0x2e);
    /* untranslated: pch = callresult(char *) */
    /* untranslated: branch callresult(char *) != 0x0 ? L_5984 : L_594e */

L_594e:
    SetSzWorkFromDt(dtHost, 0xffff);
    strcpy(szFile, 0x57a4);
    pch = strrchr(szFile, 0x2e);

L_5984:
    /* untranslated: ofn.nFileExtension = part[1:0](szFile[pch]) */
    ofn.nFileOffset = 0x0;
    fFileErrSilent = 0x1;
    goto LGotFileName;

L_59a2:
    /* untranslated: part[0:1](szFile) = 0x0 */
    if ((fRaceOnly == 0x0))
        goto L_59bb;
    else
        goto L_59b5;

L_59b5:
    t_merge_59be_0001 = 0x317;
    goto L_59be;

L_59bb:
    t_merge_59be_0001 = 0x318;

L_59be:
    CchGetString(t_merge_59be_0001, szFilter);
    i = 0x0;
    goto L_59d3;

L_59cf:
    i = (i + 0x1);

L_59d3:
    if (((uint16_t)(szFilter[i]) == 0x0))
        goto L_5a0a;
    else
        goto L_59e7;

L_59e7:
    if (((uint16_t)(szFilter[i]) != 0x7c))
        goto L_59cf;
    else
        goto L_59fb;

L_59fb:
    szFilter[i] = 0x0;

L_5a0a:
    memset(ofn, 0x0, 0x48);
    LOWORD(ofn) = 0x48;
    HIWORD(ofn) = 0x0;
    ofn.hwndOwner = hwnd;
    LOWORD(ofn.lpstrFilter) = szFilter;
    /* untranslated: HIWORD(ofn.lpstrFilter) = ss */
    /* untranslated: part[20:2](ofn) = 0x1 */
    /* untranslated: part[22:2](ofn) = 0x0 */
    LOWORD(ofn.lpstrFile) = szFile;
    /* untranslated: HIWORD(ofn.lpstrFile) = ss */
    /* untranslated: part[28:2](ofn) = 0x100 */
    /* untranslated: part[30:2](ofn) = 0x0 */
    LOWORD(ofn.lpstrFileTitle) = szFileTitle;
    /* untranslated: HIWORD(ofn.lpstrFileTitle) = ss */
    /* untranslated: part[36:2](ofn) = 0x100 */
    /* untranslated: part[38:2](ofn) = 0x0 */
    ofn.lpstrInitialDir = "";
    /* untranslated: part[48:2](ofn) = 0x1804 */
    /* untranslated: part[50:2](ofn) = 0x0 */
    if ((GetOpenFileName(&(ofn)) == 0x0))
        goto L_5d95;
    else
        goto LGotFileName;

LGotFileName:
    /* untranslated: part[0:1](szDirName) = 0x0 */
    if ((fRaceOnly != 0x0))
        goto L_5aad;
    else
        goto L_5aa7;

L_5aa7:
    t_merge_5ab0_0001 = 0x1;
    goto L_5ab0;

L_5aad:
    t_merge_5ab0_0001 = 0x0;

L_5ab0:
    fRet = FWasRaceFile(szFile[ofn.nFileOffset], t_merge_5ab0_0001);
    if ((fRaceOnly == 0x0))
        goto L_5af7;
    else
        goto L_5ad0;

L_5ad0:
    if ((fRet <= 0x0))
        goto L_5af0;
    else
        goto L_5ada;

L_5ada:
    strcpy(0x1318, szFile[ofn.nFileOffset]);

L_5af0:
    return fRet;

L_5af7:
    if ((fRet == 0x0))
        goto L_5b64;
    else
        goto L_5b01;

L_5b01:
    if ((ini.fStartupFile == 0x0))
        goto L_5b29;
    else
        goto L_5b0f;

L_5b0f:
    if ((LOWORD(vSerialNumber) != 0x0))
        goto L_5b29;
    else
        goto L_5b19;

L_5b19:
    if ((HIWORD(vSerialNumber) != 0x0))
        goto L_5b29;
    else
        goto L_5b23;

L_5b23:
    fRet = 0xffff;

L_5b29:
    fFileErrSilent = 0x0;
    ini.fStartupFile = 0x0;
    if ((fRet != 0xffff))
        goto L_5b4b;
    else
        goto L_5b45;

L_5b45:
    return 0xffff;

L_5b4b:
    RaceCreationWizard(hwnd, 0x0, 0x0);
    return 0x0;

L_5b64:
    /* untranslated: byte ss:[bp+(ofn.nFileExtension + 0xffff)-0x14c] = 0x0 */
    DestroyCurGame();
    strcpy(0x56a2, szFile);
    if ((FLoadGame(szFile, szFile[ofn.nFileExtension]) != 0x0))
        goto L_5bc8;
    else
        goto L_5ba8;

L_5ba8:
    if ((ini.fStartupFile == 0x0))
        goto L_5bc2;
    else
        goto L_5bb6;

L_5bb6:
    ini.wFlags = 0x0;
    fFileErrSilent = 0x0;

L_5bc2:
    return 0x0;

L_5bc8:
    if ((ini.fStartupFile == 0x0))
        goto L_5c8b;
    else
        goto L_5bd6;

L_5bd6:
    fFileErrSilent = 0x0;
    ini.fStartupFile = 0x0;
    pch = strrchr(szFile, 0x5c);
    if ((pch == 0x0))
        goto L_5c30;
    else
        goto L_5c07;

L_5c07:
    i = szFile[pch];
    strncpy(0x252, szFile, i);
    /* untranslated: part[594:1](i) = 0x0 */

L_5c30:
    if ((idPlayer != 0xffff))
        goto L_5c5a;
    else
        goto L_5c3a;

L_5c3a:
    if ((ini.fGen != 0x0))
        goto L_5c5a;
    else
        goto L_5c4e;

L_5c4e:
    gd.fClose = 0x1;

L_5c5a:
    /* untranslated: branch LOWORD(game) != part[18:2](ini) ? L_5c7f : L_5c6a */

L_5c6a:
    /* untranslated: branch HIWORD(game) != part[20:2](ini) ? L_5c7f : L_5c73 */

L_5c73:
    if ((game.turn <= ini.turn))
        goto L_5c8b;
    else
        goto L_5c7f;

L_5c7f:
    ini.fTry = 0x0;

L_5c8b:
    sel.grobjFull = grobjNone;
    sel.grobj = grobjNone;
    sel.iwpAct = 0xffff;
    sel.id = 0xffff;
    sel.scan.grobjFull = grobjNone;
    sel.scan.grobj = grobjNone;
    sel.scan.iwp = 0xffff;
    sel.scan.ifl = 0xffff;
    sel.scan.idpl = 0xffff;
    fOrdersVis = 0x0;
    CreateChildWindows();
    if ((idPlayer == 0xffff))
        goto L_5d37;
    else
        goto L_5cd3;

L_5cd3:
    grobjIni = ini.grobjSel;
    SendMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((grobjIni == grobjNone))
        goto L_5d28;
    else
        goto L_5d0a;

L_5d0a:
    if ((ini.grobjSel != 0x0))
        goto L_5d28;
    else
        goto L_5d22;

L_5d22:
    return 0x1;

L_5d28:
    ini.grobjSel = 0x0;
    goto L_5d6b;

L_5d37:
    if ((hwndTitle == 0x0))
        goto L_5d60;
    else
        goto L_5d41;

L_5d41:
    if ((fFreeingTitle != 0x0))
        goto L_5d60;
    else
        goto L_5d4b;

L_5d4b:
    fFreeingTitle = 0x1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;

L_5d60:
    BringUpHostDlg();
    return 0x0;

L_5d6b:
    if ((ini.grobjSel != 0x0))
        goto L_5d9b;
    else
        goto L_5d83;

L_5d83:
    if ((cPlanet == 0x0))
        goto L_5d9b;
    else
        goto L_5d8d;

L_5d8d:
    FFindSomethingAndSelectIt();

L_5d95:
    return 0x0;

L_5d9b:
    return 0x1;
}

int16_t FWasRaceFile(char *szFile, int16_t fChkPass) {
    int16_t  idsError;
    int32_t  lSaltSav;
    PLAYER   plr;
    jmp_buf *penvMemSav[9];
    jmp_buf  env[9];
    int16_t  fRet;
    int16_t  fSav;

L_5da8:
    idsError = 0xffff;
    fRet = 0x0;
    fSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_5e48;
    else
        goto LBadFile;

LBadFile:
    StreamClose();
    penvMem = penvMemSav;
    fFileErrSilent = fSav;
    if ((fFileErrSilent != 0x0))
        goto L_5e41;
    else
        goto L_5e09;

L_5e09:
    if ((idsError == 0xffff))
        goto L_5e41;
    else
        goto L_5e12;

L_5e12:
    strcpy(0x57a4, szFile);
    AlertSz(PszFormatIds(idsError, 0x0), MB_ICONHAND);

L_5e41:
    return fRet;

L_5e48:
    StreamOpen(szFile, 0x20);
    ReadRt();
    if ((hdrCur.rt != rtBOF))
        goto L_5eb2;
    else
        goto L_5e6f;

L_5e6f:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0xc) & 0xf) != 0x2 ? L_5eb2 : L_5e82 */

L_5e82:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0x5) & 0x7f) < 0x31 ? L_5eb2 : L_5e9a */

L_5e9a:
    /* untranslated: branch ((part[8:2](rgbCur) >> 0x5) & 0x7f) < 0x54 ? L_5ec0 : L_5eb2 */

L_5eb2:
    idsError = 0xd;
    fRet = 0xffff;
    goto LBadFile;

L_5ec0:
    /* untranslated: wVersFile = part[8:2](rgbCur) */
    /* untranslated: branch (part[14:2](rgbCur) & 0xff) != 0x5 ? L_5dec : L_5ed1 */

L_5ed1:

L_5ed7:
    ReadRt();
    if ((hdrCur.rt != rtPlr))
        goto LBadFile;
    else
        goto L_5eec;

L_5eec:

L_5ef2:
    idsError = 0x3;
    ReadRtPlr(&(plr), 0x4b98);
    ReadRt();
    if ((hdrCur.rt != rtEOF))
        goto LBadFile;
    else
        goto L_5f1d;

L_5f1d:

L_5f23:
    if ((LOWORD(rgbCur) != IRaceChecksum(&(plr))))
        goto LBadFile;
    else
        goto L_5f36;

L_5f36:

L_5f3c:
    lSaltSav = lSaltCur;
    /* untranslated: LOWORD(lSaltCur) = part[12:2](plr) */
    /* untranslated: HIWORD(lSaltCur) = part[14:2](plr) */
    if ((fChkPass == 0x0))
        goto L_5f84;
    else
        goto L_5f61;

L_5f61:
    if ((FCheckPassword() != 0x0))
        goto L_5f84;
    else
        goto L_5f6e;

L_5f6e:
    lSaltCur = lSaltSav;
    fRet = 0xffff;
    goto LBadFile;

L_5f84:
    lSaltCur = lSaltSav;
    /* untranslated: branch part[12:2](plr) != 0x0 ? L_5fa5 : L_5f9b */

L_5f9b:
    /* untranslated: branch part[14:2](plr) == 0x0 ? L_5fb8 : L_5fa5 */

L_5fa5:
    strcpy(0x1308, 0x22e0);
    goto L_5fbd;

L_5fb8:
    /* untranslated: part[0:1](szRacePass) = 0x0 */

L_5fbd:
    vplr = plr;
    strcpy(0x1318, szFile);
    StreamClose();
    fFileErrSilent = fSav;
    penvMem = penvMemSav;
    return 0x1;
}

void BringUpHostDlg() {
    POINT pt;
    int16_t (**lpProc)();
    int16_t fRet;

L_5ffc:
    if ((gd.fHostMode != 0x0))
        goto L_6055;
    else
        goto L_6019;

L_6019:
    if ((gd.fReadOnly != 0x0))
        goto L_6049;
    else
        goto L_6031;

L_6031:
    FMarkFile(dtHost, 0xffff, 0x1, 0x1);

L_6049:
    gd.fHostMode = 0x1;

L_6055:
    ShowWindow(hwndFrame, SW_HIDE);
    if ((ini.fWait == 0x0))
        goto Top;
    else
        goto L_6074;

L_6074:
    ini.fWait = 0x0;
    goto LAutoMode;

Top:
    lpProc = MakeProcInstance(HostModeDialog, hInst);
    fRet = DialogBox(hInst, IDD_HOST_MODE, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet != 0xffff))
        goto L_6117;
    else
        goto LAutoMode;

LAutoMode:
    ShowWindow(hwndFrame, SW_SHOWMINIMIZED);
    uTimerId = SetTimer(0x0, 0xd, 0x2710, lpfnHostTimerProc);
    uTimerType = 0xd;
    HostTimerProc(0x0, WM_NULL, uTimerId, 0x0);
    goto L_623a;

L_6117:
    if ((fRet == 0x0))
        goto L_6184;
    else
        goto LNextGen;

LNextGen:
    if ((gd.fProgressTxt == 0x0))
        goto L_6138;
    else
        goto L_6133;

L_6133:
    ShowProgressGauge();

L_6138:
    EnsureAis();
    FGenerateTurn();
    if ((iPassCnt == 0x0))
        goto L_6176;
    else
        goto L_614c;

L_614c:
    iPassCnt = (iPassCnt - 0x1);
    if ((GetAsyncKeyState(0x10) < 0x0))
        goto L_6176;
    else
        goto L_6162;

L_6162:
    if ((GetAsyncKeyState(0x11) >= 0x0))
        goto LNextGen;
    else
        goto L_6170;

L_6170:

L_6176:
    iPassCnt = 0x0;
    HideProgressGauge();
    goto Top;

L_6184:
    if ((gd.fReadOnly != 0x0))
        goto L_61b4;
    else
        goto L_619c;

L_619c:
    FMarkFile(dtHost, 0xffff, 0x1, 0x0);

L_61b4:
    gd.fHostMode = 0x0;
    DestroyCurGame();
    if ((ini.fGen != 0x0))
        goto L_623a;
    else
        goto L_61d9;

L_61d9:
    pt.x = GetSystemMetrics(SM_CXSCREEN);
    pt.y = GetSystemMetrics(SM_CYSCREEN);
    hwndTitle = CreateWindow(szTitle, "Stars!", 0x90000000, 0x0, 0x0, pt.x, pt.y, hwndFrame, 0x0, hInst, 0x0);
    fFreeingTitle = 0x0;

L_623a:
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
    uint32_t t_merge_6379_0001;

L_6240:
    if ((hdcIn == 0x0))
        goto L_625b;
    else
        goto L_6252;

L_6252:
    hdc = hdcIn;
    goto L_6266;

L_625b:
    hdc = GetDC(hwnd);

L_6266:
    bkMode = SetBkMode(hdc, OPAQUE);
    crBackSav = SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, HIWORD(rghfontArial8));
    x = ((dyArial8 + 0xa) + LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN16), 0x4)));
    yCur = 0x30;
    SetRect(&(rcDiamond), 0x6, yCur, (dyArial8 + 0x7), ((yCur + dyArial8) + 0x1));
    i = 0x0;
    goto L_62f5;

L_62f1:
    i = (i + 0x1);

L_62f5:
    if ((i >= game.cPlayer))
        goto L_649f;
    else
        goto L_6300;

L_6300:
    DrawDiamond(hdc, &(rcDiamond), hbrBBlue);
    cch = _wsprintf(szWork, PszGetCompressedString(idsD2), (i + 0x1));
    RightTextOut(hdc, x, yCur, 0x57a4, cch, 0x0);
    if ((rgOut[i] <= 0x0))
        goto L_6373;
    else
        goto L_636a;

L_636a:
    t_merge_6379_0001 = 0x7f;
    goto L_6379;

L_6373:
    t_merge_6379_0001 = 0x7f00;

L_6379:
    SetTextColor(hdc, t_merge_6379_0001);
    /* untranslated: call CchGetString(part[122:0](szDirName[rgOut[i]]), szStat) -> callresult(int16_t) */
    if ((gd.fNoHostNames == 0x0))
        goto L_63cf;
    else
        goto L_63ac;

L_63ac:
    /* untranslated: cch = _wsprintf(szWork, " %s", szStat, ss) */
    goto L_641b;

L_63cf:
    /* untranslated: cch = _wsprintf(szWork, PszGetCompressedString(idsSS), PszPlayerName(i, 0x1, 0x1, 0x1, 0x0, 0x0), 0x25, szStat, ss) */

L_641b:
    /* untranslated: branch ((part[84:2](rgplr[i]) >> 0x4) & 0x1) == 0x0 ? L_6450 : L_643c */

L_643c:
    strcat(0x57a4, 0x403);
    cch = (cch + 0x9);

L_6450:
    TextOut(hdc, (x + 0x4), yCur, szWork, cch);
    SetTextColor(hdc, crWindowText);
    OffsetRect(&(rcDiamond), 0x0, (dyArial8 + 0x4));
    yCur = (yCur + (dyArial8 + 0x4));
    goto L_62f1;

L_649f:
    cch = _wsprintf(szWork, PCTD, (game.turn + 0x961));
    SetWindowText(GetDlgItem(hwnd, IDC_U16_0x07E0), szWork);
    GetTickCount();
    /* untranslated: dsec = (uint32_t)(words((hiword(callresult(uint32_t)) - HIWORD(ctickLast)), (loword(callresult(uint32_t)) - LOWORD(ctickLast))) / 0x3e8) */
    if ((HIWORD(dsec) > 0x0))
        goto L_653d;
    else
        goto L_6503;

L_6503:
    if ((HIWORD(dsec) < 0x0))
        goto L_6511;
    else
        goto L_6508;

L_6508:
    if ((LOWORD(dsec) >= 0x3c))
        goto L_653d;
    else
        goto L_6511;

L_6511:
    cch = _wsprintf(szWork, PszGetCompressedString(idsDSeconds), LOWORD(dsec));
    goto L_663a;

L_653d:
    dmin = LOWORD((uint32_t)((dsec / 0x3c)));
    LOWORD(dsec) = (LOWORD(dsec) - LOWORD((0x3c * dmin)));
    HIWORD(dsec) = (HIWORD(dsec) - 0x0);
    if ((dmin >= 0x3c))
        goto L_659b;
    else
        goto L_656b;

L_656b:
    cch = _wsprintf(szWork, PszGetCompressedString(idsD02d), dmin, LOWORD(dsec));
    goto L_663a;

L_659b:
    /* untranslated: dhour = (words(dmin, 0x0) / 0x3c) */
    dmin = (dmin - LOWORD((0x3c * dhour)));
    if ((dhour >= 0x18))
        goto L_65ee;
    else
        goto L_65ba;

L_65ba:
    cch = _wsprintf(szWork, PszGetCompressedString(idsD02d02d), dhour, dmin, LOWORD(dsec));
    goto L_663a;

L_65ee:
    /* untranslated: dday = (words(dhour, 0x0) / 0x18) */
    dhour = (dhour - LOWORD((dday * 0x18)));
    cch = _wsprintf(szWork, PszGetCompressedString(idsDDaysD02d02d), dday, dhour, dmin, LOWORD(dsec));

L_663a:
    SetWindowText(GetDlgItem(hwnd, IDC_U16_0x07E1), szWork);
    SetBkMode(hdc, bkMode);
    SetBkColor(hdc, crBackSav);
    if ((hdcIn != 0x0))
        goto L_6680;
    else
        goto L_6675;

L_6675:
    ReleaseDC(hwnd, hdc);

L_6680:
    return;
}

void VerifyTurns() {
    int16_t idsError;
    int16_t idCur;
    int16_t cAi;
    int16_t i;
    int16_t cOut;
    int16_t fOut;

L_6686:
    idCur = idPlayer;
    cOut = 0x0;
    cAi = 0x0;
    lpcd = LpAlloc(0x2ee0, htMisc);
    lpxf = LpAlloc(0x61a8, htMisc);
    vrgPlanResExtra = LpAlloc((game.cPlanMax * 0x2), htMisc);
    fmemset(&(vrgPlanResExtra), 0x0, (game.cPlanMax * 0x2));
    vrgts = LpAlloc((game.cPlayer * 0x10), htMisc);
    cColDrop = 0x0;
    cXferFull = 0x0;
    imemMsgCur = 0x0;
    i = 0x0;
    goto L_673d;

L_6739:
    i = (i + 0x1);

L_673d:
    if ((i >= game.cPlayer))
        goto L_690a;
    else
        goto L_6748;

L_6748:
    fOut = rgOut[i];
    idsError = 0x0;
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) != 0x0 ? L_678e : L_6777 */

L_6777:
    if ((FCheckLogFile(i, &(idsError)) == 0x0))
        goto L_6825;
    else
        goto L_678e;

L_678e:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) == 0x0 ? L_67be : L_67ac */

L_67ac:
    cAi = (cAi + 0x1);
    rgOut[i] = 0x0;
    goto L_68d6;

L_67be:
    _wsprintf(szWork, "%s.x%d", 0x56a2, 0x25, (i + 0x1));
    idPlayer = i;
    if ((FLoadLogFile(0x57a4) == 0x0))
        goto L_6817;
    else
        goto L_67fc;

L_67fc:
    if ((FRunLogFile() != 0x0))
        goto L_6817;
    else
        goto L_6809;

L_6809:
    rgOut[i] = 0x3;
    goto L_68d6;

L_6817:
    rgOut[i] = 0x0;

L_6825:
    if ((idsError == 0x0))
        goto L_687b;
    else
        goto L_682e;

L_682e:
    goto L_6861;

L_6834:
    rgOut[i] = 0x5;
    goto L_6874;

L_6842:
    rgOut[i] = 0x4;
    goto L_6874;

L_6850:
    rgOut[i] = 0x3;
    goto L_6874;

L_6861:
    if ((idsError == 0x1c))
        goto L_6842;
    else
        goto L_6869;

L_6869:
    if ((idsError != 0x1d))
        goto L_6850;
    else
        goto L_686e;

L_686e:

L_6874:
    cOut = (cOut + 0x1);
    goto L_68d6;

L_687b:
    /* untranslated: branch (part[84:2](rgplr[i]) & 0x1) == 0x0 ? L_68a2 : L_6894 */

L_6894:
    rgOut[i] = 0xffff;
    goto L_68d6;

L_68a2:
    if ((gd.fPartialTurn == 0x0))
        goto L_68c7;
    else
        goto L_68b5;

L_68b5:
    rgOut[i] = 0x2;
    cOut = (cOut + 0x1);
    goto L_68d6;

L_68c7:
    rgOut[i] = 0x1;
    cOut = (cOut + 0x1);

L_68d6:
    if ((LOWORD(ctickLast) != 0x0))
        goto L_68ea;
    else
        goto L_68e0;

L_68e0:
    if ((HIWORD(ctickLast) == 0x0))
        goto L_68fb;
    else
        goto L_68ea;

L_68ea:
    if ((rgOut[i] == fOut))
        goto L_6739;
    else
        goto L_68fb;

L_68fb:
    ctickLast = GetTickCount();

L_690a:
    FreeLp(&(vrgPlanResExtra), htMisc);
    vrgPlanResExtra = 0x0;
    FreeLp(&(vrgts), htMisc);
    vrgts = 0x0;
    FreeLp(&(lpcd), htMisc);
    lpcd = 0x0;
    FreeLp(&(lpxf), htMisc);
    lpxf = 0x0;
    idPlayer = idCur;
    return;
}

int16_t CTurnsOutSafe() {
    int16_t idPlayerSav;
    int16_t fHostModeSav;
    int16_t fGenSav;
    int16_t cturn;

L_6996:
    fHostModeSav = gd.fHostMode;
    fGenSav = gd.fGeneratingTurn;
    idPlayerSav = idPlayer;
    idPlayer = 0xffff;
    gd.fHostMode = 0x1;
    gd.fGeneratingTurn = 0x0;
    cturn = CFindTurnsOutstanding();
    LOWORD(gd) = ((LOWORD(gd) & 0xfffd) | ((fGenSav & 0x1) << 0x1));
    LOWORD(gd) = ((LOWORD(gd) & 0xfff7) | ((fHostModeSav & 0x1) << 0x3));
    idPlayer = idPlayerSav;

L_6a1f:
    return cturn;
}

int16_t CFindTurnsOutstanding() {
    int16_t  idsError;
    int16_t  cAi;
    int16_t  i;
    int16_t  cOut;
    int16_t  fSav;
    int16_t  fOut;
    uint16_t t_merge_6be3_0001;

L_6a26:
    cOut = 0x0;
    cAi = 0x0;
    fSav = fFileErrSilent;
    fFileErrSilent = 0x1;
    gd.fGeneratingTurn = 0x1;
    i = 0x0;
    goto L_6a5d;

L_6a59:
    i = (i + 0x1);

L_6a5d:
    if ((i >= game.cPlayer))
        goto L_6bc3;
    else
        goto L_6a68;

L_6a68:
    fOut = rgOut[i];
    idsError = 0x0;
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) != 0x0 ? L_6aae : L_6a97 */

L_6a97:
    if ((FCheckLogFile(i, &(idsError)) == 0x0))
        goto L_6ade;
    else
        goto L_6aae;

L_6aae:
    /* untranslated: branch ((part[6:2](rgplr[i]) >> 0x9) & 0x1) == 0x0 ? L_6ad0 : L_6acc */

L_6acc:
    cAi = (cAi + 0x1);

L_6ad0:
    rgOut[i] = 0x0;
    goto L_6b8f;

L_6ade:
    if ((idsError == 0x0))
        goto L_6b34;
    else
        goto L_6ae7;

L_6ae7:
    goto L_6b1a;

L_6aed:
    rgOut[i] = 0x5;
    goto L_6b2d;

L_6afb:
    rgOut[i] = 0x4;
    goto L_6b2d;

L_6b09:
    rgOut[i] = 0x3;
    goto L_6b2d;

L_6b1a:
    if ((idsError == 0x1c))
        goto L_6afb;
    else
        goto L_6b22;

L_6b22:
    if ((idsError != 0x1d))
        goto L_6b09;
    else
        goto L_6b27;

L_6b27:

L_6b2d:
    cOut = (cOut + 0x1);
    goto L_6b8f;

L_6b34:
    /* untranslated: branch (part[84:2](rgplr[i]) & 0x1) == 0x0 ? L_6b5b : L_6b4d */

L_6b4d:
    rgOut[i] = 0xffff;
    goto L_6b8f;

L_6b5b:
    if ((gd.fPartialTurn == 0x0))
        goto L_6b80;
    else
        goto L_6b6e;

L_6b6e:
    rgOut[i] = 0x2;
    cOut = (cOut + 0x1);
    goto L_6b8f;

L_6b80:
    rgOut[i] = 0x1;
    cOut = (cOut + 0x1);

L_6b8f:
    if ((LOWORD(ctickLast) != 0x0))
        goto L_6ba3;
    else
        goto L_6b99;

L_6b99:
    if ((HIWORD(ctickLast) == 0x0))
        goto L_6bb4;
    else
        goto L_6ba3;

L_6ba3:
    if ((rgOut[i] == fOut))
        goto L_6a59;
    else
        goto L_6bb4;

L_6bb4:
    ctickLast = GetTickCount();

L_6bc3:
    gd.fGeneratingTurn = 0x0;
    if ((cAi != game.cPlayer))
        goto L_6be0;
    else
        goto L_6bda;

L_6bda:
    t_merge_6be3_0001 = 0x1;
    goto L_6be3;

L_6be0:
    t_merge_6be3_0001 = 0x0;

L_6be3:
    HIWORD(gd) = ((HIWORD(gd) & 0xffef) | ((t_merge_6be3_0001 & 0x1) << 0x4));
    fFileErrSilent = 0x0;

L_6c0f:
    return cOut;
}

int16_t HostModeDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t (**lpProc)();
    int16_t     fRet;
    RECT        rc;
    int16_t     mf;
    POINT       pt;
    int16_t     tpm;
    int16_t     i;
    int16_t     iRet;
    int16_t     iSel;
    int16_t     iDiamond;
    HMENU       hmenuPopup;
    MSG         msg;
    HDC         hdc;
    PAINTSTRUCT ps;
    uint16_t    t_merge_6cac_0001;
    uint16_t    t_merge_6ce0_0001;
    uint16_t    t_merge_6d14_0001;
    uint16_t    t_merge_6f12_0001;
    uint16_t    t_merge_6f5d_0001;
    uint16_t    t_merge_6f77_0001;
    uint16_t    t_merge_6fbd_0001;
    uint16_t    t_merge_7057_0001;
    uint16_t    t_merge_7107_0001;
    uint16_t    t_merge_712d_0001;
    uint16_t    t_merge_7198_0001;
    uint16_t    t_merge_723c_0001;
    uint16_t    t_merge_73b8_0001;
    uint16_t    t_merge_7519_0001;

L_6c16:
    goto L_756d;

L_6c25:
    StickyDlgPos(hwnd, 0x416, 0x1);
    SetWindowText(GetDlgItem(hwnd, 0x409), "");
    SetWindowText(GetDlgItem(hwnd, 0x40a), szBase);
    GetDlgItem(hwnd, 0x408);
    if ((gd.fReadOnly != 0x0))
        goto L_6ca9;
    else
        goto L_6c8f;

L_6c8f:
    if ((vtimer.fAutoGenWhenIn != 0x0))
        goto L_6ca3;
    else
        goto L_6c99;

L_6c99:
    if ((vtimer.mdForce == 0x0))
        goto L_6ca9;
    else
        goto L_6ca3;

L_6ca3:
    t_merge_6cac_0001 = 0x1;
    goto L_6cac;

L_6ca9:
    t_merge_6cac_0001 = 0x0;

L_6cac:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_6cac_0001) -> callresult(int16_t) */
    GetDlgItem(hwnd, 0x407);
    if ((gd.fReadOnly != 0x0))
        goto L_6cdd;
    else
        goto L_6cd7;

L_6cd7:
    t_merge_6ce0_0001 = 0x1;
    goto L_6ce0;

L_6cdd:
    t_merge_6ce0_0001 = 0x0;

L_6ce0:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_6ce0_0001) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDC_U16_0x07DF);
    if ((gd.fReadOnly != 0x0))
        goto L_6d11;
    else
        goto L_6d0b;

L_6d0b:
    t_merge_6d14_0001 = 0x1;
    goto L_6d14;

L_6d11:
    t_merge_6d14_0001 = 0x0;

L_6d14:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_6d14_0001) -> callresult(int16_t) */
    uTimerId = SetTimer(hwnd, 0xd, 0x2710, 0x0);

L_6d35:
    if ((fProcessingTimer != 0x0))
        goto Done;
    else
        goto L_6d3c;

L_6d3c:

L_6d42:
    fProcessingTimer = 0x1;
    CFindTurnsOutstanding();
    DrawHostDialog2(hwnd, 0x0);
    fProcessingTimer = 0x0;

Done:
    if ((message == WM_TIMER))
        goto L_6d72;
    else
        goto L_6d6c;

L_6d6c:
    return 0x1;

L_6d72:
    return 0x0;

L_6d78:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_6da0:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_6db6:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.x < 0x6))
        goto L_75c0;
    else
        goto L_6dda;

L_6dda:
    if ((pt.x >= (dyArial8 + 0x7)))
        goto L_75c0;
    else
        goto L_6de8;

L_6de8:
    if ((pt.y < 0x30))
        goto L_75c0;
    else
        goto L_6dee;

L_6dee:

L_6df4:
    iDiamond = ((uint32_t)((pt.y + 0xffd0)) / (dyArial8 + 0x4));
    if ((iDiamond >= game.cPlayer))
        goto L_75c0;
    else
        goto L_6e12;

L_6e12:
    if ((((uint32_t)((pt.y + 0xffd0)) % (dyArial8 + 0x4)) >= (dyArial8 + 0x1)))
        goto L_75c0;
    else
        goto L_6e2c;

L_6e2c:

L_6e32:
    if ((message != WM_SETCURSOR))
        goto L_6e4a;
    else
        goto L_6e3b;

L_6e3b:
    SetCursor(hcurHand);
    return 0x1;

L_6e4a:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0x9) & 0x1) == 0x0 ? L_6e96 : L_6e68 */

L_6e68:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0xd) & 0x7) != 0x7 ? L_6e8e : L_6e86 */

L_6e86:
    iSel = 0x2;
    goto L_6e9b;

L_6e8e:
    iSel = 0x1;

L_6e96:
    iSel = 0x0;

L_6e9b:
    hmenuPopup = CreatePopupMenu();
    iPopMenuSel = 0xffff;
    i = 0x0;
    goto L_6f92;

L_6eb1:
    CchGetString((i + 0x20f), 0x57a4);
    if ((i != 0x1))
        goto L_6f18;
    else
        goto L_6ecd;

L_6ecd:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0x9) & 0x1) == 0x0 ? L_6f0f : L_6eeb */

L_6eeb:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0xd) & 0x7) == 0x7 ? L_6f0f : L_6f09 */

L_6f09:
    t_merge_6f12_0001 = 0x0;
    goto L_6f12;

L_6f0f:
    t_merge_6f12_0001 = 0x3;

L_6f12:
    mf = t_merge_6f12_0001;
    goto L_6f60;

L_6f18:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0x9) & 0x1) == 0x0 ? L_6f54 : L_6f36 */

L_6f36:
    /* untranslated: branch ((part[6:2](rgplr[iDiamond]) >> 0xd) & 0x7) != 0x7 ? L_6f5a : L_6f54 */

L_6f54:
    t_merge_6f5d_0001 = 0x0;
    goto L_6f5d;

L_6f5a:
    t_merge_6f5d_0001 = 0x3;

L_6f5d:
    mf = t_merge_6f5d_0001;

L_6f60:
    if ((i != iSel))
        goto L_6f74;
    else
        goto L_6f6e;

L_6f6e:
    t_merge_6f77_0001 = 0x8;
    goto L_6f77;

L_6f74:
    t_merge_6f77_0001 = 0x0;

L_6f77:
    AppendMenu(hmenuPopup, (t_merge_6f77_0001 | mf), (i + 0x3a98), szWork);
    i = (i + 0x1);

L_6f92:
    if ((i < 0x3))
        goto L_6eb1;
    else
        goto L_6f9b;

L_6f9b:
    ClientToScreen(hwnd, &(pt));
    if ((message != WM_LBUTTONDOWN))
        goto L_6fba;
    else
        goto L_6fb4;

L_6fb4:
    t_merge_6fbd_0001 = 0x0;
    goto L_6fbd;

L_6fba:
    t_merge_6fbd_0001 = 0x2;

L_6fbd:
    tpm = t_merge_6fbd_0001;
    TrackPopupMenu(hmenuPopup, (0x4 | tpm), pt.x, pt.y, 0x0, hwnd, 0x0);
    DestroyMenu(hmenuPopup);
    iRet = 0xffff;
    if ((PeekMessage(&(msg), hwnd, 0x111, 0x111, 0x2) == 0x0))
        goto L_7031;
    else
        goto L_7014;

L_7014:
    if ((msg.wParam < 0x3a98))
        goto L_7031;
    else
        goto L_701e;

L_701e:
    if ((msg.wParam >= 0x3afc))
        goto L_7031;
    else
        goto L_7028;

L_7028:
    iRet = (msg.wParam + 0xc568);

L_7031:
    if ((iRet == 0xffff))
        goto L_71b3;
    else
        goto L_703a;

L_703a:
    if ((iSel == iRet))
        goto L_71b3;
    else
        goto L_7045;

L_7045:
    if ((iRet <= 0x0))
        goto L_7054;
    else
        goto L_704e;

L_704e:
    t_merge_7057_0001 = 0x1;
    goto L_7057;

L_7054:
    t_merge_7057_0001 = 0x0;

L_7057:
    /* untranslated: part[6:2](rgplr[iDiamond]) = ((part[6:2](rgplr[iDiamond]) & 0xfdff) | ((t_merge_7057_0001 & 0x1) * 0x200)) */
    if ((iRet != 0x2))
        goto L_70c3;
    else
        goto L_709b;

L_709b:
    /* untranslated: part[6:2](rgplr[iDiamond]) = ((part[6:2](rgplr[iDiamond]) & 0x1fff) | 0xe000) */

L_70c3:
    /* untranslated: ss:[bp-0x36] = (part[14:2](rgplr[iDiamond]) ~ 0x0) */
    /* untranslated: part[12:2](rgplr[iDiamond]) = (part[12:2](rgplr[iDiamond]) ~ 0x0) */
    /* untranslated: part[14:2](rgplr[iDiamond]) = ss:[bp-0x36] */
    if ((iRet == 0x0))
        goto L_7104;
    else
        goto L_70fe;

L_70fe:
    t_merge_7107_0001 = 0x1;
    goto L_7107;

L_7104:
    t_merge_7107_0001 = 0x0;

L_7107:
    FMarkFile(dtTurn, iDiamond, 0x8, t_merge_7107_0001);
    if ((iRet == 0x0))
        goto L_712a;
    else
        goto L_7124;

L_7124:
    t_merge_712d_0001 = 0x1;
    goto L_712d;

L_712a:
    t_merge_712d_0001 = 0x0;

L_712d:
    FMarkFile(dtHost, iDiamond, 0x8, t_merge_712d_0001);
    gd.fAisDone = 0x0;
    fProcessingTimer = 0x1;
    CFindTurnsOutstanding();
    GetDlgItem(hwnd, 0x408);
    if ((gd.fAllAis != 0x0))
        goto L_7195;
    else
        goto L_717b;

L_717b:
    if ((vtimer.fAutoGenWhenIn != 0x0))
        goto L_718f;
    else
        goto L_7185;

L_7185:
    if ((vtimer.mdForce == 0x0))
        goto L_7195;
    else
        goto L_718f;

L_718f:
    t_merge_7198_0001 = 0x1;
    goto L_7198;

L_7195:
    t_merge_7198_0001 = 0x0;

L_7198:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_7198_0001) -> callresult(int16_t) */
    DrawHostDialog2(hwnd, 0x0);
    fProcessingTimer = 0x0;

L_71b3:
    return 0x0;

L_71b9:
    hdc = BeginPaint(hwnd, &(ps));
    if ((LOWORD(ctickLast) != 0x0))
        goto L_71e4;
    else
        goto L_71d5;

L_71d5:
    if ((HIWORD(ctickLast) != 0x0))
        goto L_71e4;
    else
        goto L_71df;

L_71df:
    CFindTurnsOutstanding();

L_71e4:
    if ((gd.fReadOnly != 0x0))
        goto L_7242;
    else
        goto L_71fc;

L_71fc:
    GetDlgItem(hwnd, 0x408);
    if ((gd.fAllAis != 0x0))
        goto L_7239;
    else
        goto L_721f;

L_721f:
    if ((vtimer.fAutoGenWhenIn != 0x0))
        goto L_7233;
    else
        goto L_7229;

L_7229:
    if ((vtimer.mdForce == 0x0))
        goto L_7239;
    else
        goto L_7233;

L_7233:
    t_merge_723c_0001 = 0x1;
    goto L_723c;

L_7239:
    t_merge_723c_0001 = 0x0;

L_723c:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_723c_0001) -> callresult(int16_t) */

L_7242:
    DrawHostDialog2(hwnd, hdc);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_7265:
    if ((wParam == 0x407))
        goto L_7282;
    else
        goto L_726f;

L_726f:
    if ((wParam == 0x2))
        goto L_7282;
    else
        goto L_7278;

L_7278:
    if ((wParam != 0x408))
        goto L_73fd;
    else
        goto L_7282;

L_7282:
    if ((wParam != 0x407))
        goto L_7380;
    else
        goto L_728c;

L_728c:
    if ((GetAsyncKeyState(0x10) >= 0x0))
        goto L_72c0;
    else
        goto L_729d;

L_729d:
    if ((GetAsyncKeyState(0x11) >= 0x0))
        goto L_72b7;
    else
        goto L_72ae;

L_72ae:
    iPassCnt = 0x3e7;
    goto L_72e0;

L_72b7:
    iPassCnt = 0x9;

L_72c0:
    if ((GetAsyncKeyState(0x11) >= 0x0))
        goto L_72da;
    else
        goto L_72d1;

L_72d1:
    iPassCnt = 0x63;
    goto L_72e0;

L_72da:
    iPassCnt = 0x0;

L_72e0:
    if ((iPassCnt == 0x0))
        goto L_7344;
    else
        goto L_72ea;

L_72ea:
    _wsprintf(szWork, PszGetCompressedString(idsSureWantForceGenerateDTurnsRow), (iPassCnt + 0x1));
    if ((MessageBox(GetFocus(), szWork, "Stars!", 0x2034) == 0x6))
        goto L_7380;
    else
        goto L_7335;

L_7335:
    iPassCnt = 0x0;
    return 0x1;

L_7344:
    if ((CFindTurnsOutstanding() == 0x0))
        goto L_7380;
    else
        goto L_7351;

L_7351:
    if ((AlertSz(PszFormatIds(idsSureWishGenerateOptionDoesGuaranteePlayers, 0x0), 0x1024) == 0x6))
        goto L_7380;
    else
        goto L_737a;

L_737a:
    return 0x1;

L_7380:
    StickyDlgPos(hwnd, 0x416, 0x0);
    if ((wParam != 0x2))
        goto L_73a5;
    else
        goto L_739f;

L_739f:
    t_merge_73b8_0001 = 0x0;
    goto L_73b8;

L_73a5:
    if ((wParam != 0x408))
        goto L_73b5;
    else
        goto L_73af;

L_73af:
    t_merge_73b8_0001 = 0xffff;
    goto L_73b8;

L_73b5:
    t_merge_73b8_0001 = 0x1;

L_73b8:
    EndDialog(hwnd, t_merge_73b8_0001);
    if ((wParam != 0x408))
        goto L_73d0;
    else
        goto L_73c8;

L_73c8:
    EnsureAis();
    goto L_73f4;

L_73d0:
    if ((wParam != 0x2))
        goto L_73f4;
    else
        goto L_73d9;

L_73d9:
    if ((gd.fClose == 0x0))
        goto L_73f4;
    else
        goto L_73eb;

L_73eb:
    PostQuitMessage(vretExitValue);

L_73f4:
    return 0x1;

L_73fd:
    if ((wParam != 0x7df))
        goto L_7467;
    else
        goto L_7407;

L_7407:
    if ((FCheckPassword() == 0x0))
        goto L_75c0;
    else
        goto L_7411;

L_7411:

L_7417:
    lpProc = MakeProcInstance(NewPasswordDlg, hInst);
    fRet = DialogBox(hInst, IDD_NEW_PASSWORD, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    SetFocus(hwnd);
    return fRet;

L_7467:
    if ((wParam != 0x405))
        goto L_7528;
    else
        goto L_7471;

L_7471:
    lpProc = MakeProcInstance(HostOptionsDialog, hInst);
    fRet = DialogBox(hInst, IDD_HOST_OPTIONS, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    SetFocus(hwnd);
    if ((fRet == 0x0))
        goto L_751f;
    else
        goto L_74c1;

L_74c1:
    if ((gd.fReadOnly != 0x0))
        goto L_751f;
    else
        goto L_74d9;

L_74d9:
    GetDlgItem(hwnd, 0x408);
    if ((gd.fAllAis != 0x0))
        goto L_7516;
    else
        goto L_74fc;

L_74fc:
    if ((vtimer.fAutoGenWhenIn != 0x0))
        goto L_7510;
    else
        goto L_7506;

L_7506:
    if ((vtimer.mdForce == 0x0))
        goto L_7516;
    else
        goto L_7510;

L_7510:
    t_merge_7519_0001 = 0x1;
    goto L_7519;

L_7516:
    t_merge_7519_0001 = 0x0;

L_7519:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_7519_0001) -> callresult(int16_t) */

L_751f:
    return fRet;

L_7528:
    if ((wParam != 0x76))
        goto L_75c0;
    else
        goto L_7531;

L_7531:
    WinHelp(hwnd, szHelpFile, 0x1, 0x440);
    return 0x1;

L_7555:
    KillTimer(hwnd, uTimerId);
    uTimerId = 0x0;
    goto L_75c0;

L_756d:
    if ((message == WM_DESTROY))
        goto L_7555;
    else
        goto L_7575;

L_7575:
    if ((message == WM_PAINT))
        goto L_71b9;
    else
        goto L_757d;

L_757d:
    if ((message == WM_ERASEBKGND))
        goto L_6d78;
    else
        goto L_7585;

L_7585:
    if ((message == WM_CTLCOLOR))
        goto L_6da0;
    else
        goto L_758d;

L_758d:
    if ((message == WM_SETCURSOR))
        goto L_6db6;
    else
        goto L_7595;

L_7595:
    if ((message == WM_INITDIALOG))
        goto L_6c25;
    else
        goto L_759d;

L_759d:
    if ((message == WM_COMMAND))
        goto L_7265;
    else
        goto L_75a5;

L_75a5:
    if ((message == WM_TIMER))
        goto L_6d35;
    else
        goto L_75ad;

L_75ad:
    if ((message == WM_LBUTTONDOWN))
        goto L_6db6;
    else
        goto L_75b5;

L_75b5:
    if ((message == WM_RBUTTONDOWN))
        goto L_6db6;
    else
        goto L_75c0;

L_75c0:
    return 0x0;
}

int16_t HostOptionsDialog(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    uint16_t    t_merge_767e_0001;

L_75ce:
    goto L_76c0;

L_75dd:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_7605:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_761b:
    hdc = BeginPaint(hwnd, &(ps));
    DrawHostOptions(hwnd, hdc, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_7657:
    if ((wParam == 0x2))
        goto L_7669;
    else
        goto L_7660;

L_7660:
    if ((wParam != 0x1))
        goto L_768d;
    else
        goto L_7669;

L_7669:
    if ((wParam != 0x1))
        goto L_767b;
    else
        goto L_7675;

L_7675:
    t_merge_767e_0001 = 0x1;
    goto L_767e;

L_767b:
    t_merge_767e_0001 = 0x0;

L_767e:
    EndDialog(hwnd, t_merge_767e_0001);
    return 0x1;

L_768d:
    if ((wParam != 0x76))
        goto L_76f3;
    else
        goto L_7696;

L_7696:
    WinHelp(hwnd, szHelpFile, 0x1, 0x440);
    return 0x1;

L_76c0:
    if ((message == WM_DESTROY))
        goto L_76f3;
    else
        goto L_76c8;

L_76c8:
    if ((message == WM_PAINT))
        goto L_761b;
    else
        goto L_76d0;

L_76d0:
    if ((message == WM_ERASEBKGND))
        goto L_75dd;
    else
        goto L_76d8;

L_76d8:
    if ((message == WM_CTLCOLOR))
        goto L_7605;
    else
        goto L_76e0;

L_76e0:
    if ((message == WM_INITDIALOG))
        goto L_75dd;
    else
        goto L_76e8;

L_76e8:
    if ((message == WM_COMMAND))
        goto L_7657;
    else
        goto L_76f3;

L_76f3:
    return 0x0;
}

void DrawHostOptions(HWND hwnd, HDC hdc, int16_t iDraw) {
L_7706:
    return;
}

void HostTimerProc(HWND hwnd, WMType msg, uint16_t idTimer, uint32_t dwTime) {
    HWND    hwndT;
    char    szExt[4];
    int16_t cOut;
    int16_t fSav;
    int16_t idCur;

L_7716:
    if ((fProcessingTimer != 0x0))
        goto L_79a3;
    else
        goto L_7726;

L_7726:

L_772c:
    fProcessingTimer = 0x1;
    fSav = fFileErrSilent;
    if ((uTimerType != 0xe))
        goto L_7830;
    else
        goto L_7742;

L_7742:
    if ((FNewTurnAvail(idPlayer) == 0x0))
        goto Done;
    else
        goto L_7756;

L_7756:
    idCur = idPlayer;
    KillTimer(hwnd, uTimerId);
    _wsprintf(&(szExt), MPCTD, (idPlayer + 0x1));
    DestroyCurGame();
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_77c6;
    else
        goto L_77a2;

L_77a2:
    AlertSz(PszFormatIds(idsUnableOpenNewTurnFile, 0x0), MB_ICONHAND);
    goto Done;

L_77c6:
    idPlayer = idCur;
    CreateChildWindows();
    uTimerId = SetTimer(0x0, 0xf, 0x3e8, lpfnHostTimerProc);
    uTimerType = 0xf;
    FlashWindow(hwndFrame, 0x1);
    SetWindowText(hwndFrame, PszGetCompressedString(idsNewTurnAvailable2));
    MessageBeep(0x30);
    cOut = 0x1;
    goto RedrawText;

L_7830:
    if ((uTimerType != 0xf))
        goto Loop;
    else
        goto L_783a;

L_783a:
    FlashWindow(hwndFrame, 0x1);
    goto Done;

Loop:
    cOut = CFindTurnsOutstanding();
    if ((gd.fAllAis == 0x0))
        goto L_78a2;
    else
        goto L_7868;

L_7868:
    AlertSz(PszFormatIds(idsAutoGenerateDisabledBecauseHumanPlayersDead, 0x0), MB_ICONHAND);
    EnableWindow(GetDlgItem(hwnd, 0x408), 0x0);
    goto Done;

L_78a2:
    _wsprintf(szWork, PszGetCompressedString(idsHostModeDPlayer), cOut);
    if ((cOut == 0x1))
        goto L_78dd;
    else
        goto L_78cd;

L_78cd:
    strcat(0x57a4, 0x421);

L_78dd:
    strcat(0x57a4, PszGetCompressedString(idsOut));
    SetWindowText(hwndFrame, szWork);

RedrawText:
    hwndT = GetWindow(hwndFrame, GW_HWNDPREV);
    if ((GetWindow(hwndT, GW_OWNER) != hwndFrame))
        goto L_793f;
    else
        goto L_792b;

L_792b:
    InvalidateRect(hwndT, 0x0, 0x1);

L_793f:
    if ((cOut != 0x0))
        goto Done;
    else
        goto L_7948;

L_7948:
    if ((gd.fProgressTxt == 0x0))
        goto L_7960;
    else
        goto L_795b;

L_795b:
    ShowProgressGauge();

L_7960:
    EnsureAis();
    FGenerateTurn();
    HideProgressGauge();
    if ((ini.fGen == 0x0))
        goto L_798f;
    else
        goto L_7983;

L_7983:
    PostQuitMessage(vretExitValue);
    goto Done;

L_798f:
    EnsureAis();
    goto Loop;

Done:
    fProcessingTimer = 0x0;
    fFileErrSilent = fSav;

L_79a3:
    return;
}

void GetWindowRc(HWND hwnd, RECT *prc) {
    WINDOWPLACEMENT wndpl;

L_79ac:
    wndpl.length = 0x16;
    GetWindowPlacement(hwnd, &(wndpl));
    *(prc) = wndpl.rcNormalPosition;
    prc->right = (prc->right - prc->left);
    prc->bottom = (prc->bottom - prc->top);
    return;
}

void SetWindowIniString(char *sz, HWND hwnd) {
    char ch;
    RECT rc;

L_79f6:
    if ((IsZoomed(hwnd) == 0x0))
        goto L_7a16;
    else
        goto L_7a0f;

L_7a0f:
    ch = 0x4d;
    goto L_7a31;

L_7a16:
    if ((IsIconic(hwnd) == 0x0))
        goto L_7a2d;
    else
        goto L_7a26;

L_7a26:
    ch = 0x49;
    goto L_7a31;

L_7a2d:
    ch = 0x52;

L_7a31:
    GetWindowRc(hwnd, &(rc));
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), (uint16_t)(ch), rc.left, rc.top, rc.right, rc.bottom);
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
    uint16_t t_merge_806e_0001;
    uint16_t t_merge_844b_0001;

L_7a76:
    /* untranslated: part[0:1](szPd) = 0x25 */
    /* untranslated: part[1:1](szPd) = 0x64 */
    /* untranslated: part[2:1](szPd) = 0x0 */
    CchGetString(idsWindows, szSection);
    CchGetString(idsStarsIni, szIniFile);
    CchGetString(idsGlobalsettings, szEntry);
    FormatSerialAndEnv(vSerialNumber, 0x519e, 0x57a4);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsResolution, szEntry);
    i = 0x0;
    if ((vcScreenColors > 0x4))
        goto L_7b17;
    else
        goto L_7b13;

L_7b13:
    i = (i | 0x1);

L_7b17:
    if ((gd.mdScreenSize != 0x0))
        goto L_7b2e;
    else
        goto L_7b2a;

L_7b2a:
    i = (i | 0x2);

L_7b2e:
    _wsprintf(szWork, &(szPd), i);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsMain, szEntry);
    SetWindowIniString(0x57a4, hwndFrame);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportfleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptFleet.ptDlg.x, vrptFleet.ptDlg.y, (vrptFleet.ptDlg.x + vrptFleet.ptSize.x),
              (vrptFleet.ptDlg.y + vrptFleet.ptSize.y));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportefleetwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptEFleet.ptDlg.x, vrptEFleet.ptDlg.y, (vrptEFleet.ptDlg.x + vrptEFleet.ptSize.x),
              (vrptEFleet.ptDlg.y + vrptEFleet.ptSize.y));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportbtlwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptBattle.ptDlg.x, vrptBattle.ptDlg.y, (vrptBattle.ptDlg.x + vrptBattle.ptSize.x),
              (vrptBattle.ptDlg.y + vrptBattle.ptSize.y));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportplanwin, szEntry);
    _wsprintf(szWork, PszGetCompressedString(idsC04d04d04d04d), 0x4d, vrptPlanet.ptDlg.x, vrptPlanet.ptDlg.y, (vrptPlanet.ptDlg.x + vrptPlanet.ptSize.x),
              (vrptPlanet.ptDlg.y + vrptPlanet.ptSize.y));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsLayout, szEntry);
    _wsprintf(szWork, &(szPd), iWindowLayout);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle1width, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dxPlanWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle1height, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dyMsgWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle1height2, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dyMinWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle2width, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dx2PlanWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle2height, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dy2MsgWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsStyle2height2, szEntry);
    _wsprintf(szWork, &(szPd), vfs.dy2MinWant);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsToolbar, szEntry);
    _wsprintf(szWork, &(szPd), gd.fToolbar);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    iPass = 0x2;
    rgtile = 0x7fc;
    ctile = 0x6;
    CchGetString(idsPlanettiles, szEntry);

L_7fd8:
    t_7fd8 = iPass;
    iPass = (iPass - 0x1);
    if ((t_7fd8 == 0x0))
        goto L_80db;
    else
        goto L_7fe7;

L_7fe7:
    psz = 0x57a4;
    iCol = 0x0;
    i = 0x0;
    goto L_8008;

L_7ffa:
    i = (i + 0x1);
    psz = (psz + 0x1);

L_8008:
    if ((i >= ctile))
        goto L_8097;
    else
        goto L_8013;

L_8013:
    /* untranslated: branch (part[10:2](rgtile[i]) & 0x7) <= iCol ? L_8042 : L_8031 */

L_8031:
    iCol = (iCol + 0x1);
    *(psz) = 0x2a;
    psz = (psz + 0x1);
    goto L_8013;

L_8042:
    /* untranslated: branch ((part[10:2](rgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_806b : L_8065 */

L_8065:
    t_merge_806e_0001 = 0x41;
    goto L_806e;

L_806b:
    t_merge_806e_0001 = 0x61;

L_806e:
    *(psz) = LOBYTE(t_merge_806e_0001);
    /* untranslated: *psz = (*psz + lobyte(((part[10:2](rgtile[i]) >> 0x3) & 0xf))) */
    goto L_7ffa;

L_8097:
    *(psz) = 0x0;
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsShiptiles, szEntry);
    rgtile = 0x90e;
    ctile = 0x7;
    goto L_7fd8;

L_80db:
    CchGetString(idsSelection, szEntry);
    goto L_8110;

L_80f1:
    ch = 0x4e;
    goto L_8133;

L_80f8:
    ch = 0x50;
    goto L_8133;

L_80ff:
    ch = 0x53;
    goto L_8133;

L_8106:
    ch = 0x45;
    goto L_8133;

L_8110:
    if ((sel.grobj == grobjNone))
        goto L_80f1;
    else
        goto L_8118;

L_8118:
    if ((sel.grobj == grobjPlanet))
        goto L_80f8;
    else
        goto L_8120;

L_8120:
    if ((sel.grobj == grobjFleet))
        goto L_80ff;
    else
        goto L_8128;

L_8128:
    if ((sel.grobj == grobjOther))
        goto L_8106;
    else
        goto L_8133;

L_8133:
    _wsprintf(szWork, PszGetCompressedString(idsCCD), (uint16_t)(ch), (uint16_t)(LOBYTE((idPlayer + 0x42))), sel.id);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsMessage, szEntry);
    _wsprintf(szWork, PCTD, iMsgCur);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsGameid, szEntry);
    _wsprintf(szWork, "%lx", LOWORD(game), HIWORD(game));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsScanzoom, szEntry);
    LOWORD(szWork) = LOBYTE((iScanZoom + 0x35));
    /* untranslated: part[1:1](szWork) = 0x0 */
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    if ((gd.fChgScanner == 0x0))
        goto L_83c9;
    else
        goto L_8270;

L_8270:
    itoa(grbitScan, 0x57a4, 0xa);
    CchGetString(idsScanmodev25, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    itoa(grbitScanShip, 0x57a4, 0xa);
    CchGetString(idsScanfilterv25, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    itoa(grbitScanEShip, 0x57a4, 0xa);
    CchGetString(idsScanefilterv25, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    itoa(grbitScanMines, 0x57a4, 0xa);
    CchGetString(idsScanmines, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    itoa(vpctRadarView, 0x57a4, 0xa);
    CchGetString(idsScanradar, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));

L_83c9:
    itoa(cMinGrafMax, 0x57a4, 0xa);
    CchGetString(idsMineralscale, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    if ((idPlayer == 0xffff))
        goto L_852b;
    else
        goto L_8418;

L_8418:
    CchGetString(idsFiles, szSection);
    CchGetString(idsWait2, szEntry);
    if ((uTimerId != 0x0))
        goto L_8448;
    else
        goto L_8442;

L_8442:
    t_merge_844b_0001 = 0x0;
    goto L_844b;

L_8448:
    t_merge_844b_0001 = 0x1;

L_844b:
    LOWORD(szWork) = LOBYTE((t_merge_844b_0001 + 0x30));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    if ((gd.fWriteTurnNum == 0x0))
        goto L_84d6;
    else
        goto L_8485;

L_8485:
    itoa(game.turn, 0x57a4, 0xa);
    CchGetString(idsTurn, szEntry);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    gd.fWriteTurnNum = 0x0;

L_84d6:
    CchGetString(idsFile1, szEntry);
    _wsprintf(szWork, "%s.m%d", 0x56a2, 0x25, (idPlayer + 0x1));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));

L_852b:
    CchGetString(idsMisc, szSection);
    if ((gd.fChgReports == 0x0))
        goto L_8854;
    else
        goto L_854e;

L_854e:
    CchGetString(idsReportplanfld, szEntry);
    _wsprintf(szWork, PCTD, LOWORD(vrptPlanet));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportplansort, szEntry);
    i = vrptPlanet.icolSort;
    if ((vrptPlanet.fAscending == 0x0))
        goto L_85c2;
    else
        goto L_85bd;

L_85bd:
    i = (i | 0x100);

L_85c2:
    _wsprintf(szWork, PCTD, i);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportfleetfld, szEntry);
    _wsprintf(szWork, PCTD, LOWORD(vrptFleet));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportfleetsort, szEntry);
    i = vrptFleet.icolSort;
    if ((vrptFleet.fAscending == 0x0))
        goto L_8670;
    else
        goto L_866b;

L_866b:
    i = (i | 0x100);

L_8670:
    _wsprintf(szWork, PCTD, i);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportefleetfld, szEntry);
    _wsprintf(szWork, PCTD, LOWORD(vrptEFleet));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportefltsort, szEntry);
    i = vrptEFleet.icolSort;
    if ((vrptEFleet.fAscending == 0x0))
        goto L_871e;
    else
        goto L_8719;

L_8719:
    i = (i | 0x100);

L_871e:
    _wsprintf(szWork, PCTD, i);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportbtlfld, szEntry);
    _wsprintf(szWork, PCTD, LOWORD(vrptBattle));
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportbtlsort, szEntry);
    i = vrptBattle.icolSort;
    if ((vrptBattle.fAscending == 0x0))
        goto L_87cc;
    else
        goto L_87c7;

L_87c7:
    i = (i | 0x100);

L_87cc:
    _wsprintf(szWork, PCTD, i);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsReportdefgraph, szEntry);
    _wsprintf(szWork, PCTD, gd.iCurGraph);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));

L_8854:
    CchGetString(idsHistoryinfo, szEntry);
    _wsprintf(szWork, PCTD, uDateInstalled);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    CchGetString(idsVcrspeed, szEntry);
    _wsprintf(szWork, PCTD, viSpeedVCR);
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    if ((gd.fChgZipOrd == 0x0))
        goto L_8a86;
    else
        goto L_8900;

L_8900:
    CchGetString(idsZiporders, szSection);
    i = 0x0;
    goto L_8a7d;

L_8918:
    strcpy(szEntry, szSection);
    psz = szEntry[strlen(szEntry)];
    *(psz) = LOBYTE((i + 0x31));
    psz[0x1] = 0x0;
    if ((vrgZip[i].fValid == 0x0))
        goto L_8a53;
    else
        goto L_8967;

L_8967:
    psz = 0x57a4;
    iPass = 0x0;
    goto L_8a2b;

L_8974:
    psz = (psz + 0x1);
    *(psz) = LOBYTE(((((vrgZip[i].txp.rgia[iPass] >> 0xc) & 0xf) & 0xff) + 0x61));
    psz = (psz + 0x1);
    *(psz) = LOBYTE(((((vrgZip[i].txp.rgia[iPass] & 0xfff) & 0xf) & 0xff) + 0x61));
    psz = (psz + 0x1);
    *(psz) = LOBYTE((((((vrgZip[i].txp.rgia[iPass] & 0xfff) >> 0x4) & 0xf) & 0xff) + 0x61));
    psz = (psz + 0x1);
    *(psz) = LOBYTE((((((vrgZip[i].txp.rgia[iPass] & 0xfff) >> 0x8) & 0xf) & 0xff) + 0x61));
    iPass = (iPass + 0x1);

L_8a2b:
    if ((iPass < 0x5))
        goto L_8974;
    else
        goto L_8a34;

L_8a34:
    /* untranslated: call strcpy(psz, part[10:0](vrgZip[i])) -> callresult(char *) */
    goto L_8a58;

L_8a53:
    /* untranslated: part[0:1](szWork) = 0x0 */

L_8a58:
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    i = (i + 0x1);

L_8a7d:
    if ((i < 0x4))
        goto L_8918;
    else
        goto L_8a86;

L_8a86:
    if ((gd.fChgZipProd == 0x0))
        goto L_8c81;
    else
        goto L_8a9e;

L_8a9e:
    i = 0x0;
    goto L_8c78;

L_8aa6:
    CchGetString(idsZiporders, szSection);
    strcpy(szEntry, szSection);
    psz = szEntry[strlen(szEntry)];
    psz = (psz + 0x1);
    *(psz) = 0x50;
    *(psz) = LOBYTE((i + 0x31));
    psz[0x1] = 0x0;
    if ((vrgZipProd[i].fValid == 0x0))
        goto L_8c4e;
    else
        goto L_8b0f;

L_8b0f:
    psz = 0x57a4;
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte((part[14:1](vrgZipProd[i]) + 0x61)) */
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte((part[15:1](vrgZipProd[i]) + 0x61)) */
    iPass = 0x0;
    goto L_8b5a;

L_8b56:
    iPass = (iPass + 0x1);

L_8b5a:
    /* untranslated: branch iPass >= part[15:1](vrgZipProd[i]) ? L_8c34 : L_8b73 */

L_8b73:
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte((((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] & 0xf) & 0xff) + 0x61)) */
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte(((((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] >> 0x4) & 0xf) & 0xff) + 0x61)) */
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte(((((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] >> 0x8) & 0xf) & 0xff) + 0x61)) */
    psz = (psz + 0x1);
    /* untranslated: *psz = lobyte(((((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] >> 0xc) & 0xf) & 0xff) + 0x61)) */
    goto L_8b56;

L_8c34:
    strcpy(psz, vrgZipProd[i]);
    goto L_8c53;

L_8c4e:
    /* untranslated: part[0:1](szWork) = 0x0 */

L_8c53:
    WritePrivateProfileString(&(szSection), &(szEntry), szWork, &(szIniFile));
    i = (i + 0x1);

L_8c78:
    if ((i < 0x5))
        goto L_8aa6;
    else
        goto L_8c81;

L_8c81:
    return;
}

void RefitFrameChildren() {
    int16_t  dyMsg;
    HMENU    hmenu;
    int16_t  i;
    int16_t  dyMinMin;
    int16_t  dyMsgMin;
    int16_t  dyMin;
    int16_t  dyT;
    int16_t  dyTot;
    int16_t  yScanner;
    int16_t  t_merge_8cea_0001;
    int16_t  t_merge_8d1e_0001;
    int16_t  t_merge_8d1e_0002;
    int16_t  t_merge_8d39_0001;
    int16_t  t_merge_8d39_0002;
    int16_t  t_merge_8f06_0001;
    int16_t  t_merge_8f3a_0001;
    int16_t  t_merge_8f3a_0002;
    int16_t  t_merge_8f55_0001;
    int16_t  t_merge_8f55_0002;
    uint16_t t_merge_910b_0001;

L_8c88:
    if ((hwndFrame == 0x0))
        goto L_911f;
    else
        goto L_8c9b;

L_8c9b:
    if ((IsIconic(hwndFrame) != 0x0))
        goto L_911f;
    else
        goto L_8ca9;

L_8ca9:

L_8caf:
    goto L_90a7;

L_8cb5:
    if (((vfs.dx - vfs.dxPlanWant) >= 0x64))
        goto L_8cd0;
    else
        goto L_8cc4;

L_8cc4:
    vfs.xTop = (vfs.dx + 0xff9c);
    goto L_8cd6;

L_8cd0:
    vfs.xTop = vfs.dxPlanWant;

L_8cd6:
    if ((vfs.xTop <= 0xc6))
        goto L_8ce7;
    else
        goto L_8ce1;

L_8ce1:
    t_merge_8cea_0001 = vfs.xTop;
    goto L_8cea;

L_8ce7:
    t_merge_8cea_0001 = 0xc6;

L_8cea:
    vfs.xTop = t_merge_8cea_0001;
    dyMsgMin = ((LOWORD((0xd * dyArial8)) >> 0x1) + 0xa);
    dyMinMin = (LOWORD((0xd * dyArial8)) + 0xffdc);
    if ((vfs.dyMsgWant <= dyMsgMin))
        goto L_8d1b;
    else
        goto L_8d15;

L_8d15:
    t_merge_8d1e_0001 = vfs.dyMsgWant;
    t_merge_8d1e_0002 = vfs.dyMsgWant;
    goto L_8d1e;

L_8d1b:
    t_merge_8d1e_0001 = dyMsgMin;
    t_merge_8d1e_0002 = dyMsgMin;

L_8d1e:
    dyMsg = t_merge_8d1e_0001;
    vfs.dyMsgWant = t_merge_8d1e_0002;
    if ((vfs.dyMinWant <= dyMinMin))
        goto L_8d36;
    else
        goto L_8d30;

L_8d30:
    t_merge_8d39_0001 = vfs.dyMinWant;
    t_merge_8d39_0002 = vfs.dyMinWant;
    goto L_8d39;

L_8d36:
    t_merge_8d39_0001 = dyMinMin;
    t_merge_8d39_0002 = dyMinMin;

L_8d39:
    dyMin = t_merge_8d39_0001;
    vfs.dyMinWant = t_merge_8d39_0002;
    if (((vfs.dy - ((dyMsg + dyMin) + 0x10)) >= 0x32))
        goto L_8dc1;
    else
        goto L_8d56;

L_8d56:
    dyT = (vfs.dy + 0xffbe);
    dyTot = (dyMsg + dyMin);
    dyMsg = MulDiv(dyT, dyMsg, dyTot);
    dyMin = MulDiv(dyT, dyMin, dyTot);
    if ((dyMsg >= dyMsgMin))
        goto L_8da7;
    else
        goto L_8d95;

L_8d95:
    dyMin = (dyMin - (dyMsgMin - dyMsg));
    dyMsg = dyMsgMin;
    goto L_8dc1;

L_8da7:
    if ((dyMin >= dyMinMin))
        goto L_8dc1;
    else
        goto L_8db2;

L_8db2:
    dyMsg = (dyMsg - (dyMinMin - dyMin));
    dyMin = dyMinMin;

L_8dc1:
    vfs.y1 = (((vfs.dy - dyMin) - dyMsg) + 0xfff0);
    vfs.y2 = ((vfs.y1 + dyMsg) + 0x8);
    if ((hwndScanner == 0x0))
        goto L_90c2;
    else
        goto L_8de6;

L_8de6:
    if ((gd.fToolbar == 0x0))
        goto L_8e28;
    else
        goto L_8df9;

L_8df9:
    MoveWindow(hwndTb, (vfs.xTop + 0x8), 0x0, ((vfs.dx - vfs.xTop) + 0xfff8), 0x24, 0x1);
    yScanner = 0x24;
    goto L_8e4a;

L_8e28:
    MoveWindow(hwndTb, 0x0, 0xff9c, 0x32, 0x32, 0x1);
    yScanner = 0x0;

L_8e4a:
    MoveWindow(hwndScanner, (vfs.xTop + 0x8), yScanner, ((vfs.dx - vfs.xTop) + 0xfff8), (vfs.dy - yScanner), 0x1);
    MoveWindow(hwndPlanet, 0x0, 0x0, vfs.xTop, vfs.y1, 0x1);
    MoveWindow(hwndMessage, 0x0, (vfs.y1 + 0x8), vfs.xTop, dyMsg, 0x1);
    MoveWindow(hwndMine, 0x0, (vfs.y2 + 0x8), vfs.xTop, dyMin, 0x1);

L_8ed1:
    if (((vfs.dx - vfs.dx2PlanWant) >= 0xc8))
        goto L_8eec;
    else
        goto L_8ee0;

L_8ee0:
    vfs.xTop = (vfs.dx + 0xff38);
    goto L_8ef2;

L_8eec:
    vfs.xTop = vfs.dx2PlanWant;

L_8ef2:
    if ((vfs.xTop <= 0xc6))
        goto L_8f03;
    else
        goto L_8efd;

L_8efd:
    t_merge_8f06_0001 = vfs.xTop;
    goto L_8f06;

L_8f03:
    t_merge_8f06_0001 = 0xc6;

L_8f06:
    vfs.xTop = t_merge_8f06_0001;
    dyMsgMin = ((LOWORD((0xd * dyArial8)) >> 0x1) + 0xa);
    dyMinMin = (LOWORD((0xd * dyArial8)) + 0xffdc);
    if ((vfs.dy2MsgWant <= dyMsgMin))
        goto L_8f37;
    else
        goto L_8f31;

L_8f31:
    t_merge_8f3a_0001 = vfs.dy2MsgWant;
    t_merge_8f3a_0002 = vfs.dy2MsgWant;
    goto L_8f3a;

L_8f37:
    t_merge_8f3a_0001 = dyMsgMin;
    t_merge_8f3a_0002 = dyMsgMin;

L_8f3a:
    dyMsg = t_merge_8f3a_0001;
    vfs.dy2MsgWant = t_merge_8f3a_0002;
    if ((vfs.dy2MinWant <= dyMinMin))
        goto L_8f52;
    else
        goto L_8f4c;

L_8f4c:
    t_merge_8f55_0001 = vfs.dy2MinWant;
    t_merge_8f55_0002 = vfs.dy2MinWant;
    goto L_8f55;

L_8f52:
    t_merge_8f55_0001 = dyMinMin;
    t_merge_8f55_0002 = dyMinMin;

L_8f55:
    dyMin = t_merge_8f55_0001;
    vfs.dy2MinWant = t_merge_8f55_0002;
    if (((vfs.dy - (dyMsg + 0x8)) >= 0x64))
        goto L_8f78;
    else
        goto L_8f6f;

L_8f6f:
    dyMsg = (vfs.dy + 0xff94);

L_8f78:
    if (((vfs.dy - (dyMin + 0x8)) >= 0x64))
        goto L_8f95;
    else
        goto L_8f8c;

L_8f8c:
    dyMin = (vfs.dy + 0xff94);

L_8f95:
    vfs.y1 = ((vfs.dy - dyMsg) + 0xfff8);
    vfs.y2 = ((vfs.dy - dyMin) + 0xfff8);
    if ((hwndScanner == 0x0))
        goto L_90c2;
    else
        goto L_8fb7;

L_8fb7:
    if ((gd.fToolbar == 0x0))
        goto L_8fef;
    else
        goto L_8fca;

L_8fca:
    MoveWindow(hwndTb, 0x0, 0x0, vfs.dx, 0x24, 0x1);
    yScanner = 0x24;
    goto L_9011;

L_8fef:
    MoveWindow(hwndTb, 0x0, 0xff9c, 0x32, 0x32, 0x1);
    yScanner = 0x0;

L_9011:
    MoveWindow(hwndScanner, (vfs.xTop + 0x8), yScanner, ((vfs.dx - vfs.xTop) + 0xfff8), (vfs.y2 - yScanner), 0x1);
    MoveWindow(hwndPlanet, 0x0, yScanner, vfs.xTop, (vfs.y1 - yScanner), 0x1);
    MoveWindow(hwndMessage, 0x0, (vfs.y1 + 0x8), vfs.xTop, dyMsg, 0x1);
    MoveWindow(hwndMine, (vfs.xTop + 0x8), (vfs.y2 + 0x8), ((vfs.dx - vfs.xTop) + 0xfff8), dyMin, 0x1);

L_90a7:
    if ((iWindowLayout == 0x0))
        goto L_8cb5;
    else
        goto L_90af;

L_90af:
    if ((iWindowLayout == 0x1))
        goto L_8ed1;
    else
        goto L_90b7;

L_90b7:
    if ((iWindowLayout != 0x2))
        goto L_8cb5;
    else
        goto L_90bc;

L_90bc:

L_90c2:
    hmenu = GetASubMenu(hwndFrame, 0x1);
    hmenu = GetSubMenu(hmenu, 0x4);
    i = 0x82;
    goto L_9115;

L_90ec:
    if (((i + 0xff7e) != iWindowLayout))
        goto L_9108;
    else
        goto L_9102;

L_9102:
    t_merge_910b_0001 = 0x8;
    goto L_910b;

L_9108:
    t_merge_910b_0001 = 0x0;

L_910b:
    CheckMenuItem(hmenu, i, t_merge_910b_0001);
    i = (i + 0x1);

L_9115:
    if ((i <= 0x84))
        goto L_90ec;
    else
        goto L_911f;

L_911f:
    return;
}

int32_t TitleWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
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
    uint16_t    t_merge_91a2_0001;
    uint16_t    t_merge_91f5_0001;

L_9126:
    goto L_97fc;

L_9135:
    if ((vcScreenColors < 0x8))
        goto L_9171;
    else
        goto L_913f;

L_913f:
    vhdibTitle = HdibLoadBigResource(0x1c1);
    if ((vhpalSplash != 0x0))
        goto L_9171;
    else
        goto L_9158;

L_9158:
    if ((vhdibTitle == 0x0))
        goto L_9171;
    else
        goto L_9162;

L_9162:
    vhpalSplash = HpalFromDib(vhdibTitle);

L_9171:
    GetClientRect(hwnd, &(rc));
    if ((0x78 <= (rc.right >> 0x3)))
        goto L_9199;
    else
        goto L_9193;

L_9193:
    t_merge_91a2_0001 = 0x78;
    goto L_91a2;

L_9199:
    t_merge_91a2_0001 = (rc.right >> 0x3);

L_91a2:
    dx = t_merge_91a2_0001;
    if ((rc.bottom >= 0x28a))
        goto L_91bb;
    else
        goto L_91af;

L_91af:
    dx = (dx + ((uint32_t)(dx) / 0x6));

L_91bb:
    dxGap = ((rc.right - (dx << 0x2)) >> 0x2);
    xCur = (dxGap >> 0x1);
    if ((rc.bottom <= 0x1f4))
        goto L_91f0;
    else
        goto L_91e0;

L_91e0:
    /* untranslated: t_merge_91f5_0001 = (words(loword((0x5 * dyArial8)), signhiword(loword((0x5 * dyArial8)))) / 0x2) */
    goto L_91f5;

L_91f0:
    t_merge_91f5_0001 = (dyArial8 * 0x2);

L_91f5:
    dy = t_merge_91f5_0001;
    i = 0x0;
    goto L_92d7;

L_9200:
    /* untranslated: psz = PszGetCompressedString(part[9:0](szFrame[i])) */
    /* untranslated: rghwndBtnSplash[i] = CreateWindow("BUTTON", psz, 0x50000000, xCur, ((rc.bottom - dy) - (words(loword((0x5 * dyArial8)),
     * signhiword(loword((0x5 * dyArial8)))) / 0x2)), dx, dy, hwnd, i, hInst, 0x0) */
    if ((i != 0x2))
        goto L_92a2;
    else
        goto L_9271;

L_9271:
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_9295;
    else
        goto L_927d;

L_927d:
    if ((access(0x56a2, 0x0) != 0xffff))
        goto L_92a2;
    else
        goto L_9295;

L_9295:
    /* untranslated: call EnableWindow(part[4:2](rghwndBtnSplash), 0x0) -> callresult(int16_t) */

L_92a2:
    if ((rc.bottom >= 0x1f4))
        goto L_92ca;
    else
        goto L_92ac;

L_92ac:
    SendMessage(rghwndBtnSplash[i], WM_SETFONT, HIWORD(rghfontArial8), 0x0);

L_92ca:
    xCur = (xCur + (dx + dxGap));
    i = (i + 0x1);

L_92d7:
    if ((i >= 0x4))
        goto L_982f;
    else
        goto L_92dd;

L_92dd:

MapIt:
    if ((vcScreenColors < 0x8))
        goto L_982f;
    else
        goto L_92ea;

L_92ea:

L_92f0:
    hdc = GetDC(hwnd);
    hpalSav = SelectPalette(hdc, vhpalSplash, 0x0);
    i = RealizePalette(hdc);
    SelectPalette(hdc, hpalSav, 0x0);
    ReleaseDC(hwnd, hdc);
    if ((i == 0x0))
        goto L_935c;
    else
        goto L_933c;

L_933c:
    InvalidateRect(hwnd, 0x0, 0x1);
    return 0x10000;

L_935c:
    return 0x0;

L_9368:
    if ((wParam != hwnd))
        goto MapIt;
    else
        goto L_9370;

L_9370:

L_9379:
    if ((vhdibTitle == 0x0))
        goto L_939b;
    else
        goto L_9383;

L_9383:
    GlobalUnlock(vhdibTitle);
    FreeResource(vhdibTitle);
    vhdibTitle = 0x0;

L_939b:
    if ((fFreeingTitle != 0x0))
        goto Default;
    else
        goto L_93a5;

L_93a5:
    if ((gd.fExitWindows == 0x0))
        goto L_93ca;
    else
        goto L_93b8;

L_93b8:
    ExitWindows((uint32_t)(vretExitValue), 0x0);
    goto Default;

L_93ca:
    WriteIniSettings();
    PostQuitMessage(vretExitValue);

L_93db:
    if ((wParam != 0x0))
        goto L_9455;
    else
        goto L_93e4;

L_93e4:
    NewGameWizard(hwnd, 0x0);
    if ((LOWORD(lpPlanets) != 0x0))
        goto L_941b;
    else
        goto L_93fd;

L_93fd:
    if ((HIWORD(lpPlanets) != 0x0))
        goto L_941b;
    else
        goto L_9407;

L_9407:
    if ((LOWORD(game) != 0x0))
        goto L_941b;
    else
        goto L_9411;

L_9411:
    if ((HIWORD(game) == 0x0))
        goto L_944a;
    else
        goto L_941b;

L_941b:
    if ((fFreeingTitle != 0x0))
        goto L_943a;
    else
        goto L_9425;

L_9425:
    fFreeingTitle = 0x1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;

L_943a:
    ShowWindow(hwndFrame, SW_SHOW);
    goto L_982f;

L_944a:
    SetFocus(hwnd);

L_9455:
    if ((wParam != 0x1))
        goto L_9514;
    else
        goto LOpenGame;

LOpenGame:
    if ((FOpenGame(hwnd, 0x0) <= 0x0))
        goto L_94fd;
    else
        goto L_9475;

L_9475:
    if ((fFreeingTitle != 0x0))
        goto L_9494;
    else
        goto L_947f;

L_947f:
    fFreeingTitle = 0x1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;

L_9494:
    if ((idPlayer == 0xffff))
        goto L_94ab;
    else
        goto L_949e;

L_949e:
    ShowWindow(hwndFrame, SW_SHOW);

L_94ab:
    InitializeMenu(0x0);
    PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((game.fTutorial == 0x0))
        goto L_9505;
    else
        goto L_94e4;

L_94e4:
    if ((idPlayer != 0x0))
        goto L_9505;
    else
        goto L_94ee;

L_94ee:
    StartTutor(0x0);

L_94fd:
    SetFocus(hwnd);

L_9505:
    ini.fStartupFile = 0x0;
    goto L_982f;

L_9514:
    if ((wParam != 0x2))
        goto L_952f;
    else
        goto L_951d;

L_951d:
    ini.fStartupFile = 0x1;
    goto LOpenGame;

L_952f:
    if ((wParam != 0x3))
        goto L_982f;
    else
        goto L_9538;

L_9538:
    if ((gd.fExitWindows == 0x0))
        goto L_955d;
    else
        goto L_954b;

L_954b:
    ExitWindows((uint32_t)(vretExitValue), 0x0);
    goto L_982f;

L_955d:
    WriteIniSettings();
    PostQuitMessage(vretExitValue);

L_956e:
    if ((IsIconic(hwnd) == 0x0))
        goto L_95b9;
    else
        goto L_957e;

L_957e:
    hdc = BeginPaint(hwnd, &(ps));
    DrawIcon(hdc, 0x2, 0x2, hiconStars);
    EndPaint(hwnd, &(ps));
    goto L_982f;

L_95b9:
    plf = LocalAlloc(0x40, 0x32);
    hdc = BeginPaint(hwnd, &(ps));
    hbrSav = SelectObject(hdc, hbrButtonFace);
    GetClientRect(hwnd, &(rcWnd));
    if ((vcScreenColors < 0x8))
        goto LTry16Color;
    else
        goto L_9603;

L_9603:
    SetTextColor(hdc, 0x2ff7fff);
    if ((vhdibTitle == 0x0))
        goto LTry16Color;
    else
        goto L_961a;

L_961a:

L_9620:
    SelectPalette(hdc, vhpalSplash, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, 0x0, 0x0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), vhdibTitle, 0x0, 0x0, 0x320, 0x258, 0xcc0020);
    goto L_9752;

LTry16Color:
    rc.left = 0x0;
    rc.right = rcWnd.right;
    rc.bottom = 0x0;
    dx = rcWnd.right;
    DrawABunchOfStars(hdc, &(rcWnd));
    /* untranslated: plf->lfHeight = (sext16to32((rcWnd.bottom neg 0x0)) / 0x3) */
    strcpy((plf + 0x12), 0x706);
    hfont = CreateFontIndirect(plf);
    SetTextColor(hdc, 0x9b009b);
    if ((hfont == 0x0))
        goto L_9752;
    else
        goto L_96f0;

L_96f0:
    hfontSav = SelectObject(hdc, hfont);
    SetBkMode(hdc, TRANSPARENT);
    rcT = rcWnd;
    /* untranslated: rcT.bottom = (words(loword((0x3 * rcT.bottom)), signhiword(loword((0x3 * rcT.bottom)))) / 0x4) */
    RcCtrTextOut(hdc, &(rcT), 0x441, 0x6);
    SelectObject(hdc, hfontSav);
    DeleteObject(hfont);

L_9752:
    SelectObject(hdc, HIWORD(rghfontArial10));
    SetBkMode(hdc, TRANSPARENT);
    SzVersion();
    GetWindowRect(LOWORD(rghwndBtnSplash), &(rcT));
    /* untranslated: rcWnd.top = (rcT.top - (words(loword((0x9 * dyArial8)), signhiword(loword((0x9 * dyArial8)))) / 0x2)) */
    /* untranslated: rcWnd.bottom = ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + rcWnd.top) */
    RcCtrTextOut(hdc, &(rcWnd), 0x57a4, strlen(0x57a4));
    EndPaint(hwnd, &(ps));
    LocalFree(plf);
    goto L_982f;

Default:
    return DefWindowProc(hwnd, msg, wParam, lParam);

L_97fc:
    if ((msg == WM_CREATE))
        goto L_9135;
    else
        goto L_9804;

L_9804:
    if ((msg == WM_DESTROY))
        goto L_9379;
    else
        goto L_980c;

L_980c:
    if ((msg == WM_PAINT))
        goto L_956e;
    else
        goto L_9814;

L_9814:
    if ((msg == WM_COMMAND))
        goto L_93db;
    else
        goto L_981c;

L_981c:
    if ((msg == WM_QUERYNEWPALETTE))
        goto MapIt;
    else
        goto L_9824;

L_9824:
    if ((msg != WM_PALETTECHANGED))
        goto Default;
    else
        goto L_9829;

L_9829:

L_982f:
    return 0x0;
}
