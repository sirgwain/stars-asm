#include "common.h"

int16_t ChangeProduction(int16_t fClear) {
    jmp_buf  env;
    jmp_buf *penvMemSav;
    FARPROC  lpProcProd;
    PROD     rgprod[64];
    int16_t  fSuccess;

    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        if (lpplProdGlob != 0) {
            FreePl((PL *)lpplProdGlob);
        }
        lpplProdGlob = NULL;
        if (hwndProdDlg != 0) {
            EndDialog(hwndProdDlg, 0);
        }
        hwndProdDlg = 0;
        fDlgUp = 0;
        AlertSz(PszFormatIds(idsThereIsntEnoughFreeMemoryModifyProduction, NULL), MB_ICONHAND);
        penvMem = penvMemSav;
        return 0;
    }
    if (fClear != 0) {
        fSuccess = 1;
    } else {
        InitProduction(rgprod);
        fDlgUp = 1;
        lpProcProd = MakeProcInstance(ProductionDlg, hInst);
        fSuccess = DialogBox(hInst, MAKEINTRESOURCE(IDD_PRODUCTION), hwndFrame, lpProcProd);
        FreeProcInstance(lpProcProd);
        hwndProdDlg = 0;
        fDlgUp = 0;
    }
    FinishProduction(fSuccess);
    if (fSuccess != 0 && sel.grobj == grobjPlanet) {
        DrawPlanShip(NULL, 8);
    }
    penvMem = penvMemSav;
    return 1;
}

