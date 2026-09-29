#include "common.h"

int16_t rgpctMineHit[3] = {3, 10, 35};
int16_t rgiWarpSafe[3] = {4, 6, 5};
int16_t rgrgdmgMinMine[3][2] = {{500, 600}, {2000, 2500}};
int16_t rgrgdmgMine[3][2] = {{100, 125}, {500, 600}};

int16_t FGenerateTurn() {
    int16_t  fErrSav;
    char    *pchT;
    int16_t  ish;
    int16_t  j;
    uint8_t  mpiplr2[16];
    uint8_t  rgfNoXFile[16];
    jmp_buf *penvMemSav;
    int16_t  ifl;
    FLEET   *lpfl;
    char    *pchCur;
    int16_t  i;
    jmp_buf  env;
    char     szT[256];
    HCURSOR  hcurSav;
    int16_t  idCur;
    int16_t  fFollow;
    char    *pchBak;
    int16_t  fSuccess;
    int16_t  fDone;
    FLEET   *lpflTarget;
    ORDER    ord;
    int16_t  cAdv;
    PLANET  *lppl;
    PLANET  *lpplMac;
    int16_t  dPlanRange;
    int16_t  dRange;
    int16_t  iSteal;
    int16_t  pctDetect;
    HS      *t_fields_1;
    int16_t  t_call_120d;
    int16_t  t_merge_1670_0001;

    idCur = idPlayer;
    fSuccess = 0;
    hcurSav = SetCursor(LoadCursor(0x0, MAKEINTRESOURCE(0x7f02)));
    DestroyCurGame();
    if (gd.fTutorial != 0x0) {
        Randomize(0x499602d2);
    }
    fErrSav = fFileErrSilent;
    fFileErrSilent = 1;
    UpdateProgressGauge(360);
    if (FLoadGame(szBase, "hst") != 0) {
        TurnLog(idsGeneratingYearD);
        fFileErrSilent = fErrSav;
        if ((wVersFile >> 0xc & 0xf) <= 0x0) {
            for (i = 0; i < game.cPlayer && rgplr[i].iPlrBmp == 0x0; i++) {
            }
            if (i == game.cPlayer) {
                for (i = 0; i < game.cPlayer; i++) {
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xff07) | (i & 0x1f) * 0x8;
                }
            }
        }
        penvMemSav = penvMem;
        penvMem = &env;
        if (setjmp(env) == 0) {
            lpcd = LpAlloc(1000 * sizeof(COLDROP), htMisc);
            lpxf = LpAlloc(1000 * sizeof(XFERFULL), htMisc);
            vrgPlanResExtra = LpAlloc(game.cPlanMax * 2, htMisc);
            fmemset(vrgPlanResExtra, 0, game.cPlanMax * 2);
            vrgts = LpAlloc(game.cPlayer * sizeof(TURNSERIAL), htMisc);
            UpdateProgressGauge(370);
            cColDrop = 0;
            cXferFull = 0;
            gd.fGeneratingTurn = 0x1;
            gd.fRetryOpens = 0x1;
            imemMsgCur = 0;
            for (i = 0; i < game.cPlayer; i++) {
                mpiplr2[i] = LOBYTE(i);
            }
            for (i = 0; i < game.cPlayer; i++) {
                j = Random(game.cPlayer - i) + i;
                if (j != i) {
                    idCur = mpiplr2[j];
                    mpiplr2[j] = mpiplr2[i];
                    mpiplr2[i] = LOBYTE(idCur);
                }
            }
            i = 0;
            while (1) {
                if (i >= game.cPlayer)
                    goto L_03b5;
                j = mpiplr2[i];
                _wsprintf(szWork, "%s.x%d", szBase, j + 1);
                idPlayer = j;
                vrgts[j].lSerialNumber = -1;
                if (FLoadLogFile(szWork) != 0 && FRunLogFile() == 0)
                    break;
                UpdateProgressGauge(MulDiv(60, i + 1, game.cPlayer) + 370);
                i = i + 1;
            }
            AlertSz(PszFormatIds(idsPlayerLogFileAppearsCorruptUnableLoad, 0x0), MB_ICONHAND);
            goto FreeStuffUp;
        L_03b5:
            idPlayer = -1;
            for (i = 0; i < game.cPlayer; i++) {
                if (rgplr[i].fCrippled == 0x0 && rgplr[i].fAi == 0x0 && (gd.fTutorial == 0x0 || i != 0)) {
                    if (vrgts[i].lSerialNumber == -1 || FValidSerialLong(vrgts[i].lSerialNumber) != 0) {
                        if (vrgts[i].lSerialNumber != -1) {
                            rgplr[i].wFlags = rgplr[i].wFlags & 0xfffb;
                            for (j = 0; j < i; j++) {
                                if (rgplr[j].fCrippled == 0x0 && rgplr[j].fAi == 0x0 && vrgts[i].lSerialNumber == vrgts[j].lSerialNumber &&
                                    fmemcmp(vrgts[i].rgbConfig, vrgts[j].rgbConfig, 0xb) != 0) {
                                    rgplr[j].wFlags = (rgplr[j].wFlags & 0xfffb) | 0x4;
                                    rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffb) | (0x1 & 0x1) * 0x4;
                                }
                            }
                        }
                    } else {
                        rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffb) | 0x4;
                    }
                } else {
                    rgplr[i].wFlags = rgplr[i].wFlags & 0xfffb;
                }
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (rgplr[i].fCheater != 0x0) {
                    j = IPlrAlsoCheater(i);
                    FSendPlrMsg2(i, (j == -1 ? 0 : 1) + 256, -5, j, 0);
                    if (game.turn > 0xa && (game.turn & 0x7) == (i & 0x7)) {
                        FSendPlrMsg2(i, 259, -5, 0, 0);
                    }
                }
            }
            for (i = 0; i < game.cPlayer; i++) {
                for (ish = 0; ish < 16; ish++) {
                    if (rglpshdef[i][ish].fFree == 0x0 && rglpshdef[i][ish].hul.rghs[0].grhst != hstEngine) {
                        rglpshdef[i][ish].hul.rghs[0].grhst = hstEngine;
                        rglpshdef[i][ish].hul.rghs[0].iItem = 0x1;
                        if (rglpshdef[i][ish].hul.rghs[0].cItem < 0x1) {
                            t_fields_1 = rglpshdef[i][ish].hul.rghs;
                            t_fields_1->cItem = 0x1;
                        }
                    }
                }
            }
            fFollow = 0;
            for (ifl = 0; ifl < cFleet; ifl++) {
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    break;
                lpfl->fNoHeal = 0x0;
                if (lpfl->cord != 1 || lpfl->lpplord->rgord[0].grobj != grobjFleet) {
                    if (lpfl->lpplord->rgord[0].grobj == grobjFleet && lpfl->cord == 1) {
                        FSendPlrMsg(lpfl->iPlayer, 312, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                    }
                    lpfl->fMark = 0x0;
                } else {
                    fFollow = 1;
                    lpfl->fMark = 0x1;
                }
            }
            ValidateWaypoints();
            if (fFollow != 0) {
                fFollow = 1;
                for (i = 0; i < 8 && fFollow != 0; i++) {
                    fFollow = 0;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0x0)
                            break;
                        if (lpfl->fMark != 0x0 && lpfl->cord == 1) {
                            ord = lpfl->lpplord->rgord[0];
                            if (ord.grobj == grobjFleet) {
                                lpflTarget = LpflFromId(ord.id);
                                if (lpflTarget != 0x0 && (lpflTarget->cord != 1 || lpflTarget->lpplord->rgord[0].grobj == grobjFleet)) {
                                    if (lpflTarget->cord == 1)
                                        continue;
                                    fFollow = 1;
                                    if (lpfl->lpplord->iordMax <= 0x1) {
                                        lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, 0x2);
                                    }
                                    lpfl->lpplord->rgord[1] = lpflTarget->lpplord->rgord[1];
                                    lpfl->lpplord->rgord[1].txp = lpfl->lpplord->rgord[0].txp;
                                    lpfl->cord = 2;
                                    lpfl->lpplord->iordMac = 0x2;
                                    continue;
                                }
                                FSendPlrMsg(lpfl->iPlayer, 312, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                            }
                            lpfl->fMark = 0x0;
                        }
                    }
                }
            }
            UpdateProgressGauge(440);
            DoOrders(0);
            UpdateProgressGauge(530);
            for (i = 0; i < game.cPlayer; i++) {
                for (j = 0; j < 16; j++) {
                    SetRaceStat(&rgplr[i], j, GetRaceStat(&rgplr[i], j));
                }
                if ((int16_t)rgplr[i].pctResearch < 0 || (int16_t)rgplr[i].pctResearch > 100) {
                    rgplr[i].pctResearch = 15;
                }
                if ((int16_t)rgplr[i].pctIdealGrowth < 0) {
                    rgplr[i].pctIdealGrowth = 1;
                }
                if ((int16_t)rgplr[i].pctIdealGrowth > 20) {
                    rgplr[i].pctIdealGrowth = 20;
                }
                j = rgplr[i].fHacker;
                cAdv = CAdvantagePoints(&rgplr[i]);
                if ((cAdv < 0 || j != rgplr[i].fHacker) && rgplr[i].fAi == 0x0) {
                    FSendPlrMsg2(i, 279, -1, 0, 0);
                    for (j = 0; j < game.cPlayer; j++) {
                        if (i != j && rgplr[i].fAi == 0x0) {
                            FSendPlrMsg2(j, 386, -1, i, 0);
                        }
                    }
                    rgplr[i].wFlags = (rgplr[i].wFlags & 0xffef) | 0x10;
                    if (cAdv < 500) {
                        while ((int16_t)rgplr[i].rgAttr[0] < 25) {
                            rgplr[i].rgAttr[0] = rgplr[i].rgAttr[0] + 1;
                            cAdv = CAdvantagePoints(&rgplr[i]);
                            if (cAdv >= 500)
                                break;
                        }
                    }
                    if (cAdv < 500) {
                        while ((int16_t)rgplr[i].pctIdealGrowth > 1) {
                            rgplr[i].pctIdealGrowth = rgplr[i].pctIdealGrowth - 1;
                            cAdv = CAdvantagePoints(&rgplr[i]);
                            if (cAdv >= 500)
                                break;
                        }
                    }
                    if (cAdv < 500) {
                        for (j = 8; j <= 13; j++) {
                            rgplr[i].rgAttr[j] = 0;
                            cAdv = CAdvantagePoints(&rgplr[i]);
                            if (cAdv >= 500)
                                break;
                        }
                    }
                }
            }
            UnmarkMineFields();
            MoveThings(0);
            UpdateProgressGauge(550);
            MoveFleets();
            lppl = lpPlanets;
            lpplMac = lpPlanets + cPlanet;
            for (; lppl < lpplMac; lppl++) {
                lppl->fHomeworld = 0x0;
            }
            for (i = 0; i < game.cPlayer; i++) {
                lpPlanets[rgplr[i].idPlanetHome].fHomeworld = 0x1;
            }
            UpdateProgressGauge(650);
            ThingDecay();
            BreedColonistsInTransit();
            UpdateProgressGauge(700);
            Produce();
            UpdateProgressGauge(750);
            MoveThings(1);
            UpdateProgressGauge(770);
            FuelFleets();
            DoOrders(1);
            SweepForMines();
            HealShips();
            AutoTerraform();
            RemoteTerraforming();
            UpdateProgressGauge(850);
            SpankTheCheaters();
            ValidateWaypoints();
            UpdateGuesses();
            UpdateProgressGauge(852);
            FMarkFile(dtHost, -1, 1, 0);
            CreateBackupDir();
            game.turn = game.turn + 0x1;
            pchCur = &szBase[strlen(szBase)];
            pchT = strrchr(szBase, 92);
            strcpy(szT, szBackup);
            if (pchT != 0x0) {
                strcat(szT, pchT + 1);
            } else {
                strcat(szT, szBase);
            }
            pchBak = &szT[strlen(szT)];
            UpdateProgressGauge(854);
            UpdatePlayerScores();
            for (i = 0; i < game.cPlayer; i++) {
                for (j = 0; j < 10; j++) {
                    if (rglpshdefSB[i][j].fFree == 0x0) {
                        t_call_120d = PctCloakFromHuldef(&rglpshdefSB[i][j].hul, i, 0x0);
                        rglpshdefSB[i][j].lVisible = (int32_t)(100 - t_call_120d);
                        rglpshdefSB[i][j].lVisible = (uint32_t)(rglpshdefSB[i][j].lVisible * rglpshdefSB[i][j].lVisible);
                    }
                }
                for (j = 0; j < 16; j++) {
                    if (rglpshdef[i][j].fFree == 0x0) {
                        dRange = GetShdefScannerRange(rglpshdef[i] + j, i, &dPlanRange, &pctDetect, &iSteal);
                        rglpshdef[i][j].dScanRange = dRange;
                        rglpshdef[i][j].dScanRange2 = dPlanRange;
                        rglpshdef[i][j].pctDetect = LOBYTE(pctDetect);
                        rglpshdef[i][j].iSteal = LOBYTE(iSteal);
                        if (FCanBuildShdef(rglpshdef[i] + j, i) == 0) {
                            rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0x7fff) | 0x8000;
                        }
                    }
                }
            }
            j = 856;
            fDone = 0;
            memset(rgfNoXFile, 0, 0x10);
            i = 0;
            while (fDone == 0) {
                UpdateProgressGauge(j);
                j = j + 17 / (game.cPlayer + 1);
                if (i >= game.cPlayer) {
                    i = -1;
                    fDone = 1;
                }
                if (i < 0) {
                    strcpy(pchCur, ".hst");
                    strcpy(pchBak, ".hst");
                } else {
                    _wsprintf(pchCur, ".x%d", i + 1);
                    strcpy(pchBak, pchCur);
                    remove(szT);
                    if (access(szBase, 0) != -1) {
                        rename(szBase, szT);
                    } else {
                        rgfNoXFile[i] = 0x1;
                    }
                    pchBak[1] = 'm';
                    pchCur[1] = 'm';
                }
                remove(szT);
                if (i < 0 || rgfNoXFile[i] == 0x0) {
                    rename(szBase, szT);
                } else {
                    StarsCopyFile(szT, szBase);
                }
                *pchCur = 0;
                i = i + 1;
            }
            j = 875;
            fDone = 0;
            game.wGen = (uint16_t)Random(8);
            i = 0;
            while (fDone == 0) {
                UpdateProgressGauge(j);
                j = j + 122 / (game.cPlayer + 1);
                if (i >= game.cPlayer) {
                    i = -1;
                    fDone = 1;
                }
                if (i == -1 || rgfNoXFile[i] == 0x0) {
                    t_merge_1670_0001 = 0;
                } else {
                    t_merge_1670_0001 = 1;
                }
                FWriteDataFile(szBase, i, t_merge_1670_0001);
                i = i + 1;
            }
            UpdateProgressGauge(998);
            imemLogCur = 0;
            fSuccess = 1;
        }
    FreeStuffUp:
        UpdateProgressGauge(1000);
        FreeLp(vrgPlanResExtra, htMisc);
        vrgPlanResExtra = 0x0;
        FreeLp(vrgts, htMisc);
        vrgts = 0x0;
        FreeLp(lpcd, htMisc);
        lpcd = 0x0;
        FreeLp(lpxf, htMisc);
        lpxf = 0x0;
        gd.fGeneratingTurn = 0x0;
        gd.fRetryOpens = 0x0;
        idPlayer = -1;
        if (fSuccess != 0 && ini.fGen != 0x0) {
            vretExitValue = 1;
        }
        SetCursor(hcurSav);
        TurnLog(fSuccess + 1380);
        return fSuccess;
    }
    fFileErrSilent = fErrSav;
    SetCursor(hcurSav);
    TurnLog(idsCantFindHostFile);
    return 0;
}

