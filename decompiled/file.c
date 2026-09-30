#include "common.h"

char     mpishdefishTutor[6] = {3, 4, 9, 6, 7, 14};
uint32_t bogi[25] = {0,          2758532406, 2759089752, 2759620184, 2772193450, 2772310925, 2772620565, 2774814161, 2775015431,
                     2776435914, 2777735210, 2777770221, 2777770865, 2781735025, 2781887514, 2782225790, 2782673087, 2783066231,
                     2811254769, 2811333876, 2811336841, 2811341117, 2816596978, 2816992636, 4294967295};

int16_t FReadShDef(RTSHDEF *lprt, SHDEF *lpshdef, int16_t iplrLoad) {
    char     szTemp[40];
    SHDEF    shdef;
    uint8_t *lpb;
    int16_t  ishdef;
    int16_t  cch;
    int16_t  iFirst;
    int16_t  cOut;
    int16_t  fOkay;
    HUL     *lphulBase;
    uint32_t wt;
    int16_t  c;
    HUL     *lphul;
    PART     part;
    HS      *t_fields_1;
    uint32_t t_fields_2;
    uint32_t t_fields_3;

    memset(&shdef, 0, sizeof(SHDEF));
    shdef.hul.ihuldef = lprt->ihuldef;
    shdef.wFlags = lprt->wFlags;
    shdef.hul.chs = lprt->chs;
    shdef.hul.ibmp = lprt->ibmp;
    if (shdef.det == 7) {
        shdef.hul.dp = lprt->dp;
        shdef.turn = lprt->turn;
        shdef.cBuilt = lprt->cBuilt;
        shdef.cExist = lprt->cExist;
        lpb = (uint8_t *)lprt->rghs;
        fmemmove(shdef.hul.rghs, lpb, lprt->chs * 4);
        lpb += 4 * lprt->chs;
    } else {
        shdef.hul.wtEmpty = lprt->wtEmpty;
        lpb = &lprt->chs;
    }
    iFirst = LphuldefFromId(shdef.hul.ihuldef)->hul.ibmp;
    if (shdef.hul.ibmp < iFirst || shdef.hul.ibmp >= iFirst + 4) {
        shdef.hul.ibmp = (shdef.hul.ibmp & 3) | iFirst;
    }
    cch = *lpb;
    lpb++;
    if (cch == 0) {
        fstrcpy(shdef.hul.szClass, lpb);
    } else {
        cOut = 32;
        if (cch > 32) {
            return 0;
        }
        fmemmove(szTemp, lpb, cch);
        FDecompressUserString(szTemp, cch, shdef.hul.szClass, &cOut);
    }
    ishdef = shdef.ishdef;
    if (ishdef >= 16) {
        ishdef -= 16;
    }
    if (shdef.det == 7 || lpshdef[ishdef].fFree != 0 || lpshdef[ishdef].det < 7) {
        lpshdef[ishdef] = shdef;
    } else if (shdef.hul.ihuldef != lpshdef[ishdef].hul.ihuldef || shdef.hul.ibmp != lpshdef[ishdef].hul.ibmp) {
        lpshdef[ishdef] = shdef;
    }
    if (idPlayer != -1) {
        UpdateShdefCost(lpshdef + ishdef);
    }
    if (lpshdef[ishdef].det == 7) {
        lphul = &lpshdef[ishdef].hul;
        lphulBase = &LphuldefFromId(lphul->ihuldef)->hul;
        wt = (uint32_t)lphulBase->wtEmpty;
        for (c = 0; c < lphul->chs; c++) {
            if (lphul->rghs[c].cItem > 0) {
                part.hs = lphul->rghs[c];
                fOkay = FLookupPart(&part);
                if (idPlayer == -1) {
                    fOkay = 0;
                }
                if ((part.hs.grhst & lphulBase->rghs[c].grhst) == 0 || ((fOkay > 1 && shdef.fGift == 0) || part.hs.cItem > lphulBase->rghs[c].cItem)) {
                    lphul->rghs[c].cItem = 0;
                }
                wt += (uint32_t)(part.pcom->cMass * lphul->rghs[c].cItem);
            }
            if (c == 0 && lphul->rghs[0].cItem == 0 && lphulBase->rghs[0].grhst == hstEngine) {
                lphul->rghs[0].grhst = hstEngine;
                lphul->rghs[0].iItem = 1;
                lphul->rghs[0].cItem = lphulBase->rghs[0].cItem;
                part.hs.grhst = lphul->rghs[0].grhst;
                t_fields_1 = &part.hs;
                t_fields_2 = lphul->rghs[0].iItem;
                t_fields_3 = lphul->rghs[0].cItem;
                t_fields_1->iItem = t_fields_2;
                t_fields_1->cItem = t_fields_3;
                FLookupPart(&part);
                wt += (uint32_t)(part.pcom->cMass * lphul->rghs[0].cItem);
            }
        }
        lphul->wtEmpty = LOWORD(wt);
    }
    return 1;
}

void ReadRtPlr(PLAYER *pplr, uint8_t *pbIn) {
    int16_t iOff;
    PLAYER *pplrRaw;
    int16_t cOut;
    char   *psz;

    pplrRaw = (PLAYER *)pbIn;
    memset(pplr, 0, sizeof(PLAYER));
    if (pplrRaw->det == 7) {
        memmove(pplr, pbIn, 112);
        memmove(pplr->rgmdRelation, pbIn + 113, pbIn[112]);
        iOff = 112 + pbIn[112] + 1;
    } else {
        memmove(pplr, pbIn, 8);
        iOff = 8;
    }
    if (pbIn[iOff] == 0) {
        strcpy(pplr->szName, (char *)(pbIn + (iOff + 1)));
        iOff += strlen(pplr->szName) + 2;
    } else {
        cOut = 32;
        FDecompressUserString((char *)(pbIn + (iOff + 1)), pbIn[iOff], pplr->szName, &cOut);
        iOff += pbIn[iOff] + 1;
    }
    if ((wVersFile >> 5 & 0x7f) < 55) {
        psz = PszPlayerName(0, isupper((int16_t)(int8_t)pplr->szName[0]), 1, 0, 0, pplr);
        strcpy(pplr->szNames, psz);
    } else if (pbIn[iOff] == 0) {
        strcpy(pplr->szNames, (char *)(pbIn + (iOff + 1)));
    } else {
        cOut = 32;
        FDecompressUserString((char *)(pbIn + (iOff + 1)), pbIn[iOff], pplr->szNames, &cOut);
    }
    pplr->fLearned = 0;
    return;
}