void InitProduction(PROD *rgprod) {
    int16_t  iWarp;
    int16_t  iSrc;
    uint16_t u;
    int16_t  i;
    int16_t  ipl;
    PART     part;
    PROD    *lpprod;
    uint16_t t_scratch_m1a;
    uint16_t t_scratch_m1a_2;
    uint16_t t_scratch_m1a_3;
    uint16_t t_scratch_m1a_4;
    uint16_t t_scratch_m1a_5;

    t_scratch_m1a = sel.pl.fNoResearch;
    gd.fNoResearchSav = t_scratch_m1a;
    if (rgprod == 0) {
        rgprod = pProdGlob;
    }
    if (sel.pl.lpplprod != 0) {
        i = sel.pl.lpplprod->iprodMac;
    } else {
        i = 2;
    }
    lpplProdGlob = (PLPROD *)LpplAlloc(4, i, htOrd);
    if (sel.pl.lpplprod != 0) {
        fmemcpy(lpplProdGlob->rgprod, sel.pl.lpplprod->rgprod, i * 4);
    } else {
        i = 0;
    }
    lpplProdGlob->iprodMac = LOBYTE(i);
    cProdGlob = 0;
    pProdGlob = rgprod;
    memset(rgprod, 0, 64 * sizeof(PROD));
    if (sel.pl.fStarbase != 0 && LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax != 0) {
        for (i = 0; i < 16; i++) {
            if (rgshdef[i].fFree == 0 && rgshdef[i].fGift == 0) {
                t_scratch_m1a_2 = rgshdef[i].hul.wtEmpty;
                if (LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax >= t_scratch_m1a_2) {
                    rgprod[cProdGlob].cItem = 0x3ff;
                    rgprod[cProdGlob].iItem = i;
                    rgprod[cProdGlob].grobj = grobjFleet;
                    cProdGlob++;
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        if (rglpshdefSB[idPlayer][i].fFree == 0 && rglpshdefSB[idPlayer][i].fGift == 0 && (sel.pl.isb != i || sel.pl.fStarbase == 0)) {
            rgprod[cProdGlob].cItem = 1;
            rgprod[cProdGlob].iItem = LOWORD((int16_t)(i + 16));
            rgprod[cProdGlob].grobj = grobjFleet;
            cProdGlob++;
        }
    }
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 14;
    if (FLookupPart(&part) == 1) {
        rgprod[cProdGlob].cItem = 1;
        rgprod[cProdGlob].iItem = iobjGenesis;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    iWarp = IWarpMAFromLppl(&sel.pl, NULL);
    if (iWarp > 0) {
        for (i = 0; i < 4; i++) {
            rgprod[cProdGlob].cItem = 0x3ff;
            rgprod[cProdGlob].iItem = LOWORD((int16_t)(i + 14));
            rgprod[cProdGlob].grobj = grobjPlanet;
            cProdGlob++;
        }
    }
    t_scratch_m1a_3 = sel.pl.cFactories;
    u = CMaxFactories(&sel.pl, idPlayer) - t_scratch_m1a_3;
    if (u > 0) {
        rgprod[cProdGlob].cItem = LOWORD(1020 >= u ? (uint32_t)u : 1020);
        rgprod[cProdGlob].iItem = mdIdleFactory;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    t_scratch_m1a_4 = sel.pl.cMines;
    u = CMaxMines(&sel.pl, idPlayer) - t_scratch_m1a_4;
    if (u > 0) {
        rgprod[cProdGlob].cItem = LOWORD(1020 >= u ? (uint32_t)u : 1020);
        rgprod[cProdGlob].iItem = mdIdleMine;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    t_scratch_m1a_5 = sel.pl.cDefenses;
    u = CMaxDefenses(&sel.pl, idPlayer) - t_scratch_m1a_5;
    if (u > 0) {
        rgprod[cProdGlob].cItem = u;
        rgprod[cProdGlob].iItem = mdIdleDefense;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    rgprod[cProdGlob].cItem = 0x3ff;
    rgprod[cProdGlob].iItem = mdIdleAlchemy;
    rgprod[cProdGlob].grobj = grobjPlanet;
    cProdGlob++;
    if (sel.pl.iScanner == 31 && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
        rgprod[cProdGlob].cItem = 1;
        rgprod[cProdGlob].iItem = iobjPlanetaryScanner;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    i = IpctCanTerraformLppl(&sel.pl);
    if (i > 0) {
        rgprod[cProdGlob].cItem = LOWORD((uint32_t)i);
        rgprod[cProdGlob].iItem = mdIdleTerraform;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob++;
    }
    for (i = 0; i < 7; i++) {
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            switch (i) {
            default:
                goto L_0c45;
            case 0:
            case 1:
            case 2:
                break;
            }
            continue;
        }
    L_0c45:
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raTerra || (i != 4 && i != 5)) {
            rgprod[cProdGlob].cItem = 0x3ff;
            rgprod[cProdGlob].iItem = LOWORD(i);
            rgprod[cProdGlob].grobj = grobjPlanet;
            cProdGlob++;
        }
    }
    ipl = 0;
    lpprod = lpplProdGlob->rgprod;
    while (ipl < lpplProdGlob->iprodMac) {
        for (iSrc = 0; iSrc < cProdGlob && ((uint32_t)pProdGlob[iSrc].grobj != lpprod->grobj || (uint32_t)pProdGlob[iSrc].iItem != lpprod->iItem); iSrc++) {
        }
        if (iSrc >= cProdGlob) {
            if (ipl + 1 < lpplProdGlob->iprodMac) {
                fmemcpy(lpprod, lpprod + 1, (lpplProdGlob->iprodMac - (ipl + 1)) * sizeof(PROD));
                ipl--;
            }
            lpplProdGlob->iprodMac--;
        } else {
            if (pProdGlob[iSrc].cItem < lpprod->cItem) {
                lpprod->cItem = LOWORD((uint32_t)pProdGlob[iSrc].cItem);
            }
            if (pProdGlob[iSrc].cItem != 0x3ff) {
                if (pProdGlob[iSrc].cItem < lpprod->cItem) {
                    pProdGlob[iSrc].cItem = 0;
                } else {
                    pProdGlob[iSrc].cItem -= lpprod->cItem;
                }
            }
        }
        ipl++;
        lpprod++;
    }
    if (gd.fTutorial != 0 && idPlayer == 0) {
        AdvanceTutor();
    }
    return;
}

void FinishProduction(int16_t fWrite) {
    if (fWrite != 0) {
        FreePl((PL *)sel.pl.lpplprod);
        if (lpplProdGlob != 0 && lpplProdGlob->iprodMac == 0) {
            FreePl((PL *)lpplProdGlob);
            lpplProdGlob = NULL;
        }
        sel.pl.lpplprod = lpplProdGlob;
        lpplProdGlob = NULL;
        FLookupPlanet(-1, &sel.pl);
        FLookupPlanet(sel.pl.id, &sel.pl);
        if (fAi == 0) {
            FillPlanetProdLB(NULL, NULL, NULL);
            DrawPlanShip(NULL, 64);
        }
    } else {
        sel.pl.fNoResearch = gd.fNoResearchSav;
        FreePl((PL *)lpplProdGlob);
    }
    lpplProdGlob = NULL;
    if (gd.fTutorial != 0 && idPlayer == 0) {
        tutor.fProgress = 1;
        AdvanceTutor();
    }
    return;
}

INT_PTR CALLBACK ProductionDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC                hdc;
    PAINTSTRUCT        ps;
    RECT               rc;
    int16_t            dxPBtn;
    int16_t            dy;
    RECT               rcT;
    int16_t            i;
    int16_t            xCtr;
    int16_t            dx;
    int16_t            dyLB;
    int16_t            rgidProdBtns[10];
    DRAWITEMSTRUCT    *lpdis;
    MEASUREITEMSTRUCT *lpmis;
    POINT16            pt;
    int16_t            cMax;
    char               sz255[2];
    char              *rgszZip[6];
    ZIPPRODQ           rgzp[4];
    FARPROC            lpProc;
    int16_t            fRet;
    HCURSOR            hcs;
    HWND               t_scratch_m2e;
    POINT              t_pt_188d;
    POINT              t_pt_189c_1;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        DrawProductionDlg(hwnd, hdc, &rc, -1);
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        switch (message) {
        case WM_INITDIALOG:
            rgidProdBtns[0] = 1070;
            rgidProdBtns[1] = 1071;
            rgidProdBtns[2] = 1;
            rgidProdBtns[3] = 2;
            rgidProdBtns[4] = 1048;
            rgidProdBtns[5] = 1049;
            rgidProdBtns[6] = 1081;
            rgidProdBtns[7] = 1082;
            rgidProdBtns[8] = 1069;
            rgidProdBtns[9] = 118;
            hwndProdDlg = hwnd;
            if (rgplr[idPlayer].cPlanet <= 1) {
                EnableWindow(GetDlgItem(hwnd, IDC_NEXT), 0);
                EnableWindow(GetDlgItem(hwnd, IDC_U16_0x042E), 0);
            }
            if (gd.mdScreenSize >= 1) {
                dx = 760;
                dy = 580;
            } else {
                dx = 610;
                dy = 24 * dyArial8 + 24;
            }
            SetWindowPos(hwnd, NULL, 0, 0, dx, dy, SWP_NOMOVE | SWP_NOZORDER);
            GetClientRect(hwnd, &rc);
            xCtr = rc.right >> 1;
            dyLB = rc.bottom - (int16_t)(17 * dyArial8) / 2 - 24;
            rc.left = (int16_t)(11 * rc.right) / 20 + 16;
            dxPBtn = (int16_t)(rc.right - rc.left) / 4;
            rc.bottom -= (int16_t)(3 * dyArial8) / 2 + 6;
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x008B), NULL, 6, rc.bottom, rc.left - 12, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
            for (i = 0; i < 4; i++) {
                SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), NULL, rc.left, rc.bottom, dxPBtn - 6, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                rc.left += dxPBtn;
            }
            dxPBtn += 24;
            rc.left = xCtr - ((dxPBtn - 6) >> 1);
            dy = (int16_t)(dyLB - 9 * dyArial8) / 5 + (int16_t)(3 * dyArial8) / 2 - 3;
            rc.top = 8;
            if (dy > 50) {
                rc.top += (int16_t)((dy - 50) * 5) / 2;
                dy = 50;
            }
            for (i = 4; i < 10; i++) {
                SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), NULL, rc.left, rc.top, dxPBtn - 6, (int16_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                rc.top += dy;
            }
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0416), NULL, 6, 6, rc.left - 12, dyLB, SWP_NOZORDER);
            GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0416), &rcT);
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0417), NULL, rc.left + dxPBtn, 6, rc.left - 12, rcT.bottom - rcT.top, SWP_NOZORDER);
            ScreenToClient(hwnd, (POINT *)&rcT.right);
            yTopFutureTech = rcT.bottom;
            InitializeProductionDlg(hwnd);
            if (gd.mdScreenSize == 1 && ptStickyProduceDlg.y == -1) {
                ptStickyProduceDlg.y = 0;
            }
            StickyDlgPos(hwnd, &ptStickyProduceDlg, 1);
            return 1;
        case WM_DRAWITEM:
            lpdis = (DRAWITEMSTRUCT *)lParam;
            if (lpdis->itemID == -1) {
                HandleFocusState(lpdis, -2);
            } else {
                switch (lpdis->itemAction) {
                case 1:
                case 2:
                case 4:
                    DrawCBEntireItem(lpdis, 4);
                }
            }
            return 1;
        case WM_MEASUREITEM:
            lpmis = (MEASUREITEMSTRUCT *)lParam;
            lpmis->itemHeight = dyArial8 + 2;
            return 1;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            if (PtInRect(&rcProdDiamond, PointFrom16(pt)) == 0)
                break;
            if (message == WM_LBUTTONDOWN) {
                GlobalPD.grPopup = grPopupString;
                GlobalPD.dxOut = 180;
                GlobalPD.psz = szPopupBuffer;
                CchGetString(idsRightClickBlueDiamondApplyProductionTemplate, szPopupBuffer);
                Popup(hwnd, pt.x, pt.y);
                break;
            }
            sz255[0] = -1;
            sz255[1] = 0;
            cMax = 0;
            for (i = 0; i < 4; i++) {
                if (vrgZipProd[i].fValid != 0) {
                    rgszZip[cMax++] = vrgZipProd[i].szName;
                }
            }
            rgszZip[cMax++] = sz255;
            rgszZip[cMax++] = PszGetCompressedString(idsCustomize);
            i = PopupMenu(hwnd, pt.x, pt.y, cMax, NULL, rgszZip, -1, 1);
            if (i == cMax - 1) {
                memcpy(rgzp, vrgZipProd, 160);
                lpProc = MakeProcInstance(ZipProdDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_ZIP_PROD), hwnd, lpProc);
                FreeProcInstance(lpProc);
                if (fRet != 0)
                    break;
                memcpy(vrgZipProd, rgzp, 160);
                break;
            }
            if (i < 0)
                break;
            for (cMax = 0; cMax < 4 && (vrgZipProd[cMax].fValid == 0 || i-- != 0); cMax++) {
            }
            ProdCommandHandler(hwnd, 0x816, cMax);
            break;
        case WM_SETCURSOR:
            hcs = 0;
            GetCursorPos(&t_pt_188d);
            pt = PointTo16(t_pt_188d);
            t_pt_189c_1 = PointFrom16(pt);
            ScreenToClient(hwnd, &t_pt_189c_1);
            pt = PointTo16(t_pt_189c_1);
            if (PtInRect(&rcProdDiamond, PointFrom16(pt)) == 0)
                break;
            SetCursor(hcurArrowHelp);
            return 1;
        case WM_COMMAND:
            ProdCommandHandler(hwnd, wParam, lParam);
        }
    } else {
        t_scratch_m2e = GET_WM_CTLCOLOR_HWND(wParam, lParam);
        if (t_scratch_m2e == GetDlgItem(hwnd, IDC_U16_0x008B) || HIWORD(lParam) == 6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
}

void ProdCommandHandler(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    int32_t  lSel;
    int16_t  iSrc;
    HWND     hwndLB;
    int16_t  c;
    int16_t  iDst;
    PROD     prodLast;
    int16_t  ipl;
    int16_t  fRefillSrc;
    int16_t  iMac;
    RECT     rc;
    PROD    *lpprod;
    PROD     prod;
    int16_t  cMax;
    PLPROD  *lpplprodT;
    uint32_t t_merge_1e76_0001_wide;
    uint16_t t_scratch_m34;

    switch (GET_WM_COMMAND_ID(wParam, lParam)) {
    case 0x418:
    AddItem:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_GETCURSEL, 0, 0);
        if (lSel < 0)
            break;
        for (iSrc = 0; iSrc < cProdGlob && (pProdGlob[iSrc].cItem == 0 || lSel-- != 0); iSrc++) {
        }
        prod = pProdGlob[iSrc];
        if ((GetAsyncKeyState(VK_CONTROL) & 0xfffe) != 0) {
            if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
                prod.cItem = LOWORD(prod.cItem < 1020 ? (uint32_t)prod.cItem : 1020);
            } else {
                prod.cItem = LOWORD(prod.cItem < 100 ? (uint32_t)prod.cItem : 100);
            }
        } else if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
            prod.cItem = LOWORD(prod.cItem < 10 ? (uint32_t)prod.cItem : 10);
        } else {
            prod.cItem = 1;
        }
        if (pProdGlob[iSrc].cItem != 0x3ff) {
            pProdGlob[iSrc].cItem -= prod.cItem;
        }
        iMac = lpplProdGlob->iprodMac;
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0, 0);
        if (lSel < 0) {
            iDst = iMac - 1;
            if (iDst < 0) {
                iDst = 0;
            }
        } else {
            iDst = LOWORD(lSel) - 1;
        }
        if (iDst < iMac) {
            if (iDst >= 0) {
                prodLast = lpplProdGlob->rgprod[iDst];
                if ((uint32_t)prodLast.iItem == prod.iItem && (uint32_t)prodLast.grobj == prod.grobj)
                    goto RingItUp;
            }
            iDst++;
            if (iDst >= iMac)
                goto L_2099;
            prodLast = lpplProdGlob->rgprod[iDst];
            if ((uint32_t)prodLast.iItem != prod.iItem || (uint32_t)prodLast.grobj != prod.grobj)
                goto L_2099;
        RingItUp:
            if (1020 < lpplProdGlob->rgprod[iDst].cItem + (uint32_t)prod.cItem) {
                t_merge_1e76_0001_wide = 1020;
            } else {
                t_scratch_m34 = prod.cItem;
                t_merge_1e76_0001_wide = (uint32_t)lpplProdGlob->rgprod[iDst].cItem + (uint32_t)t_scratch_m34;
            }
            lpplProdGlob->rgprod[iDst].cItem = LOWORD(t_merge_1e76_0001_wide);
            if (lpplProdGlob->rgprod[iDst].cItem <= 1 || lpplProdGlob->rgprod[iDst].iItem != iobjAlchemy || lpplProdGlob->rgprod[iDst].grobj != grobjPlanet)
                goto FixedUp;
            lpplProdGlob->rgprod[iDst].cItem = 1;
            goto FixedUp;
        }
    L_2099:
        if (iMac >= 40) {
            MessageBeep(0);
            goto RedrawText;
        }
        if (iMac == lpplProdGlob->iprodMax) {
            lpplProdGlob = (PLPROD *)LpplReAlloc((PL *)lpplProdGlob, iMac + 4);
        }
        if (iDst != iMac) {
            fmemmove(lpplProdGlob + (1 + (iDst + 1)), &lpplProdGlob->rgprod[iDst], (iMac - iDst) * 4);
        }
        lpplProdGlob->rgprod[iDst] = prod;
        lpplProdGlob->iprodMac++;
    FixedUp:
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, iDst + 1, 0);
        if (pProdGlob[iSrc].cItem != 0)
            goto RedrawText;
        FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
        goto RedrawText;
    case 0x419:
    RemoveItem:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0, 0);
        if (lSel <= 0)
            break;
        iMac = lpplProdGlob->iprodMac;
        lSel--;
        prod = lpplProdGlob->rgprod[lSel];
        for (iSrc = 0; iSrc < cProdGlob && ((uint32_t)pProdGlob[iSrc].grobj != prod.grobj || (uint32_t)pProdGlob[iSrc].iItem != prod.iItem); iSrc++) {
        }
        fRefillSrc = pProdGlob[iSrc].cItem == 0 ? 1 : 0;
        if ((GetAsyncKeyState(VK_CONTROL) & 0xfffe) != 0) {
            if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
                c = 1020;
            } else {
                c = 100;
            }
        } else if ((GetAsyncKeyState(VK_SHIFT) & 0xfffe) != 0) {
            c = 10;
        } else {
            c = 1;
        }
        if (prod.grobj == grobjPlanet && prod.iItem == iobjAlchemy) {
            c = 1020;
        }
        c = c >= prod.cItem ? prod.cItem : c;
        if (pProdGlob[iSrc].cItem != 0x3ff) {
            pProdGlob[iSrc].cItem += c;
        }
        lpplProdGlob->rgprod[lSel].cItem -= c;
        if (lpplProdGlob->rgprod[lSel].cItem == 0) {
            if ((int32_t)(lSel + 1) < iMac) {
                fmemmove(&lpplProdGlob->rgprod[lSel], &lpplProdGlob->rgprod[lSel + 1], (iMac - LOWORD(lSel) - 1) * sizeof(PROD));
            } else {
                lSel--;
            }
            lpplProdGlob->iprodMac--;
        }
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
        if (lSel >= 0) {
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 1, 0);
        }
        if (fRefillSrc == 0)
            goto RedrawText;
        FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
        goto RedrawText;
    case 0x42d:
    case IDC_IMPORT:
        ipl = 0;
        lpprod = lpplProdGlob->rgprod;
        while (ipl < lpplProdGlob->iprodMac) {
            for (iSrc = 0; iSrc < cProdGlob && ((uint32_t)pProdGlob[iSrc].grobj != lpprod->grobj || (uint32_t)pProdGlob[iSrc].iItem != lpprod->iItem); iSrc++) {
            }
            if (pProdGlob[iSrc].cItem != 0x3ff) {
                pProdGlob[iSrc].cItem += lpprod->cItem;
            }
            ipl++;
            lpprod++;
        }
        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_IMPORT) {
            cMax = lpplProdGlob->iprodMac + vrgZipProd[lParam].cpq;
            if (cMax < 1) {
                cMax = 1;
            }
            lpplprodT = (PLPROD *)LpplAlloc(4, cMax, htOrd);
            fmemset(lpplprodT->rgprod, 0, cMax * 4);
            iDst = 0;
            for (iSrc = 0; iSrc < lpplProdGlob->iprodMac; iSrc++) {
                if (lpplProdGlob->rgprod[iSrc].grobj != grobjPlanet || lpplProdGlob->rgprod[iSrc].iItem >= mdIdleFactory) {
                    lpplprodT->rgprod[iDst] = lpplProdGlob->rgprod[iSrc];
                    iDst++;
                }
            }
            for (iSrc = 0; iSrc < vrgZipProd[lParam].cpq; iSrc++) {
                if ((GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh || vrgZipProd[lParam].rgpq[iSrc].mdIdle > 2) &&
                    (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raTerra ||
                     (vrgZipProd[lParam].rgpq[iSrc].mdIdle != 4 && vrgZipProd[lParam].rgpq[iSrc].mdIdle != 5))) {
                    lpplprodT->rgprod[iDst].grobj = grobjPlanet;
                    lpplprodT->rgprod[iDst].iItem = vrgZipProd[lParam].rgpq[iSrc].mdIdle;
                    lpplprodT->rgprod[iDst].cItem = vrgZipProd[lParam].rgpq[iSrc].cQuan;
                    iDst++;
                }
            }
            lpplprodT->iprodMac = LOBYTE(iDst);
            FreePl((PL *)lpplProdGlob);
            lpplProdGlob = lpplprodT;
            sel.pl.fNoResearch = vrgZipProd[lParam].fNoResearch;
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), BM_SETCHECK, sel.pl.fNoResearch, 0);
        } else {
            lpplProdGlob->iprodMac = 0;
        }
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, 0, 0);
        FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_SETCURSEL, 0, 0);
        goto RedrawText;
    case IDC_U16_0x0416:
    case IDC_U16_0x0417:
        if (GET_WM_COMMAND_CMD(wParam, lParam) == 1)
            goto RedrawText;
        if (GET_WM_COMMAND_CMD(wParam, lParam) != 2)
            break;
        if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x0416)
            goto AddItem;
        goto RemoveItem;
    case IDC_U16_0x008B:
        hwndLB = GetDlgItem(hwnd, IDC_U16_0x0417);
        sel.pl.fNoResearch = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), BM_GETCHECK, 0, 0));
        lSel = SendMessage(hwndLB, LB_GETCURSEL, 0, 0);
        FillPlanetProdLB(hwndLB, lpplProdGlob, NULL);
        SendMessage(hwndLB, LB_SETCURSEL, LOWORD(lSel), 0);
        goto RedrawText;
    case IDOK:
    case IDCANCEL:
        hwndProdDlg = 0;
        StickyDlgPos(hwnd, &ptStickyProduceDlg, 0);
        EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
        break;
    case IDC_U16_0x042E:
    case IDC_NEXT:
        c = GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT ? 1 : -1;
        FinishProduction(1);
        if (GetKeyState(VK_SHIFT) < 0) {
            SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT ? 1 : 0));
        } else {
            SelectAdjPlanet(c, 0);
        }
        InitProduction(NULL);
        InitializeProductionDlg(hwnd);
        GetClientRect(hwnd, &rc);
        rc.top = yTopFutureTech;
        rc.bottom = 9 * dyArial8 + rc.top;
        InvalidateRect(hwnd, &rc, 1);
        break;
    case 0x43a:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0, 0);
        iMac = lpplProdGlob->iprodMac;
        if (lSel <= 0 || lSel >= iMac)
            break;
        lSel--;
        prod = lpplProdGlob->rgprod[lSel];
        lpplProdGlob->rgprod[lSel] = lpplProdGlob->rgprod[lSel + 1];
        lpplProdGlob->rgprod[lSel + 1] = prod;
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 2, 0);
        goto RedrawText;
    case 0x439:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0, 0);
        if (lSel <= 1)
            break;
        iMac = lpplProdGlob->iprodMac;
        lSel -= 2;
        prod = lpplProdGlob->rgprod[lSel];
        lpplProdGlob->rgprod[lSel] = lpplProdGlob->rgprod[lSel + 1];
        lpplProdGlob->rgprod[lSel + 1] = prod;
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 1, 0);
        goto RedrawText;
    case IDC_HELP:
        WinHelp(hwnd, szHelpFile, 1, 1059);
    }
    return;
