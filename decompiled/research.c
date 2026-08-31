#include "common.h"

uint16_t rggrbitBrParts[17] = {6655, 8, 16, 64, 2048, 1, 4096, 256, 128, 512, 32768, 2, 4, 16384, 1024, 8192, 32};
int32_t  rglTechCost[27] = {0,     50,    80,    130,   210,   340,   550,   890,   1440,  2330,  3770,  6100,  9870, 13850,
                            18040, 22440, 27050, 31870, 36900, 42140, 47590, 53250, 59120, 65200, 71490, 77990, 84700};

int16_t ResearchDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    int16_t     y;
    int16_t     i;
    PAINTSTRUCT ps;
    int16_t     dx;
    RECT        rc;
    HWND        hwndRad;
    int16_t     dxCurrent;
    PLANET     *lppl;
    int16_t     c;
    PLANET     *lpplMac;
    HFONT       hfontSav;
    char       *psz;
    RECT        rcWindow;
    POINT       pt;
    int16_t     iResTechNext;
    int16_t     fChg;

L_0000:
    goto L_08b8;

L_000f:
    pctResGlob = (uint16_t)(rgplr[idPlayer].pctResearch);
    iResTechNow = ((uint16_t)(rgplr[idPlayer].iTechCur) & 0xf);
    CheckRadioButton(hwnd, 0x431, 0x436, (iResTechNow + 0x431));
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0x1]);
    c = CchGetString(idsCurrent, szWork);
    dxCurrent = LOWORD(GetTextExtent(hdc, szWork, c));
    dxResRadio = 0x0;
    i = 0x431;
    goto L_0114;

L_009f:
    c = CchGetString((i + 0xfc23), szWork);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    if ((dx <= dxResRadio))
        goto L_00db;
    else
        goto L_00d5;

L_00d5:
    dxResRadio = dx;

L_00db:
    hwndRad = GetDlgItem(hwnd, i);
    SetWindowText(hwndRad, szWork);
    SendMessage(hwndRad, WM_SETFONT, rghfontArial8[0x1], 0x0);
    i = (i + 0x1);

L_0114:
    if ((i <= 0x436))
        goto L_009f;
    else
        goto L_011e;

L_011e:
    dxResRadio = (dxResRadio + 0x40);
    y = (((dyArial8 * 0x4) - (dyArial8 >> 0x1)) + 0x1);
    i = 0x431;
    goto L_018b;

L_0140:
    hwndRad = GetDlgItem(hwnd, i);
    /* untranslated: call SetWindowPos(hwndRad, 0x0, 0x10, y, dxResRadio, (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2), 0x44)
     * -> callresult(int16_t) */
    /* untranslated: y = (y + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    i = (i + 0x1);

L_018b:
    if ((i <= 0x436))
        goto L_0140;
    else
        goto L_0195;

L_0195:
    hwndRad = GetDlgItem(hwnd, 0x43b);
    i = 0x0;
    goto L_01d9;

L_01ac:
    psz = PszGetCompressedString((i + 0x53));
    /* untranslated: call SendMessage(hwndRad, CB_ADDSTRING, 0x0, words(ds, psz)) -> callresult(LRESULT) */
    i = (i + 0x1);

L_01d9:
    if ((i <= 0x7))
        goto L_01ac;
    else
        goto L_01e2;

L_01e2:
    i = ((uint16_t)(rgplr[idPlayer].iTechCur) >> 0x4);
    if ((i != 0x6))
        goto L_020e;
    else
        goto L_0206;

L_0206:
    i = 0x0;
    goto L_021b;

L_020e:
    if ((i >= 0x6))
        goto L_021b;
    else
        goto L_0217;

L_0217:
    i = (i + 0x1);

L_021b:
    SendMessage(hwndRad, CB_SETCURSEL, i, 0x0);
    dxResLeft = ((dxResRadio + dxCurrent) + 0x28);
    dxResRight = 0x0;
    i = 0x4c;
    goto L_0289;

L_024d:
    c = CchGetString(i, szWork);
    dx = LOWORD(GetTextExtent(hdc, szWork, c));
    if ((dx <= dxResRight))
        goto L_0285;
    else
        goto L_027f;

L_027f:
    dxResRight = dx;

L_0285:
    i = (i + 0x1);

L_0289:
    if ((i <= 0x51))
        goto L_024d;
    else
        goto L_0292;

L_0292:
    dxResStrRight = dxResRight;
    dx = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsN9999992), 0x7));
    dxResRight = (dxResRight + (dx + 0x30));
    y = (y + LOWORD((0xa * dyArial8)));
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDCANCEL), 0x0, ((dxResLeft + dxResRight) + 0xff68), ((y - (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2)) + 0xfff8), 0x46, (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2), 0x44) ->
     * callresult(int16_t) */
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDC_HELP), 0x0, ((dxResLeft + dxResRight) + 0xffb4), ((y - (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2)) + 0xfff8), 0x46, (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2), 0x44) ->
     * callresult(int16_t) */
    GetWindowRect(hwnd, &(rcWindow));
    GetClientRect(hwnd, &(rc));
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, ((((dxResLeft + dxResRight) + rcWindow.right) - rcWindow.left) - rc.right),
                 (((y + rcWindow.bottom) - rcWindow.top) - rc.bottom), 0x6);
    StickyDlgPos(hwnd, &(ptStickyResDlg), 0x1);
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    lResTotal = 0x0;
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_044a;

L_0419:
    if ((lppl->iPlayer != idPlayer))
        goto L_0446;
    else
        goto L_0425;

L_0425:

L_042b:
    lResTotal = (lResTotal + (uint32_t)(CResourcesAtPlanet(lppl, idPlayer)));

L_0446:
    lppl = (lppl + 0x1);

L_044a:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0419;
    else
        goto L_0458;

L_0458:
    lResBudget = ProjectedResearchSpending((uint32_t)(pctResGlob));
    if ((gd.fTutorial == 0x0))
        goto L_0485;
    else
        goto L_0480;

L_0480:
    AdvanceTutor();

L_0485:
    return 0x1;

L_048b:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_04b3:
    i = 0x431;
    goto L_04e0;

L_04bb:
    /* untranslated: ss:[bp-0x34] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x34] == GetDlgItem(hwnd, i) ? L_04ea : L_04d6 */

L_04d6:

L_04dc:
    i = (i + 0x1);

L_04e0:
    if ((i <= 0x436))
        goto L_04bb;
    else
        goto L_04ea;

L_04ea:
    if ((i <= 0x436))
        goto L_0510;
    else
        goto L_04f4;

L_04f4:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_08fb;
    else
        goto L_0510;

L_0510:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_0529:
    return FTrackResearchDlg(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), wParam);

L_0557:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DrawResearchDlg(hwnd, hdc, &(rc), 0xffff);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_05a3:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.x <= 0xc))
        goto L_0605;
    else
        goto L_05c7;

L_05c7:
    if ((pt.x >= (dxResLeft + 0xfff4)))
        goto L_0605;
    else
        goto L_05d5;

L_05d5:
    if ((pt.y < yTopFutureTech))
        goto L_0605;
    else
        goto L_05e0;

L_05e0:
    if ((pt.y >= (LOWORD((cFutureTech * dyArial8)) + yTopFutureTech)))
        goto L_0605;
    else
        goto L_05f3;

L_05f3:
    SetCursor(hcurArrowHelp);
    return 0x1;

L_0605:
    if ((yTopTechNote == 0xffff))
        goto L_0645;
    else
        goto L_060f;

L_060f:
    if ((pt.y < yTopTechNote))
        goto L_0645;
    else
        goto L_061a;

L_061a:
    if ((pt.y >= ((dyArial8 * 0x2) + yTopTechNote)))
        goto L_0645;
    else
        goto L_062b;

L_062b:
    if ((pt.x <= dxResLeft))
        goto L_0645;
    else
        goto L_0636;

L_0636:
    SetCursor(hcurArrowHelp);
    return 0x1;

L_0645:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_06bb;
    else
        goto L_0661;

L_0661:
    if ((wParam < 0x431))
        goto L_06bb;
    else
        goto L_066b;

L_066b:
    if ((wParam > 0x436))
        goto L_06bb;
    else
        goto L_0675;

L_0675:
    if ((IsDlgButtonChecked(hwnd, wParam) == 0x0))
        goto L_08fb;
    else
        goto L_0689;

L_0689:
    iResTechNow = (wParam + 0xfbcf);
    GetClientRect(hwnd, &(rc));
    DrawResearchDlg(hwnd, 0x0, &(rc), 0x4);

L_06bb:
    if ((wParam != 0x2))
        goto L_0888;
    else
        goto L_06c4;

L_06c4:
    fChg = 0x0;
    iResTechNext = LOWORD(SendMessage(GetDlgItem(hwnd, 0x43b), CB_GETCURSEL, 0x0, 0x0));
    if ((iResTechNext != 0x0))
        goto L_06ff;
    else
        goto L_06f7;

L_06f7:
    iResTechNext = 0x6;
    goto L_070c;

L_06ff:
    if ((iResTechNext > 0x6))
        goto L_070c;
    else
        goto L_0708;

L_0708:
    iResTechNext = (iResTechNext - 0x1);

L_070c:
    if ((iResTechNow != ((uint16_t)(rgplr[idPlayer].iTechCur) & 0xf)))
        goto L_0761;
    else
        goto L_0728;

L_0728:
    if ((iResTechNext != ((uint16_t)(rgplr[idPlayer].iTechCur) >> 0x4)))
        goto L_0761;
    else
        goto L_0748;

L_0748:
    if ((pctResGlob == (uint16_t)(rgplr[idPlayer].pctResearch)))
        goto L_0843;
    else
        goto L_0761;

L_0761:
    rgplr[idPlayer].pctResearch = LOBYTE(pctResGlob);
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xfff0) | iResTechNow));
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xff0f) | (iResTechNext * 0x10)));
    i = (((uint16_t)(rgplr[idPlayer].iTechCur) * 0x100) + pctResGlob);
    WriteMemRt(0x22, 0x2, i);
    fChg = 0x1;
    if ((gd.fTutorial == 0x0))
        goto L_0843;
    else
        goto L_0828;

L_0828:
    if ((idPlayer != 0x0))
        goto L_0843;
    else
        goto L_0832;

L_0832:
    tutor.fChange = 0x1;
    AdvanceTutor();

L_0843:
    StickyDlgPos(hwnd, &(ptStickyResDlg), 0x0);
    EndDialog(hwnd, fChg);
    pctResGlob = 0xffff;
    if ((gd.fTutorial == 0x0))
        goto L_087f;
    else
        goto L_087a;

L_087a:
    AdvanceTutor();

L_087f:
    return 0x1;

L_0888:
    if ((wParam != 0x76))
        goto L_08fb;
    else
        goto L_0891;

L_0891:
    WinHelp(hwnd, szHelpFile, 0x1, 0x42e);
    return 0x1;

L_08b8:
    if ((message == WM_PAINT))
        goto L_0557;
    else
        goto L_08c0;

L_08c0:
    if ((message == WM_ERASEBKGND))
        goto L_048b;
    else
        goto L_08c8;

L_08c8:
    if ((message == WM_CTLCOLOR))
        goto L_04b3;
    else
        goto L_08d0;

L_08d0:
    if ((message == WM_SETCURSOR))
        goto L_05a3;
    else
        goto L_08d8;

L_08d8:
    if ((message == WM_INITDIALOG))
        goto L_000f;
    else
        goto L_08e0;

L_08e0:
    if ((message == WM_COMMAND))
        goto L_0645;
    else
        goto L_08e8;

L_08e8:
    if ((message == WM_LBUTTONDOWN))
        goto L_0529;
    else
        goto L_08f0;

L_08f0:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_0529;
    else
        goto L_08fb;

L_08fb:
    return 0x0;
}

void DrawResearchDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t grbitDraw) {
    int16_t  dxCurrent;
    char     szTemp[60];
    RECT     rcT;
    int16_t  iMax;
    int16_t  iTechSav;
    int16_t  iter;
    int16_t  fCreatedDC;
    int16_t  mdAvail;
    int16_t  i;
    int16_t  c;
    int16_t  grbitCur;
    COLORREF crBackSav;
    COLORREF crForeSav;
    HFONT    hfontSav;
    int16_t  xNum;
    int16_t  xCtr;
    int16_t  dx;
    char     szTemp2[60];
    PART     part;
    int32_t  l;
    int16_t  iMin;
    RECT     rc;
    int32_t  lSpent;
    HBRUSH   hbrSav;
    int32_t  lRBEffective;
    int16_t  cch;
    uint32_t t_merge_10a5_0001_wide;
    uint16_t t_merge_1406_0001;

L_090a:
    fCreatedDC = 0x0;
    if ((hdc != 0x0))
        goto L_0931;
    else
        goto L_0921;

L_0921:
    fCreatedDC = 0x1;
    hdc = GetDC(hwnd);

L_0931:
    hfontSav = SelectObject(hdc, rghfontArial8[0x1]);
    crForeSav = SetTextColor(hdc, 0x0);
    crBackSav = SetBkColor(hdc, crButtonFace);
    if (((grbitDraw & 0xff) == 0x0))
        goto DrawRightSide;
    else
        goto L_0977;

L_0977:

L_097d:
    c = CchGetString(idsCurrent, szWork);
    dxCurrent = LOWORD(GetTextExtent(hdc, szWork, c));
    SetRect(&(rc), 0x8, dyArial8, (dxResLeft + 0xfff8), (LOWORD((0xc * dyArial8)) + dyArial8));
    if (((grbitDraw & 0x3) == 0x0))
        goto DrawComingAttractions;
    else
        goto L_09d8;

L_09d8:

L_09de:
    _Draw3dFrame(hdc, &(rc), 0xffff);
    c = CchGetString(idsTechnologyStatus, szWork);
    TextOut(hdc, (rc.left + 0x8), (rc.top - (dyArial8 >> 0x1)), szWork, c);
    rc.top = (rc.top + (dyArial8 >> 0x1));
    c = CchGetString(idsCurrent, szWork);
    TextOut(hdc, ((rc.right + 0xfff8) - dxCurrent), rc.top, szWork, c);
    rc.top = (rc.top + dyArial8);
    i = CchGetString(idsFieldStudy, szWork);
    TextOut(hdc, (rc.left + 0x1a), rc.top, szWork, i);
    xCtr = ((rc.right + 0xfff8) - (dxCurrent >> 0x1));
    i = CchGetString(idsLevel, szWork);
    CtrTextOut(hdc, xCtr, rc.top, szWork, i);
    PatBlt(hdc, (rc.left + 0x8), (rc.top + dyArial8), ((rc.right - rc.left) + 0xfff0), 0x1, BLACKNESS);
    rc.top = (rc.top + (((dyArial8 >> 0x2) + dyArial8) + 0x2));
    i = 0x0;
    goto L_0b89;

L_0b28:
    c = _wsprintf(szWork, PCTD, (uint16_t)(rgplr[idPlayer].rgTech[i]));
    CtrTextOut(hdc, xCtr, rc.top, szWork, c);
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    i = (i + 0x1);

L_0b89:
    if ((i < 0x6))
        goto L_0b28;
    else
        goto DrawComingAttractions;

DrawComingAttractions:
    /* untranslated: call SetRect(&rc, 0x8, ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + rc.bottom), (dxResLeft + 0xfff8),
     * (prc->bottom + 0xfff8)) -> callresult(void) */
    if (((grbitDraw & 0x4) == 0x0))
        goto L_0c1b;
    else
        goto L_0bd4;

L_0bd4:
    if (((grbitDraw & 0x3) != 0x0))
        goto L_0c1b;
    else
        goto L_0be2;

L_0be2:
    /* untranslated: call ExpandRc(&rc, 0xfffa, ((dyArial8 >> 0x1) neg 0x0)) -> callresult(void) */
    rc.top = (rc.top + (dyArial8 >> 0x1));
    InvalidateRect(hwnd, &(rc), 0x1);
    goto DrawRightSide;

L_0c1b:
    _Draw3dFrame(hdc, &(rc), 0xffff);
    c = CchGetString(idsExpectedResearchBenefits, szWork);
    TextOut(hdc, (rc.left + 0x8), (rc.top - (dyArial8 >> 0x1)), szWork, c);
    iTechSav = ((uint16_t)(rgplr[idPlayer].iTechCur) & 0xf);
    cFutureTech = 0x0;
    yTopFutureTech = (rc.top + dyArial8);
    iter = 0x1;
    goto L_0e9d;

L_0c97:
    iMax = iter;
    iMin = iter;
    if ((iter != 0x1))
        goto L_0cbd;
    else
        goto L_0caa;

L_0caa:
    SetTextColor(hdc, 0x7f00);
    goto L_0d00;

L_0cbd:
    if ((iter < 0x2))
        goto L_0ce2;
    else
        goto L_0cc6;

L_0cc6:
    if ((iter > 0x4))
        goto L_0ce2;
    else
        goto L_0ccf;

L_0ccf:
    SetTextColor(hdc, 0x7f0000);
    goto L_0d00;

L_0ce2:
    SetTextColor(hdc, 0x0);
    if ((iter != 0x9))
        goto L_0d00;
    else
        goto L_0cfb;

L_0cfb:
    iMax = 0x1a;

L_0d00:
    grbitCur = 0x1;
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xfff0) | iResTechNow));
    goto L_0e90;

L_0d36:
    if (((grbitCur & 0xffff) == 0x0))
        goto L_0e8a;
    else
        goto L_0d44;

L_0d44:
    i = 0x0;
    part.hs.grhst = grbitCur;

L_0d53:
    part.hs.iItem = i;
    FLookupPart(&(part));
    /* untranslated: mdAvail = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) == 0x0 ? L_0e8a : L_0d7c */

L_0d7c:

