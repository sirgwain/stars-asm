LRESULT CALLBACK MessageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     i;
    char       *psz;
    PAINTSTRUCT ps;
    int16_t     dy;
    int16_t     dx;
    RECT        rc;
    POINT16     pt;
    HCURSOR     hcs;
    HtMsgType   ht;
    int16_t     fSet;
    MessageId   idm;
    MSGPLR     *lpmp;
    MSGPLR     *lpmpSrc;
    char       *lpsz;
    HBRUSH      hbrSav;
    COLORREF    crFore;
    int16_t     dxMax;
    COLORREF    crBack;
    RECT        rcActual;
    int16_t     cch;
    int16_t     iMode;
    char        szT[32];
    MSGPLR     *lpmsgplr;
    THING      *lpth;
    SCAN        scan;
    FARPROC     lpProc;
    int16_t     fRet;
    int32_t     lSerial;
    POINT       t_pt_6090;
    POINT       t_pt_609f_1;
    uint16_t    t_scratch_m30;
    int16_t     t_62dd;
    char       *t_merge_66fb_0001;
    uint16_t    t_scratch_m74_2;

L_5c92:
    goto L_71a4;

L_5ca1:
    i = 0;
    goto L_5d3c;

L_5ca9:
    hwndMessage = hwnd;
    rghwndMsgBtn[i] =
        CreateWindow("BUTTON", PszGetCompressedString(i + 1356), WS_CHILD, 100, 100, i == 3 ? 50 : 44, (3 * dyArial8 >> 1) - 1, hwnd, NULL, hInst, NULL);
    SendMessage(rghwndMsgBtn[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
    i++;

L_5d3c:
    if (i < 4)
        goto L_5ca9;
    else
        goto L_5d45;

L_5d45:
    hwndMsgDrop = CreateWindow("COMBOBOX", "MsgDD", CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd, NULL, hInst, NULL);
    SendMessage(hwndMsgDrop, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
    hwndMsgEdit = CreateWindow("EDIT", NULL, ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD | WS_BORDER, 100, 100, 200, 50, hwnd, NULL, hInst, NULL);
    SendMessage(hwndMsgEdit, EM_LIMITTEXT, 0x3c8, 0);
    SendMessage(hwndMsgEdit, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
    hwndMsgScroll =
        CreateWindow("EDIT", NULL, ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_CHILD | WS_BORDER | WS_VSCROLL, 100, 100, 200, 50, hwnd, NULL, hInst, NULL);
    SetMsgTitle(hwnd);
    SendMessage(hwndMsgDrop, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsEverybody));
    i = 0;
    goto L_5e8d;

L_5e89:
    i++;

L_5e8d:
    if (i >= game.cPlayer)
        goto L_5ed5;
    else
        goto L_5e98;

L_5e98:
    psz = PszPlayerName(i, 1, 1, 1, 0, NULL);
    SendMessage(hwndMsgDrop, CB_ADDSTRING, 0, (LPARAM)psz);
    goto L_5e89;

L_5ed5:
    SendMessage(hwndMsgDrop, CB_SETCURSEL, 0, 0);
    goto L_7207;

L_5ef1:
    dx = LOWORD(lParam);
    dy = HIWORD(lParam);
    i = 0;
    goto L_5f62;

L_5f19:
    SetWindowPos(rghwndMsgBtn[i], NULL, dx - 48, ((3 * dyArial8 >> 1) + 2) * i + 3 + dyArial8 * 2, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    i++;

L_5f62:
    if (i < 3)
        goto L_5f19;
    else
        goto L_5f6b;

L_5f6b:
    SetRect(&rcMsgText, 4, dyArial8 * 2 + 3, dx - 52, dy - 4);
    SetRect(&rcMsgTitle, 4, 4, dx - 4, dyArial8 * 2 - 4);
    rc = rcMsgText;
    ExpandRc(&rc, -4, -4);
    SetWindowPos(hwndMsgDrop, NULL, rc.left + 30, rc.top, rc.right - rc.left - 84, rc.bottom - rc.top, SWP_NOZORDER);
    SetWindowPos(rghwndMsgBtn[3], NULL, rc.right - 50, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    rc.top += dyShipDD + 3;
    SetWindowPos(hwndMsgEdit, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
    goto Default;

L_6059:
    GetClientRect(hwnd, &rc);
    FillRect((HDC)wParam, &rc, hbrButtonFace);
    return 1;

L_6084:
    hcs = 0;
    GetCursorPos(&t_pt_6090);
    pt = PointTo16(t_pt_6090);
    t_pt_609f_1 = PointFrom16(pt);
    ScreenToClient(hwnd, &t_pt_609f_1);
    pt = PointTo16(t_pt_609f_1);
    if (HtMsgBox(pt) == htMsgNone)
        goto Default;
    else
        goto L_60ba;

L_60ba:
    SetCursor(hcurHand);
    return 1;

L_60cf:
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);
    ht = HtMsgBox(pt);
    if (ht != htMsgCurrent)
        goto L_61c3;
    else
        goto CheckBox;

CheckBox:
    if (iMsgCur < 0)
        goto L_7207;
    else
        goto L_6116;

L_6116:
    idm = IdmGetMessageN(iMsgCur);
    fSet = (bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) == 0 ? 0 : 1;
    SetFilteringGroups(idm, fSet == 0 ? 1 : 0);
    DirtyGame(1);
    if (gd.fTutorial == 0)
        goto L_61a0;
    else
        goto L_619b;

L_619b:
    AdvanceTutor();

L_61a0:
    InvalidateRect(hwndMessage, NULL, 1);
    SetMsgTitle(hwnd);
    goto L_7207;

L_61c3:
    if (ht != htMsgZoom)
        goto L_628f;
    else
        goto ZoomBox;

ZoomBox:
    fViewFilteredMsg = fViewFilteredMsg == 0 ? 1 : 0;
    if (iMsgCur < 0)
        goto L_623f;
    else
        goto L_61ec;

L_61ec:
    t_scratch_m30 = 1 << (IdmGetMessageN(iMsgCur) & 7);
    if (((bitfMsgFiltered[IdmGetMessageN(iMsgCur) >> 3] & t_scratch_m30) == 0 ? 0 : 1) == fViewFilteredMsg)
        goto L_626c;
    else
        goto L_623f;

L_623f:
    i = IMsgNext(fViewFilteredMsg);
    if (i != -1)
        goto L_6266;
    else
        goto L_6257;

L_6257:
    i = IMsgPrev(fViewFilteredMsg);

L_6266:
    iMsgCur = i;

L_626c:
    InvalidateRect(hwndMessage, NULL, 1);
    SetMsgTitle(hwnd);
    goto L_7207;

L_628f:
    if (ht != htMsgMode)
        goto L_7207;
    else
        goto ToggleMsgMode;

ToggleMsgMode:
    if (gd.fSendMsgMode == 0)
        goto L_62ba;
    else
        goto L_62ab;

L_62ab:
    FFinishPlrMsgEntry(0);
    goto L_637a;

L_62ba:
    if (iMsgCur < cMsg)
        goto L_6374;
    else
        goto L_62c6;

L_62c6:
    lpmpSrc = vlpmsgplrIn;
    i = iMsgCur - cMsg;

L_62dd:
    t_62dd = i;
    i--;
    if (t_62dd == 0)
        goto L_62ff;
    else
        goto L_62ec;

L_62ec:
    lpmpSrc = lpmpSrc->lpmsgplrNext;
    goto L_62dd;

L_62ff:
    lpmp = vlpmsgplrOut;
    iMsgSendCur = 0;
    goto L_6352;

L_6315:
    if (lpmp->iPlrTo - 1 != lpmpSrc->iPlrFrom)
        goto L_633d;
    else
        goto L_632b;

L_632b:
    if (lpmp->iInRe == iMsgCur)
        goto L_6364;
    else
        goto L_633d;

L_633d:
    lpmp = lpmp->lpmsgplrNext;
    iMsgSendCur++;

L_6352:
    if (lpmp != 0)
        goto L_6315;
    else
        goto L_6364;

L_6364:
    viInRe = lpmpSrc->iPlrFrom + 1;
    goto L_637a;

L_6374:
    viInRe = 0;

L_637a:
    gd.fSendMsgMode = gd.fSendMsgMode == 0 ? 1 : 0;
    InvalidateRect(hwndMessage, NULL, 1);
    SetMsgTitle(hwnd);
    SetFocus(hwndMsgEdit);

L_63dd:
    goto L_7207;

L_63e0:
    ((MINMAXINFO *)lParam)->ptMinTrackSize.x = dxWinFrame * 2 + 198;
    ((MINMAXINFO *)lParam)->ptMinTrackSize.y = (0xd * dyArial8 >> 1) + 0x16;
    goto Default;

L_640f:
    if (GET_WM_CTLCOLOR_HWND(wParam, lParam) != hwndMsgScroll)
        goto Default;
    else
        goto L_641e;

L_641e:
    SetBkColor((HDC)wParam, crButtonFace);
    return (LRESULT)hbrButtonFace;

L_643a:
    hdc = BeginPaint(hwnd, &ps);
    _Draw3dFrame(hdc, &rcMsgTitle, 0);
    crFore = SetTextColor(hdc, crButtonText);
    crBack = SetBkColor(hdc, crButtonFace);
    cch = strlen(szMsgTitle);
    dxMax = rcMsgTitle.right - rcMsgTitle.left - 48;

L_64a7:
    if (cch <= 0)
        goto L_64d1;
    else
        goto L_64b0;

L_64b0:
    if ((int16_t)LOWORD(GetTextExtent(hdc, szMsgTitle, cch)) <= dxMax)
        goto L_64d1;
    else
        goto L_64ca;

L_64ca:
    cch--;
    goto L_64a7;

L_64d1:
    RcCtrTextOut(hdc, &rcMsgTitle, szMsgTitle, cch);
    DecorateMsgTitleBar(hdc, &rcMsgTitle);
    rc = rcMsgText;
    dx = rc.right - rc.left;
    dy = rc.bottom - rc.top;
    hbrSav = SelectObject(hdc, hbrButtonShadow);
    PatBlt(hdc, rc.left, rc.top, dx, 1, PATCOPY);
    PatBlt(hdc, rc.left, rc.top, 1, dy, PATCOPY);
    SelectObject(hdc, hbrButtonHilite);
    PatBlt(hdc, rc.left, rc.bottom - 1, dx, 1, PATCOPY);
    PatBlt(hdc, rc.right - 1, rc.top, 1, dy, PATCOPY);
    SelectObject(hdc, hbrSav);
    ExpandRc(&rc, -4, -4);
    if (gd.fSendMsgMode != 0)
        goto L_69ee;
    else
        goto L_65e0;

L_65e0:
    if (iMsgCur < cMsg)
        goto L_6796;
    else
        goto L_65ec;

L_65ec:
    lpmsgplr = vlpmsgplrIn;
    i = cMsg;
    goto L_6616;

L_6602:
    lpmsgplr = lpmsgplr->lpmsgplrNext;
    i++;

L_6616:
    if (i < iMsgCur)
        goto L_6602;
    else
        goto L_6621;

L_6621:
    if (CchGetString(idsSCC, szT) >= 32)
        goto L_663f;
    else
        goto L_6639;

L_6639:
    goto L_6642;

L_663f:

L_6642:
    cch = _wsprintf(lpb2k, szT, PszPlayerName(lpmsgplr->iPlrFrom, 1, 1, 1, 0, NULL), 13, 10);
    if (CchGetString(idsSCC2, szT) >= 32)
        goto L_66a9;
    else
        goto L_66a3;

L_66a3:
    goto L_66ac;

L_66a9:

L_66ac:
    if (lpmsgplr->iPlrTo != 0)
        goto L_66d2;
    else
        goto L_66c1;

L_66c1:
    t_merge_66fb_0001 = PszGetCompressedString(idsEverybody);
    goto L_66fb;

L_66d2:
    t_merge_66fb_0001 = PszPlayerName(lpmsgplr->iPlrTo - 1, 1, 1, 1, 0, NULL);

L_66fb:
    cch += _wsprintf(lpb2k + cch, szT, t_merge_66fb_0001, 13, 10);
    if (lpmsgplr->cLen < 0)
        goto L_6762;
    else
        goto L_672b;

L_672b:
    i = 1000;
    FDecompressUserString(lpmsgplr->rgbMsg, lpmsgplr->cLen, lpb2k + cch, &i);
    goto L_6786;

L_6762:
    fstrcpy(lpb2k + cch, lpmsgplr->rgbMsg);

L_6786:
    lpsz = lpb2k;
    goto L_683c;

L_6796:
    idm = IdmGetMessageN(iMsgCur);
    if (iMsgCur >= 0)
        goto L_67d0;
    else
        goto L_67af;

L_67af:
    if (cMsg <= 0)
        goto L_67d0;
    else
        goto L_67b9;

L_67b9:
    lpsz = PszGetCompressedString(idsMessagesHaveSentYearFilteredIfWant);
    goto L_683c;

L_67d0:
    if (iMsgCur < 0)
        goto L_6828;
    else
        goto L_67da;

L_67da:
    if ((bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) == 0)
        goto L_6828;
    else
        goto L_6807;

L_6807:
    if (fViewFilteredMsg != 0)
        goto L_6828;
    else
        goto L_6811;

L_6811:
    lpsz = PszGetCompressedString(idsMessageTypeHasFilteredWillShownDefault);
    goto L_683c;

L_6828:
    lpsz = PszGetMessageN(iMsgCur);

L_683c:
    SetTextColor(hdc, 0xffffff);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if (iMsgCur >= 0)
        goto L_686f;
    else
        goto L_6865;

L_6865:
    if (cMsg > 0)
        goto L_68c6;
    else
        goto L_686f;

L_686f:
    if (iMsgCur < 0)
        goto L_6903;
    else
        goto L_6879;

L_6879:
    if (iMsgCur >= cMsg)
        goto L_6903;
    else
        goto L_6885;

L_6885:
    t_scratch_m74_2 = 1 << (IdmGetMessageN(iMsgCur) & 7);
    if ((bitfMsgFiltered[IdmGetMessageN(iMsgCur) >> 3] & t_scratch_m74_2) == 0)
        goto L_6903;
    else
        goto L_68c6;

L_68c6:
    cch = CchGetString(idsFiltered, szWork);
    DiaganolTextOut(hdc, &rc, szWork, cch);
    lpsz = PszGetMessageN(iMsgCur);

L_6903:
    SetTextColor(hdc, crButtonText);
    rcActual = rc;
    DrawText(hdc, lpsz, fstrlen(lpsz), &rcActual, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);
    if (rcActual.bottom > rc.bottom)
        goto L_6998;
    else
        goto L_6955;

L_6955:
    if (rcActual.right > rc.right)
        goto L_6998;
    else
        goto L_6960;

L_6960:
    ShowWindow(hwndMsgScroll, SW_HIDE);
    DrawText(hdc, lpsz, fstrlen(lpsz), &rc, DT_WORDBREAK | DT_NOPREFIX);
    goto L_69e0;

L_6998:
    SetWindowText(hwndMsgScroll, lpsz);
    ExpandRc(&rc, 4, 4);
    SetWindowPos(hwndMsgScroll, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);

L_69e0:
    SetBkMode(hdc, iMode);
    goto L_6a4b;

L_69ee:
    iMode = SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, crButtonText);
    cch = CchGetString(idsTo3, szT);
    RightTextOut(hdc, rc.left + 26, rc.top, szT, cch, 0);
    SetBkMode(hdc, iMode);

L_6a4b:
    SetTextColor(hdc, crFore);
    SetBkColor(hdc, crBack);
    EndPaint(hwnd, &ps);
    goto L_7207;

L_6a79:
    if (wParam == VK_DOWN)
        goto NextMsg;
    else
        goto L_6a88;

L_6a88:
    if (wParam == VK_UP)
        goto PrevMsg;
    else
        goto L_6a94;

L_6a94:
    if (gd.fSendMsgMode != 0)
        goto Default;
    else
        goto L_6aaa;

L_6aaa:
    if (wParam != VK_HOME)
        goto L_6abf;
    else
        goto L_6ab3;

L_6ab3:
    iMsgCur = -1;
    goto NextMsg;

L_6abf:
    if (wParam != VK_END)
        goto L_7207;
    else
        goto L_6ac8;

L_6ac8:
    iMsgCur = cMsg + vcmsgplrIn;
    goto PrevMsg;

L_6ad8:
    if (wParam == '\r')
        goto GotoMsg;
    else
        goto L_6ae7;

L_6ae7:
    if (wParam == '+')
        goto CheckBox;
    else
        goto L_6af6;

L_6af6:
    if (wParam != '-')
        goto L_7207;
    else
        goto L_6aff;

L_6aff:
    i = 0;
    goto L_6b34;

L_6b07:
    if ((bitfMsgSent[i] & bitfMsgFiltered[i]) != 0)
        goto L_6b3f;
    else
        goto L_6b30;

L_6b30:
    i++;

L_6b34:
    if ((uint16_t)i < 49)
        goto L_6b07;
    else
        goto L_6b3f;

L_6b3f:
    if (i != 49)
        goto ZoomBox;
    else
        goto L_6b47;

L_6b47:
    goto L_7207;

L_6b50:
    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0)
        goto L_6b75;
    else
        goto L_6b6c;

L_6b6c:
    SetFocus(hwndFrame);

L_6b75:
    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndMsgBtn[0])
        goto L_6c7e;
    else
        goto L_6b84;

L_6b84:
    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0)
        goto L_6c7e;
    else
        goto PrevMsg;

PrevMsg:
    if (gd.fSendMsgMode == 0)
        goto L_6bc2;
    else
        goto L_6bb3;

L_6bb3:
    FFinishPlrMsgEntry(-1);
    goto SetupNewMsg;

L_6bc2:
    if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) == 0)
        goto L_6bee;
    else
        goto L_6bd6;

