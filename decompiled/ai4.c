#include "common.h"

uint16_t vrgCyberIshAip[36] = {0,   7,   14,  21,  28,  35,  42,  49,  56,  63,  70,  75,  80,  84,  91,  98,  109, 122,
                               129, 136, 143, 150, 157, 164, 171, 178, 185, 196, 207, 218, 229, 240, 251, 262, 275, 288};
uint8_t  vrgCyberAip[301] = {
    8,  4,  4,  18, 17, 18, 20, 8,  4,  4,  5,  17, 18, 20, 8,  4,  4,  4,  17, 18, 19, 8,  3,  3,  14, 17, 18, 19, 8,  4,  3,  2,  17, 18, 20, 8,  0,  0,
    18, 17, 18, 19, 8,  0,  0,  10, 17, 18, 19, 8,  0,  0,  11, 17, 18, 19, 8,  1,  1,  11, 17, 18, 19, 8,  1,  1,  11, 17, 18, 11, 44, 10, 15, 4,  4,  44,
    17, 11, 0,  0,  24, 26, 25, 10, 8,  21, 23, 23, 23, 12, 10, 8,  21, 22, 22, 22, 12, 10, 8,  14, 10, 33, 33, 33, 33, 33, 17, 20, 19, 8,  18, 20, 33, 33,
    33, 33, 33, 33, 33, 33, 33, 33, 8,  20, 19, 4,  4,  13, 17, 8,  20, 19, 4,  3,  3,  17, 8,  20, 19, 3,  2,  10, 17, 8,  19, 11, 0,  0,  0,  17, 8,  19,
    11, 0,  0,  18, 17, 8,  19, 11, 0,  0,  10, 17, 8,  19, 11, 1,  1,  11, 17, 8,  19, 11, 1,  1,  0,  17, 8,  19, 11, 1,  1,  10, 17, 8,  18, 10, 2,  2,
    3,  3,  2,  17, 20, 20, 8,  20, 10, 2,  2,  3,  3,  2,  17, 20, 20, 8,  18, 10, 0,  0,  3,  3,  2,  17, 20, 11, 8,  18, 10, 1,  1,  0,  0,  1,  17, 11,
    11, 8,  11, 10, 1,  1,  0,  0,  1,  17, 11, 11, 8,  20, 10, 1,  1,  2,  2,  1,  17, 11, 11, 8,  11, 10, 1,  1,  1,  1,  1,  17, 11, 11, 8,  11, 11, 1,
    1,  1,  20, 20, 2,  3,  3,  15, 19, 8,  11, 11, 1,  1,  1,  1,  1,  1,  19, 19, 15, 19, 8,  20, 20, 2,  2,  2,  3,  3,  3,  19, 19, 15, 19};
uint8_t vrgAiCybertronResOrder[42] = {100, 66, 131, 163, 35, 70,  102, 134, 10, 106, 72,  109, 38, 169, 137, 39, 73,  112, 14, 43,  140,
                                      171, 77, 114, 47,  18, 145, 49,  116, 81, 178, 149, 23,  86, 117, 55,  90, 154, 26,  58, 122, 186};