L_0d82:
    if ((part.hs.grhst != hstTerra))
        goto L_0dde;
    else
        goto L_0d8d;

L_0d8d:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceTT) == 0x0))
        goto L_0dde;
    else
        goto L_0dae;

L_0dae:
    if ((part.hs.iItem == iterraGravityTerraform3))
        goto L_0e8a;
    else
        goto L_0dbd;

L_0dbd:
    if ((part.hs.iItem == iterraTempTerraform3))
        goto L_0e8a;
    else
        goto L_0dcc;

L_0dcc:
    if ((part.hs.iItem == iterraRadiationTerraform3))
        goto L_0e8a;
    else
        goto L_0dd8;

L_0dd8:

L_0dde:
    if ((iMin > (mdAvail + 0xffff)))
        goto L_0e83;
    else
        goto L_0ded;

L_0ded:
    if ((iMax < (mdAvail + 0xffff)))
        goto L_0e83;
    else
        goto L_0dfb;

L_0dfb:
    rc.top = (rc.top + dyArial8);
    if (((rc.top + dyArial8) > rc.bottom))
        goto TooManyToFinish;
    else
        goto L_0e10;

L_0e10:

L_0e16:
    /* untranslated: call fstrcpy(szWork, &part[8:0](part)) -> callresult(char *) */
    TextOut(hdc, (rc.left + 0x8), rc.top, szWork, strlen(szWork));
    if ((cFutureTech >= 0x8))
        goto L_0e83;
    else
        goto L_0e66;

L_0e66:
    cFutureTech = (cFutureTech + 0x1);
    rghsFutureTech[cFutureTech].grhst = part.hs.grhst;
    HIWORD(rghsFutureTech[cFutureTech]) = HIWORD(part.hs);

L_0e83:
    i = (i + 0x1);

L_0e8a:
    grbitCur = (grbitCur * 0x2);

L_0e90:
    if ((grbitCur != 0x0))
        goto L_0d36;
    else
        goto L_0e99;

L_0e99:
    iter = (iter + 0x1);

L_0e9d:
    if ((iter < 0xa))
        goto L_0c97;
    else
        goto TooManyToFinish;

TooManyToFinish:
    rgplr[idPlayer].iTechCur = LOBYTE((((uint16_t)(rgplr[idPlayer].iTechCur) & 0xfff0) | iTechSav));
    SetTextColor(hdc, 0x0);

DrawRightSide:
    /* untranslated: call SetRect(&rc, (dxResLeft + 0x8), dyArial8, ((dxResLeft + dxResRight) + 0xfff8), ((words(loword((0xe * dyArial8)),
     * signhiword(loword((0xe * dyArial8)))) / 0x2) + dyArial8)) -> callresult(void) */
    xCtr = ((rc.left + 0x8) + dxResStrRight);
    /* untranslated: ss:[bp-0xbc] = xCtr */
    /* untranslated: xNum = (loword(GetTextExtent(hdc, "999999", 0x6)) + ss:[bp-0xbc]) */
    if (((grbitDraw & 0x4) == 0x0))
        goto L_0f9e;
    else
        goto L_0f57;

L_0f57:
    if (((grbitDraw & 0x3) != 0x0))
        goto L_0f9e;
    else
        goto L_0f65;

L_0f65:
    /* untranslated: call ExpandRc(&rc, 0xfffa, ((dyArial8 >> 0x1) neg 0x0)) -> callresult(void) */
    rc.top = (rc.top + (dyArial8 >> 0x1));
    InvalidateRect(hwnd, &(rc), 0x1);
    goto CleanUp;

L_0f9e:
    if (((grbitDraw & 0x300) == 0x0))
        goto DrawResourceAlloc;
    else
        goto L_0fa9;

L_0fa9:

L_0faf:
    if (((uint16_t)(rgplr[idPlayer].rgTech[iResTechNow]) < 0x1a))
        goto L_0fdf;
    else
        goto L_0fd0;

L_0fd0:
    l = 0xffffffff;
    goto L_10ad;

L_0fdf:
    l = GetTechLevelCost(iResTechNow, ((uint16_t)(rgplr[idPlayer].rgTech[iResTechNow]) + 0x1), idPlayer);
    lSpent = rgplr[idPlayer].rgResSpent[iResTechNow];
    if ((game.fSlowTech == 0x0))
        goto L_1063;
    else
        goto L_104b;

L_104b:
    lSpent = (int32_t)((lSpent * 0x2));

L_1063:
    if ((0x0 < (HIWORD(l) - HIWORD(lSpent))))
        goto L_1095;
    else
        goto L_1080;

L_1080:
    if ((0x0 > (HIWORD(l) - HIWORD(lSpent))))
        goto L_108c;
    else
        goto L_1085;

L_1085:
    if ((0x0 <= (LOWORD(l) - LOWORD(lSpent))))
        goto L_1095;
    else
        goto L_108c;

L_108c:
    t_merge_10a5_0001_wide = 0x0;
    goto L_10a5;

L_1095:
    t_merge_10a5_0001_wide = (l - lSpent);

L_10a5:
    l = t_merge_10a5_0001_wide;

L_10ad:
    if (((grbitDraw & 0x200) == 0x0))
        goto L_1121;
    else
        goto L_10bb;

L_10bb:
    if (((grbitDraw & 0xf) != 0x0))
        goto L_1121;
    else
        goto L_10c9;

L_10c9:
    /* untranslated: rc.top = (rc.top + (((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) * 0x2) + dyArial8)) */
    hbrSav = SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, xCtr, rc.top, (xNum - xCtr), dyArial8, PATCOPY);
    SelectObject(hdc, hbrSav);
    goto DrawYearComplete;

L_1121:
    _Draw3dFrame(hdc, &(rc), 0xffff);
    c = CchGetString(idsCurrentlyResearching, szWork);
    TextOut(hdc, (rc.left + 0x8), (rc.top - (dyArial8 >> 0x1)), szWork, c);
    rc.top = (rc.top + dyArial8);
    CchGetString((iResTechNow + 0x54), szTemp);
    CchGetString(idsSTechLevelD, szTemp2);
    /* untranslated: c = _wsprintf(szWork, szTemp2, szTemp, ss, (sext8to16(rgplr[idPlayer].rgTech[iResTechNow]) + 0x1)) */
    RightTextOut(hdc, xCtr, rc.top, szWork, c, 0x0);
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsResourcesNeededComplete), 0x0, 0x0);
    if ((LOWORD(l) != 0xffff))
        goto L_1256;
    else
        goto L_1236;

L_1236:
    if ((HIWORD(l) != 0xffff))
        goto L_1256;
    else
        goto L_1240;

L_1240:
    c = CchGetString(idsMaxed, szWork);
    goto L_1277;

L_1256:
    c = _wsprintf(szWork, PCTLD, LOWORD(l), HIWORD(l));

L_1277:
    TextOut(hdc, xCtr, rc.top, szWork, c);
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsEstimatedTimeCompletion), 0x0, 0x0);

DrawYearComplete:
    if ((LOWORD(l) != 0xffff))
        goto L_130b;
    else
        goto L_12d2;

L_12d2:
    if ((HIWORD(l) != 0xffff))
        goto L_130b;
    else
        goto L_12dc;

L_12dc:
    c = CchGetString(idsMaxed, szWork);
    TextOut(hdc, xCtr, rc.top, szWork, c);
    goto L_144a;

L_130b:
    if ((LOWORD(l) != 0x0))
        goto L_1331;
    else
        goto L_1315;

L_1315:
    if ((HIWORD(l) != 0x0))
        goto L_1331;
    else
        goto L_131f;

L_131f:
    l = 0x1;
    goto PrintYear;

L_1331:
    if ((LOWORD(lResBudget) != 0x0))
        goto L_1374;
    else
        goto L_133b;

L_133b:
    if ((HIWORD(lResBudget) != 0x0))
        goto L_1374;
    else
        goto L_1345;

L_1345:
    c = CchGetString(idsNever2, szWork);
    TextOut(hdc, xCtr, rc.top, szWork, c);
    goto L_144a;

L_1374:
    lRBEffective = lResBudget;
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceGeneralizedResearch) == 0x0))
        goto L_13bc;
    else
        goto L_13a4;

L_13a4:
    lRBEffective = (lRBEffective - (int32_t)((lRBEffective >> 0x1)));

L_13bc:
    /* untranslated: l = (int32_t)(words(((HIWORD(l) + HIWORD(lRBEffective)) + 0xffff), ((LOWORD(l) + LOWORD(lRBEffective)) + 0xffff)) / lRBEffective) */

PrintYear:
    if ((LOWORD(l) != 0x1))
        goto L_1403;
    else
        goto L_13f3;

L_13f3:
    if ((HIWORD(l) != 0x0))
        goto L_1403;
    else
        goto L_13fd;

L_13fd:
    t_merge_1406_0001 = 0x20;
    goto L_1406;

L_1403:
    t_merge_1406_0001 = 0x73;

L_1406:
    c = _wsprintf(szWork, PszGetCompressedString(idsLdYearC), LOWORD(l), HIWORD(l), t_merge_1406_0001);
    TextOut(hdc, xCtr, rc.top, szWork, c);

L_144a:
    RightTextOut(hdc, (xCtr + 0xffc4), ((rc.top + dyArial8) + 0x5), PszGetCompressedString(idsFieldResearch), 0x0, 0x0);
    GetClientRect(GetDlgItem(hwnd, 0x43b), &(rcT));
    MapWindowPoints(GetDlgItem(hwnd, 0x43b), hwnd, &(rcT), 0x2);
    if ((rcT.top == ((rc.top + dyArial8) + 0x2)))
        goto DrawResourceAlloc;
    else
        goto L_14c9;

L_14c9:
    SetWindowPos(GetDlgItem(hwnd, 0x43b), 0x0, (xCtr + 0xffc4), ((rc.top + dyArial8) + 0x2), ((rc.right - xCtr) + 0x32), LOWORD((0x9 * dyArial8)), 0xc);

DrawResourceAlloc:
    /* untranslated: rc.top = ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + rc.bottom) */
    rc.bottom = ((dyArial8 * 0x8) + rc.top);
    if (((grbitDraw & 0x400) == 0x0))
        goto DrawAnnualRes;
    else
        goto L_153a;

L_153a:

L_1540:
    _Draw3dFrame(hdc, &(rc), 0xffff);
    c = CchGetString(idsResourceAllocation, szWork);
    TextOut(hdc, (rc.left + 0x8), (rc.top - (dyArial8 >> 0x1)), szWork, c);

DrawAnnualRes:
    rc.top = (rc.top + dyArial8);
    if (((grbitDraw & 0x800) == 0x0))
        goto DrawTotalSpent;
    else
        goto L_159f;

L_159f:

L_15a5:
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsAnnualResourcesPlanets), 0x0, 0x0);
    c = _wsprintf(szWork, PCTLD, LOWORD(lResTotal), HIWORD(lResTotal));
    RightTextOut(hdc, xNum, rc.top, szWork, c, 0x0);

DrawTotalSpent:
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    if (((grbitDraw & 0x1000) == 0x0))
        goto DrawBudget;
    else
        goto L_1626;

L_1626:

L_162c:
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsTotalResourcesSpentResearchLastYear), 0x0, 0x0);
    c = _wsprintf(szWork, PCTLD, LOWORD(rgplr[idPlayer].lResLastYear), HIWORD(rgplr[idPlayer].lResLastYear));
    RightTextOut(hdc, xNum, rc.top, szWork, c, 0x0);

DrawBudget:
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    dx = 0x0;
    if (((grbitDraw & 0x2000) != 0x0))
        goto L_16f4;
    else
        goto L_16bf;

L_16bf:
    if (((grbitDraw & 0x4000) == 0x0))
        goto DrawResPct;
    else
        goto L_16cd;

L_16cd:
    if (((grbitDraw & 0xf) != 0x0))
        goto DrawResPct;
    else
        goto L_16db;

L_16db:
    dx = LOWORD(GetTextExtent(hdc, "100", 0x3));

L_16f4:
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsResourcesBudgetedResearch), 0x0, 0x0);

DrawResPct:
    c = _wsprintf(szWork, PCTD, pctResGlob);
    RightTextOut(hdc, xNum, rc.top, szWork, c, dx);
    if (((grbitDraw & 0x4000) == 0x0))
        goto L_1773;
    else
        goto L_1762;

L_1762:
    if (((grbitDraw & 0xf) == 0x0))
        goto DrawProjBudg;
    else
        goto L_176d;

L_176d:

L_1773:
    dx = LOWORD(GetTextExtent(hdc, "%", 0x1));
    TextOut(hdc, xNum, rc.top, "%", 0x1);
    rcSpinTop.left = ((xNum + dx) + 0x4);
    rcSpinTop.top = (rc.top + 0xfffc);
    rcSpinTop.right = (rcSpinTop.left + 0xf);
    rcSpinTop.bottom = (((dyArial8 >> 0x1) + rc.top) + 0x1);
    rcSpinBot = rcSpinTop;
    OffsetRect(rcSpinBot, 0x0, ((rcSpinTop.bottom - rcSpinTop.top) + 0xffff));
    DrawBtn(hdc, &(rcSpinTop), 0xa0, 0x0, 0x0);
    DrawBtn(hdc, &(rcSpinBot), 0xa1, 0x0, 0x0);

DrawProjBudg:
    /* untranslated: rc.top = (rc.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    if (((grbitDraw & 0x4000) == 0x0))
        goto L_1861;
    else
        goto L_1850;

L_1850:
    if (((grbitDraw & 0xf) == 0x0))
        goto DrawProjBudgData;
    else
        goto L_185b;

L_185b:

L_1861:
    RightTextOut(hdc, xCtr, rc.top, PszGetCompressedString(idsYearsProjectedResearchBudget), 0x0, 0x0);

DrawProjBudgData:
    c = _wsprintf(szWork, PCTLD, LOWORD(lResBudget), HIWORD(lResBudget));
    RightTextOut(hdc, xNum, rc.top, szWork, c, (xNum - xCtr));
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceGeneralizedResearch) != 0x0))
        goto L_190b;
    else
        goto L_18ea;

L_18ea:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceBleedingEdgeTech) == 0x0))
        goto L_1a44;
    else
        goto L_190b;

L_190b:
    cch = CchGetString(idsRaceHas, szTemp);
    SelectObject(hdc, rghfontArial8[0x1]);
    rc.top = (rc.top + LOWORD((0x3 * dyArial8)));
    rc.left = (dxResLeft + 0x8);
    yTopTechNote = rc.top;
    dx = LOWORD(GetTextExtent(hdc, szTemp, cch));
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceGeneralizedResearch) == 0x0))
        goto L_19d4;
    else
        goto L_197e;

L_197e:
    TextOut(hdc, rc.left, rc.top, szTemp, cch);
    c = CchGetString(idsGeneralizedResearch, szWork);
    TextOut(hdc, (rc.left + dx), rc.top, szWork, c);
    rc.top = (rc.top + (dyArial8 + 0x2));

L_19d4:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceBleedingEdgeTech) == 0x0))
        goto CleanUp;
    else
        goto L_19f5;

L_19f5:
    TextOut(hdc, rc.left, rc.top, szTemp, cch);
    c = CchGetString(idsBleedingEdgeTechnology, szWork);
    TextOut(hdc, (rc.left + dx), rc.top, szWork, c);

L_1a44:
    yTopTechNote = 0xffff;

CleanUp:
    SetBkColor(hdc, crBackSav);
    SetTextColor(hdc, crForeSav);
    SelectObject(hdc, hfontSav);
    if ((fCreatedDC == 0x0))
        goto L_1a85;
    else
        goto L_1a7a;

L_1a7a:
    ReleaseDC(hwnd, hdc);

L_1a85:
    return;
}

int16_t FTrackResearchDlg(HWND hwnd, int16_t x, int16_t y, int16_t fkb) {
    int16_t  bt;
    POINT    pt;
    int16_t  dChg;
    int16_t  i;
    int16_t  cNew;
    RECT    *prc;
    BTNT     btnt;
    RECT     rc;
    uint16_t t_merge_1cdc_0001;
    uint16_t t_merge_1d00_0001;

L_1a8c:
    pt.x = x;
    pt.y = y;
    /* untranslated: branch PtInRect(rcSpinTop, words(pt.y, pt.x)) == 0x0 ? L_1acd : L_1abb */

L_1abb:
    i = 0x1;
    prc = 0x275c;
    bt = 0xa0;
    goto L_1c54;

L_1acd:
    /* untranslated: branch PtInRect(rcSpinBot, words(pt.y, pt.x)) == 0x0 ? L_1af9 : L_1ae7 */

L_1ae7:
    i = 0xffff;
    prc = 0x2764;
    bt = 0xa1;
    goto L_1c54;

L_1af9:
    if ((y < yTopFutureTech))
        goto L_1b7f;
    else
        goto L_1b04;

L_1b04:
    if ((y >= (LOWORD((cFutureTech * dyArial8)) + yTopFutureTech)))
        goto L_1b7f;
    else
        goto L_1b17;

L_1b17:
    if ((x <= 0xc))
        goto L_1b7f;
    else
        goto L_1b20;

L_1b20:
    if ((x >= (dxResLeft + 0xfff4)))
        goto L_1b7f;
    else
        goto L_1b2e;

L_1b2e:
    i = ((uint32_t)((y - yTopFutureTech)) / dyArial8);
    HIWORD(GlobalPD) = rghsFutureTech[i].grhst;
    /* untranslated: part[4:2](GlobalPD) = HIWORD(rghsFutureTech[i]) */
    /* untranslated: call FLookupPart(&part[2:0](GlobalPD)) -> callresult(int16_t) */
    GlobalPD.grPopup = grPopupComponent;
    Popup(hwnd, x, y);
    return 0x1;

