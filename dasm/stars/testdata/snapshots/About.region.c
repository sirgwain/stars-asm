INT_PTR CALLBACK About(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT    rc;
    HDC     hdc;
    int16_t i;
    HWND    hwndCtl;
    FARPROC lpProc;

    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            iAbout1st = -11;
            iAboutPartial = 0;
            SetWindowText(GetDlgItem(hwnd, IDC_ABOUT_DEMO_TEXT), SzVersion());
            uTimerId = SetTimer(hwnd, 14, 50, NULL);
            return 1;
        case WM_TIMER:
            hwndCtl = GetDlgItem(hwnd, IDC_ABOUT_CREDITS_TEXT);
            iAboutPartial += 2;
            if (iAboutPartial >= dyArial8) {
                iAboutPartial = 0;
                iAbout1st++;
                if (iAbout1st > 78) {
                    iAbout1st = -11;
                }
            }
            GetClientRect(hwndCtl, &rc);
            hdc = GetDC(hwndCtl);
            SelectObject(hdc, rghfontArial8[1]);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, crButtonFace);
            SetTextColor(hdc, crButtonText);
            IntersectClipRect(hdc, 0, 0, rc.right, rc.bottom);
            rc.top -= iAboutPartial;
            rc.bottom = rc.top + dyArial8;
            for (i = iAbout1st; i < iAbout1st + 10; i++) {
                if (i >= 0 && i < 77) {
                    RcCtrTextOut(hdc, &rc, PszGetCompressedString(i + 631), -1);
                } else if (i >= 77) {
                    break;
                }
                OffsetRect(&rc, 0, dyArial8);
            }
            rc.bottom = 1000;
            FillRect(hdc, &rc, hbrButtonFace);
            SelectClipRgn(hdc, NULL);
            ReleaseDC(hwnd, hdc);
            break;
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                KillTimer(hwnd, uTimerId);
                uTimerId = 0;
                EndDialog(hwnd, 1);
                return 1;
            case IDC_ABOUT_ORDER_INFO:
                lpProc = MakeProcInstance(OrderInfoDlg, hInst);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ORDER_INFO), hwnd, lpProc);
                FreeProcInstance(lpProc);
            }
        }
    } else if (HIWORD(lParam) == 6) {
        SetBkColor((HDC)wParam, crButtonFace);
        return (INT_PTR)hbrButtonFace;
    }
    return 0;
}