void DoCyberAiTurn(PROD *rgprod) {
    int32_t        rgResCost[4];
    int16_t        cSBDefenderFleets;
    int32_t        rgResAvail[4];
    FLEET         *lpflEnemy;
    int32_t        cExistColony;
    int16_t        cFlMineLayers;
    uint8_t        rgRecycleShdef[16];
    PLANET        *lppl;
    int16_t        cMineLayers;
    int16_t        ifl;
    int16_t        i;
    FLEET         *lpfl;
    int16_t        cFlDestroyers;
    int16_t        cFlArmadas;
    int16_t        iLatestSBDefender;
    int32_t        cExistCargo;
    int16_t        iroCur;
    int16_t        j;
    FLEET         *lpflAttack;
    int16_t        iLatestCargo;
    int16_t        pctValueIdeal;
    int16_t        ipl;
    int16_t       *lpiHistSize;
    int16_t        iBuilt;
    int16_t        iLatestDestroyer;
    uint16_t       cRecyclePeriod;
    int16_t        ishdefLatestSB;
    int16_t        pctValue;
    PLANET        *lpplMac;
    CYBERINFO     *lpciPlan;
    int32_t        lNewPop;
    int16_t        iLatestBattle;
    int16_t        dOffsetPlanTemp;
    int16_t        iAttackStr;
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        idPlanDst;
    int16_t        fWrite;
    int16_t        fScrap;
    SHDEF          shdef;
    uint8_t        rgRecycleSBShdef[16];
    int16_t        id;
    uint8_t       *lpb;
    int16_t        iStrDef;
    ORDER          ord;
    int16_t        cFr;
    int16_t        iSBDef;
    PLANET        *lpplEnemy;

    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    cSBDefenderFleets = 0;
    cFlMineLayers = 0;
    cMineLayers = 0;
    cFlArmadas = 0;
    cFlDestroyers = 0;
    lpiHistSize = (int16_t *)vlpbAiData;
    if (*lpiHistSize == 2) {
        fmemset(vlpbAiData, 0, 0x2000);
        *lpiHistSize = game.cPlanMax * 2 + 2;
    }
    fMarkedPlanets = 0;
    iroCur = IroEnsureAi(vrgAiCybertronResOrder, 42, &ishdefLatestSB, 17);
    EnsureCyberAiShdefs(iroCur);
    MergeAllShdefs(1);
    MergeAllShdefs(48);
    MergeAllShdefs(-16384);
    MergeAllShdefs(960);
    MergeAllShdefs(15360);
    iAttackStr = 1;
    if (game.turn > 50) {
        iAttackStr += (uint32_t)(game.turn - 50) / 10;
    }
    if (game.turn > 100) {
        iAttackStr += (uint32_t)(game.turn - 100) / 10 * ((uint32_t)game.turn / 100);
    }
    j = 3;
    if (game.turn > 130) {
        j += (uint32_t)(game.turn - 120) / 20;
    }
    if (j > 50) {
        j = 50;
    }
    vrgAiCyberArmadaPotency[0] = LOBYTE(j);
    vrgAiCyberArmadaPotency[1] = LOBYTE((int16_t)(j & 0xff) / 2);
    j = 6;
    if (game.turn > 115) {
        j += (uint32_t)(game.turn - 100) / 22;
    }
    if (j > 12) {
        j = 12;
    }
    vrgAiCyberArmadaPotency[2] = LOBYTE(j);
    vrgAiCyberArmadaPotency[3] = LOBYTE(3 >= j / 2 - 1 ? j / 2 - 1 : 3);
    memset(rgRecycleShdef, 0, 16);
    if (game.turn < 120) {
        cRecyclePeriod = 50;
    } else {
        cRecyclePeriod = game.turn < 200 ? 70 : game.turn < 400 ? 100 : 300;
    }
    CheckAiShdefStatus(4, 5, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    rgRecycleShdef[iLatestDestroyer] = 0;
    CheckAiShdefStatus(14, 15, cRecyclePeriod, &iLatestSBDefender, rgRecycleShdef);
    rgRecycleShdef[iLatestDestroyer] = 0;
    cExistCargo = CheckAiShdefStatus(2, 3, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    rgRecycleShdef[iLatestCargo] = 0;
    iLatestBattle = -1;
    for (i = 0; i <= 1; i++) {
        if (rgshdef[i * 4 + 6].fFree == 0 && game.turn - rgshdef[i * 4 + 6].turn > cRecyclePeriod) {
            fScrap = 1;
            for (j = i * 4 + 9; j >= i * 4 + 6; j--) {
                if (rgshdef[j].fFree == 0) {
                    if (rgshdef[j].cExist == 0 && (fScrap != 0 || j != i * 4 + 6)) {
                        shdef = rgshdef[j];
                        shdef.fFree = 1;
                        FChangeAiShdef(&shdef, j);
                    } else {
                        rgRecycleShdef[j] = 1;
                        fScrap = 0;
                    }
                }
            }
        }
        if (rgshdef[i * 4 + 6].fFree == 0 && (iLatestBattle == -1 || rgshdef[iLatestBattle * 4 + 6].turn < rgshdef[i * 4 + 6].turn)) {
            iLatestBattle = i;
        }
    }
    cExistColony = rgshdef[1].cExist;
    if (game.turn > 80) {
        SplitOutShdefs(rgRecycleShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[0] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[1] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 16);
        rgRecycleSBShdef[3] = 2;
        rgRecycleSBShdef[2] = 2;
        SplitOutShdefs(rgRecycleSBShdef);
    }
    lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + dOffsetPlanTemp);
    fmemset(lpciPlanTemp, 0, game.cPlanMax * sizeof(CYBERINFOTEMP));
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        lpciPlan = (CYBERINFO *)(vlpbAiData + (lppl->id * 2 + 2));
        lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + (dOffsetPlanTemp + lppl->id * 2));
        if (lpciPlan->iPktTarget > 0) {
            lpciPlan->iPktTarget += 0x7ff;
        }
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
        } else if (lppl->iPlayer == idPlayer && lppl->fStarbase != 0) {
            switch (lppl->isb) {
            case 1:
            case 3:
            case 6:
            case 8:
                if (lppl->rgwtMin[0] < 10) {
                    lpciPlanTemp->fNeedsMin1 = 1;
                }
                if (lppl->rgwtMin[1] < 10) {
                    lpciPlanTemp->fNeedsMin2 = 1;
                }
                if (lppl->rgwtMin[2] >= 10)
                    break;
                lpciPlanTemp->fNeedsMin3 = 1;
                break;
            default:
                if (lppl->rgwtMin[0] < 1000) {
                    lpciPlanTemp->fNeedsMin1 = 1;
                }
                if (lppl->rgwtMin[1] < 1000) {
                    lpciPlanTemp->fNeedsMin2 = 1;
                }
                if (lppl->rgwtMin[2] < 1000) {
                    lpciPlanTemp->fNeedsMin3 = 1;
                }
            }
        }
    }
    lpflAttack = NULL;
    lpflEnemy = NULL;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0)
            break;
        if (fMarkedPlanets == 0) {
            IdNearestColonizablePlanet(lpfl, NULL);
        }
        if (lpfl->iPlayer != idPlayer) {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        } else if (lpfl->rgcsh[14] > 0 || lpfl->rgcsh[15] > 0) {
            cSBDefenderFleets++;
        } else {
            if (lpfl->rgcsh[0] > 0) {
                cFlMineLayers++;
                cMineLayers += lpfl->rgcsh[0];
            }
            for (i = 4; i <= 13 && lpfl->rgcsh[i] <= 0; i++) {
            }
            if (i <= 13) {
                lpfl->lpflNext = lpflAttack;
                lpflAttack = lpfl;
                if (j > 5 && ((lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || lpfl->idPlanet != -1)) {
                    if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                        id = lpfl->lpplord->rgord[1].id;
                    } else {
                        id = lpfl->idPlanet;
                    }
                    lpb = vlpbAiPlanet + (10 + 16 * id);
                    if (*lpb != 0) {
                        *lpb |= 0x80;
                    }
                    cFlArmadas++;
                } else {
                    cFlDestroyers++;
                }
            } else if ((lpfl->rgcsh[2] > 0 || lpfl->rgcsh[3] > 0) && lpfl->cord > 1 && lpfl->rgwtMin[3] > 0 && lpfl->lpplord->rgord[1].grobj == grobjPlanet &&
                       lpciPlanTemp[lpfl->lpplord->rgord[1].id].cFreightersDst < 3) {
                lpciPlanTemp[lpfl->lpplord->rgord[1].id].cFreightersDst = lpciPlanTemp[lpfl->lpplord->rgord[1].id].cFreightersDst + 1;
            }
        }
    }
    UpdateProgressGauge(-926);
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
                if ((lpfl->cord > 1 && lpfl->idPlanet == -1) || FMoveToNearestStarbase(lpfl, 0) != 0)
                    continue;
            }
            if ((lpfl->rgcsh[14] > 0 || lpfl->rgcsh[15] > 0) && lpfl->idPlanet != -1) {
                iStrDef = 0;
                if (game.turn - rgshdef[14].turn < cRecyclePeriod - 10) {
                    iStrDef += lpfl->rgcsh[14];
                }
                if (game.turn - rgshdef[15].turn < cRecyclePeriod - 10) {
                    iStrDef += lpfl->rgcsh[15];
                }
                if (iStrDef > 0) {
                    lpciPlanTemp[lpfl->idPlanet].fDefended = 1;
                    lpciPlanTemp[lpfl->idPlanet].fNeedDefenders = iStrDef >= iAttackStr * 2 ? 0 : 1;
                }
            } else {
                for (i = 4; i <= 13 && lpfl->rgcsh[i] <= 0; i++) {
                }
                if (i <= 13) {
                    if (lpfl->rgcsh[4] > 0 || lpfl->rgcsh[4] > 0) {
                        if (lpfl->rgcsh[4] + lpfl->rgcsh[4] >= iAttackStr || lpfl->cord > 1) {
                            IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);
                        }
                        if (lpfl->cord == 1 && Random(100) < 75) {
                            FFindBuddyAndJoinUp(lpfl, 4, 5, 100, 200);
                        }
                    } else {
                        TargetCyberArmada(lpfl);
                        if (lpfl->cord == 1 && Random(100) < 75) {
                            if (i <= 9) {
                                FFindBuddyAndJoinUp(lpfl, 6, 9, 100, 200);
                            } else {
                                FFindBuddyAndJoinUp(lpfl, 10, 13, 100, 200);
                            }
                        }
                    }
                } else if (lpfl->cord <= 1) {
                    if (game.turn <= 5 && lpfl->rgcsh[0] > 0 && lpfl->cord == 1) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                    } else {
                        if (lpfl->rgcsh[1] > 0) {
                            idPlanDst = IdNearestColonizablePlanet(lpfl, NULL);
                            if (idPlanDst == -1) {
                                if (lpfl->lpplord->rgord[0].grobj != grobjPlanet)
                                    continue;
                                lpciPlanTemp[lpfl->lpplord->rgord[0].id].fIdleColonizers = 1;
                                continue;
                            }
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            if (lpfl->idPlanet != -1) {
                                FLookupPlanet(lpfl->idPlanet, &sel.pl);
                                if (sel.pl.iPlayer == idPlayer) {
                                    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 250);
                                    FLookupFleet(lpfl->id, &sel.fl);
                                }
                            }
                            if (idPlanDst != -1) {
                                if (FColonizeAiFleet(lpfl, idPlanDst) == 0 || lpfl->cord <= 1)
                                    continue;
                                vlpbAiPlanet[lpfl->lpplord->rgord[1].id * 16 + 15] = 4;
                                continue;
                            }
                        }
                        if (lpfl->rgcsh[2] > 0 || lpfl->rgcsh[3] > 0) {
                            if (lpfl->iplan != 4) {
                                ChangeMainObjSel(grobjFleet, lpfl->id);
                                sel.fl.iplan = 4;
                                FLookupFleet(-1, &sel.fl);
                            }
                            DoCyberFreighter(lpfl, lpciPlanTemp);
                        } else if (lpfl->rgcsh[0] > 0 && game.turn > 40 && lpfl->cord == 1) {
                            if (lpfl->cord > 1) {
                                if (lpfl->lpplord->rgord[0].grTask != grTaskNone) {
                                    ClearAiCurrentTask(lpfl, 1);
                                }
                            } else if ((cFlMineLayers <= 55 && (cFlMineLayers <= 40 || Random(3) == 0)) || FFindBuddyAndJoinUp(lpfl, 0, 0, 72, 108) == 0) {
                                if (lpfl->rgcsh[0] >= 7 && Random(5) == 0) {
                                    idPlanDst = IdRandomPlanetNearby(lpfl->pt, 105, 1);
                                    if (idPlanDst != -1 && idPlanDst != lpfl->idPlanet) {
                                        ClearAiCurrentTask(lpfl, 1);
                                        ord.id = idPlanDst;
                                        ord.grobj = grobjPlanet;
                                        ord.pt = rgptPlan[idPlanDst];
                                        ord.grTask = grTaskLayMines;
                                        ord.fValidTask = 1;
                                        ord.iWarp = 4;
                                        FMoveAiFleet(lpfl, &ord, 0);
                                        continue;
                                    }
                                }
                                if (lpfl->lpplord->rgord[0].grTask != grTaskLayMines) {
                                    ChangeMainObjSel(grobjFleet, lpfl->id);
                                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                                    sel.fl.lpplord->rgord[0].tlm.cTime = 5;
                                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 5;
                                    FLookupFleet(-1, &sel.fl);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    UpdateProgressGauge(-926);
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        lpciPlan = (CYBERINFO *)(vlpbAiData + (lppl->id * 2 + 2));
        lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + (dOffsetPlanTemp + lppl->id * 2));
        ChangeMainObjSel(grobjPlanet, lppl->id);
        InitProduction(rgprod);
        fWrite = 0;
        lNewPop = (uint32_t)(lppl->rgwtMin[3] * PctTrueMaxGrowth(idPlayer));
        GetResourcesAvailable(lppl, rgResAvail);
        GetProdQCost(lppl, rgResCost);
        for (i = 0; i < 4; i++) {
            if (rgResCost[i] > rgResAvail[i])
                goto LFinishProduction;
        }
        pctValue = PctPlanetDesirability(lppl, idPlayer);
        pctValueIdeal = PctPlanetOptValue(lppl, idPlayer);
        if (pctValue < 10) {
            i = LOWORD((int32_t)((rgResAvail[3] - rgResCost[3]) / 70)) + 1;
            AddItemToQueue(12, i, grobjPlanet, 1);
            fWrite = 1;
        } else {
            if (pctValue < pctValueIdeal && rgResAvail[3] - rgResCost[3] > 70) {
                AddItemToQueue(12, 1, grobjPlanet, 1);
                fWrite = 1;
            }
            if (lppl->fStarbase != 0) {
                switch (lppl->isb) {
                default:
                    if ((lpciPlan->fBltColony == 0 || lNewPop > 5500) && lpciPlanTemp->fIdleColonizers == 0 && cExistColony < 40 &&
                        FShouldPlanetBuildColonizer(lppl) != 0) {
                        AddItemToQueue(1, 1, grobjFleet, 1);
                        lpciPlan->fBltColony = 1;
                        fWrite = 1;
                        if (lNewPop > 15000 && game.turn < 100) {
                            AddItemToQueue(1, 1, grobjFleet, 1);
                        }
                    } else {
                        lpciPlan->fBltColony = 0;
                    }
                    if (lppl->rgwtMin[3] > 2000 && rgshdef[2].fFree == 0 && cExistCargo < 50 && lpciPlanTemp->cIdleFreighters < 1 &&
                        LpplFindClosestEnum(lppl, FEnumDropOffStage2) != 0) {
                        AddItemToQueue(iLatestCargo, 1, grobjFleet, 1);
                        fWrite = 1;
                    }
                    if (rgshdef[0].hul.ihuldef == ihuldefFrigate && Random(4) == 0 && cMineLayers < 10000) {
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
                            AddItemToQueue(0, 4, grobjFleet, 1);
                            fWrite = 1;
                        }
                    }
                    iSBDef = lpciPlanTemp->fNeedDefenders == 0 ? -1 : iLatestSBDefender;
                    if (iSBDef == -1 && lpciPlanTemp->fDefended == 0 && cSBDefenderFleets < 40) {
                        lpplEnemy = LpplFindClosestEnum(lppl, FEnumCalcEnemyPlanets);
                        if (lpplEnemy == 0 || LDistance2(rgptPlan[lppl->id], rgptPlan[lpplEnemy->id]) > 90000) {
                            iSBDef = Random(100) >= 10 ? -1 : iLatestSBDefender;
                        } else {
                            iSBDef = Random(100) >= 50 ? -1 : iLatestSBDefender;
                        }
                    }
                    if (cFlDestroyers > 120) {
                        iLatestDestroyer = -1;
                    }
                    if (cFlArmadas > 250) {
                        iLatestBattle = -1;
                    }
                    iBuilt = iAddAttackFleet(lppl, iAttackStr, iLatestDestroyer, iLatestBattle, iSBDef);
                    fWrite |= iBuilt == 0 ? 0 : 1;
                    if (iBuilt == 1) {
                        cFlArmadas++;
                    }
                    if (iBuilt == 2) {
                        cSBDefenderFleets++;
                    }
                    if (iBuilt == 3) {
                        cFlDestroyers++;
                    }
                case 1:
                case 3:
                case 6:
                case 8:
                    break;
                }
            }
        }
    LFinishProduction:
        FinishProduction(fWrite);
    }
    UpdateProgressGauge(-926);
    HandleBasicAiTasks(iroCur, rgprod, IshdefAiSBLatest(), rgResAvail, rgResCost);
    UpdateProgressGauge(-926);
    DoCyberPackets();
    UpdateProgressGauge(-926);
    FillProductionQueue();
    return;
}

