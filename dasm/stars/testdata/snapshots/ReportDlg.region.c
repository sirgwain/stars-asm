LRESULT CALLBACK ReportDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    HMENU       hmenu;
    RECT        rc;
    int16_t     i;
    int16_t     dx;
    uint16_t    swp;
    int16_t     cRow;
    POINT16     pt;
    int16_t     ibit;
    int16_t     iCol;
    int16_t     iRow;
    int16_t     xCur;
    int16_t     iCur;
    int16_t     iNew;
    PAINTSTRUCT ps;
    MessageId   idm;

    switch (msg) {
    case WM_CREATE:
        hwndReportDlg = hwnd;
        hdc = GetDC(hwnd);
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < vprptCur->cFields; i++) {
            dx = DxReportColHdr(vprptCur->irpt, i, szWork, hdc);
            vprptCur->rgbdx[i] = dx / 2;
        }
        ReleaseDC(hwnd, hdc);
        SortReportCache(vprptCur->irpt, vprptCur->icolSort);
        SetWindowPos(hwnd, NULL, 0, 0, vprptCur->ptSize.x, vprptCur->ptSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
        StickyDlgPos(hwnd, &vprptCur->ptDlg, 1);
        vprptCur->hwndVScroll = CreateWindow("SCROLLBAR", NULL, SBS_VERT | WS_CHILD, 0, 0, 50, 50, hwnd, NULL, hInst, NULL);
        vprptCur->hwndHScroll = CreateWindow("SCROLLBAR", NULL, WS_CHILD, 0, 0, 50, 50, hwnd, NULL, hInst, NULL);
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    case WM_SIZE:
        GetClientRect(hwnd, &rc);
        cRow = (int16_t)(rc.bottom - 36) / (dyArial8 + 4);
        vprptCur->cRowsVis = cRow >= vprptCur->cRows ? vprptCur->cRows : cRow;
        if (vprptCur->cRowsVis >= vprptCur->cRows) {
            swp = SWP_NOZORDER | SWP_HIDEWINDOW;
            vprptCur->irowFirst = 0;
            SetScrollPos(vprptCur->hwndVScroll, SB_CTL, 0, 0);
        } else {
            swp = SWP_NOZORDER | SWP_SHOWWINDOW;
            if (vprptCur->irowFirst + vprptCur->cRowsVis > vprptCur->cRows && vprptCur->irowFirst > 0) {
                vprptCur->irowFirst = vprptCur->cRows - vprptCur->cRowsVis;
                if (vprptCur->irowFirst < 0) {
                    vprptCur->irowFirst = 0;
                }
            }
            SetScrollPos(vprptCur->hwndVScroll, SB_CTL, vprptCur->irowFirst, 0);
            SetScrollRange(vprptCur->hwndVScroll, SB_CTL, 0, vprptCur->cRows - vprptCur->cRowsVis, 1);
        }
        dx = GetSystemMetrics(SM_CXVSCROLL);
        SetWindowPos(vprptCur->hwndVScroll, NULL, rc.right - dx, dyArial8 + 6, dx, (dyArial8 + 4) * vprptCur->cRowsVis + 1, swp);
        SetHScrollBar();
        if (msg == WM_CREATE) {
            return 1;
        }
        return 0;
    case WM_GETMINMAXINFO:
        ((MINMAXINFO *)lParam)->ptMinTrackSize.x = 300;
        ((MINMAXINFO *)lParam)->ptMinTrackSize.y = 220;
        return 0;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
        xCur = 2;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (pt.y < 2 || pt.x < 2)
            goto L_09c8;
        if (pt.y < dyArial8 + 6) {
            iRow = -1;
        } else {
            iRow = (int16_t)(pt.y - 2 - (dyArial8 + 4)) / (dyArial8 + 4);
            if (iRow >= vprptCur->cRowsVis)
                goto L_09c8;
            iRow += vprptCur->irowFirst;
        }
        iCol = -1;
        i = 0;
        ibit = 1;
        while (i < vprptCur->cFields) {
            if ((ibit & vprptCur->grbitVisible) != 0 && (i == 0 || i >= vprptCur->cFieldFirst)) {
                xCur += vprptCur->rgbdx[i] * 2;
                if (xCur > pt.x) {
                    iCol = i;
                    break;
                }
            }
            i++;
            ibit *= 2;
        }
        if (iCol == -1)
            goto L_09c8;
        if (iRow == -1) {
            ReportColumnPopup(pt, iCol, msg == WM_RBUTTONDOWN);
        } else {
            ExecuteReportClick(pt, vprptCur->irpt, iCol, iRow);
        }
        if (gd.fTutorial == 0)
            goto L_09c8;
        AdvanceTutor();
        goto L_09c8;
    case WM_VSCROLL:
        iCur = GetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), SB_CTL);
        iNew = iCur;
        if (GET_WM_VSCROLL_CODE(wParam, lParam) <= SB_BOTTOM) {
            switch (GET_WM_VSCROLL_CODE(wParam, lParam)) {
            case SB_BOTTOM:
                iNew = 2000;
                break;
            case SB_LINEDOWN:
                iNew++;
                break;
            case SB_LINEUP:
                iNew--;
                break;
            case SB_PAGEDOWN:
                iNew += vprptCur->cRowsVis - 1;
                break;
            case SB_PAGEUP:
                iNew -= vprptCur->cRowsVis - 1;
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                iNew = GET_WM_VSCROLL_POS(wParam, lParam);
                break;
            case SB_TOP:
                iNew = 0;
            }
        }
        if (iNew > vprptCur->cRows - vprptCur->cRowsVis) {
            iNew = vprptCur->cRows - vprptCur->cRowsVis;
        }
        if (iNew < 0) {
            iNew = 0;
        }
        if (iNew != iCur) {
            vprptCur->irowFirst = iNew;
            GetClientRect(hwnd, &rc);
            rc.left = 2;
            rc.right -= GetSystemMetrics(SM_CXVSCROLL);
            rc.top = dyArial8 + 6;
            rc.bottom = (dyArial8 + 4) * vprptCur->cRowsVis + rc.top;
            ScrollWindow(hwnd, 0, (dyArial8 + 4) * (iCur - iNew), &rc, &rc);
            SetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), SB_CTL, iNew, 1);
            UpdateWindow(hwnd);
        }
        return 0;
    case WM_HSCROLL:
        iCur = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), SB_CTL);
        iNew = iCur;
        if (GET_WM_HSCROLL_CODE(wParam, lParam) <= SB_BOTTOM) {
            switch (GET_WM_HSCROLL_CODE(wParam, lParam)) {
            case SB_BOTTOM:
                iNew = 2000;
                break;
            case SB_LINEDOWN:
                iNew++;
                break;
            case SB_LINEUP:
                iNew--;
                break;
            case SB_PAGEDOWN:
                iNew += 3;
                break;
            case SB_PAGEUP:
                iNew -= 3;
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                iNew = GET_WM_HSCROLL_POS(wParam, lParam);
                break;
            case SB_TOP:
                iNew = 0;
            }
        }
        if (iNew > vprptCur->cColScroll) {
            iNew = vprptCur->cColScroll;
        }
        if (iNew < 0) {
            iNew = 0;
        }
        if (iNew != iCur) {
            SetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), SB_CTL, iNew, 1);
            iNew = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), SB_CTL);
            if (iNew != iCur) {
                i = 1;
                for (ibit = 2; i < vprptCur->cFields && ((ibit & vprptCur->grbitVisible) == 0 || iNew-- > 0); ibit *= 2) {
                    i++;
                }
                vprptCur->cFieldFirst = i;
                InvalidateRect(hwnd, NULL, 1);
                UpdateWindow(hwnd);
            }
        }
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawReport(hwnd, hdc, &ps.rcPaint);
        EndPaint(hwnd, &ps);
        gd.fRptSafeDraw = 0;
        return 1;
    case WM_DESTROY:
        StickyDlgPos(hwnd, &vprptCur->ptDlg, 0);
        GetWindowRect(hwnd, &rc);
        vprptCur->ptSize.x = rc.right - rc.left;
        vprptCur->ptSize.y = rc.bottom - rc.top;
        hwndReportDlg = 0;
        fBrowserValid = 0;
        hmenu = GetASubMenu(hwndFrame, menuReport);
        switch (vprptCur->irpt) {
        case rptFleets:
            idm = 2303;
            break;
        case rptEnemyFleets:
            idm = 2304;
            break;
        case rptPlanets:
            idm = 2301;
            break;
        case rptBattles:
            idm = 2305;
        }
        CheckMenuItem(hmenu, idm, MF_UNCHECKED);
        vprptCur = 0;
        if (gd.fTutorial == 0)
            goto L_09c8;
        AdvanceTutor();
        goto L_09c8;
    case WM_COMMAND:
        if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) {
            DestroyWindow(hwnd);
            return 1;
        }
    default:
    L_09c8:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
