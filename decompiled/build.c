#include "common.h"

HullSlotType rggrbitPartsSB[8] = {hstEnabledSB, hstArmor, hstBeam, hstSpecialE, hstSpecialSB, hstShield, hstTorp, hstWeapon};
StringId     rgidsPartsSB[8] = {idsAll, idsArmor3, idsBeamWeapons, idsElectrical, idsOrbital, idsShields3, idsTorpedoes, idsWeapons2};
HullSlotType rggrbitParts[13] = {hstEnabled, hstArmor,  hstBeam,    hstBomb,   hstSpecialE, hstEngine, hstSpecialM,
                                 hstMines,   hstMining, hstScanner, hstShield, hstTorp,     hstWeapon};
StringId     rgidsParts[13] = {idsAll,        idsArmor3,       idsBeamWeapons, idsBombs,    idsElectrical, idsEngines, idsMechanical,
                               idsMineLayers, idsMiningRobots, idsScanners,    idsShields3, idsTorpedoes,  idsWeapons2};
HullSlotType rghstCat[14] = {hstWeapon, hstSpecialEM, hstArmor,  hstBeam,     hstBomb,     hstEngine, hstMines,
                             hstMining, hstScanner,   hstShield, hstSpecialE, hstSpecialM, hstTorp,   hstSpecialSB};
StringId     rgidsCat[14] = {idsWeapons2,     idsDevices,  idsArmor3,   idsBeamWeapons, idsBombs,      idsEngines,   idsMineLayers,
                             idsMiningRobots, idsScanners, idsShields3, idsElectrical,  idsMechanical, idsTorpedoes, idsOrbital};

int16_t ShipBuilder(POINT16 ptDlgSize) {
    FARPROC lpProcSlot;
    int16_t fSuccess;

    ptslotGlob = ptDlgSize;
    if (gd.mdScreenSize > 0) {
        ptslotGlob.y += 3 * dyArial8;
    }
    fStarbaseMode = 0;
    lpshdefBuild = NthValidShdef(0);
    lpProcSlot = MakeProcInstance(SlotDlg, hInst);
    fSuccess = DialogBox(hInst, MAKEINTRESOURCE(IDD_SLOT), hwndFrame, lpProcSlot);
    FreeProcInstance(lpProcSlot);
    if (sel.grobj == grobjPlanet && sel.pl.lpplprod != 0) {
        FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, NULL);
    }
    return 0;
}

void ShowMainControls(HWND hwnd, int16_t sw) {
    ShowWindow(GetDlgItem(hwnd, IDC_IMPORT), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_EDIT), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_DELETE), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_PREV), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_NEXT2), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_FIRST), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_LAST), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_UP), sw);
    ShowWindow(GetDlgItem(hwnd, IDC_DOWN), sw);
    ShowWindow(GetDlgItem(hwnd, IDOK), sw == SW_SHOW ? SW_HIDE : SW_SHOW);
    SetDlgItemText(hwnd, IDCANCEL, PszGetCompressedString(sw == SW_SHOW ? idsDone : idsCancel));
    return;
}

int16_t FCheckQueuedShip(HWND hwnd, SHDEF *lpshdef, int16_t fEdit) {
    char     rgch[40];
    int16_t  fProgress;
    int16_t  id;
    StringId ids;
    int16_t  cshQueued;
    char    *t_merge_0342_0001;
    char    *t_merge_036d_0001;
    char    *t_merge_03ca_0001;
    char    *t_merge_03e2_0001;
    char    *t_merge_043f_0001;

    cshQueued = CshQueued(lpshdef->ishdef, &fProgress, fEdit);
    if (lpshdef->cExist > 0 || cshQueued != 0) {
        if (fEdit != 0) {
            ids = idsCurrentlyHaveDSSIfDelete2;
        } else {
            ids = fStarbaseMode == 0 ? idsCurrentlyHaveDSSDProduction : idsCurrentlyHaveDSSDProduction2;
        }
        CchGetString(idsWorkDone, rgch);
        if (lpshdef->cExist > 0 && cshQueued != 0) {
            t_merge_0342_0001 = fProgress == 0 ? "" : rgch;
            t_merge_036d_0001 = lpshdef->cExist == 1 ? "" : "s";
            _wsprintf(szWork, PszGetCompressedString(ids), LOWORD(lpshdef->cExist), lpshdef->hul.szClass, t_merge_036d_0001, cshQueued, t_merge_0342_0001);
        } else if (cshQueued != 0) {
            t_merge_03ca_0001 = fProgress == 0 ? "" : rgch;
            t_merge_03e2_0001 = cshQueued == 1 ? "" : "s";
            _wsprintf(szWork, PszGetCompressedString(ids + 1), cshQueued, lpshdef->hul.szClass, t_merge_03e2_0001, t_merge_03ca_0001);
        } else {
            t_merge_043f_0001 = lpshdef->cExist == 1 ? "" : "s";
            _wsprintf(szWork, PszGetCompressedString(ids + 2), LOWORD(lpshdef->cExist), lpshdef->hul.szClass, t_merge_043f_0001);
        }
        id = MessageBox(GetFocus(), szWork, PszGetCompressedString(fEdit + 742), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL);
        SetFocus(hwnd);
        if (id == 7) {
            return 0;
        }
        if (lpshdef->cExist > 0) {
            DestroyAllIshdef(lpshdef->ishdef, idPlayer);
            InvalidateRect(hwndScanner, NULL, 1);
            InvalidateRect(hwndMessage, NULL, 1);
        } else {
            RemoveIshdefFromAllQueues(lpshdef->ishdef, fEdit);
        }
    }
    return 1;
}

