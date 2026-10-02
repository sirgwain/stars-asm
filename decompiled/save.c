#include "common.h"

void WriteOrders(FLEET *lpfl) {
    int16_t cord;
    ORDER  *lpord;

    if (lpfl->cord != 0) {
        cord = lpfl->cord;
        lpord = lpfl->lpplord->rgord;
        for (; cord != 0; cord--) {
            if (lpord->grTask == grTaskNone) {
                WriteRt(rtOrderB, 8, lpord);
            } else {
                WriteRt(rtOrderA, 18, lpord);
            }
            lpord++;
        }
    }
    return;
}

void WriteRtPlr(PLAYER *pplr, uint8_t *pbStore) {
    uint8_t  rgb[264];
    int16_t  i;
    uint8_t *pb;
    int16_t  cOut;

    if (pbStore == 0) {
        pbStore = rgb;
    }
    if (pplr->fDead != 0) {
        pplr->det = detAll;
    }
    memmove(pbStore, pplr, sizeof(PLAYER));
    if (pplr->det == detAll) {
        for (i = 15; i >= 0 && pplr->rgmdRelation[i] == 0; i--) {
        }
        i++;
        pb = pbStore + 112;
        *pb++ = LOBYTE(i);
        memmove(pb, pplr->rgmdRelation, i);
        pb += i;
    } else {
        pb = pbStore + 8;
    }
    cOut = 31;
    if (pplr->szName[0] != 0 && FCompressUserString(pplr->szName, pb + 1, &cOut) != 0) {
        *pb = LOBYTE(cOut);
        pb += 1 + cOut;
    } else {
        strcpy(pb + 1, pplr->szName);
        *pb = 0;
        pb += 2 + strlen(pplr->szName);
    }
    cOut = 31;
    if (pplr->szNames[0] != 0 && FCompressUserString(pplr->szNames, pb + 1, &cOut) != 0) {
        *pb = LOBYTE(cOut);
        pb += 1 + cOut;
    } else {
        strcpy(pb + 1, pplr->szNames);
        *pb = 0;
        pb += 2 + strlen(pplr->szNames);
    }
    WriteRt(rtPlr, pb - pbStore, pbStore);
    return;
}

