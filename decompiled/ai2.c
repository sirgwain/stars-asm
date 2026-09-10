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
    int16_t t_merge_001c_0001;

L_0000:
    if ((game.turn >= 0x14))
        goto L_0019;
    else
        goto L_0013;

L_0013:
    t_merge_001c_0001 = 0;
    goto L_001c;

L_0019:
    t_merge_001c_0001 = 15;

L_001c:
    iroCur = IroEnsureAi(0x0, 0, 0x0, t_merge_001c_0001);
    fMarkedPlanets = 0;
    HandleBasicAiTasks(iroCur, rgprod, -1, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

int16_t FPotentISWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

L_012e:
    cEquiv = 0;
    ish = 11;
    goto L_0162;

L_0144:
    cEquiv = (cEquiv + lpfl->rgcsh[ish]);
    ish = (ish + 1);

L_0162:
    if ((ish <= 12))
        goto L_0144;
    else
        goto L_016b;

L_016b:
    ish = 9;
    goto L_0193;

L_0173:
    cEquiv = (cEquiv + (lpfl->rgcsh[ish] * 2));
    ish = (ish + 1);

L_0193:
    if ((ish <= 10))
        goto L_0173;
    else
        goto L_019c;

L_019c:
    if ((iPotency >= 2))
        goto L_01ae;
    else
        goto L_01a5;

L_01a5:
    return 1;

L_01ae:
    if ((cEquiv < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_01c2;
    else
        goto L_01bc;

L_01bc:
    return 1;

L_01c2:
    return 0;
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
    int16_t  t_merge_021f_0001;
    uint16_t t_merge_032d_0001;
    uint16_t t_merge_036b_0001;
    uint16_t t_merge_072d_0001;
    PLANET  *t_merge_14d6_0001;
    PLANET  *t_merge_160c_0001;

L_01e0:
    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0x0;
    if ((game.turn >= 0xa))
        goto L_021c;
    else
        goto L_0216;

L_0216:
    t_merge_021f_0001 = 0;
    goto L_021f;

L_021c:
    t_merge_021f_0001 = 20;

L_021f:
    iroCur = IroEnsureAi(vrgAiISResOrder, 18, &(ishdefSBLatest), t_merge_021f_0001);
    if ((game.turn <= 0x32))
        goto L_026c;
    else
        goto L_0245;

L_0245:
    MergeAllShdefs(7692);
    MergeAllShdefs(64);
    MergeAllShdefs(16384);
    goto L_0282;

L_026c:
    if ((game.turn <= 0x1e))
        goto L_0282;
    else
        goto L_0276;

L_0276:
    MergeAllShdefs(16384);

L_0282:
    j = 3;
    if ((game.turn <= 0x82))
        goto L_02a4;
    else
        goto L_0293;

L_0293:
    j = (j + ((uint32_t)((game.turn + 0xff88)) / 20));

L_02a4:
    if ((j <= 50))
        goto L_02b4;
    else
        goto L_02ae;

L_02ae:
    j = 50;

L_02b4:
    LOWORD(vrgAiArmadaPotency) = LOBYTE(j);
    vrgAiArmadaPotency[1] = LOBYTE(((uint32_t)((j & 0xff)) / 0x2));
    j = 6;
    if ((game.turn <= 0x73))
        goto L_02ec;
    else
        goto L_02db;

L_02db:
    j = (j + ((uint32_t)((game.turn + 0xff9c)) / 22));

L_02ec:
    if ((j <= 12))
        goto L_02fc;
    else
        goto L_02f6;

L_02f6:
    j = 12;

L_02fc:
    vrgAiArmadaPotency[2] = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 2) + 0xffff)))
        goto L_0320;
    else
        goto L_031a;

L_031a:
    t_merge_032d_0001 = 0x3;
    goto L_032d;

L_0320:
    t_merge_032d_0001 = (((uint32_t)(j) / 2) + 0xffff);

L_032d:
    vrgAiArmadaPotency[3] = LOBYTE(t_merge_032d_0001);
    memset(&(rgRecycleShdef), 0, 0x10);
    if ((game.turn >= 0x78))
        goto L_0357;
    else
        goto L_034e;

L_034e:
    cRecyclePeriod = 0x32;
    goto L_036f;

L_0357:
    if ((game.turn >= 0xc8))
        goto L_0368;
    else
        goto L_0362;

L_0362:
    t_merge_036b_0001 = 0x46;
    goto L_036b;

L_0368:
    t_merge_036b_0001 = 0x64;

L_036b:
    cRecyclePeriod = t_merge_036b_0001;

L_036f:
    CheckAiShdefStatus(11, 12, cRecyclePeriod, &(iLatestCruiser), rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(4, 5, cRecyclePeriod, &(iLatestCargo), rgRecycleShdef);
    CheckAiShdefStatus(2, 3, cRecyclePeriod, &(iLatestBomber), rgRecycleShdef);
    CheckAiShdefStatus(9, 10, cRecyclePeriod, &(iLatestBattle), rgRecycleShdef);
    if ((game.turn <= 0x3c))
        goto L_03fb;
    else
        goto L_03ef;

L_03ef:
    SplitOutShdefs(rgRecycleShdef);

L_03fb:
    EnsureISShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_0477;

L_0435:
    if ((lppl->iPlayer != -1))
        goto L_0473;
    else
        goto L_0442;

L_0442:
    if ((lppl->det < 0x3))
        goto L_0473;
    else
        goto L_0454;

L_0454:
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0))
        goto L_0473;
    else
        goto L_046e;

L_046e:
    cplanCol = (cplanCol + 0x1);

L_0473:
    lppl = (lppl + 1);

L_0477:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0435;
    else
        goto L_0485;

L_0485:
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_0d31;

L_04b4:
    vlpbAiPlanet[((lppl->id * 16) + 9)] = 0x1;
    if ((lppl->iPlayer == idPlayer))
        goto L_056b;
    else
        goto L_04e4;

L_04e4:
    if ((lppl->iPlayer == -1))
        goto L_056b;
    else
        goto L_04f1;

L_04f1:
    vlpbAiPlanet[((lppl->id * 16) + 10)] = LOBYTE(((lppl->fStarbase & 0xff) + 0x1));
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0))
        goto L_0561;
    else
        goto L_0540;

L_0540:
    vlpbAiPlanet[((lppl->id * 16) + 3)] = 0x1;

L_0561:
    cplBadGuy = (cplBadGuy + 1);
    goto L_0d2d;

L_056b:
    if ((lppl->iPlayer != idPlayer))
        goto L_05bf;
    else
        goto L_057a;

L_057a:
    if ((PctPlanetDesirability(lppl, idPlayer) >= 0))
        goto L_05bf;
    else
        goto L_0594;

L_0594:
    cplNegative = (cplNegative + 1);
    vlpbAiPlanet[((lppl->id * 16) + 2)] = 0x1;
    goto L_0d2d;

L_05bf:
    if ((lppl->fStarbase == 0x0))
        goto L_0d2d;
    else
        goto L_05d6;

L_05d6:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_05f6;
    else
        goto L_05e3;

L_05e3:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_0d2d;
    else
        goto L_05e8;

L_05e8:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x5dc))
        goto L_0d2d;
    else
        goto L_05f6;

L_05f6:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0;
    b = 0x0;
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    goto L_0653;

L_063d:
    i = (i + 1);
    lpprod = (lpprod + 1);

L_0653:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_06c3;
    else
        goto L_0666;

L_0666:
    if ((lpprod->grobj != grobjFleet))
        goto L_063d;
    else
        goto L_0687;

L_0687:
    if ((0x0 != 0x0))
        goto L_063d;
    else
        goto L_068f;

L_068f:
    if ((0x0 > 0x0))
        goto L_063d;
    else
        goto L_06b0;

L_06b0:
    if ((0x0 < 0x0))
        goto L_06c3;
    else
        goto L_06b5;

L_06b5:
    if ((lpprod->iItem >= iobjPacketGerm))
        goto L_063d;
    else
        goto L_06c3;