void DoOrders(int16_t fPostMovement) {
    PLANET *lppl;
    PLANET *lpplMac;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        lppl->fWasInhabited = lppl->iPlayer == -1 ? 0x0 : 0x1;
    }
    if (fPostMovement != 0) {
        idBattle = (game.turn & 0xf) * 0x100 + 0x1;
        DoBattles(fPostMovement);
    }
    DoThingInteractions(fPostMovement);
    if (fPostMovement != 0) {
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            lppl->turn = 0;
        }
    }
    SatisfyOrders(fPostMovement == 0 ? 1 : 3);
    DropColonists();
    UpdateResearchStatus(0);
    SatisfyOrders(fPostMovement == 0 ? 2 : 4);
    if (fPostMovement == 0) {
        TransferToOthers();
    }
    return;
}

void MoveThings(int16_t fPostProd) {
    int16_t   k;
    int16_t   dUni;
    double    d;
    POINT16   pt;
    int16_t   iMax;
    POINT16   ptDst;
    int16_t   dLeft;
    THING    *lpth;
    int16_t   fAnythingMoved;
    int16_t   fMajorMove;
    MessageId idm;
    int16_t   iLow;
    POINT16   ptSrc;
    THING    *lpthMac;
    int16_t   dRange;
    POINT16   ptBase;
    int16_t   iX;
    int16_t   rgC[2];
    int16_t   rgwtTerra[3];
    int32_t   wtTot;
    int16_t   iWarp2;
    int16_t   iWarp;
    int16_t   fTerra;
    PLANET   *lppl;
    int16_t   wtCur;
    int16_t   pctMinKeep;
    int16_t   fTwoMAs;
    int32_t   lDefKilled;
    int32_t   lColKilled;
    int16_t   i;
    int16_t   pctCaught;
    float     pct;
    int32_t   dmgRaw;
    int16_t   iWarpPacket;
    int16_t   iWarpPacket2;
    THING    *lpth2;
    THING    *lpth2Mac;
    int16_t   pctRate;
    int16_t   iplr;
    int16_t   rgMin[3];
    int16_t   cTerraPerm;
    int16_t   cTerraTemp;
    int16_t   rgMax[3];
    int16_t   rgCost[3];
    double    dyRound;
    double    dxRound;
    double    r;
    int16_t   t_scratch_m36;
    int16_t   t_19fb;
    uint16_t  t_merge_2b4d_0001;

    fAnythingMoved = 0;
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith != ithWormhole || fPostProd == 0) {
            if (lpth->ith == ithMysteryTrader && fPostProd == 0) {
                dRange = lpth->tht.iWarp;
                if (dRange >= 13 || Random(25) != 0)
                    goto L_1c3e;
                idm = idmMysteryTraderHasUnexplicablyChangedHisCourse;
                if (Random(3) != 0)
                    goto LSpeedUpOnly;
                goto LRetargetFreighter;
            }
            if (lpth->ith != ithMineralPacket || lpth->thp.iWarp == 0x0 || (fPostProd != 0 && lpth->thp.fMoved != 0x0))
                continue;
            if (lpth->thp.rgwtMin[0] == 0 && lpth->thp.rgwtMin[1] == 0 && lpth->thp.rgwtMin[2] == 0)
                goto LFreeThePacket;
            lpth->thp.fMoved = 0x1;
            fAnythingMoved = 1;
            dRange = lpth->thp.iWarp + 4;
            dRange = dRange * dRange;
            if (fPostProd != 0) {
                dRange = dRange >> 0x1;
            }
            ptDst = rgptPlan[lpth->thp.idPlanet];
        MoveTh:
            ptSrc = lpth->pt;
            d = DGetDistance(ptSrc.x, ptSrc.y, ptDst.x, ptDst.y);
            dLeft = LOWORD((int32_t)d);
            if (dLeft > dRange) {
                dxRound = ptDst.x <= ptSrc.x ? -0.5 : 0.5;
                dyRound = ptDst.y <= ptSrc.y ? -0.5 : 0.5;
                if (d > 0.0001 || d < -0.0001) {
                    r = (double)(int32_t)dRange / d;
                    ptSrc.x = LOWORD((int32_t)(r * (double)(int32_t)(ptDst.x - ptSrc.x) + dxRound)) + ptSrc.x;
                    ptSrc.y = LOWORD((int32_t)(r * (double)(int32_t)(ptDst.y - ptSrc.y) + dyRound)) + ptSrc.y;
                    if (ptSrc.x == ptDst.x && ptSrc.y == ptDst.y)
                        goto MadeItThere;
                    lpth->pt = ptSrc;
                }
                if (fPostProd != 0 && lpth->ith == ithMineralPacket && FPacketDecay(lpth, 50) != 0)
                    goto LPacketAlreadyFreed;
                continue;
            }
        MadeItThere:
            if (lpth->ith != ithMysteryTrader) {
                if (lpth->ith == ithMineralPacket) {
                    pctRate = MulDiv(dLeft, 100, dRange);
                    if (pctRate >= 0) {
                        if (pctRate > 100) {
                            pctRate = 100;
                        }
                    } else {
                        pctRate = 0;
                    }
                    if (fPostProd != 0) {
                        pctRate = pctRate >> 0x1;
                    }
                    if (FPacketDecay(lpth, pctRate) != 0)
                        goto LPacketAlreadyFreed;
                }
                lppl = lpPlanets + lpth->thp.idPlanet;
                iWarpPacket = lpth->thp.iWarp + 4;
                iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                if (fTwoMAs != 0) {
                    iWarp = iWarp + 1;
                }
                if (iWarp > 0 && GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMassAccel) {
                    rglpshdefSB[lppl->iPlayer][lppl->isb].grbitPlr = rglpshdefSB[lppl->iPlayer][lppl->isb].grbitPlr | 0x1 << lpth->iplr;
                }
                fTerra = GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMassAccel ? 1 : 0;
                iWarp2 = iWarp * iWarp;
                iWarpPacket2 = iWarpPacket * iWarpPacket;
                if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raStargate) {
                    iWarp2 = (int32_t)iWarp2 / 2;
                }
                if (iWarp2 < iWarpPacket2) {
                    if (iWarp <= 0) {
                        pctCaught = 0;
                    } else {
                        iWarp = iWarp2;
                        pctCaught = LOWORD((int32_t)((int32_t)((int32_t)iWarp * 1000) / (int32_t)iWarpPacket2));
                    }
                } else {
                    pctCaught = 1000;
                }
                pctMinKeep = 1000 - pctCaught;
                for (i = 0; i < 3; i++) {
                    rgwtTerra[i] = LOWORD((int32_t)((int32_t)((int32_t)lpth->thp.rgwtMin[i] * (int32_t)pctMinKeep) / 0x3e8));
                }
                pctMinKeep = (int32_t)(1000 - pctCaught) / 9 + pctCaught;
                wtTot = 0;
                for (i = 0; i < 3; i++) {
                    if (lpth->thp.rgwtMin[i] < 0) {
                        lpth->thp.rgwtMin[i] = 0;
                    }
                    wtTot = wtTot + (int32_t)lpth->thp.rgwtMin[i];
                    lppl->rgwtMin[i] = lppl->rgwtMin[i] + (int32_t)((int32_t)((int32_t)lpth->thp.rgwtMin[i] * (int32_t)pctMinKeep) / 0x3e8);
                }
                if (pctCaught != 1000) {
                    dmgRaw = (int32_t)((int32_t)((int32_t)(iWarpPacket * iWarpPacket - iWarp) * wtTot) / 0xa0);
                    if (fTerra != 0) {
                        iplr = lpth->iplr;
                        for (i = 0; i < 3; i++) {
                            cTerraTemp = 0;
                            cTerraPerm = 0;
                            while (rgwtTerra[i] > 0) {
                                wtCur = rgwtTerra[i] >= 100 ? 100 : rgwtTerra[i];
                                if (Random(200) < wtCur) {
                                    cTerraTemp = cTerraTemp + 1;
                                    if (Random(10) == 0) {
                                        cTerraPerm = cTerraPerm + 1;
                                    }
                                }
                                rgwtTerra[i] = rgwtTerra[i] - 100;
                            }
                            if (cTerraPerm > 0) {
                                if ((int16_t)rgplr[iplr].rgEnvVarMin[i] >= 0) {
                                    if ((int16_t)lppl->rgEnvVarOrig[i] >= (int16_t)rgplr[iplr].rgEnvVar[i]) {
                                        if ((int16_t)lppl->rgEnvVarOrig[i] <= (int16_t)rgplr[iplr].rgEnvVar[i]) {
                                            cTerraPerm = 0;
                                        } else if ((int16_t)lppl->rgEnvVarOrig[i] - cTerraPerm >= (int16_t)rgplr[iplr].rgEnvVar[i]) {
                                            cTerraPerm = -cTerraPerm;
                                        } else {
                                            cTerraPerm = (int16_t)rgplr[iplr].rgEnvVar[i] - (int16_t)lppl->rgEnvVarOrig[i];
                                        }
                                    } else if ((int16_t)lppl->rgEnvVarOrig[i] + cTerraPerm > (int16_t)rgplr[iplr].rgEnvVar[i]) {
                                        cTerraPerm = (int16_t)rgplr[iplr].rgEnvVar[i] - (int16_t)lppl->rgEnvVarOrig[i];
                                    }
                                } else if ((int16_t)lppl->rgEnvVarOrig[i] >= 50) {
                                    cTerraPerm = cTerraPerm >= 99 - (int16_t)lppl->rgEnvVarOrig[i] ? 99 - (int16_t)lppl->rgEnvVarOrig[i] : cTerraPerm;
                                } else {
                                    cTerraPerm = -(cTerraPerm >= (int16_t)lppl->rgEnvVarOrig[i] - 1 ? (int16_t)lppl->rgEnvVarOrig[i] - 1 : cTerraPerm);
                                }
                                if (cTerraPerm != 0) {
                                    FSendPlrMsg(iplr, 305, lppl->id, cTerraPerm <= 0 ? 0 : 1, i, lppl->id, abs(cTerraPerm), 0, 0, 0);
                                    if (lppl->iPlayer != -1 && lppl->iPlayer != iplr) {
                                        FSendPlrMsg(iplr, 306, lppl->id, cTerraPerm <= 0 ? 0 : 1, i, lppl->id, abs(cTerraPerm), 0, 0, 0);
                                    }
                                    lppl->rgEnvVarOrig[i] = lppl->rgEnvVarOrig[i] + LOBYTE(cTerraPerm);
                                }
                            }
                            if (cTerraTemp > 0) {
                                idPlayer = iplr;
                                if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) != 0) {
                                    idPlayer = -1;
                                    if ((int16_t)rgplr[iplr].rgEnvVarMin[i] >= 0) {
                                        if (rgMin[i] == -1) {
                                            if (rgMax[i] == -1) {
                                                cTerraTemp = 0;
                                            } else if (cTerraTemp > rgMax[i] - (int16_t)lppl->rgEnvVar[i]) {
                                                cTerraTemp = rgMax[i] - (int16_t)lppl->rgEnvVar[i];
                                            }
                                        } else if (cTerraTemp <= (int16_t)lppl->rgEnvVar[i] - rgMin[i]) {
                                            cTerraTemp = -cTerraTemp;
                                        } else {
                                            cTerraTemp = rgMin[i] - (int16_t)lppl->rgEnvVar[i];
                                        }
                                    } else {
                                        cTerraTemp = (int32_t)cTerraTemp / 2;
                                        if ((int16_t)lppl->rgEnvVarOrig[i] >= 50) {
                                            cTerraTemp = cTerraTemp >= 99 - (int16_t)lppl->rgEnvVar[i] ? 99 - (int16_t)lppl->rgEnvVar[i] : cTerraTemp;
                                        } else {
                                            cTerraTemp = -(cTerraTemp >= (int16_t)lppl->rgEnvVar[i] - 1 ? (int16_t)lppl->rgEnvVar[i] - 1 : cTerraTemp);
                                        }
                                    }
                                    if (cTerraTemp != 0) {
                                        lppl->rgEnvVar[i] = lppl->rgEnvVar[i] + LOBYTE(cTerraTemp);
                                        FSendPlrMsg(iplr, 307, lppl->id, cTerraTemp <= 0 ? 0 : 1, i, lppl->id, i << 0x8 | (int16_t)lppl->rgEnvVar[i], 0, 0, 0);
                                        if (lppl->iPlayer != -1 && lppl->iPlayer != iplr) {
                                            FSendPlrMsg(iplr, 308, lppl->id, cTerraTemp <= 0 ? 0 : 1, i, lppl->id, i << 0x8 | (int16_t)lppl->rgEnvVar[i], 0, 0,
                                                        0);
                                        }
                                    }
                                } else {
                                    idPlayer = -1;
                                }
                            }
                        }
                    }
                    if (lppl->iPlayer == -1)
                        goto LFreeThePacket;
                    CalcPctSurvive(lppl, &pct, 0x0);
                    dmgRaw = (int32_t)(pct * (double)dmgRaw);
                    if (dmgRaw != 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                        lColKilled = lppl->rgwtMin[3];
                        if (lColKilled != 0) {
                            lColKilled = (int32_t)((int32_t)(lColKilled * dmgRaw) / 1000);
                            if (lColKilled < dmgRaw) {
                                lColKilled = dmgRaw;
                            }
                            if (lppl->rgwtMin[3] > 0 && (lColKilled >= lppl->rgwtMin[3] || lColKilled < 0)) {
                                FSendPlrMsg2(lppl->iPlayer, 218, lppl->id, lppl->id, lpth->iplr);
                                UninhabitPlanet(lppl);
                                goto LFreeThePacket;
                            }
                            lDefKilled = (int32_t)((int32_t)(lppl->cDefenses * dmgRaw) / 0x3e8);
                            if (lDefKilled == 0 && lppl->cDefenses != 0x0) {
                                t_merge_2b4d_0001 = (int32_t)Random(20) < dmgRaw ? 0x1 : 0x0;
                                lDefKilled = (uint32_t)t_merge_2b4d_0001;
                            }
                            if (lDefKilled < (int32_t)(dmgRaw / 20)) {
                                lDefKilled = (int32_t)(dmgRaw / 20);
                            }
                            if (lDefKilled > lppl->cDefenses) {
                                lDefKilled = lppl->cDefenses;
                            }
                            if (lDefKilled != 0) {
                                idm = iWarp == 0 ? idmBombardedKtMineralPacketColonistsDefensesDestroy : idmMassAcceleratorPartiallySuccessfullyCapturingKtM2;
                                FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, LOWORD(wtTot), HIWORD(wtTot), lpth->iplr, LOWORD(lColKilled),
                                            LOWORD(lDefKilled), 0);
                                lppl->cDefenses = lppl->cDefenses - LOWORD(lDefKilled);
                            } else {
                                idm = iWarp == 0 ? idmBombardedKtMineralPacketColonistsKilledCollision : idmMassAcceleratorPartiallySuccessfullyCapturingKtM;
                                FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, LOWORD(wtTot), HIWORD(wtTot), lpth->iplr, LOWORD(lColKilled), 0, 0);
                            }
                        } else {
                            FSendPlrMsg2(lppl->iPlayer, 385, lppl->id, lppl->id, lpth->iplr);
                            lDefKilled = lppl->cDefenses;
                            lColKilled = 0;
                        }
                        lppl->rgwtMin[3] = lppl->rgwtMin[3] - lColKilled;
                        goto LFreeThePacket;
                    }
                }
                FSendPlrMsg(lppl->iPlayer, iWarp <= 0 ? 326 : 213, lppl->id, lppl->id, lpth->iplr, LOWORD(wtTot), HIWORD(wtTot), 0, 0, 0);
                goto LFreeThePacket;
            }
            lpth2 = lpThings;
            lpth2Mac = lpThings + cThing;
            for (; lpth2 < lpth2Mac && (lpth2->ith != ithMysteryTrader || lpth2 == lpth); lpth2++) {
            }
            if (lpth2 != lpth2Mac || Random(2) == 0)
                goto LFreeThePacket;
            lpth->pt = lpth->tht.ptDest;
            dRange = lpth->tht.iWarp - 2;
            if (dRange < 6) {
                dRange = 6;
            }
            idm = idmMysteryTraderHasDecidedMakeAnotherPass;
        LRetargetFreighter:
            if (Random(2) != 0) {
                rgC[0] = 1020;
            } else {
                rgC[0] = 400 * game.mdSize + 1380;
            }
            rgC[1] = Random(400 * game.mdSize + 361) + 1020;
            iX = Random(2);
            lpth->tht.ptDest.x = rgC[iX];
            lpth->tht.ptDest.y = rgC[iX == 0 ? 1 : 0];
        LSpeedUpOnly:
            dRange = dRange + 1;
            lpth->tht.iWarp = dRange;
            for (k = 0; k < game.cPlayer; k++) {
                FSendPlrMsg2(k, idm, -6, lpth->idFull, 0);
            }
        L_1c3e:
            dRange = lpth->tht.iWarp;
            dRange = dRange * dRange;
            ptDst = lpth->tht.ptDest;
            fAnythingMoved = 1;
            if (idm == idmMysteryTraderHasDecidedMakeAnotherPass)
                continue;
            goto MoveTh;
        LFreeThePacket:
            FreeLpth(lpth);
        LPacketAlreadyFreed:
            lpth = lpth - 1;
            lpthMac = lpthMac - 1;
        } else {
            k = 0;
            ptBase = lpth->pt;
            t_scratch_m36 = Random(100);
            fMajorMove = t_scratch_m36 >= PctWormholeMoves(lpth) ? 0 : 1;
            if (fMajorMove == 0) {
                lpth->thw.cLastMove = lpth->thw.cLastMove + 0x1;
            } else {
                lpth->thw.grbitPlr = 0x0;
                dUni = 400 * game.mdSize + 400;
                lpth->thw.cLastMove = 0x0;
            }
            iMax = 16;
            while (1) {
                t_19fb = k;
                k = k + 1;
                if (t_19fb >= 100)
                    break;
                if (fMajorMove == 0) {
                    lpth->pt.x = Random(25) + ptBase.x - 12;
                    lpth->pt.y = Random(25) + ptBase.y - 12;
                } else {
                    lpth->pt.x = Random(dUni) + 1000;
                    lpth->pt.y = Random(dUni) + 1000;
                }
                if (lpth->pt.x != ptBase.x || lpth->pt.y != ptBase.y) {
                    iLow = IValidateWormholePos(lpth);
                    if (iLow == 0)
                        break;
                    if (iLow < iMax) {
                        iMax = iLow;
                        pt = lpth->pt;
                    }
                }
            }
            if (iLow != 0) {
                lpth->pt = pt;
            }
        }
    }
    if (fAnythingMoved != 0) {
        ValidateWaypoints();
    }
    return;
}