RedrawText:
    GetClientRect(hwnd, &rc);
    rc.top = yTopFutureTech;
    rc.bottom = 7 * dyArial8 + rc.top;
    rc.left += 130;
    InvalidateRect(hwnd, &rc, 1);
    DrawProductionDlg(hwnd, NULL, &rc, -1);
    return;
}

void InitializeProductionDlg(HWND hwnd) {
    char    rgch[86];
    int16_t i;
    int16_t iSel;
    PROD   *lpprod;

    iSel = -1;
    _wsprintf(rgch, PszGetCompressedString(idsProductionQueueS), PszGetPlanetName(sel.pl.id));
    SetWindowText(hwnd, rgch);
    FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_SETCURSEL, 0, 0);
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    while (i < lpplProdGlob->iprodMac) {
        if (lpprod->grobj != grobjPlanet || lpprod->iItem >= mdIdleFactory) {
            iSel = i;
        }
        i++;
        lpprod++;
    }
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, NULL);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, iSel + 1, 0);
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), BM_SETCHECK, sel.pl.fNoResearch, 0);
    return;
}

void DrawProductionDlg(HWND hwnd, HDC hdc, RECT *prc, int16_t iDraw) {
    int32_t lSel;
    int16_t iSrc;
    int16_t idc;
    int16_t fCreatedDC;
    int16_t i;
    int16_t c;
    int32_t rgCost[4];
    int16_t dxkT;
    int16_t k;
    RECT    rc;
    PROD    prod;
    char    szT[100];

    fCreatedDC = 0;
    if (hdc == 0) {
        fCreatedDC = 1;
        hdc = GetDC(hwnd);
    }
    SelectObject(hdc, rghfontArial8[0]);
    dxkT = LOWORD(GetTextExtent(hdc, PszGetCompressedString(idsKt), 2));
    SelectObject(hdc, rghfontArial8[1]);
    SetBkColor(hdc, crButtonFace);
    for (i = 0; i < 2; i++) {
        idc = i == 0 ? 1046 : 1047;
        GetWindowRect(GetDlgItem(hwnd, idc), &rc);
        ScreenToClient(hwnd, (POINT *)&rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        lSel = SendMessage(GetDlgItem(hwnd, idc), 0x409, 0, 0);
        if (lSel >= 0 && (lSel != 0 || i != 1)) {
            if (i == 0) {
                for (iSrc = 0; iSrc < cProdGlob && (pProdGlob[iSrc].cItem == 0 || lSel-- != 0); iSrc++) {
                }
                prod = pProdGlob[iSrc];
                prod.cItem = 1;
            } else {
                lSel--;
                prod = lpplProdGlob->rgprod[lSel];
            }
            GetProductionCosts(&sel.pl, &prod, rgCost, idPlayer, 0);
            rc.bottom = yTopFutureTech + 4;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsRequiredMinerals, szWork);
            TextOut(hdc, rc.left, rc.bottom, szWork, c);
            rc.left += 20;
            rc.right -= 20;
            for (k = 0; k <= 3; k++) {
                c = k == 3 ? 5 : k;
                rc.bottom += dyArial8;
                SelectObject(hdc, rghfontArial8[1]);
                SetTextColor(hdc, rgcrMinerals[c]);
                TextOut(hdc, rc.left, rc.bottom, rgszMinerals[c], lstrlen(rgszMinerals[c]));
                SelectObject(hdc, rghfontArial8[0]);
                SetTextColor(hdc, crWindowText);
                c = _wsprintf(szWork, PCTLD, rgCost[k]);
                RightTextOut(hdc, rc.right - dxkT - 2, rc.bottom, szWork, c, dxMaxMineralQuan);
                if (k <= 2) {
                    TextOut(hdc, rc.right - dxkT, rc.bottom, PszGetCompressedString(idsKt), 2);
                }
            }
            if (i != 0) {
                rc.bottom += (int16_t)(3 * dyArial8) / 2;
                SelectObject(hdc, rghfontArial8[1]);
                c = _wsprintf(szT, PszGetCompressedString(idsDDoneCompletion), prod.pct);
                if (PszProductionETA(&sel.pl, lpplProdGlob, LOWORD(lSel), NULL, NULL) != szWork) {
                }
                strcpy(&szT[c], szWork);
                TextOut(hdc, rc.left - 20, rc.bottom, szT, strlen(szT));
                SelectObject(hdc, rghfontArial8[0]);
            }
        }
    }
    GetClientRect(hwnd, &rc);
    rc.top = rc.bottom - ((int16_t)(5 * dyArial8) / 2 + 12);
    rc.bottom = (dyArial8 | 1) + rc.top;
    rc.left = 6;
    rc.right = rc.left + dyArial8;
    DrawDiamond(hdc, &rc, hbrBBlue);
    rcProdDiamond = rc;
    SelectObject(hdc, rghfontArial8[1]);
    c = CchGetString(idsApplyDefineProductionTemplate, szWork);
    TextOut(hdc, rc.right + 4, rc.top, szWork, c);
    if (fCreatedDC != 0) {
        ReleaseDC(hwnd, hdc);
    }
    return;
}

