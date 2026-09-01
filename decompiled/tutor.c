#include "common.h"

ITEMACTION rgiaUnloadAllCol[5] = {{0}, {0}, {0}, {.iAction = iActionUnloadAll}};
ITEMACTION rgiaQuikDrop[5] = {{.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionUnloadAll},
                              {.iAction = iActionLoadDunnage}};
ITEMACTION rgiaQuikLoad[5] = {{.iAction = iActionLoadAll}, {.iAction = iActionLoadAll}, {.iAction = iActionLoadAll}, {0}, {.iAction = iActionLoadDunnage}};
ITEMACTION rgiaLoadAllCol[5] = {{0}, {0}, {0}, {.iAction = iActionLoadAll}};
ZIPPRODQ1  rgzpqTut[2] = {{
                              .fNoResearch = 1,
                              .cpq = 2,
                              .rgpq = {{.w = 193, .mdIdle = 1, .cQuan = 3}, {.w = 192, .cQuan = 3}},
                         },
                          {
                              .fNoResearch = 1,
                              .cpq = 3,
                              .rgpq = {{.w = 132, .mdIdle = 4, .cQuan = 2}, {.w = 193, .mdIdle = 1, .cQuan = 3}, {.w = 192, .cQuan = 3}},
                         }};

int16_t TutorDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HMENU hmenu;
    RECT  rc;
    int16_t (**lpProc)();
    int16_t  fRet;
    uint16_t t_merge_0108_0001;

L_0000:
    goto L_0228;

L_000f:
    tutor.hwnd = hwnd;
    SetWindowPos(hwnd, 0xffff, 0x0, 0x0, 0x0, 0x0, 0x3);
    StickyDlgPos(hwnd, &(ptStickyTutorDlg), 0x1);
    return 0x1;

L_004e:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0076:
    DrawTutorText(hwnd);
    return 0x1;

L_0087:
    PostMessage(hwndFrame, WM_CHAR, wParam, lParam);
    return 0x0;

L_00a3:
    if ((wParam != 0x9c7))
        goto L_0142;
    else
        goto L_00ad;

L_00ad:
    lpProc = MakeProcInstance(PanicDlg, hInst);
    fRet = DialogBox(hInst, IDD_PANIC, hwnd, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((fRet == 0))
        goto L_025b;
    else
        goto L_00f5;

L_00f5:
    if ((fRet != 2506))
        goto L_0105;
    else
        goto L_00ff;

L_00ff:
    t_merge_0108_0001 = 0x1;
    goto L_0108;

L_0105:
    t_merge_0108_0001 = 0x0;

L_0108:
    tutor.wFlags = ((tutor.wFlags & 0xfdff) | ((t_merge_0108_0001 & 0x1) << 0x9));
    PostMessage(hwndFrame, WM_STARS_CONTINUE, fRet, 0x0);

L_0142:
    if ((wParam != 0x2))
        goto L_01a0;
    else
        goto L_014b;

L_014b:
    ShowTutor(0x0);
    if ((tutor.fShowHidMsg == 0x0))
        goto L_0197;
    else
        goto L_016a;

L_016a:
    AlertSz(PszFormatIds(idsMakeTutorialReappearCompleteTaskChooseTutorial, 0x0), MB_ICONASTERISK);
    tutor.fShowHidMsg = 0x0;

L_0197:
    return 0x1;

L_01a0:
    if ((wParam != 0x76))
        goto L_025b;
    else
        goto L_01a9;

L_01a9:
    WinHelp(hwnd, szHelpFile, 0x1, (uint32_t)(tutor.idh));
    return 0x1;

L_01cb:
    StickyDlgPos(hwnd, &(ptStickyTutorDlg), 0x0);
    tutor.hwnd = 0x0;
    hmenu = GetASubMenu(hwndFrame, 0x5);
    CheckMenuItem(hmenu, 0x9c5, 0x0);
    EndDialog(hwnd, 0x1);
    EndTutor(0x1);
    return 0x1;

L_0228:
    if ((message == WM_DESTROY))
        goto L_01cb;
    else
        goto L_0230;

L_0230:
    if ((message == WM_PAINT))
        goto L_0076;
    else
        goto L_0238;

L_0238:
    if ((message == WM_ERASEBKGND))
        goto L_004e;
    else
        goto L_0240;

L_0240:
    if ((message == WM_CHAR))
        goto L_0087;
    else
        goto L_0248;

L_0248:
    if ((message == WM_INITDIALOG))
        goto L_000f;
    else
        goto L_0250;

L_0250:
    if ((message == WM_COMMAND))
        goto L_00a3;
    else
        goto L_025b;

L_025b:
    return 0x0;
}

int16_t PanicDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT rc;

L_026a:
    goto L_034b;

L_0279:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_02a1:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_0366;
    else
        goto L_02bd;

L_02bd:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_02d6:
    if ((wParam != 0x2))
        goto L_02f4;
    else
        goto L_02df;

L_02df:
    EndDialog(hwnd, 0x0);
    return 0x1;

L_02f4:
    if ((wParam != 0x76))
        goto L_031f;
    else
        goto L_02fd;

L_02fd:
    WinHelp(hwnd, szHelpFile, 0x1, (uint32_t)(tutor.idh));
    return 0x1;

L_031f:
    if ((wParam == 0x9c9))
        goto L_0333;
    else
        goto L_0329;

L_0329:
    if ((wParam != 0x9ca))
        goto L_0366;
    else
        goto L_0333;

L_0333:
    EndDialog(hwnd, wParam);
    return 0x1;

L_034b:
    if ((message == WM_ERASEBKGND))
        goto L_0279;
    else
        goto L_0353;

L_0353:
    if ((message == WM_CTLCOLOR))
        goto L_02a1;
    else
        goto L_035b;

L_035b:
    if ((message == WM_COMMAND))
        goto L_02d6;
    else
        goto L_0366;

L_0366:
    return 0x0;
}

void ShowTutor(int16_t fShow) {
    uint16_t t_merge_03a0_0001;

L_0374:
    if ((tutor.hwnd == 0x0))
        goto L_03b9;
    else
        goto L_0384;

L_0384:

L_038a:
    if ((fShow == 0))
        goto L_039d;
    else
        goto L_0397;

L_0397:
    t_merge_03a0_0001 = 0x5;
    goto L_03a0;

L_039d:
    t_merge_03a0_0001 = 0x0;

L_03a0:
    ShowWindow(tutor.hwnd, t_merge_03a0_0001);
    tutor.wFlags = ((tutor.wFlags & 0xfffe) | (fShow & 0x1));

L_03b9:
    return;
}

void DrawTutorText(HWND hwnd) {
    HDC         hdc;
    int16_t     yTop;
    int16_t     fPara;
    PAINTSTRUCT ps;
    int16_t     didt;
    int16_t     xLeft;
    int16_t     cch;
    char        rgch[256];
    RECT        rcBtn;
    RECT        rc;
    uint16_t    t_merge_0636_0001;

L_03c0:
    hdc = BeginPaint(hwnd, &(ps));
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[0]);
    GetWindowRect(hwnd, &(rc));
    GetWindowRect(GetDlgItem(hwnd, IDCANCEL), &(rcBtn));
    rc.bottom = rcBtn.top;
    ScreenToClient(hwnd, &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rc.top = (rc.top + (dyArial8 * 2));
    rc.bottom = (rc.bottom - ((uint32_t)((dyArial8 * 0x2)) / 3));
    rc.left = (rc.left + ((uint32_t)((dyArial8 * 0x2)) / 3));
    rc.right = (rc.right - ((uint32_t)((dyArial8 * 0x2)) / 3));
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, rc.left, rc.top, (rc.right - rc.left), 0x1, PATCOPY);
    PatBlt(hdc, rc.left, rc.top, 0x1, (rc.bottom - rc.top), PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rc.left, (rc.bottom - 1), (rc.right - rc.left), 0x1, PATCOPY);
    PatBlt(hdc, (rc.right - 1), rc.top, 0x1, (rc.bottom - rc.top), PATCOPY);
    /* untranslated: call ExpandRc(&rc, ((sext16to32(dyArial8) / 2) neg 0x0), ((sext16to32(dyArial8) / 2) neg 0x0)) -> callresult(void) */
    yTop = rc.top;
    FillRect(hdc, &(rc), hbrButtonFace);
    SetTextColor(hdc, crButtonText);
    didt = 0;
    goto L_0696;

L_058b:
    cch = CchTutorString(rgch, (tutor.idt + didt));
    if ((cch == 1))
        goto L_069f;
    else
        goto L_05a8;

L_05a8:

L_05ae:
    /* untranslated: fPara = (byte ds:[(0x175f + sext8to16(rgch[0x0]))] & 0x1) */
    if ((fPara == 0))
        goto L_05e8;
    else
        goto L_05cc;

L_05cc:
    xLeft = rc.left;
    if ((didt == 0))
        goto L_05e8;
    else
        goto L_05dc;

L_05dc:
    yTop = (yTop + ((uint32_t)(dyArial8) / 2));

L_05e8:
    if (((tutor.idt + didt) != tutor.idtBold))
        goto L_0617;
    else
        goto L_05f7;

L_05f7:
    SetTextColor(hdc, crButtonFace);
    SetBkColor(hdc, crButtonText);

L_0617:
    if ((didt == 0))
        goto L_0633;
    else
        goto L_0624;

L_0624:
    if ((fPara == 0))
        goto L_0633;
    else
        goto L_062d;

L_062d:
    t_merge_0636_0001 = 0x1;
    goto L_0636;

L_0633:
    t_merge_0636_0001 = 0x0;

L_0636:
    WrapTextOut(hdc, &(xLeft), &(yTop), rgch, cch, rc.left, (rc.right - rc.left), 0x0, t_merge_0636_0001, 0x1);
    if (((tutor.idt + didt) != tutor.idtBold))
        goto L_0692;
    else
        goto L_0672;

L_0672:
    SetTextColor(hdc, crButtonText);
    SetBkColor(hdc, crButtonFace);

L_0692:
    didt = (didt + 1);

L_0696:
    if ((didt < 8))
        goto L_058b;
    else
        goto L_069f;

L_069f:
    EndPaint(hwnd, &(ps));
    return;
}

void StartTutor(int16_t fRestart) {
    int16_t cx;
    int16_t cch;

L_06b4:
    if ((gd.fTutorial != 0x0))
        goto L_0a08;
    else
        goto L_06d0;

L_06d0:
    memset(&(tutor), 0x0, 0x2c);
    if ((LOWORD(lpfnTutorDlgProc) != 0x0))
        goto L_0748;
    else
        goto L_06ee;

L_06ee:
    if ((HIWORD(lpfnTutorDlgProc) != 0x0))
        goto L_0748;
    else
        goto L_06f8;

L_06f8:
    lpfnTutorDlgProc = MakeProcInstance(TutorDlg, hInst);
    if ((LOWORD(lpfnTutorDlgProc) != 0x0))
        goto L_0748;
    else
        goto L_071a;

L_071a:
    if ((HIWORD(lpfnTutorDlgProc) != 0x0))
        goto L_0748;
    else
        goto L_0724;

L_0724:
    AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
    goto L_0a29;

L_0748:
    gd.fTutorial = 0x1;
    SaveGameState();
    if ((fFreeingTitle != 0))
        goto L_0785;
    else
        goto L_0763;

L_0763:
    fFreeingTitle = 1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;
    ShowWindow(hwndFrame, SW_SHOW);

L_0785:
    grbitScan = 0x4e0;
    cx = GetSystemMetrics(SM_CXSCREEN);
    if ((cx < 1280))
        goto L_07aa;
    else
        goto L_07a1;

L_07a1:
    iScanZoom = 3;
    goto L_07d6;

L_07aa:
    if ((cx < 1024))
        goto L_07bd;
    else
        goto L_07b4;

L_07b4:
    iScanZoom = 2;
    goto L_07d6;

L_07bd:
    if ((cx < 800))
        goto L_07d0;
    else
        goto L_07c7;

L_07c7:
    iScanZoom = 1;
    goto L_07d6;

L_07d0:
    iScanZoom = 0;

L_07d6:
    if ((LOWORD(game.lid) != 0x0))
        goto L_093b;
    else
        goto L_07e0;

L_07e0:
    if ((HIWORD(game.lid) != 0x0))
        goto L_093b;
    else
        goto L_07ea;

L_07ea:
    cch = CchGetString(idsTutorial, szBase);
    if ((fRestart != 0))
        goto L_08ac;
    else
        goto L_0806;

L_0806:
    strcat(szBase, ".xy");
    if ((access(szBase, 0x0) == -1))
        goto L_08a4;
    else
        goto L_082e;

L_082e:
    if ((AlertSz(PszFormatIds(idsTutorialHasRunBeforeWouldLikeDestroy, 0x0), 0x2024) == 6))
        goto L_08a4;
    else
        goto L_0857;

L_0857:
    szBase[cch] = 0;
    strcat(szBase, ".m1");
    ini.fStartupFile = 0x1;
    if ((FOpenGame(hwndFrame, 0x0) <= 0))
        goto L_0898;
    else
        goto L_0893;

L_0893:
    CreateChildWindows();

L_0898:
    ini.fStartupFile = 0x0;

L_08a4:
    szBase[cch] = 0;

L_08ac:
    if ((LOWORD(game.lid) != 0x0))
        goto L_08ea;
    else
        goto L_08b6;

L_08b6:
    if ((HIWORD(game.lid) != 0x0))
        goto L_08ea;
    else
        goto L_08c0;

L_08c0:
    CreateTutorWorld();
    /* untranslated: call memset(&part[14:0](vrgZipProd), 0x0, 0x1a) -> callresult(void *) */
    vrgZipProd[0].fValid = 0x1;
    gd.fChgZipProd = 0x1;

L_08ea:
    InitializeMenu(0x0);
    PostMessage(hwndFrame, WM_COMMAND, 0xfa1, 0x0);
    if ((fFreeingTitle != 0))
        goto L_092e;
    else
        goto L_0919;

L_0919:
    fFreeingTitle = 1;
    DestroyWindow(hwndTitle);
    hwndTitle = 0x0;

L_092e:
    ShowWindow(hwndFrame, SW_SHOW);

L_093b:
    tutor.idsError = -1;
    tutor.fShowHidMsg = 0x1;
    tutor.idt = 0;
    tutor.idtBold = 0;
    tutor.fProgress = 0x0;

L_0965:
    if ((FTutorTaskDone() == 0))
        goto L_098e;
    else
        goto L_0972;

L_0972:
    if ((tutor.fTurnDone != 0x0))
        goto L_098e;
    else
        goto L_0986;

L_0986:
    tutor.idt = (tutor.idt + 8);
    goto L_0965;

L_098e:
    if ((tutor.fTutorDone == 0x0))
        goto L_09b3;
    else
        goto L_09a4;

L_09a4:
    EndTutor(0x0);
    goto L_0a29;

L_09b3:
    if ((tutor.hwnd != 0x0))
        goto L_09da;
    else
        goto L_09bd;

L_09bd:
    CreateDialog(hInst, IDD_TUTOR, hwndFrame, &(lpfnTutorDlgProc));

L_09da:
    if ((tutor.idt == 0))
        goto L_0a08;
    else
        goto L_09e4;

L_09e4:
    if ((tutor.fTurnDone != 0x0))
        goto L_09fd;
    else
        goto L_09f8;

L_09f8:
    tutor.idt = (tutor.idt - 8);

L_09fd:
    tutor.idtBold = tutor.idt;
    AdvanceTutor();

L_0a08:
    ShowTutor(0x1);
    InvalidateRect(tutor.hwnd, 0x0, 0x1);

L_0a29:
    return;
}

void AdvanceTutor() {
    char     szTitle[50];
    int16_t  fRedraw;
    int16_t  idtT;
    int16_t  fTaskDone;
    RECT     rc;
    uint16_t t_merge_0a6c_0001;

L_0a30:
    fRedraw = 0;
    tutor.fChange = 0x0;
    idtT = tutor.idtBold;
    fTaskDone = FTutorTaskDone();
    if ((idtT == tutor.idtBold))
        goto L_0a69;
    else
        goto L_0a63;

L_0a63:
    t_merge_0a6c_0001 = 0x1;
    goto L_0a6c;

L_0a69:
    t_merge_0a6c_0001 = 0x0;

L_0a6c:
    fRedraw = t_merge_0a6c_0001;
    if ((fTaskDone != 0))
        goto L_0a8a;
    else
        goto L_0a78;

L_0a78:
    if ((fRedraw != 0))
        goto LUpdatePage;
    else
        goto L_0a7e;

L_0a7e:

L_0a8a:
    if ((tutor.fTurnDone != 0x0))
        goto L_0aff;
    else
        goto SkipToNext;

SkipToNext:
    tutor.idt = (tutor.idt + 8);
    tutor.idtBold = tutor.idt;
    tutor.idsError = -1;
    tutor.fNoErrors = 0x1;
    tutor.fProgress = 0x0;
    if ((FTutorTaskDone() == 0))
        goto L_0aeb;
    else
        goto L_0ad4;

L_0ad4:
    if ((tutor.fTurnDone == 0x0))
        goto SkipToNext;
    else
        goto L_0ae5;

L_0ae5:

L_0aeb:
    fRedraw = 1;
    tutor.fNoErrors = 0x0;
    goto L_0b05;

L_0aff:
    tutor.idh = 3510;

L_0b05:
    if ((tutor.idt >= 640))
        goto L_0b26;
    else
        goto L_0b10;

L_0b10:
    if ((tutor.fTutorDone == 0x0))
        goto L_0b4c;
    else
        goto L_0b26;

L_0b26:
    if ((tutor.idsError == 522))
        goto L_0b3d;
    else
        goto L_0b31;

L_0b31:
    TutorError(0x20a);

L_0b3d:
    EndTutor(0x0);
    goto L_0bfb;

L_0b4c:
    if ((fRedraw == 0))
        goto L_0bfb;
    else
        goto LUpdatePage;

LUpdatePage:
    _wsprintf(szTitle, PszGetCompressedString(idsStarsTutorPageD80), (((uint32_t)(tutor.idt) / 8) + 0x1));
    SetWindowText(tutor.hwnd, szTitle);
    ShowTutor(0x1);
    GetWindowRect(tutor.hwnd, &(rc));
    ScreenToClient(tutor.hwnd, &(rc));
    ScreenToClient(tutor.hwnd, &(rc.right));
    /* untranslated: call ExpandRc(&rc, (dyArial8 neg 0), loword((-2 * dyArial8))) -> callresult(void) */
    InvalidateRect(tutor.hwnd, &(rc), 0x1);

L_0bfb:
    return;
}

void EndTutor(int16_t fClose) {
L_0c02:
    if ((gd.fTutorial == 0x0))
        goto L_0c89;
    else
        goto L_0c1e;

L_0c1e:
    gd.fTutorial = 0x0;
    if ((tutor.hwnd == 0x0))
        goto L_0c3d;
    else
        goto L_0c34;

L_0c34:
    DestroyWindow(tutor.hwnd);

L_0c3d:
    game.fTutorial = 0x0;
    if ((fClose == 0))
        goto L_0c5a;
    else
        goto L_0c52;

L_0c52:
    RestoreGameState();
    goto L_0c66;

L_0c5a:
    tutor.fFreeing = 0x1;

L_0c66:
    memset(&(tutor), 0x0, 0x2c);
    Randomize2(GetTickCount());

L_0c89:
    return;
}

void SaveGameState() {
    HMENU    hmenu;
    uint16_t t_merge_0d66_0001;
    uint16_t t_merge_0da7_0001;

L_0c90:
    tutor.fGameSaved = 0x0;
    tutor.grbitScan = grbitScan;
    tutor.iScanZoom = iScanZoom;
    tutor.wFlags = ((tutor.wFlags & 0xf7ff) | ((gd.fToolbar & 0x1) << 0xb));
    tutor.zpq = vrgZipProd[0x0].zpq1;
    /* untranslated: tutor.wFlags = ((tutor.wFlags & 0xefff) | (((part[13:2](vrgZipProd[0x0]) & 0xff) & 0x1) << 0xc)) */
    vrgZipProd[0].zpq1 = vrgZipProd[0x4].zpq1;
    /* untranslated: part[13:2](vrgZipProd[0x0]) = part[13:2](vrgZipProd[0x4]) */
    if ((gd.fToolbar != 0x0))
        goto L_0db2;
    else
        goto L_0d37;

L_0d37:
    hmenu = GetASubMenu(hwndFrame, 0x1);
    if ((gd.fToolbar != 0x0))
        goto L_0d63;
    else
        goto L_0d5d;

L_0d5d:
    t_merge_0d66_0001 = 0x1;
    goto L_0d66;

L_0d63:
    t_merge_0d66_0001 = 0x0;

L_0d66:
    gd.fToolbar = t_merge_0d66_0001;
    if ((gd.fToolbar == 0x0))
        goto L_0da4;
    else
        goto L_0d9e;

L_0d9e:
    t_merge_0da7_0001 = 0x8;
    goto L_0da7;

L_0da4:
    t_merge_0da7_0001 = 0x0;

L_0da7:
    CheckMenuItem(hmenu, 0xb3, t_merge_0da7_0001);
    RefitFrameChildren();

L_0db2:
    tutor.icolFSort = vrptFleet.icolSort;
    if ((vrptFleet.icolSort != 1))
        goto L_0dcc;
    else
        goto L_0dc2;

L_0dc2:
    if ((vrptFleet.fAscending != 0))
        goto L_0de8;
    else
        goto L_0dcc;

L_0dcc:
    vrptFleet.icolSort = 1;
    vrptFleet.fAscending = 1;
    InvalidateReport(0x1, 0x0);

L_0de8:
    if ((LOWORD(game.lid) != 0x0))
        goto L_0dfc;
    else
        goto L_0df2;

L_0df2:
    if ((HIWORD(game.lid) == 0x0))
        goto L_0e15;
    else
        goto L_0dfc;

L_0dfc:
    if ((game.fTutorial != 0x0))
        goto L_0e15;
    else
        goto L_0e10;

L_0e10:
    DestroyCurGame();

L_0e15:
    return;
}

void RestoreGameState() {
    HMENU    hmenu;
    uint16_t t_merge_0ec2_0001;
    uint16_t t_merge_0f03_0001;

L_0e1c:
    if ((tutor.fGameSaved != 0x0))
        goto L_0e35;
    else
        goto L_0e35;

L_0e35:
    grbitScan = tutor.grbitScan;
    iScanZoom = tutor.iScanZoom;
    vrgZipProd[4].zpq1 = vrgZipProd[0x0].zpq1;
    /* untranslated: part[13:2](vrgZipProd[0x4]) = part[13:2](vrgZipProd[0x0]) */
    vrgZipProd[0].zpq1 = tutor.zpq;
    /* untranslated: part[13:2](vrgZipProd[0x0]) = lobyte(tutor.fValidQ) */
    if ((gd.fToolbar == tutor.fTBVis))
        goto L_0f0e;
    else
        goto L_0e93;

L_0e93:
    hmenu = GetASubMenu(hwndFrame, 0x1);
    if ((gd.fToolbar != 0x0))
        goto L_0ebf;
    else
        goto L_0eb9;

L_0eb9:
    t_merge_0ec2_0001 = 0x1;
    goto L_0ec2;

L_0ebf:
    t_merge_0ec2_0001 = 0x0;

L_0ec2:
    gd.fToolbar = t_merge_0ec2_0001;
    if ((gd.fToolbar == 0x0))
        goto L_0f00;
    else
        goto L_0efa;

L_0efa:
    t_merge_0f03_0001 = 0x8;
    goto L_0f03;

L_0f00:
    t_merge_0f03_0001 = 0x0;

L_0f03:
    CheckMenuItem(hmenu, 0xb3, t_merge_0f03_0001);
    RefitFrameChildren();

L_0f0e:
    if ((vrptFleet.icolSort == tutor.icolFSort))
        goto L_0f30;
    else
        goto L_0f1a;

L_0f1a:
    vrptFleet.icolSort = tutor.icolFSort;
    InvalidateReport(0x1, 0x1);

L_0f30:
    return;
}

