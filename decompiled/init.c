#include "common.h"

uint8_t rgPalGray[20] = {10, 20, 30, 40, 61, 71, 81, 92, 112, 122, 133, 143, 161, 171, 182, 193, 215, 225, 235, 245};

int16_t FCreateStuff() {
    int16_t fFailed;
    int16_t dy;
    int16_t i;
    HBITMAP hbmp;
    int16_t dx;

L_0014:
    fFailed = 0x0;
    dx = GetSystemMetrics(SM_CXSCREEN);
    dy = GetSystemMetrics(SM_CYSCREEN);
    if ((dx < 0x320))
        goto L_004e;
    else
        goto L_0044;

L_0044:
    if ((dy >= 0x258))
        goto L_005d;
    else
        goto L_004e;

L_004e:
    gd.mdScreenSize = 0x0;
    goto L_00af;

L_005d:
    if ((dx < 0x400))
        goto L_0071;
    else
        goto L_0067;

L_0067:
    if ((dy >= 0x300))
        goto L_0080;
    else
        goto L_0071;

L_0071:
    gd.mdScreenSize = 0x1;
    goto L_00af;

L_0080:
    if ((dx < 0x457))
        goto L_0094;
    else
        goto L_008a;

L_008a:
    if ((dy >= 0x378))
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
    /* untranslated: vplr = part[0:192](vrgplrDef) */
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
    LOWORD(rghbrMineral) = HbrGet(0xff0000);
    HIWORD(rghbrMineral) = HbrGet(0x7f00);
    /* untranslated: part[4:2](rghbrMineral) = HbrGet(0xffff) */
    /* untranslated: part[6:2](rghbrMineral) = HbrGet(0xffffff) */
    /* untranslated: part[8:2](rghbrMineral) = HbrGet(0xff) */
    LOWORD(rghbrPlanetAttr) = HbrGet(0x7f0000);
    HIWORD(rghbrPlanetAttr) = HbrGet(0xff0000);
    /* untranslated: part[4:2](rghbrPlanetAttr) = HbrGet(0x7f) */
    /* untranslated: part[6:2](rghbrPlanetAttr) = HbrGet(0xff) */
    /* untranslated: part[8:2](rghbrPlanetAttr) = HbrGet(0x7f00) */
    /* untranslated: part[10:2](rghbrPlanetAttr) = HbrGet(0xff00) */
    LOWORD(rghbrMinSum) = HbrGet(0xff0000);
    HIWORD(rghbrMinSum) = HbrGet(0x7f0000);
    /* untranslated: part[4:2](rghbrMinSum) = HbrGet(0xff00) */
    /* untranslated: part[6:2](rghbrMinSum) = HbrGet(0x7f00) */
    /* untranslated: part[8:2](rghbrMinSum) = HbrGet(0xffff) */
    /* untranslated: part[10:2](rghbrMinSum) = HbrGet(0x7f7f) */
    /* untranslated: part[12:2](rghbrMinSum) = HbrGet(0xff) */
    /* untranslated: part[14:2](rghbrMinSum) = HbrGet(0x7f) */
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
    i = 0x0;
    goto L_04f1;

L_04b7:
    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(((i + 0x1cc) | 0x0)));
    rghbrPat[i] = CreatePatternBrush(hbmp);
    DeleteObject(hbmp);
    i = (i + 0x1);

L_04f1:
    if ((i < 0x3))
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
    fFailed = 0x1;

L_06bc:
    i = 0x0;
    goto L_0720;

L_06c4:
    /* untranslated: rghdibShips[i] = HdibLoadBigResource(part[12:0](szBrowser[i])) */
    if ((rghdibShips[i] != 0x0))
        goto L_06f0;
    else
        goto L_06eb;

L_06eb:
    fFailed = 0x1;

L_06f0:
    /* untranslated: rghdibShipsT[i] = HdibLoadBigResource(part[3:0](szTitle[i])) */
    if ((rghdibShipsT[i] != 0x0))
        goto L_071c;
    else
        goto L_0717;

L_0717:
    fFailed = 0x1;

L_071c:
    i = (i + 0x1);

L_0720:
    if ((i < 0x5))
        goto L_06c4;
    else
        goto L_0729;

L_0729:
    i = 0x0;
    goto L_0761;

L_0731:
    /* untranslated: rghdibInventory[i] = HdibLoadBigResource(part[8:0](szMine[i])) */
    if ((rghdibInventory[i] != 0x0))
        goto L_075d;
    else
        goto L_0758;

L_0758:
    fFailed = 0x1;

L_075d:
    i = (i + 0x1);

L_0761:
    if ((i < 0x7))
        goto L_0731;
    else
        goto L_076a;

L_076a:
    /* untranslated: vhpal = HpalFromDib(part[6:2](rghdibShips)) */
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
    LOWORD(rghiconVCR) = LoadIcon(hInst, "Bang1Ico");
    HIWORD(rghiconVCR) = LoadIcon(hInst, "Bang2Ico");
    /* untranslated: part[4:2](rghiconVCR) = LoadIcon(hInst, "Bang3Ico") */
    /* untranslated: part[6:2](rghiconVCR) = LoadIcon(hInst, "Torp1Ico") */
    /* untranslated: part[8:2](rghiconVCR) = LoadIcon(hInst, "Torp2Ico") */
    /* untranslated: part[10:2](rghiconVCR) = LoadIcon(hInst, "Torp3Ico") */
    /* untranslated: part[12:2](rghiconVCR) = LoadIcon(hInst, "Torp4Ico") */
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
    if ((fFailed != 0x0))
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

