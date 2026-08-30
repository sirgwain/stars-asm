#include "common.h"

uint16_t vrgMacIshAip[31] = {0,   7,   14,  21,  28,  35,  42,  49,  56,  63,  70,  74,  85,  96,  107, 118,
                             129, 140, 151, 162, 173, 175, 181, 187, 191, 194, 201, 208, 215, 222, 235};
uint8_t  vrgMacAip[248] = {8,  4,  4,  4,  17, 18, 19, 8,  3,  3,  14, 17, 18, 19, 8,  4,  3,  2,  17, 18, 20, 8,  4,  4,  5,  17, 18, 20, 8,  0,  0,
                           10, 17, 18, 19, 8,  0,  0,  11, 17, 18, 19, 8,  1,  1,  11, 17, 18, 19, 8,  1,  1,  11, 17, 18, 11, 24, 21, 23, 23, 23, 12,
                           10, 24, 21, 22, 22, 22, 12, 10, 24, 26, 25, 10, 8,  11, 10, 1,  1,  1,  1,  2,  9,  11, 19, 8,  13, 10, 1,  1,  0,  0,  0,
                           9,  11, 19, 8,  13, 10, 0,  0,  1,  1,  0,  9,  11, 19, 8,  13, 10, 0,  0,  0,  0,  3,  9,  11, 19, 8,  13, 10, 4,  4,  4,
                           4,  4,  9,  20, 19, 8,  13, 10, 4,  3,  3,  7,  2,  9,  20, 19, 8,  13, 10, 2,  3,  7,  7,  3,  9,  20, 19, 8,  13, 10, 4,
                           4,  3,  3,  5,  9,  20, 19, 8,  14, 10, 33, 33, 33, 33, 33, 17, 20, 19, 8,  40, 24, 41, 42, 42, 42, 42, 24, 41, 43, 43, 43,
                           43, 24, 41, 42, 42, 44, 16, 37, 8,  13, 12, 3,  3,  2,  10, 8,  37, 12, 1,  1,  11, 9,  8,  13, 12, 4,  4,  7,  10, 8,  11,
                           12, 0,  0,  0,  17, 8,  9,  9,  3,  3,  33, 33, 10, 17, 10, 6,  7,  20, 8,  10, 10, 7,  5,  20, 20, 4,  4,  19, 4,  2,  3};
uint8_t  vrgAiMacintiResOrder[8] = {3, 66, 20, 113, 84, 52, 119, 55};

void DoMacintiAiTurn(PROD *rgprod) {
    int16_t  iLatestCargo;
    int16_t  cColFleet;
    int32_t  rgResCost[4];
    int16_t  idPlanDst;
    int16_t  j;
    FLEET   *lpflEnemy;
    int32_t  rgResAvail[4];
    PLANET  *lpplMac;
    int16_t  iLatestCruiser;
    int16_t  ishLastBattle;
    THING   *lpthWorm;
    int16_t  cFlMineLayers;
    int16_t  fShouldColonize;
    int16_t  cFlDestroyers;
    int16_t  cshDestroyer;
    int16_t  iAiLvl;
    int16_t  iLatestBattle;
    PLANET  *lppl;
    int16_t  cFlCargo;
    int16_t  iLatestBomber;
    int16_t  ifl;
    int16_t  i;
    FLEET   *lpfl;
    int16_t  cGenesis;
    int16_t  cFlArmadas;
    int16_t  cRes;
    int16_t  cFr;
    int16_t  iroCur;
    int16_t  iLatestMiner;
    int16_t  fUsingTempColonizer;
    int16_t  iLatestDestroyer;
    int16_t  ipl;
    int16_t  iLatestColony;
    uint16_t cRecyclePeriod;
    uint8_t  rgRecycleShdef[16];
    uint8_t *lpb;
    int16_t  cFlMineLayersBase;
    int16_t  cFlMiners;
    uint16_t rgCosts[4];
    FLEET   *lpflAttack;
    int16_t  iPlanet;
    int32_t  l;
    int16_t  fWrite;
    int16_t  fTonsOfMinerals;
    uint8_t  rgRecycleSBShdef[10];
    PROD    *lpprod;
    PART     part;
    SHDEF    shdef;
    PLANET  *lpplDest;
    int32_t  lLeast;
    PLANET  *lpplBest;
    int16_t  iplDest;
    int32_t  cMine;
    int16_t  id;
    int16_t  cConc;
    int16_t  iLatest;
    int16_t  dy;
    int32_t  lDist;
    int16_t  dx;
    ORDER    ord;
    PLANET  *lpplDrop;
    uint16_t t_merge_01e7_0001;
    uint16_t t_merge_0224_0001;
    uint16_t t_merge_0308_0001;
    uint16_t t_merge_14ad_0001;
    uint16_t t_merge_15cf_0001;
    uint16_t t_merge_186c_0001;
    uint16_t t_merge_18a9_0001;
    uint16_t t_merge_2919_0001;
    uint16_t t_merge_2934_0001;

L_0008:
    /* untranslated: iAiLvl = ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) */
    iPlanet = rgplr[idPlayer].idPlanetHome;
    iroCur = IroEnsureAi(vrgAiMacintiResOrder, 0x8, 0x0, 0xf);
    if ((game.turn < 0x28))
        goto L_0083;
    else
        goto L_0066;

L_0066:
    /* untranslated: branch ((part[1152:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_00f3 : L_0079 */

L_0079:
    /* untranslated: branch part[1029:2](rgshdef) != 0xf ? L_00f3 : L_0083 */

L_0083:
    fUsingTempColonizer = 0x1;
    if ((FLookupPartX(&(part), 0x1, 0xf) != 0x1))
        goto L_00f8;
    else
        goto L_00a5;

L_00a5:
    /* untranslated: branch part[1160:2](rgshdef) != 0x0 ? L_00f8 : L_00af */

L_00af:
    /* untranslated: branch part[1162:2](rgshdef) != 0x0 ? L_00f8 : L_00b9 */

L_00b9:
    /* untranslated: shdef = part[1029:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x7);
    fUsingTempColonizer = 0x0;

L_00f3:
    fUsingTempColonizer = 0x0;

L_00f8:
    ishLastBattle = (0x7 - fUsingTempColonizer);
    if ((fUsingTempColonizer == 0x0))
        goto L_0119;
    else
        goto L_010a;

L_010a:
    MergeAllShdefs(0x37c);
    goto L_0125;

L_0119:
    MergeAllShdefs(0x3fc);

L_0125:
    MergeAllShdefs(0x1);
    MergeAllShdefs(0xc000);
    MergeAllShdefs(0x3000);
    j = 0x6;
    if ((game.turn <= 0x82))
        goto L_0169;
    else
        goto L_0159;

L_0159:
    /* untranslated: j = (j + (words((game.turn + 0xff88), 0x0) / 0x14)) */

L_0169:
    if ((j <= 0x32))
        goto L_0177;
    else
        goto L_0172;

L_0172:
    j = 0x32;

L_0177:
    LOWORD(vrgAiArmadaPotency) = LOBYTE(j);
    /* untranslated: part[1:2](vrgAiArmadaPotency) = lobyte((sext16to32((j & 0xff)) / 0x2)) */
    j = 0x6;
    if ((game.turn <= 0x73))
        goto L_01ab;
    else
        goto L_019b;

L_019b:
    /* untranslated: j = (j + (words((game.turn + 0xff9c), 0x0) / 0x16)) */

L_01ab:
    if ((j <= 0xc))
        goto L_01b9;
    else
        goto L_01b4;

L_01b4:
    j = 0xc;

L_01b9:
    HIWORD(vrgAiArmadaPotency) = LOBYTE(j);
    if ((0x3 >= (((uint32_t)(j) / 0x2) + 0xffff)))
        goto L_01db;
    else
        goto L_01d5;

L_01d5:
    t_merge_01e7_0001 = 0x3;
    goto L_01e7;

L_01db:
    t_merge_01e7_0001 = (((uint32_t)(j) / 0x2) + 0xffff);

L_01e7:
    /* untranslated: part[3:2](vrgAiArmadaPotency) = lobyte(t_merge_01e7_0001) */
    memset(rgRecycleShdef, 0x0, 0x10);
    if ((game.turn >= 0x78))
        goto L_0210;
    else
        goto L_0208;

L_0208:
    cRecyclePeriod = 0x32;
    goto L_0227;

L_0210:
    if ((game.turn >= 0xc8))
        goto L_0221;
    else
        goto L_021b;

L_021b:
    t_merge_0224_0001 = 0x46;
    goto L_0224;

L_0221:
    t_merge_0224_0001 = 0x64;

L_0224:
    cRecyclePeriod = t_merge_0224_0001;

L_0227:
    /* untranslated: cshDestroyer = CheckAiShdefStatus(0xc, 0xd, cRecyclePeriod, &iLatestDestroyer, &part[0:1](rgRecycleShdef)) */
    i = 0xc;
    goto L_02a1;

L_024d:
    if ((rgRecycleShdef[i] == 0x0))
        goto L_029d;
    else
        goto L_0262;

L_0262:
    /* untranslated: branch ((part[123:2](rgshdef[i]) >> 0x9) & 0x1) != 0x0 ? L_029d : L_0280 */

L_0280:
    if ((rgshdef[i].hul.ihuldef != ihuldefNubian))
        goto L_029d;
    else
        goto L_0292;

L_0292:
    rgRecycleShdef[i] = 0x0;

L_029d:
    i = (i + 0x1);

L_02a1:
    if ((i <= 0xd))
        goto L_024d;
    else
        goto L_02aa;

L_02aa:
    /* untranslated: call CheckAiShdefStatus(0xe, 0xf, 0x1388, &iLatestMiner, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: branch ((part[2328:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_03c3 : L_02d9 */

L_02d9:
    if ((FLookupPartX(&(part), 0x80, 0x6) != 0x1))
        goto L_03c3;
    else
        goto L_02f6;

L_02f6:
    if ((iLatestMiner != 0xe))
        goto L_0305;
    else
        goto L_02ff;

L_02ff:
    t_merge_0308_0001 = 0xf;
    goto L_0308;

L_0305:
    t_merge_0308_0001 = 0xe;

L_0308:
    i = t_merge_0308_0001;
    /* untranslated: branch (part[2126:2](rgshdef) & 0xff) == 0x6 ? L_0382 : L_0319 */

L_0319:
    i = 0xe;

LUpgradeMiner:
    /* untranslated: branch part[131:2](rgshdef[i]) != 0x0 ? L_037b : L_0333 */

L_0333:
    /* untranslated: branch part[133:2](rgshdef[i]) != 0x0 ? L_037b : L_033d */

L_033d:
    shdef = rgshdef[i];
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), i);
    goto L_03c3;

L_037b:
    /* untranslated: part[14:1](rgRecycleShdef) = 0x3 */

L_0382:
    /* untranslated: branch (part[2273:2](rgshdef) & 0xff) == 0x6 ? L_03c3 : L_0390 */

L_0390:
    if ((FLookupPartX(&(part), 0x1, 0xf) != 0x1))
        goto L_03c3;
    else
        goto L_03ad;

L_03ad:
    /* untranslated: branch (part[2118:2](rgshdef) & 0xff) == 0xf ? L_03c3 : L_03bb */

L_03bb:
    i = 0xf;
    goto LUpgradeMiner;

L_03c3:
    /* untranslated: call CheckAiShdefStatus(0xa, 0xb, cRecyclePeriod, &iLatestCargo, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x8, 0x9, cRecyclePeriod, &iLatestBomber, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x2, 0x4, cRecyclePeriod, &iLatestCruiser, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    /* untranslated: call CheckAiShdefStatus(0x5, ishLastBattle, cRecyclePeriod, &iLatestBattle, &part[0:1](rgRecycleShdef)) -> callresult(int16_t) */
    if ((game.turn <= 0x50))
        goto L_04ec;
    else
        goto L_0438;

L_0438:
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0xa);
    /* untranslated: part[0:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0xa);
    /* untranslated: part[1:1](rgRecycleSBShdef) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0xa);
    /* untranslated: part[1:1](l) = 0x2 */
    /* untranslated: part[0:1](l) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    memset(rgRecycleSBShdef, 0x0, 0xa);
    /* untranslated: part[1:1](fTonsOfMinerals) = 0x2 */
    /* untranslated: part[0:1](fTonsOfMinerals) = 0x2 */
    /* untranslated: call SplitOutShdefs(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */

L_04ec:
    EnsureMacintiShdefs();
    /* untranslated: call EnsureMacintiStarbaseDesigns(&part[0:1](rgRecycleSBShdef)) -> callresult(void) */
    vAiMacRecycleSB = rgRecycleSBShdef;
    fShouldColonize = FShouldWeBuildColonizers(&(cColFleet));
    /* untranslated: branch (part[207:2](rgshdef) & 0xff) != 0xf ? L_052a : L_0522 */

L_0522:
    iLatestColony = 0x1;
    goto L_0554;

L_052a:
    /* untranslated: branch ((part[1152:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_054f : L_053d */

L_053d:
    /* untranslated: branch part[1029:2](rgshdef) != 0xf ? L_054f : L_0547 */

L_0547:
    iLatestColony = 0x7;
    goto L_0554;

L_054f:
    iLatestColony = 0x1;

L_0554:
    if ((iLatestColony != 0x1))
        goto L_0579;
    else
        goto L_055d;

L_055d:
    if ((fUsingTempColonizer == 0x0))
        goto L_0579;
    else
        goto L_0566;

L_0566:
    /* untranslated: branch (game.turn - part[272:2](rgshdef)) <= 0x5 ? L_0579 : L_0575 */

L_0575:
    /* untranslated: part[7:1](rgRecycleShdef) = 0x2 */

L_0579:
    lpb = &(vlpbAiPlanet[0xe]);
    i = 0x0;
    goto L_05a5;

L_0594:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_05a5:
    if ((i >= game.cPlanMax))
        goto L_05ba;
    else
        goto L_05b0;

L_05b0:
    *(lpb) = 0x0;
    goto L_0594;

L_05ba:
    cFlMiners = 0x0;
    cFlCargo = 0x0;
    cFlDestroyers = 0x0;
    cFlArmadas = 0x0;
    cFlMineLayers = 0x0;
    ifl = 0x0;
    goto L_05e0;

L_05dc:
    ifl = (ifl + 0x1);

L_05e0:
    if ((ifl >= cFleet))
        goto L_0764;
    else
        goto L_05eb;

