#include "common.h"

void WriteOrders(FLEET *lpfl) {
    int16_t cord;
    ORDER  *lpord;

    if (lpfl->cord != 0) {
        cord = lpfl->cord;
        lpord = lpfl->lpplord->rgord;
        for (; cord != 0; cord--) {
            if (lpord->grTask != grTaskNone) {
                WriteRt(rtOrderA, 18, lpord);
            } else {
                WriteRt(rtOrderB, 8, lpord);
            }
            lpord = lpord + 1;
        }
    }
    return;
}

void WriteRtPlr(PLAYER *pplr, uint8_t *pbStore) {
    uint8_t  rgb[264];
    int16_t  i;
    uint8_t *pb;
    int16_t  cOut;
    uint8_t *t_55c4;

    if (pbStore == 0x0) {
        pbStore = rgb;
    }
    if (pplr->fDead != 0x0) {
        pplr->det = 0x7;
    }
    memmove(pbStore, pplr, sizeof(PLAYER));
    if (pplr->det != 0x7) {
        pb = pbStore + 8;
    } else {
        for (i = 15; i >= 0 && (int16_t)pplr->rgmdRelation[i] == 0; i--) {
        }
        i = i + 1;
        pb = pbStore + 112;
        t_55c4 = pb;
        pb = pb + 1;
        *t_55c4 = LOBYTE(i);
        memmove(pb, pplr->rgmdRelation, i);
        pb = pb + i;
    }
    cOut = 31;
    if ((int16_t)pplr->szName[0] == 0 || FCompressUserString(pplr->szName, pb + 1, &cOut) == 0) {
        strcpy(pb + 1, pplr->szName);
        *pb = 0x0;
        pb = pb + (2 + strlen(pplr->szName));
    } else {
        *pb = LOBYTE(cOut);
        pb = pb + (1 + cOut);
    }
    cOut = 31;
    if ((int16_t)pplr->szNames[0] == 0 || FCompressUserString(pplr->szNames, pb + 1, &cOut) == 0) {
        strcpy(pb + 1, pplr->szNames);
        *pb = 0x0;
        pb = pb + (2 + strlen(pplr->szNames));
    } else {
        *pb = LOBYTE(cOut);
        pb = pb + (1 + cOut);
    }
    WriteRt(rtPlr, pb - pbStore, pbStore);
    return;
}

