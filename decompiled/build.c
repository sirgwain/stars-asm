#include "common.h"

uint16_t     rggrbitPartsSB[8] = {2620, 8, 16, 2048, 512, 4, 32, 48};
int16_t      rgidsPartsSB[8] = {1087, 1088, 1089, 1091, 1096, 1099, 1103, 1104};
uint16_t     rggrbitParts[13] = {6655, 8, 16, 64, 2048, 1, 4096, 256, 128, 2, 4, 32, 48};
int16_t      rgidsParts[13] = {1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1098, 1099, 1103, 1104};
HullSlotType rghstCat[14] = {hstWeapon, hstSpecialEM, hstArmor,  hstBeam,     hstBomb,     hstEngine, hstMines,
                             hstMining, hstScanner,   hstShield, hstSpecialE, hstSpecialM, hstTorp,   hstSpecialSB};
StringId     rgidsCat[14] = {idsWeapons2,     idsDevices,  idsArmor3,   idsBeamWeapons, idsBombs,      idsEngines,   idsMineLayers,
                             idsMiningRobots, idsScanners, idsShields3, idsElectrical,  idsMechanical, idsTorpedoes, idsOrbital};

int16_t ShipBuilder(POINT ptDlgSize) {
    int16_t (**lpProcSlot)();
    int16_t fSuccess;

L_008c:
    ptslotGlob.x = ptDlgSize.x;
    ptslotGlob.y = ptDlgSize.y;
    if ((gd.mdScreenSize <= 0x0))
        goto L_00c0;
    else
        goto L_00b5;

L_00b5:
    ptslotGlob.y = (ptslotGlob.y + LOWORD((0x3 * dyArial8)));

L_00c0:
    fStarbaseMode = 0;
    lpshdefBuild = NthValidShdef(0x0);
    lpProcSlot = MakeProcInstance(SlotDlg, hInst);
    fSuccess = DialogBox(hInst, IDD_SLOT, hwndFrame, &(lpProcSlot));
    FreeProcInstance(&(lpProcSlot));
    if ((sel.grobj != grobjPlanet))
        goto L_0153;
    else
        goto L_0123;

L_0123:
    if ((LOWORD(sel.pl.lpplprod) != 0x0))
        goto L_0137;
    else
        goto L_012d;

L_012d:
    if ((HIWORD(sel.pl.lpplprod) == 0x0))
        goto L_0153;
    else
        goto L_0137;

L_0137:
    FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);

L_0153:

L_0159:
    return 0x0;
}

void ShowMainControls(HWND hwnd, ShowWindowCmd sw) {
    uint16_t t_merge_0245_0001;
    uint16_t t_merge_0264_0001;

L_0160:
    ShowWindow(GetDlgItem(hwnd, IDC_IMPORT), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_EDIT), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_DELETE), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_PREV), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_NEXT2), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_FIRST), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_LAST), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_UP), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_DOWN), sw);
    GetDlgItem(hwnd, IDOK);
    if ((sw != SW_SHOW))
        goto L_0242;
    else
        goto L_023c;

L_023c:
    t_merge_0245_0001 = 0x0;
    goto L_0245;

L_0242:
    t_merge_0245_0001 = 0x5;

L_0245:
    /* untranslated: call ShowWindow(callresult(HWND), t_merge_0245_0001) -> callresult(int16_t) */
    if ((sw != SW_SHOW))
        goto L_0261;
    else
        goto L_025b;

L_025b:
    t_merge_0264_0001 = 0x2e2;
    goto L_0264;

L_0261:
    t_merge_0264_0001 = 0x2e3;

L_0264:
    SetDlgItemText(hwnd, IDCANCEL, PszGetCompressedString(t_merge_0264_0001));
    return;
}

int16_t FCheckQueuedShip(HWND hwnd, SHDEF *lpshdef, int16_t fEdit) {
    char     rgch[40];
    int16_t  fProgress;
    int16_t  id;
    StringId ids;
    int16_t  cshQueued;
    uint16_t t_merge_02f2_0001;
    char     t_merge_0342_0001[40];
    uint16_t t_merge_036d_0001;
    char     t_merge_03ca_0001[40];
    uint16_t t_merge_03e2_0001;
    uint16_t t_merge_043f_0001;

L_027c:
    cshQueued = CshQueued(lpshdef->ishdef, &(fProgress), fEdit);
    if ((HIWORD(lpshdef->cExist) > 0x0))
        goto L_02ce;
    else
        goto L_02b5;

L_02b5:
    if ((HIWORD(lpshdef->cExist) < 0x0))
        goto L_02c5;
    else
        goto L_02ba;

L_02ba:
    if ((LOWORD(lpshdef->cExist) > 0x0))
        goto L_02ce;
    else
        goto L_02c5;

L_02c5:
    if ((cshQueued == 0))
        goto L_0543;
    else
        goto L_02ce;

L_02ce:
    if ((fEdit == 0))
        goto L_02df;
    else
        goto L_02d7;

L_02d7:
    ids = idsCurrentlyHaveDSSIfDelete2;
    goto L_02f5;

L_02df:
    if ((fStarbaseMode == 0))
        goto L_02ef;
    else
        goto L_02e9;

L_02e9:
    t_merge_02f2_0001 = 0x26e;
    goto L_02f2;

L_02ef:
    t_merge_02f2_0001 = 0x26b;

L_02f2:
    ids = t_merge_02f2_0001;

L_02f5:
    CchGetString(idsWorkDone, rgch);
    if ((HIWORD(lpshdef->cExist) < 0x0))
        goto L_03ab;
    else
        goto L_0313;

L_0313:
    if ((HIWORD(lpshdef->cExist) > 0x0))
        goto L_0323;
    else
        goto L_0318;

L_0318:
    if ((LOWORD(lpshdef->cExist) <= 0x0))
        goto L_03ab;
    else
        goto L_0323;

L_0323:
    if ((cshQueued == 0))
        goto L_03ab;
    else
        goto L_032c;

L_032c:
    if ((fProgress == 0))
        goto L_033d;
    else
        goto L_0335;

L_0335:
    t_merge_0342_0001 = rgch;
    goto L_0342;

L_033d:
    t_merge_0342_0001 = "";

L_0342:
    if ((LOWORD(lpshdef->cExist) != 0x1))
        goto L_0368;
    else
        goto L_0355;

L_0355:
    if ((HIWORD(lpshdef->cExist) != 0x0))
        goto L_0368;
    else
        goto L_0360;

L_0360:
    t_merge_036d_0001 = 0xc85;
    goto L_036d;

L_0368:
    t_merge_036d_0001 = 0xc86;

L_036d:
    _wsprintf(szWork, PszGetCompressedString(ids), LOWORD(lpshdef->cExist), (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), t_merge_036d_0001, cshQueued,
              t_merge_0342_0001);
    goto L_047e;

L_03ab:
    if ((cshQueued == 0))
        goto L_0419;
    else
        goto L_03b4;

L_03b4:
    if ((fProgress == 0))
        goto L_03c5;
    else
        goto L_03bd;

L_03bd:
    t_merge_03ca_0001 = rgch;
    goto L_03ca;

L_03c5:
    t_merge_03ca_0001 = "";

L_03ca:
    if ((cshQueued != 1))
        goto L_03dd;
    else
        goto L_03d5;

L_03d5:
    t_merge_03e2_0001 = 0xc89;
    goto L_03e2;

L_03dd:
    t_merge_03e2_0001 = 0xc8a;

L_03e2:
    _wsprintf(szWork, PszGetCompressedString((ids + 1)), cshQueued, (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), t_merge_03e2_0001, t_merge_03ca_0001);
    goto L_047e;

L_0419:
    if ((LOWORD(lpshdef->cExist) != 0x1))
        goto L_043a;
    else
        goto L_0427;

L_0427:
    if ((HIWORD(lpshdef->cExist) != 0x0))
        goto L_043a;
    else
        goto L_0432;

L_0432:
    t_merge_043f_0001 = 0xc8c;
    goto L_043f;

L_043a:
    t_merge_043f_0001 = 0xc8d;

L_043f:
    _wsprintf(szWork, PszGetCompressedString((ids + 2)), LOWORD(lpshdef->cExist), (LOWORD(lpshdef) + 0x8), HIWORD(lpshdef), t_merge_043f_0001);

L_047e:
    id = MessageBox(GetFocus(), szWork, PszGetCompressedString((fEdit + 742)), 0x2024);
    SetFocus(hwnd);
    if ((id != 7))
        goto L_04c1;
    else
        goto L_04bb;

L_04bb:
    return 0x0;

L_04c1:
    if ((HIWORD(lpshdef->cExist) < 0x0))
        goto L_0528;
    else
        goto L_04cf;

L_04cf:
    if ((HIWORD(lpshdef->cExist) > 0x0))
        goto L_04df;
    else
        goto L_04d4;

L_04d4:
    if ((LOWORD(lpshdef->cExist) <= 0x0))
        goto L_0528;
    else
        goto L_04df;

L_04df:
    DestroyAllIshdef(lpshdef->ishdef, idPlayer);
    InvalidateRect(hwndScanner, 0x0, 0x1);
    InvalidateRect(hwndMessage, 0x0, 0x1);
    goto L_0543;

L_0528:
    RemoveIshdefFromAllQueues(lpshdef->ishdef, fEdit);

L_0543:
    return 0x1;
}

int16_t SlotDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT               rcWindow;
    HDC                hdc;
    RECT               rcGBox;
    SHDEF             *lpshdef;
    int16_t            left;
    PAINTSTRUCT        ps;
    HWND               hwndItem;
    int16_t            cch;
    int32_t            lSel;
    RECT               rc;
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    int16_t            i;
    POINT              pt;
    int16_t            fProtoSB;
    int16_t            fProgress;
    PART               part;
    int16_t            cshQueued;
    int16_t            j;
    uint16_t           t_merge_0bdb_0001;
    uint16_t           t_merge_0c80_0001;
    uint16_t           t_merge_0cc3_0001;
    uint16_t           t_merge_0cc3_0002;
    uint16_t           t_merge_0dce_0001;
    uint16_t           t_merge_0e24_0001;
    uint16_t           t_merge_0e84_0001;
    uint16_t           t_merge_107a_0001;
    uint16_t           t_merge_107a_0002;
    uint16_t           t_merge_1110_0001;
    uint16_t           t_merge_11a4_0001;
    uint16_t           t_merge_11fb_0001;
    uint16_t           t_128a;
    uint16_t           t_12f7;
    uint16_t           t_merge_13e5_0001;
    uint16_t           t_merge_178d_0001;
    uint16_t           t_merge_1c0b_0001;
    uint16_t           t_1c3c;
    uint16_t           t_merge_1d40_0001;
    uint16_t           t_merge_1fe4_0001;
    uint16_t           t_merge_23ac_0001;
    uint16_t           t_merge_2580_0001;
    uint16_t           t_merge_25d3_0001;

L_0550:
    goto L_25e6;

L_055f:
    fHullCopy = 0;
    hwndSlotDlg = hwnd;
    GetWindowRect(hwnd, &(rcWindow));
    GetClientRect(hwnd, &(rc));
    SetWindowPos(hwnd, 0x0, 0x0, 0x0, (((ptslotGlob.x + rcWindow.right) - rcWindow.left) - rc.right),
                 (((ptslotGlob.y + rcWindow.bottom) - rcWindow.top) - rc.bottom), 0x6);
    StickyDlgPos(hwnd, &(ptStickySlotDlg), 0x1);
    UpdateSlotGlobals();
    hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
    SetWindowPos(hwndItem, 0x0, (ptslotGlob.x - 256), 0x20, 0xf0, 0x10a, SWP_NOZORDER);
    FillBuildPartsLB(hwndItem, rggrbitParts[0]);
    yBuildInfoSum = 340;
    lpfnRealListProc = GetWindowLong(hwndItem, 0xfffc);
    SetWindowLong(hwndItem, 0xfffc, lpfnFakeListProc);
    CheckRadioButton(hwnd, 0x810, 0x811, 0x810);
    CheckRadioButton(hwnd, 0x812, 0x815, 0x812);
    mdBuild = mdBuildShdef;
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    SetWindowPos(hwndItem, 0x0, (ptslotGlob.x - 264), 0x8, 0xf0, 0x64, SWP_NOZORDER);
    FillBuildDD(hwndItem, mdBuild);
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDOK), 0x0, (ptslotGlob.x - 226), ((ptslotGlob.y - (words(loword((3 * dyArial8)), signhiword(loword((3 *
     * dyArial8)))) / 0x2)) + 0xfffa), 0x44, (words(loword((3 * dyArial8)), signhiword(loword((3 * dyArial8)))) / 0x2), 0x84) -> callresult(int16_t) */
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDCANCEL), 0x0, (ptslotGlob.x - 148), ((ptslotGlob.y - (words(loword((3 * dyArial8)),
     * signhiword(loword((3 * dyArial8)))) / 0x2)) + 0xfffa), 0x44, (words(loword((3 * dyArial8)), signhiword(loword((3 * dyArial8)))) / 0x2), SWP_NOZORDER) ->
     * callresult(int16_t) */
    /* untranslated: call SetWindowPos(GetDlgItem(hwnd, IDC_HELP), 0x0, (ptslotGlob.x - 74), ((ptslotGlob.y - (words(loword((3 * dyArial8)),
     * signhiword(loword((3 * dyArial8)))) / 0x2)) + 0xfffa), 0x44, (words(loword((3 * dyArial8)), signhiword(loword((3 * dyArial8)))) / 0x2), SWP_NOZORDER) ->
     * callresult(int16_t) */
    SetDlgItemText(hwnd, IDCANCEL, PszGetCompressedString(idsDone));
    if ((gd.fTutorial == 0x0))
        goto L_07c4;
    else
        goto L_07bf;

L_07bf:
    AdvanceTutor();

L_07c4:
    return 0x1;

L_07ca:
    lpdis = lParam;
    if ((lpdis->itemID != 0xffff))
        goto L_07f8;
    else
        goto L_07e3;

L_07e3:
    HandleFocusState(lpdis, 0xfffe);
    goto L_085f;

L_07f8:
    goto L_0844;

L_0802:
    DrawDlgLBEntireItem(lpdis, 0xfffc);
    goto L_085f;

L_0817:
    DrawDlgLBEntireItem(lpdis, 0xfffc);
    goto L_085f;

L_082c:
    DrawDlgLBEntireItem(lpdis, 0xfffc);
    goto L_085f;

L_0844:
    if ((lpdis->itemAction == 0x1))
        goto L_0802;
    else
        goto L_084c;

L_084c:
    if ((lpdis->itemAction == 0x2))
        goto L_0817;
    else
        goto L_0854;

L_0854:
    if ((lpdis->itemAction == 0x4))
        goto L_082c;
    else
        goto L_085f;

L_085f:
    return 0x1;

L_0865:
    lpmis = lParam;
    lpmis->itemHeight = 0x42;
    return 0x1;

L_0880:
    i = 2064;
    goto L_08ad;

L_0888:
    /* untranslated: ss:[bp-0x4e] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x4e] == GetDlgItem(hwnd, i) ? L_08b7 : L_08a3 */

L_08a3:

L_08a9:
    i = (i + 1);

L_08ad:
    if ((i <= 2069))
        goto L_0888;
    else
        goto L_08b7;

L_08b7:
    if ((i <= 2069))
        goto L_08e0;
    else
        goto L_08c1;

L_08c1:
    /* untranslated: ss:[bp-0x4e] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x4e] != GetDlgItem(hwnd, IDC_SHIPLIST) ? L_2641 : L_08e0 */

L_08e0:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_08f9:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((PtInRect(rgrcBuildSpin, pt) != 0))
        goto L_0948;
    else
        goto L_092e;

L_092e:
    if ((PtInRect(&(rgrcBuildSpin[0x1]), pt) == 0))
        goto L_2641;
    else
        goto L_0948;

L_0948:
    SetCursor(hcurHand);
    return 0x1;

L_095a:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0982:
    hdc = BeginPaint(hwnd, &(ps));
    if ((mdBuild == mdBuildEdit))
        goto L_0b66;
    else
        goto L_099e;

L_099e:
    GetWindowRect(GetDlgItem(hwnd, IDC_PREV), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_NEXT2), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    cch = CchGetString(idsDesign, szWork);
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    SelectObject(hdc, rghfontArial8[0]);
    GetWindowRect(GetDlgItem(hwnd, IDC_FIRST), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_DOWN), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[1]);
    cch = CchGetString(idsView, szWork);
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    SelectObject(hdc, rghfontArial8[0]);

L_0b66:
    GetClientRect(hwnd, &(rc));
    DrawSlotDlg(hwnd, hdc, &(rc), 0xffff);
    DrawBuildSelComp(hwnd, hdc, 0xffff);
    DrawBuildSelHull(hwnd, hdc, 0xffff, 0x0);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_0bc8:
    if ((message != WM_RBUTTONDOWN))
        goto L_0bd8;
    else
        goto L_0bd2;

L_0bd2:
    t_merge_0bdb_0001 = 0x1;
    goto L_0bdb;

L_0bd8:
    t_merge_0bdb_0001 = 0x0;

L_0bdb:
    return FTrackSlot(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), wParam, 0x0, t_merge_0bdb_0001);

L_0c0e:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0d21;
    else
        goto L_0c2a;

L_0c2a:
    if ((wParam < 0x810))
        goto L_0d21;
    else
        goto L_0c34;

L_0c34:
    if ((wParam > 0x811))
        goto L_0d21;
    else
        goto L_0c3e;

L_0c3e:
    fStarbaseMode = (wParam - 2064);
    wParam = (mdBuild + 2066);
    GetClientRect(hwnd, &(rc));
    if (((rc.right >> 0x1) >= (rc.right - 352)))
        goto L_0c7a;
    else
        goto L_0c72;

L_0c72:
    t_merge_0c80_0001 = (rc.right >> 0x1);
    goto L_0c80;

L_0c7a:
    t_merge_0c80_0001 = (rc.right - 352);

L_0c80:
    rc.left = t_merge_0c80_0001;
    InvalidateRect(hwnd, &(rc), 0x1);
    lpshdefBuild = 0x0;
    fHullCopy = 0;
    if ((fStarbaseMode == 0))
        goto L_0cbc;
    else
        goto L_0cb2;

L_0cb2:
    t_merge_0cc3_0001 = rggrbitPartsSB[0];
    t_merge_0cc3_0002 = 0x0;
    goto L_0cc3;

L_0cbc:
    t_merge_0cc3_0001 = rggrbitParts[0];
    t_merge_0cc3_0002 = 0x0;

L_0cc3:
    LOWORD(lSel) = t_merge_0cc3_0001;
    HIWORD(lSel) = t_merge_0cc3_0002;
    FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), LOWORD(lSel));
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    FillBuildDD(hwndItem, mdBuild);
    SendMessage(hwndItem, CB_SETCURSEL, 0x0, 0x0);
    goto FixupShip;

L_0d21:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_0eb2;
    else
        goto L_0d3d;

L_0d3d:
    if ((wParam < 0x812))
        goto L_0eb2;
    else
        goto L_0d47;

L_0d47:
    if ((wParam > 0x815))
        goto L_0eb2;
    else
        goto L_0d51;

L_0d51:
    lSel = 0;

LRestart:
    lpshdefBuild = 0x0;
    fHullCopy = 0;
    mdBuild = (wParam + 0xf7ee);
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    UpdateSlotGlobals();
    FillBuildDD(hwndItem, mdBuild);
    SendMessage(hwndItem, CB_SETCURSEL, LOWORD(lSel), 0x0);
    if ((wParam != 0x815))
        goto L_0dcb;
    else
        goto L_0dc5;

L_0dc5:
    t_merge_0dce_0001 = 0x0;
    goto L_0dce;

L_0dcb:
    t_merge_0dce_0001 = 0x8;

