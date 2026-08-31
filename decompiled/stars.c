#include "common.h"

int16_t WinMain(uint16_t hInstance, uint16_t hPrevInstance, char *lpCmdLine, int16_t nCmdShow) {
    char    *pch;
    char    *lpT;
    int16_t  i;
    MSG      msg;
    uint16_t t_merge_03ec_0001;

L_0000:
    hInst = hInstance;
    szBase[0x0] = 0x0;
    ini.wFlags = 0x0;
    memset(&(tutor), 0x0, 0x2c);
    memset(&(vtimer), 0x0, 0xa);
    vtimer.fAutoGenWhenIn = 0x1;
    if ((hPrevInstance != 0x0))
        goto L_0085;
    else
        goto L_0051;

L_0051:
    if ((InitMDIApp() != 0x0))
        goto L_0085;
    else
        goto L_005e;

L_005e:
    AlertSz(PszFormatIds(idsUnableInitializeStars, 0x0), MB_ICONHAND);
    return 0x0;

L_0085:
    Randomize2(GetTickCount());
    if ((FCreateStuff() != 0x0))
        goto L_00a7;
    else
        goto L_00a1;

L_00a1:
    return 0x0;

L_00a7:
    if ((FGetSystemColors() != 0x0))
        goto L_00db;
    else
        goto L_00b4;

L_00b4:
    AlertSz(PszFormatIds(idsUnableInitializeStars, 0x0), MB_ICONHAND);
    return 0x0;

L_00db:
    if ((InitInstance(nCmdShow) != 0x0))
        goto L_0115;
    else
        goto L_00ee;

L_00ee:
    AlertSz(PszFormatIds(idsUnableInitializeStars, 0x0), MB_ICONHAND);
    return 0x0;

L_0115:
    lpT = lpCmdLine;

L_0121:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0571;
    else
        goto L_0130;

L_0130:
    if (((uint16_t)(*(lpT)) != 0x20))
        goto L_0146;
    else
        goto L_013f;

L_013f:
    lpT = (lpT + 0x1);
    goto L_0130;

L_0146:
    if (((uint16_t)(*(lpT)) == 0x2d))
        goto L_0164;
    else
        goto L_0155;

L_0155:
    if (((uint16_t)(*(lpT)) != 0x2f))
        goto L_0517;
    else
        goto L_0164;

L_0164:
    lpT = (lpT + 0x1);

L_0168:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0121;
    else
        goto L_0177;

L_0177:
    if (((uint16_t)(*(lpT)) == 0x20))
        goto L_0121;
    else
        goto L_0186;

L_0186:
    goto L_0489;

L_0190:
    goto L_050d;
    ini.fWait = 0x1;
    goto L_050d;
    lpT = (lpT + 0x1);

L_01a6:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0238;
    else
        goto L_01b5;

L_01b5:
    if (((uint16_t)(*(lpT)) == 0x20))
        goto L_0238;
    else
        goto L_01c4;

L_01c4:
    goto L_01fe;

L_01ce:
    ini.fDumpFleets = 0x1;
    goto L_0231;

L_01dd:
    ini.fDumpPlanets = 0x1;
    goto L_0231;

L_01ec:
    ini.fDumpMap = 0x1;
    goto L_0231;

L_01fe:
    if (((uint16_t)(*(lpT)) == 0x46))
        goto L_01ce;
    else
        goto L_0206;

L_0206:
    if (((uint16_t)(*(lpT)) == 0x4d))
        goto L_01ec;
    else
        goto L_020e;

L_020e:
    if (((uint16_t)(*(lpT)) == 0x50))
        goto L_01dd;
    else
        goto L_0216;

L_0216:
    if (((uint16_t)(*(lpT)) == 0x66))
        goto L_01ce;
    else
        goto L_021e;

L_021e:
    if (((uint16_t)(*(lpT)) == 0x6d))
        goto L_01ec;
    else
        goto L_0226;

L_0226:
    if (((uint16_t)(*(lpT)) == 0x70))
        goto L_01dd;
    else
        goto L_0231;

L_0231:
    lpT = (lpT + 0x1);
    goto L_01a6;

L_0238:
    lpT = (lpT - 0x1);
    goto L_050d;
    ini.fGen = 0x1;
    i = 0x0;

L_0250:
    if (((uint16_t)(lpT[0x1]) < 0x30))
        goto L_02cb;
    else
        goto L_0260;

L_0260:
    if (((uint16_t)(lpT[0x1]) > 0x39))
        goto L_02cb;
    else
        goto L_0270;

L_0270:
    lpT = (lpT + 0x1);
    i = ((LOWORD((0xa * i)) + (uint16_t)(*(lpT))) + 0xffd0);
    if ((i <= 0x3e8))
        goto L_0250;
    else
        goto L_0299;

L_0299:
    i = 0x3e8;

L_029e:
    if (((uint16_t)(lpT[0x1]) < 0x30))
        goto L_02cb;
    else
        goto L_02ae;

L_02ae:
    if (((uint16_t)(lpT[0x1]) > 0x39))
        goto L_02cb;
    else
        goto L_02be;

L_02be:
    lpT = (lpT + 0x1);
    goto L_029e;

L_02cb:
    if ((i <= 0x0))
        goto L_02dd;
    else
        goto L_02d4;

L_02d4:
    ini.cTurnGen = (i + 0xffff);

L_02dd:
    goto L_050d;
    ini.fNewGame = 0x1;
    goto L_050d;
    gd.fHotSeat = 0x1;
    goto L_050d;
    gd.fExitWindows = 0x1;
    goto L_050d;
    lpT = (lpT + 0x1);

L_0311:
    if (((uint16_t)(*(lpT)) != 0x20))
        goto L_0327;
    else
        goto L_0320;

L_0320:
    lpT = (lpT + 0x1);
    goto L_0311;

L_0327:
    pch = 0x56a2;

L_032c:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0360;
    else
        goto L_033b;

L_033b:
    if (((uint16_t)(*(lpT)) == 0x20))
        goto L_0360;
    else
        goto L_034a;

L_034a:
    *(pch) = *(lpT);
    pch = (pch + 0x1);
    lpT = (lpT + 0x1);
    goto L_032c;

L_0360:
    *(pch) = 0x0;
    lpT = (lpT - 0x1);
    if ((FSetUpBatchProcessing() == 0x0))
        goto L_03a7;
    else
        goto L_0377;

L_0377:
    ini.fBatch = 0x1;
    ini.fGen = 0x1;
    ini.fStartupFile = 0x1;
    ini.fCmdLine = 0x1;

L_03a7:
    goto L_050d;
    ini.fValidate = 0x1;
    goto L_050d;
    ini.fLogging = 0x1;
    goto L_050d;
    ini.fTry = 0x1;
    goto L_050d;
    if (((uint16_t)(LOWORD(szBase)) == 0x0))
        goto L_03e9;
    else
        goto L_03e3;

L_03e3:
    t_merge_03ec_0001 = 0x1;
    goto L_03ec;

L_03e9:
    t_merge_03ec_0001 = 0x0;

L_03ec:
    ini.wFlags = ((ini.wFlags & 0xfffd) | ((t_merge_03ec_0001 & 0x1) << 0x1));
    goto L_050d;
    lpT = (lpT + 0x1);

L_040d:
    if (((uint16_t)(*(lpT)) != 0x20))
        goto L_0423;
    else
        goto L_041c;

L_041c:
    lpT = (lpT + 0x1);
    goto L_040d;

L_0423:
    pch = 0x22e0;

L_0428:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0466;
    else
        goto L_0437;

L_0437:
    if (((uint16_t)(*(lpT)) == 0x20))
        goto L_0466;
    else
        goto L_0446;

L_0446:
    if ((pch >= 0x22ef))
        goto L_0466;
    else
        goto L_0450;

L_0450:
    *(pch) = *(lpT);
    pch = (pch + 0x1);
    lpT = (lpT + 0x1);
    goto L_0428;

L_0466:
    *(pch) = 0x0;
    lpT = (lpT - 0x1);
    lSaltLast = LSaltFromSz(szPassLast);
    goto L_050d;

L_0489:
    if ((((uint16_t)(*(lpT)) - 0x41) > 0x37))
        goto L_0190;
    else
        goto L_0494;

L_0494:
    goto L_ffffffff;

L_050d:
    lpT = (lpT + 0x1);
    goto L_0168;

L_0517:
    pch = 0x56a2;

L_051c:
    if (((uint16_t)(*(lpT)) == 0x0))
        goto L_0550;
    else
        goto L_052b;

L_052b:
    if (((uint16_t)(*(lpT)) == 0x20))
        goto L_0550;
    else
        goto L_053a;

L_053a:
    *(pch) = *(lpT);
    lpT = (lpT + 0x1);
    pch = (pch + 0x1);
    goto L_051c;

L_0550:
    *(pch) = 0x0;
    ini.fStartupFile = 0x1;
    ini.fCmdLine = 0x1;

L_0571:
    PostMessage(hwndFrame, WM_STARS_STARTUP, 0x0, 0x0);

L_058a:
    if ((GetMessage(&(msg), 0x0, 0x0, 0x0) == 0x0))
        goto L_0690;
    else
        goto L_05aa;

L_05aa:
    if ((hwndTitle == 0x0))
        goto L_05eb;
    else
        goto L_05b4;

L_05b4:
    if ((TranslateAccelerator(hwndFrame, hAccelTitle, &(msg)) != 0x0))
        goto L_058a;
    else
        goto L_05d0;

L_05d0:
    TranslateMessage(&(msg));
    DispatchMessage(&(msg));

L_05eb:
    if ((IsIconic(hwndFrame) != 0x0))
        goto L_0618;
    else
        goto L_05fc;

L_05fc:
    if ((TranslateAccelerator(hwndFrame, hAccel, &(msg)) != 0x0))
        goto L_058a;
    else
        goto L_0618;

L_0618:
    TranslateMessage(&(msg));
    if ((msg.message == 0x100))
        goto L_0638;
    else
        goto L_062e;

L_062e:
    if ((msg.message != 0x101))
        goto L_065b;
    else
        goto L_0638;

L_0638:
    if ((FHandleKey(msg.hwnd, msg.message, msg.wParam, msg.lParam) != 0x0))
        goto L_058a;
    else
        goto L_065b;

L_065b:
    if ((msg.message != 0x102))
        goto L_0681;
    else
        goto L_0665;

L_0665:
    if ((FHandleChar(msg.hwnd, msg.wParam, msg.lParam) != 0x0))
        goto L_058a;
    else
        goto L_0681;

L_0681:
    DispatchMessage(&(msg));

L_0690:
    FreeStuff();
    return msg.wParam;
}

