#include "common.h"

uint8_t vrgISIshAip[19] = {0, 3, 6, 9, 12, 19, 26, 33, 40, 47, 54, 65, 76, 87, 98, 101, 105, 112, 117};
uint8_t vrgISAip[182] = {30, 31, 10, 30, 26, 4,  30, 26, 3, 30, 26, 2,  30, 0,  0,  13, 9,  18, 11, 30, 1,  1,  11, 9,  18, 11, 8,  10, 15, 7, 4,
                         6,  9,  8,  11, 13, 0,  0,  0,  9, 8,  10, 15, 7,  3,  6,  17, 8,  11, 13, 1,  1,  1,  17, 8,  12, 10, 6,  3,  5,  3, 7,
                         9,  20, 20, 8,  12, 10, 0,  0,  0, 0,  0,  9,  19, 11, 8,  12, 10, 6,  3,  4,  2,  7,  17, 20, 20, 8,  12, 10, 1,  1, 1,
                         1,  1,  17, 19, 11, 8,  16, 10, 8, 21, 23, 12, 8,  21, 23, 23, 23, 12, 10, 30, 10, 12, 25, 32, 8,  16, 10, 19};
uint8_t vrgAiISResOrder[18] = {134, 100, 69, 37, 164, 4, 135, 102, 71, 40, 166, 7, 140, 109, 73, 43, 167, 10};

void DoMaidAiTurn(PROD *rgprod) {
    int32_t  rgResCost[4];
    int32_t  rgResAvail[4];
    int16_t  iroCur;
    uint16_t t_merge_001c_0001;

L_0000:
    if ((game.turn >= 0x14))
        goto L_0019;
    else
        goto L_0013;

L_0013:
    t_merge_001c_0001 = 0x0;
    goto L_001c;

L_0019:
    t_merge_001c_0001 = 0xf;

L_001c:
    iroCur = IroEnsureAi(0x0, 0x0, 0x0, t_merge_001c_0001);
    fMarkedPlanets = 0x0;
    HandleBasicAiTasks(iroCur, rgprod, 0xffff, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

int16_t FPotentISWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

L_012e:
    cEquiv = 0x0;
    ish = 0xb;
    goto L_0162;

L_0144:
    cEquiv = (cEquiv + lpfl->rgcsh[ish]);
    ish = (ish + 0x1);

L_0162:
    if ((ish <= 0xc))
        goto L_0144;
    else
        goto L_016b;

L_016b:
    ish = 0x9;
    goto L_0193;

L_0173:
    cEquiv = (cEquiv + (lpfl->rgcsh[ish] * 0x2));
    ish = (ish + 0x1);

L_0193:
    if ((ish <= 0xa))
        goto L_0173;
    else
        goto L_019c;

L_019c:
    if ((iPotency >= 0x2))
        goto L_01ae;
    else
        goto L_01a5;

L_01a5:
    return 0x1;

L_01ae:
    if ((cEquiv < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_01c2;
    else
        goto L_01bc;

L_01bc:
    return 0x1;

L_01c2:
    return 0x0;
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
    uint16_t t_merge_021f_0001;
    uint16_t t_merge_032d_0001;
    uint16_t t_merge_036b_0001;
    uint16_t t_merge_072d_0001;
    PLANET  *t_merge_14d6_0001;
    PLANET  *t_merge_160c_0001;

L_01e0:
    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0x0;
    cplNegative = 0x0;
    cplanCol = 0x0;
    if ((game.turn >= 0xa))
        goto L_021c;
    else
        goto L_0216;

L_0216:
    t_merge_021f_0001 = 0x0;
    goto L_021f;

L_021c:
    t_merge_021f_0001 = 0x14;

L_021f:
    iroCur = IroEnsureAi(vrgAiISResOrder, 0x12, &(ishdefSBLatest), t_merge_021f_0001);
    if ((game.turn <= 0x32))
        goto L_026c;
    else
        goto L_0245;

L_0245:
    MergeAllShdefs(0x1e0c);
    MergeAllShdefs(0x40);
    MergeAllShdefs(0x4000);
    goto L_0282;

L_026c:
    if ((game.turn <= 0x1e))
        goto L_0282;
    else
        goto L_0276;

L_0276:
    MergeAllShdefs(0x4000);

L_0282:
    j = 0x3;
    if ((game.turn <= 0x82))
        goto L_02a4;
    else
        goto L_0293;

L_0293:
    /* untranslated: j = (j + (words((game.turn + 0xff88), 0x0) / 0x14)) */

L_02a4:
    if ((j <= 0x32))
        goto L_02b4;
    else
        goto L_02ae;

L_02ae:
    j = 0x32;

L_02b4:
    LOWORD(vrgAiArmadaPotency) = LOBYTE(j);
    vrgAiArmadaPotency[0x1] = LOBYTE(((uint32_t)((j & 0xff)) / 0x2));
    j = 0x6;
    if ((game.turn <= 0x73))
        goto L_02ec;
    else
        goto L_02db;

L_02db:
    /* untranslated: j = (j + (words((game.turn + 0xff9c), 0x0) / 0x16)) */

L_02ec:
    if ((j <= 0xc))
        goto L_02fc;
    else
        goto L_02f6;

L_02f6:
    j = 0xc;

L_02fc:
    vrgAiArmadaPotency[0x2] = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 0x2) + 0xffff)))
        goto L_0320;
    else
        goto L_031a;