L_06c3:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_06e5;
    else
        goto L_06d6;

L_06d6:
    FinishProduction(0);
    goto L_0d2d;

L_06e5:
    if ((((uint32_t)(rgplr[idPlayer].cPlanet) / 10) <= (vlpbAiData[2] * 0x2)))
        goto L_0723;
    else
        goto L_070b;

L_070b:
    t_merge_072d_0001 = ((uint32_t)(rgplr[idPlayer].cPlanet) / 10);
    goto L_072d;

L_0723:
    t_merge_072d_0001 = (vlpbAiData[2] * 0x2);

L_072d:
    cFr = t_merge_072d_0001;
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 5))
        goto L_07a4;
    else
        goto L_0748;

L_0748:
    if ((iLatestCargo == -1))
        goto L_07a4;
    else
        goto L_0752;

L_0752:
    if ((cExistCargo < cFr))
        goto L_0785;
    else
        goto L_075d;

L_075d:
    if ((cExistCargo >= ((uint32_t)(LOWORD((10 * cFr))) / 0x7)))
        goto L_07a4;
    else
        goto L_0771;

L_0771:
    if ((Random(4) != 0))
        goto L_07a4;
    else
        goto L_0785;

L_0785:
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 1);
    fWrite = 1;

L_07a4:
    if ((cplanCol == 0x0))
        goto L_07ea;
    else
        goto L_07ae;

L_07ae:
    if ((LOWORD(rgshdef[1].cExist) != 0x0))
        goto L_07ea;
    else
        goto L_07b8;

L_07b8:
    if ((HIWORD(rgshdef[1].cExist) != 0x0))
        goto L_07ea;
    else
        goto L_07c2;

L_07c2:
    if ((game.turn <= 0xa))
        goto L_07ea;
    else
        goto L_07cc;

L_07cc:
    AddItemToQueue(0x1, 0x1, grobjFleet, 1);
    fWrite = 1;

L_07ea:
    l = (uint32_t)((lppl->rgwtMin[3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    cRes = CResourcesAtPlanet(lppl, idPlayer);
    if ((rgshdef[6].fFree != 0x0))
        goto L_093c;
    else
        goto L_0839;

L_0839:
    if ((Random(3) != 0))
        goto L_093c;
    else
        goto L_084d;

L_084d:
    id = lppl->id;
    cFr = 0;
    ifl = 0;
    goto L_0868;

L_0864:
    ifl = (ifl + 1);

L_0868:
    if ((ifl >= cFleet))
        goto L_08df;
    else
        goto L_0873;

L_0873:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_08a3;
    else
        goto L_089b;

L_089b:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_08df;
    else
        goto L_08a3;

L_08a3:
    if ((lpfl->idPlanet != id))
        goto L_0864;
    else
        goto L_08b3;

L_08b3:
    if ((lpfl->rgcsh[6] <= 0))
        goto L_0864;
    else
        goto L_08c0;

L_08c0:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0864;
    else
        goto L_08cf;

L_08cf:
    cFr = lpfl->rgcsh[6];

L_08df:
    if ((cFr < 10))
        goto L_0905;
    else
        goto L_08e8;

L_08e8:
    if ((cFr >= 17))
        goto L_093c;
    else
        goto L_08f1;

L_08f1:
    if ((Random(8) != 0))
        goto L_093c;
    else
        goto L_0905;

L_0905:
    if ((Random(((cFr * 2) + 1)) != 0))
        goto L_093c;
    else
        goto L_091e;

L_091e:
    AddItemToQueue(0x6, 0x3, grobjFleet, 1);
    fWrite = 1;

L_093c:
    if ((iLatestBomber == -1))
        goto L_0a25;
    else
        goto L_0946;

L_0946:
    id = lppl->id;
    ifl = 0;
    goto L_095c;

L_0958:
    ifl = (ifl + 1);

L_095c:
    if ((ifl >= cFleet))
        goto L_0a25;
    else
        goto L_0967;

L_0967:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0997;
    else
        goto L_098f;

L_098f:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_0a25;
    else
        goto L_0997;

L_0997:
    if ((lpfl->idPlanet != id))
        goto L_0958;
    else
        goto L_09a7;

L_09a7:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0958;
    else
        goto L_09b6;

L_09b6:
    if ((FPotentISWarFleet(lpfl, 2) == 0))
        goto L_0958;
    else
        goto L_09d0;

L_09d0:
    if ((iLatestBomber == -1))
        goto L_0a25;
    else
        goto L_09da;

L_09da:
    if (((lpfl->rgcsh[2] + lpfl->rgcsh[3]) < (vrgAiArmadaPotency[2] & 0xff)))
        goto L_0a25;
    else
        goto L_0a00;

L_0a00:
    AddItemToQueue(iLatestBomber, 0x4, grobjFleet, 1);
    fWrite = 1;
    goto FinishProd;

L_0a25:
    if ((iLatestCruiser == -1))
        goto L_0ba1;
    else
        goto L_0a2e;

L_0a2e:
    if ((HIWORD(rgshdef[iLatestCruiser].cExist) > HIWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 12) + 0x8)))))
        goto L_0ba1;
    else
        goto L_0a61;

L_0a61:
    if ((HIWORD(rgshdef[iLatestCruiser].cExist) < HIWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 12) + 0x8)))))
        goto L_0a6f;
    else
        goto L_0a66;

L_0a66:
    if ((LOWORD(rgshdef[iLatestCruiser].cExist) >= LOWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 12) + 0x8)))))
        goto L_0ba1;
    else
        goto L_0a6f;

L_0a6f:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0;
    goto L_0ae6;

L_0a9b:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_0ae2;
    else
        goto L_0ad2;

L_0ad2:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0ad7;

L_0ad7:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0ae2;

L_0ae2:
    i = (i + 1);

L_0ae6:
    if ((i < 4))
        goto L_0a9b;
    else
        goto L_0aef;

L_0aef:
    i = 0;
    goto L_0b98;

L_0af7:
    GetTrueHullCost(idPlayer, rgshdef[iLatestCruiser], rgCosts);
    j = 0;
    goto L_0b6c;

L_0b1f:
    rgResAvail[j] = (rgResAvail[j] - (uint32_t)(rgCosts[j]));
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_0b67;
    else
        goto L_0b57;

L_0b57:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_0b5c;

L_0b5c:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_0b67;

L_0b67:
    j = (j + 1);

L_0b6c:
    if ((j < 4))
        goto L_0b1f;
    else
        goto L_0b76;

L_0b76:
    fWrite = 1;
    AddItemToQueue(iLatestCruiser, 0x1, grobjFleet, 1);
    i = (i + 1);

L_0b98:
    if ((i < 5))
        goto L_0af7;
    else
        goto L_0ba1;

L_0ba1:
    if ((iLatestBattle == -1))
        goto FinishProd;
    else
        goto L_0bab;

L_0bab:
    if ((HIWORD(rgshdef[iLatestBattle].cExist) > HIWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 24) + 0x4)))))
        goto FinishProd;
    else
        goto L_0bdf;

L_0bdf:
    if ((HIWORD(rgshdef[iLatestBattle].cExist) < HIWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 24) + 0x4)))))
        goto L_0bed;
    else
        goto L_0be4;

L_0be4:
    if ((LOWORD(rgshdef[iLatestBattle].cExist) >= LOWORD((uint32_t)((((uint32_t)(game.cPlanMax) / 24) + 0x4)))))
        goto FinishProd;
    else
        goto L_0bed;

L_0bed:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0;
    goto L_0c64;

L_0c19:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_0c60;
    else
        goto L_0c50;

L_0c50:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0c55;

L_0c55:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0c60;

L_0c60:
    i = (i + 1);

L_0c64:
    if ((i < 4))
        goto L_0c19;
    else
        goto L_0c6d;

L_0c6d:
    i = 0;
    goto L_0d18;

