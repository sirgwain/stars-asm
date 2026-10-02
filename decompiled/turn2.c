#include "common.h"

void Produce() {
    int32_t    lResCur;
    int16_t    cMax;
    int32_t    rgResAvail[4];
    int16_t    iprodCur;
    mdProdStat mdStatus;
    int16_t    cBuilt;
    int16_t    fNoResearch;
    PLANET    *lppl;
    int16_t    i;
    MessageId  idm;
    PROD       prodPartial;
    int16_t    fPrevProdIsAlch;
    int16_t    fAutoBuildDone;
    int32_t    lResearchTake;
    PROD      *lpprod;
    PLANET    *lpplMac;
    int16_t    cMax2;

    MineMinerals();
    for (i = 0; i < game.cPlayer; i++) {
        rgplr[i].lResLastYear = 0;
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->lpplprod == 0) {
            if (lppl->iPlayer != -1) {
                FSendPlrMsg2(lppl->iPlayer, idmProductionQueueEmpty, lppl->id, lppl->id, 0);
                lResCur = CResourcesAtPlanet(lppl, lppl->iPlayer);
                if (lResCur != 0 && vrgPlanResExtra[lppl->id] != 0) {
                    lResCur += (int32_t)(lResCur * (uint32_t)vrgPlanResExtra[lppl->id]) / (int32_t)((uint32_t)vrgPlanResExtra[lppl->id] + lResCur);
                }
                rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + lResCur;
            }
        } else if (lppl->iPlayer != -1 && lppl->lpplprod->iprodMac != 0) {
            fNoResearch = lppl->fNoResearch;
            for (i = 0; i < 3; i++) {
                rgResAvail[i] = lppl->rgwtMin[i];
            }
            lResCur = CResourcesAtPlanet(lppl, lppl->iPlayer);
            if (lResCur != 0 && vrgPlanResExtra[lppl->id] != 0) {
                lResCur += (int32_t)(lResCur * (uint32_t)vrgPlanResExtra[lppl->id]) / (int32_t)((uint32_t)vrgPlanResExtra[lppl->id] + lResCur);
            }
            rgResAvail[3] = lResCur;
            if (rgplr[lppl->iPlayer].fCheater != 0) {
                rgResAvail[3] = (int32_t)(rgResAvail[3] * 4) / 5;
            }
            if (rgResAvail[3] != 0) {
                if (fNoResearch != 0) {
                    lResearchTake = 0;
                } else {
                    lResearchTake = (int32_t)(rgResAvail[3] * (int16_t)rgplr[lppl->iPlayer].pctResearch) / 100;
                    rgResAvail[3] -= lResearchTake;
                    rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + lResearchTake;
                }
                fAutoBuildDone = 1;
                while (1) {
                    fPrevProdIsAlch = 0;
                    iprodCur = 0;
                    while (1) {
                        if (lppl->lpplprod == 0 || iprodCur >= lppl->lpplprod->iprodMac)
                            goto L_0b9a;
                        lpprod = &lppl->lpplprod->rgprod[iprodCur];
                        if (lpprod->cItem > 0) {
                            if (lpprod->grobj == grobjPlanet) {
                                if ((lpprod->iItem >= iobjPlanetaryScannerFirst && lpprod->iItem <= iobjPlanetaryScannerSnooper620X) ||
                                    lpprod->iItem == iobjPlanetaryScanner) {
                                    if (lppl->iScanner != 31) {
                                        FSendPlrMsg2(lppl->iPlayer, idmOrderBuildScannerCanceledAlreadyHaveScanner, lppl->id, lppl->id, 0);
                                        goto RemoveFromQueue;
                                    }
                                } else if (lpprod->iItem >= iobjPacketIron && lpprod->iItem <= iobjPacketMixed) {
                                    if (IWarpMAFromLppl(lppl, NULL) == 0 || lppl->idFling == 0) {
                                        FSendPlrMsg2(lppl->iPlayer, idmHasOrdersBuildMineralPacketEitherDoesnt, lppl->id, lppl->id, 0);
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
                                        cMax -= lppl->cFactories;
                                        goto LCantBuildP;
                                    case mdIdleMine:
                                        cMax = CMaxMines(lppl, lppl->iPlayer);
                                        cMax2 = CMaxOperableMines(lppl, lppl->iPlayer, 1);
                                        if (cMax2 > cMax) {
                                            cMax = cMax2;
                                        }
                                        cMax -= lppl->cMines;
                                        goto LCantBuildP;
                                    case mdIdleDefense:
                                        cMax = CMaxDefenses(lppl, lppl->iPlayer);
                                        cMax2 = CMaxOperableDefenses(lppl, lppl->iPlayer, 1);
                                        if (cMax2 > cMax) {
                                            cMax = cMax2;
                                        }
                                        cMax -= lppl->cDefenses;
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
                                    if (cMax < (int32_t)lpprod->cItem) {
                                        FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
                                        if (cMax <= 0)
                                            goto RemoveFromQueue;
                                        lpprod->cItem = cMax;
                                    }
                                }
                            }
                        L_07fc:
                            if (lpprod->iItem == iobjAlchemy && lpprod->grobj == grobjPlanet && iprodCur < lppl->lpplprod->iprodMac - 1) {
                                fPrevProdIsAlch = 1;
                                iprodCur++;
                                continue;
                            }
                            prodPartial.cItem = 0;
                            cBuilt = CBuildProdItem(lppl, lpprod, &prodPartial, rgResAvail, fPrevProdIsAlch, (int16_t *)&mdStatus, 0);
                            if (fAutoBuildDone != 0 && (mdStatus == mdProdStatSomeAuto || mdStatus == mdProdStatNoneAuto)) {
                                fAutoBuildDone = 0;
                            }
                            if (cBuilt > 0 && FBuildObject(lppl, lpprod->grobj, lpprod->iItem, cBuilt, rgResAvail) == 0 &&
                                (lpprod->grobj != grobjPlanet || lpprod->iItem >= mdIdleFactory)) {
                                lpprod->cItem = 0;
                            }
                            if (lppl->iPlayer == -1 && lppl->lpplprod == 0)
                                break;
                            if (mdStatus != mdProdStatComplete) {
                                if ((int16_t)mdStatus < mdProdStatSome)
                                    goto L_0b8e;
                                if (prodPartial.cItem <= 0)
                                    goto L_0b9a;
                                if (lppl->lpplprod->iprodMac == lppl->lpplprod->iprodMax) {
                                    lppl->lpplprod = (PLPROD *)LpplReAlloc((PL *)lppl->lpplprod, lppl->lpplprod->iprodMac + 1);
                                }
                                fmemmove(&lppl->lpplprod->rgprod[1], lppl->lpplprod->rgprod, lppl->lpplprod->iprodMac * sizeof(PROD));
                                lppl->lpplprod->rgprod[0] = prodPartial;
                                lppl->lpplprod->iprodMac++;
                                goto L_0b9a;
                            }
                        }
                    RemoveFromQueue:
                        if (lppl->lpplprod->iprodMac == fPrevProdIsAlch + 1)
                            goto L_09eb;
                        if (iprodCur < lppl->lpplprod->iprodMac - 1) {
                            fmemmove(lppl->lpplprod + (1 + (iprodCur - fPrevProdIsAlch)), lppl->lpplprod + (1 + (iprodCur + 1)),
                                     (lppl->lpplprod->iprodMac - iprodCur - 1) * 4);
                        }
                        lppl->lpplprod->iprodMac -= LOBYTE(fPrevProdIsAlch + 1);
                        iprodCur -= fPrevProdIsAlch + 1;
                    L_0b8e:
                        iprodCur++;
                        fPrevProdIsAlch = 0;
                    }
                }
            L_09eb:
                FreePl((PL *)lppl->lpplprod);
                lppl->lpplprod = NULL;
            L_0b9a:
                if (lppl->lpplprod == 0 || (iprodCur >= lppl->lpplprod->iprodMac && fAutoBuildDone != 0)) {
                    FSendPlrMsg2(lppl->iPlayer, idmHasCompletedOrdersProductionQueueEmpty, lppl->id, lppl->id, 0);
                }
                for (i = 0; i < 3; i++) {
                    lppl->rgwtMin[i] = rgResAvail[i];
                }
                rgplr[lppl->iPlayer].lResLastYear = rgplr[lppl->iPlayer].lResLastYear + rgResAvail[3];
            }
        }
    }
    UpdatePopulations();
    UpdateResearchStatus(1);
    if (game.fNoRandom == 0) {
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
    uint16_t t_scratch_m56;
    uint16_t t_scratch_m56_2;
    uint16_t t_scratch_m56_3;

    cAlchemy = 0;
    pctInitial = lpprod->pct;
    prod = *lpprod;
    GetProductionCosts(lppl, lpprod, rgCost, lppl->iPlayer, 1);
    cBuilt = 0;
    fAutoBuild = prod.grobj == grobjPlanet && prod.iItem < mdIdleFactory;
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
            if (IWarpMAFromLppl(lppl, NULL) == 0 || lppl->idFling == 0) {
                cMax = 0;
            }
        }
        if (cMax < 0) {
            cMax = 0;
        }
        if ((uint32_t)prod.cItem > (uint32_t)cMax || prod.iItem == iobjAlchemy) {
            prod.cItem = cMax;
        }
    }
    for (i = 0; i < 4; i++) {
        rgCostPaid[i] = (uint32_t)((uint32_t)(rgCost[i] * prod.pct) / 100);
    }
    while (prod.cItem > 0) {
        for (i = 0; i < 4 && rgCost[i] - rgCostPaid[i] <= rgRes[i]; i++) {
        }
        if (i < 4) {
            fMineralBlocked = 0;
            fResourceBlocked = 0;
            pct = 100;
            for (i = 0; i < 4; i++) {
                if (rgCost[i] > 0) {
                    if (rgRes[i] >= rgCost[i]) {
                        pctT = 100;
                    } else {
                        pctT = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i]) * 100) / rgCost[i]);
                        pctTooBig = (int32_t)((int32_t)((rgRes[i] + rgCostPaid[i] + 1) * 100) / rgCost[i]);
                        pctT = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
                    }
                    if (pctT < pct) {
                        lMinNeeded = rgCost[i] - rgCostPaid[i] - rgRes[i];
                        pct = pctT;
                        if (i == 3) {
                            fResourceBlocked = 1;
                        } else {
                            fMineralBlocked = 1;
                        }
                    }
                }
            }
            if (fMineralBlocked != 0 && fAutoBuild != 0) {
                if (fAlchemy == 0) {
                    fAutoBuild = 2;
                    break;
                }
            } else {
                for (i = 0; i < 4; i++) {
                    AddCost = (int32_t)(rgCost[i] * pct) / 100 - rgCostPaid[i];
                    rgRes[i] -= AddCost;
                    rgCostPaid[i] += AddCost;
                }
                prod.pct = LOWORD(pct);
                if (fAlchemy == 0 || fResourceBlocked != 0)
                    break;
            }
            lAlchCost = (uint32_t)(GetRaceGrbit(&rgplr[lppl->iPlayer], ibitRaceMineralAlchemy) == 0 ? 100 : 25);
            cCanBuild = (int32_t)(rgRes[3] / lAlchCost);
            if (cCanBuild > lMinNeeded) {
                cCanBuild = lMinNeeded;
            }
            if (cCanBuild > 0) {
                for (i = 0; i < 3; i++) {
                    rgRes[i] += cCanBuild;
                }
                rgRes[i] -= (uint32_t)(lAlchCost * cCanBuild);
                cAlchemy += LOWORD(cCanBuild);
            }
            if (cCanBuild != lMinNeeded)
                goto L_14de;
        } else {
            cBuilt++;
            prod.cItem--;
            prod.pct = 0;
            for (i = 0; i < 4; i++) {
                rgRes[i] -= rgCost[i] - rgCostPaid[i];
                rgCostPaid[i] = 0;
            }
        }
    }
    goto L_1712;