INT_PTR CALLBACK SlotDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT               rcWindow;
    HDC                hdc;
    RECT               rcGBox;
    SHDEF             *lpshdef;
    int16_t            left;
    PAINTSTRUCT        ps;
    HWND               hwndItem;
    int16_t            cch;
    int32_t            lSel;
    RECT               rc;
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    int16_t            i;
    POINT16            pt;
    int16_t            fProtoSB;
    int16_t            fProgress;
    PART               part;
    int16_t            cshQueued;
    int16_t            j;
    HWND               t_scratch_m4e;
    HWND               t_scratch_m4e_2;
    POINT              t_pt_0900;
    POINT              t_pt_090f_1;
    HWND               t_call_1152;
    HWND               t_call_11bb;
    int16_t            t_merge_1fe4_0001;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        if (mdBuild != mdBuildEdit) {
            GetWindowRect(GetDlgItem(hwnd, IDC_PREV), &rcGBox);
            ScreenToClient(hwnd, (POINT *)&rcGBox);
            GetWindowRect(GetDlgItem(hwnd, IDC_NEXT2), &rc);
            ScreenToClient(hwnd, (POINT *)&rc.right);
            rcGBox.right = rc.right;
            rcGBox.bottom = rc.bottom;
            ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
            _Draw3dFrame(hdc, &rcGBox, -1);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsDesign, szWork);
            TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
            SelectObject(hdc, rghfontArial8[0]);
            GetWindowRect(GetDlgItem(hwnd, IDC_FIRST), &rcGBox);
            ScreenToClient(hwnd, (POINT *)&rcGBox);
            GetWindowRect(GetDlgItem(hwnd, IDC_DOWN), &rc);
            ScreenToClient(hwnd, (POINT *)&rc.right);
            rcGBox.right = rc.right;
            rcGBox.bottom = rc.bottom;
            ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
            _Draw3dFrame(hdc, &rcGBox, -1);
            SetBkColor(hdc, crButtonFace);
            SelectObject(hdc, rghfontArial8[1]);
            cch = CchGetString(idsView, szWork);
            TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
            SelectObject(hdc, rghfontArial8[0]);
        }
        GetClientRect(hwnd, &rc);
        DrawSlotDlg(hwnd, hdc, &rc, -1);
        DrawBuildSelComp(hwnd, hdc, -1);
        DrawBuildSelHull(hwnd, hdc, -1, NULL);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 2064; i <= 2069; i++) {
            t_scratch_m4e = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m4e == GetDlgItem(hwnd, i))
                break;
        }
        if (i > 2069) {
            t_scratch_m4e_2 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m4e_2 != GetDlgItem(hwnd, IDC_SHIPLIST)) {
                return 0;
            }
        }
        SetBkColor((HDC)wParam, crButtonFace);
        return (INT_PTR)hbrButtonFace;
    }
    switch (message) {
    case WM_INITDIALOG:
        fHullCopy = 0;
        hwndSlotDlg = hwnd;
        GetWindowRect(hwnd, &rcWindow);
        GetClientRect(hwnd, &rc);
        SetWindowPos(hwnd, NULL, 0, 0, ptslotGlob.x + rcWindow.right - rcWindow.left - rc.right, ptslotGlob.y + rcWindow.bottom - rcWindow.top - rc.bottom,
                     SWP_NOMOVE | SWP_NOZORDER);
        StickyDlgPos(hwnd, &ptStickySlotDlg, 1);
        UpdateSlotGlobals();
        hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
        SetWindowPos(hwndItem, NULL, ptslotGlob.x - 256, 32, 240, 266, SWP_NOZORDER);
        FillBuildPartsLB(hwndItem, rggrbitParts[0]);
        yBuildInfoSum = 340;
        lpfnRealListProc = GetWindowLong(hwndItem, GWL_WNDPROC);
        SetWindowLong(hwndItem, GWL_WNDPROC, lpfnFakeListProc);
        CheckRadioButton(hwnd, 2064, 2065, 2064);
        CheckRadioButton(hwnd, 2066, 2069, 2066);
        mdBuild = mdBuildShdef;
        hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
        SetWindowPos(hwndItem, NULL, ptslotGlob.x - 264, 8, 240, 100, SWP_NOZORDER);
        FillBuildDD(hwndItem, mdBuild);
        SetWindowPos(GetDlgItem(hwnd, IDOK), NULL, ptslotGlob.x - 226, ptslotGlob.y - (int16_t)(3 * dyArial8) / 2 - 6, 68, (int16_t)(3 * dyArial8) / 2,
                     SWP_NOZORDER | SWP_HIDEWINDOW);
        SetWindowPos(GetDlgItem(hwnd, IDCANCEL), NULL, ptslotGlob.x - 148, ptslotGlob.y - (int16_t)(3 * dyArial8) / 2 - 6, 68, (int16_t)(3 * dyArial8) / 2,
                     SWP_NOZORDER);
        SetWindowPos(GetDlgItem(hwnd, IDC_HELP), NULL, ptslotGlob.x - 74, ptslotGlob.y - (int16_t)(3 * dyArial8) / 2 - 6, 68, (int16_t)(3 * dyArial8) / 2,
                     SWP_NOZORDER);
        SetDlgItemText(hwnd, IDCANCEL, PszGetCompressedString(idsDone));
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
        return 1;
    case WM_DRAWITEM:
        lpdis = (DRAWITEMSTRUCT *)lParam;
        if (lpdis->itemID == -1) {
            HandleFocusState(lpdis, -2);
        } else {
            switch (lpdis->itemAction) {
            case 1:
                DrawDlgLBEntireItem(lpdis, -4);
                break;
            case 2:
                DrawDlgLBEntireItem(lpdis, -4);
                break;
            case 4:
                DrawDlgLBEntireItem(lpdis, -4);
            }
        }
        return 1;
    case WM_MEASUREITEM:
        lpmis = (MEASUREITEMSTRUCT *)lParam;
        lpmis->itemHeight = 66;
        return 1;
    case WM_SETCURSOR:
        GetCursorPos(&t_pt_0900);
        pt = PointTo16(t_pt_0900);
        t_pt_090f_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_090f_1);
        pt = PointTo16(t_pt_090f_1);
        if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) == 0 && PtInRect(&rgrcBuildSpin[1], PointFrom16(pt)) == 0)
            break;
        SetCursor(hcurHand);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
        return FTrackSlot(hwnd, LOWORD(lParam), HIWORD(lParam), wParam, 0, message == WM_RBUTTONDOWN ? 1 : 0);
    case WM_COMMAND:
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 0 && GET_WM_COMMAND_ID(wParam, lParam) >= IDC_PREV && GET_WM_COMMAND_ID(wParam, lParam) <= IDC_NEXT2) {
            fStarbaseMode = GET_WM_COMMAND_ID(wParam, lParam) - 2064;
            wParam = mdBuild + 2066;
            GetClientRect(hwnd, &rc);
            rc.left = rc.right >> 1 >= rc.right - 352 ? rc.right - 352 : rc.right >> 1;
            InvalidateRect(hwnd, &rc, 1);
            lpshdefBuild = NULL;
            fHullCopy = 0;
            lSel = fStarbaseMode == 0 ? (uint32_t)rggrbitParts[0] : (uint32_t)rggrbitPartsSB[0];
            FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), LOWORD(lSel));
            hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
            FillBuildDD(hwndItem, mdBuild);
            SendMessage(hwndItem, CB_SETCURSEL, 0, 0);
        } else {
            if (GET_WM_COMMAND_CMD(wParam, lParam) != 0 || GET_WM_COMMAND_ID(wParam, lParam) < IDC_FIRST || GET_WM_COMMAND_ID(wParam, lParam) > IDC_DOWN) {
                if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_EDITNAME && GET_WM_COMMAND_CMD(wParam, lParam) == 0x400 && fInEditUpdate == 0) {
                    fInEditUpdate = 1;
                    GetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), szWork, 250);
                    lSel = SendMessage(GET_WM_COMMAND_HWND(wParam, lParam), EM_GETSEL, 0, 0);
                    if (FStringFitsScreen(szWork, 160) == 0) {
                        SetWindowText(GET_WM_COMMAND_HWND(wParam, lParam), szWork);
                        SendMessage(GET_WM_COMMAND_HWND(wParam, lParam), EM_SETSEL, LOWORD(lSel), (int16_t)HIWORD(lSel));
                    }
                    lstrcpy(lpshdefBuild->hul.szClass, szWork);
                    DrawBuildSelHull(hwnd, NULL, 256, NULL);
                    fInEditUpdate = 0;
                    if (gd.fTutorial == 0)
                        break;
                    AdvanceTutor();
                    break;
                }
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDC_COMBOBOX:
                    if (GET_WM_COMMAND_CMD(wParam, lParam) != 1) {
                        return 0;
                    }
                    goto FixupShip;
                case IDC_U16_0x080C:
                    if (GET_WM_COMMAND_CMD(wParam, lParam) != 1) {
                        return 0;
                    }
                    SetBuildSelection(-1);
                    return 0;
                case IDC_DELETE:
                    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0) {
                        return 0;
                    }
                    fProgress = 0;
                    cshQueued = 0;
                    if (gd.fTutorial != 0 && FTutorialEnabledShipBuilder(0) == 0) {
                        return 0;
                    }
                    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
                    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
                    lpshdef = NthValidShdef(LOWORD(lSel));
                    if (lSel < 0 || lpshdef == 0 ||
                        ((fStarbaseMode != 0 && lSel == 0 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) ||
                         FCheckQueuedShip(hwnd, lpshdef, 0) == 0)) {
                        return 0;
                    }
                    lpshdef->fFree = 1;
                    lpshdef->cBuilt = 0;
                    lpshdef->cExist = 0;
                    if (fStarbaseMode != 0) {
                        rgplr[idPlayer].cshdefSB += 15;
                    } else {
                        rgplr[idPlayer].cShDef--;
                    }
                    LogChangeShDef(lpshdef);
                    FillBuildDD(hwndItem, mdBuild);
                    lpshdefBuild = NthValidShdef(0);
                    if ((fStarbaseMode != 0 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) || lpshdefBuild == 0) {
                        EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0);
                        EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0);
                        if (lpshdefBuild == 0) {
                            EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), 0);
                        }
                    }
                    UpdateSlotGlobals();
                    GetClientRect(hwnd, &rc);
                    rc.left = rc.right >> 1 >= rc.right - 352 ? rc.right - 352 : rc.right >> 1;
                    InvalidateRect(hwnd, &rc, 1);
                    if (fHullCopy != 0)
                        goto LRestart;
                    return 0;
                case IDC_IMPORT:
                    if (GET_WM_COMMAND_CMD(wParam, lParam) != 0) {
                        if (gd.fTutorial == 0) {
                            return 0;
                        }
                        AdvanceTutor();
                        return 0;
                    }
                    if (gd.fTutorial != 0 && FTutorialEnabledShipBuilder(1) == 0) {
                        return 0;
                    }
                    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
                    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
                    if (lSel < 0) {
                        return 0;
                    }
                    if (fStarbaseMode != 0) {
                        for (i = 0; i < 10 && rglpshdefSB[idPlayer][i].fFree == 0; i++) {
                        }
                    } else {
                        for (i = 0; i < 16 && rgshdef[i].fFree == 0; i++) {
                        }
                    }
                    if (mdBuild == mdBuildShdef || mdBuild == mdBuildEnemyShdef) {
                        if (mdBuild == mdBuildShdef) {
                            lpshdef = NthValidShdef(LOWORD(lSel));
                            if (lpshdef->fGift == 0)
                                goto L_19c4;
                        } else {
                            lpshdef = NthValidEnemyShdef(LOWORD(lSel));
                        }
                        if (fStarbaseMode != 0) {
                            part.hs.grhst = hstSBHull;
                            part.hs.iItem = lpshdef->hul.ihuldef - 32;
                        } else {
                            part.hs.grhst = hstHull;
                            part.hs.iItem = lpshdef->hul.ihuldef;
                        }
                        if (FLookupPart(&part) != 1) {
                            AlertSz(PszFormatIds(idsCantCopyShipDesignBecauseCantBuild, NULL), MB_ICONHAND);
                            return 0;
                        }
                    L_19c4:
                        if (lSel < 0 || lpshdef == 0) {
                            return 0;
                        }
                        if (fStarbaseMode != 0) {
                            rglpshdefSB[idPlayer][i] = *lpshdef;
                            lpshdef = rglpshdefSB[idPlayer] + i;
                        } else {
                            rgshdef[i] = *lpshdef;
                            lpshdef = &rgshdef[i];
                        }
                        lpshdef->cExist = 0;
                        lpshdef->cBuilt = 0;
                        if (mdBuild == mdBuildShdef && lpshdef->fGift == 0) {
                            MakeNewName(lpshdef->hul.szClass);
                        } else {
                            lpshdef->fGift = 0;
                            for (j = 0; j < lpshdef->hul.chs; j++) {
                                if (lpshdef->hul.rghs[j].cItem > 0) {
                                    part.hs = lpshdef->hul.rghs[j];
                                    if (FLookupPart(&part) != 1) {
                                        lpshdef->hul.rghs[j].cItem = 0;
                                    }
                                }
                            }
                        }
                    } else {
                        if (fStarbaseMode != 0) {
                            part.hs.grhst = hstSBHull;
                        } else {
                            part.hs.grhst = hstHull;
                        }
                        for (j = 0; j < (fStarbaseMode == 0 ? 32 : 5); j++) {
                            part.hs.iItem = j;
                            if (FLookupPart(&part) == 1 && lSel-- <= 0)
                                break;
                        }
                        if (fStarbaseMode != 0) {
                            lpshdef = rglpshdefSB[idPlayer] + i;
                            j += 32;
                        } else {
                            lpshdef = &rgshdef[i];
                        }
                        fmemset(lpshdef, 0, sizeof(SHDEF));
                        lpshdef->hul = LphuldefFromId(j)->hul;
                        lpshdef->det = detAll;
                        fmemset(lpshdef->hul.rghs, 0, 64);
                    }
                    CheckRadioButton(hwnd, 2066, 2069, 2066);
                    lpshdef->turn = game.turn;
                    lpshdef->ishdef = (fStarbaseMode == 0 ? 0 : 16) + i;
                    UpdateShdefCost(lpshdef);
                    if (fStarbaseMode != 0) {
                        rgplr[idPlayer].cshdefSB++;
                    } else {
                        rgplr[idPlayer].cShDef++;
                    }
                    LogChangeShDef(lpshdef);
                    FillBuildDD(hwndItem, mdBuild);
                    SendMessage(hwndItem, CB_SETCURSEL, i, 0);
                    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 1);
                    lpshdefBuild = lpshdef;
                    UpdateSlotGlobals();
                    fHullCopy = 1;
                    break;
                case IDC_EDIT:
                    if ((gd.fTutorial != 0 && FTutorialEnabledShipBuilder(2) == 0) ||
                        (fStarbaseMode != 0 && lpshdefBuild->ishdef == 16 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh)) {
                        return 0;
                    }
                    hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
                    lSel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
                    lpshdef = NthValidShdef(LOWORD(lSel));
                    if (lSel < 0 || lpshdef == 0 || (fStarbaseMode == 0 && FCheckQueuedShip(hwnd, lpshdef, 1) == 0)) {
                        return 0;
                    }
                    break;
                case IDOK:
                case IDCANCEL:
                    if (mdBuild != mdBuildEdit) {
                        SetBuildSelection(-2);
                        StickyDlgPos(hwnd, &ptStickySlotDlg, 0);
                        hwndSlotDlg = 0;
                        EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                        if (gd.fTutorial != 0) {
                            AdvanceTutor();
                        }
                        return 1;
                    }
                    lSel = 0;
                    if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK) {
                        if (fStarbaseMode == 0 && shdefBuild.hul.rghs[0].cItem == 0) {
                            AlertSz(PszFormatIds(idsShipDesignDoesHaveAnyEnginesMust, NULL), MB_ICONHAND);
                            return 0;
                        }
                        if (gd.fTutorial != 0 && FTutorialEnabledShipBuilder(3) == 0) {
                            return 0;
                        }
                        GetWindowText(GetDlgItem(hwnd, IDC_EDITNAME), shdefBuild.hul.szClass, 32);
                        shdefBuild.cBuilt = 0;
                        shdefBuild.cExist = 0;
                        shdefBuild.fFree = 0;
                        UpdateShdefCost(&shdefBuild);
                        if (fStarbaseMode != 0) {
                            ishdefBuild -= 16;
                            rglpshdefSB[idPlayer][ishdefBuild] = shdefBuild;
                            LogChangeShDef(&shdefBuild);
                            for (i = 0; i < ishdefBuild; i++) {
                                if (rglpshdefSB[idPlayer][i].fFree == 0) {
                                    lSel++;
                                }
                            }
                            ishdefBuild += 16;
                        } else {
                            rgshdef[ishdefBuild] = shdefBuild;
                            LogChangeShDef(&rgshdef[ishdefBuild]);
                            for (i = 0; i < ishdefBuild; i++) {
                                if (rgshdef[i].fFree == 0) {
                                    lSel++;
                                }
                            }
                        }
                    } else if (gd.fTutorial != 0 && FTutorialEnabledShipBuilder(4) == 0) {
                        return 0;
                    }
                    InvalidateRect(hwnd, NULL, 1);
                    ShowMainControls(hwnd, SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, IDC_EDITNAME), SW_HIDE);
                    hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
                    FillBuildPartsLB(hwndItem, fStarbaseMode == 0 ? rggrbitParts[0] : rggrbitPartsSB[0]);
                    SetWindowPos(hwndItem, NULL, ptslotGlob.x - 256, 32, 240, 266, SWP_NOZORDER);
                    ShowWindow(GetDlgItem(hwnd, IDC_U16_0x080C), SW_HIDE);
                    if (fHullCopy != 0) {
                        if (GET_WM_COMMAND_ID(wParam, lParam) != IDCANCEL && fStarbaseMode == 0 && lpshdefBuild->hul.rghs[0].cItem == 0) {
                            wParam = 2;
                        }
                        if (GET_WM_COMMAND_ID(wParam, lParam) == IDCANCEL) {
                            if (fStarbaseMode != 0) {
                                shdefBuild.fFree = 1;
                                rglpshdefSB[idPlayer][ishdefBuild - 16] = shdefBuild;
                                rgplr[idPlayer].cshdefSB += 15;
                                LogChangeShDef(&shdefBuild);
                            } else {
                                rgshdef[ishdefBuild].wFlags = (rgshdef[ishdefBuild].wFlags & 0xfdff) | 0x200;
                                rgplr[idPlayer].cShDef--;
                                LogChangeShDef(&rgshdef[ishdefBuild]);
                            }
                        }
                    }
                    wParam = 2066;
                    goto LRestart;
                case IDC_HELP:
                    WinHelp(hwnd, szHelpFile, 1, (uint32_t)(mdBuild == mdBuildEdit ? 3039 : 1066));
                    return 1;
                default:
                    return 0;
                }
                if (lpshdefBuild == 0)
                    break;
                InvalidateRect(hwnd, NULL, 1);
                mdBuild = mdBuildEdit;
                ishdefBuild = lpshdefBuild->ishdef;
                shdefBuild = *lpshdefBuild;
                lpshdefBuild = &shdefBuild;
                t_merge_1fe4_0001 = fStarbaseMode == 0 ? 6655 : 2620;
                FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), t_merge_1fe4_0001);
                FillBuildDD(GetDlgItem(hwnd, IDC_COMBOBOX), mdBuild);
                ShowMainControls(hwnd, SW_HIDE);
                SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x080C), NULL, 16, 32, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
                SetWindowPos(GetDlgItem(hwnd, IDC_COMBOBOX), NULL, 16, 8, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
                SetWindowPos(GetDlgItem(hwnd, IDC_EDITNAME), NULL, ptslotGlob.x - 264, 8, 240, 3 * dyArial8 >> 1, SWP_NOZORDER | SWP_SHOWWINDOW);
                SetWindowText(GetDlgItem(hwnd, IDC_EDITNAME), shdefBuild.hul.szClass);
                SendMessage(GetDlgItem(hwnd, IDC_EDITNAME), EM_LIMITTEXT, 0x1f, 0);
                if (gd.fTutorial == 0)
                    break;
                AdvanceTutor();
                break;
            }
            lSel = 0;
        LRestart:
            lpshdefBuild = NULL;
            fHullCopy = 0;
            mdBuild = GET_WM_COMMAND_ID(wParam, lParam) - 2066;
            hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
            UpdateSlotGlobals();
            FillBuildDD(hwndItem, mdBuild);
            SendMessage(hwndItem, CB_SETCURSEL, LOWORD(lSel), 0);
            SetWindowPos(hwndItem, NULL, ptslotGlob.x - 256 - (GET_WM_COMMAND_ID(wParam, lParam) == IDC_DOWN ? 0 : 8), 8, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            GetClientRect(hwnd, &rc);
            left = rc.left;
            rc.left = rc.right >> 1 >= rc.right - 352 ? rc.right - 352 : rc.right >> 1;
            InvalidateRect(hwnd, &rc, 1);
            rc.right = rc.left;
            rc.left = left;
            rc.top = yBuildInfoSum;
            InvalidateRect(hwnd, &rc, 1);
            hwndItem = GetDlgItem(hwnd, IDC_U16_0x080C);
            ShowWindow(hwndItem, mdBuild == mdBuildComp ? SW_SHOW : SW_HIDE);
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_DOWN) {
                if (gd.fTutorial == 0)
                    break;
                AdvanceTutor();
                break;
            }
        }
    FixupShip:
        hwndItem = GetDlgItem(hwnd, IDC_COMBOBOX);
        lSel = SendMessage(hwndItem, CB_GETCURSEL, 0, 0);
        switch (mdBuild) {
        case mdBuildComp:
        case mdBuildEdit:
            if (lSel == -1) {
                lSel = 0;
            } else {
                lSel = fStarbaseMode == 0 ? (uint32_t)rggrbitParts[lSel] : (uint32_t)rggrbitPartsSB[lSel];
            }
            FillBuildPartsLB(GetDlgItem(hwnd, IDC_U16_0x080C), LOWORD(lSel));
            break;
        case mdBuildShdef:
        case mdBuildHuldef:
        case mdBuildEnemyShdef:
            if (lSel != -1) {
                if (mdBuild == mdBuildShdef) {
                    fProtoSB = fStarbaseMode != 0 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh;
                    lpshdefBuild = NthValidShdef(LOWORD(lSel));
                    CshQueued(lpshdefBuild->ishdef, &fProgress, 0);
                    t_call_1152 = GetDlgItem(hwndSlotDlg, IDC_EDIT);
                    EnableWindow(t_call_1152, lpshdefBuild->cExist == 0 && fProgress == 0 && (fProtoSB == 0 || lSel > 0));
                    if (fProtoSB != 0) {
                        t_call_11bb = GetDlgItem(hwndSlotDlg, IDC_DELETE);
                        EnableWindow(t_call_11bb, lSel > 0 && lpshdefBuild->cExist == 0);
                    }
                } else if (mdBuild == mdBuildEnemyShdef) {
                    lpshdefBuild = NthValidEnemyShdef(LOWORD(lSel));
                    if (lpshdefBuild->det != detAll) {
                        i = lpshdefBuild->hul.ihuldef;
                    }
                } else {
                    if (fStarbaseMode != 0) {
                        part.hs.grhst = hstSBHull;
                        for (i = 0; i < 5; i++) {
                            part.hs.iItem = i;
                            if (FLookupPart(&part) == 1 && lSel-- <= 0)
                                break;
                        }
                        i += 32;
                    } else {
                        part.hs.grhst = hstHull;
                        for (i = 0; i < 32; i++) {
                            part.hs.iItem = i;
                            if (FLookupPart(&part) == 1 && lSel-- <= 0)
                                break;
                        }
                    }
                    shdefBuild.hul = LphuldefFromId(i)->hul;
                    shdefBuild.hul.ihuldef = i & 0xff;
                    for (i = 0; i < shdefBuild.hul.chs; i++) {
                        shdefBuild.hul.rghs[i].cItem = 0;
                    }
                    lpshdefBuild = &shdefBuild;
                    UpdateShdefCost(lpshdefBuild);
                }
            }
            UpdateSlotGlobals();
            GetClientRect(hwnd, &rc);
            rc.left = rc.right >> 1 >= rc.right - 352 ? rc.right - 352 : rc.right >> 1;
            InvalidateRect(hwnd, &rc, 1);
        }
        SetBuildSelection(-2);
        DrawBuildSelHull(hwnd, NULL, -1, NULL);
        if (lpshdefBuild == 0) {
            EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0);
            EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0);
            EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), 0);
        }
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    }
    return 0;
}

void DrawSlotDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw) {
    int16_t  yTop;
    int16_t  iMax;
    int16_t  cSlot;
    int16_t  fCreatedDC;
    HDC      hdcMem;
    int16_t  c;
    int16_t  i;
    int16_t  bkMode;
    int16_t  j;
    int16_t  cItem;
    int16_t  ibmp;
    HBITMAP  hbmpSav;
    int16_t  xLeft;
    PART     part;
    HULDEF  *lphuldef;
    RECT     rc;
    int16_t  iInventSel;
    HPEN     hpenSav;
    HBRUSH   hbrSav;
    COLORREF crBkSav;

    fCreatedDC = 0;
    if (mdBuild != mdBuildComp && lpshdefBuild != 0) {
        lphuldef = LphuldefFromId(lpshdefBuild->hul.ihuldef);
        cSlot = lphuldef->hul.chs;
        if (hdc == 0) {
            fCreatedDC = 1;
            hdc = GetDC(hwnd);
        }
        if (hwndSlotDlg == 0) {
            xLeft = 4;
            yTop = 6;
        } else {
            xLeft = ptslotGlob.x - 338;
            yTop = 6;
        }
        DrawFleetBitmap(NULL, hdc, xLeft, yTop, 1, lpshdefBuild->hul.ibmp, 0, 0, -1, 0);
        hdcMem = CreateCompatibleDC(hdc);
        hbmpSav = SelectObject(hdcMem, hbmpScanner);
        iInventSel = 0;
        if (mdBuild != mdBuildEdit) {
            rgrcBuildSpin[1].top = -5;
            rgrcBuildSpin[0].top = -5;
            rgrcBuildSpin[1].bottom = -6;
            rgrcBuildSpin[0].bottom = -6;
        } else {
            SetRect(rgrcBuildSpin, xLeft + 21, yTop + 69, xLeft + 35, yTop + 83);
            rgrcBuildSpin[1] = rgrcBuildSpin[0];
            OffsetRect(&rgrcBuildSpin[1], 14, 0);
            for (i = 0; i < 2; i++) {
                DrawBtn(hdc, &rgrcBuildSpin[i], (i == 0 ? 2 : 3) | 0x20, 0, NULL);
            }
        }
        SelectObject(hdc, rghfontArial8[1]);
        bkMode = SetBkMode(hdc, TRANSPARENT);
        if (iDraw == -1) {
            i = 0;
            iMax = cSlot;
        } else {
            i = iDraw;
            iMax = iDraw + 1;
        }
        if (lphuldef->hul.wtCargoMax != 0) {
            rc = rcCargo;
            if (fStarbaseMode != 0 && (lphuldef->hul.ihuldef == ihuldefSpaceDock || lphuldef->hul.ihuldef == ihuldefDeathStart)) {
                hbrSav = SelectObject(hdc, hbrDock);
                hpenSav = SelectObject(hdc, GetStockObject(BLACK_PEN));
                Ellipse(hdc, rc.left - 12, rc.top - 12, rc.right + 12, rc.bottom + 12);
                SelectObject(hdc, hpenSav);
                SelectObject(hdc, hbrSav);
            } else {
                FillRect(hdc, &rcCargo, GetStockObject(BLACK_BRUSH));
                rc.top++;
                rc.bottom--;
                rc.left++;
                rc.right--;
                FillRect(hdc, &rc, fStarbaseMode == 0 ? hbrCargo : hbrDock);
            }
            rc.bottom = (int16_t)(rc.bottom - rc.top) / 2 + rc.top;
            if (fStarbaseMode == 0) {
                RcCtrTextOut(hdc, &rc, PszGetCompressedString(idsCargo3), 0);
                c = _wsprintf(szWork, PCTDKT, WtMaxShdefStat(lpshdefBuild, 2));
                RcCtrTextOut(hdc, &rcCargo, szWork, 0);
            } else {
                if ((uint32_t)lphuldef->hul.wtCargoMax == 0xffff) {
                    RcCtrTextOut(hdc, &rc, PszGetCompressedString(idsUnlimited), 0);
                } else {
                    c = _wsprintf(szWork, PCTDKT, lphuldef->hul.wtCargoMax);
                    RcCtrTextOut(hdc, &rc, szWork, 0);
                }
                RcCtrTextOut(hdc, &rcCargo, PszGetCompressedString(idsSpace), 0);
            }
            rc.top = rc.bottom;
            rc.bottom = rcCargo.bottom - 1;
            if (fStarbaseMode != 0) {
                RcCtrTextOut(hdc, &rc, PszGetCompressedString(idsDock), 0);
            } else {
                RcCtrTextOut(hdc, &rc, PszGetCompressedString(idsMax), 0);
            }
        }
        for (; i < iMax; i++) {
            FillRect(hdc, &vrgrcSlot[i], hbr50Screen);
            cItem = lpshdefBuild->hul.rghs[i].cItem;
            if (cItem > 0) {
                part.hs = lpshdefBuild->hul.rghs[i];
                FLookupPart(&part);
                ibmp = part.pcom->ibmp;
                iInventSel = ibmp >> 5;
                DibBlt(hdc, vrgrcSlot[i].left, vrgrcSlot[i].top, 64, 64, rghdibInventory[iInventSel], (ibmp & 7) * 0x40, (3 - (ibmp >> 3 & 3)) * 0x40, 64, 64,
                       13369376);
                if (mdBuild != mdBuildEdit) {
                    SelectObject(hdcMem, hbmpScanner);
                    for (j = 0; j < 4; j++) {
                        BitBlt(hdc, (j >= 2 ? 57 : 3) + vrgrcSlot[i].left, ((j & 1) == 0 ? 57 : 3) + vrgrcSlot[i].top, 4, 4, hdcMem, 22, 33, SRCCOPY);
                    }
                }
                if (cItem == 1 && lphuldef->hul.rghs[i].cItem == 1 && part.hs.grhst == hstSpecialSB) {
                    szWork[0] = 0;
                    c = 0;
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsDD), cItem, lphuldef->hul.rghs[i].cItem);
                }
            } else {
                SelectObject(hdcMem, hbmpBackBld);
                ibmp = IEmptyBmpFromGrhst(lphuldef->hul.rghs[i].grhst);
                BitBlt(hdc, vrgrcSlot[i].left, vrgrcSlot[i].top, 64, 64, hdcMem, (ibmp & 7) * 0x40, (ibmp >> 3 & 3) * 0x40, SRCCOPY);
                iInventSel = -1;
                if ((lphuldef->hul.rghs[i].grhst & 1) != 0) {
                    c = _wsprintf(szWork, PszGetCompressedString(idsNeedsD), lphuldef->hul.rghs[i].cItem);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsD3), lphuldef->hul.rghs[i].cItem);
                }
            }
            CtrTextOut(hdc, vrgrcSlot[i].left + 32, vrgrcSlot[i].bottom - dyArial6 - 4, szWork, c);
            if (iselSlot == i) {
                crBkSav = SetBkColor(hdc, 0xffffff);
                FrameRect(hdc, &vrgrcSlot[i], hbr50Screen);
                ExpandRc(&vrgrcSlot[i], -1, -1);
                FrameRect(hdc, &vrgrcSlot[i], hbr50Screen);
                ExpandRc(&vrgrcSlot[i], 1, 1);
                SetBkColor(hdc, crBkSav);
            }
        }
        if ((hwndPopup == 0 || GlobalPD.fHideCounts == 0) && mdBuild == mdBuildShdef) {
            SetRect(&rc, ptPlaque.x, ptPlaque.y, ptPlaque.x + 60, ptPlaque.y + 30);
            SelectPalette(hdc, vhpal, 0);
            RealizePalette(hdc);
            DibBlt(hdc, ptPlaque.x, ptPlaque.y, 60, 30, hdibPlaque, 0, 0, 60, 30, 13369376);
            c = _wsprintf(szWork, PszGetCompressedString(idsLdLd), lpshdefBuild->cExist, lpshdefBuild->cBuilt);
            SelectObject(hdc, rghfontArial8[1]);
            if (LOWORD(GetTextExtent(hdc, szWork, c)) > 50) {
                SelectObject(hdc, rghfontArial7[0]);
                if (LOWORD(GetTextExtent(hdc, szWork, c)) > 50) {
                    SelectObject(hdc, rghfontArial6[0]);
                }
            }
            RcCtrTextOut(hdc, &rc, szWork, c);
        }
        SetBkMode(hdc, bkMode);
        SelectObject(hdcMem, hbmpSav);
        DeleteDC(hdcMem);
        if (fCreatedDC != 0) {
            ReleaseDC(hwnd, hdc);
        }
    }
    return;
}

