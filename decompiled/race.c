#include "common.h"

int16_t rgRacePrimaryTrait[10] = {40, 95, 45, 10, -100, -150, 120, 180, 90, -66};
char    rgRW3Spacing[7] = {4, 3, 3, 3, 3, 3, 3};
int16_t rgRaceAdvDisPts[14] = {-235, -25, -159, -201, 40, -240, -155, 160, 240, 255, 325, 180, 70, 30};
char    rgRW3IStat[7] = "";
int16_t rgRaceDisEnvPts[6] = {150, 330, 540, 780, 1050, 1380};
char    rgRW3Width[7] = {-2, 2, 2, 2, -2, 2, 2};
char    rgRaceStatMax[16] = {25, 15, 25, 25, 25, 15, 25, 6, 2, 2, 2, 2, 2, 2, 9};
char    rgRaceStatMin[16] = {7, 5, 5, 5, 5, 2, 5};

int16_t RaceCreationWizard(HWND hwndParent, int16_t fReadOnly, int16_t fDontWrite) {
    int16_t mdRet;
    int16_t (**lpProc)();
    RECT    rgrcStack[17];
    int16_t cpts;
    char   *t_merge_038c_0001;

L_0000:
    vrgrcRCW = rgrcStack;
    fRCWReadOnly = fReadOnly;
    hwndRaceParent = hwndParent;

Step1:
    iPanelActive = 1;
    lpProc = MakeProcInstance(RaceWizardDlg1, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_1, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_0071;
    else
        goto L_006b;

L_006b:
    return 0x0;

L_0071:
    if ((mdRet == 3))
        goto Finish;
    else
        goto L_0077;

L_0077:

Step2:
    iPanelActive = 2;
    lpProc = MakeProcInstance(RaceWizardDlg4, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_4, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_00d2;
    else
        goto L_00cc;

L_00cc:
    return 0x0;

L_00d2:
    if ((mdRet == 1))
        goto Step1;
    else
        goto L_00d8;

L_00d8:

L_00de:
    if ((mdRet == 3))
        goto Finish;
    else
        goto L_00e4;

L_00e4:

Step3:
    iPanelActive = 3;
    lpProc = MakeProcInstance(RaceWizardDlg5, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_5, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_013f;
    else
        goto L_0139;

L_0139:
    return 0x0;

L_013f:
    if ((mdRet == 1))
        goto Step2;
    else
        goto L_0145;

L_0145:

L_014b:
    if ((mdRet == 3))
        goto Finish;
    else
        goto L_0151;

L_0151:

Step4:
    iPanelActive = 4;
    lpProc = MakeProcInstance(RaceWizardDlg2, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_2, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_01ac;
    else
        goto L_01a6;

L_01a6:
    return 0x0;

L_01ac:
    if ((mdRet == 1))
        goto Step3;
    else
        goto L_01b2;

L_01b2:

L_01b8:
    if ((mdRet == 3))
        goto Finish;
    else
        goto L_01be;

L_01be:

Step5:
    iPanelActive = 5;
    lpProc = MakeProcInstance(RaceWizardDlg3, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_3, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_0219;
    else
        goto L_0213;

L_0213:
    return 0x0;

L_0219:
    if ((mdRet == 1))
        goto Step4;
    else
        goto L_021f;

L_021f:

L_0225:
    if ((mdRet == 3))
        goto Finish;
    else
        goto L_022b;

L_022b:

Step6:
    iPanelActive = 6;
    lpProc = MakeProcInstance(RaceWizardDlg6, hInst);
    mdRet = DialogBox(hInst, IDD_RACE_WIZARD_6, hwndRaceParent, &(lpProc));
    FreeProcInstance(&(lpProc));
    if ((mdRet != 0))
        goto L_0286;
    else
        goto L_0280;

L_0280:
    return 0x0;

L_0286:
    if ((mdRet == 1))
        goto Step5;
    else
        goto L_028c;

L_028c:

L_0292:
    if ((mdRet == 3))
        goto Finish;
    else
        goto Finish;

Finish:
    if ((fRCWReadOnly == 0))
        goto L_02ae;
    else
        goto L_02a8;

L_02a8:
    return 0x0;

L_02ae:
    cpts = CAdvantagePoints(&(vplr));
    if ((cpts >= 0))
        goto L_0344;
    else
        goto L_02c8;

L_02c8:
    /* untranslated: call _wsprintf(szWork, PszGetCompressedString(idsAdvantagePointsCurrentlyHoleDPointsCannot), (cpts neg 0)) -> callresult(int16_t) */
    AlertSz(szWork, MB_ICONHAND);
    goto L_0319;

L_0319:
    if ((iPanelActive == 2))
        goto Step2;
    else
        goto L_0321;

L_0321:
    if ((iPanelActive == 3))
        goto Step3;
    else
        goto L_0329;

L_0329:
    if ((iPanelActive == 4))
        goto Step4;
    else
        goto L_0331;

L_0331:
    if ((iPanelActive == 5))
        goto Step5;
    else
        goto L_0339;

L_0339:
    if ((iPanelActive != 6))
        goto Step1;
    else
        goto L_033e;

L_033e:

L_0344:
    lSaltCur = LSaltFromSz(szRacePass);
    lSaltLast = -5;
    if ((FCheckPassword() == 0))
        goto Step1;
    else
        goto L_036d;

L_036d:

L_0373:
    if (((uint16_t)(LOWORD(szRaceFile)) == 0x0))
        goto L_0389;
    else
        goto L_0383;

L_0383:
    t_merge_038c_0001 = szRaceFile;
    goto L_038c;

L_0389:
    t_merge_038c_0001 = "stars.r1";

L_038c:
    if ((FSaveRace(t_merge_038c_0001, &(vplr)) == 0))
        goto Step1;
    else
        goto L_039a;

L_039a:

L_03a0:
    return 0x1;
}

int16_t RaceWizardDlg1(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    int16_t     iPlrBmp;
    HWND        hwndCB;
    char       *psz;
    POINT       pt;
    HDC         hdc;
    BTNT        btnt;
    int16_t     bt;
    RECT       *prc;
    int16_t     iDir;
    int16_t     iCur;
    int16_t     iOffset;
    PLAYER     *pplr;
    PAINTSTRUCT ps;
    int16_t     j;
    int16_t     cch;
    RECT        rcGBox;
    notype      k;
    char        szBuf[32];
    uint16_t    t_merge_0b8c_0001;
    uint16_t    t_merge_0f7d_0001;

L_03ac:
    goto L_1016;

L_03bb:
    iPlrBmp = vplr.iPlrBmp;
    SetRCWTitle(hwnd, iPanelActive);
    SetDlgItemText(hwnd, IDC_EDIT1, "");
    SetDlgItemText(hwnd, IDC_EDITNAME, "");
    if (((uint16_t)(LOWORD(vplr.szName)) != 0x0))
        goto L_0435;
    else
        goto L_040b;

L_040b:
    GetDlgItemText(hwnd, IDC_RADRACE1, "", 0x10);
    SetDlgItemText(hwnd, IDC_EDIT1, "");

L_0435:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    if ((game.fTutorial == 0x0))
        goto L_0478;
    else
        goto L_045c;

L_045c:
    if ((idPlayer != 0))
        goto L_0478;
    else
        goto L_0466;

L_0466:
    if ((fRCWReadOnly == 0))
        goto L_0478;
    else
        goto L_0470;

L_0470:
    i = 0;
    goto L_050a;

L_0478:
    i = 0;
    goto L_04e7;

L_0480:
    vplr.wMdPlr = ((vplr.wMdPlr & 0xff07) | ((((vrgplrDef[i].wMdPlr >> 0x3) & 0x1f) & 0x1f) << 0x3));
    if ((fmemcmp(vplr, &(vrgplrDef[i]), 0x80) == 0))
        goto L_04f0;
    else
        goto L_04dd;

L_04dd:

L_04e3:
    i = (i + 1);

L_04e7:
    if ((i < 7))
        goto L_0480;
    else
        goto L_04f0;

L_04f0:
    vplr.wMdPlr = ((vplr.wMdPlr & 0xff07) | ((iPlrBmp & 0x1f) << 0x3));

L_050a:
    CheckRadioButton(hwnd, 0x10f, 0x116, (i + 271));
    SendDlgItemMessage(hwnd, 0x10c, 0x415, 0xf, 0x0);
    SendDlgItemMessage(hwnd, 0x81b, 0x415, 0xf, 0x0);
    SendDlgItemMessage(hwnd, 0x10d, 0x415, 0x10, 0x0);
    hwndCB = GetDlgItem(hwnd, IDC_COMBOBOX);
    i = 262;
    goto L_05b5;

L_058c:
    psz = PszGetCompressedString(i);
    /* untranslated: call SendMessage(hwndCB, CB_ADDSTRING, 0x0, words(ds, psz)) -> callresult(LRESULT) */
    i = (i + 1);

L_05b5:
    if ((i <= 266))
        goto L_058c;
    else
        goto L_05bf;

L_05bf:
    i = GetRaceStat(&(vplr), rsUseLeftover);
    SendMessage(hwndCB, CB_SETCURSEL, i, 0x0);
    if ((LOWORD(vplr.lSalt) != 0x0))
        goto L_05fe;
    else
        goto L_05f4;

L_05f4:
    if ((HIWORD(vplr.lSalt) == 0x0))
        goto L_0611;
    else
        goto L_05fe;

L_05fe:
    SetDlgItemText(hwnd, IDC_U16_0x010D, szRacePass);

L_0611:
    if ((fRCWReadOnly == 0))
        goto L_0693;
    else
        goto L_061b;

L_061b:
    i = 268;
    goto L_063c;

L_0623:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 1);

L_063c:
    if ((i <= 269))
        goto L_0623;
    else
        goto L_0646;

L_0646:
    EnableWindow(GetDlgItem(hwnd, IDC_EDITNAME), 0x0);
    i = 271;
    goto L_067d;

L_0664:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 1);

L_067d:
    if ((i <= 278))
        goto L_0664;
    else
        goto L_0687;

L_0687:
    EnableWindow(hwndCB, 0x0);

L_0693:
    return 0x1;

L_0699:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    if ((fRCWReadOnly != 0))
        goto L_1051;
    else
        goto L_06c3;

L_06c3:
    if ((PtInRect(rgrcBuildSpin, pt) != 0))
        goto L_06f7;
    else
        goto L_06dd;

L_06dd:
    if ((PtInRect(&(rgrcBuildSpin[0x1]), pt) == 0))
        goto L_1051;
    else
        goto L_06f7;

L_06f7:
    if ((PtInRect(rgrcBuildSpin, pt) == 0))
        goto L_0723;
    else
        goto L_0711;

L_0711:
    iDir = -1;
    bt = 34;
    prc = rgrcBuildSpin;
    goto L_0732;

L_0723:
    iDir = 1;
    bt = 35;
    prc = &(rgrcBuildSpin[0x1]);

L_0732:
    iCur = vplr.iPlrBmp;
    if ((iCur < 32))
        goto L_074f;
    else
        goto L_074a;

L_074a:
    iCur = 0;

L_074f:
    hdc = GetDC(hwnd);
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    InitBtnTrack(&(btnt), hwnd, 0x0, prc, bt, 0x50, 0x0, 0x0, 0x0);

L_079b:
    if ((FTrackBtn(&(btnt)) == 0))
        goto L_0823;
    else
        goto L_07af;

L_07af:
    iCur = ((uint32_t)(((iCur + 0x20) + iDir)) % 32);
    DibBlt(hdc, (rgrcBuildSpin[0].left - 2), (rgrcBuildSpin[0].top - 35), 0x20, 0x20, hdibRaces, ((iCur & 0x7) * 0x20), ((0x3 - (iCur >> 0x3)) * 0x20), 0x20,
           0x20, 0xcc0020);
    goto L_079b;

L_0823:
    vplr.wMdPlr = ((vplr.wMdPlr & 0xff07) | ((iCur & 0x1f) << 0x3));
    ReleaseDC(hwnd, hdc);
    return 0x1;

L_0851:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0879:
    i = 271;
    goto L_08a6;

L_0881:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_08b0 : L_089c */

L_089c:

L_08a2:
    i = (i + 1);

L_08a6:
    if ((i <= 278))
        goto L_0881;
    else
        goto L_08b0;

L_08b0:
    if ((i <= 278))
        goto L_08d6;
    else
        goto L_08ba;

L_08ba:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_1051;
    else
        goto L_08d6;

L_08d6:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_08ef:
    hdc = BeginPaint(hwnd, &(ps));
    j = 271;
    goto L_0923;

L_0909:
    if ((IsDlgButtonChecked(hwnd, j) != 0x0))
        goto L_092d;
    else
        goto L_0919;

L_0919:

L_091f:
    j = (j + 1);

L_0923:
    if ((j <= 278))
        goto L_0909;
    else
        goto L_092d;

L_092d:
    if ((j > 277))
        goto L_0951;
    else
        goto L_0937;

L_0937:
    pplr = vrgplrDef[(j - 271)];
    goto L_0956;

L_0951:
    pplr = vplr;

L_0956:
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), pplr);
    GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0116), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    cch = CchGetString(idsPredefinedRaces, szWork);
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox.right));
    pt.x = (rcGBox.right + 32);
    pt.y = (rcGBox.bottom - 32);
    iOffset = vplr.iPlrBmp;
    if ((iOffset < 32))
        goto L_0aa6;
    else
        goto L_0aa1;

L_0aa1:
    iOffset = 0;

L_0aa6:
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, pt.x, pt.y, 0x20, 0x20, hdibRaces, ((iOffset & 0x7) * 0x20), ((0x3 - (iOffset >> 0x3)) * 0x20), 0x20, 0x20, 0xcc0020);
    if ((fRCWReadOnly != 0))
        goto L_0bb7;
    else
        goto L_0b1f;

L_0b1f:
    SetRect(rgrcBuildSpin, (pt.x + 2), (pt.y + 35), (pt.x + 16), (pt.y + 49));
    rgrcBuildSpin[1] = rgrcBuildSpin[0];
    OffsetRect(&(rgrcBuildSpin[0x1]), 0xe, 0x0);
    i = 0;
    goto L_0bae;

L_0b72:
    if ((i != 0))
        goto L_0b89;
    else
        goto L_0b83;

L_0b83:
    t_merge_0b8c_0001 = 0x2;
    goto L_0b8c;

L_0b89:
    t_merge_0b8c_0001 = 0x3;

L_0b8c:
    DrawBtn(hdc, rgrcBuildSpin[i], (t_merge_0b8c_0001 | 0x20), 0x0, 0x0);
    i = (i + 1);

L_0bae:
    if ((i < 2))
        goto L_0b72;
    else
        goto L_0bb7;

L_0bb7:
    EndPaint(hwnd, &(ps));
    return 0x1;

L_0bcc:
    if ((wParam != 0x76))
        goto L_0bf6;
    else
        goto L_0bd5;

L_0bd5:
    WinHelp(hwnd, szHelpFile, 0x1, 0x3ff);
    return 0x1;

L_0bf6:
    i = 0;
    goto L_0c16;

L_0bfe:
    if ((wParam == rgidRaceBtn[i]))
        goto L_0c1f;
    else
        goto L_0c0c;

L_0c0c:

L_0c12:
    i = (i + 1);

L_0c16:
    if ((i < 4))
        goto L_0bfe;
    else
        goto L_0c1f;

L_0c1f:
    if ((i >= 4))
        goto L_0d7a;
    else
        goto L_0c28;

L_0c28:
    if ((wParam == 0x2))
        goto L_0d53;
    else
        goto L_0c31;

L_0c31:
    iPlrBmp = vplr.iPlrBmp;
    j = 271;
    goto L_0c62;

L_0c48:
    if ((IsDlgButtonChecked(hwnd, j) != 0x0))
        goto L_0c6c;
    else
        goto L_0c58;

L_0c58:

L_0c5e:
    j = (j + 1);

L_0c62:
    if ((j <= 278))
        goto L_0c48;
    else
        goto L_0c6c;

L_0c6c:
    if ((j > 277))
        goto L_0c99;
    else
        goto L_0c76;

L_0c76:
    vplr = vrgplrDef[(j - 271)];

L_0c99:
    GetDlgItemText(hwnd, IDC_EDIT1, "", 0x20);
    GetDlgItemText(hwnd, IDC_EDITNAME, "", 0x20);
    GetRaceStat(&(vplr), rsUseLeftover);
    GetDlgItemText(hwnd, IDC_U16_0x010D, szRacePass, 0x10);
    j = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_GETCURSEL, 0x0, 0x0));
    SetRaceStat(&(vplr), rsUseLeftover, j);
    vplr.lSalt = LSaltFromSz(szRacePass);
    vplr.wMdPlr = ((vplr.wMdPlr & 0xff07) | ((iPlrBmp & 0x1f) << 0x3));

L_0d53:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_0d7a:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_1051;
    else
        goto L_0d96;

L_0d96:
    if ((wParam < 0x10f))
        goto L_1051;
    else
        goto L_0da0;

L_0da0:
    if ((wParam > 0x116))
        goto L_1051;
    else
        goto L_0daa;

L_0daa:
    memset(&(vplr.szName), 0x0, 0x20);
    GetDlgItemText(hwnd, IDC_EDIT1, "", 0x20);
    memset(&(vplr.szNames), 0x0, 0x20);
    GetDlgItemText(hwnd, IDC_EDITNAME, "", 0x20);
    GetDlgItemText(hwnd, wParam, szBuf, 0x20);
    i = 0;
    goto L_0e4a;