int16_t FAskKillTutor() {
L_0f36:
    if ((game.turn >= 0x1e))
        goto L_0f72;
    else
        goto L_0f49;

L_0f49:
    if ((AlertSz(PszFormatIds(idsCurrentlyRunningStarsTutorialDoWantExit, 0x0), 0x2024) != 6))
        goto L_0f87;
    else
        goto L_0f72;

L_0f72:
    EndTutor(0x1);
    return 0x1;

L_0f87:
    return 0x0;
}

int16_t FTutorTaskDone() {
    HS       hs1;
    HS       hs;
    HS       hs2;
    uint16_t t_merge_11f8_0001;
    uint16_t t_merge_1286_0001;
    uint16_t t_merge_130a_0001;
    uint16_t t_merge_1547_0001;
    uint16_t t_merge_16b4_0001;
    uint16_t t_merge_1820_0001;
    uint16_t t_merge_1902_0001;
    uint16_t t_merge_1a1e_0001;
    uint16_t t_merge_1ad7_0001;
    uint16_t t_merge_1b39_0001;
    uint16_t t_merge_1c45_0001;
    uint16_t t_merge_1c89_0001;
    uint16_t t_merge_1d3b_0001;
    uint16_t t_merge_1dee_0001;
    uint16_t t_merge_1e5f_0001;
    uint16_t t_merge_1f19_0001;
    uint16_t t_merge_1f5c_0001;
    uint16_t t_merge_1ff7_0001;
    uint16_t t_merge_2060_0001;
    uint16_t t_merge_213b_0001;
    uint16_t t_merge_227e_0001;
    uint16_t t_merge_2338_0001;
    uint16_t t_merge_2409_0001;
    uint16_t t_merge_249f_0001;
    uint16_t t_merge_24c2_0001;
    uint16_t t_merge_2668_0001;
    uint16_t t_merge_2782_0001;
    uint16_t t_merge_29a8_0001;
    uint16_t t_merge_2a51_0001;
    uint16_t t_merge_2a78_0001;
    uint16_t t_merge_2bec_0001;
    uint16_t t_merge_2e45_0001;
    uint16_t t_merge_2e97_0001;
    uint16_t t_merge_2f4b_0001;
    uint16_t t_merge_2fa3_0001;
    LRESULT  t_call_308b;
    uint16_t t_merge_314a_0001;
    uint16_t t_merge_3246_0001;
    uint16_t t_merge_3274_0001;
    uint16_t t_merge_348c_0001;
    uint16_t t_merge_36af_0001;
    uint16_t t_merge_37cf_0001;
    uint16_t t_merge_38ba_0001;
    uint16_t t_merge_3ce0_0001;
    uint16_t t_merge_3df9_0001;
    uint16_t t_merge_3ea9_0001;
    uint16_t t_merge_4169_0001;
    uint16_t t_merge_452f_0001;
    FLEET   *t_call_46b2;
    FLEET   *t_call_4bfa;
    uint16_t t_merge_4c70_0001;
    uint16_t t_merge_53ea_0001;
    FLEET   *t_call_56df;
    uint16_t t_merge_5794_0001;
    FLEET   *t_call_57a7;
    uint16_t t_merge_58c6_0001;

L_0fbc:
    goto L_63fe;
    goto L_120a;

L_0fd1:
    tutor.idtBold = 5;
    return FCheckMessages(0x270f, 0xffff, 0x0);

L_0fee:
    tutor.idtBold = 11;
    if ((FCheckSelection(grobjFleet, 0x0) != 0))
        goto L_1012;
    else
        goto L_100c;

L_100c:
    return 0x0;

L_1012:
    tutor.idtBold = 15;
    return FCheckFleetWP(0x0, 0x1, grobjPlanet, 0xc, 0x0, 0xffff);

L_103b:
    tutor.idtBold = 18;
    if ((FCheckSelection(grobjFleet, 0x1) != 0))
        goto L_1065;
    else
        goto L_1059;

L_1059:
    tutor.idh = 6001;
    return 0x0;

L_1065:
    tutor.idtBold = 21;
    return FCheckFleetWP(0x1, 0x1, grobjPlanet, 0x10, 0x0, 0xffff);

L_108e:
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_10d2;
    else
        goto L_10a6;

L_10a6:
    tutor.idtBold = 31;
    return FCheckFleetWP(0x4, 0x1, grobjPlanet, 0xf, 0x0, 0xffff);

L_10d2:
    if ((FCheckSelection(grobjFleet, 0x3) == 0))
        goto L_10f3;
    else
        goto L_10ea;

L_10ea:
    tutor.idtBold = 29;
    goto L_111a;

L_10f3:
    if ((FCheckSelection(grobjFleet, 0x2) == 0))
        goto L_1114;
    else
        goto L_110b;

L_110b:
    tutor.idtBold = 27;
    goto L_111a;

L_1114:
    tutor.idtBold = 25;

L_111a:
    return 0x0;

L_1120:
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_1167;
    else
        goto L_1138;

L_1138:
    if ((tutor.fProgress != 0x0))
        goto L_1167;
    else
        goto L_114b;

L_114b:
    if ((tutor.fAutoComplete != 0x0))
        goto L_1167;
    else
        goto L_115e;

L_115e:
    tutor.idtBold = 32;
    goto L_11fb;

L_1167:
    if ((FCheckSelection(grobjFleet, 0x5) == 0))
        goto L_11ae;
    else
        goto L_117f;

L_117f:
    if ((tutor.fProgress != 0x0))
        goto L_11ae;
    else
        goto L_1192;

L_1192:
    if ((tutor.fAutoComplete != 0x0))
        goto L_11ae;
    else
        goto L_11a5;

L_11a5:
    tutor.idtBold = 34;
    goto L_11fb;

L_11ae:
    if ((FCheckResearch(0x1, 0x6, 0xf) == 0))
        goto L_11e5;
    else
        goto L_11ca;

L_11ca:
    tutor.idtBold = 39;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_11e5:
    if ((pctResGlob != -1))
        goto L_11f5;
    else
        goto L_11ef;

L_11ef:
    t_merge_11f8_0001 = 0x25;
    goto L_11f8;

L_11f5:
    t_merge_11f8_0001 = 0x26;

L_11f8:
    tutor.idtBold = t_merge_11f8_0001;

L_11fb:
    return 0x0;

L_1201:

L_120a:
    if ((tutor.idt == 0))
        goto L_0fd1;
    else
        goto L_1212;

L_1212:
    if ((tutor.idt == 8))
        goto L_0fee;
    else
        goto L_121a;

L_121a:
    if ((tutor.idt == 16))
        goto L_103b;
    else
        goto L_1222;

L_1222:
    if ((tutor.idt == 24))
        goto L_108e;
    else
        goto L_122a;

L_122a:
    if ((tutor.idt == 32))
        goto L_1120;
    else
        goto L_1232;

L_1232:
    goto L_1201;
    goto L_12aa;

L_123b:

L_1241:
    if ((FCheckQueue(0xd, 0x0, grobjPlanet, 0x7, 0x14, 0x0) != 0))
        goto L_128f;
    else
        goto L_1269;

L_1269:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_1283;
    else
        goto L_1273;

L_1273:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_1283;
    else
        goto L_127d;

L_127d:
    t_merge_1286_0001 = 0x2a;
    goto L_1286;

L_1283:
    t_merge_1286_0001 = 0x2b;

L_1286:
    tutor.idtBold = t_merge_1286_0001;
    return 0x0;

L_128f:
    tutor.idtBold = 47;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_12aa:
    if ((tutor.idt == 40))
        goto L_1241;
    else
        goto L_12b2;

L_12b2:
    goto L_123b;
    goto L_1935;

L_12bb:
    if ((FCheckFleetWP(0x1, 0x1, grobjPlanet, 0x15, 0x0, 0xffff) == 0))
        goto L_12e9;
    else
        goto L_12e3;

L_12e3:
    return 0x1;

L_12e9:
    if ((FCheckSelection(grobjFleet, 0x0) == 0))
        goto L_1307;
    else
        goto L_1301;

L_1301:
    t_merge_130a_0001 = 0x32;
    goto L_130a;

L_1307:
    t_merge_130a_0001 = 0x30;

L_130a:
    tutor.idtBold = t_merge_130a_0001;
    if ((FCheckFleetWP(0x0, 0x1, grobjPlanet, 0x9, 0x0, 0xffff) != 0))
        goto L_133b;
    else
        goto L_1335;

L_1335:
    return 0x0;

L_133b:
    tutor.idtBold = 51;
    if ((FCheckFleetWP(0x0, 0x2, grobjPlanet, 0x3, 0x0, 0xffff) != 0))
        goto L_136f;
    else
        goto L_1369;

L_1369:
    return 0x0;

L_136f:
    tutor.idtBold = 52;
    if ((FCheckFleetWP(0x0, 0x3, grobjPlanet, 0x8, 0x0, 0xffff) != 0))
        goto L_13a3;
    else
        goto L_139d;

L_139d:
    return 0x0;

L_13a3:
    tutor.idtBold = 53;
    if ((FCheckFleetWP(0x0, 0x4, grobjPlanet, 0x5, 0x0, 0xffff) != 0))
        goto L_13d7;
    else
        goto L_13d1;

L_13d1:
    return 0x0;

L_13d7:
    tutor.idtBold = 54;
    if ((FCheckFleetWP(0x0, 0x5, grobjPlanet, 0x2, 0x0, 0xffff) != 0))
        goto L_140b;
    else
        goto L_1405;

L_1405:
    return 0x0;

L_140b:
    tutor.idtBold = 55;
    return FCheckSelection(grobjFleet, 0x1);

L_1424:
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0xe, 0x0, 0xffff) == 0))
        goto L_1452;
    else
        goto L_144c;

L_144c:
    return 0x1;

L_1452:
    tutor.idtBold = 57;
    if ((FCheckFleetWP(0x1, 0x1, grobjPlanet, 0x15, 0x0, 0xffff) != 0))
        goto L_1486;
    else
        goto L_1480;

L_1480:
    return 0x0;

L_1486:
    tutor.idtBold = 58;
    if ((FCheckFleetWP(0x1, 0x2, grobjPlanet, 0x13, 0x0, 0xffff) != 0))
        goto L_14ba;
    else
        goto L_14b4;

L_14b4:
    return 0x0;

L_14ba:
    tutor.idtBold = 59;
    if ((FCheckFleetWP(0x1, 0x3, grobjPlanet, 0x14, 0x0, 0xffff) != 0))
        goto L_14ee;
    else
        goto L_14e8;

L_14e8:
    return 0x0;

L_14ee:
    tutor.idtBold = 60;
    if ((FCheckFleetWP(0x1, 0x4, grobjPlanet, 0x7, 0x0, 0xffff) != 0))
        goto L_1522;
    else
        goto L_151c;

L_151c:
    return 0x0;

L_1522:
    if ((FCheckMessages(0x2, 0xffff, 0x0) == 0))
        goto L_1544;
    else
        goto L_153e;

L_153e:
    t_merge_1547_0001 = 0x3f;
    goto L_1547;

L_1544:
    t_merge_1547_0001 = 0x3d;

L_1547:
    tutor.idtBold = t_merge_1547_0001;
    return FCheckSelection(grobjFleet, 0x4);

L_155d:
    if ((FCheckFleetWP(0x5, 0x1, grobjPlanet, 0xc, 0x3, 0xffff) == 0))
        goto L_158b;
    else
        goto L_1585;

L_1585:
    return 0x1;

L_158b:
    tutor.idtBold = 64;
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0xe, 0x0, 0xffff) != 0))
        goto L_15bf;
    else
        goto L_15b9;

L_15b9:
    return 0x0;

L_15bf:
    tutor.idtBold = 65;
    if ((FCheckFleetWP(0x4, 0x2, grobjPlanet, 0x11, 0x0, 0xffff) != 0))
        goto L_15f3;
    else
        goto L_15ed;

L_15ed:
    return 0x0;

L_15f3:
    tutor.idtBold = 66;
    if ((FCheckFleetWP(0x4, 0x3, grobjPlanet, 0x12, 0x0, 0xffff) != 0))
        goto L_1627;
    else
        goto L_1621;

L_1621:
    return 0x0;

L_1627:
    tutor.idtBold = 67;
    if ((FCheckFleetWP(0x4, 0x4, grobjPlanet, 0x17, 0x0, 0xffff) != 0))
        goto L_165b;
    else
        goto L_1655;

L_1655:
    return 0x0;

L_165b:
    tutor.idtBold = 68;
    if ((FCheckFleetWP(0x4, 0x5, grobjPlanet, 0x16, 0x0, 0xffff) != 0))
        goto L_168f;
    else
        goto L_1689;

L_1689:
    return 0x0;

L_168f:
    if ((FCheckMessages(0x4, 0xffff, 0x0) == 0))
        goto L_16b1;
    else
        goto L_16ab;

L_16ab:
    t_merge_16b4_0001 = 0x47;
    goto L_16b4;

L_16b1:
    t_merge_16b4_0001 = 0x45;

L_16b4:
    tutor.idtBold = t_merge_16b4_0001;
    return FCheckSummary(grobjPlanet, 0xc);

L_16ca:
    if ((FCheckColonizeWP(0x2, 0x10, 0xffff) == 0))
        goto L_16ec;
    else
        goto L_16e6;

L_16e6:
    return 0x1;

L_16ec:
    if ((FCheckFleetWP(0x5, 0x1, grobjPlanet, 0xc, 0x3, 0xffff) == 0))
        goto L_1729;
    else
        goto L_1714;

L_1714:
    tutor.fProgress = 0x0;
    return 0x1;

L_1729:
    if ((FCheckFleetWP(0x5, 0x1, grobjPlanet, 0xc, 0x0, 0xffff) == 0))
        goto L_175a;
    else
        goto L_1751;

L_1751:
    tutor.idtBold = 79;
    goto L_1781;

L_175a:
    if ((FCheckSelection(grobjFleet, 0x5) == 0))
        goto L_177b;
    else
        goto L_1772;

L_1772:
    tutor.idtBold = 77;
    goto L_1781;

L_177b:
    tutor.idtBold = 76;

L_1781:
    return 0x0;

L_1787:
    if ((FCheckColonizeWP(0x2, 0x10, 0xffff) == 0))
        goto L_17a9;
    else
        goto L_17a3;

L_17a3:
    return 0x1;

L_17a9:
    if ((FCheckSummary(grobjPlanet, 0x10) == 0))
        goto L_17c7;
    else
        goto L_17c1;

L_17c1:
    return 0x1;

L_17c7:
    if ((FCheckSummary(grobjPlanet, 0xf) == 0))
        goto L_1823;
    else
        goto L_17df;

L_17df:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) == 0))
        goto L_1804;
    else
        goto L_17fb;

L_17fb:
    tutor.idtBold = 86;
    goto L_1823;

L_1804:
    if ((tutor.fProgress == 0x0))
        goto L_181d;
    else
        goto L_1817;

L_1817:
    t_merge_1820_0001 = 0x54;
    goto L_1820;

L_181d:
    t_merge_1820_0001 = 0x53;

L_1820:
    tutor.idtBold = t_merge_1820_0001;

L_1823:
    return 0x0;

L_1829:
    if ((FCheckCargo(LpflFromId(0x2), 0x0, 0x0, 0x0, 0x19) != 0))
        goto L_189d;
    else
        goto L_1857;

L_1857:
    if ((FCheckSelection(grobjFleet, 0x2) == 0))
        goto L_1891;
    else
        goto L_186f;

L_186f:
    if ((mdXferDlg != -1))
        goto L_1888;
    else
        goto L_1879;

L_1879:
    tutor.idh = 1514;
    tutor.idtBold = 90;
    goto L_1897;

L_1888:
    tutor.idtBold = 91;

L_1891:
    tutor.idtBold = 89;

L_1897:
    return 0x0;

L_189d:
    if ((FCheckColonizeWP(0x2, 0x10, 0xffff) != 0))
        goto L_1914;
    else
        goto L_18b9;

L_18b9:
    if ((FCheckFleetWP(0x2, 0x1, grobjPlanet, 0x10, 0x0, 0xffff) != 0))
        goto L_1908;
    else
        goto L_18e1;

L_18e1:
    if ((FCheckSelection(grobjFleet, 0x2) == 0))
        goto L_18ff;
    else
        goto L_18f9;

L_18f9:
    t_merge_1902_0001 = 0x5c;
    goto L_1902;

L_18ff:
    t_merge_1902_0001 = 0x59;

L_1902:
    tutor.idtBold = t_merge_1902_0001;
    goto L_190e;

L_1908:
    tutor.idtBold = 93;

L_190e:
    return 0x0;

L_1914:
    tutor.idtBold = 95;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_192c:

L_1935:
    if ((tutor.idt == 48))
        goto L_12bb;
    else
        goto L_193d;

L_193d:
    if ((tutor.idt == 56))
        goto L_1424;
    else
        goto L_1945;

L_1945:
    if ((tutor.idt == 64))
        goto L_155d;
    else
        goto L_194d;

L_194d:
    if ((tutor.idt == 72))
        goto L_16ca;
    else
        goto L_1955;

L_1955:
    if ((tutor.idt == 80))
        goto L_1787;
    else
        goto L_195d;

L_195d:
    if ((tutor.idt == 88))
        goto L_1829;
    else
        goto L_1965;

L_1965:
    goto L_192c;
    goto L_1cb9;

L_196e:

L_1974:
    if ((FCheckMessages(0xffff, idmHaveBuiltFactories, 0x1) != 0))
        goto L_199c;
    else
        goto L_1990;

L_1990:
    tutor.idtBold = 97;
    return 0x0;

L_199c:
    if ((FCheckQueue(0xd, 0x0, grobjPlanet, 0x1, 0x1e, 0x0) != 0))
        goto L_1a27;
    else
        goto L_19c4;

L_19c4:
    if ((FCheckMessages(0x1, 0xffff, 0x0) == 0))
        goto L_19f8;
    else
        goto L_19e0;

L_19e0:
    if ((FCheckSelection(grobjPlanet, 0xd) != 0))
        goto L_1a01;
    else
        goto L_19f8;

L_19f8:
    tutor.idtBold = 98;
    goto L_1a21;

L_1a01:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_1a1b;
    else
        goto L_1a0b;

L_1a0b:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_1a1b;
    else
        goto L_1a15;

L_1a15:
    t_merge_1a1e_0001 = 0x65;
    goto L_1a1e;

L_1a1b:
    t_merge_1a1e_0001 = 0x66;

L_1a1e:
    tutor.idtBold = t_merge_1a1e_0001;

L_1a21:
    return 0x0;

L_1a27:
    return 0x1;

L_1a2d:
    if ((FCheckQueue(0x10, 0x0, grobjPlanet, 0x7, 0x3, 0x1) == 0))
        goto L_1a7d;
    else
        goto L_1a55;

L_1a55:
    if ((FCheckQueue(0x10, 0x1, grobjPlanet, 0x8, 0x3, 0x1) != 0))
        goto L_1ae0;
    else
        goto L_1a7d;

L_1a7d:
    if ((FCheckMessages(0x3, 0xffff, 0x0) == 0))
        goto L_1ab1;
    else
        goto L_1a99;

L_1a99:
    if ((FCheckSelection(grobjPlanet, 0x10) != 0))
        goto L_1aba;
    else
        goto L_1ab1;

L_1ab1:
    tutor.idtBold = 104;
    goto L_1ada;

L_1aba:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_1ad4;
    else
        goto L_1ac4;

L_1ac4:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_1ad4;
    else
        goto L_1ace;

L_1ace:
    t_merge_1ad7_0001 = 0x6a;
    goto L_1ad7;

L_1ad4:
    t_merge_1ad7_0001 = 0x6b;

L_1ad7:
    tutor.idtBold = t_merge_1ad7_0001;

L_1ada:
    return 0x0;

L_1ae0:
    if ((FCheckCargo(LpflFromId(0x3), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_1b4b;
    else
        goto L_1b0e;

L_1b0e:
    if ((FCheckSelection(grobjFleet, 0x3) == 0))
        goto L_1b3f;
    else
        goto L_1b26;

L_1b26:
    if ((mdXferDlg == -1))
        goto L_1b36;
    else
        goto L_1b30;

L_1b30:
    t_merge_1b39_0001 = 0x6f;
    goto L_1b39;

L_1b36:
    t_merge_1b39_0001 = 0x6e;

L_1b39:
    tutor.idtBold = t_merge_1b39_0001;
    goto L_1b45;

L_1b3f:
    tutor.idtBold = 109;

L_1b45:
    return 0x0;

L_1b4b:
    return 0x1;

L_1b51:
    if ((FCheckXferWP(0x3, 0x1, 0x10, 0xffff, rgiaQuikDrop) != 0))
        goto L_1be6;
    else
        goto L_1b78;

L_1b78:
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0x10, 0x1, 0xffff) != 0))
        goto L_1bda;
    else
        goto L_1ba0;

L_1ba0:
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0x10, 0xffff, 0xffff) != 0))
        goto L_1bd1;
    else
        goto L_1bc8;

L_1bc8:
    tutor.idtBold = 112;
    goto L_1be0;

L_1bd1:
    tutor.idtBold = 113;

L_1bda:
    tutor.idtBold = 114;

L_1be0:
    return 0x0;

L_1be6:
    if ((LpflFromId(0x0)->cord >= 6))
        goto L_1c06;
    else
        goto L_1c00;

L_1c00:
    return 0x1;

L_1c06:
    if ((FCheckSelection(grobjFleet, 0x0) == 0))
        goto L_1c24;
    else
        goto L_1c1e;

L_1c1e:
    return 0x1;

L_1c24:
    if ((FCheckSummary(grobjPlanet, 0x9) == 0))
        goto L_1c42;
    else
        goto L_1c3c;

L_1c3c:
    t_merge_1c45_0001 = 0x76;
    goto L_1c45;

L_1c42:
    t_merge_1c45_0001 = 0x73;

L_1c45:
    tutor.idtBold = t_merge_1c45_0001;
    return 0x0;

L_1c4e:
    if ((LpflFromId(0x0)->cord != 6))
        goto L_1c92;
    else
        goto L_1c68;

L_1c68:
    if ((FCheckSummary(grobjPlanet, 0x9) == 0))
        goto L_1c86;
    else
        goto L_1c80;

L_1c80:
    t_merge_1c89_0001 = 0x7b;
    goto L_1c89;

L_1c86:
    t_merge_1c89_0001 = 0x79;

L_1c89:
    tutor.idtBold = t_merge_1c89_0001;
    return 0x0;

L_1c92:
    tutor.idtBold = 127;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_1cb9:
    if ((tutor.idt == 96))
        goto L_1974;
    else
        goto L_1cc1;

L_1cc1:
    if ((tutor.idt == 104))
        goto L_1a2d;
    else
        goto L_1cc9;

L_1cc9:
    if ((tutor.idt == 112))
        goto L_1b51;
    else
        goto L_1cd1;

L_1cd1:
    if ((tutor.idt == 120))
        goto L_1c4e;
    else
        goto L_1cd9;

L_1cd9:
    goto L_196e;
    goto L_1e83;

L_1ce2:

L_1ce8:
    if ((LpflFromId(0x4)->cord != 6))
        goto L_1d86;
    else
        goto L_1d02;

L_1d02:
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_1d41;
    else
        goto L_1d1a;

L_1d1a:
    if ((FCheckSummary(grobjPlanet, 0xe) == 0))
        goto L_1d38;
    else
        goto L_1d32;

L_1d32:
    t_merge_1d3b_0001 = 0x84;
    goto L_1d3b;

L_1d38:
    t_merge_1d3b_0001 = 0x83;

L_1d3b:
    tutor.idtBold = t_merge_1d3b_0001;
    goto L_1d80;

L_1d41:
    if ((FCheckSummary(grobjPlanet, 0xe) != 0))
        goto L_1d71;
    else
        goto L_1d59;

L_1d59:
    if ((FCheckSummary(grobjFleet, 0x4) == 0))
        goto L_1d7a;
    else
        goto L_1d71;

L_1d71:
    tutor.idtBold = 130;
    goto L_1d80;

L_1d7a:
    tutor.idtBold = 128;

L_1d80:
    return 0x0;

L_1d86:
    if ((LpplFromId(0xd)->lpplprod->iprodMac != 0x3))
        goto L_1daf;
    else
        goto L_1da9;

L_1da9:
    return 0x1;

