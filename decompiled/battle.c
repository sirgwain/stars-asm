#include "common.h"

uint8_t rgbrcStart[136] = {68,  65,  88,  20,  136, 129, 17,  136, 129, 24,  20,  134, 65,  72,  130, 65,  88,  130, 23,  134, 19,  17,  81,
                           130, 134, 104, 40,  21,  49,  97,  131, 134, 104, 56,  22,  19,  49,  104, 131, 22,  97,  56,  134, 19,  68,  18,
                           21,  24,  65,  72,  84,  113, 120, 131, 134, 49,  104, 131, 22,  97,  56,  134, 19,  67,  54,  102, 65,  88,  130,
                           23,  134, 19,  97,  56,  33,  132, 21,  120, 17,  49,  81,  113, 19,  21,  23,  56,  88,  131, 133, 135, 68,  17,
                           49,  81,  113, 130, 132, 134, 136, 104, 72,  40,  23,  21,  19,  17,  49,  81,  113, 130, 132, 134, 136, 104, 72,
                           40,  23,  21,  19,  68,  17,  49,  81,  113, 130, 132, 134, 136, 104, 72,  40,  23,  21,  19,  51,  102};

int16_t RelationsDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rcGBox;
    int16_t     mdSBase;
    uint16_t    t_merge_00dc_0001;

L_0088:
    goto L_048d;

L_0097:
    StickyDlgPos(hwnd, &(ptStickyRelationsDlg), 0x1);
    if ((idPlayer != 0x0))
        goto L_00d9;
    else
        goto L_00d3;

L_00d3:
    t_merge_00dc_0001 = 0x1;
    goto L_00dc;

L_00d9:
    t_merge_00dc_0001 = 0x0;

L_00dc:
    /* untranslated: call CheckRadioButton(hwnd, 0x7d4, 0x7d6, (sext8to16(byte ds:[(ss:[bp-0xe] + t_merge_00dc_0001)]) + 0x7d4)) -> callresult(void) */
    i = 0x0;
    goto L_00f9;

L_00f5:
    i = (i + 0x1);

L_00f9:
    if ((i >= game.cPlayer))
        goto L_014f;
    else
        goto L_0104;

L_0104:
    if ((i == idPlayer))
        goto L_00f5;
    else
        goto L_010f;

L_010f:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), CB_LIMITTEXT, 0x0, words(ds, PszPlayerName(i, 0x0, 0x0, 0x0, 0x0, 0x0))) ->
     * callresult(LRESULT) */

L_014f:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), CB_GETCURSEL, 0x0, 0x0);
    fDirtyPlan = 0x0;

L_0177:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_019f:
    hdc = BeginPaint(hwnd, &(ps));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x07D5), &(rcGBox));
    ScreenToClient(hwnd, &(rcGBox));
    GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x07D6), &(rc));
    ScreenToClient(hwnd, &(rc.right));
    rcGBox.right = rc.right;
    rcGBox.bottom = rc.bottom;
    ExpandRc(&(rcGBox), dyArial8, (dyArial8 >> 0x1));
    _Draw3dFrame(hdc, &(rcGBox), 0xffff);
    SetBkColor(hdc, crButtonFace);
    SelectObject(hdc, rghfontArial8[0x1]);
    i = CchGetString(idsRelation, szWork);
    TextOut(hdc, (rcGBox.left + 0x8), (rcGBox.top - (dyArial8 >> 0x1)), szWork, i);
    SelectObject(hdc, rghfontArial8[0x0]);
    EndPaint(hwnd, &(ps));
    return 0x1;

L_02aa:
    /* untranslated: ss:[bp-0xe] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0xe] == GetDlgItem(hwnd, IDC_U16_0x07D3) ? L_04c0 : L_02c9 */

L_02c9:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_02e2:
    if ((wParam != 0x2))
        goto L_034a;
    else
        goto L_02eb;

L_02eb:
    StickyDlgPos(hwnd, &(ptStickyRelationsDlg), 0x0);
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((i < idPlayer))
        goto L_0332;
    else
        goto L_032e;

L_032e:
    i = (i + 0x1);

L_0332:
    EndDialog(hwnd, (i + 0x3));
    return 0x1;

L_034a:
    if ((wParam < 0x7d4))
        goto L_03bf;
    else
        goto L_0354;

L_0354:
    if ((wParam > 0x7d6))
        goto L_03bf;
    else
        goto L_035e;

L_035e:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((i < idPlayer))
        goto L_0392;
    else
        goto L_038e;

L_038e:
    i = (i + 0x1);

L_0392:
    rgplr[idPlayer].rgmdRelation[i] = LOBYTE((wParam + 0xf82c));
    fDirtyPlan = 0x1;
    goto L_04c0;

L_03bf:
    if ((wParam != 0x7d3))
        goto L_042d;
    else
        goto L_03c9;

L_03c9:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x07D3), CB_GETLBTEXTLEN, 0x0, 0x0));
    if ((i < idPlayer))
        goto L_03fd;
    else
        goto L_03f9;

L_03f9:
    i = (i + 0x1);

L_03fd:
    CheckRadioButton(hwnd, 0x7d4, 0x7d6, ((uint16_t)(rgplr[idPlayer].rgmdRelation[i]) + 0x7d4));
    goto L_04c0;

L_042d:
    if ((wParam != 0x76))
        goto L_04c0;
    else
        goto L_0436;

L_0436:
    WinHelp(hwnd, szHelpFile, 0x1, 0x43b);
    return 0x1;

L_045a:
    if ((fDirtyPlan == 0x0))
        goto L_04c0;
    else
        goto L_0464;

L_0464:
    mdSBase = (grbitScan & 0xf);
    LogChangeRelations();
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_048d:
    if ((message == WM_DESTROY))
        goto L_045a;
    else
        goto L_0495;

L_0495:
    if ((message == WM_PAINT))
        goto L_019f;
    else
        goto L_049d;

L_049d:
    if ((message == WM_ERASEBKGND))
        goto L_0177;
    else
        goto L_04a5;

L_04a5:
    if ((message == WM_CTLCOLOR))
        goto L_02aa;
    else
        goto L_04ad;

L_04ad:
    if ((message == WM_INITDIALOG))
        goto L_0097;
    else
        goto L_04b5;

L_04b5:
    if ((message == WM_COMMAND))
        goto L_02e2;
    else
        goto L_04c0;

L_04c0:
    return 0x0;
}

int16_t NewPlanNameDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    RECT     rc;
    uint16_t t_merge_05e2_0001;

L_04ce:
    goto L_0621;

L_04dd:
    SetWindowPos(hwnd, 0x0, (ptStickyBattlePlansDlg.x + 0x46), (ptStickyBattlePlansDlg.y + 0x46), 0x0, 0x0, 0x15);
    SendDlgItemMessage(hwnd, 0x10c, 0x415, 0x1f, 0x0);
    SetDlgItemText(hwnd, IDC_EDIT1, "");
    return 0x1;

L_0538:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0560:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_0644;
    else
        goto L_057c;

L_057c:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_0595:
    if ((wParam == 0x1))
        goto L_05a7;
    else
        goto L_059e;

L_059e:
    if ((wParam != 0x2))
        goto L_05f1;
    else
        goto L_05a7;

L_05a7:
    if ((wParam != 0x1))
        goto L_05cd;
    else
        goto L_05b0;

L_05b0:
    GetDlgItemText(hwnd, IDC_EDIT1, "", 0x20);
    fDirtyPlan = 0x1;

L_05cd:
    if ((wParam != 0x1))
        goto L_05df;
    else
        goto L_05d9;

L_05d9:
    t_merge_05e2_0001 = 0x1;
    goto L_05e2;

L_05df:
    t_merge_05e2_0001 = 0x0;

L_05e2:
    EndDialog(hwnd, t_merge_05e2_0001);
    return 0x1;

L_05f1:
    if ((wParam != 0x76))
        goto L_0644;
    else
        goto L_05fa;

L_05fa:
    WinHelp(hwnd, szHelpFile, 0x1, 0x439);
    return 0x1;

L_0621:
    if ((message == WM_ERASEBKGND))
        goto L_0538;
    else
        goto L_0629;

L_0629:
    if ((message == WM_CTLCOLOR))
        goto L_0560;
    else
        goto L_0631;

L_0631:
    if ((message == WM_INITDIALOG))
        goto L_04dd;
    else
        goto L_0639;

L_0639:
    if ((message == WM_COMMAND))
        goto L_0595;
    else
        goto L_0644;

L_0644:
    return 0x0;
}

int16_t BattlePlansDlg(HWND hwnd, WMType message, uint16_t wParam, int32_t lParam) {
    int16_t (**lpProc)();
    int16_t  idc;
    int16_t  i;
    int16_t  fRet;
    RECT     rc;
    int16_t  cLen;
    uint16_t t_merge_0756_0001;
    uint16_t t_merge_077c_0001;
    uint16_t t_merge_1105_0001;
    uint16_t t_merge_112b_0001;
    uint8_t  t_11bd;
    uint16_t t_merge_160a_0001;
    uint16_t t_merge_1630_0001;

L_0652:
    goto L_16d5;

L_0661:
    StickyDlgPos(hwnd, &(ptStickyBattlePlansDlg), 0x1);
    iPlanSelDlg = 0x0;
    if ((sel.grobj != grobjFleet))
        goto L_068d;
    else
        goto L_0684;

L_0684:
    /* untranslated: iPlanSelDlg = (part[96:2](sel.fl) & 0xff) */

L_068d:
    btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
    i = 0x0;
    goto L_06c5;

L_06c1:
    i = (i + 0x1);

L_06c5:
    if ((i >= rgcbtlplan[idPlayer]))
        goto L_0714;
    else
        goto L_06d8;

L_06d8:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, words(HIWORD(rglpbtlplan[idPlayer]), ((LOWORD(rglpbtlplan[idPlayer])
     * + loword((0x24 * i))) + 0x4))) -> callresult(LRESULT) */
    goto L_06c1;

L_0714:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0x0);
    GetDlgItem(hwnd, IDC_RENAME);
    if ((iPlanSelDlg <= 0x0))
        goto L_0753;
    else
        goto L_074d;

L_074d:
    t_merge_0756_0001 = 0x1;
    goto L_0756;

L_0753:
    t_merge_0756_0001 = 0x0;

L_0756:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_0756_0001) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDC_DELETE);
    if ((iPlanSelDlg <= 0x0))
        goto L_0779;
    else
        goto L_0773;

L_0773:
    t_merge_077c_0001 = 0x1;
    goto L_077c;

L_0779:
    t_merge_077c_0001 = 0x0;

L_077c:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_077c_0001) -> callresult(int16_t) */
    i = 0x198;
    goto L_07b7;

L_078a:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(i))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_07b7:
    if ((i <= 0x19d))
        goto L_078a;
    else
        goto L_07c1;

L_07c1:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0x0);
    i = 0x190;
    goto L_0820;

L_07f3:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(i))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_0820:
    if ((i <= 0x197))
        goto L_07f3;
    else
        goto L_082a;

L_082a:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0x0);
    if ((game.fSinglePlr != 0x0))
        goto L_0946;
    else
        goto L_0861;

L_0861:
    i = 0x78;
    goto L_0896;

L_0869:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(i))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_0896:
    if ((i <= 0x7b))
        goto L_0869;
    else
        goto L_089f;

L_089f:
    i = 0x0;
    goto L_08ab;

L_08a7:
    i = (i + 0x1);

L_08ab:
    if ((i >= game.cPlayer))
        goto L_0901;
    else
        goto L_08b6;

L_08b6:
    if ((i == idPlayer))
        goto L_08a7;
    else
        goto L_08c1;

L_08c1:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, words(ds, PszPlayerName(i, 0x0, 0x1, 0x0, 0x0, 0x0))) ->
     * callresult(LRESULT) */

L_0901:
    i = btlplan.iplrAttack;
    if ((i < (idPlayer + 0x4)))
        goto L_0921;
    else
        goto L_091d;

L_091d:
    i = (i - 0x1);

L_0921:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0x0);
    goto L_09a8;

L_0946:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(idsEveryone))) ->
     * callresult(LRESULT) */
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, 0x0, 0x0);
    EnableWindow(GetDlgItem(hwnd, IDC_U16_0x0422), 0x0);

L_09a8:
    i = 0x190;
    goto L_09dd;

L_09b0:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_ADDSTRING, 0x0, words(ds, PszGetCompressedString(i))) -> callresult(LRESULT) */
    i = (i + 0x1);

L_09dd:
    if ((i <= 0x197))
        goto L_09b0;
    else
        goto L_09e7;

L_09e7:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), CB_LIMITTEXT, btlplan.fDumpCargo, 0x0);
    fDirtyPlan = 0x0;
    if ((gd.fTutorial == 0x0))
        goto L_0a5c;
    else
        goto L_0a57;

L_0a57:
    AdvanceTutor();

L_0a5c:
    return 0x1;

L_0a62:
    idc = 0x41d;
    goto L_0a8f;

L_0a6a:
    /* untranslated: ss:[bp-0x16] = LOWORD(lParam) */
    /* untranslated: branch ss:[bp-0x16] == GetDlgItem(hwnd, idc) ? L_0a99 : L_0a85 */

L_0a85:

L_0a8b:
    idc = (idc + 0x1);

L_0a8f:
    if ((idc <= 0x422))
        goto L_0a6a;
    else
        goto L_0a99;

L_0a99:
    if ((idc >= 0x41d))
        goto L_0abf;
    else
        goto L_0aa3;

L_0aa3:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x6))
        goto L_16f8;
    else
        goto L_0abf;

L_0abf:
    SetBkColor(wParam, crButtonFace);
    return hbrButtonFace;

L_0ad8:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x1;

L_0b00:
    if ((wParam == 0x1))
        goto L_0b12;
    else
        goto L_0b09;

L_0b09:
    if ((wParam != 0x2))
        goto L_0bb8;
    else
        goto L_0b12;

L_0b12:
    if ((fDirtyPlan == 0x0))
        goto L_0b6e;
    else
        goto L_0b1c;

L_0b1c:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x16] = loword((0x24 * iPlanSelDlg)) */
    LogChangeBtlplan(&(btlplan));

L_0b6e:
    StickyDlgPos(hwnd, &(ptStickyBattlePlansDlg), 0x0);
    EndDialog(hwnd, iPlanSelDlg);
    if ((sel.grobj != grobjFleet))
        goto L_0ba9;
    else
        goto L_0b97;

L_0b97:
    /* untranslated: call FillBattleDD(((part[96:2](sel.fl) & 0xff) + 0x1)) -> callresult(void) */

L_0ba9:
    iPlanSelDlg = 0xffff;
    return 0x1;

L_0bb8:
    if ((wParam != 0x41d))
        goto L_0c05;
    else
        goto L_0bc2;

L_0bc2:
    btlplan.fDumpCargo = LOWORD(SendDlgItemMessage(hwnd, 0x41d, 0x400, 0x0, 0x0));
    fDirtyPlan = 0x1;
    goto L_16f8;

L_0c05:
    if ((wParam != 0x817))
        goto L_0e25;
    else
        goto L_0c0f;

L_0c0f:
    if ((fDirtyPlan == 0x0))
        goto L_0c71;
    else
        goto L_0c19;

L_0c19:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x16] = loword((0x24 * iPlanSelDlg)) */
    LogChangeBtlplan(&(btlplan));
    fDirtyPlan = 0x0;

L_0c71:
    btlplan.fDelete = 0x1;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x16] = loword((0x24 * iPlanSelDlg)) */
    btlplan.iplan = iPlanSelDlg;
    if ((FDeleteBattlePlan(iPlanSelDlg, 0x1) == 0x0))
        goto L_0dd0;
    else
        goto L_0cf7;

L_0cf7:
    LogChangeBtlplan(&(btlplan));
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, (iPlanSelDlg + 0xffff), 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0x0);
    i = 0x0;
    goto L_0d56;

L_0d52:
    i = (i + 0x1);

L_0d56:
    if ((i >= rgcbtlplan[idPlayer]))
        goto L_0da5;
    else
        goto L_0d69;

L_0d69:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, words(HIWORD(rglpbtlplan[idPlayer]), ((LOWORD(rglpbtlplan[idPlayer])
     * + loword((0x24 * i))) + 0x4))) -> callresult(LRESULT) */
    goto L_0d52;

L_0da5:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, (iPlanSelDlg + 0xffff), 0x0);
    goto LSelectName;

L_0dd0:
    btlplan.fDelete = 0x0;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    goto L_16f8;

L_0e25:
    if ((wParam != 0x41f))
        goto L_0e70;
    else
        goto L_0e2f;

L_0e2f:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), CB_GETCURSEL, 0x0, 0x0));
    btlplan.mdTarget1 = i;
    fDirtyPlan = 0x1;
    goto L_16f8;

L_0e70:
    if ((wParam != 0x420))
        goto L_0ec4;
    else
        goto L_0e7a;

L_0e7a:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), CB_GETCURSEL, 0x0, 0x0));
    btlplan.mdTarget2 = i;
    fDirtyPlan = 0x1;
    goto L_16f8;

L_0ec4:
    if ((wParam != 0x422))
        goto L_0f41;
    else
        goto L_0ece;

L_0ece:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), CB_GETCURSEL, 0x0, 0x0));
    if ((game.fSinglePlr == 0x0))
        goto L_0f0d;
    else
        goto L_0f05;

L_0f05:
    i = 0x3;
    goto L_0f1f;

L_0f0d:
    if ((i < (idPlayer + 0x4)))
        goto L_0f1f;
    else
        goto L_0f1b;

L_0f1b:
    i = (i + 0x1);

L_0f1f:
    btlplan.iplrAttack = i;
    fDirtyPlan = 0x1;
    goto L_16f8;

L_0f41:
    if ((wParam != 0x421))
        goto L_0f92;
    else
        goto L_0f4b;

L_0f4b:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, wParam), CB_GETCURSEL, 0x0, 0x0));
    btlplan.mdTactic = i;
    fDirtyPlan = 0x1;
    goto L_16f8;

L_0f92:
    if ((wParam != 0x41b))
        goto L_1134;
    else
        goto LRename;

LRename:
    StickyDlgPos(hwnd, &(ptStickyBattlePlansDlg), 0x0);
    lpProc = MakeProcInstance(NewPlanNameDlg, hInst);
    fRet = DialogBox(hInst, IDD_RENAME, hwndFrame, &(lpProc));
    FreeProcInstance(&(lpProc));
    SetFocus(hwnd);
    if ((fRet == 0x0))
        goto L_10e5;
    else
        goto L_1000;

L_1000:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x16] = loword((0x24 * iPlanSelDlg)) */
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0x0);
    i = 0x0;
    goto L_1074;

L_1070:
    i = (i + 0x1);

L_1074:
    if ((i >= rgcbtlplan[idPlayer]))
        goto L_10c3;
    else
        goto L_1087;

L_1087:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, words(HIWORD(rglpbtlplan[idPlayer]), ((LOWORD(rglpbtlplan[idPlayer])
     * + loword((0x24 * i))) + 0x4))) -> callresult(LRESULT) */
    goto L_1070;

L_10c3:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0x0);

L_10e5:
    GetDlgItem(hwnd, IDC_RENAME);
    if ((iPlanSelDlg <= 0x0))
        goto L_1102;
    else
        goto L_10fc;

L_10fc:
    t_merge_1105_0001 = 0x1;
    goto L_1105;

L_1102:
    t_merge_1105_0001 = 0x0;

L_1105:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_1105_0001) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDC_DELETE);
    if ((iPlanSelDlg <= 0x0))
        goto L_1128;
    else
        goto L_1122;

L_1122:
    t_merge_112b_0001 = 0x1;
    goto L_112b;

L_1128:
    t_merge_112b_0001 = 0x0;

L_112b:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_112b_0001) -> callresult(int16_t) */
    goto L_16f8;

L_1134:
    if ((wParam != 0x41c))
        goto L_149b;
    else
        goto L_113e;

L_113e:
    if ((rgcbtlplan[idPlayer] != 0xf))
        goto L_1157;
    else
        goto L_1151;

L_1151:
    return 0x0;

L_1157:
    if ((fDirtyPlan == 0x0))
        goto L_11b9;
    else
        goto L_1161;

L_1161:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x18] = loword((0x24 * iPlanSelDlg)) */
    LogChangeBtlplan(&(btlplan));
    fDirtyPlan = 0x0;

L_11b9:
    t_11bd = rgcbtlplan[idPlayer];
    rgcbtlplan[idPlayer] = (rgcbtlplan[idPlayer] + 0x1);
    iPlanSelDlg = t_11bd;
    cLen = strlen(btlplan.szName);
    if ((cLen > 0x1b))
        goto L_128b;
    else
        goto L_11e1;

L_11e1:

L_11e7:
    /* untranslated: branch byte ds:[(cLen + 0x151d7)] != 0x29 ? L_123c : L_11ff */

L_11ff:
    /* untranslated: branch (byte ds:[(0x175f + byte ds:[(cLen + 0x151d6)])] & 0x4) == 0x0 ? L_123c : L_1224 */

L_1224:
    /* untranslated: branch byte ds:[(cLen + 0x151d5)] == 0x28 ? L_1254 : L_123c */

L_123c:
    strcpy(btlplan.szName[cLen], " (2)");
    goto L_128b;

L_1254:
    /* untranslated: branch byte ds:[(cLen + 0x151d6)] != 0x39 ? L_127d : L_126c */

L_126c:
    /* untranslated: byte ds:[(cLen + 0x151d6)] = 0x30 */
    goto L_128b;

L_127d:
    /* untranslated: byte ds:[(cLen + 0x151d6)] = (byte ds:[(cLen + 0x151d6)] + 0x1) */

L_128b:
    btlplan.iplan = iPlanSelDlg;
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x18] = loword((0x24 * iPlanSelDlg)) */
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_RESETCONTENT, 0x0, 0x0);
    i = 0x0;
    goto L_1345;

L_1341:
    i = (i + 0x1);

L_1345:
    if ((i >= rgcbtlplan[idPlayer]))
        goto L_1394;
    else
        goto L_1358;

L_1358:
    /* untranslated: call SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_ADDSTRING, 0x0, words(HIWORD(rglpbtlplan[idPlayer]), ((LOWORD(rglpbtlplan[idPlayer])
     * + loword((0x24 * i))) + 0x4))) -> callresult(LRESULT) */
    goto L_1341;

L_1394:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_SETCURSEL, iPlanSelDlg, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), CB_LIMITTEXT, btlplan.fDumpCargo, 0x0);
    i = btlplan.iplrAttack;
    if ((i < (idPlayer + 0x4)))
        goto L_1452;
    else
        goto L_144e;

L_144e:
    i = (i - 0x1);

L_1452:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0x0);
    fDirtyPlan = 0x1;
    wParam = 0x41f;
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), 0x1);
    goto LRename;

L_149b:
    if ((wParam != 0x41e))
        goto L_16a5;
    else
        goto LSelectName;

LSelectName:
    i = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x041E), CB_GETCURSEL, 0x0, 0x0));
    if ((i == iPlanSelDlg))
        goto L_16f8;
    else
        goto L_14d5;

L_14d5:
    if ((fDirtyPlan == 0x0))
        goto L_1537;
    else
        goto L_14df;

L_14df:
    rglpbtlplan[idPlayer][iPlanSelDlg] = btlplan;
    /* untranslated: ss:[bp-0x16] = loword((0x24 * iPlanSelDlg)) */
    LogChangeBtlplan(&(btlplan));
    fDirtyPlan = 0x0;

L_1537:
    iPlanSelDlg = i;
    btlplan = rglpbtlplan[idPlayer][iPlanSelDlg];
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041F), CB_SETCURSEL, btlplan.mdTarget1, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0420), CB_SETCURSEL, btlplan.mdTarget2, 0x0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x041D), CB_LIMITTEXT, btlplan.fDumpCargo, 0x0);
    wParam = 0x41f;
    GetDlgItem(hwnd, IDC_RENAME);
    if ((iPlanSelDlg <= 0x0))
        goto L_1607;
    else
        goto L_1601;

L_1601:
    t_merge_160a_0001 = 0x1;
    goto L_160a;

L_1607:
    t_merge_160a_0001 = 0x0;

L_160a:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_160a_0001) -> callresult(int16_t) */
    GetDlgItem(hwnd, IDC_DELETE);
    if ((iPlanSelDlg <= 0x0))
        goto L_162d;
    else
        goto L_1627;

L_1627:
    t_merge_1630_0001 = 0x1;
    goto L_1630;

L_162d:
    t_merge_1630_0001 = 0x0;

L_1630:
    /* untranslated: call EnableWindow(callresult(HWND), t_merge_1630_0001) -> callresult(int16_t) */
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0421), CB_SETCURSEL, btlplan.mdTactic, 0x0);
    i = btlplan.iplrAttack;
    if ((i < (idPlayer + 0x4)))
        goto L_1680;
    else
        goto L_167c;

L_167c:
    i = (i - 0x1);

L_1680:
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0422), CB_SETCURSEL, i, 0x0);

L_16a5:
    if ((wParam != 0x76))
        goto L_16f8;
    else
        goto L_16ae;

L_16ae:
    WinHelp(hwnd, szHelpFile, 0x1, 0x439);
    return 0x1;

L_16d5:
    if ((message == WM_ERASEBKGND))
        goto L_0ad8;
    else
        goto L_16dd;

L_16dd:
    if ((message == WM_CTLCOLOR))
        goto L_0a62;
    else
        goto L_16e5;

L_16e5:
    if ((message == WM_INITDIALOG))
        goto L_0661;
    else
        goto L_16ed;

L_16ed:
    if ((message == WM_COMMAND))
        goto L_0b00;
    else
        goto L_16f8;

L_16f8:
    return 0x0;
}

int16_t FDeleteBattlePlan(int16_t iplan, int16_t fWarn) {
    int16_t fFoundBigger;
    int16_t iflMac;
    int16_t i;
    FLEET  *lpfl;

L_1706:
    fFoundBigger = 0x0;

LCommit:
    iflMac = 0x0;
    goto L_1720;

L_171c:
    iflMac = (iflMac + 0x1);

L_1720:
    if ((iflMac >= cFleet))
        goto L_181a;
    else
        goto L_172b;

L_172b:
    lpfl = rglpfl[iflMac];
    if ((LOWORD(rglpfl[iflMac]) != 0x0))
        goto L_175b;
    else
        goto L_1753;

L_1753:
    if ((*(rglpfl[iflMac] + 0x2) == 0x0))
        goto L_181a;
    else
        goto L_175b;

L_175b:
    if ((lpfl->iPlayer < idPlayer))
        goto L_171c;
    else
        goto L_1767;

L_1767:

L_1770:
    if ((lpfl->iPlayer > idPlayer))
        goto L_181a;
    else
        goto L_177c;

L_177c:

L_1785:
    if ((lpfl->iplan < iplan))
        goto L_171c;
    else
        goto L_1799;

L_1799:
    if ((lpfl->iplan <= iplan))
        goto L_17c9;
    else
        goto L_17ad;

L_17ad:
    if ((fWarn != 0x0))
        goto L_17c1;
    else
        goto L_17b6;

L_17b6:
    lpfl->iplan = (lpfl->iplan - 0x1);
    goto L_171c;

L_17c1:
    fFoundBigger = 0x1;

L_17c9:
    if ((fWarn == 0x0))
        goto L_180f;
    else
        goto L_17d2;

L_17d2:
    if ((AlertSz(PszFormatIds(idsCurrentlyHaveFleetsUsingBattlePlanIf, 0x0), 0x31) != 0x2))
        goto L_1804;
    else
        goto L_17fb;

L_17fb:
    return 0x0;

L_1804:
    fWarn = 0x0;
    goto LCommit;

L_180f:
    lpfl->iplan = (lpfl->iplan - 0x1);

L_181a:
    if ((fWarn == 0x0))
        goto L_1834;
    else
        goto L_1823;

L_1823:
    if ((fFoundBigger == 0x0))
        goto L_1834;
    else
        goto L_182c;

L_182c:
    fWarn = 0x0;
    goto LCommit;

L_1834:
    rgcbtlplan[idPlayer] = (rgcbtlplan[idPlayer] - 0x1);
    i = iplan;
    goto L_184a;

L_1846:
    i = (i + 0x1);

L_184a:
    if ((i >= rgcbtlplan[idPlayer]))
        goto L_191d;
    else
        goto L_185d;

L_185d:
    rglpbtlplan[idPlayer][i] = rglpbtlplan[idPlayer][(i + 0x1)];
    LOWORD(rglpbtlplan[idPlayer][i]) = ((LOWORD(rglpbtlplan[idPlayer][i]) & 0xff0f) | ((i & 0xf) * 0x10));
    goto L_1846;

L_191d:
    return 0x1;
}

void SpankTheCheaters() {
    int32_t lSell;
    PLANET *lppl;
    FLEET  *lpfl;
    int16_t ifl;
    int16_t i;
    int32_t pctSell;
    int16_t fCheater;
    int16_t fSellOff;
    char    rgfCheater[16];
    PLANET *lpplMac;

L_192a:
    fCheater = 0x0;
    i = 0x0;
    goto L_1944;

L_1940:
    i = (i + 0x1);

L_1944:
    if ((i >= game.cPlayer))
        goto L_197f;
    else
        goto L_194f;

L_194f:
    rgfCheater[i] = LOBYTE(((rgplr[i].wFlags >> 0x2) & 0x1));
    if (((uint16_t)(LOBYTE(((rgplr[i].wFlags >> 0x2) & 0x1))) == 0x0))
        goto L_1940;
    else
        goto L_1977;

L_1977:
    fCheater = 0x1;

L_197f:
    if ((fCheater == 0x0))
        goto L_1e10;
    else
        goto L_1988;

L_1988:
    if ((game.turn < 0xa))
        goto L_1e10;
    else
        goto L_198f;

L_198f:

L_1995:
    ifl = 0x0;
    goto L_19a1;

L_199d:
    ifl = (ifl + 0x1);

L_19a1:
    if ((ifl >= cFleet))
        goto L_1b6e;
    else
        goto L_19ac;

L_19ac:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_19dc;
    else
        goto L_19d4;

L_19d4:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1b6e;
    else
        goto L_19dc;

L_19dc:
    if ((lpfl->fDead != 0x0))
        goto L_199d;
    else
        goto L_19f3;

L_19f3:
    if (((uint16_t)(rgfCheater[lpfl->iPlayer]) == 0x0))
        goto L_199d;
    else
        goto L_1a0a;

L_1a0a:
    if ((Random(0xc) != 0x0))
        goto L_1a56;
    else
        goto L_1a1e;

L_1a1e:
    lpfl->fDead = 0x1;
    FSendPlrMsg2(lpfl->iPlayer, 0x104, 0xfffb, lpfl->id, 0x0);
    goto L_199d;

L_1a56:
    fSellOff = 0x0;
    i = 0x0;
    goto L_1b35;

L_1a63:
    if ((HIWORD(lpfl->rgwtMin[i]) < 0x0))
        goto L_1b31;
    else
        goto L_1a83;

L_1a83:
    if ((HIWORD(lpfl->rgwtMin[i]) > 0x0))
        goto L_1a91;
    else
        goto L_1a88;

L_1a88:
    if ((LOWORD(lpfl->rgwtMin[i]) <= 0x0))
        goto L_1b31;
    else
        goto L_1a91;

L_1a91:
    if ((fSellOff != 0x0))
        goto L_1ab5;
    else
        goto L_1a9a;

L_1a9a:
    pctSell = (uint32_t)((Random(0xb) + 0xa));
    fSellOff = 0x1;

L_1ab5:
    lSell = (int32_t)(((uint32_t)((lpfl->rgwtMin[i] * pctSell)) / 0x64));
    if ((LOWORD(lSell) != 0x0))
        goto L_1b0e;
    else
        goto L_1afb;

L_1afb:
    if ((HIWORD(lSell) != 0x0))
        goto L_1b0e;
    else
        goto L_1b04;

L_1b04:
    lSell = 0x1;

L_1b0e:
    lpfl->rgwtMin[i] = (lpfl->rgwtMin[i] - lSell);

L_1b31:
    i = (i + 0x1);

L_1b35:
    if ((i <= 0x2))
        goto L_1a63;
    else
        goto L_1b3e;

L_1b3e:
    if ((fSellOff == 0x0))
        goto L_199d;
    else
        goto L_1b47;

L_1b47:
    FSendPlrMsg2(lpfl->iPlayer, 0x105, 0xfffb, lpfl->id, LOWORD(pctSell));

L_1b6e:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_1e02;

L_1b99:
    if ((lppl->iPlayer == 0xffff))
        goto L_1dfe;
    else
        goto L_1ba6;

L_1ba6:
    if (((uint16_t)(rgfCheater[lppl->iPlayer]) == 0x0))
        goto L_1dfe;
    else
        goto L_1bbd;

L_1bbd:
    if ((0x0 < 0x0))
        goto L_1cf1;
    else
        goto L_1bde;

L_1bde:
    if ((0x0 > 0x0))
        goto L_1beb;
    else
        goto L_1be3;

L_1be3:
    /* untranslated: branch (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) <= 0x0 ? L_1cf1 : L_1beb */

L_1beb:
    if ((Random(0x8) != 0x0))
        goto L_1cf1;
    else
        goto L_1bff;

L_1bff:
    pctSell = (uint32_t)((Random(0x1f) + 0x5));
    /* untranslated: lSell = (int32_t)((uint32_t)(words(0x0, (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff)) * pctSell) / 0x64) */
    if ((HIWORD(lSell) > 0x0))
        goto L_1c71;
    else
        goto L_1c59;

L_1c59:
    if ((HIWORD(lSell) < 0x0))
        goto L_1c67;
    else
        goto L_1c5e;

L_1c5e:
    if ((LOWORD(lSell) > 0x0))
        goto L_1c71;
    else
        goto L_1c67;

L_1c67:
    lSell = 0x1;

L_1c71:
    /* untranslated: ss:[bp-0x32] = ((*(lppl+0x14) - (LOWORD(lSell) << 0x8)) & 0xff00) */
    /* untranslated: ss:[bp-0x30] = ((*(lppl+0x16) - 0x0) & 0xf) */
    lppl->cMines = 0x0;
    /* untranslated: *(lppl+0x14) = (*(lppl+0x14) | ss:[bp-0x32]) */
    /* untranslated: *(lppl+0x16) = (*(lppl+0x16) | ss:[bp-0x30]) */
    FSendPlrMsg2(lppl->iPlayer, 0x106, 0xfffb, lppl->id, LOWORD(lSell));
    goto L_1dfe;

L_1cf1:
    if ((Random(0xf) != 0x0))
        goto L_1dfe;
    else
        goto L_1d05;

L_1d05:
    i = Random(0x3);
    pctSell = (uint32_t)((Random(0x29) + 0x5));
    lSell = (int32_t)(((uint32_t)((lppl->rgwtMin[i] * pctSell)) / 0x64));
    if ((HIWORD(lSell) < 0x0))
        goto L_1dfe;
    else
        goto L_1d70;

L_1d70:
    if ((HIWORD(lSell) > 0x0))
        goto L_1d7e;
    else
        goto L_1d75;

L_1d75:
    if ((LOWORD(lSell) <= 0x0))
        goto L_1dfe;
    else
        goto L_1d7e;

L_1d7e:
    if ((HIWORD(lSell) < 0x0))
        goto L_1da0;
    else
        goto L_1d87;

L_1d87:
    if ((HIWORD(lSell) > 0x0))
        goto L_1d96;
    else
        goto L_1d8c;

L_1d8c:
    if ((LOWORD(lSell) <= 0x7530))
        goto L_1da0;
    else
        goto L_1d96;

L_1d96:
    lSell = 0x7530;

L_1da0:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] - lSell);
    FSendPlrMsg(lppl->iPlayer, 0x107, 0xfffb, lppl->id, LOWORD(lSell), (i + 0x1), 0x0, 0x0, 0x0, 0x0);

L_1dfe:
    lppl = (lppl + 0x1);

L_1e02:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1b99;
    else
        goto L_1e10;

L_1e10:
    return;
}

int16_t FFleetHasBombs(FLEET *lpfl) {
    HUL    *lphul;
    int16_t imd;
    int16_t ishdef;

L_1e16:
    ishdef = 0x0;
    goto L_1ead;

L_1e27:
    if ((lpfl->rgcsh[ishdef] == 0x0))
        goto L_1ea9;
    else
        goto L_1e44;

L_1e44:
    lphul = &(rglpshdef[lpfl->iplr][ishdef]);
    imd = ((*(LphuldefFromId(lphul->ihuldef) + 0x7b) >> 0x6) & 0xf);
    if ((FHullHasBombs(lphul) == 0x0))
        goto L_1ea9;
    else
        goto L_1ea3;

L_1ea3:
    return 0x1;

L_1ea9:
    ishdef = (ishdef + 0x1);

L_1ead:
    if ((ishdef < 0x10))
        goto L_1e27;
    else
        goto L_1eb6;

L_1eb6:
    return 0x0;
}

int16_t FHullHasBombs(HUL *lphul) {
    HS     *lphs;
    int16_t ihs;

L_1ec2:
    lphs = lphul->rghs;
    ihs = 0x0;
    goto L_1ef7;

L_1ee4:
    ihs = (ihs + 0x1);
    lphs = (lphs + 0x1);

L_1ef7:
    if ((ihs >= lphul->chs))
        goto L_1fb2;
    else
        goto L_1f09;

L_1f09:
    if ((lphs->grhst != hstBomb))
        goto L_1f35;
    else
        goto L_1f15;

L_1f15:
    if ((lphs->cItem == 0x0))
        goto L_1f35;
    else
        goto L_1f2c;

L_1f2c:
    return 0x1;

L_1f35:
    if ((lphs->grhst != hstBeam))
        goto L_1f73;
    else
        goto L_1f41;

L_1f41:
    if ((lphs->iItem != ibeamMultiContainedMunition))
        goto L_1f73;
    else
        goto L_1f53;

L_1f53:
    if ((lphs->cItem <= 0x0))
        goto L_1f73;
    else
        goto L_1f6a;

L_1f6a:
    return 0x1;

L_1f73:
    if ((lphs->grhst != hstSpecialM))
        goto L_1ee4;
    else
        goto L_1f80;

L_1f80:
    if ((lphs->iItem != ispecialMOrbitalConstructionModule))
        goto L_1ee4;
    else
        goto L_1f92;

L_1f92:
    if ((lphs->cItem <= 0x0))
        goto L_1ee4;
    else
        goto L_1fa9;

L_1fa9:
    return 0x1;

L_1fb2:
    return 0x0;
}