L_0dce:
    SetWindowPos(hwndItem, 0x0, ((ptslotGlob.x - 256) - t_merge_0dce_0001), 0x8, 0x0, 0x0, 0x5);
    GetClientRect(hwnd, &(rc));
    left = rc.left;
    if (((rc.right >> 0x1) >= (rc.right - 352)))
        goto L_0e1e;
    else
        goto L_0e16;

L_0e16:
    t_merge_0e24_0001 = (rc.right >> 0x1);
    goto L_0e24;

L_0e1e:
    t_merge_0e24_0001 = (rc.right - 352);

L_0e24:
    rc.left = t_merge_0e24_0001;
    InvalidateRect(hwnd, &(rc), 0x1);
    rc.right = rc.left;
    rc.left = left;
    rc.top = yBuildInfoSum;
    InvalidateRect(hwnd, &(rc), 0x1);
    hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
    if ((mdBuild != mdBuildComp))
        goto L_0e81;
    else
        goto L_0e7b;

L_0e7b:
    t_merge_0e84_0001 = 0x5;
    goto L_0e84;

L_0e81:
    t_merge_0e84_0001 = 0x0;

L_0e84:
    ShowWindow(hwndItem, t_merge_0e84_0001);
    if ((wParam != 0x815))
        goto FixupShip;
    else
        goto L_0e91;

L_0e91:

L_0e97:
    if ((gd.fTutorial == 0x0))
        goto L_2641;
    else
        goto L_0eaa;

L_0eaa:
    AdvanceTutor();

L_0eb2:
    if ((wParam != 0x81b))
        goto L_0fbc;
    else
        goto L_0ebc;

L_0ebc:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x400))
        goto L_0fbc;
    else
        goto L_0ed8;

L_0ed8:
    if ((fInEditUpdate != 0))
        goto L_0fbc;
    else
        goto L_0ee2;

L_0ee2:
    fInEditUpdate = 1;
    GetWindowText(LOWORD(lParam), szWork, 0xfa);
    lSel = SendMessage(LOWORD(lParam), WM_USER, 0x0, 0x0);
    if ((FStringFitsScreen(szWork, 0xa0) != 0))
        goto L_0f69;
    else
        goto L_0f3c;

L_0f3c:
    SetWindowText(LOWORD(lParam), szWork);
    SendMessage(LOWORD(lParam), CB_LIMITTEXT, 0x0, lSel);

L_0f69:
    lstrcpy(lpshdefBuild->hul.szClass, szWork);
    DrawBuildSelHull(hwnd, 0x0, 0x100, 0x0);
    fInEditUpdate = 0;
    if ((gd.fTutorial == 0x0))
        goto L_2641;
    else
        goto L_0fb4;

L_0fb4:
    AdvanceTutor();

L_0fbc:
    if ((wParam != 0x81a))
        goto L_14a0;
    else
        goto L_0fc6;

L_0fc6:
    goto L_1492;

FixupShip:
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0x0, 0x0);
    if ((mdBuild == mdBuildComp))
        goto L_101e;
    else
        goto L_1014;

L_1014:
    if ((mdBuild != mdBuildEdit))
        goto L_109f;
    else
        goto L_101e;

L_101e:
    if ((LOWORD(lSel) != 0xffff))
        goto L_103d;
    else
        goto L_1027;

L_1027:
    if ((HIWORD(lSel) != 0xffff))
        goto L_103d;
    else
        goto L_1030;

L_1030:
    lSel = 0;
    goto L_1080;

L_103d:
    if ((fStarbaseMode == 0))
        goto L_1062;
    else
        goto L_1047;

L_1047:
    t_merge_107a_0001 = rggrbitPartsSB[lSel];
    t_merge_107a_0002 = 0x0;
    goto L_107a;

L_1062:
    t_merge_107a_0001 = rggrbitParts[lSel];
    t_merge_107a_0002 = 0x0;

L_107a:
    LOWORD(lSel) = t_merge_107a_0001;
    HIWORD(lSel) = t_merge_107a_0002;

L_1080:
    FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), LOWORD(lSel));
    goto L_13fb;

L_109f:
    if ((mdBuild == mdBuildShdef))
        goto L_10bd;
    else
        goto L_10a9;

L_10a9:
    if ((mdBuild == mdBuildHuldef))
        goto L_10bd;
    else
        goto L_10b3;

L_10b3:
    if ((mdBuild != mdBuildEnemyShdef))
        goto L_13fb;
    else
        goto L_10bd;

L_10bd:
    if ((LOWORD(lSel) != 0xffff))
        goto L_10d2;
    else
        goto L_10c6;

L_10c6:
    if ((HIWORD(lSel) == 0xffff))
        goto LClearSelection;
    else
        goto L_10cc;

L_10cc:

L_10d2:
    if ((mdBuild != mdBuildShdef))
        goto L_1204;
    else
        goto L_10dc;

L_10dc:
    if ((fStarbaseMode == 0))
        goto L_110d;
    else
        goto L_10e6;

L_10e6:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_110d;
    else
        goto L_1107;

L_1107:
    t_merge_1110_0001 = 0x1;
    goto L_1110;

L_110d:
    t_merge_1110_0001 = 0x0;

L_1110:
    fProtoSB = t_merge_1110_0001;
    lpshdefBuild = NthValidShdef(LOWORD(lSel));
    CshQueued(lpshdefBuild->ishdef, &(fProgress), 0x0);
    GetDlgItem(hwndSlotDlg, IDC_EDIT);
    if ((LOWORD(lpshdefBuild->cExist) != 0x0))
        goto L_11a1;
    else
        goto L_1167;

L_1167:
    if ((HIWORD(lpshdefBuild->cExist) != 0x0))
        goto L_11a1;
    else
        goto L_1172;

L_1172:
    if ((fProgress != 0))
        goto L_11a1;
    else
        goto L_117b;

L_117b:
    if ((fProtoSB == 0))
        goto L_119b;
    else
        goto L_1184;

L_1184:
    if ((HIWORD(lSel) < 0x0))
        goto L_11a1;
    else
        goto L_118d;

L_118d:
    if ((HIWORD(lSel) > 0x0))
        goto L_119b;
    else
        goto L_1192;

L_1192:
    if ((LOWORD(lSel) <= 0x0))
        goto L_11a1;
    else
        goto L_119b;

L_119b:
    t_merge_11a4_0001 = 0x1;
    goto L_11a4;

L_11a1:
    t_merge_11a4_0001 = 0x0;

L_11a4:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_11a4_0001) -> callresult(int16_t) */
    if ((fProtoSB == 0))
        goto LClearSelection;
    else
        goto L_11b3;

L_11b3:
    GetDlgItem(hwndSlotDlg, IDC_DELETE);
    if ((HIWORD(lSel) < 0x0))
        goto L_11f8;
    else
        goto L_11ca;

L_11ca:
    if ((HIWORD(lSel) > 0x0))
        goto L_11d8;
    else
        goto L_11cf;

L_11cf:
    if ((LOWORD(lSel) <= 0x0))
        goto L_11f8;
    else
        goto L_11d8;

L_11d8:
    if ((LOWORD(lpshdefBuild->cExist) != 0x0))
        goto L_11f8;
    else
        goto L_11e7;

L_11e7:
    if ((HIWORD(lpshdefBuild->cExist) != 0x0))
        goto L_11f8;
    else
        goto L_11f2;

L_11f2:
    t_merge_11fb_0001 = 0x1;
    goto L_11fb;

L_11f8:
    t_merge_11fb_0001 = 0x0;

L_11fb:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_11fb_0001) -> callresult(int16_t) */

L_1204:
    if ((mdBuild != mdBuildEnemyShdef))
        goto L_124a;
    else
        goto L_120e;

L_120e:
    lpshdefBuild = NthValidEnemyShdef(LOWORD(lSel));
    if ((lpshdefBuild->det == 0x7))
        goto LClearSelection;
    else
        goto L_1234;

L_1234:

L_123a:
    i = lpshdefBuild->hul.ihuldef;
    goto LClearSelection;

L_124a:
    if ((fStarbaseMode == 0))
        goto L_12c1;
    else
        goto L_1254;

L_1254:
    part.hs.grhst = hstSBHull;
    i = 0;
    goto L_12b1;

L_1261:
    part.hs.iItem = i;
    if ((FLookupPart(&(part)) != 1))
        goto L_12ad;
    else
        goto L_1287;

L_1287:
    LOWORD(lSel) = (LOWORD(lSel) - 0x1);
    t_128a = HIWORD(lSel);
    HIWORD(lSel) = (HIWORD(lSel) - 0x0);
    if ((t_128a > 0x0))
        goto L_12ad;
    else
        goto L_129d;

L_129d:
    if ((HIWORD(lSel) < 0x0))
        goto L_12ba;
    else
        goto L_12a2;

L_12a2:
    if ((LOWORD(lSel) <= 0x0))
        goto L_12ba;
    else
        goto L_12a7;

L_12a7:

L_12ad:
    i = (i + 1);

L_12b1:
    if ((i < 5))
        goto L_1261;
    else
        goto L_12ba;

L_12ba:
    i = (i + 32);
    goto L_1327;

L_12c1:
    part.hs.grhst = hstHull;
    i = 0;
    goto L_131e;

L_12ce:
    part.hs.iItem = i;
    if ((FLookupPart(&(part)) != 1))
        goto L_131a;
    else
        goto L_12f4;

L_12f4:
    LOWORD(lSel) = (LOWORD(lSel) - 0x1);
    t_12f7 = HIWORD(lSel);
    HIWORD(lSel) = (HIWORD(lSel) - 0x0);
    if ((t_12f7 > 0x0))
        goto L_131a;
    else
        goto L_130a;

L_130a:
    if ((HIWORD(lSel) < 0x0))
        goto L_1327;
    else
        goto L_130f;

L_130f:
    if ((LOWORD(lSel) <= 0x0))
        goto L_1327;
    else
        goto L_1314;

L_1314:

L_131a:
    i = (i + 1);

L_131e:
    if ((i < 32))
        goto L_12ce;
    else
        goto L_1327;

L_1327:
    shdefBuild.hul = LphuldefFromId(i)->hul;
    shdefBuild.hul.ihuldef = (i & 0xff);
    i = 0;
    goto L_135f;

L_135b:
    i = (i + 1);

L_135f:
    /* untranslated: branch i >= (part[122:2](shdefBuild.hul) & 0xff) ? L_1394 : L_136d */

L_136d:
    HIWORD(shdefBuild.hul.rghs[i]) = (shdefBuild.hul.rghs[i].iItem | 0x0);
    goto L_135b;

L_1394:
    lpshdefBuild = &(shdefBuild);
    UpdateShdefCost(lpshdefBuild);

LClearSelection:
    UpdateSlotGlobals();
    GetClientRect(hwnd, &(rc));
    if (((rc.right >> 0x1) >= (rc.right - 352)))
        goto L_13df;
    else
        goto L_13d7;

L_13d7:
    t_merge_13e5_0001 = (rc.right >> 0x1);
    goto L_13e5;

L_13df:
    t_merge_13e5_0001 = (rc.right - 352);

L_13e5:
    rc.left = t_merge_13e5_0001;
    InvalidateRect(hwnd, &(rc), 0x1);

L_13fb:
    SetBuildSelection(0xfffe);
    DrawBuildSelHull(hwnd, 0x0, 0xffff, 0x0);
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_1477;
    else
        goto L_1428;

L_1428:
    if ((HIWORD(lpshdefBuild) != 0x0))
        goto L_1477;
    else
        goto L_1432;

L_1432:
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0x0);
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0x0);
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), 0x0);

L_1477:
    if ((gd.fTutorial == 0x0))
        goto L_2641;
    else
        goto L_148a;

L_148a:
    AdvanceTutor();

L_1492:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x1))
        goto L_2641;
    else
        goto L_1497;

L_1497:

L_14a0:
    if ((wParam != 0x80c))
        goto L_14d5;
    else
        goto L_14aa;

L_14aa:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x1))
        goto L_2641;
    else
        goto L_14c6;

L_14c6:
    SetBuildSelection(0xffff);

L_14d5:
    if ((wParam != 0x817))
        goto L_17b3;
    else
        goto L_14df;

L_14df:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_2641;
    else
        goto L_14fb;

L_14fb:
    fProgress = 0;
    cshQueued = 0;
    if ((gd.fTutorial == 0x0))
        goto L_152f;
    else
        goto L_1518;

L_1518:
    if ((FTutorialEnabledShipBuilder(0x0) == 0))
        goto L_2641;
    else
        goto L_1529;

L_1529:

L_152f:
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0x0, 0x0);
    lpshdef = NthValidShdef(LOWORD(lSel));
    if ((HIWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_157a;

L_157a:
    if ((HIWORD(lSel) > 0x0))
        goto L_1588;
    else
        goto L_157f;

L_157f:
    if ((LOWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_1588;

L_1588:
    if ((LOWORD(lpshdef) != 0x0))
        goto L_159d;
    else
        goto L_1591;

L_1591:
    if ((HIWORD(lpshdef) == 0x0))
        goto L_2641;
    else
        goto L_1597;

L_1597:

L_159d:
    if ((fStarbaseMode == 0))
        goto L_15dd;
    else
        goto L_15a7;

L_15a7:
    if ((LOWORD(lSel) != 0x0))
        goto L_15dd;
    else
        goto L_15b0;

L_15b0:
    if ((HIWORD(lSel) != 0x0))
        goto L_15dd;
    else
        goto L_15b9;

L_15b9:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raMacintosh))
        goto L_2641;
    else
        goto L_15d7;

L_15d7:

L_15dd:
    if ((FCheckQueuedShip(hwnd, lpshdef, 0x0) == 0))
        goto L_2641;
    else
        goto L_15f7;

L_15f7:

L_15fd:
    lpshdef->fFree = 0x1;
    lpshdef->cBuilt = 0x0;
    lpshdef->cExist = 0x0;
    if ((fStarbaseMode == 0))
        goto L_1680;
    else
        goto L_163c;

L_163c:
    /* untranslated: ss:[bp-0x50] = ((part[4:2](rgplr[idPlayer]) + 0xf000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[idPlayer]) = rgplr[idPlayer].cFleet */
    /* untranslated: part[4:2](rgplr[idPlayer]) = (part[4:2](rgplr[idPlayer]) | ss:[bp-0x50]) */
    goto L_1690;

L_1680:
    rgplr[idPlayer].cShDef = (rgplr[idPlayer].cShDef - 1);

L_1690:
    LogChangeShDef(lpshdef);
    FillBuildDD(hwndItem, mdBuild);
    lpshdefBuild = NthValidShdef(0x0);
    if ((fStarbaseMode == 0))
        goto L_16eb;
    else
        goto L_16ca;

L_16ca:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raMacintosh))
        goto L_16ff;
    else
        goto L_16eb;

L_16eb:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_1758;
    else
        goto L_16f5;

L_16f5:
    if ((HIWORD(lpshdefBuild) != 0x0))
        goto L_1758;
    else
        goto L_16ff;

L_16ff:
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0x0);
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0x0);
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_1758;
    else
        goto L_1737;

L_1737:
    if ((HIWORD(lpshdefBuild) != 0x0))
        goto L_1758;
    else
        goto L_1741;

L_1741:
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), 0x0);

L_1758:
    UpdateSlotGlobals();
    GetClientRect(hwnd, &(rc));
    if (((rc.right >> 0x1) >= (rc.right - 352)))
        goto L_1787;
    else
        goto L_177f;

L_177f:
    t_merge_178d_0001 = (rc.right >> 0x1);
    goto L_178d;

L_1787:
    t_merge_178d_0001 = (rc.right - 352);

L_178d:
    rc.left = t_merge_178d_0001;
    InvalidateRect(hwnd, &(rc), 0x1);
    if ((fHullCopy != 0))
        goto LRestart;
    else
        goto L_17aa;

L_17aa:

L_17b3:
    if ((wParam != 0x816))
        goto L_1e55;
    else
        goto L_17bd;

L_17bd:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_1e3a;
    else
        goto L_17d9;

L_17d9:
    if ((gd.fTutorial == 0x0))
        goto L_1803;
    else
        goto L_17ec;

L_17ec:
    if ((FTutorialEnabledShipBuilder(0x1) == 0))
        goto L_2641;
    else
        goto L_17fd;

L_17fd:

L_1803:
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0x0, 0x0);
    if ((HIWORD(lSel) > 0x0))
        goto L_184a;
    else
        goto L_1839;

