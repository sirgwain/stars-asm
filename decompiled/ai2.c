#include "common.h"

uint8_t vrgISIshAip[19] = {0, 3, 6, 9, 12, 19, 26, 33, 40, 47, 54, 65, 76, 87, 98, 101, 105, 112, 117};
uint8_t vrgISAip[182] = {30, 31, 10, 30, 26, 4,  30, 26, 3, 30, 26, 2,  30, 0,  0,  13, 9,  18, 11, 30, 1,  1,  11, 9,  18, 11, 8,  10, 15, 7, 4,
                         6,  9,  8,  11, 13, 0,  0,  0,  9, 8,  10, 15, 7,  3,  6,  17, 8,  11, 13, 1,  1,  1,  17, 8,  12, 10, 6,  3,  5,  3, 7,
                         9,  20, 20, 8,  12, 10, 0,  0,  0, 0,  0,  9,  19, 11, 8,  12, 10, 6,  3,  4,  2,  7,  17, 20, 20, 8,  12, 10, 1,  1, 1,
                         1,  1,  17, 19, 11, 8,  16, 10, 8, 21, 23, 12, 8,  21, 23, 23, 23, 12, 10, 30, 10, 12, 25, 32, 8,  16, 10, 19};
uint8_t vrgAiISResOrder[18] = {134, 100, 69, 37, 164, 4, 135, 102, 71, 40, 166, 7, 140, 109, 73, 43, 167, 10};

