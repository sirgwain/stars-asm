int16_t InitMDIApp() {
    WNDCLASS wc;

L_0000:
    wc.style = 0xb;
    wc.lpfnWndProc = FrameWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = 0xd;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = szFrame;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0076;
    else
        goto L_0070;

L_0070:
    return 0x0;

L_0076:
    wc.style = 0x20b;
    wc.lpfnWndProc = MessageWndProc;
    wc.hIcon = 0x0;
    wc.lpszMenuName = 0x0;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMessage;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_00c5;
    else
        goto L_00bf;

L_00bf:
    return 0x0;

L_00c5:
    wc.style = 0x20b;
    wc.lpfnWndProc = ScannerWndProc;
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = szScan;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0105;
    else
        goto L_00ff;

L_00ff:
    return 0x0;

L_0105:
    wc.style = 0x20b;
    wc.lpfnWndProc = MineWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMine;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0145;
    else
        goto L_013f;

L_013f:
    return 0x0;

L_0145:
    wc.style = 0x208;
    wc.lpfnWndProc = TbWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szTb;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0185;
    else
        goto L_017f;

L_017f:
    return 0x0;

L_0185:
    wc.style = 0x200;
    wc.lpfnWndProc = PlanetWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = szPlanet;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_01ca;
    else
        goto L_01c4;

L_01c4:
    return 0x0;

L_01ca:
    wc.style = 0xa00;
    wc.lpfnWndProc = PopupWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = szPopup;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_020f;
    else
        goto L_0209;

L_0209:
    return 0x0;

L_020f:
    wc.style = 0xa00;
    wc.lpfnWndProc = TooltipWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = szTooltip;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0254;
    else
        goto L_024e;

L_024e:
    return 0x0;

L_0254:
    wc.style = 0x200;
    wc.lpfnWndProc = BrowserWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0x0;
    wc.lpszClassName = szBrowser;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_0299;
    else
        goto L_0293;

L_0293:
    return 0x0;

L_0299:
    wc.style = 0x0;
    wc.lpfnWndProc = TitleWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0x0;
    wc.lpszClassName = szTitle;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_030c;
    else
        goto L_0306;

L_0306:
    return 0x0;

L_030c:
    wc.style = 0xb;
    wc.lpfnWndProc = ReportDlg;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0x0;
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = 0x0;
    wc.lpszClassName = szReport;
    if ((RegisterClass(&(wc)) != 0x0))
        goto L_037f;
    else
        goto L_0379;

L_0379:
    return 0x0;

L_037f:
    return 0x1;
}
