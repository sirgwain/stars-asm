#include "common.h"

void Produce() {
    int32_t   lResCur;
    int16_t   cMax;
    int32_t   rgResAvail[4];
    int16_t   iprodCur;
    int16_t   mdStatus;
    int16_t   cBuilt;
    int16_t   fNoResearch;
    PLANET   *lppl;
    int16_t   i;
    MessageId idm;
    PROD      prodPartial;
    int16_t   fPrevProdIsAlch;
    int16_t   fAutoBuildDone;
    int32_t   lResearchTake;
    PROD     *lpprod;
    PLANET   *lpplMac;
    int16_t   cMax2;
    uint16_t  t_scratch_m3e;

    MineMinerals();
    for (i = 0; i < game.cPlayer; i++) {
        rgplr[i].lResLastYear = 0;
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->lpplprod != 0x0) {
            if (lppl->iPlayer != -1 && lppl->lpplprod->iprodMac != 0x0) {
                fNoResearch = lppl->fNoResearch;
                for (i = 0; i < 3; i++) {
                    rgResAvail[i] = lppl->rgwtMin[i];
                }
                lResCur = (int32_t)CResourcesAtPlanet(lppl, lppl->iPlayer);
                if (lResCur != 0 && vrgPlanResExtra[lppl->id] != 0x0) {
                    lResCur = lResCur +
                              (int32_t)((int32_t)(lResCur * (uint32_t)vrgPlanResExtra[lppl->id]) / (int32_t)((uint32_t)vrgPlanResExtra[lppl->id] + lResCur));
                }
                rgResAvail[3] = lResCur;
                if (rgplr[lppl->iPlayer].fCheater != 0x0) {
                    rgResAvail[3] = (int32_t)((int32_t)(rgResAvail[3] * 4) / 5);
                }
                if (rgResAvail[3] != 0) {
                    if (fNoResearch == 0) {
                        lResearchTake = (int32_t)((int32_t)(rgResAvail[3] * (int32_t)(int16_t)rgplr[lppl->iPlayer].pctResearch) / 0x64);
                        rgResAvail[3] = rgResAvail[3] - lResearchTake;
                        rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + lResearchTake;
                    } else {
                        lResearchTake = 0;
                    }
                    fAutoBuildDone = 1;
                    while (1) {
                        fPrevProdIsAlch = 0;
                        iprodCur = 0;
                        while (1) {
                            if (lppl->lpplprod == 0x0 || iprodCur >= lppl->lpplprod->iprodMac)
                                goto L_0b9a;
                            lpprod = &lppl->lpplprod->rgprod[iprodCur];
                            if (lpprod->cItem > 0x0) {
                                if (lpprod->grobj == grobjPlanet) {
                                    if ((lpprod->iItem < iobjPlanetaryScannerFirst || lpprod->iItem > iobjPlanetaryScannerSnooper620X) &&
                                        lpprod->iItem != iobjPlanetaryScanner) {
                                        if (lpprod->iItem >= iobjPacketIron && lpprod->iItem <= iobjPacketMixed) {
                                            if (IWarpMAFromLppl(lppl, 0x0) == 0 || lppl->idFling == 0x0) {
                                                FSendPlrMsg2(lppl->iPlayer, 297, lppl->id, lppl->id, 0);
                                                goto RemoveFromQueue;
                                            }
                                        } else {
                                            switch (lpprod->iItem) {
                                            case mdIdleFactory:
                                                cMax = CMaxFactories(lppl, lppl->iPlayer);
                                                cMax2 = CMaxOperableFactories(lppl, lppl->iPlayer, 1);
                                                if (cMax2 > cMax) {
                                                    cMax = cMax2;
                                                }
                                                cMax = cMax - lppl->cFactories;
                                                goto LCantBuildP;
                                            case mdIdleMine:
                                                cMax = CMaxMines(lppl, lppl->iPlayer);
                                                cMax2 = CMaxOperableMines(lppl, lppl->iPlayer, 1);
                                                if (cMax2 > cMax) {
                                                    cMax = cMax2;
                                                }
                                                cMax = cMax - lppl->cMines;
                                                goto LCantBuildP;
                                            case mdIdleDefense:
                                                cMax = CMaxDefenses(lppl, lppl->iPlayer);
                                                cMax2 = CMaxOperableDefenses(lppl, lppl->iPlayer, 1);
                                                if (cMax2 > cMax) {
                                                    cMax = cMax2;
                                                }
                                                cMax = cMax - lppl->cDefenses;
                                                goto LCantBuildP;
                                            case mdIdleTerraform:
                                                cMax = IpctCanTerraformLppl(lppl);
                                                idm = idmHasOrdersTerraformBeyondMaximumAllowedOrders;
                                                goto LCantBuildP2;
                                            default:
                                                goto L_07fc;
                                            }
                                            goto L_0b9a;
                                        LCantBuildP:
                                            idm = idmHasOrdersBuildPlanetaryInstallationsBeyondMaximu;
                                        LCantBuildP2:
                                            if (cMax < lpprod->cItem) {
                                                FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
                                                if (cMax <= 0)
                                                    goto RemoveFromQueue;
                                                lpprod->cItem = cMax;
                                            }
                                        }
                                    } else if (lppl->iScanner != 0x1f) {
                                        FSendPlrMsg2(lppl->iPlayer, 185, lppl->id, lppl->id, 0);
                                        goto RemoveFromQueue;
                                    }
                                }
                            L_07fc:
                                if (lpprod->iItem == iobjAlchemy && lpprod->grobj == grobjPlanet && iprodCur < lppl->lpplprod->iprodMac - 0x1) {
                                    fPrevProdIsAlch = 1;
                                    iprodCur = iprodCur + 1;
                                    continue;
                                }
                                prodPartial.cItem = 0x0;
                                cBuilt = CBuildProdItem(lppl, lpprod, &prodPartial, rgResAvail, fPrevProdIsAlch, &mdStatus, 0);
                                if (fAutoBuildDone != 0 && (mdStatus == 3 || mdStatus == 4)) {
                                    fAutoBuildDone = 0;
                                }
                                if (cBuilt > 0 && FBuildObject(lppl, lpprod->grobj, lpprod->iItem, cBuilt, rgResAvail) == 0 &&
                                    (lpprod->grobj != grobjPlanet || lpprod->iItem >= mdIdleFactory)) {
                                    lpprod->cItem = 0x0;
                                }
                                if (lppl->iPlayer == -1 && lppl->lpplprod == 0x0)
                                    break;
                                if (mdStatus != 0) {
                                    if (mdStatus < 5)
                                        goto L_0b8e;
                                    goto L_0aaf;
                                }
                            }
                        RemoveFromQueue:
                            if (lppl->lpplprod->iprodMac == fPrevProdIsAlch + 1)
                                goto L_09eb;
                            if (iprodCur < lppl->lpplprod->iprodMac - 0x1) {
                                fmemmove(lppl->lpplprod + (1 + (iprodCur - fPrevProdIsAlch)), lppl->lpplprod + (1 + (iprodCur + 1)),
                                         (lppl->lpplprod->iprodMac - iprodCur - 0x1) * 0x4);
                            }
                            lppl->lpplprod->iprodMac = lppl->lpplprod->iprodMac - LOBYTE(fPrevProdIsAlch + 1);
                            iprodCur = iprodCur - (fPrevProdIsAlch + 1);
                        L_0b8e:
                            iprodCur = iprodCur + 1;
                            fPrevProdIsAlch = 0;
                        }
                    }
                L_0aaf:
                    if (prodPartial.cItem <= 0x0)
                        goto L_0b9a;
                    t_scratch_m3e = lppl->lpplprod->iprodMac;
                    if (t_scratch_m3e == lppl->lpplprod->iprodMax) {
                        lppl->lpplprod = (PLPROD *)LpplReAlloc((PL *)lppl->lpplprod, lppl->lpplprod->iprodMac + 0x1);
                    }
                    fmemmove(&lppl->lpplprod->rgprod[1], lppl->lpplprod->rgprod, lppl->lpplprod->iprodMac * sizeof(PROD));
                    lppl->lpplprod->rgprod[0] = prodPartial;
                    lppl->lpplprod->iprodMac = lppl->lpplprod->iprodMac + 0x1;
                    goto L_0b9a;
                L_09eb:
                    FreePl((PL *)lppl->lpplprod);
                    lppl->lpplprod = 0x0;
                L_0b9a:
                    if (lppl->lpplprod == 0x0 || (iprodCur >= lppl->lpplprod->iprodMac && fAutoBuildDone != 0)) {
                        FSendPlrMsg2(lppl->iPlayer, 62, lppl->id, lppl->id, 0);
                    }
                    for (i = 0; i < 3; i++) {
                        lppl->rgwtMin[i] = rgResAvail[i];
                    }
                    rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + rgResAvail[3];
                }
            }
        } else if (lppl->iPlayer != -1) {
            FSendPlrMsg2(lppl->iPlayer, 63, lppl->id, lppl->id, 0);
            lResCur = (int32_t)CResourcesAtPlanet(lppl, lppl->iPlayer);
            if (lResCur != 0 && vrgPlanResExtra[lppl->id] != 0x0) {
                lResCur =
                    lResCur + (int32_t)((int32_t)(lResCur * (uint32_t)vrgPlanResExtra[lppl->id]) / (int32_t)((uint32_t)vrgPlanResExtra[lppl->id] + lResCur));
            }
            rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + lResCur;
        }
    }
    UpdatePopulations();
    UpdateResearchStatus(1);
    if (game.fNoRandom == 0x0) {
        RandomEvents();
    }
    return;
}

int16_t CBuildProdItem(PLANET *lppl, PROD *lpprod, PROD *pprodPartial, int32_t *rgRes, int16_t fAlchemy, int16_t *pmdStatus, int16_t fCalcOnly) {
    int32_t  pctT;
    int16_t  cMax;
    uint32_t iobjOther;
    int32_t  cCanBuild;
    int32_t  lMinNeeded;
    int32_t  lAlchCost;
    PROD     prod;
    int16_t  fAutoBuild;
    int16_t  cBuilt;
    int16_t  cAlchemy;
    int32_t  rgCostPaid[4];
    int16_t  i;
    int16_t  fResourceBlocked;
    int32_t  pctInitial;
    int32_t  pctTooBig;
    int32_t  pct;
    int32_t  rgCost[4];
    int16_t  fMineralBlocked;
    int32_t  AddCost;
    int16_t  t_merge_0d4c_0001;
    uint16_t t_scratch_m56;
    uint16_t t_scratch_m56_2;
    uint16_t t_scratch_m56_3;
    int32_t  t_merge_126a_0001;
    int32_t  t_merge_15f8_0001;
    int16_t  t_merge_189c_0001;

    cAlchemy = 0;
    pctInitial = lpprod->pct;
    prod = *lpprod;
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 1);
    cBuilt = 0;
    if (prod.grobj == grobjPlanet && prod.iItem < mdIdleFactory) {
        t_merge_0d4c_0001 = 1;
    } else {
        t_merge_0d4c_0001 = 0;
    }
    fAutoBuild = t_merge_0d4c_0001;
    if (fAutoBuild != 0) {
        cMax = 1000;
        switch (prod.iItem) {
        case iobjMine:
            iobjOther = mdIdleMine;
            t_scratch_m56 = lppl->cMines;
            cMax = CMaxOperableMines(lppl, lppl->iPlayer, 1) - t_scratch_m56;
            break;
        case iobjFactory:
            iobjOther = mdIdleFactory;
            t_scratch_m56_2 = lppl->cFactories;
            cMax = CMaxOperableFactories(lppl, lppl->iPlayer, 1) - t_scratch_m56_2;
            break;
        case iobjDefense:
            iobjOther = mdIdleDefense;
            t_scratch_m56_3 = lppl->cDefenses;
            cMax = CMaxOperableDefenses(lppl, lppl->iPlayer, 1) - t_scratch_m56_3;
            break;
        case iobjAlchemy:
            iobjOther = mdIdleAlchemy;
            break;
        case iobjMinTerraform:
        case iobjMaxTerraform:
            iobjOther = mdIdleTerraform;
            cMax = IpctCanTerraformLppl(lppl);
            if (cMax <= 0 || prod.iItem != iobjMinTerraform || ChgPopFromPlanet(lppl, 0) < 0 || PctPlanetDesirability(lppl, lppl->iPlayer) <= 0)
                break;
            cMax = 0;
            break;
        case iobjPacket:
            iobjOther = iobjPacketMixed;
            if (IWarpMAFromLppl(lppl, 0x0) == 0 || lppl->idFling == 0x0) {
                cMax = 0;
            }
        default:
        }
        if (cMax < 0) {
            cMax = 0;
        }
        if ((uint32_t)prod.cItem > (uint32_t)(int32_t)cMax || prod.iItem == iobjAlchemy) {
            prod.cItem = cMax;
        }
    }
    for (i = 0; i < 4; i++) {
        rgCostPaid[i] = (uint32_t)((uint32_t)(rgCost[i] * prod.pct) / 0x64);
    }
    while (1) {
        if (prod.cItem <= 0x0)
            goto L_1712;
        for (i = 0; i < 4 && rgCost[i] - rgCostPaid[i] <= rgRes[i]; i++) {
        }
        if (i >= 4) {
            cBuilt = cBuilt + 1;
            prod.cItem = prod.cItem - 0x1;
            prod.pct = 0x0;
            for (i = 0; i < 4; i++) {
                rgRes[i] = rgRes[i] - (rgCost[i] - rgCostPaid[i]);
                rgCostPaid[i] = 0;
            }
        } else {
            fMineralBlocked = 0;
            fResourceBlocked = 0;
            pct = 100;
            for (i = 0; i < 4; i++) {
                if (rgCost[i] > 0) {
                    if (rgRes[i] < rgCost[i]) {
                        pctT = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i]) * 0x64) / rgCost[i]);
                        pctTooBig = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i] + 0x1) * 0x64) / rgCost[i]);
                        t_merge_126a_0001 = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
                        pctT = t_merge_126a_0001;
                    } else {
                        pctT = 100;
                    }
                    if (pctT < pct) {
                        lMinNeeded = rgCost[i] - rgCostPaid[i] - rgRes[i];
                        pct = pctT;
                        if (i != 3) {
                            fMineralBlocked = 1;
                        } else {
                            fResourceBlocked = 1;
                        }
                    }
                }
            }
            if (fMineralBlocked == 0 || fAutoBuild == 0) {
                for (i = 0; i < 4; i++) {
                    AddCost = (int32_t)((int32_t)(rgCost[i] * pct) / 100) - rgCostPaid[i];
                    rgRes[i] = rgRes[i] - AddCost;
                    rgCostPaid[i] = rgCostPaid[i] + AddCost;
                }
                prod.pct = LOWORD(pct);
                if (fAlchemy == 0 || fResourceBlocked != 0)
                    goto L_1712;
            } else if (fAlchemy == 0) {
                break;
            }
            lAlchCost = (uint32_t)(GetRaceGrbit(&rgplr[lppl->iPlayer], ibitRaceMineralAlchemy) == 0 ? 0x64 : 0x19);
            cCanBuild = (int32_t)(rgRes[3] / lAlchCost);
            if (cCanBuild > lMinNeeded) {
                cCanBuild = lMinNeeded;
            }
            if (cCanBuild > 0) {
                for (i = 0; i < 3; i++) {
                    rgRes[i] = rgRes[i] + cCanBuild;
                }
                rgRes[i] = rgRes[i] - (uint32_t)(lAlchCost * cCanBuild);
                cAlchemy = cAlchemy + LOWORD(cCanBuild);
            }
            if (cCanBuild != lMinNeeded)
                goto L_14de;
        }
    }
    fAutoBuild = 2;
    goto L_1712;
