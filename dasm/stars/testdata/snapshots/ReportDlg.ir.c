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

L_0018:
    goto L_0965;

L_0027:
    hwndReportDlg = hwnd;
    hdc = GetDC(hwnd);
    SelectObject(hdc, rghfontArial8[1]);
    i = 0;
    goto L_0050;

L_004c:
    i++;

L_0050:
    if (i >= vprptCur->cFields)
        goto L_0097;
    else
        goto L_005f;

L_005f:
    dx = DxReportColHdr(vprptCur->irpt, i, szWork, hdc);
    vprptCur->rgbdx[i] = LOBYTE(dx / 2);
    goto L_004c;

L_0097:
    ReleaseDC(hwnd, hdc);
    SortReportCache(vprptCur->irpt, vprptCur->icolSort);
    SetWindowPos(hwnd, NULL, 0, 0, vprptCur->ptSize.x, vprptCur->ptSize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
    StickyDlgPos(hwnd, &vprptCur->ptDlg, 1);
    vprptCur->hwndVScroll = CreateWindow("SCROLLBAR", NULL, SBS_VERT | WS_CHILD, 0, 0, 50, 50, hwnd, NULL, hInst, NULL);
    vprptCur->hwndHScroll = CreateWindow("SCROLLBAR", NULL, WS_CHILD, 0, 0, 50, 50, hwnd, NULL, hInst, NULL);
    if (gd.fTutorial == 0)
        goto L_019b;
    else
        goto L_0196;

L_0196:
    AdvanceTutor();

L_019b:
    GetClientRect(hwnd, &rc);
    cRow = (int16_t)(rc.bottom - 36) / (dyArial8 + 4);
    vprptCur->cRowsVis = cRow >= vprptCur->cRows ? vprptCur->cRows : cRow;
    if (vprptCur->cRowsVis < vprptCur->cRows)
        goto L_021c;
    else
        goto L_01f3;

L_01f3:
    swp = 132;
    vprptCur->irowFirst = 0;
    SetScrollPos(vprptCur->hwndVScroll, 2, 0, 0);
    goto L_02b5;

L_021c:
    swp = 68;
    if (vprptCur->irowFirst + vprptCur->cRowsVis <= vprptCur->cRows)
        goto L_0273;
    else
        goto L_023b;

L_023b:
    if (vprptCur->irowFirst <= 0)
        goto L_0273;
    else
        goto L_0248;

L_0248:
    vprptCur->irowFirst = vprptCur->cRows - vprptCur->cRowsVis;
    if (vprptCur->irowFirst >= 0)
        goto L_0273;
    else
        goto L_026a;

L_026a:
    vprptCur->irowFirst = 0;

L_0273:
    SetScrollPos(vprptCur->hwndVScroll, 2, vprptCur->irowFirst, 0);
    SetScrollRange(vprptCur->hwndVScroll, 2, 0, vprptCur->cRows - vprptCur->cRowsVis, 1);

L_02b5:
    dx = GetSystemMetrics(SM_CXVSCROLL);
    SetWindowPos(vprptCur->hwndVScroll, NULL, rc.right - dx, dyArial8 + 6, dx, (dyArial8 + 4) * vprptCur->cRowsVis + 1, swp);
    SetHScrollBar();
    if (msg != WM_CREATE)
        goto L_030b;
    else
        goto L_0304;

L_0304:
    return 1;

L_030b:

L_030f:
    return 0;

L_0312:
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x = 300;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y = 220;
    return 0;

L_0337:
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;

L_0362:
    xCur = 2;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);
    if (pt.y < 2)
        goto L_09c8;
    else
        goto L_0390;

L_0390:
    if (pt.x < 2)
        goto L_09c8;
    else
        goto L_039c;

L_039c:
    if (pt.y >= dyArial8 + 6)
        goto L_03b2;
    else
        goto L_03aa;

L_03aa:
    iRow = -1;
    goto L_03f1;

L_03b2:
    iRow = (int16_t)(pt.y - 2 - (dyArial8 + 4)) / (dyArial8 + 4);
    if (iRow >= vprptCur->cRowsVis)
        goto L_09c8;
    else
        goto L_03e7;

L_03e7:
    iRow += vprptCur->irowFirst;

L_03f1:
    iCol = -1;
    i = 0;
    ibit = 1;
    goto L_0416;

L_0406:
    i++;
    ibit *= 2;

L_0416:
    if (i >= vprptCur->cFields)
        goto L_0489;
    else
        goto L_0425;

L_0425:
    if ((ibit & vprptCur->grbitVisible) != 0)
        goto L_0442;
    else
        goto L_0406;