L_14de:
    if (rgRes[3] > 0 && pprodPartial != 0) {
        memset(pprodPartial, 0, sizeof(PROD));
        pprodPartial->grobj = grobjPlanet;
        pprodPartial->iItem = mdIdleAlchemy;
        pprodPartial->cItem = 1;
        pctT = (int32_t)((int32_t)(rgRes[3] * 100) / lAlchCost);
        pctTooBig = (int32_t)((int32_t)((rgRes[3] + 1) * 100) / lAlchCost);
        pctT = pctT <= pctTooBig - 1 ? pctTooBig - 1 : pctT;
        pprodPartial->pct = LOWORD(pctT);
        rgRes[3] -= (int32_t)(pctT * lAlchCost) / 100;
    }
L_1712:
    if (cBuilt > 0 && prod.grobj == grobjPlanet && (prod.iItem == mdIdleAlchemy || prod.iItem == iobjAlchemy)) {
        cAlchemy += cBuilt;
        for (i = 0; i < 3; i++) {
            rgRes[i] += cBuilt;
        }
    }
    if (cAlchemy != 0 && fCalcOnly == 0 && gd.fGeneratingTurn != 0) {
        FSendPlrMsg2(lppl->iPlayer, idmScientistsHaveTransmutedCommonMaterialsKtEach, lppl->id, lppl->id, cAlchemy);
    }
    if (pmdStatus != 0) {
        if (fAutoBuild == 2) {
            *pmdStatus = cBuilt <= 0 ? 4 : 3;
        } else if (fAutoBuild != 0 && prod.cItem == 0) {
            *pmdStatus = cBuilt <= 0 ? 2 : 1;
        } else if (cBuilt == 0) {
            *pmdStatus = pctInitial == prod.pct ? 7 : 6;
        } else if (prod.cItem == 0) {
            *pmdStatus = 0;
        } else {
            *pmdStatus = 5;
        }
    }
    if (fCalcOnly == 0 && fAutoBuild == 0) {
        *lpprod = prod;
    }
    if (fAutoBuild != 0 && pprodPartial != 0 && pprodPartial->cItem == 0 && prod.pct > 0) {
        *pprodPartial = prod;
        pprodPartial->cItem = 1;
        pprodPartial->iItem = LOWORD(iobjOther);
    }
    return cBuilt;
}