L_031a:
    t_merge_032d_0001 = 0x3;
    goto L_032d;

L_0320:
    t_merge_032d_0001 = (((uint32_t)(j) / 0x2) + 0xffff);

L_032d:
    vrgAiArmadaPotency[0x3] = LOBYTE(t_merge_032d_0001);
    memset(rgRecycleShdef, 0x0, 0x10);
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
    CheckAiShdefStatus(0xb, 0xc, cRecyclePeriod, &(iLatestCruiser), rgRecycleShdef);
    cExistCargo = CheckAiShdefStatus(0x4, 0x5, cRecyclePeriod, &(iLatestCargo), rgRecycleShdef);
    CheckAiShdefStatus(0x2, 0x3, cRecyclePeriod, &(iLatestBomber), rgRecycleShdef);
    CheckAiShdefStatus(0x9, 0xa, cRecyclePeriod, &(iLatestBattle), rgRecycleShdef);
    if ((game.turn <= 0x3c))
        goto L_03fb;
    else
        goto L_03ef;

L_03ef:
    SplitOutShdefs(rgRecycleShdef);

L_03fb:
    EnsureISShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0477;

L_0435:
    if ((lppl->iPlayer != 0xffff))
        goto L_0473;
    else
        goto L_0442;

L_0442:
    if ((lppl->det < 0x3))
        goto L_0473;
    else
        goto L_0454;

L_0454:
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0x0))
        goto L_0473;
    else
        goto L_046e;

L_046e:
    cplanCol = (cplanCol + 0x1);

L_0473:
    lppl = (lppl + 0x1);

L_0477:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_0435;
    else
        goto L_0485;

L_0485:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_0d31;

L_04b4:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x9))] = 0x1 */
    if ((lppl->iPlayer == idPlayer))
        goto L_056b;
    else
        goto L_04e4;

L_04e4:
    if ((lppl->iPlayer == 0xffff))
        goto L_056b;
    else
        goto L_04f1;

L_04f1:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xa))] = lobyte(((lppl->fStarbase & 0xff) + 0x1)) */
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0x0))
        goto L_0561;
    else
        goto L_0540;

L_0540:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x3))] = 0x1 */

L_0561:
    cplBadGuy = (cplBadGuy + 0x1);
    goto L_0d2d;

L_056b:
    if ((lppl->iPlayer != idPlayer))
        goto L_05bf;
    else
        goto L_057a;

L_057a:
    if ((PctPlanetDesirability(lppl, idPlayer) >= 0x0))
        goto L_05bf;
    else
        goto L_0594;

L_0594:
    cplNegative = (cplNegative + 0x1);
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] = 0x1 */
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
        goto L_05f0;

L_05f0:

L_05f6:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    b = 0x0;
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_0653;

L_063d:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_0653:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_06c3;
    else
        goto L_0666;

L_0666:
    if ((lpprod->grobj != 0x2))
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
    if ((lpprod->iItem < 0x10))
        goto L_06c3;
    else
        goto L_06ba;

L_06ba:

L_06c3:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_06e5;
    else
        goto L_06d6;

L_06d6:
    FinishProduction(0x0);
    goto L_0d2d;

L_06e5:
    if ((((uint32_t)(rgplr[idPlayer].cPlanet) / 0xa) <= (vlpbAiData[0x2] * 0x2)))
        goto L_0723;
    else
        goto L_070b;

L_070b:
    t_merge_072d_0001 = ((uint32_t)(rgplr[idPlayer].cPlanet) / 0xa);
    goto L_072d;

L_0723:
    t_merge_072d_0001 = (vlpbAiData[0x2] * 0x2);

L_072d:
    cFr = t_merge_072d_0001;
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_07a4;
    else
        goto L_0748;

L_0748:
    if ((iLatestCargo == 0xffff))
        goto L_07a4;
    else
        goto L_0752;

L_0752:
    if ((cExistCargo < cFr))
        goto L_0785;
    else
        goto L_075d;

L_075d:
    /* untranslated: branch cExistCargo >= (words(loword((0xa * cFr)), signhiword(loword((0xa * cFr)))) / 0x7) ? L_07a4 : L_0771 */

L_0771:
    if ((Random(0x4) != 0x0))
        goto L_07a4;
    else
        goto L_0785;

L_0785:
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_07a4:
    if ((cplanCol == 0x0))
        goto L_07ea;
    else
        goto L_07ae;

L_07ae:
    if ((LOWORD(rgshdef[0x1].cExist) != 0x0))
        goto L_07ea;
    else
        goto L_07b8;

L_07b8:
    if ((HIWORD(rgshdef[0x1].cExist) != 0x0))
        goto L_07ea;
    else
        goto L_07c2;

L_07c2:
    if ((game.turn <= 0xa))
        goto L_07ea;
    else
        goto L_07cc;

