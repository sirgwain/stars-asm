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

L_0000:
    idSav = idPlayer;
    fAi = 0x1;
    _wsprintf(&(szExt), MPCTD, (iPlayer + 0x1));
    DestroyCurGame();
    if ((FLoadGame(0x56a2, szExt) != 0x0))
        goto L_0060;
    else
        goto L_0050;

L_0050:
    idPlayer = idSav;
    fAi = 0x0;
    goto L_02e8;

L_0060:
    /* untranslated: branch (part[84:2](rgplr[idPlayer]) & 0x1) != 0x0 ? L_0239 : L_0077 */

L_0077:

L_007d:
    vlpbAiPlanet = LpAlloc((game.cPlanMax * 0x10), htMisc);
    vrglpplAi = LpAlloc((game.cPlanMax * 0x4), htMisc);
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_00ff;
    else
        goto L_00c1;

L_00c1:
    if ((HIWORD(vlpbAiData) != 0x0))
        goto L_00ff;
    else
        goto L_00cb;

L_00cb:
    vlpbAiData = LpAlloc(0x2000, htMisc);
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_00f6;
    else
        goto L_00ec;

L_00ec:
    if ((HIWORD(vlpbAiData) == 0x0))
        goto L_00ff;
    else
        goto L_00f6;

L_00f6:
    LOWORD(vlpbAiData) = 0x2;

L_00ff:
    if ((LOWORD(vlpbAiPlanet) != 0x0))
        goto L_0113;
    else
        goto L_0109;

L_0109:
    if ((HIWORD(vlpbAiPlanet) == 0x0))
        goto Cleanup;
    else
        goto L_0113;

L_0113:
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_0127;
    else
        goto L_011d;

L_011d:
    if ((HIWORD(vlpbAiData) == 0x0))
        goto Cleanup;
    else
        goto L_0127;

L_0127:
    if ((LOWORD(vrglpplAi) != 0x0))
        goto L_013e;
    else
        goto L_0131;

L_0131:
    if ((HIWORD(vrglpplAi) == 0x0))
        goto Cleanup;
    else
        goto L_0138;

L_0138:

L_013e:
    fmemset(&(vlpbAiPlanet), 0x0, (game.cPlanMax * 0x10));
    ComputeShdefPowers();
    MarkPlanetsUnderAttack();
    IncreaseAIMinefieldSizes();
    InitRandomPlanetList();
    if ((wMdPlr == 0xffff))
        goto L_018c;
    else
        goto L_017b;

L_017b:
    /* untranslated: part[6:2](rgplr[iPlayer]) = wMdPlr */

L_018c:
    goto L_0218;
    DoRobotoidAiTurn(rgprod);
    goto Cleanup;
    DoCyberAiTurn(rgprod);
    goto Cleanup;
    DoMacintiAiTurn(rgprod);
    goto Cleanup;
    DoTurinDroneAiTurn(rgprod);
    goto Cleanup;
    DoMaidAiTurn(rgprod);
    goto Cleanup;
    DoAutomitronAiTurn(rgprod);
    goto Cleanup;
    DoRototillAiTurn(rgprod);
    goto Cleanup;

L_0218:

Cleanup:
    FWriteLogFile(0x56a2, iPlayer);
    FWriteHistFile(iPlayer);
    if ((LOWORD(vrglpplAi) != 0x0))
        goto L_0267;
    else
        goto L_025d;

L_025d:
    if ((HIWORD(vrglpplAi) == 0x0))
        goto L_0287;
    else
        goto L_0267;

L_0267:
    FreeLp(&(vrglpplAi), htMisc);
    vrglpplAi = 0x0;

L_0287:
    if ((LOWORD(vlpbAiData) != 0x0))
        goto L_029b;
    else
        goto L_0291;

L_0291:
    if ((HIWORD(vlpbAiData) == 0x0))
        goto L_02db;
    else
        goto L_029b;

L_029b:
    FreeLp(&(vlpbAiPlanet), htMisc);
    FreeLp(&(vlpbAiData), htMisc);
    vlpbAiPlanet = 0x0;
    vlpbAiData = 0x0;

L_02db:
    idPlayer = idSav;
    fAi = 0x0;

L_02e8:
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
    uint16_t t_merge_035a_0001;
    uint16_t t_merge_0441_0001;
    uint16_t t_merge_047e_0001;
    uint16_t t_merge_094e_0001;
    uint16_t t_merge_0c59_0001;
    uint16_t t_merge_0d3a_0001;
    uint16_t t_merge_0efe_0001;
    PLANET  *t_merge_19f4_0001_wide;
    PLANET  *t_merge_1bdd_0001_wide;
    uint16_t t_merge_1e4a_0001;
    uint16_t t_merge_1e65_0001;

L_0312:
    /* untranslated: iAiLvl = ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) */
    iPlanet = rgplr[idPlayer].idPlanetHome;
    if ((game.turn >= 0xa))
        goto L_0357;
    else
        goto L_0351;

L_0351:
    t_merge_035a_0001 = 0x0;
    goto L_035a;

L_0357:
    t_merge_035a_0001 = 0xf;

L_035a:
    iroCur = IroEnsureAi(vrgAiRobotoidResOrder, 0x24, &(ishdefSBLatest), t_merge_035a_0001);
    if ((game.turn <= 0x32))
        goto L_03a3;
    else
        goto L_037f;

L_037f:
    MergeAllShdefs(0x6fc);
    MergeAllShdefs(0x1);
    MergeAllShdefs(0xc000);

L_03a3:
    j = 0x4;
    if ((game.turn <= 0x82))
        goto L_03c3;
    else
        goto L_03b3;

L_03b3:
    /* untranslated: j = (j + (words((game.turn + 0xff88), 0x0) / 0x14)) */

L_03c3:
    if ((j <= 0x32))
        goto L_03d1;
    else
        goto L_03cc;

L_03cc:
    j = 0x32;

L_03d1:
    LOWORD(vrgAiArmadaPotency) = LOBYTE(j);
    /* untranslated: part[1:2](vrgAiArmadaPotency) = lobyte((sext16to32((j & 0xff)) / 0x2)) */
    j = 0x6;
    if ((game.turn <= 0x73))
        goto L_0405;
    else
        goto L_03f5;

L_03f5:
    /* untranslated: j = (j + (words((game.turn + 0xff9c), 0x0) / 0x16)) */

L_0405:
    if ((j <= 0xc))
        goto L_0413;
    else
        goto L_040e;

L_040e:
    j = 0xc;

L_0413:
    HIWORD(vrgAiArmadaPotency) = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 0x2) + 0xffff)))
        goto L_0435;
    else
        goto L_042f;

L_042f:
    t_merge_0441_0001 = 0x3;
    goto L_0441;

L_0435:
    t_merge_0441_0001 = (((uint32_t)(j) / 0x2) + 0xffff);

L_0441:
    /* untranslated: part[3:2](vrgAiArmadaPotency) = lobyte(t_merge_0441_0001) */
    memset(rgRecycleShdef, 0x0, 0x10);
    if ((game.turn >= 0x78))
        goto L_046a;
    else
        goto L_0462;

L_0462:
    cRecyclePeriod = 0x32;
    goto L_0481;

L_046a:
    if ((game.turn >= 0xc8))
        goto L_047b;
    else
        goto L_0475;

L_0475:
    t_merge_047e_0001 = 0x46;
    goto L_047e;

L_047b:
    t_merge_047e_0001 = 0x64;

L_047e:
    cRecyclePeriod = t_merge_047e_0001;

L_0481:
    /* untranslated: call CheckAiShdefStatus(0xe, 0xf, cRecyclePeriod, &iLatestDestroyer, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    i = 0xe;
    goto L_04f8;

L_04a4:
    if ((rgRecycleShdef[i] == 0x0))
        goto L_04f4;
    else
        goto L_04b9;

L_04b9:
    /* untranslated: branch ((part[123:2](rgshdef[i]) >> 0x9) & 0x1) != 0x0 ? L_04f4 : L_04d7 */

L_04d7:
    if ((rgshdef[i].hul.ihuldef != ihuldefNubian))
        goto L_04f4;
    else
        goto L_04e9;

L_04e9:
    rgRecycleShdef[i] = 0x0;

L_04f4:
    i = (i + 0x1);

L_04f8:
    if ((i <= 0xf))
        goto L_04a4;
    else
        goto L_0501;

L_0501:
    /* untranslated: cExistCargo = CheckAiShdefStatus(0xb, 0xd, cRecyclePeriod, &iLatestCargo, &part[0:1](rgRecycleShdef)) */
    /* untranslated: call CheckAiShdefStatus(0x9, 0xa, cRecyclePeriod, &iLatestBomber, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x2, 0x5, cRecyclePeriod, &iLatestMeta, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x6, 0x7, (words(loword((0x3 * cRecyclePeriod)), 0x0) / 0x2), &iLatestBattle, &part[0:1](rgRecycleShdef)) ->
     * callresult(int16_t) */
    if ((game.turn <= 0x50))
        goto L_0611;
    else
        goto L_0586;

L_0586:
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[0:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[1:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0x10);
    /* untranslated: part[13:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: part[12:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: part[11:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */

L_0611:
    EnsureRobotoidShdefs();
    cFlDestroyers = 0x0;
    ifl = 0x0;
    goto L_0627;

L_0623:
    ifl = (ifl + 0x1);

L_0627:
    if ((ifl >= cFleet))
        goto L_069b;
    else
        goto L_0632;

L_0632:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0662;
    else
        goto L_065a;

L_065a:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_069b;
    else
        goto L_0662;

L_0662:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0623;
    else
        goto L_0671;

L_0671:
    if ((iLatestDestroyer == 0xffff))
        goto L_0623;
    else
        goto L_067a;

L_067a:
    if ((lpfl->rgcsh[0xe] != 0x0))
        goto L_0694;
    else
        goto L_0687;

L_0687:
    if ((lpfl->rgcsh[0xf] == 0x0))
        goto L_0623;
    else
        goto L_0694;

L_0694:
    cFlDestroyers = (cFlDestroyers + 0x1);

L_069b:
    fShouldColonize = FShouldWeBuildColonizers(&(cColFleet));
    UpdateProgressGauge(0xfc62);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_078c;

L_06e5:
    if ((lppl->iPlayer == idPlayer))
        goto L_0788;
    else
        goto L_06f4;

L_06f4:
    if ((lppl->iPlayer == 0xffff))
        goto L_0788;
    else
        goto L_0701;

L_0701:
    /* untranslated: i = ((words(lppl->uPopGuess, 0x0) / 0xfa) + 0x1) */
    if ((i <= 0x6))
        goto L_0726;
    else
        goto L_0721;

L_0721:
    i = 0x6;

L_0726:
    if ((lppl->fStarbase == 0x0))
        goto L_0741;
    else
        goto L_073d;

L_073d:
    i = (i + 0x1);

L_0741:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] = LOBYTE(i);
    vlpbAiPlanet[((lppl->id * 0x10) + 0x9)] = 0x1;

L_0788:
    lppl = (lppl + 0x1);

L_078c:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_06e5;
    else
        goto L_079a;

L_079a:
    ipl = 0x0;
    goto L_07a6;

L_07a2:
    ipl = (ipl + 0x1);

L_07a6:
    if ((ipl >= vclpplAi))
        goto L_1097;
    else
        goto L_07b1;

L_07b1:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_07e1;
    else
        goto L_07d9;

L_07d9:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_1097;
    else
        goto L_07e1;

L_07e1:
    if ((lppl->fStarbase == 0x0))
        goto L_07a2;
    else
        goto L_07f8;

L_07f8:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_0818;
    else
        goto L_0805;

L_0805:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_07a2;
    else
        goto L_080a;

L_080a:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0xc8))
        goto L_07a2;
    else
        goto L_0812;

L_0812:

L_0818:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_0870;

L_085a:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_0870:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_08e0;
    else
        goto L_0883;

L_0883:
    if ((lpprod->grobj != 0x2))
        goto L_085a;
    else
        goto L_08a4;

L_08a4:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_085a : L_08ac */

L_08ac:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) > 0x0))
        goto L_085a;
    else
        goto L_08cd;

L_08cd:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_08e0 : L_08d2 */

L_08d2:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x10 ? L_08e0 : L_08d7 */

L_08d7:

L_08e0:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_0902;
    else
        goto L_08f3;

L_08f3:
    FinishProduction(0x0);
    goto L_07a2;

L_0902:
    if ((((uint32_t)(rgplr[idPlayer].cPlanet) / 0x8) <= (vlpbAiData[0x2] * 0x4)))
        goto L_0942;
    else
        goto L_092a;

L_092a:
    t_merge_094e_0001 = ((uint32_t)(rgplr[idPlayer].cPlanet) / 0x8);
    goto L_094e;

L_0942:
    t_merge_094e_0001 = (vlpbAiData[0x2] * 0x4);

L_094e:
    cFr = t_merge_094e_0001;
    if ((iLatestCargo == 0xffff))
        goto L_09ae;
    else
        goto L_095a;

L_095a:
    if ((cExistCargo < ((uint32_t)((cFr * 0x8)) / 0xa)))
        goto L_0990;
    else
        goto L_0971;

L_0971:
    if ((cExistCargo >= cFr))
        goto L_09ae;
    else
        goto L_097c;

L_097c:
    if ((Random(0x3) != 0x0))
        goto L_09ae;
    else
        goto L_0990;

L_0990:
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_09ae:
    if ((fShouldColonize != 0x0))
        goto L_09e2;
    else
        goto L_09b7;

L_09b7:
    if ((cColFleet > 0x19))
        goto TryShip2;
    else
        goto L_09c0;

L_09c0:
    if ((Random((vlpbAiData[0x2] * 0x8)) != 0x0))
        goto TryShip2;
    else
        goto L_09dc;

L_09dc:

L_09e2:
    if ((game.turn < 0x5))
        goto TryShip2;
    else
        goto L_09e9;

L_09e9:

L_09ef:
    if ((game.turn > 0x14))
        goto L_0a11;
    else
        goto L_09f9;

L_09f9:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);