void WriteRtShDef(SHDEF *lpshdef, uint8_t **ppbStore) {
    uint8_t  rgb[147];
    char     szHulName[32];
    uint8_t *pb;
    int16_t  cOut;

    rgb[2] = LOBYTE(lpshdef->hul.ihuldef);
    RawStore16(rgb, lpshdef->wFlags);
    rgb[6] = lpshdef->hul.chs;
    rgb[3] = LOBYTE(lpshdef->hul.ibmp);
    if (lpshdef->det != 0x7) {
        RawStore16(&rgb[4], lpshdef->hul.wtEmpty);
        pb = &rgb[6];
    } else {
        RawStore16(&rgb[4], lpshdef->hul.dp);
        RawStore32(&rgb[7], lpshdef->turn | ((uint32_t)lpshdef->cBuilt & 0xffff) << 0x10);
        RawStore32(&rgb[11], ((uint32_t)lpshdef->cBuilt >> 0x10 & 0xffff) | ((uint32_t)lpshdef->cExist & 0xffff) << 0x10);
        RawStore16(&rgb[15], HIWORD(lpshdef->cExist));
        pb = &rgb[17];
        fmemmove(pb, lpshdef->hul.rghs, rgb[6] * 0x4);
        pb = pb + rgb[6] * 4;
    }
    if (lpshdef->det != 0x7) {
        fstrcpy(szHulName, LphuldefFromId(lpshdef->hul.ihuldef)->hul.szClass);
    } else {
        fstrcpy(szHulName, lpshdef->hul.szClass);
    }
    cOut = 31;
    if ((int16_t)szHulName[0] == 0 || FCompressUserString(szHulName, pb + 1, &cOut) == 0) {
        strcpy(pb + 1, szHulName);
        *pb = 0x0;
        pb = pb + (2 + strlen(szHulName));
    } else {
        *pb = LOBYTE(cOut);
        pb = pb + (1 + cOut);
    }
    if (ppbStore == 0x0) {
        WriteRt(rtShDef, pb - rgb, rgb);
    } else {
        memmove(*ppbStore, rgb, pb - rgb);
        *ppbStore = *ppbStore + (pb - rgb);
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
    int16_t  mdTarget;
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
    int16_t  t_merge_5be5_0001;
    int16_t  t_merge_5be5_0002;
    uint16_t t_scratch_m7a;
    uint16_t t_scratch_m7a_3;
    uint16_t t_scratch_m7a_4;

    fRet = 1;
    SetVisiblePlanFleet(iPlayer);
    if (gd.fGeneratingTurn != 0x0 && iPlayer != -1) {
        for (i = 0; i < cFleet; i++) {
            lpfl = rglpfl[i];
            if (rglpfl[i] == 0x0)
                break;
            if (lpfl->fDead == 0x0 && lpfl->iplr == iPlayer) {
                for (j = 0; j < 16 && lpfl->rgcsh[j] == 0; j++) {
                }
                if (j != 16) {
                    lpord = lpfl->lpplord->rgord;
                    if (lpord->grobj == grobjFleet) {
                        if (FFindNearestObject(lpord->pt, 0x81, &scan) == 0) {
                            lpord->grobj = grobjOther;
                            lpord->id = 0;
                        } else {
                            lpord->grobj = grobjPlanet;
                            lpord->id = scan.idpl;
                        }
                    }
                    if (lpord->grTask == grTaskNone && lpfl->cord > 1 && lpord[1].grTask == grTaskPatrol) {
                        lpord->grTask = grTaskPatrol;
                        lpord->tptl.iWarp = lpord[1].tptl.iWarp;
                        lpord->tptl.iDist = lpord[1].tptl.iDist;
                    }
                    if (lpord->grTask == grTaskPatrol && (lpfl->cord <= 1 || lpord[1].grobj != grobjFleet)) {
                        lpflBest = 0x0;
                        lBest = 100000000;
                        fFoundIdeal = 0;
                        if (lpfl->idPlanet != -1 || lpfl->cord < 2 || lpfl->fRepOrders == 0x0) {
                            t_merge_5be5_0001 = lpfl->pt.x;
                            t_merge_5be5_0002 = lpfl->pt.y;
                        } else {
                            t_merge_5be5_0001 = lpord[1].pt.x;
                            t_merge_5be5_0002 = lpord[1].pt.y;
                        }
                        pt.x = t_merge_5be5_0001;
                        pt.y = t_merge_5be5_0002;
                        lpbtlplan = rglpbtlplan[lpfl->iPlayer] + lpfl->iplan;
                        mdTarget = lpbtlplan->mdTarget1;
                        for (iflT = 0; iflT < cFleet; iflT++) {
                            lpflTarget = rglpfl[iflT];
                            if (rglpfl[iflT] == 0x0)
                                break;
                            if (lpflTarget->fInclude != 0x0 && lpflTarget->iPlayer != iPlayer) {
                                dx = (int32_t)(lpflTarget->pt.x - pt.x);
                                dy = (int32_t)(lpflTarget->pt.y - pt.y);
                                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                                if (((fFoundIdeal == 0 && lpflTarget->fMark == 0x0) || (l < lBest && (fFoundIdeal == 0 || lpflTarget->fMark == 0x0))) &&
                                    (FMatchTarget(lpflTarget, mdTarget, 0) != 0 && FAttackPlayer(lpfl, lpflTarget->iPlayer) != 0)) {
                                    lpflBest = lpflTarget;
                                    lBest = l;
                                    if (lpflTarget->fMark == 0x0) {
                                        fFoundIdeal = 1;
                                    }
                                }
                            }
                        }
                        if (fFoundIdeal != 0 && gd.fTutorial == 0x0) {
                            lpflBest->fMark = 0x1;
                        }
                        j = 0x32 * lpord->tptl.iDist + 50;
                        if (j == 550) {
                            j = 10000;
                        }
                        if (lpflBest != 0x0 && lBest != 0 && lBest <= (int32_t)(uint32_t)((int32_t)j * (int32_t)j)) {
                            if (lpfl->lpplord->iordMax <= lpfl->cord + 1) {
                                lpfl->lpplord = (PLORD *)LpplReAlloc((PL *)lpfl->lpplord, lpfl->cord + 2);
                                lpord = lpfl->lpplord->rgord;
                            }
                            if (lpfl->cord > 1) {
                                fmemmove(lpord + 2, lpord + 1, (lpfl->cord - 1) * sizeof(ORDER));
                            }
                            if (lpfl->cord != 1) {
                                if (lpord[1].tsell.iPlrX != 0x0) {
                                    lpord[1].iWarp = lpord[1].tsell.iPlrX;
                                } else {
                                    t_scratch_m7a_4 = IFindIdealWarp(lpfl, 0);
                                    lpord[1].iWarp = t_scratch_m7a_4;
                                }
                            } else {
                                fmemset(lpord + 1, 0, sizeof(ORDER));
                                lpord[1].fValidTask = 0x1;
                                lpord[1].grTask = grTaskPatrol;
                                lpord[1].tptl = lpord->tptl;
                                if (lpord[1].tptl.iWarp != 0x0) {
                                    lpord[1].iWarp = lpord[1].tsell.iPlrX;
                                } else {
                                    t_scratch_m7a = IFindIdealWarp(lpfl, 0);
                                    lpord[1].iWarp = t_scratch_m7a;
                                }
                                if (lpfl->fRepOrders != 0x0) {
                                    lpord[2] = *lpord;
                                    t_scratch_m7a_3 = IFindIdealWarp(lpfl, 0);
                                    lpord[2].iWarp = t_scratch_m7a_3;
                                    lpfl->cord = lpfl->cord + 1;
                                    lpfl->lpplord->iordMac = lpfl->lpplord->iordMac + 0x1;
                                }
                            }
                            lpord[1].pt = lpflBest->pt;
                            lpord[1].id = lpflBest->id;
                            lpord[1].grobj = grobjFleet;
                            lpfl->cord = lpfl->cord + 1;
                            lpfl->lpplord->iordMac = lpfl->lpplord->iordMac + 0x1;
                            FSendPlrMsg(iPlayer, 255, lpfl->id | 0x8000, lpfl->id, lpflBest->id, 0, 0, 0, 0, 0);
                        }
                    }
                    if (lpord->grTask != grTaskXfer && lpfl->cord > 1) {
                        for (iord = 1; iord < lpfl->cord; iord++) {
                            if (lpord[iord].grobj != grobjThing) {
                                if (lpord[iord].grobj == grobjFleet) {
                                    fNoAutoTrack = lpord[iord].fNoAutoTrack;
                                    if (fNoAutoTrack != 0) {
                                        lpord[iord].fNoAutoTrack = 0x0;
                                    }
                                    lpflT = LpflFromId(lpord[iord].id);
                                    if (lpflT != 0x0 && lpflT->fDead == 0x0) {
                                        if (lpflT->fInclude != 0x0)
                                            continue;
                                        if (lpflT->idPlanet == -1 || fNoAutoTrack != 0) {
                                            FSendPlrMsg(iPlayer, 42, lpfl->id | 0x8000, lpfl->id, 0, 0, 0, 0, 0, 0);
                                        } else {
                                            FSendPlrMsg(iPlayer, 41, lpfl->id | 0x8000, lpfl->id, lpflT->idPlanet, 0, 0, 0, 0, 0);
                                        }
                                    } else {
                                        FSendPlrMsg(iPlayer, 40, lpfl->id | 0x8000, lpfl->id, lpord[iord].id, 0, 0, 0, 0, 0);
                                    }
                                    lpord[iord].grobj = grobjOther;
                                    lpord[iord].id = iord;
                                    if (FFindNearestObject(lpord[iord].pt, 0x81, &scan) != 0) {
                                        lpord[iord].grobj = grobjPlanet;
                                        lpord[iord].id = scan.idpl;
                                    }
                                }
                            } else {
                                lpth = LpthFromId(lpord[iord].id);
                                if (lpth == 0x0 || (lpth->ith == ithMysteryTrader && lpth->tht.fInclude == 0x0) ||
                                    (lpth->ith == ithMinefield && (0x1 << iPlayer & lpth->thm.grbitPlrNow) == 0x0) ||
                                    (lpth->ith == ithWormhole && lpth->thw.fInclude == 0x0)) {
                                    if (lpth == 0x0 || lpth->ith != ithWormhole) {
                                        if (lpth == 0x0 || lpth->ith != ithMysteryTrader) {
                                            if (lpth != 0x0 && lpth->ith == ithMinefield) {
                                                FSendPlrMsg2(lpfl->iPlayer, 273, lpfl->id | 0x8000, lpfl->id, 0);
                                            }
                                        } else {
                                            FSendPlrMsg2(lpfl->iPlayer, 272, lpfl->id | 0x8000, lpfl->id, 0);
                                        }
                                    } else {
                                        FSendPlrMsg2(lpfl->iPlayer, 248, lpfl->id | 0x8000, lpfl->id, 0);
                                    }
                                    lpord[iord].grobj = grobjOther;
                                    lpord[iord].id = iord;
                                }
                            }
                        }
                    }
                } else {
                    lpfl->fDead = 0x1;
                }
            }
        }
    }
    MarkPlayersThatSentMsgs(iPlayer);
    MarkPlanetsPlayerLost(iPlayer);
    if (iPlayer != -1) {
        _wsprintf(szWork, "%s.m%d", pszFileBase, iPlayer + 1);
    } else {
        _wsprintf(szWork, "%s.hst", pszFileBase);
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0 || ((fAppend == 0 || FAppendFile(iPlayer) == 0) && FCreateFile(iPlayer == -1 ? dtHost : dtTurn, iPlayer, 0x0) == 0)) {
        idPlayer = iPlayer;
        if (fAppend == 0) {
            if (iPlayer == -1) {
                idPlayer = -1;
                AlertSz(PszFormatIds(idsUnableCreateHostFile, 0x0), MB_ICONHAND);
            } else {
                AlertSz(PszFormatIds(idsUnableCreateNewTurnFile, 0x0), MB_ICONHAND);
            }
        } else {
            AlertSz(PszFormatIds(idsUnableUpdateTurnFile, 0x0), MB_ICONHAND);
        }
        idPlayer = -1;
        fRet = 0;
    } else {
        WriteBattles(iPlayer);
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0x0 || rgplr[i].fDead != 0x0) {
                if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raTerra) {
                    rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 0x7;
                }
                WriteRtPlr(&rgplr[i], 0x0);
            }
        }
        if (iPlayer == -1 && lSaltCur != 0) {
            WriteRt(rtChgPassword, 4, &lSaltCur);
        }
        WritePlayerMessages(iPlayer);
        i = 0;
        lpplT = lpPlanets;
        while (i < cPlanet) {
            if (lpplT->fInclude != 0x0) {
                if (lpplT->det != 0x7) {
                    if (lpplT->det != 0x2) {
                        WritePlanet(lpplT, rtPlanetB, 0);
                    } else {
                        pl = *lpplT;
                        lpplT->fStarbase = 0x0;
                        lpplT->det = 0x3;
                        WritePlanet(lpplT, rtPlanetB, 0);
                        *lpplT = pl;
                    }
                } else {
                    WritePlanet(lpplT, rtPlanet, 0);
                    if (lpplT->lpplprod != 0x0) {
                        WriteRt(rtProdQ, lpplT->lpplprod->iprodMac * 4, lpplT->lpplprod->rgprod);
                    }
                }
            }
            i = i + 1;
            lpplT = lpplT + 1;
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0x0) {
                lpshdef = rglpshdef[i];
                for (j = 0; j < 16; j++) {
                    if (lpshdef[j].fFree == 0x0 && lpshdef[j].fInclude != 0x0) {
                        WriteRtShDef(lpshdef + j, 0x0);
                    }
                }
            }
        }
        for (i = 0; i < cFleet; i++) {
            lpflT = rglpfl[i];
            if (rglpfl[i] == 0x0)
                break;
            if (lpflT->fInclude != 0x0) {
                WriteFleet(lpflT);
            }
        }
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fInclude != 0x0) {
                lpshdef = rglpshdefSB[i];
                for (j = 0; j < 10; j++) {
                    if (lpshdef[j].fFree == 0x0 && lpshdef[j].fInclude != 0x0) {
                        WriteRtShDef(lpshdef + j, 0x0);
                    }
                }
            }
        }
        if (iPlayer != -1 && vlprgScoreX != 0x0) {
            for (i = 0; i < game.cPlayer; i++) {
                if (gd.fGameOverMan != 0x0 || i == iPlayer || rgplr[i].fDead != 0x0 || (game.fVisScores != 0x0 && game.turn >= 0x14)) {
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
                if ((lpth->ith != ithMinefield || (0x1 << iPlayer & lpth->thm.grbitPlrNow) == 0x0) &&
                    (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0x0) && (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0x0) &&
                    (lpth->ith != ithWormhole || lpth->thw.fInclude == 0x0))
                    continue;
            }
        L_6de7:
            i = i + 1;
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
                    if ((lpth->ith != ithMinefield || (0x1 << iPlayer & lpth->thm.grbitPlrNow) == 0x0) &&
                        (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0x0) && (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0x0) &&
                        (lpth->ith != ithWormhole || lpth->thw.fInclude == 0x0))
                        continue;
                }
            L_6f62:
                WriteRt(rtThing, 18, lpth);
            }
        }
        if (iPlayer != -1) {
            i = iPlayer;
            iMax = iPlayer + 1;
        } else {
            i = 0;
            iMax = game.cPlayer;
        }
        for (; i < iMax; i++) {
            lpbtlplan = rglpbtlplan[i];
            j = 0;
            while (j < rgcbtlplan[i]) {
                WriteBattlePlan(lpbtlplan, 0);
                j = j + 1;
                lpbtlplan = lpbtlplan + 1;
            }
        }
        WriteRt(rtEOF, 2, &game.turn);
        StreamClose();
    }
    SetVisiblePlanFleet(-1);
    return fRet;
}