L_0e1f:
    if ((strcmp(vplr.szName, PszGetCompressedString((i + 1383))) == 0))
        goto L_0e53;
    else
        goto L_0e40;

L_0e40:

L_0e46:
    i = (i + 1);

L_0e4a:
    if ((i < 7))
        goto L_0e1f;
    else
        goto L_0e53;

L_0e53:
    if ((i >= 7))
        goto L_0ef9;
    else
        goto L_0e5c;

L_0e5c:
    if ((wParam >= 0x116))
        goto L_0ef9;
    else
        goto L_0e66;

L_0e66:
    memset(&(vplr.szName), 0x0, 0x20);
    CchGetString((wParam + 0x458), vplr.szName);
    SetDlgItemText(hwnd, IDC_EDIT1, "");
    memset(&(vplr.szNames), 0x0, 0x20);
    psz = PszPlayerName(0x0, 0x1, 0x1, 0x0, 0x0, &(vplr));
    strcpy(vplr.szNames, psz);
    SetDlgItemText(hwnd, IDC_EDITNAME, "");

L_0ef9:
    if ((wParam > 0x115))
        goto L_0f19;
    else
        goto L_0f03;

L_0f03:
    pplr = vrgplrDef[(wParam - 271)];
    goto L_0f1e;

L_0f19:
    pplr = vplr;

L_0f1e:
    InvalidateAdvPtsRect(hwnd);
    i = GetRaceStat(pplr, rsUseLeftover);
    SendMessage(GetDlgItem(hwnd, IDC_COMBOBOX), CB_SETCURSEL, i, 0x0);
    GetDlgItem(hwnd, IDC_NEXT);
    if ((wParam == 0x115))
        goto L_0f7a;
    else
        goto L_0f74;

L_0f74:
    t_merge_0f7d_0001 = 0x1;
    goto L_0f7d;

L_0f7a:
    t_merge_0f7d_0001 = 0x0;

L_0f7d:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_0f7d_0001) -> callresult(int16_t) */
    vplr.wMdPlr = ((vplr.wMdPlr & 0xff07) | ((pplr->iPlrBmp & 0x1f) << 0x3));
    GetWindowRect(GetDlgItem(hwnd, IDC_COMBOBOX), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rc.left = (rc.right + 32);
    rc.top = (rc.bottom - 32);
    rc.right = (rc.left + 32);
    rc.bottom = (rc.top + 32);
    InvalidateRect(hwnd, &(rc), 0x1);

L_1016:
    if ((message == WM_PAINT))
        goto L_08ef;
    else
        goto L_101e;

L_101e:
    if ((message == WM_ERASEBKGND))
        goto L_0851;
    else
        goto L_1026;

L_1026:
    if ((message == WM_CTLCOLOR))
        goto L_0879;
    else
        goto L_102e;

L_102e:
    if ((message == WM_INITDIALOG))
        goto L_03bb;
    else
        goto L_1036;

L_1036:
    if ((message == WM_COMMAND))
        goto L_0bcc;
    else
        goto L_103e;

L_103e:
    if ((message == WM_LBUTTONDOWN))
        goto L_0699;
    else
        goto L_1046;

L_1046:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_0699;
    else
        goto L_1051;

L_1051:
    return 0x0;
}

int16_t RaceWizardDlg2(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    int16_t     yTop;
    int16_t     dy;
    int16_t     dxMiddle;
    int16_t     dxLabel;
    int16_t     cch;
    char        szTemp[20];
    HFONT       hfontSav;
    PAINTSTRUCT ps;
    POINT       pt;
    int16_t     iVar;
    uint16_t    t_merge_136b_0001;

L_1060:
    goto L_1749;

L_106f:
    SetRCWTitle(hwnd, iPanelActive);
    viStore = -1;
    hdc = GetDC(hwnd);
    GetClientRect(hwnd, &(rc));
    hfontSav = SelectObject(hdc, rghfontArial8[1]);
    cch = CchGetString(idsTemperature, szTemp);
    dxLabel = (LOWORD(GetTextExtent(hdc, szTemp, cch)) + 10);
    dxMiddle = (LOWORD(GetTextExtent(hdc, "200mR", 0x5)) + 10);
    dxMiddle = ((rc.right - dxLabel) - dxMiddle);
    /* untranslated: dy = (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 2) */
    yTop = LOWORD((0x3 * dyArial8));
    SetRect(vrgrcRCW, dxLabel, yTop, (dxLabel + dy), (yTop + dy));
    SetRect(((uint8_t *)(vrgrcRCW) + 0x8), ((dxLabel + dy) + 0x6), yTop, (((dxLabel + dxMiddle) - dy) + 0xfffa), (yTop + dy));
    SetRect(((uint8_t *)(vrgrcRCW) + 0x10), ((dxLabel + dxMiddle) - dy), yTop, (dxLabel + dxMiddle), (yTop + dy));
    SetRect(((uint8_t *)(vrgrcRCW) + 0x18), dxLabel, ((yTop + dy) + 0x4), (LOWORD((3 * dy)) + dxLabel), (((dy * 2) + yTop) + 0x4));
    SetRect(((uint8_t *)(vrgrcRCW) + 0x20), ((dxLabel + dxMiddle) - LOWORD((3 * dy))), ((yTop + dy) + 0x4), (dxLabel + dxMiddle), (((dy * 2) + yTop) + 0x4));
    i = 0;
    goto L_12e0;

L_1212:
    vrgrcRCW[(i + 5)] = vrgrcRCW[i];
    OffsetRect(((uint8_t *)(vrgrcRCW) + ((i + 5) * 0x8)), 0x0, LOWORD((3 * dy)));
    vrgrcRCW[(i + 10)] = vrgrcRCW[i];
    OffsetRect(((uint8_t *)(vrgrcRCW) + ((i + 10) * 0x8)), 0x0, LOWORD((6 * dy)));
    i = (i + 1);

L_12e0:
    if ((i < 5))
        goto L_1212;
    else
        goto L_12e9;

L_12e9:
    i = 0;
    goto L_1375;

L_12f1:
    SetWindowPos(GetDlgItem(hwnd, (i + 291)), 0x0, ((LOWORD((3 * dy)) + dxLabel) + 0x6), vrgrcRCW[(LOWORD((5 * i)) + 3)].top,
                 ((dxMiddle - LOWORD((6 * dy))) + 0xfff4), dy, SWP_NOZORDER);
    if (((uint16_t)(vplr.rgEnvVarMax[i]) >= 0))
        goto L_1368;
    else
        goto L_1362;

L_1362:
    t_merge_136b_0001 = 0x1;
    goto L_136b;

L_1368:
    t_merge_136b_0001 = 0x0;

L_136b:
    CheckDlgButton(hwnd, (i + 291), t_merge_136b_0001);
    i = (i + 1);

L_1375:
    if ((i < 3))
        goto L_12f1;
    else
        goto L_137e;

L_137e:
    cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
    /* untranslated: ss:[bp-0x30] = loword(GetTextExtent(hdc, "15%", 0x3)) */
    /* untranslated: vrgrcRCW[15].left = (((dxLabel + loword(GetTextExtent(hdc, szWork, cch))) + ss:[bp-0x30]) + 4) */
    *(vrgrcRCW + 0x7a) = ((LOWORD((0x9 * dy)) + yTop) + 0xfffd);
    *(vrgrcRCW + 0x7c) = (vrgrcRCW[0xf].left + 15);
    *(vrgrcRCW + 0x7e) = (((dyArial8 >> 0x1) + *(vrgrcRCW + 0x7a)) + 0x3);
    vrgrcRCW[16] = vrgrcRCW[15];
    OffsetRect(((uint8_t *)(vrgrcRCW) + 0x80), 0x0, ((*(vrgrcRCW + 0x7e) - *(vrgrcRCW + 0x7a)) + 0xffff));
    LOWORD(crcRCW) = 0x11;
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    if ((fRCWReadOnly == 0))
        goto L_14a7;
    else
        goto L_147c;

L_147c:
    i = 291;
    goto L_149d;

L_1484:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 1);

L_149d:
    if ((i <= 293))
        goto L_1484;
    else
        goto L_14a7;

L_14a7:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    return 0x1;

L_14c0:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_14e8:
    i = 291;
    goto L_1515;

L_14f0:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_151f : L_150b */

L_150b:

L_1511:
    i = (i + 1);

L_1515:
    if ((i <= 293))
        goto L_14f0;
    else
        goto L_151f;

L_151f:
    if ((i <= 293))
        goto L_1545;
    else
        goto L_1529;

L_1529:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_178c;
    else
        goto L_1545;

L_1545:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_155e:
    hdc = BeginPaint(hwnd, &(ps));
    viStore = -2;
    DrawRace2(hwnd, hdc, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_159d:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((IrcRaceDlgHitTest(pt) < 0))
        goto L_178c;
    else
        goto L_15ce;

L_15ce:
    SetCursor(hcurHand);
    return 0x1;

L_15e0:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    return FTrackRaceDlg2(hwnd, pt, wParam);

L_1618:
    if ((wParam != 0x76))
        goto L_1642;
    else
        goto L_1621;

L_1621:
    WinHelp(hwnd, szHelpFile, 0x1, 0x41d);
    return 0x1;

L_1642:
    i = 0;
    goto L_1662;

L_164a:
    if ((wParam == rgidRaceBtn[i]))
        goto L_166b;
    else
        goto L_1658;

L_1658:

L_165e:
    i = (i + 1);

L_1662:
    if ((i < 4))
        goto L_164a;
    else
        goto L_166b;

L_166b:
    if ((i >= 4))
        goto L_1698;
    else
        goto L_1674;

L_1674:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_1698:
    if ((wParam < 0x123))
        goto L_178c;
    else
        goto L_16a2;

L_16a2:
    if ((wParam > 0x125))
        goto L_178c;
    else
        goto L_16ac;

L_16ac:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), WM_USER, 0x0, 0x0));
    iVar = (wParam - 291);
    if ((i != 1))
        goto L_1707;
    else
        goto L_16e3;

L_16e3:
    vplr.rgEnvVar[iVar] = -1;
    vplr.rgEnvVarMax[iVar] = -1;
    vplr.rgEnvVarMin[iVar] = -1;
    goto L_1728;

L_1707:
    vplr.rgEnvVarMin[iVar] = 20;
    vplr.rgEnvVarMax[iVar] = 80;
    vplr.rgEnvVar[iVar] = 50;

L_1728:
    DrawRace2(hwnd, 0x0, ((0x1 << iVar) | 0xff00));

L_1749:
    if ((message == WM_PAINT))
        goto L_155e;
    else
        goto L_1751;

L_1751:
    if ((message == WM_ERASEBKGND))
        goto L_14c0;
    else
        goto L_1759;

L_1759:
    if ((message == WM_CTLCOLOR))
        goto L_14e8;
    else
        goto L_1761;

L_1761:
    if ((message == WM_SETCURSOR))
        goto L_159d;
    else
        goto L_1769;

L_1769:
    if ((message == WM_INITDIALOG))
        goto L_106f;
    else
        goto L_1771;

L_1771:
    if ((message == WM_COMMAND))
        goto L_1618;
    else
        goto L_1779;

L_1779:
    if ((message == WM_LBUTTONDOWN))
        goto L_15e0;
    else
        goto L_1781;

L_1781:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_15e0;
    else
        goto L_178c;

L_178c:
    return 0x0;
}

void DrawRace2(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  iPit;
    int16_t  bt;
    int16_t  iMax;
    char     szT[32];
    int16_t  dy;
    int16_t  iMin;
    int16_t  bkMode;
    int16_t  fCreatedDC;
    int16_t  xRLabel;
    int16_t  i;
    int16_t  iMod;
    char    *psz;
    int16_t  dx;
    int16_t  cch;
    int16_t  bt1;
    RECT     rc;
    int32_t  l2;
    int16_t  iStore;
    int32_t  l;
    uint16_t t_merge_17bb_0001;
    uint16_t t_merge_1ae5_0001;
    uint16_t t_merge_1b22_0001;
    char    *t_merge_1b70_0001;
    uint16_t t_1f04;
    uint16_t t_merge_207b_0001;

L_179a:
    fCreatedDC = 0;
    if ((fRCWReadOnly == 0))
        goto L_17b8;
    else
        goto L_17b2;

L_17b2:
    t_merge_17bb_0001 = 0x4;
    goto L_17bb;

L_17b8:
    t_merge_17bb_0001 = 0x0;

L_17bb:
    bt1 = t_merge_17bb_0001;
    if ((hdc != 0x0))
        goto L_17d7;
    else
        goto L_17c7;

L_17c7:
    fCreatedDC = 1;
    hdc = GetDC(hwnd);

L_17d7:
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), 0x0);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    xRLabel = (((uint32_t)((rc.right - *(vrgrcRCW + 0x14))) / 2) + *(vrgrcRCW + 0x14));
    i = 0;
    goto L_1a84;

L_1836:
    if ((((0x1 << i) & iDraw) == 0x0))
        goto L_1a80;
    else
        goto L_1846;

L_1846:

L_184c:
    if ((iDraw == -1))
        goto L_18d2;
    else
        goto L_1855;

L_1855:
    SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, *(vrgrcRCW + 0x14), vrgrcRCW[(LOWORD((5 * i)) + 2)].top, (rc.right - *(vrgrcRCW + 0x14)),
           (vrgrcRCW[(LOWORD((5 * i)) + 4)].bottom - vrgrcRCW[(LOWORD((5 * i)) + 2)].top), PATCOPY);

L_18d2:
    if (((uint16_t)(vplr.rgEnvVarMax[i]) >= 0))
        goto L_1930;
    else
        goto L_18e5;

L_18e5:
    cch = CchGetString(idsN2, szT);
    CtrTextOut(hdc, xRLabel, ((vrgrcRCW[(LOWORD((5 * i)) + 2)].top + 1) + dyArial8), szT, cch);
    goto L_1a1b;

L_1930:
    psz = PszCalcEnvVar(i, (uint16_t)(vplr.rgEnvVarMin[i]));
    CtrTextOut(hdc, xRLabel, (vrgrcRCW[(LOWORD((5 * i)) + 2)].top + 1), psz, 0x0);
    cch = CchGetString(idsTo2, szT);
    CtrTextOut(hdc, xRLabel, ((vrgrcRCW[(LOWORD((5 * i)) + 2)].top + 1) + dyArial8), szT, cch);
    psz = PszCalcEnvVar(i, (uint16_t)(vplr.rgEnvVarMax[i]));
    CtrTextOut(hdc, xRLabel, ((vrgrcRCW[(LOWORD((5 * i)) + 2)].top + 1) + (dyArial8 * 2)), psz, 0x0);

L_1a1b:
    if ((iDraw != -1))
        goto L_1a80;
    else
        goto L_1a21;

L_1a21:

L_1a27:
    RightTextOut(hdc, (vrgrcRCW[LOWORD((5 * i))].left - 4), (((uint32_t)(dyArial8) / 4) + vrgrcRCW[LOWORD((5 * i))].top), rgszPlanetAttr[i], 0x0, 0x0);

L_1a80:
    i = (i + 1);

L_1a84:
    if ((i < 3))
        goto L_1836;
    else
        goto L_1a8d;

L_1a8d:
    iPit = 0;
    iMod = 0;
    i = 0;
    goto L_1aac;

L_1a9e:
    i = (i + 1);
    iMod = (iMod + 1);

L_1aac:
    if ((i >= 15))
        goto L_1d1f;
    else
        goto L_1ab5;

L_1ab5:
    if ((iMod < 5))
        goto L_1ac3;
    else
        goto L_1abe;

L_1abe:
    iMod = 0;

L_1ac3:
    if (((uint16_t)(vplr.rgEnvVarMax[((uint32_t)(i) / 5)]) >= 0))
        goto L_1ae2;
    else
        goto L_1adc;

L_1adc:
    t_merge_1ae5_0001 = 0x4;
    goto L_1ae5;

L_1ae2:
    t_merge_1ae5_0001 = 0x0;

L_1ae5:
    bt = t_merge_1ae5_0001;
    if ((iMod == 0))
        goto L_1afa;
    else
        goto L_1af1;

L_1af1:
    if ((iMod != 2))
        goto L_1b47;
    else
        goto L_1afa;

L_1afa:
    if (((iDraw & 0xfff0) == 0x0))
        goto L_1a9e;
    else
        goto L_1b08;

L_1b08:
    if ((iMod == 0))
        goto L_1b1f;
    else
        goto L_1b19;

L_1b19:
    t_merge_1b22_0001 = 0x3;
    goto L_1b22;

L_1b1f:
    t_merge_1b22_0001 = 0x2;

L_1b22:
    DrawBtn(hdc, &(vrgrcRCW[i]), ((t_merge_1b22_0001 | bt) | bt1), 0x0, 0x0);

L_1b47:
    if ((iMod == 1))
        goto L_1b9d;
    else
        goto L_1b50;

L_1b50:
    if (((iDraw & 0xfff0) == 0x0))
        goto L_1a9e;
    else
        goto L_1b5e;

L_1b5e:
    if ((iMod != 3))
        goto L_1b6d;
    else
        goto L_1b67;

L_1b67:
    t_merge_1b70_0001 = "<<     >>";
    goto L_1b70;

L_1b6d:
    t_merge_1b70_0001 = ">>     <<";

