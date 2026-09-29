#include "common.h"

void BattleVCR(int16_t iBattle) {
    FARPROC  lpProc;
    jmp_buf *penvMemSav;
    jmp_buf  env;
    HB      *lphb;

    lpProc = 0x0;
    penvMemSav = penvMem;
    viStepVCRCur = -1;
    gd.fVCRTimer = 0x0;
    if (gd.mdScreenSize < 0x2) {
        dxyVCRBoard = 353;
        dxyVCRSquare = 32;
    } else {
        dxyVCRBoard = 673;
        dxyVCRSquare = 64;
    }
    lphb = rglphb[11];
    vlpbdVCR = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
    while (vlpbdVCR->id != iBattle) {
        if (vlpbdVCR->id != 0xffff) {
            vlpbdVCR = (BTLDATA *)((uint8_t *)vlpbdVCR + vlpbdVCR->cbData);
        } else {
            lphb = lphb->lphbNext;
            if (lphb == 0x0) {
                return;
            }
            vlpbdVCR = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        }
    }
    vlpbdVCRNext = (BTLDATA *)((uint8_t *)vlpbdVCR + vlpbdVCR->cbData);
    penvMem = &env;
    if (setjmp(env) == 0) {
        vrgtok = LpAlloc(vlpbdVCR->ctok * sizeof(TOK), htMisc);
        vrgdpVCR = LpAlloc(vlpbdVCR->ctok * 0x4, htMisc);
        vcStepVCR = SetVCRBoard(30000) - 1;
        vcRound = viRound;
        SetVCRBoard(-1);
        if (gd.fTutorial != 0x0) {
            AdvanceTutor();
        }
        lpProc = MakeProcInstance(VCRDlg, hInst);
        DialogBox(hInst, MAKEINTRESOURCE(IDD_VCR), hwndFrame, lpProc);
    } else {
        penvMem = penvMemSav;
        AlertSz(PszFormatIds(idsMemory, 0x0), MB_ICONHAND);
    }
    if (lpProc != 0x0) {
        FreeProcInstance(lpProc);
    }
    if (vrgtok != 0x0) {
        FreeLp(vrgtok, htMisc);
    }
    if (vrgdpVCR != 0x0) {
        FreeLp(vrgdpVCR, htMisc);
    }
    vrgtok = 0x0;
    vrgdpVCR = 0x0;
    return;
}

int16_t CBattles() {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  cBattles;

    cBattles = 0;
    lphb = rglphb[11];
    if (lphb != 0x0) {
        lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        while (1) {
            if (lpbd->id != 0xffff) {
                if (lpbd->cbData == 0x0) {
                    return cBattles;
                }
                lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
                cBattles = cBattles + 1;
            } else {
                lphb = lphb->lphbNext;
                if (lphb == 0x0 || lphb->ibTop <= sizeof(HB))
                    break;
                lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
            }
        }
        return cBattles;
    }
    return 0;
}

BTLDATA *BtlDataGet(int16_t i) {
    BTLDATA *lpbd;
    HB      *lphb;
    int16_t  t_0419;

    lphb = rglphb[11];
    if (lphb != 0x0) {
        lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        while (1) {
            if (lpbd->id != 0xffff) {
                if (lpbd->cbData == 0x0) {
                    return 0x0;
                }
                t_0419 = i;
                i = i - 1;
                if (t_0419 <= 0) {
                    return lpbd;
                }
                lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
            } else {
                lphb = lphb->lphbNext;
                if (lphb == 0x0 || lphb->ibTop <= sizeof(HB))
                    break;
                lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
            }
        }
        return 0x0;
    }
    return 0x0;
}

int32_t CBattleUnits(BTLDATA *lpbd, uint16_t grbitBU) {
    TOK    *lptok;
    int16_t ctok;
    int32_t lUnits;
    int16_t i;
    int16_t imd;

    ctok = lpbd->ctok;
    lUnits = 0;
    for (i = 0; i < ctok; i++) {
        lptok = &lpbd->rgtok[i];
        if (lptok->iplr != idPlayer) {
            if ((grbitBU & 0x2) == 0x0)
                continue;
        } else if ((grbitBU & 0x1) == 0x0) {
            continue;
        }
        if ((grbitBU & 0x4) != 0x0 || lptok->ishdef < 0x10) {
            if ((grbitBU & 0xf8) != 0xf8 && lptok->ishdef < 0x10) {
                imd = LphuldefFromId(rglpshdef[lptok->iplr][lptok->ishdef].hul.ihuldef)->imdCategory;
                if (imd > 1 && imd < 6) {
                    switch (imd) {
                    case 2:
                        if ((grbitBU & 0x10) == 0x0)
                            break;
                        goto L_0600;
                    case 3:
                        if ((grbitBU & 0x20) == 0x0)
                            break;
                        goto L_0600;
                    case 5:
                        if ((grbitBU & 0x40) == 0x0)
                            break;
                        goto L_0600;
                    case 4:
                        if ((grbitBU & 0x80) == 0x0)
                            break;
                    default:
                        goto L_0600;
                    }
                    continue;
                }
                if ((grbitBU & 0x8) == 0x0)
                    continue;
            }
        L_0600:
            lUnits = lUnits + (uint32_t)lptok->csh;
        }
    }
    return lUnits;
}

int32_t CBattleKills(BTLDATA *lpbd, int16_t fOurDead) {
    int32_t  cKilled;
    BTLDATA *lpbdNext;
    int16_t  i;
    BTLREC  *lpbr;
    int16_t  cKill;

    lpbr = (BTLREC *)&lpbd->rgtok[lpbd->ctok];
    lpbdNext = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
    cKilled = 0;
    for (; lpbr < (BTLREC *)lpbdNext; lpbr = (BTLREC *)&lpbr->rgkill[lpbr->ctok]) {
        cKill = lpbr->ctok;
        for (i = 0; i < cKill; i++) {
            if (lpbr->rgkill[i].cshKill > 0x0) {
                if (lpbd->rgtok[lpbr->rgkill[i].itok].iplr != idPlayer) {
                    if (fOurDead == 0) {
                        cKilled = cKilled + (uint32_t)lpbr->rgkill[i].cshKill;
                    }
                } else if (fOurDead != 0) {
                    cKilled = cKilled + (uint32_t)lpbr->rgkill[i].cshKill;
                }
            }
        }
    }
    return cKilled;
}

int32_t LdpFromItokDv(int16_t itok, DV *lpdv) {
    DV       dv;
    uint16_t dpShdef;
    int16_t  csh;
    int32_t  dp;

    if (lpdv != 0x0) {
        dv.dp = lpdv->dp;
    } else {
        dv.dp = 0x0;
    }
    dpShdef = LpshdefFromTok(vrgtok + itok)->hul.dp;
    dp = (uint32_t)((uint32_t)dpShdef * (uint32_t)vrgtok[itok].csh);
    if (dv.dp != 0x0) {
        csh = LOWORD((int32_t)((int32_t)((uint32_t)vrgtok[itok].csh * dv.pctSh) / 0x64));
        if (csh <= 0) {
            csh = 1;
        }
        dp = dp - (int32_t)((int32_t)((int32_t)((int32_t)((uint32_t)dpShdef * dv.pctDp) / 0xa) * (int32_t)csh) / 0x32);
    }
    return dp;
}