L_0c75:
    GetTrueHullCost(idPlayer, rgshdef[iLatestBattle], rgCosts);
    j = 0;
    goto L_0ceb;

L_0c9e:
    rgResAvail[j] = (rgResAvail[j] - (uint32_t)(rgCosts[j]));
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_0ce6;
    else
        goto L_0cd6;

L_0cd6:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_0cdb;

L_0cdb:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_0ce6;

L_0ce6:
    j = (j + 1);

L_0ceb:
    if ((j < 4))
        goto L_0c9e;
    else
        goto L_0cf5;

L_0cf5:
    fWrite = 1;
    AddItemToQueue(iLatestBattle, 0x1, grobjFleet, 1);
    i = (i + 1);

L_0d18:
    if ((i < 5))
        goto L_0c75;
    else
        goto FinishProd;

FinishProd:
    FinishProduction(fWrite);

L_0d2d:
    lppl = (lppl + 1);

L_0d31:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_04b4;
    else
        goto L_0d3f;

L_0d3f:
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0;
    goto L_0d61;

L_0d5d:
    ifl = (ifl + 1);

L_0d61:
    if ((ifl >= cFleet))
        goto L_1154;
    else
        goto L_0d6c;

L_0d6c:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0d9c;
    else
        goto L_0d94;

L_0d94:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_1154;
    else
        goto L_0d9c;

L_0d9c:
    if ((lpfl->iPlayer == idPlayer))
        goto L_0dce;
    else
        goto L_0dab;

L_0dab:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_0d5d;

L_0dce:
    if ((FIsAiAttack(lpfl) == 0))
        goto L_0e05;
    else
        goto L_0de4;

L_0de4:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;

L_0e05:
    lpfl->fMark = 0x0;
    if ((FIsAiTransport(lpfl) == 0))
        goto L_1083;
    else
        goto L_0e2f;

L_0e2f:
    idPlanDst = -1;
    if ((lpfl->cord <= 1))
        goto L_0e6d;
    else
        goto L_0e41;

L_0e41:
    if ((lpfl->lpplord->rgord[1].grobj != grobjPlanet))
        goto LCheckForColDrop;
    else
        goto L_0e5c;

L_0e5c:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_0e6a:
    goto LCheckForColDrop;

L_0e6d:
    idPlanDst = lpfl->idPlanet;

LCheckForColDrop:
    if ((idPlanDst == -1))
        goto L_0d5d;
    else
        goto L_0e80;

L_0e80:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_0ea3;
    else
        goto L_0e9a;

L_0e9a:
    if ((HIWORD(lppl) == 0x0))
        goto L_0ebf;
    else
        goto L_0ea3;

L_0ea3:
    if ((lppl->iPlayer == -1))
        goto L_0d5d;
    else
        goto L_0eb0;

L_0eb0:
    if ((lppl->iPlayer == idPlayer))
        goto L_0d5d;
    else
        goto L_0ebf;

L_0ebf:
    if ((vlpbAiPlanet[((idPlanDst * 16) + 3)] == 0x0))
        goto LBlowAwayOrders;
    else
        goto L_0ee7;

L_0ee7:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto LBlowAwayOrders;
    else
        goto L_0ef4;

L_0ef4:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_0f03;
    else
        goto L_0ef9;

L_0ef9:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto LBlowAwayOrders;
    else
        goto L_0f03;

L_0f03:
    if ((GetRaceStat(&(rgplr[lppl->iPlayer]), rsMajorAdv) == raMacintosh))
        goto LBlowAwayOrders;
    else
        goto L_0f27;

L_0f27:
    memset(&(ord), 0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.txp.rgia[3].iAction = iActionUnloadAll;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((sel.fl.lpplord->rgord[0].id != idPlanDst))
        goto L_0fe0;
    else
        goto L_0fb4;

L_0fb4:
    sel.fl.lpplord->rgord[0] = ord;
    goto L_1009;

L_0fe0:
    sel.fl.lpplord->rgord[1] = ord;

L_1009:
    FLookupFleet(-1, sel.fl.id);
    vlpbAiPlanet[((idPlanDst * 16) + 3)] = (vlpbAiPlanet[((idPlanDst * 16) + 0x3)] | 0x80);
    goto L_0d5d;

LBlowAwayOrders:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.cord = 1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(-1, sel.fl.id);
    ClearAiCurrentTask(lpfl, 0);
    goto L_0d5d;

L_1083:
    if ((lpfl->rgcsh[1] == 0))
        goto L_0d5d;
    else
        goto L_1090;

L_1090:
    idPlanDst = -1;
    if ((lpfl->cord <= 1))
        goto L_10ce;
    else
        goto L_10a2;

L_10a2:
    if ((lpfl->lpplord->rgord[1].grobj != grobjPlanet))
        goto L_10d8;
    else
        goto L_10bd;

L_10bd:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_10cb:
    goto L_10d8;

L_10ce:
    idPlanDst = lpfl->idPlanet;

L_10d8:
    if ((idPlanDst == -1))
        goto L_0d5d;
    else
        goto L_10e1;

L_10e1:
    if ((vlpbAiPlanet[((idPlanDst * 16) + 3)] != 0x0))
        goto LCheckForColDrop;
    else
        goto L_110f;

L_110f:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_1132;
    else
        goto L_1129;

L_1129:
    if ((HIWORD(lppl) == 0x0))
        goto L_0d5d;
    else
        goto L_1132;

L_1132:
    if ((lppl->iPlayer == -1))
        goto L_0d5d;
    else
        goto L_113f;

L_113f:
    if ((lppl->iPlayer != idPlayer))
        goto LBlowAwayOrders;
    else
        goto L_114b;

L_114b:
    goto L_0d5d;

L_1154:
    fMarkedPlanets = 0;
    ifl = 0;
    goto L_1166;

L_1162:
    ifl = (ifl + 1);

L_1166:
    if ((ifl >= cFleet))
        goto BestSpeed;
    else
        goto L_1171;

L_1171:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_11a1;
    else
        goto L_1199;

L_1199:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto BestSpeed;
    else
        goto L_11a1;

L_11a1:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1162;
    else
        goto L_11b3;

L_11b3:
    if ((lpfl->cord <= 1))
        goto L_1225;
    else
        goto L_11c0;

L_11c0:
    if ((rgshdef[0].hul.rghs[0].iItem >= 0xa))
        goto L_1162;
    else
        goto L_11ce;

L_11ce:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > 0x0))
        goto L_1162;
    else
        goto L_11db;

L_11db:
    if ((HIWORD(lpfl->rgwtMin[0x4]) < 0x0))
        goto LScrapFleet;
    else
        goto L_11e0;

L_11e0:
    if ((LOWORD(lpfl->rgwtMin[0x4]) >= 0x2))
        goto L_1162;
    else
        goto LScrapFleet;

LScrapFleet:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
    FLookupFleet(-1, sel.fl.id);

L_1222:
    goto L_1162;

L_1225:
    if ((lpfl->rgcsh[6] == 0))
        goto L_12a4;
    else
        goto L_1232;

L_1232:
    if ((lpfl->cord != 1))
        goto L_1162;
    else
        goto L_123f;

L_123f:
    if ((lpfl->lpplord->rgord[0].grTask != grTaskNone))
        goto L_1162;
    else
        goto L_1255;

L_1255:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.lpplord->rgord[0].grTask = grTaskLayMines;
    sel.fl.lpplord->rgord[0].tlm.cTime = 0x5;
    sel.fl.lpplord->rgord[0].tlm.cTimeOld = 0x5;
    FLookupFleet(-1, sel.fl.id);

L_12a1:
    goto L_1162;

L_12a4:
    if ((lpfl->rgcsh[1] == 0))
        goto LTryFreighters;
    else
        goto L_12b4;

L_12b4:
    if ((rgshdef[1].hul.ihuldef != ihuldefMediumFreighter))
        goto LScrapFleet;
    else
        goto L_12c1;

L_12c1:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == -1))
        goto L_1306;
    else
        goto L_12e0;