L_0442:
    if (i == 0)
        goto L_045a;
    else
        goto L_044b;

L_044b:
    if (i < vprptCur->cFieldFirst)
        goto L_0406;
    else
        goto L_045a;

L_045a:
    xCur += vprptCur->rgbdx[i] * 2;
    if (xCur <= pt.x)
        goto L_0406;
    else
        goto L_047d;

L_047d:
    iCol = i;

L_0489:
    if (iCol == -1)
        goto L_09c8;
    else
        goto L_0495;

L_0495:
    if (iRow != -1)
        goto L_04c6;
    else
        goto L_049e;

L_049e:
    ReportColumnPopup(pt, iCol, msg == WM_RBUTTONDOWN ? 1 : 0);
    goto L_04e1;

L_04c6:
    ExecuteReportClick(pt, vprptCur->irpt, iCol, iRow);

L_04e1:
    if (gd.fTutorial == 0)
        goto L_09c8;
    else
        goto L_04f4;

L_04f4:
    AdvanceTutor();

L_04f9:
    goto L_09c8;

L_04fc:
    iCur = GetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), 2);
    iNew = iCur;
    goto L_0576;

L_0529:
    iNew = 2000;
    goto L_0597;

L_0531:
    iNew++;
    goto L_0597;

L_0538:
    iNew--;
    goto L_0597;

L_053f:
    iNew += vprptCur->cRowsVis - 1;
    goto L_0597;

L_054f:
    iNew -= vprptCur->cRowsVis - 1;
    goto L_0597;

L_055f:
    iNew = GET_WM_VSCROLL_POS(wParam, lParam);
    goto L_0597;

L_056b:
    iNew = 0;
    goto L_0597;

L_0576:
    if (GET_WM_VSCROLL_CODE(wParam, lParam) > SB_BOTTOM)
        goto L_0597;
    else
        goto L_057e;

L_057e:
    switch (GET_WM_VSCROLL_CODE(wParam, lParam) * 2) {
    case 0x0:
        goto L_0538;
    case 0x2:
        goto L_0531;
    case 0x4:
        goto L_054f;
    case 0x6:
        goto L_053f;
    case 0x8:
        goto L_055f;
    case 0xa:
        goto L_055f;
    case 0xc:
        goto L_056b;
    case 0xe:
        goto L_0529;
    }

L_0597:
    if (iNew <= vprptCur->cRows - vprptCur->cRowsVis)
        goto L_05be;
    else
        goto L_05ad;

L_05ad:
    iNew = vprptCur->cRows - vprptCur->cRowsVis;

L_05be:
    if (iNew >= 0)
        goto L_05cc;
    else
        goto L_05c7;

L_05c7:
    iNew = 0;

L_05cc:
    if (iNew == iCur)
        goto L_0673;
    else
        goto L_05d7;

L_05d7:
    vprptCur->irowFirst = iNew;
    GetClientRect(hwnd, &rc);
    rc.left = 2;
    rc.right -= GetSystemMetrics(SM_CXVSCROLL);
    rc.top = dyArial8 + 6;
    rc.bottom = (dyArial8 + 4) * vprptCur->cRowsVis + rc.top;
    ScrollWindow(hwnd, 0, (dyArial8 + 4) * (iCur - iNew), &rc, &rc);
    SetScrollPos(GET_WM_VSCROLL_HWND(wParam, lParam), 2, iNew, 1);
    UpdateWindow(hwnd);

L_0673:
    return 0;

L_067c:
    iCur = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2);
    iNew = iCur;
    goto L_06e4;

L_06a9:
    iNew = 2000;
    goto L_0705;

L_06b1:
    iNew++;
    goto L_0705;

L_06b8:
    iNew--;
    goto L_0705;

L_06bf:
    iNew += 3;
    goto L_0705;

L_06c6:
    iNew -= 3;
    goto L_0705;

L_06cd:
    iNew = GET_WM_HSCROLL_POS(wParam, lParam);
    goto L_0705;

L_06d9:
    iNew = 0;
    goto L_0705;

L_06e4:
    if (GET_WM_HSCROLL_CODE(wParam, lParam) > SB_BOTTOM)
        goto L_0705;
    else
        goto L_06ec;

L_06ec:
    switch (GET_WM_HSCROLL_CODE(wParam, lParam) * 2) {
    case 0x0:
        goto L_06b8;
    case 0x2:
        goto L_06b1;
    case 0x4:
        goto L_06c6;
    case 0x6:
        goto L_06bf;
    case 0x8:
        goto L_06cd;
    case 0xa:
        goto L_06cd;
    case 0xc:
        goto L_06d9;
    case 0xe:
        goto L_06a9;
    }