L_1daf:
    if ((FCheckSelection(grobjPlanet, 0xd) == 0))
        goto L_1dcd;
    else
        goto L_1dc7;

L_1dc7:
    return 0x1;

L_1dcd:
    if ((FCheckSummary(grobjPlanet, 0x15) == 0))
        goto L_1deb;
    else
        goto L_1de5;

L_1de5:
    t_merge_1dee_0001 = 0x87;
    goto L_1dee;

L_1deb:
    t_merge_1dee_0001 = 0x85;

L_1dee:
    tutor.idtBold = t_merge_1dee_0001;
    return 0x0;

L_1df7:
    if ((LpplFromId(0xd)->lpplprod->iprodMac != 0x3))
        goto L_1e42;
    else
        goto L_1e1a;

L_1e1a:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x2, 0x1, 0x0) != 0))
        goto L_1e68;
    else
        goto L_1e42;

L_1e42:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_1e5c;
    else
        goto L_1e4c;

L_1e4c:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_1e5c;
    else
        goto L_1e56;

L_1e56:
    t_merge_1e5f_0001 = 0x88;
    goto L_1e5f;

L_1e5c:
    t_merge_1e5f_0001 = 0x89;

L_1e5f:
    tutor.idtBold = t_merge_1e5f_0001;
    return 0x0;

L_1e68:
    tutor.idtBold = 143;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_1e83:
    if ((tutor.idt == 128))
        goto L_1ce8;
    else
        goto L_1e8b;

L_1e8b:
    if ((tutor.idt == 136))
        goto L_1df7;
    else
        goto L_1e93;

L_1e93:
    goto L_1ce2;
    goto L_2084;

L_1e9c:

L_1ea2:
    if ((FCheckCargo(LpflFromId(0x2), 0x0, 0x0, 0x0, 0x19) == 0))
        goto L_1f28;
    else
        goto L_1ed0;

L_1ed0:
    if ((FCheckFleetWP(0x2, 0x1, grobjPlanet, 0xe, 0xffff, 0xffff) != 0))
        goto L_1f22;
    else
        goto L_1ef8;

L_1ef8:
    if ((FCheckScanner(0x3, 0xffff) == 0))
        goto L_1f16;
    else
        goto L_1f10;

L_1f10:
    t_merge_1f19_0001 = 0x97;
    goto L_1f19;

L_1f16:
    t_merge_1f19_0001 = 0x96;

L_1f19:
    tutor.idtBold = t_merge_1f19_0001;
    return 0x0;

L_1f22:
    return 0x1;

L_1f28:
    if ((FCheckSelection(grobjFleet, 0x2) != 0))
        goto L_1f49;
    else
        goto L_1f40;

L_1f40:
    tutor.idtBold = 144;
    goto L_1f5f;

L_1f49:
    if ((mdXferDlg == -1))
        goto L_1f59;
    else
        goto L_1f53;

L_1f53:
    t_merge_1f5c_0001 = 0x94;
    goto L_1f5c;

L_1f59:
    t_merge_1f5c_0001 = 0x92;

L_1f5c:
    tutor.idtBold = t_merge_1f5c_0001;

L_1f5f:
    return 0x0;

L_1f65:
    if ((FCheckColonizeWP(0x2, 0xe, 0xffff) != 0))
        goto L_1f8d;
    else
        goto L_1f81;

L_1f81:
    tutor.idtBold = 152;
    return 0x0;

L_1f8d:
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_2000;
    else
        goto L_1fb5;

L_1fb5:
    if ((FCheckScanner(0x0, 0xffff) != 0))
        goto L_1fd6;
    else
        goto L_1fcd;

L_1fcd:
    tutor.idtBold = 153;
    goto L_1ffa;

L_1fd6:
    if ((FCheckSelection(grobjFleet, 0x3) == 0))
        goto L_1ff4;
    else
        goto L_1fee;

L_1fee:
    t_merge_1ff7_0001 = 0x9b;
    goto L_1ff7;

L_1ff4:
    t_merge_1ff7_0001 = 0x9a;

L_1ff7:
    tutor.idtBold = t_merge_1ff7_0001;

L_1ffa:
    return 0x0;

L_2000:
    if ((LpflFromId(0x0)->cord != 5))
        goto L_2069;
    else
        goto L_201a;

L_201a:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) == 0))
        goto L_203f;
    else
        goto L_2036;

L_2036:
    tutor.idtBold = 158;
    goto L_2063;

L_203f:
    if ((FCheckSelection(grobjPlanet, 0x10) == 0))
        goto L_205d;
    else
        goto L_2057;

L_2057:
    t_merge_2060_0001 = 0x9e;
    goto L_2060;

L_205d:
    t_merge_2060_0001 = 0x9c;

L_2060:
    tutor.idtBold = t_merge_2060_0001;

L_2063:
    return 0x0;

L_2069:
    tutor.idtBold = 159;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_2084:
    if ((tutor.idt == 144))
        goto L_1ea2;
    else
        goto L_208c;

L_208c:
    if ((tutor.idt == 152))
        goto L_1f65;
    else
        goto L_2094;

L_2094:
    goto L_1e9c;
    goto L_22a2;

L_209d:

L_20a3:
    if ((FCheckXferWP(0x3, 0x1, 0xc, 0xffff, rgiaQuikLoad) != 0))
        goto L_2156;
    else
        goto L_20ca;

L_20ca:
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0xc, 0x1, 0xffff) != 0))
        goto L_214a;
    else
        goto L_20f2;

L_20f2:
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0xc, 0xffff, 0xffff) != 0))
        goto L_2141;
    else
        goto L_211a;

L_211a:
    if ((FCheckSelection(grobjFleet, 0x3) == 0))
        goto L_2138;
    else
        goto L_2132;

L_2132:
    t_merge_213b_0001 = 0xa3;
    goto L_213b;

L_2138:
    t_merge_213b_0001 = 0xa0;

L_213b:
    tutor.idtBold = t_merge_213b_0001;
    goto L_2150;

L_2141:
    tutor.idtBold = 164;

L_214a:
    tutor.idtBold = 165;

L_2150:
    return 0x0;

L_2156:
    tutor.idtBold = 166;
    return FCheckFleetWP(0x3, 0x2, grobjPlanet, 0xd, 0xffff, 0xffff);

L_217f:
    tutor.fNoErrors = 0x1;
    if ((FCheckXferWP(0x3, 0x2, 0xd, 0xffff, rgiaQuikDrop) != 0))
        goto L_21ca;
    else
        goto L_21b2;

L_21b2:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 170;
    return 0x0;

L_21ca:
    tutor.fNoErrors = 0x0;
    tutor.idh = 1518;
    tutor.idtBold = 171;
    if ((((*(LpflFromId(0x3) + 0x4) >> 0x9) & 0x1) != 0x0))
        goto L_220c;
    else
        goto L_2206;

L_2206:
    return 0x0;

L_220c:
    tutor.idh = 1059;
    if ((LpplFromId(0xd)->lpplprod->iprodMac != 0x3))
        goto L_225d;
    else
        goto L_2235;

L_2235:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x2, 0x1, 0x0) != 0))
        goto L_2287;
    else
        goto L_225d;

L_225d:
    if ((FCheckSelection(grobjPlanet, 0xd) == 0))
        goto L_227b;
    else
        goto L_2275;

L_2275:
    t_merge_227e_0001 = 0xae;
    goto L_227e;

L_227b:
    t_merge_227e_0001 = 0xad;

L_227e:
    tutor.idtBold = t_merge_227e_0001;
    return 0x0;

L_2287:
    tutor.idtBold = 175;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_22a2:
    if ((tutor.idt == 160))
        goto L_20a3;
    else
        goto L_22aa;

L_22aa:
    if ((tutor.idt == 168))
        goto L_217f;
    else
        goto L_22b2;

L_22b2:
    goto L_209d;
    goto L_25c5;

L_22bb:

L_22c1:
    if ((FCheckCargo(LpflFromId(0x6), 0x0, 0x0, 0x0, 0x19) != 0))
        goto L_22fb;
    else
        goto L_22ef;

L_22ef:
    tutor.idtBold = 176;
    return 0x0;

L_22fb:
    if ((FCheckColonizeWP(0x6, 0x12, 0xffff) != 0))
        goto L_2341;
    else
        goto L_2317;

L_2317:
    if ((FCheckScanner(0x3, 0xffff) == 0))
        goto L_2335;
    else
        goto L_232f;

L_232f:
    t_merge_2338_0001 = 0xb3;
    goto L_2338;

L_2335:
    t_merge_2338_0001 = 0xb1;

L_2338:
    tutor.idtBold = t_merge_2338_0001;
    return 0x0;

L_2341:
    tutor.idh = 1507;
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_2392;
    else
        goto L_236a;

L_236a:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x2, 0x3, 0x0) != 0))
        goto L_239e;
    else
        goto L_2392;

L_2392:
    tutor.idtBold = 181;
    return 0x0;

L_239e:
    if ((FCheckScanner(0x0, 0xffff) != 0))
        goto L_23c2;
    else
        goto L_23b6;

L_23b6:
    tutor.idtBold = 182;
    return 0x0;

L_23c2:
    return 0x1;

L_23c8:
    if ((FCheckMessages(0xffff, idmHaveBuiltMines, 0x1) != 0))
        goto L_2412;
    else
        goto L_23e4;

L_23e4:
    if ((FCheckMessages(0x3, 0xffff, 0x0) == 0))
        goto L_2406;
    else
        goto L_2400;

L_2400:
    t_merge_2409_0001 = 0xba;
    goto L_2409;

L_2406:
    t_merge_2409_0001 = 0xb8;

L_2409:
    tutor.idtBold = t_merge_2409_0001;
    return 0x0;

L_2412:
    if ((FCheckQueue(0x10, 0x0, grobjPlanet, 0x1, 0xa, 0x1) == 0))
        goto L_2462;
    else
        goto L_243a;

L_243a:
    if ((FCheckQueue(0x10, 0x1, grobjPlanet, 0x0, 0xa, 0x1) != 0))
        goto L_24cb;
    else
        goto L_2462;

L_2462:
    if ((FCheckSelection(grobjPlanet, 0x10) != 0))
        goto L_24a5;
    else
        goto L_247a;

L_247a:
    if ((FCheckMessages(0x4, 0xffff, 0x0) == 0))
        goto L_249c;
    else
        goto L_2496;

L_2496:
    t_merge_249f_0001 = 0xbc;
    goto L_249f;

L_249c:
    t_merge_249f_0001 = 0xbb;

L_249f:
    tutor.idtBold = t_merge_249f_0001;
    goto L_24c5;

L_24a5:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_24bf;
    else
        goto L_24af;

L_24af:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_24bf;
    else
        goto L_24b9;

L_24b9:
    t_merge_24c2_0001 = 0xbc;
    goto L_24c2;

L_24bf:
    t_merge_24c2_0001 = 0xbd;

L_24c2:
    tutor.idtBold = t_merge_24c2_0001;

L_24c5:
    return 0x0;

L_24cb:
    tutor.idtBold = 190;
    tutor.fProgress = 0x0;
    return FCheckMessages(0x270f, 0xffff, 0x0);

L_24f4:
    if ((tutor.fProgress != 0x0))
        goto L_254d;
    else
        goto L_2507;

L_2507:
    if ((tutor.fAutoComplete != 0x0))
        goto L_254d;
    else
        goto L_251a;

L_251a:
    if ((FCheckSummary(grobjFleet, 0x200) == 0))
        goto L_2541;
    else
        goto L_2532;

L_2532:
    tutor.fProgress = 0x1;
    goto L_254d;

L_2541:
    tutor.idtBold = 192;
    return 0x0;

L_254d:
    tutor.idh = 1507;
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_259e;
    else
        goto L_2576;

L_2576:
    if ((FCheckQueue(0xd, 0x2, grobjFleet, 0x0, 0x2, 0x0) != 0))
        goto L_25aa;
    else
        goto L_259e;

L_259e:
    tutor.idtBold = 196;
    return 0x0;

L_25aa:
    tutor.idtBold = 198;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_25c5:
    if ((tutor.idt == 176))
        goto L_22c1;
    else
        goto L_25cd;

L_25cd:
    if ((tutor.idt == 184))
        goto L_23c8;
    else
        goto L_25d5;

L_25d5:
    if ((tutor.idt == 192))
        goto L_24f4;
    else
        goto L_25dd;

L_25dd:
    goto L_22bb;
    goto L_293e;

L_25e6:

L_25ec:
    if ((rgplr[0].cFleet != 0xb))
        goto L_261c;
    else
        goto L_25fa;

L_25fa:
    if ((FCheckColonizeWP(0xa, 0x8, 0xffff) == 0))
        goto L_261c;
    else
        goto L_2616;

L_2616:
    return 0x1;

L_261c:
    if ((rgplr[0].cFleet == 0x9))
        goto L_2634;
    else
        goto L_262a;

L_262a:
    if ((mdXferDlg != 1))
        goto L_266e;
    else
        goto L_2634;

L_2634:
    if ((FCheckSelection(grobjFleet, 0x7) != 0))
        goto L_2655;
    else
        goto L_264c;

L_264c:
    tutor.idtBold = 200;
    goto L_26d3;

L_2655:
    if ((mdXferDlg != 1))
        goto L_2665;
    else
        goto L_265f;

L_265f:
    t_merge_2668_0001 = 0xcb;
    goto L_2668;

L_2665:
    t_merge_2668_0001 = 0xca;

L_2668:
    tutor.idtBold = t_merge_2668_0001;

L_266e:
    if ((FCheckCargo(LpflFromId(0x7), 0x0, 0x0, 0x0, 0x32) == 0))
        goto L_26c4;
    else
        goto L_269c;

L_269c:
    if ((FCheckFleetWP(0x7, 0x1, grobjPlanet, 0x8, 0x2, 0xffff) != 0))
        goto L_26cd;
    else
        goto L_26c4;

L_26c4:
    tutor.idtBold = 205;
    goto L_26d3;

L_26cd:
    tutor.idtBold = 206;

L_26d3:
    return 0x0;

L_26d9:
    tutor.fNoErrors = 0x1;
    if ((FCheckColonizeWP(0x7, 0x11, 0xffff) != 0))
        goto L_272d;
    else
        goto L_2701;

L_2701:
    tutor.fNoErrors = 0x0;
    FCheckColonizeWP(0x7, 0x8, 0xffff);
    tutor.idtBold = 209;
    return 0x0;

L_272d:
    tutor.fNoErrors = 0x0;
    if ((FCheckFleetWP(0x8, 0x1, grobjPlanet, 0x9, 0xffff, 0xffff) != 0))
        goto L_278b;
    else
        goto L_2761;

L_2761:
    if ((FCheckSelection(grobjFleet, 0x8) == 0))
        goto L_277f;
    else
        goto L_2779;

L_2779:
    t_merge_2782_0001 = 0xd6;
    goto L_2782;

L_277f:
    t_merge_2782_0001 = 0xd4;

L_2782:
    tutor.idtBold = t_merge_2782_0001;
    return 0x0;

L_278b:
    tutor.fProgress = 0x0;
    return 0x1;

L_279d:
    tutor.fNoErrors = 0x0;
    if ((FCheckMessages(0xffff, idmHasUnloaded, 0x1) != 0))
        goto L_27dd;
    else
        goto L_27c5;

L_27c5:
    tutor.fProgress = 0x0;
    tutor.idtBold = 216;
    return 0x0;

L_27dd:
    tutor.fNoErrors = 0x1;
    if ((FCheckResearch(0x1, 0x6, 0x1e) == 0))
        goto L_2817;
    else
        goto L_2805;

L_2805:
    tutor.fNoErrors = 0x0;
    return 0x1;

L_2817:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_284b;
    else
        goto L_2833;

L_2833:
    tutor.idtBold = 217;
    tutor.fProgress = 0x0;
    return 0x0;

L_284b:
    if ((tutor.fProgress != 0x0))
        goto L_289e;
    else
        goto L_285e;

L_285e:
    if ((tutor.fAutoComplete != 0x0))
        goto L_289e;
    else
        goto L_2871;

L_2871:
    if ((FCheckSummary(grobjPlanet, 0x5) == 0))
        goto L_2892;
    else
        goto L_2889;

L_2889:
    tutor.idtBold = 219;
    goto L_2898;

L_2892:
    tutor.idtBold = 217;

L_2898:
    return 0x0;

L_289e:
    tutor.idtBold = 221;
    if ((pctResGlob == -1))
        goto L_28c0;
    else
        goto L_28ae;

L_28ae:
    tutor.fProgress = 0x0;
    return 0x1;

L_28c0:
    return 0x0;

L_28c6:
    if ((pctResGlob == -1))
        goto L_28fb;
    else
        goto L_28d0;

L_28d0:
    if ((tutor.fProgress != 0x0))
        goto L_28ec;
    else
        goto L_28e3;

L_28e3:
    tutor.idtBold = 226;
    goto L_28f2;

L_28ec:
    tutor.idtBold = 228;

L_28f2:
    return 0x0;

L_28fb:
    if ((FCheckResearch(0x1, 0x6, 0x1e) != 0))
        goto L_2923;
    else
        goto L_2917;

L_2917:
    tutor.idtBold = 228;
    return 0x0;

L_2923:
    tutor.idtBold = 230;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_293e:
    if ((tutor.idt == 200))
        goto L_25ec;
    else
        goto L_2946;

L_2946:
    if ((tutor.idt == 208))
        goto L_26d9;
    else
        goto L_294e;

L_294e:
    if ((tutor.idt == 216))
        goto L_279d;
    else
        goto L_2956;

L_2956:
    if ((tutor.idt == 224))
        goto L_28c6;
    else
        goto L_295e;

L_295e:
    goto L_25e6;
    goto L_2b14;

L_2967:

L_296d:
    tutor.fNoErrors = 0x1;
    if ((FCheckResearch(0x1, 0x3, 0x1e) != 0))
        goto L_29bd;
    else
        goto L_2995;

L_2995:
    if ((pctResGlob != -1))
        goto L_29a5;
    else
        goto L_299f;

L_299f:
    t_merge_29a8_0001 = 0xe8;
    goto L_29a8;

L_29a5:
    t_merge_29a8_0001 = 0xec;

L_29a8:
    tutor.idtBold = t_merge_29a8_0001;
    tutor.fNoErrors = 0x0;
    return 0x0;

L_29bd:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 238;
    return FCheckMessages(0xffff, idmHasLoadedMiningRobotsWorking, 0x1);

L_29e6:
    if ((FCheckColonizeWP(0x9, 0x2, 0xffff) != 0))
        goto L_2a81;
    else
        goto L_2a02;

L_2a02:
    if ((FCheckSelection(grobjFleet, 0x9) == 0))
        goto L_2a57;
    else
        goto L_2a1a;

L_2a1a:
    if ((FCheckCargo(LpflFromId(0x9), 0x0, 0x0, 0x0, 0x19) == 0))
        goto L_2a4e;
    else
        goto L_2a48;

L_2a48:
    t_merge_2a51_0001 = 0xf3;
    goto L_2a51;

L_2a4e:
    t_merge_2a51_0001 = 0xf2;

L_2a51:
    tutor.idtBold = t_merge_2a51_0001;
    goto L_2a7b;

L_2a57:
    if ((FCheckSummary(grobjPlanet, 0x2) == 0))
        goto L_2a75;
    else
        goto L_2a6f;

L_2a6f:
    t_merge_2a78_0001 = 0xf1;
    goto L_2a78;

L_2a75:
    t_merge_2a78_0001 = 0xf0;

L_2a78:
    tutor.idtBold = t_merge_2a78_0001;

L_2a7b:
    return 0x0;

L_2a81:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x0, 0x1, grobjPlanet, 0x4, 0xffff, 0xffff) != 0))
        goto L_2aed;
    else
        goto L_2ab5;

L_2ab5:
    FCheckFleetWP(0x0, 0x1, grobjPlanet, 0x2, 0xffff, 0xffff);
    tutor.idtBold = 245;
    tutor.fNoErrors = 0x0;
    return 0x0;

L_2aed:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 247;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_2b14:
    if ((tutor.idt == 232))
        goto L_296d;
    else
        goto L_2b1c;

L_2b1c:
    if ((tutor.idt == 240))
        goto L_29e6;
    else
        goto L_2b24;

L_2b24:
    goto L_2967;
    goto L_2ddd;

L_2b2d:

L_2b33:
    if ((FCheckMessages(0xffff, idmHasDismantledKtMineralsWhichHaveDeposited, 0x1) != 0))
        goto L_2b67;
    else
        goto L_2b4f;

L_2b4f:
    tutor.idtBold = 248;
    tutor.fProgress = 0x0;
    return 0x0;

L_2b67:
    if ((FCheckQueue(0xe, 0x0, grobjPlanet, 0x1, 0x3, 0x1) == 0))
        goto L_2bb7;
    else
        goto L_2b8f;

L_2b8f:
    if ((FCheckQueue(0xe, 0x1, grobjPlanet, 0x0, 0x3, 0x1) != 0))
        goto L_2bfe;
    else
        goto L_2bb7;

L_2bb7:
    if ((FCheckSelection(grobjPlanet, 0xe) == 0))
        goto L_2bf2;
    else
        goto L_2bcf;

L_2bcf:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_2be9;
    else
        goto L_2bd9;

L_2bd9:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_2be9;
    else
        goto L_2be3;

L_2be3:
    t_merge_2bec_0001 = 0xfa;
    goto L_2bec;

L_2be9:
    t_merge_2bec_0001 = 0xfb;

L_2bec:
    tutor.idtBold = t_merge_2bec_0001;
    goto L_2bf8;

L_2bf2:
    tutor.idtBold = 249;

L_2bf8:
    return 0x0;

L_2bfe:
    if ((FCheckTemplate(0x0) == 0))
        goto L_2c18;
    else
        goto L_2c12;

L_2c12:
    return 0x1;

L_2c18:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_2c38;
    else
        goto L_2c22;

L_2c22:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_2c38;
    else
        goto L_2c2c;

L_2c2c:
    tutor.idtBold = 254;
    return 0x0;

L_2c38:
    if ((vyZPDStatic == -1))
        goto L_2c48;
    else
        goto L_2c42;

L_2c42:
    return 0x1;

L_2c48:
    tutor.idtBold = 255;
    return 0x0;

L_2c54:
    if ((FCheckTemplate(0x0) != 0))
        goto L_2c74;
    else
        goto L_2c68;

L_2c68:
    tutor.idtBold = 256;
    return 0x0;

L_2c74:
    if ((tutor.fProgress != 0x0))
        goto L_2ca7;
    else
        goto L_2c87;

L_2c87:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_2c9b;
    else
        goto L_2c91;

L_2c91:
    if ((HIWORD(lpplProdGlob) == 0x0))
        goto L_2ca7;
    else
        goto L_2c9b;

L_2c9b:
    tutor.idtBold = 257;
    return 0x0;

L_2ca7:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_2cf2;
    else
        goto L_2cca;

L_2cca:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x2, 0x1, 0x0) != 0))
        goto L_2d65;
    else
        goto L_2cf2;

L_2cf2:
    tutor.idh = 1059;
    if ((tutor.fProgress != 0x0))
        goto L_2d59;
    else
        goto L_2d0b;

L_2d0b:
    if ((FCheckSelection(grobjPlanet, 0xd) != 0))
        goto L_2d59;
    else
        goto L_2d23;

L_2d23:
    if ((FCheckSummary(grobjPlanet, 0x17) == 0))
        goto L_2d50;
    else
        goto L_2d3b;

L_2d3b:
    tutor.fProgress = 0x1;
    tutor.idtBold = 260;
    goto L_2d5f;

L_2d50:
    tutor.idtBold = 259;

L_2d59:
    tutor.idtBold = 260;

L_2d5f:
    return 0x0;

L_2d65:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_2db0;
    else
        goto L_2d88;

L_2d88:
    if ((FCheckQueue(0xd, 0x2, grobjFleet, 0x3, 0x1, 0x0) != 0))
        goto L_2dc2;
    else
        goto L_2db0;

L_2db0:
    tutor.idh = 1059;
    tutor.idtBold = 262;
    return 0x0;

L_2dc2:
    tutor.idtBold = 263;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_2ddd:
    if ((tutor.idt == 248))
        goto L_2b33;
    else
        goto L_2de5;