int16_t FLoadGame(char *pszFileName, char *pszExt) {
    int16_t  iplrSav;
    int16_t  cPlanetHist;
    STARPACK sp;
    int16_t  cPlanetAlloc;
    int16_t  fHaveHistoryData;
    jmp_buf *penvMemSav;
    int16_t  fSilentSav;
    PLANET  *lppl;
    int16_t  i;
    THING   *lpth;
    FLEET   *lpfl;
    jmp_buf  env;
    int16_t  cturn;
    THING   *lpthMac;
    int16_t  iPlayer;
    int16_t  j;
    PLANET  *lpplMac;
    int16_t  dt;
    int16_t  grf;
    int16_t  x;
    POINT16  pt;
    int16_t  iplr;
    SCOREX   sx;
    int16_t  isx;
    uint16_t turnCur;
    uint8_t *lpb;
    int16_t  cThingFile;
    int16_t  fHist;
    int16_t  iP;
    int16_t  fWorking;
    int16_t  iprod;
    int16_t  iFirst;
    int16_t  iLast;
    PROD    *lpprod;
    int16_t  iWarp;
    int16_t  fTwo;
    char     szT[256];
    char     szIniFile[16];
    char     szSection[16];
    char    *psz;
    char     szEntry[16];
    void    *t_call_1e2a;
    int32_t  t_call_2884;

    grf = 0;
    cturn = 0;
    strcpy(szBase, pszFileName);
    gd.fFleetLinkValid = 0;
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) == 0 && FOpenFile(dtXY, -1, 32) != 0) {
        ReadRt();
        if (hdrCur.rt == rtGame) {
            game = *(GAME *)rgbCur;
            game.fDirty = 0;
            dGal = 400 * game.mdSize + 400;
            dGalInv = dGal + 2000;
            x = 1000;
            for (i = 0; i < game.cPlanMax; i++) {
                RgFromStream(&sp, 4);
                x += sp.dx;
                rgptPlan[i].x = x;
                rgptPlan[i].y = sp.y;
                rgidPlan[i] = sp.id;
                if (x >= dGal + 1000 || rgptPlan[i].y >= dGal + 1000 || rgidPlan[i] > 999)
                    goto XYCorrupt;
            }
            ReadRt();
            if (hdrCur.rt == rtEOF) {
                StreamClose();
                if (((int16_t)(int8_t)*pszExt == 'h' || (int16_t)(int8_t)*pszExt == 'H') &&
                    ((int16_t)(int8_t)pszExt[1] == 's' || (int16_t)(int8_t)pszExt[1] == 'S')) {
                    dt = 2;
                    iPlayer = -1;
                } else {
                    dt = 3;
                    grf |= 0x3000;
                    iPlayer = atoi(pszExt + 1);
                    iPlayer--;
                }
                ResetMessages();
                memset(rgplr, 0, game.cPlayer * 192);
                ResetHb(htShips);
                idPlayer = iPlayer;
                fSilentSav = fFileErrSilent;
                fFileErrSilent = 1;
                if (iPlayer != -1 && FOpenFile(dtHist, iPlayer, 32) != 0) {
                    ReadRt();
                    if (hdrCur.rt == rtHistHdr) {
                        cPlanetHist = RawLoad16(rgbCur);
                        cPlanetAlloc = cPlanetHist + RawLoad16(&rgbCur[2]);
                        if (cPlanetAlloc > 1000) {
                            cPlanetAlloc = 1000;
                        }
                        lpPlanets = LpAlloc((1 <= cPlanetAlloc ? cPlanetAlloc : 1) * sizeof(PLANET), htPlanets);
                        ReadRt();
                        i = 0;
                        lppl = lpPlanets;
                        while (i < cPlanetHist) {
                            if (hdrCur.rt != rtPlanetB || FReadPlanet(iPlayer, lppl, 1, 0) == 0)
                                goto CorruptHist;
                            if (lppl->iPlayer == iPlayer) {
                                lppl->iPlayer = -1;
                                lppl->det = 3;
                            }
                            ReadRt();
                            i++;
                            lppl++;
                        }
                        if (hdrCur.rt == rtMsgFilt) {
                            if (hdrCur.cb > (uint16_t)cbbitfMsg)
                                goto CorruptHist;
                            memcpy(bitfMsgFiltered, rgbCur, hdrCur.cb);
                            ReadRt();
                        }
                        while (hdrCur.rt == rtPlr) {
                            i = (int16_t)(int8_t)rgbCur[0];
                            ReadRtPlr(&rgplr[i], rgbCur);
                            rgplr[i].cPlanet = 0;
                            rgplr[i].cFleet = 0;
                            ReadRt();
                        }
                        i = 0;
                        while (hdrCur.rt == rtShDef) {
                            for (; rgplr[i].cShDef == 0 && i < game.cPlayer; i++) {
                            }
                            if (i == game.cPlayer)
                                break;
                            if (rglpshdef[i] == 0) {
                                rglpshdef[i] = LpAlloc(16 * sizeof(SHDEF), htShips);
                                for (j = 0; j < 16; j++) {
                                    rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfdff) | 0x200;
                                    rglpshdef[i][j].grbitPlr = 0;
                                }
                            }
                            iplrSav = idPlayer;
                            if (idPlayer == -1) {
                                idPlayer = i;
                            } else {
                                idPlayer = -1;
                            }
                            if (FReadShDef((RTSHDEF *)rgbCur, rglpshdef[i], iplrSav) == 0)
                                goto CorruptHist;
                            idPlayer = iplrSav;
                            rgplr[i].cShDef--;
                            ReadRt();
                        }
                        i = 0;
                        while (hdrCur.rt == rtShDef) {
                            for (; rgplr[i].cshdefSB == 0 && i < game.cPlayer; i++) {
                            }
                            if (i == game.cPlayer)
                                break;
                            if (rglpshdefSB[i] == 0) {
                                rglpshdefSB[i] = LpAlloc(10 * sizeof(SHDEF), htShips);
                                for (j = 0; j < 10; j++) {
                                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfdff) | 0x200;
                                    rglpshdefSB[i][j].grbitPlr = 0;
                                }
                            }
                            iplrSav = idPlayer;
                            if (idPlayer == -1) {
                                idPlayer = i;
                            } else {
                                idPlayer = -1;
                            }
                            if (FReadShDef((RTSHDEF *)rgbCur, rglpshdefSB[i], iplrSav) == 0)
                                goto CorruptHist;
                            idPlayer = iplrSav;
                            rgplr[i].cshdefSB += 15;
                            ReadRt();
                        }
                        while (hdrCur.rt == rtScore) {
                            iplr = RawLoad16(rgbCur) & 0x1f;
                            sx = *(SCOREX *)rgbCur;
                            if (rgsxPlr[iplr] == 0) {
                                rgsxPlr[iplr] = LpAlloc(101 * sizeof(SCOREX), htMisc);
                                rgcsxPlr[iplr] = 0;
                            }
                            if (rgsxPlr[iplr] != 0) {
                                if (sx.fHistory != 0) {
                                    turnCur = sx.turn;
                                } else {
                                    turnCur = game.turn;
                                }
                                for (isx = 0; isx < rgcsxPlr[iplr] && turnCur > rgsxPlr[iplr][isx].turn; isx++) {
                                }
                                if ((isx >= rgcsxPlr[iplr] || turnCur == rgsxPlr[iplr][isx].turn) && isx < 101) {
                                    if (isx == rgcsxPlr[iplr]) {
                                        rgcsxPlr[iplr]++;
                                    }
                                } else if (rgcsxPlr[iplr] < 101) {
                                    fmemmove(rgsxPlr[iplr] + (isx + 1), rgsxPlr[iplr] + isx, (rgcsxPlr[iplr] - isx) * sizeof(SCOREX));
                                    rgcsxPlr[iplr]++;
                                } else if (isx > 0) {
                                    if (isx > 1) {
                                        fmemmove(rgsxPlr[iplr], rgsxPlr[iplr] + 1, (isx - 1) * sizeof(SCOREX));
                                    }
                                    isx--;
                                }
                                rgsxPlr[iplr][isx] = sx;
                                rgsxPlr[iplr][isx].turn = turnCur;
                                rgsxPlr[iplr][isx].wWord = (rgsxPlr[iplr][isx].wWord & 0x7fff) | 0x8000;
                            }
                            ReadRt();
                        }
                        if (hdrCur.rt == rtAiData) {
                            if (rgplr[idPlayer].fAi != 0) {
                                if (vlpbAiData == 0) {
                                    vlpbAiData = LpAlloc(8096, htMisc);
                                    if (vlpbAiData == 0)
                                        goto CorruptHist;
                                }
                                lpb = vlpbAiData;
                                while (hdrCur.rt == rtAiData) {
                                    fmemmove(lpb, rgbCur, hdrCur.cb);
                                    lpb += hdrCur.cb;
                                    ReadRt();
                                }
                            } else {
                                while (hdrCur.rt == rtAiData) {
                                    ReadRt();
                                }
                            }
                        }
                        if (hdrCur.rt == rtThing) {
                            cThing = RawLoad16(rgbCur);
                            cThingAlloc = cThing + 10;
                            if (cThingAlloc > 4050) {
                                cThingAlloc = 4050;
                            }
                            lpThings = LpAlloc(cThingAlloc * sizeof(THING), htThings);
                            if (lpThings == 0)
                                goto CorruptHist;
                            fmemset(lpThings, 0, cThingAlloc * sizeof(THING));
                            ReadRt();
                            i = 0;
                            lpth = lpThings;
                            while (i < cThing) {
                                if (hdrCur.rt != rtThing)
                                    goto CorruptHist;
                                fmemcpy(lpth, rgbCur, hdrCur.cb);
                                ReadRt();
                                i++;
                                lpth++;
                            }
                        }
                        StreamClose();
                        goto L_1526;
                    }
                CorruptHist:
                    StreamClose();
                    AlertSz(PszFormatIds(idsHistoryFileAppearsCorruptHistoricalDataWill, NULL), MB_ICONHAND);
                }
                cPlanetHist = 0;
                FreeLp(lpPlanets, htPlanets);
                lpPlanets = NULL;
                cThing = 0;
                FreeLp(lpThings, htThings);
                lpThings = NULL;
            L_1526:
                fFileErrSilent = fSilentSav;
                GetFileStatus(dt, iPlayer);
                if (FOpenFile(dt | grf, iPlayer, 32) == 0)
                    goto LError;
                if (iPlayer == -1) {
                    gd.fGameOverMan = RawLoad16(&rgbCur[14]) >> 0xb & 1;
                }
                while (1) {
                    cturn++;
                    cPlanet = 0;
                    cFleet = 0;
                    ReadRt();
                    while (hdrCur.rt == rtBtlData || hdrCur.rt == rtContinue) {
                        if (hdrCur.rt != rtContinue) {
                            if (lpbBattleLog == 0) {
                                lpbBattleLog = LpAlloc(0xffc8, htBattle);
                                lpbBattleCur = lpbBattleLog;
                            }
                            if (0xffc8 - (uint32_t)(LOWORD(lpbBattleCur) & 0xffff) < (uint32_t)RawLoad16(&rgbCur[6])) {
                                RawStore16(lpbBattleCur, 0xffff);
                                lpbBattleCur = LpAlloc(0xffc8, htBattle);
                            }
                        }
                        fmemmove(lpbBattleCur, rgbCur, hdrCur.cb);
                        lpbBattleCur += hdrCur.cb;
                        ReadRt();
                    }
                    if (lpbBattleCur != 0) {
                        RawStore16(lpbBattleCur, 0xffff);
                        if ((wVersFile >> 5 & 0x7f) < 80) {
                            UpdateBattleRecords();
                        }
                    }
                    while (hdrCur.rt == rtPlr) {
                        i = (int16_t)(int8_t)rgbCur[0];
                        ReadRtPlr(&rgplr[i], rgbCur);
                        cPlanet += rgplr[i].cPlanet;
                        rgplr[i].cPlanet = 0;
                        cFleet += rgplr[i].cFleet;
                        rgplr[i].cFleet = 0;
                        ReadRt();
                    }
                    if (dt != 2) {
                        lSaltCur = rgplr[iPlayer].lSalt;
                    } else if (hdrCur.rt == rtChgPassword) {
                        lSaltCur = RawLoad32(rgbCur);
                        ReadRt();
                    } else {
                        lSaltCur = 0;
                    }
                    if (FCheckPassword() == 0)
                        break;
                    ReadPlayerMessages();
                    ResetHb(htFleets);
                    ResetHb(htOrd);
                    FreeLp(rglpfl, htMisc);
                    rglpfl = NULL;
                    if (lpPlanets == 0) {
                        cPlanetAlloc = 1 <= cPlanet ? cPlanet : 1;
                        lpPlanets = LpAlloc(cPlanetAlloc * sizeof(PLANET), htPlanets);
                    }
                    lppl = lpPlanets;
                    j = 0;
                    for (i = 0; i < cPlanet; i++) {
                        fHaveHistoryData = 0;
                        if (cPlanetHist != 0) {
                            for (; j < cPlanetHist && (int16_t)(RawLoad16(rgbCur) << 5) >> 5 > lppl->id; lppl++) {
                                j++;
                            }
                            if (j < cPlanetHist && (int16_t)(RawLoad16(rgbCur) << 5) >> 5 == lppl->id) {
                                fHaveHistoryData = 1;
                            } else {
                                if (cPlanetAlloc == cPlanetHist) {
                                    cPlanetAlloc += 8;
                                    lpPlanets = LpReAlloc(lpPlanets, cPlanetAlloc * sizeof(PLANET), htPlanets);
                                    lppl = lpPlanets + j;
                                }
                                if (j < cPlanetHist) {
                                    fmemmove(lppl + 1, lppl, (cPlanetHist - j) * sizeof(PLANET));
                                }
                                cPlanetHist++;
                            }
                        }
                        if (FReadPlanet(iPlayer, lppl, 0, fHaveHistoryData) == 0)
                            goto Corrupt;
                        if (lppl->iPlayer != -1) {
                            rgplr[lppl->iPlayer].cPlanet = rgplr[lppl->iPlayer].cPlanet + 1;
                        }
                        ReadRt();
                        if (hdrCur.rt == rtProdQ) {
                            if (lppl->lpplprod != 0 && lppl->lpplprod->iprodMax <= hdrCur.cb / 4) {
                                FreePl((PL *)lppl->lpplprod);
                                lppl->lpplprod = NULL;
                            }
                            if (lppl->lpplprod == 0) {
                                lppl->lpplprod = (PLPROD *)LpplAlloc(4, hdrCur.cb / 4 + 2, htOrd);
                            }
                            fmemmove(lppl->lpplprod->rgprod, rgbCur, hdrCur.cb);
                            lppl->lpplprod->iprodMac = LOBYTE(hdrCur.cb / 4);
                            ReadRt();
                        }
                        if (cPlanetHist == 0) {
                            lppl++;
                        }
                    }
                    if (cPlanetHist != 0) {
                        cPlanet = cPlanetHist;
                    }
                    for (i = 0; i < game.cPlayer; i++) {
                        if (i == iPlayer) {
                            rglpshdef[i] = rgshdef;
                        } else {
                            if (rgplr[i].fInclude == 0)
                                continue;
                            if (rglpshdef[i] != 0)
                                goto L_1c32;
                            rglpshdef[i] = LpAlloc(16 * sizeof(SHDEF), htShips);
                        }
                        for (j = 0; j < 16; j++) {
                            rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfdff) | 0x200;
                            rglpshdef[i][j].grbitPlr = 0;
                        }
                    L_1c32:
                        iplrSav = idPlayer;
                        if (idPlayer == -1) {
                            idPlayer = i;
                        } else if (i != idPlayer) {
                            idPlayer = -1;
                        }
                        for (j = 0; j < rgplr[i].cShDef; j++) {
                            if (hdrCur.rt != rtShDef) {
                                idPlayer = iplrSav;
                                goto Corrupt;
                            }
                            if (FReadShDef((RTSHDEF *)rgbCur, rglpshdef[i], iplrSav) == 0)
                                goto Corrupt;
                            ReadRt();
                        }
                        idPlayer = iplrSav;
                    }
                    for (i = 0; i < game.cPlayer; i++) {
                        rgplr[i].cShDef = 0;
                        if (rglpshdef[i] != 0) {
                            for (j = 0; j < 16; j++) {
                                if (rglpshdef[i][j].fFree == 0) {
                                    if (i != idPlayer && gd.fGeneratingTurn == 0 && rgplr[i].fDead != 0) {
                                        rglpshdef[i][j].wFlags = (rglpshdef[i][j].wFlags & 0xfdff) | 0x200;
                                    } else {
                                        rgplr[i].cShDef++;
                                    }
                                }
                            }
                        }
                    }
                    rglpfl = LpAlloc((1 <= cFleet ? cFleet : 1) * sizeof(FLEET *), htMisc);
                    for (i = 0; i < cFleet; i++) {
                        t_call_1e2a = LpAlloc(sizeof(FLEET), htFleets);
                        rglpfl[i] = t_call_1e2a;
                        lpfl = t_call_1e2a;
                        if (FReadFleet(lpfl) == 0)
                            goto LError;
                        rgplr[lpfl->iPlayer].cFleet = rgplr[lpfl->iPlayer].cFleet + 1;
                    }
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgplr[i].fInclude != 0) {
                            if (rglpshdefSB[i] == 0) {
                                rglpshdefSB[i] = LpAlloc(10 * sizeof(SHDEF), htShips);
                                for (j = 0; j < 10; j++) {
                                    rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfdff) | 0x200;
                                    rglpshdefSB[i][j].grbitPlr = 0;
                                }
                            }
                            iplrSav = idPlayer;
                            if (idPlayer == -1) {
                                idPlayer = i;
                            } else if (i != idPlayer) {
                                idPlayer = -1;
                            }
                            for (j = 0; j < rgplr[i].cshdefSB; j++) {
                                if (hdrCur.rt != rtShDef) {
                                    idPlayer = iplrSav;
                                    goto Corrupt;
                                }
                                if (FReadShDef((RTSHDEF *)rgbCur, rglpshdefSB[i], iplrSav) == 0)
                                    goto Corrupt;
                                ReadRt();
                            }
                            idPlayer = iplrSav;
                        }
                    }
                    for (i = 0; i < game.cPlayer; i++) {
                        rgplr[i].cshdefSB = 0;
                        if (rglpshdefSB[i] != 0) {
                            for (j = 0; j < 10; j++) {
                                if (rglpshdefSB[i][j].fFree == 0) {
                                    if (i != idPlayer && gd.fGeneratingTurn == 0 && rgplr[i].fDead != 0) {
                                        rglpshdefSB[i][j].wFlags = (rglpshdefSB[i][j].wFlags & 0xfdff) | 0x200;
                                    } else {
                                        rgplr[i].cshdefSB++;
                                    }
                                }
                            }
                        }
                    }
                    if (vlprgScoreX == 0) {
                        vlprgScoreX = LpAlloc(game.cPlayer * sizeof(SCOREX), htMisc);
                        fmemset(vlprgScoreX, 0, game.cPlayer * sizeof(SCOREX));
                    }
                    while (hdrCur.rt == rtScore) {
                        iplr = RawLoad16(rgbCur) & 0x1f;
                        vlprgScoreX[iplr] = *(SCOREX *)rgbCur;
                        if (rgsxPlr[iplr] == 0) {
                            rgsxPlr[iplr] = LpAlloc(101 * sizeof(SCOREX), htMisc);
                            rgcsxPlr[iplr] = 0;
                        }
                        if (rgsxPlr[iplr] != 0) {
                            if (vlprgScoreX[iplr].fHistory != 0) {
                                turnCur = vlprgScoreX[iplr].turn;
                            } else {
                                turnCur = game.turn;
                            }
                            for (isx = 0; isx < rgcsxPlr[iplr] && turnCur > rgsxPlr[iplr][isx].turn; isx++) {
                            }
                            if ((isx >= rgcsxPlr[iplr] || turnCur == rgsxPlr[iplr][isx].turn) && isx < 101) {
                                if (isx == rgcsxPlr[iplr]) {
                                    rgcsxPlr[iplr]++;
                                }
                            } else if (rgcsxPlr[iplr] < 101) {
                                fmemmove(rgsxPlr[iplr] + (isx + 1), rgsxPlr[iplr] + isx, (rgcsxPlr[iplr] - isx) * sizeof(SCOREX));
                                rgcsxPlr[iplr]++;
                            } else if (isx > 0) {
                                if (isx > 1) {
                                    fmemmove(rgsxPlr[iplr], rgsxPlr[iplr] + 1, (isx - 1) * sizeof(SCOREX));
                                }
                                isx--;
                            }
                            rgsxPlr[iplr][isx] = vlprgScoreX[iplr];
                            rgsxPlr[iplr][isx].turn = turnCur;
                            rgsxPlr[iplr][isx].wWord = (rgsxPlr[iplr][isx].wWord & 0x7fff) | 0x8000;
                        }
                        ReadRt();
                    }
                    if (lpThings != 0) {
                        FreeLp(lpThings, htThings);
                        lpThings = NULL;
                        cThing = 0;
                    }
                    if (hdrCur.rt == rtThing) {
                        fHist = cThing <= 0 ? 0 : 1;
                        cThingFile = RawLoad16(rgbCur);
                        cThingAlloc = 10 <= cThingFile ? cThingFile : 10;
                        if (lpThings == 0) {
                            lpThings = LpAlloc(cThingAlloc * sizeof(THING), htThings);
                            if (lpThings == 0)
                                goto LError;
                            fmemset(lpThings, 0, cThingAlloc * sizeof(THING));
                        }
                        ReadRt();
                        lpth = lpThings;
                        j = 0;
                        for (i = 0; i < cThingFile; i++) {
                            fHaveHistoryData = 0;
                            if (fHist != 0) {
                                for (; j < cThing && RawLoad16(rgbCur) > lpth->idFull; lpth++) {
                                    j++;
                                }
                                if (j < cThing && RawLoad16(rgbCur) == lpth->idFull) {
                                    fHaveHistoryData = 1;
                                    goto LFoundThing;
                                }
                                if (cThingAlloc == cThing) {
                                    cThingAlloc += 8;
                                    lpThings = LpReAlloc(lpThings, cThingAlloc * sizeof(THING), htThings);
                                    lpth = lpThings + j;
                                }
                                if (j < cThing) {
                                    fmemmove(lpth + 1, lpth, (cThing - j) * sizeof(THING));
                                    fmemset(lpth, 0, sizeof(THING));
                                }
                            }
                            cThing++;
                        LFoundThing:
                            fmemcpy(lpth, rgbCur, hdrCur.cb);
                            lpth->turn = game.turn;
                            lpth++;
                            j++;
                            ReadRt();
                        }
                    } else {
                        cThing = 0;
                        cThingAlloc = 10;
                        lpThings = LpAlloc(cThingAlloc * sizeof(THING), htThings);
                    }
                    if (hdrCur.rt == rtSel) {
                        ReadRt();
                    }
                    iplrSav = idPlayer;
                    while (hdrCur.rt == rtBtlPlan) {
                        iP = RawLoad16(rgbCur) & 0xf;
                        idPlayer = iP;
                        if (rglpbtlplan[iP] == 0) {
                            rglpbtlplan[iP] = LpAlloc(16 * sizeof(BTLPLAN), htShips);
                        }
                        UnpackBattlePlan(rgbCur, rglpbtlplan[iP] + rgcbtlplan[iP], rgcbtlplan[iP]);
                        rgcbtlplan[iP]++;
                        ReadRt();
                    }
                    idPlayer = iplrSav;
                    if (hdrCur.rt != rtEOF)
                        goto Corrupt;
                    t_call_2884 = filelength(hf);
                    if (t_call_2884 == tell(hf))
                        goto L_2a56;
                    ReadRt();
                    if (hdrCur.rt != rtBOF)
                        goto L_2a35;
                    game.turn = RawLoad16(&rgbCur[10]);
                    game.wGen = RawLoad16(&rgbCur[14]) >> 0xd & 7;
                    for (i = 0; i < game.cPlayer; i++) {
                        rgplr[i].cShDef = 0;
                        rgplr[i].cFleet = 0;
                        rgplr[i].cPlanet = 0;
                        rgplr[i].cshdefSB = 0;
                        rgcbtlplan[i] = 0;
                    }
                    lppl = lpPlanets;
                    lpplMac = lpPlanets + cPlanet;
                    for (; lppl < lpplMac; lppl++) {
                        if (lppl->iPlayer == iPlayer) {
                            lppl->iPlayer = -1;
                            lppl->det = 3;
                            if (lppl->lpplprod != 0) {
                                FreePl((PL *)lppl->lpplprod);
                                lppl->lpplprod = NULL;
                            }
                        }
                    }
                    cPlanetHist = cPlanet;
                }
                if (ini.fValidate == 0 && ini.fLogging == 0)
                    goto LError;
                AlertSz(PszFormatIds(idsPasswordHaveEnteredIncorrectPleaseTry, NULL), MB_ICONHAND);
                goto LError;
            L_2a35:
                AlertSz(PszFormatIds(idsWarningIgnoringUnexpectedDataAfterEof, NULL), MB_ICONHAND);
            L_2a56:
                StreamClose();
                if (cturn > 1 && rgplr[iPlayer].fAi == 0 && ini.fDumpPlanets == 0 && ini.fDumpFleets == 0 && ini.fDumpMap == 0) {
                    _wsprintf(szWork, PszGetCompressedString(idsNoteDYearsDataRead), cturn);
                    AlertSz(szWork, MB_ICONASTERISK);
                }
                if (strnicmp(pszExt, "hst", 3) != 0) {
                    if (rgplr[iPlayer].fAi == 0) {
                        lpth = lpThings;
                        lpthMac = lpThings + cThing;
                        for (; lpth < lpthMac; lpth++) {
                            if (lpth->ith == ithMineralPacket && lpth->thp.iWarp != 0) {
                                lppl = LpplFromId(lpth->thp.idPlanet);
                                if (lppl != 0 && lppl->iPlayer == iPlayer) {
                                    iWarp = IWarpMAFromLppl(lppl, &fTwo);
                                    if (iWarp + fTwo < lpth->thp.iWarp + 4) {
                                        FSendPlrMsg2XGen(0, 337, -6, lpth->idFull, lppl->id);
                                    }
                                }
                            }
                        }
                        if (game.fTutorial == 0) {
                            lppl = lpPlanets;
                            lpplMac = lpPlanets + cPlanet;
                            for (; lppl < lpplMac; lppl++) {
                                if (lppl->iPlayer == iPlayer && lppl->fStarbase != 0 && lppl->lpplprod != 0 &&
                                    rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef != ihuldefOrbitalFort) {
                                    fWorking = 0;
                                    iprod = 0;
                                    lpprod = lppl->lpplprod->rgprod;
                                    while (iprod < lppl->lpplprod->iprodMac) {
                                        EstimateItemProdSched(lppl, NULL, iprod, &iFirst, &iLast);
                                        if (iLast > 1) {
                                            fWorking = 0;
                                            break;
                                        }
                                        if (iLast == 1 && (lpprod->grobj != grobjPlanet || lpprod->iItem >= mdIdleFactory)) {
                                            fWorking = 1;
                                        }
                                        iprod++;
                                        lpprod++;
                                    }
                                    if (fWorking != 0) {
                                        FSendPlrMsg2XGen(0, 338, lppl->id, lppl->id, 0);
                                    }
                                }
                            }
                            i = CBattles();
                            if (i > 0) {
                                FSendPlrMsg2XGen(1, (i <= 1 ? 0 : 1) + 339, -7, i, 0);
                            }
                        }
                    }
                    if (gd.fDontDoLogFiles == 0) {
                        _wsprintf(szWork, "%s.x%s", pszFileName, pszExt + 1);
                        if (FLoadLogFile(szWork) == 0 || FRunLogFile() == 0) {
                            AlertSz(PszFormatIds(idsPlayerLogFileAppearsCorruptUnableLoad, NULL), MB_ICONHAND);
                            goto LError;
                        }
                    }
                    for (i = 0; i < game.cPlayer; i++) {
                        rgplr[i].cFleet = 0;
                        rgplr[i].cPlanet = 0;
                    }
                    lppl = lpPlanets;
                    lpplMac = lpPlanets + cPlanet;
                    for (; lppl < lpplMac; lppl++) {
                        if (lppl->iPlayer != -1) {
                            rgplr[lppl->iPlayer].cPlanet = rgplr[lppl->iPlayer].cPlanet + 1;
                        } else {
                            lppl->fStarbase = 0;
                        }
                    }
                    j = 0;
                    for (i = 0; i < cFleet; i++) {
                        lpfl = rglpfl[i];
                        if (rglpfl[i] == 0)
                            break;
                        j = lpfl->iPlayer;
                        rgplr[j].cFleet++;
                    }
                }
                idPlayer = iPlayer;
                if (idPlayer != -1 && rgplr[idPlayer].fAi == 0 && vrgszMRU != 0) {
                    strcpy(szT, pszFileName);
                    strcat(szT, ".");
                    strcat(szT, pszExt);
                    if (fstricmp(szT, vrgszMRU) != 0) {
                        for (i = 1; i < 8 && fstricmp(szT, vrgszMRU + 256 * i) != 0; i++) {
                        }
                        for (; i >= 1; i--) {
                            fstrcpy(vrgszMRU + 256 * i, vrgszMRU + 256 * (i - 1));
                        }
                        fstrcpy(vrgszMRU, szT);
                        CchGetString(idsStarsIni, szIniFile);
                        CchGetString(idsFiles, szSection);
                        CchGetString(idsFile1, szEntry);
                        psz = &szEntry[strlen(szEntry) - 1];
                        for (i = 0; i < 9; i++) {
                            *psz = LOBYTE(i + 49);
                            fstrcpy(szT, vrgszMRU + 256 * i);
                            WritePrivateProfileString(szSection, szEntry, szT, szIniFile);
                        }
                    }
                }
                return 1;
            Corrupt:
                AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, NULL), MB_ICONHAND);
                goto LError;
            }
        }
    XYCorrupt:
        AlertSz(PszFormatIds(idsUniverseDefinitionFileSeemsMissingCorrupt, NULL), MB_ICONHAND);
    }