L_05eb:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_061b;
    else
        goto L_0613;

L_0613:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_0764;
    else
        goto L_061b;

L_061b:
    if ((lpfl->iPlayer != idPlayer))
        goto L_05dc;
    else
        goto L_062a;

L_062a:
    if ((iLatestMiner == 0xffff))
        goto L_0652;
    else
        goto L_0633;

L_0633:
    if ((lpfl->rgcsh[0xe] != 0x0))
        goto L_064d;
    else
        goto L_0640;

L_0640:
    if ((lpfl->rgcsh[0xf] == 0x0))
        goto L_0652;
    else
        goto L_064d;

L_064d:
    cFlMiners = (cFlMiners + 0x1);

L_0652:
    if ((lpfl->rgcsh[0x0] == 0x0))
        goto L_0663;
    else
        goto L_065f;

L_065f:
    cFlMineLayers = (cFlMineLayers + 0x1);

L_0663:
    if ((iLatestDestroyer == 0xffff))
        goto L_068a;
    else
        goto L_066c;

L_066c:
    if ((lpfl->rgcsh[0xc] != 0x0))
        goto L_0686;
    else
        goto L_0679;

L_0679:
    if ((lpfl->rgcsh[0xd] == 0x0))
        goto L_068a;
    else
        goto L_0686;

L_0686:
    cFlDestroyers = (cFlDestroyers + 0x1);

L_068a:
    if ((iLatestCargo == 0xffff))
        goto L_0728;
    else
        goto L_0693;

L_0693:
    if ((lpfl->rgcsh[0xa] != 0x0))
        goto L_06ad;
    else
        goto L_06a0;

L_06a0:
    if ((lpfl->rgcsh[0xb] == 0x0))
        goto L_0728;
    else
        goto L_06ad;

L_06ad:
    cFlCargo = (cFlCargo + 0x1);
    if ((lpfl->idPlanet != 0xffff))
        goto L_0728;
    else
        goto L_06be;

L_06be:
    if ((lpfl->cord <= 0x1))
        goto L_0728;
    else
        goto L_06cb;

L_06cb:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_0728 : L_06e6 */

L_06e6:
    if ((HIWORD(lpfl->rgwtMin[0x3]) < 0x0))
        goto L_0728;
    else
        goto L_06f3;

L_06f3:
    if ((HIWORD(lpfl->rgwtMin[0x3]) > 0x0))
        goto L_0702;
    else
        goto L_06f8;

L_06f8:
    if ((LOWORD(lpfl->rgwtMin[0x3]) <= 0x0))
        goto L_0728;
    else
        goto L_0702;

L_0702:
    vlpbAiPlanet[((lpfl->lpplord->rgord[0x1].id * 0x10) + 0xe)] = (vlpbAiPlanet[((lpfl->lpplord->rgord[0x1].id * 0x10) + 0xe)] | 0x1);

L_0728:
    i = 0x2;
    goto L_0758;

L_0730:
    if ((lpfl->rgcsh[i] == 0x0))
        goto L_0754;
    else
        goto L_074d;

L_074d:
    cFlArmadas = (cFlArmadas + 0x1);
    goto L_05dc;

L_0754:
    i = (i + 0x1);

L_0758:
    if ((i > 0x9))
        goto L_05dc;
    else
        goto L_075e;

L_075e:

L_0764:
    cFlMineLayersBase = cFlMineLayers;
    lpb = &(vlpbAiPlanet[0xd]);
    i = 0x0;
    goto L_0797;

L_0786:
    i = (i + 0x1);
    lpb = (lpb + 0x10);

L_0797:
    if ((i >= game.cPlanMax))
        goto L_07ac;
    else
        goto L_07a2;

L_07a2:
    *(lpb) = 0x0;
    goto L_0786;

L_07ac:
    ipl = 0x0;
    goto L_07b8;

L_07b4:
    ipl = (ipl + 0x1);

L_07b8:
    if ((ipl >= vclpplAi))
        goto L_0817;
    else
        goto L_07c3;

L_07c3:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_07f3;
    else
        goto L_07eb;

L_07eb:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_0817;
    else
        goto L_07f3;

L_07f3:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xd)] = 0x1;
    goto L_07b4;

L_0817:
    if ((game.turn <= 0x78))
        goto L_0867;
    else
        goto L_0821;

L_0821:
    if ((FLookupPartX(&(part), 0x8000, 0xd) != 0x1))
        goto L_0867;
    else
        goto L_083e;

L_083e:
    cGenesis = ((uint32_t)(rgplr[idPlayer].cPlanet) / 0x14);
    if ((cGenesis <= 0xa))
        goto L_086c;
    else
        goto L_085f;

L_085f:
    cGenesis = 0xa;

L_0867:
    cGenesis = 0x0;

L_086c:
    UpdateProgressGauge(0xfc62);
    lppl = lpPlanets;
    lpplMac = &(lpPlanets[cPlanet]);
    goto L_094e;

L_08a7:
    if ((lppl->iPlayer == idPlayer))
        goto L_094a;
    else
        goto L_08b6;

L_08b6:
    if ((lppl->iPlayer == 0xffff))
        goto L_094a;
    else
        goto L_08c3;

L_08c3:
    /* untranslated: i = ((words(lppl->uPopGuess, 0x0) / 0xfa) + 0x1) */
    if ((i <= 0x6))
        goto L_08e8;
    else
        goto L_08e3;

L_08e3:
    i = 0x6;

L_08e8:
    if ((lppl->fStarbase == 0x0))
        goto L_0903;
    else
        goto L_08ff;

L_08ff:
    i = (i + 0x1);

L_0903:
    vlpbAiPlanet[((lppl->id * 0x10) + 0xa)] = LOBYTE(i);
    vlpbAiPlanet[((lppl->id * 0x10) + 0x9)] = 0x1;

L_094a:
    lppl = (lppl + 0x1);

L_094e:
    if ((LOWORD(lppl) < LOWORD(lpplMac)))
        goto L_08a7;
    else
        goto L_095c;

L_095c:
    ipl = 0x0;
    goto L_0968;

L_0964:
    ipl = (ipl + 0x1);

L_0968:
    if ((ipl >= vclpplAi))
        goto L_1a11;
    else
        goto L_0973;

L_0973:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_09a3;
    else
        goto L_099b;

L_099b:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_1a11;
    else
        goto L_09a3;

L_09a3:
    if ((lppl->fStarbase == 0x0))
        goto L_0964;
    else
        goto L_09ba;

L_09ba:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto L_0964;
    else
        goto L_09c7;

L_09c7:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_09d7;
    else
        goto L_09cc;

L_09cc:
    if ((LOWORD(lppl->rgwtMin[0x3]) < 0xc8))
        goto L_0964;
    else
        goto L_09d7;

L_09d7:
    if ((lppl->isb == 0x0))
        goto L_0964;
    else
        goto L_09e9;

L_09e9:
    if ((lppl->isb != 0x1))
        goto L_0a08;
    else
        goto L_09fb;

L_09fb:
    if ((game.turn > 0x19))
        goto L_0964;
    else
        goto L_0a02;

L_0a02:

L_0a08:
    ChangeMainObjSel(grobjPlanet, lppl->id);
    if ((LOWORD(lppl->lpplprod) != 0x0))
        goto L_0a31;
    else
        goto L_0a27;

L_0a27:
    if ((HIWORD(lppl->lpplprod) == 0x0))
        goto L_0a4a;
    else
        goto L_0a31;

L_0a31:
    if ((lppl->lpplprod->iprodMac >= 0x18))
        goto L_0964;
    else
        goto L_0a44;

L_0a44:

L_0a4a:
    InitProduction(rgprod);
    fWrite = 0x0;
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_0a90;

L_0a7a:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_0a90:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_0b00;
    else
        goto L_0aa3;

L_0aa3:
    if ((lpprod->grobj != 0x2))
        goto L_0a7a;
    else
        goto L_0ac4;

L_0ac4:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_0a7a : L_0acc */

L_0acc:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) > 0x0))
        goto L_0a7a;
    else
        goto L_0aed;

L_0aed:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_0b00 : L_0af2 */

L_0af2:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0x10 ? L_0b00 : L_0af7 */

L_0af7:

L_0b00:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_0b22;
    else
        goto L_0b13;

L_0b13:
    FinishProduction(0x0);
    goto L_0964;

L_0b22:
    cRes = CResourcesAtPlanet(lppl, idPlayer);
    if ((game.turn <= 0x78))
        goto LTryCargo;
    else
        goto L_0b41;

L_0b41:
    if ((IWarpMAFromLppl(lppl, &(j)) < 0xa))
        goto LTryCargo;
    else
        goto L_0b5b;

L_0b5b:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto LTryCargo;
    else
        goto L_0b68;

L_0b68:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_0b78;
    else
        goto L_0b6d;

L_0b6d:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x2710))
        goto LTryCargo;
    else
        goto L_0b78;

L_0b78:
    if ((Random(0x4) != 0x0))
        goto LTryCargo;
    else
        goto L_0b8c;

L_0b8c:
    lpplBest = 0x0;
    lLeast = 0x186a0;
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_0bd9;

L_0bc3:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_0bd9:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_0c77;
    else
        goto L_0bec;

L_0bec:
    if ((lpprod->grobj != 0x1))
        goto L_0bc3;
    else
        goto L_0c0d;

L_0c0d:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_0bc3 : L_0c15 */

L_0c15:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) < 0x0))
        goto L_0bc3;
    else
        goto L_0c36;

L_0c36:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) > 0x0 ? L_0c43 : L_0c3b */

L_0c3b:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) < 0xe ? L_0bc3 : L_0c43 */

L_0c43:
    if (((HIWORD((uint32_t)((lpprod >> 0xa))) & 0x0) > 0x0))
        goto L_0bc3;
    else
        goto L_0c64;

L_0c64:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) < 0x0 ? L_0f5b : L_0c69 */

L_0c69:
    /* untranslated: branch (loword(callresult(uint32_t)) & 0x7f) <= 0x11 ? L_0f5b : L_0c6e */

L_0c6e:

L_0c77:
    j = Random(0x3);
    i = j;
    goto L_0c93;

L_0c8f:
    i = (i + 0x1);

L_0c93:
    if ((i >= (j + 0x3)))
        goto L_0cdc;
    else
        goto L_0ca1;

L_0ca1:
    if ((HIWORD(lppl->rgwtMin[((uint32_t)(i) % 0x3)]) < 0x0))
        goto L_0c8f;
    else
        goto L_0cc7;

L_0cc7:
    if ((HIWORD(lppl->rgwtMin[((uint32_t)(i) % 0x3)]) > 0x0))
        goto L_0cdc;
    else
        goto L_0ccc;

L_0ccc:
    if ((LOWORD(lppl->rgwtMin[((uint32_t)(i) % 0x3)]) > 0x1388))
        goto L_0cdc;
    else
        goto L_0cd3;

L_0cd3:

L_0cdc:
    if ((i == (j + 0x3)))
        goto LTryCargo;
    else
        goto L_0ce7;

L_0ce7:

L_0ced:
    i = ((uint32_t)(i) % 0x3);
    iplDest = 0x0;
    goto L_0d07;

L_0d02:
    iplDest = (iplDest + 0x1);

L_0d07:
    if ((iplDest >= vclpplAi))
        goto L_0e21;
    else
        goto L_0d13;

L_0d13:
    lpplDest = vrglpplAi[iplDest];
    if ((LOWORD(vrglpplAi[iplDest]) != 0x0))
        goto L_0d46;
    else
        goto L_0d3e;

L_0d3e:
    if ((*(vrglpplAi[iplDest] + 0x2) == 0x0))
        goto L_0e21;
    else
        goto L_0d46;

L_0d46:
    if ((HIWORD(lpplDest->rgwtMin[i]) > HIWORD(lLeast)))
        goto L_0d02;
    else
        goto L_0d6f;

L_0d6f:
    if ((HIWORD(lpplDest->rgwtMin[i]) < HIWORD(lLeast)))
        goto L_0d7c;
    else
        goto L_0d74;

L_0d74:
    if ((LOWORD(lpplDest->rgwtMin[i]) >= LOWORD(lLeast)))
        goto L_0d02;
    else
        goto L_0d7c;

L_0d7c:
    if ((IWarpMAFromLppl(lpplDest, &(j)) < 0xa))
        goto L_0d02;
    else
        goto L_0d98;

L_0d98:
    /* untranslated: l = LDistance2(words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x), words(rgptPlan[lpplDest->id].y, rgptPlan[lpplDest->id].x)) */
    if ((HIWORD(l) > 0x1))
        goto L_0d02;
    else
        goto L_0dd7;

L_0dd7:
    if ((HIWORD(l) < 0x1))
        goto L_0de7;
    else
        goto L_0ddc;

L_0ddc:
    if ((LOWORD(l) >= 0x6444))
        goto L_0d02;
    else
        goto L_0de7;

L_0de7:
    lLeast = lpplDest->rgwtMin[i];
    lpplBest = lpplDest;

L_0e21:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_0e35;
    else
        goto L_0e2b;

L_0e2b:
    if ((HIWORD(lpplBest) == 0x0))
        goto LTryCargo;
    else
        goto L_0e35;

L_0e35:
    if ((HIWORD(lLeast) > HIWORD((int32_t)((lppl->rgwtMin[i] / 0x5)))))
        goto LTryCargo;
    else
        goto L_0e68;

L_0e68:
    /* untranslated: branch HIWORD(lLeast) < hiword(callresult(int32_t)) ? L_0e76 : L_0e6d */

L_0e6d:
    /* untranslated: branch LOWORD(lLeast) >= loword(callresult(int32_t)) ? L_0f5b : L_0e76 */

L_0e76:
    l = (int32_t)((lppl->rgwtMin[i] / 0x5));
    if ((HIWORD(l) < 0x0))
        goto L_0ece;
    else
        goto L_0eb2;

L_0eb2:
    if ((HIWORD(l) > 0x0))
        goto L_0ec2;
    else
        goto L_0eb7;

L_0eb7:
    if ((LOWORD(l) <= 0x4e20))
        goto L_0ece;
    else
        goto L_0ec2;

L_0ec2:
    l = 0x4e20;

L_0ece:
    l = (int32_t)((l / 0x64));
    AddItemToQueue((i + 0xe), LOWORD(l), grobjPlanet, 0x1);
    FinishProduction(0x1);
    sel.pl.iWarpFling = 0x7;
    /* untranslated: part[198:2](sel) = ((part[198:2](sel) & 0xfc00) | ((lpplBest->id + 0x1) & 0x3ff)) */
    FLookupPlanet(0xffff, 0x49ee);
    goto L_0964;

