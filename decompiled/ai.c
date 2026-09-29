#include "common.h"

uint8_t vrgAiRobotoidResOrder[36] = {66, 99,  35, 100, 2, 131, 70, 37,  102, 164, 133, 6,   39,  106, 6,   135, 42, 73,
                                     76, 109, 46, 112, 9, 138, 80, 170, 15,  52,  84,  144, 172, 56,  147, 120, 22, 122};
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
    if (FLoadGame(szBase, szExt) != 0) {
        if (rgplr[idPlayer].fDead == 0x0) {
            vlpbAiPlanet = LpAlloc(game.cPlanMax * 16, htMisc);
            vrglpplAi = LpAlloc(game.cPlanMax * sizeof(PLANET *), htMisc);
            if (vlpbAiData == 0x0) {
                vlpbAiData = LpAlloc(0x2000, htMisc);
                if (vlpbAiData != 0x0) {
                    RawStore16(vlpbAiData, 0x2);
                }
            }
            if (vlpbAiPlanet != 0x0 && vlpbAiData != 0x0 && vrglpplAi != 0x0) {
                fmemset(vlpbAiPlanet, 0, game.cPlanMax * 16);
                ComputeShdefPowers();
                MarkPlanetsUnderAttack();
                IncreaseAIMinefieldSizes();
                InitRandomPlanetList();
                if (wMdPlr != 0xffff) {
                    rgplr[iPlayer].wMdPlr = wMdPlr;
                }
                if (rgplr[iPlayer].idAi <= 0x7) {
                    switch (rgplr[iPlayer].idAi) {
                    case 0:
                        DoRobotoidAiTurn(rgprod);
                        break;
                    case 4:
                        DoCyberAiTurn(rgprod);
                        break;
                    case 5:
                        DoMacintiAiTurn(rgprod);
                        break;
                    case 1:
                        DoTurinDroneAiTurn(rgprod);
                        break;
                    case 7:
                        DoMaidAiTurn(rgprod);
                        break;
                    case 2:
                        DoAutomitronAiTurn(rgprod);
                        break;
                    case 3:
                        DoRototillAiTurn(rgprod);
                    case 6:
                    }
                }
            }
        }
        FWriteLogFile(szBase, iPlayer);
        FWriteHistFile(iPlayer);
        if (vrglpplAi != 0x0) {
            FreeLp(vrglpplAi, htMisc);
            vrglpplAi = 0x0;
        }
        if (vlpbAiData != 0x0) {
            FreeLp(vlpbAiPlanet, htMisc);
            FreeLp(vlpbAiData, htMisc);
            vlpbAiPlanet = 0x0;
            vlpbAiData = 0x0;
        }
        idPlayer = idSav;
        fAi = 0;
    } else {
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
    int16_t  t_call_1269;
    PLANET  *t_merge_1bdd_0001;
    PLANET  *t_call_1cbc;

    iAiLvl = rgplr[idPlayer].lvlAi;
    iPlanet = rgplr[idPlayer].idPlanetHome;
    iroCur = IroEnsureAi(vrgAiRobotoidResOrder, 36, &ishdefSBLatest, game.turn >= 0xa ? 15 : 0);
    if (game.turn > 0x32) {
        MergeAllShdefs(1788);
        MergeAllShdefs(1);
        MergeAllShdefs(-16384);
    }
    j = 4;
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
    CheckAiShdefStatus(14, 15, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    for (i = 14; i <= 15; i++) {
        if (rgRecycleShdef[i] != 0x0 && rgshdef[i].fFree == 0x0 && rgshdef[i].hul.ihuldef == ihuldefNubian) {
            rgRecycleShdef[i] = 0x0;
        }
    }
    cExistCargo = CheckAiShdefStatus(11, 13, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(9, 10, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(2, 5, cRecyclePeriod, &iLatestMeta, rgRecycleShdef);
    CheckAiShdefStatus(6, 7, (uint32_t)(0x3 * cRecyclePeriod) / 0x2, &iLatestBattle, rgRecycleShdef);
    if (game.turn > 0x50) {
        SplitOutShdefs(rgRecycleShdef);
        memset(rgRecycleSBShdef, 0, 0x10);
        rgRecycleSBShdef[0] = 0x2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 0x10);
        rgRecycleSBShdef[1] = 0x2;
        SplitOutShdefs(rgRecycleSBShdef);
        memset(rgRecycleSBShdef, 0, 0x10);
        rgRecycleSBShdef[13] = 0x2;
        rgRecycleSBShdef[12] = 0x2;
        rgRecycleSBShdef[11] = 0x2;
        SplitOutShdefs(rgRecycleSBShdef);
    }
    EnsureRobotoidShdefs();
    cFlDestroyers = 0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer && iLatestDestroyer != -1 && (lpfl->rgcsh[14] != 0 || lpfl->rgcsh[15] != 0)) {
            cFlDestroyers = cFlDestroyers + 1;
        }
    }
    fShouldColonize = FShouldWeBuildColonizers(&cColFleet);
    UpdateProgressGauge(-926);
    lppl = lpPlanets;
    lpplMac = lpPlanets + cPlanet;
    for (; lppl < lpplMac; lppl++) {
        if (lppl->iPlayer != idPlayer && lppl->iPlayer != -1) {
            i = lppl->uPopGuess / 250 + 1;
            if (i > 6) {
                i = 6;
            }
            if (lppl->fStarbase != 0x0) {
                i = i + 1;
            }
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE(i);
            vlpbAiPlanet[lppl->id * 16 + 9] = 0x1;
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0)
            break;
        if (lppl->fStarbase != 0x0 && lppl->rgwtMin[3] >= 200) {
            ChangeMainObjSel(grobjPlanet, lppl->id);
            InitProduction(rgprod);
            fWrite = 0;
            i = 0;
            for (lpprod = lpplProdGlob->rgprod; i < lpplProdGlob->iprodMac && (lpprod->grobj != grobjFleet || lpprod->iItem >= iobjPacketGerm); lpprod++) {
                i = i + 1;
            }
            if (i >= lpplProdGlob->iprodMac) {
                cFr = (int32_t)rgplr[idPlayer].cPlanet / 8 <= RawLoad16((uint8_t *)vlpbAiData + 0x2) * 0x4 ? RawLoad16((uint8_t *)vlpbAiData + 0x2) * 4
                                                                                                           : (int32_t)rgplr[idPlayer].cPlanet / 8;
                if (iLatestCargo != -1 && (cExistCargo < (int32_t)(cFr * 8) / 10 || (cExistCargo < cFr && Random(3) == 0))) {
                    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 1);
                    fWrite = 1;
                }
                if ((fShouldColonize != 0 || (cColFleet <= 25 && Random(RawLoad16((uint8_t *)vlpbAiData + 0x2) * 8) == 0)) && game.turn >= 0x5) {
                    if (game.turn <= 0x14) {
                        AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                    }
                    AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                    fWrite = 1;
                    l = (uint32_t)(lppl->rgwtMin[3] * (int32_t)PctTrueMaxGrowth(idPlayer));
                    cRes = CResourcesAtPlanet(lppl, idPlayer);
                    if (l > 2300 && cRes > 35 && iAiLvl > 0) {
                        AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                        if (l > 3600 && cRes > 50 && iAiLvl > 1) {
                            AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                        }
                    }
                }
                if (rgshdef[0].hul.ihuldef == ihuldefFrigate && Random(4) == 0) {
                    id = lppl->id;
                    cFr = 0;
                    ifl = 0;
                    while (1) {
                        if (ifl >= cFleet)
                            goto L_0ba3;
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0x0)
                            goto L_0ba3;
                        if (lpfl->idPlanet == id && lpfl->rgcsh[0] > 0 && lpfl->iPlayer == idPlayer)
                            break;
                        ifl = ifl + 1;
                    }
                    cFr = lpfl->rgcsh[0];
                L_0ba3:
                    if ((cFr < 10 || (cFr < 17 && Random(10) == 0)) && Random(cFr * 2 + 1) == 0) {
                        AddItemToQueue(0x0, 0x4, grobjFleet, 1);
                        fWrite = 1;
                    }
                }
                for (i = 0; i <= 2 && lppl->rgwtMin[i] >= 5000; i++) {
                }
                fTonsOfMinerals = i == 2 ? 1 : 0;
                if (iLatestBomber != -1) {
                    id = lppl->id;
                    ifl = 0;
                    while (1) {
                        if (ifl >= cFleet)
                            goto L_0d54;
                        lpfl = rglpfl[ifl];
                        if (rglpfl[ifl] == 0x0)
                            goto L_0d54;
                        if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentRobWarFleet(lpfl, 2) != 0)
                            break;
                        ifl = ifl + 1;
                    }
                    if (iLatestBomber != -1 && lpfl->rgcsh[9] + lpfl->rgcsh[10] < vrgAiArmadaPotency[2]) {
                        AddItemToQueue(iLatestBomber, fTonsOfMinerals == 0 ? 0x4 : 0x6, grobjFleet, 1);
                        fWrite = 1;
                        goto FinishProd;
                    }
                }
            L_0d54:
                if (iLatestMeta != -1 && (rgshdef[iLatestMeta].cExist < (uint32_t)((int32_t)game.cPlanMax / 7 + 0x6) || Random(2) != 0)) {
                    if (iLatestBattle == -1 || Random(2) != 0) {
                        iLatest = iLatestMeta;
                    } else {
                        iLatest = iLatestBattle;
                    }
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    GetTrueHullCost(idPlayer, &rgshdef[iLatest].hul, rgCosts);
                    for (j = 0; j < 4; j++) {
                        rgResAvail[j] = rgResAvail[j] - (uint32_t)((uint32_t)(0x3 * rgCosts[j]) / 0x5);
                        if (rgResAvail[j] < 0)
                            goto TryShip3;
                    }
                    AddItemToQueue(iLatest, fTonsOfMinerals == 0 ? 0x1 : 0x5, grobjFleet, 1);
                    fWrite = 1;
                }
            TryShip3:
                if (iLatestDestroyer != -1 && rgshdef[iLatestDestroyer].cExist < (uint32_t)((int32_t)game.cPlanMax / 12 + 0x8)) {
                    GetResourcesAvailable(lppl, rgResAvail);
                    GetProdQCost(lppl, rgResCost);
                    for (i = 0; i < 4; i++) {
                        rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                        if (rgResAvail[i] < 0)
                            goto FinishProd;
                    }
                    for (i = 0; i < 5; i++) {
                        GetTrueHullCost(idPlayer, &rgshdef[iLatestDestroyer].hul, rgCosts);
                        for (j = 0; j < 4; j++) {
                            rgResAvail[j] = rgResAvail[j] - (uint32_t)rgCosts[j];
                            if (rgResAvail[j] < 0)
                                goto FinishProd;
                        }
                        fWrite = 1;
                        AddItemToQueue(iLatestDestroyer, 0x1, grobjFleet, 1);
                    }
                }
            FinishProd:
                FinishProduction(fWrite);
            } else {
                FinishProduction(0);
            }
        }
    }
    UpdateProgressGauge(-926);
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjThing) {
            dx = lpfl->pt.x - lpfl->lpplord->rgord[1].pt.x;
            dy = lpfl->pt.y - lpfl->lpplord->rgord[1].pt.y;
            lDist = (uint32_t)((int32_t)dx * (int32_t)dx) + (uint32_t)((int32_t)dy * (int32_t)dy);
            if (lDist > 40000) {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.cord = 1;
                sel.fl.lpplord->iordMac = 0x1;
                FLookupFleet(-1, &sel.fl);
            }
        }
        if (lpfl->iPlayer == idPlayer) {
            if (lpfl->rgcsh[0] <= 0 || game.turn <= 0x28 || lpfl->cord != 1) {
                if (FIsAiAttack(lpfl) == 0) {
                    if (FIsAiTransport(lpfl) != 0) {
                        idPlanDst = -1;
                        if (lpfl->cord <= 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone) {
                            idPlanDst = lpfl->idPlanet;
                        } else if (lpfl->lpplord->rgord[1].grobj == grobjPlanet) {
                            idPlanDst = lpfl->lpplord->rgord[1].id;
                        }
                        if (idPlanDst != -1) {
                            lppl = LpplFromId(idPlanDst);
                            if ((lppl == 0x0 || lppl->iPlayer != idPlayer) && lpfl->rgwtMin[3] == 0) {
                                ChangeMainObjSel(grobjFleet, lpfl->id);
                                sel.fl.cord = 1;
                                sel.fl.lpplord->iordMac = 0x1;
                                FLookupFleet(-1, &sel.fl);
                                ClearAiCurrentTask(lpfl, 0);
                            }
                        }
                    }
                } else {
                    lpfl->lpflNext = lpflAttack;
                    lpflAttack = lpfl;
                    if (lpfl->lpplord->rgord[0].grTask == grTaskLayMines) {
                        ClearAiCurrentTask(lpfl, 1);
                    }
                    for (j = 2; j <= 7 && lpfl->rgcsh[j] <= 0; j++) {
                    }
                    if (j <= 7 && ((lpfl->cord > 1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || lpfl->idPlanet != -1)) {
                        if (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjPlanet) {
                            id = lpfl->idPlanet;
                        } else {
                            id = lpfl->lpplord->rgord[1].id;
                        }
                        lpb = vlpbAiPlanet + (10 + 16 * id);
                        if (*lpb != 0x0) {
                            *lpb = *lpb | 0x80;
                        }
                    }
                }
            } else {
                if (lpfl->rgcsh[0] >= 7 && Random(5) == 0) {
                    t_call_1269 = IdRandomPlanetNearby(lpfl->pt, 105, 1);
                    idPlanDst = t_call_1269;
                    if (t_call_1269 != -1 && idPlanDst != lpfl->idPlanet) {
                        ClearAiCurrentTask(lpfl, 1);
                        ord.id = idPlanDst;
                        ord.grobj = grobjPlanet;
                        ord.pt = rgptPlan[idPlanDst];
                        ord.grTask = grTaskNone;
                        ord.fValidTask = 0x1;
                        ord.iWarp = 0x4;
                        FMoveAiFleet(lpfl, &ord, 0);
                        goto L_15d1;
                    }
                }
                if (lpfl->lpplord->rgord[0].grTask == grTaskNone) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                    sel.fl.lpplord->rgord[0].tlm.cTime = 0x5;
                    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 0x5;
                    FLookupFleet(-1, &sel.fl);
                    continue;
                }
            }
        L_15d1:
            if (game.turn > 0x14 || lpfl->rgcsh[0] <= 0) {
                if (lpfl->cord <= 1 && lpfl->rgcsh[1] != 0 && (game.turn >= 0x5 || game.mdStartDist == 0)) {
                    if (iAiLvl > 1 && lpfl->idPlanet != -1) {
                        lpplDrop = LpplFromId(lpfl->idPlanet);
                        if (lpplDrop != 0x0 && lpplDrop->iPlayer != -1 && lpplDrop->iPlayer != idPlayer && lpfl->rgwtMin[3] > 0 &&
                            GetRaceStat(&rgplr[lpplDrop->iPlayer], rsMajorAdv) != raMacintosh) {
                            memset(&ord, 0, sizeof(ORDER));
                            ord.pt = rgptPlan[lpplDrop->id];
                            ord.id = lpplDrop->id;
                            ord.grobj = grobjPlanet;
                            ord.fValidTask = 0x1;
                            ord.grTask = grTaskXfer;
                            ord.txp.rgia[3].iAction = iActionUnloadAll;
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            sel.fl.lpplord->rgord[0] = ord;
                            FLookupFleet(-1, &sel.fl);
                            lpplDrop = LpplFindClosestEnum(lpplDrop, FEnumOurStarbase);
                            if (lpplDrop != 0x0) {
                                memset(&ord, 0, sizeof(ORDER));
                                ord.id = lpplDrop->id;
                                ord.grobj = grobjPlanet;
                                ord.pt = rgptPlan[lpplDrop->id];
                                ord.grTask = grTaskNone;
                                ord.fValidTask = 0x1;
                                ord.iWarp = 0x4;
                                FMoveAiFleet(lpfl, &ord, 0);
                                continue;
                            }
                            continue;
                        }
                    }
                    idPlanDst = IdNearestColonizablePlanet(lpfl, &lpthWorm);
                    if (idPlanDst != -1 || lpthWorm != 0x0) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        if (lpfl->idPlanet != -1) {
                            XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 10);
                            FLookupFleet(lpfl->id, &sel.fl);
                        }
                        if (idPlanDst == -1) {
                            FGotoWormholeAiFleet(lpfl, lpthWorm);
                        } else {
                            FColonizeAiFleet(lpfl, idPlanDst);
                        }
                    } else if (lpfl->idPlanet != -1) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                    }
                }
            } else {
                ChangeMainObjSel(grobjFleet, lpfl->id);
                sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                FLookupFleet(-1, &sel.fl);
            }
        } else {
            lpfl->lpflNext = lpflEnemy;
            lpflEnemy = lpfl;
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0 || lppl->fStarbase != 0x0)
            break;
    }
    lpplHome = ipl == vclpplAi ? 0x0 : lppl;
    if (lpplHome != 0x0) {
        for (ifl = 0; ifl < cFleet; ifl++) {
            lpfl = rglpfl[ifl];
            if (rglpfl[ifl] == 0x0)
                break;
            if (lpfl->iPlayer == idPlayer && lpfl->cord <= 1 && FIsAiTransport(lpfl) != 0) {
                if (lpfl->iplan != 0x4) {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    sel.fl.iplan = 0x4;
                    FLookupFleet(-1, &sel.fl);
                }
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
                t_merge_1bdd_0001 = lppl == 0x0 ? lpplHome : lppl;
                IdTargetFreighter(lpfl, t_merge_1bdd_0001);
            }
        }
        UpdateProgressGauge(-926);
    }
    for (ifl = 0; ifl < cFleet; ifl++) {
        lpfl = rglpfl[ifl];
        if (rglpfl[ifl] == 0x0)
            break;
        if (lpfl->iPlayer == idPlayer) {
            for (i = 0; i < 16 && (lpfl->rgcsh[i] <= 0 || rgRecycleShdef[i] != 0x0); i++) {
            }
            if (i == 16) {
                if (lpfl->idPlanet != -1) {
                    t_call_1cbc = LpplFromId(lpfl->idPlanet);
                    lppl = t_call_1cbc;
                    if (t_call_1cbc != 0x0 && lppl->iPlayer == idPlayer && (lppl->fStarbase != 0x0 || Random(5) == 0)) {
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                        sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
                        FLookupFleet(-1, &sel.fl);
                        continue;
                    }
                }
                if ((lpfl->cord > 1 && lpfl->idPlanet == -1 && lpfl->lpplord->rgord[1].grobj == grobjPlanet) || FMoveToNearestStarbase(lpfl, 0) != 0)
                    continue;
            }
            i = 2;
            while (1) {
                if (i > 10)
                    goto L_1de7;
                if (lpfl->rgcsh[i] > 0)
                    break;
                i = i + 1;
            }
            IdTargetArmada(lpfl);
        L_1de7:
            if (i > 10 && FIsAiAttack(lpfl) != 0 && (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjFleet) &&
                ((cFlDestroyers <= (game.turn <= 0x78 ? 0x46 : 0x32) && (cFlDestroyers <= (game.turn <= 0x78 ? 0x3c : 0x28) || Random(3) != 0)) ||
                 ((lpfl->rgcsh[iLatestDestroyer] >= 20 && Random(20) != 0) || FFindBuddyAndJoinUp(lpfl, 14, 15, 36, 72) == 0))) {
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
        if (rgshdef[ish].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[2] >= 2 && (int16_t)rgplr[idPlayer].rgTech[3] >= (ish - 11) * 3 + 0x4 &&
            (ish == 11 || game.turn - rgshdef[ish - 1].turn > 0xe)) {
            if ((int16_t)rgplr[idPlayer].rgTech[3] >= 10) {
                for (i = 0; i < 5 && FCreateAiShdef(ish, 31, &vrgRobAip[vrgRobIshAip[Random(6) + 8]]) == 0; i++) {
                }
            } else {
                FCreateAiShdef(ish, 11, &vrgRobAip[vrgRobIshAip[ish == 11 ? 14 : 15]]);
            }
        }
    }
    if (rgshdef[14].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 6 && (int16_t)rgplr[idPlayer].rgTech[2] >= 6 && (int16_t)rgplr[idPlayer].rgTech[0] >= 2) {
        for (i = 0;
             i < 5 && (FCreateAiShdef(14, 29, &vrgRobAip[vrgRobIshAip[37]]) != 0 || FCreateAiShdef(14, 6, &vrgRobAip[vrgRobIshAip[Random(4) + 16]]) == 0);
             i++) {
        }
    }
    if (rgshdef[15].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[4] >= 10 && (int16_t)rgplr[idPlayer].rgTech[3] >= 8 &&
        (int16_t)rgplr[idPlayer].rgTech[2] >= 9 && (int16_t)rgplr[idPlayer].rgTech[1] >= 14 && FCreateAiShdef(15, 29, &vrgRobAip[vrgRobIshAip[37]]) == 0) {
        for (i = 0; i < 5 && FCreateAiShdef(15, 6, &vrgRobAip[vrgRobIshAip[Random(4) + 20]]) == 0; i++) {
        }
    }
    for (ish = 2; ish <= 5; ish++) {
        if (rgshdef[ish].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 10 && (int16_t)rgplr[idPlayer].rgTech[3] >= 10 &&
            (int16_t)rgplr[idPlayer].rgTech[2] >= 9 && (int16_t)rgplr[idPlayer].rgTech[0] >= 6 &&
            (ish == 2 || (rgshdef[ish - 1].fFree == 0x0 && game.turn - rgshdef[ish - 1].turn > 0xc))) {
            shBase = (ish - 0x2 & 0x1) == 0x0 ? 0 : 4;
            for (i = 0; i < 5 && FCreateAiShdef(ish, 31, &vrgRobAip[vrgRobIshAip[Random(4) + shBase]]) == 0; i++) {
            }
        }
    }
    for (ish = 6; ish <= 7; ish++) {
        if (rgshdef[ish].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[5] >= 4 && (int16_t)rgplr[idPlayer].rgTech[4] >= 10 &&
            (int16_t)rgplr[idPlayer].rgTech[3] >= 12 && (int16_t)rgplr[idPlayer].rgTech[2] >= 12 && (int16_t)rgplr[idPlayer].rgTech[0] >= 6 &&
            (int16_t)rgplr[idPlayer].rgTech[1] >= 15 && (ish == 6 || (rgshdef[ish - 1].fFree == 0x0 && game.turn - rgshdef[ish - 1].turn > 0x14))) {
            shBase = ish == 6 ? 27 : 31;
            for (i = 0; i < 5 && FCreateAiShdef(ish, 9, &vrgRobAip[vrgRobIshAip[Random(4) + shBase]]) == 0; i++) {
            }
        }
    }
    for (ish = 9; ish <= 10; ish++) {
        if (rgshdef[ish].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 14 &&
            ((ish == 9 || (rgshdef[ish - 1].fFree == 0x0 && game.turn - rgshdef[ish - 1].turn > 0xf)) &&
             FCreateAiShdef(ish, 9, &vrgRobAip[vrgRobIshAip[36]]) == 0)) {
            FCreateAiShdef(ish, 19, &vrgRobAip[vrgRobIshAip[ish == 9 ? 24 : 25]]);
        }
    }
    if (rgshdef[0].hul.ihuldef != ihuldefFrigate && rgplr[idPlayer].lvlAi > 0x1 && rgshdef[0].cExist == 0x0 && (int16_t)rgplr[idPlayer].rgTech[5] >= 4 &&
        (int16_t)rgplr[idPlayer].rgTech[4] >= 5 && (int16_t)rgplr[idPlayer].rgTech[3] >= 6 && (int16_t)rgplr[idPlayer].rgTech[2] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[0] >= 6) {
        shdef = rgshdef[0];
        shdef.fFree = 0x1;
        FChangeAiShdef(&shdef, 0);
        FCreateAiShdef(0, 5, &vrgRobAip[vrgRobIshAip[26]]);
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
    int32_t t_merge_30e4_0001;
    int32_t t_merge_315a_0001;

    if (lpfl->cord > 1) {
        ord = lpfl->lpplord->rgord[1];
        if (LDistance2(lpfl->pt, ord.pt) <= 62500 || ord.grobj != grobjFleet) {
            if (ord.grobj == grobjFleet) {
                return 0;
            }
            if (ord.grobj == grobjPlanet) {
                lppl = LpplFromId(ord.id);
                if (lppl == 0x0 || ((lppl->iPlayer != -1 && (lppl->iPlayer != idPlayer || lppl->fStarbase != 0x0)) || lppl->turn != game.turn)) {
                    return 0;
                }
            }
        }
    }
    cshWar = 0;
    for (ish = 2; ish <= 5; ish++) {
        cshWar = cshWar + lpfl->rgcsh[ish];
    }
    for (ish = 6; ish <= 7; ish++) {
        cshWar = cshWar + lpfl->rgcsh[ish] * 2;
    }
    cshBomb = lpfl->rgcsh[9] + lpfl->rgcsh[10];
    lpfl->fMark = 0x1;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (lpfl->idPlanet != -1) {
        lppl = LpplFromId(lpfl->idPlanet);
        if (lppl->iPlayer != idPlayer || lppl->fStarbase == 0x0) {
            if (cshWar >= vrgAiArmadaPotency[1] && cshBomb >= vrgAiArmadaPotency[3]) {
                if (lppl->iPlayer != -1) {
                    if (lppl->iPlayer != idPlayer) {
                        if (lppl->iPlayer == idPlayer) {
                            return 0;
                        }
                        lPopUs = lpfl->rgwtMin[3];
                        lPopEnemy = (int32_t)(lppl->uPopGuess * 0x4);
                        pctDef = (uint32_t)(lppl->uDefGuess * 0x6) + 6;
                        pctDef = (int32_t)((int32_t)(pctDef * 3) / 4);
                        lPopEnemy = (int32_t)((int32_t)(lPopEnemy * 100) / (100 - pctDef));
                        if (lPopEnemy >= (int32_t)(lPopUs / 5) && (lPopEnemy >= 200 || lPopUs <= 350) && (lPopEnemy >= 10 || lPopUs <= 150)) {
                            return 0;
                        }
                        cXfer = (int32_t)((int32_t)(lPopEnemy * 5) / 4);
                        t_merge_30e4_0001 = cXfer <= (int32_t)(lpfl->rgwtMin[3] / 2) ? (int32_t)(lpfl->rgwtMin[3] / 2) : cXfer;
                        if (lpfl->rgwtMin[3] < t_merge_30e4_0001) {
                            t_merge_315a_0001 = lpfl->rgwtMin[3];
                        } else if (cXfer <= (int32_t)(lpfl->rgwtMin[3] / 2)) {
                            t_merge_315a_0001 = (int32_t)(lpfl->rgwtMin[3] / 2);
                        } else {
                            t_merge_315a_0001 = cXfer;
                        }
                        cXfer = t_merge_315a_0001;
                        if (cXfer > 30000) {
                            cXfer = 30000;
                        }
                        XferAiTroopers(lpfl->id, lppl->id, LOWORD(cXfer));
                        FLookupFleet(lpfl->id, &sel.fl);
                        return 0;
                    }
                    if (lppl->rgwtMin[3] > 1000) {
                        XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, (int32_t)LOWORD(lppl->rgwtMin[3]) / 5);
                        FLookupFleet(lpfl->id, &sel.fl);
                    }
                }
            } else {
                ClearAiCurrentTask(lpfl, 0);
                if (rgplr[idPlayer].lvlAi <= 0x1 || ((cshWar <= vrgAiArmadaPotency[0] * 0x2 || Random(10) >= 5) &&
                                                     (cshWar <= vrgAiArmadaPotency[0] * 0x4 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
                    lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
                    goto TargetEveryArmada;
                }
            }
        } else if (cshWar < vrgAiArmadaPotency[0] || cshBomb < vrgAiArmadaPotency[2]) {
            if (rgplr[idPlayer].lvlAi <= 0x1 || (cshWar <= vrgAiArmadaPotency[0] * 0x2 && cshWar < 60) ||
                (Random(10) >= 5 && (cshWar <= vrgAiArmadaPotency[0] * 0x3 || Random(10) >= 7) && (cshWar <= 120 || Random(10) >= 7))) {
                return 0;
            }
        } else {
            if (sel.pl.rgwtMin[3] <= 3000) {
                if (sel.pl.rgwtMin[3] <= 2000) {
                    if (sel.pl.rgwtMin[3] <= 1000) {
                        cCol = 0;
                    } else {
                        cCol = (int32_t)(sel.pl.rgwtMin[3] / 20);
                    }
                } else {
                    cCol = (int32_t)(sel.pl.rgwtMin[3] / 15);
                }
            } else {
                cCol = (int32_t)(sel.pl.rgwtMin[3] / 10);
            }
            if (cCol > 0) {
                XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, LOWORD(cCol));
                FLookupFleet(lpfl->id, &sel.fl);
            }
        }
        if (game.fAisBand == 0x0) {
            lpplTarget = 0x0;
        } else {
            lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
        }
        if (lpplTarget == 0x0) {
            lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);
        }
    TargetEveryArmada:
        if (lpplTarget != 0x0) {
            vlpbAiPlanet[lpplTarget->id * 16 + 10] = vlpbAiPlanet[lpplTarget->id * 16 + 0xa] | 0x80;
            ord.id = lpplTarget->id;
            ord.grobj = grobjPlanet;
            ord.pt = rgptPlan[lpplTarget->id];
        } else {
            lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
            if (lpflTarget == 0x0) {
                return 0;
            }
            ord.id = lpflTarget->id;
            ord.grobj = grobjFleet;
            ord.pt = lpflTarget->pt;
        }
        ord.grTask = grTaskNone;
        ord.fValidTask = 0x1;
        ord.iWarp = 0x4;
        if (FMoveAiFleet(lpfl, &ord, 0) == 0) {
            return -1;
        }
        return 0;
    }
    MoveToNearestPlanetOrEnemy(lpfl, 150);
    return 0;
}

int16_t FPotentRobWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

    cEquiv = 0;
    for (ish = 2; ish <= 5; ish++) {
        cEquiv = cEquiv + lpfl->rgcsh[ish];
    }
    for (ish = 6; ish <= 7; ish++) {
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

int16_t FEnumCalcEnemyFleets(FLEET *lpflSrc, FLEET *lpflTest) {
    if (lpflTest->iPlayer == idPlayer) {
        return 0;
    }
    return 1;
}

int16_t FEnumCalcArmadaDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

    if (lpplSrc != lpplTest) {
        id = lpplTest->id;
        b = vlpbAiPlanet[id * 16 + 10];
        if (b != 0x0) {
            l2 = LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]);
            if (l2 < 2500) {
                b = b + 0x7;
            } else if (l2 < 10000) {
                b = b + 0x5;
            } else if (l2 < 22500) {
                b = b + 0x4;
            } else if (l2 < 40000) {
                b = b + 0x3;
            } else if (l2 < 90000) {
                b = b + 0x2;
            } else if (l2 < 250000) {
                b = b + 0x1;
            }
            if ((b & 0x80) == 0x0 || Random(4) == 0) {
                return b;
            }
        }
        return 0;
    }
    return 0;
}

int16_t FEnumCalcArmadaHumanDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

    if (lpplSrc != lpplTest) {
        id = lpplTest->id;
        if (rgplr[lpplTest->iPlayer].fAi == 0x0) {
            b = vlpbAiPlanet[id * 16 + 10];
            if (b != 0x0) {
                l2 = LDistance2(rgptPlan[lpplSrc->id], rgptPlan[lpplTest->id]);
                if (l2 < 2500) {
                    b = b + 0x7;
                } else if (l2 < 10000) {
                    b = b + 0x5;
                } else if (l2 < 22500) {
                    b = b + 0x4;
                } else if (l2 < 40000) {
                    b = b + 0x3;
                } else if (l2 < 90000) {
                    b = b + 0x2;
                } else if (l2 < 250000) {
                    b = b + 0x1;
                }
                if ((b & 0x80) == 0x0 || Random(4) == 0) {
                    return b;
                }
            }
            return 0;
        }
        return 0;
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
    uint16_t t_merge_3c51_0001;
    PLANET  *t_merge_4910_0001;
    PLANET  *t_merge_54df_0001;

    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0x0;
    cplMiners = 0;
    iroCur = IroEnsureAi(vrgAiTurinDroneResOrder, 31, &ishdefSBLatest, 15);
    if (rgshdef[13].fFree == 0x0) {
        MergeAllShdefs(-7952);
    }
    if (rgshdef[12].fFree == 0x0) {
        MergeAllShdefs(4096);
    }
    if (rgshdef[10].fFree == 0x0) {
        MergeAllShdefs(3072);
    }
    if (rgshdef[2].fFree == 0x0) {
        MergeAllShdefs(12);
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
    CheckAiShdefStatus(6, 7, cRecyclePeriod, &iLatestCruiser, rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(8, 9, cRecyclePeriod, &iLatestCargo, rgRecycleShdef);
    CheckAiShdefStatus(13, 14, cRecyclePeriod, &iLatestBomber, rgRecycleShdef);
    CheckAiShdefStatus(4, 5, cRecyclePeriod, &iLatestBattle, rgRecycleShdef);
    CheckAiShdefStatus(12, 12, cRecyclePeriod, &iLatestLayer, rgRecycleShdef);
    CheckAiShdefStatus(15, 15, cRecyclePeriod, &iLatestTroop, rgRecycleShdef);
    if ((int16_t)rgplr[idPlayer].rgTech[3] < 7) {
        iLatestMiner = -1;
    } else {
        CheckAiShdefStatus(2, 3, cRecyclePeriod, &iLatestMiner, rgRecycleShdef);
    }
    CheckAiShdefStatus(10, 11, cRecyclePeriod, &iLatestDestroyer, rgRecycleShdef);
    if (game.turn > 0x3c) {
        SplitOutShdefs(rgRecycleShdef);
    }
    EnsureTurinDroneShdefs(iroCur);
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
            cplMiners = cplMiners + 1;
        }
        if (lppl->iPlayer == idPlayer || lppl->iPlayer == -1) {
            if (lppl->iPlayer == idPlayer) {
                if (PctPlanetDesirability(lppl, idPlayer) >= 0) {
                    if (lppl->fStarbase == 0x0 || lppl->rgwtMin[3] < 200) {
                        ChangeMainObjSel(grobjPlanet, lppl->id);
                        t_merge_3c51_0001 = lppl->rgwtMin[3] < 200 ? 0x1 : 0x0;
                        sel.pl.fNoResearch = LOWORD((uint32_t)t_merge_3c51_0001);
                        if ((uint32_t)sel.pl.fNoResearch != lppl->fNoResearch) {
                            FLookupPlanet(-1, &sel.pl);
                        }
                    } else {
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
                            if (game.turn != 0x0) {
                                if (rgshdef[0].fFree == 0x0 && rgshdef[0].hul.ihuldef == ihuldefFrigate &&
                                    rgshdef[0].cExist < (uint32_t)((int32_t)game.cPlanMax / 4 >= 0x20 ? 0x20 : (int32_t)game.cPlanMax / 4) &&
                                    rgshdef[0].cExist > (uint32_t)(rgshdef[0].cBuilt / 0xa)) {
                                    AddItemToQueue(0x0, 0x1, grobjFleet, 1);
                                    fWrite = 1;
                                }
                            } else {
                                i = game.cPlanMax;
                                while (i > 0) {
                                    AddItemToQueue(0x0, 0x1, grobjFleet, 1);
                                    if (i <= 190) {
                                        i = i - 30;
                                    } else {
                                        i = i - 100;
                                    }
                                    fWrite = 1;
                                }
                            }
                            cFr = (int32_t)rgplr[idPlayer].cPlanet / 10 <= RawLoad16((uint8_t *)vlpbAiData + 0x2) * 0x2
                                      ? RawLoad16((uint8_t *)vlpbAiData + 0x2) * 2
                                      : (int32_t)rgplr[idPlayer].cPlanet / 10;
                            if ((int16_t)rgplr[idPlayer].rgTech[2] >= 5 && iLatestCargo != -1 &&
                                (cExistCargo < cFr || (cExistCargo < (int32_t)(10 * cFr) / 0x7 && Random(4) == 0))) {
                                AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 1);
                                fWrite = 1;
                            }
                            if ((cplanCol != 0x0 || cplBadGuy != 0) && rgshdef[1].cExist < 0x2) {
                                AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                                AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                                AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                                AddItemToQueue(0x1, 0x1, grobjFleet, 1);
                                fWrite = 1;
                            }
                            l = (uint32_t)(lppl->rgwtMin[3] * (int32_t)PctTrueMaxGrowth(idPlayer));
                            cRes = CResourcesAtPlanet(lppl, idPlayer);
                            if (rgshdef[12].fFree == 0x0 && Random(3) == 0) {
                                id = lppl->id;
                                cFr = 0;
                                ifl = 0;
                                while (1) {
                                    if (ifl >= cFleet)
                                        goto L_412b;
                                    lpfl = rglpfl[ifl];
                                    if (rglpfl[ifl] == 0x0)
                                        goto L_412b;
                                    if (lpfl->idPlanet == id && lpfl->rgcsh[12] > 0 && lpfl->iPlayer == idPlayer)
                                        break;
                                    ifl = ifl + 1;
                                }
                                cFr = lpfl->rgcsh[12];
                            L_412b:
                                if ((cFr < 10 || (cFr < 17 && Random(8) == 0)) && Random(cFr * 2 + 1) == 0) {
                                    AddItemToQueue(0xc, 0x3, grobjFleet, 1);
                                    fWrite = 1;
                                }
                            }
                            if (iLatestBomber != -1) {
                                id = lppl->id;
                                ifl = 0;
                                while (1) {
                                    if (ifl >= cFleet)
                                        goto L_4274;
                                    lpfl = rglpfl[ifl];
                                    if (rglpfl[ifl] == 0x0)
                                        goto L_4274;
                                    if (lpfl->idPlanet == id && lpfl->iPlayer == idPlayer && FPotentRobWarFleet(lpfl, 2) != 0)
                                        break;
                                    ifl = ifl + 1;
                                }
                                if (iLatestBomber != -1 && lpfl->rgcsh[13] + lpfl->rgcsh[14] >= vrgAiArmadaPotency[2]) {
                                    AddItemToQueue(iLatestBomber, 0x4, grobjFleet, 1);
                                    fWrite = 1;
                                    goto FinishProd;
                                }
                            }
                        L_4274:
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
                            if (iLatestDestroyer != -1 && rgshdef[iLatestDestroyer].cExist < (uint32_t)((int32_t)game.cPlanMax / 4 + 0xc)) {
                                GetResourcesAvailable(lppl, rgResAvail);
                                GetProdQCost(lppl, rgResCost);
                                for (i = 0; i < 4; i++) {
                                    rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                                    if (rgResAvail[i] < 0)
                                        goto FinishProd;
                                }
                                for (i = 0; i < 5; i++) {
                                    GetTrueHullCost(idPlayer, &rgshdef[iLatestDestroyer].hul, rgCosts);
                                    for (j = 0; j < 4; j++) {
                                        rgResAvail[j] = rgResAvail[j] - (uint32_t)rgCosts[j];
                                        if (rgResAvail[j] < 0)
                                            goto FinishProd;
                                    }
                                    fWrite = 1;
                                    AddItemToQueue(iLatestDestroyer, 0x1, grobjFleet, 1);
                                }
                            }
                            if (iLatestTroop != -1 && rgshdef[15].fFree == 0x0 &&
                                rgshdef[iLatestTroop].cExist < (uint32_t)((int32_t)game.cPlanMax / 12 + 0x8)) {
                                GetResourcesAvailable(lppl, rgResAvail);
                                GetProdQCost(lppl, rgResCost);
                                for (i = 0; i < 4; i++) {
                                    rgResAvail[i] = rgResAvail[i] - rgResCost[i];
                                    if (rgResAvail[i] < 0)
                                        goto FinishProd;
                                }
                                for (i = 0; i < 5; i++) {
                                    GetTrueHullCost(idPlayer, &rgshdef[iLatestTroop].hul, rgCosts);
                                    for (j = 0; j < 4; j++) {
                                        rgResAvail[j] = rgResAvail[j] - (uint32_t)rgCosts[j];
                                        if (rgResAvail[j] < 0)
                                            goto FinishProd;
                                    }
                                    fWrite = 1;
                                    AddItemToQueue(iLatestTroop, 0x1, grobjFleet, 1);
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
            }
        } else {
            vlpbAiPlanet[lppl->id * 16 + 10] = LOBYTE((lppl->fStarbase & 0xff) + 0x1);
            pct = PctPlanetOptValue(lppl, idPlayer);
            if (pct > 0) {
                vlpbAiPlanet[lppl->id * 16 + 3] = LOBYTE(pct);
                cplBadGuy = cplBadGuy + 1;
            }
        }
    }
    for (ipl = 0; ipl < vclpplAi; ipl++) {
        lppl = vrglpplAi[ipl];
        if (vrglpplAi[ipl] == 0x0 || lppl->fStarbase != 0x0)
            break;
    }
    t_merge_4910_0001 = lppl == lpplMac ? 0x0 : lppl;
    lpplHome = t_merge_4910_0001;
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
            if ((lpfl->rgcsh[2] != 0 || lpfl->rgcsh[3] != 0) && lpfl->lpplord->rgord[lpfl->cord - 1].grTask != grTaskNone) {
                if (lpfl->idPlanet == -1) {
                    if (lpfl->cord <= 1)
                        continue;
                    lpfl->lpplord->rgord[1].grTask = grTaskMine;
                    if (lpfl->lpplord->rgord[1].grobj != grobjPlanet) {
                    }
                    idPlanDst = lpfl->lpplord->rgord[1].id;
                } else {
                    if (LpplFromId(lpfl->idPlanet)->iPlayer != -1)
                        goto LBlowAwayOrders;
                    idPlanDst = lpfl->idPlanet;
                }
                vlpbAiPlanet[idPlanDst * 16 + 1] = vlpbAiPlanet[idPlanDst * 16 + 0x1] | 0x80;
                continue;
            }
            if (lpfl->rgcsh[8] == 0 && lpfl->rgcsh[9] == 0) {
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
            if (idPlanDst == -1) {
                if (lpfl->cord <= 1 || lpfl->lpplord->rgord[1].grobj != grobjOther)
                    continue;
            } else {
                lppl = LpplFromId(idPlanDst);
                if (lppl != 0x0 && (lppl->iPlayer == -1 || lppl->iPlayer == idPlayer))
                    continue;
                if (vlpbAiPlanet[idPlanDst * 16 + 3] != 0x0 && lpfl->rgwtMin[3] > 0 && GetRaceStat(&rgplr[lppl->iPlayer], rsMajorAdv) != raMacintosh &&
                    lppl->fStarbase == 0x0) {
                    memset(&ord, 0, sizeof(ORDER));
                    ord.pt = rgptPlan[idPlanDst];
                    ord.grobj = grobjPlanet;
                    ord.id = idPlanDst;
                    ord.grTask = grTaskXfer;
                    ord.fValidTask = 0x1;
                    ord.txp.rgia[3].iAction = iActionUnloadAll;
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if (sel.fl.lpplord->rgord[0].id != idPlanDst || sel.fl.lpplord->rgord[0].grobj != grobjPlanet) {
                        sel.fl.lpplord->rgord[1] = ord;
                    } else {
                        sel.fl.lpplord->rgord[0] = ord;
                    }
                    FLookupFleet(-1, &sel.fl);
                    vlpbAiPlanet[idPlanDst * 16 + 3] = vlpbAiPlanet[idPlanDst * 16 + 0x3] | 0x80;
                    FMoveToNearestStarbase(lpfl, 0);
                    continue;
                }
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
            if (lpfl->rgcsh[2] == 0 && lpfl->rgcsh[3] == 0) {
                if (lpfl->cord > 1)
                    goto L_4eb0;
                if (lpfl->rgcsh[1] == 0) {
                    if (lpfl->rgcsh[8] == 0 && lpfl->rgcsh[9] == 0) {
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
                                                goto L_4eb0;
                                            if (lpfl->pt.x == lpflT->pt.x && lpfl->pt.y == lpflT->pt.y && FIsAiAttack(lpflT) != 0)
                                                break;
                                            lpflT = lpflT->lpflNext;
                                        }
                                    }
                                } else {
                                    if (lppl->fStarbase != 0x0 &&
                                        (lpfl->rgcsh[13] + lpfl->rgcsh[14] < vrgAiArmadaPotency[2] || lpfl->rgcsh[4] + lpfl->rgcsh[5] < vrgAiArmadaPotency[1]))
                                        goto L_4eb0;
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
                                goto L_4eb0;
                            }
                            goto L_4eb0;
                        }
                        if (lpfl->rgcsh[0] == 0 && lpfl->rgcsh[10] == 0 && lpfl->rgcsh[11] == 0) {
                            if (lpfl->rgcsh[12] == 0 || lpfl->cord != 1 || lpfl->lpplord->rgord[0].grTask != grTaskNone)
                                goto L_4eb0;
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
                            sel.fl.lpplord->rgord[0].tlm.cTime = 0x5;
                            sel.fl.lpplord->rgord[0].tlm.cTimeOld = 0x5;
                            FLookupFleet(-1, &sel.fl);
                            goto L_4eb0;
                        }
                        if (lpfl->rgcsh[0] == 0 || (int16_t)rgplr[idPlayer].rgTech[3] < 6 || rgshdef[0].hul.ihuldef != ihuldefScout) {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &lpthWorm);
                            goto L_4eb0;
                        }
                    } else {
                        if (game.turn != 0x0) {
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
                                t_merge_54df_0001 = lppl == 0x0 ? lpplHome : lppl;
                                IdTargetFreighter(lpfl, t_merge_54df_0001);
                                goto L_4eb0;
                            }
                            break;
                        }
                        ChangeMainObjSel(grobjFleet, lpfl->id);
                    }
                } else {
                    ChangeMainObjSel(grobjFleet, lpfl->id);
                    if ((lpfl->idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.rgwtMin[3] >= 50) || lpfl->rgwtMin[3] != 0) {
                        lpthWorm = 0x0;
                        idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
                        if (lpfl->idPlanet == -1 || sel.pl.iPlayer != idPlayer) {
                            lppl = lpplHome;
                        } else {
                            ChangeMainObjSel(grobjFleet, lpfl->id);
                            XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 25);
                            FLookupFleet(lpfl->id, &sel.fl);
                            lppl = LpplFromId(lpfl->idPlanet);
                        }
                        if (idPlanDst == -1) {
                            if (lppl != 0x0) {
                                lpplDest = LpplFindClosestEnum(lppl, FEnumCalcColonistDrop);
                                if (lpplDest != 0x0) {
                                    vlpbAiPlanet[lpplDest->id * 16 + 10] = vlpbAiPlanet[lpplDest->id * 16 + 0xa] | 0x80;
                                    memset(&ord, 0, sizeof(ORDER));
                                    ord.id = lpplDest->id;
                                    ord.grobj = grobjPlanet;
                                    ord.pt = rgptPlan[lpplDest->id];
                                    ord.grTask = grTaskXfer;
                                    ord.fValidTask = 0x1;
                                    ord.iWarp = 0x6;
                                    ord.txp.rgia[3].iAction = iActionUnloadAll;
                                    FMoveAiFleet(lpfl, &ord, 0);
                                    goto L_4eb0;
                                }
                            }
                            if (lpthWorm == 0x0 || Random(100) >= 10)
                                goto L_4eb0;
                            FGotoWormholeAiFleet(lpfl, lpthWorm);
                            goto L_4eb0;
                        }
                        FColonizeAiFleet(lpfl, idPlanDst);
                        vlpbAiPlanet[idPlanDst * 16 + 15] = 0x4;
                        goto L_4eb0;
                    }
                    if ((sel.fl.idPlanet != -1 && sel.pl.iPlayer == idPlayer && sel.pl.fStarbase != 0x0) ||
                        (rgshdef[1].hul.rghs[0].iItem > 0x2 && FMoveToNearestStarbase(lpfl, 0) != 0))
                        goto L_4eb0;
                }
            } else if (game.turn != 0x0) {
                if (lpfl->idPlanet == -1)
                    goto L_4eb0;
                ChangeMainObjSel(grobjFleet, lpfl->id);
                b = vlpbAiPlanet[lpfl->idPlanet * 16 + 1];
                if (b >= 0x4)
                    goto L_4eb0;
                lppl = LpplFindBestEnum(&sel.pl, FEnumCalcMinerDest);
                if (lppl != 0x0) {
                    memset(&ord, 0, sizeof(ORDER));
                    ord.id = lppl->id;
                    ord.grobj = grobjPlanet;
                    ord.pt = rgptPlan[lppl->id];
                    ord.grTask = grTaskMine;
                    ord.fValidTask = 0x1;
                    ord.iWarp = 0x6;
                    FMoveAiFleet(lpfl, &ord, 1);
                    vlpbAiPlanet[lppl->id * 16 + 1] = vlpbAiPlanet[lppl->id * 16 + 0x1] | 0x80;
                    vlpbAiPlanet[lpfl->idPlanet * 16 + 1] = vlpbAiPlanet[lpfl->idPlanet * 16 + 0x1] & 0x80;
                    goto L_4eb0;
                }
                goto L_4eb0;
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

    if (rgshdef[8].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[2] >= 5 && (int16_t)rgplr[idPlayer].rgTech[3] >= 8) {
        FCreateAiShdef(8, 12, &vrgTDAip[vrgTDIshAip[12]]);
    }
    if (rgshdef[9].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7 && (int16_t)rgplr[idPlayer].rgTech[3] >= 11) {
        FCreateAiShdef(9, 13, &vrgTDAip[vrgTDIshAip[15]]);
    }
    if (rgshdef[10].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 5 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 4 && (int16_t)rgplr[idPlayer].rgTech[2] >= 5) {
        for (i = 0; i < 4 && FCreateAiShdef(10, 6, &vrgTDAip[vrgTDIshAip[Random(1) + 2]]) == 0; i++) {
        }
    }
    if (rgshdef[1].fFree != 0x0 || rgshdef[1].cExist == 0x0) {
        if (rgshdef[1].fFree == 0x0 && rgshdef[1].hul.ihuldef != ihuldefPrivateer) {
            shdef = rgshdef[1];
            shdef.fFree = 0x1;
            FChangeAiShdef(&shdef, 1);
        }
        FCreateAiShdef(1, 15, &vrgTDAip[vrgTDIshAip[0]]);
    }
    if (rgshdef[0].fFree != 0x0 || rgshdef[0].cExist == 0x0) {
        if (rgshdef[0].fFree == 0x0) {
            shdef = rgshdef[0];
            shdef.fFree = 0x1;
            FChangeAiShdef(&shdef, 0);
        }
        FCreateAiShdef(0, 5, &vrgTDAip[vrgTDIshAip[1]]);
    }
    if ((rgshdef[2].fFree != 0x0 || rgshdef[2].cExist == 0x0) && (int16_t)rgplr[idPlayer].rgTech[3] >= 7 && (int16_t)rgplr[idPlayer].rgTech[4] >= 4) {
        if (rgshdef[2].fFree == 0x0) {
            shdef = rgshdef[2];
            shdef.fFree = 0x1;
            FChangeAiShdef(&shdef, 2);
        }
        FCreateAiShdef(2, 22, &vrgTDAip[vrgTDIshAip[17]]);
    }
    if ((rgshdef[12].fFree != 0x0 || rgshdef[12].cExist == 0x0) && (int16_t)rgplr[idPlayer].rgTech[3] >= 4 && (int16_t)rgplr[idPlayer].rgTech[5] >= 4) {
        FCreateAiShdef(12, 11, &vrgTDAip[vrgTDIshAip[14]]);
    }
    if (rgshdef[13].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 8 && (int16_t)rgplr[idPlayer].rgTech[4] >= 7 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 6) {
        FCreateAiShdef(13, 18, &vrgTDAip[vrgTDIshAip[13]]);
    }
    if (rgshdef[14].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 11 && (int16_t)rgplr[idPlayer].rgTech[4] >= 12 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 15 && (int16_t)rgplr[idPlayer].rgTech[2] >= 9) {
        FCreateAiShdef(14, 18, &vrgTDAip[vrgTDIshAip[13]]);
    }
    if (rgshdef[4].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 13 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(4, 9, &vrgTDAip[vrgTDIshAip[Random(4) + 8]]) == 0; i++) {
        }
    }
    if (rgshdef[15].fFree != 0x0 && (int16_t)rgplr[idPlayer].rgTech[1] >= 5 && (int16_t)rgplr[idPlayer].rgTech[4] >= 6 &&
        (int16_t)rgplr[idPlayer].rgTech[3] >= 13 && (int16_t)rgplr[idPlayer].rgTech[2] >= 7) {
        for (i = 0; i < 5 && FCreateAiShdef(15, 12, &vrgTDAip[vrgTDIshAip[12]]) == 0; i++) {
        }
    }
    return;
}

int16_t FEnumCalcMinerDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;

    if (lpplSrc != lpplTest) {
        id = lpplTest->id;
        b = vlpbAiPlanet[id * 16 + 1];
        if (b == 0x0 || (Random(100) >= 25 && (b & 0x80) != 0x0)) {
            return 0;
        }
        return b;
    }
    return 0;
}

int16_t FEnumCalcColonistDrop(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t bWant;
    uint8_t bEnemy;

    if (lpplSrc != lpplTest) {
        id = lpplTest->id;
        bEnemy = vlpbAiPlanet[lpplTest->id * 16 + 10];
        bWant = vlpbAiPlanet[lpplTest->id * 16 + 3];
        if (bEnemy != 0x1 || bWant == 0x0 || ((bEnemy & 0x80) != 0x0 && Random(100) >= 25)) {
            return 0;
        }
        if (GetRaceStat(&rgplr[lpplTest->iPlayer], rsMajorAdv) != raMacintosh) {
            return bWant;
        }
        return 0;
    }
    return 0;
}