int16_t FFleetHasTeeth(FLEET *lpfl) {
    int16_t ishdef;

L_1fbe:
    ishdef = 0x0;
    goto L_205d;

L_1fcf:
    if ((lpfl->rgcsh[ishdef] == 0x0))
        goto L_2059;
    else
        goto L_1fec;

L_1fec:
    if ((FHullHasTeeth(&(rglpshdef[lpfl->iplr][ishdef])) == 0x0))
        goto L_2059;
    else
        goto L_2020;

L_2020:
    if (((rglpshdef[lpfl->iplr][ishdef].wFlags & 0xff) != 0x7))
        goto L_2059;
    else
        goto L_2053;

L_2053:
    return 0x1;

L_2059:
    ishdef = (ishdef + 0x1);

L_205d:
    if ((ishdef < 0x10))
        goto L_1fcf;
    else
        goto L_2066;

L_2066:
    return 0x0;
}

int16_t FHullHasTeeth(HUL *lphul) {
    HS     *lphs;
    int16_t ihs;

L_2072:
    lphs = lphul->rghs;
    ihs = 0x0;
    goto L_20a7;

L_2094:
    ihs = (ihs + 0x1);
    lphs = (lphs + 0x1);

L_20a7:
    if ((ihs >= lphul->chs))
        goto L_20ea;
    else
        goto L_20b9;

L_20b9:
    if (((lphs->grhst & 0x30) == 0x0))
        goto L_2094;
    else
        goto L_20ca;

L_20ca:
    if ((lphs->cItem <= 0x0))
        goto L_2094;
    else
        goto L_20e1;

L_20e1:
    return 0x1;

L_20ea:
    return 0x0;
}

int16_t FFuelTanker(SHDEF *lpshdef) {
L_20f6:
    if ((lpshdef->hul.ihuldef == ihuldefFuelTransport))
        goto L_2117;
    else
        goto L_210b;

L_210b:
    if ((lpshdef->hul.ihuldef != ihuldefSuperFuelXport))
        goto L_211d;
    else
        goto L_2117;

L_2117:
    return 0x1;

L_211d:
    return 0x0;
}

void CheckTarget(TOK *ptok, FLEET *lpfl, int16_t ishdef) {
    int16_t  iplr;
    BTLPLAN *lpbtlplan;
    int16_t  ibp;
    SHDEF   *lpshdef;

L_212a:
    iplr = lpfl->iplr;
    lpshdef = &(rglpshdef[iplr][ishdef]);
    if ((FHullHasTeeth(&(lpshdef)) == 0x0))
        goto L_218e;
    else
        goto L_2177;

L_2177:
    ptok->mdTarget0 = 0x3;
    goto L_222d;

L_218e:
    if ((FHullHasBombs(&(lpshdef)) == 0x0))
        goto L_21bb;
    else
        goto L_21a4;

L_21a4:
    ptok->mdTarget0 = 0x4;
    goto L_222d;

L_21bb:
    if ((FFuelTanker(lpshdef) == 0x0))
        goto L_21e8;
    else
        goto L_21d1;

L_21d1:
    ptok->mdTarget0 = 0x6;
    goto L_222d;

L_21e8:
    if ((WtMaxShdefStat(lpshdef, 0x2) == 0x0))
        goto L_2219;
    else
        goto L_2202;

L_2202:
    ptok->mdTarget0 = 0x7;
    goto L_222d;

L_2219:
    ptok->mdTarget0 = 0x5;

L_222d:
    ibp = lpfl->iplan;
    lpbtlplan = &(rglpbtlplan[iplr][ibp]);
    *(ptok + 0x17) = ((*(ptok + 0x17) & 0xfff0) | (lpbtlplan->mdTarget1 & 0xf));
    *(ptok + 0x17) = ((*(ptok + 0x17) & 0xff0f) | ((lpbtlplan->mdTarget2 & 0xf) << 0x4));
    if ((ptok->mdTarget0 != 0x3))
        goto L_2304;
    else
        goto L_22cf;

L_22cf:
    *(ptok + 0x17) = ((*(ptok + 0x17) & 0xf0ff) | ((lpbtlplan->mdTactic & 0xf) << 0x8));
    goto L_2318;

L_2304:
    ptok->mdTactic = 0x0;

L_2318:
    if ((ptok->mdTactic != 0x0))
        goto L_2343;
    else
        goto L_232f;

L_232f:
    ptok->dzDis = 0x7;

L_2343:
    return;
}

int16_t FDumpCargo(FLEET *lpfl) {
    POINT   pt;
    PLANET *lppl;
    int16_t i;

L_234a:
    i = 0x0;
    goto L_238b;

L_235b:
    if ((LOWORD(lpfl->rgwtMin[i]) != 0x0))
        goto L_2394;
    else
        goto L_237a;

L_237a:
    if ((HIWORD(lpfl->rgwtMin[i]) != 0x0))
        goto L_2394;
    else
        goto L_2381;

L_2381:

L_2387:
    i = (i + 0x1);

L_238b:
    if ((i <= 0x2))
        goto L_235b;
    else
        goto L_2394;

L_2394:
    if ((i <= 0x2))
        goto L_23a3;
    else
        goto L_239d;

L_239d:
    return 0x0;

L_23a3:
    if ((((LOWORD(rglpbtlplan[lpfl->iplr][lpfl->iplan]) >> 0xf) & 0x1) != 0x0))
        goto L_23e9;
    else
        goto L_23e3;

L_23e3:
    return 0x0;

L_23e9:
    if ((lpfl->idPlanet == 0xffff))
        goto L_245d;
    else
        goto L_23f6;

L_23f6:
    lppl = LpplFromId(lpfl->idPlanet);
    i = 0x0;
    goto L_2451;

L_2413:
    lppl->rgwtMin[i] = (lppl->rgwtMin[i] + lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_2451:
    if ((i > 0x2))
        goto L_249a;
    else
        goto L_2457;

L_2457:

L_245d:
    pt.x = lpfl->pt.x;
    pt.y = lpfl->pt.y;
    DropSalvage(&(lpthBattle), &(lpfl->rgwtMin), lpfl->iplr, &(pt));

L_249a:
    i = 0x0;
    goto L_24c7;

L_24a2:
    lpfl->rgwtMin[i] = 0x0;
    i = (i + 0x1);

L_24c7:
    if ((i <= 0x2))
        goto L_24a2;
    else
        goto L_24d0;

L_24d0:
    return 0x1;
}

void DropSalvage(THING **plpth, int32_t *rgwtMinerals, int16_t iplr, POINT *ppt) {
    int32_t wtTotal;
    int32_t wt;
    int16_t i;
    THING  *lpth;

L_24dc:
    lpth = *(plpth);
    wtTotal = 0x0;
    i = 0x0;
    goto L_2509;

L_2505:
    i = (i + 0x1);

L_2509:
    if ((i >= game.cPlanMax))
        goto L_253b;
    else
        goto L_2514;

L_2514:
    if ((ppt->x != rgptPlan[i].x))
        goto L_2505;
    else
        goto L_252d;

L_252d:
    if ((ppt->y == rgptPlan[i].y))
        goto L_294c;
    else
        goto L_2532;

L_2532:

L_253b:
    i = 0x0;
    goto L_2565;

L_2543:
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 0x1);

L_2565:
    if ((i >= 0x3))
        goto L_25cb;
    else
        goto L_256b;

L_256b:

L_2571:
    i = 0x0;
    goto L_25c2;

L_2579:
    rgwtMinerals[i] = (uint32_t)(Random(0xa));
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 0x1);

L_25c2:
    if ((i < 0x3))
        goto L_2579;
    else
        goto L_25cb;

L_25cb:
    if ((LOWORD(wtTotal) != 0x0))
        goto L_25dd;
    else
        goto L_25d4;

L_25d4:
    if ((HIWORD(wtTotal) == 0x0))
        goto L_2571;
    else
        goto L_25dd;

L_25dd:
    if ((LOWORD(lpth) != 0x0))
        goto L_2657;
    else
        goto L_25e6;

L_25e6:
    if ((HIWORD(lpth) != 0x0))
        goto L_2657;
    else
        goto L_25ef;

L_25ef:
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((LOWORD(lpth) != 0x0))
        goto L_2619;
    else
        goto L_260d;

L_260d:
    if ((HIWORD(lpth) == 0x0))
        goto L_294c;
    else
        goto L_2613;

L_2613:

L_2619:
    lpth->thp.iWarp = 0x0;
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    lpth->thp.idPlanet = 0x3ff;
    goto L_26e9;

L_2657:
    i = 0x0;
    goto L_26cc;

L_265f:
    /* untranslated: LOWORD(rgwtMinerals[i]) = (LOWORD(rgwtMinerals[i]) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(rgwtMinerals[i]) = (HIWORD(rgwtMinerals[i]) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: LOWORD(wtTotal) = (LOWORD(wtTotal) + HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))]) */
    /* untranslated: HIWORD(wtTotal) = (HIWORD(wtTotal) + signhiword(HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))])) */
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = 0x0 */
    i = (i + 0x1);

L_26cc:
    if ((i < 0x3))
        goto L_265f;
    else
        goto L_26d5;

L_26d5:
    lpth->thp.wtMax = 0x0;

L_26e9:
    lpth->thp.fMoved = 0x1;
    goto L_2927;

L_2700:
    i = 0x0;
    goto L_291e;

L_2708:
    /* untranslated: branch (hiword((uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) + HIWORD(rgwtMinerals[i])) < 0x0 ? L_2842 : L_2746 */

L_2746:
    /* untranslated: branch (hiword((uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) + HIWORD(rgwtMinerals[i])) > 0x0 ? L_2753 : L_274b */

L_274b:
    /* untranslated: branch (loword((uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) + LOWORD(rgwtMinerals[i])) <= 0x7530 ? L_2842 : L_2753 */

L_2753:
    /* untranslated: wt = (0x7530 - (uint32_t)(words(0x0, lpth->thp.wtMax) * 0xa)) */
    wtTotal = (wtTotal - wt);
    lpth->thp.wtMax = 0xbb8;
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + LOWORD(wt)) */
    rgwtMinerals[i] = (rgwtMinerals[i] - wt);
    lpth = LpthNew(iplr, ithMineralPacket);
    if ((LOWORD(lpth) != 0x0))
        goto L_2804;
    else
        goto L_27f8;

L_27f8:
    if ((HIWORD(lpth) == 0x0))
        goto L_294c;
    else
        goto L_27fe;

L_27fe:

L_2804:
    lpth->thp.iWarp = 0x0;
    lpth->thp.idPlanet = 0x3ff;
    lpth->pt.x = ppt->x;
    lpth->pt.y = ppt->y;
    goto L_2900;

L_2842:
    /* untranslated: ss:[bp-0x12] = ((loword((int32_t)(words((HIWORD(rgwtMinerals[i]) + 0x0), (LOWORD(rgwtMinerals[i]) + 0x9)) / 0xa)) + *(lpth+0xe)) & 0x3fff)
     */
    lpth->thp.wtMax = 0x0;
    /* untranslated: *(lpth+0xe) = (*(lpth+0xe) | ss:[bp-0x12]) */
    /* untranslated: HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] = (HIWORD(lpth):[((LOWORD(lpth) + 0x8) + (i * 0x2))] + LOWORD(rgwtMinerals[i])) */
    wtTotal = (wtTotal - rgwtMinerals[i]);
    rgwtMinerals[i] = 0x0;

L_2900:
    if ((HIWORD(wtTotal) > 0x0))
        goto L_291a;
    else
        goto L_2909;

L_2909:
    if ((HIWORD(wtTotal) < 0x0))
        goto L_2927;
    else
        goto L_290e;

L_290e:
    if ((LOWORD(wtTotal) <= 0x0))
        goto L_2927;
    else
        goto L_2914;

L_2914:

L_291a:
    i = (i + 0x1);

L_291e:
    if ((i < 0x3))
        goto L_2708;
    else
        goto L_2927;

L_2927:
    if ((HIWORD(wtTotal) > 0x0))
        goto L_2700;
    else
        goto L_2930;

L_2930:
    if ((HIWORD(wtTotal) < 0x0))
        goto L_293e;
    else
        goto L_2935;

L_2935:
    if ((LOWORD(wtTotal) > 0x0))
        goto L_2700;
    else
        goto L_293e;

L_293e:
    *(plpth) = lpth;

L_294c:
    return;
}

int16_t CplrBattle(FLEET *lpfl, uint16_t *rggrfAttack, uint16_t *pgrfPlayer, uint16_t *pgrfSpectator) {
    int16_t  iplrStarbase;
    FLEET   *lpflCur;
    int32_t  rgcsh[16];
    uint16_t grPlr;
    int16_t  iplrCur;
    PLANET  *lppl;
    int16_t  cplr;
    int16_t  i;
    int16_t  mdRel;
    uint8_t  rgctok[16];
    int16_t  fChange;
    uint16_t iplrAttack;
    int16_t  fAttack;
    int16_t  cshdef;
    int16_t  ishdef;
    int16_t  cflTotal;
    uint16_t grfPlayer;
    int16_t  ctokNew;
    int16_t  ctokFleet;

L_2952:
    fAttack = 0x0;
    iplrStarbase = 0xffff;
    grfPlayer = 0x0;
    cshdef = 0x0;
    grfMissed = 0x0;
    cflTotal = 0x0;
    *(pgrfSpectator) = 0x0;
    memset(rggrfAttack, 0x0, 0x20);
    memset(rgcsh, 0x0, 0x40);
    if ((lpfl->idPlanet == 0xffff))
        goto L_2b38;
    else
        goto L_29b5;

L_29b5:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->fStarbase == 0x0))
        goto L_2b1a;
    else
        goto L_29e1;

L_29e1:
    iplrStarbase = lppl->iPlayer;
    grfPlayer = (grfPlayer | (0x1 << iplrStarbase));
    iplrAttack = ((*(rglpbtlplan[iplrStarbase] + 0x2) >> 0x8) & 0x1f);
    if ((FHullHasTeeth(&(rglpshdefSB[iplrStarbase][lppl->isb])) == 0x0))
        goto L_2b38;
    else
        goto L_2a42;

L_2a42:
    if ((iplrAttack == 0x0))
        goto L_2b38;
    else
        goto L_2a4b;

L_2a4b:
    goto L_2afc;

L_2a51:
    /* untranslated: rggrfAttack[iplrCur] = (((0x1 << iplrStarbase) ~ 0x0) & 0xffff) */
    goto L_2b38;

L_2a6d:
    i = 0x0;
    goto L_2a79;

L_2a75:
    i = (i + 0x1);

L_2a79:
    if ((i >= game.cPlayer))
        goto L_2b38;
    else
        goto L_2a84;

L_2a84:
    if ((i == iplrStarbase))
        goto L_2a75;
    else
        goto L_2a8f;

L_2a8f:
    mdRel = (uint16_t)(rgplr[iplrStarbase].rgmdRelation[i]);
    if ((mdRel == 0x2))
        goto L_2ac5;
    else
        goto L_2ab3;

L_2ab3:
    if ((mdRel != 0x0))
        goto L_2a75;
    else
        goto L_2abc;

L_2abc:
    if ((iplrAttack != 0x2))
        goto L_2a75;
    else
        goto L_2ac5;

L_2ac5:
    rggrfAttack[iplrStarbase] = (rggrfAttack[iplrStarbase] | (0x1 << i));

L_2adf:
    rggrfAttack[iplrCur] = (rggrfAttack[iplrCur] | (0x1 << (iplrAttack + 0xfffc)));
    goto L_2b38;

L_2afc:
    if ((iplrAttack == 0x1))
        goto L_2a6d;
    else
        goto L_2b04;

L_2b04:
    if ((iplrAttack == 0x2))
        goto L_2a6d;
    else
        goto L_2b0c;

L_2b0c:
    if ((iplrAttack != 0x3))
        goto L_2adf;
    else
        goto L_2b11;

L_2b11:

L_2b1a:
    if ((lppl->iPlayer == 0xffff))
        goto L_2b38;
    else
        goto L_2b27;

L_2b27:
    *(pgrfSpectator) = (*(pgrfSpectator) | (0x1 << lppl->iPlayer));

L_2b38:
    lpflCur = lpfl;

L_2b44:
    if ((lpflCur->fDead != 0x0))
        goto LNextFleet;
    else
        goto L_2b58;

L_2b58:

L_2b5e:
    iplrCur = lpflCur->iPlayer;
    grfPlayer = (grfPlayer | (0x1 << iplrCur));
    if (((HIWORD(rglpbtlplan[lpflCur->iplr][lpflCur->iplan]) & 0xf) == 0x0))
        goto L_2d54;
    else
        goto L_2baf;

L_2baf:
    if ((((HIWORD(rglpbtlplan[lpflCur->iplr][lpflCur->iplan]) >> 0x8) & 0x1f) == 0x0))
        goto L_2d54;
    else
        goto L_2bf0;

L_2bf0:
    if ((FFleetHasTeeth(lpflCur) == 0x0))
        goto L_2d54;
    else
        goto L_2c06;

L_2c06:
    fAttack = 0x1;
    iplrAttack = ((HIWORD(rglpbtlplan[iplrCur][lpflCur->iplan]) >> 0x8) & 0x1f);
    if (((HIWORD(rglpbtlplan[lpflCur->iplr][lpflCur->iplan]) & 0xf) != 0x0))
        goto L_2c7d;
    else
        goto L_2c78;

L_2c78:
    iplrAttack = 0x0;

L_2c7d:
    goto L_2d31;

L_2c86:
    /* untranslated: rggrfAttack[iplrCur] = (((0x1 << iplrCur) ~ 0x0) & 0xffff) */
    goto L_2d54;

L_2ca2:
    i = 0x0;
    goto L_2cae;

L_2caa:
    i = (i + 0x1);

L_2cae:
    if ((i >= game.cPlayer))
        goto L_2d54;
    else
        goto L_2cb9;

L_2cb9:
    if ((i == iplrCur))
        goto L_2caa;
    else
        goto L_2cc4;

L_2cc4:
    mdRel = (uint16_t)(rgplr[iplrCur].rgmdRelation[i]);
    if ((mdRel == 0x2))
        goto L_2cfa;
    else
        goto L_2ce8;

L_2ce8:
    if ((mdRel != 0x0))
        goto L_2caa;
    else
        goto L_2cf1;

L_2cf1:
    if ((iplrAttack != 0x2))
        goto L_2caa;
    else
        goto L_2cfa;

L_2cfa:
    rggrfAttack[iplrCur] = (rggrfAttack[iplrCur] | (0x1 << i));

L_2d14:
    rggrfAttack[iplrCur] = (rggrfAttack[iplrCur] | (0x1 << (iplrAttack + 0xfffc)));
    goto L_2d54;

L_2d31:
    if ((iplrAttack == 0x0))
        goto L_2d54;
    else
        goto L_2d39;

L_2d39:
    if ((iplrAttack == 0x1))
        goto L_2ca2;
    else
        goto L_2d41;

L_2d41:
    if ((iplrAttack == 0x2))
        goto L_2ca2;
    else
        goto L_2d49;

L_2d49:
    if ((iplrAttack != 0x3))
        goto L_2d14;
    else
        goto L_2d4e;

L_2d4e:

L_2d54:
    lpflCur->fDone = 0x1;
    lpflCur->fInclude = 0x1;

LNextFleet:
    lpflCur = lpflCur->lpflNext;
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_2b44;
    else
        goto L_2d9b;

L_2d9b:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_2b44;
    else
        goto L_2da3;

L_2da3:
    if ((fAttack != 0x0))
        goto L_2db2;
    else
        goto L_2dac;

L_2dac:
    return 0x0;

L_2db2:
    iplrAttack = 0x0;
    i = 0x0;
    goto L_2dc3;

L_2dbf:
    i = (i + 0x1);

L_2dc3:
    if ((i >= game.cPlayer))
        goto L_2df5;
    else
        goto L_2dce;

L_2dce:
    if ((rggrfAttack[i] == 0x0))
        goto L_2dbf;
    else
        goto L_2de0;

L_2de0:
    iplrAttack = (iplrAttack | (grfPlayer & rggrfAttack[i]));

L_2df5:
    if ((iplrAttack != 0x0))
        goto L_2e04;
    else
        goto L_2dfe;

L_2dfe:
    return 0x0;

L_2e04:
    i = 0x0;
    goto L_2e10;

L_2e0c:
    i = (i + 0x1);

L_2e10:
    if ((i >= game.cPlayer))
        goto L_2e9d;
    else
        goto L_2e1b;

L_2e1b:
    if (((rggrfAttack[i] & iplrAttack) == 0x0))
        goto L_2e3d;
    else
        goto L_2e32;

L_2e32:
    iplrAttack = (iplrAttack | (0x1 << i));

L_2e3d:
    if ((((0x1 << i) & iplrAttack) == 0x0))
        goto L_2e0c;
    else
        goto L_2e50;

L_2e50:
    iplrCur = 0x0;
    goto L_2e5c;

L_2e58:
    iplrCur = (iplrCur + 0x1);

L_2e5c:
    if ((iplrCur >= game.cPlayer))
        goto L_2e0c;
    else
        goto L_2e67;

L_2e67:
    if ((((0x1 << i) & rggrfAttack[iplrCur]) == 0x0))
        goto L_2e58;
    else
        goto L_2e83;

L_2e83:
    rggrfAttack[i] = (rggrfAttack[i] | (0x1 << iplrCur));

L_2e9d:
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_2eb8;
    else
        goto L_2eab;

L_2eab:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_2eb8;
    else
        goto L_2eb3;

L_2eb3:
    fChange = 0x0;

L_2eb8:
    iplrCur = lpflCur->iPlayer;
    grPlr = (0x1 << iplrCur);
    if (((grfPlayer & grPlr) == 0x0))
        goto L_2fbc;
    else
        goto L_2edb;

L_2edb:
    if (((iplrAttack & grPlr) != 0x0))
        goto L_2fbc;
    else
        goto L_2ee9;

L_2ee9:
    rggrfAttack[iplrCur] = 0x0;
    i = 0x0;
    goto L_2f03;

L_2eff:
    i = (i + 0x1);

L_2f03:
    if ((i >= game.cPlayer))
        goto L_2f94;
    else
        goto L_2f0e;

L_2f0e:
    if ((i == iplrCur))
        goto L_2eff;
    else
        goto L_2f19;

L_2f19:
    if (((uint16_t)(rgplr[iplrCur].rgmdRelation[i]) != 0x1))
        goto L_2eff;
    else
        goto L_2f39;

L_2f39:
    if ((((0x1 << i) & iplrAttack) == 0x0))
        goto L_2eff;
    else
        goto L_2f4c;

L_2f4c:
    if ((((0x1 << i) & rggrfAttack[iplrCur]) == 0x0))
        goto L_2f79;
    else
        goto L_2f68;

L_2f68:
    rggrfAttack[iplrCur] = 0x0;
    goto L_2f94;

L_2f79:
    rggrfAttack[iplrCur] = (rggrfAttack[iplrCur] | rggrfAttack[i]);

L_2f94:
    if ((rggrfAttack[iplrCur] == 0x0))
        goto L_2faf;
    else
        goto L_2fa6;

L_2fa6:
    iplrAttack = (iplrAttack | grPlr);
    goto L_2fb7;

L_2faf:
    /* untranslated: grfPlayer = (grfPlayer & (grPlr ~ 0x0)) */

L_2fb7:
    fChange = 0x1;

L_2fbc:
    lpflCur = lpflCur->lpflNext;
    if ((fChange != 0x0))
        goto L_2e9d;
    else
        goto L_2fd6;

L_2fd6:
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_2e9d;
    else
        goto L_2fe4;

L_2fe4:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_2e9d;
    else
        goto L_2fec;

L_2fec:
    if ((iplrStarbase == 0xffff))
        goto L_3022;
    else
        goto L_2ff5;

L_2ff5:
    if ((((0x1 << iplrStarbase) & grfPlayer) == 0x0))
        goto L_3022;
    else
        goto L_3008;

L_3008:
    rgcsh[iplrStarbase] = 0x1;
    cshdef = 0x1;

L_3022:
    fChange = 0x0;

L_3027:
    iplrCur = lpflCur->iPlayer;
    grPlr = (0x1 << iplrCur);
    if (((grfPlayer & grPlr) == 0x0))
        goto L_30e8;
    else
        goto L_304a;

L_304a:
    ishdef = 0x0;
    goto L_30dc;

L_3052:
    if ((lpflCur->rgcsh[ishdef] == 0x0))
        goto L_30d8;
    else
        goto L_306f;

L_306f:
    if ((((*(LphuldefFromId(rglpshdef[iplrCur][ishdef].hul.ihuldef) + 0x7b) >> 0x6) & 0xf) == 0x0))
        goto L_30d4;
    else
        goto L_30ab;

L_30ab:
    rgcsh[iplrCur] = (rgcsh[iplrCur] + (uint32_t)(lpflCur->rgcsh[ishdef]));

L_30d4:
    cshdef = (cshdef + 0x1);

L_30d8:
    ishdef = (ishdef + 0x1);

L_30dc:
    if ((ishdef >= 0x10))
        goto L_3104;
    else
        goto L_30e2;

L_30e2:

L_30e8:
    *(pgrfSpectator) = (*(pgrfSpectator) | grPlr);
    lpflCur->fInclude = 0x0;

L_3104:
    lpflCur = lpflCur->lpflNext;
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_3027;
    else
        goto L_3123;

L_3123:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_3027;
    else
        goto L_312b;

L_312b:
    cplr = 0x0;
    goto L_314b;

L_3133:
    if (((iplrAttack & 0x1) == 0x0))
        goto L_3145;
    else
        goto L_3141;

L_3141:
    cplr = (cplr + 0x1);

L_3145:
    iplrAttack = (iplrAttack >> 0x1);

L_314b:
    if ((iplrAttack != 0x0))
        goto L_3133;
    else
        goto L_3154;

L_3154:
    if ((cshdef <= 0xff))
        goto L_3378;
    else
        goto L_315e;

L_315e:
    ctokNew = 0x0;
    i = (0xff0000 / cplr);
    if ((iplrStarbase == 0xffff))
        goto L_318d;
    else
        goto L_3176;

L_3176:
    if ((((0x1 << iplrStarbase) & grfPlayer) == 0x0))
        goto L_318d;
    else
        goto L_3189;

L_3189:
    ctokNew = (ctokNew + 0x1);

L_318d:
    memset(rgctok, 0x0, 0x10);

L_31a1:
    if ((lpflCur->fInclude == 0x0))
        goto L_3267;
    else
        goto L_31b8;

L_31b8:
    ctokFleet = 0x0;
    iplrCur = lpflCur->iPlayer;
    ishdef = 0x0;
    goto L_31f4;

L_31cf:
    if ((lpflCur->rgcsh[ishdef] == 0x0))
        goto L_31f0;
    else
        goto L_31ec;

L_31ec:
    ctokFleet = (ctokFleet + 0x1);

L_31f0:
    ishdef = (ishdef + 0x1);

L_31f4:
    if ((ishdef < 0x10))
        goto L_31cf;
    else
        goto L_31fd;

L_31fd:
    if (((rgctok[iplrCur] + ctokFleet) <= i))
        goto L_3254;
    else
        goto L_3215;

L_3215:
    lpflCur->fInclude = 0x0;
    lpflCur->fBombed = 0x1;
    lpflCur->fSkipped = 0x1;
    goto L_3267;

L_3254:
    rgctok[iplrCur] = (rgctok[iplrCur] + LOBYTE(ctokFleet));
    ctokNew = (ctokNew + ctokFleet);

L_3267:
    lpflCur = lpflCur->lpflNext;
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_31a1;
    else
        goto L_3286;

L_3286:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_31a1;
    else
        goto L_328e;

L_328e:
    if ((ctokNew >= 0xff))
        goto L_3378;
    else
        goto L_3298;

L_3298:
    if ((lpflCur->fSkipped == 0x0))
        goto L_3351;
    else
        goto L_32af;

L_32af:
    ctokFleet = 0x0;
    iplrCur = lpflCur->iPlayer;
    ishdef = 0x0;
    goto L_32eb;

L_32c6:
    if ((lpflCur->rgcsh[ishdef] == 0x0))
        goto L_32e7;
    else
        goto L_32e3;

L_32e3:
    ctokFleet = (ctokFleet + 0x1);

L_32e7:
    ishdef = (ishdef + 0x1);

L_32eb:
    if ((ishdef < 0x10))
        goto L_32c6;
    else
        goto L_32f4;

L_32f4:
    if (((ctokNew + ctokFleet) > 0xff))
        goto L_3351;
    else
        goto L_3302;

L_3302:
    lpflCur->fInclude = 0x1;
    lpflCur->fBombed = 0x0;
    lpflCur->fSkipped = 0x0;
    rgctok[iplrCur] = (rgctok[iplrCur] + LOBYTE(ctokFleet));
    ctokNew = (ctokNew + ctokFleet);

L_3351:
    lpflCur = lpflCur->lpflNext;
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_3298;
    else
        goto L_3370;

L_3370:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_3298;
    else
        goto L_3378;

L_3378:
    *(pgrfPlayer) = grfPlayer;
    if ((fChange == 0x0))
        goto L_338f;
    else
        goto L_3389;

L_3389:
    return 0xffff;

L_338f:
    return cplr;
}

int16_t SpdOfShip(FLEET *lpfl, int16_t ishdef, TOK *ptok, int16_t fDumpCargo, SHDEF *lpshdef) {
    int32_t  wtCargoFleetMax;
    int16_t  spd;
    int16_t  iWarp;
    uint16_t wt;
    int16_t  cHalfThruster;
    int16_t  cThruster;
    int32_t  wtFleetCargo;
    int16_t  j;
    int16_t  cEngineT;
    uint16_t wtCargoShdefMax;
    int16_t  iEngine;
    ENGINE  *lpengine;
    uint16_t t_merge_370c_0001;
    uint16_t t_merge_385a_0001;
    uint16_t t_merge_387e_0001;

L_339c:
    if ((LOWORD(lpshdef) != 0x0))
        goto L_33d8;
    else
        goto L_33ae;

L_33ae:
    if ((HIWORD(lpshdef) != 0x0))
        goto L_33d8;
    else
        goto L_33b7;

L_33b7:
    lpshdef = &(rglpshdef[lpfl->iPlayer][ishdef]);

L_33d8:
    iEngine = 0xffff;
    cHalfThruster = 0x0;
    cThruster = 0x0;
    j = 0x0;
    goto L_33f4;

L_33f0:
    j = (j + 0x1);

L_33f4:
    if ((j >= lpshdef->hul.chs))
        goto L_361f;
    else
        goto L_3406;

L_3406:
    /* untranslated: branch ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) == 0x0 ? L_33f0 : L_342d */

L_342d:

L_3433:
    goto L_35f9;

L_344f:
    /* untranslated: iEngine = (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) */
    /* untranslated: cEngineT = ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) */
    if ((iEngine != 0x8))
        goto L_33f0;
    else
        goto L_349d;

L_349d:
    /* untranslated: cHalfThruster = (cHalfThruster + ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff)) */

L_34c5:
    goto L_353a;

L_34e5:
    /* untranslated: cThruster = (cThruster + (((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff) * 0x2)) */
    goto L_33f0;

L_350f:
    /* untranslated: cThruster = (cThruster + ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff)) */
    goto L_33f0;

L_353a:
    /* untranslated: branch (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) == 0x7 ? L_350f : L_3542 */

L_3542:
    /* untranslated: branch (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x8 ? L_33f0 : L_3547 */

L_3547:

L_3550:
    goto L_359b;

L_3570:
    /* untranslated: cThruster = (cThruster + ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff)) */
    goto L_33f0;

L_359b:
    /* untranslated: branch (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x4 ? L_33f0 : L_35a0 */

L_35a0:

L_35a9:
    /* untranslated: branch (lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] & 0xff) != 0x6 ? L_33f0 : L_35ce */

L_35ce:
    /* untranslated: cHalfThruster = (cHalfThruster + ((lpshdef->hul.rgTech[part[12:0](rgcrPlrHistory[j])] >> 0x8) & 0xff)) */

L_35f9:
    if ((lpshdef->hul.rghs[j].grhst == hstEngine))
        goto L_344f;
    else
        goto L_3601;

L_3601:
    if ((lpshdef->hul.rghs[j].grhst == hstMining))
        goto L_35a9;
    else
        goto L_3609;

L_3609:
    if ((lpshdef->hul.rghs[j].grhst == hstSpecialE))
        goto L_3550;
    else
        goto L_3611;

L_3611:
    if ((lpshdef->hul.rghs[j].grhst != hstSpecialM))
        goto L_33f0;
    else
        goto L_3616;

L_3616:

L_361f:
    cThruster = (cThruster + ((uint32_t)((cHalfThruster + 0x1)) / 0x2));
    if ((iEngine == 0xffff))
        goto L_3640;
    else
        goto L_3637;

L_3637:
    if ((cEngineT != 0x0))
        goto L_3646;
    else
        goto L_3640;

L_3640:
    return 0x0;

L_3646:
    lpengine = LpengineFromId(iEngine);
    if ((iEngine == 0x7))
        goto L_3684;
    else
        goto L_3660;

L_3660:
    if ((iEngine == 0x8))
        goto L_3684;
    else
        goto L_3669;

L_3669:
    if ((iEngine == 0x9))
        goto L_3684;
    else
        goto L_3672;

L_3672:
    if ((iEngine == 0xe))
        goto L_3684;
    else
        goto L_367b;

L_367b:
    if ((iEngine != 0xf))
        goto L_368c;
    else
        goto L_3684;

L_3684:
    iWarp = 0xa;
    goto L_36c1;

L_368c:
    iWarp = 0x9;
    goto L_36b8;

L_3694:
    if ((lpengine->rgcFuelUsed[iWarp] <= 0x78))
        goto L_36c1;
    else
        goto L_36ae;

L_36ae:

L_36b4:
    iWarp = (iWarp - 0x1);

L_36b8:
    if ((iWarp > 0x0))
        goto L_3694;
    else
        goto L_36c1;

L_36c1:
    spd = ((iWarp + 0xfffc) + cThruster);
    if ((LOWORD(lpfl) != 0x0))
        goto L_36df;
    else
        goto L_36d6;

L_36d6:
    if ((HIWORD(lpfl) == 0x0))
        goto L_3711;
    else
        goto L_36df;

L_36df:
    if ((GetRaceStat(rgplr[lpfl->iPlayer], rsMajorAdv) != raAttack))
        goto L_3709;
    else
        goto L_3703;

L_3703:
    t_merge_370c_0001 = 0x1;
    goto L_370c;

L_3709:
    t_merge_370c_0001 = 0x0;

L_370c:
    spd = (spd + (t_merge_370c_0001 * 0x2));

L_3711:
    wt = lpshdef->hul.wtEmpty;
    if ((LOWORD(lpfl) != 0x0))
        goto L_372d;
    else
        goto L_3724;

L_3724:
    if ((HIWORD(lpfl) == 0x0))
        goto L_3809;
    else
        goto L_372d;

L_372d:
    wtCargoShdefMax = WtMaxShdefStat(lpshdef, 0x2);
    if ((wtCargoShdefMax == 0x0))
        goto L_37c7;
    else
        goto L_374b;

L_374b:
    wtCargoFleetMax = LGetFleetStat(lpfl, 0x2);
    wtFleetCargo = (((lpfl->rgwtMin[0x0] + lpfl->rgwtMin[0x1]) + lpfl->rgwtMin[0x2]) + lpfl->rgwtMin[0x3]);
    /* untranslated: wtFleetCargo = (int32_t)((uint32_t)(wtFleetCargo * words(0x0, wtCargoShdefMax)) / wtCargoFleetMax) */
    wt = (wt + LOWORD(wtFleetCargo));
    goto L_37cc;

L_37c7:
    fDumpCargo = 0x0;

L_37cc:
    if ((fDumpCargo == 0x0))
        goto L_37d9;
    else
        goto L_37d5;

L_37d5:
    spd = (spd - 0x1);

L_37d9:
    ptok->wFlags = ((ptok->wFlags & 0xc3ff) | ((Random(0xf) & 0xf) << 0xa));

L_3809:
    if ((LOWORD(ptok) != 0x0))
        goto L_381b;
    else
        goto L_3812;

L_3812:
    if ((HIWORD(ptok) == 0x0))
        goto L_3825;
    else
        goto L_381b;

L_381b:
    ptok->wt = wt;

L_3825:
    /* untranslated: spd = (spd - (words((words(wt, 0x0) / 0x46), 0x0) / ((HIWORD(lpshdef->hul.rghs[0x0]) >> 0x8) & 0xff))) */
    if ((0x8 >= spd))
        goto L_3857;
    else
        goto L_3851;

L_3851:
    t_merge_385a_0001 = 0x8;
    goto L_385a;

L_3857:
    t_merge_385a_0001 = spd;

L_385a:
    if ((0x0 <= t_merge_385a_0001))
        goto L_386a;
    else
        goto L_3864;

L_3864:
    t_merge_387e_0001 = 0x0;
    goto L_387e;

L_386a:
    if ((0x8 >= spd))
        goto L_387b;
    else
        goto L_3875;

L_3875:
    t_merge_387e_0001 = 0x8;
    goto L_387e;

L_387b:
    t_merge_387e_0001 = spd;

L_387e:
    spd = t_merge_387e_0001;
    return spd;
}

SHDEF *LpshdefFromTok(TOK *ptok) {
L_388e:
    if ((ptok->ishdef < 0x10))
        goto L_38e3;
    else
        goto L_38a9;

L_38a9:
    /* untranslated: return ptroff(rglpshdefSB[ptok->iplr], ss:[bp-0x4]) */

L_38e3:
    /* untranslated: ss:[bp-0x4] = loword((ptok->ishdef * 0x93)) */
    /* untranslated: return ptroff(rglpshdef[ptok->iplr], ss:[bp-0x4]) */
}

