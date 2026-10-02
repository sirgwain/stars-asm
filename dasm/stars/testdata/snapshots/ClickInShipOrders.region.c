HCURSOR ClickInShipOrders(POINT16 pt, int16_t sks, int16_t fCursor, int16_t fRightBtn) {
    int32_t    lCur;
    HDC        hdc;
    PLANET     pl;
    int16_t    iWarp;
    POINT16    ptOld;
    int16_t    idPlan;
    int32_t    lMax;
    int32_t    lSel;
    int16_t    iSkip;
    int32_t    xRnd;
    int16_t    grbit;
    XFER       xf;
    int32_t    lNew;
    int16_t    irc;
    int32_t    dx;
    int32_t    lTempMin;
    int16_t    fFirst;
    int16_t    fTwoMAs;
    int32_t    lTempMax;
    int16_t    cMax;
    char       sz255[2];
    int16_t    i;
    char      *rgszZip[11];
    ZIPORDER   rgzo[4];
    FARPROC    lpProc;
    int16_t    fRet;
    TASKXPORT *lptxp;
    int16_t    fSep;
    int16_t    c;
    ORDER     *lpord;
    THING     *lpth;
    FLEET     *lpfl;
    int32_t    rgid[100];
    int16_t    iChecked;
    THING     *lpthMac;
    SCAN       scan;

    lTempMin = 0;
    irc = -1;
    if (sel.grobj == grobjNone) {
        return NULL;
    }
    if (PtInRect(&rgrcRef[5], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        if (fRightBtn != 0) {
            sz255[0] = -1;
            sz255[1] = 0;
            for (i = 0; i < 4; i++) {
                rgszZip[i] = rgszZipOrder[i];
            }
            rgszZip[4] = sz255;
            cMax = 5;
            for (i = 0; i < 4; i++) {
                if (vrgZip[i].fValid != 0) {
                    rgszZip[cMax++] = vrgZip[i].szName;
                }
            }
            if (cMax > 5) {
                rgszZip[cMax++] = sz255;
            }
            rgszZip[cMax++] = PszGetCompressedString(idsCustomize);
            i = PopupMenu(hwndPlanet, pt.x, pt.y, cMax, NULL, rgszZip, -1, 1);
            if (i == cMax - 1) {
                memcpy(rgzo, vrgZip, 96);
                lpProc = MakeProcInstance(ZipOrderDlg, hInst);
                fRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_ZIP_PROD), hwndFrame, lpProc);
                FreeProcInstance(lpProc);
                if (fRet == 0) {
                    memcpy(vrgZip, rgzo, 96);
                }
            } else {
                if (i > 4) {
                    i -= 4;
                    iSkip = 0;
                    while (i != 0) {
                        if (vrgZip[iSkip].fValid != 0) {
                            i--;
                            if (i == 0)
                                break;
                        }
                        iSkip++;
                    }
                    sel.fl.lpplord->rgord[sel.iwpAct].txp = vrgZip[iSkip].txp;
                } else {
                    if (i == -1)
                        goto L_8b82;
                    lptxp = (TASKXPORT *)&sel.fl.lpplord->rgord[sel.iwpAct].txp;
                    switch (i) {
                    case 0:
                        lptxp->rgia[4].iAction = iActionLoadDunnage;
                        for (i = 0; i < 3; i++) {
                            lptxp->rgia[i].iAction = iActionLoadAll;
                        }
                        lptxp->rgia[3].iAction = iActionNone;
                        break;
                    case 1:
                        lptxp->rgia[4].iAction = iActionLoadDunnage;
                        for (i = 0; i <= 3; i++) {
                            lptxp->rgia[i].iAction = iActionUnloadAll;
                        }
                        break;
                    case 2:
                        lptxp->rgia[4].iAction = iActionLoadDunnage;
                        for (i = 0; i < 3; i++) {
                            lptxp->rgia[i].iAction = iActionWaitPercent;
                            lptxp->rgia[i].cQuan = 100;
                        }
                        lptxp->rgia[3].iAction = iActionNone;
                        break;
                    case 3:
                        for (i = 0; i < 5; i++) {
                            lptxp->rgia[i].iAction = iActionNone;
                            lptxp->rgia[i].cQuan = 0;
                        }
                    }
                }
                FLookupFleet(-1, &sel.fl);
                UpdateOrdersDDs(1);
                DrawPlanShip(NULL, tileStarbaseOrWaypoint);
            }
        } else {
            GlobalPD.grPopup = grPopupShipOrders;
            Popup(hwndPlanet, pt.x, pt.y);
        }
    } else if (PtInRect(&rgrcRef[12], PointFrom16(pt)) != 0) {
        if (fCursor != 0) {
            return hcurArrowHelp;
        }
        if (fRightBtn != 0) {
            iChecked = -1;
            lpord = &sel.fl.lpplord->rgord[sel.iwpAct];
            FFindNearestObject(lpord->pt, grobjPlanet | grobjFleet | grobjOther | grobjThing | mdExact, &scan);
            if (scan.idpl != -1) {
                rgid[0] = scan.idpl;
            } else {
                rgid[0] = 268435456;
            }
            rgid[1] = -1;
            c = 2;
            if (lpord->grobj == grobjPlanet || lpord->grobj == grobjOther) {
                iChecked = 0;
            }
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0)
                    break;
                if (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y && lpfl->id != sel.fl.id) {
                    if (lpord->grobj == grobjFleet && lpord->id == lpfl->id) {
                        iChecked = c;
                    }
                    rgid[c++] = lpfl->id | 0x80000000;
                    if (c >= 100)
                        break;
                }
            }
            if (c == 2) {
                c = 1;
            }
            fSep = c == 0 ? 1 : 0;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (scan.pt.x == lpth->pt.x && scan.pt.y == lpth->pt.y) {
                    if (fSep == 0) {
                        if (c >= 100)
                            break;
                        rgid[c++] = -1;
                        fSep = 1;
                    }
                    if (c >= 100)
                        break;
                    rgid[c++] = (uint32_t)(uint16_t)lpth->idFull | 0x20000000;
                }
            }
            i = PopupMenu(hwndPlanet, pt.x, pt.y, c, rgid, NULL, iChecked, 1);
            if (i >= 0) {
                if (i == 0 && rgid[0] == 268435456) {
                    lpord->grobj = grobjOther;
                    lpord->id = -1;
                } else if ((rgid[i] & 0x20000000) != 0) {
                    lpord->grobj = grobjThing;
                    lpord->id = LOWORD(rgid[i]);
                } else if ((rgid[i] & 0x80000000) != 0) {
                    lpord->grobj = grobjFleet;
                    lpord->id = LOWORD(rgid[i]);
                } else {
                    lpord->grobj = grobjPlanet;
                    lpord->id = LOWORD(rgid[0]);
                }
                FLookupFleet(-1, &sel.fl);
                FillOrdersLB();
                SetOrdersLbSel(sel.iwpAct);
            }
        } else {
            GlobalPD.grPopup = grPopupString;
            GlobalPD.dxOut = 180;
            GlobalPD.psz = szPopupBuffer;
            CchGetString(idsRightClickBlueDiamondBringPopupMenu, szPopupBuffer);
            Popup(hwndPlanet, pt.x, pt.y);
        }
    } else {
        if (fRightBtn != 0) {
            return NULL;
        }
        if (PtInRect(rgrcRef, PointFrom16(pt)) != 0) {
            if (sel.grobj != grobjFleet && fCursor != 0) {
                return NULL;
            }
            irc = 0;
            lTempMax = 11;
            lMax = 11;
            lCur = sel.fl.lpplord->rgord[sel.iwpAct].iWarp;
            grbit = 6;
        } else if (PtInRect(&rgrcRef[15], PointFrom16(pt)) != 0) {
            irc = 15;
            iWarp = IWarpMAFromLppl(&sel.pl, &fTwoMAs);
            lTempMax = (int16_t)(iWarp - 1);
            lMax = (int16_t)(iWarp - 1);
            lTempMin = 1;
            lCur = sel.pl.iWarpFling;
        } else if (PtInRect(&rgrcRef[1], PointFrom16(pt)) != 0) {
            if (sel.grobj == grobjFleet) {
                idPlan = sel.fl.idPlanet;
                iSkip = sel.fl.id;
            } else {
                iSkip = -1;
                idPlan = sel.pl.id;
            }
            lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
            FLookupOrbitingXfer(idPlan, LOWORD(lSel), &xf, iSkip);
            if (xf.grobj != grobjFleet || xf.fl.iPlayer != idPlayer) {
                return NULL;
            }
            irc = 1;
            lMax = LGetFleetStat(&xf.fl, 1);
            lCur = xf.fl.rgwtMin[4];
            grbit = 4;
            if (sel.grobj == grobjFleet) {
                lTempMax = lCur + sel.fl.rgwtMin[4];
                lTempMin = lCur - (LGetFleetStat(&sel.fl, 1) - sel.fl.rgwtMin[4]);
            } else {
                lTempMax = lCur;
            }
        } else {
            if (PtInRect(&rgrcRef[3], PointFrom16(pt)) != 0) {
                if (fCursor != 0) {
                    return hcurHand;
                }
                if (sel.fl.idPlanet != -1) {
                    TransferStuff(sel.fl.id, grobjFleet, sel.fl.idPlanet, grobjPlanet, mdXferCargo);
                } else {
                    lpth = lpThings;
                    lpthMac = lpThings + cThing;
                    for (; lpth < lpthMac && (lpth->ith != ithMineralPacket || sel.fl.pt.x != lpth->pt.x || sel.fl.pt.y != lpth->pt.y); lpth++) {
                    }
                    if (lpth == lpthMac) {
                        TransferStuff(sel.fl.id, grobjFleet, -1, grobjOther, mdXferCargo);
                    } else {
                        MessageBeep(MB_OK);
                    }
                }
                return NULL;
            }
            if (PtInRect(&rgrcRef[4], PointFrom16(pt)) != 0) {
                if (fCursor != 0) {
                    return hcurHand;
                }
                lSel = SendMessage(hwndShipDD, CB_GETCURSEL, 0, 0);
                if (lSel == -1) {
                    return NULL;
                }
                if (FLookupOrbitingXfer(sel.pl.id, LOWORD(lSel), &xf, sel.grobj == grobjFleet ? sel.fl.id : -1) != 0) {
                    TransferStuff(sel.id, sel.grobj, xf.id, xf.grobj, mdXferCargo);
                }
                return NULL;
            }
            if (PtInRect(&rgrcRef[18], PointFrom16(pt)) != 0) {
                if ((sel.grobj != grobjFleet && fCursor != 0) || sel.fl.lpplord->rgord[sel.iwpAct].grTask != grTaskPatrol) {
                    return NULL;
                }
                irc = 18;
                lTempMax = 10;
                lMax = 10;
                lCur = (uint32_t)sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX;
                grbit = 7;
            }
        }
    }