L_14de:
    if (rgRes[3] > 0 && pprodPartial != 0x0) {
        memset(pprodPartial, 0, sizeof(PROD));
        pprodPartial->grobj = grobjPlanet;
        pprodPartial->iItem = mdIdleAlchemy;
        pprodPartial->cItem = 0x1;
        pctT = (int32_t)((int32_t)(rgRes[3] * 100) / lAlchCost);
        pctTooBig = (int32_t)((int32_t)((rgRes[3] + 1) * 0x64) / lAlchCost);
        t_merge_15f8_0001 = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
        pctT = t_merge_15f8_0001;
        pprodPartial->pct = LOWORD(pctT);
        rgRes[3] = rgRes[3] - (int32_t)((int32_t)(pctT * lAlchCost) / 100);
    }
L_1712:
    if (cBuilt > 0 && prod.grobj == grobjPlanet && (prod.iItem == mdIdleAlchemy || prod.iItem == iobjAlchemy)) {
        cAlchemy = cAlchemy + cBuilt;
        for (i = 0; i < 3; i++) {
            rgRes[i] = rgRes[i] + (int32_t)cBuilt;
        }
    }
    if (cAlchemy != 0 && fCalcOnly == 0 && gd.fGeneratingTurn != 0x0) {
        FSendPlrMsg2(lppl->iPlayer, 140, lppl->id, lppl->id, cAlchemy);
    }
    if (pmdStatus != 0x0) {
        if (fAutoBuild != 2) {
            if (fAutoBuild == 0 || prod.cItem != 0x0) {
                if (cBuilt != 0) {
                    if (prod.cItem != 0x0) {
                        *pmdStatus = 5;
                    } else {
                        *pmdStatus = 0;
                    }
                } else {
                    t_merge_189c_0001 = pctInitial == prod.pct ? 7 : 6;
                    *pmdStatus = t_merge_189c_0001;
                }
            } else {
                *pmdStatus = cBuilt <= 0 ? 2 : 1;
            }
        } else {
            *pmdStatus = cBuilt <= 0 ? 4 : 3;
        }
    }
    if (fCalcOnly == 0 && fAutoBuild == 0) {
        *lpprod = prod;
    }
    if (fAutoBuild != 0 && pprodPartial != 0x0 && pprodPartial->cItem == 0x0 && prod.pct > 0x0) {
        *pprodPartial = prod;
        pprodPartial->cItem = 0x1;
        pprodPartial->iItem = LOWORD(iobjOther);
    }
    return cBuilt;
}

int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals) {
    int16_t   iWarp;
    int16_t   i;
    FLEET    *lpfl;
    MessageId idm;
    int16_t   fTwoMAs;
    SHDEF    *lpshdef;
    int16_t   cAllowed;
    int32_t   dpOrig;
    int16_t   cshDamaged;
    int16_t   cshOrig;
    uint16_t  dpShdef;
    THING    *lpthMac;
    int16_t   iDecayRate;
    THING    *lpth;
    int16_t   raMajor;
    int16_t   iWarpAsked;
    int16_t   cSize;
    int16_t   rgwt[3];
    int32_t   l;
    int16_t   iEnv;
    PART      part;
    uint16_t  t_scratch_m16_3;
    uint16_t  t_scratch_m16_4;
    uint16_t  t_scratch_m16_5;
    int16_t   t_scratch_m16_6;
    int16_t   t_call_2d26;
    int16_t   t_scratch_m16_7;
    int16_t   t_2da8;
    int16_t   t_merge_2e53_0001;

    if (grobj != grobjFleet) {
        if (grobj != grobjPlanet) {
            return 0;
        }
        if ((uint16_t)iItem > 27) {
            return 0;
        }
        switch (iItem) {
        case 1:
        case 7:
            t_scratch_m16_3 = lppl->cFactories;
            cAllowed = CMaxFactories(lppl, lppl->iPlayer) - t_scratch_m16_3;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cFactories = lppl->cFactories + cBuilt;
            idm = idmHaveBuiltFactory;
            break;
        case 0:
        case 8:
            t_scratch_m16_4 = lppl->cMines;
            cAllowed = CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cMines = lppl->cMines + cBuilt;
            idm = idmHaveBuiltMine;
            break;
        case 2:
        case 9:
            t_scratch_m16_5 = lppl->cDefenses;
            cAllowed = CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt <= 0) {
                return 0;
            }
            lppl->cDefenses = lppl->cDefenses + cBuilt;
            idm = idmHaveBuiltDefenseOutpost;
            break;
        case 6:
        case 14:
        case 15:
        case 16:
        case 17:
            raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (iWarp != 0) {
                if (lppl->idFling != 0x0) {
                    if (iItem == 6) {
                        iItem = 17;
                    }
                    if (iItem != 17) {
                        cSize = raMajor == 6 ? 70 : 100;
                    } else {
                        cSize = raMajor == 6 ? 25 : 40;
                    }
                    for (i = 0; i < 3; i++) {
                        if (i != iItem - 14 && iItem != 17) {
                            rgwt[i] = 0;
                        } else {
                            l = (uint32_t)((int32_t)cSize * (int32_t)cBuilt);
                            if (l > 32760) {
                                l = 32760;
                            }
                            rgwt[i] = LOWORD(l);
                        }
                    }
                    iWarpAsked = lppl->iWarpFling + 4;
                    if (iWarpAsked < 5 || iWarpAsked > iWarp + 3) {
                        iWarpAsked = iWarp + fTwoMAs;
                    }
                    if (iWarpAsked > iWarp + fTwoMAs) {
                        iDecayRate = iWarpAsked - iWarp - fTwoMAs;
                    } else {
                        iDecayRate = 0;
                    }
                    if (raMajor == 7 && iDecayRate < 3) {
                        iDecayRate = iDecayRate + 1;
                    }
                    iWarp = iWarpAsked - 4;
                    lpth = lpThings;
                    lpthMac = lpThings + cThing;
                    for (; lpth < lpthMac && (lpth->iplr != lppl->iPlayer || lpth->ith != ithMineralPacket || lpth->pt.x != rgptPlan[lppl->id].x ||
                                              lpth->pt.y != rgptPlan[lppl->id].y || lpth->thp.iWarp != iWarp || lpth->thp.idPlanet != lppl->idFling - 0x1 ||
                                              lpth->thp.iDecayRate != iDecayRate || lpth->thp.wtMax >= 0x65e);
                         lpth++) {
                    }
                    if (lpth != lpthMac) {
                        lpth->thp.wtMax = 0x0;
                        for (i = 0; i < 3; i++) {
                            lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] + rgwt[i];
                            if (lpth->thp.rgwtMin[i] < 0) {
                                lpth->thp.rgwtMin[i] = 32760;
                            }
                            lpth->thp.wtMax = lpth->thp.wtMax + (int32_t)(lpth->thp.rgwtMin[i] + 9) / 10;
                        }
                        FSendPlrMsg2(lppl->iPlayer, 212, lppl->id, lppl->id, lppl->idFling - 1);
                        return 1;
                    }
                    lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
                    if (lpth != 0x0) {
                        for (i = 0; i < 3; i++) {
                            lpth->thp.rgwtMin[i] = rgwt[i];
                            lpth->thp.wtMax = lpth->thp.wtMax + (int32_t)(rgwt[i] + 9) / 10;
                        }
                        lpth->thp.iWarp = iWarp;
                        lpth->thp.iDecayRate = iDecayRate;
                        lpth->thp.idPlanet = lppl->idFling - 0x1;
                        lpth->pt = rgptPlan[lppl->id];
                        FSendPlrMsg2(lppl->iPlayer, 211, lppl->id, lppl->id, lppl->idFling - 1);
                        return 1;
                    }
                    FSendPlrMsg2(lppl->iPlayer, 297, lppl->id, lppl->id, 0);
                    return 1;
                }
                FSendPlrMsg2(lppl->iPlayer, 210, lppl->id, lppl->id, 0);
                return 0;
            }
            FSendPlrMsg2(lppl->iPlayer, 209, lppl->id, lppl->id, 0);
            return 0;
        case 13:
            for (i = 0; i < game.cPlayer; i++) {
                FSendPlrMsg2(i, 283, lppl->id, lppl->id, 0);
            }
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->cFactories = 0x0;
                lppl->cMines = 0x0;
                lppl->cDefenses = 0x0;
                lppl->iScanner = 0x1f;
            }
            for (i = 0; i < 3; i++) {
                lppl->rgwtMin[i] = 0;
                t_scratch_m16_6 = Random(50);
                t_call_2d26 = Random(50);
                lppl->rgEnvVarOrig[i] = LOBYTE(t_call_2d26 + 1 + t_scratch_m16_6);
                lppl->rgEnvVar[i] = LOBYTE(t_call_2d26 + 1 + t_scratch_m16_6);
                t_scratch_m16_7 = Random(40);
                lppl->rgMinConc[i] = LOBYTE(Random(40) + 25 + t_scratch_m16_7);
            }
            return 1;
        case 4:
        case 5:
        case 12:
            while (1) {
                t_2da8 = cBuilt;
                cBuilt = cBuilt - 1;
                if (t_2da8 == 0)
                    break;
                i = IBestTerraform(lppl, 1);
                if (i != 0) {
                    iEnv = abs(i) - 1;
                    cAllowed = (int16_t)lppl->rgEnvVar[iEnv] + (i <= 0 ? -1 : 1);
                    if (0x1 <= (99 >= cAllowed ? cAllowed : 0x63)) {
                        if (99 >= cAllowed) {
                            t_merge_2e53_0001 = cAllowed;
                        } else {
                            t_merge_2e53_0001 = 99;
                        }
                    } else {
                        t_merge_2e53_0001 = 1;
                    }
                    cAllowed = t_merge_2e53_0001;
                    lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
                    FSendPlrMsg(lppl->iPlayer, 123, lppl->id, lppl->id, i <= 0 ? 0 : 1, iEnv, iEnv * 256 + cAllowed, 0, 0, 0);
                }
            }
            return 1;
        case 27:
            idPlayer = lppl->iPlayer;
            LookupBestPlanetaryScanner(&part);
            idPlayer = -1;
            iItem = part.hs.iItem + 18;
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
            FSendPlrMsg(lppl->iPlayer, 124, lppl->id, lppl->id, -32768, iItem - 18, 0, 0, 0, 0);
            lppl->iScanner = iItem - 18;
        case 3:
        case 10:
        case 11:
            return 1;
        }
        cBuilt = cBuilt + FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id);
        if (cBuilt <= 1) {
            FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
        } else {
            FSendPlrMsg2(lppl->iPlayer, idm + 1, lppl->id, cBuilt, lppl->id);
        }
    } else {
        if (iItem >= 16) {
            iItem = iItem - 16;
            lpshdef = rglpshdefSB[lppl->iPlayer] + iItem;
            if (lpshdef->fFree == 0x0 && FCanBuildShdef(lpshdef, lppl->iPlayer) != 0) {
                idm = idmHasBuiltNew;
                if (lpshdef->hul.wtCargoMax != 0x0) {
                    idm = idm + 1;
                    if ((uint32_t)lpshdef->hul.wtCargoMax == 0xffff) {
                        idm = idm + 1;
                    }
                }
                FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, lppl->iPlayer << 0x5 | iItem + 16, LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0,
                            0, 0, 0);
                if (lppl->fStarbase != 0x0 && rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef > rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef) {
                    KillQueuedShips(lppl);
                }
                iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                if (lppl->fStarbase == 0x0) {
                    lppl->fStarbase = 0x1;
                } else {
                    rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 0x1;
                }
                lppl->isb = iItem;
                if (iWarp <= 0) {
                    iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                    if (iWarp <= 0) {
                        lppl->iWarpFling = 0x0;
                        lppl->idFling = 0x0;
                        KillQueuedMassPackets(lppl);
                    } else {
                        lppl->iWarpFling = iWarp + fTwoMAs - 4;
                    }
                }
                lpshdef->cBuilt = lpshdef->cBuilt + 0x1;
                lpshdef->cExist = lpshdef->cExist + 0x1;
                return 1;
            }
            return 0;
        }
        if (lppl->fStarbase == 0x0 || iItem >= 16) {
            return 0;
        }
        lpshdef = rglpshdef[lppl->iPlayer] + iItem;
        if (lpshdef->fFree != 0x0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
            FSendPlrMsg2(lppl->iPlayer, 79, lppl->id, iItem + 1, 0);
            return 0;
        }
        if (rgplr[lppl->iPlayer].cFleet == 0x200) {
            i = 0;
            while (1) {
                if (i >= cFleet)
                    goto L_214f;
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0x0 || lpfl->iPlayer > lppl->iPlayer)
                    goto L_214f;
                if (lpfl->iPlayer >= lppl->iPlayer && lpfl->lpplord->rgord[0].pt.x == rgptPlan[lppl->id].x &&
                    lpfl->lpplord->rgord[0].pt.y == rgptPlan[lppl->id].y && 32766 - cBuilt > lpfl->rgcsh[iItem])
                    break;
                i = i + 1;
            }
            if (lpfl->rgcsh[iItem] == 0 || lpfl->rgdv[iItem].pctDp == 0x0) {
                lpfl->rgdv[iItem].dp = 0x0;
            } else {
                dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
                cshOrig = lpfl->rgcsh[iItem];
                cshDamaged = LOWORD((int32_t)((int32_t)(lpfl->rgdv[iItem].pctSh * (int32_t)cshOrig) / 0x64));
                if (cshDamaged == 0) {
                    cshDamaged = 1;
                }
                dpOrig = (int32_t)((int32_t)((int32_t)((int32_t)((uint32_t)dpShdef * lpfl->rgdv[iItem].pctDp) / 0xa) * (int32_t)cshDamaged) / 0x32);
                lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)((int32_t)((int32_t)cshDamaged * 100) / (int32_t)(cshOrig + cBuilt)));
                if (lpfl->rgdv[iItem].pctSh == 0x0) {
                    lpfl->rgdv[iItem].pctSh = 0x1;
                }
                cshDamaged = LOWORD((int32_t)((int32_t)(lpfl->rgdv[iItem].pctSh * (int32_t)(cshOrig + cBuilt)) / 0x64));
                if (cshDamaged == 0) {
                    cshDamaged = 1;
                }
                lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)((int32_t)((int32_t)((int32_t)(dpOrig * 5) / (int32_t)cshDamaged) * 100) / (int32_t)dpShdef));
            }
            CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
            FSendPlrMsg(lppl->iPlayer, 313, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, lpfl->id, 0, 0, 0);
            return 1;
        L_214f:
            FSendPlrMsg(lppl->iPlayer, 186, lppl->id, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, 0, 0, 0, 0);
            return 0;
        }
        lpfl = LpflNew(lppl->iPlayer, lppl->id);
        CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
        lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
        if (lppl->idRoute == 0x0) {
            AutoFleetOrder(lpfl, lppl);
            if (cBuilt != 1) {
                FSendPlrMsg(lppl->iPlayer, 48, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, 0, 0, 0, 0);
            } else {
                FSendPlrMsg2(lppl->iPlayer, 47, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 0x5 | iItem);
            }
        } else {
            AutoRouteFleet(lpfl, lppl);
            if (cBuilt != 1) {
                idm = lpfl->lpplord->rgord[1].iWarp == 0x0 ? idmStarbaseHasBuiltNewShipsWhichWill : idmStarbaseHasBuiltNewShipsWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 0x5 | iItem, lppl->idRoute - 1, 0, 0, 0);
            } else {
                idm = lpfl->lpplord->rgord[1].iWarp == 0x0 ? idmStarbaseHasBuiltNewWhichWillRouted : idmStarbaseHasBuiltNewWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 0x5 | iItem, lppl->idRoute - 1, 0, 0, 0, 0);
            }
        }
    }
    return 1;
}

void CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip) {
    lpfl->rgcsh[ishdef] = lpfl->rgcsh[ishdef] + cShip;
    rglpshdef[iPlr][ishdef].cExist = rglpshdef[iPlr][ishdef].cExist + (int32_t)cShip;
    rglpshdef[iPlr][ishdef].cBuilt = rglpshdef[iPlr][ishdef].cBuilt + (int32_t)cShip;
    return;
}

void RandomEvents() {
    MeteorStrike();
    PlanetaryClimateChange();
    DiscoverNewMinerals();
    MysteryTrader();
    return;
}

void TransferToOthers() {
    int32_t   l2;
    int16_t   idDst;
    XFER      rgxf[2];
    int16_t   idSrc;
    int16_t   i;
    MessageId idm;
    XFERFULL *lpxfMax;
    XFERFULL *lpxfCur;
    int32_t   l;

    if (cXferFull != 0) {
        lpxfCur = lpxf;
        lpxfMax = lpxf + cXferFull;
        for (; lpxfCur < lpxfMax; lpxfCur++) {
            if (FLookupObject(lpxfCur->grobj2, lpxfCur->id2, &rgxf[1].fl) != 0) {
                if (lpxfCur->grobj1 != 0x1) {
                    rgxf[0].fl.iPlayer = lpxfCur->id1 >> 0x9 & 0xf;
                } else {
                    rgxf[0].fl.iPlayer = LpplFromId(lpxfCur->id1)->iPlayer;
                }
                for (i = 0; i < 5; i++) {
                    l2 = lpxfCur->rgcQuan[i];
                    if (l2 != 0) {
                        l = ChgCargo(lpxfCur->grobj2, lpxfCur->id2, i, l2, 0x0);
                        if (l != l2) {
                            idm = i == 4 ? idmAttemptedTransferColonistsSuccessfullyReceivedRe : idmAttemptedTransferSuccessfullyReceived;
                            idSrc = lpxfCur->id1 | (lpxfCur->grobj1 == 0x2 ? 0x8000 : 0x0);
                            idDst = lpxfCur->id2 | (lpxfCur->grobj2 == 0x2 ? 0x8000 : 0x0);
                            if (l == 0) {
                                idm = idm + 4;
                            }
                            FSendPlrMsg(rgxf[0].fl.iPlayer, idm, idSrc, idSrc, LOWORD(l2), HIWORD(l2), i, idDst, LOWORD(l), HIWORD(l));
                            idm = i == 4 ? idmReceivedHoweverColonistsSentRemainsOtherColonist : idmReceivedHoweverSentRemainderLostSpace;
                            if (l != 0) {
                                FSendPlrMsg(rgxf[1].fl.iPlayer, idm, idDst, idDst, LOWORD(l), HIWORD(l), i, idSrc, LOWORD(l2), HIWORD(l2));
                            } else {
                                FSendPlrMsg(rgxf[1].fl.iPlayer, idm + 4, idDst, idDst, LOWORD(l2), HIWORD(l2), i, idSrc, 0, 0);
                            }
                        } else {
                            idm = i == 4 ? idmSuccessfullyTransferred2 : idmSuccessfullyTransferred;
                            idSrc = lpxfCur->id1 | (lpxfCur->grobj1 == 0x2 ? 0x8000 : 0x0);
                            idDst = lpxfCur->id2 | (lpxfCur->grobj2 == 0x2 ? 0x8000 : 0x0);
                            FSendPlrMsg(rgxf[0].fl.iPlayer, idm, idSrc, idSrc, LOWORD(l), HIWORD(l), i, idDst, 0, 0);
                            idm = i == 4 ? idmSuccessfullyReceived2 : idmSuccessfullyReceived;
                            FSendPlrMsg(rgxf[1].fl.iPlayer, idm, idDst, idDst, LOWORD(l), HIWORD(l), i, idSrc, 0, 0);
                        }
                    }
                }
            }
        }
    }
    return;
}