int16_t FSetUpBatchProcessing() {
    char   *pch;
    jmp_buf env[9];
    int16_t fSuccess;
    int16_t cb;

L_06a4:
    fSuccess = 0x0;
    penvMem = env;
    if ((setjmp(env) != 0x0))
        goto LError;
    else
        goto L_06c9;

L_06c9:

L_06cf:
    StreamOpen(szBase, 0x20);
    cb = LOWORD(filelength(hf));
    lpchBatch = LpAlloc(cb, htPerm);
    RgFromStream(&(lpchBatch), cb);
    lpchBatchMac = &(lpchBatch[cb]);
    pch = 0x56a2;

L_0733:
    if (((uint16_t)(*(lpchBatch)) == 0xa))
        goto L_0774;
    else
        goto L_0743;

L_0743:
    if ((LOWORD(lpchBatch) != LOWORD(lpchBatchMac)))
        goto L_075c;
    else
        goto L_0753;

L_0753:
    if ((HIWORD(lpchBatch) == HIWORD(lpchBatchMac)))
        goto L_0774;
    else
        goto L_075c;

L_075c:
    *(pch) = *(lpchBatch);
    lpchBatch = (lpchBatch + 0x1);
    pch = (pch + 0x1);
    goto L_0733;

L_0774:
    lpchBatch = (lpchBatch + 0x1);
    *(pch - 0x1) = 0x0;
    fSuccess = 0x1;

LError:
    penvMem = 0x0;
    StreamClose();
    if ((fSuccess != 0x0))
        goto L_079e;
    else
        goto L_0799;

L_0799:
    szBase[0x0] = 0x0;

L_079e:

L_07a4:
    return fSuccess;
}