L_8b82:
    if (irc == -1) {
        return NULL;
    }
    if (fCursor != 0) {
        return hcurHand;
    }
    dx = (int16_t)(rgrcRef[irc].right - rgrcRef[irc].left - 2);
    xRnd = (int32_t)(dx / (lMax + 1)) >> 1;
    hdc = GetDC(hwndPlanet);
    SetCapture(hwndPlanet);
    ptOld.y = -1;
    ptOld.x = -1;
    lTempMax = lMax < lTempMax ? lMax : lTempMax;
    lTempMin = 0 <= lTempMin ? lTempMin : 0;
    fFirst = 1;
    while (fFirst != 0 || FGetMouseMove(&pt) != 0) {
        fFirst = 0;
        if (pt.x != ptOld.x || pt.y != ptOld.y) {
            ptOld = pt;
            lNew = (int32_t)((int32_t)(((int16_t)(pt.x - rgrcRef[irc].left) + xRnd) * lMax) / dx);
            if (lTempMin > (lNew < lTempMax ? lNew : lTempMax)) {
                lNew = lTempMin;
            } else if (lNew >= lTempMax) {
                lNew = lTempMax;
            }
            if (lNew != lCur) {
                switch (irc) {
                case 0:
                    sel.fl.lpplord->rgord[sel.iwpAct].iWarp = LOWORD(lNew);
                    DrawPlanShip(NULL, tileFleetOrders | tileMinimized);
                    break;
                case 18:
                    sel.fl.lpplord->rgord[sel.iwpAct].tsell.iPlrX = LOWORD(lNew);
                    DrawPlanShip(NULL, tileStarbaseOrWaypoint | tileMinimized);
                    break;
                case 15:
                    DrawMassWarpGauge(hdc, &rgrcRef[15], fTwoMAs == 0 ? iWarp : -iWarp, LOWORD(lNew) + 4);
                    break;
                case 2:
                    sel.fl.rgwtMin[4] = lNew;
                    DrawFleetGauge(hdc, &rgrcRef[irc], NULL, grbit);
                    break;
                case 1:
                    if (sel.grobj == grobjFleet) {
                        sel.fl.rgwtMin[4] -= lNew - lCur;
                        DrawFleetGauge(hdc, &rgrcRef[2], &sel.fl, grbit);
                    } else {
                        DrawPlanShip(NULL, tileMineralsOrCargo | tileMinimized);
                    }
                    xf.fl.rgwtMin[4] = lNew;
                    DrawFleetGauge(hdc, &rgrcRef[irc], &xf.fl, grbit);
                }
                lCur = lNew;
            }
        }
    }
    grbit = sel.scan.grobj == grobjOther ? sel.scan.grobjFull : sel.scan.grobj;
    switch (irc) {
    case 2:
        FLookupFleet(-1, &sel.fl);
        FLookupPlanet(-1, &pl);
        DrawPlanShip(NULL, tileFleetOrders | tileFleetComp | tileMinimized);
        if ((grbit & 1) != 0 && sel.fl.idPlanet == sel.scan.idpl)
            goto FixMinWin;
        if ((grbit & 2) == 0 || sel.fl.id != rglpfl[sel.scan.ifl]->id)
            break;
        InvalidateRect(hwndMine, NULL, 1);
        break;
    case 1:
        FLookupFleet(-1, &xf.fl);
        if (sel.grobj == grobjFleet) {
            FLookupFleet(-1, &sel.fl);
            DrawPlanShip(NULL, tileMineralsOrCargo | tileFleetOrders | tileFleetComp | tileMinimized);
            break;
        }
        FLookupPlanet(-1, &sel.pl);
        if ((grbit & 1) != 0 && sel.pl.id == sel.scan.idpl)
            goto FixMinWin;
        break;
    case 0:
    case 18:
        FLookupFleet(-1, &sel.fl);
        break;
    case 15:
        if (LOWORD(lCur) != sel.pl.iWarpFling) {
            sel.pl.iWarpFling = LOWORD(lCur);
            FLookupPlanet(-1, &sel.pl);
        }
    }
    goto L_914e;
FixMinWin:
    InvalidateMineralBars();
L_914e:
    ReleaseCapture();
    return (HCURSOR)(uintptr_t)ReleaseDC(hwndPlanet, hdc);
}