int16_t FCreateFonts(HDC hdc) {
    int16_t    i;
    LOGFONT   *plf;
    HFONT      hfontSav;
    TEXTMETRIC tm;
    int32_t    l;

L_0ab2:
    plf = LocalAlloc(0x40, 0x32);
    i = 0x0;
    goto L_0b13;

L_0ad3:
    if (((uint16_t)(rgszArial[i][0x0]) != 0x0))
        goto L_0b0f;
    else
        goto L_0aed;

L_0aed:
    CchGetString((i + 0x537), rgszArial[i]);

L_0b0f:
    i = (i + 0x1);

L_0b13:
    if ((i < 0x4))
        goto L_0ad3;
    else
        goto L_0b1c;

L_0b1c:
    /* untranslated: plf->lfHeight = (MulDiv(0xa, GetDeviceCaps(hdc, LOGPIXELSY), 0x48) neg 0x0) */
    i = 0x0;
    goto L_0b82;

L_0b45:
    strcpy((plf + 0x12), rgszArial[i]);
    rghfontArial10[i] = CreateFontIndirect(plf);
    i = (i + 0x1);

L_0b82:
    if ((i < 0x2))
        goto L_0b45;
    else
        goto L_0b8b;

L_0b8b:
    strcpy((plf + 0x12), 0x6a6);
    /* untranslated: plf->lfHeight = (MulDiv(0x6, GetDeviceCaps(hdc, LOGPIXELSY), 0x48) neg 0x0) */
    rghfontArial6 = CreateFontIndirect(plf);
    /* untranslated: plf->lfHeight = (MulDiv(0x7, GetDeviceCaps(hdc, LOGPIXELSY), 0x48) neg 0x0) */
    rghfontArial7 = CreateFontIndirect(plf);
    /* untranslated: plf->lfHeight = (MulDiv(0x8, GetDeviceCaps(hdc, LOGPIXELSY), 0x48) neg 0x0) */
    i = 0x0;
    goto L_0c66;

L_0c29:
    strcpy((plf + 0x12), rgszArial[i]);
    rghfontArial8[i] = CreateFontIndirect(plf);
    i = (i + 0x1);

L_0c66:
    if ((i < 0x4))
        goto L_0c29;
    else
        goto L_0c6f;

L_0c6f:
    strcpy((plf + 0x12), 0x6c6);
    plf->lfEscapement = 0xc4e;
    /* untranslated: part[8:2](rghfontArial8) = CreateFontIndirect(plf) */
    hfontSav = SelectObject(hdc, LOWORD(rghfontArial8));
    GetTextMetrics(hdc, &(tm));
    dyArial8 = (tm.tmHeight + tm.tmExternalLeading);
    l = GetTextExtent(hdc, "88888888kT", 0xa);
    dxMaxMineralQuan = LOWORD(l);
    SelectObject(hdc, rghfontArial7);
    GetTextMetrics(hdc, &(tm));
    dyArial7 = (tm.tmHeight + tm.tmExternalLeading);
    SelectObject(hdc, rghfontArial6);
    GetTextMetrics(hdc, &(tm));
    dyArial6 = (tm.tmHeight + tm.tmExternalLeading);
    SelectObject(hdc, LOWORD(rghfontArial10));
    GetTextMetrics(hdc, &(tm));
    dyArial10 = (tm.tmHeight + tm.tmExternalLeading);
    SelectObject(hdc, hfontSav);
    LocalFree(plf);

L_0d6a:
    return 0x1;
}

int16_t InitInstance(int16_t nCmdShow) {
    int16_t  sw;
    RECT     rc;
    uint16_t t_merge_0e82_0001;

L_0d70:
    ini.fWait = 0x0;
    ini.fStartupFile = 0x0;
    ini.grobjSel = 0x0;
    ini.idPlayer = 0xffff;
    ReadIniSettings();
    rc = ini.wnFrame.rc;
    hwndFrame = CreateWindow(szFrame, "Stars!", WS_OVERLAPPEDWINDOW, rc.left, rc.top, rc.right, rc.bottom, 0x0, 0x0, hInst, 0x0);
    if ((hwndFrame != 0x0))
        goto L_0e05;
    else
        goto L_0dff;

L_0dff:
    return 0x0;

L_0e05:
    LoadAccelerators(hInst, MAKEINTRESOURCE(0x74));
    /* untranslated: hAccel = callresult(HACCEL) */
    /* untranslated: branch callresult(HACCEL) != 0x0 ? L_0e27 : L_0e21 */

L_0e21:
    return 0x0;

L_0e27:
    LoadAccelerators(hInst, MAKEINTRESOURCE(0x438));
    /* untranslated: hAccelTitle = callresult(HACCEL) */
    /* untranslated: branch callresult(HACCEL) != 0x0 ? L_0e49 : L_0e43 */

L_0e43:
    return 0x0;

L_0e49:
    if ((nCmdShow == 0x1))
        goto L_0e5b;
    else
        goto L_0e52;

L_0e52:
    sw = nCmdShow;
    goto L_0e85;

L_0e5b:
    if ((ini.wnFrame.fMaximized != 0x0))
        goto L_0e79;
    else
        goto L_0e69;

L_0e69:
    if ((ini.wnFrame.fMinimized == 0x0))
        goto L_0e7f;
    else
        goto L_0e79;

L_0e79:
    t_merge_0e82_0001 = 0x3;
    goto L_0e82;

L_0e7f:
    t_merge_0e82_0001 = 0x1;

L_0e82:
    sw = t_merge_0e82_0001;

L_0e85:
    ShowWindow(hwndFrame, sw);
    ShowWindow(hwndFrame, SW_HIDE);
    return 0x1;
}

void InitTiles() {
    int16_t  yTop;
    int16_t  ctile;
    TILE    *rgtile;
    int16_t  i;
    int16_t  iPass;
    uint16_t iCol;
    int16_t  t_0ec2;

L_0eaa:
    iPass = 0x2;
    rgtile = 0x7fc;
    ctile = 0x6;

L_0ec2:
    t_0ec2 = iPass;
    iPass = (iPass - 0x1);
    if ((t_0ec2 == 0x0))
        goto L_1019;
    else
        goto L_0ed1;

L_0ed1:
    iCol = 0x0;
    yTop = 0x4;
    i = 0x0;
    goto L_1001;

L_0ee3:
    /* untranslated: branch (part[10:2](rgtile[i]) & 0x7) == iCol ? L_0f1f : L_0f01 */

L_0f01:
    yTop = 0x4;
    /* untranslated: iCol = (part[10:2](rgtile[i]) & 0x7) */

L_0f1f:
    rgtile[i].dyFull = (rgtile[i].dyFull + LOWORD((rgtile[i].yTop * dyArial8)));
    rgtile[i].yTop = yTop;
    /* untranslated: part[10:2](rgtile[i]) = ((part[10:2](rgtile[i]) & 0xf7ff) | 0x0) */
    /* untranslated: part[10:2](rgtile[i]) = ((part[10:2](rgtile[i]) & 0xefff) | 0x0) */
    /* untranslated: branch ((part[10:2](rgtile[i]) >> 0x7) & 0x1) == 0x0 ? L_0ff4 : L_0fd8 */

L_0fd8:
    yTop = (yTop + (rgtile[i].dyFull + 0x4));
    goto L_0ffd;

L_0ff4:
    yTop = (yTop + (dyArial8 + 0x7));

L_0ffd:
    i = (i + 0x1);

L_1001:
    if ((i < ctile))
        goto L_0ee3;
    else
        goto L_100c;

L_100c:
    rgtile = 0x90e;
    ctile = 0x7;
    goto L_0ec2;

L_1019:
    return;
}

