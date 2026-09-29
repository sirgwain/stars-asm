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
    if (IS_WM_CTLCOLOR(message) != 0) {
        if (HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        switch (message) {
        case WM_INITDIALOG:
            iAbout1st = -11;
            iAboutPartial = 0;
            SetWindowText(GetDlgItem(hwnd, 0x401), SzVersion());
            uTimerId = SetTimer(hwnd, 0xe, 0x32, 0x0);
            return 1;
        case WM_TIMER:
            hwndCtl = GetDlgItem(hwnd, IDC_U16_0x041F);
            iAboutPartial = iAboutPartial + 2;
            if (iAboutPartial >= dyArial8) {
                iAboutPartial = 0;
                iAbout1st = iAbout1st + 1;
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
            rc.top = rc.top - iAboutPartial;
            rc.bottom = rc.top + dyArial8;
            for (i = iAbout1st; i < iAbout1st + 10; i++) {
                if (i < 0 || i >= 77) {
                    if (i >= 77)
                        break;
                } else {
                    RcCtrTextOut(hdc, &rc, PszGetCompressedString(i + 631), -1);
                }
                OffsetRect(&rc, 0, dyArial8);
            }
            rc.bottom = 1000;
            FillRect(hdc, &rc, hbrButtonFace);
            SelectClipRgn(hdc, 0x0);
            ReleaseDC(hwnd, hdc);
            break;
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                KillTimer(hwnd, uTimerId);
                uTimerId = 0x0;
                EndDialog(hwnd, 1);
                return 1;
            case IDC_HELP:
                lpProc = MakeProcInstance(OrderInfoDlg, hInst);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ORDER_INFO), hwnd, lpProc);
                FreeProcInstance(lpProc);
            default:
            }
        default:
        }
    }
    return 0;
}