void DoCyberPackets() {
    int16_t        fTwoMA;
    int32_t        rgResCost[4];
    int32_t        rgResAvail[4];
    int16_t        iWarp;
    PLANET        *lpplDst;
    PLANET        *lppl;
    int16_t        i;
    int16_t        ipl;
    int16_t        iWarpDst;
    CYBERINFO     *lpciPlan;
    PROD           rgprod[64];
    int16_t        dOffsetPlanTemp;
    int16_t        idPlanDst;
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        fWrite;
    int16_t        iPacketMax;
    int16_t        iMinLimit;
    int16_t        iPacketAdd;
    CYBERINFO     *lpciPlanDst;
    CYBERINFOTEMP *lpciPlanT;
    int16_t        iWarpSrc;
    int32_t       *plMinMax;
    int32_t        lPackets;
    int32_t        lMineral;
    int16_t        cResLeft;
    double         dDistance;
    double         dMod;
    int16_t        cPacket[3];
    int32_t        lMinNeeded;
    double         dDistanceTgt;
    int16_t        iMin;
    int32_t        t_merge_20f9_0001;

    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        if (lppl->fStarbase != 0) {
            lpciPlan = (CYBERINFO *)(vlpbAiData + (lppl->id * 2 + 2));
            lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + (dOffsetPlanTemp + lppl->id * 2));
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            fWrite = 0;
            GetResourcesAvailable(lppl, rgResAvail);
            GetProdQCost(lppl, rgResCost);
            if (lpciPlan->fNeedScanPkt == 0) {
                switch (lppl->isb) {
                case 1:
                case 3:
                case 6:
                case 8:
                    if (lppl->rgwtMin[0] > 700 || lppl->rgwtMin[1] > 700 || lppl->rgwtMin[2] > 700) {
                        iPacketMax = LOWORD((int32_t)(rgResAvail[3] / 2));
                        iPacketMax /= 5;
                        if (iPacketMax < 7) {
                            lpplDst = NULL;
                        } else {
                            lpplDst = LpplFindClosestEnum(lppl, FEnumNeedMinerals);
                        }
                        if (lpplDst != 0) {
                            lpciPlanDst = (CYBERINFO *)(vlpbAiData + (lpplDst->id * 2 + 2));
                            lpciPlanT = (CYBERINFOTEMP *)(vlpbAiData + (dOffsetPlanTemp + lpplDst->id * 2));
                            iPacketAdd = 0;
                            switch (lpplDst->isb) {
                            case 1:
                            case 3:
                            case 6:
                            case 8:
                                iMinLimit = 10;
                                break;
                            default:
                                iMinLimit = 1000;
                            }
                            if (lpplDst->rgwtMin[0] < iMinLimit && lppl->rgwtMin[0] > 700) {
                                iPacketAdd = iPacketMax >= 7 ? 7 : iPacketMax;
                                AddItemToQueue(14, iPacketAdd, grobjPlanet, 0);
                                iPacketMax -= iPacketAdd;
                                lpciPlanT->fNeedsMin1 = 0;
                                fWrite = 1;
                            }
                            if (lpplDst->rgwtMin[1] < iMinLimit && lppl->rgwtMin[1] > 700) {
                                iPacketAdd = iPacketMax >= 7 ? 7 : iPacketMax;
                                AddItemToQueue(15, iPacketAdd, grobjPlanet, 0);
                                iPacketMax -= iPacketAdd;
                                lpciPlanT->fNeedsMin2 = 0;
                                fWrite = 1;
                            }
                            if (lpplDst->rgwtMin[2] < iMinLimit && lppl->rgwtMin[2] > 700) {
                                iPacketAdd = iPacketMax >= 7 ? 7 : iPacketMax;
                                AddItemToQueue(16, iPacketAdd, grobjPlanet, 0);
                                iPacketMax -= iPacketAdd;
                                lpciPlanT->fNeedsMin3 = 0;
                                fWrite = 1;
                            }
                            if (fWrite == 0)
                                break;
                            iWarpSrc = IWarpMAFromLppl(lppl, &fTwoMA) - 4;
                            if (fTwoMA != 0) {
                                iWarpSrc++;
                            }
                            iWarpDst = IWarpMAFromLppl(lpplDst, &fTwoMA) - 4;
                            if (fTwoMA != 0) {
                                iWarpDst++;
                            }
                            sel.pl.iWarpFling = iWarpSrc >= iWarpDst ? iWarpDst : iWarpSrc;
                            sel.pl.idFling = lpplDst->id + 1;
                            FLookupPlanet(-1, &sel.pl);
                            break;
                        }
                    }
                default:
                    goto L_1fa3;
                }
                goto LFinish;
            }
        L_1fa3:
            if (lpciPlan->fNeedScanPkt == 0 && (rgplr[idPlayer].lvlAi > 1 || (rgplr[idPlayer].lvlAi == 1 && Random(3) == 0))) {
                plMinMax = (int32_t *)(vlpbAiData + (dOffsetPlanTemp + game.cPlanMax * 2));
                rgResAvail[0] -= rgResCost[0];
                rgResAvail[1] -= rgResCost[1];
                rgResAvail[2] -= rgResCost[2];
                lMineral = rgResAvail[0] - 70 + (rgResAvail[1] - 70) + (rgResAvail[2] - 70);
                cResLeft = LOWORD((int32_t)(rgResAvail[3] / 2));
                lPackets = (int16_t)((int16_t)(cResLeft - 5) / 5);
                t_merge_20f9_0001 = lMineral < (int32_t)(uint32_t)(lPackets * 70) ? lMineral : (uint32_t)(lPackets * 70);
                *plMinMax = t_merge_20f9_0001;
                if (lMineral > 150) {
                    lpplDst = LpplFindClosestEnum(lppl, FEnumPktAttack);
                } else {
                    lpplDst = NULL;
                }
                if (lpplDst != 0) {
                    cPacket[0] = 0;
                    cPacket[1] = 0;
                    cPacket[2] = 0;
                    lpciPlanDst = (CYBERINFO *)(vlpbAiData + (lpplDst->id * 2 + 2));
                    lpciPlanT = (CYBERINFOTEMP *)(vlpbAiData + (dOffsetPlanTemp + lpplDst->id * 2));
                    iPacketAdd = 0;
                    if (lpplDst->fStarbase != 0) {
                        iWarpDst = IWarpMAFromLppl(lpplDst, &fTwoMA);
                        if (fTwoMA != 0) {
                            iWarpDst++;
                        }
                    } else {
                        iWarpDst = 0;
                    }
                    iWarp = IWarpMAFromLppl(lppl, &fTwoMA) + 3;
                    dDistance = (double)(int16_t)(iWarp * iWarp);
                    dDistanceTgt = DGetDistance(rgptPlan[lppl->id].x, rgptPlan[lppl->id].y, rgptPlan[lpplDst->id].x, rgptPlan[lpplDst->id].y);
                    dMod = (double)(uint32_t)((iWarp * iWarp - iWarpDst * iWarpDst) * (100 - (lpplDst->uDefGuess + 5))) / 16000.0;
                    lMinNeeded = (int32_t)((1000 >= (lpplDst->uPopGuess + 25) * 4 ? (double)(uint32_t)((lpplDst->uPopGuess + 25) * 4) : 1000.0) / dMod);
                    lMineral = lMinNeeded < lMineral ? lMinNeeded : lMineral;
                    if (fTwoMA != 0) {
                        lMineral = (int32_t)((double)lMineral / pow(0.875, dDistanceTgt / dDistance));
                    } else {
                        lMineral = (int32_t)((double)lMineral / pow(0.75, dDistanceTgt / dDistance));
                    }
                    for (; lMineral > 0; lMineral -= 70) {
                        iMin = 0;
                        if (rgResAvail[iMin] < rgResAvail[1]) {
                            iMin = 1;
                        }
                        if (rgResAvail[iMin] < rgResAvail[2]) {
                            iMin = 2;
                        }
                        cPacket[iMin]++;
                        rgResAvail[iMin] -= 70;
                    }
                    if (cPacket[0] > 0) {
                        AddItemToQueue(14, cPacket[0], grobjPlanet, 0);
                    }
                    if (cPacket[1] > 0) {
                        AddItemToQueue(15, cPacket[1], grobjPlanet, 0);
                    }
                    if (cPacket[2] > 0) {
                        AddItemToQueue(16, cPacket[2], grobjPlanet, 0);
                    }
                    sel.pl.iWarpFling = iWarp - 4;
                    sel.pl.idFling = lpplDst->id + 1;
                    FLookupPlanet(-1, &sel.pl);
                    lpciPlanDst->iPktTarget = 3;
                    if (dDistance < dDistanceTgt) {
                        lpciPlan->fNeedScanPkt = 1;
                    }
                    lpciPlan->fLaunchedPkt = 0;
                    fWrite = 1;
                    goto LFinish;
                }
            }
            if (lpciPlan->fLaunchedPkt == 0 || lpciPlan->fNeedScanPkt != 0) {
                if (lpciPlan->fNeedScanPkt == 0) {
                    i = Random(7);
                    if (i == lpciPlan->iLstPktDir) {
                        lpciPlan->iLstPktDir = i + 1;
                    } else {
                        lpciPlan->iLstPktDir = i;
                    }
                    idPlanDst = IdGetBestScannerDest(lppl, lpciPlan->iLstPktDir);
                }
                if (lpciPlan->fNeedScanPkt != 0 || idPlanDst != -1) {
                    lpciPlanDst = (CYBERINFO *)(vlpbAiData + (idPlanDst * 2 + 2));
                    if (lpciPlanDst->iPktTarget == 0 && FAddPacketToQueue(lppl) != 0) {
                        lpciPlanDst->iPktTarget = 3;
                        lpciPlan->fLaunchedPkt = lpciPlan->fNeedScanPkt == 0 ? 1 : 0;
                        fWrite = 1;
                    } else {
                        lpciPlan->fLaunchedPkt = 0;
                    }
                    iWarp = IWarpMAFromLppl(lppl, &fTwoMA) - 1;
                    sel.pl.iWarpFling = iWarp;
                    if (lpciPlan->fNeedScanPkt == 0) {
                        sel.pl.idFling = idPlanDst;
                    } else {
                        lpciPlan->fNeedScanPkt = 0;
                    }
                    FLookupPlanet(-1, &sel.pl);
                }
            } else {
                lpciPlan->fLaunchedPkt = 0;
            }
        LFinish:
            FinishProduction(fWrite);
        }
    }
    return;
}