void DropColonists() {
    COLDROP *lpcdLook;
    int16_t  fTie;
    int32_t  cMax;
    PLANET   pl;
    int32_t  lDefensePower;
    int32_t  cPowerTot;
    COLDROP *lpcdCur;
    int16_t  iMax;
    int16_t  idPlanet;
    int16_t  iplrOldOwner;
    int32_t  cColTot;
    int32_t  lOldPop;
    int32_t  c2nd;
    int16_t  i;
    int32_t  rgcPower[16];
    int16_t  cSides;
    float    pctSurvive;
    int32_t  rgcCol[16];
    int32_t  lPower;
    COLDROP *lpcdMax;
    int16_t  cpq;
    int16_t  iDst;
    PROD     prod;
    int16_t  ipq;
    int16_t  iTech;
    int16_t  iBonus;
    uint16_t t_scratch_mf8;
    PLANET  *t_call_4159;

    if (cColDrop != 0) {
        lpcdCur = lpcd;
        lpcdMax = lpcd + cColDrop;
        for (; lpcdCur < lpcdMax; lpcdCur++) {
            if (lpcdCur->idPlanetDst != -1 && lpcdCur->cColonist != 0) {
                memset(rgcCol, 0, 0x40);
                memset(rgcPower, 0, 0x40);
                cPowerTot = 0;
                cColTot = 0;
                idPlanet = lpcdCur->idPlanetDst;
                FLookupPlanet(idPlanet, &pl);
                iplrOldOwner = pl.iPlayer;
                CalcPctSurvive(&pl, &pctSurvive, 0x0);
                pctSurvive = pctSurvive + (1.0 - pctSurvive) / 4.0;
                for (lpcdLook = lpcdCur; lpcdLook < lpcdMax; lpcdLook++) {
                    if (idPlanet == lpcdLook->idPlanetDst) {
                        if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) != raMacintosh || (lpcdLook->fCanColonize != 0x0 && pl.iPlayer == -1)) {
                            if (pl.iPlayer != -1 || lpcdLook->fCanColonize != 0x0) {
                                if (pl.fStarbase == 0x0 || pl.iPlayer == -1) {
                                    rgcCol[lpcdLook->idPlr] = rgcCol[lpcdLook->idPlr] + lpcdLook->cColonist;
                                    cColTot = cColTot + lpcdLook->cColonist;
                                    if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) != raAttack) {
                                        if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) != raMacintosh) {
                                            lPower = 110;
                                        } else {
                                            lPower = 0;
                                        }
                                    } else {
                                        lPower = 165;
                                    }
                                    lPower = (int32_t)((double)(int32_t)((int32_t)(lpcdLook->cColonist * lPower) / 100) * pctSurvive);
                                    cPowerTot = cPowerTot + lPower;
                                    rgcPower[lpcdLook->idPlr] = rgcPower[lpcdLook->idPlr] + lPower;
                                } else {
                                    FSendPlrMsg2(lpcdLook->idPlr, 88, pl.id, pl.id, 0);
                                }
                            } else {
                                FSendPlrMsg(lpcdLook->idPlr, 2, pl.id, LOWORD(lpcdLook->cColonist), HIWORD(lpcdLook->cColonist), pl.id, 0, 0, 0, 0);
                            }
                        } else {
                            FSendPlrMsg2(lpcdLook->idPlr, 87, pl.id, pl.id, 0);
                        }
                        lpcdLook->idPlanetDst = -1;
                    }
                }
                if (pl.iPlayer == -1) {
                    lDefensePower = 0;
                    lOldPop = 0;
                } else {
                    if (GetRaceStat(&rgplr[pl.iPlayer], rsMajorAdv) != raDefend) {
                        lPower = 100;
                    } else {
                        lPower = 200;
                    }
                    lDefensePower = (int32_t)((int32_t)(pl.rgwtMin[3] * lPower) / 100);
                    if (lDefensePower > cPowerTot) {
                        for (i = 0; i < 16; i++) {
                            if (rgcCol[i] != 0) {
                                if (pctSurvive != 1.0) {
                                    FSendPlrMsg(i, 1, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), pl.id, (int32_t)((1.0 - pctSurvive) * 10000.0),
                                                pl.iPlayer | 0x30, 0, 0);
                                    FSendPlrMsg(pl.iPlayer, 4, pl.id, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), i | 0x30, 0, 0, 0);
                                } else {
                                    FSendPlrMsg(i, 0, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), pl.id, pl.iPlayer | 0x30, 0, 0, 0);
                                    FSendPlrMsg(pl.iPlayer, 3, pl.id, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), i | 0x30, 0, 0, 0);
                                }
                            }
                        }
                        pl.rgwtMin[3] = pl.rgwtMin[3] - (int32_t)((int32_t)(pl.rgwtMin[3] * cPowerTot) / lDefensePower);
                        goto WritePlanet;
                    }
                    lOldPop = pl.rgwtMin[3];
                    UninhabitPlanet(&pl);
                }
                cMax = -1;
                c2nd = 0;
                cSides = 0;
                fTie = 0;
                iMax = 0;
                for (i = 0; i < game.cPlayer; i++) {
                    if (rgcCol[i] != 0) {
                        cSides = cSides + 1;
                        if (rgcPower[i] >= cMax) {
                            if (rgcPower[i] != cMax) {
                                fTie = 0;
                                c2nd = cMax;
                                cMax = rgcPower[i];
                                iMax = i;
                            } else {
                                fTie = 1;
                            }
                        }
                    }
                }
                if (cMax < 0)
                    continue;
                if (fTie == 0) {
                    if (iplrOldOwner == -1) {
                        if (cSides <= 1) {
                            FSendPlrMsg2(iMax, (GetRaceStat(&rgplr[iMax], rsMajorAdv) == raMacintosh ? 1 : 0) + 10, pl.id, pl.id, 0);
                        } else {
                            for (i = 0; i < 16; i++) {
                                if (rgcCol[i] != 0) {
                                    if (i != iMax) {
                                        FSendPlrMsg(i, 9, pl.id, cSides, pl.id, iMax | 0xb0, 0, 0, 0, 0);
                                    } else {
                                        FSendPlrMsg2(i, 8, pl.id, cSides, pl.id);
                                    }
                                }
                            }
                        }
                    } else {
                        for (i = 0; i < 16; i++) {
                            if (rgcCol[i] != 0) {
                                if (i != iMax) {
                                    FSendPlrMsg2(i, 13, pl.id, pl.id, 0);
                                } else {
                                    FSendPlrMsg2(i, 12, pl.id, iplrOldOwner | 0x20, pl.id);
                                }
                            }
                        }
                        FSendPlrMsg(iplrOldOwner, 7, pl.id, iMax | 0x30, pl.id, LOWORD(rgcCol[iMax]), HIWORD(rgcCol[iMax]), 0, 0, 0);
                        memset(rgTechBattle, 0, 0x6);
                        memset(rgTechTrader, 0, 0xd);
                        for (i = 0; i < 6; i++) {
                            rgTechBattle[i] = rgplr[iplrOldOwner].rgTech[i];
                        }
                        i = ITechLearnATech(iMax, -1, pl.id, idmWreckageDiscoveredBattleHasBoostedResearchResour, 0x0);
                        pl.iPlayer = -1;
                    }
                    if (iMax != -1) {
                        cpq = rgplr[iMax].zpq1.cpq;
                        t_scratch_mf8 = rgplr[iMax].zpq1.fNoResearch;
                        pl.fNoResearch = t_scratch_mf8;
                        if (cpq > 0) {
                            pl.lpplprod = (PLPROD *)LpplAlloc(0x4, rgplr[iMax].zpq1.cpq, htOrd);
                            memset(&prod, 0, sizeof(PROD));
                            prod.grobj = grobjPlanet;
                            iDst = 0;
                            for (ipq = 0; ipq < cpq; ipq++) {
                                if ((GetRaceStat(&rgplr[iMax], rsMajorAdv) != raMacintosh || rgplr[iMax].zpq1.rgpq[ipq].mdIdle > 0x2) &&
                                    (GetRaceStat(&rgplr[iMax], rsMajorAdv) != raTerra ||
                                     (rgplr[iMax].zpq1.rgpq[ipq].mdIdle != 0x4 && rgplr[iMax].zpq1.rgpq[ipq].mdIdle != 0x5))) {
                                    prod.iItem = rgplr[iMax].zpq1.rgpq[ipq].mdIdle;
                                    prod.cItem = rgplr[iMax].zpq1.rgpq[ipq].cQuan;
                                    pl.lpplprod->rgprod[iDst] = prod;
                                    iDst = iDst + 1;
                                }
                            }
                            if (iDst <= 0) {
                                FreePl((PL *)pl.lpplprod);
                                pl.lpplprod = 0x0;
                            } else {
                                pl.lpplprod->iprodMac = LOBYTE(iDst);
                                t_call_4159 = LpplFromId(pl.id);
                                t_call_4159->lpplprod = pl.lpplprod;
                            }
                        }
                    }
                    pl.iPlayer = iMax;
                    if (GetRaceStat(&rgplr[iMax], rsMajorAdv) == raMacintosh) {
                        pl.fStarbase = 0x1;
                        pl.isb = 0x0;
                        rglpshdefSB[iMax]->cExist = rglpshdefSB[iMax]->cExist + 0x1;
                        rglpshdefSB[iMax]->cBuilt = rglpshdefSB[iMax]->cBuilt + 0x1;
                    }
                    if (cPowerTot != 0 && cMax != 0) {
                        lPower = (int32_t)((int32_t)(cMax * (cPowerTot - lDefensePower)) / cPowerTot);
                        pl.rgwtMin[3] = (int32_t)((int32_t)(rgcCol[iMax] * lPower) / cMax);
                    } else {
                        pl.rgwtMin[3] = rgcCol[iMax];
                    }
                    if (c2nd > 0) {
                        pl.rgwtMin[3] = (int32_t)((int32_t)(pl.rgwtMin[3] * (cMax - c2nd)) / cMax);
                    }
                    if (pl.rgwtMin[3] < 1) {
                        pl.rgwtMin[3] = 1;
                    }
                } else {
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgcCol[i] != 0) {
                            FSendPlrMsg2(i, 6, pl.id, cSides, pl.id);
                        }
                    }
                    if (iplrOldOwner != -1) {
                        FSendPlrMsg2(iplrOldOwner, 5, pl.id, cSides, pl.id);
                        pl.iPlayer = -1;
                    }
                }
            WritePlanet:
                if (pl.iPlayer != -1 && pl.fArtifact != 0x0) {
                    pl.fArtifact = 0x0;
                    if (game.fNoRandom == 0x0) {
                        iTech = Random(6);
                        iBonus = Random(301) + 100;
                        if (pl.rgwtMin[3] < 10) {
                            iBonus = (int32_t)(LOWORD(pl.rgwtMin[3]) * iBonus) / 10;
                        }
                        FSendPlrMsg(pl.iPlayer, 94, -2, pl.id, iTech, iBonus, 0, 0, 0, 0);
                        rgplr[pl.iPlayer].rgResSpent[iTech] = rgplr[pl.iPlayer].rgResSpent[iTech] + (int32_t)iBonus;
                        if (game.fSlowTech != 0x0) {
                            iBonus = iBonus >> 0x1;
                        }
                    }
                }
                FLookupPlanet(-1, &pl);
            }
        }
        cColDrop = 0;
    }
    return;
}

void HealShips() {
    int16_t pctShipHeal;
    int16_t dpHeal;
    PLANET *lppl;
    int16_t i;
    FLEET  *lpfl;
    SHDEF  *lpshdef;
    int16_t pct;
    int16_t ishdef;
    PLANET *lpplMac;

    pctShipHeal = 0;
    for (i = 0; i < cFleet; i++) {
        lpfl = rglpfl[i];
        if (rglpfl[i] == 0x0)
            break;
        if (lpfl->fDead == 0x0 && lpfl->fNoHeal == 0x0) {
            dpHeal = 0;
            pctShipHeal = 0;
            for (ishdef = 0; ishdef < 16; ishdef++) {
                if (lpfl->rgdv[ishdef].dp != 0x0) {
                    dpHeal = 1;
                }
                if (lpfl->rgcsh[ishdef] != 0) {
                    if (rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef != ihuldefSuperFuelXport) {
                        if (pctShipHeal < 5 && rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef == ihuldefFuelTransport) {
                            pctShipHeal = 25;
                        }
                    } else {
                        pctShipHeal = 50;
                    }
                }
            }
            if (dpHeal != 0) {
                if (lpfl->fHereAllTurn != 0x0) {
                    if (lpfl->idPlanet != -1) {
                        lppl = LpplFromId(lpfl->idPlanet);
                        if (lppl->iPlayer != lpfl->iPlayer) {
                            if (lppl->iPlayer != -1) {
                                pct = 15;
                            } else {
                                pct = 15;
                            }
                        } else if (lppl->fStarbase == 0x0 || lppl->fNoHeal != 0x0) {
                            pct = 25;
                        } else {
                            lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
                            if (LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax == 0x0) {
                                pct = 40;
                            } else {
                                pct = 100;
                            }
                        }
                    } else {
                        pct = 10;
                    }
                } else {
                    pct = 5;
                }
                if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raDefend) {
                    pct = pct * 2;
                }
                pct = pct + pctShipHeal;
                for (ishdef = 0; ishdef < 16; ishdef++) {
                    if (lpfl->rgdv[ishdef].dp != 0x0) {
                        if (lpfl->rgdv[ishdef].pctDp <= (uint16_t)pct) {
                            lpfl->rgdv[ishdef].dp = 0x0;
                        } else {
                            lpfl->rgdv[ishdef].pctDp = lpfl->rgdv[ishdef].pctDp - pct;
                        }
                    }
                }
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->fStarbase != 0x0 && lppl->fNoHeal == 0x0) {
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raDefend) {
                pct = 50;
            } else {
                pct = 75;
            }
            if (lppl->pctDp != 0x0) {
                lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
                if ((uint16_t)pct <= lppl->pctDp) {
                    lppl->pctDp = lppl->pctDp - pct;
                } else {
                    lppl->pctDp = 0x0;
                }
            }
        }
    }
    return;
}

void AutoTerraform() {
    int16_t rgMax[3];
    int16_t rgp[16];
    PLANET *lppl;
    int16_t i;
    int16_t rgMin[3];
    int16_t rgCost[3];
    int16_t fTerra;
    PLANET *lpplMac;
    int16_t t_scratch_m42_2;

    fTerra = 0;
    for (i = 0; i < game.cPlayer; i++) {
        rgp[i] = GetRaceStat(&rgplr[i], rsMajorAdv) == raTerra ? 1 : 0;
        if (rgp[i] != 0) {
            fTerra = 1;
        }
    }
    if (fTerra != 0) {
        lppl = lpPlanets;
        lpplMac = lpPlanets + cPlanet;
        for (; lppl < lpplMac; lppl++) {
            if (lppl->iPlayer != -1 && rgp[lppl->iPlayer] != 0) {
                if (lppl->fStarbase != 0x0 && lppl->iPlayer == -1) {
                    lppl->fStarbase = 0x0;
                }
                i = Random(3);
                if ((int16_t)rgplr[lppl->iPlayer].rgEnvVar[i] != -1 && (int16_t)rgplr[lppl->iPlayer].rgEnvVar[i] != (int16_t)lppl->rgEnvVarOrig[i] &&
                    Random(10) == 0) {
                    if (lppl->rgwtMin[3] < 1000) {
                        t_scratch_m42_2 = Random(1000);
                        if (t_scratch_m42_2 >= LOWORD(lppl->rgwtMin[3]))
                            goto L_4b5c;
                    }
                    if ((int16_t)rgplr[lppl->iPlayer].rgEnvVar[i] >= (int16_t)lppl->rgEnvVarOrig[i]) {
                        lppl->rgEnvVarOrig[i] = lppl->rgEnvVarOrig[i] + 1;
                    } else {
                        lppl->rgEnvVarOrig[i] = lppl->rgEnvVarOrig[i] - 1;
                    }
                    FSendPlrMsg2(lppl->iPlayer, 348, lppl->id, lppl->id, i);
                }
            L_4b5c:
                if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) != 0) {
                    for (i = 0; i < 3; i++) {
                        if (rgMin[i] == -1) {
                            if (rgMax[i] != -1) {
                                lppl->rgEnvVar[i] = LOBYTE(rgMax[i]);
                            }
                        } else {
                            lppl->rgEnvVar[i] = LOBYTE(rgMin[i]);
                        }
                    }
                    i = PctPlanetDesirability(lppl, lppl->iPlayer);
                    FSendPlrMsg2(lppl->iPlayer, 342, lppl->id, lppl->id, i);
                }
            }
        }
    }
    return;
}

