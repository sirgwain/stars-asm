#include "common.h"

int16_t ChangeProduction(int16_t fClear) {
    jmp_buf  env;
    jmp_buf *penvMemSav;
    FARPROC  lpProcProd;
    PROD     rgprod[64];
    int16_t  fSuccess;

    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0) {
        if (fClear == 0) {
            InitProduction(rgprod);
            fDlgUp = 1;
            lpProcProd = MakeProcInstance(ProductionDlg, hInst);
            fSuccess = DialogBox(hInst, MAKEINTRESOURCE(IDD_PRODUCTION), hwndFrame, lpProcProd);
            FreeProcInstance(lpProcProd);
            hwndProdDlg = 0x0;
            fDlgUp = 0;
        } else {
            fSuccess = 1;
        }
        FinishProduction(fSuccess);
        if (fSuccess != 0 && sel.grobj == grobjPlanet) {
            DrawPlanShip(0x0, 8);
        }
        penvMem = penvMemSav;
        return 1;
    }
    if (lpplProdGlob != 0x0) {
        FreePl((PL *)lpplProdGlob);
    }
    lpplProdGlob = 0x0;
    if (hwndProdDlg != 0x0) {
        EndDialog(hwndProdDlg, 0);
    }
    hwndProdDlg = 0x0;
    fDlgUp = 0;
    AlertSz(PszFormatIds(idsThereIsntEnoughFreeMemoryModifyProduction, 0x0), MB_ICONHAND);
    penvMem = penvMemSav;
    return 0;
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
    if (rgprod == 0x0) {
        rgprod = pProdGlob;
    }
    if (sel.pl.lpplprod != 0x0) {
        i = sel.pl.lpplprod->iprodMac;
    } else {
        i = 2;
    }
    lpplProdGlob = (PLPROD *)LpplAlloc(0x4, i, htOrd);
    if (sel.pl.lpplprod != 0x0) {
        fmemcpy(lpplProdGlob->rgprod, sel.pl.lpplprod->rgprod, i * 4);
    } else {
        i = 0;
    }
    lpplProdGlob->iprodMac = LOBYTE(i);
    cProdGlob = 0;
    pProdGlob = rgprod;
    memset(rgprod, 0, 64 * sizeof(PROD));
    if (sel.pl.fStarbase != 0x0 && LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax != 0x0) {
        for (i = 0; i < 16; i++) {
            if (rgshdef[i].fFree == 0x0 && rgshdef[i].fGift == 0x0) {
                t_scratch_m1a_2 = rgshdef[i].hul.wtEmpty;
                if (LphuldefFromId(rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef)->hul.wtCargoMax >= t_scratch_m1a_2) {
                    rgprod[cProdGlob].cItem = 0x3ff;
                    rgprod[cProdGlob].iItem = i;
                    rgprod[cProdGlob].grobj = grobjFleet;
                    cProdGlob = cProdGlob + 1;
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        if (rglpshdefSB[idPlayer][i].fFree == 0x0 && rglpshdefSB[idPlayer][i].fGift == 0x0 && (sel.pl.isb != i || sel.pl.fStarbase == 0x0)) {
            rgprod[cProdGlob].cItem = 0x1;
            rgprod[cProdGlob].iItem = LOWORD((int32_t)(i + 16));
            rgprod[cProdGlob].grobj = grobjFleet;
            cProdGlob = cProdGlob + 1;
        }
    }
    part.hs.grhst = hstPlanetary;
    part.hs.iItem = 0xe;
    if (FLookupPart(&part) == 1) {
        rgprod[cProdGlob].cItem = 0x1;
        rgprod[cProdGlob].iItem = iobjGenesis;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    iWarp = IWarpMAFromLppl(&sel.pl, 0x0);
    if (iWarp > 0) {
        for (i = 0; i < 4; i++) {
            rgprod[cProdGlob].cItem = 0x3ff;
            rgprod[cProdGlob].iItem = LOWORD((int32_t)(i + 14));
            rgprod[cProdGlob].grobj = grobjPlanet;
            cProdGlob = cProdGlob + 1;
        }
    }
    t_scratch_m1a_3 = sel.pl.cFactories;
    u = CMaxFactories(&sel.pl, idPlayer) - t_scratch_m1a_3;
    if (u > 0x0) {
        rgprod[cProdGlob].cItem = LOWORD(0x3fc >= u ? (uint32_t)u : 0x3fc);
        rgprod[cProdGlob].iItem = mdIdleFactory;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    t_scratch_m1a_4 = sel.pl.cMines;
    u = CMaxMines(&sel.pl, idPlayer) - t_scratch_m1a_4;
    if (u > 0x0) {
        rgprod[cProdGlob].cItem = LOWORD(0x3fc >= u ? (uint32_t)u : 0x3fc);
        rgprod[cProdGlob].iItem = mdIdleMine;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    t_scratch_m1a_5 = sel.pl.cDefenses;
    u = CMaxDefenses(&sel.pl, idPlayer) - t_scratch_m1a_5;
    if (u > 0x0) {
        rgprod[cProdGlob].cItem = u;
        rgprod[cProdGlob].iItem = mdIdleDefense;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    rgprod[cProdGlob].cItem = 0x3ff;
    rgprod[cProdGlob].iItem = mdIdleAlchemy;
    rgprod[cProdGlob].grobj = grobjPlanet;
    cProdGlob = cProdGlob + 1;
    if (sel.pl.iScanner == 0x1f && GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh) {
        rgprod[cProdGlob].cItem = 0x1;
        rgprod[cProdGlob].iItem = iobjPlanetaryScanner;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    i = IpctCanTerraformLppl(&sel.pl);
    if (i > 0) {
        rgprod[cProdGlob].cItem = LOWORD((uint32_t)i);
        rgprod[cProdGlob].iItem = mdIdleTerraform;
        rgprod[cProdGlob].grobj = grobjPlanet;
        cProdGlob = cProdGlob + 1;
    }
    for (i = 0; i < 7; i++) {
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) == raMacintosh) {
            switch (i) {
            default:
                goto L_0c45;
            case 0:
            case 1:
            case 2:
            }
            continue;
        }
    L_0c45:
        if (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raTerra || (i != 4 && i != 5)) {
            rgprod[cProdGlob].cItem = 0x3ff;
            rgprod[cProdGlob].iItem = LOWORD((int32_t)i);
            rgprod[cProdGlob].grobj = grobjPlanet;
            cProdGlob = cProdGlob + 1;
        }
    }
    ipl = 0;
    lpprod = lpplProdGlob->rgprod;
    while (ipl < lpplProdGlob->iprodMac) {
        for (iSrc = 0; iSrc < cProdGlob && ((uint32_t)pProdGlob[iSrc].grobj != lpprod->grobj || (uint32_t)pProdGlob[iSrc].iItem != lpprod->iItem); iSrc++) {
        }
        if (iSrc < cProdGlob) {
            if (pProdGlob[iSrc].cItem < lpprod->cItem) {
                lpprod->cItem = LOWORD((uint32_t)pProdGlob[iSrc].cItem);
            }
            if (pProdGlob[iSrc].cItem != 0x3ff) {
                if (pProdGlob[iSrc].cItem < lpprod->cItem) {
                    pProdGlob[iSrc].cItem = 0x0;
                } else {
                    pProdGlob[iSrc].cItem = pProdGlob[iSrc].cItem - lpprod->cItem;
                }
            }
        } else {
            if (ipl + 1 < lpplProdGlob->iprodMac) {
                fmemcpy(lpprod, lpprod + 1, (lpplProdGlob->iprodMac - (ipl + 1)) * sizeof(PROD));
                ipl = ipl - 1;
            }
            lpplProdGlob->iprodMac = lpplProdGlob->iprodMac - 0x1;
        }
        ipl = ipl + 1;
        lpprod = lpprod + 1;
    }
    if (gd.fTutorial != 0x0 && idPlayer == 0) {
        AdvanceTutor();
    }
    return;
}

void FinishProduction(int16_t fWrite) {
    if (fWrite == 0) {
        sel.pl.fNoResearch = gd.fNoResearchSav;
        FreePl((PL *)lpplProdGlob);
    } else {
        FreePl((PL *)sel.pl.lpplprod);
        if (lpplProdGlob != 0x0 && lpplProdGlob->iprodMac == 0x0) {
            FreePl((PL *)lpplProdGlob);
            lpplProdGlob = 0x0;
        }
        sel.pl.lpplprod = lpplProdGlob;
        lpplProdGlob = 0x0;
        FLookupPlanet(-1, &sel.pl);
        FLookupPlanet(sel.pl.id, &sel.pl);
        if (fAi == 0) {
            FillPlanetProdLB(0x0, 0x0, 0x0);
            DrawPlanShip(0x0, 64);
        }
    }
    lpplProdGlob = 0x0;
    if (gd.fTutorial != 0x0 && idPlayer == 0) {
        tutor.fProgress = 0x1;
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
    int16_t            t_1722;
    int16_t            t_173f;
    int16_t            t_175e;
    int16_t            t_184a;
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
    if (IS_WM_CTLCOLOR(message) != 0) {
        t_scratch_m2e = GET_WM_CTLCOLOR_HWND(wParam, lParam);
        if (t_scratch_m2e == GetDlgItem(hwnd, IDC_U16_0x008B) || HIWORD(lParam) == 0x6) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
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
            if (gd.mdScreenSize < 0x1) {
                dx = 610;
                dy = 24 * dyArial8 + 24;
            } else {
                dx = 760;
                dy = 580;
            }
            SetWindowPos(hwnd, 0x0, 0, 0, dx, dy, SWP_NOMOVE | SWP_NOZORDER);
            GetClientRect(hwnd, &rc);
            xCtr = rc.right >> 0x1;
            dyLB = rc.bottom - (int32_t)(17 * dyArial8) / 2 - 24;
            rc.left = (int32_t)(11 * rc.right) / 20 + 16;
            dxPBtn = (int32_t)(rc.right - rc.left) / 4;
            rc.bottom = rc.bottom - ((int32_t)(3 * dyArial8) / 2 + 6);
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x008B), 0x0, 6, rc.bottom, rc.left - 12, (int32_t)(3 * dyArial8) / 2, SWP_NOZORDER);
            for (i = 0; i < 4; i++) {
                SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), 0x0, rc.left, rc.bottom, dxPBtn - 6, (int32_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                rc.left = rc.left + dxPBtn;
            }
            dxPBtn = dxPBtn + 24;
            rc.left = xCtr - ((dxPBtn - 0x6) >> 0x1);
            dy = (int32_t)(dyLB - 9 * dyArial8) / 5 + (int32_t)(3 * dyArial8) / 2 - 3;
            rc.top = 8;
            if (dy > 50) {
                rc.top = rc.top + (int32_t)((dy - 50) * 5) / 2;
                dy = 50;
            }
            for (i = 4; i < 10; i++) {
                SetWindowPos(GetDlgItem(hwnd, rgidProdBtns[i]), 0x0, rc.left, rc.top, dxPBtn - 6, (int32_t)(3 * dyArial8) / 2, SWP_NOZORDER);
                rc.top = rc.top + dy;
            }
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0416), 0x0, 6, 6, rc.left - 12, dyLB, SWP_NOZORDER);
            GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0416), &rcT);
            SetWindowPos(GetDlgItem(hwnd, IDC_U16_0x0417), 0x0, rc.left + dxPBtn, 6, rc.left - 12, rcT.bottom - rcT.top, SWP_NOZORDER);
            ScreenToClient(hwnd, (POINT *)&rcT.right);
            yTopFutureTech = rcT.bottom;
            InitializeProductionDlg(hwnd);
            if (gd.mdScreenSize == 0x1 && ptStickyProduceDlg.y == -1) {
                ptStickyProduceDlg.y = 0;
            }
            StickyDlgPos(hwnd, &ptStickyProduceDlg, 1);
            return 1;
        case WM_DRAWITEM:
            lpdis = (DRAWITEMSTRUCT *)lParam;
            if (lpdis->itemID != -1) {
                switch (lpdis->itemAction) {
                case 0x1:
                case 0x2:
                case 0x4:
                    DrawCBEntireItem(lpdis, 4);
                default:
                }
            } else {
                HandleFocusState(lpdis, -2);
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
            if (message != WM_LBUTTONDOWN) {
                sz255[0] = -1;
                sz255[1] = 0;
                cMax = 0;
                for (i = 0; i < 4; i++) {
                    if (vrgZipProd[i].fValid != 0x0) {
                        t_1722 = cMax;
                        cMax = cMax + 1;
                        rgszZip[t_1722] = vrgZipProd[i].szName;
                    }
                }
                t_173f = cMax;
                cMax = cMax + 1;
                rgszZip[t_173f] = sz255;
                t_175e = cMax;
                cMax = cMax + 1;
                rgszZip[t_175e] = PszGetCompressedString(idsCustomize);
                i = PopupMenu(hwnd, pt.x, pt.y, cMax, 0x0, rgszZip, -1, 1);
                if (i != cMax - 1) {
                    if (i < 0)
                        break;
                    for (cMax = 0; cMax < 4; cMax++) {
                        if (vrgZipProd[cMax].fValid != 0x0) {
                            t_184a = i;
                            i = i - 1;
                            if (t_184a == 0)
                                break;
                        }
                    }
                    ProdCommandHandler(hwnd, 0x816, (int32_t)cMax);
                    break;
                }
                memcpy(rgzp, vrgZipProd, 0xa0);
                lpProc = MakeProcInstance(ZipProdDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_ZIP_PROD), hwnd, lpProc);
                FreeProcInstance(lpProc);
                if (fRet != 0)
                    break;
                memcpy(vrgZipProd, rgzp, 0xa0);
                break;
            }
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsRightClickBlueDiamondApplyProductionTemplate, szPopupBuffer);
            Popup(hwnd, pt.x, pt.y);
            break;
        case WM_SETCURSOR:
            hcs = 0x0;
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
        default:
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
    int32_t  t_1a18;
    uint32_t t_merge_1abe_0001_wide;
    uint32_t t_merge_1b2c_0001_wide;
    uint32_t t_merge_1bae_0001_wide;
    uint32_t t_merge_1e76_0001_wide;
    uint16_t t_scratch_m34;
    int16_t  t_merge_2363_0001;
    uint16_t t_scratch_m30_5;
    uint16_t t_scratch_m32_5;

    switch (GET_WM_COMMAND_ID(wParam, lParam)) {
    case 0x418:
    AddItem:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_GETCURSEL, 0x0, 0);
        if (lSel < 0)
            break;
        for (iSrc = 0; iSrc < cProdGlob; iSrc++) {
            if (pProdGlob[iSrc].cItem != 0x0) {
                t_1a18 = lSel;
                lSel = lSel - 1;
                if (t_1a18 == 0)
                    break;
            }
        }
        prod = pProdGlob[iSrc];
        if ((GetAsyncKeyState(17) & 0xfffe) == 0x0) {
            if ((GetAsyncKeyState(16) & 0xfffe) == 0x0) {
                prod.cItem = 0x1;
            } else {
                t_merge_1bae_0001_wide = prod.cItem < 0xa ? (uint32_t)prod.cItem : 0xa;
                prod.cItem = LOWORD(t_merge_1bae_0001_wide);
            }
        } else if ((GetAsyncKeyState(16) & 0xfffe) == 0x0) {
            t_merge_1b2c_0001_wide = prod.cItem < 0x64 ? (uint32_t)prod.cItem : 0x64;
            prod.cItem = LOWORD(t_merge_1b2c_0001_wide);
        } else {
            t_merge_1abe_0001_wide = prod.cItem < 0x3fc ? (uint32_t)prod.cItem : 0x3fc;
            prod.cItem = LOWORD(t_merge_1abe_0001_wide);
        }
        if (pProdGlob[iSrc].cItem != 0x3ff) {
            pProdGlob[iSrc].cItem = pProdGlob[iSrc].cItem - prod.cItem;
        }
        iMac = lpplProdGlob->iprodMac;
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0x0, 0);
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
            iDst = iDst + 1;
            if (iDst >= iMac)
                goto L_2099;
            prodLast = lpplProdGlob->rgprod[iDst];
            if ((uint32_t)prodLast.iItem != prod.iItem || (uint32_t)prodLast.grobj != prod.grobj)
                goto L_2099;
        RingItUp:
            if (0x3fc < lpplProdGlob->rgprod[iDst].cItem + (uint32_t)prod.cItem) {
                t_merge_1e76_0001_wide = 0x3fc;
            } else {
                t_scratch_m34 = prod.cItem;
                t_merge_1e76_0001_wide = (uint32_t)lpplProdGlob->rgprod[iDst].cItem + (uint32_t)t_scratch_m34;
            }
            lpplProdGlob->rgprod[iDst].cItem = LOWORD(t_merge_1e76_0001_wide);
            if (lpplProdGlob->rgprod[iDst].cItem <= 0x1 || lpplProdGlob->rgprod[iDst].iItem != iobjAlchemy || lpplProdGlob->rgprod[iDst].grobj != grobjPlanet)
                goto FixedUp;
            lpplProdGlob->rgprod[iDst].cItem = 0x1;
            goto FixedUp;
        }
    L_2099:
        if (iMac >= 40) {
            MessageBeep(0x0);
            goto RedrawText;
        }
        if (iMac == lpplProdGlob->iprodMax) {
            lpplProdGlob = (PLPROD *)LpplReAlloc((PL *)lpplProdGlob, iMac + 4);
        }
        if (iDst != iMac) {
            fmemmove(lpplProdGlob + (1 + (iDst + 1)), &lpplProdGlob->rgprod[iDst], (iMac - iDst) * 4);
        }
        lpplProdGlob->rgprod[iDst] = prod;
        lpplProdGlob->iprodMac = lpplProdGlob->iprodMac + 0x1;
    FixedUp:
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, iDst + 1, 0);
        if (pProdGlob[iSrc].cItem != 0x0)
            goto RedrawText;
        FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
        goto RedrawText;
    case 0x419:
    RemoveItem:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0x0, 0);
        if (lSel <= 0)
            break;
        iMac = lpplProdGlob->iprodMac;
        lSel = lSel - 1;
        prod = lpplProdGlob->rgprod[lSel];
        for (iSrc = 0; iSrc < cProdGlob && ((uint32_t)pProdGlob[iSrc].grobj != prod.grobj || (uint32_t)pProdGlob[iSrc].iItem != prod.iItem); iSrc++) {
        }
        t_merge_2363_0001 = pProdGlob[iSrc].cItem == 0x0 ? 1 : 0;
        fRefillSrc = t_merge_2363_0001;
        if ((GetAsyncKeyState(17) & 0xfffe) == 0x0) {
            if ((GetAsyncKeyState(16) & 0xfffe) == 0x0) {
                c = 1;
            } else {
                c = 10;
            }
        } else if ((GetAsyncKeyState(16) & 0xfffe) == 0x0) {
            c = 100;
        } else {
            c = 1020;
        }
        if (prod.grobj == grobjPlanet && prod.iItem == iobjAlchemy) {
            c = 1020;
        }
        c = c >= prod.cItem ? prod.cItem : c;
        if (pProdGlob[iSrc].cItem != 0x3ff) {
            pProdGlob[iSrc].cItem = pProdGlob[iSrc].cItem + c;
        }
        lpplProdGlob->rgprod[lSel].cItem = lpplProdGlob->rgprod[lSel].cItem - c;
        if (lpplProdGlob->rgprod[lSel].cItem == 0x0) {
            if ((int32_t)(lSel + 1) < (int32_t)iMac) {
                fmemmove(&lpplProdGlob->rgprod[lSel], &lpplProdGlob->rgprod[lSel + 1], (iMac - LOWORD(lSel) - 0x1) * sizeof(PROD));
            } else {
                lSel = lSel - 1;
            }
            lpplProdGlob->iprodMac = lpplProdGlob->iprodMac - 0x1;
        }
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
        if (lSel >= 0) {
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 0x1, 0);
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
                pProdGlob[iSrc].cItem = pProdGlob[iSrc].cItem + lpprod->cItem;
            }
            ipl = ipl + 1;
            lpprod = lpprod + 1;
        }
        if (GET_WM_COMMAND_ID(wParam, lParam) != IDC_IMPORT) {
            lpplProdGlob->iprodMac = 0x0;
        } else {
            t_scratch_m30_5 = vrgZipProd[lParam].cpq;
            cMax = lpplProdGlob->iprodMac + t_scratch_m30_5;
            if (cMax < 1) {
                cMax = 1;
            }
            lpplprodT = (PLPROD *)LpplAlloc(0x4, cMax, htOrd);
            fmemset(lpplprodT->rgprod, 0, cMax * 4);
            iDst = 0;
            for (iSrc = 0; iSrc < lpplProdGlob->iprodMac; iSrc++) {
                if (lpplProdGlob->rgprod[iSrc].grobj != grobjPlanet || lpplProdGlob->rgprod[iSrc].iItem >= mdIdleFactory) {
                    lpplprodT->rgprod[iDst] = lpplProdGlob->rgprod[iSrc];
                    iDst = iDst + 1;
                }
            }
            for (iSrc = 0; iSrc < vrgZipProd[lParam].cpq; iSrc++) {
                if ((GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raMacintosh || vrgZipProd[lParam].rgpq[iSrc].mdIdle > 0x2) &&
                    (GetRaceStat(&rgplr[idPlayer], rsMajorAdv) != raTerra ||
                     (vrgZipProd[lParam].rgpq[iSrc].mdIdle != 0x4 && vrgZipProd[lParam].rgpq[iSrc].mdIdle != 0x5))) {
                    lpplprodT->rgprod[iDst].grobj = grobjPlanet;
                    lpplprodT->rgprod[iDst].iItem = vrgZipProd[lParam].rgpq[iSrc].mdIdle;
                    lpplprodT->rgprod[iDst].cItem = vrgZipProd[lParam].rgpq[iSrc].cQuan;
                    iDst = iDst + 1;
                }
            }
            lpplprodT->iprodMac = LOBYTE(iDst);
            FreePl((PL *)lpplProdGlob);
            lpplProdGlob = lpplprodT;
            t_scratch_m32_5 = vrgZipProd[lParam].fNoResearch;
            sel.pl.fNoResearch = t_scratch_m32_5;
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), BM_SETCHECK, sel.pl.fNoResearch, 0);
        }
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, 0x0, 0);
        FillProdSrcLB(GetDlgItem(hwnd, IDC_U16_0x0416), -1);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_SETCURSEL, 0x0, 0);
        goto RedrawText;
    case IDC_U16_0x0416:
    case IDC_U16_0x0417:
        if (GET_WM_COMMAND_CMD(wParam, lParam) != 0x1) {
            if (GET_WM_COMMAND_CMD(wParam, lParam) != 0x2)
                break;
            if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_U16_0x0416)
                goto AddItem;
            goto RemoveItem;
        }
        goto RedrawText;
    case IDC_U16_0x008B:
        hwndLB = GetDlgItem(hwnd, IDC_U16_0x0417);
        sel.pl.fNoResearch = LOWORD(SendMessage(GetDlgItem(hwnd, IDC_U16_0x008B), BM_GETCHECK, 0x0, 0));
        lSel = SendMessage(hwndLB, LB_GETCURSEL, 0x0, 0);
        FillPlanetProdLB(hwndLB, lpplProdGlob, 0x0);
        SendMessage(hwndLB, LB_SETCURSEL, LOWORD(lSel), 0);
        goto RedrawText;
    case IDOK:
    case IDCANCEL:
        hwndProdDlg = 0x0;
        StickyDlgPos(hwnd, &ptStickyProduceDlg, 0);
        EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
        break;
    case IDC_U16_0x042E:
    case IDC_NEXT:
        c = GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT ? 1 : -1;
        FinishProduction(1);
        if (GetKeyState(16) >= 0) {
            SelectAdjPlanet(c, 0);
        } else {
            SelectAdjPlanet(0, IdFindAdjStarbase(sel.pl.id, GET_WM_COMMAND_ID(wParam, lParam) == IDC_NEXT ? 1 : 0));
        }
        InitProduction(0x0);
        InitializeProductionDlg(hwnd);
        GetClientRect(hwnd, &rc);
        rc.top = yTopFutureTech;
        rc.bottom = 9 * dyArial8 + rc.top;
        InvalidateRect(hwnd, &rc, 1);
        break;
    case 0x43a:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0x0, 0);
        iMac = lpplProdGlob->iprodMac;
        if (lSel > 0 && lSel < (int32_t)iMac) {
            lSel = lSel - 1;
            prod = lpplProdGlob->rgprod[lSel];
            lpplProdGlob->rgprod[lSel] = lpplProdGlob->rgprod[lSel + 1];
            lpplProdGlob->rgprod[lSel + 1] = prod;
            FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
            SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 0x2, 0);
            goto RedrawText;
        }
        break;
    case 0x439:
        lSel = SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_GETCURSEL, 0x0, 0);
        if (lSel <= 1)
            break;
        iMac = lpplProdGlob->iprodMac;
        lSel = lSel - 2;
        prod = lpplProdGlob->rgprod[lSel];
        lpplProdGlob->rgprod[lSel] = lpplProdGlob->rgprod[lSel + 1];
        lpplProdGlob->rgprod[lSel + 1] = prod;
        FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
        SendMessage(GetDlgItem(hwnd, IDC_U16_0x0417), LB_SETCURSEL, LOWORD(lSel) + 0x1, 0);
        goto RedrawText;
    case IDC_HELP:
        WinHelp(hwnd, szHelpFile, 0x1, 0x423);
    default:
    }
    return;