int16_t FCanKillTok(TOK *ptok1, TOK *ptok2) {
    int32_t lp1;
    int32_t lp2;

L_391e:
    LpshdefFromTok(ptok1);
    /* untranslated: LOWORD(lp1) = *(callresult(SHDEF *)+0x87) */
    /* untranslated: HIWORD(lp1) = *(callresult(SHDEF *)+0x89) */
    LpshdefFromTok(ptok2);
    /* untranslated: LOWORD(lp2) = *(callresult(SHDEF *)+0x87) */
    /* untranslated: HIWORD(lp2) = *(callresult(SHDEF *)+0x89) */
    if ((HIWORD(lp2) < HIWORD(lp1)))
        goto L_398f;
    else
        goto L_3979;

L_3979:
    if ((HIWORD(lp2) > HIWORD(lp1)))
        goto L_3986;
    else
        goto L_397e;

L_397e:
    if ((LOWORD(lp2) <= LOWORD(lp1)))
        goto L_398f;
    else
        goto L_3986;

L_3986:
    return 0x0;

L_398f:
    if (((HIWORD(lp2) & 0x7fff) > (HIWORD(lp1) & 0x7fff)))
        goto L_39c6;
    else
        goto L_39b1;

L_39b1:
    if (((HIWORD(lp2) & 0x7fff) < (HIWORD(lp1) & 0x7fff)))
        goto L_39bd;
    else
        goto L_39b6;

L_39b6:
    if (((LOWORD(lp2) & 0xf000) >= (LOWORD(lp1) & 0xf000)))
        goto L_39c6;
    else
        goto L_39bd;

L_39bd:
    return 0x1;

L_39c6:
    if (((LOWORD(lp2) & 0xff00) != (LOWORD(lp1) & 0xff00)))
        goto L_3a1a;
    else
        goto L_39e8;

L_39e8:
    if (((HIWORD(lp2) & 0x7fff) != (HIWORD(lp1) & 0x7fff)))
        goto L_3a1a;
    else
        goto L_39ef;

L_39ef:
    if ((ptok1->spd < ptok2->spd))
        goto L_3a1a;
    else
        goto L_3a14;

L_3a14:
    return 0x1;

L_3a1a:
    return 0x0;
}

void DoBattles(int16_t fPostMovement) {
    int16_t  cplr;
    int16_t  ifl;
    FLEET   *lpfl;
    uint16_t grfSpectator;
    uint16_t grfPlayer;
    uint16_t rggrfAttack[16];

L_3a26:
    LinkFleets(fPostMovement);
    vrgtok = LpAlloc(0x1d00, htMisc);
    vlpwtCargo = LpAlloc(0x200, htMisc);
    ifl = 0x0;
    goto L_3a74;

L_3a70:
    ifl = (ifl + 0x1);

L_3a74:
    if ((ifl >= cFleet))
        goto L_3b70;
    else
        goto L_3a7f;

L_3a7f:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_3aaf;
    else
        goto L_3aa7;

L_3aa7:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_3b70;
    else
        goto L_3aaf;

L_3aaf:
    lpfl->fBombed = 0x0;
    if ((lpfl->fDone != 0x0))
        goto L_3a70;
    else
        goto L_3ada;

L_3ada:
    if ((lpfl->fDead != 0x0))
        goto L_3a70;
    else
        goto L_3af1;

L_3af1:
    if ((LOWORD(lpfl->lpflNext) != 0x0))
        goto L_3b0b;
    else
        goto L_3afe;

L_3afe:
    if ((HIWORD(lpfl->lpflNext) == 0x0))
        goto L_3a70;
    else
        goto L_3b05;

L_3b05:

L_3b0b:
    cplr = CplrBattle(lpfl, rggrfAttack, &(grfPlayer), &(grfSpectator));
    goto L_3b5a;

L_3b2e:
    if ((FDoCoolBattle(lpfl, cplr, rggrfAttack, grfPlayer, grfSpectator) != 0x0))
        goto L_3a70;
    else
        goto L_3b4e;

L_3b4e:

L_3b5a:
    /* untranslated: branch callresult(int16_t) == 0xffff ? L_3a70 : L_3b62 */

L_3b62:
    /* untranslated: branch callresult(int16_t) != 0x0 ? L_3b2e : L_3b67 */

L_3b67:

L_3b70:
    FreeLp(&(vlpwtCargo), htMisc);
    FreeLp(&(vrgtok), htMisc);
    vlpwtCargo = 0x0;
    vrgtok = 0x0;
    if ((LOWORD(lpbBattleT) != 0x0))
        goto L_3bc4;
    else
        goto L_3bba;

L_3bba:
    if ((HIWORD(lpbBattleT) == 0x0))
        goto L_3bed;
    else
        goto L_3bc4;

L_3bc4:
    LOWORD(lpbBattleT) = 0xffff;
    FreeLp(&(lpbBattleT), htBattle);
    lpbBattleT = 0x0;

L_3bed:
    if ((LOWORD(lpbBattleCur) != 0x0))
        goto L_3c01;
    else
        goto L_3bf7;

L_3bf7:
    if ((HIWORD(lpbBattleCur) == 0x0))
        goto L_3c0a;
    else
        goto L_3c01;

L_3c01:
    LOWORD(lpbBattleCur) = 0xffff;

L_3c0a:
    DoBombing();
    return;
}

void RegenShield(TOK *ptok) {
    int32_t dpNew;
    int32_t dpOrig;

L_3c16:
    dpOrig = DpShieldOfShdef(LpshdefFromTok(ptok), ptok->iplr);
    if ((ptok->dpShield == 0x0))
        goto L_3cb3;
    else
        goto L_3c52;

L_3c52:

L_3c58:
    LOWORD(dpNew) = (ptok->dpShield + LOWORD((int32_t)((dpOrig / 0xa))));
    HIWORD(dpNew) = (0x0 + HIWORD((int32_t)((dpOrig / 0xa))));
    if ((HIWORD(dpNew) < HIWORD(dpOrig)))
        goto L_3ca6;
    else
        goto L_3c8d;

L_3c8d:
    if ((HIWORD(dpNew) > HIWORD(dpOrig)))
        goto L_3c9a;
    else
        goto L_3c92;

L_3c92:
    if ((LOWORD(dpNew) <= LOWORD(dpOrig)))
        goto L_3ca6;
    else
        goto L_3c9a;

L_3c9a:
    dpNew = dpOrig;

L_3ca6:
    ptok->dpShield = LOWORD(dpNew);

L_3cb3:
    return;
}

int16_t InitFromHuldef(HUL *lphul, int16_t *ppctBC) {
    int16_t ihs;
    int16_t i;
    int16_t pct;
    int16_t initBase;
    int16_t cbc;
    int16_t pctBC;
    PART    part;

L_3cba:
    pct = 0x0;
    cbc = 0x0;
    initBase = (*(LphuldefFromId(lphul->ihuldef) + 0x7b) & 0x3f);
    ihs = 0x0;
    goto L_3cf5;

L_3cf1:
    ihs = (ihs + 0x1);

L_3cf5:
    if ((ihs >= lphul->chs))
        goto L_3e37;
    else
        goto L_3d07;

L_3d07:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    if ((part.hs.cItem == 0x0))
        goto L_3cf1;
    else
        goto L_3d3a;

L_3d3a:

L_3d40:
    if (((part.hs.grhst & 0x800) == 0x0))
        goto L_3ddf;
    else
        goto L_3d4e;

L_3d4e:
    goto L_3dc1;

L_3d57:
    FLookupPart(&(part));
    cbc = (cbc + LOWORD(((part.hs.iItem + 0xfffc) * part.hs.cItem)));
    /* untranslated: pctBC = part[52:2](part.pcom) */
    i = 0x0;
    goto L_3d93;

L_3d8f:
    i = (i + 0x1);

L_3d93:
    if ((i >= part.hs.cItem))
        goto L_3cf1;
    else
        goto L_3da6;

L_3da6:
    /* untranslated: pct = (pct + (words(loword(((0x64 - pct) * pctBC)), signhiword(loword(((0x64 - pct) * pctBC)))) / 0x64)) */
    goto L_3d8f;

L_3dc1:
    if ((part.hs.iItem == 0x5))
        goto L_3d57;
    else
        goto L_3dc9;

L_3dc9:
    if ((part.hs.iItem == 0x6))
        goto L_3d57;
    else
        goto L_3dd1;

L_3dd1:
    if ((part.hs.iItem != 0x7))
        goto L_3cf1;
    else
        goto L_3dd6;

L_3dd6:

L_3ddf:
    if (((part.hs.grhst & 0x10) == 0x0))
        goto L_3cf1;
    else
        goto L_3ded;

L_3ded:
    if ((part.hs.iItem != 0x12))
        goto L_3cf1;
    else
        goto L_3dfb;

L_3dfb:
    pctBC = 0xa;
    i = 0x0;
    goto L_3e0c;

L_3e08:
    i = (i + 0x1);

L_3e0c:
    if ((i >= part.hs.cItem))
        goto L_3cf1;
    else
        goto L_3e1f;

L_3e1f:
    /* untranslated: pct = (pct + (words(loword(((0x64 - pct) * pctBC)), signhiword(loword(((0x64 - pct) * pctBC)))) / 0x64)) */
    goto L_3e08;

L_3e37:
    if ((ppctBC == 0x0))
        goto L_3e48;
    else
        goto L_3e40;

L_3e40:
    *(ppctBC) = pct;

L_3e48:
    initBase = (initBase + cbc);
    if ((initBase < 0x40))
        goto L_3e5c;
    else
        goto L_3e57;

L_3e57:
    initBase = 0x3f;

L_3e5c:

L_3e62:
    return initBase;
}

void CheckInitiative(TOK *ptok) {
    SHDEF  *lpshdef;
    int16_t pctBC;

L_3e68:
    lpshdef = LpshdefFromTok(ptok);
    idPlayer = ptok->iplr;
    ptok->initBase = LOBYTE(InitFromHuldef(&(lpshdef), &(pctBC)));
    idPlayer = 0xffff;
    ptok->pctBC = LOBYTE(pctBC);
    return;
}

void CheckWeapons(TOK *ptok, int16_t *pfDampeningField, uint8_t *pinit) {
    int16_t pctJam;
    int32_t ldp;
    int32_t pctBeamDef;
    int16_t ihs;
    int16_t initMac;
    int16_t init;
    int16_t dxyMax;
    int16_t i;
    int32_t pctCap;
    int16_t initMin;
    int32_t pctHit;
    SHDEF  *lpshdef;
    int16_t dxyLim;
    int16_t initBase;
    HUL    *lphul;
    int16_t dxyPart;
    PART    part;

L_3ec2:
    pctCap = 0x3e8;
    pctBeamDef = 0x3e8;
    pctHit = 0x2710;
    initBase = ptok->initBase;
    initMin = 0xffff;
    initMac = 0xffff;
    lpshdef = LpshdefFromTok(ptok);
    lphul = lpshdef;
    dxyMax = 0xffff;
    dxyLim = 0xffff;
    ldp = DpShieldOfShdef(lpshdef, ptok->iplr);
    ihs = 0x0;
    goto L_3f55;

L_3f51:
    ihs = (ihs + 0x1);

L_3f55:
    if ((ihs >= lphul->chs))
        goto L_4348;
    else
        goto L_3f67;

L_3f67:
    if (((lphul->rghs[ihs].grhst & 0x18be) == 0x0))
        goto L_3f51;
    else
        goto L_3f8b;

L_3f8b:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) == 0x0 ? L_3f51 : L_3fb5 */

L_3fb5:
    pctJam = 0x64;
    dxyPart = 0xffff;
    part.hs.grhst = lphul->rghs[ihs].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    if (((part.hs.grhst & 0x30) == 0x0))
        goto L_402d;
    else
        goto L_3ff0;

L_3ff0:
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    /* untranslated: init = (initBase + part[56:2](part.pcom)) */
    if ((init < 0x40))
        goto L_4032;
    else
        goto L_4025;

L_4025:
    init = 0x3f;

L_402d:
    init = 0xffff;

L_4032:
    goto L_4233;

L_4038:
    goto L_410c;
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    pctJam = (0x64 - part.pspecial->grAbility);
    goto L_4138;
    pctJam = 0x5a;
    goto L_4138;
    *(pfDampeningField) = 0x1;
    goto L_4138;
    ptok->fDetector = 0x1;
    goto L_4138;
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    i = part.hs.cItem;
    goto L_40fd;

L_40c9:
    /* untranslated: pctCap = (int32_t)((uint32_t)(pctCap * words((signhiword(part[52:2](part[4:4](part))) + 0x0), (part[52:2](part[4:4](part)) + 0x64))) /
     * 0x64) */
    i = (i - 0x1);

L_40fd:
    if ((i <= 0x0))
        goto L_426e;
    else
        goto L_4103;

L_4103:

L_410c:

L_4138:

L_413b:
    if ((part.hs.iItem != ispecialMBeamDeflector))
        goto L_426e;
    else
        goto L_4149;

L_4149:
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    i = part.hs.cItem;
    goto L_41b1;

L_4179:
    /* untranslated: pctBeamDef = (int32_t)((uint32_t)(pctBeamDef * words((0x0 - signhiword(part.pspecial->grAbility)), (0x64 - part.pspecial->grAbility))) /
     * 0x64) */
    i = (i - 0x1);

L_41b1:
    if ((i <= 0x0))
        goto L_426e;
    else
        goto L_41b7;

L_41b7:

L_41c0:
    if ((part.hs.iItem != iminingAlienMiner))
        goto L_426e;
    else
        goto L_41ce;

L_41ce:
    pctJam = 0x46;

L_41d6:
    if ((part.hs.iItem != iarmorMegaPolyShell))
        goto L_426e;
    else
        goto L_41e4;

L_41e4:
    pctJam = 0x50;

L_41ec:
    if ((part.hs.iItem != ishieldLangstonShell))
        goto L_426e;
    else
        goto L_41fa;

L_41fa:
    pctJam = 0x5f;

L_4202:
    dxyPart = part.pbeam->dRangeMax;
    goto L_426e;

L_420f:
    ptok->fTorp = 0x1;
    dxyPart = part.ptorp->dRangeMax;
    goto L_426e;

L_4233:
    if ((part.hs.grhst == hstShield))
        goto L_41ec;
    else
        goto L_423b;

L_423b:
    if ((part.hs.grhst == hstArmor))
        goto L_41d6;
    else
        goto L_4243;

L_4243:
    if ((part.hs.grhst == hstBeam))
        goto L_4202;
    else
        goto L_424b;

L_424b:
    if ((part.hs.grhst == hstTorp))
        goto L_420f;
    else
        goto L_4253;

L_4253:
    if ((part.hs.grhst == hstMining))
        goto L_41c0;
    else
        goto L_425b;

L_425b:
    if ((part.hs.grhst == hstSpecialE))
        goto L_4038;
    else
        goto L_4263;

L_4263:
    if ((part.hs.grhst == hstSpecialM))
        goto L_413b;
    else
        goto L_426e;

L_426e:
    if ((pctJam >= 0x64))
        goto L_42c5;
    else
        goto L_4277;

L_4277:
    i = part.hs.cItem;
    goto L_42bc;

L_4288:
    pctHit = (uint32_t)((pctHit * (uint32_t)(pctJam)));
    pctHit = (int32_t)((pctHit / 0x64));
    i = (i - 0x1);

L_42bc:
    if ((i > 0x0))
        goto L_4288;
    else
        goto L_42c5;

L_42c5:
    if ((dxyPart == 0xffff))
        goto L_3f51;
    else
        goto L_42ce;

L_42ce:
    if ((ptok->grobj != grobjPlanet))
        goto L_42e4;
    else
        goto L_42e0;

L_42e0:
    dxyPart = (dxyPart + 0x1);

L_42e4:
    if ((dxyMax < 0x0))
        goto L_42f8;
    else
        goto L_42ed;

L_42ed:
    if ((dxyMax <= dxyPart))
        goto L_42fe;
    else
        goto L_42f8;

L_42f8:
    dxyMax = dxyPart;

L_42fe:
    if ((dxyPart <= dxyLim))
        goto L_430f;
    else
        goto L_4309;

L_4309:
    dxyLim = dxyPart;

L_430f:
    pinit[init] = 0x1;
    if ((initMin == 0xffff))
        goto L_432e;
    else
        goto L_4323;

L_4323:
    if ((init >= initMin))
        goto L_4334;
    else
        goto L_432e;

L_432e:
    initMin = init;

L_4334:
    if ((init <= initMac))
        goto L_3f51;
    else
        goto L_433f;

L_433f:
    initMac = init;

L_4348:
    if ((LOWORD(pctHit) != 0x2710))
        goto L_435b;
    else
        goto L_4352;

L_4352:
    if ((HIWORD(pctHit) == 0x0))
        goto L_439f;
    else
        goto L_435b;

L_435b:
    /* untranslated: ptok->pctJam = lobyte((0x64 - loword((int32_t)(words((HIWORD(pctHit) + 0x0), (LOWORD(pctHit) + 0x32)) / 0x64)))) */
    if ((ptok->pctJam <= 0x5f))
        goto L_43a7;
    else
        goto L_4394;

L_4394:
    ptok->pctJam = 0x5f;

L_439f:
    ptok->pctJam = 0x0;

L_43a7:
    if ((ptok->grobj != grobjPlanet))
        goto L_43d0;
    else
        goto L_43b9;

L_43b9:
    /* untranslated: ptok->pctJam = (ptok->pctJam - lobyte((words(ptok->pctJam, signhiword(ptok->pctJam)) / 0x4))) */

L_43d0:
    if ((LOWORD(pctCap) != 0x3e8))
        goto L_43e3;
    else
        goto L_43da;

L_43da:
    if ((HIWORD(pctCap) == 0x0))
        goto L_441f;
    else
        goto L_43e3;

L_43e3:
    if ((HIWORD(pctCap) < 0x0))
        goto L_4405;
    else
        goto L_43ec;

L_43ec:
    if ((HIWORD(pctCap) > 0x0))
        goto L_43fb;
    else
        goto L_43f1;

L_43f1:
    if ((LOWORD(pctCap) <= 0x9f6))
        goto L_4405;
    else
        goto L_43fb;

L_43fb:
    pctCap = 0x9f6;

L_4405:
    ptok->pctCap = LOBYTE(LOWORD((int32_t)((pctCap / 0xa))));

L_441f:
    ptok->pctBeamDef = LOBYTE(LOWORD((int32_t)((pctBeamDef / 0xa))));
    *(ptok + 0x19) = ((*(ptok + 0x19) & 0xff0f) | ((dxyMax & 0xf) << 0x4));
    *(ptok + 0x19) = ((*(ptok + 0x19) & 0xfff0) | (dxyLim & 0xf));
    ptok->initMin = LOBYTE(initMin);
    ptok->initMac = LOBYTE(initMac);
    if ((0x0 != 0x0))
        goto L_44b4;
    else
        goto L_44ac;

L_44ac:
    if (((HIWORD(ldp) & 0xffff) == 0x0))
        goto L_44c0;
    else
        goto L_44b4;

L_44b4:
    ptok->dpShield = 0xffff;
    goto L_44cd;

L_44c0:
    ptok->dpShield = LOWORD(ldp);

L_44cd:
    return;
}

void RandomizeTokOrder() {
    TOK     tok;
    int16_t itokSwap;
    int16_t itok;

L_44d4:
    itokSwap = 0xffff;
    itok = 0x0;
    goto L_45a3;

L_44ea:
    itokSwap = (Random((vctok - itok)) + itok);
    if ((itokSwap == itok))
        goto L_459f;
    else
        goto L_450a;

L_450a:
    tok = vrgtok[itokSwap];
    vrgtok[itokSwap] = vrgtok[itok];
    vrgtok[itok] = tok;

L_459f:
    itok = (itok + 0x1);

L_45a3:
    if ((itok < vctok))
        goto L_44ea;
    else
        goto L_45ae;

L_45ae:
    return;
}

void InitializeBoard(FLEET *lpfl, int16_t ibrc, uint16_t grfPlayer, uint8_t *pinit, int16_t *pinitMin, int16_t *pinitMac) {
    int16_t   iplr;
    FLEET    *lpflCur;
    TOK      *ptok;
    int16_t   initMac;
    PLANET   *lppl;
    int16_t   fDampeningField;
    int16_t   initMin;
    uint16_t *lpwtCargoCur;
    TOK      *ptokT;
    uint8_t   mpiplrdibrc[16];
    int16_t   fDumpCargo;
    int16_t   ishdef;
    uint8_t   rgfTorp[16];
    int16_t   t_461c;
    uint16_t  t_merge_4b51_0001;
    uint16_t  t_merge_4bc2_0001;

L_45b4:
    initMin = 0xffff;
    initMac = 0xffff;
    fDampeningField = 0x0;
    lpwtCargoCur = vlpwtCargo;
    ishdef = 0x0;
    memset(mpiplrdibrc, 0xff, 0x10);
    iplr = 0x0;
    goto L_45fe;

L_45fa:
    iplr = (iplr + 0x1);

L_45fe:
    if ((iplr >= game.cPlayer))
        goto L_4630;
    else
        goto L_4609;

L_4609:
    if ((((0x1 << iplr) & grfPlayer) == 0x0))
        goto L_45fa;
    else
        goto L_461c;

L_461c:
    t_461c = ishdef;
    ishdef = (ishdef + 0x1);
    mpiplrdibrc[iplr] = LOBYTE(t_461c);

L_4630:
    memset(rgfTorp, 0x0, 0x10);
    lpflCur = lpfl;
    ptok = vrgtok;
    if ((lpfl->idPlanet == 0xffff))
        goto L_485e;
    else
        goto L_466a;

L_466a:
    lppl = LpplFromId(lpfl->idPlanet);
    iplr = lppl->iPlayer;
    if ((iplr == 0xffff))
        goto L_485e;
    else
        goto L_4692;

L_4692:
    if ((lppl->fStarbase == 0x0))
        goto L_485e;
    else
        goto L_46a9;

L_46a9:
    if ((((0x1 << iplr) & grfPlayer) == 0x0))
        goto L_485e;
    else
        goto L_46bc;

L_46bc:
    ptok->grobj = grobjPlanet;
    lppl->fNoHeal = 0x1;
    /* untranslated: ptok->brc = byte cs:[(mpiplrdibrc[iplr] + ibrc)] */
    ptok->id = lppl->id;
    ptok->iplr = LOBYTE(iplr);
    ptok->csh = 0x1;
    ptok->ishdef = LOBYTE((lppl->isb + 0x10));
    CheckInitiative(ptok);
    CheckWeapons(ptok, &(fDampeningField), pinit);
    rgfTorp[iplr] = (rgfTorp[iplr] | LOBYTE(ptok->fTorp));
    if ((ptok->initBase != 0xff))
        goto L_478d;
    else
        goto L_4776;

L_4776:
    ptok->mdTarget0 = 0x5;
    goto L_47a1;

L_478d:
    ptok->mdTarget0 = 0x3;

L_47a1:
    ptok->mdTarget1 = 0x1;
    ptok->mdTarget2 = 0x1;
    ptok->dv.dp = ((ptok->dv.dp & 0x7f) | ((lppl->pctDp & 0x1ff) << 0x7));
    ptok->mdTactic = 0x5;
    if ((((ptok->dv.dp >> 0x7) & 0x1ff) == 0x0))
        goto L_483d;
    else
        goto L_4829;

L_4829:
    ptok->dv.dp = ((ptok->dv.dp & 0xff80) | 0x64);

L_483d:
    ptok->spd = 0x0;
    ptok->wt = 0xffff;
    ptok = (ptok + 0x1);

L_485e:
    if ((lpflCur->fDead != 0x0))
        goto L_4a68;
    else
        goto L_4875;

L_4875:
    if ((lpflCur->fSkipped == 0x0))
        goto L_489f;
    else
        goto L_488c;

L_488c:
    grfMissed = (grfMissed | (0x1 << lpflCur->iPlayer));
    goto L_4a68;

L_489f:
    if ((lpflCur->fInclude == 0x0))
        goto L_4a68;
    else
        goto L_48b6;

L_48b6:
    lpflCur->fNoHeal = 0x1;
    iplr = lpflCur->iPlayer;
    fDumpCargo = FDumpCargo(lpflCur);
    ishdef = 0x0;
    goto L_4a5f;

L_48ed:
    if ((lpflCur->rgcsh[ishdef] == 0x0))
        goto L_4a5b;
    else
        goto L_490a;

L_490a:
    ptok->grobj = grobjFleet;
    /* untranslated: ptok->brc = byte cs:[(mpiplrdibrc[iplr] + ibrc)] */
    ptok->id = lpflCur->id;
    ptok->iplr = LOBYTE(lpflCur->iplr);
    ptok->ishdef = LOBYTE(ishdef);
    ptok->csh = lpflCur->rgcsh[ishdef];
    /* untranslated: ptok->dv.dp = HIWORD(lpflCur):[((LOWORD(lpflCur) + 0x2c) + (ishdef * 0x2))] */
    CheckInitiative(ptok);
    CheckWeapons(ptok, &(fDampeningField), pinit);
    rgfTorp[iplr] = (rgfTorp[iplr] | LOBYTE(ptok->fTorp));
    CheckTarget(ptok, lpflCur, ishdef);
    *(ptok + 0x19) = ((*(ptok + 0x19) & 0xf0ff) | ((SpdOfShip(lpflCur, ishdef, ptok, fDumpCargo, 0x0) & 0xf) << 0x8));
    ptok = (ptok + 0x1);
    if ((((uint32_t)((LOWORD(ptok) - LOWORD(vrgtok))) / 0x1d) > 0xff))
        goto LTooManyTokens;
    else
        goto L_4a55;

L_4a55:

L_4a5b:
    ishdef = (ishdef + 0x1);

L_4a5f:
    if ((ishdef < 0x10))
        goto L_48ed;
    else
        goto L_4a68;

L_4a68:
    lpflCur = lpflCur->lpflNext;
    if ((LOWORD(lpflCur) != LOWORD(lpfl)))
        goto L_485e;
    else
        goto L_4a87;

L_4a87:
    if ((HIWORD(lpflCur) != HIWORD(lpfl)))
        goto L_485e;
    else
        goto LTooManyTokens;

LTooManyTokens:
    vctok = ((uint32_t)((LOWORD(ptok) - LOWORD(vrgtok))) / 0x1d);
    RandomizeTokOrder();
    ptokT = vrgtok;
    goto L_4c7e;

L_4ac4:
    ptokT->fMoved = 0x1;
    ptokT->fActive = 0x1;
    if ((ptokT->initMin != 0xff))
        goto L_4b12;
    else
        goto L_4afe;

L_4afe:
    ptokT->mdTarget1 = 0x0;

L_4b12:
    if ((ptokT->dpShield == 0x0))
        goto L_4b4e;
    else
        goto L_4b1f;

L_4b1f:
    if ((GetRaceGrbit(rgplr[ptokT->iplr], ibitRaceRegeneratingShields) == 0x0))
        goto L_4b4e;
    else
        goto L_4b48;

L_4b48:
    t_merge_4b51_0001 = 0x1;
    goto L_4b51;

L_4b4e:
    t_merge_4b51_0001 = 0x0;

L_4b51:
    ptokT->wFlags = ((ptokT->wFlags & 0xfff7) | ((t_merge_4b51_0001 & 0x1) << 0x3));
    if ((fDampeningField == 0x0))
        goto L_4be6;
    else
        goto L_4b7e;

L_4b7e:
    if ((ptokT->grobj == grobjPlanet))
        goto L_4be6;
    else
        goto L_4b90;

L_4b90:
    if (((ptokT->spd + 0xfffc) <= 0x0))
        goto L_4bbf;
    else
        goto L_4baa;

L_4baa:
    t_merge_4bc2_0001 = (ptokT->spd + 0xfffc);
    goto L_4bc2;

L_4bbf:
    t_merge_4bc2_0001 = 0x0;

L_4bc2:
    *(ptokT + 0x19) = ((*(ptokT + 0x19) & 0xf0ff) | ((t_merge_4bc2_0001 & 0xf) << 0x8));

L_4be6:
    lpbBattleCur = *(ptokT);
    lpbBattleCur = (lpbBattleCur + 0x1d);
    if ((ptokT->initMin == 0xff))
        goto L_4c40;
    else
        goto L_4c18;

L_4c18:
    if ((initMin == 0xffff))
        goto L_4c33;
    else
        goto L_4c21;

L_4c21:
    if ((ptokT->initMin >= initMin))
        goto L_4c40;
    else
        goto L_4c33;

L_4c33:
    initMin = ptokT->initMin;

L_4c40:
    if ((ptokT->initMac == 0xff))
        goto L_4c7a;
    else
        goto L_4c52;

L_4c52:
    if ((initMac == 0xffff))
        goto L_4c6d;
    else
        goto L_4c5b;

L_4c5b:
    if ((ptokT->initMac <= initMac))
        goto L_4c7a;
    else
        goto L_4c6d;

L_4c6d:
    initMac = ptokT->initMac;

L_4c7a:
    ptokT = (ptokT + 0x1);

L_4c7e:
    if ((LOWORD(ptokT) < LOWORD(ptok)))
        goto L_4ac4;
    else
        goto L_4c8c;

L_4c8c:
    *(pinitMin) = (initMin & 0xff);
    *(pinitMac) = (initMac & 0xff);
    return;
}

int16_t DzFromBrcBrc(uint8_t brc1, uint8_t brc2) {
    int16_t dy;
    int16_t dx;

L_4ca8:
    dx = ((brc1 & 0xf) - (brc2 & 0xf));
    dx = abs(dx);
    dy = ((brc1 >> 0x4) - (brc2 >> 0x4));
    dy = abs(dy);
    if ((dx <= dy))
        goto L_4d22;
    else
        goto L_4d1c;

L_4d1c:
    return dx;

L_4d22:
    return dy;
}

int32_t DpFromPtokBrcToBrc(TOK *ptok, uint8_t brcSrc, uint8_t brcTarget, TOK *ptokTarget, int16_t fProximity) {
    int16_t  dz;
    int32_t  dpMax;
    int32_t  dpShdef;
    int16_t  ihs;
    int32_t  cTorpBase;
    int32_t  dpTotal;
    int16_t  fOutOfRange;
    int32_t  dRange;
    HUL     *lphul;
    int32_t  cTorpHit;
    int32_t  dp;
    PART     part;
    int32_t  dpShieldsLeft;
    uint32_t t_merge_4e5b_0001_wide;
    uint16_t t_merge_4e8b_0001;

L_4d2e:
    dz = DzFromBrcBrc(brcSrc, brcTarget);
    dpTotal = 0x0;
    if ((fProximity != 0x0))
        goto L_4d7e;
    else
        goto L_4d63;

L_4d63:
    if ((dz <= ptok->dxyLim))
        goto L_4d7e;
    else
        goto L_4d75;

L_4d75:
    return 0x0;

L_4d7e:
    lphul = LpshdefFromTok(ptok);
    ihs = 0x0;
    goto L_4d9e;

L_4d9a:
    ihs = (ihs + 0x1);

L_4d9e:
    if ((ihs >= lphul->chs))
        goto L_51cb;
    else
        goto L_4db0;

L_4db0:
    if (((lphul->rghs[ihs].grhst & 0x30) == 0x0))
        goto L_4d9a;
    else
        goto L_4dd4;

L_4dd4:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) == 0x0 ? L_4d9a : L_4dfe */

L_4dfe:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    if ((ptok->grobj != grobjPlanet))
        goto L_4e58;
    else
        goto L_4e52;

L_4e52:
    t_merge_4e5b_0001_wide = 0x10001;
    goto L_4e5b;

L_4e58:
    t_merge_4e5b_0001_wide = 0x0;

L_4e5b:
    /* untranslated: LOWORD(dRange) = (loword(t_merge_4e5b_0001_wide) + part[52:2](part.pcom)) */
    /* untranslated: HIWORD(dRange) = signhiword((hiword(t_merge_4e5b_0001_wide) + part[52:2](part.pcom))) */
    if ((HIWORD(dRange) > SIGNHIWORD(dz)))
        goto L_4e88;
    else
        goto L_4e75;

L_4e75:
    if ((HIWORD(dRange) < SIGNHIWORD(dz)))
        goto L_4e82;
    else
        goto L_4e7a;

L_4e7a:
    if ((LOWORD(dRange) >= dz))
        goto L_4e88;
    else
        goto L_4e82;

L_4e82:
    t_merge_4e8b_0001 = 0x1;
    goto L_4e8b;

L_4e88:
    t_merge_4e8b_0001 = 0x0;

L_4e8b:
    fOutOfRange = t_merge_4e8b_0001;
    if ((fOutOfRange == 0x0))
        goto L_4ea3;
    else
        goto L_4e97;

L_4e97:
    if ((fProximity == 0x0))
        goto L_4d9a;
    else
        goto L_4e9d;

L_4e9d:

L_4ea3:
    /* untranslated: dp = (uint32_t)(sext16to32(part[54:2](part.pcom)) * words(0x0, part.hs.cItem)) */
    goto L_51b5;

L_4ece:
    if ((ptok->pctCap == 0x0))
        goto L_4f0d;
    else
        goto L_4ee0;

L_4ee0:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptok->pctCap), ptok->pctCap)) / 0x64) */

L_4f0d:
    if ((dz <= 0x0))
        goto L_4f60;
    else
        goto L_4f16;

L_4f16:
    if ((HIWORD(dRange) < 0x0))
        goto L_4f60;
    else
        goto L_4f1f;

L_4f1f:
    if ((HIWORD(dRange) > 0x0))
        goto L_4f2d;
    else
        goto L_4f24;

L_4f24:
    if ((LOWORD(dRange) <= 0x0))
        goto L_4f60;
    else
        goto L_4f2d;

L_4f2d:
    dp = (dp - (int32_t)(((int32_t)(((uint32_t)((dp * (uint32_t)(dz))) / 0xa)) / dRange)));

L_4f60:
    if ((ptokTarget->pctBeamDef >= 0x64))
        goto L_4f9f;
    else
        goto L_4f72;

L_4f72:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptokTarget->pctBeamDef), ptokTarget->pctBeamDef)) / 0x64) */

L_4f9f:
    if (((part.pbeam->grfAbilities & 0x1) == 0x0))
        goto L_4ffb;
    else
        goto L_4fb1;

L_4fb1:
    /* untranslated: dpShieldsLeft = (uint32_t)(words(0x0, ptokTarget->dpShield) * words(0x0, ptok->csh)) */
    if ((HIWORD(dp) < HIWORD(dpShieldsLeft)))
        goto L_4ffb;
    else
        goto L_4fe2;

L_4fe2:
    if ((HIWORD(dp) > HIWORD(dpShieldsLeft)))
        goto L_4fef;
    else
        goto L_4fe7;

L_4fe7:
    if ((LOWORD(dp) <= LOWORD(dpShieldsLeft)))
        goto L_4ffb;
    else
        goto L_4fef;

L_4fef:
    dp = dpShieldsLeft;

L_4ffb:
    if ((fOutOfRange == 0x0))
        goto L_505b;
    else
        goto L_5004;

L_5004:
    /* untranslated: dp = (int32_t)(dp / words((signhiword((dz + 0xa)) - HIWORD(dRange)), ((dz + 0xa) - LOWORD(dRange)))) */
    if ((HIWORD(dp) > 0x0))
        goto L_505b;
    else
        goto L_503a;

L_503a:
    if ((HIWORD(dp) < 0x0))
        goto L_5047;
    else
        goto L_503f;

L_503f:
    if ((LOWORD(dp) >= part.hs.cItem))
        goto L_505b;
    else
        goto L_5047;

L_5047:
    LOWORD(dp) = part.hs.cItem;
    HIWORD(dp) = 0x0;

L_505b:
    /* untranslated: dpTotal = (dpTotal + (uint32_t)(dp * words(0x0, ptok->csh))) */
    goto L_4d9a;

L_507b:
    /* untranslated: cTorpBase = (uint32_t)((uint32_t)(words(0x0, part.hs.cItem) * words(0x0, ptok->csh)) * 0xc8) */
    cTorpHit = CTorpHit(cTorpBase, ptokTarget, part.ptorp->dHitChance, ptok->pctBC);
    dp = (int32_t)(((uint32_t)(((uint32_t)(part.ptorp->dp) * cTorpHit)) / 0xc8));
    if ((ptokTarget->dpShield <= 0x0))
        goto L_5146;
    else
        goto L_5114;

L_5114:
    /* untranslated: dp = (dp + (int32_t)((uint32_t)(words((HIWORD(cTorpBase) - HIWORD(cTorpHit)), (LOWORD(cTorpBase) - LOWORD(cTorpHit))) *
     * sext16to32(part.ptorp->dp)) / 0x640)) */

L_5146:
    if ((fOutOfRange == 0x0))
        goto L_51a6;
    else
        goto L_514f;

L_514f:
    /* untranslated: dp = (int32_t)(dp / words((signhiword((dz + 0xa)) - HIWORD(dRange)), ((dz + 0xa) - LOWORD(dRange)))) */
    if ((HIWORD(dp) > 0x0))
        goto L_51a6;
    else
        goto L_5185;

L_5185:
    if ((HIWORD(dp) < 0x0))
        goto L_5192;
    else
        goto L_518a;

L_518a:
    if ((LOWORD(dp) >= part.hs.cItem))
        goto L_51a6;
    else
        goto L_5192;

L_5192:
    LOWORD(dp) = part.hs.cItem;
    HIWORD(dp) = 0x0;

L_51a6:
    dpTotal = (dpTotal + dp);
    goto L_4d9a;

L_51b5:
    if ((part.hs.grhst == hstBeam))
        goto L_4ece;
    else
        goto L_51bd;

L_51bd:
    if ((part.hs.grhst != hstTorp))
        goto L_4d9a;
    else
        goto L_51c2;

L_51c2:

L_51cb:
    LOWORD(dpShdef) = LpshdefFromTok(ptokTarget)->hul.dp;
    HIWORD(dpShdef) = 0x0;
    /* untranslated: dpMax = (uint32_t)(words((0x0 + HIWORD(dpShdef)), (ptokTarget->dpShield + LOWORD(dpShdef))) * words(0x0, ptokTarget->csh)) */
    if ((ptokTarget->dv.dp == 0x0))
        goto L_52d3;
    else
        goto L_5220;

L_5220:
    if ((HIWORD(dpMax) < 0x0))
        goto L_52d3;
    else
        goto L_5229;

L_5229:
    if ((HIWORD(dpMax) > 0x0))
        goto L_5237;
    else
        goto L_522e;

L_522e:
    if ((LOWORD(dpMax) <= 0x0))
        goto L_52d3;
    else
        goto L_5237;