int16_t FAppendFile(int16_t iPlayer) {
    if (FMarkFile(0x2003, iPlayer, 4, 1) != 0) {
        WriteBOF(iPlayer, 3, 1);
        return 1;
    }
    return 0;
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
        fPlayerCur = 0x1 << iPlayer;
        while (lphb != 0x0) {
            for (lpbtldata = (BTLDATA *)lpbBattle; lphb->ibTop <= sizeof(HB) || lpbtldata->id == 0xffff; lpbtldata = (BTLDATA *)lpbBattle) {
                lphb = lphb->lphbNext;
                if (lphb == 0x0) {
                    return;
                }
                lpbBattle = (uint8_t *)lphb + (sizeof(HB) + 2);
            }
            if ((lpbtldata->grfPlr & fPlayerCur) == 0x0) {
                lpbBattle = lpbBattle + lpbtldata->cbData;
            } else {
                for (i = 0; i < game.cPlayer; i++) {
                    if (i != iPlayer && rgplr[i].fInclude == 0x0 && (0x1 << i & lpbtldata->grfPlr) != 0x0) {
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                        rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 0x3;
                    }
                }
                for (i = 0; i < lpbtldata->ctok; i++) {
                    if (lpbtldata->rgtok[i].iplr != iPlayer) {
                        if (lpbtldata->rgtok[i].grobj != grobjPlanet) {
                            lpfl = LpflFromId(lpbtldata->rgtok[i].id);
                            if (lpfl->iPlayer != iPlayer && rgplr[lpfl->iPlayer].fInclude == 0x0) {
                                rgplr[lpfl->iPlayer].wMdPlr = (rgplr[lpfl->iPlayer].wMdPlr & 0xfeff) | 0x100;
                                rgplr[lpfl->iPlayer].wMdPlr = (rgplr[lpfl->iPlayer].wMdPlr & 0xfff8) | 0x3;
                            }
                            if (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].fInclude == 0x0) {
                                rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags =
                                    (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xfeff) | 0x100;
                                rgplr[lpfl->iPlayer].cShDef = rgplr[lpfl->iPlayer].cShDef + 1;
                            }
                            rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags =
                                (rglpshdef[lpfl->iPlayer][lpbtldata->rgtok[i].ishdef].wFlags & 0xff00) | 0x7;
                            if (lpfl->fDead == 0x0) {
                                if (lpfl->fInclude == 0x0) {
                                    rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 0x1;
                                    lpfl->fInclude = 0x1;
                                    lpfl->det = 0x0;
                                }
                                if (lpfl->det < 0x3) {
                                    lpfl->det = 0x3;
                                }
                            }
                        } else {
                            iplr = lpbtldata->rgtok[i].iplr;
                            lppl = LpplFromId(lpbtldata->rgtok[i].id);
                            if (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].fInclude == 0x0) {
                                rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].wFlags =
                                    (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 0x10].wFlags & 0xfeff) | 0x100;
                                rgplr[iplr].cshdefSB = rgplr[iplr].cshdefSB + 0x1;
                            }
                            rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 16].wFlags =
                                (rglpshdefSB[iplr][lpbtldata->rgtok[i].ishdef - 0x10].wFlags & 0xff00) | 0x7;
                        }
                    }
                }
                if (lpbtldata->idPlanet != 0xffff) {
                    lppl = LpplFromId(lpbtldata->idPlanet);
                    MarkPlanet(lppl, iPlayer, 0x1);
                }
                if (lpbtldata->cbData >= 0x400) {
                    cb = lpbtldata->ctok * 0x1d + 14;
                    lpbtlrec = (BTLREC *)(lpbBattle + cb);
                    if (cb >= 1024) {
                        ctok = 34;
                        ctok = ctok >= lpbtldata->ctok ? lpbtldata->ctok : ctok;
                        if (ctok > lpbtldata->ctok) {
                            ctok = lpbtldata->ctok;
                        }
                        WriteRt(rtBtlData, ctok * 29 + 14, lpbBattle);
                        lpbBattle = lpbBattle + (14 + 29 * ctok);
                        ctok = lpbtldata->ctok - ctok;
                        while (ctok > 0) {
                            if ((uint16_t)ctok <= 35) {
                                WriteRt(rtContinue, ctok * 29, lpbBattle);
                                lpbBattle = lpbBattle + 29 * ctok;
                                ctok = 0;
                            } else {
                                WriteRt(rtContinue, 1015, lpbBattle);
                                lpbBattle = lpbBattle + 1015;
                                ctok = ctok - 35;
                            }
                        }
                    } else {
                        WriteRt(rtBtlData, cb, lpbBattle);
                        lpbBattle = lpbBattle + cb;
                    }
                    cb = lpbtldata->cbData - 14 - lpbtldata->ctok * 0x1d;
                    if (cb >= 1024) {
                        while (cb != 0) {
                            cbRec = lpbtlrec->ctok * 8 + 6;
                            if (cbRec < 1024) {
                                cbT = 0;
                                do {
                                    cbT = cbT + cbRec;
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + cbRec);
                                    cb = cb - cbRec;
                                    if (cb != 0) {
                                        cbRec = lpbtlrec->ctok * 8 + 6;
                                    }
                                } while (cb != 0 && cbT + cbRec < 1024);
                                WriteRt(rtContinue, cbT, lpbBattle);
                            } else {
                                cb = cb - cbRec;
                                for (; cbRec >= 1024; cbRec = cbRec - 1023) {
                                    WriteRt(rtContinue, 1023, lpbtlrec);
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + 0x3ff);
                                }
                                if (cbRec != 0) {
                                    WriteRt(rtContinue, cbRec, lpbtlrec);
                                    lpbtlrec = (BTLREC *)((uint8_t *)lpbtlrec + cbRec);
                                }
                            }
                            lpbBattle = (uint8_t *)lpbtlrec;
                        }
                    } else {
                        WriteRt(rtContinue, cb, lpbtlrec);
                        lpbBattle = lpbBattle + cb;
                    }
                } else {
                    WriteRt(rtBtlData, lpbtldata->cbData, lpbBattle);
                    lpbBattle = lpbBattle + lpbtldata->cbData;
                }
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
    uint8_t *t_7ca1;
    uint8_t *t_7d06;
    uint8_t *t_7d93;
    uint8_t *t_7f4f;
    uint16_t t_scratch_m5c_11;

    memset(rgb, 0, 0x50);
    RawStore16(rgb, (RawLoad16(rgb) & 0xf800) | (lppl->id & 0x7ff));
    RawStore16(rgb, (RawLoad16(rgb) & 0x7ff) | (lppl->iPlayer & 0x1f) << 0xb);
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xff80) | (lppl->det & 0x7f));
    if (rt == rtPlanetB && lppl->det > 0x3) {
        RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xff80) | ((fHistory == 0 ? 0x4 : 0x3) & 0x7f));
    }
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xfeff) | (lppl->fInclude & 0x1) << 0x8);
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xfdff) | (lppl->fStarbase & 0x1) << 0x9);
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xff7f) | (lppl->fHomeworld & 0x1) << 0x7);
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0x7fff) | (lppl->fFirstYear & 0x1) << 0xf);
    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xbfff) | ((lppl->idRoute == 0x0 ? 0x0 : 0x1) & 0x1) << 0xe);
    pbBase = &rgb[4];
    pb = pbBase;
    if ((RawLoad16(&rgb[2]) & 0x7f) > 0x1) {
        pb = pbBase + 1;
        bMask = 0x3;
        i = 0;
        while (i < 3) {
            if (lppl->rgpctMinLevel[i] > 0x0) {
                *pbBase = *pbBase | LOBYTE(bMask & 0x55);
                t_7ca1 = pb;
                pb = pb + 1;
                *t_7ca1 = lppl->rgpctMinLevel[i];
            }
            i = i + 1;
            bMask = LOBYTE(bMask * 0x4);
        }
        i = 0;
        while (i < 3) {
            *pb = lppl->rgMinConc[i];
            i = i + 1;
            pb = pb + 1;
        }
        for (i = 0; i < 3; i++) {
            t_7d06 = pb;
            pb = pb + 1;
            *t_7d06 = lppl->rgEnvVar[i];
            if ((int16_t)lppl->rgEnvVar[i] != (int16_t)lppl->rgEnvVarOrig[i]) {
                RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xfbff) | 0x400);
            }
        }
        if ((RawLoad16(&rgb[2]) >> 0xa & 0x1) != 0x0) {
            for (i = 0; i < 3; i++) {
                t_7d93 = pb;
                pb = pb + 1;
                *t_7d93 = lppl->rgEnvVarOrig[i];
            }
        }
        if (lppl->iPlayer != -1) {
            RawStore16(pb, lppl->uGuesses);
            pb = pb + 2;
        }
        if ((RawLoad16(&rgb[2]) & 0x7f) > 0x3) {
            pbBase = pb;
            pb = pb + 1;
            bMask = 0x3;
            i = 0;
            while (i < 4) {
                if ((i != 3 || lppl->det >= 0x7) && lppl->rgwtMin[i] > 0) {
                    if (lppl->rgwtMin[i] <= 255) {
                        *pbBase = *pbBase | LOBYTE(bMask & 0x55);
                        t_7f4f = pb;
                        pb = pb + 1;
                        *t_7f4f = LOBYTE(LOWORD(lppl->rgwtMin[i]));
                    } else if (lppl->rgwtMin[i] <= 65535) {
                        *pbBase = *pbBase | LOBYTE(bMask & 0xaa);
                        RawStore16(pb, LOWORD(lppl->rgwtMin[i]));
                        pb = pb + 2;
                    } else {
                        *pbBase = *pbBase | LOBYTE(bMask & 0xff);
                        RawStore16(pb, LOWORD(lppl->rgwtMin[i]));
                        RawStore16((uint8_t *)pb + 0x2, HIWORD(lppl->rgwtMin[i]));
                        pb = pb + 4;
                    }
                }
                i = i + 1;
                bMask = LOBYTE(bMask * 0x4);
            }
            if (*pbBase != 0x0) {
                RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xdfff) | 0x2000);
            } else {
                pb = pbBase;
            }
            if (rt != rtPlanetB) {
                t_scratch_m5c_11 = lppl->fArtifact;
                RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xefff) | (t_scratch_m5c_11 & 0x1) << 0xc);
                if ((lppl->iPlayer != -1 && (lppl->iDeltaPop != 0x0 || lppl->fNoResearch != 0x0)) ||
                    (lppl->cMines != 0x0 || lppl->cFactories != 0x0 || lppl->cDefenses != 0x0 || lppl->iScanner != 0x1f)) {
                    RawStore16(&rgb[2], (RawLoad16(&rgb[2]) & 0xf7ff) | 0x800);
                    fmemmove(pb, lppl->rgbImp, 0x8);
                    pb = pb + 8;
                }
                if (lppl->iPlayer != -1) {
                    if (lppl->fStarbase != 0x0) {
                        RawStore16(pb, lppl->isb | lppl->pctDp << 0x4);
                        RawStore16((uint8_t *)pb + 0x2, lppl->idFling | lppl->iWarpFling << 0xa | lppl->fNoHeal << 0xe | lppl->unused3 << 0xf);
                        pb = pb + 4;
                    }
                    if (lppl->idRoute != 0x0) {
                        RawStore16(pb, lppl->wRouting);
                        pb = pb + 2;
                    }
                }
                WriteRt(rtPlanet, pb - rgb, rgb);
                return;
            }
        }
    }
    if (lppl->fStarbase != 0x0) {
        *pb = LOBYTE(lppl->isb);
        pb = pb + 1;
    }
    if (fHistory != 0) {
        RawStore16(pb, lppl->turn);
        pb = pb + 2;
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
    uint8_t  *t_82e8;
    uint16_t *t_834b;
    uint16_t *t_8714;
    uint8_t  *t_873a;
    uint8_t  *t_874c;

    fmemmove(rgb, lpfl, 0xc);
    fByte = 1;
    grMask = 0x1;
    us = 0x0;
    i = 0;
    while (i < 16) {
        if (lpfl->rgcsh[i] > 0) {
            us = us | grMask;
            if (lpfl->rgcsh[i] > 255) {
                fByte = 0;
            }
        }
        i = i + 1;
        grMask = grMask * 0x2;
    }
    RawStore16(&rgb[4], (RawLoad16(&rgb[4]) & 0xf7ff) | (fByte & 0x1) << 0xb);
    RawStore16(&rgb[12], us);
    pb = &rgb[14];
    if (fByte == 0) {
        pus = (uint16_t *)pb;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                t_834b = pus;
                pus = pus + 1;
                *t_834b = lpfl->rgcsh[i];
            }
        }
        pb = (uint8_t *)pus;
    } else {
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                t_82e8 = pb;
                pb = pb + 1;
                *t_82e8 = LOBYTE(lpfl->rgcsh[i]);
            }
        }
    }
    if (lpfl->det >= 0x4) {
        fByte = 0;
        grMask = 0x3;
        pus = (uint16_t *)pb;
        pb = pb + 2;
        us = 0x0;
        i = 0;
        while (i < 5) {
            if (lpfl->rgwtMin[i] > 0 && (lpfl->det == 0x7 || (i != 4 && i != 3))) {
                if (lpfl->rgwtMin[i] <= 255) {
                    us = us | (grMask & 0x155);
                    *pb = LOBYTE(LOWORD(lpfl->rgwtMin[i]));
                    pb = pb + 1;
                } else if (lpfl->rgwtMin[i] <= 65535) {
                    us = us | (grMask & 0x2aa);
                    RawStore16(pb, LOWORD(lpfl->rgwtMin[i]));
                    pb = pb + 2;
                } else {
                    us = us | (grMask & 0x3ff);
                    RawStore16(pb, LOWORD(lpfl->rgwtMin[i]));
                    RawStore16((uint8_t *)pb + 0x2, HIWORD(lpfl->rgwtMin[i]));
                    pb = pb + 4;
                }
            }
            i = i + 1;
            grMask = grMask * 0x4;
        }
        *pus = us;
    }
    if (lpfl->det >= 0x7) {
        grMask = 0x1;
        us = 0x0;
        i = 0;
        while (i < 16) {
            if (lpfl->rgdv[i].dp != 0x0) {
                us = us | grMask;
            }
            i = i + 1;
            grMask = grMask * 0x2;
        }
        RawStore16(pb, us);
        pb = pb + 2;
        pus = (uint16_t *)pb;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgdv[i].dp != 0x0) {
                t_8714 = pus;
                pus = pus + 1;
                *t_8714 = lpfl->rgdv[i].dp;
            }
        }
        pb = (uint8_t *)pus;
        t_873a = pb;
        pb = pb + 1;
        *t_873a = lpfl->iplan;
        t_874c = pb;
        pb = pb + 1;
        *t_874c = LOBYTE(lpfl->cord);
        WriteRt(rtFleetA, pb - rgb, rgb);
        WriteOrders(lpfl);
        if (lpfl->lpszName != 0x0) {
            WriteRtString(lpfl->lpszName);
        }
    } else {
        wt = 0;
        RawStore16(pb, lpfl->dirFltX | lpfl->dirFltY << 0x8);
        RawStore16((uint8_t *)pb + 0x2,
                   lpfl->iwarpFlt | lpfl->fdirValid << 0x4 | lpfl->fCompChg << 0x5 | lpfl->fTargeted << 0x6 | lpfl->fSkipped << 0x7 | lpfl->fUnused << 0x8);
        pb = pb + 4;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] > 0) {
                wt = wt + (uint32_t)((int32_t)lpfl->rgcsh[i] * (uint32_t)rglpshdef[lpfl->iPlayer][i].hul.wtEmpty);
            }
        }
        for (i = 0; i <= 3; i++) {
            wt = wt + lpfl->rgwtMin[i];
        }
        RawStore16(pb, LOWORD(wt));
        RawStore16((uint8_t *)pb + 0x2, HIWORD(wt));
        pb = pb + 4;
        WriteRt(0x11, pb - rgb, rgb);
    }
    return;
}