LError:
    game.fDirty = 0;
    DestroyCurGame();
    StreamClose();
    if (ini.fValidate == 0 && ini.fLogging == 0 && hwndTitle == 0) {
        pt.x = GetSystemMetrics(SM_CXSCREEN);
        pt.y = GetSystemMetrics(SM_CYSCREEN);
        hwndTitle = CreateWindow(szTitle, "Stars!", WS_POPUP | WS_VISIBLE, 0, 0, pt.x, pt.y, hwndFrame, NULL, hInst, NULL);
        fFreeingTitle = 0;
        ShowWindow(hwndFrame, SW_HIDE);
    }
    return 0;
}

int16_t FReadPlanet(int16_t iPlayer, PLANET *lppl, int16_t fHistory, int16_t fPreInited) {
    int16_t   fFirstYear;
    int16_t   fRouting;
    uint8_t   bMask;
    int16_t   i;
    uint8_t  *pb;
    MessageId idm;
    int16_t   pctOpt;
    int16_t   pct;
    uint8_t  *t_3533;

    fFirstYear = 0;
    if (fPreInited == 0) {
        fmemset(lppl, 0, sizeof(PLANET));
    }
    if (fHistory != 0 || iPlayer == -1) {
        lppl->fFirstYear = RawLoad16(&rgbCur[2]) >> 0xf & 1;
    } else if (fPreInited == 0) {
        fFirstYear = 1;
        lppl->fFirstYear = 1;
    } else if (lppl->fFirstYear != 0) {
        if (lppl->turn != game.turn) {
            lppl->fFirstYear = 0;
        } else {
            fFirstYear = 1;
        }
    }
    lppl->id = (int16_t)(RawLoad16(rgbCur) << 5) >> 5;
    lppl->iPlayer = (int16_t)RawLoad16(rgbCur) >> 0xb;
    if (lppl->det < (RawLoad16(&rgbCur[2]) & 0x7f)) {
        lppl->det = RawLoad16(&rgbCur[2]) & 0x7f;
    }
    lppl->fInclude = RawLoad16(&rgbCur[2]) >> 8 & 1;
    lppl->fStarbase = RawLoad16(&rgbCur[2]) >> 9 & 1;
    lppl->fHomeworld = RawLoad16(&rgbCur[2]) >> 7 & 1;
    fRouting = RawLoad16(&rgbCur[2]) >> 0xe & 1;
    if (lppl->fStarbase != 0 && lppl->iPlayer == -1) {
        lppl->fStarbase = 0;
    }
    if (fHistory == 0) {
        lppl->turn = game.turn;
    }
    pb = &rgbCur[4];
    if ((RawLoad16(&rgbCur[2]) & 0x7f) >= 3) {
        bMask = *pb;
        pb++;
        i = 0;
        while (i < 3) {
            if ((bMask & 3) != 0) {
                if ((bMask & 3) != 1) {
                    return 0;
                }
                lppl->rgpctMinLevel[i] = *pb++;
            } else {
                lppl->rgpctMinLevel[i] = 0;
            }
            i++;
            bMask = LOBYTE(bMask >> 2);
        }
        i = 0;
        while (i < 3) {
            lppl->rgMinConc[i] = *pb;
            i++;
            pb++;
        }
        for (i = 0; i < 3; i++) {
            if (*pb > 100) {
                return 0;
            }
            t_3533 = pb;
            pb++;
            lppl->rgEnvVarOrig[i] = *t_3533;
            lppl->rgEnvVar[i] = *t_3533;
        }
        if ((RawLoad16(&rgbCur[2]) >> 0xa & 1) != 0) {
            for (i = 0; i < 3; i++) {
                if (*pb > 100) {
                    return 0;
                }
                lppl->rgEnvVarOrig[i] = *pb++;
            }
        }
        if ((int16_t)RawLoad16(rgbCur) >> 0xb != -1) {
            lppl->uGuesses = RawLoad16(pb);
            pb += 2;
        }
        if (lppl->det <= 3)
            goto LFinishBRecord;
        if ((RawLoad16(&rgbCur[2]) >> 0xd & 1) != 0) {
            bMask = *pb;
            pb++;
            i = 0;
            while (i < 4) {
                switch (bMask & 3) {
                default:
                    break;
                case 0:
                    lppl->rgwtMin[i] = 0;
                    break;
                case 1:
                    lppl->rgwtMin[i] = (uint32_t)*pb++;
                    break;
                case 2:
                    lppl->rgwtMin[i] = (uint32_t)RawLoad16(pb);
                    pb += 2;
                    break;
                case 3:
                    lppl->rgwtMin[i] = RawLoad32(pb);
                    pb += 4;
                }
                i++;
                bMask = LOBYTE(bMask >> 2);
            }
        }
        if (hdrCur.rt == rtPlanetB)
            goto LFinishBRecord;
        if ((RawLoad16(&rgbCur[2]) >> 0xb & 1) != 0) {
            fmemmove(lppl->rgbImp, pb, 8);
            pb += 8;
        } else {
            lppl->fArtifact = RawLoad16(&rgbCur[2]) >> 0xc & 1;
            lppl->iScanner = 31;
            lppl->cDefenses = 0;
        }
        if (lppl->iPlayer != -1) {
            if (lppl->fStarbase != 0) {
                lppl->lStarbase = RawLoad32(pb);
                lppl->fNoHeal = 0;
                pb += 4;
            }
            if (fRouting != 0) {
                lppl->wRouting = RawLoad16(pb);
            }
        }
        return 1;
    }
LFinishBRecord:
    if (lppl->fStarbase != 0) {
        lppl->isb = *pb;
        pb++;
    }
    if (fHistory != 0) {
        lppl->turn = RawLoad16(pb);
        pb += 2;
    } else if (fFirstYear != 0) {
        if (lppl->iPlayer != -1) {
            FSendPlrMsg2XGen(0, 170, lppl->id, lppl->id, lppl->iPlayer | 0x30);
        } else if (lppl->det <= 1) {
            FSendPlrMsg2XGen(0, 173, lppl->id, lppl->id, 0);
        } else if (GetRaceStat(&rgplr[iPlayer], rsMajorAdv) == raTerra) {
            pctOpt = PctPlanetOptValue(lppl, iPlayer);
            FSendPlrMsg2XGen(0, 349, lppl->id, lppl->id, pctOpt);
        } else {
            pct = PctPlanetDesirability(lppl, iPlayer);
            if (pct > 0) {
                pct = PctTrueMaxGrowth(iPlayer) * pct;
                idm = idmHaveFoundNewHabitablePlanetColonistsWill;
            } else {
                pctOpt = PctPlanetOptValue(lppl, iPlayer);
                if (pctOpt > 0) {
                    pct = PctTrueMaxGrowth(iPlayer) * pctOpt;
                    idm = idmHaveFoundNewPlanetWhichHaveAbility;
                } else {
                    pct = 10 * pct;
                    idm = idmHaveFoundNewPlanetWhichUnfortunatelyHabitable;
                }
            }
            FSendPlrMsg2XGen(0, idm, lppl->id, abs(pct), lppl->id);
        }
    }
    return 1;
}

