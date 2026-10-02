int16_t FWriteDataFile(char *pszFileBase, int16_t iPlayer, int16_t fAppend) {
    int16_t  iMax;
    FLEET   *lpflT;
    int16_t  fNoAutoTrack;
    BTLPLAN *lpbtlplan;
    int16_t  j;
    jmp_buf *penvMemSav;
    int16_t  i;
    ORDER   *lpord;
    THING   *lpth;
    FLEET   *lpfl;
    jmp_buf  env;
    int16_t  iord;
    SHDEF   *lpshdef;
    THING   *lpthMac;
    int16_t  fRet;
    PLANET  *lpplT;
    SCAN     scan;
    MdTarget mdTarget;
    FLEET   *lpflTarget;
    POINT16  pt;
    int32_t  dy;
    int16_t  iflT;
    FLEET   *lpflBest;
    int16_t  fFoundIdeal;
    int32_t  dx;
    int32_t  lBest;
    int32_t  l;
    PLANET   pl;

    fRet = TRUE;
    SetVisiblePlanFleet(iPlayer);
    if (gd.fGeneratingTurn != 0 && iPlayer != -1) {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpfl->fDead == 0 && lpfl->iplr == iPlayer) {
                for (j = 0; j < 16 && lpfl->rgcsh[j] == 0; j++) {
                }
                if (j == 16) {
                    lpfl->fDead = TRUE;
                } else {
                    lpord = lpfl->lpplord->rgord;
                    if (lpord->grobj == grobjFleet) {
                        if (FFindNearestObject(lpord->pt, grobjPlanet | mdExact, &scan) != 0) {
                            lpord->grobj = grobjPlanet;
                            lpord->id = scan.idpl;
                        } else {
                            lpord->grobj = grobjOther;
                            lpord->id = 0;
                        }
                    }
                    if (lpord->grTask == grTaskNone && lpfl->cord > 1 && lpord[1].grTask == grTaskPatrol) {
                        lpord->grTask = grTaskPatrol;
                        lpord->tptl.iWarp = lpord[1].tptl.iWarp;
                        lpord->tptl.iDist = lpord[1].tptl.iDist;
                    }
                    if (lpord->grTask == grTaskPatrol && (lpfl->cord <= 1 || lpord[1].grobj != grobjFleet)) {
                        lpflBest = NULL;
                        lBest = 100000000;
                        fFoundIdeal = FALSE;
                        if (lpfl->idPlanet == -1 && lpfl->cord >= 2 && lpfl->fRepOrders != 0) {
                            pt = lpord[1].pt;
                        } else {
                            pt = lpfl->pt;
                        }
                        lpbtlplan = rglpbtlplan[lpfl->iPlayer] + lpfl->iplan;
                        mdTarget = lpbtlplan->mdTarget1;
                        for (iflT = 0; iflT < cFleet; iflT++) {
                            lpflTarget = rglpfl[iflT];
                            if (rglpfl[iflT] == 0)
                                break;
                            if (lpflTarget->fInclude != 0 && lpflTarget->iPlayer != iPlayer) {
                                dx = (int16_t)(lpflTarget->pt.x - pt.x);
                                dy = (int16_t)(lpflTarget->pt.y - pt.y);
                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if (((fFoundIdeal == 0 && lpflTarget->fMark == 0) || (l < lBest && (fFoundIdeal == 0 || lpflTarget->fMark == 0))) &&
                                    (FMatchTarget(lpflTarget, mdTarget, FALSE) != 0 && FAttackPlayer(lpfl, lpflTarget->iPlayer) != 0)) {
                                    lpflBest = lpflTarget;
                                    lBest = l;
                                    if (lpflTarget->fMark == 0) {
                                        fFoundIdeal = TRUE;
                                    }
                                }
                            }
                        }
                        if (fFoundIdeal != 0 && gd.fTutorial == 0) {
                            lpflBest->fMark = TRUE;
                        }
                        j = 50 * lpord->tptl.iDist + 50;
                        if (j == 550) {
                            j = 10000;
                        }
                        if (lpflBest != 0 && lBest != 0 && lBest <= (int32_t)(uint32_t)(j * j)) {
                            if (lpfl->lpplord->iordMax <= lpfl->cord + 1) {
                                lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, lpfl->cord + 2);
                                lpord = lpfl->lpplord->rgord;
                            }
                            if (lpfl->cord > 1) {
                                fmemmove(lpord + 2, lpord + 1, (lpfl->cord - 1) * sizeof(ORDER));
                            }
                            if (lpfl->cord == 1) {
                                fmemset(lpord + 1, 0, sizeof(ORDER));
                                lpord[1].fValidTask = TRUE;
                                lpord[1].grTask = grTaskPatrol;
                                lpord[1].tptl = lpord->tptl;
                                if (lpord[1].tptl.iWarp == 0) {
                                    lpord[1].iWarp = IFindIdealWarp(lpfl, FALSE);
                                } else {
                                    lpord[1].iWarp = lpord[1].tsell.iPlrX;
                                }
                                if (lpfl->fRepOrders != 0) {
                                    lpord[2] = *lpord;
                                    lpord[2].iWarp = IFindIdealWarp(lpfl, FALSE);
                                    lpfl->cord++;
                                    lpfl->lpplord->iordMac++;
                                }
                            } else if (lpord[1].tsell.iPlrX == 0) {
                                lpord[1].iWarp = IFindIdealWarp(lpfl, FALSE);
                            } else {
                                lpord[1].iWarp = lpord[1].tsell.iPlrX;
                            }
                            lpord[1].pt = lpflBest->pt;
                            lpord[1].id = lpflBest->id;
                            lpord[1].grobj = grobjFleet;
                            lpfl->cord++;
                            lpfl->lpplord->iordMac++;
                            FSendPlrMsg(iPlayer, idmPatrollingHasTargetedIntercept, lpfl->id | 0x8000, lpfl->id, lpflBest->id, 0, 0, 0, 0, 0);
                        }
                    }
                    if (lpord->grTask != grTaskXfer && lpfl->cord > 1) {
                        for (iord = 1; iord < lpfl->cord; iord++) {
                            if (lpord[iord].grobj == grobjThing) {
                                lpth = LpthFromId(lpord[iord].id);
                                if (lpth == 0 || (lpth->ith == ithMysteryTrader && lpth->tht.fInclude == 0) ||
                                    (lpth->ith == ithMinefield && (1 << iPlayer & lpth->thm.grbitPlrNow) == 0) ||
                                    (lpth->ith == ithWormhole && lpth->thw.fInclude == 0)) {
                                    if (lpth != 0 && lpth->ith == ithWormhole) {
                                        FSendPlrMsg2(lpfl->iPlayer, idmWormholeHeadingHasVanishedOrdersHaveChanged, lpfl->id | 0x8000, lpfl->id, 0);
                                    } else if (lpth != 0 && lpth->ith == ithMysteryTrader) {
                                        FSendPlrMsg2(lpfl->iPlayer, idmMysteryTraderHeadingHasVanishedOrdersHave, lpfl->id | 0x8000, lpfl->id, 0);
                                    } else if (lpth != 0 && lpth->ith == ithMinefield) {
                                        FSendPlrMsg2(lpfl->iPlayer, idmMineFieldHeadingHasVanishedOrdersHave, lpfl->id | 0x8000, lpfl->id, 0);
                                    }
                                    lpord[iord].grobj = grobjOther;
                                    lpord[iord].id = iord;
                                }
                            } else if (lpord[iord].grobj == grobjFleet) {
                                fNoAutoTrack = lpord[iord].fNoAutoTrack;
                                if (fNoAutoTrack != 0) {
                                    lpord[iord].fNoAutoTrack = FALSE;
                                }
                                lpflT = LpflFromId(lpord[iord].id);
                                if (lpflT == 0 || lpflT->fDead != 0) {
                                    FSendPlrMsg(iPlayer, idmSWaypointAppearsHaveDestroyedHasDisappeared, lpfl->id | 0x8000, lpfl->id, lpord[iord].id, 0, 0, 0,
                                                0, 0);
                                } else {
                                    if (lpflT->fInclude != 0)
                                        continue;
                                    if (lpflT->idPlanet != -1 && fNoAutoTrack == 0) {
                                        FSendPlrMsg(iPlayer, idmFleetTrackingAppearsHaveDuckedBehindOrders, lpfl->id | 0x8000, lpfl->id, lpflT->idPlanet, 0, 0,
                                                    0, 0, 0);
                                    } else {
                                        FSendPlrMsg(iPlayer, idmFleetTrackingAppearsHaveOutrunRangeScanners, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                                    }
                                }
                                lpord[iord].grobj = grobjOther;
                                lpord[iord].id = iord;
                                if (FFindNearestObject(lpord[iord].pt, grobjPlanet | mdExact, &scan) != 0) {
                                    lpord[iord].grobj = grobjPlanet;
                                    lpord[iord].id = scan.idpl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    MarkPlayersThatSentMsgs(iPlayer);
    MarkPlanetsPlayerLost(iPlayer);
    if (iPlayer == -1) {
        _wsprintf(szWork, "%s.hst", pszFileBase);
    } else {
        _wsprintf(szWork, "%s.m%d", pszFileBase, iPlayer + 1);
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0 || ((fAppend == 0 || FAppendFile(iPlayer) == 0) && FCreateFile(iPlayer == -1 ? dtHost : dtTurn, iPlayer, NULL) == 0)) {
        idPlayer = iPlayer;
        if (fAppend != 0) {
            AlertSz(PszFormatIds(idsUnableUpdateTurnFile, NULL), MB_ICONHAND);
        } else if (iPlayer != -1) {
            AlertSz(PszFormatIds(idsUnableCreateNewTurnFile, NULL), MB_ICONHAND);
        } else {
            idPlayer = -1;
            AlertSz(PszFormatIds(idsUnableCreateHostFile, NULL), MB_ICONHAND);
        }
        idPlayer = -1;
        fRet = FALSE;
    } else {
        WriteBattles(iPlayer);
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0 || rgplr[i].fDead != 0) {
                if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raTerra) {
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 7;
                }
                WriteRtPlr(&rgplr[i], NULL);
            }
        }
        if (iPlayer == -1 && lSaltCur != 0) {
            WriteRt(rtChgPassword, 4, &lSaltCur);
        }
        WritePlayerMessages(iPlayer);
        i = 0;
        lpplT = lpPlanets;
        while (i < cPlanet) {
            if (lpplT->fInclude != 0) {
                if (lpplT->det == detAll) {
                    WritePlanet(lpplT, rtPlanet, FALSE);
                    if (lpplT->lpplprod != 0) {
                        WriteRt(rtProdQ, lpplT->lpplprod->iprodMac * 4, lpplT->lpplprod->rgprod);
                    }
                } else if (lpplT->det == detObscure) {
                    pl = *lpplT;
                    lpplT->fStarbase = FALSE;
                    lpplT->det = detSome;
                    WritePlanet(lpplT, rtPlanetB, FALSE);
                    *lpplT = pl;
                } else {
                    WritePlanet(lpplT, rtPlanetB, FALSE);
                }
            }
            i++;
            lpplT++;
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0) {
                lpshdef = rglpshdef[i];
                for (j = 0; j < 16; j++) {
                    if (lpshdef[j].fFree == 0 && lpshdef[j].fInclude != 0) {
                        WriteRtShDef(lpshdef + j, NULL);
                    }
                }
            }
        }
        for (i = 0; i < cFleet; i++) {
            lpflT = rglpfl[i];
            if (rglpfl[i] == 0)
                break;
            if (lpflT->fInclude != 0) {
                WriteFleet(lpflT);
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0) {
                lpshdef = rglpshdefSB[i];
                for (j = 0; j < 10; j++) {
                    if (lpshdef[j].fFree == 0 && lpshdef[j].fInclude != 0) {
                        WriteRtShDef(lpshdef + j, NULL);
                    }
                }
            }
        }
        if (iPlayer != -1 && vlprgScoreX != 0) {
            for (i = 0; i < game.cPlayer; i++) {
                if (gd.fGameOverMan != 0 || i == iPlayer || rgplr[i].fDead != 0 || (game.fVisScores != 0 && game.turn >= 20)) {
                    WriteRt(rtScore, 24, vlprgScoreX + i);
                }
            }
        }
        i = 0;
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (iPlayer != -1) {
                if (iPlayer == lpth->iplr) {
                    switch (lpth->ith) {
                    case ithMineralPacket:
                    case ithMysteryTrader:
                    case ithWormhole:
                        goto L_6d30;
                    default:
                        goto L_6de7;
                    }
                    continue;
                }
            L_6d30:
                if ((lpth->ith != ithMinefield || (1 << iPlayer & lpth->thm.grbitPlrNow) == 0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0) &&
                    (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0) && (lpth->ith != ithWormhole || lpth->thw.fInclude == 0))
                    continue;
            }
        L_6de7:
            i++;
        }
        if (i > 0) {
            WriteRt(rtThing, 2, &i);
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (iPlayer != -1) {
                    if (iPlayer == lpth->iplr) {
                        switch (lpth->ith) {
                        case ithMineralPacket:
                        case ithMysteryTrader:
                        case ithWormhole:
                            goto L_6eab;
                        default:
                            goto L_6f62;
                        }
                        continue;
                    }
                L_6eab:
                    if ((lpth->ith != ithMinefield || (1 << iPlayer & lpth->thm.grbitPlrNow) == 0) &&
                        (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0) && (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0) &&
                        (lpth->ith != ithWormhole || lpth->thw.fInclude == 0))
                        continue;
                }
            L_6f62:
                WriteRt(rtThing, 18, lpth);
            }
        }
        if (iPlayer == -1) {
            i = 0;
            iMax = game.cPlayer;
        } else {
            i = iPlayer;
            iMax = iPlayer + 1;
        }
        for (; i < iMax; i++) {
            lpbtlplan = rglpbtlplan[i];
            j = 0;
            while (j < rgcbtlplan[i]) {
                WriteBattlePlan(lpbtlplan, FALSE);
                j++;
                lpbtlplan++;
            }
        }
        WriteRt(rtEOF, 2, &game.turn);
        StreamClose();
    }
    SetVisiblePlanFleet(-1);
    return fRet;
}