L_2de5:
    if ((tutor.idt == 256))
        goto L_2c54;
    else
        goto L_2ded;

L_2ded:
    goto L_2b2d;
    goto L_328c;

L_2df6:

L_2dfc:
    if ((FCheckFleetWP(0x0, 0x1, grobjPlanet, 0xa, 0xffff, 0xffff) != 0))
        goto L_2e4e;
    else
        goto L_2e24;

L_2e24:
    if ((FCheckSelection(grobjFleet, 0x0) == 0))
        goto L_2e42;
    else
        goto L_2e3c;

L_2e3c:
    t_merge_2e45_0001 = 0x109;
    goto L_2e45;

L_2e42:
    t_merge_2e45_0001 = 0x108;

L_2e45:
    tutor.idtBold = t_merge_2e45_0001;
    return 0x0;

L_2e4e:
    if ((FCheckFleetWP(0x1, 0x1, grobjPlanet, 0xd, 0x5, 0xffff) != 0))
        goto L_2ea0;
    else
        goto L_2e76;

L_2e76:
    if ((FCheckSelection(grobjFleet, 0x1) == 0))
        goto L_2e94;
    else
        goto L_2e8e;

L_2e8e:
    t_merge_2e97_0001 = 0x10c;
    goto L_2e97;

L_2e94:
    t_merge_2e97_0001 = 0x10a;

L_2e97:
    tutor.idtBold = t_merge_2e97_0001;
    return 0x0;

L_2ea0:
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_2ed4;
    else
        goto L_2ec8;

L_2ec8:
    tutor.idtBold = 269;
    return 0x0;

L_2ed4:
    if ((FCheckFleetWP(0x8, 0x1, grobjFleet, 0x200, 0xffff, 0xffff) != 0))
        goto L_2f08;
    else
        goto L_2efc;

L_2efc:
    tutor.idtBold = 271;
    return 0x0;

L_2f08:
    return 0x1;

L_2f0e:
    if ((FCheckColonizeWP(0x2, 0x4, 0xffff) != 0))
        goto L_2f54;
    else
        goto L_2f2a;

L_2f2a:
    if ((FCheckSelection(grobjFleet, 0x2) == 0))
        goto L_2f48;
    else
        goto L_2f42;

L_2f42:
    t_merge_2f4b_0001 = 0x112;
    goto L_2f4b;

L_2f48:
    t_merge_2f4b_0001 = 0x110;

L_2f4b:
    tutor.idtBold = t_merge_2f4b_0001;
    return 0x0;

L_2f54:
    if ((FCheckCargo(LpflFromId(0xb), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_2fac;
    else
        goto L_2f82;

L_2f82:
    if ((FCheckSelection(grobjFleet, 0xb) == 0))
        goto L_2fa0;
    else
        goto L_2f9a;

L_2f9a:
    t_merge_2fa3_0001 = 0x114;
    goto L_2fa3;

L_2fa0:
    t_merge_2fa3_0001 = 0x113;

L_2fa3:
    tutor.idtBold = t_merge_2fa3_0001;
    return 0x0;

L_2fac:
    if ((FCheckFleetWP(0xb, 0x1, grobjPlanet, 0x5, 0xffff, 0xffff) == 0))
        goto L_30bb;
    else
        goto L_2fd4;

L_2fd4:
    if ((FCheckFleetWP(0xb, 0x1, grobjPlanet, 0x5, 0x1, 0xffff) == 0))
        goto L_30b2;
    else
        goto L_2ffc;

L_2ffc:
    if ((FCheckXferWP(0xb, 0x1, 0x5, 0xffff, rgiaUnloadAllCol) == 0))
        goto L_3038;
    else
        goto L_3023;

L_3023:
    tutor.fProgress = 0x0;
    return 0x1;

L_3038:
    if ((FCheckSelection(grobjFleet, 0xb) == 0))
        goto L_30a9;
    else
        goto L_3050;

L_3050:
    if ((sel.fl.lpplord->rgord[sel.iwpAct].grTask != 0x1))
        goto L_30a9;
    else
        goto L_3077;

L_3077:
    t_call_308b = SendMessage(rghwndOrderDD[1], CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(t_call_308b) != 0x4))
        goto L_30a9;
    else
        goto L_3098;

L_3098:
    if ((HIWORD(t_call_308b) != 0x0))
        goto L_30a9;
    else
        goto L_30a0;

L_30a0:
    tutor.idtBold = 279;
    goto L_30c1;

L_30a9:
    tutor.idtBold = 278;

L_30b2:
    tutor.idtBold = 277;

L_30bb:
    tutor.idtBold = 276;

L_30c1:
    return 0x0;

L_30c7:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x2))
        goto L_3112;
    else
        goto L_30ea;

L_30ea:
    if ((FCheckQueue(0xd, 0x0, grobjPlanet, 0x8, 0x46, 0x0) != 0))
        goto L_311b;
    else
        goto L_3112;

L_3112:
    tutor.idtBold = 280;
    goto L_31fd;

L_311b:
    if ((FCheckResearch(0x3, 0x5, 0x1e) != 0))
        goto L_3150;
    else
        goto L_3137;

L_3137:
    if ((pctResGlob != -1))
        goto L_3147;
    else
        goto L_3141;

L_3141:
    t_merge_314a_0001 = 0x119;
    goto L_314a;

L_3147:
    t_merge_314a_0001 = 0x11a;

L_314a:
    tutor.idtBold = t_merge_314a_0001;
    goto L_31fd;

L_3150:
    if ((FCheckSelection(grobjFleet, 0x8) == 0))
        goto L_3171;
    else
        goto L_3168;

L_3168:
    return 0x1;

L_3171:
    if ((tutor.fProgress == 0x0))
        goto L_31aa;
    else
        goto L_3184;

L_3184:
    if ((FCheckMessages(0xb, 0xffff, 0x0) == 0))
        goto L_31aa;
    else
        goto L_31a0;

L_31a0:
    if ((hwndBrowser == 0x0))
        goto L_31d2;
    else
        goto L_31aa;

L_31aa:
    if ((hwndBrowser != 0x0))
        goto L_31bd;
    else
        goto L_31b4;

L_31b4:
    tutor.idtBold = 283;
    goto L_31fd;

L_31bd:
    tutor.idtBold = 284;
    tutor.fProgress = 0x1;

L_31d2:
    if ((FCheckMessages(0xd, 0xffff, 0x0) != 0))
        goto L_31f7;
    else
        goto L_31ee;

L_31ee:
    tutor.idtBold = 285;
    goto L_31fd;

L_31f7:
    tutor.idtBold = 286;

L_31fd:
    return 0x0;

L_3203:
    if ((tutor.fProgress != 0x0))
        goto L_324f;
    else
        goto L_3216;

L_3216:
    if ((tutor.fAutoComplete != 0x0))
        goto L_324f;
    else
        goto L_3229;

L_3229:
    if ((LOWORD(vrgtok) != 0x0))
        goto L_3243;
    else
        goto L_3233;

L_3233:
    if ((HIWORD(vrgtok) != 0x0))
        goto L_3243;
    else
        goto L_323d;

L_323d:
    t_merge_3246_0001 = 0x121;
    goto L_3246;

L_3243:
    t_merge_3246_0001 = 0x122;

L_3246:
    tutor.idtBold = t_merge_3246_0001;
    return 0x0;

L_324f:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) == 0))
        goto L_3271;
    else
        goto L_326b;

L_326b:
    t_merge_3274_0001 = 0x127;
    goto L_3274;

L_3271:
    t_merge_3274_0001 = 0x123;

L_3274:
    tutor.idtBold = t_merge_3274_0001;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_328c:
    if ((tutor.idt == 264))
        goto L_2dfc;
    else
        goto L_3294;

L_3294:
    if ((tutor.idt == 272))
        goto L_2f0e;
    else
        goto L_329c;

L_329c:
    if ((tutor.idt == 280))
        goto L_30c7;
    else
        goto L_32a4;

L_32a4:
    if ((tutor.idt == 288))
        goto L_3203;
    else
        goto L_32ac;

L_32ac:
    goto L_2df6;
    goto L_34b9;

L_32b5:

L_32bb:
    if ((LpplFromId(0x8)->lpplprod->iprodMac <= 0x2))
        goto L_32e4;
    else
        goto L_32de;

L_32de:
    return 0x1;

L_32e4:
    if ((FCheckSummary(grobjFleet, 0x3) == 0))
        goto L_3302;
    else
        goto L_32fc;

L_32fc:
    return 0x1;

L_3302:
    if ((FCheckSummary(grobjThing, 0xffff) == 0))
        goto L_3323;
    else
        goto L_331a;

L_331a:
    tutor.idtBold = 303;
    goto L_3366;

L_3323:
    if ((FCheckMessages(0x1, 0xffff, 0x0) == 0))
        goto L_3360;
    else
        goto L_333f;

L_333f:
    if ((FCheckSelection(grobjFleet, 0x8) == 0))
        goto L_3360;
    else
        goto L_3357;

L_3357:
    tutor.idtBold = 300;
    goto L_3366;

L_3360:
    tutor.idtBold = 297;

L_3366:
    return 0x0;

L_336c:
    if ((FCheckMessages(0x5, 0xffff, 0x0) != 0))
        goto L_3394;
    else
        goto L_3388;

L_3388:
    tutor.idtBold = 306;
    return 0x0;

L_3394:
    if ((FCheckMessages(0x6, 0xffff, 0x0) != 0))
        goto L_33bc;
    else
        goto L_33b0;

L_33b0:
    tutor.idtBold = 308;
    return 0x0;

L_33bc:
    tutor.idtBold = 310;
    return FCheckSelection(grobjPlanet, 0x12);

L_33d5:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_3420;
    else
        goto L_33f8;

L_33f8:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x3, 0x2, 0x0) != 0))
        goto L_349e;
    else
        goto L_3420;

L_3420:
    if ((LpplFromId(0x8)->lpplprod->iprodMac < 0x3))
        goto L_346b;
    else
        goto L_3443;

L_3443:
    if ((FCheckQueue(0x8, 0x0, grobjPlanet, 0xc, 0x2, 0x1) != 0))
        goto L_3492;
    else
        goto L_346b;

L_346b:
    if ((FCheckSelection(grobjPlanet, 0x8) == 0))
        goto L_3489;
    else
        goto L_3483;

L_3483:
    t_merge_348c_0001 = 0x13c;
    goto L_348c;

L_3489:
    t_merge_348c_0001 = 0x139;

L_348c:
    tutor.idtBold = t_merge_348c_0001;
    goto L_3498;

L_3492:
    tutor.idtBold = 318;

L_3498:
    return 0x0;

L_349e:
    tutor.idtBold = 319;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_34b9:
    if ((tutor.idt == 296))
        goto L_32bb;
    else
        goto L_34c1;

L_34c1:
    if ((tutor.idt == 304))
        goto L_336c;
    else
        goto L_34c9;

L_34c9:
    if ((tutor.idt == 312))
        goto L_33d5;
    else
        goto L_34d1;

L_34d1:
    goto L_32b5;
    goto L_397e;

L_34da:

L_34e0:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x7 ? L_34f2 : L_34ec */

L_34ec:
    return 0x1;