void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore) {
    uint8_t  rgb[147];
    char     szHulName[32];
    uint8_t *pb;
    int16_t  cOut;

    ((RTSHDEF *)rgb)->ihuldef = LOBYTE(lpshdef->hul.ihuldef);
    ((RTSHDEF *)rgb)->wFlags = lpshdef->wFlags;
    ((RTSHDEF *)rgb)->chs = lpshdef->hul.chs;
    ((RTSHDEF *)rgb)->ibmp = LOBYTE(lpshdef->hul.ibmp);
    if (lpshdef->det == detAll) {
        ((RTSHDEF *)rgb)->dp = lpshdef->hul.dp;
        ((RTSHDEF *)rgb)->turn = lpshdef->turn;
        ((RTSHDEF *)rgb)->cBuilt = lpshdef->cBuilt;
        ((RTSHDEF *)rgb)->cExist = lpshdef->cExist;
        pb = (uint8_t *)&((RTSHDEF *)rgb)->rghs;
        fmemmove(pb, lpshdef->hul.rghs, ((RTSHDEF *)rgb)->chs * 4);
        pb += ((RTSHDEF *)rgb)->chs * 4;
    } else {
        ((RTSHDEF *)rgb)->wtEmpty = lpshdef->hul.wtEmpty;
        pb = &((RTSHDEF *)rgb)->chs;
    }
    if (lpshdef->det == detAll) {
        fstrcpy(szHulName, lpshdef->hul.szClass);
    } else {
        fstrcpy(szHulName, LphuldefFromId(lpshdef->hul.ihuldef)->hul.szClass);
    }
    cOut = 31;
    if (szHulName[0] != 0 && FCompressUserString(szHulName, pb + 1, &cOut) != 0) {
        *pb = LOBYTE(cOut);
        pb += 1 + cOut;
    } else {
        strcpy(pb + 1, szHulName);
        *pb = 0;
        pb += 2 + strlen(szHulName);
    }
    if (ppbStore != 0) {
        memmove(*ppbStore, rgb, pb - rgb);
        *ppbStore += pb - rgb;
    } else {
        WriteRt(rtShDef, pb - rgb, rgb);
    }
    return;
}

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

    fRet = 1;
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
                    lpfl->fDead = 1;
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
                        fFoundIdeal = 0;
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
                                    (FMatchTarget(lpflTarget, mdTarget, 0) != 0 && FAttackPlayer(lpfl, lpflTarget->iPlayer) != 0)) {
                                    lpflBest = lpflTarget;
                                    lBest = l;
                                    if (lpflTarget->fMark == 0) {
                                        fFoundIdeal = 1;
                                    }
                                }
                            }
                        }
                        if (fFoundIdeal != 0 && gd.fTutorial == 0) {
                            lpflBest->fMark = 1;
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
                                lpord[1].fValidTask = 1;
                                lpord[1].grTask = grTaskPatrol;
                                lpord[1].tptl = lpord->tptl;
                                if (lpord[1].tptl.iWarp == 0) {
                                    lpord[1].iWarp = IFindIdealWarp(lpfl, 0);
                                } else {
                                    lpord[1].iWarp = lpord[1].tsell.iPlrX;
                                }
                                if (lpfl->fRepOrders != 0) {
                                    lpord[2] = *lpord;
                                    lpord[2].iWarp = IFindIdealWarp(lpfl, 0);
                                    lpfl->cord++;
                                    lpfl->lpplord->iordMac++;
                                }
                            } else if (lpord[1].tsell.iPlrX == 0) {
                                lpord[1].iWarp = IFindIdealWarp(lpfl, 0);
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
                                    lpord[iord].fNoAutoTrack = 0;
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
        fRet = 0;
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
                    WritePlanet(lpplT, rtPlanet, 0);
                    if (lpplT->lpplprod != 0) {
                        WriteRt(rtProdQ, lpplT->lpplprod->iprodMac * 4, lpplT->lpplprod->rgprod);
                    }
                } else if (lpplT->det == detObscure) {
                    pl = *lpplT;
                    lpplT->fStarbase = 0;
                    lpplT->det = detSome;
                    WritePlanet(lpplT, rtPlanetB, 0);
                    *lpplT = pl;
                } else {
                    WritePlanet(lpplT, rtPlanetB, 0);
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
                WriteBattlePlan(lpbtlplan, 0);
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

int16_t FAppendFile(int16_t iPlayer) {
    if (FMarkFile(8195, iPlayer, mdMarkMulti, 1) == 0) {
        return 0;
    }
    WriteBOF(iPlayer, 3, 1);
    return 1;
}

void WriteBattles(int16_t iPlayer) {
    int16_t  ctok;
    int16_t  cbRec;
    PLANET  *lppl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  cbT;
    uint16_t fPlayerCur;
    BTLREC  *lpbtlrec;
    uint8_t *lpbBattle;
    HB      *lphb;
    BTLDATA *lpbtldata;
    int16_t  cb;
    int16_t  iplr;

    cbT = 0;
    if (iPlayer != -1 && lpbBattleLog != lpbBattleCur) {
        lphb = rglphb[11];
        lpbBattle = (uint8_t *)lphb + (sizeof(HB) + 2);
        fPlayerCur = 1 << iPlayer;
        while (lphb != 0) {
            for (lpbtldata = (BTLDATA *)lpbBattle; lphb->ibTop <= sizeof(HB) || lpbtldata->id == 0xffff; lpbtldata = (BTLDATA *)lpbBattle) {
                lphb = lphb->lphbNext;
                if (lphb == 0) {
                    return;
                }
                lpbBattle = (uint8_t *)lphb + (sizeof(HB) + 2);
            }
            if ((lpbtldata->grfPlr & fPlayerCur) != 0) {
                for (i = 0; i < game.cPlayer; i++) {
                    if (i != iPlayer && rgplr[i].fInclude == 0 && (1 << i & lpbtldata->grfPlr) != 0) {
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 3;
                    }
                }
                for (i = 0; i < lpbtldata->ctok; i++) {
                    if (lpbtldata->rgtok[i].iplr != iPlayer) {
                        if (lpbtldata->rgtok[i].grobj == grobjPlanet) {
                            iplr = lpbtldata->rgtok[i].iplr;
                            lppl = LpplFromId(lpbtldata->rgtok[i].id);
                            if (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].fInclude == 0) {
                                rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].wFlags =
                                    (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 0x10].wFlags & 0xfeff) | 0x100;
                                rgplr[iplr].cshdefSB++;
                            }
                            rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].wFlags =
                                (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 0x10].wFlags & 0xff00) | 7;
                        } else {
                            lpfl = LpflFromId(lpbtldata->rgtok[i].id);
                            if (lpfl->iPlayer != iPlayer && rgplr[lpfl->iPlayer].fInclude == 0) {
                                rgplr[lpfl->iPlayer].wMdPlr = (rgplr[lpfl->iPlayer].wMdPlr & 0xfeff) | 0x100;
                                rgplr[lpfl->iPlayer].wMdPlr = (rgplr[lpfl->iPlayer].wMdPlr & 0xfff8) | 3;
                            }
                            if (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].fInclude == 0) {
                                rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags =
                                    (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xfeff) | 0x100;
                                rgplr[lpfl->iPlayer].cShDef = rgplr[lpfl->iPlayer].cShDef + 1;
                            }
                            rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags =
                                (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xff00) | 7;
                            if (lpfl->fDead == 0) {
                                if (lpfl->fInclude == 0) {
                                    rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 1;
                                    lpfl->fInclude = 1;
                                    lpfl->det = detNone;
                                }
                                if (lpfl->det < detSome) {
                                    lpfl->det = detSome;
                                }
                            }
                        }
                    }
                }
                if (lpbtldata->idPlanet != 0xffff) {
                    lppl = LpplFromId(lpbtldata->idPlanet);
                    MarkPlanet(lppl, iPlayer, detMinimal);
                }
                if (lpbtldata->cbData < 0x400) {
                    WriteRt(rtBtlData, lpbtldata->cbData, lpbBattle);
                    lpbBattle += lpbtldata->cbData;
                } else {
                    cb = lpbtldata->ctok * 29 + 14;
                    lpbtlrec = (BTLREC *)(lpbBattle + cb);
                    if (cb < 1024) {
                        WriteRt(rtBtlData, cb, lpbBattle);
                        lpbBattle += cb;
                    } else {
                        ctok = 34;
                        ctok = ctok >= lpbtldata->ctok ? lpbtldata->ctok : ctok;
                        if (ctok > lpbtldata->ctok) {
                            ctok = lpbtldata->ctok;
                        }
                        WriteRt(rtBtlData, ctok * 29 + 14, lpbBattle);
                        lpbBattle += 14 + 29 * ctok;
                        ctok = lpbtldata->ctok - ctok;
                        while (ctok > 0) {
                            if ((uint16_t)ctok > 35) {
                                WriteRt(rtContinue, 1015, lpbBattle);
                                lpbBattle += 1015;
                                ctok -= 35;
                            } else {
                                WriteRt(rtContinue, ctok * 29, lpbBattle);
                                lpbBattle += 29 * ctok;
                                ctok = 0;
                            }
                        }
                    }
                    cb = lpbtldata->cbData - 14 - lpbtldata->ctok * 29;
                    if (cb < 1024) {
                        WriteRt(rtContinue, cb, lpbtlrec);
                        lpbBattle += cb;
                    } else {
                        while (cb != 0) {
                            cbRec = lpbtlrec->ctok * 8 + 6;
                            if (cbRec >= 1024) {
                                cb -= cbRec;
                                for (; cbRec >= 1024; cbRec -= 1023) {
                                    WriteRt(rtContinue, 1023, lpbtlrec);
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + 0x3ff);
                                }
                                if (cbRec != 0) {
                                    WriteRt(rtContinue, cbRec, lpbtlrec);
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + cbRec);
                                }
                            } else {
                                cbT = 0;
                                do {
                                    cbT += cbRec;
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + cbRec);
                                    cb -= cbRec;
                                    if (cb != 0) {
                                        cbRec = lpbtlrec->ctok * 8 + 6;
                                    }
                                } while (cb != 0 && cbT + cbRec < 1024);
                                WriteRt(rtContinue, cbT, lpbBattle);
                            }
                            lpbBattle = (uint8_t *)lpbtlrec;
                        }
                    }
                }
            } else {
                lpbBattle += lpbtldata->cbData;
            }
        }
    }
    return;
}