L_6bd6:
    iMsgCur = -1;
    i = IMsgNext(0);
    goto L_6bfd;

L_6bee:
    i = IMsgPrev(0);

L_6bfd:
    if (i == -1)
        goto L_6c0f;
    else
        goto L_6c06;

L_6c06:
    iMsgCur = i;
    goto SetupNewMsg;

L_6c0f:
    if (iMsgCur != cMsg + vcmsgplrIn)
        goto L_7207;
    else
        goto L_6c1f;

L_6c1f:
    iMsgCur--;

SetupNewMsg:
    gd.fGotoVCR = 0;
    SetMsgTitle(hwnd);
    InvalidateRect(hwnd, &rcMsgText, 1);
    if (gd.fTutorial == 0)
        goto L_7207;
    else
        goto L_6c67;

L_6c67:
    tutor.fChange = 1;
    AdvanceTutor();

L_6c78:
    goto L_7207;

L_6c7e:
    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndMsgBtn[2])
        goto L_6d25;
    else
        goto L_6c8d;

L_6c8d:
    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0)
        goto L_6d25;
    else
        goto NextMsg;

NextMsg:
    if (gd.fSendMsgMode == 0)
        goto L_6ccb;
    else
        goto L_6cbc;

L_6cbc:
    FFinishPlrMsgEntry(1);
    goto SetupNewMsg;