L_5237:
    /* untranslated: dpMax = (dpMax - (int32_t)((uint32_t)((int32_t)((uint32_t)((int32_t)((uint32_t)(dpShdef * words(0x0, ((ptokTarget->dv.dp >> 0x7) & 0x1ff)))
     * / 0xa) * words(0x0, (ptokTarget->dv.dp & 0x7f))) / 0xa) * words(0x0, ptokTarget->csh)) / 0x1f4)) */
    if ((HIWORD(dpMax) > 0x0))
        goto L_52d3;
    else
        goto L_52bb;

L_52bb:
    if ((HIWORD(dpMax) < 0x0))
        goto L_52c9;
    else
        goto L_52c0;

L_52c0:
    if ((LOWORD(dpMax) > 0x0))
        goto L_52d3;
    else
        goto L_52c9;

L_52c9:
    dpMax = 0x1;

L_52d3:
    if ((HIWORD(dpTotal) < HIWORD(dpMax)))
        goto L_5303;
    else
        goto L_52e1;

L_52e1:
    if ((HIWORD(dpTotal) > HIWORD(dpMax)))
        goto L_52ee;
    else
        goto L_52e6;

L_52e6:
    if ((LOWORD(dpTotal) <= LOWORD(dpMax)))
        goto L_5303;
    else
        goto L_52ee;

L_52ee:
    if ((fProximity != 0x0))
        goto L_5303;
    else
        goto L_52f7;

L_52f7:
    dpTotal = dpMax;

L_5303:
    return dpTotal;
}

int16_t DzMoveRangeToConsider(TOK *ptok, uint16_t grfAttack, uint8_t *pbrc) {
    int16_t  dzNonSapper;
    uint8_t  dz;
    int16_t  iplr;
    uint16_t mdTarget;
    uint8_t  dzBest;
    int16_t  itokLook;
    int16_t  iplrTarget;
    TOK     *ptokTarget;
    int16_t  dzMax;
    uint8_t  brcCur;
    int16_t  ihs;
    SHDEF   *lpshdef;
    HUL     *lphul;
    PART     part;
    uint16_t t_merge_537b_0001;

L_5312:
    brcCur = ptok->brc;
    dzMax = (ptok->dxyLim + ptok->dMovesLeft);
    if ((FDoesPrimaryTargetTypeExist(ptok, grfAttack) == 0x0))
        goto L_5369;
    else
        goto L_535c;

L_535c:
    t_merge_537b_0001 = ptok->mdTarget1;
    goto L_537b;

L_5369:
    t_merge_537b_0001 = ptok->mdTarget2;

L_537b:
    mdTarget = t_merge_537b_0001;
    iplr = ptok->iplr;
    dzBest = 0xa;
    *(pbrc) = 0xff;
    if ((ptok->dxyLim != 0x3))
        goto L_54a6;
    else
        goto L_53a7;

L_53a7:
    dzNonSapper = 0xffff;
    lpshdef = LpshdefFromTok(ptok);
    lphul = lpshdef;
    ihs = 0x0;
    goto L_53d8;

L_53d4:
    ihs = (ihs + 0x1);

L_53d8:
    if ((ihs >= lphul->chs))
        goto L_54b3;
    else
        goto L_53ea;

L_53ea:
    if ((lphul->rghs[ihs].grhst != hstBeam))
        goto L_53d4;
    else
        goto L_5409;

L_5409:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) == 0x0 ? L_53d4 : L_5433 */

L_5433:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    idPlayer = ptok->iplr;
    FLookupPart(&(part));
    idPlayer = 0xffff;
    /* untranslated: branch (part[58:2](part.pcom) & 0x1) != 0x0 ? L_53d4 : L_5487 */

L_5487:
    /* untranslated: branch part[52:2](part.pcom) <= dzNonSapper ? L_53d4 : L_5496 */

L_5496:
    /* untranslated: dzNonSapper = part[52:2](part.pcom) */

L_54a6:
    dzNonSapper = ptok->dxyLim;

L_54b3:
    if ((ptok->dxyMax >= ptok->dxyLim))
        goto L_552a;
    else
        goto L_54d6;

L_54d6:
    if ((ptok->mdTactic == 0x5))
        goto L_5504;
    else
        goto L_54ed;

L_54ed:
    if ((ptok->mdTactic != 0x3))
        goto L_552a;
    else
        goto L_5504;

L_5504:
    dzMax = (ptok->dxyMax + ptok->dMovesLeft);

L_552a:
    ptokTarget = vrgtok;
    itokLook = 0x0;
    goto L_5552;

L_553f:
    itokLook = (itokLook + 0x1);
    ptokTarget = (ptokTarget + 0x1);

L_5552:
    if ((itokLook >= vctok))
        goto L_56cc;
    else
        goto L_555d;

L_555d:
    iplrTarget = ptokTarget->iplr;
    if ((iplrTarget == iplr))
        goto L_553f;
    else
        goto L_5575;

L_5575:
    if ((((0x1 << iplrTarget) & grfAttack) == 0x0))
        goto L_553f;
    else
        goto L_5585;

L_5585:

L_558b:
    if ((ptokTarget->fActive == 0x0))
        goto L_553f;
    else
        goto L_559d;

L_559d:
    if ((FIsTargetOfMdTarget(ptokTarget, mdTarget) == 0x0))
        goto L_553f;
    else
        goto L_55b4;

L_55b4:

L_55ba:
    dz = LOBYTE(DzFromBrcBrc(brcCur, ptokTarget->brc));
    if ((ptokTarget->dMovesLeft < ptok->dMovesLeft))
        goto L_5600;
    else
        goto L_55fc;

L_55fc:
    dz = (dz + 0x1);

L_5600:
    if ((dz > dzMax))
        goto L_5669;
    else
        goto L_560e;

L_560e:
    if ((ptokTarget->dpShield > 0x0))
        goto L_564e;
    else
        goto L_561b;

L_561b:
    if ((dzNonSapper == ptok->dxyLim))
        goto L_564e;
    else
        goto L_562d;

L_562d:
    if ((dz > (dzNonSapper + ptok->dMovesLeft)))
        goto L_5669;
    else
        goto L_564e;

L_564e:
    *(pbrc) = 0xff;
    return ptok->dMovesLeft;

L_5669:
    if ((dz >= dzBest))
        goto L_553f;
    else
        goto L_5682;

L_5682:
    if ((HIWORD(DpFromPtokBrcToBrc(ptok, 0x0, 0x0, ptokTarget, 0x0)) < 0x0))
        goto L_553f;
    else
        goto L_56aa;

L_56aa:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_56b7 : L_56af */

L_56af:
    /* untranslated: branch loword(callresult(int32_t)) <= 0x0 ? L_553f : L_56b7 */

L_56b7:
    dzBest = dz;
    *(pbrc) = ptokTarget->brc;

L_56cc:
    return 0x1;
}

int16_t FDoesPrimaryTargetTypeExist(TOK *ptok, uint16_t grfAttack) {
    uint16_t mdTarget;
    int16_t  iplr;
    int16_t  iplrLook;
    TOK      tok;
    int16_t  itokLook;

L_56d8:
    iplr = ptok->iplr;
    mdTarget = ptok->mdTarget1;
    if ((mdTarget != 0x0))
        goto L_570a;
    else
        goto L_5704;

L_5704:
    return 0x0;

L_570a:
    itokLook = 0x0;
    goto L_5862;

L_5712:
    iplrLook = vrgtok[itokLook].iplr;
    if ((iplrLook == iplr))
        goto L_585e;
    else
        goto L_5739;

L_5739:
    if ((((0x1 << iplrLook) & grfAttack) == 0x0))
        goto L_585e;
    else
        goto L_5749;

L_5749:

L_574f:
    tok = vrgtok[itokLook];
    if ((tok.fActive == 0x0))
        goto L_585e;
    else
        goto L_577e;

L_577e:

L_5784:
    goto L_583c;
    goto L_57f1;

L_5790:
    if ((tok.mdTarget0 != mdTarget))
        goto L_585e;
    else
        goto L_57a0;

L_57a0:

L_57a9:
    if ((tok.mdTarget0 < mdTarget))
        goto L_585e;
    else
        goto L_57b9;

L_57b9:

L_57c2:
    if ((tok.mdTarget0 == 0x4))
        goto L_581c;
    else
        goto L_57d5;

L_57d5:
    if ((tok.mdTarget0 != 0x7))
        goto L_585e;
    else
        goto L_57e5;

L_57e5:

L_57f1:
    if ((mdTarget == 0x3))
        goto L_5790;
    else
        goto L_57f9;

L_57f9:
    if ((mdTarget == 0x4))
        goto L_57c2;
    else
        goto L_5801;

L_5801:
    if ((mdTarget == 0x5))
        goto L_57a9;
    else
        goto L_5809;

L_5809:
    if ((mdTarget == 0x6))
        goto L_5790;
    else
        goto L_5811;

L_5811:
    if ((mdTarget == 0x7))
        goto L_5790;
    else
        goto L_581c;

L_581c:
    goto L_5873;
    if ((tok.grobj != grobjPlanet))
        goto L_585e;
    else
        goto L_5830;

L_5830:
    return 0x1;

L_583c:

L_585e:
    itokLook = (itokLook + 0x1);

L_5862:
    if ((itokLook < vctok))
        goto L_5712;
    else
        goto L_586d;

L_586d:
    return 0x0;

L_5873:
}

int16_t FIsTargetOfMdTarget(TOK *ptok, int16_t mdTarget) {
    uint16_t t_merge_5985_0001;

L_587a:
    goto L_5964;

L_5889:
    goto L_5985;
    goto L_5985;
    if ((ptok->grobj != grobjPlanet))
        goto L_58ad;
    else
        goto L_58a7;

L_58a7:
    goto L_58b0;

L_58ad:

L_58b0:
    goto L_5985;
    if ((ptok->mdTarget0 == 0x4))
        goto L_58e1;
    else
        goto L_58ca;

L_58ca:
    if ((ptok->mdTarget0 != 0x7))
        goto L_58e7;
    else
        goto L_58e1;

L_58e1:
    goto L_58ea;

L_58e7:

L_58ea:
    goto L_5985;
    if ((ptok->mdTarget0 != mdTarget))
        goto L_590a;
    else
        goto L_5904;

L_5904:
    goto L_590d;

L_590a:

L_590d:
    goto L_5985;
    if ((ptok->mdTarget0 == 0x5))
        goto L_5955;
    else
        goto L_5927;

L_5927:
    if ((ptok->mdTarget0 == 0x7))
        goto L_5955;
    else
        goto L_593e;

L_593e:
    if ((ptok->mdTarget0 != 0x6))
        goto L_595b;
    else
        goto L_5955;

L_5955:
    t_merge_5985_0001 = 0x1;
    goto L_5985;

L_595b:
    t_merge_5985_0001 = 0x0;

L_5964:
    if ((mdTarget > 0x7))
        goto L_5889;
    else
        goto L_596c;

L_596c:
    t_merge_5985_0001 = (mdTarget * 0x2);
    goto L_ffffffff;

L_5985:
    return t_merge_5985_0001;
}

int32_t ScoreGuessBattleDamage(TOK *ptokSrc, uint8_t brc, int16_t fPrimary, uint16_t grfAttack) {
    int16_t  iBest;
    int16_t  dMoves;
    int16_t  rgy[2];
    TOK     *ptok;
    int16_t  yEnemy;
    int16_t  dzEnemy;
    int32_t  dpGivenBest;
    int32_t  dpTakenBest;
    int16_t  y;
    int32_t  dpTakenTotal;
    int32_t  dpGivenCur;
    int16_t  i;
    int16_t  xEnemy;
    int16_t  yCur;
    int32_t  dpTaken;
    int32_t  scoreThemBest;
    int32_t  scoreThem;
    int16_t  dzCur;
    int16_t  rgx[2];
    uint8_t  brcEnemy;
    int32_t  dpGiven;
    int16_t  dMax;
    int32_t  scoreUs;
    uint8_t  iplrSrc;
    int16_t  fWeAttack;
    int16_t  xCur;
    int16_t  x;
    int16_t  dMin;
    int16_t  itok;
    uint16_t t_merge_5aa7_0001;
    uint16_t t_merge_5adb_0001;
    uint16_t t_merge_5b63_0001;
    uint16_t t_merge_5bc5_0001;
    uint16_t t_merge_5bfb_0001;
    uint16_t t_merge_5c7a_0001;
    uint16_t t_merge_5d12_0001;

L_598c:
    iplrSrc = ptokSrc->iplr;
    xCur = (ptokSrc->brc & 0xf);
    yCur = (ptokSrc->brc >> 0x4);
    dpGivenBest = 0x0;
    dpTakenTotal = 0x0;
    ptok = vrgtok;
    itok = 0x0;
    goto L_5a01;

L_59f0:
    ptok = (ptok + 0x1);
    itok = (itok + 0x1);

L_5a01:
    if ((itok >= vctok))
        goto L_5dfa;
    else
        goto L_5a0c;

L_5a0c:
    if ((ptok->fActive == 0x0))
        goto L_59f0;
    else
        goto L_5a1e;

L_5a1e:
    if ((iplrSrc == ptok->iplr))
        goto L_59f0;
    else
        goto L_5a3d;

L_5a3d:
    if ((((0x1 << ptok->iplr) & grfAttack) == 0x0))
        goto L_59f0;
    else
        goto L_5a56;

L_5a56:

L_5a5c:
    dzCur = DzFromBrcBrc(ptok->brc, brc);
    if ((ptok->dMovesLeft < ptokSrc->dMovesLeft))
        goto L_5aa4;
    else
        goto L_5a9e;

L_5a9e:
    t_merge_5aa7_0001 = 0x1;
    goto L_5aa7;

L_5aa4:
    t_merge_5aa7_0001 = 0x0;

L_5aa7:
    dMoves = t_merge_5aa7_0001;
    if ((dMoves != 0x0))
        goto L_5abf;
    else
        goto L_5ab3;

L_5ab3:
    dMax = dzCur;
    dMin = dzCur;
    goto L_5c52;

L_5abf:
    if ((0x0 <= (dzCur - dMoves)))
        goto L_5ad5;
    else
        goto L_5acf;

L_5acf:
    t_merge_5adb_0001 = 0x0;
    goto L_5adb;

L_5ad5:
    t_merge_5adb_0001 = (dzCur - dMoves);

L_5adb:
    dMin = t_merge_5adb_0001;
    xEnemy = (ptok->brc & 0xf);
    yEnemy = (ptok->brc >> 0x4);
    rgx[0x0] = (xEnemy - dMoves);
    rgx[0x1] = (xEnemy + dMoves);
    rgy[0x0] = (yEnemy - dMoves);
    rgy[0x1] = (yEnemy + dMoves);
    dMax = dzCur;
    x = 0x0;
    goto L_5c49;

L_5b35:
    y = 0x0;
    goto L_5c3c;

L_5b3d:
    if ((0x0 <= rgx[x]))
        goto L_5b57;
    else
        goto L_5b51;

L_5b51:
    t_merge_5b63_0001 = 0x0;
    goto L_5b63;

L_5b57:
    t_merge_5b63_0001 = rgx[x];

L_5b63:
    if ((0x9 >= t_merge_5b63_0001))
        goto L_5b73;
    else
        goto L_5b6d;

L_5b6d:
    goto L_5b99;

L_5b73:
    if ((0x0 <= rgx[x]))
        goto L_5b8d;
    else
        goto L_5b87;

L_5b87:
    goto L_5b99;

L_5b8d:

L_5b99:
    if ((0x0 <= rgy[y]))
        goto L_5bb9;
    else
        goto L_5bb3;

L_5bb3:
    t_merge_5bc5_0001 = 0x0;
    goto L_5bc5;

L_5bb9:
    t_merge_5bc5_0001 = rgy[y];

L_5bc5:
    if ((0x9 >= t_merge_5bc5_0001))
        goto L_5bd5;
    else
        goto L_5bcf;

L_5bcf:
    t_merge_5bfb_0001 = 0x9;
    goto L_5bfb;

L_5bd5:
    if ((0x0 <= rgy[y]))
        goto L_5bef;
    else
        goto L_5be9;

L_5be9:
    t_merge_5bfb_0001 = 0x0;
    goto L_5bfb;

L_5bef:
    t_merge_5bfb_0001 = rgy[y];

L_5bfb:
    /* untranslated: brcEnemy = lobyte((((t_merge_5bfb_0001 & 0xf) << 0x4) | ss:[bp-0x56])) */
    dzEnemy = DzFromBrcBrc(brc, brcEnemy);
    if ((dzEnemy <= dMax))
        goto L_5c38;
    else
        goto L_5c32;

L_5c32:
    dMax = dzEnemy;

L_5c38:
    y = (y + 0x1);

L_5c3c:
    if ((y < 0x2))
        goto L_5b3d;
    else
        goto L_5c45;

L_5c45:
    x = (x + 0x1);

L_5c49:
    if ((x < 0x2))
        goto L_5b35;
    else
        goto L_5c52;

L_5c52:
    if ((fPrimary == 0x0))
        goto L_5c68;
    else
        goto L_5c5b;

L_5c5b:
    t_merge_5c7a_0001 = ptokSrc->mdTarget1;
    goto L_5c7a;

L_5c68:
    t_merge_5c7a_0001 = ptokSrc->mdTarget2;

L_5c7a:
    fWeAttack = FIsTargetOfMdTarget(ptok, t_merge_5c7a_0001);
    scoreThemBest = 0x1c9c380;
    iBest = dMin;
    i = dMin;
    goto L_5db9;

L_5ca5:
    if ((fWeAttack == 0x0))
        goto L_5ce8;
    else
        goto L_5cae;

L_5cae:
    dpGiven = DpFromPtokBrcToBrc(ptokSrc, 0x0, ((((i & 0xf) << 0x4) | 0x0) & 0xff), ptok, 0x0);
    goto L_5cf2;

L_5ce8:
    dpGiven = 0x0;

L_5cf2:
    if ((ptokSrc->mdTactic != 0x0))
        goto L_5d0f;
    else
        goto L_5d09;

L_5d09:
    t_merge_5d12_0001 = 0x1;
    goto L_5d12;

L_5d0f:
    t_merge_5d12_0001 = 0x0;

L_5d12:
    dpTaken = DpFromPtokBrcToBrc(ptok, 0x0, ((((i & 0xf) << 0x4) | 0x0) & 0xff), ptokSrc, t_merge_5d12_0001);
    scoreThem = ScoreFromGiveAndTakeAndTactic(dpTaken, dpGiven, ptok->mdTactic);
    if ((HIWORD(scoreThem) > HIWORD(scoreThemBest)))
        goto L_5db5;
    else
        goto L_5d7e;

L_5d7e:
    if ((HIWORD(scoreThem) < HIWORD(scoreThemBest)))
        goto L_5d8b;
    else
        goto L_5d83;

L_5d83:
    if ((LOWORD(scoreThem) > LOWORD(scoreThemBest)))
        goto L_5db5;
    else
        goto L_5d8b;

L_5d8b:
    scoreThemBest = scoreThem;
    iBest = i;
    dpTakenBest = dpTaken;
    dpGivenCur = dpGiven;

L_5db5:
    i = (i + 0x1);

L_5db9:
    if ((i <= dMax))
        goto L_5ca5;
    else
        goto L_5dc4;

L_5dc4:
    if ((HIWORD(dpGivenCur) < HIWORD(dpGivenBest)))
        goto L_5deb;
    else
        goto L_5dd2;

L_5dd2:
    if ((HIWORD(dpGivenCur) > HIWORD(dpGivenBest)))
        goto L_5ddf;
    else
        goto L_5dd7;

L_5dd7:
    if ((LOWORD(dpGivenCur) <= LOWORD(dpGivenBest)))
        goto L_5deb;
    else
        goto L_5ddf;

L_5ddf:
    dpGivenBest = dpGivenCur;

L_5deb:
    dpTakenTotal = (dpTakenTotal + dpTakenBest);
    goto L_59f0;

L_5dfa:
    scoreUs = ScoreFromGiveAndTakeAndTactic(dpGivenBest, dpTakenTotal, ptokSrc->mdTactic);

L_5e2d:
    return scoreUs;
}

int32_t ScoreFromGiveAndTakeAndTactic(int32_t dpGive, int32_t dpTake, int16_t mdTactic) {
    int32_t  score;
    uint32_t t_merge_5f11_0001;

L_5e34:
    goto L_5ef4;

L_5e43:
    goto L_5f11;
    goto L_5f11;
    goto L_5f11;
    /* untranslated: LOWORD(score) = (LOWORD(dpGive) neg 0x0) */
    /* untranslated: HIWORD(score) = ((HIWORD(dpGive) + 0x0) neg 0x0) */
    if ((LOWORD(score) != 0x0))
        goto L_5e8a;
    else
        goto L_5e81;

L_5e81:
    if ((HIWORD(score) == 0x0))
        goto L_5edc;
    else
        goto L_5e8a;

L_5e8a:
    /* untranslated: score = (int32_t)((uint32_t)(score * 0x64) / words((HIWORD(dpTake) + 0x0), (LOWORD(dpTake) + 0x1))) */
    if ((HIWORD(score) < 0x0))
        goto L_5ee8;
    else
        goto L_5ec1;

L_5ec1:
    if ((HIWORD(score) > 0x0))
        goto L_5ecf;
    else
        goto L_5ec6;

L_5ec6:
    if ((LOWORD(score) < 0x0))
        goto L_5ee8;
    else
        goto L_5ecf;

L_5ecf:
    score = 0xffffffff;

L_5edc:
    score = dpTake;

L_5ee8:
    t_merge_5f11_0001 = score;
    goto L_5f11;

L_5ef4:
    if ((mdTactic > 0x5))
        goto L_5e43;
    else
        goto L_5efc;

L_5efc:
    /* untranslated: t_merge_5f11_0001 = words((mdTactic * 0x2), dx) */
    goto L_ffffffff;

L_5f11:
    return t_merge_5f11_0001;
}

int16_t DxyMoveTokTo(TOK *ptok, int16_t spdMove, uint16_t grfAttack) {
    uint16_t iplr;
    int16_t  xMax;
    int16_t  dz;
    int32_t  scoreBest;
    uint8_t  brc;
    int32_t  rgscoreNear[3][3];
    int32_t  score;
    int16_t  cBest;
    int16_t  yMin;
    int16_t  dy;
    int16_t  mdTactic;
    int16_t  y;
    uint8_t  brcOOR;
    int16_t  i;
    int16_t  yCur;
    uint8_t  brcBest;
    int16_t  dzAwayBest;
    int16_t  yMax;
    int16_t  dx;
    int16_t  xCur;
    int16_t  fPrimary;
    int32_t  dp;
    int16_t  dzAway;
    int16_t  x;
    int32_t  lLow;
    int16_t  cLow;
    int16_t  fXMajor;
    POINT    rgptDeltas[2];
    uint16_t t_merge_6353_0001;
    int16_t  t_643a;
    uint16_t t_merge_648f_0001;
    int16_t  t_6576;
    uint16_t t_merge_65cd_0001;
    uint16_t t_merge_65e2_0001;
    uint16_t t_merge_65f7_0001;

L_5f18:
    iplr = ptok->iplr;
    dp = 0x0;
    xCur = (ptok->brc & 0xf);
    yCur = (ptok->brc >> 0x4);
    if ((ptok->grobj == grobjPlanet))
        goto LReturnDxy;
    else
        goto L_5f6f;

L_5f6f:
    if ((spdMove == 0x0))
        goto LReturnDxy;
    else
        goto L_5f75;

L_5f75:

L_5f7b:
    scoreBest = 0x1c9c380;
    mdTactic = ptok->mdTactic;
    fPrimary = FDoesPrimaryTargetTypeExist(ptok, grfAttack);
    x = 0x0;
    goto L_5fe9;

L_5fb3:
    y = 0x0;
    goto L_5fdc;

L_5fbb:
    rgscoreNear[x][y] = 0x1c9c380;
    y = (y + 0x1);

L_5fdc:
    if ((y < 0x3))
        goto L_5fbb;
    else
        goto L_5fe5;

L_5fe5:
    x = (x + 0x1);

L_5fe9:
    if ((x < 0x3))
        goto L_5fb3;
    else
        goto L_5ff2;

L_5ff2:
    dz = DzMoveRangeToConsider(ptok, grfAttack, &(brcOOR));
    x = (xCur - dz);
    if ((x >= 0x0))
        goto L_6021;
    else
        goto L_601c;

L_601c:
    x = 0x0;

L_6021:
    yMin = (yCur - dz);
    if ((yMin >= 0x0))
        goto L_6038;
    else
        goto L_6033;

L_6033:
    yMin = 0x0;

L_6038:
    xMax = (xCur + dz);
    if ((xMax < 0xa))
        goto L_604f;
    else
        goto L_604a;

L_604a:
    xMax = 0x9;

L_604f:
    yMax = (yCur + dz);
    if ((yMax < 0xa))
        goto L_6273;
    else
        goto L_6061;

L_6061:
    yMax = 0x9;

L_6069:
    y = yMin;
    goto L_6264;

L_6072:
    brc = LOBYTE((((y & 0xf) << 0x4) | (x & 0xf)));
    dx = (xCur - x);
    dy = (yCur - y);
    dx = abs(dx);
    dy = abs(dy);
    score = ScoreGuessBattleDamage(ptok, brc, fPrimary, grfAttack);
    if ((mdTactic != 0x0))
        goto L_6176;
    else
        goto L_60e3;

L_60e3:
    i = 0x0;
    goto L_6144;

L_60eb:
    if ((vrgtok[i].brc != brc))
        goto L_6140;
    else
        goto L_6117;

L_6117:
    if ((vrgtok[i].iplr != iplr))
        goto L_6140;
    else
        goto L_6138;

L_6138:
    score = (score + 0x2);

L_6140:
    i = (i + 0x1);

L_6144:
    if ((i < vctok))
        goto L_60eb;
    else
        goto L_614f;

L_614f:
    if ((brc != ptok->brc))
        goto L_6176;
    else
        goto L_616e;

L_616e:
    score = (score - 0x1);

L_6176:
    dzAway = DzFromBrcBrc(ptok->brc, brc);
    if ((dzAway > 0x1))
        goto L_61c9;
    else
        goto L_619c;

L_619c:
    rgscoreNear[((x - xCur) + 0x1)][((y - yCur) + 0x1)] = score;

L_61c9:
    if ((HIWORD(score) < HIWORD(scoreBest)))
        goto L_6205;
    else
        goto L_61d7;

L_61d7:
    if ((HIWORD(score) > HIWORD(scoreBest)))
        goto L_61e4;
    else
        goto L_61dc;

L_61dc:
    if ((LOWORD(score) < LOWORD(scoreBest)))
        goto L_6205;
    else
        goto L_61e4;

L_61e4:
    if ((LOWORD(score) != LOWORD(scoreBest)))
        goto L_6260;
    else
        goto L_61f2;

L_61f2:
    if ((HIWORD(score) != HIWORD(scoreBest)))
        goto L_6260;
    else
        goto L_61fa;

L_61fa:
    if ((dzAway > dzAwayBest))
        goto L_6260;
    else
        goto L_6205;

L_6205:
    if ((LOWORD(score) != LOWORD(scoreBest)))
        goto L_6243;
    else
        goto L_6213;

L_6213:
    if ((HIWORD(score) != HIWORD(scoreBest)))
        goto L_6243;
    else
        goto L_621b;

L_621b:
    if ((dzAway != dzAwayBest))
        goto L_6243;
    else
        goto L_6226;

L_6226:
    cBest = (cBest + 0x1);
    if ((Random(cBest) == 0x0))
        goto LTakeSquare;
    else
        goto L_623a;

L_623a:

L_6243:
    cBest = 0x1;
    scoreBest = score;
    dzAwayBest = dzAway;

LTakeSquare:
    brcBest = brc;

L_6260:
    y = (y + 0x1);

L_6264:
    if ((y <= yMax))
        goto L_6072;
    else
        goto L_626f;

L_626f:
    x = (x + 0x1);

L_6273:
    if ((x <= xMax))
        goto L_6069;
    else
        goto L_627e;

L_627e:
    if ((brcOOR == 0xff))
        goto L_6292;
    else
        goto L_628c;

L_628c:
    brcBest = brcOOR;

L_6292:
    dzAway = DzFromBrcBrc(ptok->brc, brcBest);
    if ((dzAway <= 0x1))
        goto L_6720;
    else
        goto L_62b8;

L_62b8:
    dx = ((brcBest & 0xf) - xCur);
    dy = ((brcBest >> 0x4) - yCur);
    /* untranslated: ss:[bp-0x5c] = abs(dx) */
    /* untranslated: branch ss:[bp-0x5c] != abs(dy) ? L_6329 : L_62fe */

L_62fe:
    if ((dx <= 0x0))
        goto L_630e;
    else
        goto L_6307;

L_6307:
    xCur = (xCur + 0x1);
    goto L_6312;

L_630e:
    xCur = (xCur - 0x1);

L_6312:
    if ((dy <= 0x0))
        goto L_6322;
    else
        goto L_631b;

L_631b:
    yCur = (yCur + 0x1);
    goto L_6707;

L_6322:
    yCur = (yCur - 0x1);

L_6329:
    if ((dx != 0x0))
        goto L_6465;
    else
        goto L_6332;

L_6332:
    lLow = 0x11e1a300;
    cLow = 0x0;
    if ((dy >= 0x0))
        goto L_6350;
    else
        goto L_634a;

L_634a:
    t_merge_6353_0001 = 0x0;
    goto L_6353;

L_6350:
    t_merge_6353_0001 = 0x2;

L_6353:
    dy = t_merge_6353_0001;
    yCur = (yCur + (dy + 0xffff));
    i = 0x0;
    goto L_63f2;

L_6367:
    if ((HIWORD(rgscoreNear[i][dy]) > HIWORD(lLow)))
        goto L_63ee;
    else
        goto L_6389;

L_6389:
    if ((HIWORD(rgscoreNear[i][dy]) < HIWORD(lLow)))
        goto L_6395;
    else
        goto L_638e;

L_638e:
    if ((LOWORD(rgscoreNear[i][dy]) > LOWORD(lLow)))
        goto L_63ee;
    else
        goto L_6395;

L_6395:
    if ((HIWORD(rgscoreNear[i][dy]) > HIWORD(lLow)))
        goto L_63ea;
    else
        goto L_63b7;

L_63b7:
    if ((HIWORD(rgscoreNear[i][dy]) < HIWORD(lLow)))
        goto L_63c3;
    else
        goto L_63bc;

L_63bc:
    if ((LOWORD(rgscoreNear[i][dy]) >= LOWORD(lLow)))
        goto L_63ea;
    else
        goto L_63c3;

L_63c3:
    lLow = rgscoreNear[i][dy];
    cLow = 0x1;
    goto L_63ee;

L_63ea:
    cLow = (cLow + 0x1);

L_63ee:
    i = (i + 0x1);

L_63f2:
    if ((i < 0x3))
        goto L_6367;
    else
        goto L_63fb;

L_63fb:
    x = Random(cLow);
    i = 0x0;
    goto L_6450;

L_6411:
    if ((LOWORD(rgscoreNear[i][dy]) != LOWORD(lLow)))
        goto L_644c;
    else
        goto L_6432;

L_6432:
    if ((HIWORD(rgscoreNear[i][dy]) != HIWORD(lLow)))
        goto L_644c;
    else
        goto L_643a;

L_643a:
    t_643a = x;
    x = (x - 0x1);
    if ((t_643a == 0x0))
        goto L_6459;
    else
        goto L_6446;

L_6446:

L_644c:
    i = (i + 0x1);

L_6450:
    if ((i < 0x3))
        goto L_6411;
    else
        goto L_6459;

L_6459:
    xCur = (xCur + (i + 0xffff));
    goto L_6707;

L_6465:
    if ((dy != 0x0))
        goto L_65a1;
    else
        goto L_646e;

L_646e:
    lLow = 0x11e1a300;
    cLow = 0x0;
    if ((dx >= 0x0))
        goto L_648c;
    else
        goto L_6486;

L_6486:
    t_merge_648f_0001 = 0x0;
    goto L_648f;

L_648c:
    t_merge_648f_0001 = 0x2;

L_648f:
    dx = t_merge_648f_0001;
    xCur = (xCur + (dx + 0xffff));
    i = 0x0;
    goto L_652e;

L_64a3:
    if ((HIWORD(rgscoreNear[dx][i]) > HIWORD(lLow)))
        goto L_652a;
    else
        goto L_64c5;

L_64c5:
    if ((HIWORD(rgscoreNear[dx][i]) < HIWORD(lLow)))
        goto L_64d1;
    else
        goto L_64ca;

L_64ca:
    if ((LOWORD(rgscoreNear[dx][i]) > LOWORD(lLow)))
        goto L_652a;
    else
        goto L_64d1;

L_64d1:
    if ((HIWORD(rgscoreNear[dx][i]) > HIWORD(lLow)))
        goto L_6526;
    else
        goto L_64f3;

L_64f3:
    if ((HIWORD(rgscoreNear[dx][i]) < HIWORD(lLow)))
        goto L_64ff;
    else
        goto L_64f8;

L_64f8:
    if ((LOWORD(rgscoreNear[dx][i]) >= LOWORD(lLow)))
        goto L_6526;
    else
        goto L_64ff;

L_64ff:
    lLow = rgscoreNear[dx][i];
    cLow = 0x1;
    goto L_652a;

L_6526:
    cLow = (cLow + 0x1);

L_652a:
    i = (i + 0x1);

L_652e:
    if ((i < 0x3))
        goto L_64a3;
    else
        goto L_6537;

L_6537:
    x = Random(cLow);
    i = 0x0;
    goto L_658c;

L_654d:
    if ((LOWORD(rgscoreNear[dx][i]) != LOWORD(lLow)))
        goto L_6588;
    else
        goto L_656e;

L_656e:
    if ((HIWORD(rgscoreNear[dx][i]) != HIWORD(lLow)))
        goto L_6588;
    else
        goto L_6576;

L_6576:
    t_6576 = x;
    x = (x - 0x1);
    if ((t_6576 == 0x0))
        goto L_6595;
    else
        goto L_6582;

L_6582:

L_6588:
    i = (i + 0x1);

L_658c:
    if ((i < 0x3))
        goto L_654d;
    else
        goto L_6595;

L_6595:
    yCur = (yCur + (i + 0xffff));
    goto L_6707;

L_65a1:
    /* untranslated: ss:[bp-0x66] = abs(dx) */
    /* untranslated: branch ss:[bp-0x66] <= abs(dy) ? L_65ca : L_65c4 */

L_65c4:
    t_merge_65cd_0001 = 0x1;
    goto L_65cd;

L_65ca:
    t_merge_65cd_0001 = 0x0;

L_65cd:
    fXMajor = t_merge_65cd_0001;
    if ((dx <= 0x0))
        goto L_65df;
    else
        goto L_65d9;

L_65d9:
    t_merge_65e2_0001 = 0x2;
    goto L_65e2;

L_65df:
    t_merge_65e2_0001 = 0x0;

L_65e2:
    dx = t_merge_65e2_0001;
    if ((dy <= 0x0))
        goto L_65f4;
    else
        goto L_65ee;

L_65ee:
    t_merge_65f7_0001 = 0x2;
    goto L_65f7;

L_65f4:
    t_merge_65f7_0001 = 0x0;

L_65f7:
    dy = t_merge_65f7_0001;
    rgptDeltas[0x0].x = dx;
    rgptDeltas[0x0].y = dy;
    if ((fXMajor == 0x0))
        goto L_661d;
    else
        goto L_660f;

L_660f:
    rgptDeltas[0x1].x = dx;
    rgptDeltas[0x1].y = 0x1;
    goto L_6628;

L_661d:
    rgptDeltas[0x1].x = 0x1;
    rgptDeltas[0x1].y = dy;

L_6628:
    if ((HIWORD(rgscoreNear[rgptDeltas[0x0].x][rgptDeltas[0x0].y]) < HIWORD(rgscoreNear[rgptDeltas[0x1].x][rgptDeltas[0x1].y])))
        goto L_66d1;
    else
        goto L_6669;

L_6669:
    /* untranslated: branch HIWORD(rgscoreNear[rgptDeltas[0x0].x][rgptDeltas[0x0].y]) > ss:[bp-0x66] ? L_6675 : L_666e */

L_666e:
    /* untranslated: branch LOWORD(rgscoreNear[rgptDeltas[0x0].x][rgptDeltas[0x0].y]) < ss:[bp-0x68] ? L_66d1 : L_6675 */

L_6675:
    if ((LOWORD(rgscoreNear[rgptDeltas[0x0].x][rgptDeltas[0x0].y]) != LOWORD(rgscoreNear[rgptDeltas[0x1].x][rgptDeltas[0x1].y])))
        goto L_66d9;
    else
        goto L_66b5;

L_66b5:
    /* untranslated: branch HIWORD(rgscoreNear[rgptDeltas[0x0].x][rgptDeltas[0x0].y]) != ss:[bp-0x66] ? L_66d9 : L_66bd */

L_66bd:
    if ((Random(0x2) != 0x0))
        goto L_66d9;
    else
        goto L_66d1;

L_66d1:
    i = 0x0;
    goto L_66de;

L_66d9:
    i = 0x1;

L_66de:
    xCur = (xCur + (rgptDeltas[i].x + 0xffff));
    yCur = (yCur + (rgptDeltas[i].y + 0xffff));

L_6707:
    brcBest = LOBYTE((((yCur & 0xf) << 0x4) | (xCur & 0xf)));

L_6720:
    if ((LOWORD(scoreBest) != 0xc380))
        goto L_6734;
    else
        goto L_672a;

L_672a:
    if ((HIWORD(scoreBest) == 0x1c9))
        goto LReturnDxy;
    else
        goto L_6734;

L_6734:
    if (((brcBest & 0xf) > 0x9))
        goto L_675b;
    else
        goto L_6745;

L_6745:
    if (((brcBest >> 0x4) <= 0x9))
        goto L_6765;
    else
        goto L_675b;

L_675b:
    brcBest = ptok->brc;

L_6765:
    ptok->brc = brcBest;

LReturnDxy:
    ptok->fMoved = 0x1;

L_6789:
    return 0x1;
}