void WritePlanet(PLANET *lppl, RecordType rt, int16_t fHistory) {
    uint8_t  bMask;
    uint8_t  rgb[80];
    uint8_t *pbBase;
    int16_t  i;
    uint8_t *pb;

    memset(rgb, 0, 80);
    ((RTPLANET *)rgb)->id = lppl->id;
    ((RTPLANET *)rgb)->iPlayer = lppl->iPlayer;
    ((RTPLANET *)rgb)->det = lppl->det;
    if (rt == rtPlanetB && lppl->det > detSome) {
        ((RTPLANET *)rgb)->det = fHistory == 0 ? 4 : 3;
    }
    ((RTPLANET *)rgb)->fInclude = lppl->fInclude;
    ((RTPLANET *)rgb)->fStarbase = lppl->fStarbase;
    ((RTPLANET *)rgb)->fHomeworld = lppl->fHomeworld;
    ((RTPLANET *)rgb)->fFirstYear = lppl->fFirstYear;
    ((RTPLANET *)rgb)->fRouting = lppl->idRoute == 0 ? 0 : 1;
    pbBase = (uint8_t *)(((RTPLANET *)rgb) + 1);
    pb = pbBase;
    if (((RTPLANET *)rgb)->det > detMinimal) {
        pb = pbBase + 1;
        bMask = 3;
        i = 0;
        while (i < 3) {
            if (lppl->rgpctMinLevel[i] > 0) {
                *pbBase |= LOBYTE(bMask & 0x55);
                *pb++ = lppl->rgpctMinLevel[i];
            }
            i++;
            bMask = LOBYTE(bMask * 4);
        }
        i = 0;
        while (i < 3) {
            *pb = lppl->rgMinConc[i];
            i++;
            pb++;
        }
        for (i = 0; i < 3; i++) {
            *pb++ = lppl->rgEnvVar[i];
            if (lppl->rgEnvVar[i] != lppl->rgEnvVarOrig[i]) {
                ((RTPLANET *)rgb)->fIncEVO = 1;
            }
        }
        if (((RTPLANET *)rgb)->fIncEVO != 0) {
            for (i = 0; i < 3; i++) {
                *pb++ = lppl->rgEnvVarOrig[i];
            }
        }
        if (lppl->iPlayer != -1) {
            RawStore16(pb, lppl->uGuesses);
            pb += 2;
        }
        if (((RTPLANET *)rgb)->det > detSome) {
            pbBase = pb;
            pb++;
            bMask = 3;
            i = 0;
            while (i < 4) {
                if ((i != 3 || lppl->det >= detAll) && lppl->rgwtMin[i] > 0) {
                    if (lppl->rgwtMin[i] <= 255) {
                        *pbBase |= LOBYTE(bMask & 0x55);
                        *pb++ = LOBYTE(LOWORD(lppl->rgwtMin[i]));
                    } else if (lppl->rgwtMin[i] > 65535) {
                        *pbBase |= LOBYTE(bMask & 0xff);
                        RawStore16(pb, LOWORD(lppl->rgwtMin[i]));
                        RawStore16((uint8_t *)pb + 0x2, HIWORD(lppl->rgwtMin[i]));
                        pb += 4;
                    } else {
                        *pbBase |= LOBYTE(bMask & 0xaa);
                        RawStore16(pb, LOWORD(lppl->rgwtMin[i]));
                        pb += 2;
                    }
                }
                i++;
                bMask = LOBYTE(bMask * 4);
            }
            if (*pbBase == 0) {
                pb = pbBase;
            } else {
                ((RTPLANET *)rgb)->fIncSurfMin = 1;
            }
            if (rt != rtPlanetB) {
                ((RTPLANET *)rgb)->fIsArtifact = lppl->fArtifact;
                if ((lppl->iPlayer != -1 && (lppl->iDeltaPop != 0 || lppl->fNoResearch != 0)) ||
                    (lppl->cMines != 0 || lppl->cFactories != 0 || lppl->cDefenses != 0 || lppl->iScanner != 31)) {
                    ((RTPLANET *)rgb)->fIncImp = 1;
                    fmemmove(pb, lppl->rgbImp, 8);
                    pb += 8;
                }
                if (lppl->iPlayer != -1) {
                    if (lppl->fStarbase != 0) {
                        RawStore16(pb, lppl->isb | lppl->pctDp << 4);
                        RawStore16((uint8_t *)pb + 0x2, lppl->idFling | lppl->iWarpFling << 0xa | lppl->fNoHeal << 0xe | lppl->unused3 << 0xf);
                        pb += 4;
                    }
                    if (lppl->idRoute != 0) {
                        RawStore16(pb, lppl->wRouting);
                        pb += 2;
                    }
                }
                WriteRt(rtPlanet, pb - rgb, rgb);
                return;
            }
        }
    }
    if (lppl->fStarbase != 0) {
        *pb = LOBYTE(lppl->isb);
        pb++;
    }
    if (fHistory != 0) {
        RawStore16(pb, lppl->turn);
        pb += 2;
    }
    WriteRt(rtPlanetB, pb - rgb, rgb);
    return;
}

void WriteFleet(FLEET *lpfl) {
    uint16_t *pus;
    uint8_t   rgb[134];
    uint16_t  us;
    int16_t   i;
    uint8_t  *pb;
    int16_t   fByte;
    uint16_t  grMask;
    int32_t   wt;

    fmemmove(rgb, lpfl, 12);
    fByte = 1;
    grMask = 1;
    us = 0;
    i = 0;
    while (i < 16) {
        if (lpfl->rgcsh[i] > 0) {
            us |= grMask;
            if (lpfl->rgcsh[i] > 255) {
                fByte = 0;
            }
        }
        i++;
        grMask *= 2;
    }
    RawStore16(&rgb[4], (RawLoad16(&rgb[4]) & 0xf7ff) | (fByte & 1) << 0xb);
    RawStore16(&rgb[12], us);
    pb = &rgb[14];
    if (fByte != 0) {
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                *pb++ = LOBYTE(lpfl->rgcsh[i]);
            }
        }
    } else {
        pus = (uint16_t *)pb;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                *pus++ = lpfl->rgcsh[i];
            }
        }
        pb = (uint8_t *)pus;
    }
    if (lpfl->det >= detMore) {
        fByte = 0;
        grMask = 3;
        pus = (uint16_t *)pb;
        pb += 2;
        us = 0;
        i = 0;
        while (i < 5) {
            if (lpfl->rgwtMin[i] > 0 && (lpfl->det == detAll || (i != 4 && i != 3))) {
                if (lpfl->rgwtMin[i] <= 255) {
                    us |= grMask & 0x155;
                    *pb = LOBYTE(LOWORD(lpfl->rgwtMin[i]));
                    pb++;
                } else if (lpfl->rgwtMin[i] > 65535) {
                    us |= grMask & 0x3ff;
                    RawStore16(pb, LOWORD(lpfl->rgwtMin[i]));
                    RawStore16((uint8_t *)pb + 0x2, HIWORD(lpfl->rgwtMin[i]));
                    pb += 4;
                } else {
                    us |= grMask & 0x2aa;
                    RawStore16(pb, LOWORD(lpfl->rgwtMin[i]));
                    pb += 2;
                }
            }
            i++;
            grMask *= 4;
        }
        *pus = us;
    }
    if (lpfl->det < detAll) {
        wt = 0;
        RawStore16(pb, lpfl->dirFltX | lpfl->dirFltY << 8);
        RawStore16((uint8_t *)pb + 0x2,
                   lpfl->iwarpFlt | lpfl->fdirValid << 4 | lpfl->fCompChg << 5 | lpfl->fTargeted << 6 | lpfl->fSkipped << 7 | lpfl->fUnused << 8);
        pb += 4;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                wt += (uint32_t)(lpfl->rgcsh[i] * (uint32_t)rglpshdef[lpfl->iPlayer][i].hul.wtEmpty);
            }
        }
        for (i = 0; i <= 3; i++) {
            wt += lpfl->rgwtMin[i];
        }
        RawStore16(pb, LOWORD(wt));
        RawStore16((uint8_t *)pb + 0x2, HIWORD(wt));
        pb += 4;
        WriteRt(rtFleetB, pb - rgb, rgb);
    } else {
        grMask = 1;
        us = 0;
        i = 0;
        while (i < 16) {
            if (lpfl->rgdv[i].dp != 0) {
                us |= grMask;
            }
            i++;
            grMask *= 2;
        }
        RawStore16(pb, us);
        pb += 2;
        pus = (uint16_t *)pb;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgdv[i].dp != 0) {
                *pus++ = lpfl->rgdv[i].dp;
            }
        }
        pb = (uint8_t *)pus;
        *pb++ = lpfl->iplan;
        *pb++ = LOBYTE(lpfl->cord);
        WriteRt(rtFleetA, pb - rgb, rgb);
        WriteOrders(lpfl);
        if (lpfl->lpszName != 0) {
            WriteRtString(lpfl->lpszName);
        }
    }
    return;
}