L_6ccb:
    if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) == 0)
        goto L_6cfb;
    else
        goto L_6cdf;

L_6cdf:
    iMsgCur = cMsg + vcmsgplrIn;
    i = IMsgPrev(0);
    goto L_6d0a;

L_6cfb:
    i = IMsgNext(0);

L_6d0a:
    if (i == -1)
        goto L_7207;
    else
        goto L_6d13;

L_6d13:
    iMsgCur = i;
    goto SetupNewMsg;

L_6d25:
    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndMsgBtn[3])
        goto L_6d62;
    else
        goto L_6d34;

L_6d34:
    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0)
        goto L_6d62;
    else
        goto L_6d50;

L_6d50:
    FFinishPlrMsgEntry(1000);
    goto SetupNewMsg;

L_6d62:
    if (GET_WM_COMMAND_HWND(wParam, lParam) != rghwndMsgBtn[1])
        goto Default;
    else
        goto L_6d71;

L_6d71:
    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0)
        goto Default;
    else
        goto GotoMsg;

GotoMsg:
    if (gd.fSendMsgMode != 0)
        goto ToggleMsgMode;
    else
        goto L_6da0;

L_6da0:
    if (iMsgCur >= cMsg)
        goto ToggleMsgMode;
    else
        goto L_6daf;