int16_t FReadFleet(FLEET *lpfl) {
    uint16_t  us;
    int16_t   cord;
    int16_t   fByte;
    ORDER    *lpord;
    int16_t   i;
    int16_t   cish;
    uint8_t  *pb;
    int16_t   cch;
    uint16_t *pus;
    char      szT[33];
    int16_t   cOut;

    cish = 0;
    fmemset(lpfl, 0, sizeof(FLEET));
    fmemmove(lpfl, rgbCur, 12);
    fByte = lpfl->fDone;
    us = RawLoad16(&rgbCur[12]);
    pb = &rgbCur[14];
    if (fByte != 0) {
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgcsh[i] = *pb++;
                if (lpfl->rgcsh[i] != 0) {
                    cish++;
                }
            }
            i++;
        }
    } else {
        pus = (uint16_t *)pb;
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgcsh[i] = *pus++;
                if (lpfl->rgcsh[i] != 0) {
                    cish++;
                }
            }
            i++;
        }
        pb = (uint8_t *)pus;
    }
    if (cish == 0) {
        lpfl->fDead = 1;
    }
    if (lpfl->det >= 4) {
        us = RawLoad16(pb);
        pb += 2;
        i = 0;
        while (i < 5) {
            switch (us & 3) {
            default:
                break;
            case 1:
                lpfl->rgwtMin[i] = (uint32_t)*pb;
                pb++;
                break;
            case 2:
                lpfl->rgwtMin[i] = (uint32_t)RawLoad16(pb);
                pb += 2;
                break;
            case 3:
                lpfl->rgwtMin[i] = RawLoad32(pb);
                pb += 4;
            }
            i++;
            us >>= 2;
        }
    }
    if (lpfl->det < 7) {
        lpfl->dirLong = RawLoad32(pb);
        pb += 4;
        lpfl->wtFleet = RawLoad32(pb);
        pb += 4;
        ReadRt();
        return 1;
    }
    if (hdrCur.rt == rtFleetA) {
        us = RawLoad16(pb);
        pb += 2;
        pus = (uint16_t *)pb;
        i = 0;
        for (; us != 0; us >>= 1) {
            if ((us & 1) != 0) {
                lpfl->rgdv[i].dp = *pus++;
                if (lpfl->rgdv[i].pctDp >= 500) {
                    lpfl->rgdv[i].pctDp = 499;
                }
            }
            i++;
        }
        pb = (uint8_t *)pus;
        lpfl->iplan = *pb++;
        lpfl->cord = *pb++;
        lpfl->lpplord = (PLORD *)LpplAlloc(18, lpfl->cord + 1, htOrd);
        fmemset(lpfl->lpplord->rgord, 0, (lpfl->cord + 1) * 18);
        cord = lpfl->cord;
        lpord = lpfl->lpplord->rgord;
        for (; cord != 0; cord--) {
            memset(rgbCur, 0, 18);
            ReadRt();
            if (hdrCur.rt != rtOrderA && hdrCur.rt != rtOrderB)
                goto Corrupt;
            *lpord = *(ORDER *)rgbCur;
            lpord->fNoAutoTrack = 0;
            lpord++;
        }
        lpfl->lpplord->iordMac = LOBYTE(lpfl->cord);
        if (lpfl->idPlanet != -1) {
            if (lpfl->idPlanet > game.cPlanMax) {
                lpfl->idPlanet = -1;
            }
            if (lpfl->pt.x != rgptPlan[lpfl->idPlanet].x || lpfl->pt.y != rgptPlan[lpfl->idPlanet].y) {
                if (i != 0 || game.turn != 0)
                    goto Corrupt;
                lpfl->pt = rgptPlan[lpfl->idPlanet];
            }
        }
        ReadRt();
        if (hdrCur.rt == rtString) {
            cch = (int16_t)(int8_t)rgbCur[0];
            if (cch == 0) {
                lpfl->lpszName = LpAlloc(strlen(&rgbCur[1]) + 1, htString);
                fstrcpy(lpfl->lpszName, &rgbCur[1]);
            } else {
                cOut = 32;
                FDecompressUserString(&rgbCur[1], cch, szT, &cOut);
                lpfl->lpszName = LpAlloc(strlen(szT) + 1, htString);
                fstrcpy(lpfl->lpszName, szT);
            }
            ReadRt();
        } else {
            lpfl->lpszName = NULL;
        }
        return 1;
    }