void WriteRtString(char *lpsz) {
    uint8_t rgb[33];
    int16_t cOut;

    if (lpsz != 0x0 && (int16_t)*lpsz != 0) {
        cOut = 31;
        if (FCompressUserString(lpsz, &rgb[1], &cOut) == 0) {
            fstrcpy(&rgb[1], lpsz);
            rgb[0] = 0x0;
            cOut = fstrlen(lpsz) + 1;
        } else {
            rgb[0] = LOBYTE(cOut);
        }
        WriteRt(rtString, cOut + 1, rgb);
    }
    return;
}

void MarkFleet(FLEET *lpfl, int16_t det) {
    int16_t i;
    SHDEF  *lpshdef;

    if (lpfl->fInclude == 0x0) {
        lpshdef = rglpshdef[lpfl->iPlayer];
        lpfl->fInclude = 0x1;
        lpfl->det = 0x0;
        lpfl->fdirValid = 0x1;
        rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 0x1;
        for (i = 0; i < 16; i++) {
            if (lpfl->rgcsh[i] != 0) {
                lpshdef[i].wFlags = (lpshdef[i].wFlags & 0xfeff) | 0x100;
            }
        }
    }
    if (lpfl->det < (uint16_t)det) {
        lpfl->det = det;
    }
    return;
}

