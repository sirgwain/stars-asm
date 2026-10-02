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
    uint16_t    t_scratch_m30;
    int16_t     t_62dd;
    char       *t_merge_66fb_0001;
    uint16_t    t_scratch_m74_2;

    switch (message) {
    case WM_CREATE:
        for (i = 0; i < 4; i++) {
            hwndMessage = hwnd;
            rghwndMsgBtn[i] = CreateWindow("BUTTON", PszGetCompressedString(i + 1356), WS_CHILD, 100, 100, i == 3 ? 50 : 44, (3 * dyArial8 >> 1) - 1, hwnd,
                                           NULL, hInst, NULL);
            SendMessage(rghwndMsgBtn[i], WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        }
        hwndMsgDrop = CreateWindow("COMBOBOX", "MsgDD", CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 100, 100, 200, 80, hwnd, NULL, hInst, NULL);
        SendMessage(hwndMsgDrop, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndMsgEdit = CreateWindow("EDIT", NULL, ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD | WS_BORDER, 100, 100, 200, 50, hwnd, NULL, hInst, NULL);
        SendMessage(hwndMsgEdit, EM_LIMITTEXT, 0x3c8, 0);
        SendMessage(hwndMsgEdit, WM_SETFONT, (WPARAM)rghfontArial8[1], 0);
        hwndMsgScroll = CreateWindow("EDIT", NULL, ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_CHILD | WS_BORDER | WS_VSCROLL, 100, 100, 200, 50, hwnd,
                                     NULL, hInst, NULL);
        SetMsgTitle(hwnd);
        SendMessage(hwndMsgDrop, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsEverybody));
        for (i = 0; i < game.cPlayer; i++) {
            psz = PszPlayerName(i, 1, 1, 1, 0, NULL);
            SendMessage(hwndMsgDrop, CB_ADDSTRING, 0, (LPARAM)psz);
        }
        SendMessage(hwndMsgDrop, CB_SETCURSEL, 0, 0);
        break;
    case WM_SIZE:
        dx = LOWORD(lParam);
        dy = HIWORD(lParam);
        for (i = 0; i < 3; i++) {
            SetWindowPos(rghwndMsgBtn[i], NULL, dx - 48, ((3 * dyArial8 >> 1) + 2) * i + 3 + dyArial8 * 2, 0, 0,
                         SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        }
        SetRect(&rcMsgText, 4, dyArial8 * 2 + 3, dx - 52, dy - 4);
        SetRect(&rcMsgTitle, 4, 4, dx - 4, dyArial8 * 2 - 4);
        rc = rcMsgText;
        ExpandRc(&rc, -4, -4);
        SetWindowPos(hwndMsgDrop, NULL, rc.left + 30, rc.top, rc.right - rc.left - 84, rc.bottom - rc.top, SWP_NOZORDER);
        SetWindowPos(rghwndMsgBtn[3], NULL, rc.right - 50, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        rc.top += dyShipDD + 3;
        SetWindowPos(hwndMsgEdit, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
        goto Default;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        _Draw3dFrame(hdc, &rcMsgTitle, 0);
        crFore = SetTextColor(hdc, crButtonText);
        crBack = SetBkColor(hdc, crButtonFace);
        cch = strlen(szMsgTitle);
        dxMax = rcMsgTitle.right - rcMsgTitle.left - 48;
        for (; cch > 0 && (int16_t)LOWORD(GetTextExtent(hdc, szMsgTitle, cch)) > dxMax; cch--) {
        }
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
        if (gd.fSendMsgMode == 0) {
            if (iMsgCur >= cMsg) {
                lpmsgplr = vlpmsgplrIn;
                for (i = cMsg; i < iMsgCur; i++) {
                    lpmsgplr = lpmsgplr->lpmsgplrNext;
                }
                if (CchGetString(idsSCC, szT) >= 32) {
                }
                cch = _wsprintf(lpb2k, szT, PszPlayerName(lpmsgplr->iPlrFrom, 1, 1, 1, 0, NULL), 13, 10);
                if (CchGetString(idsSCC2, szT) >= 32) {
                }
                t_merge_66fb_0001 = lpmsgplr->iPlrTo == 0 ? PszGetCompressedString(idsEverybody) : PszPlayerName(lpmsgplr->iPlrTo - 1, 1, 1, 1, 0, NULL);
                cch += _wsprintf(lpb2k + cch, szT, t_merge_66fb_0001, 13, 10);
                if (lpmsgplr->cLen >= 0) {
                    i = 1000;
                    FDecompressUserString(lpmsgplr->rgbMsg, lpmsgplr->cLen, lpb2k + cch, &i);
                } else {
                    fstrcpy(lpb2k + cch, lpmsgplr->rgbMsg);
                }
                lpsz = lpb2k;
            } else {
                idm = IdmGetMessageN(iMsgCur);
                if (iMsgCur < 0 && cMsg > 0) {
                    lpsz = PszGetCompressedString(idsMessagesHaveSentYearFilteredIfWant);
                } else if (iMsgCur >= 0 && (bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) != 0 && fViewFilteredMsg == 0) {
                    lpsz = PszGetCompressedString(idsMessageTypeHasFilteredWillShownDefault);
                } else {
                    lpsz = PszGetMessageN(iMsgCur);
                }
            }
            SetTextColor(hdc, 0xffffff);
            iMode = SetBkMode(hdc, TRANSPARENT);
            if (iMsgCur >= 0 || cMsg <= 0) {
                if (iMsgCur < 0 || iMsgCur >= cMsg)
                    goto L_6903;
                t_scratch_m74_2 = 1 << (IdmGetMessageN(iMsgCur) & 7);
                if ((bitfMsgFiltered[IdmGetMessageN(iMsgCur) >> 3] & t_scratch_m74_2) == 0)
                    goto L_6903;
            }
            cch = CchGetString(idsFiltered, szWork);
            DiaganolTextOut(hdc, &rc, szWork, cch);
            lpsz = PszGetMessageN(iMsgCur);
        L_6903:
            SetTextColor(hdc, crButtonText);
            rcActual = rc;
            DrawText(hdc, lpsz, fstrlen(lpsz), &rcActual, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);
            if (rcActual.bottom <= rc.bottom && rcActual.right <= rc.right) {
                ShowWindow(hwndMsgScroll, SW_HIDE);
                DrawText(hdc, lpsz, fstrlen(lpsz), &rc, DT_WORDBREAK | DT_NOPREFIX);
            } else {
                SetWindowText(hwndMsgScroll, lpsz);
                ExpandRc(&rc, 4, 4);
                SetWindowPos(hwndMsgScroll, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_SHOWWINDOW);
            }
            SetBkMode(hdc, iMode);
        } else {
            iMode = SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, crButtonText);
            cch = CchGetString(idsTo3, szT);
            RightTextOut(hdc, rc.left + 26, rc.top, szT, cch, 0);
            SetBkMode(hdc, iMode);
        }
        SetTextColor(hdc, crFore);
        SetBkColor(hdc, crBack);
        EndPaint(hwnd, &ps);
        break;
    default:
        if (IS_WM_CTLCOLOR(message) != 0) {
            if (GET_WM_CTLCOLOR_HWND(wParam, lParam) != hwndMsgScroll)
                goto Default;
            SetBkColor((HDC)wParam, crButtonFace);
            return (LRESULT)hbrButtonFace;
        }
        switch (message) {
        case WM_SETCURSOR:
            hcs = 0;
            GetCursorPos16(&pt);
            ScreenToClient16(hwnd, &pt);
            if (HtMsgBox(pt) == htMsgNone)
                goto Default;
            SetCursor(hcurHand);
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            ht = HtMsgBox(pt);
            switch (ht) {
            case htMsgCurrent:
                goto CheckBox;
            case htMsgZoom:
                goto ZoomBox;
            case htMsgMode:
                goto ToggleMsgMode;
            }
            return 0;
        case WM_GETMINMAXINFO:
            ((MINMAXINFO *)lParam)->ptMinTrackSize.x = dxWinFrame * 2 + 198;
            ((MINMAXINFO *)lParam)->ptMinTrackSize.y = (0xd * dyArial8 >> 1) + 0x16;
            goto Default;
        case WM_KEYDOWN:
            if (wParam == VK_DOWN)
                goto NextMsg;
            if (wParam == VK_UP)
                goto PrevMsg;
            if (gd.fSendMsgMode != 0)
                goto Default;
            if (wParam == VK_HOME) {
                iMsgCur = -1;
                goto NextMsg;
            }
            if (wParam != VK_END) {
                return 0;
            }
            iMsgCur = cMsg + vcmsgplrIn;
            goto PrevMsg;
        case WM_CHAR:
            switch (wParam) {
            case '+':
                goto CheckBox;
            case '-':
                for (i = 0; (uint16_t)i < 49 && (bitfMsgSent[i] & bitfMsgFiltered[i]) == 0; i++) {
                }
                if (i != 49)
                    goto ZoomBox;
                break;
            case '\r':
                goto GotoMsg;
            }
            return 0;
        case WM_COMMAND:
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                SetFocus(hwndFrame);
            }
            if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndMsgBtn[0] && GET_WM_COMMAND_CMD(wParam, lParam) == 0)
                goto PrevMsg;
            if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndMsgBtn[2] && GET_WM_COMMAND_CMD(wParam, lParam) == 0)
                goto NextMsg;
            if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndMsgBtn[3] && GET_WM_COMMAND_CMD(wParam, lParam) == 0) {
                FFinishPlrMsgEntry(1000);
                goto SetupNewMsg;
            }
            if (GET_WM_COMMAND_HWND(wParam, lParam) == rghwndMsgBtn[1] && GET_WM_COMMAND_CMD(wParam, lParam) == 0)
                goto GotoMsg;
        default:
            goto Default;
        }
    ZoomBox:
        fViewFilteredMsg = fViewFilteredMsg == 0 ? 1 : 0;
        if (iMsgCur >= 0) {
            t_scratch_m30 = 1 << (IdmGetMessageN(iMsgCur) & 7);
            if (((bitfMsgFiltered[IdmGetMessageN(iMsgCur) >> 3] & t_scratch_m30) == 0 ? 0 : 1) == fViewFilteredMsg)
                goto L_626c;
        }
        i = IMsgNext(fViewFilteredMsg);
        if (i == -1) {
            i = IMsgPrev(fViewFilteredMsg);
        }
        iMsgCur = i;
    L_626c:
        InvalidateRect(hwndMessage, NULL, 1);
        SetMsgTitle(hwnd);
        break;
    CheckBox:
        if (iMsgCur < 0)
            break;
        idm = IdmGetMessageN(iMsgCur);
        fSet = (bitfMsgFiltered[idm >> 3] & 1 << (idm & 7)) == 0 ? 0 : 1;
        SetFilteringGroups(idm, fSet == 0 ? 1 : 0);
        DirtyGame(1);
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
        InvalidateRect(hwndMessage, NULL, 1);
        SetMsgTitle(hwnd);
        break;
    GotoMsg:
        if (gd.fSendMsgMode == 0 && iMsgCur < cMsg) {
            if (mdMsgObj <= mdMsgObjBattleReport) {
                switch (mdMsgObj) {
                case mdMsgObjPlanet:
                    SelectAdjPlanet(0, idMsgObj);
                    UpdateWindow(hwndScanner);
                    SendMessage(hwndScanner, WM_CHAR, 'v', 0);
                    idm = IdmGetMessageN(iMsgCur);
                    if (idm != 62 && idm != 63 && (idm < 175 || idm > 180))
                        break;
                    if (gd.fGotoVCR == 0) {
                        gd.fGotoVCR = 1;
                        SetMsgTitle(hwnd);
                        break;
                    }
                    if (sel.grobj != grobjPlanet || sel.id != idMsgObj)
                        break;
                    ChangeProduction(0);
                    break;
                case mdMsgObjFleet:
                    SelectAdjFleet(0, idMsgObj);
                    UpdateWindow(hwndScanner);
                    SendMessage(hwndScanner, WM_CHAR, 'v', 0);
                    break;
                case mdMsgObjThing:
                    lpth = LpthFromId(vptMsg.x);
                    if (lpth == 0)
                        break;
                    scan.pt = lpth->pt;
                    scan.grobj = grobjThing;
                    ChangeScanSel(&scan, 0);
                    CtrPointScan(scan.pt, 1);
                    break;
                case mdMsgObjBattle:
                    SelectOursAtObject(&vptMsg);
                    if (gd.fGotoVCR != 0) {
                        BattleVCR(idMsgObj);
                        break;
                    }
                    gd.fGotoVCR = 1;
                    SetMsgTitle(hwnd);
                    break;
                case mdMsgObjResearch:
                    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_RESEARCH, 0);
                    break;
                case mdMsgObjScore:
                    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_SCORE, 0);
                    break;
                case mdMsgObjShipDesign:
                    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_SHIP_BUILDER, 0);
                    break;
                case mdMsgObjRelations:
                    PostMessage(hwndFrame, WM_COMMAND, IDM_GAME_RELATIONS2, 0);
                    break;
                case mdMsgObjBattleReport:
                    if (hwndReportDlg != 0 && vprptCur == &vrptBattle)
                        break;
                    PostMessage(hwndFrame, WM_COMMAND, IDM_REPORT_BATTLE, 0);
                    break;
                case mdMsgObjSerialNumber:
                    szWork[200] = 2;
                    lpProc = MakeProcInstance(MsgDlg, hInst);
                    fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_SERIAL_NUMBER), hwndTitle == 0 ? hwndFrame : hwndTitle, lpProc);
                    FreeProcInstance(lpProc);
                    if (fRet == 0)
                        break;
                    if (FValidSerialNo(szWork, &lSerial) != 0) {
                        vSerialNumber = lSerial;
                        memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
                        break;
                    }
                    if (vSerialNumber != 0)
                        break;
                    memcpy(vrgbMachineConfig, vrgbEnvCur, 11);
                    break;
                case mdMsgObjPart:
                    vpartBrowser.hs.grhst = 1 << (idMsgObj >> 8 & 0xf);
                    vpartBrowser.hs.iItem = idMsgObj & 0xff;
                    FLookupPart(&vpartBrowser);
                    if (hwndBrowser != 0) {
                        InvalidateRect(hwndBrowserChild, NULL, 1);
                    } else {
                        fBrowserValid = 1;
                        PostMessage(hwndFrame, WM_COMMAND, IDM_VIEW_BROWSER_TOGGLE2, 0);
                    }
                }
            }
            if (gd.fTutorial == 0)
                break;
            tutor.fChange = 1;
            AdvanceTutor();
            break;
        }
    ToggleMsgMode:
        if (gd.fSendMsgMode != 0) {
            FFinishPlrMsgEntry(0);
        } else if (iMsgCur >= cMsg) {
            lpmpSrc = vlpmsgplrIn;
            i = iMsgCur - cMsg;
            while (1) {
                t_62dd = i;
                i--;
                if (t_62dd == 0)
                    break;
                lpmpSrc = lpmpSrc->lpmsgplrNext;
            }
            lpmp = vlpmsgplrOut;
            iMsgSendCur = 0;
            while (lpmp != 0 && (lpmp->iPlrTo - 1 != lpmpSrc->iPlrFrom || lpmp->iInRe != iMsgCur)) {
                lpmp = lpmp->lpmsgplrNext;
                iMsgSendCur++;
            }
            viInRe = lpmpSrc->iPlrFrom + 1;
        } else {
            viInRe = 0;
        }
        gd.fSendMsgMode = gd.fSendMsgMode == 0 ? 1 : 0;
        InvalidateRect(hwndMessage, NULL, 1);
        SetMsgTitle(hwnd);
        SetFocus(hwndMsgEdit);
        break;
    PrevMsg:
        if (gd.fSendMsgMode != 0) {
            FFinishPlrMsgEntry(-1);
            goto SetupNewMsg;
        }
        if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
            iMsgCur = -1;
            i = IMsgNext(0);
        } else {
            i = IMsgPrev(0);
        }
        if (i != -1) {
            iMsgCur = i;
            goto SetupNewMsg;
        }
        if (iMsgCur != cMsg + vcmsgplrIn)
            break;
        iMsgCur--;
        goto SetupNewMsg;
    NextMsg:
        if (gd.fSendMsgMode != 0) {
            FFinishPlrMsgEntry(1);
        } else {
            if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
                iMsgCur = cMsg + vcmsgplrIn;
                i = IMsgPrev(0);
            } else {
                i = IMsgNext(0);
            }
            if (i == -1)
                break;
            iMsgCur = i;
        }
    SetupNewMsg:
        gd.fGotoVCR = 0;
        SetMsgTitle(hwnd);
        InvalidateRect(hwnd, &rcMsgText, 1);
        if (gd.fTutorial != 0) {
            tutor.fChange = 1;
            AdvanceTutor();
        }
    }
    return 0;
Default:
    return DefWindowProc(hwnd, message, wParam, lParam);
}