L_07cc:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_07ea:
    l = (uint32_t)((lppl->rgwtMin[0x3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    cRes = CResourcesAtPlanet(lppl, idPlayer);
    if ((((rgshdef[0x6].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_093c;
    else
        goto L_0839;

L_0839:
    if ((Random(0x3) != 0x0))
        goto L_093c;
    else
        goto L_084d;

L_084d:
    id = lppl->id;
    cFr = 0x0;
    ifl = 0x0;
    goto L_0868;

L_0864:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_08df;
    else
        goto L_08a3;

L_08a3:
    if ((lpfl->idPlanet != id))
        goto L_0864;
    else
        goto L_08b3;

L_08b3:
    if ((lpfl->rgcsh[0x6] <= 0x0))
        goto L_0864;
    else
        goto L_08c0;

L_08c0:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0864;
    else
        goto L_08cf;

L_08cf:
    cFr = lpfl->rgcsh[0x6];
    goto L_08df;

L_08df:
    if ((cFr < 0xa))
        goto L_0905;
    else
        goto L_08e8;

L_08e8:
    if ((cFr >= 0x11))
        goto L_093c;
    else
        goto L_08f1;

L_08f1:
    if ((Random(0x8) != 0x0))
        goto L_093c;
    else
        goto L_0905;

L_0905:
    if ((Random(((cFr * 0x2) + 0x1)) != 0x0))
        goto L_093c;
    else
        goto L_091e;

L_091e:
    AddItemToQueue(0x6, 0x3, grobjFleet, 0x1);
    fWrite = 0x1;

L_093c:
    if ((iLatestBomber == 0xffff))
        goto L_0a25;
    else
        goto L_0946;

L_0946:
    id = lppl->id;
    ifl = 0x0;
    goto L_095c;

L_0958:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
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
    if ((FPotentISWarFleet(lpfl, 0x2) == 0x0))
        goto L_0958;
    else
        goto L_09d0;

L_09d0:
    if ((iLatestBomber == 0xffff))
        goto L_0a25;
    else
        goto L_09da;

L_09da:
    if (((lpfl->rgcsh[0x2] + lpfl->rgcsh[0x3]) < (vrgAiArmadaPotency[0x2] & 0xff)))
        goto L_0a25;
    else
        goto L_09fa;

L_09fa:

L_0a00:
    AddItemToQueue(iLatestBomber, 0x4, grobjFleet, 0x1);
    fWrite = 0x1;
    goto FinishProd;

L_0a25:
    if ((iLatestCruiser == 0xffff))
        goto L_0ba1;
    else
        goto L_0a2e;

L_0a2e:
    if ((HIWORD(rgshdef[iLatestCruiser].cExist) > 0x0))
        goto L_0ba1;
    else
        goto L_0a61;

L_0a61:
    /* untranslated: branch HIWORD(rgshdef[iLatestCruiser].cExist) < ss:[bp-0xa2] ? L_0a6f : L_0a66 */

L_0a66:
    /* untranslated: branch LOWORD(rgshdef[iLatestCruiser].cExist) >= ss:[bp-0xa4] ? L_0ba1 : L_0a6f */

L_0a6f:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
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
        goto L_0adc;

L_0adc:

L_0ae2:
    i = (i + 0x1);

L_0ae6:
    if ((i < 0x4))
        goto L_0a9b;
    else
        goto L_0aef;

L_0aef:
    i = 0x0;
    goto L_0b98;

L_0af7:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestCruiser]), rgCosts);
    j = 0x0;
    goto L_0b6c;

L_0b1f:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
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
        goto L_0b61;

L_0b61:

L_0b67:
    j = (j + 0x1);

L_0b6c:
    if ((j < 0x4))
        goto L_0b1f;
    else
        goto L_0b76;

L_0b76:
    fWrite = 0x1;
    AddItemToQueue(iLatestCruiser, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_0b98:
    if ((i < 0x5))
        goto L_0af7;
    else
        goto L_0ba1;

L_0ba1:
    if ((iLatestBattle == 0xffff))
        goto FinishProd;
    else
        goto L_0bab;

L_0bab:
    if ((HIWORD(rgshdef[iLatestBattle].cExist) > 0x0))
        goto FinishProd;
    else
        goto L_0bdf;

L_0bdf:
    /* untranslated: branch HIWORD(rgshdef[iLatestBattle].cExist) < ss:[bp-0xa2] ? L_0bed : L_0be4 */

L_0be4:
    /* untranslated: branch LOWORD(rgshdef[iLatestBattle].cExist) >= ss:[bp-0xa4] ? L_0d21 : L_0bed */

L_0bed:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
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
        goto L_0c5a;

L_0c5a:

L_0c60:
    i = (i + 0x1);

L_0c64:
    if ((i < 0x4))
        goto L_0c19;
    else
        goto L_0c6d;

L_0c6d:
    i = 0x0;
    goto L_0d18;

L_0c75:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestBattle]), rgCosts);
    j = 0x0;
    goto L_0ceb;

L_0c9e:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
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
        goto L_0ce0;

L_0ce0:

L_0ce6:
    j = (j + 0x1);

L_0ceb:
    if ((j < 0x4))
        goto L_0c9e;
    else
        goto L_0cf5;

L_0cf5:
    fWrite = 0x1;
    AddItemToQueue(iLatestBattle, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_0d18:
    if ((i < 0x5))
        goto L_0c75;
    else
        goto FinishProd;

FinishProd:
    FinishProduction(fWrite);

L_0d2d:
    lppl = (lppl + 0x1);

L_0d31:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_04b4;
    else
        goto L_0d3f;

L_0d3f:
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_0d61;

L_0d5d:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
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
    if ((FIsAiAttack(lpfl) == 0x0))
        goto L_0e05;
    else
        goto L_0de4;

L_0de4:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;

L_0e05:
    lpfl->fMark = 0x0;
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_1083;
    else
        goto L_0e2f;

L_0e2f:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_0e6d;
    else
        goto L_0e41;

L_0e41:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_0e77 : L_0e5c */

L_0e5c:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_0e6d:
    idPlanDst = lpfl->idPlanet;

LCheckForColDrop:
    if ((idPlanDst == 0xffff))
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
    if ((lppl->iPlayer == 0xffff))
        goto L_0d5d;
    else
        goto L_0eb0;

L_0eb0:
    if ((lppl->iPlayer == idPlayer))
        goto L_0d5d;
    else
        goto L_0ebf;

L_0ebf:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] == 0x0 ? L_103a : L_0ee7 */

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
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto LBlowAwayOrders;
    else
        goto L_0f27;

L_0f27:
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.txp.rgia[0x3] = ((ord.txp.rgia[0x3] & 0xfff) | 0x2000);
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((sel.fl.lpplord->rgord[0x0].id != idPlanDst))
        goto L_0fe0;
    else
        goto L_0fb4;

L_0fb4:
    sel.fl.lpplord->rgord[0x0] = ord;
    goto L_1009;

L_0fe0:
    sel.fl.lpplord->rgord[0x1] = ord;

L_1009:
    FLookupFleet(0xffff, &(sel.fl));
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((idPlanDst * 0x10) + 0x3))] | 0x80) */
    goto L_0d5d;