L_0a11:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;
    l = (uint32_t)((lppl->rgwtMin[0x3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    cRes = CResourcesAtPlanet(lppl, idPlayer);
    if ((HIWORD(l) < 0x0))
        goto TryShip2;
    else
        goto L_0a75;

L_0a75:
    if ((HIWORD(l) > 0x0))
        goto L_0a85;
    else
        goto L_0a7a;

L_0a7a:
    if ((LOWORD(l) <= 0x8fc))
        goto TryShip2;
    else
        goto L_0a85;

L_0a85:
    if ((cRes <= 0x23))
        goto TryShip2;
    else
        goto L_0a8e;

L_0a8e:
    if ((iAiLvl <= 0x0))
        goto TryShip2;
    else
        goto L_0a97;

L_0a97:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    if ((HIWORD(l) < 0x0))
        goto TryShip2;
    else
        goto L_0ab9;

L_0ab9:
    if ((HIWORD(l) > 0x0))
        goto L_0ac9;
    else
        goto L_0abe;

L_0abe:
    if ((LOWORD(l) <= 0xe10))
        goto TryShip2;
    else
        goto L_0ac9;

L_0ac9:
    if ((cRes <= 0x32))
        goto TryShip2;
    else
        goto L_0ad2;

L_0ad2:
    if ((iAiLvl <= 0x1))
        goto TryShip2;
    else
        goto L_0adb;

L_0adb:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);

TryShip2:
    if ((LOWORD(rgshdef) != 0x5))
        goto L_0c00;
    else
        goto L_0afd;

L_0afd:
    if ((Random(0x4) != 0x0))
        goto L_0c00;
    else
        goto L_0b11;

L_0b11:
    id = lppl->id;
    cFr = 0x0;
    ifl = 0x0;
    goto L_0b2c;

L_0b28:
    ifl = (ifl + 0x1);

L_0b2c:
    if ((ifl >= cFleet))
        goto L_0ba3;
    else
        goto L_0b37;

L_0b37:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0b67;
    else
        goto L_0b5f;

L_0b5f:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_0ba3;
    else
        goto L_0b67;

L_0b67:
    if ((lpfl->idPlanet != id))
        goto L_0b28;
    else
        goto L_0b77;

L_0b77:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_0b28;
    else
        goto L_0b84;

L_0b84:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0b28;
    else
        goto L_0b93;

L_0b93:
    cFr = lpfl->rgcsh[0x0];
    goto L_0ba3;

L_0ba3:
    if ((cFr < 0xa))
        goto L_0bc9;
    else
        goto L_0bac;

L_0bac:
    if ((cFr >= 0x11))
        goto L_0c00;
    else
        goto L_0bb5;

L_0bb5:
    if ((Random(0xa) != 0x0))
        goto L_0c00;
    else
        goto L_0bc9;

L_0bc9:
    /* untranslated: branch Random(part[1:0](mpicolgrbitBU[cFr])) != 0x0 ? L_0c00 : L_0be2 */

L_0be2:
    AddItemToQueue(0x0, 0x4, grobjFleet, 0x1);
    fWrite = 0x1;

L_0c00:
    i = 0x0;
    goto L_0c3e;

L_0c08:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_0c3a;
    else
        goto L_0c28;

L_0c28:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_0c47;
    else
        goto L_0c2d;

L_0c2d:
    if ((LOWORD(lppl->rgwtMin[i]) < 0x1388))
        goto L_0c47;
    else
        goto L_0c34;

L_0c34:

L_0c3a:
    i = (i + 0x1);

L_0c3e:
    if ((i <= 0x2))
        goto L_0c08;
    else
        goto L_0c47;

L_0c47:
    if ((i != 0x2))
        goto L_0c56;
    else
        goto L_0c50;

L_0c50:
    t_merge_0c59_0001 = 0x1;
    goto L_0c59;

L_0c56:
    t_merge_0c59_0001 = 0x0;

L_0c59:
    fTonsOfMinerals = t_merge_0c59_0001;
    if ((iLatestBomber == 0xffff))
        goto L_0d54;
    else
        goto L_0c66;

L_0c66:
    id = lppl->id;
    ifl = 0x0;
    goto L_0c7c;

L_0c78:
    ifl = (ifl + 0x1);

L_0c7c:
    if ((ifl >= cFleet))
        goto L_0d54;
    else
        goto L_0c87;

L_0c87:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_0cb7;
    else
        goto L_0caf;

L_0caf:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_0d54;
    else
        goto L_0cb7;

L_0cb7:
    if ((lpfl->idPlanet != id))
        goto L_0c78;
    else
        goto L_0cc7;

L_0cc7:
    if ((lpfl->iPlayer != idPlayer))
        goto L_0c78;
    else
        goto L_0cd6;

L_0cd6:
    if ((FPotentRobWarFleet(lpfl, 0x2) == 0x0))
        goto L_0c78;
    else
        goto L_0cf0;

L_0cf0:
    if ((iLatestBomber == 0xffff))
        goto L_0d54;
    else
        goto L_0cfa;

L_0cfa:
    if (((lpfl->rgcsh[0x9] + lpfl->rgcsh[0xa]) >= (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_0d54;
    else
        goto L_0d1a;

L_0d1a:

L_0d20:
    if ((fTonsOfMinerals == 0x0))
        goto L_0d37;
    else
        goto L_0d31;

L_0d31:
    t_merge_0d3a_0001 = 0x6;
    goto L_0d3a;

L_0d37:
    t_merge_0d3a_0001 = 0x4;

L_0d3a:
    AddItemToQueue(iLatestBomber, t_merge_0d3a_0001, grobjFleet, 0x1);
    fWrite = 0x1;
    goto FinishProd;

L_0d54:
    if ((iLatestMeta == 0xffff))
        goto TryShip3;
    else
        goto L_0d5d;

L_0d5d:
    /* untranslated: branch part[133:2](rgshdef[iLatestMeta]) < 0x0 ? L_0db2 : L_0d90 */

L_0d90:
    /* untranslated: branch part[133:2](rgshdef[iLatestMeta]) > ss:[bp-0x8e] ? L_0d9e : L_0d95 */

L_0d95:
    /* untranslated: branch part[131:2](rgshdef[iLatestMeta]) < ss:[bp-0x90] ? L_0db2 : L_0d9e */

L_0d9e:
    if ((Random(0x2) == 0x0))
        goto TryShip3;
    else
        goto L_0db2;

L_0db2:
    if ((iLatestBattle == 0xffff))
        goto L_0dd9;
    else
        goto L_0dbb;

L_0dbb:
    if ((Random(0x2) != 0x0))
        goto L_0dd9;
    else
        goto L_0dcf;

L_0dcf:
    iLatest = iLatestBattle;
    goto L_0de0;

L_0dd9:
    iLatest = iLatestMeta;

L_0de0:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_0e57;

L_0e0c:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_0e53;
    else
        goto L_0e43;

L_0e43:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0e48;

L_0e48:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0e4d;

L_0e4d:

L_0e53:
    i = (i + 0x1);

L_0e57:
    if ((i < 0x4))
        goto L_0e0c;
    else
        goto L_0e60;

L_0e60:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatest]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_0edb;

L_0e88:
    /* untranslated: LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - (words(loword((0x3 * rgCosts[j])), 0x0) / 0x5)) */
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_0ed7;
    else
        goto L_0ec7;

L_0ec7:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto TryShip3;
    else
        goto L_0ecc;

L_0ecc:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto TryShip3;
    else
        goto L_0ed1;

L_0ed1:

L_0ed7:
    j = (j + 0x1);

L_0edb:
    if ((j < 0x4))
        goto L_0e88;
    else
        goto L_0ee4;

L_0ee4:
    if ((fTonsOfMinerals == 0x0))
        goto L_0efb;
    else
        goto L_0ef5;

L_0ef5:
    t_merge_0efe_0001 = 0x5;
    goto L_0efe;

L_0efb:
    t_merge_0efe_0001 = 0x1;

L_0efe:
    AddItemToQueue(iLatest, t_merge_0efe_0001, grobjFleet, 0x1);
    fWrite = 0x1;

TryShip3:
    if ((iLatestDestroyer == 0xffff))
        goto FinishProd;
    else
        goto L_0f1b;

L_0f1b:
    /* untranslated: branch part[133:2](rgshdef[iLatestDestroyer]) > 0x0 ? L_1088 : L_0f4e */

L_0f4e:
    /* untranslated: branch part[133:2](rgshdef[iLatestDestroyer]) < ss:[bp-0x8e] ? L_0f5c : L_0f53 */

L_0f53:
    /* untranslated: branch part[131:2](rgshdef[iLatestDestroyer]) >= ss:[bp-0x90] ? L_1088 : L_0f5c */

L_0f5c:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_0fd3;

L_0f88:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_0fcf;
    else
        goto L_0fbf;

L_0fbf:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0fc4;

L_0fc4:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_0fc9;

L_0fc9:

L_0fcf:
    i = (i + 0x1);

L_0fd3:
    if ((i < 0x4))
        goto L_0f88;
    else
        goto L_0fdc;

L_0fdc:
    i = 0x0;
    goto L_107f;

L_0fe4:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestDestroyer]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_1054;

L_100b:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_1050;
    else
        goto L_1040;

L_1040:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_1045;

L_1045:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_104a;

L_104a:

L_1050:
    j = (j + 0x1);

L_1054:
    if ((j < 0x4))
        goto L_100b;
    else
        goto L_105d;

L_105d:
    fWrite = 0x1;
    AddItemToQueue(iLatestDestroyer, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_107f:
    if ((i < 0x5))
        goto L_0fe4;
    else
        goto FinishProd;

FinishProd:
    FinishProduction(fWrite);
    goto L_07a2;

L_1097:
    UpdateProgressGauge(0xfc62);
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_10c3;

L_10bf:
    ifl = (ifl + 0x1);

L_10c3:
    if ((ifl >= cFleet))
        goto L_1978;
    else
        goto L_10ce;

L_10ce:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_10fe;
    else
        goto L_10f6;

L_10f6:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1978;
    else
        goto L_10fe;

L_10fe:
    if ((lpfl->cord <= 0x1))
        goto L_11df;
    else
        goto L_110b;

L_110b:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x8 ? L_11df : L_1126 */

L_1126:
    dx = (lpfl->pt.x - lpfl->lpplord->rgord[0x1].pt.x);
    dy = (lpfl->pt.y - lpfl->lpplord->rgord[0x1].pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) < 0x0))
        goto L_11df;
    else
        goto L_119e;

L_119e:
    if ((HIWORD(lDist) > 0x0))
        goto L_11ae;
    else
        goto L_11a3;

L_11a3:
    if ((LOWORD(lDist) <= 0x9c40))
        goto L_11df;
    else
        goto L_11ae;

L_11ae:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);

L_11df:
    if ((lpfl->iPlayer == idPlayer))
        goto L_1211;
    else
        goto L_11ee;

L_11ee:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_10bf;

L_1211:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_1375;
    else
        goto L_121e;

L_121e:
    if ((game.turn <= 0x28))
        goto L_1375;
    else
        goto L_1228;

L_1228:
    if ((lpfl->cord != 0x1))
        goto L_1375;
    else
        goto L_1235;

L_1235:
    if ((lpfl->rgcsh[0x0] < 0x7))
        goto L_130d;
    else
        goto L_1242;

L_1242:
    if ((Random(0x5) != 0x0))
        goto L_130d;
    else
        goto L_1256;

L_1256:
    /* untranslated: call IdRandomPlanetNearby(words(lpfl->pt.y, lpfl->pt.x), 0x69, 0x1) -> callresult(int16_t) */
    /* untranslated: idPlanDst = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) == 0xffff ? L_130d : L_127c */

L_127c:
    if ((idPlanDst == lpfl->idPlanet))
        goto L_130d;
    else
        goto L_128b;

L_128b:
    ClearAiCurrentTask(lpfl, 0x1);
    ord.id = idPlanDst;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);
    goto L_15d1;

L_130d:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x0 ? L_15d1 : L_1323 */

L_1323:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x6) */
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    FLookupFleet(0xffff, 0x4972);
    goto L_10bf;

L_1375:
    if ((FIsAiAttack(lpfl) == 0x0))
        goto L_14c8;
    else
        goto L_138b;

L_138b:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x6 ? L_13d0 : L_13be */

L_13be:
    ClearAiCurrentTask(lpfl, 0x1);

L_13d0:
    j = 0x2;
    goto L_13fc;

L_13d8:
    if ((lpfl->rgcsh[j] > 0x0))
        goto L_1405;
    else
        goto L_13f2;

L_13f2:

L_13f8:
    j = (j + 0x1);

L_13fc:
    if ((j <= 0x7))
        goto L_13d8;
    else
        goto L_1405;

L_1405:
    if ((j > 0x7))
        goto L_15d1;
    else
        goto L_140e;

L_140e:
    if ((lpfl->cord <= 0x1))
        goto L_1436;
    else
        goto L_141b;

L_141b:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_1443 : L_1436 */

L_1436:
    if ((lpfl->idPlanet == 0xffff))
        goto L_15d1;
    else
        goto L_1443;

L_1443:
    if ((lpfl->cord <= 0x1))
        goto L_147d;
    else
        goto L_1450;

L_1450:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_147d : L_146b */

L_146b:
    id = lpfl->lpplord->rgord[0x1].id;
    goto L_1488;

L_147d:
    id = lpfl->idPlanet;

L_1488:
    lpb = ((uint8_t *)(vlpbAiPlanet) + ((id * 0x10) + 0xa));
    if ((*(lpb) == 0x0))
        goto L_15d1;
    else
        goto L_14bd;

L_14bd:
    *(lpb) = (*(lpb) | 0x80);

L_14c8:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_15d1;
    else
        goto L_14de;

L_14de:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_1532;
    else
        goto L_14f0;

L_14f0:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x0 ? L_1532 : L_1506 */

L_1506:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_153c : L_1521 */

L_1521:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_1532:
    idPlanDst = lpfl->idPlanet;

L_153c:
    if ((idPlanDst == 0xffff))
        goto L_15d1;
    else
        goto L_1545;

L_1545:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_1568;
    else
        goto L_155f;

L_155f:
    if ((HIWORD(lppl) == 0x0))
        goto L_1577;
    else
        goto L_1568;

L_1568:
    if ((lppl->iPlayer == idPlayer))
        goto L_15d1;
    else
        goto L_1577;

L_1577:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_15d1;
    else
        goto L_1584;

L_1584:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_15d1;
    else
        goto L_158e;

L_158e:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);
    ClearAiCurrentTask(lpfl, 0x0);

L_15d1:
    if ((game.turn > 0x14))
        goto L_1623;
    else
        goto L_15db;

L_15db:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_1623;
    else
        goto L_15e8;

L_15e8:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_10bf;

L_1623:
    if ((lpfl->cord > 0x1))
        goto L_10bf;
    else
        goto L_162d;

L_162d:

L_1633:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto L_10bf;
    else
        goto L_1640;

L_1640:
    if ((game.turn >= 0x5))
        goto L_1657;
    else
        goto L_164a;

L_164a:
    if ((game.mdStartDist != 0x0))
        goto L_10bf;
    else
        goto L_1651;

L_1651:

L_1657:
    if ((iAiLvl <= 0x1))
        goto L_1876;
    else
        goto L_1660;

L_1660:
    if ((lpfl->idPlanet == 0xffff))
        goto L_1876;
    else
        goto L_166d;

L_166d:
    lpplDrop = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lpplDrop) != 0x0))
        goto L_1698;
    else
        goto L_168e;

L_168e:
    if ((HIWORD(lpplDrop) == 0x0))
        goto L_1876;
    else
        goto L_1698;

L_1698:
    if ((lpplDrop->iPlayer == 0xffff))
        goto L_1876;
    else
        goto L_16a6;

L_16a6:
    if ((lpplDrop->iPlayer == idPlayer))
        goto L_1876;
    else
        goto L_16b6;

L_16b6:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_1876;
    else
        goto L_16c3;

L_16c3:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_16d2;
    else
        goto L_16c8;

L_16c8:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_1876;
    else
        goto L_16d2;

L_16d2:
    if ((GetRaceStat(rgplr[lpplDrop->iPlayer], rsMajorAdv) == raMacintosh))
        goto L_1876;
    else
        goto L_16f7;

L_16f7:
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[lpplDrop->id].x;
    ord.pt.y = rgptPlan[lpplDrop->id].y;
    ord.id = lpplDrop->id;
    ord.grobj = grobjPlanet;
    ord.fValidTask = 0x1;
    ord.grTask = grTaskXfer;
    /* untranslated: part[14:2](ord) = ((part[14:2](ord) & 0xfff) | 0x2000) */
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.lpplord->rgord[0x0] = ord;
    FLookupFleet(0xffff, 0x4972);
    lpplDrop = LpplFindClosestEnum(lpplDrop, FEnumOurStarbase);
    if ((LOWORD(lpplDrop) != 0x0))
        goto L_17e9;
    else
        goto L_17df;

L_17df:
    if ((HIWORD(lpplDrop) == 0x0))
        goto L_10bf;
    else
        goto L_17e9;

L_17e9:
    memset(ord, 0x0, 0x12);
    ord.id = lpplDrop->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplDrop->id].x;
    ord.pt.y = rgptPlan[lpplDrop->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);

L_1876:
    idPlanDst = IdNearestColonizablePlanet(lpfl, &(lpthWorm));
    if ((idPlanDst != 0xffff))
        goto L_18ee;
    else
        goto L_1894;

L_1894:
    if ((LOWORD(lpthWorm) != 0x0))
        goto L_18ee;
    else
        goto L_189d;

L_189d:
    if ((HIWORD(lpthWorm) != 0x0))
        goto L_18ee;
    else
        goto L_18a6;

L_18a6:
    if ((lpfl->idPlanet == 0xffff))
        goto L_10bf;
    else
        goto L_18b3;

L_18b3:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);

L_18ee:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet == 0xffff))
        goto L_1944;
    else
        goto L_190d;

