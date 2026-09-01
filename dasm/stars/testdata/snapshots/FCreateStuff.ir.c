int16_t FCreateStuff() {
    int16_t fFailed;
    int16_t dy;
    int16_t i;
    HBITMAP hbmp;
    int16_t dx;

L_0014:
    fFailed = 0;
    dx = GetSystemMetrics(SM_CXSCREEN);
    dy = GetSystemMetrics(SM_CYSCREEN);
    if ((dx < 800))
        goto L_004e;
    else
        goto L_0044;

L_0044:
    if ((dy >= 600))
        goto L_005d;
    else
        goto L_004e;

L_004e:
    gd.mdScreenSize = 0x0;
    goto L_00af;

L_005d:
    if ((dx < 1024))
        goto L_0071;
    else
        goto L_0067;

L_0067:
    if ((dy >= 768))
        goto L_0080;
    else
        goto L_0071;

L_0071:
    gd.mdScreenSize = 0x1;
    goto L_00af;

L_0080:
    if ((dx < 1111))
        goto L_0094;
    else
        goto L_008a;

L_008a:
    if ((dy >= 888))
        goto L_00a3;
    else
        goto L_0094;

L_0094:
    gd.mdScreenSize = 0x2;
    goto L_00af;

L_00a3:
    gd.mdScreenSize = 0x3;

L_00af:
    gd.fNoIdleChecks = 0x0;
    gd.fAisDone = 0x0;
    vplr = vrgplrDef[0];
    hrgnHuge = CreateRectRgn(0xfff6, 0xfff6, 0x7d0, 0x7d0);
    hrgnScratch = CreateRectRgn(0x0, 0x0, 0xa, 0xa);
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
    hpenShip = CreatePen(0x0, 0x1, 0xff00);
    hpenDkGreen = CreatePen(0x0, 0x1, 0x7f00);
    hpenStarbase = CreatePen(0x0, 0x1, 0xff0000);
    hpenEnemy = CreatePen(0x0, 0x1, 0xff);
    hpenMassPath = CreatePen(0x2, 0x1, 0x7f7f7f);
    hpenRadar = CreatePen(0x0, 0x1, 0x7f);
    hpenRadarNear = 0x0;
    hpenDkBlue = CreatePen(0x0, 0x1, 0x7f0000);
    hpenYellow = CreatePen(0x0, 0x1, 0xffff);
    hpenDkYellow = CreatePen(0x0, 0x1, 0x7f7f);
    hpenDkPurple = CreatePen(0x0, 0x1, 0x7f007f);
    hbmp = LoadBitmap(hInst, "Screen50Bmp");
    hbr50Screen = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    i = 0;
    goto L_04f1;

L_04b7:
    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(((i + 0x1cc) | 0x0)));
    rghbrPat[i] = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    i = (i + 1);

L_04f1:
    if ((i < 3))
        goto L_04b7;
    else
        goto L_04fa;

L_04fa:
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
    hcurTrashCan = LoadCursor(hInst, MAKEINTRESOURCE(0x7a));
    hcurNoWay = LoadCursor(hInst, MAKEINTRESOURCE(0x79));
    hcurResizeWE = LoadCursor(hInst, MAKEINTRESOURCE(0x102));
    hcurResizeNS = LoadCursor(hInst, MAKEINTRESOURCE(0x104));
    hcurResize4Way = LoadCursor(hInst, MAKEINTRESOURCE(0x107));
    hcurArrowHelp = LoadCursor(hInst, MAKEINTRESOURCE(0x108));
    hcurHand = LoadCursor(hInst, MAKEINTRESOURCE(0x109));
    hbmpScanner = LoadBitmap(hInst, "ScannerBmp");
    hbmpScanShip = LoadBitmap(hInst, MAKEINTRESOURCE(0x58));
    hbmpUnknownPlanet = LoadBitmap(hInst, "UnknownPlanetBmp");
    hbmpNumbers = LoadBitmap(hInst, MAKEINTRESOURCE(0xf9));
    hdibPlanets = HdibLoadBigResource(0x70);
    hdibThings = HdibLoadBigResource(0x57);
    hdibToolbar = HdibLoadBigResource(0xb2);
    if ((hdibPlanets == 0x0))
        goto L_06b7;
    else
        goto L_06a3;