int16_t IPlrAlsoCheater(int16_t iplr) {
    int16_t i;

L_07aa:
    if ((FValidSerialLong(vrgts[iplr].lSerialNumber) != 0x0))
        goto L_07e9;
    else
        goto L_07e3;

L_07e3:
    return 0xffff;

L_07e9:
    i = 0x0;
    goto L_07f5;

L_07f1:
    i = (i + 0x1);

L_07f5:
    if ((i >= game.cPlayer))
        goto L_08c5;
    else
        goto L_0800;

L_0800:
    if ((i == iplr))
        goto L_07f1;
    else
        goto L_080b;

L_080b:
    if ((((rgplr[i].wFlags >> 0x2) & 0x1) == 0x0))
        goto L_07f1;
    else
        goto L_0828;

L_0828:
    if ((LOWORD(vrgts[iplr].lSerialNumber) != LOWORD(vrgts[i].lSerialNumber)))
        goto L_07f1;
    else
        goto L_0867;

L_0867:
    if ((HIWORD(vrgts[iplr].lSerialNumber) != HIWORD(vrgts[i].lSerialNumber)))
        goto L_07f1;
    else
        goto L_0870;

L_0870:
    if ((fmemcmp(&(vrgts[iplr].rgbConfig), &(vrgts[i].rgbConfig), 0xb) == 0x0))
        goto L_07f1;
    else
        goto L_08bc;

L_08bc:
    return i;

L_08c5:
    return 0xffff;
}

int16_t FGetSystemColors() {
    HDC         hdc;
    BITMAPINFO *lpbi;

L_08d2:
    if ((hbrButtonFace == 0x0))
        goto L_08f1;
    else
        goto L_08e5;

L_08e5:
    FreeHbr(hbrButtonFace);

L_08f1:
    if ((hbrButtonHilite == 0x0))
        goto L_0907;
    else
        goto L_08fb;

L_08fb:
    FreeHbr(hbrButtonHilite);

L_0907:
    if ((hbrButtonShadow == 0x0))
        goto L_091d;
    else
        goto L_0911;

L_0911:
    FreeHbr(hbrButtonShadow);

L_091d:
    if ((hbrButtonText == 0x0))
        goto L_0933;
    else
        goto L_0927;

L_0927:
    FreeHbr(hbrButtonText);

L_0933:
    if ((hbrWindowText == 0x0))
        goto L_0949;
    else
        goto L_093d;

L_093d:
    FreeHbr(hbrWindowText);

L_0949:
    if ((hbrWindow == 0x0))
        goto L_095f;
    else
        goto L_0953;

L_0953:
    FreeHbr(hbrWindow);

L_095f:
    if ((hbrWindowFrame == 0x0))
        goto L_0975;
    else
        goto L_0969;

L_0969:
    FreeHbr(hbrWindowFrame);

L_0975:
    if ((hbrDesktop == 0x0))
        goto L_098b;
    else
        goto L_097f;

L_097f:
    FreeHbr(hbrDesktop);

L_098b:
    crButtonFace = GetSysColor(0xf);
    hbrButtonFace = HbrGet(crButtonFace);
    crButtonHilite = GetSysColor(0x14);
    hbrButtonHilite = HbrGet(crButtonHilite);
    crButtonShadow = GetSysColor(0x10);
    hbrButtonShadow = HbrGet(crButtonShadow);
    crButtonText = GetSysColor(0x12);
    hbrButtonText = HbrGet(crButtonText);
    hbrWindowFrame = HbrGet(GetSysColor(0x6));
    hbrDesktop = HbrGet(GetSysColor(0x1));
    crWindow = GetSysColor(0x5);
    hbrWindow = HbrGet(crWindow);
    crWindowText = GetSysColor(0x8);
    hbrWindowText = HbrGet(crWindowText);
    dyTitleBar = GetSystemMetrics(SM_CYCAPTION);
    dxWinFrame = GetSystemMetrics(SM_CXFRAME);
    dyWinFrame = GetSystemMetrics(SM_CYFRAME);
    if ((hdibPlaque == 0x0))
        goto L_0b09;
    else
        goto L_0ab7;

L_0ab7:
    lpbi = GlobalLock(hdibPlaque);
    *(lpbi + 0x40e) = LOBYTE(LOWORD(crButtonFace));
    *(lpbi + 0x40d) = LOBYTE((LOWORD(crButtonFace) >> 0x8));
    *(lpbi + 0x40c) = LOBYTE(LOWORD((uint32_t)((crButtonFace >> 0x10))));
    GlobalUnlock(hdibPlaque);

L_0b09:
    if ((hdibToolbar == 0x0))
        goto L_0b65;
    else
        goto L_0b13;

L_0b13:
    lpbi = GlobalLock(hdibToolbar);
    *(lpbi + 0x41e) = LOBYTE(LOWORD(crButtonFace));
    *(lpbi + 0x41d) = LOBYTE((LOWORD(crButtonFace) >> 0x8));
    *(lpbi + 0x41c) = LOBYTE(LOWORD((uint32_t)((crButtonFace >> 0x10))));
    GlobalUnlock(hdibToolbar);

L_0b65:
    hdc = GetDC(0x0);
    /* untranslated: ss:[bp-0x6] = GetDeviceCaps(hdc, BITSPIXEL) */
    /* untranslated: vcScreenColors = loword((ss:[bp-0x6] * GetDeviceCaps(hdc, PLANES))) */
    ReleaseDC(0x0, hdc);

L_0ba8:
    return 0x1;
}

