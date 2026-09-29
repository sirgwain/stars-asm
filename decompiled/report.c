#include "common.h"

uint16_t mpicolgrbitBU[12] = {255, 255, 255, 255, 255, 255, 255, 8, 16, 32, 64, 128};

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
    int16_t     t_07d4;

    switch (msg) {
    case WM_CREATE:
        hwndReportDlg = hwnd;
        hdc = GetDC(hwnd);
        SelectObject(hdc, rghfontArial8[1]);
        for (i = 0; i < vprptCur->cFields; i++) {
            dx = DxReportColHdr(vprptCur->irpt, i, szWork, hdc);
            vprptCur->rgbdx[i] = LOBYTE((int32_t)dx / 2);
        }
        ReleaseDC(hwnd, hdc);
        SortReportCache(vprptCur->irpt, vprptCur->icolSort);
        SetWindowPos(hwnd, 0x0, 0, 0, vprptCur->ptSize.x, vprptCur->ptSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
        StickyDlgPos(hwnd, &vprptCur->ptDlg, 1);
        vprptCur->hwndVScroll = CreateWindow("SCROLLBAR", 0x0, SBS_VERT | WS_CHILD, 0, 0, 50, 50, hwnd, 0x0, hInst, 0x0);
        vprptCur->hwndHScroll = CreateWindow("SCROLLBAR", 0x0, WS_CHILD, 0, 0, 50, 50, hwnd, 0x0, hInst, 0x0);
        if (gd.fTutorial != 0x0) {
            AdvanceTutor();
        }
    case WM_SIZE:
        GetClientRect(hwnd, &rc);
        cRow = (int32_t)(rc.bottom - 36) / (dyArial8 + 4);
        vprptCur->cRowsVis = cRow >= vprptCur->cRows ? vprptCur->cRows : cRow;
        if (vprptCur->cRowsVis < vprptCur->cRows) {
            swp = 0x44;
            if (vprptCur->irowFirst + vprptCur->cRowsVis > vprptCur->cRows && vprptCur->irowFirst > 0) {
                vprptCur->irowFirst = vprptCur->cRows - vprptCur->cRowsVis;
                if (vprptCur->irowFirst < 0) {
                    vprptCur->irowFirst = 0;
                }
            }
            SetScrollPos(vprptCur->hwndVScroll, 2, vprptCur->irowFirst, 0);
            SetScrollRange(vprptCur->hwndVScroll, 2, 0, vprptCur->cRows - vprptCur->cRowsVis, 1);
        } else {
            swp = 0x84;
            vprptCur->irowFirst = 0;
            SetScrollPos(vprptCur->hwndVScroll, 2, 0, 0);
        }
        dx = GetSystemMetrics(SM_CXVSCROLL);
        SetWindowPos(vprptCur->hwndVScroll, 0x0, rc.right - dx, dyArial8 + 6, dx, (dyArial8 + 4) * vprptCur->cRowsVis + 1, swp);
        SetHScrollBar();
        if (msg != WM_CREATE) {
            return 0;
        }
        return 1;
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
        if (pt.y >= dyArial8 + 6) {
            iRow = (int32_t)(pt.y - 2 - (dyArial8 + 4)) / (dyArial8 + 4);
            if (iRow >= vprptCur->cRowsVis)
                goto L_09c8;
            iRow = iRow + vprptCur->irowFirst;
        } else {
            iRow = -1;
        }
        iCol = -1;
        i = 0;
        ibit = 1;
        while (1) {
            if (i >= vprptCur->cFields)
                goto L_0489;
            if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0 && (i == 0 || i >= vprptCur->cFieldFirst)) {
                xCur = xCur + vprptCur->rgbdx[i] * 2;
                if (xCur > pt.x)
                    break;
            }
            i = i + 1;
            ibit = ibit * 2;
        }
        iCol = i;
    L_0489:
        if (iCol == -1)
            goto L_09c8;
        if (iRow != -1) {
            ExecuteReportClick(pt, vprptCur->irpt, iCol, iRow);
        } else {
            ReportColumnPopup(pt, iCol, msg == WM_RBUTTONDOWN ? 1 : 0);
        }
        if (gd.fTutorial == 0x0)
            goto L_09c8;
        AdvanceTutor();
        goto L_09c8;
    case WM_VSCROLL:
        iCur = GetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), 2);
        iNew = iCur;
        if (GET_WM_VSCROLL_CODE(wParam, lParam) <= SB_BOTTOM) {
            switch (GET_WM_VSCROLL_CODE(wParam, lParam)) {
            case 7:
                iNew = 2000;
                break;
            case 1:
                iNew = iNew + 1;
                break;
            case 0:
                iNew = iNew - 1;
                break;
            case 3:
                iNew = iNew + (vprptCur->cRowsVis - 1);
                break;
            case 2:
                iNew = iNew - (vprptCur->cRowsVis - 1);
                break;
            case 4:
            case 5:
                iNew = GET_WM_VSCROLL_POS(wParam, lParam);
                break;
            case 6:
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
            rc.right = rc.right - GetSystemMetrics(SM_CXVSCROLL);
            rc.top = dyArial8 + 6;
            rc.bottom = (dyArial8 + 4) * vprptCur->cRowsVis + rc.top;
            ScrollWindow(hwnd, 0, (dyArial8 + 4) * (iCur - iNew), &rc, &rc);
            SetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), 2, iNew, 1);
            UpdateWindow(hwnd);
        }
        return 0;
    case WM_HSCROLL:
        iCur = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2);
        iNew = iCur;
        if (GET_WM_HSCROLL_CODE(wParam, lParam) <= SB_BOTTOM) {
            switch (GET_WM_HSCROLL_CODE(wParam, lParam)) {
            case 7:
                iNew = 2000;
                break;
            case 1:
                iNew = iNew + 1;
                break;
            case 0:
                iNew = iNew - 1;
                break;
            case 3:
                iNew = iNew + 3;
                break;
            case 2:
                iNew = iNew - 3;
                break;
            case 4:
            case 5:
                iNew = GET_WM_HSCROLL_POS(wParam, lParam);
                break;
            case 6:
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
            SetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2, iNew, 1);
            iNew = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2);
            if (iNew != iCur) {
                i = 1;
                ibit = 2;
                while (i < vprptCur->cFields) {
                    if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0) {
                        t_07d4 = iNew;
                        iNew = iNew - 1;
                        if (t_07d4 <= 0)
                            break;
                    }
                    i = i + 1;
                    ibit = ibit * 2;
                }
                vprptCur->cFieldFirst = i;
                InvalidateRect(hwnd, 0x0, 1);
                UpdateWindow(hwnd);
            }
        }
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawReport(hwnd, hdc, &ps.rcPaint);
        EndPaint(hwnd, &ps);
        gd.fRptSafeDraw = 0x0;
        return 1;
    case WM_DESTROY:
        StickyDlgPos(hwnd, &vprptCur->ptDlg, 0);
        GetWindowRect(hwnd, &rc);
        vprptCur->ptSize.x = rc.right - rc.left;
        vprptCur->ptSize.y = rc.bottom - rc.top;
        hwndReportDlg = 0x0;
        fBrowserValid = 0;
        hmenu = GetASubMenu(hwndFrame, 4);
        switch (vprptCur->irpt) {
        case 1:
            idm = 0x8ff;
            break;
        case 2:
            idm = 0x900;
            break;
        case 0:
            idm = 0x8fd;
            break;
        case 3:
            idm = 0x901;
        default:
        }
        CheckMenuItem(hmenu, idm, 0x0);
        vprptCur = 0x0;
        if (gd.fTutorial == 0x0)
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

void SetHScrollBar() {
    uint16_t swp;
    int16_t  dy;
    int16_t  ccolSkipped;
    int16_t  ccolHidden;
    int16_t  i;
    int16_t  ibit;
    int16_t  xRight;
    int16_t  dx;
    int16_t  xTitle;
    RECT     rc;

    GetClientRect(hwndReportDlg, &rc);
    dx = GetSystemMetrics(SM_CXVSCROLL);
    xTitle = vprptCur->rgbdx[0] * 2 + 2;
    xRight = rc.right - xTitle - 2 - dx;
    ccolSkipped = 0;
    ccolHidden = 0;
    i = vprptCur->cFields - 1;
    ibit = 0x1 << vprptCur->cFields;
    while (i > 0) {
        if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0) {
            if (i < vprptCur->cFieldFirst) {
                ccolSkipped = ccolSkipped + 1;
            }
            xRight = xRight - vprptCur->rgbdx[i] * 2;
            if (xRight < 0) {
                ccolHidden = ccolHidden + 1;
            }
        }
        i = i - 1;
        ibit = ibit >> 0x1;
    }
    vprptCur->cColScroll = 0;
    if (ccolHidden != 0) {
        swp = 0x44;
        if (ccolSkipped > ccolHidden) {
            ccolSkipped = 0;
            vprptCur->cFieldFirst = 1;
        }
        SetScrollPos(vprptCur->hwndHScroll, 2, ccolSkipped, 0);
        SetScrollRange(vprptCur->hwndHScroll, 2, 0, ccolHidden, 1);
        vprptCur->cColScroll = ccolHidden;
    } else {
        swp = 0x84;
        vprptCur->cFieldFirst = 1;
        SetScrollPos(vprptCur->hwndHScroll, 2, 0, 0);
    }
    dy = GetSystemMetrics(SM_CYHSCROLL);
    SetWindowPos(vprptCur->hwndHScroll, 0x0, xTitle, dyArial8 + 6 + (dyArial8 + 4) * vprptCur->cRowsVis + 1, rc.right - dx - xTitle, dy, swp);
    return;
}

void DrawReport(HWND hwnd, HDC hdc, RECT *prc) {
    char    szTit[40];
    int16_t irowLast;
    int16_t j;
    int16_t i;
    int16_t yRow;
    int16_t ibit;
    int16_t dx;
    int16_t xCol;
    RECT    rc;

    ibit = 1;
    xCol = 2;
    yRow = 2;
    SelectObject(hdc, rghfontArial8[1]);
    SetBkMode(hdc, TRANSPARENT);
    if (prc->top <= dyArial8 + 4 + yRow) {
        i = 0;
        while (i < vprptCur->cFields) {
            if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0 && (i == 0 || i >= vprptCur->cFieldFirst)) {
                dx = DxReportColHdr(vprptCur->irpt, i, szTit, hdc);
                vprptCur->rgbdx[i] = LOBYTE((int32_t)dx / 2);
                SetRect(&rc, xCol, yRow, xCol + dx - 1, dyArial8 + 4 + yRow);
                if (gd.fRptSafeDraw != 0x0) {
                    FillRect(hdc, &rc, hbrButtonFace);
                }
                if (i != 0) {
                    CtrTextOut(hdc, (int32_t)(rc.right - rc.left) / 2 + rc.left, rc.top + 2, szTit, 0);
                } else {
                    TextOut(hdc, rc.left + 3, rc.top + 2, szTit, strlen(szTit));
                }
                _Draw3dFrame(hdc, &rc, 0);
                xCol = xCol + dx;
            }
            i = i + 1;
            ibit = ibit * 2;
        }
    }
    irowLast = vprptCur->irowFirst + vprptCur->cRowsVis;
    if (irowLast > vprptCur->cRows) {
        irowLast = vprptCur->cRows;
    }
    for (i = vprptCur->irowFirst; i < irowLast; i++) {
        yRow = yRow + (dyArial8 + 4);
        xCol = 2;
        if (yRow >= prc->top && yRow <= prc->bottom) {
            SelectObject(hdc, hbrButtonShadow);
            PatBlt(hdc, xCol, yRow, 1, dyArial8 + 4, PATCOPY);
        }
        j = 0;
        ibit = 1;
        while (j < vprptCur->cFields) {
            if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0 && (j == 0 || j >= vprptCur->cFieldFirst)) {
                dx = vprptCur->rgbdx[j] * 2;
                if (yRow >= prc->top - (dyArial8 + 4) && yRow <= prc->bottom) {
                    SelectObject(hdc, hbrButtonShadow);
                    PatBlt(hdc, xCol + dx - 1, yRow, 1, dyArial8 + 4, PATCOPY);
                    PatBlt(hdc, xCol, dyArial8 + 4 + yRow, dx, 1, PATCOPY);
                    SetRect(&rc, xCol + 2, yRow + 2, xCol + dx - 3, dyArial8 + 4 + yRow - 1);
                    if (gd.fRptSafeDraw != 0x0) {
                        FillRect(hdc, &rc, hbrButtonFace);
                    }
                    DrawReportItem(hdc, &rc, vprptCur->irpt, i, j);
                }
                xCol = xCol + dx;
            }
            j = j + 1;
            ibit = ibit * 2;
        }
    }
    return;
}

INT_PTR CALLBACK ScoreXDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int16_t     i;
    RECT        rc;
    HDC         hdc;
    PAINTSTRUCT ps;
    POINT16     pt;
    char        szT[40];
    int16_t     cchHistory;
    char       *rgszScan[20];
    int16_t     c;
    int32_t     rgid[12];
    char       *psz;
    int16_t     iSel;
    int16_t     cch;
    POINT       t_pt_1082;
    POINT       t_pt_1091_1;
    int16_t     t_11c6;

    switch (message) {
    case WM_INITDIALOG:
        InitScoreDlg(hwnd, gd.fScoreVictory);
        fInScoreDialog = 1;
        StickyDlgPos(hwnd, &ptStickyScoreXDlg, 1);
        hwndScoreXDlg = hwnd;
        if (gd.fTutorial != 0x0) {
            AdvanceTutor();
        }
        return 1;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (gd.fScoreVictory != 0x2) {
            if (gd.fScoreVictory == 0x0) {
                DrawScoreReport(hdc);
            } else {
                DrawVCReport(hdc);
            }
        } else {
            DrawHistoryReport(hdc);
        }
        EndPaint(hwnd, &ps);
        return 1;
    case WM_SETCURSOR:
        if (gd.fScoreVictory != 0x2) {
            return 0;
        }
        GetCursorPos(&t_pt_1082);
        pt = PointTo16(t_pt_1082);
        t_pt_1091_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_1091_1);
        pt = PointTo16(t_pt_1091_1);
        if (pt.y >= dyArial10 + dyArial8 - 2 || pt.y <= 2) {
            return 0;
        }
        SetCursor(hcurHand);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        c = 0;
        if (gd.fScoreVictory != 0x2 || HIWORD(lParam) <= 0x2 || HIWORD(lParam) >= (uint16_t)(dyArial10 + dyArial8 - 2)) {
            return 0;
        }
        cchHistory = CchGetString(idsHistory, szT);
        for (i = 0; i < 8; i++) {
            strcpy(&szWork[i * 40], szT);
            psz = &szWork[i * 40 + cchHistory];
            cch = CchGetString(i + 435, psz);
            psz[cch - 1] = 0;
            rgid[c] = (uint32_t)(gd.iCurGraph == i ? 0x1 : 0x0);
            t_11c6 = c;
            c = c + 1;
            rgszScan[t_11c6] = &szWork[i * 40];
        }
        iSel = PopupMenu(hwnd, LOWORD(lParam), HIWORD(lParam), c, rgid, rgszScan, -2, 0);
        if (iSel == -1) {
            return 0;
        }
        gd.iCurGraph = iSel;
        gd.fChgReports = 0x1;
        InvalidateRect(hwnd, 0x0, 1);
        return 0;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case IDCANCEL:
            StickyDlgPos(hwnd, &ptStickyScoreXDlg, 0);
            EndDialog(hwnd, i);
            fInScoreDialog = 0;
            hwndScoreXDlg = 0x0;
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, 0x1, 0x455);
            return 1;
        case IDC_U16_0x00C6:
            gd.fScoreVictory = (uint32_t)(gd.fScoreVictory + 0x1) % 0x3;
            InvalidateRect(hwnd, 0x0, 1);
            InitScoreDlg(hwnd, gd.fScoreVictory);
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
        default:
        }
    default:
        return 0;
    }
}

