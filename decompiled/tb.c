#include "common.h"

char    vrgTBBtn[29] = "";
int16_t vrgpctZoom[9] = {25, 38, 50, 75, 100, 125, 150, 200, 400};

int32_t TbWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    int16_t     fInside;
    POINT       pt;
    StringId    ids;
    int16_t     itb;
    PAINTSTRUCT ps;
    int16_t     i;
    int16_t     fCur;
    int16_t     fDown;
    int16_t     iSel;
    int16_t     dx;
    POINT       ptBtn;
    int16_t     j;
    int16_t     x;
    RECT        rc;
    HWND        hwndCE;
    int16_t     pct;
    uint16_t    t_merge_02d6_0001;
    uint16_t    t_merge_03b9_0001;

L_001e:
    goto L_068b;

L_002d:
    x = 0x4;
    i = 0x0;
    goto L_0211;

L_003a:
    /* untranslated: itb = sext8to16(byte 0xe:[i]) */
    dx = DxOfBtn(itb);
    if ((itb > 0xfffd))
        goto L_0207;
    else
        goto L_005d;

L_005d:
    if ((itb != 0xfffd))
        goto L_0207;
    else
        goto L_0066;

L_0066:
    hwndTBRadar = CreateWindow("COMBOBOX", 0x0, 0x50200042, x, (((uint32_t)(((0x1c - dyArial8) + 0xfff8)) / 0x2) + 0x4), dx, (LOWORD((0xb * dyArial8)) + 0x1c),
                               hwnd, 0x0, hInst, 0x0);
    SendMessage(hwndTBRadar, WM_SETFONT, HIWORD(rghfontArial8), 0x0);
    iSel = 0xffff;
    j = 0x0;
    goto L_0137;

L_00e3:
    pct = (0x64 - LOWORD((0xa * j)));
    if ((pct != vpctRadarView))
        goto L_0102;
    else
        goto L_00fc;

L_00fc:
    iSel = j;

L_0102:
    _wsprintf(szWork, PCTDPCTPCT, pct);
    SendMessage(hwndTBRadar, CB_ADDSTRING, 0x0, 0x2557a4);
    j = (j + 0x1);

L_0137:
    if ((j < 0xa))
        goto L_00e3;
    else
        goto L_0140;

L_0140:
    SendMessage(hwndTBRadar, CB_SETEXTENDEDUI, 0x4, 0x0);
    SendMessage(hwndTBRadar, CB_SETCURSEL, iSel, 0x0);
    _wsprintf(szWork, PCTDPCTPCT, vpctRadarView);
    SetWindowText(hwndTBRadar, szWork);
    lpfnRealComboProc = GetWindowLong(hwndTBRadar, 0xfffc);
    SetWindowLong(hwndTBRadar, 0xfffc, lpfnFakeComboProc);
    hwndCE = GetWindow(hwndTBRadar, GW_CHILD);
    if ((hwndCE == 0x0))
        goto L_0207;
    else
        goto L_01df;

L_01df:
    lpfnRealCEProc = GetWindowLong(hwndCE, 0xfffc);
    SetWindowLong(hwndCE, 0xfffc, lpfnFakeCEProc);

L_0207:
    x = (x + dx);
    i = (i + 0x1);

L_0211:
    if ((i >= 0x1d))
        goto L_06de;
    else
        goto L_0217;

L_0217:

L_021d:
    ShowTooltip(0xffff, 0x0);
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    ptBtn.x = pt.x;
    ptBtn.y = pt.y;
    itb = ItbFromPpt(&(ptBtn));
    if ((itb < 0x0))
        goto L_06de;
    else
        goto L_026e;

L_026e:

L_0274:
    fDown = FIsButtonDown(itb);
    if ((fDown == 0x0))
        goto L_02a0;
    else
        goto L_028b;

L_028b:
    if ((itb > 0x5))
        goto L_02a0;
    else
        goto L_0294;

L_0294:
    MessageBeep(0x0);
    goto L_06de;

L_02a0:
    if ((itb == 0xd))
        goto L_02c4;
    else
        goto L_02a9;

L_02a9:
    if ((itb == 0xf))
        goto L_02c4;
    else
        goto L_02b2;

L_02b2:
    if ((itb == 0x10))
        goto L_02c4;
    else
        goto L_02bb;

L_02bb:
    if ((itb != 0x8))
        goto L_02e5;
    else
        goto L_02c4;

L_02c4:
    if ((fDown != 0x0))
        goto L_02d3;
    else
        goto L_02cd;

L_02cd:
    t_merge_02d6_0001 = 0x1;
    goto L_02d6;

L_02d3:
    t_merge_02d6_0001 = 0x0;

L_02d6:
    ExecuteButton(itb, t_merge_02d6_0001);
    goto L_06de;

L_02e5:
    dx = DxOfBtn(itb);
    rc.left = ptBtn.x;
    rc.top = ptBtn.y;
    rc.right = (rc.left + dx);
    rc.bottom = (rc.top + 0x1c);
    hdc = GetDC(hwnd);
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    SetCapture(hwnd);
    fCur = 0xffff;

L_0341:
    if ((FGetMouseMove(&(pt)) == 0x0))
        goto L_0399;
    else
        goto L_0355;

L_0355:
    /* untranslated: fInside = PtInRect(&rc, words(pt.y, pt.x)) */
    if ((fCur == fInside))
        goto L_0341;
    else
        goto L_0375;

L_0375:
    /* untranslated: call DrawBitmapButton(hdc, words(ptBtn.y, ptBtn.x), itb, (fDown + fInside)) -> callresult(void) */
    fCur = fInside;

L_0399:
    ReleaseCapture();
    if ((fInside == 0x0))
        goto L_03d3;
    else
        goto L_03a7;

L_03a7:
    if ((fDown != 0x0))
        goto L_03b6;
    else
        goto L_03b0;

L_03b0:
    t_merge_03b9_0001 = 0x1;
    goto L_03b9;

L_03b6:
    t_merge_03b9_0001 = 0x0;

L_03b9:
    ExecuteButton(itb, t_merge_03b9_0001);
    fDown = FIsButtonDown(itb);