int16_t FTrackSlot(HWND hwnd, int16_t x, int16_t y, int16_t fkb, int16_t fListBox, int16_t fRightBtn) {
    HDC     hdc;
    POINT16 ptOld;
    POINT16 ptTileSize;
    int16_t ibmpY;
    POINT16 pt;
    int16_t cSlot;
    int16_t iSrc;
    POINT16 ptDNew;
    int16_t ibmpX;
    HDC     hdcMem;
    int16_t i;
    HBITMAP hbmpFullSav;
    RECT    rcStart;
    int16_t fUseMem;
    HBITMAP hbmpScreen;
    int16_t ibmp;
    HBITMAP hbmpOld;
    HDC     hdcMemFull;
    POINT16 ptD;
    int16_t iSel;
    HBITMAP hbmpSav;
    HS      hs;
    int16_t fFirst;
    PART    part;
    RECT    rc;
    int16_t iDir;
    int16_t yTop;
    int16_t bt;
    BTNT    btnt;
    RECT   *prc;
    int16_t iBase;
    int16_t iCur;
    int16_t xLeft;
    int16_t dyStart;
    int16_t dxStart;
    POINT   t_pt_33dc_1;
    POINT   t_pt_340f_1;

    fFirst = 1;
    if (lpshdefBuild == 0) {
        return 0;
    }
    pt.x = x;
    pt.y = y;
    cSlot = LphuldefFromId(lpshdefBuild->hul.ihuldef)->hul.chs;
    if (fRightBtn == 0 && hwndSlotDlg != 0 && (PtInRect(rgrcBuildSpin, PointFrom16(pt)) != 0 || PtInRect(&rgrcBuildSpin[1], PointFrom16(pt)) != 0)) {
        if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) != 0) {
            iDir = -1;
            bt = 34;
            prc = rgrcBuildSpin;
        } else {
            iDir = 1;
            bt = 35;
            prc = &rgrcBuildSpin[1];
        }
        iBase = lpshdefBuild->hul.ibmp;
        iCur = iBase & 3;
        iBase -= iCur;
        xLeft = ptslotGlob.x - 336;
        yTop = 8;
        InitBtnTrack(&btnt, hwnd, NULL, prc, bt, 80, 0, 0, NULL);
        while (FTrackBtn(&btnt) != 0) {
            iCur = iCur + 4 + iDir & 3;
            DrawFleetBitmap(NULL, btnt.hdc, xLeft, yTop, 0, iBase + iCur, 0, 0, -1, 0);
        }
        lpshdefBuild->hul.ibmp = iBase + iCur;
        return 1;
    }
    if (fListBox == 0) {
        GetClientRect(hwnd, &rc);
        for (iSrc = 0; iSrc < cSlot && PtInRect(&vrgrcSlot[iSrc], PointFrom16(pt)) == 0; iSrc++) {
        }
        if (iSrc == cSlot) {
            return 0;
        }
        hs = lpshdefBuild->hul.rghs[iSrc];
        if (hs.cItem <= 0) {
            SetBuildSelection(iSrc);
            return 0;
        }
        part.hs = hs;
        FLookupPart(&part);
        ibmp = part.pcom->ibmp;
        rcStart = vrgrcSlot[iSrc];
    } else {
        iSel = LOWORD(SendMessage(hwnd, LB_GETCURSEL, 0, 0));
        if (iSel == -1) {
            return 0;
        }
        SendMessage(hwnd, LB_GETTEXT, iSel, (LPARAM)szWork);
        ibmp = (int16_t)(int8_t)szWork[2] - 65 + ((int16_t)(int8_t)szWork[3] - 65) * 26;
        iSrc = -1;
        hs.grhst = 1 << ((int16_t)(int8_t)szWork[0] - 0x41);
        hs.iItem = (int16_t)(int8_t)szWork[1] - 65;
        hs.cItem = 1;
        rcStart.left = 2;
        rcStart.right = 66;
        rcStart.top = y / 66 * 66 + 1;
        rcStart.bottom = rcStart.top + 64;
        ClientToScreen(hwnd, (POINT *)&rcStart);
        ClientToScreen(hwnd, (POINT *)&rcStart.right);
        t_pt_33dc_1 = PointFrom16(pt);
        ClientToScreen(hwnd, &t_pt_33dc_1);
        pt = PointTo16(t_pt_33dc_1);
        hwnd = hwndSlotDlg;
        ScreenToClient(hwnd, (POINT *)&rcStart);
        ScreenToClient(hwnd, (POINT *)&rcStart.right);
        t_pt_340f_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_340f_1);
        pt = PointTo16(t_pt_340f_1);
        x = pt.x;
        y = pt.y;
        if (hs.grhst == hstEngine && hs.iItem == iengineSettlersDelight && lpshdefBuild->hul.ihuldef != ihuldefMiniColonyShip) {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsSettlersDelightEngineMayMountedDesignsBased, szPopupBuffer);
            Popup(hwnd, x, y);
            return 1;
        }
        if (hs.grhst == hstSpecialM && hs.iItem == ispecialMOrbitalConstructionModule && lpshdefBuild->hul.ihuldef != ihuldefColonyShip) {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsOrbitalConstructionModuleMayMountedDesignsBased, szPopupBuffer);
            Popup(hwnd, x, y);
            return 1;
        }
        if (hs.grhst == hstSpecialE && hs.iItem == ispecialETransportCloaking && LphuldefFromId(lpshdefBuild->hul.ihuldef)->imdAttack != 0) {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsTransportCloakingModuleMayPlacedHullCould, szPopupBuffer);
            Popup(hwnd, x, y);
            return 1;
        }
    }
    SetBuildSelection(iSrc);
    if (fRightBtn != 0) {
        GlobalPD.part = part;
        GlobalPD.grPopup = grPopupComponent;
        Popup(hwnd, x, y);
        return 1;
    }
    if (mdBuild != mdBuildEdit) {
        return 0;
    }
    ptTileSize.y = 64;
    ptTileSize.x = 64;
    ibmpX = ibmp & 7;
    ibmpY = ibmp >> 3 & 3;
    hdc = GetDC(hwnd);
    SelectPalette(hdc, vhpal, 0);
    RealizePalette(hdc);
    hdcMem = CreateCompatibleDC(hdc);
    hdcMemFull = CreateCompatibleDC(hdc);
    SelectPalette(hdcMemFull, vhpal, 0);
    RealizePalette(hdcMemFull);
    hbmpOld = CreateCompatibleBitmap(hdc, ptTileSize.x, ptTileSize.y);
    hbmpSav = SelectObject(hdcMem, hbmpOld);
    hbmpScreen = CreateCompatibleBitmap(hdc, 3 * ptTileSize.x, 3 * ptTileSize.y);
    hbmpFullSav = SelectObject(hdcMemFull, hbmpScreen);
    SetCapture(hwnd);
    ptOld.y = -1;
    ptOld.x = -1;
    while (FGetMouseMove(&pt) != 0) {
        if (pt.x != ptOld.x || pt.y != ptOld.y) {
            if (fFirst != 0) {
                fUseMem = 0;
                fFirst = 0;
            } else {
                SelectObject(hdcMem, hbmpOld);
                ptDNew.x = pt.x - ptOld.x;
                ptDNew.y = pt.y - ptOld.y;
                fUseMem = abs(ptDNew.x) < ptTileSize.x && abs(ptDNew.y) < ptTileSize.y;
                if (fUseMem == 0) {
                    BitBlt(hdc, rcStart.left + ptD.x, rcStart.top + ptD.y, ptTileSize.x, ptTileSize.y, hdcMem, 0, 0, SRCCOPY);
                } else {
                    BitBlt(hdcMemFull, 0, 0, 3 * ptTileSize.x, 3 * ptTileSize.y, hdc, rcStart.left + ptD.x - ptTileSize.x, rcStart.top + ptD.y - ptTileSize.y,
                           SRCCOPY);
                    BitBlt(hdcMemFull, ptTileSize.x, ptTileSize.y, ptTileSize.x, ptTileSize.y, hdcMem, 0, 0, SRCCOPY);
                }
            }
            ptOld = pt;
            ptD.x = pt.x - x;
            ptD.y = pt.y - y;
            SelectObject(hdcMem, hbmpOld);
            if (fUseMem == 0) {
                BitBlt(hdcMem, 0, 0, ptTileSize.x, ptTileSize.y, hdc, rcStart.left + ptD.x, rcStart.top + ptD.y, SRCCOPY);
                DibBlt(hdc, rcStart.left + ptD.x, rcStart.top + ptD.y, 64, 64, rghdibInventory[ibmp / 32], ibmpX * 64, (3 - ibmpY) * 64, 64, 64, 13369376);
            } else {
                dxStart = 0;
                dyStart = 0;
                BitBlt(hdcMem, 0, 0, ptTileSize.x, ptTileSize.y, hdcMemFull, ptTileSize.x + ptDNew.x, ptTileSize.y + ptDNew.y, SRCCOPY);
                DibBlt(hdcMemFull, ptTileSize.x + ptDNew.x, ptTileSize.y + ptDNew.y, 64, 64, rghdibInventory[ibmp / 32], ibmpX * 64, (3 - ibmpY) * 64, 64, 64,
                       13369376);
                rc = rcStart;
                OffsetRc(&rc, ptD.x, ptD.y);
                if (ptDNew.x > 0) {
                    rc.left -= ptDNew.x;
                    dxStart = ptDNew.x;
                } else {
                    rc.right -= ptDNew.x;
                }
                if (ptDNew.y > 0) {
                    rc.top -= ptDNew.y;
                    dyStart = ptDNew.y;
                } else {
                    rc.bottom -= ptDNew.y;
                }
                BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMemFull, ptTileSize.x + ptDNew.x - dxStart,
                       ptTileSize.y + ptDNew.y - dyStart, SRCCOPY);
            }
            i = IDropPart(pt, hs, iSrc, 1);
            if (i < 0 || i == 2) {
                SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(0x7f00)));
            } else if (i == 1 && iSrc >= 0) {
                SetCursor(hcurTrashCan);
            } else {
                SetCursor(hcurNoWay);
            }
        }
    }
    if (fFirst == 0) {
        SelectObject(hdcMem, hbmpOld);
        BitBlt(hdc, rcStart.left + ptD.x, rcStart.top + ptD.y, 64, 64, hdcMem, 0, 0, SRCCOPY);
    }
    ReleaseCapture();
    SelectObject(hdcMem, hbmpSav);
    SelectObject(hdcMemFull, hbmpFullSav);
    DeleteObject(hbmpOld);
    DeleteObject(hbmpScreen);
    DeleteDC(hdcMem);
    DeleteDC(hdcMemFull);
    i = IDropPart(pt, hs, iSrc, 0);
    ReleaseDC(hwnd, hdc);
    return 1;
}