int32_t CTorpHit(int32_t cTorpBase, TOK *ptok, int16_t pctBase, int16_t pctBC) {
    int32_t pctJam;
    int16_t i;
    int32_t pctHit;
    int32_t cTorpHit;

L_6790:
    if ((LOWORD(cTorpBase) != 0x0))
        goto L_67ab;
    else
        goto L_67a2;

L_67a2:
    if ((HIWORD(cTorpBase) == 0x0))
        goto L_67b4;
    else
        goto L_67ab;

L_67ab:
    if ((pctBase != 0x0))
        goto L_67bd;
    else
        goto L_67b4;

L_67b4:
    return 0x0;

L_67bd:
    LOWORD(pctJam) = ptok->pctJam;
    HIWORD(pctJam) = 0x0;
    if ((LOWORD(pctJam) != 0x0))
        goto L_67e2;
    else
        goto L_67d9;

L_67d9:
    if ((HIWORD(pctJam) == 0x0))
        goto L_682e;
    else
        goto L_67e2;

L_67e2:
    if ((pctBC == 0x0))
        goto L_682e;
    else
        goto L_67eb;

L_67eb:
    pctJam = (pctJam - (uint32_t)(pctBC));
    if ((HIWORD(pctJam) > 0x0))
        goto L_6829;
    else
        goto L_67fe;

L_67fe:
    if ((HIWORD(pctJam) < 0x0))
        goto L_680c;
    else
        goto L_6803;

L_6803:
    if ((LOWORD(pctJam) >= 0x0))
        goto L_6829;
    else
        goto L_680c;

L_680c:
    /* untranslated: pctBC = (LOWORD(pctJam) neg 0x0) */
    pctJam = 0x0;
    goto L_682e;

L_6829:
    pctBC = 0x0;

L_682e:
    if ((pctBC == 0x0))
        goto L_6877;
    else
        goto L_6837;

L_6837:
    /* untranslated: pctHit = (0x64 - (int32_t)((uint32_t)(words((0x0 - signhiword(pctBase)), (0x64 - pctBase)) * sext16to32((0x64 - pctBC))) / 0x64)) */
    goto L_68c4;

L_6877:
    if ((LOWORD(pctJam) != 0x0))
        goto L_6889;
    else
        goto L_6880;

L_6880:
    if ((HIWORD(pctJam) == 0x0))
        goto L_68ba;
    else
        goto L_6889;

L_6889:
    /* untranslated: pctHit = (int32_t)((uint32_t)(sext16to32(pctBase) * words((0x0 - HIWORD(pctJam)), (0x64 - LOWORD(pctJam)))) / 0x64) */
    goto L_68c4;

L_68ba:
    pctHit = (uint32_t)(pctBase);

L_68c4:
    if ((HIWORD(pctHit) > 0x0))
        goto L_68e5;
    else
        goto L_68cd;

L_68cd:
    if ((HIWORD(pctHit) < 0x0))
        goto L_68db;
    else
        goto L_68d2;

L_68d2:
    if ((LOWORD(pctHit) >= 0x1))
        goto L_68e5;
    else
        goto L_68db;

L_68db:
    pctHit = 0x1;

L_68e5:
    if ((HIWORD(pctHit) < 0x0))
        goto L_6905;
    else
        goto L_68ee;

L_68ee:
    if ((HIWORD(pctHit) > 0x0))
        goto L_68fc;
    else
        goto L_68f3;

L_68f3:
    if ((LOWORD(pctHit) < 0x64))
        goto L_6905;
    else
        goto L_68fc;

L_68fc:
    return cTorpBase;

L_6905:
    if ((HIWORD(cTorpBase) < 0x0))
        goto L_6946;
    else
        goto L_690e;

L_690e:
    if ((HIWORD(cTorpBase) > 0x0))
        goto L_691d;
    else
        goto L_6913;

L_6913:
    if ((LOWORD(cTorpBase) <= 0xc8))
        goto L_6946;
    else
        goto L_691d;

L_691d:
    cTorpHit = (int32_t)(((uint32_t)((cTorpBase * pctHit)) / 0x64));
    goto L_699c;

L_6946:
    cTorpHit = 0x0;
    i = 0x0;
    goto L_6983;

L_6958:
    if ((Random(0x64) >= LOWORD(pctHit)))
        goto L_697f;
    else
        goto L_6977;

L_6977:
    cTorpHit = (cTorpHit + 0x1);

L_697f:
    i = (i + 0x1);

L_6983:
    if ((SIGNHIWORD(i) < HIWORD(cTorpBase)))
        goto L_6958;
    else
        goto L_698f;

L_698f:
    if ((SIGNHIWORD(i) > HIWORD(cTorpBase)))
        goto L_699c;
    else
        goto L_6994;

L_6994:
    if ((i < LOWORD(cTorpBase)))
        goto L_6958;
    else
        goto L_699c;

L_699c:
    return cTorpHit;
}

int16_t FAttack(int16_t itokAttacker, int16_t init, BTLREC *lpbtlrec, uint16_t grfAttack) {
    int32_t  dpShieldLeft;
    int16_t  dz;
    SHDEF   *lpshdefE;
    int32_t  dpArmorLeft;
    int32_t  dpSingle;
    int32_t  scoreBest;
    TOK     *ptok;
    int16_t  ctokDamaged;
    int16_t  itokTarget;
    int32_t  dpMain;
    int32_t  score;
    int16_t  fSetItok;
    int16_t  dxRangeCur;
    int16_t  ihs;
    int32_t  cTorpMiss;
    int32_t  cTorpFire;
    int32_t  cTorpsLeft;
    int16_t  i;
    int32_t  cTorpBase;
    uint16_t grfWeapon;
    int16_t  cItem;
    int32_t  pctHit;
    TOK     *ptokTarget;
    SHDEF   *lpshdef;
    int32_t  lValue;
    int32_t  dpT;
    HUL     *lphul;
    int32_t  cTorpHit;
    int16_t  fPrimary;
    int32_t  dp;
    int16_t  itok;
    int32_t  dpCol;
    TOK     *ptokE;
    PART     part;
    int32_t  nds;
    int16_t  fCapMissile;
    int32_t  nts;
    int32_t  ntk;
    int32_t  dpShieldCur;
    int32_t  dpHitArmor;
    uint16_t t_merge_6b29_0001;
    uint16_t t_merge_6ef9_0001;
    uint16_t t_71f9;
    uint16_t t_7242;
    uint16_t t_merge_7293_0001;
    int32_t  t_merge_7484_0001_wide;
    int32_t  t_merge_77fb_0001_wide;

L_69ac:
    dxRangeCur = 0x0;
    fSetItok = 0x0;
    ctokDamaged = 0x0;
    ptok = &(vrgtok[itokAttacker]);
    lpshdef = LpshdefFromTok(ptok);
    lphul = lpshdef;
    ihs = 0x0;
    goto L_6a06;

L_6a02:
    ihs = (ihs + 0x1);

L_6a06:
    if ((ihs >= lphul->chs))
        goto L_7cb8;
    else
        goto L_6a18;

L_6a18:
    if (((lphul->rghs[ihs].grhst & 0x30) == 0x0))
        goto L_6a02;
    else
        goto L_6a3c;

L_6a3c:
    /* untranslated: branch ((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) == 0x0 ? L_6a02 : L_6a66 */

L_6a66:
    part.hs.grhst = lphul->rghs[ihs].grhst;
    /* untranslated: HIWORD(part.hs) = lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] */
    idPlayer = ptok->iplr;
    if ((FLookupPart(&(part)) == 0x0))
        goto L_6ab0;
    else
        goto L_6aaa;

L_6aaa:
    goto L_6ab3;

L_6ab0:

L_6ab3:
    idPlayer = 0xffff;
    /* untranslated: cItem = ((lphul->rgTech[part[12:0](rgcrPlrHistory[ihs])] >> 0x8) & 0xff) */
    /* untranslated: i = (ptok->initBase + part[56:2](part.pcom)) */
    if ((i < 0x40))
        goto L_6b00;
    else
        goto L_6afb;

L_6afb:
    i = 0x3f;

L_6b00:
    if ((i != init))
        goto L_6a02;
    else
        goto L_6b08;

L_6b08:

L_6b0e:
    if ((ptok->grobj != grobjPlanet))
        goto L_6b26;
    else
        goto L_6b20;

L_6b20:
    t_merge_6b29_0001 = 0x1;
    goto L_6b29;

L_6b26:
    t_merge_6b29_0001 = 0x0;

L_6b29:
    /* untranslated: dxRangeCur = (t_merge_6b29_0001 + part[52:2](part.pcom)) */
    if ((part.hs.grhst != hstBeam))
        goto L_6d99;
    else
        goto L_6b3c;

L_6b3c:
    if (((part.pbeam->grfAbilities & 0x2) == 0x0))
        goto L_6d99;
    else
        goto L_6b4e;

L_6b4e:
    /* untranslated: dp = (uint32_t)((uint32_t)(sext16to32(part.pbeam->dp) * sext16to32(cItem)) * words(0x0, ptok->csh)) */
    if ((part.pbeam->dp < 0xc8))
        goto L_6b92;
    else
        goto L_6b8a;

L_6b8a:
    grfWeapon = 0x2;
    goto L_6b97;

L_6b92:
    grfWeapon = 0x1;

L_6b97:
    if ((ptok->pctCap == 0x0))
        goto L_6bd6;
    else
        goto L_6ba9;

L_6ba9:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptok->pctCap), ptok->pctCap)) / 0x64) */

L_6bd6:
    dpT = dp;
    ptokE = vrgtok;
    itok = 0x0;
    goto L_6c0b;

L_6bfa:
    ptokE = (ptokE + 0x1);
    itok = (itok + 0x1);

L_6c0b:
    if ((itok >= vctok))
        goto L_6a02;
    else
        goto L_6c16;

L_6c16:
    if ((ptokE->fActive == 0x0))
        goto L_6bfa;
    else
        goto L_6c28;

L_6c28:
    if ((ptokE->iplr == ptok->iplr))
        goto L_6bfa;
    else
        goto L_6c4b;

L_6c4b:
    if ((((0x1 << ptokE->iplr) & grfAttack) == 0x0))
        goto L_6bfa;
    else
        goto L_6c64;

L_6c64:

L_6c6a:
    if ((DzFromBrcBrc(ptokE->brc, ptok->brc) > dxRangeCur))
        goto L_6bfa;
    else
        goto L_6c8d;

L_6c8d:

L_6c93:
    if ((FIsTargetOfMdTarget(ptokE, ptok->mdTarget1) != 0x0))
        goto L_6ce0;
    else
        goto L_6cb4;

L_6cb4:
    if ((FIsTargetOfMdTarget(ptokE, ptok->mdTarget2) == 0x0))
        goto L_6bfa;
    else
        goto L_6cda;

L_6cda:

L_6ce0:
    if ((ptokE->pctBeamDef >= 0x64))
        goto L_6d1f;
    else
        goto L_6cf2;

L_6cf2:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptokE->pctBeamDef), ptokE->pctBeamDef)) / 0x64) */

L_6d1f:
    if ((FDamageTok(ptokE, itok, &(dp), 0x0, grfWeapon, (part.pbeam->grfAbilities & 0x1), 0x0) == 0x0))
        goto L_6d87;
    else
        goto L_6d56;

L_6d56:
    if ((fSetItok != 0x0))
        goto L_6d83;
    else
        goto L_6d5f;

L_6d5f:
    fSetItok = 0x1;
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff) | ((itok & 0xff) << 0x8));

L_6d83:
    ctokDamaged = (ctokDamaged + 0x1);

L_6d87:
    dp = dpT;
    goto L_6bfa;

L_6d99:
    if ((part.hs.grhst != hstBeam))
        goto L_6ddd;
    else
        goto L_6da2;

L_6da2:
    /* untranslated: dpMain = (uint32_t)((uint32_t)(sext16to32(part.pbeam->dp) * sext16to32(cItem)) * words(0x0, ptok->csh)) */
    cTorpsLeft = 0x0;
    goto LFindAnotherTarget;

L_6ddd:
    dpMain = 0x0;
    /* untranslated: cTorpsLeft = (uint32_t)(sext16to32(cItem) * words(0x0, ptok->csh)) */

LFindAnotherTarget:
    fPrimary = 0x1;
    goto L_7561;

L_6e0c:
    scoreBest = 0x0;
    ptokTarget = 0x0;
    ptokE = vrgtok;
    itok = 0x0;
    goto L_6e49;

L_6e38:
    ptokE = (ptokE + 0x1);
    itok = (itok + 0x1);

L_6e49:
    if ((itok >= vctok))
        goto L_7548;
    else
        goto L_6e54;

L_6e54:
    if ((ptokE->fActive == 0x0))
        goto L_6e38;
    else
        goto L_6e66;

L_6e66:
    if ((ptokE->iplr == ptok->iplr))
        goto L_6e38;
    else
        goto L_6e89;

L_6e89:
    if ((((0x1 << ptokE->iplr) & grfAttack) == 0x0))
        goto L_6e38;
    else
        goto L_6ea2;

L_6ea2:

L_6ea8:
    if ((DzFromBrcBrc(ptokE->brc, ptok->brc) > dxRangeCur))
        goto L_6e38;
    else
        goto L_6ecb;

L_6ecb:

L_6ed1:
    if ((fPrimary == 0x0))
        goto L_6ee7;
    else
        goto L_6eda;

L_6eda:
    t_merge_6ef9_0001 = ptok->mdTarget1;
    goto L_6ef9;

L_6ee7:
    t_merge_6ef9_0001 = ptok->mdTarget2;

L_6ef9:
    if ((FIsTargetOfMdTarget(ptokE, t_merge_6ef9_0001) == 0x0))
        goto L_6e38;
    else
        goto L_6f0d;

L_6f0d:

L_6f13:
    lpshdefE = LpshdefFromTok(ptokE);
    /* untranslated: lValue = (uint32_t)(words(0x0, (lpshdefE->hul.resCost + lpshdefE->hul.rgwtOreCost[0x1])) * words(0x0, ptokE->csh)) */
    if ((HIWORD(lValue) > 0x1))
        goto L_6f8c;
    else
        goto L_6f61;

L_6f61:
    if ((HIWORD(lValue) < 0x1))
        goto L_6f70;
    else
        goto L_6f66;

L_6f66:
    if ((LOWORD(lValue) >= 0x86a0))
        goto L_6f8c;
    else
        goto L_6f70;

L_6f70:
    lValue = (uint32_t)((lValue * 0x64));
    goto L_6f96;

L_6f8c:
    lValue = 0x989680;

L_6f96:
    LOWORD(dpSingle) = lpshdefE->hul.dp;
    HIWORD(dpSingle) = 0x0;
    /* untranslated: dpShieldLeft = (uint32_t)(words(0x0, ptokE->dpShield) * words(0x0, ptokE->csh)) */
    /* untranslated: dpArmorLeft = (uint32_t)(dpSingle * words(0x0, ptokE->csh)) */
    if ((ptokE->dv.dp == 0x0))
        goto L_706e;
    else
        goto L_6ff3;

L_6ff3:
    /* untranslated: dpArmorLeft = (dpArmorLeft - (int32_t)((uint32_t)((int32_t)((uint32_t)((int32_t)((uint32_t)(dpSingle * words(0x0, ((ptokE->dv.dp >> 0x7) &
     * 0x1ff))) / 0xa) * words(0x0, (ptokE->dv.dp & 0x7f))) / 0xa) * words(0x0, ptokE->csh)) / 0x1f4)) */

L_706e:
    if ((HIWORD(dpArmorLeft) > 0x0))
        goto L_708f;
    else
        goto L_7077;

L_7077:
    if ((HIWORD(dpArmorLeft) < 0x0))
        goto L_7085;
    else
        goto L_707c;

L_707c:
    if ((LOWORD(dpArmorLeft) > 0x0))
        goto L_708f;
    else
        goto L_7085;

L_7085:
    dpArmorLeft = 0x1;

L_708f:
    goto L_74f9;

L_7095:
    if ((ptokE->pctBeamDef >= 0x64))
        goto L_70d6;
    else
        goto L_70a7;

L_70a7:
    /* untranslated: lValue = (int32_t)((uint32_t)(lValue * words(0x0, ptokE->pctBeamDef)) / 0x64) */

L_70d6:
    /* untranslated: branch (part[58:2](part.pcom) & 0x1) == 0x0 ? L_7141 : L_70e8 */

L_70e8:
    if ((HIWORD(dpShieldLeft) > 0x0))
        goto L_710c;
    else
        goto L_70f1;

L_70f1:
    if ((HIWORD(dpShieldLeft) < 0x0))
        goto L_70ff;
    else
        goto L_70f6;

L_70f6:
    if ((LOWORD(dpShieldLeft) > 0x0))
        goto L_710c;
    else
        goto L_70ff;

L_70ff:
    score = 0x0;
    goto L_750c;

L_710c:
    /* untranslated: score = (int32_t)(words(((hiword((uint32_t)(lValue * 0x64)) + HIWORD(dpShieldLeft)) + 0xffff), ((loword((uint32_t)(lValue * 0x64)) +
     * LOWORD(dpShieldLeft)) + 0xffff)) / dpShieldLeft) */

L_7141:
    /* untranslated: score = (int32_t)((uint32_t)(lValue * 0x64) / words(((HIWORD(dpArmorLeft) + HIWORD(dpShieldLeft)) + 0x0), ((LOWORD(dpArmorLeft) +
     * LOWORD(dpShieldLeft)) + 0x1))) */
    if ((HIWORD(score) > 0x0))
        goto L_750c;
    else
        goto L_717e;

L_717e:
    if ((HIWORD(score) < 0x0))
        goto L_718c;
    else
        goto L_7183;

L_7183:
    if ((LOWORD(score) > 0x0))
        goto L_750c;
    else
        goto L_718c;

L_718c:
    score = 0x1;

L_7199:
    pctHit = (uint32_t)(part.ptorp->dHitChance);
    if ((ptok->pctBC < ptokE->pctJam))
        goto L_7216;
    else
        goto L_71ca;

L_71ca:
    /* untranslated: ss:[bp-0x7a] = ptokE->pctJam */
    t_71f9 = LOWORD(pctHit);
    /* untranslated: LOWORD(pctHit) = (LOWORD(pctHit) + loword((int32_t)(words(hiword((uint32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - LOWORD(pctHit))) *
     * words(signhiword((ptok->pctBC - ss:[bp-0x7a])), (ptok->pctBC - ss:[bp-0x7a])))), loword((uint32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - LOWORD(pctHit)))
     * * words(signhiword((ptok->pctBC - ss:[bp-0x7a])), (ptok->pctBC - ss:[bp-0x7a]))))) / 0x64))) */
    /* untranslated: HIWORD(pctHit) = (HIWORD(pctHit) + hiword((int32_t)(words(hiword((uint32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - t_71f9)) *
     * words(signhiword((ptok->pctBC - ss:[bp-0x7a])), (ptok->pctBC - ss:[bp-0x7a])))), loword((uint32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - t_71f9)) *
     * words(signhiword((ptok->pctBC - ss:[bp-0x7a])), (ptok->pctBC - ss:[bp-0x7a]))))) / 0x64))) */
    goto L_7257;

L_7216:
    /* untranslated: ss:[bp-0x7a] = ptok->pctBC */
    t_7242 = LOWORD(pctHit);
    /* untranslated: LOWORD(pctHit) = (LOWORD(pctHit) - loword((int32_t)(words(hiword((uint32_t)(pctHit * words(signhiword((ptokE->pctJam - ss:[bp-0x7a])),
     * (ptokE->pctJam - ss:[bp-0x7a])))), loword((uint32_t)(pctHit * words(signhiword((ptokE->pctJam - ss:[bp-0x7a])), (ptokE->pctJam - ss:[bp-0x7a]))))) /
     * 0x64))) */
    /* untranslated: HIWORD(pctHit) = (HIWORD(pctHit) - hiword((int32_t)(words(hiword((uint32_t)(words(HIWORD(pctHit), t_7242) * words(signhiword((ptokE->pctJam
     * - ss:[bp-0x7a])), (ptokE->pctJam - ss:[bp-0x7a])))), loword((uint32_t)(words(HIWORD(pctHit), t_7242) * words(signhiword((ptokE->pctJam - ss:[bp-0x7a])),
     * (ptokE->pctJam - ss:[bp-0x7a]))))) / 0x64))) */

L_7257:
    if ((HIWORD(pctHit) < 0x0))
        goto L_74e9;
    else
        goto L_7260;

L_7260:
    if ((HIWORD(pctHit) > 0x0))
        goto L_726e;
    else
        goto L_7265;

L_7265:
    if ((LOWORD(pctHit) <= 0x0))
        goto L_74e9;
    else
        goto L_726e;

L_726e:
    if ((part.hs.iItem < itorpJihadMissile))
        goto L_7290;
    else
        goto L_727c;

L_727c:
    if ((part.hs.iItem > itorpArmageddonMissile))
        goto L_7290;
    else
        goto L_728a;

L_728a:
    t_merge_7293_0001 = 0x1;
    goto L_7293;

L_7290:
    t_merge_7293_0001 = 0x0;

L_7293:
    fCapMissile = t_merge_7293_0001;
    if ((HIWORD(dpArmorLeft) > 0x1))
        goto L_72e0;
    else
        goto L_729f;

L_729f:
    if ((HIWORD(dpArmorLeft) < 0x1))
        goto L_72ae;
    else
        goto L_72a4;

L_72a4:
    if ((LOWORD(dpArmorLeft) >= 0x86a0))
        goto L_72e0;
    else
        goto L_72ae;

L_72ae:
    nts = (int32_t)(((int32_t)(((uint32_t)((dpArmorLeft * 0x64)) * 0x2)) / pctHit));
    goto L_7307;

L_72e0:
    nts = (uint32_t)(((int32_t)((dpArmorLeft / pctHit)) * 0xc8));

L_7307:
    if ((HIWORD(dpShieldLeft) > 0x1))
        goto L_7386;
    else
        goto L_7310;

L_7310:
    if ((HIWORD(dpShieldLeft) < 0x1))
        goto L_731f;
    else
        goto L_7315;

L_7315:
    if ((LOWORD(dpShieldLeft) >= 0x86a0))
        goto L_7386;
    else
        goto L_731f;

L_731f:
    /* untranslated: nds = (int32_t)((uint32_t)(dpShieldLeft * 0x64) / words((hiword((int32_t)(pctHit / 0x2)) + hiword((int32_t)(words((0x0 - HIWORD(pctHit)),
     * (0x64 - LOWORD(pctHit))) / 0x8))), (loword((int32_t)(pctHit / 0x2)) + loword((int32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - LOWORD(pctHit))) / 0x8)))))
     */
    goto L_73ea;

L_7386:
    /* untranslated: nds = (uint32_t)((int32_t)(dpShieldLeft / words((hiword((int32_t)(pctHit / 0x2)) + hiword((int32_t)(words((0x0 - HIWORD(pctHit)), (0x64 -
     * LOWORD(pctHit))) / 0x8))), (loword((int32_t)(pctHit / 0x2)) + loword((int32_t)(words((0x0 - HIWORD(pctHit)), (0x64 - LOWORD(pctHit))) / 0x8))))) * 0x64)
     */

L_73ea:
    /* untranslated: ntk = (int32_t)((uint32_t)(words((HIWORD(dpArmorLeft) - hiword((int32_t)((uint32_t)(nds * pctHit) / 0xc8))), (LOWORD(dpArmorLeft) -
     * loword((int32_t)((uint32_t)(nds * pctHit) / 0xc8)))) * 0x64) / (uint32_t)(pctHit * sext16to32((fCapMissile + 0x1)))) */
    if ((HIWORD(nts) > (HIWORD(nds) + HIWORD(ntk))))
        goto L_7476;
    else
        goto L_745e;

L_745e:
    if ((HIWORD(nts) < (HIWORD(nds) + HIWORD(ntk))))
        goto L_746c;
    else
        goto L_7463;

L_7463:
    if ((LOWORD(nts) >= (LOWORD(nds) + LOWORD(ntk))))
        goto L_7476;
    else
        goto L_746c;

L_746c:
    t_merge_7484_0001_wide = nts;
    goto L_7484;

L_7476:
    t_merge_7484_0001_wide = (nds + ntk);

L_7484:
    score = t_merge_7484_0001_wide;
    if ((HIWORD(score) < 0x0))
        goto L_74dc;
    else
        goto L_7493;

L_7493:
    if ((HIWORD(score) > 0x0))
        goto L_74a1;
    else
        goto L_7498;

L_7498:
    if ((LOWORD(score) <= 0x0))
        goto L_74dc;
    else
        goto L_74a1;

L_74a1:
    score = (int32_t)((lValue / score));
    if ((HIWORD(score) > 0x0))
        goto L_750c;
    else
        goto L_74c1;

L_74c1:
    if ((HIWORD(score) < 0x0))
        goto L_74cf;
    else
        goto L_74c6;

L_74c6:
    if ((LOWORD(score) > 0x0))
        goto L_750c;
    else
        goto L_74cf;

L_74cf:
    score = 0x1;

L_74dc:
    score = 0x0;

L_74e9:
    score = 0x0;

L_74f9:
    if ((part.hs.grhst == hstBeam))
        goto L_7095;
    else
        goto L_7501;

L_7501:
    if ((part.hs.grhst != hstTorp))
        goto L_7095;
    else
        goto L_7506;

L_7506:

L_750c:
    if ((HIWORD(score) < HIWORD(scoreBest)))
        goto L_6e38;
    else
        goto L_751a;

L_751a:
    if ((HIWORD(score) > HIWORD(scoreBest)))
        goto L_7527;
    else
        goto L_751f;

L_751f:
    if ((LOWORD(score) <= LOWORD(scoreBest)))
        goto L_6e38;
    else
        goto L_7527;

L_7527:
    scoreBest = score;
    ptokTarget = ptokE;
    itokTarget = itok;

L_7548:
    if ((LOWORD(ptokTarget) != 0x0))
        goto L_756a;
    else
        goto L_7551;

L_7551:
    if ((HIWORD(ptokTarget) != 0x0))
        goto L_756a;
    else
        goto L_7557;

L_7557:

L_755d:
    fPrimary = (fPrimary - 0x1);

L_7561:
    if ((fPrimary >= 0x0))
        goto L_6e0c;
    else
        goto L_756a;

L_756a:
    if ((LOWORD(ptokTarget) != 0x0))
        goto L_757f;
    else
        goto L_7573;

L_7573:
    if ((HIWORD(ptokTarget) == 0x0))
        goto L_6a02;
    else
        goto L_7579;

L_7579:

L_757f:
    dz = DzFromBrcBrc(ptokTarget->brc, ptok->brc);
    goto L_7c71;

L_75a6:
    dp = dpMain;
    if ((ptok->pctCap == 0x0))
        goto L_75f1;
    else
        goto L_75c4;

L_75c4:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptok->pctCap), ptok->pctCap)) / 0x64) */

L_75f1:
    if ((ptokTarget->pctBeamDef >= 0x64))
        goto L_7630;
    else
        goto L_7603;

L_7603:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words(signhiword(ptokTarget->pctBeamDef), ptokTarget->pctBeamDef)) / 0x64) */

L_7630:
    if ((dz <= 0x0))
        goto L_7696;
    else
        goto L_7639;

L_7639:
    if ((part.pbeam->dRangeMax <= 0x0))
        goto L_7696;
    else
        goto L_7646;

L_7646:
    /* untranslated: dp = (int32_t)((uint32_t)(dp * words((0x0 - hiword((int32_t)((uint32_t)(sext16to32(dz) * 0xa) / sext16to32(part.pbeam->dRangeMax)))), (0x64
     * - loword((int32_t)((uint32_t)(sext16to32(dz) * 0xa) / sext16to32(part.pbeam->dRangeMax)))))) / 0x64) */

L_7696:
    if ((part.pbeam->dp < 0xc8))
        goto L_76ac;
    else
        goto L_76a4;

L_76a4:
    grfWeapon = 0x2;
    goto L_76b1;

L_76ac:
    grfWeapon = 0x1;

L_76b1:
    dpT = dp;
    if ((FDamageTok(ptokTarget, itokTarget, &(dp), 0x0, grfWeapon, (part.pbeam->grfAbilities & 0x1), 0x0) == 0x0))
        goto L_7725;
    else
        goto L_76f4;

L_76f4:
    if ((fSetItok != 0x0))
        goto L_7721;
    else
        goto L_76fd;

L_76fd:
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff) | ((itokTarget & 0xff) << 0x8));
    fSetItok = 0x1;

L_7721:
    ctokDamaged = (ctokDamaged + 0x1);

L_7725:
    if ((HIWORD(dp) < 0x0))
        goto L_7804;
    else
        goto L_772e;

L_772e:
    if ((HIWORD(dp) > 0x0))
        goto L_773c;
    else
        goto L_7733;

L_7733:
    if ((LOWORD(dp) <= 0x0))
        goto L_7804;
    else
        goto L_773c;

L_773c:
    if ((HIWORD(dpT) < 0x0))
        goto L_7804;
    else
        goto L_7745;

L_7745:
    if ((HIWORD(dpT) > 0x0))
        goto L_7753;
    else
        goto L_774a;

L_774a:
    if ((LOWORD(dpT) <= 0x0))
        goto L_7804;
    else
        goto L_7753;

L_7753:
    if ((HIWORD(dpMain) > 0x1))
        goto L_77a8;
    else
        goto L_775c;

L_775c:
    if ((HIWORD(dpMain) < 0x1))
        goto L_776a;
    else
        goto L_7761;

L_7761:
    if ((LOWORD(dpMain) >= 0x0))
        goto L_77a8;
    else
        goto L_776a;

L_776a:
    if ((HIWORD(dp) > 0x1))
        goto L_77a8;
    else
        goto L_7773;

L_7773:
    if ((HIWORD(dp) < 0x1))
        goto L_7781;
    else
        goto L_7778;

L_7778:
    if ((LOWORD(dp) >= 0x0))
        goto L_77a8;
    else
        goto L_7781;

L_7781:
    lValue = (int32_t)(((uint32_t)((dpMain * dp)) / dpT));
    goto L_77c5;

L_77a8:
    lValue = __ftol();

L_77c5:
    if (((HIWORD(dpMain) + 0xffff) > HIWORD(lValue)))
        goto L_77f5;
    else
        goto L_77d9;

L_77d9:
    if (((HIWORD(dpMain) + 0xffff) < HIWORD(lValue)))
        goto L_77e6;
    else
        goto L_77de;

L_77de:
    if (((LOWORD(dpMain) + 0xffff) >= LOWORD(lValue)))
        goto L_77f5;
    else
        goto L_77e6;

L_77e6:
    t_merge_77fb_0001_wide = (dpMain + 0xffffffff);
    goto L_77fb;

L_77f5:
    t_merge_77fb_0001_wide = lValue;

L_77fb:
    dpMain = t_merge_77fb_0001_wide;
    goto L_7c84;

L_7804:
    dpMain = 0x0;

L_7811:
    if ((HIWORD(cTorpsLeft) > 0x0))
        goto L_782b;
    else
        goto L_781a;

L_781a:
    if ((HIWORD(cTorpsLeft) < 0x0))
        goto L_7c84;
    else
        goto L_781f;

L_781f:
    if ((LOWORD(cTorpsLeft) <= 0x0))
        goto L_7c84;
    else
        goto L_7825;

L_7825:

L_782b:
    grfWeapon = 0x4;
    cTorpBase = cTorpsLeft;
    cTorpHit = CTorpHit(cTorpBase, ptokTarget, part.ptorp->dHitChance, ptok->pctBC);
    lpshdefE = LpshdefFromTok(ptokTarget);
    LOWORD(dpSingle) = lpshdefE->hul.dp;
    HIWORD(dpSingle) = 0x0;
    /* untranslated: dpShieldLeft = (uint32_t)(words(0x0, ptokTarget->dpShield) * words(0x0, ptokTarget->csh)) */
    /* untranslated: dpArmorLeft = (uint32_t)(dpSingle * words(0x0, ptokTarget->csh)) */
    if ((ptokTarget->dv.dp == 0x0))
        goto L_7954;
    else
        goto L_78d9;

L_78d9:
    /* untranslated: dpArmorLeft = (dpArmorLeft - (int32_t)((uint32_t)((int32_t)((uint32_t)((int32_t)((uint32_t)(dpSingle * words(0x0, ((ptokTarget->dv.dp >>
     * 0x7) & 0x1ff))) / 0xa) * words(0x0, (ptokTarget->dv.dp & 0x7f))) / 0xa) * words(0x0, ptokTarget->csh)) / 0x1f4)) */

L_7954:
    dp = (uint32_t)(part.ptorp->dp);
    if ((part.hs.iItem < itorpJihadMissile))
        goto L_79ad;
    else
        goto L_7970;

L_7970:
    if ((part.hs.iItem > itorpArmageddonMissile))
        goto L_79ad;
    else
        goto L_797e;

L_797e:
    if ((HIWORD(dpShieldLeft) > 0x0))
        goto L_79a9;
    else
        goto L_7987;

L_7987:
    if ((HIWORD(dpShieldLeft) < 0x0))
        goto L_7995;
    else
        goto L_798c;

L_798c:
    if ((LOWORD(dpShieldLeft) > 0x0))
        goto L_79a9;
    else
        goto L_7995;

L_7995:
    dp = (int32_t)((dp * 0x2));

L_79a9:
    grfWeapon = (grfWeapon | 0x8);

L_79ad:
    i = ptokTarget->csh;
    if ((SIGNHIWORD(i) > HIWORD(cTorpBase)))
        goto L_79f6;
    else
        goto L_79c3;

L_79c3:
    if ((SIGNHIWORD(i) < HIWORD(cTorpBase)))
        goto L_79d0;
    else
        goto L_79c8;

L_79c8:
    if ((i >= LOWORD(cTorpBase)))
        goto L_79f6;
    else
        goto L_79d0;

L_79d0:
    if ((HIWORD((uint32_t)((cTorpHit * dp))) > HIWORD(dpArmorLeft)))
        goto L_7b3c;
    else
        goto L_79e9;

L_79e9:
    if ((HIWORD((uint32_t)((cTorpHit * dp))) < HIWORD(dpArmorLeft)))
        goto L_79f6;
    else
        goto L_79ee;

L_79ee:
    if ((LOWORD((uint32_t)((cTorpHit * dp))) > LOWORD(dpArmorLeft)))
        goto L_7b3c;
    else
        goto L_79f6;

L_79f6:
    cTorpFire = cTorpHit;
    cTorpMiss = (cTorpBase - cTorpHit);
    goto L_7b55;

L_7a1a:
    /* untranslated: cTorpFire = (int32_t)(words(((hiword((uint32_t)(sext16to32(i) * cTorpHit)) + HIWORD(cTorpBase)) + 0xffff),
     * ((loword((uint32_t)(sext16to32(i) * cTorpHit)) + LOWORD(cTorpBase)) + 0xffff)) / cTorpBase) */
    cTorpMiss = ((uint32_t)(i)-cTorpFire);
    dpShieldCur = (dpShieldLeft - (int32_t)(((uint32_t)((cTorpMiss * dp)) / 0x8)));
    if ((HIWORD(dpShieldCur) > 0x0))
        goto L_7aab;
    else
        goto L_7a93;

L_7a93:
    if ((HIWORD(dpShieldCur) < 0x0))
        goto L_7aa1;
    else
        goto L_7a98;

L_7a98:
    if ((LOWORD(dpShieldCur) >= 0x0))
        goto L_7aab;
    else
        goto L_7aa1;

L_7aa1:
    dpShieldCur = 0x0;

L_7aab:
    dpShieldCur = (dpShieldCur - (int32_t)(((uint32_t)((cTorpFire * dp)) / 0x2)));
    dpHitArmor = (int32_t)(((uint32_t)((cTorpFire * dp)) / 0x2));
    if ((HIWORD(dpShieldCur) > 0x0))
        goto L_7b1a;
    else
        goto L_7b00;

L_7b00:
    if ((HIWORD(dpShieldCur) < 0x0))
        goto L_7b0e;
    else
        goto L_7b05;

L_7b05:
    if ((LOWORD(dpShieldCur) >= 0x0))
        goto L_7b1a;
    else
        goto L_7b0e;

L_7b0e:
    dpHitArmor = (dpHitArmor - dpShieldCur);

L_7b1a:
    if ((HIWORD(dpHitArmor) < HIWORD(dpArmorLeft)))
        goto L_7b38;
    else
        goto L_7b28;

L_7b28:
    if ((HIWORD(dpHitArmor) > HIWORD(dpArmorLeft)))
        goto L_7b55;
    else
        goto L_7b2d;

L_7b2d:
    if ((LOWORD(dpHitArmor) >= LOWORD(dpArmorLeft)))
        goto L_7b55;
    else
        goto L_7b32;

L_7b32:

L_7b38:
    i = (i + 0x1);

L_7b3c:
    if ((SIGNHIWORD(i) < HIWORD(cTorpBase)))
        goto L_7a1a;
    else
        goto L_7b48;

L_7b48:
    if ((SIGNHIWORD(i) > HIWORD(cTorpBase)))
        goto L_7b55;
    else
        goto L_7b4d;

L_7b4d:
    if ((i <= LOWORD(cTorpBase)))
        goto L_7a1a;
    else
        goto L_7b55;

L_7b55:
    dpCol = (int32_t)(((uint32_t)((cTorpMiss * dp)) / 0x8));
    if ((HIWORD(dpCol) < 0x0))
        goto L_7bca;
    else
        goto L_7b84;

L_7b84:
    if ((HIWORD(dpCol) > 0x0))
        goto L_7b92;
    else
        goto L_7b89;

L_7b89:
    if ((LOWORD(dpCol) <= 0x0))
        goto L_7bca;
    else
        goto L_7b92;

L_7b92:
    if ((FDamageTok(ptokTarget, itokTarget, &(dpCol), 0x0, (grfWeapon | 0x80), 0x1, 0x0) == 0x0))
        goto L_7bca;
    else
        goto L_7bc6;

L_7bc6:
    ctokDamaged = (ctokDamaged + 0x1);

L_7bca:
    dpT = (int32_t)(((uint32_t)((cTorpFire * dp)) / 0x2));
    cTorpBase = (cTorpFire + cTorpMiss);
    FDamageTok(ptokTarget, itokTarget, &(dpT), dpT, grfWeapon, 0x0, &(cTorpBase));
    ctokDamaged = (ctokDamaged + 0x1);
    if ((fSetItok != 0x0))
        goto L_7c59;
    else
        goto L_7c35;