L_06a3:
    if ((hdibThings == 0x0))
        goto L_06b7;
    else
        goto L_06ad;

L_06ad:
    if ((hdibToolbar != 0x0))
        goto L_06bc;
    else
        goto L_06b7;

L_06b7:
    fFailed = 1;

L_06bc:
    i = 0;
    goto L_0720;

L_06c4:
    rghdibShips[i] = HdibLoadBigResource((i + 552));
    if ((rghdibShips[i] != 0x0))
        goto L_06f0;
    else
        goto L_06eb;

L_06eb:
    fFailed = 1;

L_06f0:
    rghdibShipsT[i] = HdibLoadBigResource((i + 557));
    if ((rghdibShipsT[i] != 0x0))
        goto L_071c;
    else
        goto L_0717;

L_0717:
    fFailed = 1;

L_071c:
    i = (i + 1);

L_0720:
    if ((i < 5))
        goto L_06c4;
    else
        goto L_0729;

L_0729:
    i = 0;
    goto L_0761;

L_0731:
    rghdibInventory[i] = HdibLoadBigResource((i + 500));
    if ((rghdibInventory[i] != 0x0))
        goto L_075d;
    else
        goto L_0758;

L_0758:
    fFailed = 1;

L_075d:
    i = (i + 1);

L_0761:
    if ((i < 7))
        goto L_0731;
    else
        goto L_076a;

L_076a:
    vhpal = HpalFromDib(rghdibShips[3]);
    hdibRaces = HdibLoadBigResource(0x85);
    hdibRacesT = HdibLoadBigResource(0x50);
    hdibRacesX = HdibLoadBigResource(0x4f);
    hbmpBackBld = LoadBitmap(hInst, MAKEINTRESOURCE(0x77));
    hbmpMsg = LoadBitmap(hInst, MAKEINTRESOURCE(0x86));
    hbmpMono = LoadBitmap(hInst, MAKEINTRESOURCE(0xc7));
    hdibPlaque = HdibLoadBigResource(0x437);
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
    if ((fFailed != 0))
        goto L_0a7f;
    else
        goto L_0a07;

L_0a07:
    if ((hbmpScanner == 0x0))
        goto L_0a7f;
    else
        goto L_0a11;

L_0a11:
    if ((hbmpUnknownPlanet == 0x0))
        goto L_0a7f;
    else
        goto L_0a1b;

L_0a1b:
    if ((hbmpBackBld == 0x0))
        goto L_0a7f;
    else
        goto L_0a25;

L_0a25:
    if ((hdibRaces == 0x0))
        goto L_0a7f;
    else
        goto L_0a2f;

L_0a2f:
    if ((hdibRacesT == 0x0))
        goto L_0a7f;
    else
        goto L_0a39;

L_0a39:
    if ((hdibRacesX == 0x0))
        goto L_0a7f;
    else
        goto L_0a43;

L_0a43:
    if ((hbmpMono == 0x0))
        goto L_0a7f;
    else
        goto L_0a4d;

L_0a4d:
    if ((hbmpScanShip == 0x0))
        goto L_0a7f;
    else
        goto L_0a57;

L_0a57:
    if ((hbmpMsg == 0x0))
        goto L_0a7f;
    else
        goto L_0a61;

L_0a61:
    if ((hiconHost == 0x0))
        goto L_0a7f;
    else
        goto L_0a6b;

L_0a6b:
    if ((hiconStars == 0x0))
        goto L_0a7f;
    else
        goto L_0a75;

L_0a75:
    if ((hiconWait != 0x0))
        goto L_0aa6;
    else
        goto L_0a7f;

L_0a7f:
    AlertSz(PszFormatIds(idsUnableLoadBitmaps, 0x0), MB_ICONHAND);
    return 0x0;

L_0aa6:
    return 0x1;
}