LTryCargo:
    if ((iLatestCargo == 0xffff))
        goto L_0fc0;
    else
        goto L_0f64;

L_0f64:
    if ((cFlCargo >= 0x40))
        goto L_0fc0;
    else
        goto L_0f6d;

L_0f6d:
    if ((cFlCargo >= ((uint32_t)(rgplr[idPlayer].cPlanet) / 0x4)))
        goto L_0fc0;
    else
        goto L_0f8a;

L_0f8a:
    if ((Random(0x3) != 0x0))
        goto L_0fc0;
    else
        goto L_0f9e;

L_0f9e:
    cFlCargo = (cFlCargo + 0x1);
    AddItemToQueue(iLatestCargo, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

L_0fc0:
    if ((fShouldColonize == 0x0))
        goto L_0fe5;
    else
        goto L_0fc9;

L_0fc9:
    if ((cColFleet <= 0x28))
        goto L_0ffc;
    else
        goto L_0fd2;

L_0fd2:
    if ((game.turn > 0x78))
        goto L_0fe5;
    else
        goto L_0fdc;

L_0fdc:
    if ((cColFleet <= 0x64))
        goto L_0ffc;
    else
        goto L_0fe5;

L_0fe5:
    if ((Random(0x64) >= 0x8))
        goto TryShip2;
    else
        goto L_0ff6;

L_0ff6:

L_0ffc:
    if ((cColFleet < 0x32))
        goto L_1012;
    else
        goto L_1005;

L_1005:
    if ((game.turn > 0x78))
        goto TryShip2;
    else
        goto L_100c;

L_100c:

L_1012:
    i = 0x0;
    goto L_104f;

L_101a:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_104b;
    else
        goto L_103a;

L_103a:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto FinishProd;
    else
        goto L_103f;

L_103f:
    if ((LOWORD(lppl->rgwtMin[i]) < 0x1e))
        goto FinishProd;
    else
        goto L_1045;

L_1045:

L_104b:
    i = (i + 0x1);

L_104f:
    if ((i <= 0x2))
        goto L_101a;
    else
        goto L_1058;

L_1058:
    if ((FShouldPlanetBuildColonizer(lppl) == 0x0))
        goto TryShip2;
    else
        goto L_106b;

L_106b:

L_1071:
    cColFleet = (cColFleet + 0x1);
    AddItemToQueue(iLatestColony, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;
    if ((game.turn < 0x5))
        goto FinishProd;
    else
        goto L_109a;

L_109a:

L_10a0:
    l = (uint32_t)((lppl->rgwtMin[0x3] * (uint32_t)(PctTrueMaxGrowth(idPlayer))));
    if ((HIWORD(l) < 0x0))
        goto TryShip2;
    else
        goto L_10d1;

L_10d1:
    if ((HIWORD(l) > 0x0))
        goto L_10e1;
    else
        goto L_10d6;

L_10d6:
    if ((LOWORD(l) <= 0x8fc))
        goto TryShip2;
    else
        goto L_10e1;

L_10e1:
    if ((cRes <= 0x23))
        goto TryShip2;
    else
        goto L_10ea;

L_10ea:
    if ((iAiLvl <= 0x0))
        goto TryShip2;
    else
        goto L_10f3;

L_10f3:
    cColFleet = (cColFleet + 0x1);
    AddItemToQueue(iLatestColony, 0x1, grobjFleet, 0x1);
    if ((HIWORD(l) < 0x0))
        goto TryShip2;
    else
        goto L_1119;

L_1119:
    if ((HIWORD(l) > 0x0))
        goto L_1129;
    else
        goto L_111e;

L_111e:
    if ((LOWORD(l) <= 0xe10))
        goto TryShip2;
    else
        goto L_1129;

L_1129:
    if ((cRes <= 0x32))
        goto TryShip2;
    else
        goto L_1132;

L_1132:
    if ((iAiLvl <= 0x1))
        goto TryShip2;
    else
        goto L_113b;

L_113b:
    AddItemToQueue(iLatestColony, 0x1, grobjFleet, 0x1);

TryShip2:
    if ((iLatestMiner == 0xffff))
        goto TryShip2b;
    else
        goto L_115c;

L_115c:
    if ((cFlMiners >= 0x3c))
        goto TryShip2b;
    else
        goto L_1166;

L_1166:
    /* untranslated: branch part[133:2](rgshdef[iLatestMiner]) > 0x0 ? L_1315 : L_117b */

L_117b:
    /* untranslated: branch part[133:2](rgshdef[iLatestMiner]) < 0x0 ? L_118b : L_1180 */

L_1180:
    /* untranslated: branch part[131:2](rgshdef[iLatestMiner]) >= 0x1388 ? L_1315 : L_118b */

L_118b:
    if ((Random(0x2) != 0x0))
        goto TryShip2b;
    else
        goto L_119f;

L_119f:
    id = lppl->id;
    cMine = 0x0;
    ifl = 0x0;
    goto L_11c1;

L_11bd:
    ifl = (ifl + 0x1);

L_11c1:
    if ((ifl >= cFleet))
        goto L_1254;
    else
        goto L_11cc;

L_11cc:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_11fc;
    else
        goto L_11f4;

L_11f4:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_1254;
    else
        goto L_11fc;

L_11fc:
    if ((lpfl->idPlanet != id))
        goto L_11bd;
    else
        goto L_120c;

L_120c:
    if ((lpfl->rgcsh[iLatestMiner] <= 0x0))
        goto L_11bd;
    else
        goto L_1229;

L_1229:
    if ((lpfl->iPlayer != idPlayer))
        goto L_11bd;
    else
        goto L_1238;

L_1238:
    cMine = CMineFromLpfl(lpfl);
    goto L_1254;

L_1254:
    if ((HIWORD(cMine) < 0x0))
        goto L_1271;
    else
        goto L_125e;

L_125e:
    if ((HIWORD(cMine) > 0x0))
        goto TryShip2b;
    else
        goto L_1263;

L_1263:
    if ((LOWORD(cMine) > 0x3e8))
        goto TryShip2b;
    else
        goto L_126b;

L_126b:

L_1271:
    cFr = 0x0;
    i = 0x0;
    goto L_129d;

L_127e:
    cFr = (cFr + lppl->rgMinConc[i]);
    i = (i + 0x1);

L_129d:
    if ((i < 0x3))
        goto L_127e;
    else
        goto L_12a6;

L_12a6:
    cFr = LOWORD((0x3 * cFr));
    if ((HIWORD(cMine) > SIGNHIWORD(cFr)))
        goto L_12d4;
    else
        goto L_12bc;

L_12bc:
    if ((HIWORD(cMine) < SIGNHIWORD(cFr)))
        goto L_12ca;
    else
        goto L_12c1;

L_12c1:
    if ((LOWORD(cMine) >= cFr))
        goto L_12d4;
    else
        goto L_12ca;

L_12ca:
    if ((cFr > 0x96))
        goto L_12f2;
    else
        goto L_12d4;

L_12d4:
    if ((cFlMiners >= 0x1e))
        goto TryShip2b;
    else
        goto L_12de;

L_12de:
    if ((Random(0xa) == 0x0))
        goto TryShip2b;
    else
        goto L_12f2;

L_12f2:
    cFlMiners = (cFlMiners + 0x1);
    AddItemToQueue(iLatestMiner, 0x1, grobjFleet, 0x1);
    fWrite = 0x1;

TryShip2b:
    if ((LOWORD(rgshdef) != 0x5))
        goto L_1454;
    else
        goto L_131f;

L_131f:
    if ((cFlMineLayers >= 0x3c))
        goto L_1454;
    else
        goto L_1328;

L_1328:
    /* untranslated: branch part[133:2](rgshdef[iLatestMiner]) > 0x0 ? L_1454 : L_133d */

L_133d:
    /* untranslated: branch part[133:2](rgshdef[iLatestMiner]) < 0x0 ? L_134d : L_1342 */

L_1342:
    /* untranslated: branch part[131:2](rgshdef[iLatestMiner]) >= 0x1d4c ? L_1454 : L_134d */

L_134d:
    if ((Random(0x4) != 0x0))
        goto L_1454;
    else
        goto L_1361;

L_1361:
    id = lppl->id;
    cFr = 0x0;
    ifl = 0x0;
    goto L_137c;

L_1378:
    ifl = (ifl + 0x1);

L_137c:
    if ((ifl >= cFleet))
        goto L_13f3;
    else
        goto L_1387;

L_1387:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_13b7;
    else
        goto L_13af;

L_13af:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_13f3;
    else
        goto L_13b7;

L_13b7:
    if ((lpfl->idPlanet != id))
        goto L_1378;
    else
        goto L_13c7;

L_13c7:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_1378;
    else
        goto L_13d4;

L_13d4:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1378;
    else
        goto L_13e3;

L_13e3:
    cFr = lpfl->rgcsh[0x0];
    goto L_13f3;

L_13f3:
    if ((cFr < 0xa))
        goto L_1419;
    else
        goto L_13fc;

L_13fc:
    if ((cFr >= 0x11))
        goto L_1454;
    else
        goto L_1405;

L_1405:
    if ((Random(0xa) != 0x0))
        goto L_1454;
    else
        goto L_1419;

L_1419:
    /* untranslated: branch Random(part[1:0](mpicolgrbitBU[cFr])) != 0x0 ? L_1454 : L_1432 */

L_1432:
    cFlMineLayers = (cFlMineLayers + 0x3);
    AddItemToQueue(0x0, 0x4, grobjFleet, 0x1);
    fWrite = 0x1;

L_1454:
    i = 0x0;
    goto L_1492;

L_145c:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_148e;
    else
        goto L_147c;

L_147c:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_149b;
    else
        goto L_1481;

L_1481:
    if ((LOWORD(lppl->rgwtMin[i]) < 0x1388))
        goto L_149b;
    else
        goto L_1488;

L_1488:

L_148e:
    i = (i + 0x1);

L_1492:
    if ((i <= 0x2))
        goto L_145c;
    else
        goto L_149b;

L_149b:
    if ((i != 0x2))
        goto L_14aa;
    else
        goto L_14a4;

L_14a4:
    t_merge_14ad_0001 = 0x1;
    goto L_14ad;

L_14aa:
    t_merge_14ad_0001 = 0x0;

L_14ad:
    fTonsOfMinerals = t_merge_14ad_0001;
    if ((iLatestBomber == 0xffff))
        goto L_15e8;
    else
        goto L_14ba;

L_14ba:
    if ((cFlArmadas >= 0x8c))
        goto L_15e8;
    else
        goto L_14c4;

L_14c4:
    if ((cFlArmadas < 0x3c))
        goto L_14d7;
    else
        goto L_14cd;

L_14cd:
    if ((cRes <= 0x7d0))
        goto L_15e8;
    else
        goto L_14d7;

L_14d7:
    id = lppl->id;
    if ((cFlArmadas <= 0x6e))
        goto L_1501;
    else
        goto L_14ea;

L_14ea:
    if ((Random(0x3) == 0x0))
        goto LAddBombers;
    else
        goto L_14fb;

L_14fb:

L_1501:
    ifl = 0x0;
    goto L_150d;

L_1509:
    ifl = (ifl + 0x1);

L_150d:
    if ((ifl >= cFleet))
        goto L_15e8;
    else
        goto L_1518;

L_1518:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_1548;
    else
        goto L_1540;

L_1540:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_15e8;
    else
        goto L_1548;

L_1548:
    if ((lpfl->idPlanet != id))
        goto L_1509;
    else
        goto L_1558;

L_1558:
    if ((lpfl->iPlayer != idPlayer))
        goto L_1509;
    else
        goto L_1567;

L_1567:
    if ((FPotentMacWarFleet(lpfl, 0x0) == 0x0))
        goto L_1509;
    else
        goto L_1581;

L_1581:
    if ((iLatestBomber == 0xffff))
        goto L_15e8;
    else
        goto L_158a;

L_158a:
    if (((lpfl->rgcsh[0x8] + lpfl->rgcsh[0x9]) >= (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_15e8;
    else
        goto L_15aa;

L_15aa:

LAddBombers:
    cFlArmadas = (cFlArmadas + 0x2);
    if ((fTonsOfMinerals == 0x0))
        goto L_15cc;
    else
        goto L_15c6;

L_15c6:
    t_merge_15cf_0001 = 0xc;
    goto L_15cf;

L_15cc:
    t_merge_15cf_0001 = 0x4;

L_15cf:
    AddItemToQueue(iLatestBomber, t_merge_15cf_0001, grobjFleet, 0x1);
    fWrite = 0x1;
    goto FinishProd;

L_15e8:
    if ((cGenesis <= 0x0))
        goto LTryCruiser;
    else
        goto L_15f1;

L_15f1:
    if ((HIWORD(lppl->rgwtMin[0x3]) < 0x0))
        goto LTryCruiser;
    else
        goto L_15fe;

L_15fe:
    if ((HIWORD(lppl->rgwtMin[0x3]) > 0x0))
        goto L_160e;
    else
        goto L_1603;

L_1603:
    if ((LOWORD(lppl->rgwtMin[0x3]) <= 0x2710))
        goto LTryCruiser;
    else
        goto L_160e;

L_160e:
    i = 0x0;
    lpprod = &(lpplProdGlob->rgprod);
    goto L_1643;

L_162d:
    i = (i + 0x1);
    lpprod = (lpprod + 0x1);

L_1643:
    if ((i >= lpplProdGlob->iprodMac))
        goto L_16ae;
    else
        goto L_1656;

L_1656:
    if ((lpprod->grobj != 0x1))
        goto L_162d;
    else
        goto L_1677;

L_1677:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) != 0x0 ? L_162d : L_167f */

L_167f:
    if ((lpprod->iItem != 0xd))
        goto L_162d;
    else
        goto L_16a0;

L_16a0:
    /* untranslated: branch (hiword(callresult(uint32_t)) & 0x0) == 0x0 ? L_16ae : L_16a5 */

L_16a5:

L_16ae:
    if ((i < lpplProdGlob->iprodMac))
        goto LTryCruiser;
    else
        goto L_16be;

L_16be:

L_16c4:
    i = 0x0;
    goto L_1702;

L_16cc:
    if ((HIWORD(lppl->rgwtMin[i]) > 0x0))
        goto L_16fe;
    else
        goto L_16ec;

L_16ec:
    if ((HIWORD(lppl->rgwtMin[i]) < 0x0))
        goto L_170b;
    else
        goto L_16f1;

L_16f1:
    if ((LOWORD(lppl->rgwtMin[i]) < 0x7d0))
        goto L_170b;
    else
        goto L_16f8;

L_16f8:

L_16fe:
    i = (i + 0x1);

L_1702:
    if ((i <= 0x2))
        goto L_16cc;
    else
        goto L_170b;

L_170b:
    if ((i == 0x2))
        goto LTryCruiser;
    else
        goto L_1711;

L_1711:

L_1717:
    cConc = 0x0;
    i = 0x0;
    goto L_1745;

L_1725:
    cConc = (cConc + lppl->rgMinConc[i]);
    i = (i + 0x1);

L_1745:
    if ((i < 0x3))
        goto L_1725;
    else
        goto L_174e;

L_174e:
    if ((cConc < 0xf))
        goto L_1794;
    else
        goto L_1758;

L_1758:
    if ((cConc >= 0x1e))
        goto L_1776;
    else
        goto L_1762;

L_1762:
    if ((Random(0x3) != 0x0))
        goto L_1794;
    else
        goto L_1776;

L_1776:
    if ((cConc >= 0x3c))
        goto LTryCruiser;
    else
        goto L_1780;

L_1780:
    if ((Random(0x5) == 0x0))
        goto LTryCruiser;
    else
        goto L_1794;

L_1794:
    cGenesis = (cGenesis - 0x1);
    AddItemToQueue(0xd, 0x1, grobjPlanet, 0x1);
    AddItemToQueue(0xc, 0x4b, grobjPlanet, 0x1);
    fWrite = 0x1;

LTryCruiser:
    if ((iLatestCruiser == 0xffff))
        goto TryShip3;
    else
        goto L_17d7;

L_17d7:
    if ((cFlArmadas >= 0x82))
        goto TryShip3;
    else
        goto L_17e1;

L_17e1:
    if ((game.turn <= 0x14))
        goto TryShip3;
    else
        goto L_17eb;

L_17eb:
    if ((cFlArmadas < 0x32))
        goto L_17fe;
    else
        goto L_17f4;

L_17f4:
    if ((cRes <= 0x7d0))
        goto TryShip3;
    else
        goto L_17fe;

L_17fe:
    if ((fTonsOfMinerals != 0x0))
        goto L_181f;
    else
        goto L_1808;

L_1808:
    if ((Random(0x3) != 0x0))
        goto TryShip3;
    else
        goto L_1819;

L_1819:

L_181f:
    if ((iLatestBattle == 0xffff))
        goto L_1846;
    else
        goto L_1828;

L_1828:
    if ((Random(0x3) != 0x0))
        goto L_1846;
    else
        goto L_183c;

L_183c:
    iLatest = iLatestBattle;
    goto L_184d;

L_1846:
    iLatest = iLatestCruiser;

L_184d:
    cFlArmadas = (cFlArmadas + 0x1);
    if ((fTonsOfMinerals == 0x0))
        goto L_1869;
    else
        goto L_1863;

L_1863:
    t_merge_186c_0001 = 0xa;
    goto L_186c;

L_1869:
    t_merge_186c_0001 = 0x2;

L_186c:
    AddItemToQueue(iLatest, t_merge_186c_0001, grobjFleet, 0x1);
    fWrite = 0x1;
    if ((fTonsOfMinerals != 0x0))
        goto FinishProd;
    else
        goto L_1887;

L_1887:

TryShip3:
    if ((iLatestDestroyer == 0xffff))
        goto FinishProd;
    else
        goto L_1896;

L_1896:
    if ((game.turn >= 0x78))
        goto L_18a6;
    else
        goto L_18a0;

L_18a0:
    t_merge_18a9_0001 = 0x50;
    goto L_18a9;

L_18a6:
    t_merge_18a9_0001 = 0x3c;

L_18a9:
    if ((cFlDestroyers >= t_merge_18a9_0001))
        goto FinishProd;
    else
        goto L_18b1;

L_18b1:
    if ((cshDestroyer >= 0x7d0))
        goto FinishProd;
    else
        goto L_18bb;

L_18bb:
    GetResourcesAvailable(lppl, rgResAvail);
    GetProdQCost(lppl, rgResCost);
    i = 0x0;
    goto L_1932;

L_18e7:
    rgResAvail[i] = (rgResAvail[i] - rgResCost[i]);
    if ((HIWORD(rgResAvail[i]) > 0x0))
        goto L_192e;
    else
        goto L_191e;

L_191e:
    if ((HIWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_1923;

L_1923:
    if ((LOWORD(rgResAvail[i]) < 0x0))
        goto FinishProd;
    else
        goto L_1928;

L_1928:

L_192e:
    i = (i + 0x1);

L_1932:
    if ((i < 0x4))
        goto L_18e7;
    else
        goto L_193b;

L_193b:
    GetTrueHullCost(idPlayer, &(rgshdef[iLatestDestroyer]), &(LOWORD(rgCosts)));
    i = 0x0;
    goto L_19ce;

L_1963:
    j = 0x0;
    goto L_19b5;

L_196b:
    LOWORD(rgResAvail[j]) = (LOWORD(rgResAvail[j]) - rgCosts[j]);
    HIWORD(rgResAvail[j]) = (HIWORD(rgResAvail[j]) - 0x0);
    if ((HIWORD(rgResAvail[j]) > 0x0))
        goto L_19b1;
    else
        goto L_19a1;

L_19a1:
    if ((HIWORD(rgResAvail[j]) < 0x0))
        goto L_19be;
    else
        goto L_19a6;

L_19a6:
    if ((LOWORD(rgResAvail[j]) < 0x0))
        goto L_19be;
    else
        goto L_19ab;

L_19ab:

L_19b1:
    j = (j + 0x1);

L_19b5:
    if ((j < 0x4))
        goto L_196b;
    else
        goto L_19be;

L_19be:
    if ((j < 0x4))
        goto L_19d7;
    else
        goto L_19c4;

L_19c4:

L_19ca:
    i = (i + 0x1);

L_19ce:
    if ((i < 0x14))
        goto L_1963;
    else
        goto L_19d7;

L_19d7:
    if ((i <= 0x0))
        goto FinishProd;
    else
        goto L_19e0;

L_19e0:
    fWrite = 0x1;
    AddItemToQueue(iLatestDestroyer, i, grobjFleet, 0x1);
    cFlDestroyers = (cFlDestroyers + 0x1);

FinishProd:
    FinishProduction(fWrite);
    goto L_0964;

L_1a11:
    UpdateProgressGauge(0xfc62);
    lpflAttack = 0x0;
    lpflEnemy = 0x0;
    ifl = 0x0;
    goto L_1a3f;

L_1a3b:
    ifl = (ifl + 0x1);

L_1a3f:
    if ((ifl >= cFleet))
        goto L_250a;
    else
        goto L_1a4a;

L_1a4a:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_1a7a;
    else
        goto L_1a72;

L_1a72:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_250a;
    else
        goto L_1a7a;

L_1a7a:
    if ((lpfl->cord <= 0x1))
        goto L_1b5b;
    else
        goto L_1a87;

L_1a87:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x8 ? L_1b5b : L_1aa2 */

L_1aa2:
    dx = (lpfl->pt.x - lpfl->lpplord->rgord[0x1].pt.x);
    dy = (lpfl->pt.y - lpfl->lpplord->rgord[0x1].pt.y);
    lDist = ((uint32_t)(((uint32_t)(dx) * (uint32_t)(dx))) + (uint32_t)(((uint32_t)(dy) * (uint32_t)(dy))));
    if ((HIWORD(lDist) < 0x0))
        goto L_1b5b;
    else
        goto L_1b1a;

L_1b1a:
    if ((HIWORD(lDist) > 0x0))
        goto L_1b2a;
    else
        goto L_1b1f;

L_1b1f:
    if ((LOWORD(lDist) <= 0x9c40))
        goto L_1b5b;
    else
        goto L_1b2a;

L_1b2a:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);

L_1b5b:
    if ((lpfl->iPlayer == idPlayer))
        goto L_1b8d;
    else
        goto L_1b6a;

L_1b6a:
    lpfl->lpflNext = lpflEnemy;
    lpflEnemy = lpfl;
    goto L_1a3b;

L_1b8d:
    if ((lpfl->rgcsh[0x0] <= 0x0))
        goto L_1d75;
    else
        goto L_1b9a;

L_1b9a:
    if ((game.turn <= 0x28))
        goto L_1d75;
    else
        goto L_1ba4;

L_1ba4:
    if ((lpfl->cord <= 0x1))
        goto L_1bdc;
    else
        goto L_1bb1;

L_1bb1:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x0 ? L_1a3b : L_1bc7 */

L_1bc7:
    ClearAiCurrentTask(lpfl, 0x1);

L_1bdc:
    if ((cFlMineLayersBase > 0x37))
        goto L_1c04;
    else
        goto L_1be6;

L_1be6:
    if ((cFlMineLayersBase <= 0x28))
        goto L_1c35;
    else
        goto L_1bf0;

L_1bf0:
    if ((Random(0x3) == 0x0))
        goto L_1c35;
    else
        goto L_1c04;

L_1c04:
    if ((FFindBuddyAndJoinUp(lpfl, 0x0, 0x0, 0x48, 0x6c) != 0x0))
        goto L_1a3b;
    else
        goto L_1c2f;

L_1c2f:

L_1c35:
    if ((lpfl->rgcsh[0x0] < 0x7))
        goto L_1d0d;
    else
        goto L_1c42;

L_1c42:
    if ((Random(0x5) != 0x0))
        goto L_1d0d;
    else
        goto L_1c56;

L_1c56:
    /* untranslated: call IdRandomPlanetNearby(words(lpfl->pt.y, lpfl->pt.x), 0x69, 0x1) -> callresult(int16_t) */
    /* untranslated: idPlanDst = callresult(int16_t) */
    /* untranslated: branch callresult(int16_t) == 0xffff ? L_1d0d : L_1c7c */

L_1c7c:
    if ((idPlanDst == lpfl->idPlanet))
        goto L_1d0d;
    else
        goto L_1c8b;

L_1c8b:
    ClearAiCurrentTask(lpfl, 0x1);
    ord.id = idPlanDst;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[idPlanDst].x;
    ord.pt.y = rgptPlan[idPlanDst].y;
    ord.grTask = grTaskLayMines;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    FMoveAiFleet(lpfl, &(ord), 0x0);
    goto L_2154;

L_1d0d:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x6 ? L_2154 : L_1d23 */

L_1d23:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x6) */
    /* untranslated: part[8:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    /* untranslated: part[10:2](sel.fl.lpplord->rgord[0x0]) = 0x5 */
    FLookupFleet(0xffff, 0x4972);
    goto L_1a3b;

L_1d75:
    if ((lpfl->rgcsh[0xe] > 0x0))
        goto L_1d8f;
    else
        goto L_1d82;

L_1d82:
    if ((lpfl->rgcsh[0xf] <= 0x0))
        goto L_1f18;
    else
        goto L_1d8f;

L_1d8f:
    if ((lpfl->cord != 0x1))
        goto L_1f18;
    else
        goto L_1d9c;

L_1d9c:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) == 0x3 ? L_1ded : L_1db2 */

L_1db2:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x3) */
    FLookupFleet(0xffff, 0x4972);
    goto L_1a3b;

L_1ded:
    if ((cFlMiners > 0x3a))
        goto L_1e15;
    else
        goto L_1df7;

L_1df7:
    if ((cFlMiners <= 0x30))
        goto L_1e46;
    else
        goto L_1e01;

L_1e01:
    if ((Random(0x3) == 0x0))
        goto L_1e46;
    else
        goto L_1e15;

L_1e15:
    if ((FFindBuddyAndJoinUp(lpfl, 0xe, 0xf, 0x48, 0x6c) != 0x0))
        goto L_1a3b;
    else
        goto L_1e40;

L_1e40:

L_1e46:
    if ((lpfl->idPlanet == 0xffff))
        goto L_1ef3;
    else
        goto L_1e53;

L_1e53:
    LpplFromId(lpfl->idPlanet);
    /* untranslated: lppl = callresult(PLANET *) */
    /* untranslated: branch faroff(callresult(PLANET *)) != 0x0 ? L_1e78 : L_1e70 */

L_1e70:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_1ef3 : L_1e78 */

L_1e78:
    if ((lppl->iPlayer != idPlayer))
        goto L_1ef3;
    else
        goto L_1e87;

L_1e87:
    j = 0x0;
    i = 0x0;
    goto L_1eb3;

L_1e94:
    j = (j + lppl->rgMinConc[i]);
    i = (i + 0x1);

L_1eb3:
    if ((i < 0x3))
        goto L_1e94;
    else
        goto L_1ebc;

L_1ebc:
    if ((j < 0x1e))
        goto L_1ee2;
    else
        goto L_1ec5;

L_1ec5:
    if ((j >= 0x3c))
        goto L_1ef3;
    else
        goto L_1ece;

L_1ece:
    if ((Random(0x3) != 0x0))
        goto L_1ef3;
    else
        goto L_1ee2;

L_1ee2:
    FRetargetMiner(lpfl);
    goto L_1a3b;

L_1ef3:
    if ((Random(0xa) != 0x0))
        goto L_2154;
    else
        goto L_1f07;

L_1f07:
    FRetargetMiner(lpfl);

L_1f18:
    if ((FIsAiAttack(lpfl) == 0x0))
        goto L_204b;
    else
        goto L_1f2e;

L_1f2e:
    lpfl->lpflNext = lpflAttack;
    lpflAttack = lpfl;
    j = 0x2;
    goto L_1f7b;

L_1f57:
    if ((lpfl->rgcsh[j] > 0x0))
        goto L_1f86;
    else
        goto L_1f71;

L_1f71:

L_1f77:
    j = (j + 0x1);

L_1f7b:
    if ((j <= ishLastBattle))
        goto L_1f57;
    else
        goto L_1f86;

L_1f86:
    if ((j > ishLastBattle))
        goto L_2154;
    else
        goto L_1f91;

L_1f91:
    if ((lpfl->cord <= 0x1))
        goto L_1fb9;
    else
        goto L_1f9e;

L_1f9e:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_1fc6 : L_1fb9 */

L_1fb9:
    if ((lpfl->idPlanet == 0xffff))
        goto L_2154;
    else
        goto L_1fc6;

L_1fc6:
    if ((lpfl->cord <= 0x1))
        goto L_2000;
    else
        goto L_1fd3;

L_1fd3:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_2000 : L_1fee */

L_1fee:
    id = lpfl->lpplord->rgord[0x1].id;
    goto L_200b;

L_2000:
    id = lpfl->idPlanet;

L_200b:
    lpb = ((uint8_t *)(vlpbAiPlanet) + ((id * 0x10) + 0xa));
    if ((*(lpb) == 0x0))
        goto L_2154;
    else
        goto L_2040;

L_2040:
    *(lpb) = (*(lpb) | 0x80);

L_204b:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_2154;
    else
        goto L_2061;

L_2061:
    idPlanDst = 0xffff;
    if ((lpfl->cord <= 0x1))
        goto L_20b5;
    else
        goto L_2073;

L_2073:
    /* untranslated: branch (part[6:2](lpfl->lpplord->rgord[0x0]) & 0xf) != 0x0 ? L_20b5 : L_2089 */

L_2089:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) != 0x1 ? L_20bf : L_20a4 */

L_20a4:
    idPlanDst = lpfl->lpplord->rgord[0x1].id;

L_20b5:
    idPlanDst = lpfl->idPlanet;

L_20bf:
    if ((idPlanDst == 0xffff))
        goto L_2154;
    else
        goto L_20c8;

L_20c8:
    lppl = LpplFromId(idPlanDst);
    if ((LOWORD(lppl) != 0x0))
        goto L_20eb;
    else
        goto L_20e2;

L_20e2:
    if ((HIWORD(lppl) == 0x0))
        goto L_20fa;
    else
        goto L_20eb;

L_20eb:
    if ((lppl->iPlayer == idPlayer))
        goto L_2154;
    else
        goto L_20fa;

L_20fa:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_2154;
    else
        goto L_2107;

L_2107:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_2154;
    else
        goto L_2111;

L_2111:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);
    ClearAiCurrentTask(lpfl, 0x0);