Corrupt:
    AlertSz(PszFormatIds(idsGameFileAppearsCorruptUnableLoadFile, NULL), MB_ICONHAND);
    return 0;
}

void UnpackBattlePlan(uint8_t *lpb, BTLPLAN *lpbtlplan, int16_t iplan) {
    char    szTemp[33];
    char    szName[33];
    int16_t cch;
    int16_t cOut;

    fmemmove(lpbtlplan, lpb, 4);
    lpb += 4;
    cch = *lpb;
    lpb++;
    if (cch == 0) {
        fstrcpy(lpbtlplan->szName, lpb);
    } else {
        cOut = 32;
        fmemmove(szTemp, lpb, cOut);
        FDecompressUserString(szTemp, cch, szName, &cOut);
        fmemmove(lpbtlplan->szName, szName, cOut);
    }
    lpbtlplan->iplan = iplan;
    return;
}

void UpdateBattleRecords() {
    BTLDATA  *lpbd;
    BTLREC   *lpbr;
    int16_t   cKill;
    HB       *lphb;
    BTLREC26 *lpbr26;
    int16_t   itok;

    lphb = rglphb[11];
    if (lphb != 0) {
        lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
        while (1) {
            if (lpbd->id == 0xffff) {
                lphb = lphb->lphbNext;
                if (lphb == 0 || lphb->ibTop <= sizeof(HB))
                    break;
                lpbd = (BTLDATA *)((uint8_t *)lphb + (sizeof(HB) + 2));
            } else {
                if (lpbd->cbData == 0)
                    break;
                lpbr = (BTLREC *)&lpbd->rgtok[lpbd->ctok];
                lpbr26 = (BTLREC26 *)lpbr;
                lpbd = (BTLDATA *)((uint8_t *)lpbd + lpbd->cbData);
                while (lpbr < (BTLREC *)lpbd) {
                    cKill = lpbr26->ctok;
                    itok = lpbr26->itokAttack;
                    lpbr->ctok = cKill;
                    lpbr->itokAttack = itok;
                    lpbr = (BTLREC *)&lpbr->rgkill[lpbr->ctok];
                    lpbr26 = (BTLREC26 *)lpbr;
                }
            }
        }
    }
    return;
}