L_12e0:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_1306;
    else
        goto L_12ec;

L_12ec:
    if ((HIWORD(sel.pl.rgwtMin[0x3]) > 0x0))
        goto L_1374;
    else
        goto L_12f6;

L_12f6:
    if ((HIWORD(sel.pl.rgwtMin[0x3]) < 0x0))
        goto L_1306;
    else
        goto L_12fb;

L_12fb:
    if ((LOWORD(sel.pl.rgwtMin[0x3]) >= 0xc8))
        goto L_1374;
    else
        goto L_1306;

L_1306:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_1374;
    else
        goto L_1313;

L_1313:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_1374;
    else
        goto L_131d;

L_131d:
    if ((sel.fl.idPlanet == -1))
        goto L_1346;
    else
        goto L_1327;

L_1327:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_1346;
    else
        goto L_1333;

L_1333:
    if ((sel.pl.fStarbase != 0x0))
        goto L_1162;
    else
        goto L_1346;

L_1346:
    if ((rgshdef[1].hul.rghs[0].iItem < 0x2))
        goto LScrapFleet;
    else
        goto L_1354;

L_1354:
    if ((FMoveToNearestStarbase(lpfl, 0) == 0))
        goto LScrapFleet;
    else
        goto L_136b;

L_136b:
    goto L_1162;

L_1374:
    lpthWorm = 0x0;
    idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
    if ((lpfl->idPlanet == -1))
        goto L_13f5;
    else
        goto L_13a0;

L_13a0:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_13f5;
    else
        goto L_13ac;

L_13ac:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 150);
    FLookupFleet(lpfl->id, sel.fl.id);

L_13f5:
    if ((idPlanDst == -1))
        goto L_1162;
    else
        goto L_13fe;

L_13fe:
    FColonizeAiFleet(lpfl, idPlanDst);
    vlpbAiPlanet[((idPlanDst * 16) + 15)] = 0x4;

L_142d:
    goto L_1162;

LTryFreighters:
    if ((FIsAiTransport(lpfl) == 0))
        goto LTryBombers;
    else
        goto L_1449;

L_1449:
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_14a5;

L_1478:
    if ((lppl->iPlayer != idPlayer))
        goto L_14a1;
    else
        goto L_1487;

L_1487:
    if ((lppl->fStarbase != 0x0))
        goto L_14b3;
    else
        goto L_14a1;

L_14a1:
    lppl = (lppl + 1);

L_14a5:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1478;
    else
        goto L_14b3;

L_14b3:
    if ((LOWORD(lppl) != LOWORD(lpplMac)))
        goto L_14d0;
    else
        goto L_14c1;

L_14c1:
    if ((HIWORD(lppl) != HIWORD(lpplMac)))
        goto L_14d0;
    else
        goto L_14c9;

L_14c9:
    t_merge_14d6_0001 = 0x0;
    goto L_14d6;

L_14d0:
    t_merge_14d6_0001 = lppl;

L_14d6:
    lpplHome = t_merge_14d6_0001;
    if ((LOWORD(lpplHome) != 0x0))
        goto L_14f1;
    else
        goto L_14e5;

L_14e5:
    if ((HIWORD(lpplHome) == 0x0))
        goto BestSpeed;
    else
        goto L_14f1;

L_14f1:
    lppl = 0x0;
    i = 0;
    goto L_1507;

L_1503:
    i = (i + 1);

L_1507:
    if ((i >= vlpbAiData[2]))
        goto L_15b3;
    else
        goto L_1517;

L_1517:
    j = 0;
    goto L_1525;

L_1520:
    j = (j + 1);

L_1525:
    if ((j >= vlpbAiData[((i * 20) + 6)]))
        goto L_1589;
    else
        goto L_1549;

L_1549:
    if ((vlpbAiData[(((i * 20) + (j * 2)) + 8)] != lpfl->id))
        goto L_1520;
    else
        goto L_1589;

L_1589:
    if ((j >= vlpbAiData[((i * 20) + 6)]))
        goto L_1503;
    else
        goto L_15b3;

L_15b3:
    if ((i >= vlpbAiData[2]))
        goto L_15eb;
    else
        goto L_15c3;

L_15c3:
    lppl = LpplFromId(vlpbAiData[((i * 20) + 4)]);

L_15eb:
    if ((LOWORD(lppl) != 0x0))
        goto L_15fd;
    else
        goto L_15f4;

L_15f4:
    if ((HIWORD(lppl) == 0x0))
        goto L_1606;
    else
        goto L_15fd;

L_15fd:
    t_merge_160c_0001 = lppl;
    goto L_160c;

L_1606:
    t_merge_160c_0001 = lpplHome;

L_160c:
    IdTargetFreighter(lpfl, t_merge_160c_0001);
    goto L_1162;

LTryBombers:
    if ((lpfl->rgcsh[2] != 0))
        goto L_163c;
    else
        goto L_162c;

L_162c:
    if ((lpfl->rgcsh[3] == 0))
        goto LTryScouts;
    else
        goto L_163c;

L_163c:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == -1))
        goto L_1769;
    else
        goto L_165b;

L_165b:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_16e2;
    else
        goto L_167f;

L_167f:
    if ((lppl->fStarbase == 0x0))
        goto L_16cd;
    else
        goto L_1696;

L_1696:
    if ((lpfl->rgcsh[2] >= 2))
        goto L_16b0;
    else
        goto L_16a3;

L_16a3:
    if ((lpfl->rgcsh[3] < 2))
        goto L_1162;
    else
        goto L_16b0;

L_16b0:
    if ((lpfl->rgcsh[9] >= 3))
        goto L_16cd;
    else
        goto L_16bd;

L_16bd:
    if ((lpfl->rgcsh[10] < 3))
        goto L_1162;
    else
        goto L_16cd;

L_16cd:
    FLookupFleet(lpfl->id, sel.fl.id);
    goto LTargetBomber;

L_16e2:
    if ((lppl->iPlayer == -1))
        goto LTargetBomber;
    else
        goto L_16ef;

L_16ef:
    lpflT = lpflEnemy;
    goto L_170f;

L_16fe:
    lpflT = lpflT->lpflNext;

L_170f:
    if ((LOWORD(lpflT) != 0x0))
        goto L_1721;
    else
        goto L_1718;

L_1718:
    if ((HIWORD(lpflT) == 0x0))
        goto L_1162;
    else
        goto L_1721;

L_1721:
    if ((lpfl->pt.x != lpflT->pt.x))
        goto L_16fe;
    else
        goto L_1734;

L_1734:
    if ((lpfl->pt.y != lpflT->pt.y))
        goto L_16fe;
    else
        goto L_1747;

L_1747:
    if ((FIsAiAttack(lpflT) != 0))
        goto LTargetBomber;
    else
        goto L_175a;

L_175a:
    goto L_16fe;

L_1769:
    lppl = lpplHome;

LTargetBomber:
    if ((game.fAisBand == 0x0))
        goto L_17aa;
    else
        goto L_178b;

L_178b:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_17b4;

L_17aa:
    lpplDest = 0x0;

L_17b4:
    if ((LOWORD(lpplDest) != 0x0))
        goto L_17e2;
    else
        goto L_17bd;

L_17bd:
    if ((HIWORD(lpplDest) != 0x0))
        goto L_17e2;
    else
        goto L_17c6;

L_17c6:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

L_17e2:
    lppl = lpplDest;
    if ((LOWORD(lppl) != 0x0))
        goto L_1800;
    else
        goto L_17f7;

L_17f7:
    if ((HIWORD(lppl) == 0x0))
        goto L_1162;
    else
        goto L_1800;

L_1800:
    vlpbAiPlanet[((lppl->id * 16) + 10)] = (vlpbAiPlanet[((lppl->id * 16) + 0xa)] | 0x80);
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0);

L_1888:
    goto L_1162;

LTryScouts:
    if ((lpfl->rgcsh[0] == 0))
        goto L_1162;
    else
        goto L_189b;

