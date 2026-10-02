#include "common.h"

uint8_t vrgAiRobotoidResOrder[36] = {
    aiResearchPropulsion2,     aiResearchConstruction3,   aiResearchWeapons3,      aiResearchConstruction4,  aiResearchEnergy2,      aiResearchElectronics3,
    aiResearchPropulsion6,     aiResearchWeapons5,        aiResearchConstruction6, aiResearchBiotechnology4, aiResearchElectronics5, aiResearchEnergy6,
    aiResearchWeapons7,        aiResearchConstruction10,  aiResearchEnergy6,       aiResearchElectronics7,   aiResearchWeapons10,    aiResearchPropulsion9,
    aiResearchPropulsion12,    aiResearchConstruction13,  aiResearchWeapons14,     aiResearchConstruction16, aiResearchEnergy9,      aiResearchElectronics10,
    aiResearchPropulsion16,    aiResearchBiotechnology10, aiResearchEnergy15,      aiResearchWeapons20,      aiResearchPropulsion20, aiResearchElectronics16,
    aiResearchBiotechnology12, aiResearchWeapons24,       aiResearchElectronics19, aiResearchConstruction24, aiResearchEnergy22,     aiResearchConstruction26};
uint8_t vrgTDAip[141] = {8,  31, 8,  26, 0,  37, 8,  0,  0,  0,  9,  18, 11, 8,  1,  1,  11, 9, 18, 11, 8,  37, 15, 7,  4,  6,  9,  8,
                         11, 13, 0,  0,  0,  9,  8,  37, 15, 7,  3,  6,  17, 8,  11, 13, 1,  1, 1,  17, 8,  12, 37, 6,  3,  5,  3,  7,
                         9,  20, 20, 8,  12, 37, 0,  0,  0,  0,  0,  9,  19, 11, 8,  12, 37, 6, 3,  4,  2,  7,  17, 20, 20, 8,  12, 37,
                         1,  1,  1,  1,  1,  17, 19, 11, 8,  10, 16, 27, 17, 0,  13, 39, 11, 8, 21, 22, 12, 39, 8,  10, 12, 25, 25, 8,
                         37, 17, 0,  13, 11, 16, 27, 8,  13, 28, 28, 8,  13, 28, 28, 28, 28, 8, 37, 16, 26, 17, 10, 13, 19, 39};
uint8_t vrgRobAip[301] = {
    8,  4,  10, 10, 13, 9,  9,  8,  10, 5,  4,  13, 12, 15, 8,  10, 4,  7,  13, 12, 14, 8,  10, 3,  3,  13, 12, 14, 8,  9,  1,  1,  11, 11, 12, 8,  0,  9,
    10, 13, 11, 12, 8,  9,  0,  0,  10, 11, 12, 8,  1,  9,  12, 12, 11, 11, 8,  10, 16, 16, 3,  12, 2,  8,  16, 4,  3,  14, 12, 13, 8,  3,  16, 10, 16, 12,
    14, 8,  16, 1,  11, 12, 10, 10, 8,  16, 11, 12, 16, 1,  0,  8,  10, 16, 16, 11, 0,  0,  8,  10, 15, 4,  4,  8,  9,  11, 0,  0,  8,  4,  4,  4,  17, 18,
    19, 8,  3,  3,  14, 17, 18, 19, 8,  4,  3,  2,  17, 18, 20, 8,  4,  4,  5,  17, 18, 20, 8,  0,  0,  10, 17, 18, 19, 8,  0,  0,  11, 17, 18, 19, 8,  1,
    1,  11, 17, 18, 19, 8,  1,  1,  11, 17, 18, 11, 24, 21, 23, 23, 23, 12, 10, 24, 21, 22, 22, 22, 12, 10, 24, 26, 25, 10, 8,  11, 10, 1,  1,  1,  1,  2,
    9,  11, 19, 8,  13, 10, 1,  1,  0,  0,  0,  9,  11, 19, 8,  13, 10, 0,  0,  1,  1,  0,  9,  11, 19, 8,  13, 10, 0,  0,  0,  0,  3,  9,  11, 19, 8,  13,
    10, 4,  4,  4,  4,  4,  9,  20, 19, 8,  13, 10, 4,  3,  3,  7,  2,  9,  20, 19, 8,  13, 10, 2,  3,  7,  7,  3,  9,  20, 19, 8,  13, 10, 4,  4,  3,  3,
    5,  9,  20, 19, 8,  33, 10, 17, 12, 33, 33, 8,  13, 10, 33, 33, 33, 33, 33, 17, 20, 19, 8,  10, 10, 7,  5,  20, 20, 4,  4,  19, 4,  2,  3};
uint8_t  vrgTDIshAip[19] = {0, 2, 6, 13, 20, 27, 34, 41, 48, 59, 70, 81, 92, 101, 106, 111, 119, 123, 129};
uint8_t  vrgAiTurinDroneResOrder[31] = {66, 100, 164, 4,  37,  70, 102, 40,  6,  134, 73,  167, 104, 136, 165, 105,
                                        7,  138, 42,  76, 107, 10, 44,  141, 80, 46,  111, 142, 170, 48,  14};
uint16_t vrgRobIshAip[38] = {0,   7,   14,  21,  28,  35,  42,  49,  56,  63,  70,  77,  84,  91,  98,  103, 108, 115, 122,
                             129, 136, 143, 150, 157, 164, 171, 178, 182, 193, 204, 215, 226, 237, 248, 259, 270, 277, 288};

void DoAiTurn(int16_t iPlayer, uint16_t wMdPlr) {
    char    szExt[4];
    PROD    rgprod[64];
    int16_t idSav;

    idSav = idPlayer;
    fAi = 1;
    _wsprintf(szExt, MPCTD, iPlayer + 1);
    DestroyCurGame();
    if (FLoadGame(szBase, szExt) == 0) {
        idPlayer = idSav;
        fAi = 0;
    } else {
        if (rgplr[idPlayer].fDead == 0) {
            vlpbAiPlanet = LpAlloc(game.cPlanMax * 16, htMisc);
            vrglpplAi = LpAlloc(game.cPlanMax * sizeof(PLANET *), htMisc);
            if (vlpbAiData == 0) {
                vlpbAiData = LpAlloc(0x2000, htMisc);
                if (vlpbAiData != 0) {
                    RawStore16(vlpbAiData, 2);
                }
            }
            if (vlpbAiPlanet != 0 && vlpbAiData != 0 && vrglpplAi != 0) {
                fmemset(vlpbAiPlanet, 0, game.cPlanMax * 16);
                ComputeShdefPowers();
                MarkPlanetsUnderAttack();
                IncreaseAIMinefieldSizes();
                InitRandomPlanetList();
                if (wMdPlr != 0xffff) {
                    rgplr[iPlayer].wMdPlr = wMdPlr;
                }
                if (rgplr[iPlayer].idAi <= idAiMaid) {
                    switch (rgplr[iPlayer].idAi) {
                    case idAiRobotoid:
                        DoRobotoidAiTurn(rgprod);
                        break;
                    case idAiCybertron:
                        DoCyberAiTurn(rgprod);
                        break;
                    case idAiMacinti:
                        DoMacintiAiTurn(rgprod);
                        break;
                    case idAiTurinDrone:
                        DoTurinDroneAiTurn(rgprod);
                        break;
                    case idAiMaid:
                        DoMaidAiTurn(rgprod);
                        break;
                    case idAiAutomitron:
                        DoAutomitronAiTurn(rgprod);
                        break;
                    case idAiRototill:
                        DoRototillAiTurn(rgprod);
                    }
                }
            }
        }
        FWriteLogFile(szBase, iPlayer);
        FWriteHistFile(iPlayer);
        if (vrglpplAi != 0) {
            FreeLp(vrglpplAi, htMisc);
            vrglpplAi = NULL;
        }
        if (vlpbAiData != 0) {
            FreeLp(vlpbAiPlanet, htMisc);
            FreeLp(vlpbAiData, htMisc);
            vlpbAiPlanet = NULL;
            vlpbAiData = NULL;
        }
        idPlayer = idSav;
        fAi = 0;
    }
    return;
}

