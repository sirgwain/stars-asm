#include "common.h"

int16_t ZipOrderDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    HDC            hdc;
    int16_t        i;
    PAINTSTRUCT    ps;
    RECT           rc;
    HWND           hwndRad;
    char          *psz;
    char          *pszT;
    RECT           rcGBox;
    int16_t        cch;
    int16_t        xCtr;
    XferActionType iAction;
    int16_t (**lpProc)();
    uint16_t t_merge_0524_0001;

L_0000:
    goto L_07f8;

L_000f:
    SetWindowText(hwnd, PszGetCompressedString(idsCustomizeZipOrders));
    ShowWindow(GetDlgItem(hwnd, IDC_U16_0x0417), SW_HIDE);
    hwndZipOrderDlg = hwnd;
    CheckRadioButton(hwnd, 0x431, 0x434, 0x431);
    EnableZipBtns(hwnd, 0x0);
    iResTechNow = 0;
    i = 1073;
    goto L_0145;

L_0074:
    if ((vrgZip[(i - 1073)].fValid == 0x0))
        goto L_00f3;
    else
        goto L_0092;

L_0092:
    pszT = szWork;
    psz = vrgZip[(i - 0x431)].szName;

L_00af:
    if (((uint16_t)(*(psz)) == 0))
        goto L_00e5;
    else
        goto L_00bd;

L_00bd:
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_00af;
    else
        goto L_00d8;

L_00d8:
    pszT = (pszT + 0x1);
    *(pszT) = 38;

L_00e5:
    *(pszT) = 0;
    psz = szWork;
    goto L_0124;

L_00f3:
    psz = PszGetCompressedString(idsUnusedD);
    _wsprintf(szWork, psz, (i - 1072));
    psz = szWork;

L_0124:
    hwndRad = GetDlgItem(hwnd, i);
    SetWindowText(hwndRad, psz);
    i = (i + 1);

L_0145:
    if ((i <= 1076))
        goto L_0074;
    else
        goto L_014f;

L_014f:
    StickyDlgPos(hwnd, &(ptStickyZipOrderDlg), 0x1);
    if ((gd.fTutorial == 0x0))
        goto L_017a;
    else
        goto L_0175;

L_0175:
    AdvanceTutor();

L_017a:
    return 0x1;

L_0180:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_01a8:
    i = 1073;
    goto L_01d5;

L_01b0:
    /* untranslated: ss:[bp-0x30] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x30] == GetDlgItem(hwnd, i) ? L_01df : L_01cb */

L_01cb:

L_01d1:
    i = (i + 1);

L_01d5:
    if ((i <= 1076))
        goto L_01b0;
    else
        goto L_01df;

L_01df:
    if ((i > 1076))
        goto L_0823;
    else
        goto L_01e9;

L_01e9:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_0202:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    GetWindowRect(GetDlgItem(hwnd, 0x431), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0434), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    cch = CchGetString(idsCustomOrders, szWork);
    TextOut(hdc, (rcGBox.left + 8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, cch);
    rcGBox.top = (rcGBox.bottom + 8);
    if ((vrgZip[iResTechNow].fValid == 0x0))
        goto L_0444;
    else
        goto L_031e;

L_031e:
    xCtr = (LOWORD(GetTextExtent(hdc, rgszMinerals[0x2], 0x9)) + 8);
    i = 0;
    goto L_0438;

L_033f:
    SetTextColor(hdc, rgcrMinerals[i]);
    RightTextOut(hdc, xCtr, rcGBox.top, rgszMinerals[i], 0x0, 0x0);
    SetTextColor(hdc, 0x0);
    iAction = vrgZip[iResTechNow].txp.rgia[i].iAction;
    cch = CchGetString((iAction + 109), szWork);
    if (((uint16_t)(szWork[(cch - 1)]) != 46))
        goto L_0408;
    else
        goto L_03d1;

L_03d1:
    _wsprintf(&(szWork[(cch - 3)]), " %dkT", vrgZip[iResTechNow].txp.rgia[i].cQuan);

L_0408:
    TextOut(hdc, (xCtr + 6), rcGBox.top, szWork, strlen(szWork));
    rcGBox.top = (rcGBox.top + dyArial8);
    i = (i + 1);

L_0438:
    if ((i >= 5))
        goto L_0470;
    else
        goto L_043e;

L_043e:

L_0444:
    cch = CchGetString(idsEmptyCustomSlot, szWork);
    TextOut(hdc, 0xc, rcGBox.top, szWork, cch);

L_0470:
    EndPaint(hwnd, &(ps));
    return 0x1;

L_0485:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x0))
        goto L_04e4;
    else
        goto L_04a1;

L_04a1:
    if ((wParam < 0x431))
        goto L_04e4;
    else
        goto L_04ab;

L_04ab:
    if ((wParam > 0x434))
        goto L_04e4;
    else
        goto L_04b5;

L_04b5:
    iResTechNow = (wParam - 1073);
    EnableZipBtns(hwnd, iResTechNow);
    InvalidateRect(hwnd, 0x0, 0x1);
    goto L_0823;

L_04e4:
    if ((wParam == 0x1))
        goto L_04f6;
    else
        goto L_04ed;

L_04ed:
    if ((wParam != 0x2))
        goto L_054b;
    else
        goto L_04f6;

L_04f6:
    hwndZipOrderDlg = 0x0;
    StickyDlgPos(hwnd, &(ptStickyZipOrderDlg), 0x0);
    if ((wParam != 0x1))
        goto L_0521;
    else
        goto L_051b;

L_051b:
    t_merge_0524_0001 = 0x1;
    goto L_0524;

L_0521:
    t_merge_0524_0001 = 0x0;

L_0524:
    EndDialog(hwnd, t_merge_0524_0001);
    if ((gd.fTutorial == 0x0))
        goto L_0542;
    else
        goto L_053d;

L_053d:
    AdvanceTutor();

L_0542:
    return 0x1;

L_054b:
    if ((wParam == 0x816))
        goto L_055f;
    else
        goto L_0555;

L_0555:
    if ((wParam != 0x41b))
        goto L_0749;
    else
        goto L_055f;

L_055f:
    if ((vrgZip[iResTechNow].fValid == 0x0))
        goto L_059a;
    else
        goto L_0579;

L_0579:
    strcpy(szWork, vrgZip[iResTechNow].szName);
    goto L_05bd;

L_059a:
    _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);

L_05bd:
    lpProc = MakeProcInstance(RenameZipDlg, hInst);
    if ((DialogBox(hInst, IDD_RENAME, hwndFrame, &(lpProc)) == 0))
        goto L_0727;
    else
        goto L_05f7;

L_05f7:
    if (((uint16_t)(LOWORD(szWork)) != 0x0))
        goto L_0626;
    else
        goto L_0603;

L_0603:
    _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);

L_0626:
    strcpy(vrgZip[iResTechNow].szName, szWork);
    pszT = &(szWork[0x40]);
    psz = szWork;

L_064e:
    if (((uint16_t)(*(psz)) == 0))
        goto L_0684;
    else
        goto L_065c;

L_065c:
    psz = (psz + 0x1);
    pszT = (pszT + 0x1);
    *(pszT) = *(psz);
    if (((uint16_t)(*(psz)) != 38))
        goto L_064e;
    else
        goto L_0677;

L_0677:
    pszT = (pszT + 0x1);
    *(pszT) = 38;

L_0684:
    *(pszT) = 0;
    SetWindowText(GetDlgItem(hwnd, (iResTechNow + 1073)), "");
    if ((wParam != 0x816))
        goto L_0718;
    else
        goto L_06b0;

L_06b0:
    vrgZip[iResTechNow].fValid = 0x1;
    vrgZip[iResTechNow].txp.rgia = sel.fl.lpplord->rgord[sel.iwpAct].txp.rgia;
    InvalidateRect(hwnd, 0x0, 0x1);

L_0718:
    EnableZipBtns(hwnd, iResTechNow);

L_0727:
    FreeProcInstance(&(lpProc));
    SetFocus(hwnd);
    gd.fChgZipOrd = 0x1;
    goto L_0823;

L_0749:
    if ((wParam != 0x817))
        goto L_07c8;
    else
        goto L_0753;

L_0753:
    vrgZip[iResTechNow].fValid = 0x0;
    _wsprintf(szWork, PszGetCompressedString(idsUnusedD), (iResTechNow + 1));
    SetWindowText(GetDlgItem(hwnd, (iResTechNow + 1073)), szWork);
    InvalidateRect(hwnd, 0x0, 0x1);
    gd.fChgZipOrd = 0x1;
    goto L_0823;

L_07c8:
    if ((wParam != 0x76))
        goto L_0823;
    else
        goto L_07d1;

L_07d1:
    WinHelp(hwnd, szHelpFile, 0x1, 0x44a);
    return 0x1;

L_07f8:
    if ((message == WM_PAINT))
        goto L_0202;
    else
        goto L_0800;

L_0800:
    if ((message == WM_ERASEBKGND))
        goto L_0180;
    else
        goto L_0808;

L_0808:
    if ((message == WM_CTLCOLOR))
        goto L_01a8;
    else
        goto L_0810;

L_0810:
    if ((message == WM_INITDIALOG))
        goto L_000f;
    else
        goto L_0818;

L_0818:
    if ((message == WM_COMMAND))
        goto L_0485;
    else
        goto L_0823;

L_0823:
    return 0x0;
}

void EnableZipBtns(HWND hwnd, int16_t iSel) {
    int16_t fEnabled;

L_0832:
    fEnabled = vrgZip[iSel].fValid;
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), fEnabled);
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), fEnabled);
    return;
}

int16_t RenameZipDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    StringId ids;
    RECT     rc;
    uint16_t t_merge_09e8_0001;
    uint16_t t_merge_0a23_0001;

L_0880:
    goto L_0a36;

L_088f:
    if ((hwndZipOrderDlg == 0x0))
        goto L_08a1;
    else
        goto L_0899;

L_0899:
    ids = idsRenameZipOrder;
    goto L_08a6;

L_08a1:
    ids = idsRenameProductionTemplate;

L_08a6:
    SetWindowText(hwnd, PszGetCompressedString(ids));
    SetWindowPos(hwnd, 0x0, (ptStickyRenameDlg.x + 70), (ptStickyRenameDlg.y + 70), 0x0, 0x0, 0x15);
    SendDlgItemMessage(hwnd, 0x10c, 0x415, 0xc, 0x0);
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT1), szWork);
    StickyDlgPos(hwnd, &(ptStickyRenameDlg), 0x1);
    return 0x1;

L_0931:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0959:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_0a59;
    else
        goto L_0975;

L_0975:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_098e:
    if ((wParam == 0x1))
        goto L_09a0;
    else
        goto L_0997;

L_0997:
    if ((wParam != 0x2))
        goto L_09f7;
    else
        goto L_09a0;

L_09a0:
    if ((wParam != 0x1))
        goto L_09c0;
    else
        goto L_09a9;

L_09a9:
    GetDlgItemText(hwnd, IDC_EDIT1, szWork, 0xe);

L_09c0:
    StickyDlgPos(hwnd, &(ptStickyRenameDlg), 0x0);
    if ((wParam != 0x1))
        goto L_09e5;
    else
        goto L_09df;

L_09df:
    t_merge_09e8_0001 = 0x1;
    goto L_09e8;

L_09e5:
    t_merge_09e8_0001 = 0x0;