L_2154:
    if ((game.turn > 0xa))
        goto L_21b3;
    else
        goto L_215e;

L_215e:
    if ((lpfl->rgcsh[0x0] > 0x0))
        goto LRecycle;
    else
        goto L_216b;

L_216b:
    if ((lpfl->rgcsh[0x2] == 0x0))
        goto L_21b3;
    else
        goto LRecycle;

LRecycle:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_1a3b;

L_21b3:
    if ((lpfl->cord <= 0x1))
        goto L_222a;
    else
        goto L_21c0;

L_21c0:
    if ((fUsingTempColonizer == 0x0))
        goto L_1a3b;
    else
        goto L_21c9;

L_21c9:
    /* untranslated: branch part[7:1](rgRecycleShdef) == 0x0 ? L_1a3b : L_21d7 */

L_21d7:
    if ((lpfl->rgcsh[0x7] == 0x0))
        goto L_1a3b;
    else
        goto L_21e4;

L_21e4:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.cord = 0x1;
    sel.fl.lpplord->iordMac = 0x1;
    FLookupFleet(0xffff, 0x4972);
    FMoveToNearestStarbase(lpfl, 0x0);

L_222a:
    if ((lpfl->rgcsh[0x1] != 0x0))
        goto L_2250;
    else
        goto L_2237;