L_1b7f:
    if ((yTopTechNote == 0xffff))
        goto L_1c4e;
    else
        goto L_1b89;

L_1b89:
    if ((y < yTopTechNote))
        goto L_1c4e;
    else
        goto L_1b94;

L_1b94:
    if ((y >= (LOWORD((0x3 * dyArial8)) + yTopTechNote)))
        goto L_1c4e;
    else
        goto L_1ba7;

L_1ba7:
    if ((x <= dxResLeft))
        goto L_1c4e;
    else
        goto L_1bb2;

L_1bb2:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceGeneralizedResearch) == 0x0))
        goto L_1c15;
    else
        goto L_1bd3;

L_1bd3:
    /* untranslated: branch y < ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + yTopTechNote) ? L_1c0d : L_1bec */

L_1bec:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceBleedingEdgeTech) != 0x0))
        goto L_1c15;
    else
        goto L_1c0d;

L_1c0d:
    i = 0x144;
    goto L_1c1a;

L_1c15:
    i = 0x14c;

L_1c1a:
    /* untranslated: part[4:2](GlobalPD) = PszGetCompressedString(i) */
    HIWORD(GlobalPD) = dxResRight;
    GlobalPD.grPopup = grPopupString;
    Popup(hwnd, x, y);
    return 0x1;

L_1c4e:
    return 0x0;

L_1c54:
    GetClientRect(hwnd, &(rc));
    InitBtnTrack(&(btnt), hwnd, 0x0, prc, bt, 0x50, 0x0, 0x0, 0x0);
    if (((fkb & 0x4) == 0x0))
        goto L_1ca2;
    else
        goto L_1c9a;

L_1c9a:
    dChg = 0xa;
    goto L_1ca7;

L_1ca2:
    dChg = 0x1;

L_1ca7:
    if ((FTrackBtn(&(btnt)) == 0x0))
        goto L_1d42;
    else
        goto L_1cbb;

L_1cbb:
    cNew = (LOWORD((dChg * i)) + pctResGlob);
    if ((0x0 <= cNew))
        goto L_1cd9;
    else
        goto L_1cd3;

L_1cd3:
    t_merge_1cdc_0001 = 0x0;
    goto L_1cdc;

L_1cd9:
    t_merge_1cdc_0001 = cNew;

L_1cdc:
    if ((0x64 >= t_merge_1cdc_0001))
        goto L_1cec;
    else
        goto L_1ce6;

L_1ce6:
    t_merge_1d00_0001 = 0x64;
    goto L_1d00;

L_1cec:
    if ((0x0 <= cNew))
        goto L_1cfd;
    else
        goto L_1cf7;

L_1cf7:
    t_merge_1d00_0001 = 0x0;
    goto L_1d00;

L_1cfd:
    t_merge_1d00_0001 = cNew;

L_1d00:
    cNew = t_merge_1d00_0001;
    if ((cNew == pctResGlob))
        goto L_1ca7;
    else
        goto L_1d0e;

L_1d0e:
    pctResGlob = cNew;
    lResBudget = ProjectedResearchSpending((uint32_t)(pctResGlob));
    DrawResearchDlg(hwnd, btnt.hdc, &(rc), 0x4200);

L_1d42:
    return 0x1;
}

int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr) {
    int32_t lCost;
    int16_t i;
    int16_t cTech;

L_1dba:
    cTech = 0x0;
    i = 0x0;
    goto L_1def;

L_1dd0:
    cTech = (cTech + (uint16_t)(rgplr[iplr].rgTech[i]));
    i = (i + 0x1);

L_1def:
    if ((i < 0x6))
        goto L_1dd0;
    else
        goto L_1df8;

L_1df8:
    /* untranslated: LOWORD(lCost) = (loword((0xa * cTech)) + cs:[(iLevel * 0x4)+0x1d4e]) */
    /* untranslated: HIWORD(lCost) = (signhiword(loword((0xa * cTech))) + cs:[(iLevel * 0x4)+0x1d50]) */
    i = (GetRaceStat(rgplr[iplr], (iTech + 0x8)) + 0xffff);
    if ((i == 0x0))
        goto L_1e83;
    else
        goto L_1e40;

L_1e40:
    if ((i >= 0x0))
        goto L_1e6a;
    else
        goto L_1e49;

L_1e49:
    lCost = (lCost + (lCost - (int32_t)((lCost >> 0x2))));
    goto L_1e83;

L_1e6a:
    lCost = (int32_t)((lCost / 0x2));

L_1e83:
    if ((game.fSlowTech == 0x0))
        goto L_1ea7;
    else
        goto L_1e93;

L_1e93:
    lCost = (int32_t)((lCost * 0x2));

L_1ea7:

L_1eb0:
    return lCost;
}

int16_t BrowserDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    HMENU       hmenu;
    int16_t     i;
    int16_t     c;
    PAINTSTRUCT ps;
    HFONT       hfontSav;
    int16_t     dx;
    RECT        rc;
    HWND        hwndDD;
    int32_t     lSel;
    int16_t     md;
    int16_t     fShowAll;
    int16_t     fAllHsts;
    uint16_t    iItemStart;
    int16_t     iStart;
    int16_t     cIter;
    int16_t     iOff;
    uint16_t    t_merge_1f1d_0001;
    uint16_t    t_merge_203e_0001;
    uint16_t    t_merge_2090_0001;
    uint16_t    t_merge_20d1_0001;
    uint16_t    t_merge_2172_0001;
    uint16_t    t_merge_220d_0001;
    uint16_t    t_merge_2470_0001;
    uint32_t    t_merge_24db_0001;
    uint16_t    t_merge_25d7_0001;
    uint16_t    t_merge_2628_0001;
    uint16_t    t_merge_263e_0001;
    int16_t     t_2675;

L_1ed8:
    goto L_2835;

L_1ee7:
    hwndBrowser = hwnd;
    GetSystemMetrics(SM_CXDLGFRAME);
    if ((dyArial8 <= 0xe))
        goto L_1f1a;
    else
        goto L_1f14;

L_1f14:
    t_merge_1f1d_0001 = 0x28;
    goto L_1f1d;

L_1f1a:
    t_merge_1f1d_0001 = 0x0;

L_1f1d:
    /* untranslated: ss:[bp-0x3c] = GetSystemMetrics(SM_CYCAPTION) */
    /* untranslated: call SetWindowPos(hwnd, 0x0, 0x0, 0x0, ((t_merge_1f1d_0001 + 0x166) + ss:[bp-0x3a]), (((((((dyArial10 + 0x48) + loword((0xc * dyArial8))) +
     * 0x6) + loword((0x3 * dyArial8))) + 0x19) + (GetSystemMetrics(SM_CYDLGFRAME) * 0x2)) + ss:[bp-0x3c]), 0x6) -> callresult(int16_t) */
    StickyDlgPos(hwnd, &(ptStickyBrowserDlg), 0x1);
    hdc = GetDC(hwnd);
    hfontSav = SelectObject(hdc, rghfontArial8[0x1]);
    hwndDD = GetDlgItem(hwnd, IDC_U16_0x010B);
    c = GetDlgItemText(hwnd, IDC_U16_0x042E, szWork, 0x50);
    dx = (LOWORD(GetTextExtent(hdc, szWork, c)) + 0xe);
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x042E), 0x0, 0x6, 0x6, dx, (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 *
     * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDC_NEXT);
    if ((dyArial8 <= 0xe))
        goto L_203b;
    else
        goto L_2035;

L_2035:
    t_merge_203e_0001 = 0x28;
    goto L_203e;

L_203b:
    t_merge_203e_0001 = 0x0;

L_203e:
    /* untranslated: call SetWindowPos(callresult(HWND), 0x0, ((t_merge_203e_0001 + 0x15e) - dx), 0x6, dx, (words(loword((0x3 * dyArial8)),
     * signhiword(loword((0x3 * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    if ((dyArial8 <= 0xe))
        goto L_208d;
    else
        goto L_2087;

L_2087:
    t_merge_2090_0001 = 0x28;
    goto L_2090;

L_208d:
    t_merge_2090_0001 = 0x0;

L_2090:
    /* untranslated: call SetWindowPos(hwndDD, 0x0, (dx + 0xc), 0x6, (((t_merge_2090_0001 + 0x158) - ss:[bp-0x3a]) + 0xfff4), loword((0x12 * dyArial8)),
     * SWP_NOZORDER) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDCANCEL);
    if ((dyArial8 <= 0xe))
        goto L_20ce;
    else
        goto L_20c8;

L_20c8:
    t_merge_20d1_0001 = 0x28;
    goto L_20d1;

L_20ce:
    t_merge_20d1_0001 = 0x0;

L_20d1:
    /* untranslated: call SetWindowPos(callresult(HWND), 0x0, ((t_merge_20d1_0001 + 0x15e) - dx), (((((dyArial10 + 0x48) + loword((0xc * dyArial8))) + 0x6) +
     * (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) + 0x12), dx, (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 *
     * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t) */
    GetDlgItem(hwnd, 0x10a);
    if ((dyArial8 <= 0xe))
        goto L_216f;
    else
        goto L_2169;

L_2169:
    t_merge_2172_0001 = 0x28;
    goto L_2172;

L_216f:
    t_merge_2172_0001 = 0x0;

L_2172:
    /* untranslated: call SetWindowPos(callresult(HWND), 0x0, 0x6, (((((dyArial10 + 0x48) + loword((0xc * dyArial8))) + 0x6) + ss:[bp-0x3a]) + 0x12),
     * ((t_merge_2172_0001 + 0x158) - dx), (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2), SWP_NOZORDER) -> callresult(int16_t)
     */
    SelectObject(hdc, hfontSav);
    ReleaseDC(hwnd, hdc);
    if ((fBrowserValid != 0x0))
        goto L_21c2;
    else
        goto L_21b0;

L_21b0:
    vpartBrowser.hs.grhst = hstArmor;
    vpartBrowser.hs.iItem = 0x0;

L_21c2:
    FLookupPart(&(vpartBrowser));
    if ((dyArial8 <= 0xe))
        goto L_220a;
    else
        goto L_2204;

L_2204:
    t_merge_220d_0001 = 0x28;
    goto L_220d;

L_220a:
    t_merge_220d_0001 = 0x0;

L_220d:
    /* untranslated: hwndBrowserChild = CreateWindow(szBrowser, 0x0, 0x50000000, 0x6, ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) /
     * 0x2) + 0xc), (t_merge_220d_0001 + 0x158), (((dyArial10 + 0x48) + loword((0xc * dyArial8))) + 0x6), hwnd, 0x0, hInst, 0x0) */
    i = 0x43f;
    goto L_226b;

L_2248:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(i))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_226b:
    if ((i < 0x450))
        goto L_2248;
    else
        goto L_2275;

L_2275:
    SendMessage(hwndDD, CB_SETCURSEL, 0x0, 0x0);
    if ((gd.fTutorial == 0x0))
        goto L_22a5;
    else
        goto L_22a0;

L_22a0:
    AdvanceTutor();

L_22a5:
    return 0x1;

L_22ab:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_22d3:
    i = 0x10a;
    goto L_2300;

L_22db:
    /* untranslated: ss:[bp-0x38] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x38] == GetDlgItem(hwnd, i) ? L_230a : L_22f6 */

L_22f6:

L_22fc:
    i = (i + 0x1);

L_2300:
    if ((i <= 0x10b))
        goto L_22db;
    else
        goto L_230a;

L_230a:
    if ((i <= 0x10b))
        goto L_2330;
    else
        goto L_2314;

L_2314:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_2868;
    else
        goto L_2330;

L_2330:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_2349:
    hdc = BeginPaint(hwnd, &(ps));
    EndPaint(hwnd, &(ps));
    return 0x1;

L_2370:
    StickyDlgPos(hwnd, &(ptStickyBrowserDlg), 0x0);
    hwndBrowser = 0x0;
    fBrowserValid = 0x0;
    hmenu = GetASubMenu(hwndFrame, 0x5);
    CheckMenuItem(hmenu, 0x100, 0x0);
    goto L_2868;

L_23b5:
    if ((wParam != 0x2))
        goto L_242d;
    else
        goto L_23be;

L_23be:
    StickyDlgPos(hwnd, &(ptStickyBrowserDlg), 0x0);
    hwndBrowser = 0x0;
    fBrowserValid = 0x0;
    hmenu = GetASubMenu(hwndFrame, 0x5);
    CheckMenuItem(hmenu, 0x100, 0x0);
    EndDialog(hwnd, 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_2424;
    else
        goto L_241f;

L_241f:
    AdvanceTutor();

L_2424:
    return 0x1;

L_242d:
    if ((wParam != 0x10b))
        goto L_2572;
    else
        goto L_2437;

L_2437:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x1))
        goto L_2868;
    else
        goto L_2453;

L_2453:
    if ((IsDlgButtonChecked(hwnd, 0x10a) != 0x0))
        goto L_246d;
    else
        goto L_2467;

L_2467:
    t_merge_2470_0001 = 0x1;
    goto L_2470;

L_246d:
    t_merge_2470_0001 = 0x0;

L_2470:
    fShowAll = t_merge_2470_0001;
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x010B), CB_GETCURSEL, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_24b5;
    else
        goto L_24a4;

L_24a4:
    if ((HIWORD(lSel) < 0x0))
        goto L_2868;
    else
        goto L_24a9;

L_24a9:
    if ((LOWORD(lSel) < 0x0))
        goto L_2868;
    else
        goto L_24af;

L_24af:

L_24b5:
    if ((HIWORD(lSel) < 0x0))
        goto L_24d5;
    else
        goto L_24be;

L_24be:
    if ((HIWORD(lSel) > 0x0))
        goto L_24cc;
    else
        goto L_24c3;

L_24c3:
    if ((LOWORD(lSel) <= 0x1))
        goto L_24d5;
    else
        goto L_24cc;

L_24cc:
    t_merge_24db_0001 = lSel;
    goto L_24db;

L_24d5:
    t_merge_24db_0001 = 0x10000;

L_24db:
    /* untranslated: vpartBrowser.hs.grhst = cs:[loword((int32_t)(t_merge_24db_0001 * 0x2))+0x1eb6] */
    vpartBrowser.hs.iItem = 0x0;

L_24f9:
    md = FLookupPart(&(vpartBrowser));
    if ((md != 0x0))
        goto L_2520;
    else
        goto L_2511;

L_2511:
    vpartBrowser.pcom = 0x0;
    goto L_2557;

L_2520:
    if ((md == 0x1))
        goto L_2557;
    else
        goto L_2529;

L_2529:
    if ((fShowAll != 0x0))
        goto L_2557;
    else
        goto L_252f;

L_252f:

L_2535:
    /* untranslated: ss:[bp-0x40] = ((HIWORD(vpartBrowser.hs) + 0x1) & 0xff) */
    vpartBrowser.hs.iItem = 0x0;
    /* untranslated: HIWORD(vpartBrowser.hs) = (HIWORD(vpartBrowser.hs) | ss:[bp-0x40]) */
    goto L_24f9;

L_2557:
    InvalidateRect(hwndBrowserChild, 0x0, 0x1);

L_2572:
    if ((wParam == 0x42f))
        goto L_2586;
    else
        goto L_257c;

L_257c:
    if ((wParam != 0x42e))
        goto L_2868;
    else
        goto L_2586;

L_2586:
    iItemStart = vpartBrowser.hs.iItem;
    cIter = 0x0;
    lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x010B), CB_GETCURSEL, 0x0, 0x0);
    if ((LOWORD(lSel) != 0x0))
        goto L_25d4;
    else
        goto L_25c5;

L_25c5:
    if ((HIWORD(lSel) != 0x0))
        goto L_25d4;
    else
        goto L_25ce;

L_25ce:
    t_merge_25d7_0001 = 0x1;
    goto L_25d7;

L_25d4:
    t_merge_25d7_0001 = 0x0;

L_25d7:
    fAllHsts = t_merge_25d7_0001;
    i = 0x0;
    goto L_25fc;

L_25e2:
    /* untranslated: branch vpartBrowser.hs.grhst == cs:[(i * 0x2)+0x1eb6] ? L_2605 : L_25f2 */

L_25f2:

L_25f8:
    i = (i + 0x1);

L_25fc:
    if ((i < 0x11))
        goto L_25e2;
    else
        goto L_2605;

L_2605:
    iStart = i;
    if ((IsDlgButtonChecked(hwnd, 0x10a) != 0x0))
        goto L_2625;
    else
        goto L_261f;

L_261f:
    t_merge_2628_0001 = 0x1;
    goto L_2628;

L_2625:
    t_merge_2628_0001 = 0x0;

L_2628:
    fShowAll = t_merge_2628_0001;
    if ((wParam != 0x42f))
        goto L_263b;
    else
        goto L_2635;

L_2635:
    t_merge_263e_0001 = 0x1;
    goto L_263e;

L_263b:
    t_merge_263e_0001 = 0xffff;

L_263e:
    iOff = t_merge_263e_0001;

L_2641:
    /* untranslated: ss:[bp-0x4a] = ((iOff + HIWORD(vpartBrowser.hs)) & 0xff) */
    vpartBrowser.hs.iItem = 0x0;
    /* untranslated: HIWORD(vpartBrowser.hs) = (HIWORD(vpartBrowser.hs) | ss:[bp-0x4a]) */
    /* untranslated: branch ss:[bp-0x4a] != iItemStart ? L_2675 : L_266c */

L_266c:
    if ((fAllHsts == 0x0))
        goto L_27b3;
    else
        goto Top;

Top:
    t_2675 = cIter;
    cIter = (cIter + 0x1);
    if ((t_2675 > 0x15e))
        goto NullItem;
    else
        goto L_2681;

L_2681:

L_2687:
    md = FLookupPart(&(vpartBrowser));
    if ((md != 0x0))
        goto L_2775;
    else
        goto L_269f;