LBlowAwayOrders:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, &(sel.fl));
    ClearAiCurrentTask(lpfl, 0x0);
    goto L_0d5d;

L_1083:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto L_0d5d;
    else
        goto L_1090;

L_1090:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_10ce;
    else
        goto L_10a2;

L_10a2:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_10d8 : L_10bd */

L_10bd:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_10ce:
    idPlanDst = lpfl->idPlanet;

L_10d8:
    if ((idPlanDst == 0xffff))
        goto L_0d5d;
    else
        goto L_10e1;

L_10e1:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] != 0x0 ? L_0e77 : L_1106 */

L_1106:

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
    if ((lppl->iPlayer == 0xffff))
        goto L_0d5d;
    else
        goto L_113f;

L_113f:
    if ((lppl->iPlayer != idPlayer))
        goto LBlowAwayOrders;
    else
        goto L_114b;

L_114b:

L_1154:
    fMarkedPlanets = 0x0;
    ifl = 0x0;
    goto L_1166;

L_1162:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto BestSpeed;
    else
        goto L_11a1;

L_11a1:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1162;
    else
        goto L_11ad;

L_11ad:

L_11b3:
    if ((lpfl->cord <= 0x1))
        goto L_1225;
    else
        goto L_11c0;

L_11c0:
    if (((HIWORD(rgshdef[0x0].hul.rghs[0x0]) & 0xff) >= 0xa))
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
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, &(sel.fl));

L_1225:
    if ((lpfl->rgcsh[0x6] == 0x0))
        goto L_12a4;
    else
        goto L_1232;

L_1232:
    if ((lpfl->cord != 0x1))
        goto L_1162;
    else
        goto L_123f;

L_123f:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x0 ? L_1162 : L_1255 */

L_1255:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x6) */
    sel.fl.lpplord->rgord[0x0].tsell.iPlrX = 0x5;
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    FLookupFleet(0xffff, &(sel.fl));

L_12a4:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto LTryFreighters;
    else
        goto L_12ae;

L_12ae:

L_12b4:
    if ((rgshdef[0x1].hul.ihuldef != ihuldefMediumFreighter))
        goto LScrapFleet;
    else
        goto L_12bb;

L_12bb:

L_12c1:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == 0xffff))
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
    if ((sel.fl.idPlanet == 0xffff))
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
    if (((HIWORD(rgshdef[0x1].hul.rghs[0x0]) & 0xff) < 0x2))
        goto LScrapFleet;
    else
        goto L_1354;

L_1354:
    if ((FMoveToNearestStarbase(lpfl, 0x0) == 0x0))
        goto LScrapFleet;
    else
        goto L_136b;

L_136b:

L_1374:
    lpthWorm = 0x0;
    idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
    if ((lpfl->idPlanet == 0xffff))
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
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 0x3, 0x96);
    FLookupFleet(lpfl->id, &(sel.fl));

L_13f5:
    if ((idPlanDst == 0xffff))
        goto L_1162;
    else
        goto L_13fe;

L_13fe:
    FColonizeAiFleet(lpfl, idPlanDst);
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0xf))] = 0x4 */

LTryFreighters:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto LTryBombers;
    else
        goto L_1443;

L_1443:

L_1449:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
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
        goto L_149b;

L_149b:

L_14a1:
    lppl = (lppl + 0x1);

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
        goto L_14eb;

L_14eb:

L_14f1:
    lppl = 0x0;
    i = 0x0;
    goto L_1507;

L_1503:
    i = (i + 0x1);

L_1507:
    if ((i >= vlpbAiData[0x2]))
        goto L_15b3;
    else
        goto L_1517;

L_1517:
    j = 0x0;
    goto L_1525;

L_1520:
    j = (j + 0x1);