L_09e8:
    EndDialog(hwnd, t_merge_09e8_0001);
    return 0x1;

L_09f7:
    if ((wParam != 0x76))
        goto L_0a59;
    else
        goto L_0a00;

L_0a00:
    if ((hwndZipOrderDlg != 0x0))
        goto L_0a1f;
    else
        goto L_0a18;

L_0a18:
    t_merge_0a23_0001 = 0x452;
    goto L_0a23;

L_0a1f:
    t_merge_0a23_0001 = 0xc1f;

L_0a23:
    /* untranslated: call WinHelp(hwnd, szHelpFile, 0x1, words(0x0, t_merge_0a23_0001)) -> callresult(int16_t) */
    return 0x1;

L_0a36:
    if ((message == WM_ERASEBKGND))
        goto L_0931;
    else
        goto L_0a3e;

L_0a3e:
    if ((message == WM_CTLCOLOR))
        goto L_0959;
    else
        goto L_0a46;

L_0a46:
    if ((message == WM_INITDIALOG))
        goto L_088f;
    else
        goto L_0a4e;

L_0a4e:
    if ((message == WM_COMMAND))
        goto L_098e;
    else
        goto L_0a59;

L_0a59:
    return 0x0;
}

int16_t RenameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT     rc;
    int32_t  lSel;
    uint16_t t_merge_0bcd_0001;

L_0a68:
    goto L_0ccc;

L_0a77:
    SetWindowText(hwnd, PszGetCompressedString(idsRenameFleet));
    SetWindowPos(hwnd, 0x0, (ptStickyRenameDlg.x + 70), (ptStickyRenameDlg.y + 70), 0x0, 0x0, 0x15);
    SendDlgItemMessage(hwnd, 0x10c, 0x415, 0x1f, 0x0);
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT1), szWork);
    StickyDlgPos(hwnd, &(ptStickyRenameDlg), 0x1);
    return 0x1;

L_0b03:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0b2b:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_0cef;
    else
        goto L_0b47;

L_0b47:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_0b60:
    if ((wParam == 0x1))
        goto L_0b72;
    else
        goto L_0b69;

L_0b69:
    if ((wParam != 0x2))
        goto L_0bdc;
    else
        goto L_0b72;

L_0b72:
    if ((wParam != 0x1))
        goto L_0ba5;
    else
        goto L_0b7b;

L_0b7b:
    GetDlgItemText(hwnd, IDC_EDIT1, szWork, 0x20);
    FStringFitsScreen(szWork, 0xa0);

L_0ba5:
    StickyDlgPos(hwnd, &(ptStickyRenameDlg), 0x0);
    if ((wParam != 0x1))
        goto L_0bca;
    else
        goto L_0bc4;

L_0bc4:
    t_merge_0bcd_0001 = 0x1;
    goto L_0bcd;

L_0bca:
    t_merge_0bcd_0001 = 0x0;

L_0bcd:
    EndDialog(hwnd, t_merge_0bcd_0001);
    return 0x1;

L_0bdc:
    if ((wParam != 0x10c))
        goto L_0c9c;
    else
        goto L_0be6;

L_0be6:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x400))
        goto L_0c9c;
    else
        goto L_0c02;

L_0c02:
    if ((fInEditUpdate != 0))
        goto L_0c9c;
    else
        goto L_0c0c;

L_0c0c:
    fInEditUpdate = 1;
    GetWindowText(LOWORD(lParam), szWork, 0xfa);
    lSel = SendMessage(LOWORD(lParam), WM_USER, 0x0, 0x0);
    if ((FStringFitsScreen(szWork, 0xa0) != 0))
        goto L_0c93;
    else
        goto L_0c66;

L_0c66:
    SetWindowText(LOWORD(lParam), szWork);
    SendMessage(LOWORD(lParam), CB_LIMITTEXT, 0x0, lSel);

L_0c93:
    fInEditUpdate = 0;
    goto L_0cef;

L_0c9c:
    if ((wParam != 0x76))
        goto L_0cef;
    else
        goto L_0ca5;

L_0ca5:
    WinHelp(hwnd, szHelpFile, 0x1, 0x447);
    return 0x1;

L_0ccc:
    if ((message == WM_ERASEBKGND))
        goto L_0b03;
    else
        goto L_0cd4;

L_0cd4:
    if ((message == WM_CTLCOLOR))
        goto L_0b2b;
    else
        goto L_0cdc;

L_0cdc:
    if ((message == WM_INITDIALOG))
        goto L_0a77;
    else
        goto L_0ce4;

L_0ce4:
    if ((message == WM_COMMAND))
        goto L_0b60;
    else
        goto L_0cef;

L_0cef:
    return 0x0;
}

int16_t FStargateJump(FLEET *lpfl, int16_t isbsSrc, int16_t isbsDst, int16_t dDist) {
    int16_t   dpPerShdefNew;
    int16_t   dpShdef;
    POINT     pt;
    int16_t   id;
    FLEET     flSrc;
    int16_t   cshT;
    uint8_t   pctKill;
    int16_t   i;
    int32_t   cshOrig;
    MessageId idm;
    int32_t   cshKill;
    int16_t   pct;
    int16_t   rgpct[16];
    int16_t   cshdef;
    int16_t   ishdef;
    int32_t   dp;
    int16_t   dpPerShdefOld;
    int16_t   cshDamagedOld;
    FLEET     flDead;

L_0cfe:
    cshdef = 0;
    cshKill = 0;
    cshOrig = 0;
    memset(rgpct, 0x0, 0x20);
    flSrc = *(lpfl);
    if ((flSrc.lpplord->rgord[1].grobj != 0x1))
        goto L_0d77;
    else
        goto L_0d6a;

L_0d6a:
    id = flSrc.lpplord->rgord[0x1].id;
    goto L_0dc4;

L_0d77:
    pt.x = flSrc.lpplord->rgord[0x1].pt.x;
    pt.y = flSrc.lpplord->rgord[0x1].pt.y;
    id = 0;
    goto L_0d94;

L_0d90:
    id = (id + 1);

L_0d94:
    if ((id >= game.cPlanMax))
        goto L_0dc4;
    else
        goto L_0d9f;

L_0d9f:
    if ((pt.x != rgptPlan[id].x))
        goto L_0d90;
    else
        goto L_0db6;

L_0db6:
    if ((pt.y == rgptPlan[id].y))
        goto L_0dc4;
    else
        goto L_0dbb;

L_0dbb:

L_0dc4:
    ishdef = 0;
    goto L_0ef2;

L_0dcd:
    if ((flSrc.rgcsh[ishdef] == 0))
        goto L_0eed;
    else
        goto L_0de0;

L_0de0:
    cshdef = (cshdef + 1);
    cshOrig = (cshOrig + (uint32_t)(flSrc.rgcsh[ishdef]));
    MdCalcStargateDamage(isbsSrc, isbsDst, dDist, rglpshdef[flSrc.iPlayer][ishdef].hul.wtEmpty, rgpct[ishdef]);
    goto L_0eca;

L_0e3c:
    FSendPlrMsg(flSrc.iPlayer, 0xe3, (flSrc.id | 0x8000), flSrc.id, flSrc.idPlanet, id, 0x0, 0x0, 0x0, 0x0);
    return 0x0;

L_0e75:
    FSendPlrMsg(flSrc.iPlayer, 0xe4, (flSrc.id | 0x8000), flSrc.id, flSrc.idPlanet, id, ishdef, 0x0, 0x0, 0x0);
    return 0x0;

L_0eae:
    cshdef = (cshdef - 1);
    goto L_0eed;

L_0eb6:
    flSrc.fNoHeal = 0x1;
    goto L_0eed;

L_0eca:
    /* untranslated: branch callresult(int16_t) == -2 ? L_0e75 : L_0ed2 */

L_0ed2:
    /* untranslated: branch callresult(int16_t) == -1 ? L_0e3c : L_0eda */

L_0eda:
    /* untranslated: branch callresult(int16_t) == 0 ? L_0eae : L_0ee2 */

L_0ee2:
    /* untranslated: branch callresult(int16_t) == 1 ? L_0eb6 : L_0eed */

L_0eed:
    ishdef = (ishdef + 1);

L_0ef2:
    if ((ishdef < 16))
        goto L_0dcd;
    else
        goto L_0efc;

L_0efc:
    if ((cshdef != 0))
        goto L_0f53;
    else
        goto LKilledEmAll;

LKilledEmAll:
    lpfl->fDead = 0x1;
    FSendPlrMsg(flSrc.iPlayer, 0xe7, (flSrc.id | 0x8000), flSrc.id, flSrc.idPlanet, id, 0x0, 0x0, 0x0, 0x0);
    return 0x0;

L_0f53:
    memset(flDead, 0x0, 0x7c);
    ishdef = 0;
    goto L_1390;

L_0f71:
    if ((flSrc.rgcsh[ishdef] == 0))
        goto L_138b;
    else
        goto L_0f84;

L_0f84:
    if ((rgpct[ishdef] == 0))
        goto L_138b;
    else
        goto L_0f98;

L_0f98:
    if ((rgpct[ishdef] != 100))
        goto L_0fe8;
    else
        goto L_0fac;

L_0fac:
    cshKill = (cshKill + (uint32_t)(flSrc.rgcsh[ishdef]));
    flSrc.rgcsh[ishdef] = 0;
    /* untranslated: ss:[bp+(ishdef * 2)-0x5c] = 0x0 */
    cshdef = (cshdef - 1);
    goto L_1358;

L_0fe8:
    cshT = flSrc.rgcsh[ishdef];
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raStargate))
        goto L_1025;
    else
        goto L_101d;

L_101d:
    pctKill = 0x0;
    goto L_103d;

L_1025:
    pctKill = LOBYTE(((uint32_t)(rgpct[ishdef]) / 0x3));

L_103d:
    dpShdef = rglpshdef[lpfl->iPlayer][ishdef].hul.dp;
    /* untranslated: branch ss:[bp+(ishdef * 2)-0x5c] == 0x0 ? L_10ba : L_1075 */

L_1075:
    /* untranslated: cshDamagedOld = loword((int32_t)(words(hiword((uint32_t)(sext16to32(cshT) * words(signhiword((ss:[bp+(ishdef * 0x2)-0x5c] & 0x7f)),
     * (ss:[bp+(ishdef * 0x2)-0x5c] & 0x7f)))), loword((uint32_t)(sext16to32(cshT) * words(signhiword((ss:[bp+(ishdef * 0x2)-0x5c] & 0x7f)), (ss:[bp+(ishdef *
     * 0x2)-0x5c] & 0x7f))))) / 0x64)) */
    if ((cshDamagedOld != 0))
        goto L_10c0;
    else
        goto L_10b1;

L_10b1:
    cshDamagedOld = 1;

L_10ba:
    cshDamagedOld = 0;

L_10c0:
    if ((pctKill <= 0x0))
        goto L_116f;
    else
        goto L_10cf;

L_10cf:
    i = 0;
    goto L_10dd;

L_10d8:
    i = (i + 1);

L_10dd:
    if ((i >= flSrc.rgcsh[ishdef]))
        goto L_1155;
    else
        goto L_10f3;

L_10f3:
    if ((Random(0x64) >= pctKill))
        goto L_10d8;
    else
        goto L_1115;

L_1115:
    cshT = (cshT - 1);
    if ((cshDamagedOld == 0))
        goto L_10d8;
    else
        goto L_1124;