L_7c35:
    fSetItok = 0x1;
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff) | ((itokTarget & 0xff) << 0x8));

L_7c59:
    cTorpsLeft = (cTorpsLeft - (cTorpFire + cTorpMiss));
    goto L_7c84;

L_7c71:
    if ((part.hs.grhst == hstBeam))
        goto L_75a6;
    else
        goto L_7c79;

L_7c79:
    if ((part.hs.grhst == hstTorp))
        goto L_7811;
    else
        goto L_7c84;

L_7c84:
    if ((HIWORD(dpMain) > 0x0))
        goto LFindAnotherTarget;
    else
        goto L_7c8d;

L_7c8d:
    if ((HIWORD(dpMain) < 0x0))
        goto L_7c9b;
    else
        goto L_7c92;

L_7c92:
    if ((LOWORD(dpMain) > 0x0))
        goto LFindAnotherTarget;
    else
        goto L_7c9b;

L_7c9b:
    if ((HIWORD(cTorpsLeft) < 0x0))
        goto L_6a02;
    else
        goto L_7ca4;

L_7ca4:
    if ((HIWORD(cTorpsLeft) > 0x0))
        goto LFindAnotherTarget;
    else
        goto L_7ca9;

L_7ca9:
    if ((LOWORD(cTorpsLeft) > 0x0))
        goto LFindAnotherTarget;
    else
        goto L_7caf;

L_7caf:

L_7cb8:
    lpbtlrec->ctok = ctokDamaged;
    if ((ctokDamaged == 0x0))
        goto L_7cd1;
    else
        goto L_7ccb;

L_7ccb:
    return 0x1;

L_7cd1:
    return 0x0;
}

void KillShips(TOK *ptok, int16_t cshKill, int16_t ishdef, FLEET *lpfl, int16_t fFallout) {
    FLEET   flDead;
    int16_t i;
    FLEET   flSrc;
    int16_t csh;

L_7cde:
    if ((cshKill == 0x0))
        goto L_7ee1;
    else
        goto L_7ced;

L_7ced:

L_7cf3:
    if ((fFallout == 0x0))
        goto L_7d1f;
    else
        goto L_7cfc;

L_7cfc:
    MarkTechsSeen(LpshdefFromTok(ptok), ptok->iplr);

L_7d1f:
    flSrc = *(lpfl);
    memset(flDead, 0x0, 0x7c);
    csh = (lpfl->rgcsh[ishdef] - cshKill);
    flDead.rgcsh[ishdef] = cshKill;
    flSrc.rgcsh[ishdef] = csh;
    ptok->csh = csh;
    if ((csh != 0x0))
        goto L_7e3c;
    else
        goto L_7d9f;

L_7d9f:
    ptok->fActive = 0x0;
    ishdef = 0x0;
    goto L_7dd5;

L_7dbb:
    if ((flSrc.rgcsh[ishdef] != 0x0))
        goto L_7dde;
    else
        goto L_7dcb;

L_7dcb:

L_7dd1:
    ishdef = (ishdef + 0x1);

L_7dd5:
    if ((ishdef < 0x10))
        goto L_7dbb;
    else
        goto L_7dde;

L_7dde:
    if ((ishdef != 0x10))
        goto L_7e3c;
    else
        goto L_7de7;

L_7de7:
    lpfl->fDead = 0x1;
    if ((fFallout == 0x0))
        goto L_7e3c;
    else
        goto L_7e04;

L_7e04:
    i = 0x0;
    goto L_7e33;

L_7e0c:
    flDead.rgwtMin[i] = flSrc.rgwtMin[i];
    i = (i + 0x1);

L_7e33:
    if ((i <= 0x2))
        goto L_7e0c;
    else
        goto L_7e3c;

L_7e3c:
    if ((lpfl->fDead != 0x0))
        goto L_7e83;
    else
        goto L_7e53;

L_7e53:
    flDead.iPlayer = flSrc.iPlayer;
    flDead.fDead = 0x1;
    flDead.det = 0x7;
    FleetTransferCargoBalance(&(flSrc), &(flDead));

L_7e83:
    if ((fFallout == 0x0))
        goto L_7eb8;
    else
        goto L_7e8c;

L_7e8c:
    flDead.iPlayer = flSrc.iPlayer;
    flDead.pt.x = flSrc.pt.x;
    flDead.pt.y = flSrc.pt.y;
    flDead.idPlanet = flSrc.idPlanet;
    CreateSalvage(&(flDead), &(lpthBattle));

L_7eb8:
    if ((lpfl->fDead != 0x0))
        goto L_7ee1;
    else
        goto L_7ecf;

L_7ecf:
    *(lpfl) = flSrc;

L_7ee1:
    return;
}

void CreateSalvage(FLEET *pfl, THING **plpth) {
    int32_t  wtTotal;
    SHDEF   *lpshdefT;
    PLANET  *lppl;
    int16_t  i;
    int32_t  rgwtMinerals[3];
    int16_t  j;
    int16_t  fBleeding;
    SHDEF    shdefT;
    uint32_t t_merge_80cd_0001_wide;

L_7ee8:
    fBleeding = GetRaceGrbit(rgplr[pfl->iPlayer], ibitRaceBleedingEdgeTech);
    gd.fDontCalcBleed = 0x1;
    idPlayer = pfl->iPlayer;
    if ((pfl->idPlanet == 0xffff))
        goto L_7f47;
    else
        goto L_7f30;

L_7f30:
    lppl = LpplFromId(pfl->idPlanet);
    goto L_7f51;

L_7f47:
    lppl = 0x0;

L_7f51:
    i = 0x0;
    goto L_810d;

L_7f59:
    rgwtMinerals[i] = 0x0;
    j = 0x0;
    goto L_8061;

L_7f76:
    if ((pfl->rgcsh[j] <= 0x0))
        goto L_805d;
    else
        goto L_7f8a;

L_7f8a:

L_7f90:
    if ((fBleeding == 0x0))
        goto L_7fe8;
    else
        goto L_7f99;

L_7f99:
    shdefT = rglpshdef[pfl->iPlayer][j];
    UpdateShdefCost(&(shdefT));
    LOWORD(lpshdefT) = shdefT;
    /* untranslated: HIWORD(lpshdefT) = ss */
    goto L_8008;

L_7fe8:
    lpshdefT = &(rglpshdef[pfl->iPlayer][j]);

L_8008:
    /* untranslated: rgwtMinerals[i] = (rgwtMinerals[i] + (int32_t)((uint32_t)(sext16to32(pfl->rgcsh[j]) * words(0x0, lpshdefT->hul.rgwtOreCost[i])) / 0x3)) */

L_805d:
    j = (j + 0x1);

L_8061:
    if ((j < 0x10))
        goto L_7f76;
    else
        goto L_806a;

L_806a:
    rgwtMinerals[i] = (rgwtMinerals[i] + pfl->rgwtMin[i]);
    if ((LOWORD(lppl) != 0x0))
        goto L_80a3;
    else
        goto L_809a;

L_809a:
    if ((HIWORD(lppl) == 0x0))
        goto L_8109;
    else
        goto L_80a3;

L_80a3:
    if ((lppl->fStarbase == 0x0))
        goto L_80c9;
    else
        goto L_80c2;

L_80c2:
    t_merge_80cd_0001_wide = 0x80008;
    goto L_80cd;

L_80c9:
    t_merge_80cd_0001_wide = 0x50005;

L_80cd:
    /* untranslated: LOWORD(lppl->rgwtMin[i]) = (LOWORD(lppl->rgwtMin[i]) + loword((int32_t)(words(hiword((uint32_t)(rgwtMinerals[i] * words(0x0,
     * loword(t_merge_80cd_0001_wide)))), loword((uint32_t)(rgwtMinerals[i] * words(0x0, hiword(t_merge_80cd_0001_wide))))) / 0xa))) */
    /* untranslated: HIWORD(lppl->rgwtMin[i]) = (HIWORD(lppl->rgwtMin[i]) + hiword((int32_t)((uint32_t)(rgwtMinerals[i] * words(0x0,
     * hiword(t_merge_80cd_0001_wide))) / 0xa))) */

L_8109:
    i = (i + 0x1);

L_810d:
    if ((i <= 0x2))
        goto L_7f59;
    else
        goto L_8116;

L_8116:
    if ((LOWORD(lppl) != 0x0))
        goto L_81bb;
    else
        goto L_811f;

L_811f:
    if ((HIWORD(lppl) != 0x0))
        goto L_81bb;
    else
        goto L_8128;

L_8128:
    wtTotal = 0x0;
    i = 0x0;
    goto L_817f;

L_813a:
    rgwtMinerals[i] = (rgwtMinerals[i] - (int32_t)((rgwtMinerals[i] >> 0x2)));
    wtTotal = (wtTotal + rgwtMinerals[i]);
    i = (i + 0x1);

L_817f:
    if ((i <= 0x2))
        goto L_813a;
    else
        goto L_8188;

L_8188:
    if ((LOWORD(wtTotal) != 0x0))
        goto L_819a;
    else
        goto L_8191;

L_8191:
    if ((HIWORD(wtTotal) == 0x0))
        goto L_81bb;
    else
        goto L_819a;

L_819a:
    DropSalvage(plpth, rgwtMinerals, pfl->iPlayer, ((uint8_t *)(pfl) + 0x8));

L_81bb:
    gd.fDontCalcBleed = 0x0;
    idPlayer = 0xffff;
    return;
}

int16_t FDamageTok(TOK *ptok, int16_t itok, int32_t *pdpBeam, int32_t dpTorp, uint16_t grfWeapon, int16_t fShieldsOnly, int32_t *pcTorp) {
    int16_t   pctSh;
    DV        dv;
    uint16_t *pwLosses;
    int16_t   cshOrigDamaged;
    int32_t   dpShdef;
    int32_t   ddpOrig;
    int32_t   dpOrig;
    PLANET   *lppl;
    int16_t   i;
    int16_t   cshOrig;
    FLEET    *lpfl;
    int32_t   cKillMax;
    int16_t   csh;
    int32_t   dpT;
    int16_t   pctDp;
    int16_t   ishdef;
    int32_t   dp;
    uint16_t  pctDpNew;

L_81d4:
    LOWORD(dp) = pdpBeam;
    HIWORD(dp) = *(pdpBeam + 0x2);
    fmemset(&(lpbBattleCur), 0x0, 0x8);
    *(lpbBattleCur) = LOBYTE(itok);
    lpbBattleCur[0x1] = LOBYTE(grfWeapon);
    if ((ptok->dpShield == 0x0))
        goto L_8304;
    else
        goto L_8225;

L_8225:
    LOWORD(dpOrig) = ptok->dpShield;
    HIWORD(dpOrig) = 0x0;
    LOWORD(dpT) = (ptok->dpShield - LOWORD(dpOrig));
    HIWORD(dpT) = (0x0 - HIWORD(dpOrig));
    /* untranslated: dpOrig = (uint32_t)(dpOrig * words(0x0, ptok->csh)) */
    if ((HIWORD(dpOrig) < HIWORD(dp)))
        goto L_82d6;
    else
        goto L_8276;

L_8276:
    if ((HIWORD(dpOrig) > HIWORD(dp)))
        goto L_8283;
    else
        goto L_827b;

L_827b:
    if ((LOWORD(dpOrig) <= LOWORD(dp)))
        goto L_82d6;
    else
        goto L_8283;

L_8283:
    dpOrig = (dpOrig - dp);
    lpbBattleCur[0x4] = WPackLong(dp);
    /* untranslated: ptok->dpShield = (loword((int32_t)(dpOrig / words(0x0, ptok->csh))) + LOWORD(dpT)) */
    dp = 0x0;
    goto L_8313;

L_82d6:
    dp = (dp - dpOrig);
    lpbBattleCur[0x4] = WPackLong(dpOrig);
    ptok->dpShield = 0x0;

L_8304:
    if ((fShieldsOnly == 0x0))
        goto L_8313;
    else
        goto L_830d;

L_830d:
    return 0x0;

L_8313:
    if ((LOWORD(dp) != 0x0))
        goto L_8325;
    else
        goto L_831c;

L_831c:
    if ((HIWORD(dp) == 0x0))
        goto L_832e;
    else
        goto L_8325;

L_8325:
    if ((fShieldsOnly == 0x0))
        goto L_8387;
    else
        goto L_832e;

L_832e:
    if ((LOWORD(dpTorp) != 0x0))
        goto L_8387;
    else
        goto L_8337;

L_8337:
    if ((HIWORD(dpTorp) != 0x0))
        goto L_8387;
    else
        goto L_8340;

L_8340:
    lpbBattleCur[0x6] = ptok->dv.dp;
    pdpBeam = LOWORD(dp);
    *(pdpBeam + 0x2) = HIWORD(dp);
    if (((lpbBattleCur[0x1] & 0x4) == 0x0))
        goto L_837c;
    else
        goto L_8373;

L_8373:
    lpbBattleCur[0x1] = (lpbBattleCur[0x1] | 0xc0);

L_837c:
    lpbBattleCur = (lpbBattleCur + 0x8);
    return 0x1;

L_8387:
    if ((pcTorp == 0x0))
        goto L_83a1;
    else
        goto L_8390;

L_8390:
    LOWORD(cKillMax) = pcTorp;
    HIWORD(cKillMax) = *(pcTorp + 0x2);
    goto L_83ab;

L_83a1:
    cKillMax = 0x7fffffff;

L_83ab:
    dp = (dp + dpTorp);
    ishdef = ptok->ishdef;
    LOWORD(dpShdef) = LpshdefFromTok(ptok)->hul.dp;
    HIWORD(dpShdef) = 0x0;
    dv.dp = ptok->dv.dp;
    if ((ptok->grobj != grobjPlanet))
        goto L_8655;
    else
        goto L_83ff;

L_83ff:
    lppl = LpplFromId(ptok->id);
    cKillMax = (cKillMax - 0x1);
    if ((dv.pctDp == 0x0))
        goto L_845f;
    else
        goto L_842f;

L_842f:
    /* untranslated: dp = (dp + (int32_t)((uint32_t)(dpShdef * words(0x0, dv.pctDp)) / 0x1f4)) */

L_845f:
    if ((HIWORD(dp) < HIWORD(dpShdef)))
        goto L_8514;
    else
        goto L_846d;

L_846d:
    if ((HIWORD(dp) > HIWORD(dpShdef)))
        goto L_847a;
    else
        goto L_8472;

L_8472:
    if ((LOWORD(dp) < LOWORD(dpShdef)))
        goto L_8514;
    else
        goto L_847a;

L_847a:
    lpbBattleCur[0x6] = ((lpbBattleCur[0x6] & 0x7f) | 0xfa00);
    lpbBattleCur[0x2] = 0x1;
    ptok->fActive = 0x0;
    ptok->csh = 0x0;
    fStarbaseDied = 0x1;
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_8601;
    else
        goto L_84e1;

L_84e1:
    lppl->fStarbase = 0x0;
    KillQueuedShips(lppl);
    KillQueuedMassPackets(lppl);

L_8514:
    pctDpNew = LOWORD((int32_t)(((uint32_t)((dp * 0x1f4)) / dpShdef)));
    if ((lppl->pctDp == pctDpNew))
        goto L_859a;
    else
        goto L_8551;

L_8551:
    *(lppl + 0x2c) = (lppl->isb | ((LOWORD((int32_t)(((uint32_t)((dp * 0x1f4)) / dpShdef))) & 0xfff) << 0x4));
    goto L_85c4;

L_859a:
    /* untranslated: ss:[bp-0x38] = ((*(lppl+0x2c) + 0x10) & 0xfff0) */
    lppl->pctDp = 0x0;
    /* untranslated: *(lppl+0x2c) = (*(lppl+0x2c) | ss:[bp-0x38]) */

L_85c4:
    lpbBattleCur[0x6] = ((lpbBattleCur[0x6] & 0x7f) | ((lppl->pctDp & 0x1ff) << 0x7));
    fStarbaseDamaged = 0x1;

L_8601:
    if ((((lpbBattleCur[0x6] >> 0x7) & 0x1ff) == 0x0))
        goto L_863e;
    else
        goto L_8619;

L_8619:
    lpbBattleCur[0x6] = ((lpbBattleCur[0x6] & 0xff80) | 0x64);
    ptok->dv.dp = lpbBattleCur[0x6];

L_863e:
    pdpBeam = 0x0;
    *(pdpBeam + 0x2) = 0x0;
    lpbBattleCur = (lpbBattleCur + 0x8);
    return 0x1;

L_8655:
    if ((ptok->mdTactic != 0x1))
        goto L_8694;
    else
        goto L_866c;

L_866c:
    ptok->mdTactic = 0x0;
    ptok->dzDis = 0x7;

L_8694:
    lpfl = LpflFromId(ptok->id);
    dpOrig = dp;
    csh = ptok->csh;
    cshOrig = csh;
    if ((dv.pctDp == 0x0))
        goto L_875d;
    else
        goto L_86d8;

L_86d8:
    /* untranslated: cshOrigDamaged = loword((int32_t)((uint32_t)(sext16to32(csh) * words(0x0, dv.pctSh)) / 0x64)) */
    if ((cshOrigDamaged != 0x0))
        goto L_870e;
    else
        goto L_8709;

L_8709:
    cshOrigDamaged = 0x1;

L_870e:
    /* untranslated: ddpOrig = (int32_t)((uint32_t)(dpShdef * words(0x0, dv.pctDp)) / 0x1f4) */
    if ((LOWORD(ddpOrig) != 0x0))
        goto L_876c;
    else
        goto L_8747;

L_8747:
    if ((HIWORD(ddpOrig) != 0x0))
        goto L_876c;
    else
        goto L_8750;

L_8750:
    ddpOrig = 0x1;

L_875d:
    cshOrigDamaged = 0x0;
    ddpOrig = 0x0;

L_876c:
    pwLosses = (vrgPlrLosses + (((ptok->iplr << 0x4) + ishdef) * 0x2));
    *(pwLosses) = (*(pwLosses) | 0x8000);
    if ((cshOrigDamaged == 0x0))
        goto L_8836;
    else
        goto L_879c;

L_879c:
    csh = cshOrigDamaged;
    dpShdef = (dpShdef - ddpOrig);

L_87ae:
    if ((HIWORD(dp) < HIWORD(dpShdef)))
        goto L_8815;
    else
        goto L_87bc;

L_87bc:
    if ((HIWORD(dp) > HIWORD(dpShdef)))
        goto L_87c9;
    else
        goto L_87c1;

L_87c1:
    if ((LOWORD(dp) < LOWORD(dpShdef)))
        goto L_8815;
    else
        goto L_87c9;

L_87c9:
    if ((csh == 0x0))
        goto L_8815;
    else
        goto L_87d2;

L_87d2:
    if ((LOWORD(cKillMax) != 0x0))
        goto L_87e4;
    else
        goto L_87db;

L_87db:
    if ((HIWORD(cKillMax) == 0x0))
        goto L_8815;
    else
        goto L_87e4;

L_87e4:
    dp = (dp - dpShdef);
    csh = (csh - 0x1);
    cKillMax = (cKillMax - 0x1);
    if (((*(pwLosses) & 0x1fff) >= 0x1fff))
        goto L_87ae;
    else
        goto L_880c;

L_880c:
    *(pwLosses) = (*(pwLosses) + 0x1);

L_8815:
    dpShdef = (dpShdef + ddpOrig);
    i = cshOrigDamaged;
    cshOrigDamaged = csh;
    csh = (csh + (cshOrig - i));

L_8836:
    if ((HIWORD(dp) < HIWORD(dpShdef)))
        goto L_889d;
    else
        goto L_8844;

L_8844:
    if ((HIWORD(dp) > HIWORD(dpShdef)))
        goto L_8851;
    else
        goto L_8849;

L_8849:
    if ((LOWORD(dp) < LOWORD(dpShdef)))
        goto L_889d;
    else
        goto L_8851;

L_8851:
    if ((csh == 0x0))
        goto L_889d;
    else
        goto L_885a;

L_885a:
    if ((LOWORD(cKillMax) != 0x0))
        goto L_886c;
    else
        goto L_8863;

L_8863:
    if ((HIWORD(cKillMax) == 0x0))
        goto L_889d;
    else
        goto L_886c;

L_886c:
    dp = (dp - dpShdef);
    csh = (csh - 0x1);
    cKillMax = (cKillMax - 0x1);
    if (((*(pwLosses) & 0x1fff) >= 0x1fff))
        goto L_8836;
    else
        goto L_8894;

L_8894:
    *(pwLosses) = (*(pwLosses) + 0x1);

L_889d:
    if ((HIWORD(cKillMax) > 0x0))
        goto L_88be;
    else
        goto L_88a6;

L_88a6:
    if ((HIWORD(cKillMax) < 0x0))
        goto L_88b4;
    else
        goto L_88ab;

L_88ab:
    if ((LOWORD(cKillMax) > 0x0))
        goto L_88be;
    else
        goto L_88b4;

L_88b4:
    dp = 0x0;

L_88be:
    if ((LOWORD(dp) != 0x0))
        goto L_88d0;
    else
        goto L_88c7;

L_88c7:
    if ((HIWORD(dp) == 0x0))
        goto L_898b;
    else
        goto L_88d0;

L_88d0:
    if ((csh == 0x0))
        goto L_898b;
    else
        goto L_88d9;

L_88d9:
    if ((cshOrigDamaged == 0x0))
        goto L_8913;
    else
        goto L_88e2;

L_88e2:
    dp = (dp + (((uint32_t)((ddpOrig * (uint32_t)(cshOrigDamaged))) + (uint32_t)(csh)) + 0xffffffff));

L_8913:
    dp = (int32_t)((dp / (uint32_t)(csh)));
    if ((LOWORD(dp) != 0x0))
        goto L_8946;
    else
        goto L_8933;

L_8933:
    if ((HIWORD(dp) != 0x0))
        goto L_8946;
    else
        goto L_893c;

L_893c:
    dp = 0x1;

L_8946:
    /* untranslated: pctDp = loword((int32_t)(words(((hiword((uint32_t)(dp * 0x1f4)) + HIWORD(dpShdef)) + 0xffff), ((loword((uint32_t)(dp * 0x1f4)) +
     * LOWORD(dpShdef)) + 0xffff)) / dpShdef)) */
    if ((pctDp != 0x0))
        goto L_8983;
    else
        goto L_897e;

L_897e:
    pctDp = 0x1;

L_8983:
    pctSh = 0x64;
    goto L_89f1;

L_898b:
    if ((cshOrigDamaged == 0x0))
        goto L_89e6;
    else
        goto L_8994;

L_8994:
    /* untranslated: pctSh = loword((int32_t)(words(((hiword((uint32_t)(sext16to32(cshOrigDamaged) * 0x64)) + signhiword(csh)) + 0xffff),
     * ((loword((uint32_t)(sext16to32(cshOrigDamaged) * 0x64)) + csh) + 0xffff)) / sext16to32(csh))) */
    pctDp = ((ptok->dv.dp >> 0x7) & 0x1ff);
    goto L_89f1;

L_89e6:
    pctSh = 0x0;
    pctDp = 0x0;

L_89f1:
    lpbBattleCur[0x2] = (ptok->csh - csh);
    if ((csh == ptok->csh))
        goto L_8a38;
    else
        goto L_8a14;

L_8a14:
    KillShips(ptok, lpbBattleCur[0x2], ishdef, lpfl, 0x1);

L_8a38:
    if ((csh == 0x0))
        goto L_8aa5;
    else
        goto L_8a41;

L_8a41:
    if ((pctDp <= 0x1f3))
        goto L_8a50;
    else
        goto L_8a4b;

L_8a4b:
    pctDp = 0x1f3;

L_8a50:
    dv.dp = (dv.pctSh | ((pctDp & 0x1ff) << 0x7));
    dv.dp = ((dv.dp & 0xff80) | (pctSh & 0x7f));
    ptok->dv.dp = dv.dp;
    /* untranslated: HIWORD(lpfl):[((LOWORD(lpfl) + 0x2c) + (ishdef * 0x2))] = dv.dp */
    dp = 0x0;

L_8aa5:
    if ((HIWORD(dp) < HIWORD(dpTorp)))
        goto L_8ad7;
    else
        goto L_8ab3;

L_8ab3:
    if ((HIWORD(dp) > HIWORD(dpTorp)))
        goto L_8ac0;
    else
        goto L_8ab8;

L_8ab8:
    if ((LOWORD(dp) <= LOWORD(dpTorp)))
        goto L_8ad7;
    else
        goto L_8ac0;

L_8ac0:
    pdpBeam = (LOWORD(dp) - LOWORD(dpTorp));
    *(pdpBeam + 0x2) = (HIWORD(dp) - HIWORD(dpTorp));
    goto L_8ae3;

L_8ad7:
    pdpBeam = 0x0;
    *(pdpBeam + 0x2) = 0x0;

L_8ae3:
    LOWORD(dpOrig) = (LOWORD(dpOrig) - pdpBeam);
    HIWORD(dpOrig) = (HIWORD(dpOrig) - *(pdpBeam + 0x2));
    lpbBattleCur[0x6] = ptok->dv.dp;
    lpbBattleCur = (lpbBattleCur + 0x8);
    if ((pcTorp == 0x0))
        goto L_8b1c;
    else
        goto L_8b0e;

L_8b0e:
    pcTorp = LOWORD(cKillMax);
    *(pcTorp + 0x2) = HIWORD(cKillMax);

L_8b1c:
    return 0x1;
}

int16_t DxyFromSpdRound(uint16_t spd, int16_t iRound) {
    int16_t  dxy;
    uint16_t t_merge_8b61_0001;
    uint16_t t_merge_8b7e_0001;
    uint16_t t_merge_8b9b_0001;

L_8b28:
    /* untranslated: dxy = (words((spd + 0x2), 0x0) / 0x4) */
    goto L_8ba4;

L_8b4a:
    if (((iRound & 0x1) != 0x0))
        goto L_8b5e;
    else
        goto L_8b58;

L_8b58:
    t_merge_8b61_0001 = 0x1;
    goto L_8b61;

L_8b5e:
    t_merge_8b61_0001 = 0x0;

L_8b61:
    dxy = (dxy + t_merge_8b61_0001);
    goto L_8bbf;

L_8b67:
    if (((iRound & 0x3) == 0x2))
        goto L_8b7b;
    else
        goto L_8b75;

L_8b75:
    t_merge_8b7e_0001 = 0x1;
    goto L_8b7e;

L_8b7b:
    t_merge_8b7e_0001 = 0x0;

L_8b7e:
    dxy = (dxy + t_merge_8b7e_0001);
    goto L_8bbf;

L_8b84:
    if (((iRound & 0x3) != 0x0))
        goto L_8b98;
    else
        goto L_8b92;

L_8b92:
    t_merge_8b9b_0001 = 0x1;
    goto L_8b9b;

L_8b98:
    t_merge_8b9b_0001 = 0x0;

L_8b9b:
    dxy = (dxy + t_merge_8b9b_0001);
    goto L_8bbf;

L_8ba4:
    if (((spd & 0x3) == 0x0))
        goto L_8b4a;
    else
        goto L_8bac;

L_8bac:
    if (((spd & 0x3) == 0x1))
        goto L_8b67;
    else
        goto L_8bb4;

L_8bb4:
    if (((spd & 0x3) == 0x3))
        goto L_8b84;
    else
        goto L_8bbf;

L_8bbf:

L_8bc5:
    return dxy;
}

int16_t FDoCoolBattle(FLEET *lpfl, int16_t cplr, uint16_t *rggrfAttack, uint16_t grfPlayer, uint16_t grfSpectator) {
    int16_t  cShipsInvolved;
    uint8_t *lpbMax;
    TOK     *ptok;
    uint16_t wt;
    int16_t  cShdefsInvolved;
    uint8_t *lpbSav;
    int16_t  initMac;
    int16_t  init;
    uint16_t wtT;
    uint16_t grplrLeft;
    int16_t  i;
    int16_t  j;
    int16_t  initMin;
    BTLREC  *lpbtlrec;
    int16_t  iRound;
    FLEET   *lpflT;
    uint16_t brcOrig;
    BTLDATA *lpbtldata;
    uint8_t  rgfInit[64];
    uint16_t rgPlrLosses[256];
    uint16_t wtNext;
    int16_t  itok;
    jmp_buf  env[9];
    jmp_buf *penvMemSav[9];
    PLANET  *lppl;
    int32_t  lwt;
    int16_t  t_8f4b;
    uint16_t t_922e;

L_8bcc:
    if ((LOWORD(lpbBattleLog) != 0x0))
        goto L_8c3e;
    else
        goto L_8bdf;

L_8bdf:
    if ((HIWORD(lpbBattleLog) != 0x0))
        goto L_8c3e;
    else
        goto L_8be9;

L_8be9:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_8c19;
    else
        goto L_8c0c;

L_8c0c:
    penvMem = penvMemSav;
    return 0xffff;

L_8c19:
    lpbBattleLog = LpAlloc(0xffc8, htBattle);
    lpbBattleCur = lpbBattleLog;

L_8c3e:
    if ((LOWORD(lpbBattleT) != 0x0))
        goto L_8c99;
    else
        goto L_8c48;

L_8c48:
    if ((HIWORD(lpbBattleT) != 0x0))
        goto L_8c99;
    else
        goto L_8c52;

L_8c52:
    penvMemSav = penvMem;
    penvMem = env;
    if ((setjmp(env) == 0x0))
        goto L_8c82;
    else
        goto L_8c75;

L_8c75:
    penvMem = penvMemSav;
    return 0xffff;

L_8c82:
    lpbBattleT = LpAlloc(0xffc8, htBattle);

L_8c99:
    lpbSav = lpbBattleCur;
    lpbBattleCur = lpbBattleT;
    lpbMax = &(*(lpbBattleT - 0x48));
    memset(rgPlrLosses, 0x0, 0x200);
    vrgPlrLosses = rgPlrLosses;
    memset(rgfInit, 0x0, 0x40);
    fmemset(&(vrgtok), 0x0, 0x1d00);
    vctok = 0x0;
    lpbtldata = lpbBattleCur;
    lpbBattleCur = (lpbBattleCur + 0xe);
    memset(&(rgTechBattle), 0x0, 0x6);
    memset(&(rgTechTrader), 0x0, 0xd);
    lpthBattle = 0x0;
    cShdefsInvolved = 0x0;
    cShipsInvolved = 0x0;
    fStarbaseDied = 0x0;
    fStarbaseDamaged = 0x0;
    lpflT = lpfl;

L_8d7e:
    if ((lpflT->fDead != 0x0))
        goto L_8dff;
    else
        goto L_8d95;

L_8d95:
    i = 0x0;
    goto L_8df6;

L_8d9d:
    if ((lpflT->rgcsh[i] <= 0x0))
        goto L_8df2;
    else
        goto L_8dba;

L_8dba:
    cShipsInvolved = (cShipsInvolved + lpflT->rgcsh[i]);
    rgPlrLosses[((lpflT->iPlayer << 0x4) + i)] = 0x8000;

L_8df2:
    i = (i + 0x1);

L_8df6:
    if ((i < 0x10))
        goto L_8d9d;
    else
        goto L_8dff;

L_8dff:
    lpflT = lpflT->lpflNext;
    if ((LOWORD(lpflT) != LOWORD(lpfl)))
        goto L_8e26;
    else
        goto L_8e1e;

L_8e1e:
    if ((HIWORD(lpflT) == HIWORD(lpfl)))
        goto L_8e38;
    else
        goto L_8e26;

L_8e26:
    if ((LOWORD(lpflT) != 0x0))
        goto L_8d7e;
    else
        goto L_8e2f;

L_8e2f:
    if ((HIWORD(lpflT) != 0x0))
        goto L_8d7e;
    else
        goto L_8e38;

L_8e38:
    i = 0x0;
    goto L_8e6a;

L_8e40:
    if ((rgPlrLosses[i] == 0x0))
        goto L_8e66;
    else
        goto L_8e53;

L_8e53:
    rgPlrLosses[i] = 0x0;
    cShdefsInvolved = (cShdefsInvolved + 0x1);

L_8e66:
    i = (i + 0x1);

L_8e6a:
    if ((i < 0x100))
        goto L_8e40;
    else
        goto L_8e74;

L_8e74:
    if ((lpfl->idPlanet == 0xffff))
        goto L_8ee6;
    else
        goto L_8e81;

L_8e81:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->fStarbase == 0x0))
        goto L_8ee6;
    else
        goto L_8eb0;

L_8eb0:
    if ((((0x1 << lppl->iPlayer) & grfPlayer) == 0x0))
        goto L_8ee6;
    else
        goto L_8ec8;

L_8ec8:
    cShdefsInvolved = (cShdefsInvolved + 0x1);
    cShipsInvolved = (cShipsInvolved + 0x1);
    lppl->fNoHeal = 0x1;

L_8ee6:
    /* untranslated: call InitializeBoard(lpfl, (words(loword(((cplr + 0xffff) * cplr)), signhiword(loword(((cplr + 0xffff) * cplr)))) / 0x2), grfPlayer,
     * rgfInit, &initMin, &initMac) -> callresult(void) */
    lpbtldata->cplr = LOBYTE(cplr);
    lpbtldata->ctok = LOBYTE(vctok);
    lpbtldata->idPlanet = lpfl->idPlanet;
    lpbtldata->pt.x = lpfl->pt.x;
    lpbtldata->pt.y = lpfl->pt.y;
    t_8f4b = idBattle;
    idBattle = (idBattle + 0x1);
    lpbtldata->id = t_8f4b;
    iRound = 0x0;
    goto L_982b;

L_8f61:
    grplrLeft = 0x0;
    itok = 0x0;
    goto L_9092;

L_8f6f:
    if (((vrgtok[itok].wFlags & 0x1) == 0x0))
        goto L_908d;
    else
        goto L_8f91;

L_8f91:
    grplrLeft = (grplrLeft | (0x1 << vrgtok[itok].iplr));
    /* untranslated: part[25:2](vrgtok[itok]) = ((part[25:2](vrgtok[itok]) & 0xfff) | 0x0) */
    if ((iRound <= 0x0))
        goto L_908d;
    else
        goto L_8ffa;

L_8ffa:
    if ((vrgtok[itok].dpShield <= 0x0))
        goto L_908d;
    else
        goto L_9017;

L_9017:
    if (((vrgtok[itok].wFlags & 0x1) == 0x0))
        goto L_908d;
    else
        goto L_9039;

L_9039:
    if ((GetRaceGrbit(rgplr[vrgtok[itok].iplr], ibitRaceRegeneratingShields) == 0x0))
        goto L_908d;
    else
        goto L_9072;

L_9072:
    RegenShield(&(vrgtok[itok]));

L_908d:
    itok = (itok + 0x1);

L_9092:
    if ((itok < vctok))
        goto L_8f6f;
    else
        goto L_909e;

L_909e:
    if ((((grplrLeft + 0xffff) & grplrLeft) == 0x0))
        goto L_9834;
    else
        goto L_90ac;

L_90ac:

L_90b2:
    ptok = vrgtok;
    itok = 0x0;
    goto L_90de;

L_90cb:
    ptok = (ptok + 0x1);
    itok = (itok + 0x1);

L_90de:
    if ((itok >= vctok))
        goto L_9169;
    else
        goto L_90ea;

L_90ea:
    if ((ptok->fActive == 0x0))
        goto L_90cb;
    else
        goto L_90fc;

L_90fc:
    if ((ptok->grobj != grobjPlanet))
        goto L_9125;
    else
        goto L_910e;

L_910e:
    ptok->dMovesLeft = 0x0;
    goto L_90cb;

L_9125:
    ptok->wFlags = ((ptok->wFlags & 0x3fff) | ((DxyFromSpdRound(ptok->spd, iRound) & 0x3) << 0xe));

L_9169:
    j = 0x3;
    goto L_94c6;

L_9171:
    wtNext = 0x0;
    wt = 0x7530;
    i = vctok;

L_9182:
    wtNext = 0x0;
    ptok = vrgtok;
    itok = 0x0;
    goto L_91b4;

L_91a1:
    ptok = (ptok + 0x1);
    itok = (itok + 0x1);

L_91b4:
    if ((itok >= vctok))
        goto L_94b1;
    else
        goto L_91c0;

L_91c0:
    if ((ptok->fActive == 0x0))
        goto L_91df;
    else
        goto L_91d2;

L_91d2:
    if ((ptok->wt != 0xffff))
        goto L_91e6;
    else
        goto L_91df;

L_91df:
    i = (i - 0x1);
    goto L_91a1;

L_91e6:
    LOWORD(lwt) = ptok->dwt;
    HIWORD(lwt) = 0x0;
    lwt = (lwt - 0x7);
    lwt = (int32_t)((lwt * 0x2));
    t_922e = LOWORD(lwt);
    /* untranslated: LOWORD(lwt) = (ptok->wt + loword((int32_t)((uint32_t)(words(0x0, ptok->wt) * lwt) / 0x64))) */
    /* untranslated: HIWORD(lwt) = (0x0 + hiword((int32_t)((uint32_t)(words(0x0, ptok->wt) * words(HIWORD(lwt), t_922e)) / 0x64))) */
    wtT = LOWORD(lwt);
    if ((wtT <= wtNext))
        goto L_92ac;
    else
        goto L_9277;

L_9277:
    if ((wtT >= wt))
        goto L_92ac;
    else
        goto L_9282;

L_9282:
    if ((DxyFromSpdRound(ptok->spd, iRound) == 0x0))
        goto L_92ac;
    else
        goto L_92a5;

L_92a5:
    wtNext = wtT;

L_92ac:
    if ((wtT != wt))
        goto L_91a1;
    else
        goto L_92b7;

L_92b7:
    i = (i - 0x1);
    if ((ptok->dMovesLeft < j))
        goto L_91a1;
    else
        goto L_92cf;

L_92cf:

L_92d5:
    lpbtlrec = lpbBattleCur;
    lpbBattleCur = (lpbBattleCur + 0x6);
    lpbtlrec->itok = LOBYTE(itok);
    lpbtlrec->ctok = 0x0;
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff) | ((itok & 0xff) << 0x8));
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xfff0) | (iRound & 0xf));
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff0f) | ((ptok->dzDis & 0xf) << 0x4));
    brcOrig = vrgtok[itok].brc;
    if ((ptok->mdTactic != 0x0))
        goto L_9411;
    else
        goto L_93a4;

