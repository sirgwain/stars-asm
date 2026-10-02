void MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks) {
    PLANET    *lppl;
    HtMineType ht;
    PART       part;
    char       rgsz[16][32];
    char      *rgpsz[16];
    int16_t    c;
    FLEET     *lpfl;
    int16_t    rgid[16];
    int16_t    ishdef;
    int16_t    rgMin[3];
    PLANET     pl;
    int16_t    rgCost[3];
    int16_t    rgMax[3];
    int16_t    fOurs;
    int16_t    i;
    int16_t    idNew;
    SCAN       scan;
    int16_t    rgi[9];
    int16_t    iChecked;
    char      *psz[9];
    int32_t    rglQuan[3];
    int32_t    rglT[3];
    int16_t    ifl;
    int32_t    cMines;
    int32_t    lVal;

    ht = HtMineWindow(hwndMine, x, y);
    if ((msg != 516 || ht == htMineScanSel || ht == htMineShipOrFleet) && ht <= htMineMinefieldType) {
        switch (ht) {
        case htMineOwner:
            if (sel.scan.grobj == grobjPlanet) {
                lppl = LpplFromId(sel.scan.idpl);
                GlobalPD.iPlayer = lppl->iPlayer;
            } else if (sel.scan.grobj == grobjThing) {
                GlobalPD.iPlayer = lpThings[sel.scan.ith].iplr;
            } else {
                GlobalPD.iPlayer = rglpfl[sel.scan.ifl]->iplr;
            }
            GlobalPD.grPopup = grPopupPlayer;
            Popup(hwndMine, x, y);
            break;
        case htMineMinefieldType:
            part.hs.iItem = mpiTypeiItem[lpThings[sel.scan.ith].thm.iType];
            part.hs.grhst = hstMines;
            FLookupPart(&part);
            GlobalPD.grPopup = grPopupComponent;
            GlobalPD.part = part;
            Popup(hwndMine, x, y);
            break;
        case htMineShipOrFleet:
            if (msg == 516) {
                lpfl = rglpfl[sel.scan.ifl];
                c = 0;
                for (ishdef = 0; ishdef < 16; ishdef++) {
                    if (lpfl->rgcsh[ishdef] > 0) {
                        rgid[c] = ishdef;
                        fstrcpy(rgsz[c], rglpshdef[lpfl->iPlayer][ishdef].hul.szClass);
                        rgpsz[c] = rgsz[c];
                        c++;
                    }
                }
                if (c > 1) {
                    c = PopupMenu(hwndMine, x, y, c, NULL, rgpsz, -1, 1);
                    if (c == -1)
                        break;
                } else {
                    c = 0;
                }
                GlobalPD.grPopup = grPopupShdef;
                GlobalPD.lpshdef = rglpshdef[lpfl->iPlayer] + rgid[c];
                GlobalPD.fHideCounts = idPlayer != lpfl->iPlayer;
                GlobalPD.fShowDamage = 0;
                GlobalPD.fToken = 0;
                GlobalPD.fSummary = 1;
            } else {
                GlobalPD.grPopup = grPopupFleet;
                GlobalPD.lpfl = rglpfl[sel.scan.ifl];
                GlobalPD.fRedDamage = GlobalPD.lpfl->det == detAll;
                GlobalPD.grbit = 0xff;
            }
            Popup(hwndMine, x, y);
            break;
        case htMineStarbase:
            GlobalPD.grPopup = grPopupShdef;
            lppl = LpplFromId(sel.scan.idpl);
            GlobalPD.lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
            GlobalPD.fHideCounts = idPlayer != lppl->iPlayer;
            GlobalPD.fShowDamage = 1;
            GlobalPD.fToken = 0;
            GlobalPD.fSummary = 1;
            Popup(hwndMine, x, y);
            break;
        case htMinePlanet:
            GlobalPD.grPopup = grPopupPlanet;
            GlobalPD.idPlanet = sel.scan.idpl;
            Popup(hwndMine, x, y);
            break;
        case htMineEnvVar0:
        case htMineEnvVar1:
        case htMineEnvVar2:
            FLookupPlanet(sel.scan.idpl, &pl);
            GlobalPD.grPopup = grPopupPlanetEnv;
            GlobalPD.idPlanet = pl.id;
            GlobalPD.iPlanetVar = ht - 6;
            if (pl.det >= detSome) {
                GlobalPD.iPlanVal = pl.rgEnvVar[GlobalPD.iPlanetVar];
            } else {
                GlobalPD.iPlanVal = -1;
            }
            if (pl.det >= detSome && FCanTerraformLppl(&pl, rgMin, rgMax, rgCost, 1) != 0) {
                GlobalPD.iPlanMin = rgMin[GlobalPD.iPlanetVar];
                GlobalPD.iPlanMax = rgMax[GlobalPD.iPlanetVar];
                if (GlobalPD.iPlanMin == -1) {
                    GlobalPD.iPlanMin = GlobalPD.iPlanVal;
                }
                if (GlobalPD.iPlanMax == -1) {
                    GlobalPD.iPlanMax = GlobalPD.iPlanVal;
                }
                if (GlobalPD.iPlanMin != GlobalPD.iPlanMax)
                    goto L_401c;
            }
            GlobalPD.iPlanMin = -1;
            GlobalPD.iPlanMax = -1;
        L_401c:
            GlobalPD.iPlrVal = rgplr[idPlayer].rgEnvVar[GlobalPD.iPlanetVar];
            GlobalPD.iPlrMin = rgplr[idPlayer].rgEnvVarMin[GlobalPD.iPlanetVar];
            GlobalPD.iPlrMax = rgplr[idPlayer].rgEnvVarMax[GlobalPD.iPlanetVar];
            Popup(hwndMine, x, y);
            break;
        case htMineScanSel:
            if (msg == 516) {
                PopupMineralScanChoices(hwndMine, x, y);
                break;
            }
            scan = sel.scan;
            scan.iwp = 0;
            if (scan.grobj == grobjThing) {
                i = scan.ith + 1;
                goto L_4113;
            }
        L_4192:
            for (i = scan.grobj == grobjFleet ? scan.ifl + 1 : 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y))
                    break;
            }
            if (i < cFleet) {
                scan.ifl = i;
                scan.grobj = grobjFleet;
                idNew = rglpfl[i]->id;
                fOurs = rglpfl[i]->iPlayer == idPlayer;
                goto ChangeIt;
            }
            if ((scan.grobjFull & grobjThing) != 0) {
                i = 0;
                goto L_4113;
            }
            if ((scan.grobjFull & grobjPlanet) != 0)
                goto CheckPlanet;
        CheckFleet:
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || (scan.pt.x == lpfl->pt.x && scan.pt.y == lpfl->pt.y))
                    break;
            }
            if (i != cFleet || (scan.grobjFull & grobjThing) == 0) {
                scan.grobj = grobjFleet;
                scan.ifl = i;
                idNew = rglpfl[i]->id;
                fOurs = rglpfl[i]->iPlayer == idPlayer;
                goto ChangeIt;
            }
            i = 0;
        L_4113:
            while (1) {
                if (i >= cThing || (lpThings[i].pt.x == scan.pt.x && lpThings[i].pt.y == scan.pt.y)) {
                    if (i < cThing)
                        break;
                    if ((scan.grobjFull & grobjPlanet) != 0)
                        goto CheckPlanet;
                    if ((scan.grobjFull & grobjFleet) != 0)
                        goto CheckFleet;
                    if ((scan.grobjFull & grobjThing) == 0)
                        goto L_4192;
                    i = 0;
                } else {
                    i++;
                }
            }
            scan.ith = i;
            scan.grobj = grobjThing;
            idNew = lpThings[i].idFull;
            fOurs = 0;
            goto ChangeIt;
        CheckPlanet:
            scan.grobj = grobjPlanet;
            idNew = scan.idpl;
            lppl = LpplFromId(idNew);
            if (lppl == 0) {
                fOurs = 0;
            } else {
                fOurs = lppl->iPlayer == idPlayer;
            }
        ChangeIt:
            if (fOurs == 0 || sel.grobj != grobjFleet) {
                scan.iwp = sel.scan.iwp;
            }
            ChangeScanSel(&scan, 2);
            if (fOurs == 0)
                break;
            RedrawScanSel(NULL, 0);
            ChangeMainObjSel(scan.grobj, idNew);
            RedrawScanSel(NULL, 1);
            break;
        case htMineScale:
            iChecked = -1;
            rgi[0] = 100;
            rgi[1] = 500;
            rgi[2] = 1000;
            rgi[3] = 2500;
            rgi[4] = 5000;
            rgi[5] = 7500;
            rgi[6] = 10000;
            rgi[7] = 20000;
            rgi[8] = 30000;
            for (i = 0; i < 9; i++) {
                _wsprintf(rgsz[i], PCTDKT, rgi[i]);
                psz[i] = rgsz[i];
                if (rgi[i] == cMinGrafMax) {
                    iChecked = i;
                }
            }
            i = PopupMenu(hwndMine, x, y, 9, NULL, psz, iChecked, 1);
            if (i == -1 || rgi[i] == cMinGrafMax)
                break;
            cMinGrafMax = rgi[i];
            InvalidateRect(hwndMine, NULL, 1);
            if ((grbitScan & grbitScanViewMask) != 1)
                break;
            InvalidateRect(hwndScanner, NULL, 1);
            break;
        case htMineMineralConc1:
        case htMineMineralConc2:
        case htMineMineralConc3:
            FLookupPlanet(sel.scan.idpl, &pl);
            GlobalPD.grPopup = grPopupMineral;
            GlobalPD.rgi[0] = (int16_t)(ht - 1);
            for (i = 1; i <= 4; i++) {
                GlobalPD.rgi[i] = -1;
            }
            if (pl.det >= detSome) {
                GlobalPD.rgi[3] = (uint32_t)pl.rgpctMinLevel[ht + 2];
                GlobalPD.rgi[1] = pl.fHomeworld;
                if (pl.det > detSome) {
                    lVal = 0;
                    GlobalPD.rgi[2] = pl.rgwtMin[ht - 1];
                    EstMineralsMined(&pl, rglQuan, -1, 0);
                    GlobalPD.rgi[4] = rglQuan[ht - 1];
                    if (pl.iPlayer == -1) {
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == pl.id && lpfl->iPlayer == idPlayer && lpfl->fDead == 0 && lpfl->lpplord->rgord[0].grTask == grTaskMine) {
                                cMines = CMineFromLpfl(lpfl);
                                if (cMines > 0) {
                                    EstMineralsMined(&pl, rglT, cMines, 0);
                                    lVal += rglT[ht - 1];
                                }
                            }
                        }
                        if (lVal > 0) {
                            GlobalPD.rgi[4] = lVal;
                        }
                    }
                }
            }
            Popup(hwndMine, x, y);
        }
    }
    return;
}