void InitScoreDlg(HWND hwnd, int16_t fVictory) {
    HDC      hdc;
    int16_t  dxDig;
    int16_t  dy;
    int16_t  dyFrame;
    int16_t  dxFrame;
    RECT     rcWindow;
    char    *psz;
    int16_t  dx;
    RECT     rc;
    uint16_t t_scratch_m22_2;

    hdc = GetDC(hwnd);
    SelectObject(hdc, rghfontArial8[1]);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 1));
    if (fVictory != 2) {
        if (fVictory == 0) {
            psz = PszGetCompressedString(idsUnarmedShips2);
            vdxScoreX = LOWORD(GetTextExtent(hdc, psz, strlen(psz))) + 8;
            dx = (4 <= game.cPlayer ? game.cPlayer : 0x4) * dxDig * 0x5 + vdxScoreX + 8;
            dy = (int32_t)(11 * dyArial8 * 0x3) / 2 + 88;
        } else {
            psz = PszGetCompressedString(idsExceedsSecondPlaceScore);
            vdxScoreX = (uint32_t)(LOWORD(GetTextExtent(hdc, psz, strlen(psz))) * 0x3) / 2 + 6 * dxDig;
            dx = (int32_t)((4 <= game.cPlayer ? game.cPlayer : 0x4) * dyArial8 * 0x3) / 2 + vdxScoreX + 8;
            dy = (int32_t)(11 * dyArial8 * 0x3) / 2 + 88;
        }
    } else {
        dx = 600;
        dy = 400;
    }
    ReleaseDC(hwnd, hdc);
    GetWindowRect(hwnd, &rcWindow);
    GetClientRect(hwnd, &rc);
    dxFrame = rcWindow.right - rcWindow.left - rc.right;
    dyFrame = rcWindow.bottom - rcWindow.top - rc.bottom;
    SetWindowPos(hwnd, 0x0, 0, 0, dxFrame + dx, dyFrame + dy, SWP_NOMOVE | SWP_NOZORDER);
    GetWindowRect(GetDlgItem(hwnd, IDCANCEL), &rc);
    MapWindowPoints(0x0, hwnd, (POINT *)&rc, 0x2);
    OffsetRect(&rc, 0, dy - 4 - rc.bottom);
    dx = (int32_t)(dx - (rc.right - rc.left) * 3) / 4;
    SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x00C6), 0x0, dx, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    SetWindowPos(GetDlgItem(hwnd, IDCANCEL), 0x0, dx * 2 + (rc.right - rc.left), rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    t_scratch_m22_2 = (rc.right - rc.left) * 2;
    SetWindowPos(GetDlgItem(hwnd, IDC_HELP), 0x0, 3 * dx + t_scratch_m22_2, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    SetWindowText(hwnd, PszGetCompressedString(fVictory + 1210));
    return;
}

void DrawVCReport(HDC hdc) {
    int16_t  grbitVC;
    int16_t  xStart;
    int16_t  dxDig;
    int16_t  yTop;
    POINT16  pt;
    int16_t  cCurSav;
    StringId ids;
    COLORREF cr;
    int16_t  cCur;
    HDC      hdcMem;
    int16_t  j;
    int16_t  i;
    int16_t  iPass;
    char    *psz;
    HBITMAP  hbmpSav;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    int16_t  idsT;
    int16_t  vcVal;
    StringId t_1bfe;
    COLORREF t_merge_1c4c_0001;

    yTop = 88;
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[4]);
    xLeft = vdxScoreX;
    for (i = 0; i < game.cPlayer; i++) {
        psz = PszPlayerName(i, 1, 1, 1, 0, 0x0);
        cch = strlen(psz);
        l = GetTextExtent(hdc, psz, cch);
        dxDig = LOWORD((int32_t)((double)(uint32_t)LOWORD(l) / 1.4142));
        if (rgplr[i].fInclude == 0x0 || rgplr[i].fDead == 0x0) {
            if (vlprgScoreX[i].fWinner == 0x0) {
                cr = 0x0;
            } else {
                cr = 0xff0000;
            }
        } else {
            cr = 0x7f7f7f;
        }
        SetTextColor(hdc, cr);
        TextOut(hdc, xLeft - dxDig + (int32_t)(3 * dyArial8) / 2, yTop - dxDig - (int32_t)dyArial8 / 2 - 4, psz, cch);
        xLeft = xLeft + (int32_t)(3 * dyArial8) / 2;
    }
    SelectObject(hdc, rghfontArial8[1]);
    SelectObject(hdc, hbrButtonShadow);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 1));
    ids = idsOwns;
    cCur = 0;
    hdcMem = CreateCompatibleDC(hdc);
    hbmpSav = SelectObject(hdcMem, hbmpMono);
    SetTextColor(hdc, 0x0);
    SetBkColor(hdc, 0xffffff);
    xLeft = vdxScoreX + 4;
    pt.x = (int32_t)((int32_t)(3 * dyArial8) / 0x2 - 0xc) / 2;
    pt.y = (int32_t)((int32_t)(3 * dyArial8) / 0x2 - 0xb) / 2 - (int32_t)dyArial8 / 4;
    for (i = 0; i < game.cPlayer; i++) {
        grbitVC = vlprgScoreX[i].grbitVC;
        j = 0;
        while (j < 7) {
            if ((grbitVC & 0x1) != 0x0) {
                BitBlt(hdc, xLeft + pt.x, yTop + pt.y + (int32_t)(j * dyArial8 * 0x3) / 2, 14, 12, hdcMem, 0, 0, SRCAND);
                if ((rgplr[i].fInclude == 0x0 || rgplr[i].fDead == 0x0) && GetVCCheck(&game, (j <= 1 ? 0 : 1) + j) != 0) {
                    if (vlprgScoreX[i].fWinner == 0x0)
                        goto L_1917;
                    SetTextColor(hdc, 0xff0000);
                } else {
                    SetTextColor(hdc, crButtonShadow);
                }
                SetBkColor(hdc, 0x0);
                BitBlt(hdc, xLeft + pt.x, yTop + pt.y + (int32_t)(j * dyArial8 * 0x3) / 2, 14, 12, hdcMem, 0, 0, SRCPAINT);
                SetTextColor(hdc, 0x0);
                SetBkColor(hdc, 0xffffff);
            }
        L_1917:
            j = j + 1;
            grbitVC = grbitVC >> 0x1;
        }
        PatBlt(hdc, xLeft, yTop - (int32_t)dyArial8 / 4, 1, (int32_t)(7 * dyArial8 * 0x3) / 2 + 1, PATCOPY);
        xLeft = xLeft + (int32_t)(3 * dyArial8) / 2;
    }
    PatBlt(hdc, xLeft, yTop - (int32_t)dyArial8 / 4, 1, (int32_t)(7 * dyArial8 * 0x3) / 2 + 1, PATCOPY);
    SelectObject(hdcMem, hbmpSav);
    DeleteDC(hdcMem);
    for (i = 0; i < 9; i++) {
        for (iPass = 0; iPass < 2; iPass++) {
            if (iPass != 0) {
                ids = ids - 3;
                cCur = cCurSav;
                if (i >= 7) {
                    xStart = 8;
                } else {
                    xStart = vdxScoreX - xLeft;
                }
            } else {
                cCurSav = cCur;
                xStart = 0;
            }
            xLeft = xStart;
            if (i <= 7 && iPass == 1) {
                PatBlt(hdc, vdxScoreX + 4, yTop - (int32_t)dyArial8 / 4, (int32_t)(dyArial8 * game.cPlayer * 0x3) / 2, 1, PATCOPY);
                if (i == 7) {
                    yTop = yTop + (int32_t)dyArial8 / 2;
                }
            }
            t_1bfe = ids;
            ids = ids + 1;
            cch = CchGetString(ids, szWork);
            if (iPass == 1) {
                if (i < 7 && GetVCCheck(&game, cCur) == 0) {
                    t_merge_1c4c_0001 = 0x7f7f7f;
                } else {
                    t_merge_1c4c_0001 = 0x0;
                }
                SetTextColor(hdc, t_merge_1c4c_0001);
                TextOut(hdc, xLeft, yTop, szWork, cch);
            }
            xLeft = xLeft + LOWORD(GetTextExtent(hdc, szWork, cch));
            j = 0;
            while (1) {
                if (j >= 2)
                    goto L_1dd3;
                if (j == 1 && i != 1)
                    break;
                vcVal = GetVCVal(&game, cCur, 0);
                if (i != 0) {
                    idsT = ids;
                } else {
                    idsT = 965;
                    vcVal = LOWORD((int32_t)((int32_t)((int32_t)vcVal * (int32_t)game.cPlanMax) / 0x64));
                }
                cch = _wsprintf(szWork, PCTD, vcVal);
                if (i == 3) {
                    strcat(szWork, "%");
                    cch = cch + 1;
                }
                if (iPass == 1) {
                    TextOut(hdc, xLeft, yTop, szWork, cch);
                }
                xLeft = xLeft + LOWORD(GetTextExtent(hdc, szWork, cch));
                if (i != 2 && i != 3) {
                    xLeft = xLeft + 4;
                }
                cch = CchGetString(idsT, szWork);
                if (iPass == 1) {
                    TextOut(hdc, xLeft, yTop, szWork, cch);
                }
                xLeft = xLeft + LOWORD(GetTextExtent(hdc, szWork, cch));
                cCur = cCur + 1;
                ids = ids + 1;
                j = j + 1;
            }
            ids = ids + 1;
        L_1dd3:
            if (iPass == 1) {
                yTop = yTop + (int32_t)(3 * dyArial8) / 2;
            }
        }
    }
    return;
}

void DrawScoreReport(HDC hdc) {
    int16_t  dxDig;
    int16_t  yTop;
    POINT16  pt;
    int16_t  dx45;
    StringId ids;
    COLORREF cr;
    int32_t  lMax;
    int16_t  j;
    int16_t  i;
    int16_t  iPass;
    char    *psz;
    int32_t  lVal;
    int16_t  cch;
    int16_t  xLeft;
    int32_t  l;
    StringId t_20ef;
    COLORREF t_merge_23b0_0001;
    COLORREF t_merge_23e2_0001;

    yTop = 88;
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, rghfontArial8[1]);
    dxDig = LOWORD(GetTextExtent(hdc, "9", 1));
    SelectObject(hdc, rghfontArial8[4]);
    xLeft = (int32_t)(5 * dxDig - (int32_t)(3 * dyArial8) / 0x2) / 2 + vdxScoreX;
    for (i = 0; i < game.cPlayer; i++) {
        psz = PszPlayerName(i, 1, 1, 1, 0, 0x0);
        cch = strlen(psz);
        l = GetTextExtent(hdc, psz, cch);
        dx45 = LOWORD((int32_t)((double)(uint32_t)LOWORD(l) / 1.4142));
        if (rgplr[i].fInclude == 0x0 || rgplr[i].fDead == 0x0) {
            if (vlprgScoreX[i].fWinner == 0x0) {
                cr = 0x0;
            } else {
                cr = 0xff0000;
            }
        } else {
            cr = 0x7f7f7f;
        }
        SetTextColor(hdc, cr);
        TextOut(hdc, xLeft - dx45 + (int32_t)(3 * dyArial8) / 2, yTop - dx45 - (int32_t)dyArial8 / 2 - 4, psz, cch);
        xLeft = xLeft + 5 * dxDig;
    }
    SelectObject(hdc, rghfontArial8[1]);
    SelectObject(hdc, hbrButtonShadow);
    ids = idsPlanets;
    xLeft = vdxScoreX + 4;
    pt.x = (int32_t)((int32_t)(3 * dyArial8) / 0x2 - 0xc) / 2;
    pt.y = (int32_t)((int32_t)(3 * dyArial8) / 0x2 - 0xb) / 2 - (int32_t)dyArial8 / 4;
    for (i = 0; i <= game.cPlayer; i++) {
        PatBlt(hdc, xLeft, yTop - (int32_t)dyArial8 / 4, 1, (int32_t)(9 * dyArial8 * 0x3) / 2 + 1, PATCOPY);
        xLeft = xLeft + 5 * dxDig;
    }
    SetTextColor(hdc, 0x0);
    for (i = 0; i < 9; i++) {
        PatBlt(hdc, vdxScoreX + 4, yTop - (int32_t)dyArial8 / 4, game.cPlayer * dxDig * 5, 1, PATCOPY);
        t_20ef = ids;
        ids = ids + 1;
        cch = CchGetString(ids, szWork);
        SetTextColor(hdc, 0x0);
        RightTextOut(hdc, vdxScoreX, yTop, szWork, cch, 0);
        xLeft = 5 * dxDig + vdxScoreX + 2;
        lMax = 0;
        for (iPass = 0; iPass < 2; iPass++) {
            for (j = 0; j < game.cPlayer; j++) {
                if (vlprgScoreX[j].fValid == 0x0) {
                    lVal = -1;
                } else if ((uint16_t)i <= 8) {
                    switch (i) {
                    case 0:
                        lVal = (int32_t)vlprgScoreX[j].score.cPlanet;
                        break;
                    case 1:
                        lVal = (int32_t)vlprgScoreX[j].score.cStarbase;
                        break;
                    case 2:
                    case 3:
                    case 4:
                        lVal = (int32_t)((uint32_t)(vlprgScoreX[j].score.rgcsh[i - 0x2] & 0x1fff) << (vlprgScoreX[j].score.rgcsh[i - 0x2] >> 0xd << 0x1));
                        break;
                    case 5:
                        lVal = (int32_t)vlprgScoreX[j].score.cTechLevels;
                        break;
                    case 6:
                        lVal = vlprgScoreX[j].score.cResources;
                        break;
                    case 7:
                        lVal = vlprgScoreX[j].score.lScore;
                        break;
                    case 8:
                        lVal = (int32_t)vlprgScoreX[j].turn;
                    }
                }
                if (iPass != 0) {
                    if (lVal >= 0 && (rgplr[j].fInclude == 0x0 || rgplr[j].fDead == 0x0)) {
                        if (i != 8) {
                            t_merge_23e2_0001 = lVal == lMax ? 0xff0000 : 0x0;
                            SetTextColor(hdc, t_merge_23e2_0001);
                        } else {
                            t_merge_23b0_0001 = lVal == 1 ? 0xff0000 : 0x0;
                            SetTextColor(hdc, t_merge_23b0_0001);
                        }
                        psz = PszFromLongK(lVal, &cch);
                        RightTextOut(hdc, xLeft, yTop, psz, cch, 0);
                    }
                } else if (lVal > lMax) {
                    lMax = lVal;
                }
                if (iPass == 1) {
                    xLeft = xLeft + 5 * dxDig;
                }
            }
        }
        yTop = yTop + (int32_t)(3 * dyArial8) / 2;
    }
    PatBlt(hdc, vdxScoreX + 4, yTop - (int32_t)dyArial8 / 4, game.cPlayer * dxDig * 5, 1, PATCOPY);
    return;
}