L_1124:
    /* untranslated: branch Random(0x1f4) >= ((ss:[bp+(ishdef * 2)-0x5c] >> 0x7) & 0x1ff) ? L_10d8 : L_114d */

L_114d:
    cshDamagedOld = (cshDamagedOld - 1);

L_1155:
    cshKill = (cshKill + (uint32_t)((flSrc.rgcsh[ishdef] - cshT)));

L_116f:
    if ((cshT == 0))
        goto L_1329;
    else
        goto L_1179;

L_1179:
    /* untranslated: branch ss:[bp+(ishdef * 2)-0x5c] == 0x0 ? L_11d5 : L_118c */

L_118c:
    /* untranslated: dpPerShdefOld = loword((int32_t)(words(hiword((uint32_t)(sext16to32(dpShdef) * words(signhiword(((ss:[bp+(ishdef * 0x2)-0x5c] >> 0x7) &
     * 0x1ff)), ((ss:[bp+(ishdef * 0x2)-0x5c] >> 0x7) & 0x1ff)))), loword((uint32_t)(sext16to32(dpShdef) * words(signhiword(((ss:[bp+(ishdef * 0x2)-0x5c] >>
     * 0x7) & 0x1ff)), ((ss:[bp+(ishdef * 0x2)-0x5c] >> 0x7) & 0x1ff))))) / 0x1f4)) */
    if ((dpPerShdefOld != 0))
        goto L_11db;
    else
        goto L_11cc;

L_11cc:
    dpPerShdefOld = 1;

L_11d5:
    dpPerShdefOld = 0;

L_11db:
    dpPerShdefNew = LOWORD((int32_t)(((uint32_t)(((uint32_t)(dpShdef) * (uint32_t)(rgpct[ishdef]))) / 0x64)));
    if ((dpPerShdefNew != 0))
        goto L_1217;
    else
        goto L_1212;

L_1212:
    dpPerShdefNew = 1;

L_1217:
    if ((cshDamagedOld == 0))
        goto L_1245;
    else
        goto L_1221;

L_1221:
    if (((dpPerShdefNew + dpPerShdefOld) < dpShdef))
        goto L_1245;
    else
        goto L_1230;

L_1230:
    cshKill = (cshKill + (uint32_t)(cshDamagedOld));
    cshT = (cshT - cshDamagedOld);

L_1245:
    if ((cshT == 0))
        goto L_1329;
    else
        goto L_124f;

L_124f:
    dp = ((uint32_t)(((uint32_t)(dpPerShdefNew) * (uint32_t)(cshT))) + (uint32_t)(((uint32_t)(dpPerShdefOld) * (uint32_t)(cshDamagedOld))));
    pct = LOWORD((int32_t)(((uint32_t)(((int32_t)((dp / (uint32_t)(cshT))) * 0x1f4)) / (uint32_t)(dpShdef))));
    if ((pct != 0))
        goto L_12d4;
    else
        goto L_12ce;

L_12ce:
    pct = 1;

L_12d4:
    /* untranslated: ss:[bp+(ishdef * 2)-0x5c] = ((ss:[bp+(ishdef * 0x2)-0x5c] & 0x7f) | ((pct & 0x1ff) << 0x7)) */
    /* untranslated: ss:[bp+(ishdef * 2)-0x5c] = ((ss:[bp+(ishdef * 0x2)-0x5c] & 0xff80) | 0x64) */

L_1329:
    flSrc.rgcsh[ishdef] = cshT;
    if ((cshT != 0))
        goto L_1358;
    else
        goto L_1344;

L_1344:
    /* untranslated: ss:[bp+(ishdef * 2)-0x5c] = 0x0 */
    cshdef = (cshdef - 1);

L_1358:
    flDead.rgcsh[ishdef] = (lpfl->rgcsh[ishdef] - flSrc.rgcsh[ishdef]);

L_138b:
    ishdef = (ishdef + 1);

L_1390:
    if ((ishdef < 16))
        goto L_0f71;
    else
        goto L_139a;

L_139a:
    if ((cshdef == 0))
        goto LKilledEmAll;
    else
        goto L_13a1;

L_13a1:

L_13a7:
    if ((LOWORD(cshKill) != 0x0))
        goto L_13bb;
    else
        goto L_13b1;

L_13b1:
    if ((HIWORD(cshKill) == 0x0))
        goto L_150f;
    else
        goto L_13bb;

L_13bb:
    if ((0x0 != 0x0))
        goto L_13d9;
    else
        goto L_13d1;

L_13d1:
    if (((HIWORD(cshKill) & 0xffff) == 0x0))
        goto L_1431;
    else
        goto L_13d9;

L_13d9:
    FSendPlrMsg(lpfl->iPlayer, 0xeb, (lpfl->id | 0x8000), lpfl->id, lpfl->idPlanet, id, LOWORD(cshKill), (LOWORD((uint32_t)((cshKill >> 0x10))) & 0xffff), 0x0,
                0x0);
    goto L_14d9;

L_1431:
    if ((HIWORD(cshKill) > HIWORD((int32_t)((cshOrig >> 0x2)))))
        goto L_1461;
    else
        goto L_144a;

L_144a:
    if ((HIWORD(cshKill) < HIWORD((int32_t)((cshOrig >> 0x2)))))
        goto L_1458;
    else
        goto L_144f;

L_144f:
    if ((LOWORD(cshKill) >= LOWORD((int32_t)((cshOrig >> 0x2)))))
        goto L_1461;
    else
        goto L_1458;

L_1458:
    idm = idmUsedStargateReachLosingShipsTreacherousVoid;
    goto L_1497;

L_1461:
    if ((HIWORD(cshKill) < HIWORD((int32_t)((cshOrig >> 0x1)))))
        goto L_1491;
    else
        goto L_147a;

L_147a:
    if ((HIWORD(cshKill) > HIWORD((int32_t)((cshOrig >> 0x1)))))
        goto L_1488;
    else
        goto L_147f;

L_147f:
    if ((LOWORD(cshKill) <= LOWORD((int32_t)((cshOrig >> 0x1)))))
        goto L_1491;
    else
        goto L_1488;

L_1488:
    idm = idmUsedStargateReachUnfortunatelyLosingShipsGreat;
    goto L_1497;

L_1491:
    idm = idmUsedStargateReachLosingShipsUnforgivingVoid;

L_1497:
    FSendPlrMsg(lpfl->iPlayer, idm, (lpfl->id | 0x8000), lpfl->id, lpfl->idPlanet, id, LOWORD(cshKill), 0x0, 0x0, 0x0);

L_14d9:
    flDead.iPlayer = flSrc.iPlayer;
    flDead.fDead = 0x1;
    flDead.det = 0x7;
    FleetTransferCargoBalance(&(flSrc), &(flDead));

L_150f:
    *(lpfl) = flSrc;
    return 0x1;
}

int16_t MdCalcStargateDamage(int16_t isbsSrc, int16_t isbsDst, int16_t dDist, int16_t wt, int16_t *ppctDmg) {
    int32_t dBaseDistance;
    PART    partDst;
    PART    partSrc;
    int32_t pctSurviveT;
    int32_t pctSurvive;

L_152e:
    pctSurvive = 10000;
    partDst.hs.grhst = hstSpecialSB;
    partSrc.hs.grhst = hstSpecialSB;
    partSrc.hs.iItem = isbsSrc;
    partDst.hs.iItem = isbsDst;
    FLookupPart(&(partSrc));
    FLookupPart(&(partDst));
    dBaseDistance = (uint32_t)(partSrc.pspecialsb->grAbility2);
    if ((LOWORD(dBaseDistance) != 0xffff))
        goto L_15b2;
    else
        goto L_159f;

L_159f:
    if ((HIWORD(dBaseDistance) != 0xffff))
        goto L_15b2;
    else
        goto L_15a8;

L_15a8:
    dBaseDistance = 8000;

L_15b2:
    if ((SIGNHIWORD(dDist) < HIWORD((uint32_t)((dBaseDistance * 5)))))
        goto L_15f1;
    else
        goto L_15dc;

L_15dc:
    /* untranslated: branch ss:[bp-0x20] > hiword((uint32_t)(dBaseDistance * 5)) ? L_15e8 : L_15e1 */

L_15e1:
    /* untranslated: branch ss:[bp-0x22] <= loword((uint32_t)(dBaseDistance * 5)) ? L_15f1 : L_15e8 */

L_15e8:
    return 0xffff;

L_15f1:
    if ((partSrc.pspecialsb->grAbility <= 0))
        goto L_163a;
    else
        goto L_15fe;

L_15fe:
    if ((SIGNHIWORD(wt) > HIWORD((uint32_t)(((uint32_t)(partSrc.pspecialsb->grAbility) * 5)))))
        goto L_1683;
    else
        goto L_162e;

L_162e:
    /* untranslated: branch ss:[bp-0x20] < hiword((uint32_t)(sext16to32(partSrc.pspecialsb->grAbility) * 5)) ? L_163a : L_1633 */

L_1633:
    /* untranslated: branch ss:[bp-0x22] > loword((uint32_t)(sext16to32(partSrc.pspecialsb->grAbility) * 5)) ? L_1683 : L_163a */

L_163a:
    if ((partDst.pspecialsb->grAbility <= 0))
        goto L_1689;
    else
        goto L_1647;

L_1647:
    if ((SIGNHIWORD(wt) < HIWORD((uint32_t)(((uint32_t)(partDst.pspecialsb->grAbility) * 5)))))
        goto L_1689;
    else
        goto L_1677;

L_1677:
    /* untranslated: branch ss:[bp-0x20] > hiword((uint32_t)(sext16to32(partDst.pspecialsb->grAbility) * 5)) ? L_1683 : L_167c */

L_167c:
    /* untranslated: branch ss:[bp-0x22] <= loword((uint32_t)(sext16to32(partDst.pspecialsb->grAbility) * 5)) ? L_1689 : L_1683 */

L_1683:
    return 0xfffe;

L_1689:
    if ((SIGNHIWORD(dDist) < HIWORD(dBaseDistance)))
        goto L_170f;
    else
        goto L_1695;

L_1695:
    if ((SIGNHIWORD(dDist) > HIWORD(dBaseDistance)))
        goto L_16a2;
    else
        goto L_169a;

L_169a:
    if ((dDist <= LOWORD(dBaseDistance)))
        goto L_170f;
    else
        goto L_16a2;

L_16a2:
    /* untranslated: pctSurvive = (int32_t)((uint32_t)(words((hiword((uint32_t)(dBaseDistance * 0x5)) - signhiword(dDist)), (loword((uint32_t)(dBaseDistance *
     * 0x5)) - dDist)) * 0x9c4) / dBaseDistance) */
    if ((HIWORD(pctSurvive) > 0x0))
        goto L_170f;
    else
        goto L_16f4;

L_16f4:
    if ((HIWORD(pctSurvive) < 0x0))
        goto TotalDeath;
    else
        goto L_16f9;

L_16f9:
    if ((LOWORD(pctSurvive) > 0x0))
        goto L_170f;
    else
        goto TotalDeath;

TotalDeath:
    *(ppctDmg) = 100;
    return 0x0;

L_170f:
    if ((wt <= partSrc.pspecialsb->grAbility))
        goto L_17be;
    else
        goto L_171e;

L_171e:
    if ((partSrc.pspecialsb->grAbility <= 0))
        goto L_17be;
    else
        goto L_172b;

L_172b:
    /* untranslated: pctSurviveT = (int32_t)((uint32_t)(words((hiword((uint32_t)(sext16to32(partSrc.pspecialsb->grAbility) * 0x5)) - signhiword(wt)),
     * (loword((uint32_t)(sext16to32(partSrc.pspecialsb->grAbility) * 0x5)) - wt)) * 0x9c4) / sext16to32(partSrc.pspecialsb->grAbility)) */
    if ((HIWORD(pctSurviveT) > 0x0))
        goto L_1798;
    else
        goto L_1787;

L_1787:
    if ((HIWORD(pctSurviveT) < 0x0))
        goto TotalDeath;
    else
        goto L_178c;

L_178c:
    if ((LOWORD(pctSurviveT) <= 0x0))
        goto TotalDeath;
    else
        goto L_1792;

L_1792:

L_1798:
    pctSurvive = (int32_t)(((uint32_t)((pctSurvive * pctSurviveT)) / 0x2710));

L_17be:
    if ((wt <= partDst.pspecialsb->grAbility))
        goto L_186d;
    else
        goto L_17cd;

L_17cd:
    if ((partDst.pspecialsb->grAbility <= 0))
        goto L_186d;
    else
        goto L_17da;

L_17da:
    /* untranslated: pctSurviveT = (int32_t)((uint32_t)(words((hiword((uint32_t)(sext16to32(partDst.pspecialsb->grAbility) * 0x5)) - signhiword(wt)),
     * (loword((uint32_t)(sext16to32(partDst.pspecialsb->grAbility) * 0x5)) - wt)) * 0x9c4) / sext16to32(partDst.pspecialsb->grAbility)) */
    if ((HIWORD(pctSurviveT) > 0x0))
        goto L_1847;
    else
        goto L_1836;

L_1836:
    if ((HIWORD(pctSurviveT) < 0x0))
        goto TotalDeath;
    else
        goto L_183b;

L_183b:
    if ((LOWORD(pctSurviveT) <= 0x0))
        goto TotalDeath;
    else
        goto L_1841;

L_1841:

L_1847:
    pctSurvive = (int32_t)(((uint32_t)((pctSurvive * pctSurviveT)) / 0x2710));

L_186d:
    /* untranslated: *ppctDmg = loword((int32_t)(words((0x0 - HIWORD(pctSurvive)), (0x2710 - LOWORD(pctSurvive))) / 0x64)) */
    return 0x1;
}