void DrawBuildSelComp(HWND hwnd, HDC hdc, int16_t iDraw) {
    uint16_t grhst;
    HS       hsShip;
    uint16_t rgCosts[4];
    int16_t  fCreatedDC;
    int16_t  c;
    int16_t  i;
    COLORREF crForeSav;
    int16_t  fPlural;
    int16_t  k;
    char     szWord[80];
    COLORREF crBackSav;
    HS       hsHul;
    int16_t  cch;
    PART     part;
    int16_t  x;
    int16_t  dxkT;
    RECT     rc;
    int16_t  iSel;
    char    *pch;
    HULDEF  *t_call_3c58;
    uint32_t t_fields_1;
    uint32_t t_fields_2;
    int16_t  t_top_3f19;

    fCreatedDC = 0;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    GetClientRect(hwndSlotDlg, &rc);
    rc.bottom -= 8;
    rc.left = 8;
    rc.right = rc.left + 256;
    rc.top = yBuildInfoSum;
    SelectObject(hdc, rghfontArial8[1]);
    crForeSav = SetTextColor(hdc, 0);
    crBackSav = SetBkColor(hdc, crButtonFace);
    FillRect(hdc, &rc, hbrButtonFace);
    if (fStarbaseMode != 0) {
        rc.top += dyArial8;
    }
    if (iselSlot != -2) {
        if (iselSlot == -1) {
            iSel = LOWORD(SendMessage(GetDlgItem(hwndSlotDlg, IDC_U16_0x080C), LB_GETCURSEL, 0, 0));
            if (iSel == -1)
                goto Restore;
            SendMessage(GetDlgItem(hwndSlotDlg, IDC_U16_0x080C), LB_GETTEXT, iSel, (LPARAM)szWork);
            hsShip.cItem = 1;
            hsShip.grhst = 1 << ((int16_t)(int8_t)szWork[0] - 0x41);
            hsShip.iItem = (int16_t)(int8_t)szWork[1] - 65;
        } else {
            if (lpshdefBuild == 0)
                goto Restore;
            hsShip = lpshdefBuild->hul.rghs[iselSlot];
            t_call_3c58 = LphuldefFromId(lpshdefBuild->hul.ihuldef);
            hsHul.grhst = t_call_3c58->hul.rghs[iselSlot].grhst;
            t_fields_1 = t_call_3c58->hul.rghs[iselSlot].iItem;
            t_fields_2 = t_call_3c58->hul.rghs[iselSlot].cItem;
            hsHul.iItem = t_fields_1;
            hsHul.cItem = t_fields_2;
            if (hsShip.cItem == 0) {
                i = CchGetString((hsHul.grhst & 1) == 0 ? idsCanHold : idsRequiresExactly, szWork);
                fPlural = hsHul.cItem == 1 ? 0 : 1;
                if (fPlural == 0) {
                    i += CchGetString(idsOne, &szWork[i]);
                } else {
                    i += _wsprintf(&szWork[i], "%d ", hsHul.cItem);
                }
                grhst = hsHul.grhst;
                while (grhst != 0) {
                    for (i = 0; i < 14; i++) {
                        if ((grhst & rghstCat[i]) == rghstCat[i]) {
                            grhst &= ~rghstCat[i];
                            break;
                        }
                    }
                    cch = CchGetString(rgidsCat[i], szWord);
                    if (fPlural == 0) {
                        if ((int16_t)(int8_t)szWord[cch - 1] == 115) {
                            if ((int16_t)(int8_t)szWord[cch - 2] == 101 && (int16_t)(int8_t)szWord[cch - 3] == 111) {
                                szWord[cch - 2] = 0;
                            } else {
                                szWord[cch - 1] = 0;
                            }
                        } else {
                            for (pch = &szWord[cch - 1]; pch > szWord && (int16_t)(int8_t)*pch != 40; pch--) {
                            }
                            if ((int16_t)(int8_t)*pch == 40 && pch > &szWord[1] && (int16_t)(int8_t)pch[-1] == 32 && (int16_t)(int8_t)pch[-2] == 115) {
                                strcpy(pch + -2, pch + -1);
                            }
                        }
                    }
                    if (grhst != 0) {
                        if ((grhst - 1 & grhst) != 0) {
                            strcat(szWord, ", ");
                        } else {
                            strcat(szWord, PszGetCompressedString(idsOr));
                        }
                    }
                    strcat(szWork, szWord);
                }
                x = rc.left;
                t_top_3f19 = rc.top;
                WrapTextOut(hdc, &x, &t_top_3f19, szWork, 0, rc.left, rc.right - rc.left, NULL, 0, 1);
                rc.top = t_top_3f19;
                rc.top += dyArial8;
                goto Restore;
            }
        }
        part.hs = hsShip;
        FLookupPart(&part);
        dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 2));
        fPlural = hsShip.cItem == 1 ? 0 : 1;
        if (fPlural == 0) {
            CchGetString(idsOne, szWord);
        } else {
            _wsprintf(szWord, "%d ", hsShip.cItem);
        }
        fstrcat(szWord, part.pcom->szName);
        if (fPlural != 0) {
            strcat(szWord, "s");
        }
        cch = _wsprintf(szWork, PszGetCompressedString(idsCostS), szWord);
        TextOut(hdc, rc.left, rc.top, szWork, cch);
        rc.left += 8;
        rc.right -= 8;
        c = hsShip.cItem;
        GetTruePartCost(idPlayer, &part, rgCosts);
        for (k = 0; k < 3; k++) {
            rc.top += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            SetTextColor(hdc, rgcrMinerals[k]);
            TextOut(hdc, rc.left, rc.top, rgszMinerals[k], lstrlen(rgszMinerals[k]));
            SelectObject(hdc, rghfontArial8[0]);
            SetTextColor(hdc, crWindowText);
            cch = _wsprintf(szWork, PCTLD, (uint32_t)(c * (uint32_t)rgCosts[k]));
            RightTextOut(hdc, rc.right - dxkT - 64, rc.top, szWork, cch, dxMaxMineralQuan);
            TextOut(hdc, rc.right - dxkT - 64, rc.top, PszGetCompressedString(idsKt), 2);
        }
        rc.top += dyArial8;
        SelectObject(hdc, rghfontArial8[1]);
        SetTextColor(hdc, rgcrMinerals[5]);
        TextOut(hdc, rc.left, rc.top, rgszMinerals[5], lstrlen(rgszMinerals[5]));
        SelectObject(hdc, rghfontArial8[0]);
        SetTextColor(hdc, crWindowText);
        cch = _wsprintf(szWork, PCTLD, (uint32_t)(c * (uint32_t)rgCosts[3]));
        RightTextOut(hdc, rc.right - dxkT - 64, rc.top, szWork, cch, dxMaxMineralQuan);
        if (fStarbaseMode == 0) {
            rc.left -= 8;
            rc.top += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            cch = _wsprintf(szWork, PszGetCompressedString(idsMassLdkt), (uint32_t)(c * part.pcom->cMass));
            TextOut(hdc, rc.left, rc.top, szWork, cch);
        }
    }