void DoRobotoidAiTurn(PROD *rgprod) {
    int32_t  rgResCost[4];
    FLEET   *lpflEnemy;
    int32_t  rgResAvail[4];
    int16_t  cExistCargo;
    int16_t  cFlDestroyers;
    int16_t  iLatestDestroyer;
    int16_t  cColFleet;
    THING   *lpthWorm;
    int16_t  idPlanDst;
    int16_t  j;
    uint8_t  rgRecycleShdef[16];
    int16_t  fShouldColonize;
    PLANET  *lppl;
    int16_t  ifl;
    int16_t  i;
    uint16_t rgCosts[4];
    FLEET   *lpflAttack;
    FLEET   *lpfl;
    PLANET  *lpplHome;
    int16_t  cRes;
    int16_t  iroCur;
    int16_t  iAiLvl;
    int16_t  iLatestCargo;
    int16_t  ipl;
    int16_t  fTonsOfMinerals;
    int16_t  ishdefSBLatest;
    PLANET  *lpplMac;
    int16_t  iLatestMeta;
    uint16_t cRecyclePeriod;
    int16_t  cFr;
    int16_t  iLatestBattle;
    int16_t  iPlanet;
    int16_t  iLatestBomber;
    int32_t  l;
    int16_t  fWrite;
    PROD    *lpprod;
    uint8_t  rgRecycleSBShdef[16];
    int16_t  id;
    int16_t  iLatest;
    int16_t  dy;
    int32_t  lDist;
    int16_t  dx;
    ORDER    ord;
    uint8_t *lpb;
    PLANET  *lpplDrop;

    iAiLvl = rgplr[idPlayer].lvlAi;
    iPlanet = rgplr[idPlayer].idPlanetHome;
    iroCur = IroEnsureAi((uint8_t *)vrgAiRobotoidResOrder, 36, &ishdefSBLatest, game.turn >= 10 ? 15 : 0);
    if (game.turn > 50) {
        MergeAllShdefs(1788);
        MergeAllShdefs(1);
        MergeAllShdefs(-16384);
    }
    j = 4;
    if (game.turn > 130) {
        j += (uint32_t)(game.turn - 120) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiArmadaPotency[0] = LOBYTE(j);
    vrgAiArmadaPotency[1] = LOBYTE((int16_t)(j & 0xff) / 2);
    j = 6;
    if (game.turn > 115) {
        j += (uint32_t)(game.turn - 100) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiArmadaPotency[2] = LOBYTE(j);
    vrgAiArmadaPotency[3] = LOBYTE(3 >= j / 2 - 1 ? j / 2 - 1 : 3);
    memset(rgRecycleShdef, 0, 16);
    if (game.turn < 120) {
        cRecyclePeriod = 50;
    } else {
        cRecyclePeriod = game.turn >= 200 ? 100 : 70;
    }
    CheckAiShdefStatus(14, 15, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    for (i = 14; i <= 15; i++) {
        if (rgRecycleShdef[i] != 0 && rgshdef[i].fFree == 0 && rgshdef[i].hul.ihuldef == ihuldefNubian) {
            rgRecycleShdef[i] = 0;
        }
    }
    cExistCargo = CheckAiShdefStatus(11, 13, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(9, 10, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(2, 5, cRecyclePeriod, &iLatestMeta, rgRecycleShdef);
    CheckAiShdefStatus(6, 7, (uint32_t)(3 * cRecyclePeriod) / 2, &iLatestBattle, rgRecycleShdef);
    if (game.turn > 80) {
        SplitOutShdefs(rgRecycleShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[0] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[1] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[13] = 2;
        rgRecycleSBShdef[12] = 2;
        rgRecycleSBShdef[11] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
    }
    EnsureRobotoidShdefs();
    cFlDestroyers = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer && iLatestDestroyer != -1 && (lpfl->rgcsh[14] != 0 || lpfl->rgcsh[15] != 0)) {
            cFlDestroyers++;
        }
    }
    fShouldColonize = FShouldWeBuildColonizers(&cColFleet);
    UpdateProgressGauge(progressStep4);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            i = lppl->uPopGuess / 250 + 1;
            if (i > 6) {
                i = 6;
            }
            if (lppl->fStarbase != 0) {
                i++;
            }
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE(i);
            vlpbAiPlanet[lppl->id * 16 + 9] = 1;
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (lppl->fStarbase != 0 && lppl->rgwtMin[3] >= 200) {
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            fWrite = 0;
            i = 0;
            for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm); lpprod++) {
                i++;
            }
            if (i < lpplProdGlob->iprodMac) {
                FinishProduction(0);
            } else {
                cFr = rgplr[idPlayer].cPlanet / 8 <= RawLoad16((uint8_t *)vlpbAiData + 0x2) * 4 ? RawLoad16((uint8_t *)vlpbAiData + 0x2) * 4
                                                                                                : rgplr[idPlayer].cPlanet / 8;
                if (iLatestCargo != -1 && (cExistCargo < (int16_t)(cFr * 8) / 10 || (cExistCargo < cFr && Random(3) == 0))) {
                    AddItemToQueue(iLatestCargo, 1, grobjFleet, addItemEnd);
                    fWrite = 1;
                }
                if ((fShouldColonize != 0 || (cColFleet <= 25 && Random(RawLoad16((uint8_t *)vlpbAiData + 0x2) * 8) == 0)) && game.turn >= 5) {
                    if (game.turn <= 20) {
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                    }
                    AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                    fWrite = 1;
                    l = (uint32_t)(lppl->rgwtMin[3] * PctTrueMaxGrowth(idPlayer));
                    cRes = CResourcesAtPlanet(lppl, idPlayer);
                    if (l > 2300 && cRes > 35 && iAiLvl > 0) {
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        if (l > 3600 && cRes > 50 && iAiLvl > 1) {
                            AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        }
                    }
                }
                if (rgshdef[0].hul.ihuldef == ihuldefFrigate && Random(4) == 0) {
                    id = lppl->id;
                    cFr = 0;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->idPlanet == id && lpfl->rgcsh[0] > 0 && lpfl->iPlayer == idPlayer) {
                            cFr = lpfl->rgcsh[0];
                            break;
                        }
                    }
                    if ((cFr < 10 || (cFr < 17 && Random(10) == 0)) && Random(cFr * 2 + 1) == 0) {
                        AddItemToQueue(0, 4, grobjFleet, addItemEnd);
                        fWrite = 1;
                    }
                }
                for (i = 0; i <= 2 && lppl->rgwtMin[i] >= 5000; i++) {
                }
                fTonsOfMinerals = i == 2 ? 1 : 0;
                if (iLatestBomber != -1) {
                    id = lppl->id;
                    for (ifl = 0; ifl < cFleet; ifl++) {
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0)
                            break;
                        if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentRobWarFleet(lpfl, 2) != 0) {
                            if (iLatestBomber == -1 || lpfl->rgcsh[9] + lpfl->rgcsh[10] >= vrgAiArmadaPotency[2])
                                break;
                            AddItemToQueue(iLatestBomber, fTonsOfMinerals == 0 ? 4 : 6, grobjFleet, addItemEnd);
                            fWrite = 1;
                            goto FinishProd;
                        }
                    }
                }
                if (iLatestMeta != -1 && (rgshdef[iLatestMeta].cExist < (uint32_t)(game.cPlanMax / 7 + 6) || Random(2) != 0)) {
                    if (iLatestBattle != -1 && Random(2) == 0) {
                        iLatest = iLatestBattle;
                    } else {
                        iLatest = iLatestMeta;
                    }
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] -= rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    GetTrueHullCost(idPlayer, &rgshdef[iLatest].hul, rgCosts);
                    for (j = 0; j < 4; j++) {
                        rgResAvail[j] -= (uint32_t)((uint32_t)(3 * rgCosts[j]) / 5);
                        if (rgResAvail[j] < 0)
                            goto TryShip3;
                    }
                    AddItemToQueue(iLatest, fTonsOfMinerals == 0 ? 1 : 5, grobjFleet, addItemEnd);
                    fWrite = 1;
                }
            TryShip3:
                if (iLatestDestroyer != -1 && rgshdef[iLatestDestroyer].cExist < (uint32_t)(game.cPlanMax / 12 + 8)) {
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] -= rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    for (i = 0; i < 5; i++) {
                        GetTrueHullCost(idPlayer, &rgshdef[iLatestDestroyer].hul, rgCosts);
                        for (j = 0; j < 4; j++) {
                            rgResAvail[j] -= (uint32_t)rgCosts[j];
                            if (rgResAvail[j] < 0)
                                goto FinishProd;
                        }
                        fWrite = 1;
                        AddItemToQueue(iLatestDestroyer, 1, grobjFleet, addItemEnd);
                    }
                }
            FinishProd:
                FinishProduction(fWrite);
            }
        }
    }
    UpdateProgressGauge(progressStep4);
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjThing) {
            dx = lpfl->pt.x - lpfl->lpplord->rgord[1].pt.x;
            dy = lpfl->pt.y - lpfl->lpplord->rgord[1].pt.y;
            lDist = (uint32_t)(dx * dx) + (uint32_t)(dy * dy);
            if (lDist > 40000) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.cord = 1;
                sel.fl.lpplord->iordMac = 1;
                FLookupFleet(-1, &sel.fl);
            }
        }
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else {
            if (lpfl->rgcsh[0] > 0 && game.turn > 40 && lpfl->cord == 1) {
                if (lpfl->rgcsh[0] >= 7 && Random(5) == 0) {
                    idPlanDst = IdRandomPlanetNearby(lpfl->pt, 105, 1);
                    if (idPlanDst != -1 && idPlanDst != lpfl->idPlanet) {
                        ClearAiCurrentTask(lpfl, 1);
                        ord.id = idPlanDst;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[idPlanDst];
                        ord.grTask = grTaskNone;
                        ord.fValidTask = 1;
                        ord.iWarp = 4;
                        FMoveAiFleet(lpfl, &ord, 0);
                        goto L_15d1;
                    }
                }
                if (lpfl->lpplord->rgord[0].grTask == grTaskNone) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                    sel.fl.lpplord->rgord[0].tlm.cTime = 5;
                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 5;
                    FLookupFleet(-1, &sel.fl);
                    continue;
                }
            } else if (FIsAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
                if (lpfl->lpplord->rgord[0].grTask == grTaskLayMines) {
                    ClearAiCurrentTask(lpfl, 1);
                }
                for (j = 2; j <= 7 && lpfl->rgcsh[j] <= 0; j++) {
                }
                if (j <= 7 && ((lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || lpfl->idPlanet != -1)) {
                    if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                        id = lpfl->lpplord->rgord[1].id;
                    } else {
                        id = lpfl->idPlanet;
                    }
                    lpb = vlpbAiPlanet + (10 + 16 * id);
                    if (*lpb != 0) {
                        *lpb |= 0x80;
                    }
                }
            } else if (FIsAiTransport(lpfl) != 0) {
                idPlanDst = -1;
                if (lpfl->cord <= 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                if (idPlanDst != -1) {
                    lppl = LpplFromId(idPlanDst);
                    if ((lppl == 0 || lppl->iPlayer != idPlayer) && lpfl->rgwtMin[3] == 0) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.cord = 1;
                        sel.fl.lpplord->iordMac = 1;
                        FLookupFleet(-1, &sel.fl);
                        ClearAiCurrentTask(lpfl, 0);
                    }
                }
            }
        L_15d1:
            if (game.turn <= 20 && lpfl->rgcsh[0] > 0) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                FLookupFleet(-1, &sel.fl);
            } else if (lpfl->cord <= 1 && lpfl->rgcsh[1] != 0 && (game.turn >= 5 || game.mdStartDist == startDistClose)) {
                if (iAiLvl > 1 && lpfl->idPlanet != -1) {
                    lpplDrop = LpplFromId(lpfl->idPlanet);
                    if (lpplDrop != 0 && lpplDrop->iPlayer != -1 && lpplDrop->iPlayer != idPlayer && lpfl->rgwtMin[3] > 0 &&
                        GetRaceStat(&rgplr[lpplDrop->iPlayer], rsMajorAdv) != raMacintosh) {
                        memset(&ord, 0, sizeof(ORDER));
                        ord.pt = rgptPlan[lpplDrop->id];
                        ord.id = lpplDrop->id;
                        ord.grobj = grobjPlanet;
                        ord.fValidTask = 1;
                        ord.grTask = grTaskXfer;
                        ord.txp.rgia[3].iAction = iActionUnloadAll;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0] = ord;
                        FLookupFleet(-1, &sel.fl);
                        lpplDrop = LpplFindClosestEnum(lpplDrop, FEnumOurStarbase);
                        if (lpplDrop == 0)
                            continue;
                        memset(&ord, 0, sizeof(ORDER));
                        ord.id = lpplDrop->id;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[lpplDrop->id];
                        ord.grTask = grTaskNone;
                        ord.fValidTask = 1;
                        ord.iWarp = 4;
                        FMoveAiFleet(lpfl, &ord, 0);
                        continue;
                    }
                }
                idPlanDst = IdNearestColonizablePlanet(lpfl, &lpthWorm);
                if (idPlanDst == -1 && lpthWorm == 0) {
                    if (lpfl->idPlanet != -1) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                    }
                } else {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (lpfl->idPlanet != -1) {
                        XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, 10);
                        FLookupFleet(lpfl->id, &sel.fl);
                    }
                    if (idPlanDst != -1) {
                        FColonizeAiFleet(lpfl, idPlanDst);
                    } else {
                        FGotoWormholeAiFleet(lpfl, lpthWorm);
                    }
                }
            }
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0 || lppl->fStarbase != 0)
            break;
    }
    lpplHome = ipl == vclpplAi ? NULL : lppl;
    if (lpplHome != 0) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0)
                break;
            if (lpfl->iPlayer == idPlayer && lpfl->cord <= 1 && FIsAiTransport(lpfl) != 0) {
                if (lpfl->iplan != 4) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.iplan = 4;
                    FLookupFleet(-1, &sel.fl);
                }
                lppl = NULL;
                for (i = 0; i < (int16_t)RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                    for (j = 0; j < (int16_t)RawLoad16(vlpbAiData + (i * 20 + 6)) && RawLoad16(vlpbAiData + (i * 20 + j * 2 + 8)) != lpfl->id; j++) {
                    }
                    if (j < (int16_t)RawLoad16(vlpbAiData + (i * 20 + 6)))
                        break;
                }
                if (i < (int16_t)RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                    lppl = LpplFromId(RawLoad16(vlpbAiData + (i * 20 + 4)));
                }
                IdTargetFreighter(lpfl, lppl == 0 ? lpplHome : lppl);
            }
        }
        UpdateProgressGauge(progressStep4);
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            for (i = 0; i < 16 && (lpfl->rgcsh[i] <= 0 || rgRecycleShdef[i] != 0); i++) {
            }
            if (i == 16) {
                if (lpfl->idPlanet != -1) {
                    lppl = LpplFromId(lpfl->idPlanet);
                    if (lppl != 0 && lppl->iPlayer == idPlayer && (lppl->fStarbase != 0 || Random(5) == 0)) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                        continue;
                    }
                }
                if ((lpfl->cord > 1 && lpfl->idPlanet == -1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || FMoveToNearestStarbase(lpfl, 0) != 0)
                    continue;
            }
            for (i = 2; i <= 10; i++) {
                if (lpfl->rgcsh[i] > 0) {
                    IdTargetArmada(lpfl);
                    break;
                }
            }
            if (i > 10 && FIsAiAttack(lpfl) != 0 && (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjFleet) &&
                ((cFlDestroyers <= (game.turn <= 120 ? 70 : 50) && (cFlDestroyers <= (game.turn <= 120 ? 60 : 40) || Random(3) != 0)) ||
                 (((iLatestDestroyer != -1 ? lpfl->rgcsh[iLatestDestroyer] : lpfl->pt.y) >= 20 && Random(20) != 0) ||
                  FFindBuddyAndJoinUp(lpfl, 14, 15, 36, 72) == 0))) {
                IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);
            }
        }
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureRobotoidShdefs() {
    int16_t ish;
    int16_t i;
    int16_t shBase;
    SHDEF   shdef;

    for (ish = 11; ish <= 13; ish++) {
        if (rgshdef[ish].fFree != 0 && rgplr[idPlayer].rgTech[2] >= 2 && rgplr[idPlayer].rgTech[3] >= (ish - 11) * 3 + 4 &&
            (ish == 11 || (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 14)) {
            if (rgplr[idPlayer].rgTech[3] < 10) {
                FCreateAiShdef(ish, ihuldefPrivateer, &vrgRobAip[vrgRobIshAip[ish == 11 ? 14 : 15]]);
            } else {
                for (i = 0; i < 5 && FCreateAiShdef(ish, ihuldefMetaMorph, &vrgRobAip[vrgRobIshAip[Random(6) + 8]]) == 0; i++) {
                }
            }
        }
    }
    if (rgshdef[14].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 6 && rgplr[idPlayer].rgTech[3] >= 6 &&
        rgplr[idPlayer].rgTech[2] >= 6 && rgplr[idPlayer].rgTech[0] >= 2) {
        for (i = 0; i < 5 && (FCreateAiShdef(14, ihuldefNubian, &vrgRobAip[vrgRobIshAip[37]]) != 0 ||
                              FCreateAiShdef(14, ihuldefDestroyer, &vrgRobAip[vrgRobIshAip[Random(4) + 16]]) == 0);
             i++) {
        }
    }
    if (rgshdef[15].fFree != 0 && rgplr[idPlayer].rgTech[4] >= 10 && rgplr[idPlayer].rgTech[3] >= 8 && rgplr[idPlayer].rgTech[2] >= 9 &&
        rgplr[idPlayer].rgTech[1] >= 14 && FCreateAiShdef(15, ihuldefNubian, &vrgRobAip[vrgRobIshAip[37]]) == 0) {
        for (i = 0; i < 5 && FCreateAiShdef(15, ihuldefDestroyer, &vrgRobAip[vrgRobIshAip[Random(4) + 20]]) == 0; i++) {
        }
    }
    for (ish = 2; ish <= 5; ish++) {
        if (rgshdef[ish].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 10 && rgplr[idPlayer].rgTech[3] >= 10 && rgplr[idPlayer].rgTech[2] >= 9 &&
            rgplr[idPlayer].rgTech[0] >= 6 && (ish == 2 || (rgshdef[ish - 1].fFree == 0 && (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 12))) {
            shBase = (ish - 2 & 1) == 0 ? 0 : 4;
            for (i = 0; i < 5 && FCreateAiShdef(ish, ihuldefMetaMorph, &vrgRobAip[vrgRobIshAip[Random(4) + shBase]]) == 0; i++) {
            }
        }
    }
    for (ish = 6; ish <= 7; ish++) {
        if (rgshdef[ish].fFree != 0 && rgplr[idPlayer].rgTech[5] >= 4 && rgplr[idPlayer].rgTech[4] >= 10 && rgplr[idPlayer].rgTech[3] >= 12 &&
            rgplr[idPlayer].rgTech[2] >= 12 && rgplr[idPlayer].rgTech[0] >= 6 && rgplr[idPlayer].rgTech[1] >= 15 &&
            (ish == 6 || (rgshdef[ish - 1].fFree == 0 && (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 20))) {
            shBase = ish == 6 ? 27 : 31;
            for (i = 0; i < 5 && FCreateAiShdef(ish, ihuldefBattleship, &vrgRobAip[vrgRobIshAip[Random(4) + shBase]]) == 0; i++) {
            }
        }
    }
    for (ish = 9; ish <= 10; ish++) {
        if (rgshdef[ish].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 14 &&
            ((ish == 9 || (rgshdef[ish - 1].fFree == 0 && (uint16_t)(game.turn - rgshdef[ish - 1].turn) > 15)) &&
             FCreateAiShdef(ish, ihuldefBattleship, &vrgRobAip[vrgRobIshAip[36]]) == 0)) {
            FCreateAiShdef(ish, ihuldefB52Bomber, &vrgRobAip[vrgRobIshAip[ish == 9 ? 24 : 25]]);
        }
    }
    if (rgshdef[0].hul.ihuldef != ihuldefFrigate && rgplr[idPlayer].lvlAi > lvlAiStandard && rgshdef[0].cExist == 0 && rgplr[idPlayer].rgTech[5] >= 4 &&
        rgplr[idPlayer].rgTech[4] >= 5 && rgplr[idPlayer].rgTech[3] >= 6 && rgplr[idPlayer].rgTech[2] >= 6 && rgplr[idPlayer].rgTech[0] >= 6) {
        shdef = rgshdef[0];
        shdef.fFree = 1;
        FChangeAiShdef(&shdef, 0);
        FCreateAiShdef(0, ihuldefFrigate, &vrgRobAip[vrgRobIshAip[26]]);
    }
    return;
}

int16_t IdTargetArmada(FLEET *lpfl) {
    int16_t cshWar;
    FLEET  *lpflTarget;
    PLANET *lpplTarget;
    ORDER   ord;
    int16_t ish;
    PLANET *lppl;
    int32_t cCol;
    int16_t cshBomb;
    int32_t pctDef;
    int32_t lPopUs;
    int32_t lPopEnemy;
    int32_t cXfer;

    if (lpfl->cord > 1) {
        ord = lpfl->lpplord->rgord[1];
        if (LDistance2(lpfl->pt, ord.pt) <= 62500 || ord.grobj != grobjFleet) {
            if (ord.grobj == grobjFleet) {
                return 0;
            }
            if (ord.grobj == grobjPlanet) {
                lppl = LpplFromId(ord.id);
                if (lppl == 0 || ((lppl->iPlayer != -1 && (lppl->iPlayer != idPlayer || lppl->fStarbase != 0)) || lppl->turn != game.turn)) {
                    return 0;
                }
            }
        }
    }
    cshWar = 0;
    for (ish = 2; ish <= 5; ish++) {
        cshWar += lpfl->rgcsh[ish];
    }
    for (ish = 6; ish <= 7; ish++) {
        cshWar += lpfl->rgcsh[ish] * 2;
    }
    cshBomb = lpfl->rgcsh[9] + lpfl->rgcsh[10];
    lpfl->fMark = 1;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (lpfl->idPlanet == -1) {
        MoveToNearestPlanetOrEnemy(lpfl, 150);
        return 0;
    }
    lppl = LpplFromId(lpfl->idPlanet);
    if (lppl->iPlayer == idPlayer && lppl->fStarbase != 0) {
        if (cshWar >= vrgAiArmadaPotency[0] && cshBomb >= vrgAiArmadaPotency[2]) {
            if (sel.pl.rgwtMin[3] > 3000) {
                cCol = (int32_t)(sel.pl.rgwtMin[3] / 10);
            } else if (sel.pl.rgwtMin[3] > 2000) {
                cCol = (int32_t)(sel.pl.rgwtMin[3] / 15);
            } else if (sel.pl.rgwtMin[3] > 1000) {
                cCol = (int32_t)(sel.pl.rgwtMin[3] / 20);
            } else {
                cCol = 0;
            }
            if (cCol > 0) {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, LOWORD(cCol));
                FLookupFleet(lpfl->id, &sel.fl);
            }
        } else if (rgplr[idPlayer].lvlAi <= lvlAiStandard || (cshWar <= vrgAiArmadaPotency[0] * 2 && cshWar < 60) ||
                   (Random(10) >= 5 && (cshWar <= vrgAiArmadaPotency[0] * 3 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
            return 0;
        }
    } else if (cshWar < vrgAiArmadaPotency[1] || cshBomb < vrgAiArmadaPotency[3]) {
        ClearAiCurrentTask(lpfl, 0);
        if (rgplr[idPlayer].lvlAi <= lvlAiStandard || ((cshWar <= vrgAiArmadaPotency[0] * 2 || Random(10) >= 5) &&
                                                       (cshWar <= vrgAiArmadaPotency[0] * 4 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
            lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
            goto TargetEveryArmada;
        }
    } else if (lppl->iPlayer != -1) {
        if (lppl->iPlayer == idPlayer) {
            if (lppl->rgwtMin[3] > 1000) {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, (int16_t)LOWORD(lppl->rgwtMin[3]) / 5);
                FLookupFleet(lpfl->id, &sel.fl);
            }
        } else {
            if (lppl->iPlayer == idPlayer) {
                return 0;
            }
            lPopUs = lpfl->rgwtMin[3];
            lPopEnemy = (int32_t)(lppl->uPopGuess * 4);
            pctDef = (uint32_t)(lppl->uDefGuess * 6) + 6;
            pctDef = (int32_t)(pctDef * 3) / 4;
            lPopEnemy = (int32_t)((int32_t)(lPopEnemy * 100) / (100 - pctDef));
            if (lPopEnemy >= (int32_t)(lPopUs / 5) && (lPopEnemy >= 200 || lPopUs <= 350) && (lPopEnemy >= 10 || lPopUs <= 150)) {
                return 0;
            }
            cXfer = (int32_t)(lPopEnemy * 5) / 4;
            if (lpfl->rgwtMin[3] < (cXfer <= (int32_t)(lpfl->rgwtMin[3] / 2) ? (int32_t)(lpfl->rgwtMin[3] / 2) : cXfer)) {
                cXfer = lpfl->rgwtMin[3];
            } else if (cXfer <= (int32_t)(lpfl->rgwtMin[3] / 2)) {
                cXfer = (int32_t)(lpfl->rgwtMin[3] / 2);
            }
            if (cXfer > 30000) {
                cXfer = 30000;
            }
            XferAiTroopers(lpfl->id, lppl->id, LOWORD(cXfer));
            FLookupFleet(lpfl->id, &sel.fl);
            return 0;
        }
    }
    if (game.fAisBand != 0) {
        lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    } else {
        lpplTarget = NULL;
    }
    if (lpplTarget == 0) {
        lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
    }
TargetEveryArmada:
    if (lpplTarget != 0) {
        vlpbAiPlanet[lpplTarget->id * 16 + 10] = vlpbAiPlanet[lpplTarget->id * 16 + 0xa] | 0x80;
        ord.id = lpplTarget->id;
        ord.grobj = grobjPlanet;
        ord.pt = rgptPlan[lpplTarget->id];
    } else {
        lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
        if (lpflTarget == 0) {
            return 0;
        }
        ord.id = lpflTarget->id;
        ord.grobj = grobjFleet;
        ord.pt = lpflTarget->pt;
    }
    ord.grTask = grTaskNone;
    ord.fValidTask = 1;
    ord.iWarp = 4;
    if (FMoveAiFleet(lpfl, &ord, 0) == 0) {
        return -1;
    }
    return 0;
}

int16_t FPotentRobWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

    cEquiv = 0;
    for (ish = 2; ish <= 5; ish++) {
        cEquiv += lpfl->rgcsh[ish];
    }
    for (ish = 6; ish <= 7; ish++) {
        cEquiv += lpfl->rgcsh[ish] * 2;
    }
    if (iPotency < 2) {
        return 1;
    }
    if (cEquiv >= vrgAiArmadaPotency[0]) {
        return 1;
    }
    return 0;
}

int16_t FEnumCalcEnemyFleets(FLEET *lpflSrc, FLEET *lpflTest) {
    if (lpflTest->iPlayer != idPlayer) {
        return 1;
    }
    return 0;
}

int16_t FEnumCalcArmadaDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

    if (lpplSrc == lpplTest) {
        return 0;
    }
    id = lpplTest->id;
    b = vlpbAiPlanet[id * 16 + 10];
    if (b != 0) {
        l2 = LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]);
        if (l2 < 2500) {
            b += 7;
        } else if (l2 < 10000) {
            b += 5;
        } else if (l2 < 22500) {
            b += 4;
        } else if (l2 < 40000) {
            b += 3;
        } else if (l2 < 90000) {
            b += 2;
        } else if (l2 < 250000) {
            b++;
        }
        if ((b & 0x80) == 0 || Random(4) == 0) {
            return b;
        }
    }
    return 0;
}

int16_t FEnumCalcArmadaHumanDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

    if (lpplSrc == lpplTest) {
        return 0;
    }
    id = lpplTest->id;
    if (rgplr[lpplTest->iPlayer].fAi != 0) {
        return 0;
    }
    b = vlpbAiPlanet[id * 16 + 10];
    if (b != 0) {
        l2 = LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]);
        if (l2 < 2500) {
            b += 7;
        } else if (l2 < 10000) {
            b += 5;
        } else if (l2 < 22500) {
            b += 4;
        } else if (l2 < 40000) {
            b += 3;
        } else if (l2 < 90000) {
            b += 2;
        } else if (l2 < 250000) {
            b++;
        }
        if ((b & 0x80) == 0 || Random(4) == 0) {
            return b;
        }
    }
    return 0;
}