L_1b70:
    DrawBtn(hdc, &(vrgrcRCW[i]), ((0x8 | bt) | bt1), 0x0, t_merge_1b70_0001);

L_1b9d:
    if ((((0x1 << iPit) & iDraw) == 0x0))
        goto L_1d18;
    else
        goto L_1bb0;

L_1bb0:
    PatBlt(hdc, vrgrcRCW[i].left, vrgrcRCW[i].top, (vrgrcRCW[i].right - vrgrcRCW[i].left), (vrgrcRCW[i].bottom - vrgrcRCW[i].top), BLACKNESS);
    if ((bt != 0))
        goto L_1d18;
    else
        goto L_1c35;

L_1c35:
    iMin = (uint16_t)(vplr.rgEnvVarMin[iPit]);
    iMax = (uint16_t)(vplr.rgEnvVarMax[iPit]);
    dx = ((vrgrcRCW[i].right - vrgrcRCW[i].left) - 2);
    dy = ((vrgrcRCW[i].bottom - vrgrcRCW[i].top) - 2);
    SelectObject(hdc, rghbrPlanetAttr[iPit][0]);
    PatBlt(hdc, ((MulDiv(iMin, dx, 0x64) + vrgrcRCW[i].left) + 0x1), (vrgrcRCW[i].top + 1), MulDiv((iMax - iMin), dx, 0x64), dy, PATCOPY);

L_1d18:
    iPit = (iPit + 1);

L_1d1f:
    if (((iDraw & 0x8) == 0x0))
        goto L_1e53;
    else
        goto L_1d2d;

L_1d2d:
    if ((iDraw != -1))
        goto L_1db7;
    else
        goto L_1d36;

L_1d36:
    cch = CchGetString(idsMaximumColonistGrowthRatePerYear, szWork);
    TextOut(hdc, vrgrcRCW->left, (*(vrgrcRCW + 0x7a) + 0x3), szWork, cch);
    DrawBtn(hdc, ((uint8_t *)(vrgrcRCW) + 0x78), (0xa0 | bt1), 0x0, 0x0);
    DrawBtn(hdc, ((uint8_t *)(vrgrcRCW) + 0x80), (0xa1 | bt1), 0x0, 0x0);
    goto L_1e09;

L_1db7:
    dx = LOWORD(GetTextExtent(hdc, "15%", 0x3));
    SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, ((vrgrcRCW[15].left - 4) - dx), (*(vrgrcRCW + 0x7a) + 0x3), dx, dyArial8, PATCOPY);

L_1e09:
    /* untranslated: cch = _wsprintf(szWork, PCTDPCTPCT, sext8to16(part[25:2](vplr))) */
    RightTextOut(hdc, (vrgrcRCW[15].left - 4), (*(vrgrcRCW + 0x7a) + 0x3), szWork, cch, 0x0);

L_1e53:
    if (((iDraw & 0x7) == 0x0))
        goto L_213f;
    else
        goto L_1e61;

L_1e61:
    l = 1;
    i = 0;
    goto L_1fb6;

L_1e75:
    if (((uint16_t)(vplr.rgEnvVarMax[i]) < 0))
        goto L_1eb2;
    else
        goto L_1e88;

L_1e88:
    if ((((uint16_t)(vplr.rgEnvVarMax[i]) - (uint16_t)(vplr.rgEnvVarMin[i])) != 0x64))
        goto L_1ed2;
    else
        goto L_1eb2;

L_1eb2:
    l = (uint32_t)((l * 100));
    goto L_1fb2;

L_1ed2:
    if ((i != 2))
        goto L_1f18;
    else
        goto L_1edb;

L_1edb:
    /* untranslated: ss:[bp-0xb8] = sext8to16(vplr.rgEnvVarMin[i]) */
    t_1f04 = LOWORD(l);
    /* untranslated: LOWORD(l) = loword((uint32_t)(l * words(signhiword((sext8to16(vplr.rgEnvVarMax[i]) - ss:[bp-0xb8])), (sext8to16(vplr.rgEnvVarMax[i]) -
     * ss:[bp-0xb8])))) */
    /* untranslated: HIWORD(l) = hiword((uint32_t)(words(HIWORD(l), t_1f04) * words(signhiword((sext8to16(vplr.rgEnvVarMax[i]) - ss:[bp-0xb8])),
     * (sext8to16(vplr.rgEnvVarMax[i]) - ss:[bp-0xb8])))) */
    goto L_1fb2;

L_1f18:
    l2 = 0;
    iStore = (uint16_t)(vplr.rgEnvVarMin[i]);

L_1f31:
    if ((iStore > (uint16_t)(vplr.rgEnvVarMax[i])))
        goto L_1f88;
    else
        goto L_1f45;

L_1f45:
    if ((iStore >= 10))
        goto L_1f5d;
    else
        goto L_1f4f;

L_1f4f:
    l2 = (l2 + (uint32_t)(iStore));
    goto L_1f80;

L_1f5d:
    if ((iStore >= 90))
        goto L_1f72;
    else
        goto L_1f67;

L_1f67:
    l2 = (l2 + 10);
    goto L_1f80;

L_1f72:
    l2 = (l2 + (uint32_t)((0x64 - iStore)));

L_1f80:
    iStore = (iStore + 1);
    goto L_1f31;

L_1f88:
    l = (int32_t)(((uint32_t)((l * l2)) / 0x9));

L_1fb2:
    i = (i + 1);

L_1fb6:
    if ((i < 3))
        goto L_1e75;
    else
        goto L_1fbf;

L_1fbf:
    if ((HIWORD(l) > 0x0))
        goto L_1fe4;
    else
        goto L_1fc9;

L_1fc9:
    if ((HIWORD(l) < 0x0))
        goto L_1fd8;
    else
        goto L_1fce;

L_1fce:
    if ((LOWORD(l) >= 0x1))
        goto L_1fe4;
    else
        goto L_1fd8;

L_1fd8:
    l = 1;

L_1fe4:
    /* untranslated: l2 = (int32_t)(words((hiword((int32_t)(l >> 0x1)) + 0xf), (loword((int32_t)(l >> 0x1)) + 0x4240)) / l) */
    iStore = LOWORD(l2);
    if ((LOWORD(l) != 0x4240))
        goto L_2034;
    else
        goto L_2024;

L_2024:
    if ((HIWORD(l) != 0xf))
        goto L_2034;
    else
        goto L_202e;

L_202e:
    iStore = 0;

L_2034:
    if ((iStore == viStore))
        goto L_213f;
    else
        goto L_2040;

L_2040:
    viStore = iStore;
    if ((LOWORD(l2) != 0x1))
        goto L_208b;
    else
        goto L_2050;

L_2050:
    if ((HIWORD(l2) != 0x0))
        goto L_208b;
    else
        goto L_2059;

L_2059:
    if ((LOWORD(l) != 0x4240))
        goto L_2078;
    else
        goto L_2068;

L_2068:
    if ((HIWORD(l) != 0xf))
        goto L_2078;
    else
        goto L_2072;

L_2072:
    t_merge_207b_0001 = 0x188;
    goto L_207b;

L_2078:
    t_merge_207b_0001 = 0x189;

L_207b:
    cch = CchGetString(t_merge_207b_0001, szWork);
    goto L_20bd;

L_208b:
    CchGetString(idsCanExpect1DPlanetsWillHabitable, szT);
    cch = _wsprintf(szWork, szT, LOWORD(l2), HIWORD(l2));

L_20bd:
    rc.left = vrgrcRCW->left;
    rc.top = ((*(vrgrcRCW + 0x7a) + dyArial8) + 8);
    rc.right = (*(vrgrcRCW + 0x7c) + 20);
    rc.bottom = (LOWORD((0x3 * dyArial8)) + rc.top);
    SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), PATCOPY);
    DrawText(hdc, szWork, cch, &(rc), 0x10);

L_213f:
    SetBkMode(hdc, bkMode);
    if ((fCreatedDC == 0))
        goto L_215e;
    else
        goto L_2153;

L_2153:
    ReleaseDC(hwnd, hdc);

L_215e:
    return;
}

int16_t IrcRaceDlgHitTest(POINT pt) {
    int16_t i;

L_2164:
    if ((fRCWReadOnly == 0))
        goto L_217d;
    else
        goto L_2177;

L_2177:
    return 0xffff;

L_217d:
    i = 0;
    goto L_21b2;

L_2185:
    if ((PtInRect(((uint8_t *)(vrgrcRCW) + (i * 8)), pt) != 0))
        goto L_21bd;
    else
        goto L_21a8;

L_21a8:

L_21ae:
    i = (i + 1);

L_21b2:
    if ((i < LOWORD(crcRCW)))
        goto L_2185;
    else
        goto L_21bd;

L_21bd:
    if ((i >= LOWORD(crcRCW)))
        goto L_21f7;
    else
        goto L_21c8;

L_21c8:
    if ((iPanelActive != 4))
        goto L_21f1;
    else
        goto L_21d2;

L_21d2:
    if (((uint16_t)(vplr.rgEnvVarMax[((uint32_t)(i) / 5)]) >= 0))
        goto L_21f1;
    else
        goto L_21eb;

L_21eb:
    return 0xffff;

L_21f1:
    return i;

L_21f7:
    return 0xffff;
}

int16_t FTrackRaceDlg2(HWND hwnd, POINT pt, int16_t kbd) {
    BTNT     btnt;
    int16_t  bt;
    int16_t  dShift;
    char     iMax;
    char     iMin;
    int16_t  i;
    int16_t  irc;
    int16_t  iMod;
    char    *psz;
    int16_t  dWidth;
    int16_t  dx;
    int16_t  t_merge_2360_0001;
    int16_t  t_merge_2388_0001;
    uint16_t t_merge_255f_0001;
    uint16_t t_merge_2587_0001;
    uint16_t t_merge_264c_0001;

L_2204:
    irc = IrcRaceDlgHitTest(pt);
    if ((irc >= 0))
        goto L_222d;
    else
        goto L_2227;

L_2227:
    return 0x0;

L_222d:
    iMod = ((uint32_t)(irc) % 5);
    i = ((uint32_t)(irc) / 5);
    if ((iMod != 1))
        goto L_242e;
    else
        goto L_224e;

L_224e:
    if ((irc >= 15))
        goto L_242e;
    else
        goto L_2257;

L_2257:
    SetCapture(hwnd);
    SetCursor(hcurCloseGrab);
    dWidth = ((uint32_t)(((uint16_t)(vplr.rgEnvVarMax[i]) - (uint16_t)(vplr.rgEnvVarMin[i]))) / 2);

L_2291:
    if ((FGetMouseMove(&(pt)) == 0))
        goto L_2423;
    else
        goto L_22a5;

L_22a5:
    if ((pt.x >= vrgrcRCW[irc].left))
        goto L_22d2;
    else
        goto L_22be;

L_22be:
    pt.x = vrgrcRCW[irc].left;

L_22d2:
    if ((pt.x <= vrgrcRCW[irc].right))
        goto L_2301;
    else
        goto L_22ec;

L_22ec:
    pt.x = vrgrcRCW[irc].right;

L_2301:
    dShift = MulDiv((pt.x - vrgrcRCW[irc].left), 0x64, (vrgrcRCW[irc].right - vrgrcRCW[irc].left));
    if ((dShift >= (100 - dWidth)))
        goto L_235a;
    else
        goto L_2354;

L_2354:
    t_merge_2360_0001 = dShift;
    goto L_2360;

L_235a:
    t_merge_2360_0001 = (100 - dWidth);

L_2360:
    if ((dWidth <= t_merge_2360_0001))
        goto L_236e;
    else
        goto L_2368;

L_2368:
    t_merge_2388_0001 = dWidth;
    goto L_2388;

L_236e:
    if ((dShift >= (100 - dWidth)))
        goto L_2382;
    else
        goto L_237c;

L_237c:
    t_merge_2388_0001 = dShift;
    goto L_2388;

L_2382:
    t_merge_2388_0001 = (100 - dWidth);

L_2388:
    dShift = t_merge_2388_0001;
    if (((uint16_t)(vplr.rgEnvVarMax[i]) == (dShift + dWidth)))
        goto L_2291;
    else
        goto L_23a3;

L_23a3:
    vplr.rgEnvVarMin[i] = LOBYTE((dShift - dWidth));
    vplr.rgEnvVarMax[i] = LOBYTE((dShift + dWidth));
    vplr.rgEnvVar[i] = LOBYTE(((uint16_t)(vplr.rgEnvVarMin[i]) + ((uint32_t)(((uint16_t)(vplr.rgEnvVarMax[i]) - (uint16_t)(vplr.rgEnvVarMin[i]))) / 0x2)));
    DrawRace2(hwnd, 0x0, (0x1 << i));

L_2423:
    ReleaseCapture();
    return 0x1;

L_242e:
    dWidth = 0;
    dShift = 0;
    psz = 0x0;
    if ((irc != 15))
        goto L_2454;
    else
        goto L_2447;

L_2447:
    dShift = 1;
    bt = 160;
    goto L_24c2;

L_2454:
    if ((irc != 16))
        goto L_246a;
    else
        goto L_245d;

L_245d:
    dShift = -1;
    bt = 161;
    goto L_24c2;

L_246a:
    if ((iMod != 0))
        goto L_2480;
    else
        goto L_2473;

L_2473:
    dShift = -1;
    bt = 2;
    goto L_24c2;

L_2480:
    if ((iMod != 2))
        goto L_2496;
    else
        goto L_2489;

L_2489:
    dShift = 1;
    bt = 3;
    goto L_24c2;

L_2496:
    if ((iMod != 3))
        goto L_24b2;
    else
        goto L_249f;

L_249f:
    dWidth = 1;
    bt = 8;
    psz = vrgszRCWWidth[0];
    goto L_24c2;

L_24b2:
    dWidth = -1;
    bt = 8;
    psz = vrgszRCWWidth[1];

L_24c2:
    InitBtnTrack(&(btnt), hwnd, 0x0, &(vrgrcRCW[irc]), bt, 0x50, 0x0, 0x0, psz);
    if (((kbd & 0x4) == 0x0))
        goto L_2517;
    else
        goto L_2505;

L_2505:
    dWidth = LOWORD((0xa * dWidth));
    dShift = LOWORD((0xa * dShift));

L_2517:
    if ((FTrackBtn(&(btnt)) == 0))
        goto L_2737;
    else
        goto L_252b;

L_252b:
    if ((irc == 15))
        goto L_253d;
    else
        goto L_2534;

L_2534:
    if ((irc != 16))
        goto L_25ba;
    else
        goto L_253d;

L_253d:
    /* untranslated: iMin = lobyte((sext8to16(part[25:2](vplr)) + dShift)) */
    if ((1 <= (uint16_t)(iMin)))
        goto L_255b;
    else
        goto L_2555;

L_2555:
    t_merge_255f_0001 = 0x1;
    goto L_255f;

L_255b:
    t_merge_255f_0001 = (uint16_t)(iMin);

L_255f:
    if ((0x14 >= t_merge_255f_0001))
        goto L_256f;
    else
        goto L_2569;

L_2569:
    t_merge_2587_0001 = 0x14;
    goto L_2587;

L_256f:
    if ((1 <= (uint16_t)(iMin)))
        goto L_2583;
    else
        goto L_257d;

L_257d:
    t_merge_2587_0001 = 0x1;
    goto L_2587;

L_2583:
    t_merge_2587_0001 = (uint16_t)(iMin);

L_2587:
    iMin = LOBYTE(t_merge_2587_0001);
    /* untranslated: branch sext8to16(iMin) == sext8to16(part[25:2](vplr)) ? L_2517 : L_259f */

L_259f:
    /* untranslated: part[25:2](vplr) = iMin */
    DrawRace2(hwnd, btnt.hdc, 0x8);

L_25ba:
    iMin = LOBYTE(((uint16_t)(vplr.rgEnvVarMin[i]) - (dWidth - dShift)));
    iMax = LOBYTE(((uint16_t)(vplr.rgEnvVarMax[i]) + (dWidth + dShift)));
    if (((uint16_t)(iMax) <= 100))
        goto L_2610;
    else
        goto L_2602;

L_2602:
    iMin = (iMin - LOBYTE(((uint16_t)(iMax) + 0xff9c)));
    iMax = 100;

L_2610:
    if (((uint16_t)(iMin) >= 0))
        goto L_2653;
    else
        goto L_261c;

L_261c:
    if ((0x64 >= ((uint16_t)(iMax) - (uint16_t)(iMin))))
        goto L_263c;
    else
        goto L_2636;

L_2636:
    t_merge_264c_0001 = 0x64;
    goto L_264c;

L_263c:
    /* untranslated: ss:[bp-0x32] = sext8to16(iMin) */
    /* untranslated: t_merge_264c_0001 = (sext8to16(iMax) - ss:[bp-0x32]) */

L_264c:
    iMax = LOBYTE(t_merge_264c_0001);
    iMin = 0;

L_2653:
    dx = ((uint16_t)(iMax) - (uint16_t)(iMin));
    if ((dx >= 20))
        goto L_2686;
    else
        goto L_266f;

L_266f:
    dx = ((0x14 - dx) >> 0x1);
    iMin = (iMin - LOBYTE(dx));
    iMax = (iMax + LOBYTE(dx));

L_2686:
    if (((uint16_t)(vplr.rgEnvVarMin[i]) != (uint16_t)(iMin)))
        goto L_26be;
    else
        goto L_26a2;

L_26a2:
    if (((uint16_t)(vplr.rgEnvVarMax[i]) == (uint16_t)(iMax)))
        goto L_2517;
    else
        goto L_26be;

L_26be:
    vplr.rgEnvVarMin[i] = iMin;
    vplr.rgEnvVarMax[i] = iMax;
    vplr.rgEnvVar[i] = LOBYTE(((uint16_t)(vplr.rgEnvVarMin[i]) + ((uint32_t)(((uint16_t)(vplr.rgEnvVarMax[i]) - (uint16_t)(vplr.rgEnvVarMin[i]))) / 0x2)));
    DrawRace2(hwnd, btnt.hdc, (0x1 << i));

L_2737:
    if ((irc >= 15))
        goto L_2785;
    else
        goto L_2740;

L_2740:
    vplr.rgEnvVar[i] = LOBYTE(((uint16_t)(vplr.rgEnvVarMin[i]) + ((uint32_t)(((uint16_t)(vplr.rgEnvVarMax[i]) - (uint16_t)(vplr.rgEnvVarMin[i]))) / 0x2)));

L_2785:
    return 0x1;
}