L_1839:
    if ((HIWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_183e;

L_183e:
    if ((LOWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_1844;

L_1844:

L_184a:
    if ((fStarbaseMode == 0))
        goto L_189d;
    else
        goto L_1854;

L_1854:
    i = 0;
    goto L_1891;

L_185c:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_18d3;
    else
        goto L_1887;

L_1887:

L_188d:
    i = (i + 1);

L_1891:
    if ((i >= 10))
        goto L_18d3;
    else
        goto L_1897;

L_1897:

L_189d:
    i = 0;
    goto L_18ca;

L_18a5:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_18d3;
    else
        goto L_18c0;

L_18c0:

L_18c6:
    i = (i + 1);

L_18ca:
    if ((i < 16))
        goto L_18a5;
    else
        goto L_18d3;

L_18d3:
    if ((mdBuild == mdBuildShdef))
        goto L_18e7;
    else
        goto L_18dd;

L_18dd:
    if ((mdBuild != mdBuildEnemyShdef))
        goto L_1bd5;
    else
        goto L_18e7;

L_18e7:
    if ((mdBuild != mdBuildShdef))
        goto L_1923;
    else
        goto L_18f1;

L_18f1:
    lpshdef = NthValidShdef(LOWORD(lSel));
    if ((lpshdef->fGift != 0x0))
        goto LStripDown;
    else
        goto L_191a;

L_191a:

L_1923:
    lpshdef = NthValidEnemyShdef(LOWORD(lSel));

LStripDown:
    if ((fStarbaseMode == 0))
        goto L_196a;
    else
        goto L_1942;

L_1942:
    part.hs.grhst = hstSBHull;
    part.hs.iItem = (lpshdef->hul.ihuldef - 32);
    goto L_198c;

L_196a:
    part.hs.grhst = hstHull;
    part.hs.iItem = lpshdef->hul.ihuldef;

L_198c:
    if ((FLookupPart(&(part)) == 1))
        goto L_19c4;
    else
        goto L_19a0;

L_19a0:
    AlertSz(PszFormatIds(idsCantCopyShipDesignBecauseCantBuild, 0x0), MB_ICONHAND);
    goto L_2641;

L_19c4:
    if ((HIWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_19cd;

L_19cd:
    if ((HIWORD(lSel) > 0x0))
        goto L_19db;
    else
        goto L_19d2;

L_19d2:
    if ((LOWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_19db;

L_19db:
    if ((LOWORD(lpshdef) != 0x0))
        goto L_19f0;
    else
        goto L_19e4;

L_19e4:
    if ((HIWORD(lpshdef) == 0x0))
        goto L_2641;
    else
        goto L_19ea;

L_19ea:

L_19f0:
    if ((fStarbaseMode == 0))
        goto L_1a67;
    else
        goto L_19fa;

L_19fa:
    rglpshdefSB[idPlayer][i] = *(lpshdef);
    /* untranslated: ss:[bp-0x4e] = loword((0x93 * i)) */
    lpshdef = &(rglpshdefSB[idPlayer][i]);
    goto L_1aa6;

L_1a67:
    rgshdef[i] = *(lpshdef);
    lpshdef = &(rgshdef[i]);

L_1aa6:
    lpshdef->cExist = 0x0;
    lpshdef->cBuilt = 0x0;
    if ((mdBuild != mdBuildShdef))
        goto L_1b02;
    else
        goto L_1ad3;

L_1ad3:
    if ((lpshdef->fGift != 0x0))
        goto L_1b02;
    else
        goto L_1aea;

L_1aea:
    MakeNewName(lpshdef->hul.szClass);
    goto L_1d0f;

L_1b02:
    lpshdef->fGift = 0x0;
    j = 0;
    goto L_1b22;

L_1b1e:
    j = (j + 1);

L_1b22:
    if ((j >= lpshdef->hul.chs))
        goto L_1d0f;
    else
        goto L_1b34;

L_1b34:
    if ((lpshdef->hul.rghs[j].cItem <= 0x0))
        goto L_1b1e;
    else
        goto L_1b5e;

L_1b5e:
    part.hs.grhst = lpshdef->hul.rghs[j].grhst;
    HIWORD(part.hs) = HIWORD(lpshdef->hul.rghs[j]);
    if ((FLookupPart(&(part)) == 1))
        goto L_1b1e;
    else
        goto L_1b95;

L_1b95:
    HIWORD(lpshdef->hul.rghs[j]) = (lpshdef->hul.rghs[j].iItem | 0x0);

L_1bd5:
    if ((fStarbaseMode == 0))
        goto L_1be7;
    else
        goto L_1bdf;

L_1bdf:
    part.hs.grhst = hstSBHull;
    goto L_1bec;

L_1be7:
    part.hs.grhst = hstHull;

L_1bec:
    j = 0;
    goto L_1bf8;

L_1bf4:
    j = (j + 1);

L_1bf8:
    if ((fStarbaseMode == 0))
        goto L_1c08;
    else
        goto L_1c02;

L_1c02:
    t_merge_1c0b_0001 = 0x5;
    goto L_1c0b;

L_1c08:
    t_merge_1c0b_0001 = 0x20;

L_1c0b:
    if ((j >= t_merge_1c0b_0001))
        goto L_1c62;
    else
        goto L_1c13;

L_1c13:
    part.hs.iItem = j;
    if ((FLookupPart(&(part)) != 1))
        goto L_1bf4;
    else
        goto L_1c39;

L_1c39:
    LOWORD(lSel) = (LOWORD(lSel) - 0x1);
    t_1c3c = HIWORD(lSel);
    HIWORD(lSel) = (HIWORD(lSel) - 0x0);
    if ((t_1c3c > 0x0))
        goto L_1bf4;
    else
        goto L_1c4f;

L_1c4f:
    if ((HIWORD(lSel) < 0x0))
        goto L_1c62;
    else
        goto L_1c54;

L_1c54:
    if ((LOWORD(lSel) <= 0x0))
        goto L_1c62;
    else
        goto L_1c59;

L_1c59:

L_1c62:
    if ((fStarbaseMode == 0))
        goto L_1c91;
    else
        goto L_1c6c;

L_1c6c:
    lpshdef = &(rglpshdefSB[idPlayer][i]);
    j = (j + 32);
    goto L_1ca4;

L_1c91:
    lpshdef = &(rgshdef[i]);

L_1ca4:
    fmemset(&(lpshdef), 0x0, 0x93);
    lpshdef->hul = LphuldefFromId(j)->hul;
    lpshdef->det = 0x7;
    fmemset(&(lpshdef->hul.rghs), 0x0, 0x40);

L_1d0f:
    CheckRadioButton(hwnd, 0x812, 0x815, 0x812);
    lpshdef->turn = game.turn;
    if ((fStarbaseMode == 0))
        goto L_1d3d;
    else
        goto L_1d37;

L_1d37:
    t_merge_1d40_0001 = 0x10;
    goto L_1d40;

L_1d3d:
    t_merge_1d40_0001 = 0x0;

L_1d40:
    lpshdef->wFlags = ((lpshdef->wFlags & 0x83ff) | (((t_merge_1d40_0001 + i) & 0x1f) << 0xa));
    UpdateShdefCost(lpshdef);
    if ((fStarbaseMode == 0))
        goto L_1dc3;
    else
        goto L_1d7f;

L_1d7f:
    /* untranslated: ss:[bp-0x4e] = ((part[4:2](rgplr[idPlayer]) + 0x1000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[idPlayer]) = rgplr[idPlayer].cFleet */
    /* untranslated: part[4:2](rgplr[idPlayer]) = (part[4:2](rgplr[idPlayer]) | ss:[bp-0x4e]) */
    goto L_1dd3;

L_1dc3:
    rgplr[idPlayer].cShDef = (rgplr[idPlayer].cShDef + 1);

L_1dd3:
    LogChangeShDef(lpshdef);
    FillBuildDD(hwndItem, mdBuild);
    SendMessage(hwndItem, CB_SETCURSEL, i, 0x0);
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0x1);
    lpshdefBuild = lpshdef;
    UpdateSlotGlobals();
    fHullCopy = 1;
    goto EditDesign;

L_1e3a:
    if ((gd.fTutorial == 0x0))
        goto L_2641;
    else
        goto L_1e4d;

L_1e4d:
    AdvanceTutor();

L_1e55:
    if ((wParam != 0x818))
        goto L_20ff;
    else
        goto L_1e5f;

L_1e5f:
    if ((gd.fTutorial == 0x0))
        goto L_1e89;
    else
        goto L_1e72;

L_1e72:
    if ((FTutorialEnabledShipBuilder(0x2) == 0))
        goto L_2641;
    else
        goto L_1e83;

L_1e83:

L_1e89:
    if ((fStarbaseMode == 0))
        goto L_1ecf;
    else
        goto L_1e93;

L_1e93:
    if ((lpshdefBuild->ishdef != 0x10))
        goto L_1ecf;
    else
        goto L_1eab;

L_1eab:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) == raMacintosh))
        goto L_2641;
    else
        goto L_1ec9;

L_1ec9:

L_1ecf:
    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0x0, 0x0);
    lpshdef = NthValidShdef(LOWORD(lSel));
    if ((HIWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_1f1a;

L_1f1a:
    if ((HIWORD(lSel) > 0x0))
        goto L_1f28;
    else
        goto L_1f1f;

L_1f1f:
    if ((LOWORD(lSel) < 0x0))
        goto L_2641;
    else
        goto L_1f28;

L_1f28:
    if ((LOWORD(lpshdef) != 0x0))
        goto L_1f3d;
    else
        goto L_1f31;

L_1f31:
    if ((HIWORD(lpshdef) == 0x0))
        goto L_2641;
    else
        goto L_1f37;

L_1f37:

L_1f3d:
    if ((fStarbaseMode != 0))
        goto EditDesign;
    else
        goto L_1f47;

L_1f47:
    if ((FCheckQueuedShip(hwnd, lpshdef, 0x1) == 0))
        goto L_2641;
    else
        goto L_1f61;

L_1f61:

EditDesign:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_1f7e;
    else
        goto L_1f71;

L_1f71:
    if ((HIWORD(lpshdefBuild) == 0x0))
        goto L_2641;
    else
        goto L_1f78;

L_1f78:

L_1f7e:
    InvalidateRect(hwnd, 0x0, 0x1);
    mdBuild = mdBuildEdit;
    ishdefBuild = lpshdefBuild->ishdef;
    shdefBuild = *(lpshdefBuild);
    lpshdefBuild = &(shdefBuild);
    if ((fStarbaseMode == 0))
        goto L_1fe1;
    else
        goto L_1fdb;

L_1fdb:
    t_merge_1fe4_0001 = 0xa3c;
    goto L_1fe4;

L_1fe1:
    t_merge_1fe4_0001 = 0x19ff;

L_1fe4:
    FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), t_merge_1fe4_0001);
    FillBuildDD(GetDlgItem(hwnd, IDC_COMBOBOX), mdBuild);
    ShowMainControls(hwnd, SW_HIDE);
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x080C), 0x0, 0x10, 0x20, 0x0, 0x0, 0x45);
    SetWindowPos(GetDlgItem(hwnd, IDC_COMBOBOX), 0x0, 0x10, 0x8, 0x0, 0x0, 0x45);
    SetWindowPos(GetDlgItem(hwnd, IDC_EDITNAME), 0x0, (ptslotGlob.x - 264), 0x8, 0xf0, (LOWORD((3 * dyArial8)) >> 0x1), 0x44);
    SetWindowText(GetDlgItem(hwnd, IDC_EDITNAME), "");
    SendMessage(GetDlgItem(hwnd, IDC_EDITNAME), CB_SETEXTENDEDUI, 0x1f, 0x0);
    if ((gd.fTutorial == 0x0))
        goto L_2641;
    else
        goto L_20f7;

L_20f7:
    AdvanceTutor();

L_20ff:
    if ((wParam == 0x1))
        goto L_2111;
    else
        goto L_2108;

L_2108:
    if ((wParam != 0x2))
        goto L_25a7;
    else
        goto L_2111;

L_2111:
    if ((mdBuild != mdBuildEdit))
        goto L_2546;
    else
        goto L_211b;

L_211b:
    lSel = 0;
    if ((wParam != 0x1))
        goto L_2325;
    else
        goto L_212e;

L_212e:
    if ((fStarbaseMode != 0))
        goto L_2172;
    else
        goto L_2138;

L_2138:
    if ((shdefBuild.hul.rghs[0].cItem != 0x0))
        goto L_2172;
    else
        goto L_214b;

L_214b:
    AlertSz(PszFormatIds(idsShipDesignDoesHaveAnyEnginesMust, 0x0), MB_ICONHAND);
    return 0x0;

L_2172:
    if ((gd.fTutorial == 0x0))
        goto L_219c;
    else
        goto L_2185;

L_2185:
    if ((FTutorialEnabledShipBuilder(0x3) == 0))
        goto L_2641;
    else
        goto L_2196;

L_2196:

L_219c:
    GetWindowText(GetDlgItem(hwnd, IDC_EDITNAME), "", 0x20);
    shdefBuild.cBuilt = 0x0;
    shdefBuild.cExist = 0x0;
    shdefBuild.fFree = 0x0;
    UpdateShdefCost(shdefBuild);
    if ((fStarbaseMode == 0))
        goto L_22a6;
    else
        goto L_21f6;

L_21f6:
    ishdefBuild = (ishdefBuild - 16);
    rglpshdefSB[idPlayer][ishdefBuild] = shdefBuild;
    /* untranslated: ss:[bp-0x4e] = loword((0x93 * ishdefBuild)) */
    LogChangeShDef(shdefBuild);
    i = 0;
    goto L_2293;

L_2259:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_228f;
    else
        goto L_2287;

L_2287:
    lSel = (lSel + 1);

L_228f:
    i = (i + 1);

L_2293:
    if ((i < ishdefBuild))
        goto L_2259;
    else
        goto L_229e;

L_229e:
    ishdefBuild = (ishdefBuild + 16);
    goto L_234f;

L_22a6:
    rgshdef[ishdefBuild] = shdefBuild;
    LogChangeShDef(&(rgshdef[ishdefBuild]));
    i = 0;
    goto L_2317;

L_22ed:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_2313;
    else
        goto L_230b;

L_230b:
    lSel = (lSel + 1);

L_2313:
    i = (i + 1);

L_2317:
    if ((i >= ishdefBuild))
        goto L_234f;
    else
        goto L_231f;

L_231f:

L_2325:
    if ((gd.fTutorial == 0x0))
        goto L_234f;
    else
        goto L_2338;

L_2338:
    if ((FTutorialEnabledShipBuilder(0x4) == 0))
        goto L_2641;
    else
        goto L_2349;

L_2349:

L_234f:
    InvalidateRect(hwnd, 0x0, 0x1);
    ShowMainControls(hwnd, SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, IDC_EDITNAME), SW_HIDE);
    hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
    if ((fStarbaseMode == 0))
        goto L_23a8;
    else
        goto L_23a1;

L_23a1:
    t_merge_23ac_0001 = rggrbitPartsSB[0];
    goto L_23ac;

L_23a8:
    t_merge_23ac_0001 = rggrbitParts[0];

L_23ac:
    FillBuildPartsLB(hwndItem, t_merge_23ac_0001);
    SetWindowPos(hwndItem, 0x0, (ptslotGlob.x - 256), 0x20, 0xf0, 0x10a, SWP_NOZORDER);
    ShowWindow(GetDlgItem(hwnd, IDC_U16_0x080C), SW_HIDE);
    if ((fHullCopy == 0))
        goto L_253e;
    else
        goto L_23fb;

L_23fb:
    if ((wParam == 0x2))
        goto L_242b;
    else
        goto L_2404;

L_2404:
    if ((fStarbaseMode != 0))
        goto L_242b;
    else
        goto L_240e;

L_240e:
    if ((lpshdefBuild->hul.rghs[0].cItem != 0x0))
        goto L_242b;
    else
        goto L_2426;

L_2426:
    wParam = 0x2;

L_242b:
    if ((wParam != 0x2))
        goto L_253e;
    else
        goto L_2434;

L_2434:
    if ((fStarbaseMode == 0))
        goto L_24ec;
    else
        goto L_243e;

L_243e:
    shdefBuild.fFree = 0x1;
    rglpshdefSB[idPlayer][(ishdefBuild - 16)] = shdefBuild;
    /* untranslated: ss:[bp-0x4e] = ((part[4:2](rgplr[idPlayer]) + 0xf000) & 0xf000) */
    /* untranslated: part[4:2](rgplr[idPlayer]) = rgplr[idPlayer].cFleet */
    /* untranslated: part[4:2](rgplr[idPlayer]) = (part[4:2](rgplr[idPlayer]) | ss:[bp-0x4e]) */
    LogChangeShDef(shdefBuild);
    goto L_253e;

L_24ec:
    rgshdef[ishdefBuild].wFlags = ((rgshdef[ishdefBuild].wFlags & 0xfdff) | 0x200);
    rgplr[idPlayer].cShDef = (rgplr[idPlayer].cShDef - 1);
    LogChangeShDef(&(rgshdef[ishdefBuild]));

L_253e:
    wParam = 0x812;
    goto LRestart;

L_2546:
    SetBuildSelection(0xfffe);
    StickyDlgPos(hwnd, &(ptStickySlotDlg), 0x0);
    hwndSlotDlg = 0x0;
    if ((wParam != 0x1))
        goto L_257d;
    else
        goto L_2577;

L_2577:
    t_merge_2580_0001 = 0x1;
    goto L_2580;

L_257d:
    t_merge_2580_0001 = 0x0;

L_2580:
    EndDialog(hwnd, t_merge_2580_0001);
    if ((gd.fTutorial == 0x0))
        goto L_259e;
    else
        goto L_2599;

L_2599:
    AdvanceTutor();

L_259e:
    return 0x1;

L_25a7:
    if ((wParam != 0x76))
        goto L_2641;
    else
        goto L_25b0;

L_25b0:
    if ((mdBuild != mdBuildEdit))
        goto L_25cf;
    else
        goto L_25c8;

L_25c8:
    t_merge_25d3_0001 = 0xbdf;
    goto L_25d3;

L_25cf:
    t_merge_25d3_0001 = 0x42a;

L_25d3:
    /* untranslated: call WinHelp(hwnd, szHelpFile, 0x1, words(0x0, t_merge_25d3_0001)) -> callresult(int16_t) */
    return 0x1;

L_25e6:
    if ((message == WM_PAINT))
        goto L_0982;
    else
        goto L_25ee;

L_25ee:
    if ((message == WM_ERASEBKGND))
        goto L_095a;
    else
        goto L_25f6;

L_25f6:
    if ((message == WM_CTLCOLOR))
        goto L_0880;
    else
        goto L_25fe;

L_25fe:
    if ((message == WM_SETCURSOR))
        goto L_08f9;
    else
        goto L_2606;

L_2606:
    if ((message == WM_DRAWITEM))
        goto L_07ca;
    else
        goto L_260e;

L_260e:
    if ((message == WM_MEASUREITEM))
        goto L_0865;
    else
        goto L_2616;

L_2616:
    if ((message == WM_INITDIALOG))
        goto L_055f;
    else
        goto L_261e;

L_261e:
    if ((message == WM_COMMAND))
        goto L_0c0e;
    else
        goto L_2626;

L_2626:
    if ((message == WM_LBUTTONDOWN))
        goto L_0bc8;
    else
        goto L_262e;

L_262e:
    if ((message == WM_LBUTTONDBLCLK))
        goto L_0bc8;
    else
        goto L_2636;

L_2636:
    if ((message == WM_RBUTTONDOWN))
        goto L_0bc8;
    else
        goto L_2641;

L_2641:
    return 0x0;
}

void DrawSlotDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw) {
    int16_t  yTop;
    int16_t  iMax;
    int16_t  cSlot;
    int16_t  fCreatedDC;
    HDC      hdcMem;
    int16_t  c;
    int16_t  i;
    int16_t  bkMode;
    int16_t  j;
    int16_t  cItem;
    int16_t  ibmp;
    HBITMAP  hbmpSav;
    int16_t  xLeft;
    PART     part;
    HULDEF  *lphuldef;
    RECT     rc;
    int16_t  iInventSel;
    HPEN     hpenSav;
    HBRUSH   hbrSav;
    COLORREF crBkSav;
    uint16_t t_merge_27c5_0001;
    HBRUSH   t_merge_2916_0001;
    uint16_t t_merge_2bcc_0001;
    uint16_t t_merge_2bf1_0001;

L_2650:
    fCreatedDC = 0;
    if ((mdBuild == mdBuildComp))
        goto L_3064;
    else
        goto L_2668;

L_2668:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_267f;
    else
        goto L_2672;

L_2672:
    if ((HIWORD(lpshdefBuild) == 0x0))
        goto L_3064;
    else
        goto L_2679;

L_2679:

L_267f:
    lphuldef = LphuldefFromId(lpshdefBuild->hul.ihuldef);
    cSlot = lphuldef->hul.chs;
    if ((hdc != 0x0))
        goto L_26ba;
    else
        goto L_26aa;

L_26aa:
    fCreatedDC = 1;
    hdc = GetDC(hwnd);

L_26ba:
    if ((hwndSlotDlg != 0x0))
        goto L_26d1;
    else
        goto L_26c4;

L_26c4:
    xLeft = 4;
    yTop = 6;
    goto L_26df;

L_26d1:
    xLeft = (ptslotGlob.x - 338);
    yTop = 6;

L_26df:
    DrawFleetBitmap(0x0, hdc, xLeft, yTop, 0x1, lpshdefBuild->hul.ibmp, 0x0, 0x0, 0xffff, 0x0);
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpScanner);
    iInventSel = 0;
    if ((mdBuild == mdBuildEdit))
        goto L_2758;
    else
        goto L_273d;

L_273d:
    rgrcBuildSpin[1].top = -5;
    rgrcBuildSpin[0].top = -5;
    rgrcBuildSpin[1].bottom = -6;
    rgrcBuildSpin[0].bottom = -6;
    goto L_27f0;

L_2758:
    SetRect(rgrcBuildSpin, (xLeft + 21), (yTop + 69), (xLeft + 35), (yTop + 83));
    rgrcBuildSpin[1] = rgrcBuildSpin[0];
    OffsetRect(&(rgrcBuildSpin[0x1]), 0xe, 0x0);
    i = 0;
    goto L_27e7;

L_27ab:
    if ((i != 0))
        goto L_27c2;
    else
        goto L_27bc;

L_27bc:
    t_merge_27c5_0001 = 0x2;
    goto L_27c5;

L_27c2:
    t_merge_27c5_0001 = 0x3;

L_27c5:
    DrawBtn(hdc, rgrcBuildSpin[i], (t_merge_27c5_0001 | 0x20), 0x0, 0x0);
    i = (i + 1);

L_27e7:
    if ((i < 2))
        goto L_27ab;
    else
        goto L_27f0;

L_27f0:
    SelectObject(hdc, rghfontArial8[1]);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    if ((iDraw != -1))
        goto L_2822;
    else
        goto L_2814;

L_2814:
    i = 0;
    iMax = cSlot;
    goto L_2831;

L_2822:
    i = iDraw;
    iMax = (iDraw + 1);

L_2831:
    if ((lphuldef->hul.wtCargoMax == 0x0))
        goto L_2eed;
    else
        goto L_283e;

L_283e:
    rc = rcCargo;
    if ((fStarbaseMode == 0))
        goto L_28d0;
    else
        goto L_2857;