void DrawHistoryReport(HDC hdc) {
    char     szT[100];
    RECT     rcChart;
    uint16_t dYear;
    POINT16  pt;
    int16_t  dy;
    int32_t  cYears;
    int32_t  cCur;
    uint16_t iYearBase;
    int16_t  j;
    int16_t  i;
    int16_t  yCur;
    int16_t  cDrawn;
    char    *psz;
    int16_t  dx;
    int32_t  cScaleMax;
    int16_t  xCur;
    int32_t  cInc;
    int16_t  cch;
    RECT     rcDiamond;
    RECT     rc;
    HPEN     hpenSav;
    HPEN     hpen;
    SCOREX  *lpsx;

    SetBkMode(hdc, TRANSPARENT);
    GetClientRect(hwndScoreXDlg, &rcChart);
    ExpandRc(&rcChart, -48, 0);
    rcChart.top = dyArial10 + dyArial8;
    rcChart.bottom = rcChart.bottom - dyArial8 * 4;
    rc = rcChart;
    rc.top = (int32_t)dyArial8 / 2;
    rc.bottom = rc.top + dyArial10;
    cch = CchGetString(idsHistory, szT);
    psz = &szT[cch];
    cch = CchGetString(gd.iCurGraph + 0x1b3, psz);
    psz[cch - 1] = 0;
    SelectObject(hdc, rghfontArial10[1]);
    SetTextColor(hdc, crButtonText);
    RcCtrTextOut(hdc, &rc, szT, 0);
    SetRect(&rcDiamond, 15, rc.top, dyArial10 + 15, rc.top + dyArial10);
    DrawDiamond(hdc, &rcDiamond, hbrBBlue);
    dx = rcChart.right - rcChart.left;
    dy = rcChart.bottom - rcChart.top;
    PatBlt(hdc, rcChart.left, rcChart.top, dx, dy, BLACKNESS);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rcChart.left - 1, rcChart.bottom, dx + 3, 1, PATCOPY);
    PatBlt(hdc, rcChart.left - 2, rcChart.bottom + 1, dx + 4, 1, PATCOPY);
    PatBlt(hdc, rcChart.right, rcChart.top - 1, 1, dy + 3, PATCOPY);
    PatBlt(hdc, rcChart.right + 1, rcChart.top - 2, 1, dy + 4, PATCOPY);
    SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, rcChart.left - 2, rcChart.top - 2, dx + 3, 1, PATCOPY);
    PatBlt(hdc, rcChart.left - 2, rcChart.top - 1, dx + 2, 1, PATCOPY);
    PatBlt(hdc, rcChart.left - 2, rcChart.top - 2, 1, dy + 3, PATCOPY);
    PatBlt(hdc, rcChart.left - 1, rcChart.top - 2, 1, dy + 2, PATCOPY);
    dx = dx - 4;
    dy = dy - 4;
    ExpandRc(&rcChart, -2, -2);
    if (game.turn > 0x64) {
        iYearBase = game.turn - 0x64;
    } else {
        iYearBase = 0x0;
    }
    cYears = (uint32_t)((int32_t)((int32_t)(game.turn + 0x4) / 0x5) * 5);
    if (cYears <= 100) {
        if (cYears > 50) {
            cYears = (uint32_t)((int32_t)((cYears + 5) / 0xa) * 10);
        }
    } else {
        cYears = 100;
    }
    xCur = rcChart.left;
    SelectObject(hdc, rghfontArial8[1]);
    if (cYears <= 50) {
        j = 5;
    } else {
        j = 10;
    }
    cDrawn = (int32_t)LOWORD(cYears) / j;
    if (cDrawn > 0) {
        for (i = 0; i <= cDrawn; i++) {
            xCur = (int32_t)(dx * i) / cDrawn + rcChart.left;
            cch = _wsprintf(szT, PCTD, iYearBase + 0x960 + i * j);
            CtrTextOut(hdc, xCur, rcChart.bottom + 6, szT, cch);
            if (i > 0 && i < cDrawn) {
                PatBlt(hdc, xCur, rcChart.top - 2, 1, dy + 4, PATCOPY);
            }
        }
    }
    cScaleMax = -1;
    for (i = 0; i < game.cPlayer; i++) {
        if (rgsxPlr[i] != 0x0) {
            for (j = 0; j < rgcsxPlr[i]; j++) {
                cCur = LFetchScoreXVal(rgsxPlr[i] + j, gd.iCurGraph);
                if (cCur > cScaleMax) {
                    cScaleMax = cCur;
                }
            }
        }
    }
    if (cScaleMax >= 0) {
        if (cScaleMax < 5) {
            cScaleMax = 5;
        }
        if (cScaleMax <= 12) {
            cInc = 1;
        } else if (cScaleMax <= 25) {
            cInc = 2;
        } else if (cScaleMax <= 60) {
            cInc = 5;
        } else if (cScaleMax <= 120) {
            cInc = 10;
        } else if (cScaleMax <= 300) {
            cInc = 25;
        } else if (cScaleMax <= 600) {
            cInc = 50;
        } else if (cScaleMax <= 1200) {
            cInc = 100;
        } else if (cScaleMax <= 6000) {
            cInc = 500;
        } else if (cScaleMax <= 12000) {
            cInc = 1000;
        } else {
            cInc = (uint32_t)((int32_t)((int32_t)(cScaleMax / 12) / 500) * 500);
        }
        xCur = rcChart.left - 6;
        for (cCur = cInc; cCur < cScaleMax; cCur = cCur + cInc) {
            yCur = rcChart.bottom - LOWORD((int32_t)((int32_t)((int32_t)dy * cCur) / cScaleMax));
            if (yCur < (int32_t)dyArial8 / 2 + rcChart.top)
                break;
            cch = _wsprintf(szWork, PCTLD, cCur);
            RightTextOut(hdc, xCur, yCur - (int32_t)dyArial8 / 2, szWork, cch, 0);
            PatBlt(hdc, rcChart.left - 2, yCur, dx + 4, 1, PATCOPY);
        }
        xCur = rcChart.left + 6;
        yCur = rcChart.top + 6;
        for (i = 0; i < game.cPlayer; i++) {
            if (rgsxPlr[i] != 0x0) {
                psz = PszPlayerName(i, 1, 1, 0, 0, 0x0);
                SetTextColor(hdc, i == idPlayer ? 0xffffff : rgcrPlrHistory[i]);
                TextOut(hdc, xCur, yCur, psz, strlen(psz));
                yCur = yCur + dyArial8;
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgsxPlr[i] != 0x0) {
                cDrawn = 0;
                hpen = CreatePen(0, 1, i == idPlayer ? 0xffffff : rgcrPlrHistory[i]);
                hpenSav = SelectObject(hdc, hpen);
                for (j = 0; j < rgcsxPlr[i]; j++) {
                    lpsx = rgsxPlr[i] + j;
                    if (lpsx->turn >= iYearBase && lpsx->turn <= LOWORD(cYears) + iYearBase) {
                        dYear = lpsx->turn - iYearBase;
                        cCur = LFetchScoreXVal(lpsx, gd.iCurGraph);
                        pt.x = LOWORD((int32_t)((int32_t)((uint32_t)dYear * (int32_t)dx) / cYears)) + rcChart.left;
                        pt.y = rcChart.bottom - LOWORD((int32_t)((int32_t)(cCur * (int32_t)dy) / cScaleMax));
                        if (cDrawn != 0) {
                            LineTo(hdc, pt.x, pt.y);
                        } else {
                            MoveTo(hdc, pt.x, pt.y);
                        }
                        cDrawn = cDrawn + 1;
                    }
                }
                if (cDrawn == 1) {
                    SetPixel(hdc, pt.x, pt.y, i == idPlayer ? 0xffffff : rgcrPlrHistory[i]);
                }
                SelectObject(hdc, hpenSav);
                DeleteObject(hpen);
            }
        }
    }
    return;
}

int32_t LFetchScoreXVal(SCOREX *lpsx, int16_t iVal) {
    if ((uint16_t)iVal > 7) {
        return 0;
    }
    switch (iVal) {
    case 0:
        return (int32_t)lpsx->score.cPlanet;
    case 1:
        return (int32_t)lpsx->score.cStarbase;
    case 2:
    case 3:
    case 4:
        return (int32_t)((uint32_t)(lpsx->score.rgcsh[iVal - 2] & 0x1fff) << (lpsx->score.rgcsh[iVal - 2] >> 0xd << 0x1));
    case 5:
        return (int32_t)lpsx->score.cTechLevels;
    case 6:
        return lpsx->score.cResources;
    case 7:
        return lpsx->score.lScore;
    }
}

int16_t DxReportColHdr(int16_t irpt, int16_t iCol, char *psz, HDC hdc) {
    char     szT[40];
    StringId ids;
    int16_t  dxDigit;
    int16_t  dx;
    int16_t  cch;
    int16_t  dx2;

    szT[0] = '8';
    dxDigit = LOWORD(GetTextExtent(hdc, szT, 1));
    switch (irpt) {
    default:
        *psz = 0;
        return 0;
    case 0:
        cch = CchGetString(iCol + 1113, psz);
        dx = LOWORD(GetTextExtent(hdc, psz, cch));
        if ((uint16_t)iCol > 14)
            break;
        switch (iCol) {
        case 1:
        case 13:
        case 14:
            dx2 = 15 * dxDigit;
            goto DxChk;
        case 0:
            dx = dx * 2;
            break;
        case 5:
            dx = 3 * dx + 20;
            break;
        case 4:
            ids = idsN100100;
            goto ChkAltString;
        case 12:
            ids = idsN10001000;
            goto ChkAltString;
        case 6:
        case 7:
            dx2 = 5 * dxDigit;
            goto DxChk;
        case 10:
        case 11:
            dx2 = 11 * dxDigit;
            goto DxChk;
        case 9:
            dx2 = 14 * dxDigit;
            goto DxChk;
        case 3:
            dx2 = dxDigit * 4 + 2;
            goto DxChk;
        case 2:
        case 8:
        }
        break;
    ChkAltString:
        cch = CchGetString(ids, szT);
        dx2 = LOWORD(GetTextExtent(hdc, szT, cch));
    DxChk:
        if (dx2 <= dx)
            break;
        dx = dx2;
        break;
    case 1:
        cch = CchGetString(iCol + 1138, psz);
        dx = LOWORD(GetTextExtent(hdc, psz, cch));
        if ((uint16_t)iCol > 11)
            break;
        switch (iCol) {
        case 2:
            dx = dx * 2 + (int32_t)dx / 2;
            break;
        case 0:
        case 1:
        case 3:
        case 8:
        case 10:
        case 11:
            dx = dx * 2;
            break;
        case 4:
            dx = dx * 2 - dxDigit;
            break;
        case 5:
            dx = dx * 4 + (int32_t)dx / 2;
            break;
        case 6:
            dx2 = 5 * dxDigit;
            if (dx2 <= dx)
                break;
            dx = dx2;
            break;
        case 7:
            dx2 = 19 * dxDigit;
            if (dx2 > dx) {
                dx = dx2;
            }
        case 9:
        }
        break;
    case 2:
        cch = CchGetString(iCol + 1150, psz);
        dx = LOWORD(GetTextExtent(hdc, psz, cch));
        switch (iCol) {
        case 2:
            dx = dx * 2 + (int32_t)dx / 2;
            break;
        case 0:
            dx = 3 * dx;
            break;
        case 1:
        case 5:
            dx = dx * 2;
        default:
        }
        break;
    case 3:
        cch = CchGetString(iCol + 1162, psz);
        dx = LOWORD(GetTextExtent(hdc, psz, cch));
        if (iCol == 0) {
            dx = dx * 2 + (int32_t)dx / 2;
        }
    }
    dx = dx + 5;
    return (int32_t)(dx + 1) / 2 * 0x2;
}