int16_t RaceWizardDlg3(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    POINT       pt;
    HDC         hdc;
    PAINTSTRUCT ps;

L_2792:
    goto L_2a3b;

L_27a1:
    SetRCWTitle(hwnd, iPanelActive);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), CB_LIMITTEXT, GetRaceGrbit(&(vplr), ibitRaceCheapFact), 0x0);
    if ((fRCWReadOnly != 0))
        goto L_2801;
    else
        goto L_27e9;

L_27e9:
    if ((GetRaceStat(&(vplr), rsMajorAdv) != raMacintosh))
        goto L_2817;
    else
        goto L_2801;

L_2801:
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0123), 0x0);

L_2817:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    return 0x1;

L_2830:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_2858:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, IDC_U16_0x0123) ? L_2893 : L_2877 */

L_2877:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_2a7e;
    else
        goto L_2893;

L_2893:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_28ac:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((IrcRaceDlgHitTest(pt) < 0))
        goto L_2a7e;
    else
        goto L_28dd;

L_28dd:
    SetCursor(hcurHand);
    return 0x1;

L_28ef:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    return FTrackRaceDlg3(hwnd, pt, wParam);

L_2927:
    hdc = BeginPaint(hwnd, &(ps));
    DrawRace3(hwnd, hdc, 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_2960:
    if ((wParam != 0x76))
        goto L_298a;
    else
        goto L_2969;

L_2969:
    WinHelp(hwnd, szHelpFile, 0x1, 0x420);
    return 0x1;

L_298a:
    i = 0;
    goto L_29aa;

L_2992:
    if ((wParam == rgidRaceBtn[i]))
        goto L_29b3;
    else
        goto L_29a0;

L_29a0:

L_29a6:
    i = (i + 1);

L_29aa:
    if ((i < 4))
        goto L_2992;
    else
        goto L_29b3;

L_29b3:
    if ((i >= 4))
        goto L_29e0;
    else
        goto L_29bc;

L_29bc:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_29e0:
    if ((wParam != 0x123))
        goto L_2a7e;
    else
        goto L_29ea;

L_29ea:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), WM_USER, 0x0, 0x0));
    SetRaceGrbit(&(vplr), ibitRaceCheapFact, i);
    DrawRace3(hwnd, 0x0, 0x63);

L_2a3b:
    if ((message == WM_PAINT))
        goto L_2927;
    else
        goto L_2a43;

L_2a43:
    if ((message == WM_ERASEBKGND))
        goto L_2830;
    else
        goto L_2a4b;

L_2a4b:
    if ((message == WM_CTLCOLOR))
        goto L_2858;
    else
        goto L_2a53;

L_2a53:
    if ((message == WM_SETCURSOR))
        goto L_28ac;
    else
        goto L_2a5b;

L_2a5b:
    if ((message == WM_INITDIALOG))
        goto L_27a1;
    else
        goto L_2a63;

L_2a63:
    if ((message == WM_COMMAND))
        goto L_2960;
    else
        goto L_2a6b;

L_2a6b:
    if ((message == WM_LBUTTONDOWN))
        goto L_28ef;
    else
        goto L_2a73;

L_2a73:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_28ef;
    else
        goto L_2a7e;

L_2a7e:
    return 0x0;
}

void DrawRace3(HWND hwnd, HDC hdc, int16_t iDraw) {
    int16_t  dxItem;
    int16_t  idsT;
    int16_t  fMacintosh;
    int16_t  yTop;
    int16_t  bt;
    StringId ids;
    COLORREF crBkSav;
    int16_t  bkMode;
    int16_t  fCreatedDC;
    int16_t  dxkT;
    int16_t  i;
    int16_t  irc;
    int16_t  dxDig;
    int16_t  dx;
    int16_t  cch;
    RECT     rc;
    uint16_t t_merge_2ac5_0001;
    uint16_t t_merge_2ae9_0001;

L_2aa4:
    fCreatedDC = 0;
    if ((fRCWReadOnly == 0))
        goto L_2ac2;
    else
        goto L_2abc;

L_2abc:
    t_merge_2ac5_0001 = 0x4;
    goto L_2ac5;

L_2ac2:
    t_merge_2ac5_0001 = 0x0;

L_2ac5:
    bt = t_merge_2ac5_0001;
    if ((GetRaceStat(&(vplr), rsMajorAdv) != raMacintosh))
        goto L_2ae6;
    else
        goto L_2ae0;

L_2ae0:
    t_merge_2ae9_0001 = 0x1;
    goto L_2ae9;

L_2ae6:
    t_merge_2ae9_0001 = 0x0;

L_2ae9:
    fMacintosh = t_merge_2ae9_0001;
    if ((hdc != 0x0))
        goto L_2b05;
    else
        goto L_2af5;

L_2af5:
    fCreatedDC = 1;
    hdc = GetDC(hwnd);

L_2b05:
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), 0x0);
    bkMode = SetBkMode(hdc, OPAQUE);
    crBkSav = SetBkColor(hdc, crButtonFace);
    SetTextColor(hdc, crWindowText);
    SelectObject(hdc, rghfontArial8[1]);
    yTop = (LOWORD((0x3 * dyArial8)) + 6);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 0x1));
    dxkT = LOWORD(GetTextExtent(hdc, "kT", 0x2));
    ids = idsOneResourceGeneratedEachYearEvery;
    irc = 0;
    i = 0;
    goto L_2f63;

L_2bb3:
    if ((i != 1))
        goto L_2bda;
    else
        goto L_2bbc;

L_2bbc:
    if ((fMacintosh == 0))
        goto L_2bda;
    else
        goto L_2bc5;

L_2bc5:
    SetTextColor(hdc, crButtonShadow);
    bt = 4;

L_2bda:
    if ((i != 4))
        goto L_2c32;
    else
        goto L_2be3;

L_2be3:
    if ((iDraw != -1))
        goto L_2c22;
    else
        goto L_2bec;

L_2bec:
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0123), 0x0, 0x6, yTop, (rc.right - 12), (words(loword((3 * dyArial8)), signhiword(loword((3 *
     * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */

L_2c22:
    /* untranslated: yTop = (yTop + (words(loword((0x5 * dyArial8)), signhiword(loword((0x5 * dyArial8)))) / 2)) */

L_2c32:
    if ((fMacintosh == 0))
        goto L_2c4d;
    else
        goto L_2c3b;

L_2c3b:
    if ((ids != idsOneResourceGeneratedEachYearEvery))
        goto L_2c4d;
    else
        goto L_2c45;

L_2c45:
    idsT = 260;
    goto L_2c53;

L_2c4d:
    idsT = ids;

L_2c53:
    ids = (ids + 1);
    cch = CchGetString(idsT, szWork);
    if ((iDraw != -1))
        goto L_2c8b;
    else
        goto L_2c72;

L_2c72:
    TextOut(hdc, 0x6, yTop, szWork, cch);

L_2c8b:
    dx = (LOWORD(GetTextExtent(hdc, szWork, cch)) + 6);
    dxItem = LOWORD((abs((uint16_t)(rgRW3Width[i])) * dxDig));
    _wsprintf(szWork, PCTD, GetRaceStat(&(vplr), (uint16_t)(rgRW3IStat[i])));
    if (((uint16_t)(rgRW3Width[i]) >= 0))
        goto L_2d3d;
    else
        goto L_2cf9;

L_2cf9:
    if ((i > 0))
        goto L_2d0b;
    else
        goto L_2d02;

L_2d02:
    if ((fMacintosh != 0))
        goto L_2d3d;
    else
        goto L_2d0b;

L_2d0b:
    dxItem = (dxItem + dxkT);
    if ((i != 0))
        goto L_2d2d;
    else
        goto L_2d1a;

L_2d1a:
    strcat(szWork, "00");
    goto L_2d3d;

L_2d2d:
    strcat(szWork, "kT");

L_2d3d:
    dx = (dx + dxItem);
    if ((iDraw == -1))
        goto L_2d57;
    else
        goto L_2d4c;

L_2d4c:
    if ((iDraw != i))
        goto L_2d73;
    else
        goto L_2d57;

L_2d57:
    RightTextOut(hdc, dx, yTop, szWork, 0x0, dxItem);

L_2d73:
    vrgrcRCW[irc].left = (dx + 4);
    vrgrcRCW[irc].top = (yTop - 3);
    vrgrcRCW[irc].right = (vrgrcRCW[irc].left + 15);
    vrgrcRCW[irc].bottom = (((dyArial8 >> 0x1) + vrgrcRCW[irc].top) + 0x3);
    vrgrcRCW[(irc + 1)] = vrgrcRCW[irc];
    OffsetRect(((uint8_t *)(vrgrcRCW) + ((irc + 1) * 0x8)), 0x0, ((vrgrcRCW[irc].bottom - vrgrcRCW[irc].top) + 0xffff));
    if ((iDraw != -1))
        goto L_2eda;
    else
        goto L_2e83;

L_2e83:
    DrawBtn(hdc, &(vrgrcRCW[irc]), (0xa0 | bt), 0x0, 0x0);
    DrawBtn(hdc, &(vrgrcRCW[(irc + 1)]), (0xa1 | bt), 0x0, 0x0);

L_2eda:
    if ((iDraw != -1))
        goto L_2f41;
    else
        goto L_2ee3;

L_2ee3:
    if ((fMacintosh == 0))
        goto L_2efe;
    else
        goto L_2eec;

L_2eec:
    if ((ids != idsColonists))
        goto L_2efe;
    else
        goto L_2ef6;

L_2ef6:
    idsT = 261;
    goto L_2f04;

L_2efe:
    idsT = ids;

L_2f04:
    cch = CchGetString(idsT, szWork);
    TextOut(hdc, (vrgrcRCW[irc].right + 4), yTop, szWork, cch);

L_2f41:
    ids = (ids + 1);
    irc = (irc + 2);
    /* untranslated: yTop = (yTop + (words(loword((sext8to16(rgRW3Spacing[i]) * dyArial8)), signhiword(loword((sext8to16(rgRW3Spacing[i]) * dyArial8)))) / 2))
     */
    i = (i + 1);

L_2f63:
    if ((i < 7))
        goto L_2bb3;
    else
        goto L_2f6c;

L_2f6c:
    if ((fMacintosh == 0))
        goto L_2f7e;
    else
        goto L_2f75;

L_2f75:
    LOWORD(crcRCW) = 0x2;
    goto L_2f84;

L_2f7e:
    LOWORD(crcRCW) = irc;

L_2f84:
    SetBkColor(hdc, crBkSav);
    SetBkMode(hdc, bkMode);
    if ((fCreatedDC == 0))
        goto L_2fb1;
    else
        goto L_2fa6;

L_2fa6:
    ReleaseDC(hwnd, hdc);

L_2fb1:
    return;
}

int16_t FTrackRaceDlg3(HWND hwnd, POINT pt, int16_t kbd) {
    BTNT    btnt;
    int16_t bt;
    int16_t dShift;
    int16_t i;
    int16_t irc;
    int16_t iMod;
    int16_t iStat;

L_2fb8:
    irc = IrcRaceDlgHitTest(pt);
    if ((irc >= 0))
        goto L_2fe1;
    else
        goto L_2fdb;

L_2fdb:
    return 0x0;

L_2fe1:
    iMod = (irc & 0x1);
    i = (irc >> 0x1);
    if ((iMod != 0))
        goto L_3008;
    else
        goto L_2ffb;

L_2ffb:
    dShift = 1;
    bt = 160;
    goto L_3012;

L_3008:
    dShift = -1;
    bt = 161;

L_3012:
    InitBtnTrack(&(btnt), hwnd, 0x0, &(vrgrcRCW[irc]), bt, 0x50, 0x0, 0x0, 0x0);
    if (((kbd & 0x4) == 0x0))
        goto L_305f;
    else
        goto L_3056;

L_3056:
    dShift = LOWORD((0x3 * dShift));

L_305f:
    if ((FTrackBtn(&(btnt)) == 0))
        goto L_30c5;
    else
        goto L_3073;

L_3073:
    iStat = GetRaceStat(&(vplr), (uint16_t)(rgRW3IStat[i]));
    if ((SetRaceStat(&(vplr), (uint16_t)(rgRW3IStat[i]), (iStat + dShift)) == iStat))
        goto L_305f;
    else
        goto L_30b1;

L_30b1:
    DrawRace3(hwnd, btnt.hdc, i);

L_30c5:
    return 0x1;
}

int16_t GetRaceStat(PLAYER *pplr, RaceStat iStat) {
L_30d2:

L_30ee:
    return (uint16_t)(pplr->rgAttr[iStat]);
}

int16_t SetRaceStat(PLAYER *pplr, RaceStat iStat, int16_t iVal) {
L_3114:
    if ((iVal >= (uint16_t)(rgRaceStatMin[iStat])))
        goto L_313a;
    else
        goto L_312e;

L_312e:
    iVal = (uint16_t)(rgRaceStatMin[iStat]);

L_313a:
    if ((iVal <= (uint16_t)(rgRaceStatMax[iStat])))
        goto L_3157;
    else
        goto L_314b;

L_314b:
    iVal = (uint16_t)(rgRaceStatMax[iStat]);

L_3157:
    pplr->rgAttr[iStat] = LOBYTE(iVal);

L_316f:
    return iVal;
}

int16_t GetRaceGrbit(PLAYER *pplr, RaceGrbit ibit) {
L_3176:
    if (((LOWORD((int32_t)((0x10000 << ibit))) & LOWORD(pplr->grbitAttr)) != 0x0))
        goto L_31a6;
    else
        goto L_319e;

L_319e:
    if (((HIWORD((int32_t)((0x10000 << ibit))) & HIWORD(pplr->grbitAttr)) == 0x0))
        goto L_31ac;
    else
        goto L_31a6;

L_31a6:
    return 0x1;

L_31ac:
    return 0x0;
}

void SetRaceGrbit(PLAYER *pplr, RaceGrbit ibit, int16_t fSet) {
    uint32_t grMask;

L_31b8:
    grMask = (int32_t)((0x10000 << ibit));
    if ((fSet == 0))
        goto L_31f0;
    else
        goto L_31de;

L_31de:
    LOWORD(pplr->grbitAttr) = (LOWORD(pplr->grbitAttr) | LOWORD(grMask));
    HIWORD(pplr->grbitAttr) = (HIWORD(pplr->grbitAttr) | HIWORD(grMask));
    goto L_3203;

L_31f0:
    /* untranslated: LOWORD(pplr->grbitAttr) = (LOWORD(pplr->grbitAttr) & (LOWORD(grMask) ~ 0x0)) */
    /* untranslated: HIWORD(pplr->grbitAttr) = (HIWORD(pplr->grbitAttr) & (HIWORD(grMask) ~ 0x0)) */

L_3203:
    return;
}

int16_t RaceWizardDlg4(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    char        szT[600];
    StringId    ids;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;

L_320a:
    goto L_3755;

L_3219:
    SetRCWTitle(hwnd, iPanelActive);
    CheckRadioButton(hwnd, 0x10f, 0x118, (GetRaceStat(&(vplr), rsMajorAdv) + 271));
    if ((fRCWReadOnly == 0))
        goto L_3281;
    else
        goto L_3256;

L_3256:
    i = 271;
    goto L_3277;

L_325e:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 1);

L_3277:
    if ((i <= 280))
        goto L_325e;
    else
        goto L_3281;

L_3281:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    return 0x1;

L_329a:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_32c2:
    i = 271;
    goto L_32ef;

L_32ca:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_32f9 : L_32e5 */

L_32e5:

L_32eb:
    i = (i + 1);

L_32ef:
    if ((i <= 280))
        goto L_32ca;
    else
        goto L_32f9;

L_32f9:
    if ((i <= 280))
        goto L_331f;
    else
        goto L_3303;

L_3303:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_3780;
    else
        goto L_331f;

L_331f:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_3338:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), 0x0);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    GetWindowRect(GetDlgItem(hwnd, IDC_RADRACE1), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0118), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), (dyArial8 + 2), (dyArial8 >> 0x1));
    rcGBox.top = (rcGBox.top - 4);
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    cch = CchGetString(idsPrimaryRacialTrait, szWork);
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    GetClientRect(hwnd, &(rc));
    rc.top = (rcGBox.bottom + 12);
    rc.left = (rc.left + 12);
    rc.right = (rc.right - 12);
    GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    rc.bottom = (rcGBox.top - 6);
    _Draw3dFrame(hdc, &(rc), 0xffff);
    cch = CchGetString(idsDescriptionTrait, szWork);
    TextOut(hdc, (rc.left + 8), (rc.top - (dyArial8 >> 0x1)), szWork, cch);
    ids = (LOWORD((GetRaceStat(&(vplr), rsMajorAdv) * 0x3)) + 0x114);
    cch = 0;
    i = 0;
    goto L_3532;