L_2857:
    if ((lphuldef->hul.ihuldef == ihuldefSpaceDock))
        goto L_286f;
    else
        goto L_2863;

L_2863:
    if ((lphuldef->hul.ihuldef != ihuldefDeathStart))
        goto L_28d0;
    else
        goto L_286f;

L_286f:
    hbrSav = SelectObject(hdc, hbrDock);
    hpenSav = SelectObject(hdc, GetStockObject(BLACK_PEN));
    Ellipse(hdc, (rc.left - 12), (rc.top - 12), (rc.right + 12), (rc.bottom + 12));
    SelectObject(hdc, hpenSav);
    SelectObject(hdc, hbrSav);
    goto L_291c;

L_28d0:
    FillRect(hdc, rcCargo, GetStockObject(BLACK_BRUSH));
    rc.top = (rc.top + 1);
    rc.bottom = (rc.bottom - 1);
    rc.left = (rc.left + 1);
    rc.right = (rc.right - 1);
    if ((fStarbaseMode == 0))
        goto L_2913;
    else
        goto L_290d;

L_290d:
    t_merge_2916_0001 = hbrDock;
    goto L_2916;

L_2913:
    t_merge_2916_0001 = hbrCargo;

L_2916:
    FillRect(hdc, &(rc), t_merge_2916_0001);

L_291c:
    rc.bottom = (((uint32_t)((rc.bottom - rc.top)) / 2) + rc.top);
    if ((fStarbaseMode != 0))
        goto L_29a0;
    else
        goto L_2938;

L_2938:
    RcCtrTextOut(hdc, &(rc), PszGetCompressedString(idsCargo3), 0x0);
    c = _wsprintf(szWork, PCTDKT, WtMaxShdefStat(lpshdefBuild, 0x2));
    RcCtrTextOut(hdc, &(rcCargo), szWork, 0x0);
    goto L_2a34;

L_29a0:
    if ((lphuldef->hul.wtCargoMax != 0xffff))
        goto L_29dd;
    else
        goto L_29b2;

L_29b2:
    if ((0x0 != 0x0))
        goto L_29dd;
    else
        goto L_29ba;

L_29ba:
    RcCtrTextOut(hdc, &(rc), PszGetCompressedString(idsUnlimited), 0x0);
    goto L_2a14;

L_29dd:
    c = _wsprintf(szWork, PCTDKT, lphuldef->hul.wtCargoMax);
    RcCtrTextOut(hdc, &(rc), szWork, 0x0);

L_2a14:
    RcCtrTextOut(hdc, &(rcCargo), PszGetCompressedString(idsSpace), 0x0);

L_2a34:
    rc.top = rc.bottom;
    rc.bottom = (rcCargo.bottom - 1);
    if ((fStarbaseMode == 0))
        goto L_2a70;
    else
        goto L_2a4d;

L_2a4d:
    RcCtrTextOut(hdc, &(rc), PszGetCompressedString(idsDock), 0x0);
    goto L_2eed;

L_2a70:
    RcCtrTextOut(hdc, &(rc), PszGetCompressedString(idsMax), 0x0);

L_2a93:
    FillRect(hdc, &(vrgrcSlot[i]), hbr50Screen);
    cItem = lpshdefBuild->hul.rghs[i].cItem;
    if ((cItem <= 0))
        goto L_2cc5;
    else
        goto L_2ae1;

L_2ae1:
    part.hs.grhst = lpshdefBuild->hul.rghs[i].grhst;
    HIWORD(part.hs) = HIWORD(lpshdefBuild->hul.rghs[i]);
    FLookupPart(&(part));
    ibmp = part.pcom->ibmp;
    iInventSel = (ibmp >> 0x5);
    DibBlt(hdc, vrgrcSlot[i].left, vrgrcSlot[i].top, 0x40, 0x40, rghdibInventory[iInventSel], ((ibmp & 0x7) * 0x40), ((0x3 - ((ibmp >> 0x3) & 0x3)) * 0x40),
           0x40, 0x40, 0xcc0020);
    if ((mdBuild == mdBuildEdit))
        goto L_2c30;
    else
        goto L_2ba3;

L_2ba3:
    SelectObject(hdcMem, hbmpScanner);
    j = 0;
    goto L_2c27;

L_2bb7:
    if ((j >= 2))
        goto L_2bc9;
    else
        goto L_2bc3;

L_2bc3:
    t_merge_2bcc_0001 = 0x3;
    goto L_2bcc;

L_2bc9:
    t_merge_2bcc_0001 = 0x39;

L_2bcc:
    if (((j & 0x1) == 0x0))
        goto L_2bee;
    else
        goto L_2be8;

L_2be8:
    t_merge_2bf1_0001 = 0x3;
    goto L_2bf1;

L_2bee:
    t_merge_2bf1_0001 = 0x39;

L_2bf1:
    BitBlt(hdc, (t_merge_2bcc_0001 + vrgrcSlot[i].left), (t_merge_2bf1_0001 + vrgrcSlot[i].top), 0x4, 0x4, hdcMem, 0x16, 0x21, SRCCOPY);
    j = (j + 1);

L_2c27:
    if ((j < 4))
        goto L_2bb7;
    else
        goto L_2c30;

L_2c30:
    if ((cItem != 1))
        goto L_2c7a;
    else
        goto L_2c39;

L_2c39:
    if ((lphuldef->hul.rghs[i].cItem != 0x1))
        goto L_2c7a;
    else
        goto L_2c63;

L_2c63:
    if ((part.hs.grhst != hstSpecialSB))
        goto L_2c7a;
    else
        goto L_2c6d;

L_2c6d:
    szWork[0] = 0;
    c = 0;
    goto L_2e03;

L_2c7a:
    c = _wsprintf(szWork, PszGetCompressedString(idsDD), cItem, lphuldef->hul.rghs[i].cItem);

L_2cc5:
    SelectObject(hdcMem, hbmpBackBld);
    ibmp = IEmptyBmpFromGrhst(lphuldef->hul.rghs[i].grhst);
    BitBlt(hdc, vrgrcSlot[i].left, vrgrcSlot[i].top, 0x40, 0x40, hdcMem, ((ibmp & 0x7) * 0x40), (((ibmp >> 0x3) & 0x3) * 0x40), SRCCOPY);
    iInventSel = -1;
    if (((lphuldef->hul.rghs[i].grhst & 0x1) == 0x0))
        goto L_2dbe;
    else
        goto L_2d76;

L_2d76:
    c = _wsprintf(szWork, PszGetCompressedString(idsNeedsD), lphuldef->hul.rghs[i].cItem);
    goto L_2e03;

L_2dbe:
    c = _wsprintf(szWork, PszGetCompressedString(idsD3), lphuldef->hul.rghs[i].cItem);

L_2e03:
    CtrTextOut(hdc, (vrgrcSlot[i].left + 32), ((vrgrcSlot[i].bottom - dyArial6) + 0xfffc), szWork, c);
    if ((iselSlot != i))
        goto L_2ee9;
    else
        goto L_2e4b;

L_2e4b:
    crBkSav = SetBkColor(hdc, 0xffffff);
    FrameRect(hdc, &(vrgrcSlot[i]), hbr50Screen);
    ExpandRc(vrgrcSlot[i], 0xffff, 0xffff);
    FrameRect(hdc, &(vrgrcSlot[i]), hbr50Screen);
    ExpandRc(vrgrcSlot[i], 0x1, 0x1);
    SetBkColor(hdc, crBkSav);

L_2ee9:
    i = (i + 1);

L_2eed:
    if ((i < iMax))
        goto L_2a93;
    else
        goto L_2ef8;

L_2ef8:
    if ((hwndPopup == 0x0))
        goto L_2f0c;
    else
        goto L_2f02;

L_2f02:
    /* untranslated: branch part[8:2](GlobalPD) != 0x0 ? L_3032 : L_2f0c */

L_2f0c:
    if ((mdBuild != mdBuildShdef))
        goto L_3032;
    else
        goto L_2f16;

L_2f16:
    SetRect(&(rc), ptPlaque.x, ptPlaque.y, (ptPlaque.x + 60), (ptPlaque.y + 30));
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    DibBlt(hdc, ptPlaque.x, ptPlaque.y, 0x3c, 0x1e, hdibPlaque, 0x0, 0x0, 0x3c, 0x1e, 0xcc0020);
    c = _wsprintf(szWork, PszGetCompressedString(idsLdLd), LOWORD(lpshdefBuild->cExist), HIWORD(lpshdefBuild->cExist), LOWORD(lpshdefBuild->cBuilt),
                  HIWORD(lpshdefBuild->cBuilt));
    SelectObject(hdc, rghfontArial8[1]);
    if ((LOWORD(GetTextExtent(hdc, szWork, c)) <= 0x32))
        goto L_301c;
    else
        goto L_2fea;

L_2fea:
    SelectObject(hdc, rghfontArial7);
    if ((LOWORD(GetTextExtent(hdc, szWork, c)) <= 0x32))
        goto L_301c;
    else
        goto L_3010;

L_3010:
    SelectObject(hdc, rghfontArial6);

L_301c:
    RcCtrTextOut(hdc, &(rc), szWork, c);

L_3032:
    SetBkMode(hdc, bkMode);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    if ((fCreatedDC == 0))
        goto L_3064;
    else
        goto L_3059;

L_3059:
    ReleaseDC(hwnd, hdc);

L_3064:
    return;
}

int16_t FTrackSlot(HWND hwnd, int16_t x, int16_t y, int16_t fkb, int16_t fListBox, int16_t fRightBtn) {
    HDC      hdc;
    POINT    ptOld;
    POINT    ptTileSize;
    int16_t  ibmpY;
    POINT    pt;
    int16_t  cSlot;
    int16_t  iSrc;
    POINT    ptDNew;
    int16_t  ibmpX;
    HDC      hdcMem;
    int16_t  i;
    HBITMAP  hbmpFullSav;
    RECT     rcStart;
    int16_t  fUseMem;
    HBITMAP  hbmpScreen;
    int16_t  ibmp;
    HBITMAP  hbmpOld;
    HDC      hdcMemFull;
    POINT    ptD;
    int16_t  iSel;
    HBITMAP  hbmpSav;
    HS       hs;
    int16_t  fFirst;
    PART     part;
    RECT     rc;
    int16_t  iDir;
    int16_t  yTop;
    int16_t  bt;
    BTNT     btnt;
    RECT    *prc;
    int16_t  iBase;
    int16_t  iCur;
    int16_t  xLeft;
    int16_t  dyStart;
    int16_t  dxStart;
    uint16_t t_merge_3703_0001;

L_306a:
    fFirst = 1;
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_3092;
    else
        goto L_3082;

L_3082:
    if ((HIWORD(lpshdefBuild) != 0x0))
        goto L_3092;
    else
        goto L_308c;

L_308c:
    return 0x0;

L_3092:
    pt.x = x;
    pt.y = y;
    cSlot = LphuldefFromId(lpshdefBuild->hul.ihuldef)->hul.chs;
    if ((fRightBtn != 0))
        goto L_31fc;
    else
        goto L_30c4;

L_30c4:
    if ((hwndSlotDlg == 0x0))
        goto L_31fc;
    else
        goto L_30ce;

L_30ce:
    if ((PtInRect(rgrcBuildSpin, pt) != 0))
        goto L_3102;
    else
        goto L_30e8;

L_30e8:
    if ((PtInRect(&(rgrcBuildSpin[0x1]), pt) == 0))
        goto L_31fc;
    else
        goto L_3102;

L_3102:
    if ((PtInRect(rgrcBuildSpin, pt) == 0))
        goto L_312e;
    else
        goto L_311c;

L_311c:
    iDir = -1;
    bt = 34;
    prc = rgrcBuildSpin;
    goto L_313d;

L_312e:
    iDir = 1;
    bt = 35;
    prc = &(rgrcBuildSpin[0x1]);

L_313d:
    iBase = lpshdefBuild->hul.ibmp;
    iCur = (iBase & 0x3);
    iBase = (iBase - iCur);
    xLeft = (ptslotGlob.x - 336);
    yTop = 8;
    InitBtnTrack(&(btnt), hwnd, 0x0, prc, bt, 0x50, 0x0, 0x0, 0x0);

L_318e:
    if ((FTrackBtn(&(btnt)) == 0))
        goto L_31e8;
    else
        goto L_31a2;

L_31a2:
    iCur = (((iCur + 0x4) + iDir) & 0x3);
    DrawFleetBitmap(0x0, btnt.hdc, xLeft, yTop, 0x0, (iBase + iCur), 0x0, 0x0, 0xffff, 0x0);
    goto L_318e;

L_31e8:
    lpshdefBuild->hul.ibmp = (iBase + iCur);
    return 0x1;

L_31fc:
    if ((fListBox != 0))
        goto L_32ee;
    else
        goto L_3205;

L_3205:
    GetClientRect(hwnd, &(rc));
    iSrc = 0;
    goto L_3248;

L_321c:
    if ((PtInRect(&(vrgrcSlot[iSrc]), pt) != 0))
        goto L_3253;
    else
        goto L_323e;

L_323e:

L_3244:
    iSrc = (iSrc + 1);

L_3248:
    if ((iSrc < cSlot))
        goto L_321c;
    else
        goto L_3253;

L_3253:
    if ((iSrc != cSlot))
        goto L_3264;
    else
        goto L_325e;

L_325e:
    return 0x0;

L_3264:
    hs.grhst = lpshdefBuild->hul.rghs[iSrc].grhst;
    HIWORD(hs) = HIWORD(lpshdefBuild->hul.rghs[iSrc]);
    if ((hs.cItem > 0x0))
        goto L_32ad;
    else
        goto L_329c;

L_329c:
    SetBuildSelection(iSrc);
    return 0x0;

L_32ad:
    part.hs.grhst = hs.grhst;
    HIWORD(part.hs) = HIWORD(hs);
    FLookupPart(&(part));
    ibmp = part.pcom->ibmp;
    rcStart = vrgrcSlot[iSrc];
    goto L_3559;

L_32ee:
    iSel = LOWORD(SendMessage(hwnd, CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((iSel != -1))
        goto L_3318;
    else
        goto L_3312;

L_3312:
    return 0x0;

L_3318:
    /* untranslated: call SendMessage(hwnd, CB_INSERTSTRING, iSel, words(ds, 0x57a4)) -> callresult(LRESULT) */
    ibmp = (((uint16_t)(szWork[0x2]) - 65) + LOWORD((((uint16_t)(szWork[0x3]) + 0xffbf) * 0x1a)));
    iSrc = -1;
    hs.grhst = (0x1 << ((uint16_t)(LOWORD(szWork)) + 0xffbf));
    hs.iItem = ((uint16_t)(szWork[0x1]) - 65);
    hs.cItem = 0x1;
    rcStart.left = 2;
    rcStart.right = 66;
    rcStart.top = (LOWORD((((uint32_t)(y) / 0x42) * 0x42)) + 1);
    rcStart.bottom = (rcStart.top + 64);
    ClientToScreen(hwnd, &(rcStart));
    ClientToScreen(hwnd, &(rcStart.right));
    ClientToScreen(hwnd, &(pt));
    hwnd = hwndSlotDlg;
    ScreenToClient(hwnd, &(rcStart));
    ScreenToClient(hwnd, &(rcStart.right));
    ScreenToClient(hwnd, &(pt));
    x = pt.x;
    y = pt.y;
    if ((hs.grhst != hstEngine))
        goto L_3480;
    else
        goto L_3429;

L_3429:
    if ((hs.iItem != iengineSettlersDelight))
        goto L_3480;
    else
        goto L_3437;

L_3437:
    if ((lpshdefBuild->hul.ihuldef == ihuldefMiniColonyShip))
        goto L_3480;
    else
        goto L_3444;

L_3444:
    GlobalPD.grPopup = grPopupString;
    GlobalPD.dxOut = 180;
    GlobalPD.psz = szPopupBuffer;
    CchGetString(idsSettlersDelightEngineMayMountedDesignsBased, szPopupBuffer);
    Popup(hwnd, x, y);
    return 0x1;

L_3480:
    if ((hs.grhst != hstSpecialM))
        goto L_34e1;
    else
        goto L_348a;

L_348a:
    if ((hs.iItem != ispecialMOrbitalConstructionModule))
        goto L_34e1;
    else
        goto L_3498;

L_3498:
    if ((lpshdefBuild->hul.ihuldef == ihuldefColonyShip))
        goto L_34e1;
    else
        goto L_34a5;

L_34a5:
    GlobalPD.grPopup = grPopupString;
    GlobalPD.dxOut = 180;
    GlobalPD.psz = szPopupBuffer;
    CchGetString(idsOrbitalConstructionModuleMayMountedDesignsBased, szPopupBuffer);
    Popup(hwnd, x, y);
    return 0x1;

L_34e1:
    if ((hs.grhst != hstSpecialE))
        goto L_3559;
    else
        goto L_34eb;

L_34eb:
    if ((hs.iItem != ispecialETransportCloaking))
        goto L_3559;
    else
        goto L_34f9;

L_34f9:
    if ((((*(LphuldefFromId(lpshdefBuild->hul.ihuldef) + 0x7b) >> 0x6) & 0xf) == 0x0))
        goto L_3559;
    else
        goto L_3520;

L_3520:
    GlobalPD.grPopup = grPopupString;
    GlobalPD.dxOut = 180;
    GlobalPD.psz = szPopupBuffer;
    CchGetString(idsTransportCloakingModuleMayPlacedHullCould, szPopupBuffer);
    Popup(hwnd, x, y);
    return 0x1;

L_3559:
    SetBuildSelection(iSrc);
    if ((fRightBtn == 0))
        goto L_3599;
    else
        goto L_356d;

L_356d:
    GlobalPD.part = part;
    GlobalPD.grPopup = grPopupComponent;
    Popup(hwnd, x, y);
    return 0x1;

L_3599:
    if ((mdBuild == mdBuildEdit))
        goto L_35a9;
    else
        goto L_35a3;

L_35a3:
    return 0x0;

L_35a9:
    ptTileSize.y = 64;
    ptTileSize.x = 64;
    ibmpX = (ibmp & 0x7);
    ibmpY = ((ibmp >> 0x3) & 0x3);
    hdc = GetDC(hwnd);
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    hdcMem = CreateCompatibleDC(hdc);
    hdcMemFull = CreateCompatibleDC(hdc);
    SelectPalette(hdcMemFull, vhpal, 0x0);
    RealizePalette(hdcMemFull);
    hbmpOld = CreateCompatibleBitmap(hdc, ptTileSize.x, ptTileSize.y);
    hbmpSav = SelectObject(hdcMem, hbmpOld);
    hbmpScreen = CreateCompatibleBitmap(hdc, LOWORD((0x3 * ptTileSize.x)), LOWORD((0x3 * ptTileSize.y)));
    hbmpFullSav = SelectObject(hdcMemFull, hbmpScreen);
    SetCapture(hwnd);
    ptOld.y = -1;
    ptOld.x = -1;

L_3676:
    if ((FGetMouseMove(&(pt)) == 0))
        goto L_39fc;
    else
        goto L_368a;

L_368a:
    if ((pt.x != ptOld.x))
        goto L_36a0;
    else
        goto L_3695;

L_3695:
    if ((pt.y == ptOld.y))
        goto L_3676;
    else
        goto L_36a0;

L_36a0:
    if ((fFirst == 0))
        goto L_36b7;
    else
        goto L_36a9;

L_36a9:
    fUseMem = 0;
    fFirst = 0;
    goto L_37a5;

L_36b7:
    SelectObject(hdcMem, hbmpOld);
    ptDNew.x = (pt.x - ptOld.x);
    ptDNew.y = (pt.y - ptOld.y);
    if ((abs(ptDNew.x) >= ptTileSize.x))
        goto L_3700;
    else
        goto L_36e7;

L_36e7:
    if ((abs(ptDNew.y) >= ptTileSize.y))
        goto L_3700;
    else
        goto L_36fa;

L_36fa:
    t_merge_3703_0001 = 0x1;
    goto L_3703;

L_3700:
    t_merge_3703_0001 = 0x0;

L_3703:
    fUseMem = t_merge_3703_0001;
    if ((fUseMem != 0))
        goto L_3741;
    else
        goto L_370f;

L_370f:
    BitBlt(hdc, (rcStart.left + ptD.x), (rcStart.top + ptD.y), ptTileSize.x, ptTileSize.y, hdcMem, 0x0, 0x0, SRCCOPY);
    goto L_37a5;

L_3741:
    BitBlt(hdcMemFull, 0x0, 0x0, LOWORD((3 * ptTileSize.x)), LOWORD((3 * ptTileSize.y)), hdc, ((rcStart.left + ptD.x) - ptTileSize.x),
           ((rcStart.top + ptD.y) - ptTileSize.y), SRCCOPY);
    BitBlt(hdcMemFull, ptTileSize.x, ptTileSize.y, ptTileSize.x, ptTileSize.y, hdcMem, 0x0, 0x0, SRCCOPY);

L_37a5:
    ptOld.x = pt.x;
    ptOld.y = pt.y;
    ptD.x = (pt.x - x);
    ptD.y = (pt.y - y);
    SelectObject(hdcMem, hbmpOld);
    if ((fUseMem != 0))
        goto L_3860;
    else
        goto L_37d7;

L_37d7:
    BitBlt(hdcMem, 0x0, 0x0, ptTileSize.x, ptTileSize.y, hdc, (rcStart.left + ptD.x), (rcStart.top + ptD.y), SRCCOPY);
    DibBlt(hdc, (rcStart.left + ptD.x), (rcStart.top + ptD.y), 0x40, 0x40, rghdibInventory[((uint32_t)(ibmp) / 32)], (ibmpX * 64), ((3 - ibmpY) * 0x40), 0x40,
           0x40, 0xcc0020);
    goto L_3988;

L_3860:
    dxStart = 0;
    dyStart = 0;
    BitBlt(hdcMem, 0x0, 0x0, ptTileSize.x, ptTileSize.y, hdcMemFull, (ptTileSize.x + ptDNew.x), (ptTileSize.y + ptDNew.y), SRCCOPY);
    DibBlt(hdcMemFull, (ptTileSize.x + ptDNew.x), (ptTileSize.y + ptDNew.y), 0x40, 0x40, rghdibInventory[((uint32_t)(ibmp) / 32)], (ibmpX * 64),
           ((3 - ibmpY) * 0x40), 0x40, 0x40, 0xcc0020);
    rc = rcStart;
    OffsetRc(&(rc), ptD.x, ptD.y);
    if ((ptDNew.x <= 0))
        goto L_3929;
    else
        goto L_391a;

L_391a:
    rc.left = (rc.left - ptDNew.x);
    dxStart = ptDNew.x;
    goto L_392f;

L_3929:
    rc.right = (rc.right - ptDNew.x);

L_392f:
    if ((ptDNew.y <= 0))
        goto L_3947;
    else
        goto L_3938;

L_3938:
    rc.top = (rc.top - ptDNew.y);
    dyStart = ptDNew.y;
    goto L_394d;

L_3947:
    rc.bottom = (rc.bottom - ptDNew.y);

L_394d:
    BitBlt(hdc, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hdcMemFull, ((ptTileSize.x + ptDNew.x) - dxStart),
           ((ptTileSize.y + ptDNew.y) - dyStart), SRCCOPY);

L_3988:
    i = IDropPart(pt, hs, iSrc, 0x1);
    if ((i < 0))
        goto L_39b8;
    else
        goto L_39af;

L_39af:
    if ((i != 2))
        goto L_39d2;
    else
        goto L_39b8;

L_39b8:
    SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f00)));
    goto L_3676;