void KillUsedWaypoints() {
    int16_t j;
    int16_t i;
    FLEET  *lpfl;
    int16_t fRep;
    PLANET *lppl;

L_189a:
    if ((cFleet <= 0))
        goto L_1d2b;
    else
        goto L_18aa;

L_18aa:

L_18b0:
    i = 0;
    goto L_18bc;

L_18b8:
    i = (i + 1);

L_18bc:
    if ((i >= cFleet))
        goto L_1d2b;
    else
        goto L_18c7;

L_18c7:
    lpfl = rglpfl[i];
    if ((LOWORD(rglpfl[i]) != 0x0))
        goto L_18f7;
    else
        goto L_18ef;

L_18ef:
    if ((*(rglpfl[i] + 0x2) == 0x0))
        goto L_1d2b;
    else
        goto L_18f7;

L_18f7:
    if ((lpfl->cord <= 1))
        goto L_18b8;
    else
        goto L_1904;

L_1904:
    if ((lpfl->fMark == 0x0))
        goto L_196f;
    else
        goto L_191b;

L_191b:
    DeleteWpFar(lpfl, 0x1, 0x0);
    FSendPlrMsg(lpfl->iPlayer, 0x137, (lpfl->id | 0x8000), lpfl->id, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto L_18b8;

L_196f:
    j = 1;
    goto L_197b;

L_1977:
    j = (j + 1);

L_197b:
    if ((j >= lpfl->cord))
        goto L_1ae7;
    else
        goto L_198a;

L_198a:
    if ((lpfl->lpplord->rgord[j].grobj != 0x2))
        goto L_1977;
    else
        goto L_19b8;

L_19b8:
    lpfl->lpflNext = LpflFromId(lpfl->lpplord->rgord[j].id);
    if ((LOWORD(lpfl->lpflNext) != 0x0))
        goto L_1a00;
    else
        goto L_19f6;

L_19f6:
    if ((HIWORD(lpfl->lpflNext) == 0x0))
        goto L_1a7c;
    else
        goto L_1a00;

L_1a00:
    if ((lpfl->lpplord->rgord[j].fNoAutoTrack != 0x0))
        goto L_1a6a;
    else
        goto L_1a2e;

L_1a2e:
    /* untranslated: ss:[bp-0xe] = lpfl->lpflNext->pt.y */
    lpfl->lpplord->rgord[j].pt.x = lpfl->lpflNext->pt.x;
    /* untranslated: lpfl->lpplord->rgord[j].pt.y = ss:[bp-0xe] */

L_1a6a:
    lpfl->lpflNext = 0x0;
    goto L_1977;

L_1a7c:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[j]) = ((part[6:2](lpfl->lpplord->rgord[j]) & 0xf0ff) | 0x400) */
    lpfl->lpplord->rgord[j].id = 0;

L_1ae7:
    if ((lpfl->pt.x != lpfl->lpplord->rgord[1].pt.x))
        goto L_18b8;
    else
        goto L_1b02;

L_1b02:
    if ((lpfl->pt.y != lpfl->lpplord->rgord[1].pt.y))
        goto L_18b8;
    else
        goto L_1b0b;

L_1b0b:
    lpfl->lpplord->rgord[0] = lpfl->lpplord->rgord[1];
    if ((lpfl->lpplord->rgord[0].grobj != 0x2))
        goto NoOrdFixupYet;
    else
        goto L_1b65;

L_1b65:
    if ((lpfl->lpplord->rgord[0].grTask == 0x1))
        goto NoOrdFixupYet;
    else
        goto L_1b78;

L_1b78:

L_1b7e:
    if ((lpfl->lpplord->rgord[0].grTask == 0x4))
        goto NoOrdFixupYet;
    else
        goto L_1b94;

L_1b94:
    if ((lpfl->idPlanet != -1))
        goto L_1bcd;
    else
        goto L_1ba1;

L_1ba1:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf0ff) | 0x400) */
    lpfl->lpplord->rgord[0].id = 0;
    goto NoOrdFixupYet;

L_1bcd:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x0]) = ((part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf0ff) | 0x100) */
    lpfl->lpplord->rgord[0].id = lpfl->idPlanet;

NoOrdFixupYet:
    if ((lpfl->lpplord->rgord[1].grTask != 0x7))
        goto L_1c2c;
    else
        goto L_1c11;

L_1c11:
    if ((lpfl->lpplord->rgord[1].grobj == 0x2))
        goto L_1c41;
    else
        goto L_1c2c;

L_1c2c:
    fRep = lpfl->fRepOrders;
    goto L_1c46;

L_1c41:
    fRep = 0;

L_1c46:
    DeleteWpFar(lpfl, 0x1, fRep);
    if ((lpfl->cord != 1))
        goto L_18b8;
    else
        goto L_1c68;

L_1c68:
    goto L_1d04;
    goto L_1d28;
    if ((lpfl->idPlanet == -1))
        goto L_1cc6;
    else
        goto L_1c89;

L_1c89:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != lpfl->iPlayer))
        goto L_1cc6;
    else
        goto L_1cb1;

L_1cb1:
    if ((lppl->idRoute != 0x0))
        goto L_18b8;
    else
        goto L_1cc0;

L_1cc0:

L_1cc6:
    FSendPlrMsg(lpfl->iPlayer, 0x4e, (lpfl->id | 0x8000), lpfl->id, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
    goto L_18b8;

L_1d04:

L_1d28:

L_1d2b:
    return;
}

void NoAutoTrackFleet(FLEET *lpflTarget) {
    int16_t iplr;
    int16_t idTarget;
    int16_t i;
    ORDER  *lpord;
    int16_t ifl;
    FLEET  *lpfl;

L_1d32:
    iplr = lpflTarget->iPlayer;
    idTarget = lpflTarget->id;
    ifl = 0;
    goto L_1d5a;

L_1d56:
    ifl = (ifl + 1);

L_1d5a:
    if ((ifl >= cFleet))
        goto L_1e4c;
    else
        goto L_1d65;

L_1d65:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_1d95;
    else
        goto L_1d8d;

L_1d8d:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1e4c;
    else
        goto L_1d95;

L_1d95:
    if ((lpfl->iPlayer == iplr))
        goto L_1d56;
    else
        goto L_1da4;

L_1da4:
    if ((lpfl->cord <= 1))
        goto L_1d56;
    else
        goto L_1dae;

L_1dae:

L_1db4:
    lpord = &(lpfl->lpplord->rgord[0x1]);
    i = 1;
    goto L_1de5;

L_1dd2:
    i = (i + 1);
    lpord = (lpord + 0x1);

L_1de5:
    if ((i >= lpfl->cord))
        goto L_1d56;
    else
        goto L_1df4;

L_1df4:
    if ((lpord->grobj != grobjFleet))
        goto L_1dd2;
    else
        goto L_1e0b;

L_1e0b:
    if ((lpord->id != idTarget))
        goto L_1dd2;
    else
        goto L_1e17;

L_1e17:

L_1e1d:
    lpord->fNoAutoTrack = 0x1;
    lpord->pt.x = lpflTarget->pt.x;
    lpord->pt.y = lpflTarget->pt.y;
    goto L_1dd2;

L_1e4c:
    return;
}