L_03d3:
    if ((itb > 0x5))
        goto L_03fd;
    else
        goto L_03dc;

L_03dc:
    GetClientRect(hwnd, &(rc));
    DrawToolbar(hdc, &(rc));
    goto L_0414;

L_03fd:
    /* untranslated: call DrawBitmapButton(hdc, words(ptBtn.y, ptBtn.x), itb, fDown) -> callresult(void) */

L_0414:
    ReleaseDC(hwnd, hdc);
    goto L_06de;

L_0422:
    pt.x = LOWORD(lParam);
    pt.y = (LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff);
    if ((hwnd == hwndTb))
        goto L_0464;
    else
        goto L_044d;

L_044d:
    MapWindowPoints(hwnd, hwndTb, &(pt), 0x1);

L_0464:
    if ((pt.x != vptTbLast.x))
        goto L_047e;
    else
        goto L_0473;

L_0473:
    if ((pt.y == vptTbLast.y))
        goto L_06de;
    else
        goto L_0478;

L_0478:

L_047e:
    vptTbLast.x = pt.x;
    vptTbLast.y = pt.y;
    itb = ItbFromPpt(&(pt));
    if ((itb < 0x0))
        goto L_04f8;
    else
        goto L_04a3;

L_04a3:
    ids = (itb + 0x16a);

LShowTip:
    rc.left = pt.x;
    rc.right = (DxOfBtn(itb) + rc.left);
    rc.top = pt.y;
    rc.bottom = 0x1c;
    MapWindowPoints(hwndTb, 0x0, &(rc), 0x2);
    ShowTooltip(ids, &(rc));
    goto L_06de;

L_04f8:
    if ((itb != 0xfffd))
        goto L_06de;
    else
        goto L_0501;

L_0501:
    ids = idsScannerEffective;
    goto LShowTip;

L_050c:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrButtonFace);
    return 0x10000;

L_0537:
    SetBkColor(wParam, crButtonFace);
    /* untranslated: return words(hbrButtonFace, 0x0) */

L_0550:
    SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f00)));
    return 0x10000;

L_0570:
    if ((LOWORD(lParam) != hwndTBRadar))
        goto L_06de;
    else
        goto L_057f;

L_057f:
    if (((LOWORD((uint32_t)((lParam >> 0x10))) & 0xffff) != 0x8))
        goto L_06de;
    else
        goto L_059b;

L_059b:
    PostMessage(hwnd, 0x5f4, 0x0, 0x0);

L_05b6:
    TerminateToolbarFocus(0x0);
    goto L_06de;

L_05c5:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    PatBlt(hdc, 0x0, 0x0, rc.right, 0x1, BLACKNESS);
    PatBlt(hdc, 0x0, (rc.bottom + 0xffff), rc.right, 0x1, BLACKNESS);
    if ((iWindowLayout != 0x0))
        goto L_0650;
    else
        goto L_0631;

L_0631:
    PatBlt(hdc, 0x0, 0x0, 0x1, rc.bottom, BLACKNESS);

L_0650:
    DrawToolbar(hdc, &(rc));
    EndPaint(hwnd, &(ps));
    goto L_06de;

L_0671:
    return DefWindowProc(hwnd, msg, wParam, lParam);

L_068b:
    if ((msg == WM_CREATE))
        goto L_002d;
    else
        goto L_0693;

L_0693:
    if ((msg == WM_PAINT))
        goto L_05c5;
    else
        goto L_069b;

L_069b:
    if ((msg == WM_ERASEBKGND))
        goto L_050c;
    else
        goto L_06a3;

L_06a3:
    if ((msg == WM_CTLCOLOR))
        goto L_0537;
    else
        goto L_06ab;

L_06ab:
    if ((msg == WM_SETCURSOR))
        goto L_0550;
    else
        goto L_06b3;

L_06b3:
    if ((msg == WM_COMMAND))
        goto L_0570;
    else
        goto L_06bb;

L_06bb:
    if ((msg == WM_MOUSEMOVE))
        goto L_0422;
    else
        goto L_06c3;

L_06c3:
    if ((msg == WM_LBUTTONDOWN))
        goto L_021d;
    else
        goto L_06cb;

L_06cb:
    if ((msg == WM_LBUTTONDBLCLK))
        goto L_021d;
    else
        goto L_06d3;

L_06d3:
    if ((msg != 0x5f4))
        goto L_0671;
    else
        goto L_06d8;

L_06d8:

L_06de:
    return 0x0;
}

void DrawToolbar(HDC hdc, RECT *prc) {
    POINT   pt;
    int16_t i;
    int16_t ibtn;

L_06f0:
    SelectPalette(hdc, vhpal, 0x0);
    RealizePalette(hdc);
    pt.x = 0x4;
    pt.y = 0x4;
    i = 0x0;
    goto L_077d;

L_0723:
    /* untranslated: ibtn = sext8to16(byte 0xe:[i]) */
    if ((ibtn < 0x0))
        goto L_0762;
    else
        goto L_0738;

L_0738:
    /* untranslated: call DrawBitmapButton(hdc, words(pt.y, pt.x), sext8to16(byte 0xe:[i]), FIsButtonDown(ibtn)) -> callresult(void) */
    goto L_076b;

L_0762:
    if ((ibtn <= 0xfffd))
        goto L_076b;
    else
        goto L_076b;

L_076b:
    pt.x = (pt.x + DxOfBtn(ibtn));
    i = (i + 0x1);

L_077d:
    if ((i < 0x1d))
        goto L_0723;
    else
        goto L_0786;

L_0786:
    return;
}