L_34f2:
    if ((FCheckXferWP(0x0, 0x1, 0x8, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_3568;
    else
        goto L_3519;

L_3519:
    if ((FCheckCargo(LpflFromId(0x0), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_3550;
    else
        goto L_3547;

L_3547:
    tutor.idtBold = 320;
    goto L_3556;

L_3550:
    tutor.idtBold = 321;

L_3556:
    tutor.fProgress = 0x0;
    return 0x0;

L_3568:
    if ((FCheckMessages(0x3, 0xffff, 0x0) != 0))
        goto L_3590;
    else
        goto L_3584;

L_3584:
    tutor.idtBold = 322;
    return 0x0;

L_3590:
    if ((FCheckResearch(0x5, 0x2, 0x1e) != 0))
        goto L_35b8;
    else
        goto L_35ac;

L_35ac:
    tutor.idtBold = 324;
    return 0x0;

L_35b8:
    if ((tutor.fProgress != 0x0))
        goto L_3609;
    else
        goto L_35cb;

L_35cb:
    if ((tutor.fAutoComplete != 0x0))
        goto L_3609;
    else
        goto L_35de;

L_35de:
    if ((hwndBrowser != 0x0))
        goto L_35f1;
    else
        goto L_35e8;

L_35e8:
    tutor.idtBold = 325;
    goto L_3603;

L_35f1:
    tutor.idtBold = 327;
    tutor.fProgress = 0x1;

L_3603:
    return 0x0;

L_3609:
    tutor.idtBold = 327;
    if ((hwndSlotDlg == 0x0))
        goto L_361f;
    else
        goto L_3619;

L_3619:
    return 0x1;

L_361f:
    return 0x0;

L_3625:
    hs.grhst = hstEngine;
    HIWORD(hs) = 0x103;
    hs1.grhst = hstScanner;
    HIWORD(hs1) = 0x101;
    if ((hwndSlotDlg != 0x0))
        goto L_3655;
    else
        goto L_3643;

L_3643:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x7 ? L_3655 : L_364f */

L_364f:
    return 0x1;

L_3655:
    if ((FCheckShipBuilder(0x4, 0xffff) != 0))
        goto L_36b8;
    else
        goto L_366d;

L_366d:
    if ((FCheckShipBuilder(0x1, 0x7) == 0))
        goto L_368e;
    else
        goto L_3685;

L_3685:
    tutor.idtBold = 330;
    goto L_36b2;

L_368e:
    if ((FCheckShipBuilder(0x1, 0xffff) == 0))
        goto L_36ac;
    else
        goto L_36a6;

L_36a6:
    t_merge_36af_0001 = 0x149;
    goto L_36af;

L_36ac:
    t_merge_36af_0001 = 0x148;

L_36af:
    tutor.idtBold = t_merge_36af_0001;

L_36b2:
    return 0x0;

L_36b8:
    tutor.idh = 3039;
    if ((lpshdefBuild->hul.rghs[0].cItem == 0x0))
        goto L_36f2;
    else
        goto L_36d6;

L_36d6:
    if ((FCheckBuilderPart(0x0, &(hs), 0x1) != 0))
        goto L_36fe;
    else
        goto L_36f2;

L_36f2:
    tutor.idtBold = 332;
    return 0x0;

L_36fe:
    if ((lpshdefBuild->hul.rghs[1].cItem == 0x0))
        goto L_3732;
    else
        goto L_3716;

L_3716:
    if ((FCheckBuilderPart(0x1, &(hs1), 0x1) != 0))
        goto L_373e;
    else
        goto L_3732;

L_3732:
    tutor.idtBold = 333;
    return 0x0;

L_373e:
    if ((lpshdefBuild->hul.rghs[2].cItem == 0x0))
        goto L_376e;
    else
        goto L_3756;

L_3756:
    if ((lpshdefBuild->hul.rghs[3].cItem != 0x0))
        goto L_379e;
    else
        goto L_376e;

L_376e:
    if ((FCheckShipBuilder(0x4, 0x8) != 0))
        goto L_3792;
    else
        goto L_3786;

L_3786:
    tutor.idtBold = 334;
    return 0x0;

L_3792:
    tutor.idtBold = 335;
    return 0x0;

L_379e:
    return 0x1;

L_37a4:
    if ((hwndSlotDlg == 0x0))
        goto L_37d8;
    else
        goto L_37ae;

L_37ae:
    if ((FCheckShipBuilder(0x4, 0xffff) == 0))
        goto L_37cc;
    else
        goto L_37c6;

L_37c6:
    t_merge_37cf_0001 = 0x151;
    goto L_37cf;

L_37cc:
    t_merge_37cf_0001 = 0x152;

L_37cf:
    tutor.idtBold = t_merge_37cf_0001;
    return 0x0;

L_37d8:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_3846;
    else
        goto L_37fb;

L_37fb:
    if ((LpplFromId(0xd)->lpplprod->iprodMac != 0x3))
        goto L_3852;
    else
        goto L_381e;

L_381e:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x6, 0x1, 0x0) != 0))
        goto L_3852;
    else
        goto L_3846;

L_3846:
    tutor.idtBold = 339;
    return 0x0;

L_3852:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_389d;
    else
        goto L_3875;

L_3875:
    if ((FCheckQueue(0xd, 0x0, grobjPlanet, 0x8, 0x64, 0x0) != 0))
        goto L_38cf;
    else
        goto L_389d;

L_389d:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_38b7;
    else
        goto L_38a7;

L_38a7:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_38b7;
    else
        goto L_38b1;

L_38b1:
    t_merge_38ba_0001 = 0x155;
    goto L_38ba;

L_38b7:
    t_merge_38ba_0001 = 0x156;

L_38ba:
    tutor.idtBold = t_merge_38ba_0001;
    tutor.fProgress = 0x0;
    return 0x0;

L_38cf:
    return 0x1;

L_38d5:
    if ((tutor.fProgress != 0x0))
        goto L_3907;
    else
        goto L_38e8;

L_38e8:
    if ((tutor.fAutoComplete != 0x0))
        goto L_3907;
    else
        goto L_38fb;

L_38fb:
    tutor.idtBold = 344;
    return 0x0;

L_3907:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_392f;
    else
        goto L_3923;

L_3923:
    tutor.idtBold = 345;
    return 0x0;

L_392f:
    if ((FCheckFleetWP(0x8, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_3963;
    else
        goto L_3957;

L_3957:
    tutor.idtBold = 348;
    return 0x0;

L_3963:
    tutor.idtBold = 350;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_397e:
    if ((tutor.idt == 320))
        goto L_34e0;
    else
        goto L_3986;

L_3986:
    if ((tutor.idt == 328))
        goto L_3625;
    else
        goto L_398e;

L_398e:
    if ((tutor.idt == 336))
        goto L_37a4;
    else
        goto L_3996;

L_3996:
    if ((tutor.idt == 344))
        goto L_38d5;
    else
        goto L_399e;

L_399e:
    goto L_34da;
    goto L_3a75;

L_39a7:

L_39ad:
    if ((LpplFromId(0x2)->lpplprod->iprodMac < 0x3))
        goto L_3a0c;
    else
        goto L_39d0;

L_39d0:
    if ((FCheckQueue(0x2, 0x0, grobjPlanet, 0x4, 0x2, 0x1) == 0))
        goto L_3a0c;
    else
        goto L_39f8;

L_39f8:
    if ((FCheckTemplate(0x1) != 0))
        goto L_3a5a;
    else
        goto L_3a0c;

L_3a0c:
    if ((FCheckSelection(grobjPlanet, 0x2) == 0))
        goto L_3a2d;
    else
        goto L_3a24;

L_3a24:
    tutor.idtBold = 356;
    goto L_3a54;

L_3a2d:
    if ((FCheckSelection(grobjPlanet, 0x11) == 0))
        goto L_3a4e;
    else
        goto L_3a45;

L_3a45:
    tutor.idtBold = 354;
    goto L_3a54;

L_3a4e:
    tutor.idtBold = 352;

L_3a54:
    return 0x0;

L_3a5a:
    tutor.idtBold = 358;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_3a75:
    if ((tutor.idt == 352))
        goto L_39ad;
    else
        goto L_3a7d;

L_3a7d:
    goto L_39a7;
    goto L_3c46;

L_3a86:

L_3a8c:
    if ((FCheckFleetWP(0x8, 0x1, grobjPlanet, 0xb, 0xffff, 0xffff) != 0))
        goto L_3ac0;
    else
        goto L_3ab4;

L_3ab4:
    tutor.idtBold = 360;
    return 0x0;

L_3ac0:
    if ((FCheckCargo(LpflFromId(0x6), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_3b06;
    else
        goto L_3aee;

L_3aee:
    tutor.idtBold = 361;
    tutor.fProgress = 0x0;
    return 0x0;

L_3b06:
    if ((tutor.fAutoComplete != 0x0))
        goto L_3b6d;
    else
        goto L_3b19;

L_3b19:
    if ((tutor.fProgress != 0x0))
        goto L_3b6d;
    else
        goto L_3b2c;

L_3b2c:
    if ((vprptCur != 0x0))
        goto L_3b3f;
    else
        goto L_3b36;

L_3b36:
    tutor.idtBold = 363;
    goto L_3b67;

L_3b3f:
    tutor.fProgress = 0x1;
    if ((vprptCur->icolSort != 4))
        goto L_3b61;
    else
        goto L_3b58;

L_3b58:
    tutor.idtBold = 366;
    goto L_3b67;

L_3b61:
    tutor.idtBold = 364;

L_3b67:
    return 0x0;

L_3b6d:
    if ((FCheckXferWP(0x6, 0x1, 0x5, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_3ba0;
    else
        goto L_3b94;

L_3b94:
    tutor.idtBold = 367;
    return 0x0;

L_3ba0:
    return 0x1;

L_3ba6:
    if ((tutor.fAutoComplete != 0x0))
        goto L_3bf7;
    else
        goto L_3bb9;

L_3bb9:
    if ((tutor.fProgress != 0x0))
        goto L_3bf7;
    else
        goto L_3bcc;

L_3bcc:
    if ((pctResGlob == -1))
        goto L_3beb;
    else
        goto L_3bd6;

L_3bd6:
    tutor.fProgress = 0x1;
    tutor.idtBold = 371;
    goto L_3bf1;

L_3beb:
    tutor.idtBold = 368;

L_3bf1:
    return 0x0;

L_3bf7:
    if ((FCheckFleetWP(0xb, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_3c2b;
    else
        goto L_3c1f;

L_3c1f:
    tutor.idtBold = 372;
    return 0x0;

L_3c2b:
    tutor.idtBold = 374;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_3c46:
    if ((tutor.idt == 360))
        goto L_3a8c;
    else
        goto L_3c4e;

L_3c4e:
    if ((tutor.idt == 368))
        goto L_3ba6;
    else
        goto L_3c56;

L_3c56:
    goto L_3a86;
    goto L_3e45;

L_3c5f:

L_3c65:
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0x5, 0xffff, 0xffff) != 0))
        goto L_3c99;
    else
        goto L_3c8d;

L_3c8d:
    tutor.idtBold = 376;
    return 0x0;

L_3c99:
    tutor.fNoErrors = 0x1;
    if ((LpflFromId(0x7)->cord != 1))
        goto L_3ce9;
    else
        goto L_3cbf;

L_3cbf:
    if ((FCheckSelection(grobjFleet, 0x7) == 0))
        goto L_3cdd;
    else
        goto L_3cd7;

L_3cd7:
    t_merge_3ce0_0001 = 0x17a;
    goto L_3ce0;

L_3cdd:
    t_merge_3ce0_0001 = 0x179;

L_3ce0:
    tutor.idtBold = t_merge_3ce0_0001;
    return 0x0;

L_3ce9:
    if ((FCheckFleetWP(0x7, 0x1, grobjPlanet, 0xc, 0xffff, 0xffff) == 0))
        goto L_3d29;
    else
        goto L_3d11;

L_3d11:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 378;
    return 0x0;

L_3d29:
    tutor.fNoErrors = 0x0;
    if ((FCheckFleetWP(0x7, 0x1, grobjFleet, 0x5, 0x4, 0xffff) != 0))
        goto L_3d69;
    else
        goto L_3d5d;

L_3d5d:
    tutor.idtBold = 378;
    return 0x0;

L_3d69:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x2))
        goto L_3ddc;
    else
        goto L_3d8c;

L_3d8c:
    if ((FCheckQueue(0xd, 0x0, grobjPlanet, 0x1, 0x3c, 0x0) == 0))
        goto L_3ddc;
    else
        goto L_3db4;

L_3db4:
    if ((FCheckQueue(0xd, 0x1, grobjPlanet, 0x0, 0x3c, 0x0) != 0))
        goto L_3e02;
    else
        goto L_3ddc;

L_3ddc:
    if ((LOWORD(lpplProdGlob) != 0x0))
        goto L_3df6;
    else
        goto L_3de6;

L_3de6:
    if ((HIWORD(lpplProdGlob) != 0x0))
        goto L_3df6;
    else
        goto L_3df0;

L_3df0:
    t_merge_3df9_0001 = 0x17c;
    goto L_3df9;

L_3df6:
    t_merge_3df9_0001 = 0x17d;

L_3df9:
    tutor.idtBold = t_merge_3df9_0001;
    return 0x0;

L_3e02:
    if ((FCheckResearch(0x2, 0x3, 0x1e) != 0))
        goto L_3e2a;
    else
        goto L_3e1e;

L_3e1e:
    tutor.idtBold = 382;
    return 0x0;

L_3e2a:
    tutor.idtBold = 383;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_3e45:
    if ((tutor.idt == 376))
        goto L_3c65;
    else
        goto L_3e4d;

L_3e4d:
    goto L_3c5f;
    goto L_3ecd;

L_3e56:

L_3e5c:
    if ((FCheckFleetWP(0x0, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_3eb2;
    else
        goto L_3e84;

L_3e84:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) == 0))
        goto L_3ea6;
    else
        goto L_3ea0;

L_3ea0:
    t_merge_3ea9_0001 = 0x181;
    goto L_3ea9;

L_3ea6:
    t_merge_3ea9_0001 = 0x180;

L_3ea9:
    tutor.idtBold = t_merge_3ea9_0001;
    return 0x0;

L_3eb2:
    tutor.idtBold = 387;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_3ecd:
    if ((tutor.idt == 384))
        goto L_3e5c;
    else
        goto L_3ed5;

L_3ed5:
    goto L_3e56;
    goto L_4009;

L_3ede:

L_3ee4:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_3f2f;
    else
        goto L_3f07;

L_3f07:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x3, 0x1, 0x0) != 0))
        goto L_3f3b;
    else
        goto L_3f2f;

L_3f2f:
    tutor.idtBold = 392;
    return 0x0;

L_3f3b:
    if ((FCheckMessages(0x5, 0xffff, 0x0) != 0))
        goto L_3f6f;
    else
        goto L_3f57;

L_3f57:
    tutor.fProgress = 0x0;
    tutor.idtBold = 393;
    return 0x0;

L_3f6f:
    if ((tutor.fAutoComplete != 0x0))
        goto L_3fd8;
    else
        goto L_3f82;

L_3f82:
    if ((tutor.fProgress != 0x0))
        goto L_3fd8;
    else
        goto L_3f95;

L_3f95:
    if ((FCheckSelection(grobjPlanet, 0x10) == 0))
        goto L_3fc9;
    else
        goto L_3fad;

L_3fad:
    if ((pctResGlob == -1))
        goto L_3fc0;
    else
        goto L_3fb7;

L_3fb7:
    tutor.idtBold = 396;
    goto L_3fcf;

L_3fc0:
    tutor.idtBold = 395;

L_3fc9:
    tutor.idtBold = 393;

L_3fcf:
    return 0x0;

L_3fd8:
    if ((pctResGlob == -1))
        goto L_3fee;
    else
        goto L_3fe2;

L_3fe2:
    tutor.idtBold = 398;
    return 0x0;

L_3fee:
    tutor.idtBold = 399;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_4009:
    if ((tutor.idt == 392))
        goto L_3ee4;
    else
        goto L_4011;

L_4011:
    goto L_3ede;
    goto L_42de;

L_401a:

L_4020:
    if ((FCheckCargo(LpflFromId(0xb), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_405a;
    else
        goto L_404e;

L_404e:
    tutor.idtBold = 400;
    return 0x0;

L_405a:
    if ((FCheckXferWP(0xb, 0x1, 0x5, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_408d;
    else
        goto L_4081;

L_4081:
    tutor.idtBold = 401;
    return 0x0;

L_408d:
    tutor.fNoErrors = 0x1;
    if ((FCheckXferWP(0xb, 0x2, 0xd, 0xffff, rgiaLoadAllCol) != 0))
        goto L_40d8;
    else
        goto L_40c0;

L_40c0:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 402;
    return 0x0;

L_40d8:
    tutor.fNoErrors = 0x0;
    tutor.idh = 1518;
    tutor.idtBold = 403;
    if ((((*(LpflFromId(0xb) + 0x4) >> 0x9) & 0x1) != 0x0))
        goto L_411a;
    else
        goto L_4114;

L_4114:
    return 0x0;

L_411a:
    if ((FCheckCargo(LpflFromId(0x1), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_4172;
    else
        goto L_4148;

L_4148:
    if ((FCheckSelection(grobjFleet, 0x1) == 0))
        goto L_4166;
    else
        goto L_4160;

L_4160:
    t_merge_4169_0001 = 0x197;
    goto L_4169;

L_4166:
    t_merge_4169_0001 = 0x196;

L_4169:
    tutor.idtBold = t_merge_4169_0001;
    return 0x0;

L_4172:
    return 0x1;

L_4178:
    if ((FCheckXferWP(0x1, 0x1, 0x2, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_41ab;
    else
        goto L_419f;

L_419f:
    tutor.idtBold = 408;
    return 0x0;

L_41ab:
    tutor.fNoErrors = 0x1;
    if ((FCheckXferWP(0x1, 0x2, 0xd, 0xffff, rgiaLoadAllCol) != 0))
        goto L_41f6;
    else
        goto L_41de;

L_41de:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 409;
    return 0x0;

L_41f6:
    tutor.fNoErrors = 0x0;
    tutor.idh = 1518;
    tutor.idtBold = 410;
    if ((((*(LpflFromId(0x1) + 0x4) >> 0x9) & 0x1) != 0x0))
        goto L_4238;
    else
        goto L_4232;

L_4232:
    return 0x0;

L_4238:
    if ((LpplFromId(0x10)->lpplprod->iprodMac < 0x3))
        goto L_4283;
    else
        goto L_425b;

L_425b:
    if ((FCheckQueue(0x10, 0x2, grobjPlanet, 0x5, 0x1, 0x1) != 0))
        goto L_428f;
    else
        goto L_4283;

L_4283:
    tutor.idtBold = 412;
    return 0x0;

L_428f:
    if ((FCheckFleetWP(0x6, 0x1, grobjPlanet, 0xd, 0xffff, 0xffff) != 0))
        goto L_42c3;
    else
        goto L_42b7;

L_42b7:
    tutor.idtBold = 413;
    return 0x0;

L_42c3:
    tutor.idtBold = 414;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_42de:
    if ((tutor.idt == 400))
        goto L_4020;
    else
        goto L_42e6;

L_42e6:
    if ((tutor.idt == 408))
        goto L_4178;
    else
        goto L_42ee;

L_42ee:
    goto L_401a;
    goto L_4479;

L_42f7:

L_42fd:
    if ((FCheckFleetWP(0x8, 0x1, grobjPlanet, 0x6, 0xffff, 0xffff) != 0))
        goto L_4331;
    else
        goto L_4325;

L_4325:
    tutor.idtBold = 416;
    return 0x0;

L_4331:
    if ((FCheckResearch(0x3, 0x1, 0x1e) != 0))
        goto L_4359;
    else
        goto L_434d;

L_434d:
    tutor.idtBold = 417;
    return 0x0;

L_4359:
    if ((rgplr[0].cshdefSB == 0x1))
        goto L_4376;
    else
        goto L_436c;

L_436c:
    if ((hwndSlotDlg == 0x0))
        goto L_4407;
    else
        goto L_4376;

L_4376:
    if ((hwndSlotDlg != 0x0))
        goto L_439c;
    else
        goto L_4380;

L_4380:
    if ((rgplr[0].cshdefSB != 0x1))
        goto L_439c;
    else
        goto L_4393;

L_4393:
    tutor.idtBold = 418;
    goto L_4401;

L_439c:
    if ((FCheckShipBuilder(0x4, 0xffff) == 0))
        goto L_43be;
    else
        goto L_43b4;

L_43b4:
    if ((fStarbaseMode != 0))
        goto L_43da;
    else
        goto L_43be;

L_43be:
    if ((rgplr[0].cshdefSB != 0x1))
        goto L_43da;
    else
        goto L_43d1;

L_43d1:
    tutor.idtBold = 419;
    goto L_4401;

L_43da:
    if ((lpshdefBuild->hul.rghs[0].cItem != 0x0))
        goto L_43fb;
    else
        goto L_43f2;

L_43f2:
    tutor.idtBold = 420;
    goto L_4401;

L_43fb:
    tutor.idtBold = 421;

L_4401:
    return 0x0;

L_4407:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_4452;
    else
        goto L_442a;

L_442a:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x11, 0x1, 0x0) != 0))
        goto L_445e;
    else
        goto L_4452;

L_4452:
    tutor.idtBold = 422;
    return 0x0;

L_445e:
    tutor.idtBold = 423;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_4479:
    if ((tutor.idt == 416))
        goto L_42fd;
    else
        goto L_4481;

L_4481:
    goto L_42f7;
    goto L_468f;

L_448a:

L_4490:
    if ((FCheckCargo(LpflFromId(0x0), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_44ca;
    else
        goto L_44be;

L_44be:
    tutor.idtBold = 424;
    return 0x0;

L_44ca:
    if ((FCheckXferWP(0x0, 0x1, 0x5, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_44fd;
    else
        goto L_44f1;

L_44f1:
    tutor.idtBold = 425;
    return 0x0;

L_44fd:
    if ((FCheckZip(0x0, rgiaUnloadAllCol, idsDropcol) != 0))
        goto L_4538;
    else
        goto L_451c;

L_451c:
    if ((hwndZipOrderDlg != 0x0))
        goto L_452c;
    else
        goto L_4526;

L_4526:
    t_merge_452f_0001 = 0x1ab;
    goto L_452f;

L_452c:
    t_merge_452f_0001 = 0x1ac;

L_452f:
    tutor.idtBold = t_merge_452f_0001;
    return 0x0;

L_4538:
    tutor.fNoErrors = 0x1;
    if ((FCheckXferWP(0x0, 0x2, 0xd, 0xffff, rgiaLoadAllCol) != 0))
        goto L_4583;
    else
        goto L_456b;

L_456b:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 430;
    return 0x0;

L_4583:
    tutor.fNoErrors = 0x0;
    tutor.idh = 1518;
    tutor.idtBold = 431;
    if ((((*(LpflFromId(0x0) + 0x4) >> 0x9) & 0x1) != 0x0))
        goto L_45c5;
    else
        goto L_45bf;

L_45bf:
    return 0x0;

L_45c5:
    return 0x1;

L_45cb:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_45f3;
    else
        goto L_45e7;

L_45e7:
    tutor.idtBold = 432;
    return 0x0;

L_45f3:
    if ((tutor.fAutoComplete != 0x0))
        goto L_4674;
    else
        goto L_4606;

L_4606:
    if ((tutor.fProgress != 0x0))
        goto L_4674;
    else
        goto L_4619;

L_4619:
    if ((vprptCur != 0x0))
        goto L_462c;
    else
        goto L_4623;

L_4623:
    tutor.idtBold = 434;
    goto L_466e;

L_462c:
    if ((vprptCur->icolSort != 11))
        goto L_4668;
    else
        goto L_4639;

L_4639:
    if ((vprptCur->fAscending != 0))
        goto L_4668;
    else
        goto L_4646;

L_4646:
    if ((vprptCur->iSubsort != 3))
        goto L_4668;
    else
        goto L_4653;

L_4653:
    tutor.idtBold = 439;
    tutor.fProgress = 0x1;
    goto L_466e;

L_4668:
    tutor.idtBold = 435;

L_466e:
    return 0x0;

L_4674:
    tutor.idtBold = 439;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_468f:
    if ((tutor.idt == 424))
        goto L_4490;
    else
        goto L_4697;

L_4697:
    if ((tutor.idt == 432))
        goto L_45cb;
    else
        goto L_469f;

L_469f:
    goto L_448a;
    goto L_49b5;

L_46a8:

L_46ae:
    t_call_46b2 = LpflFromId(0x3);
    if ((LOWORD(t_call_46b2->rgwtMin[0x4]) != 0x17f))
        goto L_46d3;
    else
        goto L_46c9;

L_46c9:
    if ((HIWORD(t_call_46b2->rgwtMin[0x4]) == 0x0))
        goto L_4706;
    else
        goto L_46d3;

L_46d3:
    if ((FCheckSelection(grobjFleet, 0x3) != 0))
        goto L_46f4;
    else
        goto L_46eb;

L_46eb:
    tutor.idtBold = 440;
    goto L_4700;

L_46f4:
    tutor.idtBold = 445;
    tutor.idh = 1517;

L_4700:
    return 0x0;

L_4706:
    return 0x1;

L_470c:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_4757;
    else
        goto L_472f;

L_472f:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x3, 0x1, 0x0) != 0))
        goto L_4763;
    else
        goto L_4757;

L_4757:
    tutor.idtBold = 448;
    return 0x0;

L_4763:
    if ((FCheckResearch(0x1, 0x2, 0x1e) != 0))
        goto L_478b;
    else
        goto L_477f;

L_477f:
    tutor.idtBold = 449;
    return 0x0;

L_478b:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_47b3;
    else
        goto L_47a7;

L_47a7:
    tutor.idtBold = 450;
    return 0x0;

L_47b3:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) < 0x8 ? L_47c9 : L_47bf */

L_47bf:
    if ((hwndSlotDlg == 0x0))
        goto L_48b5;
    else
        goto L_47c9;

L_47c9:
    hs.grhst = hstEngine;
    HIWORD(hs) = 0x104;
    hs1.grhst = hstMines;
    HIWORD(hs1) = 0x301;
    if ((hwndSlotDlg != 0x0))
        goto L_47f9;
    else
        goto L_47e7;

L_47e7:
    tutor.idtBold = 450;
    tutor.idh = 1001;
    return 0x0;

L_47f9:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) >= 0x8 ? L_4829 : L_4805 */

L_4805:
    if ((FCheckShipBuilder(0x4, 0xffff) != 0))
        goto L_4829;
    else
        goto L_481d;

L_481d:
    tutor.idtBold = 451;
    return 0x0;

L_4829:
    tutor.idh = 3039;
    if ((lpshdefBuild->hul.rghs[0].cItem == 0x0))
        goto L_4863;
    else
        goto L_4847;

L_4847:
    if ((FCheckBuilderPart(0x0, &(hs), 0x1) != 0))
        goto L_486c;
    else
        goto L_4863;

L_4863:
    tutor.idtBold = 452;
    goto L_48af;

L_486c:
    if ((lpshdefBuild->hul.rghs[2].cItem == 0x0))
        goto L_48a0;
    else
        goto L_4884;

L_4884:
    if ((FCheckBuilderPart(0x2, &(hs1), 0x1) != 0))
        goto L_48a9;
    else
        goto L_48a0;

L_48a0:
    tutor.idtBold = 453;
    goto L_48af;

L_48a9:
    tutor.idtBold = 454;

L_48af:
    return 0x0;

L_48b5:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x5))
        goto L_4900;
    else
        goto L_48d8;

L_48d8:
    if ((FCheckQueue(0xd, 0x2, grobjFleet, 0x7, 0x1, 0x0) != 0))
        goto L_490c;
    else
        goto L_4900;

L_4900:
    tutor.idtBold = 455;
    return 0x0;

L_490c:
    tutor.idtBold = 455;
    return 0x1;

L_4918:
    if ((FCheckFleetWP(0x4, 0x1, grobjFleet, 0x204, 0xffff, 0xffff) != 0))
        goto L_498e;
    else
        goto L_4940;

L_4940:
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_4961;
    else
        goto L_4958;

L_4958:
    tutor.idtBold = 459;
    goto L_4988;

L_4961:
    if ((FCheckSummary(grobjFleet, 0x204) == 0))
        goto L_4982;
    else
        goto L_4979;

L_4979:
    tutor.idtBold = 458;
    goto L_4988;

L_4982:
    tutor.idtBold = 456;

L_4988:
    return 0x0;

L_498e:
    tutor.idtBold = 461;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_49b5:
    if ((tutor.idt == 440))
        goto L_46ae;
    else
        goto L_49bd;

L_49bd:
    if ((tutor.idt == 448))
        goto L_470c;
    else
        goto L_49c5;

L_49c5:
    if ((tutor.idt == 456))
        goto L_4918;
    else
        goto L_49cd;

L_49cd:
    goto L_46a8;
    goto L_4f4f;

L_49d6:

L_49dc:
    if ((FCheckCargo(LpflFromId(0x6), 0x0, 0x0, 0x0, 0xd2) == 0))
        goto L_4a10;
    else
        goto L_4a0a;

L_4a0a:
    return 0x1;

L_4a10:
    if ((FCheckMessages(0x1, 0xffff, 0x0) != 0))
        goto L_4a5f;
    else
        goto L_4a2c;

L_4a2c:
    if ((FCheckSelection(grobjFleet, 0x4) != 0))
        goto L_4a5f;
    else
        goto L_4a44;

L_4a44:
    tutor.idtBold = 464;
    tutor.fProgress = 0x0;
    return 0x0;

L_4a5f:
    if ((FCheckMessages(0x2, 0xffff, 0x0) != 0))
        goto L_4aa2;
    else
        goto L_4a7b;

L_4a7b:
    if ((FCheckSelection(grobjFleet, 0x6) != 0))
        goto L_4aa2;
    else
        goto L_4a93;

L_4a93:
    tutor.idtBold = 468;
    return 0x0;

L_4aa2:
    if ((tutor.fAutoComplete != 0x0))
        goto L_4b09;
    else
        goto L_4ab5;

L_4ab5:
    if ((tutor.fProgress != 0x0))
        goto L_4b09;
    else
        goto L_4ac8;

L_4ac8:
    if ((vprptCur != 0x0))
        goto L_4adb;
    else
        goto L_4ad2;

L_4ad2:
    tutor.idtBold = 469;
    goto L_4b03;

L_4adb:
    if ((vprptCur->icolSort != 2))
        goto L_4afd;
    else
        goto L_4ae8;

L_4ae8:
    tutor.idtBold = 471;
    tutor.fProgress = 0x1;
    goto L_4b03;

L_4afd:
    tutor.idtBold = 469;

L_4b03:
    return 0x0;

L_4b09:
    return 0x1;

L_4b0f:
    if ((FCheckCargo(LpflFromId(0x6), 0x0, 0x0, 0x0, 0xd2) != 0))
        goto L_4b4c;
    else
        goto L_4b3d;

L_4b3d:
    tutor.idtBold = 472;
    return 0x0;

L_4b4c:
    if ((FCheckFleetWP(0x6, 0x1, grobjPlanet, 0x11, 0xffff, 0xffff) != 0))
        goto L_4b83;
    else
        goto L_4b74;

L_4b74:
    tutor.idtBold = 472;
    return 0x0;

L_4b83:
    if ((FCheckFleetWP(0x6, 0x1, grobjPlanet, 0x11, 0x1, 0xffff) != 0))
        goto L_4bba;
    else
        goto L_4bab;

L_4bab:
    tutor.idtBold = 473;
    return 0x0;

L_4bba:
    if ((FCheckXferWP(0x6, 0x1, 0x11, 0xffff, rgiaUnloadAllCol) != 0))
        goto L_4bf6;
    else
        goto L_4be1;

L_4be1:
    tutor.idtBold = 474;
    tutor.idh = 1520;
    return 0x0;

L_4bf6:
    t_call_4bfa = LpflFromId(0x2);
    if ((FAROFF(t_call_4bfa) != 0x0))
        goto L_4c12;
    else
        goto L_4c0a;

L_4c0a:
    if ((FARSEG(t_call_4bfa) == 0x0))
        goto L_4ca7;
    else
        goto L_4c12;

L_4c12:
    if ((FCheckMessages(0x3, 0xffff, 0x0) != 0))
        goto L_4c37;
    else
        goto L_4c2e;

L_4c2e:
    tutor.idtBold = 475;
    goto L_4ca1;

L_4c37:
    if ((FCheckSelection(grobjFleet, 0x3) != 0))
        goto L_4c7c;
    else
        goto L_4c4f;

L_4c4f:
    if ((FCheckSelection(grobjFleet, 0x2) == 0))
        goto L_4c6d;
    else
        goto L_4c67;

L_4c67:
    t_merge_4c70_0001 = 0x1dd;
    goto L_4c70;

L_4c6d:
    t_merge_4c70_0001 = 0x1db;

L_4c70:
    tutor.idtBold = t_merge_4c70_0001;
    tutor.idh = 1517;
    goto L_4ca1;

L_4c7c:
    if ((vrgiflMerge != 0x0))
        goto L_4c95;
    else
        goto L_4c86;

L_4c86:
    tutor.idtBold = 478;
    tutor.idh = 1516;
    goto L_4ca1;

L_4c95:
    tutor.idh = 1107;
    tutor.idtBold = 479;

L_4ca1:
    return 0x0;

L_4ca7:
    tutor.fProgress = 0x0;
    return 0x1;

L_4cb9:
    if ((FCheckFleetWP(0x7, 0x0, grobjPlanet, 0xd, 0x6, 0xffff) != 0))
        goto L_4ced;
    else
        goto L_4ce1;

L_4ce1:
    tutor.idtBold = 480;
    return 0x0;

L_4ced:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_4d38;
    else
        goto L_4d10;

L_4d10:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x6, 0x1, 0x0) != 0))
        goto L_4d50;
    else
        goto L_4d38;

L_4d38:
    tutor.idtBold = 481;
    tutor.fProgress = 0x0;
    return 0x0;

L_4d50:
    if ((FCheckMessages(0xe, 0xffff, 0x0) == 0))
        goto L_4dc2;
    else
        goto L_4d6c;

L_4d6c:
    if ((FCheckSelection(grobjFleet, 0x4) != 0))
        goto L_4d9c;
    else
        goto L_4d84;

L_4d84:
    if ((FCheckSelection(grobjFleet, 0x8) == 0))
        goto L_4dc2;
    else
        goto L_4d9c;

L_4d9c:
    if ((tutor.fAutoComplete != 0x0))
        goto L_4dee;
    else
        goto L_4daf;

L_4daf:
    if ((tutor.fProgress != 0x0))
        goto L_4dee;
    else
        goto L_4dc2;

L_4dc2:
    if ((LOWORD(vrgtok) != 0x0))
        goto L_4dd6;
    else
        goto L_4dcc;

L_4dcc:
    if ((HIWORD(vrgtok) == 0x0))
        goto L_4de2;
    else
        goto L_4dd6;

L_4dd6:
    tutor.fProgress = 0x1;

L_4de2:
    tutor.idtBold = 482;
    return 0x0;

L_4dee:
    if ((LpflFromId(0x4)->lpplord->rgord[0].grobj == 0x2))
        goto L_4e28;
    else
        goto L_4e16;

L_4e16:
    tutor.idtBold = 484;
    tutor.idh = 1518;
    return 0x0;

L_4e28:
    if ((LpflFromId(0x8)->cord >= 3))
        goto L_4f00;
    else
        goto L_4e42;

L_4e42:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) == 0))
        goto L_4e76;
    else
        goto L_4e5e;

L_4e5e:
    if ((FCheckSelection(grobjFleet, 0x8) != 0))
        goto L_4e7f;
    else
        goto L_4e76;

L_4e76:
    tutor.idtBold = 485;
    goto L_4efa;

L_4e7f:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x8, 0x1, grobjPlanet, 0x1, 0xffff, 0xffff) == 0))
        goto L_4ebc;
    else
        goto L_4eb3;

L_4eb3:
    tutor.idtBold = 487;
    goto L_4eee;

L_4ebc:
    tutor.fNoErrors = 0x0;
    FCheckFleetWP(0x8, 0x1, grobjPlanet, 0x6, 0xffff, 0xffff);
    tutor.idtBold = 486;

L_4eee:
    tutor.fNoErrors = 0x0;

L_4efa:
    return 0x0;

L_4f00:
    if ((FCheckFleetWP(0x8, 0x2, grobjPlanet, 0x0, 0xffff, 0xffff) != 0))
        goto L_4f34;
    else
        goto L_4f28;

L_4f28:
    tutor.idtBold = 487;
    return 0x0;

L_4f34:
    tutor.idtBold = 487;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_4f4f:
    if ((tutor.idt == 464))
        goto L_49dc;
    else
        goto L_4f57;

L_4f57:
    if ((tutor.idt == 472))
        goto L_4b0f;
    else
        goto L_4f5f;

L_4f5f:
    if ((tutor.idt == 480))
        goto L_4cb9;
    else
        goto L_4f67;

L_4f67:
    goto L_49d6;
    goto L_5065;

L_4f70:

L_4f76:
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0x5, 0xffff, 0xffff) != 0))
        goto L_4faa;
    else
        goto L_4f9e;

L_4f9e:
    tutor.idtBold = 488;
    return 0x0;

L_4faa:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x2, 0x1, grobjFleet, 0x5, 0x4, 0xffff) != 0))
        goto L_5016;
    else
        goto L_4fde;