void AutoRouteFleet(FLEET *lpfl, PLANET *lppl) {
    int32_t dTravel;
    int16_t iWarp;
    int16_t pctDmg;
    int16_t wt;
    int32_t cTurns;
    int16_t i;
    ORDER  *lpord;
    PLANET *lpplRoute;
    int16_t isbsDst;
    int16_t wtBig;
    int16_t ishdef;
    int16_t ishdefBig;
    int16_t isbsSrc;
    int32_t t_call_213c;
    int32_t t_call_222b;

L_1e52:
    lpfl->cord = 2;
    lpfl->lpplord->iordMac = 0x2;
    lpord = &(lpfl->lpplord->rgord[0x1]);
    lpord->grTask = grTaskAutoRoute;
    lpord->grobj = grobjPlanet;
    lpord->id = (lppl->idRoute - 1);
    lpplRoute = LpplFromId((lppl->idRoute + 0xffff));
    lpord->pt.x = rgptPlan[(lppl->idRoute - 0x1)].x;
    lpord->pt.y = rgptPlan[(lppl->idRoute - 0x1)].y;
    lpord->fValidTask = 0x1;
    iWarp = IFindIdealWarp(lpfl, 0x0);
    DGetDistance(lpfl->pt.x, lpfl->pt.y, lpord->pt.x, lpord->pt.y);
    dTravel = __ftol();
    if ((lppl->iPlayer != lpplRoute->iPlayer))
        goto L_218b;
    else
        goto L_1f79;

L_1f79:
    if ((lppl->fStarbase == 0x0))
        goto L_218b;
    else
        goto L_1f90;

L_1f90:
    if ((lpplRoute->fStarbase == 0x0))
        goto L_218b;
    else
        goto L_1fa7;

L_1fa7:
    isbsDst = IStargateFromLppl(lpplRoute);
    isbsSrc = IStargateFromLppl(lppl);
    i = 0;
    goto L_2001;

L_1fd1:
    if ((LOWORD(lpfl->rgwtMin[i]) != 0x0))
        goto L_200a;
    else
        goto L_1ff0;

L_1ff0:
    if ((HIWORD(lpfl->rgwtMin[i]) != 0x0))
        goto L_200a;
    else
        goto L_1ff7;

L_1ff7:

L_1ffd:
    i = (i + 1);

L_2001:
    if ((i < 4))
        goto L_1fd1;
    else
        goto L_200a;

L_200a:
    if ((i != 4))
        goto L_20d3;
    else
        goto L_2013;

L_2013:
    if ((isbsDst == -1))
        goto L_20d3;
    else
        goto L_201c;

L_201c:
    if ((isbsSrc == -1))
        goto L_20d3;
    else
        goto L_2025;

L_2025:
    wt = 0;
    wtBig = 0;
    ishdefBig = -1;
    ishdef = 0;
    goto L_2098;

L_203c:
    if ((lpfl->rgcsh[ishdef] == 0))
        goto L_2094;
    else
        goto L_2059;

L_2059:
    wt = rglpshdef[lpfl->iPlayer][ishdef].hul.wtEmpty;
    if ((wt <= wtBig))
        goto L_2094;
    else
        goto L_2088;

L_2088:
    wtBig = wt;
    ishdefBig = ishdef;

L_2094:
    ishdef = (ishdef + 1);

L_2098:
    if ((ishdef < 16))
        goto L_203c;
    else
        goto L_20a1;

L_20a1:
    if ((MdCalcStargateDamage(isbsSrc, isbsDst, LOWORD(dTravel), wtBig, &(pctDmg)) != 1))
        goto L_20d3;
    else
        goto L_20c5;

L_20c5:
    if ((pctDmg != 0))
        goto L_20d3;
    else
        goto L_20ce;

L_20ce:
    iWarp = 11;

L_20d3:
    if ((iWarp >= 9))
        goto L_218b;
    else
        goto L_20dc;

L_20dc:
    if ((LphuldefFromId(rglpshdefSB[lpfl->iPlayer][lpplRoute->isb].hul.ihuldef)->hul.wtCargoMax == 0x0))
        goto L_218b;
    else
        goto L_211b;

L_211b:
    iWarp = 9;
    goto L_2160;

L_2123:
    t_call_213c = EstFuelUse(lpfl, 0x0, iWarp, 0xffffffff, 0x1);
    if ((HIWORD(t_call_213c) < HIWORD(dTravel)))
        goto L_215c;
    else
        goto L_214c;

L_214c:
    if ((HIWORD(t_call_213c) > HIWORD(dTravel)))
        goto L_2169;
    else
        goto L_2151;

L_2151:
    if ((LOWORD(t_call_213c) >= LOWORD(dTravel)))
        goto L_2169;
    else
        goto L_2156;

L_2156:

L_215c:
    iWarp = (iWarp - 1);

L_2160:
    if ((iWarp > 0))
        goto L_2123;
    else
        goto L_2169;

L_2169:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((iWarp & 0xf) << 0x4));

L_218b:
    if ((iWarp >= 11))
        goto L_2254;
    else
        goto L_2194;

L_2194:
    if ((iWarp == 0))
        goto L_2254;
    else
        goto L_219d;

L_219d:
    cTurns = (int32_t)(((int32_t)((dTravel / (uint32_t)(iWarp))) / (uint32_t)(iWarp)));
    goto L_2202;

L_21c4:
    iWarp = (iWarp - 1);
    if ((HIWORD((int32_t)(((int32_t)((dTravel / (uint32_t)(iWarp))) / (uint32_t)(iWarp)))) < HIWORD(cTurns)))
        goto L_2202;
    else
        goto L_21ee;

L_21ee:
    if ((HIWORD((int32_t)(((int32_t)((dTravel / (uint32_t)(iWarp))) / (uint32_t)(iWarp)))) > HIWORD(cTurns)))
        goto L_21fb;
    else
        goto L_21f3;

L_21f3:
    if ((LOWORD((int32_t)(((int32_t)((dTravel / (uint32_t)(iWarp))) / (uint32_t)(iWarp)))) <= LOWORD(cTurns)))
        goto L_2202;
    else
        goto L_21fb;

L_21fb:
    iWarp = (iWarp + 1);
    goto L_220b;

L_2202:
    if ((iWarp > 2))
        goto L_21c4;
    else
        goto L_220b;

L_220b:
    if ((iWarp == 0))
        goto L_2254;
    else
        goto L_2214;

L_2214:
    t_call_222b = EstFuelUse(lpfl, 0x0, iWarp, dTravel, 0x0);
    if ((HIWORD(t_call_222b) < HIWORD(lpfl->rgwtMin[0x4])))
        goto L_2254;
    else
        goto L_223f;

L_223f:
    if ((HIWORD(t_call_222b) > HIWORD(lpfl->rgwtMin[0x4])))
        goto L_224d;
    else
        goto L_2244;

L_2244:
    if ((LOWORD(t_call_222b) <= LOWORD(lpfl->rgwtMin[0x4])))
        goto L_2254;
    else
        goto L_224d;

L_224d:
    iWarp = (iWarp - 1);
    goto L_220b;

L_2254:
    *(lpord + 0x6) = ((*(lpord + 0x6) & 0xff0f) | ((iWarp & 0xf) << 0x4));
    return;
}

int16_t FColonizer(FLEET *lpfl) {
    int16_t i;
    int32_t l;

L_227c:
    i = 0;
    goto L_230d;

L_228d:
    if ((lpfl->rgcsh[i] == 0))
        goto L_2309;
    else
        goto L_22aa;

L_22aa:
    l = (int32_t)((0x10000 << rglpshdef[lpfl->iPlayer][i].hul.ihuldef));
    if (((LOWORD(l) & 0xc000) != 0x0))
        goto L_2303;
    else
        goto L_22fb;

L_22fb:
    if ((0x0 == 0x0))
        goto L_2309;
    else
        goto L_2303;

L_2303:
    return 0x1;

L_2309:
    i = (i + 1);

L_230d:
    if ((i < 16))
        goto L_228d;
    else
        goto L_2316;

L_2316:
    return 0x0;
}

int16_t FScout(FLEET *lpfl) {
    int16_t i;
    int32_t l;

L_2322:
    i = 0;
    goto L_23b3;

L_2333:
    if ((lpfl->rgcsh[i] == 0))
        goto L_23af;
    else
        goto L_2350;

L_2350:
    l = (int32_t)((0x10000 << rglpshdef[lpfl->iPlayer][i].hul.ihuldef));
    if (((LOWORD(l) & 0x70) != 0x0))
        goto L_23a9;
    else
        goto L_23a1;

L_23a1:
    if ((0x0 == 0x0))
        goto L_23af;
    else
        goto L_23a9;

L_23a9:
    return 0x1;

L_23af:
    i = (i + 1);

L_23b3:
    if ((i < 16))
        goto L_2333;
    else
        goto L_23bc;

L_23bc:
    return 0x0;
}

void AutoFleetOrder(FLEET *lpfl, PLANET *lppl) {
    int32_t cMine;
    int16_t ifl;
    ORDER  *lpord;
    FLEET  *lpflT;
    int16_t fFoundFleet;
    int32_t t_call_2436;

L_23c8:
    fFoundFleet = 0;
    lpord = lpfl->lpplord->rgord;
    if ((lppl->iPlayer == -1))
        goto L_2430;
    else
        goto L_23f9;

L_23f9:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_25c4;
    else
        goto L_241d;

L_241d:
    if ((lppl->iPlayer != lpfl->iPlayer))
        goto L_25c4;
    else
        goto L_2430;

L_2430:
    t_call_2436 = CMineFromLpfl(lpfl);
    if ((LOWORD(t_call_2436) != 0x0))
        goto L_244e;
    else
        goto L_2446;

L_2446:
    if ((HIWORD(t_call_2436) == 0x0))
        goto L_25c4;
    else
        goto L_244e;

L_244e:
    if ((lppl->iPlayer != -1))
        goto L_256f;
    else
        goto L_245b;

L_245b:
    ifl = 0;
    goto L_2467;

L_2463:
    ifl = (ifl + 1);

L_2467:
    if ((ifl >= cFleet))
        goto L_256f;
    else
        goto L_2472;

L_2472:
    lpflT = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_24a2;
    else
        goto L_249a;

L_249a:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_256f;
    else
        goto L_24a2;

L_24a2:
    if ((lpflT->iPlayer < lpfl->iPlayer))
        goto L_2463;
    else
        goto L_24b5;

L_24b5:
    if ((lpflT->fDead != 0x0))
        goto L_2463;
    else
        goto L_24cc;

L_24cc:
    if ((LOWORD(lpfl) != LOWORD(lpflT)))
        goto L_24e5;
    else
        goto L_24da;

L_24da:
    if ((HIWORD(lpfl) == HIWORD(lpflT)))
        goto L_2463;
    else
        goto L_24df;

L_24df:

L_24e5:
    if ((lpflT->iPlayer > lpfl->iPlayer))
        goto L_256f;
    else
        goto L_24f5;

L_24f5:

L_24fb:
    if ((lpflT->pt.x != lpfl->pt.x))
        goto L_2463;
    else
        goto L_250e;

L_250e:
    if ((lpflT->pt.y != lpfl->pt.y))
        goto L_2463;
    else
        goto L_2521;

L_2521:
    cMine = CMineFromLpfl(lpflT);
    if ((HIWORD(cMine) < 0x0))
        goto L_2463;
    else
        goto L_253e;

L_253e:
    if ((HIWORD(cMine) > 0x0))
        goto L_254c;
    else
        goto L_2543;

L_2543:
    if ((LOWORD(cMine) <= 0x0))
        goto L_2463;
    else
        goto L_254c;

L_254c:
    if ((HIWORD(cMine) > 0x0))
        goto L_2463;
    else
        goto L_2555;

L_2555:
    if ((HIWORD(cMine) < 0x0))
        goto L_2564;
    else
        goto L_255a;

L_255a:
    if ((LOWORD(cMine) >= 0xfa0))
        goto L_2463;
    else
        goto L_2564;

L_2564:
    fFoundFleet = 1;
    goto L_256f;

L_256f:
    if ((fFoundFleet == 0))
        goto L_25b0;
    else
        goto L_2578;

L_2578:
    lpord->grTask = grTaskMerge;
    lpord->grobj = grobjFleet;
    lpord->id = lpflT->id;
    goto L_25c4;

L_25b0:
    lpord->grTask = grTaskMine;

L_25c4:
    lpfl->iplan = 0x0;
    return;
}