void DrawBitmapButton(HDC hdc, POINT pt, int16_t ibtn, int16_t fDown) {
    int16_t  dx;
    HBRUSH   hbrBotRight;
    HBRUSH   hbrTopLeft;
    int16_t  dxDraw;
    uint16_t t_merge_07b5_0001;

L_078c:
    dx = DxOfBtn(ibtn);
    if ((dx >= 0x18))
        goto L_07b2;
    else
        goto L_07ac;

L_07ac:
    t_merge_07b5_0001 = 0x7;
    goto L_07b5;

L_07b2:
    t_merge_07b5_0001 = 0x18;

L_07b5:
    dxDraw = t_merge_07b5_0001;
    if ((fDown == 0x0))
        goto L_07d0;
    else
        goto L_07c1;

L_07c1:
    hbrTopLeft = hbrButtonShadow;
    hbrBotRight = hbrButtonHilite;
    goto L_07dc;

L_07d0:
    hbrTopLeft = hbrButtonHilite;
    hbrBotRight = hbrButtonShadow;

L_07dc:
    SelectObject(hdc, hbrTopLeft);
    PatBlt(hdc, (pt.x + 0x2), pt.y, (dx + 0xfffc), 0x1, PATCOPY);
    PatBlt(hdc, pt.x, (pt.y + 0x2), 0x1, 0x18, PATCOPY);
    PatBlt(hdc, (pt.x + 0x1), (pt.y + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, (pt.x + 0x1), (pt.y + 0x1a), 0x1, 0x1, PATCOPY);
    SelectObject(hdc, hbrBotRight);
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x1b), (dx + 0xfffc), 0x1, PATCOPY);
    PatBlt(hdc, ((pt.x + dx) + 0xffff), (pt.y + 0x2), 0x1, 0x18, PATCOPY);
    PatBlt(hdc, ((pt.x + dx) + 0xfffe), (pt.y + 0x1), 0x1, 0x1, PATCOPY);
    PatBlt(hdc, ((pt.x + dx) + 0xfffe), (pt.y + 0x1a), 0x1, 0x1, PATCOPY);
    SelectObject(hdc, hbrButtonFace);
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x1), (dx + 0xfffc), 0x1, PATCOPY);
    PatBlt(hdc, (pt.x + 0x1), (pt.y + 0x2), 0x1, 0x18, PATCOPY);
    if ((fDown == 0x0))
        goto L_0a38;
    else
        goto L_098d;

L_098d:
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x2), (dx + 0xfffc), fDown, PATCOPY);
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x2), fDown, 0x18, PATCOPY);
    if ((fDown != 0x1))
        goto L_0a8a;
    else
        goto L_09e3;

L_09e3:
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x1a), (dx + 0xfffc), 0x1, PATCOPY);
    PatBlt(hdc, ((pt.x + dx) + 0xfffe), (pt.y + 0x2), 0x1, 0x18, PATCOPY);

L_0a38:
    PatBlt(hdc, (pt.x + 0x2), (pt.y + 0x19), (dx + 0xfffc), 0x2, PATCOPY);
    PatBlt(hdc, ((pt.x + dx) + 0xfffd), (pt.y + 0x2), 0x2, 0x18, PATCOPY);

L_0a8a:
    DibBlt(hdc, ((pt.x + 0x2) + fDown), ((pt.y + 0x2) + fDown), dxDraw, 0x17, hdibToolbar, LOWORD((0x18 * ibtn)), 0x0, dxDraw, 0x17, 0xcc0020);
    if ((fDown <= 0x1))
        goto L_0b0c;
    else
        goto L_0ad7;

L_0ad7:
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, ((pt.x + dx) + 0xfffe), (pt.y + 0x1a), 0x1, 0x1, PATCOPY);

L_0b0c:
    return;
}

int16_t ItbFromPpt(POINT *ppt) {
    int16_t i;
    int16_t dx;
    int16_t x;

L_0b12:
    x = 0x4;
    if ((ppt->x < 0x4))
        goto L_0b43;
    else
        goto L_0b2b;

L_0b2b:
    if ((ppt->y < 0x4))
        goto L_0b43;
    else
        goto L_0b37;

L_0b37:
    if ((ppt->y < 0x20))
        goto L_0b49;
    else
        goto L_0b43;

L_0b43:
    return 0xffff;

L_0b49:
    i = 0x0;
    goto L_0b9c;

L_0b51:
    /* untranslated: dx = DxOfBtn(sext8to16(byte 0xe:[i])) */
    if (((x + dx) <= ppt->x))
        goto L_0b92;
    else
        goto L_0b76;

L_0b76:
    ppt->x = x;
    ppt->y = 0x4;
    /* untranslated: return sext8to16(byte 0xe:[i]) */

L_0b92:
    x = (x + dx);
    i = (i + 0x1);

L_0b9c:
    if ((i < 0x1d))
        goto L_0b51;
    else
        goto L_0ba5;

L_0ba5:
    return 0xffff;
}

int16_t DxOfBtn(int16_t itb) {
L_0bb2:
    if ((itb < 0x0))
        goto L_0be5;
    else
        goto L_0bc4;

L_0bc4:
    if ((itb == 0xd))
        goto L_0bdf;
    else
        goto L_0bcd;

L_0bcd:
    if ((itb == 0xf))
        goto L_0bdf;
    else
        goto L_0bd6;

L_0bd6:
    return 0x1d;

L_0bdf:
    return 0xb;

L_0be5:
    if ((itb != 0xffff))
        goto L_0bf4;
    else
        goto L_0bee;

L_0bee:
    return 0x6;

L_0bf4:
    if ((itb != 0xfffe))
        goto L_0c03;
    else
        goto L_0bfd;

L_0bfd:
    return 0x2;

L_0c03:
    goto L_0c22;

L_0c09:
    if ((dyArial8 >= 0x10))
        goto L_0c19;
    else
        goto L_0c13;

L_0c13:
    return 0x3c;

L_0c19:
    return 0x46;

L_0c22:
    if ((itb == 0xfffd))
        goto L_0c09;
    else
        goto L_0c2d;

L_0c2d:
    return 0x0;
}