L_190d:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0xa);
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_1944:
    if ((idPlanDst == 0xffff))
        goto L_1961;
    else
        goto L_194d;

L_194d:
    FColonizeAiFleet(lpfl, idPlanDst);
    goto L_10bf;

L_1961:
    FGotoWormholeAiFleet(lpfl, lpthWorm);

L_1978:
    ipl = 0x0;
    goto L_1984;

L_1980:
    ipl = (ipl + 0x1);

L_1984:
    if ((ipl >= vclpplAi))
        goto L_19dc;
    else
        goto L_198f;

L_198f:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_19bf;
    else
        goto L_19b7;

L_19b7:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_19dc;
    else
        goto L_19bf;

L_19bf:
    if ((lppl->fStarbase != 0x0))
        goto L_19dc;
    else
        goto L_19d3;

L_19d3:

L_19dc:
    if ((ipl != vclpplAi))
        goto L_19ee;
    else
        goto L_19e7;

L_19e7:
    t_merge_19f4_0001_wide = 0x0;
    goto L_19f4;

L_19ee:
    t_merge_19f4_0001_wide = lppl;

L_19f4:
    lpplHome = t_merge_19f4_0001_wide;
    if ((LOWORD(lpplHome) != 0x0))
        goto L_1a0f;
    else
        goto L_1a03;

L_1a03:
    if ((HIWORD(lpplHome) == 0x0))
        goto AtkMissions;
    else
        goto L_1a09;

L_1a09:

L_1a0f:
    ifl = 0x0;
    goto L_1a1b;

L_1a17:
    ifl = (ifl + 0x1);

L_1a1b:
    if ((ifl >= cFleet))
        goto L_1bf0;
    else
        goto L_1a26;

L_1a26:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_1a56;
    else
        goto L_1a4e;

L_1a4e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1bf0;
    else
        goto L_1a56;

L_1a56:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1a17;
    else
        goto L_1a65;

L_1a65:
    if ((lpfl->cord > 0x1))
        goto L_1a17;
    else
        goto L_1a6f;

L_1a6f:

L_1a75:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_1a17;
    else
        goto L_1a88;

L_1a88:

L_1a8e:
    if ((lpfl->iplan == 0x4))
        goto L_1ac7;
    else
        goto L_1aa0;

L_1aa0:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.iplan = 0x4;
    FLookupFleet(0xffff, 0x4972);

L_1ac7:
    lppl = 0x0;
    i = 0x0;
    goto L_1add;

L_1ad9:
    i = (i + 0x1);

L_1add:
    if ((i >= vlpbAiData[0x2]))
        goto L_1b84;
    else
        goto L_1aed;

L_1aed:
    j = 0x0;
    goto L_1af9;

L_1af5:
    j = (j + 0x1);

L_1af9:
    if ((j >= HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_1b5b;
    else
        goto L_1b1c;

L_1b1c:
    if ((vlpbAiData[(((0x4 + LOWORD((0x14 * i))) + 0x4) + (j * 0x2))] == LOWORD(lpfl)))
        goto L_1b5b;
    else
        goto L_1b52;

L_1b52:

L_1b5b:
    if ((j < HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_1b84;
    else
        goto L_1b7b;

L_1b7b:

L_1b84:
    if ((i >= vlpbAiData[0x2]))
        goto L_1bbc;
    else
        goto L_1b94;

L_1b94:
    lppl = LpplFromId(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);

L_1bbc:
    if ((LOWORD(lppl) != 0x0))
        goto L_1bce;
    else
        goto L_1bc5;

L_1bc5:
    if ((HIWORD(lppl) == 0x0))
        goto L_1bd7;
    else
        goto L_1bce;

L_1bce:
    t_merge_1bdd_0001_wide = lppl;
    goto L_1bdd;

L_1bd7:
    t_merge_1bdd_0001_wide = lpplHome;

L_1bdd:
    IdTargetFreighter(lpfl, t_merge_1bdd_0001_wide);
    goto L_1a17;

L_1bf0:
    UpdateProgressGauge(0xfc62);

AtkMissions:
    ifl = 0x0;
    goto L_1c08;

L_1c04:
    ifl = (ifl + 0x1);

L_1c08:
    if ((ifl >= cFleet))
        goto L_1f0f;
    else
        goto L_1c13;

L_1c13:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_1c43;
    else
        goto L_1c3b;

L_1c3b:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1f0f;
    else
        goto L_1c43;

L_1c43:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1c04;
    else
        goto L_1c4f;

L_1c4f:

L_1c55:
    i = 0x0;
    goto L_1c96;

L_1c5d:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_1c92;
    else
        goto L_1c7a;

L_1c7a:
    if ((rgRecycleShdef[i] == 0x0))
        goto L_1c9f;
    else
        goto L_1c8c;

L_1c8c:

L_1c92:
    i = (i + 0x1);

L_1c96:
    if ((i < 0x10))
        goto L_1c5d;
    else
        goto L_1c9f;

L_1c9f:
    if ((i != 0x10))
        goto L_1da4;
    else
        goto L_1ca8;

L_1ca8:
    if ((lpfl->idPlanet == 0xffff))
        goto L_1d52;
    else
        goto L_1cb5;

L_1cb5:
    LpplFromId(lpfl->idPlanet);
    /* untranslated: lppl = callresult(PLANET *) */
    /* untranslated: branch faroff(callresult(PLANET *)) != 0x0 ? L_1cda : L_1cd2 */

L_1cd2:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_1d52 : L_1cda */

L_1cda:
    if ((lppl->iPlayer != idPlayer))
        goto L_1d52;
    else
        goto L_1ce9;

L_1ce9:
    if ((lppl->fStarbase != 0x0))
        goto L_1d14;
    else
        goto L_1d00;

L_1d00:
    if ((Random(0x5) != 0x0))
        goto L_1d52;
    else
        goto L_1d14;

L_1d14:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_1c04;

L_1d52:
    if ((lpfl->cord <= 0x1))
        goto L_1d87;
    else
        goto L_1d5f;

L_1d5f:
    if ((lpfl->idPlanet != 0xffff))
        goto L_1d87;
    else
        goto L_1d6c;

L_1d6c:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_1c04 : L_1d87 */

L_1d87:
    if ((FMoveToNearestStarbase(lpfl, 0x0) != 0x0))
        goto L_1c04;
    else
        goto L_1d9e;

L_1d9e:

L_1da4:
    i = 0x2;
    goto L_1dde;

L_1dac:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_1dda;
    else
        goto L_1dc9;

L_1dc9:
    IdTargetArmada(lpfl);
    goto L_1de7;

L_1dda:
    i = (i + 0x1);

L_1dde:
    if ((i <= 0xa))
        goto L_1dac;
    else
        goto L_1de7;

L_1de7:
    if ((i <= 0xa))
        goto L_1c04;
    else
        goto L_1ded;

L_1ded:

L_1df3:
    if ((FIsAiAttack(lpfl) == 0x0))
        goto L_1c04;
    else
        goto L_1e06;

L_1e06:

L_1e0c:
    if ((lpfl->cord <= 0x1))
        goto L_1e37;
    else
        goto L_1e19;

L_1e19:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x2 ? L_1c04 : L_1e31 */

L_1e31:

L_1e37:
    if ((game.turn <= 0x78))
        goto L_1e47;
    else
        goto L_1e41;

L_1e41:
    t_merge_1e4a_0001 = 0x32;
    goto L_1e4a;

L_1e47:
    t_merge_1e4a_0001 = 0x46;

L_1e4a:
    if ((cFlDestroyers > t_merge_1e4a_0001))
        goto L_1e81;
    else
        goto L_1e52;

L_1e52:
    if ((game.turn <= 0x78))
        goto L_1e62;
    else
        goto L_1e5c;

L_1e5c:
    t_merge_1e65_0001 = 0x28;
    goto L_1e65;

L_1e62:
    t_merge_1e65_0001 = 0x3c;

L_1e65:
    if ((cFlDestroyers <= t_merge_1e65_0001))
        goto L_1ee3;
    else
        goto L_1e6d;

L_1e6d:
    if ((Random(0x3) != 0x0))
        goto L_1ee3;
    else
        goto L_1e81;

L_1e81:
    if ((lpfl->rgcsh[iLatestDestroyer] < 0x14))
        goto L_1eb2;
    else
        goto L_1e9e;

L_1e9e:
    if ((Random(0x14) != 0x0))
        goto L_1ee3;
    else
        goto L_1eb2;

L_1eb2:
    if ((FFindBuddyAndJoinUp(lpfl, 0xe, 0xf, 0x24, 0x48) != 0x0))
        goto L_1c04;
    else
        goto L_1edd;

L_1edd:

L_1ee3:
    IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);
    goto L_1c04;

L_1f0f:
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureRobotoidShdefs() {
    int16_t  ish;
    int16_t  i;
    int16_t  shBase;
    SHDEF    shdef;
    uint16_t t_merge_2173_0001;
    uint16_t t_merge_24c8_0001;
    uint16_t t_merge_263a_0001;
    uint16_t t_merge_2758_0001;

L_20ae:
    ish = 0xb;
    goto L_21eb;

L_20bf:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_21e7 : L_20dd */

L_20dd:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x2))
        goto L_21e7;
    else
        goto L_20f5;

L_20f5:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < (LOWORD(((ish + 0xfff5) * 0x3)) + 0x4)))
        goto L_21e7;
    else
        goto L_2122;

L_2122:
    if ((ish == 0xb))
        goto L_2149;
    else
        goto L_212b;

L_212b:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0xe))
        goto L_21e7;
    else
        goto L_2149;

L_2149:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) >= 0xa))
        goto L_2197;
    else
        goto L_2161;

L_2161:
    if ((ish != 0xb))
        goto L_2170;
    else
        goto L_216a;

L_216a:
    t_merge_2173_0001 = 0xe;
    goto L_2173;

L_2170:
    t_merge_2173_0001 = 0xf;

L_2173:
    /* untranslated: call FCreateAiShdef(ish, 0xb, &vrgRobAip[0x12:[(t_merge_2173_0001 * 0x2)+0x1f34]]) -> callresult(int16_t) */
    goto L_21e7;

L_2197:
    i = 0x0;
    goto L_21de;

L_219f:
    /* untranslated: branch FCreateAiShdef(ish, 0x1f, &vrgRobAip[0x12:[((Random(0x6) + 0x8) * 0x2)+0x1f34]]) != 0x0 ? L_21e7 : L_21d4 */

L_21d4:

L_21da:
    i = (i + 0x1);

L_21de:
    if ((i < 0x5))
        goto L_219f;
    else
        goto L_21e7;

L_21e7:
    ish = (ish + 0x1);

L_21eb:
    if ((ish <= 0xd))
        goto L_20bf;
    else
        goto L_21f4;

L_21f4:
    /* untranslated: branch ((part[2181:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_22f5 : L_2207 */

L_2207:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_22f5;
    else
        goto L_221f;

L_221f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x6))
        goto L_22f5;
    else
        goto L_2237;

L_2237:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_22f5;
    else
        goto L_224f;

L_224f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x6))
        goto L_22f5;
    else
        goto L_2267;

L_2267:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) < 0x2))
        goto L_22f5;
    else
        goto L_227f;

L_227f:
    i = 0x0;
    goto L_22ec;

L_2287:
    /* untranslated: branch FCreateAiShdef(0xe, 0x1d, &vrgRobAip[part[74:2](vrgRobIshAip)]) != 0x0 ? L_22e8 : L_22ac */

L_22ac:
    /* untranslated: branch FCreateAiShdef(0xe, 0x6, &vrgRobAip[0x12:[((Random(0x4) + 0x10) * 0x2)+0x1f34]]) != 0x0 ? L_22f5 : L_22e2 */

L_22e2:

L_22e8:
    i = (i + 0x1);

L_22ec:
    if ((i < 0x5))
        goto L_2287;
    else
        goto L_22f5;

L_22f5:
    /* untranslated: branch ((part[2328:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_23de : L_2308 */

L_2308:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0xa))
        goto L_23de;
    else
        goto L_2320;

L_2320:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x8))
        goto L_23de;
    else
        goto L_2338;

L_2338:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x9))
        goto L_23de;
    else
        goto L_2350;

L_2350:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xe))
        goto L_23de;
    else
        goto L_2368;

L_2368:
    /* untranslated: branch FCreateAiShdef(0xf, 0x1d, &vrgRobAip[part[74:2](vrgRobIshAip)]) != 0x0 ? L_23de : L_238d */

L_238d:
    i = 0x0;
    goto L_23d5;

L_2395:
    /* untranslated: branch FCreateAiShdef(0xf, 0x6, &vrgRobAip[0x12:[((Random(0x4) + 0x14) * 0x2)+0x1f34]]) != 0x0 ? L_23de : L_23cb */

L_23cb:

L_23d1:
    i = (i + 0x1);

L_23d5:
    if ((i < 0x5))
        goto L_2395;
    else
        goto L_23de;

L_23de:
    ish = 0x2;
    goto L_251f;

L_23e6:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_251b : L_2404 */

L_2404:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xa))
        goto L_251b;
    else
        goto L_241c;

L_241c:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xa))
        goto L_251b;
    else
        goto L_2434;

L_2434:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x9))
        goto L_251b;
    else
        goto L_244c;

L_244c:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) < 0x6))
        goto L_251b;
    else
        goto L_2464;

L_2464:
    if ((ish == 0x2))
        goto L_24ae;
    else
        goto L_246d;

L_246d:
    /* untranslated: branch ((part[123:2](rgshdef[(ish - 0x1)]) >> 0x9) & 0x1) != 0x0 ? L_251b : L_2490 */

L_2490:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0xc))
        goto L_251b;
    else
        goto L_24ae;

L_24ae:
    if ((((ish + 0xfffe) & 0x1) == 0x0))
        goto L_24c5;
    else
        goto L_24bf;

L_24bf:
    t_merge_24c8_0001 = 0x4;
    goto L_24c8;

L_24c5:
    t_merge_24c8_0001 = 0x0;

L_24c8:
    shBase = t_merge_24c8_0001;
    i = 0x0;
    goto L_2512;

L_24d3:
    /* untranslated: branch FCreateAiShdef(ish, 0x1f, &vrgRobAip[0x12:[((Random(0x4) + shBase) * 0x2)+0x1f34]]) != 0x0 ? L_251b : L_2508 */

L_2508:

L_250e:
    i = (i + 0x1);

L_2512:
    if ((i < 0x5))
        goto L_24d3;
    else
        goto L_251b;

L_251b:
    ish = (ish + 0x1);

L_251f:
    if ((ish <= 0x5))
        goto L_23e6;
    else
        goto L_2528;

L_2528:
    ish = 0x6;
    goto L_2691;

L_2530:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_268d : L_254e */

L_254e:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) < 0x4))
        goto L_268d;
    else
        goto L_2566;

L_2566:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0xa))
        goto L_268d;
    else
        goto L_257e;

L_257e:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xc))
        goto L_268d;
    else
        goto L_2596;

L_2596:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0xc))
        goto L_268d;
    else
        goto L_25ae;

L_25ae:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) < 0x6))
        goto L_268d;
    else
        goto L_25c6;

L_25c6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xf))
        goto L_268d;
    else
        goto L_25de;

L_25de:
    if ((ish == 0x6))
        goto L_2628;
    else
        goto L_25e7;

L_25e7:
    /* untranslated: branch ((part[123:2](rgshdef[(ish - 0x1)]) >> 0x9) & 0x1) != 0x0 ? L_268d : L_260a */

L_260a:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0x14))
        goto L_268d;
    else
        goto L_2628;

L_2628:
    if ((ish != 0x6))
        goto L_2637;
    else
        goto L_2631;

L_2631:
    t_merge_263a_0001 = 0x1b;
    goto L_263a;

L_2637:
    t_merge_263a_0001 = 0x1f;

L_263a:
    shBase = t_merge_263a_0001;
    i = 0x0;
    goto L_2684;