INT_PTR CALLBACK AskSaveDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return 1;
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        case IDC_SAVE:
        case IDC_NO_DON_T_SAVE:
        case IDC_SAVESUBMIT:
            EndDialog(hwnd, GET_WM_COMMAND_ID(wParam, lParam) == IDC_NO_DON_T_SAVE ? 0 : GET_WM_COMMAND_ID(wParam, lParam) == IDC_SAVESUBMIT ? -1 : 1);
            return 1;
        case IDC_HELP:
            WinHelp(hwnd, szHelpFile, 1, 1090);
            return 1;
        }
    case WM_DESTROY:
    default:
        return 0;
    }
}

void PromptSaveGame() {
    FARPROC lpProc;
    int16_t fRet;

    lpProc = MakeProcInstance(AskSaveDialog, hInst);
    fRet = DialogBox(hInst, game.fSinglePlr == 0 ? MAKEINTRESOURCE(IDD_SAVE_TURN1) : MAKEINTRESOURCE(IDD_SAVE_TURN2), hwndFrame, lpProc);
    FreeProcInstance(lpProc);
    if (fRet != 0) {
        gd.fSubmit = fRet == -1 ? 1 : 0;
        FWriteLogFile(szBase, idPlayer);
        FWriteHistFile(idPlayer);
    }
    return;
}