void RemoteTerraforming() {
    int16_t  fHelp;
    int16_t  iBest;
    int16_t  pctCur;
    PLANET  *lppl;
    int16_t  ifl;
    FLEET   *lpfl;
    int16_t  cDone;
    int16_t  iEnv;
    int16_t  cAllowed;
    int32_t  ipct;
    int16_t  pctNew;
    int16_t  t_merge_4d92_0001;
    int32_t  t_4dd5;
    int16_t  t_merge_4e9d_0001;
    uint16_t t_merge_4f13_0001;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->fDead == 0x0 && lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].iPlayer != -1) {
            ipct = PctTerraFromLpfl(lpfl);
            if (ipct > 0) {
                lppl = lpPlanets + lpfl->idPlanet;
                if (lpfl->iPlayer != lppl->iPlayer && (int16_t)rgplr[lpfl->iPlayer].rgmdRelation[lppl->iPlayer] != 1 && lpfl->iPlayer != lppl->iPlayer) {
                    t_merge_4d92_0001 = 0;
                } else {
                    t_merge_4d92_0001 = 1;
                }
                fHelp = t_merge_4d92_0001;
                if (fHelp != 0 || lppl->fStarbase == 0x0) {
                    pctCur = PctPlanetDesirability(lppl, lppl->iPlayer);
                    cDone = 0;
                    while (1) {
                        t_4dd5 = ipct;
                        ipct = ipct - 1;
                        if (t_4dd5 <= 0)
                            break;
                        iBest = IBestRemoteTerra(lppl, lpfl->iPlayer, fHelp);
                        if (iBest == 0)
                            break;
                        iEnv = abs(iBest) - 1;
                        cAllowed = (int16_t)lppl->rgEnvVar[iEnv] + (iBest <= 0 ? -1 : 1);
                        if (0x1 <= (99 >= cAllowed ? cAllowed : 0x63)) {
                            if (99 >= cAllowed) {
                                t_merge_4e9d_0001 = cAllowed;
                            } else {
                                t_merge_4e9d_0001 = 99;
                            }
                        } else {
                            t_merge_4e9d_0001 = 1;
                        }
                        cAllowed = t_merge_4e9d_0001;
                        lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
                        cDone = cDone + 1;
                    }
                    pctNew = PctPlanetDesirability(lppl, lppl->iPlayer);
                    t_merge_4f13_0001 = pctCur == pctNew ? 0x1 : 0x0;
                    FSendPlrMsg(lpfl->iPlayer, (fHelp == 0 ? 346 : 300) + t_merge_4f13_0001, lpfl->id | 0x8000, lpfl->id, lppl->id, pctCur, pctNew, 0, 0, 0);
                    if (lpfl->iPlayer != lppl->iPlayer && pctNew != pctCur) {
                        FSendPlrMsg(lppl->iPlayer, fHelp == 0 ? 346 : 300, lppl->id, lpfl->id, lppl->id, pctCur, pctNew, 0, 0, 0);
                    }
                }
            }
        }
    }
    return;
}

int16_t FQueueColonistDrop(FLEET *lpfl, PLANET *lppl, int32_t cColonists) {
    int16_t  iColDrop;
    COLDROP *lpcdT;

    if (cColonists <= 0) {
        return 1;
    }
    iColDrop = 0;
    lpcdT = lpcd;
    for (; iColDrop < cColDrop && (lpcdT->idFleetSrc != lpfl->id || lpcdT->idPlanetDst != lppl->id); iColDrop++) {
        lpcdT = lpcdT + 1;
    }
    if (iColDrop == cColDrop) {
        if (cColDrop >= 1000) {
            return 0;
        }
        lpcdT->idFleetSrc = lpfl->id;
        lpcdT->idPlr = lpfl->iPlayer;
        lpcdT->idPlanetDst = lppl->id;
        lpcdT->cColonist = 0;
        lpcdT->fCanColonize = 0x1;
        cColDrop = cColDrop + 1;
    }
    lpcdT->cColonist = lpcdT->cColonist + cColonists;
    return LOWORD(cColonists);
}

void UpdatePopulations() {
    int32_t  lPopChg;
    PLANET  *lppl;
    PLANET  *lpplMac;
    int32_t  lPopOld;
    int16_t  fMac;
    uint16_t t_merge_52d8_0001;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer != -1 && lppl->rgwtMin[3] != 0) {
            lPopChg = ChgPopFromPlanet(lppl, 1);
            if (lPopChg != 0 && lPopChg < 0 && lppl->rgwtMin[3] > 0) {
                lPopOld = lppl->rgwtMin[3] - lPopChg;
                if (PctPlanetDesirability(lppl, lppl->iPlayer) >= 0) {
                    FSendPlrMsg(lppl->iPlayer, 38, lppl->id, lppl->id, -LOWORD(lPopChg), LOWORD((uint32_t)((uint32_t)-lPopChg >> 0x10)), 0, 0, 0, 0);
                } else {
                    FSendPlrMsg(lppl->iPlayer, 37, lppl->id, lppl->id, LOWORD(lPopOld), HIWORD(lPopOld), LOWORD(lppl->rgwtMin[3]), HIWORD(lppl->rgwtMin[3]), 0,
                                0);
                }
            }
        }
        if (lppl->iPlayer != -1 && lppl->rgwtMin[3] == 0) {
            fMac = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh ? 1 : 0;
            t_merge_52d8_0001 = lPopChg < 0 ? 0x23 : 0x40;
            FSendPlrMsg2(lppl->iPlayer, t_merge_52d8_0001 + fMac, lppl->id, lppl->id, 0);
            UninhabitPlanet(lppl);
        }
        if (lppl->iPlayer == -1) {
            UninhabitPlanet(lppl);
        }
    }
    return;
}

void UpdateGuesses() {
    PLANET *lppl;
    float   pct;
    PLANET *lpplMac;
    int32_t l;
    int16_t t_call_53c8;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->rgwtMin[3] != 0) {
            l = lppl->rgwtMin[3];
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                t_call_53c8 = Random((int32_t)(l >> 0x2));
                l = l + (-(int32_t)(l >> 0x3) + (int32_t)t_call_53c8);
                l = (int32_t)(l >> 0x2);
                if (l <= 4090) {
                    if (l < 1) {
                        l = 1;
                    }
                } else {
                    l = 4090;
                }
            } else {
                l = 0;
            }
            lppl->uPopGuess = LOWORD(l);
            if (lppl->cDefenses != 0x0) {
                CalcPctSurvive(lppl, &pct, 0x0);
                l = 100 - (int32_t)(pct * 100.0 + 0.5) + 4;
                l = (int32_t)(l / 6);
                if (l < 1) {
                    l = 1;
                } else if (l > 15) {
                    l = 15;
                }
                lppl->uDefGuess = LOWORD(l);
            } else {
                lppl->uDefGuess = 0x0;
            }
        } else {
            lppl->uGuesses = 0x0;
        }
    }
    return;
}

void MineMinerals() {
    int32_t rglQuan[3];
    PLANET *lppl;
    PLANET *lpplMac;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        EstMineralsMined(lppl, rglQuan, -1, 1);
    }
    return;
}

void MeteorStrike() {
    int16_t  rgEnv[3];
    int16_t  iT;
    int32_t  rgQuan[4];
    int16_t  iSize;
    PLANET  *lppl;
    int16_t  rgAffect[3];
    int16_t  i;
    int16_t  iConc;
    int16_t  j;
    uint16_t t_merge_5834_0001;

    if (Random(20) == 0) {
        lppl = lpPlanets + Random(cPlanet);
        if ((lppl->iPlayer == -1 || lppl->rgwtMin[3] <= 50 || game.turn >= 0x14) && game.turn >= 0xa) {
            iSize = Random(4);
            for (i = 0; i < 3; i++) {
                rgEnv[i] = i;
            }
            for (i = 0; i < 3; i++) {
                j = Random(3);
                iT = rgEnv[i];
                rgEnv[i] = rgEnv[j];
                rgEnv[j] = iT;
            }
            for (i = 0; i < 3; i++) {
                rgAffect[i] = i;
                rgQuan[i] = (int32_t)(Random(250) + 50);
            }
            for (i = 0; i < 2; i++) {
                j = Random(3 - i) + i;
                iT = rgAffect[i];
                rgAffect[i] = rgAffect[j];
                rgAffect[j] = iT;
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (i != lppl->iPlayer || GetRaceStat(&rgplr[i], rsMajorAdv) == raMacintosh) {
                    t_merge_5834_0001 = 0x83;
                } else {
                    t_merge_5834_0001 = 0x87;
                }
                FSendPlrMsg(i, t_merge_5834_0001 + iSize, lppl->id, lppl->id, rgEnv[0], rgEnv[1], rgEnv[2], 0, 0, 0);
            }
            if (lppl->iPlayer != -1 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->rgwtMin[3] = lppl->rgwtMin[3] - (int32_t)((int32_t)(lppl->rgwtMin[3] * (int32_t)(20 * iSize + 0x19)) / 0x64);
            }
            for (i = 0; i <= iSize && i < 3; i++) {
                rgQuan[rgAffect[i]] = rgQuan[rgAffect[i]] + (int32_t)(Random(17000) + 3000);
                iConc = lppl->rgMinConc[rgAffect[i]];
                iConc = iConc + (Random(50) + 50);
                if (iSize == 3) {
                    iConc = iConc + (Random(15) + 15);
                }
                if (iConc > 200) {
                    iConc = 200;
                }
                lppl->rgMinConc[rgAffect[i]] = LOBYTE(iConc);
            }
            for (i = 0; i < 3; i++) {
                lppl->rgwtMin[i] = lppl->rgwtMin[i] + (int32_t)(rgQuan[i] >> 0x4);
            }
            for (i = 0; i < 3 && i <= iSize; i++) {
                iT = Random(3) + 3;
                if (iSize == 3) {
                    iT = iT + (Random(3) + 3);
                }
                if (Random(2) != 0) {
                    iT = -iT;
                }
                j = (int16_t)lppl->rgEnvVar[i] + iT;
                if (j >= 1) {
                    if (j > 99) {
                        j = 99;
                    }
                } else {
                    j = 1;
                }
                lppl->rgEnvVar[i] = LOBYTE(j);
                j = (int16_t)lppl->rgEnvVarOrig[i] + iT;
                if (j >= 1) {
                    if (j > 99) {
                        j = 99;
                    }
                } else {
                    j = 1;
                }
                lppl->rgEnvVarOrig[i] = LOBYTE(j);
            }
            TossNonAutoBuildItems(lppl);
        }
    }
    return;
}

void TossNonAutoBuildItems(PLANET *lppl) {
    int16_t iDst;
    int16_t iSrc;

    if (lppl->lpplprod != 0x0) {
        iDst = 0;
        for (iSrc = 0; iSrc < lppl->lpplprod->iprodMac; iSrc++) {
            if (lppl->lpplprod->rgprod[iSrc].grobj == grobjPlanet && lppl->lpplprod->rgprod[iSrc].iItem < mdIdleFactory) {
                if (iSrc > iDst) {
                    lppl->lpplprod->rgprod[iDst] = lppl->lpplprod->rgprod[iSrc];
                }
                iDst = iDst + 1;
            }
        }
        if (iDst <= 0) {
            FreePl((PL *)lppl->lpplprod);
            lppl->lpplprod = 0x0;
        } else {
            lppl->lpplprod->iprodMac = LOBYTE(iDst);
        }
    }
    return;
}

void PlanetaryClimateChange() {
    int16_t iT;
    PLANET *lppl;
    int16_t i;
    int16_t j;

    if (Random(20) == 0) {
        lppl = lpPlanets + Random(cPlanet);
        if (lppl->iPlayer == -1 || lppl->rgwtMin[3] <= 50 || game.turn >= 0x14) {
            i = Random(3);
            if (lppl->iPlayer != -1) {
                FSendPlrMsg2(lppl->iPlayer, 253, lppl->id, lppl->id, i);
            }
            iT = Random(3) + 3;
            if (iT == 3) {
                iT = iT + (Random(3) + 3);
            }
            if (Random(2) != 0) {
                iT = -iT;
            }
            j = (int16_t)lppl->rgEnvVar[i] + iT;
            if (j >= 1) {
                if (j > 99) {
                    j = 99;
                }
            } else {
                j = 1;
            }
            lppl->rgEnvVar[i] = LOBYTE(j);
            j = (int16_t)lppl->rgEnvVarOrig[i] + iT;
            if (j >= 1) {
                if (j > 99) {
                    j = 99;
                }
            } else {
                j = 1;
            }
            lppl->rgEnvVarOrig[i] = LOBYTE(j);
            TossNonAutoBuildItems(lppl);
        }
    }
    return;
}