L_2645:
    /* untranslated: branch FCreateAiShdef(ish, 0x9, &vrgRobAip[0x12:[((Random(0x4) + shBase) * 0x2)+0x1f34]]) != 0x0 ? L_268d : L_267a */

L_267a:

L_2680:
    i = (i + 0x1);

L_2684:
    if ((i < 0x5))
        goto L_2645;
    else
        goto L_268d;

L_268d:
    ish = (ish + 0x1);

L_2691:
    if ((ish <= 0x7))
        goto L_2530;
    else
        goto L_269a;

L_269a:
    ish = 0x9;
    goto L_277d;

L_26a2:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_2779 : L_26c0 */

L_26c0:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xe))
        goto L_2779;
    else
        goto L_26d8;

L_26d8:
    if ((ish == 0x9))
        goto L_2722;
    else
        goto L_26e1;

L_26e1:
    /* untranslated: branch ((part[123:2](rgshdef[(ish - 0x1)]) >> 0x9) & 0x1) != 0x0 ? L_2779 : L_2704 */

L_2704:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0xf))
        goto L_2779;
    else
        goto L_2722;

L_2722:
    /* untranslated: branch FCreateAiShdef(ish, 0x9, &vrgRobAip[part[72:2](vrgRobIshAip)]) != 0x0 ? L_2779 : L_2746 */

L_2746:
    if ((ish != 0x9))
        goto L_2755;
    else
        goto L_274f;

L_274f:
    t_merge_2758_0001 = 0x18;
    goto L_2758;

L_2755:
    t_merge_2758_0001 = 0x19;

L_2758:
    /* untranslated: call FCreateAiShdef(ish, 0x13, &vrgRobAip[0x12:[(t_merge_2758_0001 * 0x2)+0x1f34]]) -> callresult(int16_t) */

L_2779:
    ish = (ish + 0x1);

L_277d:
    if ((ish <= 0xa))
        goto L_26a2;
    else
        goto L_2786;

L_2786:
    if ((LOWORD(rgshdef) == 0x5))
        goto L_2888;
    else
        goto L_2790;

L_2790:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_2888 : L_27af */

L_27af:
    /* untranslated: branch part[131:2](rgshdef) != 0x0 ? L_2888 : L_27b9 */

L_27b9:
    /* untranslated: branch part[133:2](rgshdef) != 0x0 ? L_2888 : L_27c3 */

L_27c3:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) < 0x4))
        goto L_2888;
    else
        goto L_27db;

L_27db:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x5))
        goto L_2888;
    else
        goto L_27f3;

L_27f3:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_2888;
    else
        goto L_280b;

L_280b:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x6))
        goto L_2888;
    else
        goto L_2823;

L_2823:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) < 0x6))
        goto L_2888;
    else
        goto L_283b;

L_283b:
    /* untranslated: shdef = part[0:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x0);
    /* untranslated: call FCreateAiShdef(0x0, 0x5, &vrgRobAip[part[52:2](vrgRobIshAip)]) -> callresult(int16_t) */

L_2888:
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
    int32_t t_merge_30e4_0003_wide;
    int32_t t_merge_315a_0001_wide;

L_288e:
    if ((lpfl->cord <= 0x1))
        goto LTryNewTarget;
    else
        goto L_28a4;

L_28a4:
    ord = lpfl->lpplord->rgord[0x1];
    /* untranslated: branch hiword(LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(ord.pt.y, ord.pt.x))) < 0x0 ? L_290b : L_28e8 */

L_28e8:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_28f5 : L_28ed */

L_28ed:
    /* untranslated: branch loword(callresult(int32_t)) <= 0xf424 ? L_290b : L_28f5 */

L_28f5:
    if ((ord.grobj == grobjFleet))
        goto LTryNewTarget;
    else
        goto L_2905;

L_2905:

L_290b:
    if ((ord.grobj != grobjFleet))
        goto L_2924;
    else
        goto L_291e;

L_291e:
    return 0x0;

L_2924:
    if ((ord.grobj != grobjPlanet))
        goto LTryNewTarget;
    else
        goto L_2937;

L_2937:
    lppl = LpplFromId(ord.id);
    if ((LOWORD(lppl) != 0x0))
        goto L_295a;
    else
        goto L_2951;

L_2951:
    if ((HIWORD(lppl) == 0x0))
        goto L_299d;
    else
        goto L_295a;

L_295a:
    if ((lppl->iPlayer == 0xffff))
        goto L_298d;
    else
        goto L_2967;

L_2967:
    if ((lppl->iPlayer != idPlayer))
        goto L_299d;
    else
        goto L_2976;

L_2976:
    if ((lppl->fStarbase != 0x0))
        goto L_299d;
    else
        goto L_298d;

L_298d:
    if ((lppl->turn == game.turn))
        goto LTryNewTarget;
    else
        goto L_299d;

L_299d:
    return 0x0;

LTryNewTarget:
    cshWar = 0x0;
    ish = 0x2;
    goto L_29ce;

L_29b0:
    cshWar = (cshWar + lpfl->rgcsh[ish]);
    ish = (ish + 0x1);

L_29ce:
    if ((ish <= 0x5))
        goto L_29b0;
    else
        goto L_29d7;

L_29d7:
    ish = 0x6;
    goto L_29ff;

L_29df:
    cshWar = (cshWar + (lpfl->rgcsh[ish] * 0x2));
    ish = (ish + 0x1);

L_29ff:
    if ((ish <= 0x7))
        goto L_29df;
    else
        goto L_2a08;

L_2a08:
    cshBomb = (lpfl->rgcsh[0x9] + lpfl->rgcsh[0xa]);
    lpfl->fMark = 0x1;
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet != 0xffff))
        goto L_2a64;
    else
        goto L_2a4c;

L_2a4c:
    MoveToNearestPlanetOrEnemy(lpfl, 0x96);
    return 0x0;

L_2a64:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_2dc6;
    else
        goto L_2a88;

L_2a88:
    if ((lppl->fStarbase == 0x0))
        goto L_2dc6;
    else
        goto L_2a9f;

L_2a9f:
    if ((cshWar < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_2d30;
    else
        goto L_2aad;

L_2aad:
    if ((cshBomb < (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_2d30;
    else
        goto L_2abb;

L_2abb:
    /* untranslated: branch part[194:2](sel) < 0x0 ? L_2af3 : L_2ac5 */

L_2ac5:
    /* untranslated: branch part[194:2](sel) > 0x0 ? L_2ad5 : L_2aca */

L_2aca:
    /* untranslated: branch part[192:2](sel) <= 0xbb8 ? L_2af3 : L_2ad5 */

L_2ad5:
    /* untranslated: cCol = (int32_t)(words(part[194:2](sel), part[192:2](sel)) / 0xa) */
    goto L_2b6d;

L_2af3:
    /* untranslated: branch part[194:2](sel) < 0x0 ? L_2b2b : L_2afd */

L_2afd:
    /* untranslated: branch part[194:2](sel) > 0x0 ? L_2b0d : L_2b02 */

L_2b02:
    /* untranslated: branch part[192:2](sel) <= 0x7d0 ? L_2b2b : L_2b0d */

L_2b0d:
    /* untranslated: cCol = (int32_t)(words(part[194:2](sel), part[192:2](sel)) / 0xf) */
    goto L_2b6d;

L_2b2b:
    /* untranslated: branch part[194:2](sel) < 0x0 ? L_2b63 : L_2b35 */

L_2b35:
    /* untranslated: branch part[194:2](sel) > 0x0 ? L_2b45 : L_2b3a */

L_2b3a:
    /* untranslated: branch part[192:2](sel) <= 0x3e8 ? L_2b63 : L_2b45 */

L_2b45:
    /* untranslated: cCol = (int32_t)(words(part[194:2](sel), part[192:2](sel)) / 0x14) */
    goto L_2b6d;

L_2b63:
    cCol = 0x0;

L_2b6d:
    if ((HIWORD(cCol) < 0x0))
        goto TargetPotentArmada;
    else
        goto L_2b76;

L_2b76:
    if ((HIWORD(cCol) > 0x0))
        goto L_2b84;
    else
        goto L_2b7b;

L_2b7b:
    if ((LOWORD(cCol) <= 0x0))
        goto TargetPotentArmada;
    else
        goto L_2b84;

L_2b84:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, LOWORD(cCol));
    FLookupFleet(LOWORD(lpfl), 0x4972);

TargetPotentArmada:
    if ((game.fAisBand == 0x0))
        goto L_2bf3;
    else
        goto L_2bd4;

L_2bd4:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_2bfd;

L_2bf3:
    lpplTarget = 0x0;

L_2bfd:
    if ((LOWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_2c06;

L_2c06:
    if ((HIWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_2c0f;

L_2c0f:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

TargetEveryArmada:
    if ((LOWORD(lpplTarget) != 0x0))
        goto L_2c3d;
    else
        goto L_2c34;

L_2c34:
    if ((HIWORD(lpplTarget) == 0x0))
        goto L_2cd6;
    else
        goto L_2c3d;

L_2c3d:
    vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] = (vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] | 0x80);
    ord.id = lpplTarget->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplTarget->id].x;
    ord.pt.y = rgptPlan[lpplTarget->id].y;

FinishTargeting:
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_31af;
    else
        goto L_2ccd;

L_2ccd:
    return 0xffff;

L_2cd6:
    lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_2d04;
    else
        goto L_2cfb;

L_2cfb:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_31af;
    else
        goto L_2d04;

L_2d04:
    ord.id = LOWORD(lpflTarget);
    ord.grobj = grobjFleet;
    ord.pt.x = lpflTarget->pt.x;
    ord.pt.y = lpflTarget->pt.y;
    goto FinishTargeting;

L_2d30:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_31af : L_2d4f */

L_2d4f:
    if ((cshWar > ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_2d68;
    else
        goto L_2d5f;

L_2d5f:
    if ((cshWar < 0x3c))
        goto L_31af;
    else
        goto L_2d68;

L_2d68:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_2d7c;

L_2d7c:
    if ((cshWar <= LOWORD(((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x3))))
        goto L_2da3;
    else
        goto L_2d8f;

L_2d8f:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_2da3;

L_2da3:
    if ((cshWar <= 0x78))
        goto L_31af;
    else
        goto L_2dac;

L_2dac:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_2dbd;

L_2dbd:

L_2dc6:
    /* untranslated: branch cshWar < (part[1:2](vrgAiArmadaPotency) & 0xff) ? L_2de2 : L_2dd4 */

L_2dd4:
    /* untranslated: branch cshBomb >= (part[3:2](vrgAiArmadaPotency) & 0xff) ? L_2e9f : L_2de2 */

L_2de2:
    ClearAiCurrentTask(lpfl, 0x0);
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_2e7d : L_2e13 */

L_2e13:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_2e37;
    else
        goto L_2e23;

L_2e23:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_2e37;

L_2e37:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x4)))
        goto L_2e5d;
    else
        goto L_2e49;

L_2e49:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_2e5d;

L_2e5d:
    if ((cshWar <= 0x78))
        goto L_2e7d;
    else
        goto L_2e66;

L_2e66:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_2e77;

L_2e77:

L_2e7d:
    lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
    goto TargetEveryArmada;

L_2e9f:
    if ((lppl->iPlayer == 0xffff))
        goto TargetPotentArmada;
    else
        goto L_2ea9;

L_2ea9:

L_2eb2:
    if ((lppl->iPlayer != idPlayer))
        goto L_2f29;
    else
        goto L_2ec1;

L_2ec1:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto TargetPotentArmada;
    else
        goto L_2ece;

L_2ece:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_2ede;
    else
        goto L_2ed3;

L_2ed3:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x3e8))
        goto TargetPotentArmada;
    else
        goto L_2ede;

L_2ede:
    /* untranslated: call XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, (words(LOWORD(lppl->rgwtMin[0x3]),
     * signhiword(LOWORD(lppl->rgwtMin[0x3]))) / 0x5)) -> callresult(int16_t) */
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_2f29:
    if ((lppl->iPlayer == idPlayer))
        goto L_31af;
    else
        goto L_2f38;

L_2f38:
    lPopUs = lpfl->rgwtMin[0x3];
    /* untranslated: lPopEnemy = (int32_t)(words(lppl->uPopGuess, 0x0) * 0x4) */
    /* untranslated: pctDef = ((uint32_t)(words(0x0, lppl->uDefGuess) * 0x6) + 0x6) */
    pctDef = (int32_t)(((uint32_t)((pctDef * 0x3)) / 0x4));
    /* untranslated: lPopEnemy = (int32_t)((uint32_t)(lPopEnemy * 0x64) / words((0x0 - HIWORD(pctDef)), (0x64 - LOWORD(pctDef)))) */
    if ((HIWORD(lPopEnemy) < HIWORD((int32_t)((lPopUs / 0x5)))))
        goto L_306e;
    else
        goto L_3002;

L_3002:
    /* untranslated: branch HIWORD(lPopEnemy) > hiword(callresult(int32_t)) ? L_300f : L_3007 */

L_3007:
    /* untranslated: branch LOWORD(lPopEnemy) < loword(callresult(int32_t)) ? L_306e : L_300f */

L_300f:
    if ((HIWORD(lPopEnemy) > 0x0))
        goto L_303f;
    else
        goto L_3018;

L_3018:
    if ((HIWORD(lPopEnemy) < 0x0))
        goto L_3027;
    else
        goto L_301d;

L_301d:
    if ((LOWORD(lPopEnemy) >= 0xc8))
        goto L_303f;
    else
        goto L_3027;

L_3027:
    if ((HIWORD(lPopUs) > 0x0))
        goto L_306e;
    else
        goto L_3030;

L_3030:
    if ((HIWORD(lPopUs) < 0x0))
        goto L_303f;
    else
        goto L_3035;

L_3035:
    if ((LOWORD(lPopUs) > 0x15e))
        goto L_306e;
    else
        goto L_303f;

L_303f:
    if ((HIWORD(lPopEnemy) > 0x0))
        goto L_31af;
    else
        goto L_3048;

L_3048:
    if ((HIWORD(lPopEnemy) < 0x0))
        goto L_3056;
    else
        goto L_304d;

L_304d:
    if ((LOWORD(lPopEnemy) >= 0xa))
        goto L_31af;
    else
        goto L_3056;

L_3056:
    if ((HIWORD(lPopUs) < 0x0))
        goto L_31af;
    else
        goto L_305f;

L_305f:
    if ((HIWORD(lPopUs) > 0x0))
        goto L_306e;
    else
        goto L_3064;

L_3064:
    if ((LOWORD(lPopUs) <= 0x96))
        goto L_31af;
    else
        goto L_306e;

L_306e:
    cXfer = (int32_t)(((uint32_t)((lPopEnemy * 0x5)) / 0x4));
    if ((HIWORD(cXfer) < HIWORD((int32_t)((lpfl->rgwtMin[0x3] / 0x2)))))
        goto L_30cc;
    else
        goto L_30b6;

L_30b6:
    /* untranslated: branch HIWORD(cXfer) > hiword(callresult(int32_t)) ? L_30c3 : L_30bb */

L_30bb:
    /* untranslated: branch LOWORD(cXfer) <= loword(callresult(int32_t)) ? L_30cc : L_30c3 */

L_30c3:
    t_merge_30e4_0003_wide = cXfer;
    goto L_30e4;

L_30cc:
    /* untranslated: t_merge_30e4_0003_wide = callresult(int32_t) */

L_30e4:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > HIWORD(t_merge_30e4_0003_wide)))
        goto L_310c;
    else
        goto L_30f0;

L_30f0:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < HIWORD(t_merge_30e4_0003_wide)))
        goto L_30fe;
    else
        goto L_30f5;

L_30f5:
    if ((LOWORD(lpfl->rgwtMin[0x3]) >= LOWORD(t_merge_30e4_0003_wide)))
        goto L_310c;
    else
        goto L_30fe;

L_30fe:
    t_merge_315a_0001_wide = lpfl->rgwtMin[0x3];
    goto L_315a;

L_310c:
    if ((HIWORD(cXfer) < HIWORD((int32_t)((lpfl->rgwtMin[0x3] / 0x2)))))
        goto L_3142;
    else
        goto L_312c;