Restore:
    SelectObject(hdc, rghfontArial8[0]);
    SetTextColor(hdc, crForeSav);
    SetBkColor(hdc, crBackSav);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

int16_t PctJammerFromHul(HUL *lphul) {
    int32_t pctJam;
    int16_t ihs;
    int16_t i;
    int32_t pctHit;
    PART    part;

    pctHit = 10000;
    for (ihs = 0; ihs < lphul->chs; ihs++) {
        part.hs = lphul->rghs[ihs];
        if (part.hs.grhst == hstSpecialE) {
            if (part.hs.iItem >= ispecialEJammer10 && part.hs.iItem <= ispecialEJammer50) {
                FLookupPart(&part);
                pctJam = (int16_t)(100 - part.pspecial->grAbility);
            } else if (part.hs.iItem == ispecialEMultiFunctionPod) {
                pctJam = 90;
            } else {
                pctJam = 100;
            }
        } else if (part.hs.grhst == hstArmor && part.hs.iItem == iarmorMegaPolyShell) {
            pctJam = 80;
        } else if (part.hs.grhst == hstMining && part.hs.iItem == iminingAlienMiner) {
            pctJam = 70;
        } else if (part.hs.grhst == hstShield && part.hs.iItem == ishieldLangstonShell) {
            pctJam = 95;
        } else {
            pctJam = 100;
        }
        if (pctJam < 100) {
            for (i = part.hs.cItem; i > 0; i--) {
                pctHit = (uint32_t)(pctHit * pctJam);
                pctHit = (int32_t)(pctHit / 100);
            }
        }
    }
    if (pctHit < 100) {
        pctHit = 100;
    }
    pctJam = (int16_t)(100 - (int16_t)(LOWORD(pctHit) + 50) / 100);
    if ((int16_t)lphul->ihuldef > ihuldefOrbitalFort) {
        pctJam -= (int32_t)(pctJam / 4);
    }
    if (pctJam > 95) {
        pctJam = 95;
    }
    return LOWORD(pctJam);
}

void DrawBuildSelHull(HWND hwnd, HDC hdc, int16_t iDraw, RECT *prc) {
    char     rgch[20];
    DV       dv;
    uint16_t rgCosts[4];
    int16_t  fCreatedDC;
    COLORREF crForeSav;
    int16_t  dxMineral;
    int16_t  k;
    COLORREF crBackSav;
    int16_t  csh;
    HUL     *lphul;
    int32_t  dpShield;
    int16_t  cch;
    int32_t  dp;
    int16_t  dxkT;
    RECT     rc;
    int32_t  lwt;
    int16_t  i;
    int16_t  j;
    int16_t  dPlanRange;
    int16_t  dRange;
    int16_t  pctDetect;
    int16_t  pct;
    char    *t_merge_478e_0001;
    LPCSTR   t_merge_4c09_0001;

    fCreatedDC = 0;
    if (mdBuild != mdBuildComp) {
        if (hdc == 0) {
            fCreatedDC = 1;
            hdc = GetDC(hwnd);
        }
        if (prc == 0) {
            GetClientRect(hwnd, &rc);
            rc.bottom -= 32;
            rc.right -= 4;
            rc.left = rc.right - 320;
            rc.top = (fStarbaseMode == 0 ? 0 : dyArial8) + yBuildInfoSum;
            FillRect(hdc, &rc, hbrButtonFace);
        } else {
            rc = *prc;
        }
        if (lpshdefBuild != 0) {
            lphul = &lpshdefBuild->hul;
            SelectObject(hdc, rghfontArial8[0]);
            dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 2));
            SelectObject(hdc, rghfontArial8[1]);
            dxMineral = LOWORD(GetTextExtent(hdc, rgszMinerals[2], strlen(rgszMinerals[2]))) + 6;
            crForeSav = SetTextColor(hdc, 0);
            crBackSav = SetBkColor(hdc, crButtonFace);
            if (hwndPopup != 0 && GlobalPD.grPopup == grPopupShdef && GlobalPD.fToken != 0) {
                GetVCRStats(viVCRFocus, &dp, &dv, &dpShield, &csh);
                dpShield = (uint32_t)vrgtok[viVCRFocus].dpShield - dpShield;
                if (dpShield < 0) {
                    dpShield = 0;
                }
                dpShield = (uint32_t)(dpShield * csh);
                if (csh == 0)
                    goto LDeadToken;
            } else {
                dp = (uint32_t)lphul->dp;
                dpShield = DpShieldOfShdef(lpshdefBuild, idPlayer);
            }
            if (hwndPopup != 0 && fStarbaseMode != 0) {
                rc.top += dyArial8;
                cch = CchGetString(idsCost, szWork);
            } else {
                CchGetString(idsHull, rgch);
                t_merge_478e_0001 = mdBuild == mdBuildHuldef ? rgch : "";
                cch = _wsprintf(szWork, PszGetCompressedString(idsCostOneSS), lphul->szClass, t_merge_478e_0001);
            }
            TextOut(hdc, rc.left, rc.top, szWork, cch);
            rc.left += 8;
            rc.right -= 8;
            GetTrueHullCost(idPlayer, lphul, rgCosts);
            if (fStarbaseMode != 0 && (GetRaceGrbit(&rgplr[idPlayer], ibitRaceISB) != 0 || GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh)) {
                for (k = 0; k < 4; k++) {
                    rgCosts[k] -= (uint32_t)rgCosts[k] / 5;
                }
            }
            if (fStarbaseMode != 0) {
                for (k = 0; k < 4; k++) {
                    rgCosts[k] -= (uint32_t)rgCosts[k] / 2;
                }
            }
            for (k = 0; k <= 5; k++) {
                if (k == 3) {
                    k = 5;
                }
                rc.top += dyArial8;
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, rgcrMinerals[k]);
                TextOut(hdc, rc.left, rc.top, rgszMinerals[k], lstrlen(rgszMinerals[k]));
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crWindowText);
                cch = _wsprintf(szWork, PCTD, k == 5 ? rgCosts[3] : rgCosts[k]);
                RightTextOut(hdc, rc.left + dxMineral + dxMaxMineralQuan - dxkT, rc.top, szWork, cch, dxMaxMineralQuan);
                if (k < 5) {
                    TextOut(hdc, rc.left + dxMineral + dxMaxMineralQuan - dxkT, rc.top, PszGetCompressedString(idsKt), 2);
                }
            }
            rc.left -= 8;
            rc.top += dyArial8;
            SelectObject(hdc, rghfontArial8[1]);
            if (fStarbaseMode == 0) {
                if (hwndPopup != 0 && GlobalPD.grPopup == grPopupShdef && GlobalPD.fToken != 0) {
                    lwt = (uint32_t)vrgtok[viVCRFocus].wt;
                } else {
                    lwt = (uint32_t)lphul->wtEmpty;
                }
                cch = _wsprintf(szWork, PszGetCompressedString(idsMassLdkt), lwt);
                TextOut(hdc, rc.left, rc.top, szWork, cch);
            }
            rc.top -= dyArial8 * 4;
            rc.left += dxMineral + dxMaxMineralQuan + 24;
            if (fStarbaseMode == 0 && (hwndPopup == 0 || GlobalPD.grPopup != grPopupShdef || GlobalPD.fToken == 0)) {
                cch = _wsprintf(szWork, PszGetCompressedString(idsDmg), WtMaxShdefStat(lpshdefBuild, 1));
                RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan);
                cch = CchGetString(idsMaxFuel, szWork);
                TextOut(hdc, rc.left, rc.top, szWork, cch);
                rc.top += dyArial8;
            }
            cch = _wsprintf(szWork, PszGetCompressedString(idsLddp), dp);
            RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan + 10);
            cch = CchGetString(idsArmor, szWork);
            TextOut(hdc, rc.left, rc.top, szWork, cch);
            rc.top += dyArial8;
            if (mdBuild != mdBuildHuldef) {
                t_merge_4c09_0001 = dpShield == 0 ? PszGetCompressedString(idsNone) : "%lddp";
                cch = _wsprintf(szWork, t_merge_4c09_0001, dpShield);
                RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan + 10);
                cch = CchGetString(idsShields, szWork);
                TextOut(hdc, rc.left, rc.top, szWork, cch);
            }
            rc.top += dyArial8;
            if (mdBuild != mdBuildHuldef) {
                lpshdefBuild->lPower = LComputePower(lpshdefBuild);
                if (lpshdefBuild->lPower != 0) {
                    cch = _wsprintf(szWork, PCTLD, lpshdefBuild->lPower);
                    RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan);
                    cch = CchGetString(idsRating, szWork);
                    TextOut(hdc, rc.left, rc.top, szWork, cch);
                    rc.top += dyArial8;
                }
            }
            if (gd.mdScreenSize > 0 && mdBuild != mdBuildHuldef) {
                if (mdBuild != mdBuildEnemyShdef) {
                    i = idPlayer;
                } else {
                    i = -1;
                }
                i = PctCloakFromHuldef(&lpshdefBuild->hul, i, NULL);
                j = PctJammerFromHul(&lpshdefBuild->hul);
                cch = _wsprintf(szWork, PszGetCompressedString(idsDD4), i, j);
                RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan);
                cch = CchGetString(idsCloakJam, szWork);
                TextOut(hdc, rc.left, rc.top, szWork, cch);
                rc.top += dyArial8;
                i = InitFromHuldef(&lpshdefBuild->hul, NULL);
                if (fStarbaseMode != 0 || lpshdefBuild->hul.rghs[0].cItem == 0) {
                    j = 0;
                } else {
                    j = SpdOfShip(NULL, 0, NULL, 0, lpshdefBuild) + 1;
                }
                cch = _wsprintf(szWork, PszGetCompressedString(idsDS), i, &rgszSpeed[j * 3]);
                RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan);
                cch = CchGetString((dyArial8 <= 14 ? 0 : 1) + 1196, szWork);
                TextOut(hdc, rc.left, rc.top, szWork, cch);
                rc.top += dyArial8;
                if (fStarbaseMode == 0) {
                    if (mdBuild != mdBuildEnemyShdef) {
                        i = idPlayer;
                    } else {
                        i = -1;
                    }
                    dRange = GetShdefScannerRange(lpshdefBuild, i, &dPlanRange, &pctDetect, NULL);
                    if (dRange > 0) {
                        if (pctDetect >= 100) {
                            cch = _wsprintf(szWork, PszGetCompressedString(idsDD6), dRange, dPlanRange);
                        } else {
                            cch = _wsprintf(szWork, PszGetCompressedString(idsDDD), dRange, dPlanRange, pctDetect);
                        }
                        RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan + 40);
                        cch = CchGetString((dyArial8 <= 14 ? 0 : 1) + 1199, szWork);
                        TextOut(hdc, rc.left, rc.top, szWork, cch);
                        rc.top += dyArial8;
                    }
                } else if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
                    cch = CommaFormatLong(szWork, (uint32_t)(rglPopMac[lpshdefBuild->hul.ihuldef - 32] * 100));
                    RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan + 16);
                    cch = CchGetString((dyArial8 <= 14 ? 0 : 1) + 1271, szWork);
                    TextOut(hdc, rc.left, rc.top, szWork, cch);
                    rc.top += dyArial8;
                }
            }
            if (hwndPopup != 0 && GlobalPD.grPopup == grPopupShdef && GlobalPD.fShowDamage != 0) {
                if (GlobalPD.fToken == 0) {
                    if (fStarbaseMode == 0) {
                        if (GlobalPD.fSummary != 0) {
                            k = lpshdefBuild->ishdef;
                            csh = rglpfl[sel.scan.ifl]->rgcsh[k];
                            dv.dp = rglpfl[sel.scan.ifl]->rgdv[k].dp;
                        } else {
                            k = lpshdefBuild->ishdef;
                            csh = sel.fl.rgcsh[k];
                            dv.dp = sel.fl.rgdv[k].dp;
                        }
                    } else {
                        dv.dp = 0;
                        if (GlobalPD.fSummary != 0) {
                            dv.pctDp = LpplFromId(sel.scan.idpl)->pctDp;
                        } else {
                            dv.pctDp = sel.pl.pctDp;
                        }
                        csh = 1;
                        if (dv.pctDp != 0) {
                            dv.pctSh = 100;
                        }
                    }
                }
                if (dv.dp != 0) {
                    SetTextColor(hdc, 127);
                    pct = dv.pctDp / 5;
                    if (pct <= 0) {
                        pct = 1;
                    }
                    csh = LOWORD((int32_t)(csh * dv.pctSh) / 100);
                    if (csh <= 0) {
                        csh = 1;
                    }
                    if (fStarbaseMode != 0) {
                        cch = _wsprintf(szWork, PCTDPCTPCT, pct);
                    } else {
                        cch = _wsprintf(szWork, PszGetCompressedString(idsLdD), csh, pct);
                    }
                    dp = 1;
                } else {
                    dp = 0;
                }
                if (dp != 0) {
                    RightTextOut(hdc, rc.right - 8, rc.top, szWork, cch, dxMaxMineralQuan + 15);
                    cch = CchGetString(idsDamage, szWork);
                    TextOut(hdc, rc.left, rc.top, szWork, cch);
                }
            }
        LDeadToken:
            SelectObject(hdc, rghfontArial8[0]);
            SetTextColor(hdc, crForeSav);
            SetBkColor(hdc, crBackSav);
        }
        if (fCreatedDC != 0) {
            ReleaseDC(hwnd, hdc);
        }
    }
    return;
}