void FreeStuff() {
    int16_t i;
    int16_t j;

L_0bae:
    if ((hbrButtonFace == 0x0))
        goto L_0bcd;
    else
        goto L_0bc1;

L_0bc1:
    FreeHbr(hbrButtonFace);

L_0bcd:
    if ((hbrButtonHilite == 0x0))
        goto L_0be3;
    else
        goto L_0bd7;

L_0bd7:
    FreeHbr(hbrButtonHilite);

L_0be3:
    if ((hbrButtonShadow == 0x0))
        goto L_0bf9;
    else
        goto L_0bed;

L_0bed:
    FreeHbr(hbrButtonShadow);

L_0bf9:
    if ((hbrButtonText == 0x0))
        goto L_0c0f;
    else
        goto L_0c03;

L_0c03:
    FreeHbr(hbrButtonText);

L_0c0f:
    if ((hbrWindowText == 0x0))
        goto L_0c25;
    else
        goto L_0c19;

L_0c19:
    FreeHbr(hbrWindowText);

L_0c25:
    if ((hbrWindow == 0x0))
        goto L_0c3b;
    else
        goto L_0c2f;

L_0c2f:
    FreeHbr(hbrWindow);

L_0c3b:
    if ((hbrWindowFrame == 0x0))
        goto L_0c51;
    else
        goto L_0c45;

L_0c45:
    FreeHbr(hbrWindowFrame);

L_0c51:
    if ((hbrDesktop == 0x0))
        goto L_0c67;
    else
        goto L_0c5b;

L_0c5b:
    FreeHbr(hbrDesktop);

L_0c67:
    if ((hbrRed == 0x0))
        goto L_0c7d;
    else
        goto L_0c71;

L_0c71:
    FreeHbr(hbrRed);

L_0c7d:
    if ((hbrGreen == 0x0))
        goto L_0c93;
    else
        goto L_0c87;

L_0c87:
    FreeHbr(hbrGreen);

L_0c93:
    if ((hbrBlue == 0x0))
        goto L_0ca9;
    else
        goto L_0c9d;

L_0c9d:
    FreeHbr(hbrBlue);

L_0ca9:
    if ((hbrPurple == 0x0))
        goto L_0cbf;
    else
        goto L_0cb3;

L_0cb3:
    FreeHbr(hbrPurple);

L_0cbf:
    if ((hbrTooltip == 0x0))
        goto L_0cd5;
    else
        goto L_0cc9;

L_0cc9:
    FreeHbr(hbrTooltip);

L_0cd5:
    i = 0x0;
    goto L_0d01;

L_0cdd:
    if ((rghbrMineral[i] == 0x0))
        goto L_0cfd;
    else
        goto L_0cec;

L_0cec:
    FreeHbr(rghbrMineral[i]);

L_0cfd:
    i = (i + 0x1);

L_0d01:
    if ((i <= 0x4))
        goto L_0cdd;
    else
        goto L_0d0a;

L_0d0a:
    i = 0x0;
    goto L_0d48;

L_0d12:
    j = 0x0;
    goto L_0d3b;

L_0d1a:
    FreeHbr(rghbrPlanetAttr[i][j]);
    j = (j + 0x1);

L_0d3b:
    if ((j < 0x2))
        goto L_0d1a;
    else
        goto L_0d44;

L_0d44:
    i = (i + 0x1);

L_0d48:
    if ((i < 0x3))
        goto L_0d12;
    else
        goto L_0d51;

L_0d51:
    i = 0x0;
    goto L_0d8f;

L_0d59:
    j = 0x0;
    goto L_0d82;

L_0d61:
    FreeHbr(rghbrMinSum[i][j]);
    j = (j + 0x1);

L_0d82:
    if ((j < 0x2))
        goto L_0d61;
    else
        goto L_0d8b;

L_0d8b:
    i = (i + 0x1);

L_0d8f:
    if ((i < 0x4))
        goto L_0d59;
    else
        goto L_0d98;

L_0d98:
    FreeProcInstance(lpfnFakeComboProc);
    FreeProcInstance(lpfnFakeCEProc);
    FreeProcInstance(lpfnFakeEditProc);
    FreeProcInstance(lpfnFakeListProc);
    FreeProcInstance(lpfnHostTimerProc);
    FreeProcInstance(lpfnBrowserDlgProc);
    if ((LOWORD(lpfnTutorDlgProc) != 0x0))
        goto L_0dfa;
    else
        goto L_0df0;

L_0df0:
    if ((HIWORD(lpfnTutorDlgProc) == 0x0))
        goto L_0e07;
    else
        goto L_0dfa;

L_0dfa:
    FreeProcInstance(lpfnTutorDlgProc);

L_0e07:
    DeleteObject(hrgnHuge);
    DeleteObject(hrgnScratch);
    SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f00)));
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
    i = 0x0;
    goto L_0eec;

L_0eda:
    DestroyIcon(rghiconVCR[i]);
    i = (i + 0x1);