void FillProdSrcLB(HWND hwndLB, int16_t mdFill) {
    char    szT[80];
    int16_t i;
    char   *psz;

    for (i = 0; i < 6; i++) {
        szT[i] = ' ';
    }
    SendMessage(hwndLB, LB_RESETCONTENT, 0, 0);
    for (i = 0; i < cProdGlob; i++) {
        if (pProdGlob[i].cItem > 0) {
            psz = PszNameProdItem(pProdGlob + i);
            strcpy(&szT[6], psz);
            if (pProdGlob[i].grobj == grobjFleet) {
                szT[0] = LOBYTE(pProdGlob[i].iItem < iobjPacketGerm ? 42 : 35);
            } else if (pProdGlob[i].iItem < mdIdleFactory) {
                szT[0] = 'I';
                strcat(&szT[6], " (Auto Build)");
            } else {
                szT[0] = ' ';
            }
            SendMessage(hwndLB, LB_ADDSTRING, 0, (LPARAM)szT);
        }
    }
    return;
}

char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

    iItem = lpprod->iItem;
    if (lpprod->grobj == grobjFleet) {
        if (iItem >= 16) {
            iItem -= 16;
            if (rglpshdefSB[idPlayer][iItem].fFree == 0) {
                fstrcpy(szWork, rglpshdefSB[idPlayer][iItem].hul.szClass);
                if (sel.pl.fStarbase == 0) {
                    return szWork;
                }
                iDelta = rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - rglpshdefSB[idPlayer][iItem].hul.ihuldef;
                if (iDelta > 0) {
                    strcat(szWork, " (downgrade)");
                    return szWork;
                }
                if (iDelta >= 0) {
                    return szWork;
                }
                strcat(szWork, " (upgrade)");
                return szWork;
            }
        } else if (rgshdef[iItem].fFree == 0) {
            strcpy(szWork, rgshdef[iItem].hul.szClass);
            return szWork;
        }
        szWork[0] = 0;
        return szWork;
    }
    if (iItem >= 18 && iItem <= 26) {
        fstrcpy(szWork, LpplanetaryFromId(LOWORD(iItem) - 18)->szName);
    } else if (iItem == 27) {
        CchGetString(idsPlanetaryScanner, szWork);
    } else {
        CchGetString(LOWORD(iItem) + 126, szWork);
    }
    return szWork;
}

