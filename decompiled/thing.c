#include "common.h"

THING *LpthNew(int16_t iplr, ThingType ith) {
    int16_t iItem;
    int16_t i;
    THING  *lpth;
    THING   thNew;

    if (cThing < 4050) {
        memset(&thNew, 0, sizeof(THING));
        thNew.iplr = iplr;
        thNew.ith = ith;
        lpth = lpThings;
        i = 0;
        for (; i < cThing && thNew.idFull > lpth->idFull; lpth++) {
            i = i + 1;
        }
        if (i < cThing && thNew.idFull == lpth->idFull) {
            iItem = lpth->id;
            while (1) {
                if (i >= cThing)
                    goto L_0146;
                if (iItem >= 511)
                    break;
                if (lpth->idFull != thNew.idFull)
                    goto L_0146;
                i = i + 1;
                thNew.id = thNew.id + 0x1;
                lpth = lpth + 1;
                iItem = iItem + 1;
            }
            return 0x0;
        }
    L_0146:
        if (cThing >= cThingAlloc) {
            cThingAlloc = cThingAlloc + 10;
            if (lpThings != 0x0) {
                lpThings = LpReAlloc(lpThings, cThingAlloc * sizeof(THING), htThings);
            } else {
                lpThings = LpAlloc(cThingAlloc * sizeof(THING), htThings);
            }
            lpth = lpThings + i;
        }
        if (i < cThing) {
            fmemmove(lpth + 1, lpth, (cThing - i) * sizeof(THING));
        }
        cThing = cThing + 1;
        fmemcpy(lpth, &thNew, sizeof(THING));
        return lpth;
    }
    return 0x0;
}

void FreeLpth(THING *lpth) {
    if (lpth < lpThings + (cThing - 1)) {
        fmemmove(lpth, lpth + 1, (cThing - (int32_t)((uint8_t *)lpth - (uint8_t *)lpThings) / 0x12 - 0x1) * sizeof(THING));
    }
    cThing = cThing - 1;
    return;
}

int16_t CPlanetsInCircle(POINT16 pt, int32_t r2) {
    int16_t  xStart;
    POINT16 *ppt;
    int16_t  yEnd;
    int16_t  dy;
    POINT16 *pptEnd;
    int16_t  yStart;
    int16_t  i;
    int16_t  r;
    int16_t  cPl;
    int16_t  dx;
    int16_t  xEnd;

    r = LOWORD((int32_t)(sqrt((double)r2) + 0.9999));
    xStart = pt.x - r;
    xEnd = pt.x + r;
    yStart = pt.y - r;
    yEnd = pt.y + r;
    cPl = 0;
    pptEnd = &rgptPlan[game.cPlanMax];
    dx = rgptPlan[game.cPlanMax - 1].x - rgptPlan[0].x;
    i = LOWORD((int32_t)((int32_t)((int32_t)(pt.x - rgptPlan[0].x) * (int32_t)game.cPlanMax) / (int32_t)dx));
    if (i >= game.cPlanMax) {
        i = game.cPlanMax - 1;
    }
    if (i < 0) {
        i = 0;
    }
    for (ppt = &rgptPlan[i]; ppt->x >= xStart && ppt > rgptPlan; ppt--) {
    }
    for (; ppt->x <= xEnd && ppt < pptEnd; ppt++) {
        if (ppt->x >= xStart && ppt->y >= yStart && ppt->y <= yEnd) {
            dx = ppt->x - pt.x;
            dy = ppt->y - pt.y;
            if ((uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy) <= r2) {
                cPl = cPl + 1;
            }
        }
    }
    return cPl;
}