L_312c:
    /* untranslated: branch HIWORD(cXfer) > hiword(callresult(int32_t)) ? L_3139 : L_3131 */

L_3131:
    /* untranslated: branch LOWORD(cXfer) <= loword(callresult(int32_t)) ? L_3142 : L_3139 */

L_3139:
    t_merge_315a_0001_wide = cXfer;
    goto L_315a;

L_3142:
    /* untranslated: t_merge_315a_0001_wide = callresult(int32_t) */

L_315a:
    cXfer = t_merge_315a_0001_wide;
    if ((HIWORD(cXfer) < 0x0))
        goto L_3182;
    else
        goto L_3169;

L_3169:
    if ((HIWORD(cXfer) > 0x0))
        goto L_3178;
    else
        goto L_316e;

L_316e:
    if ((LOWORD(cXfer) <= 0x7530))
        goto L_3182;
    else
        goto L_3178;

L_3178:
    cXfer = 0x7530;

L_3182:
    XferAiTroopers(LOWORD(lpfl), lppl->id, LOWORD(cXfer));
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_31af:
    return 0x0;
}

int16_t FPotentRobWarFleet(FLEET *lpfl, int16_t iPotency) {
    int16_t ish;
    int16_t cEquiv;

L_31bc:
    cEquiv = 0x0;
    ish = 0x2;
    goto L_31f0;

L_31d2:
    cEquiv = (cEquiv + lpfl->rgcsh[ish]);
    ish = (ish + 0x1);

L_31f0:
    if ((ish <= 0x5))
        goto L_31d2;
    else
        goto L_31f9;

L_31f9:
    ish = 0x6;
    goto L_3221;

L_3201:
    cEquiv = (cEquiv + (lpfl->rgcsh[ish] * 0x2));
    ish = (ish + 0x1);

L_3221:
    if ((ish <= 0x7))
        goto L_3201;
    else
        goto L_322a;

L_322a:
    if ((iPotency >= 0x2))
        goto L_323c;
    else
        goto L_3233;

L_3233:
    return 0x1;

L_323c:
    if ((cEquiv < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_3250;
    else
        goto L_324a;

L_324a:
    return 0x1;

L_3250:
    return 0x0;
}

int16_t FEnumCalcEnemyFleets(FLEET *lpflSrc, FLEET *lpflTest) {
L_325c:
    if ((lpflTest->iPlayer == idPlayer))
        goto L_327a;
    else
        goto L_3274;

L_3274:
    return 0x1;

L_327a:
    return 0x0;
}

int16_t FEnumCalcArmadaDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

L_3286:
    if ((LOWORD(lpplSrc) != LOWORD(lpplTest)))
        goto L_32ab;
    else
        goto L_329d;

L_329d:
    if ((HIWORD(lpplSrc) != HIWORD(lpplTest)))
        goto L_32ab;
    else
        goto L_32a5;

L_32a5:
    return 0x0;

L_32ab:
    id = lpplTest->id;
    b = vlpbAiPlanet[((id * 0x10) + 0xa)];
    if ((b == 0x0))
        goto L_33f9;
    else
        goto L_32e2;

L_32e2:
    /* untranslated: l2 = LDistance2(words(rgptPlan[lpplSrc->id].y, rgptPlan[lpplSrc->id].x), words(rgptPlan[lpplTest->id].y, rgptPlan[lpplTest->id].x)) */
    if ((HIWORD(l2) > 0x0))
        goto L_3333;
    else
        goto L_331d;

L_331d:
    if ((HIWORD(l2) < 0x0))
        goto L_332c;
    else
        goto L_3322;

L_3322:
    if ((LOWORD(l2) >= 0x9c4))
        goto L_3333;
    else
        goto L_332c;

L_332c:
    b = (b + 0x7);
    goto L_33cb;

L_3333:
    if ((HIWORD(l2) > 0x0))
        goto L_3352;
    else
        goto L_333c;

L_333c:
    if ((HIWORD(l2) < 0x0))
        goto L_334b;
    else
        goto L_3341;

L_3341:
    if ((LOWORD(l2) >= 0x2710))
        goto L_3352;
    else
        goto L_334b;

L_334b:
    b = (b + 0x5);
    goto L_33cb;

L_3352:
    if ((HIWORD(l2) > 0x0))
        goto L_3371;
    else
        goto L_335b;

L_335b:
    if ((HIWORD(l2) < 0x0))
        goto L_336a;
    else
        goto L_3360;

L_3360:
    if ((LOWORD(l2) >= 0x57e4))
        goto L_3371;
    else
        goto L_336a;

L_336a:
    b = (b + 0x4);
    goto L_33cb;

L_3371:
    if ((HIWORD(l2) > 0x0))
        goto L_3390;
    else
        goto L_337a;

L_337a:
    if ((HIWORD(l2) < 0x0))
        goto L_3389;
    else
        goto L_337f;

L_337f:
    if ((LOWORD(l2) >= 0x9c40))
        goto L_3390;
    else
        goto L_3389;

L_3389:
    b = (b + 0x3);
    goto L_33cb;

L_3390:
    if ((HIWORD(l2) > 0x1))
        goto L_33af;
    else
        goto L_3399;

L_3399:
    if ((HIWORD(l2) < 0x1))
        goto L_33a8;
    else
        goto L_339e;

L_339e:
    if ((LOWORD(l2) >= 0x5f90))
        goto L_33af;
    else
        goto L_33a8;

L_33a8:
    b = (b + 0x2);
    goto L_33cb;

L_33af:
    if ((HIWORD(l2) > 0x3))
        goto L_33cb;
    else
        goto L_33b8;

L_33b8:
    if ((HIWORD(l2) < 0x3))
        goto L_33c7;
    else
        goto L_33bd;

L_33bd:
    if ((LOWORD(l2) >= 0xd090))
        goto L_33cb;
    else
        goto L_33c7;

L_33c7:
    b = (b + 0x1);

L_33cb:
    if (((b & 0x80) == 0x0))
        goto L_33f0;
    else
        goto L_33dc;

L_33dc:
    if ((Random(0x4) != 0x0))
        goto L_33f9;
    else
        goto L_33f0;

L_33f0:
    return b;

L_33f9:
    return 0x0;
}

int16_t FEnumCalcArmadaHumanDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;
    int32_t l2;

L_3406:
    if ((LOWORD(lpplSrc) != LOWORD(lpplTest)))
        goto L_342b;
    else
        goto L_341d;

L_341d:
    if ((HIWORD(lpplSrc) != HIWORD(lpplTest)))
        goto L_342b;
    else
        goto L_3425;

L_3425:
    return 0x0;

L_342b:
    id = lpplTest->id;
    /* untranslated: branch ((part[6:2](rgplr[lpplTest->iPlayer]) >> 0x9) & 0x1) == 0x0 ? L_345c : L_3456 */

L_3456:
    return 0x0;

L_345c:
    b = vlpbAiPlanet[((id * 0x10) + 0xa)];
    if ((b == 0x0))
        goto L_35a1;
    else
        goto L_348a;

L_348a:
    /* untranslated: l2 = LDistance2(words(rgptPlan[lpplSrc->id].y, rgptPlan[lpplSrc->id].x), words(rgptPlan[lpplTest->id].y, rgptPlan[lpplTest->id].x)) */
    if ((HIWORD(l2) > 0x0))
        goto L_34db;
    else
        goto L_34c5;

L_34c5:
    if ((HIWORD(l2) < 0x0))
        goto L_34d4;
    else
        goto L_34ca;

L_34ca:
    if ((LOWORD(l2) >= 0x9c4))
        goto L_34db;
    else
        goto L_34d4;

L_34d4:
    b = (b + 0x7);
    goto L_3573;

L_34db:
    if ((HIWORD(l2) > 0x0))
        goto L_34fa;
    else
        goto L_34e4;

L_34e4:
    if ((HIWORD(l2) < 0x0))
        goto L_34f3;
    else
        goto L_34e9;

L_34e9:
    if ((LOWORD(l2) >= 0x2710))
        goto L_34fa;
    else
        goto L_34f3;

L_34f3:
    b = (b + 0x5);
    goto L_3573;

L_34fa:
    if ((HIWORD(l2) > 0x0))
        goto L_3519;
    else
        goto L_3503;

L_3503:
    if ((HIWORD(l2) < 0x0))
        goto L_3512;
    else
        goto L_3508;

L_3508:
    if ((LOWORD(l2) >= 0x57e4))
        goto L_3519;
    else
        goto L_3512;

L_3512:
    b = (b + 0x4);
    goto L_3573;

L_3519:
    if ((HIWORD(l2) > 0x0))
        goto L_3538;
    else
        goto L_3522;

L_3522:
    if ((HIWORD(l2) < 0x0))
        goto L_3531;
    else
        goto L_3527;

L_3527:
    if ((LOWORD(l2) >= 0x9c40))
        goto L_3538;
    else
        goto L_3531;

L_3531:
    b = (b + 0x3);
    goto L_3573;

L_3538:
    if ((HIWORD(l2) > 0x1))
        goto L_3557;
    else
        goto L_3541;

L_3541:
    if ((HIWORD(l2) < 0x1))
        goto L_3550;
    else
        goto L_3546;

L_3546:
    if ((LOWORD(l2) >= 0x5f90))
        goto L_3557;
    else
        goto L_3550;

L_3550:
    b = (b + 0x2);
    goto L_3573;

L_3557:
    if ((HIWORD(l2) > 0x3))
        goto L_3573;
    else
        goto L_3560;

L_3560:
    if ((HIWORD(l2) < 0x3))
        goto L_356f;
    else
        goto L_3565;

L_3565:
    if ((LOWORD(l2) >= 0xd090))
        goto L_3573;
    else
        goto L_356f;

L_356f:
    b = (b + 0x1);

L_3573:
    if (((b & 0x80) == 0x0))
        goto L_3598;
    else
        goto L_3584;

L_3584:
    if ((Random(0x4) != 0x0))
        goto L_35a1;
    else
        goto L_3598;

L_3598:
    return b;

L_35a1:
    return 0x0;
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
    uint16_t t_merge_37de_0001;
    uint16_t t_merge_381c_0001;
    uint16_t t_merge_3c51_0001;
    uint16_t t_merge_3e7a_0001;
    uint16_t t_merge_3f26_0001;
    PLANET  *t_merge_4910_0001_wide;
    PLANET  *t_merge_54df_0001_wide;

L_3670:
    iPlanet = rgplr[idPlayer].idPlanetHome;
    cplBadGuy = 0x0;
    cplNegative = 0x0;
    cplanCol = 0x0;
    cplMiners = 0x0;
    iroCur = IroEnsureAi(vrgAiTurinDroneResOrder, 0x1f, &(ishdefSBLatest), 0xf);
    /* untranslated: branch ((part[2034:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_36e3 : L_36d7 */

L_36d7:
    MergeAllShdefs(0xe0f0);

L_36e3:
    /* untranslated: branch ((part[1887:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_3702 : L_36f6 */

L_36f6:
    MergeAllShdefs(0x1000);

L_3702:
    /* untranslated: branch ((part[1593:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_3721 : L_3715 */

L_3715:
    MergeAllShdefs(0xc00);

L_3721:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_3740 : L_3734 */

L_3734:
    MergeAllShdefs(0xc);

L_3740:
    j = 0x3;
    if ((game.turn <= 0x82))
        goto L_3760;
    else
        goto L_3750;

L_3750:
    /* untranslated: j = (j + (words((game.turn + 0xff88), 0x0) / 0x14)) */

L_3760:
    if ((j <= 0x32))
        goto L_376e;
    else
        goto L_3769;

L_3769:
    j = 0x32;

L_376e:
    LOWORD(vrgAiArmadaPotency) = LOBYTE(j);
    /* untranslated: part[1:2](vrgAiArmadaPotency) = lobyte((sext16to32((j & 0xff)) / 0x2)) */
    j = 0x6;
    if ((game.turn <= 0x73))
        goto L_37a2;
    else
        goto L_3792;

L_3792:
    /* untranslated: j = (j + (words((game.turn + 0xff9c), 0x0) / 0x16)) */

L_37a2:
    if ((j <= 0xc))
        goto L_37b0;
    else
        goto L_37ab;

L_37ab:
    j = 0xc;

L_37b0:
    HIWORD(vrgAiArmadaPotency) = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 0x2) + 0xffff)))
        goto L_37d2;
    else
        goto L_37cc;

L_37cc:
    t_merge_37de_0001 = 0x3;
    goto L_37de;

L_37d2:
    t_merge_37de_0001 = (((uint32_t)(j) / 0x2) + 0xffff);

L_37de:
    /* untranslated: part[3:2](vrgAiArmadaPotency) = lobyte(t_merge_37de_0001) */
    memset(rgRecycleShdef, 0x0, 0x10);
    if ((game.turn >= 0x78))
        goto L_3808;
    else
        goto L_37ff;

L_37ff:
    cRecyclePeriod = 0x32;
    goto L_3820;

L_3808:
    if ((game.turn >= 0xc8))
        goto L_3819;
    else
        goto L_3813;

L_3813:
    t_merge_381c_0001 = 0x46;
    goto L_381c;

L_3819:
    t_merge_381c_0001 = 0x64;

L_381c:
    cRecyclePeriod = t_merge_381c_0001;

L_3820:
    /* untranslated: call CheckAiShdefStatus(0x6, 0x7, cRecyclePeriod, &iLatestCruiser, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: cExistCargo = CheckAiShdefStatus(0x8, 0x9, cRecyclePeriod, &iLatestCargo, &part[0:1](rgRecycleShdef)) */
    /* untranslated: call CheckAiShdefStatus(0xd, 0xe, cRecyclePeriod, &iLatestBomber, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x4, 0x5, cRecyclePeriod, &iLatestBattle, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0xc, 0xc, cRecyclePeriod, &iLatestLayer, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0xf, 0xf, cRecyclePeriod, &iLatestTroop, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x7))
        goto L_3906;
    else
        goto L_38e6;

L_38e6:
    /* untranslated: call CheckAiShdefStatus(0x2, 0x3, cRecyclePeriod, &iLatestMiner, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    goto L_390c;

L_3906:
    iLatestMiner = 0xffff;

L_390c:
    /* untranslated: call CheckAiShdefStatus(0xa, 0xb, cRecyclePeriod, &iLatestDestroyer, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    if ((game.turn <= 0x3c))
        goto L_393e;
    else
        goto L_3932;

L_3932:
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleShdef)) -> callresult(void) */

L_393e:
    EnsureTurinDroneShdefs(iroCur);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_39bd;

L_397b:
    if ((lppl->iPlayer != 0xffff))
        goto L_39b9;
    else
        goto L_3988;

L_3988:
    if ((lppl->det < 0x3))
        goto L_39b9;
    else
        goto L_399a;

L_399a:
    if ((PctPlanetOptValue(lppl, idPlayer) <= 0x0))
        goto L_39b9;
    else
        goto L_39b4;

L_39b4:
    cplanCol = (cplanCol + 0x1);

L_39b9:
    lppl = (lppl + 0x1);

L_39bd:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_397b;
    else
        goto L_39cd;

L_39cd:
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_4873;

L_39fe:
    vlpbAiPlanet[((lppl->id * 0x10) + 0x9)] = 0x1;
    if ((lppl->iPlayer != 0xffff))
        goto L_3ae8;
    else
        goto L_3a2c;

L_3a2c:
    if ((lppl->det < 0x3))
        goto L_3ae8;
    else
        goto L_3a3e;

L_3a3e:
    b = 0x0;
    i = 0x0;
    goto L_3aa2;

L_3a4a:
    if ((lppl->rgMinConc[i] <= 0x42))
        goto L_3a72;
    else
        goto L_3a6a;

L_3a6a:
    bT = 0x4b;
    goto L_3a94;

L_3a72:
    /* untranslated: bT = lobyte((words(lppl->rgMinConc[i], signhiword(lppl->rgMinConc[i])) / 0x2)) */