void SetBuildSelection(int16_t iSrc) {
    int16_t iSelOld;
    RECT    rc;

    if (iSrc != iselSlot) {
        iSelOld = iselSlot;
        iselSlot = iSrc;
        GetClientRect(hwndSlotDlg, &rc);
        if (iSelOld >= 0) {
            DrawSlotDlg(hwndSlotDlg, NULL, &rc, iSelOld);
        }
        if (iselSlot >= 0) {
            DrawSlotDlg(hwndSlotDlg, NULL, &rc, iselSlot);
        }
    } else if (iSrc != -1) {
        return;
    }
    DrawBuildSelComp(hwndSlotDlg, NULL, -1);
    return;
}

int16_t IDropPart(POINT16 pt, HS hsSrc, int16_t iSrc, int16_t fNoModify) {
    int16_t  cSlot;
    int16_t  cNew;
    int16_t  i;
    HS       hsHul;
    HS       hsDst;
    RECT     rc;
    HULDEF  *t_call_5761;
    uint32_t t_fields_1;
    uint32_t t_fields_2;

    GetClientRect(hwndSlotDlg, &rc);
    if ((GetAsyncKeyState(VK_CONTROL) & 0xfffe) != 0) {
        if (iSrc < 0) {
            hsSrc.cItem = 100;
        }
    } else if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
        if (iSrc < 0 || hsSrc.cItem > 4) {
            hsSrc.cItem = 4;
        }
    } else if (iSrc >= 0) {
        hsSrc.cItem = 1;
    }
    cSlot = LphuldefFromId(lpshdefBuild->hul.ihuldef)->hul.chs;
    for (i = 0; i < cSlot && PtInRect(&vrgrcSlot[i], PointFrom16(pt)) == 0; i++) {
    }
    if (i == cSlot) {
        if (pt.x < rc.right >> 1) {
            if (fNoModify == 0 && iSrc >= 0) {
                if ((lpshdefBuild->hul.rghs[iSrc].grhst & 1) != 0) {
                    hsSrc.cItem = 100;
                }
                lpshdefBuild->hul.rghs[iSrc].cItem =
                    0 <= lpshdefBuild->hul.rghs[iSrc].cItem - hsSrc.cItem ? lpshdefBuild->hul.rghs[iSrc].cItem - hsSrc.cItem : 0;
                UpdateShdefCost(lpshdefBuild);
                GetClientRect(hwndSlotDlg, &rc);
                DrawSlotDlg(hwndSlotDlg, NULL, &rc, iSrc);
                rc.top = yBuildInfoSum;
                InvalidateRect(hwndSlotDlg, &rc, 1);
                DrawBuildSelComp(hwndSlotDlg, NULL, -1);
                DrawBuildSelHull(hwndSlotDlg, NULL, -1, NULL);
                if (gd.fTutorial != 0) {
                    AdvanceTutor();
                }
            }
            return 1;
        }
        return 0;
    }
    hsDst = lpshdefBuild->hul.rghs[i];
    t_call_5761 = LphuldefFromId(lpshdefBuild->hul.ihuldef);
    hsHul.grhst = t_call_5761->hul.rghs[i].grhst;
    t_fields_1 = t_call_5761->hul.rghs[i].iItem;
    t_fields_2 = t_call_5761->hul.rghs[i].cItem;
    hsHul.iItem = t_fields_1;
    hsHul.cItem = t_fields_2;
    if ((hsHul.grhst & 1) != 0) {
        hsSrc.cItem = 100;
    }
    if (i == iSrc) {
        return 2;
    }
    if (hsDst.cItem >= hsHul.cItem ||
        ((hsDst.cItem > 0 && (hsDst.grhst != hsSrc.grhst || hsDst.iItem != hsSrc.iItem)) || (hsDst.cItem == 0 && (hsSrc.grhst & hsHul.grhst) == 0))) {
        if (fNoModify == 0) {
            MessageBeep(0);
        }
        return 3;
    }
    if (fNoModify == 0) {
        hsDst.grhst = hsSrc.grhst;
        hsDst.iItem = hsSrc.iItem;
        cNew = (uint16_t)(hsDst.cItem + hsSrc.cItem) >= hsHul.cItem ? hsHul.cItem : hsDst.cItem + hsSrc.cItem;
        if (iSrc >= 0) {
            lpshdefBuild->hul.rghs[iSrc].cItem -= cNew - hsDst.cItem;
        }
        hsDst.cItem = cNew;
        lpshdefBuild->hul.rghs[i] = hsDst;
        UpdateShdefCost(lpshdefBuild);
        SetBuildSelection(i);
        GetClientRect(hwndSlotDlg, &rc);
        rc.top = yBuildInfoSum;
        InvalidateRect(hwndSlotDlg, &rc, 1);
        if (gd.fTutorial != 0) {
            AdvanceTutor();
        }
    }
    return -1;
}

void DrawDlgLBEntireItem(DRAWITEMSTRUCT *lpdis, int16_t inflate) {
    COLORREF cr;
    COLORREF crForeSav;
    int16_t  ibmp;
    int16_t  bkSav;
    RECT     rc;

    CopyRect(&rc, &lpdis->rcItem);
    FillRect(lpdis->hDC, &lpdis->rcItem, GetStockObject((lpdis->itemState & 0x10) == 0 ? WHITE_BRUSH : BLACK_BRUSH));
    InflateRect(&rc, -2, -1);
    SendMessage(lpdis->hwndItem, LB_GETTEXT, lpdis->itemID, (LPARAM)szWork);
    SelectPalette(lpdis->hDC, vhpal, 0);
    RealizePalette(lpdis->hDC);
    ibmp = (int16_t)(int8_t)szWork[2] - 65 + ((int16_t)(int8_t)szWork[3] - 65) * 26;
    DibBlt(lpdis->hDC, rc.left, rc.top, 64, 64, rghdibInventory[ibmp >> 5], (ibmp & 7) * 0x40, (3 - (ibmp >> 3) & 3) * 0x40, 64, 64, 13369376);
    cr = (lpdis->itemState & 0x10) != 0 ? crWindow : crWindow == 0 ? 0xffffff : 0;
    crForeSav = SetTextColor(lpdis->hDC, cr);
    bkSav = SetBkMode(lpdis->hDC, TRANSPARENT);
    TextOut(lpdis->hDC, rc.left + 66, rc.top + 0x20 - (dyArial8 >> 1), &szWork[4], strlen(&szWork[4]));
    SetTextColor(lpdis->hDC, crForeSav);
    SetBkMode(lpdis->hDC, bkSav);
    HandleFocusState(lpdis, inflate + 2);
    return;
}

SHDEF *NthValidShdef(int16_t n) {
    int16_t i;

    if (fStarbaseMode != 0) {
        for (i = 0; i < 10; i++) {
            if (rglpshdefSB[idPlayer][i].fFree == 0 && n-- == 0) {
                return rglpshdefSB[idPlayer] + i;
            }
        }
    } else {
        for (i = 0; i < 16; i++) {
            if (rgshdef[i].fFree == 0 && n-- == 0) {
                return &rgshdef[i];
            }
        }
    }
    return NULL;
}

SHDEF *NthValidEnemyShdef(int16_t n) {
    int16_t i;
    int16_t j;

    if (fStarbaseMode != 0) {
        for (i = 0; i < game.cPlayer; i++) {
            if (rglpshdefSB[i] != 0 && i != idPlayer) {
                for (j = 0; j < 10; j++) {
                    if (rglpshdefSB[i][j].fFree == 0 && n-- == 0) {
                        return rglpshdefSB[i] + j;
                    }
                }
            }
        }
    } else {
        for (i = 0; i < game.cPlayer; i++) {
            if (rglpshdef[i] != 0 && i != idPlayer) {
                for (j = 0; j < 16; j++) {
                    if (rglpshdef[i][j].fFree == 0 && n-- == 0) {
                        return rglpshdef[i] + j;
                    }
                }
            }
        }
    }
    return NULL;
}