L_269f:
    if ((wParam != 0x42f))
        goto L_26e2;
    else
        goto L_26a9;

L_26a9:
    if ((fAllHsts == 0x0))
        goto L_26e2;
    else
        goto L_26b2;

L_26b2:
    i = (i + 0x1);
    if ((i < 0x11))
        goto L_26c6;
    else
        goto L_26c1;

L_26c1:
    i = 0x1;

L_26c6:
    /* untranslated: vpartBrowser.hs.grhst = cs:[(i * 0x2)+0x1eb6] */
    vpartBrowser.hs.iItem = 0x0;
    goto Top;

L_26e2:
    if ((vpartBrowser.hs.iItem != 0x0))
        goto L_2708;
    else
        goto L_26f0;

L_26f0:
    if ((fAllHsts != 0x0))
        goto L_2708;
    else
        goto L_26f9;

L_26f9:
    vpartBrowser.pcom = 0x0;
    goto L_27b3;

L_2708:
    if ((wParam != 0x42e))
        goto L_275c;
    else
        goto L_2712;

L_2712:
    if ((fAllHsts == 0x0))
        goto L_275c;
    else
        goto L_271b;

L_271b:
    if ((vpartBrowser.hs.iItem <= 0x64))
        goto L_2775;
    else
        goto L_2729;

L_2729:
    i = (i - 0x1);
    if ((i > 0x0))
        goto L_273d;
    else
        goto L_2738;

L_2738:
    i = 0x10;

L_273d:
    /* untranslated: vpartBrowser.hs.grhst = cs:[(i * 0x2)+0x1eb6] */
    vpartBrowser.hs.iItem = 0x64;
    goto Top;

L_275c:
    if ((wParam != 0x42f))
        goto L_2775;
    else
        goto L_2766;

L_2766:
    vpartBrowser.hs.iItem = 0x0;
    goto Top;

L_2775:
    if ((md == 0x1))
        goto L_27b3;
    else
        goto L_277e;

L_277e:
    if ((md == 0x0))
        goto L_2641;
    else
        goto L_2787;

L_2787:
    if ((fShowAll == 0x0))
        goto L_2641;
    else
        goto L_2790;

L_2790:
    if ((md != 0xffff))
        goto L_27b3;
    else
        goto L_2799;

L_2799:
    if ((FShouldPartBeHidden(&(vpartBrowser)) == 0x0))
        goto L_27b3;
    else
        goto L_27aa;

L_27aa:

L_27b3:
    if ((vpartBrowser.hs.iItem != iItemStart))
        goto L_27de;
    else
        goto L_27c1;

L_27c1:
    if ((iStart != i))
        goto L_27de;
    else
        goto L_27cc;

L_27cc:
    if ((md == 0x1))
        goto L_2868;
    else
        goto L_27d5;

L_27d5:
    if ((fShowAll != 0x0))
        goto L_2868;
    else
        goto L_27de;

L_27de:
    if ((vpartBrowser.hs.iItem != iItemStart))
        goto L_281a;
    else
        goto L_27ec;

L_27ec:
    if ((iStart != i))
        goto L_281a;
    else
        goto L_27f7;

L_27f7:
    if ((FLookupPart(&(vpartBrowser)) == 0x1))
        goto L_2868;
    else
        goto L_2808;

L_2808:

NullItem:
    vpartBrowser.pcom = 0x0;

L_281a:
    InvalidateRect(hwndBrowserChild, 0x0, 0x1);

L_2835:
    if ((message == WM_DESTROY))
        goto L_2370;
    else
        goto L_283d;

L_283d:
    if ((message == WM_PAINT))
        goto L_2349;
    else
        goto L_2845;

L_2845:
    if ((message == WM_ERASEBKGND))
        goto L_22ab;
    else
        goto L_284d;

L_284d:
    if ((message == WM_CTLCOLOR))
        goto L_22d3;
    else
        goto L_2855;

L_2855:
    if ((message == WM_INITDIALOG))
        goto L_1ee7;
    else
        goto L_285d;

L_285d:
    if ((message == WM_COMMAND))
        goto L_23b5;
    else
        goto L_2868;

L_2868:
    return 0x0;
}

int32_t BrowserWndProc(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    POINT       pt;
    int16_t     i;
    PAINTSTRUCT ps;
    RECT        rc;

L_2876:
    goto L_2a89;

L_2888:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    DisplayComponentInfo(hdc, rc.right, rc.bottom, &(vpartBrowser));
    EndPaint(hwnd, &(ps));
    goto L_2ab4;

L_28d0:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    goto Validate;

L_28ee:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_2919:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);

Validate:
    if ((vpartBrowser.hs.grhst == hstHull))
        goto L_294f;
    else
        goto L_2944;

L_2944:
    if ((vpartBrowser.hs.grhst != hstSBHull))
        goto Default;
    else
        goto L_294f;

L_294f:
    if ((hwndSlotDlg != 0x0))
        goto Default;
    else
        goto L_2959;

L_2959:
    if ((pt.x < 0x5))
        goto Default;
    else
        goto L_2962;

L_2962:
    if ((pt.x >= 0x45))
        goto Default;
    else
        goto L_296b;

L_296b:
    if ((pt.y < (dyArial10 + 0x5)))
        goto Default;
    else
        goto L_2979;

L_2979:
    if ((pt.y >= (dyArial10 + 0x45)))
        goto Default;
    else
        goto L_2984;

L_2984:

L_298a:
    if ((message != WM_LBUTTONDOWN))
        goto L_2a5d;
    else
        goto L_2994;

L_2994:
    GlobalPD.grPopup = grPopupShdefSB;
    if ((vpartBrowser.hs.grhst != hstSBHull))
        goto L_29cf;
    else
        goto L_29a5;

L_29a5:
    shdefBuild.hul = LphuldefSBFromId(vpartBrowser.hs.iItem)->hul;
    goto L_29f6;

L_29cf:
    shdefBuild.hul = LphuldefFromId(vpartBrowser.hs.iItem)->hul;

L_29f6:
    i = 0x0;
    goto L_2a02;

L_29fe:
    i = (i + 0x1);

L_2a02:
    /* untranslated: branch i >= (part[122:2](shdefBuild.hul) & 0xff) ? L_2a37 : L_2a10 */

L_2a10:
    HIWORD(shdefBuild.hul.rghs[i]) = ((HIWORD(shdefBuild.hul.rghs[i]) & 0xff) | 0x0);
    goto L_29fe;

L_2a37:
    HIWORD(GlobalPD) = 0x2436;
    /* untranslated: part[4:2](GlobalPD) = ds */
    /* untranslated: part[6:2](GlobalPD) = 0x0 */
    Popup(hwnd, pt.x, pt.y);
    goto L_2a66;

L_2a5d:
    SetCursor(hcurArrowHelp);

L_2a66:
    return 0x10000;

Default:
    return DefWindowProc(hwnd, message, wParam, lParam);

L_2a89:
    if ((message == WM_CREATE))
        goto L_2ab4;
    else
        goto L_2a91;

L_2a91:
    if ((message == WM_PAINT))
        goto L_2888;
    else
        goto L_2a99;

L_2a99:
    if ((message == WM_ERASEBKGND))
        goto L_28ee;
    else
        goto L_2aa1;

L_2aa1:
    if ((message == WM_SETCURSOR))
        goto L_28d0;
    else
        goto L_2aa9;

L_2aa9:
    if ((message != WM_LBUTTONDOWN))
        goto Default;
    else
        goto L_2aae;

L_2aae:

L_2ab4:
    return 0x0;
}

void DisplayComponentInfo(HDC hdc, int16_t dx, int16_t dy, PART *ppart) {
    uint16_t rgCosts[4];
    int16_t  idsT;
    StringId ids;
    int16_t  dxStr;
    int16_t  c;
    int16_t  yText;
    int16_t  i;
    int16_t  yCur;
    int16_t  fReq;
    int16_t  yStart;
    int16_t  xNum;
    int16_t  xText;
    RECT     rcData;
    int32_t  l;
    int16_t  dxT;
    char     rgch[2];
    int16_t  dyPct;
    int16_t  dxDigit;
    int16_t  yBase;
    int16_t  y;
    int16_t  dxWarp;
    int16_t  pct;
    int16_t  fWarp10;
    int16_t  iEff;
    COLORREF crFore;
    int16_t  cch;
    int16_t  x;
    HPEN     hpenSav;
    COLORREF crBack;
    HBRUSH   hbrSav;
    int16_t  pctT;
    char     szT[256];
    int16_t  dyText;
    char    *psz;
    int16_t  dmgFloor;
    int16_t  dmgMin;
    int16_t  dmgShipRam;
    int16_t  iWarp;
    int16_t  dmgShip;
    int16_t  pctHit;
    int16_t  dxLabel;
    int16_t  dmgMinRam;
    int32_t  lpct;
    int16_t  xBase;
    int16_t  dxQuan;
    char     ch;
    int32_t  ldelta;
    RECT     rcT;
    uint32_t t_merge_2e70_0001;
    uint16_t t_merge_335d_0001;
    uint16_t t_merge_379d_0001;
    uint16_t t_merge_39e7_0001;
    uint16_t t_merge_3bff_0001;
    uint16_t t_merge_4e19_0001;
    uint16_t t_merge_5016_0001;
    uint16_t t_merge_5094_0001;
    uint16_t t_merge_522a_0001;
    uint16_t t_merge_526a_0001;
    uint16_t t_merge_52aa_0001;
    uint16_t t_merge_578c_0001;
    uint16_t t_merge_6105_0001;
    uint32_t t_merge_6561_0001;

L_2ac6:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, 0x0, (dy + 0xffff), dx, 0x1, PATCOPY);
    PatBlt(hdc, (dx + 0xffff), 0x0, 0x1, dy, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, 0x0, 0x0, dx, 0x1, PATCOPY);
    PatBlt(hdc, 0x0, 0x0, 0x1, dy, PATCOPY);
    PatBlt(hdc, 0x4, ((dy - (dyArial8 * 0x2)) + 0xfffe), (dx + 0xfff8), 0x1, PATCOPY);
    SetBkMode(hdc, TRANSPARENT);
    if ((LOWORD(ppart->pcom) != 0x0))
        goto L_2c05;
    else
        goto L_2bae;

L_2bae:
    if ((HIWORD(ppart->pcom) != 0x0))
        goto L_2c05;
    else
        goto L_2bb7;

L_2bb7:
    SetRect(&(rcData), 0x5, 0x5, (dx + 0xfffb), (dy + 0xfffb));
    c = CchGetString(idsNoneAvailable, szWork);
    DiaganolTextOut(hdc, &(rcData), szWork, c);
    goto L_65a7;

L_2c05:
    SelectObject(hdc, rghfontArial10[0x1]);
    fstrcpy(szWork, &(ppart->pcom->szName));
    CtrTextOut(hdc, (dx >> 0x1), 0x3, szWork, 0x0);
    if ((ppart->hs.grhst == hstHull))
        goto L_2c65;
    else
        goto L_2c59;

L_2c59:
    if ((ppart->hs.grhst != hstSBHull))
        goto L_2ca4;
    else
        goto L_2c65;

L_2c65:
    DrawFleetBitmap(0x0, hdc, 0x5, (dyArial10 + 0x5), 0x0, ppart->pcom->ibmp, 0x0, 0x0, 0xffff, 0x0);
    goto L_2d35;

L_2ca4:
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, 0x5, (dyArial10 + 0x5), 0x40, 0x40, rghdibInventory[(ppart->pcom->ibmp >> 0x5)], ((ppart->pcom->ibmp & 0x7) * 0x40),
           (((0x3 - (ppart->pcom->ibmp >> 0x3)) & 0x3) * 0x40), 0x40, 0x40, 0xcc0020);

L_2d35:
    SelectObject(hdc, rghfontArial8[0x1]);
    yCur = (dyArial10 + 0x47);
    c = CchGetString(idsTechReq, szWork);
    TextOut(hdc, 0x5, yCur, szWork, c);
    dxStr = 0x0;
    i = 0x0;
    goto L_2dc3;

L_2d83:
    c = CchGetString((i + 0x5b), szWork);
    dxT = LOWORD(GetTextExtent(hdc, szWork, c));
    if ((dxT <= dxStr))
        goto L_2dbf;
    else
        goto L_2db9;

L_2db9:
    dxStr = dxT;

L_2dbf:
    i = (i + 0x1);

L_2dc3:
    if ((i < 0x6))
        goto L_2d83;
    else
        goto L_2dcc;

L_2dcc:
    dxStr = (dxStr + 0x5);
    xNum = (dxStr + LOWORD(GetTextExtent(hdc, "99", 0x2)));
    fReq = 0x0;
    i = 0x0;
    goto L_2f11;

L_2df8:
    if (((uint16_t)(ppart->pcom->rgTech[i]) <= 0x0))
        goto L_2f0d;
    else
        goto L_2e19;

L_2e19:
    yCur = (yCur + dyArial8);
    if (((uint16_t)(ppart->pcom->rgTech[i]) <= (uint16_t)(rgplr[idPlayer].rgTech[i])))
        goto L_2e6a;
    else
        goto L_2e61;

L_2e61:
    t_merge_2e70_0001 = 0x7f;
    goto L_2e70;

L_2e6a:
    t_merge_2e70_0001 = 0x0;

L_2e70:
    SetTextColor(hdc, t_merge_2e70_0001);
    c = CchGetString((i + 0x5b), szWork);
    RightTextOut(hdc, dxStr, yCur, szWork, c, 0x0);
    SetTextColor(hdc, 0x0);
    c = _wsprintf(szWork, PCTD, (uint16_t)(ppart->pcom->rgTech[i]));
    RightTextOut(hdc, xNum, yCur, szWork, c, 0x0);
    fReq = 0x1;

L_2f0d:
    i = (i + 0x1);

L_2f11:
    if ((i < 0x6))
        goto L_2df8;
    else
        goto L_2f1a;

L_2f1a:
    if ((fReq != 0x0))
        goto L_2f55;
    else
        goto L_2f23;

L_2f23:
    yCur = (yCur + dyArial8);
    c = CchGetString(idsNone3, szWork);
    CtrTextOut(hdc, 0x25, yCur, szWork, c);

L_2f55:
    if ((FLookupPart(ppart) > 0x0))
        goto L_2f75;
    else
        goto L_2f68;

L_2f68:
    l = 0xffffffff;
    goto L_2f93;

L_2f75:
    l = CostOfDevelopingItem(&(ppart->pcom->rgTech));

L_2f93:
    if ((HIWORD(l) < 0x1))
        goto L_300e;
    else
        goto L_2f9c;

L_2f9c:
    if ((HIWORD(l) > 0x1))
        goto L_2fab;
    else
        goto L_2fa1;

L_2fa1:
    if ((LOWORD(l) <= 0x869f))
        goto L_300e;
    else
        goto L_2fab;

L_2fab:
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsCostLdk), loword((int32_t)(words((HIWORD(l) + 0x0), (LOWORD(l) + 0x1f4)) / 0x3e8)),
     * hiword((int32_t)(words((HIWORD(l) + 0x0), (LOWORD(l) + 0x1f4)) / 0x3e8))) */
    TextOut(hdc, 0x5, ((yCur + dyArial8) + 0x4), szWork, c);
    goto L_30fc;

L_300e:
    if ((HIWORD(l) < 0x0))
        goto L_3071;
    else
        goto L_3017;

L_3017:
    if ((HIWORD(l) > 0x0))
        goto L_3025;
    else
        goto L_301c;

L_301c:
    if ((LOWORD(l) <= 0x0))
        goto L_3071;
    else
        goto L_3025;

L_3025:
    c = _wsprintf(szWork, PszGetCompressedString(idsCostLd), LOWORD(l), HIWORD(l));
    TextOut(hdc, 0x5, ((yCur + dyArial8) + 0x4), szWork, c);
    goto L_30fc;

L_3071:
    if ((LOWORD(l) != 0xffff))
        goto L_30d1;
    else
        goto L_307a;

L_307a:
    if ((HIWORD(l) != 0xffff))
        goto L_30d1;
    else
        goto L_3083;

L_3083:
    SetTextColor(hdc, 0x7f);
    CtrTextOut(hdc, 0x25, ((yCur + dyArial8) + 0x4), PszGetCompressedString(idsUnavail), 0x0);
    SetTextColor(hdc, 0x0);
    goto L_30fc;

L_30d1:
    CtrTextOut(hdc, 0x25, ((yCur + dyArial8) + 0x4), PszGetCompressedString(idsAvailable), 0x0);

L_30fc:
    yStart = (dyArial10 + 0x5);
    if (((dyArial8 * 0x4) >= 0x40))
        goto L_3125;
    else
        goto L_3114;

L_3114:
    yStart = (yStart + ((0x40 - (dyArial8 << 0x2)) >> 0x1));

L_3125:
    yCur = yStart;
    dxStr = 0x0;
    i = 0x0;
    goto L_3178;

L_3138:
    dxT = LOWORD(GetTextExtent(hdc, rgszMinerals[i], lstrlen(rgszMinerals[i])));
    if ((dxT <= dxStr))
        goto L_3174;
    else
        goto L_316e;

L_316e:
    dxStr = dxT;

L_3174:
    i = (i + 0x1);

L_3178:
    if ((i <= 0x5))
        goto L_3138;
    else
        goto L_3181;

L_3181:
    dxStr = (dxStr + 0x4d);
    GetTruePartCost(idPlayer, ppart, rgCosts);
    i = 0x0;
    goto L_32c9;

L_31a0:
    if ((i == 0x4))
        goto L_32c5;
    else
        goto L_31a9;

L_31a9:
    if ((i == 0x3))
        goto L_32c5;
    else
        goto L_31af;

L_31af:

