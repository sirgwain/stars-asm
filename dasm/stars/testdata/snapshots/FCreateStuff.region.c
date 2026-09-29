int16_t FCreateStuff() {
    int16_t fFailed;
    int16_t dy;
    int16_t i;
    HBITMAP hbmp;
    int16_t dx;

    fFailed = 0;
    dx = GetSystemMetrics(SM_CXSCREEN);
    dy = GetSystemMetrics(SM_CYSCREEN);
    if (dx >= 800 && dy >= 600) {
        if (dx >= 1024 && dy >= 768) {
            if (dx >= 1111 && dy >= 888) {
                gd.mdScreenSize = 0x3;
            } else {
                gd.mdScreenSize = 0x2;
            }
        } else {
            gd.mdScreenSize = 0x1;
        }
    } else {
        gd.mdScreenSize = 0x0;
    }
    gd.fNoIdleChecks = 0x0;
    gd.fAisDone = 0x0;
    vplr = vrgplrDef[0];
    hrgnHuge = CreateRectRgn(-10, -10, 2000, 2000);
    hrgnScratch = CreateRectRgn(0, 0, 10, 10);
    hbrShip = HbrGet(0xff00);
    hbrStarbase = HbrGet(0xffff);
    hbrBBlue = HbrGet(0xff0000);
    hbrEnemy = HbrGet(0xff);
    hbrSelect = HbrGet(0xffff);
    hbrRed = HbrGet(0xff);
    hbrBlue = HbrGet(0x7f0000);
    hbrGreen = HbrGet(0x7f00);
    hbrRadar = HbrGet(0x7f);
    hbrPurple = HbrGet(0x7f007f);
    hbrTooltip = HbrGet(0x9fffff);
    hbrRadarNear = 0x0;
    rghbrMineral[0] = HbrGet(0xff0000);
    rghbrMineral[1] = HbrGet(0x7f00);
    rghbrMineral[2] = HbrGet(0xffff);
    rghbrMineral[3] = HbrGet(0xffffff);
    rghbrMineral[4] = HbrGet(0xff);
    rghbrPlanetAttr[0][0] = HbrGet(0x7f0000);
    rghbrPlanetAttr[0][1] = HbrGet(0xff0000);
    rghbrPlanetAttr[1][0] = HbrGet(0x7f);
    rghbrPlanetAttr[1][1] = HbrGet(0xff);
    rghbrPlanetAttr[2][0] = HbrGet(0x7f00);
    rghbrPlanetAttr[2][1] = HbrGet(0xff00);
    rghbrMinSum[0][0] = HbrGet(0xff0000);
    rghbrMinSum[0][1] = HbrGet(0x7f0000);
    rghbrMinSum[1][0] = HbrGet(0xff00);
    rghbrMinSum[1][1] = HbrGet(0x7f00);
    rghbrMinSum[2][0] = HbrGet(0xffff);
    rghbrMinSum[2][1] = HbrGet(0x7f7f);
    rghbrMinSum[3][0] = HbrGet(0xff);
    rghbrMinSum[3][1] = HbrGet(0x7f);
    hbrYellow = HbrGet(0xffff);
    hbrDkYellow = HbrGet(0x7f7f);
    hbrLightGray = HbrGet(0xc0c0c0);
    hbrGray = HbrGet(0x808080);
    hpenShip = CreatePen(0, 1, 0xff00);
    hpenDkGreen = CreatePen(0, 1, 0x7f00);
    hpenStarbase = CreatePen(0, 1, 0xff0000);
    hpenEnemy = CreatePen(0, 1, 0xff);
    hpenMassPath = CreatePen(2, 1, 0x7f7f7f);
    hpenRadar = CreatePen(0, 1, 0x7f);
    hpenRadarNear = 0x0;
    hpenDkBlue = CreatePen(0, 1, 0x7f0000);
    hpenYellow = CreatePen(0, 1, 0xffff);
    hpenDkYellow = CreatePen(0, 1, 0x7f7f);
    hpenDkPurple = CreatePen(0, 1, 0x7f007f);
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
    if (hdibPlanets == 0x0 || hdibThings == 0x0 || hdibToolbar == 0x0) {
        fFailed = 1;
    }
    for (i = 0; i < 5; i++) {
        rghdibShips[i] = HdibLoadBigResource(i + 552);
        if (rghdibShips[i] == 0x0) {
            fFailed = 1;
        }
        rghdibShipsT[i] = HdibLoadBigResource(i + 557);
        if (rghdibShipsT[i] == 0x0) {
            fFailed = 1;
        }
    }
    for (i = 0; i < 7; i++) {
        rghdibInventory[i] = HdibLoadBigResource(i + 500);
        if (rghdibInventory[i] == 0x0) {
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
    lpLog = LpAlloc(0x7d00, htLog);
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
    if (fFailed == 0 && hbmpScanner != 0x0 && hbmpUnknownPlanet != 0x0 && hbmpBackBld != 0x0 && hdibRaces != 0x0 && hdibRacesT != 0x0 && hdibRacesX != 0x0 &&
        hbmpMono != 0x0 && hbmpScanShip != 0x0 && hbmpMsg != 0x0 && hiconHost != 0x0 && hiconStars != 0x0 && hiconWait != 0x0) {
        return 1;
    }
    AlertSz(PszFormatIds(idsUnableLoadBitmaps, 0x0), MB_ICONHAND);
    return 0;
}