void GetProductionCosts(PLANET *lppl, PROD *lpprod, uint32_t *rgCost, int16_t iplr, int16_t fOnlyOne) {
    uint16_t rgCostsCur[4];
    uint32_t iItem;
    uint16_t rgCosts[4];
    int16_t  i;
    int16_t  j;
    SHDEF   *lpshdef;
    int16_t  raMajor;
    uint32_t cItem;
    int16_t  fStarbase;
    PART     part;
    int16_t  cost;
    int16_t  chs;
    HUL     *lphulNew;
    HUL     *lphulCur;
    int16_t  costUpg;
    int16_t  costHalf;
    HUL     *lphulT;
    int16_t  rgCostsPartCur[4];
    int16_t  rgCostsPartNew[4];
    uint32_t t_merge_4b68_0001;

    raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
    fStarbase = 0;
    iItem = lpprod->iItem;
    cItem = lpprod->cItem;
    if (lpprod->grobj == grobjFleet) {
        if (iItem >= iobjPacketGerm) {
            lpshdef = rglpshdefSB[iplr];
            iItem -= 16;
            fStarbase = 1;
        } else {
            lpshdef = rglpshdef[iplr];
        }
        if (lpshdef[iItem].fFree != 0) {
            for (i = 0; i < 4; i++) {
                rgCost[i] = 0;
            }
            return;
        }
        GetTrueHullCost(iplr, &lpshdef[iItem].hul, rgCosts);
        if (fStarbase != 0 && lppl->fStarbase != 0) {
            lphulCur = &rglpshdefSB[iplr][lppl->isb].hul;
            lphulNew = &lpshdef[iItem].hul;
            GetTrueHullCost(iplr, lphulCur, rgCostsCur);
            if (lphulCur->ihuldef != lphulNew->ihuldef) {
                for (i = 0; i < 4; i++) {
                    costHalf = (uint32_t)rgCosts[i] / 2;
                    costUpg = rgCosts[i] - (int16_t)rgCostsCur[i] / 2;
                    if (costHalf > costUpg) {
                        rgCosts[i] = costHalf;
                    } else {
                        rgCosts[i] = costUpg;
                    }
                }
            } else {
                lphulT = &LphuldefFromId(lphulCur->ihuldef)->hul;
                part.hs.grhst = hstNone;
                part.phul = lphulT;
                GetTruePartCost(iplr, &part, rgCostsPartCur);
                for (i = 0; i < 4; i++) {
                    rgCosts[i] -= rgCostsPartCur[i];
                }
                chs = lphulCur->chs;
                for (i = 0; i < chs; i++) {
                    if (lphulCur->rghs[i].cItem != 0 && lphulNew->rghs[i].cItem != 0) {
                        part.hs = lphulCur->rghs[i];
                        FLookupPart(&part);
                        GetTruePartCost(iplr, &part, rgCostsPartCur);
                        part.hs = lphulNew->rghs[i];
                        FLookupPart(&part);
                        GetTruePartCost(iplr, &part, rgCostsPartNew);
                        if (lphulCur->rghs[i].grhst != lphulNew->rghs[i].grhst) {
                            for (j = 0; j < 4; j++) {
                                rgCostsPartCur[j] *= lphulCur->rghs[i].cItem;
                                rgCostsPartNew[j] *= lphulNew->rghs[i].cItem;
                                cost = (int16_t)(3 * rgCostsPartNew[j]) / 10 <= rgCostsPartNew[j] - (int16_t)(7 * rgCostsPartCur[j]) / 10
                                           ? rgCostsPartNew[j] - (int16_t)(7 * rgCostsPartCur[j]) / 10
                                           : (int16_t)(3 * rgCostsPartNew[j]) / 10;
                                rgCosts[j] -= rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j];
                            }
                        } else if (lphulCur->rghs[i].iItem != lphulNew->rghs[i].iItem) {
                            for (j = 0; j < 4; j++) {
                                rgCostsPartCur[j] *= lphulCur->rghs[i].cItem;
                                rgCostsPartNew[j] *= lphulNew->rghs[i].cItem;
                                cost = (int16_t)(rgCostsPartNew[j] * 2) / 10 <= rgCostsPartNew[j] - (int16_t)(rgCostsPartCur[j] * 8) / 10
                                           ? rgCostsPartNew[j] - (int16_t)(rgCostsPartCur[j] * 8) / 10
                                           : (int16_t)(rgCostsPartNew[j] * 2) / 10;
                                rgCosts[j] -= rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j];
                            }
                        } else {
                            for (j = 0; j < 4; j++) {
                                rgCostsPartCur[j] *= lphulCur->rghs[i].cItem;
                                rgCostsPartNew[j] *= lphulNew->rghs[i].cItem;
                                cost = 0 <= rgCostsPartNew[j] - rgCostsPartCur[j] ? rgCostsPartNew[j] - rgCostsPartCur[j] : 0;
                                rgCosts[j] -= rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j];
                            }
                        }
                    }
                }
            }
        }
        if (fStarbase != 0 && (GetRaceGrbit(&rgplr[iplr], ibitRaceISB) != 0 || GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh)) {
            for (i = 0; i < 4; i++) {
                rgCosts[i] -= (uint32_t)rgCosts[i] / 5;
            }
        }
        if (fStarbase != 0) {
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)((uint32_t)(rgCosts[i] + 1) / 2);
            }
        } else {
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
        }
    } else {
        switch (iItem) {
        case iobjFactory:
        case mdIdleFactory:
            cost = GetRaceGrbit(&rgplr[iplr], ibitRaceCheapFact);
            if (gd.fTutorial != 0) {
                for (i = 0; i < 3; i++) {
                    rgCost[i] = (int16_t)(2 - cost);
                }
            } else {
                rgCost[1] = 0;
                *rgCost = 0;
                rgCost[2] = (int16_t)(4 - cost);
            }
            rgCost[3] = GetRaceStat(&rgplr[iplr], rsFactBuild);
            break;
        case iobjMine:
        case mdIdleMine:
            for (i = 0; i < 3; i++) {
                rgCost[i] = 0;
            }
            rgCost[3] = GetRaceStat(&rgplr[iplr], rsMineBuild);
            break;
        case iobjDefense:
        case mdIdleDefense:
            part.hs.grhst = hstPlanetary;
            part.hs.iItem = 9;
            FLookupPart(&part);
            for (i = 0; i < 3; i++) {
                rgCost[i] = part.pplanetary->rgwtOreCost[i];
            }
            rgCost[3] = (uint32_t)part.pplanetary->resCost;
            if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raDefend)
                break;
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)((uint32_t)(rgCost[i] * 3) / 5);
            }
            break;
        case iobjAlchemy:
        case mdIdleAlchemy:
            for (i = 0; i < 3; i++) {
                rgCost[i] = 0;
            }
            rgCost[3] = (uint32_t)(GetRaceGrbit(&rgplr[iplr], ibitRaceMineralAlchemy) == 0 ? 100 : 25);
            break;
        case iobjGenesis:
            part.hs.grhst = hstPlanetary;
            part.hs.iItem = 14;
            FLookupPart(&part);
            GetTruePartCost(iplr, &part, rgCosts);
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
            break;
        case iobjPacketIron:
        case iobjPacketBor:
        case iobjPacketGerm:
            j = raMajor == 6 ? 70 : raMajor == 7 ? 120 : 110;
            for (i = 0; i < 3; i++) {
                t_merge_4b68_0001 = iItem - 14 == (uint32_t)i ? j : 0;
                rgCost[i] = t_merge_4b68_0001;
            }
            rgCost[i] = (uint32_t)(raMajor == 6 ? 5 : 10);
            break;
        case iobjPacket:
        case iobjPacketMixed:
            j = raMajor == 6 ? 25 : raMajor == 7 ? 48 : 44;
            for (i = 0; i < 3; i++) {
                rgCost[i] = j;
            }
            rgCost[i] = (uint32_t)(raMajor == 6 ? 5 : 10);
            break;
        case iobjMinTerraform:
        case iobjMaxTerraform:
        case mdIdleTerraform:
            rgCost[2] = 0;
            rgCost[1] = 0;
            *rgCost = 0;
            if (GetRaceGrbit(&rgplr[iplr], ibitRaceTT) != 0) {
                rgCost[3] = 70;
            } else {
                rgCost[3] = 100;
            }
            if (raMajor != 3)
                break;
            rgCost[3] = (uint32_t)(rgCost[3] / 2);
            break;
        case iobjPlanetaryScanner:
            iItem = iobjPlanetaryScannerFirst;
        case iobjPlanetaryScannerFirst:
        case iobjPlanetaryScannerViewer90:
        case iobjPlanetaryScannerScoper150:
        case iobjPlanetaryScannerScoper220:
        case iobjPlanetaryScannerScoper280:
        case iobjPlanetaryScannerSnooper320X:
        case iobjPlanetaryScannerSnooper400X:
        case iobjPlanetaryScannerSnooper500X:
        case iobjPlanetaryScannerSnooper620X:
            part.hs.grhst = hstPlanetary;
            part.hs.iItem = LOWORD(iItem) - 18;
            FLookupPart(&part);
            GetTruePartCost(iplr, &part, rgCosts);
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
        }
    }
    if (fOnlyOne == 0) {
        for (i = 0; i < 4; i++) {
            rgCost[i] = (uint32_t)(rgCost[i] * cItem);
        }
    }
    return;
}