L_3a94:
    b = (b + LOBYTE(bT));
    i = (i + 0x1);

L_3aa2:
    if ((i < 0x3))
        goto L_3a4a;
    else
        goto L_3aab;

L_3aab:
    if (((b & 0x80) == 0x0))
        goto L_3ac0;
    else
        goto L_3abc;

L_3abc:
    b = 0x7f;

L_3ac0:
    vlpbAiPlanet[((lppl->id * 0x10) + 0x1)] = b;
    cplMiners = (cplMiners + 0x1);

L_3ae8:
    if ((lppl->iPlayer == idPlayer))
        goto L_3b88;
    else
        goto L_3af7;

L_3af7:
    if ((lppl->iPlayer == 0xffff))
        goto L_3b88;
    else
        goto L_3b04;

L_3b04:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] = LOBYTE(((lppl->fStarbase & 0xff) + 0x1));
    pct = PctPlanetOptValue(lppl, idPlayer);
    if ((pct <= 0x0))
        goto L_486f;
    else
        goto L_3b59;

L_3b59:
    vlpbAiPlanet[((lppl->id * 0x10) + 0x3)] = LOBYTE(pct);
    cplBadGuy = (cplBadGuy + 0x1);

L_3b88:
    if ((lppl->iPlayer != idPlayer))
        goto L_486f;
    else
        goto L_3b94;

L_3b94:

L_3b9d:
    if ((PctPlanetDesirability(lppl, idPlayer) >= 0x0))
        goto L_3be3;
    else
        goto L_3bb7;

L_3bb7:
    cplNegative = (cplNegative + 0x1);
    vlpbAiPlanet[((lppl->id * 0x10) + 0x2)] = 0x1;
    goto L_486f;

L_3be3:
    if ((lppl->fStarbase == 0x0))
        goto L_3c17;
    else
        goto L_3bfa;

L_3bfa:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_3cfd;
    else
        goto L_3c07;

L_3c07:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_3c17;
    else
        goto L_3c0c;

L_3c0c:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0xc8))
        goto L_3cfd;
    else
        goto L_3c17;

L_3c17:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_3c4d;
    else
        goto L_3c36;

L_3c36:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_3c46;
    else
        goto L_3c3b;

L_3c3b:
    if ((LOWORD(lppl->rgwtMin[0x3]) >= 0xc8))
        goto L_3c4d;
    else
        goto L_3c46;

L_3c46:
    t_merge_3c51_0001 = 0x1;
    goto L_3c51;

L_3c4d:
    t_merge_3c51_0001 = 0x0;

L_3c51:
    /* untranslated: part[176:2](sel) = ((part[176:2](sel) & 0xffff) | loword((int32_t)(words((t_merge_3c51_0001 & 0x1), (0x0 & 0x0)) << 0x17))) */
    /* untranslated: part[178:2](sel) = ((part[178:2](sel) & 0xff7f) | hiword((int32_t)(words((ss:[bp-0xb0] & 0x1), (ss:[bp-0xae] & 0x0)) << 0x17))) */
    /* untranslated: branch (loword((uint32_t)(words(part[176:2](sel), part[178:2](sel)) >> 0x17)) & 0x1) != (loword((uint32_t)(words(*(lppl+0x18),
     * *(lppl+0x1a)) >> 0x17)) & 0x1) ? L_3cea : L_3ce3 */

L_3ce3:
    /* untranslated: branch ss:[bp-0xb2] == (hiword(callresult(uint32_t)) & 0x0) ? L_486f : L_3cea */

L_3cea:
    FLookupPlanet(0xffff, 0x49ee);

L_3cfd:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    InitProduction(rgprod);
    fWrite = 0x0;
    b = 0x0;
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_3d59;

L_3d43:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_3d59:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_3dc9;
    else
        goto L_3d6c;

L_3d6c:
    if ((lpprod->grobj != 0x2))
        goto L_3d43;
    else
        goto L_3d8d;

L_3d8d:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_3d43 : L_3d95 */

L_3d95:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) > 0x0))
        goto L_3d43;
    else
        goto L_3db6;

L_3db6:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_3dc9 : L_3dbb */

L_3dbb:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x10 ? L_3dc9 : L_3dc0 */

L_3dc0:

L_3dc9:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_3deb;
    else
        goto L_3ddc;

L_3ddc:
    FinishProduction(0x0);
    goto L_486f;

L_3deb:
    if ((game.turn != 0x0))
        goto L_3e3d;
    else
        goto L_3df5;

L_3df5:
    i = game.cPlanMax;
    goto L_3e31;

L_3dfe:
    AddItemToQueue(0x0, 0x1, grobjFleet, 0x1);
    if ((i <= 0xbe))
        goto L_3e27;
    else
        goto L_3e20;

L_3e20:
    i = (i - 0x64);
    goto L_3e2b;

L_3e27:
    i = (i - 0x1e);

L_3e2b:
    fWrite = 0x1;

L_3e31:
    if ((i <= 0x0))
        goto L_3ede;
    else
        goto L_3e37;

L_3e37:

L_3e3d:
    /* untranslated: branch ((part[123:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_3ede : L_3e50 */

L_3e50:
    if ((LOWORD(rgshdef) != 0x5))
        goto L_3ede;
    else
        goto L_3e5a;

L_3e5a:
    if ((((uint32_t)(game.cPlanMax) / 0x4) >= 0x20))
        goto L_3e77;
    else
        goto L_3e6b;

L_3e6b:
    t_merge_3e7a_0001 = ((uint32_t)(game.cPlanMax) / 0x4);
    goto L_3e7a;

L_3e77:
    t_merge_3e7a_0001 = 0x20;

L_3e7a:
    /* untranslated: branch part[133:2](rgshdef) > 0x0 ? L_3ede : L_3e86 */

L_3e86:
    /* untranslated: branch part[133:2](rgshdef) < 0x0 ? L_3e94 : L_3e8b */

L_3e8b:
    /* untranslated: branch part[131:2](rgshdef) >= t_merge_3e7a_0001 ? L_3ede : L_3e94 */

L_3e94:
    /* untranslated: branch part[133:2](rgshdef) < hiword((uint32_t)(words(part[129:2](rgshdef), part[127:2](rgshdef)) / 0xa)) ? L_3ede : L_3eb2 */

L_3eb2:
    /* untranslated: branch part[133:2](rgshdef) > hiword(callresult(uint32_t)) ? L_3ec0 : L_3eb7 */

L_3eb7:
    /* untranslated: branch part[131:2](rgshdef) <= loword(callresult(uint32_t)) ? L_3ede : L_3ec0 */