int16_t SetVCRBoard(int16_t iStep) {
    TOK    *ptok;
    int16_t i;
    int16_t itok;

    if (iStep != viStepVCRCur || viStepVCRCur == -1) {
        if (viStepVCRCur > iStep || viStepVCRCur == -1) {
            for (i = 0; i < vlpbdVCR->ctok; i++) {
                vrgtok[i] = vlpbdVCR->rgtok[i];
                vrgdpVCR[i] = LdpFromItokDv(i, &vrgtok[i].dv);
            }
            viStepVCRCur = -1;
            viRound = 0;
            viVCRFocus = 0;
            vbrcVCRFocus = vrgtok->brc;
            vlpbrVCR = (BTLREC *)&vlpbdVCR->rgtok[vlpbdVCR->ctok];
        }
        for (; viStepVCRCur < iStep && vlpbrVCR < (BTLREC *)vlpbdVCRNext; viStepVCRCur++) {
            if (viStepVCRCur >= 0) {
                for (i = 0; i < vlpbrVCR->ctok; i++) {
                    itok = vlpbrVCR->rgkill[i].itok;
                    ptok = vrgtok + itok;
                    ptok->csh = ptok->csh - vlpbrVCR->rgkill[i].cshKill;
                    ptok->dv.dp = vlpbrVCR->rgkill[i].dv.dp;
                    vrgdpVCR[itok] = LdpFromItokDv(itok, &vlpbrVCR->rgkill[i].dv);
                    if (ptok->dpShield != 0x0 && vlpbrVCR->rgkill[i].dpShield != 0x0) {
                        if ((int32_t)(uint32_t)((uint32_t)ptok->dpShield * (uint32_t)ptok->csh) <=
                            (int32_t)((uint32_t)(vlpbrVCR->rgkill[i].dpShield & 0x1fff) << (vlpbrVCR->rgkill[i].dpShield >> 0xd << 0x1))) {
                            ptok->dpShield = 0x0;
                        } else {
                            ptok->dpShield =
                                ptok->dpShield -
                                LOWORD((int32_t)((int32_t)((uint32_t)(vlpbrVCR->rgkill[i].dpShield & 0x1fff) << (vlpbrVCR->rgkill[i].dpShield >> 0xd << 0x1)) /
                                                 (int32_t)ptok->csh));
                        }
                    }
                }
                vlpbrVCR = (BTLREC *)&vlpbrVCR->rgkill[vlpbrVCR->ctok];
                if (vlpbrVCR->iRound > (uint16_t)viRound) {
                    viRound = vlpbrVCR->iRound;
                    ptok = vrgtok;
                    i = 0;
                    while (i < vlpbdVCR->ctok) {
                        if (ptok->fRegen != 0x0) {
                            RegenShield(ptok);
                        }
                        ptok->fMoved = 0x0;
                        i = i + 1;
                        ptok = ptok + 1;
                    }
                }
            }
            if (vlpbrVCR < (BTLREC *)vlpbdVCRNext) {
                vrgtok[vlpbrVCR->itok].brc = LOBYTE(((uint16_t)LOWORD(vlpbdVCRNext) & 0xff00) | ((uint16_t)vlpbrVCR->brcDest & 0xff));
                vbrcVCRFocus = vrgtok[vlpbrVCR->itok].brc;
                viVCRFocus = vlpbrVCR->itok;
                vrgtok[vlpbrVCR->itok].wFlags = (vrgtok[vlpbrVCR->itok].wFlags & 0xfc1f) | (vlpbrVCR->dzDis & 0x1f) * 0x20;
                if (vrgtok[vlpbrVCR->itok].dzDis == 0x4) {
                    vrgtok[vlpbrVCR->itok].mdTactic = 0x0;
                }
            }
        }
        EnableVCRButtons();
        return viStepVCRCur;
    }
    return iStep;
}