L_31b5:
    SelectObject(hdc, rghfontArial8[0x1]);
    SetTextColor(hdc, rgcrMinerals[i]);
    RightTextOut(hdc, dxStr, yCur, rgszMinerals[i], 0x0, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    SetTextColor(hdc, crWindowText);
    if ((i == 0x5))
        goto L_3231;
    else
        goto L_321f;

L_321f:
    c = rgCosts[i];
    goto L_3237;

L_3231:
    c = rgCosts[0x3];

L_3237:
    if ((ppart->hs.grhst == hstSBHull))
        goto L_324f;
    else
        goto L_3243;

L_3243:
    if ((ppart->hs.grhst != hstSpecialSB))
        goto L_325b;
    else
        goto L_324f;

L_324f:
    c = (c - ((uint32_t)(c) / 0x2));

L_325b:
    c = _wsprintf(szWork, PCTD, c);
    RightTextOut(hdc, (dxStr + dxMaxMineralQuan), yCur, szWork, c, 0x0);
    if ((i >= 0x5))
        goto L_32bf;
    else
        goto L_32a1;

L_32a1:
    TextOut(hdc, (dxStr + dxMaxMineralQuan), yCur, "kT", 0x2);

L_32bf:
    yCur = (yCur + dyArial8);

L_32c5:
    i = (i + 0x1);

L_32c9:
    if ((i <= 0x5))
        goto L_31a0;
    else
        goto L_32d2;

L_32d2:
    SelectObject(hdc, rghfontArial8[0x1]);
    if ((ppart->pcom->cMass == 0x0))
        goto L_333a;
    else
        goto L_32ee;

L_32ee:
    c = _wsprintf(szWork, PszGetCompressedString(idsMassDkt), ppart->pcom->cMass);
    TextOut(hdc, ((dxStr + dxMaxMineralQuan) + 0x20), yStart, szWork, c);

L_333a:
    if ((0x49 <= (xNum + 0x4)))
        goto L_3357;
    else
        goto L_3351;

L_3351:
    t_merge_335d_0001 = 0x49;
    goto L_335d;

L_3357:
    t_merge_335d_0001 = (xNum + 0x4);

L_335d:
    SetRect(&(rcData), t_merge_335d_0001, (dyArial10 + 0x47), (dx + 0xfffb), (((dy + 0xfffb) - (dyArial8 * 0x2)) + 0xfffc));
    if ((dyArial8 <= 0xe))
        goto L_3390;
    else
        goto L_338c;

L_338c:
    rcData.left = (rcData.left + 0x4);

L_3390:
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, rcData.left, rcData.top, (rcData.right - rcData.left), 0x1, PATCOPY);
    PatBlt(hdc, rcData.left, rcData.top, 0x1, (rcData.bottom - rcData.top), PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rcData.left, (rcData.bottom + 0xffff), (rcData.right - rcData.left), 0x1, PATCOPY);
    PatBlt(hdc, (rcData.right + 0xffff), rcData.top, 0x1, (rcData.bottom - rcData.top), PATCOPY);
    ExpandRc(&(rcData), 0xfffd, 0xfffd);
    if ((gd.fBleedingEdge == 0x0))
        goto L_34b6;
    else
        goto L_345b;

L_345b:
    if ((LOWORD(l) != 0x0))
        goto L_34b6;
    else
        goto L_3464;

L_3464:
    if ((HIWORD(l) != 0x0))
        goto L_34b6;
    else
        goto L_346d;

L_346d:
    SetTextColor(hdc, crButtonHilite);
    c = CchGetString(idsBleedingEdge, szWork);
    DiaganolTextOut(hdc, &(rcData), szWork, c);
    SetTextColor(hdc, crWindowText);

L_34b6:
    ids = 0xffff;
    goto L_6451;

L_34c6:
    SelectObject(hdc, rghfontArial8[0x1]);
    rcData.right = (rcData.right - 0x2);
    c = CchGetString(idsFuelUsageVsWarpSpeed, szWork);
    CtrTextOut(hdc, (((rcData.right - rcData.left) >> 0x1) + rcData.left), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    c = CchGetString(idsWarp, szWork);
    dxStr = LOWORD(GetTextExtent(hdc, szWork, c));
    dxWarp = ((uint32_t)((((rcData.right - rcData.left) - dxStr) + 0xfff8)) / 0xa);
    dyPct = ((uint32_t)((((rcData.bottom - rcData.top) - dyArial8) + 0xfff8)) / 0x6);
    PatBlt(hdc, ((rcData.left + dxStr) + 0x6), rcData.top, 0x1, (((rcData.bottom - rcData.top) - dyArial8) + 0xfffc), BLACKNESS);
    PatBlt(hdc, ((rcData.left + dxStr) + 0x6), ((rcData.bottom - dyArial8) + 0xfffc), (((rcData.right - rcData.left) - dxStr) + 0xfffa), 0x1, BLACKNESS);
    x = (rcData.left + dxStr);
    y = (((rcData.bottom - dyArial8) + 0xfffc) - LOWORD((0x6 * dyPct)));
    pct = 0x320;
    SetTextColor(hdc, 0x7f);
    goto L_36be;

L_35fe:
    if ((pct != 0x64))
        goto L_364e;
    else
        goto L_3607;

L_3607:
    SetTextColor(hdc, 0x0);
    SelectObject(hdc, hbrGray);
    PatBlt(hdc, (x + 0x4), y, (((rcData.right - rcData.left) - dxStr) + 0xfffc), 0x1, PATCOPY);

L_364e:
    c = _wsprintf(szWork, PCTDPCTPCT, pct);
    RightTextOut(hdc, x, (y - (dyArial8 >> 0x1)), szWork, c, 0x0);
    PatBlt(hdc, (x + 0x4), y, 0x5, 0x1, BLACKNESS);
    pct = (pct >> 0x1);
    y = (y + dyPct);

L_36be:
    if ((pct >= 0x19))
        goto L_35fe;
    else
        goto L_36c7;

L_36c7:
    y = (rcData.bottom - dyArial8);
    RightTextOut(hdc, x, y, PszGetCompressedString(idsWarp), 0x0, 0x0);
    x = (x + 0x6);
    yBase = ((rcData.bottom - dyArial8) + 0xfffc);
    dxDigit = (LOWORD(GetTextExtent(hdc, "0", 0x1)) >> 0x1);
    rgch[0x0] = 0x30;
    cch = 0x1;
    crFore = SetTextColor(hdc, crButtonText);
    if ((ppart->hs.iItem == iengineInterspace10))
        goto L_3794;
    else
        goto L_3750;

L_3750:
    if ((ppart->hs.iItem == iengineTransStar10))
        goto L_3794;
    else
        goto L_3761;

L_3761:
    if ((ppart->hs.iItem == iengineTransGalacticMizerScoop))
        goto L_3794;
    else
        goto L_3772;

L_3772:
    if ((ppart->hs.iItem == iengineGalaxyScoop))
        goto L_3794;
    else
        goto L_3783;

L_3783:
    if ((ppart->hs.iItem != iengineEnigmaPulsar))
        goto L_379a;
    else
        goto L_3794;

L_3794:
    t_merge_379d_0001 = 0x1;
    goto L_379d;

L_379a:
    t_merge_379d_0001 = 0x0;

L_379d:
    fWarp10 = t_merge_379d_0001;
    i = 0x0;
    goto L_3920;

L_37a8:
    if ((i != 0xa))
        goto L_383c;
    else
        goto L_37b1;

L_37b1:
    cch = 0x2;
    rgch[0x0] = 0x31;
    rgch[0x1] = 0x30;
    if ((fWarp10 != 0x0))
        goto L_383c;
    else
        goto L_37c7;

L_37c7:
    hbrSav = SelectObject(hdc, rghbrPat[0x0]);
    SetTextColor(hdc, 0xffff);
    crBack = SetBkColor(hdc, crButtonFace);
    PatBlt(hdc, ((x - dxWarp) + 0x1), rcData.top, dxWarp, (yBase - rcData.top), PATCOPY);
    SetBkColor(hdc, crBack);
    SelectObject(hdc, hbrSav);

L_383c:
    if ((ppart->pengine->rgcFuelUsed[i] > 0x78))
        goto L_38b6;
    else
        goto L_385c;

L_385c:
    if ((i != 0xa))
        goto L_386e;
    else
        goto L_3865;

L_3865:
    if ((fWarp10 != 0x0))
        goto L_38a3;
    else
        goto L_386e;

L_386e:
    if ((ppart->pengine->rgcFuelUsed[(i + 0x1)] > 0x78))
        goto L_38a3;
    else
        goto L_3891;

L_3891:
    if ((i != 0x9))
        goto L_38b6;
    else
        goto L_389a;

L_389a:
    if ((fWarp10 != 0x0))
        goto L_38b6;
    else
        goto L_38a3;

L_38a3:
    SetTextColor(hdc, 0x7f0000);
    goto L_38c6;

L_38b6:
    SetTextColor(hdc, crButtonText);

L_38c6:
    TextOut(hdc, (x - LOWORD((dxDigit * cch))), y, rgch, cch);
    if ((i <= 0x0))
        goto L_3912;
    else
        goto L_38f0;

L_38f0:
    PatBlt(hdc, x, (y + 0xfffa), 0x1, 0x5, BLACKNESS);

L_3912:
    rgch[0x0] = (rgch[0x0] + 0x1);
    x = (x + dxWarp);
    i = (i + 0x1);

L_3920:
    if ((i <= 0xa))
        goto L_37a8;
    else
        goto L_3929;

L_3929:
    SetTextColor(hdc, crFore);
    hpenSav = SelectObject(hdc, hpenDkBlue);
    x = ((rcData.left + dxStr) + 0x6);
    i = 0x0;
    goto L_3a4b;

L_395a:
    y = yBase;
    pct = 0x19;
    iEff = ppart->pengine->rgcFuelUsed[i];
    goto L_3993;

L_3985:
    pct = (pct * 0x2);
    y = (y - dyPct);

L_3993:
    if ((iEff >= pct))
        goto L_3985;
    else
        goto L_399e;

L_399e:
    if ((pct != 0x19))
        goto L_39ed;
    else
        goto L_39a7;

L_39a7:
    if ((iEff != 0x0))
        goto L_39b7;
    else
        goto L_39b0;

L_39b0:
    y = (y - 0x1);
    goto L_3a19;

L_39b7:
    /* untranslated: branch 0x1 <= (words(loword(((iEff * 0x4) * dyPct)), signhiword(loword(((iEff * 0x4) * dyPct)))) / 0x64) ? L_39d7 : L_39d1 */

L_39d1:
    t_merge_39e7_0001 = 0x1;
    goto L_39e7;

L_39d7:
    /* untranslated: t_merge_39e7_0001 = (words(loword(((iEff * 0x4) * dyPct)), signhiword(loword(((iEff * 0x4) * dyPct)))) / 0x64) */

L_39e7:
    y = (y - t_merge_39e7_0001);

L_39ed:
    pctT = MulDiv((iEff - (pct >> 0x1)), 0x64, (pct >> 0x1));
    /* untranslated: y = (y - (words(loword((pctT * dyPct)), signhiword(loword((pctT * dyPct)))) / 0x64)) */

L_3a19:
    if ((i != 0x0))
        goto L_3a33;
    else
        goto L_3a22;

L_3a22:
    MoveTo(hdc, x, y);
    goto L_3a41;

L_3a33:
    LineTo(hdc, x, y);

L_3a41:
    x = (x + dxWarp);
    i = (i + 0x1);

L_3a4b:
    if ((i <= 0xa))
        goto L_395a;
    else
        goto L_3a54;

L_3a54:
    SelectObject(hdc, hpenSav);
    goto L_3ac9;

L_3a6c:
    if ((ppart->hs.iItem < iengineSubGalacticFuelScoop))
        goto L_3a93;
    else
        goto L_3a7d;

L_3a7d:
    if ((ppart->hs.iItem > iengineTransGalacticMizerScoop))
        goto L_3a93;
    else
        goto L_3a8e;

L_3a8e:
    ids = idsEngineWillUnavailableIfHaveLesserRacial;

L_3a93:
    goto L_3ae8;
    ids = idsEngineCanMountedMiniColonizerHullRequires;
    goto L_3ae8;
    ids = idsEngineCreatesPowerfulWavesRadiationWillKill;
    goto L_3ae8;
    ids = idsEngineRequiresLesserRacialTraitImprovedFuel;
    goto L_3ae8;
    ids = idsEngineRequiresLesserRacialTraitImprovedFuel2;
    goto L_3ae8;
    ids = idsEngineRequiresLesserRacialTraitRamScoop;
    goto L_3ae8;
    ids = idsOriginEngineUnknownAdds14Square;
    goto L_64d4;

L_3ac9:
    if ((ppart->pengine->grfAbilities > 0x6))
        goto L_3a6c;
    else
        goto L_3ad1;

L_3ad1:
    goto L_ffffffff;

L_3ae8:

L_3aeb:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x0]);
    i = ppart->pscanner->dRange;
    if ((i != 0x0))
        goto L_3b37;
    else
        goto L_3b21;

L_3b21:
    c = CchGetString(idsEnemyFleetsCannotDetectedScannerUnlessSame, szWork);
    goto L_3b68;

L_3b37:
    c = CchGetString(idsEnemyFleetsOrbitingPlanetCanDetectedD, szT);
    c = _wsprintf(szWork, szT, i);

L_3b68:
    dyText = DrawText(hdc, szWork, c, &(rcData), 0x810);
    rcData.top = (rcData.top + ((dyArial8 >> 0x1) + dyText));
    i = ppart->pscanner->grfAbilities;
    if ((i != 0x0))
        goto L_3bc1;
    else
        goto L_3bab;

L_3bab:
    c = CchGetString(idsScannerCapableDeterminingPlanetsEnvironmentCompo, szWork);
    goto L_3c70;

L_3bc1:
    if ((i == 0x4))
        goto L_3c22;
    else
        goto L_3bca;

L_3bca:
    c = CchGetString(idsScannerCanDeterminePlanetsBasicStatsDistance, szT);
    if ((i != 0x1))
        goto L_3bed;
    else
        goto L_3be7;

L_3be7:
    t_merge_3bff_0001 = 0x32;
    goto L_3bff;

L_3bed:
    if ((i != 0x2))
        goto L_3bfc;
    else
        goto L_3bf6;

L_3bf6:
    t_merge_3bff_0001 = 0x64;
    goto L_3bff;

L_3bfc:
    t_merge_3bff_0001 = 0xc8;

L_3bff:
    c = _wsprintf(szWork, szT, t_merge_3bff_0001);
    ids = idsScannerWillUnavailableIfHaveLesserRacial;
    goto L_3c70;

L_3c22:
    if ((ppart->hs.iItem != iscannerPickPocketScanner))
        goto L_3c3b;
    else
        goto L_3c33;

L_3c33:
    ids = idsScannerCapablePenetratingDefensesEnemyFleetsAllo;
    goto L_3c59;

L_3c3b:
    if ((ppart->hs.iItem != iscannerRobberBaronScanner))
        goto L_3c54;
    else
        goto L_3c4c;

L_3c4c:
    ids = idsScannerCanDeterminePlanetsStatsDistance120;
    goto L_3c59;

L_3c54:
    ids = idsScannerCanDeterminePlanetsBasicStatsDistance2;

L_3c59:
    c = CchGetString(ids, szWork);
    ids = idsScannerRequiresPrimaryRacialTraitSuperStealth;

L_3c70:
    dyText = DrawText(hdc, szWork, c, &(rcData), 0x810);
    goto L_64d4;

L_3c94:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    xText = (((uint32_t)((rcData.right - rcData.left)) / 0x2) + rcData.left);
    idsT = 0x48;
    if ((ppart->hs.iItem == ihuldefMiniColonyShip))
        goto L_3ce1;
    else
        goto L_3cd0;

L_3cd0:
    if ((ppart->hs.iItem != ihuldefMetaMorph))
        goto L_3ce9;
    else
        goto L_3ce1;

L_3ce1:
    ids = idsHullRequiresPrimaryRacialTraitHyperExpansion;
    goto L_3e36;

L_3ce9:
    if ((ppart->hs.iItem == ihuldefSuperFreighter))
        goto L_3d0b;
    else
        goto L_3cfa;

L_3cfa:
    if ((ppart->hs.iItem != ihuldefFuelTransport))
        goto L_3d13;
    else
        goto L_3d0b;

L_3d0b:
    ids = idsHullRequiresPrimaryRacialTraitInnerStrength;
    goto L_3e36;

L_3d13:
    if ((ppart->hs.iItem != ihuldefMaxiMiner))
        goto L_3d2c;
    else
        goto L_3d24;

L_3d24:
    ids = idsHullUnavailableIfHaveRaceDisadvantageBasic;
    goto L_3e36;

L_3d2c:
    if ((ppart->hs.iItem == ihuldefMidgetMiner))
        goto L_3d5f;
    else
        goto L_3d3d;

L_3d3d:
    if ((ppart->hs.iItem == ihuldefMiner))
        goto L_3d5f;
    else
        goto L_3d4e;

L_3d4e:
    if ((ppart->hs.iItem != ihuldefUltraMiner))
        goto L_3d67;
    else
        goto L_3d5f;

L_3d5f:
    ids = idsMiningHullRequiresLesserRacialTraitAdvanced;
    goto L_3e36;

L_3d67:
    if ((ppart->hs.iItem == ihuldefDreadnought))
        goto L_3d89;
    else
        goto L_3d78;

L_3d78:
    if ((ppart->hs.iItem != ihuldefBattleCruiser))
        goto L_3d91;
    else
        goto L_3d89;

L_3d89:
    ids = idsHullRequiresPrimaryRacialTraitWarMonger;
    goto L_3e36;

L_3d91:
    if ((ppart->hs.iItem == ihuldefRogue))
        goto L_3db3;
    else
        goto L_3da2;