int16_t FBuildObject(PLANET *lppl, GrobjClass grobj, int16_t iItem, int16_t cBuilt, int32_t *rgMinerals) {
    int16_t       iWarp;
    int16_t       i;
    FLEET        *lpfl;
    MessageId     idm;
    int16_t       fTwoMAs;
    SHDEF        *lpshdef;
    int16_t       cAllowed;
    int32_t       dpOrig;
    int16_t       cshDamaged;
    int16_t       cshOrig;
    uint16_t      dpShdef;
    THING        *lpthMac;
    PacketDecay   iDecayRate;
    THING        *lpth;
    RaceAttribute raMajor;
    int16_t       iWarpAsked;
    int16_t       cSize;
    int16_t       rgwt[3];
    int32_t       l;
    EnvType       iEnv;
    PART          part;
    uint16_t      t_scratch_m16_3;
    uint16_t      t_scratch_m16_4;
    uint16_t      t_scratch_m16_5;
    int16_t       t_scratch_m16_6;
    int16_t       t_call_2d26;
    int16_t       t_scratch_m16_7;

    if (grobj == grobjFleet) {
        if (iItem >= 16) {
            iItem -= 16;
            lpshdef = rglpshdefSB[lppl->iPlayer] + iItem;
            if (lpshdef->fFree != 0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
                return 0;
            }
            idm = idmHasBuiltNew;
            if (lpshdef->hul.wtCargoMax != 0) {
                idm++;
                if ((uint32_t)lpshdef->hul.wtCargoMax == 0xffff) {
                    idm++;
                }
            }
            FSendPlrMsg(lppl->iPlayer, idm, lppl->id, lppl->id, lppl->iPlayer << 5 | iItem + 0x10, LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax, 0, 0,
                        0, 0);
            if (lppl->fStarbase != 0 && (int16_t)rglpshdefSB[lppl->iPlayer][lppl->isb].hul.ihuldef > (int16_t)rglpshdefSB[lppl->iPlayer][iItem].hul.ihuldef) {
                KillQueuedShips(lppl);
            }
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (lppl->fStarbase != 0) {
                rglpshdefSB[lppl->iPlayer][lppl->isb].cExist = rglpshdefSB[lppl->iPlayer][lppl->isb].cExist - 1;
            } else {
                lppl->fStarbase = 1;
            }
            lppl->isb = iItem;
            if (iWarp <= 0) {
                iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
                if (iWarp > 0) {
                    lppl->iWarpFling = iWarp + fTwoMAs - 4;
                } else {
                    lppl->iWarpFling = 0;
                    lppl->idFling = 0;
                    KillQueuedMassPackets(lppl);
                }
            }
            lpshdef->cBuilt++;
            lpshdef->cExist++;
            return 1;
        }
        if (lppl->fStarbase == 0 || iItem >= 16) {
            return 0;
        }
        lpshdef = rglpshdef[lppl->iPlayer] + iItem;
        if (lpshdef->fFree != 0 || FCanBuildShdef(lpshdef, lppl->iPlayer) == 0) {
            FSendPlrMsg2(lppl->iPlayer, idmStarbaseFailedBuildNewShipTypeBecause, lppl->id, iItem + 1, 0);
            return 0;
        }
        if (rgplr[lppl->iPlayer].cFleet == 0x200) {
            for (i = 0; i < cFleet; i++) {
                lpfl = rglpfl[i];
                if (rglpfl[i] == 0 || lpfl->iPlayer > lppl->iPlayer)
                    break;
                if (lpfl->iPlayer >= lppl->iPlayer && lpfl->lpplord->rgord[0].pt.x == rgptPlan[lppl->id].x &&
                    lpfl->lpplord->rgord[0].pt.y == rgptPlan[lppl->id].y && 32766 - cBuilt > lpfl->rgcsh[iItem]) {
                    if (lpfl->rgcsh[iItem] != 0 && lpfl->rgdv[iItem].pctDp != 0) {
                        dpShdef = rglpshdef[lpfl->iPlayer][iItem].hul.dp;
                        cshOrig = lpfl->rgcsh[iItem];
                        cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * cshOrig) / 100);
                        if (cshDamaged == 0) {
                            cshDamaged = 1;
                        }
                        dpOrig = (int32_t)((int32_t)((uint32_t)dpShdef * lpfl->rgdv[iItem].pctDp) / 10 * cshDamaged) / 50;
                        lpfl->rgdv[iItem].pctSh = LOWORD((int32_t)(cshDamaged * 100) / (int16_t)(cshOrig + cBuilt));
                        if (lpfl->rgdv[iItem].pctSh == 0) {
                            lpfl->rgdv[iItem].pctSh = 1;
                        }
                        cshDamaged = LOWORD((int32_t)(lpfl->rgdv[iItem].pctSh * (int16_t)(cshOrig + cBuilt)) / 100);
                        if (cshDamaged == 0) {
                            cshDamaged = 1;
                        }
                        lpfl->rgdv[iItem].pctDp = LOWORD((int32_t)((int32_t)(dpOrig * 5) / cshDamaged) * 100 / (int32_t)dpShdef);
                    } else {
                        lpfl->rgdv[iItem].dp = 0;
                    }
                    CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
                    FSendPlrMsg(lppl->iPlayer, idmStarbaseBuiltNewSDueLack27b, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lpfl->id, 0, 0,
                                0);
                    return 1;
                }
            }
            FSendPlrMsg(lppl->iPlayer, idmStarbaseBuiltNewShipSTypeLost, lppl->id, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);
            return 0;
        }
        lpfl = LpflNew(lppl->iPlayer, lppl->id);
        CreateShip(lppl->iPlayer, lpfl, iItem, cBuilt);
        lpfl->rgwtMin[4] = LGetFleetStat(lpfl, 1);
        if (lppl->idRoute != 0) {
            AutoRouteFleet(lpfl, lppl);
            if (cBuilt == 1) {
                idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewWhichWillRouted : idmStarbaseHasBuiltNewWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0, 0);
            } else {
                idm = lpfl->lpplord->rgord[1].iWarp == 0 ? idmStarbaseHasBuiltNewShipsWhichWill : idmStarbaseHasBuiltNewShipsWhichRouted;
                FSendPlrMsg(lppl->iPlayer, idm, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, lppl->idRoute - 1, 0, 0, 0);
            }
        } else {
            AutoFleetOrder(lpfl, lppl);
            if (cBuilt == 1) {
                FSendPlrMsg2(lppl->iPlayer, idmStarbaseHasBuiltNew, lpfl->id | 0x8000, lppl->id, lppl->iPlayer << 5 | iItem);
            } else {
                FSendPlrMsg(lppl->iPlayer, idmStarbaseHasBuiltNewShips, lpfl->id | 0x8000, lppl->id, cBuilt, lppl->iPlayer << 5 | iItem, 0, 0, 0, 0);
            }
        }
    } else {
        if (grobj != grobjPlanet) {
            return 0;
        }
        if ((uint16_t)iItem > iobjPlanetaryScanner) {
            return 0;
        }
        switch (iItem) {
        case iobjFactory:
        case mdIdleFactory:
            t_scratch_m16_3 = lppl->cFactories;
            cAllowed = CMaxFactories(lppl, lppl->iPlayer) - t_scratch_m16_3;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt > 0) {
                lppl->cFactories += cBuilt;
                idm = idmHaveBuiltFactory;
                break;
            }
            return 0;
        case iobjMine:
        case mdIdleMine:
            t_scratch_m16_4 = lppl->cMines;
            cAllowed = CMaxMines(lppl, lppl->iPlayer) - t_scratch_m16_4;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt > 0) {
                lppl->cMines += cBuilt;
                idm = idmHaveBuiltMine;
                break;
            }
            return 0;
        case iobjDefense:
        case mdIdleDefense:
            t_scratch_m16_5 = lppl->cDefenses;
            cAllowed = CMaxDefenses(lppl, lppl->iPlayer) - t_scratch_m16_5;
            cBuilt = cBuilt >= cAllowed ? cAllowed : cBuilt;
            if (cBuilt > 0) {
                lppl->cDefenses += cBuilt;
                idm = idmHaveBuiltDefenseOutpost;
                break;
            }
            return 0;
        case iobjPacket:
        case iobjPacketIron:
        case iobjPacketBor:
        case iobjPacketGerm:
        case iobjPacketMixed:
            raMajor = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv);
            iWarp = IWarpMAFromLppl(lppl, &fTwoMAs);
            if (iWarp == 0) {
                FSendPlrMsg2(lppl->iPlayer, idmMineralPacketFormedHasDisintegratedBecausePlanet, lppl->id, lppl->id, 0);
                return 0;
            }
            if (lppl->idFling == 0) {
                FSendPlrMsg2(lppl->iPlayer, idmMineralPacketFormedHasDisintegratedBecauseDidnt, lppl->id, lppl->id, 0);
                return 0;
            }
            if (iItem == iobjPacket) {
                iItem = iobjPacketMixed;
            }
            if (iItem == iobjPacketMixed) {
                cSize = raMajor == raMassAccel ? 25 : 40;
            } else {
                cSize = raMajor == raMassAccel ? 70 : 100;
            }
            for (i = 0; i < 3; i++) {
                if (i == iItem - 14 || iItem == iobjPacketMixed) {
                    l = (uint32_t)(cSize * cBuilt);
                    if (l > 32760) {
                        l = 32760;
                    }
                    rgwt[i] = LOWORD(l);
                } else {
                    rgwt[i] = 0;
                }
            }
            iWarpAsked = lppl->iWarpFling + 4;
            if (iWarpAsked < 5 || iWarpAsked > iWarp + 3) {
                iWarpAsked = iWarp + fTwoMAs;
            }
            if (iWarpAsked <= iWarp + fTwoMAs) {
                iDecayRate = decayNone;
            } else {
                iDecayRate = iWarpAsked - iWarp - fTwoMAs;
            }
            if (raMajor == raStargate && (int16_t)iDecayRate < decay50Pct) {
                iDecayRate++;
            }
            iWarp = iWarpAsked - 4;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac &&
                   (lpth->iplr != lppl->iPlayer || lpth->ith != ithMineralPacket || lpth->pt.x != rgptPlan[lppl->id].x || lpth->pt.y != rgptPlan[lppl->id].y ||
                    lpth->thp.iWarp != iWarp || lpth->thp.idPlanet != lppl->idFling - 1 || lpth->thp.iDecayRate != iDecayRate || lpth->thp.wtMax >= 1630);
                 lpth++) {
            }
            if (lpth != lpthMac) {
                lpth->thp.wtMax = 0;
                for (i = 0; i < 3; i++) {
                    lpth->thp.rgwtMin[i] += rgwt[i];
                    if (lpth->thp.rgwtMin[i] < 0) {
                        lpth->thp.rgwtMin[i] = 32760;
                    }
                    lpth->thp.wtMax += (int16_t)(lpth->thp.rgwtMin[i] + 9) / 10;
                }
                FSendPlrMsg2(lppl->iPlayer, idmHasProducedMineralPacketWhichHasCombined, lppl->id, lppl->id, lppl->idFling - 1);
                return 1;
            }
            lpth = LpthNew(lppl->iPlayer, ithMineralPacket);
            if (lpth == 0) {
                FSendPlrMsg2(lppl->iPlayer, idmHasOrdersBuildMineralPacketEitherDoesnt, lppl->id, lppl->id, 0);
                return 1;
            }
            for (i = 0; i < 3; i++) {
                lpth->thp.rgwtMin[i] = rgwt[i];
                lpth->thp.wtMax += (int16_t)(rgwt[i] + 9) / 10;
            }
            lpth->thp.iWarp = iWarp;
            lpth->thp.iDecayRate = iDecayRate;
            lpth->thp.idPlanet = lppl->idFling - 1;
            lpth->pt = rgptPlan[lppl->id];
            FSendPlrMsg2(lppl->iPlayer, idmHasProducedMineralPacketWhichHasDestination, lppl->id, lppl->id, lppl->idFling - 1);
            return 1;
        case iobjGenesis:
            for (i = 0; i < game.cPlayer; i++) {
                FSendPlrMsg2(i, idmStrongFundamentalForcesHaveRebirthed, lppl->id, lppl->id, 0);
            }
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->cFactories = 0;
                lppl->cMines = 0;
                lppl->cDefenses = 0;
                lppl->iScanner = 31;
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
        case iobjMinTerraform:
        case iobjMaxTerraform:
        case mdIdleTerraform:
            while (cBuilt-- != 0) {
                i = IBestTerraform(lppl, 1);
                if (i != 0) {
                    iEnv = abs(i) - 1;
                    cAllowed = lppl->rgEnvVar[iEnv] + (i <= 0 ? -1 : 1);
                    if (1 > (99 >= cAllowed ? cAllowed : 99)) {
                        cAllowed = 1;
                    } else if (99 < cAllowed) {
                        cAllowed = 99;
                    }
                    lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
                    FSendPlrMsg(lppl->iPlayer, idmTerraformingEffortsHave, lppl->id, lppl->id, i <= 0 ? 0 : 1, iEnv, iEnv * 256 + cAllowed, 0, 0, 0);
                }
            }
            return 1;
        case iobjPlanetaryScanner:
            idPlayer = lppl->iPlayer;
            LookupBestPlanetaryScanner(&part);
            idPlayer = -1;
            iItem = part.hs.iItem + 18;
        case iobjPlanetaryScannerFirst:
        case iobjPlanetaryScannerViewer90:
        case iobjPlanetaryScannerScoper150:
        case iobjPlanetaryScannerScoper220:
        case iobjPlanetaryScannerScoper280:
        case iobjPlanetaryScannerSnooper320X:
        case iobjPlanetaryScannerSnooper400X:
        case iobjPlanetaryScannerSnooper500X:
        case iobjPlanetaryScannerSnooper620X:
            FSendPlrMsg(lppl->iPlayer, idmHasBuiltNewPlanetaryScanner, lppl->id, lppl->id, -32768, iItem - 18, 0, 0, 0, 0);
            lppl->iScanner = iItem - 18;
        case iobjAlchemy:
        case 10:
        case mdIdleAlchemy:
            return 1;
        }
        cBuilt += FRemovePlayerMessage(lppl->iPlayer, idm, lppl->id);
        if (cBuilt > 1) {
            FSendPlrMsg2(lppl->iPlayer, idm + 1, lppl->id, cBuilt, lppl->id);
        } else {
            FSendPlrMsg2(lppl->iPlayer, idm, lppl->id, lppl->id, 0);
        }
    }
    return 1;
}