L_6daf:
    goto L_713a;

L_6db8:
    SelectAdjPlanet(0, idMsgObj);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 'v', 0);
    idm = IdmGetMessageN(iMsgCur);
    if (idm == 62)
        goto L_6e1f;
    else
        goto L_6e02;

L_6e02:
    if (idm == 63)
        goto L_6e1f;
    else
        goto L_6e0b;

L_6e0b:
    if (idm < 175)
        goto L_7163;
    else
        goto L_6e15;

L_6e15:
    if (idm > 180)
        goto L_7163;
    else
        goto L_6e1f;

L_6e1f:
    if (gd.fGotoVCR != 0)
        goto L_6e4c;
    else
        goto L_6e32;

L_6e32:
    gd.fGotoVCR = 1;
    SetMsgTitle(hwnd);
    goto L_7163;

L_6e4c:
    if (sel.grobj != grobjPlanet)
        goto L_7163;
    else
        goto L_6e56;

L_6e56:
    if (sel.id != idMsgObj)
        goto L_7163;
    else
        goto L_6e62;

L_6e62:
    ChangeProduction(0);

L_6e6e:
    goto L_7163;

L_6e71:
    SelectAdjFleet(0, idMsgObj);
    UpdateWindow(hwndScanner);
    SendMessage(hwndScanner, WM_CHAR, 'v', 0);
    goto L_7163;