void EstimateItemProdSched(PLANET *lppl, PLPROD *lpplprod, ProdItemType iItem, int16_t *piFirst, int16_t *piLast) {
    int32_t    cResearch;
    PLANET     pl;
    int32_t    rglQuan[3];
    int16_t    cBuilt;
    PROD       prodPartial;
    mdProdStat mdStatus;
    int16_t    i;
    int16_t    j;
    int16_t    iPass;
    int16_t    fAlchemy;
    int16_t    iMac;
    int32_t    rgRes[4];
    PROD      *lpprod;

    if (lpplprod == 0) {
        lpplprod = lppl->lpplprod;
    }
    pl = *lppl;
    pl.lpplprod = (PLPROD *)LpplAlloc(4, lpplprod->iprodMax, htOrd);
    fmemcpy(pl.lpplprod->rgprod, lpplprod->rgprod, lpplprod->iprodMac * 4);
    pl.lpplprod->iprodMac = lpplprod->iprodMac;
    iMac = lpplprod->iprodMac;
    prodPartial.cItem = 0;
    *piLast = 0;
    *piFirst = 0;
    for (iPass = 1; iPass < 100; iPass++) {
        EstMineralsMined(&pl, rglQuan, -1, 1);
        for (j = 0; j < 3; j++) {
            rgRes[j] = pl.rgwtMin[j];
        }
        rgRes[3] = CResourcesAtPlanet(&pl, lppl->iPlayer);
        if (pl.fNoResearch == 0) {
            cResearch = (int32_t)(rgRes[3] * (int16_t)rgplr[lppl->iPlayer].pctResearch) / 100;
            rgRes[3] -= cResearch;
        } else {
            cResearch = 0;
        }
        fAlchemy = 0;
        for (i = -1; i < iMac; i++) {
            if (i == -1) {
                lpprod = &prodPartial;
            } else {
                lpprod = &pl.lpplprod->rgprod[i];
            }
            if (lpprod->cItem != 0) {
                if (lpprod->iItem == iobjAlchemy && lpprod->grobj == grobjPlanet) {
                    if (i < iMac - 1) {
                        if (i == iItem) {
                            *piLast = -1;
                            *piFirst = -1;
                            goto LCleanUp;
                        }
                        fAlchemy = 1;
                        continue;
                    }
                    lpprod->cItem = 1020;
                }
                cBuilt = CBuildProdItem(&pl, lpprod, i == -1 ? NULL : &prodPartial, rgRes, fAlchemy, (int16_t *)&mdStatus, 0);
                if (iItem == i) {
                    if (cBuilt > 0 && *piFirst == 0) {
                        *piFirst = iPass;
                    }
                    switch (mdStatus) {
                    case mdProdStatSkippedAuto:
                        if (*piFirst == 0)
                            goto LCleanUp;
                        *piLast = iPass - 1;
                        goto LCleanUp;
                    case mdProdStatComplete:
                    case mdProdStatCompleteAuto:
                        *piLast = iPass;
                        goto LCleanUp;
                    default:
                        goto L_52a2;
                    }
                    goto LCleanUp;
                }
            L_52a2:
                fAlchemy = 0;
                if (lpprod->grobj == grobjPlanet) {
                    switch (lpprod->iItem) {
                    case iobjMine:
                    case mdIdleMine:
                        pl.cMines += cBuilt;
                        break;
                    case iobjFactory:
                    case mdIdleFactory:
                        pl.cFactories += cBuilt;
                    }
                }
                if (mdStatus >= mdProdStatSome)
                    break;
            }
        }
        if (iItem < iobjMine)
            goto L_53d7;
        for (j = 0; j < 3; j++) {
            pl.rgwtMin[j] = rgRes[j];
        }
        ChgPopFromPlanet(&pl, 1);
    }
    if (*piFirst == 0) {
        *piFirst = 100;
    }
    *piLast = 100;
    goto LCleanUp;
L_53d7:
    *piFirst = LOWORD(rgRes[3]);
    if (iItem == 0xffff) {
        *piFirst += LOWORD(cResearch);
    }
LCleanUp:
    if (pl.lpplprod != 0) {
        FreePl((PL *)pl.lpplprod);
    }
    return;
}