L_3ec0:
    AddItemToQueue(0x0, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_3ede:
    if ((((uint32_t)(rgplr[idPlayer].cPlanet) / 0xa) <= (vlpbAiData[0x2] * 0x2)))
        goto L_3f1c;
    else
        goto L_3f04;

L_3f04:
    t_merge_3f26_0001 = ((uint32_t)(rgplr[idPlayer].cPlanet) / 0xa);
    goto L_3f26;

L_3f1c:
    t_merge_3f26_0001 = (vlpbAiData[0x2] * 0x2);

L_3f26:
    cFr = t_merge_3f26_0001;
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_3fa0;
    else
        goto L_3f42;

L_3f42:
    if ((iLatestCargo == 0xffff))
        goto L_3fa0;
    else
        goto L_3f4c;

L_3f4c:
    if ((cExistCargo < cFr))
        goto L_3f81;
    else
        goto L_3f58;

L_3f58:
    /* untranslated: branch cExistCargo >= (words(loword((0xa * cFr)), signhiword(loword((0xa * cFr)))) / 0x7) ? L_3fa0 : L_3f6d */

L_3f6d:
    if ((Random(0x4) != 0x0))
        goto L_3fa0;
    else
        goto L_3f81;

L_3f81:
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_3fa0:
    if ((cplanCol != 0x0))
        goto L_3fb4;
    else
        goto L_3faa;

L_3faa:
    if ((cplBadGuy == 0x0))
        goto L_4033;
    else
        goto L_3fb4;

L_3fb4:
    /* untranslated: branch part[280:2](rgshdef) > 0x0 ? L_4033 : L_3fbe */

L_3fbe:
    /* untranslated: branch part[280:2](rgshdef) < 0x0 ? L_3fcd : L_3fc3 */

L_3fc3:
    /* untranslated: branch part[278:2](rgshdef) >= 0x2 ? L_4033 : L_3fcd */

L_3fcd:
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    AddItemToQueue(0x1, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_4033:
    l = (uint32_t)((lppl->rgwtMin[0x3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    cRes = CResourcesAtPlanet(lppl, idPlayer);
    /* untranslated: branch ((part[1887:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_418b : L_4083 */

L_4083:
    if ((Random(0x3) != 0x0))
        goto L_418b;
    else
        goto L_4097;

L_4097:
    id = lppl->id;
    cFr = 0x0;
    ifl = 0x0;
    goto L_40b3;

L_40af:
    ifl = (ifl + 0x1);

L_40b3:
    if ((ifl >= cFleet))
        goto L_412b;
    else
        goto L_40be;

L_40be:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_40ee;
    else
        goto L_40e6;

L_40e6:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_412b;
    else
        goto L_40ee;

L_40ee:
    if ((lpfl->idPlanet != id))
        goto L_40af;
    else
        goto L_40fe;

L_40fe:
    if ((lpfl->rgcsh[0xc] <= 0x0))
        goto L_40af;
    else
        goto L_410b;

L_410b:
    if ((lpfl->iPlayer != idPlayer))
        goto L_40af;
    else
        goto L_411a;

L_411a:
    cFr = lpfl->rgcsh[0xc];
    goto L_412b;

L_412b:
    if ((cFr < 0xa))
        goto L_4153;
    else
        goto L_4135;

L_4135:
    if ((cFr >= 0x11))
        goto L_418b;
    else
        goto L_413f;

L_413f:
    if ((Random(0x8) != 0x0))
        goto L_418b;
    else
        goto L_4153;

L_4153:
    /* untranslated: branch Random(part[1:0](mpicolgrbitBU[cFr])) != 0x0 ? L_418b : L_416d */

L_416d:
    AddItemToQueue(0xc, 0x3, grobjFleet, 0x1);
    fWrite = 0x1;

L_418b:
    if ((iLatestBomber == 0xffff))
        goto L_4274;
    else
        goto L_4195;

L_4195:
    id = lppl->id;
    ifl = 0x0;
    goto L_41ab;

L_41a7:
    ifl = (ifl + 0x1);

L_41ab:
    if ((ifl >= cFleet))
        goto L_4274;
    else
        goto L_41b6;

L_41b6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_41e6;
    else
        goto L_41de;

L_41de:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4274;
    else
        goto L_41e6;

L_41e6:
    if ((lpfl->idPlanet != id))
        goto L_41a7;
    else
        goto L_41f6;

L_41f6:
    if ((lpfl->iPlayer != idPlayer))
        goto L_41a7;
    else
        goto L_4205;

L_4205:
    if ((FPotentRobWarFleet(lpfl, 0x2) == 0x0))
        goto L_41a7;
    else
        goto L_421f;

L_421f:
    if ((iLatestBomber == 0xffff))
        goto L_4274;
    else
        goto L_4229;

L_4229:
    if (((lpfl->rgcsh[0xd] + lpfl->rgcsh[0xe]) < (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_4274;
    else
        goto L_4249;

L_4249:

L_424f:
    AddItemToQueue(iLatestBomber, 0x4, grobjFleet, 0x1);
    fWrite = 0x1;
    goto FinishProd;

L_4274:
    if ((iLatestBattle == 0xffff))
        goto L_43ee;
    else
        goto L_427e;

L_427e:
    /* untranslated: branch part[133:2](rgshdef[iLatestBattle]) > 0x0 ? L_43ee : L_42b2 */

L_42b2:
    /* untranslated: branch part[133:2](rgshdef[iLatestBattle]) < ss:[bp-0xae] ? L_42c0 : L_42b7 */

L_42b7:
    /* untranslated: branch part[131:2](rgshdef[iLatestBattle]) >= ss:[bp-0xb0] ? L_43ee : L_42c0 */

L_42c0:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_4337;

L_42ec:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_4333;
    else
        goto L_4323;

L_4323:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_4328;

L_4328:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_432d;

L_432d:

L_4333:
    i = (i + 0x1);

L_4337:
    if ((i < 0x4))
        goto L_42ec;
    else
        goto L_4340;

L_4340:
    i = 0x0;
    goto L_43e5;

L_4348:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestBattle]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_43b9;

L_4370:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_43b5;
    else
        goto L_43a5;

L_43a5:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_43aa;

L_43aa:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_43af;

L_43af:

L_43b5:
    j = (j + 0x1);

L_43b9:
    if ((j < 0x4))
        goto L_4370;
    else
        goto L_43c2;

L_43c2:
    fWrite = 0x1;
    AddItemToQueue(iLatestBattle, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_43e5:
    if ((i < 0x5))
        goto L_4348;
    else
        goto L_43ee;

L_43ee:
    if ((iLatestCruiser == 0xffff))
        goto L_4564;
    else
        goto L_43f7;

L_43f7:
    /* untranslated: branch part[133:2](rgshdef[iLatestCruiser]) > 0x0 ? L_4564 : L_442a */

L_442a:
    /* untranslated: branch part[133:2](rgshdef[iLatestCruiser]) < ss:[bp-0xae] ? L_4438 : L_442f */

L_442f:
    /* untranslated: branch part[131:2](rgshdef[iLatestCruiser]) >= ss:[bp-0xb0] ? L_4564 : L_4438 */

L_4438:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_44af;

L_4464:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_44ab;
    else
        goto L_449b;

L_449b:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_44a0;

L_44a0:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_44a5;

L_44a5:

L_44ab:
    i = (i + 0x1);

L_44af:
    if ((i < 0x4))
        goto L_4464;
    else
        goto L_44b8;

L_44b8:
    i = 0x0;
    goto L_455b;

L_44c0:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestCruiser]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_4530;

L_44e7:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_452c;
    else
        goto L_451c;

L_451c:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_4521;

L_4521:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_4526;

L_4526:

L_452c:
    j = (j + 0x1);

L_4530:
    if ((j < 0x4))
        goto L_44e7;
    else
        goto L_4539;

L_4539:
    fWrite = 0x1;
    AddItemToQueue(iLatestCruiser, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_455b:
    if ((i < 0x5))
        goto L_44c0;
    else
        goto L_4564;

L_4564:
    if ((iLatestDestroyer == 0xffff))
        goto L_46da;
    else
        goto L_456d;

L_456d:
    /* untranslated: branch part[133:2](rgshdef[iLatestDestroyer]) > 0x0 ? L_46da : L_45a0 */

L_45a0:
    /* untranslated: branch part[133:2](rgshdef[iLatestDestroyer]) < ss:[bp-0xae] ? L_45ae : L_45a5 */

L_45a5:
    /* untranslated: branch part[131:2](rgshdef[iLatestDestroyer]) >= ss:[bp-0xb0] ? L_46da : L_45ae */

L_45ae:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_4625;

L_45da:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_4621;
    else
        goto L_4611;

L_4611:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_4616;

L_4616:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_461b;

L_461b:

L_4621:
    i = (i + 0x1);

L_4625:
    if ((i < 0x4))
        goto L_45da;
    else
        goto L_462e;

L_462e:
    i = 0x0;
    goto L_46d1;

L_4636:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestDestroyer]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_46a6;

L_465d:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_46a2;
    else
        goto L_4692;

L_4692:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_4697;

L_4697:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_469c;

L_469c:

L_46a2:
    j = (j + 0x1);

L_46a6:
    if ((j < 0x4))
        goto L_465d;
    else
        goto L_46af;

L_46af:
    fWrite = 0x1;
    AddItemToQueue(iLatestDestroyer, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_46d1:
    if ((i < 0x5))
        goto L_4636;
    else
        goto L_46da;

L_46da:
    if ((iLatestTroop == 0xffff))
        goto FinishProd;
    else
        goto L_46e3;

L_46e3:
    /* untranslated: branch ((part[2328:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_4863 : L_46f6 */

L_46f6:
    /* untranslated: branch part[133:2](rgshdef[iLatestTroop]) > 0x0 ? L_4863 : L_4729 */

L_4729:
    /* untranslated: branch part[133:2](rgshdef[iLatestTroop]) < ss:[bp-0xae] ? L_4737 : L_472e */

L_472e:
    /* untranslated: branch part[131:2](rgshdef[iLatestTroop]) >= ss:[bp-0xb0] ? L_4863 : L_4737 */

L_4737:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_47ae;

L_4763:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_47aa;
    else
        goto L_479a;

L_479a:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_479f;

L_479f:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_47a4;

L_47a4:

L_47aa:
    i = (i + 0x1);

L_47ae:
    if ((i < 0x4))
        goto L_4763;
    else
        goto L_47b7;

L_47b7:
    i = 0x0;
    goto L_485a;

L_47bf:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestTroop]), &(LOWORD(rgCosts)));
    j = 0x0;
    goto L_482f;

L_47e6:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_482b;
    else
        goto L_481b;

L_481b:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_4820;

L_4820:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto FinishProd;
    else
        goto L_4825;

L_4825:

L_482b:
    j = (j + 0x1);

L_482f:
    if ((j < 0x4))
        goto L_47e6;
    else
        goto L_4838;

L_4838:
    fWrite = 0x1;
    AddItemToQueue(iLatestTroop, 0x1, grobjFleet, 0x1);
    i = (i + 0x1);

L_485a:
    if ((i < 0x5))
        goto L_47bf;
    else
        goto FinishProd;

FinishProd:
    FinishProduction(fWrite);

L_486f:
    lppl = (lppl + 0x1);

L_4873:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_39fe;
    else
        goto L_4883;

L_4883:
    ipl = 0x0;
    goto L_4891;

L_488c:
    ipl = (ipl + 0x1);

L_4891:
    if ((ipl >= vclpplAi))
        goto L_48eb;
    else
        goto L_489d;

L_489d:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_48ce;
    else
        goto L_48c6;

L_48c6:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_48eb;
    else
        goto L_48ce;

L_48ce:
    if ((lppl->fStarbase != 0x0))
        goto L_48eb;
    else
        goto L_48e2;

L_48e2:

L_48eb:
    if ((LOWORD(lppl) != LOWORD(lpplMac)))
        goto L_490a;
    else
        goto L_48fb;

L_48fb:
    if ((HIWORD(lppl) != HIWORD(lpplMac)))
        goto L_490a;
    else
        goto L_4903;

L_4903:
    t_merge_4910_0001_wide = 0x0;
    goto L_4910;

L_490a:
    t_merge_4910_0001_wide = lppl;

L_4910:
    lpplHome = t_merge_4910_0001_wide;
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_4936;

L_4932:
    ifl = (ifl + 0x1);

L_4936:
    if ((ifl >= cFleet))
        goto L_4ea2;
    else
        goto L_4941;

L_4941:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_4971;
    else
        goto L_4969;

L_4969:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_4ea2;
    else
        goto L_4971;

L_4971:
    if ((lpfl->iPlayer == idPlayer))
        goto L_49a3;
    else
        goto L_4980;

L_4980:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_4932;

L_49a3:
    if ((FIsTurinDroneAiAttack(lpfl) == 0x0))
        goto L_49d6;
    else
        goto L_49b9;

L_49b9:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;

L_49d6:
    lpfl->fMark = 0x0;
    if ((lpfl->rgcsh[0x2] != 0x0))
        goto L_4a04;
    else
        goto L_49f7;

L_49f7:
    if ((lpfl->rgcsh[0x3] == 0x0))
        goto L_4b45;
    else
        goto L_4a04;

L_4a04:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[(lpfl->cord - 0x1)]) & 0xf) == 0x0 ? L_4b45 : L_4a46 */

L_4a46:
    if ((lpfl->idPlanet == 0xffff))
        goto L_4ac3;
    else
        goto L_4a53;

L_4a53:
    if ((LpplFromId(lpfl->idPlanet)->iPlayer == 0xffff))
        goto L_4ab6;
    else
        goto LBlowAwayOrders;

LBlowAwayOrders:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);
    ClearAiCurrentTask(lpfl, 0x0);
    goto L_4932;

L_4ab6:
    idPlanDst = lpfl->idPlanet;
    goto L_4b24;

L_4ac3:
    if ((lpfl->cord <= 0x1))
        goto L_4932;
    else
        goto L_4ad0;

L_4ad0:
    /* untranslated: part[6:2](lpfl->lpplord->rgord[0x1]) = ((part[6:2](lpfl->lpplord->rgord[0x1]) & 0xfff0) | 0x3) */
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_4b0d : L_4b07 */

L_4b07:
    goto L_4b10;

L_4b0d:

L_4b10:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;
    goto L_4b24;

L_4b24:
    vlpbAiPlanet[((idPlanDst * 0x10) + 0x1)] = (vlpbAiPlanet[((idPlanDst * 0x10) + 0x1)] | 0x80);
    goto L_4932;

L_4b45:
    if ((lpfl->rgcsh[0x8] != 0x0))
        goto L_4b5f;
    else
        goto L_4b52;

L_4b52:
    if ((lpfl->rgcsh[0x9] == 0x0))
        goto L_4dd1;
    else
        goto L_4b5f;

L_4b5f:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_4b9d;
    else
        goto L_4b71;

L_4b71:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_4ba7 : L_4b8c */

L_4b8c:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_4b9d:
    idPlanDst = lpfl->idPlanet;

LCheckForColDrop:
    if ((idPlanDst == 0xffff))
        goto L_4da3;
    else
        goto L_4bb0;

L_4bb0:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_4bd3;
    else
        goto L_4bca;

L_4bca:
    if ((HIWORD(lppl) == 0x0))
        goto L_4bef;
    else
        goto L_4bd3;

L_4bd3:
    if ((lppl->iPlayer == 0xffff))
        goto L_4932;
    else
        goto L_4be0;

L_4be0:
    if ((lppl->iPlayer == idPlayer))
        goto L_4932;
    else
        goto L_4bef;

L_4bef:
    if ((vlpbAiPlanet[((idPlanDst * 0x10) + 0x3)] == 0x0))
        goto LBlowAwayOrders;
    else
        goto L_4c17;

L_4c17:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto LBlowAwayOrders;
    else
        goto L_4c24;

L_4c24:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_4c33;
    else
        goto L_4c29;

L_4c29:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto LBlowAwayOrders;
    else
        goto L_4c33;

L_4c33:
    if ((GetRaceStat(rgplr[lppl->iPlayer], rsMajorAdv) == raMacintosh))
        goto LBlowAwayOrders;
    else
        goto L_4c57;

L_4c57:
    if ((lppl->fStarbase != 0x0))
        goto LBlowAwayOrders;
    else
        goto L_4c6e;

L_4c6e:
    memset(ord, 0x0, 0x12);
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grobj = grobjPlanet;
    ord.id = idPlanDst;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    /* untranslated: part[14:2](ord) = ((part[14:2](ord) & 0xfff) | 0x2000) */
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((sel.fl.lpplord->rgord[0x0].id != idPlanDst))
        goto L_4d32;
    else
        goto L_4cef;

L_4cef:
    /* untranslated: branch ((part[6:2](sel.fl.lpplord->rgord[0x0]) >> 0x8) & 0xf) != 0x1 ? L_4d32 : L_4d07 */

L_4d07:
    sel.fl.lpplord->rgord[0x0] = ord;
    goto L_4d5a;

L_4d32:
    sel.fl.lpplord->rgord[0x1] = ord;

L_4d5a:
    FLookupFleet(0xffff, 0x4972);
    vlpbAiPlanet[((idPlanDst * 0x10) + 0x3)] = (vlpbAiPlanet[((idPlanDst * 0x10) + 0x3)] | 0x80);
    FMoveToNearestStarbase(lpfl, 0x0);
    goto L_4932;

L_4da3:
    if ((lpfl->cord <= 0x1))
        goto L_4932;
    else
        goto L_4db0;

L_4db0:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x4 ? L_4a70 : L_4dc8 */

L_4dc8:

L_4dd1:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto L_4932;
    else
        goto L_4dde;

L_4dde:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_4e1c;
    else
        goto L_4df0;

L_4df0:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_4e26 : L_4e0b */

L_4e0b:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_4e1c:
    idPlanDst = lpfl->idPlanet;

L_4e26:
    if ((idPlanDst == 0xffff))
        goto L_4932;
    else
        goto L_4e2f;

L_4e2f:
    if ((vlpbAiPlanet[((idPlanDst * 0x10) + 0x3)] != 0x0))
        goto LCheckForColDrop;
    else
        goto L_4e54;

L_4e54:

L_4e5d:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_4e80;
    else
        goto L_4e77;

L_4e77:
    if ((HIWORD(lppl) == 0x0))
        goto L_4932;
    else
        goto L_4e80;

L_4e80:
    if ((lppl->iPlayer == 0xffff))
        goto L_4932;
    else
        goto L_4e8d;

L_4e8d:
    if ((lppl->iPlayer != idPlayer))
        goto LBlowAwayOrders;
    else
        goto L_4e99;

L_4e99:

L_4ea2:
    fMarkedPlanets = 0x0;
    ifl = 0x0;
    goto L_4eb4;

L_4eb0:
    ifl = (ifl + 0x1);

L_4eb4:
    if ((ifl >= cFleet))
        goto BestSpeed;
    else
        goto L_4ebf;

L_4ebf:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_4eef;
    else
        goto L_4ee7;

L_4ee7:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto BestSpeed;
    else
        goto L_4eef;

L_4eef:
    if ((lpfl->iPlayer != idPlayer))
        goto L_4eb0;
    else
        goto L_4efb;

L_4efb:

L_4f01:
    if ((lpfl->rgcsh[0x2] != 0x0))
        goto L_4f1b;
    else
        goto L_4f0e;

L_4f0e:
    if ((lpfl->rgcsh[0x3] == 0x0))
        goto L_5069;
    else
        goto L_4f1b;

L_4f1b:
    if ((game.turn == 0x0))
        goto LScrapFleet;
    else
        goto L_4f22;

L_4f22:

L_4f28:
    if ((lpfl->idPlanet == 0xffff))
        goto L_4eb0;
    else
        goto L_4f35;

L_4f35:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    b = vlpbAiPlanet[((lpfl->idPlanet * 0x10) + 0x1)];
    if ((b >= 0x4))
        goto L_4eb0;
    else
        goto L_4f79;

L_4f79:
    lppl = LpplFindBestEnum(&(sel.pl), FEnumCalcMinerDest);
    if ((LOWORD(lppl) != 0x0))
        goto L_4fa8;
    else
        goto L_4f9f;

L_4f9f:
    if ((HIWORD(lppl) == 0x0))
        goto L_4eb0;
    else
        goto L_4fa8;

L_4fa8:
    memset(ord, 0x0, 0x12);
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskMine;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    FMoveAiFleet(lpfl, &(ord), 0x1);
    vlpbAiPlanet[((lppl->id * 0x10) + 0x1)] = (vlpbAiPlanet[((lppl->id * 0x10) + 0x1)] | 0x80);
    vlpbAiPlanet[((lpfl->idPlanet * 0x10) + 0x1)] = (vlpbAiPlanet[((lpfl->idPlanet * 0x10) + 0x1)] & 0x80);

L_5069:
    if ((lpfl->cord > 0x1))
        goto L_4eb0;
    else
        goto L_5073;

L_5073:

L_5079:
    if ((lpfl->rgcsh[0x1] == 0x0))
        goto LTryFreighters;
    else
        goto L_5083;

L_5083:

L_5089:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet == 0xffff))
        goto L_50cd;
    else
        goto L_50a8;

L_50a8:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_50cd;
    else
        goto L_50b4;

L_50b4:
    /* untranslated: branch part[194:2](sel) > 0x0 ? L_5170 : L_50be */

L_50be:
    /* untranslated: branch part[194:2](sel) < 0x0 ? L_50cd : L_50c3 */

L_50c3:
    /* untranslated: branch part[192:2](sel) >= 0x32 ? L_5170 : L_50cd */

L_50cd:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_5170;
    else
        goto L_50da;

L_50da:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_5170;
    else
        goto L_50e4;

L_50e4:
    if ((sel.fl.idPlanet == 0xffff))
        goto L_510d;
    else
        goto L_50ee;

L_50ee:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_510d;
    else
        goto L_50fa;

L_50fa:
    if ((sel.pl.fStarbase != 0x0))
        goto L_4eb0;
    else
        goto L_510d;

L_510d:
    /* untranslated: branch (part[207:2](rgshdef) & 0xff) <= 0x2 ? L_5135 : L_511b */

L_511b:
    if ((FMoveToNearestStarbase(lpfl, 0x0) != 0x0))
        goto L_4eb0;
    else
        goto LScrapFleet;

LScrapFleet:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);

L_5170:
    lpthWorm = 0x0;
    idPlanDst = IdNearestColonizablePlanet(lpfl, 0x0);
    if ((lpfl->idPlanet == 0xffff))
        goto L_5209;
    else
        goto L_519c;

L_519c:
    if ((sel.pl.iPlayer != idPlayer))
        goto L_5209;
    else
        goto L_51a8;

L_51a8:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, 0x19);
    FLookupFleet(LOWORD(lpfl), 0x4972);
    lppl = LpplFromId(lpfl->idPlanet);
    goto L_5215;

L_5209:
    lppl = lpplHome;

L_5215:
    if ((idPlanDst == 0xffff))
        goto L_5250;
    else
        goto L_521e;

L_521e:
    FColonizeAiFleet(lpfl, idPlanDst);
    vlpbAiPlanet[((idPlanDst * 0x10) + 0xf)] = 0x4;
    goto L_4eb0;

L_5250:
    if ((LOWORD(lppl) != 0x0))
        goto L_5262;
    else
        goto L_5259;

L_5259:
    if ((HIWORD(lppl) == 0x0))
        goto L_533b;
    else
        goto L_5262;

L_5262:
    lpplDest = LpplFindClosestEnum(lppl, FEnumCalcColonistDrop);
    if ((LOWORD(lpplDest) != 0x0))
        goto L_5290;
    else
        goto L_5287;

L_5287:
    if ((HIWORD(lpplDest) == 0x0))
        goto L_533b;
    else
        goto L_5290;

L_5290:
    vlpbAiPlanet[((lpplDest->id * 0x10) + 0xa)] = (vlpbAiPlanet[((lpplDest->id * 0x10) + 0xa)] | 0x80);
    memset(ord, 0x0, 0x12);
    ord.id = lpplDest->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplDest->id].x;
    ord.pt.y = rgptPlan[lpplDest->id].y;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    /* untranslated: part[14:2](ord) = ((part[14:2](ord) & 0xfff) | 0x2000) */
    FMoveAiFleet(lpfl, &(ord), 0x0);
    goto L_4eb0;

L_533b:
    if ((LOWORD(lpthWorm) != 0x0))
        goto L_534d;
    else
        goto L_5344;

L_5344:
    if ((HIWORD(lpthWorm) == 0x0))
        goto L_4eb0;
    else
        goto L_534d;

L_534d:
    if ((Random(0x64) >= 0xa))
        goto L_4eb0;
    else
        goto L_5361;

L_5361:
    FGotoWormholeAiFleet(lpfl, lpthWorm);

LTryFreighters:
    if ((lpfl->rgcsh[0x8] != 0x0))
        goto L_5395;
    else
        goto L_5385;

L_5385:
    if ((lpfl->rgcsh[0x9] == 0x0))
        goto LTryBombers;
    else
        goto L_538f;

L_538f:

L_5395:
    if ((game.turn != 0x0))
        goto L_53b4;
    else
        goto L_539f;

L_539f:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    goto LScrapFleet;

L_53b4:
    if ((LOWORD(lpplHome) != 0x0))
        goto L_53c9;
    else
        goto L_53bd;