L_39d2:
    if ((i != 1))
        goto L_39f0;
    else
        goto L_39db;

L_39db:
    if ((iSrc < 0))
        goto L_39f0;
    else
        goto L_39e4;

L_39e4:
    SetCursor(hcurTrashCan);
    goto L_3676;

L_39f0:
    SetCursor(hcurNoWay);

L_39fc:
    if ((fFirst != 0))
        goto L_3a41;
    else
        goto L_3a05;

L_3a05:
    SelectObject(hdcMem, hbmpOld);
    BitBlt(hdc, (rcStart.left + ptD.x), (rcStart.top + ptD.y), 0x40, 0x40, hdcMem, 0x0, 0x0, SRCCOPY);

L_3a41:
    ReleaseCapture();
    SelectObject(hdcMem, hbmpSav);
    SelectObject(hdcMemFull, hbmpFullSav);
    DeleteObject(hbmpOld);
    DeleteObject(hbmpScreen);
    DeleteDC(hdcMem);
    DeleteDC(hdcMemFull);
    i = IDropPart(pt, hs, iSrc, 0x0);
    ReleaseDC(hwnd, hdc);
    return 0x1;
}

void DrawBuildSelComp(HWND hwnd, HDC hdc, int16_t iDraw) {
    uint16_t grhst;
    HS       hsShip;
    uint16_t rgCosts[4];
    int16_t  fCreatedDC;
    int16_t  c;
    int16_t  i;
    COLORREF crForeSav;
    int16_t  fPlural;
    int16_t  k;
    char     szWord[80];
    COLORREF crBackSav;
    HS       hsHul;
    int16_t  cch;
    PART     part;
    int16_t  x;
    int16_t  dxkT;
    RECT     rc;
    int16_t  iSel;
    char    *pch;
    uint16_t t_merge_3caf_0001;
    uint16_t t_merge_3cd7_0001;
    uint16_t t_merge_3f7f_0001;

L_3ab2:
    fCreatedDC = 0;
    if ((hdc != 0x0))
        goto L_3ad9;
    else
        goto L_3ac9;

L_3ac9:
    fCreatedDC = 1;
    hdc = GetDC(hwnd);

L_3ad9:
    GetClientRect(hwndSlotDlg, &(rc));
    rc.bottom = (rc.bottom - 8);
    rc.left = 8;
    rc.right = (rc.left + 256);
    rc.top = yBuildInfoSum;
    SelectObject(hdc, rghfontArial8[1]);
    crForeSav = SetTextColor(hdc, 0x0);
    crBackSav = SetBkColor(hdc, crButtonFace);
    FillRect(hdc, &(rc), hbrButtonFace);
    if ((fStarbaseMode == 0))
        goto L_3b64;
    else
        goto L_3b5d;

L_3b5d:
    rc.top = (rc.top + dyArial8);

L_3b64:
    if ((iselSlot == -2))
        goto Restore;
    else
        goto L_3b6b;

L_3b6b:

L_3b71:
    if ((iselSlot != -1))
        goto L_3c15;
    else
        goto L_3b7b;

L_3b7b:
    iSel = LOWORD(SendMessage(GetDlgItem(hwndSlotDlg, IDC_U16_0x080C), CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((iSel == -1))
        goto Restore;
    else
        goto L_3ba9;

L_3ba9:

L_3baf:
    /* untranslated: call SendMessage(GetDlgItem(hwndSlotDlg, IDC_U16_0x080C), CB_INSERTSTRING, iSel, words(ds, 0x57a4)) -> callresult(LRESULT) */
    hsShip.cItem = 0x1;
    hsShip.grhst = (0x1 << ((uint16_t)(LOWORD(szWork)) + 0xffbf));
    hsShip.iItem = ((uint16_t)(szWork[0x1]) - 65);
    goto HullPart;

L_3c15:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_3c2c;
    else
        goto L_3c1f;

L_3c1f:
    if ((HIWORD(lpshdefBuild) == 0x0))
        goto Restore;
    else
        goto L_3c26;

L_3c26:

L_3c2c:
    hsShip.grhst = lpshdefBuild->hul.rghs[iselSlot].grhst;
    HIWORD(hsShip) = HIWORD(lpshdefBuild->hul.rghs[iselSlot]);
    LphuldefFromId(lpshdefBuild->hul.ihuldef);
    /* untranslated: hsHul.grhst = callresult(HULDEF *)->hul.rghs[iselSlot].grhst */
    /* untranslated: HIWORD(hsHul) = HIWORD(callresult(HULDEF *)->hul.rghs[iselSlot]) */
    if ((hsShip.cItem != 0x0))
        goto HullPart;
    else
        goto L_3c94;

L_3c94:
    if (((hsHul.grhst & 0x1) == 0x0))
        goto L_3cac;
    else
        goto L_3ca6;

L_3ca6:
    t_merge_3caf_0001 = 0x2f9;
    goto L_3caf;

L_3cac:
    t_merge_3caf_0001 = 0x2fa;

L_3caf:
    i = CchGetString(t_merge_3caf_0001, szWork);
    if ((hsHul.cItem == 0x1))
        goto L_3cd4;
    else
        goto L_3cce;

L_3cce:
    t_merge_3cd7_0001 = 0x1;
    goto L_3cd7;

L_3cd4:
    t_merge_3cd7_0001 = 0x0;

L_3cd7:
    fPlural = t_merge_3cd7_0001;
    if ((fPlural != 0))
        goto L_3cfc;
    else
        goto L_3ce3;

L_3ce3:
    i = (i + CchGetString(idsOne, szWork[i]));
    goto L_3d24;

L_3cfc:
    i = (i + _wsprintf(&(szWork[i]), "%d ", hsHul.cItem));

L_3d24:
    grhst = hsHul.grhst;
    goto L_3eda;

L_3d2d:
    i = 0;
    goto L_3d67;

L_3d35:
    if (((grhst & rghstCat[i]) != rghstCat[i]))
        goto L_3d63;
    else
        goto L_3d51;

L_3d51:
    /* untranslated: grhst = (grhst & (rghstCat[i] ~ 0x0)) */
    goto L_3d70;

L_3d63:
    i = (i + 1);

L_3d67:
    if ((i < 14))
        goto L_3d35;
    else
        goto L_3d70;

L_3d70:
    cch = CchGetString(rgidsCat[i], szWord);
    if ((fPlural != 0))
        goto L_3e84;
    else
        goto L_3d92;

L_3d92:
    if (((uint16_t)(szWord[(cch - 1)]) != 115))
        goto L_3df6;
    else
        goto L_3da8;

L_3da8:
    if (((uint16_t)(szWord[(cch - 2)]) != 101))
        goto L_3de5;
    else
        goto L_3dbe;

L_3dbe:
    if (((uint16_t)(szWord[(cch - 3)]) != 111))
        goto L_3de5;
    else
        goto L_3dd4;

L_3dd4:
    szWord[(cch - 2)] = 0;
    goto L_3e84;

L_3de5:
    szWord[(cch - 1)] = 0;

L_3df6:
    pch = szWord[(cch - 1)];
    goto L_3e21;

L_3e0a:
    if (((uint16_t)(*(pch)) == 40))
        goto L_3e2d;
    else
        goto L_3e16;

L_3e16:

L_3e1c:
    pch = (pch - 0x1);

L_3e21:
    if ((pch > szWord))
        goto L_3e0a;
    else
        goto L_3e2d;

L_3e2d:
    if (((uint16_t)(*(pch)) != 40))
        goto L_3e84;
    else
        goto L_3e3c;

L_3e3c:
    if ((pch <= &(szWord[0x1])))
        goto L_3e84;
    else
        goto L_3e48;

L_3e48:
    if (((uint16_t)(*(pch - 0x1)) != 32))
        goto L_3e84;
    else
        goto L_3e58;

L_3e58:
    if (((uint16_t)(*(pch - 0x2)) != 115))
        goto L_3e84;
    else
        goto L_3e68;

L_3e68:
    strcpy(&(pch[0xfffe]), &(pch[0xffff]));

L_3e84:
    if ((grhst == 0x0))
        goto L_3eca;
    else
        goto L_3e8d;

L_3e8d:
    if ((((grhst + 0xffff) & grhst) == 0x0))
        goto L_3eb1;
    else
        goto L_3e9e;

L_3e9e:
    strcat(szWord, ", ");
    goto L_3eca;

L_3eb1:
    strcat(szWord, PszGetCompressedString(idsOr));

L_3eca:
    strcat(szWork, szWord);

L_3eda:
    if ((grhst != 0x0))
        goto L_3d2d;
    else
        goto L_3ee3;

L_3ee3:
    x = rc.left;
    WrapTextOut(hdc, &(x), &(rc.top), szWork, 0x0, rc.left, (rc.right - rc.left), 0x0, 0x0, 0x1);
    rc.top = (rc.top + dyArial8);
    goto Restore;

HullPart:
    part.hs.grhst = hsShip.grhst;
    HIWORD(part.hs) = HIWORD(hsShip);
    FLookupPart(&(part));
    dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 0x2));
    if ((hsShip.cItem == 0x1))
        goto L_3f7c;
    else
        goto L_3f76;

L_3f76:
    t_merge_3f7f_0001 = 0x1;
    goto L_3f7f;

L_3f7c:
    t_merge_3f7f_0001 = 0x0;

L_3f7f:
    fPlural = t_merge_3f7f_0001;
    if ((fPlural != 0))
        goto L_3f9e;
    else
        goto L_3f8b;

L_3f8b:
    CchGetString(idsOne, szWord);
    goto L_3fc0;

L_3f9e:
    _wsprintf(szWord, "%d ", hsShip.cItem);

L_3fc0:
    /* untranslated: call fstrcat(szWord, &part[8:0](part)) -> callresult(char *) */
    if ((fPlural == 0))
        goto L_3ff5;
    else
        goto L_3fe5;

L_3fe5:
    strcat(szWord, "s");

L_3ff5:
    cch = _wsprintf(szWork, PszGetCompressedString(idsCostS), &(szWord));
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.left = (rc.left + 8);
    rc.right = (rc.right - 8);
    c = hsShip.cItem;
    GetTruePartCost(idPlayer, &(part), rgCosts);
    k = 0;
    goto L_416f;

L_406c:
    rc.top = (rc.top + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, rgcrMinerals[k]);
    TextOut(hdc, rc.left, rc.top, rgszMinerals[k], lstrlen(rgszMinerals[k]));
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crWindowText);
    /* untranslated: cch = _wsprintf(szWork, PCTLD, loword((uint32_t)(sext16to32(c) * words(0x0, rgCosts[k]))), hiword((uint32_t)(sext16to32(c) * words(0x0,
     * rgCosts[k])))) */
    RightTextOut(hdc, ((rc.right - dxkT) + 0xffc0), rc.top, szWork, cch, dxMaxMineralQuan);
    TextOut(hdc, ((rc.right - dxkT) + 0xffc0), rc.top, PszGetCompressedString(idsKt), 0x2);
    k = (k + 1);

L_416f:
    if ((k < 3))
        goto L_406c;
    else
        goto L_4178;

L_4178:
    rc.top = (rc.top + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, rgcrMinerals[5]);
    TextOut(hdc, rc.left, rc.top, rgszMinerals[5], lstrlen(rgszMinerals[5]));
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crWindowText);
    /* untranslated: cch = _wsprintf(szWork, PCTLD, loword((uint32_t)(sext16to32(c) * words(0x0, rgCosts[0x3]))), hiword((uint32_t)(sext16to32(c) * words(0x0,
     * rgCosts[0x3])))) */
    RightTextOut(hdc, ((rc.right - dxkT) + 0xffc0), rc.top, szWork, cch, dxMaxMineralQuan);
    if ((fStarbaseMode != 0))
        goto Restore;
    else
        goto L_4239;

L_4239:
    rc.left = (rc.left - 8);
    rc.top = (rc.top + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    cch = _wsprintf(szWork, PszGetCompressedString(idsMassLdkt), LOWORD((uint32_t)(((uint32_t)(c) * (uint32_t)(part.pcom->cMass)))),
                    HIWORD((uint32_t)(((uint32_t)(c) * (uint32_t)(part.pcom->cMass)))));
    TextOut(hdc, rc.left, rc.top, szWork, cch);

Restore:
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crForeSav);
    SetBkColor(hdc, crBackSav);
    if ((fCreatedDC == 0))
        goto L_42e0;
    else
        goto L_42d5;

L_42d5:
    ReleaseDC(hwnd, hdc);

L_42e0:
    return;
}

int16_t PctJammerFromHul(HUL *lphul) {
    int32_t pctJam;
    int16_t ihs;
    int16_t i;
    int32_t pctHit;
    PART    part;

L_42e6:
    pctHit = 10000;
    ihs = 0;
    goto L_4305;

L_4301:
    ihs = (ihs + 1);

L_4305:
    if ((ihs >= lphul->chs))
        goto L_4487;
    else
        goto L_4317;

L_4317:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    HIWORD(part.hs) = HIWORD(lphul->rghs[ihs]);
    if ((part.hs.grhst != hstSpecialE))
        goto L_43a8;
    else
        goto L_4344;

L_4344:
    if ((part.hs.iItem < ispecialEJammer10))
        goto L_4380;
    else
        goto L_4352;

L_4352:
    if ((part.hs.iItem > ispecialEJammer50))
        goto L_4380;
    else
        goto L_4360;

L_4360:
    FLookupPart(&(part));
    pctJam = (uint32_t)((0x64 - part.pspecial->grAbility));
    goto L_441f;

L_4380:
    if ((part.hs.iItem != ispecialEMultiFunctionPod))
        goto L_439b;
    else
        goto L_438e;

L_438e:
    pctJam = 90;
    goto L_441f;

L_439b:
    pctJam = 100;

L_43a8:
    if ((part.hs.grhst != hstArmor))
        goto L_43cc;
    else
        goto L_43b1;

L_43b1:
    if ((part.hs.iItem != iarmorMegaPolyShell))
        goto L_43cc;
    else
        goto L_43bf;

L_43bf:
    pctJam = 80;
    goto L_441f;

L_43cc:
    if ((part.hs.grhst != hstMining))
        goto L_43f1;
    else
        goto L_43d6;

L_43d6:
    if ((part.hs.iItem != iminingAlienMiner))
        goto L_43f1;
    else
        goto L_43e4;

L_43e4:
    pctJam = 70;
    goto L_441f;

L_43f1:
    if ((part.hs.grhst != hstShield))
        goto L_4415;
    else
        goto L_43fa;

L_43fa:
    if ((part.hs.iItem != ishieldLangstonShell))
        goto L_4415;
    else
        goto L_4408;

L_4408:
    pctJam = 95;
    goto L_441f;

L_4415:
    pctJam = 100;

L_441f:
    if ((HIWORD(pctJam) > 0x0))
        goto L_4301;
    else
        goto L_4428;

L_4428:
    if ((HIWORD(pctJam) < 0x0))
        goto L_4436;
    else
        goto L_442d;

L_442d:
    if ((LOWORD(pctJam) >= 0x64))
        goto L_4301;
    else
        goto L_4436;

L_4436:
    i = part.hs.cItem;
    goto L_447b;

L_4447:
    pctHit = (uint32_t)((pctHit * pctJam));
    pctHit = (int32_t)((pctHit / 100));
    i = (i - 1);

L_447b:
    if ((i <= 0))
        goto L_4301;
    else
        goto L_4481;

L_4481:

L_4487:
    if ((HIWORD(pctHit) > 0x0))
        goto L_44a8;
    else
        goto L_4490;

L_4490:
    if ((HIWORD(pctHit) < 0x0))
        goto L_449e;
    else
        goto L_4495;

L_4495:
    if ((LOWORD(pctHit) >= 0x64))
        goto L_44a8;
    else
        goto L_449e;

L_449e:
    pctHit = 100;

L_44a8:
    pctJam = (uint32_t)((0x64 - ((uint32_t)((LOWORD(pctHit) + 0x32)) / 0x64)));
    if ((lphul->ihuldef <= ihuldefOrbitalFort))
        goto L_44ed;
    else
        goto L_44d4;

L_44d4:
    pctJam = (pctJam - (int32_t)((pctJam / 4)));

L_44ed:
    if ((HIWORD(pctJam) < 0x0))
        goto L_450e;
    else
        goto L_44f6;

L_44f6:
    if ((HIWORD(pctJam) > 0x0))
        goto L_4504;
    else
        goto L_44fb;

L_44fb:
    if ((LOWORD(pctJam) <= 0x5f))
        goto L_450e;
    else
        goto L_4504;

L_4504:
    pctJam = 95;

L_450e:

L_4517:
    return LOWORD(pctJam);
}