int32_t CMineFromLpfl(FLEET *lpfl) {
    int32_t cMine;
    int16_t j;
    int16_t i;
    HUL    *lphuldef;
    PART    part;
    int32_t cMineTot;
    int16_t chs;
    HS     *lphs;

L_25d2:
    cMineTot = 0;
    i = 0;
    goto L_2723;

L_25ed:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_271f;
    else
        goto L_260a;

L_260a:
    lphuldef = &(rglpshdef[lpfl->iPlayer][i]);
    chs = lphuldef->chs;
    cMine = 0;
    j = 0;
    lphs = lphuldef->rghs;
    goto L_2670;

L_265d:
    j = (j + 1);
    lphs = (lphs + 0x1);

L_2670:
    if ((j >= chs))
        goto L_26f4;
    else
        goto L_267b;

L_267b:
    if ((lphs->grhst != hstMining))
        goto L_265d;
    else
        goto L_2688;

L_2688:
    if ((lphs->iItem < iminingRoboMidgetMiner))
        goto L_265d;
    else
        goto L_269a;

L_269a:
    if ((lphs->iItem > iminingAlienMiner))
        goto L_265d;
    else
        goto L_26ac;

L_26ac:
    part.hs.grhst = lphs->grhst;
    HIWORD(part.hs) = *(lphs + 0x2);
    FLookupPart(&(part));
    /* untranslated: cMine = (cMine + (uint32_t)(words(0x0, lphs->cItem) * sext16to32(part[52:2](part.pcom)))) */

L_26f4:
    cMineTot = (cMineTot + (uint32_t)((cMine * (uint32_t)(lpfl->rgcsh[i]))));

L_271f:
    i = (i + 1);

L_2723:
    if ((i < 16))
        goto L_25ed;
    else
        goto L_272c;

L_272c:
    if ((HIWORD(cMineTot) > 0x0))
        goto L_274d;
    else
        goto L_2735;

L_2735:
    if ((HIWORD(cMineTot) < 0x0))
        goto L_2744;
    else
        goto L_273a;

L_273a:
    if ((LOWORD(cMineTot) >= 0xfa0))
        goto L_274d;
    else
        goto L_2744;

L_2744:
    return cMineTot;

L_274d:
    return 0xfa00000;
}

int32_t PctTerraFromLpfl(FLEET *lpfl) {
    int16_t j;
    int32_t pctTot;
    int16_t i;
    int32_t pct;
    HUL    *lphuldef;
    int16_t chs;
    HS     *lphs;

L_275c:
    pctTot = 0;
    i = 0;
    goto L_286e;

L_2777:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_286a;
    else
        goto L_2794;

L_2794:
    lphuldef = &(rglpshdef[lpfl->iPlayer][i]);
    chs = lphuldef->chs;
    pct = 0;
    j = 0;
    lphs = lphuldef->rghs;
    goto L_27fa;

L_27e7:
    j = (j + 1);
    lphs = (lphs + 0x1);

L_27fa:
    if ((j >= chs))
        goto L_283f;
    else
        goto L_2805;

L_2805:
    if ((lphs->grhst != hstMining))
        goto L_27e7;
    else
        goto L_2812;

L_2812:
    if ((lphs->iItem != iminingOrbitalAdjuster))
        goto L_27e7;
    else
        goto L_2824;

L_2824:
    LOWORD(pct) = (LOWORD(pct) + lphs->cItem);
    HIWORD(pct) = (HIWORD(pct) + 0x0);

L_283f:
    pctTot = (pctTot + (uint32_t)((pct * (uint32_t)(lpfl->rgcsh[i]))));

L_286a:
    i = (i + 1);

L_286e:
    if ((i < 16))
        goto L_2777;
    else
        goto L_2877;

L_2877:

L_2880:
    return pctTot;
}

int32_t CLayMinesFromLpfl(FLEET *lpfl, int16_t iType, int16_t ishdef) {
    uint16_t iMin;
    uint16_t iMax;
    int32_t  cMine;
    int16_t  j;
    int16_t  i;
    HUL     *lphul;
    PART     part;
    int32_t  cMineTot;
    int16_t  chs;
    HS      *lphs;

L_2886:
    cMineTot = 0;
    goto L_28d6;

L_289f:
    iMin = 0x0;
    iMax = 0x9;
    goto L_28f9;

L_28ac:
    iMin = 0x0;
    iMax = 0x3;
    goto L_28f9;

L_28b9:
    iMin = 0x4;
    iMax = 0x6;
    goto L_28f9;

L_28c6:
    iMin = 0x7;
    iMax = 0x9;
    goto L_28f9;

L_28d6:
    if ((iType == -1))
        goto L_289f;
    else
        goto L_28de;

L_28de:
    if ((iType == 0))
        goto L_28ac;
    else
        goto L_28e6;

L_28e6:
    if ((iType == 1))
        goto L_28b9;
    else
        goto L_28ee;

L_28ee:
    if ((iType != 2))
        goto L_289f;
    else
        goto L_28f3;

L_28f3:

L_28f9:
    i = 0;
    goto L_2ac4;

L_2901:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_2ac0;
    else
        goto L_291e;

L_291e:
    if ((ishdef == -1))
        goto L_2935;
    else
        goto L_2927;

L_2927:
    if ((i != ishdef))
        goto L_2ac0;
    else
        goto L_292f;

L_292f:

L_2935:
    lphul = &(rglpshdef[lpfl->iPlayer][i]);
    chs = lphul->chs;
    cMine = 0;
    j = 0;
    lphs = lphul->rghs;
    goto L_299b;

L_2988:
    j = (j + 1);
    lphs = (lphs + 0x1);

L_299b:
    if ((j >= chs))
        goto L_2a69;
    else
        goto L_29a6;

L_29a6:
    if ((lphs->grhst != hstMines))
        goto L_2a1f;
    else
        goto L_29b3;

L_29b3:
    if ((lphs->iItem < iMin))
        goto L_2a1f;
    else
        goto L_29c5;

L_29c5:
    if ((lphs->iItem > iMax))
        goto L_2a1f;
    else
        goto L_29d7;

L_29d7:
    part.hs.grhst = lphs->grhst;
    HIWORD(part.hs) = *(lphs + 0x2);
    FLookupPart(&(part));
    /* untranslated: cMine = (cMine + (uint32_t)(words(0x0, lphs->cItem) * sext16to32(part[52:2](part.pcom)))) */
    goto L_2988;

L_2a1f:
    if ((iType > 0))
        goto L_2988;
    else
        goto L_2a28;

L_2a28:
    if ((lphs->grhst != hstBeam))
        goto L_2988;
    else
        goto L_2a34;

L_2a34:
    if ((lphs->iItem != ibeamMultiContainedMunition))
        goto L_2988;
    else
        goto L_2a46;

L_2a46:
    /* untranslated: cMine = (cMine + (int32_t)(words(lphs->cItem, 0x0) * 0x4)) */

L_2a69:
    if ((lphul->ihuldef == ihuldefMiniMineLayer))
        goto L_2a81;
    else
        goto L_2a75;

L_2a75:
    if ((lphul->ihuldef != ihuldefSuperMineLayer))
        goto L_2a95;
    else
        goto L_2a81;

L_2a81:
    cMine = (int32_t)((cMine * 2));

L_2a95:
    cMineTot = (cMineTot + (uint32_t)((cMine * (uint32_t)(lpfl->rgcsh[i]))));

L_2ac0:
    i = (i + 1);

L_2ac4:
    if ((i < 16))
        goto L_2901;
    else
        goto L_2acd;

L_2acd:
    if ((HIWORD(cMineTot) > 0x98))
        goto L_2afd;
    else
        goto L_2ad7;

L_2ad7:
    if ((HIWORD(cMineTot) < 0x98))
        goto L_2ae6;
    else
        goto L_2adc;

L_2adc:
    if ((LOWORD(cMineTot) > 0x9680))
        goto L_2afd;
    else
        goto L_2ae6;

L_2ae6:
    if ((HIWORD(cMineTot) > 0x0))
        goto L_2b09;
    else
        goto L_2aef;

L_2aef:
    if ((HIWORD(cMineTot) < 0x0))
        goto L_2afd;
    else
        goto L_2af4;

L_2af4:
    if ((LOWORD(cMineTot) >= 0x0))
        goto L_2b09;
    else
        goto L_2afd;

L_2afd:
    return 0xe10005f5;

L_2b09:
    return (uint32_t)((cMineTot * 10));
}

int32_t CMineSweepFromLpfl(FLEET *lpfl) {
    int32_t lPowTot;
    int16_t i;
    HUL    *lphul;
    int32_t lPow;

L_2b26:
    lPowTot = 0;
    i = 0;
    goto L_2bc2;

L_2b41:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_2bbe;
    else
        goto L_2b5e;

L_2b5e:
    lphul = &(rglpshdef[lpfl->iPlayer][i]);
    lPow = CMineSweepFromLphul(lphul);
    lPowTot = (lPowTot + (uint32_t)((lPow * (uint32_t)(lpfl->rgcsh[i]))));

L_2bbe:
    i = (i + 1);

L_2bc2:
    if ((i < 16))
        goto L_2b41;
    else
        goto L_2bcb;

L_2bcb:
    if ((HIWORD(lPowTot) > 0x0))
        goto L_2beb;
    else
        goto L_2bd4;

L_2bd4:
    if ((HIWORD(lPowTot) < 0x0))
        goto L_2be2;
    else
        goto L_2bd9;

L_2bd9:
    if ((LOWORD(lPowTot) > 0x0))
        goto L_2beb;
    else
        goto L_2be2;

L_2be2:
    return 0x0;

L_2beb:
    return lPowTot;
}

int32_t CMineSweepFromLphul(HUL *lphul) {
    int16_t  chs;
    HS      *lphs;
    int32_t  lRange;
    int16_t  j;
    int16_t  fStarbase;
    int32_t  lPow;
    PART     part;
    uint16_t t_merge_2c18_0001;

L_2bfa:
    if ((lphul->ihuldef < ihuldefOrbitalFort))
        goto L_2c15;
    else
        goto L_2c0f;

L_2c0f:
    t_merge_2c18_0001 = 0x1;
    goto L_2c18;

L_2c15:
    t_merge_2c18_0001 = 0x0;

L_2c18:
    fStarbase = t_merge_2c18_0001;
    chs = lphul->chs;
    lPow = 0;
    j = 0;
    lphs = lphul->rghs;
    goto L_2c60;

L_2c4d:
    j = (j + 1);
    lphs = (lphs + 0x1);

L_2c60:
    if ((j >= chs))
        goto L_2d2f;
    else
        goto L_2c6b;

L_2c6b:
    if ((lphs->grhst != hstBeam))
        goto L_2c4d;
    else
        goto L_2c77;

L_2c77:
    part.hs.grhst = lphs->grhst;
    HIWORD(part.hs) = *(lphs + 0x2);
    FLookupPart(&(part));
    /* untranslated: branch (part[58:2](part.pcom) & 0x2) == 0x0 ? L_2cb2 : L_2ca5 */

L_2ca5:
    lRange = 4;
    goto L_2cd8;

L_2cb2:
    if (((part.pbeam->grfAbilities & 0x1) != 0x0))
        goto L_2c4d;
    else
        goto L_2cc1;

L_2cc1:

L_2cca:
    lRange = (uint32_t)(part.pbeam->dRangeMax);

L_2cd8:
    if ((fStarbase == 0))
        goto L_2ce9;
    else
        goto L_2ce1;

L_2ce1:
    lRange = (lRange + 1);

L_2ce9:
    /* untranslated: lPow = (lPow + (uint32_t)((uint32_t)((uint32_t)(lRange * lRange) * words(0x0, lphs->cItem)) * sext16to32(part.pbeam->dp))) */

L_2d2f:
    if ((HIWORD(lPow) > 0x0))
        goto L_2d4f;
    else
        goto L_2d38;

L_2d38:
    if ((HIWORD(lPow) < 0x0))
        goto L_2d46;
    else
        goto L_2d3d;

L_2d3d:
    if ((LOWORD(lPow) > 0x0))
        goto L_2d4f;
    else
        goto L_2d46;

L_2d46:
    return 0x0;

L_2d4f:
    return lPow;
}

