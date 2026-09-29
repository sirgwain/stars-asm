int16_t InitMDIApp() {
    WNDCLASS wc;

    wc.style = 0xb;
    wc.lpfnWndProc = (WNDPROC)FrameWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, "StarsIco");
    wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
    wc.hbrBackground = (HBRUSH)13;
    wc.lpszMenuName = "StarsMenu";
    wc.lpszClassName = szFrame;
    if (RegisterClass(&wc) != 0x0) {
        wc.style = 0x20b;
        wc.lpfnWndProc = (WNDPROC)MessageWndProc;
        wc.hIcon = 0x0;
        wc.lpszMenuName = 0x0;
        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
        wc.lpszClassName = szMessage;
        if (RegisterClass(&wc) != 0x0) {
            wc.style = 0x20b;
            wc.lpfnWndProc = (WNDPROC)ScannerWndProc;
            wc.hbrBackground = GetStockObject(BLACK_BRUSH);
            wc.lpszClassName = szScan;
            if (RegisterClass(&wc) != 0x0) {
                wc.style = 0x20b;
                wc.lpfnWndProc = (WNDPROC)MineWndProc;
                wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                wc.lpszClassName = szMine;
                if (RegisterClass(&wc) != 0x0) {
                    wc.style = 0x208;
                    wc.lpfnWndProc = (WNDPROC)TbWndProc;
                    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                    wc.lpszClassName = szTb;
                    if (RegisterClass(&wc) != 0x0) {
                        wc.style = 0x200;
                        wc.lpfnWndProc = (WNDPROC)PlanetWndProc;
                        wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                        wc.hIcon = 0x0;
                        wc.lpszClassName = szPlanet;
                        if (RegisterClass(&wc) != 0x0) {
                            wc.style = 0xa00;
                            wc.lpfnWndProc = (WNDPROC)PopupWndProc;
                            wc.hbrBackground = GetStockObject(WHITE_BRUSH);
                            wc.hIcon = 0x0;
                            wc.lpszClassName = szPopup;
                            if (RegisterClass(&wc) != 0x0) {
                                wc.style = 0xa00;
                                wc.lpfnWndProc = (WNDPROC)TooltipWndProc;
                                wc.hbrBackground = GetStockObject(WHITE_BRUSH);
                                wc.hIcon = 0x0;
                                wc.lpszClassName = szTooltip;
                                if (RegisterClass(&wc) != 0x0) {
                                    wc.style = 0x200;
                                    wc.lpfnWndProc = (WNDPROC)BrowserWndProc;
                                    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                                    wc.hIcon = 0x0;
                                    wc.lpszClassName = szBrowser;
                                    if (RegisterClass(&wc) != 0x0) {
                                        wc.style = 0x0;
                                        wc.lpfnWndProc = (WNDPROC)TitleWndProc;
                                        wc.cbClsExtra = 0;
                                        wc.cbWndExtra = 0;
                                        wc.hInstance = hInst;
                                        wc.hIcon = 0x0;
                                        wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
                                        wc.hbrBackground = GetStockObject(BLACK_BRUSH);
                                        wc.lpszMenuName = 0x0;
                                        wc.lpszClassName = szTitle;
                                        if (RegisterClass(&wc) != 0x0) {
                                            wc.style = 0xb;
                                            wc.lpfnWndProc = (WNDPROC)ReportDlg;
                                            wc.cbClsExtra = 0;
                                            wc.cbWndExtra = 0;
                                            wc.hInstance = hInst;
                                            wc.hIcon = 0x0;
                                            wc.hCursor = LoadCursor(0x0, MAKEINTRESOURCE(0x7f00));
                                            wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
                                            wc.lpszMenuName = 0x0;
                                            wc.lpszClassName = szReport;
                                            if (RegisterClass(&wc) != 0x0) {
                                                return 1;
                                            }
                                            return 0;
                                        }
                                        return 0;
                                    }
                                    return 0;
                                }
                                return 0;
                            }
                            return 0;
                        }
                        return 0;
                    }
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}