int16_t IdGetBestScannerDest(PLANET *lppl, int16_t iDir) {
    int16_t iDistance;
    int16_t iWarp;
    PLANET *lpplDst;
    int16_t dAdjust;
    int16_t iSize;
    POINT16 ptEdge;
    SCAN    scan;

    ptEdge = rgptPlan[lppl->id];
    iSize = 400 * game.mdSize + 400;
    iWarp = IWarpMAFromLppl(lppl, NULL) + 3;
    iDistance = iWarp * iWarp;
    ptEdge.x -= 1000;
    ptEdge.y -= 1000;
    if ((uint16_t)iDir > 7) {
        return -1;
    }
    switch (iDir) {
    case 0:
        ptEdge.x = iSize;
        break;
    case 1:
        if (iSize - ptEdge.x > ptEdge.y) {
            ptEdge.x += ptEdge.y;
            ptEdge.y = 0;
            break;
        }
        ptEdge.y -= iSize - ptEdge.x;
        ptEdge.x = iSize;
        break;
    case 2:
        ptEdge.y = 0;
        break;
    case 3:
        if (ptEdge.x > ptEdge.y) {
            ptEdge.x -= ptEdge.y;
            ptEdge.y = 0;
            break;
        }
        ptEdge.y -= ptEdge.x;
        ptEdge.x = 0;
        break;
    case 4:
        ptEdge.x = 0;
        break;
    case 5:
        if (iSize - ptEdge.x > ptEdge.y) {
            ptEdge.y += ptEdge.x;
            ptEdge.x = 0;
            break;
        }
        ptEdge.x -= iSize - ptEdge.y;
        ptEdge.y = iSize;
        break;
    case 6:
        ptEdge.y = iSize;
        break;
    case 7:
        if (ptEdge.x > ptEdge.y) {
            ptEdge.y += iSize - ptEdge.x;
            ptEdge.x = iSize;
        } else {
            ptEdge.x += iSize - ptEdge.y;
            ptEdge.y = iSize;
        }
    }
    dAdjust = Random((int32_t)((double)iSize * 0.3));
    dAdjust -= LOWORD((int32_t)((double)iSize * 0.15));
    if (ptEdge.x == 0 || ptEdge.x == iSize) {
        ptEdge.y += dAdjust;
        if (ptEdge.y > iSize) {
            dAdjust = ptEdge.y - iSize;
            ptEdge.y = iSize;
        } else if (ptEdge.y < 0) {
            dAdjust = 0 - ptEdge.y;
            ptEdge.y = 0;
        } else {
            dAdjust = 0;
        }
        if (ptEdge.x == 0) {
            ptEdge.x += dAdjust;
        } else {
            ptEdge.x -= dAdjust;
        }
    } else {
        ptEdge.x += dAdjust;
        if (ptEdge.x > iSize) {
            dAdjust = ptEdge.x - iSize;
            ptEdge.x = iSize;
        } else if (ptEdge.x < 0) {
            dAdjust = 0 - ptEdge.x;
            ptEdge.x = 0;
        } else {
            dAdjust = 0;
        }
        if (ptEdge.y == 0) {
            ptEdge.y += dAdjust;
        } else {
            ptEdge.y -= dAdjust;
        }
    }
    dAdjust = Random(iDistance);
    if (ptEdge.y == 0) {
        ptEdge.y += dAdjust;
    } else if (ptEdge.y == iSize) {
        ptEdge.y -= dAdjust;
    } else if (ptEdge.x == 0) {
        ptEdge.x += dAdjust;
    } else if (ptEdge.x == iSize) {
        ptEdge.x -= dAdjust;
    }
    ptEdge.y += 1000;
    ptEdge.x += 1000;
    if (FFindNearestObject(ptEdge, 0x21, &scan) != 0) {
        lpplDst = LpplFromId(scan.idpl);
        if ((lpplDst != 0 && lpplDst->iPlayer == idPlayer) || (int32_t)LDistance2(rgptPlan[scan.idpl], rgptPlan[lppl->id]) < (int16_t)(iDistance * iDistance)) {
            return -1;
        }
        return scan.idpl + 1;
    }
    return -1;
}