void GetIniWinRc(char *szSection, char *szIniFile, StringId ids, WN *pwn) {
    int16_t  fInitalized;
    int16_t  fMinimized;
    int16_t  fMaximized;
    char     szEntry[16];
    int16_t  cch;
    RECT     rc;
    int16_t  j;
    char    *pch;
    int16_t  i;
    int16_t  fNeg;
    int16_t  rg[4];
    uint16_t t_merge_10bc_0001;
    uint16_t t_merge_11cc_0001;
    uint16_t t_merge_11e4_0001;

L_1020:
    CchGetString(ids, szEntry);
    cch = GetPrivateProfileString(szSection, &(szEntry), "X", szWork, 0x14, szIniFile);
    if ((cch != 0x11))
        goto NoRc;
    else
        goto L_1070;

L_1070:
    if (((uint16_t)(LOWORD(szWork)) == 0x4d))
        goto L_10cc;
    else
        goto L_107c;

L_107c:
    if (((uint16_t)(LOWORD(szWork)) == 0x52))
        goto L_10cc;
    else
        goto L_1088;

L_1088:
    if (((uint16_t)(LOWORD(szWork)) == 0x49))
        goto L_10cc;
    else
        goto NoRc;

NoRc:
    rc.left = 0x8000;
    rc.right = 0x8000;
    rc.bottom = 0x0;
    rc.top = 0x0;
    if ((ids != idsMain))
        goto L_10b9;
    else
        goto L_10b3;

L_10b3:
    t_merge_10bc_0001 = 0x1;
    goto L_10bc;

L_10b9:
    t_merge_10bc_0001 = 0x0;

L_10bc:
    fMaximized = t_merge_10bc_0001;
    fMinimized = 0x0;
    fInitalized = 0x0;
    goto L_11ec;

L_10cc:
    pch = 0x57a5;
    i = 0x0;
    goto L_1196;

L_10d9:
    rg[i] = 0x0;
    fNeg = 0x0;
    j = 0x0;
    goto L_1102;

L_10f4:
    j = (j + 0x1);
    pch = (pch + 0x1);

L_1102:
    if ((j >= 0x4))
        goto L_116f;
    else
        goto L_110b;

L_110b:
    if (((uint16_t)(*(pch)) != 0x2d))
        goto L_1121;
    else
        goto L_1119;

L_1119:
    fNeg = 0x1;
    goto L_10f4;

L_1121:
    if (((uint16_t)(*(pch)) < 0x30))
        goto NoRc;
    else
        goto L_112f;

L_112f:
    if (((uint16_t)(*(pch)) > 0x39))
        goto NoRc;
    else
        goto L_113a;

L_113a:

L_1140:
    rg[i] = (LOWORD((0xa * rg[i])) + ((uint16_t)(*(pch)) + 0xffd0));
    goto L_10f4;

L_116f:
    if ((fNeg == 0x0))
        goto L_1192;
    else
        goto L_1178;

L_1178:
    /* untranslated: rg[i] = (rg[i] neg 0x0) */

L_1192:
    i = (i + 0x1);

L_1196:
    if ((i < 0x4))
        goto L_10d9;
    else
        goto L_119f;

L_119f:
    rc.left = LOWORD(rg);
    rc.top = HIWORD(rg);
    /* untranslated: rc.right = part[4:2](rg) */
    /* untranslated: rc.bottom = part[6:2](rg) */
    if (((uint16_t)(LOWORD(szWork)) != 0x4d))
        goto L_11c9;
    else
        goto L_11c3;

L_11c3:
    t_merge_11cc_0001 = 0x1;
    goto L_11cc;

L_11c9:
    t_merge_11cc_0001 = 0x0;

L_11cc:
    fMaximized = t_merge_11cc_0001;
    if (((uint16_t)(LOWORD(szWork)) != 0x49))
        goto L_11e1;
    else
        goto L_11db;

L_11db:
    t_merge_11e4_0001 = 0x1;
    goto L_11e4;

L_11e1:
    t_merge_11e4_0001 = 0x0;

L_11e4:
    fMinimized = t_merge_11e4_0001;
    fInitalized = 0x1;

L_11ec:
    pwn->rc = rc;
    *(pwn + 0x8) = ((*(pwn + 0x8) & 0xfffe) | (fMaximized & 0x1));
    *(pwn + 0x8) = ((*(pwn + 0x8) & 0xfffd) | ((fMinimized & 0x1) << 0x1));
    *(pwn + 0x8) = ((*(pwn + 0x8) & 0xfffb) | ((fInitalized & 0x1) << 0x2));
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
    uint16_t t_merge_1473_0001;
    uint16_t t_merge_1498_0001;
    int16_t  t_merge_14df_0001;
    int16_t  t_merge_1500_0001;
    int16_t  t_merge_1547_0001;
    int16_t  t_merge_1568_0001;
    int16_t  t_merge_15af_0001;
    int16_t  t_merge_15d0_0001;
    int16_t  t_merge_1617_0001;
    int16_t  t_merge_1638_0001;
    int16_t  t_merge_167f_0001;
    int16_t  t_merge_16a0_0001;
    int16_t  t_merge_16e7_0001;
    int16_t  t_merge_1708_0001;
    uint16_t t_merge_174e_0001;
    uint16_t t_1a22;
    uint16_t t_merge_1fc6_0001;
    uint16_t t_merge_2027_0001;
    uint16_t t_merge_208c_0001;
    uint16_t t_merge_2175_0001;
    uint16_t t_merge_2203_0001;
    uint16_t t_merge_2291_0001;
    uint16_t t_merge_231f_0001;
    uint16_t t_merge_2483_0001;
    uint16_t t_merge_2981_0001;

L_124c:
    ini.fGen = 0x0;
    ini.fTry = 0x0;
    ini.fWait = 0x0;
    CchGetString(idsWindows, szSection);
    CchGetString(idsStarsIni, szIniFile);
    GetIniWinRc(szSection, szIniFile, idsMain, 0x2290);
    GetIniWinRc(szSection, szIniFile, idsReportfleetwin, &(wnT));
    if ((wnT.rc.left == 0x8000))
        goto L_12f2;
    else
        goto L_12d3;

L_12d3:
    vrptFleet.ptDlg.x = wnT.rc.left;
    vrptFleet.ptDlg.y = wnT.rc.top;
    vrptFleet.ptSize.x = (wnT.rc.right - wnT.rc.left);
    vrptFleet.ptSize.y = (wnT.rc.bottom - wnT.rc.top);

L_12f2:
    GetIniWinRc(szSection, szIniFile, idsReportefleetwin, &(wnT));
    if ((wnT.rc.left == 0x8000))
        goto L_1333;
    else
        goto L_1314;

L_1314:
    vrptEFleet.ptDlg.x = wnT.rc.left;
    vrptEFleet.ptDlg.y = wnT.rc.top;
    vrptEFleet.ptSize.x = (wnT.rc.right - wnT.rc.left);
    vrptEFleet.ptSize.y = (wnT.rc.bottom - wnT.rc.top);

L_1333:
    GetIniWinRc(szSection, szIniFile, idsReportbtlwin, &(wnT));
    if ((wnT.rc.left == 0x8000))
        goto L_1374;
    else
        goto L_1355;

L_1355:
    vrptBattle.ptDlg.x = wnT.rc.left;
    vrptBattle.ptDlg.y = wnT.rc.top;
    vrptBattle.ptSize.x = (wnT.rc.right - wnT.rc.left);
    vrptBattle.ptSize.y = (wnT.rc.bottom - wnT.rc.top);

L_1374:
    GetIniWinRc(szSection, szIniFile, idsReportplanwin, &(wnT));
    if ((wnT.rc.left == 0x8000))
        goto L_13b5;
    else
        goto L_1396;

L_1396:
    vrptPlanet.ptDlg.x = wnT.rc.left;
    vrptPlanet.ptDlg.y = wnT.rc.top;
    vrptPlanet.ptSize.x = (wnT.rc.right - wnT.rc.left);
    vrptPlanet.ptSize.y = (wnT.rc.bottom - wnT.rc.top);

L_13b5:
    CchGetString(idsResolution, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if ((i != 0x0))
        goto L_142d;
    else
        goto L_13ef;

L_13ef:
    if ((vcScreenColors <= 0x4))
        goto L_140c;
    else
        goto L_13f9;

L_13f9:
    if ((gd.mdScreenSize != 0x0))
        goto L_142d;
    else
        goto L_140c;

L_140c:
    AlertSz(PszFormatIds(idsNoteStarsPrefersScreenResolutionLeast800x600, 0x0), MB_ICONHAND);

L_142d:
    CchGetString(idsLayout, szEntry);
    iWindowLayout = GetPrivateProfileInt(&(szSection), &(szEntry), 0x1, &(szIniFile));
    if ((0x0 <= iWindowLayout))
        goto L_1470;
    else
        goto L_146a;

L_146a:
    t_merge_1473_0001 = 0x0;
    goto L_1473;

L_1470:
    t_merge_1473_0001 = iWindowLayout;

L_1473:
    if ((0x2 >= t_merge_1473_0001))
        goto L_1483;
    else
        goto L_147d;

L_147d:
    t_merge_1498_0001 = 0x2;
    goto L_1498;

L_1483:
    if ((0x0 <= iWindowLayout))
        goto L_1495;
    else
        goto L_148f;

L_148f:
    t_merge_1498_0001 = 0x0;
    goto L_1498;

L_1495:
    t_merge_1498_0001 = iWindowLayout;

L_1498:
    iWindowLayout = t_merge_1498_0001;
    CchGetString(idsStyle1width, szEntry);
    vfs.dxPlanWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0x18c, &(szIniFile));
    if ((vfs.dxPlanWant <= 0xa))
        goto L_14dc;
    else
        goto L_14d6;

L_14d6:
    t_merge_14df_0001 = vfs.dxPlanWant;
    goto L_14df;

L_14dc:
    t_merge_14df_0001 = 0xa;

L_14df:
    if ((t_merge_14df_0001 >= 0x7d0))
        goto L_14fd;
    else
        goto L_14e7;

L_14e7:
    if ((vfs.dxPlanWant <= 0xa))
        goto L_14f7;
    else
        goto L_14f1;

L_14f1:
    t_merge_1500_0001 = vfs.dxPlanWant;
    goto L_1500;

L_14f7:
    t_merge_1500_0001 = 0xa;

L_14fd:
    t_merge_1500_0001 = 0x7d0;

L_1500:
    vfs.dxPlanWant = t_merge_1500_0001;
    CchGetString(idsStyle1height, szEntry);
    vfs.dyMsgWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0x6e, &(szIniFile));
    if ((vfs.dyMsgWant <= 0xa))
        goto L_1544;
    else
        goto L_153e;