L_189b:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((rgshdef[0].hul.rghs[0].iItem >= 0xa))
        goto L_18da;
    else
        goto L_18bb;

L_18bb:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > 0x0))
        goto L_18da;
    else
        goto L_18c8;

L_18c8:
    if ((HIWORD(lpfl->rgwtMin[0x4]) < 0x0))
        goto LScrapFleet;
    else
        goto L_18cd;

L_18cd:
    if ((LOWORD(lpfl->rgwtMin[0x4]) < 0x2))
        goto LScrapFleet;
    else
        goto L_18da;

L_18da:
    IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &(lpthWorm));
    goto L_1162;

BestSpeed:
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureISShdefs(int16_t iroCur) {
    SHDEF   shdef;
    int16_t i;

L_1938:
    if ((rgshdef[4].fFree == 0x0))
        goto L_198c;
    else
        goto L_1954;

L_1954:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 5))
        goto L_198c;
    else
        goto L_196c;

L_196c:
    FCreateAiShdef(4, 1, &(vrgISAip[(vrgISIshAip[14] & 0xff)]));

L_198c:
    if ((rgshdef[5].fFree == 0x0))
        goto L_19d7;
    else
        goto L_199f;

L_199f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 7))
        goto L_19d7;
    else
        goto L_19b7;

L_19b7:
    FCreateAiShdef(5, 3, &(vrgISAip[(vrgISIshAip[18] & 0xff)]));

L_19d7:
    if ((rgshdef[14].fFree == 0x0))
        goto L_1a9f;
    else
        goto L_19ea;

L_19ea:
    if (((uint16_t)(rgplr[idPlayer].rgTech[1]) < 5))
        goto L_1a9f;
    else
        goto L_1a02;

L_1a02:
    if (((uint16_t)(rgplr[idPlayer].rgTech[4]) < 6))
        goto L_1a9f;
    else
        goto L_1a1a;

L_1a1a:
    if (((uint16_t)(rgplr[idPlayer].rgTech[3]) < 4))
        goto L_1a9f;
    else
        goto L_1a32;

L_1a32:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 5))
        goto L_1a9f;
    else
        goto L_1a4a;

L_1a4a:
    i = 0;
    goto L_1a95;

L_1a53:
    if ((FCreateAiShdef(14, 6, &(vrgISAip[vrgISIshAip[(Random(1) + 4)]])) != 0))
        goto L_1a9f;
    else
        goto L_1a90;

L_1a90:
    i = (i + 1);

L_1a95:
    if ((i < 4))
        goto L_1a53;
    else
        goto L_1a9f;

L_1a9f:
    if ((rgshdef[1].fFree != 0x0))
        goto L_1ac6;
    else
        goto L_1ab2;

L_1ab2:
    if ((LOWORD(rgshdef[1].cExist) != 0x0))
        goto L_1b29;
    else
        goto L_1abc;

L_1abc:
    if ((HIWORD(rgshdef[1].cExist) != 0x0))
        goto L_1b29;
    else
        goto L_1ac6;

L_1ac6:
    if ((rgshdef[1].fFree != 0x0))
        goto L_1b09;
    else
        goto L_1ad9;

L_1ad9:
    shdef = rgshdef[1];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 1);

L_1b09:
    FCreateAiShdef(1, 1, &(vrgISAip[(LOWORD(vrgISIshAip) & 0xff)]));

L_1b29:
    if ((rgshdef[0].fFree != 0x0))
        goto L_1b50;
    else
        goto L_1b3c;

L_1b3c:
    if ((LOWORD(rgshdef[0].cExist) != 0x0))
        goto L_1bb3;
    else
        goto L_1b46;

L_1b46:
    if ((HIWORD(rgshdef[0].cExist) != 0x0))
        goto L_1bb3;
    else
        goto L_1b50;

L_1b50:
    if ((rgshdef[0].fFree != 0x0))
        goto L_1b93;
    else
        goto L_1b63;

L_1b63:
    shdef = rgshdef[0];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0);

L_1b93:
    FCreateAiShdef(0, 4, &(vrgISAip[(vrgISIshAip[1] & 0xff)]));

L_1bb3:
    if ((rgshdef[6].fFree == 0x0))
        goto L_1c2e;
    else
        goto L_1bc6;

L_1bc6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[3]) < 4))
        goto L_1c2e;
    else
        goto L_1bde;

L_1bde:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 5))
        goto L_1c2e;
    else
        goto L_1bf6;

L_1bf6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[5]) < 6))
        goto L_1c2e;
    else
        goto L_1c0e;

L_1c0e:
    FCreateAiShdef(6, 11, &(vrgISAip[(vrgISIshAip[17] & 0xff)]));

L_1c2e:
    if ((rgshdef[2].fFree == 0x0))
        goto L_1cc1;
    else
        goto L_1c41;

L_1c41:
    if (((uint16_t)(rgplr[idPlayer].rgTech[1]) < 8))
        goto L_1cc1;
    else
        goto L_1c59;

L_1c59:
    if (((uint16_t)(rgplr[idPlayer].rgTech[4]) < 7))
        goto L_1cc1;
    else
        goto L_1c71;

L_1c71:
    if (((uint16_t)(rgplr[idPlayer].rgTech[3]) < 6))
        goto L_1cc1;
    else
        goto L_1c89;

L_1c89:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 7))
        goto L_1cc1;
    else
        goto L_1ca1;

L_1ca1:
    FCreateAiShdef(2, 17, &(vrgISAip[(vrgISIshAip[15] & 0xff)]));

L_1cc1:
    if ((rgshdef[3].fFree == 0x0))
        goto L_1d54;
    else
        goto L_1cd4;

L_1cd4:
    if (((uint16_t)(rgplr[idPlayer].rgTech[1]) < 11))
        goto L_1d54;
    else
        goto L_1cec;

L_1cec:
    if (((uint16_t)(rgplr[idPlayer].rgTech[4]) < 12))
        goto L_1d54;
    else
        goto L_1d04;

L_1d04:
    if (((uint16_t)(rgplr[idPlayer].rgTech[3]) < 15))
        goto L_1d54;
    else
        goto L_1d1c;

L_1d1c:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 9))
        goto L_1d54;
    else
        goto L_1d34;

L_1d34:
    FCreateAiShdef(3, 19, &(vrgISAip[(vrgISIshAip[16] & 0xff)]));

L_1d54:
    if ((rgshdef[9].fFree == 0x0))
        goto L_1e1c;
    else
        goto L_1d67;

L_1d67:
    if (((uint16_t)(rgplr[idPlayer].rgTech[1]) < 5))
        goto L_1e1c;
    else
        goto L_1d7f;

L_1d7f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[4]) < 6))
        goto L_1e1c;
    else
        goto L_1d97;

L_1d97:
    if (((uint16_t)(rgplr[idPlayer].rgTech[3]) < 13))
        goto L_1e1c;
    else
        goto L_1daf;

L_1daf:
    if (((uint16_t)(rgplr[idPlayer].rgTech[2]) < 7))
        goto L_1e1c;
    else
        goto L_1dc7;

L_1dc7:
    i = 0;
    goto L_1e12;

L_1dd0:
    if ((FCreateAiShdef(9, 9, &(vrgISAip[vrgISIshAip[(Random(4) + 10)]])) != 0))
        goto L_1e1c;
    else
        goto L_1e0d;

L_1e0d:
    i = (i + 1);

L_1e12:
    if ((i < 5))
        goto L_1dd0;
    else
        goto L_1e1c;

L_1e1c:
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
    int16_t  t_merge_1e69_0001;
    THING  **t_merge_2a5b_0001;
    PLANET  *t_merge_2bd5_0001;
    PLANET  *t_merge_2d04_0001;

L_1e22:
    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0;
    cplNegative = 0;
    cplanCol = 0x0;
    fColonyShipInQueue = 0;
    fBomberInQueue = 0;
    if ((game.turn >= 0x14))
        goto L_1e66;
    else
        goto L_1e60;