int16_t FIsButtonDown(int16_t itb) {
L_0c3a:
    goto L_0d62;
    if (((grbitScan & 0xf) != itb))
        goto L_0c5f;
    else
        goto L_0c59;

L_0c59:
    goto L_0c62;

L_0c5f:

L_0c62:
    goto L_0d9d;
    if (((grbitScan & 0x10) == 0x0))
        goto L_0c79;
    else
        goto L_0c73;

L_0c73:
    goto L_0c7c;

L_0c79:

L_0c7c:
    goto L_0d9d;
    if (((grbitScan & 0x20) == 0x0))
        goto L_0c93;
    else
        goto L_0c8d;

L_0c8d:
    goto L_0c96;

L_0c93:

L_0c96:
    goto L_0d9d;
    if (((grbitScan & 0x40) == 0x0))
        goto L_0cb7;
    else
        goto L_0ca7;

L_0ca7:
    if ((grbitScanMines != 0xf))
        goto L_0cb7;
    else
        goto L_0cb1;

L_0cb1:
    goto L_0cba;

L_0cb7:

L_0cba:
    goto L_0d9d;
    if (((grbitScan & 0x80) == 0x0))
        goto L_0cd1;
    else
        goto L_0ccb;

L_0ccb:
    goto L_0cd4;

L_0cd1:

L_0cd4:
    goto L_0d9d;
    if (((grbitScan & 0x100) == 0x0))
        goto L_0ceb;
    else
        goto L_0ce5;

L_0ce5:
    goto L_0cee;

L_0ceb:

L_0cee:
    goto L_0d9d;
    if (((grbitScan & 0x400) == 0x0))
        goto L_0d05;
    else
        goto L_0cff;

L_0cff:
    goto L_0d08;

L_0d05:

L_0d08:
    goto L_0d9d;
    if (((grbitScan & 0x1000) == 0x0))
        goto L_0d1f;
    else
        goto L_0d19;

L_0d19:
    goto L_0d22;

L_0d1f:

L_0d22:
    goto L_0d9d;
    if (((grbitScan & 0x200) == 0x0))
        goto L_0d39;
    else
        goto L_0d33;

L_0d33:
    goto L_0d3c;

L_0d39:

L_0d3c:
    goto L_0d9d;
    if (((grbitScan & 0x800) == 0x0))
        goto L_0d53;
    else
        goto L_0d4d;

L_0d4d:
    goto L_0d56;

L_0d53:

L_0d56:
    goto L_0d9d;
    goto L_0d9d;

L_0d62:

L_0d9d:
    return 0x0;
}

void ExecuteButton(int16_t itb, int16_t fDown) {
    uint16_t grbitNew;
    POINT    pt;
    char    *rgszScan[12];
    int16_t  c;
    int16_t  i;
    uint16_t grbit;
    int32_t  rgid[12];
    int16_t  iSel;
    uint16_t grbitSh;
    int16_t  ish;
    uint16_t t_merge_0e93_0001;
    uint16_t t_merge_0ebc_0001;
    uint16_t t_merge_0f6f_0001;
    uint16_t t_merge_1176_0001;
    uint16_t t_merge_13c3_0001;
    uint16_t t_merge_153f_0001;

L_0db6:
    gd.fChgScanner = 0x1;
    goto L_160f;

L_0dd1:
    goto L_167a;
    if ((fDown == 0x0))
        goto L_167a;
    else
        goto L_0dda;

L_0dda:

L_0de0:
    grbitScan = (itb + (grbitScan & 0x3ff0));
    goto L_1644;
    grbitNew = 0x10;

LBitDiddle:
    if ((fDown == 0x0))
        goto L_0e0a;
    else
        goto L_0e00;

L_0e00:
    grbitScan = (grbitScan | grbitNew);
    goto L_0e13;

L_0e0a:
    /* untranslated: grbitScan = (grbitScan & (grbitNew ~ 0x0)) */

L_0e13:
    goto L_1644;
    grbitNew = 0x20;
    goto LBitDiddle;
    grbitNew = 0x80;
    goto LBitDiddle;
    grbitNew = 0x100;
    goto LBitDiddle;
    grbitNew = 0x400;
    goto LBitDiddle;
    grbitNew = 0x1000;
    goto LBitDiddle;
    grbitNew = 0x200;
    goto LBitDiddle;
    grbitNew = 0x800;
    goto LBitDiddle;
    grbit = 0x1;
    c = 0x0;
    if (((grbitScan & 0x40) != 0x0))
        goto L_0e6c;
    else
        goto L_0e66;

L_0e66:
    grbitScanMines = 0x0;

L_0e6c:
    i = 0x4fe;
    goto L_0f0d;

L_0e74:
    if ((i != 0x4fe))
        goto L_0ea7;
    else
        goto L_0e7e;

L_0e7e:
    if ((grbitScanMines != 0xf))
        goto L_0e8f;
    else
        goto L_0e88;

L_0e88:
    t_merge_0e93_0001 = 0x1;
    goto L_0e93;

L_0e8f:
    t_merge_0e93_0001 = 0x0;

L_0e93:
    LOWORD(rgid[c]) = t_merge_0e93_0001;
    HIWORD(rgid[c]) = 0x0;
    goto L_0ecd;

L_0ea7:
    if ((grbitScanMines != 0x0))
        goto L_0eb8;
    else
        goto L_0eb1;

L_0eb1:
    t_merge_0ebc_0001 = 0x1;
    goto L_0ebc;

L_0eb8:
    t_merge_0ebc_0001 = 0x0;

L_0ebc:
    LOWORD(rgid[c]) = t_merge_0ebc_0001;
    HIWORD(rgid[c]) = 0x0;

L_0ecd:
    CchGetString(i, (0x5844 + LOWORD(((i + 0xfb02) * 0x1e))));
    c = (c + 0x1);
    rgszScan[c] = (0x5844 + LOWORD(((i + 0xfb02) * 0x1e)));
    i = (i + 0x1);

L_0f0d:
    if ((i <= 0x4ff))
        goto L_0e74;
    else
        goto L_0f17;

L_0f17:
    rgid[c] = 0x0;
    /* untranslated: part[250:1](szWork) = 0xff */
    /* untranslated: part[251:1](szWork) = 0x0 */
    c = (c + 0x1);
    rgszScan[c] = 0x589e;
    i = 0x0;
    goto L_0fba;

L_0f50:
    if ((((0x1 << i) & grbitScanMines) == 0x0))
        goto L_0f6b;
    else
        goto L_0f64;

L_0f64:
    t_merge_0f6f_0001 = 0x1;
    goto L_0f6f;

L_0f6b:
    t_merge_0f6f_0001 = 0x0;

L_0f6f:
    LOWORD(rgid[c]) = t_merge_0f6f_0001;
    HIWORD(rgid[c]) = 0x0;
    CchGetString((i + 0x500), (0x57a4 + LOWORD((0x1e * i))));
    c = (c + 0x1);
    rgszScan[c] = (0x57a4 + LOWORD((0x1e * i)));
    i = (i + 0x1);

L_0fba:
    if ((i < 0x4))
        goto L_0f50;
    else
        goto L_0fc3;

L_0fc3:
    GetCursorPos(&(pt));
    ScreenToClient(hwndTb, &(pt));
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, 0xfffe, 0x0);
    if ((iSel == 0xffff))
        goto L_167a;
    else
        goto L_100d;