L_2237:
    if ((fUsingTempColonizer == 0x0))
        goto L_1a3b;
    else
        goto L_2240;

L_2240:
    if ((lpfl->rgcsh[0x7] == 0x0))
        goto L_1a3b;
    else
        goto L_224a;

L_224a:

L_2250:
    if ((fUsingTempColonizer == 0x0))
        goto L_2272;
    else
        goto L_2259;

L_2259:
    if ((iLatestColony != 0x1))
        goto L_2272;
    else
        goto L_2262;

L_2262:
    if ((lpfl->idPlanet != 0xffff))
        goto LRecycle;
    else
        goto L_226c;

L_226c:

L_2272:
    if ((iAiLvl <= 0x1))
        goto L_2388;
    else
        goto L_227b;

L_227b:
    if ((lpfl->idPlanet == 0xffff))
        goto L_2388;
    else
        goto L_2288;

L_2288:
    lpplDrop = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lpplDrop) != 0x0))
        goto L_22b3;
    else
        goto L_22a9;

L_22a9:
    if ((HIWORD(lpplDrop) == 0x0))
        goto L_22ff;
    else
        goto L_22b3;

L_22b3:
    if ((lpplDrop->iPlayer == 0xffff))
        goto L_22ff;
    else
        goto L_22c1;

L_22c1:
    if ((lpplDrop->iPlayer == idPlayer))
        goto L_22ff;
    else
        goto L_22d1;

L_22d1:
    if ((lpplDrop->fStarbase != 0x0))
        goto L_22ff;
    else
        goto L_22e9;

L_22e9:
    if ((lpplDrop->uPopGuess < 0x32))
        goto L_1a3b;
    else
        goto L_22f9;

L_22f9:

L_22ff:
    if ((LOWORD(lpplDrop) != 0x0))
        goto L_2313;
    else
        goto L_2309;

L_2309:
    if ((HIWORD(lpplDrop) == 0x0))
        goto L_2388;
    else
        goto L_2313;

L_2313:
    if ((lpplDrop->iPlayer != 0xffff))
        goto L_2388;
    else
        goto L_2321;

L_2321:
    if ((LOWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_234d;
    else
        goto L_232e;

L_232e:
    if ((HIWORD(lpfl->rgwtMin[0x3]) != 0x0))
        goto L_234d;
    else
        goto L_2338;

L_2338:
    FMoveToNearestStarbase(lpfl, 0x1);
    goto L_1a3b;

L_234d:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x2) */
    FLookupFleet(0xffff, 0x4972);
    goto L_1a3b;

L_2388:
    idPlanDst = IdNearestColonizablePlanet(lpfl, &(lpthWorm));
    if ((idPlanDst != 0xffff))
        goto L_2400;
    else
        goto L_23a6;

L_23a6:
    if ((LOWORD(lpthWorm) != 0x0))
        goto L_2400;
    else
        goto L_23af;

L_23af:
    if ((HIWORD(lpthWorm) != 0x0))
        goto L_2400;
    else
        goto L_23b8;

L_23b8:
    if ((lpfl->idPlanet == 0xffff))
        goto L_1a3b;
    else
        goto L_23c5;

L_23c5:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);

L_2400:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet == 0xffff))
        goto L_24d6;
    else
        goto L_241f;

L_241f:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lppl) != 0x0))
        goto L_2446;
    else
        goto L_243d;

L_243d:
    if ((HIWORD(lppl) == 0x0))
        goto L_248e;
    else
        goto L_2446;

L_2446:
    l = (int32_t)((lppl->rgwtMin[0x3] / 0xa));
    if ((HIWORD(l) < 0x0))
        goto L_249a;
    else
        goto L_2470;

L_2470:
    if ((HIWORD(l) > 0x0))
        goto L_247f;
    else
        goto L_2475;

L_2475:
    if ((LOWORD(l) <= 0x19))
        goto L_249a;
    else
        goto L_247f;

L_247f:
    l = 0x19;

L_248e:
    l = 0x0;

L_249a:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, LOWORD(l));
    FLookupFleet(LOWORD(lpfl), 0x4972);

L_24d6:
    if ((idPlanDst == 0xffff))
        goto L_24f3;
    else
        goto L_24df;

L_24df:
    FColonizeAiFleet(lpfl, idPlanDst);
    goto L_1a3b;

L_24f3:
    FGotoWormholeAiFleet(lpfl, lpthWorm);

L_250a:
    ifl = 0x0;
    goto L_2516;

L_2512:
    ifl = (ifl + 0x1);

L_2516:
    if ((ifl >= cFleet))
        goto L_25d3;
    else
        goto L_2521;

L_2521:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2551;
    else
        goto L_2549;

L_2549:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_25d3;
    else
        goto L_2551;

L_2551:
    if ((lpfl->iPlayer != idPlayer))
        goto L_2512;
    else
        goto L_2560;

L_2560:
    if ((lpfl->cord > 0x1))
        goto L_2512;
    else
        goto L_256a;

L_256a:

L_2570:
    if ((FIsAiTransport(lpfl) == 0x0))
        goto L_2512;
    else
        goto L_2583;

L_2583:

L_2589:
    if ((lpfl->iplan == 0x4))
        goto L_25c2;
    else
        goto L_259b;

L_259b:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    sel.fl.iplan = 0x4;
    FLookupFleet(0xffff, 0x4972);

L_25c2:
    IdTargetMacFreighter(lpfl);
    goto L_2512;

L_25d3:
    UpdateProgressGauge(0xfc62);
    ifl = 0x0;
    goto L_25eb;

L_25e7:
    ifl = (ifl + 0x1);

L_25eb:
    if ((ifl >= cFleet))
        goto L_29e0;
    else
        goto L_25f6;

L_25f6:
    lpfl = rglpfl[ifl];
    if ((LOWORD(rglpfl[ifl]) != 0x0))
        goto L_2626;
    else
        goto L_261e;

L_261e:
    if ((*(rglpfl[ifl] + 0x2) == 0x0))
        goto L_29e0;
    else
        goto L_2626;

L_2626:
    if ((lpfl->iPlayer != idPlayer))
        goto L_25e7;
    else
        goto L_2632;

L_2632:

L_2638:
    i = 0x0;
    goto L_2679;

L_2640:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_2675;
    else
        goto L_265d;

L_265d:
    if ((rgRecycleShdef[i] == 0x0))
        goto L_2682;
    else
        goto L_266f;

L_266f:

L_2675:
    i = (i + 0x1);

L_2679:
    if ((i < 0x10))
        goto L_2640;
    else
        goto L_2682;

L_2682:
    if ((i != 0x10))
        goto L_2787;
    else
        goto L_268b;

L_268b:
    if ((lpfl->idPlanet == 0xffff))
        goto L_2735;
    else
        goto L_2698;

L_2698:
    LpplFromId(lpfl->idPlanet);
    /* untranslated: lppl = callresult(PLANET *) */
    /* untranslated: branch faroff(callresult(PLANET *)) != 0x0 ? L_26bd : L_26b5 */

L_26b5:
    /* untranslated: branch farseg(callresult(PLANET *)) == 0x0 ? L_2735 : L_26bd */

L_26bd:
    if ((lppl->iPlayer != idPlayer))
        goto L_2735;
    else
        goto L_26cc;

L_26cc:
    if ((lppl->fStarbase != 0x0))
        goto L_26f7;
    else
        goto L_26e3;

L_26e3:
    if ((Random(0x5) != 0x0))
        goto L_2735;
    else
        goto L_26f7;

L_26f7:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    /* untranslated: part[6:2](sel.fl.lpplord->rgord[0x0]) = ((part[6:2](sel.fl.lpplord->rgord[0x0]) & 0xfff0) | 0x5) */
    FLookupFleet(0xffff, 0x4972);
    goto L_25e7;

L_2735:
    if ((lpfl->cord <= 0x1))
        goto L_276a;
    else
        goto L_2742;

L_2742:
    if ((lpfl->idPlanet != 0xffff))
        goto L_276a;
    else
        goto L_274f;

L_274f:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x1 ? L_25e7 : L_276a */

L_276a:
    if ((FMoveToNearestStarbase(lpfl, 0x0) != 0x0))
        goto L_25e7;
    else
        goto L_2781;

L_2781:

L_2787:
    i = 0x2;
    goto L_28ad;

L_278f:
    if ((lpfl->rgcsh[i] <= 0x0))
        goto L_28a9;
    else
        goto L_27ac;

L_27ac:
    if ((i != 0x7))
        goto L_27be;
    else
        goto L_27b5;

L_27b5:
    if ((fUsingTempColonizer != 0x0))
        goto L_28a9;
    else
        goto L_27be;

L_27be:
    if ((cFlArmadas > 0x64))
        goto L_27e4;
    else
        goto L_27c7;

L_27c7:
    if ((cFlArmadas <= 0x5a))
        goto L_2898;
    else
        goto L_27d0;

L_27d0:
    if ((Random(0x3) != 0x0))
        goto L_2898;
    else
        goto L_27e4;

L_27e4:
    l = 0x0;
    j = 0x2;
    goto L_281c;

L_27f8:
    l = (l + (uint32_t)(lpfl->rgcsh[j]));
    j = (j + 0x1);

L_281c:
    if ((j <= 0x9))
        goto L_27f8;
    else
        goto L_2825;

L_2825:
    if ((SIGNHIWORD(Random(0x64)) > (HIWORD(l) + 0xffff)))
        goto L_2867;
    else
        goto L_2847;

L_2847:
    /* untranslated: branch signhiword(callresult(int16_t)) < (HIWORD(l) + 0xffff) ? L_2853 : L_284c */

L_284c:
    /* untranslated: branch callresult(int16_t) > (LOWORD(l) + 0xfff6) ? L_2867 : L_2853 */

L_2853:
    if ((Random(0x14) != 0x0))
        goto L_2898;
    else
        goto L_2867;

L_2867:
    if ((FFindBuddyAndJoinUp(lpfl, 0x2, 0x9, 0x64, 0xc8) != 0x0))
        goto L_28b6;
    else
        goto L_2892;

L_2892:

L_2898:
    TargetMacArmada(lpfl);
    goto L_28b6;

L_28a9:
    i = (i + 0x1);

L_28ad:
    if ((i <= 0x9))
        goto L_278f;
    else
        goto L_28b6;

L_28b6:
    if ((i <= 0x9))
        goto L_25e7;
    else
        goto L_28bc;

L_28bc:

L_28c2:
    if ((FIsAiAttack(lpfl) == 0x0))
        goto L_25e7;
    else
        goto L_28d5;

L_28d5:

L_28db:
    if ((lpfl->cord <= 0x1))
        goto L_2906;
    else
        goto L_28e8;

L_28e8:
    /* untranslated: branch ((part[6:2](lpfl->lpplord->rgord[0x1]) >> 0x8) & 0xf) == 0x2 ? L_25e7 : L_2900 */

L_2900:

L_2906:
    if ((game.turn <= 0x78))
        goto L_2916;
    else
        goto L_2910;

L_2910:
    t_merge_2919_0001 = 0x32;
    goto L_2919;

L_2916:
    t_merge_2919_0001 = 0x46;

L_2919:
    if ((cFlDestroyers > t_merge_2919_0001))
        goto L_2950;
    else
        goto L_2921;