INT_PTR CALLBACK VCRDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC         hdc;
    int16_t     i;
    int16_t     ibtn;
    RECT        rc;
    int16_t     dyFrame;
    RECT        rcWindow;
    int16_t     dx;
    POINT16     pt;
    uint8_t     brc;
    int16_t     bkMode;
    int16_t     bt;
    RECT       *prc;
    int16_t     iDir;
    int16_t     iCur;
    COLORREF    crBkSav;
    BTNT        btnt;
    int16_t     iSel;
    PAINTSTRUCT ps;
    int16_t     iStep;
    int16_t     dStep;
    POINT       t_pt_107d;
    POINT       t_pt_108c_1;
    uint16_t    t_scratch_m18_3;
    uint16_t    t_scratch_m18_4;

    switch (message) {
    case WM_INITDIALOG:
        hwndVCRDlg = hwnd;
        GetWindowRect(hwnd, &rcWindow);
        GetClientRect(hwnd, &rc);
        dyFrame = rcWindow.bottom - rcWindow.top - rc.bottom;
        GetWindowRect(GetDlgItem(hwnd, IDC_VCR_REW_ALL), &rc);
        SetWindowPos(hwnd, 0x0, 0, 0, dxyVCRBoard + 250, dyFrame + 24 + dxyVCRBoard + (rc.bottom - rc.top), SWP_NOMOVE | SWP_NOZORDER);
        for (i = 0; i < 7; i++) {
            if (i >= 5) {
                if (i != 5) {
                    if (i == 6) {
                        ibtn = 118;
                    }
                } else {
                    ibtn = 1;
                }
            } else {
                ibtn = i + 161;
            }
            GetWindowRect(GetDlgItem(hwnd, ibtn), &rc);
            MapWindowPoints(0x0, hwnd, (POINT *)&rc, 0x2);
            if (dxyVCRSquare < 64) {
                dx = 0;
            } else {
                dx = rc.right - rc.left;
                dx = (int32_t)dxyVCRBoard / 2 + 8 - (int32_t)(7 * dx) / 2 - 24 + (dx + 8) * i;
                dx = dx - rc.left;
            }
            OffsetRect(&rc, dx, dxyVCRBoard + 16 - rc.top);
            SetWindowPos(GetDlgItem(hwnd, ibtn), 0x0, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        EnableVCRButtons();
        StickyDlgPos(hwnd, &ptStickyVCRDlg, 1);
        fAnimate = 1;
        return 1;
    case WM_ERASEBKGND:
        GetClientRect(hwnd, &rc);
        FillRect((HDC)wParam, &rc, hbrButtonFace);
        return 1;
    case WM_SETCURSOR:
        GetCursorPos(&t_pt_107d);
        pt = PointTo16(t_pt_107d);
        t_pt_108c_1 = PointFrom16(pt);
        ScreenToClient(hwnd, &t_pt_108c_1);
        pt = PointTo16(t_pt_108c_1);
        if (pt.x <= 8 || pt.x >= (dxyVCRSquare + 3) * 10 + 0x8 || pt.y < 8 || pt.y >= (dxyVCRSquare + 3) * 10 + 0x8) {
            return 0;
        }
        SetCursor(hcurHand);
        return 1;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) == 0 && PtInRect(&rgrcBuildSpin[1], PointFrom16(pt)) == 0) {
            pt.x = (int32_t)(pt.x - 8) / (dxyVCRSquare + 3);
            pt.y = (int32_t)(pt.y - 8) / (dxyVCRSquare + 3);
            if (pt.x < 10 || pt.y < 2 || pt.y >= 10 || viVCRFocus < 0) {
                if (pt.x < 0 || pt.y < 0 || pt.x >= 10 || pt.y >= 10) {
                    return 0;
                }
                brc = LOBYTE((pt.y & 0xf) << 0x4 | (pt.x & 0xf));
                if (message != WM_RBUTTONDOWN) {
                    t_scratch_m18_3 = brc;
                    if (t_scratch_m18_3 != vbrcVCRFocus || viVCRFocus == -1) {
                        vbrcVCRFocus = brc;
                        viVCRFocus = vlpbdVCR->ctok - 1;
                    }
                    i = viVCRFocus;
                    while (1) {
                        i = i + 1;
                        if (i == vlpbdVCR->ctok) {
                            i = 0;
                        }
                        t_scratch_m18_4 = vrgtok[i].brc;
                        if (t_scratch_m18_4 == brc && vrgtok[i].csh > 0x0)
                            break;
                        if (i == viVCRFocus)
                            goto L_15fb;
                    }
                    viVCRFocus = i;
                    goto GoodSel;
                L_15fb:
                    viVCRFocus = -1;
                } else {
                    iSel = PopupVCRMenu(hwnd, LOWORD(lParam), HIWORD(lParam), brc);
                    if (iSel < 0) {
                        return 0;
                    }
                    vbrcVCRFocus = brc;
                    viVCRFocus = iSel;
                }
            GoodSel:
                DrawVCR(0x0, -2, -1);
                return 0;
            }
            GlobalPD.grPopup = grPopupShdef;
            if (vrgtok[viVCRFocus].grobj != grobjPlanet) {
                GlobalPD.lpshdef = rglpshdef[vrgtok[viVCRFocus].iplr] + vrgtok[viVCRFocus].ishdef;
            } else {
                GlobalPD.lpshdef = rglpshdefSB[vrgtok[viVCRFocus].iplr] + (vrgtok[viVCRFocus].ishdef - 16);
            }
            GlobalPD.fShowDamage = 1;
            GlobalPD.fToken = 1;
            GlobalPD.fHideCounts = vrgtok[viVCRFocus].iplr == idPlayer ? 0 : 1;
            Popup(hwnd, LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
        if (PtInRect(rgrcBuildSpin, PointFrom16(pt)) == 0) {
            iDir = 1;
            bt = 35;
            prc = &rgrcBuildSpin[1];
        } else {
            iDir = -1;
            bt = 34;
            prc = rgrcBuildSpin;
        }
        iCur = viSpeedVCR;
        if (iCur <= 4) {
            if (iCur < 0) {
                iCur = 0;
            }
        } else {
            iCur = 4;
        }
        hdc = GetDC(hwnd);
        bkMode = SetBkMode(hdc, OPAQUE);
        crBkSav = SetBkColor(hdc, crButtonFace);
        SelectObject(hdc, rghfontArial8[1]);
        InitBtnTrack(&btnt, hwnd, 0x0, prc, bt, 80, 0, 0, 0x0);
        while (FTrackBtn(&btnt) != 0) {
            if ((iDir == -1 && iCur > 0) || (iDir == 1 && iCur < 4)) {
                iCur = iCur + iDir;
                bt = _wsprintf(szWork, PszGetCompressedString(idsPlaybackSpeedD), iCur + 1);
                TextOut(hdc, ptSpeedVCR.x, ptSpeedVCR.y, szWork, bt);
            }
        }
        viSpeedVCR = iCur;
        SelectObject(hdc, rghfontArial8[0]);
        SetBkColor(hdc, crBkSav);
        ReleaseDC(hwnd, hdc);
        if (gd.fVCRTimer != 0x0) {
            KillTimer(hwnd, 0xa6c);
            gd.fVCRTimer = SetTimer(hwnd, 0xa6c, 0x23a - 120 * viSpeedVCR, 0x0) == 0x0 ? 0x0 : 0x1;
        }
        return 1;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DrawVCR(hdc, -1, -1);
        EndPaint(hwnd, &ps);
        return 1;
    case WM_TIMER:
        if (gd.fVCRTimer == 0x0) {
            return 0;
        }
        if (viStepVCRCur != vcStepVCR)
            goto NextBtn;
        i = 2;
        break;
    case WM_COMMAND:
        if (GET_WM_COMMAND_ID(wParam, lParam) >= IDC_VCR_REW_ALL && GET_WM_COMMAND_ID(wParam, lParam) <= IDC_VCR_FWD_ALL) {
            i = GET_WM_COMMAND_ID(wParam, lParam) - 161;
            if (gd.fVCRTimer == 0x0)
                goto L_16d3;
            break;
        }
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case IDOK:
        case IDCANCEL:
            if (gd.fVCRTimer != 0x0) {
                gd.fVCRTimer = 0x0;
                KillTimer(hwnd, 0xa6c);
            }
            if (gd.fTutorial != 0x0) {
                tutor.fProgress = 0x1;
            }
            StickyDlgPos(hwnd, &ptStickyVCRDlg, 0);
            EndDialog(hwnd, i);
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, 0x1, 0x43a);
            return 1;
        default:
            return 0;
        }
    case WM_DESTROY:
        hwndVCRDlg = 0x0;
    default:
        return 0;
    }
    KillTimer(hwnd, 0xa6c);
    gd.fVCRTimer = 0x0;
    if (i == 2) {
        return 0;
    }
L_16d3:
    if (GetAsyncKeyState(17) >= 0) {
        if (GetAsyncKeyState(16) >= 0) {
            dStep = 1;
        } else {
            dStep = 10;
        }
    } else {
        dStep = 100;
    }
    fAnimate = 0;
    switch (i) {
    case 0:
        iStep = -1;
        break;
    case 1:
        iStep = viStepVCRCur - dStep;
        if (iStep >= -1)
            break;
        iStep = -1;
        break;
    case 2:
        gd.fVCRTimer = SetTimer(hwnd, 0xa6c, 0x23a - 120 * viSpeedVCR, 0x0) == 0x0 ? 0x0 : 0x1;
    case 3:
        goto NextBtn;
    case 4:
        iStep = vcStepVCR;
    default:
    }
    goto L_1807;
NextBtn:
    if (i != 3) {
        iStep = viStepVCRCur + 1;
    } else {
        iStep = viStepVCRCur + dStep;
    }
    if (iStep > vcStepVCR) {
        iStep = vcStepVCR;
    }
    fAnimate = viSpeedVCR >= 4 ? 0 : 1;
L_1807:
    SetVCRBoard(iStep);
    DrawVCR(0x0, -2, -1);
    return 0;
}

void GetVCRStats(int16_t itok, int32_t *pdpArmor, DV *pdv, int32_t *pdpShields, int16_t *pcsh) {
    int16_t  cshT;
    DV       dv;
    int32_t  dpShields;
    int32_t  dpArmor;
    int16_t  i;
    int16_t  cshKill;
    uint16_t dpShdef;

    dpArmor = 0;
    dpShields = 0;
    cshKill = 0;
    dv.dp = 0xffff;
    dpArmor = vrgdpVCR[itok];
    for (i = 0; i < vlpbrVCR->ctok; i++) {
        if (vlpbrVCR->rgkill[i].itok == itok) {
            cshKill = cshKill + vlpbrVCR->rgkill[i].cshKill;
            dpShields = dpShields + (int32_t)((uint32_t)(vlpbrVCR->rgkill[i].dpShield & 0x1fff) << (vlpbrVCR->rgkill[i].dpShield >> 0xd << 0x1));
            dv.dp = vlpbrVCR->rgkill[i].dv.dp;
        }
    }
    if (dv.dp == 0xffff) {
        dv.dp = vrgtok[itok].dv.dp;
        if (dv.pctDp > 0x1f3) {
            dv.pctDp = 0x1f3;
        }
    } else {
        dpShdef = LpshdefFromTok(vrgtok + itok)->hul.dp;
        cshT = vrgtok[itok].csh - cshKill;
        dpArmor = (uint32_t)((uint32_t)dpShdef * (int32_t)cshT);
        cshT = LOWORD((int32_t)((int32_t)((int32_t)cshT * dv.pctSh) / 0x64));
        if (cshT <= 0) {
            cshT = 1;
        }
        dpArmor = dpArmor - (int32_t)((int32_t)((int32_t)((int32_t)((uint32_t)dpShdef * dv.pctDp) / 0xa) * (int32_t)cshT) / 0x32);
    }
    cshT = vrgtok[itok].csh - cshKill;
    if (cshT < 1) {
        cshT = 0;
        dpArmor = 0;
        dpShields = (uint32_t)((uint32_t)vrgtok[itok].dpShield * (uint32_t)vrgtok[itok].csh);
    }
    if (pdv != 0x0) {
        pdv->dp = dv.dp;
    }
    if (pcsh != 0x0) {
        *pcsh = cshT;
    }
    if (pdpArmor != 0x0) {
        *pdpArmor = dpArmor;
    }
    if (pdpShields != 0x0) {
        *pdpShields = dpShields;
    }
    return;
}