void DoTurinDroneAiTurn(PROD *rgprod) {
    int32_t  rgResCost[4];
    int16_t  iLatestCruiser;
    int32_t  rgResAvail[4];
    FLEET   *lpflEnemy;
    int16_t  cExistCargo;
    int16_t  iLatestDestroyer;
    THING   *lpthWorm;
    PLANET  *lpplDest;
    uint8_t  rgRecycleShdef[16];
    int16_t  idPlanDst;
    int16_t  j;
    int16_t  iLatestLayer;
    ORDER    ord;
    PLANET  *lppl;
    int16_t  iLatestTroop;
    uint16_t rgCosts[4];
    PLANET  *lpplHome;
    FLEET   *lpflAttack;
    FLEET   *lpfl;
    int16_t  ifl;
    int16_t  i;
    FLEET   *lpflT;
    uint8_t  b;
    int16_t  cRes;
    int16_t  iroCur;
    int16_t  iLatestMiner;
    int16_t  iLatestCargo;
    int16_t  cplMiners;
    PLANET  *lpplMac;
    int16_t  ishdefSBLatest;
    int16_t  cplNegative;
    int16_t  ipl;
    uint16_t cRecyclePeriod;
    uint16_t cplanCol;
    int16_t  cFr;
    int16_t  cplBadGuy;
    int16_t  iLatestBattle;
    int16_t  iLatestBomber;
    int32_t  l;
    PROD    *lpprod;
    int16_t  fWrite;
    int16_t  iPlanet;
    uint8_t  bT;
    int16_t  pct;
    int16_t  id;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0;
    cplMiners = 0;
    iroCur = IroEnsureAi(vrgAiTurinDroneResOrder, 31, &ishdefSBLatest, 15);
    if (rgshdef[13].fFree == 0) {
        MergeAllShdefs(-7952);
    }
    if (rgshdef[12].fFree == 0) {
        MergeAllShdefs(4096);
    }
    if (rgshdef[10].fFree == 0) {
        MergeAllShdefs(3072);
    }
    if (rgshdef[2].fFree == 0) {
        MergeAllShdefs(12);
    }
    j = 3;
    if (game.turn > 130) {
        j += (uint32_t)(game.turn - 120) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiArmadaPotency[0] = LOBYTE(j);
    vrgAiArmadaPotency[1] = LOBYTE((int16_t)(j & 0xff) / 2);
    j = 6;
    if (game.turn > 115) {
        j += (uint32_t)(game.turn - 100) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiArmadaPotency[2] = LOBYTE(j);
    vrgAiArmadaPotency[3] = LOBYTE(3 >= j / 2 - 1 ? j / 2 - 1 : 3);
    memset(rgRecycleShdef, 0, 16);
    if (game.turn < 120) {
        cRecyclePeriod = 50;
    } else {
        cRecyclePeriod = game.turn >= 200 ? 100 : 70;
    }
    CheckAiShdefStatus(6, 7, cRecyclePeriod, &iLatestCruiser, rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(8, 9, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(13, 14, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(4, 5, cRecyclePeriod, &iLatestBattle, rgRecycleShdef);
    CheckAiShdefStatus(12, 12, cRecyclePeriod, &iLatestLayer, rgRecycleShdef);
    CheckAiShdefStatus(15, 15, cRecyclePeriod, &iLatestTroop, rgRecycleShdef);
    if (rgplr[idPlayer].rgTech[3] >= 7) {
        CheckAiShdefStatus(2, 3, cRecyclePeriod, &iLatestMiner, rgRecycleShdef);
    } else {
        iLatestMiner = -1;
    }
    CheckAiShdefStatus(10, 11, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    if (game.turn > 60) {
        SplitOutShdefs(rgRecycleShdef);
    }
    EnsureTurinDroneShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer == -1 && lppl->det >= detSome && PctPlanetOptValue(lppl, idPlayer) > 0) {
            cplanCol++;
        }
    }
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        vlpbAiPlanet[lppl->id * 16 + 9] = 1;
        if (lppl->iPlayer == -1 && lppl->det >= detSome) {
            b = 0;
            for (i = 0; i < 3; i++) {
                if (lppl->rgMinConc[i] > 66) {
                    bT = 75;
                } else {
                    bT = LOBYTE((int16_t)lppl->rgMinConc[i] / 2);
                }
                b += LOBYTE(bT);
            }
            if ((b & 0x80) != 0) {
                b = 127;
            }
            vlpbAiPlanet[lppl->id * 16 + 1] = b;
            cplMiners++;
        }
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 1);
            pct = PctPlanetOptValue(lppl, idPlayer);
            if (pct > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = LOBYTE(pct);
                cplBadGuy++;
            }
        } else if (lppl->iPlayer == idPlayer) {
            if (PctPlanetDesirability(lppl, idPlayer) < 0) {
                cplNegative++;
                vlpbAiPlanet[lppl->id * 16 + 2] = 1;
            } else if (lppl->fStarbase == 0 || lppl->rgwtMin[3] < 200) {
                ChangeMainObjSel(grobjPlanet, lppl->id);
                sel.pl.fNoResearch = LOWORD((uint32_t)(lppl->rgwtMin[3] < 200 ? 1 : 0));
                if ((uint32_t)sel.pl.fNoResearch != lppl->fNoResearch) {
                    FLookupPlanet(-1, &sel.pl);
                }
            } else {
                ChangeMainObjSel(grobjPlanet, lppl->id);
                InitProduction(rgprod);
                fWrite = 0;
                b = 0;
                i = 0;
                for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm); lpprod++) {
                    i++;
                }
                if (i < lpplProdGlob->iprodMac) {
                    FinishProduction(0);
                } else {
                    if (game.turn == 0) {
                        i = game.cPlanMax;
                        while (i > 0) {
                            AddItemToQueue(0, 1, grobjFleet, addItemEnd);
                            if (i > 190) {
                                i -= 100;
                            } else {
                                i -= 30;
                            }
                            fWrite = 1;
                        }
                    } else if (rgshdef[0].fFree == 0 && rgshdef[0].hul.ihuldef == ihuldefFrigate &&
                               rgshdef[0].cExist < (uint32_t)(game.cPlanMax / 4 >= 32 ? 32 : game.cPlanMax / 4) &&
                               rgshdef[0].cExist > (uint32_t)(rgshdef[0].cBuilt / 10)) {
                        AddItemToQueue(0, 1, grobjFleet, addItemEnd);
                        fWrite = 1;
                    }
                    cFr = rgplr[idPlayer].cPlanet / 10 <= RawLoad16((uint8_t *)vlpbAiData + 0x2) * 2 ? RawLoad16((uint8_t *)vlpbAiData + 0x2) * 2
                                                                                                     : rgplr[idPlayer].cPlanet / 10;
                    if (rgplr[idPlayer].rgTech[2] >= 5 && iLatestCargo != -1 &&
                        (cExistCargo < cFr || (cExistCargo < (int16_t)(10 * cFr) / 7 && Random(4) == 0))) {
                        AddItemToQueue(iLatestCargo, 1, grobjFleet, addItemEnd);
                        fWrite = 1;
                    }
                    if ((cplanCol != 0 || cplBadGuy != 0) && rgshdef[1].cExist < 2) {
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        AddItemToQueue(1, 1, grobjFleet, addItemEnd);
                        fWrite = 1;
                    }
                    l = (uint32_t)(lppl->rgwtMin[3] * PctTrueMaxGrowth(idPlayer));
                    cRes = CResourcesAtPlanet(lppl, idPlayer);
                    if (rgshdef[12].fFree == 0 && Random(3) == 0) {
                        id = lppl->id;
                        cFr = 0;
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == id && lpfl->rgcsh[12] > 0 && lpfl->iPlayer == idPlayer) {
                                cFr = lpfl->rgcsh[12];
                                break;
                            }
                        }
                        if ((cFr < 10 || (cFr < 17 && Random(8) == 0)) && Random(cFr * 2 + 1) == 0) {
                            AddItemToQueue(12, 3, grobjFleet, addItemEnd);
                            fWrite = 1;
                        }
                    }
                    if (iLatestBomber != -1) {
                        id = lppl->id;
                        for (ifl = 0; ifl < cFleet; ifl++) {
                            lpfl = rglpfl[ifl];
                            if (rglpfl[ifl] == 0)
                                break;
                            if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentRobWarFleet(lpfl, 2) != 0) {
                                if (iLatestBomber == -1 || lpfl->rgcsh[13] + lpfl->rgcsh[14] < vrgAiArmadaPotency[2])
                                    break;
                                AddItemToQueue(iLatestBomber, 4, grobjFleet, addItemEnd);
                                fWrite = 1;
                                goto FinishProd;
                            }
                        }
                    }
                    if (iLatestBattle != -1 && rgshdef[iLatestBattle].cExist < (uint32_t)(game.cPlanMax / 24 + 4)) {
                        GetResourcesAvailable(lppl, rgResAvail);
                        GetProdQCost(lppl, rgResCost);
                        for (i = 0; i < 4; i++) {
                            rgResAvail[i] -= rgResCost[i];
                            if (rgResAvail[i] < 0)
                                goto FinishProd;
                        }
                        for (i = 0; i < 5; i++) {
                            GetTrueHullCost(idPlayer, &rgshdef[iLatestBattle].hul, rgCosts);
                            for (j = 0; j < 4; j++) {
                                rgResAvail[j] -= (uint32_t)rgCosts[j];
                                if (rgResAvail[j] < 0)
                                    goto FinishProd;
                            }
                            fWrite = 1;
                            AddItemToQueue(iLatestBattle, 1, grobjFleet, addItemEnd);
                        }
                    }
                    if (iLatestCruiser != -1 && rgshdef[iLatestCruiser].cExist < (uint32_t)(game.cPlanMax / 12 + 8)) {
                        GetResourcesAvailable(lppl, rgResAvail);
                        GetProdQCost(lppl, rgResCost);
                        for (i = 0; i < 4; i++) {
                            rgResAvail[i] -= rgResCost[i];
                            if (rgResAvail[i] < 0)
                                goto FinishProd;
                        }
                        for (i = 0; i < 5; i++) {
                            GetTrueHullCost(idPlayer, &rgshdef[iLatestCruiser].hul, rgCosts);
                            for (j = 0; j < 4; j++) {
                                rgResAvail[j] -= (uint32_t)rgCosts[j];
                                if (rgResAvail[j] < 0)
                                    goto FinishProd;
                            }
                            fWrite = 1;
                            AddItemToQueue(iLatestCruiser, 1, grobjFleet, addItemEnd);
                        }
                    }
                    if (iLatestDestroyer != -1 && rgshdef[iLatestDestroyer].cExist < (uint32_t)(game.cPlanMax / 4 + 12)) {
                        GetResourcesAvailable(lppl, rgResAvail);
                        GetProdQCost(lppl, rgResCost);
                        for (i = 0; i < 4; i++) {
                            rgResAvail[i] -= rgResCost[i];
                            if (rgResAvail[i] < 0)
                                goto FinishProd;
                        }
                        for (i = 0; i < 5; i++) {
                            GetTrueHullCost(idPlayer, &rgshdef[iLatestDestroyer].hul, rgCosts);
                            for (j = 0; j < 4; j++) {
                                rgResAvail[j] -= (uint32_t)rgCosts[j];
                                if (rgResAvail[j] < 0)
                                    goto FinishProd;
                            }
                            fWrite = 1;
                            AddItemToQueue(iLatestDestroyer, 1, grobjFleet, addItemEnd);
                        }
                    }
                    if (iLatestTroop != -1 && rgshdef[15].fFree == 0 && rgshdef[iLatestTroop].cExist < (uint32_t)(game.cPlanMax / 12 + 8)) {
                        GetResourcesAvailable(lppl, rgResAvail);
                        GetProdQCost(lppl, rgResCost);
                        for (i = 0; i < 4; i++) {
                            rgResAvail[i] -= rgResCost[i];
                            if (rgResAvail[i] < 0)
                                goto FinishProd;
                        }
                        for (i = 0; i < 5; i++) {
                            GetTrueHullCost(idPlayer, &rgshdef[iLatestTroop].hul, rgCosts);
                            for (j = 0; j < 4; j++) {
                                rgResAvail[j] -= (uint32_t)rgCosts[j];
                                if (rgResAvail[j] < 0)
                                    goto FinishProd;
                            }
                            fWrite = 1;
                            AddItemToQueue(iLatestTroop, 1, grobjFleet, addItemEnd);
                        }
                    }
                FinishProd:
                    FinishProduction(fWrite);
                }
            }
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0 || lppl->fStarbase != 0)
            break;
    }
    lpplHome = lppl == lpplMac ? NULL : lppl;
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else {
            if (FIsTurinDroneAiAttack(lpfl) != 0) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
            }
            lpfl->fMark = 0;
            if ((lpfl->rgcsh[2] != 0 || lpfl->rgcsh[3] != 0) && lpfl->lpplord->rgord[lpfl->cord - 1].grTask != grTaskNone) {
                if (lpfl->idPlanet != -1) {
                    if (LpplFromId(lpfl->idPlanet)->iPlayer != -1)
                        goto LBlowAwayOrders;
                    idPlanDst = lpfl->idPlanet;
                } else {
                    if (lpfl->cord <= 1)
                        continue;
                    lpfl->lpplord->rgord[1].grTask = grTaskMine;
                    if (lpfl->lpplord->rgord[1].grobj != grobjPlanet) {
                    }
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
                vlpbAiPlanet[idPlanDst * 16 + 1] = vlpbAiPlanet[idPlanDst * 16 + 1] | 0x80;
                continue;
            }
            if (lpfl->rgcsh[8] != 0 || lpfl->rgcsh[9] != 0) {
                idPlanDst = -1;
                if (lpfl->cord <= 1) {
                    idPlanDst = lpfl->idPlanet;
                } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                }
            } else {
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
                if (vlpbAiPlanet[idPlanDst * 16 + 3] == 0) {
                    lppl = LpplFromId(idPlanDst);
                    if (lppl != 0 && lppl->iPlayer != -1 && lppl->iPlayer != idPlayer)
                        goto LBlowAwayOrders;
                    continue;
                }
            }
            if (idPlanDst != -1) {
                lppl = LpplFromId(idPlanDst);
                if (lppl != 0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh &&
                    lppl->fStarbase == 0) {
                    memset(&ord, 0, sizeof(ORDER));
                    ord.pt = rgptPlan[idPlanDst];
                    ord.grobj = grobjPlanet;
                    ord.id = idPlanDst;
                    ord.grTask = grTaskXfer;
                    ord.fValidTask = 1;
                    ord.txp.rgia[3].iAction = iActionUnloadAll;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (sel.fl.lpplord->rgord[0].id == idPlanDst && sel.fl.lpplord->rgord[0].grobj == grobjPlanet) {
                        sel.fl.lpplord->rgord[0] = ord;
                    } else {
                        sel.fl.lpplord->rgord[1] = ord;
                    }
                    FLookupFleet(-1, &sel.fl);
                    vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 3] | 0x80;
                    FMoveToNearestStarbase(lpfl, 0);
                    continue;
                }
            } else if (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjOther) {
                continue;
            }
        LBlowAwayOrders:
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.cord = 1;
            sel.fl.lpplord->iordMac = 1;
            FLookupFleet(-1, &sel.fl);
            ClearAiCurrentTask(lpfl, 0);
        }
    }
    fMarkedPlanets = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->rgcsh[2] != 0 || lpfl->rgcsh[3] != 0) {
                if (game.turn != 0) {
                    if (lpfl->idPlanet == -1)
                        continue;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    b = vlpbAiPlanet[lpfl->idPlanet * 16 + 1];
                    if (b >= 4)
                        continue;
                    lppl = LpplFindBestEnum(&sel.pl, FEnumCalcMinerDest);
                    if (lppl == 0)
                        continue;
                    memset(&ord, 0, sizeof(ORDER));
                    ord.id = lppl->id;
                    ord.grobj = grobjPlanet;
                    ord.pt = rgptPlan[lppl->id];
                    ord.grTask = grTaskMine;
                    ord.fValidTask = 1;
                    ord.iWarp = 6;
                    FMoveAiFleet(lpfl, &ord, 1);
                    vlpbAiPlanet[lppl->id * 16 + 1] = vlpbAiPlanet[lppl->id * 16 + 1] | 0x80;
                    vlpbAiPlanet[lpfl->idPlanet * 16 + 1] = vlpbAiPlanet[lpfl->idPlanet * 16 + 1] & 0x80;
                    continue;
                }
            } else {
                if (lpfl->cord > 1)
                    continue;
                if (lpfl->rgcsh[1] != 0) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if ((lpfl->idPlanet == -1 || sel.pl.iPlayer != idPlayer || sel.pl.rgwtMin[3] < 50) && lpfl->rgwtMin[3] == 0) {
                        if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0) ||
                            (rgshdef[1].hul.rghs[0].iItem > 2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                            continue;
                    } else {
                        lpthWorm = NULL;
                        idPlanDst = IdNearestColonizablePlanet(lpfl, NULL);
                        if (lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, Colonists, 25);
                            FLookupFleet(lpfl->id, &sel.fl);
                            lppl = LpplFromId(lpfl->idPlanet);
                        } else {
                            lppl = lpplHome;
                        }
                        if (idPlanDst != -1) {
                            FColonizeAiFleet(lpfl, idPlanDst);
                            vlpbAiPlanet[idPlanDst * 16 + 15] = 4;
                            continue;
                        }
                        if (lppl != 0) {
                            lpplDest = LpplFindClosestEnum(lppl, FEnumCalcColonistDrop);
                            if (lpplDest != 0) {
                                vlpbAiPlanet[lpplDest->id * 16 + 10] = vlpbAiPlanet[lpplDest->id * 16 + 0xa] | 0x80;
                                memset(&ord, 0, sizeof(ORDER));
                                ord.id = lpplDest->id;
                                ord.grobj = grobjPlanet;
                                ord.pt = rgptPlan[lpplDest->id];
                                ord.grTask = grTaskXfer;
                                ord.fValidTask = 1;
                                ord.iWarp = 6;
                                ord.txp.rgia[3].iAction = iActionUnloadAll;
                                FMoveAiFleet(lpfl, &ord, 0);
                                continue;
                            }
                        }
                        if (lpthWorm == 0 || Random(100) >= 10)
                            continue;
                        FGotoWormholeAiFleet(lpfl, lpthWorm);
                        continue;
                    }
                } else if (lpfl->rgcsh[8] != 0 || lpfl->rgcsh[9] != 0) {
                    if (game.turn == 0) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                    } else {
                        if (lpplHome == 0)
                            break;
                        lppl = NULL;
                        for (i = 0; i < (int16_t)RawLoad16((uint8_t *)vlpbAiData + 0x2); i++) {
                            for (j = 0; j < (int16_t)RawLoad16(vlpbAiData + (i * 20 + 6)) && RawLoad16(vlpbAiData + (i * 20 + j * 2 + 8)) != lpfl->id; j++) {
                            }
                            if (j < (int16_t)RawLoad16(vlpbAiData + (i * 20 + 6)))
                                break;
                        }
                        if (i < (int16_t)RawLoad16((uint8_t *)vlpbAiData + 0x2)) {
                            lppl = LpplFromId(RawLoad16(vlpbAiData + (i * 20 + 4)));
                        }
                        IdTargetFreighter(lpfl, lppl == 0 ? lpplHome : lppl);
                        continue;
                    }
                } else if (lpfl->rgcsh[13] == 0 && lpfl->rgcsh[14] == 0) {
                    if (lpfl->rgcsh[0] != 0 || lpfl->rgcsh[10] != 0 || lpfl->rgcsh[11] != 0) {
                        if (lpfl->rgcsh[0] == 0 || rgplr[idPlayer].rgTech[3] < 6 || rgshdef[0].hul.ihuldef != ihuldefScout) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                            continue;
                        }
                    } else {
                        if (lpfl->rgcsh[12] == 0 || lpfl->cord != 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone)
                            continue;
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                        sel.fl.lpplord->rgord[0].tlm.cTime = 5;
                        sel.fl.lpplord->rgord[0].tlm.cTimeOld = 5;
                        FLookupFleet(-1, &sel.fl);
                        continue;
                    }
                } else {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (lpfl->idPlanet != -1) {
                        lppl = LpplFromId(lpfl->idPlanet);
                        if (lppl->iPlayer == idPlayer) {
                            if (lppl->fStarbase != 0 &&
                                (lpfl->rgcsh[13] + lpfl->rgcsh[14] < vrgAiArmadaPotency[2] || lpfl->rgcsh[4] + lpfl->rgcsh[5] < vrgAiArmadaPotency[1]))
                                continue;
                            FLookupFleet(lpfl->id, &sel.fl);
                        } else if (lppl->iPlayer != -1) {
                            lpflT = lpflEnemy;
                            while (1) {
                                if (lpflT == 0)
                                    goto L_4eb0;
                                if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                    break;
                                lpflT = lpflT->lpflNext;
                            }
                        }
                    } else {
                        lppl = lpplHome;
                    }
                    if (game.fAisBand != 0) {
                        lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
                    } else {
                        lpplDest = NULL;
                    }
                    if (lpplDest == 0) {
                        lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
                    }
                    lppl = lpplDest;
                    if (lppl == 0)
                        continue;
                    vlpbAiPlanet[lppl->id * 16 + 10] = vlpbAiPlanet[lppl->id * 16 + 0xa] | 0x80;
                    ord.id = lppl->id;
                    ord.grobj = grobjPlanet;
                    ord.pt = rgptPlan[lppl->id];
                    ord.grTask = grTaskNone;
                    ord.fValidTask = 1;
                    ord.iWarp = 4;
                    FMoveAiFleet(lpfl, &ord, 0);
                    continue;
                }
            }
            ChangeMainObjSel(grobjFleet, lpfl->id);
            sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
            FLookupFleet(-1, &sel.fl);
        }
    L_4eb0:;
    }
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureTurinDroneShdefs(int16_t iroCur) {
    SHDEF   shdef;
    int16_t i;

    if (rgshdef[8].fFree != 0 && rgplr[idPlayer].rgTech[2] >= 5 && rgplr[idPlayer].rgTech[3] >= 8) {
        FCreateAiShdef(8, ihuldefRogue, &vrgTDAip[vrgTDIshAip[12]]);
    }
    if (rgshdef[9].fFree != 0 && rgplr[idPlayer].rgTech[2] >= 7 && rgplr[idPlayer].rgTech[3] >= 11) {
        FCreateAiShdef(9, ihuldefGalleon, &vrgTDAip[vrgTDIshAip[15]]);
    }
    if (rgshdef[10].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 5 && rgplr[idPlayer].rgTech[3] >= 4 &&
        rgplr[idPlayer].rgTech[2] >= 5) {
        for (i = 0; i < 4 && FCreateAiShdef(10, ihuldefDestroyer, &vrgTDAip[vrgTDIshAip[Random(1) + 2]]) == 0; i++) {
        }
    }
    if (rgshdef[1].fFree != 0 || rgshdef[1].cExist == 0) {
        if (rgshdef[1].fFree == 0 && rgshdef[1].hul.ihuldef != ihuldefPrivateer) {
            shdef = rgshdef[1];
            shdef.fFree = 1;
            FChangeAiShdef(&shdef, 1);
        }
        FCreateAiShdef(1, ihuldefColonyShip, &vrgTDAip[vrgTDIshAip[0]]);
    }
    if (rgshdef[0].fFree != 0 || rgshdef[0].cExist == 0) {
        if (rgshdef[0].fFree == 0) {
            shdef = rgshdef[0];
            shdef.fFree = 1;
            FChangeAiShdef(&shdef, 0);
        }
        FCreateAiShdef(0, ihuldefFrigate, &vrgTDAip[vrgTDIshAip[1]]);
    }
    if ((rgshdef[2].fFree != 0 || rgshdef[2].cExist == 0) && rgplr[idPlayer].rgTech[3] >= 7 && rgplr[idPlayer].rgTech[4] >= 4) {
        if (rgshdef[2].fFree == 0) {
            shdef = rgshdef[2];
            shdef.fFree = 1;
            FChangeAiShdef(&shdef, 2);
        }
        FCreateAiShdef(2, ihuldefMiner, &vrgTDAip[vrgTDIshAip[17]]);
    }
    if ((rgshdef[12].fFree != 0 || rgshdef[12].cExist == 0) && rgplr[idPlayer].rgTech[3] >= 4 && rgplr[idPlayer].rgTech[5] >= 4) {
        FCreateAiShdef(12, ihuldefPrivateer, &vrgTDAip[vrgTDIshAip[14]]);
    }
    if (rgshdef[13].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 8 && rgplr[idPlayer].rgTech[4] >= 7 && rgplr[idPlayer].rgTech[3] >= 6) {
        FCreateAiShdef(13, ihuldefStealthBomber, &vrgTDAip[vrgTDIshAip[13]]);
    }
    if (rgshdef[14].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 11 && rgplr[idPlayer].rgTech[4] >= 12 && rgplr[idPlayer].rgTech[3] >= 15 &&
        rgplr[idPlayer].rgTech[2] >= 9) {
        FCreateAiShdef(14, ihuldefStealthBomber, &vrgTDAip[vrgTDIshAip[13]]);
    }
    if (rgshdef[4].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 6 && rgplr[idPlayer].rgTech[3] >= 13 &&
        rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(4, ihuldefBattleship, &vrgTDAip[vrgTDIshAip[Random(4) + 8]]) == 0; i++) {
        }
    }
    if (rgshdef[15].fFree != 0 && rgplr[idPlayer].rgTech[1] >= 5 && rgplr[idPlayer].rgTech[4] >= 6 && rgplr[idPlayer].rgTech[3] >= 13 &&
        rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(15, ihuldefRogue, &vrgTDAip[vrgTDIshAip[12]]) == 0; i++) {
        }
    }
    return;
}

int16_t FEnumCalcMinerDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;

    if (lpplSrc == lpplTest) {
        return 0;
    }
    id = lpplTest->id;
    b = vlpbAiPlanet[id * 16 + 1];
    if (b != 0 && (Random(100) < 25 || (b & 0x80) == 0)) {
        return b;
    }
    return 0;
}

int16_t FEnumCalcColonistDrop(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t bWant;
    uint8_t bEnemy;

    if (lpplSrc == lpplTest) {
        return 0;
    }
    id = lpplTest->id;
    bEnemy = vlpbAiPlanet[lpplTest->id * 16 + 10];
    bWant = vlpbAiPlanet[lpplTest->id * 16 + 3];
    if (bEnemy == 1 && bWant != 0 && ((bEnemy & 0x80) == 0 || Random(100) < 25)) {
        if (GetRaceStat(&rgplr[lpplTest->iPlayer], rsMajorAdv) == raMacintosh) {
            return 0;
        }
        return bWant;
    }
    return 0;
}