L_2921:
    if ((game.turn <= 0x78))
        goto L_2931;
    else
        goto L_292b;

L_292b:
    t_merge_2934_0001 = 0x28;
    goto L_2934;

L_2931:
    t_merge_2934_0001 = 0x3c;

L_2934:
    if ((cFlDestroyers <= t_merge_2934_0001))
        goto L_29b2;
    else
        goto L_293c;

L_293c:
    if ((Random(0x3) != 0x0))
        goto L_29b2;
    else
        goto L_2950;

L_2950:
    if ((lpfl->rgcsh[iLatestDestroyer] < 0x14))
        goto L_2981;
    else
        goto L_296d;

L_296d:
    if ((Random(0x14) != 0x0))
        goto L_29b2;
    else
        goto L_2981;

L_2981:
    if ((FFindBuddyAndJoinUp(lpfl, 0xc, 0xd, 0x24, 0x48) != 0x0))
        goto L_25e7;
    else
        goto L_29ac;

L_29ac:

L_29b2:
    IdTargetAttack(lpfl, lpflAttack, lpflEnemy, game.fAisBand);
    goto L_25e7;

L_29e0:
    HandleBasicAiTasks(iroCur, rgprod, 0x0, rgResAvail, rgResCost);
    FillProductionQueue();
    return;
}

void EnsureMacintiShdefs() {
    int16_t  ish;
    int16_t  i;
    PART     part;
    int16_t  fAdvanced;
    SHDEF    shdef;
    int16_t  shBase;
    uint16_t t_merge_2b93_0001;
    uint16_t t_merge_2beb_0001;
    uint16_t t_merge_30b7_0001;
    uint16_t t_merge_30e0_0001;
    uint16_t t_merge_3236_0001;

L_2b3c:
    ish = 0xe;
    goto L_2c55;

L_2b4d:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_2c51 : L_2b6b */

L_2b6b:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) < 0x2 ? L_2b90 : L_2b8a */

L_2b8a:
    t_merge_2b93_0001 = 0x1;
    goto L_2b93;

L_2b90:
    t_merge_2b93_0001 = 0x0;

L_2b93:
    fAdvanced = t_merge_2b93_0001;
    if ((fAdvanced == 0x0))
        goto L_2bc3;
    else
        goto L_2b9f;

L_2b9f:
    if ((ish != 0xf))
        goto L_2bc3;
    else
        goto L_2ba8;

L_2ba8:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0xf))
        goto L_2c51;
    else
        goto L_2bbd;

L_2bbd:

L_2bc3:
    if ((fAdvanced != 0x0))
        goto L_2be8;
    else
        goto L_2be2;

L_2be2:
    t_merge_2beb_0001 = 0x1;
    goto L_2beb;

L_2be8:
    t_merge_2beb_0001 = 0x0;

L_2beb:
    /* untranslated: branch FCreateAiShdef(ish, (0x18 - t_merge_2beb_0001), &vrgMacAip[0x15:[((fAdvanced + 0x15) * 0x2)+0x2a06]]) != 0x0 ? L_2c51 : L_2c04 */

L_2c04:
    if ((ish != 0xe))
        goto L_2c51;
    else
        goto L_2c0d;

L_2c0d:
    if ((fAdvanced == 0x0))
        goto L_2c35;
    else
        goto L_2c16;

L_2c16:
    /* untranslated: call FCreateAiShdef(ish, 0x16, &vrgMacAip[part[44:2](vrgMacIshAip)]) -> callresult(int16_t) */
    goto L_2c51;

L_2c35:
    /* untranslated: call FCreateAiShdef(ish, 0x15, &vrgMacAip[part[46:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_2c51:
    ish = (ish + 0x1);

L_2c55:
    if ((ish <= 0xf))
        goto L_2b4d;
    else
        goto L_2c5e;

L_2c5e:
    /* untranslated: branch ((part[1887:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_2d14 : L_2c71 */

L_2c71:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0x5))
        goto L_2d14;
    else
        goto L_2c89;

L_2c89:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x6))
        goto L_2d14;
    else
        goto L_2ca1;

L_2ca1:
    /* untranslated: branch FCreateAiShdef(0xc, 0x1d, &vrgMacAip[part[60:2](vrgMacIshAip)]) != 0x0 ? L_2d14 : L_2cc6 */

L_2cc6:
    i = 0x0;
    goto L_2d0b;

L_2cce:
    /* untranslated: branch FCreateAiShdef(0xc, 0x6, &vrgMacAip[0x15:[(Random(0x4) * 0x2)+0x2a06]]) != 0x0 ? L_2d14 : L_2d01 */

L_2d01:

L_2d07:
    i = (i + 0x1);

L_2d0b:
    if ((i < 0x5))
        goto L_2cce;
    else
        goto L_2d14;

L_2d14:
    /* untranslated: branch ((part[2034:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_2dcd : L_2d27 */

L_2d27:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xa))
        goto L_2dcd;
    else
        goto L_2d3f;

L_2d3f:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x9))
        goto L_2dcd;
    else
        goto L_2d57;

L_2d57:
    /* untranslated: branch FCreateAiShdef(0xd, 0x1d, &vrgMacAip[part[60:2](vrgMacIshAip)]) != 0x0 ? L_2dcd : L_2d7c */

L_2d7c:
    i = 0x0;
    goto L_2dc4;

L_2d84:
    /* untranslated: branch FCreateAiShdef(0xd, 0x6, &vrgMacAip[0x15:[((Random(0x4) + 0x4) * 0x2)+0x2a06]]) != 0x0 ? L_2dcd : L_2dba */

L_2dba:

L_2dc0:
    i = (i + 0x1);

L_2dc4:
    if ((i < 0x5))
        goto L_2d84;
    else
        goto L_2dcd;

L_2dcd:
    /* untranslated: branch ((part[1593:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_2e22 : L_2de0 */

L_2de0:
    /* untranslated: branch FCreateAiShdef(0xa, 0x2, &vrgMacAip[part[48:2](vrgMacIshAip)]) != 0x0 ? L_2e22 : L_2e05 */

L_2e05:
    /* untranslated: call FCreateAiShdef(0xa, 0x1, &vrgMacAip[part[48:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_2e22:
    /* untranslated: branch ((part[1740:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_2e52 : L_2e35 */

L_2e35:
    /* untranslated: call FCreateAiShdef(0xb, 0x2, &vrgMacAip[part[48:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_2e52:
    if ((game.turn >= 0x14))
        goto L_2eb3;
    else
        goto L_2e5c;

L_2e5c:
    /* untranslated: branch ((part[417:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_2eb3 : L_2e6f */

L_2e6f:
    /* untranslated: branch part[425:2](rgshdef) != 0x0 ? L_2eb3 : L_2e79 */

L_2e79:
    /* untranslated: branch part[427:2](rgshdef) != 0x0 ? L_2eb3 : L_2e83 */

L_2e83:
    /* untranslated: shdef = part[294:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x2);

L_2eb3:
    ish = 0x2;
    goto L_2f54;

L_2ebb:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_2f50 : L_2ed9 */

L_2ed9:
    if ((ish == 0x2))
        goto L_2f00;
    else
        goto L_2ee2;

L_2ee2:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0x14))
        goto L_2f50;
    else
        goto L_2f00;

L_2f00:
    i = 0x0;
    goto L_2f47;

L_2f08:
    /* untranslated: branch FCreateAiShdef(ish, 0x7, &vrgMacAip[0x15:[((Random(0x4) + 0x19) * 0x2)+0x2a06]]) != 0x0 ? L_2f50 : L_2f3d */

L_2f3d:

L_2f43:
    i = (i + 0x1);

L_2f47:
    if ((i < 0x5))
        goto L_2f08;
    else
        goto L_2f50;

L_2f50:
    ish = (ish + 0x1);

L_2f54:
    if ((ish <= 0x4))
        goto L_2ebb;
    else
        goto L_2f5d;

L_2f5d:
    if ((game.turn >= 0x28))
        goto L_2f97;
    else
        goto L_2f67;

L_2f67:
    /* untranslated: branch ((part[1152:2](rgshdef) >> 0x9) & 0x1) == 0x0 ? L_2f97 : L_2f7a */

L_2f7a:
    /* untranslated: call FCreateAiShdef(0x7, 0xf, &vrgMacAip[part[40:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_2f97:
    if ((FLookupPartX(&(part), 0x1, 0xf) != 0x1))
        goto L_3035;
    else
        goto L_2fb3;

L_2fb3:
    /* untranslated: branch ((part[270:2](rgshdef) >> 0x9) & 0x1) != 0x0 ? L_3035 : L_2fc6 */

L_2fc6:
    /* untranslated: branch part[278:2](rgshdef) != 0x0 ? L_3035 : L_2fd0 */

L_2fd0:
    /* untranslated: branch part[280:2](rgshdef) != 0x0 ? L_3035 : L_2fda */

L_2fda:
    /* untranslated: branch (part[207:2](rgshdef) & 0xff) == 0xf ? L_3035 : L_2fe8 */

L_2fe8:
    /* untranslated: shdef = part[147:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x1);
    /* untranslated: call FCreateAiShdef(0x1, 0xf, &vrgMacAip[part[40:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_3035:
    ish = 0x5;
    goto L_316f;

L_303d:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_316b : L_305b */

L_305b:
    if ((ish == 0x5))
        goto L_30a5;
    else
        goto L_3064;

L_3064:
    /* untranslated: branch ((part[123:2](rgshdef[(ish - 0x1)]) >> 0x9) & 0x1) != 0x0 ? L_316b : L_3087 */

L_3087:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0x14))
        goto L_316b;
    else
        goto L_30a5;

L_30a5:
    if ((ish != 0x5))
        goto L_30b4;
    else
        goto L_30ae;

L_30ae:
    t_merge_30b7_0001 = 0xb;
    goto L_30b7;

L_30b4:
    t_merge_30b7_0001 = 0xf;

L_30b7:
    shBase = t_merge_30b7_0001;
    if ((ish != 0x7))
        goto L_30e3;
    else
        goto L_30c3;

L_30c3:
    if ((Random(0x2) == 0x0))
        goto L_30dd;
    else
        goto L_30d7;

L_30d7:
    t_merge_30e0_0001 = 0xb;
    goto L_30e0;

L_30dd:
    t_merge_30e0_0001 = 0xf;

L_30e0:
    shBase = t_merge_30e0_0001;

L_30e3:
    if ((Random(0x3) == 0x0))
        goto L_311b;
    else
        goto L_30f7;

L_30f7:
    /* untranslated: branch FCreateAiShdef(ish, 0x1d, &vrgMacAip[part[58:2](vrgMacIshAip)]) != 0x0 ? L_316b : L_311b */

L_311b:
    i = 0x0;
    goto L_3162;

L_3123:
    /* untranslated: branch FCreateAiShdef(ish, 0x9, &vrgMacAip[0x15:[((Random(0x4) + shBase) * 0x2)+0x2a06]]) != 0x0 ? L_316b : L_3158 */

L_3158:

L_315e:
    i = (i + 0x1);

L_3162:
    if ((i < 0x5))
        goto L_3123;
    else
        goto L_316b;

L_316b:
    ish = (ish + 0x1);

L_316f:
    if ((ish <= 0x7))
        goto L_303d;
    else
        goto L_3178;

L_3178:
    ish = 0x8;
    goto L_325b;

L_3180:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) == 0x0 ? L_3257 : L_319e */

L_319e:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x1]) < 0xe))
        goto L_3257;
    else
        goto L_31b6;

L_31b6:
    if ((ish == 0x8))
        goto L_3200;
    else
        goto L_31bf;

L_31bf:
    /* untranslated: branch ((part[123:2](rgshdef[(ish - 0x1)]) >> 0x9) & 0x1) != 0x0 ? L_3257 : L_31e2 */

L_31e2:
    if (((game.turn - rgshdef[(ish - 0x1)].turn) <= 0xf))
        goto L_3257;
    else
        goto L_3200;

L_3200:
    /* untranslated: branch FCreateAiShdef(ish, 0x9, &vrgMacAip[part[38:2](vrgMacIshAip)]) != 0x0 ? L_3257 : L_3224 */

L_3224:
    if ((ish != 0x8))
        goto L_3233;
    else
        goto L_322d;

L_322d:
    t_merge_3236_0001 = 0x8;
    goto L_3236;

L_3233:
    t_merge_3236_0001 = 0x9;

L_3236:
    /* untranslated: call FCreateAiShdef(ish, 0x13, &vrgMacAip[0x15:[(t_merge_3236_0001 * 0x2)+0x2a06]]) -> callresult(int16_t) */

L_3257:
    ish = (ish + 0x1);

L_325b:
    if ((ish <= 0x9))
        goto L_3180;
    else
        goto L_3264;

L_3264:
    if ((LOWORD(rgshdef) == 0x5))
        goto L_3366;
    else
        goto L_326e;

L_326e:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_3366 : L_328d */

L_328d:
    /* untranslated: branch part[131:2](rgshdef) != 0x0 ? L_3366 : L_3297 */

L_3297:
    /* untranslated: branch part[133:2](rgshdef) != 0x0 ? L_3366 : L_32a1 */

L_32a1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x5]) < 0x4))
        goto L_3366;
    else
        goto L_32b9;

L_32b9:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x4]) < 0x5))
        goto L_3366;
    else
        goto L_32d1;

L_32d1:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x3]) < 0x6))
        goto L_3366;
    else
        goto L_32e9;

L_32e9:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x2]) < 0x6))
        goto L_3366;
    else
        goto L_3301;

L_3301:
    if (((uint16_t)(rgplr[idPlayer].rgTech[0x0]) < 0x6))
        goto L_3366;
    else
        goto L_3319;

L_3319:
    /* untranslated: shdef = part[0:147](rgshdef) */
    shdef.fFree = 0x1;
    FChangeAiShdef(&(shdef), 0x0);
    /* untranslated: call FCreateAiShdef(0x0, 0x5, &vrgMacAip[part[20:2](vrgMacIshAip)]) -> callresult(int16_t) */

L_3366:
    return;
}