L_153e:
    t_merge_1547_0001 = vfs.dyMsgWant;
    goto L_1547;

L_1544:
    t_merge_1547_0001 = 0xa;

L_1547:
    if ((t_merge_1547_0001 >= 0x7d0))
        goto L_1565;
    else
        goto L_154f;

L_154f:
    if ((vfs.dyMsgWant <= 0xa))
        goto L_155f;
    else
        goto L_1559;

L_1559:
    t_merge_1568_0001 = vfs.dyMsgWant;
    goto L_1568;

L_155f:
    t_merge_1568_0001 = 0xa;

L_1565:
    t_merge_1568_0001 = 0x7d0;

L_1568:
    vfs.dyMsgWant = t_merge_1568_0001;
    CchGetString(idsStyle1height2, szEntry);
    vfs.dyMinWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0xc0, &(szIniFile));
    if ((vfs.dyMinWant <= 0xa))
        goto L_15ac;
    else
        goto L_15a6;

L_15a6:
    t_merge_15af_0001 = vfs.dyMinWant;
    goto L_15af;

L_15ac:
    t_merge_15af_0001 = 0xa;

L_15af:
    if ((t_merge_15af_0001 >= 0x7d0))
        goto L_15cd;
    else
        goto L_15b7;

L_15b7:
    if ((vfs.dyMinWant <= 0xa))
        goto L_15c7;
    else
        goto L_15c1;

L_15c1:
    t_merge_15d0_0001 = vfs.dyMinWant;
    goto L_15d0;

L_15c7:
    t_merge_15d0_0001 = 0xa;

L_15cd:
    t_merge_15d0_0001 = 0x7d0;

L_15d0:
    vfs.dyMinWant = t_merge_15d0_0001;
    CchGetString(idsStyle2width, szEntry);
    vfs.dx2PlanWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0x18c, &(szIniFile));
    if ((vfs.dx2PlanWant <= 0xa))
        goto L_1614;
    else
        goto L_160e;

L_160e:
    t_merge_1617_0001 = vfs.dx2PlanWant;
    goto L_1617;

L_1614:
    t_merge_1617_0001 = 0xa;

L_1617:
    if ((t_merge_1617_0001 >= 0x7d0))
        goto L_1635;
    else
        goto L_161f;

L_161f:
    if ((vfs.dx2PlanWant <= 0xa))
        goto L_162f;
    else
        goto L_1629;

L_1629:
    t_merge_1638_0001 = vfs.dx2PlanWant;
    goto L_1638;

L_162f:
    t_merge_1638_0001 = 0xa;

L_1635:
    t_merge_1638_0001 = 0x7d0;

L_1638:
    vfs.dx2PlanWant = t_merge_1638_0001;
    CchGetString(idsStyle2height, szEntry);
    vfs.dy2MsgWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0x6e, &(szIniFile));
    if ((vfs.dy2MsgWant <= 0xa))
        goto L_167c;
    else
        goto L_1676;

L_1676:
    t_merge_167f_0001 = vfs.dy2MsgWant;
    goto L_167f;

L_167c:
    t_merge_167f_0001 = 0xa;

L_167f:
    if ((t_merge_167f_0001 >= 0x7d0))
        goto L_169d;
    else
        goto L_1687;

L_1687:
    if ((vfs.dy2MsgWant <= 0xa))
        goto L_1697;
    else
        goto L_1691;

L_1691:
    t_merge_16a0_0001 = vfs.dy2MsgWant;
    goto L_16a0;

L_1697:
    t_merge_16a0_0001 = 0xa;

L_169d:
    t_merge_16a0_0001 = 0x7d0;

L_16a0:
    vfs.dy2MsgWant = t_merge_16a0_0001;
    CchGetString(idsStyle2height2, szEntry);
    vfs.dy2MinWant = GetPrivateProfileInt(&(szSection), &(szEntry), 0xc0, &(szIniFile));
    if ((vfs.dy2MinWant <= 0xa))
        goto L_16e4;
    else
        goto L_16de;

L_16de:
    t_merge_16e7_0001 = vfs.dy2MinWant;
    goto L_16e7;

L_16e4:
    t_merge_16e7_0001 = 0xa;

L_16e7:
    if ((t_merge_16e7_0001 >= 0x7d0))
        goto L_1705;
    else
        goto L_16ef;

L_16ef:
    if ((vfs.dy2MinWant <= 0xa))
        goto L_16ff;
    else
        goto L_16f9;

L_16f9:
    t_merge_1708_0001 = vfs.dy2MinWant;
    goto L_1708;

L_16ff:
    t_merge_1708_0001 = 0xa;

L_1705:
    t_merge_1708_0001 = 0x7d0;

L_1708:
    vfs.dy2MinWant = t_merge_1708_0001;
    CchGetString(idsToolbar, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x1, &(szIniFile));
    if ((i == 0x0))
        goto L_174b;
    else
        goto L_1745;