L_6ea6:
    lpth = LpthFromId(vptMsg.x);
    if (lpth != 0)
        goto L_6eca;
    else
        goto L_7163;

L_6eca:
    scan.pt = lpth->pt;
    scan.grobj = grobjThing;
    ChangeScanSel(&scan, 0);
    CtrPointScan(scan.pt, 1);

L_6f02:
    goto L_7163;

L_6f05:
    SelectOursAtObject(&vptMsg);
    if (gd.fGotoVCR == 0)
        goto L_6f33;
    else
        goto L_6f24;

L_6f24:
    BattleVCR(idMsgObj);
    goto L_7163;

L_6f33:
    gd.fGotoVCR = 1;
    SetMsgTitle(hwnd);

L_6f4a:
    goto L_7163;

L_6f4d:
    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_RESEARCH, 0);
    goto L_7163;

L_6f69:
    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_SCORE, 0);
    goto L_7163;

L_6f85:
    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_SHIP_BUILDER, 0);
    goto L_7163;

L_6fa1:
    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_RELATIONS2, 0);
    goto L_7163;

L_6fbd:
    if (hwndReportDlg == 0)
        goto L_6fd2;
    else
        goto L_6fc7;

L_6fc7:
    if (vprptCur == &vrptBattle)
        goto L_7163;
    else
        goto L_6fd2;