int16_t FRetargetMiner(FLEET *lpfl) {
    int16_t cConc;
    ORDER   ord;
    int16_t cConcBest;
    PLANET *lppl;
    int16_t cConcCur;
    int16_t ipl;
    PLANET *lpplBest;

L_336c:
    lpplBest = 0x0;
    cConcCur = 0x0;
    cConcBest = 0x0;
    ipl = 0x0;
    goto L_3395;

L_3391:
    ipl = (ipl + 0x1);

L_3395:
    if ((ipl >= vclpplAi))
        goto L_3487;
    else
        goto L_33a0;

L_33a0:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_33d0;
    else
        goto L_33c8;

L_33c8:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_3487;
    else
        goto L_33d0;

L_33d0:
    /* untranslated: branch hiword(LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x))) > 0x0 ? L_3391 : L_33fd */

L_33fd:
    /* untranslated: branch hiword(callresult(int32_t)) < 0x0 ? L_340a : L_3402 */

L_3402:
    /* untranslated: branch loword(callresult(int32_t)) >= 0x1440 ? L_3391 : L_340a */

L_340a:
    cConc = (((lppl->rgMinConc[0x0] * 0x8) + LOWORD((lppl->rgMinConc[0x1] * 0xa))) + LOWORD((lppl->rgMinConc[0x2] * 0x7)));
    if ((lppl->id != lpfl->idPlanet))
        goto L_3467;
    else
        goto L_3461;

L_3461:
    cConcCur = cConc;

L_3467:
    if ((cConc <= cConcBest))
        goto L_3391;
    else
        goto L_3472;

L_3472:
    lpplBest = lppl;
    cConcBest = cConc;

L_3487:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_3499;
    else
        goto L_3490;

L_3490:
    if ((HIWORD(lpplBest) == 0x0))
        goto L_34ad;
    else
        goto L_3499;

L_3499:
    /* untranslated: branch (words(loword((0x6 * cConcCur)), signhiword(loword((0x6 * cConcCur)))) / 0x5) < cConcBest ? L_34b3 : L_34ad */

L_34ad:
    return 0x0;

L_34b3:
    ord.id = lpplBest->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplBest->id].x;
    ord.pt.y = rgptPlan[lpplBest->id].y;
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x6;
    return FMoveAiFleet(lpfl, &(ord), 0x0);
}

int16_t IdTargetMacFreighter(FLEET *lpfl) {
    int32_t cMax;
    int32_t cColLeft;
    int16_t cResGainMost;
    int32_t cColHaul;
    int16_t cResGain;
    ORDER   ord;
    PLANET *lpplHere;
    int16_t pctCapMost;
    int16_t pctCapHere;
    int16_t cResLost;
    PLANET *lppl;
    int16_t pctKilled;
    int16_t i;
    int32_t lDist;
    int16_t ipl;
    PLANET *lpplBest;
    int16_t iM;

L_3524:
    if ((lpfl->idPlanet != 0xffff))
        goto L_3547;
    else
        goto L_353a;

L_353a:
    lpplHere = 0x0;
    goto LFindPickup;

L_3547:
    lpplHere = LpplFromId(lpfl->idPlanet);
    if ((LOWORD(lpplHere) != 0x0))
        goto L_356e;
    else
        goto L_3565;

L_3565:
    if ((HIWORD(lpplHere) == 0x0))
        goto LFindPickup;
    else
        goto L_356e;

L_356e:
    if ((lpplHere->iPlayer != idPlayer))
        goto LFindPickup;
    else
        goto L_357a;

L_357a:

L_3580:
    pctCapHere = PctPlanetCapacity(lpplHere);
    if ((pctCapHere <= 0x19))
        goto LFindPickup;
    else
        goto L_359a;

L_359a:
    if ((HIWORD(lpplHere->rgwtMin[0x3]) > 0x0))
        goto L_35ba;
    else
        goto L_35a7;

L_35a7:
    if ((HIWORD(lpplHere->rgwtMin[0x3]) < 0x0))
        goto LFindPickup;
    else
        goto L_35ac;

L_35ac:
    if ((LOWORD(lpplHere->rgwtMin[0x3]) < 0x3e8))
        goto LFindPickup;
    else
        goto L_35b4;

L_35b4:

L_35ba:
    cColHaul = LGetFleetStat(lpfl, 0x2);
    i = 0x0;
    goto L_3601;

L_35da:
    cColHaul = (cColHaul - lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_3601:
    if ((i <= 0x2))
        goto L_35da;
    else
        goto L_360a;

L_360a:
    cMax = (int32_t)((lpplHere->rgwtMin[0x3] / 0x14));
    if ((HIWORD(cColHaul) < HIWORD(cMax)))
        goto L_364f;
    else
        goto L_3636;

L_3636:
    if ((HIWORD(cColHaul) > HIWORD(cMax)))
        goto L_3643;
    else
        goto L_363b;

L_363b:
    if ((LOWORD(cColHaul) <= LOWORD(cMax)))
        goto L_364f;
    else
        goto L_3643;

L_3643:
    cColHaul = cMax;

L_364f:
    if ((HIWORD(cColHaul) > 0x0))
        goto L_3669;
    else
        goto L_3658;

L_3658:
    if ((HIWORD(cColHaul) < 0x0))
        goto LFindPickup;
    else
        goto L_365d;

L_365d:
    if ((LOWORD(cColHaul) <= 0x0))
        goto LFindPickup;
    else
        goto L_3663;

L_3663:

L_3669:
    cResLost = CResourcesAtPlanet(lpplHere, idPlayer);
    lpplHere->rgwtMin[0x3] = (lpplHere->rgwtMin[0x3] - cColHaul);
    cResLost = (cResLost - CResourcesAtPlanet(lpplHere, idPlayer));
    lpplHere->rgwtMin[0x3] = (lpplHere->rgwtMin[0x3] + cColHaul);
    lpplBest = 0x0;
    cResGainMost = 0x0;
    ipl = 0x0;
    goto L_36d0;

L_36cc:
    ipl = (ipl + 0x1);

L_36d0:
    if ((ipl >= vclpplAi))
        goto L_389c;
    else
        goto L_36db;

L_36db:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_370b;
    else
        goto L_3703;

L_3703:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_389c;
    else
        goto L_370b;

L_370b:
    if ((LOWORD(lppl) != LOWORD(lpplHere)))
        goto L_3724;
    else
        goto L_3719;

L_3719:
    if ((HIWORD(lppl) == HIWORD(lpplHere)))
        goto L_36cc;
    else
        goto L_371e;

L_371e:

L_3724:
    if (((vlpbAiPlanet[((lppl->id * 0x10) + 0xe)] & 0x1) != 0x0))
        goto L_36cc;
    else
        goto L_374f;

L_374f:

L_3755:
    /* untranslated: lDist = LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x)) */
    if ((HIWORD(lDist) < 0x0))
        goto L_379b;
    else
        goto L_3789;

L_3789:
    if ((HIWORD(lDist) > 0x0))
        goto L_36cc;
    else
        goto L_378e;

L_378e:
    if ((LOWORD(lDist) > 0x9c40))
        goto L_36cc;
    else
        goto L_3795;

L_3795:

L_379b:
    if ((HIWORD(lDist) < 0x0))
        goto L_37bb;
    else
        goto L_37a4;

L_37a4:
    if ((HIWORD(lDist) > 0x0))
        goto L_37b3;
    else
        goto L_37a9;

L_37a9:
    if ((LOWORD(lDist) <= 0x57e4))
        goto L_37bb;
    else
        goto L_37b3;

L_37b3:
    pctKilled = 0xc;
    goto L_3800;

L_37bb:
    if ((HIWORD(lDist) < 0x0))
        goto L_37db;
    else
        goto L_37c4;

L_37c4:
    if ((HIWORD(lDist) > 0x0))
        goto L_37d3;
    else
        goto L_37c9;

L_37c9:
    if ((LOWORD(lDist) <= 0x2710))
        goto L_37db;
    else
        goto L_37d3;

L_37d3:
    pctKilled = 0x9;
    goto L_3800;

L_37db:
    if ((HIWORD(lDist) < 0x0))
        goto L_37fb;
    else
        goto L_37e4;

L_37e4:
    if ((HIWORD(lDist) > 0x0))
        goto L_37f3;
    else
        goto L_37e9;

L_37e9:
    if ((LOWORD(lDist) <= 0x9c4))
        goto L_37fb;
    else
        goto L_37f3;

L_37f3:
    pctKilled = 0x6;
    goto L_3800;

L_37fb:
    pctKilled = 0x3;

L_3800:
    cColLeft = (cColHaul - (int32_t)(((uint32_t)((cColHaul * (uint32_t)(pctKilled))) / 0x64)));
    lppl->rgwtMin[0x3] = (lppl->rgwtMin[0x3] + cColLeft);
    cResGain = CResourcesAtPlanet(lppl, idPlayer);
    lppl->rgwtMin[0x3] = (lppl->rgwtMin[0x3] - cColLeft);
    cResGain = (cResGain - CResourcesAtPlanet(lppl, idPlayer));
    if ((cResGain <= cResGainMost))
        goto L_36cc;
    else
        goto L_3887;

L_3887:
    cResGainMost = cResGain;
    lpplBest = lppl;

L_389c:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_38ae;
    else
        goto L_38a5;

L_38a5:
    if ((HIWORD(lpplBest) == 0x0))
        goto LFindPickup;
    else
        goto L_38ae;

L_38ae:
    if ((cResGainMost < (cResLost + 0x5)))
        goto LFindPickup;
    else
        goto L_38bc;

L_38bc:
    if ((cResGainMost >= (cResLost + 0xa)))
        goto L_38d4;
    else
        goto L_38ca;

L_38ca:
    if ((game.turn > 0x50))
        goto LFindPickup;
    else
        goto L_38d4;

L_38d4:
    if ((cResGainMost >= (cResLost + 0xf)))
        goto L_38f0;
    else
        goto L_38e2;

L_38e2:
    if ((game.turn > 0xa0))
        goto LFindPickup;
    else
        goto L_38ea;

L_38ea:

L_38f0:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), 0x3, LOWORD(cColHaul));
    FLookupFleet(LOWORD(lpfl), 0x4972);
    vlpbAiPlanet[((lpplBest->id * 0x10) + 0xe)] = (vlpbAiPlanet[((lpplBest->id * 0x10) + 0xe)] | 0x1);

LMoveToLpplBest:
    memset(ord, 0x0, 0x12);
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplBest->id].x;
    ord.pt.y = rgptPlan[lpplBest->id].y;
    ord.id = lpplBest->id;
    ord.grTask = grTaskXfer;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    i = 0x0;
    goto L_39ec;

L_39ca:
    /* untranslated: ss:[bp+(i * 0x2)-0x1c] = ((ss:[bp+(i * 0x2)-0x1c] & 0xfff) | 0x2000) */
    i = (i + 0x1);

L_39ec:
    if ((i <= 0x3))
        goto L_39ca;
    else
        goto L_39f5;

L_39f5:
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) != 0x0))
        goto L_3a19;
    else
        goto L_3a13;

L_3a13:
    return 0xffff;

L_3a19:
    return lpplBest->id;

LFindPickup:
    if ((LOWORD(lpplHere) != 0x0))
        goto L_3a34;
    else
        goto L_3a2b;

L_3a2b:
    if ((HIWORD(lpplHere) == 0x0))
        goto L_3cd8;
    else
        goto L_3a34;

L_3a34:
    iM = 0x0;
    goto L_3a72;

L_3a3c:
    if ((HIWORD(lpplHere->rgwtMin[iM]) < 0x0))
        goto L_3a6e;
    else
        goto L_3a5c;

L_3a5c:
    if ((HIWORD(lpplHere->rgwtMin[iM]) > 0x0))
        goto L_3a7b;
    else
        goto L_3a61;

L_3a61:
    if ((LOWORD(lpplHere->rgwtMin[iM]) >= 0x9c4))
        goto L_3a7b;
    else
        goto L_3a68;

L_3a68:

L_3a6e:
    iM = (iM + 0x1);

L_3a72:
    if ((iM <= 0x2))
        goto L_3a3c;
    else
        goto L_3a7b;

L_3a7b:
    if ((iM > 0x2))
        goto L_3cd8;
    else
        goto L_3a84;

L_3a84:
    lpplBest = 0x0;
    cMax = 0x3e8;
    ipl = 0x0;
    goto L_3aa4;

L_3aa0:
    ipl = (ipl + 0x1);

L_3aa4:
    if ((ipl >= vclpplAi))
        goto L_3ba2;
    else
        goto L_3aaf;

L_3aaf:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_3adf;
    else
        goto L_3ad7;

L_3ad7:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_3ba2;
    else
        goto L_3adf;

L_3adf:
    if ((LOWORD(lppl) != LOWORD(lpplHere)))
        goto L_3af8;
    else
        goto L_3aed;

L_3aed:
    if ((HIWORD(lppl) == HIWORD(lpplHere)))
        goto L_3aa0;
    else
        goto L_3af2;

L_3af2:

L_3af8:
    /* untranslated: lDist = LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x)) */
    if ((HIWORD(lDist) < 0x0))
        goto L_3b3e;
    else
        goto L_3b2c;

L_3b2c:
    if ((HIWORD(lDist) > 0x0))
        goto L_3aa0;
    else
        goto L_3b31;

L_3b31:
    if ((LOWORD(lDist) > 0x9c40))
        goto L_3aa0;
    else
        goto L_3b38;

L_3b38:

L_3b3e:
    if ((HIWORD(lppl->rgwtMin[iM]) > HIWORD(cMax)))
        goto L_3aa0;
    else
        goto L_3b63;

L_3b63:
    if ((HIWORD(lppl->rgwtMin[iM]) < HIWORD(cMax)))
        goto L_3b70;
    else
        goto L_3b68;

L_3b68:
    if ((LOWORD(lppl->rgwtMin[iM]) >= LOWORD(cMax)))
        goto L_3aa0;
    else
        goto L_3b70;

L_3b70:
    cMax = lppl->rgwtMin[iM];
    lpplBest = lppl;

L_3ba2:
    if ((LOWORD(lpplBest) != 0x0))
        goto L_3bb4;
    else
        goto L_3bab;

L_3bab:
    if ((HIWORD(lpplBest) == 0x0))
        goto L_3cd8;
    else
        goto L_3bb4;

L_3bb4:
    if ((HIWORD(cMax) > 0x0))
        goto L_3cd8;
    else
        goto L_3bbd;

L_3bbd:
    if ((HIWORD(cMax) < 0x0))
        goto L_3bcc;
    else
        goto L_3bc2;

L_3bc2:
    if ((LOWORD(cMax) >= 0xc8))
        goto L_3cd8;
    else
        goto L_3bcc;