L_1745:
    t_merge_174e_0001 = 0x1;
    goto L_174e;

L_174b:
    t_merge_174e_0001 = 0x0;

L_174e:
    HIWORD(gd) = ((HIWORD(gd) & 0x7fff) | ((t_merge_174e_0001 & 0x1) << 0xf));
    CchGetString(idsGlobalsettings, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), " ", szWork, 0x28, &(szIniFile));
    if ((cch == 0x1c))
        goto L_17c3;
    else
        goto L_17b4;

L_17b4:
    vSerialNumber = 0x0;
    goto L_17d7;

L_17c3:
    FSerialAndEnvFromSz(0x8ac, 0x519e, 0x57a4);

L_17d7:
    CchGetString(idsPlanettiles, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), "X", szWork, 0x14, &(szIniFile));
    ReadIniTileSettings(0x57a4, 0x7fc, 0x6);
    CchGetString(idsShiptiles, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), "X", szWork, 0x14, &(szIniFile));
    ReadIniTileSettings(0x57a4, 0x90e, 0x7);
    CchGetString(idsSelection, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), "N", szWork, 0x14, &(szIniFile));
    if ((cch >= 0x3))
        goto L_18d4;
    else
        goto L_18c5;

L_18c5:
    ini.grobjSel = 0x0;
    goto L_1995;

L_18d4:
    goto L_191a;

L_18db:
    ini.grobjSel = 0x0;
    goto L_193d;

L_18ea:
    ini.grobjSel = 0x1;
    goto L_193d;

L_18f9:
    ini.grobjSel = 0x2;
    goto L_193d;

L_1908:
    ini.grobjSel = 0x4;
    goto L_193d;

L_191a:
    if (((uint16_t)(LOWORD(szWork)) == 0x45))
        goto L_1908;
    else
        goto L_1922;

L_1922:
    if (((uint16_t)(LOWORD(szWork)) == 0x4e))
        goto L_18db;
    else
        goto L_192a;

L_192a:
    if (((uint16_t)(LOWORD(szWork)) == 0x50))
        goto L_18ea;
    else
        goto L_1932;

L_1932:
    if (((uint16_t)(LOWORD(szWork)) != 0x53))
        goto L_18db;
    else
        goto L_1937;

L_1937:

L_193d:
    /* untranslated: branch sext8to16(part[1:2](szWork)) < 0x42 ? L_1962 : L_1949 */

L_1949:
    /* untranslated: branch sext8to16(part[1:2](szWork)) > 0x51 ? L_1962 : L_1955 */

L_1955:
    /* untranslated: ini.idPlayer = (sext8to16(part[1:2](szWork)) + 0xffbe) */
    goto L_196e;

L_1962:
    ini.grobjSel = 0x0;

L_196e:
    if ((ini.grobjSel == 0x0))
        goto L_1995;
    else
        goto L_1986;

L_1986:
    ini.iObjSel = atoi(0x57a6);

L_1995:
    CchGetString(idsMessage, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    ini.iMsg = i;
    CchGetString(idsGameid, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), "0", szWork, 0xa, &(szIniFile));
    /* untranslated: part[18:2](ini) = 0x0 */
    /* untranslated: part[20:2](ini) = 0x0 */
    i = 0x0;
    goto L_1aa4;

L_1a1f:
    /* untranslated: t_1a22 = part[18:2](ini) */
    /* untranslated: part[18:2](ini) = loword((int32_t)(words(part[18:2](ini), part[20:2](ini)) * 0x10)) */
    /* untranslated: part[20:2](ini) = hiword((int32_t)(words(t_1a22, part[20:2](ini)) * 0x10)) */
    /* untranslated: branch sext8to16(part[22436:1](i)) < 0x30 ? L_1a6c : L_1a45 */

L_1a45:
    /* untranslated: branch sext8to16(part[22436:1](i)) > 0x39 ? L_1a6c : L_1a55 */

L_1a55:
    /* untranslated: part[18:2](ini) = (part[18:2](ini) + (sext8to16(part[22436:1](i)) + 0xffd0)) */
    /* untranslated: part[20:2](ini) = (part[20:2](ini) + signhiword((sext8to16(part[22436:1](i)) + 0xffd0))) */
    goto L_1aa0;

L_1a6c:
    /* untranslated: branch sext8to16(part[22436:1](i)) < 0x61 ? L_1aa0 : L_1a7c */

L_1a7c:
    /* untranslated: branch sext8to16(part[22436:1](i)) > 0x66 ? L_1aa0 : L_1a8c */

L_1a8c:
    /* untranslated: part[18:2](ini) = (part[18:2](ini) + (sext8to16(part[22436:1](i)) + 0xffa9)) */
    /* untranslated: part[20:2](ini) = (part[20:2](ini) + signhiword((sext8to16(part[22436:1](i)) + 0xffa9))) */

L_1aa0:
    i = (i + 0x1);

L_1aa4:
    if ((i < cch))
        goto L_1a1f;
    else
        goto L_1aaf;

L_1aaf:
    CchGetString(idsScanzoom, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x4, &(szIniFile));
    if ((w < 0x1))
        goto L_1afb;
    else
        goto L_1ae9;

L_1ae9:
    if ((w > 0x9))
        goto L_1afb;
    else
        goto L_1af2;

L_1af2:
    iScanZoom = (w + 0xfffb);

L_1afb:
    CchGetString(idsScanfilterv25, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    grbitScanShip = w;
    CchGetString(idsScanefilterv25, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    grbitScanEShip = w;
    CchGetString(idsScanmines, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0xf, &(szIniFile));
    grbitScanMines = (w & 0xf);
    CchGetString(idsScanradar, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x64, &(szIniFile));
    if ((w < 0x0))
        goto L_1be6;
    else
        goto L_1bdd;

L_1bdd:
    if ((w <= 0x64))
        goto L_1beb;
    else
        goto L_1be6;

L_1be6:
    w = 0x64;

L_1beb:
    vpctRadarView = w;
    CchGetString(idsScanmodev25, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0xe0, &(szIniFile));
    grbitScan = (w & 0xc00f);
    if ((grbitScan < 0x6))
        goto L_1c44;
    else
        goto L_1c35;

L_1c35:
    grbitScan = 0x0;
    grbitScanShip = 0x0;
    goto L_1c4e;

L_1c44:
    grbitScan = (grbitScan | (w & 0x3ff0));

L_1c4e:
    CchGetString(idsMineralscale, szEntry);
    cMinGrafMax = GetPrivateProfileInt(&(szSection), &(szEntry), cMinGrafMax, &(szIniFile));
    if ((cMinGrafMax < 0x64))
        goto L_1c94;
    else
        goto L_1c89;

L_1c89:
    if ((cMinGrafMax <= 0x7530))
        goto L_1c9a;
    else
        goto L_1c94;

L_1c94:
    cMinGrafMax = 0x1388;

L_1c9a:
    CchGetString(idsFiles, szSection);
    CchGetString(idsLogging, szEntry);
    ini.wFlags = ((ini.wFlags & 0x7fff) | ((GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile)) & 0x1) << 0xf));
    CchGetString(idsWait2, szEntry);
    ini.wFlags = ((ini.wFlags & 0xfffb) | ((GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile)) & 0x1) << 0x2));
    CchGetString(idsFile1, szEntry);
    cch = GetPrivateProfileString(&(szSection), &(szEntry), ".", szWork, 0x100, &(szIniFile));
    if ((cch <= 0x3))
        goto L_1da7;
    else
        goto L_1d88;