void WriteBattlePlan(BTLPLAN *lpbtlplan, int16_t fLog) {
    uint8_t  rgb[36];
    uint8_t *pb;
    char     szPlanName[32];
    int16_t  cOut;

    fmemmove(rgb, lpbtlplan, 0x4);
    if (lpbtlplan->fDelete == 0x0) {
        pb = &rgb[4];
        fstrcpy(szPlanName, lpbtlplan->szName);
        cOut = 31;
        if ((int16_t)szPlanName[0] == 0 || FCompressUserString(szPlanName, pb + 1, &cOut) == 0) {
            strcpy(pb + 1, szPlanName);
            *pb = 0x0;
            pb = pb + (2 + strlen(szPlanName));
        } else {
            *pb = LOBYTE(cOut);
            pb = pb + (1 + cOut);
        }
    } else {
        pb = &rgb[2];
    }
    if (fLog == 0) {
        WriteRt(rtBtlPlan, pb - rgb, rgb);
    } else {
        WriteMemRt(30, pb - rgb, rgb);
    }
    return;
}

void MarkPlanet(PLANET *lppl, int16_t iPlr, uint16_t det) {
    SHDEF *lpshdef;

    if (lppl->fInclude == 0x0) {
        lppl->fInclude = 0x1;
        lppl->det = 0x0;
        rgplr[iPlr].cPlanet = rgplr[iPlr].cPlanet + 1;
    }
    if (lppl->det < det) {
        lppl->det = det;
    }
    if (lppl->iPlayer != -1 && rgplr[lppl->iPlayer].fInclude == 0x0) {
        rgplr[lppl->iPlayer].wMdPlr = (rgplr[lppl->iPlayer].wMdPlr & 0xfeff) | 0x100;
        rgplr[lppl->iPlayer].wMdPlr = (rgplr[lppl->iPlayer].wMdPlr & 0xfff8) | 0x3;
    }
    if (det != 0x2 && lppl->iPlayer != -1 && lppl->fStarbase != 0x0) {
        lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
        if (lpshdef->fInclude == 0x0) {
            lpshdef->fInclude = 0x1;
            lpshdef->det = 0x0;
            rgplr[lppl->iPlayer].cshdefSB = rgplr[lppl->iPlayer].cshdefSB + 0x1;
        }
        if (lpshdef->det < 0x3) {
            lpshdef->det = 0x3;
        }
    }
    return;
}

void SetSzWorkFromDt(DtFileType dt, int16_t iPlayer) {
    char    *pchSlash;
    int16_t  c;
    char    *pchDot;
    uint16_t t_merge_8dc4_0001;

    pchDot = strrchr(szBase, 46);
    if (pchDot != 0x0) {
        pchSlash = strrchr(szBase, 92);
        if (pchSlash == 0x0 || pchSlash < pchDot) {
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
        if (dt != dtLog) {
            if (dt != dtHist) {
                t_merge_8dc4_0001 = 0x6d;
            } else {
                t_merge_8dc4_0001 = 0x68;
            }
        } else {
            t_merge_8dc4_0001 = 0x78;
        }
        _wsprintf(&szWork[c], "%c%d", t_merge_8dc4_0001, iPlayer + 1);
    }
    return;
}

int16_t FCreateFile(DtFileType dt, int16_t iPlayer, char *szForceName) {
    jmp_buf *penvMemSav;
    jmp_buf  env;
    char    *psz;

    if (szForceName == 0x0) {
        SetSzWorkFromDt(dt, iPlayer);
        psz = szWork;
    } else {
        psz = szForceName;
    }
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0) {
        StreamOpen(psz, 4114);
        WriteBOF(iPlayer, dt, 0);
        penvMem = penvMemSav;
        return 1;
    }
    penvMem = penvMemSav;
    return 0;
}

void WriteBOF(int16_t iPlayer, int16_t dt, int16_t fMulti) {
    RTBOF    rtbof;
    int16_t  t_scratch_m16;
    uint16_t t_merge_900d_0001;

    memset(&rtbof, 0, sizeof(RTBOF));
    strncpy(rtbof.rgid, "J3J3", 0x4);
    rtbof.lidGame = game.lid;
    rtbof.wGen = game.wGen;
    rtbof.verInc = 0x0;
    rtbof.verMinor = 0x53;
    rtbof.verMajor = 0x2;
    rtbof.turn = game.turn;
    rtbof.fCrippled = 0x0;
    rtbof.iPlayer = iPlayer;
    t_scratch_m16 = Random(2000);
    rtbof.lSaltTime = (int16_t)(LOWORD(GetTickCount()) + t_scratch_m16);
    rtbof.dt = dt;
    rtbof.fDone = gd.fSubmit;
    rtbof.fInUse = gd.fHostMode;
    if (dt != 2 || gd.fGameOverMan == 0x0) {
        t_merge_900d_0001 = 0x0;
    } else {
        t_merge_900d_0001 = 0x1;
    }
    rtbof.fGameOverMan = t_merge_900d_0001;
    WriteRt(rtBOF, 16, &rtbof);
    return;
}

int16_t FMarkFile(DtFileType dt, int16_t iPlayer, int16_t mdMark, int16_t f) {
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
    if (setjmp(env) == 0) {
        fFileErrSilent = 1;
        StreamOpen(szWork, 18);
        fFileErrSilent = fSilentSav;
        ids = idsGameFileAppearsCorruptUnableLoadFile;
        ReadRt();
        if (hdrCur.rt == rtBOF) {
            if ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) >= 0x2 && ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) != 0x2 || (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) >= 0x31)) {
                if ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) <= 0x2 &&
                    ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) != 0x2 || (RawLoad16(&rgbCur[8]) >> 0x5 & 0x7f) < 0x54)) {
                    rtbof = *(RTBOF *)rgbCur;
                    if (game.lid != 0) {
                        if (rtbof.lidGame != game.lid) {
                            FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                        } else {
                            fChange = 0;
                            switch (mdMark) {
                            case 1:
                                if (rtbof.fInUse == f)
                                    goto L_93f5;
                                rtbof.fInUse = f;
                                fChange = 1;
                                goto L_93f5;
                            case 2:
                                if (rtbof.fDone == f)
                                    goto L_93f5;
                                rtbof.fDone = f;
                                fChange = 1;
                                goto L_93f5;
                            case 4:
                                if (rtbof.fMulti == f)
                                    goto L_93f5;
                                rtbof.fMulti = f;
                                fChange = 1;
                                goto L_93f5;
                            case 8:
                                do {
                                    GetFileSeeds(&lSeedSav1, &lSeedSav2);
                                    ReadRt();
                                } while (hdrCur.rt != rtPlr || (int16_t)rgbCur[0] != iPlayer);
                                if ((RawLoad16(&rgbCur[6]) >> 0x9 & 0x1) != f) {
                                    if ((RawLoad16(&rgbCur[6]) >> 0x9 & 0x1) == 0x0) {
                                        RawStore16(&rgbCur[6], (RawLoad16(&rgbCur[6]) & 0xfdff) | 0x200);
                                        RawStore16(&rgbCur[6], (RawLoad16(&rgbCur[6]) & 0x1fff) | 0xe000);
                                    } else {
                                        if ((RawLoad16(&rgbCur[6]) >> 0xd & 0x7) != 0x7)
                                            break;
                                        RawStore16(&rgbCur[6], RawLoad16(&rgbCur[6]) & 0xfdff);
                                    }
                                    RawStore32(&rgbCur[12], ~RawLoad32(&rgbCur[12]));
                                    lseek(hf, (int32_t)-(hdrCur.cb + 0x2), 1);
                                    SetFileSeeds(lSeedSav1, lSeedSav2);
                                    WriteRt(rtPlr, hdrCur.cb, rgbCur);
                                    fChange = dt == dtTurn ? 1 : 0;
                                    rtbof.fDone = 0x0;
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
                } else {
                    FileError(0x2ca);
                }
            } else {
                FileError(0x4d3);
            }
        } else {
            FileError(idmColonistsDroppedDestroyedSpiritedFighting);
        }
        if ((dt & 0x2000) == 0x0 || fSuccess == 0) {
            StreamClose();
        } else {
            lseek(hf, 0, 2);
        }
        penvMem = penvMemSav;
        return fSuccess;
    }
    fFileErrSilent = fSilentSav;
    if (ids != idsUniverseDefinitionFileSeemsMissingCorrupt) {
        FileError(ids);
    }
    StreamClose();
    penvMem = penvMemSav;
    return 0;
}