L_3bcc:
    cColHaul = LGetFleetStat(lpfl, 0x2);
    i = 0x0;
    goto L_3c13;

L_3bec:
    cColHaul = (cColHaul - lpfl->rgwtMin[i]);
    i = (i + 0x1);

L_3c13:
    if ((i <= 0x2))
        goto L_3bec;
    else
        goto L_3c1c;

L_3c1c:
    cMax = (int32_t)((lpplHere->rgwtMin[iM] / 0x5));
    if ((HIWORD(cColHaul) < HIWORD(cMax)))
        goto L_3c73;
    else
        goto L_3c5a;

L_3c5a:
    if ((HIWORD(cColHaul) > HIWORD(cMax)))
        goto L_3c67;
    else
        goto L_3c5f;

L_3c5f:
    if ((LOWORD(cColHaul) <= LOWORD(cMax)))
        goto L_3c73;
    else
        goto L_3c67;

L_3c67:
    cColHaul = cMax;

L_3c73:
    if ((HIWORD(cColHaul) < 0x0))
        goto L_3c9c;
    else
        goto L_3c7c;

L_3c7c:
    if ((HIWORD(cColHaul) > 0x0))
        goto L_3c8a;
    else
        goto L_3c81;

L_3c81:
    if ((LOWORD(cColHaul) <= 0x0))
        goto L_3c9c;
    else
        goto L_3c8a;

L_3c8a:
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));

L_3c9c:
    XferAiSupply(grobjPlanet, lpfl->idPlanet, grobjFleet, LOWORD(lpfl), iM, LOWORD(cColHaul));
    FLookupFleet(LOWORD(lpfl), 0x4972);
    goto LMoveToLpplBest;

L_3cd8:
    lpplBest = 0x0;
    pctCapMost = 0x0;
    ipl = 0x0;
    goto L_3cf3;

L_3cef:
    ipl = (ipl + 0x1);

L_3cf3:
    if ((ipl >= vclpplAi))
        goto L_3e18;
    else
        goto L_3cfe;

L_3cfe:
    lppl = vrglpplAi[ipl];
    if ((LOWORD(vrglpplAi[ipl]) != 0x0))
        goto L_3d2e;
    else
        goto L_3d26;

L_3d26:
    if ((*(vrglpplAi[ipl] + 0x2) == 0x0))
        goto L_3e18;
    else
        goto L_3d2e;

L_3d2e:
    if ((LOWORD(lppl) != LOWORD(lpplHere)))
        goto L_3d47;
    else
        goto L_3d3c;

L_3d3c:
    if ((HIWORD(lppl) == HIWORD(lpplHere)))
        goto L_3cef;
    else
        goto L_3d41;

L_3d41:

L_3d47:
    /* untranslated: lDist = LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(rgptPlan[lppl->id].y, rgptPlan[lppl->id].x)) */
    if ((HIWORD(lDist) < 0x0))
        goto L_3d8d;
    else
        goto L_3d7b;

L_3d7b:
    if ((HIWORD(lDist) > 0x0))
        goto L_3cef;
    else
        goto L_3d80;

L_3d80:
    if ((LOWORD(lDist) > 0x9c40))
        goto L_3cef;
    else
        goto L_3d87;

L_3d87:

L_3d8d:
    pctCapHere = PctPlanetCapacity(lppl);
    if ((HIWORD(lDist) < 0x0))
        goto L_3dbd;
    else
        goto L_3da7;

L_3da7:
    if ((HIWORD(lDist) > 0x0))
        goto L_3db6;
    else
        goto L_3dac;

L_3dac:
    if ((LOWORD(lDist) <= 0x57e4))
        goto L_3dbd;
    else
        goto L_3db6;

L_3db6:
    pctCapHere = (pctCapHere - 0x6);
    goto L_3df8;

L_3dbd:
    if ((HIWORD(lDist) < 0x0))
        goto L_3ddc;
    else
        goto L_3dc6;

L_3dc6:
    if ((HIWORD(lDist) > 0x0))
        goto L_3dd5;
    else
        goto L_3dcb;

L_3dcb:
    if ((LOWORD(lDist) <= 0x2710))
        goto L_3ddc;
    else
        goto L_3dd5;

L_3dd5:
    pctCapHere = (pctCapHere - 0x4);
    goto L_3df8;

L_3ddc:
    if ((HIWORD(lDist) < 0x0))
        goto L_3df8;
    else
        goto L_3de5;

L_3de5:
    if ((HIWORD(lDist) > 0x0))
        goto L_3df4;
    else
        goto L_3dea;

L_3dea:
    if ((LOWORD(lDist) <= 0x9c4))
        goto L_3df8;
    else
        goto L_3df4;

L_3df4:
    pctCapHere = (pctCapHere - 0x2);

L_3df8:
    if ((pctCapMost >= pctCapHere))
        goto L_3cef;
    else
        goto L_3e03;

L_3e03:
    lpplBest = lppl;
    pctCapMost = pctCapHere;

L_3e18:
    if ((LOWORD(lpplBest) != 0x0))
        goto LMoveToLpplBest;
    else
        goto L_3e21;

L_3e21:
    if ((HIWORD(lpplBest) != 0x0))
        goto LMoveToLpplBest;
    else
        goto L_3e27;

L_3e27:

L_3e2d:
    return 0xffff;
}

void TargetMacArmada(FLEET *lpfl) {
    FLEET  *lpflTarget;
    ORDER   ord;
    int16_t cshBomb;
    PLANET *lppl;
    int16_t cshWar;
    PLANET *lpplTarget;

L_3e3a:
    if ((lpfl->cord <= 0x1))
        goto LTryNewTarget;
    else
        goto L_3e50;

L_3e50:
    ord = lpfl->lpplord->rgord[0x1];
    /* untranslated: branch hiword(LDistance2(words(lpfl->pt.y, lpfl->pt.x), words(ord.pt.y, ord.pt.x))) < 0x0 ? L_3eb7 : L_3e94 */

L_3e94:
    /* untranslated: branch hiword(callresult(int32_t)) > 0x0 ? L_3ea1 : L_3e99 */

L_3e99:
    /* untranslated: branch loword(callresult(int32_t)) <= 0xf424 ? L_3eb7 : L_3ea1 */

L_3ea1:
    if ((ord.grobj == grobjFleet))
        goto LTryNewTarget;
    else
        goto L_3eb1;

L_3eb1:

L_3eb7:
    if ((ord.grobj == grobjFleet))
        goto L_42e5;
    else
        goto L_3ec7;

L_3ec7:

L_3ecd:
    if ((ord.grobj != grobjPlanet))
        goto LTryNewTarget;
    else
        goto L_3ee0;

L_3ee0:
    lppl = LpplFromId(ord.id);
    if ((LOWORD(lppl) != 0x0))
        goto L_3f03;
    else
        goto L_3efa;

L_3efa:
    if ((HIWORD(lppl) == 0x0))
        goto L_42e5;
    else
        goto L_3f03;

L_3f03:
    if ((lppl->iPlayer == 0xffff))
        goto L_3f36;
    else
        goto L_3f10;

L_3f10:
    if ((lppl->iPlayer != idPlayer))
        goto L_42e5;
    else
        goto L_3f1f;

L_3f1f:
    if ((lppl->fStarbase != 0x0))
        goto L_42e5;
    else
        goto L_3f36;

L_3f36:
    if ((lppl->turn != game.turn))
        goto L_42e5;
    else
        goto L_3f43;

L_3f43:

LTryNewTarget:
    FPotentMacWarFleet(lpfl, &(cshWar));
    cshBomb = (lpfl->rgcsh[0x8] + lpfl->rgcsh[0x9]);
    lpfl->fMark = 0x1;
    ChangeMainObjSel(grobjFleet, LOWORD(lpfl));
    if ((lpfl->idPlanet != 0xffff))
        goto L_3fb4;
    else
        goto L_3f9f;

L_3f9f:
    MoveToNearestPlanetOrEnemy(lpfl, 0x1c2);
    goto L_42e5;

L_3fb4:
    lppl = LpplFromId(lpfl->idPlanet);
    if ((lppl->iPlayer != idPlayer))
        goto L_41f9;
    else
        goto L_3fd8;

L_3fd8:
    if ((cshWar < (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_4163;
    else
        goto L_3fe6;

L_3fe6:
    if ((cshBomb < (HIWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_4163;
    else
        goto TargetPotentArmada;

TargetPotentArmada:
    if ((game.fAisBand == 0x0))
        goto L_4029;
    else
        goto L_400a;

L_400a:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaHumanDest);
    goto L_4033;

L_4029:
    lpplTarget = 0x0;

L_4033:
    if ((LOWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_403c;

L_403c:
    if ((HIWORD(lpplTarget) != 0x0))
        goto TargetEveryArmada;
    else
        goto L_4045;

L_4045:
    lpplTarget = LpplFindBestEnum(lppl, FEnumCalcArmadaDest);

TargetEveryArmada:
    if ((LOWORD(lpplTarget) != 0x0))
        goto L_4073;
    else
        goto L_406a;

L_406a:
    if ((HIWORD(lpplTarget) == 0x0))
        goto L_4109;
    else
        goto L_4073;

L_4073:
    vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] = (vlpbAiPlanet[((lpplTarget->id * 0x10) + 0xa)] | 0x80);
    ord.id = lpplTarget->id;
    ord.grobj = grobjPlanet;
    ord.pt.x = rgptPlan[lpplTarget->id].x;
    ord.pt.y = rgptPlan[lpplTarget->id].y;

FinishTargeting:
    ord.grTask = grTaskNone;
    ord.fValidTask = 0x1;
    ord.iWarp = 0x4;
    if ((FMoveAiFleet(lpfl, &(ord), 0x0) == 0x0))
        goto L_42e5;
    else
        goto L_4100;

L_4100:

L_4109:
    lpflTarget = LpflFindClosestEnum(lpfl, FEnumCalcEnemyFleets);
    if ((LOWORD(lpflTarget) != 0x0))
        goto L_4137;
    else
        goto L_412e;

L_412e:
    if ((HIWORD(lpflTarget) == 0x0))
        goto L_42e5;
    else
        goto L_4137;

L_4137:
    ord.id = LOWORD(lpflTarget);
    ord.grobj = grobjFleet;
    ord.pt.x = lpflTarget->pt.x;
    ord.pt.y = lpflTarget->pt.y;
    goto FinishTargeting;

L_4163:
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_42e5 : L_4182 */

L_4182:
    if ((cshWar > ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_419b;
    else
        goto L_4192;

L_4192:
    if ((cshWar < 0x3c))
        goto L_42e5;
    else
        goto L_419b;

L_419b:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_41af;

L_41af:
    if ((cshWar <= LOWORD(((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x3))))
        goto L_41d6;
    else
        goto L_41c2;

L_41c2:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_41d6;

L_41d6:
    if ((cshWar <= 0x78))
        goto L_42e5;
    else
        goto L_41df;

L_41df:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_41f0;

L_41f0:

L_41f9:
    /* untranslated: branch cshWar < (part[1:2](vrgAiArmadaPotency) & 0xff) ? L_4215 : L_4207 */

L_4207:
    /* untranslated: branch cshBomb >= (part[3:2](vrgAiArmadaPotency) & 0xff) ? L_42d2 : L_4215 */

L_4215:
    ClearAiCurrentTask(lpfl, 0x0);
    /* untranslated: branch ((part[6:2](rgplr[idPlayer]) >> 0xa) & 0x7) <= 0x1 ? L_42b0 : L_4246 */

L_4246:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x2)))
        goto L_426a;
    else
        goto L_4256;

L_4256:
    if ((Random(0xa) < 0x5))
        goto TargetPotentArmada;
    else
        goto L_426a;

L_426a:
    if ((cshWar <= ((LOWORD(vrgAiArmadaPotency) & 0xff) * 0x4)))
        goto L_4290;
    else
        goto L_427c;

L_427c:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_4290;

L_4290:
    if ((cshWar <= 0x78))
        goto L_42b0;
    else
        goto L_4299;

L_4299:
    if ((Random(0xa) < 0x7))
        goto TargetPotentArmada;
    else
        goto L_42aa;

L_42aa:

L_42b0:
    lpplTarget = LpplFindClosestEnum(lppl, FEnumOurStarbase);
    goto TargetEveryArmada;

L_42d2:
    if ((lppl->iPlayer == 0xffff))
        goto TargetPotentArmada;
    else
        goto L_42dc;

L_42dc:

L_42e5:
    return;
}

int16_t FPotentMacWarFleet(FLEET *lpfl, int16_t *pcEquiv) {
    int16_t ish;
    int16_t cEquiv;

L_42ec:
    cEquiv = 0x0;
    ish = 0x2;
    goto L_4320;

L_4302:
    cEquiv = (cEquiv + lpfl->rgcsh[ish]);
    ish = (ish + 0x1);

L_4320:
    if ((ish <= 0x4))
        goto L_4302;
    else
        goto L_4329;

L_4329:
    ish = 0x5;
    goto L_4351;

L_4331:
    cEquiv = (cEquiv + (lpfl->rgcsh[ish] * 0x2));
    ish = (ish + 0x1);

L_4351:
    if ((ish <= 0x7))
        goto L_4331;
    else
        goto L_435a;

L_435a:
    if ((cEquiv >= (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto Success;
    else
        goto L_4365;

L_4365:

L_436b:
    ish = 0x8;
    goto L_43e0;

L_4373:
    if ((lpfl->rgcsh[ish] == 0x0))
        goto L_43dc;
    else
        goto L_4390;

L_4390:
    /* untranslated: branch ((part[123:2](rgshdef[ish]) >> 0x9) & 0x1) != 0x0 ? L_43dc : L_43ae */

L_43ae:
    if ((rgshdef[ish].hul.ihuldef != ihuldefBattleship))
        goto L_43dc;
    else
        goto L_43c0;

L_43c0:
    cEquiv = (cEquiv + (lpfl->rgcsh[ish] * 0x2));

L_43dc:
    ish = (ish + 0x1);

L_43e0:
    if ((ish <= 0x9))
        goto L_4373;
    else
        goto L_43e9;

L_43e9:
    if ((cEquiv <= (LOWORD(vrgAiArmadaPotency) & 0xff)))
        goto L_440e;
    else
        goto Success;

Success:
    if ((pcEquiv == 0x0))
        goto L_4408;
    else
        goto L_4400;

L_4400:
    *(pcEquiv) = cEquiv;

L_4408:
    return 0x1;

L_440e:
    return 0x0;
}