L_1d88:
    ini.fStartupFile = 0x1;
    strcpy(0x56a2, 0x57a4);
    goto L_1db3;

L_1da7:
    ini.fStartupFile = 0x0;

L_1db3:
    if ((LOWORD(vrgszMRU) != 0x0))
        goto L_1dde;
    else
        goto L_1dbd;

L_1dbd:
    if ((HIWORD(vrgszMRU) != 0x0))
        goto L_1dde;
    else
        goto L_1dc7;

L_1dc7:
    vrgszMRU = LpAlloc(0x900, htPerm);

L_1dde:
    psz = (szEntry[strlen(szEntry)] + 0xffff);
    i = 0x0;
    goto L_1e6b;

L_1dff:
    *(psz) = LOBYTE((i + 0x31));
    cch = GetPrivateProfileString(&(szSection), &(szEntry), ".", ((uint8_t *)(vrgszMRU) + (i * 0x100)), 0x100, &(szIniFile));
    if ((cch >= 0x4))
        goto L_1e67;
    else
        goto L_1e4f;

L_1e4f:
    /* untranslated: byte HIWORD(vrgszMRU):[(LOWORD(vrgszMRU) + (i * 0x100))] = 0x0 */

L_1e67:
    i = (i + 0x1);

L_1e6b:
    if ((i < 0x9))
        goto L_1dff;
    else
        goto L_1e74;

L_1e74:
    iPass = 0x0;
    i = 0x0;
    goto L_1efd;

L_1e82:
    /* untranslated: branch sext8to16(byte HIWORD(vrgszMRU):[(LOWORD(vrgszMRU) + (i * 0x100))]) == 0x0 ? L_1ef9 : L_1ea2 */

L_1ea2:
    if ((i == iPass))
        goto L_1ef5;
    else
        goto L_1ead;

L_1ead:
    fstrcpy(((uint8_t *)(vrgszMRU) + (iPass * 0x100)), ((uint8_t *)(vrgszMRU) + (i * 0x100)));
    /* untranslated: byte HIWORD(vrgszMRU):[(LOWORD(vrgszMRU) + (i * 0x100))] = 0x0 */

L_1ef5:
    iPass = (iPass + 0x1);

L_1ef9:
    i = (i + 0x1);

L_1efd:
    if ((i < 0x9))
        goto L_1e82;
    else
        goto L_1f06;