L_100d:

L_1013:
    if ((iSel >= 0x3))
        goto L_1037;
    else
        goto L_101c;

L_101c:
    if ((iSel != 0x0))
        goto L_102e;
    else
        goto L_1025;

L_1025:
    grbitScanMines = 0xf;
    goto L_1047;

L_102e:
    grbitScanMines = 0x0;

L_1037:
    iSel = (iSel - 0x3);
    grbitScanMines = (grbitScanMines ^ (0x1 << iSel));

L_1047:
    if ((grbitScanMines == 0x0))
        goto L_1059;
    else
        goto L_1051;

L_1051:
    grbitScan = (grbitScan | 0x40);
    goto L_105e;

L_1059:
    grbitScan = (grbitScan & 0xffbf);

L_105e:
    InvalidateRect(hwndTb, 0x0, 0x1);
    goto L_1644;
    c = 0x0;
    i = 0x4fb;
    goto L_10d9;

L_1083:
    rgid[c] = 0x0;
    CchGetString(i, (0x57a4 + LOWORD(((i + 0xfb05) * 0x14))));
    c = (c + 0x1);
    rgszScan[c] = (0x57a4 + LOWORD(((i + 0xfb05) * 0x14)));
    i = (i + 0x1);

L_10d9:
    if ((i <= 0x4fd))
        goto L_1083;
    else
        goto L_10e3;

L_10e3:
    rgid[c] = 0x0;
    /* untranslated: part[200:1](szWork) = 0xff */
    /* untranslated: part[201:1](szWork) = 0x0 */
    c = (c + 0x1);
    rgszScan[c] = 0x586c;
    ish = 0x0;
    grbitSh = 0x1;
    goto L_1135;

L_1125:
    ish = (ish + 0x1);
    grbitSh = (grbitSh * 0x2);

L_1135:
    if ((ish >= 0x10))
        goto L_11ab;
    else
        goto L_113e;

L_113e:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) != 0x0 ? L_1125 : L_115c */

L_115c:
    if (((grbitSh & grbitScanShip) == 0x0))
        goto L_1172;
    else
        goto L_116b;

L_116b:
    t_merge_1176_0001 = 0x1;
    goto L_1176;

L_1172:
    t_merge_1176_0001 = 0x0;

L_1176:
    LOWORD(rgid[c]) = t_merge_1176_0001;
    HIWORD(rgid[c]) = 0x0;
    c = (c + 0x1);
    /* untranslated: rgszScan[c] = part[8:0](rgshdef[ish].hul) */

L_11ab:
    GetCursorPos(&(pt));
    ScreenToClient(hwndTb, &(pt));
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, 0xfffe, 0x0);
    if ((iSel == 0xffff))
        goto L_167a;
    else
        goto L_11f8;

L_11f8:

L_11fe:
    if ((iSel >= 0x4))
        goto L_1251;
    else
        goto L_1208;

L_1208:
    if ((iSel != 0x0))
        goto L_121b;
    else
        goto L_1212;

L_1212:
    grbitScanShip = 0xffff;
    goto L_1233;

L_121b:
    if ((iSel != 0x1))
        goto L_122d;
    else
        goto L_1225;

L_1225:
    grbitScanShip = (grbitScanShip ^ 0xffff);
    goto L_1233;

L_122d:
    grbitScanShip = 0x0;

L_1233:
    if (((grbitScan & 0x200) != 0x0))
        goto L_12e6;
    else
        goto L_1241;

L_1241:
    if ((grbitScanShip != 0x0))
        goto LInvalS;
    else
        goto L_1248;

L_1248:

L_1251:
    iSel = (iSel - 0x4);
    ish = 0x0;
    goto L_1294;

L_125e:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) != 0x0 ? L_1290 : L_127c */

L_127c:
    iSel = (iSel - 0x1);
    if ((iSel < 0x0))
        goto L_129d;
    else
        goto L_128a;

L_128a:

L_1290:
    ish = (ish + 0x1);

L_1294:
    if ((ish < 0x10))
        goto L_125e;
    else
        goto L_129d;

L_129d:
    grbitScanShip = (grbitScanShip ^ (0x1 << ish));
    if (((grbitScan & 0x200) != 0x0))
        goto L_12e6;
    else
        goto L_12b7;

L_12b7:
    if ((((0x1 << ish) & grbitScanShip) == 0x0))
        goto L_12e6;
    else
        goto LInvalS;

LInvalS:
    grbitScan = (grbitScan | 0x200);
    InvalidateRect(hwndTb, 0x0, 0x1);

L_12e6:
    if (((grbitScan & 0x200) == 0x0))
        goto L_167a;
    else
        goto L_12f1;

L_12f1:

L_12f7:
    goto L_1644;
    grbit = 0x1;
    c = 0x0;
    i = 0x4fb;
    goto L_1361;

L_130c:
    rgid[c] = 0x0;
    CchGetString(i, (0x586c + LOWORD(((i + 0xfb05) * 0x19))));
    c = (c + 0x1);
    rgszScan[c] = (0x586c + LOWORD(((i + 0xfb05) * 0x19)));
    i = (i + 0x1);

L_1361:
    if ((i <= 0x4fd))
        goto L_130c;
    else
        goto L_136b;

L_136b:
    rgid[c] = 0x0;
    /* untranslated: part[300:1](szWork) = 0xff */
    /* untranslated: part[301:1](szWork) = 0x0 */
    c = (c + 0x1);
    rgszScan[c] = 0x58d0;
    i = 0x0;
    goto L_140e;