void DrawBuildSelHull(HWND hwnd, HDC hdc, int16_t iDraw, RECT *prc) {
    char     rgch[20];
    DV       dv;
    uint16_t rgCosts[4];
    int16_t  fCreatedDC;
    COLORREF crForeSav;
    int16_t  dxMineral;
    int16_t  k;
    COLORREF crBackSav;
    int16_t  csh;
    HUL     *lphul;
    int32_t  dpShield;
    int16_t  cch;
    int32_t  dp;
    int16_t  dxkT;
    RECT     rc;
    int32_t  lwt;
    int16_t  i;
    int16_t  j;
    int16_t  dPlanRange;
    int16_t  dRange;
    int16_t  pctDetect;
    int16_t  pct;
    int16_t  t_merge_458e_0001;
    char     t_merge_478e_0001[20];
    uint16_t t_merge_4957_0001;
    char    *t_merge_4c09_0001;
    uint16_t t_merge_4ee0_0001;
    uint16_t t_merge_4ff3_0001;
    uint16_t t_merge_50b2_0001;

L_451e:
    fCreatedDC = 0;
    if ((mdBuild == mdBuildComp))
        goto L_53d3;
    else
        goto L_4533;

L_4533:

L_4539:
    if ((hdc != 0x0))
        goto L_4552;
    else
        goto L_4542;

L_4542:
    fCreatedDC = 1;
    hdc = GetDC(hwnd);

L_4552:
    if ((prc != 0x0))
        goto L_45ab;
    else
        goto L_455b;

L_455b:
    GetClientRect(hwnd, &(rc));
    rc.bottom = (rc.bottom - 32);
    rc.right = (rc.right - 4);
    rc.left = (rc.right - 320);
    if ((fStarbaseMode == 0))
        goto L_458b;
    else
        goto L_4585;

L_4585:
    t_merge_458e_0001 = dyArial8;
    goto L_458e;

L_458b:
    t_merge_458e_0001 = 0;

L_458e:
    rc.top = (t_merge_458e_0001 + yBuildInfoSum);
    FillRect(hdc, &(rc), hbrButtonFace);
    goto L_45ba;

L_45ab:
    rc = *(prc);

L_45ba:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_45ce;
    else
        goto L_45c4;

L_45c4:
    if ((HIWORD(lpshdefBuild) == 0x0))
        goto LReleaseDC;
    else
        goto L_45ce;

L_45ce:
    lphul = lpshdefBuild;
    goto L_45e1;

L_45e1:
    SelectObject(hdc, rghfontArial8[0]);
    dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 0x2));
    SelectObject(hdc, rghfontArial8[1]);
    dxMineral = (LOWORD(GetTextExtent(hdc, rgszMinerals[0x2], strlen(rgszMinerals[0x2]))) + 6);
    crForeSav = SetTextColor(hdc, 0x0);
    crBackSav = SetBkColor(hdc, crButtonFace);
    if ((hwndPopup == 0x0))
        goto L_470d;
    else
        goto L_4670;

L_4670:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_470d;
    else
        goto L_467a;

L_467a:
    if ((GlobalPD.fToken == 0))
        goto L_470d;
    else
        goto L_4684;

L_4684:
    GetVCRStats(viVCRFocus, &(dp), dv, &(dpShield), &(csh));
    LOWORD(dpShield) = (vrgtok[viVCRFocus].dpShield - LOWORD(dpShield));
    HIWORD(dpShield) = (0x0 - HIWORD(dpShield));
    if ((HIWORD(dpShield) > 0x0))
        goto L_46e7;
    else
        goto L_46cf;

L_46cf:
    if ((HIWORD(dpShield) < 0x0))
        goto L_46dd;
    else
        goto L_46d4;

L_46d4:
    if ((LOWORD(dpShield) >= 0x0))
        goto L_46e7;
    else
        goto L_46dd;

L_46dd:
    dpShield = 0;

L_46e7:
    dpShield = (uint32_t)((dpShield * (uint32_t)(csh)));
    if ((csh == 0))
        goto LDeadToken;
    else
        goto L_4704;

L_4704:

L_470d:
    LOWORD(dp) = lphul->dp;
    HIWORD(dp) = 0x0;
    dpShield = DpShieldOfShdef(lpshdefBuild, idPlayer);

L_4737:
    if ((hwndPopup == 0x0))
        goto L_4767;
    else
        goto L_4741;

L_4741:
    if ((fStarbaseMode == 0))
        goto L_4767;
    else
        goto L_474b;

L_474b:
    rc.top = (rc.top + dyArial8);
    cch = CchGetString(idsCost, szWork);
    goto L_47bf;

L_4767:
    CchGetString(idsHull, rgch);
    if ((mdBuild != mdBuildHuldef))
        goto L_4789;
    else
        goto L_4781;

L_4781:
    t_merge_478e_0001 = rgch;
    goto L_478e;

L_4789:
    t_merge_478e_0001 = "";

L_478e:
    cch = _wsprintf(szWork, PszGetCompressedString(idsCostOneSS), (LOWORD(lphul) + 0x8), HIWORD(lphul), t_merge_478e_0001);

L_47bf:
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.left = (rc.left + 8);
    rc.right = (rc.right - 8);
    GetTrueHullCost(idPlayer, lphul, rgCosts);
    if ((fStarbaseMode == 0))
        goto L_4875;
    else
        goto L_47ff;

L_47ff:
    if ((GetRaceGrbit(rgplr[idPlayer], ibitRaceISB) != 0))
        goto L_4841;
    else
        goto L_4820;

L_4820:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_4875;
    else
        goto L_4841;

L_4841:
    k = 0;
    goto L_486c;

L_4849:
    /* untranslated: rgCosts[k] = (rgCosts[k] - (words(rgCosts[k], 0x0) / 0x5)) */
    k = (k + 1);

L_486c:
    if ((k < 4))
        goto L_4849;
    else
        goto L_4875;

L_4875:
    if ((fStarbaseMode == 0))
        goto L_48b3;
    else
        goto L_487f;

L_487f:
    k = 0;
    goto L_48aa;

L_4887:
    /* untranslated: rgCosts[k] = (rgCosts[k] - (words(rgCosts[k], 0x0) / 0x2)) */
    k = (k + 1);

L_48aa:
    if ((k < 4))
        goto L_4887;
    else
        goto L_48b3;

L_48b3:
    k = 0;
    goto L_49d2;

L_48bb:
    if ((k != 3))
        goto L_48c9;
    else
        goto L_48c4;

L_48c4:
    k = 5;

L_48c9:
    rc.top = (rc.top + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    SetTextColor(hdc, rgcrMinerals[k]);
    TextOut(hdc, rc.left, rc.top, rgszMinerals[k], lstrlen(rgszMinerals[k]));
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crWindowText);
    if ((k != 5))
        goto L_494b;
    else
        goto L_4945;

L_4945:
    t_merge_4957_0001 = rgCosts[3];
    goto L_4957;

L_494b:
    t_merge_4957_0001 = rgCosts[k];

L_4957:
    cch = _wsprintf(szWork, PCTD, t_merge_4957_0001);
    RightTextOut(hdc, (((rc.left + dxMineral) + dxMaxMineralQuan) - dxkT), rc.top, szWork, cch, dxMaxMineralQuan);
    if ((k >= 5))
        goto L_49ce;
    else
        goto L_49a1;

L_49a1:
    TextOut(hdc, (((rc.left + dxMineral) + dxMaxMineralQuan) - dxkT), rc.top, PszGetCompressedString(idsKt), 0x2);

L_49ce:
    k = (k + 1);

L_49d2:
    if ((k <= 5))
        goto L_48bb;
    else
        goto L_49db;

L_49db:
    rc.left = (rc.left - 8);
    rc.top = (rc.top + dyArial8);
    SelectObject(hdc, rghfontArial8[1]);
    if ((fStarbaseMode != 0))
        goto L_4a8c;
    else
        goto L_49fb;

L_49fb:
    if ((hwndPopup == 0x0))
        goto L_4a3c;
    else
        goto L_4a05;

L_4a05:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_4a3c;
    else
        goto L_4a0f;

L_4a0f:
    if ((GlobalPD.fToken == 0))
        goto L_4a3c;
    else
        goto L_4a19;

L_4a19:
    LOWORD(lwt) = vrgtok[viVCRFocus].wt;
    HIWORD(lwt) = 0x0;
    goto L_4a4c;

L_4a3c:
    LOWORD(lwt) = lphul->wtEmpty;
    HIWORD(lwt) = 0x0;

L_4a4c:
    cch = _wsprintf(szWork, PszGetCompressedString(idsMassLdkt), LOWORD(lwt), HIWORD(lwt));
    TextOut(hdc, rc.left, rc.top, szWork, cch);

L_4a8c:
    rc.top = (rc.top - (dyArial8 * 4));
    rc.left = (rc.left + ((dxMineral + dxMaxMineralQuan) + 24));
    if ((fStarbaseMode != 0))
        goto L_4b53;
    else
        goto L_4aad;

L_4aad:
    if ((hwndPopup == 0x0))
        goto L_4acb;
    else
        goto L_4ab7;

L_4ab7:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto L_4acb;
    else
        goto L_4ac1;

L_4ac1:
    if ((GlobalPD.fToken != 0))
        goto L_4b53;
    else
        goto L_4acb;

L_4acb:
    cch = _wsprintf(szWork, PszGetCompressedString(idsDmg), WtMaxShdefStat(lpshdefBuild, 0x1));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, dxMaxMineralQuan);
    cch = CchGetString(idsMaxFuel, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);

L_4b53:
    cch = _wsprintf(szWork, PszGetCompressedString(idsLddp), LOWORD(dp), HIWORD(dp));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, (dxMaxMineralQuan + 10));
    cch = CchGetString(idsArmor, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);
    if ((mdBuild == mdBuildHuldef))
        goto L_4c6b;
    else
        goto L_4bdb;

L_4bdb:
    if ((LOWORD(dpShield) != 0x0))
        goto L_4c04;
    else
        goto L_4bea;

L_4bea:
    if ((HIWORD(dpShield) != 0x0))
        goto L_4c04;
    else
        goto L_4bf3;

L_4bf3:
    t_merge_4c09_0001 = PszGetCompressedString(idsNone);
    goto L_4c09;

L_4c04:
    t_merge_4c09_0001 = 0xcbf;

L_4c09:
    cch = _wsprintf(szWork, t_merge_4c09_0001, LOWORD(dpShield), HIWORD(dpShield));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, (dxMaxMineralQuan + 10));
    cch = CchGetString(idsShields, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);

L_4c6b:
    rc.top = (rc.top + dyArial8);
    if ((mdBuild == mdBuildHuldef))
        goto L_4d2b;
    else
        goto L_4c7b;

L_4c7b:
    LComputePower(lpshdefBuild);
    /* untranslated: *(lpshdefBuild+0x87) = loword(callresult(int32_t)) */
    /* untranslated: *(lpshdefBuild+0x89) = hiword(callresult(int32_t)) */
    if ((*(lpshdefBuild + 0x87) != 0x0))
        goto L_4cb3;
    else
        goto L_4ca8;

L_4ca8:
    if ((*(lpshdefBuild + 0x89) == 0x0))
        goto L_4d2b;
    else
        goto L_4cb3;

L_4cb3:
    cch = _wsprintf(szWork, PCTLD, *(lpshdefBuild + 0x87), *(lpshdefBuild + 0x89));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, dxMaxMineralQuan);
    cch = CchGetString(idsRating, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);

L_4d2b:
    if ((gd.mdScreenSize <= 0x0))
        goto L_50df;
    else
        goto L_4d3e;

L_4d3e:
    if ((mdBuild == mdBuildHuldef))
        goto L_50df;
    else
        goto L_4d48;

L_4d48:
    if ((mdBuild == mdBuildEnemyShdef))
        goto L_4d5b;
    else
        goto L_4d52;

L_4d52:
    i = idPlayer;
    goto L_4d60;

L_4d5b:
    i = -1;

L_4d60:
    i = PctCloakFromHuldef(&(lpshdefBuild), i, 0x0);
    j = PctJammerFromHul(&(lpshdefBuild));
    cch = _wsprintf(szWork, PszGetCompressedString(idsDD4), i, j);
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, dxMaxMineralQuan);
    cch = CchGetString(idsCloakJam, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);
    i = InitFromHuldef(&(lpshdefBuild), 0x0);
    if ((fStarbaseMode != 0))
        goto L_4e3f;
    else
        goto L_4e27;

L_4e27:
    if ((lpshdefBuild->hul.rghs[0].cItem != 0x0))
        goto L_4e47;
    else
        goto L_4e3f;

L_4e3f:
    j = 0;
    goto L_4e75;

L_4e47:
    j = (SpdOfShip(0x0, 0x0, 0x0, 0x0, lpshdefBuild) + 1);

L_4e75:
    cch = _wsprintf(szWork, PszGetCompressedString(idsDS), i, &(rgszSpeed[(0x3 * j)]));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, dxMaxMineralQuan);
    if ((dyArial8 <= 14))
        goto L_4edd;
    else
        goto L_4ed7;

L_4ed7:
    t_merge_4ee0_0001 = 0x1;
    goto L_4ee0;

L_4edd:
    t_merge_4ee0_0001 = 0x0;

L_4ee0:
    cch = CchGetString((t_merge_4ee0_0001 + 0x4ac), szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);
    if ((fStarbaseMode != 0))
        goto L_5023;
    else
        goto L_4f17;

L_4f17:
    if ((mdBuild == mdBuildEnemyShdef))
        goto L_4f2a;
    else
        goto L_4f21;

L_4f21:
    i = idPlayer;
    goto L_4f2f;

L_4f2a:
    i = -1;

L_4f2f:
    dRange = GetShdefScannerRange(lpshdefBuild, i, &(dPlanRange), &(pctDetect), 0x0);
    if ((dRange <= 0))
        goto L_50df;
    else
        goto L_4f5a;

L_4f5a:
    if ((pctDetect < 100))
        goto L_4f8e;
    else
        goto L_4f63;

L_4f63:
    cch = _wsprintf(szWork, PszGetCompressedString(idsDD6), dRange, dPlanRange);
    goto L_4fb9;

L_4f8e:
    cch = _wsprintf(szWork, PszGetCompressedString(idsDDD), dRange, dPlanRange, pctDetect);

L_4fb9:
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, (dxMaxMineralQuan + 40));
    if ((dyArial8 <= 14))
        goto L_4ff0;
    else
        goto L_4fea;

L_4fea:
    t_merge_4ff3_0001 = 0x1;
    goto L_4ff3;

L_4ff0:
    t_merge_4ff3_0001 = 0x0;

L_4ff3:
    cch = CchGetString((t_merge_4ff3_0001 + 0x4af), szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);

L_5023:
    if ((GetRaceStat(rgplr[idPlayer], rsMajorAdv) != raMacintosh))
        goto L_50df;
    else
        goto L_5044;

L_5044:
    cch = CommaFormatLong(szWork, (uint32_t)((rglPopMac[(lpshdefBuild->hul.ihuldef - 32)] * 100)));
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, (dxMaxMineralQuan + 16));
    if ((dyArial8 <= 14))
        goto L_50af;
    else
        goto L_50a9;

L_50a9:
    t_merge_50b2_0001 = 0x1;
    goto L_50b2;

L_50af:
    t_merge_50b2_0001 = 0x0;

L_50b2:
    cch = CchGetString((t_merge_50b2_0001 + 0x4f7), szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);
    rc.top = (rc.top + dyArial8);

L_50df:
    if ((hwndPopup == 0x0))
        goto LDeadToken;
    else
        goto L_50e9;

L_50e9:
    if ((GlobalPD.grPopup != grPopupShdef))
        goto LDeadToken;
    else
        goto L_50f3;

L_50f3:
    if ((GlobalPD.fShowDamage == 0))
        goto LDeadToken;
    else
        goto L_50fd;

L_50fd:
    if ((GlobalPD.fToken != 0))
        goto L_525a;
    else
        goto L_5107;

L_5107:
    if ((fStarbaseMode != 0))
        goto L_51c1;
    else
        goto L_5111;

L_5111:
    if ((GlobalPD.fSummary == 0))
        goto L_518d;
    else
        goto L_511b;

L_511b:
    k = lpshdefBuild->ishdef;
    csh = rglpfl[sel.scan.ifl]->rgcsh[k];
    /* untranslated: dv.dp = *(rglpfl[sel.scan.ifl]+0x2):[((LOWORD(rglpfl[sel.scan.ifl]) + 0x2c) + (k * 0x2))] */
    goto L_525a;

L_518d:
    k = lpshdefBuild->ishdef;
    csh = sel.fl.rgcsh[k];
    dv.dp = mpicolgrbitBU[(k + 9423)];

L_51c1:
    dv.dp = 0x0;
    if ((GlobalPD.fSummary == 0))
        goto L_520d;
    else
        goto L_51d0;

L_51d0:
    dv.dp = (dv.pctSh | ((((*(LpplFromId(sel.scan.idpl) + 0x2c) >> 0x4) & 0xfff) & 0x1ff) << 0x7));
    goto L_5236;

L_520d:
    dv.dp = (dv.pctSh | ((sel.pl.pctDp & 0x1ff) << 0x7));

L_5236:
    csh = 1;
    if ((dv.pctDp == 0x0))
        goto L_525a;
    else
        goto L_524e;

L_524e:
    dv.pctSh = 0x64;

L_525a:
    if ((dv.dp == 0x0))
        goto L_532d;
    else
        goto L_5263;

L_5263:
    SetTextColor(hdc, 0x7f);
    /* untranslated: pct = (words(dv.pctDp, 0x0) / 5) */
    if ((pct > 0))
        goto L_5296;
    else
        goto L_5291;

L_5291:
    pct = 1;

L_5296:
    /* untranslated: csh = loword((int32_t)((uint32_t)(sext16to32(csh) * words(0x0, dv.pctSh)) / 0x64)) */
    if ((csh > 0))
        goto L_52cc;
    else
        goto L_52c7;

L_52c7:
    csh = 1;

L_52cc:
    if ((fStarbaseMode == 0))
        goto L_52f5;
    else
        goto L_52d6;

L_52d6:
    cch = _wsprintf(szWork, PCTDPCTPCT, pct);
    goto L_5320;

L_52f5:
    cch = _wsprintf(szWork, PszGetCompressedString(idsLdD), csh, SIGNHIWORD(csh), pct);

L_5320:
    dp = 1;
    goto L_5337;

L_532d:
    dp = 0;

L_5337:
    if ((LOWORD(dp) != 0x0))
        goto L_5349;
    else
        goto L_5340;

L_5340:
    if ((HIWORD(dp) == 0x0))
        goto LDeadToken;
    else
        goto L_5349;

L_5349:
    RightTextOut(hdc, (rc.right - 8), rc.top, szWork, cch, (dxMaxMineralQuan + 15));
    cch = CchGetString(idsDamage, szWork);
    TextOut(hdc, rc.left, rc.top, szWork, cch);

LDeadToken:
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crForeSav);
    SetBkColor(hdc, crBackSav);

LReleaseDC:
    if ((fCreatedDC == 0))
        goto L_53d3;
    else
        goto L_53c8;

L_53c8:
    ReleaseDC(hwnd, hdc);

L_53d3:
    return;
}