L_0705:
    if (iNew <= vprptCur->cColScroll)
        goto L_071e;
    else
        goto L_0714;

L_0714:
    iNew = vprptCur->cColScroll;

L_071e:
    if (iNew >= 0)
        goto L_072c;
    else
        goto L_0727;

L_0727:
    iNew = 0;

L_072c:
    if (iNew == iCur)
        goto L_080f;
    else
        goto L_0737;

L_0737:
    SetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2, iNew, 1);
    iNew = GetScrollPos(GET_WM_HSCROLL_HWND(wParam, lParam), 2);
    if (iNew == iCur)
        goto L_080f;
    else
        goto L_0788;

L_0788:
    i = 1;
    ibit = 2;
    goto L_07a8;

L_0798:
    i++;
    ibit *= 2;

L_07a8:
    if (i >= vprptCur->cFields)
        goto L_07e9;
    else
        goto L_07b7;

L_07b7:
    if ((ibit & vprptCur->grbitVisible) != 0)
        goto L_07d4;
    else
        goto L_0798;

L_07d4:
    if (iNew-- > 0)
        goto L_0798;
    else
        goto L_07e9;

L_07e9:
    vprptCur->cFieldFirst = i;
    InvalidateRect(hwnd, NULL, 1);
    UpdateWindow(hwnd);

L_080f:
    return 0;

L_0818:
    hdc = BeginPaint(hwnd, &ps);
    DrawReport(hwnd, hdc, &ps.rcPaint);
    EndPaint(hwnd, &ps);
    gd.fRptSafeDraw = 0;
    return 1;

L_0860:
    StickyDlgPos(hwnd, &vprptCur->ptDlg, 0);
    GetWindowRect(hwnd, &rc);
    vprptCur->ptSize.x = rc.right - rc.left;
    vprptCur->ptSize.y = rc.bottom - rc.top;
    hwndReportDlg = 0;
    fBrowserValid = 0;
    hmenu = GetASubMenu(hwndFrame, 4);
    goto L_08f1;

L_08ce:
    idm = 0x8ff;
    goto L_0914;

L_08d6:
    idm = 0x900;
    goto L_0914;

L_08de:
    idm = 0x8fd;
    goto L_0914;

L_08e6:
    idm = 0x901;
    goto L_0914;

L_08f1:
    if (vprptCur->irpt == 0)
        goto L_08de;
    else
        goto L_08f9;

L_08f9:
    if (vprptCur->irpt == 1)
        goto L_08ce;
    else
        goto L_0901;

L_0901:
    if (vprptCur->irpt == 2)
        goto L_08d6;
    else
        goto L_0909;

L_0909:
    if (vprptCur->irpt == 3)
        goto L_08e6;
    else
        goto L_0914;

L_0914:
    CheckMenuItem(hmenu, idm, 0);
    vprptCur = 0;
    if (gd.fTutorial == 0)
        goto L_09c8;
    else
        goto L_093d;

L_093d:
    AdvanceTutor();

L_0942:
    goto L_09c8;

L_0945:
    if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL)
        goto L_09c8;
    else
        goto L_094e;

L_094e:
    DestroyWindow(hwnd);
    return 1;

L_0965:
    if (msg == WM_CREATE)
        goto L_0027;
    else
        goto L_096d;

L_096d:
    if (msg == WM_DESTROY)
        goto L_0860;
    else
        goto L_0975;

L_0975:
    if (msg == WM_SIZE)
        goto L_019b;
    else
        goto L_097d;

L_097d:
    if (msg == WM_PAINT)
        goto L_0818;
    else
        goto L_0985;

L_0985:
    if (msg == WM_ERASEBKGND)
        goto L_0337;
    else
        goto L_098d;

L_098d:
    if (msg == WM_GETMINMAXINFO)
        goto L_0312;
    else
        goto L_0995;

L_0995:
    if (msg == WM_COMMAND)
        goto L_0945;
    else
        goto L_099d;

L_099d:
    if (msg == WM_HSCROLL)
        goto L_067c;
    else
        goto L_09a5;

L_09a5:
    if (msg == WM_VSCROLL)
        goto L_04fc;
    else
        goto L_09ad;

L_09ad:
    if (msg == WM_LBUTTONDOWN)
        goto L_0362;
    else
        goto L_09b5;

L_09b5:
    if (msg == WM_LBUTTONDBLCLK)
        goto L_0362;
    else
        goto L_09bd;

L_09bd:
    if (msg == WM_RBUTTONDOWN)
        goto L_0362;
    else
        goto L_09c8;

L_09c8:
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