void DrawVCR(HDC hdc, int16_t iStart, int16_t iEnd) {
    int16_t  ctok;
    int16_t  ibmpRace;
    int16_t  bkMode;
    HBRUSH   hbrSav;
    int32_t  dpShields;
    int16_t  itokT;
    int32_t  dpT;
    int16_t  fCreatedDC;
    int32_t  dpArmor;
    int16_t  y;
    uint8_t  rgfSeen[256];
    int16_t  c;
    int16_t  i;
    uint8_t  brcT;
    SHDEF   *lpshdef;
    int16_t  ibmp;
    int16_t  csh;
    char    *psz;
    int16_t  dx;
    int16_t  j;
    char     szT[96];
    int16_t  fJam;
    RECT     rc;
    int16_t  x;
    int16_t  cshT;
    int32_t  dpShT;
    DV       dv;
    int16_t  xT;
    int16_t  cshNew;
    char    *t_merge_2766_0001;
    uint16_t t_merge_27d6_0001;
    StringId t_merge_2965_0001;
    uint8_t  t_merge_2d54_0001;
    uint16_t t_scratch_m19e_5;
    uint16_t t_scratch_m19e_8;
    uint16_t t_scratch_m19e_9;

    fCreatedDC = hdc == 0x0 ? 1 : 0;
    if (fCreatedDC != 0) {
        hdc = GetDC(hwndVCRDlg);
    }
    hbrSav = SelectObject(hdc, hbrButtonFace);
    bkMode = SetBkMode(hdc, TRANSPARENT);
    memset(rgfSeen, 0, 0x100);
    GetClientRect(hwndVCRDlg, &rc);
    if (iStart == -2) {
        PatBlt(hdc, dxyVCRBoard + 10, 0, rc.right - dxyVCRBoard - 10, dxyVCRBoard + 8, PATCOPY);
        iStart = -1;
    }
    SelectObject(hdc, hbrButtonShadow);
    if (iStart == -1) {
        PatBlt(hdc, 8, 8, dxyVCRBoard, 2, PATCOPY);
        PatBlt(hdc, 8, 8, 2, dxyVCRBoard, PATCOPY);
        SelectObject(hdc, hbrButtonHilite);
        PatBlt(hdc, 9, dxyVCRBoard + 6, dxyVCRBoard - 1, 1, PATCOPY);
        PatBlt(hdc, 8, dxyVCRBoard + 7, dxyVCRBoard, 1, PATCOPY);
        PatBlt(hdc, dxyVCRBoard + 6, 9, 1, dxyVCRBoard - 3, PATCOPY);
        PatBlt(hdc, dxyVCRBoard + 7, 8, 1, dxyVCRBoard - 2, PATCOPY);
        SelectObject(hdc, hbrButtonShadow);
        for (i = 1; i < 10; i++) {
            PatBlt(hdc, (dxyVCRSquare + 3) * i + 9, 10, 1, dxyVCRBoard - 4, PATCOPY);
            PatBlt(hdc, 10, (dxyVCRSquare + 3) * i + 9, dxyVCRBoard - 4, 1, PATCOPY);
        }
        x = dxyVCRBoard + 14;
        y = 8;
        SelectObject(hdc, rghfontArial8[1]);
        c = _wsprintf(szWork, PszGetCompressedString(idsPhaseDDRoundDD), viStepVCRCur + 2, vcStepVCR + 2, viRound + 1, vcRound + 1);
        TextOut(hdc, x, y, szWork, c);
        y = y + (dyArial8 + 4);
        c = _wsprintf(szWork, PszGetCompressedString(idsPlaybackSpeedD), viSpeedVCR + 1);
        dx = LOWORD(GetTextExtent(hdc, szWork, c));
        TextOut(hdc, x, y, szWork, c);
        ptSpeedVCR.x = x;
        ptSpeedVCR.y = y;
        SetRect(rgrcBuildSpin, x + dx, y, x + dx + 14, y + 14);
        rgrcBuildSpin[1] = rgrcBuildSpin[0];
        OffsetRect(&rgrcBuildSpin[1], 14, 0);
        for (i = 0; i < 2; i++) {
            DrawBtn(hdc, &rgrcBuildSpin[i], (i == 0 ? 0x2 : 0x3) | 0x20, 0, 0x0);
        }
        if (viStepVCRCur >= 0) {
            y = y + (dyArial8 + 4);
            psz = PszPlayerName(vrgtok[vlpbrVCR->itok].iplr, 1, 1, 1, 0, 0x0);
            TextOut(hdc, x, y, szWork, strlen(psz));
            y = y + dyArial8;
            if (vlpbrVCR->itok == viVCRFocus) {
                SetTextColor(hdc, 0x7f0000);
            }
            if (vrgtok[vlpbrVCR->itok].grobj != grobjPlanet) {
                lpshdef = rglpshdef[vrgtok[vlpbrVCR->itok].iplr] + vrgtok[vlpbrVCR->itok].ishdef;
            } else {
                lpshdef = rglpshdefSB[vrgtok[vlpbrVCR->itok].iplr] + (vrgtok[vlpbrVCR->itok].ishdef - 16);
            }
            csh = vrgtok[vlpbrVCR->itok].csh;
            if (csh <= 1) {
                fstrcpy(szWork, lpshdef->hul.szClass);
                c = strlen(szWork);
            } else {
                c = _wsprintf(szWork, PszGetCompressedString(idsSD), lpshdef->hul.szClass, csh);
            }
            TextOut(hdc, x, y, szWork, c);
            y = y + dyArial8;
            SetTextColor(hdc, crButtonText);
            fJam = 0;
            if (vlpbrVCR->ctok > 0) {
                psz = PszPlayerName(vrgtok[vlpbrVCR->itokAttack].iplr, 0, 1, 1, 0, 0x0);
                c = _wsprintf(szT, PszGetCompressedString(idsAttacksS), psz);
                TextOut(hdc, x, y, szT, c);
                y = y + dyArial8;
                if (vlpbrVCR->rgkill[0].dv.dp != 0x0) {
                    SetTextColor(hdc, 0x7f);
                }
                if (vrgtok[vlpbrVCR->itokAttack].grobj != grobjPlanet) {
                    lpshdef = rglpshdef[vrgtok[vlpbrVCR->itokAttack].iplr] + vrgtok[vlpbrVCR->itokAttack].ishdef;
                } else {
                    lpshdef = rglpshdefSB[vrgtok[vlpbrVCR->itokAttack].iplr] + (vrgtok[vlpbrVCR->itokAttack].ishdef - 16);
                }
                csh = vrgtok[vlpbrVCR->itokAttack].csh;
                if (csh <= 1) {
                    fstrcpy(szWork, lpshdef->hul.szClass);
                    c = strlen(szWork);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsSD), lpshdef->hul.szClass, csh);
                }
                TextOut(hdc, x, y, szWork, c);
                y = y + dyArial8;
                SetTextColor(hdc, crButtonText);
                brcT = vrgtok[vlpbrVCR->itokAttack].brc;
                dpArmor = 0;
                dpShields = 0;
                j = 0;
                for (i = vlpbrVCR->ctok; i > 0; i--) {
                    itokT = vlpbrVCR->rgkill[i - 1].itok;
                    if (itokT == vlpbrVCR->itokAttack && fJam == 0) {
                        fJam = fJam | (vlpbrVCR->rgkill[i - 0x1].grfWeapon & 0xc0);
                    }
                    j = j + vlpbrVCR->rgkill[i - 1].cshKill;
                    if (rgfSeen[itokT] == 0x0) {
                        rgfSeen[itokT] = 0x1;
                        GetVCRStats(itokT, &dpT, 0x0, &dpShT, &cshT);
                        dpArmor = dpArmor + (vrgdpVCR[itokT] - dpT);
                        dpShields = dpShields + dpShT;
                    }
                }
                c = _wsprintf(szWork, PszGetCompressedString(idsDDDoing), brcT & 0xf, brcT >> 0x4);
                TextOut(hdc, x, y, szWork, c);
                y = y + dyArial8;
                if (dpShields != 0) {
                    CchGetString(idsAnd, szT);
                    if (dpArmor <= 0) {
                        if (j <= 0) {
                            t_merge_2766_0001 = ".";
                        } else {
                            t_merge_2766_0001 = ",";
                        }
                    } else {
                        t_merge_2766_0001 = szT;
                    }
                    c = _wsprintf(szWork, PszGetCompressedString(idsLdDamageShieldsS), dpShields, t_merge_2766_0001);
                    TextOut(hdc, x, y, szWork, c);
                    y = y + dyArial8;
                }
                if (dpArmor != 0) {
                    t_merge_27d6_0001 = j <= 0 ? 0x2e : 0x2c;
                    c = _wsprintf(szWork, PszGetCompressedString(idsLdDamageArmorC), dpArmor, t_merge_27d6_0001);
                    TextOut(hdc, x, y, szWork, c);
                    y = y + dyArial8;
                }
                if ((fJam & 0x40) != 0x0 && dpShields == 0 && dpArmor == 0) {
                    psz = PszGetCompressedString(idsDamage3);
                    TextOut(hdc, x, y, psz, strlen(psz));
                    y = y + dyArial8;
                }
                if (j > 0) {
                    c = _wsprintf(szWork, PszGetCompressedString(idsDestroyingDShip), j);
                    if (j != 1) {
                        strcpy(&szWork[c], "s.");
                        c = c + 2;
                    } else {
                        strcpy(&szWork[c], ".");
                        c = c + 1;
                    }
                    TextOut(hdc, x, y, szWork, c);
                    y = y + dyArial8;
                }
            }
            if (fJam != 0) {
                SetTextColor(hdc, 0x7f);
                if ((fJam & 0x40) == 0x0 || dpArmor != 0) {
                    t_merge_2965_0001 = idsTorpedoesDeflected;
                } else {
                    t_merge_2965_0001 = idsTorpedoesDeflected2;
                }
                c = CchGetString(t_merge_2965_0001, szWork);
                TextOut(hdc, x, y, szWork, c);
                y = y + dyArial8;
                SetTextColor(hdc, crButtonText);
            }
        }
        if (vbrcVCRFocus != 0xff) {
            y = 200;
            c = _wsprintf(szWork, PszGetCompressedString(idsSelectionDD), vbrcVCRFocus & 0xf, vbrcVCRFocus >> 0x4);
            TextOut(hdc, x, y, szWork, c);
            y = y + dyArial8;
            if (viVCRFocus >= 0) {
                csh = 0;
                psz = PszPlayerName(vrgtok[viVCRFocus].iplr, 1, 1, 1, 0, 0x0);
                c = strlen(psz);
                TextOut(hdc, x, y, szWork, c);
                y = y + dyArial8;
                if (vrgtok[viVCRFocus].grobj != grobjPlanet) {
                    lpshdef = rglpshdef[vrgtok[viVCRFocus].iplr] + vrgtok[viVCRFocus].ishdef;
                } else {
                    lpshdef = rglpshdefSB[vrgtok[viVCRFocus].iplr] + (vrgtok[viVCRFocus].ishdef - 16);
                }
                csh = vrgtok[viVCRFocus].csh;
                GetVCRStats(viVCRFocus, &dpT, &dv, &dpShields, &cshT);
                cshNew = cshT;
                cshT = csh - cshT;
                if (csh <= 1 && cshT == 0) {
                    fstrcpy(szWork, lpshdef->hul.szClass);
                    c = strlen(szWork);
                } else {
                    c = _wsprintf(szWork, PszGetCompressedString(idsSD), lpshdef->hul.szClass, csh);
                }
                if (cshT != 0) {
                    c = c + _wsprintf(&szWork[c], " (-%d)", cshT);
                }
                SetTextColor(hdc, 0x7f0000);
                TextOut(hdc, x, y, szWork, c);
                y = y + dyArial8;
                xT = (int32_t)(rc.right - (dxyVCRBoard + 14)) / 2 + x;
                SetTextColor(hdc, crButtonText);
                if (cshNew <= 0) {
                    c = CchGetString(idsDead3, szWork);
                    TextOut(hdc, x, y, szWork, c);
                    y = y + 5 * dyArial8;
                } else {
                    if (vrgtok[viVCRFocus].grobj != grobjPlanet) {
                        i = vrgtok[viVCRFocus].spd + 1;
                    } else {
                        i = 0;
                    }
                    t_merge_2d54_0001 = vrgtok[viVCRFocus].initMin >= 0xff ? 0x0 : vrgtok[viVCRFocus].initMin;
                    c = _wsprintf(szWork, PszGetCompressedString(idsInitiativeD), t_merge_2d54_0001);
                    TextOut(hdc, x, y, szWork, c);
                    c = _wsprintf(szWork, PszGetCompressedString(idsMovementS), &rgszSpeed[i * 3]);
                    TextOut(hdc, xT, y, szWork, c);
                    y = y + dyArial8;
                    c = _wsprintf(szWork, PszGetCompressedString(idsArmorLd), dpT);
                    TextOut(hdc, x, y, szWork, c);
                    if (dv.dp != 0x0) {
                        csh = csh - cshT;
                        csh = LOWORD((int32_t)((int32_t)(dv.pctSh * (int32_t)csh) / 0x64));
                        if (csh <= 0) {
                            csh = 1;
                        }
                        dpT = (uint32_t)(dv.pctDp / 0x5);
                        if (dpT == 0) {
                            dpT = 1;
                        }
                        SetTextColor(hdc, 0x7f);
                        if (vrgtok[viVCRFocus].grobj != grobjPlanet) {
                            c = _wsprintf(szWork, PszGetCompressedString(idsDamageLdD), (int32_t)csh, LOWORD(dpT));
                        } else {
                            c = _wsprintf(szWork, PszGetCompressedString(idsDamageD), LOWORD(dpT));
                        }
                    } else {
                        c = CchGetString(idsDamageNone, szWork);
                    }
                    TextOut(hdc, xT, y, szWork, c);
                    SetTextColor(hdc, crButtonText);
                    y = y + dyArial8;
                    if ((uint32_t)((uint32_t)vrgtok[viVCRFocus].dpShield * (int32_t)cshNew) - dpShields <= 0x0) {
                        c = CchGetString(idsShieldsNone, szWork);
                    } else {
                        c = _wsprintf(szWork, PszGetCompressedString(idsShieldsLd),
                                      (uint32_t)((uint32_t)vrgtok[viVCRFocus].dpShield * (int32_t)cshNew) - dpShields);
                    }
                    TextOut(hdc, x, y, szWork, c);
                    y = y + dyArial8;
                    if (vrgtok[viVCRFocus].pctJam != 0x0) {
                        c = _wsprintf(szWork, PszGetCompressedString(idsJammingD), vrgtok[viVCRFocus].pctJam);
                        TextOut(hdc, x, y, szWork, c);
                        y = y + dyArial8;
                    }
                    if (vrgtok[viVCRFocus].grobj != grobjPlanet) {
                        if (vbrcVCRFocus == 0xff) {
                            i = 414;
                        } else {
                            i = vrgtok[viVCRFocus].mdTactic + 408;
                        }
                        if (i != 408) {
                            CchGetString(idsTacticS, szT);
                            c = _wsprintf(szWork, szT, PszGetCompressedString(i));
                        } else {
                            CchGetString(idsTacticSDMoves, szT);
                            c = _wsprintf(szWork, szT, PszGetCompressedString(i), vrgtok[viVCRFocus].dzDis);
                        }
                        TextOut(hdc, x, y, szWork, c);
                        y = y + dyArial8;
                    }
                    if (i != 414) {
                        CchGetString(idsPrimayTargetS, szT);
                        i = vrgtok[viVCRFocus].mdTarget1 + 400;
                        c = _wsprintf(szWork, szT, PszGetCompressedString(i));
                        TextOut(hdc, x, y, szWork, c);
                        y = y + dyArial8;
                        CchGetString(idsSecondaryTargetS, szT);
                        i = vrgtok[viVCRFocus].mdTarget2 + 400;
                        c = _wsprintf(szWork, szT, PszGetCompressedString(i));
                        TextOut(hdc, x, y, szWork, c);
                        y = y + dyArial8;
                    }
                }
                SetRect(&rc, x, y + 4, x + dyArial8 + 4, y + dyArial8 + 8);
                DrawBtn(hdc, &rc, 8, 0, "?");
            }
        }
        iStart = 0;
        iEnd = 99;
    }
    for (i = iStart; i <= iEnd; i++) {
        x = (int32_t)i % 10;
        y = (int32_t)i / 10;
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, dxyVCRSquare + 2, 1, BLACKNESS);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, 1, dxyVCRSquare + 2, BLACKNESS);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10 + dxyVCRSquare + 1, (dxyVCRSquare + 3) * y + 10, 1, dxyVCRSquare + 2, BLACKNESS);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10 + dxyVCRSquare + 1, dxyVCRSquare + 2, 1, BLACKNESS);
        ctok = 0;
        ibmp = -1;
        dpT = 0;
        for (j = 0; j < vlpbdVCR->ctok; j++) {
            t_scratch_m19e_5 = vrgtok[j].brc;
            if (t_scratch_m19e_5 == (((y & 0xf) << 0x4 | (x & 0xf)) & 0xff) && vrgtok[j].csh > 0x0) {
                ctok = ctok + 1;
                dpT = dpT + (uint32_t)vrgtok[j].csh;
                if (ibmp == -1 || j == viVCRFocus) {
                    if (vrgtok[j].grobj != grobjPlanet) {
                        ibmp = rglpshdef[vrgtok[j].iplr][vrgtok[j].ishdef].hul.ibmp;
                    } else {
                        ibmp = rglpshdefSB[vrgtok[j].iplr][vrgtok[j].ishdef - 16].hul.ibmp;
                    }
                    ibmpRace = rgplr[vrgtok[j].iplr].iPlrBmp;
                }
            }
        }
        if (dpT < 32767) {
            csh = LOWORD(dpT);
        } else {
            csh = 32767;
        }
        if (ctok <= 0) {
            PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, dxyVCRSquare + 2, dxyVCRSquare + 2, BLACKNESS);
        } else {
            PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, dxyVCRSquare + 2, 1, BLACKNESS);
            PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, 1, dxyVCRSquare + 2, BLACKNESS);
            PatBlt(hdc, (dxyVCRSquare + 3) * x + 10 + dxyVCRSquare + 1, (dxyVCRSquare + 3) * y + 10, 1, dxyVCRSquare + 2, BLACKNESS);
            PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10 + dxyVCRSquare + 1, dxyVCRSquare + 2, 1, BLACKNESS);
            DrawFleetBitmap(0x0, hdc, (dxyVCRSquare + 3) * x + 11, (dxyVCRSquare + 3) * y + 11, 0, ibmp, ctok, dxyVCRSquare >= 64 ? 0 : 1, ibmpRace, csh);
        }
        t_scratch_m19e_8 = vbrcVCRFocus;
        if (t_scratch_m19e_8 != (((y & 0xf) << 0x4 | (x & 0xf)) & 0xff) || viVCRFocus != -1) {
            if (viVCRFocus < 0)
                continue;
            t_scratch_m19e_9 = vrgtok[viVCRFocus].brc;
            if (t_scratch_m19e_9 != (((y & 0xf) << 0x4 | (x & 0xf)) & 0xff) || vrgtok[viVCRFocus].csh <= 0x0)
                continue;
        }
        hbrSav = SelectObject(hdc, hbrBlue);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, dxyVCRSquare + 1, 2, PATCOPY);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10, 2, dxyVCRSquare + 1, PATCOPY);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10, (dxyVCRSquare + 3) * y + 10 + dxyVCRSquare, dxyVCRSquare + 1, 2, PATCOPY);
        PatBlt(hdc, (dxyVCRSquare + 3) * x + 10 + dxyVCRSquare, (dxyVCRSquare + 3) * y + 10, 2, dxyVCRSquare + 1, PATCOPY);
        SelectObject(hdc, hbrSav);
    }
    if (vlpbrVCR->ctok > 0) {
        AnimateAttack(hdc);
    }
    SetBkMode(hdc, bkMode);
    SelectObject(hdc, hbrSav);
    if (fCreatedDC != 0) {
        ReleaseDC(hwndVCRDlg, hdc);
    }
    return;
}