void DiscoverNewMinerals() {
    PLANET *lppl;
    int16_t i;

    if (Random(15 - game.mdSize) == 0) {
        lppl = lpPlanets + Random(cPlanet);
        if (game.turn >= 0xa) {
            i = Random(3);
            if (lppl->iPlayer != -1) {
                FSendPlrMsg(lppl->iPlayer, 254, lppl->id, lppl->id, i, 0, 0, 0, 0, 0);
            }
            if (lppl->rgMinConc[i] < 0xb4) {
                lppl->rgMinConc[i] = lppl->rgMinConc[i] + LOBYTE(Random(15) + 5);
            }
        }
    }
    return;
}

void MysteryTrader() {
    int16_t  iSrc;
    int16_t  cRand;
    int16_t  i;
    THING   *lpth;
    int16_t  grbitTrader;
    int16_t  rgC[4];
    uint16_t t_scratch_m18;
    int16_t  t_call_6055;

    if (game.turn >= 0x28) {
        if ((uint32_t)game.turn % 0x64 != 0x47) {
            if ((uint32_t)game.turn % 0x64 != 0x21) {
                if ((game.turn & 0x7f) != 0x31) {
                    if ((game.turn & 0x1) != 0x0) {
                        return;
                    }
                    cRand = 7;
                } else {
                    cRand = 4;
                }
            } else {
                cRand = 3;
            }
        } else {
            cRand = 2;
        }
        if (Random(cRand) == 0) {
            lpth = LpthNew(0, ithMysteryTrader);
            if (lpth != 0x0) {
                t_scratch_m18 = Random(5) + 8;
                lpth->tht.iWarp = t_scratch_m18;
                for (i = 0; i < 4; i = i + 2) {
                    rgC[i] = Random(400 * game.mdSize + 361) + 1020;
                }
                if (Random(2) != 0) {
                    rgC[1] = 400 * game.mdSize + 1380;
                    rgC[3] = 1020;
                } else {
                    rgC[1] = 1020;
                    rgC[3] = 400 * game.mdSize + 1380;
                }
                t_call_6055 = Random(2);
                iSrc = t_call_6055;
                lpth->pt.x = rgC[t_call_6055];
                lpth->pt.y = rgC[iSrc == 0 ? 1 : 0];
                lpth->tht.ptDest.x = rgC[iSrc + 2];
                lpth->tht.ptDest.y = rgC[(iSrc == 0 ? 1 : 0) + 2];
                if (game.turn >= 0x64) {
                    if (game.turn >= 0xfa) {
                        cRand = 2;
                    } else {
                        cRand = 3;
                    }
                } else {
                    cRand = 5;
                }
                if (lpth->tht.iWarp > 0x9) {
                    if (lpth->tht.iWarp >= 0xb) {
                        cRand = cRand - 1;
                    }
                } else {
                    cRand = cRand + 1;
                }
                if (Random(10) >= cRand) {
                    grbitTrader = 0x1 << Random(13);
                    switch (grbitTrader) {
                    case 64:
                    case 128:
                    case 1024:
                    case 2048:
                        grbitTrader = 0x1 << Random(13);
                        if (((game.turn < 0x78 && grbitTrader == 128) || (game.turn < 0x96 && grbitTrader == 1024) ||
                             (game.turn < 0xb4 && grbitTrader == 2048)) &&
                            Random(2) != 0) {
                            grbitTrader = 0;
                        }
                    default:
                    }
                    lpth->tht.grbitTrader = grbitTrader;
                } else if (Random(6) != 0) {
                    lpth->tht.grbitTrader = 0x0;
                } else {
                    lpth->tht.grbitTrader = 0x1000;
                }
                for (i = 0; i < game.cPlayer; i++) {
                    FSendPlrMsg2(i, 299, -6, lpth->idFull, 0);
                }
            }
        }
    }
    return;
}

void UpdatePlayerScores() {
    int32_t  lScoreTot;
    int16_t  cFirst;
    SCORE    score;
    int16_t  cDead;
    int16_t  c;
    int16_t  i;
    uint8_t  rgcCond[16];
    uint16_t wWinners2;
    int32_t  rglScore[16];
    int16_t  iScoreMax;
    int16_t  j;
    uint16_t wWinners;
    int16_t  imsg;
    int32_t  lScore2nd;
    int32_t  lScoreMax;
    uint16_t t_scratch_m86_5;
    int32_t  t_scratch_m88_2;
    uint16_t t_scratch_m86_6;
    uint16_t t_scratch_m86_7;
    int16_t  t_scratch_m86_8;
    uint16_t t_scratch_m86_9;
    int32_t  t_scratch_m88_6;
    uint16_t t_scratch_m86_10;
    uint16_t t_scratch_m86_12;
    uint16_t t_scratch_m86_13;

    cDead = 0;
    cFirst = 0;
    iScoreMax = 0;
    lScore2nd = 0;
    lScoreTot = 0;
    gd.fGameOverMan = 0x0;
    memset(rgcCond, 0, 0x10);
    for (i = 0; i < game.cPlayer; i++) {
        rglScore[i] = CalcPlayerScore(i, &score);
        vlprgScoreX[i].score = score;
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffe0) | (i & 0x1f);
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffdf) | 0x20;
        vlprgScoreX[i].wWord = vlprgScoreX[i].wWord & 0xc03f;
        lScoreTot = lScoreTot + rglScore[i];
        if (score.cPlanet == 0 && score.rgcsh[0] == 0x0 && score.rgcsh[1] == 0x0 && score.rgcsh[2] == 0x0 && rgplr[i].fDead == 0x0) {
            rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffe) | 0x1;
            for (j = 0; j < game.cPlayer; j++) {
                if (j != i) {
                    FSendPrependedPlrMsg(j, 187, -4, i | 0x30, 0, 0, 0, 0, 0, 0);
                }
            }
        }
        if (score.cPlanet >= MulDiv(cPlanet, GetVCVal(&game, 0, 0), 100)) {
            t_scratch_m86_5 = (vlprgScoreX[i].wWord | 0x40) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0x0;
            vlprgScoreX[i].wWord = vlprgScoreX[i].wWord | t_scratch_m86_5;
            if (GetVCCheck(&game, 0) != 0) {
                rgcCond[i] = rgcCond[i] + 0x1;
            }
        }
        t_scratch_m88_2 = (int32_t)((uint32_t)(score.rgcsh[2] & 0x1fff) << (score.rgcsh[2] >> 0xd << 0x1));
        if ((int32_t)t_scratch_m88_2 >= (int32_t)GetVCVal(&game, 6, 0)) {
            t_scratch_m86_6 = (vlprgScoreX[i].wWord | 0x800) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0x0;
            vlprgScoreX[i].wWord = vlprgScoreX[i].wWord | t_scratch_m86_6;
            if (GetVCCheck(&game, 6) != 0) {
                rgcCond[i] = rgcCond[i] + 0x1;
            }
        }
        if (rglScore[i] >= (int32_t)GetVCVal(&game, 3, 0)) {
            t_scratch_m86_7 = (vlprgScoreX[i].wWord | 0x100) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0x0;
            vlprgScoreX[i].wWord = vlprgScoreX[i].wWord | t_scratch_m86_7;
            if (GetVCCheck(&game, 3) != 0) {
                rgcCond[i] = rgcCond[i] + 0x1;
            }
        }
        c = 0;
        for (j = 0; j < 6; j++) {
            t_scratch_m86_8 = (int16_t)rgplr[i].rgTech[j];
            if (t_scratch_m86_8 >= GetVCVal(&game, 1, 0)) {
                c = c + 1;
            }
        }
        if (c >= GetVCVal(&game, 2, 0)) {
            t_scratch_m86_9 = (vlprgScoreX[i].wWord | 0x80) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0x0;
            vlprgScoreX[i].wWord = vlprgScoreX[i].wWord | t_scratch_m86_9;
            if (GetVCCheck(&game, 1) != 0) {
                rgcCond[i] = rgcCond[i] + 0x1;
            }
        }
        t_scratch_m88_6 = (int32_t)(score.cResources / 1000);
        if ((int32_t)t_scratch_m88_6 >= (int32_t)GetVCVal(&game, 5, 0)) {
            t_scratch_m86_10 = (vlprgScoreX[i].wWord | 0x400) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0x0;
            vlprgScoreX[i].wWord = vlprgScoreX[i].wWord | t_scratch_m86_10;
            if (GetVCCheck(&game, 5) != 0) {
                rgcCond[i] = rgcCond[i] + 0x1;
            }
        }
    }
    if (game.cPlayer != 1) {
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fDead != 0x0) {
                cDead = cDead + 1;
            }
            rgplr[i].wScore = 0x1;
            for (j = 0; j < game.cPlayer; j++) {
                if (rglScore[j] > rglScore[i]) {
                    rgplr[i].wScore = rgplr[i].wScore + 0x1;
                }
            }
            if (rgplr[i].wScore != 0x1) {
                if (rgplr[i].wScore == 0x2) {
                    lScore2nd = rglScore[i];
                }
            } else {
                iScoreMax = i;
                lScoreMax = rglScore[i];
                cFirst = cFirst + 1;
            }
        }
        if (cFirst > 1) {
            lScore2nd = lScoreMax;
        }
        for (i = 0; i < game.cPlayer; i++) {
            vlprgScoreX[i].turn = rgplr[i].wScore;
        }
        if (game.turn >= GetVCVal(&game, 7, 0) && cFirst == 1) {
            t_scratch_m86_12 = (vlprgScoreX[iScoreMax].wWord | 0x1000) & 0x3fc0;
            vlprgScoreX[iScoreMax].grbitVC = 0x0;
            vlprgScoreX[iScoreMax].wWord = vlprgScoreX[iScoreMax].wWord | t_scratch_m86_12;
            if (GetVCCheck(&game, 7) != 0) {
                rgcCond[iScoreMax] = rgcCond[iScoreMax] + 0x1;
            }
        }
        if (cDead + 1 < game.cPlayer) {
            if (lScoreMax >= (int32_t)((int32_t)(lScore2nd * (int32_t)(GetVCVal(&game, 4, 0) + 100)) / 0x64)) {
                t_scratch_m86_13 = (vlprgScoreX[iScoreMax].wWord | 0x200) & 0x3fc0;
                vlprgScoreX[iScoreMax].grbitVC = 0x0;
                vlprgScoreX[iScoreMax].wWord = vlprgScoreX[iScoreMax].wWord | t_scratch_m86_13;
                if (GetVCCheck(&game, 4) != 0) {
                    rgcCond[iScoreMax] = rgcCond[iScoreMax] + 0x1;
                }
            }
            if (game.turn >= (uint16_t)GetVCVal(&game, 9, 0)) {
                wWinners = 0x0;
                j = GetVCVal(&game, 8, 0);
                if (j >= 1) {
                    for (i = game.cPlayer - 1; i >= 0; i--) {
                        wWinners = wWinners * 0x2;
                        if (rgcCond[i] >= j) {
                            vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xbfff) | 0x4000;
                            wWinners = wWinners | 0x1;
                        }
                    }
                    if (wWinners != 0x0) {
                        gd.fGameOverMan = 0x1;
                    }
                }
                if (gd.fGameOverMan != 0x0) {
                    i = 0;
                    j = 1;
                    while (i < game.cPlayer) {
                        wWinners2 = wWinners;
                        if (rgplr[i].fDead == 0x0) {
                            if ((j & wWinners) == 0x0) {
                                imsg = 181;
                            } else if ((j ^ wWinners) == 0x0) {
                                imsg = 182;
                            } else {
                                imsg = 183;
                                wWinners2 = wWinners2 & ~j;
                            }
                        } else {
                            imsg = 184;
                        }
                        FSendPrependedPlrMsg(i, imsg, -4, wWinners2, 0, 0, 0, 0, 0, 0);
                        i = i + 1;
                        j = j * 2;
                    }
                }
            }
        } else {
            gd.fGameOverMan = 0x1;
            if (rgplr[iScoreMax].fDead == 0x0) {
                FSendPrependedPlrMsg(iScoreMax, 188, -4, 0, 0, 0, 0, 0, 0, 0);
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (i != iScoreMax) {
                    FSendPrependedPlrMsg(i, 184, -4, 0, 0, 0, 0, 0, 0, 0);
                }
            }
        }
    } else {
        vlprgScoreX->iRank = 1;
    }
    return;
}