L_1f06:
    CchGetString(idsTurn, szEntry);
    ini.turn = GetPrivateProfileInt(&(szSection), &(szEntry), game.turn, &(szIniFile));
    CchGetString(idsMisc, szSection);
    CchGetString(idsDefaultpassword, szEntry);
    GetPrivateProfileString(&(szSection), &(szEntry), "", vszDefPass, 0x10, &(szIniFile));
    CchGetString(idsProgress, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if ((w == 0x0))
        goto L_1fc3;
    else
        goto L_1fbd;

L_1fbd:
    t_merge_1fc6_0001 = 0x1;
    goto L_1fc6;

L_1fc3:
    t_merge_1fc6_0001 = 0x0;

L_1fc6:
    HIWORD(gd) = ((HIWORD(gd) & 0xfbff) | ((t_merge_1fc6_0001 & 0x1) << 0xa));
    CchGetString(idsNewreports, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if ((w == 0x0))
        goto L_2024;
    else
        goto L_201e;

L_201e:
    t_merge_2027_0001 = 0x1;
    goto L_2027;

L_2024:
    t_merge_2027_0001 = 0x0;

L_2027:
    /* untranslated: part[6:2](gd) = ((part[6:2](gd) & 0xffdf) | ((t_merge_2027_0001 & 0x1) << 0x5)) */
    CchGetString(idsNohostnames, szEntry);
    w = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if ((w == 0x0))
        goto L_2089;
    else
        goto L_2083;

L_2083:
    t_merge_208c_0001 = 0x1;
    goto L_208c;

L_2089:
    t_merge_208c_0001 = 0x0;

L_208c:
    /* untranslated: part[6:2](gd) = ((part[6:2](gd) & 0xffbf) | ((t_merge_208c_0001 & 0x1) << 0x6)) */
    CchGetString(idsBackups, szEntry);
    vcBackupDirs = GetPrivateProfileInt(&(szSection), &(szEntry), 0x1, &(szIniFile));
    if ((vcBackupDirs < 0x1))
        goto L_20ef;
    else
        goto L_20e4;

L_20e4:
    if ((vcBackupDirs <= 0x3e7))
        goto L_20f5;
    else
        goto L_20ef;

L_20ef:
    vcBackupDirs = 0x1;

L_20f5:
    CchGetString(idsReportplanfld, szEntry);
    LOWORD(vrptPlanet) = GetPrivateProfileInt(&(szSection), &(szEntry), 0xffff, &(szIniFile));
    HIWORD(vrptPlanet) = 0x0;
    CchGetString(idsReportplansort, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if (((i & 0x100) == 0x0))
        goto L_2172;
    else
        goto L_216c;

L_216c:
    t_merge_2175_0001 = 0x1;
    goto L_2175;

L_2172:
    t_merge_2175_0001 = 0x0;

L_2175:
    vrptPlanet.fAscending = t_merge_2175_0001;
    i = (i & 0xff);
    vrptPlanet.icolSort = i;
    CchGetString(idsReportfleetfld, szEntry);
    LOWORD(vrptFleet) = GetPrivateProfileInt(&(szSection), &(szEntry), 0xffff, &(szIniFile));
    HIWORD(vrptFleet) = 0x0;
    CchGetString(idsReportfleetsort, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if (((i & 0x100) == 0x0))
        goto L_2200;
    else
        goto L_21fa;

L_21fa:
    t_merge_2203_0001 = 0x1;
    goto L_2203;

L_2200:
    t_merge_2203_0001 = 0x0;

L_2203:
    vrptFleet.fAscending = t_merge_2203_0001;
    i = (i & 0xff);
    vrptFleet.icolSort = i;
    CchGetString(idsReportefleetfld, szEntry);
    LOWORD(vrptEFleet) = GetPrivateProfileInt(&(szSection), &(szEntry), 0xffff, &(szIniFile));
    HIWORD(vrptEFleet) = 0x0;
    CchGetString(idsReportefltsort, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if (((i & 0x100) == 0x0))
        goto L_228e;
    else
        goto L_2288;

L_2288:
    t_merge_2291_0001 = 0x1;
    goto L_2291;

L_228e:
    t_merge_2291_0001 = 0x0;

L_2291:
    vrptEFleet.fAscending = t_merge_2291_0001;
    i = (i & 0xff);
    vrptEFleet.icolSort = i;
    CchGetString(idsReportbtlfld, szEntry);
    LOWORD(vrptBattle) = GetPrivateProfileInt(&(szSection), &(szEntry), 0xffff, &(szIniFile));
    HIWORD(vrptBattle) = 0x0;
    CchGetString(idsReportbtlsort, szEntry);
    i = GetPrivateProfileInt(&(szSection), &(szEntry), 0x0, &(szIniFile));
    if (((i & 0x100) == 0x0))
        goto L_231c;
    else
        goto L_2316;

L_2316:
    t_merge_231f_0001 = 0x1;
    goto L_231f;

L_231c:
    t_merge_231f_0001 = 0x0;

L_231f:
    vrptBattle.fAscending = t_merge_231f_0001;
    i = (i & 0xff);
    vrptBattle.icolSort = i;
    CchGetString(idsReportdefgraph, szEntry);
    /* untranslated: part[6:2](gd) = ((part[6:2](gd) & 0xfff0) | (GetPrivateProfileInt(&szSection, &szEntry, 0x7, &szIniFile) & 0xf)) */
    CchGetString(idsReportefltsort, szEntry);
    if ((gd.iCurGraph <= 0x7))
        goto L_239d;
    else
        goto L_2391;

L_2391:
    gd.iCurGraph = 0x7;

L_239d:
    CchGetString(idsVcrspeed, szEntry);
    viSpeedVCR = GetPrivateProfileInt(&(szSection), &(szEntry), 0x1, &(szIniFile));
    strdate(0x57a4);
    /* untranslated: part[5:1](szWork) = 0x0 */
    HIWORD(szWork) = 0x0;
    /* untranslated: ss:[bp-0x4a] = loword((loword((atoi(0x57aa) * 0x1f)) * 0xc)) */
    /* untranslated: ss:[bp-0x4c] = loword((atoi(0x57a4) * 0x1f)) */
    /* untranslated: uDateCur = ((atoi(0x57a7) + ss:[bp-0x4c]) + ss:[bp-0x4a]) */
    CchGetString(idsHistoryinfo, szEntry);
    uDateInstalled = GetPrivateProfileInt(&(szSection), &(szEntry), 0xffff, &(szIniFile));
    if ((uDateCur >= uDateInstalled))
        goto L_246c;
    else
        goto L_2466;

L_2466:
    uDateInstalled = uDateCur;

L_246c:
    if ((uDateCur < (uDateInstalled + 0x15)))
        goto L_2480;
    else
        goto L_247a;

L_247a:
    t_merge_2483_0001 = 0x1;
    goto L_2483;

L_2480:
    t_merge_2483_0001 = 0x0;

L_2483:
    /* untranslated: part[4:2](gd) = ((part[4:2](gd) & 0xfffd) | ((t_merge_2483_0001 & 0x1) << 0x1)) */
    CchGetString(idsFonts, szSection);
    i = 0x0;
    goto L_253b;

L_24b5:
    CchGetString((i + 0xc5), szEntry);
    GetPrivateProfileString(&(szSection), &(szEntry), "", szWork, 0x50, &(szIniFile));
    cch = strlen(0x57a4);
    if ((cch < 0x5))
        goto L_2537;
    else
        goto L_250c;

L_250c:
    if ((cch > 0x1f))
        goto L_2537;
    else
        goto L_2512;

L_2512:

L_2518:
    strcpy(rgszArial[i], 0x57a4);

L_2537:
    i = (i + 0x1);

L_253b:
    if ((i < 0x4))
        goto L_24b5;
    else
        goto L_2544;

L_2544:
    CchGetString(idsZiporders, szSection);
    memset(0x5264, 0x0, 0x60);
    i = 0x0;
    goto L_27fd;

L_2570:
    strcpy(szEntry, szSection);
    psz = szEntry[strlen(szEntry)];
    *(psz) = LOBYTE((i + 0x31));
    psz[0x1] = 0x0;
    GetPrivateProfileString(&(szSection), &(szEntry), "", szWork, 0x50, &(szIniFile));
    cch = strlen(0x57a4);
    if ((cch < 0x14))
        goto L_27f9;
    else
        goto L_25ea;

L_25ea:
    if ((cch > 0x20))
        goto L_27f9;
    else
        goto L_25f0;

L_25f0:

L_25f6:
    psz = 0x57a4;
    iPass = 0x0;
    goto L_2611;

L_2603:
    iPass = (iPass + 0x1);
    psz = (psz + 0x1);

L_2611:
    if ((iPass >= 0x14))
        goto L_263c;
    else
        goto L_261a;

L_261a:
    if (((uint16_t)(*(psz)) < 0x61))
        goto L_263c;
    else
        goto L_2628;

L_2628:
    if (((uint16_t)(*(psz)) > 0x70))
        goto L_263c;
    else
        goto L_2633;

L_2633:

L_263c:
    if ((iPass < 0x14))
        goto L_27f9;
    else
        goto L_2642;

L_2642:

L_2648:
    psz = 0x57a4;
    iPass = 0x0;
    goto L_27c5;

L_2655:
    vrgZip[i].txp.rgia[iPass] = ((vrgZip[i].txp.rgia[iPass] & 0xfff) | ((((uint16_t)(*(psz)) + 0xff9f) & 0xf) * 0x1000));
    psz = (psz + 0x1);
    vrgZip[i].txp.rgia[iPass] = ((vrgZip[i].txp.rgia[iPass] & 0xf000) | (((uint16_t)(*(psz)) + 0xff9f) & 0xfff));
    psz = (psz + 0x1);
    /* untranslated: ss:[bp-0x4a] = ((((sext8to16(*psz) + 0xff9f) * 0x10) | vrgZip[i].txp.rgia[iPass]) & 0xfff) */
    vrgZip[i].txp.rgia[iPass] = (vrgZip[i].txp.rgia[iPass] & 0xf000);
    /* untranslated: vrgZip[i].txp.rgia[iPass] = (vrgZip[i].txp.rgia[iPass] | ss:[bp-0x4a]) */
    psz = (psz + 0x1);
    /* untranslated: ss:[bp-0x4a] = ((((sext8to16(*psz) + 0xff9f) * 0x100) | vrgZip[i].txp.rgia[iPass]) & 0xfff) */
    vrgZip[i].txp.rgia[iPass] = (vrgZip[i].txp.rgia[iPass] & 0xf000);
    /* untranslated: vrgZip[i].txp.rgia[iPass] = (vrgZip[i].txp.rgia[iPass] | ss:[bp-0x4a]) */
    psz = (psz + 0x1);
    iPass = (iPass + 0x1);

L_27c5:
    if ((iPass < 0x5))
        goto L_2655;
    else
        goto L_27ce;

L_27ce:
    /* untranslated: call strcpy(part[10:0](vrgZip[i]), psz) -> callresult(char *) */
    vrgZip[i].fValid = 0x1;

L_27f9:
    i = (i + 0x1);

L_27fd:
    if ((i < 0x4))
        goto L_2570;
    else
        goto L_2806;

L_2806:
    memset(0x22f6, 0x0, 0xc8);
    i = 0x0;
    goto L_2b65;

L_2822:
    strcpy(szEntry, szSection);
    psz = szEntry[strlen(szEntry)];
    psz = (psz + 0x1);
    *(psz) = 0x50;
    *(psz) = LOBYTE((i + 0x31));
    psz[0x1] = 0x0;
    GetPrivateProfileString(&(szSection), &(szEntry), "", szWork, 0x50, &(szIniFile));
    cch = strlen(0x57a4);
    if ((cch < 0x3))
        goto L_2b61;
    else
        goto L_28a6;

L_28a6:
    if ((cch > 0x40))
        goto L_2b61;
    else
        goto L_28ac;

L_28ac:

L_28b2:
    psz = 0x57a4;
    cpq = ((uint16_t)(psz[0x1]) + 0xff9f);
    if ((cpq < 0x0))
        goto L_2b61;
    else
        goto L_28cd;

L_28cd:
    if ((cpq > 0xc))
        goto L_2b61;
    else
        goto L_28d3;

L_28d3:

L_28d9:
    iPass = 0x0;
    goto L_28ef;

L_28e1:
    iPass = (iPass + 0x1);
    psz = (psz + 0x1);

L_28ef:
    if ((iPass >= ((cpq * 0x4) + 0x2)))
        goto L_2923;
    else
        goto L_2901;

L_2901:
    if (((uint16_t)(*(psz)) < 0x61))
        goto L_2923;
    else
        goto L_290f;

L_290f:
    if (((uint16_t)(*(psz)) > 0x70))
        goto L_2923;
    else
        goto L_291a;

L_291a:

L_2923:
    if ((iPass < ((cpq * 0x4) + 0x2)))
        goto L_2b61;
    else
        goto L_2932;

L_2932:

L_2938:
    if ((strlen(psz) > 0xc))
        goto L_2b61;
    else
        goto L_2948;

L_2948:

L_294e:
    strcpy(vrgZipProd[i], psz);
    psz = 0x57a4;
    if (((uint16_t)(*(psz)) == 0x61))
        goto L_297e;
    else
        goto L_2978;

L_2978:
    t_merge_2981_0001 = 0x1;
    goto L_2981;

L_297e:
    t_merge_2981_0001 = 0x0;

L_2981:
    /* untranslated: part[14:1](vrgZipProd[i]) = lobyte(t_merge_2981_0001) */
    vrgZipProd[i].fValid = 0x1;
    /* untranslated: part[15:1](vrgZipProd[i]) = lobyte(cpq) */
    psz = (psz + 0x2);
    iPass = 0x0;
    goto L_2b56;

L_29c7:
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = (sext8to16(*psz) + 0xff9f) */
    psz = (psz + 0x1);
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = (0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] | ((sext8to16(*psz) + 0xff9f) *
     * 0x10)) */
    psz = (psz + 0x1);
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = (0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] | ((sext8to16(*psz) + 0xff9f) *
     * 0x100)) */
    psz = (psz + 0x1);
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = (0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] | ((sext8to16(*psz) + 0xff9f) *
     * 0x1000)) */
    psz = (psz + 0x1);
    /* untranslated: branch ((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] >> 0x6) & 0x3ff) <= 0x3fc ? L_2af0 : L_2ab2 */

L_2ab2:
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = ((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] & 0x3f) | 0x40) */

L_2af0:
    /* untranslated: branch (0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] & 0x3f) < 0x7 ? L_2b52 : L_2b14 */

L_2b14:
    /* untranslated: 0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] = ((0x25:[(part[16:0](vrgZipProd[i]) + (iPass * 0x2))] & 0xffc0) | 0x0) */

L_2b52:
    iPass = (iPass + 0x1);

L_2b56:
    if ((iPass < cpq))
        goto L_29c7;
    else
        goto L_2b61;

L_2b61:
    i = (i + 0x1);

L_2b65:
    if ((i < 0x5))
        goto L_2822;
    else
        goto L_2b6e;

L_2b6e:
    CchGetString(idsDefault, 0x22f6);
    /* untranslated: part[13:1](vrgZipProd) = 0x1 */
    return;
}