void WriteRtString(char *lpsz) {
    uint8_t rgb[33];
    int16_t cOut;

    if (lpsz != 0 && *lpsz != 0) {
        cOut = 31;
        if (FCompressUserString(lpsz, &rgb[1], &cOut) != 0) {
            rgb[0] = LOBYTE(cOut);
        } else {
            fstrcpy(&rgb[1], lpsz);
            rgb[0] = 0;
            cOut = fstrlen(lpsz) + 1;
        }
        WriteRt(rtString, cOut + 1, rgb);
    }
    return;
}

void MarkFleet(FLEET *lpfl, DetType det) {
    int16_t i;
    SHDEF  *lpshdef;

    if (lpfl->fInclude == 0) {
        lpshdef = rglpshdef[lpfl->iPlayer];
        lpfl->fInclude = 1;
        lpfl->det = detNone;
        lpfl->fdirValid = 1;
        rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 1;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] != 0) {
                lpshdef[i].wFlags = (lpshdef[i].wFlags & 0xfeff) | 0x100;
            }
        }
    }
    if (lpfl->det < det) {
        lpfl->det = det;
    }
    return;
}

void WriteBattlePlan(BTLPLAN *lpbtlplan, int16_t fLog) {
    uint8_t  rgb[36];
    uint8_t *pb;
    char     szPlanName[32];
    int16_t  cOut;

    fmemmove(rgb, lpbtlplan, 4);
    if (lpbtlplan->fDelete != 0) {
        pb = &rgb[2];
    } else {
        pb = &rgb[4];
        fstrcpy(szPlanName, lpbtlplan->szName);
        cOut = 31;
        if (szPlanName[0] != 0 && FCompressUserString(szPlanName, pb + 1, &cOut) != 0) {
            *pb = LOBYTE(cOut);
            pb += 1 + cOut;
        } else {
            strcpy(pb + 1, szPlanName);
            *pb = 0;
            pb += 2 + strlen(szPlanName);
        }
    }
    if (fLog != 0) {
        WriteMemRt(rtBtlPlan, pb - rgb, rgb);
    } else {
        WriteRt(rtBtlPlan, pb - rgb, rgb);
    }
    return;
}

void MarkPlanet(PLANET *lppl, int16_t iPlr, DetType det) {
    SHDEF *lpshdef;

    if (lppl->fInclude == 0) {
        lppl->fInclude = 1;
        lppl->det = detNone;
        rgplr[iPlr].cPlanet++;
    }
    if (lppl->det < det) {
        lppl->det = det;
    }
    if (lppl->iPlayer != -1 && rgplr[lppl->iPlayer].fInclude == 0) {
        rgplr[lppl->iPlayer].wMdPlr = (rgplr[lppl->iPlayer].wMdPlr & 0xfeff) | 0x100;
        rgplr[lppl->iPlayer].wMdPlr = (rgplr[lppl->iPlayer].wMdPlr & 0xfff8) | 3;
    }
    if (det != detObscure && lppl->iPlayer != -1 && lppl->fStarbase != 0) {
        lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
        if (lpshdef->fInclude == 0) {
            lpshdef->fInclude = 1;
            lpshdef->det = detNone;
            rgplr[lppl->iPlayer].cshdefSB = rgplr[lppl->iPlayer].cshdefSB + 1;
        }
        if (lpshdef->det < detSome) {
            lpshdef->det = detSome;
        }
    }
    return;
}

void SetSzWorkFromDt(DtFileType dt, int16_t iPlayer) {
    char   *pchSlash;
    int16_t c;
    char   *pchDot;

    pchDot = strrchr(szBase, 46);
    if (pchDot != 0) {
        pchSlash = strrchr(szBase, 92);
        if (pchSlash == 0 || pchSlash < pchDot) {
            *pchDot = 0;
        }
    }
    c = _wsprintf(szWork, "%s.", szBase);
    switch (dt) {
    case dtXY:
    default:
        strcat(szWork, "xy");
        break;
    case dtHost:
        strcat(szWork, "hst");
        break;
    case dtLog:
    case dtTurn:
    case dtHist:
        _wsprintf(&szWork[c], "%c%d", dt == dtLog ? 120 : dt == dtHist ? 104 : 109, iPlayer + 1);
    }
    return;
}

int16_t FCreateFile(DtFileType dt, int16_t iPlayer, char *szForceName) {
    jmp_buf *penvMemSav;
    jmp_buf  env;
    char    *psz;

    if (szForceName != 0) {
        psz = szForceName;
    } else {
        SetSzWorkFromDt(dt, iPlayer);
        psz = szWork;
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        penvMem = penvMemSav;
        return 0;
    }
    StreamOpen(psz, mdCreate);
    WriteBOF(iPlayer, dt, 0);
    penvMem = penvMemSav;
    return 1;
}

void WriteBOF(int16_t iPlayer, int16_t dt, int16_t fMulti) {
    RTBOF   rtbof;
    int16_t t_scratch_m16;

    memset(&rtbof, 0, sizeof(RTBOF));
    strncpy(rtbof.rgid, "J3J3", 4);
    rtbof.lidGame = game.lid;
    rtbof.wGen = game.wGen;
    rtbof.verInc = 0;
    rtbof.verMinor = 83;
    rtbof.verMajor = 2;
    rtbof.turn = game.turn;
    rtbof.fCrippled = 0;
    rtbof.iPlayer = iPlayer;
    t_scratch_m16 = Random(2000);
    rtbof.lSaltTime = (int16_t)(LOWORD(GetTickCount()) + t_scratch_m16);
    rtbof.dt = dt;
    rtbof.fDone = gd.fSubmit;
    rtbof.fInUse = gd.fHostMode;
    rtbof.fGameOverMan = dt == 2 && gd.fGameOverMan != 0;
    WriteRt(rtBOF, 16, &rtbof);
    return;
}