void DestroyCurGame() {
    int16_t i;

    if (gd.fSendMsgMode != 0) {
        FFinishPlrMsgEntry(0);
    }
    if (idPlayer != -1 && game.fDirty != 0) {
        PromptSaveGame();
    }
    ResetHb(htPlanets);
    lpPlanets = NULL;
    cPlanet = 0;
    ResetHb(htFleets);
    rglpfl = NULL;
    cFleet = 0;
    ResetHb(htThings);
    lpThings = NULL;
    cThing = 0;
    cThingAlloc = 0;
    vlprgScoreX = NULL;
    vrptFleet.fCached = 0;
    vrptPlanet.fCached = 0;
    vrptBattle.fCached = 0;
    vrptEFleet.fCached = 0;
    for (i = 0; i < 16; i++) {
        rgsxPlr[i] = NULL;
    }
    lpbBattleT = NULL;
    lpbBattleLog = NULL;
    lpbBattleCur = NULL;
    gd.fAisDone = 0;
    gd.fGotoVCR = 0;
    gd.fFleetLinkValid = 0;
    ResetHb(htBattle);
    if (rglphb[11] != 0) {
        rglphb[11][1].cbBlock = 0xffff;
    }
    ResetHb(htMisc);
    ResetHb(htString);
    ResetHb(htShips);
    ResetHb(htOrd);
    ResetHb(htPlrMsg);
    for (i = 0; i < 16; i++) {
        rglpshdef[i] = NULL;
        rglpshdefSB[i] = NULL;
        rglpbtlplan[i] = NULL;
        rgcbtlplan[i] = 0;
    }
    if (sel.grobj != grobjNone) {
        ini.grobjSel = sel.grobj;
        ini.iObjSel = sel.id;
        ini.idPlayer = idPlayer;
        ini.lid = game.lid;
    }
    idPlayer = -1;
    imemLogCur = 0;
    imemLogPrev = -1;
    iMsgCur = 0;
    vlpbAiData = NULL;
    ResetMessages();
    lSaltCur = 0;
    ctickLast = 0;
    game.lid = 0;
    game.cPlayer = 0;
    game.cPlanMax = 0;
    game.fDirty = 0;
    game.turn = 0;
    game.szName[0] = 0;
    gd.fGameOverMan = 0;
    gd.fSendMsgMode = 0;
    if (hwndBrowser != 0) {
        DestroyWindow(hwndBrowser);
    }
    if (hwndReportDlg != 0) {
        DestroyWindow(hwndReportDlg);
    }
    if (hwndPopup != 0) {
        DestroyWindow(hwndPopup);
        hwndPopup = 0;
    }
    hwndActive = 0;
    sel.scan.grobjFull = grobjNone;
    sel.scan.grobj = grobjNone;
    sel.scan.iwp = -1;
    sel.scan.ifl = -1;
    sel.scan.idpl = -1;
    fOrdersVis = 0;
    sel.grobjFull = grobjNone;
    sel.grobj = grobjNone;
    sel.id = -1;
    sel.pt.y = 0;
    sel.pt.x = 0;
    sel.pl.id = -1;
    sel.fl.id = -1;
    sel.fl.lpplord = NULL;
    sel.pl.lpplprod = NULL;
    dxPlanetProdLB = 0;
    dxOrderED = 0;
    dxFleetCompLB = 0;
    dxShipLB = 0;
    dxShipDD = 0;
    for (i = 0; i < 3; i++) {
        rgdxOrderDD[i] = 0;
    }
    return;
}

int16_t FBogusLong(uint32_t lSerial) {
    int16_t i;

    lSerial ^= 0xa5a5a5a5;
    for (i = 0; lSerial > bogi[i]; i++) {
    }
    if (lSerial == bogi[i]) {
        return 1;
    }
    return 0;
}

int16_t FValidSerialLong(uint32_t lSerial) {
    uint32_t lNumber;
    int16_t  i;
    uint32_t lSeries;

    if (FBogusLong(lSerial) != 0) {
        return 0;
    }
    lSeries = lSerial;
    for (i = 0; i < 4; i++) {
        lSeries = (uint32_t)(lSeries / 36);
    }
    lNumber = lSeries;
    for (i = 0; i < 4; i++) {
        lNumber = (uint32_t)(lNumber * 36);
    }
    lNumber = lSerial - lNumber;
    if (lNumber < 100 || lNumber > 0x16e360) {
        return 0;
    }
    switch (lSeries) {
    default:
        return 0;
    case 18:
    case 22:
    case 2:
    case 4:
    case 6:
        return 1;
    }
}

void FileError(MessageId ids) {
    idsFileError = ids;
    if (fFileErrSilent == 0 && gd.fGeneratingTurn == 0) {
        AlertSz(PszFormatIds(ids, NULL), MB_ICONHAND);
    }
    return;
}

void GetFileStatus(int16_t dt, int16_t iPlayer) {
    SetSzWorkFromDt(dt, iPlayer);
    gd.fReadOnly = access(szWork, 2) == 0 ? 0 : 1;
    return;
}