L_1525:
    if ((j >= HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_1589;
    else
        goto L_1549;

L_1549:
    if ((vlpbAiData[(((0x4 + LOWORD((0x14 * i))) + 0x4) + (j * 0x2))] == lpfl->id))
        goto L_1589;
    else
        goto L_1580;

L_1580:

L_1589:
    if ((j < HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_15b3;
    else
        goto L_15aa;

L_15aa:

L_15b3:
    if ((i >= vlpbAiData[0x2]))
        goto L_15eb;
    else
        goto L_15c3;

L_15c3:
    lppl = LpplFromId(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);

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
    if ((lpfl->rgcsh[0x2] != 0x0))
        goto L_163c;
    else
        goto L_162c;

L_162c:
    if ((lpfl->rgcsh[0x3] == 0x0))
        goto LTryScouts;
    else
        goto L_1636;

L_1636:

L_163c:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == 0xffff))
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
    if ((lpfl->rgcsh[0x2] >= 0x2))
        goto L_16b0;
    else
        goto L_16a3;

L_16a3:
    if ((lpfl->rgcsh[0x3] < 0x2))
        goto L_1162;
    else
        goto L_16b0;

L_16b0:
    if ((lpfl->rgcsh[0x9] >= 0x3))
        goto L_16cd;
    else
        goto L_16bd;

L_16bd:
    if ((lpfl->rgcsh[0xa] < 0x3))
        goto L_1162;
    else
        goto L_16c7;

L_16c7:

L_16cd:
    FLookupFleet(lpfl->id, &(sel.fl));
    goto LTargetBomber;

L_16e2:
    if ((lppl->iPlayer == 0xffff))
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
    if ((FIsAiAttack(lpflT) != 0x0))
        goto LTargetBomber;
    else
        goto L_175a;

L_175a:

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
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xa))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((lppl->id * 0x10) + 0xa))] | 0x80) */
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);

LTryScouts:
    if ((lpfl->rgcsh[0x0] == 0x0))
        goto L_1162;
    else
        goto L_1895;

L_1895:

L_189b:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if (((HIWORD(rgshdef[0x0].hul.rghs[0x0]) & 0xff) >= 0xa))
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
        goto L_18d4;

