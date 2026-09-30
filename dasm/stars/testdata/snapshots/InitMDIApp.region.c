int16_t InitMDIApp() {
    WNDCLASS wc;

    wc.style = 11;
    wc.lpfnWndProc = (WNDPROC)FrameWndProc16;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, "StarsIco");
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = (HBRUSH)13;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = szFrame;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)MessageWndProc;
    wc.hIcon = 0;
    wc.lpszMenuName = NULL;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMessage;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)ScannerWndProc;
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = szScan;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 523;
    wc.lpfnWndProc = (WNDPROC)MineWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szMine;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 520;
    wc.lpfnWndProc = (WNDPROC)TbWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szTb;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0x200;
    wc.lpfnWndProc = (WNDPROC)PlanetWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szPlanet;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 2560;
    wc.lpfnWndProc = (WNDPROC)PopupWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szPopup;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 2560;
    wc.lpfnWndProc = (WNDPROC)TooltipWndProc;
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szTooltip;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0x200;
    wc.lpfnWndProc = (WNDPROC)BrowserWndProc;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.hIcon = 0;
    wc.lpszClassName = szBrowser;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)TitleWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szTitle;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    wc.style = 11;
    wc.lpfnWndProc = (WNDPROC)ReportDlg;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szReport;
    if (RegisterClass(&wc) == 0) {
        return 0;
    }
    return 1;
}