void CreateShip(int16_t iPlr, FLEET *lpfl, int16_t ishdef, int16_t cShip) {
    lpfl->rgcsh[ishdef] += cShip;
    rglpshdef[iPlr][ishdef].cExist += cShip;
    rglpshdef[iPlr][ishdef].cBuilt += cShip;
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
                if (lpxfCur->grobj1 == 1) {
                    rgxf[0].fl.iPlayer = LpplFromId(lpxfCur->id1)->iPlayer;
                } else {
                    rgxf[0].fl.iPlayer = lpxfCur->id1 >> 9 & 0xf;
                }
                for (i = 0; i < 5; i++) {
                    l2 = lpxfCur->rgcQuan[i];
                    if (l2 != 0) {
                        l = ChgCargo(lpxfCur->grobj2, lpxfCur->id2, i, l2, NULL);
                        if (l != l2) {
                            idm = i == 4 ? idmAttemptedTransferColonistsSuccessfullyReceivedRe : idmAttemptedTransferSuccessfullyReceived;
                            idSrc = lpxfCur->id1 | (lpxfCur->grobj1 == 2 ? 0x8000 : 0);
                            idDst = lpxfCur->id2 | (lpxfCur->grobj2 == 2 ? 0x8000 : 0);
                            if (l == 0) {
                                idm += 4;
                            }
                            FSendPlrMsg(rgxf[0].fl.iPlayer, idm, idSrc, idSrc, LOWORD(l2), HIWORD(l2), i, idDst, LOWORD(l), HIWORD(l));
                            idm = i == 4 ? idmReceivedHoweverColonistsSentRemainsOtherColonist : idmReceivedHoweverSentRemainderLostSpace;
                            if (l == 0) {
                                FSendPlrMsg(rgxf[1].fl.iPlayer, idm + 4, idDst, idDst, LOWORD(l2), HIWORD(l2), i, idSrc, 0, 0);
                            } else {
                                FSendPlrMsg(rgxf[1].fl.iPlayer, idm, idDst, idDst, LOWORD(l), HIWORD(l), i, idSrc, LOWORD(l2), HIWORD(l2));
                            }
                        } else {
                            idm = i == 4 ? idmSuccessfullyTransferred2 : idmSuccessfullyTransferred;
                            idSrc = lpxfCur->id1 | (lpxfCur->grobj1 == 2 ? 0x8000 : 0);
                            idDst = lpxfCur->id2 | (lpxfCur->grobj2 == 2 ? 0x8000 : 0);
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
    PLANET  *t_call_4159;

    if (cColDrop != 0) {
        lpcdCur = lpcd;
        lpcdMax = lpcd + cColDrop;
        for (; lpcdCur < lpcdMax; lpcdCur++) {
            if (lpcdCur->idPlanetDst != -1 && lpcdCur->cColonist != 0) {
                memset(rgcCol, 0, 64);
                memset(rgcPower, 0, 64);
                cPowerTot = 0;
                cColTot = 0;
                idPlanet = lpcdCur->idPlanetDst;
                FLookupPlanet(idPlanet, &pl);
                iplrOldOwner = pl.iPlayer;
                CalcPctSurvive(&pl, &pctSurvive, NULL);
                pctSurvive = (float)(pctSurvive + ((long double)1.0 - pctSurvive) / 4.0);
                for (lpcdLook = lpcdCur; lpcdLook < lpcdMax; lpcdLook++) {
                    if (idPlanet == lpcdLook->idPlanetDst) {
                        if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) == raMacintosh && (lpcdLook->fCanColonize == 0 || pl.iPlayer != -1)) {
                            FSendPlrMsg2(lpcdLook->idPlr, idmColonistsAttemptingSetShopReducedProtoplasmicBlo, pl.id, pl.id, 0);
                        } else if (pl.iPlayer == -1 && lpcdLook->fCanColonize == 0) {
                            FSendPlrMsg(lpcdLook->idPlr, idmColonistsForcedTransportDiedBecauseDidColonize, pl.id, LOWORD(lpcdLook->cColonist),
                                        HIWORD(lpcdLook->cColonist), pl.id, 0, 0, 0, 0);
                        } else if (pl.fStarbase != 0 && pl.iPlayer != -1) {
                            FSendPlrMsg2(lpcdLook->idPlr, idmColonistsAssaultingHaveKilledForcesOrbitingStarb, pl.id, pl.id, 0);
                        } else {
                            rgcCol[lpcdLook->idPlr] = rgcCol[lpcdLook->idPlr] + lpcdLook->cColonist;
                            cColTot += lpcdLook->cColonist;
                            if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) == raAttack) {
                                lPower = 165;
                            } else if (GetRaceStat(&rgplr[lpcdLook->idPlr], rsMajorAdv) == raMacintosh) {
                                lPower = 0;
                            } else {
                                lPower = 110;
                            }
                            lPower = (int32_t)((long double)((int32_t)(lpcdLook->cColonist * lPower) / 100) * pctSurvive);
                            cPowerTot += lPower;
                            rgcPower[lpcdLook->idPlr] = rgcPower[lpcdLook->idPlr] + lPower;
                        }
                        lpcdLook->idPlanetDst = -1;
                    }
                }
                if (pl.iPlayer != -1) {
                    if (GetRaceStat(&rgplr[pl.iPlayer], rsMajorAdv) == raDefend) {
                        lPower = 200;
                    } else {
                        lPower = 100;
                    }
                    lDefensePower = (int32_t)(pl.rgwtMin[3] * lPower) / 100;
                    if (lDefensePower > cPowerTot) {
                        for (i = 0; i < 16; i++) {
                            if (rgcCol[i] != 0) {
                                if ((long double)pctSurvive == (long double)1.0) {
                                    FSendPlrMsg(i, idmColonistsDroppedMassacredGroundTroops, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), pl.id,
                                                pl.iPlayer | 0x30, 0, 0, 0);
                                    FSendPlrMsg(pl.iPlayer, idmGroundTroopsValiantlyDestroyedAttackingBarbarian, pl.id, pl.id, LOWORD(rgcCol[i]),
                                                HIWORD(rgcCol[i]), i | 0x30, 0, 0, 0);
                                } else {
                                    FSendPlrMsg(i, idmColonistsDroppedDestroyedPlanetaryDefensesRestMa, pl.id, LOWORD(rgcCol[i]), HIWORD(rgcCol[i]), pl.id,
                                                (int32_t)(((long double)1.0 - pctSurvive) * 10000), pl.iPlayer | 0x30, 0, 0);
                                    FSendPlrMsg(pl.iPlayer, idmPlanetaryDefensesGroundTroopsDestroyedInvadingTr, pl.id, pl.id, LOWORD(rgcCol[i]),
                                                HIWORD(rgcCol[i]), i | 0x30, 0, 0, 0);
                                }
                            }
                        }
                        pl.rgwtMin[3] -= (int32_t)((int32_t)(pl.rgwtMin[3] * cPowerTot) / lDefensePower);
                        goto WritePlanet;
                    }
                    lOldPop = pl.rgwtMin[3];
                    UninhabitPlanet(&pl);
                } else {
                    lDefensePower = 0;
                    lOldPop = 0;
                }
                cMax = -1;
                c2nd = 0;
                cSides = 0;
                fTie = 0;
                iMax = 0;
                for (i = 0; i < game.cPlayer; i++) {
                    if (rgcCol[i] != 0) {
                        cSides++;
                        if (rgcPower[i] >= cMax) {
                            if (rgcPower[i] == cMax) {
                                fTie = 1;
                            } else {
                                fTie = 0;
                                c2nd = cMax;
                                cMax = rgcPower[i];
                                iMax = i;
                            }
                        }
                    }
                }
                if (cMax < 0)
                    continue;
                if (fTie != 0) {
                    for (i = 0; i < game.cPlayer; i++) {
                        if (rgcCol[i] != 0) {
                            FSendPlrMsg2(i, idmInvolvedWayAssaultNobodysTroopsSurvivedBrutal, pl.id, cSides, pl.id);
                        }
                    }
                    if (iplrOldOwner != -1) {
                        FSendPlrMsg2(iplrOldOwner, idmMultitudeEnemiesHaveMountedProngAttackResulting, pl.id, cSides, pl.id);
                        pl.iPlayer = -1;
                    }
                } else {
                    if (iplrOldOwner != -1) {
                        for (i = 0; i < 16; i++) {
                            if (rgcCol[i] != 0) {
                                if (i == iMax) {
                                    FSendPlrMsg2(i, idmTroopsCrushSColonistsControlPlanet, pl.id, iplrOldOwner | 0x20, pl.id);
                                } else {
                                    FSendPlrMsg2(i, idmColonistsDroppedDestroyedSpiritedFighting, pl.id, pl.id, 0);
                                }
                            }
                        }
                        FSendPlrMsg(iplrOldOwner, idmHaveAttackedFirstRateStormTroopersThough, pl.id, iMax | 0x30, pl.id, LOWORD(rgcCol[iMax]),
                                    HIWORD(rgcCol[iMax]), 0, 0, 0);
                        memset(rgTechBattle, 0, 6);
                        memset(rgTechTrader, 0, 13);
                        for (i = 0; i < 6; i++) {
                            rgTechBattle[i] = rgplr[iplrOldOwner].rgTech[i];
                        }
                        i = ITechLearnATech(iMax, -1, pl.id, idmWreckageDiscoveredBattleHasBoostedResearchResour, NULL);
                        pl.iPlayer = -1;
                    } else if (cSides > 1) {
                        for (i = 0; i < 16; i++) {
                            if (rgcCol[i] != 0) {
                                if (i == iMax) {
                                    FSendPlrMsg2(i, idmInvolvedWayRaceUninhabitedPlanetForcesCrush, pl.id, cSides, pl.id);
                                } else {
                                    FSendPlrMsg(i, idmColonistsDestroyedWayRaceUninhabitedPlanetContro, pl.id, cSides, pl.id, iMax | 0xb0, 0, 0, 0, 0);
                                }
                            }
                        }
                    } else {
                        FSendPlrMsg2(iMax, (GetRaceStat(&rgplr[iMax], rsMajorAdv) == raMacintosh ? 1 : 0) + 10, pl.id, pl.id, 0);
                    }
                    if (iMax != -1) {
                        cpq = rgplr[iMax].zpq1.cpq;
                        pl.fNoResearch = rgplr[iMax].zpq1.fNoResearch;
                        if (cpq > 0) {
                            pl.lpplprod = (PLPROD *)LpplAlloc(4, rgplr[iMax].zpq1.cpq, htOrd);
                            memset(&prod, 0, sizeof(PROD));
                            prod.grobj = grobjPlanet;
                            iDst = 0;
                            for (ipq = 0; ipq < cpq; ipq++) {
                                if ((GetRaceStat(&rgplr[iMax], rsMajorAdv) != raMacintosh || rgplr[iMax].zpq1.rgpq[ipq].mdIdle > iobjDefense) &&
                                    (GetRaceStat(&rgplr[iMax], rsMajorAdv) != raTerra ||
                                     (rgplr[iMax].zpq1.rgpq[ipq].mdIdle != iobjMinTerraform && rgplr[iMax].zpq1.rgpq[ipq].mdIdle != iobjMaxTerraform))) {
                                    prod.iItem = rgplr[iMax].zpq1.rgpq[ipq].mdIdle;
                                    prod.cItem = rgplr[iMax].zpq1.rgpq[ipq].cQuan;
                                    pl.lpplprod->rgprod[iDst] = prod;
                                    iDst++;
                                }
                            }
                            if (iDst > 0) {
                                pl.lpplprod->iprodMac = LOBYTE(iDst);
                                t_call_4159 = LpplFromId(pl.id);
                                t_call_4159->lpplprod = pl.lpplprod;
                            } else {
                                FreePl((PL *)pl.lpplprod);
                                pl.lpplprod = NULL;
                            }
                        }
                    }
                    pl.iPlayer = iMax;
                    if (GetRaceStat(&rgplr[iMax], rsMajorAdv) == raMacintosh) {
                        pl.fStarbase = 1;
                        pl.isb = 0;
                        rglpshdefSB[iMax]->cExist++;
                        rglpshdefSB[iMax]->cBuilt++;
                    }
                    if (cPowerTot == 0 || cMax == 0) {
                        pl.rgwtMin[3] = rgcCol[iMax];
                    } else {
                        lPower = (int32_t)((int32_t)(cMax * (cPowerTot - lDefensePower)) / cPowerTot);
                        pl.rgwtMin[3] = (int32_t)((int32_t)(rgcCol[iMax] * lPower) / cMax);
                    }
                    if (c2nd > 0) {
                        pl.rgwtMin[3] = (int32_t)((int32_t)(pl.rgwtMin[3] * (cMax - c2nd)) / cMax);
                    }
                    if (pl.rgwtMin[3] < 1) {
                        pl.rgwtMin[3] = 1;
                    }
                }
            WritePlanet:
                if (pl.iPlayer != -1 && pl.fArtifact != 0) {
                    pl.fArtifact = 0;
                    if (game.fNoRandom == 0) {
                        iTech = Random(6);
                        iBonus = Random(301) + 100;
                        if (pl.rgwtMin[3] < 10) {
                            iBonus = (int16_t)(LOWORD(pl.rgwtMin[3]) * iBonus) / 10;
                        }
                        FSendPlrMsg(pl.iPlayer, idmColonistsSettlingHaveFoundStrangeArtifactBoostin, gotoResearch, pl.id, iTech, iBonus, 0, 0, 0, 0);
                        rgplr[pl.iPlayer].rgResSpent[iTech] = rgplr[pl.iPlayer].rgResSpent[iTech] + iBonus;
                        if (game.fSlowTech != 0) {
                            iBonus >>= 1;
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
        if (rglpfl[i] == 0)
            break;
        if (lpfl->fDead == 0 && lpfl->fNoHeal == 0) {
            dpHeal = 0;
            pctShipHeal = 0;
            for (ishdef = 0; ishdef < 16; ishdef++) {
                if (lpfl->rgdv[ishdef].dp != 0) {
                    dpHeal = 1;
                }
                if (lpfl->rgcsh[ishdef] != 0) {
                    if (rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef == ihuldefSuperFuelXport) {
                        pctShipHeal = 50;
                    } else if (pctShipHeal < 5 && rglpshdef[lpfl->iPlayer][ishdef].hul.ihuldef == ihuldefFuelTransport) {
                        pctShipHeal = 25;
                    }
                }
            }
            if (dpHeal != 0) {
                if (lpfl->fHereAllTurn == 0) {
                    pct = 5;
                } else if (lpfl->idPlanet == -1) {
                    pct = 10;
                } else {
                    lppl = LpplFromId(lpfl->idPlanet);
                    if (lppl->iPlayer != lpfl->iPlayer) {
                        if (lppl->iPlayer == -1) {
                            pct = 15;
                        } else {
                            pct = 15;
                        }
                    } else if (lppl->fStarbase != 0 && lppl->fNoHeal == 0) {
                        lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
                        if (LphuldefFromId(lpshdef->hul.ihuldef)->hul.wtCargoMax != 0) {
                            pct = 100;
                        } else {
                            pct = 40;
                        }
                    } else {
                        pct = 25;
                    }
                }
                if (GetRaceStat(&rgplr[lpfl->iPlayer], rsMajorAdv) == raDefend) {
                    pct *= 2;
                }
                pct += pctShipHeal;
                for (ishdef = 0; ishdef < 16; ishdef++) {
                    if (lpfl->rgdv[ishdef].dp != 0) {
                        if (lpfl->rgdv[ishdef].pctDp > (uint16_t)pct) {
                            lpfl->rgdv[ishdef].pctDp -= pct;
                        } else {
                            lpfl->rgdv[ishdef].dp = 0;
                        }
                    }
                }
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->fStarbase != 0 && lppl->fNoHeal == 0) {
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raDefend) {
                pct = 75;
            } else {
                pct = 50;
            }
            if (lppl->pctDp != 0) {
                lpshdef = rglpshdefSB[lppl->iPlayer] + lppl->isb;
                if ((uint16_t)pct > lppl->pctDp) {
                    lppl->pctDp = 0;
                } else {
                    lppl->pctDp -= pct;
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
                if (lppl->fStarbase != 0 && lppl->iPlayer == -1) {
                    lppl->fStarbase = 0;
                }
                i = Random(3);
                if (rgplr[lppl->iPlayer].rgEnvVar[i] != -1 && rgplr[lppl->iPlayer].rgEnvVar[i] != lppl->rgEnvVarOrig[i] && Random(10) == 0) {
                    if (lppl->rgwtMin[3] < 1000) {
                        t_scratch_m42_2 = Random(1000);
                        if (t_scratch_m42_2 >= (int16_t)LOWORD(lppl->rgwtMin[3]))
                            goto L_4b5c;
                    }
                    if (rgplr[lppl->iPlayer].rgEnvVar[i] < lppl->rgEnvVarOrig[i]) {
                        lppl->rgEnvVarOrig[i]--;
                    } else {
                        lppl->rgEnvVarOrig[i]++;
                    }
                    FSendPlrMsg2(lppl->iPlayer, idmEngineersHaveManagedImproveUnderlying1, lppl->id, lppl->id, i);
                }
            L_4b5c:
                if (FCanTerraformLppl(lppl, rgMin, rgMax, rgCost, 1) != 0) {
                    for (i = 0; i < 3; i++) {
                        if (rgMin[i] != -1) {
                            lppl->rgEnvVar[i] = LOBYTE(rgMin[i]);
                        } else if (rgMax[i] != -1) {
                            lppl->rgEnvVar[i] = LOBYTE(rgMax[i]);
                        }
                    }
                    i = PctPlanetDesirability(lppl, lppl->iPlayer);
                    FSendPlrMsg2(lppl->iPlayer, idmHasAutoTerraformedValue, lppl->id, lppl->id, i);
                }
            }
        }
    }
    return;
}

void RemoteTerraforming() {
    int16_t fHelp;
    int16_t iBest;
    int16_t pctCur;
    PLANET *lppl;
    int16_t ifl;
    FLEET  *lpfl;
    int16_t cDone;
    EnvType iEnv;
    int16_t cAllowed;
    int32_t ipct;
    int16_t pctNew;

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->fDead == 0 && lpfl->idPlanet != -1 && lpPlanets[lpfl->idPlanet].iPlayer != -1) {
            ipct = PctTerraFromLpfl(lpfl);
            if (ipct > 0) {
                lppl = lpPlanets + lpfl->idPlanet;
                fHelp = lpfl->iPlayer == lppl->iPlayer || rgplr[lpfl->iPlayer].rgmdRelation[lppl->iPlayer] == 1 || lpfl->iPlayer == lppl->iPlayer;
                if (fHelp != 0 || lppl->fStarbase == 0) {
                    pctCur = PctPlanetDesirability(lppl, lppl->iPlayer);
                    cDone = 0;
                    while (ipct-- > 0) {
                        iBest = IBestRemoteTerra(lppl, lpfl->iPlayer, fHelp);
                        if (iBest == 0)
                            break;
                        iEnv = abs(iBest) - 1;
                        cAllowed = lppl->rgEnvVar[iEnv] + (iBest <= 0 ? -1 : 1);
                        if (1 > (99 >= cAllowed ? cAllowed : 99)) {
                            cAllowed = 1;
                        } else if (99 < cAllowed) {
                            cAllowed = 99;
                        }
                        lppl->rgEnvVar[iEnv] = LOBYTE(cAllowed);
                        cDone++;
                    }
                    pctNew = PctPlanetDesirability(lppl, lppl->iPlayer);
                    FSendPlrMsg(lpfl->iPlayer, (fHelp == 0 ? 346 : 300) + (pctCur == pctNew ? 1 : 0), lpfl->id | 0x8000, lpfl->id, lppl->id, pctCur, pctNew, 0,
                                0, 0);
                    if (lpfl->iPlayer != lppl->iPlayer && pctNew != pctCur) {
                        FSendPlrMsg(lppl->iPlayer, fHelp == 0 ? idmHasDegradedValue : idmHasImprovedValue, lppl->id, lpfl->id, lppl->id, pctCur, pctNew, 0, 0,
                                    0);
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
        lpcdT++;
    }
    if (iColDrop == cColDrop) {
        if (cColDrop >= 1000) {
            return 0;
        }
        lpcdT->idFleetSrc = lpfl->id;
        lpcdT->idPlr = lpfl->iPlayer;
        lpcdT->idPlanetDst = lppl->id;
        lpcdT->cColonist = 0;
        lpcdT->fCanColonize = 1;
        cColDrop++;
    }
    lpcdT->cColonist += cColonists;
    return LOWORD(cColonists);
}

void UpdatePopulations() {
    int32_t lPopChg;
    PLANET *lppl;
    PLANET *lpplMac;
    int32_t lPopOld;
    int16_t fMac;

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer != -1 && lppl->rgwtMin[3] != 0) {
            lPopChg = ChgPopFromPlanet(lppl, 1);
            if (lPopChg != 0 && lPopChg < 0 && lppl->rgwtMin[3] > 0) {
                lPopOld = lppl->rgwtMin[3] - lPopChg;
                if (PctPlanetDesirability(lppl, lppl->iPlayer) < 0) {
                    FSendPlrMsg(lppl->iPlayer, idmPopulationHasDecreased, lppl->id, lppl->id, LOWORD(lPopOld), HIWORD(lPopOld), LOWORD(lppl->rgwtMin[3]),
                                HIWORD(lppl->rgwtMin[3]), 0, 0);
                } else {
                    FSendPlrMsg(lppl->iPlayer, idmPopulationHasDecreasedColonistsDueOvercrowding, lppl->id, lppl->id, -LOWORD(lPopChg),
                                LOWORD((uint32_t)((uint32_t)-lPopChg >> 0x10)), 0, 0, 0, 0);
                }
            }
        }
        if (lppl->iPlayer != -1 && lppl->rgwtMin[3] == 0) {
            fMac = GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh ? 1 : 0;
            FSendPlrMsg2(lppl->iPlayer, (lPopChg < 0 ? 35 : 64) + fMac, lppl->id, lppl->id, 0);
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

    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->rgwtMin[3] != 0) {
            l = lppl->rgwtMin[3];
            if (GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh) {
                l = 0;
            } else {
                l += -(int32_t)(l >> 3) + Random((int32_t)(l >> 2));
                l = (int32_t)(l >> 2);
                if (l > 4090) {
                    l = 4090;
                } else if (l < 1) {
                    l = 1;
                }
            }
            lppl->uPopGuess = LOWORD(l);
            if (lppl->cDefenses == 0) {
                lppl->uDefGuess = 0;
            } else {
                CalcPctSurvive(lppl, &pct, NULL);
                l = 100 - (int32_t)((long double)pct * 100.0 + 0.5) + 4;
                l = (int32_t)(l / 6);
                if (l < 1) {
                    l = 1;
                } else if (l > 15) {
                    l = 15;
                }
                lppl->uDefGuess = LOWORD(l);
            }
        } else {
            lppl->uGuesses = 0;
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
        if ((lppl->iPlayer == -1 || lppl->rgwtMin[3] <= 50 || game.turn >= 20) && game.turn >= 10) {
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
                rgQuan[i] = (int16_t)(Random(250) + 50);
            }
            for (i = 0; i < 2; i++) {
                j = Random(3 - i) + i;
                iT = rgAffect[i];
                rgAffect[i] = rgAffect[j];
                rgAffect[j] = iT;
            }
            for (i = 0; i < game.cPlayer; i++) {
                t_merge_5834_0001 = i == lppl->iPlayer && GetRaceStat(&rgplr[i], rsMajorAdv) != raMacintosh ? 135 : 131;
                FSendPlrMsg(i, t_merge_5834_0001 + iSize, lppl->id, lppl->id, rgEnv[0], rgEnv[1], rgEnv[2], 0, 0, 0);
            }
            if (lppl->iPlayer != -1 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                lppl->rgwtMin[3] -= (int32_t)(lppl->rgwtMin[3] * (int16_t)(20 * iSize + 25)) / 100;
            }
            for (i = 0; i <= iSize && i < 3; i++) {
                rgQuan[rgAffect[i]] = rgQuan[rgAffect[i]] + (int16_t)(Random(17000) + 3000);
                iConc = lppl->rgMinConc[rgAffect[i]];
                iConc += Random(50) + 50;
                if (iSize == 3) {
                    iConc += Random(15) + 15;
                }
                if (iConc > 200) {
                    iConc = 200;
                }
                lppl->rgMinConc[rgAffect[i]] = LOBYTE(iConc);
            }
            for (i = 0; i < 3; i++) {
                lppl->rgwtMin[i] += (int32_t)(rgQuan[i] >> 4);
            }
            for (i = 0; i < 3 && i <= iSize; i++) {
                iT = Random(3) + 3;
                if (iSize == 3) {
                    iT += Random(3) + 3;
                }
                if (Random(2) != 0) {
                    iT = -iT;
                }
                j = lppl->rgEnvVar[i] + iT;
                if (j < 1) {
                    j = 1;
                } else if (j > 99) {
                    j = 99;
                }
                lppl->rgEnvVar[i] = LOBYTE(j);
                j = lppl->rgEnvVarOrig[i] + iT;
                if (j < 1) {
                    j = 1;
                } else if (j > 99) {
                    j = 99;
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

    if (lppl->lpplprod != 0) {
        iDst = 0;
        for (iSrc = 0; iSrc < lppl->lpplprod->iprodMac; iSrc++) {
            if (lppl->lpplprod->rgprod[iSrc].grobj == grobjPlanet && lppl->lpplprod->rgprod[iSrc].iItem < mdIdleFactory) {
                if (iSrc > iDst) {
                    lppl->lpplprod->rgprod[iDst] = lppl->lpplprod->rgprod[iSrc];
                }
                iDst++;
            }
        }
        if (iDst > 0) {
            lppl->lpplprod->iprodMac = LOBYTE(iDst);
        } else {
            FreePl((PL *)lppl->lpplprod);
            lppl->lpplprod = NULL;
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
        if (lppl->iPlayer == -1 || lppl->rgwtMin[3] <= 50 || game.turn >= 20) {
            i = Random(3);
            if (lppl->iPlayer != -1) {
                FSendPlrMsg2(lppl->iPlayer, idmFundamentalChangesEnvironmentHavePermanentlyAlte, lppl->id, lppl->id, i);
            }
            iT = Random(3) + 3;
            if (iT == 3) {
                iT += Random(3) + 3;
            }
            if (Random(2) != 0) {
                iT = -iT;
            }
            j = lppl->rgEnvVar[i] + iT;
            if (j < 1) {
                j = 1;
            } else if (j > 99) {
                j = 99;
            }
            lppl->rgEnvVar[i] = LOBYTE(j);
            j = lppl->rgEnvVarOrig[i] + iT;
            if (j < 1) {
                j = 1;
            } else if (j > 99) {
                j = 99;
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
        if (game.turn >= 10) {
            i = Random(3);
            if (lppl->iPlayer != -1) {
                FSendPlrMsg(lppl->iPlayer, idmSurveyorsHaveDiscoveredPreviouslyUnknownDepositS, lppl->id, lppl->id, i, 0, 0, 0, 0, 0);
            }
            if (lppl->rgMinConc[i] < 180) {
                lppl->rgMinConc[i] += LOBYTE(Random(15) + 5);
            }
        }
    }
    return;
}

void MysteryTrader() {
    int16_t     iSrc;
    int16_t     cRand;
    int16_t     i;
    THING      *lpth;
    GrbitTrader grbitTrader;
    int16_t     rgC[4];

    if (game.turn >= 40) {
        if ((uint32_t)game.turn % 100 == 71) {
            cRand = 2;
        } else if ((uint32_t)game.turn % 100 == 33) {
            cRand = 3;
        } else if ((game.turn & 0x7f) == 0x31) {
            cRand = 4;
        } else {
            if ((game.turn & 1) != 0) {
                return;
            }
            cRand = 7;
        }
        if (Random(cRand) == 0) {
            lpth = LpthNew(0, ithMysteryTrader);
            if (lpth != 0) {
                lpth->tht.iWarp = Random(5) + 8;
                for (i = 0; i < 4; i += 2) {
                    rgC[i] = Random(400 * game.mdSize + 361) + 1020;
                }
                if (Random(2) == 0) {
                    rgC[1] = 1020;
                    rgC[3] = 400 * game.mdSize + 1380;
                } else {
                    rgC[1] = 400 * game.mdSize + 1380;
                    rgC[3] = 1020;
                }
                iSrc = Random(2);
                lpth->pt.x = rgC[iSrc];
                lpth->pt.y = rgC[iSrc == 0 ? 1 : 0];
                lpth->tht.ptDest.x = rgC[iSrc + 2];
                lpth->tht.ptDest.y = rgC[(iSrc == 0 ? 1 : 0) + 2];
                if (game.turn < 100) {
                    cRand = 5;
                } else if (game.turn < 250) {
                    cRand = 3;
                } else {
                    cRand = 2;
                }
                if (lpth->tht.iWarp <= 9) {
                    cRand++;
                } else if (lpth->tht.iWarp >= 11) {
                    cRand--;
                }
                if (Random(10) < cRand) {
                    if (Random(6) == 0) {
                        lpth->tht.grbitTrader = grbitTraderLifeboat;
                    } else {
                        lpth->tht.grbitTrader = grbitTraderNone;
                    }
                } else {
                    grbitTrader = 1 << Random(13);
                    switch (grbitTrader) {
                    case grbitTraderTorp:
                    case grbitTraderBeam:
                    case grbitTraderGenesis:
                    case grbitTraderJumpgate:
                        grbitTrader = 1 << Random(13);
                        if (((game.turn < 120 && grbitTrader == grbitTraderBeam) || (game.turn < 150 && grbitTrader == grbitTraderGenesis) ||
                             (game.turn < 180 && grbitTrader == grbitTraderJumpgate)) &&
                            Random(2) != 0) {
                            grbitTrader = grbitTraderNone;
                        }
                    }
                    lpth->tht.grbitTrader = grbitTrader;
                }
                for (i = 0; i < game.cPlayer; i++) {
                    FSendPlrMsg2(i, idmMysteriousTradingVesselBroadcastingProposalHasDe, gotoThing, lpth->idFull, 0);
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
    gd.fGameOverMan = 0;
    memset(rgcCond, 0, 16);
    for (i = 0; i < game.cPlayer; i++) {
        rglScore[i] = CalcPlayerScore(i, &score);
        vlprgScoreX[i].score = score;
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffe0) | (i & 0x1f);
        vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xffdf) | 0x20;
        vlprgScoreX[i].wWord &= 0xc03f;
        lScoreTot += rglScore[i];
        if (score.cPlanet == 0 && score.rgcsh[0] == 0 && score.rgcsh[1] == 0 && score.rgcsh[2] == 0 && rgplr[i].fDead == 0) {
            rgplr[i].wFlags = (rgplr[i].wFlags & 0xfffe) | 1;
            for (j = 0; j < game.cPlayer; j++) {
                if (j != i) {
                    FSendPrependedPlrMsg(j, idmTracesHaveEliminatedGalaxyMayRestPeace, gotoScore, i | 0x30, 0, 0, 0, 0, 0, 0);
                }
            }
        }
        if (score.cPlanet >= MulDiv(cPlanet, GetVCVal(&game, vcOwnsPercentPlanets, 0), 100)) {
            t_scratch_m86_5 = (vlprgScoreX[i].wWord | 0x40) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0;
            vlprgScoreX[i].wWord |= t_scratch_m86_5;
            if (GetVCCheck(&game, vcOwnsPercentPlanets) != 0) {
                rgcCond[i]++;
            }
        }
        t_scratch_m88_2 = (int32_t)((uint32_t)(score.rgcsh[2] & 0x1fff) << (score.rgcsh[2] >> 0xd << 1));
        if ((int32_t)t_scratch_m88_2 >= GetVCVal(&game, vcOwnsCapitalShips, 0)) {
            t_scratch_m86_6 = (vlprgScoreX[i].wWord | 0x800) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0;
            vlprgScoreX[i].wWord |= t_scratch_m86_6;
            if (GetVCCheck(&game, vcOwnsCapitalShips) != 0) {
                rgcCond[i]++;
            }
        }
        if (rglScore[i] >= GetVCVal(&game, vcExceedsScore, 0)) {
            t_scratch_m86_7 = (vlprgScoreX[i].wWord | 0x100) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0;
            vlprgScoreX[i].wWord |= t_scratch_m86_7;
            if (GetVCCheck(&game, vcExceedsScore) != 0) {
                rgcCond[i]++;
            }
        }
        c = 0;
        for (j = 0; j < 6; j++) {
            t_scratch_m86_8 = rgplr[i].rgTech[j];
            if (t_scratch_m86_8 >= GetVCVal(&game, vcAttainsTechLevel, 0)) {
                c++;
            }
        }
        if (c >= GetVCVal(&game, vcAttainsTechFields, 0)) {
            t_scratch_m86_9 = (vlprgScoreX[i].wWord | 0x80) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0;
            vlprgScoreX[i].wWord |= t_scratch_m86_9;
            if (GetVCCheck(&game, vcAttainsTechLevel) != 0) {
                rgcCond[i]++;
            }
        }
        t_scratch_m88_6 = (int32_t)(score.cResources / 1000);
        if ((int32_t)t_scratch_m88_6 >= GetVCVal(&game, vcProductionCapacity, 0)) {
            t_scratch_m86_10 = (vlprgScoreX[i].wWord | 0x400) & 0x3fc0;
            vlprgScoreX[i].grbitVC = 0;
            vlprgScoreX[i].wWord |= t_scratch_m86_10;
            if (GetVCCheck(&game, vcProductionCapacity) != 0) {
                rgcCond[i]++;
            }
        }
    }
    if (game.cPlayer == 1) {
        vlprgScoreX->iRank = 1;
    } else {
        for (i = 0; i < game.cPlayer; i++) {
            if (rgplr[i].fDead != 0) {
                cDead++;
            }
            rgplr[i].wScore = 1;
            for (j = 0; j < game.cPlayer; j++) {
                if (rglScore[j] > rglScore[i]) {
                    rgplr[i].wScore++;
                }
            }
            if (rgplr[i].wScore == 1) {
                iScoreMax = i;
                lScoreMax = rglScore[i];
                cFirst++;
            } else if (rgplr[i].wScore == 2) {
                lScore2nd = rglScore[i];
            }
        }
        if (cFirst > 1) {
            lScore2nd = lScoreMax;
        }
        for (i = 0; i < game.cPlayer; i++) {
            vlprgScoreX[i].turn = rgplr[i].wScore;
        }
        if ((int16_t)game.turn >= GetVCVal(&game, vcHighestScoreAfterYears, 0) && cFirst == 1) {
            t_scratch_m86_12 = (vlprgScoreX[iScoreMax].wWord | 0x1000) & 0x3fc0;
            vlprgScoreX[iScoreMax].grbitVC = 0;
            vlprgScoreX[iScoreMax].wWord |= t_scratch_m86_12;
            if (GetVCCheck(&game, vcHighestScoreAfterYears) != 0) {
                rgcCond[iScoreMax]++;
            }
        }
        if (cDead + 1 >= game.cPlayer) {
            gd.fGameOverMan = 1;
            if (rgplr[iScoreMax].fDead == 0) {
                FSendPrependedPlrMsg(iScoreMax, idmTracesEveryOtherRivalHaveEliminatedGalaxy, gotoScore, 0, 0, 0, 0, 0, 0, 0);
            }
            for (i = 0; i < game.cPlayer; i++) {
                if (i != iScoreMax) {
                    FSendPrependedPlrMsg(i, idmDeadPlanetsHaveOverrunSpaceshipsDefeated, gotoScore, 0, 0, 0, 0, 0, 0, 0);
                }
            }
        } else {
            if (lScoreMax >= (int32_t)(lScore2nd * (int16_t)(GetVCVal(&game, vcExceedsSecondPlaceBy, 0) + 100)) / 100) {
                t_scratch_m86_13 = (vlprgScoreX[iScoreMax].wWord | 0x200) & 0x3fc0;
                vlprgScoreX[iScoreMax].grbitVC = 0;
                vlprgScoreX[iScoreMax].wWord |= t_scratch_m86_13;
                if (GetVCCheck(&game, vcExceedsSecondPlaceBy) != 0) {
                    rgcCond[iScoreMax]++;
                }
            }
            if (game.turn >= (uint16_t)GetVCVal(&game, vcMinYearsBeforeWin, 0)) {
                wWinners = 0;
                j = GetVCVal(&game, vcMeetsNumCriteria, 0);
                if (j >= 1) {
                    for (i = game.cPlayer - 1; i >= 0; i--) {
                        wWinners *= 2;
                        if (rgcCond[i] >= j) {
                            vlprgScoreX[i].wWord = (vlprgScoreX[i].wWord & 0xbfff) | 0x4000;
                            wWinners |= 1;
                        }
                    }
                    if (wWinners != 0) {
                        gd.fGameOverMan = 1;
                    }
                }
                if (gd.fGameOverMan != 0) {
                    i = 0;
                    j = 1;
                    while (i < game.cPlayer) {
                        wWinners2 = wWinners;
                        if (rgplr[i].fDead != 0) {
                            imsg = 184;
                        } else if ((j & wWinners) == 0) {
                            imsg = 181;
                        } else if ((j ^ wWinners) != 0) {
                            imsg = 183;
                            wWinners2 &= ~j;
                        } else {
                            imsg = 182;
                        }
                        FSendPrependedPlrMsg(i, imsg, gotoScore, wWinners2, 0, 0, 0, 0, 0, 0);
                        i++;
                        j *= 2;
                    }
                }
            }
        }
    }
    return;
}

void CreateBackupDir() {
    char *pchT;

    strcpy(szBackup, szBase);
    pchT = strrchr(szBackup, 92);
    if (pchT == 0) {
        pchT = szBackup;
    } else {
        pchT++;
    }
    *pchT = 0;
    if (vcBackupDirs <= 1) {
        strcpy(pchT, "backup");
    } else if (vcBackupDirs <= 99) {
        _wsprintf(pchT, "backup%d", (uint32_t)game.turn % vcBackupDirs);
    } else {
        _wsprintf(pchT, "backup.%03d", (uint32_t)game.turn % vcBackupDirs);
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

    if (lpth->thp.iDecayRate <= decayNone) {
        return 0;
    }
    switch (lpth->thp.iDecayRate) {
    case decay10Pct:
        iRate = 10;
        break;
    case decay25Pct:
        iRate = 25;
        break;
    case decay50Pct:
        iRate = 50;
    }
    if (GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMassAccel) {
        iRate /= 2;
        iRateMin = 5;
    } else {
        iRateMin = 10;
    }
    lDecay = 0;
    for (i = 0; i < 3; i++) {
        if (lpth->thp.rgwtMin[i] != 0) {
            wDecay = LOWORD((int32_t)((uint32_t)(lpth->thp.rgwtMin[i] * iRate) * pctRate) / 10000);
            wDecay = iRateMin <= wDecay ? wDecay : iRateMin;
            if (lpth->thp.rgwtMin[i] <= (int16_t)wDecay) {
                wDecay = lpth->thp.rgwtMin[i];
            }
            lpth->thp.rgwtMin[i] -= wDecay;
            lDecay += lpth->thp.rgwtMin[i];
        }
    }
    if (lDecay == 0) {
        FreeLpth(lpth);
        return 1;
    }
    lpth->thp.wtMax = LOWORD((int32_t)((lDecay + 9) / 10));
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

    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        lpfl->fBombed = 0;
    }
    lpth = lpThings;
    lpthMac = lpThings + cThing;
    for (; lpth < lpthMac; lpth++) {
        if (lpth->ith == ithMineralPacket) {
            if (lpth->thp.iWarp == 0) {
                if (lpth->thp.fMoved != 0) {
                    lpth->thp.fMoved = 0;
                    continue;
                }
                lDecay = 0;
                for (i = 0; i < 3; i++) {
                    if (lpth->thp.rgwtMin[i] != 0) {
                        wDecay = 10 <= lpth->thp.rgwtMin[i] / 10 ? lpth->thp.rgwtMin[i] / 10 : 10;
                        lpth->thp.rgwtMin[i] -= wDecay;
                        if (lpth->thp.rgwtMin[i] < 0) {
                            lpth->thp.rgwtMin[i] = 0;
                        }
                        lDecay += lpth->thp.rgwtMin[i];
                    }
                }
                if (lDecay == 0) {
                    FreeLpth(lpth);
                } else {
                    lpth->thp.wtMax = LOWORD((int32_t)((lDecay + 9) / 10));
                    continue;
                }
            } else if (FPacketDecay(lpth, 100) == 0) {
                continue;
            }
            lpth--;
            lpthMac--;
        } else if (lpth->ith == ithMinefield) {
            fMineExpert = GetRaceStat(&rgplr[lpth->iplr], rsMajorAdv) == raMines ? 1 : 0;
            if (lpth->thm.fDetonate != 0) {
                lDecay = lpth->thm.cMines;
                for (ifl = 0; ifl < cFleet; ifl++) {
                    lpfl = rglpfl[ifl];
                    if (rglpfl[ifl] == 0)
                        break;
                    if (lpfl->fDead == 0) {
                        dx = (int16_t)(lpfl->pt.x - lpth->pt.x);
                        dy = (int16_t)(lpfl->pt.y - lpth->pt.y);
                        if (lpfl->fBombed == 0 && (uint32_t)(dx * dx) + (uint32_t)(dy * dy) <= lDecay) {
                            FTravelThroughMineFields(lpfl, NULL, lpth);
                            lpfl->fBombed = 1;
                        }
                    }
                }
            }
            pctDecay = (int16_t)(((fMineExpert == 0 ? 1 : 0) * 3 + 1) * CPlanetsInCircle(lpth->pt, lpth->thm.cMines) + 2);
            if (pctDecay > 50) {
                pctDecay = 50;
            }
            if (lpth->thm.fDetonate != 0) {
                pctDecay += 25;
            }
            lDecay = (int32_t)(lpth->thm.cMines * pctDecay) / 100;
            if (lDecay < pctDecay) {
                lDecay = pctDecay;
            }
            if (lpth->thm.iType != mineSpeedBump) {
                lDecay = 10 <= lDecay ? lDecay : 10;
            }
            if (lDecay >= lpth->thm.cMines) {
                FreeLpth(lpth);
                lpth--;
                lpthMac--;
            } else {
                lpth->thm.cMines -= lDecay;
            }
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
            lpth->thm.grbitPlrNow = 0;
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
        if (rglpfl[ifl] == 0)
            break;
        cMine = CMineSweepFromLpfl(lpfl);
        if (cMine > 0 && lpfl->fDead == 0) {
            iplr = lpfl->iplr;
            grbitPlr = 1 << lpfl->iplr;
            pt = lpfl->pt;
            lpth = lpThings;
            lpthMac = lpThings + cThing;
            for (; lpth < lpthMac; lpth++) {
                if (lpth->ith == ithMinefield && lpth->iplr != iplr && FAttackPlayer(lpfl, lpth->iplr) != 0) {
                    dx = (int16_t)(pt.x - lpth->pt.x);
                    dy = (int16_t)(pt.y - lpth->pt.y);
                    lCur = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                    if (lpth->thm.cMines >= (uint32_t)(dx * dx) + (uint32_t)(dy * dy)) {
                        if (lpth->thm.iType == mineSpeedBump) {
                            cMineCur = (int32_t)(cMine / 3);
                        } else {
                            cMineCur = cMine;
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
                        FSendPlrMsg(lpfl->iPlayer, idmHasSweptMinesMineField, 0x8000 | lpfl->id, lpfl->id, LOWORD(cMineCur), HIWORD(cMineCur), lpth->iplr,
                                    lpth->thm.iType, lpth->pt.x, lpth->pt.y);
                        FSendPlrMsg(lpth->iplr, idmSomeoneHasSweptMinesMineField, gotoThing, lpth->idFull, LOWORD(cMineCur), HIWORD(cMineCur), lpth->thm.iType,
                                    lpth->pt.x, lpth->pt.y, 0);
                        lpth->thm.cMines -= cMineCur;
                        if (lpth->thm.cMines <= 0) {
                            FreeLpth(lpth);
                            lpth--;
                            lpthMac--;
                        } else {
                            lpth->thm.grbitPlr |= 1 << lpfl->iPlayer;
                        }
                    }
                }
            }
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->fStarbase != 0 && lppl->iPlayer != -1) {
            cMine = CMineSweepFromLphul(&rglpshdefSB[lppl->iPlayer][lppl->isb].hul);
            if (cMine > 0) {
                iplr = lppl->iPlayer;
                grbitPlr = 1 << lppl->iPlayer;
                pt = rgptPlan[lppl->id];
                lpth = lpThings;
                lpthMac = lpThings + cThing;
                for (; lpth < lpthMac; lpth++) {
                    if (lpth->ith == ithMinefield && lpth->iplr != iplr && iplr != lpth->iplr && rgplr[iplr].rgmdRelation[lpth->iplr] != 1) {
                        dx = (int16_t)(pt.x - lpth->pt.x);
                        dy = (int16_t)(pt.y - lpth->pt.y);
                        lCur = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
                        if (lpth->thm.cMines >= (uint32_t)(dx * dx) + (uint32_t)(dy * dy)) {
                            if (lpth->thm.iType == mineSpeedBump) {
                                cMineCur = (int32_t)(cMine / 3);
                            } else {
                                cMineCur = cMine;
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
                            FSendPlrMsg(iplr, idmStarbaseHasSweptMinesMineField, lppl->id, lppl->id, LOWORD(cMineCur), HIWORD(cMineCur), lpth->iplr,
                                        lpth->thm.iType, lpth->pt.x, lpth->pt.y);
                            FSendPlrMsg(lpth->iplr, idmSomeoneHasSweptMinesMineField, gotoThing, lpth->idFull, LOWORD(cMineCur), HIWORD(cMineCur),
                                        lpth->thm.iType, lpth->pt.x, lpth->pt.y, 0);
                            lpth->thm.cMines -= cMineCur;
                            if (lpth->thm.cMines <= 0) {
                                FreeLpth(lpth);
                                lpth--;
                                lpthMac--;
                            } else {
                                lpth->thm.grbitPlr |= 1 << lppl->iPlayer;
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

    fNoBreeders = 1;
    for (i = 0; i < game.cPlayer; i++) {
        t_call_7e83 = GetRaceStat(&rgplr[i], rsMajorAdv);
        t_merge_7e9a_0001 =
            t_call_7e83 == raDefend ? ((uint16_t)t_call_7e83 & 0xff00) | ((uint16_t)1 & 0xff) : ((uint16_t)t_call_7e83 & 0xff00) | ((uint16_t)0 & 0xff);
        grfBreeder[i] = LOBYTE(t_merge_7e9a_0001);
        if ((int16_t)(int8_t)LOBYTE(t_merge_7e9a_0001) == 1) {
            fNoBreeders = 0;
        }
    }
    if (fNoBreeders == 0) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            if (lpfl->fDead == 0 && grfBreeder[lpfl->iPlayer] != 0 && lpfl->rgwtMin[3] != 0) {
                lColGain = (int32_t)(lpfl->rgwtMin[3] * (int16_t)rgplr[lpfl->iPlayer].pctIdealGrowth) / 200;
                if (lColGain <= 0) {
                    if (Random(3) != 0)
                        continue;
                    lColGain = 1;
                }
                lColGainAct = ChgCargo(grobjFleet, lpfl->id, Colonists, lColGain, NULL);
                if (lColGainAct > 0) {
                    FSendPlrMsg2(lpfl->iPlayer, idmColonistsHaveMadeGoodUseTimeIncreasing, lpfl->id | 0x8000, lpfl->id, LOWORD(lColGainAct));
                }
                if (lColGainAct < lColGain && lpfl->idPlanet != -1) {
                    lppl = LpplFromId(lpfl->idPlanet);
                    if (lppl != 0 && lppl->iPlayer == lpfl->iPlayer) {
                        lColGain -= lColGainAct;
                        lppl->rgwtMin[3] += lColGain;
                        FSendPlrMsg(lpfl->iPlayer, idmBreedingActivitiesHaveOverflowedLivingSpaceColon, lpfl->id | 0x8000, lpfl->id, LOWORD(lColGain),
                                    HIWORD(lColGain), lpfl->idPlanet, 0, 0, 0);
                    }
                }
            }
        }
    }
    return;
}

void UpdateResearchStatus(int16_t fUsePool) {
    int16_t      mdAvail;
    int16_t      fRedoItAll;
    int16_t      iTechCur;
    int16_t      fUsePoolOrig;
    int16_t      iTechNext;
    int16_t      iT;
    int16_t      iItem;
    int16_t      fGeneral;
    int16_t      fChgNow;
    int16_t      i;
    int16_t      ibitCur;
    int32_t      rglFieldSpent[6];
    HullSlotType grbitCur;
    int16_t      cPlrAlive;
    int32_t      lSpent;
    PART         part;
    int32_t      l;
    int16_t      iTT;
    int32_t      l15pct;
    int16_t      iTechNext2;
    char         TechLevel;
    int16_t      jj;
    int16_t      iGoto;
    MessageId    idm;

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
        iTechCur = rgplr[i].iTechCur & 0xf;
        iTechNext = (int16_t)(int8_t)(rgplr[i].iTechCur >> 4);
        idPlayer = i;
        if (rgplr[i].fDead == 0) {
            cPlrAlive++;
        }
        do {
            fRedoItAll = 0;
            for (iT = 0; iT < 6; iT++) {
                lSpent = rgplr[i].rgResSpent[iT];
                fChgNow = 0;
                if (game.fSlowTech != 0) {
                    lSpent = (int32_t)(lSpent * 2);
                }
                if (iT == iTechCur && fUsePool != 0 && fGeneral < 2) {
                    if (fGeneral != 0) {
                        fRedoItAll = 1;
                        fGeneral = 2;
                        lSpent += (int32_t)((rgplr[i].lResLastYear + 1) / 2);
                        rglFieldSpent[iT] += (int32_t)((rgplr[i].lResLastYear + 1) / 2);
                        for (iTT = 0; iTT < 6; iTT++) {
                            if (iTT != iT) {
                                l15pct = (int32_t)((uint32_t)(rgplr[i].lResLastYear * 3) + 19) / 20;
                                if (game.fSlowTech != 0) {
                                    rgplr[i].rgResSpent[iTT] += (int32_t)(l15pct / 2);
                                } else {
                                    rgplr[i].rgResSpent[iTT] += l15pct;
                                }
                                rglFieldSpent[iTT] += l15pct;
                            }
                        }
                    } else {
                        lSpent += rgplr[i].lResLastYear;
                        rglFieldSpent[iT] += rgplr[i].lResLastYear;
                    }
                }
                while (rgplr[i].rgTech[iT] < 26 && (rgplr[i].fCrippled == 0 || rgplr[i].rgTech[iT] < 10) &&
                       (rgplr[i].fCheater == 0 || rgplr[i].rgTech[iT] < 10)) {
                    l = GetTechLevelCost(iT, rgplr[i].rgTech[iT] + 1, i);
                    if (l > lSpent || (rgplr[i].rgTech[iT] >= 26 && (rgplr[i].fCrippled != 0 || rgplr[i].rgTech[iT] >= 26)))
                        goto L_89a5;
                    iTechNext2 = iTechCur;
                    lSpent -= l;
                    rgplr[i].rgTech[iT]++;
                    TechLevel = rgplr[i].rgTech[iT];
                    if (TechLevel == 26 && iTechNext == 6) {
                        iTechNext = 7;
                    }
                    if (iTechCur == iT && iTechNext != 6) {
                        if (iTechNext != 7) {
                            iTechNext2 = iTechNext;
                        } else {
                            iTechNext2 = 0;
                            for (jj = 1; jj < 6; jj++) {
                                if (rgplr[i].rgTech[jj] < LOBYTE((int16_t)(((uint16_t)iTechNext2 & 0xff00) | ((uint16_t)rgplr[i].rgTech[iTechNext2] & 0xff)))) {
                                    iTechNext2 = jj;
                                }
                            }
                        }
                        fChgNow = 1;
                    }
                    FSendPlrMsg(i, fGeneral == 0 ? idmScientistsHaveCompletedResearchTechLevelWill : idmScientistsHaveCompletedResearchTechLevelPrimary,
                                gotoResearch, TechLevel, iT, iTechNext2, 0, 0, 0, 0);
                    grbitCur = hstEngine;
                    ibitCur = 0;
                    while (grbitCur != hstNone) {
                        if ((grbitCur & (hstEngine | hstScanner | hstShield | hstArmor | hstBeam | hstTorp | hstBomb | hstMining | hstMines | hstSpecialSB |
                                         hstSBHull | hstSpecialE | hstSpecialM | hstTerra | hstHull | hstPlanetary)) != 0) {
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
                                    case hstSBHull:
                                        idm = idmRecentBreakthroughHasAlsoGivenHullDesign;
                                        iGoto = -3;
                                        goto L_8798;
                                    case hstHull:
                                        idm = idmRecentBreakthroughHasAlsoGivenHullType;
                                        iGoto = -3;
                                        goto L_8798;
                                    case hstTerra:
                                        if (GetRaceGrbit(&rgplr[i], ibitRaceTT) != 0) {
                                            switch (iItem) {
                                            case iterraGravityTerraform3:
                                            case iterraTempTerraform3:
                                            case iterraRadiationTerraform3:
                                                iItem++;
                                                break;
                                            default:
                                                goto L_873a;
                                            }
                                            break;
                                        }
                                    default:
                                    L_873a:
                                        if (grbitCur == hstPlanetary && iItem >= iplanetarySDI && iItem <= iplanetaryNeutronShield) {
                                            idm = idmRecentBreakthroughHasAlsoTaughtHowBuild;
                                        } else if (grbitCur == hstPlanetary && iItem >= iplanetaryViewer50 && iItem <= iplanetarySnooper620X) {
                                            idm = idmRecentBreakthroughHasAlsoTaughtHowBuild2;
                                        } else {
                                            idm = idmRecentBreakthroughHasAlsoGivenBenefit;
                                        }
                                        iGoto = ibitCur << 8 | 0xc000 | iItem;
                                        goto L_8798;
                                    }
                                    continue;
                                L_8798:
                                    FSendPlrMsg(i, idm, iGoto, iT, grbitCur, iItem, 0, 0, 0, 0);
                                }
                                iItem++;
                            }
                        }
                        grbitCur *= 2;
                        ibitCur++;
                    }
                    if ((fUsePool != 0 || fChgNow != 0 || iTechNext == 7) && iTechNext != 6 && iT == iTechCur) {
                        if (iTechNext == 7) {
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
                            rgplr[i].rgResSpent[iT] = 0;
                            iTechCur = iTechNext;
                            iTechNext = 7;
                        } else {
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
                            rgplr[i].rgResSpent[iT] = 0;
                            iTechCur = iTechNext;
                            iTechNext = 6;
                        }
                        if (game.fSlowTech != 0) {
                            lSpent = (int32_t)((lSpent + 1) >> 1);
                        }
                        rgplr[i].rgResSpent[iTechCur] += lSpent;
                        fUsePool = 0;
                        fRedoItAll = 1;
                        break;
                    }
                }
                continue;
            L_89a5:
                if (game.fSlowTech != 0) {
                    lSpent = (int32_t)((lSpent + 1) >> 1);
                }
                rgplr[i].rgResSpent[iT] = lSpent;
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
                        lSpent = (int32_t)(rglFieldSpent[iT] / cPlrAlive) / 2;
                        if (lSpent > 1) {
                            fRedoItAll = 1;
                            FSendPlrMsg2(i, idmIntelligenceGatheringActivitiesCombinedSynergist, gotoResearch, iT, LOWORD(lSpent));
                            if (game.fSlowTech != 0) {
                                lSpent = (int32_t)((lSpent + 1) >> 1);
                            }
                            rgplr[i].rgResSpent[iT] += lSpent;
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