void DrawReportItem(HDC hdc, RECT *prc, int16_t irpt, int16_t irow, int16_t icol) {
    BTLDATA *lpbd;
    char     szT[100];
    char     chT;
    char    *lpsz;
    PLANET  *lppl;
    int16_t  j;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  dx;
    char    *psz;
    int16_t  xCur;
    int16_t  cch;
    int32_t  l;
    HBRUSH   hbr;
    RECT     rc;
    float    pct;
    int32_t  rgl[4];
    int16_t  iItem;
    PLANET   pl;
    int16_t  fEnough;
    int16_t  t_call_3d42;
    int16_t  t_merge_422c_0001;
    uint16_t t_scratch_m8a;
    uint16_t t_scratch_m8a_2;
    uint16_t t_scratch_m8a_3;
    uint16_t t_scratch_m8a_4;

    szT[0] = '8';
    dx = LOWORD(GetTextExtent(hdc, szT, 1));
    switch (irpt) {
    case 0:
        lppl = lpPlanets + vlprgidPlanet[irow];
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, 0x0);
        SetBkMode(hdc, TRANSPARENT);
        if ((uint16_t)icol > 14)
            break;
        switch (icol) {
        case 1:
            if (lppl->fStarbase != 0x0) {
                lpsz = rglpshdefSB[idPlayer][lppl->isb].hul.szClass;
            } else {
                lpsz = szDblDash;
            }
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, lpsz, fstrlen(lpsz), 0x0);
            break;
        case 14:
            if (lppl->idRoute != 0x0) {
                psz = PszGetPlanetName(lppl->idRoute - 1);
            } else {
                psz = szDblDash;
            }
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 13:
            if (lppl->idFling != 0x0) {
                psz = PszGetPlanetName(lppl->idFling - 1);
            } else {
                psz = szDblDash;
            }
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 0:
            if (sel.grobj == grobjPlanet && sel.pl.id == lppl->id) {
                SetTextColor(hdc, 0x7f);
            }
            psz = PszGetPlanetName(lppl->id);
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            if (lppl->fStarbase == 0x0)
                break;
            rc = *prc;
            dx = (int32_t)(rc.bottom - rc.top) / 3 - 1;
            rc.left = rc.right - dx;
            if (LphuldefFromId(rglpshdefSB[idPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax == 0x0) {
                hbr = hbrBlue;
            } else {
                hbr = hbrYellow;
            }
            rc.top = prc->top;
            rc.bottom = rc.top + dx;
            FillRect(hdc, &rc, hbr);
            if (IWarpMAFromLppl(lppl, 0x0) > 0) {
                rc.top = prc->top + dx + 1;
                rc.bottom = rc.top + dx;
                FillRect(hdc, &rc, hbrPurple);
            }
            if (IStargateFromLppl(lppl) == -1)
                break;
            rc.top = (dx + 1) * 2 + prc->top;
            rc.bottom = rc.top + dx;
            FillRect(hdc, &rc, hbrGreen);
            break;
        case 2:
            if (CalcPlanetMaxPop(lppl->id, lppl->iPlayer) < lppl->rgwtMin[3]) {
                SetTextColor(hdc, 0xff);
            }
            cch = CommaFormatLong(szT, (uint32_t)(lppl->rgwtMin[3] * 100));
            RightTextOut(hdc, prc->right, prc->top, szT, cch, 0);
            break;
        case 3:
            cch = _wsprintf(szT, PCTDPCTPCT, PctPlanetCapacity(lppl));
            RightTextOut(hdc, prc->right, prc->top, szT, cch, 0);
            break;
        case 8:
            i = lppl->cDefenses;
            j = CMaxOperableDefenses(lppl, idPlayer, 0);
            if (i <= 0) {
                szT[2] = '-';
                szT[1] = '-';
                szT[0] = '-';
                cch = 3;
                goto DrawPlusDef;
            }
            CalcPctSurvive(lppl, &pct, 0x0);
            pct = 1.0 - pct;
            cch = _wsprintf(szT, PCTDXPCTDPCTPCT, LOWORD((int32_t)(pct * 100.0)),
                            LOWORD((int32_t)((pct - (double)(int32_t)LOWORD((int32_t)(pct * 100.0)) / 100.0) * 10000.0)));
            goto DrawPlusDef;
        case 7:
            i = lppl->cFactories;
            j = CMaxOperableFactories(lppl, idPlayer, 0);
            goto DrawMineFact;
        case 6:
            i = lppl->cMines;
            j = CMaxOperableMines(lppl, idPlayer, 0);
            goto DrawMineFact;
        case 4:
            cch = CchGetString(idsN100, szT);
            dx = LOWORD(GetTextExtent(hdc, szT, cch));
            i = PctPlanetDesirability(lppl, idPlayer);
            cch = _wsprintf(szT, PCTDPCTPCT, i);
            if (i <= 10) {
                SetTextColor(hdc, i >= 0 ? 0x7f7f : 0xff);
            }
            RightTextOut(hdc, prc->left + dx, prc->top, szT, cch, 0);
            j = PctPlanetOptValue(lppl, idPlayer);
            if (j <= i)
                break;
            if (j > 0) {
                SetTextColor(hdc, j > 10 ? 0x0 : 0x7f7f);
            }
            cch = _wsprintf(szT, "(%d%%)", j);
            RightTextOut(hdc, prc->right, prc->top, szT, cch, 0);
            break;
        case 9:
        case 10:
            xCur = dx * 4 + prc->left;
            if (icol == 10) {
                xCur = xCur - dx;
                EstMineralsMined(lppl, rgl, -1, 0);
            }
            for (i = 0; i < 3; i++) {
                if (icol != 9) {
                    l = rgl[i];
                } else {
                    l = lppl->rgwtMin[i];
                }
                DrawMineralItem(hdc, xCur, prc->top, i, l);
                if (icol != 10) {
                    xCur = xCur + 5 * dx;
                } else {
                    xCur = xCur + dx * 4;
                }
            }
            break;
        case 11:
            xCur = 3 * dx + prc->left;
            for (i = 0; i < 3; i++) {
                iItem = lppl->rgMinConc[i];
                DrawMineralItem(hdc, xCur, prc->top, i, (int32_t)iItem);
                xCur = xCur + dx * 4;
            }
            break;
        case 5:
            pl = sel.pl;
            sel.pl = *lppl;
            FillPlanetProdLB(0x0, 0x0, lppl);
            sel.pl = pl;
            DrawProductionItem(hdc, prc, szWork, 0, 0, 2);
            break;
        case 12:
            cch = CchGetString(idsN1000, szT);
            dx = LOWORD(GetTextExtent(hdc, szT, cch));
            t_call_3d42 = CResourcesAtPlanet(lppl, idPlayer);
            j = t_call_3d42;
            i = t_call_3d42;
            if (lppl->fNoResearch == 0x0) {
                i = i - MulDiv(i, (int16_t)rgplr[idPlayer].pctResearch, 100);
            }
            CchGetString(idsD4, szT);
            cch = _wsprintf(szWork, szT, i);
            RightTextOut(hdc, prc->left + dx, prc->top, szWork, cch, 0);
            cch = _wsprintf(szT, PCTD, j);
            RightTextOut(hdc, prc->right, prc->top, szT, cch, 0);
        }
        break;
    DrawMineFact:
        cch = CommaFormatLong(szT, (int32_t)i);
    DrawPlusDef:
        if (i >= j) {
            SetTextColor(hdc, i == j ? 0x7f00 : 0xff);
        }
        RightTextOut(hdc, prc->right, prc->top, szT, cch, 0);
        break;
    case 1:
        lpfl = rglpfl[vlprgidFleet[irow]];
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, 0x0);
        SetBkMode(hdc, TRANSPARENT);
        if ((uint16_t)icol > 11)
            break;
        switch (icol) {
        case 10:
            i = lpfl->iplan;
            fstrcpy(szT, rglpbtlplan[lpfl->iplr][lpfl->iplan].szName);
            psz = szT;
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 0:
            if (sel.grobj == grobjFleet && sel.fl.id == lpfl->id) {
                SetTextColor(hdc, 0x7f);
            }
            psz = PszGetFleetName(lpfl->id);
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 2:
            if (lpfl->idPlanet == -1) {
                psz = szT;
                _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpfl->pt.x, lpfl->pt.y);
            } else {
                psz = PszGetPlanetName(lpfl->idPlanet);
            }
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 3:
            psz = PszGetDestName(lpfl, hdc);
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 5:
            psz = PszGetTaskName(lpfl, &i);
            if (i != -1) {
                SetTextColor(hdc, rgcrMinerals[i]);
            }
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            break;
        case 4:
            psz = PszGetETA(hdc, lpfl, 0x0);
            RightTextOut(hdc, prc->right, prc->top, psz, strlen(psz), 0);
            break;
        case 7:
            xCur = dx * 4 + prc->left;
            for (i = 0; i <= 3; i++) {
                DrawMineralItem(hdc, xCur, prc->top, i, lpfl->rgwtMin[i]);
                xCur = xCur + 5 * dx;
            }
            break;
        case 6:
            if (lpfl->cord > 1 && lpfl->rgwtMin[4] < EstFuelUse(lpfl, 0, lpfl->lpplord->rgord[1].iWarp, -1, 0)) {
                t_merge_422c_0001 = 0;
            } else {
                t_merge_422c_0001 = 1;
            }
            fEnough = t_merge_422c_0001;
            DrawMineralItem(hdc, dx * 4 + prc->left, prc->top, -fEnough, lpfl->rgwtMin[4]);
            break;
        case 8:
            i = IshdefPrimaryFromLpfl(lpfl, &j);
            if (lpfl->rgdv[i].dp != 0x0) {
                SetTextColor(hdc, 0xff);
            }
            prc->right = prc->right - 6 * dx;
            psz = rgshdef[i].hul.szClass;
            ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
            psz = szT;
            cch = _wsprintf(psz, PCTD, lpfl->rgcsh[i]);
            chT = '+';
            t_scratch_m8a = LOWORD(GetTextExtent(hdc, &chT, 1));
            RightTextOut(hdc, 6 * dx + prc->right - t_scratch_m8a, prc->top, psz, cch, 0);
            if (j <= 1)
                break;
            t_scratch_m8a_2 = LOWORD(GetTextExtent(hdc, &chT, 1));
            TextOut(hdc, 6 * dx + prc->right - t_scratch_m8a_2, prc->top, &chT, 1);
            break;
        case 9:
            i = PctCloakFromLpfl(lpfl);
            if (i != 0) {
                psz = szT;
                cch = _wsprintf(psz, PCTDPCTPCT, i);
            } else {
                cch = strlen(szDblDash);
                psz = szDblDash;
            }
            RightTextOut(hdc, prc->right - 5, prc->top, psz, cch, 0);
            break;
        case 11:
            DrawMineralItem(hdc, prc->right - 2, prc->top, -1, WtFromLpfl(lpfl));
            break;
        case 1:
            psz = szT;
            cch = _wsprintf(psz, "%d", lpfl->ifl + 0x1);
            RightTextOut(hdc, prc->right - 2, prc->top, psz, cch, 0);
        }
        break;
    case 3:
        lpbd = BtlDataGet(vlprgidMisc[irow]);
        if (lpbd != 0x0) {
            SelectObject(hdc, rghfontArial8[1]);
            SetTextColor(hdc, 0x0);
            SetBkMode(hdc, TRANSPARENT);
            if ((uint16_t)icol > 14)
                break;
            switch (icol) {
            case 0:
                if (lpbd->idPlanet == 0xffff) {
                    psz = szT;
                    _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpbd->pt.x, lpbd->pt.y);
                } else {
                    psz = PszGetPlanetName(lpbd->idPlanet);
                }
                if (lpbd->pt.x == sel.scan.pt.x && lpbd->pt.y == sel.scan.pt.y) {
                    SetTextColor(hdc, 0x7f);
                }
                ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
                break;
            case 2:
                DrawMineralItem(hdc, prc->right - 2, prc->top, -1, (uint32_t)lpbd->cplr);
                break;
            case 1:
                if (CBattleUnits(lpbd, 0x5) != 0) {
                    chT = 'O';
                } else if (CBattleUnits(lpbd, 0x6) != 0) {
                    chT = 'T';
                } else {
                    chT = ' ';
                }
                CtrTextOut(hdc, (int32_t)(prc->right - prc->left) / 2 + prc->left, prc->top, &chT, 1);
                break;
            case 6:
                i = 11;
                goto BtlUnitsCom;
            case 7:
                i = 19;
                goto BtlUnitsCom;
            case 8:
                i = 35;
                goto BtlUnitsCom;
            case 9:
                i = 67;
                goto BtlUnitsCom;
            case 10:
                i = 131;
                goto BtlUnitsCom;
            case 3:
                i = 255;
                goto BtlUnitsCom;
            case 4:
                i = 253;
                goto BtlUnitsCom;
            case 5:
                i = 254;
                goto BtlUnitsCom;
            case 11:
            case 12:
                l = CBattleKills(lpbd, icol == 11 ? 1 : 0);
                DrawMineralItem(hdc, prc->right - 2, prc->top, -1, l);
                break;
            case 13:
                i = 253;
                goto LUnitsLeft;
            case 14:
                i = 254;
                goto LUnitsLeft;
            }
            break;
        LUnitsLeft:
            l = CBattleUnits(lpbd, i);
            l = l - CBattleKills(lpbd, icol == 13 ? 1 : 0);
            DrawMineralItem(hdc, prc->right - 2, prc->top, -1, l);
            break;
        BtlUnitsCom:
            DrawMineralItem(hdc, prc->right - 2, prc->top, -1, CBattleUnits(lpbd, i));
            break;
        }
        break;
    case 2:
        lpfl = rglpfl[vlprgidMisc[irow]];
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, 0x0);
        SetBkMode(hdc, TRANSPARENT);
        if ((uint16_t)icol <= 11) {
            switch (icol) {
            case 0:
                if (sel.scan.grobj == grobjFleet && rglpfl[sel.scan.ifl]->id == lpfl->id) {
                    SetTextColor(hdc, 0x7f);
                }
                psz = PszGetFleetName(lpfl->id);
                ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
                break;
            case 2:
                if (lpfl->idPlanet == -1) {
                    psz = szT;
                    _wsprintf(psz, PszGetCompressedString(idsSpaceDD), lpfl->pt.x, lpfl->pt.y);
                } else {
                    psz = PszGetPlanetName(lpfl->idPlanet);
                }
                ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, psz, strlen(psz), 0x0);
                break;
            case 5:
                i = IshdefPrimaryFromLpfl(lpfl, &j);
                prc->right = prc->right - 6 * dx;
                lpsz = rglpshdef[lpfl->iPlayer][i].hul.szClass;
                ExtTextOut(hdc, prc->left, prc->top, 0x4, prc, lpsz, fstrlen(lpsz), 0x0);
                psz = szT;
                cch = _wsprintf(psz, PCTD, lpfl->rgcsh[i]);
                chT = '+';
                t_scratch_m8a_3 = LOWORD(GetTextExtent(hdc, &chT, 1));
                RightTextOut(hdc, 6 * dx + prc->right - t_scratch_m8a_3, prc->top, psz, cch, 0);
                if (j <= 1)
                    break;
                t_scratch_m8a_4 = LOWORD(GetTextExtent(hdc, &chT, 1));
                TextOut(hdc, 6 * dx + prc->right - t_scratch_m8a_4, prc->top, &chT, 1);
                break;
            case 6:
                l = 0;
                for (i = 0; i < 16; i++) {
                    l = l + (int32_t)lpfl->rgcsh[i];
                }
                DrawMineralItem(hdc, prc->right - 2, prc->top, -1, l);
                break;
            case 7:
                l = 0;
                for (i = 0; i < 16; i++) {
                    if (lpfl->rgcsh[i] != 0) {
                        j = LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef)->imdCategory;
                        if (j <= 1 || j >= 6) {
                            l = l + (int32_t)lpfl->rgcsh[i];
                        }
                    }
                }
                DrawMineralItem(hdc, prc->right - 2, prc->top, -1, l);
                break;
            case 8:
                j = 2;
                goto LEFleetCount;
            case 9:
                j = 3;
                goto LEFleetCount;
            case 10:
                j = 5;
                goto LEFleetCount;
            case 11:
                j = 4;
                goto LEFleetCount;
            case 4:
                DrawMineralItem(hdc, prc->right - 2, prc->top, -1, lpfl->wtFleet);
                break;
            case 1:
                psz = szT;
                cch = _wsprintf(psz, "%d", lpfl->ifl + 0x1);
                RightTextOut(hdc, prc->right - 2, prc->top, psz, cch, 0);
                break;
            case 3:
                if (lpfl->fdirValid != 0x0 && lpfl->iwarpFlt > 0x0) {
                    psz = szT;
                    cch = _wsprintf(psz, "%d", lpfl->iwarpFlt);
                } else {
                    psz = "--";
                    cch = 2;
                }
                RightTextOut(hdc, prc->right - 2, prc->top, psz, cch, 0);
            }
            break;
        LEFleetCount:
            l = 0;
            for (i = 0; i < 16; i++) {
                if (lpfl->rgcsh[i] != 0 && LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef)->imdCategory == j) {
                    l = l + (int32_t)lpfl->rgcsh[i];
                }
            }
            DrawMineralItem(hdc, prc->right - 2, prc->top, -1, l);
        }
    default:
    }
    return;
}

void DrawMineralItem(HDC hdc, int16_t x, int16_t y, int16_t iMineral, int32_t l) {
    char   *psz;
    int16_t cch;

    if (iMineral >= 0) {
        SetTextColor(hdc, rgcrMinerals[iMineral]);
    } else {
        SetTextColor(hdc, 0x0);
    }
    if (l < 0) {
        cch = strlen(szDblDash);
        psz = szDblDash;
    } else {
        psz = PszFromLongK(l, &cch);
    }
    RightTextOut(hdc, x, y, psz, cch, 0);
    return;
}

char *PszGetDestName(FLEET *lpfl, HDC hdc) {
    int16_t i;
    ORDER   ord;

    ord = lpfl->lpplord->rgord[0];
    if (lpfl->cord > 1) {
        if (ord.fValidTask != 0x0) {
            switch (ord.grTask) {
            case grTaskXfer:
                i = 0;
                while (1) {
                    if (i >= 5)
                        goto L_5064;
                    if (ord.txp.rgia[i].iAction == iActionWaitPercent)
                        goto LDelayed;
                    i = i + 1;
                }
            case grTaskColonize:
                if (ord.grobj != grobjPlanet)
                    break;
                return szDblDash;
            case grTaskLayMines:
            LDelayed:
                if (hdc != 0x0) {
                    SetTextColor(hdc, 0x7f);
                }
                return PszGetCompressedString(idsDelayed);
            default:
                break;
            case grTaskMerge:
            case grTaskScrap:
                return szDblDash;
            }
        }
    L_5064:
        ord = lpfl->lpplord->rgord[1];
        return PszGetLocName(ord.grobj, ord.id, ord.pt.x, ord.pt.y);
    }
    return szDblDash;
}

int16_t FDestIsWP0(FLEET *lpfl) {
    int16_t i;
    ORDER   ord;

    ord = lpfl->lpplord->rgord[0];
    if (lpfl->cord <= 1) {
        return 1;
    }
    if (ord.fValidTask != 0x0) {
        switch (ord.grTask) {
        case grTaskXfer:
            i = 0;
            while (1) {
                if (i >= 5) {
                    return 0;
                }
                if (ord.txp.rgia[i].iAction == iActionWaitPercent)
                    break;
                i = i + 1;
            }
            return 1;
        case grTaskColonize:
            if (ord.grobj != grobjPlanet)
                break;
        case grTaskMerge:
        case grTaskScrap:
        case grTaskLayMines:
            return 1;
        default:
        }
    }
    return 0;
}