L_3522:
    i = (i + 1);
    ids = (ids + 1);

L_3532:
    if ((i >= 3))
        goto L_3559;
    else
        goto L_353b;

L_353b:
    cch = (cch + CchGetString(ids, szT[cch]));
    goto L_3522;

L_3559:
    /* untranslated: call ExpandRc(&rc, ((dyArial8 neg 0) + 0xfffe), ((dyArial8 >> 0x1) neg 0x0)) -> callresult(void) */
    rc.top = (rc.top + 4);
    DrawText(hdc, szT, cch, &(rc), 0x10);
    rcCargo = rc;
    EndPaint(hwnd, &(ps));
    return 0x1;

L_35be:
    if ((wParam != 0x76))
        goto L_35e8;
    else
        goto L_35c7;

L_35c7:
    WinHelp(hwnd, szHelpFile, 0x1, 0x408);
    return 0x1;

L_35e8:
    i = 0;
    goto L_3608;

L_35f0:
    if ((wParam == rgidRaceBtn[i]))
        goto L_3611;
    else
        goto L_35fe;

L_35fe:

L_3604:
    i = (i + 1);

L_3608:
    if ((i < 4))
        goto L_35f0;
    else
        goto L_3611;

L_3611:
    if ((i >= 4))
        goto L_3641;
    else
        goto L_361a;

L_361a:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_3641:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_3780;
    else
        goto L_365d;

L_365d:
    if ((wParam < 0x10f))
        goto L_3780;
    else
        goto L_3667;

L_3667:
    if ((wParam > 0x118))
        goto L_3780;
    else
        goto L_3671;

L_3671:
    i = (wParam - 271);
    SetRaceStat(&(vplr), rsMajorAdv, i);
    if ((GetRaceStat(&(vplr), rsMajorAdv) != raMacintosh))
        goto L_3731;
    else
        goto L_36a5;

L_36a5:
    SetRaceStat(&(vplr), rsFactProd, 0xa);
    SetRaceStat(&(vplr), rsFactBuild, 0xa);
    SetRaceStat(&(vplr), rsFactOperate, 0xa);
    SetRaceStat(&(vplr), rsMineProd, 0xa);
    SetRaceStat(&(vplr), rsMineBuild, 0x5);
    SetRaceStat(&(vplr), rsMineOperate, 0xa);
    SetRaceGrbit(&(vplr), ibitRaceCheapFact, 0x0);

L_3731:
    InvalidateAdvPtsRect(hwnd);
    InvalidateRect(hwnd, rcCargo, 0x0);

L_3755:
    if ((message == WM_PAINT))
        goto L_3338;
    else
        goto L_375d;

L_375d:
    if ((message == WM_ERASEBKGND))
        goto L_329a;
    else
        goto L_3765;

L_3765:
    if ((message == WM_CTLCOLOR))
        goto L_32c2;
    else
        goto L_376d;

L_376d:
    if ((message == WM_INITDIALOG))
        goto L_3219;
    else
        goto L_3775;

L_3775:
    if ((message == WM_COMMAND))
        goto L_35be;
    else
        goto L_3780;

L_3780:
    return 0x0;
}

int16_t RaceWizardDlg5(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HWND        hwndCtl;
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;

L_378e:
    goto L_3b75;

L_379d:
    SetRCWTitle(hwnd, iPanelActive);
    cColDrop = 0;
    i = 0;
    goto L_3825;

L_37ba:
    hwndCtl = GetDlgItem(hwnd, (i + 291));
    SetWindowText(hwndCtl, PszGetCompressedString((i + 306)));
    SendMessage(hwndCtl, CB_LIMITTEXT, GetRaceGrbit(&(vplr), i), 0x0);
    if ((fRCWReadOnly == 0))
        goto L_3821;
    else
        goto L_3815;

L_3815:
    EnableWindow(hwndCtl, 0x0);

L_3821:
    i = (i + 1);

L_3825:
    if ((i <= 13))
        goto L_37ba;
    else
        goto L_382e;

L_382e:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    return 0x1;

L_3847:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_386f:
    i = 291;
    goto L_389c;

L_3877:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_38a6 : L_3892 */

L_3892:

L_3898:
    i = (i + 1);

L_389c:
    if ((i <= 304))
        goto L_3877;
    else
        goto L_38a6;

L_38a6:
    if ((i <= 304))
        goto L_38cc;
    else
        goto L_38b0;

L_38b0:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_3ba0;
    else
        goto L_38cc;

L_38cc:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_38e5:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), 0x0);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0130), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox.right));
    GetClientRect(hwnd, &(rc));
    rc.top = (rcGBox.bottom + 12);
    rc.left = (rc.left + 12);
    rc.right = (rc.right - 12);
    GetWindowRect(GetDlgItem(hwnd, IDC_HELP), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    rc.bottom = (rcGBox.top - 12);
    _Draw3dFrame(hdc, &(rc), 0xffff);
    rcCargo = rc;
    rcCargo.top = (rcCargo.top - (dyArial8 >> 0x1));
    cch = CchGetString((cColDrop + 306), szWork);
    TextOut(hdc, (rc.left + 8), (rc.top - (dyArial8 >> 0x1)), szWork, cch);
    cch = CchGetString((cColDrop + 320), szWork);
    /* untranslated: call ExpandRc(&rc, ((dyArial8 neg 0) + 0xfffe), ((dyArial8 >> 0x1) neg 0x0)) -> callresult(void) */
    rc.top = (rc.top + 4);
    DrawText(hdc, szWork, cch, &(rc), 0x10);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_3a7c:
    if ((wParam != 0x76))
        goto L_3aa6;
    else
        goto L_3a85;

L_3a85:
    WinHelp(hwnd, szHelpFile, 0x1, 0x411);
    return 0x1;

L_3aa6:
    i = 0;
    goto L_3ac6;

L_3aae:
    if ((wParam == rgidRaceBtn[i]))
        goto L_3acf;
    else
        goto L_3abc;

L_3abc:

L_3ac2:
    i = (i + 1);

L_3ac6:
    if ((i < 4))
        goto L_3aae;
    else
        goto L_3acf;

L_3acf:
    if ((i >= 4))
        goto L_3afc;
    else
        goto L_3ad8;

L_3ad8:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_3afc:
    if ((wParam < 0x123))
        goto L_3ba0;
    else
        goto L_3b06;

L_3b06:
    if ((wParam > 0x130))
        goto L_3ba0;
    else
        goto L_3b10;

L_3b10:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), WM_USER, 0x0, 0x0));
    cColDrop = (wParam - 291);
    SetRaceGrbit(&(vplr), cColDrop, i);
    InvalidateAdvPtsRect(hwnd);
    InvalidateRect(hwnd, rcCargo, 0x0);

L_3b75:
    if ((message == WM_PAINT))
        goto L_38e5;
    else
        goto L_3b7d;

L_3b7d:
    if ((message == WM_ERASEBKGND))
        goto L_3847;
    else
        goto L_3b85;

L_3b85:
    if ((message == WM_CTLCOLOR))
        goto L_386f;
    else
        goto L_3b8d;

L_3b8d:
    if ((message == WM_INITDIALOG))
        goto L_379d;
    else
        goto L_3b95;

L_3b95:
    if ((message == WM_COMMAND))
        goto L_3a7c;
    else
        goto L_3ba0;

L_3ba0:
    return 0x0;
}

int16_t RaceWizardDlg6(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     cch;
    RECT        rcGBox;
    uint16_t    t_merge_3c78_0001;

L_3bae:
    goto L_4074;

L_3bbd:
    SetRCWTitle(hwnd, iPanelActive);
    i = 0;
    goto L_3c19;

L_3bd4:
    CheckRadioButton(hwnd, (LOWORD((3 * i)) + 0x10f), (LOWORD((3 * i)) + 0x111), ((LOWORD((3 * i)) + 0x10f) + GetRaceStat(&(vplr), (i + 8))));
    i = (i + 1);

L_3c19:
    if ((i < 6))
        goto L_3bd4;
    else
        goto L_3c22;

L_3c22:
    if ((fRCWReadOnly == 0))
        goto L_3c57;
    else
        goto L_3c2c;

L_3c2c:
    i = 271;
    goto L_3c4d;

L_3c34:
    EnableWindow(GetDlgItem(hwnd, i), 0x0);
    i = (i + 1);

L_3c4d:
    if ((i <= 288))
        goto L_3c34;
    else
        goto L_3c57;

L_3c57:
    if ((GetRaceStat(&(vplr), rsMajorAdv) != raNone))
        goto L_3c75;
    else
        goto L_3c6f;

L_3c6f:
    t_merge_3c78_0001 = 0x1;
    goto L_3c78;

L_3c75:
    t_merge_3c78_0001 = 0x0;

L_3c78:
    _wsprintf(szWork, PszGetCompressedString(idsCosts75ExtraResearchFieldsStartTech), (t_merge_3c78_0001 + 0x3));
    SetWindowText(GetDlgItem(hwnd, IDC_U16_0x0123), szWork);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0123), CB_LIMITTEXT, GetRaceGrbit(&(vplr), ibitRaceTech3), 0x0);
    if ((fRCWReadOnly == 0))
        goto L_3d03;
    else
        goto L_3ced;

L_3ced:
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0123), 0x0);

L_3d03:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x1);
    return 0x1;

L_3d1c:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_3d44:
    i = 271;
    goto L_3d71;

L_3d4c:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, i) ? L_3d7b : L_3d67 */

L_3d67:

L_3d6d:
    i = (i + 1);

L_3d71:
    if ((i <= 288))
        goto L_3d4c;
    else
        goto L_3d7b;

L_3d7b:
    if ((i <= 288))
        goto L_3dc0;
    else
        goto L_3d85;

L_3d85:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, IDC_U16_0x0123) ? L_3dc0 : L_3da4 */

L_3da4:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_409f;
    else
        goto L_3dc0;

L_3dc0:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_3dd9:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawRaceAdvantagePoints(hdc, &(rc), 0x0);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    i = 0;
    goto L_3f16;

L_3e31:
    GetWindowRect(GetDlgItem(hwnd, (LOWORD((3 * i)) + 0x10f)), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, (LOWORD((3 * i)) + 0x111)), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    cch = CchGetString((i + 84), szWork);
    cch = (cch + CchGetString(idsResearch, szWork[cch]));
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    i = (i + 1);

L_3f16:
    if ((i < 6))
        goto L_3e31;
    else
        goto L_3f1f;

L_3f1f:
    EndPaint(hwnd, &(ps));
    return 0x1;

L_3f34:
    if ((wParam != 0x76))
        goto L_3f5e;
    else
        goto L_3f3d;

L_3f3d:
    WinHelp(hwnd, szHelpFile, 0x1, 0x421);
    return 0x1;

L_3f5e:
    i = 0;
    goto L_3f7e;

L_3f66:
    if ((wParam == rgidRaceBtn[i]))
        goto L_3f87;
    else
        goto L_3f74;

L_3f74:

L_3f7a:
    i = (i + 1);

L_3f7e:
    if ((i < 4))
        goto L_3f66;
    else
        goto L_3f87;

L_3f87:
    if ((i >= 4))
        goto L_3fb7;
    else
        goto L_3f90;

L_3f90:
    StickyDlgPos(hwnd, &(ptStickyRaceDlg), 0x0);
    EndDialog(hwnd, i);
    return 0x1;

L_3fb7:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_4021;
    else
        goto L_3fd3;

L_3fd3:
    if ((wParam < 0x10f))
        goto L_4021;
    else
        goto L_3fdd;

L_3fdd:
    if ((wParam > 0x120))
        goto L_4021;
    else
        goto L_3fe7;

L_3fe7:
    i = (wParam - 271);
    SetRaceStat(&(vplr), (((uint32_t)(i) / 3) + 0x8), ((uint32_t)(i) % 3));
    InvalidateAdvPtsRect(hwnd);
    goto L_409f;

L_4021:
    if ((wParam != 0x123))
        goto L_409f;
    else
        goto L_402b;

L_402b:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), WM_USER, 0x0, 0x0));
    SetRaceGrbit(&(vplr), ibitRaceTech3, i);
    InvalidateAdvPtsRect(hwnd);

L_4074:
    if ((message == WM_PAINT))
        goto L_3dd9;
    else
        goto L_407c;

L_407c:
    if ((message == WM_ERASEBKGND))
        goto L_3d1c;
    else
        goto L_4084;

L_4084:
    if ((message == WM_CTLCOLOR))
        goto L_3d44;
    else
        goto L_408c;

L_408c:
    if ((message == WM_INITDIALOG))
        goto L_3bbd;
    else
        goto L_4094;

L_4094:
    if ((message == WM_COMMAND))
        goto L_3f34;
    else
        goto L_409f;

L_409f:
    return 0x0;
}

void BoundsCheckPlayer(PLAYER *pplr) {
    int16_t i;

L_40ae:
    i = 0;
    goto L_4321;

L_40bf:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) != -1))
        goto L_413c;
    else
        goto L_40d7;

L_40d7:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) != -1))
        goto L_4107;
    else
        goto L_40ef;

L_40ef:
    if (((uint16_t)(pplr->rgEnvVar[i]) == -1))
        goto L_431d;
    else
        goto L_4107;

L_4107:
    pplr->rgEnvVar[i] = -1;
    pplr->rgEnvVarMax[i] = -1;
    pplr->fHacker = 0x1;

L_413c:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) >= 0))
        goto L_4176;
    else
        goto L_4154;

L_4154:
    pplr->rgEnvVarMin[i] = 0;
    pplr->fHacker = 0x1;

L_4176:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) <= 100))
        goto L_41b0;
    else
        goto L_418e;

L_418e:
    pplr->rgEnvVarMin[i] = 100;
    pplr->fHacker = 0x1;

L_41b0:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) <= 100))
        goto L_41ea;
    else
        goto L_41c8;

L_41c8:
    pplr->rgEnvVarMax[i] = 100;
    pplr->fHacker = 0x1;

L_41ea:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) >= (uint16_t)(pplr->rgEnvVarMin[i])))
        goto L_4249;
    else
        goto L_4219;

L_4219:
    pplr->rgEnvVarMax[i] = pplr->rgEnvVarMin[i];
    pplr->fHacker = 0x1;

L_4249:
    if (((uint16_t)(pplr->rgEnvVar[i]) ==
         ((uint16_t)(pplr->rgEnvVarMin[i]) + ((uint32_t)(((uint16_t)(pplr->rgEnvVarMax[i]) - (uint16_t)(pplr->rgEnvVarMin[i]))) / 0x2))))
        goto L_431d;
    else
        goto L_42b2;

L_42b2:
    pplr->rgEnvVar[i] = LOBYTE(((uint16_t)(pplr->rgEnvVarMin[i]) + ((uint32_t)(((uint16_t)(pplr->rgEnvVarMax[i]) - (uint16_t)(pplr->rgEnvVarMin[i]))) / 0x2)));
    pplr->fHacker = 0x1;

L_431d:
    i = (i + 1);

L_4321:
    if ((i < 3))
        goto L_40bf;
    else
        goto L_432a;

L_432a:
    if (((uint16_t)(pplr->pctIdealGrowth) <= 20))
        goto L_4352;
    else
        goto L_4339;

L_4339:
    pplr->pctIdealGrowth = 20;
    pplr->fHacker = 0x1;

L_4352:
    i = 0;
    goto L_4400;

L_435a:
    if (((uint16_t)(pplr->rgAttr[i]) >= (uint16_t)(rgRaceStatMin[i])))
        goto L_43ab;
    else
        goto L_4382;

L_4382:
    pplr->rgAttr[i] = rgRaceStatMin[i];
    pplr->fHacker = 0x1;

L_43ab:
    if (((uint16_t)(pplr->rgAttr[i]) <= (uint16_t)(rgRaceStatMax[i])))
        goto L_43fc;
    else
        goto L_43d3;

L_43d3:
    pplr->rgAttr[i] = rgRaceStatMax[i];
    pplr->fHacker = 0x1;

L_43fc:
    i = (i + 1);

L_4400:
    if ((i < 16))
        goto L_435a;
    else
        goto L_4409;

L_4409:
    return;
}