int16_t FAddPacketToQueue(PLANET *lppl) {
    int32_t rgResCost[4];
    int16_t iMineral;
    int32_t rgResAvail[4];

    iMineral = 0;
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    if (rgResAvail[iMineral] - rgResCost[iMineral] < rgResAvail[1] - rgResCost[1]) {
        iMineral = 1;
    }
    if (rgResAvail[iMineral] - rgResCost[iMineral] < rgResAvail[2] - rgResCost[2]) {
        iMineral = 2;
    }
    if (rgResAvail[iMineral] - rgResCost[iMineral] >= 170) {
        AddItemToQueue(iMineral + 14, 1, grobjPlanet, 0);
        return 1;
    }
    return 0;
}

void FillProductionQueue() {
    PLANET *lppl;
    int16_t ipl;
    PROD    rgprod[64];

    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0)
            break;
        ChangeMainObjSel(grobjPlanet, lppl->id);
        InitProduction(rgprod);
        FinishProduction(FFillProdMinesAndFactories(lppl));
    }
    return;
}

int16_t FFillProdMinesAndFactories(PLANET *lppl) {
    int32_t rgResCost[4];
    int32_t rgResAvail[4];
    int16_t iAddFactories;
    int16_t iAddMines;
    PROD    prod;
    int32_t rgAlchCost[4];
    int32_t rgMineCost[4];
    int32_t rgFactCost[4];
    int32_t rgResLeft[4];
    int16_t iAddAlchemy;
    int16_t iMaxTerra;
    int16_t i;
    int16_t iMaxFactories;
    int16_t iMaxFactBuildable;
    int16_t iMaxMines;
    int16_t fInsert;
    PROD   *lpprod;
    int16_t cAdd;
    int16_t t_scratch_m7e;
    int16_t t_scratch_m80;
    int16_t t_scratch_m7e_2;
    int16_t t_scratch_m80_2;
    int16_t t_merge_3493_0001;

    fInsert = 0;
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    for (i = 0; i < 4; i++) {
        rgResLeft[i] = rgResAvail[i] - rgResCost[i];
    }
    if (rgResLeft[3] < 0) {
        return 0;
    }
    iMaxTerra = 0;
    iMaxMines = 0;
    iAddMines = 0;
    iMaxFactories = 0;
    iAddFactories = 0;
    iAddAlchemy = 0;
    rgMineCost[3] = 0;
    rgFactCost[3] = 0;
    rgAlchCost[3] = 0;
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    while (i < lpplProdGlob->iprodMac) {
        if (lpprod->grobj == grobjPlanet && lpprod->iItem == mdIdleMine) {
            iAddMines += lpprod->cItem;
        }
        if (lpprod->grobj == grobjPlanet && lpprod->iItem == mdIdleFactory) {
            iAddFactories += lpprod->cItem;
        }
        i++;
        lpprod++;
    }
    for (i = cProdGlob - 1; i >= 0; i--) {
        if (pProdGlob[i].grobj == grobjPlanet) {
            if (pProdGlob[i].iItem == mdIdleMine) {
                t_scratch_m7e = CMaxOperableMines(lppl, idPlayer, 0);
                if (0 > t_scratch_m7e - CMinesOperating(lppl) - iAddMines) {
                    iMaxMines = 0;
                } else {
                    t_scratch_m80 = CMaxOperableMines(lppl, idPlayer, 0);
                    iMaxMines = t_scratch_m80 - CMinesOperating(lppl) - iAddMines;
                }
                GetProductionCosts(lppl, pProdGlob + i, rgMineCost, idPlayer, 1);
            }
            if (pProdGlob[i].iItem == mdIdleFactory) {
                t_scratch_m7e_2 = CMaxOperableFactories(lppl, idPlayer, 0);
                if (0 > t_scratch_m7e_2 - CFactoriesOperating(lppl) - iAddFactories) {
                    iMaxFactories = 0;
                } else {
                    t_scratch_m80_2 = CMaxOperableFactories(lppl, idPlayer, 0);
                    iMaxFactories = t_scratch_m80_2 - CFactoriesOperating(lppl) - iAddFactories;
                }
                GetProductionCosts(lppl, pProdGlob + i, rgFactCost, idPlayer, 1);
            }
            if (pProdGlob[i].iItem == mdIdleAlchemy) {
                GetProductionCosts(lppl, pProdGlob + i, rgAlchCost, idPlayer, 1);
            }
            if (pProdGlob[i].iItem == mdIdleTerraform) {
                iMaxTerra = pProdGlob[i].cItem;
            }
        }
    }
    if (iMaxTerra > 0 && rgplr[idPlayer].idAi == 5) {
        cAdd = LOWORD((int32_t)((rgResLeft[3] + 69) / 70));
        if (cAdd > iMaxTerra) {
            cAdd = iMaxTerra;
        }
        if (cAdd > 0) {
            AddItemToQueue(12, cAdd, grobjPlanet, 0);
            return 1;
        }
    }
    if (rgResLeft[0] <= 0 || rgResLeft[1] <= 0 || rgResLeft[2] <= 0) {
        if (lppl->lpplprod != 0) {
            prod = lppl->lpplprod->rgprod[0];
            if (prod.grobj == grobjPlanet && (prod.iItem == iobjAlchemy || prod.iItem == mdIdleAlchemy)) {
                return 0;
            }
        }
        if (iMaxMines > 0) {
            iAddMines = iMaxMines >= LOWORD((int32_t)(rgResLeft[3] / rgMineCost[3])) ? LOWORD((int32_t)(rgResLeft[3] / rgMineCost[3])) : iMaxMines;
        } else {
            iAddMines = 0;
        }
        rgResLeft[3] -= (uint32_t)(iAddMines * rgMineCost[3]);
        iAddFactories = 0;
        iAddAlchemy = 0;
        fInsert = 1;
    } else {
        iMaxFactBuildable = iMaxFactories;
        if (iMaxFactories > 0) {
            if (gd.fTutorial == 0) {
                iMaxFactories =
                    iMaxFactories >= LOWORD((int32_t)(rgResLeft[2] / rgFactCost[2])) ? LOWORD((int32_t)(rgResLeft[2] / rgFactCost[2])) : iMaxFactories;
            } else {
                t_merge_3493_0001 =
                    (iMaxFactories >= LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0])) ? LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0])) : iMaxFactories) >=
                            LOWORD((int32_t)(rgResLeft[1] / rgFactCost[1]))
                        ? LOWORD((int32_t)(rgResLeft[1] / rgFactCost[1]))
                    : iMaxFactories < LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0])) ? iMaxFactories
                                                                                      : LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0]));
                if (t_merge_3493_0001 >= LOWORD((int32_t)(rgResLeft[2] / rgFactCost[2]))) {
                    iMaxFactories = LOWORD((int32_t)(rgResLeft[2] / rgFactCost[2]));
                } else if ((iMaxFactories >= LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0]))
                                ? LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0]))
                                : iMaxFactories) >= LOWORD((int32_t)(rgResLeft[1] / rgFactCost[1]))) {
                    iMaxFactories = LOWORD((int32_t)(rgResLeft[1] / rgFactCost[1]));
                } else if (iMaxFactories >= LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0]))) {
                    iMaxFactories = LOWORD((int32_t)(rgResLeft[0] / rgFactCost[0]));
                }
            }
        }
        if (iMaxFactories > 0) {
            iAddFactories = iMaxFactories >= LOWORD((int32_t)(rgResLeft[3] / rgFactCost[3])) ? LOWORD((int32_t)(rgResLeft[3] / rgFactCost[3])) : iMaxFactories;
        } else {
            iAddFactories = 0;
        }
        rgResLeft[3] -= (uint32_t)(iAddFactories * rgFactCost[3]);
        if (iMaxMines > 0) {
            iAddMines = iMaxMines >= LOWORD((int32_t)(rgResLeft[3] / rgMineCost[3])) ? LOWORD((int32_t)(rgResLeft[3] / rgMineCost[3])) : iMaxMines;
        } else {
            iAddMines = 0;
        }
        rgResLeft[3] -= (uint32_t)(iAddMines * rgMineCost[3]);
    }
    if (rgAlchCost[3] > 0 && rgplr[idPlayer].rgTech[3] == 26 && rgplr[idPlayer].rgTech[5] == 26 && rgplr[idPlayer].rgTech[0] == 26 &&
        rgplr[idPlayer].rgTech[1] == 26 && rgplr[idPlayer].rgTech[4] == 26 && rgplr[idPlayer].rgTech[2] == 26) {
        iAddAlchemy = 0 <= LOWORD((int32_t)(rgResLeft[3] / rgAlchCost[3])) + 1 ? LOWORD((int32_t)(rgResLeft[3] / rgAlchCost[3])) + 1 : 0;
    } else {
        iAddAlchemy = 0;
    }
    rgResLeft[3] -= (uint32_t)(iAddAlchemy * rgAlchCost[3]);
    if (iAddFactories > 0) {
        AddItemToQueue(7, iAddFactories, grobjPlanet, 1);
    }
    if (iAddMines > 0) {
        AddItemToQueue(8, iAddMines, grobjPlanet, 0);
    }
    if (iAddAlchemy > 0 && game.turn > 100) {
        AddItemToQueue(11, iAddAlchemy, grobjPlanet, fInsert == 0 ? 1 : 0);
    }
    if (iAddMines + iAddFactories + iAddAlchemy > 0) {
        return 1;
    }
    return 0;
}