void DrawThingGauge(HDC hdc, RECT *prc, THING *lpth, int16_t md) {
    int16_t iMode;
    int16_t cSections;
    int16_t fDisabled;
    HBRUSH  rghbr[5];
    int16_t c;
    int16_t i;
    int32_t rgSize[5];
    int32_t lMax;
    int32_t l;

    fDisabled = 0;
    SelectObject(hdc, rghfontArial8[1]);
    cSections = 1;
    lMax = (uint32_t)(lpth->thp.wtMax * 0xa);
    if (md < 0 || md > 4) {
        if (md == 5) {
            for (i = 0; i < 3; i++) {
                rghbr[i] = rghbrMineral[i];
                rgSize[i] = (int32_t)lpth->thp.rgwtMin[i];
            }
            cSections = 3;
        }
    } else if (md != 4 && md != 3) {
        rghbr[0] = rghbrMineral[md];
        rgSize[0] = (int32_t)lpth->thp.rgwtMin[md];
    } else {
        rghbr[0] = hbrButtonShadow;
        rgSize[0] = lMax;
        fDisabled = 1;
    }
    l = LDrawGauge(hdc, prc, cSections, rgSize, rghbr, lMax);
    iMode = SetBkMode(hdc, TRANSPARENT);
    if (fDisabled != 0) {
        l = 0;
    }
    if (cSections != 1) {
        c = _wsprintf(szWork, "%ld of %ldkT", l, lMax);
    } else {
        c = _wsprintf(szWork, "%ldkT", l);
    }
    l = GetTextExtent(hdc, szWork, c);
    if (LOWORD(l) < prc->right - prc->left - 3) {
        RcCtrTextOut(hdc, prc, szWork, c);
    }
    SetBkMode(hdc, iMode);
    return;
}

int16_t IValidateWormholePos(THING *lpthWorm) {
    int16_t iRet;
    POINT16 pt;
    int32_t dy;
    THING  *lpthMac;
    FLEET  *lpfl;
    int16_t ifl;
    int16_t i;
    THING  *lpth;
    int16_t dUni;
    int32_t dx;
    int32_t l;

    iRet = 0;
    dUni = 400 * game.mdSize + 1400;
    pt = lpthWorm->pt;
    if (pt.x >= 1000 && pt.y >= 1000) {
        if (pt.x <= dUni && pt.y <= dUni) {
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (pt.x == lpth->pt.x && pt.y == lpth->pt.y && lpth != lpthWorm) {
                    return 15;
                }
            }
            i = 0;
            while (1) {
                if (i >= game.cPlanMax)
                    goto L_0769;
                if (pt.x == rgptPlan[i].x && pt.y == rgptPlan[i].y)
                    break;
                i = i + 1;
            }
            return 15;
        L_0769:
            ifl = 0;
            while (1) {
                if (ifl >= cFleet)
                    goto L_07dc;
                lpfl = rglpfl[ifl];
                if (rglpfl[ifl] == 0x0)
                    goto L_07dc;
                if (pt.x == lpfl->pt.x && pt.y == lpfl->pt.y)
                    break;
                ifl = ifl + 1;
            }
            return 15;
        L_07dc:
            if (pt.x < 1010 || pt.y < 1010 || pt.x > dUni - 10 || pt.y > dUni - 10) {
                iRet = iRet | 0x4;
            }
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (lpth->ith == ithWormhole && lpth != lpthWorm) {
                    dx = (int32_t)(pt.x - lpth->pt.x);
                    dy = (int32_t)(pt.y - lpth->pt.y);
                    l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                    if (lpth->idFull != lpthWorm->thw.idPartner) {
                        if (l < 900) {
                            if (l < 16) {
                                iRet = iRet | 0x8;
                            } else if (l < 64) {
                                iRet = iRet | 0x4;
                            } else if (l < 225) {
                                iRet = iRet | 0x2;
                            } else {
                                iRet = iRet | 0x1;
                            }
                        }
                    } else if (l < 4900) {
                        if (l < 25) {
                            iRet = iRet | 0x8;
                        } else if (l < 100) {
                            iRet = iRet | 0x4;
                        } else if (l < 900) {
                            iRet = iRet | 0x2;
                        } else {
                            iRet = iRet | 0x1;
                        }
                    }
                }
            }
            for (i = 0; i < game.cPlanMax; i++) {
                dx = (int32_t)(pt.x - rgptPlan[i].x);
                dy = (int32_t)(pt.y - rgptPlan[i].y);
                l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                if (l < 784) {
                    if (l < 25) {
                        iRet = iRet | 0x8;
                    } else if (l < 100) {
                        iRet = iRet | 0x4;
                    } else if (l < 400) {
                        iRet = iRet | 0x2;
                    } else {
                        iRet = iRet | 0x1;
                    }
                }
            }
            return iRet;
        }
        return 15;
    }
    return 15;
}