char *PszGetETA(HDC hdc, FLEET *lpfl, int16_t *pcYears) {
    POINT16 pt;
    int16_t c;
    int16_t i;
    ORDER   ord;
    char   *psz;

    ord = lpfl->lpplord->rgord[0];
    pt = ord.pt;
    if (lpfl->cord > 1) {
        if (ord.fValidTask != 0x0) {
            switch (ord.grTask) {
            case grTaskXfer:
                i = 0;
                while (1) {
                    if (i >= 5)
                        goto L_5290;
                    if (ord.txp.rgia[i].iAction == iActionWaitPercent)
                        goto LNoETA;
                    i = i + 1;
                }
            case grTaskColonize:
                if (ord.grobj == grobjPlanet)
                    goto LNoETA;
            default:
                break;
            case grTaskMerge:
            case grTaskScrap:
            case grTaskLayMines:
                goto LNoETA;
            }
        }
    L_5290:
        ord = lpfl->lpplord->rgord[1];
        CchGetETA(hdc, lpfl, szWork, 1, 1);
        if (hdc != 0x0 && EstFuelUse(lpfl, 0, ord.iWarp, -1, 0) > lpfl->rgwtMin[4]) {
            SetTextColor(hdc, 0xff);
        }
        if (pcYears != 0x0) {
            psz = szWork;
            c = 0;
            for (; (int16_t)*psz >= '0' && (int16_t)*psz <= '9'; psz++) {
                c = 10 * c + ((int16_t)*psz - 48);
            }
            if (c == 0) {
                c = 32000;
            }
            *pcYears = c;
        }
        return szWork;
    }
LNoETA:
    if (pcYears != 0x0) {
        *pcYears = 0;
    }
    return szDblDash;
}

char *PszGetTaskName(FLEET *lpfl, int16_t *picr) {
    int16_t        icr;
    StringId       ids;
    XferActionType opOrd;
    int16_t        iZip;
    int16_t        i;
    ORDER          ord;
    int16_t        fPercent;
    char          *psz;

    icr = -1;
    ord = lpfl->lpplord->rgord[0];
    *picr = -1;
    if (ord.fValidTask != 0x0 && ord.grTask - 1 <= 0x7) {
        switch (ord.grTask) {
        case 1:
            for (i = 0; i < 4; i++) {
                if (ord.txp.rgia[i].iAction == iActionWaitPercent)
                    goto LShowTask;
            }
        case 3:
        case 7:
            goto L_5476;
        case 2:
        case 4:
        case 5:
        case 6:
        case 8:
        }
        goto LShowTask;
    }
L_5476:
    if (lpfl->cord > 1) {
        ord = lpfl->lpplord->rgord[1];
    }
LShowTask:
    if (ord.fValidTask != 0x0) {
        ids = ord.grTask + 99;
        switch (ord.grTask) {
        case grTaskXfer:
            for (i = 0; i < 4; i++) {
                if (vrgZip[i].fValid != 0x0 && memcmp(&vrgZip[i], &ord.txp, 0xa) == 0)
                    goto L_5513;
            }
            ids = idsAction;
            iZip = -1;
            if (ord.txp.rgia[4].iAction == iActionLoadDunnage &&
                (ord.txp.rgia[3].iAction == iActionNone || (ord.txp.rgia[3].iAction == iActionUnloadAll && ord.txp.rgia[0].iAction == iActionUnloadAll))) {
                opOrd = ord.txp.rgia[0].iAction;
                switch (opOrd) {
                case iActionLoadAll:
                case iActionUnloadAll:
                case iActionWaitPercent:
                    for (i = 1; i < 3 && ord.txp.rgia[i].iAction == opOrd; i++) {
                    }
                    if (i == 3) {
                        switch (opOrd) {
                        case iActionLoadAll:
                            iZip = 0;
                            break;
                        case iActionUnloadAll:
                            iZip = 1;
                            break;
                        case iActionWaitPercent:
                            iZip = 2;
                        default:
                        }
                        return rgszZipOrder[iZip];
                    }
                default:
                }
            }
            for (i = 4; i >= 0; i--) {
                opOrd = ord.txp.rgia[i].iAction;
                if (opOrd + 109 > ids) {
                    ids = opOrd + 109;
                    icr = i;
                }
            }
            if (ids == idsAction) {
                return PszGetCompressedString(idsTransport);
            }
            opOrd = ord.txp.rgia[icr].iAction;
            *picr = icr;
            fPercent = 0;
            if (opOrd - 1 > 0x8)
                break;
            switch (opOrd) {
            case 7:
                if (icr == 4) {
                    ids = idsLoadOptimal;
                }
            case 1:
            case 2:
                return PszGetCompressedString(ids);
            case 5:
            case 6:
                fPercent = 1;
            case 4:
            case 8:
            case 9:
                psz = PszGetCompressedString(ids);
                psz[strlen(psz) - 3] = 0;
                if (fPercent == 0) {
                    _wsprintf(szWork, icr == 4 ? "%s %dmg" : "%s %dkT", psz, ord.txp.rgia[icr].cQuan);
                } else {
                    _wsprintf(szWork, "%s %d%%", psz, ord.txp.rgia[icr].cQuan);
                }
                return szWork;
            case 3:
                return szDblDash;
            }
        L_5513:
            return vrgZip[i].szName;
        case grTaskLayMines:
            if (ord.tlm.cTime >= 0x5) {
                CchGetString(ids, szWork);
            } else {
                _wsprintf(szWork, "%s  %dy", PszGetCompressedString(ids), ord.tlm.cTime + 0x1);
            }
            return szWork;
        case grTaskPatrol:
            if (ord.tptl.iDist >= 0xb) {
                CchGetString(ids, szWork);
            } else {
                _wsprintf(szWork, "%s  %dly", PszGetCompressedString(ids), (ord.tptl.iDist + 0x1) * 0x32);
            }
            return szWork;
        case grTaskAutoRoute:
        default:
            return PszGetCompressedString(ids);
        }
    }
    return szDblDash;
}

void SortReportCache(int16_t irpt, int16_t icol) {
    uint16_t rgidRep[1024];
    PLANET  *lpplMac;
    int16_t  cRows;
    uint16_t iItem;
    PLANET  *lppl;
    FLEET   *lpfl;
    int16_t  i;
    int16_t  t_598d;
    int16_t  t_5a26;
    int16_t  t_5ab5;

    cRows = 0;
    iItem = 0x0;
    if (vprptCur->icolSort != icol) {
        vicolSortPrev = vprptCur->icolSort;
        viSubsortPrev = vprptCur->iSubsort;
        vfAscendingPrev = vprptCur->fAscending;
        vprptCur->icolSort = icol;
        gd.fChgReports = 0x1;
    }
    if (hwndReportDlg != 0x0 || vprptCur->fCached == 0) {
        switch (irpt) {
        case 1:
            vlprgidRep = vlprgidFleet;
            for (iItem = 0x0; iItem < cFleet; iItem++) {
                lpfl = rglpfl[iItem];
                if (rglpfl[iItem] == 0x0)
                    break;
                if (lpfl->iplr == idPlayer) {
                    t_598d = cRows;
                    cRows = cRows + 1;
                    rgidRep[t_598d] = iItem;
                }
            }
            goto L_5b5d;
        case 2:
            vlprgidRep = vlprgidMisc;
            vrptBattle.fCached = 0;
            for (iItem = 0x0; iItem < cFleet; iItem++) {
                lpfl = rglpfl[iItem];
                if (rglpfl[iItem] == 0x0)
                    break;
                if (lpfl->iplr != idPlayer) {
                    t_5a26 = cRows;
                    cRows = cRows + 1;
                    rgidRep[t_5a26] = iItem;
                }
                if (cRows >= 1020)
                    break;
            }
            goto L_5b5d;
        case 0:
            vlprgidRep = vlprgidPlanet;
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                if (lppl->iPlayer == idPlayer && lppl->det == 0x7) {
                    t_5ab5 = cRows;
                    cRows = cRows + 1;
                    rgidRep[t_5ab5] = iItem;
                }
                iItem = iItem + 0x1;
            }
            goto L_5b5d;
        case 3:
            vlprgidRep = vlprgidMisc;
            vrptEFleet.fCached = 0;
            cRows = CBattles();
            for (i = 0; i < cRows; i++) {
                rgidRep[i] = i;
            }
            goto L_5b5d;
        default:
        }
        return;
    L_5b5d:
        vprptCur->cRows = cRows;
        qsort(rgidRep, cRows, 0x2, (QSORTCOMPARE)ICompReport);
        fmemcpy(vlprgidRep, rgidRep, cRows * 2);
        vprptCur->fCached = 1;
    }
    return;
}