RedrawText:
    GetClientRect(hwnd, &rc);
    rc.top = yTopFutureTech;
    rc.bottom = 7 * dyArial8 + rc.top;
    rc.left = rc.left + 130;
    InvalidateRect(hwnd, &rc, 1);
    DrawProductionDlg(hwnd, 0x0, &rc, -1);
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
    SendMessage(GetDlgItem(hwnd, IDC_U16_0x0416), LB_SETCURSEL, 0x0, 0);
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    while (i < lpplProdGlob->iprodMac) {
        if (lpprod->grobj != grobjPlanet || lpprod->iItem >= mdIdleFactory) {
            iSel = i;
        }
        i = i + 1;
        lpprod = lpprod + 1;
    }
    FillPlanetProdLB(GetDlgItem(hwnd, IDC_U16_0x0417), lpplProdGlob, 0x0);
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
    int32_t t_3735;

    fCreatedDC = 0;
    if (hdc == 0x0) {
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
        lSel = SendMessage(GetDlgItem(hwnd, idc), 0x409, 0x0, 0);
        if (lSel >= 0 && (lSel != 0 || i != 1)) {
            if (i != 0) {
                lSel = lSel - 1;
                prod = lpplProdGlob->rgprod[lSel];
            } else {
                for (iSrc = 0; iSrc < cProdGlob; iSrc++) {
                    if (pProdGlob[iSrc].cItem != 0x0) {
                        t_3735 = lSel;
                        lSel = lSel - 1;
                        if (t_3735 == 0)
                            break;
                    }
                }
                prod = pProdGlob[iSrc];
                prod.cItem = 0x1;
            }
            GetProductionCosts(&sel.pl, &prod, rgCost, idPlayer, 0);
            rc.bottom = yTopFutureTech + 4;
            SelectObject(hdc, rghfontArial8[1]);
            c = CchGetString(idsRequiredMinerals, szWork);
            TextOut(hdc, rc.left, rc.bottom, szWork, c);
            rc.left = rc.left + 20;
            rc.right = rc.right - 20;
            for (k = 0; k <= 3; k++) {
                c = k == 3 ? 5 : k;
                rc.bottom = rc.bottom + dyArial8;
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
                rc.bottom = rc.bottom + (int32_t)(3 * dyArial8) / 2;
                SelectObject(hdc, rghfontArial8[1]);
                c = _wsprintf(szT, PszGetCompressedString(idsDDoneCompletion), prod.pct);
                if (PszProductionETA(&sel.pl, lpplProdGlob, LOWORD(lSel), 0x0, 0x0) != szWork) {
                }
                strcpy(&szT[c], szWork);
                TextOut(hdc, rc.left - 20, rc.bottom, szT, strlen(szT));
                SelectObject(hdc, rghfontArial8[0]);
            }
        }
    }
    GetClientRect(hwnd, &rc);
    rc.top = rc.bottom - ((int32_t)(5 * dyArial8) / 2 + 12);
    rc.bottom = (dyArial8 | 0x1) + rc.top;
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
    char     szT[80];
    int16_t  i;
    char    *psz;
    uint16_t t_merge_3c0f_0001;

    for (i = 0; i < 6; i++) {
        szT[i] = ' ';
    }
    SendMessage(hwndLB, LB_RESETCONTENT, 0x0, 0);
    for (i = 0; i < cProdGlob; i++) {
        if (pProdGlob[i].cItem > 0x0) {
            psz = PszNameProdItem(pProdGlob + i);
            strcpy(&szT[6], psz);
            if (pProdGlob[i].grobj != grobjFleet) {
                if (pProdGlob[i].iItem < mdIdleFactory) {
                    szT[0] = 'I';
                    strcat(&szT[6], " (Auto Build)");
                } else {
                    szT[0] = ' ';
                }
            } else {
                t_merge_3c0f_0001 = pProdGlob[i].iItem < iobjPacketGerm ? 0x2a : 0x23;
                szT[0] = LOBYTE(t_merge_3c0f_0001);
            }
            SendMessage(hwndLB, LB_ADDSTRING, 0x0, (LPARAM)szT);
        }
    }
    return;
}