int16_t CAdvantagePoints(PLAYER *pplr) {
    int16_t  pctGrowth;
    int16_t  iSpread;
    int32_t  cPoints;
    int16_t  cBad;
    int16_t  cCur;
    int16_t  i;
    int16_t  rgi[3];
    int16_t  cGood;
    int32_t  lInnate;
    int16_t  raMajor;
    int16_t  cOperate;
    int16_t  cProduce;
    uint16_t t_merge_44c4_0001;
    uint16_t t_merge_44f2_0001;
    uint16_t t_merge_46d5_0001;
    uint16_t t_merge_46f4_0001;
    uint16_t t_merge_4709_0001;
    int16_t  t_merge_47a5_0001;

L_444c:
    cPoints = 0;
    cPoints = 1650;
    BoundsCheckPlayer(pplr);
    raMajor = GetRaceStat(pplr, rsMajorAdv);
    lInnate = (int32_t)((LInnateRaceHabitability(pplr) / 2000));
    if ((20 >= (uint16_t)(pplr->pctIdealGrowth)))
        goto L_44bd;
    else
        goto L_44b7;

L_44b7:
    t_merge_44c4_0001 = 0x14;
    goto L_44c4;

L_44bd:
    t_merge_44c4_0001 = (uint16_t)(pplr->pctIdealGrowth);

L_44c4:
    if ((0x1 <= t_merge_44c4_0001))
        goto L_44d4;
    else
        goto L_44ce;

L_44ce:
    t_merge_44f2_0001 = 0x1;
    goto L_44f2;

L_44d4:
    if ((20 >= (uint16_t)(pplr->pctIdealGrowth)))
        goto L_44eb;
    else
        goto L_44e5;

L_44e5:
    t_merge_44f2_0001 = 0x14;
    goto L_44f2;

L_44eb:
    t_merge_44f2_0001 = (uint16_t)(pplr->pctIdealGrowth);

L_44f2:
    iSpread = t_merge_44f2_0001;
    if ((iSpread == (uint16_t)(pplr->pctIdealGrowth)))
        goto L_4524;
    else
        goto L_4504;

L_4504:
    iSpread = 1;
    pplr->pctIdealGrowth = 1;
    pplr->fHacker = 0x1;

L_4524:
    pctGrowth = iSpread;
    if ((iSpread <= 5))
        goto L_45c3;
    else
        goto L_4533;

L_4533:
    if ((iSpread <= 13))
        goto L_4561;
    else
        goto L_453c;

L_453c:
    if ((iSpread >= 20))
        goto L_4559;
    else
        goto L_4545;

L_4545:
    iSpread = (LOWORD(((iSpread + 0xfff3) * 0x3)) + 21);
    goto L_45df;

L_4559:
    iSpread = 45;

L_4561:
    if ((iSpread != 6))
        goto L_4576;
    else
        goto L_456a;

L_456a:
    cPoints = (cPoints + 3600);
    goto L_45b2;

L_4576:
    if ((iSpread != 7))
        goto L_458b;
    else
        goto L_457f;

L_457f:
    cPoints = (cPoints + 2250);
    goto L_45b2;

L_458b:
    if ((iSpread != 8))
        goto L_45a0;
    else
        goto L_4594;

L_4594:
    cPoints = (cPoints + 600);
    goto L_45b2;

L_45a0:
    if ((iSpread != 9))
        goto L_45b2;
    else
        goto L_45a9;

L_45a9:
    cPoints = (cPoints + 225);

L_45b2:
    iSpread = (((iSpread - 5) * 2) + 5);

L_45c3:
    cPoints = (cPoints + (uint32_t)(((uint32_t)((0x6 - iSpread)) * 0x1068)));

L_45df:
    lInnate = (int32_t)(((uint32_t)((lInnate * (uint32_t)(iSpread))) / 0x18));
    cPoints = (cPoints - lInnate);
    cGood = 0;
    i = 0;
    goto L_4668;

L_461e:
    if (((uint16_t)(pplr->rgEnvVar[i]) < 0))
        goto L_4660;
    else
        goto L_4636;

L_4636:
    cPoints = (cPoints + (uint32_t)((abs(((uint16_t)(pplr->rgEnvVar[i]) + 0xffce)) * 0x4)));
    goto L_4664;

L_4660:
    cGood = (cGood + 1);

L_4664:
    i = (i + 1);

L_4668:
    if ((i < 3))
        goto L_461e;
    else
        goto L_4671;

L_4671:
    if ((cGood <= 1))
        goto L_4683;
    else
        goto L_467a;

L_467a:
    cPoints = (cPoints - 150);

L_4683:
    cOperate = GetRaceStat(pplr, rsFactOperate);
    cProduce = GetRaceStat(pplr, rsFactProd);
    if ((cOperate > 10))
        goto L_46b9;
    else
        goto L_46b0;

L_46b0:
    if ((cProduce <= 10))
        goto L_4781;
    else
        goto L_46b9;

L_46b9:
    if ((0x1 <= (cOperate - 9)))
        goto L_46cf;
    else
        goto L_46c9;

L_46c9:
    t_merge_46d5_0001 = 0x1;
    goto L_46d5;

L_46cf:
    t_merge_46d5_0001 = (cOperate - 9);

L_46d5:
    cOperate = t_merge_46d5_0001;
    if ((0x1 <= (cProduce - 9)))
        goto L_46ee;
    else
        goto L_46e8;

L_46e8:
    t_merge_46f4_0001 = 0x1;
    goto L_46f4;

L_46ee:
    t_merge_46f4_0001 = (cProduce - 9);

L_46f4:
    cProduce = t_merge_46f4_0001;
    if ((raMajor != 0))
        goto L_4706;
    else
        goto L_4700;

L_4700:
    t_merge_4709_0001 = 0x3;
    goto L_4709;

L_4706:
    t_merge_4709_0001 = 0x2;

L_4709:
    cProduce = LOWORD((t_merge_4709_0001 * cProduce));
    if ((cGood < 2))
        goto L_474e;
    else
        goto L_4718;

L_4718:
    cPoints = (cPoints - (int32_t)(((uint32_t)(((uint32_t)(((uint32_t)(cOperate) * (uint32_t)(cProduce))) * (uint32_t)(pctGrowth))) / 0x2)));
    goto L_4781;

L_474e:
    cPoints = (cPoints - (int32_t)(((uint32_t)(((uint32_t)(((uint32_t)(cOperate) * (uint32_t)(cProduce))) * (uint32_t)(pctGrowth))) / 0x9)));

L_4781:
    i = GetRaceStat(pplr, rsResGen);
    if ((i >= 25))
        goto L_47a2;
    else
        goto L_479c;

L_479c:
    t_merge_47a5_0001 = i;
    goto L_47a5;

L_47a2:
    t_merge_47a5_0001 = 25;

L_47a5:
    i = t_merge_47a5_0001;
    if ((i > 7))
        goto L_47bd;
    else
        goto L_47b1;

L_47b1:
    cPoints = (cPoints - 2400);
    goto L_4802;

L_47bd:
    if ((i != 8))
        goto L_47d2;
    else
        goto L_47c6;

L_47c6:
    cPoints = (cPoints - 1260);
    goto L_4802;

L_47d2:
    if ((i != 9))
        goto L_47e7;
    else
        goto L_47db;

L_47db:
    cPoints = (cPoints - 600);
    goto L_4802;

L_47e7:
    if ((i <= 10))
        goto L_4802;
    else
        goto L_47f0;

L_47f0:
    LOWORD(cPoints) = (LOWORD(cPoints) + LOWORD(((i + 0xfff6) * 0x78)));
    HIWORD(cPoints) = (HIWORD(cPoints) + SIGNHIWORD(LOWORD(((i + 0xfff6) * 0x78))));

L_4802:
    if ((raMajor == 8))
        goto L_4a0b;
    else
        goto L_480b;

L_480b:
    rgi[0] = (10 - GetRaceStat(pplr, rsFactProd));
    rgi[1] = (10 - GetRaceStat(pplr, rsFactBuild));
    rgi[2] = (10 - GetRaceStat(pplr, rsFactOperate));
    cCur = 0;
    if ((rgi[0] <= 0))
        goto L_486a;
    else
        goto L_485e;

L_485e:
    cCur = (cCur + LOWORD((0x64 * rgi[0x0])));
    goto L_4873;

L_486a:
    cCur = (cCur + LOWORD((0x79 * rgi[0x0])));

L_4873:
    if ((rgi[1] >= 0))
        goto L_4888;
    else
        goto L_487c;

L_487c:
    cCur = (cCur - LOWORD((0x37 * rgi[0x1])));
    goto L_4896;

L_4888:
    cCur = (cCur - LOWORD((LOWORD((rgi[0x1] * rgi[0x1])) * 0x3c)));

L_4896:
    if ((rgi[2] <= 0))
        goto L_48ab;
    else
        goto L_489f;

L_489f:
    cCur = (cCur + LOWORD((0x28 * rgi[0x2])));
    goto L_48b4;

L_48ab:
    cCur = (cCur + LOWORD((0x23 * rgi[0x2])));

L_48b4:
    if ((cCur <= 700))
        goto L_48d0;
    else
        goto L_48be;

L_48be:
    cCur = (((uint32_t)((cCur + 0xfd44)) / 3) + 700);

L_48d0:
    if ((rgi[2] > -7))
        goto L_4915;
    else
        goto L_48d9;

L_48d9:
    if ((rgi[2] < -11))
        goto L_48f3;
    else
        goto L_48e2;

L_48e2:
    cCur = (cCur - LOWORD(((0xfffa - rgi[0x2]) * 0x1e)));
    goto L_4915;

L_48f3:
    if ((rgi[2] < -14))
        goto L_4910;
    else
        goto L_48fc;

L_48fc:
    cCur = (cCur - (LOWORD(((0xfff4 - rgi[0x2]) * 0x2d)) + 225));
    goto L_4915;

L_4910:
    cCur = (cCur - 360);

L_4915:
    if ((rgi[0] > -3))
        goto L_4931;
    else
        goto L_491e;

L_491e:
    cCur = (cCur - LOWORD((LOWORD(((0xfffe - rgi[0x0]) * 0x14)) * 0x3)));

L_4931:
    cPoints = (cPoints + (uint32_t)(cCur));
    if ((GetRaceGrbit(pplr, ibitRaceCheapFact) == 0))
        goto L_495b;
    else
        goto L_4952;

L_4952:
    cPoints = (cPoints - 175);

L_495b:
    rgi[0] = (10 - GetRaceStat(pplr, rsMineProd));
    rgi[1] = (3 - GetRaceStat(pplr, rsMineBuild));
    rgi[2] = (10 - GetRaceStat(pplr, rsMineOperate));
    cCur = 0;
    if ((rgi[0] <= 0))
        goto L_49ba;
    else
        goto L_49ae;

L_49ae:
    cCur = (cCur + LOWORD((0x64 * rgi[0x0])));
    goto L_49c3;

L_49ba:
    cCur = (cCur + LOWORD((0xa9 * rgi[0x0])));

L_49c3:
    if ((rgi[1] > 0))
        goto L_49db;
    else
        goto L_49cc;

L_49cc:
    cCur = (cCur - (LOWORD((0x41 * rgi[0x1])) - 80));
    goto L_49e0;

L_49db:
    cCur = (cCur - 360);

L_49e0:
    if ((rgi[2] <= 0))
        goto L_49f5;
    else
        goto L_49e9;

L_49e9:
    cCur = (cCur + LOWORD((0x28 * rgi[0x2])));
    goto L_49fe;

L_49f5:
    cCur = (cCur + LOWORD((0x23 * rgi[0x2])));

L_49fe:
    cPoints = (cPoints + (uint32_t)(cCur));
    goto L_4a14;

L_4a0b:
    cPoints = (cPoints + 210);

L_4a14:
    cPoints = (cPoints - (uint32_t)(rgRacePrimaryTrait[raMajor]));
    cBad = 0;
    cGood = 0;
    i = 0;
    goto L_4a7e;

L_4a38:
    if ((GetRaceGrbit(pplr, i) == 0))
        goto L_4a7a;
    else
        goto L_4a4e;

L_4a4e:
    if ((rgRaceAdvDisPts[i] >= 0))
        goto L_4a65;
    else
        goto L_4a5e;

L_4a5e:
    cBad = (cBad + 1);
    goto L_4a69;

L_4a65:
    cGood = (cGood + 1);

L_4a69:
    cPoints = (cPoints + (uint32_t)(rgRaceAdvDisPts[i]));

L_4a7a:
    i = (i + 1);

L_4a7e:
    if ((i <= 13))
        goto L_4a38;
    else
        goto L_4a87;

L_4a87:
    if (((cBad + cGood) <= 0x4))
        goto L_4ab2;
    else
        goto L_4a95;

L_4a95:
    LOWORD(cPoints) = (LOWORD(cPoints) - LOWORD((LOWORD(((cBad + cGood) * 0xa)) * ((cBad + cGood) + 0xfffc))));
    HIWORD(cPoints) = (HIWORD(cPoints) - SIGNHIWORD(LOWORD((LOWORD(((cBad + cGood) * 0xa)) * ((cBad + cGood) + 0xfffc)))));

L_4ab2:
    if (((cGood - cBad) <= 0x3))
        goto L_4ad5;
    else
        goto L_4ac0;

L_4ac0:
    LOWORD(cPoints) = (LOWORD(cPoints) - LOWORD((((cGood - cBad) + 0xfffd) * 0x3c)));
    HIWORD(cPoints) = (HIWORD(cPoints) - SIGNHIWORD(LOWORD((((cGood - cBad) + 0xfffd) * 0x3c))));

L_4ad5:
    if (((cBad - cGood) <= 0x3))
        goto L_4af8;
    else
        goto L_4ae3;

L_4ae3:
    LOWORD(cPoints) = (LOWORD(cPoints) - LOWORD((((cBad - cGood) + 0xfffd) * 0x28)));
    HIWORD(cPoints) = (HIWORD(cPoints) - SIGNHIWORD(LOWORD((((cBad - cGood) + 0xfffd) * 0x28))));

L_4af8:
    if ((GetRaceGrbit(pplr, ibitRaceNoAdvScanner) == 0))
        goto L_4b4a;
    else
        goto L_4b0f;

L_4b0f:
    if ((raMajor != 6))
        goto L_4b24;
    else
        goto L_4b18;

L_4b18:
    cPoints = (cPoints - 280);
    goto L_4b4a;

L_4b24:
    if ((raMajor != 1))
        goto L_4b39;
    else
        goto L_4b2d;

L_4b2d:
    cPoints = (cPoints - 200);
    goto L_4b4a;

L_4b39:
    if ((raMajor != 9))
        goto L_4b4a;
    else
        goto L_4b42;

L_4b42:
    cPoints = (cPoints - 40);

L_4b4a:
    cCur = 0;
    i = 8;
    goto L_4b6f;

L_4b57:
    cCur = (cCur + (GetRaceStat(pplr, i) - 1));
    i = (i + 1);

L_4b6f:
    if ((i <= 13))
        goto L_4b57;
    else
        goto L_4b78;

L_4b78:
    if ((cCur <= 0))
        goto L_4bbd;
    else
        goto L_4b81;

L_4b81:
    LOWORD(cPoints) = (LOWORD(cPoints) - LOWORD((LOWORD((cCur * cCur)) * 0x82)));
    HIWORD(cPoints) = (HIWORD(cPoints) - SIGNHIWORD(LOWORD((LOWORD((cCur * cCur)) * 0x82))));
    if ((cCur != 6))
        goto L_4ba8;
    else
        goto L_4b9c;

L_4b9c:
    cPoints = (cPoints + 1430);
    goto L_4c09;

L_4ba8:
    if ((cCur != 5))
        goto L_4c09;
    else
        goto L_4bb1;

L_4bb1:
    cPoints = (cPoints + 520);

L_4bbd:
    if ((cCur >= 0))
        goto L_4c09;
    else
        goto L_4bc6;

L_4bc6:
    /* untranslated: cPoints = (cPoints + sext16to32(rgRaceDisEnvPts[((cCur neg 0x0) - 0x1)])) */
    /* untranslated: branch (cCur neg 0) <= 0x4 ? L_4c09 : L_4be9 */

L_4be9:
    if ((GetRaceStat(pplr, rsResGen) >= 10))
        goto L_4c09;
    else
        goto L_4c00;

L_4c00:
    cPoints = (cPoints - 190);

L_4c09:
    if ((GetRaceGrbit(pplr, ibitRaceTech3) == 0))
        goto L_4c29;
    else
        goto L_4c20;

L_4c20:
    cPoints = (cPoints - 180);

L_4c29:
    if ((raMajor != 8))
        goto L_4c51;
    else
        goto L_4c32;

L_4c32:
    if ((GetRaceStat(pplr, rsTechBonus1) != 2))
        goto L_4c51;
    else
        goto L_4c49;

L_4c49:
    cPoints = (cPoints - 100);

L_4c51:

L_4c67:
    return LOWORD((int32_t)((cPoints / 3)));
}

int32_t LInnateRaceHabitability(PLAYER *pplr) {
    int16_t  iTry;
    PLANET   pl;
    double   l2;
    int16_t  rgSteps[3];
    PLAYER   plrT;
    int16_t  rgDelta[3];
    int16_t  fTotalTerra;
    int16_t  rgInc[3];
    int16_t  i;
    int16_t  iTerra;
    int16_t  j;
    int32_t  l1;
    int16_t  rgBase[3];
    double   l3;
    int16_t  iDelta;
    int32_t  pctDesire;
    int16_t  k;
    double   lInnate;
    int16_t  pctTerra;
    uint16_t t_merge_4d0e_0001;
    uint16_t t_merge_4d28_0001;

L_4c6e:
    plrT = rgplr[0];
    lInnate = (double)(0);
    fTotalTerra = GetRaceGrbit(pplr, ibitRaceTT);
    rgplr[0] = *(pplr);
    rgDelta[2] = 0;
    rgDelta[1] = 0;
    rgDelta[0] = 0;
    iTerra = 0;
    goto L_548c;

L_4cde:
    if ((iTerra != 0))
        goto L_4cf1;
    else
        goto L_4ce8;

L_4ce8:
    pctTerra = 0;
    goto L_4d2c;

L_4cf1:
    if ((iTerra != 1))
        goto L_4d15;
    else
        goto L_4cfb;

L_4cfb:
    if ((fTotalTerra == 0))
        goto L_4d0b;
    else
        goto L_4d05;

L_4d05:
    t_merge_4d0e_0001 = 0x8;
    goto L_4d0e;

L_4d0b:
    t_merge_4d0e_0001 = 0x5;

L_4d0e:
    pctTerra = t_merge_4d0e_0001;
    goto L_4d2c;

L_4d15:
    if ((fTotalTerra == 0))
        goto L_4d25;
    else
        goto L_4d1f;

L_4d1f:
    t_merge_4d28_0001 = 0x11;
    goto L_4d28;

L_4d25:
    t_merge_4d28_0001 = 0xf;

L_4d28:
    pctTerra = t_merge_4d28_0001;

L_4d2c:
    i = 0;
    goto L_4f56;

L_4d35:
    if (((uint16_t)(pplr->rgEnvVar[i]) > 100))
        goto L_4dcb;
    else
        goto L_4d4e;

L_4d4e:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) > 100))
        goto L_4dcb;
    else
        goto L_4d67;