L_0eec:
    if ((i < 0x7))
        goto L_0eda;
    else
        goto L_0ef5;

L_0ef5:
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
    i = 0x0;
    goto L_0fc9;

L_0f8d:
    GlobalUnlock(rghdibShips[i]);
    FreeResource(rghdibShips[i]);
    GlobalUnlock(rghdibShipsT[i]);
    FreeResource(rghdibShipsT[i]);
    i = (i + 0x1);

L_0fc9:
    if ((i < 0x5))
        goto L_0f8d;
    else
        goto L_0fd2;

L_0fd2:
    i = 0x0;
    goto L_0ffa;

L_0fda:
    GlobalUnlock(rghdibInventory[i]);
    FreeResource(rghdibInventory[i]);
    i = (i + 0x1);

L_0ffa:
    if ((i < 0x7))
        goto L_0fda;
    else
        goto L_1003;

L_1003:
    FreeLp(&(lpLog), htLog);
    lpLog = 0x0;
    FreeLp(&(lpMsg), htMsg);
    lpMsg = 0x0;
    DeleteObject(vhpal);
    if ((vhpalSplash == 0x0))
        goto L_105f;
    else
        goto L_1056;

L_1056:
    DeleteObject(vhpalSplash);

L_105f:
    FreeHbr(hbrShip);
    FreeHbr(hbrStarbase);
    FreeHbr(hbrBBlue);
    FreeHbr(hbrEnemy);
    FreeHbr(hbrSelect);
    FreeHbr(hbrRadar);
    if ((hbrRadarNear == 0x0))
        goto L_10bd;
    else
        goto L_10b1;

L_10b1:
    FreeHbr(hbrRadarNear);

L_10bd:
    FreeHbr(hbrLightGray);
    FreeHbr(hbrGray);
    FreeHbr(hbrYellow);
    FreeHbr(hbrDkYellow);
    DeleteObject(hbr50Screen);
    i = 0x0;
    goto L_1110;

L_10fe:
    DeleteObject(rghbrPat[i]);
    i = (i + 0x1);

L_1110:
    if ((i < 0x3))
        goto L_10fe;
    else
        goto L_1119;

L_1119:
    DeleteObject(hbrCargo);
    DeleteObject(hbrDock);
    DeleteObject(hpenShip);
    DeleteObject(hpenDkGreen);
    DeleteObject(hpenDkPurple);
    DeleteObject(hpenStarbase);
    DeleteObject(hpenEnemy);
    DeleteObject(hpenMassPath);
    DeleteObject(hpenRadar);
    if ((hpenRadarNear == 0x0))
        goto L_117d;
    else
        goto L_1174;

L_1174:
    DeleteObject(hpenRadarNear);

L_117d:
    DeleteObject(hpenDkBlue);
    DeleteObject(hpenYellow);
    DeleteObject(hpenDkYellow);
    DeleteObject(rghfontArial10[0x0]);
    DeleteObject(rghfontArial10[0x1]);
    i = 0x0;
    goto L_11c4;

L_11b2:
    DeleteObject(rghfontArial8[i]);
    i = (i + 0x1);

L_11c4:
    if ((i < 0x5))
        goto L_11b2;
    else
        goto L_11cd;

L_11cd:
    DeleteObject(rghfontArial6);
    DeleteObject(rghfontArial7);
    i = 0x0;
    goto L_1202;

L_11e7:
    FreeHb(rglphb[i]);
    i = (i + 0x1);

L_1202:
    if ((i < 0xc))
        goto L_11e7;
    else
        goto L_120b;

L_120b:
    return;
}

char *SzVersion() {
L_1212:
    _wsprintf(szWork, PszGetCompressedString(idsVersionD02dC), 0x2, 0x3c, 0x6a);

L_124c:
    return 0x57a4;
}

int16_t About(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT    rc;
    HDC     hdc;
    int16_t i;
    HWND    hwndCtl;
    int16_t (**lpProc)();

L_1252:
    goto L_14e5;

L_1261:
    iAbout1st = 0xfff5;
    iAboutPartial = 0x0;
    SetWindowText(GetDlgItem(hwnd, 0x401), SzVersion());
    uTimerId = SetTimer(hwnd, 0xe, 0x32, 0x0);
    return 0x1;

L_12a9:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_12d1:
    hwndCtl = GetDlgItem(hwnd, IDC_U16_0x041F);
    iAboutPartial = (iAboutPartial + 0x2);
    if ((iAboutPartial < dyArial8))
        goto L_130c;
    else
        goto L_12f1;

L_12f1:
    iAboutPartial = 0x0;
    iAbout1st = (iAbout1st + 0x1);
    if ((iAbout1st <= 0x4e))
        goto L_130c;
    else
        goto L_1306;

L_1306:
    iAbout1st = 0xfff5;

L_130c:
    GetClientRect(hwndCtl, &(rc));
    hdc = GetDC(hwndCtl);
    SelectObject(hdc, rghfontArial8[0x1]);
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, crButtonFace);
    SetTextColor(hdc, crButtonText);
    IntersectClipRect(hdc, 0x0, 0x0, rc.right, rc.bottom);
    rc.top = (rc.top - iAboutPartial);
    rc.bottom = (rc.top + dyArial8);
    i = iAbout1st;
    goto L_1391;

L_138d:
    i = (i + 0x1);

L_1391:
    if ((i >= (iAbout1st + 0xa)))
        goto L_13fa;
    else
        goto L_139f;

L_139f:
    if ((i < 0x0))
        goto L_13d7;
    else
        goto L_13a8;

L_13a8:
    if ((i >= 0x4d))
        goto L_13d7;
    else
        goto L_13b1;

