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
