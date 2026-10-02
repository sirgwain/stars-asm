#include "common.h"

uint8_t rgPalGray[20] = {10, 20, 30, 40, 61, 71, 81, 92, 112, 122, 133, 143, 161, 171, 182, 193, 215, 225, 235, 245};

int16_t FCreateStuff() {
    int16_t fFailed;
    int16_t dy;
    int16_t i;
    HBITMAP hbmp;
    int16_t dx;

    fFailed = 0;
    dx = GetSystemMetrics(SM_CXSCREEN);
    dy = GetSystemMetrics(SM_CYSCREEN);
    if (dx < 800 || dy < 600) {
        gd.mdScreenSize = 0;
    } else if (dx < 1024 || dy < 768) {
        gd.mdScreenSize = 1;
    } else if (dx < 1111 || dy < 888) {
        gd.mdScreenSize = 2;
    } else {
        gd.mdScreenSize = 3;
    }
    gd.fNoIdleChecks = 0;
    gd.fAisDone = 0;
    vplr = vrgplrDef[0];
    hrgnHuge = CreateRectRgn(-10, -10, 2000, 2000);
    hrgnScratch = CreateRectRgn(0, 0, 10, 10);
    hbrShip = HbrGet(65280);
    hbrStarbase = HbrGet(0xffff);
    hbrBBlue = HbrGet(16711680);
    hbrEnemy = HbrGet(0xff);
    hbrSelect = HbrGet(0xffff);
    hbrRed = HbrGet(0xff);
    hbrBlue = HbrGet(8323072);
    hbrGreen = HbrGet(32512);
    hbrRadar = HbrGet(127);
    hbrPurple = HbrGet(8323199);
    hbrTooltip = HbrGet(10485759);
    hbrRadarNear = 0;
    rghbrMineral[0] = HbrGet(16711680);
    rghbrMineral[1] = HbrGet(32512);
    rghbrMineral[2] = HbrGet(0xffff);
    rghbrMineral[3] = HbrGet(0xffffff);
    rghbrMineral[4] = HbrGet(0xff);
    rghbrPlanetAttr[0][0] = HbrGet(8323072);
    rghbrPlanetAttr[0][1] = HbrGet(16711680);
    rghbrPlanetAttr[1][0] = HbrGet(127);
    rghbrPlanetAttr[1][1] = HbrGet(0xff);
    rghbrPlanetAttr[2][0] = HbrGet(32512);
    rghbrPlanetAttr[2][1] = HbrGet(65280);
    rghbrMinSum[0][0] = HbrGet(16711680);
    rghbrMinSum[0][1] = HbrGet(8323072);
    rghbrMinSum[1][0] = HbrGet(65280);
    rghbrMinSum[1][1] = HbrGet(32512);
    rghbrMinSum[2][0] = HbrGet(0xffff);
    rghbrMinSum[2][1] = HbrGet(32639);
    rghbrMinSum[3][0] = HbrGet(0xff);
    rghbrMinSum[3][1] = HbrGet(127);
    hbrYellow = HbrGet(0xffff);
    hbrDkYellow = HbrGet(32639);
    hbrLightGray = HbrGet(12632256);
    hbrGray = HbrGet(8421504);
    hpenShip = CreatePen(0, 1, 65280);
    hpenDkGreen = CreatePen(0, 1, 32512);
    hpenStarbase = CreatePen(0, 1, 16711680);
    hpenEnemy = CreatePen(0, 1, 0xff);
    hpenMassPath = CreatePen(2, 1, 8355711);
    hpenRadar = CreatePen(0, 1, 127);
    hpenRadarNear = 0;
    hpenDkBlue = CreatePen(0, 1, 8323072);
    hpenYellow = CreatePen(0, 1, 0xffff);
    hpenDkYellow = CreatePen(0, 1, 32639);
    hpenDkPurple = CreatePen(0, 1, 8323199);
    hbmp = LoadBitmap(hInst, "Screen50Bmp");
    hbr50Screen = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    for (i = 0; i < 3; i++) {
        hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(i + 460));
        rghbrPat[i] = CreatePatternBrush(hbmp);
        DeleteObject(hbmp);
    }
    hbmp = LoadBitmap(hInst, "CargoBmp");
    hbrCargo = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    hbmp = LoadBitmap(hInst, "DockBmp");
    hbrDock = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    hcurScanner = LoadCursor(hInst, "ScannerCur");
    hcurScanAdd = LoadCursor(hInst, "ScannerAdd");
    hcurOpenGrab = LoadCursor(hInst, "OpenGrabCur");
    hcurCloseGrab = LoadCursor(hInst, "CloseGrabCur");
    hcurTrashCan = LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRASH_CAN));
    hcurNoWay = LoadCursor(hInst, MAKEINTRESOURCE(IDC_NO_WAY));
    hcurResizeWE = LoadCursor(hInst, MAKEINTRESOURCE(IDC_RESIZE_WE));
    hcurResizeNS = LoadCursor(hInst, MAKEINTRESOURCE(IDC_RESIZE_NS));
    hcurResize4Way = LoadCursor(hInst, MAKEINTRESOURCE(IDC_RESIZE_4WAY));
    hcurArrowHelp = LoadCursor(hInst, MAKEINTRESOURCE(IDC_ARROW_HELP));
    hcurHand = LoadCursor(hInst, MAKEINTRESOURCE(IDC_HAND));
    hbmpScanner = LoadBitmap(hInst, "ScannerBmp");
    hbmpScanShip = LoadBitmap(hInst, MAKEINTRESOURCE(IDDIB_SCANNER_TOOLBAR));
    hbmpUnknownPlanet = LoadBitmap(hInst, "UnknownPlanetBmp");
    hbmpNumbers = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FONT_DIGITS));
    hdibPlanets = HdibLoadBigResource(IDDIB_PLANET_ICONS);
    hdibThings = HdibLoadBigResource(IDDIB_THING_ICONS);
    hdibToolbar = HdibLoadBigResource(IDB_TOOLBAR);
    if (hdibPlanets == 0 || hdibThings == 0 || hdibToolbar == 0) {
        fFailed = 1;
    }
    for (i = 0; i < 5; i++) {
        rghdibShips[i] = HdibLoadBigResource(i + 552);
        if (rghdibShips[i] == 0) {
            fFailed = 1;
        }
        rghdibShipsT[i] = HdibLoadBigResource(i + 557);
        if (rghdibShipsT[i] == 0) {
            fFailed = 1;
        }
    }
    for (i = 0; i < 7; i++) {
        rghdibInventory[i] = HdibLoadBigResource(i + 500);
        if (rghdibInventory[i] == 0) {
            fFailed = 1;
        }
    }
    vhpal = HpalFromDib(rghdibShips[3]);
    hdibRaces = HdibLoadBigResource(IDDIB_PLAYER_ICONS);
    hdibRacesT = HdibLoadBigResource(IDDIB_PLAYER_ICONS_SMALL);
    hdibRacesX = HdibLoadBigResource(IDDIB_PLAYER_ICONS_TINY);
    hbmpBackBld = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EMPTY_HULL_SLOT));
    hbmpMsg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MSGFILTER_CHECKBOX));
    hbmpMono = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILTER_CHECKBOX_MONO));
    hdibPlaque = HdibLoadBigResource(IDDIB_NUM_DESIGNS_PLATE);
    hiconStars = LoadIcon(hInst, "StarsIco");
    hiconHost = LoadIcon(hInst, "HostIco");
    hiconWait = LoadIcon(hInst, "WaitIco");
    rghiconVCR[0] = LoadIcon(hInst, "Bang1Ico");
    rghiconVCR[1] = LoadIcon(hInst, "Bang2Ico");
    rghiconVCR[2] = LoadIcon(hInst, "Bang3Ico");
    rghiconVCR[3] = LoadIcon(hInst, "Torp1Ico");
    rghiconVCR[4] = LoadIcon(hInst, "Torp2Ico");
    rghiconVCR[5] = LoadIcon(hInst, "Torp3Ico");
    rghiconVCR[6] = LoadIcon(hInst, "Torp4Ico");
    lpLog = LpAlloc(32000, htLog);
    lpMsg = LpAlloc(0xffc8, htMsg);
    lpfnFakeComboProc = MakeProcInstance(FakeComboProc, hInst);
    lpfnFakeCEProc = MakeProcInstance(FakeCEProc, hInst);
    lpfnFakeEditProc = MakeProcInstance(FakeEditProc, hInst);
    lpfnFakeListProc = MakeProcInstance(FakeListProc, hInst);
    lpfnHostTimerProc = MakeProcInstance(HostTimerProc, hInst);
    lpfnBrowserDlgProc = MakeProcInstance(BrowserDlg, hInst);
    lpfnReportDlgProc = MakeProcInstance(ReportDlg, hInst);
    lpfnGaugeDlgProc = MakeProcInstance(ProgressGaugeDlg, hInst);
    GetDiskSerialNumber();
    lpb2k = LpAlloc(0x800, htPerm);
    vlprgidMisc = LpAlloc(0x800, htPerm);
    vlprgidPlanet = LpAlloc(0x800, htPerm);
    vlprgidFleet = LpAlloc(0x800, htPerm);
    if (fFailed != 0 || hbmpScanner == 0 || hbmpUnknownPlanet == 0 || hbmpBackBld == 0 || hdibRaces == 0 || hdibRacesT == 0 || hdibRacesX == 0 ||
        hbmpMono == 0 || hbmpScanShip == 0 || hbmpMsg == 0 || hiconHost == 0 || hiconStars == 0 || hiconWait == 0) {
        AlertSz(PszFormatIds(idsUnableLoadBitmaps, NULL), MB_ICONHAND);
        return 0;
    }
    return 1;
}