L_18d4:

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
    if ((((rgshdef[0x4].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_198c;
    else
        goto L_1954;

L_1954:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_198c;
    else
        goto L_196c;

L_196c:
    FCreateAiShdef(0x4, 0x1, &(vrgISAip[(vrgISIshAip[0xe] & 0xff)]));

L_198c:
    if ((((rgshdef[0x5].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_19d7;
    else
        goto L_199f;

L_199f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_19d7;
    else
        goto L_19b7;

L_19b7:
    FCreateAiShdef(0x5, 0x3, &(vrgISAip[(vrgISIshAip[0x12] & 0xff)]));

L_19d7:
    if ((((rgshdef[0xe].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_1a9f;
    else
        goto L_19ea;

L_19ea:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_1a9f;
    else
        goto L_1a02;

L_1a02:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x6))
        goto L_1a9f;
    else
        goto L_1a1a;

L_1a1a:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x4))
        goto L_1a9f;
    else
        goto L_1a32;

L_1a32:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_1a9f;
    else
        goto L_1a4a;

L_1a4a:
    i = 0x0;
    goto L_1a95;

L_1a53:
    /* untranslated: branch FCreateAiShdef(0xe, 0x6, &vrgISAip[byte cs:[(Random(0x1) + 0x4)+0x64]]) != 0x0 ? L_1a9f : L_1a8a */

L_1a8a:

L_1a90:
    i = (i + 0x1);

L_1a95:
    if ((i < 0x4))
        goto L_1a53;
    else
        goto L_1a9f;

L_1a9f:
    if ((((rgshdef[0x1].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_1ac6;
    else
        goto L_1ab2;

L_1ab2:
    if ((LOWORD(rgshdef[0x1].cExist) != 0x0))
        goto L_1b29;
    else
        goto L_1abc;

L_1abc:
    if ((HIWORD(rgshdef[0x1].cExist) != 0x0))
        goto L_1b29;
    else
        goto L_1ac6;

L_1ac6:
    if ((((rgshdef[0x1].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_1b09;
    else
        goto L_1ad9;

L_1ad9:
    shdef = rgshdef[0x1];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x1);

L_1b09:
    FCreateAiShdef(0x1, 0x1, &(vrgISAip[(LOWORD(vrgISIshAip) & 0xff)]));

L_1b29:
    if ((((rgshdef[0x0].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_1b50;
    else
        goto L_1b3c;

L_1b3c:
    if ((LOWORD(rgshdef[0x0].cExist) != 0x0))
        goto L_1bb3;
    else
        goto L_1b46;

L_1b46:
    if ((HIWORD(rgshdef[0x0].cExist) != 0x0))
        goto L_1bb3;
    else
        goto L_1b50;

L_1b50:
    if ((((rgshdef[0x0].wFlags >> 0x9) & 0x1) != 0x0))
        goto L_1b93;
    else
        goto L_1b63;

L_1b63:
    shdef = rgshdef[0x0];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x0);

L_1b93:
    FCreateAiShdef(0x0, 0x4, &(vrgISAip[(vrgISIshAip[0x1] & 0xff)]));

L_1bb3:
    if ((((rgshdef[0x6].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_1c2e;
    else
        goto L_1bc6;

L_1bc6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x4))
        goto L_1c2e;
    else
        goto L_1bde;

L_1bde:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_1c2e;
    else
        goto L_1bf6;

L_1bf6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) < 0x6))
        goto L_1c2e;
    else
        goto L_1c0e;

L_1c0e:
    FCreateAiShdef(0x6, 0xb, &(vrgISAip[(vrgISIshAip[0x11] & 0xff)]));

L_1c2e:
    if ((((rgshdef[0x2].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_1cc1;
    else
        goto L_1c41;

L_1c41:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x8))
        goto L_1cc1;
    else
        goto L_1c59;

L_1c59:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x7))
        goto L_1cc1;
    else
        goto L_1c71;

L_1c71:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_1cc1;
    else
        goto L_1c89;

L_1c89:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_1cc1;
    else
        goto L_1ca1;

L_1ca1:
    FCreateAiShdef(0x2, 0x11, &(vrgISAip[(vrgISIshAip[0xf] & 0xff)]));

L_1cc1:
    if ((((rgshdef[0x3].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_1d54;
    else
        goto L_1cd4;

L_1cd4:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xb))
        goto L_1d54;
    else
        goto L_1cec;

L_1cec:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0xc))
        goto L_1d54;
    else
        goto L_1d04;

L_1d04:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xf))
        goto L_1d54;
    else
        goto L_1d1c;

L_1d1c:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x9))
        goto L_1d54;
    else
        goto L_1d34;

L_1d34:
    FCreateAiShdef(0x3, 0x13, &(vrgISAip[(vrgISIshAip[0x10] & 0xff)]));

L_1d54:
    if ((((rgshdef[0x9].wFlags >> 0x9) & 0x1) == 0x0))
        goto L_1e1c;
    else
        goto L_1d67;

L_1d67:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_1e1c;
    else
        goto L_1d7f;

L_1d7f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x6))
        goto L_1e1c;
    else
        goto L_1d97;

L_1d97:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xd))
        goto L_1e1c;
    else
        goto L_1daf;

L_1daf:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_1e1c;
    else
        goto L_1dc7;

L_1dc7:
    i = 0x0;
    goto L_1e12;

L_1dd0:
    /* untranslated: branch FCreateAiShdef(0x9, 0x9, &vrgISAip[byte cs:[(Random(0x4) + 0xa)+0x64]]) != 0x0 ? L_1e1c : L_1e07 */

L_1e07:

L_1e0d:
    i = (i + 0x1);

L_1e12:
    if ((i < 0x5))
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
    uint16_t t_merge_1e69_0001;
    THING   *t_merge_2a5b_0001;
    PLANET  *t_merge_2bd5_0001;
    PLANET  *t_merge_2d04_0001;

L_1e22:
    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0x0;
    cplNegative = 0x0;
    cplanCol = 0x0;
    fColonyShipInQueue = 0x0;
    fBomberInQueue = 0x0;
    if ((game.turn >= 0x14))
        goto L_1e66;
    else
        goto L_1e60;

L_1e60:
    t_merge_1e69_0001 = 0x0;
    goto L_1e69;

L_1e66:
    t_merge_1e69_0001 = 0xf;

L_1e69:
    iroCur = IroEnsureAi(0x0, 0x0, &(ishdefSBLatest), t_merge_1e69_0001);
    EnsureCAShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_1efc;

L_1ebb:
    if ((lppl->iPlayer != 0xffff))
        goto L_1ef8;
    else
        goto L_1ec8;

L_1ec8:
    if ((lppl->det < 0x3))
        goto L_1ef8;
    else
        goto L_1eda;

L_1eda:
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0x0))
        goto L_1ef8;
    else
        goto L_1ef4;

L_1ef4:
    cplanCol = (cplanCol + 0x1);

L_1ef8:
    lppl = (lppl + 0x1);

L_1efc:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1ebb;
    else
        goto L_1f0a;

L_1f0a:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_22cf;

L_1f35:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x9))] = 0x1 */
    if ((lppl->iPlayer != 0xffff))
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
    i = 0x0;
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
    /* untranslated: bT = lobyte((words(lppl->rgMinConc[i], signhiword(lppl->rgMinConc[i])) / 0x2)) */

L_1fc9:
    b = (b + LOBYTE(bT));
    i = (i + 0x1);

L_1fd6:
    if ((i < 0x3))
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
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] = b */

L_2017:
    if ((lppl->iPlayer == idPlayer))
        goto L_20ad;
    else
        goto L_2026;

L_2026:
    if ((lppl->iPlayer == 0xffff))
        goto L_20ad;
    else
        goto L_2033;

L_2033:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xa))] = lobyte(((lppl->fStarbase & 0xff) + 0x1)) */
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0x0))
        goto L_20a3;
    else
        goto L_2082;

L_2082:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x3))] = 0x1 */

L_20a3:
    cplBadGuy = (cplBadGuy + 0x1);
    goto L_22cb;

L_20ad:
    if ((lppl->iPlayer != idPlayer))
        goto L_2101;
    else
        goto L_20bc;

L_20bc:
    if ((PctPlanetDesirability(lppl, idPlayer) >= 0x0))
        goto L_2101;
    else
        goto L_20d6;

L_20d6:
    cplNegative = (cplNegative + 0x1);
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x2))] = 0x1 */
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
        goto L_2132;

L_2132:

L_2138:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    b = 0x0;
    i = 0x0;
    lpprod = lpplProdGlob->rgprod;
    goto L_218e;

L_217b:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_218e:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_21fc;
    else
        goto L_21a1;

L_21a1:
    if ((lpprod->grobj != 0x2))
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
    if ((lpprod->iItem > 0x10))
        goto L_21fc;
    else
        goto L_21f3;

L_21f3:

L_21fc:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_221e;
    else
        goto L_220f;

L_220f:
    FinishProduction(0x0);
    goto L_22cb;

L_221e:
    if ((game.turn != 0x0))
        goto L_225b;
    else
        goto L_2228;