int16_t PctCloakFromLpfl(FLEET *lpfl) {
    int16_t  j;
    double   dcPts;
    double   dwtFleet;
    int16_t  i;
    int32_t  cPtsCur;
    int16_t  fUseFloat;
    HUL     *lphul;
    int32_t  wtFleet;
    int16_t  cScore;
    int32_t  cPts;
    int32_t  wtFleetCur;
    int16_t  chs;
    HS      *lphs;
    uint16_t t_merge_315e_0001;

L_2d5e:
    wtFleet = 0;
    cPts = 0;
    dwtFleet = 0;
    dcPts = 0;
    fUseFloat = 0;
    i = 0;
    goto L_2fab;

L_2d9e:
    if ((lpfl->rgcsh[i] <= 0))
        goto L_2fa7;
    else
        goto L_2dbb;

L_2dbb:
    lphul = &(rglpshdef[lpfl->iPlayer][i]);
    chs = lphul->chs;
    /* untranslated: wtFleetCur = (uint32_t)(sext16to32(lpfl->rgcsh[i]) * words(0x0, lphul->wtEmpty)) */
    cPtsCur = 0;
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raStealth))
        goto L_2e51;
    else
        goto L_2e48;

L_2e48:
    cPtsCur = (cPtsCur + 300);

L_2e51:
    j = 0;
    lphs = lphul->rghs;
    goto L_2e7f;

L_2e6c:
    j = (j + 1);
    lphs = (lphs + 0x1);

L_2e7f:
    if ((j >= chs))
        goto L_2ea2;
    else
        goto L_2e8a;

L_2e8a:
    cPtsCur = (cPtsCur + (uint32_t)(CPtsCloakFromLphs(lphs)));
    goto L_2e6c;

L_2ea2:
    if ((HIWORD(cPtsCur) < 0x0))
        goto L_2f7b;
    else
        goto L_2eab;

L_2eab:
    if ((HIWORD(cPtsCur) > 0x0))
        goto L_2eb9;
    else
        goto L_2eb0;

L_2eb0:
    if ((LOWORD(cPtsCur) <= 0x0))
        goto L_2f7b;
    else
        goto L_2eb9;

L_2eb9:
    if ((fUseFloat != 0))
        goto L_2f3d;
    else
        goto L_2ec2;

L_2ec2:
    if ((HIWORD(cPtsCur) > 0x0))
        goto L_2f24;
    else
        goto L_2ecb;

L_2ecb:
    if ((HIWORD(cPtsCur) < 0x0))
        goto L_2eda;
    else
        goto L_2ed0;

L_2ed0:
    if ((LOWORD(cPtsCur) > 0xfa0))
        goto L_2f24;
    else
        goto L_2eda;

L_2eda:
    if ((HIWORD(wtFleetCur) > 0x7))
        goto L_2f24;
    else
        goto L_2ee3;

L_2ee3:
    if ((HIWORD(wtFleetCur) < 0x7))
        goto L_2ef2;
    else
        goto L_2ee8;

L_2ee8:
    if ((LOWORD(wtFleetCur) > 0xa120))
        goto L_2f24;
    else
        goto L_2ef2;

L_2ef2:
    if ((HIWORD(cPts) > 0x5f5))
        goto L_2f24;
    else
        goto L_2efc;

L_2efc:
    if ((HIWORD(cPts) < 0x5f5))
        goto L_2f0b;
    else
        goto L_2f01;

L_2f01:
    if ((LOWORD(cPts) > 0xe100))
        goto L_2f24;
    else
        goto L_2f0b;

L_2f0b:
    if ((HIWORD(wtFleet) < 0x2fa))
        goto L_2f3d;
    else
        goto L_2f15;

L_2f15:
    if ((HIWORD(wtFleet) > 0x2fa))
        goto L_2f24;
    else
        goto L_2f1a;

L_2f1a:
    if ((LOWORD(wtFleet) <= 0xf080))
        goto L_2f3d;
    else
        goto L_2f24;

L_2f24:
    dcPts = (double)(cPts);
    dwtFleet = (double)(wtFleet);
    fUseFloat = 1;

L_2f3d:
    if ((fUseFloat == 0))
        goto L_2f64;
    else
        goto L_2f46;

L_2f46:
    dcPts = (dcPts + ((double)(cPtsCur) * (double)(wtFleetCur)));
    goto L_2f7b;

L_2f64:
    cPts = (cPts + (uint32_t)((cPtsCur * wtFleetCur)));

L_2f7b:
    if ((fUseFloat == 0))
        goto L_2f9b;
    else
        goto L_2f84;

L_2f84:
    dwtFleet = (dwtFleet + (double)(wtFleetCur));
    goto L_2fa7;

L_2f9b:
    wtFleet = (wtFleet + wtFleetCur);

L_2fa7:
    i = (i + 1);

L_2fab:
    if ((i < 16))
        goto L_2d9e;
    else
        goto L_2fb4;

L_2fb4:
    if ((fUseFloat != 0))
        goto L_2fd5;
    else
        goto L_2fbd;

L_2fbd:
    if ((LOWORD(cPts) != 0x0))
        goto L_2fd5;
    else
        goto L_2fc6;

L_2fc6:
    if ((HIWORD(cPts) != 0x0))
        goto L_2fd5;
    else
        goto L_2fcf;

L_2fcf:
    return 0x0;

L_2fd5:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) == raStealth))
        goto L_307c;
    else
        goto L_2ff9;

L_2ff9:
    if ((fUseFloat == 0))
        goto L_3044;
    else
        goto L_3002;

L_3002:
    i = 0;
    goto L_3038;

L_300a:
    dwtFleet = (dwtFleet + (double)(lpfl->rgwtMin[i]));
    i = (i + 1);

L_3038:
    if ((i > 3))
        goto L_307c;
    else
        goto L_303e;

L_303e:

L_3044:
    i = 0;
    goto L_3073;

L_304c:
    wtFleet = (wtFleet + lpfl->rgwtMin[i]);
    i = (i + 1);

L_3073:
    if ((i <= 3))
        goto L_304c;
    else
        goto L_307c;

L_307c:
    if ((fUseFloat == 0))
        goto L_309b;
    else
        goto L_3085;

L_3085:
    cPts = __ftol();
    goto L_30b2;

L_309b:
    cPts = (int32_t)((cPts / wtFleet));

L_30b2:
    if ((HIWORD(cPts) > 0x0))
        goto L_30cf;
    else
        goto L_30bb;

L_30bb:
    if ((HIWORD(cPts) < 0x0))
        goto L_30c9;
    else
        goto L_30c0;

L_30c0:
    if ((LOWORD(cPts) >= 0x0))
        goto L_30cf;
    else
        goto L_30c9;

L_30c9:
    return 0x0;

L_30cf:
    cScore = LOWORD(cPts);
    if ((cScore > 100))
        goto L_30e9;
    else
        goto L_30e1;

L_30e1:
    return (cScore >> 0x1);

L_30e9:
    cScore = (cScore - 100);
    if ((cScore > 200))
        goto L_3106;
    else
        goto L_30f7;

L_30f7:
    return ((cScore >> 0x3) + 0x32);

L_3106:
    cScore = (cScore - 200);
    if ((cScore > 312))
        goto L_3124;
    else
        goto L_3115;

L_3115:
    return (((uint32_t)(cScore) / 24) + 0x4b);

L_3124:
    cScore = (cScore - 312);
    if ((cScore > 512))
        goto L_3141;
    else
        goto L_3133;

L_3133:
    return ((cScore >> 0x6) + 0x58);

L_3141:
    if ((cScore >= 1000))
        goto L_3164;
    else
        goto L_314b;

L_314b:
    if ((cScore < 768))
        goto L_315b;
    else
        goto L_3155;

L_3155:
    t_merge_315e_0001 = 0x1;
    goto L_315e;

L_315b:
    t_merge_315e_0001 = 0x0;

L_315e:
    return (t_merge_315e_0001 + 0x60);

L_3164:
    return 0x62;
}

int16_t CPtsCloakFromLphs(HS *lphs) {
    int16_t cPts;
    PART    part;

L_3170:
    cPts = 0;
    if ((lphs->cItem > 0x0))
        goto L_319b;
    else
        goto L_3195;

L_3195:
    return 0x0;

L_319b:
    goto L_32f8;

L_31a4:
    if ((lphs->iItem < ispecialETransportCloaking))
        goto L_333b;
    else
        goto L_31b6;

L_31b6:
    if ((lphs->iItem > ispecialEMultiFunctionPod))
        goto L_333b;
    else
        goto L_31c8;

L_31c8:
    part.hs.grhst = lphs->grhst;
    HIWORD(part.hs) = *(lphs + 0x2);
    FLookupPart(&(part));
    /* untranslated: cPts = part[52:2](part.pcom) */

L_31f1:
    if ((lphs->iItem != ispecialMMultiCargoPod))
        goto L_333b;
    else
        goto L_3203;

L_3203:
    cPts = 20;

L_320b:
    if ((lphs->iItem != iscannerChameleonScanner))
        goto L_333b;
    else
        goto L_321d;

L_321d:
    cPts = 40;

L_3225:
    if ((lphs->iItem != iarmorDepletedNeutronium))
        goto L_323f;
    else
        goto L_3237;

L_3237:
    cPts = 50;
    goto L_333b;

L_323f:
    if ((lphs->iItem != iarmorMegaPolyShell))
        goto L_333b;
    else
        goto L_3251;

L_3251:
    cPts = 40;

L_3259:
    if ((lphs->iItem != ishieldShadowShield))
        goto L_3273;
    else
        goto L_326b;

L_326b:
    cPts = 70;
    goto L_333b;

L_3273:
    if ((lphs->iItem != ishieldLangstonShell))
        goto L_333b;
    else
        goto L_3285;

L_3285:
    cPts = 20;

L_328d:
    if ((lphs->iItem != ibeamMultiContainedMunition))
        goto L_333b;
    else
        goto L_329f;

L_329f:
    cPts = 20;

L_32a7:
    if ((lphs->iItem != iengineEnigmaPulsar))
        goto L_333b;
    else
        goto L_32b9;

L_32b9:
    cPts = 20;

L_32c1:
    if ((lphs->iItem != iminingAlienMiner))
        goto L_32db;
    else
        goto L_32d3;

L_32d3:
    cPts = 60;
    goto L_333b;

L_32db:
    if ((lphs->iItem != iminingOrbitalAdjuster))
        goto L_333b;
    else
        goto L_32ed;

L_32ed:
    cPts = 50;

L_32f8:
    if ((lphs->grhst == hstEngine))
        goto L_32a7;
    else
        goto L_3300;

L_3300:
    if ((lphs->grhst == hstScanner))
        goto L_320b;
    else
        goto L_3308;

L_3308:
    if ((lphs->grhst == hstShield))
        goto L_3259;
    else
        goto L_3310;

L_3310:
    if ((lphs->grhst == hstArmor))
        goto L_3225;
    else
        goto L_3318;

L_3318:
    if ((lphs->grhst == hstBeam))
        goto L_328d;
    else
        goto L_3320;

L_3320:
    if ((lphs->grhst == hstMining))
        goto L_32c1;
    else
        goto L_3328;

L_3328:
    if ((lphs->grhst == hstSpecialE))
        goto L_31a4;
    else
        goto L_3330;

L_3330:
    if ((lphs->grhst == hstSpecialM))
        goto L_31f1;
    else
        goto L_333b;

L_333b:
    if ((lphs->cItem <= 0x1))
        goto L_336a;
    else
        goto L_3352;

L_3352:
    cPts = LOWORD((cPts * lphs->cItem));

L_336a:
    return cPts;
}