L_1e60:
    t_merge_1e69_0001 = 0;
    goto L_1e69;

L_1e66:
    t_merge_1e69_0001 = 15;

L_1e69:
    iroCur = IroEnsureAi(0x0, 0, &(ishdefSBLatest), t_merge_1e69_0001);
    EnsureCAShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_1efc;

L_1ebb:
    if ((lppl->iPlayer != -1))
        goto L_1ef8;
    else
        goto L_1ec8;

L_1ec8:
    if ((lppl->det < 0x3))
        goto L_1ef8;
    else
        goto L_1eda;

L_1eda:
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0))
        goto L_1ef8;
    else
        goto L_1ef4;

L_1ef4:
    cplanCol = (cplanCol + 0x1);

L_1ef8:
    lppl = (lppl + 1);

L_1efc:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1ebb;
    else
        goto L_1f0a;

L_1f0a:
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_22cf;

L_1f35:
    vlpbAiPlanet[((lppl->id * 16) + 9)] = 0x1;
    if ((lppl->iPlayer != -1))
        goto L_2017;
    else
        goto L_1f63;

L_1f63:
    if ((lppl->det < 0x3))
        goto L_2017;
    else
        goto L_1f75;

L_1f75:
    b = 0x0;
    i = 0;
    goto L_1fd6;

L_1f81:
    if ((lppl->rgMinConc[i] <= 0x42))
        goto L_1fa8;
    else
        goto L_1fa1;

L_1fa1:
    bT = 0x4b;
    goto L_1fc9;

L_1fa8:
    bT = LOBYTE(((uint32_t)(lppl->rgMinConc[i]) / 0x2));

L_1fc9:
    b = (b + LOBYTE(bT));
    i = (i + 1);

L_1fd6:
    if ((i < 3))
        goto L_1f81;
    else
        goto L_1fdf;

L_1fdf:
    if (((b & 0x80) == 0x0))
        goto L_1ff4;
    else
        goto L_1ff0;

L_1ff0:
    b = 0x7f;

L_1ff4:
    vlpbAiPlanet[((lppl->id * 16) + 1)] = b;

L_2017:
    if ((lppl->iPlayer == idPlayer))
        goto L_20ad;
    else
        goto L_2026;

L_2026:
    if ((lppl->iPlayer == -1))
        goto L_20ad;
    else
        goto L_2033;

L_2033:
    vlpbAiPlanet[((lppl->id * 16) + 10)] = LOBYTE(((lppl->fStarbase & 0xff) + 0x1));
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0))
        goto L_20a3;
    else
        goto L_2082;

L_2082:
    vlpbAiPlanet[((lppl->id * 16) + 3)] = 0x1;

L_20a3:
    cplBadGuy = (cplBadGuy + 1);
    goto L_22cb;

L_20ad:
    if ((lppl->iPlayer != idPlayer))
        goto L_2101;
    else
        goto L_20bc;

L_20bc:
    if ((PctPlanetDesirability(lppl, idPlayer) >= 0))
        goto L_2101;
    else
        goto L_20d6;

L_20d6:
    cplNegative = (cplNegative + 1);
    vlpbAiPlanet[((lppl->id * 16) + 2)] = 0x1;
    goto L_22cb;

L_2101:
    if ((lppl->fStarbase == 0x0))
        goto L_22cb;
    else
        goto L_2118;

L_2118:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_2138;
    else
        goto L_2125;

L_2125:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_22cb;
    else
        goto L_212a;

L_212a:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0x3e8))
        goto L_22cb;
    else
        goto L_2138;

L_2138:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0;
    b = 0x0;
    i = 0;
    lpprod = lpplProdGlob->rgprod;
    goto L_218e;

L_217b:
    i = (i + 1);
    lpprod = (lpprod + 1);

L_218e:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_21fc;
    else
        goto L_21a1;

L_21a1:
    if ((lpprod->grobj != grobjFleet))
        goto L_217b;
    else
        goto L_21c1;

L_21c1:
    if ((0x0 != 0x0))
        goto L_217b;
    else
        goto L_21c9;

L_21c9:
    if ((0x0 < 0x0))
        goto L_217b;
    else
        goto L_21e9;

L_21e9:
    if ((0x0 > 0x0))
        goto L_21fc;
    else
        goto L_21ee;

L_21ee:
    if ((lpprod->iItem <= iobjPacketGerm))
        goto L_217b;
    else
        goto L_21fc;

L_21fc:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_221e;
    else
        goto L_220f;

L_220f:
    FinishProduction(0);
    goto L_22cb;

L_221e:
    if ((game.turn != 0x0))
        goto L_225b;
    else
        goto L_2228;

L_2228:
    AddItemToQueue(0x0, 0x1, grobjFleet, 1);
    AddItemToQueue(0x0, 0x1, grobjFleet, 1);
    goto L_22c0;

L_225b:
    if ((fColonyShipInQueue != 0))
        goto L_22c0;
    else
        goto L_2264;

L_2264:
    if ((LOWORD(rgshdef[1].cExist) != 0x0))
        goto L_2278;
    else
        goto L_226e;

L_226e:
    if ((HIWORD(rgshdef[1].cExist) == 0x0))
        goto L_229e;
    else
        goto L_2278;

L_2278:
    if (((HIWORD(rgshdef[1].cExist) + 0x0) > 0x0))
        goto L_22c0;
    else
        goto L_2292;

L_2292:
    if (((HIWORD(rgshdef[1].cExist) + 0x0) < 0x0))
        goto L_229e;
    else
        goto L_2297;

L_2297:
    if (((LOWORD(rgshdef[1].cExist) + 0x1) >= cplanCol))
        goto L_22c0;
    else
        goto L_229e;

L_229e:
    fColonyShipInQueue = 1;
    AddItemToQueue(0x1, 0x1, grobjFleet, 1);
    fWrite = 1;

L_22c0:
    FinishProduction(fWrite);

L_22cb:
    lppl = (lppl + 1);

L_22cf:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1f35;
    else
        goto L_22dd;

L_22dd:
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0;
    goto L_22fd;

L_22f9:
    ifl = (ifl + 1);

L_22fd:
    if ((ifl >= cFleet))
        goto L_278d;
    else
        goto L_2308;

L_2308:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2338;
    else
        goto L_2330;

L_2330:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto L_278d;
    else
        goto L_2338;

L_2338:
    if ((lpfl->iPlayer == idPlayer))
        goto L_236a;
    else
        goto L_2347;

L_2347:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_22f9;

L_236a:
    if ((FIsTurinDroneAiAttack(lpfl) == 0))
        goto L_239d;
    else
        goto L_2380;

L_2380:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;

L_239d:
    lpfl->fMark = 0x0;
    if ((lpfl->rgcsh[7] != 0))
        goto L_23cb;
    else
        goto L_23be;

L_23be:
    if ((lpfl->rgcsh[8] == 0))
        goto L_2497;
    else
        goto L_23cb;

L_23cb:
    if ((lpfl->cord < 1))
        goto L_2497;
    else
        goto L_23d8;

L_23d8:
    if ((lpfl->idPlanet == -1))
        goto L_2455;
    else
        goto L_23e5;

L_23e5:
    if ((LpplFromId(lpfl->idPlanet)->iPlayer == -1))
        goto L_2448;
    else
        goto LBlowAwayOrders;

LBlowAwayOrders:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.cord = 1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(-1, sel.fl.id);
    ClearAiCurrentTask(lpfl, 0);
    goto L_22f9;

L_2448:
    idPlanDst = lpfl->idPlanet;
    goto L_2476;

L_2455:
    if ((lpfl->cord <= 1))
        goto L_22f9;
    else
        goto L_2462;

L_2462:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_2476:
    vlpbAiPlanet[((idPlanDst * 16) + 1)] = (vlpbAiPlanet[((idPlanDst * 16) + 0x1)] | 0x80);
    goto L_22f9;