L_3da2:
    if ((ppart->hs.iItem != ihuldefStealthBomber))
        goto L_3dbb;
    else
        goto L_3db3;

L_3db3:
    ids = idsHullRequiresPrimaryRacialTraitSuperStealth;
    goto L_3e36;

L_3dbb:
    if ((ppart->hs.iItem == ihuldefMiniMineLayer))
        goto L_3ddd;
    else
        goto L_3dcc;

L_3dcc:
    if ((ppart->hs.iItem != ihuldefSuperMineLayer))
        goto L_3de5;
    else
        goto L_3ddd;

L_3ddd:
    ids = idsHullRequiresPrimaryRaceTraitSpaceDemolition;
    goto L_3e36;

L_3de5:
    if ((ppart->hs.iItem == ihuldefScout))
        goto L_3e18;
    else
        goto L_3df6;

L_3df6:
    if ((ppart->hs.iItem == ihuldefFrigate))
        goto L_3e18;
    else
        goto L_3e07;

L_3e07:
    if ((ppart->hs.iItem != ihuldefDestroyer))
        goto L_3e20;
    else
        goto L_3e18;

L_3e18:
    ids = idsHullWillHaveBuiltScannerIfJack;
    goto L_3e36;

L_3e20:
    if ((ppart->hs.iItem != ihuldefMiniMorph))
        goto L_3e36;
    else
        goto L_3e31;

L_3e31:
    ids = idsOriginHullUnknown;

L_3e36:
    i = 0x0;
    goto L_3e5f;

L_3e3e:
    i = (i + 0x1);
    idsT = (idsT + 0x1);
    /* untranslated: rcData.top = (rcData.top + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */

L_3e5f:
    if ((i >= 0x4))
        goto L_3fa0;
    else
        goto L_3e68;

L_3e68:
    c = CchGetString(idsT, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, rcData.top, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_3f62;

L_3eb4:
    c = _wsprintf(szWork, "%dmg", ppart->phul->wtFuelMax);
    goto L_3f85;

L_3eda:
    c = _wsprintf(szWork, PCTDKT, ppart->phul->wtCargoMax);
    goto L_3f85;

L_3f00:
    c = _wsprintf(szWork, PCTD, ppart->phul->dp);
    goto L_3f85;

L_3f26:
    c = _wsprintf(szWork, PCTD, (*(LphuldefFromId(LOWORD(ppart->phul)) + 0x7b) & 0x3f));
    goto L_3f85;

L_3f62:
    if ((i == 0x0))
        goto L_3eb4;
    else
        goto L_3f6a;

L_3f6a:
    if ((i == 0x1))
        goto L_3eda;
    else
        goto L_3f72;

L_3f72:
    if ((i == 0x2))
        goto L_3f00;
    else
        goto L_3f7a;

L_3f7a:
    if ((i == 0x3))
        goto L_3f26;
    else
        goto L_3f85;

L_3f85:
    TextOut(hdc, xText, rcData.top, szWork, c);
    goto L_3e3e;

L_3fa0:
    if ((ppart->hs.iItem == ihuldefFuelTransport))
        goto L_3fc2;
    else
        goto L_3fb1;

L_3fb1:
    if ((ppart->hs.iItem != ihuldefSuperFuelXport))
        goto L_4056;
    else
        goto L_3fc2;

L_3fc2:
    psz = PszGetCompressedString(idsHullWillManufacture200UnitsFuelEach);
    if ((ppart->hs.iItem != ihuldefFuelTransport))
        goto L_3fea;
    else
        goto L_3fe2;

L_3fe2:
    pct = 0x5;
    goto L_3fef;

L_3fea:
    pct = 0xa;

L_3fef:
    c = _wsprintf(szWork, psz, pct);
    SelectObject(hdc, rghfontArial8[0x0]);
    xText = rcData.left;
    yText = rcData.top;
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, xText, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    goto L_64d4;

L_4056:
    if ((ppart->hs.iItem == ihuldefMiniMineLayer))
        goto L_4078;
    else
        goto L_4067;

L_4067:
    if ((ppart->hs.iItem != ihuldefSuperMineLayer))
        goto L_64d4;
    else
        goto L_4078;

L_4078:
    c = CchGetString(idsHullWillDoubleEfficiencyMineLayingPods, szWork);
    SelectObject(hdc, rghfontArial8[0x0]);
    xText = rcData.left;
    yText = rcData.top;
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, xText, (rcData.right - rcData.left), 0x0, 0x0, 0x1);

L_40d6:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    idsT = 0x4a;
    xText = rcData.left;
    yText = rcData.top;
    if ((ppart->hs.iItem == 0x1))
        goto L_411d;
    else
        goto L_410c;

L_410c:
    if ((ppart->hs.iItem != 0x3))
        goto L_4125;
    else
        goto L_411d;

L_411d:
    ids = idsStarbaseHullRequiresLesserRacialTraitImproved;
    goto L_413b;

L_4125:
    if ((ppart->hs.iItem != 0x4))
        goto L_413b;
    else
        goto L_4136;

L_4136:
    ids = idsHullRequiresPrimaryRacialTraitAlternateReality;

L_413b:
    SelectObject(hdc, rghfontArial8[0x0]);
    /* untranslated: branch part[52:2](ppart->pcom) == 0x0 ? L_4264 : L_4157 */

L_4157:
    c = CchGetString(idsStarbaseHullHasSpaceDockCanBuild, szWork);
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x1]);
    yText = (LOWORD((0x3 * dyArial8)) + rcData.top);
    xText = (((uint32_t)((rcData.right - rcData.left)) / 0x2) + rcData.left);
    c = CchGetString(idsDockCapacity2, szWork);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    /* untranslated: branch part[52:2](ppart->pcom) == 0xffff ? L_4236 : L_4210 */

L_4210:
    /* untranslated: c = _wsprintf(szWork, PCTDKT, part[52:2](ppart->pcom)) */
    goto L_4249;

L_4236:
    c = CchGetString(idsUnlimited, szWork);

L_4249:
    TextOut(hdc, xText, yText, szWork, c);
    goto L_42b4;

L_4264:
    c = CchGetString(idsStarbaseHullDoesHaveSpaceDockCan, szWork);
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    yText = (LOWORD((0x3 * dyArial8)) + rcData.top);

L_42b4:
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    xText = (((uint32_t)((rcData.right - rcData.left)) / 0x2) + rcData.left);
    i = 0x0;
    goto L_42ff;

L_42de:
    i = (i + 0x1);
    idsT = (idsT + 0x1);
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */

L_42ff:
    if ((i >= 0x2))
        goto L_64d4;
    else
        goto L_4308;

L_4308:
    c = CchGetString(idsT, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_43b6;

L_4354:
    /* untranslated: c = _wsprintf(szWork, PCTD, part[56:2](ppart->pcom)) */
    goto L_43c9;

L_437a:
    c = _wsprintf(szWork, PCTD, (*(LphuldefFromId(LOWORD(ppart->pcom)) + 0x7b) & 0x3f));
    goto L_43c9;

L_43b6:
    if ((i == 0x0))
        goto L_4354;
    else
        goto L_43be;

L_43be:
    if ((i == 0x1))
        goto L_437a;
    else
        goto L_43c9;

L_43c9:
    TextOut(hdc, xText, yText, szWork, c);
    goto L_42de;

L_43e7:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    DxStreamTextOut(hdc, &(xText), rcData.top, PszGetCompressedString(idsShieldStrength), 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->pshield->dp);
    DxStreamTextOut(hdc, &(xText), rcData.top, szWork, c, 0x1);
    if ((ppart->hs.iItem != ishieldShadowShield))
        goto L_44d6;
    else
        goto L_4491;

L_4491:
    ids = idsArmorShieldRequiresPrimaryRacialTraitSuper;
    idsT = 0x3c;

LShieldDisp:
    c = CchGetString(idsT, szWork);
    rcData.top = (rcData.top + (dyArial8 + 0x4));
    DrawText(hdc, szWork, c, &(rcData), 0x810);
    goto L_64d4;

L_44d6:
    if ((ppart->hs.iItem != ishieldCrobySharmor))
        goto L_44f7;
    else
        goto L_44e7;

L_44e7:
    ids = idsShieldRequiresPrimaryRacialTraitInnerStrength;
    idsT = 0x3f;
    goto LShieldDisp;

L_44f7:
    if ((ppart->hs.iItem != ishieldLangstonShell))
        goto L_64d4;
    else
        goto L_4508;

L_4508:
    ids = idsOriginPartUnknown;
    idsT = 0x40;
    goto LShieldDisp;

L_4518:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    DxStreamTextOut(hdc, &(xText), rcData.top, PszGetCompressedString(idsArmorStrength2), 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->parmor->dp);
    DxStreamTextOut(hdc, &(xText), rcData.top, szWork, c, 0x1);
    if ((ppart->hs.iItem != iarmorDepletedNeutronium))
        goto L_4607;
    else
        goto L_45c2;

L_45c2:
    ids = idsArmorShieldRequiresPrimaryRacialTraitSuper;
    idsT = 0x3d;

LArmDisp:
    c = CchGetString(idsT, szWork);
    rcData.top = (rcData.top + (dyArial8 + 0x4));
    DrawText(hdc, szWork, c, &(rcData), 0x810);
    goto L_64d4;

L_4607:
    if ((ppart->hs.iItem != iarmorFieldedKelarium))
        goto L_4628;
    else
        goto L_4618;

L_4618:
    ids = idsArmorRequiresPrimaryRacialTraitInnerStrength;
    idsT = 0x3e;
    goto LArmDisp;

L_4628:
    if ((ppart->hs.iItem != iarmorMegaPolyShell))
        goto L_64d4;
    else
        goto L_4639;

L_4639:
    ids = idsOriginPartUnknown;
    c = CchGetString(idsPartAlsoActs100dpShield20Cloak, szWork);
    rcData.top = (rcData.top + (dyArial8 + 0x4));
    SelectObject(hdc, rghfontArial8[0x0]);
    DrawText(hdc, szWork, c, &(rcData), 0x810);

L_4686:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    xText = (((uint32_t)((rcData.right - rcData.left)) / 0x3) + rcData.left);
    yText = rcData.top;
    c = CchGetString(idsPower, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->pbeam->dp);
    TextOut(hdc, xText, yText, szWork, c);
    yText = (yText + dyArial8);
    c = CchGetString(idsRange, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->pbeam->dRangeMax);
    TextOut(hdc, xText, yText, szWork, c);
    yText = (yText + dyArial8);
    c = CchGetString(idsInitiative, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->pbeam->init);
    TextOut(hdc, xText, yText, szWork, c);
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    rcData.top = yText;
    if ((ppart->pbeam->grfAbilities == 0x0))
        goto L_4967;
    else
        goto L_486a;

L_486a:
    SelectObject(hdc, rghfontArial7);
    if (((ppart->pbeam->grfAbilities & 0x1) == 0x0))
        goto L_48be;
    else
        goto L_488b;

L_488b:
    c = CchGetString(idsWeaponWillDamageShieldsHasEffectArmor, szWork);
    rcData.top = (rcData.top + DrawText(hdc, szWork, c, &(rcData), 0x810));

L_48be:
    if (((ppart->pbeam->grfAbilities & 0x2) == 0x0))
        goto L_4958;
    else
        goto L_48d3;

L_48d3:
    c = CchGetString(idsWeaponHitsTargetsRangeEachTimeFired, szWork);
    rcData.top = (rcData.top + DrawText(hdc, szWork, c, &(rcData), 0x810));
    c = _wsprintf(szWork, PszGetCompressedString(idsWeaponAlsoMakesExcellentMineSweeperCapable), (ppart->pbeam->dp * 0x10));
    DrawText(hdc, szWork, c, &(rcData), 0x810);

L_4958:
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_49ec;

L_4967:
    if ((ppart->hs.iItem != ibeamMultiContainedMunition))
        goto L_49ec;
    else
        goto L_4978;

L_4978:
    ids = idsOriginPartUnknown;
    c = CchGetString(idsPartAlsoActs10CloakIncreasesTorpedo, szWork);
    SelectObject(hdc, rghfontArial7);
    rcData.top = (rcData.top + DrawText(hdc, szWork, c, &(rcData), 0x810));
    c = CchGetString(idsWeaponCanAlsoBombPlanets2Colonists, szWork);
    DrawText(hdc, szWork, c, &(rcData), 0x810);

L_49ec:
    if ((ppart->hs.iItem != ibeamMiniGun))
        goto L_4a05;
    else
        goto L_49fd;

L_49fd:
    ids = idsPartRequiresPrimaryRacialTraitInnerStrength;
    goto L_64d4;

L_4a05:
    if ((ppart->hs.iItem == ibeamGatlingNeutrinoCannon))
        goto L_4a27;
    else
        goto L_4a16;

L_4a16:
    if ((ppart->hs.iItem != ibeamBlunderbuss))
        goto L_64d4;
    else
        goto L_4a27;

L_4a27:
    ids = idsPartRequiresPrimaryRacialTraitWarMonger;

L_4a2f:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    xText = (((uint32_t)((rcData.right - rcData.left)) / 0x3) + rcData.left);
    yText = rcData.top;
    c = CchGetString(idsPower, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->ptorp->dp);
    TextOut(hdc, xText, yText, szWork, c);
    yText = (yText + dyArial8);
    c = CchGetString(idsRange, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->ptorp->dRangeMax);
    TextOut(hdc, xText, yText, szWork, c);
    yText = (yText + dyArial8);
    c = CchGetString(idsInitiative, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->ptorp->init);
    TextOut(hdc, xText, yText, szWork, c);
    yText = (yText + dyArial8);
    c = CchGetString(idsAccuracy, szWork);
    SelectObject(hdc, rghfontArial8[0x1]);
    RightTextOut(hdc, xText, yText, szWork, c, 0x0);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->ptorp->dHitChance);
    TextOut(hdc, xText, yText, szWork, c);
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    if ((ppart->hs.iItem != itorpAntiMatterTorpedo))
        goto L_4c9e;
    else
        goto L_4c96;

L_4c96:
    ids = idsOriginPartUnknown;
    goto L_64d4;

L_4c9e:
    if ((ppart->hs.iItem < itorpJihadMissile))
        goto L_64d4;
    else
        goto L_4caf;

L_4caf:
    if ((ppart->hs.iItem > itorpArmageddonMissile))
        goto L_64d4;
    else
        goto L_4cc0;

L_4cc0:
    ids = idsCapitalShipMissilesDoTwiceStatedDamage;

L_4cc8:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    if ((ppart->hs.iItem != ibombRetroBomb))
        goto L_4d29;
    else
        goto L_4d13;

L_4d13:
    c = CchGetString(idsBombDoesKillColonistsDestroyInstallationsBomb, szWork);
    goto L_4e5a;

L_4d29:
    if ((ppart->pbomb->dDmgCol != 0x0))
        goto L_4d4f;
    else
        goto L_4d39;

L_4d39:
    c = CchGetString(idsBombWillKillAnyPlanetsPopulation, szWork);
    goto L_4e5a;

L_4d4f:
    CchGetString(idsBombWillKillApproximatelyDDPlanets, szWork);
    c = _wsprintf(szT, szWork, ((uint32_t)(ppart->pbomb->dDmgCol) / 0xa), ((uint32_t)(ppart->pbomb->dDmgCol) % 0xa));
    if ((ppart->hs.iItem < ibombLadyFingerBomb))
        goto L_4dc6;
    else
        goto L_4dac;

L_4dac:
    if ((ppart->hs.iItem > ibombCherryBomb))
        goto L_4dc6;
    else
        goto L_4dbd;

L_4dbd:
    dmgFloor = 0x3;
    goto L_4df7;

L_4dc6:
    if ((ppart->hs.iItem < ibombSmartBomb))
        goto L_4df1;
    else
        goto L_4dd7;

L_4dd7:
    if ((ppart->hs.iItem > ibombAnnihilatorBomb))
        goto L_4df1;
    else
        goto L_4de8;

L_4de8:
    dmgFloor = 0x3e7;
    goto L_4df7;

L_4df1:
    dmgFloor = 0x0;

L_4df7:
    if ((dmgFloor <= 0x0))
        goto L_4e49;
    else
        goto L_4e01;

L_4e01:
    if ((dmgFloor != 0x3e7))
        goto L_4e16;
    else
        goto L_4e10;

L_4e10:
    t_merge_4e19_0001 = 0x513;
    goto L_4e19;

L_4e16:
    t_merge_4e19_0001 = 0x1c1;

L_4e19:
    CchGetString(t_merge_4e19_0001, szWork);
    c = (c + _wsprintf(&(szT[c]), szWork, LOWORD((0x64 * dmgFloor))));

L_4e49:
    strcpy(szWork, szT);

L_4e5a:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    xText = rcData.left;
    yText = (yText + (dyArial8 >> 0x1));
    if ((ppart->hs.iItem != ibombRetroBomb))
        goto L_4eb5;
    else
        goto L_4ead;

L_4ead:
    ids = idsPartRequiresPrimaryRacialTraitClaimAdjuster;
    goto L_64d4;

L_4eb5:
    if ((ppart->pbomb->dDmgBldg != 0x0))
        goto L_4edb;
    else
        goto L_4ec5;

L_4ec5:
    c = CchGetString(idsBombWillDamagePlanetsMinesFactories, szWork);
    goto L_4f10;

L_4edb:
    CchGetString(idsBombWillDestroyApproximatelyDPlanetsMines, szT);
    c = _wsprintf(szWork, szT, ppart->pbomb->dDmgBldg);

L_4f10:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x1, 0x1);
    if ((ppart->hs.iItem < ibombSmartBomb))
        goto L_4f6c;
    else
        goto L_4f53;

L_4f53:
    if ((ppart->hs.iItem > ibombAnnihilatorBomb))
        goto L_4f6c;
    else
        goto L_4f64;