char *PszNameProdItem(PROD *lpprod) {
    uint32_t iItem;
    int16_t  iDelta;

    iItem = lpprod->iItem;
    if (lpprod->grobj == grobjFleet) {
        if (iItem < 0x10) {
            if (rgshdef[iItem].fFree == 0x0) {
                strcpy(szWork, rgshdef[iItem].hul.szClass);
                return szWork;
            }
        } else {
            iItem = iItem - 0x10;
            if (rglpshdefSB[idPlayer][iItem].fFree == 0x0) {
                fstrcpy(szWork, rglpshdefSB[idPlayer][iItem].hul.szClass);
                if (sel.pl.fStarbase == 0x0) {
                    return szWork;
                }
                iDelta = rglpshdefSB[idPlayer][sel.pl.isb].hul.ihuldef - rglpshdefSB[idPlayer][iItem].hul.ihuldef;
                if (iDelta <= 0) {
                    if (iDelta >= 0) {
                        return szWork;
                    }
                    strcat(szWork, " (upgrade)");
                    return szWork;
                }
                strcat(szWork, " (downgrade)");
                return szWork;
            }
        }
        szWork[0] = 0;
        return szWork;
    }
    if (iItem >= 0x12 && iItem <= 0x1a) {
        fstrcpy(szWork, LpplanetaryFromId(LOWORD(iItem) - 18)->szName);
    } else if (iItem != 0x1b) {
        CchGetString(LOWORD(iItem) + 0x7e, szWork);
    } else {
        CchGetString(idsPlanetaryScanner, szWork);
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
    int16_t  t_merge_4b32_0001;
    uint32_t t_merge_4b68_0001;
    int16_t  t_merge_4bcf_0001;

    raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
    fStarbase = 0;
    iItem = lpprod->iItem;
    cItem = lpprod->cItem;
    if (lpprod->grobj != grobjFleet) {
        switch (iItem) {
        case iobjFactory:
        case mdIdleFactory:
            cost = GetRaceGrbit(&rgplr[iplr], ibitRaceCheapFact);
            if (gd.fTutorial == 0x0) {
                rgCost[1] = 0x0;
                *rgCost = 0x0;
                rgCost[2] = (int32_t)(4 - cost);
            } else {
                for (i = 0; i < 3; i++) {
                    rgCost[i] = (int32_t)(2 - cost);
                }
            }
            rgCost[3] = (int32_t)GetRaceStat(&rgplr[iplr], rsFactBuild);
            break;
        case iobjMine:
        case mdIdleMine:
            for (i = 0; i < 3; i++) {
                rgCost[i] = 0x0;
            }
            rgCost[3] = (int32_t)GetRaceStat(&rgplr[iplr], rsMineBuild);
            break;
        case iobjDefense:
        case mdIdleDefense:
            part.hs.grhst = hstPlanetary;
            part.hs.iItem = 0x9;
            FLookupPart(&part);
            for (i = 0; i < 3; i++) {
                rgCost[i] = (int32_t)part.pplanetary->rgwtOreCost[i];
            }
            rgCost[3] = (uint32_t)part.pplanetary->resCost;
            if (GetRaceStat(&rgplr[iplr], rsMajorAdv) != raDefend)
                break;
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)((uint32_t)(rgCost[i] * 0x3) / 0x5);
            }
            break;
        case iobjAlchemy:
        case mdIdleAlchemy:
            for (i = 0; i < 3; i++) {
                rgCost[i] = 0x0;
            }
            rgCost[3] = (uint32_t)(GetRaceGrbit(&rgplr[iplr], ibitRaceMineralAlchemy) == 0 ? 0x64 : 0x19);
            break;
        case iobjGenesis:
            part.hs.grhst = hstPlanetary;
            part.hs.iItem = 0xe;
            FLookupPart(&part);
            GetTruePartCost(iplr, &part, rgCosts);
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
            break;
        case iobjPacketIron:
        case iobjPacketBor:
        case iobjPacketGerm:
            if (raMajor != 6) {
                if (raMajor != 7) {
                    t_merge_4b32_0001 = 110;
                } else {
                    t_merge_4b32_0001 = 120;
                }
            } else {
                t_merge_4b32_0001 = 70;
            }
            j = t_merge_4b32_0001;
            for (i = 0; i < 3; i++) {
                t_merge_4b68_0001 = iItem - 14 == (uint32_t)i ? (int32_t)j : 0x0;
                rgCost[i] = t_merge_4b68_0001;
            }
            rgCost[i] = (uint32_t)(raMajor == 6 ? 0x5 : 0xa);
            break;
        case iobjPacket:
        case iobjPacketMixed:
            if (raMajor != 6) {
                if (raMajor != 7) {
                    t_merge_4bcf_0001 = 44;
                } else {
                    t_merge_4bcf_0001 = 48;
                }
            } else {
                t_merge_4bcf_0001 = 25;
            }
            j = t_merge_4bcf_0001;
            for (i = 0; i < 3; i++) {
                rgCost[i] = (int32_t)j;
            }
            rgCost[i] = (uint32_t)(raMajor == 6 ? 0x5 : 0xa);
            break;
        case iobjMinTerraform:
        case iobjMaxTerraform:
        case mdIdleTerraform:
            rgCost[2] = 0x0;
            rgCost[1] = 0x0;
            *rgCost = 0x0;
            if (GetRaceGrbit(&rgplr[iplr], ibitRaceTT) == 0) {
                rgCost[3] = 0x64;
            } else {
                rgCost[3] = 0x46;
            }
            if (raMajor != 3)
                break;
            rgCost[3] = (uint32_t)(rgCost[3] / 0x2);
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
            part.hs.iItem = LOWORD(iItem) - 0x12;
            FLookupPart(&part);
            GetTruePartCost(iplr, &part, rgCosts);
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
        default:
        }
    } else {
        if (iItem < iobjPacketGerm) {
            lpshdef = rglpshdef[iplr];
        } else {
            lpshdef = rglpshdefSB[iplr];
            iItem = iItem - 16;
            fStarbase = 1;
        }
        if (lpshdef[iItem].fFree != 0x0) {
            for (i = 0; i < 4; i++) {
                rgCost[i] = 0x0;
            }
            return;
        }
        GetTrueHullCost(iplr, &lpshdef[iItem].hul, rgCosts);
        if (fStarbase != 0 && lppl->fStarbase != 0x0) {
            lphulCur = &rglpshdefSB[iplr][lppl->isb].hul;
            lphulNew = &lpshdef[iItem].hul;
            GetTrueHullCost(iplr, lphulCur, rgCostsCur);
            if (lphulCur->ihuldef == lphulNew->ihuldef) {
                lphulT = &LphuldefFromId(lphulCur->ihuldef)->hul;
                part.hs.grhst = hstNone;
                part.phul = lphulT;
                GetTruePartCost(iplr, &part, rgCostsPartCur);
                for (i = 0; i < 4; i++) {
                    rgCosts[i] = rgCosts[i] - rgCostsPartCur[i];
                }
                chs = lphulCur->chs;
                for (i = 0; i < chs; i++) {
                    if (lphulCur->rghs[i].cItem != 0x0 && lphulNew->rghs[i].cItem != 0x0) {
                        part.hs = lphulCur->rghs[i];
                        FLookupPart(&part);
                        GetTruePartCost(iplr, &part, rgCostsPartCur);
                        part.hs = lphulNew->rghs[i];
                        FLookupPart(&part);
                        GetTruePartCost(iplr, &part, rgCostsPartNew);
                        if (lphulCur->rghs[i].grhst == lphulNew->rghs[i].grhst) {
                            if (lphulCur->rghs[i].iItem == lphulNew->rghs[i].iItem) {
                                for (j = 0; j < 4; j++) {
                                    rgCostsPartCur[j] = rgCostsPartCur[j] * lphulCur->rghs[i].cItem;
                                    rgCostsPartNew[j] = rgCostsPartNew[j] * lphulNew->rghs[i].cItem;
                                    cost = 0 <= rgCostsPartNew[j] - rgCostsPartCur[j] ? rgCostsPartNew[j] - rgCostsPartCur[j] : 0;
                                    rgCosts[j] = rgCosts[j] - (rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j]);
                                }
                            } else {
                                for (j = 0; j < 4; j++) {
                                    rgCostsPartCur[j] = rgCostsPartCur[j] * lphulCur->rghs[i].cItem;
                                    rgCostsPartNew[j] = rgCostsPartNew[j] * lphulNew->rghs[i].cItem;
                                    cost = (int32_t)(rgCostsPartNew[j] * 2) / 10 <= rgCostsPartNew[j] - (int32_t)(rgCostsPartCur[j] * 8) / 10
                                               ? rgCostsPartNew[j] - (int32_t)(rgCostsPartCur[j] * 8) / 10
                                               : (int32_t)(rgCostsPartNew[j] * 2) / 10;
                                    rgCosts[j] = rgCosts[j] - (rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j]);
                                }
                            }
                        } else {
                            for (j = 0; j < 4; j++) {
                                rgCostsPartCur[j] = rgCostsPartCur[j] * lphulCur->rghs[i].cItem;
                                rgCostsPartNew[j] = rgCostsPartNew[j] * lphulNew->rghs[i].cItem;
                                cost = (int32_t)(3 * rgCostsPartNew[j]) / 0xa <= rgCostsPartNew[j] - (int32_t)(7 * rgCostsPartCur[j]) / 0xa
                                           ? rgCostsPartNew[j] - (int32_t)(7 * rgCostsPartCur[j]) / 10
                                           : (int32_t)(3 * rgCostsPartNew[j]) / 10;
                                rgCosts[j] = rgCosts[j] - (rgCosts[j] >= (uint16_t)(rgCostsPartNew[j] - cost) ? rgCostsPartNew[j] - cost : rgCosts[j]);
                            }
                        }
                    }
                }
            } else {
                for (i = 0; i < 4; i++) {
                    costHalf = (uint32_t)rgCosts[i] / 2;
                    costUpg = rgCosts[i] - (int32_t)rgCostsCur[i] / 2;
                    if (costHalf <= costUpg) {
                        rgCosts[i] = costUpg;
                    } else {
                        rgCosts[i] = costHalf;
                    }
                }
            }
        }
        if (fStarbase != 0 && (GetRaceGrbit(&rgplr[iplr], ibitRaceISB) != 0 || GetRaceStat(&rgplr[iplr], rsMajorAdv) == raMacintosh)) {
            for (i = 0; i < 4; i++) {
                rgCosts[i] = rgCosts[i] - (uint32_t)rgCosts[i] / 0x5;
            }
        }
        if (fStarbase == 0) {
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)rgCosts[i];
            }
        } else {
            for (i = 0; i < 4; i++) {
                rgCost[i] = (uint32_t)((uint32_t)(rgCosts[i] + 0x1) / 0x2);
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
    int32_t cResearch;
    PLANET  pl;
    int32_t rglQuan[3];
    int16_t cBuilt;
    PROD    prodPartial;
    int16_t mdStatus;
    int16_t i;
    int16_t j;
    int16_t iPass;
    int16_t fAlchemy;
    int16_t iMac;
    int32_t rgRes[4];
    PROD   *lpprod;

    if (lpplprod == 0x0) {
        lpplprod = lppl->lpplprod;
    }
    pl = *lppl;
    pl.lpplprod = (PLPROD *)LpplAlloc(0x4, lpplprod->iprodMax, htOrd);
    fmemcpy(pl.lpplprod->rgprod, lpplprod->rgprod, lpplprod->iprodMac * 0x4);
    pl.lpplprod->iprodMac = lpplprod->iprodMac;
    iMac = lpplprod->iprodMac;
    prodPartial.cItem = 0x0;
    *piLast = 0;
    *piFirst = 0;
    for (iPass = 1; iPass < 100; iPass++) {
        EstMineralsMined(&pl, rglQuan, -1, 1);
        for (j = 0; j < 3; j++) {
            rgRes[j] = pl.rgwtMin[j];
        }
        rgRes[3] = (int32_t)CResourcesAtPlanet(&pl, lppl->iPlayer);
        if (pl.fNoResearch != 0x0) {
            cResearch = 0;
        } else {
            cResearch = (int32_t)((int32_t)(rgRes[3] * (int32_t)(int16_t)rgplr[lppl->iPlayer].pctResearch) / 0x64);
            rgRes[3] = rgRes[3] - cResearch;
        }
        fAlchemy = 0;
        for (i = -1; i < iMac; i++) {
            if (i != -1) {
                lpprod = &pl.lpplprod->rgprod[i];
            } else {
                lpprod = &prodPartial;
            }
            if (lpprod->cItem != 0x0) {
                if (lpprod->iItem == iobjAlchemy && lpprod->grobj == grobjPlanet) {
                    if (i < iMac - 1) {
                        if (i != iItem) {
                            fAlchemy = 1;
                            continue;
                        }
                        goto L_51c8;
                    }
                    lpprod->cItem = 0x3fc;
                }
                cBuilt = CBuildProdItem(&pl, lpprod, i == -1 ? 0x0 : &prodPartial, rgRes, fAlchemy, &mdStatus, 0);
                if (iItem == i) {
                    if (cBuilt > 0 && *piFirst == 0) {
                        *piFirst = iPass;
                    }
                    switch (mdStatus) {
                    case 2:
                        goto L_526c;
                    case 0:
                    case 1:
                        goto L_5297;
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
                        pl.cMines = pl.cMines + cBuilt;
                        break;
                    case iobjFactory:
                    case mdIdleFactory:
                        pl.cFactories = pl.cFactories + cBuilt;
                    default:
                    }
                }
                if (mdStatus >= 5)
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
L_51c8:
    *piLast = -1;
    *piFirst = -1;
    goto LCleanUp;
L_5297:
    *piLast = iPass;
    goto LCleanUp;
L_526c:
    if (*piFirst == 0)
        goto LCleanUp;
    *piLast = iPass - 1;
    goto LCleanUp;
L_53d7:
    *piFirst = LOWORD(rgRes[3]);
    if (iItem == 0xffff) {
        *piFirst = *piFirst + LOWORD(cResearch);
    }
LCleanUp:
    if (pl.lpplprod != 0x0) {
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
    char       *t_55de;
    char       *t_55f0;
    HWND        t_scratch_m32;
    char       *t_5a67;
    char       *t_5a70;
    char       *t_5a82;
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
        ExpandRc(&rcGBox, dyArial8, dyArial8 >> 0x1);
        _Draw3dFrame(hdc, &rcGBox, -1);
        SelectObject(hdc, rghfontArial8[1]);
        SetBkColor(hdc, crButtonFace);
        cch = CchGetString(idsCustomOrders, szWork);
        TextOut(hdc, rcGBox.left + 8, rcGBox.top - (dyArial8 >> 0x1), szWork, cch);
        rcGBox.top = rcGBox.bottom + 8;
        if (vrgZipProd[iResTechNow].fValid != 0x0) {
            cch = CchGetString(vrgZipProd[iResTechNow].fNoResearch + 0x4c6, szWork);
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
    if (IS_WM_CTLCOLOR(message) != 0) {
        for (i = 1073; i <= 1076; i++) {
            t_scratch_m32 = GET_WM_CTLCOLOR_HWND(wParam, lParam);
            if (t_scratch_m32 == GetDlgItem(hwnd, i))
                break;
        }
        if (i <= 1076) {
            SetBkColor((HDC)wParam, crButtonFace);
            return (INT_PTR)hbrButtonFace;
        }
    } else {
        if (message == WM_INITDIALOG) {
            SetWindowText(hwnd, PszGetCompressedString(idsCustomizeProductionTemplates));
            GetWindowRect(hwnd, &rc);
            GetClientRect(hwnd, &rc2);
            dy = rc.bottom - rc.top - rc2.bottom;
            GetWindowRect(GetDlgItem(hwnd, IDC_U16_0x0417), &rc2);
            MapWindowPoints(0x0, hwnd, (POINT *)&rc2, 0x2);
            vyZPDStatic = rc2.bottom + 2;
            dy = dy + (rc2.bottom + dyArial8 + 6);
            SetWindowPos(hwnd, 0x0, 0, 0, rc.right - rc.left, dy, SWP_NOMOVE | SWP_NOZORDER);
            CheckRadioButton(hwnd, 1073, 1076, 1073);
            EnableZipProdBtns(hwnd, 0);
            iResTechNow = 0;
            FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
            for (i = 1073; i <= 1076; i++) {
                iBase = i - 1073;
                if (vrgZipProd[iBase].fValid == 0x0) {
                    psz = PszGetCompressedString(idsUnusedD);
                    _wsprintf(szWork, psz, iBase + 1);
                    psz = szWork;
                } else {
                    pszT = szWork;
                    psz = vrgZipProd[iBase].szName;
                    while ((int16_t)*psz != 0) {
                        t_55d5 = psz;
                        psz = psz + 1;
                        t_55de = pszT;
                        pszT = pszT + 1;
                        *t_55de = *t_55d5;
                        if ((int16_t)*t_55d5 == '&') {
                            t_55f0 = pszT;
                            pszT = pszT + 1;
                            *t_55f0 = '&';
                        }
                    }
                    *pszT = 0;
                    psz = szWork;
                }
                hwndRad = GetDlgItem(hwnd, i);
                SetWindowText(hwndRad, psz);
            }
            StickyDlgPos(hwnd, &ptStickyZipProdDlg, 1);
            if (gd.fTutorial != 0x0) {
                AdvanceTutor();
            }
            return 1;
        }
        if (message == WM_COMMAND) {
            if (GET_WM_COMMAND_CMD(wParam, lParam) != 0x0 || GET_WM_COMMAND_ID(wParam, lParam) < 0x431 || GET_WM_COMMAND_ID(wParam, lParam) > IDC_U16_0x0434) {
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                case IDCANCEL:
                    StickyDlgPos(hwnd, &ptStickyZipProdDlg, 0);
                    EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDOK ? 1 : 0);
                    vyZPDStatic = -1;
                    if (gd.fTutorial != 0x0) {
                        AdvanceTutor();
                    }
                    return 1;
                case IDC_IMPORT:
                case IDC_RENAME:
                    if (vrgZipProd[iResTechNow].fValid == 0x0) {
                        _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                    } else {
                        strcpy(szWork, vrgZipProd[iResTechNow].szName);
                    }
                    lpProc = MakeProcInstance(RenameZipDlg, hInst);
                    if (iResTechNow != 0) {
                        if (DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAME), hwndFrame, lpProc) == 0)
                            goto L_5ce2;
                        if ((int16_t)szWork[0] == 0) {
                            _wsprintf(szWork, PszGetCompressedString(idsCustomD), iResTechNow);
                        }
                        strcpy(vrgZipProd[iResTechNow].szName, szWork);
                        pszT = &szWork[64];
                        psz = szWork;
                        while ((int16_t)*psz != 0) {
                            t_5a67 = psz;
                            psz = psz + 1;
                            t_5a70 = pszT;
                            pszT = pszT + 1;
                            *t_5a70 = *t_5a67;
                            if ((int16_t)*t_5a67 == '&') {
                                t_5a82 = pszT;
                                pszT = pszT + 1;
                                *t_5a82 = '&';
                            }
                        }
                        *pszT = 0;
                        SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), &szWork[64]);
                    }
                    if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_IMPORT) {
                        vrgZipProd[iResTechNow].fValid = 0x1;
                        cpq = 0;
                        for (i = 0; i < lpplProdGlob->iprodMac; i++) {
                            if (lpplProdGlob->rgprod[i].grobj == grobjPlanet && lpplProdGlob->rgprod[i].iItem < mdIdleFactory) {
                                vrgZipProd[iResTechNow].rgpq[cpq].mdIdle = lpplProdGlob->rgprod[i].iItem;
                                vrgZipProd[iResTechNow].rgpq[cpq].cQuan = lpplProdGlob->rgprod[i].cItem;
                                cpq = cpq + 1;
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
                    gd.fChgZipProd = 0x1;
                    break;
                case IDC_DELETE:
                    vrgZipProd[iResTechNow].fValid = 0x0;
                    _wsprintf(szWork, PszGetCompressedString(idsUnusedD), iResTechNow + 1);
                    SetWindowText(GetDlgItem(hwnd, iResTechNow + 1073), szWork);
                    FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
                    gd.fChgZipProd = 0x1;
                    break;
                case IDC_HELP:
                    WinHelp(hwnd, szHelpFile, 0x1, 0x452);
                    return 1;
                default:
                }
            } else {
                iResTechNow = GET_WM_COMMAND_ID(wParam, lParam) - 1073;
                EnableZipProdBtns(hwnd, iResTechNow);
                FillZipProdLB(hwnd, &vrgZipProd[iResTechNow]);
            }
        }
    }
    return 0;
}

void EnableZipProdBtns(HWND hwnd, int16_t iSel) {
    int16_t fEnabled;
    int16_t t_merge_5e24_0001;

    if (vrgZipProd[iSel].fValid == 0x0 || iSel <= 0) {
        t_merge_5e24_0001 = 0;
    } else {
        t_merge_5e24_0001 = 1;
    }
    fEnabled = t_merge_5e24_0001;
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
    SendMessage(hwndLB, LB_RESETCONTENT, 0x0, 0);
    if (pzpq->fValid != 0x0 && pzpq->cpq != 0x0) {
        CchGetString(idsSD2, szFormat);
        for (i = 0; i < pzpq->cpq; i++) {
            CchGetString(pzpq->rgpq[i].mdIdle + 0x7e, szAuto);
            if (pzpq->rgpq[i].cQuan != 0x1 && pzpq->rgpq[i].mdIdle != 0x3) {
                _wsprintf(szWork, szFormat, szAuto, pzpq->rgpq[i].cQuan);
            } else {
                strcpy(szWork, szAuto);
            }
            SendMessage(hwndLB, LB_ADDSTRING, 0x0, (LPARAM)szWork);
        }
    } else {
        SendMessage(hwndLB, LB_ADDSTRING, 0x0, (LPARAM)PszGetCompressedString(idsAutoBuildOrders));
    }
    return;
}