L_53bd:
    if ((HIWORD(lpplHome) == 0x0))
        goto BestSpeed;
    else
        goto L_53c3;

L_53c3:

L_53c9:
    lppl = 0x0;
    i = 0x0;
    goto L_53df;

L_53db:
    i = (i + 0x1);

L_53df:
    if ((i >= vlpbAiData[0x2]))
        goto L_5486;
    else
        goto L_53ef;

L_53ef:
    j = 0x0;
    goto L_53fb;

L_53f7:
    j = (j + 0x1);

L_53fb:
    if ((j >= HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_545d;
    else
        goto L_541e;

L_541e:
    if ((vlpbAiData[(((0x4 + LOWORD((0x14 * i))) + 0x4) + (j * 0x2))] == LOWORD(lpfl)))
        goto L_545d;
    else
        goto L_5454;

L_5454:

L_545d:
    if ((j < HIWORD(vlpbAiData[(0x4 + LOWORD((0x14 * i)))])))
        goto L_5486;
    else
        goto L_547d;

L_547d:

L_5486:
    if ((i >= vlpbAiData[0x2]))
        goto L_54be;
    else
        goto L_5496;

L_5496:
    lppl = LpplFromId(vlpbAiData[(0x4 + LOWORD((0x14 * i)))]);

L_54be:
    if ((LOWORD(lppl) != 0x0))
        goto L_54d0;
    else
        goto L_54c7;

L_54c7:
    if ((HIWORD(lppl) == 0x0))
        goto L_54d9;
    else
        goto L_54d0;

L_54d0:
    t_merge_54df_0001_wide = lppl;
    goto L_54df;

L_54d9:
    t_merge_54df_0001_wide = lpplHome;

L_54df:
    IdTargetFreighter(lpfl, t_merge_54df_0001_wide);
    goto L_4eb0;

LTryBombers:
    if ((lpfl->rgcsh[0xd] != 0x0))
        goto L_550f;
    else
        goto L_54ff;

L_54ff:
    if ((lpfl->rgcsh[0xe] == 0x0))
        goto LTryScouts;
    else
        goto L_5509;

L_5509:

L_550f:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet == 0xffff))
        goto L_564e;
    else
        goto L_552e;

L_552e:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_55c7;
    else
        goto L_5552;

L_5552:
    if ((lppl->fStarbase == 0x0))
        goto L_55b2;
    else
        goto L_5569;

L_5569:
    if (((lpfl->rgcsh[0xd] + lpfl->rgcsh[0xe]) < (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_4eb0;
    else
        goto L_558c;

L_558c:
    /* untranslated: branch (lpfl->rgcsh[0x4] + lpfl->rgcsh[0x5]) < (part[1:2](vrgAiArmadaPotency) & 0xff) ? L_4eb0 : L_55ac */

L_55ac:

L_55b2:
    FLookupFleet(LOWORD(lpfl), 0x4972);
    goto LTargetBomber;

L_55c7:
    if ((lppl->iPlayer == 0xffff))
        goto LTargetBomber;
    else
        goto L_55d4;

L_55d4:
    lpflT = lpflEnemy;
    goto L_55f4;

L_55e3:
    lpflT = lpflT->lpflNext;

L_55f4:
    if ((LOWORD(lpflT) != 0x0))
        goto L_5606;
    else
        goto L_55fd;

L_55fd:
    if ((HIWORD(lpflT) == 0x0))
        goto L_4eb0;
    else
        goto L_5606;

L_5606:
    if ((lpfl->pt.x != lpflT->pt.x))
        goto L_55e3;
    else
        goto L_5619;

L_5619:
    if ((lpfl->pt.y != lpflT->pt.y))
        goto L_55e3;
    else
        goto L_562c;

L_562c:
    if ((FIsAiAttack(lpflT) != 0x0))
        goto LTargetBomber;
    else
        goto L_563f;

L_563f:

L_564e:
    lppl = lpplHome;

LTargetBomber:
    if ((game.fAisBand == 0x0))
        goto L_568f;
    else
        goto L_5670;

L_5670:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_5699;

L_568f:
    lpplDest = 0x0;

L_5699:
    if ((LOWORD(lpplDest) != 0x0))
        goto L_56c7;
    else
        goto L_56a2;

L_56a2:
    if ((HIWORD(lpplDest) != 0x0))
        goto L_56c7;
    else
        goto L_56ab;

L_56ab:
    lpplDest = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

L_56c7:
    lppl = lpplDest;
    if ((LOWORD(lppl) != 0x0))
        goto L_56e5;
    else
        goto L_56dc;

L_56dc:
    if ((HIWORD(lppl) == 0x0))
        goto L_4eb0;
    else
        goto L_56e5;

L_56e5:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] = (vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] | 0x80);
    ord.id = lppl->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lppl->id].x;
    ord.pt.y = rgptPlan[lppl->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);

LTryScouts:
    if ((lpfl->rgcsh[0x0] != 0x0))
        goto L_579a;
    else
        goto L_577d;

L_577d:
    if ((lpfl->rgcsh[0xa] != 0x0))
        goto L_579a;
    else
        goto L_578a;

L_578a:
    if ((lpfl->rgcsh[0xb] == 0x0))
        goto LTryMinelayers;
    else
        goto L_5794;

L_5794:

L_579a:
    if ((lpfl->rgcsh[0x0] == 0x0))
        goto L_57cc;
    else
        goto L_57a7;

L_57a7:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_57cc;
    else
        goto L_57bf;

L_57bf:
    if ((LOWORD(rgshdef) == 0x4))
        goto LScrapFleet;
    else
        goto L_57c6;

L_57c6:

L_57cc:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    IdTargetScout(lpfl, lpflAttack, lpflEnemy, game.fAisBand, &(lpthWorm));
    goto L_4eb0;

LTryMinelayers:
    if ((lpfl->rgcsh[0xc] == 0x0))
        goto L_4eb0;
    else
        goto L_5818;

L_5818:

L_581e:
    if ((lpfl->cord != 0x1))
        goto L_4eb0;
    else
        goto L_582b;

L_582b:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x0 ? L_4eb0 : L_5841 */

L_5841:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x6) */
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    FLookupFleet(0xffff, 0x4972);

BestSpeed:
    HandleBasicAiTasks(iroCur, rgprod, ishdefSBLatest, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureTurinDroneShdefs(int16_t iroCur) {
    SHDEF   shdef;
    int16_t i;

L_58ba:
    /* untranslated: branch ((part[1299:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5926 : L_58d6 */

L_58d6:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_5926;
    else
        goto L_58ee;

L_58ee:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x8))
        goto L_5926;
    else
        goto L_5906;

L_5906:
    /* untranslated: call FCreateAiShdef(0x8, 0xc, &vrgTDAip[(part[12:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5926:
    /* untranslated: branch ((part[1446:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5989 : L_5939 */

L_5939:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_5989;
    else
        goto L_5951;

L_5951:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xb))
        goto L_5989;
    else
        goto L_5969;

L_5969:
    /* untranslated: call FCreateAiShdef(0x9, 0xd, &vrgTDAip[(part[15:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5989:
    /* untranslated: branch ((part[1593:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5a51 : L_599c */

L_599c:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_5a51;
    else
        goto L_59b4;

L_59b4:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x5))
        goto L_5a51;
    else
        goto L_59cc;

L_59cc:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x4))
        goto L_5a51;
    else
        goto L_59e4;

L_59e4:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x5))
        goto L_5a51;
    else
        goto L_59fc;

L_59fc:
    i = 0x0;
    goto L_5a47;

L_5a05:
    /* untranslated: branch FCreateAiShdef(0xa, 0x6, &vrgTDAip[byte 0x12:[(Random(0x1) + 0x2)+0x35ae]]) != 0x0 ? L_5a51 : L_5a3c */

L_5a3c:

L_5a42:
    i = (i + 0x1);

L_5a47:
    if ((i < 0x4))
        goto L_5a05;
    else
        goto L_5a51;

L_5a51:
    /* untranslated: branch ((part[270:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5a78 : L_5a64 */

L_5a64:
    /* untranslated: branch part[278:2](rgshdef) != 0x0 ? L_5ae5 : L_5a6e */

L_5a6e:
    /* untranslated: branch part[280:2](rgshdef) != 0x0 ? L_5ae5 : L_5a78 */

L_5a78:
    /* untranslated: branch ((part[270:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5ac5 : L_5a8b */

L_5a8b:
    /* untranslated: branch part[147:2](rgshdef) == 0xb ? L_5ac5 : L_5a95 */

L_5a95:
    /* untranslated: shdef = part[147:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x1);

L_5ac5:
    FCreateAiShdef(0x1, 0xf, &(vrgTDAip[(LOWORD(vrgTDIshAip) & 0xff)]));

L_5ae5:
    /* untranslated: branch ((part[123:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5b0c : L_5af8 */

L_5af8:
    /* untranslated: branch part[131:2](rgshdef) != 0x0 ? L_5b6f : L_5b02 */

L_5b02:
    /* untranslated: branch part[133:2](rgshdef) != 0x0 ? L_5b6f : L_5b0c */

L_5b0c:
    /* untranslated: branch ((part[123:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5b4f : L_5b1f */

L_5b1f:
    /* untranslated: shdef = part[0:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x0);

L_5b4f:
    /* untranslated: call FCreateAiShdef(0x0, 0x5, &vrgTDAip[(part[1:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5b6f:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5b96 : L_5b82 */

L_5b82:
    /* untranslated: branch part[425:2](rgshdef) != 0x0 ? L_5c29 : L_5b8c */

L_5b8c:
    /* untranslated: branch part[427:2](rgshdef) != 0x0 ? L_5c29 : L_5b96 */

L_5b96:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x7))
        goto L_5c29;
    else
        goto L_5bae;

L_5bae:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x4))
        goto L_5c29;
    else
        goto L_5bc6;

L_5bc6:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5c09 : L_5bd9 */

L_5bd9:
    /* untranslated: shdef = part[294:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x2);

L_5c09:
    /* untranslated: call FCreateAiShdef(0x2, 0x16, &vrgTDAip[(part[17:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5c29:
    /* untranslated: branch ((part[1887:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_5c50 : L_5c3c */

L_5c3c:
    /* untranslated: branch part[1895:2](rgshdef) != 0x0 ? L_5ca0 : L_5c46 */

L_5c46:
    /* untranslated: branch part[1897:2](rgshdef) != 0x0 ? L_5ca0 : L_5c50 */

L_5c50:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x4))
        goto L_5ca0;
    else
        goto L_5c68;

L_5c68:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) < 0x4))
        goto L_5ca0;
    else
        goto L_5c80;

L_5c80:
    /* untranslated: call FCreateAiShdef(0xc, 0xb, &vrgTDAip[(part[14:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5ca0:
    /* untranslated: branch ((part[2034:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5d1b : L_5cb3 */

L_5cb3:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x8))
        goto L_5d1b;
    else
        goto L_5ccb;

L_5ccb:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x7))
        goto L_5d1b;
    else
        goto L_5ce3;

L_5ce3:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_5d1b;
    else
        goto L_5cfb;

L_5cfb:
    /* untranslated: call FCreateAiShdef(0xd, 0x12, &vrgTDAip[(part[13:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5d1b:
    /* untranslated: branch ((part[2181:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5dae : L_5d2e */

L_5d2e:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xb))
        goto L_5dae;
    else
        goto L_5d46;

L_5d46:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0xc))
        goto L_5dae;
    else
        goto L_5d5e;

L_5d5e:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xf))
        goto L_5dae;
    else
        goto L_5d76;

L_5d76:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x9))
        goto L_5dae;
    else
        goto L_5d8e;

L_5d8e:
    /* untranslated: call FCreateAiShdef(0xe, 0x12, &vrgTDAip[(part[13:2](vrgTDIshAip) & 0xff)]) -> callresult(int16_t) */

L_5dae:
    /* untranslated: branch ((part[711:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5e76 : L_5dc1 */

L_5dc1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_5e76;
    else
        goto L_5dd9;

L_5dd9:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x6))
        goto L_5e76;
    else
        goto L_5df1;

L_5df1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xd))
        goto L_5e76;
    else
        goto L_5e09;

L_5e09:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_5e76;
    else
        goto L_5e21;

L_5e21:
    i = 0x0;
    goto L_5e6c;

L_5e2a:
    /* untranslated: branch FCreateAiShdef(0x4, 0x9, &vrgTDAip[byte 0x12:[(Random(0x4) + 0x8)+0x35ae]]) != 0x0 ? L_5e76 : L_5e61 */

L_5e61:

L_5e67:
    i = (i + 0x1);

L_5e6c:
    if ((i < 0x5))
        goto L_5e2a;
    else
        goto L_5e76;

L_5e76:
    /* untranslated: branch ((part[2328:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_5f2c : L_5e89 */

L_5e89:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_5f2c;
    else
        goto L_5ea1;

L_5ea1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x6))
        goto L_5f2c;
    else
        goto L_5eb9;

L_5eb9:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xd))
        goto L_5f2c;
    else
        goto L_5ed1;

L_5ed1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x7))
        goto L_5f2c;
    else
        goto L_5ee9;

L_5ee9:
    i = 0x0;
    goto L_5f22;

L_5ef2:
    /* untranslated: branch FCreateAiShdef(0xf, 0xc, &vrgTDAip[(part[12:2](vrgTDIshAip) & 0xff)]) != 0x0 ? L_5f2c : L_5f17 */

L_5f17:

L_5f1d:
    i = (i + 0x1);

L_5f22:
    if ((i < 0x5))
        goto L_5ef2;
    else
        goto L_5f2c;

L_5f2c:
    return;
}

int16_t FEnumCalcMinerDest(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t b;

L_5f32:
    if ((LOWORD(lpplSrc) != LOWORD(lpplTest)))
        goto L_5f57;
    else
        goto L_5f49;

L_5f49:
    if ((HIWORD(lpplSrc) != HIWORD(lpplTest)))
        goto L_5f57;
    else
        goto L_5f51;

L_5f51:
    return 0x0;

L_5f57:
    id = lpplTest->id;
    b = vlpbAiPlanet[((id * 0x10) + 0x1)];
    if ((b == 0x0))
        goto L_5fbc;
    else
        goto L_5f8e;

L_5f8e:
    if ((Random(0x64) < 0x19))
        goto L_5fb3;
    else
        goto L_5fa2;

L_5fa2:
    if (((b & 0x80) != 0x0))
        goto L_5fbc;
    else
        goto L_5fb3;

L_5fb3:
    return b;

L_5fbc:
    return 0x0;
}

int16_t FEnumCalcColonistDrop(PLANET *lpplSrc, PLANET *lpplTest) {
    int16_t id;
    uint8_t bWant;
    uint8_t bEnemy;

L_5fc8:
    if ((LOWORD(lpplSrc) != LOWORD(lpplTest)))
        goto L_5fed;
    else
        goto L_5fdf;

L_5fdf:
    if ((HIWORD(lpplSrc) != HIWORD(lpplTest)))
        goto L_5fed;
    else
        goto L_5fe7;

L_5fe7:
    return 0x0;

L_5fed:
    id = lpplTest->id;
    bEnemy = vlpbAiPlanet[((lpplTest->id * 0x10) + 0xa)];
    bWant = vlpbAiPlanet[((lpplTest->id * 0x10) + 0x3)];
    if ((bEnemy != 0x1))
        goto L_60b0;
    else
        goto L_604a;

L_604a:
    if ((bWant == 0x0))
        goto L_60b0;
    else
        goto L_6058;

L_6058:
    if (((bEnemy & 0x80) == 0x0))
        goto L_607d;
    else
        goto L_6069;

L_6069:
    if ((Random(0x64) >= 0x19))
        goto L_60b0;
    else
        goto L_607d;

L_607d:
    if ((GetRaceStat(rgplr[lpplTest->iPlayer], rsMajorAdv) != raMacintosh))
        goto L_60a7;
    else
        goto L_60a1;

L_60a1:
    return 0x0;

L_60a7:
    return bWant;

L_60b0:
    return 0x0;
}