L_6fd2:
    PostMessage(hwndFrame, WM_COMMAND, IDM_REPORT_BATTLE, 0);

L_6feb:
    goto L_7163;

L_6fee:
    szWork[200] = 2;
    lpProc = MakeProcInstance(MsgDlg, hInst);
    fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SERIAL_NUMBER), hwndTitle == 0 ? hwndFrame : hwndTitle, lpProc);
    FreeProcInstance(lpProc);
    if (fRet == 0)
        goto L_7163;
    else
        goto L_704c;

L_704c:
    if (FValidSerialNo(szWork, &lSerial) == 0)
        goto L_7088;
    else
        goto L_7064;

L_7064:
    vSerialNumber = lSerial;
    memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
    goto L_7163;

L_7088:
    if (vSerialNumber != 0)
        goto L_7163;
    else
        goto L_709c;

L_709c:
    memcpy(vrgbMachineConfig, vrgbEnvCur, 11);

L_70b0:
    goto L_7163;

L_70b3:
    vpartBrowser.hs.grhst = 1 << (idMsgObj >> 8 & 0xf);
    vpartBrowser.hs.iItem = idMsgObj & 0xff;
    FLookupPart(&vpartBrowser);
    if (hwndBrowser == 0)
        goto L_7115;
    else
        goto L_70fd;