L_13b1:
    RcCtrTextOut(hdc, &(rc), PszGetCompressedString((i + 0x277)), 0xffff);
    goto L_13e3;

L_13d7:
    if ((i >= 0x4d))
        goto L_13fa;
    else
        goto L_13dd;

L_13dd:

L_13e3:
    OffsetRect(&(rc), 0x0, dyArial8);
    goto L_138d;

L_13fa:
    rc.bottom = 0x3e8;
    FillRect(hdc, &(rc), hbrButtonFace);
    SelectClipRgn(hdc, 0x0);
    ReleaseDC(hwnd, hdc);
    goto L_1510;

L_142c:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_1510;
    else
        goto L_1448;

L_1448:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_1461:
    if ((wParam == 0x1))
        goto L_1473;
    else
        goto L_146a;

L_146a:
    if ((wParam != 0x2))
        goto L_149a;
    else
        goto L_1473;

L_1473:
    KillTimer(hwnd, uTimerId);
    uTimerId = 0x0;
    EndDialog(hwnd, 0x1);
    return 0x1;

L_149a:
    if ((wParam != 0x76))
        goto L_1510;
    else
        goto L_14a3;

L_14a3:
    lpProc = MakeProcInstance(OrderInfoDlg, hInst);
    DialogBox(hInst, IDD_ORDER_INFO, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));

L_14e5:
    if ((message == WM_ERASEBKGND))
        goto L_12a9;
    else
        goto L_14ed;

L_14ed:
    if ((message == WM_CTLCOLOR))
        goto L_142c;
    else
        goto L_14f5;

L_14f5:
    if ((message == WM_INITDIALOG))
        goto L_1261;
    else
        goto L_14fd;

L_14fd:
    if ((message == WM_COMMAND))
        goto L_1461;
    else
        goto L_1505;

L_1505:
    if ((message == WM_TIMER))
        goto L_12d1;
    else
        goto L_1510;

L_1510:
    return 0x0;
}

int16_t OrderInfoDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT rc;

L_151e:
    goto L_15b4;

L_152d:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_1555:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_15cf;
    else
        goto L_1571;

L_1571:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_158a:
    if ((wParam == 0x2))
        goto L_159c;
    else
        goto L_1593;

L_1593:
    if ((wParam != 0x1))
        goto L_15cf;
    else
        goto L_159c;

L_159c:
    EndDialog(hwnd, 0x1);
    return 0x1;

L_15b4:
    if ((message == WM_ERASEBKGND))
        goto L_152d;
    else
        goto L_15bc;

L_15bc:
    if ((message == WM_CTLCOLOR))
        goto L_1555;
    else
        goto L_15c4;

L_15c4:
    if ((message == WM_COMMAND))
        goto L_158a;
    else
        goto L_15cf;

L_15cf:
    return 0x0;
}

int16_t FHandleChar(HWND hwnd, uint16_t ch, int32_t lParam) {
    HWND hwndF;

L_15de:
    if ((hwndScanner == 0x0))
        goto L_1603;
    else
        goto L_15f1;

L_15f1:
    if ((ch == 0x2b))
        goto L_1615;
    else
        goto L_15fa;

L_15fa:
    if ((ch == 0x2d))
        goto L_1615;
    else
        goto L_1603;

L_1603:
    if ((ch == 0x76))
        goto L_1615;
    else
        goto L_160c;

L_160c:
    if ((ch != 0x56))
        goto L_164e;
    else
        goto L_1615;

L_1615:
    hwndF = GetFocus();
    if ((hwndMessage == 0x0))
        goto L_1632;
    else
        goto L_1627;

L_1627:
    if ((hwndF == hwndMsgEdit))
        goto L_164e;
    else
        goto L_1632;

L_1632:
    SendMessage(hwndScanner, WM_CHAR, ch, lParam);
    return 0x1;

L_164e:
    return 0x0;
}