L_13a4:
    if ((((0x1 << i) & grbitScanEShip) == 0x0))
        goto L_13bf;
    else
        goto L_13b8;

L_13b8:
    t_merge_13c3_0001 = 0x1;
    goto L_13c3;

L_13bf:
    t_merge_13c3_0001 = 0x0;

L_13c3:
    LOWORD(rgid[c]) = t_merge_13c3_0001;
    HIWORD(rgid[c]) = 0x0;
    CchGetString((i + 0x17d), (0x57a4 + LOWORD((0x19 * i))));
    c = (c + 0x1);
    rgszScan[c] = (0x57a4 + LOWORD((0x19 * i)));
    i = (i + 0x1);

L_140e:
    if ((i < 0x8))
        goto L_13a4;
    else
        goto L_1417;

L_1417:
    GetCursorPos(&(pt));
    ScreenToClient(hwndTb, &(pt));
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, 0xfffe, 0x0);
    if ((iSel == 0xffff))
        goto L_167a;
    else
        goto L_1461;

L_1461:

L_1467:
    if ((iSel >= 0x4))
        goto L_14b8;
    else
        goto L_1470;

L_1470:
    if ((iSel != 0x0))
        goto L_1482;
    else
        goto L_1479;

L_1479:
    grbitScanEShip = 0xff;
    goto L_149a;

L_1482:
    if ((iSel != 0x1))
        goto L_1494;
    else
        goto L_148b;

L_148b:
    grbitScanEShip = (grbitScanEShip ^ 0xff);
    goto L_149a;

L_1494:
    grbitScanEShip = 0x0;

L_149a:
    if (((grbitScan & 0x800) != 0x0))
        goto L_1505;
    else
        goto L_14a8;

L_14a8:
    if ((grbitScanEShip != 0x0))
        goto LInvalE;
    else
        goto L_14af;

L_14af:

L_14b8:
    iSel = (iSel - 0x4);
    grbitScanEShip = (grbitScanEShip ^ (0x1 << iSel));
    if (((grbitScan & 0x800) != 0x0))
        goto L_1505;
    else
        goto L_14d6;

L_14d6:
    if ((((0x1 << iSel) & grbitScanEShip) == 0x0))
        goto L_1505;
    else
        goto LInvalE;

LInvalE:
    grbitScan = (grbitScan | 0x800);
    InvalidateRect(hwndTb, 0x0, 0x1);

L_1505:
    if (((grbitScan & 0x800) == 0x0))
        goto L_167a;
    else
        goto L_1510;

L_1510:

L_1516:
    goto L_1644;
    c = 0x0;
    i = 0x0;
    goto L_159d;

L_1526:
    if (((iScanZoom + 0x4) != i))
        goto L_153b;
    else
        goto L_1534;

L_1534:
    t_merge_153f_0001 = 0x1;
    goto L_153f;

L_153b:
    t_merge_153f_0001 = 0x0;

L_153f:
    LOWORD(rgid[c]) = t_merge_153f_0001;
    HIWORD(rgid[c]) = 0x0;
    /* untranslated: call _wsprintf((0x57a4 + (i * 0x8)), PCTDPCTPCT, 0xe:[(i * 0x2)+0xda4]) -> callresult(int16_t) */
    c = (c + 0x1);
    rgszScan[c] = (0x57a4 + (i * 0x8));
    i = (i + 0x1);

L_159d:
    if ((i < 0x9))
        goto L_1526;
    else
        goto L_15a6;

L_15a6:
    GetCursorPos(&(pt));
    ScreenToClient(hwndTb, &(pt));
    iSel = PopupMenu(hwndTb, pt.x, pt.y, c, rgid, rgszScan, 0xfffe, 0x0);
    if ((iSel == 0xffff))
        goto L_167a;
    else
        goto L_15f0;

L_15f0:

L_15f6:
    CommandHandler(hwndFrame, (iSel + 0xf3d));
    goto L_167a;

L_160f:
    if ((itb > 0x11))
        goto L_0dd1;
    else
        goto L_1617;

L_1617:
    goto L_ffffffff;

L_1644:
    if ((itb == 0x6))
        goto L_1662;
    else
        goto L_164d;

L_164d:
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_1662:
    if ((gd.fTutorial == 0x0))
        goto L_167a;
    else
        goto L_1675;

L_1675:
    AdvanceTutor();

L_167a:
    return;
}

void TerminateToolbarFocus(int16_t fCancel) {
    int16_t pct;
    char   *psz;

L_1680:
    gd.fChgScanner = 0x1;
    if ((fCancel != 0x0))
        goto L_171d;
    else
        goto L_169e;

L_169e:
    GetWindowText(hwndTBRadar, szWork, 0x14);
    psz = 0x57a4;
    pct = 0x0;

L_16bc:
    if (((uint16_t)(*(psz)) < 0x30))
        goto L_16f9;
    else
        goto L_16ca;

L_16ca:
    if (((uint16_t)(*(psz)) > 0x39))
        goto L_16f9;
    else
        goto L_16d8;

L_16d8:
    pct = (LOWORD((0xa * pct)) + ((uint16_t)(*(psz)) + 0xffd0));
    psz = (psz + 0x1);
    goto L_16bc;

L_16f9:
    if (((uint16_t)(*(psz)) == 0x0))
        goto L_1723;
    else
        goto L_1707;

L_1707:
    if (((uint16_t)(*(psz)) == 0x25))
        goto L_1723;
    else
        goto L_1715;

L_1715:
    pct = 0x0;

L_171d:
    pct = vpctRadarView;

L_1723:
    if ((pct >= 0x2))
        goto L_1734;
    else
        goto L_172c;

L_172c:
    pct = 0x2;
    goto L_1742;

L_1734:
    if ((pct <= 0x64))
        goto L_1742;
    else
        goto L_173d;

L_173d:
    pct = 0x64;

L_1742:
    SendMessage(hwndTBRadar, CB_SETCURSEL, ((uint32_t)((0x64 - pct)) / 0xa), 0x0);
    _wsprintf(szWork, PCTDPCTPCT, pct);
    SetWindowText(hwndTBRadar, szWork);
    if ((pct == vpctRadarView))
        goto L_17db;
    else
        goto L_1798;

L_1798:
    vpctRadarView = pct;
    if (((grbitScan & 0x20) != 0x0))
        goto L_17c1;
    else
        goto L_17ac;

L_17ac:
    InvalidateRect(hwndTb, 0x0, 0x1);

L_17c1:
    grbitScan = (grbitScan | 0x20);
    InvalidateRect(hwndScanner, 0x0, 0x1);

L_17db:
    SetFocus(hwndFrame);
    return;
}