int16_t PctWormholeMoves(THING *lpth) {
    int16_t pct;

    pct = (int32_t)lpth->thw.cLastMove / 5;
    pct = pct - (2 - lpth->thw.iStable);
    if (pct >= 0) {
        if (pct > 6) {
            pct = 6;
        }
    } else {
        pct = 0;
    }
    return pct;
}

void DoThingInteractions(int16_t fPostMove) {
    int32_t   wtThreshhold;
    uint16_t  grbitPlrTrader;
    int16_t   iplr;
    int32_t   wtMin;
    POINT16   pt;
    int16_t   iplrSav;
    uint8_t   rgTech[6];
    int32_t   wtNext;
    int32_t   dy;
    THING    *lpthMac;
    PLANET   *lpplMac;
    PLANET   *lppl;
    int16_t   i;
    int16_t   ifl;
    FLEET    *lpfl;
    THING    *lpth;
    MessageId idm;
    int16_t   cPlrTrueMaxTech;
    int32_t   dx;
    int16_t   fMaxTech;
    int32_t   l;
    int32_t   cTech;
    int16_t   iLowest;
    int16_t   cTechCur;
    int32_t   lSpent;
    int16_t   iGoto;
    uint16_t  grbitTrader;
    int16_t   cTry;
    int16_t   iOffset;
    int16_t   ish;
    SHDEF     shdef;
    SHDEF    *lpshdefDest;
    FLEET    *lpflNew;
    int16_t   cGive;
    int16_t   iLvl;
    int16_t   iPass;
    int32_t   t_0fac;
    int16_t   t_11bc;
    SHDEF    *t_call_1268;
    int16_t   t_1851;
    int32_t   t_merge_1922_0001;

    if (fPostMove != 0) {
        lpth = lpThings;
        lpthMac = lpThings + cThing;
        for (; lpth < lpthMac; lpth++) {
            if (lpth->ith == ithMysteryTrader) {
                pt = lpth->pt;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0x0)
                        break;
                    if (lpfl->fDead == 0x0 && lpfl->pt.x == pt.x && lpfl->pt.y == pt.y) {
                        wtMin = 0;
                        for (i = 0; i <= 2; i++) {
                            wtMin = wtMin + lpfl->rgwtMin[i];
                        }
                        if (wtMin < 5000) {
                            if (lpfl->fHereAllTurn == 0x0) {
                                FSendPlrMsg2(lpfl->iplr, 264, lpfl->id | 0x8000, lpfl->id, 0);
                            }
                        } else {
                            cPlrTrueMaxTech = rgplr[lpfl->iPlayer].fCrippled == 0x0 ? 26 : 10;
                            for (i = 0; i < 6 && (int16_t)rgplr[lpfl->iPlayer].rgTech[i] >= cPlrTrueMaxTech; i++) {
                            }
                            fMaxTech = i == 6 ? 1 : 0;
                            grbitPlrTrader = rgplr[lpfl->iPlayer].grbitTrader;
                            iplr = lpfl->iPlayer;
                            if ((0x1 << iplr & lpth->tht.grbitPlr) == 0x0) {
                                lpth->tht.grbitPlr = lpth->tht.grbitPlr | 0x1 << iplr;
                                FRemovePlayerMessage(iplr, 78, lpfl->id | 0x8000);
                                lpfl->fDead = 0x1;
                                if (lpth->tht.grbitTrader == 0x0 || (lpth->tht.grbitTrader & grbitPlrTrader) != 0x0) {
                                    if (fMaxTech == 0) {
                                        cTechCur = 0;
                                        cTech = (int32_t)((wtMin - 5000) / 0x4b0) + 6;
                                        for (i = 0; i < 6; i++) {
                                            cTechCur = cTechCur + (int16_t)rgplr[lpfl->iPlayer].rgTech[i];
                                        }
                                        if (cTech > 10) {
                                            cTech = 10;
                                        }
                                        if (cTechCur < 108) {
                                            if (cTechCur < 96) {
                                                if (cTechCur < 84) {
                                                    if (cTechCur < 72) {
                                                        if (cTechCur >= 60) {
                                                            cTech = cTech - 1;
                                                        }
                                                    } else {
                                                        cTech = cTech - 2;
                                                    }
                                                } else {
                                                    cTech = cTech - 3;
                                                }
                                            } else {
                                                cTech = 2;
                                            }
                                        } else {
                                            cTech = 1;
                                        }
                                        if ((grbitPlrTrader & 0x1fff) == 0x1fff) {
                                            idm = idmHasAbsorbedMysteryTraderReturnTraderHas;
                                        } else {
                                            idm = idmHasAbsorbedMysteryTraderTraderHasGiven;
                                        }
                                        FSendPlrMsg(lpfl->iplr, idm, -1, WFromLpfl(lpfl), LOWORD(cTech), 0, 0, 0, 0, 0);
                                        while (1) {
                                            t_0fac = cTech;
                                            cTech = cTech - 1;
                                            if (t_0fac == 0)
                                                break;
                                            if (Random(4) < 3) {
                                                iLowest = Random(6);
                                                if ((int16_t)rgplr[iplr].rgTech[iLowest] < cPlrTrueMaxTech)
                                                    goto LGiveITech;
                                            }
                                            iLowest = 0;
                                            for (i = 1; i < 6; i++) {
                                                if ((int16_t)rgplr[iplr].rgTech[i] < (int16_t)rgplr[iplr].rgTech[iLowest]) {
                                                    iLowest = i;
                                                }
                                            }
                                            if ((int16_t)rgplr[iplr].rgTech[iLowest] >= cPlrTrueMaxTech)
                                                break;
                                        LGiveITech:
                                            memcpy(rgTech, rgplr[iplr].rgTech, 0x6);
                                            rgTech[iLowest] = rgTech[iLowest] + 0x1;
                                            iplrSav = idPlayer;
                                            idPlayer = iplr;
                                            wtNext = CostOfDevelopingItem(rgTech);
                                            idPlayer = iplrSav;
                                            lSpent = rgplr[iplr].rgResSpent[iLowest];
                                            lSpent = (int32_t)(lSpent * 2);
                                            if (wtNext > 0) {
                                                lSpent = lSpent + wtNext;
                                            }
                                            rgplr[iplr].rgResSpent[iLowest] = lSpent;
                                            UpdateResearchStatus(0);
                                        }
                                        continue;
                                    }
                                    if (Random(5) == 0) {
                                        idm = idmHasAbsorbedMysteryTraderHoweverTraderUnable2;
                                        FSendPlrMsg2(lpfl->iplr, idm, -1, WFromLpfl(lpfl), 0);
                                        continue;
                                    }
                                }
                                cTry = 25;
                                grbitTrader = lpth->tht.grbitTrader;
                                if (grbitTrader == 0x0) {
                                    grbitTrader = 0x1 << Random(13);
                                }
                                for (; (grbitTrader & grbitPlrTrader) != 0x0; grbitTrader = 0x1 << Random(13)) {
                                    t_11bc = cTry;
                                    cTry = cTry - 1;
                                    if (t_11bc <= 0)
                                        break;
                                }
                                if (cTry <= 0) {
                                    grbitTrader = 0x1000;
                                }
                                lpth->tht.grbitPlr = lpth->tht.grbitPlr | 0x1 << iplr;
                                if (grbitTrader != 0x1000) {
                                    idm = IdmGiveTraderPart(grbitTrader, iplr, &iGoto);
                                    FSendPlrMsg2(lpfl->iplr, idm, iGoto, WFromLpfl(lpfl), 0);
                                } else if (rgplr[iplr].fAi == 0x0) {
                                    iOffset = Random(4 - (game.turn <= 0x64 ? 0 : 1));
                                    if (iOffset >= 1) {
                                        iOffset = Random(2) + 1;
                                    }
                                    t_call_1268 = LpshdefT();
                                    shdef = t_call_1268[iOffset + 19];
                                    ish = IshFindSimilarDesign(&shdef.hul, iplr);
                                    if (ish < 0) {
                                        do {
                                            ish = ish + 1;
                                        } while (ish < 16 && rglpshdef[iplr][ish].fFree == 0x0);
                                    }
                                    if (ish < 16 && rgplr[lpfl->iplr].cFleet < 0x200) {
                                        if (Random(3) != 0) {
                                            cGive = 1;
                                        } else {
                                            cGive = 2;
                                        }
                                        if (game.turn > 0x64 && game.fSinglePlr == 0x0) {
                                            cGive = cGive + Random((uint32_t)game.turn / 100 + 1);
                                        }
                                        if (cGive > 5) {
                                            cGive = 5;
                                        }
                                        if (iOffset > 0) {
                                            cGive = cGive + Random(cGive + 1);
                                        }
                                        lpflNew = LpflNew(iplr, lpfl->idPlanet);
                                        if (lpflNew != 0x0) {
                                            FSendPlrMsg2(lpfl->iplr, 335, lpflNew->id | 0x8000, WFromLpfl(lpfl), cGive);
                                            if (lpflNew->id < lpfl->id) {
                                                ifl = ifl + 1;
                                            }
                                            lpflNew->pt = lpfl->pt;
                                            lpflNew->lpplord->rgord[0].pt = lpfl->pt;
                                            lpflNew->fHereAllTurn = 0x1;
                                            lpshdefDest = rglpshdef[iplr] + ish;
                                            if (lpshdefDest->fFree != 0x0) {
                                                *lpshdefDest = shdef;
                                                lpshdefDest->ishdef = ish;
                                                lpshdefDest->fGift = 0x1;
                                                lpshdefDest->cBuilt = 0x0;
                                                lpshdefDest->cExist = 0x0;
                                                rgplr[iplr].cShDef = rgplr[iplr].cShDef + 1;
                                                idPlayer = lpfl->iplr;
                                                UpdateShdefCost(lpshdefDest);
                                                idPlayer = -1;
                                            }
                                            lpshdefDest->cBuilt = lpshdefDest->cBuilt + (int32_t)cGive;
                                            lpshdefDest->cExist = lpshdefDest->cExist + (int32_t)cGive;
                                            lpflNew->rgcsh[ish] = cGive;
                                            lpflNew->rgwtMin[4] = LGetFleetStat(lpflNew, 1);
                                            continue;
                                        }
                                    }
                                    FSendPlrMsg2(lpfl->iplr, 336, -1, WFromLpfl(lpfl), 0);
                                }
                            } else {
                                FSendPlrMsg2(lpfl->iplr, 280, lpfl->id | 0x8000, lpfl->id, 0);
                            }
                        }
                    }
                }
                lppl = lpPlanets;
                lpplMac = lpPlanets + cPlanet;
                for (; lppl < lpplMac; lppl++) {
                    if (lppl->iPlayer != -1 && lppl->fStarbase != 0x0 && rgplr[lppl->iPlayer].fAi != 0x0 && rgplr[lppl->iPlayer].lvlAi >= 0x2 &&
                        (0x1 << lppl->iPlayer & lpth->tht.grbitPlr) == 0x0) {
                        dx = (int32_t)(rgptPlan[lppl->id].x - pt.x);
                        if (dx > 100)
                            break;
                        dy = (int32_t)(rgptPlan[lppl->id].y - pt.y);
                        l = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                        if (l <= 10000) {
                            wtNext = 0;
                            for (i = 0; i < 3; i++) {
                                wtNext = wtNext + lppl->rgwtMin[i];
                            }
                            iplr = lppl->iPlayer;
                            wtThreshhold = (uint32_t)(rgplr[iplr].lvlAi == 0x2 ? 0xdac : 0x1388);
                            if (wtNext >= wtThreshhold) {
                                if (lpth->tht.grbitTrader != 0x0) {
                                    cTry = 50;
                                    for (grbitTrader = lpth->tht.grbitTrader; (grbitTrader & rgplr[iplr].grbitTrader) != 0x0; grbitTrader = 0x1 << Random(13)) {
                                        t_1851 = cTry;
                                        cTry = cTry - 1;
                                        if (t_1851 <= 0)
                                            break;
                                    }
                                    if (cTry > 0) {
                                        rgplr[iplr].grbitTrader = rgplr[iplr].grbitTrader | grbitTrader;
                                        goto LChgMin;
                                    }
                                }
                                iLvl = 0;
                                for (i = 0; i < 6; i++) {
                                    iLvl = iLvl + (int16_t)rgplr[iplr].rgTech[i];
                                }
                                cPlrTrueMaxTech = rgplr[lpfl->iPlayer].fCrippled == 0x0 ? 26 : 10;
                                if (iLvl >= 6 * cPlrTrueMaxTech - 0x6)
                                    continue;
                                for (iPass = 0; iPass < 6; iPass++) {
                                    iLvl = 0;
                                    for (i = 1; i < 6; i++) {
                                        if ((int16_t)rgplr[iplr].rgTech[i] < (int16_t)rgplr[iplr].rgTech[iLvl]) {
                                            iLvl = i;
                                        }
                                    }
                                    rgplr[iplr].rgTech[iLvl] = rgplr[iplr].rgTech[iLvl] + 1;
                                }
                                wtNext = wtThreshhold;
                            LChgMin:
                                lpth->tht.grbitPlr = lpth->tht.grbitPlr | 0x1 << iplr;
                                for (i = 2; wtNext > 0 && i >= 0; i--) {
                                    t_merge_1922_0001 = wtNext < lppl->rgwtMin[i] ? wtNext : lppl->rgwtMin[i];
                                    wtMin = t_merge_1922_0001;
                                    lppl->rgwtMin[i] = lppl->rgwtMin[i] - wtMin;
                                    wtNext = wtNext - wtMin;
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

int16_t IdmGiveTraderPart(GrbitTrader grbitTrader, int16_t iplr, uint16_t *piGoto) {
    uint16_t  iGoto;
    MessageId idm;

    rgplr[iplr].grbitTrader = rgplr[iplr].grbitTrader | grbitTrader;
    idm = idmHasAbsorbedMysteryTraderHaveGivenPlans;
    switch (grbitTrader) {
    case grbitTraderCargo:
    default:
        iGoto = 0xcc04;
        break;
    case grbitTraderSpecial:
        iGoto = 0xcb04;
        break;
    case grbitTraderShield:
        iGoto = 0xc206;
        break;
    case grbitTraderArmor:
        iGoto = 0xc309;
        break;
    case grbitTraderMiner:
        iGoto = 0xc706;
        break;
    case grbitTraderBomb:
        iGoto = 0xc608;
        break;
    case grbitTraderBeam:
        iGoto = 0xc412;
        break;
    case grbitTraderTorp:
        iGoto = 0xc507;
        break;
    case grbitTraderHull:
        idm = idmHasAbsorbedMysteryTraderReturnHaveGiven;
        iGoto = 0xce1e;
        break;
    case grbitTraderEngine:
        iGoto = 0xc008;
        break;
    case grbitTraderGenesis:
        idm = idmHasAbsorbedMysteryTraderReturnHaveGiven2;
        iGoto = 0xcf0e;
        break;
    case grbitTraderJumpgate:
        iGoto = 0xcc09;
    }
    *piGoto = iGoto;
    return idm;
}