void DoMaidAiTurn(PROD *rgprod) {
    int32_t rgResCost[4];
    int32_t rgResAvail[4];
    int16_t iroCur;

    iroCur = IroEnsureAi(0x0, 0, 0x0, game.turn >= 0x14 ? 15 : 0);
    fMarkedPlanets = 0;
    HandleBasicAiTasks(iroCur, rgprod, -1, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

int16_t FPotentISWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

    cEquiv = 0;
    for (ish = 11; ish <= 12; ish++) {
        cEquiv = cEquiv + lpfl->rgcsh[ish];
    }
    for (ish = 9; ish <= 10; ish++) {
        cEquiv = cEquiv + lpfl->rgcsh[ish] * 2;
    }
    if (iPotency >= 2) {
        if (cEquiv < vrgAiArmadaPotency[0]) {
            return 0;
        }
        return 1;
    }
    return 1;
}

void DoAutomitronAiTurn(PROD *rgprod) {
    int16_t  cExistCargo;
    uint16_t rgCosts[4];
    int32_t  rgResCost[4];
    int16_t  iLatestCruiser;
    int32_t  rgResAvail[4];
    FLEET   *lpflEnemy;
    ORDER    ord;
    uint8_t  rgRecycleShdef[16];
    PLANET  *lpplDest;
    int16_t  cplNegative;
    THING   *lpthWorm;
    int16_t  cFr;
    int16_t  idPlanDst;
    int16_t  cplBadGuy;
    PLANET  *lpplMac;
    PLANET  *lppl;
    PLANET  *lpplHome;
    FLEET   *lpfl;
    int16_t  ifl;
    int16_t  i;
    int16_t  cRes;
    int16_t  iroCur;
    FLEET   *lpflT;
    FLEET   *lpflAttack;
    uint8_t  b;
    int16_t  ishdefSBLatest;
    uint16_t cRecyclePeriod;
    uint16_t cplanCol;
    int16_t  iLatestCargo;
    int16_t  iLatestBomber;
    int16_t  j;
    int16_t  iLatestBattle;
    int32_t  l;
    PROD    *lpprod;
    int16_t  fWrite;
    int16_t  iPlanet;
    int16_t  id;
    PLANET  *t_merge_14d6_0001;
    PLANET  *t_merge_160c_0001;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0x0;
    iroCur = IroEnsureAi(vrgAiISResOrder, 18, &ishdefSBLatest, game.turn >= 0xa ? 20 : 0);
    if (game.turn <= 0x32) {
        if (game.turn > 0x1e) {
            MergeAllShdefs(16384);
        }
    } else {
        MergeAllShdefs(7692);
        MergeAllShdefs(64);
        MergeAllShdefs(16384);
    }
    j = 3;
    if (game.turn > 0x82) {
        j = j + (uint32_t)(game.turn - 0x78) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiArmadaPotency[0] = LOBYTE(j);
    vrgAiArmadaPotency[1] = LOBYTE((int32_t)(j & 0xff) / 0x2);
    j = 6;
    if (game.turn > 0x73) {
        j = j + (uint32_t)(game.turn - 0x64) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiArmadaPotency[2] = LOBYTE(j);
    vrgAiArmadaPotency[3] = LOBYTE(0x3 >= (int32_t)j / 2 - 0x1 ? (int32_t)j / 2 - 0x1 : 0x3);
    memset(rgRecycleShdef, 0, 0x10);
    if (game.turn >= 0x78) {
        cRecyclePeriod = game.turn >= 0xc8 ? 0x64 : 0x46;
    } else {
        cRecyclePeriod = 0x32;
    }
    CheckAiShdefStatus(11, 12, cRecyclePeriod, &iLatestCruiser, rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(4, 5, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(2, 3, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(9, 10, cRecyclePeriod, &iLatestBattle, rgRecycleShdef);
    if (game.turn > 0x3c) {
        SplitOutShdefs(rgRecycleShdef);
    }
    EnsureISShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1 && lppl->det >= 0x3 && PctPlanetOptValue(lppl, idPlayer) > 0) {
            cplanCol = cplanCol + 0x1;
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        vlpbAiPlanet[lppl->id * 16 + 9] = 0x1;
        if (lppl->iPlayer == idPlayer || lppl->iPlayer == -1) {
            if (lppl->iPlayer != idPlayer || PctPlanetDesirability(lppl, idPlayer) >= 0) {
                if (lppl->fStarbase != 0x0 && lppl->rgwtMin[3] >= 1500) {
                    ChangeMainObjSel(grobjPlanet, lppl->id);
                    InitProduction(rgprod);
                    fWrite = 0;
                    b = 0x0;
                    i = 0;
                    for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm);
                         lpprod++) {
                        i = i + 1;
                    }
                    if (i >= lpplProdGlob->iprodMac) {
                        cFr = (int32_t)rgplr[idPlayer].cPlanet / 10 <= RawLoad16((uint8_t *)vlpbAiData + 0x2) * 0x2 ? RawLoad16((uint8_t *)vlpbAiData + 0x2) * 2
                                                                                                                    : (int32_t)rgplr[idPlayer].cPlanet / 10;
                        if ((int16_t)rgplr[idPlayer].rgTech[2] >= 5 && iLatestCargo != -1 &&
                            (cExistCargo < cFr || (cExistCargo < (int32_t)(10 * cFr) / 0x7 && Random(4) == 0))) {
                            AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 1);
                            fWrite = 1;
                        }
                        if (cplanCol != 0x0 && rgshdef[1].cExist == 0x0 && game.turn > 0xa) {
                            AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                            fWrite = 1;
                        }
                        l = (uint32_t)(lppl->rgwtMin[3] * (int32_t)PctTrueMaxGrowth(idPlayer));
                        cRes = CResourcesAtPlanet(lppl, idPlayer);
                        if (rgshdef[6].fFree == 0x0 && Random(3) == 0) {
                            id = lppl->id;
                            cFr = 0;
                            ifl = 0;
                            while (1) {
                                if (ifl >= cFleet)
                                    goto L_08df;
                                lpfl = rglpfl[ifl];
                                if (rglpfl[ifl] == 0x0)
                                    goto L_08df;
                                if (lpfl->idPlanet == id && lpfl->rgcsh[6] > 0 && lpfl->iPlayer == idPlayer)
                                    break;
                                ifl = ifl + 1;
                            }
                            cFr = lpfl->rgcsh[6];
                        L_08df:
                            if ((cFr < 10 || (cFr < 17 && Random(8) == 0)) && Random(cFr * 2 + 1) == 0) {
                                AddItemToQueue(0x6, 0x3, grobjFleet, 1);
                                fWrite = 1;
                            }
                        }
                        if (iLatestBomber != -1) {
                            id = lppl->id;
                            ifl = 0;
                            while (1) {
                                if (ifl >= cFleet)
                                    goto L_0a25;
                                lpfl = rglpfl[ifl];
                                if (rglpfl[ifl] == 0x0)
                                    goto L_0a25;
                                if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentISWarFleet(lpfl, 2) != 0)
                                    break;
                                ifl = ifl + 1;
                            }
                            if (iLatestBomber != -1 && lpfl->rgcsh[2] + lpfl->rgcsh[3] >= vrgAiArmadaPotency[2]) {
                                AddItemToQueue(iLatestBomber, 0x4, grobjFleet, 1);
                                fWrite = 1;
                                goto FinishProd;
                            }
                        }
                    L_0a25:
                        if (iLatestCruiser != -1 && rgshdef[iLatestCruiser].cExist < (uint32_t)((int32_t)game.cPlanMax / 12 + 0x8)) {
                            GetResourcesAvailable(lppl, rgResAvail);
                            GetProdQCost(lppl, rgResCost);
                            for (i = 0; i < 4; i++) {
                                rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                                if (rgResAvail[i] < 0)
                                    goto FinishProd;
                            }
                            for (i = 0; i < 5; i++) {
                                GetTrueHullCost(idPlayer, &rgshdef[iLatestCruiser].hul, rgCosts);
                                for (j = 0; j < 4; j++) {
                                    rgResAvail[j] = rgResAvail[j] - (uint32_t)rgCosts[j];
                                    if (rgResAvail[j] < 0)
                                        goto FinishProd;
                                }
                                fWrite = 1;
                                AddItemToQueue(iLatestCruiser, 0x1, grobjFleet, 1);
                            }
                        }
                        if (iLatestBattle != -1 && rgshdef[iLatestBattle].cExist < (uint32_t)((int32_t)game.cPlanMax / 24 + 0x4)) {
                            GetResourcesAvailable(lppl, rgResAvail);
                            GetProdQCost(lppl, rgResCost);
                            for (i = 0; i < 4; i++) {
                                rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                                if (rgResAvail[i] < 0)
                                    goto FinishProd;
                            }
                            for (i = 0; i < 5; i++) {
                                GetTrueHullCost(idPlayer, &rgshdef[iLatestBattle].hul, rgCosts);
                                for (j = 0; j < 4; j++) {
                                    rgResAvail[j] = rgResAvail[j] - (uint32_t)rgCosts[j];
                                    if (rgResAvail[j] < 0)
                                        goto FinishProd;
                                }
                                fWrite = 1;
                                AddItemToQueue(iLatestBattle, 0x1, grobjFleet, 1);
                            }
                        }
                    FinishProd:
                        FinishProduction(fWrite);
                    } else {
                        FinishProduction(0);
                    }
                }
            } else {
                cplNegative = cplNegative + 1;
                vlpbAiPlanet[lppl->id * 16 + 2] = 0x1;
            }
        } else {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 0x1);
            if (PctPlanetOptValue(lppl, idPlayer) > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = 0x1;
            }
            cplBadGuy = cplBadGuy + 1;
        }
    }
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (FIsAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
            }
            lpfl->fMark = 0x0;
            if (FIsAiTransport(lpfl) == 0) {
                if (lpfl->rgcsh[1] == 0)
                    continue;
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst == -1)
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] == 0x0) {
                    lppl = LpplFromId(idPlanDst);
                    if (lppl != 0x0 && lppl->iPlayer != -1 && lppl->iPlayer != idPlayer)
                        goto LBlowAwayOrders;
                    continue;
                }
            } else {
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
            }
            if (idPlanDst == -1)
                continue;
            lppl = LpplFromId(idPlanDst);
            if (lppl != 0x0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                continue;
            if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0x0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                memset(&ord, 0, sizeof(ORDER));
                ord.pt = rgptPlan[idPlanDst];
                ord.grobj = grobjPlanet;
                ord.id = idPlanDst;
                ord.grTask = grTaskXfer;
                ord.fValidTask = 0x1;
                ord.txp.rgia[3].iAction = iActionUnloadAll;
                ChangeMainObjSel(grobjFleet, lpfl->id);
                if (sel.fl.lpplord->rgord[0].id != idPlanDst) {
                    sel.fl.lpplord->rgord[1] = ord;
                } else {
                    sel.fl.lpplord->rgord[0] = ord;
                }
                FLookupFleet(-1, &sel.fl);
                vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 0x3] | 0x80;
                continue;
            }
        LBlowAwayOrders:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.cord = 1;
            sel.fl.lpplord->iordMac = 0x1;
            FLookupFleet(-1, &sel.fl);
            ClearAiCurrentTask(lpfl, 0);
        } else {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        }
    }
    fMarkedPlanets = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->cord <= 1) {
                if (lpfl->rgcsh[6] != 0) {
                    if (lpfl->cord != 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone)
                        goto L_1162;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                    sel.fl.lpplord->rgord[0].tlm.cTime = 0x5;
                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 0x5;
                    FLookupFleet(-1, &sel.fl);
                    goto L_1162;
                }
                if (lpfl->rgcsh[1] == 0) {
                    if (FIsAiTransport(lpfl) != 0) {
                        lppl = lpPlanets;
                        lpplMac = lpPlanets + cPlanet;
                        for (; lppl < lpplMac && (lppl->iPlayer != idPlayer || lppl->fStarbase == 0x0); lppl++) {
                        }
                        t_merge_14d6_0001 = lppl == lpplMac ? 0x0 : lppl;
                        lpplHome = t_merge_14d6_0001;
                        if (lpplHome != 0x0) {
                            lppl = 0x0;
                            for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                                for (j = 0; j < RawLoad16(vlpbAiData + (i * 20 + 6)) && RawLoad16(vlpbAiData + (i * 20 + j * 2 + 8)) != lpfl->id; j++) {
                                }
                                if (j < RawLoad16(vlpbAiData + (i * 20 + 6)))
                                    break;
                            }
                            if (i < RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                                lppl = LpplFromId(RawLoad16(vlpbAiData + (i * 20 + 4)));
                            }
                            t_merge_160c_0001 = lppl == 0x0 ? lpplHome : lppl;
                            IdTargetFreighter(lpfl, t_merge_160c_0001);
                            goto L_1162;
                        }
                        break;
                    }
                    if (lpfl->rgcsh[2] != 0 || lpfl->rgcsh[3] != 0) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->idPlanet == -1) {
                            lppl = lpplHome;
                        } else {
                            lppl = LpplFromId(lpfl->idPlanet);
                            if (lppl->iPlayer != idPlayer) {
                                if (lppl->iPlayer != -1) {
                                    lpflT = lpflEnemy;
                                    while (1) {
                                        if (lpflT == 0x0)
                                            goto L_1162;
                                        if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                            break;
                                        lpflT = lpflT->lpflNext;
                                    }
                                }
                            } else {
                                if (lppl->fStarbase != 0x0 && ((lpfl->rgcsh[2] < 2 && lpfl->rgcsh[3] < 2) || (lpfl->rgcsh[9] < 3 && lpfl->rgcsh[10] < 3)))
                                    goto L_1162;
                                FLookupFleet(lpfl->id, &sel.fl);
                            }
                        }
                        if (game.fAisBand == 0x0) {
                            lpplDest = 0x0;
                        } else {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
                        }
                        if (lpplDest == 0x0) {
                            lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
                        }
                        lppl = lpplDest;
                        if (lppl != 0x0) {
                            vlpbAiPlanet[lppl->id * 16 + 10] = vlpbAiPlanet[lppl->id * 16 + 0xa] | 0x80;
                            ord.id = lppl->id;
                            ord.grobj = grobjPlanet;
                            ord.pt = rgptPlan[lppl->id];
                            ord.grTask = grTaskNone;
                            ord.fValidTask = 0x1;
                            ord.iWarp = 0x4;
                            FMoveAiFleet(lpfl, &ord, 0);
                            goto L_1162;
                        }
                        goto L_1162;
                    }
                    if (lpfl->rgcsh[0] == 0)
                        goto L_1162;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (rgshdef[0].hul.rghs[0].iItem >= 0xa || lpfl->rgwtMin[4] >= 2) {
                        IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                        goto L_1162;
                    }
                } else if (rgshdef[1].hul.ihuldef == ihuldefMediumFreighter) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if ((lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.rgwtMin[3] >= 200) || lpfl->rgwtMin[3] != 0) {
                        lpthWorm = 0x0;
                        idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
                        if (lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 150);
                            FLookupFleet(lpfl->id, &sel.fl);
                        }
                        if (idPlanDst == -1)
                            goto L_1162;
                        FColonizeAiFleet(lpfl, idPlanDst);
                        vlpbAiPlanet[idPlanDst * 16 + 15] = 0x4;
                        goto L_1162;
                    }
                    if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0x0) ||
                        (rgshdef[1].hul.rghs[0].iItem >= 0x2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                        goto L_1162;
                }
            } else if (rgshdef[0].hul.rghs[0].iItem >= 0xa || lpfl->rgwtMin[4] >= 2) {
                goto L_1162;
            }
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
            FLookupFleet(-1, &sel.fl);
        }
    L_1162:;
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureISShdefs(int16_t iroCur) {
    SHDEF   shdef;
    int16_t i;

    if (rgshdef[4].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[2] >= 5) {
        FCreateAiShdef(4, 1, &vrgISAip[vrgISIshAip[14]]);
    }
    if (rgshdef[5].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7) {
        FCreateAiShdef(5, 3, &vrgISAip[vrgISIshAip[18]]);
    }
    if (rgshdef[14].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 4 && (int16_t)rgplr[idPlayer].rgTech[2] >= 5) {
        for (i = 0; i < 4 && FCreateAiShdef(14, 6, &vrgISAip[vrgISIshAip[Random(1) + 4]]) == 0; i++) {
        }
    }
    if (rgshdef[1].fFree != 0x0 || rgshdef[1].cExist == 0x0) {
        if (rgshdef[1].fFree == 0x0) {
            shdef = rgshdef[1];
            shdef.fFree = 0x1;
            FChangeAiShdef(&shdef, 1);
        }
        FCreateAiShdef(1, 1, &vrgISAip[vrgISIshAip[0]]);
    }
    if (rgshdef[0].fFree != 0x0 || rgshdef[0].cExist == 0x0) {
        if (rgshdef[0].fFree == 0x0) {
            shdef = rgshdef[0];
            shdef.fFree = 0x1;
            FChangeAiShdef(&shdef, 0);
        }
        FCreateAiShdef(0, 4, &vrgISAip[vrgISIshAip[1]]);
    }
    if (rgshdef[6].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[3] >= 4 && (int16_t)rgplr[idPlayer].rgTech[2] >= 5 &&
        (int16_t)rgplr[idPlayer].rgTech[5] >= 6) {
        FCreateAiShdef(6, 11, &vrgISAip[vrgISIshAip[17]]);
    }
    if (rgshdef[2].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 8 && (int16_t)rgplr[idPlayer].rgTech[4] >= 7 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 6 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7) {
        FCreateAiShdef(2, 17, &vrgISAip[vrgISIshAip[15]]);
    }
    if (rgshdef[3].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 11 && (int16_t)rgplr[idPlayer].rgTech[4] >= 12 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 15 && (int16_t)rgplr[idPlayer].rgTech[2] >= 9) {
        FCreateAiShdef(3, 19, &vrgISAip[vrgISIshAip[16]]);
    }
    if (rgshdef[9].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 13 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(9, 9, &vrgISAip[vrgISIshAip[Random(4) + 10]]) == 0; i++) {
        }
    }
    return;
}