void ShowTooltip(StringId ids, RECT *prc) {
    HDC      hdc;
    HFONT    hfontSav;
    int16_t  fVisCur;
    int16_t  cch;
    int16_t  fShowNow;
    uint16_t t_merge_1817_0001;
    uint16_t t_merge_19c1_0001;

L_17ea:
    if ((hwndTooltip == 0x0))
        goto L_1814;
    else
        goto L_17fd;

L_17fd:
    if ((IsWindowVisible(hwndTooltip) == 0x0))
        goto L_1814;
    else
        goto L_180e;

L_180e:
    t_merge_1817_0001 = 0x1;
    goto L_1817;

L_1814:
    t_merge_1817_0001 = 0x0;

L_1817:
    fVisCur = t_merge_1817_0001;
    if ((ids < idsUniverseDefinitionFileSeemsMissingCorrupt))
        goto L_182c;
    else
        goto L_1823;

L_1823:
    if ((prc != 0x0))
        goto L_1857;
    else
        goto L_182c;

L_182c:
    if ((hwndTooltip == 0x0))
        goto L_183f;
    else
        goto L_1836;

L_1836:
    DestroyWindow(hwndTooltip);

L_183f:
    if ((fVisCur == 0x0))
        goto L_19dd;
    else
        goto L_1848;

L_1848:
    vtickTooltipLast = GetTickCount();

L_1857:
    if ((ids != vidsTooltip))
        goto L_1880;
    else
        goto L_1862;

L_1862:
    if ((EqualRect(prc, vrcTooltip) != 0x0))
        goto L_19dd;
    else
        goto L_187a;

L_187a:

L_1880:
    vidsTooltip = ids;
    cch = CchGetString(vidsTooltip, 0x57a4);
    vrcTooltip = *(prc);
    hdc = GetDC(0x0);
    hfontSav = SelectObject(hdc, LOWORD(rghfontArial8));
    dxTip = LOWORD(GetTextExtent(hdc, szWork, cch));
    SelectObject(hdc, hfontSav);
    ReleaseDC(0x0, hdc);
    if ((hwndTooltip != 0x0))
        goto L_1942;
    else
        goto L_18f9;

L_18f9:
    CreateWindow(szTooltip, 0x0, WS_POPUP, 0x64, 0x64, (dxTip + 0x6), (dyArial8 + 0x6), hwndTb, 0x0, hInst, 0x0);
    goto L_1957;

L_1942:
    InvalidateRect(hwndTooltip, 0x0, 0x1);

L_1957:
    SetWindowPos(hwndTooltip, 0xffff, 0x0, 0x0, (dxTip + 0x6), (dyArial8 + 0x6), 0x216);
    /* untranslated: ss:[bp-0x10] = (LOWORD(vtickTooltipLast) + 0x190) */
    /* untranslated: ss:[bp-0xe] = (HIWORD(vtickTooltipLast) + 0x0) */
    /* untranslated: branch ss:[bp-0xe] > hiword(GetTickCount()) ? L_19b8 : L_19a3 */

L_19a3:
    /* untranslated: branch ss:[bp-0xe] < hiword(callresult(uint32_t)) ? L_19af : L_19a8 */

L_19a8:
    /* untranslated: branch ss:[bp-0x10] >= loword(callresult(uint32_t)) ? L_19b8 : L_19af */

L_19af:
    if ((fVisCur == 0x0))
        goto L_19be;
    else
        goto L_19b8;

L_19b8:
    t_merge_19c1_0001 = 0x1;
    goto L_19c1;

L_19be:
    t_merge_19c1_0001 = 0x0;

L_19c1:
    fShowNow = t_merge_19c1_0001;
    TooltipWndProc(hwndTooltip, 0x5f3, fShowNow, 0x0);

L_19dd:
    return;
}

int32_t TooltipWndProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
    HDC         hdc;
    POINT       pt;
    PAINTSTRUCT ps;
    RECT        rc;
    int16_t     bkSav;
    int16_t     cch;

L_19e4:
    goto L_1d08;

L_19f3:
    hwndTooltip = hwnd;
    goto L_1d53;

L_19fc:
    hwndTooltip = 0x0;
    if ((vidTimerTooltip == 0xffff))
        goto L_1a1e;
    else
        goto L_1a0c;

L_1a0c:
    KillTimer(hwnd, vidTimerTooltip);
    vidTimerTooltip = 0xffff;

L_1a1e:
    vidsTooltip = 0xffff;
    goto L_1d53;

L_1a27:
    DestroyWindow(hwnd);
    return 0x0;

L_1a38:
    if ((wParam != 0x0))
        goto L_1a8f;
    else
        goto L_1a41;

L_1a41:
    if ((vidTimerTooltip == 0xffff))
        goto L_1a57;
    else
        goto L_1a4b;

L_1a4b:
    KillTimer(hwnd, vidTimerTooltip);

L_1a57:
    if ((SetTimer(hwnd, 0x39e, 0x2bc, 0x0) == 0x0))
        goto L_1a80;
    else
        goto L_1a77;

L_1a77:
    vidTimerTooltip = 0x39e;
    goto L_1a86;

L_1a80:
    vidTimerTooltip = 0xffff;

L_1a86:
    return 0x0;

L_1a8f:
    wParam = 0x39e;

L_1a94:
    if ((wParam != 0x39e))
        goto L_1d53;
    else
        goto L_1a9b;

L_1a9b:

L_1aa1:
    if ((msg != WM_TIMER))
        goto L_1abb;
    else
        goto L_1aab;

L_1aab:
    if ((IsWindowVisible(hwnd) != 0x0))
        goto L_1baf;
    else
        goto L_1abb;