INT_PTR CALLBACK ZipProdDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    PAINTSTRUCT ps;
    int16_t     i;
    int16_t     iBase;
    RECT        rc;
    int16_t     dy;
    RECT        rc2;
    HWND        hwndRad;
    char       *psz;
    char       *pszT;
    RECT        rcGBox;
    int16_t     cch;
    FARPROC     lpProc;
    int16_t     cpq;
    char       *t_55d5;
    HWND        t_scratch_m32;
    char       *t_5a67;
    uint16_t    t_scratch_m3c_2;

    if (message == WM_PAINT) {
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rc);
        GetWindowRect(GetDlgItem(hwnd, 0x431), &rcGBox);
        ScreenToClient(hwnd, (POINT *)&rcGBox);
        GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0434), &rc);
        ScreenToClient(hwnd, (POINT *)&rc.right);
        rcGBox.right = rc.right;
        rcGBox.bottom = rc.bottom;
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        cch = CchGetString(idsCustomOrders, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 1), szWork, cch);
        rcGBox.top = rcGBox.bottom + 8;
        if (vrgZipProd[iResTechNow].fValid != 0) {
            cch = CchGetString(vrgZipProd[iResTechNow].fNoResearch + 1222, szWork);
            TextOut(hdc, rcGBox.left, vyZPDStatic, szWork, cch);
        }
        EndPaint(hwnd, &ps);
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    }
    if (IS_WM_CTLCOLOR(message) == 0) {
        if (message == WM_INITDIALOG) {
            SetWindowText(hwnd, PszGetCompressedString(idsCustomizeProductionTemplates));
            GetWindowRect(hwnd, &rc);
            GetClientRect(hwnd, &rc2);
            dy = rc.bottom - rc.top - rc2.bottom;
            GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0417), &rc2);
            MapWindowPoints(NULL, hwnd, (POINT *)&rc2, 2);
            vyZPDStatic = rc2.bottom + 2;
            dy += rc2.bottom + dyArial8 + 6;
            SetWindowPos(hwnd, NULL, 0, 0, rc.right - rc.left, dy, SWP_NOMOVE | SWP_NOZORDER);
            CheckRadioButton(hwnd, 1073, 1076, 1073);
            EnableZipProdBtns(hwnd, 0);
            iResTechNow = 0;
            FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
            for (i = 1073; i <= 1076; i++) {
                iBase = i - 1073;
                if (vrgZipProd[iBase].fValid != 0) {
                    pszT = szWork;
                    psz = vrgZipProd[iBase].szName;
                    while ((int16_t)(int8_t)*psz != 0) {
                        t_55d5 = psz;
                        psz++;
                        *pszT++ = *t_55d5;
                        if ((int16_t)(int8_t)*t_55d5 == 38) {
                            *pszT++ = '&';
                        }
                    }
                    *pszT = 0;
                    psz = szWork;
                } else {
                    psz = PszGetCompressedString(idsUnusedD);
                    _wsprintf(szWork, psz, iBase + 1);
                    psz = szWork;
                }
                hwndRad = GetDlgItem(hwnd, i);
                SetWindowText(hwndRad, psz);
            }
            StickyDlgPos(hwnd, &ptStickyZipProdDlg, 1);
            if (gd.fTutorial != 0) {
                AdvanceTutor();
            }
            return 1;
        }
        if (message == WM_COMMAND) {
            if (GET_WM_COMMAND_CMD(wParam, lParam) == 0 && GET_WM_COMMAND_ID(wParam, lParam) >= 0x431 && GET_WM_COMMAND_ID(wParam, lParam) <= IDC_U16_0x0434) {
                iResTechNow = GET_WM_COMMAND_ID(wParam, lParam) - 1073;
                EnableZipProdBtns(hwnd, iResTechNow);
                FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
            } else {
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                case IDCANCEL:
                    StickyDlgPos(hwnd, &ptStickyZipProdDlg, 0);
                    EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                    vyZPDStatic = -1;
                    if (gd.fTutorial != 0) {
                        AdvanceTutor();
                    }
                    return 1;
                case IDC_IMPORT:
                case IDC_RENAME:
                    if (vrgZipProd[iResTechNow].fValid != 0) {
                        strcpy(szWork, vrgZipProd[iResTechNow].szName);
                    } else {
                        _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                    }
                    lpProc = MakeProcInstance(RenameZipDlg, hInst);
                    if (iResTechNow != 0) {
                        if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc) == 0)
                            goto L_5ce2;
                        if ((int16_t)(int8_t)szWork[0] == 0) {
                            _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                        }
                        strcpy(vrgZipProd[iResTechNow].szName, szWork);
                        pszT = &szWork[64];
                        psz = szWork;
                        while ((int16_t)(int8_t)*psz != 0) {
                            t_5a67 = psz;
                            psz++;
                            *pszT++ = *t_5a67;
                            if ((int16_t)(int8_t)*t_5a67 == 38) {
                                *pszT++ = '&';
                            }
                        }
                        *pszT = 0;
                        SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), &szWork[64]);
                    }
                    if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_IMPORT) {
                        vrgZipProd[iResTechNow].fValid = 1;
                        cpq = 0;
                        for (i = 0; i < lpplProdGlob->iprodMac; i++) {
                            if (lpplProdGlob->rgprod[i].grobj == grobjPlanet && lpplProdGlob->rgprod[i].iItem < mdIdleFactory) {
                                vrgZipProd[iResTechNow].rgpq[cpq].mdIdle = lpplProdGlob->rgprod[i].iItem;
                                vrgZipProd[iResTechNow].rgpq[cpq].cQuan = lpplProdGlob->rgprod[i].cItem;
                                cpq++;
                                if (cpq >= 12)
                                    break;
                            }
                        }
                        vrgZipProd[iResTechNow].cpq = LOBYTE(cpq);
                        t_scratch_m3c_2 = sel.pl.fNoResearch;
                        vrgZipProd[iResTechNow].fNoResearch = LOBYTE(t_scratch_m3c_2);
                        FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
                    }
                    EnableZipProdBtns(hwnd, iResTechNow);
                L_5ce2:
                    FreeProcInstance(lpProc);
                    SetFocus(hwnd);
                    gd.fChgZipProd = 1;
                    break;
                case IDC_DELETE:
                    vrgZipProd[iResTechNow].fValid = 0;
                    _wsprintf(szWork, PszGetCompressedString(idsUnusedD), iResTechNow + 1);
                    SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), szWork);
                    FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
                    gd.fChgZipProd = 1;
                    break;
                case IDC_HELP:
                    WinHelp(hwnd, szHelpFile, 1, 1106);
                    return 1;
                }
            }
        }
    } else {
        for (i = 1073; i <= 1076; i++) {
            t_scratch_m32 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m32 == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 1076) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    }
    return 0;
}