L_70fd:
    InvalidateRect(hwndBrowserChild, NULL, 1);
    goto L_7163;

L_7115:
    fBrowserValid = 1;
    PostMessage(hwndFrame, WM_COMMAND, IDM_VIEW_BROWSER_TOGGLE2, 0);

L_7134:
    goto L_7163;

L_713a:
    if (mdMsgObj > mdMsgObjBattleReport)
        goto L_7163;
    else
        goto L_7142;

L_7142:
    switch (mdMsgObj * 2) {
    case 0x0:
        goto L_7163;
    case 0x2:
        goto L_6db8;
    case 0x4:
        goto L_6e71;
    case 0x6:
        goto L_6f4d;
    case 0x8:
        goto L_70b3;
    case 0xa:
        goto L_6f85;
    case 0xc:
        goto L_6f05;
    case 0xe:
        goto L_6fa1;
    case 0x10:
        goto L_6f69;
    case 0x12:
        goto L_6fee;
    case 0x14:
        goto L_6ea6;
    case 0x16:
        goto L_6fbd;
    }

L_7163:
    if (gd.fTutorial == 0)
        goto L_7207;
    else
        goto L_7176;

L_7176:
    tutor.fChange = 1;
    AdvanceTutor();

L_7187:
    goto L_7207;

Default:
    return DefWindowProc(hwnd, message, wParam, lParam);

L_71a4:
    if (message == WM_CREATE)
        goto L_5ca1;
    else
        goto L_71ac;

L_71ac:
    if (message == WM_SIZE)
        goto L_5ef1;
    else
        goto L_71b4;

L_71b4:
    if (message == WM_PAINT)
        goto L_643a;
    else
        goto L_71bc;

L_71bc:
    if (message == WM_ERASEBKGND)
        goto L_6059;
    else
        goto L_71c4;

L_71c4:
    if (IS_WM_CTLCOLOR(message) != 0)
        goto L_640f;
    else
        goto L_71cc;

L_71cc:
    if (message == WM_SETCURSOR)
        goto L_6084;
    else
        goto L_71d4;

L_71d4:
    if (message == WM_GETMINMAXINFO)
        goto L_63e0;
    else
        goto L_71dc;

L_71dc:
    if (message == WM_KEYDOWN)
        goto L_6a79;
    else
        goto L_71e4;

L_71e4:
    if (message == WM_CHAR)
        goto L_6ad8;
    else
        goto L_71ec;

L_71ec:
    if (message == WM_COMMAND)
        goto L_6b50;
    else
        goto L_71f4;

L_71f4:
    if (message == WM_LBUTTONDOWN)
        goto L_60cf;
    else
        goto L_71fc;

L_71fc:
    if (message != WM_LBUTTONDBLCLK)
        goto Default;
    else
        goto L_7201;

L_7201:
    goto L_60cf;

L_7207:
    return 0;
}