void SetBuildSelection(int16_t iSrc) {
    int16_t iSelOld;
    RECT    rc;

L_53da:
    if ((iSrc == iselSlot))
        goto L_5463;
    else
        goto L_53ee;

L_53ee:
    iSelOld = iselSlot;
    iselSlot = iSrc;
    GetClientRect(hwndSlotDlg, &(rc));
    if ((iSelOld < 0))
        goto L_542a;
    else
        goto L_5413;

L_5413:
    DrawSlotDlg(hwndSlotDlg, 0x0, &(rc), iSelOld);

L_542a:
    if ((iselSlot < 0))
        goto RedrawSel;
    else
        goto L_5434;

L_5434:
    DrawSlotDlg(hwndSlotDlg, 0x0, &(rc), iselSlot);

RedrawSel:
    DrawBuildSelComp(hwndSlotDlg, 0x0, 0xffff);
    goto L_546f;

L_5463:
    if ((iSrc == -1))
        goto RedrawSel;
    else
        goto L_5469;

L_5469:

L_546f:
    return;
}

int16_t IDropPart(POINT pt, HS hsSrc, int16_t iSrc, int16_t fNoModify) {
    int16_t  cSlot;
    int16_t  cNew;
    int16_t  i;
    HS       hsHul;
    HS       hsDst;
    RECT     rc;
    uint16_t t_merge_5646_0001;
    uint16_t t_merge_58c1_0001;

L_5476:
    GetClientRect(hwndSlotDlg, &(rc));
    if (((GetAsyncKeyState(0x11) & 0xfffe) == 0x0))
        goto L_54bb;
    else
        goto L_54a3;

L_54a3:
    if ((iSrc >= 0))
        goto L_550f;
    else
        goto L_54ac;

L_54ac:
    hsSrc.cItem = 0x64;

L_54bb:
    if (((GetAsyncKeyState(0x10) & 0xfffe) == 0x0))
        goto L_54fa;
    else
        goto L_54cf;

L_54cf:
    if ((iSrc < 0))
        goto L_54eb;
    else
        goto L_54d8;

L_54d8:
    if ((hsSrc.cItem <= 0x4))
        goto L_550f;
    else
        goto L_54eb;

L_54eb:
    hsSrc.cItem = 0x4;

L_54fa:
    if ((iSrc < 0))
        goto L_550f;
    else
        goto L_5503;

L_5503:
    hsSrc.cItem = 0x1;

L_550f:
    cSlot = LphuldefFromId(lpshdefBuild->hul.ihuldef)->hul.chs;
    i = 0;
    goto L_5560;

L_5534:
    if ((PtInRect(&(vrgrcSlot[i]), pt) != 0))
        goto L_556b;
    else
        goto L_5556;

L_5556:

L_555c:
    i = (i + 1);

L_5560:
    if ((i < cSlot))
        goto L_5534;
    else
        goto L_556b;

L_556b:
    if ((i != cSlot))
        goto L_5735;
    else
        goto L_5576;

L_5576:
    if ((pt.x >= (rc.right >> 0x1)))
        goto L_572f;
    else
        goto L_5583;

L_5583:
    if ((fNoModify != 0))
        goto L_5729;
    else
        goto L_558c;

L_558c:
    if ((iSrc < 0))
        goto L_5729;
    else
        goto L_5595;

L_5595:
    if (((lpshdefBuild->hul.rghs[iSrc].grhst & 0x1) == 0x0))
        goto L_55c7;
    else
        goto L_55bb;

L_55bb:
    hsSrc.cItem = 0x64;

L_55c7:
    if ((0x0 <= (lpshdefBuild->hul.rghs[iSrc].cItem - hsSrc.cItem)))
        goto L_5609;
    else
        goto L_5603;

L_5603:
    t_merge_5646_0001 = 0x0;
    goto L_5646;

L_5609:
    /* untranslated: ss:[bp-0x1c] = HIWORD(lpshdefBuild->hul.rghs[iSrc]) */
    /* untranslated: ss:[bp-0x1e] = hsSrc.cItem */
    /* untranslated: t_merge_5646_0001 = (((ss:[bp-0x1c] >> 0x8) & 0xff) - ss:[bp-0x1e]) */

L_5646:
    HIWORD(lpshdefBuild->hul.rghs[iSrc]) = (lpshdefBuild->hul.rghs[iSrc].iItem | ((t_merge_5646_0001 & 0xff) << 0x8));
    UpdateShdefCost(lpshdefBuild);
    GetClientRect(hwndSlotDlg, &(rc));
    DrawSlotDlg(hwndSlotDlg, 0x0, &(rc), iSrc);
    rc.top = yBuildInfoSum;
    InvalidateRect(hwndSlotDlg, &(rc), 0x1);
    DrawBuildSelComp(hwndSlotDlg, 0x0, 0xffff);
    DrawBuildSelHull(hwndSlotDlg, 0x0, 0xffff, 0x0);
    if ((gd.fTutorial == 0x0))
        goto L_5729;
    else
        goto L_5724;

L_5724:
    AdvanceTutor();

L_5729:
    return 0x1;

L_572f:
    return 0x0;

L_5735:
    hsDst.grhst = lpshdefBuild->hul.rghs[i].grhst;
    HIWORD(hsDst) = HIWORD(lpshdefBuild->hul.rghs[i]);
    LphuldefFromId(lpshdefBuild->hul.ihuldef);
    /* untranslated: hsHul.grhst = callresult(HULDEF *)->hul.rghs[i].grhst */
    /* untranslated: HIWORD(hsHul) = HIWORD(callresult(HULDEF *)->hul.rghs[i]) */
    if (((hsHul.grhst & 0x1) == 0x0))
        goto L_57a4;
    else
        goto L_5798;

L_5798:
    hsSrc.cItem = 0x64;

L_57a4:
    if ((i != iSrc))
        goto L_57b5;
    else
        goto L_57af;

L_57af:
    return 0x2;

L_57b5:
    if ((hsDst.cItem >= hsHul.cItem))
        goto L_5826;
    else
        goto L_57d3;

L_57d3:
    if ((hsDst.cItem <= 0x0))
        goto L_5805;
    else
        goto L_57e6;

L_57e6:
    if ((hsDst.grhst != hsSrc.grhst))
        goto L_5826;
    else
        goto L_57f1;

L_57f1:
    if ((hsDst.iItem != hsSrc.iItem))
        goto L_5826;
    else
        goto L_5805;

L_5805:
    if ((hsDst.cItem != 0x0))
        goto L_583e;
    else
        goto L_5818;

L_5818:
    if (((hsSrc.grhst & hsHul.grhst) != 0x0))
        goto L_583e;
    else
        goto L_5826;

L_5826:
    if ((fNoModify != 0))
        goto L_5838;
    else
        goto L_582f;

L_582f:
    MessageBeep(0x0);

L_5838:
    return 0x3;

L_583e:
    if ((fNoModify != 0))
        goto L_59e4;
    else
        goto L_5847;

L_5847:
    hsDst.grhst = hsSrc.grhst;
    hsDst.iItem = hsSrc.iItem;
    if (((hsDst.cItem + hsSrc.cItem) >= hsHul.cItem))
        goto L_58b6;
    else
        goto L_5895;

L_5895:
    /* untranslated: t_merge_58c1_0001 = (hsDst.cItem + ss:[bp-0x1a]) */
    goto L_58c1;

L_58b6:
    t_merge_58c1_0001 = hsHul.cItem;

L_58c1:
    cNew = t_merge_58c1_0001;
    if ((iSrc < 0))
        goto L_594b;
    else
        goto L_58cd;

L_58cd:
    /* untranslated: ss:[bp-0x1a] = ((HIWORD(lpshdefBuild->hul.rghs[iSrc]) - ((cNew - hsDst.cItem) << 0x8)) & 0xff00) */
    HIWORD(lpshdefBuild->hul.rghs[iSrc]) = lpshdefBuild->hul.rghs[iSrc].iItem;
    /* untranslated: HIWORD(lpshdefBuild->hul.rghs[iSrc]) = (HIWORD(lpshdefBuild->hul.rghs[iSrc]) | ss:[bp-0x1a]) */

L_594b:
    HIWORD(hsDst) = (hsDst.iItem | ((cNew & 0xff) << 0x8));
    lpshdefBuild->hul.rghs[i].grhst = hsDst.grhst;
    HIWORD(lpshdefBuild->hul.rghs[i]) = HIWORD(hsDst);
    UpdateShdefCost(lpshdefBuild);
    SetBuildSelection(i);
    GetClientRect(hwndSlotDlg, &(rc));
    rc.top = yBuildInfoSum;
    InvalidateRect(hwndSlotDlg, &(rc), 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_59e4;
    else
        goto L_59df;

L_59df:
    AdvanceTutor();

L_59e4:
    return 0xffff;
}

void DrawDlgLBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    COLORREF cr;
    COLORREF crForeSav;
    int16_t  ibmp;
    int16_t  bkSav;
    RECT     rc;
    uint16_t t_merge_5a41_0001;
    COLORREF t_merge_5b5f_0001_wide;

L_59f0:
    CopyRect(&(rc), &(lpdis->rcItem));
    if (((lpdis->itemState & 0x10) == 0x0))
        goto L_5a3e;
    else
        goto L_5a38;

L_5a38:
    t_merge_5a41_0001 = 0x4;
    goto L_5a41;

L_5a3e:
    t_merge_5a41_0001 = 0x0;

L_5a41:
    FillRect(lpdis->hDC, &(lpdis->rcItem), GetStockObject(t_merge_5a41_0001));
    InflateRect(&(rc), 0xfffe, 0xffff);
    /* untranslated: call SendMessage(lpdis->hwndItem, CB_INSERTSTRING, lpdis->itemID, words(ds, 0x57a4)) -> callresult(LRESULT) */
    SelectPalette(lpdis->hDC, vhpal, 0x0);
    RealizePalette(lpdis->hDC);
    ibmp = (((uint16_t)(szWork[0x2]) - 65) + LOWORD((((uint16_t)(szWork[0x3]) + 0xffbf) * 0x1a)));
    DibBlt(lpdis->hDC, rc.left, rc.top, 0x40, 0x40, rghdibInventory[(ibmp >> 0x5)], ((ibmp & 0x7) * 0x40), (((0x3 - (ibmp >> 0x3)) & 0x3) * 0x40), 0x40, 0x40,
           0xcc0020);
    if (((lpdis->itemState & 0x10) == 0x0))
        goto L_5b3c;
    else
        goto L_5b32;

L_5b32:
    t_merge_5b5f_0001_wide = crWindow;
    goto L_5b5f;

L_5b3c:
    if ((LOWORD(crWindow) != 0x0))
        goto L_5b59;
    else
        goto L_5b46;

L_5b46:
    if ((HIWORD(crWindow) != 0x0))
        goto L_5b59;
    else
        goto L_5b50;

L_5b50:
    t_merge_5b5f_0001_wide = 0xffffff;
    goto L_5b5f;

L_5b59:
    t_merge_5b5f_0001_wide = 0x0;

L_5b5f:
    cr = t_merge_5b5f_0001_wide;
    crForeSav = SetTextColor(lpdis->hDC, cr);
    bkSav = SetBkMode(lpdis->hDC, TRANSPARENT);
    TextOut(lpdis->hDC, (rc.left + 66), ((rc.top + 32) - (dyArial8 >> 0x1)), "", strlen(&(szWork[0x4])));
    SetTextColor(lpdis->hDC, crForeSav);
    SetBkMode(lpdis->hDC, bkSav);
    HandleFocusState(lpdis, (inflate + 2));
    return;
}