void EnableZipProdBtns(HWND hwnd, int16_t iSel) {
    int16_t fEnabled;

    fEnabled = vrgZipProd[iSel].fValid != 0 && iSel > 0;
    EnableWindow(GetDlgItem(hwnd, IDC_DELETE), fEnabled);
    EnableWindow(GetDlgItem(hwnd, IDC_RENAME), fEnabled);
    return;
}

void FillZipProdLB(HWND hwndDlg, ZIPPRODQ *pzpq) {
    int16_t i;
    HWND    hwndLB;
    char    szAuto[40];
    char    szFormat[15];
    RECT    rc;

    hwndLB = GetDlgItem(hwndDlg, IDC_U16_0x0417);
    GetClientRect(hwndDlg, &rc);
    rc.top = vyZPDStatic;
    rc.bottom = vyZPDStatic + dyArial8;
    InvalidateRect(hwndDlg, &rc, 1);
    SendMessage(hwndLB, LB_RESETCONTENT, 0, 0);
    if (pzpq->fValid == 0 || pzpq->cpq == 0) {
        SendMessage(hwndLB, LB_ADDSTRING, 0, (LPARAM)PszGetCompressedString(idsAutoBuildOrders));
    } else {
        CchGetString(idsSD2, szFormat);
        for (i = 0; i < pzpq->cpq; i++) {
            CchGetString(pzpq->rgpq[i].mdIdle + 126, szAuto);
            if (pzpq->rgpq[i].cQuan == 1 || pzpq->rgpq[i].mdIdle == 3) {
                strcpy(szWork, szAuto);
            } else {
                _wsprintf(szWork, szFormat, szAuto, pzpq->rgpq[i].cQuan);
            }
            SendMessage(hwndLB, LB_ADDSTRING, 0, (LPARAM)szWork);
        }
    }
    return;
}