void WriteRt(RecordType rt, int16_t cb, void *rg) {
    HDR hdr;

    fmemmove(rgbCur, rg, cb);
    if (rt != rtBOF) {
        if (rt != rtEOF) {
            XorFileBuf(rgbCur, cb);
        }
    } else {
        SetFileXorStream(RawLoad32(&rgbCur[4]), (int16_t)RawLoad16(&rgbCur[12]) >> 0x5, RawLoad16(&rgbCur[10]), (int16_t)(RawLoad16(&rgbCur[12]) << 0xb) >> 0xb,
                         RawLoad16(&rgbCur[14]) >> 0xc & 0x1);
    }
    hdr.cb = cb;
    hdr.rt = rt;
    RgToStream(&hdr, 0x2);
    RgToStream(rgbCur, cb);
    return;
}

void RgToStream(void *rg, uint16_t cb) {
    if (cb != 0x0 && _lwrite(hf, rg, cb) != cb) {
        AlertSz(PszFormatIds(idsErrorWritingFile, 0x0), MB_ICONHAND);
        StarsLongJump(penvMem, -1);
    }
    return;
}

void SetVisiblePlanFleet(int16_t iPlr) {
    SetVisPFInit(iPlr);
    if (iPlr != -1) {
        if (iPlr != -1) {
            UpdateProgressGauge(-927);
        }
        SetVisPFFleets(iPlr);
        if (iPlr != -1) {
            UpdateProgressGauge(-927);
        }
        SetVisPFPlanets(iPlr);
        if (iPlr != -1) {
            UpdateProgressGauge(-927);
        }
        SetVisPFThings(iPlr);
        SetVisPFFinish(iPlr);
    } else {
        rgplr[0].cPlanet = game.cPlanMax;
    }
    return;
}