L_4fde:
    tutor.fNoErrors = 0x0;
    FCheckFleetWP(0x2, 0x1, grobjPlanet, 0xc, 0xffff, 0xffff);
    tutor.idtBold = 489;
    return 0x0;

L_5016:
    tutor.fNoErrors = 0x0;
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_504a;
    else
        goto L_503e;

L_503e:
    tutor.idtBold = 490;
    return 0x0;

L_504a:
    tutor.idtBold = 492;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_5065:
    if ((tutor.idt == 488))
        goto L_4f76;
    else
        goto L_506d;

L_506d:
    goto L_4f70;
    goto L_5358;

L_5076:

L_507c:
    if ((FCheckMessages(0x3, 0xffff, 0x0) != 0))
        goto L_50a4;
    else
        goto L_5098;

L_5098:
    tutor.idtBold = 496;
    return 0x0;

L_50a4:
    if ((rgshdef[2].hul.rghs[0].iItem != 0x4))
        goto L_50bc;
    else
        goto L_50b2;

L_50b2:
    if ((hwndSlotDlg == 0x0))
        goto L_51c8;
    else
        goto L_50bc;

L_50bc:
    hs.grhst = hstEngine;
    HIWORD(hs) = 0x104;
    hs2.grhst = hstSpecialM;
    HIWORD(hs2) = 0x100;
    if ((FCheckScanner(0x3, 0xffff) != 0))
        goto L_50f4;
    else
        goto L_50e8;

L_50e8:
    tutor.idtBold = 497;
    return 0x0;

L_50f4:
    if ((hwndSlotDlg != 0x0))
        goto L_5110;
    else
        goto L_50fe;

L_50fe:
    tutor.idtBold = 499;
    tutor.idh = 1001;
    return 0x0;

L_5110:
    if ((rgshdef[2].hul.rghs[0].iItem != 0x4))
        goto L_512a;
    else
        goto L_511e;

L_511e:
    tutor.idtBold = 503;
    return 0x0;

L_512a:
    if ((FCheckShipBuilder(0x4, 0xffff) != 0))
        goto L_516c;
    else
        goto L_5142;

L_5142:
    if ((FCheckShipBuilder(0x0, 0x2) != 0))
        goto L_5163;
    else
        goto L_515a;

L_515a:
    tutor.idtBold = 500;
    goto L_51c2;

L_5163:
    tutor.idtBold = 501;

L_516c:
    if ((lpshdefBuild->hul.rghs[0].cItem == 0x0))
        goto L_51b3;
    else
        goto L_5184;

L_5184:
    if ((lpshdefBuild->hul.rghs[0].iItem != 0x4))
        goto L_51b3;
    else
        goto L_5197;

L_5197:
    if ((FCheckBuilderPart(0x1, &(hs2), 0x1) != 0))
        goto L_51bc;
    else
        goto L_51b3;

L_51b3:
    tutor.idtBold = 502;
    goto L_51c2;

L_51bc:
    tutor.idtBold = 503;

L_51c2:
    return 0x0;

L_51c8:
    return 0x1;

L_51ce:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_5219;
    else
        goto L_51f1;

L_51f1:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x2, 0x3, 0x0) != 0))
        goto L_5225;
    else
        goto L_5219;

L_5219:
    tutor.idtBold = 504;
    return 0x0;

L_5225:
    if ((FCheckMessages(0x4, 0xffff, 0x0) != 0))
        goto L_524d;
    else
        goto L_5241;

L_5241:
    tutor.idtBold = 505;
    return 0x0;

L_524d:
    if ((LpplFromId(0x11)->lpplprod->iprodMac != 0x3))
        goto L_5298;
    else
        goto L_5270;

L_5270:
    if ((FCheckQueue(0x11, 0x2, grobjPlanet, 0x5, 0x2, 0x1) != 0))
        goto L_52c5;
    else
        goto L_5298;

L_5298:
    if ((FCheckSelection(grobjPlanet, 0x11) != 0))
        goto L_52b9;
    else
        goto L_52b0;

L_52b0:
    tutor.idtBold = 505;
    goto L_52bf;

L_52b9:
    tutor.idtBold = 507;

L_52bf:
    return 0x0;

L_52c5:
    if ((FCheckMessages(0x6, 0xffff, 0x0) == 0))
        goto L_52fd;
    else
        goto L_52e1;

L_52e1:
    if ((FCheckResearch(0x2, 0x3, 0x1e) != 0))
        goto L_5309;
    else
        goto L_52fd;

L_52fd:
    tutor.idtBold = 508;
    return 0x0;

L_5309:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_5331;
    else
        goto L_5325;

L_5325:
    tutor.idtBold = 509;
    return 0x0;

L_5331:
    tutor.idtBold = 511;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_5358:
    if ((tutor.idt == 496))
        goto L_507c;
    else
        goto L_5360;

L_5360:
    if ((tutor.idt == 504))
        goto L_51ce;
    else
        goto L_5368;

L_5368:
    goto L_5076;
    goto L_5612;

L_5371:

L_5377:
    if ((FCheckMessages(0x4, 0xffff, 0x0) != 0))
        goto L_539f;
    else
        goto L_5393;

L_5393:
    tutor.idtBold = 512;
    return 0x0;

L_539f:
    if ((rgplr[0].cFleet != 0xa))
        goto L_53f3;
    else
        goto L_53ad;

L_53ad:
    if ((FCheckColonizeWP(0x9, 0x0, 0xffff) != 0))
        goto L_53f3;
    else
        goto L_53c9;

L_53c9:
    if ((FCheckSelection(grobjFleet, 0x9) == 0))
        goto L_53e7;
    else
        goto L_53e1;

L_53e1:
    t_merge_53ea_0001 = 0x202;
    goto L_53ea;

L_53e7:
    t_merge_53ea_0001 = 0x201;

L_53ea:
    tutor.idtBold = t_merge_53ea_0001;
    return 0x0;

L_53f3:
    if ((rgplr[0].cFleet != 0xa))
        goto L_540d;
    else
        goto L_5401;

L_5401:
    tutor.idtBold = 515;
    return 0x0;

L_540d:
    tutor.fNoErrors = 0x1;
    if ((FCheckColonizeWP(0x9, 0x1, 0xffff) != 0))
        goto L_5461;
    else
        goto L_5435;

L_5435:
    tutor.fNoErrors = 0x0;
    FCheckColonizeWP(0x9, 0x0, 0xffff);
    tutor.idtBold = 516;
    return 0x0;

L_5461:
    if ((FCheckColonizeWP(0xa, 0x17, 0xffff) != 0))
        goto L_54a9;
    else
        goto L_547d;

L_547d:
    tutor.fNoErrors = 0x0;
    FCheckColonizeWP(0xa, 0x0, 0xffff);
    tutor.idtBold = 517;
    return 0x0;

L_54a9:
    tutor.fNoErrors = 0x0;
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_54dd;
    else
        goto L_54d1;

L_54d1:
    tutor.idtBold = 518;
    return 0x0;

L_54dd:
    return 0x1;

L_54e3:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x4))
        goto L_552e;
    else
        goto L_5506;

L_5506:
    if ((FCheckQueue(0xd, 0x1, grobjFleet, 0x3, 0x3, 0x0) != 0))
        goto L_553a;
    else
        goto L_552e;

L_552e:
    tutor.idtBold = 521;
    return 0x0;

L_553a:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x4, 0x1, grobjFleet, 0x205, 0xffff, 0xffff) != 0))
        goto L_55eb;
    else
        goto L_556e;

L_556e:
    tutor.fNoErrors = 0x0;
    FCheckColonizeWP(0x4, 0x5, 0xffff);
    if ((FCheckSummary(grobjFleet, 0x205) != 0))
        goto L_55d6;
    else
        goto L_55a6;

L_55a6:
    if ((FCheckSummary(grobjFleet, 0x4) != 0))
        goto L_55d6;
    else
        goto L_55be;

L_55be:
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_55df;
    else
        goto L_55d6;

L_55d6:
    tutor.idtBold = 525;
    goto L_55e5;

L_55df:
    tutor.idtBold = 523;

L_55e5:
    return 0x0;

L_55eb:
    tutor.fNoErrors = 0x0;
    tutor.idtBold = 527;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_5612:
    if ((tutor.idt == 512))
        goto L_5377;
    else
        goto L_561a;

L_561a:
    if ((tutor.idt == 520))
        goto L_54e3;
    else
        goto L_5622;

L_5622:
    goto L_5371;
    goto L_5a04;

L_562b:

L_5631:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x4, 0x0, grobjFleet, 0x205, 0xffff, 0xffff) != 0))
        goto L_56a4;
    else
        goto L_5665;

L_5665:
    tutor.fNoErrors = 0x0;
    if ((FCheckSelection(grobjFleet, 0x4) == 0))
        goto L_5698;
    else
        goto L_5689;

L_5689:
    tutor.idh = 1518;
    tutor.idtBold = 530;
    goto L_569e;

L_5698:
    tutor.idtBold = 528;

L_569e:
    return 0x0;

L_56a4:
    tutor.fNoErrors = 0x0;
    if ((FCheckMessages(0x1, 0xffff, 0x0) != 0))
        goto L_56db;
    else
        goto L_56cc;

L_56cc:
    tutor.idtBold = 532;
    return 0x0;

L_56db:
    t_call_56df = LpflFromId(0xc);
    if ((FAROFF(t_call_56df) != 0x0))
        goto L_56f7;
    else
        goto L_56ef;

L_56ef:
    if ((FARSEG(t_call_56df) == 0x0))
        goto L_571b;
    else
        goto L_56f7;

L_56f7:
    if ((((*(LpflFromId(0xc) + 0x4) >> 0xa) & 0x1) == 0x0))
        goto L_5724;
    else
        goto L_571b;

L_571b:
    return 0x1;

L_5724:
    if ((FCheckCargo(LpflFromId(0xc), 0x0, 0x0, 0x0, 0x276) != 0))
        goto L_579d;
    else
        goto L_5752;

L_5752:
    if ((FCheckSelection(grobjFleet, 0xc) == 0))
        goto L_5773;
    else
        goto L_576a;

L_576a:
    tutor.idtBold = 535;
    goto L_5797;

L_5773:
    if ((FCheckSelection(grobjFleet, 0x8) == 0))
        goto L_5791;
    else
        goto L_578b;

L_578b:
    t_merge_5794_0001 = 0x216;
    goto L_5794;

L_5791:
    t_merge_5794_0001 = 0x214;

L_5794:
    tutor.idtBold = t_merge_5794_0001;

L_5797:
    return 0x0;

L_579d:
    return 0x1;

L_57a3:
    t_call_57a7 = LpflFromId(0xc);
    if ((FAROFF(t_call_57a7) != 0x0))
        goto L_57bf;
    else
        goto L_57b7;

L_57b7:
    if ((FARSEG(t_call_57a7) == 0x0))
        goto L_5805;
    else
        goto L_57bf;

L_57bf:
    if ((FCheckSelection(grobjFleet, 0xb) != 0))
        goto L_57e0;
    else
        goto L_57d7;

L_57d7:
    tutor.idtBold = 537;
    goto L_57ff;

L_57e0:
    if ((vrgiflMerge != 0x0))
        goto L_57f9;
    else
        goto L_57ea;

L_57ea:
    tutor.idtBold = 537;
    tutor.idh = 1516;
    goto L_57ff;

L_57f9:
    tutor.idtBold = 537;

L_57ff:
    return 0x0;

L_5805:
    if ((FCheckMessages(0x8, 0xffff, 0x0) != 0))
        goto L_582d;
    else
        goto L_5821;

L_5821:
    tutor.idtBold = 538;
    return 0x0;

L_582d:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_5855;
    else
        goto L_5849;

L_5849:
    tutor.idtBold = 540;
    return 0x0;

L_5855:
    tutor.idtBold = 542;
    tutor.idh = 6001;
    if ((hwndSlotDlg != 0x0))
        goto L_5877;
    else
        goto L_586b;

L_586b:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x9 ? L_587d : L_5877 */

L_5877:
    return 0x1;

L_587d:
    return 0x0;

L_5883:
    if ((hwndSlotDlg != 0x0))
        goto L_5899;
    else
        goto L_588d;

L_588d:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) >= 0x9 ? L_5992 : L_5899 */

L_5899:
    if ((FCheckShipBuilder(0x4, 0xffff) != 0))
        goto L_58cc;
    else
        goto L_58b1;

L_58b1:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) >= 0x9 ? L_58c3 : L_58bd */

L_58bd:
    t_merge_58c6_0001 = 0x221;
    goto L_58c6;

L_58c3:
    t_merge_58c6_0001 = 0x225;

L_58c6:
    tutor.idtBold = t_merge_58c6_0001;
    goto L_598c;

L_58cc:
    if ((lpshdefBuild->hul.rghs[0].cItem != 0x1))
        goto L_5944;
    else
        goto L_58e4;

L_58e4:
    if ((lpshdefBuild->hul.rghs[1].cItem != 0x1))
        goto L_5944;
    else
        goto L_58fc;

L_58fc:
    if ((lpshdefBuild->hul.rghs[2].cItem != 0x1))
        goto L_5944;
    else
        goto L_5914;

L_5914:
    if ((lpshdefBuild->hul.rghs[3].cItem != 0x1))
        goto L_5944;
    else
        goto L_592c;

L_592c:
    if ((lpshdefBuild->hul.rghs[4].cItem == 0x2))
        goto L_594d;
    else
        goto L_5944;

L_5944:
    tutor.idtBold = 546;
    goto L_598c;

L_594d:
    if ((lpshdefBuild->hul.rghs[5].cItem != 0x1))
        goto L_597d;
    else
        goto L_5965;

L_5965:
    if ((lpshdefBuild->hul.rghs[6].cItem == 0x1))
        goto L_5986;
    else
        goto L_597d;

L_597d:
    tutor.idtBold = 547;
    goto L_598c;

L_5986:
    tutor.idtBold = 549;

L_598c:
    return 0x0;

L_5992:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_59dd;
    else
        goto L_59b5;

L_59b5:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x8, 0xa, 0x0) != 0))
        goto L_59e9;
    else
        goto L_59dd;

L_59dd:
    tutor.idtBold = 550;
    return 0x0;

L_59e9:
    tutor.idtBold = 551;
    tutor.fTurnDone = 0x1;
    return 0x1;

L_5a04:
    if ((tutor.idt == 528))
        goto L_5631;
    else
        goto L_5a0c;

L_5a0c:
    if ((tutor.idt == 536))
        goto L_57a3;
    else
        goto L_5a14;

L_5a14:
    if ((tutor.idt == 544))
        goto L_5883;
    else
        goto L_5a1c;

L_5a1c:
    goto L_562b;
    goto L_5b4c;

L_5a25:

L_5a2b:
    if ((FCheckFleetWP(0x4, 0x1, grobjPlanet, 0x5, 0xffff, 0xffff) != 0))
        goto L_5a80;
    else
        goto L_5a53;

L_5a53:
    if ((FCheckSelection(grobjFleet, 0x4) != 0))
        goto L_5a74;
    else
        goto L_5a6b;

L_5a6b:
    tutor.idtBold = 552;
    goto L_5a7a;

L_5a74:
    tutor.idtBold = 554;

L_5a7a:
    return 0x0;

L_5a80:
    if ((FCheckMessages(0x6, 0xffff, 0x0) != 0))
        goto L_5aa8;
    else
        goto L_5a9c;

L_5a9c:
    tutor.idtBold = 555;
    return 0x0;

L_5aa8:
    if ((FCheckFleetWP(0xc, 0x1, grobjPlanet, 0xa, 0xffff, 0xffff) != 0))
        goto L_5afd;
    else
        goto L_5ad0;

L_5ad0:
    if ((FCheckSelection(grobjFleet, 0xc) != 0))
        goto L_5af1;
    else
        goto L_5ae8;

L_5ae8:
    tutor.idtBold = 555;
    goto L_5af7;

L_5af1:
    tutor.idtBold = 556;

L_5af7:
    return 0x0;

L_5afd:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_5b25;
    else
        goto L_5b19;

L_5b19:
    tutor.idtBold = 557;
    return 0x0;

L_5b25:
    tutor.idtBold = 559;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_5b4c:
    if ((tutor.idt == 552))
        goto L_5a2b;
    else
        goto L_5b54;

L_5b54:
    goto L_5a25;
    goto L_5ed5;

L_5b5d:

L_5b63:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x3, 0x1, grobjPlanet, 0xd, 0xffff, 0x5) != 0))
        goto L_5bd0;
    else
        goto L_5b97;

L_5b97:
    tutor.fNoErrors = 0x0;
    if ((FCheckSelection(grobjFleet, 0x3) != 0))
        goto L_5bc4;
    else
        goto L_5bbb;

L_5bbb:
    tutor.idtBold = 560;
    goto L_5bca;

L_5bc4:
    tutor.idtBold = 562;

L_5bca:
    return 0x0;

L_5bd0:
    tutor.fNoErrors = 0x0;
    if ((FCheckMessages(0x6, 0xffff, 0x0) != 0))
        goto L_5c04;
    else
        goto L_5bf8;

L_5bf8:
    tutor.idtBold = 564;
    return 0x0;

L_5c04:
    if ((FCheckFleetWP(0xd, 0x1, grobjPlanet, 0xa, 0xffff, 0xffff) != 0))
        goto L_5c38;
    else
        goto L_5c2c;

L_5c2c:
    tutor.idtBold = 565;
    return 0x0;

L_5c38:
    if ((FCheckPlanetRoute(0xd, 0xa) != 0))
        goto L_5c5c;
    else
        goto L_5c50;

L_5c50:
    tutor.idtBold = 567;
    return 0x0;

L_5c5c:
    return 0x1;

L_5c62:
    if ((FCheckMessages(0x11, 0xffff, 0x0) != 0))
        goto L_5c8a;
    else
        goto L_5c7e;

L_5c7e:
    tutor.idtBold = 569;
    return 0x0;

L_5c8a:
    if ((FCheckResearch(0x3, 0x0, 0x1e) != 0))
        goto L_5cb2;
    else
        goto L_5ca6;

L_5ca6:
    tutor.idtBold = 570;
    return 0x0;

L_5cb2:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_5cda;
    else
        goto L_5cce;

L_5cce:
    tutor.idtBold = 571;
    return 0x0;

L_5cda:
    tutor.fNoErrors = 0x1;
    if ((FCheckFleetWP(0x6, 0x0, grobjPlanet, 0x11, 0x5, 0xffff) != 0))
        goto L_5d47;
    else
        goto L_5d0e;

L_5d0e:
    tutor.fNoErrors = 0x0;
    if ((FCheckSelection(grobjFleet, 0x6) != 0))
        goto L_5d3b;
    else
        goto L_5d32;

L_5d32:
    tutor.idtBold = 572;
    goto L_5d41;

L_5d3b:
    tutor.idtBold = 574;

L_5d41:
    return 0x0;

L_5d47:
    tutor.fNoErrors = 0x0;
    return 0x1;

L_5d59:
    if ((hwndSlotDlg == 0x0))
        goto L_5e39;
    else
        goto L_5d63;

L_5d63:
    if ((((rgshdef[9].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5d8e;
    else
        goto L_5d76;

L_5d76:
    if ((FCheckShipBuilder(0x4, 0xffff) == 0))
        goto L_5e2a;
    else
        goto L_5d8e;

L_5d8e:
    if ((FCheckShipBuilder(0x4, 0xffff) == 0))
        goto L_5e21;
    else
        goto L_5da6;

L_5da6:
    if ((lpshdefBuild->hul.rghs[0].cItem == 0x2))
        goto L_5dc7;
    else
        goto L_5dbe;

L_5dbe:
    tutor.idtBold = 579;
    goto L_5e30;

L_5dc7:
    if ((lpshdefBuild->hul.rghs[1].cItem != 0x4))
        goto L_5e0f;
    else
        goto L_5ddf;

L_5ddf:
    if ((lpshdefBuild->hul.rghs[2].cItem != 0x4))
        goto L_5e0f;
    else
        goto L_5df7;

L_5df7:
    if ((lpshdefBuild->hul.rghs[3].cItem == 0x1))
        goto L_5e18;
    else
        goto L_5e0f;

L_5e0f:
    tutor.idtBold = 580;
    goto L_5e30;

L_5e18:
    tutor.idtBold = 581;

L_5e21:
    tutor.idtBold = 578;

L_5e2a:
    tutor.idtBold = 581;

L_5e30:
    return 0x0;

L_5e39:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) >= 0xa ? L_5e57 : L_5e45 */

L_5e45:
    tutor.idtBold = 577;
    tutor.idh = 6001;
    return 0x0;

L_5e57:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_5ea2;
    else
        goto L_5e7a;

L_5e7a:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x9, 0xa, 0x0) != 0))
        goto L_5eae;
    else
        goto L_5ea2;

L_5ea2:
    tutor.idtBold = 582;
    return 0x0;

L_5eae:
    tutor.idtBold = 583;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_5ed5:
    if ((tutor.idt == 560))
        goto L_5b63;
    else
        goto L_5edd;

L_5edd:
    if ((tutor.idt == 568))
        goto L_5c62;
    else
        goto L_5ee5;

L_5ee5:
    if ((tutor.idt == 576))
        goto L_5d59;
    else
        goto L_5eed;

L_5eed:
    goto L_5b5d;
    goto L_5f94;

L_5ef6:

L_5efc:
    if ((FCheckMessages(0x5, 0xffff, 0x0) != 0))
        goto L_5f24;
    else
        goto L_5f18;

L_5f18:
    tutor.idtBold = 586;
    return 0x0;

L_5f24:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_5f6d;
    else
        goto L_5f40;

L_5f40:
    if ((FCheckSelection(grobjFleet, 0xe) != 0))
        goto L_5f61;
    else
        goto L_5f58;

L_5f58:
    tutor.idtBold = 586;
    goto L_5f67;

L_5f61:
    tutor.idtBold = 588;

L_5f67:
    return 0x0;

L_5f6d:
    tutor.idtBold = 590;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_5f94:
    if ((tutor.idt == 584))
        goto L_5efc;
    else
        goto L_5f9c;

L_5f9c:
    goto L_5ef6;
    goto L_609d;

L_5fa5:

L_5fab:
    if ((FCheckMessages(0x6, 0xffff, 0x0) != 0))
        goto L_5fd3;
    else
        goto L_5fc7;

L_5fc7:
    tutor.idtBold = 592;
    return 0x0;

L_5fd3:
    if ((FCheckSelection(grobjPlanet, 0x5) != 0))
        goto L_5ff7;
    else
        goto L_5feb;

L_5feb:
    tutor.idtBold = 592;
    return 0x0;

L_5ff7:
    if ((LpplFromId(0x5)->lpplprod->iprodMac < 0x4))
        goto L_6042;
    else
        goto L_601a;

L_601a:
    if ((FCheckQueue(0x5, 0x0, grobjPlanet, 0x8, 0x64, 0x1) != 0))
        goto L_604e;
    else
        goto L_6042;

L_6042:
    tutor.idtBold = 594;
    return 0x0;

L_604e:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_6076;
    else
        goto L_606a;

L_606a:
    tutor.idtBold = 596;
    return 0x0;

L_6076:
    tutor.idtBold = 598;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_609d:
    if ((tutor.idt == 592))
        goto L_5fab;
    else
        goto L_60a5;

L_60a5:
    goto L_5fa5;
    goto L_61a0;

L_60ae:

L_60b4:
    if ((FCheckMessages(0x2, 0xffff, 0x0) != 0))
        goto L_60dc;
    else
        goto L_60d0;

L_60d0:
    tutor.idtBold = 600;
    return 0x0;

L_60dc:
    if ((tutor.fProgress != 0x0))
        goto L_610e;
    else
        goto L_60ef;

L_60ef:
    if ((tutor.fAutoComplete != 0x0))
        goto L_610e;
    else
        goto L_6102;

L_6102:
    tutor.idtBold = 602;
    return 0x0;