int ICompReport(uint16_t *pid1, uint16_t *pid2) {
    char     szT[80];
    int32_t  l2;
    int16_t  fAscending;
    int16_t  icolSort;
    int16_t  i1;
    int16_t  j;
    int16_t  i;
    int32_t  l1;
    int16_t  iSubsort;
    char    *psz;
    int16_t  iRet;
    int16_t  i2;
    int16_t  fTier2;
    int16_t  irpt;
    PLANET  *lppl2;
    PLANET  *lppl1;
    float    pct2;
    float    pct1;
    int16_t  iFirst;
    int32_t  rgl[4];
    int16_t  iLast;
    FLEET   *lpfl2;
    FLEET   *lpfl1;
    BTLDATA *lpbd1;
    int16_t  ibtl2;
    int16_t  ibtl1;
    BTLDATA *lpbd2;
    int16_t  t_scratch_m7a;
    int16_t  t_scratch_m7a_2;
    uint16_t t_scratch_m80;
    uint16_t t_scratch_m80_2;
    int16_t  t_scratch_m7a_3;
    uint16_t t_scratch_m7a_4;

    iRet = 0;
    iSubsort = vprptCur->iSubsort;
    irpt = vprptCur->irpt;
    icolSort = vprptCur->icolSort;
    fAscending = vprptCur->fAscending;
    fTier2 = 0;
    while (1) {
        switch (irpt) {
        case 0:
            lppl1 = lpPlanets + *pid1;
            lppl2 = lpPlanets + *pid2;
            if ((uint16_t)icolSort > 14)
                break;
            switch (icolSort) {
            case 1:
                if (lppl1->fStarbase != 0x0) {
                    if (lppl2->fStarbase != 0x0) {
                        iRet = fstrcmp(rglpshdefSB[idPlayer][lppl1->isb].hul.szClass, rglpshdefSB[idPlayer][lppl2->isb].hul.szClass);
                        break;
                    }
                    iRet = -1;
                    break;
                }
                if (lppl2->fStarbase != 0x0) {
                    iRet = 1;
                    break;
                }
                iRet = 0;
                break;
            case 14:
                if (lppl1->idRoute != 0x0) {
                    if (lppl2->idRoute != 0x0) {
                        psz = PszGetPlanetName(lppl1->idRoute - 1);
                        strcpy(szT, psz);
                        psz = PszGetPlanetName(lppl2->idRoute - 1);
                        iRet = strcmp(szT, psz);
                        break;
                    }
                    iRet = -1;
                    break;
                }
                if (lppl2->idRoute != 0x0) {
                    iRet = 1;
                    break;
                }
                iRet = 0;
                break;
            case 13:
                if (lppl1->idFling != 0x0) {
                    if (lppl2->idFling != 0x0) {
                        psz = PszGetPlanetName(lppl1->idFling - 1);
                        strcpy(szT, psz);
                        psz = PszGetPlanetName(lppl2->idFling - 1);
                        iRet = strcmp(szT, psz);
                        break;
                    }
                    iRet = -1;
                    break;
                }
                if (lppl2->idFling != 0x0) {
                    iRet = 1;
                    break;
                }
                iRet = 0;
                break;
            case 0:
                psz = PszGetPlanetName(lppl1->id);
                strcpy(szT, psz);
                psz = PszGetPlanetName(lppl2->id);
                iRet = strcmp(szT, psz);
                break;
            case 5:
                FillPlanetProdLB(0x0, 0x0, lppl1);
                strcpy(szT, szWork);
                FillPlanetProdLB(0x0, 0x0, lppl2);
                iRet = strcmp(&szT[6], &szWork[6]);
                break;
            case 3:
                t_scratch_m7a = PctPlanetCapacity(lppl2);
                iRet = PctPlanetCapacity(lppl1) - t_scratch_m7a;
                break;
            case 2:
                iRet = LOWORD(lppl1->rgwtMin[3]) - LOWORD(lppl2->rgwtMin[3]);
                break;
            case 4:
                t_scratch_m7a_2 = PctPlanetDesirability(lppl2, idPlayer);
                iRet = PctPlanetDesirability(lppl1, idPlayer) - t_scratch_m7a_2;
                break;
            case 11:
                if (iSubsort != 3) {
                    i1 = lppl1->rgMinConc[iSubsort];
                    i2 = lppl2->rgMinConc[iSubsort];
                } else {
                    i2 = 0;
                    i1 = 0;
                    for (i = 0; i < 3; i++) {
                        i1 = i1 + lppl1->rgMinConc[i];
                        i2 = i2 + lppl2->rgMinConc[i];
                    }
                }
                iRet = i1 - i2;
                break;
            case 8:
                if (lppl1->cDefenses != 0x0) {
                    CalcPctSurvive(lppl1, &pct1, 0x0);
                    pct1 = 1.0 - pct1;
                } else {
                    pct1 = 0.0;
                }
                if (lppl2->cDefenses != 0x0) {
                    CalcPctSurvive(lppl2, &pct2, 0x0);
                    pct2 = 1.0 - pct2;
                } else {
                    pct2 = 0.0;
                }
                if (pct1 >= pct2) {
                    if (pct1 <= pct2) {
                        iRet = 0;
                        break;
                    }
                    iRet = 1;
                    break;
                }
                iRet = -1;
                break;
            case 7:
                t_scratch_m80 = lppl2->cFactories;
                iRet = lppl1->cFactories - t_scratch_m80;
                break;
            case 6:
                t_scratch_m80_2 = lppl2->cMines;
                iRet = lppl1->cMines - t_scratch_m80_2;
                break;
            case 10:
                if (iSubsort != 3) {
                    iLast = iSubsort;
                    iFirst = iSubsort;
                } else {
                    iFirst = 0;
                    iLast = 2;
                }
                EstMineralsMined(lppl1, rgl, -1, 0);
                l1 = 0;
                for (i = iFirst; i <= iLast; i++) {
                    l1 = l1 + rgl[i];
                }
                EstMineralsMined(lppl2, rgl, -1, 0);
                l2 = 0;
                for (i = iFirst; i <= iLast; i++) {
                    l2 = l2 + rgl[i];
                }
                iRet = LOWORD(l1) - LOWORD(l2);
                break;
            case 9:
                if (iSubsort != 3) {
                    iLast = iSubsort;
                    iFirst = iSubsort;
                } else {
                    iFirst = 0;
                    iLast = 2;
                }
                l2 = 0;
                l1 = 0;
                for (i = iFirst; i <= iLast; i++) {
                    l1 = l1 + lppl1->rgwtMin[i];
                    l2 = l2 + lppl2->rgwtMin[i];
                }
                l1 = l1 - l2;
                if (l1 < 0) {
                    iRet = -1;
                    break;
                }
                if (l1 <= 0) {
                    iRet = 0;
                    break;
                }
                iRet = 1;
                break;
            case 12:
                t_scratch_m7a_3 = CResourcesAtPlanet(lppl2, idPlayer);
                iRet = CResourcesAtPlanet(lppl1, idPlayer) - t_scratch_m7a_3;
            }
            break;
        case 1:
            lpfl1 = rglpfl[*pid1];
            lpfl2 = rglpfl[*pid2];
            if ((uint16_t)icolSort > 11)
                break;
            switch (icolSort) {
            case 0:
                psz = PszGetFleetName(lpfl1->id);
                strcpy(szT, psz);
                psz = PszGetFleetName(lpfl2->id);
                iRet = strcmp(szT, psz);
                break;
            case 2:
                if (lpfl1->idPlanet == -1) {
                    _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpfl1->pt.x, lpfl1->pt.y);
                } else {
                    psz = PszGetPlanetName(lpfl1->idPlanet);
                    strcpy(szT, psz);
                }
                if (lpfl2->idPlanet == -1) {
                    _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpfl2->pt.x, lpfl2->pt.y);
                    psz = szWork;
                } else {
                    psz = PszGetPlanetName(lpfl2->idPlanet);
                }
                iRet = strcmp(szT, psz);
                break;
            case 10:
                t_scratch_m7a_4 = lpfl2->iplan;
                iRet = lpfl1->iplan - t_scratch_m7a_4;
                break;
            case 3:
                psz = PszGetDestName(lpfl1, 0x0);
                strcpy(szT, psz);
                psz = PszGetDestName(lpfl2, 0x0);
                iRet = strcmp(szT, psz);
                if (iRet >= 0) {
                    if (iRet <= 0)
                        break;
                    iRet = 1;
                    break;
                }
                iRet = -1;
                break;
            case 4:
                PszGetETA(0x0, lpfl1, &i1);
                PszGetETA(0x0, lpfl2, &i2);
                iRet = i1 - i2;
                break;
            case 9:
                l1 = (int32_t)PctCloakFromLpfl(lpfl1);
                l2 = (int32_t)PctCloakFromLpfl(lpfl2);
                goto LRetDiff;
            case 8:
                l1 = (int32_t)IshdefPrimaryFromLpfl(lpfl1, &i1);
                l2 = (int32_t)IshdefPrimaryFromLpfl(lpfl2, &i2);
                if (l1 != l2) {
                    iRet = strcmp(rgshdef[l1].hul.szClass, rgshdef[l2].hul.szClass);
                    if (iRet == 0) {
                        iRet = lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2];
                    }
                    if (iRet == 0) {
                        iRet = LOWORD(l1) - LOWORD(l2);
                    }
                } else {
                    iRet = lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2];
                }
                if (iRet != 0)
                    break;
                iRet = i1 - i2;
                break;
            case 5:
                psz = PszGetTaskName(lpfl1, &i1);
                strcpy(szT, psz);
                psz = PszGetTaskName(lpfl2, &i2);
                iRet = strcmp(szT, psz);
                if (iRet >= 0) {
                    if (iRet <= 0) {
                        if (i1 <= i2) {
                            if (i1 >= i2)
                                break;
                            iRet = -1;
                            break;
                        }
                        iRet = 1;
                        break;
                    }
                    iRet = 1;
                    break;
                }
                iRet = -1;
                break;
            case 6:
                iSubsort = -1;
            case 7:
                if (iSubsort != 4) {
                    if (iSubsort == -1) {
                        iSubsort = 4;
                    }
                    l1 = lpfl1->rgwtMin[iSubsort];
                    l2 = lpfl2->rgwtMin[iSubsort];
                    goto LRetDiff;
                }
                l2 = 0;
                l1 = 0;
                for (i = 0; i <= 3; i++) {
                    l1 = l1 + lpfl1->rgwtMin[i];
                    l2 = l2 + lpfl2->rgwtMin[i];
                }
                goto LRetDiff;
            case 11:
                l1 = WtFromLpfl(lpfl1);
                l2 = WtFromLpfl(lpfl2);
                goto LRetDiff;
            case 1:
                l1 = (int32_t)lpfl1->id;
                l2 = (int32_t)lpfl2->id;
                goto LRetDiff;
            }
            break;
        case 3:
            ibtl1 = *pid1;
            ibtl2 = *pid2;
            lpbd1 = BtlDataGet(ibtl1);
            lpbd2 = BtlDataGet(ibtl2);
            if (lpbd1 != 0x0 && lpbd2 != 0x0) {
                if ((uint16_t)icolSort > 14)
                    break;
                switch (icolSort) {
                case 0:
                    if (lpbd1->idPlanet == 0xffff) {
                        _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpbd1->pt.x, lpbd1->pt.y);
                    } else {
                        psz = PszGetPlanetName(lpbd1->idPlanet);
                        strcpy(szT, psz);
                    }
                    if (lpbd2->idPlanet == 0xffff) {
                        _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpbd2->pt.x, lpbd2->pt.y);
                        psz = szWork;
                    } else {
                        psz = PszGetPlanetName(lpbd2->idPlanet);
                    }
                    iRet = strcmp(szT, psz);
                    break;
                case 1:
                    l1 = CBattleUnits(lpbd1, 0x5);
                    if (l1 == 0) {
                        l1 = (int32_t)(CBattleUnits(lpbd1, 0x6) * 2);
                    }
                    l2 = CBattleUnits(lpbd2, 0x5);
                    if (l2 != 0)
                        goto LRetDiff;
                    l2 = (int32_t)(CBattleUnits(lpbd2, 0x6) * 2);
                    goto LRetDiff;
                case 2:
                    l1 = (uint32_t)lpbd1->cplr;
                    l2 = (uint32_t)lpbd2->cplr;
                    goto LRetDiff;
                case 6:
                    i = 11;
                    goto BtlUnitsCom;
                case 7:
                    i = 19;
                    goto BtlUnitsCom;
                case 8:
                    i = 35;
                    goto BtlUnitsCom;
                case 9:
                    i = 67;
                    goto BtlUnitsCom;
                case 10:
                    i = 131;
                    goto BtlUnitsCom;
                case 3:
                    i = 255;
                    goto BtlUnitsCom;
                case 4:
                    i = 253;
                    goto BtlUnitsCom;
                case 5:
                    i = 254;
                    goto BtlUnitsCom;
                case 11:
                case 12:
                    l1 = CBattleKills(lpbd1, icolSort == 11 ? 1 : 0);
                    l2 = CBattleKills(lpbd2, icolSort == 11 ? 1 : 0);
                    goto LRetDiff;
                case 13:
                    i = 253;
                    goto LUnitsLeft;
                case 14:
                    i = 254;
                    goto LUnitsLeft;
                }
                break;
            LUnitsLeft:
                l1 = CBattleUnits(lpbd1, i);
                l2 = CBattleUnits(lpbd2, i);
                l1 = l1 - CBattleKills(lpbd1, icolSort == 13 ? 1 : 0);
                l2 = l2 - CBattleKills(lpbd2, icolSort == 13 ? 1 : 0);
                goto LRetDiff;
            BtlUnitsCom:
                l1 = CBattleUnits(lpbd1, i);
                l2 = CBattleUnits(lpbd2, i);
                goto LRetDiff;
            }
            iRet = 0;
            break;
        case 2:
            lpfl1 = rglpfl[*pid1];
            lpfl2 = rglpfl[*pid2];
            if ((uint16_t)icolSort <= 11) {
                switch (icolSort) {
                case 0:
                    psz = PszGetFleetName(lpfl1->id);
                    strcpy(szT, psz);
                    psz = PszGetFleetName(lpfl2->id);
                    iRet = strcmp(szT, psz);
                    break;
                case 2:
                    if (lpfl1->idPlanet == -1) {
                        _wsprintf(szT, PszGetCompressedString(idsSpaceDD), lpfl1->pt.x, lpfl1->pt.y);
                    } else {
                        psz = PszGetPlanetName(lpfl1->idPlanet);
                        strcpy(szT, psz);
                    }
                    if (lpfl2->idPlanet == -1) {
                        _wsprintf(szWork, PszGetCompressedString(idsSpaceDD), lpfl2->pt.x, lpfl2->pt.y);
                        psz = szWork;
                    } else {
                        psz = PszGetPlanetName(lpfl2->idPlanet);
                    }
                    iRet = strcmp(szT, psz);
                    break;
                case 5:
                    l1 = (int32_t)IshdefPrimaryFromLpfl(lpfl1, &i1);
                    l2 = (int32_t)IshdefPrimaryFromLpfl(lpfl2, &i2);
                    if (l1 != l2) {
                        iRet = fstrcmp(rglpshdef[lpfl1->iPlayer][l1].hul.szClass, rglpshdef[lpfl2->iPlayer][l2].hul.szClass);
                        if (iRet == 0) {
                            iRet = lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2];
                        }
                        if (iRet == 0) {
                            iRet = LOWORD(l1) - LOWORD(l2);
                        }
                    } else {
                        iRet = lpfl1->rgcsh[l1] - lpfl2->rgcsh[l2];
                    }
                    if (iRet != 0)
                        break;
                    iRet = i1 - i2;
                    break;
                case 4:
                    l1 = lpfl1->wtFleet;
                    l2 = lpfl2->wtFleet;
                    goto LRetDiff;
                case 1:
                    l1 = lpfl1->ifl;
                    l2 = lpfl2->ifl;
                    goto LRetDiff;
                case 6:
                    l2 = 0;
                    l1 = 0;
                    for (i = 0; i < 16; i++) {
                        l1 = l1 + (int32_t)lpfl1->rgcsh[i];
                        l2 = l2 + (int32_t)lpfl2->rgcsh[i];
                    }
                    goto LRetDiff;
                case 7:
                    l2 = 0;
                    l1 = 0;
                    for (i = 0; i < 16; i++) {
                        if (lpfl1->rgcsh[i] != 0) {
                            j = LphuldefFromId(rglpshdef[lpfl1->iPlayer][i].hul.ihuldef)->imdCategory;
                            if (j <= 1 || j >= 6) {
                                l1 = l1 + (int32_t)lpfl1->rgcsh[i];
                            }
                        }
                        if (lpfl2->rgcsh[i] != 0) {
                            j = LphuldefFromId(rglpshdef[lpfl2->iPlayer][i].hul.ihuldef)->imdCategory;
                            if (j <= 1 || j >= 6) {
                                l2 = l2 + (int32_t)lpfl2->rgcsh[i];
                            }
                        }
                    }
                    goto LRetDiff;
                case 8:
                    j = 2;
                    goto LEFleetCount;
                case 9:
                    j = 3;
                    goto LEFleetCount;
                case 10:
                    j = 5;
                    goto LEFleetCount;
                case 11:
                    j = 4;
                    goto LEFleetCount;
                case 3:
                    if (lpfl1->fdirValid == 0x0) {
                        l1 = -1;
                    } else {
                        l1 = lpfl1->iwarpFlt;
                    }
                    if (lpfl2->fdirValid == 0x0) {
                        l2 = -1;
                        goto LRetDiff;
                    }
                    l2 = lpfl2->iwarpFlt;
                    goto LRetDiff;
                }
                break;
            LEFleetCount:
                l2 = 0;
                l1 = 0;
                for (i = 0; i < 16; i++) {
                    if (lpfl1->rgcsh[i] != 0 && LphuldefFromId(rglpshdef[lpfl1->iPlayer][i].hul.ihuldef)->imdCategory == j) {
                        l1 = l1 + (int32_t)lpfl1->rgcsh[i];
                    }
                    if (lpfl2->rgcsh[i] != 0 && LphuldefFromId(rglpshdef[lpfl2->iPlayer][i].hul.ihuldef)->imdCategory == j) {
                        l2 = l2 + (int32_t)lpfl2->rgcsh[i];
                    }
                }
                goto LRetDiff;
            }
        default:
        }
        goto L_746b;
    LRetDiff:
        l1 = l1 - l2;
        if (l1 < 0) {
            iRet = -1;
        } else if (l1 <= 0) {
            iRet = 0;
        } else {
            iRet = 1;
        }
    L_746b:
        if (fAscending == 0) {
            iRet = -iRet;
        }
        if (iRet != 0 || fTier2 != 0 || ((icolSort == vicolSortPrev && iSubsort == viSubsortPrev) || vicolSortPrev < 0))
            break;
        icolSort = vicolSortPrev;
        iSubsort = viSubsortPrev;
        fAscending = vfAscendingPrev;
        fTier2 = 1;
    }
    return iRet;
}

void ReportColumnPopup(POINT16 pt, int16_t icol, int16_t fRightBtn) {
    HDC     hdc;
    char    szT[50];
    char    rgsz[32][50];
    int16_t iBase;
    int16_t cSubsort;
    int16_t j;
    int16_t i;
    int16_t ibit;
    int16_t fccolChange;
    int16_t rgcol[32];
    char    szColTitle[50];
    int16_t cItems;
    char   *psz[32];
    int16_t cch;
    int16_t iRet;
    int16_t iHide;
    int16_t iSortLast;
    int16_t t_7699;

    cSubsort = 0;
    fccolChange = 0;
    hdc = GetDC(hwndReportDlg);
    DxReportColHdr(vprptCur->irpt, icol, szColTitle, hdc);
    cItems = 0;
    for (i = 0; i < 2; i++) {
        cch = CchGetString(i == 0 ? idsSort : idsReverseSort, rgsz[cItems]);
        strcpy(&rgsz[cItems][cch], szColTitle);
        cItems = cItems + 1;
        if (vprptCur->irpt == 0) {
            switch (icol) {
            default:
                goto L_759a;
            case 11:
            case 9:
            case 10:
                goto L_75b0;
            }
            continue;
        }
    L_759a:
        if (vprptCur->irpt != 1 || icol != 7)
            continue;
    L_75b0:
        strcpy(rgsz[cItems], rgsz[cItems - 1]);
        rgsz[cItems - 1][0] = 0;
        cItems = cItems + 1;
        for (j = 0; j < (vprptCur->irpt == 1 ? 0x1 : 0x0) + 0x3; j++) {
            strcpy(rgsz[cItems], rgszMinerals[j]);
            cItems = cItems + 1;
        }
        rgsz[cItems][0] = -1;
        rgsz[cItems][1] = 0;
        cItems = cItems + 1;
        strcpy(rgsz[cItems], PszGetCompressedString(idsWeightedAverage));
        cItems = cItems + 1;
        t_7699 = cItems;
        cItems = cItems + 1;
        rgsz[t_7699][0] = 0;
        cSubsort = 5;
    }
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems = cItems + 1;
    cch = CchGetString(idsHide, rgsz[cItems]);
    strcpy(&rgsz[cItems][cch], szColTitle);
    cch = CchGetString(idsColumn, szT);
    strcat(rgsz[cItems], szT);
    iHide = cItems;
    iSortLast = cItems;
    cItems = cItems + 1;
    rgsz[cItems][0] = -1;
    rgsz[cItems][1] = 0;
    cItems = cItems + 1;
    if (icol == 0) {
        cItems = cItems - 2;
        iHide = -1;
    }
    iBase = cItems;
    i = 0;
    ibit = 1;
    while (i < vprptCur->cFields) {
        if (((int32_t)ibit & vprptCur->grbitVisible) == 0x0) {
            DxReportColHdr(vprptCur->irpt, i, szColTitle, hdc);
            cch = CchGetString(idsShow, rgsz[cItems]);
            strcpy(&rgsz[cItems][cch], szColTitle);
            cch = CchGetString(idsColumn, szT);
            strcat(rgsz[cItems], szT);
            rgcol[cItems] = i;
            cItems = cItems + 1;
        }
        i = i + 1;
        ibit = ibit * 2;
    }
    if (cItems == iBase) {
        cItems = cItems - 1;
    }
    ReleaseDC(hwndReportDlg, hdc);
    for (i = 0; i < cItems; i++) {
        if ((int16_t)rgsz[i][0] == 0) {
            psz[i] = 0x0;
        } else {
            psz[i] = rgsz[i];
        }
    }
    iRet = PopupMenu(hwndReportDlg, pt.x, pt.y, cItems, 0x0, psz, -1, fRightBtn);
    if (iRet >= 0) {
        gd.fChgReports = 0x1;
        if (iRet >= iSortLast) {
            if (iRet != iHide) {
                if (iRet >= iBase) {
                    fccolChange = 1;
                    vprptCur->grbitVisible = vprptCur->grbitVisible | (int32_t)(0x1 << rgcol[iRet]);
                }
            } else {
                fccolChange = 1;
                vprptCur->grbitVisible = vprptCur->grbitVisible & (int32_t)~(0x1 << icol);
            }
        } else {
            vicolSortPrev = vprptCur->icolSort;
            viSubsortPrev = vprptCur->iSubsort;
            vfAscendingPrev = vprptCur->fAscending;
            vprptCur->icolSort = icol;
            if (cSubsort != 0) {
                vprptCur->iSubsort = (int32_t)(iRet - 2) % (cSubsort + 3 + (vprptCur->irpt == 1 ? 1 : 0));
                if (vprptCur->iSubsort > (vprptCur->irpt == 1 ? 0x1 : 0x0) + 0x3) {
                    vprptCur->iSubsort = (vprptCur->irpt == 1 ? 1 : 0) + 3;
                }
                vprptCur->fAscending = iRet >= cSubsort + 2 ? 0 : 1;
            } else {
                vprptCur->fAscending = iRet == 0 ? 1 : 0;
            }
            SortReportCache(vprptCur->irpt, icol);
        }
        if (fccolChange != 0) {
            SetHScrollBar();
        }
        InvalidateRect(hwndReportDlg, 0x0, 1);
    }
    return;
}