void DoRototillAiTurn(PROD *rgprod) {
    int32_t  rgResCost[4];
    int32_t  rgResAvail[4];
    FLEET   *lpflEnemy;
    ORDER    ord;
    PLANET  *lpplDest;
    int16_t  cplNegative;
    THING   *lpthWorm;
    int16_t  fColonyShipInQueue;
    int16_t  idPlanDst;
    int16_t  cplBadGuy;
    PLANET  *lpplMac;
    PLANET  *lppl;
    PLANET  *lpplHome;
    FLEET   *lpfl;
    int16_t  ifl;
    int16_t  i;
    int16_t  iroCur;
    FLEET   *lpflT;
    FLEET   *lpflAttack;
    uint8_t  b;
    int16_t  ishdefSBLatest;
    int16_t  fBomberInQueue;
    uint16_t cplanCol;
    int16_t  j;
    PROD    *lpprod;
    int16_t  fWrite;
    int16_t  iPlanet;
    uint8_t  bT;
    PLANET  *t_merge_2bd5_0001;
    PLANET  *t_merge_2d04_0001;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0x0;
    fColonyShipInQueue = 0;
    fBomberInQueue = 0;
    iroCur = IroEnsureAi(0x0, 0, &ishdefSBLatest, game.turn >= 0x14 ? 15 : 0);
    EnsureCAShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1 && lppl->det >= 0x3 && PctPlanetOptValue(lppl, idPlayer) > 0) {
            cplanCol = cplanCol + 0x1;
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        vlpbAiPlanet[lppl->id * 16 + 9] = 0x1;
        if (lppl->iPlayer == -1 && lppl->det >= 0x3) {
            b = 0x0;
            for (i = 0; i < 3; i++) {
                if (lppl->rgMinConc[i] <= 0x42) {
                    bT = LOBYTE((int32_t)lppl->rgMinConc[i] / 0x2);
                } else {
                    bT = 0x4b;
                }
                b = b + LOBYTE(bT);
            }
            if ((b & 0x80) != 0x0) {
                b = 0x7f;
            }
            vlpbAiPlanet[lppl->id * 16 + 1] = b;
        }
        if (lppl->iPlayer == idPlayer || lppl->iPlayer == -1) {
            if (lppl->iPlayer != idPlayer || PctPlanetDesirability(lppl, idPlayer) >= 0) {
                if (lppl->fStarbase != 0x0 && lppl->rgwtMin[3] >= 1000) {
                    ChangeMainObjSel(grobjPlanet, lppl->id);
                    InitProduction(rgprod);
                    fWrite = 0;
                    b = 0x0;
                    i = 0;
                    for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem <= iobjPacketGerm);
                         lpprod++) {
                        i = i + 1;
                    }
                    if (i >= lpplProdGlob->iprodMac) {
                        if (game.turn != 0x0) {
                            if (fColonyShipInQueue == 0 && (rgshdef[1].cExist == 0x0 || rgshdef[1].cExist + 0x1 < (uint32_t)cplanCol)) {
                                fColonyShipInQueue = 1;
                                AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                                fWrite = 1;
                            }
                        } else {
                            AddItemToQueue(0x0, 0x1, grobjFleet, 1);
                            AddItemToQueue(0x0, 0x1, grobjFleet, 1);
                        }
                        FinishProduction(fWrite);
                    } else {
                        FinishProduction(0);
                    }
                }
            } else {
                cplNegative = cplNegative + 1;
                vlpbAiPlanet[lppl->id * 16 + 2] = 0x1;
            }
        } else {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 0x1);
            if (PctPlanetOptValue(lppl, idPlayer) > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = 0x1;
            }
            cplBadGuy = cplBadGuy + 1;
        }
    }
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (FIsTurinDroneAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
            }
            lpfl->fMark = 0x0;
            if ((lpfl->rgcsh[7] != 0 || lpfl->rgcsh[8] != 0) && lpfl->cord >= 1) {
                if (lpfl->idPlanet == -1) {
                    if (lpfl->cord <= 1)
                        continue;
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                } else {
                    if (LpplFromId(lpfl->idPlanet)->iPlayer != -1)
                        goto LBlowAwayOrders;
                    idPlanDst = lpfl->idPlanet;
                }
                vlpbAiPlanet[idPlanDst * 16 + 1] = vlpbAiPlanet[idPlanDst * 16 + 0x1] | 0x80;
                continue;
            }
            if (FIsAiTransport(lpfl) == 0) {
                if (lpfl->rgcsh[1] == 0)
                    continue;
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst == -1)
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] == 0x0) {
                    lppl = LpplFromId(idPlanDst);
                    if (lppl != 0x0 && lppl->iPlayer != -1 && lppl->iPlayer != idPlayer)
                        goto LBlowAwayOrders;
                    continue;
                }
            } else {
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
            }
            if (idPlanDst == -1)
                continue;
            lppl = LpplFromId(idPlanDst);
            if (lppl != 0x0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                continue;
            if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0x0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh) {
                memset(&ord, 0, sizeof(ORDER));
                ord.pt = rgptPlan[idPlanDst];
                ord.grobj = grobjPlanet;
                ord.id = idPlanDst;
                ord.grTask = grTaskXfer;
                ord.fValidTask = 0x1;
                ord.txp.rgia[3].iAction = iActionUnloadAll;
                ChangeMainObjSel(grobjFleet, lpfl->id);
                if (sel.fl.lpplord->rgord[0].id != idPlanDst) {
                    sel.fl.lpplord->rgord[1] = ord;
                } else {
                    sel.fl.lpplord->rgord[0] = ord;
                }
                FLookupFleet(-1, &sel.fl);
                vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 0x3] | 0x80;
                continue;
            }
        LBlowAwayOrders:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.cord = 1;
            sel.fl.lpplord->iordMac = 0x1;
            FLookupFleet(-1, &sel.fl);
            ClearAiCurrentTask(lpfl, 0);
        } else {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        }
    }
    fMarkedPlanets = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->rgcsh[7] == 0 && lpfl->rgcsh[8] == 0) {
                if (lpfl->cord <= 1) {
                    if (lpfl->rgcsh[1] == 0) {
                        if (FIsAiTransport(lpfl) != 0) {
                            lppl = lpPlanets;
                            lpplMac = lpPlanets + cPlanet;
                            for (; lppl < lpplMac && (lppl->iPlayer != idPlayer || lppl->fStarbase == 0x0); lppl++) {
                            }
                            t_merge_2bd5_0001 = lppl == lpplMac ? 0x0 : lppl;
                            lpplHome = t_merge_2bd5_0001;
                            if (lpplHome != 0x0) {
                                lppl = 0x0;
                                for (i = 0; i < RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                                    for (j = 0; j < RawLoad16(vlpbAiData + (i * 20 + 6)) && RawLoad16(vlpbAiData + (i * 20 + j * 2 + 8)) != lpfl->id; j++) {
                                    }
                                    if (j < RawLoad16(vlpbAiData + (i * 20 + 6)))
                                        break;
                                }
                                if (i < RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                                    lppl = LpplFromId(RawLoad16(vlpbAiData + (i * 20 + 4)));
                                }
                                t_merge_2d04_0001 = lppl == 0x0 ? lpplHome : lppl;
                                IdTargetFreighter(lpfl, t_merge_2d04_0001);
                                goto L_279b;
                            }
                            break;
                        }
                        if (lpfl->rgcsh[13] != 0 || lpfl->rgcsh[14] != 0) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            if (lpfl->idPlanet == -1) {
                                lppl = lpplHome;
                            } else {
                                lppl = LpplFromId(lpfl->idPlanet);
                                if (lppl->iPlayer != idPlayer) {
                                    if (lppl->iPlayer != -1) {
                                        lpflT = lpflEnemy;
                                        while (1) {
                                            if (lpflT == 0x0)
                                                goto L_279b;
                                            if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                                break;
                                            lpflT = lpflT->lpflNext;
                                        }
                                    }
                                } else {
                                    if (lppl->fStarbase != 0x0 && lpfl->rgcsh[13] < 2 && lpfl->rgcsh[14] < 2)
                                        goto L_279b;
                                    FLookupFleet(lpfl->id, &sel.fl);
                                }
                            }
                            if (game.fAisBand == 0x0) {
                                lpplDest = 0x0;
                            } else {
                                lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
                            }
                            if (lpplDest == 0x0) {
                                lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
                            }
                            lppl = lpplDest;
                            if (lppl != 0x0) {
                                vlpbAiPlanet[lppl->id * 16 + 10] = vlpbAiPlanet[lppl->id * 16 + 0xa] | 0x80;
                                ord.id = lppl->id;
                                ord.grobj = grobjPlanet;
                                ord.pt = rgptPlan[lppl->id];
                                ord.grTask = grTaskNone;
                                ord.fValidTask = 0x1;
                                ord.iWarp = 0x4;
                                FMoveAiFleet(lpfl, &ord, 0);
                                goto L_279b;
                            }
                            goto L_279b;
                        }
                        if (lpfl->rgcsh[0] == 0)
                            goto L_279b;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->rgcsh[0] == 0 || rgshdef[0].hul.rghs[0].iItem != 0x1 || lpfl->rgwtMin[4] >= 2) {
                            IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                            goto L_279b;
                        }
                    } else {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if ((lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.rgwtMin[3] >= 50) || lpfl->rgwtMin[3] != 0) {
                            lpthWorm = 0x0;
                            idPlanDst = IdNearestColonizablePlanet(lpfl, rgshdef[1].hul.rghs[0].iItem <= 0x1 ? 0x0 : &lpthWorm);
                            if (lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer) {
                                ChangeMainObjSel(grobjFleet, lpfl->id);
                                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 25);
                                FLookupFleet(lpfl->id, &sel.fl);
                            }
                            if (idPlanDst == -1) {
                                if (lpthWorm != 0x0) {
                                    FGotoWormholeAiFleet(lpfl, lpthWorm);
                                    goto L_279b;
                                }
                                goto L_279b;
                            }
                            FColonizeAiFleet(lpfl, idPlanDst);
                            vlpbAiPlanet[idPlanDst * 16 + 15] = 0x4;
                            goto L_279b;
                        }
                        if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0x0) ||
                            (rgshdef[1].hul.rghs[0].iItem >= 0x2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                            goto L_279b;
                    }
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                    FLookupFleet(-1, &sel.fl);
                }
            } else if (lpfl->idPlanet != -1) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                b = vlpbAiPlanet[lpfl->idPlanet * 16 + 1];
                if (b < 0x4) {
                    lppl = LpplFindBestEnum(&sel.pl, FEnumCalcMinerDest);
                    if (lppl != 0x0) {
                        ord.id = lppl->id;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[lppl->id];
                        ord.grTask = grTaskMine;
                        ord.fValidTask = 0x1;
                        ord.iWarp = 0x6;
                        FMoveAiFleet(lpfl, &ord, 1);
                        vlpbAiPlanet[lppl->id * 16 + 1] = vlpbAiPlanet[lppl->id * 16 + 0x1] | 0x80;
                        vlpbAiPlanet[lpfl->idPlanet * 16 + 1] = vlpbAiPlanet[lpfl->idPlanet * 16 + 0x1] & 0x80;
                    }
                }
            }
        }
    L_279b:;
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureCAShdefs(int16_t iroCur) { return; }