int16_t FCreateFonts(HDC hdc) {
    int16_t    i;
    LOGFONT   *plf;
    HFONT      hfontSav;
    TEXTMETRIC tm;
    int32_t    l;

    plf = LocalAlloc(64, sizeof(LOGFONT));
    for (i = 0; i < 4; i++) {
        if (rgszArial[i][0] == 0) {
            CchGetString(i + 1335, rgszArial[i]);
        }
    }
    plf->lfHeight = -MulDiv(10, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    for (i = 0; i < 2; i++) {
        strcpy(plf->lfFaceName, rgszArial[i]);
        rghfontArial10[i] = CreateFontIndirect(plf);
    }
    strcpy(plf->lfFaceName, rgszArial[0]);
    plf->lfHeight = -MulDiv(6, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    rghfontArial6[0] = CreateFontIndirect(plf);
    plf->lfHeight = -MulDiv(7, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    rghfontArial7[0] = CreateFontIndirect(plf);
    plf->lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    for (i = 0; i < 4; i++) {
        strcpy(plf->lfFaceName, rgszArial[i]);
        rghfontArial8[i] = CreateFontIndirect(plf);
    }
    strcpy(plf->lfFaceName, rgszArial[1]);
    plf->lfEscapement = 3150;
    rghfontArial8[4] = CreateFontIndirect(plf);
    hfontSav = SelectObject(hdc, rghfontArial8[0]);
    GetTextMetrics(hdc, &tm);
    dyArial8 = tm.tmHeight + tm.tmExternalLeading;
    l = GetTextExtent(hdc, "88888888kT", 10);
    dxMaxMineralQuan = LOWORD(l);
    SelectObject(hdc, rghfontArial7[0]);
    GetTextMetrics(hdc, &tm);
    dyArial7 = tm.tmHeight + tm.tmExternalLeading;
    SelectObject(hdc, rghfontArial6[0]);
    GetTextMetrics(hdc, &tm);
    dyArial6 = tm.tmHeight + tm.tmExternalLeading;
    SelectObject(hdc, rghfontArial10[0]);
    GetTextMetrics(hdc, &tm);
    dyArial10 = tm.tmHeight + tm.tmExternalLeading;
    SelectObject(hdc, hfontSav);
    LocalFree(plf);
    return 1;
}

int16_t InitInstance(int16_t nCmdShow) {
    int16_t sw;
    RECT    rc;

    ini.fWait = 0;
    ini.fStartupFile = 0;
    ini.grobjSel = 0;
    ini.idPlayer = -1;
    ReadIniSettings();
    rc = ini.wnFrame.rc;
    hwndFrame = CreateWindow(szFrame, "Stars!", WS_OVERLAPPEDWINDOW, rc.left, rc.top, rc.right, rc.bottom, NULL, NULL, hInst, NULL);
    if (hwndFrame == 0) {
        return 0;
    }
    hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDA_MAIN));
    if (hAccel == 0) {
        return 0;
    }
    hAccelTitle = LoadAccelerators(hInst, MAKEINTRESOURCE(IDA_TITLE));
    if (hAccelTitle == 0) {
        return 0;
    }
    if (nCmdShow != 1) {
        sw = nCmdShow;
    } else {
        sw = ini.wnFrame.fMaximized != 0 || ini.wnFrame.fMinimized != 0 ? 3 : 1;
    }
    ShowWindow(hwndFrame, sw);
    ShowWindow(hwndFrame, SW_HIDE);
    return 1;
}

void InitTiles() {
    int16_t  yTop;
    int16_t  ctile;
    TILE    *rgtile;
    int16_t  i;
    int16_t  iPass;
    uint16_t iCol;

    iPass = 2;
    rgtile = rgtilePlanet;
    ctile = 6;
    while (iPass-- != 0) {
        iCol = 0;
        yTop = 4;
        for (i = 0; i < ctile; i++) {
            if (rgtile[i].iCol != iCol) {
                yTop = 4;
                iCol = rgtile[i].iCol;
            }
            rgtile[i].dyFull += rgtile[i].yTop * dyArial8;
            rgtile[i].yTop = yTop;
            rgtile[i].fFixCtls = 0;
            rgtile[i].fMinDraw = 0;
            if (rgtile[i].fPopped != 0) {
                yTop += rgtile[i].dyFull + 4;
            } else {
                yTop += dyArial8 + 7;
            }
        }
        rgtile = rgtileShip;
        ctile = 7;
    }
    return;
}

void GetIniWinRc(char *szSection, char *szIniFile, StringId ids, WN *pwn) {
    int16_t fInitalized;
    int16_t fMinimized;
    int16_t fMaximized;
    char    szEntry[16];
    int16_t cch;
    RECT    rc;
    int16_t j;
    char   *pch;
    int16_t i;
    int16_t fNeg;
    int16_t rg[4];

    CchGetString(ids, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, "X", szWork, 20, szIniFile);
    if (cch == 17) {
        switch (szWork[0]) {
        default:
            goto NoRc;
        case 'M':
        case 'R':
        case 'I':
            pch = &szWork[1];
            for (i = 0; i < 4; i++) {
                rg[i] = 0;
                fNeg = 0;
                j = 0;
                while (j < 4) {
                    if (*pch == '-') {
                        fNeg = 1;
                    } else {
                        if (*pch < '0' || *pch > '9')
                            goto NoRc;
                        rg[i] = 10 * rg[i] + (*pch - '0');
                    }
                    j++;
                    pch++;
                }
                if (fNeg != 0) {
                    rg[i] = -rg[i];
                }
            }
            rc.left = rg[0];
            rc.top = rg[1];
            rc.right = rg[2];
            rc.bottom = rg[3];
            fMaximized = szWork[0] == 'M';
            fMinimized = szWork[0] == 'I';
            fInitalized = 1;
        }
        goto L_11ec;
    }
NoRc:
    rc.left = -32768;
    rc.right = -32768;
    rc.bottom = 0;
    rc.top = 0;
    fMaximized = ids == idsMain;
    fMinimized = 0;
    fInitalized = 0;
L_11ec:
    pwn->rc = rc;
    pwn->fMaximized = fMaximized;
    pwn->fMinimized = fMinimized;
    pwn->fInitalized = fInitalized;
    return;
}

void ReadIniSettings() {
    uint16_t uDateCur;
    int16_t  i;
    int16_t  iPass;
    char     szEntry[16];
    WN       wnT;
    char     szIniFile[16];
    uint16_t w;
    char    *psz;
    char     szSection[16];
    int16_t  cch;
    int16_t  cpq;
    uint16_t t_scratch_m4a_8;
    uint16_t t_scratch_m4c;

    ini.fGen = 0;
    ini.fTry = 0;
    ini.fWait = 0;
    CchGetString(idsWindows, szSection);
    CchGetString(idsStarsIni, szIniFile);
    GetIniWinRc(szSection, szIniFile, idsMain, &ini.wnFrame);
    GetIniWinRc(szSection, szIniFile, idsReportfleetwin, &wnT);
    if (wnT.rc.left != -32768) {
        vrptFleet.ptDlg.x = wnT.rc.left;
        vrptFleet.ptDlg.y = wnT.rc.top;
        vrptFleet.ptSize.x = wnT.rc.right - wnT.rc.left;
        vrptFleet.ptSize.y = wnT.rc.bottom - wnT.rc.top;
    }
    GetIniWinRc(szSection, szIniFile, idsReportefleetwin, &wnT);
    if (wnT.rc.left != -32768) {
        vrptEFleet.ptDlg.x = wnT.rc.left;
        vrptEFleet.ptDlg.y = wnT.rc.top;
        vrptEFleet.ptSize.x = wnT.rc.right - wnT.rc.left;
        vrptEFleet.ptSize.y = wnT.rc.bottom - wnT.rc.top;
    }
    GetIniWinRc(szSection, szIniFile, idsReportbtlwin, &wnT);
    if (wnT.rc.left != -32768) {
        vrptBattle.ptDlg.x = wnT.rc.left;
        vrptBattle.ptDlg.y = wnT.rc.top;
        vrptBattle.ptSize.x = wnT.rc.right - wnT.rc.left;
        vrptBattle.ptSize.y = wnT.rc.bottom - wnT.rc.top;
    }
    GetIniWinRc(szSection, szIniFile, idsReportplanwin, &wnT);
    if (wnT.rc.left != -32768) {
        vrptPlanet.ptDlg.x = wnT.rc.left;
        vrptPlanet.ptDlg.y = wnT.rc.top;
        vrptPlanet.ptSize.x = wnT.rc.right - wnT.rc.left;
        vrptPlanet.ptSize.y = wnT.rc.bottom - wnT.rc.top;
    }
    CchGetString(idsResolution, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    if (i == 0 && (vcScreenColors <= 4 || gd.mdScreenSize == 0)) {
        AlertSz(PszFormatIds(idsNoteStarsPrefersScreenResolutionLeast800x600, NULL), MB_ICONHAND);
    }
    CchGetString(idsLayout, szEntry);
    iWindowLayout = GetPrivateProfileInt(szSection, szEntry, 1, szIniFile);
    iWindowLayout = 2 < (layoutLarge <= (int16_t)iWindowLayout ? iWindowLayout : 0) ? layoutSmall
                    : layoutLarge > (int16_t)iWindowLayout                          ? layoutLarge
                                                                                    : iWindowLayout;
    CchGetString(idsStyle1width, szEntry);
    vfs.dxPlanWant = GetPrivateProfileInt(szSection, szEntry, 396, szIniFile);
    vfs.dxPlanWant = (vfs.dxPlanWant <= 10 ? 10 : vfs.dxPlanWant) >= 2000 ? 2000 : vfs.dxPlanWant > 10 ? vfs.dxPlanWant : 10;
    CchGetString(idsStyle1height, szEntry);
    vfs.dyMsgWant = GetPrivateProfileInt(szSection, szEntry, 110, szIniFile);
    vfs.dyMsgWant = (vfs.dyMsgWant <= 10 ? 10 : vfs.dyMsgWant) >= 2000 ? 2000 : vfs.dyMsgWant > 10 ? vfs.dyMsgWant : 10;
    CchGetString(idsStyle1height2, szEntry);
    vfs.dyMinWant = GetPrivateProfileInt(szSection, szEntry, 192, szIniFile);
    vfs.dyMinWant = (vfs.dyMinWant <= 10 ? 10 : vfs.dyMinWant) >= 2000 ? 2000 : vfs.dyMinWant > 10 ? vfs.dyMinWant : 10;
    CchGetString(idsStyle2width, szEntry);
    vfs.dx2PlanWant = GetPrivateProfileInt(szSection, szEntry, 396, szIniFile);
    vfs.dx2PlanWant = (vfs.dx2PlanWant <= 10 ? 10 : vfs.dx2PlanWant) >= 2000 ? 2000 : vfs.dx2PlanWant > 10 ? vfs.dx2PlanWant : 10;
    CchGetString(idsStyle2height, szEntry);
    vfs.dy2MsgWant = GetPrivateProfileInt(szSection, szEntry, 110, szIniFile);
    vfs.dy2MsgWant = (vfs.dy2MsgWant <= 10 ? 10 : vfs.dy2MsgWant) >= 2000 ? 2000 : vfs.dy2MsgWant > 10 ? vfs.dy2MsgWant : 10;
    CchGetString(idsStyle2height2, szEntry);
    vfs.dy2MinWant = GetPrivateProfileInt(szSection, szEntry, 192, szIniFile);
    vfs.dy2MinWant = (vfs.dy2MinWant <= 10 ? 10 : vfs.dy2MinWant) >= 2000 ? 2000 : vfs.dy2MinWant > 10 ? vfs.dy2MinWant : 10;
    CchGetString(idsToolbar, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 1, szIniFile);
    gd.fToolbar = i != 0;
    CchGetString(idsGlobalsettings, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, " ", szWork, 40, szIniFile);
    if (cch != 28) {
        vSerialNumber = 0;
    } else {
        FSerialAndEnvFromSz(&vSerialNumber, vrgbMachineConfig, szWork);
    }
    CchGetString(idsPlanettiles, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, "X", szWork, 20, szIniFile);
    ReadIniTileSettings(szWork, rgtilePlanet, 6);
    CchGetString(idsShiptiles, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, "X", szWork, 20, szIniFile);
    ReadIniTileSettings(szWork, rgtileShip, 7);
    CchGetString(idsSelection, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, "N", szWork, 20, szIniFile);
    if (cch < 3) {
        ini.grobjSel = 0;
    } else {
        switch (szWork[0]) {
        case 'N':
        default:
            ini.grobjSel = 0;
            break;
        case 'P':
            ini.grobjSel = 1;
            break;
        case 'S':
            ini.grobjSel = 2;
            break;
        case 'E':
            ini.grobjSel = 4;
        }
        if (szWork[1] >= 'B' && szWork[1] <= 'Q') {
            ini.idPlayer = szWork[1] - 'B';
        } else {
            ini.grobjSel = 0;
        }
        if (ini.grobjSel != 0) {
            ini.iObjSel = atoi(&szWork[2]);
        }
    }
    CchGetString(idsMessage, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    ini.iMsg = i;
    CchGetString(idsGameid, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, "0", szWork, 10, szIniFile);
    ini.lid = 0;
    for (i = 0; i < cch; i++) {
        ini.lid = (int32_t)(ini.lid * 16);
        if (szWork[i] >= '0' && szWork[i] <= '9') {
            ini.lid += (int16_t)(szWork[i] - '0');
        } else if (szWork[i] >= 'a' && szWork[i] <= 'f') {
            ini.lid += (int16_t)(szWork[i] - 'W');
        }
    }
    CchGetString(idsScanzoom, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 4, szIniFile);
    if (w >= 1 && w <= 9) {
        iScanZoom = w - 5;
    }
    CchGetString(idsScanfilterv25, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    grbitScanShip = w;
    CchGetString(idsScanefilterv25, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    grbitScanEShip = w;
    CchGetString(idsScanmines, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 15, szIniFile);
    grbitScanMines = w & 0xf;
    CchGetString(idsScanradar, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 100, szIniFile);
    if (w < 0 || w > 100) {
        w = 100;
    }
    vpctRadarView = w;
    CchGetString(idsScanmodev25, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 224, szIniFile);
    grbitScan = w & 0xc00f;
    if (grbitScan >= 6) {
        grbitScan = 0;
        grbitScanShip = 0;
    } else {
        grbitScan |= w & 0x3ff0;
    }
    CchGetString(idsMineralscale, szEntry);
    cMinGrafMax = GetPrivateProfileInt(szSection, szEntry, cMinGrafMax, szIniFile);
    if (cMinGrafMax < 100 || cMinGrafMax > 30000) {
        cMinGrafMax = 5000;
    }
    CchGetString(idsFiles, szSection);
    CchGetString(idsLogging, szEntry);
    ini.fLogging = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    CchGetString(idsWait2, szEntry);
    ini.fWait = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    CchGetString(idsFile1, szEntry);
    cch = GetPrivateProfileString(szSection, szEntry, ".", szWork, 256, szIniFile);
    if (cch > 3) {
        ini.fStartupFile = 1;
        strcpy(szBase, szWork);
    } else {
        ini.fStartupFile = 0;
    }
    if (vrgszMRU == 0) {
        vrgszMRU = LpAlloc(2304, htPerm);
    }
    psz = &szEntry[strlen(szEntry) - 1];
    for (i = 0; i < 9; i++) {
        *psz = i + '1';
        cch = GetPrivateProfileString(szSection, szEntry, ".", vrgszMRU + 256 * i, 256, szIniFile);
        if (cch < 4) {
            vrgszMRU[i * 256] = 0;
        }
    }
    iPass = 0;
    for (i = 0; i < 9; i++) {
        if (vrgszMRU[i * 256] != 0) {
            if (i != iPass) {
                fstrcpy(vrgszMRU + 256 * iPass, vrgszMRU + 256 * i);
                vrgszMRU[i * 256] = 0;
            }
            iPass++;
        }
    }
    CchGetString(idsTurn, szEntry);
    ini.turn = GetPrivateProfileInt(szSection, szEntry, game.turn, szIniFile);
    CchGetString(idsMisc, szSection);
    CchGetString(idsDefaultpassword, szEntry);
    GetPrivateProfileString(szSection, szEntry, "", vszDefPass, 16, szIniFile);
    CchGetString(idsProgress, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    gd.fProgressTxt = w != 0;
    CchGetString(idsNewreports, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    gd.fPerPlayerDumps = w != 0;
    CchGetString(idsNohostnames, szEntry);
    w = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    gd.fNoHostNames = w != 0;
    CchGetString(idsBackups, szEntry);
    vcBackupDirs = GetPrivateProfileInt(szSection, szEntry, 1, szIniFile);
    if (vcBackupDirs < 1 || vcBackupDirs > 999) {
        vcBackupDirs = 1;
    }
    CchGetString(idsReportplanfld, szEntry);
    vrptPlanet.grbitVisible = (uint32_t)GetPrivateProfileInt(szSection, szEntry, -1, szIniFile);
    CchGetString(idsReportplansort, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    vrptPlanet.fAscending = (i & 0x100) != 0;
    i &= 0xff;
    vrptPlanet.icolSort = i;
    CchGetString(idsReportfleetfld, szEntry);
    vrptFleet.grbitVisible = (uint32_t)GetPrivateProfileInt(szSection, szEntry, -1, szIniFile);
    CchGetString(idsReportfleetsort, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    vrptFleet.fAscending = (i & 0x100) != 0;
    i &= 0xff;
    vrptFleet.icolSort = i;
    CchGetString(idsReportefleetfld, szEntry);
    vrptEFleet.grbitVisible = (uint32_t)GetPrivateProfileInt(szSection, szEntry, -1, szIniFile);
    CchGetString(idsReportefltsort, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    vrptEFleet.fAscending = (i & 0x100) != 0;
    i &= 0xff;
    vrptEFleet.icolSort = i;
    CchGetString(idsReportbtlfld, szEntry);
    vrptBattle.grbitVisible = (uint32_t)GetPrivateProfileInt(szSection, szEntry, -1, szIniFile);
    CchGetString(idsReportbtlsort, szEntry);
    i = GetPrivateProfileInt(szSection, szEntry, 0, szIniFile);
    vrptBattle.fAscending = (i & 0x100) != 0;
    i &= 0xff;
    vrptBattle.icolSort = i;
    CchGetString(idsReportdefgraph, szEntry);
    gd.iCurGraph = GetPrivateProfileInt(szSection, szEntry, 7, szIniFile);
    CchGetString(idsReportefltsort, szEntry);
    if (gd.iCurGraph > 7) {
        gd.iCurGraph = 7;
    }
    CchGetString(idsVcrspeed, szEntry);
    viSpeedVCR = GetPrivateProfileInt(szSection, szEntry, 1, szIniFile);
    strdate(szWork);
    szWork[5] = 0;
    szWork[2] = 0;
    t_scratch_m4a_8 = atoi(&szWork[6]) * 31 * 12;
    t_scratch_m4c = atoi(szWork) * 31;
    uDateCur = atoi(&szWork[3]) + t_scratch_m4c + t_scratch_m4a_8;
    CchGetString(idsHistoryinfo, szEntry);
    uDateInstalled = GetPrivateProfileInt(szSection, szEntry, -1, szIniFile);
    if (uDateCur < uDateInstalled) {
        uDateInstalled = uDateCur;
    }
    gd.fTrialPeriodOver = uDateCur >= (uint16_t)(uDateInstalled + 21);
    CchGetString(idsFonts, szSection);
    for (i = 0; i < 4; i++) {
        CchGetString(i + 197, szEntry);
        GetPrivateProfileString(szSection, szEntry, "", szWork, 80, szIniFile);
        cch = strlen(szWork);
        if (cch >= 5 && cch <= 31) {
            strcpy(rgszArial[i], szWork);
        }
    }
    CchGetString(idsZiporders, szSection);
    memset(vrgZip, 0, 96);
    for (i = 0; i < 4; i++) {
        strcpy(szEntry, szSection);
        psz = &szEntry[strlen(szEntry)];
        *psz = i + '1';
        psz[1] = 0;
        GetPrivateProfileString(szSection, szEntry, "", szWork, 80, szIniFile);
        cch = strlen(szWork);
        if (cch >= 20 && cch <= 32) {
            psz = szWork;
            iPass = 0;
            for (; iPass < 20 && *psz >= 'a' && *psz <= 'p'; psz++) {
                iPass++;
            }
            if (iPass >= 20) {
                psz = szWork;
                for (iPass = 0; iPass < 5; iPass++) {
                    vrgZip[i].txp.rgia[iPass].iAction = *psz - 'a';
                    psz++;
                    vrgZip[i].txp.rgia[iPass].cQuan = *psz - 'a';
                    psz++;
                    vrgZip[i].txp.rgia[iPass].cQuan =
                        ((uint32_t)(*psz - 'a') & 0xfff) << 4 | (vrgZip[i].txp.rgia[iPass].cQuan | vrgZip[i].txp.rgia[iPass].iAction << 0xc);
                    psz++;
                    vrgZip[i].txp.rgia[iPass].cQuan =
                        ((uint32_t)(*psz - 'a') & 0xff) << 8 | (vrgZip[i].txp.rgia[iPass].cQuan | vrgZip[i].txp.rgia[iPass].iAction << 0xc);
                    psz++;
                }
                strcpy(vrgZip[i].szName, psz);
                vrgZip[i].fValid = 1;
            }
        }
    }
    memset(vrgZipProd, 0, 200);
    for (i = 0; i < 5; i++) {
        strcpy(szEntry, szSection);
        psz = &szEntry[strlen(szEntry)];
        *psz++ = 'P';
        *psz = i + '1';
        psz[1] = 0;
        GetPrivateProfileString(szSection, szEntry, "", szWork, 80, szIniFile);
        cch = strlen(szWork);
        if (cch >= 3 && cch <= 64) {
            psz = szWork;
            cpq = psz[1] - 'a';
            if (cpq >= 0 && cpq <= 12) {
                iPass = 0;
                for (; iPass < cpq * 4 + 2 && *psz >= 'a' && *psz <= 'p'; psz++) {
                    iPass++;
                }
                if (iPass >= cpq * 4 + 2 && strlen(psz) <= 12) {
                    strcpy(vrgZipProd[i].szName, psz);
                    psz = szWork;
                    vrgZipProd[i].fNoResearch = *psz != 'a';
                    vrgZipProd[i].fValid = 1;
                    vrgZipProd[i].cpq = cpq;
                    psz += 2;
                    for (iPass = 0; iPass < cpq; iPass++) {
                        vrgZipProd[i].rgpq[iPass].w = *psz - 'a';
                        psz++;
                        vrgZipProd[i].rgpq[iPass].w |= (*psz - 'a') * 0x10;
                        psz++;
                        vrgZipProd[i].rgpq[iPass].w |= (*psz - 'a') * 0x100;
                        psz++;
                        vrgZipProd[i].rgpq[iPass].w |= (*psz - 'a') * 0x1000;
                        psz++;
                        if (vrgZipProd[i].rgpq[iPass].cQuan > 1020) {
                            vrgZipProd[i].rgpq[iPass].cQuan = 1;
                        }
                        if (vrgZipProd[i].rgpq[iPass].mdIdle >= mdIdleFactory) {
                            vrgZipProd[i].rgpq[iPass].mdIdle = iobjMine;
                        }
                    }
                }
            }
        }
    }
    CchGetString(idsDefault, vrgZipProd[0].szName);
    vrgZipProd[0].fValid = 1;
    return;
}

void ReadIniTileSettings(char *pszFormat, TILE *rgtile, int16_t ctile) {
    TILE     tile;
    int16_t  fPopped;
    int16_t  i;
    int16_t  iTile;
    uint16_t iCol;
    uint16_t iBit;

    iCol = 0;
    iTile = 0;
    for (; *pszFormat != 0; pszFormat++) {
        if (*pszFormat == '*') {
            if (iCol < 1) {
                iCol++;
            }
        } else if (isalpha(*pszFormat) != 0) {
            fPopped = isupper(*pszFormat);
            iBit = fPopped == 0 ? *pszFormat - 'a' : *pszFormat - 'A';
            if ((int16_t)iBit >= 0) {
                for (i = iTile; i < ctile && rgtile[i].id != iBit; i++) {
                }
                if (i != ctile) {
                    rgtile[i].iCol = iCol;
                    rgtile[i].fPopped = fPopped;
                    if (i != iTile) {
                        tile = rgtile[i];
                        rgtile[i] = rgtile[iTile];
                        rgtile[iTile] = tile;
                    }
                    iTile++;
                }
            }
        }
    }
    for (i = iTile; i < ctile; i++) {
        if (rgtile[i].iCol < iCol) {
            rgtile[i].iCol = iCol;
        }
    }
    return;
}