L_2497:
    if ((FIsAiTransport(lpfl) == 0))
        goto L_26bc;
    else
        goto L_24ad;

L_24ad:
    idPlanDst = -1;
    if ((lpfl->cord <= 1))
        goto L_24eb;
    else
        goto L_24bf;

L_24bf:
    if ((lpfl->lpplord->rgord[1].grobj != grobjPlanet))
        goto LCheckForColDrop;
    else
        goto L_24da;

L_24da:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_24e8:
    goto LCheckForColDrop;

L_24eb:
    idPlanDst = lpfl->idPlanet;

LCheckForColDrop:
    if ((idPlanDst == -1))
        goto L_22f9;
    else
        goto L_24fe;

L_24fe:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_2521;
    else
        goto L_2518;

L_2518:
    if ((HIWORD(lppl) == 0x0))
        goto L_253d;
    else
        goto L_2521;

L_2521:
    if ((lppl->iPlayer == -1))
        goto L_22f9;
    else
        goto L_252e;

L_252e:
    if ((lppl->iPlayer == idPlayer))
        goto L_22f9;
    else
        goto L_253d;

L_253d:
    if ((vlpbAiPlanet[((idPlanDst * 16) + 3)] == 0x0))
        goto LBlowAwayOrders;
    else
        goto L_2565;

L_2565:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto LBlowAwayOrders;
    else
        goto L_2572;

L_2572:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_2581;
    else
        goto L_2577;

L_2577:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto LBlowAwayOrders;
    else
        goto L_2581;

L_2581:
    if ((GetRaceStat(&(rgplr[lppl->iPlayer]), rsMajorAdv) == raMacintosh))
        goto LBlowAwayOrders;
    else
        goto L_25a5;

L_25a5:
    memset(&(ord), 0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.txp.rgia[3].iAction = iActionUnloadAll;
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((sel.fl.lpplord->rgord[0].id != idPlanDst))
        goto L_265c;
    else
        goto L_2630;

L_2630:
    sel.fl.lpplord->rgord[0] = ord;
    goto L_2685;

L_265c:
    sel.fl.lpplord->rgord[1] = ord;

L_2685:
    FLookupFleet(-1, sel.fl.id);
    vlpbAiPlanet[((idPlanDst * 16) + 3)] = (vlpbAiPlanet[((idPlanDst * 16) + 0x3)] | 0x80);
    goto L_22f9;

L_26bc:
    if ((lpfl->rgcsh[1] == 0))
        goto L_22f9;
    else
        goto L_26c9;

L_26c9:
    idPlanDst = -1;
    if ((lpfl->cord <= 1))
        goto L_2707;
    else
        goto L_26db;

L_26db:
    if ((lpfl->lpplord->rgord[1].grobj != grobjPlanet))
        goto L_2711;
    else
        goto L_26f6;

L_26f6:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_2704:
    goto L_2711;

L_2707:
    idPlanDst = lpfl->idPlanet;

L_2711:
    if ((idPlanDst == -1))
        goto L_22f9;
    else
        goto L_271a;

L_271a:
    if ((vlpbAiPlanet[((idPlanDst * 16) + 3)] != 0x0))
        goto LCheckForColDrop;
    else
        goto L_2748;

L_2748:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_276b;
    else
        goto L_2762;

L_2762:
    if ((HIWORD(lppl) == 0x0))
        goto L_22f9;
    else
        goto L_276b;

L_276b:
    if ((lppl->iPlayer == -1))
        goto L_22f9;
    else
        goto L_2778;

L_2778:
    if ((lppl->iPlayer != idPlayer))
        goto LBlowAwayOrders;
    else
        goto L_2784;

L_2784:
    goto L_22f9;

L_278d:
    fMarkedPlanets = 0;
    ifl = 0;
    goto L_279f;

L_279b:
    ifl = (ifl + 1);

L_279f:
    if ((ifl >= cFleet))
        goto BestSpeed;
    else
        goto L_27aa;

L_27aa:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_27da;
    else
        goto L_27d2;

L_27d2:
    if ((HIWORD(rglpfl[ifl]) == 0x0))
        goto BestSpeed;
    else
        goto L_27da;

L_27da:
    if ((lpfl->iPlayer != idPlayer))
        goto L_279b;
    else
        goto L_27ec;

L_27ec:
    if ((lpfl->rgcsh[7] != 0))
        goto L_2806;
    else
        goto L_27f9;

L_27f9:
    if ((lpfl->rgcsh[8] == 0))
        goto L_2933;
    else
        goto L_2806;

L_2806:
    if ((lpfl->idPlanet == -1))
        goto L_279b;
    else
        goto L_2813;

L_2813:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    b = vlpbAiPlanet[((lpfl->idPlanet * 16) + 1)];
    if ((b >= 0x4))
        goto L_279b;
    else
        goto L_2857;

L_2857:
    lppl = LpplFindBestEnum(&(sel.pl), FEnumCalcMinerDest);
    if ((LOWORD(lppl) != 0x0))
        goto L_2886;
    else
        goto L_287d;

L_287d:
    if ((HIWORD(lppl) == 0x0))
        goto L_279b;
    else
        goto L_2886;

L_2886:
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskMine;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    FMoveAiFleet(lpfl, &(ord), 1);
    vlpbAiPlanet[((lppl->id * 16) + 1)] = (vlpbAiPlanet[((lppl->id * 16) + 0x1)] | 0x80);
    vlpbAiPlanet[((lpfl->idPlanet * 16) + 1)] = (vlpbAiPlanet[((lpfl->idPlanet * 16) + 0x1)] & 0x80);

L_2930:
    goto L_279b;

L_2933:
    if ((lpfl->cord > 1))
        goto L_279b;
    else
        goto L_2943;

L_2943:
    if ((lpfl->rgcsh[1] == 0))
        goto LTryFreighters;
    else
        goto L_2953;

L_2953:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == -1))
        goto L_2997;
    else
        goto L_2972;

L_2972:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_2997;
    else
        goto L_297e;

L_297e:
    if ((HIWORD(sel.pl.rgwtMin[0x3]) > 0x0))
        goto L_2a3a;
    else
        goto L_2988;

L_2988:
    if ((HIWORD(sel.pl.rgwtMin[0x3]) < 0x0))
        goto L_2997;
    else
        goto L_298d;

L_298d:
    if ((LOWORD(sel.pl.rgwtMin[0x3]) >= 0x32))
        goto L_2a3a;
    else
        goto L_2997;

L_2997:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_2a3a;
    else
        goto L_29a4;

L_29a4:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_2a3a;
    else
        goto L_29ae;

L_29ae:
    if ((sel.fl.idPlanet == -1))
        goto L_29d7;
    else
        goto L_29b8;

L_29b8:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_29d7;
    else
        goto L_29c4;

L_29c4:
    if ((sel.pl.fStarbase != 0x0))
        goto L_279b;
    else
        goto L_29d7;

L_29d7:
    if ((rgshdef[1].hul.rghs[0].iItem < 0x2))
        goto LScrapFleet;
    else
        goto L_29e5;

L_29e5:
    if ((FMoveToNearestStarbase(lpfl, 0) != 0))
        goto L_279b;
    else
        goto LScrapFleet;

LScrapFleet:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.lpplord->rgord[0].grTask = grTaskScrap;
    FLookupFleet(-1, sel.fl.id);

L_2a37:
    goto L_279b;

L_2a3a:
    lpthWorm = 0x0;
    if ((rgshdef[1].hul.rghs[0].iItem <= 0x1))
        goto L_2a58;
    else
        goto L_2a52;

L_2a52:
    t_merge_2a5b_0001 = &(lpthWorm);
    goto L_2a5b;

L_2a58:
    t_merge_2a5b_0001 = 0x0;

L_2a5b:
    idPlanDst = IdNearestColonizablePlanet(lpfl, t_merge_2a5b_0001);
    if ((lpfl->idPlanet == -1))
        goto L_2acf;
    else
        goto L_2a7a;