L_4d67:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) > 100))
        goto L_4dcb;
    else
        goto L_4d80;

L_4d80:
    if (((uint16_t)(pplr->rgEnvVar[i]) < 0))
        goto L_4dcb;
    else
        goto L_4d99;

L_4d99:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) < 0))
        goto L_4dcb;
    else
        goto L_4db2;

L_4db2:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) >= 0))
        goto L_4e6b;
    else
        goto L_4dcb;

L_4dcb:
    if (((uint16_t)(pplr->rgEnvVar[i]) != -1))
        goto L_4e16;
    else
        goto L_4de4;

L_4de4:
    if (((uint16_t)(pplr->rgEnvVarMin[i]) != -1))
        goto L_4e16;
    else
        goto L_4dfd;

L_4dfd:
    if (((uint16_t)(pplr->rgEnvVarMax[i]) == -1))
        goto L_4e6b;
    else
        goto L_4e16;

L_4e16:
    pplr->rgEnvVarMax[i] = -1;
    pplr->rgEnvVarMin[i] = -1;
    pplr->rgEnvVar[i] = -1;
    pplr->fHacker = 0x1;
    rgplr[0] = *(pplr);

L_4e6b:
    if (((uint16_t)(pplr->rgEnvVar[i]) >= 0))
        goto L_4eb6;
    else
        goto L_4e84;

L_4e84:
    rgBase[i] = 50;
    rgInc[i] = 11;
    rgSteps[i] = 1;
    goto L_4f51;

L_4eb6:
    rgBase[i] = ((uint16_t)(pplr->rgEnvVarMin[i]) - pctTerra);
    if ((rgBase[i] >= 0))
        goto L_4efd;
    else
        goto L_4eed;

L_4eed:
    rgBase[i] = 0;

L_4efd:
    iTry = ((uint16_t)(pplr->rgEnvVarMax[i]) + pctTerra);
    if ((iTry <= 100))
        goto L_4f23;
    else
        goto L_4f1e;

L_4f1e:
    iTry = 100;

L_4f23:
    rgInc[i] = (iTry - rgBase[i]);
    rgSteps[i] = 11;

L_4f51:
    i = (i + 1);

L_4f56:
    if ((i < 3))
        goto L_4d35;
    else
        goto L_4f60;

L_4f60:
    l3 = (double)(0);
    i = 0;
    goto L_4f86;

L_4f81:
    i = (i + 1);

L_4f86:
    if ((i >= rgSteps[0]))
        goto L_5409;
    else
        goto L_4f92;

L_4f92:
    if ((i == 0))
        goto L_4fa5;
    else
        goto L_4f9c;

L_4f9c:
    if ((rgSteps[0] > 1))
        goto L_4faf;
    else
        goto L_4fa5;

L_4fa5:
    iTry = rgBase[0];
    goto L_4fc7;

L_4faf:
    /* untranslated: iTry = ((words(loword((i * rgInc[0x0])), signhiword(loword((i * rgInc[0x0])))) / (rgSteps[0] - 1)) + rgBase[0]) */

L_4fc7:
    if ((iTerra == 0))
        goto L_5042;
    else
        goto L_4fd1;

L_4fd1:
    if (((uint16_t)(pplr->rgEnvVar[0]) < 0))
        goto L_5042;
    else
        goto L_4fdd;

L_4fdd:

L_4fe3:
    iDelta = ((uint16_t)(pplr->rgEnvVar[0x0]) - iTry);
    if ((abs(iDelta) > pctTerra))
        goto L_500f;
    else
        goto L_5006;

L_5006:
    iDelta = 0;
    goto L_502c;

L_500f:
    if ((iDelta >= 0))
        goto L_5024;
    else
        goto L_5019;

L_5019:
    iDelta = (iDelta + pctTerra);
    goto L_502c;

L_5024:
    iDelta = (iDelta - pctTerra);

L_502c:
    rgDelta[0] = iDelta;
    iTry = ((uint16_t)(pplr->rgEnvVar[0x0]) - iDelta);

L_5042:
    pl.rgEnvVar[0] = LOBYTE(iTry);
    l2 = (double)(0);
    j = 0;
    goto L_506d;

L_5068:
    j = (j + 1);

L_506d:
    if ((j >= rgSteps[1]))
        goto L_538d;
    else
        goto L_5079;

L_5079:
    if ((j == 0))
        goto L_508c;
    else
        goto L_5083;

L_5083:
    if ((rgSteps[1] > 1))
        goto L_5096;
    else
        goto L_508c;

L_508c:
    iTry = rgBase[1];
    goto L_50ae;

L_5096:
    /* untranslated: iTry = ((words(loword((j * rgInc[0x1])), signhiword(loword((j * rgInc[0x1])))) / (rgSteps[1] - 1)) + rgBase[1]) */

L_50ae:
    if ((iTerra == 0))
        goto L_5129;
    else
        goto L_50b8;

L_50b8:
    if (((uint16_t)(pplr->rgEnvVar[1]) < 0))
        goto L_5129;
    else
        goto L_50c4;

L_50c4:

L_50ca:
    iDelta = ((uint16_t)(pplr->rgEnvVar[0x1]) - iTry);
    if ((abs(iDelta) > pctTerra))
        goto L_50f6;
    else
        goto L_50ed;

L_50ed:
    iDelta = 0;
    goto L_5113;

L_50f6:
    if ((iDelta >= 0))
        goto L_510b;
    else
        goto L_5100;

L_5100:
    iDelta = (iDelta + pctTerra);
    goto L_5113;

L_510b:
    iDelta = (iDelta - pctTerra);

L_5113:
    rgDelta[1] = iDelta;
    iTry = ((uint16_t)(pplr->rgEnvVar[0x1]) - iDelta);

L_5129:
    pl.rgEnvVar[1] = LOBYTE(iTry);
    l1 = 0;
    k = 0;
    goto L_5149;

L_5144:
    k = (k + 1);

L_5149:
    if ((k >= rgSteps[2]))
        goto L_531b;
    else
        goto L_5155;

L_5155:
    if ((k == 0))
        goto L_5168;
    else
        goto L_515f;

L_515f:
    if ((rgSteps[2] > 1))
        goto L_5172;
    else
        goto L_5168;

L_5168:
    iTry = rgBase[2];
    goto L_518a;

L_5172:
    /* untranslated: iTry = ((words(loword((k * rgInc[0x2])), signhiword(loword((k * rgInc[0x2])))) / (rgSteps[2] - 1)) + rgBase[2]) */

L_518a:
    if ((iTerra == 0))
        goto L_5205;
    else
        goto L_5194;

L_5194:
    if (((uint16_t)(pplr->rgEnvVar[2]) < 0))
        goto L_5205;
    else
        goto L_51a0;

L_51a0:

L_51a6:
    iDelta = ((uint16_t)(pplr->rgEnvVar[0x2]) - iTry);
    if ((abs(iDelta) > pctTerra))
        goto L_51d2;
    else
        goto L_51c9;

L_51c9:
    iDelta = 0;
    goto L_51ef;

L_51d2:
    if ((iDelta >= 0))
        goto L_51e7;
    else
        goto L_51dc;

L_51dc:
    iDelta = (iDelta + pctTerra);
    goto L_51ef;

L_51e7:
    iDelta = (iDelta - pctTerra);

L_51ef:
    rgDelta[2] = iDelta;
    iTry = ((uint16_t)(pplr->rgEnvVar[0x2]) - iDelta);

L_5205:
    pl.rgEnvVar[2] = LOBYTE(iTry);
    pctDesire = (uint32_t)(PctPlanetDesirability(&(pl), 0x0));
    iDelta = ((rgDelta[0] + rgDelta[1]) + rgDelta[2]);
    if ((iDelta <= pctTerra))
        goto L_527a;
    else
        goto L_5244;

L_5244:
    pctDesire = (pctDesire - (uint32_t)((iDelta - pctTerra)));
    if ((HIWORD(pctDesire) > 0x0))
        goto L_527a;
    else
        goto L_525f;

L_525f:
    if ((HIWORD(pctDesire) < 0x0))
        goto L_526e;
    else
        goto L_5264;

L_5264:
    if ((LOWORD(pctDesire) >= 0x0))
        goto L_527a;
    else
        goto L_526e;

L_526e:
    pctDesire = 0;

L_527a:
    pctDesire = (uint32_t)((pctDesire * pctDesire));
    if ((iTerra != 0))
        goto L_52c1;
    else
        goto L_52a1;

L_52a1:
    pctDesire = (uint32_t)((pctDesire * 7));
    goto L_5308;

L_52c1:
    if ((iTerra != 1))
        goto L_52eb;
    else
        goto L_52cb;

L_52cb:
    pctDesire = (uint32_t)((pctDesire * 5));
    goto L_5308;

L_52eb:
    pctDesire = (uint32_t)((pctDesire * 6));

L_5308:
    l1 = (l1 + pctDesire);
    goto L_5144;

L_531b:
    if (((uint16_t)(pplr->rgEnvVar[2]) < 0))
        goto L_5358;
    else
        goto L_532a;

L_532a:
    l1 = (int32_t)(((uint32_t)((l1 * (uint32_t)(rgInc[0x2]))) / 0x64));
    goto L_5375;

L_5358:
    l1 = (uint32_t)((l1 * 11));

L_5375:
    l2 = ((double)(l1) + l2);
    goto L_5068;

L_538d:
    if (((uint16_t)(pplr->rgEnvVar[1]) < 0))
        goto L_53d5;
    else
        goto L_539c;

L_539c:
    l2 = ((l2 * (double)((int32_t)(rgInc[0x1]))) / (double)(100));
    goto L_53f6;

L_53d5:
    l2 = (l2 * (double)(11));

L_53f6:
    l3 = (l3 + l2);
    goto L_4f81;

L_5409:
    if (((uint16_t)(pplr->rgEnvVar[0]) < 0))
        goto L_5453;
    else
        goto L_5418;

L_5418:
    l3 = ((l3 * (double)((int32_t)(rgInc[0x0]))) / (double)(100));
    goto L_5476;

L_5453:
    l3 = (l3 * (double)(11));

L_5476:
    lInnate = (lInnate + l3);
    iTerra = (iTerra + 1);

L_548c:
    if ((iTerra < 3))
        goto L_4cde;
    else
        goto L_5496;

L_5496:
    if ((pplr == rgplr))
        goto L_54b1;
    else
        goto L_54a0;

L_54a0:
    rgplr[0] = plrT;

L_54b1:
    __ftol();

L_54ce:
    /* untranslated: return callresult(int32_t) */
}

void InvalidateAdvPtsRect(HWND hwnd) {
    HDC        hdc;
    TEXTMETRIC tm;
    LOGFONT   *plf;
    int16_t    dyBig;
    HFONT      hfont;
    int16_t    dx;
    RECT       rc;
    HFONT      hfontSav;

L_54d4:
    plf = LocalAlloc(0x40, 0x32);
    hdc = GetDC(hwnd);
    plf->lfHeight = -24;
    strcpy(((uint8_t *)(plf) + 0x12), rgszArial[1]);
    hfont = CreateFontIndirect(plf);
    hfontSav = SelectObject(hdc, hfont);
    GetTextMetrics(hdc, &(tm));
    dyBig = (tm.tmHeight + tm.tmExternalLeading);
    dx = (LOWORD(GetTextExtent(hdc, "-99999", 0x6)) + 8);
    GetClientRect(hwnd, &(rc));
    rc.left = (rc.right - dx);
    rc.bottom = ((rc.top + dyBig) + 4);
    SelectObject(hdc, hfontSav);
    DeleteObject(hfont);
    LocalFree(plf);
    ReleaseDC(hwnd, hdc);
    InvalidateRect(hwnd, &(rc), 0x1);
    return;
}

void DrawRaceAdvantagePoints(HDC hdc, RECT *prc, PLAYER *pplr) {
    TEXTMETRIC tm;
    LOGFONT   *plf;
    COLORREF   crBkSav;
    int16_t    bkMode;
    int16_t    dyBig;
    char       szAdvantage[32];
    int16_t    c;
    COLORREF   crSav;
    HFONT      hfont;
    int16_t    dx;
    int16_t    iPts;
    int16_t    cch;
    RECT       rc;
    HFONT      hfontSav;
    uint32_t   t_merge_56b3_0001;

L_55c6:
    plf = LocalAlloc(0x40, 0x32);
    plf->lfHeight = -24;
    CchGetString(idsArialBold, ((uint8_t *)(plf) + 0x12));
    hfont = CreateFontIndirect(plf);
    hfontSav = SelectObject(hdc, hfont);
    GetTextMetrics(hdc, &(tm));
    dyBig = (tm.tmHeight + tm.tmExternalLeading);
    dx = (LOWORD(GetTextExtent(hdc, "-99999", 0x6)) + 8);
    rc = *(prc);
    rc.left = (rc.right - dx);
    rc.bottom = ((rc.top + dyBig) + 4);
    if ((pplr != 0x0))
        goto L_567b;
    else
        goto L_5676;

L_5676:
    pplr = vplr;

L_567b:
    iPts = CAdvantagePoints(pplr);
    bkMode = SetBkMode(hdc, OPAQUE);
    if ((iPts < 0))
        goto L_56ad;
    else
        goto L_56a4;

L_56a4:
    t_merge_56b3_0001 = 0x0;
    goto L_56b3;

L_56ad:
    t_merge_56b3_0001 = 0x7f;

L_56b3:
    crSav = SetTextColor(hdc, t_merge_56b3_0001);
    crBkSav = SetBkColor(hdc, crButtonFace);
    c = _wsprintf(szWork, PCTD, iPts);
    RcCtrTextOut(hdc, &(rc), szWork, 0xffff);
    SetTextColor(hdc, 0x0);
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsPointsLeft, szWork);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    cch = CchGetString(idsAdvantage, szAdvantage);
    RightTextOut(hdc, rc.left, 0x3, szAdvantage, cch, 0x0);
    RightTextOut(hdc, rc.left, (dyArial8 + 3), szWork, c, 0x0);
    rc.left = (rc.left - (dx + 4));
    PatBlt(hdc, (rc.left - 1), 0x0, 0x1, (rc.bottom + 1), BLACKNESS);
    PatBlt(hdc, (rc.left - 4), 0x0, 0x2, (rc.bottom + 4), BLACKNESS);
    PatBlt(hdc, rc.left, rc.bottom, (rc.right - rc.left), 0x1, BLACKNESS);
    PatBlt(hdc, (rc.left - 2), (rc.bottom + 2), ((rc.right - rc.left) + 0x2), 0x2, BLACKNESS);
    SetBkColor(hdc, crBkSav);
    SetTextColor(hdc, crSav);
    SetBkMode(hdc, bkMode);
    SelectObject(hdc, hfontSav);
    DeleteObject(hfont);
    LocalFree(plf);
    return;
}

uint16_t IRaceChecksum(PLAYER *pplr) {
    uint16_t  ick;
    uint16_t *p;
    int16_t   i;
    int16_t   cs;

L_5888:
    p = pplr;
    cs = 96;
    ick = 0x0;
    i = 0;
    goto L_58bc;

L_58a9:
    ick = (ick ^ p[i]);
    i = (i + 1);

L_58bc:
    if ((i < cs))
        goto L_58a9;
    else
        goto L_58c7;

L_58c7:

L_58cd:
    return ick;
}

int16_t FSaveRace(char *szFileSuggest, PLAYER *pplr) {
    uint16_t     icksum;
    char         szFileTitle[256];
    char         szDirName[256];
    char         szFilter[256];
    uint16_t     i;
    char         szFile[256];
    OPENFILENAME ofn;

L_58d4:
    if ((szFileSuggest == 0x0))
        goto L_58f9;
    else
        goto L_58e6;

L_58e6:
    strcpy(szFile, szFileSuggest);
    goto L_58fe;

L_58f9:
    szFile[0] = 0;

L_58fe:
    szDirName[0] = 0;
    CchGetString(idsStarsRaceFilesR, szFilter);
    i = 0x0;
    goto L_5922;

L_591d:
    i = (i + 0x1);

L_5922:
    if (((uint16_t)(szFilter[i]) == 0))
        goto L_595c;
    else
        goto L_5937;

L_5937:
    if (((uint16_t)(szFilter[i]) != 124))
        goto L_591d;
    else
        goto L_594c;

L_594c:
    szFilter[i] = 0;

L_595c:
    memset(ofn, 0x0, 0x48);
    ofn.lStructSize = 0x48;
    ofn.hwndOwner = hwndRaceParent;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 0x1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = 0x100;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = 0x100;
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt = "r1";
    ofn.Flags = 0x8806;
    if ((GetSaveFileName(&(ofn)) == 0))
        goto L_5aa5;
    else
        goto L_5a0e;

L_5a0e:
    if ((FCreateFile(0x5, 0xffff, szFile) != 0))
        goto L_5a52;
    else
        goto L_5a2b;

L_5a2b:
    AlertSz(PszFormatIds(idsStarsUnableSaveRaceDataFilePlease, 0x0), MB_ICONHAND);
    return 0x0;

L_5a52:
    WriteRtPlr(pplr, 0x0);
    icksum = IRaceChecksum(pplr);
    WriteRt(rtEOF, 0x2, &(icksum));
    StreamClose();
    strcpy(szRaceFile, szFile[ofn.nFileOffset]);
    goto L_5aab;

L_5aa5:
    return 0x0;

L_5aab:
    return 0x1;
}