int16_t FMarkFile(DtFileType dt, int16_t iPlayer, MdMark mdMark, int16_t f) {
    StringId ids;
    RTBOF    rtbof;
    jmp_buf *penvMemSav;
    jmp_buf  env;
    int16_t  fChange;
    int16_t  fSuccess;
    int16_t  fSilentSav;
    int32_t  lSeedSav2;
    int32_t  lSeedSav1;

    fSilentSav = fFileErrSilent;
    fSuccess = 0;
    ids = idsUniverseDefinitionFileSeemsMissingCorrupt;
    SetSzWorkFromDt(dt & 0xff, iPlayer);
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        fFileErrSilent = fSilentSav;
        if (ids != idsUniverseDefinitionFileSeemsMissingCorrupt) {
            FileError(ids);
        }
        StreamClose();
        penvMem = penvMemSav;
        return 0;
    }
    fFileErrSilent = 1;
    StreamOpen(szWork, mdReadWrite);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if (hdrCur.rt != rtBOF) {
        FileError(idmColonistsDroppedDestroyedSpiritedFighting);
    } else if (((RTBOF *)rgbCur)->verMajor < 2 || (((RTBOF *)rgbCur)->verMajor == 2 && ((RTBOF *)rgbCur)->verMinor < 49)) {
        FileError(1235);
    } else if (((RTBOF *)rgbCur)->verMajor > 2 || (((RTBOF *)rgbCur)->verMajor == 2 && ((RTBOF *)rgbCur)->verMinor >= 84)) {
        FileError(714);
    } else {
        rtbof = *((RTBOF *)rgbCur);
        if (game.lid != 0) {
            if (rtbof.lidGame != game.lid) {
                FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
            } else {
                fChange = 0;
                switch (mdMark) {
                case mdMarkInUse:
                    if (rtbof.fInUse == f)
                        goto L_93f5;
                    rtbof.fInUse = f;
                    fChange = 1;
                    goto L_93f5;
                case mdMarkDone:
                    if (rtbof.fDone == f)
                        goto L_93f5;
                    rtbof.fDone = f;
                    fChange = 1;
                    goto L_93f5;
                case mdMarkMulti:
                    if (rtbof.fMulti == f)
                        goto L_93f5;
                    rtbof.fMulti = f;
                    fChange = 1;
                    goto L_93f5;
                case mdMarkAi:
                    do {
                        GetFileSeeds(&lSeedSav1, &lSeedSav2);
                        ReadRt();
                    } while (hdrCur.rt != rtPlr || ((PLAYER *)rgbCur)->iPlayer != iPlayer);
                    if (((PLAYER *)rgbCur)->fAi != f) {
                        if (((PLAYER *)rgbCur)->fAi != 0) {
                            if (((PLAYER *)rgbCur)->idAi != idAiMaid)
                                break;
                            ((PLAYER *)rgbCur)->fAi = 0;
                        } else {
                            ((PLAYER *)rgbCur)->fAi = 1;
                            ((PLAYER *)rgbCur)->idAi = idAiMaid;
                        }
                        ((PLAYER *)rgbCur)->lSalt = ~((PLAYER *)rgbCur)->lSalt;
                        lseek(hf, (int16_t)-(hdrCur.cb + 2), 1);
                        SetFileSeeds(lSeedSav1, lSeedSav2);
                        WriteRt(rtPlr, hdrCur.cb, rgbCur);
                        fChange = dt == dtTurn ? 1 : 0;
                        rtbof.fDone = 0;
                    }
                default:
                L_93f5:
                    if (fChange != 0) {
                        lseek(hf, 0, 0);
                        WriteRt(rtBOF, 16, &rtbof);
                    }
                    fSuccess = 1;
                }
            }
        }
    }
    if ((dt & 0x2000) != 0 && fSuccess != 0) {
        lseek(hf, 0, 2);
    } else {
        StreamClose();
    }
    penvMem = penvMemSav;
    return fSuccess;
}

void WriteRt(RecordType rt, int16_t cb, void *rg) {
    HDR hdr;

    fmemmove(rgbCur, rg, cb);
    if (rt == rtBOF) {
        SetFileXorStream(((RTBOF *)rgbCur)->lidGame, ((RTBOF *)rgbCur)->lSaltTime, ((RTBOF *)rgbCur)->turn, ((RTBOF *)rgbCur)->iPlayer,
                         ((RTBOF *)rgbCur)->fCrippled);
    } else if (rt != rtEOF) {
        XorFileBuf(rgbCur, cb);
    }
    hdr.cb = cb;
    hdr.rt = rt;
    RgToStream(&hdr, 2);
    RgToStream(rgbCur, cb);
    return;
}

void RgToStream(void *rg, uint16_t cb) {
    if (cb != 0 && _lwrite(hf, rg, cb) != cb) {
        AlertSz(PszFormatIds(idsErrorWritingFile, NULL), MB_ICONHAND);
        StarsLongJump(penvMem, -1);
    }
    return;
}

void SetVisiblePlanFleet(int16_t iPlr) {
    SetVisPFInit(iPlr);
    if (iPlr == -1) {
        rgplr[0].cPlanet = game.cPlanMax;
    } else {
        if (iPlr != -1) {
            UpdateProgressGauge(progressStep1);
        }
        SetVisPFFleets(iPlr);
        if (iPlr != -1) {
            UpdateProgressGauge(progressStep1);
        }
        SetVisPFPlanets(iPlr);
        if (iPlr != -1) {
            UpdateProgressGauge(progressStep1);
        }
        SetVisPFThings(iPlr);
        SetVisPFFinish(iPlr);
    }
    return;
}