L_2a7a:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_2acf;
    else
        goto L_2a86;

L_2a86:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 3, 25);
    FLookupFleet(lpfl->id, sel.fl.id);

L_2acf:
    if ((idPlanDst == -1))
        goto L_2b0a;
    else
        goto L_2ad8;

L_2ad8:
    FColonizeAiFleet(lpfl, idPlanDst);
    vlpbAiPlanet[((idPlanDst * 16) + 15)] = 0x4;
    goto L_279b;

L_2b0a:
    if ((LOWORD(lpthWorm) != 0x0))
        goto L_2b1c;
    else
        goto L_2b13;

L_2b13:
    if ((HIWORD(lpthWorm) == 0x0))
        goto L_279b;
    else
        goto L_2b1c;

L_2b1c:
    FGotoWormholeAiFleet(lpfl, lpthWorm);

L_2b30:
    goto L_279b;

LTryFreighters:
    if ((FIsAiTransport(lpfl) == 0))
        goto LTryBombers;
    else
        goto L_2b4c;

L_2b4c:
    lppl = lpPlanets;
    lpplMac = (lpPlanets + cPlanet);
    goto L_2ba4;

L_2b77:
    if ((lppl->iPlayer != idPlayer))
        goto L_2ba0;
    else
        goto L_2b86;

L_2b86:
    if ((lppl->fStarbase != 0x0))
        goto L_2bb2;
    else
        goto L_2ba0;

L_2ba0:
    lppl = (lppl + 1);

L_2ba4:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_2b77;
    else
        goto L_2bb2;

L_2bb2:
    if ((LOWORD(lppl) != LOWORD(lpplMac)))
        goto L_2bcf;
    else
        goto L_2bc0;

L_2bc0:
    if ((HIWORD(lppl) != HIWORD(lpplMac)))
        goto L_2bcf;
    else
        goto L_2bc8;

L_2bc8:
    t_merge_2bd5_0001 = 0x0;
    goto L_2bd5;

L_2bcf:
    t_merge_2bd5_0001 = lppl;

L_2bd5:
    lpplHome = t_merge_2bd5_0001;
    if ((LOWORD(lpplHome) != 0x0))
        goto L_2bf0;
    else
        goto L_2be4;

L_2be4:
    if ((HIWORD(lpplHome) == 0x0))
        goto BestSpeed;
    else
        goto L_2bf0;

L_2bf0:
    lppl = 0x0;
    i = 0;
    goto L_2c06;

L_2c02:
    i = (i + 1);

L_2c06:
    if ((i >= vlpbAiData[2]))
        goto L_2cab;
    else
        goto L_2c16;

L_2c16:
    j = 0;
    goto L_2c22;

L_2c1e:
    j = (j + 1);

L_2c22:
    if ((j >= vlpbAiData[((i * 20) + 6)]))
        goto L_2c82;
    else
        goto L_2c45;

L_2c45:
    if ((vlpbAiData[(((i * 20) + (j * 2)) + 8)] != lpfl->id))
        goto L_2c1e;
    else
        goto L_2c82;

L_2c82:
    if ((j >= vlpbAiData[((i * 20) + 6)]))
        goto L_2c02;
    else
        goto L_2cab;

L_2cab:
    if ((i >= vlpbAiData[2]))
        goto L_2ce3;
    else
        goto L_2cbb;

L_2cbb:
    lppl = LpplFromId(vlpbAiData[((i * 20) + 4)]);

L_2ce3:
    if ((LOWORD(lppl) != 0x0))
        goto L_2cf5;
    else
        goto L_2cec;

L_2cec:
    if ((HIWORD(lppl) == 0x0))
        goto L_2cfe;
    else
        goto L_2cf5;

L_2cf5:
    t_merge_2d04_0001 = lppl;
    goto L_2d04;

L_2cfe:
    t_merge_2d04_0001 = lpplHome;

L_2d04:
    IdTargetFreighter(lpfl, t_merge_2d04_0001);
    goto L_279b;

LTryBombers:
    if ((lpfl->rgcsh[13] != 0))
        goto L_2d34;
    else
        goto L_2d24;

L_2d24:
    if ((lpfl->rgcsh[14] == 0))
        goto LTryScouts;
    else
        goto L_2d34;

L_2d34:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == -1))
        goto L_2e47;
    else
        goto L_2d53;

L_2d53:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_2dc0;
    else
        goto L_2d77;

L_2d77:
    if ((lppl->fStarbase == 0x0))
        goto L_2dab;
    else
        goto L_2d8e;

L_2d8e:
    if ((lpfl->rgcsh[13] >= 2))
        goto L_2dab;
    else
        goto L_2d9b;

L_2d9b:
    if ((lpfl->rgcsh[14] < 2))
        goto L_279b;
    else
        goto L_2dab;

L_2dab:
    FLookupFleet(lpfl->id, sel.fl.id);
    goto LTargetBomber;

L_2dc0:
    if ((lppl->iPlayer == -1))
        goto LTargetBomber;
    else
        goto L_2dcd;

L_2dcd:
    lpflT = lpflEnemy;
    goto L_2ded;

L_2ddc:
    lpflT = lpflT->lpflNext;

L_2ded:
    if ((LOWORD(lpflT) != 0x0))
        goto L_2dff;
    else
        goto L_2df6;

L_2df6:
    if ((HIWORD(lpflT) == 0x0))
        goto L_279b;
    else
        goto L_2dff;

L_2dff:
    if ((lpfl->pt.x != lpflT->pt.x))
        goto L_2ddc;
    else
        goto L_2e12;

L_2e12:
    if ((lpfl->pt.y != lpflT->pt.y))
        goto L_2ddc;
    else
        goto L_2e25;

L_2e25:
    if ((FIsAiAttack(lpflT) != 0))
        goto LTargetBomber;
    else
        goto L_2e38;

L_2e38:
    goto L_2ddc;

L_2e47:
    lppl = lpplHome;

LTargetBomber:
    if ((game.fAisBand == 0x0))
        goto L_2e88;
    else
        goto L_2e69;

L_2e69:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_2e92;

L_2e88:
    lpplDest = 0x0;

L_2e92:
    if ((LOWORD(lpplDest) != 0x0))
        goto L_2ec0;
    else
        goto L_2e9b;

L_2e9b:
    if ((HIWORD(lpplDest) != 0x0))
        goto L_2ec0;
    else
        goto L_2ea4;

L_2ea4:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

L_2ec0:
    lppl = lpplDest;
    if ((LOWORD(lppl) != 0x0))
        goto L_2ede;
    else
        goto L_2ed5;

L_2ed5:
    if ((HIWORD(lppl) == 0x0))
        goto L_279b;
    else
        goto L_2ede;

L_2ede:
    vlpbAiPlanet[((lppl->id * 16) + 10)] = (vlpbAiPlanet[((lppl->id * 16) + 0xa)] | 0x80);
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0);

L_2f66:
    goto L_279b;

LTryScouts:
    if ((lpfl->rgcsh[0] == 0))
        goto L_279b;
    else
        goto L_2f79;

L_2f79:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->rgcsh[0] == 0))
        goto L_2fc5;
    else
        goto L_2f98;

L_2f98:
    if ((rgshdef[0].hul.rghs[0].iItem != 0x1))
        goto L_2fc5;
    else
        goto L_2fa6;

L_2fa6:
    if ((HIWORD(lpfl->rgwtMin[0x4]) > 0x0))
        goto L_2fc5;
    else
        goto L_2fb3;

L_2fb3:
    if ((HIWORD(lpfl->rgwtMin[0x4]) < 0x0))
        goto LScrapFleet;
    else
        goto L_2fb8;

L_2fb8:
    if ((LOWORD(lpfl->rgwtMin[0x4]) < 0x2))
        goto LScrapFleet;
    else
        goto L_2fc5;

L_2fc5:
    IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &(lpthWorm));
    goto L_279b;

BestSpeed:
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureCAShdefs(int16_t iroCur) {
L_3020:
    return;
}