void SetRCWTitle(HWND hwnd, int16_t iStep) {
    char    szBuf[50];
    int16_t cch;

L_5ab8:
    cch = CchGetString((fRCWReadOnly + 270), szBuf);
    cch = _wsprintf(szWork, szBuf, iStep);
    SetWindowText(hwnd, szWork);
    return;
}

void CreateRandomRace(PLAYER *pplr) {
    int16_t  cPts;
    int16_t  i;
    int16_t  cPass;
    int16_t  j;
    int16_t  iVal;
    int16_t  dAwayNew;
    int16_t  dAwayCur;
    int16_t  k;
    PLAYER   plrT;
    uint16_t t_merge_5c52_0001;
    int16_t  t_merge_5e8b_0001;
    int16_t  t_merge_5ef5_0001;
    int16_t  t_6091;
    uint16_t t_merge_6109_0001;
    uint16_t t_merge_6184_0001;
    uint16_t t_merge_61f7_0001;
    uint16_t t_merge_6293_0001;
    uint16_t t_merge_6345_0001;
    uint16_t t_merge_63e4_0001;
    uint16_t t_merge_6438_0001;
    uint16_t t_merge_64f5_0001;
    uint16_t t_merge_65a9_0001;

L_5b08:
    game.szName[(pplr + 16)] = 0;
    iVal = Random(0x19);
    if ((iVal >= 4))
        goto L_5b8e;
    else
        goto L_5b31;

L_5b31:
    i = 0;
    goto L_5b6d;

L_5b39:
    pplr->rgEnvVarMax[i] = -1;
    pplr->rgEnvVarMin[i] = -1;
    pplr->rgEnvVar[i] = -1;
    i = (i + 1);

L_5b6d:
    if ((i < 3))
        goto L_5b39;
    else
        goto L_5b76;

L_5b76:
    pplr->pctIdealGrowth = LOBYTE((Random(0x4) + 0x2));
    goto L_5e59;

L_5b8e:
    if ((iVal >= 7))
        goto L_5bf4;
    else
        goto L_5b97;

L_5b97:
    i = 0;
    goto L_5bd3;

L_5b9f:
    pplr->rgEnvVar[i] = 50;
    pplr->rgEnvVarMin[i] = 0;
    pplr->rgEnvVarMax[i] = 100;
    i = (i + 1);

L_5bd3:
    if ((i < 3))
        goto L_5b9f;
    else
        goto L_5bdc;

L_5bdc:
    pplr->pctIdealGrowth = LOBYTE((Random(0x4) + 0x3));
    goto L_5e59;

L_5bf4:
    if ((iVal >= 9))
        goto L_5ccb;
    else
        goto L_5bfd;

L_5bfd:
    i = 0;
    goto L_5caa;

L_5c05:
    j = Random(0x2);
    if ((i != 2))
        goto L_5c55;
    else
        goto L_5c1d;

L_5c1d:
    if (((uint16_t)(pplr->rgEnvVar[0]) != (uint16_t)(pplr->rgEnvVar[1])))
        goto L_5c55;
    else
        goto L_5c3a;

L_5c3a:
    if (((uint16_t)(pplr->rgEnvVar[0]) == 0))
        goto L_5c4f;
    else
        goto L_5c49;

L_5c49:
    t_merge_5c52_0001 = 0x1;
    goto L_5c52;

L_5c4f:
    t_merge_5c52_0001 = 0x0;

L_5c52:
    j = t_merge_5c52_0001;

L_5c55:
    if ((j != 0))
        goto L_5c91;
    else
        goto L_5c5e;

L_5c5e:
    pplr->rgEnvVar[i] = 50;
    pplr->rgEnvVarMin[i] = 0;
    pplr->rgEnvVarMax[i] = 100;
    goto L_5ca6;

L_5c91:
    pplr->pctIdealGrowth = LOBYTE((Random(0x4) + 0x2));

L_5ca6:
    i = (i + 1);

L_5caa:
    if ((i < 3))
        goto L_5c05;
    else
        goto L_5cb3;

L_5cb3:
    pplr->pctIdealGrowth = LOBYTE((Random(0x5) + 0x2));
    goto L_5e59;

L_5ccb:
    i = 0;
    goto L_5d42;

L_5cd3:
    j = ((Random(0x28) * 2) + 20);
    k = Random(((100 - j) + 0x1));
    pplr->rgEnvVar[i] = LOBYTE((((uint32_t)(j) / 0x2) + k));
    pplr->rgEnvVarMin[i] = LOBYTE(k);
    pplr->rgEnvVarMax[i] = LOBYTE((k + j));
    i = (i + 1);

L_5d42:
    if ((i < 3))
        goto L_5cd3;
    else
        goto L_5d4b;

L_5d4b:
    if ((iVal >= 12))
        goto L_5d96;
    else
        goto L_5d54;

L_5d54:
    i = Random(0x3);
    pplr->rgEnvVarMax[i] = -1;
    pplr->rgEnvVarMin[i] = -1;
    pplr->rgEnvVar[i] = -1;
    goto L_5e44;

L_5d96:
    if ((iVal >= 14))
        goto L_5de1;
    else
        goto L_5d9f;

L_5d9f:
    i = Random(0x3);
    pplr->rgEnvVar[i] = 50;
    pplr->rgEnvVarMin[i] = 0;
    pplr->rgEnvVarMax[i] = 100;
    goto L_5e44;

L_5de1:
    if ((iVal >= 17))
        goto L_5e44;
    else
        goto L_5dea;

L_5dea:
    i = Random(0x3);
    j = Random(0x51);
    pplr->rgEnvVar[i] = LOBYTE((j + 0xa));
    pplr->rgEnvVarMin[i] = LOBYTE(j);
    pplr->rgEnvVarMax[i] = LOBYTE((j + 0x14));

L_5e44:
    pplr->pctIdealGrowth = LOBYTE((Random(0x9) + 0x7));

L_5e59:
    iVal = Random(0x3);
    i = 8;
    goto L_5e9e;

L_5e70:
    if ((iVal == 0))
        goto L_5e88;
    else
        goto L_5e79;

L_5e79:
    t_merge_5e8b_0001 = Random(0x3);
    goto L_5e8b;

L_5e88:
    t_merge_5e8b_0001 = 1;

L_5e8b:
    SetRaceStat(pplr, i, t_merge_5e8b_0001);
    i = (i + 1);

L_5e9e:
    if ((i <= 13))
        goto L_5e70;
    else
        goto L_5ea7;

L_5ea7:
    SetRaceStat(pplr, rsMajorAdv, Random(0xa));
    iVal = Random(0x4);
    i = 0;
    goto L_5f08;

L_5eda:
    if ((iVal == 0))
        goto L_5ef2;
    else
        goto L_5ee3;

L_5ee3:
    t_merge_5ef5_0001 = Random(0x2);
    goto L_5ef5;

L_5ef2:
    t_merge_5ef5_0001 = 0;

L_5ef5:
    SetRaceGrbit(pplr, i, t_merge_5ef5_0001);
    i = (i + 1);

L_5f08:
    if ((i <= 13))
        goto L_5eda;
    else
        goto L_5f11;

L_5f11:
    SetRaceGrbit(pplr, ibitRaceTech3, Random(0x2));
    SetRaceGrbit(pplr, ibitRaceCheapFact, Random(0x2));
    iVal = Random(0x3);
    if ((iVal != 0))
        goto L_5fa4;
    else
        goto L_5f61;

L_5f61:
    i = 0;
    goto L_5f86;

L_5f69:
    pplr->rgAttr[i] = vrgplrDef[0x0].rgAttr[i];
    i = (i + 1);

L_5f86:
    if ((i <= 6))
        goto L_5f69;
    else
        goto L_5f8f;

L_5f8f:
    pplr->rgAttr[7] = LOBYTE(Random(0x5));
    goto L_6003;

L_5fa4:
    i = 0;
    goto L_5ffa;

L_5fac:
    pplr->rgAttr[i] = LOBYTE(((uint16_t)(rgRaceStatMin[i]) + Random((((uint16_t)(rgRaceStatMax[i]) + 0x1) - (uint16_t)(rgRaceStatMin[i])))));
    i = (i + 1);

L_5ffa:
    if ((i <= 7))
        goto L_5fac;
    else
        goto L_6003;

L_6003:
    if ((strcmp(((uint8_t *)(pplr) + 0x80), PszGetCompressedString(idsRandom2)) != 0))
        goto L_604a;
    else
        goto L_6029;

L_6029:
    CchGetString((Random(0x18) + 1390), ((uint8_t *)(pplr) + 0x80));

L_604a:
    cPts = CAdvantagePoints(pplr);
    if ((cPts < 0))
        goto L_606d;
    else
        goto L_6061;

L_6061:
    if ((cPts <= 50))
        goto L_65ce;
    else
        goto L_6067;

L_6067:

L_606d:
    cPass = 0;

L_6072:
    CAdvantagePoints(pplr);
    /* untranslated: cPts = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) < 0 ? L_6091 : L_6088 */

L_6088:
    if ((cPts <= 50))
        goto L_65ce;
    else
        goto L_6091;

L_6091:
    t_6091 = cPass;
    cPass = (cPass + 1);
    if ((t_6091 <= 250))
        goto L_60da;
    else
        goto L_60a0;

L_60a0:
    plrT = *(pplr);
    *(pplr) = vrgplrDef[0];
    strcpy(((uint8_t *)(pplr) + 0x80), plrT.szName);
    goto L_65ce;

L_60da:
    iVal = Random(0xa);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_6103 : L_60fb */

L_60fb:
    /* untranslated: t_merge_6109_0001 = (cPts neg 0) */
    goto L_6109;

L_6103:
    t_merge_6109_0001 = (cPts - 50);

L_6109:
    dAwayCur = t_merge_6109_0001;
    if ((iVal >= 3))
        goto L_6223;
    else
        goto L_6115;

L_6115:
    i = Random(0x6);
    j = (uint16_t)(pplr->rgAttr[(i + 0x8)]);
    if ((j <= 0))
        goto L_61ad;
    else
        goto L_6143;

L_6143:
    pplr->rgAttr[(i + 8)] = (pplr->rgAttr[(i + 8)] - 1);
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_617e : L_6176 */

L_6176:
    /* untranslated: t_merge_6184_0001 = (cPts neg 0) */
    goto L_6184;

L_617e:
    t_merge_6184_0001 = (cPts - 50);

L_6184:
    dAwayNew = t_merge_6184_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_618f;

L_618f:

L_6198:
    pplr->rgAttr[(i + 8)] = LOBYTE(j);

L_61ad:
    if ((j >= 2))
        goto L_6072;
    else
        goto L_61b6;

L_61b6:
    pplr->rgAttr[(i + 8)] = (pplr->rgAttr[(i + 8)] + 1);
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_61f1 : L_61e9 */

L_61e9:
    /* untranslated: t_merge_61f7_0001 = (cPts neg 0) */
    goto L_61f7;

L_61f1:
    t_merge_61f7_0001 = (cPts - 50);

L_61f7:
    dAwayNew = t_merge_61f7_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_6202;

L_6202:

L_620b:
    pplr->rgAttr[(i + 8)] = LOBYTE(j);

L_6223:
    if ((iVal >= 6))
        goto L_62d1;
    else
        goto L_622c;

L_622c:
    iVal = Random(0xe);
    j = GetRaceGrbit(pplr, iVal);
    i = 0;
    goto L_62a8;

L_6254:
    SetRaceGrbit(pplr, iVal, i);
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_628d : L_6285 */

L_6285:
    /* untranslated: t_merge_6293_0001 = (cPts neg 0) */
    goto L_6293;

L_628d:
    t_merge_6293_0001 = (cPts - 50);

L_6293:
    dAwayNew = t_merge_6293_0001;
    if ((dAwayNew < dAwayCur))
        goto L_62b1;
    else
        goto L_629e;

L_629e:

L_62a4:
    i = (i + 1);

L_62a8:
    if ((i < 2))
        goto L_6254;
    else
        goto L_62b1;

L_62b1:
    if ((i < 2))
        goto L_6072;
    else
        goto L_62b7;

L_62b7:

L_62bd:
    SetRaceGrbit(pplr, iVal, j);
    goto L_6072;

L_62d1:
    if ((iVal >= 9))
        goto L_6383;
    else
        goto L_62da;

L_62da:
    iVal = Random(0x7);
    j = GetRaceStat(pplr, iVal);
    i = -1;
    goto L_635a;

L_6302:
    SetRaceStat(pplr, iVal, (j + i));
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_633f : L_6337 */

L_6337:
    /* untranslated: t_merge_6345_0001 = (cPts neg 0) */
    goto L_6345;

L_633f:
    t_merge_6345_0001 = (cPts - 50);

L_6345:
    dAwayNew = t_merge_6345_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6363;
    else
        goto L_6350;

L_6350:

L_6356:
    i = (i + 2);

L_635a:
    if ((i <= 1))
        goto L_6302;
    else
        goto L_6363;

L_6363:
    if ((i <= 1))
        goto L_6072;
    else
        goto L_6369;

L_6369:

L_636f:
    SetRaceStat(pplr, iVal, j);
    goto L_6072;

L_6383:
    if ((Random(0x2) == 0))
        goto L_6455;
    else
        goto L_6397;

L_6397:
    j = (uint16_t)(pplr->pctIdealGrowth);
    if ((j <= 1))
        goto L_63f5;
    else
        goto L_63aa;

L_63aa:
    pplr->pctIdealGrowth = LOBYTE((j + 0xffff));
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_63de : L_63d6 */

L_63d6:
    /* untranslated: t_merge_63e4_0001 = (cPts neg 0) */
    goto L_63e4;

L_63de:
    t_merge_63e4_0001 = (cPts - 50);

L_63e4:
    dAwayNew = t_merge_63e4_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_63ef;

L_63ef:

L_63f5:
    if ((j >= 15))
        goto L_6449;
    else
        goto L_63fe;

L_63fe:
    pplr->pctIdealGrowth = LOBYTE((j + 0x1));
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_6432 : L_642a */

L_642a:
    /* untranslated: t_merge_6438_0001 = (cPts neg 0) */
    goto L_6438;

L_6432:
    t_merge_6438_0001 = (cPts - 50);

L_6438:
    dAwayNew = t_merge_6438_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_6443;

L_6443:

L_6449:
    pplr->pctIdealGrowth = LOBYTE(j);
    goto L_6072;

L_6455:
    iVal = Random(0x3);
    if (((uint16_t)(pplr->rgEnvVar[iVal]) >= 0))
        goto L_6539;
    else
        goto L_647c;

L_647c:
    j = Random(0x1f);
    pplr->rgEnvVar[iVal] = LOBYTE((j + 0x23));
    pplr->rgEnvVarMin[iVal] = LOBYTE(j);
    pplr->rgEnvVarMax[iVal] = LOBYTE((j + 0x46));
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_64ef : L_64e7 */

L_64e7:
    /* untranslated: t_merge_64f5_0001 = (cPts neg 0) */
    goto L_64f5;

L_64ef:
    t_merge_64f5_0001 = (cPts - 50);

L_64f5:
    dAwayNew = t_merge_64f5_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_6500;

L_6500:

L_6506:
    pplr->rgEnvVarMax[iVal] = -1;
    pplr->rgEnvVarMin[iVal] = -1;
    pplr->rgEnvVar[iVal] = -1;
    goto L_6072;

L_6539:
    plrT = *(pplr);
    pplr->rgEnvVarMax[iVal] = -1;
    pplr->rgEnvVarMin[iVal] = -1;
    pplr->rgEnvVar[iVal] = -1;
    cPts = CAdvantagePoints(pplr);
    /* untranslated: branch (cPts neg 0) <= (cPts - 50) ? L_65a3 : L_659b */

L_659b:
    /* untranslated: t_merge_65a9_0001 = (cPts neg 0) */
    goto L_65a9;

L_65a3:
    t_merge_65a9_0001 = (cPts - 50);

L_65a9:
    dAwayNew = t_merge_65a9_0001;
    if ((dAwayNew < dAwayCur))
        goto L_6072;
    else
        goto L_65b4;

L_65b4:

L_65ba:
    *(pplr) = plrT;

L_65ce:
    return;
}

int16_t PctTrueMaxGrowth(int16_t iplr) {
L_65d4:
    if ((GetRaceStat(rgplr[iplr], rsMajorAdv) != raCheapCol))
        goto L_6614;
    else
        goto L_65fd;

L_65fd:
    return ((uint16_t)(rgplr[iplr].pctIdealGrowth) * 2);

L_6614:
    return (uint16_t)(rgplr[iplr].pctIdealGrowth);
}