L_2228:
    AddItemToQueue(0x0, 0x1, grobjFleet, 0x1);
    AddItemToQueue(0x0, 0x1, grobjFleet, 0x1);
    goto L_22c0;

L_225b:
    if ((fColonyShipInQueue != 0x0))
        goto L_22c0;
    else
        goto L_2264;

L_2264:
    if ((LOWORD(rgshdef[0x1].cExist) != 0x0))
        goto L_2278;
    else
        goto L_226e;

L_226e:
    if ((HIWORD(rgshdef[0x1].cExist) == 0x0))
        goto L_229e;
    else
        goto L_2278;

L_2278:
    if (((HIWORD(rgshdef[0x1].cExist) + 0x0) > 0x0))
        goto L_22c0;
    else
        goto L_2292;

L_2292:
    if (((HIWORD(rgshdef[0x1].cExist) + 0x0) < 0x0))
        goto L_229e;
    else
        goto L_2297;

L_2297:
    if (((LOWORD(rgshdef[0x1].cExist) + 0x1) >= cplanCol))
        goto L_22c0;
    else
        goto L_229e;

L_229e:
    fColonyShipInQueue = 0x1;
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_22c0:
    FinishProduction(fWrite);

L_22cb:
    lppl = (lppl + 0x1);

L_22cf:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_1f35;
    else
        goto L_22dd;

L_22dd:
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_22fd;

L_22f9:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
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
    if ((FIsTurinDroneAiAttack(lpfl) == 0x0))
        goto L_239d;
    else
        goto L_2380;

L_2380:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;

L_239d:
    lpfl->fMark = 0x0;
    if ((lpfl->rgcsh[0x7] != 0x0))
        goto L_23cb;
    else
        goto L_23be;

L_23be:
    if ((lpfl->rgcsh[0x8] == 0x0))
        goto L_2497;
    else
        goto L_23cb;

L_23cb:
    if ((lpfl->cord < 0x1))
        goto L_2497;
    else
        goto L_23d8;

L_23d8:
    if ((lpfl->idPlanet == 0xffff))
        goto L_2455;
    else
        goto L_23e5;

L_23e5:
    if ((LpplFromId(lpfl->idPlanet)->iPlayer == 0xffff))
        goto L_2448;
    else
        goto LBlowAwayOrders;

LBlowAwayOrders:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, &(sel.fl));
    ClearAiCurrentTask(lpfl, 0x0);
    goto L_22f9;

L_2448:
    idPlanDst = lpfl->idPlanet;
    goto L_2476;

L_2455:
    if ((lpfl->cord <= 0x1))
        goto L_22f9;
    else
        goto L_2462;

L_2462:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;
    goto L_2476;

L_2476:
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x1))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((idPlanDst * 0x10) + 0x1))] | 0x80) */
    goto L_22f9;

L_2497:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_26bc;
    else
        goto L_24ad;

L_24ad:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_24eb;
    else
        goto L_24bf;

L_24bf:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_24f5 : L_24da */

L_24da:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_24eb:
    idPlanDst = lpfl->idPlanet;

LCheckForColDrop:
    if ((idPlanDst == 0xffff))
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
    if ((lppl->iPlayer == 0xffff))
        goto L_22f9;
    else
        goto L_252e;

L_252e:
    if ((lppl->iPlayer == idPlayer))
        goto L_22f9;
    else
        goto L_253d;

L_253d:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] == 0x0 ? L_2402 : L_2565 */

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
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto LBlowAwayOrders;
    else
        goto L_25a5;

L_25a5:
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.txp.rgia[0x3] = ((ord.txp.rgia[0x3] & 0xfff) | 0x2000);
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((sel.fl.lpplord->rgord[0x0].id != idPlanDst))
        goto L_265c;
    else
        goto L_2630;

L_2630:
    sel.fl.lpplord->rgord[0x0] = ord;
    goto L_2685;

L_265c:
    sel.fl.lpplord->rgord[0x1] = ord;

L_2685:
    FLookupFleet(0xffff, &(sel.fl));
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((idPlanDst * 0x10) + 0x3))] | 0x80) */
    goto L_22f9;

L_26bc:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto L_22f9;
    else
        goto L_26c9;

L_26c9:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_2707;
    else
        goto L_26db;

L_26db:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_2711 : L_26f6 */

L_26f6:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_2707:
    idPlanDst = lpfl->idPlanet;

L_2711:
    if ((idPlanDst == 0xffff))
        goto L_22f9;
    else
        goto L_271a;

L_271a:
    /* untranslated: branch byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0x3))] != 0x0 ? L_24f5 : L_273f */

L_273f:

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
    if ((lppl->iPlayer == 0xffff))
        goto L_22f9;
    else
        goto L_2778;

L_2778:
    if ((lppl->iPlayer != idPlayer))
        goto LBlowAwayOrders;
    else
        goto L_2784;

L_2784:

L_278d:
    fMarkedPlanets = 0x0;
    ifl = 0x0;
    goto L_279f;

L_279b:
    ifl = (ifl + 0x1);

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
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto BestSpeed;
    else
        goto L_27da;

L_27da:
    if ((lpfl->iPlayer != idPlayer))
        goto L_279b;
    else
        goto L_27e6;

L_27e6:

L_27ec:
    if ((lpfl->rgcsh[0x7] != 0x0))
        goto L_2806;
    else
        goto L_27f9;