L_4f64:
    ids = idsBombWillAvailableIfPrimaryRaceTrait;
    goto L_64d4;

L_4f6c:
    if ((ppart->hs.iItem != ibombHushABoom))
        goto L_64d4;
    else
        goto L_4f7d;

L_4f7d:
    ids = idsOriginPartUnknown;

L_4f85:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_5135;

L_4fc9:
    idsT = 0xffff;
    goto L_5168;
    idsT = 0xdb;
    ids = idsCloakRequiresPrimaryRacialTraitSuperStealth;
    goto L_5168;
    if ((ppart->hs.iItem != ispecialEUltraStealthCloak))
        goto L_4ffc;
    else
        goto L_4fef;

L_4fef:
    ids = idsCloakRequiresPrimaryRacialTraitSuperStealth;
    idsT = 0x55;
    goto L_5019;

L_4ffc:
    if ((ppart->hs.iItem != 0x1))
        goto L_5013;
    else
        goto L_500d;

L_500d:
    t_merge_5016_0001 = 0x23;
    goto L_5016;

L_5013:
    t_merge_5016_0001 = 0x37;

L_5016:
    idsT = t_merge_5016_0001;

L_5019:
    c = _wsprintf(szWork, PszGetCompressedString(idsCloaksAnyShipReducingRangeWhichScanners), idsT);
    goto PrintSpecial;
    idsT = 0xdd;
    ids = idsOriginPartUnknown;
    goto L_5168;
    ids = idsDeviceRequiresPrimaryRacialTraitSpaceDemolition;
    idsT = 0x39b;
    goto L_5168;
    ids = idsDeviceRequiresPrimaryRacialTraitInnerStrength;
    idsT = 0x39c;
    goto L_5168;
    ids = idsDeviceRequiresPrimaryRacialTraitInterstellarTrav;
    idsT = 0x39d;
    goto L_5168;
    ids = idsDeviceRequiresPrimaryRacialTraitHyperExpansion;
    if ((ppart->hs.iItem != 0xc))
        goto L_5091;
    else
        goto L_508b;

L_508b:
    t_merge_5094_0001 = 0xa;
    goto L_5094;

L_5091:
    t_merge_5094_0001 = 0x14;

L_5094:
    c = _wsprintf(szWork, PszGetCompressedString(idsIncreasesDamageDoneBeamWeaponsShipD), t_merge_5094_0001);
    goto PrintSpecial;
    ids = idsJammingDeviceRequiresPrimaryRacialTraitInner;
    idsT = 0x3a0;
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsT), part[52:2](*(ppart+0x4))) */
    goto PrintSpecial;
    idsT = 0x3a1;
    /* untranslated: c = _wsprintf(szWork, PszGetCompressedString(idsT), part[52:2](*(ppart+0x4)), (ppart->hs.iItem + 0xfffc)) */
    goto PrintSpecial;

L_5135:
    if ((ppart->hs.iItem > ispecialEAntiMatterGenerator))
        goto L_4fc9;
    else
        goto L_513d;

L_513d:
    goto L_ffffffff;

L_5168:
    if ((idsT == 0xffff))
        goto L_64d4;
    else
        goto L_5171;

L_5171:
    c = CchGetString(idsT, szWork);

PrintSpecial:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    xText = rcData.left;
    yText = (yText + (dyArial8 * 0x2));

L_51c4:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_530f;

L_5208:
    idsT = 0xffff;
    goto L_5336;
    if ((ppart->hs.iItem != ispecialMManeuveringJet))
        goto L_5227;
    else
        goto L_5221;

L_5221:
    t_merge_522a_0001 = 0x4;
    goto L_522a;

L_5227:
    t_merge_522a_0001 = 0x2;

L_522a:
    c = _wsprintf(szWork, PszGetCompressedString(idsIncreasesSpeedBattle1DSquareMovement), t_merge_522a_0001);
    goto PrintSpecial;
    if ((ppart->hs.iItem != 0x5))
        goto L_5267;
    else
        goto L_5261;

L_5261:
    t_merge_526a_0001 = 0xfa;
    goto L_526a;

L_5267:
    t_merge_526a_0001 = 0x1f4;

L_526a:
    c = _wsprintf(szWork, PszGetCompressedString(idsPodIncreasesFuelCapacityShipDmg), t_merge_526a_0001);
    goto PrintSpecial;
    if ((ppart->hs.iItem != 0x2))
        goto L_52a7;
    else
        goto L_52a1;

L_52a1:
    t_merge_52aa_0001 = 0x32;
    goto L_52aa;

L_52a7:
    t_merge_52aa_0001 = 0x64;

L_52aa:
    c = _wsprintf(szWork, PszGetCompressedString(idsPodIncreasesCargoCapacityShipDkt), t_merge_52aa_0001);
    goto PrintSpecial;
    idsT = 0xd6;
    ids = idsOriginPartUnknown;
    goto L_5336;
    idsT = 0xd9;
    ids = idsOriginPartUnknown;
    goto L_5336;
    idsT = 0xd7;
    ids = idsPartAvailableAlternateRealityRaces;
    goto L_5336;
    idsT = 0xd8;
    ids = idsPartRequiresPrimaryRacialTraitAlternateReality;
    goto L_5336;
    idsT = 0xda;
    goto L_5336;

L_530f:
    if ((ppart->hs.iItem > ispecialMBeamDeflector))
        goto L_5208;
    else
        goto L_5317;

L_5317:
    goto L_ffffffff;

L_5336:
    if ((idsT == 0xffff))
        goto L_64d4;
    else
        goto L_533f;

L_533f:
    c = CchGetString(idsT, szWork);
    goto PrintSpecial;

L_5357:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_53cd;

L_539b:
    idsT = 0xffff;
    goto L_53fe;
    ids = idsStargateRequiresPrimaryRacialTraitInterstellarTr;
    idsT = 0x2e0;
    goto L_53fe;
    ids = idsStargatesAvailableIfPrimaryRaceTraitHyper;
    idsT = 0x2e0;
    goto L_53fe;
    ids = idsMassDriverRequiresPrimaryRacialTraitPacket;
    idsT = 0x2e1;
    goto L_53fe;

L_53cd:
    if ((ppart->hs.iItem > ispecialSBUltraDriver13))
        goto L_539b;
    else
        goto L_53d5;

L_53d5:
    goto L_ffffffff;

L_53fe:
    c = CchGetString(idsT, szWork);
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    xText = rcData.left;
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    goto L_5757;

L_545c:
    SelectObject(hdc, rghfontArial8[0x1]);
    DxStreamTextOut(hdc, &(xText), yText, PszGetCompressedString(idsWarp), 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    c = _wsprintf(szWork, PCTD, ppart->pspecialsb->grAbility);
    DxStreamTextOut(hdc, &(xText), yText, szWork, c, 0x1);
    xText = rcData.left;
    yText = (yText + dyArial8);
    idsT = 0x457;
    if ((ppart->hs.iItem <= ispecialSBMassDriver5))
        goto L_5501;
    else
        goto L_54fd;

L_54fd:
    idsT = (idsT + 0x1);

L_5501:
    c = CchGetString(idsT, szWork);
    goto L_576a;

L_5516:
    SelectObject(hdc, rghfontArial8[0x1]);
    DxStreamTextOut(hdc, &(xText), yText, PszGetCompressedString(idsSafeHullMass), 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    if ((ppart->pspecialsb->grAbility != 0xffff))
        goto L_557b;
    else
        goto L_5565;

L_5565:
    c = CchGetString(idsUnlimited, szWork);
    goto L_559e;

L_557b:
    c = _wsprintf(szWork, PCTDKT, ppart->pspecialsb->grAbility);

L_559e:
    DxStreamTextOut(hdc, &(xText), yText, szWork, c, 0x1);
    xText = rcData.left;
    yText = (yText + dyArial8);
    SelectObject(hdc, rghfontArial8[0x1]);
    DxStreamTextOut(hdc, &(xText), yText, PszGetCompressedString(idsSafeRange), 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    if ((ppart->pspecialsb->grAbility2 != 0xffff))
        goto L_562c;
    else
        goto L_5616;

L_5616:
    c = CchGetString(idsUnlimited, szWork);
    goto L_5658;

L_562c:
    c = _wsprintf(szWork, PszGetCompressedString(idsDLightYears), ppart->pspecialsb->grAbility2);

L_5658:
    DxStreamTextOut(hdc, &(xText), yText, szWork, c, 0x1);
    if ((ppart->pspecialsb->grAbility != 0xffff))
        goto L_56d0;
    else
        goto L_5685;

L_5685:
    if ((ppart->pspecialsb->grAbility2 != 0xffff))
        goto L_569d;
    else
        goto L_5695;

L_5695:
    idsT = 0xffff;
    goto L_576a;

L_569d:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsCanSuccessfullyGatedDL), LOWORD((0x5 * ppart->pspecialsb->grAbility2)));

L_56d0:
    if ((ppart->pspecialsb->grAbility2 != 0xffff))
        goto L_5713;
    else
        goto L_56e0;

L_56e0:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsDktCanSuccessfullyGatedExceeding), LOWORD((0x5 * ppart->pspecialsb->grAbility)));
    goto L_576a;

L_5713:
    c = _wsprintf(szWork, PszGetCompressedString(idsWarningShipsDktMightSuccessfullyGatedD), LOWORD((0x5 * ppart->pspecialsb->grAbility)),
                  LOWORD((0x5 * ppart->pspecialsb->grAbility2)));

L_5757:
    if ((idsT == 0x2e0))
        goto L_5516;
    else
        goto L_575f;

L_575f:
    if ((idsT == 0x2e1))
        goto L_545c;
    else
        goto L_576a;

L_576a:
    if ((idsT == 0xffff))
        goto L_64d4;
    else
        goto L_5773;

L_5773:
    xText = rcData.left;
    if ((dyArial8 > 0xe))
        goto L_5789;
    else
        goto L_5783;

L_5783:
    t_merge_578c_0001 = 0x4;
    goto L_578c;

L_5789:
    t_merge_578c_0001 = 0x0;

L_578c:
    yText = (yText + (t_merge_578c_0001 + dyArial8));
    SelectObject(hdc, rghfontArial7);
    SetTextColor(hdc, 0x7f);
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    SelectObject(hdc, rghfontArial8[0x0]);
    SetTextColor(hdc, 0x0);

L_57fe:
    dxLabel = 0xffff;
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    rcData.top = (rcData.top + LOWORD((0x5 * dyArial8)));
    i = 0x4;
    goto L_589f;

L_5841:
    rcData.top = (rcData.top - dyArial8);
    c = CchGetString((i + 0x2d6), szWork);
    if ((dxLabel != 0xffff))
        goto L_587b;
    else
        goto L_5866;

L_5866:
    dxLabel = LOWORD(GetTextExtent(hdc, szWork, c));

L_587b:
    RightTextOut(hdc, (rcData.left + dxLabel), rcData.top, szWork, c, 0x0);
    i = (i - 0x1);

L_589f:
    if ((i >= 0x0))
        goto L_5841;
    else
        goto L_58a8;

L_58a8:
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_5967;
    iWarp = 0x4;
    pctHit = 0x3;
    dmgShip = 0x64;
    dmgShipRam = 0x7d;
    dmgMin = 0x1f4;
    dmgMinRam = 0x258;
    if ((ppart->hs.iItem == iminesMineDispenser50))
        goto L_58f7;
    else
        goto L_58ef;

L_58ef:
    ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
    goto L_58fc;

L_58f7:
    ids = idsPartUnavailbleWarMonger;

L_58fc:
    goto L_598c;
    iWarp = 0x6;
    pctHit = 0xa;
    dmgShip = 0x1f4;
    dmgShipRam = 0x258;
    dmgMin = 0x7d0;
    dmgMinRam = 0x9c4;
    ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
    goto L_598c;
    iWarp = 0x5;
    pctHit = 0x23;
    dmgShip = 0x0;
    dmgShipRam = 0x0;
    dmgMin = 0x0;
    dmgMinRam = 0x0;
    if ((ppart->hs.iItem == 0x7))
        goto L_595c;
    else
        goto L_5954;

L_5954:
    ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition;
    goto L_598c;

L_595c:
    ids = idsMineRequiresPrimaryRacialTraitSpaceDemolition2;

L_5967:

L_598c:
    c = _wsprintf(szWork, PCTD, LOWORD((0xa * ppart->pmines->grAbility)));
    TextOut(hdc, ((rcData.left + dxLabel) + 0x4), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    c = _wsprintf(szWork, PszGetCompressedString(idsWarpD2), iWarp);
    TextOut(hdc, ((rcData.left + dxLabel) + 0x4), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    c = _wsprintf(szWork, PszGetCompressedString(idsDD2), ((uint32_t)(pctHit) / 0xa), ((uint32_t)(pctHit) % 0xa));
    TextOut(hdc, ((rcData.left + dxLabel) + 0x4), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    c = _wsprintf(szWork, PszGetCompressedString(idsDDEngine), dmgShip, dmgShipRam);
    TextOut(hdc, ((rcData.left + dxLabel) + 0x4), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    c = _wsprintf(szWork, PszGetCompressedString(idsDD3), dmgMin, dmgMinRam);
    TextOut(hdc, ((rcData.left + dxLabel) + 0x4), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    SelectObject(hdc, rghfontArial7);
    c = CchGetString(idsNumbersParenthesisFleetsContainingShipRamScoop, szWork);
    DrawText(hdc, szWork, c, &(rcData), 0x810);
    goto L_64d4;

L_5b56:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    goto L_5c49;
    ids = idsRobotMinerRequiresLesserRacialTraitAdvanced;
    if ((ids != 0xffff))
        goto L_5bad;
    else
        goto L_5ba8;

L_5ba8:
    ids = idsRobotMinerWillAvailableIfLesserRacial;

L_5bad:
    c = CchGetString(idsModuleContainsRobotsCapableMining, szWork);
    /* untranslated: c = (c + _wsprintf(&szWork[c], PCTD, part[52:2](*(ppart+0x4)))) */
    c = (c + CchGetString(idsKtEachMineralDependingConcentrationUninhabitedPl, szWork[c]));
    if ((ppart->hs.iItem != 0x6))
        goto L_5c28;
    else
        goto L_5c0d;

L_5c0d:
    ids = idsOriginPartUnknown;
    c = (c + CchGetString(idsModuleAlsoActs30Cloak30Jammer, szWork[c]));

L_5c28:
    goto PrintSpecial;
    ids = idsPartRequiresPrimaryRacialTraitClaimAdjuster;
    c = CchGetString(idsModifiedMiningRobotTerraformsInhabitedPlanets1, szWork);
    goto PrintSpecial;

L_5c49:

L_5c6d:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    c = 0x0;
    if ((ppart->hs.iItem > iterraTotalTerraform30))
        goto L_5cfe;
    else
        goto L_5cbd;

L_5cbd:
    ids = idsTotalTerraformingRequiresLesserRacialTraitTotal;
    c = CchGetString(idsAllowsModifyAnyPlanetsThreeEnvironmentVariables, szT);
    c = _wsprintf(szWork, szT, ppart->pterra->grAbility);
    goto L_5d55;

L_5cfe:
    c = CchGetString(idsAllowsModifyPlanetsSDOriginalValue, szT);
    c = _wsprintf(szWork, szT, rgszPlanetAttr[((uint32_t)((ppart->hs.iItem + 0xfff8)) / 0x4)], ppart->pterra->grAbility);

L_5d55:
    if ((c <= 0x0))
        goto L_64d4;
    else
        goto L_5d5e;

L_5d5e:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);

L_5d93:
    ExpandRc(&(rcData), 0xfffc, 0xfffc);
    SelectObject(hdc, rghfontArial8[0x1]);
    xText = rcData.left;
    yText = rcData.top;
    SelectObject(hdc, rghfontArial8[0x0]);
    c = 0x0;
    if ((ppart->hs.iItem < iplanetarySDI))
        goto L_62e5;
    else
        goto L_5de3;

L_5de3:
    if ((ppart->hs.iItem > iplanetaryNeutronShield))
        goto L_62e5;
    else
        goto L_5df4;

L_5df4:
    SelectObject(hdc, rghfontArial8[0x1]);
    rcData.right = (rcData.right - 0x2);
    c = CchGetString(idsShieldCoverageVsDefenseQuan, szWork);
    CtrTextOut(hdc, (((rcData.right - rcData.left) >> 0x1) + rcData.left), rcData.top, szWork, c);
    rcData.top = (rcData.top + dyArial8);
    cch = LOBYTE(CchGetString(idsNum, szT));
    dxStr = LOWORD(GetTextExtent(hdc, szT, (uint16_t)(cch)));
    dxQuan = ((uint32_t)((((rcData.right - rcData.left) - dxStr) + 0xfff6)) / 0x5);
    dyPct = ((uint32_t)((((rcData.bottom - rcData.top) - dyArial8) + 0xfff8)) / 0x5);
    PatBlt(hdc, ((rcData.left + dxStr) + 0x6), rcData.top, 0x1, (((rcData.bottom - rcData.top) - dyArial8) + 0xfffc), BLACKNESS);
    PatBlt(hdc, ((rcData.left + dxStr) + 0x6), ((rcData.bottom - dyArial8) + 0xfffc), (((rcData.right - rcData.left) - dxStr) + 0xfffa), 0x1, BLACKNESS);
    x = (rcData.left + dxStr);
    y = (((rcData.bottom - dyArial8) + 0xfffc) - LOWORD((0x5 * dyPct)));
    SetTextColor(hdc, 0x7f0000);
    cch = LOBYTE(CchGetString(idsStandard, szWork));
    RightTextOut(hdc, rcData.right, ((rcData.bottom + 0xfffd) - LOWORD((0x3 * dyArial8))), szWork, (uint16_t)(cch), 0x0);
    SetTextColor(hdc, 0x7f);
    cch = LOBYTE(CchGetString(idsSmart, szWork));
    RightTextOut(hdc, rcData.right, ((rcData.bottom + 0xfffc) - (dyArial8 * 0x2)), szWork, (uint16_t)(cch), 0x0);
    pct = 0x64;
    SetTextColor(hdc, 0x0);
    goto L_604a;

L_5fd6:
    c = _wsprintf(szWork, PCTDPCTPCT, pct);
    RightTextOut(hdc, x, (y - (dyArial8 >> 0x1)), szWork, c, 0x0);
    PatBlt(hdc, (x + 0x4), y, 0x5, 0x1, BLACKNESS);
    pct = (pct - 0x14);
    y = (y + dyPct);

L_604a:
    if ((pct > 0x0))
        goto L_5fd6;
    else
        goto L_6054;

L_6054:
    y = (rcData.bottom - dyArial8);
    RightTextOut(hdc, x, y, szT, strlen(szT), 0x0);
    x = (x + 0x6);
    dxDigit = LOWORD(GetTextExtent(hdc, "0", 0x1));
    ch = 0x30;
    i = 0x0;
    goto L_6160;

L_60b2:
    cch = LOBYTE(_wsprintf(szWork, PCTD, LOWORD((0x14 * i))));
    if ((i == 0x0))
        goto L_60ff;
    else
        goto L_60df;

L_60df:
    if ((i != 0x5))
        goto L_60f8;
    else
        goto L_60e8;

L_60e8:
    /* untranslated: t_merge_6105_0001 = (words(loword((0x3 * dxDigit)), signhiword(loword((0x3 * dxDigit)))) / 0x2) */
    goto L_6105;

L_60f8:
    t_merge_6105_0001 = dxDigit;

L_60ff:
    t_merge_6105_0001 = (dxDigit >> 0x1);

L_6105:
    TextOut(hdc, (x - t_merge_6105_0001), y, szWork, (uint16_t)(cch));
    if ((i <= 0x0))
        goto L_614f;
    else
        goto L_612b;

L_612b:
    PatBlt(hdc, x, (y + 0xfffa), 0x1, 0x5, BLACKNESS);

L_614f:
    ch = (ch + 0x1);
    x = (x + dxQuan);
    i = (i + 0x1);

L_6160:
    if ((i <= 0x5))
        goto L_60b2;
    else
        goto L_6169;

L_6169:
    hpenSav = SelectObject(hdc, hpenDkBlue);
    xBase = ((rcData.left + dxStr) + 0x6);
    yBase = ((rcData.bottom - dyArial8) + 0xfffc);
    ldelta = (uint32_t)((0x3e8 - ppart->pplanetary->grAbility));
    c = 0x0;
    goto L_62aa;

L_61b2:
    lpct = 0xf4240;
    i = 0x0;
    goto L_6271;

L_61c6:
    x = (MulDiv(i, dxQuan, 0x14) + xBase);
    y = yBase;
    /* untranslated: y = (y - loword((int32_t)((uint32_t)(words((0xf - HIWORD(lpct)), (0x4240 - LOWORD(lpct))) * sext16to32(dyPct)) / 0x30d40))) */
    lpct = (int32_t)(((uint32_t)((lpct * ldelta)) / 0x3e8));
    if ((i != 0x0))
        goto L_625d;
    else
        goto L_624a;

L_624a:
    MoveTo(hdc, x, y);
    goto L_626d;

L_625d:
    LineTo(hdc, x, y);

L_626d:
    i = (i + 0x1);

L_6271:
    if ((i <= 0x64))
        goto L_61c6;
    else
        goto L_627a;

L_627a:
    ldelta = (uint32_t)((0x3e8 - ((uint32_t)(ppart->pplanetary->grAbility) / 0x2)));
    SelectObject(hdc, hpenRadar);
    c = (c + 0x1);

L_62aa:
    if ((c < 0x2))
        goto L_61b2;
    else
        goto L_62b3;

L_62b3:
    SelectObject(hdc, hpenSav);
    c = 0x0;
    if ((ppart->hs.iItem > iplanetaryMissileBattery))
        goto L_62dd;
    else
        goto L_62d5;

L_62d5:
    ids = idsPlanetaryScannersDefensesAvailableAlternateReali;
    goto L_6410;

L_62dd:
    ids = idsPlanetaryDefenseUnavailablePrimaryRacialTraitWar;

L_62e5:
    if ((ppart->hs.iItem < iplanetaryViewer50))
        goto L_63e7;
    else
        goto L_62f6;

L_62f6:
    if ((ppart->hs.iItem > iplanetarySnooper620X))
        goto L_63e7;
    else
        goto L_6307;

L_6307:
    i = ppart->pplanetary->grAbility;
    c = CchGetString(idsEnemyFleetsOrbitingPlanetCanDetectedD, szT);
    c = _wsprintf(szWork, szT, abs(i));
    if ((i >= 0x0))
        goto L_63df;
    else
        goto L_6357;

L_6357:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);
    xText = rcData.left;
    /* untranslated: yText = (yText + (words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2)) */
    c = CchGetString(idsScannerCanDeterminePlanetsBasicStatsDistance, szT);
    /* untranslated: c = _wsprintf(szWork, szT, ((i neg 0x0) >> 0x1)) */
    ids = idsScannerWillUnavailableIfHaveLesserRacial;
    goto L_6410;

L_63df:
    ids = idsPlanetaryScannersDefensesAvailableAlternateReali;

L_63e7:
    if ((ppart->hs.iItem != iplanetaryGenesisDevice))
        goto L_6410;
    else
        goto L_63f8;

L_63f8:
    ids = idsOriginProcessUnknown;
    c = CchGetString(idsProcessGivesPlanetNewBirthTracesCivilization, szWork);

L_6410:
    if ((c <= 0x0))
        goto L_64d4;
    else
        goto L_6419;

L_6419:
    WrapTextOut(hdc, &(xText), &(yText), szWork, c, rcData.left, (rcData.right - rcData.left), 0x0, 0x0, 0x1);

L_6451:
    if ((ppart->hs.grhst == hstEngine))
        goto L_34c6;
    else
        goto L_6459;

L_6459:
    if ((ppart->hs.grhst == hstScanner))
        goto L_3aeb;
    else
        goto L_6461;

L_6461:
    if ((ppart->hs.grhst == hstShield))
        goto L_43e7;
    else
        goto L_6469;

L_6469:
    if ((ppart->hs.grhst == hstArmor))
        goto L_4518;
    else
        goto L_6471;

L_6471:
    if ((ppart->hs.grhst == hstBeam))
        goto L_4686;
    else
        goto L_6479;

L_6479:
    if ((ppart->hs.grhst == hstTorp))
        goto L_4a2f;
    else
        goto L_6481;

L_6481:
    if ((ppart->hs.grhst == hstBomb))
        goto L_4cc8;
    else
        goto L_6489;

L_6489:
    if ((ppart->hs.grhst == hstMining))
        goto L_5b56;
    else
        goto L_6491;

L_6491:
    if ((ppart->hs.grhst == hstMines))
        goto L_57fe;
    else
        goto L_6499;

L_6499:
    if ((ppart->hs.grhst == hstSpecialSB))
        goto L_5357;
    else
        goto L_64a1;

L_64a1:
    if ((ppart->hs.grhst == hstSBHull))
        goto L_40d6;
    else
        goto L_64a9;

L_64a9:
    if ((ppart->hs.grhst == hstSpecialE))
        goto L_4f85;
    else
        goto L_64b1;

L_64b1:
    if ((ppart->hs.grhst == hstSpecialM))
        goto L_51c4;
    else
        goto L_64b9;

L_64b9:
    if ((ppart->hs.grhst == hstTerra))
        goto L_5c6d;
    else
        goto L_64c1;

L_64c1:
    if ((ppart->hs.grhst == hstHull))
        goto L_3c94;
    else
        goto L_64c9;

L_64c9:
    if ((ppart->hs.grhst == hstPlanetary))
        goto L_5d93;
    else
        goto L_64d4;

L_64d4:
    if ((ids == 0xffff))
        goto L_65a7;
    else
        goto L_64dd;

L_64dd:
    c = CchGetString(ids, szWork);
    SetRect(&(rcT), 0x4, (dy - (dyArial8 * 0x2)), (dx + 0xfffc), (dy + 0xfffb));
    if ((dyArial8 <= 0xe))
        goto L_6531;
    else
        goto L_6522;

L_6522:
    SelectObject(hdc, rghfontArial6);
    goto L_653d;

L_6531:
    SelectObject(hdc, rghfontArial7);

L_653d:
    if ((LOWORD(l) != 0xffff))
        goto L_655b;
    else
        goto L_6549;

L_6549:
    if ((HIWORD(l) != 0xffff))
        goto L_655b;
    else
        goto L_6552;

L_6552:
    t_merge_6561_0001 = 0x7f;
    goto L_6561;

L_655b:
    t_merge_6561_0001 = 0x0;

L_6561:
    SetTextColor(hdc, t_merge_6561_0001);
    DrawText(hdc, szWork, c, &(rcT), 0x810);
    if ((LOWORD(l) != 0xffff))
        goto L_65a7;
    else
        goto L_658e;

L_658e:
    if ((HIWORD(l) != 0xffff))
        goto L_65a7;
    else
        goto L_6597;

L_6597:
    SetTextColor(hdc, 0x0);

L_65a7:
    return;
}

int32_t ProjectedResearchSpending(int32_t pct) {
    int32_t lRes;
    PLANET *lppl;
    int16_t cRes;
    int32_t lSpend;
    PLANET *lpplMac;
    char    pctSav;
    int16_t cBogus;

L_65ae:
    lSpend = 0x0;
    pctSav = rgplr[idPlayer].pctResearch;
    rgplr[idPlayer].pctResearch = LOBYTE(LOWORD(pct));
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_66b0;

L_6619:
    if ((lppl->iPlayer != idPlayer))
        goto L_66ac;
    else
        goto L_6625;

L_6625:

L_662b:
    lRes = (uint32_t)(CResourcesAtPlanet(lppl, idPlayer));
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_665b;
    else
        goto L_6651;

L_6651:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_6671;
    else
        goto L_665b;

L_665b:
    if ((lppl->lpplprod->iprodMac != 0x0))
        goto L_6680;
    else
        goto L_6671;

L_6671:
    lSpend = (lSpend + lRes);
    goto L_66ac;

L_6680:
    EstimateItemProdSched(lppl, 0x0, 0xffff, &(cRes), &(cBogus));
    lSpend = (lSpend + (uint32_t)(cRes));

L_66ac:
    lppl = (lppl + 0x1);

L_66b0:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_6619;
    else
        goto L_66be;

L_66be:
    rgplr[idPlayer].pctResearch = pctSav;

L_66d9:
    return lSpend;
}

int32_t CostOfDevelopingItem(char *rgTech) {
    int32_t  lSpent;
    char    *pTech;
    char     rgTechSav[6];
    int32_t  lCost;
    int16_t  fUnreachable;
    int16_t  i;
    int32_t  lCur;
    uint32_t t_merge_6879_0001_wide;

L_66e0:
    fUnreachable = 0x0;
    lCost = 0x0;
    pTech = rgplr[idPlayer].rgTech;
    i = 0x0;
    goto L_6734;

L_6714:
    if (((uint16_t)(rgTech[i]) > 0x1a))
        goto L_673d;
    else
        goto L_672a;

L_672a:

L_6730:
    i = (i + 0x1);

L_6734:
    if ((i < 0x6))
        goto L_6714;
    else
        goto L_673d;

L_673d:
    if ((i >= 0x6))
        goto L_674f;
    else
        goto L_6746;

L_6746:
    return 0xffffffff;

L_674f:
    i = 0x0;
    goto L_6883;

L_6757:
    rgTechSav[i] = pTech[i];
    if (((uint16_t)(rgTech[i]) <= (uint16_t)(pTech[i])))
        goto L_687f;
    else
        goto L_6796;

L_6796:
    lSpent = rgplr[idPlayer].rgResSpent[i];
    if ((game.fSlowTech == 0x0))
        goto L_67df;
    else
        goto L_67cb;

L_67cb:
    lSpent = (int32_t)((lSpent * 0x2));

L_67df:
    /* untranslated: LOWORD(lCur) = (LOWORD(lSpent) neg 0x0) */
    /* untranslated: HIWORD(lCur) = ((HIWORD(lSpent) + 0x0) neg 0x0) */

L_67f2:
    if (((uint16_t)(rgTech[i]) <= (uint16_t)(pTech[i])))
        goto L_684f;
    else
        goto L_681d;

L_681d:
    lCur = (lCur + GetTechLevelCost(i, ((uint16_t)(pTech[i]) + 0x1), idPlayer));
    pTech[i] = (pTech[i] + 0x1);
    goto L_67f2;

L_684f:
    if ((0x0 < HIWORD(lCur)))
        goto L_6873;
    else
        goto L_685d;

L_685d:
    if ((0x0 > HIWORD(lCur)))
        goto L_686a;
    else
        goto L_6862;

L_6862:
    if ((0x0 <= LOWORD(lCur)))
        goto L_6873;
    else
        goto L_686a;

L_686a:
    t_merge_6879_0001_wide = 0x0;
    goto L_6879;

L_6873:
    t_merge_6879_0001_wide = lCur;

L_6879:
    lCost = (lCost + t_merge_6879_0001_wide);

L_687f:
    i = (i + 0x1);

L_6883:
    if ((i < 0x6))
        goto L_6757;
    else
        goto L_688c;

L_688c:
    i = 0x0;
    goto L_68ac;

L_6894:
    pTech[i] = rgTechSav[i];
    i = (i + 0x1);

L_68ac:
    if ((i < 0x6))
        goto L_6894;
    else
        goto L_68b5;

L_68b5:
    return lCost;
}

int16_t FShouldPartBeHidden(PART *ppart) {
    int16_t  iItem;
    uint16_t grbitTrader;

L_68c4:
    if ((idPlayer != 0xffff))
        goto L_68dd;
    else
        goto L_68d7;

L_68d7:
    return 0x0;

L_68dd:
    grbitTrader = 0x0;
    iItem = ppart->hs.iItem;
    goto L_69c5;

L_68f6:
    if ((iItem != 0x12))
        goto L_6a20;
    else
        goto L_68ff;

L_68ff:
    grbitTrader = 0x80;

L_6907:
    if ((iItem != 0x7))
        goto L_6a20;
    else
        goto L_6910;

L_6910:
    grbitTrader = 0x40;

L_6918:
    if ((iItem != 0x9))
        goto L_6a20;
    else
        goto L_6921;

L_6921:
    grbitTrader = 0x8;

L_6929:
    if ((iItem != 0x6))
        goto L_6a20;
    else
        goto L_6932;

L_6932:
    grbitTrader = 0x4;

L_693a:
    if ((iItem != 0x8))
        goto L_6a20;
    else
        goto L_6943;

L_6943:
    grbitTrader = 0x20;

L_694b:
    if ((iItem != 0x6))
        goto L_6a20;
    else
        goto L_6954;

L_6954:
    grbitTrader = 0x10;

L_695c:
    if ((iItem != 0x8))
        goto L_6a20;
    else
        goto L_6965;

L_6965:
    grbitTrader = 0x200;

L_696d:
    if ((iItem != 0x1e))
        goto L_6a20;
    else
        goto L_6976;

L_6976:
    grbitTrader = 0x100;

L_697e:
    if ((iItem != 0x4))
        goto L_6a20;
    else
        goto L_6987;

L_6987:
    grbitTrader = 0x2;

L_698f:
    if ((iItem != 0x4))
        goto L_69a0;
    else
        goto L_6998;

L_6998:
    grbitTrader = 0x1;
    goto L_6a20;

L_69a0:
    if ((iItem != 0x9))
        goto L_6a20;
    else
        goto L_69a9;

L_69a9:
    grbitTrader = 0x800;

L_69b1:
    if ((iItem != 0xe))
        goto L_6a20;
    else
        goto L_69ba;

L_69ba:
    grbitTrader = 0x400;

L_69c5:
    if ((ppart->hs.grhst == hstEngine))
        goto L_695c;
    else
        goto L_69cd;

L_69cd:
    if ((ppart->hs.grhst == hstShield))
        goto L_6929;
    else
        goto L_69d5;

L_69d5:
    if ((ppart->hs.grhst == hstArmor))
        goto L_6918;
    else
        goto L_69dd;

L_69dd:
    if ((ppart->hs.grhst == hstBeam))
        goto L_68f6;
    else
        goto L_69e5;

L_69e5:
    if ((ppart->hs.grhst == hstTorp))
        goto L_6907;
    else
        goto L_69ed;

L_69ed:
    if ((ppart->hs.grhst == hstBomb))
        goto L_693a;
    else
        goto L_69f5;

L_69f5:
    if ((ppart->hs.grhst == hstMining))
        goto L_694b;
    else
        goto L_69fd;

L_69fd:
    if ((ppart->hs.grhst == hstSpecialE))
        goto L_697e;
    else
        goto L_6a05;

L_6a05:
    if ((ppart->hs.grhst == hstSpecialM))
        goto L_698f;
    else
        goto L_6a0d;

L_6a0d:
    if ((ppart->hs.grhst == hstHull))
        goto L_696d;
    else
        goto L_6a15;

L_6a15:
    if ((ppart->hs.grhst == hstPlanetary))
        goto L_69b1;
    else
        goto L_6a20;

L_6a20:
    if ((grbitTrader == 0x0))
        goto L_6a49;
    else
        goto L_6a29;

L_6a29:
    if (((rgplr[idPlayer].grbitTrader & grbitTrader) != 0x0))
        goto L_6a49;
    else
        goto L_6a43;

L_6a43:
    return 0x1;

L_6a49:
    return 0x0;
}