void DoCyberFreighter(FLEET *lpfl, CYBERINFOTEMP *lpciPlanTemp) {
    ORDER   ord;
    PLANET *lpplDst;
    PLANET *lpplCur;
    int16_t fDropOff;
    int16_t idPlanDst;
    SCAN    scan;

    fDropOff = 0;
    lpplCur = LpplFromId(lpfl->idPlanet);
    if (lpplCur == 0) {
        if (FFindNearestObject(lpfl->pt, 0x21, &scan) != 0) {
            lpplDst = LpplFromId(scan.idpl);
        } else {
            lpplDst = NULL;
        }
    } else {
        if (lpplCur->iPlayer == idPlayer) {
            if (lpplCur->rgwtMin[3] > 2000) {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 1000);
                FLookupFleet(lpfl->id, &sel.fl);
                fDropOff = 1;
            } else {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, -1000);
                FLookupFleet(lpfl->id, &sel.fl);
            }
        } else if (lpplCur->iPlayer != -1 && GetRaceStat(&rgplr[lpplCur->iPlayer], rsMajorAdv) != raMacintosh && lpplCur->fStarbase == 0) {
            FLookupFleet(lpfl->id, &sel.fl);
            idPlanDst = lpplCur->id;
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
            FMoveToNearestStarbase(lpfl, 0);
            FLookupFleet(lpfl->id, &sel.fl);
        } else {
            fDropOff = lpfl->rgwtMin[3] <= 0 ? 0 : 1;
        }
        if (fDropOff != 0) {
            lpplDst = LpplFindClosestEnum(lpplCur, FEnumDropOffStage1);
            if (lpplDst == 0) {
                lpplDst = LpplFindClosestEnum(lpplCur, FEnumDropOffStage2);
            }
            if (lpplDst != 0) {
                if (lpciPlanTemp[lpplDst->id].cFreightersDst < 3) {
                    lpciPlanTemp[lpplDst->id].cFreightersDst = lpciPlanTemp[lpplDst->id].cFreightersDst + 1;
                }
            } else if (lpplCur->iPlayer == idPlayer) {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, -1000);
                FLookupFleet(lpfl->id, &sel.fl);
                if (lpciPlanTemp[lpplCur->id].cIdleFreighters < 3) {
                    lpciPlanTemp[lpplCur->id].cIdleFreighters = lpciPlanTemp[lpplCur->id].cIdleFreighters + 1;
                }
            }
        } else {
            lpplDst = LpplFindClosestEnum(lpplCur, FEnumPickUp);
        }
    }
    if (lpplDst != 0) {
        memset(&ord, 0, sizeof(ORDER));
        ord.pt = rgptPlan[lpplDst->id];
        ord.grobj = grobjPlanet;
        ord.id = lpplDst->id;
        ord.grTask = grTaskNone;
        ord.fValidTask = 1;
        ord.iWarp = (uint16_t)IFindIdealWarp(lpfl, 1);
        if (FMoveAiFleet(lpfl, &ord, 0) != 0) {
            lpfl->fMark = 1;
        }
    }
    return;
}

int16_t FEnumDropOffStage1(PLANET *lpplSrc, PLANET *lpplTest) {
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        dOffsetPlanTemp;

    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + dOffsetPlanTemp);
    if (lpciPlanTemp[lpplTest->id].cFreightersDst == 3) {
        return 0;
    }
    if (lpplTest->iPlayer == idPlayer && lpplTest->rgwtMin[3] < 200 && lpciPlanTemp[lpplTest->id].cFreightersDst == 0) {
        if (LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]) <= 160000) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int16_t FEnumDropOffStage2(PLANET *lpplSrc, PLANET *lpplTest) {
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        dOffsetPlanTemp;

    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + dOffsetPlanTemp);
    if (lpciPlanTemp[lpplTest->id].cFreightersDst == 3) {
        return 0;
    }
    if (lpplTest->iPlayer != idPlayer || (uint32_t)(lpciPlanTemp[lpplTest->id].cFreightersDst * 210) + lpplTest->rgwtMin[3] >= 1000) {
        return 0;
    }
    if (LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]) <= 160000) {
        return 1;
    }
    return 0;
}