void InvalidateReport(int16_t irpt, int16_t fReload) {
    int16_t   fResetRpt;
    int16_t   fClearRpt;
    RPT      *prptSav;
    uint16_t *lprgidSav;
    RECT      rc;

    fClearRpt = 0;
    fResetRpt = 0;
    if (gd.fGeneratingTurn == 0x0 && fAi == 0) {
        if (hwndReportDlg == 0x0 || irpt != vprptCur->irpt) {
            if (vprptCur != 0x0) {
                if (fReload != 0 && vprptCur->irpt != irpt) {
                    lprgidSav = vlprgidRep;
                    prptSav = vprptCur;
                    fResetRpt = 1;
                    if (irpt != 0) {
                        vprptCur = &vrptFleet;
                    } else {
                        vprptCur = &vrptPlanet;
                    }
                }
            } else {
                fClearRpt = fReload;
                if (irpt != 0) {
                    vprptCur = &vrptFleet;
                } else {
                    vprptCur = &vrptPlanet;
                }
            }
        } else {
            GetClientRect(hwndReportDlg, &rc);
            if (fReload != 2) {
                rc.top = dyArial8 + 6;
                rc.bottom = (dyArial8 + 4) * vprptCur->cRowsVis + rc.top;
            }
            InvalidateRect(hwndReportDlg, &rc, fReload == 2 ? 1 : 0);
            gd.fRptSafeDraw = 0x1;
        }
        vprptCur->fCached = 0;
        if (fReload != 0) {
            SortReportCache(vprptCur->irpt, vprptCur->icolSort);
        }
        if (fClearRpt == 0) {
            if (fResetRpt == 0) {
                if (fReload != 0 && hwndReportDlg != 0x0 && irpt == vprptCur->irpt) {
                    SetScrollRange(vprptCur->hwndVScroll, 2, 0, vprptCur->cRows - vprptCur->cRowsVis, 0);
                    InvalidateRect(hwndReportDlg, 0x0, 1);
                }
            } else {
                vprptCur = prptSav;
                vlprgidRep = lprgidSav;
            }
        } else {
            vprptCur = 0x0;
        }
    }
    return;
}

void ExecuteReportClick(POINT16 pt, int16_t irpt, int16_t icol, int16_t irow) {
    HDC      hdc;
    BTLDATA *lpbd;
    PLANET  *lppl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  ibit;
    int32_t  rglQuan[4];
    int16_t  xCur;
    int16_t  dxOffset;
    SCAN     scan;
    int16_t  t_call_7e75;

    hdc = GetDC(hwndReportDlg);
    switch (irpt) {
    case 0:
        lppl = lpPlanets + vlprgidPlanet[irow];
        if (hwndProdDlg == 0x0) {
            SelectAdjPlanet(0, lppl->id);
            InvalidateReport(0, 0);
            if ((uint16_t)icol > 12)
                break;
            switch (icol) {
            case 0:
                if (lppl->fStarbase == 0x0 || pt.x <= vprptCur->rgbdx[0] * 0x2 - 0x8)
                    break;
                goto LShowStarbase;
            case 1:
                if (lppl->fStarbase == 0x0)
                    break;
                goto LShowStarbase;
            case 2:
            case 4:
                GlobalPD.grPopup = grPopupPlanet;
                GlobalPD.idPlanet = sel.pl.id;
                Popup(hwndReportDlg, pt.x, pt.y);
                break;
            case 8:
                if (sel.pl.cDefenses != 0x0) {
                    FGetBestDefensePart(&GlobalPD.part);
                    GlobalPD.grPopup = grPopupComponent;
                    Popup(hwndReportDlg, pt.x, pt.y);
                    break;
                }
                break;
            case 12:
                GlobalPD.grPopup = grPopupResources;
                GlobalPD.idPlanet = sel.pl.id;
                t_call_7e75 = CResourcesAtPlanet(&sel.pl, idPlayer);
                GlobalPD.iPlanVal = t_call_7e75;
                GlobalPD.iPlanetVar = t_call_7e75;
                if (sel.pl.fNoResearch == 0x0) {
                    GlobalPD.iPlanVal = GlobalPD.iPlanVal - MulDiv(GlobalPD.iPlanetVar, (int16_t)rgplr[idPlayer].pctResearch, 100);
                }
                Popup(hwndReportDlg, pt.x, pt.y);
                break;
            case 9:
            case 10:
            case 11:
                xCur = 2;
                i = 0;
                ibit = 1;
                while (i < icol) {
                    if (((int32_t)ibit & vprptCur->grbitVisible) != 0x0 && (i == 0 || i >= vprptCur->cFieldFirst)) {
                        xCur = xCur + vprptCur->rgbdx[i] * 2;
                    }
                    i = i + 1;
                    ibit = ibit * 2;
                }
                dxOffset = pt.x - xCur;
                for (i = 1; i <= 3 && (int32_t)(i * 2 * vprptCur->rgbdx[icol]) / 0x3 <= dxOffset; i++) {
                }
                i = i - 1;
                GlobalPD.grPopup = grPopupMineral;
                GlobalPD.rgi[0] = (int32_t)i;
                GlobalPD.rgi[2] = sel.pl.rgwtMin[i];
                GlobalPD.rgi[3] = (uint32_t)sel.pl.rgMinConc[i];
                EstMineralsMined(&sel.pl, rglQuan, -1, 0);
                GlobalPD.rgi[4] = rglQuan[i];
                GlobalPD.rgi[1] = sel.pl.fHomeworld;
                Popup(hwndReportDlg, pt.x, pt.y);
                break;
            case 5:
                if (hwndProdDlg != 0x0)
                    break;
                ChangeProduction(0);
                break;
            case 6:
            case 7:
                GlobalPD.grPopup = grPopupPlanetIndustry;
                GlobalPD.idPlan = sel.pl.id;
                GlobalPD.fFactory = icol == 7 ? 1 : 0;
                if (GlobalPD.fFactory == 0) {
                    GlobalPD.cMax = CMaxMines(&sel.pl, idPlayer);
                    GlobalPD.cCur = sel.pl.cMines;
                    GlobalPD.cOperate = CMaxOperableMines(&sel.pl, idPlayer, 0);
                } else {
                    GlobalPD.cMax = CMaxFactories(&sel.pl, idPlayer);
                    GlobalPD.cCur = sel.pl.cFactories;
                    GlobalPD.cOperate = CMaxOperableFactories(&sel.pl, idPlayer, 0);
                }
                Popup(hwndReportDlg, pt.x, pt.y);
            case 3:
            }
            break;
        LShowStarbase:
            GlobalPD.grPopup = grPopupShdef;
            GlobalPD.lpshdef = rglpshdefSB[idPlayer] + sel.pl.isb;
            GlobalPD.fHideCounts = 0;
            GlobalPD.fShowDamage = 1;
            GlobalPD.fToken = 0;
            GlobalPD.fSummary = 0;
            Popup(hwndReportDlg, pt.x, pt.y);
            break;
        }
        MessageBeep(0x0);
        break;
    case 1:
        lpfl = rglpfl[vlprgidFleet[irow]];
        if (mdXferDlg == 0xffff) {
            SelectAdjFleet(0, lpfl->id);
            InvalidateReport(1, 0);
            if ((uint16_t)(icol - 3) > 5)
                break;
            switch (icol) {
            case 6:
            case 7:
                if (mdXferDlg != 0xffff)
                    break;
                if (sel.fl.idPlanet == -1) {
                    TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
                    break;
                }
                TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
                break;
            case 3:
            case 4:
            case 5:
                if (FDestIsWP0(lpfl) != 0)
                    break;
                SendMessage(hwndShipLB, LB_SETCURSEL, 0x1, 0);
                SetScanWp(1);
                break;
            case 8:
                GlobalPD.grPopup = grPopupFleet;
                GlobalPD.fRedDamage = 1;
                GlobalPD.grbit = 0xff;
                GlobalPD.lpfl = lpfl;
                Popup(hwndReportDlg, pt.x, pt.y);
            }
            break;
        }
        MessageBeep(0x0);
        break;
    case 3:
        lpbd = BtlDataGet(vlprgidMisc[irow]);
        if (lpbd != 0x0) {
            if (lpbd->pt.x != sel.scan.pt.x || lpbd->pt.y != sel.scan.pt.y) {
                scan.pt = lpbd->pt;
                scan.grobj = 0x8f;
                ChangeScanSel(&scan, 0);
                CtrPointScan(scan.pt, 1);
                InvalidateReport(0, 0);
                if (lpbd->pt.x == sel.scan.pt.x && lpbd->pt.y == sel.scan.pt.y)
                    break;
            }
            if (hwndVCRDlg != 0x0)
                break;
            BattleVCR(lpbd->id);
            break;
        }
        break;
    case 2:
        if (vprptCur == &vrptEFleet && irow >= -2) {
            if (irow < 0) {
                if (irow != -1) {
                    irowEFleetCur = irowEFleetCur - 1;
                } else {
                    irowEFleetCur = irowEFleetCur + 1;
                }
                if (irowEFleetCur < vprptCur->cRows) {
                    if (irowEFleetCur < 0) {
                        irowEFleetCur = vprptCur->cRows - 1;
                    }
                } else {
                    irowEFleetCur = 0;
                }
                irow = irowEFleetCur;
            }
            lpfl = rglpfl[vlprgidMisc[irow]];
            if (mdXferDlg == 0xffff) {
                FFindNearestObject(lpfl->pt, grobjFleet, &scan);
                scan.ifl = vlprgidMisc[irow];
                ChangeScanSel(&scan, 2);
                FEnsurePointOnScreen(lpfl->pt, 1);
                irowEFleetCur = irow;
                InvalidateReport(2, 0);
            } else {
                MessageBeep(0x0);
            }
        }
    default:
    }
    ReleaseDC(hwndReportDlg, hdc);
    return;
}

void DumpUniverse() {
    StringId ids;
    int16_t  i;
    jmp_buf  env;
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav;
    int16_t  cch;

    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    if (game.lid != 0 && idPlayer != -1) {
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) == 0) {
            fFileErrSilent = 1;
            _wsprintf(szWork, "%s.map", szBase);
            StreamOpen(szWork, 4114);
            fOpen = 1;
            RgToStream("#\tX\tY\tName\r\n", 0xc);
            for (i = 0; i < game.cPlanMax; i++) {
                cch = _wsprintf(szWork, "%d\t%d\t%d\t%s\r\n", i + 1, rgptPlan[i].x, rgptPlan[i].y, PszGetCompressedPlanet(rgidPlan[i]));
                RgToStream(szWork, cch);
            }
            StreamClose();
        } else {
            if (fOpen != 0) {
                StreamClose();
            }
            fFileErrSilent = fSilentSav;
            fSuccess = 0;
            penvMem = penvMemSav;
        }
    } else {
        fSuccess = 0;
    }
    ids = fSuccess == 0 ? idsUnableWriteUniverseDefinitionSMapOperation : idsUniverseDefinitionHasSuccessfullyWrittenSMap;
    _wsprintf(szWork, PszGetCompressedString(ids), szBase);
    if (fSuccess == 0) {
        AlertSz(szWork, MB_ICONHAND);
    } else {
        AlertSz(szWork, MB_ICONASTERISK);
    }
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