int16_t MergeFleetsDlg(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    int16_t  i;
    RECT     rc;
    char     szT[80];
    char    *psz;
    uint16_t t_merge_3487_0001;
    LRESULT  t_call_3557;
    uint16_t t_merge_35e1_0001;
    uint16_t t_merge_365d_0001;

L_3376:
    goto L_3684;

L_3385:
    StickyDlgPos(hwnd, &(ptStickyMergeFleetsDlg), 0x1);
    i = 0;
    goto L_3497;

L_33a0:
    psz = PszGetFleetName(rglpfl[vrgiflMerge[i]]->id);
    strcpy(szT, psz);
    if ((rglpfl[vrgiflMerge[i]]->cord <= 1))
        goto L_3417;
    else
        goto L_3407;

L_3407:
    strcat(szT, " *");

L_3417:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0051), CB_LIMITTEXT, 0x0, words(ss, szT)) -> callresult(LRESULT) */
    GetDlgItem(hwnd, IDC_U16_0x0051);
    if ((vcflMerge == 2))
        goto L_347e;
    else
        goto L_3453;

L_3453:
    if ((rglpfl[vrgiflMerge[i]]->id != sel.fl.id))
        goto L_3484;
    else
        goto L_347e;

L_347e:
    t_merge_3487_0001 = 0x1;
    goto L_3487;

L_3484:
    t_merge_3487_0001 = 0x0;

L_3487:
    /* untranslated: call SendMessage(callresult(HWND), CB_GETCOUNT, t_merge_3487_0001, sext16to32(i)) -> callresult(LRESULT) */
    i = (i + 1);

L_3497:
    if ((i < vcflMerge))
        goto L_33a0;
    else
        goto L_34a2;

L_34a2:
    if ((gd.fTutorial == 0x0))
        goto L_34ba;
    else
        goto L_34b5;

L_34b5:
    AdvanceTutor();

L_34ba:
    return 0x1;

L_34c0:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_34e8:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, IDC_U16_0x0051) ? L_36a7 : L_3507 */

L_3507:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_3520:
    if ((wParam == 0x1))
        goto L_3532;
    else
        goto L_3529;

L_3529:
    if ((wParam != 0x2))
        goto L_35f0;
    else
        goto L_3532;

L_3532:
    i = 0;
    goto L_357f;

L_353a:
    t_call_3557 = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0051), CB_GETLBTEXT, i, 0x0);
    if ((LOWORD(t_call_3557) != 0x0))
        goto L_357b;
    else
        goto L_3564;

L_3564:
    if ((HIWORD(t_call_3557) != 0x0))
        goto L_357b;
    else
        goto L_356c;

L_356c:
    vrgiflMerge[i] = -1;

L_357b:
    i = (i + 1);

L_357f:
    if ((i < vcflMerge))
        goto L_353a;
    else
        goto L_358a;

L_358a:
    if ((wParam != 0x1))
        goto L_35b9;
    else
        goto L_3593;

L_3593:
    if ((gd.fTutorial == 0x0))
        goto L_35b9;
    else
        goto L_35a6;

L_35a6:
    if ((FOKMergeDialog() != 0))
        goto L_35b9;
    else
        goto L_35b3;

L_35b3:
    return 0x1;

L_35b9:
    StickyDlgPos(hwnd, &(ptStickyMergeFleetsDlg), 0x0);
    if ((wParam != 0x1))
        goto L_35de;
    else
        goto L_35d8;

L_35d8:
    t_merge_35e1_0001 = 0x1;
    goto L_35e1;

L_35de:
    t_merge_35e1_0001 = 0x0;

L_35e1:
    EndDialog(hwnd, t_merge_35e1_0001);
    return 0x1;

L_35f0:
    if ((wParam != 0x76))
        goto L_361d;
    else
        goto L_35f9;

L_35f9:
    WinHelp(hwnd, szHelpFile, 0x1, 0x453);
    return 0x1;

L_361d:
    if ((wParam == 0x7f8))
        goto L_3631;
    else
        goto L_3627;

L_3627:
    if ((wParam != 0x7f9))
        goto L_36a7;
    else
        goto L_3631;

L_3631:
    i = 0;
    goto L_366d;

L_3639:
    GetDlgItem(hwnd, IDC_U16_0x0051);
    if ((wParam != 0x7f8))
        goto L_365a;
    else
        goto L_3654;

L_3654:
    t_merge_365d_0001 = 0x1;
    goto L_365d;

L_365a:
    t_merge_365d_0001 = 0x0;

L_365d:
    /* untranslated: call SendMessage(callresult(HWND), CB_GETCOUNT, t_merge_365d_0001, sext16to32(i)) -> callresult(LRESULT) */
    i = (i + 1);

L_366d:
    if ((i < vcflMerge))
        goto L_3639;
    else
        goto L_3678;

L_3678:
    return 0x1;

L_3684:
    if ((msg == WM_ERASEBKGND))
        goto L_34c0;
    else
        goto L_368c;

L_368c:
    if ((msg == WM_CTLCOLOR))
        goto L_34e8;
    else
        goto L_3694;

L_3694:
    if ((msg == WM_INITDIALOG))
        goto L_3385;
    else
        goto L_369c;

L_369c:
    if ((msg == WM_COMMAND))
        goto L_3520;
    else
        goto L_36a7;

L_36a7:
    return 0x0;
}

void MarkTechsSeen(HUL *lphul, int16_t iplr) {
    int16_t iplrSav;
    int16_t iTech;
    int16_t ihs;
    PART    part;
    uint8_t t_merge_3753_0001;
    uint8_t t_merge_3838_0001;

L_36b6:
    iplrSav = idPlayer;
    idPlayer = iplr;
    part.hs.grhst = hstHull;
    part.hs.iItem = lphul->ihuldef;
    FLookupPart(&(part));
    iTech = 0;
    goto L_375e;

L_3701:
    if ((rgTechBattle[iTech] <= (uint16_t)(part.phul->rgTech[iTech])))
        goto L_373d;
    else
        goto L_3730;

L_3730:
    t_merge_3753_0001 = rgTechBattle[iTech];
    goto L_3753;

L_373d:
    t_merge_3753_0001 = (uint16_t)(part.phul->rgTech[iTech]);

L_3753:
    rgTechBattle[iTech] = LOBYTE(t_merge_3753_0001);
    iTech = (iTech + 1);

L_375e:
    if ((iTech < 6))
        goto L_3701;
    else
        goto L_3767;

L_3767:
    ihs = 0;
    goto L_3773;

L_376f:
    ihs = (ihs + 1);

L_3773:
    if ((ihs >= lphul->chs))
        goto L_39cb;
    else
        goto L_3785;

L_3785:
    if ((lphul->rghs[ihs].cItem == 0x0))
        goto L_376f;
    else
        goto L_37af;

L_37af:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    HIWORD(part.hs) = HIWORD(lphul->rghs[ihs]);
    FLookupPart(&(part));
    iTech = 0;
    goto L_3843;

L_37e6:
    if ((rgTechBattle[iTech] <= (uint16_t)(part.pcom->rgTech[iTech])))
        goto L_3822;
    else
        goto L_3815;

L_3815:
    t_merge_3838_0001 = rgTechBattle[iTech];
    goto L_3838;

L_3822:
    t_merge_3838_0001 = (uint16_t)(part.pcom->rgTech[iTech]);

L_3838:
    rgTechBattle[iTech] = LOBYTE(t_merge_3838_0001);
    iTech = (iTech + 1);

L_3843:
    if ((iTech < 6))
        goto L_37e6;
    else
        goto L_384c;

L_384c:
    iTech = -1;
    goto L_3936;

L_3857:
    if ((part.hs.iItem != ispecialMMultiCargoPod))
        goto L_386d;
    else
        goto L_3865;

L_3865:
    iTech = 0;
    goto L_3981;

L_386d:
    if ((part.hs.iItem != ispecialMJumpGate))
        goto L_3981;
    else
        goto L_387b;

L_387b:
    iTech = 11;

L_3883:
    if ((part.hs.iItem != ispecialEMultiFunctionPod))
        goto L_3981;
    else
        goto L_3891;

L_3891:
    iTech = 1;

L_3899:
    if ((part.hs.iItem != ishieldLangstonShell))
        goto L_3981;
    else
        goto L_38a7;

L_38a7:
    iTech = 2;

L_38af:
    if ((part.hs.iItem != iarmorMegaPolyShell))
        goto L_3981;
    else
        goto L_38bd;

L_38bd:
    iTech = 3;

L_38c5:
    if ((part.hs.iItem != iminingAlienMiner))
        goto L_3981;
    else
        goto L_38d3;

L_38d3:
    iTech = 4;

L_38db:
    if ((part.hs.iItem != ibombHushABoom))
        goto L_3981;
    else
        goto L_38e9;

L_38e9:
    iTech = 5;

L_38f1:
    if ((part.hs.iItem != ibeamMultiContainedMunition))
        goto L_3981;
    else
        goto L_38ff;

L_38ff:
    iTech = 7;

L_3907:
    if ((part.hs.iItem != itorpAntiMatterTorpedo))
        goto L_3981;
    else
        goto L_3915;

L_3915:
    iTech = 6;

L_391d:
    if ((part.hs.iItem != iengineEnigmaPulsar))
        goto L_3981;
    else
        goto L_392b;

L_392b:
    iTech = 9;

L_3936:
    if ((part.hs.grhst == hstEngine))
        goto L_391d;
    else
        goto L_393e;

L_393e:
    if ((part.hs.grhst == hstShield))
        goto L_3899;
    else
        goto L_3946;

L_3946:
    if ((part.hs.grhst == hstArmor))
        goto L_38af;
    else
        goto L_394e;

L_394e:
    if ((part.hs.grhst == hstBeam))
        goto L_38f1;
    else
        goto L_3956;

L_3956:
    if ((part.hs.grhst == hstTorp))
        goto L_3907;
    else
        goto L_395e;

L_395e:
    if ((part.hs.grhst == hstBomb))
        goto L_38db;
    else
        goto L_3966;

L_3966:
    if ((part.hs.grhst == hstMining))
        goto L_38c5;
    else
        goto L_396e;

L_396e:
    if ((part.hs.grhst == hstSpecialE))
        goto L_3883;
    else
        goto L_3976;

L_3976:
    if ((part.hs.grhst == hstSpecialM))
        goto L_3857;
    else
        goto L_3981;

L_3981:
    if ((iTech == -1))
        goto L_376f;
    else
        goto L_398a;

L_398a:
    if ((rgTechTrader[iTech] >= 0x19))
        goto L_376f;
    else
        goto L_399c;

L_399c:
    rgTechTrader[iTech] = (rgTechTrader[iTech] + LOBYTE(part.hs.cItem));
    if ((rgTechTrader[iTech] <= 0x19))
        goto L_376f;
    else
        goto L_39c0;

L_39c0:
    rgTechTrader[iTech] = 0x19;

L_39cb:
    idPlayer = iplrSav;
    return;
}