int16_t FHandleKey(HWND hwnd, int16_t iMsg, int16_t iKey, uint32_t dw) {
    HWND     hwndF;
    POINT    pt;
    HWND     hwndOver;
    int16_t  i;
    int16_t  itb;
    uint16_t md;
    int16_t  iWarp;
    int16_t  iwp;
    uint16_t t_merge_1756_0001;
    uint16_t t_merge_1a23_0001;
    int16_t  t_merge_1ad2_0001;
    uint16_t t_merge_1bbd_0001;

L_165a:
    if ((iMsg != 0x100))
        goto L_16f4;
    else
        goto L_166d;

L_166d:
    if ((iKey != 0x1b))
        goto L_169d;
    else
        goto L_1676;

L_1676:
    if ((hwndBrowser == 0x0))
        goto L_169d;
    else
        goto L_1680;

L_1680:
    if ((GetActiveWindow() != hwndBrowser))
        goto L_169d;
    else
        goto L_168e;

L_168e:
    DestroyWindow(hwndBrowser);
    return 0x1;

L_169d:
    if ((iKey != 0x1b))
        goto L_16cf;
    else
        goto L_16a6;

L_16a6:
    if ((hwndPopup == 0x0))
        goto L_16cf;
    else
        goto L_16b0;

L_16b0:
    SendMessage(hwndPopup, WM_LBUTTONUP, 0x0, 0x0);
    return 0x1;

L_16cf:
    if ((iKey != 0x1b))
        goto L_175f;
    else
        goto L_16d8;

L_16d8:
    if ((hwndReportDlg == 0x0))
        goto L_175f;
    else
        goto L_16e2;

L_16e2:
    DestroyWindow(hwndReportDlg);
    return 0x1;

L_16f4:
    if ((iMsg != 0x101))
        goto L_175f;
    else
        goto L_16fe;

L_16fe:
    if ((hwndTb == 0x0))
        goto L_175f;
    else
        goto L_1708;

L_1708:
    if ((iKey == 0x1b))
        goto L_171a;
    else
        goto L_1711;

L_1711:
    if ((iKey != 0xd))
        goto L_175f;
    else
        goto L_171a;

L_171a:
    hwndF = GetParent(GetFocus());
    if ((hwndF == hwndTb))
        goto L_1744;
    else
        goto L_1733;

L_1733:
    if ((GetParent(hwndF) != hwndTb))
        goto L_175f;
    else
        goto L_1744;

L_1744:
    if ((iKey != 0x1b))
        goto L_1753;
    else
        goto L_174d;

L_174d:
    t_merge_1756_0001 = 0x1;
    goto L_1756;

L_1753:
    t_merge_1756_0001 = 0x0;

L_1756:
    TerminateToolbarFocus(t_merge_1756_0001);

L_175f:
    if ((iKey != 0x10))
        goto L_17ae;
    else
        goto L_1768;

L_1768:
    if ((hwndScanner == 0x0))
        goto L_17ae;
    else
        goto L_1772;

L_1772:
    GetCursorPos(&(pt));
    /* untranslated: hwndOver = WindowFromPoint(words(pt.y, pt.x)) */
    if ((hwndOver != hwndScanner))
        goto L_17ae;
    else
        goto L_1797;

L_1797:
    SendMessage(hwndOver, WM_SETCURSOR, hwndOver, 0x0);

L_17ae:
    if ((iMsg == 0x100))
        goto L_17be;
    else
        goto L_17b8;

L_17b8:
    return 0x0;

L_17be:
    if ((iKey == 0x8))
        goto L_1834;
    else
        goto L_17c7;

L_17c7:
    if ((iKey == 0x2e))
        goto L_1834;
    else
        goto L_17d0;

L_17d0:
    if ((iKey == 0x28))
        goto L_1834;
    else
        goto L_17d9;

L_17d9:
    if ((iKey == 0x26))
        goto L_1834;
    else
        goto L_17e2;

L_17e2:
    if ((iKey == 0x24))
        goto L_1834;
    else
        goto L_17eb;

L_17eb:
    if ((iKey == 0x23))
        goto L_1834;
    else
        goto L_17f4;

L_17f4:
    if ((iKey < 0x30))
        goto L_1806;
    else
        goto L_17fd;

L_17fd:
    if ((iKey <= 0x39))
        goto L_1834;
    else
        goto L_1806;

L_1806:
    if ((iKey == 0xbc))
        goto L_1834;
    else
        goto L_1810;

L_1810:
    if ((iKey == 0xbe))
        goto L_1834;
    else
        goto L_181a;

L_181a:
    if ((iKey == 0xdb))
        goto L_1834;
    else
        goto L_1824;

L_1824:
    if ((iKey == 0xdd))
        goto L_1834;
    else
        goto L_182e;

L_182e:
    return 0x0;

L_1834:
    hwndF = GetFocus();
    if ((hwndMessage == 0x0))
        goto L_1939;
    else
        goto L_1846;

L_1846:
    if ((hwndTb == 0x0))
        goto L_188a;
    else
        goto L_1850;

L_1850:
    if ((hwndTb == hwndF))
        goto L_1884;
    else
        goto L_185c;

L_185c:
    if ((GetParent(hwndF) == hwndTb))
        goto L_1884;
    else
        goto L_186d;

L_186d:
    if ((GetParent(GetParent(hwndF)) != hwndTb))
        goto L_188a;
    else
        goto L_1884;

L_1884:
    return 0x0;

L_188a:
    i = 0x0;
    goto L_18ad;

L_1892:
    if ((hwndF != rghwndOrderDD[i]))
        goto L_18a9;
    else
        goto L_18a3;

L_18a3:
    return 0x0;

L_18a9:
    i = (i + 0x1);

L_18ad:
    if ((i < 0x3))
        goto L_1892;
    else
        goto L_18b6;

L_18b6:
    if ((hwndF == hwndFleetCompLB))
        goto L_190e;
    else
        goto L_18c1;

L_18c1:
    if ((hwndF == hwndPlanetProdLB))
        goto L_190e;
    else
        goto L_18cc;

L_18cc:
    if ((hwndF == hwndMsgEdit))
        goto L_190e;
    else
        goto L_18d7;

L_18d7:
    if ((hwndF == hwndMsgDrop))
        goto L_190e;
    else
        goto L_18e2;

L_18e2:
    if ((hwndF == hwndOrderED))
        goto L_190e;
    else
        goto L_18ed;

L_18ed:
    if ((hwndF == hwndMsgScroll))
        goto L_190e;
    else
        goto L_18f8;

L_18f8:
    if ((hwndF == hwndFleetCompLB))
        goto L_190e;
    else
        goto L_1903;

L_1903:
    if ((hwndF != hwndShipDD))
        goto L_1914;
    else
        goto L_190e;

L_190e:
    return 0x0;

L_1914:
    if ((hwndBrowser == 0x0))
        goto L_1939;
    else
        goto L_191e;

L_191e:
    if ((hwndF != GetDlgItem(hwndBrowser, IDC_U16_0x010B)))
        goto L_1939;
    else
        goto L_1933;

L_1933:
    return 0x0;

L_1939:
    if ((iKey < 0x30))
        goto L_1a4a;
    else
        goto L_1942;

L_1942:
    if ((iKey > 0x39))
        goto L_1a4a;
    else
        goto L_194b;

L_194b:
    if ((iKey < 0x31))
        goto L_19a2;
    else
        goto L_1954;

L_1954:
    if ((iKey > 0x36))
        goto L_19a2;
    else
        goto L_195d;

L_195d:
    md = (iKey + 0xffcf);
    if ((md == (grbitScan & 0xf)))
        goto L_199c;
    else
        goto L_1974;

L_1974:
    ExecuteButton((iKey + 0xffcf), 0x1);
    InvalidateRect(hwndTb, 0x0, 0x0);

L_199c:
    return 0x1;

L_19a2:
    goto L_19e4;

L_19a8:
    itb = 0x7;
    goto L_1a07;

L_19b0:
    itb = 0x8;
    goto L_1a07;

L_19b8:
    itb = 0x9;
    goto L_1a07;

L_19c0:
    if ((GetKeyState(0x10) >= 0x0))
        goto L_19d9;
    else
        goto L_19d1;

L_19d1:
    itb = 0x11;
    goto L_1a07;

L_19d9:
    itb = 0xb;

L_19e4:
    if ((iKey == 0x30))
        goto L_19c0;
    else
        goto L_19ec;

L_19ec:
    if ((iKey == 0x37))
        goto L_19a8;
    else
        goto L_19f4;

L_19f4:
    if ((iKey == 0x38))
        goto L_19b0;
    else
        goto L_19fc;

L_19fc:
    if ((iKey == 0x39))
        goto L_19b8;
    else
        goto L_1a07;

L_1a07:
    if ((FIsButtonDown(itb) != 0x0))
        goto L_1a20;
    else
        goto L_1a1a;

L_1a1a:
    t_merge_1a23_0001 = 0x1;
    goto L_1a23;

L_1a20:
    t_merge_1a23_0001 = 0x0;

L_1a23:
    ExecuteButton(itb, t_merge_1a23_0001);
    InvalidateRect(hwndTb, 0x0, 0x0);
    return 0x1;

L_1a4a:
    goto L_1bdd;

L_1a50:
    return 0x0;

L_1a56:
    if ((sel.grobj != grobjFleet))
        goto L_1c30;
    else
        goto L_1a60;

L_1a60:
    iKey = 0x8;
    DeleteCurWayPoint(0x8);

L_1a74:
    if ((hwndF != hwndShipLB))
        goto L_1a85;
    else
        goto L_1a7f;

L_1a7f:
    return 0x0;

L_1a85:
    SendMessage(hwndMessage, WM_KEYDOWN, iKey, dw);
    goto L_1c30;

L_1aa1:
    if ((sel.grobj != grobjFleet))
        goto L_1b9a;
    else
        goto L_1aab;

L_1aab:
    if ((sel.iwpAct > 0x0))
        goto L_1abf;
    else
        goto L_1ab5;

L_1ab5:
    if ((sel.fl.cord <= 0x1))
        goto L_1b9a;
    else
        goto L_1abf;

L_1abf:
    if ((sel.iwpAct <= 0x0))
        goto L_1acf;
    else
        goto L_1ac9;

L_1ac9:
    t_merge_1ad2_0001 = sel.iwpAct;
    goto L_1ad2;

L_1acf:
    t_merge_1ad2_0001 = 0x1;

L_1ad2:
    iwp = t_merge_1ad2_0001;
    /* untranslated: iWarp = ((part[6:2](sel.fl.lpplord->rgord[iwp]) >> 0x4) & 0xf) */
    if ((iKey != 0xbc))
        goto L_1b0f;
    else
        goto L_1b08;

L_1b08:
    iWarp = (iWarp - 0x1);
    goto L_1b13;

L_1b0f:
    iWarp = (iWarp + 0x1);

L_1b13:
    if ((iWarp < 0x0))
        goto L_1b9a;
    else
        goto L_1b1c;

L_1b1c:
    if ((iWarp > 0xb))
        goto L_1b9a;
    else
        goto L_1b25;

L_1b25:
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[iwp]) = ((part[6:2](sel.fl.lpplord->rgord[iwp]) & 0xff0f) | ((iWarp & 0xf) * 0x10)) */
    FLookupFleet(0xffff, &(sel.fl));
    DrawPlanShip(0x0, 0x4220);