L_27f9:
    if ((lpfl->rgcsh[0x8] == 0x0))
        goto L_2933;
    else
        goto L_2806;

L_2806:
    if ((lpfl->idPlanet == 0xffff))
        goto L_279b;
    else
        goto L_2813;

L_2813:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    /* untranslated: b = byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lpfl->idPlanet * 0x10) + 0x1))] */
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
    FMoveAiFleet(lpfl, &(ord), 0x1);
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0x1))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((lppl->id * 0x10) + 0x1))] | 0x80) */
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lpfl->idPlanet * 0x10) + 0x1))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((lpfl->idPlanet * 0x10) + 0x1))] & 0x80) */

L_2933:
    if ((lpfl->cord > 0x1))
        goto L_279b;
    else
        goto L_293d;

L_293d:

L_2943:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto LTryFreighters;
    else
        goto L_294d;

L_294d:

L_2953:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == 0xffff))
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
    if ((sel.fl.idPlanet == 0xffff))
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
    if (((HIWORD(rgshdef[0x1].hul.rghs[0x0]) & 0xff) < 0x2))
        goto LScrapFleet;
    else
        goto L_29e5;

L_29e5:
    if ((FMoveToNearestStarbase(lpfl, 0x0) != 0x0))
        goto L_279b;
    else
        goto LScrapFleet;

LScrapFleet:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, &(sel.fl));

L_2a3a:
    lpthWorm = 0x0;
    if (((HIWORD(rgshdef[0x1].hul.rghs[0x0]) & 0xff) <= 0x1))
        goto L_2a58;
    else
        goto L_2a52;

L_2a52:
    t_merge_2a5b_0001 = lpthWorm;
    goto L_2a5b;

L_2a58:
    t_merge_2a5b_0001 = 0x0;

L_2a5b:
    idPlanDst = IdNearestColonizablePlanet(lpfl, t_merge_2a5b_0001);
    if ((lpfl->idPlanet == 0xffff))
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
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, lpfl->id, 0x3, 0x19);
    FLookupFleet(lpfl->id, &(sel.fl));

L_2acf:
    if ((idPlanDst == 0xffff))
        goto L_2b0a;
    else
        goto L_2ad8;

L_2ad8:
    FColonizeAiFleet(lpfl, idPlanDst);
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((idPlanDst * 0x10) + 0xf))] = 0x4 */
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

LTryFreighters:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto LTryBombers;
    else
        goto L_2b46;

L_2b46:

L_2b4c:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
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
        goto L_2b9a;

L_2b9a:

L_2ba0:
    lppl = (lppl + 0x1);

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
        goto L_2bea;

L_2bea:

L_2bf0:
    lppl = 0x0;
    i = 0x0;
    goto L_2c06;

L_2c02:
    i = (i + 0x1);

L_2c06:
    if ((i >= vlpbAiData[0x2]))
        goto L_2cab;
    else
        goto L_2c16;

L_2c16:
    j = 0x0;
    goto L_2c22;

L_2c1e:
    j = (j + 0x1);

L_2c22:
    if ((j >= HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_2c82;
    else
        goto L_2c45;

L_2c45:
    if ((vlpbAiData[(((0x4 + LOWORD((0x14 * i))) + 0x4) + (j * 0x2))] == lpfl->id))
        goto L_2c82;
    else
        goto L_2c79;

L_2c79:

L_2c82:
    if ((j < HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_2cab;
    else
        goto L_2ca2;

L_2ca2:

L_2cab:
    if ((i >= vlpbAiData[0x2]))
        goto L_2ce3;
    else
        goto L_2cbb;

L_2cbb:
    lppl = LpplFromId(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);

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
    if ((lpfl->rgcsh[0xd] != 0x0))
        goto L_2d34;
    else
        goto L_2d24;

L_2d24:
    if ((lpfl->rgcsh[0xe] == 0x0))
        goto LTryScouts;
    else
        goto L_2d2e;

L_2d2e:

L_2d34:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->idPlanet == 0xffff))
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
    if ((lpfl->rgcsh[0xd] >= 0x2))
        goto L_2dab;
    else
        goto L_2d9b;

L_2d9b:
    if ((lpfl->rgcsh[0xe] < 0x2))
        goto L_279b;
    else
        goto L_2da5;

L_2da5:

L_2dab:
    FLookupFleet(lpfl->id, &(sel.fl));
    goto LTargetBomber;

L_2dc0:
    if ((lppl->iPlayer == 0xffff))
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
    if ((FIsAiAttack(lpflT) != 0x0))
        goto LTargetBomber;
    else
        goto L_2e38;

L_2e38:

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
    /* untranslated: byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) + ((lppl->id * 0x10) + 0xa))] = (byte HIWORD(vlpbAiPlanet):[(LOWORD(vlpbAiPlanet) +
     * ((lppl->id * 0x10) + 0xa))] | 0x80) */
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);

LTryScouts:
    if ((lpfl->rgcsh[0x0] == 0x0))
        goto L_279b;
    else
        goto L_2f73;

L_2f73:

L_2f79:
    ChangeMainObjSel(grobjFleet, lpfl->id);
    if ((lpfl->rgcsh[0x0] == 0x0))
        goto L_2fc5;
    else
        goto L_2f98;

L_2f98:
    if (((HIWORD(rgshdef[0x0].hul.rghs[0x0]) & 0xff) != 0x1))
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
        goto L_2fbf;

L_2fbf:

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