void SetVisPFInit(int16_t iPlr) {
    PLANET       *lpplMac;
    uint16_t      detNew;
    PLANET       *lppl;
    int16_t       j;
    FLEET        *lpfl;
    THING        *lpth;
    int16_t       ifl;
    int16_t       i;
    THING        *lpthMac;
    RaceAttribute raMajor;
    uint16_t      grbitPlr;
    int16_t       iSteal;

    raMajor = GetRaceStat(&rgplr[iPlr], rsMajorAdv);
    grbitPlr = iPlr == -1 ? 0 : 1 << iPlr;
    for (i = 0; i < game.cPlayer; i++) {
        rgplr[i].cPlanet = 0;
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (iPlr == -1 || iPlr == i || rgplr[i].fDead != 0) {
            rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
            rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 7;
        } else {
            rgplr[i].wMdPlr &= 0xfeff;
        }
        rgplr[i].cFleet = 0;
        rgplr[i].cShDef = 0;
        rgplr[i].cshdefSB = 0;
        for (j = 0; j < 16; j++) {
            if ((iPlr == -1 || iPlr == i) && rglpshdef[i][j].fFree == 0) {
                rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfeff) | 0x100;
                rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | 7;
                rglpshdef[i][j].cExist = 0;
                rgplr[i].cShDef++;
            } else {
                rglpshdef[i][j].wFlags &= 0xfeff;
            }
        }
        for (j = 0; j < 10; j++) {
            if ((iPlr == -1 || iPlr == i) && rglpshdefSB[i][j].fFree == 0) {
                rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100;
                rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | 7;
                rglpshdefSB[i][j].cExist = 0;
                rgplr[i].cshdefSB++;
            } else {
                rglpshdefSB[i][j].wFlags &= 0xfeff;
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (iPlr == -1 || iPlr == lppl->iPlayer) {
            lppl->fInclude = 1;
            lppl->det = detAll;
            if (iPlr != -1) {
                rgplr[iPlr].cPlanet++;
            }
            if (lppl->fStarbase != 0) {
                rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist + 1;
            }
        } else {
            lppl->fInclude = 0;
        }
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        lpfl->fdirValid = 0;
        lpfl->fMark = 0;
        if ((iPlr == -1 || iPlr == lpfl->iPlayer) && lpfl->fDead == 0) {
            lpfl->fInclude = 1;
            lpfl->det = detAll;
            rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 1;
            for (j = 0; j < 16; j++) {
                if (lpfl->rgcsh[j] != 0) {
                    rglpshdef[lpfl->iPlayer][j].cExist = rglpshdef[lpfl->iPlayer][j].cExist + lpfl->rgcsh[j];
                }
            }
            if (iPlr != -1 && lpfl->idPlanet != -1) {
                lppl = lpPlanets + lpfl->idPlanet;
                detNew = GetCachedFleetScannerRange(lpfl, NULL, NULL, &iSteal) < 0 ? 1 : 3;
                if (iSteal >= 2) {
                    detNew = 4;
                }
                if (lpfl->fHereAllTurn != 0 && lppl->iPlayer == -1 && lpfl->lpplord->rgord[0].grTask == grTaskMine && CMineFromLpfl(lpfl) > 0) {
                    detNew = 4;
                }
                MarkPlanet(lppl, iPlr, detNew);
            }
        } else {
            lpfl->fInclude = 0;
        }
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        switch (lpth->ith) {
        case ithMineralPacket:
            if (iPlr == -1 || raMajor == raMassAccel) {
                lpth->thp.fInclude = 1;
                if (rgplr[lpth->iplr].fInclude != 0)
                    break;
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 3;
                break;
            }
            lpth->thp.fInclude = 0;
            break;
        case ithWormhole:
            lpth->thw.fInclude = iPlr == -1 ? 1 : 0;
            break;
        case ithMysteryTrader:
            lpth->tht.fInclude = 1;
            break;
        case ithMinefield:
            if ((lpth->thm.grbitPlrNow & grbitPlr) != 0 && rgplr[lpth->iplr].fInclude == 0) {
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 3;
            }
        }
    }
    return;
}

void SetVisPFFleets(int16_t iPlr) {
    PLANET  *lpplMac;
    POINT16  pt;
    int16_t  pctCloak;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  j;
    FLEET   *lpfl;
    THING   *lpth;
    int32_t  lRadius2;
    int16_t  ifl;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  dx;
    uint16_t grbitPlr;
    int32_t  lRadPlanet2;
    int16_t  iRadPlanet;
    int16_t  iSteal;
    int16_t  pctDetect;
    int32_t  l;
    int32_t  lVis2;

    grbitPlr = iPlr == -1 ? 0 : 1 << iPlr;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->fDead == 0) {
            if (lpfl->iPlayer != iPlr) {
                if (lpfl->fInclude == 0 && lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].iPlayer == iPlr) {
                    MarkFleet(lpfl, detSome);
                }
            } else {
                if (lpfl->fBombed != 0 && lpfl->idPlanet != -1) {
                    MarkPlanet(lpPlanets + lpfl->idPlanet, iPlr, detSome);
                }
                iRadius = GetCachedFleetScannerRange(lpfl, &iRadPlanet, &pctDetect, &iSteal);
                iRadius = 0 <= iRadius ? iRadius : 0;
                lRadius2 = (uint32_t)(iRadius * iRadius);
                lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
                pt = lpfl->pt;
                for (j = 0; j < cFleet; j++) {
                    lpfl2 = rglpfl[j];
                    if (rglpfl[j] == 0)
                        break;
                    if (lpfl2->fDead == 0) {
                        if ((iSteal & 1) != 0 && pt.x == lpfl2->pt.x && pt.y == lpfl2->pt.y && (lpfl2->fInclude == 0 || lpfl2->det < detMore)) {
                            MarkFleet(lpfl2, detMore);
                        }
                        if (lpfl2->fInclude == 0) {
                            dx = abs(pt.x - lpfl2->pt.x);
                            if (dx <= iRadius) {
                                dy = abs(pt.y - lpfl2->pt.y);
                                if (dy <= iRadius) {
                                    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                    if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2 && (lpfl2->idPlanet == -1 || l <= lRadPlanet2)) {
                                        pctCloak = PctCloakFromLpfl(lpfl2);
                                        if (pctDetect != 100) {
                                            pctCloak = (int16_t)(pctCloak * pctDetect) / 100;
                                        }
                                        if (pctCloak == 0) {
                                            MarkFleet(lpfl2, detSome);
                                        } else if (l <= (int32_t)(lRadius2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100 &&
                                                   (lpfl2->idPlanet == -1 ||
                                                    l <= (int32_t)(lRadPlanet2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100)) {
                                            MarkFleet(lpfl2, detSome);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                lpth = lpThings;
                lpthMac = lpThings + cThing;
                for (; lpth < lpthMac; lpth++) {
                    if (iPlr != -1) {
                        switch (lpth->ith) {
                        case ithMinefield:
                        case ithMineralPacket:
                        case ithMysteryTrader:
                        case ithWormhole:
                            if ((lpth->ith != ithMinefield || (lpth->thm.grbitPlrNow & grbitPlr) == 0) &&
                                (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0) &&
                                (lpth->ith != ithWormhole || lpth->thw.fInclude == 0)) {
                                dx = abs(pt.x - lpth->pt.x);
                                dy = abs(pt.y - lpth->pt.y);
                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if (l <= lRadius2 || lpth->ith == ithMinefield) {
                                    switch (lpth->ith) {
                                    case ithMineralPacket:
                                        lpth->thp.fInclude = 1;
                                        goto LThIncPlr;
                                    case ithMysteryTrader:
                                        lpth->tht.fInclude = 1;
                                        break;
                                    case ithWormhole:
                                        if ((lpth->thw.grbitPlr & grbitPlr) == 0 && l > (int32_t)(lRadius2 >> 4) && l > lRadPlanet2)
                                            break;
                                        lpth->thw.grbitPlr |= grbitPlr;
                                        lpth->thw.fInclude = 1;
                                        break;
                                    default:
                                        if (((lpth->thm.grbitPlr & grbitPlr) != 0 && l <= lRadius2) || l <= lRadPlanet2 || l <= (int32_t)(lRadius2 >> 4) ||
                                            l <= lpth->thm.cMines) {
                                            lpth->thm.grbitPlr |= grbitPlr;
                                            lpth->thm.grbitPlrNow |= grbitPlr;
                                            goto LThIncPlr;
                                        }
                                    }
                                    break;
                                LThIncPlr:
                                    if (rgplr[lpth->iplr].fInclude == 0) {
                                        rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                                        rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 3;
                                    }
                                }
                            }
                        }
                    }
                }
                if ((iSteal & 2) != 0 && lpfl->idPlanet != -1) {
                    MarkPlanet(lpPlanets + lpfl->idPlanet, iPlr, detMore);
                }
                if (iRadPlanet > 0) {
                    iRadius = iRadPlanet;
                    lRadius2 = (uint32_t)(iRadius * iRadius);
                    pt = lpfl->pt;
                    lppl = lpPlanets;
                    lpplMac = lpPlanets + cPlanet;
                    for (; lppl < lpplMac; lppl++) {
                        if (lppl->fInclude == 0 || lppl->det < detSome) {
                            dx = abs(rgptPlan[lppl->id].x - pt.x);
                            if (dx <= iRadius) {
                                dy = abs(rgptPlan[lppl->id].y - pt.y);
                                if (dy <= iRadius) {
                                    d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                    if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                        if (lppl->fStarbase != 0 && lppl->iPlayer != -1) {
                                            lVis2 = rglpshdefSB[lppl->iPlayer][lppl->isb].lVisible;
                                            if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000) {
                                                MarkPlanet(lppl, iPlr, detObscure);
                                                continue;
                                            }
                                        }
                                        MarkPlanet(lppl, iPlr, detSome);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

void SetVisPFPlanets(int16_t iPlr) {
    int32_t  lRadPlanet2;
    int16_t  iRadPlanet;
    PLANET  *lpplMac;
    POINT16  pt;
    int16_t  pctCloak;
    PLANET  *lppl2;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    PLANET  *lppl;
    int16_t  j;
    THING   *lpth;
    int32_t  lRadius2;
    int16_t  i;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  fStargateView;
    int16_t  dx;
    int32_t  l;
    PLANET  *lpplMac2;
    uint16_t grbitPlr;
    int16_t  rgStargateRange[16];
    int32_t  lVis2;

    grbitPlr = iPlr == -1 ? 0 : 1 << iPlr;
    fStargateView = 0;
    if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raStargate) {
        for (i = 0; i < 10; i++) {
            rgStargateRange[i] = 0;
            if (rglpshdefSB[iPlr][i].fFree == 0) {
                rgStargateRange[i] = StargateRangeFromLppl(NULL, iPlr, i);
                if (rgStargateRange[i] > 0) {
                    fStargateView = 1;
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            for (j = 0; j < cFleet; j++) {
                lpfl2 = rglpfl[j];
                if (rglpfl[j] == 0)
                    break;
                if (lpfl2->fInclude == 0 && lpfl2->fDead == 0) {
                    dx = abs(pt.x - lpfl2->pt.x);
                    if (dx <= iRadius) {
                        dy = abs(pt.y - lpfl2->pt.y);
                        if (dy <= iRadius) {
                            l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                            if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2 && (lpfl2->idPlanet == -1 || l <= lRadPlanet2)) {
                                pctCloak = PctCloakFromLpfl(lpfl2);
                                if (pctCloak == 0) {
                                    MarkFleet(lpfl2, detSome);
                                } else if (l <= (int32_t)(lRadius2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100 &&
                                           (lpfl2->idPlanet == -1 ||
                                            l <= (int32_t)(lRadPlanet2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100)) {
                                    MarkFleet(lpfl2, detSome);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (iPlr != -1) {
                    switch (lpth->ith) {
                    case ithMinefield:
                    case ithMineralPacket:
                    case ithMysteryTrader:
                    case ithWormhole:
                        if ((lpth->ith != ithMinefield || (lpth->thm.grbitPlrNow & grbitPlr) == 0) &&
                            (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0) &&
                            (lpth->ith != ithWormhole || lpth->thw.fInclude == 0)) {
                            dx = abs(pt.x - lpth->pt.x);
                            if (dx <= iRadius) {
                                dy = abs(pt.y - lpth->pt.y);
                                if (dy <= iRadius) {
                                    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                    if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                        switch (lpth->ith) {
                                        case ithMineralPacket:
                                            lpth->thp.fInclude = 1;
                                            goto LThIncPlr2;
                                        case ithMysteryTrader:
                                            lpth->tht.fInclude = 1;
                                            break;
                                        case ithWormhole:
                                            if ((lpth->thw.grbitPlr & grbitPlr) == 0 && l > (int32_t)(lRadius2 >> 4) && l > lRadPlanet2)
                                                break;
                                            lpth->thw.grbitPlr |= grbitPlr;
                                            lpth->thw.fInclude = 1;
                                            break;
                                        default:
                                            if ((lpth->thm.grbitPlr & grbitPlr) != 0 || l <= lRadPlanet2 || l <= (int32_t)(lRadius2 >> 4)) {
                                                lpth->thm.grbitPlr |= grbitPlr;
                                                lpth->thm.grbitPlrNow |= grbitPlr;
                                                goto LThIncPlr2;
                                            }
                                        }
                                        break;
                                    LThIncPlr2:
                                        if (rgplr[lpth->iplr].fInclude == 0) {
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (fStargateView != 0 && lppl->fStarbase != 0 && rgStargateRange[lppl->isb] > 0) {
                iRadius = rgStargateRange[lppl->isb];
                lRadius2 = (uint32_t)(iRadius * iRadius);
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if ((lppl2->fInclude == 0 || lppl2->det < detSome) && lppl2->fStarbase != 0 && StargateRangeFromLppl(lppl2, 0, 0) != 0) {
                        if (iRadius < 10000) {
                            dx = abs(rgptPlan[lppl2->id].x - pt.x);
                            if (dx > iRadius)
                                continue;
                            dy = abs(rgptPlan[lppl2->id].y - pt.y);
                            if (dy > iRadius)
                                continue;
                            d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                            if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) > lRadius2)
                                continue;
                            lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                            if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000)
                                continue;
                        }
                        MarkPlanet(lppl2, iPlr, detSome);
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(progressStep1);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)(iRadius * iRadius);
            lRadPlanet2 = (uint32_t)(iRadPlanet * iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (iRadPlanet > 0) {
                iRadius = iRadPlanet;
                lRadius2 = lRadPlanet2;
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if (lppl2->fInclude == 0 || lppl2->det < detSome) {
                        dx = abs(rgptPlan[lppl2->id].x - pt.x);
                        if (dx <= iRadius) {
                            dy = abs(rgptPlan[lppl2->id].y - pt.y);
                            if (dy <= iRadius) {
                                d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                    if (lppl2->fStarbase != 0 && lppl2->iPlayer != -1) {
                                        lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                                        if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000) {
                                            MarkPlanet(lppl2, iPlr, detObscure);
                                            continue;
                                        }
                                    }
                                    MarkPlanet(lppl2, iPlr, detSome);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

void SetVisPFThings(int16_t iPlr) {
    POINT16  pt;
    int16_t  pctCloak;
    int16_t  dy;
    FLEET   *lpfl2;
    int32_t  d2;
    int16_t  j;
    THING   *lpth;
    int32_t  lRadius2;
    THING   *lpthMac;
    int16_t  iRadius;
    int16_t  dx;
    uint16_t grbitPlr;
    PLANET  *lppl2;
    THING   *lpthMac2;
    THING   *lpth2;
    int32_t  l;
    PLANET  *lpplMac2;
    int32_t  lVis2;

    grbitPlr = iPlr == -1 ? 0 : 1 << iPlr;
    if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raMassAccel) {
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMineralPacket && lpth->iplr == iPlr && lpth->thp.iWarp != 0) {
                lpth->thp.fInclude = 1;
                iRadius = lpth->thp.iWarp + 4;
                iRadius *= iRadius;
                lRadius2 = (uint32_t)(iRadius * iRadius);
                pt = lpth->pt;
                for (j = 0; j < cFleet; j++) {
                    lpfl2 = rglpfl[j];
                    if (rglpfl[j] == 0)
                        break;
                    if (lpfl2->fInclude == 0 && lpfl2->fDead == 0) {
                        dx = abs(pt.x - lpfl2->pt.x);
                        if (dx <= iRadius) {
                            dy = abs(pt.y - lpfl2->pt.y);
                            if (dy <= iRadius) {
                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                    pctCloak = PctCloakFromLpfl(lpfl2);
                                    if (pctCloak == 0) {
                                        MarkFleet(lpfl2, detSome);
                                    } else if (l <= (int32_t)(lRadius2 * (int16_t)(100 - pctCloak)) / 100 * (int16_t)(100 - pctCloak) / 100) {
                                        MarkFleet(lpfl2, detSome);
                                    }
                                }
                            }
                        }
                    }
                }
                lpth2 = lpThings;
                lpthMac2 = lpThings + cThing;
                for (; lpth2 < lpthMac2; lpth2++) {
                    if (iPlr != -1) {
                        switch (lpth2->ith) {
                        case ithMinefield:
                        case ithMineralPacket:
                        case ithMysteryTrader:
                        case ithWormhole:
                            if ((lpth2->ith != ithMinefield || (lpth2->thm.grbitPlrNow & grbitPlr) == 0) &&
                                (lpth2->ith != ithMysteryTrader || lpth2->tht.fInclude == 0) && (lpth2->ith != ithMineralPacket || lpth2->thp.fInclude == 0) &&
                                (lpth2->ith != ithWormhole || lpth2->thw.fInclude == 0)) {
                                dx = abs(pt.x - lpth2->pt.x);
                                if (dx <= iRadius) {
                                    dy = abs(pt.y - lpth2->pt.y);
                                    if (dy <= iRadius) {
                                        l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                        if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                            switch (lpth2->ith) {
                                            case ithMineralPacket:
                                                lpth2->thp.fInclude = 1;
                                                goto LThIncPlr3;
                                            case ithMysteryTrader:
                                                lpth2->tht.fInclude = 1;
                                                break;
                                            case ithWormhole:
                                                if ((lpth2->thw.grbitPlr & grbitPlr) == 0 && l > lRadius2)
                                                    break;
                                                lpth2->thw.grbitPlr |= grbitPlr;
                                                lpth2->thw.fInclude = 1;
                                                break;
                                            default:
                                                lpth2->thm.grbitPlr |= grbitPlr;
                                                lpth2->thm.grbitPlrNow |= grbitPlr;
                                                goto LThIncPlr3;
                                            }
                                            break;
                                        LThIncPlr3:
                                            if (rgplr[lpth2->iplr].fInclude == 0) {
                                                rgplr[lpth2->iplr].wMdPlr = (rgplr[lpth2->iplr].wMdPlr & 0xfeff) | 0x100;
                                                rgplr[lpth2->iplr].wMdPlr = (rgplr[lpth2->iplr].wMdPlr & 0xfff8) | 3;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if (lppl2->fInclude == 0 || lppl2->det < detSome) {
                        dx = abs(rgptPlan[lppl2->id].x - pt.x);
                        if (dx <= iRadius) {
                            dy = abs(rgptPlan[lppl2->id].y - pt.y);
                            if (dy <= iRadius) {
                                d2 = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                    if (lppl2->fStarbase != 0 && lppl2->iPlayer != -1) {
                                        lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                                        if (lVis2 < 10000 && d2 > (int32_t)(lRadius2 * lVis2) / 10000) {
                                            MarkPlanet(lppl2, iPlr, detObscure);
                                            continue;
                                        }
                                    }
                                    MarkPlanet(lppl2, iPlr, detSome);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raMines) {
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMinefield && lpth->iplr == iPlr) {
                lRadius2 = lpth->thm.cMines;
                pt = lpth->pt;
                for (j = 0; j < cFleet; j++) {
                    lpfl2 = rglpfl[j];
                    if (rglpfl[j] == 0)
                        break;
                    if (lpfl2->fInclude == 0 && lpfl2->fDead == 0 && lpfl2->idPlanet == -1) {
                        dx = abs(pt.x - lpfl2->pt.x);
                        if (dx <= lRadius2) {
                            dy = abs(pt.y - lpfl2->pt.y);
                            if (dy <= lRadius2) {
                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if ((uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lRadius2) {
                                    pctCloak = PctCloakFromLpfl(lpfl2);
                                    if (pctCloak == 0 || Random(100) >= pctCloak) {
                                        MarkFleet(lpfl2, detSome);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

void SetVisPFFinish(int16_t iPlr) {
    int16_t detMajor;
    int16_t j;
    int16_t i;

    detMajor = GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raAttack ? 7 : 3;
    for (i = 0; i < game.cPlayer; i++) {
        if (i != iPlr) {
            rgplr[i].cShDef = 0;
            for (j = 0; j < 16; j++) {
                if ((1 << iPlr & rglpshdef[i][j].grbitPlr) != 0) {
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfeff) | 0x100;
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | 7;
                } else {
                    if (rglpshdef[i][j].fInclude == 0)
                        continue;
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | (detMajor & 0xff);
                }
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 3;
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                rgplr[i].cShDef++;
            }
            rgplr[i].cshdefSB = 0;
            for (j = 0; j < 10; j++) {
                if ((1 << iPlr & rglpshdefSB[i][j].grbitPlr) != 0) {
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100;
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | 7;
                } else {
                    if (rglpshdefSB[i][j].fInclude == 0)
                        continue;
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | (detMajor & 0xff);
                }
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 3;
                rgplr[i].cshdefSB++;
            }
        }
    }
    return;
}