SHDEF *NthValidShdef(int16_t n) {
    int16_t  i;
    int16_t  t_5c4f;
    uint16_t t_merge_5cee_0001;
    uint16_t t_merge_5cee_0002;
    int16_t  t_5cb7;

L_5c06:
    if ((fStarbaseMode == 0))
        goto L_5c91;
    else
        goto L_5c19;

L_5c19:
    i = 0;
    goto L_5c85;

L_5c21:
    if ((((rglpshdefSB[idPlayer][i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5c81;
    else
        goto L_5c4f;

L_5c4f:
    t_5c4f = n;
    n = (n - 1);
    if ((t_5c4f != 0))
        goto L_5c81;
    else
        goto L_5c5e;

L_5c5e:
    /* untranslated: t_merge_5cee_0001 = (LOWORD(rglpshdefSB[idPlayer]) + ss:[bp-0x6]) */
    t_merge_5cee_0002 = HIWORD(rglpshdefSB[idPlayer]);
    goto L_5cee;

L_5c81:
    i = (i + 1);

L_5c85:
    if ((i >= 10))
        goto L_5ce5;
    else
        goto L_5c8b;

L_5c8b:

L_5c91:
    i = 0;
    goto L_5cdc;

L_5c99:
    if ((((rgshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5cd8;
    else
        goto L_5cb7;

L_5cb7:
    t_5cb7 = n;
    n = (n - 1);
    if ((t_5cb7 != 0))
        goto L_5cd8;
    else
        goto L_5cc6;

L_5cc6:
    t_merge_5cee_0001 = rgshdef[i];
    /* untranslated: t_merge_5cee_0002 = ds */
    goto L_5cee;

L_5cd8:
    i = (i + 1);

L_5cdc:
    if ((i < 16))
        goto L_5c99;
    else
        goto L_5ce5;

L_5ce5:
    t_merge_5cee_0001 = 0x0;
    t_merge_5cee_0002 = 0x0;

L_5cee:
    return farptr(t_merge_5cee_0002, t_merge_5cee_0001);
}

SHDEF *NthValidEnemyShdef(int16_t n) {
    int16_t i;
    int16_t j;
    int16_t t_5d79;
    int16_t t_5e2f;

L_5cf4:
    if ((fStarbaseMode == 0))
        goto L_5dbd;
    else
        goto L_5d07;

L_5d07:
    i = 0;
    goto L_5d13;

L_5d0f:
    i = (i + 1);

L_5d13:
    if ((i >= game.cPlayer))
        goto L_5e70;
    else
        goto L_5d1e;

L_5d1e:
    if ((LOWORD(rglpshdefSB[i]) != 0x0))
        goto L_5d39;
    else
        goto L_5d2f;

L_5d2f:
    if ((HIWORD(rglpshdefSB[i]) == 0x0))
        goto L_5d0f;
    else
        goto L_5d39;

L_5d39:
    if ((i == idPlayer))
        goto L_5d0f;
    else
        goto L_5d44;

L_5d44:
    j = 0;
    goto L_5dae;

L_5d4c:
    if ((((rglpshdefSB[i][j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5daa;
    else
        goto L_5d79;

L_5d79:
    t_5d79 = n;
    n = (n - 1);
    if ((t_5d79 != 0))
        goto L_5daa;
    else
        goto L_5d88;

L_5d88:
    /* untranslated: return ptroff(rglpshdefSB[i], ss:[bp-0x8]) */

L_5daa:
    j = (j + 1);

L_5dae:
    if ((j >= 10))
        goto L_5d0f;
    else
        goto L_5db4;

L_5db4:

L_5dbd:
    i = 0;
    goto L_5dc9;

L_5dc5:
    i = (i + 1);

L_5dc9:
    if ((i >= game.cPlayer))
        goto L_5e70;
    else
        goto L_5dd4;

L_5dd4:
    if ((LOWORD(rglpshdef[i]) != 0x0))
        goto L_5def;
    else
        goto L_5de5;

L_5de5:
    if ((HIWORD(rglpshdef[i]) == 0x0))
        goto L_5dc5;
    else
        goto L_5def;

L_5def:
    if ((i == idPlayer))
        goto L_5dc5;
    else
        goto L_5dfa;

L_5dfa:
    j = 0;
    goto L_5e64;

L_5e02:
    if ((((rglpshdef[i][j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5e60;
    else
        goto L_5e2f;

L_5e2f:
    t_5e2f = n;
    n = (n - 1);
    if ((t_5e2f != 0))
        goto L_5e60;
    else
        goto L_5e3e;

L_5e3e:
    /* untranslated: return ptroff(rglpshdef[i], ss:[bp-0x8]) */

L_5e60:
    j = (j + 1);

L_5e64:
    if ((j >= 16))
        goto L_5dc5;
    else
        goto L_5e6a;

L_5e6a:

L_5e70:
    return 0x0;
}

void FillBuildDD(HWND hwndDD, MdBuild md) {
    int16_t  ishdefMac;
    int16_t  fProgress;
    int16_t  fAdded;
    int16_t  i;
    int16_t  j;
    SHDEF   *lpshdef;
    RECT     rc;
    PART     part;
    uint16_t t_merge_5f7d_0001;
    uint16_t t_merge_5ff1_0001;
    uint16_t t_merge_6043_0001;
    uint16_t t_merge_608f_0001;
    SHDEF   *t_merge_611c_0001;
    uint16_t t_merge_63ac_0001;

L_5e80:
    SendMessage(hwndDD, CB_RESETCONTENT, 0x0, 0x0);
    if ((md == mdBuildShdef))
        goto L_5ed8;
    else
        goto L_5eaa;

L_5eaa:
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0x0);
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0x0);

L_5ed8:
    if ((fStarbaseMode == 0))
        goto L_5f00;
    else
        goto L_5ee2;

L_5ee2:
    ishdefMac = 10;
    lpshdef = rglpshdefSB[idPlayer];
    goto L_5f10;

L_5f00:
    ishdefMac = 16;
    lpshdef = rgshdef;

L_5f10:
    i = 0;
    goto L_5f43;

L_5f18:
    if ((((lpshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_5f4e;
    else
        goto L_5f39;

L_5f39:

L_5f3f:
    i = (i + 1);

L_5f43:
    if ((i < ishdefMac))
        goto L_5f18;
    else
        goto L_5f4e;

L_5f4e:
    if ((md == mdBuildShdef))
        goto L_5f69;
    else
        goto L_5f57;

L_5f57:
    if ((md == mdBuildHuldef))
        goto L_5f69;
    else
        goto L_5f60;

L_5f60:
    if ((md != mdBuildEnemyShdef))
        goto L_5f7a;
    else
        goto L_5f69;

L_5f69:
    if ((i >= ishdefMac))
        goto L_5f7a;
    else
        goto L_5f74;

L_5f74:
    t_merge_5f7d_0001 = 0x1;
    goto L_5f7d;

L_5f7a:
    t_merge_5f7d_0001 = 0x0;

L_5f7d:
    fAdded = t_merge_5f7d_0001;
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), fAdded);
    goto L_62ff;

L_5f9c:
    fAdded = 0;
    i = 0;
    goto L_60be;

L_5fa9:
    if ((((lpshdef[i].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_60ba;
    else
        goto L_5fcd;

L_5fcd:
    if ((fAdded != 0))
        goto L_6095;
    else
        goto L_5fd6;

L_5fd6:
    if ((fStarbaseMode == 0))
        goto L_5fee;
    else
        goto L_5fe8;

L_5fe8:
    t_merge_5ff1_0001 = 0x10;
    goto L_5ff1;

L_5fee:
    t_merge_5ff1_0001 = 0x0;

L_5ff1:
    CshQueued((t_merge_5ff1_0001 + i), &(fProgress), 0x0);
    GetDlgItem(hwndSlotDlg, IDC_EDIT);
    if ((LOWORD(lpshdef[i].cExist) != 0x0))
        goto L_6040;
    else
        goto L_6026;

L_6026:
    if ((HIWORD(lpshdef[i].cExist) != 0x0))
        goto L_6040;
    else
        goto L_6031;

L_6031:
    if ((fProgress != 0))
        goto L_6040;
    else
        goto L_603a;

L_603a:
    t_merge_6043_0001 = 0x1;
    goto L_6043;

L_6040:
    t_merge_6043_0001 = 0x0;

L_6043:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_6043_0001) -> callresult(int16_t) */
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0x1);
    if ((LOWORD(lpshdef[i].cExist) != 0x0))
        goto L_6086;
    else
        goto L_607b;

L_607b:
    if ((HIWORD(lpshdef[i].cExist) == 0x0))
        goto L_608c;
    else
        goto L_6086;

L_6086:
    t_merge_608f_0001 = 0x1;
    goto L_608f;

L_608c:
    t_merge_608f_0001 = 0x0;

L_608f:
    fAdded = (t_merge_608f_0001 + 1);

L_6095:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(HIWORD(lpshdef), ((LOWORD(lpshdef) + loword((147 * i))) + 0x8))) -> callresult(LRESULT)
     */

L_60ba:
    i = (i + 1);

L_60be:
    if ((i >= ishdefMac))
        goto L_632a;
    else
        goto L_60c6;

L_60c6:

L_60cc:
    i = 0;
    goto L_60d8;

L_60d4:
    i = (i + 1);

L_60d8:
    if ((i >= game.cPlayer))
        goto L_632a;
    else
        goto L_60e3;

L_60e3:
    if ((i == idPlayer))
        goto L_60d4;
    else
        goto L_60eb;

L_60eb:

L_60f1:
    if ((fStarbaseMode == 0))
        goto L_610d;
    else
        goto L_60fb;

L_60fb:
    t_merge_611c_0001 = rglpshdefSB[i];
    goto L_611c;

L_610d:
    t_merge_611c_0001 = rglpshdef[i];

L_611c:
    lpshdef = t_merge_611c_0001;
    if ((LOWORD(lpshdef) != 0x0))
        goto L_6134;
    else
        goto L_612b;

L_612b:
    if ((HIWORD(lpshdef) == 0x0))
        goto L_60d4;
    else
        goto L_6134;

L_6134:
    j = 0;
    goto L_61e4;

L_613c:
    if ((((lpshdef[j].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_61e0;
    else
        goto L_6160;

L_6160:
    if ((PszPlayerName(i, 0x1, 0x0, 0x0, 0x0, 0x0) != szWork))
        goto L_618d;
    else
        goto L_6187;

L_6187:
    goto L_6190;

L_618d:

L_6190:
    _wsprintf(&(szWork[strlen(szWork)]), " %s", ((LOWORD(lpshdef) + LOWORD((147 * j))) + 0x8), HIWORD(lpshdef));
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, 0x57a4)) -> callresult(LRESULT) */

L_61e0:
    j = (j + 1);

L_61e4:
    if ((j >= ishdefMac))
        goto L_60d4;
    else
        goto L_61ec;

L_61ec:

L_61f5:
    if ((fStarbaseMode == 0))
        goto L_620c;
    else
        goto L_61ff;

L_61ff:
    part.hs.grhst = hstSBHull;
    j = 5;
    goto L_6216;

L_620c:
    part.hs.grhst = hstHull;
    j = 32;

L_6216:
    i = 0;
    goto L_6268;

L_621e:
    part.hs.iItem = i;
    if ((FLookupPart(&(part)) != 1))
        goto L_6264;
    else
        goto L_6241;

L_6241:

L_6247:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(HIWORD(part.pcom), (LOWORD(part.pcom) + 0x8))) -> callresult(LRESULT) */

L_6264:
    i = (i + 1);

L_6268:
    if ((i >= j))
        goto L_632a;
    else
        goto L_6270;

L_6270:

L_6276:
    if ((fStarbaseMode == 0))
        goto L_62be;
    else
        goto L_6280;

L_6280:
    i = 0;
    goto L_62b2;

L_6288:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(rgidsPartsSB[i]))) -> callresult(LRESULT) */
    i = (i + 1);

L_62b2:
    if ((i >= 8))
        goto L_632a;
    else
        goto L_62b8;

L_62b8:

L_62be:
    i = 0;
    goto L_62f0;

L_62c6:
    /* untranslated: call SendMessage(hwndDD, CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(rgidsParts[i]))) -> callresult(LRESULT) */
    i = (i + 1);

L_62f0:
    if ((i >= 13))
        goto L_632a;
    else
        goto L_62f6;

L_62f6:

L_62ff:
    if ((md == mdBuildShdef))
        goto L_5f9c;
    else
        goto L_6307;

L_6307:
    if ((md == mdBuildHuldef))
        goto L_61f5;
    else
        goto L_630f;

L_630f:
    if ((md == mdBuildEnemyShdef))
        goto L_60cc;
    else
        goto L_6317;

L_6317:
    if ((md == mdBuildComp))
        goto L_6276;
    else
        goto L_631f;

L_631f:
    if ((md != mdBuildEdit))
        goto L_5f9c;
    else
        goto L_6324;

L_6324:

L_632a:
    i = LOWORD(SendMessage(hwndDD, CB_GETCOUNT, 0x0, 0x0));
    if ((i <= 32))
        goto L_6353;
    else
        goto L_634e;

L_634e:
    i = 32;

L_6353:
    GetWindowRect(hwndDD, &(rc));
    if ((dyArial8 <= 14))
        goto L_6388;
    else
        goto L_6382;

L_6382:
    goto L_638b;

L_6388:

L_638b:
    if ((dyArial8 > 14))
        goto L_63a9;
    else
        goto L_63a3;

L_63a3:
    t_merge_63ac_0001 = 0x1;
    goto L_63ac;

L_63a9:
    t_merge_63ac_0001 = 0x0;

L_63ac:
    /* untranslated: call SetWindowPos(hwndDD, 0x0, 0x0, 0x0, (rc.right - rc.left), ((loword((ss:[bp-0x24] * (dyArial8 - t_merge_63ac_0001))) + 0x8) +
     * ss:[bp-0x22]), 0x6) -> callresult(int16_t) */
    SendMessage(hwndDD, CB_SETCURSEL, 0x0, 0x0);
    return;
}

void FillBuildPartsLB(HWND hwndLB, int16_t grbit) {
    int16_t mdAvail;
    int16_t i;
    char    sz[200];
    int16_t grbitCur;
    PART    part;

L_63e8:
    grbitCur = 1;
    sz[0] = 65;
    SendMessage(hwndLB, CB_DIR, 0x0, 0x0);
    goto L_6518;

L_6417:
    if (((grbitCur & grbit) == 0x0))
        goto L_650c;
    else
        goto L_6426;

L_6426:
    i = 0;
    part.hs.grhst = grbitCur;

L_6436:
    part.hs.iItem = i;
    FLookupPart(&(part));
    /* untranslated: mdAvail = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) == 0 ? L_650c : L_645f */

L_645f:

L_6465:
    if ((fStarbaseMode == 0))
        goto L_6491;
    else
        goto L_646f;

L_646f:
    if ((grbitCur != 2048))
        goto L_6491;
    else
        goto L_647a;

L_647a:
    if ((i == 15))
        goto L_648c;
    else
        goto L_6483;

L_6483:
    if ((i != 16))
        goto L_6491;
    else
        goto L_648c;

L_648c:
    mdAvail = -1;

L_6491:
    if ((mdAvail != 1))
        goto L_6505;
    else
        goto L_649a;

L_649a:
    sz[1] = LOBYTE((i + 0x41));
    sz[2] = LOBYTE((((uint32_t)(part.pcom->ibmp) % 0x1a) + 0x41));
    sz[3] = LOBYTE((((uint32_t)(part.pcom->ibmp) / 0x1a) + 0x41));
    /* untranslated: call fstrcpy(&sz[0x4], &part[8:0](part)) -> callresult(char *) */
    /* untranslated: call SendMessage(hwndLB, CB_LIMITTEXT, 0x0, words(ss, sz)) -> callresult(LRESULT) */

L_6505:
    i = (i + 1);

L_650c:
    grbitCur = (grbitCur * 2);
    sz[0] = (sz[0] + 1);

L_6518:
    if ((grbitCur != 0))
        goto L_6417;
    else
        goto L_6522;

L_6522:
    return;
}

void UpdateSlotGlobals() {
    int16_t  yTop;
    int16_t  cSlot;
    int16_t  i;
    uint16_t wrc;
    int16_t  xLeft;
    HULDEF  *lphuldef;

L_6528:
    if ((LOWORD(lpshdefBuild) != 0x0))
        goto L_654d;
    else
        goto L_653b;

L_653b:
    if ((HIWORD(lpshdefBuild) != 0x0))
        goto L_654d;
    else
        goto L_6545;

L_6545:
    cSlot = 0;
    goto L_670f;

L_654d:
    lphuldef = LphuldefFromId(lpshdefBuild->hul.ihuldef);
    if ((hwndSlotDlg != 0x0))
        goto L_657d;
    else
        goto L_656c;

L_656c:
    xLeft = 12;
    yTop = (dyArial8 + 12);
    goto L_658b;

L_657d:
    xLeft = (ptslotGlob.x - 330);
    yTop = 32;

L_658b:
    cSlot = lphuldef->hul.chs;
    i = 0;
    goto L_665d;

L_65a0:
    vrgrcSlot[i].left = (((lphuldef->rgbrc[i] & 0xf) * 0x20) + xLeft);
    vrgrcSlot[i].top = (((lphuldef->rgbrc[i] >> 0x4) * 0x20) + yTop);
    vrgrcSlot[i].right = (vrgrcSlot[i].left + 64);
    vrgrcSlot[i].bottom = (vrgrcSlot[i].top + 64);
    i = (i + 1);

L_665d:
    if ((i < cSlot))
        goto L_65a0;
    else
        goto L_6668;

L_6668:
    if ((lphuldef->hul.wtCargoMax == 0x0))
        goto L_66fd;
    else
        goto L_6675;

L_6675:
    wrc = lphuldef->wrcCargo;
    rcCargo.left = (((((wrc >> 0x8) & 0xff) & 0xf) * 0x20) + xLeft);
    rcCargo.top = (((((wrc >> 0x8) & 0xff) >> 0x4) * 0x20) + yTop);
    rcCargo.right = (((((wrc & 0xff) & 0xff) & 0xf) * 0x20) + xLeft);
    rcCargo.bottom = (((((wrc & 0xff) & 0xff) >> 0x4) * 0x20) + yTop);

L_66fd:
    ptPlaque.x = (xLeft + 258);
    ptPlaque.y = (yTop + 273);

L_670f:
    return;
}

int16_t IEmptyBmpFromGrhst(int16_t grhst) {
    int16_t i;

L_6716:
    i = 0;
    goto L_6742;

L_6727:
    if ((rgmapBuildBmps[i] != grhst))
        goto L_673e;
    else
        goto L_6738;

L_6738:
    return i;

L_673e:
    i = (i + 1);

L_6742:
    if ((i < 21))
        goto L_6727;
    else
        goto L_674b;

L_674b:
    return 0x0;
}

int32_t FakeListProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    POINT   pt;
    int16_t iSel;

L_6758:
    goto L_6909;

L_676a:
    GetCursorPos(&(pt));
    ScreenToClient(hwnd, &(pt));
    if ((pt.x >= 64))
        goto L_6924;
    else
        goto L_678e;

L_678e:
    SetCursor(hcurHand);
    return 0x10000;

L_67a3:
    if ((LOWORD(lParam) >= 0x40))
        goto L_6924;
    else
        goto L_67b1;

L_67b1:
    if ((mdBuild == mdBuildEdit))
        goto L_67c5;
    else
        goto L_67bb;

L_67bb:
    if ((msg != WM_RBUTTONDOWN))
        goto L_6924;
    else
        goto L_67c5;

L_67c5:
    CallWindowProc(lpfnRealListProc, hwnd, 0x201, wParam, lParam);
    CallWindowProc(lpfnRealListProc, hwnd, 0x202, wParam, lParam);
    if ((msg == WM_RBUTTONDOWN))
        goto L_6813;
    else
        goto L_6809;

L_6809:
    if ((mdBuild == mdBuildEdit))
        goto L_68ca;
    else
        goto L_6813;

L_6813:
    iSel = LOWORD(SendMessage(hwnd, CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((iSel != -1))
        goto L_6840;
    else
        goto L_6837;

L_6837:
    return 0x0;

L_6840:
    /* untranslated: call SendMessage(hwnd, CB_INSERTSTRING, iSel, words(ds, 0x57a4)) -> callresult(LRESULT) */
    HIWORD(GlobalPD) = (0x1 << ((uint16_t)(LOWORD(szWork)) + 0xffbf));
    GlobalPD.part.hs.iItem = ((uint16_t)(szWork[0x1]) - 65);
    /* untranslated: call FLookupPart(&part[2:0](GlobalPD)) -> callresult(int16_t) */
    GlobalPD.grPopup = grPopupComponent;
    Popup(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff));
    return 0x0;

L_68ca:
    FTrackSlot(hwnd, LOWORD(lParam), (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff), wParam, 0x1, 0x0);
    return 0x0;

L_6909:
    if ((msg == WM_SETCURSOR))
        goto L_676a;
    else
        goto L_6911;

L_6911:
    if ((msg == WM_LBUTTONDOWN))
        goto L_67a3;
    else
        goto L_6919;

L_6919:
    if ((msg == WM_RBUTTONDOWN))
        goto L_67a3;
    else
        goto L_6924;

L_6924:
    return CallWindowProc(lpfnRealListProc, hwnd, msg, wParam, lParam);
}

void MakeNewName(char *lpsz) {
    int16_t cLen;

L_694c:
    cLen = fstrlen(lpsz);
    if ((cLen > 27))
        goto L_6a39;
    else
        goto L_696c;

L_696c:

L_6972:
    if (((uint16_t)(lpsz[(cLen - 1)]) != 41))
        goto L_69d3;
    else
        goto L_698e;

L_698e:
    /* untranslated: branch (byte ds:[(95 + sext8to16(lpsz[(cLen - 2)]))] & 0x4) == 0x0 ? L_69d3 : L_69b7 */

L_69b7:
    if (((uint16_t)(lpsz[(cLen - 3)]) == 40))
        goto L_69f2;
    else
        goto L_69d3;

L_69d3:
    fstrcpy(&(lpsz[cLen]), " (2)");
    goto L_6a39;

L_69f2:
    if (((uint16_t)(lpsz[(cLen - 2)]) != 57))
        goto L_6a25;
    else
        goto L_6a0e;

L_6a0e:
    lpsz[(cLen - 2)] = 48;
    goto L_6a39;

L_6a25:
    lpsz[(cLen - 2)] = (lpsz[(cLen - 2)] + 1);

L_6a39:
    FStringFitsScreen(lpsz, 0xa0);
    return;
}

void KillQueuedMassPackets(PLANET *lppl) {
    int16_t iprod;
    int16_t iDst;
    PROD   *lpprod;

L_6a52:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_6a72;
    else
        goto L_6a68;

L_6a68:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_6c23;
    else
        goto L_6a72;

L_6a72:
    if ((lppl->lpplprod->iprodMac == 0x0))
        goto L_6c23;
    else
        goto L_6a85;

L_6a85:

L_6a8b:
    iDst = 0;
    iprod = 0;
    lpprod = lppl->lpplprod->rgprod;
    goto L_6ac1;

L_6aae:
    iprod = (iprod + 1);
    lpprod = (lpprod + 0x1);

L_6ac1:
    if ((iprod >= lppl->lpplprod->iprodMac))
        goto L_6b97;
    else
        goto L_6ad7;

L_6ad7:
    if ((lpprod->grobj != 0x1))
        goto L_6b59;
    else
        goto L_6af7;

L_6af7:
    if ((0x0 != 0x0))
        goto L_6b59;
    else
        goto L_6aff;

L_6aff:
    if ((0x0 < 0x0))
        goto L_6b59;
    else
        goto L_6b1f;

L_6b1f:
    if ((0x0 > 0x0))
        goto L_6b2c;
    else
        goto L_6b24;

L_6b24:
    if ((lpprod->iItem < 0xe))
        goto L_6b59;
    else
        goto L_6b2c;

L_6b2c:
    if ((0x0 < 0x0))
        goto L_6aae;
    else
        goto L_6b4c;

L_6b4c:
    if ((0x0 > 0x0))
        goto L_6b59;
    else
        goto L_6b51;

L_6b51:
    if ((lpprod->iItem <= 0x11))
        goto L_6aae;
    else
        goto L_6b59;

L_6b59:
    if ((iDst == iprod))
        goto L_6b90;
    else
        goto L_6b64;

L_6b64:
    LOWORD(lppl->lpplprod[(iDst + 1)]) = LOWORD(lpprod);
    HIWORD(lppl->lpplprod[iDst].rgprod[0x0]) = *(lpprod + 0x2);

L_6b90:
    iDst = (iDst + 1);

L_6b97:
    if ((iDst != 0))
        goto L_6bc5;
    else
        goto L_6ba0;

L_6ba0:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;
    goto L_6bde;

L_6bc5:
    if ((iDst == iprod))
        goto L_6bde;
    else
        goto L_6bd0;

L_6bd0:
    lppl->lpplprod->iprodMac = LOBYTE(iDst);

L_6bde:
    if ((sel.grobj != grobjPlanet))
        goto L_6c23;
    else
        goto L_6be8;

L_6be8:
    if ((sel.pl.id != lppl->id))
        goto L_6c23;
    else
        goto L_6bf7;

L_6bf7:
    FLookupPlanet(sel.pl.id, &(sel.pl));
    FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);

L_6c23:
    return;
}

void KillQueuedShips(PLANET *lppl) {
    int16_t iprod;
    int16_t iDst;
    PROD   *lpprod;

L_6c2a:
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_6c4a;
    else
        goto L_6c40;

L_6c40:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_6e17;
    else
        goto L_6c4a;

L_6c4a:
    if ((lppl->lpplprod->iprodMac == 0x0))
        goto L_6e17;
    else
        goto L_6c5d;

L_6c5d:

L_6c63:
    iDst = 0;
    iprod = 0;
    lpprod = lppl->lpplprod->rgprod;
    goto L_6c99;

L_6c86:
    iprod = (iprod + 1);
    lpprod = (lpprod + 0x1);

L_6c99:
    if ((iprod >= lppl->lpplprod->iprodMac))
        goto L_6d8b;
    else
        goto L_6caf;

L_6caf:
    if ((lpprod->grobj != 0x2))
        goto L_6d04;
    else
        goto L_6ccf;

L_6ccf:
    if ((0x0 != 0x0))
        goto L_6d04;
    else
        goto L_6cd7;

L_6cd7:
    if ((0x0 < 0x0))
        goto L_6c86;
    else
        goto L_6cf7;

L_6cf7:
    if ((0x0 > 0x0))
        goto L_6d04;
    else
        goto L_6cfc;

L_6cfc:
    if ((lpprod->iItem < 0x10))
        goto L_6c86;
    else
        goto L_6d04;

L_6d04:
    if ((lpprod->grobj != 0x2))
        goto L_6d4d;
    else
        goto L_6d24;

L_6d24:
    if ((0x0 != 0x0))
        goto L_6d4d;
    else
        goto L_6d2c;

L_6d2c:
    LOWORD(lpprod) = ((LOWORD(lpprod) & 0xffff) | 0x0);
    lpprod->pct = 0x0;

L_6d4d:
    if ((iDst == iprod))
        goto L_6d84;
    else
        goto L_6d58;

L_6d58:
    LOWORD(lppl->lpplprod[(iDst + 1)]) = LOWORD(lpprod);
    HIWORD(lppl->lpplprod[iDst].rgprod[0x0]) = *(lpprod + 0x2);

L_6d84:
    iDst = (iDst + 1);

L_6d8b:
    if ((iDst != 0))
        goto L_6db9;
    else
        goto L_6d94;

L_6d94:
    FreePl(lppl->lpplprod);
    lppl->lpplprod = 0x0;
    goto L_6dd2;

L_6db9:
    if ((iDst == iprod))
        goto L_6dd2;
    else
        goto L_6dc4;

L_6dc4:
    lppl->lpplprod->iprodMac = LOBYTE(iDst);

L_6dd2:
    if ((sel.grobj != grobjPlanet))
        goto L_6e17;
    else
        goto L_6ddc;

L_6ddc:
    if ((sel.pl.id != lppl->id))
        goto L_6e17;
    else
        goto L_6deb;

L_6deb:
    FLookupPlanet(sel.pl.id, &(sel.pl));
    FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, 0x0);

L_6e17:
    return;
}