void Delay(int16_t ctick) {
    uint32_t     dwTickLast;
    uint32_t     dwTickCur;
    tagTIMERINFO ti;

    ti.dwSize = 0xc;
    TimerCount(&ti);
    dwTickLast = ti.dwmsSinceStart;
    do {
        TimerCount(&ti);
        dwTickCur = ti.dwmsSinceStart;
    } while (dwTickCur >= dwTickLast && dwTickCur < (int32_t)ctick + dwTickLast);
    return;
}

void AnimateAttack(HDC hdc) {
    TOK         *ptokSrc;
    TOK         *ptokAttack;
    POINT16      ptBeam1;
    int16_t      cFrame;
    int16_t      dyFrame;
    POINT16      ptRay2;
    POINT16      ptTop;
    uint32_t     dwTickLast;
    int16_t      dxFrame;
    uint32_t     dwTickCur;
    POINT16      ptBase;
    int16_t      dy;
    POINT16      ptRay1;
    int16_t      y;
    POINT16      ptRight;
    POINT16      ptDest;
    int16_t      iHit;
    uint16_t     grfWeapon;
    POINT16      ptSrc;
    POINT16      ptTorp;
    POINT16      ptLeft;
    tagTIMERINFO ti;
    int16_t      iFrame;
    int16_t      dx;
    int16_t      fKill;
    POINT16      ptDestBottom;
    POINT16      ptBeam2;
    POINT16      ptBottom;
    POINT16      ptDestTop;
    POINT16      ptDestRight;
    POINT16      ptDestLeft;
    int16_t      x;
    HDC          hdcMem;
    HBITMAP      hbmpSav;
    HBITMAP      hbmpScreen;
    uint16_t     t_scratch_m76_3;
    uint16_t     t_scratch_m76_4;
    uint16_t     t_scratch_m76_5;
    uint16_t     t_scratch_m76_8;
    uint16_t     t_scratch_m76_9;
    int16_t      t_merge_3ef2_0001;
    int16_t      t_merge_3ef2_0002;
    int16_t      t_merge_3f10_0001;
    int16_t      t_merge_3f10_0002;
    int16_t      t_merge_3f2e_0001;
    int16_t      t_merge_3f2e_0002;
    int16_t      t_merge_3f96_0001;
    int16_t      t_merge_3f96_0002;
    int16_t      t_merge_3fb4_0001;
    int16_t      t_merge_3fb4_0002;
    int16_t      t_merge_3fd2_0001;
    int16_t      t_merge_3fd2_0002;
    int16_t      t_scratch_m7c;
    int16_t      t_call_41c9;
    int16_t      t_merge_41df_0001;
    int16_t      t_call_41d7;

    grfWeapon = 0x0;
    fKill = 0;
    if (viStepVCRCur >= 0) {
        ptokSrc = vrgtok + vlpbrVCR->itok;
        x = ptokSrc->brc & 0xf;
        y = ptokSrc->brc >> 0x4;
        ptSrc.x = (dxyVCRSquare + 3) * x + 10 + (int32_t)dxyVCRSquare / 2 + 1;
        ptSrc.y = (dxyVCRSquare + 3) * y + 10 + (int32_t)dxyVCRSquare / 2 + 1;
        t_scratch_m76_3 = (int32_t)dxyVCRSquare / 2;
        ptTop.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_3 + 1;
        ptBottom.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_3 + 1;
        t_scratch_m76_4 = (int32_t)dxyVCRSquare / 2;
        ptRight.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_4 + 1;
        ptLeft.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_4 + 1;
        ptBottom.y = (int32_t)dxyVCRSquare / 3 + ptLeft.y;
        ptTop.y = ptLeft.y - (int32_t)dxyVCRSquare / 3;
        ptLeft.x = ptBottom.x - (int32_t)dxyVCRSquare / 3;
        ptRight.x = (int32_t)dxyVCRSquare / 3 + ptBottom.x;
        iHit = 0;
        do {
            grfWeapon = vlpbrVCR->rgkill[iHit].grfWeapon;
            if (vlpbrVCR->rgkill[iHit].cshKill != 0x0) {
                fKill = 1;
            }
            for (iFrame = iHit + 1; iFrame < vlpbrVCR->ctok; iFrame++) {
                t_scratch_m76_5 = vlpbrVCR->rgkill[iFrame].itok;
                if (t_scratch_m76_5 != vlpbrVCR->rgkill[iHit].itok)
                    break;
                grfWeapon = grfWeapon | vlpbrVCR->rgkill[iFrame].grfWeapon;
                if (vlpbrVCR->rgkill[iFrame].cshKill != 0x0) {
                    fKill = 1;
                }
            }
            ptokAttack = vrgtok + vlpbrVCR->rgkill[iHit].itok;
            x = ptokAttack->brc & 0xf;
            y = ptokAttack->brc >> 0x4;
            dx = (ptokSrc->brc & 0xf) - x;
            dy = (ptokSrc->brc >> 0x4) - y;
            ptDest.x = (dxyVCRSquare + 3) * x + 10 + (int32_t)dxyVCRSquare / 2 + 1;
            ptDest.y = (dxyVCRSquare + 3) * y + 10 + (int32_t)dxyVCRSquare / 2 + 1;
            t_scratch_m76_8 = (int32_t)dxyVCRSquare / 2;
            ptDestTop.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_8 + 1;
            ptDestBottom.x = (dxyVCRSquare + 3) * x + 10 + t_scratch_m76_8 + 1;
            t_scratch_m76_9 = (int32_t)dxyVCRSquare / 2;
            ptDestRight.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_9 + 1;
            ptDestLeft.y = (dxyVCRSquare + 3) * y + 10 + t_scratch_m76_9 + 1;
            ptDestBottom.y = (int32_t)dxyVCRSquare / 3 + ptDestLeft.y;
            ptDestTop.y = ptDestLeft.y - (int32_t)dxyVCRSquare / 3;
            ptDestLeft.x = ptDestBottom.x - (int32_t)dxyVCRSquare / 3;
            ptDestRight.x = (int32_t)dxyVCRSquare / 3 + ptDestBottom.x;
            iHit = iFrame;
            if (dx != 0 || dy != 0) {
                if (dx != 0 && (abs(dx) != 1 || abs(dy) <= 2)) {
                    if (dy != 0 && (abs(dy) != 1 || abs(dx) <= 2)) {
                        if (dx <= 0) {
                            if (dy <= 0) {
                                ptBeam1 = ptBottom;
                                ptRay1 = ptDestLeft;
                                ptRay2 = ptDestTop;
                            } else {
                                ptBeam1 = ptTop;
                                ptRay1 = ptDestBottom;
                                ptRay2 = ptDestLeft;
                            }
                            ptBeam2 = ptRight;
                            ptTorp.x = ptRight.x;
                            ptTorp.y = ptBeam1.y;
                        } else {
                            if (dy <= 0) {
                                ptRay2 = ptDestRight;
                                ptRay1 = ptDestTop;
                                ptBeam1 = ptBottom;
                            } else {
                                ptRay2 = ptDestBottom;
                                ptRay1 = ptDestRight;
                                ptBeam1 = ptTop;
                            }
                            ptBeam2 = ptLeft;
                            ptTorp.x = ptLeft.x;
                            ptTorp.y = ptBeam1.y;
                        }
                    } else {
                        ptBeam1 = ptTop;
                        ptBeam2 = ptBottom;
                        if (dx <= 0) {
                            t_merge_3f96_0001 = ptRight.x;
                            t_merge_3f96_0002 = ptRight.y;
                        } else {
                            t_merge_3f96_0001 = ptLeft.x;
                            t_merge_3f96_0002 = ptLeft.y;
                        }
                        ptTorp.x = t_merge_3f96_0001;
                        ptTorp.y = t_merge_3f96_0002;
                        if (dx <= 0) {
                            t_merge_3fb4_0001 = ptDestBottom.x;
                            t_merge_3fb4_0002 = ptDestBottom.y;
                        } else {
                            t_merge_3fb4_0001 = ptDestTop.x;
                            t_merge_3fb4_0002 = ptDestTop.y;
                        }
                        ptRay1.x = t_merge_3fb4_0001;
                        ptRay1.y = t_merge_3fb4_0002;
                        if (dx <= 0) {
                            t_merge_3fd2_0001 = ptDestTop.x;
                            t_merge_3fd2_0002 = ptDestTop.y;
                        } else {
                            t_merge_3fd2_0001 = ptDestBottom.x;
                            t_merge_3fd2_0002 = ptDestBottom.y;
                        }
                        ptRay2.x = t_merge_3fd2_0001;
                        ptRay2.y = t_merge_3fd2_0002;
                    }
                } else {
                    ptBeam1 = ptRight;
                    ptBeam2 = ptLeft;
                    if (dy <= 0) {
                        t_merge_3ef2_0001 = ptBottom.x;
                        t_merge_3ef2_0002 = ptBottom.y;
                    } else {
                        t_merge_3ef2_0001 = ptTop.x;
                        t_merge_3ef2_0002 = ptTop.y;
                    }
                    ptTorp.x = t_merge_3ef2_0001;
                    ptTorp.y = t_merge_3ef2_0002;
                    if (dy <= 0) {
                        t_merge_3f10_0001 = ptDestRight.x;
                        t_merge_3f10_0002 = ptDestRight.y;
                    } else {
                        t_merge_3f10_0001 = ptDestLeft.x;
                        t_merge_3f10_0002 = ptDestLeft.y;
                    }
                    ptRay2.x = t_merge_3f10_0001;
                    ptRay2.y = t_merge_3f10_0002;
                    if (dy <= 0) {
                        t_merge_3f2e_0001 = ptDestLeft.x;
                        t_merge_3f2e_0002 = ptDestLeft.y;
                    } else {
                        t_merge_3f2e_0001 = ptDestRight.x;
                        t_merge_3f2e_0002 = ptDestRight.y;
                    }
                    ptRay1.x = t_merge_3f2e_0001;
                    ptRay1.y = t_merge_3f2e_0002;
                }
                if ((grfWeapon & 0x3) != 0x0) {
                    SelectObject(hdc, (grfWeapon & 0x2) == 0x0 ? hpenEnemy : hpenStarbase);
                    MoveTo(hdc, ptBeam1.x, ptBeam1.y);
                    LineTo(hdc, ptDest.x, ptDest.y);
                    MoveTo(hdc, ptBeam2.x, ptBeam2.y);
                    LineTo(hdc, ptDest.x, ptDest.y);
                    DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[0]);
                }
                if ((grfWeapon & 0x4) != 0x0) {
                    if (fAnimate != 0) {
                        hdcMem = CreateCompatibleDC(hdc);
                        if (hdcMem == 0x0)
                            goto LFinishUp;
                        hbmpScreen = CreateCompatibleBitmap(hdc, 32, 32);
                        if (hbmpScreen == 0x0) {
                            DeleteDC(hdcMem);
                            goto LFinishUp;
                        }
                        hbmpSav = SelectObject(hdcMem, hbmpScreen);
                        t_scratch_m7c = abs(dx);
                        if (t_scratch_m7c <= abs(dy)) {
                            t_call_41d7 = abs(dy);
                            t_merge_41df_0001 = t_call_41d7;
                        } else {
                            t_call_41c9 = abs(dx);
                            t_merge_41df_0001 = t_call_41c9;
                        }
                        cFrame = t_merge_41df_0001 * ((grfWeapon & 0x4) == 0x0 ? 0x4 : 0x8);
                        ptBase = ptTorp;
                        dxFrame = ptTorp.x - ptDest.x;
                        dyFrame = ptTorp.y - ptDest.y;
                        ti.dwSize = 0xc;
                        TimerCount(&ti);
                        dwTickLast = ti.dwmsSinceStart;
                        for (iFrame = 0; iFrame < cFrame; iFrame++) {
                            BitBlt(hdcMem, 0, 0, 32, 32, hdc, ptTorp.x - 16, ptTorp.y - 16, SRCCOPY);
                            DrawIcon(hdc, ptTorp.x - 16, ptTorp.y - 16, rghiconVCR[(iFrame & 0x3) + 0x3]);
                            do {
                                TimerCount(&ti);
                                dwTickCur = ti.dwmsSinceStart;
                            } while (dwTickCur >= dwTickLast && dwTickCur < dwTickLast + 0x23 - (int32_t)(10 * viSpeedVCR));
                            dwTickLast = dwTickCur;
                            BitBlt(hdc, ptTorp.x - 16, ptTorp.y - 16, 32, 32, hdcMem, 0, 0, SRCCOPY);
                            ptTorp.x = ptBase.x - LOWORD((int32_t)((int32_t)((int32_t)dxFrame * (int32_t)iFrame) / (int32_t)cFrame));
                            ptTorp.y = ptBase.y - LOWORD((int32_t)((int32_t)((int32_t)dyFrame * (int32_t)iFrame) / (int32_t)cFrame));
                        }
                        SelectObject(hdcMem, hbmpSav);
                        DeleteObject(hbmpScreen);
                        DeleteDC(hdcMem);
                    }
                    if ((grfWeapon & 0x40) == 0x0) {
                        DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[1]);
                    }
                }
            }
        LFinishUp:;
        } while (iHit < vlpbrVCR->ctok);
        for (iFrame = 0; iFrame < vlpbrVCR->ctok; iFrame++) {
            fKill = vlpbrVCR->rgkill[iFrame].cshKill <= 0x0 ? 0 : 1;
            ptokAttack = vrgtok + vlpbrVCR->rgkill[iFrame].itok;
            ptDest.x = (ptokAttack->brc & 0xf) * (dxyVCRSquare + 0x3) + 10 + (int32_t)dxyVCRSquare / 2 + 1;
            ptDest.y = (ptokAttack->brc >> 0x4) * (dxyVCRSquare + 0x3) + 10 + (int32_t)dxyVCRSquare / 2 + 1;
            DrawIcon(hdc, ptDest.x - 16, ptDest.y - 16, rghiconVCR[fKill == 0 ? 0 : 2]);
        }
        fAnimate = 0;
    }
    return;
}