void DumpPlanets() {
    PLANET  *lpplMac;
    StringId ids;
    PLANET  *lppl;
    char     szFile[256];
    char     szForm[256];
    int16_t  j;
    int16_t  i;
    jmp_buf  env;
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav;
    char    *psz;
    int16_t  cch;
    int32_t  l;
    float    pct;
    int32_t  rgl[4];
    PART     part;

    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    if (game.lid != 0 && idPlayer != -1) {
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) == 0) {
            fFileErrSilent = 1;
            if (gd.fPerPlayerDumps == 0x0) {
                _wsprintf(szFile, "%s.pla", szBase);
            } else {
                _wsprintf(szFile, "%s.p%d", szBase, idPlayer + 1);
            }
            StreamOpen(szFile, 4114);
            fOpen = 1;
            j = gd.fPerPlayerDumps + 2;
            for (i = 0; i < j; i++) {
                cch = CchGetString(i + 1244, szForm);
                for (psz = szForm; (int16_t)*psz != 0; psz++) {
                    if ((int16_t)*psz == '*') {
                        *psz = 9;
                    }
                }
                RgToStream(szForm, cch);
                if (i == j - 1) {
                    RgToStream(szCRLF, 0x2);
                }
            }
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                strcpy(szForm, PszGetCompressedPlanet(rgidPlan[lppl->id]));
                RgToStream(szForm, strlen(szForm));
                szForm[0] = 9;
                if (lppl->iPlayer != -1) {
                    strcpy(&szForm[1], PszPlayerName(lppl->iPlayer, 1, 0, 0, 0, 0x0));
                    RgToStream(szForm, strlen(szForm));
                } else {
                    RgToStream(szForm, 0x1);
                }
                if (lppl->iPlayer != -1 && lppl->fStarbase != 0x0) {
                    fstrcpy(&szForm[1], rglpshdefSB[lppl->iPlayer][lppl->isb].hul.szClass);
                    cch = strlen(szForm);
                } else {
                    cch = 1;
                }
                RgToStream(szForm, cch);
                itoa(game.turn - lppl->turn, &szForm[1], 10);
                RgToStream(szForm, strlen(szForm));
                szForm[1] = 0;
                if (lppl->det != 0x7) {
                    if (lppl->iPlayer != -1 && lppl->det >= 0x3) {
                        l = (uint32_t)(lppl->uPopGuess * 0x190);
                        strcpy(&szForm[1], PszFromLong(l, 0x0));
                    }
                } else {
                    strcpy(&szForm[1], PszFromLong((uint32_t)(lppl->rgwtMin[3] * 100), 0x0));
                }
                RgToStream(szForm, strlen(szForm));
                if (lppl->det >= 0x3) {
                    i = PctPlanetDesirability(lppl, idPlayer);
                    _wsprintf(&szForm[1], PCTDPCTPCT, i);
                } else {
                    szForm[1] = 0;
                }
                RgToStream(szForm, strlen(szForm));
                szForm[1] = 0;
                if (lppl->det == 0x7) {
                    FillPlanetProdLB(0x0, 0x0, lppl);
                    strcpy(&szForm[1], szWork);
                }
                RgToStream(szForm, strlen(szForm));
                if (lppl->det != 0x7) {
                    szForm[2] = 9;
                    szForm[1] = 9;
                    szForm[3] = 0;
                    if (gd.fPerPlayerDumps != 0x0 && lppl->uDefGuess != 0x0) {
                        cch = _wsprintf(&szForm[3], "%d%%", lppl->uDefGuess * 0x6 + 0x3);
                        szForm[cch + 3] = 0;
                    }
                } else {
                    CalcPctSurvive(lppl, &pct, 0x0);
                    pct = 1.0 - pct;
                    _wsprintf(&szForm[1], "%ld\t%ld\t%d.%d%%", lppl->cMines, 0x0, lppl->cFactories, 0x0, LOWORD((int32_t)(pct * 100.0)),
                              LOWORD((int32_t)((pct - (double)(int32_t)LOWORD((int32_t)(pct * 100.0)) / 100.0) * 10000.0)));
                }
                RgToStream(szForm, strlen(szForm));
                szForm[1] = 0;
                for (i = 0; i < 3; i++) {
                    if (lppl->det >= 0x3) {
                        strcpy(&szForm[1], PszFromLong(lppl->rgwtMin[i], 0x0));
                    }
                    RgToStream(szForm, strlen(szForm));
                }
                szForm[1] = 0;
                for (i = 0; i < 3; i++) {
                    if (lppl->det >= 0x4) {
                        EstMineralsMined(lppl, rgl, -1, 0);
                        strcpy(&szForm[1], PszFromLong(rgl[i], 0x0));
                    }
                    RgToStream(szForm, strlen(szForm));
                }
                szForm[1] = 0;
                for (i = 0; i < 3; i++) {
                    if (lppl->det >= 0x3) {
                        strcpy(&szForm[1], PszFromInt(lppl->rgMinConc[i], 0x0));
                    }
                    RgToStream(szForm, strlen(szForm));
                }
                if (lppl->det != 0x7) {
                    szForm[1] = 0;
                } else {
                    strcpy(&szForm[1], PszFromInt(CResourcesAtPlanet(lppl, idPlayer), 0x0));
                }
                RgToStream(szForm, strlen(szForm));
                if (gd.fPerPlayerDumps != 0x0) {
                    if (lppl->det < 0x3) {
                        szForm[1] = 0;
                        for (i = 0; i < 7; i++) {
                            RgToStream(szForm, 0x1);
                        }
                    } else {
                        for (i = 0; i < 3; i++) {
                            strcpy(&szForm[1], PszCalcEnvVar(i, (int16_t)lppl->rgEnvVar[i]));
                            RgToStream(szForm, strlen(szForm));
                        }
                        for (i = 0; i < 3; i++) {
                            strcpy(&szForm[1], PszCalcEnvVar(i, (int16_t)lppl->rgEnvVarOrig[i]));
                            RgToStream(szForm, strlen(szForm));
                        }
                        strcpy(&szForm[1], PszFromInt(PctPlanetOptValue(lppl, idPlayer), 0x0));
                        strcat(&szForm[1], "%");
                        RgToStream(szForm, strlen(szForm));
                    }
                    if (lppl->det == 0x7) {
                        strcpy(&szForm[1], PszFromInt(PctPlanetCapacity(lppl), 0x0));
                        RgToStream(szForm, strlen(szForm));
                        strcpy(&szForm[1], PszFromInt(GetPlanetScannerRange(lppl, &i), 0x0));
                        RgToStream(szForm, strlen(szForm));
                        strcpy(&szForm[1], PszFromInt(i, 0x0));
                        RgToStream(szForm, strlen(szForm));
                        if (lppl->idFling != 0x0) {
                            i = lppl->iWarpFling + 4;
                            strcpy(&szForm[1], PszGetPlanetName(lppl->idFling - 1));
                        } else {
                            i = 0;
                            szForm[1] = 0;
                        }
                        RgToStream(szForm, strlen(szForm));
                        strcpy(&szForm[1], PszFromInt(i, 0x0));
                        RgToStream(szForm, strlen(szForm));
                        if (lppl->idRoute != 0x0) {
                            strcpy(&szForm[1], PszGetPlanetName(lppl->idRoute - 1));
                        } else {
                            szForm[1] = 0;
                        }
                        RgToStream(szForm, strlen(szForm));
                        if (lppl->fStarbase == 0x0) {
                            i = -1;
                        } else {
                            i = IStargateFromLppl(lppl);
                            if (i != -1) {
                                part.hs.grhst = hstSpecialSB;
                                part.hs.iItem = i;
                                FLookupPart(&part);
                                strcpy(&szForm[1], PszFromInt(part.pspecialsb->grAbility2, 0x0));
                                RgToStream(szForm, strlen(szForm));
                                strcpy(&szForm[1], PszFromInt(part.pspecialsb->grAbility, 0x0));
                                RgToStream(szForm, strlen(szForm));
                            }
                        }
                        if (i == -1) {
                            RgToStream("\t0\t0", 0x4);
                        }
                        if (lppl->fStarbase == 0x0) {
                            i = 0;
                        } else {
                            i = lppl->pctDp;
                        }
                        strcpy(&szForm[1], PszFromInt(i, 0x0));
                        RgToStream(szForm, strlen(szForm));
                    }
                }
                RgToStream(szCRLF, 0x2);
            }
            StreamClose();
        } else {
            if (fOpen != 0) {
                StreamClose();
            }
            fFileErrSilent = fSilentSav;
            fSuccess = 0;
            penvMem = penvMemSav;
        }
    } else {
        fSuccess = 0;
    }
    ids = fSuccess == 0 ? idsUnableWritePlanetInformationSOperationTerminated : idsKnownPlanetInformationHasSuccessfullyWrittenS;
    _wsprintf(szWork, PszGetCompressedString(ids), szFile);
    if (fSuccess == 0) {
        AlertSz(szWork, MB_ICONHAND);
    } else {
        AlertSz(szWork, MB_ICONASTERISK);
    }
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

void DumpFleets() {
    int16_t  iplr;
    StringId ids;
    char     szFile[256];
    char     szForm[256];
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  j;
    int16_t  i;
    jmp_buf  env;
    int16_t  fOpen;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    jmp_buf *penvMemSav;
    char    *psz;
    int16_t  cch;
    int32_t  l;

    fSilentSav = fFileErrSilent;
    fSuccess = 1;
    fOpen = 0;
    iplr = idPlayer;
    if (game.lid != 0 && idPlayer != -1) {
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) == 0) {
            fFileErrSilent = 1;
            if (gd.fPerPlayerDumps == 0x0) {
                _wsprintf(szFile, "%s.fle", szBase);
            } else {
                _wsprintf(szFile, "%s.f%d", szBase, idPlayer + 1);
            }
            StreamOpen(szFile, 4114);
            fOpen = 1;
            j = gd.fPerPlayerDumps + 2;
            for (i = 0; i < j; i++) {
                cch = CchGetString(i + 1247, szForm);
                for (psz = szForm; (int16_t)*psz != 0; psz++) {
                    if ((int16_t)*psz == '*') {
                        *psz = 9;
                    }
                }
                RgToStream(szForm, cch);
                if (i == j - 1) {
                    RgToStream(szCRLF, 0x2);
                }
            }
            iplr = idPlayer;
            for (ifl = 0; ifl < cFleet; ifl++) {
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    break;
                idPlayer = -1;
                psz = PszGetFleetName(lpfl->id);
                idPlayer = iplr;
                RgToStream(psz, strlen(psz));
                szForm[0] = 9;
                strcpy(&szForm[1], PszFromInt(lpfl->pt.x, 0x0));
                RgToStream(szForm, strlen(szForm));
                strcpy(&szForm[1], PszFromInt(lpfl->pt.y, 0x0));
                RgToStream(szForm, strlen(szForm));
                if (lpfl->idPlanet != -1) {
                    psz = PszGetPlanetName(lpfl->idPlanet);
                    strcpy(&szForm[1], psz);
                    cch = strlen(psz) + 1;
                } else {
                    cch = 1;
                }
                RgToStream(szForm, cch);
                if (lpfl->det != 0x7) {
                    if (gd.fPerPlayerDumps == 0x0 || lpfl->det >= 0x7 || lpfl->fdirValid == 0x0) {
                        szForm[1] = 0;
                    } else {
                        strcpy(&szForm[1], PszFromInt(lpfl->dirFltX, 0x0));
                        strcat(szForm, ".");
                        strcat(szForm, PszFromInt(lpfl->dirFltY, 0x0));
                    }
                } else {
                    strcpy(&szForm[1], PszGetDestName(lpfl, 0x0));
                }
                RgToStream(szForm, strlen(szForm));
                if (rglpbtlplan[lpfl->iplr] != 0x0) {
                    fstrcpy(&szForm[1], rglpbtlplan[lpfl->iplr][lpfl->iplan].szName);
                } else {
                    szForm[1] = 0;
                }
                RgToStream(szForm, strlen(szForm));
                l = 0;
                for (i = 0; i < 16; i++) {
                    l = l + (int32_t)lpfl->rgcsh[i];
                }
                strcpy(&szForm[1], PszFromLong(l, 0x0));
                RgToStream(szForm, strlen(szForm));
                for (i = 0; i < 5; i++) {
                    strcpy(&szForm[1], PszFromLong(lpfl->rgwtMin[i], 0x0));
                    RgToStream(szForm, strlen(szForm));
                }
                if (gd.fPerPlayerDumps != 0x0) {
                    strcpy(&szForm[1], PszFromInt(lpfl->iPlayer + 1, 0x0));
                    RgToStream(szForm, strlen(szForm));
                    if (lpfl->cord <= 1) {
                        szForm[1] = '0';
                        szForm[2] = 0;
                    } else {
                        strcpy(&szForm[1], PszGetETA(0x0, lpfl, 0x0));
                    }
                    RgToStream(szForm, strlen(szForm));
                    if (lpfl->cord >= 2) {
                        i = lpfl->lpplord->rgord[1].iWarp;
                    } else if (lpfl->det >= 0x7 || lpfl->fdirValid == 0x0) {
                        i = 0;
                    } else {
                        i = lpfl->iwarpFlt;
                    }
                    strcpy(&szForm[1], PszFromInt(i, 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromLong(WtFromLpfl(lpfl), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromInt(PctCloakFromLpfl(lpfl), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    j = GetFleetScannerRange(lpfl, &i, 0x0, 0x0);
                    if (j == -1) {
                        j = 0;
                    }
                    strcpy(&szForm[1], PszFromInt(j, 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromInt(i, 0x0));
                    RgToStream(szForm, strlen(szForm));
                    if (lpfl->det != 0x7) {
                        szForm[1] = 0;
                    } else {
                        strcpy(&szForm[1], PszGetTaskName(lpfl, &i));
                    }
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromLong(CMineFromLpfl(lpfl), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromLong(CMineSweepFromLpfl(lpfl), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromLong(CLayMinesFromLpfl(lpfl, -1, -1), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    strcpy(&szForm[1], PszFromLong(PctTerraFromLpfl(lpfl), 0x0));
                    RgToStream(szForm, strlen(szForm));
                    l = 0;
                    for (i = 0; i < 16; i++) {
                        if (lpfl->rgcsh[i] != 0) {
                            j = LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef)->imdCategory;
                            if (j <= 1 || j >= 6) {
                                l = l + (int32_t)lpfl->rgcsh[i];
                            }
                        }
                    }
                    strcpy(&szForm[1], PszFromLong(l, 0x0));
                    RgToStream(szForm, strlen(szForm));
                    for (j = 2; j < 6; j++) {
                        l = 0;
                        for (i = 0; i < 16; i++) {
                            if (lpfl->rgcsh[i] != 0 && LphuldefFromId(rglpshdef[lpfl->iPlayer][i].hul.ihuldef)->imdCategory == j) {
                                l = l + (int32_t)lpfl->rgcsh[i];
                            }
                        }
                        strcpy(&szForm[1], PszFromLong(l, 0x0));
                        RgToStream(szForm, strlen(szForm));
                    }
                }
                RgToStream(szCRLF, 0x2);
            }
            StreamClose();
        } else {
            if (fOpen != 0) {
                StreamClose();
            }
            fFileErrSilent = fSilentSav;
            fSuccess = 0;
            penvMem = penvMemSav;
        }
    } else {
        fSuccess = 0;
    }
    ids = fSuccess == 0 ? idsUnableWriteFleetInformationSOperationTerminated : idsKnownFleetInformationHasSuccessfullyWrittenS;
    _wsprintf(szWork, PszGetCompressedString(ids), szFile);
    if (fSuccess == 0) {
        AlertSz(szWork, MB_ICONHAND);
    } else {
        AlertSz(szWork, MB_ICONASTERISK);
    }
    fFileErrSilent = fSilentSav;
    penvMem = penvMemSav;
    return;
}

INT_PTR CALLBACK PrintMapDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int16_t i;
    RECT    rc;
    HWND    hwndEdit;

    if (msg == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(msg) != 0) {
        if (HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (msg == WM_INITDIALOG) {
            for (i = 0; i < 2; i++) {
                hwndEdit = GetDlgItem(hwnd, i + 268);
                SendMessage(hwndEdit, EM_LIMITTEXT, 0x1, 0);
                SendMessage(hwndEdit, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
                szWork[0] = LOBYTE(vrgcPrintMapPage[i] + 48);
                szWork[1] = 0;
                SetWindowText(hwndEdit, szWork);
            }
            StickyDlgPos(hwnd, &ptStickyPrintMapDlg, 1);
            return 1;
        }
        if (msg == WM_COMMAND) {
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
            case IDCANCEL:
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                    i = 0;
                    while (1) {
                        if (i >= 2)
                            goto L_a382;
                        hwndEdit = GetDlgItem(hwnd, i + 268);
                        GetWindowText(hwndEdit, szWork, 10);
                        if ((int16_t)szWork[0] == 0 || (int16_t)szWork[1] != 0 || (int16_t)szWork[0] <= '0' || (int16_t)szWork[0] > '9')
                            break;
                        vrgcPrintMapPage[i] = (int16_t)szWork[0] - 48;
                        i = i + 1;
                    }
                    AlertSz(PszFormatIds(idsMustSpecifyNumberBetween19, 0x0), MB_ICONHAND);
                    SetFocus(hwndEdit);
                }
            L_a382:
                StickyDlgPos(hwnd, &ptStickyPrintMapDlg, 0);
                EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                return 1;
            case IDC_HELP:
                WinHelp(hwnd, szHelpFile, 0x1, 0xc3c);
                return 1;
            case IDC_EDIT1:
            case IDC_U16_0x010D:
                if (GET_WM_COMMAND_CMD(wParam, lParam) == 0x400) {
                    GetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), szWork, 10);
                    if ((int16_t)szWork[0] != 0 && ((int16_t)szWork[0] <= '0' || (int16_t)szWork[0] > '9')) {
                        MessageBeep(0x0);
                        SetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), &szWork[1]);
                    }
                }
            default:
            }
        }
    }
    return 0;
}