L_610e:
    if ((FCheckMessages(0x12, 0xffff, 0x0) != 0))
        goto L_6157;
    else
        goto L_612a;

L_612a:
    if ((FCheckSelection(grobjFleet, 0xc) != 0))
        goto L_614b;
    else
        goto L_6142;

L_6142:
    tutor.idtBold = 600;
    goto L_6151;

L_614b:
    tutor.idtBold = 604;

L_6151:
    return 0x0;

L_6157:
    tutor.idtBold = 606;
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_617f;
    else
        goto L_6179;

L_6179:
    return 0x0;

L_617f:
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_61a0:
    if ((tutor.idt == 600))
        goto L_60b4;
    else
        goto L_61a8;

L_61a8:
    goto L_60ae;
    goto L_62a6;

L_61b1:

L_61b7:
    if ((FCheckMessages(0xa, 0xffff, 0x0) != 0))
        goto L_61df;
    else
        goto L_61d3;

L_61d3:
    tutor.idtBold = 608;
    return 0x0;

L_61df:
    if ((LpplFromId(0xd)->lpplprod->iprodMac < 0x3))
        goto L_622a;
    else
        goto L_6202;

L_6202:
    if ((FCheckQueue(0xd, 0x0, grobjFleet, 0x9, 0xa, 0x0) != 0))
        goto L_6257;
    else
        goto L_622a;

L_622a:
    if ((FCheckSelection(grobjPlanet, 0xd) != 0))
        goto L_624b;
    else
        goto L_6242;

L_6242:
    tutor.idtBold = 608;
    goto L_6251;

L_624b:
    tutor.idtBold = 609;

L_6251:
    return 0x0;

L_6257:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_627f;
    else
        goto L_6273;

L_6273:
    tutor.idtBold = 611;
    return 0x0;

L_627f:
    tutor.idtBold = 612;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_62a6:
    if ((tutor.idt == 608))
        goto L_61b7;
    else
        goto L_62ae;

L_62ae:
    goto L_61b1;
    goto L_6306;

L_62b7:

L_62bd:
    tutor.idtBold = 619;
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_62e5;
    else
        goto L_62df;

L_62df:
    return 0x0;

L_62e5:
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_6306:
    if ((tutor.idt == 616))
        goto L_62bd;
    else
        goto L_630e;

L_630e:
    goto L_62b7;
    goto L_636c;

L_6317:

L_631d:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_6345;
    else
        goto L_6339;

L_6339:
    tutor.idtBold = 624;
    return 0x0;

L_6345:
    tutor.idtBold = 628;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_636c:
    if ((tutor.idt == 624))
        goto L_631d;
    else
        goto L_6374;

L_6374:
    goto L_6317;
    goto L_63d2;

L_637d:
    return 0x1;

L_6383:
    if ((FCheckMessages(0x270f, 0xffff, 0x0) != 0))
        goto L_63ab;
    else
        goto L_639f;

L_639f:
    tutor.idtBold = 636;
    return 0x0;

L_63ab:
    tutor.idtBold = 637;
    tutor.fTurnDone = 0x1;
    tutor.fProgress = 0x0;
    return 0x1;

L_63d2:
    if ((tutor.idt != 632))
        goto L_637d;
    else
        goto L_63d7;

L_63d7:

L_63fe:
}

int16_t FCheckZip(int16_t iZip, ITEMACTION *lpiaGoal, StringId ids) {
    ITEMACTION *piaCur;
    int16_t     i;
    char        szT[33];
    int16_t     idhSav;

L_6460:
    idhSav = tutor.idh;
    tutor.idh = 1098;
    if ((tutor.fAutoComplete == 0x0))
        goto L_6533;
    else
        goto L_6488;

L_6488:
    vrgZip[iZip].fValid = 0x1;
    piaCur = vrgZip[iZip];
    i = 0;
    goto L_64c7;

L_64ad:
    i = (i + 1);
    piaCur = (piaCur + 0x2);
    lpiaGoal = (lpiaGoal + 0x1);

L_64c7:
    if ((i >= 5))
        goto L_6501;
    else
        goto L_64d0;

L_64d0:
    *(piaCur) = (piaCur->cQuan | ((lpiaGoal->iAction & 0xf) << 0xc));
    goto L_64ad;

L_6501:
    CchGetString(ids, szT);
    strcpy(vrgZip[iZip].szName, szT);
    return 0x1;

L_6533:
    if ((vrgZip[iZip].fValid != 0x0))
        goto L_6562;
    else
        goto L_654c;

L_654c:
    if ((hwndZipOrderDlg != 0x0))
        goto L_655c;
    else
        goto L_6556;

L_6556:
    tutor.idh = 1520;

L_655c:
    return 0x0;

L_6562:
    piaCur = vrgZip[iZip];
    tutor.idh = 1519;
    i = 0;
    goto L_6598;

L_657e:
    i = (i + 1);
    piaCur = (piaCur + 0x2);
    lpiaGoal = (lpiaGoal + 0x1);

L_6598:
    if ((i >= 5))
        goto L_65e1;
    else
        goto L_65a1;

L_65a1:
    if ((piaCur->iAction == lpiaGoal->iAction))
        goto L_657e;
    else
        goto L_65c3;

L_65c3:
    if ((piaCur->iAction == iActionNone))
        goto L_657e;
    else
        goto L_65d8;

L_65d8:
    return 0x0;

L_65e1:
    CchGetString(ids, szT);
    if ((strcmpi(szT, vrgZip[iZip].szName) != 0))
        goto L_6621;
    else
        goto L_6615;

L_6615:
    tutor.idh = idhSav;
    return 0x1;

L_6621:
    TutorError(0x208);
    return 0x0;
}

int16_t FCheckTemplate(int16_t iTemplate) {
    int16_t i;

L_666e:
    tutor.idh = 3117;
    if ((tutor.fAutoComplete == 0x0))
        goto L_66c7;
    else
        goto L_6690;

L_6690:
    vrgZipProd[0].fValid = 0x1;
    vrgZipProd[0].zpq1 = rgzpqTut[iTemplate];
    gd.fChgZipProd = 0x1;
    return 0x1;

L_66c7:
    /* untranslated: branch (part[13:2](vrgZipProd[0x0]) & 0xff) != 0x0 ? L_66db : L_66d5 */

L_66d5:
    return 0x0;

L_66db:
    /* untranslated: branch (part[14:2](vrgZipProd[0x0]) & 0xff) == rgzpqTut[iTemplate].fNoResearch ? L_6704 : L_66fe */

L_66fe:
    return 0x0;

L_6704:
    /* untranslated: branch (part[15:2](vrgZipProd[0x0]) & 0xff) == rgzpqTut[iTemplate].cpq ? L_6733 : L_672d */

L_672d:
    return 0x0;

L_6733:
    i = 0;
    goto L_673f;

L_673b:
    i = (i + 1);

L_673f:
    if ((i >= rgzpqTut[iTemplate].cpq))
        goto L_6795;
    else
        goto L_675d;

L_675d:
    if ((mpicolgrbitBU[(i + 4483)] == rgzpqTut[iTemplate].rgpq[i].w))
        goto L_673b;
    else
        goto L_678c;

L_678c:
    return 0x0;

L_6795:
    gd.fChgZipProd = 0x1;
    return 0x1;
}

void TutorError(int16_t idsError) {
L_67ae:
    if ((tutor.fNoErrors == 0x0))
        goto L_67d8;
    else
        goto L_67cf;

L_67cf:
    tutor.idsError = -1;
    goto L_6856;

L_67d8:
    if ((tutor.idsError != idsError))
        goto L_6824;
    else
        goto L_67e4;

L_67e4:
    /* untranslated: ss:[bp-0x4] = tutor.cError */
    /* untranslated: ss:[bp-0x6] = ((tutor.wFlags + 0x40) & 0x1c0) */
    tutor.cError = 0x0;
    /* untranslated: tutor.wFlags = (tutor.wFlags | ss:[bp-0x6]) */
    /* untranslated: branch ss:[bp-0x4] < 0x3 ? L_6856 : L_6824 */

L_6824:
    tutor.cError = 0x0;
    tutor.idsError = idsError;
    AlertSz(PszFormatIds(idsError, 0x0), MB_ICONHAND);

L_6856:
    return;
}

int16_t FCheckScanner(int16_t md, int16_t iZoom) {
    int16_t idhSav;

L_685c:
    idhSav = tutor.idh;
    if ((tutor.fAutoComplete == 0x0))
        goto L_6884;
    else
        goto L_687e;

L_687e:
    return 0x1;

L_6884:
    tutor.idh = 14008;
    if ((md == -1))
        goto L_68d8;
    else
        goto L_6893;

L_6893:
    if ((grbitScan == md))
        goto L_68d8;
    else
        goto L_689f;

L_689f:
    if ((md >= 6))
        goto L_68c1;
    else
        goto L_68a8;

L_68a8:
    if (((grbitScan & 0xf) == md))
        goto L_68d8;
    else
        goto L_68b8;

L_68b8:
    return 0x0;

L_68c1:
    if (((md & grbitScan) == md))
        goto L_68d8;
    else
        goto L_68d2;

L_68d2:
    return 0x0;

L_68d8:
    if ((iZoom == -1))
        goto L_68f8;
    else
        goto L_68e1;

L_68e1:
    if ((iZoom == iScanZoom))
        goto L_68f8;
    else
        goto L_68ec;

L_68ec:
    tutor.idh = 14022;
    return 0x0;

L_68f8:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckFleetName(int16_t id, StringId ids) {
    FLEET  *lpfl;
    char    szT[33];
    int16_t idhSav;

L_690a:
    idhSav = tutor.idh;
    tutor.idh = 3054;
    lpfl = LpflFromId(id);
    if ((LOWORD(lpfl) != 0x0))
        goto L_6948;
    else
        goto L_6939;

L_6939:
    if ((HIWORD(lpfl) != 0x0))
        goto L_6948;
    else
        goto L_6942;

L_6942:
    return 0x1;

L_6948:
    if ((LOWORD(lpfl->lpszName) != 0x0))
        goto L_698c;
    else
        goto L_6955;

L_6955:
    if ((HIWORD(lpfl->lpszName) != 0x0))
        goto L_698c;
    else
        goto L_695f;

L_695f:
    if ((ids != 0xffff))
        goto L_6977;
    else
        goto L_6968;

L_6968:
    tutor.idh = idhSav;
    return 0x1;

L_6977:
    if ((ids != 0xffff))
        goto L_6986;
    else
        goto L_6980;

L_6980:
    return 0x1;

L_6986:
    return 0x0;

L_698c:
    CchGetString(ids, szT);
    if ((fstricmp(szT, lpfl->lpszName) != 0))
        goto L_69c9;
    else
        goto L_69bd;

L_69bd:
    tutor.idh = idhSav;
    return 0x1;

L_69c9:
    TutorError(0x4cf);
    return 0x0;
}

int16_t FCheckSummary(GrobjClass grobj, int16_t id) {
    int16_t  fRet;
    uint16_t t_merge_6a4a_0001;
    uint16_t t_merge_6a6f_0001;
    uint16_t t_merge_6ab5_0001;

L_69e2:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_6a01;
    else
        goto L_69fb;

L_69fb:
    return 0x1;

L_6a01:
    fRet = 0;
    goto L_6abe;

L_6a0c:
    if ((sel.scan.grobj != grobjFleet))
        goto L_6a47;
    else
        goto L_6a16;

L_6a16:
    if ((sel.scan.ifl == -1))
        goto L_6a47;
    else
        goto L_6a20;

L_6a20:
    if ((rglpfl[sel.scan.ifl]->id != id))
        goto L_6a47;
    else
        goto L_6a41;

L_6a41:
    t_merge_6a4a_0001 = 0x1;
    goto L_6a4a;

L_6a47:
    t_merge_6a4a_0001 = 0x0;

L_6a4a:
    fRet = t_merge_6a4a_0001;
    goto L_6ad9;

L_6a50:
    if ((sel.scan.grobj != grobjPlanet))
        goto L_6a6c;
    else
        goto L_6a5a;

L_6a5a:
    if ((sel.scan.idpl != id))
        goto L_6a6c;
    else
        goto L_6a66;

L_6a66:
    t_merge_6a6f_0001 = 0x1;
    goto L_6a6f;

L_6a6c:
    t_merge_6a6f_0001 = 0x0;

L_6a6f:
    fRet = t_merge_6a6f_0001;
    goto L_6ad9;

L_6a75:
    if ((sel.scan.grobj != grobjThing))
        goto L_6ab2;
    else
        goto L_6a7f;

L_6a7f:
    if ((id == -1))
        goto L_6aac;
    else
        goto L_6a88;

L_6a88:
    if ((lpThings[sel.scan.ith].idFull != id))
        goto L_6ab2;
    else
        goto L_6aac;

L_6aac:
    t_merge_6ab5_0001 = 0x1;
    goto L_6ab5;

L_6ab2:
    t_merge_6ab5_0001 = 0x0;

L_6ab5:
    fRet = t_merge_6ab5_0001;
    goto L_6ad9;

L_6abe:
    if ((grobj == grobjPlanet))
        goto L_6a50;
    else
        goto L_6ac6;

L_6ac6:
    if ((grobj == grobjFleet))
        goto L_6a0c;
    else
        goto L_6ace;

L_6ace:
    if ((grobj == grobjThing))
        goto L_6a75;
    else
        goto L_6ad9;

L_6ad9:
    if ((fRet != 0))
        goto L_6ae8;
    else
        goto L_6ae2;

L_6ae2:
    tutor.idh = 14034;

L_6ae8:
    return fRet;
}

int16_t FCheckSelection(GrobjClass grobj, int16_t id) {
    int16_t  fRet;
    int16_t  idhSav;
    uint16_t t_merge_6b3e_0001;
    uint16_t t_merge_6beb_0001;
    uint16_t t_merge_6c10_0001;

L_6af4:
    idhSav = tutor.idh;
    if ((tutor.fAutoComplete != 0x0))
        goto L_6b26;
    else
        goto L_6b16;

L_6b16:
    if ((gd.fGeneratingTurn == 0x0))
        goto L_6b2c;
    else
        goto L_6b26;

L_6b26:
    return 0x1;

L_6b2c:
    if ((grobj != grobjFleet))
        goto L_6b3b;
    else
        goto L_6b35;

L_6b35:
    t_merge_6b3e_0001 = 0x2;
    goto L_6b3e;

L_6b3b:
    t_merge_6b3e_0001 = 0x1;

L_6b3e:
    if ((mdMsgObj != t_merge_6b3e_0001))
        goto L_6b5b;
    else
        goto L_6b47;

L_6b47:
    if ((id != idMsgObj))
        goto L_6b5b;
    else
        goto L_6b52;

L_6b52:
    tutor.idh = 14001;
    goto L_6bc1;

L_6b5b:
    if ((grobj != grobjPlanet))
        goto L_6b83;
    else
        goto L_6b64;

L_6b64:
    if ((sel.grobj != grobjFleet))
        goto L_6b83;
    else
        goto L_6b6e;

L_6b6e:
    if ((sel.fl.idPlanet != id))
        goto L_6b83;
    else
        goto L_6b7a;

L_6b7a:
    tutor.idh = 1514;
    goto L_6bc1;

L_6b83:
    if ((grobj != grobjFleet))
        goto L_6bbb;
    else
        goto L_6b8c;

L_6b8c:
    if ((sel.grobj != grobjPlanet))
        goto L_6bbb;
    else
        goto L_6b96;

L_6b96:
    if ((LpflFromId(id)->idPlanet != sel.pl.id))
        goto L_6bbb;
    else
        goto L_6bb2;

L_6bb2:
    tutor.idh = 1510;
    goto L_6bc1;

L_6bbb:
    tutor.idh = 1526;

L_6bc1:
    fRet = 0;
    goto L_6c19;

L_6bcc:
    if ((sel.grobj != grobjFleet))
        goto L_6be8;
    else
        goto L_6bd6;

L_6bd6:
    if ((sel.fl.id != id))
        goto L_6be8;
    else
        goto L_6be2;

L_6be2:
    t_merge_6beb_0001 = 0x1;
    goto L_6beb;

L_6be8:
    t_merge_6beb_0001 = 0x0;

L_6beb:
    fRet = t_merge_6beb_0001;
    goto L_6c2c;

L_6bf1:
    if ((sel.grobj != grobjPlanet))
        goto L_6c0d;
    else
        goto L_6bfb;

L_6bfb:
    if ((sel.pl.id != id))
        goto L_6c0d;
    else
        goto L_6c07;

L_6c07:
    t_merge_6c10_0001 = 0x1;
    goto L_6c10;

L_6c0d:
    t_merge_6c10_0001 = 0x0;

L_6c10:
    fRet = t_merge_6c10_0001;
    goto L_6c2c;

L_6c19:
    if ((grobj == grobjPlanet))
        goto L_6bf1;
    else
        goto L_6c21;

L_6c21:
    if ((grobj == grobjFleet))
        goto L_6bcc;
    else
        goto L_6c2c;

L_6c2c:
    if ((fRet == 0))
        goto L_6c3b;
    else
        goto L_6c35;

L_6c35:
    tutor.idh = idhSav;

L_6c3b:
    return fRet;
}

int16_t FCheckMessages(int16_t imsg, MessageId idm, int16_t fFilter) {
    int16_t idhSav;

L_6c48:
    idhSav = tutor.idh;
    if ((gd.fGeneratingTurn == 0x0))
        goto L_6c6d;
    else
        goto L_6c67;

L_6c67:
    return 0x1;

L_6c6d:
    tutor.idh = 14001;
    if ((imsg != 9999))
        goto L_6c9a;
    else
        goto L_6c7d;

L_6c7d:
    if ((IMsgNext(0x0) == -1))
        goto L_6c9a;
    else
        goto L_6c91;

L_6c91:
    return 0x0;

L_6c9a:
    if ((imsg == 9999))
        goto L_6cc2;
    else
        goto L_6ca4;

L_6ca4:
    if ((imsg == -1))
        goto L_6cc2;
    else
        goto L_6cad;

L_6cad:
    if ((iMsgCur >= imsg))
        goto L_6cc2;
    else
        goto L_6cb9;

L_6cb9:
    return 0x0;

L_6cc2:
    if ((idm == 0xffff))
        goto L_6d91;
    else
        goto L_6ccb;

L_6ccb:
    if ((fFilter == 0))
        goto L_6d2f;
    else
        goto L_6cd4;

L_6cd4:
    if ((tutor.fAutoComplete == 0x0))
        goto L_6d2f;
    else
        goto L_6ce7;

L_6ce7:
    if (((bitfMsgFiltered[(idm >> 0x3)] & (0x1 << (idm & 0x7))) != 0x0))
        goto L_6d2f;
    else
        goto L_6d14;

L_6d14:
    SetFilteringGroups(idm, 0x1);
    tutor.idh = idhSav;
    return 0x1;

L_6d2f:
    if ((fFilter == 0))
        goto L_6d6e;
    else
        goto L_6d38;

L_6d38:
    if (((bitfMsgFiltered[(idm >> 0x3)] & (0x1 << (idm & 0x7))) != 0x0))
        goto L_6d6e;
    else
        goto L_6d65;

L_6d65:
    return 0x0;

L_6d6e:
    if ((fFilter != 0))
        goto L_6d91;
    else
        goto L_6d77;

L_6d77:
    if ((IdmGetMessageN(iMsgCur) == idm))
        goto L_6d91;
    else
        goto L_6d8b;

L_6d8b:
    return 0x0;

L_6d91:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckResearch(int16_t iTech, int16_t iTechNext, int16_t pct) {
L_6da4:
    /* untranslated: branch (sext8to16(part[57:2](rgplr[0x0])) & 0xf) != iTech ? L_6de2 : L_6dbc */

L_6dbc:
    /* untranslated: branch (sext8to16(part[57:2](rgplr[0x0])) >> 0x4) != iTechNext ? L_6de2 : L_6dd0 */

L_6dd0:
    /* untranslated: branch sext8to16(part[56:2](rgplr[0x0])) != pct ? L_6de2 : L_6ddc */

L_6ddc:
    return 0x1;

L_6de2:
    tutor.idh = 1070;
    return 0x0;
}

int16_t FCheckFleetWP(uint16_t ifl, int16_t iord, GrobjClass grobj, int16_t id, uint16_t grTask, uint16_t iWarp) {
    ORDER    ord;
    int16_t  fRet;
    FLEET   *lpfl;
    int16_t  idh;
    int16_t  idhSav;
    uint16_t t_merge_6ef9_0001;
    uint16_t t_merge_6f33_0001;

L_6df4:
    fRet = 0;
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    tutor.idh = 1526;
    if ((LOWORD(lpfl) != 0x0))
        goto L_6e35;
    else
        goto L_6e29;

L_6e29:
    if ((HIWORD(lpfl) == 0x0))
        goto LReturn;
    else
        goto L_6e2f;

L_6e2f:

L_6e35:
    tutor.idh = 3062;
    if ((lpfl->cord < (iord + 1)))
        goto LReturn;
    else
        goto L_6e4a;

L_6e4a:

L_6e50:
    ord = lpfl->lpplord->rgord[iord];
    if (((id & 0x7fff) == 0x7fff))
        goto L_6ebc;
    else
        goto L_6e89;

L_6e89:
    if ((ord.grobj != grobj))
        goto L_6ea7;
    else
        goto L_6e9c;

L_6e9c:
    if ((ord.id == id))
        goto L_6ebc;
    else
        goto L_6ea7;

L_6ea7:
    TutorError(0x1ec);
    tutor.idh = 3063;
    goto LReturn;

L_6ebc:
    if ((ord.grTask == grTask))
        goto L_6f05;
    else
        goto L_6eca;

L_6eca:
    if ((grTask == 0xffff))
        goto L_6f05;
    else
        goto L_6ed3;

L_6ed3:
    tutor.idh = 1519;
    if ((ord.grTask == grTaskNone))
        goto LReturn;
    else
        goto L_6ee7;

L_6ee7:
    if ((grTask != 0x0))
        goto L_6ef6;
    else
        goto L_6ef0;

L_6ef0:
    t_merge_6ef9_0001 = 0x1ed;
    goto L_6ef9;

L_6ef6:
    t_merge_6ef9_0001 = 0x1ee;

L_6ef9:
    TutorError(t_merge_6ef9_0001);

L_6f05:
    tutor.idh = 1518;
    if ((iWarp == 0xffff))
        goto L_6f39;
    else
        goto L_6f14;

L_6f14:
    if ((ord.iWarp != iWarp))
        goto L_6f30;
    else
        goto L_6f2a;

L_6f2a:
    t_merge_6f33_0001 = 0x1;
    goto L_6f33;

L_6f30:
    t_merge_6f33_0001 = 0x0;

L_6f33:
    fRet = t_merge_6f33_0001;
    goto LReturn;

L_6f39:
    fRet = 1;

LReturn:
    idh = tutor.idh;
    if ((fRet != 0))
        goto L_6f6b;
    else
        goto L_6f4d;

L_6f4d:
    if ((FCheckSelection(grobjFleet, ifl) == 0))
        goto L_6f6b;
    else
        goto L_6f65;

L_6f65:
    tutor.idh = idh;

L_6f6b:
    if ((fRet == 0))
        goto L_6f7a;
    else
        goto L_6f74;

L_6f74:
    tutor.idh = idhSav;

L_6f7a:

L_6f80:
    return fRet;
}

int16_t FCheckPlanetRoute(int16_t idpl, int16_t idplRoute) {
    PLANET *lppl;
    int16_t idhSav;

L_6f86:
    idhSav = tutor.idh;
    lppl = LpplFromId(idpl);
    if ((LOWORD(lppl) != 0x0))
        goto L_6fbe;
    else
        goto L_6faf;

L_6faf:
    if ((HIWORD(lppl) != 0x0))
        goto L_6fbe;
    else
        goto L_6fb8;

L_6fb8:
    return 0x0;

L_6fbe:
    tutor.idh = 1531;
    if ((lppl->idRoute == (idplRoute + 1)))
        goto L_6fe1;
    else
        goto L_6fdb;

L_6fdb:
    return 0x0;

L_6fe1:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckLayingWP(uint16_t ifl, int16_t iord, int16_t id, int16_t iYears) {
    FLEET   *lpfl;
    int16_t  idhSav;
    int16_t  grobj;
    uint16_t t_merge_704a_0001;

L_6ff4:
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto L_702d;
    else
        goto L_701e;

L_701e:
    if ((HIWORD(lpfl) != 0x0))
        goto L_702d;
    else
        goto L_7027;

L_7027:
    return 0x0;

L_702d:
    tutor.idh = 1528;
    if (((id & 0x8000) == 0x0))
        goto L_7047;
    else
        goto L_7041;

L_7041:
    t_merge_704a_0001 = 0x2;
    goto L_704a;

L_7047:
    t_merge_704a_0001 = 0x1;

L_704a:
    grobj = t_merge_704a_0001;
    if ((FCheckFleetWP(ifl, iord, grobj, (id & 0x7fff), 0x6, 0xffff) != 0))
        goto L_707c;
    else
        goto L_7076;

L_7076:
    return 0x0;

L_707c:
    if ((lpfl->lpplord->rgord[iord].tsell.iPlrX == iYears))
        goto L_70ae;
    else
        goto L_70a8;

L_70a8:
    return 0x0;

L_70ae:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckColonizeWP(uint16_t ifl, int16_t id, uint16_t iWarp) {
    int16_t ish;
    FLEET  *lpfl;
    int16_t csh;
    int16_t idhSav;

L_70c0:
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto L_70f9;
    else
        goto L_70ea;

L_70ea:
    if ((HIWORD(lpfl) != 0x0))
        goto L_70f9;
    else
        goto L_70f3;

L_70f3:
    return 0x0;

L_70f9:
    tutor.idh = 1522;
    csh = 0;
    ish = 0;
    goto L_712a;

L_710c:
    csh = (csh + lpfl->rgcsh[ish]);
    ish = (ish + 1);

L_712a:
    if ((ish < 16))
        goto L_710c;
    else
        goto L_7133;

L_7133:
    if ((lpfl->idPlanet != 13))
        goto L_716f;
    else
        goto L_7140;

L_7140:
    if ((FCheckCargo(lpfl, 0x0, 0x0, 0x0, LOWORD((25 * csh))) != 0))
        goto L_716f;
    else
        goto L_7169;

L_7169:
    return 0x0;

L_716f:
    if ((FCheckFleetWP(ifl, 0x1, grobjPlanet, id, 0x2, iWarp) == 0))
        goto L_71a0;
    else
        goto L_7194;

L_7194:
    tutor.idh = idhSav;
    return 0x1;

L_71a0:
    return 0x0;
}

int16_t FCheckPatrolWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, uint16_t iPlan, uint16_t iDist) {
    FLEET   *lpfl;
    int16_t  idhSav;
    int16_t  grobj;
    uint16_t t_merge_7202_0001;

L_71ac:
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto L_71e5;
    else
        goto L_71d6;

L_71d6:
    if ((HIWORD(lpfl) != 0x0))
        goto L_71e5;
    else
        goto L_71df;

L_71df:
    return 0x0;

L_71e5:
    tutor.idh = 3095;
    if (((id & 0x8000) == 0x0))
        goto L_71ff;
    else
        goto L_71f9;

L_71f9:
    t_merge_7202_0001 = 0x2;
    goto L_7202;

L_71ff:
    t_merge_7202_0001 = 0x1;

L_7202:
    grobj = t_merge_7202_0001;
    if ((FCheckFleetWP(ifl, iord, grobj, (id & 0x7fff), 0x7, iWarp) != 0))
        goto L_7233;
    else
        goto L_722d;

L_722d:
    return 0x0;

L_7233:
    if ((iDist == 0xffff))
        goto L_726e;
    else
        goto L_723c;

L_723c:
    /* untranslated: branch part[10:2](lpfl->lpplord->rgord[iord]) == iDist ? L_726e : L_7262 */

L_7262:
    tutor.idh = 1519;
    return 0x0;

L_726e:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckXferWP(uint16_t ifl, int16_t iord, int16_t id, uint16_t iWarp, ITEMACTION *lpiaGoal) {
    ORDER       ord;
    int16_t     fRet;
    ITEMACTION *piaCur;
    int16_t     i;
    FLEET      *lpfl;
    int16_t     idh;
    GrobjClass  grobj;
    int16_t     idhSav;

L_7280:
    fRet = 0;
    idhSav = tutor.idh;
    if (((id & 0x8000) == 0x0))
        goto L_72af;
    else
        goto L_72a2;

L_72a2:
    id = (id & 0x7fff);
    grobj = grobjFleet;
    goto L_72b4;

L_72af:
    grobj = grobjPlanet;

L_72b4:
    lpfl = LpflFromId(ifl);
    if ((LOWORD(lpfl) != 0x0))
        goto L_72de;
    else
        goto L_72cf;

L_72cf:
    if ((HIWORD(lpfl) != 0x0))
        goto L_72de;
    else
        goto L_72d8;

L_72d8:
    return 0x0;

L_72de:
    if ((FCheckFleetWP(ifl, iord, grobj, id, 0x1, iWarp) != 0))
        goto L_7308;
    else
        goto L_7302;

L_7302:
    return 0x0;

L_7308:
    ord = lpfl->lpplord->rgord[iord];
    piaCur = &(ord.tsell.iPlrX);
    tutor.idh = 1519;
    i = 0;
    goto L_7361;

L_7347:
    i = (i + 1);
    piaCur = (piaCur + 0x2);
    lpiaGoal = (lpiaGoal + 0x1);

L_7361:
    if ((i >= 5))
        goto L_73f5;
    else
        goto L_736a;

L_736a:
    if ((piaCur->iAction == lpiaGoal->iAction))
        goto L_73b0;
    else
        goto L_738c;

L_738c:
    if ((piaCur->iAction == iActionNone))
        goto LReturn;
    else
        goto L_73a1;

L_73a1:
    TutorError(0x268);

L_73b0:
    goto L_73df;

L_73c0:
    if ((piaCur->cQuan != lpiaGoal->cQuan))
        goto LReturn;
    else
        goto L_73d6;

L_73d6:

L_73df:
    if ((piaCur->iAction == iActionUnloadExact))
        goto L_73c0;
    else
        goto L_73e7;

L_73e7:
    if ((piaCur->iAction != iActionSetAmount))
        goto L_7347;
    else
        goto L_73ec;

L_73ec:

L_73f5:
    fRet = 1;

LReturn:
    idh = tutor.idh;
    if ((fRet != 0))
        goto L_7427;
    else
        goto L_7409;

L_7409:
    if ((FCheckSelection(grobjFleet, ifl) == 0))
        goto L_7427;
    else
        goto L_7421;

L_7421:
    tutor.idh = idh;

L_7427:
    if ((fRet == 0))
        goto L_7436;
    else
        goto L_7430;

L_7430:
    tutor.idh = idhSav;

L_7436:
    return fRet;
}

int16_t FCheckQueue(int16_t ipl, int16_t iprod, GrobjClass grobj, uint16_t iItem, uint16_t cItem, uint16_t fNoResearch) {
    int16_t fRet;
    PLANET *lppl;
    PROD    prod;
    int16_t idh;
    int16_t idhSav;

L_7442:
    idhSav = tutor.idh;
    fRet = 0;
    if ((game.turn >= 0x2))
        goto L_7469;
    else
        goto L_7460;

L_7460:
    tutor.idh = 1507;
    goto L_746f;

L_7469:
    tutor.idh = 1059;

L_746f:
    lppl = LpplFromId(ipl);
    if ((LOWORD(lppl) != 0x0))
        goto L_7492;
    else
        goto L_7489;

L_7489:
    if ((HIWORD(lppl) == 0x0))
        goto LReturn;
    else
        goto L_7492;

L_7492:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_74a9;
    else
        goto L_749f;

L_749f:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto LReturn;
    else
        goto L_74a9;

L_74a9:
    if ((lppl->lpplprod->iprodMac <= iprod))
        goto LReturn;
    else
        goto L_74bc;

L_74bc:

L_74c2:
    LOWORD(prod) = LOWORD(lppl->lpplprod[(iprod + 0x1)]);
    HIWORD(prod) = HIWORD(lppl->lpplprod[iprod].rgprod[0x0]);
    if ((prod.grobj != grobj))
        goto L_753a;
    else
        goto L_750b;

L_750b:
    if ((0x0 != 0x0))
        goto L_753a;
    else
        goto L_7512;

L_7512:
    if ((prod.iItem != iItem))
        goto L_753a;
    else
        goto L_7533;

L_7533:
    if ((0x0 == 0x0))
        goto L_7549;
    else
        goto L_753a;

L_753a:
    TutorError(0x1eb);
    goto LReturn;

L_7549:
    if ((prod.cItem != cItem))
        goto L_7569;
    else
        goto L_7562;

L_7562:
    if ((0x0 == 0x0))
        goto L_7578;
    else
        goto L_7569;

L_7569:
    TutorError(0x1f0);
    goto LReturn;

L_7578:
    if ((fNoResearch == 0xffff))
        goto L_75bd;
    else
        goto L_7581;

L_7581:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x18), *(lppl+0x1a)) >> 0x17)) & 0x1) != fNoResearch ? L_75ae : L_75a7 */