int16_t PopupVCRMenu(HWND hwnd, int16_t x, int16_t y, uint8_t brc) {
    int16_t  fAttack;
    char    *rgsz[40];
    int16_t  i;
    int16_t  c;
    char     rgch[1536];
    SHDEF   *lpshdef;
    int16_t  rgid[40];
    int16_t  iChecked;
    int16_t  iSel;
    int16_t  j;
    char    *psz;
    int16_t  cch;
    int16_t  cKilled;
    uint16_t t_scratch_m6ba;
    uint16_t t_scratch_m6b8_2;

    c = 0;
    iChecked = -1;
    psz = rgch;
    t_scratch_m6ba = brc;
    fAttack = t_scratch_m6ba == vrgtok[vlpbrVCR->itokAttack].brc ? 1 : 0;
    for (i = 0; i < vlpbdVCR->ctok; i++) {
        t_scratch_m6b8_2 = vrgtok[i].brc;
        if (t_scratch_m6b8_2 == brc && vrgtok[i].csh > 0x0) {
            if (PszPlayerName(vrgtok[i].iplr, 0, 0, 0, 0, 0x0) != szWork) {
            }
            cch = strlen(szWork);
            if (vrgtok[i].grobj != grobjPlanet) {
                lpshdef = rglpshdef[vrgtok[i].iplr] + vrgtok[i].ishdef;
            } else {
                lpshdef = rglpshdefSB[vrgtok[i].iplr] + (vrgtok[i].ishdef - 16);
            }
            cch = cch + _wsprintf(&szWork[cch], " %s * %d", lpshdef->hul.szClass, vrgtok[i].csh);
            if (fAttack != 0 && vlpbrVCR->ctok > 0 && viStepVCRCur >= 0) {
                cKilled = 0;
                for (j = 0; j < vlpbrVCR->ctok; j++) {
                    if (vlpbrVCR->rgkill[j].itok == i) {
                        cKilled = cKilled + vlpbrVCR->rgkill[j].cshKill;
                    }
                }
                if (cKilled > 0) {
                    cch = cch + _wsprintf(&szWork[cch], " (-%d)", cKilled);
                }
            }
            if (psz + (cch + 1) >= &rgch[1535] || c >= 40)
                break;
            rgsz[c] = psz;
            rgid[c] = i;
            strcpy(psz, szWork);
            psz = psz + (cch + 1);
            if (i == viVCRFocus) {
                iChecked = c;
            }
            c = c + 1;
        }
    }
    if (c != 0) {
        iSel = PopupMenu(hwnd, x, y, c, 0x0, rgsz, iChecked, 1);
        if (iSel != -1) {
            return rgid[iSel];
        }
        return -1;
    }
    return -1;
}

void EnableVCRButtons() {
    int16_t i;

    for (i = 161; i < 163; i++) {
        EnableWindow(GetDlgItem(hwndVCRDlg, i), viStepVCRCur <= -1 ? 0 : 1);
    }
    for (i = 163; i < 166; i++) {
        EnableWindow(GetDlgItem(hwndVCRDlg, i), viStepVCRCur >= vcStepVCR ? 0 : 1);
    }
    if (viStepVCRCur != -1) {
        if (viStepVCRCur == vcStepVCR) {
            SetFocus(GetDlgItem(hwndVCRDlg, IDOK));
        }
    } else {
        SetFocus(GetDlgItem(hwndVCRDlg, IDC_VCR_PLAY_PAUSE));
    }
    return;
}