void CreateBackupDir() {
    char *pchT;

    strcpy(szBackup, szBase);
    pchT = strrchr(szBackup, 92);
    if (pchT != 0x0) {
        pchT = pchT + 1;
    } else {
        pchT = szBackup;
    }
    *pchT = 0;
    if (vcBackupDirs > 1) {
        if (vcBackupDirs > 99) {
            _wsprintf(pchT, "backup.%03d", (uint32_t)game.turn % vcBackupDirs);
        } else {
            _wsprintf(pchT, "backup%d", (uint32_t)game.turn % vcBackupDirs);
        }
    } else {
        strcpy(pchT, "backup");
    }
    mkdir(szBackup);
    strcat(szBackup, "\\");
    return;
}

int16_t FPacketDecay(THING *lpth, int16_t pctRate) {
    uint16_t iRateMin;
    int16_t  iRate;
    int16_t  i;
    uint16_t wDecay;
    int32_t  lDecay;

    if (lpth->thp.iDecayRate > 0x0) {
        switch (lpth->thp.iDecayRate) {
        case 0x1:
            iRate = 10;
            break;
        case 0x2:
            iRate = 25;
            break;
        case 0x3:
            iRate = 50;
        default:
        }
        if (GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) != raMassAccel) {
            iRateMin = 0xa;
        } else {
            iRate = (int32_t)iRate / 2;
            iRateMin = 0x5;
        }
        lDecay = 0;
        for (i = 0; i < 3; i++) {
            if (lpth->thp.rgwtMin[i] != 0) {
                wDecay = LOWORD((int32_t)((int32_t)((uint32_t)((int32_t)lpth->thp.rgwtMin[i] * (int32_t)iRate) * (int32_t)pctRate) / 0x2710));
                wDecay = iRateMin <= wDecay ? wDecay : iRateMin;
                if (lpth->thp.rgwtMin[i] <= wDecay) {
                    wDecay = lpth->thp.rgwtMin[i];
                }
                lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] - wDecay;
                lDecay = lDecay + (int32_t)lpth->thp.rgwtMin[i];
            }
        }
        if (lDecay != 0) {
            lpth->thp.wtMax = LOWORD((int32_t)((lDecay + 9) / 0xa));
            return 0;
        }
        FreeLpth(lpth);
        return 1;
    }
    return 0;
}

void ThingDecay() {
    THING   *lpthMac;
    int32_t  pctDecay;
    int16_t  i;
    int16_t  ifl;
    FLEET   *lpfl;
    THING   *lpth;
    uint16_t wDecay;
    int32_t  lDecay;
    int16_t  fMineExpert;
    int32_t  dy;
    int32_t  dx;
    uint16_t t_merge_74cc_0001;
    int32_t  t_merge_75cf_0001;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        lpfl->fBombed = 0x0;
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith != ithMineralPacket) {
            if (lpth->ith == ithMinefield) {
                fMineExpert = GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMines ? 1 : 0;
                if (lpth->thm.fDetonate != 0x0) {
                    lDecay = lpth->thm.cMines;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0x0)
                            break;
                        if (lpfl->fDead == 0x0) {
                            dx = (int32_t)(lpfl->pt.x - lpth->pt.x);
                            dy = (int32_t)(lpfl->pt.y - lpth->pt.y);
                            if (lpfl->fBombed == 0x0 && (uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lDecay) {
                                FTravelThroughMineFields(lpfl, 0x0, lpth);
                                lpfl->fBombed = 0x1;
                            }
                        }
                    }
                }
                t_merge_74cc_0001 = fMineExpert == 0 ? 0x1 : 0x0;
                pctDecay = (int32_t)((t_merge_74cc_0001 * 0x3 + 0x1) * CPlanetsInCircle(lpth->pt, lpth->thm.cMines) + 0x2);
                if (pctDecay > 50) {
                    pctDecay = 50;
                }
                if (lpth->thm.fDetonate != 0x0) {
                    pctDecay = pctDecay + 25;
                }
                lDecay = (int32_t)((int32_t)(lpth->thm.cMines * pctDecay) / 100);
                if (lDecay < pctDecay) {
                    lDecay = pctDecay;
                }
                if (lpth->thm.iType != 0x2) {
                    t_merge_75cf_0001 = 10 <= lDecay ? lDecay : 10;
                    lDecay = t_merge_75cf_0001;
                }
                if (lDecay < lpth->thm.cMines) {
                    lpth->thm.cMines = lpth->thm.cMines - lDecay;
                } else {
                    FreeLpth(lpth);
                    lpth = lpth - 1;
                    lpthMac = lpthMac - 1;
                }
            }
        } else {
            if (lpth->thp.iWarp != 0x0) {
                if (FPacketDecay(lpth, 100) == 0)
                    continue;
            } else {
                if (lpth->thp.fMoved != 0x0) {
                    lpth->thp.fMoved = 0x0;
                    continue;
                }
                lDecay = 0;
                for (i = 0; i < 3; i++) {
                    if (lpth->thp.rgwtMin[i] != 0) {
                        wDecay = 0xa <= (int32_t)lpth->thp.rgwtMin[i] / 10 ? (int32_t)lpth->thp.rgwtMin[i] / 10 : 0xa;
                        lpth->thp.rgwtMin[i] = lpth->thp.rgwtMin[i] - wDecay;
                        if (lpth->thp.rgwtMin[i] < 0) {
                            lpth->thp.rgwtMin[i] = 0;
                        }
                        lDecay = lDecay + (int32_t)lpth->thp.rgwtMin[i];
                    }
                }
                if (lDecay != 0) {
                    lpth->thp.wtMax = LOWORD((int32_t)((lDecay + 9) / 0xa));
                    continue;
                }
                FreeLpth(lpth);
            }
            lpth = lpth - 1;
            lpthMac = lpthMac - 1;
        }
    }
    return;
}

void UnmarkMineFields() {
    THING *lpthMac;
    THING *lpth;

    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMinefield) {
            lpth->thm.grbitPlrNow = 0x0;
        }
    }
    return;
}