L_75a7:
    if ((0x0 == 0x0))
        goto L_75bd;
    else
        goto L_75ae;

L_75ae:
    TutorError(0x519);
    goto LReturn;

L_75bd:
    fRet = 1;

LReturn:
    idh = tutor.idh;
    if ((fRet != 0))
        goto L_75ee;
    else
        goto L_75d1;

L_75d1:
    if ((FCheckSelection(grobjPlanet, ipl) == 0))
        goto L_75ee;
    else
        goto L_75e8;

L_75e8:
    tutor.idh = idh;

L_75ee:
    if ((fRet == 0))
        goto L_75fd;
    else
        goto L_75f7;

L_75f7:
    tutor.idh = idhSav;

L_75fd:

L_7603:
    return fRet;
}

int16_t FCheckBtlPlan(int16_t ibp, uint16_t imdTarget, uint16_t fSpread, uint16_t fBomb, uint16_t fDump, uint16_t mdUnarmed, uint16_t mdScout, uint16_t mdWar,
                      uint16_t mdBomber) {
    BTLPLAN *lpbtlplan;
    int16_t  idhSav;

L_760a:
    idhSav = tutor.idh;
    tutor.idh = 3105;
    if ((ibp < 0))
        goto L_7636;
    else
        goto L_7628;

L_7628:
    if ((ibp <= (LOWORD(rgcbtlplan) & 0xff)))
        goto L_763c;
    else
        goto L_7636;

L_7636:
    return 0x0;

L_763c:
    lpbtlplan = &(rglpbtlplan[0x0][ibp]);
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckCargo(FLEET *lpfl, int16_t wtMin1, int16_t wtMin2, int16_t wtMin3, int16_t wtColonists) {
    int16_t fRet;
    int16_t idh;
    int16_t idhSav;

L_7664:
    idhSav = tutor.idh;
    fRet = 0;
    if ((LOWORD(lpfl) != 0x0))
        goto L_7690;
    else
        goto L_7681;

L_7681:
    if ((HIWORD(lpfl) != 0x0))
        goto L_7690;
    else
        goto L_768a;

L_768a:
    return 0x0;

L_7690:
    tutor.idh = 1075;
    if ((wtMin1 != 0))
        goto L_76b6;
    else
        goto L_769f;

L_769f:
    if ((LOWORD(lpfl->rgwtMin[0x0]) != 0x0))
        goto L_7716;
    else
        goto L_76ac;

L_76ac:
    if ((HIWORD(lpfl->rgwtMin[0x0]) != 0x0))
        goto L_7716;
    else
        goto L_76b6;

L_76b6:
    if ((wtMin2 != 0))
        goto L_76d6;
    else
        goto L_76bf;

L_76bf:
    if ((LOWORD(lpfl->rgwtMin[0x1]) != 0x0))
        goto L_7716;
    else
        goto L_76cc;

L_76cc:
    if ((HIWORD(lpfl->rgwtMin[0x1]) != 0x0))
        goto L_7716;
    else
        goto L_76d6;

L_76d6:
    if ((wtMin3 != 0))
        goto L_76f6;
    else
        goto L_76df;

L_76df:
    if ((LOWORD(lpfl->rgwtMin[0x2]) != 0x0))
        goto L_7716;
    else
        goto L_76ec;

L_76ec:
    if ((HIWORD(lpfl->rgwtMin[0x2]) != 0x0))
        goto L_7716;
    else
        goto L_76f6;

L_76f6:
    if ((wtColonists != 0))
        goto L_7725;
    else
        goto L_76ff;

L_76ff:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_7716;
    else
        goto L_770c;

L_770c:
    if ((HIWORD(lpfl->rgwtMin[0x3]) == 0x0))
        goto L_7725;
    else
        goto L_7716;

L_7716:
    TutorError(0x1ef);
    goto LReturn;

L_7725:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != wtColonists))
        goto LReturn;
    else
        goto L_7735;

L_7735:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != SIGNHIWORD(wtColonists)))
        goto LReturn;
    else
        goto L_773e;

L_773e:
    if ((LOWORD(lpfl->rgwtMin[0x0]) != wtMin1))
        goto LReturn;
    else
        goto L_774e;

L_774e:
    if ((HIWORD(lpfl->rgwtMin[0x0]) != SIGNHIWORD(wtMin1)))
        goto LReturn;
    else
        goto L_7757;

L_7757:
    if ((LOWORD(lpfl->rgwtMin[0x1]) != wtMin2))
        goto LReturn;
    else
        goto L_7767;

L_7767:
    if ((HIWORD(lpfl->rgwtMin[0x1]) != SIGNHIWORD(wtMin2)))
        goto LReturn;
    else
        goto L_7770;

L_7770:
    if ((LOWORD(lpfl->rgwtMin[0x2]) != wtMin3))
        goto LReturn;
    else
        goto L_7780;

L_7780:
    if ((HIWORD(lpfl->rgwtMin[0x2]) != SIGNHIWORD(wtMin3)))
        goto LReturn;
    else
        goto L_7789;

L_7789:
    fRet = 1;

LReturn:
    idh = tutor.idh;
    if ((fRet != 0))
        goto L_77bd;
    else
        goto L_779d;

L_779d:
    if ((FCheckSelection(grobjFleet, lpfl->id) == 0))
        goto L_77bd;
    else
        goto L_77b7;

L_77b7:
    tutor.idh = idh;

L_77bd:
    if ((fRet == 0))
        goto L_77cc;
    else
        goto L_77c6;

L_77c6:
    tutor.idh = idhSav;

L_77cc:
    return fRet;
}

int16_t FCheckBuilderPart(int16_t iSlot, HS *phs, uint16_t cInit) {
    uint16_t cItemAct;
    int16_t  idhSav;

L_77d8:
    idhSav = tutor.idh;
    if ((hwndSlotDlg != 0x0))
        goto L_77fd;
    else
        goto L_77f1;

L_77f1:
    tutor.idh = 1066;
    return 0x0;

L_77fd:
    tutor.idh = 3040;
    if ((mdBuild == mdBuildEdit))
        goto L_7813;
    else
        goto L_780d;

L_780d:
    return 0x0;

L_7813:
    cItemAct = lpshdefBuild->hul.rghs[iSlot].cItem;
    if ((phs->cItem != 0x0))
        goto L_785f;
    else
        goto L_7850;

L_7850:
    if ((cItemAct != 0x0))
        goto L_785f;
    else
        goto L_7859;

L_7859:
    return 0x1;

L_785f:
    if ((cInit >= phs->cItem))
        goto L_78b1;
    else
        goto L_7875;

L_7875:
    if ((cItemAct < cInit))
        goto BadCnt;
    else
        goto L_7880;

L_7880:
    if ((cItemAct <= phs->cItem))
        goto L_78d5;
    else
        goto BadCnt;

BadCnt:
    TutorError(0x1f5);

BadCntSilent:
    tutor.idh = 3039;
    return 0x0;

L_78b1:
    if ((cItemAct < phs->cItem))
        goto BadCnt;
    else
        goto L_78c7;

L_78c7:
    if ((cItemAct > cInit))
        goto BadCnt;
    else
        goto L_78cf;

L_78cf:

L_78d5:
    if ((cItemAct != phs->cItem))
        goto BadCntSilent;
    else
        goto L_78e8;

L_78e8:

L_78ee:
    if ((phs->grhst != lpshdefBuild->hul.rghs[iSlot].grhst))
        goto L_7943;
    else
        goto L_7913;

L_7913:
    if ((phs->iItem == lpshdefBuild->hul.rghs[iSlot].iItem))
        goto L_7952;
    else
        goto L_7943;

L_7943:
    TutorError(0x1f6);
    goto BadCntSilent;

L_7952:
    tutor.idh = idhSav;
    return 0x1;
}

int16_t FCheckShipBuilder(int16_t iCategory, int16_t iShip) {
    int16_t iSel;
    int16_t idhSav;

L_7964:
    idhSav = tutor.idh;
    tutor.idh = 1066;
    if ((hwndSlotDlg != 0x0))
        goto L_7989;
    else
        goto L_7983;

L_7983:
    return 0x0;

L_7989:
    if ((iCategory == -1))
        goto L_79a3;
    else
        goto L_7992;

L_7992:
    if ((iCategory == mdBuild))
        goto L_79a3;
    else
        goto L_799d;

L_799d:
    return 0x0;

L_79a3:
    iSel = LOWORD(SendMessage(GetDlgItem(hwndSlotDlg, IDC_COMBOBOX), CB_GETCURSEL, 0x0, 0x0));
    if ((iShip == -1))
        goto L_79dd;
    else
        goto L_79d2;

L_79d2:
    if ((iShip != iSel))
        goto L_79e9;
    else
        goto L_79dd;

L_79dd:
    tutor.idh = idhSav;
    return 0x1;

L_79e9:
    return 0x0;
}

int16_t FTutorialEnabledShipBuilder(int16_t itutsbAction) {
    HS       hs2;
    HS       hs3;
    HS       hs;
    HS       hs1;
    HS       hs4;
    uint16_t t_merge_81d4_0001;

L_79f6:
    goto L_81a9;

L_7a05:
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7a0b:
    TutorError(0x1f1);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7a1d:
    goto L_7c15;

NoCustom:
    TutorError(0x1f2);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7a35:
    if ((tutor.idt != 328))
        goto NoCustom;
    else
        goto L_7a40;

L_7a40:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x7 ? L_7a5e : L_7a4c */

L_7a4c:
    TutorError(0x1f3);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7a5e:
    if ((FCheckShipBuilder(0x1, 0x7) != 0))
        goto L_7c40;
    else
        goto L_7a76;

L_7a76:
    TutorError(0x1fe);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7a91:
    if ((tutor.idt != 416))
        goto NoCustom;
    else
        goto L_7a9c;

L_7a9c:
    if ((rgplr[0].cshdefSB != 0x2))
        goto L_7ac1;
    else
        goto L_7aaf;

L_7aaf:
    TutorError(0x1f3);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7ac1:
    if ((FCheckShipBuilder(0x0, 0x0) == 0))
        goto L_7ae3;
    else
        goto L_7ad9;

L_7ad9:
    if ((fStarbaseMode != 0))
        goto L_7c40;
    else
        goto L_7ae3;

L_7ae3:
    TutorError(0x1f4);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7afe:
    if ((tutor.idt != 448))
        goto NoCustom;
    else
        goto L_7b09;

L_7b09:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x8 ? L_7b27 : L_7b15 */

L_7b15:
    TutorError(0x1f3);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7b27:
    if ((FCheckShipBuilder(0x1, 0x3) != 0))
        goto L_7c40;
    else
        goto L_7b3f;

L_7b3f:
    TutorError(0x1fe);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7b5a:
    if ((tutor.idt != 544))
        goto NoCustom;
    else
        goto L_7b65;

L_7b65:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0x9 ? L_7b83 : L_7b71 */

L_7b71:
    TutorError(0x1f3);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7b83:
    if ((FCheckShipBuilder(0x1, 0x4) != 0))
        goto L_7c40;
    else
        goto L_7b9b;

L_7b9b:
    TutorError(0x1fe);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7bb6:
    if ((tutor.idt != 576))
        goto NoCustom;
    else
        goto L_7bc1;

L_7bc1:
    /* untranslated: branch sext8to16(part[1:2](rgplr[0x0])) != 0xa ? L_7bdf : L_7bcd */

L_7bcd:
    TutorError(0x1f3);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7bdf:
    if ((FCheckShipBuilder(0x1, 0x8) != 0))
        goto L_7c40;
    else
        goto L_7bf7;

L_7bf7:
    TutorError(0x1fe);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7c15:
    if ((game.turn == 0xd))
        goto L_7a35;
    else
        goto L_7c1d;

L_7c1d:
    if ((game.turn == 0x14))
        goto L_7a91;
    else
        goto L_7c25;

L_7c25:
    if ((game.turn == 0x16))
        goto L_7afe;
    else
        goto L_7c2d;

L_7c2d:
    if ((game.turn == 0x1b))
        goto L_7b5a;
    else
        goto L_7c35;

L_7c35:
    if ((game.turn != 0x1d))
        goto NoCustom;
    else
        goto L_7c3a;

L_7c3a:

L_7c40:
    t_merge_81d4_0001 = 0x1;
    goto L_81d4;

L_7c46:
    goto L_7c90;

L_7c4f:
    if ((tutor.idt != 496))
        goto NoCustom;
    else
        goto L_7c5a;

L_7c5a:
    if ((FCheckShipBuilder(0x0, 0x2) != 0))
        goto L_f0000000;
    else
        goto L_7c72;

L_f0000000:
    /* untranslated: t_merge_81d4_0001 = callresult(int16_t) */
    goto L_81d4;

L_7c72:
    TutorError(0x1ff);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_7c90:
    if ((game.turn != 0x19))
        goto NoCustom;
    else
        goto L_7c95;

L_7c95:

L_7c9e:
    goto L_8158;

L_8158:
    if (((game.turn - 0xd) > 0x10))
        goto NoCustom;
    else
        goto L_8163;

L_8163:
    goto L_ffffffff;

L_8194:
    TutorError(0x1f7);
    t_merge_81d4_0001 = 0x0;
    goto L_81d4;

L_81a9:
    if ((itutsbAction == 0))
        goto L_7a0b;
    else
        goto L_81b1;

L_81b1:
    if ((itutsbAction == 1))
        goto L_7a1d;
    else
        goto L_81b9;

L_81b9:
    if ((itutsbAction == 2))
        goto L_7c46;
    else
        goto L_81c1;

L_81c1:
    if ((itutsbAction == 3))
        goto L_7c9e;
    else
        goto L_81c9;

L_81c9:
    if ((itutsbAction != 4))
        goto L_7a05;
    else
        goto L_81ce;

L_81ce:

L_81d4:
    return t_merge_81d4_0001;
}

int16_t FOKMergeDialog() {
L_81da:
    goto L_8276;

L_81e9:
    TutorError(0x204);
    return 0x0;

L_81fb:
    if ((*(vrgiflMerge) == -1))
        goto L_8289;
    else
        goto L_8207;

L_8207:
    if ((vrgiflMerge[1] == -1))
        goto L_8289;
    else
        goto L_8214;

L_8214:
    if ((vrgiflMerge[2] != -1))
        goto L_8289;
    else
        goto L_8221;

L_8221:
    if ((vrgiflMerge[3] != -1))
        goto L_8289;
    else
        goto L_822e;

L_822e:
    return 0x1;

L_8237:
    if ((*(vrgiflMerge) != -1))
        goto L_8289;
    else
        goto L_8243;

L_8243:
    if ((vrgiflMerge[1] != -1))
        goto L_8289;
    else
        goto L_8250;

L_8250:
    if ((vrgiflMerge[2] == -1))
        goto L_8289;
    else
        goto L_825d;

L_825d:
    if ((vrgiflMerge[3] == -1))
        goto L_8289;
    else
        goto L_826a;

L_826a:
    return 0x1;

L_8276:
    if ((game.turn == 0x17))
        goto L_81fb;
    else
        goto L_827e;

L_827e:
    if ((game.turn != 0x1b))
        goto L_81e9;
    else
        goto L_8283;

L_8283:

L_8289:
    TutorError(0x202);
    tutor.idh = 1107;
    return 0x0;
}