L_93a4:
    brcOrig = 0xff;
    if ((ptok->dzDis != 0x0))
        goto L_93e4;
    else
        goto L_93c5;

L_93c5:
    lpbtlrec->brcDest = 0xff;
    ptok->fActive = 0x0;
    goto L_91a1;

L_93e4:
    /* untranslated: ss:[bp-0x27c] = ((ptok->wFlags + 0xffe0) & 0x3e0) */
    ptok->dzDis = 0x0;
    /* untranslated: ptok->wFlags = (ptok->wFlags | ss:[bp-0x27c]) */

L_9411:
    DxyMoveTokTo(ptok, j, rggrfAttack[ptok->iplr]);
    /* untranslated: ss:[bp-0x27c] = ((ptok->wFlags + 0xc000) & 0xc000) */
    ptok->dMovesLeft = 0x0;
    /* untranslated: ptok->wFlags = (ptok->wFlags | ss:[bp-0x27c]) */
    if ((ptok->grobj == grobjPlanet))
        goto L_94a9;
    else
        goto L_9474;

L_9474:
    if ((brcOrig != ptok->brc))
        goto L_9498;
    else
        goto L_9486;

L_9486:
    if ((ptok->initMin != 0xff))
        goto L_94a9;
    else
        goto L_9498;

L_9498:
    lpbtlrec->brcDest = ptok->brc;
    goto L_91a1;

L_94a9:
    lpbBattleCur = (lpbBattleCur - 0x6);

L_94b1:
    wt = wtNext;
    if ((wtNext != 0x0))
        goto L_9182;
    else
        goto L_94c2;

L_94c2:
    j = (j - 0x1);

L_94c6:
    if ((j > 0x0))
        goto L_9171;
    else
        goto L_94cf;

L_94cf:
    grplrLeft = 0x0;
    i = 0x0;
    goto L_9581;

L_94dc:
    if (((vrgtok[i].wFlags & 0x1) == 0x0))
        goto L_957d;
    else
        goto L_94fd;

L_94fd:
    vrgtok[i].wFlags = ((vrgtok[i].wFlags & 0xc3ff) | ((Random(0xf) & 0xf) * 0x400));
    grplrLeft = (grplrLeft | (0x1 << vrgtok[i].iplr));

L_957d:
    i = (i + 0x1);

L_9581:
    if ((i < vctok))
        goto L_94dc;
    else
        goto L_958c;

L_958c:
    i = 0x0;
    goto L_9598;

L_9594:
    i = (i + 0x1);

L_9598:
    if ((i >= game.cPlayer))
        goto L_95dd;
    else
        goto L_95a3;

L_95a3:
    if ((((0x1 << i) & grplrLeft) == 0x0))
        goto L_9594;
    else
        goto L_95b6;

L_95b6:
    if (((grplrLeft & rggrfAttack[i]) != 0x0))
        goto L_9594;
    else
        goto L_95cd;

L_95cd:
    /* untranslated: grplrLeft = (grplrLeft & ((0x1 << i) ~ 0x0)) */

L_95dd:
    if ((((grplrLeft + 0xffff) & grplrLeft) == 0x0))
        goto L_9834;
    else
        goto L_95eb;

L_95eb:

L_95f1:
    init = initMac;
    goto L_9808;

L_95fa:
    if ((rgfInit[init] == 0x0))
        goto L_9804;
    else
        goto L_960f;

L_960f:
    itok = (vctok + 0xffff);
    goto L_97fa;

L_961c:
    if ((init < vrgtok[itok].initMin))
        goto L_97f5;
    else
        goto L_963e;

L_963e:
    if ((init > vrgtok[itok].initMac))
        goto L_97f5;
    else
        goto L_9660;

L_9660:
    grplrLeft = 0x0;
    i = 0x0;
    goto L_96b5;

L_966d:
    if (((vrgtok[i].wFlags & 0x1) == 0x0))
        goto L_96b1;
    else
        goto L_968e;

L_968e:
    grplrLeft = (grplrLeft | (0x1 << vrgtok[i].iplr));

L_96b1:
    i = (i + 0x1);

L_96b5:
    if ((i < vctok))
        goto L_966d;
    else
        goto L_96c0;

L_96c0:
    if ((((grplrLeft + 0xffff) & grplrLeft) == 0x0))
        goto L_9804;
    else
        goto L_96ce;

L_96ce:

L_96d4:
    ptok = &(vrgtok[itok]);
    if ((ptok->fActive == 0x0))
        goto L_97f5;
    else
        goto L_96fa;

L_96fa:

L_9700:
    lpbtlrec = lpbBattleCur;
    lpbBattleCur = (lpbBattleCur + 0x6);
    lpbtlrec->itok = LOBYTE(itok);
    lpbtlrec->ctok = 0x0;
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xfff0) | (iRound & 0xf));
    lpbtlrec->brcDest = ptok->brc;
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff) | ((itok & 0xff) << 0x8));
    *(lpbtlrec + 0x4) = ((*(lpbtlrec + 0x4) & 0xff0f) | ((ptok->dzDis & 0xf) << 0x4));
    if ((FAttack(itok, init, lpbtlrec, rggrfAttack[ptok->iplr]) != 0x0))
        goto L_97e1;
    else
        goto L_97d9;

L_97d9:
    lpbBattleCur = (lpbBattleCur - 0x6);
    goto L_97f5;

L_97e1:
    ptok->fMoved = 0x0;

L_97f5:
    itok = (itok - 0x1);

L_97fa:
    if ((itok >= 0x0))
        goto L_961c;
    else
        goto L_9804;

L_9804:
    init = (init - 0x1);

L_9808:
    if ((init >= initMin))
        goto L_95fa;
    else
        goto L_9813;

L_9813:
    if ((((grplrLeft + 0xffff) & grplrLeft) == 0x0))
        goto L_9834;
    else
        goto L_9821;

L_9821:

L_9827:
    iRound = (iRound + 0x1);

L_982b:
    if ((iRound < 0x10))
        goto L_8f61;
    else
        goto L_9834;

L_9834:
    lpbtldata->cbData = (LOWORD(lpbBattleCur) - LOWORD(lpbtldata));
    SendBattleMessages(lpfl, cplr, lpbtldata->id, rgPlrLosses, grfPlayer, cShipsInvolved, cShdefsInvolved, grfSpectator);
    lpbtldata->grfPlr = grfPlayer;
    if ((0x0 > 0x0))
        goto L_98d9;
    else
        goto L_98b6;

L_98b6:
    /* untranslated: branch ss:[bp-0x278] < 0x0 ? L_98c2 : L_98bb */

L_98bb:
    /* untranslated: branch ss:[bp-0x27a] >= lpbtldata->cbData ? L_98d9 : L_98c2 */

L_98c2:
    LOWORD(lpbSav) = 0xffff;
    lpbBattleT = 0x0;
    goto L_990b;

L_98d9:
    fmemmove(&(lpbSav), &(lpbtldata), lpbtldata->cbData);
    lpbBattleCur = &(lpbSav[lpbtldata->cbData]);

L_990b:
    return 0x1;
}

int16_t ITechLearnATech(int16_t iplr, int16_t x, int16_t y, MessageId idm, uint16_t *piGoto) {
    uint16_t iGoto;
    int16_t  fBattle;
    int16_t  i;
    int16_t  iTech;
    int32_t  l;
    uint16_t t_merge_9933_0001;

L_9918:
    if ((idm == 0xffff))
        goto L_9930;
    else
        goto L_992a;

L_992a:
    t_merge_9933_0001 = 0x1;
    goto L_9933;

L_9930:
    t_merge_9933_0001 = 0x0;

L_9933:
    fBattle = t_merge_9933_0001;
    if ((((rgplr[iplr].wFlags >> 0x3) & 0x1) != 0x0))
        goto L_9969;
    else
        goto L_9955;

L_9955:
    if ((Random(0x64) >= 0x32))
        goto L_996f;
    else
        goto L_9969;

L_9969:
    return 0x0;

L_996f:
    i = 0x0;
    goto L_9a6f;

L_9977:
    iTech = Random(0xd);
    if ((rgTechTrader[iTech] == 0x0))
        goto L_9a6b;
    else
        goto L_9998;

L_9998:
    if ((((0x1 << iTech) & rgplr[iplr].grbitTrader) != 0x0))
        goto L_9a6b;
    else
        goto L_99bc;

L_99bc:
    if ((Random(0x64) >= rgTechTrader[iTech]))
        goto L_9a6b;
    else
        goto L_99de;

L_99de:

L_99e4:
    idm = IdmGiveTraderPart((0x1 << iTech), iplr, &(iGoto));
    if ((fBattle == 0x0))
        goto L_9a27;
    else
        goto L_9a08;

L_9a08:
    idm = (idm + 0x2f);
    FSendPlrMsg2(iplr, idm, iGoto, x, y);
    goto L_9a38;

L_9a27:
    if ((piGoto == 0x0))
        goto L_9a38;
    else
        goto L_9a30;

L_9a30:
    *(piGoto) = iGoto;

L_9a38:
    rgplr[iplr].wFlags = ((rgplr[iplr].wFlags & 0xfff7) | 0x8);
    /* untranslated: return ((iTech + 0x1) neg 0x0) */

L_9a6b:
    i = (i + 0x1);

L_9a6f:
    if ((i < 0xd))
        goto L_9977;
    else
        goto L_9a78;

L_9a78:
    i = 0x0;
    goto L_9bf8;

L_9a80:
    iTech = Random(0x6);
    if (((uint16_t)(rgplr[iplr].rgTech[iTech]) >= rgTechBattle[iTech]))
        goto L_9bf4;
    else
        goto L_9ac0;

L_9ac0:
    l = GetTechLevelCost(iTech, ((uint16_t)(rgplr[iplr].rgTech[iTech]) + 0x1), iplr);
    if ((game.fSlowTech == 0x0))
        goto L_9b14;
    else
        goto L_9b00;

L_9b00:
    l = (int32_t)((l >> 0x1));

L_9b14:
    rgplr[iplr].rgResSpent[iTech] = (rgplr[iplr].rgResSpent[iTech] + l);
    if ((fBattle == 0x0))
        goto L_9bb3;
    else
        goto L_9b4d;

L_9b4d:
    if ((game.fSlowTech == 0x0))
        goto L_9b71;
    else
        goto L_9b5d;

L_9b5d:
    l = (int32_t)((l * 0x2));

L_9b71:
    FSendPlrMsg(iplr, idm, 0xfffe, x, y, iTech, LOWORD(l), (LOWORD((uint32_t)((l >> 0x10))) & 0xffff), 0x0, 0x0);
    goto L_9bc3;

L_9bb3:
    if ((piGoto == 0x0))
        goto L_9bc3;
    else
        goto L_9bbc;

L_9bbc:
    *(piGoto) = 0xfffe;

L_9bc3:
    rgplr[iplr].wFlags = ((rgplr[iplr].wFlags & 0xfff7) | 0x8);
    return (iTech + 0x1);

L_9bf4:
    i = (i + 0x1);

L_9bf8:
    if ((i < 0x6))
        goto L_9a80;
    else
        goto L_9c01;

L_9c01:
    return 0x0;
}

void SendBattleMessages(FLEET *lpflBtl, int16_t cplr, int16_t idBtl, uint16_t *rgPlrLosses, int16_t grfPlayer, int16_t cShipsInvolved, int16_t cShdefsInvolved,
                        uint16_t grfSpectator) {
    int16_t   iplrStarbase;
    int16_t   iplr;
    uint8_t   rgcfl[16];
    int32_t   lpopStarbase;
    uint16_t *pw;
    int16_t   isb;
    PLANET   *lppl;
    uint16_t *pwThem;
    int16_t   fAlive;
    int16_t   cUs;
    int16_t   y;
    FLEET    *lpfl;
    int16_t   cThemDead;
    int16_t   i;
    MessageId idm;
    int16_t   j;
    FLEET    *lpflT;
    int16_t   cUsDead;
    int16_t   iThem;
    uint16_t *pwUs;
    int16_t   cThem;
    int16_t   x;

L_9c0e:
    iplrStarbase = 0xffff;
    lppl = 0x0;
    memset(rgcfl, 0x0, 0x10);
    if ((lpflBtl->idPlanet == 0xffff))
        goto L_9d01;
    else
        goto L_9c47;

L_9c47:
    x = 0xffff;
    y = lpflBtl->idPlanet;
    lppl = LpplFromId(y);
    iThem = lppl->iPlayer;
    iplr = lppl->iPlayer;
    if ((iplr == 0xffff))
        goto L_9d15;
    else
        goto L_9c7d;

L_9c7d:
    if ((lppl->fStarbase != 0x0))
        goto L_9c9e;
    else
        goto L_9c94;

L_9c94:
    if ((fStarbaseDied == 0x0))
        goto L_9cb1;
    else
        goto L_9c9e;

L_9c9e:
    iplrStarbase = iplr;
    isb = lppl->isb;

L_9cb1:
    if ((fStarbaseDied == 0x0))
        goto L_9d15;
    else
        goto L_9cbb;

L_9cbb:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_9d15;
    else
        goto L_9cdf;

L_9cdf:
    lpopStarbase = lppl->rgwtMin[0x3];
    UninhabitPlanet(lppl);

L_9d01:
    x = lpflBtl->pt.x;
    y = lpflBtl->pt.y;

L_9d15:
    lpfl = lpflBtl;
    lpflT = 0x0;

L_9d2b:
    if ((lpfl->fDead != 0x0))
        goto L_9d91;
    else
        goto L_9d42;

L_9d42:
    i = 0x0;
    goto L_9d88;

L_9d4a:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_9d84;
    else
        goto L_9d67;

L_9d67:
    rgPlrLosses[((lpfl->iPlayer << 0x4) + i)] = (rgPlrLosses[((lpfl->iPlayer << 0x4) + i)] | 0x4000);

L_9d84:
    i = (i + 0x1);

L_9d88:
    if ((i < 0x10))
        goto L_9d4a;
    else
        goto L_9d91;

L_9d91:
    lpfl = lpfl->lpflNext;
    if ((LOWORD(lpfl) != LOWORD(lpflBtl)))
        goto L_9db8;
    else
        goto L_9db0;

L_9db0:
    if ((HIWORD(lpfl) == HIWORD(lpflBtl)))
        goto L_9dca;
    else
        goto L_9db8;

L_9db8:
    if ((LOWORD(lpfl) != 0x0))
        goto L_9d2b;
    else
        goto L_9dc1;

L_9dc1:
    if ((HIWORD(lpfl) != 0x0))
        goto L_9d2b;
    else
        goto L_9dca;

L_9dca:
    iplr = 0x0;
    goto L_9dd6;

L_9dd2:
    iplr = (iplr + 0x1);

L_9dd6:
    if ((iplr >= game.cPlayer))
        goto L_adff;
    else
        goto L_9de1;

L_9de1:
    if ((((0x1 << iplr) & grfPlayer) == 0x0))
        goto L_abeb;
    else
        goto L_9df4;

L_9df4:
    fAlive = 0x1;
    if ((fStarbaseDied == 0x0))
        goto L_9eb8;
    else
        goto L_9e03;

L_9e03:
    if ((GetRaceStat(rgplr[iplrStarbase], rsMajorAdv) != raMacintosh))
        goto L_9eb8;
    else
        goto L_9e23;

L_9e23:
    if ((iplr != iplrStarbase))
        goto L_9e56;
    else
        goto L_9e2e;

L_9e2e:
    if ((HIWORD(lpopStarbase) < 0x0))
        goto L_9e4e;
    else
        goto L_9e37;

L_9e37:
    if ((HIWORD(lpopStarbase) > 0x0))
        goto L_9e46;
    else
        goto L_9e3c;

L_9e3c:
    if ((LOWORD(lpopStarbase) <= 0x3e8))
        goto L_9e4e;
    else
        goto L_9e46;

L_9e46:
    idm = idmBattleTookPlaceDestroyedScreamsColonistsEcho;
    goto L_9e5b;

L_9e4e:
    idm = idmBattleTookPlaceDestroyedColonistsHaveJoined;

L_9e56:
    idm = idmBattleTookPlaceDestroyedKillingColonistsBargain;

L_9e5b:
    j = ((iplrStarbase << 0x5) | (isb + 0x10));
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, j, LOWORD(lpopStarbase), (LOWORD((uint32_t)((lpopStarbase >> 0x10))) & 0xffff), 0x0, 0x0);
    goto L_9dd2;

L_9eb8:
    if ((cplr != 0x2))
        goto CommonCountingCode;
    else
        goto L_9ec1;

L_9ec1:
    if ((cShipsInvolved == 0x2))
        goto L_a251;
    else
        goto L_9eca;

L_9eca:
    if ((cShdefsInvolved != 0x2))
        goto CommonCountingCode;
    else
        goto L_9ed0;

L_9ed0:

L_9ed6:
    pwThem = 0x0;
    pwUs = 0x0;
    pw = rgPlrLosses;
    i = 0x0;
    goto L_9f3a;

L_9eef:
    j = 0x0;
    goto L_9f05;

L_9ef7:
    j = (j + 0x1);
    pw = (pw + 0x2);

L_9f05:
    if ((j >= 0x10))
        goto L_9f36;
    else
        goto L_9f0e;

L_9f0e:
    if ((*(pw) == 0x0))
        goto L_9ef7;
    else
        goto L_9f19;

L_9f19:
    if ((i != iplr))
        goto L_9f2d;
    else
        goto L_9f24;

L_9f24:
    pwUs = pw;
    goto L_9ef7;

L_9f2d:
    pwThem = pw;

L_9f36:
    i = (i + 0x1);

L_9f3a:
    if ((i < 0x10))
        goto L_9eef;
    else
        goto L_9f43;

L_9f43:
    if ((pwUs != 0x0))
        goto L_9f56;
    else
        goto L_9f4c;

L_9f4c:
    if ((fStarbaseDied != 0x0))
        goto L_9f6f;
    else
        goto L_9f56;

L_9f56:
    if ((pwUs == 0x0))
        goto L_9fb0;
    else
        goto L_9f5f;

L_9f5f:
    if (((*(pwUs) & 0x4000) != 0x0))
        goto L_9fb0;
    else
        goto L_9f6f;

L_9f6f:
    if ((pwThem != 0x0))
        goto L_9f82;
    else
        goto L_9f78;

L_9f78:
    if ((fStarbaseDamaged != 0x0))
        goto L_9f9b;
    else
        goto L_9f82;

L_9f82:
    if ((pwThem == 0x0))
        goto L_9fa3;
    else
        goto L_9f8b;

L_9f8b:
    if (((*(pwThem) & 0x8000) == 0x0))
        goto L_9fa3;
    else
        goto L_9f9b;

L_9f9b:
    idm = idmBattleTookPlaceDestroyedWhichDamagedFray2;
    goto L_9fa8;

L_9fa3:
    idm = idmBattleTookPlaceDestroyedWhichTookDamage2;

L_9fa8:
    fAlive = 0x0;
    goto L_a01d;

L_9fb0:
    if ((pwThem != 0x0))
        goto L_9fc3;
    else
        goto L_9fb9;

L_9fb9:
    if ((fStarbaseDied != 0x0))
        goto L_9fdc;
    else
        goto L_9fc3;

L_9fc3:
    if ((pwThem == 0x0))
        goto L_a018;
    else
        goto L_9fcc;

L_9fcc:
    if (((*(pwThem) & 0x4000) != 0x0))
        goto L_a018;
    else
        goto L_9fdc;

L_9fdc:
    if ((pwUs != 0x0))
        goto L_9fef;
    else
        goto L_9fe5;

L_9fe5:
    if ((fStarbaseDamaged != 0x0))
        goto L_a008;
    else
        goto L_9fef;

L_9fef:
    if ((pwUs == 0x0))
        goto L_a010;
    else
        goto L_9ff8;

L_9ff8:
    if (((*(pwUs) & 0x8000) == 0x0))
        goto L_a010;
    else
        goto L_a008;

L_a008:
    idm = idmBattleTookPlaceDestroyedHoweverTookDamage2;
    goto L_a01d;

L_a010:
    idm = idmBattleTookPlaceDestroyedTakingDamage2;

L_a018:
    idm = idmBattleTookPlaceNeitherNorCompletelyDestroyed;

L_a01d:
    if ((pwUs != 0x0))
        goto L_a02e;
    else
        goto L_a026;

L_a026:
    cUs = 0x1;
    goto L_a039;

L_a02e:
    cUs = (*(pwUs) & 0x1fff);

L_a039:
    if ((pwThem != 0x0))
        goto L_a04a;
    else
        goto L_a042;

L_a042:
    cThem = 0x1;
    goto L_a055;

L_a04a:
    cThem = (*(pwThem) & 0x1fff);

L_a055:
    lpfl = lpflBtl;

L_a061:
    if ((lpfl->fDead != 0x0))
        goto L_a114;
    else
        goto L_a078;

L_a078:
    if ((lpfl->iPlayer != iplr))
        goto L_a0cf;
    else
        goto L_a087;

L_a087:
    if ((pwUs == 0x0))
        goto L_a0cf;
    else
        goto L_a090;

L_a090:
    cUs = (cUs + lpfl->rgcsh[(((pwUs - rgPlrLosses) >> 0x1) - (lpfl->iPlayer << 0x4))]);
    goto L_a114;

L_a0cf:
    if ((pwThem == 0x0))
        goto L_a114;
    else
        goto L_a0d8;

L_a0d8:
    cThem = (cThem + lpfl->rgcsh[(((pwThem - rgPlrLosses) >> 0x1) - (lpfl->iPlayer << 0x4))]);

L_a114:
    lpfl = lpfl->lpflNext;
    if ((LOWORD(lpfl) != LOWORD(lpflBtl)))
        goto L_a13b;
    else
        goto L_a133;

L_a133:
    if ((HIWORD(lpfl) == HIWORD(lpflBtl)))
        goto L_a14d;
    else
        goto L_a13b;

L_a13b:
    if ((LOWORD(lpfl) != 0x0))
        goto L_a061;
    else
        goto L_a144;

L_a144:
    if ((HIWORD(lpfl) != 0x0))
        goto L_a061;
    else
        goto L_a14d;

L_a14d:
    if ((pwUs == 0x0))
        goto L_a178;
    else
        goto L_a156;

L_a156:
    i = ((pwUs - rgPlrLosses) >> 0x1);
    i = (((i & 0xf0) << 0x1) | (i & 0xf));
    goto L_a190;

L_a178:
    i = ((iplrStarbase << 0x5) | (isb + 0x10));

L_a190:
    if ((pwThem == 0x0))
        goto L_a1bb;
    else
        goto L_a199;

L_a199:
    j = ((pwThem - rgPlrLosses) >> 0x1);
    j = (((j & 0xf0) << 0x1) | (j & 0xf));
    goto L_a1d3;

L_a1bb:
    j = ((iplrStarbase << 0x5) | (isb + 0x10));

L_a1d3:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, i, cUs, j, cThem, 0x0);
    if ((fAlive == 0x0))
        goto L_9dd2;
    else
        goto L_a207;

L_a207:
    if ((LOWORD(lppl) != 0x0))
        goto L_a219;
    else
        goto L_a210;

L_a210:
    if ((HIWORD(lppl) == 0x0))
        goto L_a235;
    else
        goto L_a219;

L_a219:
    if ((lppl->iPlayer == 0xffff))
        goto L_a235;
    else
        goto L_a226;

L_a226:
    if ((iplr != lppl->iPlayer))
        goto L_9dd2;
    else
        goto L_a235;

L_a235:
    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);

L_a251:
    pwThem = 0x0;
    pwUs = 0x0;
    pw = rgPlrLosses;
    i = 0x0;
    goto L_a2b5;

L_a26a:
    j = 0x0;
    goto L_a280;

L_a272:
    j = (j + 0x1);
    pw = (pw + 0x2);

L_a280:
    if ((j >= 0x10))
        goto L_a2b1;
    else
        goto L_a289;

L_a289:
    if ((*(pw) == 0x0))
        goto L_a272;
    else
        goto L_a294;

L_a294:
    if ((i != iplr))
        goto L_a2a8;
    else
        goto L_a29f;

L_a29f:
    pwUs = pw;
    goto L_a272;

L_a2a8:
    pwThem = pw;

L_a2b1:
    i = (i + 0x1);

L_a2b5:
    if ((i < 0x10))
        goto L_a26a;
    else
        goto L_a2be;

L_a2be:
    if ((pwUs != 0x0))
        goto L_a2d1;
    else
        goto L_a2c7;

L_a2c7:
    if ((fStarbaseDied != 0x0))
        goto L_a2ea;
    else
        goto L_a2d1;

L_a2d1:
    if ((pwUs == 0x0))
        goto L_a32b;
    else
        goto L_a2da;

L_a2da:
    if (((*(pwUs) & 0x3fff) == 0x0))
        goto L_a32b;
    else
        goto L_a2ea;

L_a2ea:
    if ((pwThem != 0x0))
        goto L_a2fd;
    else
        goto L_a2f3;

L_a2f3:
    if ((fStarbaseDamaged != 0x0))
        goto L_a316;
    else
        goto L_a2fd;

L_a2fd:
    if ((pwThem == 0x0))
        goto L_a31e;
    else
        goto L_a306;

L_a306:
    if (((*(pwThem) & 0x8000) == 0x0))
        goto L_a31e;
    else
        goto L_a316;

L_a316:
    idm = idmBattleTookPlaceDestroyedWhichDamagedFray;
    goto L_a323;

L_a31e:
    idm = idmBattleTookPlaceDestroyedWhichTookDamage;

L_a323:
    fAlive = 0x0;
    goto L_a398;

L_a32b:
    if ((pwThem != 0x0))
        goto L_a33e;
    else
        goto L_a334;

L_a334:
    if ((fStarbaseDied != 0x0))
        goto L_a357;
    else
        goto L_a33e;

L_a33e:
    if ((pwThem == 0x0))
        goto L_a393;
    else
        goto L_a347;

L_a347:
    if (((*(pwThem) & 0x3fff) == 0x0))
        goto L_a393;
    else
        goto L_a357;

L_a357:
    if ((pwUs != 0x0))
        goto L_a36a;
    else
        goto L_a360;

L_a360:
    if ((fStarbaseDamaged != 0x0))
        goto L_a383;
    else
        goto L_a36a;

L_a36a:
    if ((pwUs == 0x0))
        goto L_a38b;
    else
        goto L_a373;

L_a373:
    if (((*(pwUs) & 0x8000) == 0x0))
        goto L_a38b;
    else
        goto L_a383;

L_a383:
    idm = idmBattleTookPlaceDestroyedHoweverTookDamage;
    goto L_a398;

L_a38b:
    idm = idmBattleTookPlaceDestroyedTakingDamage;

L_a393:
    idm = idmBattleTookPlaceNeitherNorDestroyedIncident;

L_a398:
    if ((pwUs == 0x0))
        goto L_a3c3;
    else
        goto L_a3a1;

L_a3a1:
    i = ((pwUs - rgPlrLosses) >> 0x1);
    i = (((i & 0xf0) << 0x1) | (i & 0xf));
    goto L_a3db;

L_a3c3:
    i = ((iplrStarbase << 0x5) | (isb + 0x10));

L_a3db:
    if ((pwThem == 0x0))
        goto L_a406;
    else
        goto L_a3e4;

L_a3e4:
    j = ((pwThem - rgPlrLosses) >> 0x1);
    j = (((j & 0xf0) << 0x1) | (j & 0xf));
    goto L_a41e;

L_a406:
    j = ((iplrStarbase << 0x5) | (isb + 0x10));

L_a41e:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, i, j, 0x0, 0x0, 0x0);
    if ((fAlive == 0x0))
        goto L_9dd2;
    else
        goto L_a454;

L_a454:
    if ((LOWORD(lppl) != 0x0))
        goto L_a466;
    else
        goto L_a45d;

L_a45d:
    if ((HIWORD(lppl) == 0x0))
        goto L_a482;
    else
        goto L_a466;

L_a466:
    if ((lppl->iPlayer == 0xffff))
        goto L_a482;
    else
        goto L_a473;

L_a473:
    if ((iplr != lppl->iPlayer))
        goto L_9dd2;
    else
        goto L_a482;

L_a482:
    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);

CommonCountingCode:
    cThem = 0x0;
    cUs = 0x0;
    pw = rgPlrLosses;
    i = 0x0;
    goto L_a521;

L_a4ba:
    j = 0x0;
    goto L_a4d0;

L_a4c2:
    j = (j + 0x1);
    pw = (pw + 0x2);

L_a4d0:
    if ((j >= 0x10))
        goto L_a51d;
    else
        goto L_a4d9;

L_a4d9:
    if ((*(pw) == 0x0))
        goto L_a4c2;
    else
        goto L_a4e4;

L_a4e4:
    if ((i != iplr))
        goto L_a503;
    else
        goto L_a4ef;

L_a4ef:
    pwUs = pw;
    cUs = (cUs + (*(pw) & 0x1fff));
    goto L_a4c2;

L_a503:
    pwThem = pw;
    cThem = (cThem + (*(pw) & 0x1fff));
    iThem = i;

L_a51d:
    i = (i + 0x1);

L_a521:
    if ((i < 0x10))
        goto L_a4ba;
    else
        goto L_a52a;

L_a52a:
    iThem = (iThem | 0x30);
    cUsDead = cUs;
    cThemDead = cThem;
    if ((fStarbaseDied == 0x0))
        goto L_a563;
    else
        goto L_a544;

L_a544:
    if ((iplrStarbase != iplr))
        goto L_a556;
    else
        goto L_a54f;

L_a54f:
    cUsDead = (cUsDead + 0x1);
    goto L_a563;

L_a556:
    if ((iplrStarbase == 0xffff))
        goto L_a563;
    else
        goto L_a55f;

L_a55f:
    cThemDead = (cThemDead + 0x1);

L_a563:
    lpfl = lpflBtl;

L_a56f:
    if ((lpfl->fDead != 0x0))
        goto L_a5f6;
    else
        goto L_a586;

L_a586:
    if ((lpfl->iPlayer != iplr))
        goto L_a5c7;
    else
        goto L_a595;

L_a595:
    i = 0x0;
    goto L_a5bb;

L_a59d:
    cUs = (cUs + lpfl->rgcsh[i]);
    i = (i + 0x1);

L_a5bb:
    if ((i >= 0x10))
        goto L_a5f6;
    else
        goto L_a5c1;

L_a5c1:

L_a5c7:
    i = 0x0;
    goto L_a5ed;

L_a5cf:
    cThem = (cThem + lpfl->rgcsh[i]);
    i = (i + 0x1);

L_a5ed:
    if ((i < 0x10))
        goto L_a5cf;
    else
        goto L_a5f6;

L_a5f6:
    lpfl = lpfl->lpflNext;
    if ((LOWORD(lpfl) != LOWORD(lpflBtl)))
        goto L_a61d;
    else
        goto L_a615;

L_a615:
    if ((HIWORD(lpfl) == HIWORD(lpflBtl)))
        goto L_a62f;
    else
        goto L_a61d;

L_a61d:
    if ((LOWORD(lpfl) != 0x0))
        goto L_a56f;
    else
        goto L_a626;

L_a626:
    if ((HIWORD(lpfl) != 0x0))
        goto L_a56f;
    else
        goto L_a62f;

L_a62f:
    if ((iplrStarbase != iplr))
        goto L_a641;
    else
        goto L_a63a;

L_a63a:
    cUs = (cUs + 0x1);
    goto L_a65a;

L_a641:
    if ((iplrStarbase == 0xffff))
        goto L_a65a;
    else
        goto L_a64a;

L_a64a:
    cThem = (cThem + 0x1);
    iThem = ((iplrStarbase | 0x10) | 0x20);

L_a65a:
    if ((cplr != 0x2))
        goto L_aa4a;
    else
        goto L_a663;

L_a663:
    if ((cThem != 0x1))
        goto L_a6b4;
    else
        goto L_a66c;

L_a66c:
    if (((iThem & 0xf) != iplrStarbase))
        goto L_a695;
    else
        goto L_a67a;

L_a67a:
    j = ((iplrStarbase << 0x5) | (isb + 0x10));
    goto L_a6b4;

L_a695:
    j = ((pwThem - rgPlrLosses) >> 0x1);
    j = (((j & 0xf0) << 0x1) | (j & 0xf));

L_a6b4:
    if ((cUs != 0x1))
        goto L_a702;
    else
        goto L_a6bd;

L_a6bd:
    if ((iplr != iplrStarbase))
        goto L_a6e3;
    else
        goto L_a6c8;

L_a6c8:
    i = ((iplrStarbase << 0x5) | (isb + 0x10));
    goto L_a702;

L_a6e3:
    i = ((pwUs - rgPlrLosses) >> 0x1);
    i = (((i & 0xf0) << 0x1) | (i & 0xf));

L_a702:
    if ((cThemDead != cThem))
        goto L_a82e;
    else
        goto L_a70d;

L_a70d:
    idm = idmBattleTookPlaceAgainstForcesDestroyedEnemy;
    if ((cThemDead != 0x1))
        goto L_a789;
    else
        goto L_a71b;

L_a71b:
    idm = (idm + 0x5);
    if ((cUsDead != 0x0))
        goto L_a757;
    else
        goto L_a728;

L_a728:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, iThem, cUs, j, 0x0, 0x0);
    goto L_a9f5;

L_a757:
    FSendPlrMsg(iplr, (idm + 0x2), (idBtl | 0x4000), x, y, iThem, cUs, j, cUsDead, 0x0);

L_a789:
    if ((cUsDead != 0x0))
        goto L_a7fb;
    else
        goto L_a792;

L_a792:
    if ((cUs != 0x1))
        goto L_a7cb;
    else
        goto L_a79b;

L_a79b:
    FSendPlrMsg(iplr, 0x113, (idBtl | 0x4000), x, y, iThem, i, cThemDead, 0x0, 0x0);
    goto L_a9f5;

L_a7cb:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, iThem, cUs, 0x0, 0x0, 0x0);

L_a7fb:
    FSendPlrMsg(iplr, (idm + 0x2), (idBtl | 0x4000), x, y, iThem, cUs, cUsDead, 0x0, 0x0);

L_a82e:
    if ((cUsDead != cUs))
        goto L_a95a;
    else
        goto L_a839;

L_a839:
    idm = idmBattleTookPlaceAgainstForcesDestroyedEnemys;
    if ((cUsDead != 0x1))
        goto L_a8b5;
    else
        goto L_a847;

L_a847:
    idm = (idm + 0x5);
    if ((cThemDead != 0x0))
        goto L_a883;
    else
        goto L_a854;

L_a854:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, iThem, i, cThem, 0x0, 0x0);
    goto L_a9f5;

L_a883:
    FSendPlrMsg(iplr, (idm + 0x2), (idBtl | 0x4000), x, y, iThem, i, cThem, cThemDead, 0x0);

L_a8b5:
    if ((cThemDead != 0x0))
        goto L_a927;
    else
        goto L_a8be;

L_a8be:
    if ((cThem != 0x1))
        goto L_a8f7;
    else
        goto L_a8c7;

L_a8c7:
    FSendPlrMsg(iplr, 0x114, (idBtl | 0x4000), x, y, iThem, cUsDead, j, 0x0, 0x0);
    goto L_a9f5;

L_a8f7:
    FSendPlrMsg(iplr, idm, (idBtl | 0x4000), x, y, iThem, cThem, 0x0, 0x0, 0x0);

L_a927:
    FSendPlrMsg(iplr, (idm + 0x2), (idBtl | 0x4000), x, y, iThem, cThem, cThemDead, 0x0, 0x0);

L_a95a:
    if ((cUs != 0x1))
        goto L_a992;
    else
        goto L_a963;

L_a963:
    FSendPlrMsg(iplr, 0x115, (idBtl | 0x4000), x, y, iThem, i, cThem, cThemDead, 0x0);
    goto L_a9f5;

L_a992:
    if ((cThem != 0x1))
        goto L_a9ca;
    else
        goto L_a99b;

L_a99b:
    FSendPlrMsg(iplr, 0x116, (idBtl | 0x4000), x, y, iThem, cUs, j, cUsDead, 0x0);
    goto L_a9f5;

L_a9ca:
    FSendPlrMsg(iplr, 0x9f, (idBtl | 0x4000), x, y, iThem, cUs, cThem, cUsDead, cThemDead);

L_a9f5:
    if ((cUsDead == cUs))
        goto L_9dd2;
    else
        goto L_aa00;

L_aa00:
    if ((LOWORD(lppl) != 0x0))
        goto L_aa12;
    else
        goto L_aa09;

L_aa09:
    if ((HIWORD(lppl) == 0x0))
        goto L_aa2e;
    else
        goto L_aa12;

L_aa12:
    if ((lppl->iPlayer == 0xffff))
        goto L_aa2e;
    else
        goto L_aa1f;

L_aa1f:
    if ((iplr != lppl->iPlayer))
        goto L_9dd2;
    else
        goto L_aa2e;

L_aa2e:
    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);

L_aa4a:
    if ((cUsDead != 0x0))
        goto L_aabd;
    else
        goto L_aa53;

L_aa53:
    if ((cThem != cThemDead))
        goto IndecisiveXWay;
    else
        goto L_aa5e;

L_aa5e:
    FSendPlrMsg(iplr, 0xa4, (idBtl | 0x4000), x, y, cplr, cUs, 0x0, 0x0, 0x0);
    goto L_ab98;

IndecisiveXWay:
    FSendPlrMsg(iplr, 0xa8, (idBtl | 0x4000), x, y, cplr, cUsDead, cUs, cThemDead, cThem);

L_aabd:
    if ((cThemDead != 0x0))
        goto L_ab07;
    else
        goto L_aac6;

L_aac6:
    if ((cUs != cUsDead))
        goto IndecisiveXWay;
    else
        goto L_aad1;

L_aad1:
    FSendPlrMsg(iplr, 0xa6, (idBtl | 0x4000), x, y, cplr, cUs, cThem, 0x0, 0x0);
    goto L_ab98;

L_ab07:
    if ((cThemDead != cThem))
        goto L_ab42;
    else
        goto L_ab12;

L_ab12:
    FSendPlrMsg(iplr, 0xa5, (idBtl | 0x4000), x, y, cplr, cUsDead, cUs, 0x0, 0x0);
    goto L_ab98;

L_ab42:
    if ((cUsDead != cUs))
        goto L_ab7c;
    else
        goto L_ab4d;