L_1abb:
    vtickTooltip1stVis = GetTickCount();
    GetCursorPos(&(pt));
    /* untranslated: branch PtInRect(vrcTooltip, words(pt.y, pt.x)) == 0x0 ? L_1c05 : L_1aea */

L_1aea:

L_1af0:
    ScreenToClient(hwndFrame, &(pt));
    if (((pt.x + dxTip) <= vfs.dx))
        goto L_1b1d;
    else
        goto L_1b10;

L_1b10:
    pt.x = ((vfs.dx - dxTip) + 0xfffb);

L_1b1d:
    ClientToScreen(hwndFrame, &(pt));
    /* untranslated: call SetWindowPos(hwnd, 0xffff, pt.x, ((words(loword((0x3 * dyArial8)), signhiword(loword((0x3 * dyArial8)))) / 0x2) + pt.y), 0x0, 0x0,
     * 0x251) -> callresult(int16_t) */
    UpdateWindow(hwnd);
    if ((vidTimerTooltip == 0xffff))
        goto L_1b77;
    else
        goto L_1b6b;

L_1b6b:
    KillTimer(hwnd, vidTimerTooltip);

L_1b77:
    if ((SetTimer(hwnd, 0x39e, 0x32, 0x0) == 0x0))
        goto L_1ba0;
    else
        goto L_1b97;

L_1b97:
    vidTimerTooltip = 0x39e;
    goto L_1ba6;

L_1ba0:
    vidTimerTooltip = 0xffff;

L_1ba6:
    return 0x0;

L_1baf:
    vtickTooltipLast = GetTickCount();
    GetCursorPos(&(pt));
    /* untranslated: branch PtInRect(vrcTooltip, words(pt.y, pt.x)) == 0x0 ? L_1c05 : L_1be1 */

L_1be1:
    if (((HIWORD(vtickTooltip1stVis) + 0x0) > HIWORD(vtickTooltipLast)))
        goto L_1c0d;
    else
        goto L_1bf7;

L_1bf7:
    if (((HIWORD(vtickTooltip1stVis) + 0x0) < HIWORD(vtickTooltipLast)))
        goto LKillTip;
    else
        goto L_1bfc;

L_1bfc:
    if (((LOWORD(vtickTooltip1stVis) + 0x2710) >= LOWORD(vtickTooltipLast)))
        goto L_1c0d;
    else
        goto LKillTip;

LKillTip:
    DestroyWindow(hwnd);

L_1c0d:
    return 0x0;

L_1c16:
    GetClientRect(hwnd, &(rc));
    FillRect(wParam, &(rc), hbrTooltip);
    return 0x10000;

L_1c41:
    hdc = BeginPaint(hwnd, &(ps));
    GetClientRect(hwnd, &(rc));
    FrameRect(hdc, &(rc), hbrWindowFrame);
    SelectObject(hdc, LOWORD(rghfontArial8));
    cch = CchGetString(vidsTooltip, 0x57a4);
    bkSav = SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, crWindowText);
    ExtTextOut(hdc, 0x3, 0x3, 0x0, 0x0, szWork, cch, 0x0);
    SetBkMode(hdc, bkSav);
    EndPaint(hwnd, &(ps));
    return 0x0;

L_1d08:
    if ((msg == WM_CREATE))
        goto L_19f3;
    else
        goto L_1d10;

L_1d10:
    if ((msg == WM_DESTROY))
        goto L_19fc;
    else
        goto L_1d18;

L_1d18:
    if ((msg == WM_PAINT))
        goto L_1c41;
    else
        goto L_1d20;

L_1d20:
    if ((msg == WM_ERASEBKGND))
        goto L_1c16;
    else
        goto L_1d28;

L_1d28:
    if ((msg == WM_TIMER))
        goto L_1a94;
    else
        goto L_1d30;

L_1d30:
    if ((msg == WM_MOUSEMOVE))
        goto L_1a27;
    else
        goto L_1d38;

L_1d38:
    if ((msg == WM_LBUTTONDOWN))
        goto L_1a27;
    else
        goto L_1d40;

L_1d40:
    if ((msg == WM_LBUTTONDBLCLK))
        goto L_1a27;
    else
        goto L_1d48;

L_1d48:
    if ((msg == 0x5f3))
        goto L_1a38;
    else
        goto L_1d53;

L_1d53:
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int32_t FakeComboProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
L_1d72:
    goto L_1dae;

L_1d81:
    ShowTooltip(0xffff, 0x0);
    goto L_1dc9;

L_1d94:
    TbWndProc(hwnd, msg, wParam, lParam);
    goto L_1dc9;

L_1dae:
    if ((msg == WM_MOUSEMOVE))
        goto L_1d94;
    else
        goto L_1db6;

L_1db6:
    if ((msg == WM_LBUTTONDOWN))
        goto L_1d81;
    else
        goto L_1dbe;

L_1dbe:
    if ((msg == WM_LBUTTONDBLCLK))
        goto L_1d81;
    else
        goto L_1dc9;

L_1dc9:
    CallWindowProc(lpfnRealComboProc, hwnd, msg, wParam, lParam);

L_1de8:
    /* untranslated: return callresult(LRESULT) */
}

int32_t FakeCEProc(HWND hwnd, WMType msg, uint16_t wParam, int32_t lParam) {
L_1df0:
    goto L_1e2c;

L_1dff:
    ShowTooltip(0xffff, 0x0);
    goto L_1e47;

L_1e12:
    TbWndProc(hwnd, msg, wParam, lParam);
    goto L_1e47;

L_1e2c:
    if ((msg == WM_MOUSEMOVE))
        goto L_1e12;
    else
        goto L_1e34;

L_1e34:
    if ((msg == WM_LBUTTONDOWN))
        goto L_1dff;
    else
        goto L_1e3c;

L_1e3c:
    if ((msg == WM_LBUTTONDBLCLK))
        goto L_1dff;
    else
        goto L_1e47;

L_1e47:
    CallWindowProc(lpfnRealCEProc, hwnd, msg, wParam, lParam);

L_1e66:
    /* untranslated: return callresult(LRESULT) */
}