void ReadIniTileSettings(char *pszFormat, TILE *rgtile, int16_t ctile) {
    TILE     tile;
    int16_t  fPopped;
    int16_t  i;
    int16_t  iTile;
    uint16_t iCol;
    uint16_t iBit;
    uint16_t t_merge_2c1b_0001;

L_2b8a:
    iCol = 0x0;
    iTile = 0x0;

L_2b9d:
    if (((uint16_t)(*(pszFormat)) == 0x0))
        goto L_2d81;
    else
        goto L_2bab;

L_2bab:
    if (((uint16_t)(*(pszFormat)) != 0x2a))
        goto L_2bc9;
    else
        goto L_2bb9;

L_2bb9:
    if ((iCol >= 0x1))
        goto DoNext;
    else
        goto L_2bc2;

L_2bc2:
    iCol = (iCol + 0x1);

L_2bc9:
    /* untranslated: branch (byte 0x25:[(0x175f + sext8to16(*pszFormat))] & 0x3) == 0x0 ? L_2d7a : L_2be1 */

L_2be1:

L_2be7:
    /* untranslated: fPopped = (byte 0x25:[(0x175f + sext8to16(*pszFormat))] & 0x1) */
    if ((fPopped == 0x0))
        goto L_2c12;
    else
        goto L_2c06;

L_2c06:
    t_merge_2c1b_0001 = ((uint16_t)(*(pszFormat)) + 0xffbf);
    goto L_2c1b;

L_2c12:
    t_merge_2c1b_0001 = ((uint16_t)(*(pszFormat)) + 0xff9f);

L_2c1b:
    iBit = t_merge_2c1b_0001;
    if ((iBit < 0x0))
        goto DoNext;
    else
        goto L_2c26;

L_2c26:

L_2c2c:
    i = iTile;
    goto L_2c60;

L_2c35:
    /* untranslated: branch ((part[10:2](rgtile[i]) >> 0x3) & 0xf) == iBit ? L_2c6b : L_2c56 */

L_2c56:

L_2c5c:
    i = (i + 0x1);

L_2c60:
    if ((i < ctile))
        goto L_2c35;
    else
        goto L_2c6b;

L_2c6b:
    if ((i == ctile))
        goto DoNext;
    else
        goto L_2c73;

L_2c73:

L_2c79:
    /* untranslated: part[10:2](rgtile[i]) = ((part[10:2](rgtile[i]) & 0xfff8) | (iCol & 0x7)) */
    /* untranslated: part[10:2](rgtile[i]) = ((part[10:2](rgtile[i]) & 0xff7f) | ((fPopped & 0x1) << 0x7)) */
    if ((i == iTile))
        goto L_2d76;
    else
        goto L_2cec;

L_2cec:
    tile = rgtile[i];
    rgtile[i] = rgtile[iTile];
    rgtile[iTile] = tile;

L_2d76:
    iTile = (iTile + 0x1);

DoNext:
    pszFormat = (pszFormat + 0x1);
    goto L_2b9d;

L_2d81:
    i = iTile;
    goto L_2ddd;

L_2d8a:
    /* untranslated: branch (part[10:2](rgtile[i]) & 0x7) >= iCol ? L_2dd9 : L_2da8 */

L_2da8:
    /* untranslated: part[10:2](rgtile[i]) = ((part[10:2](rgtile[i]) & 0xfff8) | (iCol & 0x7)) */

L_2dd9:
    i = (i + 0x1);

L_2ddd:
    if ((i < ctile))
        goto L_2d8a;
    else
        goto L_2de8;

L_2de8:
    return;
}