L_ab4d:
    FSendPlrMsg(iplr, 0xa7, (idBtl | 0x4000), x, y, cplr, cUs, cThem, cThemDead, 0x0);
    goto L_ab98;

L_ab7c:
    FSendPlrMsg2(iplr, 0x7e, (idBtl | 0x4000), x, y);

L_ab98:
    if ((fAlive == 0x0))
        goto L_ad23;
    else
        goto L_aba1;

L_aba1:
    if ((LOWORD(lppl) != 0x0))
        goto L_abb3;
    else
        goto L_abaa;

L_abaa:
    if ((HIWORD(lppl) == 0x0))
        goto L_abcf;
    else
        goto L_abb3;

L_abb3:
    if ((lppl->iPlayer == 0xffff))
        goto L_abcf;
    else
        goto L_abc0;

L_abc0:
    if ((iplr != lppl->iPlayer))
        goto L_ad23;
    else
        goto L_abcf;

L_abcf:
    ITechLearnATech(iplr, x, y, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);

L_abeb:
    if ((LOWORD(lppl) != 0x0))
        goto L_abfd;
    else
        goto L_abf4;

L_abf4:
    if ((HIWORD(lppl) == 0x0))
        goto L_ac47;
    else
        goto L_abfd;

L_abfd:
    if ((lppl->iPlayer != iplr))
        goto L_ac47;
    else
        goto L_ac0c;

L_ac0c:
    FSendPlrMsg2(iplr, 0xf9, lppl->id, lppl->id, 0x0);
    ITechLearnATech(iplr, x, y, idmFleetFoundWreckageBattleWhichHasBoosted, 0x0);
    goto L_ad23;

L_ac47:
    if (((iplr & grfSpectator) == 0x0))
        goto L_ad23;
    else
        goto L_ac55;

L_ac55:
    lpfl = lpflBtl;

L_ac61:
    if ((lpfl->iPlayer == iplr))
        goto L_acac;
    else
        goto L_ac6d;

L_ac6d:

L_ac73:
    lpfl = lpfl->lpflNext;
    if ((LOWORD(lpfl) != LOWORD(lpflBtl)))
        goto L_ac9a;
    else
        goto L_ac92;

L_ac92:
    if ((HIWORD(lpfl) == HIWORD(lpflBtl)))
        goto L_acac;
    else
        goto L_ac9a;

L_ac9a:
    if ((LOWORD(lpfl) != 0x0))
        goto L_ac61;
    else
        goto L_aca3;

L_aca3:
    if ((HIWORD(lpfl) != 0x0))
        goto L_ac61;
    else
        goto L_acac;

L_acac:
    if ((LOWORD(lpfl) != 0x0))
        goto L_acbe;
    else
        goto L_acb5;

L_acb5:
    if ((HIWORD(lpfl) == 0x0))
        goto L_ad23;
    else
        goto L_acbe;

L_acbe:
    if ((lpfl->iPlayer != iplr))
        goto L_ad23;
    else
        goto L_accd;

L_accd:
    FSendPlrMsg(iplr, 0xfa, (lpfl->id | 0x8000), lpfl->id, lpfl->pt.x, lpfl->pt.y, 0x0, 0x0, 0x0, 0x0);
    ITechLearnATech(iplr, x, y, idmWreckageBattleOccurredOrbitHasBoostedResearch, 0x0);

L_ad23:
    if ((((0x1 << iplr) & grfMissed) == 0x0))
        goto L_9dd2;
    else
        goto L_ad37;

L_ad37:
    lpfl = lpflBtl;

L_ad43:
    if ((lpfl->iPlayer != iplr))
        goto L_ad6c;
    else
        goto L_ad52;

L_ad52:
    if ((lpfl->fSkipped != 0x0))
        goto L_ada5;
    else
        goto L_ad66;

L_ad66:

L_ad6c:
    lpfl = lpfl->lpflNext;
    if ((LOWORD(lpfl) != LOWORD(lpflBtl)))
        goto L_ad93;
    else
        goto L_ad8b;

L_ad8b:
    if ((HIWORD(lpfl) == HIWORD(lpflBtl)))
        goto L_ada5;
    else
        goto L_ad93;

L_ad93:
    if ((LOWORD(lpfl) != 0x0))
        goto L_ad43;
    else
        goto L_ad9c;

L_ad9c:
    if ((HIWORD(lpfl) != 0x0))
        goto L_ad43;
    else
        goto L_ada5;

L_ada5:
    if ((LOWORD(lpfl) != 0x0))
        goto L_adb7;
    else
        goto L_adae;

L_adae:
    if ((HIWORD(lpfl) == 0x0))
        goto L_9dd2;
    else
        goto L_adb7;

L_adb7:
    if ((lpfl->iPlayer != iplr))
        goto L_9dd2;
    else
        goto L_adc6;

L_adc6:
    if ((lpfl->fSkipped == 0x0))
        goto L_9dd2;
    else
        goto L_addd;

L_addd:
    FSendPlrMsg2(iplr, 0x180, (lpfl->id | 0x8000), x, y);

L_adff:
    return;
}

int16_t FAttackPlayer(FLEET *lpfl, int16_t iplr) {
    int16_t iplrCur;
    int16_t iplrT;

L_ae06:
    iplrCur = lpfl->iPlayer;
    iplrT = ((HIWORD(rglpbtlplan[iplrCur][lpfl->iplan]) >> 0x8) & 0x1f);
    goto L_aed1;

L_ae50:
    return 0x0;

L_ae56:
    return 0x1;

L_ae5c:
    if (((uint16_t)(rgplr[iplrCur].rgmdRelation[iplr]) == 0x1))
        goto L_ae82;
    else
        goto L_ae7c;

L_ae7c:
    return 0x1;

L_ae82:
    return 0x0;

L_ae88:
    if (((uint16_t)(rgplr[iplrCur].rgmdRelation[iplr]) != 0x2))
        goto L_aeae;
    else
        goto L_aea8;

L_aea8:
    return 0x1;

L_aeae:
    return 0x0;

L_aeb4:
    if ((iplr != (iplrT + 0xfffc)))
        goto L_aec8;
    else
        goto L_aec2;

L_aec2:
    return 0x1;

L_aec8:
    return 0x0;

L_aed1:
    if ((iplrT == 0x0))
        goto L_ae50;
    else
        goto L_aed9;

L_aed9:
    if ((iplrT == 0x1))
        goto L_ae88;
    else
        goto L_aee1;

L_aee1:
    if ((iplrT == 0x2))
        goto L_ae5c;
    else
        goto L_aee9;

L_aee9:
    if ((iplrT != 0x3))
        goto L_aeb4;
    else
        goto L_aeee;

L_aeee:
}

void DoBombing() {
    MessageId idmDst;
    int32_t   modKill;
    int16_t   fMulti;
    int32_t   cKillPeople;
    int32_t   dmgBombBldg;
    int32_t   cKillPeopleS;
    int32_t   cKillMine;
    int32_t   dmgBombFloor;
    MessageId idmSrc;
    int32_t   cKillDefenses;
    int32_t   cKillFact;
    int32_t   pctTerra;
    PLANET   *lppl;
    int16_t   ifl;
    FLEET    *lpfl;
    int32_t   cPPE;
    int32_t   dmgBombPeople;
    float     pctSmart;
    float     pctSuccess;
    int32_t   dmgPeopleSmart;
    double    pctSuccessHalf;
    int16_t   pctTot;
    int16_t   dChg;
    int16_t   i;
    uint16_t  t_merge_b2c2_0001;
    uint16_t  t_merge_b3b0_0001;
    uint16_t  t_merge_b5b9_0001;
    uint16_t  t_merge_b97d_0001;
    uint16_t  t_merge_b9c4_0001;
    uint16_t  t_merge_ba31_0001;
    uint16_t  t_merge_ba46_0001;
    uint16_t  t_merge_ba7d_0001;
    uint16_t  t_merge_ba92_0001;
    uint16_t  t_merge_bda6_0001;
    uint16_t  t_merge_bdbb_0001;
    uint16_t  t_merge_bdd3_0001;
    uint16_t  t_merge_bde8_0001;

L_aefa:
    ifl = 0x0;
    goto L_af0f;

L_af0b:
    ifl = (ifl + 0x1);

L_af0f:
    if ((ifl >= cFleet))
        goto L_be8d;
    else
        goto L_af1a;

L_af1a:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_af4a;
    else
        goto L_af42;

L_af42:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_be8d;
    else
        goto L_af4a;

L_af4a:
    if ((lpfl->fDead != 0x0))
        goto L_af0b;
    else
        goto L_af61;

L_af61:
    if ((lpfl->idPlanet == 0xffff))
        goto L_af0b;
    else
        goto L_af6e;

L_af6e:
    if ((lpfl->fBombed != 0x0))
        goto L_af0b;
    else
        goto L_af82;

L_af82:

L_af88:
    lppl = &(lpPlanets[lpfl->idPlanet]);
    if ((lppl->iPlayer == lpfl->iPlayer))
        goto L_af0b;
    else
        goto L_afb5;

L_afb5:
    if ((lppl->iPlayer == 0xffff))
        goto L_af0b;
    else
        goto L_afc2;

L_afc2:
    if ((FAttackPlayer(lpfl, lppl->iPlayer) == 0x0))
        goto L_af0b;
    else
        goto L_afdf;

L_afdf:
    if ((lppl->fStarbase != 0x0))
        goto L_af0b;
    else
        goto L_aff3;

L_aff3:

L_aff9:
    if ((FCalcFleetBombDamage(lpfl, &(dmgBombPeople), &(dmgBombFloor), &(dmgPeopleSmart), &(dmgBombBldg), &(pctTerra), &(fMulti)) == 0x0))
        goto L_af0b;
    else
        goto L_b024;

L_b024:

L_b02a:
    CalcPctSurvive(lppl, &(pctSuccess), &(pctSmart));
    if ((pctSuccess >= 1))
        goto L_b14e;
    else
        goto L_b055;

L_b055:
    if ((HIWORD(dmgBombPeople) < 0x0))
        goto L_b08a;
    else
        goto L_b05e;

L_b05e:
    if ((HIWORD(dmgBombPeople) > 0x0))
        goto L_b06c;
    else
        goto L_b063;

L_b063:
    if ((LOWORD(dmgBombPeople) <= 0x0))
        goto L_b08a;
    else
        goto L_b06c;

L_b06c:
    dmgBombPeople = __ftol();

L_b08a:
    if ((HIWORD(dmgBombFloor) < 0x0))
        goto L_b0bf;
    else
        goto L_b093;

L_b093:
    if ((HIWORD(dmgBombFloor) > 0x0))
        goto L_b0a1;
    else
        goto L_b098;

L_b098:
    if ((LOWORD(dmgBombFloor) <= 0x0))
        goto L_b0bf;
    else
        goto L_b0a1;

L_b0a1:
    dmgBombFloor = __ftol();

L_b0bf:
    if ((HIWORD(dmgPeopleSmart) < 0x0))
        goto L_b0f4;
    else
        goto L_b0c8;

L_b0c8:
    if ((HIWORD(dmgPeopleSmart) > 0x0))
        goto L_b0d6;
    else
        goto L_b0cd;

L_b0cd:
    if ((LOWORD(dmgPeopleSmart) <= 0x0))
        goto L_b0f4;
    else
        goto L_b0d6;

L_b0d6:
    dmgPeopleSmart = __ftol();

L_b0f4:
    if ((HIWORD(dmgBombBldg) < 0x0))
        goto L_b14e;
    else
        goto L_b0fd;

L_b0fd:
    if ((HIWORD(dmgBombBldg) > 0x0))
        goto L_b10b;
    else
        goto L_b102;

L_b102:
    if ((LOWORD(dmgBombBldg) <= 0x0))
        goto L_b14e;
    else
        goto L_b10b;

L_b10b:
    pctSuccessHalf = (1 - ((1 - pctSuccess) / 2));
    dmgBombBldg = __ftol();

L_b14e:
    /* untranslated: LOWORD(cPPE) = (((loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) + (loword((uint32_t)(words(*(lppl+0x14),
     * *(lppl+0x16)) >> 0x14)) & 0xfff)) + lppl->cDefenses) */
    HIWORD(cPPE) = ((0x0 + 0x0) + 0x0);
    cKillDefenses = 0x0;
    cKillPeople = 0x0;
    cKillMine = 0x0;
    cKillFact = 0x0;
    if ((HIWORD(dmgBombBldg) < 0x0))
        goto L_b45c;
    else
        goto L_b1fa;

L_b1fa:
    if ((HIWORD(dmgBombBldg) > 0x0))
        goto L_b208;
    else
        goto L_b1ff;

L_b1ff:
    if ((LOWORD(dmgBombBldg) <= 0x0))
        goto L_b45c;
    else
        goto L_b208;

L_b208:
    if ((HIWORD(cPPE) < 0x0))
        goto L_b45c;
    else
        goto L_b211;

L_b211:
    if ((HIWORD(cPPE) > 0x0))
        goto L_b21f;
    else
        goto L_b216;

L_b216:
    if ((LOWORD(cPPE) <= 0x0))
        goto L_b45c;
    else
        goto L_b21f;

L_b21f:
    /* untranslated: cKillFact = (uint32_t)(words(0x0, (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff)) * dmgBombBldg) */
    modKill = (int32_t)((cKillFact % cPPE));
    cKillFact = (int32_t)((cKillFact / cPPE));
    if ((HIWORD(modKill) < 0x0))
        goto L_b2c8;
    else
        goto L_b284;

L_b284:
    if ((HIWORD(modKill) > 0x0))
        goto L_b292;
    else
        goto L_b289;

L_b289:
    if ((LOWORD(modKill) <= 0x0))
        goto L_b2c8;
    else
        goto L_b292;

L_b292:
    if ((SIGNHIWORD(Random(LOWORD(cPPE))) > HIWORD(modKill)))
        goto L_b2be;
    else
        goto L_b2aa;

L_b2aa:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(modKill) ? L_b2b7 : L_b2af */

L_b2af:
    /* untranslated: branch callresult(int16_t) >= LOWORD(modKill) ? L_b2be : L_b2b7 */

L_b2b7:
    t_merge_b2c2_0001 = 0x1;
    goto L_b2c2;

L_b2be:
    t_merge_b2c2_0001 = 0x0;

L_b2c2:
    LOWORD(cKillFact) = (LOWORD(cKillFact) + t_merge_b2c2_0001);
    HIWORD(cKillFact) = (HIWORD(cKillFact) + 0x0);

L_b2c8:
    if ((HIWORD(cKillFact) < 0x0))
        goto L_b315;
    else
        goto L_b2e9;

L_b2e9:
    if ((HIWORD(cKillFact) > 0x0))
        goto L_b2f6;
    else
        goto L_b2ee;

L_b2ee:
    /* untranslated: branch LOWORD(cKillFact) <= (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) ? L_b315 : L_b2f6 */

L_b2f6:
    /* untranslated: LOWORD(cKillFact) = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x14)) & 0xfff) */
    HIWORD(cKillFact) = 0x0;

L_b315:
    /* untranslated: cKillDefenses = (uint32_t)(words(0x0, lppl->cDefenses) * dmgBombBldg) */
    modKill = (int32_t)((cKillDefenses % cPPE));
    cKillDefenses = (int32_t)((cKillDefenses / cPPE));
    if ((HIWORD(modKill) < 0x0))
        goto L_b3b6;
    else
        goto L_b372;

L_b372:
    if ((HIWORD(modKill) > 0x0))
        goto L_b380;
    else
        goto L_b377;

L_b377:
    if ((LOWORD(modKill) <= 0x0))
        goto L_b3b6;
    else
        goto L_b380;

L_b380:
    if ((SIGNHIWORD(Random(LOWORD(cPPE))) > HIWORD(modKill)))
        goto L_b3ac;
    else
        goto L_b398;

L_b398:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(modKill) ? L_b3a5 : L_b39d */

L_b39d:
    /* untranslated: branch callresult(int16_t) >= LOWORD(modKill) ? L_b3ac : L_b3a5 */

L_b3a5:
    t_merge_b3b0_0001 = 0x1;
    goto L_b3b0;

L_b3ac:
    t_merge_b3b0_0001 = 0x0;

L_b3b0:
    LOWORD(cKillDefenses) = (LOWORD(cKillDefenses) + t_merge_b3b0_0001);
    HIWORD(cKillDefenses) = (HIWORD(cKillDefenses) + 0x0);

L_b3b6:
    if ((HIWORD(cKillDefenses) < 0x0))
        goto L_b3f3;
    else
        goto L_b3cf;

L_b3cf:
    if ((HIWORD(cKillDefenses) > 0x0))
        goto L_b3dc;
    else
        goto L_b3d4;

L_b3d4:
    if ((LOWORD(cKillDefenses) <= lppl->cDefenses))
        goto L_b3f3;
    else
        goto L_b3dc;

L_b3dc:
    LOWORD(cKillDefenses) = lppl->cDefenses;
    HIWORD(cKillDefenses) = 0x0;

L_b3f3:
    cKillMine = (dmgBombBldg - (cKillFact + cKillDefenses));
    if ((HIWORD(cKillMine) < 0x0))
        goto L_b45c;
    else
        goto L_b430;

L_b430:
    if ((HIWORD(cKillMine) > 0x0))
        goto L_b43d;
    else
        goto L_b435;

L_b435:
    /* untranslated: branch LOWORD(cKillMine) <= (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) ? L_b45c : L_b43d */

L_b43d:
    /* untranslated: LOWORD(cKillMine) = (loword((uint32_t)(words(*(lppl+0x14), *(lppl+0x16)) >> 0x8)) & 0xfff) */
    HIWORD(cKillMine) = 0x0;

L_b45c:
    if ((HIWORD(dmgBombPeople) > 0x0))
        goto L_b4a1;
    else
        goto L_b465;

L_b465:
    if ((HIWORD(dmgBombPeople) < 0x0))
        goto L_b473;
    else
        goto L_b46a;

L_b46a:
    if ((LOWORD(dmgBombPeople) > 0x0))
        goto L_b4a1;
    else
        goto L_b473;

L_b473:
    if ((HIWORD(dmgBombFloor) > 0x0))
        goto L_b4a1;
    else
        goto L_b47c;

L_b47c:
    if ((HIWORD(dmgBombFloor) < 0x0))
        goto L_b48a;
    else
        goto L_b481;

L_b481:
    if ((LOWORD(dmgBombFloor) > 0x0))
        goto L_b4a1;
    else
        goto L_b48a;

L_b48a:
    if ((HIWORD(dmgPeopleSmart) < 0x0))
        goto L_b65b;
    else
        goto L_b493;

L_b493:
    if ((HIWORD(dmgPeopleSmart) > 0x0))
        goto L_b4a1;
    else
        goto L_b498;

L_b498:
    if ((LOWORD(dmgPeopleSmart) <= 0x0))
        goto L_b65b;
    else
        goto L_b4a1;

L_b4a1:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_b65b;
    else
        goto L_b4ae;

L_b4ae:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_b4bd;
    else
        goto L_b4b3;

L_b4b3:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x0))
        goto L_b65b;
    else
        goto L_b4bd;

L_b4bd:
    cKillPeopleS = (int32_t)(((uint32_t)((lppl->rgwtMin[0x3] * dmgPeopleSmart)) / 0x3e8));
    if ((HIWORD(cKillPeopleS) < HIWORD(lppl->rgwtMin[0x3])))
        goto L_b51f;
    else
        goto L_b4fb;

L_b4fb:
    if ((HIWORD(cKillPeopleS) > HIWORD(lppl->rgwtMin[0x3])))
        goto L_b508;
    else
        goto L_b500;

L_b500:
    if ((LOWORD(cKillPeopleS) < LOWORD(lppl->rgwtMin[0x3])))
        goto L_b51f;
    else
        goto L_b508;

L_b508:
    cKillPeopleS = (lppl->rgwtMin[0x3] + 0xffffffff);

L_b51f:
    /* untranslated: cKillPeople = (uint32_t)(words((HIWORD(lppl->rgwtMin[0x3]) - HIWORD(cKillPeopleS)), (LOWORD(lppl->rgwtMin[0x3]) - LOWORD(cKillPeopleS))) *
     * dmgBombPeople) */
    modKill = (int32_t)((cKillPeople % 0x3e8));
    cKillPeople = (int32_t)((cKillPeople / 0x3e8));
    if ((HIWORD(modKill) < 0x0))
        goto L_b5bf;
    else
        goto L_b57e;

L_b57e:
    if ((HIWORD(modKill) > 0x0))
        goto L_b58c;
    else
        goto L_b583;

L_b583:
    if ((LOWORD(modKill) <= 0x0))
        goto L_b5bf;
    else
        goto L_b58c;

L_b58c:
    if ((SIGNHIWORD(Random(0x3e8)) > HIWORD(modKill)))
        goto L_b5b5;
    else
        goto L_b5a1;

L_b5a1:
    /* untranslated: branch signhiword(callresult(int16_t)) < HIWORD(modKill) ? L_b5ae : L_b5a6 */

L_b5a6:
    /* untranslated: branch callresult(int16_t) > LOWORD(modKill) ? L_b5b5 : L_b5ae */

L_b5ae:
    t_merge_b5b9_0001 = 0x1;
    goto L_b5b9;

L_b5b5:
    t_merge_b5b9_0001 = 0x0;

L_b5b9:
    LOWORD(cKillPeople) = (LOWORD(cKillPeople) + t_merge_b5b9_0001);
    HIWORD(cKillPeople) = (HIWORD(cKillPeople) + 0x0);

L_b5bf:
    cKillPeople = (cKillPeople + cKillPeopleS);
    if ((HIWORD(dmgBombPeople) < 0x0))
        goto L_b603;
    else
        goto L_b5d4;

L_b5d4:
    if ((HIWORD(dmgBombPeople) > 0x0))
        goto L_b5e2;
    else
        goto L_b5d9;

L_b5d9:
    if ((LOWORD(dmgBombPeople) <= 0x0))
        goto L_b603;
    else
        goto L_b5e2;

L_b5e2:
    if ((HIWORD(cKillPeople) > 0x0))
        goto L_b603;
    else
        goto L_b5eb;

L_b5eb:
    if ((HIWORD(cKillPeople) < 0x0))
        goto L_b5f9;
    else
        goto L_b5f0;

L_b5f0:
    if ((LOWORD(cKillPeople) > 0x0))
        goto L_b603;
    else
        goto L_b5f9;

L_b5f9:
    cKillPeople = 0x1;

L_b603:
    if ((HIWORD(cKillPeople) > HIWORD(dmgBombFloor)))
        goto L_b62a;
    else
        goto L_b611;

L_b611:
    if ((HIWORD(cKillPeople) < HIWORD(dmgBombFloor)))
        goto L_b61e;
    else
        goto L_b616;

L_b616:
    if ((LOWORD(cKillPeople) >= LOWORD(dmgBombFloor)))
        goto L_b62a;
    else
        goto L_b61e;

L_b61e:
    cKillPeople = dmgBombFloor;

L_b62a:
    if ((HIWORD(cKillPeople) < HIWORD(lppl->rgwtMin[0x3])))
        goto L_b65b;
    else
        goto L_b63d;

L_b63d:
    if ((HIWORD(cKillPeople) > HIWORD(lppl->rgwtMin[0x3])))
        goto L_b64a;
    else
        goto L_b642;

L_b642:
    if ((LOWORD(cKillPeople) <= LOWORD(lppl->rgwtMin[0x3])))
        goto L_b65b;
    else
        goto L_b64a;

L_b64a:
    cKillPeople = lppl->rgwtMin[0x3];

L_b65b:
    if ((HIWORD(cKillPeople) < 0x0))
        goto L_b683;
    else
        goto L_b664;

L_b664:
    if ((HIWORD(cKillPeople) > 0x0))
        goto L_b672;
    else
        goto L_b669;

L_b669:
    if ((LOWORD(cKillPeople) <= 0x0))
        goto L_b683;
    else
        goto L_b672;

L_b672:
    lppl->rgwtMin[0x3] = (lppl->rgwtMin[0x3] - cKillPeople);

L_b683:
    if ((HIWORD(cKillFact) < 0x0))
        goto L_b6f1;
    else
        goto L_b68c;

L_b68c:
    if ((HIWORD(cKillFact) > 0x0))
        goto L_b69a;
    else
        goto L_b691;

L_b691:
    if ((LOWORD(cKillFact) <= 0x0))
        goto L_b6f1;
    else
        goto L_b69a;

L_b69a:
    /* untranslated: ss:[bp-0x4e] = 0x0 */
    /* untranslated: ss:[bp-0x4c] = ((*(lppl+0x16) - hiword((int32_t)(cKillFact << 0x14))) & 0xfff0) */
    *(lppl + 0x14) = (*(lppl + 0x14) & 0xffff);
    lppl->cFactories = 0x0;
    /* untranslated: *(lppl+0x14) = (*(lppl+0x14) | ss:[bp-0x4e]) */
    /* untranslated: *(lppl+0x16) = (*(lppl+0x16) | ss:[bp-0x4c]) */

L_b6f1:
    if ((HIWORD(cKillMine) < 0x0))
        goto L_b761;
    else
        goto L_b6fa;

L_b6fa:
    if ((HIWORD(cKillMine) > 0x0))
        goto L_b708;
    else
        goto L_b6ff;

L_b6ff:
    if ((LOWORD(cKillMine) <= 0x0))
        goto L_b761;
    else
        goto L_b708;

L_b708:
    /* untranslated: ss:[bp-0x4e] = ((*(lppl+0x14) - loword((int32_t)(cKillMine << 0x8))) & 0xff00) */
    /* untranslated: ss:[bp-0x4c] = ((*(lppl+0x16) - hiword((int32_t)(cKillMine << 0x8))) & 0xf) */
    lppl->cMines = 0x0;
    /* untranslated: *(lppl+0x14) = (*(lppl+0x14) | ss:[bp-0x4e]) */
    /* untranslated: *(lppl+0x16) = (*(lppl+0x16) | ss:[bp-0x4c]) */

L_b761:
    if ((HIWORD(cKillDefenses) < 0x0))
        goto L_b7d1;
    else
        goto L_b76a;

L_b76a:
    if ((HIWORD(cKillDefenses) > 0x0))
        goto L_b778;
    else
        goto L_b76f;

L_b76f:
    if ((LOWORD(cKillDefenses) <= 0x0))
        goto L_b7d1;
    else
        goto L_b778;

L_b778:
    /* untranslated: ss:[bp-0x4e] = ((*(lppl+0x18) - loword((int32_t)(cKillDefenses << 0x0))) & 0xfff) */
    /* untranslated: ss:[bp-0x4c] = 0x0 */
    lppl->cDefenses = 0x0;
    *(lppl + 0x1a) = (*(lppl + 0x1a) & 0xffff);
    /* untranslated: *(lppl+0x18) = (*(lppl+0x18) | ss:[bp-0x4e]) */
    /* untranslated: *(lppl+0x1a) = (*(lppl+0x1a) | ss:[bp-0x4c]) */

L_b7d1:
    if ((HIWORD(pctTerra) < 0x0))
        goto L_b9d4;
    else
        goto L_b7da;

L_b7da:
    if ((HIWORD(pctTerra) > 0x0))
        goto L_b7e8;
    else
        goto L_b7df;

L_b7df:
    if ((LOWORD(pctTerra) <= 0x0))
        goto L_b9d4;
    else
        goto L_b7e8;

L_b7e8:
    pctTot = 0x0;
    /* untranslated: ss:[bp-0x58] = 0x2 */
    /* untranslated: ss:[bp-0x56] = 0x0 */
    pctTerra = (pctTerra - __ftol());
    if ((HIWORD(pctTerra) < 0x0))
        goto L_b84b;
    else
        goto L_b832;

L_b832:
    if ((HIWORD(pctTerra) > 0x0))
        goto L_b841;
    else
        goto L_b837;

L_b837:
    if ((LOWORD(pctTerra) <= 0x1f4))
        goto L_b84b;
    else
        goto L_b841;

L_b841:
    pctTerra = 0x1f4;

L_b84b:
    i = 0x0;
    goto L_b930;

L_b853:
    dChg = ((uint16_t)(lppl->rgEnvVar[i]) - (uint16_t)(lppl->rgEnvVarOrig[i]));
    if ((dChg <= 0x0))
        goto L_b8d8;
    else
        goto L_b895;

L_b895:
    if ((SIGNHIWORD(dChg) < HIWORD(pctTerra)))
        goto L_b8b7;
    else
        goto L_b8a1;

L_b8a1:
    if ((SIGNHIWORD(dChg) > HIWORD(pctTerra)))
        goto L_b8ae;
    else
        goto L_b8a6;

L_b8a6:
    if ((dChg < LOWORD(pctTerra)))
        goto L_b8b7;
    else
        goto L_b8ae;

L_b8ae:
    dChg = LOWORD(pctTerra);

L_b8b7:
    lppl->rgEnvVar[i] = (lppl->rgEnvVar[i] - LOBYTE(dChg));
    pctTot = (pctTot + dChg);
    goto L_b92c;

L_b8d8:
    if ((dChg >= 0x0))
        goto L_b92c;
    else
        goto L_b8e1;

L_b8e1:
    /* untranslated: branch signhiword((dChg neg 0x0)) < HIWORD(pctTerra) ? L_b90c : L_b8ef */

L_b8ef:
    /* untranslated: branch signhiword((dChg neg 0x0)) > HIWORD(pctTerra) ? L_b8fc : L_b8f4 */

L_b8f4:
    /* untranslated: branch (dChg neg 0x0) < LOWORD(pctTerra) ? L_b90c : L_b8fc */

L_b8fc:
    /* untranslated: dChg = (LOWORD(pctTerra) neg 0x0) */

L_b90c:
    lppl->rgEnvVar[i] = (lppl->rgEnvVar[i] - LOBYTE(dChg));
    /* untranslated: pctTot = (pctTot + (dChg neg 0x0)) */

L_b92c:
    i = (i + 0x1);

L_b930:
    if ((i < 0x3))
        goto L_b853;
    else
        goto L_b939;

L_b939:
    if ((pctTot <= 0x0))
        goto L_b9d4;
    else
        goto L_b942;

L_b942:
    if ((fMulti == 0x0))
        goto L_b97a;
    else
        goto L_b974;

L_b974:
    t_merge_b97d_0001 = 0x17a;
    goto L_b97d;

L_b97a:
    t_merge_b97d_0001 = 0x12e;

L_b97d:
    FSendPlrMsg(lpfl->iPlayer, t_merge_b97d_0001, (lpfl->id | 0x8000), lpfl->id, lppl->id, pctTot, 0x0, 0x0, 0x0, 0x0);
    if ((fMulti == 0x0))
        goto L_b9c1;
    else
        goto L_b9bb;

L_b9bb:
    t_merge_b9c4_0001 = 0x17b;
    goto L_b9c4;

L_b9c1:
    t_merge_b9c4_0001 = 0x12e;

L_b9c4:
    FSendPlrMsg(lppl->iPlayer, t_merge_b9c4_0001, lppl->id, lpfl->id, lppl->id, pctTot, 0x0, 0x0, 0x0, 0x0);

L_b9d4:
    cPPE = ((cKillMine + cKillFact) + cKillDefenses);
    if ((HIWORD(cPPE) < 0x0))
        goto L_bd61;
    else
        goto L_b9f5;

L_b9f5:
    if ((HIWORD(cPPE) > 0x0))
        goto L_ba03;
    else
        goto L_b9fa;

L_b9fa:
    if ((LOWORD(cPPE) <= 0x0))
        goto L_bd61;
    else
        goto L_ba03;

L_ba03:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_ba6b;
    else
        goto L_ba10;

L_ba10:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_ba1f;
    else
        goto L_ba15;

L_ba15:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x0))
        goto L_ba6b;
    else
        goto L_ba1f;

L_ba1f:
    if ((fMulti == 0x0))
        goto L_ba2e;
    else
        goto L_ba28;

L_ba28:
    t_merge_ba31_0001 = 0x169;
    goto L_ba31;

L_ba2e:
    t_merge_ba31_0001 = 0x63;

L_ba31:
    idmSrc = t_merge_ba31_0001;
    if ((fMulti == 0x0))
        goto L_ba43;
    else
        goto L_ba3d;

L_ba3d:
    t_merge_ba46_0001 = 0x173;
    goto L_ba46;

L_ba43:
    t_merge_ba46_0001 = 0x6d;

L_ba46:
    idmDst = t_merge_ba46_0001;
    if ((HIWORD(cPPE) < 0x0))
        goto L_ba98;
    else
        goto L_ba52;

L_ba52:
    if ((HIWORD(cPPE) > 0x0))
        goto L_ba60;
    else
        goto L_ba57;

L_ba57:
    if ((LOWORD(cPPE) <= 0x1))
        goto L_ba98;
    else
        goto L_ba60;

L_ba60:
    idmSrc = (idmSrc + 0x1);
    idmDst = (idmDst + 0x1);

L_ba6b:
    if ((fMulti == 0x0))
        goto L_ba7a;
    else
        goto L_ba74;

L_ba74:
    t_merge_ba7d_0001 = 0x17c;
    goto L_ba7d;

L_ba7a:
    t_merge_ba7d_0001 = 0x8f;

L_ba7d:
    idmSrc = t_merge_ba7d_0001;
    if ((fMulti == 0x0))
        goto L_ba8f;
    else
        goto L_ba89;

L_ba89:
    t_merge_ba92_0001 = 0x17d;
    goto L_ba92;

L_ba8f:
    t_merge_ba92_0001 = 0x90;

L_ba92:
    idmDst = t_merge_ba92_0001;
    goto GenericBombMsg;

L_ba98:
    if ((HIWORD(cKillPeople) < 0x0))
        goto L_bc0a;
    else
        goto L_baa1;

L_baa1:
    if ((HIWORD(cKillPeople) > 0x0))
        goto L_baaf;
    else
        goto L_baa6;

L_baa6:
    if ((LOWORD(cKillPeople) <= 0x0))
        goto L_bc0a;
    else
        goto L_baaf;

L_baaf:
    if ((pctSuccess != 1))
        goto L_bb47;
    else
        goto GenericBombMsg;

GenericBombMsg:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0x0, 0x0, 0x0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), 0x0, 0x0, 0x0);
    goto L_be65;

L_bb47:
    idmSrc = (idmSrc + 0x5);
    idmDst = (idmDst + 0x5);
    /* untranslated: ss:[bp-0x52] = 0x2710 */
    /* untranslated: ss:[bp-0x50] = 0x0 */
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), LOWORD(__ftol()), 0x0, 0x0);
    /* untranslated: ss:[bp-0x52] = 0x2710 */
    /* untranslated: ss:[bp-0x50] = 0x0 */
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), LOWORD(cPPE), LOWORD(__ftol()), 0x0, 0x0);

L_bc0a:
    idmSrc = (idmSrc - 0x2);
    idmDst = (idmDst - 0x2);
    if ((pctSuccess != 1))
        goto L_bca4;
    else
        goto L_bc27;

L_bc27:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cPPE), 0x0, 0x0, 0x0, 0x0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), 0x0, 0x0, 0x0, 0x0);
    goto L_be65;

L_bca4:
    idmSrc = (idmSrc + 0x5);
    idmDst = (idmDst + 0x5);
    /* untranslated: ss:[bp-0x52] = 0x2710 */
    /* untranslated: ss:[bp-0x50] = 0x0 */
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cPPE), LOWORD(__ftol()), 0x0, 0x0, 0x0);
    /* untranslated: ss:[bp-0x52] = 0x2710 */
    /* untranslated: ss:[bp-0x50] = 0x0 */
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cPPE), LOWORD(__ftol()), 0x0, 0x0, 0x0);

L_bd61:
    if ((HIWORD(cKillPeople) < 0x0))
        goto L_be65;
    else
        goto L_bd6a;

L_bd6a:
    if ((HIWORD(cKillPeople) > 0x0))
        goto L_bd78;
    else
        goto L_bd6f;

L_bd6f:
    if ((LOWORD(cKillPeople) <= 0x0))
        goto L_be65;
    else
        goto L_bd78;

L_bd78:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_bdc1;
    else
        goto L_bd85;

L_bd85:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_bd94;
    else
        goto L_bd8a;

L_bd8a:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x0))
        goto L_bdc1;
    else
        goto L_bd94;

L_bd94:
    if ((fMulti == 0x0))
        goto L_bda3;
    else
        goto L_bd9d;

L_bd9d:
    t_merge_bda6_0001 = 0x166;
    goto L_bda6;

L_bda3:
    t_merge_bda6_0001 = 0x60;

L_bda6:
    idmSrc = t_merge_bda6_0001;
    if ((fMulti == 0x0))
        goto L_bdb8;
    else
        goto L_bdb2;

L_bdb2:
    t_merge_bdbb_0001 = 0x170;
    goto L_bdbb;

L_bdb8:
    t_merge_bdbb_0001 = 0x6a;

L_bdbb:
    idmDst = t_merge_bdbb_0001;
    goto L_bdeb;

L_bdc1:
    if ((fMulti == 0x0))
        goto L_bdd0;
    else
        goto L_bdca;

L_bdca:
    t_merge_bdd3_0001 = 0x17c;
    goto L_bdd3;

L_bdd0:
    t_merge_bdd3_0001 = 0x8f;

L_bdd3:
    idmSrc = t_merge_bdd3_0001;
    if ((fMulti == 0x0))
        goto L_bde5;
    else
        goto L_bddf;

L_bddf:
    t_merge_bde8_0001 = 0x17d;
    goto L_bde8;

L_bde5:
    t_merge_bde8_0001 = 0x90;

L_bde8:
    idmDst = t_merge_bde8_0001;

L_bdeb:
    FSendPlrMsg(lpfl->iPlayer, idmSrc, (lpfl->id | 0x8000), lpfl->id, lppl->id, LOWORD(cKillPeople), 0x0, 0x0, 0x0, 0x0);
    FSendPlrMsg(lppl->iPlayer, idmDst, lppl->id, lpfl->id, lppl->id, LOWORD(cKillPeople), 0x0, 0x0, 0x0, 0x0);

L_be65:
    if ((LOWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_af0b;
    else
        goto L_be72;

L_be72:
    if ((HIWORD(lppl->rgwtMin[0x3]) != 0x0))
        goto L_af0b;
    else
        goto L_be7c;

L_be7c:
    UninhabitPlanet(lppl);

L_be8d:
    return;
}
