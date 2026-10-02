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
    int16_t  t_call_120d;

    idCur = idPlayer;
    fSuccess = 0;
    hcurSav = SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32514)));
    DestroyCurGame();
    if (gd.fTutorial != 0) {
        Randomize(1234567890);
    }
    fErrSav = fFileErrSilent;
    fFileErrSilent = 1;
    UpdateProgressGauge(360);
    if (FLoadGame(szBase, "hst") == 0) {
        fFileErrSilent = fErrSav;
        SetCursor(hcurSav);
        TurnLog(idsCantFindHostFile);
        return 0;
    }
    TurnLog(idsGeneratingYearD);
    fFileErrSilent = fErrSav;
    if ((wVersFile >> 0xc & 0xf) <= 0) {
        for (i = 0; i < game.cPlayer && rgplr[i].iPlrBmp == 0; i++) {
        }
        if (i == game.cPlayer) {
            for (i = 0; i < game.cPlayer; i++) {
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xff07) | (i & 0x1f) * 8;
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
        gd.fGeneratingTurn = 1;
        gd.fRetryOpens = 1;
        imemMsgCur = 0;
        for (i = 0; i < game.cPlayer; i++) {
            mpiplr2[i] = i;
        }
        for (i = 0; i < game.cPlayer; i++) {
            j = Random(game.cPlayer - i) + i;
            if (j != i) {
                idCur = mpiplr2[j];
                mpiplr2[j] = mpiplr2[i];
                mpiplr2[i] = idCur;
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            j = mpiplr2[i];
            _wsprintf(szWork, "%s.x%d", szBase, j + 1);
            idPlayer = j;
            vrgts[j].lSerialNumber = -1;
            if (FLoadLogFile(szWork) != 0 && FRunLogFile() == 0) {
                AlertSz(PszFormatIds(idsPlayerLogFileAppearsCorruptUnableLoad, NULL), MB_ICONHAND);
                goto FreeStuffUp;
            }
            UpdateProgressGauge(MulDiv(60, i + 1, game.cPlayer) + 370);
        }
        idPlayer = -1;
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fCrippled != 0 || rgplr[i].fAi != 0 || (gd.fTutorial != 0 && i == 0)) {
                rgplr[i].wFlags &= 0xfffb;
            } else if (vrgts[i].lSerialNumber != -1 && FValidSerialLong(vrgts[i].lSerialNumber) == 0) {
                rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffb) | 4;
            } else if (vrgts[i].lSerialNumber != -1) {
                rgplr[i].wFlags &= 0xfffb;
                for (j = 0; j < i; j++) {
                    if (rgplr[j].fCrippled == 0 && rgplr[j].fAi == 0 && vrgts[i].lSerialNumber == vrgts[j].lSerialNumber &&
                        fmemcmp(vrgts[i].rgbConfig, vrgts[j].rgbConfig, 11) != 0) {
                        rgplr[j].wFlags = (rgplr[j].wFlags & 0xfffb) | 4;
                        rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffb) | (1 & 1) * 4;
                    }
                }
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fCheater != 0) {
                j = IPlrAlsoCheater(i);
                FSendPlrMsg2(i, (j != -1) + 0x100, gotoSerialNumber, j, 0);
                if (game.turn > 10 && (game.turn & 7) == (i & 7)) {
                    FSendPlrMsg2(i, idmFleetCaptainsHaveStagedStrikeDemandFree, gotoSerialNumber, 0, 0);
                }
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            for (ish = 0; ish < 16; ish++) {
                if (rglpshdef[i][ish].fFree == 0 && rglpshdef[i][ish].hul.rghs[0].grhst != hstEngine) {
                    rglpshdef[i][ish].hul.rghs[0].grhst = hstEngine;
                    rglpshdef[i][ish].hul.rghs[0].iItem = 1;
                    if (rglpshdef[i][ish].hul.rghs[0].cItem < 1) {
                        rglpshdef[i][ish].hul.rghs[0].cItem = 1;
                    }
                }
            }
        }
        fFollow = 0;
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            lpfl->fNoHeal = 0;
            if (lpfl->cord == 1 && lpfl->lpplord->rgord[0].grobj == grobjFleet) {
                fFollow = 1;
                lpfl->fMark = 1;
            } else {
                if (lpfl->lpplord->rgord[0].grobj == grobjFleet && lpfl->cord == 1) {
                    FSendPlrMsg(lpfl->iPlayer, idmHadOrdersFollowFleetWhichDidntMove, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                }
                lpfl->fMark = 0;
            }
        }
        ValidateWaypoints();
        if (fFollow != 0) {
            fFollow = 1;
            for (i = 0; i < 8 && fFollow != 0; i++) {
                fFollow = 0;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0)
                        break;
                    if (lpfl->fMark != 0 && lpfl->cord == 1) {
                        ord = lpfl->lpplord->rgord[0];
                        if (ord.grobj == grobjFleet) {
                            lpflTarget = LpflFromId(ord.id);
                            if (lpflTarget == 0 || (lpflTarget->cord == 1 && lpflTarget->lpplord->rgord[0].grobj != grobjFleet)) {
                                FSendPlrMsg(lpfl->iPlayer, idmHadOrdersFollowFleetWhichDidntMove, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                            } else {
                                if (lpflTarget->cord == 1)
                                    continue;
                                fFollow = 1;
                                if (lpfl->lpplord->iordMax <= 1) {
                                    lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, 2);
                                }
                                lpfl->lpplord->rgord[1] = lpflTarget->lpplord->rgord[1];
                                lpfl->lpplord->rgord[1].txp = lpfl->lpplord->rgord[0].txp;
                                lpfl->cord = 2;
                                lpfl->lpplord->iordMac = 2;
                                continue;
                            }
                        }
                        lpfl->fMark = 0;
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
            if (rgplr[i].pctResearch < 0 || rgplr[i].pctResearch > 100) {
                rgplr[i].pctResearch = 15;
            }
            if (rgplr[i].pctIdealGrowth < 0) {
                rgplr[i].pctIdealGrowth = 1;
            }
            if (rgplr[i].pctIdealGrowth > 20) {
                rgplr[i].pctIdealGrowth = 20;
            }
            j = rgplr[i].fHacker;
            cAdv = CAdvantagePoints(&rgplr[i]);
            if ((cAdv < 0 || j != rgplr[i].fHacker) && rgplr[i].fAi == 0) {
                FSendPlrMsg2(i, idmRaceDefinitionHasTamperedStatisticsHaveAltered, gotoNone, 0, 0);
                for (j = 0; j < game.cPlayer; j++) {
                    if (i != j && rgplr[i].fAi == 0) {
                        FSendPlrMsg2(j, idmHackedRaceDiscoveredRaceStatisticsHaveAltered, gotoNone, i, 0);
                    }
                }
                rgplr[i].wFlags = (rgplr[i].wFlags & 0xffef) | 0x10;
                if (cAdv < 500) {
                    while (rgplr[i].rgAttr[0] < 25) {
                        rgplr[i].rgAttr[0]++;
                        cAdv = CAdvantagePoints(&rgplr[i]);
                        if (cAdv >= 500)
                            break;
                    }
                }
                if (cAdv < 500) {
                    while (rgplr[i].pctIdealGrowth > 1) {
                        rgplr[i].pctIdealGrowth--;
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
            lppl->fHomeworld = 0;
        }
        for (i = 0; i < game.cPlayer; i++) {
            lpPlanets[rgplr[i].idPlanetHome].fHomeworld = 1;
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
        FMarkFile(dtHost, -1, mdMarkInUse, 0);
        CreateBackupDir();
        game.turn++;
        pchCur = &szBase[strlen(szBase)];
        pchT = strrchr(szBase, 92);
        strcpy(szT, szBackup);
        if (pchT == 0) {
            strcat(szT, szBase);
        } else {
            strcat(szT, pchT + 1);
        }
        pchBak = &szT[strlen(szT)];
        UpdateProgressGauge(854);
        UpdatePlayerScores();
        for (i = 0; i < game.cPlayer; i++) {
            for (j = 0; j < 10; j++) {
                if (rglpshdefSB[i][j].fFree == 0) {
                    t_call_120d = PctCloakFromHuldef(&rglpshdefSB[i][j].hul, i, NULL);
                    rglpshdefSB[i][j].lVisible = (int16_t)(100 - t_call_120d);
                    rglpshdefSB[i][j].lVisible = (uint32_t)(rglpshdefSB[i][j].lVisible * rglpshdefSB[i][j].lVisible);
                }
            }
            for (j = 0; j < 16; j++) {
                if (rglpshdef[i][j].fFree == 0) {
                    dRange = GetShdefScannerRange(rglpshdef[i] + j, i, &dPlanRange, &pctDetect, &iSteal);
                    rglpshdef[i][j].dScanRange = dRange;
                    rglpshdef[i][j].dScanRange2 = dPlanRange;
                    rglpshdef[i][j].pctDetect = pctDetect;
                    rglpshdef[i][j].iSteal = iSteal;
                    if (FCanBuildShdef(rglpshdef[i] + j, i) == 0) {
                        rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0x7fff) | 0x8000;
                    }
                }
            }
        }
        j = 856;
        fDone = 0;
        memset(rgfNoXFile, 0, 16);
        i = 0;
        while (fDone == 0) {
            UpdateProgressGauge(j);
            j += 17 / (game.cPlayer + 1);
            if (i >= game.cPlayer) {
                i = -1;
                fDone = 1;
            }
            if (i >= 0) {
                _wsprintf(pchCur, ".x%d", i + 1);
                strcpy(pchBak, pchCur);
                remove(szT);
                if (access(szBase, 0) == -1) {
                    rgfNoXFile[i] = 1;
                } else {
                    rename(szBase, szT);
                }
                pchBak[1] = 'm';
                pchCur[1] = 'm';
            } else {
                strcpy(pchCur, ".hst");
                strcpy(pchBak, ".hst");
            }
            remove(szT);
            if (i >= 0 && rgfNoXFile[i] != 0) {
                StarsCopyFile(szT, szBase);
            } else {
                rename(szBase, szT);
            }
            *pchCur = 0;
            i++;
        }
        j = 875;
        fDone = 0;
        game.wGen = (uint16_t)Random(8);
        i = 0;
        while (fDone == 0) {
            UpdateProgressGauge(j);
            j += 122 / (game.cPlayer + 1);
            if (i >= game.cPlayer) {
                i = -1;
                fDone = 1;
            }
            FWriteDataFile(szBase, i, i != -1 && rgfNoXFile[i] != 0);
            i++;
        }
        UpdateProgressGauge(998);
        imemLogCur = 0;
        fSuccess = 1;
    }
FreeStuffUp:
    UpdateProgressGauge(1000);
    FreeLp(vrgPlanResExtra, htMisc);
    vrgPlanResExtra = NULL;
    FreeLp(vrgts, htMisc);
    vrgts = NULL;
    FreeLp(lpcd, htMisc);
    lpcd = NULL;
    FreeLp(lpxf, htMisc);
    lpxf = NULL;
    gd.fGeneratingTurn = 0;
    gd.fRetryOpens = 0;
    idPlayer = -1;
    if (fSuccess != 0 && ini.fGen != 0) {
        vretExitValue = 1;
    }
    SetCursor(hcurSav);
    TurnLog(fSuccess + 1380);
    return fSuccess;
}