int16_t FOpenFile(DtFileType dt, int16_t iPlayer, int16_t md) {
    RTBOF     rtbof;
    StringId  ids;
    int16_t   fCheckMulti;
    int16_t   fRewind;
    int16_t   fSilentSav;
    jmp_buf  *penvMemSav;
    jmp_buf   env;
    MessageId t_merge_4c1e_0001;

    fSilentSav = fFileErrSilent;
    ids = idsCantOpenFile;
    gd.fPartialTurn = 0;
    fCheckMulti = dt & 0x2000;
    fRewind = dt & 0x1000;
    dt &= 0xff;
    SetSzWorkFromDt(dt, iPlayer);
    penvMemSav = penvMem;
    penvMem = &env;
    if (setjmp(env) != 0) {
        fFileErrSilent = fSilentSav;
        FileError(ids);
        StreamClose();
        penvMem = penvMemSav;
        return 0;
    }
    fFileErrSilent = 1;
    StreamOpen(szWork, md);
    fFileErrSilent = fSilentSav;
    ids = idsGameFileAppearsCorruptUnableLoadFile;
    ReadRt();
    if (hdrCur.rt != rtBOF || (RawLoad16(&rgbCur[8]) >> 0xc & 0xf) != 2 || (RawLoad16(&rgbCur[8]) >> 5 & 0x7f) < 49 ||
        (RawLoad16(&rgbCur[8]) >> 5 & 0x7f) >= 84) {
        if (hdrCur.rt == rtBOF) {
            t_merge_4c1e_0001 =
                (RawLoad16(&rgbCur[8]) >> 0xc & 0xf) > 2 || ((RawLoad16(&rgbCur[8]) >> 0xc & 0xf) == 2 && (RawLoad16(&rgbCur[8]) >> 5 & 0x7f) > 84) ? 0x2ca
                                                                                                                                                    : 0x4d3;
            FileError(t_merge_4c1e_0001);
        } else {
            FileError(idmColonistsDroppedDestroyedSpiritedFighting);
        }
    } else {
        rtbof = *(RTBOF *)rgbCur;
        if (rtbof.iPlayer != iPlayer) {
            FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
        } else {
            if (game.lid != 0) {
                if (rtbof.lidGame != game.lid) {
                    FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                    goto LBadFile;
                }
                if (dt != dtHist) {
                    if (fCheckMulti != 0 && rtbof.fMulti != 0) {
                        lseek(hf, -4, 2);
                        ReadRt();
                        if (hdrCur.rt != rtEOF && hdrCur.cb != 2)
                            goto LBadFile;
                        rtbof.turn = RawLoad16(rgbCur);
                        game.wGen = rtbof.wGen;
                    }
                    if (game.turn == 0 && game.turn != rtbof.turn) {
                        game.turn = rtbof.turn;
                        game.wGen = rtbof.wGen;
                    } else {
                        if (rtbof.turn != game.turn) {
                            FileError(idmVigilantFleetsManagedDefeatSavageVerminWithout);
                            goto LBadFile;
                        }
                        if (dt == dtHost && gd.fHostMode == 0 && rtbof.fInUse != 0) {
                            if (AlertSz(PszFormatIds(idsHostFileMarkedUseAnotherInstanceStars, NULL), MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL) != IDYES)
                                goto LBadFile;
                        } else {
                            if (rtbof.fDone == 0 && gd.fGeneratingTurn != 0 && gd.fForceTurn == 0) {
                                gd.fPartialTurn = 1;
                                goto LBadFile;
                            }
                            if (dt == dtLog && game.fTutorial == 0 && rtbof.wGen != game.wGen) {
                                FileError(idmBraveForcesObliteratedVastlyGreaterForcesCowardl);
                                goto LBadFile;
                            }
                        }
                    }
                } else if (rtbof.iPlayer != iPlayer) {
                    goto LBadFile;
                }
            }
            if (fRewind != 0) {
                lseek(hf, 0, 0);
                ReadRt();
            }
            penvMem = penvMemSav;
            wVersFile = rtbof.wVersion;
            gd.fFileCrippled = rtbof.fCrippled;
            return 1;
        }
    }
LBadFile:
    StreamClose();
    penvMem = penvMemSav;
    return 0;
}

int16_t FNewTurnAvail(int16_t idPlayer) {
    uint16_t wGenOld;
    uint16_t turnOld;
    int16_t  fNew;

    turnOld = game.turn;
    wGenOld = game.wGen;
    fFileErrSilent = 1;
    game.turn = 0;
    fNew = FOpenFile(0x2003, idPlayer, 32);
    if (fNew != 0) {
        StreamClose();
        fNew = game.turn <= turnOld ? 0 : 1;
    }
    game.turn = turnOld;
    game.wGen = wGenOld;
    return fNew;
}

int16_t FCheckFile(DtFileType dt, int16_t iPlayer, uint16_t md) {
    int16_t  fReturn;
    int16_t  fOpened;
    uint16_t wGenOld;
    int16_t  f;
    int16_t  fErrSav;

    fErrSav = fFileErrSilent;
    wGenOld = game.wGen;
    if (dt == dtHost) {
        f = gd.fHostMode;
        gd.fHostMode = 1;
    }
    fFileErrSilent = 1;
    fOpened = FOpenFile(dt, iPlayer, 32);
    switch (md) {
    case 1:
        if (fOpened == 0 || (RawLoad16(&rgbCur[14]) >> 9 & 1) != 0) {
            fReturn = 1;
            break;
        }
        fReturn = 0;
        break;
    case 2:
        if (fOpened != 0 && (RawLoad16(&rgbCur[14]) >> 8 & 1) != 0) {
            fReturn = 1;
            break;
        }
        fReturn = 0;
        break;
    case 4:
        if (fOpened != 0 && (RawLoad16(&rgbCur[14]) >> 0xa & 1) != 0) {
            fReturn = 1;
            break;
        }
        fReturn = 0;
        break;
    case 8:
        if (fOpened == 0) {
            fReturn = 0;
        } else {
            do {
                ReadRt();
            } while (hdrCur.rt != rtPlr && (int16_t)(int8_t)rgbCur[0] != iPlayer);
            fReturn = RawLoad16(&rgbCur[6]) >> 9 & 1;
        }
    }
    if (fOpened != 0) {
        StreamClose();
    }
    if (dt == dtHost) {
        gd.fHostMode = f;
    }
    fFileErrSilent = fErrSav;
    game.wGen = wGenOld;
    return fReturn;
}

void ReadRt() {
    RgFromStream(&hdrCur, 2);
    if (hdrCur.cb != 0) {
        RgFromStream(rgbCur, hdrCur.cb);
    }
    if (hdrCur.rt == rtBOF) {
        SetFileXorStream(RawLoad32(&rgbCur[4]), (int16_t)RawLoad16(&rgbCur[12]) >> 5, RawLoad16(&rgbCur[10]), (int16_t)(RawLoad16(&rgbCur[12]) << 0xb) >> 0xb,
                         RawLoad16(&rgbCur[14]) >> 0xc & 1);
    } else if (hdrCur.rt != rtEOF) {
        XorFileBuf(rgbCur, hdrCur.cb);
    }
    return;
}

int16_t FBadFileError(StringId ids) {
    switch (ids) {
    case idsUniverseDefinitionFileSeemsMissingCorrupt:
    case idsPlayerLogFileAppearsCorruptUnableLoad:
    case idsHistoryFileAppearsCorruptHistoricalDataWill:
    case idsGameFileAppearsCorruptUnableLoadFile:
    case idsErrorWritingFile:
    case idsFileDate:
    case idsFileGame:
        return 1;
    default:
        return 0;
    }
}

void StreamOpen(char *szFile, int16_t mdOpen) {
    uint32_t dwTick;
    OFSTRUCT of;
    int16_t  fNoErr;
    uint32_t dwTickCur;

    dwTick = 0;
    fNoErr = (mdOpen & 0x4000) == 0 ? 0 : 1;
    mdOpen &= 0xbfff;
    while (1) {
        hf = OpenFile(szFile, &of, mdOpen);
        if (hf != -1) {
            return;
        }
        if (gd.fRetryOpens == 0 || of.nErrCode == 2)
            break;
        dwTickCur = GetTickCount();
        if (dwTick == 0) {
            dwTick = dwTickCur + 4000;
        }
        if (dwTickCur >= dwTick)
            break;
        dwTickCur += 500;
        while (GetTickCount() < dwTickCur) {
        }
    }
    if (fNoErr == 0) {
        FileError(idmPlanetaryDefensesGroundTroopsDestroyedInvadingTr);
    }
    StarsLongJump(penvMem, -1);
    return;
}

void StreamClose() {
    if (hf != -1) {
        _lclose(hf);
        hf = -1;
    }
    return;
}

void RgFromStream(void *rg, uint16_t cb) {
    if (cb != 0) {
        if (vlpMemStream != 0) {
            fmemcpy(rg, vlpMemStream, cb);
            vlpMemStream += cb;
        } else if (_lread(hf, rg, cb) != cb) {
            FileError(idmGroundTroopsValiantlyDestroyedAttackingBarbarian);
            StarsLongJump(penvMem, -1);
        }
    }
    return;
}
