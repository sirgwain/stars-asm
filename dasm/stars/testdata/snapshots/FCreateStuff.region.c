int16_t FCreateStuff() {
    int16_t fFailed;
    int16_t dy;
    int16_t i;
    HBITMAP hbmp;
    int16_t dx;

    fFailed = FALSE;
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
    gd.fNoIdleChecks = FALSE;
    gd.fAisDone = FALSE;
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
        fFailed = TRUE;
    }
    for (i = 0; i < 5; i++) {
        rghdibShips[i] = HdibLoadBigResource(i + 552);
        if (rghdibShips[i] == 0) {
            fFailed = TRUE;
        }
        rghdibShipsT[i] = HdibLoadBigResource(i + 557);
        if (rghdibShipsT[i] == 0) {
            fFailed = TRUE;
        }
    }
    for (i = 0; i < 7; i++) {
        rghdibInventory[i] = HdibLoadBigResource(i + 500);
        if (rghdibInventory[i] == 0) {
            fFailed = TRUE;
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
        return FALSE;
    }
    return TRUE;
}