void FuelFleets() {
    int16_t j;
    int32_t cPods;
    PLANET *lppl;
    int16_t i;
    int16_t ifl;
    FLEET  *lpfl;
    SHDEF  *lpshdef;
    int32_t csh;
    HUL    *lphul;
    int32_t t_call_321d;
    int32_t t_call_3276;
    int32_t t_merge_32b9_0001;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->fDead == 0x0) {
            if (lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].fStarbase != 0x0) {
                lppl = lpPlanets + lpfl->idPlanet;
                if (lppl->iPlayer != -1 && ((lpfl->iPlayer == lppl->iPlayer || (int16_t)rgplr[lppl->iPlayer].rgmdRelation[lpfl->iPlayer] == 1) &&
                                            LphuldefFromId(rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef)->hul.wtCargoMax != 0x0)) {
                    lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
                    continue;
                }
            }
            csh = 0;
            cPods = 0;
            for (i = 0; i < 16; i++) {
                if (lpfl->rgcsh[i] != 0) {
                    lphul = &rglpshdef[lpfl->iPlayer][i].hul;
                    for (j = lphul->chs - 1; j >= 0; j--) {
                        if (lphul->rghs[j].grhst == hstSpecialE && lphul->rghs[j].iItem == 0x10) {
                            cPods = cPods + (uint32_t)((int32_t)lpfl->rgcsh[i] * lphul->rghs[j].cItem);
                        }
                    }
                    lpshdef = rglpshdef[lpfl->iPlayer] + i;
                    if (lpshdef->hul.ihuldef == ihuldefFuelTransport || lpshdef->hul.ihuldef == ihuldefSuperFuelXport) {
                        csh = csh + (uint32_t)((int32_t)lpfl->rgcsh[i] * 200);
                    }
                }
            }
            if (csh != 0 || cPods != 0) {
                t_call_321d = LGetFleetStat(lpfl, 1);
                if ((int32_t)t_call_321d < lpfl->rgwtMin[4] + csh + (uint32_t)(cPods * 50)) {
                    t_call_3276 = LGetFleetStat(lpfl, 1);
                    t_merge_32b9_0001 = t_call_3276;
                } else {
                    t_merge_32b9_0001 = lpfl->rgwtMin[4] + csh + (uint32_t)(cPods * 50);
                }
                lpfl->rgwtMin[4] = t_merge_32b9_0001;
            }
        }
    }
    return;
}