L_1b9a:
    return 0x1;

L_1ba0:
    pt.x = 0x0;
    pt.y = 0x0;
    if ((iKey != 0xdb))
        goto L_1bba;
    else
        goto L_1bb4;

L_1bb4:
    t_merge_1bbd_0001 = 0xfffe;
    goto L_1bbd;

L_1bba:
    t_merge_1bbd_0001 = 0xffff;

L_1bbd:
    /* untranslated: call ExecuteReportClick(words(pt.y, pt.x), 0x2, 0x0, t_merge_1bbd_0001) -> callresult(void) */
    return 0x1;

L_1bdd:
    if ((iKey == 0x8))
        goto L_1a56;
    else
        goto L_1be5;

L_1be5:
    if ((iKey == 0x23))
        goto L_1a74;
    else
        goto L_1bed;

L_1bed:
    if ((iKey == 0x24))
        goto L_1a74;
    else
        goto L_1bf5;

L_1bf5:
    if ((iKey == 0x26))
        goto L_1a74;
    else
        goto L_1bfd;

L_1bfd:
    if ((iKey == 0x28))
        goto L_1a74;
    else
        goto L_1c05;

L_1c05:
    if ((iKey == 0x2e))
        goto L_1a56;
    else
        goto L_1c0d;

L_1c0d:
    if ((iKey == 0xbc))
        goto L_1aa1;
    else
        goto L_1c15;

L_1c15:
    if ((iKey == 0xbe))
        goto L_1aa1;
    else
        goto L_1c1d;

L_1c1d:
    if ((iKey == 0xdb))
        goto L_1ba0;
    else
        goto L_1c25;

L_1c25:
    if ((iKey != 0xdd))
        goto L_1a50;
    else
        goto L_1c2a;

L_1c2a:

L_1c30:
    return 0x1;
}