void SweepForMines() {
    int16_t  iplr;
    THING   *lpthMac;
    POINT16  pt;
    int32_t  dy;
    int32_t  lCur;
    PLANET  *lppl;
    int16_t  ifl;
    FLEET   *lpfl;
    THING   *lpth;
    int32_t  cMineCur;
    int32_t  dx;
    int32_t  cMine;
    uint16_t grbitPlr;
    PLANET  *lpplMac;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        cMine = CMineSweepFromLpfl(lpfl);
        if (cMine > 0 && lpfl->fDead == 0x0) {
            iplr = lpfl->iplr;
            grbitPlr = 0x1 << lpfl->iplr;
            pt = lpfl->pt;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (lpth->ith == ithMinefield && lpth->iplr != iplr && FAttackPlayer(lpfl, lpth->iplr) != 0) {
                    dx = (int32_t)(pt.x - lpth->pt.x);
                    dy = (int32_t)(pt.y - lpth->pt.y);
                    lCur = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                    if (lpth->thm.cMines >= (uint32_t)(dx * dx) + (uint32_t)(dy * dy)) {
                        if (lpth->thm.iType != 0x2) {
                            cMineCur = cMine;
                        } else {
                            cMineCur = (int32_t)(cMine / 3);
                        }
                        if (cMineCur < 2) {
                            cMineCur = 2;
                        }
                        if (lpth->thm.cMines - cMineCur < lCur - 1) {
                            cMineCur = lpth->thm.cMines - lCur + 1;
                        }
                        if (cMineCur > lpth->thm.cMines) {
                            cMineCur = lpth->thm.cMines;
                        }
                        FSendPlrMsg(lpfl->iPlayer, 194, 0x8000 | lpfl->id, lpfl->id, LOWORD(cMineCur), HIWORD(cMineCur), lpth->iplr, lpth->thm.iType,
                                    lpth->pt.x, lpth->pt.y);
                        FSendPlrMsg(lpth->iplr, 190, -6, lpth->idFull, LOWORD(cMineCur), HIWORD(cMineCur), lpth->thm.iType, lpth->pt.x, lpth->pt.y, 0);
                        lpth->thm.cMines = lpth->thm.cMines - cMineCur;
                        if (lpth->thm.cMines <= 0) {
                            FreeLpth(lpth);
                            lpth = lpth - 1;
                            lpthMac = lpthMac - 1;
                        } else {
                            lpth->thm.grbitPlr = lpth->thm.grbitPlr | 0x1 << lpfl->iPlayer;
                        }
                    }
                }
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->fStarbase != 0x0 && lppl->iPlayer != -1) {
            cMine = CMineSweepFromLphul(&rglpshdefSB[lppl->iPlayer][lppl->isb].hul);
            if (cMine > 0) {
                iplr = lppl->iPlayer;
                grbitPlr = 0x1 << lppl->iPlayer;
                pt = rgptPlan[lppl->id];
                lpth = lpThings;
                lpthMac = lpThings + cThing;
                for (; lpth < lpthMac; lpth++) {
                    if (lpth->ith == ithMinefield && lpth->iplr != iplr && iplr != lpth->iplr && (int16_t)rgplr[iplr].rgmdRelation[lpth->iplr] != 1) {
                        dx = (int32_t)(pt.x - lpth->pt.x);
                        dy = (int32_t)(pt.y - lpth->pt.y);
                        lCur = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                        if (lpth->thm.cMines >= (uint32_t)(dx * dx) + (uint32_t)(dy * dy)) {
                            if (lpth->thm.iType != 0x2) {
                                cMineCur = cMine;
                            } else {
                                cMineCur = (int32_t)(cMine / 3);
                            }
                            if (cMineCur < 2) {
                                cMineCur = 2;
                            }
                            if (lpth->thm.cMines - cMineCur < lCur - 1) {
                                cMineCur = lpth->thm.cMines - lCur + 1;
                            }
                            if (cMineCur > lpth->thm.cMines) {
                                cMineCur = lpth->thm.cMines;
                            }
                            FSendPlrMsg(iplr, 244, lppl->id, lppl->id, LOWORD(cMineCur), HIWORD(cMineCur), lpth->iplr, lpth->thm.iType, lpth->pt.x, lpth->pt.y);
                            FSendPlrMsg(lpth->iplr, 190, -6, lpth->idFull, LOWORD(cMineCur), HIWORD(cMineCur), lpth->thm.iType, lpth->pt.x, lpth->pt.y, 0);
                            lpth->thm.cMines = lpth->thm.cMines - cMineCur;
                            if (lpth->thm.cMines <= 0) {
                                FreeLpth(lpth);
                                lpth = lpth - 1;
                                lpthMac = lpthMac - 1;
                            } else {
                                lpth->thm.grbitPlr = lpth->thm.grbitPlr | 0x1 << lppl->iPlayer;
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

void BreedColonistsInTransit() {
    int16_t       fNoBreeders;
    char          grfBreeder[16];
    int32_t       lColGain;
    PLANET       *lppl;
    int16_t       ifl;
    FLEET        *lpfl;
    int16_t       i;
    int32_t       lColGainAct;
    RaceAttribute t_call_7e83;
    uint16_t      t_merge_7e9a_0001;
    PLANET       *t_call_8058;

    fNoBreeders = 1;
    for (i = 0; i < game.cPlayer; i++) {
        t_call_7e83 = GetRaceStat(&rgplr[i], rsMajorAdv);
        t_merge_7e9a_0001 =
            t_call_7e83 == raDefend ? ((uint16_t)t_call_7e83 & 0xff00) | ((uint16_t)0x1 & 0xff) : ((uint16_t)t_call_7e83 & 0xff00) | ((uint16_t)0x0 & 0xff);
        grfBreeder[i] = LOBYTE(t_merge_7e9a_0001);
        if ((int16_t)LOBYTE(t_merge_7e9a_0001) == 0x1) {
            fNoBreeders = 0;
        }
    }
    if (fNoBreeders == 0) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0x0)
                break;
            if (lpfl->fDead == 0x0 && (int16_t)grfBreeder[lpfl->iPlayer] != 0 && lpfl->rgwtMin[3] != 0) {
                lColGain = (int32_t)((int32_t)(lpfl->rgwtMin[3] * (int32_t)(int16_t)rgplr[lpfl->iPlayer].pctIdealGrowth) / 0xc8);
                if (lColGain <= 0) {
                    if (Random(3) != 0)
                        continue;
                    lColGain = 1;
                }
                lColGainAct = ChgCargo(grobjFleet, lpfl->id, 3, lColGain, 0x0);
                if (lColGainAct > 0) {
                    FSendPlrMsg2(lpfl->iPlayer, 251, lpfl->id | 0x8000, lpfl->id, LOWORD(lColGainAct));
                }
                if (lColGainAct < lColGain && lpfl->idPlanet != -1) {
                    t_call_8058 = LpplFromId(lpfl->idPlanet);
                    lppl = t_call_8058;
                    if (t_call_8058 != 0x0 && lppl->iPlayer == lpfl->iPlayer) {
                        lColGain = lColGain - lColGainAct;
                        lppl->rgwtMin[3] = lppl->rgwtMin[3] + lColGain;
                        FSendPlrMsg(lpfl->iPlayer, 344, lpfl->id | 0x8000, lpfl->id, LOWORD(lColGain), HIWORD(lColGain), lpfl->idPlanet, 0, 0, 0);
                    }
                }
            }
        }
    }
    return;
}

void UpdateResearchStatus(int16_t fUsePool) {
    int16_t   mdAvail;
    int16_t   fRedoItAll;
    int16_t   iTechCur;
    int16_t   fUsePoolOrig;
    int16_t   iTechNext;
    int16_t   iT;
    int16_t   iItem;
    int16_t   fGeneral;
    int16_t   fChgNow;
    int16_t   i;
    int16_t   ibitCur;
    int32_t   rglFieldSpent[6];
    int16_t   grbitCur;
    int16_t   cPlrAlive;
    int32_t   lSpent;
    PART      part;
    int32_t   l;
    int16_t   iTT;
    int32_t   l15pct;
    int16_t   iTechNext2;
    char      TechLevel;
    int16_t   jj;
    int16_t   iGoto;
    MessageId idm;

    cPlrAlive = 0;
    fUsePoolOrig = fUsePool;
    if (fUsePool != 0) {
        for (i = 0; i < 6; i++) {
            rglFieldSpent[i] = 0;
        }
    }
    for (i = 0; i < game.cPlayer; i++) {
        fUsePool = fUsePoolOrig;
        fGeneral = GetRaceGrbit(&rgplr[i], ibitRaceGeneralizedResearch);
        iTechCur = (int16_t)rgplr[i].iTechCur & 0xf;
        iTechNext = (int16_t)(rgplr[i].iTechCur >> 0x4);
        idPlayer = i;
        if (rgplr[i].fDead == 0x0) {
            cPlrAlive = cPlrAlive + 1;
        }
        do {
            fRedoItAll = 0;
            for (iT = 0; iT < 6; iT++) {
                lSpent = rgplr[i].rgResSpent[iT];
                fChgNow = 0;
                if (game.fSlowTech != 0x0) {
                    lSpent = (int32_t)(lSpent * 2);
                }
                if (iT == iTechCur && fUsePool != 0 && fGeneral < 2) {
                    if (fGeneral == 0) {
                        lSpent = lSpent + rgplr[i].lResLastYear;
                        rglFieldSpent[iT] = rglFieldSpent[iT] + rgplr[i].lResLastYear;
                    } else {
                        fRedoItAll = 1;
                        fGeneral = 2;
                        lSpent = lSpent + (int32_t)((rgplr[i].lResLastYear + 1) / 0x2);
                        rglFieldSpent[iT] = rglFieldSpent[iT] + (int32_t)((rgplr[i].lResLastYear + 1) / 0x2);
                        for (iTT = 0; iTT < 6; iTT++) {
                            if (iTT != iT) {
                                l15pct = (int32_t)((int32_t)((uint32_t)(rgplr[i].lResLastYear * 3) + 19) / 0x14);
                                if (game.fSlowTech == 0x0) {
                                    rgplr[i].rgResSpent[iTT] = rgplr[i].rgResSpent[iTT] + l15pct;
                                } else {
                                    rgplr[i].rgResSpent[iTT] = rgplr[i].rgResSpent[iTT] + (int32_t)(l15pct / 2);
                                }
                                rglFieldSpent[iTT] = rglFieldSpent[iTT] + l15pct;
                            }
                        }
                    }
                }
                do {
                    if ((int16_t)rgplr[i].rgTech[iT] >= 26 || (rgplr[i].fCrippled != 0x0 && (int16_t)rgplr[i].rgTech[iT] >= 10) ||
                        (rgplr[i].fCheater != 0x0 && (int16_t)rgplr[i].rgTech[iT] >= 10))
                        goto L_89f3;
                    l = GetTechLevelCost(iT, (int16_t)rgplr[i].rgTech[iT] + 1, i);
                    if (l > lSpent || ((int16_t)rgplr[i].rgTech[iT] >= 26 && (rgplr[i].fCrippled != 0x0 || (int16_t)rgplr[i].rgTech[iT] >= 26)))
                        goto L_89a5;
                    iTechNext2 = iTechCur;
                    lSpent = lSpent - l;
                    rgplr[i].rgTech[iT] = rgplr[i].rgTech[iT] + 1;
                    TechLevel = rgplr[i].rgTech[iT];
                    if ((int16_t)TechLevel == 26 && iTechNext == 6) {
                        iTechNext = 7;
                    }
                    if (iTechCur == iT && iTechNext != 6) {
                        if (iTechNext == 7) {
                            iTechNext2 = 0;
                            for (jj = 1; jj < 6; jj++) {
                                if (rgplr[i].rgTech[jj] < LOBYTE((int16_t)(((uint16_t)iTechNext2 & 0xff00) | ((uint16_t)rgplr[i].rgTech[iTechNext2] & 0xff)))) {
                                    iTechNext2 = jj;
                                }
                            }
                        } else {
                            iTechNext2 = iTechNext;
                        }
                        fChgNow = 1;
                    }
                    FSendPlrMsg(i, fGeneral == 0 ? 80 : 310, -2, (int16_t)TechLevel, iT, iTechNext2, 0, 0, 0, 0);
                    grbitCur = 1;
                    ibitCur = 0;
                    while (grbitCur != 0) {
                        if ((grbitCur & 0xffff) != 0x0) {
                            iItem = 0;
                            part.hs.grhst = grbitCur;
                            while (1) {
                                part.hs.iItem = iItem;
                                mdAvail = FLookupPart(&part);
                                if (mdAvail == 0)
                                    break;
                                if (mdAvail == 1 &&
                                    rgplr[i].rgTech[iT] == LOBYTE((int16_t)(((uint16_t)iT & 0xff00) | ((uint16_t)part.pcom->rgTech[iT] & 0xff)))) {
                                    switch (grbitCur) {
                                    case 1024:
                                        idm = idmRecentBreakthroughHasAlsoGivenHullDesign;
                                        iGoto = -3;
                                        goto L_8798;
                                    case 16384:
                                        idm = idmRecentBreakthroughHasAlsoGivenHullType;
                                        iGoto = -3;
                                        goto L_8798;
                                    case 8192:
                                        if (GetRaceGrbit(&rgplr[i], ibitRaceTT) != 0) {
                                            switch (iItem) {
                                            case 8:
                                            case 12:
                                            case 16:
                                                iItem = iItem + 1;
                                                break;
                                            default:
                                                goto L_873a;
                                            }
                                            break;
                                        }
                                    default:
                                    L_873a:
                                        if (grbitCur != -32768 || iItem < 9 || iItem > 13) {
                                            if (grbitCur != -32768 || iItem < 0 || iItem > 8) {
                                                idm = idmRecentBreakthroughHasAlsoGivenBenefit;
                                            } else {
                                                idm = idmRecentBreakthroughHasAlsoTaughtHowBuild2;
                                            }
                                        } else {
                                            idm = idmRecentBreakthroughHasAlsoTaughtHowBuild;
                                        }
                                        iGoto = ibitCur << 0x8 | 0xc000 | iItem;
                                        goto L_8798;
                                    }
                                    continue;
                                L_8798:
                                    FSendPlrMsg(i, idm, iGoto, iT, grbitCur, iItem, 0, 0, 0, 0);
                                }
                                iItem = iItem + 1;
                            }
                        }
                        grbitCur = grbitCur * 2;
                        ibitCur = ibitCur + 1;
                    }
                } while ((fUsePool == 0 && fChgNow == 0 && iTechNext != 7) || iTechNext == 6 || iT != iTechCur);
                if (iTechNext != 7) {
                    rgplr[idPlayer].iTechCur =
                        LOBYTE(((uint16_t)(((uint16_t)(((uint16_t)(192 * idPlayer) & 0xff00) | ((uint16_t)rgplr[idPlayer].iTechCur & 0xff)) & 0xff00) |
                                           ((uint16_t)(rgplr[idPlayer].iTechCur & 0xf) & 0xff)) &
                                0xff00) |
                               ((uint16_t)((rgplr[idPlayer].iTechCur & 0xf) | 0x60) & 0xff));
                    rgplr[idPlayer].iTechCur =
                        LOBYTE(((uint16_t)(((uint16_t)(((uint16_t)(192 * idPlayer) & 0xff00) | ((uint16_t)rgplr[idPlayer].iTechCur & 0xff)) & 0xff00) |
                                           ((uint16_t)(rgplr[idPlayer].iTechCur & 0xf0) & 0xff)) &
                                0xff00) |
                               ((uint16_t)((rgplr[idPlayer].iTechCur & 0xf0) | LOBYTE(iTechNext)) & 0xff));
                    rgplr[i].rgResSpent[iT] = 0x0;
                    iTechCur = iTechNext;
                    iTechNext = 6;
                } else {
                    iTechNext = 0;
                    for (jj = 1; jj < 6; jj++) {
                        if (rgplr[i].rgTech[jj] < LOBYTE((int16_t)(((uint16_t)iTechNext & 0xff00) | ((uint16_t)rgplr[i].rgTech[iTechNext] & 0xff)))) {
                            iTechNext = jj;
                        }
                    }
                    rgplr[idPlayer].iTechCur =
                        LOBYTE(((uint16_t)(((uint16_t)(((uint16_t)(192 * idPlayer) & 0xff00) | ((uint16_t)rgplr[idPlayer].iTechCur & 0xff)) & 0xff00) |
                                           ((uint16_t)(rgplr[idPlayer].iTechCur & 0xf0) & 0xff)) &
                                0xff00) |
                               ((uint16_t)((rgplr[idPlayer].iTechCur & 0xf0) | LOBYTE(iTechNext)) & 0xff));
                    rgplr[i].rgResSpent[iT] = 0x0;
                    iTechCur = iTechNext;
                    iTechNext = 7;
                }
                if (game.fSlowTech != 0x0) {
                    lSpent = (int32_t)((lSpent + 0x1) >> 0x1);
                }
                rgplr[i].rgResSpent[iTechCur] = rgplr[i].rgResSpent[iTechCur] + lSpent;
                fUsePool = 0;
                fRedoItAll = 1;
                goto L_89f3;
            L_89a5:
                if (game.fSlowTech != 0x0) {
                    lSpent = (int32_t)((lSpent + 0x1) >> 0x1);
                }
                rgplr[i].rgResSpent[iT] = lSpent;
            L_89f3:;
            }
        } while (fRedoItAll != 0);
    }
    idPlayer = -1;
    fRedoItAll = 0;
    if (fUsePoolOrig != 0 && cPlrAlive > 1) {
        for (i = 0; i < game.cPlayer; i++) {
            if (GetRaceStat(&rgplr[i], rsMajorAdv) == raStealth) {
                for (iT = 0; iT < 6; iT++) {
                    if (rglFieldSpent[iT] > 0) {
                        lSpent = (int32_t)((int32_t)(rglFieldSpent[iT] / (int32_t)cPlrAlive) / 2);
                        if (lSpent > 1) {
                            fRedoItAll = 1;
                            FSendPlrMsg2(i, 345, -2, iT, LOWORD(lSpent));
                            if (game.fSlowTech != 0x0) {
                                lSpent = (int32_t)((lSpent + 0x1) >> 0x1);
                            }
                            rgplr[i].rgResSpent[iT] = rgplr[i].rgResSpent[iT] + lSpent;
                        }
                    }
                }
            }
        }
        if (fRedoItAll != 0) {
            UpdateResearchStatus(0);
        }
    }
    return;
}

int16_t IBestRemoteTerra(PLANET *lppl, int16_t iplr, int16_t fHelp) {
    int16_t iBest;
    int16_t i;
    PLAYER  plrSav;

    plrSav = rgplr[lppl->iPlayer];
    rgplr[lppl->iPlayer] = rgplr[iplr];
    for (i = 0; i < 3; i++) {
        rgplr[lppl->iPlayer].rgEnvVar[i] = LOBYTE((int16_t)(((uint16_t)i & 0xff00) | ((uint16_t)plrSav.rgEnvVar[i] & 0xff)));
        rgplr[lppl->iPlayer].rgEnvVarMin[i] = LOBYTE((int16_t)(((uint16_t)i & 0xff00) | ((uint16_t)plrSav.rgEnvVarMin[i] & 0xff)));
        rgplr[lppl->iPlayer].rgEnvVarMax[i] = LOBYTE((int16_t)(((uint16_t)i & 0xff00) | ((uint16_t)plrSav.rgEnvVarMax[i] & 0xff)));
    }
    iBest = IBestTerraform(lppl, fHelp);
    rgplr[lppl->iPlayer] = plrSav;
    return iBest;
}