void MoveFleets() {
    int32_t  dTravel;
    int16_t  cPass;
    int32_t  wtFuel2Dest;
    double   d;
    int16_t  fGotEnufFuel;
    int16_t  fRanOutOfFuel;
    ORDER   *lpord;
    POINT16  ptEnd;
    int16_t  ifl;
    FLEET   *lpfl;
    double   r;
    int32_t  pct;
    int16_t  dMineTravel;
    int32_t  dRange;
    POINT16  ptBeg;
    int32_t  wtFuelUsed;
    int32_t  dActTravel;
    int32_t  lFuelGain;
    int16_t  fDone;
    SCAN     scan;
    PLANET  *lpplDst;
    int32_t  wtColonists;
    int16_t  i;
    PLANET  *lpplSrc;
    int16_t  fJumpgate;
    int16_t  isbsDst;
    int16_t  isbsSrc;
    POINT16  ptMsg;
    int32_t  wtMinerals;
    int32_t  cDie;
    int16_t  cKill;
    int16_t  ish;
    FLEET    flSrc;
    int16_t  cTry;
    FLEET    flDead;
    int16_t  cKillTot;
    int16_t  fDead;
    int16_t  dy;
    int16_t  dx;
    int32_t  lFuelGainAct;
    double   dyRound;
    double   dxRound;
    int16_t  iCtr;
    THING   *lpthDest;
    THING   *lpth;
    int16_t  grbitPlr;
    int16_t  t_merge_3de5_0001;
    int32_t  t_merge_3e66_0001;
    int16_t  t_40c6;
    int32_t  t_merge_4403_0001;
    int32_t  t_merge_450b_0001;
    int16_t  t_merge_47c3_0001;
    int32_t  t_merge_48a1_0001;
    int32_t  t_call_48bc;
    uint16_t t_merge_4c51_0001;
    int32_t  t_merge_4ca3_0001;
    int32_t  t_call_4e92;
    int16_t  t_4f1e;

    cPass = 0;
    if (cFleet > 0) {
        do {
            fDone = 1;
            for (ifl = 0; ifl < cFleet; ifl++) {
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    break;
                if (cPass == 0) {
                    lpfl->dirLong = 0;
                    lpfl->fHereAllTurn = 0x1;
                }
                if (lpfl->fDead == 0x0 && (cPass <= 0 || lpfl->fDone == 0x0)) {
                    lpfl->fDone = 0x1;
                    lpord = lpfl->lpplord->rgord;
                    if (lpord->grTask != grTaskXfer && lpord->grTask != grTaskLayMines && lpfl->cord > 1 && lpord[1].iWarp != 0x0) {
                        if (rgplr[lpfl->iPlayer].fCheater != 0x0) {
                            if (game.turn > 0xa && (game.turn & 0x7) == (lpfl->iPlayer & 0x7))
                                continue;
                            if (Random(4) == 0) {
                                FSendPlrMsg2(lpfl->iPlayer, 258, -5, lpfl->id, 0);
                                continue;
                            }
                        }
                        if (cPass != 0 || lpord[1].iWarp <= 0x6 || lpord[1].iWarp == 0xb || GetRaceGrbit(&rgplr[lpfl->iPlayer], ibitRaceCheapEngines) == 0 ||
                            Random(10) != 0) {
                            if (lpord[1].iWarp < 0xb) {
                                ptBeg = lpfl->pt;
                                if (cPass > 0 && lpord[1].fNoAutoTrack == 0x0) {
                                    lpord[1].pt = lpfl->lpflNext->pt;
                                }
                                ptEnd.x = lpord[1].pt.x;
                                ptEnd.y = lpord[1].pt.y;
                                dRange = EstFuelUse(lpfl, 0, -1, -1, 1);
                                wtFuel2Dest = EstFuelUse(lpfl, 0, -1, -1, 0);
                                t_merge_3de5_0001 = wtFuel2Dest <= lpfl->rgwtMin[4] ? 1 : 0;
                                fGotEnufFuel = t_merge_3de5_0001;
                                fRanOutOfFuel = 0;
                                if (fGotEnufFuel != 0) {
                                    t_merge_3e66_0001 =
                                        dRange <= (int32_t)(uint32_t)(lpord[1].iWarp * lpord[1].iWarp) ? (uint32_t)(lpord[1].iWarp * lpord[1].iWarp) : dRange;
                                    dRange = t_merge_3e66_0001;
                                }
                                if (cPass != 0) {
                                    if (lpfl->lpflNext->fDone == 0x0) {
                                        dTravel = lpfl->dMoveLeft >= (int32_t)(lpfl->dMoveLeft + lpfl->dMoveUsed + 4) / 5
                                                      ? (int32_t)((int32_t)(lpfl->dMoveLeft + lpfl->dMoveUsed + 4) / 5)
                                                      : (int32_t)lpfl->dMoveLeft;
                                    } else {
                                        dTravel = (int32_t)lpfl->dMoveLeft;
                                    }
                                    dRange = dRange - (int32_t)lpfl->dMoveUsed;
                                    if (dRange < 0) {
                                        dRange = 0;
                                    }
                                } else {
                                    if (lpfl->rgwtMin[3] > 10 && GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raMacintosh) {
                                        cDie = (int32_t)((int32_t)((uint32_t)(lpfl->rgwtMin[3] * 3) + 33) / 0x64);
                                        if (cDie > 0) {
                                            lpfl->rgwtMin[3] = lpfl->rgwtMin[3] - cDie;
                                            FSendPlrMsg(lpfl->iPlayer, 193, lpfl->id | 0x8000, LOWORD(cDie), HIWORD(cDie), lpfl->id, 0, 0, 0, 0);
                                        }
                                    }
                                    if (lpord[1].iWarp == 0xa) {
                                        flSrc = *lpfl;
                                        fDead = 1;
                                        cKillTot = 0;
                                        memset(&flDead, 0, sizeof(FLEET));
                                        for (ish = 0; ish < 16; ish++) {
                                            if (flSrc.rgcsh[ish] != 0) {
                                                switch (rglpshdef[lpfl->iPlayer][ish].hul.rghs[0].iItem) {
                                                default:
                                                    cKill = 0;
                                                    cTry = flSrc.rgcsh[ish];
                                                    while (1) {
                                                        t_40c6 = cTry;
                                                        cTry = cTry - 1;
                                                        if (t_40c6 == 0)
                                                            break;
                                                        if (Random(10) == 0) {
                                                            cKill = cKill + 1;
                                                        }
                                                    }
                                                    if (cKill > 0) {
                                                        cKillTot = cKillTot + cKill;
                                                        flSrc.rgcsh[ish] = flSrc.rgcsh[ish] - cKill;
                                                        flDead.rgcsh[ish] = cKill;
                                                    }
                                                case 0x7:
                                                case 0x9:
                                                case 0xe:
                                                case 0xf:
                                                case 0x8:
                                                }
                                                if (flSrc.rgcsh[ish] > 0) {
                                                    fDead = 0;
                                                }
                                            }
                                        }
                                        if (fDead != 0) {
                                            lpfl->fDead = 0x1;
                                            FSendPlrMsg2(lpfl->iPlayer, 225, lpfl->id | 0x8000, lpfl->id, 0);
                                            continue;
                                        }
                                        if (cKillTot > 0) {
                                            flDead.iPlayer = flSrc.iPlayer;
                                            flDead.fDead = 0x1;
                                            flDead.det = 0x7;
                                            FleetTransferCargoBalance(&flSrc, &flDead);
                                            *lpfl = flSrc;
                                            if (cKillTot != 1) {
                                                FSendPlrMsg2(lpfl->iPlayer, 224, lpfl->id | 0x8000, cKillTot, lpfl->id);
                                            } else {
                                                FSendPlrMsg2(lpfl->iPlayer, 223, lpfl->id | 0x8000, lpfl->id, 0);
                                            }
                                        }
                                    }
                                    dTravel = (uint32_t)(lpord[1].iWarp * lpord[1].iWarp);
                                    if (lpord[1].grobj == grobjFleet) {
                                        lpfl->lpflNext = LpflFromId(lpord[1].id);
                                        if (lpfl->lpflNext != 0x0) {
                                            fDone = 0;
                                            lpfl->fDone = 0x0;
                                            lpfl->lPower = (uint32_t)LOWORD(dTravel);
                                            lpfl->lFuelUsed = 0;
                                            continue;
                                        }
                                    }
                                }
                                d = DGetDistance(ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
                                t_merge_4403_0001 = dTravel < (int32_t)LOWORD((int32_t)(d + 0.9999)) ? dTravel : (int32_t)LOWORD((int32_t)(d + 0.9999));
                                dTravel = t_merge_4403_0001;
                                if (dTravel <= dRange) {
                                    if (cPass > 0) {
                                        lpfl->rgwtMin[4] = lpfl->rgwtMin[4] + lpfl->lFuelUsed;
                                        dTravel = dTravel + (int32_t)lpfl->dMoveUsed;
                                    }
                                    wtFuelUsed = EstFuelUse(lpfl, 0, -1, dTravel, 0);
                                    if (cPass > 0) {
                                        lpfl->lFuelUsed = wtFuelUsed;
                                        dTravel = dTravel - (int32_t)lpfl->dMoveUsed;
                                    }
                                    t_merge_450b_0001 = 0x0 <= lpfl->rgwtMin[4] - wtFuelUsed ? lpfl->rgwtMin[4] - wtFuelUsed : 0;
                                    lpfl->rgwtMin[4] = t_merge_450b_0001;
                                } else {
                                    lpfl->rgwtMin[4] = 0;
                                    wtFuelUsed = 1;
                                    dTravel = dRange;
                                }
                                if (lpfl->rgwtMin[4] == 0 && wtFuelUsed > 0 && ((d - 0.99999 >= (double)dTravel || dRange == 0) && fGotEnufFuel == 0)) {
                                    i = 0;
                                    do {
                                        i = i + 1;
                                    } while (EstFuelUse(lpfl, 0, i, -1, 0) == 0 && i < 10);
                                    if (i <= 1) {
                                        FSendPlrMsg2(lpfl->iPlayer, 39, lpfl->id | 0x8000, lpfl->id, 0);
                                    } else {
                                        lpfl->lpplord->rgord[1].iWarp = i - 1;
                                        FSendPlrMsg2(lpfl->iPlayer, 139, lpfl->id | 0x8000, lpfl->id, i - 1);
                                    }
                                    fRanOutOfFuel = 1;
                                }
                                if (dRange == 0)
                                    continue;
                                lpfl->fHereAllTurn = 0x0;
                                dx = ptEnd.x - ptBeg.x;
                                dy = ptEnd.y - ptBeg.y;
                                if (dx != 0 || dy != 0) {
                                    lpfl->fdirValid = 0x1;
                                    for (; abs(dx) > 127 || abs(dy) > 127; dy = (int32_t)dy / 2) {
                                        dx = (int32_t)dx / 2;
                                    }
                                    lpfl->dirFltX = dx + 127;
                                    lpfl->dirFltY = dy + 127;
                                    lpfl->iwarpFlt = lpfl->lpplord->rgord[1].iWarp;
                                }
                                dActTravel = (int32_t)(d - 0.99999);
                                t_merge_47c3_0001 = dTravel < dActTravel ? LOWORD(dTravel) : LOWORD(dActTravel);
                                dMineTravel = t_merge_47c3_0001;
                                if (lpord[1].iWarp >= 0xb || FTravelThroughMineFields(lpfl, &dMineTravel, 0x0) != 0) {
                                    if (fRanOutOfFuel == 0 && GetFuelFree(lpfl) > 0) {
                                        t_merge_48a1_0001 = dTravel < dActTravel ? dTravel : dActTravel;
                                        t_call_48bc = LCalcFuelGainFromRamScoops(lpfl, lpord[1].iWarp, t_merge_48a1_0001);
                                        lFuelGain = t_call_48bc;
                                        if (t_call_48bc > 0) {
                                            lFuelGainAct = ChgCargo(grobjFleet, lpfl->id, 4, lFuelGain, 0x0);
                                            if (lFuelGain > 32500) {
                                                lFuelGain = 32500;
                                            }
                                            FSendPlrMsg2(lpfl->iPlayer, 243, lpfl->id | 0x8000, lpfl->id, LOWORD(lFuelGain));
                                        }
                                    }
                                } else {
                                    lpfl->dMoveLeft = 0;
                                    if (lpfl->fDead != 0x0)
                                        continue;
                                    if ((int32_t)dMineTravel < dActTravel) {
                                        dTravel = (int32_t)dMineTravel;
                                    }
                                }
                                if (dActTravel >= dTravel && dActTravel > 0) {
                                    dxRound = ptEnd.x <= ptBeg.x ? -0.5 : 0.5;
                                    dyRound = ptEnd.y <= ptBeg.y ? -0.5 : 0.5;
                                    if (d > 0.0001 || d < -0.0001) {
                                        r = (double)dTravel / d;
                                        lpfl->pt.x = LOWORD((int32_t)(r * (double)(int32_t)(ptEnd.x - ptBeg.x) + dxRound)) + ptBeg.x;
                                        lpfl->pt.y = LOWORD((int32_t)(r * (double)(int32_t)(ptEnd.y - ptBeg.y) + dyRound)) + ptBeg.y;
                                        lpfl->idPlanet = -1;
                                    }
                                    if (cPass <= 0 || lpfl->dMoveLeft <= 0)
                                        goto L_4b2d;
                                    lpfl->dMoveUsed = lpfl->dMoveUsed + LOWORD(dTravel);
                                    lpfl->dMoveLeft = lpfl->dMoveLeft - LOWORD(dTravel);
                                    if (lpfl->dMoveLeft <= 0 || fRanOutOfFuel != 0)
                                        goto L_4b2d;
                                    fDone = 0;
                                    lpfl->fDone = 0x0;
                                    goto L_4b2d;
                                }
                            } else {
                                fJumpgate = 0;
                                gd.fRadiatingEngine = 0x0;
                                ptMsg = lpord->pt;
                                ptBeg = lpord->pt;
                                if (lpord->grobj != grobjPlanet) {
                                    isbsSrc = -1;
                                } else {
                                    ptMsg.x = -1;
                                    ptMsg.y = lpord->id;
                                    lpplSrc = LpplFromId(lpord->id);
                                    isbsSrc = IStargateFromLppl(lpplSrc);
                                }
                                if (isbsSrc != -1) {
                                    if (lpplSrc->iPlayer != lpfl->iPlayer && (int16_t)rgplr[lpplSrc->iPlayer].rgmdRelation[lpfl->iPlayer] != 1) {
                                        FSendPlrMsg(lpfl->iPlayer, 230, lpfl->id | 0x8000, lpfl->id, lpplSrc->id, lpplSrc->id, 0, 0, 0, 0);
                                        continue;
                                    }
                                } else if (FFleetCanJumpgate(lpfl) == 0) {
                                    FSendPlrMsg(lpfl->iPlayer, 222, lpfl->id | 0x8000, lpfl->id, ptMsg.x, ptMsg.y, 0, 0, 0, 0);
                                    continue;
                                }
                                ptMsg.x = lpord[1].pt.x;
                                ptMsg.y = lpord[1].pt.y;
                                ptEnd.x = lpord[1].pt.x;
                                ptEnd.y = lpord[1].pt.y;
                                if (lpord[1].grobj != grobjPlanet) {
                                    for (i = 0; i < game.cPlanMax && (ptEnd.x != rgptPlan[i].x || ptEnd.y != rgptPlan[i].y); i++) {
                                    }
                                    if (i >= game.cPlanMax) {
                                        FSendPlrMsg(lpfl->iPlayer, 327, lpfl->id | 0x8000, lpfl->id, ptEnd.x, ptEnd.y, 0, 0, 0, 0);
                                        continue;
                                    }
                                    ptMsg.x = -1;
                                    ptMsg.y = i;
                                    lpplDst = LpplFromId(i);
                                    isbsDst = IStargateFromLppl(lpplDst);
                                } else {
                                    ptMsg.x = -1;
                                    ptMsg.y = lpord[1].id;
                                    lpplDst = LpplFromId(lpord[1].id);
                                    isbsDst = IStargateFromLppl(lpplDst);
                                }
                                if (isbsDst == -1) {
                                    FSendPlrMsg(lpfl->iPlayer, 226, lpfl->id | 0x8000, lpfl->id, lpplDst->id, ptMsg.x, ptMsg.y, 0, 0, 0);
                                    continue;
                                }
                                if (lpplDst->iPlayer != lpfl->iPlayer && (int16_t)rgplr[lpplDst->iPlayer].rgmdRelation[lpfl->iPlayer] != 1) {
                                    FSendPlrMsg(lpfl->iPlayer, 229, lpfl->id | 0x8000, lpfl->id, lpplDst->id, lpplDst->id, lpplDst->id, 0, 0, 0);
                                    continue;
                                }
                                if (isbsSrc == -1) {
                                    fJumpgate = 1;
                                    isbsSrc = isbsDst;
                                }
                                if (fJumpgate == 0 && GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) != raStargate) {
                                    if (lpfl->rgwtMin[3] > 0 && lpplSrc->iPlayer != lpfl->iPlayer) {
                                        FSendPlrMsg2(lpfl->iPlayer, 350, lpfl->id | 0x8000, lpfl->id, lpplSrc->id);
                                        continue;
                                    }
                                    wtMinerals = 0;
                                    for (i = 0; i <= 2; i++) {
                                        if (lpfl->rgwtMin[i] != 0) {
                                            wtMinerals = wtMinerals + lpfl->rgwtMin[i];
                                            lpplSrc->rgwtMin[i] = lpplSrc->rgwtMin[i] + lpfl->rgwtMin[i];
                                            lpfl->rgwtMin[i] = 0;
                                        }
                                    }
                                    wtColonists = lpfl->rgwtMin[3];
                                    lpplSrc->rgwtMin[3] = lpplSrc->rgwtMin[3] + lpfl->rgwtMin[3];
                                    lpfl->rgwtMin[3] = 0;
                                    if (wtColonists != 0) {
                                        if (wtMinerals != 0) {
                                            FSendPlrMsg(lpfl->iPlayer, 238, lpfl->id | 0x8000, lpfl->id, LOWORD(wtColonists), HIWORD(wtColonists),
                                                        LOWORD(wtMinerals), HIWORD(wtMinerals), lpplSrc->id, 0);
                                            if (lpfl->iPlayer != lpplSrc->iPlayer) {
                                                FSendPlrMsg(lpplSrc->iPlayer, 238, lpplSrc->id, lpfl->id, LOWORD(wtColonists), HIWORD(wtColonists),
                                                            LOWORD(wtMinerals), HIWORD(wtMinerals), lpplSrc->id, 0);
                                            }
                                        } else {
                                            FSendPlrMsg(lpfl->iPlayer, 237, lpfl->id | 0x8000, lpfl->id, LOWORD(wtColonists), HIWORD(wtColonists), lpplSrc->id,
                                                        0, 0, 0);
                                            if (lpfl->iPlayer != lpplSrc->iPlayer) {
                                                FSendPlrMsg(lpplSrc->iPlayer, 237, lpplSrc->id, lpfl->id, LOWORD(wtColonists), HIWORD(wtColonists), lpplSrc->id,
                                                            0, 0, 0);
                                            }
                                        }
                                    } else if (wtMinerals != 0) {
                                        FSendPlrMsg(lpfl->iPlayer, 236, lpfl->id | 0x8000, lpfl->id, LOWORD(wtMinerals), HIWORD(wtMinerals), lpplSrc->id, 0, 0,
                                                    0);
                                        if (lpfl->iPlayer != lpplSrc->iPlayer) {
                                            FSendPlrMsg(lpplSrc->iPlayer, 236, lpplSrc->id, lpfl->id, LOWORD(wtMinerals), HIWORD(wtMinerals), lpplSrc->id, 0, 0,
                                                        0);
                                        }
                                    }
                                }
                                dTravel = (int32_t)DGetDistance(ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
                                if (FStargateJump(lpfl, isbsSrc, isbsDst, LOWORD(dTravel)) == 0)
                                    continue;
                                lpfl->fHereAllTurn = 0x0;
                                NoAutoTrackFleet(lpfl);
                            }
                            lpfl->pt = ptEnd;
                            if (lpord[1].grobj != grobjPlanet) {
                                lpfl->idPlanet = -1;
                            } else {
                                lpfl->idPlanet = lpord[1].id;
                            }
                            if (cPass > 0) {
                                lpfl->lpflNext->fDone = 0x1;
                            }
                        L_4b2d:
                            if (gd.fRadiatingEngine != 0x0 && lpfl->rgwtMin[3] > 0 && cPass <= 1 &&
                                (int16_t)rgplr[lpfl->iPlayer].rgEnvVarMin[2] + (int16_t)rgplr[lpfl->iPlayer].rgEnvVarMax[2] < 0xaa &&
                                (int16_t)rgplr[lpfl->iPlayer].rgEnvVarMax[2] != -1) {
                                iCtr = (int32_t)((int16_t)rgplr[lpfl->iPlayer].rgEnvVarMin[2] + (int16_t)rgplr[lpfl->iPlayer].rgEnvVarMax[2]) / 2;
                                pct = (int32_t)((0x56 - iCtr) >> 0x1);
                                pct = (int32_t)((int32_t)(pct * lpfl->rgwtMin[3]) / 100);
                                t_merge_4c51_0001 = 1 <= pct ? pct : 0x1;
                                if (lpfl->rgwtMin[3] < t_merge_4c51_0001) {
                                    t_merge_4ca3_0001 = lpfl->rgwtMin[3];
                                } else if (1 <= pct) {
                                    t_merge_4ca3_0001 = pct;
                                } else {
                                    t_merge_4ca3_0001 = 1;
                                }
                                pct = t_merge_4ca3_0001;
                                FSendPlrMsg2(lpfl->iPlayer, 116, lpfl->id | 0x8000, LOWORD(pct), lpfl->id);
                                lpfl->rgwtMin[3] = lpfl->rgwtMin[3] - pct;
                            }
                            if (ptEnd.x == lpfl->pt.x && ptEnd.y == lpfl->pt.y && lpord[1].grobj == grobjThing) {
                                lpth = LpthFromId(lpord[1].id);
                                if (lpth != 0x0 && lpth->ith == ithWormhole) {
                                    grbitPlr = 0x1 << lpfl->iPlayer;
                                    lpthDest = LpthFromId(lpth->thw.idPartner);
                                    NoAutoTrackFleet(lpfl);
                                    lpth->thw.grbitPlrTrav = lpth->thw.grbitPlrTrav | grbitPlr;
                                    lpthDest->thw.grbitPlrTrav = lpthDest->thw.grbitPlrTrav | grbitPlr;
                                    lpthDest->thw.grbitPlr = lpthDest->thw.grbitPlr | grbitPlr;
                                    lpfl->pt = lpthDest->pt;
                                    lpord[1].pt = lpthDest->pt;
                                }
                            }
                            if (lpfl->idPlanet == -1 && FFindNearestObject(lpfl->pt, 0x81, &scan) != 0) {
                                lpfl->idPlanet = scan.idpl;
                            }
                            lpord->pt = lpfl->pt;
                            lpord->id = lpfl->idPlanet;
                            lpord->grobj = lpfl->idPlanet == -1 ? 0x4 : 0x1;
                            if (fGotEnufFuel != 0) {
                                t_call_4e92 = EstFuelUse(lpfl, 0, -1, -1, 0);
                                wtFuel2Dest = t_call_4e92;
                                if (t_call_4e92 > lpfl->rgwtMin[4]) {
                                    if (LGetFleetStat(lpfl, 1) <= wtFuel2Dest) {
                                        lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
                                    } else {
                                        lpfl->rgwtMin[4] = wtFuel2Dest;
                                    }
                                }
                            }
                        } else {
                            FSendPlrMsg2(lpfl->iPlayer, 242, lpfl->id | 0x8000, lpfl->id, 0);
                        }
                    }
                }
            }
            if (fDone != 0)
                break;
            t_4f1e = cPass;
            cPass = cPass + 1;
        } while (t_4f1e < 10);
        KillUsedWaypoints();
    }
    return;
}

int16_t FTravelThroughMineFields(FLEET *lpfl, int16_t *pdTravel, THING *lpthHit) {
    int32_t  d2Closest;
    int16_t  rgishInc[16];
    int16_t  dTravel;
    POINT16  ptAct;
    int16_t  iWarp;
    POINT16  ptDst;
    int16_t  dy;
    int32_t  d2;
    int16_t  j;
    int16_t  dEnd;
    FLEET    flSrc;
    int32_t  dpsh;
    int16_t  cshT;
    int32_t  dmgReduce;
    int32_t  dmgToApply;
    int16_t  i;
    THING   *lpth;
    int16_t  dmgExtra;
    int16_t  cshDamaged;
    int16_t  fMineExpert;
    POINT16  ptSrc;
    int16_t  iPlayer;
    int16_t  cFields;
    int16_t  dStart;
    FLEET    flDead;
    THING   *lpthMac;
    int32_t  csh;
    int16_t  rgi[3];
    int16_t  pct;
    int32_t  dmgTot;
    int16_t  cshDead;
    int16_t  rgcField[3];
    int16_t  raMajor;
    int16_t  dx;
    int32_t  dpShield;
    int16_t  iType;
    int16_t  rgFieldE[3][8];
    THING   *lpthClosest;
    int16_t  cishInc;
    THING   *lpthSalvage;
    int16_t  fHasRamScoop;
    int16_t  dmgPer;
    int16_t  rgFieldS[3][8];
    int16_t  cEngines;
    int32_t  dmgPerShip;
    uint16_t ibit;
    uint16_t t_merge_4fc4_0001;
    int16_t  t_59a9;
    int32_t  t_merge_5ae4_0001;
    double   t_call_5df4;
    int16_t  t_merge_63a1_0001;
    int16_t  t_merge_63f3_0001;
    int16_t  t_merge_6494_0001;
    int16_t  t_merge_64e6_0001;

    lpthSalvage = 0x0;
    cshDead = 0;
    dTravel = *pdTravel;
    cishInc = 0;
    iPlayer = lpfl->iPlayer;
    raMajor = GetRaceStat(&rgplr[iPlayer], rsMajorAdv);
    t_merge_4fc4_0001 = raMajor == 5 ? 0x1 : 0x0;
    fMineExpert = t_merge_4fc4_0001 * 2 + (raMajor == 1 ? 1 : 0);
    if (lpthHit != 0x0) {
        iWarp = 0;
    } else {
        ptSrc = lpfl->pt;
        ptDst = lpfl->lpplord->rgord[1].pt;
        for (iWarp = 3; iWarp < 10 && iWarp * iWarp < dTravel - 1; iWarp++) {
        }
        if (iWarp <= fMineExpert + 3 || (ptSrc.x == ptDst.x && ptSrc.y == ptDst.y)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            rgcField[i] = 0;
        }
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->iplr != iPlayer && lpth->ith == ithMinefield && (int16_t)rgplr[lpth->iplr].rgmdRelation[iPlayer] != 1 &&
                FIntersectCircleLine(ptSrc, ptDst, lpth->pt, lpth->thm.cMines, dTravel, &dStart, &dEnd) != 0) {
                iType = lpth->thm.iType;
                for (i = 0; i < rgcField[iType] && rgFieldE[iType][i] < dStart; i++) {
                }
                if (i != rgcField[iType]) {
                    if (dEnd >= rgFieldS[iType][i] - 1) {
                        if (dStart < rgFieldS[iType][i]) {
                            rgFieldS[iType][i] = dStart;
                        }
                        if (dEnd > rgFieldE[iType][i]) {
                            rgFieldE[iType][i] = dEnd;
                            for (j = i + 1; j < rgcField[iType] && rgFieldS[iType][j] <= dEnd; j++) {
                            }
                            if (rgFieldE[iType][j - 1] > dEnd) {
                                rgFieldE[iType][i] = rgFieldE[iType][j - 1];
                            }
                            i = i + 1;
                            for (; j < rgcField[iType]; j++) {
                                rgFieldS[iType][i] = rgFieldS[iType][j];
                                rgFieldE[iType][i] = rgFieldE[iType][j];
                                i = i + 1;
                            }
                            rgcField[iType] = rgcField[iType] - (j - i);
                        }
                    } else if (rgcField[iType] < 8) {
                        for (j = rgcField[iType]; j > i; j--) {
                            rgFieldS[iType][j] = rgFieldS[iType][j - 1];
                            rgFieldE[iType][j] = rgFieldE[iType][j - 1];
                        }
                        rgFieldS[iType][i] = dStart;
                        rgFieldE[iType][i] = dEnd;
                        rgcField[iType] = rgcField[iType] + 1;
                    }
                } else if (i < 8) {
                    rgFieldS[iType][i] = dStart;
                    rgFieldE[iType][i] = dEnd;
                    rgcField[iType] = rgcField[iType] + 1;
                }
            }
        }
        cFields = rgcField[0] + rgcField[1] + rgcField[2];
        if (cFields == 0) {
            return 1;
        }
    }
    fHasRamScoop = 0;
    csh = 0;
    for (i = 0; i < 16; i++) {
        if (lpfl->rgcsh[i] > 0) {
            csh = csh + (int32_t)lpfl->rgcsh[i];
            j = rglpshdef[iPlayer][i].hul.rghs[0].iItem;
            if (LpengineFromId(j)->rgcFuelUsed[4] == 0) {
                fHasRamScoop = 1;
            }
        }
    }
    if (lpthHit != 0x0) {
        iType = lpthHit->thm.iType;
    } else {
        rgi[2] = 0;
        rgi[1] = 0;
        rgi[0] = 0;
        for (; cFields > 0; cFields--) {
            dStart = 10000;
            iType = -1;
            for (i = 0; i < 3; i++) {
                if (rgi[i] < rgcField[i] && dStart > rgFieldS[i][rgi[i]]) {
                    dStart = rgFieldS[i][rgi[i]];
                    iType = i;
                }
            }
            dEnd = rgFieldE[iType][rgi[iType]] - rgFieldS[iType][rgi[iType]];
            if (iWarp > rgiWarpSafe[iType] + fMineExpert) {
                pct = (iWarp - rgiWarpSafe[iType] - fMineExpert) * rgpctMineHit[iType];
                for (i = 0; i < dEnd && Random(1000) >= pct; i++) {
                }
                if (i != dEnd)
                    goto L_57e6;
            }
            rgi[iType] = rgi[iType] + 1;
        }
        return 1;
    L_57e6:
        dEnd = dStart + i;
    }
    cshDead = 0;
    dmgTot = 0;
    if (rgrgdmgMine[iType][fHasRamScoop] != 0) {
        dmgPer = rgrgdmgMine[iType][fHasRamScoop];
        dmgExtra = rgrgdmgMinMine[iType][fHasRamScoop] - rgrgdmgMine[iType][fHasRamScoop] * LOWORD(csh);
        if (csh >= 5 || dmgExtra <= 0) {
            dmgExtra = 0;
        }
        flSrc = *lpfl;
        memset(&flDead, 0, sizeof(FLEET));
        flDead.iPlayer = flSrc.iPlayer;
        flDead.fDead = 0x1;
        flDead.det = 0x7;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0 &&
                (lpthHit == 0x0 || lpthHit->iplr != lpfl->iPlayer ||
                 (rglpshdef[lpfl->iPlayer][i].hul.ihuldef != ihuldefMiniMineLayer && rglpshdef[lpfl->iPlayer][i].hul.ihuldef != ihuldefSuperMineLayer))) {
                cshT = lpfl->rgcsh[i];
                t_59a9 = cishInc;
                cishInc = cishInc + 1;
                rgishInc[t_59a9] = i;
                cEngines = rglpshdef[lpfl->iPlayer][i].hul.rghs[0].cItem;
                dpShield = (uint32_t)((int32_t)cshT * DpShieldOfShdef(rglpshdef[iPlayer] + i, iPlayer));
                dpsh = (uint32_t)rglpshdef[iPlayer][i].hul.dp;
                dmgToApply = (uint32_t)(((uint32_t)((int32_t)cshT * (int32_t)dmgPer) + (int32_t)dmgExtra) * (int32_t)cEngines);
                dmgTot = dmgTot + dmgToApply;
                t_merge_5ae4_0001 = dpShield < (int32_t)(dmgToApply >> 0x1) ? dpShield : (int32_t)(dmgToApply >> 0x1);
                dmgReduce = t_merge_5ae4_0001;
                dmgToApply = dmgToApply - dmgReduce;
                cshDamaged = LOWORD((int32_t)((int32_t)((int32_t)lpfl->rgcsh[i] * lpfl->rgdv[i].pctSh) / 0x64));
                dmgToApply = dmgToApply + (int32_t)((int32_t)((uint32_t)(dpsh * lpfl->rgdv[i].pctDp) * (int32_t)cshDamaged) / 0x1f4);
                dmgExtra = 0;
                dmgPerShip = (int32_t)(dmgToApply / (int32_t)cshT);
                if (dmgPerShip <= dpsh) {
                    flSrc.rgdv[i].pctSh = 0x64;
                    flSrc.rgdv[i].pctDp = LOWORD((int32_t)((int32_t)(dmgPerShip * 500) / dpsh));
                    if (flSrc.rgdv[i].pctDp == 0x0) {
                        flSrc.rgdv[i].pctDp = 0x1;
                    }
                } else {
                    cshDead = cshDead + cshT;
                    flDead.rgcsh[i] = cshT;
                    flSrc.rgcsh[i] = 0;
                    cshT = 0;
                }
                dmgToApply = 0;
            }
        }
        if (lpthHit != 0x0 && dmgTot == 0) {
            return 0;
        }
        if ((int32_t)cshDead != csh) {
            FleetTransferCargoBalance(&flSrc, &flDead);
        }
        *lpfl = flSrc;
        if ((int32_t)cshDead == csh) {
            lpfl->fDead = 0x1;
        }
    }
    if (lpthHit != 0x0) {
        ptAct = lpthHit->pt;
        lpthClosest = lpthHit;
    } else {
        dx = ptDst.x - ptSrc.x;
        dy = ptDst.y - ptSrc.y;
        t_call_5df4 = sqrt((double)((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)(ptDst.y - ptSrc.y))));
        dTravel = LOWORD((int32_t)(t_call_5df4 + 0.5));
        ptAct.x = MulDiv(dx, dEnd, (int32_t)(t_call_5df4 + 0.5)) + ptSrc.x;
        ptAct.y = MulDiv(dy, dEnd, dTravel) + ptSrc.y;
        if (cshDead != 0) {
            lpthSalvage = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpthSalvage < lpthMac &&
                   (lpthSalvage->pt.x != ptAct.x || lpthSalvage->pt.y != ptAct.y || lpthSalvage->ith != ithMineralPacket || lpthSalvage->thp.iWarp != 0x0);
                 lpthSalvage++) {
            }
            if (lpthSalvage == lpthMac) {
                lpthSalvage = 0x0;
            }
            DropSalvage(&lpthSalvage, lpfl->rgwtMin, flSrc.iplr, &ptAct);
        }
        d2Closest = 100000000;
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->iplr != iPlayer && lpth->ith == ithMinefield && (int16_t)rgplr[lpth->iplr].rgmdRelation[iPlayer] != 1 && lpth->thm.iType == iType) {
                dx = lpth->pt.x - ptAct.x;
                dy = lpth->pt.y - ptAct.y;
                d2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)(lpth->pt.y - ptAct.y)) - lpth->thm.cMines;
                if (d2 < d2Closest) {
                    d2Closest = d2;
                    lpthClosest = lpth;
                }
            }
        }
        d2 = (int32_t)(lpthClosest->thm.cMines / 20);
        if (d2 <= 50) {
            if (d2 < 10) {
                d2 = 10;
            }
        } else {
            d2 = (int32_t)(lpthClosest->thm.cMines / 100);
            if (d2 < 50) {
                d2 = 50;
            }
        }
    }
    if (GetRaceStat(&rgplr[lpthClosest->iplr], rsMajorAdv) == raMines) {
        ibit = 0x1 << lpthClosest->iplr;
        if (cishInc != 0) {
            for (i = 0; i < cishInc; i++) {
                rglpshdef[iPlayer][rgishInc[i]].grbitPlr = rglpshdef[iPlayer][rgishInc[i]].grbitPlr | ibit;
            }
        } else {
            for (i = 0; i < 16; i++) {
                if (lpfl->rgcsh[i] > 0) {
                    rglpshdef[iPlayer][i].grbitPlr = rglpshdef[iPlayer][i].grbitPlr | ibit;
                }
            }
        }
    }
    if (dmgTot > 32760) {
        dmgTot = 32760;
    }
    if (dmgTot != 0) {
        if (cshDead != 0) {
            if ((int32_t)cshDead < csh) {
                if (iPlayer != lpthClosest->iplr) {
                    t_merge_6494_0001 = lpthHit == 0x0 ? 199 : 353;
                    FSendPlrMsg(iPlayer, t_merge_6494_0001, lpfl->id | 0x8000, lpfl->id | 0x8000, lpthClosest->iplr, iType, ptAct.x, ptAct.y, LOWORD(dmgTot),
                                cshDead);
                }
                t_merge_64e6_0001 = lpthHit == 0x0 ? 203 : 356;
                FSendPlrMsg(lpthClosest->iplr, t_merge_64e6_0001, lpfl->id | 0x8000, lpfl->id | 0x8000, iType, ptAct.x, ptAct.y, LOWORD(dmgTot), cshDead, 0);
            } else if (lpthSalvage != 0x0) {
                flDead.id = lpfl->id;
                if (iPlayer != lpthClosest->iplr) {
                    FSendPlrMsg(iPlayer, 200, -6, lpthSalvage->idFull, WFromLpfl(&flDead), lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0);
                }
                FSendPlrMsg(lpthClosest->iplr, 204, -6, lpthSalvage->idFull, lpfl->id, iType, ptAct.x, ptAct.y, 0, 0);
            } else {
                flDead.id = lpfl->id;
                if (iPlayer != lpthClosest->iplr) {
                    FSendPlrMsg(iPlayer, 351, -1, WFromLpfl(&flDead), lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0, 0);
                }
                if (lpfl->iPlayer != lpthClosest->iplr) {
                    FSendPlrMsg(lpthClosest->iplr, 204, -6, lpthClosest->idFull, lpfl->id, iType, ptAct.x, ptAct.y, 0, 0);
                } else {
                    FSendPlrMsg(lpthClosest->iplr, 354, -1, WFromLpfl(&flDead), iType, ptAct.x, ptAct.y, 0, 0, 0);
                }
            }
        } else {
            if (iPlayer != lpthClosest->iplr) {
                t_merge_63a1_0001 = lpthHit == 0x0 ? 198 : 352;
                FSendPlrMsg(iPlayer, t_merge_63a1_0001, lpfl->id | 0x8000, lpfl->id | 0x8000, lpthClosest->iplr, iType, ptAct.x, ptAct.y, LOWORD(dmgTot), 0);
            }
            t_merge_63f3_0001 = lpthHit == 0x0 ? 202 : 355;
            FSendPlrMsg(lpthClosest->iplr, t_merge_63f3_0001, lpfl->id | 0x8000, lpfl->id | 0x8000, iType, ptAct.x, ptAct.y, LOWORD(dmgTot), 0, 0);
        }
    } else {
        if (iPlayer != lpthClosest->iplr) {
            FSendPlrMsg(iPlayer, 197, lpfl->id | 0x8000, lpfl->id | 0x8000, lpthClosest->iplr, iType, ptAct.x, ptAct.y, 0, 0);
        }
        FSendPlrMsg(lpthClosest->iplr, 201, lpfl->id | 0x8000, lpfl->id | 0x8000, iType, ptAct.x, ptAct.y, 0, 0, 0);
    }
    if (lpthHit == 0x0) {
        if (d2 < lpthClosest->thm.cMines) {
            lpthClosest->thm.cMines = lpthClosest->thm.cMines - d2;
            lpthClosest->thm.grbitPlrNow = lpthClosest->thm.grbitPlrNow | 0x1 << iPlayer;
            lpthClosest->thm.grbitPlr = lpthClosest->thm.grbitPlr | 0x1 << iPlayer;
        } else {
            FreeLpth(lpthClosest);
        }
        *pdTravel = dEnd;
    }
    lpfl->fNoHeal = 0x1;
    return 0;
}