int16_t FEnumPickUp(PLANET *lpplSrc, PLANET *lpplTest) {
    if (lpplTest->iPlayer == idPlayer && lpplTest->rgwtMin[3] > 2200 && lpplTest->fStarbase != 0) {
        return 1;
    }
    return 0;
}

int16_t FEnumNeedMinerals(PLANET *lpplSrc, PLANET *lpplTest) {
    double         dDistance;
    int16_t        iWarpSrc;
    int16_t        fTwoMA;
    int16_t        iMinLimit;
    CYBERINFO     *lpciPlan;
    int16_t        iWarpDst;
    CYBERINFOTEMP *lpciPlanTemp;
    int16_t        dOffsetPlanTemp;

    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    lpciPlan = (CYBERINFO *)(vlpbAiData + 2);
    lpciPlanTemp = (CYBERINFOTEMP *)(vlpbAiData + dOffsetPlanTemp);
    switch (lpplTest->isb) {
    case 1:
    case 3:
    case 6:
    case 8:
        iMinLimit = 10;
        break;
    default:
        iMinLimit = 1000;
    }
    if (lpplTest->iPlayer != idPlayer || lpplTest->fStarbase == 0 || lpciPlan[lpplTest->id].iPktTarget > 0) {
        return 0;
    }
    if ((lpplSrc->rgwtMin[0] > 700 && lpciPlanTemp[lpplTest->id].fNeedsMin1 != 0) ||
        (lpplSrc->rgwtMin[1] > 700 && lpciPlanTemp[lpplTest->id].fNeedsMin2 != 0) ||
        (lpplSrc->rgwtMin[2] > 700 && lpciPlanTemp[lpplTest->id].fNeedsMin3 != 0)) {
        iWarpSrc = IWarpMAFromLppl(lpplSrc, &fTwoMA);
        if (fTwoMA != 0) {
            iWarpSrc++;
        }
        iWarpDst = IWarpMAFromLppl(lpplTest, &fTwoMA);
        if (fTwoMA != 0) {
            iWarpDst++;
        }
        dDistance = iWarpSrc >= iWarpDst ? (double)iWarpDst : (double)iWarpSrc;
        dDistance = dDistance * dDistance * 3.5;
        if ((double)LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]) <= dDistance * dDistance) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int16_t FEnumPktAttack(PLANET *lpplSrc, PLANET *lpplTest) {
    double     dDistance;
    int16_t    fTwoMA;
    double     dMod;
    int16_t    iWarp;
    int32_t   *plMinMax;
    int32_t    lMineral;
    int32_t    lMinNeeded;
    CYBERINFO *lpciPlan;
    int16_t    iWarpDst;
    int16_t    dOffsetPlanTemp;
    double     dDistanceTgt;
    int16_t    t_call_4345;
    double     t_scratch_m3c;
    double     t_scratch_m3c_2;

    lMineral = 0;
    iWarpDst = 0;
    dOffsetPlanTemp = game.cPlanMax * 2 + 2;
    lpciPlan = (CYBERINFO *)(vlpbAiData + 2);
    plMinMax = (int32_t *)(vlpbAiData + (dOffsetPlanTemp + game.cPlanMax * 2));
    if (lpplTest->iPlayer == idPlayer || lpplTest->iPlayer == -1 || lpciPlan[lpplTest->id].iPktTarget > 0 ||
        GetRaceStat(&rgplr[lpplTest->iPlayer], rsMajorAdv) == raMacintosh) {
        return 0;
    }
    if (lpplTest->fStarbase != 0) {
        if (rglpshdefSB[lpplTest->iPlayer][lpplTest->isb].det != 7) {
            return 0;
        }
        iWarpDst = IWarpMAFromLppl(lpplTest, &fTwoMA);
        if (fTwoMA != 0) {
            iWarpDst++;
        }
    }
    t_call_4345 = IWarpMAFromLppl(lpplSrc, &fTwoMA);
    iWarp = t_call_4345 + 3;
    dDistance = (double)(int16_t)(t_call_4345 + 3);
    if (iWarp == iWarpDst) {
        return 0;
    }
    dDistance *= dDistance;
    dDistanceTgt = DGetDistance(rgptPlan[lpplSrc->id].x, rgptPlan[lpplSrc->id].y, rgptPlan[lpplTest->id].x, rgptPlan[lpplTest->id].y);
    if (dDistanceTgt <= dDistance * 2.5) {
        if (fTwoMA != 0) {
            t_scratch_m3c = (double)*plMinMax;
            lMineral = (int32_t)(t_scratch_m3c * pow(0.875, dDistanceTgt / dDistance));
        } else {
            t_scratch_m3c_2 = (double)*plMinMax;
            lMineral = (int32_t)(t_scratch_m3c_2 * pow(0.75, dDistanceTgt / dDistance));
        }
        if (lpplTest->uDefGuess > 99 || lpplTest->uPopGuess == 0) {
            return 0;
        }
        dMod = (double)(uint32_t)((iWarp * iWarp - iWarpDst * iWarpDst) * (100 - (lpplTest->uDefGuess + 5))) / 16000.0;
        lMinNeeded = (int32_t)((1000 >= (lpplTest->uPopGuess + 25) * 4 ? (double)(uint32_t)((lpplTest->uPopGuess + 25) * 4) : 1000.0) / dMod);
        if (lMinNeeded <= lMineral) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int16_t FEnumCalcEnemyPlanets(PLANET *lpplSrc, PLANET *lpplTest) {
    if (lpplTest->iPlayer != idPlayer && lpplTest->iPlayer != -1) {
        return 1;
    }
    return 0;
}

int16_t iBuildCyberStarbase(PLANET *lppl) {
    int16_t ishdefSB;
    PROD    rgprod[64];

    if (lppl->fStarbase != 0 || PctPlanetDesirability(lppl, idPlayer) < 15 || lppl->rgwtMin[3] < 500) {
        return -1;
    }
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    if (lppl->rgMinConc[0] > 15 && lppl->rgMinConc[1] > 15 && lppl->rgMinConc[2] > 15) {
        ishdefSB = IshdefAiSBLatest();
    } else {
        ishdefSB = IshdefAiSBLatestOF();
    }
    return ishdefSB;
}

void EnsureCyberAiShdefs(int16_t iroCur) {
    int16_t low;
    int16_t ish;
    int16_t ishCur;
    int16_t i;
    int16_t high;
    SHDEF   shdef;

    if (rgshdef[0].fFree == 0 && rgshdef[0].hul.ihuldef != ihuldefFrigate && rgplr[idPlayer].lvlAi > 1 && rgshdef[0].cExist == 0 && game.turn > 5) {
        shdef = rgshdef[0];
        shdef.fFree = 1;
        FChangeAiShdef(&shdef, 0);
    }
    if (rgshdef[0].fFree != 0) {
        FCreateAiShdef(0, 5, &vrgCyberAip[vrgCyberIshAip[12]]);
    }
    if (rgshdef[4].fFree != 0 && game.turn > 30 && (game.turn > 75 || FCreateAiShdef(4, 6, &vrgCyberAip[vrgCyberIshAip[0]]) == 0)) {
        for (i = 5; i > 0 && FCreateAiShdef(4, 6, &vrgCyberAip[vrgCyberIshAip[Random(i)]]) == 0; i--) {
        }
    }
    if (rgshdef[5].fFree != 0 && rgshdef[4].fFree == 0 && game.turn > rgshdef[4].turn + 20 &&
        (game.turn > 75 || FCreateAiShdef(5, 6, &vrgCyberAip[vrgCyberIshAip[5]]) == 0)) {
        for (i = 5; i > 0 && FCreateAiShdef(5, 6, &vrgCyberAip[vrgCyberIshAip[Random(i) + 5]]) == 0; i--) {
        }
    }
    if (rgshdef[2].fFree != 0 && game.turn > 20) {
        FCreateAiShdef(2, 11, &vrgCyberAip[vrgCyberIshAip[10]]);
    }
    if (rgshdef[3].fFree != 0 && rgshdef[2].fFree == 0 && game.turn > rgshdef[2].turn + 20) {
        FCreateAiShdef(3, 11, &vrgCyberAip[vrgCyberIshAip[11]]);
    }
    for (ish = 6; ish <= 10; ish += 4) {
        if (rgshdef[ish].fFree != 0 && ((ish == 6 && game.turn > 40) || (rgshdef[6].fFree == 0 && game.turn > rgshdef[6].turn + 30))) {
            ishCur = ish + 2;
            for (i = 3; i > 0; i--) {
                if (FCreateAiShdef(ishCur, 29, &vrgCyberAip[vrgCyberIshAip[Random(i) + 33]]) != 0) {
                    ishCur--;
                    break;
                }
            }
            for (i = 4; i > 0; i--) {
                if (FCreateAiShdef(ishCur, 9, &vrgCyberAip[vrgCyberIshAip[Random(i) + 29]]) != 0) {
                    ishCur--;
                    break;
                }
            }
            for (i = 3; i > 0; i--) {
                if (FCreateAiShdef(ishCur, 9, &vrgCyberAip[vrgCyberIshAip[Random(i) + 26]]) != 0) {
                    ishCur--;
                    break;
                }
            }
            if (ishCur >= ish) {
                high = 9 - (ishCur - ish) * 3;
                low = 26 - (9 - (ishCur - ish) * 3);
                while (1) {
                    for (i = high; i > 0 && FCreateAiShdef(ishCur, 7, &vrgCyberAip[vrgCyberIshAip[Random(i) + low]]) == 0; i--) {
                    }
                    ishCur--;
                    if (ishCur < ish)
                        break;
                    high = 3;
                    low = ishCur == ish ? 17 : 20;
                }
            }
            if (FCreateAiShdef(ish + 3, 29, &vrgCyberAip[vrgCyberIshAip[16]]) == 0 && FCreateAiShdef(ish + 3, 9, &vrgCyberAip[vrgCyberIshAip[15]]) == 0 &&
                FCreateAiShdef(ish + 3, 19, &vrgCyberAip[vrgCyberIshAip[14]]) == 0) {
                FCreateAiShdef(ish + 3, 19, &vrgCyberAip[vrgCyberIshAip[13]]);
            }
        }
    }
    for (ish = 14; ish <= 15; ish++) {
        if (rgshdef[ish].fFree != 0 && ((ish == 14 && game.turn > 30) || (rgshdef[14].fFree == 0 && game.turn > rgshdef[14].turn + 20))) {
            for (i = 7; i > 0 && FCreateAiShdef(ish, 9, &vrgCyberAip[vrgCyberIshAip[Random(i) + 26]]) == 0; i--) {
            }
            if (i == 0) {
                for (i = 9; i > 0 && FCreateAiShdef(ish, 7, &vrgCyberAip[vrgCyberIshAip[Random(i) + 17]]) == 0; i--) {
                }
            }
            if (i == 0) {
                for (i = 10; i > 0 && FCreateAiShdef(ish, 6, &vrgCyberAip[vrgCyberIshAip[Random(i)]]) == 0; i--) {
                }
            }
        }
    }
    return;
}

int16_t iAddAttackFleet(PLANET *lppl, int16_t iAttackStr, int16_t iBestDestroyer, int16_t iBestBattle, int16_t iBestSBDefender) {
    int16_t fRet;
    int16_t iMaxFactories;
    int16_t iMaxMines;
    int16_t iRand;
    int16_t t_scratch_mc;
    int16_t t_scratch_me;
    int16_t t_scratch_mc_2;
    int16_t t_scratch_me_2;
    int16_t t_scratch_mc_3;

    fRet = 0;
    iRand = Random(100);
    t_scratch_mc = CMaxOperableMines(lppl, idPlayer, 0);
    if (0 > t_scratch_mc - CMinesOperating(lppl)) {
        iMaxMines = 0;
    } else {
        t_scratch_me = CMaxOperableMines(lppl, idPlayer, 0);
        iMaxMines = t_scratch_me - CMinesOperating(lppl);
    }
    t_scratch_mc_2 = CMaxOperableFactories(lppl, idPlayer, 0);
    if (0 > t_scratch_mc_2 - CFactoriesOperating(lppl)) {
        iMaxFactories = 0;
    } else {
        t_scratch_me_2 = CMaxOperableFactories(lppl, idPlayer, 0);
        iMaxFactories = t_scratch_me_2 - CFactoriesOperating(lppl);
    }
    t_scratch_mc_3 = Random(100);
    if (t_scratch_mc_3 < (iMaxMines < 100 || iMaxFactories < 100 ? 90 : 60)) {
        return 0;
    }
    if (iBestBattle != -1 && iRand > 50) {
        if (rgshdef[iBestBattle * 4 + 6].fFree == 0) {
            AddItemToQueue(iBestBattle * 4 + 6, 2, grobjFleet, 1);
        }
        if (rgshdef[iBestBattle * 4 + 7].fFree == 0) {
            AddItemToQueue(iBestBattle * 4 + 7, 2, grobjFleet, 1);
        }
        if (rgshdef[iBestBattle * 4 + 8].fFree == 0 && Random(100) < 75) {
            AddItemToQueue(iBestBattle * 4 + 8, 1, grobjFleet, 1);
        }
        if (rgshdef[iBestBattle * 4 + 9].fFree == 0 && Random(100) < 50) {
            AddItemToQueue(iBestBattle * 4 + 9, 1, grobjFleet, 1);
        }
        return 1;
    }
    if (iBestSBDefender != -1 && iRand > 25) {
        AddItemToQueue(iBestSBDefender, 1, grobjFleet, 1);
        return 2;
    }
    if (iBestDestroyer != -1) {
        AddItemToQueue(iBestDestroyer, 1, grobjFleet, 1);
        return 3;
    }
    return 0;
}

void TargetCyberArmada(FLEET *lpfl) {
    FLEET  *lpflTarget;
    ORDER   ord;
    PLANET *lppl;
    int16_t cshBomb;
    int16_t cshWar;
    PLANET *lpplTarget;

    if (lpfl->cord > 1) {
        ord = lpfl->lpplord->rgord[1];
        if (LDistance2(lpfl->pt, ord.pt) <= 62500 || ord.grobj != grobjFleet) {
            if (ord.grobj == grobjFleet) {
                return;
            }
            if (ord.grobj == grobjPlanet) {
                lppl = LpplFromId(ord.id);
                if (lppl == 0 || ((lppl->iPlayer != -1 && (lppl->iPlayer != idPlayer || lppl->fStarbase != 0)) || lppl->turn != game.turn)) {
                    return;
                }
            }
        }
    }
    cshWar = 0;
    cshWar += lpfl->rgcsh[6];
    cshWar += lpfl->rgcsh[7];
    cshWar += lpfl->rgcsh[8] * 2;
    cshWar += lpfl->rgcsh[10];
    cshWar += lpfl->rgcsh[11];
    cshWar += lpfl->rgcsh[12] * 2;
    cshBomb = lpfl->rgcsh[9] + lpfl->rgcsh[13];
    lpfl->fMark = 1;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (lpfl->idPlanet == -1) {
        MoveToNearestPlanetOrEnemy(lpfl, 450);
    } else {
        lppl = LpplFromId(lpfl->idPlanet);
        if (lppl->iPlayer == idPlayer) {
            if ((cshWar < vrgAiArmadaPotency[0] || cshBomb < vrgAiArmadaPotency[2]) &&
                (rgplr[idPlayer].lvlAi <= 1 || (cshWar <= vrgAiArmadaPotency[0] * 2 && cshWar < 60) ||
                 (Random(10) >= 5 && (cshWar <= vrgAiArmadaPotency[0] * 3 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7)))) {
                return;
            }
        } else if (cshWar < vrgAiArmadaPotency[1] || cshBomb < vrgAiArmadaPotency[3]) {
            ClearAiCurrentTask(lpfl, 0);
            if (rgplr[idPlayer].lvlAi <= 1 || ((cshWar <= vrgAiArmadaPotency[0] * 2 || Random(10) >= 5) &&
                                               (cshWar <= vrgAiArmadaPotency[0] * 4 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
                lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
                goto TargetEveryArmada;
            }
        } else if (lppl->iPlayer != -1) {
            return;
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
                return;
            }
            ord.id = lpflTarget->id;
            ord.grobj = grobjFleet;
            ord.pt = lpflTarget->pt;
        }
        ord.grTask = grTaskNone;
        ord.fValidTask = 1;
        ord.iWarp = 4;
        if (FMoveAiFleet(lpfl, &ord, 0) == 0) {
        }
    }
    return;
}