void FillBuildDD(HWND hwndDD, MdBuild md) {
    int16_t ishdefMac;
    int16_t fProgress;
    int16_t fAdded;
    int16_t i;
    int16_t j;
    SHDEF  *lpshdef;
    RECT    rc;
    PART    part;
    HWND    t_call_6005;

    SendMessage(hwndDD, CB_RESETCONTENT, 0, 0);
    if (md != mdBuildShdef) {
        EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 0);
        EnableWindow(GetDlgItem(hwndSlotDlg, IDC_EDIT), 0);
    }
    if (fStarbaseMode != 0) {
        ishdefMac = 10;
        lpshdef = rglpshdefSB[idPlayer];
    } else {
        ishdefMac = 16;
        lpshdef = rgshdef;
    }
    for (i = 0; i < ishdefMac && lpshdef[i].fFree == 0; i++) {
    }
    switch (md) {
    case mdBuildShdef:
    case mdBuildHuldef:
    case mdBuildEnemyShdef:
        if (i < ishdefMac) {
            fAdded = 1;
            break;
        }
    default:
        fAdded = 0;
    }
    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_IMPORT), fAdded);
    switch (md) {
    case mdBuildShdef:
    default:
        fAdded = 0;
        for (i = 0; i < ishdefMac; i++) {
            if (lpshdef[i].fFree == 0) {
                if (fAdded == 0) {
                    CshQueued((fStarbaseMode == 0 ? 0 : 16) + i, &fProgress, 0);
                    t_call_6005 = GetDlgItem(hwndSlotDlg, IDC_EDIT);
                    EnableWindow(t_call_6005, lpshdef[i].cExist == 0 && fProgress == 0);
                    EnableWindow(GetDlgItem(hwndSlotDlg, IDC_DELETE), 1);
                    fAdded = (lpshdef[i].cExist == 0 ? 0 : 1) + 1;
                }
                SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)lpshdef[i].hul.szClass);
            }
        }
        break;
    case mdBuildEnemyShdef:
        for (i = 0; i < game.cPlayer; i++) {
            if (i != idPlayer) {
                lpshdef = fStarbaseMode == 0 ? rglpshdef[i] : rglpshdefSB[i];
                if (lpshdef != 0) {
                    for (j = 0; j < ishdefMac; j++) {
                        if (lpshdef[j].fFree == 0) {
                            if (PszPlayerName(i, 1, 0, 0, 0, NULL) != szWork) {
                            }
                            _wsprintf(&szWork[strlen(szWork)], " %s", lpshdef[j].hul.szClass);
                            SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)szWork);
                        }
                    }
                }
            }
        }
        break;
    case mdBuildHuldef:
        if (fStarbaseMode != 0) {
            part.hs.grhst = hstSBHull;
            j = 5;
        } else {
            part.hs.grhst = hstHull;
            j = 32;
        }
        for (i = 0; i < j; i++) {
            part.hs.iItem = i;
            if (FLookupPart(&part) == 1) {
                SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)part.pcom->szName);
            }
        }
        break;
    case mdBuildComp:
    case mdBuildEdit:
        if (fStarbaseMode != 0) {
            for (i = 0; i < 8; i++) {
                SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(rgidsPartsSB[i]));
            }
        } else {
            for (i = 0; i < 13; i++) {
                SendMessage(hwndDD, CB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(rgidsParts[i]));
            }
        }
    }
    i = LOWORD(SendMessage(hwndDD, CB_GETCOUNT, 0, 0));
    if (i > 32) {
        i = 32;
    }
    GetWindowRect(hwndDD, &rc);
    SetWindowPos(hwndDD, NULL, 0, 0, rc.right - rc.left, (i + 1) * (dyArial8 - (dyArial8 > 14 ? 0 : 1)) + 8 + (dyArial8 <= 14 ? 0 : 1),
                 SWP_NOMOVE | SWP_NOZORDER);
    SendMessage(hwndDD, CB_SETCURSEL, 0, 0);
    return;
}

void FillBuildPartsLB(HWND hwndLB, int16_t grbit) {
    int16_t mdAvail;
    int16_t i;
    char    sz[200];
    int16_t grbitCur;
    PART    part;

    grbitCur = 1;
    sz[0] = 'A';
    SendMessage(hwndLB, LB_RESETCONTENT, 0, 0);
    while (grbitCur != 0) {
        if ((grbitCur & grbit) != 0) {
            i = 0;
            part.hs.grhst = grbitCur;
            while (1) {
                part.hs.iItem = i;
                mdAvail = FLookupPart(&part);
                if (mdAvail == 0)
                    break;
                if (fStarbaseMode != 0 && grbitCur == 2048 && (i == 15 || i == 16)) {
                    mdAvail = -1;
                }
                if (mdAvail == 1) {
                    sz[1] = LOBYTE(i + 65);
                    sz[2] = LOBYTE(part.pcom->ibmp % 26 + 65);
                    sz[3] = LOBYTE(part.pcom->ibmp / 26 + 65);
                    fstrcpy(&sz[4], part.pcom->szName);
                    SendMessage(hwndLB, LB_ADDSTRING, 0, (LPARAM)sz);
                }
                i++;
            }
        }
        grbitCur *= 2;
        sz[0]++;
    }
    return;
}

void UpdateSlotGlobals() {
    int16_t  yTop;
    int16_t  cSlot;
    int16_t  i;
    uint16_t wrc;
    int16_t  xLeft;
    HULDEF  *lphuldef;

    if (lpshdefBuild == 0) {
        cSlot = 0;
    } else {
        lphuldef = LphuldefFromId(lpshdefBuild->hul.ihuldef);
        if (hwndSlotDlg == 0) {
            xLeft = 12;
            yTop = dyArial8 + 12;
        } else {
            xLeft = ptslotGlob.x - 330;
            yTop = 32;
        }
        cSlot = lphuldef->hul.chs;
        for (i = 0; i < cSlot; i++) {
            vrgrcSlot[i].left = (lphuldef->rgbrc[i] & 0xf) * 0x20 + xLeft;
            vrgrcSlot[i].top = (lphuldef->rgbrc[i] >> 4) * 0x20 + yTop;
            vrgrcSlot[i].right = vrgrcSlot[i].left + 64;
            vrgrcSlot[i].bottom = vrgrcSlot[i].top + 64;
        }
        if (lphuldef->hul.wtCargoMax != 0) {
            wrc = lphuldef->wrcCargo;
            rcCargo.left = (wrc >> 8 & 0xff & 0xf) * 0x20 + xLeft;
            rcCargo.top = ((wrc >> 8 & 0xff) >> 4) * 0x20 + yTop;
            rcCargo.right = (wrc & 0xff & 0xff & 0xf) * 0x20 + xLeft;
            rcCargo.bottom = ((wrc & 0xff & 0xff) >> 4) * 0x20 + yTop;
        }
        ptPlaque.x = xLeft + 258;
        ptPlaque.y = yTop + 273;
    }
    return;
}

int16_t IEmptyBmpFromGrhst(int16_t grhst) {
    int16_t i;

    for (i = 0; i < 21; i++) {
        if (rgmapBuildBmps[i] == grhst) {
            return i;
        }
    }
    return 0;
}

LRESULT CALLBACK FakeListProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    POINT16 pt;
    int16_t iSel;
    POINT   t_pt_6771;
    POINT   t_pt_6780_1;

    switch (msg) {
    case WM_SETCURSOR:
        GetCursorPos(&t_pt_6771);
        pt = PointTo16(t_pt_6771);
        t_pt_6780_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_6780_1);
        pt = PointTo16(t_pt_6780_1);
        if (pt.x >= 64)
            goto L_6924;
        SetCursor(hcurHand);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        if (LOWORD(lParam) < 64 && (mdBuild == mdBuildEdit || msg == WM_RBUTTONDOWN)) {
            CallWindowProc(lpfnRealListProc, hwnd, 513, wParam, lParam);
            CallWindowProc(lpfnRealListProc, hwnd, 514, wParam, lParam);
            if (msg == WM_RBUTTONDOWN || mdBuild != mdBuildEdit) {
                iSel = LOWORD(SendMessage(hwnd, LB_GETCURSEL, 0, 0));
                if (iSel == -1) {
                    return 0;
                }
                SendMessage(hwnd, LB_GETTEXT, iSel, (LPARAM)szWork);
                GlobalPD.part.hs.grhst = 1 << ((int16_t)(int8_t)szWork[0] - 0x41);
                GlobalPD.part.hs.iItem = (int16_t)(int8_t)szWork[1] - 65;
                FLookupPart(&GlobalPD.part);
                GlobalPD.grPopup = grPopupComponent;
                Popup(hwnd, LOWORD(lParam), HIWORD(lParam));
                return 0;
            }
            FTrackSlot(hwnd, LOWORD(lParam), HIWORD(lParam), wParam, 1, 0);
            return 0;
        }
    default:
    L_6924:
        return CallWindowProc(lpfnRealListProc, hwnd, msg, wParam, lParam);
    }
}

void MakeNewName(char *lpsz) {
    int16_t cLen;

    cLen = fstrlen(lpsz);
    if (cLen <= 27) {
        if ((int16_t)(int8_t)lpsz[cLen - 1] != 41 || isdigit((int16_t)(int8_t)lpsz[cLen - 2]) == 0 || (int16_t)(int8_t)lpsz[cLen - 3] != 40) {
            fstrcpy(lpsz + cLen, " (2)");
        } else if ((int16_t)(int8_t)lpsz[cLen - 2] == 57) {
            lpsz[cLen - 2] = '0';
        } else {
            lpsz[cLen - 2] = lpsz[cLen - 2] + 1;
        }
    }
    FStringFitsScreen(lpsz, 160);
    return;
}

void KillQueuedMassPackets(PLANET *lppl) {
    int16_t iprod;
    int16_t iDst;
    PROD   *lpprod;

    if (lppl->lpplprod != 0 && lppl->lpplprod->iprodMac != 0) {
        iDst = 0;
        iprod = 0;
        lpprod = lppl->lpplprod->rgprod;
        while (iprod < lppl->lpplprod->iprodMac) {
            if (lpprod->grobj != grobjPlanet || lpprod->iItem < iobjPacketIron || lpprod->iItem > iobjPacketMixed) {
                if (iDst != iprod) {
                    lppl->lpplprod->rgprod[iDst] = *lpprod;
                }
                iDst++;
            }
            iprod++;
            lpprod++;
        }
        if (iDst == 0) {
            FreePl((PL *)lppl->lpplprod);
            lppl->lpplprod = NULL;
        } else if (iDst != iprod) {
            lppl->lpplprod->iprodMac = LOBYTE(iDst);
        }
        if (sel.grobj == grobjPlanet && sel.pl.id == lppl->id) {
            FLookupPlanet(sel.pl.id, &sel.pl);
            FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, NULL);
        }
    }
    return;
}

void KillQueuedShips(PLANET *lppl) {
    int16_t iprod;
    int16_t iDst;
    PROD   *lpprod;

    if (lppl->lpplprod != 0 && lppl->lpplprod->iprodMac != 0) {
        iDst = 0;
        iprod = 0;
        lpprod = lppl->lpplprod->rgprod;
        while (iprod < lppl->lpplprod->iprodMac) {
            if (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm) {
                if (lpprod->grobj == grobjFleet) {
                    lpprod->pct = 0;
                }
                if (iDst != iprod) {
                    lppl->lpplprod->rgprod[iDst] = *lpprod;
                }
                iDst++;
            }
            iprod++;
            lpprod++;
        }
        if (iDst == 0) {
            FreePl((PL *)lppl->lpplprod);
            lppl->lpplprod = NULL;
        } else if (iDst != iprod) {
            lppl->lpplprod->iprodMac = LOBYTE(iDst);
        }
        if (sel.grobj == grobjPlanet && sel.pl.id == lppl->id) {
            FLookupPlanet(sel.pl.id, &sel.pl);
            FillPlanetProdLB(hwndPlanetProdLB, sel.pl.lpplprod, NULL);
        }
    }
    return;
}