void SetVisPFInit(int16_t iPlr) {
    PLANET  *lpplMac;
    uint16_t detNew;
    PLANET  *lppl;
    int16_t  j;
    FLEET   *lpfl;
    THING   *lpth;
    int16_t  ifl;
    int16_t  i;
    THING   *lpthMac;
    int16_t  raMajor;
    uint16_t grbitPlr;
    int16_t  iSteal;

    raMajor = GetRaceStat(&rgplr[iPlr], rsMajorAdv);
    grbitPlr = iPlr == -1 ? 0x0 : 0x1 << iPlr;
    for (i = 0; i < game.cPlayer; i++) {
        rgplr[i].cPlanet = 0;
    }
    for (i = 0; i < game.cPlayer; i++) {
        if (iPlr != -1 && iPlr != i && rgplr[i].fDead == 0x0) {
            rgplr[i].wMdPlr = rgplr[i].wMdPlr & 0xfeff;
        } else {
            rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
            rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 0x7;
        }
        rgplr[i].cFleet = 0x0;
        rgplr[i].cShDef = 0;
        rgplr[i].cshdefSB = 0x0;
        for (j = 0; j < 16; j++) {
            if ((iPlr != -1 && iPlr != i) || rglpshdef[i][j].fFree != 0x0) {
                rglpshdef[i][j].wFlags = rglpshdef[i][j].wFlags & 0xfeff;
            } else {
                rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfeff) | 0x100;
                rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | 0x7;
                rglpshdef[i][j].cExist = 0x0;
                rgplr[i].cShDef = rgplr[i].cShDef + 1;
            }
        }
        for (j = 0; j < 10; j++) {
            if ((iPlr != -1 && iPlr != i) || rglpshdefSB[i][j].fFree != 0x0) {
                rglpshdefSB[i][j].wFlags = rglpshdefSB[i][j].wFlags & 0xfeff;
            } else {
                rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100;
                rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | 0x7;
                rglpshdefSB[i][j].cExist = 0x0;
                rgplr[i].cshdefSB = rgplr[i].cshdefSB + 0x1;
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (iPlr != -1 && iPlr != lppl->iPlayer) {
            lppl->fInclude = 0x0;
        } else {
            lppl->fInclude = 0x1;
            lppl->det = 0x7;
            if (iPlr != -1) {
                rgplr[iPlr].cPlanet = rgplr[iPlr].cPlanet + 1;
            }
            if (lppl->fStarbase != 0x0) {
                rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist + 0x1;
            }
        }
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        lpfl->fdirValid = 0x0;
        lpfl->fMark = 0x0;
        if ((iPlr != -1 && iPlr != lpfl->iPlayer) || lpfl->fDead != 0x0) {
            lpfl->fInclude = 0x0;
        } else {
            lpfl->fInclude = 0x1;
            lpfl->det = 0x7;
            rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 0x1;
            for (j = 0; j < 16; j++) {
                if (lpfl->rgcsh[j] != 0) {
                    rglpshdef[lpfl->iPlayer][j].cExist = rglpshdef[lpfl->iPlayer][j].cExist + (int32_t)lpfl->rgcsh[j];
                }
            }
            if (iPlr != -1 && lpfl->idPlanet != -1) {
                lppl = lpPlanets + lpfl->idPlanet;
                detNew = GetCachedFleetScannerRange(lpfl, 0x0, 0x0, &iSteal) < 0 ? 0x1 : 0x3;
                if (iSteal >= 2) {
                    detNew = 0x4;
                }
                if (lpfl->fHereAllTurn != 0x0 && lppl->iPlayer == -1 && lpfl->lpplord->rgord[0].grTask == grTaskMine && CMineFromLpfl(lpfl) > 0) {
                    detNew = 0x4;
                }
                MarkPlanet(lppl, iPlr, detNew);
            }
        }
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        switch (lpth->ith) {
        case ithMineralPacket:
            if (iPlr != -1 && raMajor != 6) {
                lpth->thp.fInclude = 0x0;
                break;
            }
            lpth->thp.fInclude = 0x1;
            if (rgplr[lpth->iplr].fInclude != 0x0)
                break;
            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3;
            break;
        case ithWormhole:
            lpth->thw.fInclude = iPlr == -1 ? 0x1 : 0x0;
            break;
        case ithMysteryTrader:
            lpth->tht.fInclude = 0x1;
            break;
        case ithMinefield:
            if ((lpth->thm.grbitPlrNow & grbitPlr) != 0x0 && rgplr[lpth->iplr].fInclude == 0x0) {
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3;
            }
        default:
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
    int16_t  t_call_a39c;
    int16_t  t_call_a3ba;
    int16_t  t_call_aa5f;
    int16_t  t_call_aa88;

    grbitPlr = iPlr == -1 ? 0x0 : 0x1 << iPlr;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->fDead == 0x0) {
            if (lpfl->iPlayer == iPlr) {
                if (lpfl->fBombed != 0x0 && lpfl->idPlanet != -1) {
                    MarkPlanet(lpPlanets + lpfl->idPlanet, iPlr, 0x3);
                }
                iRadius = GetCachedFleetScannerRange(lpfl, &iRadPlanet, &pctDetect, &iSteal);
                iRadius = 0 <= iRadius ? iRadius : 0;
                lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
                lRadPlanet2 = (uint32_t)((int32_t)iRadPlanet * (int32_t)iRadPlanet);
                pt = lpfl->pt;
                for (j = 0; j < cFleet; j++) {
                    lpfl2 = rglpfl[j];
                    if (rglpfl[j] == 0x0)
                        break;
                    if (lpfl2->fDead == 0x0) {
                        if ((iSteal & 0x1) != 0x0 && pt.x == lpfl2->pt.x && pt.y == lpfl2->pt.y && (lpfl2->fInclude == 0x0 || lpfl2->det < 0x4)) {
                            MarkFleet(lpfl2, 4);
                        }
                        if (lpfl2->fInclude == 0x0) {
                            t_call_a39c = abs(pt.x - lpfl2->pt.x);
                            dx = t_call_a39c;
                            if (t_call_a39c <= iRadius) {
                                t_call_a3ba = abs(pt.y - lpfl2->pt.y);
                                dy = t_call_a3ba;
                                if (t_call_a3ba <= iRadius) {
                                    l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                    if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2 &&
                                        (lpfl2->idPlanet == -1 || l <= lRadPlanet2)) {
                                        pctCloak = PctCloakFromLpfl(lpfl2);
                                        if (pctDetect != 100) {
                                            pctCloak = (int32_t)(pctCloak * pctDetect) / 100;
                                        }
                                        if (pctCloak != 0) {
                                            if (l <= (int32_t)((int32_t)((int32_t)((int32_t)(lRadius2 * (int32_t)(100 - pctCloak)) / 0x64) *
                                                                         (int32_t)(100 - pctCloak)) /
                                                               0x64) &&
                                                (lpfl2->idPlanet == -1 ||
                                                 l <= (int32_t)((int32_t)((int32_t)((int32_t)(lRadPlanet2 * (int32_t)(100 - pctCloak)) / 0x64) *
                                                                          (int32_t)(100 - pctCloak)) /
                                                                0x64))) {
                                                MarkFleet(lpfl2, 3);
                                            }
                                        } else {
                                            MarkFleet(lpfl2, 3);
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
                            if ((lpth->ith != ithMinefield || (lpth->thm.grbitPlrNow & grbitPlr) == 0x0) &&
                                (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0x0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0x0) &&
                                (lpth->ith != ithWormhole || lpth->thw.fInclude == 0x0)) {
                                dx = abs(pt.x - lpth->pt.x);
                                dy = abs(pt.y - lpth->pt.y);
                                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if (l <= lRadius2 || lpth->ith == ithMinefield) {
                                    switch (lpth->ith) {
                                    case ithMineralPacket:
                                        lpth->thp.fInclude = 0x1;
                                        goto LThIncPlr;
                                    case ithMysteryTrader:
                                        lpth->tht.fInclude = 0x1;
                                        break;
                                    case ithWormhole:
                                        if ((lpth->thw.grbitPlr & grbitPlr) != 0x0 || l <= (int32_t)(lRadius2 >> 0x4) || l <= lRadPlanet2) {
                                            lpth->thw.grbitPlr = lpth->thw.grbitPlr | grbitPlr;
                                            lpth->thw.fInclude = 0x1;
                                            break;
                                        }
                                        break;
                                    default:
                                        if (((lpth->thm.grbitPlr & grbitPlr) != 0x0 && l <= lRadius2) || l <= lRadPlanet2 || l <= (int32_t)(lRadius2 >> 0x4) ||
                                            l <= lpth->thm.cMines) {
                                            lpth->thm.grbitPlr = lpth->thm.grbitPlr | grbitPlr;
                                            lpth->thm.grbitPlrNow = lpth->thm.grbitPlrNow | grbitPlr;
                                            goto LThIncPlr;
                                        }
                                    }
                                    break;
                                LThIncPlr:
                                    if (rgplr[lpth->iplr].fInclude == 0x0) {
                                        rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                                        rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3;
                                    }
                                }
                            }
                        default:
                        }
                    }
                }
                if ((iSteal & 0x2) != 0x0 && lpfl->idPlanet != -1) {
                    MarkPlanet(lpPlanets + lpfl->idPlanet, iPlr, 0x4);
                }
                if (iRadPlanet > 0) {
                    iRadius = iRadPlanet;
                    lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
                    pt = lpfl->pt;
                    lppl = lpPlanets;
                    lpplMac = lpPlanets + cPlanet;
                    for (; lppl < lpplMac; lppl++) {
                        if (lppl->fInclude == 0x0 || lppl->det < 0x3) {
                            t_call_aa5f = abs(rgptPlan[lppl->id].x - pt.x);
                            dx = t_call_aa5f;
                            if (t_call_aa5f <= iRadius) {
                                t_call_aa88 = abs(rgptPlan[lppl->id].y - pt.y);
                                dy = t_call_aa88;
                                if (t_call_aa88 <= iRadius) {
                                    d2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                    if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                        if (lppl->fStarbase != 0x0 && lppl->iPlayer != -1) {
                                            lVis2 = rglpshdefSB[lppl->iPlayer][lppl->isb].lVisible;
                                            if (lVis2 < 10000 && d2 > (int32_t)((int32_t)(lRadius2 * lVis2) / 10000)) {
                                                MarkPlanet(lppl, iPlr, 0x2);
                                                continue;
                                            }
                                        }
                                        MarkPlanet(lppl, iPlr, 0x3);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if (lpfl->fInclude == 0x0 && lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].iPlayer == iPlr) {
                MarkFleet(lpfl, 3);
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
    int16_t  t_call_ac7a;
    int16_t  t_call_add8;
    int16_t  t_call_adf6;
    int16_t  t_call_b188;
    int16_t  t_call_b1a6;
    int16_t  t_call_b5eb;
    int16_t  t_call_b614;
    int16_t  t_call_b861;
    int16_t  t_call_b88a;

    grbitPlr = iPlr == -1 ? 0x0 : 0x1 << iPlr;
    fStargateView = 0;
    if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raStargate) {
        for (i = 0; i < 10; i++) {
            rgStargateRange[i] = 0;
            if (rglpshdefSB[iPlr][i].fFree == 0x0) {
                t_call_ac7a = StargateRangeFromLppl(0x0, iPlr, i);
                rgStargateRange[i] = t_call_ac7a;
                if (t_call_ac7a > 0) {
                    fStargateView = 1;
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(-927);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
            lRadPlanet2 = (uint32_t)((int32_t)iRadPlanet * (int32_t)iRadPlanet);
            pt = rgptPlan[lppl->id];
            for (j = 0; j < cFleet; j++) {
                lpfl2 = rglpfl[j];
                if (rglpfl[j] == 0x0)
                    break;
                if (lpfl2->fInclude == 0x0 && lpfl2->fDead == 0x0) {
                    t_call_add8 = abs(pt.x - lpfl2->pt.x);
                    dx = t_call_add8;
                    if (t_call_add8 <= iRadius) {
                        t_call_adf6 = abs(pt.y - lpfl2->pt.y);
                        dy = t_call_adf6;
                        if (t_call_adf6 <= iRadius) {
                            l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                            if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2 &&
                                (lpfl2->idPlanet == -1 || l <= lRadPlanet2)) {
                                pctCloak = PctCloakFromLpfl(lpfl2);
                                if (pctCloak != 0) {
                                    if (l <=
                                            (int32_t)((int32_t)((int32_t)((int32_t)(lRadius2 * (int32_t)(100 - pctCloak)) / 0x64) * (int32_t)(100 - pctCloak)) /
                                                      0x64) &&
                                        (lpfl2->idPlanet == -1 ||
                                         l <= (int32_t)((int32_t)((int32_t)((int32_t)(lRadPlanet2 * (int32_t)(100 - pctCloak)) / 0x64) *
                                                                  (int32_t)(100 - pctCloak)) /
                                                        0x64))) {
                                        MarkFleet(lpfl2, 3);
                                    }
                                } else {
                                    MarkFleet(lpfl2, 3);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(-927);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
            lRadPlanet2 = (uint32_t)((int32_t)iRadPlanet * (int32_t)iRadPlanet);
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
                        if ((lpth->ith != ithMinefield || (lpth->thm.grbitPlrNow & grbitPlr) == 0x0) &&
                            (lpth->ith != ithMysteryTrader || lpth->tht.fInclude == 0x0) && (lpth->ith != ithMineralPacket || lpth->thp.fInclude == 0x0) &&
                            (lpth->ith != ithWormhole || lpth->thw.fInclude == 0x0)) {
                            t_call_b188 = abs(pt.x - lpth->pt.x);
                            dx = t_call_b188;
                            if (t_call_b188 <= iRadius) {
                                t_call_b1a6 = abs(pt.y - lpth->pt.y);
                                dy = t_call_b1a6;
                                if (t_call_b1a6 <= iRadius) {
                                    l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                    if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                        switch (lpth->ith) {
                                        case ithMineralPacket:
                                            lpth->thp.fInclude = 0x1;
                                            goto LThIncPlr2;
                                        case ithMysteryTrader:
                                            lpth->tht.fInclude = 0x1;
                                            break;
                                        case ithWormhole:
                                            if ((lpth->thw.grbitPlr & grbitPlr) != 0x0 || l <= (int32_t)(lRadius2 >> 0x4) || l <= lRadPlanet2) {
                                                lpth->thw.grbitPlr = lpth->thw.grbitPlr | grbitPlr;
                                                lpth->thw.fInclude = 0x1;
                                                break;
                                            }
                                            break;
                                        default:
                                            if ((lpth->thm.grbitPlr & grbitPlr) != 0x0 || l <= lRadPlanet2 || l <= (int32_t)(lRadius2 >> 0x4)) {
                                                lpth->thm.grbitPlr = lpth->thm.grbitPlr | grbitPlr;
                                                lpth->thm.grbitPlrNow = lpth->thm.grbitPlrNow | grbitPlr;
                                                goto LThIncPlr2;
                                            }
                                        }
                                        break;
                                    LThIncPlr2:
                                        if (rgplr[lpth->iplr].fInclude == 0x0) {
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfeff) | 0x100;
                                            rgplr[lpth->iplr].wMdPlr = (rgplr[lpth->iplr].wMdPlr & 0xfff8) | 0x3;
                                        }
                                    }
                                }
                            }
                        }
                    default:
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(-927);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
            lRadPlanet2 = (uint32_t)((int32_t)iRadPlanet * (int32_t)iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (fStargateView != 0 && lppl->fStarbase != 0x0 && rgStargateRange[lppl->isb] > 0) {
                iRadius = rgStargateRange[lppl->isb];
                lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if ((lppl2->fInclude == 0x0 || lppl2->det < 0x3) && lppl2->fStarbase != 0x0 && StargateRangeFromLppl(lppl2, 0, 0) != 0) {
                        if (iRadius < 10000) {
                            t_call_b5eb = abs(rgptPlan[lppl2->id].x - pt.x);
                            dx = t_call_b5eb;
                            if (t_call_b5eb > iRadius)
                                continue;
                            t_call_b614 = abs(rgptPlan[lppl2->id].y - pt.y);
                            dy = t_call_b614;
                            if (t_call_b614 > iRadius)
                                continue;
                            d2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                            if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) > lRadius2)
                                continue;
                            lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                            if (lVis2 < 10000 && d2 > (int32_t)((int32_t)(lRadius2 * lVis2) / 10000))
                                continue;
                        }
                        MarkPlanet(lppl2, iPlr, 0x3);
                    }
                }
            }
        }
    }
    if (iPlr != -1) {
        UpdateProgressGauge(-927);
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == iPlr) {
            iRadius = GetPlanetScannerRange(lppl, &iRadPlanet);
            lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
            lRadPlanet2 = (uint32_t)((int32_t)iRadPlanet * (int32_t)iRadPlanet);
            pt = rgptPlan[lppl->id];
            if (iRadPlanet > 0) {
                iRadius = iRadPlanet;
                lRadius2 = lRadPlanet2;
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if (lppl2->fInclude == 0x0 || lppl2->det < 0x3) {
                        t_call_b861 = abs(rgptPlan[lppl2->id].x - pt.x);
                        dx = t_call_b861;
                        if (t_call_b861 <= iRadius) {
                            t_call_b88a = abs(rgptPlan[lppl2->id].y - pt.y);
                            dy = t_call_b88a;
                            if (t_call_b88a <= iRadius) {
                                d2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                    if (lppl2->fStarbase != 0x0 && lppl2->iPlayer != -1) {
                                        lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                                        if (lVis2 < 10000 && d2 > (int32_t)((int32_t)(lRadius2 * lVis2) / 10000)) {
                                            MarkPlanet(lppl2, iPlr, 0x2);
                                            continue;
                                        }
                                    }
                                    MarkPlanet(lppl2, iPlr, 0x3);
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
    int16_t  t_call_bb81;
    int16_t  t_call_bb9f;
    int16_t  t_call_bde6;
    int16_t  t_call_be04;
    int16_t  t_call_c06f;
    int16_t  t_call_c098;
    int16_t  t_call_c327;
    int16_t  t_call_c353;

    grbitPlr = iPlr == -1 ? 0x0 : 0x1 << iPlr;
    if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) != raMassAccel) {
        if (GetRaceStat(&rgplr[iPlr], rsMajorAdv) == raMines) {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (lpth->ith == ithMinefield && lpth->iplr == iPlr) {
                    lRadius2 = lpth->thm.cMines;
                    pt = lpth->pt;
                    for (j = 0; j < cFleet; j++) {
                        lpfl2 = rglpfl[j];
                        if (rglpfl[j] == 0x0)
                            break;
                        if (lpfl2->fInclude == 0x0 && lpfl2->fDead == 0x0 && lpfl2->idPlanet == -1) {
                            t_call_c327 = abs(pt.x - lpfl2->pt.x);
                            dx = t_call_c327;
                            if ((int32_t)t_call_c327 <= lRadius2) {
                                t_call_c353 = abs(pt.y - lpfl2->pt.y);
                                dy = t_call_c353;
                                if ((int32_t)t_call_c353 <= lRadius2) {
                                    l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                    if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                        pctCloak = PctCloakFromLpfl(lpfl2);
                                        if (pctCloak == 0 || Random(100) >= pctCloak) {
                                            MarkFleet(lpfl2, 3);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMineralPacket && lpth->iplr == iPlr && lpth->thp.iWarp != 0x0) {
                lpth->thp.fInclude = 0x1;
                iRadius = lpth->thp.iWarp + 4;
                iRadius = iRadius * iRadius;
                lRadius2 = (uint32_t)((int32_t)iRadius * (int32_t)iRadius);
                pt = lpth->pt;
                for (j = 0; j < cFleet; j++) {
                    lpfl2 = rglpfl[j];
                    if (rglpfl[j] == 0x0)
                        break;
                    if (lpfl2->fInclude == 0x0 && lpfl2->fDead == 0x0) {
                        t_call_bb81 = abs(pt.x - lpfl2->pt.x);
                        dx = t_call_bb81;
                        if (t_call_bb81 <= iRadius) {
                            t_call_bb9f = abs(pt.y - lpfl2->pt.y);
                            dy = t_call_bb9f;
                            if (t_call_bb9f <= iRadius) {
                                l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                    pctCloak = PctCloakFromLpfl(lpfl2);
                                    if (pctCloak != 0) {
                                        if (l <=
                                            (int32_t)((int32_t)((int32_t)((int32_t)(lRadius2 * (int32_t)(100 - pctCloak)) / 0x64) * (int32_t)(100 - pctCloak)) /
                                                      0x64)) {
                                            MarkFleet(lpfl2, 3);
                                        }
                                    } else {
                                        MarkFleet(lpfl2, 3);
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
                            if ((lpth2->ith != ithMinefield || (lpth2->thm.grbitPlrNow & grbitPlr) == 0x0) &&
                                (lpth2->ith != ithMysteryTrader || lpth2->tht.fInclude == 0x0) &&
                                (lpth2->ith != ithMineralPacket || lpth2->thp.fInclude == 0x0) && (lpth2->ith != ithWormhole || lpth2->thw.fInclude == 0x0)) {
                                t_call_bde6 = abs(pt.x - lpth2->pt.x);
                                dx = t_call_bde6;
                                if (t_call_bde6 <= iRadius) {
                                    t_call_be04 = abs(pt.y - lpth2->pt.y);
                                    dy = t_call_be04;
                                    if (t_call_be04 <= iRadius) {
                                        l = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                        if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                            switch (lpth2->ith) {
                                            case ithMineralPacket:
                                                lpth2->thp.fInclude = 0x1;
                                                goto LThIncPlr3;
                                            case ithMysteryTrader:
                                                lpth2->tht.fInclude = 0x1;
                                                break;
                                            case ithWormhole:
                                                if ((lpth2->thw.grbitPlr & grbitPlr) != 0x0 || l <= lRadius2) {
                                                    lpth2->thw.grbitPlr = lpth2->thw.grbitPlr | grbitPlr;
                                                    lpth2->thw.fInclude = 0x1;
                                                    break;
                                                }
                                                break;
                                            default:
                                                lpth2->thm.grbitPlr = lpth2->thm.grbitPlr | grbitPlr;
                                                lpth2->thm.grbitPlrNow = lpth2->thm.grbitPlrNow | grbitPlr;
                                                goto LThIncPlr3;
                                            }
                                            break;
                                        LThIncPlr3:
                                            if (rgplr[lpth2->iplr].fInclude == 0x0) {
                                                rgplr[lpth2->iplr].wMdPlr = (rgplr[lpth2->iplr].wMdPlr & 0xfeff) | 0x100;
                                                rgplr[lpth2->iplr].wMdPlr = (rgplr[lpth2->iplr].wMdPlr & 0xfff8) | 0x3;
                                            }
                                        }
                                    }
                                }
                            }
                        default:
                        }
                    }
                }
                lppl2 = lpPlanets;
                lpplMac2 = lpPlanets + cPlanet;
                for (; lppl2 < lpplMac2; lppl2++) {
                    if (lppl2->fInclude == 0x0 || lppl2->det < 0x3) {
                        t_call_c06f = abs(rgptPlan[lppl2->id].x - pt.x);
                        dx = t_call_c06f;
                        if (t_call_c06f <= iRadius) {
                            t_call_c098 = abs(rgptPlan[lppl2->id].y - pt.y);
                            dy = t_call_c098;
                            if (t_call_c098 <= iRadius) {
                                d2 = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
                                if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= lRadius2) {
                                    if (lppl2->fStarbase != 0x0 && lppl2->iPlayer != -1) {
                                        lVis2 = rglpshdefSB[lppl2->iPlayer][lppl2->isb].lVisible;
                                        if (lVis2 < 10000 && d2 > (int32_t)((int32_t)(lRadius2 * lVis2) / 10000)) {
                                            MarkPlanet(lppl2, iPlr, 0x2);
                                            continue;
                                        }
                                    }
                                    MarkPlanet(lppl2, iPlr, 0x3);
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
                if ((0x1 << iPlr & rglpshdef[i][j].grbitPlr) == 0x0) {
                    if (rglpshdef[i][j].fInclude == 0x0)
                        continue;
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | (detMajor & 0xff);
                } else {
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfeff) | 0x100;
                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xff00) | 0x7;
                }
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 0x3;
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                rgplr[i].cShDef = rgplr[i].cShDef + 1;
            }
            rgplr[i].cshdefSB = 0x0;
            for (j = 0; j < 10; j++) {
                if ((0x1 << iPlr & rglpshdefSB[i][j].grbitPlr) == 0x0) {
                    if (rglpshdefSB[i][j].fInclude == 0x0)
                        continue;
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | (detMajor & 0xff);
                } else {
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfeff) | 0x100;
                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xff00) | 0x7;
                }
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfeff) | 0x100;
                rgplr[i].wMdPlr = (rgplr[i].wMdPlr & 0xfff8) | 0x3;
                rgplr[i].cshdefSB = rgplr[i].cshdefSB + 0x1;
            }
        }
    }
    return;
}
